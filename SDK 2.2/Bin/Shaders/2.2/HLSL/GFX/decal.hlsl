#include "..\fog\fog_common.h"

string entrynames = "vs_2_0 vs_main;ps_2_0 ps_main";
string macro0
<
	string name = "_FOG_";
	string desc = "FOG";
	string type = "PRG";
	string GroupID = "1";
>;

float4x4 	g_matViewProj;
float4x4 	g_matTexture;
float3      g_vecEyePos;

struct VS_INPUT
{
	float4 position     : POSITION0;
	float4 diffuse		: COLOR0;
};

struct VS_OUTPUT
{
	float4 position   	: POSITION0;
	float4 diffuse  	: COLOR0;
	float4 tex       	: TEXCOORD0;
	float4 fogParam     : TEXCOORD6;	
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output;
    Output.position = mul(Input.position, g_matViewProj);
    Output.diffuse = Input.diffuse;
    Output.tex = mul(Input.position, g_matTexture);
    Output.fogParam = ComputeCompositeFogSimple(Input.position.xyz - g_vecEyePos, Input.position.y);
    return Output;
}

sampler g_Sampler;

float4 ps_main(VS_OUTPUT Input): COLOR
{
    //return float4(Input.tex.xyz, 1);
    float4 color = tex2D(g_Sampler, Input.tex.xy / Input.tex.w) * Input.diffuse;
    
#ifdef _FOG_    
    color.rgb = lerp(Input.fogParam.rgb, color.rgb, Input.fogParam.a);
#endif    

    return color;
}

