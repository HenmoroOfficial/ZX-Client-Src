string entrynames = "ps_2_0 ps_main";

sampler g_samp0	: register(s0);
sampler g_samp1	: register(s1);

float2 samp0PixelSize;
float fBlurSize;
float fLastPass;

float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{
	
	float4 c0 = tex2D(g_samp0, texcoord); 
	float4 color = 0;
/*	const float2 offset[9] = 	{ 	float2(-1, -1),
									float2(-1, 0),
									float2(-1, 1),
									float2(0, -1),
									float2(0, 0),
									float2(0, 1),
									float2(1, -1),
									float2(1, 0),
									float2(1, 1),
								};
*/
	const float2 offset[4] = { 	float2(-1.5, -1.5),
								float2(-1.5,  1.5),
								float2(1.5,  -1.5),
								float2(1.5,   1.5),
								};
	for(int i = 0; i < 4; i++)
	{
		float4 s = tex2D(g_samp1, texcoord + offset[i] * samp0PixelSize);
		color += s;
	}	
	color /= 4.0;
	if(fLastPass > 0.5)
		return color + c0;
	else
		return color * (1 + fBlurSize) + c0 * (1 - fBlurSize);
}