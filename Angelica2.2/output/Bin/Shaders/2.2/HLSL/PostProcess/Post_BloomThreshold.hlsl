string entrynames = "ps_2_0 ps_main";

sampler g_Sampler 	: register(s0);
float g_fThreshold;
float g_fBrightScale;
float2 g_vPixelSize;


float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{
	//return float4(1,0,0,1);
	float2 hs = g_vPixelSize * 0.5;
	float4 c0 = tex2D(g_Sampler, texcoord + float2(hs.x, hs.y));
	float4 c1 = tex2D(g_Sampler, texcoord + float2(-hs.x, hs.y));
	float4 c2 = tex2D(g_Sampler, texcoord + float2(hs.x, -hs.y));
	float4 c3 = tex2D(g_Sampler, texcoord + float2(-hs.x, -hs.y));
	float4 c = (c0 + c1 + c2 + c3) * 0.25;
	//c = c * c * 2.0;
	float t = c.r * 0.299 + c.g * 0.587 + c.b * 0.114;
	if( t > g_fThreshold)
		return float4((c * g_fBrightScale * (t - g_fThreshold)).rgb,1);
	else
		return float4(0,0,0,1);
}