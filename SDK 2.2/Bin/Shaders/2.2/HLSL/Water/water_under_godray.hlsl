string entrynames = "ps_2_0 ps; vs_2_0 vs";	

#include "water_fft_common.h"

sampler2D	sceneDepthSampler 	: register(s0);		//POINT Clamp
sampler2D	oceanBumpMapSampler : register(s1);		//LINEAR Wrap
sampler2D   causticSampler1		: register(s2);
sampler2D	causticSampler2		: register(s3);

float3 g_vCameraPos : register(c0);
float g_fFogDensity : register(c1);
float g_fWaterFogScaleExp : register(c2);
float3 g_waterFogColor : register(c3);
float g_fWaterHeight : register(c4);

float4x4 g_lightViewMat : register(c5);
float3 g_PS_SunLightDir : register(c9); 	//太阳光方向 = float4(1,1,1);
float g_fTime : register(c10);

float2 g_vCausticOffset : register(c11);
float g_fCausticLerp : register(c12);
float g_fStartDis : register(c13);

struct VSOutPositionUVNormal
{
    float4 pos   : POSITION;   // vertex position 
    float2 uv    : TEXCOORD0;  // vertex texture coords 
    float3 n    : TEXCOORD1;
};

VSOutPositionUVNormal vs(float4 pos : POSITION,
                         float2 uv : TEXCOORD0,
                         float3 n : NORMAL)
{
    VSOutPositionUVNormal output;
    output.pos = pos;
    output.uv = uv;
    output.n = n;
    return output;
}

float4 ps(VSOutPositionUVNormal In) : COLOR0
{
    float4 output = 0;
    float zFar01 = tex2D(sceneDepthSampler, In.uv).r;
	
	float3 cCaustic = 0;
	float4 g_fCausticLerpDis = float4(0.5, 2, 4, 6);
	
#if ((PROFILE == 3) || defined(_SDR_3_0))
	for(int i = 0; i < 8; i++)
#else
	for(int i = 0; i < 2; i++)
#endif
	{
		float3 worldP = g_vCameraPos.xyz + In.n * (0.00005 * (1 + (i + g_fStartDis) / 32 * 0.5)) * (i + g_fStartDis); /*In.n * zFar01 + g_vCameraPos.xyz*/;
		//cCaustic += ComputeCausticColor(worldP, g_PS_SunLightDir, (float3x3)(g_lightViewMat), g_fTime, oceanBumpMapSampler);
		float fCausticIllum = GetCausticIllum(g_fCausticLerpDis, g_fWaterHeight - worldP.y);
		cCaustic += ComputeCausticColor2(worldP, (float3x3)(g_lightViewMat), g_vCausticOffset, causticSampler1, causticSampler2, g_fCausticLerp) * fCausticIllum;
	}
	cCaustic /= (32.0);
	cCaustic *= 0.7;
	
	//return float4(0,0,0,0);
	
	return float4(cCaustic,1);
/*	
	float fLength = length(worldP - g_vCameraPos);
	
	float4 cWaterFog = GetWaterVolumetricFogColor(fLength, g_fFogDensity, g_fWaterFogScaleExp, g_waterFogColor);
		
	float3 cBackColor = tex2D(backBufferSampler, In.uv).rgb;
	float3 cCaustic = tex2D(causticSampler, In.uv).rgb;
		
	float4 g_fCausticLerpDis = float4(0.5, 2, 3, 6);
	float fCausticIllum = GetCausticIllum(g_fCausticLerpDis, g_fWaterHeight - worldP.y); // 根据深度淡入淡出焦散颜色	
	
	float3 result = cBackColor + cCaustic * fCausticIllum;
	result = lerp( cWaterFog.xyz, result, cWaterFog.w );
	return float4(result, 1);
*/	
	
}