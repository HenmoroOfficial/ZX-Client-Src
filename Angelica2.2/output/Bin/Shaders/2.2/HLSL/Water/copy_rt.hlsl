string entrynames = "ps_2_0 ps; vs_2_0 vs";	

float2 g_vPixelSize = float2(1.f / 800.f, 1.f / 600.f);
sampler srcRT : register(s0);

void vs(in float4 position : POSITION,
	in float2 texcoord : TEXCOORD0,
	out float4 oPosition : POSITION,
	out float2 oTexcoord : TEXCOORD0)
{
	oPosition = position;
	oPosition.x -=  g_vPixelSize.x;
	oPosition.y +=  g_vPixelSize.y;
	oTexcoord = texcoord;
}



float4 ps(in float2 texcoord : TEXCOORD0) : COLOR0
{
	return tex2D(srcRT, texcoord).rgba;
}