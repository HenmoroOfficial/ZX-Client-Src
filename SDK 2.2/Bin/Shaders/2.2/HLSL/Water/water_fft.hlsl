
#ifdef _ANGELICA_2_1
#include "common_fog.hlslvh"
float4 ComputeFog(float3 eyetopos, float y, float4x4 params)
{
	return ComputeCompositeFog(eyetopos, y, params._m00, params._m10, params._m20, params._m30, 
	params._m02_m12_m22_m32, params._m03_m13_m23_m33, params._m01, params._m11, params._m21, params._m31);
}
#else
#include "..\fog\fog_common.h"
float4 ComputeFog(float3 eyetopos, float y, float4x4 params)
{
	return ComputeCompositeFogSimple(eyetopos, y);
}
string entrynames = "ps_3_0 ps; vs_3_0 vs";	

string macro1
<
	string name = "ENABLE_RAIN";
	string desc = "是否开启雨波";
	string type = "PRG";
	string GroupID = "2";
>;

string macro2
<
	string name = "ENABLE_CAUSTIC";
	string desc = "是否开焦散";
	string type = "PRG";
	string GroupID = "3";
>;

// string macro3
// <
// 	string name = "ENABLE_HIGH_REFRA";
// 	string desc = "是否开色差";
// 	string type = "PRG";
// 	string GroupID = "4";
// >;

// string macro4
// <
// 	string name = "ENABLE_FFT";
// 	string desc = "是否开FFT波动";
// 	string type = "PRG";
// 	string GroupID = "5";
// >;
#define ENABLE_FFT 1

string macro5
<
	string name = "ENABLE_WAVE";
	string desc = "是否开波动";
	string type = "PRG";
	string GroupID = "6";
>;

string macro6
<
	string name = "ENABLE_FOG";
	string desc = "是否开雾";
	string type = "PRG";
	string GroupID = "7";
>;

// string macro7
// <
// 	string name = "ENABLE_BLOOM";
// 	string desc = "是否开BLOOM";
// 	string type = "PRG";
// 	string GroupID = "8";
// >;

#endif

#include "water_fft_common.h"

float4x4 g_mViewProj; 		//ViewProj矩阵    view projection matrix
float4x4 g_mReflProj;    
//应该就是当前摄像机的view * proj * 0-1偏移. Crysis并不是每帧都更新这个纹理, 传入的ReflProj可能并不是当前帧的ViewProj矩阵
//搜索eRTUpdate_WaterReflect, 综合考虑了摄像机朝向和位置,以及上一帧的更新时间
//搜索FRT_CAMERA_REFLECTED_WATERPLANE查看渲染Reflection的过程

float g_fWaterLevelAtCamera;	//摄像机处的水面高度
float g_fWaterLevel;			//全局水面高度
								//float4 g_fWaterLevel;		//水面高度 //m_fWaterLevel, m_fWaterLevel, fWaterHeightAtCameraPos
//float g_bUseFFT;				//是否使用FFT纹理偏移顶点

float3 g_PS_SunLightDir; 	//太阳光方向 = float4(1,1,1);
float3 g_PS_SkyColor; 		//天空颜色 = float3(0.2, 0.3, 0.8);
float3 g_PS_SunColor; 		//太阳颜色 = float3(1, 1, 1);

// water waves parameters    
float g_fWindSpeed;
float g_fWaveSpeed;
float g_fWaveAmount;

float g_fWaveSize;
float g_fDirCos;
float g_fDirSin;

float4 g_vNearFarDis;					//vs ps 公用
float3 g_vCameraPos;					//vs ps 公用

float2 g_vOceanMeshRot;
float g_fTime;

float4 g_vScreenSize;

//用户设置的参数:
float g_fSoftIntersectionFactor = 1.0;	//"Controls softness of water surface intersection with other opaque scene geometry";       0~10
float g_fReflectionAmount = 0.5;		//"Set reflection scale";     0~10
float g_fWhiteCapsAmount = 0.75;		//"Set white caps";      0~2   
float g_fSunMultiplier = 12.0;			//"g_fSunMultiplier" 0~32
float g_fFresnelScale = 1.0;			// 0~32
float g_fEdgeFoamAmount = 1.0;

//float g_bEnableNew = 0;				//vs ps 公用
float g_bEdgeFoam = 1;
float g_bWaveFoam= 1;
float g_bWaveGloss = 1;
float g_bCubeRefl = 0;
//float g_bRain = 1;
//float g_bCaustic = 1;



float4x4 fogParamsP			:	register(c28);

float3 g_waterFogColor;
float g_fFogDensity = 0.8f;
float g_fWaterFogScaleExp = 5;

