string entrynames = "vs_2_0 vs_main;ps_2_0 ps_main;ps_2_0 ps_main_mask";

float4x4 	g_matViewProj: ViewPorjection;
float4x4    g_matTex;
float4 g_vScreenSize : register(c0);

string macro0
<
	string name = "_3D_";
	string desc = "3D";
	string type = "PRG";
	string GroupID = "1";
>;

struct VS_INPUT
{
	float4 position     : POSITION0;
	float2 tex          : TEXCOORD0;
	float2 tex1         : TEXCOORD1;
	float2 tex2         : TEXCOORD2;
};

struct VS_OUTPUT
{
	float4 position     : POSITION0;
	float2 tex          : TEXCOORD0;
	float4 tex1         : TEXCOORD1;
	float2 tex2         : TEXCOORD2;
};

struct PS_INPUT
{
	float4 position     : POSITION0;
	float2 tex          : TEXCOORD0;
	float4 tex1         : TEXCOORD1;
	float2 tex2         : TEXCOORD2;
#ifdef _SDR_3_0	
	float2 screenSpace  : VPOS;
#endif
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output;
	
    Output.position = mul(Input.position, g_matViewProj);
    Output.tex = Input.tex;
    Output.tex1 = mul(Input.position, g_matTex);
    Output.tex2 = Input.tex2;

    return Output;
}

sampler2D g_NormalSampler : register(s0);

sampler2D g_ScreenSampler : register(s1);

sampler2D g_MaskSampler : register(s2);

float4 ps_main_mask(VS_OUTPUT Input): COLOR0
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

float4 ps_main(PS_INPUT Input): COLOR0
{
    
    float4 vNormal = tex2D(g_NormalSampler, Input.tex);
    vNormal.xy = vNormal.xy * 2 - 1;
#ifdef _SDR_3_0	
	float2 vBaseUV = ( Input.screenSpace.xy) / g_vScreenSize.xy + g_vScreenSize.zw;
#else
	#ifdef _3D_    
		float2 vBaseUV = Input.tex1.xy / Input.tex1.w;
	#else
		float2 vBaseUV = Input.tex1.xy;
	#endif    
#endif
    float2 vDeltaUV = Input.tex2 * float2(-1, 1);
    float2 vUV = vBaseUV + vDeltaUV * vNormal.xy * vNormal.a;
    float fMask = tex2D(g_MaskSampler, vUV);
    if (fMask < 1)
        vUV = vBaseUV;
    return tex2D(g_ScreenSampler, vUV);    
}

