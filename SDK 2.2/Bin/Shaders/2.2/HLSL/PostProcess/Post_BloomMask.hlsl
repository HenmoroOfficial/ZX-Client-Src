string entrynames = "ps_2_0 ps_main";

sampler g_SamplerMask : register(s0);
float g_fBrightScale;


float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{	
	float4 bloomColor = tex2D(g_SamplerMask, texcoord);
	float mask = bloomColor.w * g_fBrightScale;
	float4 outColor = bloomColor.xyzw * mask;
	outColor.w = 1.0;

	return outColor;
}