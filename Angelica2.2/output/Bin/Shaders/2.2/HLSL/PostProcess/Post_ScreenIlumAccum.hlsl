string entrynames = "ps_2_0 ps_main";

sampler g_SamplerLast;
sampler g_SamplerCur;
float g_fElapsedTime;

float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0  //to 64x64
{
	float4 fAdaptedLum = tex2D(g_SamplerLast, float2(0.5f, 0.5f));
    float4 fCurrentLum = tex2D(g_SamplerCur, float2(0.5f, 0.5f));
    float4 fNewAdaptation = fAdaptedLum + (fCurrentLum - fAdaptedLum) * ( 1 - pow( 0.98f, 30 * g_fElapsedTime ) );
    return fNewAdaptation;
}