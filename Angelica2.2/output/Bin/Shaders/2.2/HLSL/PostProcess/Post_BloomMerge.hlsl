string entrynames = "ps_2_0 ps_main";

sampler g_sampBackBuffer	: register(s0);
sampler g_sampBloom1		: register(s1);
sampler g_sampBloom2		: register(s2);
sampler g_sampBloom3		: register(s3);
float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{
	float3 crBackBuffer 	= tex2D(g_sampBackBuffer, texcoord).xyz;
	float3 crBloomColor1 	= tex2D(g_sampBloom1, texcoord).xyz;
	float3 crBloomColor2 	= tex2D(g_sampBloom2, texcoord).xyz;
	float3 crBloomColor3 	= tex2D(g_sampBloom3, texcoord).xyz;
	
	const float w1 = 0.2;
	const float w2 = 0.3;
	const float w3 = 0.5;
	float3 crBloomColor = crBloomColor1 * w1 + crBloomColor2 * w2 + crBloomColor3 * w3;
	//crBloomColor = sqrt(crBloomColor * 0.5);
	float3 color = crBackBuffer.xyz + crBloomColor.xyz;
	
	//color = crBloomColor1.xyz;
	
	return float4(color, 1);
}
