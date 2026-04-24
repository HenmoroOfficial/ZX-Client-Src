//	grass shader by instance rendering technique
//	on graphic cards that do support ps 3.0
//	
//	add wind force for input vertice and provide view-proj transform on them
//
//	
//	By: ZhangYachuan

#include "grass_common.h"

string entrynames = "ps_3_0 ps_main; vs_3_0 vs_main";

struct VS_INPUT
{
	//	grass mesh data template (in local space)
	float3 Position : POSITION;
	float3 WindParam: TEXCOORD0;		// (x: wind index, y: vertex weight)
	float2 uv		: TEXCOORD1;

	//	Instance data for grass
	float3 vWorldPos: TEXCOORD2;
	float3 vScaleRot: TEXCOORD3;
	float3 vOffset	: TEXCOORD4;
};

struct VS_OUTPUT
{
	float4 oPos		: POSITION;
	float4 uv		: TEXCOORD0;		// uv.xy: self texture uv, uv.zw: lightmap texture uv
	float4 fogParam : TEXCOORD1;
};

float3 _CalcVertexPos(float3 vPos, float2 windParam)
{
//	int iIndex = (int)(((1 + sin(vPos.x + vPos.z)) * 0.5f) * GRASS_OFFSET_NUM);
//	iIndex %= GRASS_OFFSET_NUM;
	float4 vOffset = g_fSoftness * windParam.y * g_WindOffset[windParam.x];
	return vPos + vOffset.xyz;
}

float3 _CalcVertexPosHW(float3 vPos, float3 vOffset, float2 windParam)
{
	//	y > 0 means the grass is pressed, and could not move freely
	//	y == 0 means the grass is free, maybe shaking or free standing (waving)
	if (vOffset.y > 0)
		return vPos + g_fSoftness * windParam.y * float3(vOffset.x, 0, vOffset.z);
	else
 		return _CalcVertexPos(vPos, windParam) + g_fSoftness * windParam.y * vOffset;
}

float3 _CalcRotScaledPos(float3 vPos, float3 vScaleRot)
{
	//	rotation
	float3 vRotatedPos = vPos;
	vRotatedPos.x = vPos.x * vScaleRot.x + vPos.z * vScaleRot.y;
	vRotatedPos.z = vPos.z * vScaleRot.x - vPos.x * vScaleRot.y;

	//	scale
	vRotatedPos *= vScaleRot.z;
	return vRotatedPos;
}

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT output;
		
	float3 vRotScaledPos = _CalcRotScaledPos(Input.Position.xyz, Input.vScaleRot);

	float4 vPosInWorld;
	vPosInWorld.xyz = _CalcVertexPosHW(vRotScaledPos + Input.vWorldPos, Input.vOffset, Input.WindParam.xy);
	vPosInWorld.w = 1.0f;

	float2 uvMask;
	_vs_CalcMaskUV(vPosInWorld.xyz, uvMask);

	output.oPos = mul(vPosInWorld, g_matViewProjection);
	output.uv = float4(Input.uv, uvMask);
    output.fogParam = ComputeCompositeFogSimple(vPosInWorld.xyz - g_vecEyePos, vPosInWorld.y);

	return output;
}

float4 ps_main(VS_OUTPUT Input) : COLOR0
{
	float4 lmDay = tex2D(g_LMDaySampler, Input.uv.zw) * g_LitmapParams.x;
	float4 lmNig = tex2D(g_LMNightSampler, Input.uv.zw) * g_LitmapParams.y;
	float4 baseCol = tex2D(g_BaseSampler, Input.uv.xy);
	float4 lmCol = lerp(lmDay, lmNig, g_LitmapParams.w);
	float4 color = float4((baseCol * lmCol).rgb, baseCol.a);
    color.rgb = lerp(Input.fogParam.rgb, color.rgb, Input.fogParam.a);
    return color;
}