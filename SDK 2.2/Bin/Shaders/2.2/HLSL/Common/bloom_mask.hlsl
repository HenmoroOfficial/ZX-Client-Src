string entrynames = "ps_2_0 ps_main";

float4 vBloomCol;
sampler2D g_DiffuseSampler;

float4 ps_main(in float2 uv : TEXCOORD0) : COLOR0
{
	return float4(vBloomCol.x, vBloomCol.y, vBloomCol.z, tex2D(g_DiffuseSampler, uv).a);
}