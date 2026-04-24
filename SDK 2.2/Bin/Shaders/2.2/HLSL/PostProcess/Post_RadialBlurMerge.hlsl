string entrynames = "ps_2_0 ps_main";

sampler backBufferSampler : register(s0);
sampler bluredSampler : register(s1);

float2 g_vRadialBlurCenter;
float g_fRbBlurLength;
float g_fInFocusRadius;
float g_fOutFocusRadius;

float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{     
	float4 output = 0;
	float len = length(g_vRadialBlurCenter - texcoord);
	float2 dir = (g_vRadialBlurCenter - texcoord) / len;
	
	len = smoothstep(g_fInFocusRadius, g_fOutFocusRadius, len) * g_fRbBlurLength;

	float2 blurVector = dir * len;
 	float2 currentUV = texcoord;

 	float2 deltaUV = blurVector  * g_fRbBlurLength; 
 	
	float blurItem = saturate(length(deltaUV) / 0.01);
	float4 backColor = tex2D(backBufferSampler, texcoord);
	float4 bluredColor = tex2D(bluredSampler, texcoord);
	

	output = lerp(backColor, bluredColor, blurItem);
	return output;
}
