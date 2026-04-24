string entrynames = "ps_2_0 ps_main";

string macro0
<
	string name = "_FAST_";
	string desc = "ÊÇ·ñ¿ìËÙ";
	string type = "PRG";
	string GroupID = "1";
>;

sampler backBufferSampler : register(s0);

float2 g_vRadialBlurCenter;
float g_fRbBlurLength;
float g_fInFocusRadius;
float g_fOutFocusRadius;

float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{     
#ifdef _FAST_
	const int g_fRadialBlurSampleNum = 2;
#else
	const int g_fRadialBlurSampleNum = 8;
#endif	
	float4 output = 0;
	float len = length(g_vRadialBlurCenter - texcoord);
	float2 dir = (g_vRadialBlurCenter - texcoord) / len;
	
	len = smoothstep(g_fInFocusRadius, g_fOutFocusRadius, len) * g_fRbBlurLength;

	float2 blurVector = dir * len;
 	float2 currentUV = texcoord;

 	float2 deltaUV = blurVector / (float)(g_fRadialBlurSampleNum) * g_fRbBlurLength; 
 	float4 blurred = 0;

	for(int i = 0; i < g_fRadialBlurSampleNum; i++) 
	{
		blurred += tex2D(backBufferSampler, currentUV );
		currentUV += deltaUV;
	}
	blurred /= (float)g_fRadialBlurSampleNum;

	output = blurred;
	return output;
}
