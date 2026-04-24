#include "..\Shadow\Shadow_common.h"
#include "..\fog\fog_common.h"

string entrynames = "vs_2_0 vs_main;ps_2_0 ps_main";

string macro0
<
	string name = "_FOG_";
	string desc = "是否开启雾";
	string type = "PRG";
	string GroupID = "1";
>;

string macro1
<
	string name = "_SHADOW_";
	string desc = "是否开阴影";
	string type = "PRG;SDR_3_0";
	string GroupID = "2";
>;

string macro2
<
	string name = "_HWPCF_";
	string desc = "是否开硬件PCF";
	string type = "PRG;SDR_3_0";
	string GroupID = "3";
>;

string macro3
<
	string name = "_REAL_TIME_";
	string desc = "实时Diffuse";
	string type = "PRG";
	string GroupID = "4";
>;

string macro4
<
	string name = "_SHADOW_HIGH_";
	string desc = "高品质阴影";
	string type = "PRG;SDR_3_0";
	string GroupID = "5";
>;


string layer1
<
	string name = "_LAYER1_";
	string desc = "1层纹理";
	string type = "PRG";
	string GroupID = "6";
>;

string layer2
<
	string name = "_LAYER2_";
	string desc = "2层纹理";
	string type = "PRG";
	string GroupID = "6";
>;

string layer3
<
	string name = "_LAYER3_";
	string desc = "3层纹理";
	string type = "PRG";
	string GroupID = "6";
>;

string layer4
<
	string name = "_LAYER4_";
	string desc = "4层纹理";
	string type = "PRG";
	string GroupID = "6";
>;

string layer5
<
	string name = "_LAYER5_";
	string desc = "5层纹理";
	string type = "PRG";
	string GroupID = "6";
>;

string layer6
<
	string name = "_LAYER6_";
	string desc = "6层纹理";
	string type = "PRG";
	string GroupID = "6";
>;



#if defined(_LAYER1_)
    #define nLayers 1
#elif defined(_LAYER2_)
    #define nLayers 2
#elif defined(_LAYER3_)
    #define nLayers 3
#elif defined(_LAYER4_)
    #define nLayers 4
#elif defined(_LAYER5_)
    #define nLayers 5
#elif defined(_LAYER6_)
    #define nLayers 6
#else
    #define nLayers 0
#endif

struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 uv0		: TEXCOORD0;
	float2 uv1		: TEXCOORD1;
	float2 uv2		: TEXCOORD2;
	float2 uv3		: TEXCOORD3;
};

struct VS_OUTPUT
{
	float4 oPos		: POSITION;
	float4 oSpecular: COLOR0;		// specular
#if nLayers > 3	
	float4 uvBase[3]	: TEXCOORD0;
#else
	float2 uvBase[3]	: TEXCOORD0;
#endif

	float2 uvMask	: TEXCOORD3;
  	float4 oDiffuse	: TEXCOORD4;		// diffuse

#ifdef _SHADOW_
	//Shadow
	float4	shadowProj : TEXCOORD5;
#endif

#ifdef _FOG_ 
    float4 fogParam : TEXCOORD6;
#endif    
};

sampler g_ShadowMapSampler : register(s0);
sampler g_ShadowMapFloat : register(s1);

sampler2D g_BaseSampler[6] : register(s2);

sampler2D g_MaskMapSampler0 : register(s8);
sampler2D g_MaskMapSampler1 : register(s9);

//	g_MaskMap is mask texture of the first 3 base texture layers
//	red   - layer 3 mask
//	green - layer 2 mask
//	blue  - layer 1 mask

sampler2D g_LMDaySampler : register(s10);
sampler2D g_LMNightSampler : register(s11);
sampler2D g_ColTexSampler : register(s12);

float4 g_LitmapParams; // {scaleDay, scaleNight, 0, g_fDNFactor};
float4 g_SettingShadowMapSize;

