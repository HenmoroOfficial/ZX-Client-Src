string entrynames = "ps_2_0 ps; vs_2_0 vs";	

float2 gpost_vPixelSize : register(c0) = float2(1.f / 800.f, 1.f / 600.f);
sampler g_GBufferSampler	: register(s0);

struct ScreenQuadVSOut
{
    float4 Position	: POSITION;		// vertex position 
    float2 TexCoord	: TEXCOORD0;	// vertex texture coords 
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
    return output;    
}

float4 ps( float2 vTexCoord	: TEXCOORD0) : COLOR0
{
	return tex2D(g_GBufferSampler, vTexCoord);
}