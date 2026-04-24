#include "..\fog\fog_common.h"

string entrynames = "ps_3_0 ps_main";	

float4 g_param1; //float2 g_InvResolutionSmall = float2(2.f / 800, 2.f / 600);
float4 g_param2; //float g_ZFar;
								//float g_darkest;
								//float g_bAlreadyHasFog;
								//float g_BlurLength	
float4 g_param3;				//float2 g_FocalLen
								//float2 g_InvFocalLen;
//计算雾相关							
#define g_BlurLength 	1.25
#define sampleNum 		16

sampler g_SSAOTextureSampler	: register(s0);
sampler g_GBufferSampler		: register(s1);
sampler g_BackBuffer			: register(s3);
sampler g_SmallGBufferSampler	: register(s4);

float3 UV2Eye(float2 uv, float eyeZ, float2 g_InvFocalLen)
{
    uv = (uv * float2(2.0, -2.0) - float2(1.0, -1.0));
    return float3(uv * g_InvFocalLen * eyeZ, eyeZ);
}

float4 ps_main(float2 uv : TEXCOORD0) : COLOR0
{
	const float2 g_InvResolutionSmall	= g_param1.xy;
	const float g_ZFar					= g_param2.x;
	const float g_darkest 				= g_param2.y;
	const float g_bAlreadyHasFog 		= g_param2.z;
	const float2 g_InvFocalLen 			= g_param3.zw;
	
    const float2 BlurOffset = g_InvResolutionSmall * g_BlurLength;

    const float2 centerTC = uv;

	float fSumWeight = 1;
	float cColor = tex2D(g_SSAOTextureSampler, centerTC).r * fSumWeight;
	
	//float center_d_0_1 = ;
	float center_d = tex2D(g_GBufferSampler, uv).r * g_ZFar;

	/*
	float2 arrOffsets[12] =
	{	 
	  float2( 1, 0),
	  float2( -1, 0),
	  float2( 0, 1),
	  float2( 0, -1),
	  float2( 1,-1),
	  float2(-1,-1),
	  float2( 1, 1),
	  float2(-1, 1),
	  float2( 0,-2),
	  float2( 0, 2),
	  float2( 2, 0),
	  float2(-2, 0)
	};
*/
	const float2 arrOffsets[16] =
	{	 
	  float2( -1.5, -1.5),
	  float2( -1.5, -0.5),
	  float2( -1.5, 0.5),
	  float2( -1.5, 1.5),
	  float2( -0.5, -1.5),
	  float2( -0.5, -0.5),
	  float2( -0.5, 0.5),
	  float2( -0.5, 1.5),
	  float2( 0.5, -1.5),
	  float2( 0.5, -0.5),
	  float2( 0.5, 0.5),
	  float2( 0.5, 1.5),
	  float2( 1.5, -1.5),
	  float2( 1.5, -0.5),
	  float2( 1.5, 0.5),
	  float2( 1.5, 1.5),
	};
	
	for(int i = 0; i < sampleNum; i++)
	{
		float2 sampleTC = centerTC + BlurOffset.xy * arrOffsets[i].xy;
		float d = tex2D( /*g_GBufferSampler */ g_SmallGBufferSampler, sampleTC).r * g_ZFar;
		float ddiff = ( (center_d - d) / center_d );
		float w = 1;
		if(ddiff > 0.005) //0.005
		    w = 0;
		cColor += tex2D(g_SSAOTextureSampler, sampleTC).r * w;
		fSumWeight += w;
	}
	cColor = cColor / fSumWeight;
	cColor = saturate(cColor);

	cColor =  g_darkest + cColor * (1 - g_darkest);						//线性
	
	float3 backColor = tex2D(g_BackBuffer, uv).rgb;
	
	//if(center_d > g_ZFar - 1 )
	//	return float4(backColor, 1);
	
	//return float4(backColor * cColor, 1);
	backColor = backColor * cColor;
	return float4(backColor,1);
	
	// 雾 挪到了下一个pass进行
	//float3 eyePos = UV2Eye(uv, center_d, g_InvFocalLen);
	//float3 worldPos = mul(float4(eyePos, 1), viewInv);
	//float4 fogColor = ComputeCompositeFogSimple(worldPos.xyz - g_vecEyePos, worldPos.y);
	//backColor.rgb = lerp(fogColor.rgb, backColor.rgb, fogColor.a);

	
	
}



