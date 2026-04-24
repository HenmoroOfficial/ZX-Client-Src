string entrynames = "ps_3_0 ps";

sampler g_GBufferSampler	: register(s0);	//A3D_SAMPLER2D_DEF_SIMPLE(g_GBufferSampler, g_GBuffer, LINEAR, CLAMP)
float4 g_param				: register(c0);
//float2 vScreenPixelSize : 

float4 ps( float2 vTexCoord	: TEXCOORD0) : COLOR0
{
	float2 vHalfScreenPixelSize = g_param.xy * 0.5;
	float2 offset[4] =
	{
		float2(1, 1),
		float2(1, -1),
		float2(-1, 1),
		float2(-1, -1)
	};
	
	float4 z = 0;
	for(int i = 0; i < 4; i++)
	{
		float4 zr = tex2D(g_GBufferSampler, vTexCoord + offset[i] * vHalfScreenPixelSize);
		z += zr * 0.25;
	}
	return z;
}