float g_fRainLerp = 0.f;

float g_bEnableArea;
float4 g_waterArea;   //xy: minXZ; zw: maxXZ

float2 g_vCurrentSmallWaterStart : register(c32);
float4 g_vSmallWaterWidth; //x: smallWaterWidth  y: 衰减距离 z：最远距离

sampler2D	fftTexSampler 			: register(s0);		//POINT Wrap
sampler2D	oceanBumpMapSampler 	: register(s4);		//LINEAR Wrap
sampler2D	foamSampler 			: register(s1);		//LINEAR Wrap MipFilter = POINT;
sampler2D	waterGlossMapSampler 	: register(s2);		//LINEAR Wrap
sampler2D	_causticSampler			: register(s3);		//LINEAR Wrap MipFilter = POINT;
sampler2D	envMapSamplerRefl		: register(s5);		//LINEAR Clamp
sampler2D	envMapSamplerRefr		: register(s6);		//POINT Clamp
sampler2D	sceneDepthSampler		: register(s7);		//POINT Clamp
sampler2D	_causticRTSampler		: register(s8);		//LINEAR Wrap MipFilter = POINT;
sampler2D	rainSampler1				: register(s9);
sampler2D   rainSampler2 				: register(s10);
samplerCUBE g_CubeReflectSampler : register(s11);
struct a2vWater
{
	float4 Position : POSITION;
	float2 TexCoord : TEXCOORD0;
};

struct v2fWater
{
  float4 Position  : POSITION;   // in projection space
  float4 foamTC    : TEXCOORD0;  // foam coordinates
  float4 bumpTC     : TEXCOORD1;  // wz if next frequency wave x2
  float4 screenProj : TEXCOORD2; // refraction projection
  float4 envTC      : TEXCOORD3; // reflection projection

  float4 viewVec   : TEXCOORD4;
  float4 normalVec   : TEXCOORD5; // w unused
  
  float4 cFoamColor : COLOR0;

  float4 worldPos : TEXCOORD6;

  float4 fogColor : TEXCOORD7;
  
  //float2 testUV : TEXCOORD8;
};

struct v2fWater2
{
  float4 Position  : POSITION;   // in projection space
  float4 foamTC    : TEXCOORD0;  // foam coordinates
  float4 bumpTC     : TEXCOORD1;  // wz if next frequency wave x2
  float4 screenProj : TEXCOORD2; // refraction projection
  float4 envTC      : TEXCOORD3; // reflection projection

  float4 viewVec   : TEXCOORD4;
  float4 normalVec   : TEXCOORD5; // w unused
  
  float4 cFoamColor : COLOR0;

  float4 worldPos : TEXCOORD6;

  float4 fogColor : TEXCOORD7;
  //float2 testUV : TEXCOORD8;
#if ((PROFILE == 3) || defined(_SDR_3_0))
  float4 screenSpace : VPOS;
#endif 
};

float ComputeWaterMeshSpreadDistance()
{
	float fCameraHeight = g_vCameraPos.y;
	float height = abs(fCameraHeight - g_fWaterLevel);
	if(height < 10.f)
		return 90.f;
	else if(height < 20.f)
		return 200.f;
	else
		return 400.f;
}


