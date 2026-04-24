string entrynames = "ps_2_0 ps_main";

float2 g_srcPixelSize;

sampler backBufferTextureSampler		: register(s0);

float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{
	float2 o = g_srcPixelSize * 0.5;
	float4 c1 = tex2D(backBufferTextureSampler, texcoord + float2(o.x, o.y));
	float4 c2 = tex2D(backBufferTextureSampler, texcoord + float2(-o.x, o.y));
	float4 c3 = tex2D(backBufferTextureSampler, texcoord + float2(o.x, -o.y));
	float4 c4 = tex2D(backBufferTextureSampler, texcoord + float2(-o.x, -o.y));
	return (c1 + c2 + c3 + c4) / 4.f;
}