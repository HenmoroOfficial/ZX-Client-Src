//-------------------------------------------------
//FileName : Editor_PlaneMesh_VS.hlsl
//Created by liyi 2012, 1, 11
//-------------------------------------------------

float4x4 g_matWorldViewProj;
float4x4 g_matCastShadowWVP;

string entrynames = "vs_2_0 vs_main";

struct VS_INPUT
{
	float4 Position:POSITION;
};

struct VS_OUTPUT
{
	float4 Position:POSITION;
	float4 TexCoord:TEXCOORD0;
	float4 WorldPos:TEXCOORD1;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;
	Output.Position = mul(Input.Position, g_matWorldViewProj);
	Output.TexCoord = mul(Input.Position, g_matCastShadowWVP);
	Output.WorldPos = Input.Position;
	return Output;
}