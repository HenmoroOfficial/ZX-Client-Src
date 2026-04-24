//--------------------------------------------------------------------------------------
// Copyright (c) Archosaur Studio. All rights reserved.
// Created by liyi 2012, 3, 16
// Version : 1.0
//--------------------------------------------------------------------------------------
#include "..\fog\fog_common.h"

string entrynames = "vs_2_0 vs_AboveWater;ps_2_0 ps_AboveWater; ps_2_0 ps_UnderWater";

string macro0
<
	string name = "_FOG_";
	string desc = "FOG";
	string type = "PRG";
	string GroupID = "1";
>;


//--------------------------------------------------------------------------------------
//Vertex	Variables
//--------------------------------------------------------------------------------------
float4x4 g_matWVP;

float4 g_MainLightPos;
float4 g_vCameraPos;

bool g_bStaticMesh = false;	//	Static mesh flag, must set default value to false, otherwise some water technique won't work
float4 g_vMeshBlkParams;
#define g_fMeshBlkLeft g_vMeshBlkParams.x
#define g_fMeshBlkHei g_vMeshBlkParams.y
#define g_fMeshBlkTop g_vMeshBlkParams.z
#define g_fMeshGridSize g_vMeshBlkParams.w

float g_fNMapUVScale;		//	normal map uv scale
float4 g_vWaveNormalMapOffset;	//	.xy: offset for wave normal map 0; .zw: offset for wave normal map 1

float4x4 g_matReflect;

float2 g_ScreenDim;


//--------------------------------------------------------------------------------------
//	Vertex Shader
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position : POSITION;
	float4 uvWave : TEXCOORD0;
	float4 ReflectTexCoord : TEXCOORD1;
	float4 vEyeVec : TEXCOORD2;		//	xyz: vector from vertex to camera; .w: approximate eye to vertex distance
	float4 vLightVec : TEXCOORD3;	//	Vector from vertex to light; .w: fog factor
	float4 uvGBuf : TEXCOORD4;
	float3 vPosInWorld : TEXCOORD5;
	float4 fogParam   : TEXCOORD6;
};

struct VS_OUTPUT2
{
	float4 Position : POSITION;
	float4 uvWave : TEXCOORD0;
	float4 ReflectTexCoord : TEXCOORD1;
	float4 vEyeVec : TEXCOORD2;		//	xyz: vector from vertex to camera; .w: approximate eye to vertex distance
	float4 vLightVec : TEXCOORD3;	//	Vector from vertex to light; .w: fog factor
	float4 uvGBuf : TEXCOORD4;
	float3 vPosInWorld : TEXCOORD5;
	float4 fogParam   : TEXCOORD6;
	#ifdef _SDR_3_0
		float4 screenSpace : VPOS;
	#endif 
};
//	Calculate screen-space texture coordinates, this function add the half-pixel offset
//	due to the texturing algorithm used by Direct3D
float4 CalcScreenSpaceTexCoord(float4 vProjPos, float2 TexDim)
{
	float2 xy = float2(vProjPos.x + vProjPos.w, vProjPos.w - vProjPos.y);
	float4 vPos = float4(0.5f * (xy + vProjPos.w / TexDim), vProjPos.zw);
	return vPos;
}

