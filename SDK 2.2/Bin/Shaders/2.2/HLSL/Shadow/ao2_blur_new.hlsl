
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
string entrynames = "ps_3_0 ps";	

#endif



float4 g_param1 : register(c0); //float2 g_InvResolutionSmall = float2(2.f / 800, 2.f / 600);
float4 g_param2 : register(c1); //float g_ZFar;
								//float g_darkest;	
float4x4 fogParamsP			:	register(c3);

float4 g_cameraPos			: 	register(c8);
				
#define g_BlurLength 	0.75
#define sampleNum 		4

sampler g_SSAOTextureSampler	: register(s0);
sampler g_GBufferSampler		: register(s1);
sampler g_UnlinearDepthSampler  : register(s2);
sampler g_BackBuffer			: register(s3);
sampler g_SmallGBufferSampler	: register(s4);

float4 ps(	float2 uv : TEXCOORD0,
			float3 deltaPos	: TEXCOORD1 ) : COLOR0
{
	float2 g_InvResolutionSmall		= g_param1.xy;
	float g_ZFar					= g_param2.x;
	float g_darkest 				= g_param2.y;
	float g_bMul 					= g_param2.z;
	
    float2 BlurOffset = g_InvResolutionSmall * g_BlurLength;

    float2 centerTC = uv;

	float fSumWeight = 1;
	float cColor = tex2D(g_SSAOTextureSampler, centerTC).r * fSumWeight;
	
	float center_d_0_1 = tex2D(g_GBufferSampler, uv).r;
	float center_d =  center_d_0_1 * g_ZFar;

    float3 worldP = deltaPos * center_d_0_1 + g_cameraPos.xyz;
	
	float2 arrOffsets[4] =
	{	 
	   float2( 1, 0),
	   float2( -1, 0),
	   float2( 0, 1),
	   float2( 0, -1),
	};
	
	for(int i = 0; i < sampleNum; i++)
	{
		float2 sampleTC = centerTC + BlurOffset.xy * arrOffsets[i].xy;
		float d = tex2D(g_SmallGBufferSampler, sampleTC).r * g_ZFar;
		float ddiff = ( (center_d - d) / center_d );
		float w = 1;
		if(ddiff > 0.005)
		    w = 0;
		cColor += tex2D(g_SSAOTextureSampler, sampleTC).r * w;
		fSumWeight += w;
	}
	cColor = cColor / fSumWeight;
	cColor = saturate(cColor);

	cColor =  g_darkest + cColor * (1 - g_darkest);						//线性
	
	float3 vFog1 = worldP.xyz - g_cameraPos.xyz;
	float4 fogColor = ComputeFog(vFog1, worldP.y, fogParamsP);
	
	
	float fade = fogColor.w;
	//fade = 0: 完全在雾里
	//fade = 1: 在雾外面
	float fogIntensity = 1 - fade;

	if(center_d_0_1 >= (1.f - 1e-2f) )
		fogIntensity = 0.f;
	
	float3 backColor = tex2D(g_BackBuffer, uv).xyz;
	return float4( lerp(backColor * cColor, fogColor.rgb, fogIntensity), 1);

}



