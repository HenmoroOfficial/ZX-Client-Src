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


sampler g_ShadowMapSampler : register(s0);
sampler g_ShadowMapFloat : register(s1);


sampler2D g_BaseSampler0 : register(s2);
sampler2D g_BaseSampler1 : register(s3);
sampler2D g_BaseSampler2 : register(s4);
sampler2D g_MaskMapSampler0 : register(s5);
//	g_MaskMap is mask texture of the first 3 base texture layers
//	red   - layer 3 mask
//	green - layer 2 mask
//	blue  - layer 1 mask

sampler2D g_LMDaySampler   : register(s6);
sampler2D g_LMNightSampler : register(s7);
sampler2D g_ColTexSampler  : register(s8);


float3 g_LayerEnableFlags0;
float3 g_vProjAxis0;
float4 g_LitmapParams; // {scaleDay, scaleNight, 0, g_fDNFactor};
float3 g_vScaleU0 = {1.0f, 1.0f, 1.0f};
float3 g_vScaleV0 = {1.0f, 1.0f, 1.0f};
float4 g_SettingShadowMapSize;	//x: shadow map size  y: sample num

#include "terrain_common.h"

VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT Output;
	glb_vs_Common(input, Output);

	float2 projUV[3] = 
	{
		input.uv2,
		input.uv0,
		input.uv1,
	};

	Output.uv01 = float4(projUV[g_vProjAxis0.x] * float2(g_vScaleU0.x, g_vScaleV0.x), projUV[g_vProjAxis0.y] * float2(g_vScaleU0.y, g_vScaleV0.y));
	Output.uv23 = float4(projUV[g_vProjAxis0.z] * float2(g_vScaleU0.z, g_vScaleV0.z), 0, 0);
	Output.uv45 = float4(0, 0, 0, 0);
	Output.uvMask = input.uv3;
	return Output;
}

float4 ps_main(PS_INPUT input) : COLOR0
{
	float4 base0 = tex2D(g_BaseSampler0, input.uv01.xy);
	float4 base1 = tex2D(g_BaseSampler1, input.uv01.zw);
	float4 base2 = tex2D(g_BaseSampler2, input.uv23.xy);
	float3 mask = tex2D(g_MaskMapSampler0, input.uvMask).rgb * g_LayerEnableFlags0;
	
	float4 color = base0 * mask.b;
	color = lerp(color, base1, mask.g);
	color = lerp(color, base2, mask.r);

	float4 cmData = tex2D(g_ColTexSampler, input.uvMask);
	color.rgb = (color.rgb * cmData.rgb * g_fColTexMax);
	
	float4 lm0 = tex2D(g_LMDaySampler, input.uvMask) * g_LitmapParams.x;
	float4 lm1 = tex2D(g_LMNightSampler, input.uvMask) * g_LitmapParams.y;
	float4 lmColor = lerp(lm0, lm1, g_LitmapParams.w);
	
#ifdef _SHADOW_
	bool bHWPC = false;
	#ifdef _HWPCF_
		bHWPC = true;
	#endif
	float fShadowMapSize = g_SettingShadowMapSize.x;
	float4 vShadow = glb_ComputShadowColor(g_ShadowMapSampler, g_ShadowMapFloat, 
		input.shadowProj, true, bHWPC, fShadowMapSize, g_SettingShadowMapSize.y);	
#else
	float4 vShadow = float4(1,1,1,1);
#endif
	
	float4 terrainColor = glb_CalcFinalOutput(color, lmColor, input.oSpecular, input.oDiffuse, vShadow);
#ifdef _FOG_	
	return float4(lerp(input.fogParam.rgb, terrainColor.rgb, input.fogParam.a), 1);
#else	
    return terrainColor;
#endif

}