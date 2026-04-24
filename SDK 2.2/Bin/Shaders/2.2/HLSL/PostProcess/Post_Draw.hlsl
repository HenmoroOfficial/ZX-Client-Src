string entrynames = "ps_2_0 ps_main";

sampler sampSrcColor : register(s0);

float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{
	return tex2D(sampSrcColor, texcoord);
}