// Get vertex from screen space into world spasace
float GetGridVertexPos( inout float4 vPos, float2 uv)
{

//原来的代码
/*		
	float atten = ( vPos.z );								//
															// simple lod - keep nearby detail until 200 meters after that project all vertices to horizon
    vPos.xy -= float2(0.2, 0.2);	//稍微左下角偏移一点, 这样旋转后能彻底覆盖视野范围内.
	
	vPos.z = vPos.y;				//交换Y和Z
	
	//设置顶点高度
	vPos.y = g_fWaterLevel;
		
	float fLod0 = ComputeWaterMeshSpreadDistance();//90; 	//波浪出现在120米以内				// *( 1 / min(g_fWaveAmount, 2.0 ));
	if(atten > 99 && g_bEnableArea < 0.5)
	{
		//vPos.y += 3;			//边缘的顶点提高三米?
		fLod0 = g_vNearFarDis.y * 4;	//最边缘的一个顶点延伸到很远的地方
	}
	//缩放
	vPos.xz = vPos.xz * fLod0;
	
	//旋转
	float2 rotedPos;
	rotedPos.x = g_vOceanMeshRot.x * vPos.x + (-g_vOceanMeshRot.y) * vPos.z;
	rotedPos.y = g_vOceanMeshRot.y * vPos.x +  (g_vOceanMeshRot.x) * vPos.z;
	vPos.x = rotedPos.x;
	vPos.z = rotedPos.y;
	//vPos.xz = rotedPos.xy;
	
	//根据摄像机位置偏移
	float2 cpos = g_vCameraPos.xz;
	cpos.xy -= frac(cpos.xy * 0.4) * 2.5;
	vPos.xz += cpos.xy; //g_vCameraPos.xz;
*/
	vPos.x *= g_vSmallWaterWidth.x;
	vPos.z = vPos.y;
	vPos.z *= g_vSmallWaterWidth.x;
	vPos.x += g_vCurrentSmallWaterStart.x;
	vPos.z += g_vCurrentSmallWaterStart.y;
	vPos.y = g_fWaterLevel;
	
	float fDisToCamera = length(vPos.xz - g_vCameraPos.xz);
	
	if(fDisToCamera > 0.95 * g_vSmallWaterWidth.z)
		vPos.xz = normalize(vPos.xz - g_vCameraPos.xz) * g_vNearFarDis.y + g_vCameraPos.xz;

/*
		(g_vSideNoNeighber.x > 0.5 && uv.y > 1.001) || 
		(g_vSideNoNeighber.y > 0.5 && uv.x > 1.001) ||
		(g_vSideNoNeighber.z > 0.5 && uv.y < -0.001) ||
		(g_vSideNoNeighber.w > 0.5 && uv.x < -0.001))
	{
		vPos.xz = normalize(vPos.xz - g_vCameraPos.xz) * g_vNearFarDis.y + g_vCameraPos.xz;
	}
*/
	
	//根据Area作出变换
	//float4 g_waterArea;   //xy: minXZ; zw: maxXZ
	vPos.x = max(vPos.x, g_waterArea.x);
	vPos.z = max(vPos.z, g_waterArea.y);
	vPos.x = min(vPos.x, g_waterArea.z);
	vPos.z = min(vPos.z, g_waterArea.w);
  
	return fDisToCamera;
}

float4 tex2Dlod_bilinear( in sampler2D s, in float2 t, in float2 textureSize, in float2 texelSize )
{
	float2 f = frac( t * textureSize );
	// TODO: take advantage of arbitrary swizzles for 2.x & 3.0
	float4 t00 = tex2Dlod( s,  float4(t, 0, 0) );
	
	float4 t10 = tex2Dlod( s, float4( t + float2( texelSize.x, 0 ), 0, 0) );
	float4 tA = lerp( t00, t10, f.x );

	float4 t01 = tex2Dlod( s, float4( t + float2( 0, texelSize.y ), 0, 0) );
	float4 t11 = tex2Dlod( s, float4( t + float2( texelSize.x, texelSize.y ), 0, 0) );
	float4 tB = lerp( t01, t11, f.x );

	return lerp( tA, tB, f.y );
}

// Get waves
void GetWave(inout float3 vPos, in float2 FlowDir, float2 uv, float fDisToCamera)
{  
	float3 vFakePos = vPos;
	float fDisScale = 1.f;
	/*
	if( (g_vSideNoNeighber.x > 0.01 && uv.y > 0.99) || 
		(g_vSideNoNeighber.y > 0.01 && uv.x > 0.99) ||
		(g_vSideNoNeighber.z > 0.01 && uv.y < 0.01) ||
		(g_vSideNoNeighber.w > 0.01 && uv.x < 0.01))
		*/
	fDisScale = lerp(1, 0, saturate((fDisToCamera - 0.7 * g_vSmallWaterWidth.z) / (0.25 * g_vSmallWaterWidth.z)));
	//if(fDisToCamera > 0.8 * g_vSmallWaterWidth.z)
	//{
	//	return;
	//}
	
#ifndef ENABLE_WAVE
	return;
#else	
#ifndef ENABLE_FFT
	
	{
		float fPhaseTest = length(vFakePos.xz);

		const float4 vFrequencies = 5* g_fWaveSpeed * float4(0.233, 0.455, 0.6135, -0.1467);
		const float4 vPhases = g_fWaveAmount * float4(0.1, 0.159, 0.557, 0.2199);
		const float4 vAmplitudes = g_fWaveSize * float4(1, 0.5, 0.25, 0.5); 
		
		float4 vWaveFreq = g_fTime * vFrequencies;                                   // 1 inst
		
		float4 vCosPhase = (fPhaseTest + vFakePos.x)* vPhases;                                    // 1 inst
		float4 vCosWave = cos( vWaveFreq * FlowDir.x + vCosPhase );                               // 8 inst
		
		float4 vSinPhase = (fPhaseTest + vFakePos.y)* vPhases;                                    // 1 inst
		float4 vSinWave = sin( vWaveFreq * FlowDir.y + vSinPhase );                               // 8 inst

		float fAnimSum = dot(vCosWave, vAmplitudes) + dot(vSinWave, vAmplitudes);                 // 3 inst

		const float fAnimAmplitudeScale = 1.0 / 5.0;
		vPos.y += fAnimSum * fAnimAmplitudeScale * fDisScale; 
		return;
	}

#else	
	{
		float4 v0 = tex2Dlod_bilinear( fftTexSampler, (vFakePos.xz) * 0.0125 * g_fWaveAmount * 1.25, 64, 1.0 / 64); //因为VS中不能用双线性采样
		float3 offset = v0.xyz * 0.06 * g_fWaveSize * float3(1.5,1.5,1); 
		vPos.xyz += offset.xzy * fDisScale;  //转换YZ
	}
#endif
#endif

}

