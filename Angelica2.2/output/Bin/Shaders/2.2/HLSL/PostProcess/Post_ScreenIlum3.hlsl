string entrynames = "ps_2_0 ps_main";

sampler g_SamplerLinear;
float2 gpost_vPixelSize;

float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{
	float offSet = gpost_vPixelSize.x * 0.25;
	float4 s1 = tex2D(g_SamplerLinear, texcoord + float2( offSet,  offSet));
	float4 s2 = tex2D(g_SamplerLinear, texcoord + float2(-offSet,  offSet));
	float4 s3 = tex2D(g_SamplerLinear, texcoord + float2( offSet, -offSet));
	float4 s4 = tex2D(g_SamplerLinear, texcoord + float2(-offSet, -offSet));
	float4 r = (s1 + s2 + s3 + s4) * 0.25;
	return float4(exp(r.rgb), r.a);
}