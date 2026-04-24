#include "..\fog\fog_common.h"

string entrynames = "ps_3_0 ps_main";	

float g_ZFar;
float4 g_FocalLen;				//float2 g_FocalLen
								//float2 g_InvFocalLen;
float4x4 matWorld    	: World;
float3 g_vecEyePos 		: EyePos;
float4x4 viewInv 		: ViewInverse;

sampler g_GBufferSampler;
sampler g_BackBuffer;

float3 UV2Eye(float2 uv, float eyeZ, float2 g_InvFocalLen)
{
    uv = (uv * float2(2.0, -2.0) - float2(1.0, -1.0));
    return float3(uv * g_InvFocalLen * eyeZ, eyeZ);
}

float4 ps_main(   float2 uv   : TEXCOORD0,
				  float3 norm : TEXCOORD1) : COLOR0
{
	const float2 g_InvFocalLen = g_FocalLen.zw;
    const float2 centerTC = uv;
	float center_d_0_1 = tex2D(g_GBufferSampler, uv).r;
	float center_d = center_d_0_1 * g_ZFar;

	float4 backColor = tex2D(g_BackBuffer, uv);
	
	//return float4(center_d_0_1,center_d_0_1,center_d_0_1,1);
	if(center_d_0_1 > 0.99)
		return backColor;
	// Он
	//float3 eyePos = UV2Eye(uv, center_d, g_InvFocalLen);
	float3 worldPos = g_vecEyePos + norm * center_d_0_1;
	
	//mul(float4(eyePos, 1), viewInv);
	
	//return float4(worldPos / 100.f, 1);
	
	
	float4 fogColor = ComputeCompositeFogSimple(worldPos.xyz - g_vecEyePos, worldPos.y);
	backColor.rgb = lerp(fogColor.rgb, backColor.rgb, fogColor.a);

	return backColor;
}



