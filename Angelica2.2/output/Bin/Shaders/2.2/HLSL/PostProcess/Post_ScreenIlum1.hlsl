string entrynames = "ps_2_0 ps_main";

sampler g_SamplerLinear;
sampler g_SamplerLInearDepth;

float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0  //to 64x64
{
	const float3 LUMINANCE_VECTOR  = float3(0.2125f, 0.7154f, 0.0721f);
	float3 sample =  tex2D(g_SamplerLinear, texcoord).xyz;
	float  fLogLumSum = log( max(dot(sample, LUMINANCE_VECTOR), 0.0001f)); //做一个截断. 万一小于0就会悲剧了.
	float depth = tex2D(g_SamplerLInearDepth, texcoord);
	return float4(fLogLumSum, fLogLumSum, fLogLumSum, depth);
}