VS_OUTPUT vs_AboveWater(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;
	
	float4 vInPos = Input.Position;
	if (g_bStaticMesh)
	{
		vInPos.x = g_fMeshBlkLeft + Input.Position.x * g_fMeshGridSize;
		vInPos.y = g_fMeshBlkHei;
		vInPos.z = g_fMeshBlkTop + Input.Position.z * g_fMeshGridSize;
		vInPos.w = 1.0f;
	}

	Output.Position = mul(vInPos, g_matWVP);
	
	//	Calculate normal map uv
	float2 uvBase = Input.TexCoord* g_fNMapUVScale;
	Output.uvWave.xy = uvBase + g_vWaveNormalMapOffset.xy;
	Output.uvWave.zw = uvBase + g_vWaveNormalMapOffset.zw;
	
	Output.ReflectTexCoord = mul(vInPos, g_matReflect);
	
	//	Calculate view and light vector
	Output.vEyeVec.xyz = g_vCameraPos.xyz - vInPos.xyz;
	Output.vEyeVec.w = Output.Position.z;
	Output.vLightVec.xyz = g_MainLightPos.xyz - vInPos.xyz;
	Output.vPosInWorld = vInPos.xyz;
	
	Output.uvGBuf = CalcScreenSpaceTexCoord(Output.Position, g_ScreenDim);

	Output.fogParam = ComputeCompositeFogSimple(vInPos.xyz - g_vCameraPos.xyz, vInPos.y);

	return Output;
}


//--------------------------------------------------------------------------------------
// textures and samplers
//--------------------------------------------------------------------------------------
//FIXME!!
//A3D_SAMPLER2D_DEF(g_NormalSampler, g_NormalTex1, LINEAR, LINEAR, LINEAR, WRAP, WRAP)
sampler2D g_NormalSampler: register(s0);

//A3D_SAMPLER2D_DEF(g_NormalSampler1, g_NormalTex1, LINEAR, LINEAR, LINEAR, WRAP, WRAP)
sampler2D g_NormalSampler1: register(s1);

//A3D_SAMPLER2D_DEF(g_ReflectSampler, g_ReflectTex, POINT, POINT, POINT, CLAMP, CLAMP)
sampler2D g_ReflectSampler: register(s2);

//A3D_SAMPLER2D_DEF(g_RefractSampler, g_RefractTex, POINT, POINT, POINT, CLAMP, CLAMP)
sampler2D g_RefractSampler: register(s3);

samplerCUBE g_CubeReflectSampler: register(s4);

//--------------------------------------------------------------------------------------
//Pixel variables
//--------------------------------------------------------------------------------------
float3 g_vWaterColor = float3(0.5f, 0.5f, 0.5f);
float4 g_MainLightColor;

#define WATER_SPEC_POWER	250.0

float4 g_vMisc;		//	.x: water height; .y: edge height scale; .z: min reflection factor; .w: darkness factor
#define g_fWaterHei g_vMisc.x
#define g_fEdgeHeiScale g_vMisc.y
#define g_fMinReflect g_vMisc.z
#define g_fDarkFactor g_vMisc.w

bool g_bCubeReflect;

//--------------------------------------------------------------------------------------
//Pixel Util Function
//--------------------------------------------------------------------------------------

float CalcFresnel(float3 vToEyeVec)
{
	//	Compute the fresnel term to blend reflection and refraction maps
	float3 vUpAxis = float3(0.0f, 1.0f, 0.0f);
	float fAngle = saturate(dot(vToEyeVec, vUpAxis));
	float fFresnel = 0.02037f + (1.0f - 0.02037f) * pow(1.0f - fAngle, 5.0f);
	return fFresnel;
}

float3 SamplerNormalMap(sampler2D sample, float2 uv)
{
		float3 Normal =  tex2D(sample, uv).xyz * 2.0f -1.0f;
		return Normal;
}

float3 AppendDetailNormal(float3 Normal, float3 DetailNormal)
{
	return normalize(Normal * DetailNormal.z + float3(DetailNormal.x, DetailNormal.y, 0.0f ));
}

