string entrynames = "ps_2_0 ps; vs_2_0 vs";	

#include "water_fft_common.h"

sampler2D	sceneDepthSampler 	: register(s0);		//POINT Clamp
sampler2D	causticSampler : register(s1);
sampler2D	backBufferSampler : register(s2);
sampler2D   godRaySampler : register(s3);

float3 g_vCameraPos : register(c0);
float g_fFogDensity : register(c1);
float g_fWaterFogScaleExp : register(c2);
float3 g_waterFogColor : register(c3);
float g_fWaterHeight : register(c4);

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
    float3 worldP = In.n * zFar01 + g_vCameraPos.xyz;
	
	float fLength = length(worldP - g_vCameraPos);
	
	float4 cWaterFog = GetWaterVolumetricFogColor(fLength, g_fFogDensity, g_fWaterFogScaleExp, g_waterFogColor);
		
	float3 cBackColor = tex2D(backBufferSampler, In.uv).rgb;
	float3 cCaustic = tex2D(causticSampler, In.uv).rgb;
		
	float4 g_fCausticLerpDis = float4(0.5, 2, 3, 6);
	float fCausticIllum = GetCausticIllum(g_fCausticLerpDis, g_fWaterHeight - worldP.y); // 根据深度淡入淡出焦散颜色	
	
	float3 result = cBackColor + cCaustic * fCausticIllum;
	result = lerp( cWaterFog.xyz, result, cWaterFog.w );
	
	result += tex2D(godRaySampler, In.uv).xyz;
	
	return float4(result, 1);
	//refractColor.xyz = 
	
	
	
}
