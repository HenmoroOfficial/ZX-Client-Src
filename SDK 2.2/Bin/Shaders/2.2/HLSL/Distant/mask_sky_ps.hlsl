sampler g_DepthSampler: register(s0);
string entrynames = "ps_2_0 ps_main";

float4 ps_main(float2 tex: TEXCOORD0): COLOR
{
    float d = tex2D(g_DepthSampler, tex);
    clip(d - 1.0f);
    return float4(1, 0, 0, 1);
}
