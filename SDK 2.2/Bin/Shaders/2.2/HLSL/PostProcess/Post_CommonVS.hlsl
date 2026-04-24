string entrynames = "vs_2_0 vs_main";

float2 g_vPixelSize = float2(1.f / 800.f, 1.f / 600.f);

void vs_main(in float4 position : POSITION,
			 in float2 texcoord : TEXCOORD0,
			 in float2 texcoord1 : TEXCOORD1,
			 out float4 oPosition : POSITION,
			 out float2 oTexcoord : TEXCOORD0,
			 out float2 oTexcoord1 : TEXCOORD1)
{
	oPosition = position;
	oPosition.x -=  g_vPixelSize.x;
	oPosition.y +=  g_vPixelSize.y;
	oTexcoord = texcoord;
	oTexcoord1 = texcoord1;
}
