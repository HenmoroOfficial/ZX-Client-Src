#include "..\common\common.h"

string entrynames = "ps_2_0 ps_showDepth; ps_2_0 ps_maxDownSample; ps_2_0 ps_writeDepth";

sampler2D g_DepthSampler
<
    string MagFilter = "POINT";
    string MinFilter = "POINT";
    string MipFilter = "POINT";
>;

float4 g_PixelOffset[2];

float4 ps_showDepth(float2 Tex: TEXCOORD0): COLOR 
{
    float4 color = tex2D(g_DepthSampler, Tex);
    float z = glb_GetLinearDepth(color.x);
    return float4(z, z, z, 1);
}

float4 ps_maxDownSample(float2 Tex: TEXCOORD0): COLOR 
{
    float fDepth0 = tex2D(g_DepthSampler, Tex + g_PixelOffset[0].xy).x;
    float fDepth1 = tex2D(g_DepthSampler, Tex + g_PixelOffset[0].zw).x;
    float fDepth2 = tex2D(g_DepthSampler, Tex + g_PixelOffset[1].xy).x;
    float fDepth3 = tex2D(g_DepthSampler, Tex + g_PixelOffset[1].zw).x;
    float fDepth = max(max(fDepth0, fDepth1), max(fDepth2, fDepth3));
    return fDepth;
}

float4 ps_writeDepth(float2 Tex: TEXCOORD0, out float fDepth: DEPTH) : COLOR0
{
    fDepth = tex2D(g_DepthSampler, Tex).x;
    return float4(0.0, 0.0, 0.0, 0.0);
}