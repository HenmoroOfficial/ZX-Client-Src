string entrynames = "ps_3_0 ps";	

sampler g_inputSampler	: register(s0);
sampler g_depthSampler  : register(s1);

float4 g_pixelSize : register(c0); 

#define SAMPLE_NUM 2

float4 ps(float2 uv : TEXCOORD0) : COLOR0
{
	float2 pixelSize = g_pixelSize.xy;
	float center_d = tex2D(g_depthSampler, uv).r;
	
	float fSumWeight = 1.f;
	float4 cColor = tex2D(g_inputSampler, uv);
	//return cColor;
	
	for(int i = 0; i < SAMPLE_NUM; i++)
	{
		float2 sampleTC[2];
		float2 offset = float2(0, g_pixelSize.y) * (i + 1);
		sampleTC[0] = uv + offset;
		sampleTC[1] = uv - offset;
		for(int j = 0; j < 2; j++)
		{
			float d = tex2D(g_depthSampler, sampleTC[j]).r;
			float ddiff = abs((center_d - d)/center_d);
			float w = 1;
			if(ddiff > 0.01)
				w = 0;
			cColor += tex2D(g_inputSampler, sampleTC[j]) * w;
			fSumWeight += w;
		}
	}
	
	return cColor / fSumWeight;
}