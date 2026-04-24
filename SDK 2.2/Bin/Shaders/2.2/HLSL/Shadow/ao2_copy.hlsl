string entrynames = "ps_3_0 ps_main";

sampler g_inputSampler	: register(s0);

float4 ps_main(float2 uv : TEXCOORD0) : COLOR0
{
	return tex2D(g_inputSampler, uv);
}