//--------------------------------------------------------------------------------------
// Copyright (c) Archosaur Studio. All rights reserved.
// Created by liyi 2012, 3, 19
// Version : 1.0
//--------------------------------------------------------------------------------------
string entrynames = "vs_2_0 vs_FrameRefract;ps_2_0 ps_FrameRefract;ps_2_0 ps_FrameRefract_UnderWater;ps_2_0 ps_FrameRefract_UnderWater_RealTime";


//-------------------------------------------------------------------------
//	Vertex variables
//-------------------------------------------------------------------------

float2 g_vFrameSize;	//	Frame buffer size (.xy: width & height)

//-------------------------------------------------------------------------
//	Vertex input output
//-------------------------------------------------------------------------

struct VS_INPUT_BASIC
{
	float4 vPos : POSITION;
	float2 uv : TEXCOORD0;
	float4 vEyeVec : TEXCOORD1;
};

struct VS_OUTPUT_BASIC
{
	float4 Position : POSITION;
	float2 uv : TEXCOORD0;
	float4 vEyeVec : TEXCOORD1;
};

//-------------------------------------------------------------------------
//	Vertex shader
//-------------------------------------------------------------------------

VS_OUTPUT_BASIC vs_FrameRefract(VS_INPUT_BASIC input)
{
	VS_OUTPUT_BASIC output;

	output.Position = input.vPos;
	output.uv = input.uv +  0.5f/g_vFrameSize;
	output.vEyeVec = input.vEyeVec;
	
	return output;
}

//-------------------------------------------------------------------------
//	Pixel variables
//-------------------------------------------------------------------------
float3 g_vCameraPos ;

float g_fWaterHeight;
float g_fEdgeHeiScale;

//-------------------------------------------------------------------------
//	Texture and Samplers
//-------------------------------------------------------------------------
//A3D_SAMPLER2D_DEF(g_FrameSampler, g_FrameTex, POINT, POINT, NONE, CLAMP, CLAMP)
sampler2D g_FrameSampler : register(s0);

//A3D_SAMPLER2D_DEF(g_GBufSampler, g_GBufTex, POINT, POINT, POINT, CLAMP, CLAMP)
sampler2D g_DepthSampler : register(s1);

float g_fProjRatioX;					//	zf / (zf - zn)
float g_fProjRatioY;			//	zn / (zn - zf)

float GBufUnpackDepth(float fProjZ)
{
	return g_fProjRatioY / (fProjZ - g_fProjRatioX);
}
//-------------------------------------------------------------------------
//	Local functions
//-------------------------------------------------------------------------

//	Get pixel position in world space
float3 CalcPixelPosInWorld(float2 uvGBuf, float3 vEyeVec)
{
	float4 colGBuf = tex2D(g_DepthSampler, uvGBuf);
	float fDepth = GBufUnpackDepth(colGBuf.r); 
	return g_vCameraPos + vEyeVec * fDepth;
}

//-------------------------------------------------------------------------
//	Generate refract effect, eye is above water
//-------------------------------------------------------------------------

float4 ps_FrameRefract(VS_OUTPUT_BASIC input) : COLOR0
{
	//	Re-construct pixel position in world space
	float3 vPosInWorld = CalcPixelPosInWorld(input.uv, input.vEyeVec.xyz);

	//	Get height delta
	float fHeiDelta = g_fWaterHeight - vPosInWorld.y;
	clip(fHeiDelta + 0.5f);
	fHeiDelta = max(fHeiDelta, 0.0f);

	float4 vOut = tex2D(g_FrameSampler, input.uv);
	vOut.a = fHeiDelta / (1.0f + fHeiDelta);
	
	return vOut;
}

//-------------------------------------------------------------------------
//	Generate refract effect, eye is under water
//-------------------------------------------------------------------------

float4 ps_FrameRefract_UnderWater(VS_OUTPUT_BASIC input) : COLOR0
{
	//	Re-construct pixel position in world space
	float3 vPosInWorld = CalcPixelPosInWorld(input.uv, input.vEyeVec.xyz);

	//	Get height delta
	float fHeiDelta = vPosInWorld.y - g_fWaterHeight;	//	Different with ps_FrameRefract() !
	clip(fHeiDelta + 0.5f);
	fHeiDelta = max(fHeiDelta, 0.0f);

	float4 vOut = tex2D(g_FrameSampler, input.uv);
	vOut.a = saturate(fHeiDelta * g_fEdgeHeiScale);

	return vOut;
}

float4 ps_FrameRefract_UnderWater_RealTime(VS_OUTPUT_BASIC input) : COLOR0
{
	//	Re-construct pixel position in world space
	float3 vPosInWorld = CalcPixelPosInWorld(input.uv, input.vEyeVec.xyz);

	//	Get height delta
	float fHeiDelta = vPosInWorld.y - g_fWaterHeight;	//	Different with ps_FrameRefract() !
	clip(fHeiDelta + 0.5f);
	fHeiDelta = max(fHeiDelta, 0.0f);

	float4 vOut = float4(0.0f, 0.0f, 0.0f, 0.0f);
	vOut.a = saturate(fHeiDelta * g_fEdgeHeiScale);

	return vOut;
}

//-----------------------------------------------------------------------
// Technique
//-----------------------------------------------------------------------

/*
technique tech_EyeAboveWater
<
string usage = "<basic><editor>";
>
{
    pass P0
    {
		VertexShader = AUTO_COMPILE_VS vs_FrameRefract();
		PixelShader  = AUTO_COMPILE_PS ps_FrameRefract(); 
    }
}

technique tech_EyeUnderWater
<
string usage = "<basic><editor>";
>
{
    pass P0
    {
		VertexShader = AUTO_COMPILE_VS vs_FrameRefract();
		PixelShader  = AUTO_COMPILE_PS ps_FrameRefract_UnderWater(); 
    }
}

*/