float4x4 g_matViewProjection;
float3   g_vecEyePos;
float3   g_vecMainLightDir;
float4   g_colMtlDiffuse;
float4   g_colMtlSpecular;
float    g_fSpecPower;
static const float	g_fColTexMax = 1.9921875;	// See A3DTerrainBlk.h
float4x4 g_matLightWVP;	//Shadow

float4 g_colDiffuse : Diffuse;
float4 g_colAmbient : Ambient0;
float4 g_Ambient2 : Ambient1;
float g_fDiffusePower : DirPower;
float3 g_vLightDir : LightDir;

float4 g_texAxis[2] : register(c0);
float4 g_texScale[3] : register(c2);

float glb_CalcHDotN(float3 vNormal, float3 vLightDir, float3 vViewDir)
{
	float3 vHalf = normalize(vLightDir + vViewDir);
	return saturate(dot(vHalf, vNormal));
}

VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT Output;

	float2 projUV[3] = 
	{
		input.uv2,
		input.uv0,
		input.uv1,
	};

  	Output.uvBase[0].xy = projUV[g_texAxis[0].x] * g_texScale[0].xy;
  	Output.uvBase[1].xy = projUV[g_texAxis[0].y] * g_texScale[0].zw;
  	Output.uvBase[2].xy = projUV[g_texAxis[0].z] * g_texScale[1].xy;

#if nLayers > 3
  	Output.uvBase[0].zw = projUV[g_texAxis[0].w] * g_texScale[1].zw;
  	Output.uvBase[1].zw = projUV[g_texAxis[1].x] * g_texScale[2].xy;
  	Output.uvBase[2].zw = projUV[g_texAxis[1].y] * g_texScale[2].zw;
#endif
	Output.uvMask = input.uv3;

	Output.oPos = mul(float4(input.Position, 1), g_matViewProjection);
	float3 vCameraToWorldPos = input.Position - g_vecEyePos;
	
#ifdef _REAL_TIME_	
	float fNDotL = -dot(input.Normal, g_vLightDir);
	
	float fA1 = saturate(-fNDotL);
	float fA2 = 1 - abs(fNDotL);
	fNDotL = pow(saturate(fNDotL), 0.5);

	float3 colDiffuse = (g_fDiffusePower * g_colDiffuse.rgb * fNDotL);
	float3 colAmbient = (g_colAmbient.rgb * fA1 + g_Ambient2.rgb * fA2);
    float3 diffuse = colDiffuse.rgb + colAmbient.rgb;

	Output.oSpecular = 0;
	Output.oDiffuse = float4(diffuse, 1);
#else
	Output.oDiffuse = float4(1.0, 1.0, 1.0, 1);	//diffuse
	float3 vViewDir = -normalize(vCameraToWorldPos);
	float fHDotN = glb_CalcHDotN(input.Normal, -g_vLightDir, vViewDir);
	Output.oSpecular = pow(fHDotN, g_fSpecPower) * g_colMtlSpecular;	//specular
#endif

#ifdef _SHADOW_
	//Shadow
	Output.shadowProj = glb_ComputeShadowProj(float4(input.Position, 1), g_matLightWVP);
#endif

#ifdef _FOG_	
	Output.fogParam = ComputeCompositeFogSimple(vCameraToWorldPos, input.Position.y);
#endif	

	Output.uvMask = input.uv3;
	return Output;
}

float4 glb_CalcFinalOutput(float4 color, float3 lmColor, float3 specular, float3 diffuse, float4 shadowColor)
{
	float3 finalCol = color.rgb + color.a * specular * lmColor;
	finalCol = finalCol * diffuse;
	finalCol = finalCol * shadowColor.rgb;
	return float4(finalCol * lmColor, 1);
}

float3 g_EnableMask[6] : register(c0);


