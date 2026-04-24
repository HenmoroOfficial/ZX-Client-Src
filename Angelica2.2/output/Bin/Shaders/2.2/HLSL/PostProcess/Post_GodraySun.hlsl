string entrynames = "ps_2_0 ps_main";

sampler g_BackSampler 	: register(s0);
sampler g_DepthSampler 	: register(s1);

float m_fOcclusionDepthRange_0_1;	//大于此深度的区域颜色会被保留进行Bloom,小于此深度的区域颜色会变为黑
float m_fBloomThreshold;		//亮度大于此部分才会被保留进行Bloom

float4 ps_main(in float2 texcoord  : TEXCOORD0) : COLOR0
{
	float4 backColor = tex2D(g_BackSampler, texcoord);
	const float3 weights = float3(0.2126, 0.7152, 0.0722);
	float backIlum = dot(weights, backColor.rgb);
	
	//backColor = backColor * backColor * 2.0;
	float depth = tex2D(g_DepthSampler, texcoord).r;
	if(depth >= m_fOcclusionDepthRange_0_1 && backIlum >= m_fBloomThreshold)
		return float4(backColor.rgb, 1);
	else
		return float4(0,0,0,0);
/*
	float2 currentUV = texcoord;

	float normalColor;
	float disToSun = length(currentUV - g_vRadialBlurCenter);
	float sunColor = 0.88;
	float fogColor = 0.25;

	if(disToSun < g_sunSize)
		normalColor = sunColor;
	else if(disToSun < g_sunSize * 1.5f)
		normalColor = sunColor - (sunColor - fogColor) * (disToSun - g_sunSize) / (0.5 * g_sunSize);
	else
		normalColor = fogColor;	//有一个朦胧的底色
		
	float depthMask = normalColor;
	return float4(depthMask,depthMask,depthMask,depthMask);
	*/
}