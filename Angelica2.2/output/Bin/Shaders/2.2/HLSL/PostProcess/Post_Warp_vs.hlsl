string entrynames = "vs_2_0 vs_main";

float2 g_vPixelSize = float2(1.f / 800.f, 1.f / 600.f);
float g_fTime;
float g_fWarpSpeed;

struct WarpVSOut
{
	float4 position : POSITION;   // vertex position 
	float2 texcoord : TEXCOORD0;  // vertex texture coords 
	float2 pertubeTC : TEXCOORD1;
};

WarpVSOut vs_main(float4 pos : POSITION, 
									float2 uv  : TEXCOORD0)
{
	WarpVSOut output;
	output.position = pos;
	output.position.x -=  g_vPixelSize.x;
	output.position.y +=  g_vPixelSize.y;
	output.texcoord = uv;  
	output.pertubeTC = uv;
	output.pertubeTC.x *= 2.f;
	output.pertubeTC.y *= 2.f; 
	float yd = g_fTime * 0.5f * g_fWarpSpeed;
	if( yd > 1.0f ) yd -= 1.0f;
	output.pertubeTC.y += yd;

	return output;
}