//	Build wave normal from 2 wave normal textures
float3 BuildWaveNormal(sampler2D sam0, float2 uv0, sampler2D sam1, float2 uv1)
{
	//	Note: exchange y and z channel
	//1
	//float3 vNormal0 = tex2D(sam0, uv0 * 4).xzy;
	//float3 vNormal1 = tex2D(sam1, uv1 * 2).xzy;
	
	//float3 vNormal = (vNormal0 + vNormal1) * 2.0f - 2.0f;
	//vNormal = normalize(vNormal);
	
	//2
	
	float3 vNormal = normalize((tex2D(sam0, uv0 * 8) + tex2D(sam1,uv1 * 4)-1).xzy);
	return vNormal;
	
	
	//3
	/*
	float3 vNormal1 = SamplerNormalMap(sam0, uv0 * 4);
	float3 vNormal2 = SamplerNormalMap(sam1, uv1 * 8);
	float3 vNormal3 = SamplerNormalMap(sam0, (uv0 + uv1) * 10.0f);
	float3 vNormal4 = AppendDetailNormal(vNormal1 , vNormal2);
	//float3 vNormal  = AppendDetailNormal(vNormal4, vNormal3);
	
	
	return vNormal4.xzy;
	*/
}

float3 WaterScattering(float3 InColor, float fDepth, float fLength)
{
	float extinction = exp(-0.02f * max(fDepth, 0.0f));
    float alpha = (1 - extinction * exp(-max(fLength, 0.0f) * 0.2f));
    return lerp(InColor, g_vWaterColor, alpha);
}

