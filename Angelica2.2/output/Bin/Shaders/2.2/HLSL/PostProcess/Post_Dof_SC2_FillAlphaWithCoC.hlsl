string entrynames = "ps_2_0 ps_main";

float g_fZFar;
float g_fNearDis;
float g_fFocusDis1;
float g_fFocusDis2;
float g_fFarDis;
float g_fClampBlurFar;

sampler depthTextureSampler : register(s0);
sampler backBufferSampler   : register(s1);

float ComputeDepthBlur(float depth)
{
	float f;
	if(depth < g_fFocusDis1)
	{
		f = (depth - g_fFocusDis1) / (g_fFocusDis1 - g_fNearDis);
		f = clamp(f, -1.f, 0);
	}
	else if (depth > g_fFocusDis2)
	{
		f = (depth - g_fFocusDis2) / (g_fFarDis - g_fFocusDis2);
		f = clamp(f, 0, g_fClampBlurFar);
	}
	else
		f = 0;
	return f * 0.5f + 0.5f;
}

float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0    
{
	float4 c = 0;
	c.rgb = tex2D(backBufferSampler, texcoord).rgb;
	float depth = tex2D(depthTextureSampler, texcoord).r;
	c.a = ComputeDepthBlur(depth * g_fZFar);
	c.a = abs(c.a - 0.5) / 0.5;
	return c;
}