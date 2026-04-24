string entrynames = "vs_3_0 vs";	

float2 gpost_vPixelSize : register(c0) = float2(1.f / 800.f, 1.f / 600.f);

struct ScreenQuadVSOut
{
    float4 Position	: POSITION;		// vertex position 
    float2 TexCoord	: TEXCOORD0;	// vertex texture coords 
	float3 deltaPos	: TEXCOORD1;
};

ScreenQuadVSOut vs(	float4 pos	: POSITION,
					float2 uv	: TEXCOORD0,
					float3 deltaPos : TEXCOORD1)
{
    ScreenQuadVSOut output;
    output.Position = pos;
	output.Position.x -=  gpost_vPixelSize.x;
	output.Position.y +=  gpost_vPixelSize.y;
    output.TexCoord = uv;  
	output.deltaPos = deltaPos;
    return output;    
}