float4 ps_main(VS_OUTPUT input) : COLOR0
{
    float4 baseColor = tex2D(g_BaseSampler[0], input.uvBase[0].xy);
	float3 mask0 = tex2D(g_MaskMapSampler0, input.uvMask).rgb;
	float3 mask1 = tex2D(g_MaskMapSampler1, input.uvMask).rgb;
    
	float4 color = baseColor * mask0.b;
	float mask;
	#if nLayers == 6
		color = lerp(color, tex2D(g_BaseSampler[1],input.uvBase[1].xy), mask0.y);
		color = lerp(color, tex2D(g_BaseSampler[2],input.uvBase[2].xy), mask0.x);
		color = lerp(color, tex2D(g_BaseSampler[3],input.uvBase[0].zw), mask1.z);
		color = lerp(color, tex2D(g_BaseSampler[4],input.uvBase[1].zw), mask1.y);
		color = lerp(color, tex2D(g_BaseSampler[5],input.uvBase[2].zw), mask1.x);
	#elif nLayers == 5
		mask = -dot(g_EnableMask[1].xy, mask0.xy);
		color = lerp(color, tex2D(g_BaseSampler[1], input.uvBase[1].xy), mask);
		mask = saturate(dot(-g_EnableMask[2].xy, mask0.xy)) + saturate(dot(g_EnableMask[2], mask1));
		color = lerp(color, tex2D(g_BaseSampler[2], input.uvBase[2].xy), mask);
		mask = dot(g_EnableMask[3], mask1);
		color = lerp(color, tex2D(g_BaseSampler[3], input.uvBase[0].zw), mask);
		mask = dot(g_EnableMask[4], mask1);
		color = lerp(color, tex2D(g_BaseSampler[4], input.uvBase[1].zw), mask);
	#else

		for (int n = 1 ; n < nLayers && n < 3; n++)
		{
			mask = saturate(dot(-g_EnableMask[n].xy, mask0.xy)) + saturate(dot(g_EnableMask[n],mask1));
			color = lerp(color, tex2D(g_BaseSampler[n], input.uvBase[n].xy), mask);
		}
		#if nLayers == 4
			mask = dot(g_EnableMask[3], mask1);
			//return float4(mask, mask, 0, 1);
			color = lerp(color, tex2D(g_BaseSampler[3], input.uvBase[0].zw), mask);
		#endif
		
	#endif

	float4 cmData = tex2D(g_ColTexSampler, input.uvMask);
	
	color.rgb = (color.rgb * cmData.rgb * g_fColTexMax);

#ifndef _REAL_TIME_
	float4 lm0 = tex2D(g_LMDaySampler, input.uvMask) * g_LitmapParams.x;
	float4 lm1 = tex2D(g_LMNightSampler, input.uvMask) * g_LitmapParams.y;
	float4 lmColor = lerp(lm0, lm1, g_LitmapParams.w);
#else	
    float4 lmColor = float4(1, 1, 1, 1);
#endif

	float4 vIntensityInLight = float4(1,1,1,1);
	float4 vIntensityInShadow = float4(0.2,0.2,0.2,1.0);
	float4 vAmbientColor = float4(0,0,0,0.2);
	
#ifdef _SHADOW_
	bool bHWPC = false;
	#ifdef _HWPCF_
		bHWPC = true;
	#endif
    int nSampleCount = 1;
    #ifdef _SHADOW_HIGH_
        nSampleCount = 5;
    #endif    

	float fShadowMapSize = g_SettingShadowMapSize.x;
	float4 vShadow = glb_ComputShadowColor(g_ShadowMapSampler, g_ShadowMapFloat, 
		input.shadowProj, true, bHWPC, fShadowMapSize, nSampleCount);	
#else
	float4 vShadow = float4(1,1,1,1);
#endif

	float4 terrainColor = glb_CalcFinalOutput(color, lmColor.rgb, input.oSpecular.rgb, input.oDiffuse.rgb, vShadow);
#ifdef _FOG_	
	return float4(lerp(input.fogParam.rgb, terrainColor.rgb, input.fogParam.a), 1);
#else	
    return terrainColor;
#endif
}