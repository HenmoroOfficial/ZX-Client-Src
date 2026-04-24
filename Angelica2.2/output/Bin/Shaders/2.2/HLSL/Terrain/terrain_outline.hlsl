#include "..\fog\fog_common.h"
#include "..\Shadow\Shadow_common.h"

string entrynames = "vs_2_0 vs_main;ps_2_0 ps_main;ps_2_0 ps_main_detail";

string macro0
<
	string name = "_FOG_";
	string desc = "_FOG_";
	string type = "PRG";
	string GroupID = "1";
>;

string macro1
<
	string name = "_SHADOW_";
	string desc = "是否开阴影";
	string type = "PRG";
	string GroupID = "2";
>;

string macro2
<
	string name = "_HWPCF_";
	string desc = "是否开硬件PCF";
	string type = "PRG;SDR_3_0";
	string GroupID = "3";
>;

float4x4 g_matViewProjection;
float3 g_vecEyePos;



sampler g_TextureSamplerDay   : register(s0);
sampler g_TextureSamplerNight : register(s1);

float g_fDNFactor;

//Shadow
float4x4 g_matLightWVP;	
float4 g_SettingShadowMapSize;
sampler	  g_ShadowMapSampler : register(s3);
sampler   g_ShadowMapFloat : register(s4);

struct VS_INPUT
{
	float4 Position : POSITION;
	float2 Tex		: TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Position 	: POSITION;
    float2 Tex      	: TEXCOORD0;
	float4 fogParam 	: TEXCOORD1; // fog
	float4 shadowProj 	: TEXCOORD2; // shadow
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
   	VS_OUTPUT Output;
    Output.Position = mul(Input.Position, g_matViewProjection);
    Output.Tex = Input.Tex;
    Output.fogParam = ComputeCompositeFogSimple(Input.Position.xyz - g_vecEyePos, Input.Position.y);
	Output.shadowProj = glb_ComputeShadowProj(float4(Input.Position), g_matLightWVP);
    return Output;
}

float4 ps_main(VS_OUTPUT Input) : COLOR
{
    float4 colorDay = tex2D(g_TextureSamplerDay, Input.Tex);
    float4 colorNight = tex2D(g_TextureSamplerNight, Input.Tex);
    float4 terrainColor = lerp(colorDay, colorNight, g_fDNFactor);
#ifdef _FOG_    
	float4 fogColor = Input.fogParam;
	return float4(lerp(fogColor.rgb, terrainColor.rgb, fogColor.a), 1);
#else
    return terrainColor;
#endif	
}


struct PS_INPUT
{
    float4 Position 	: POSITION;
    float2 Tex      	: TEXCOORD0;
	float4 fogParam 	: TEXCOORD1; // fog
	float4 shadowProj 	: TEXCOORD2; // shadow
};


sampler g_TextureSamplerDetail : register(s2);
float g_fScaleDetail;

float4 ps_main_detail(PS_INPUT Input) : COLOR
{
    float4 colorDay = tex2D(g_TextureSamplerDay, Input.Tex);
    float4 colorNight = tex2D(g_TextureSamplerNight, Input.Tex);
    float4 colorDetail = tex2D(g_TextureSamplerDetail, Input.Tex * g_fScaleDetail);
    float4 terrainColor = lerp(colorDay, colorNight, g_fDNFactor) * colorDetail * 2;
#ifdef _FOG_    
	float4 fogColor = Input.fogParam;
	terrainColor = float4(lerp(fogColor.rgb, terrainColor.rgb, fogColor.a), 1);
#endif
    
	
	
#ifdef _SHADOW_
	bool bHWPC = false;
	#ifdef _HWPCF_
		bHWPC = true;
	#endif
	float fShadowMapSize = g_SettingShadowMapSize.x;
	float4 vShadow = glb_ComputShadowColor(g_ShadowMapSampler, g_ShadowMapFloat, 
		Input.shadowProj, true, bHWPC, fShadowMapSize, g_SettingShadowMapSize.y);	
#else
	float4 vShadow = float4(1,1,1,1);
#endif
	//return float4(vShadow.xyz, 1);
	
	
	terrainColor *= vShadow;
	
	return terrainColor;
}