float3 ComputeSurfaceNormal( in float2 vFlowDir, in float3 vPosOrig, in float3 vPos, float fDisp, float2 uv, float fDisToCamera)
{
	float3 vNormal = float3(0,1,0);
#ifndef ENABLE_WAVE
	return vNormal;
#else	
	float3 vPos1 = vPosOrig.xyz + float3(1,0,0);
	float3 vPos1Orig = vPos1;
	GetWave( vPos1.xyz, vFlowDir, uv, fDisToCamera);  
	vPos1.xyz = lerp(vPosOrig.xyz, vPos1.xyz, fDisp);                                        // 2 alu
	vNormal.x = vPos.y - vPos1.y;

	vPos1 = vPosOrig.xyz + float3(0,0,1);
	vPos1Orig = vPos1;
	GetWave( vPos1.xyz, vFlowDir, uv, fDisToCamera);
	vPos1.xyz = lerp(vPosOrig.xyz, vPos1.xyz, fDisp);                                        // 2 alu
	vNormal.z = vPos.y - vPos1.y;

	return vNormal;
#endif	
}

float4 HPosToScreenTC(float4 HPos)
{
	float4 ScrTC = HPos;
	ScrTC.xy = (HPos.xy * float2(1,-1) + HPos.ww  ) * 0.5;
	ScrTC.xy += g_vScreenSize.zw * HPos.w;
	return ScrTC;
}

