string entrynames = "ps_2_0 ps_main";

float2 g_vPixelSize;
sampler backBufferTextureSampler : register(s0);
	
#define blurWidth 0.75 //0.75

float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{
	float2 du1 = float2(0, g_vPixelSize.y) * blurWidth;
	float2 du2 = float2(0, g_vPixelSize.y * 2) * blurWidth;
	float4 s1 = tex2D(backBufferTextureSampler, texcoord - du2);
	float4 s2 = tex2D(backBufferTextureSampler, texcoord - du1);
	float4 s3 = tex2D(backBufferTextureSampler, texcoord);
	float4 s4 = tex2D(backBufferTextureSampler, texcoord + du1);
	float4 s5 = tex2D(backBufferTextureSampler, texcoord + du2);

	float4 blurred = ( s1 + s2 + s3 + s4 + s5) / 5;
  return blurred;
}