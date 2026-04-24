//--------------------------------------------------------------------------------------
// Copyright (c) Archosaur Studio. All rights reserved.
// Created by liyi 2012, 3, 6
// FileName : SimpleWater.hlsl
// Version : 1.0
//--------------------------------------------------------------------------------------

#include "..\fog\fog_common.h"

string entrynames = "vs_2_0 vs_Main; ps_2_0 ps_EyeAboveWater; ps_2_0 ps_EyeUnderWater";
string macro0
<
	string name = "_FOG_";
	string desc = "FOG";
	string type = "PRG";
	string GroupID = "1";
>;

//--------------------------------------------------------------------------------------
//	Vertex Variables
//--------------------------------------------------------------------------------------

float4x4 g_matWVP;
float3 g_vCameraPos;

float4x4 g_matTexTrans;

float3 g_MainLightPos;

//static mesh
bool g_bStaticMesh = false;
float4 g_vMeshBlkParams;
#define g_fMeshBlkLeft g_vMeshBlkParams.x
#define g_fMeshBlkHei g_vMeshBlkParams.y
#define g_fMeshBlkTop g_vMeshBlkParams.z
#define g_fMeshGridSize g_vMeshBlkParams.w

//--------------------------------------------------------------------------------------
//	Pixel Variables
//--------------------------------------------------------------------------------------

float4 g_MainLightColor;
float4 g_vWaterColor;



struct VS_INPUT
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position : POSITION;
	float3 Specular : COLOR1;
	float2 TexCoord : TEXCOORD0;
	float3 CameraDir : TEXCOORD1;
	float4 fogParam   : TEXCOORD2;
};

//--------------------------------------------------------------------------------------
//	samplers
//--------------------------------------------------------------------------------------
sampler2D g_BaseSampler: register(s0);


//--------------------------------------------------------------------------------------
//	Vertex shader
//--------------------------------------------------------------------------------------

VS_OUTPUT vs_Main(VS_INPUT Input)
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

	Output.Position = mul(vInPos, g_matWVP);//FIXME!!
	
	Output.TexCoord = mul(float3(Input.TexCoord, 1.0f), (float3x3)g_matTexTrans).xy;//FIXME!!
	//Output.TexCoord = Input.TexCoord;
	Output.CameraDir = normalize(g_vCameraPos.xyz - vInPos.xyz);
	
	float3 vLightDir = normalize(g_MainLightPos.xyz - vInPos.xyz);
	float3 vHalf = normalize(vLightDir + Output.CameraDir);
	float fHDotN =  saturate(dot(vHalf, Input.Normal));
	Output.Specular = g_MainLightColor.rgb * pow(fHDotN, 25.0f);
	
	Output.fogParam = ComputeCompositeFogSimple(vInPos.xyz - g_vCameraPos.xyz, vInPos.y);

	return Output;
}

//--------------------------------------------------------------------------------------
//	Pixel shader
//--------------------------------------------------------------------------------------

//	Calculate specular factor
float CalcSpecFactor(VS_OUTPUT Input)
{
	float3 vCameraDir = normalize(Input.CameraDir);
	float fFactor = pow(1.0f - vCameraDir.y, 5);
	return fFactor;
}

float4 ps_EyeAboveWater(VS_OUTPUT Input) : COLOR0
{
	float4 colBaseTex = tex2D(g_BaseSampler, Input.TexCoord);
	
	float3 colDiffuse = lerp(g_vWaterColor.rgb, colBaseTex.rgb, colBaseTex.a);
	float fSpecFactor = CalcSpecFactor(Input);
	float3 colSpecular = Input.Specular.rgb * fSpecFactor * colBaseTex.a * 8.0f;
	float fAlpha = colBaseTex.a + g_vWaterColor.a;
	
	float3 color = colDiffuse + colSpecular;
	
	#ifdef _FOG_    
    color.rgb = lerp(Input.fogParam.rgb, color.rgb, Input.fogParam.a);
	#endif
	return float4(color, fAlpha);
}

float4 ps_EyeUnderWater(VS_OUTPUT Input) : COLOR0
{
	float4 colBaseTex = tex2D(g_BaseSampler, Input.TexCoord);
	
	float3 color = lerp(g_vWaterColor.rgb, colBaseTex.rgb, colBaseTex.a);
	float fAlpha = colBaseTex.a + g_vWaterColor.a;
	
	#ifdef _FOG_    
    color.rgb = lerp(Input.fogParam.rgb, color.rgb, Input.fogParam.a);
	#endif
	
	return float4(color, fAlpha);
}


