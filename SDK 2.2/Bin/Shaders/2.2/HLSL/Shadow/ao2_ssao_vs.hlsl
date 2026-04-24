string entrynames = "vs_3_0 vs";

//float2 gpost_vPixelSize : register(c0);
//float2 g_ResolutionSmall : register(c1) = float2(800 / 2,600 / 2);
float4 g_resolutionSmall : register(c0) = float4(2 / 800, 2 / 600, 800 / 2, 600 / 2);

float4 g_CurrentFrame	 : register(c1);

struct VSOut
{
    float4 pos  : POSITION;   // vertex position 
    float2 uv  	: TEXCOORD0;  // vertex texture coords 
    float2 uv2  : TEXCOORD1;
	float3 deltaPos : TEXCOORD2;
};
VSOut vs( float4 pos : POSITION,
		  float2 uv : TEXCOORD0,
		  float3 deltaPos : TEXCOORD1)
{
    VSOut output;
    output.pos = pos;
    output.uv = uv;  
	output.pos.x -=  g_resolutionSmall.x;
	output.pos.y +=  g_resolutionSmall.y;

	output.uv2 = uv * g_resolutionSmall.zw / 4;     //line 是4

	//只做4帧的累积
	float f_2 = g_CurrentFrame / 2.f;
	float2 offset = float2( floor(f_2) / 2.f, frac(f_2));
	output.uv2 += offset;
	
	output.deltaPos = deltaPos;
	
    return output; 
}