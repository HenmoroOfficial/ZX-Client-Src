#include "..\fog\fog_common.h"
#include "..\common\common.h"
string entrynames = "vs_2_0 vs_main;vs_2_0 vs_main_tl;ps_2_0 ps_main";
string macro0
<
	string name = "_FOG_";
	string desc = "FOG";
	string type = "PRG";
	string GroupID = "1";
>;

string macro1
<
	string name = "_SOFT_EDGE_";
	string desc = "SOFT EDGE";
	string type = "PRG;SDR_3_0";
	string GroupID = "2";
>;

float4x4 	matView: register(c1);
float4x4 	matProj: register(c5);
float4x3    matWorld[70] : register(c10);
float4x4    matVPInv: ViewProjectionInverse;
float3      g_vecEyePos;

struct VS_INPUT
{
	float4 position     : POSITION0;
	float4 blendIndices	: BLENDINDICES0;
	float4 diffuse		: COLOR0;
	float2 tex          : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position   	: POSITION0;
	float4 diffuse  	: COLOR0;
	float2 tex       	: TEXCOORD0;
	float4 fogParam     : TEXCOORD6;	
#ifdef _SOFT_EDGE_
	float4 GBufUV       : TEXCOORD2;
#endif    
	
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output;
	float4 vWorldPos;
	vWorldPos.w = 1;	
	// 因为blendIndices是D3DCOLOR，所以最低字节在Blue分量，并且是0..1的
	float nBlendIndex = Input.blendIndices.b * 255.01;
    vWorldPos.xyz = mul(Input.position, matWorld[nBlendIndex]);
    Output.position = mul(vWorldPos, matView);
    Output.position = mul(Output.position, matProj);
    Output.diffuse = Input.diffuse;
    Output.tex = Input.tex;

#ifdef _SOFT_EDGE_
    Output.GBufUV = glb_CalcScreenSpaceTexCoord(Output.position);
#endif    
    Output.fogParam = ComputeCompositeFogSimple(vWorldPos.xyz - g_vecEyePos, vWorldPos.y);

    return Output;
}

struct VS_INPUT_TL
{
	float4 position     : POSITION0;
	float4 diffuse		: COLOR0;
	float2 tex          : TEXCOORD0;
};

float2 g_ScreenRcp: register(c0); // (2 / width, -2 / height)
float2 g_Offset: register(c1); // (x, y)

VS_OUTPUT vs_main_tl(VS_INPUT_TL Input)
{
    VS_OUTPUT Output;

    Output.position = float4((Input.position.xy - g_Offset) * g_ScreenRcp + float2(-1, 1), 
                              Input.position.z, 
                              1.0f / Input.position.w);
    
	Output.position.xyz *= Output.position.w;

    float4 worldPos = mul(Output.position, matVPInv);
    worldPos.xyz /= worldPos.w;
    
    Output.diffuse = Input.diffuse;
    Output.tex = Input.tex;
    Output.fogParam = ComputeCompositeFogSimple(worldPos.xyz - g_vecEyePos, worldPos.y); 
#ifdef _SOFT_EDGE_
    Output.GBufUV = glb_CalcScreenSpaceTexCoord(Output.position);
#endif    
    
    return Output;
}


sampler g_Sampler: register(s0);
sampler g_DepthSampler: register(s1);
float4 g_vSoftEdgeMask; // rgba component:0 is affected by soft edge, 1 is not affected

float gfx_Contrast(float Input, float ContrastPower)
{
#if 1
    bool IsAboveHalf = Input > 0.5;
    float ToRaise = saturate(2 * (IsAboveHalf ? 1-Input : Input));
    float Output = 0.5 * pow(ToRaise, ContrastPower);
    Output = IsAboveHalf ? 1-Output : Output;
    return Output;
#else
    return 1.0 - exp2(-2*pow(2.0 * saturate(Input), ContrastPower));
#endif
}

float4 ps_main(VS_OUTPUT Input): COLOR
{
    float4 color = tex2D(g_Sampler, Input.tex) * Input.diffuse;

#ifdef _SOFT_EDGE_
    float2 vGBufUV = Input.GBufUV.xy / Input.GBufUV.w;

    float preDepth = glb_GetLinearDepth(tex2D(g_DepthSampler, vGBufUV).r);
    float curDepth = glb_GetLinearDepth(Input.GBufUV.z / Input.GBufUV.w);
    float diffDepth = preDepth - curDepth; 

    
    color *= max(g_vSoftEdgeMask, (saturate(diffDepth * 1000.0f)));
#endif    
    
#ifdef _FOG_    
    color.rgb = lerp(Input.fogParam.rgb, color.rgb, Input.fogParam.a);
#endif    

    return color;
}

