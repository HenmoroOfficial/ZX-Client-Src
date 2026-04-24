string entrynames = "ps_2_0 ps_main";

sampler BaseSampler : register(s0);
sampler MaskSampler : register(s1);
float4 g_crMask;
float4x4 matColorTone;
	
struct VS_OUTPUT
{
	float4 position : POSITION;   // vertex position
	float4 color    : COLOR0;
	float2 texcoord : TEXCOORD0;  // vertex texture coords
};

float4 ps(VS_OUTPUT Input) : COLOR0
{
	float4 mask = tex2D(MaskSampler, Input.texcoord);
	float4 color = tex2D(BaseSampler, Input.texcoord);
	float4 c = mul(float4(color.xyz, 1), matColorTone);
	return float4(c.xyz/c.w, saturate(dot(mask, g_crMask)));
}
