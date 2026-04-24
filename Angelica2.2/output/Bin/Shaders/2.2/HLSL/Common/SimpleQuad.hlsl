//--------------------------------------------------------------------------------------
// Copyright (c) Archosaur Studio. All rights reserved.
// Created by duyuxin 2010.4.24
// Version : 1.0
//--------------------------------------------------------------------------------------
string entrynames = "vs_2_0 vs_SimpleQuad; ps_2_0 ps_FillColor";
float4	g_colClear;
float4	g_vRTSize;		//	Render target size. xy: width and height; zw: 1.0 / with and 1.0 / height

//--------------------------------------------------------------------------------------
//	Local things
//--------------------------------------------------------------------------------------

struct VS_INPUT
{
	float4 vPos : POSITION;
	float2 uv : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 vPos : POSITION;
	float2 uv : TEXCOORD0;
};

VS_OUTPUT vs_SimpleQuad(VS_INPUT input)
{
	VS_OUTPUT output;
	output.vPos = input.vPos;
	//	Note: must add the half-pixel offset to uv
	output.uv = input.uv + g_vRTSize.zw * 0.5f;
	return output;
}

//--------------------------------------------------------------------------------------
//	Fill color
//--------------------------------------------------------------------------------------

float4 ps_FillColor(VS_OUTPUT input) : COLOR
{
	return g_colClear;
}

//--------------------------------------------------------------------------------------
//	Techniques
//--------------------------------------------------------------------------------------
/*
technique tech_FillColor
<
string ps = "20";
string vs = "20";
>
{
	pass p0
	{
		VertexShader = compile vs_2_0 vs_SimpleQuad();
		PixelShader = compile ps_2_0 ps_FillColor();
	}
}
*/