v2fWater vs(a2vWater IN)
{
	v2fWater OUT = (v2fWater)0;
	//OUT.testUV = IN.TexCoord;
	
	
	float4 vPos = IN.Position;   
	
	//边缘处的顶点位置和法线偏移需要衰减
	//z存放了边缘衰减值, 越靠近边缘此值越大
	float bEdgeDispl = 0;//1 - saturate(saturate(vPos.z + 0.05) * 2 - 1.0); //边缘位移量
	
	float bDebug = vPos.z;
	
	//流向
	float2 FlowDir = float2(g_fDirCos, g_fDirSin); //OceanParams1.yz;
	
	//从屏幕空间投影到世界空间
	float fDisToCamera = GetGridVertexPos(vPos, IN.TexCoord); // 30 alu
	
	vPos.w = 1;
	float4 vPosOrig = vPos;

	//采样FFT, 得到世界空间的位置
	GetWave( vPos.xyz, FlowDir, IN.TexCoord, fDisToCamera);
	
#ifdef ENABLE_FOG

	#if ((PROFILE == 3) || defined(_SDR_3_0))
		OUT.fogColor = float4(0,0,0,0);
	#else	
		OUT.fogColor = ComputeFog(vPos.xyz - g_vCameraPos.xyz, vPos.y, fogParamsP);
	#endif
#endif
	
	//靠近摄像机也要衰减
	float fDistToCam = length(g_vCameraPos.xyz - vPos.xyz);
	float fNearbyCameraAtten = (saturate( fDistToCam * fDistToCam));
	vPos.xyz = lerp(vPosOrig.xyz, vPos.xyz, saturate(fNearbyCameraAtten * (1 - bEdgeDispl)));
	
	
	//输出反射纹理坐标, 用摄像机当前高度替换网格高度
	float4 vProjTex = mul(float4(vPos.x, g_fWaterLevelAtCamera, vPos.z, 1), g_mReflProj);			//g_fWaterLevelAtCamera:摄像机处的海水高度
	OUT.envTC = vProjTex;
	
	//计算法线方向, 通过采样FFT纹理得到
	OUT.normalVec.xyz = ComputeSurfaceNormal( FlowDir, vPosOrig.xyz, vPos.xyz, bEdgeDispl, IN.TexCoord, fDisToCamera); //输出的Y是1
	OUT.normalVec.y += 0.1;
	
	//泡沫颜色. Crysis中, 水面越高, 水面越平,则该值越大, PS里面泡沫也越大
	OUT.cFoamColor.x = saturate(2 * (vPos.y - g_fWaterLevel)); //我修改后, 只考虑高度.这样浪尖也会变白	
	
	float3 viewVec = g_vCameraPos.xyz - vPos.xyz;                                                // 1 alu
	OUT.viewVec.xyz =  viewVec.xyz;                                                       // 3 alu

	OUT.normalVec.w = 1;
	
	//投影Pos
	OUT.Position = mul(vPos, g_mViewProj);    
	
	// 距离越远, 波动越剧烈, 法线扰动越小      Attenuate normal strength to make tesselation artefacts less visible     
	OUT.normalVec.xz *=  saturate(1- OUT.Position.w * /*0.0075 * 0.5*/ 0.01 +  0.15); // + saturate(0.25 / g_fWaveAmount));
	// 线性深度   Scale texture coordinates to get mix of low/high frequency details    
	OUT.viewVec.w = OUT.Position.w * g_vNearFarDis.w; //g_vNearFarDis.w : 1 / zFar
	
	//投影出屏幕坐标, 用来计算折射
	OUT.screenProj.xyw = HPosToScreenTC( OUT.Position ).xyw;                                                  // 2 alu
	
	// 根据世界空间位置计算纹理坐标      dont touch this scales !
	float2 vTex = vPos.xz * 0.005;                                             // 1 alu
	
	//凹凸贴图的坐标偏移. 随时间流动.  Output bump layers texture coordinates
	float2 vTranslation= FlowDir * ( g_fTime * g_fWindSpeed  *  0.0025);
	//各种叠加 Output texture coordinates for water - don't touch texture scales! hand tweaked stuff
	OUT.bumpTC.xywz = vTex.xyxy * float4(4, 4, 2, 2) + vTranslation.xyxy * float4(1, 1, 2, 2);
	
	//输出泡沫的纹理坐标.   Output texture coordinates for foam - don't touch texture scales! hand tweaked stuff
	float2 fAnimFoam = frac( g_fTime * g_fWaveSpeed * 0.01 * float2(0.2, 1) * 2);  //OceanParams0.z m_oceanWavesSpeed
	OUT.foamTC.xywz = vTex.xyxy * float4(58*0.2, 52*0.2, 38, 42)*3 + fAnimFoam.xxyy * float4(-1, -1, 1, 1);
	
	//顶点经过波浪计算后偏移量, 与泡沫计算有关
	float fWaveDispAmount = abs(vPosOrig.y - vPos.y);
	OUT.cFoamColor.x += fWaveDispAmount; // float4(bDebug.xx, , 0);
	float t = saturate((fDistToCam - g_vSmallWaterWidth.y) / (g_vSmallWaterWidth.z - g_vSmallWaterWidth.y));
	OUT.cFoamColor.x = lerp(OUT.cFoamColor.x, 0, t);
	
	
	//PYP测试用
	//OUT.testTC = IN.TexCoord;
	OUT.worldPos = vPos;
	
	return OUT;
}


float3 FetchNormalMap(sampler2D bumpMap, float2 bumpTC)
{
  float3 bumpNormal = tex2D( bumpMap, bumpTC.xy ).xyz;
  bumpNormal.z = 1;
  return bumpNormal;
}

float3 BumpGen( in v2fWater2 IN  )	//返回向上是z的bump
{
  float3 bumpNormal;
  float3 vVertexNormal = IN.normalVec.xyz;  
         
  float2 bumpColorC = FetchNormalMap(oceanBumpMapSampler, IN.bumpTC.xy*2).rg * 2 - float2(1,1);             
  float2 bumpColorD = FetchNormalMap(oceanBumpMapSampler, IN.bumpTC.wz*2).rg * 2 - float2(1,1);           
  
  float2 bumpHighFreq = (bumpColorC + bumpColorD);   

  bumpNormal = float3( /*bumpLowFreq.xy + */ bumpHighFreq.xy,1);

  bumpNormal.xy *= 0.2;	//方向偏移减弱
  bumpNormal.z = 1; 							//sqrt(1 - dot(bumpNormal.xy, bumpNormal.xy));
  
  bumpNormal.xyz += vVertexNormal.xzy; //加上原有方向
  
  //bumpNormal.xyz = vVertexNormal.xzy;
  
#ifdef ENABLE_RAIN
	if(true)
	{
		float2 rainNormal1 = tex2D(rainSampler1, IN.bumpTC.xy * 10).wy;		//用的dxt5格式压缩的法线图
		float2 rainNormal2 = tex2D(rainSampler2, IN.bumpTC.xy * 10).wy;
		float2 rainNormal = lerp(rainNormal1, rainNormal2, g_fRainLerp);
		bumpNormal.xy += ( rainNormal * 2 - float2(1,1) ) * 4;
	}
	else
	{
		float2 rainNormal1 = FetchNormalMap(rainSampler1, IN.bumpTC.xy * 20).rg;
		float2 rainNormal2 = FetchNormalMap(rainSampler2, IN.bumpTC.xy * 20).rg;
		float2 rainNormal = lerp(rainNormal1, rainNormal2, g_fRainLerp);
		bumpNormal.xy += ( rainNormal * 2 - float2(1,1) ) * 1;
	}
	
#endif
  
  bumpNormal.xyz =  normalize( bumpNormal );	//归一化
  return bumpNormal;
}

