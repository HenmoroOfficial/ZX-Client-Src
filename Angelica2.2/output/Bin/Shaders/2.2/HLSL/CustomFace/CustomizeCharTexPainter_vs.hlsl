string entrynames = "vs_2_0 vs_main";

float4x4 g_matWVP;

struct VS_OUTPUT
{
	float4 position : POSITION;   // vertex position 
	float4 color    : COLOR0;
	float2 texcoord : TEXCOORD0;  // vertex texture coords 
};

VS_OUTPUT vs(
  float4 pos   : POSITION, 
  float4 color : COLOR0,
  float2 uv    : TEXCOORD0)
{
	VS_OUTPUT output;
	output.position = mul(pos, g_matWVP);
	output.texcoord = uv;  
	output.color = color;
	return output;
}