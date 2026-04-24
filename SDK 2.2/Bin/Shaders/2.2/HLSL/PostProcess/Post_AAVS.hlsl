string entrynames = "vs_2_0 vs_main";

float2 g_vPixelSize = float2(1.f / 800.f, 1.f / 600.f);

void vs_main(in float4 position : POSITION,
			 in float2 texcoord : TEXCOORD0,
			 out float4 oPosition : POSITION,
			 out float4 oTexcoord : TEXCOORD0)
{
	#define FXAA_SUBPIX_SHIFT (1.0/4.0)
	oPosition = position;
	oPosition.x -=  g_vPixelSize.x;
	oPosition.y +=  g_vPixelSize.y;
	oTexcoord.xy = texcoord;
	oTexcoord.zw = texcoord - (g_vPixelSize * (0.5 + FXAA_SUBPIX_SHIFT));
}