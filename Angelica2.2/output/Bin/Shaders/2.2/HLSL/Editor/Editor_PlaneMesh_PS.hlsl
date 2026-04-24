//-------------------------------------------------
//FileName : Editor_PlaneMesh_PS.hlsl
//Created by liyi 2012, 1, 11
//-------------------------------------------------

float3 g_vPtLightPos;
float4 g_vPtAtten;	//z is range, w is on/off flag
float4 g_colPtDiffuse;

string entrynames = "ps_2_0 ps_main";

struct PS_INPUT
{
	float4 TexCoord:TEXCOORD0;
	float4 WorldPos:TEXCOORD1;
};

float2 CalcTexCoord(float4 Input)
{
	//Input /= Input.w;
	return float2(Input.x *0.5f + 0.5f, Input.y * (-0.5f) + 0.5f);
}

sampler2D g_CastShadowSampler;

float4 SamplerCastShadow(float2 TexCoord)
{
	float4 Color = tex2D(g_CastShadowSampler, TexCoord); 
	//if(Color.r == 0.0f && Color.g == 0.0f && Color.b == 0.0f)
	if(Color.a == 0.0f)
	{
		Color = float4(0.8f, 0.8f, 0.8f,1.0f);
	}
	else
	{	
		Color = float4(0.3f, 0.3f, 0.3f, 1.0f);
	}
	return Color;
}



float g_fCastShadowSize = 1024.0f;

float4 ps_main(PS_INPUT Input):COLOR0
{
	float2 TexCoord = CalcTexCoord(Input.TexCoord);
	float4 Color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	float fOffset = 1.0f/g_fCastShadowSize;
	float2 TexCoordOffset[5] =
	{
		{0.0f, 0.0f},
		{0.0f, fOffset},
		{fOffset, 0.0f},
		{-fOffset, 0.0f},
		{0.0f, -fOffset}
	
	};

	float3 WorldPos = Input.WorldPos.xyz / Input.WorldPos.w;
	float3 vDelta = g_vPtLightPos - WorldPos;
	float d = length(vDelta);
	float e = (d / g_vPtAtten.z) * g_vPtAtten.y;
	float strength = g_vPtAtten.x / exp(e * e);
		// 1.0 / ( g_vPtAtten.x + g_vPtAtten.y * d + g_vPtAtten.z * d * d) * g_vPtAtten.w;

	for( int i = 0; i < 5; i++)
	{
			Color += SamplerCastShadow(TexCoord + TexCoordOffset[i]) ;
	}
	Color.xyz += strength * g_colPtDiffuse.xyz;
	return Color/5.0f;
	//Color = SamplerCastShadow(TexCoord);
	//return Color;
	
}