float3 CalcReflection(float3 N, float3 L)
{
	return 2.0f * dot(N, L) * N  - L;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 ps_AboveWater(VS_OUTPUT2 Input):COLOR0
{
	float3 vToEyeVec = normalize(Input.vEyeVec.xyz);
	float3 vToLightVec = normalize(Input.vLightVec.xyz);

	//	Build pixel normal from 2 wave normal map.
	float3 vNormal = BuildWaveNormal(g_NormalSampler, Input.uvWave.xy, g_NormalSampler1, Input.uvWave.zw);
	//return float4(vNormal, 1.0f);//FIXME!! TEST!!
	
	//	Get the reflection vector from the eye
	float3 vMLightRef = float3(0.0f,0.0f, 0.0f);
	#ifdef _SDR_3_0
	
		float3 vHalf = normalize(vToEyeVec + vToLightVec); // N dot H
		float fSpecPowerBase = saturate(dot(vHalf, vNormal));
		//	Compute the reflection from main light
		vMLightRef = pow(fSpecPowerBase, WATER_SPEC_POWER) * g_MainLightColor.rgb *float3(5.0f, 2.0f, 1.0f);
	#endif
	
	//return float4(vMLightRef, 1.0f); //TEST!!

	//	Compute the fresnel term to blend reflection and refraction maps
	float fFresnel = CalcFresnel(vToEyeVec);

	//	Get the reflection and refraction from render target
	//	Edge alpha is store in reflect texture's alpha channel
	//	Refract uv is same as GBuf uv
	
	#ifdef _SDR_3_0	
		float2 screenUV = ( Input.screenSpace.xy) / g_ScreenDim + float2(0.5, 0.5) / g_ScreenDim; //To support 3D Vision
		float2 ReflectTexCoord = screenUV;
		float2 RefractTexCoord = screenUV;
	#else
		float2 ReflectTexCoord = Input.uvGBuf.xy / Input.uvGBuf.w; 
		float2 RefractTexCoord  = ReflectTexCoord;
	#endif
	
	float4 RefractTex = tex2D(g_RefractSampler, RefractTexCoord);
	
	//	Add disturb to reflection and refraction
	float2 turb = float2(0.07f, -0.03f);
	float2 ReflectTurb = turb.x * float2(vNormal.x, vNormal.z) + ReflectTexCoord;
	float2 RefractTurb = turb.y * float2(vNormal.x, vNormal.z) + RefractTexCoord;
	
	//	Sample refract and reflect texture
	float4 realReflectTex = tex2D(g_ReflectSampler, ReflectTurb);
	float4 cubeReflectTex = texCUBE(g_CubeReflectSampler, CalcReflection(vNormal, vToEyeVec));
	float4 PerturbReflectTex;
	if(g_bCubeReflect)
	{
		PerturbReflectTex = cubeReflectTex;
	}
	else
	{	
		PerturbReflectTex = realReflectTex;
	}

	//return PerturbReflectTex; //TEST!!
	
	float4 PerturbRefractTex = tex2D(g_RefractSampler, RefractTurb);
	
	//	Calcualte edge alpha
	float2 vTempHeiDelta = float2(RefractTex.a, PerturbRefractTex.a);
	vTempHeiDelta = vTempHeiDelta / (1.0f - vTempHeiDelta);
	float2 vTempEdgeAlpha = saturate(vTempHeiDelta * g_fEdgeHeiScale);
	
	float fEdgeAlpha = vTempEdgeAlpha.y;
	float fHeiDelta = vTempHeiDelta.y;

	if (fEdgeAlpha < 0.01f)
	{
		PerturbRefractTex = RefractTex;
		fEdgeAlpha = vTempEdgeAlpha.x;
		fHeiDelta = vTempHeiDelta.x;
	}	
	
	//	Interpolate refraction color with water color based on depth factor
	float fDarkness = fHeiDelta * g_fDarkFactor;
	fDarkness = 1.0f / exp2(fDarkness);
	float3 vFinalColor = lerp(g_vWaterColor, PerturbRefractTex.rgb, fDarkness);
	
	//	Interpolate the reflection and refraction colors based on the fresnel term
	vFinalColor = lerp(vFinalColor, PerturbReflectTex.rgb, fFresnel);

	#ifdef _FOG_    
    vFinalColor.rgb = lerp(Input.fogParam.rgb, vFinalColor.rgb, Input.fogParam.a);
    vMLightRef *= Input.fogParam.a;
	#endif
		
		vFinalColor = lerp(PerturbRefractTex.rgb, vFinalColor, fEdgeAlpha);
		float fE2 = saturate(fEdgeAlpha * 3.0f);
		vFinalColor += vMLightRef*fE2;
		
		return float4(vFinalColor, 1.0f);
}

//	Under water, per-pixel lighting
float4 ps_UnderWater(VS_OUTPUT2 Input) : COLOR0
{
	float3 vToEyeVec = normalize(Input.vEyeVec.xyz);
	float3 vToLightVec = normalize(Input.vLightVec.xyz);

	//	Build pixel normal from 2 wave normal map.
	float3 vNormal = BuildWaveNormal(g_NormalSampler, Input.uvWave.xy, g_NormalSampler1, Input.uvWave.zw);

	//	Get the reflection and refraction from render target
	//	Edge alpha is store in reflect texture's alpha channel
	//	Refract uv is same as GBuf uv
	
	#ifdef _SDR_3_0	
		float2 screenUV = ( Input.screenSpace.xy) / g_ScreenDim + float2(0.5, 0.5) / g_ScreenDim;	//To support 3D Vision
		float2 RefractTexCoord = screenUV;
	#else
		float2 RefractTexCoord = Input.uvGBuf.xy / Input.uvGBuf.w;
	#endif
	
	float4 RefractTex = tex2D(g_RefractSampler, RefractTexCoord);
	
	//	Add disturb to reflection and refraction
	float turb = -0.03f;
	float2 RefractTurb = turb * float2(vNormal.x, vNormal.z) + RefractTexCoord;
	
	//	Sample refract and reflect texture
	float4 PerturbRefractTex = tex2D(g_RefractSampler, RefractTurb);
	
	if (PerturbRefractTex.a < 0.01f)
		PerturbRefractTex = RefractTex;
	
	//	Combine refraction color and the main light's reflection
	float3 vFinalColor = PerturbRefractTex.rgb;
	
	//	Use scattering effect instead of fog effect
	vFinalColor = WaterScattering(vFinalColor, 0.0f, Input.vEyeVec.w * 0.18f);
	
	//return float4(vFinalColor, 1.0f);
	
	
	#ifdef _FOG_    
    vFinalColor.rgb = lerp(Input.fogParam.rgb, vFinalColor.rgb, Input.fogParam.a);
	#endif
	return float4(vFinalColor, 1.0f);
}