float GetFresnel(float NdotI, float power)
{
  float facing = (1.0 - NdotI);
  return saturate(pow(facing, power));
}

float3 CalcReflection(float3 N, float3 L)
{
	return 2.0f * dot(N, L) * N  - L;
}

struct psOutput
{
	float4		OutColor	: COLOR0;
#ifdef ENABLE_BLOOM	
	float4		BloomColor	: COLOR1;
#endif	
};

psOutput ps(v2fWater2 IN) : COLOR0
{	
	psOutput output;
	output.OutColor = float4(0,0,0,0);
#ifdef ENABLE_BLOOM		
	output.BloomColor = float4(0,0,0,0);
#endif	
	
	
	float fShadowOccl = 1; //1.0f - tex2Dproj(shadowOccludeMapSampler, IN.screenProj.xyzw).x;               
#if ((PROFILE == 3) || defined(_SDR_3_0))
	float2 screenUV = ( IN.screenSpace.xy) / g_vScreenSize.xy + g_vScreenSize.zw;
#else	
	float2 screenUV = ( IN.screenProj.xy / IN.screenProj.w );
#endif	

#if ((PROFILE == 3) || defined(_SDR_3_0))
	//从4层法线图生成NormalMap. 也可以只用两层
	float3 vVertexNormal = IN.normalVec.xyz;  
	float3 bumpNormal = BumpGen( IN ); //返回的法线向上是z, 需要与y调换

	float3 vFinalNormal = ( vVertexNormal.xyz + float3(bumpNormal.x, 0, bumpNormal.y) );
#else
	float2 bumpV = (tex2D(oceanBumpMapSampler, IN.bumpTC.xy).rg * 4 - float2(1,1)) * 0.1;
	float3 bumpNormal = normalize(float3(bumpV.x, bumpV.y, 1));
	float3 vFinalNormal = ( IN.normalVec.xyz + float3(bumpNormal.x, 0, bumpNormal.y) );
#endif
 
	//获取水面下深度
    float3 viewVec = normalize(IN.viewVec.xyz);  	
 	float sceneDepth = tex2D(sceneDepthSampler, screenUV).r * g_vNearFarDis.y; //DecodeSceneDepth( sceneDepthSampler, IN.screenProj );
 	float waterDepth = IN.screenProj.w;
	
 	float depth = (sceneDepth - waterDepth ); //水面下深度; 	
	
	//软相交. 越靠近岸边值越低
#if ((PROFILE == 3) || defined(_SDR_3_0))
 	float softIntersect = saturate( g_fSoftIntersectionFactor * depth);
	softIntersect *= softIntersect;
#else
	float softIntersect = 1.f;
#endif	
	
    //计算NdotI, 以及Fresnel项     
	float NdotI = (dot( viewVec.xyz, vFinalNormal.xyz ) );    // need detail textures rendering for underwater reflection
	NdotI = abs( NdotI ) ;
	float fFresnel= GetFresnel( NdotI /*, 0 */, 5) * g_fFresnelScale;
  	
	//计算内部反射项. 视点在水面下则为1; 在水面上则为0
	float fInsideReflection = saturate(-viewVec.y * 1000);	//y>0则等于0, y小于0则等于1
	
	//计算反射颜色
	//return tex2D(envMapSamplerRefl, screenUV);
	//return float4(1,0,0,1);
	
	float2 reflNewst = screenUV + bumpNormal.xy * 0.16;	//根据法线偏移一点
	
	float4 reflectColor = tex2D(envMapSamplerRefl, reflNewst);

/*	
	output.OutColor = reflectColor;
#ifdef ENABLE_BLOOM		
	output.BloomColor = float4(0,0,0,0);
#endif 
	return output;
*/
	
	if(g_bCubeRefl > 0.5)
		reflectColor = texCUBE(g_CubeReflectSampler, CalcReflection(vFinalNormal, viewVec));
		
	float2 refrNewst =  screenUV;//折射纹理坐标,未偏移
		
	//计算折射基本坐标
	float2 refrTC = bumpNormal.xy * (0.1 + 0.1 * fInsideReflection) * saturate(softIntersect * 4); //根据法线做偏移, 水下时做额外偏移, 越靠近岸边偏移越少
  
	//取折射颜色, 考虑rgb波长, 做不同的偏移
	const float fRefrMaskBias = 0.0001;
	IN.viewVec.w += fRefrMaskBias;	//把水体表面的深度加上一个Bias
		
#ifdef ENABLE_HIGH_REFRA		
		const float rScale = 1.15;	//1.15;
		const float gScale = 1.0;
		const float bScale = 0.85;   //0.85;
		
   	    float3 depthRefr = tex2D( sceneDepthSampler, refrNewst + refrTC.xy * rScale ).xyz;
 	    depthRefr.y =      tex2D( sceneDepthSampler, refrNewst + refrTC.xy * gScale ).x;
 	    depthRefr.z =      tex2D( sceneDepthSampler, refrNewst + refrTC.xy * bScale ).x;
   	   	  
 	    float3 fRefractionMask =  (IN.viewVec.www < depthRefr.xyz );	//如果水体表面的深度大于场景深度, 则表明采样的点在水面上,必须略过.
 	    refrTC *= fRefractionMask.x * fRefractionMask.y * fRefractionMask.z; 	
  
		float3 refractColor = 0; 
 
		refractColor.x = tex2D(envMapSamplerRefr, refrNewst + refrTC * rScale ).x;
		refractColor.y = tex2D(envMapSamplerRefr, refrNewst + refrTC * gScale ).y;
		refractColor.z = tex2D(envMapSamplerRefr, refrNewst + refrTC * bScale ).z;
#else
  	    float3 depthRefr = tex2D( sceneDepthSampler, refrNewst + refrTC.xy).rrr;
 	    float fRefractionMask =  (IN.viewVec.w < depthRefr.y );	//如果水体表面的深度大于场景深度, 则表明采样的点在水面上,必须略过.
 	    refrTC *= fRefractionMask; 	
  
		float3 refractColor = 0; 
 
		refractColor.xyz = tex2D(envMapSamplerRefr, refrNewst + refrTC ).xyz;
#endif 
	//return float4(refractColor.xyz, 1);

	
	float sceneDepthAfterRefr = depthRefr.y * g_vNearFarDis.y; 
	float depthAfterRefr = (sceneDepthAfterRefr - waterDepth ); //折射后水面下深度; 	

  //取焦散颜色
#ifdef ENABLE_CAUSTIC
  {
  		//return float4( tex2D(_causticRTSampler, refrNewst).rgb /*+ refractColor.xyz*/, 1);
		float4 cCaustic = tex2D(_causticRTSampler, refrNewst + refrTC);
		float fCausticIllum = 0;
		float vertDepth = depthAfterRefr * abs(dot(float3(0,1,0), viewVec));
		float4 g_fCausticLerpDis = float4(0.5, 2, 3, 6);
		fCausticIllum = GetCausticIllum(g_fCausticLerpDis, vertDepth); // 根据深度淡入淡出焦散颜色
		cCaustic.xyz *= fCausticIllum;
		
		refractColor.xyz += cCaustic.xyz;	//加上焦散颜色 暂时不添加焦三
		//return float4(cCaustic.xyz, 1);
  }
#endif 
		
	float wl = depth;
	
	if(depthAfterRefr < 0)
		;
	else
		wl = depthAfterRefr;
		
	float4 cWaterFog = GetWaterVolumetricFogColor(wl, g_fFogDensity, g_fWaterFogScaleExp, g_waterFogColor);

	refractColor.xyz = lerp( cWaterFog.xyz, refractColor.xyz, cWaterFog.w );
  
	float3 cFoamFinal = float3(0,0,0);
	
	//采样光泽
	float fGloss = tex2D(waterGlossMapSampler, IN.bumpTC.wz + vFinalNormal.xz * 0.1).x; //采样Gloss纹理, 根据法线做偏移
	fGloss = fGloss * fGloss * 2;
	
#if ((PROFILE == 3) || defined(_SDR_3_0))
	//根据光泽修正菲涅尔系数
	float fFresnelGloss = (fGloss * 0.5 + 0.5);
	fFresnel = saturate( fFresnel * g_fReflectionAmount * fFresnelGloss);   //Gloss越大, 菲涅尔越大,  最终颜色越向反射颜色偏移
#endif

	//根据Fresnel合并折射和反射颜色
	float3 final= lerp(refractColor, reflectColor.xyz, saturate(fFresnel + saturate(1 - NdotI * 2) * fInsideReflection)); //如果视点在水下,还需要额外偏向于反射

	float4 bloomColor = float4(0,0,0,1);

#if ((PROFILE == 3) || defined(_SDR_3_0))
	{
		//计算泡沫颜色, 两层泡沫
		float fFoam = saturate(tex2D(foamSampler, IN.foamTC.xy - vFinalNormal.xz * 0.1).y); //计算泡沫纹理坐标,根据法线做偏移. 采样Foam纹理, 偏移较少
		fFoam += tex2D(foamSampler, IN.foamTC.wz  + vFinalNormal.xz * 0.4).y * 1.2;  		//采样Foam纹理, 偏移较多   //我的算法,结果差不多
	
		float fFoamLuminance = saturate( fFoam - 0.85 ); //泡沫亮度
  
		//岸边的泡沫
		float3 cFoamEdge = float3(0,0,0);
		if(g_bEdgeFoam > 0.5f)
		{
			float foamSoftIntersect = saturate( depth * 0.1 / saturate(fFoamLuminance) );  //越靠近岸边越小
			float foam = smoothstep(0.4, 0.8, 1 - foamSoftIntersect ) * 0.5 * g_fEdgeFoamAmount; // * softIntersect;  //让SoftIntersect与岸边泡沫无关
			cFoamEdge = foam * 0.6;
		}
		//return float4(cFoamEdge, 1);
		
		//波浪中的泡沫
		float fCrestFoam = 0;
		if(g_bWaveFoam > 0.5f)
		{
			fCrestFoam = 0.5 * (fFoamLuminance * fGloss * IN.cFoamColor.x);   //泡沫是与Gloss纹理叠加而来的.
		}
		
		//波浪的Gloss
		float3 cWavesGloss = float3(0,0,0);
		if(g_bWaveGloss > 0.5)
		{		
			cWavesGloss = fGloss * 0.1;
			cWavesGloss *= g_fWhiteCapsAmount;     
		}
		
		cFoamFinal = saturate( (cFoamEdge * fFoamLuminance + fCrestFoam)  + cWavesGloss);

		// Add foam
		cFoamFinal =  (g_PS_SkyColor.xyz + g_PS_SunColor.xyz )  * cFoamFinal; // 2 alu
		
		final += cFoamFinal;	
	}	

    //SunShine
	{
		float3 vSunLightDirReverse = g_PS_SunLightDir;
		//vSunLightDirReverse.z = -vSunLightDirReverse.z;
		float3 bumpNormalY = normalize(float3(bumpNormal.x, bumpNormal.z, bumpNormal.y));
		
		float fNDotUp = saturate(vSunLightDirReverse.y);
		bumpNormalY.y *= lerp(1, 0.15, fNDotUp);					//当太阳很高的时候，让normal扰动更剧烈
		bumpNormalY = normalize(bumpNormalY);
	
		float3 mirrorEye = ( 2 * dot( viewVec, bumpNormalY) * bumpNormalY - viewVec );
		float dotSpec =  saturate(dot( mirrorEye.xyz, vSunLightDirReverse ) * 0.5 + 0.5);

		float3 vSunGlow = float3(0,0,0);
	
		float powDot = pow(dotSpec, 400);
		vSunGlow = fFresnel * powDot * fGloss * g_PS_SunColor.xyz * pow(10, fNDotUp * 1 + 1);			//当太阳很高的时候，让高光强度变得很大

		vSunGlow += fGloss * vSunGlow;
		
		bloomColor.xyz = vSunGlow.xyz * fShadowOccl * g_fSunMultiplier;
		
		final.xyz += bloomColor.xyz;
	}
#endif	

	final.xyz = lerp( refractColor, final.xyz, softIntersect * saturate(IN.screenProj.w));	


#ifdef ENABLE_FOG
	#if ((PROFILE == 3) || defined(_SDR_3_0))
		float4 fogColor = ComputeFog(IN.worldPos.xyz - g_vCameraPos.xyz, IN.worldPos.y, fogParamsP);
	#else
		float4 fogColor = IN.fogColor;
	#endif	
	//由于折射的时候已经算过雾了，这里再算的话会出错。近处的不考虑雾了。
	float fDistance = length(IN.worldPos.xyz - g_vCameraPos.xyz);
	float fFogScale = smoothstep(100, 200, fDistance);
	final.xyz = lerp( fogColor.xyz, final.xyz, max(fogColor.w, 1.f - fFogScale));
#endif

  float fA =  1;
  float4 Color = max( float4(final.xyz, fA), 0);
  
  	output.OutColor = Color;
	
#ifdef ENABLE_BLOOM		
	output.BloomColor = bloomColor;//float4(0,0,0,0);//
#endif 
  
  return output;
  
 
}


