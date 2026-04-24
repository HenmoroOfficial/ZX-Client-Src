//	grass shader by lock and collect rendering technique
//	on graphic cards that do not support ps 3.0
//	
//	add wind force for input vertice and provide view-proj transform on them
//
//	
//	By: ZhangYachuan

#include "grass_common.h"


string entrynames = "ps_2_0 ps_main; vs_2_0 vs_main";

struct VS_INPUT
{
	float3 Position : POSITION;
	float3 WindParam: TEXCOORD0;		// (x: wind index, y: vertex weight)
	float2 uv		: TEXCOORD1;
};

struct VS_OUTPUT
{
	float4 oPos		: POSITION;
	float4 uv		: TEXCOORD0;		// uv.xy: self texture uv, uv.zw: lightmap texture uv
	float4 fogParam : TEXCOORD1;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;
	float3 vOffset = Input.WindParam.y * g_WindOffset[Input.WindParam.x].xyz;
	float3 vCurPos = vOffset + Input.Position;
	Output.oPos = mul(float4(vCurPos, 1), g_matViewProjection);
	float2 uvMask;
	_vs_CalcMaskUV(vCurPos, uvMask);
	Output.uv = float4(Input.uv, uvMask);
	Output.fogParam = ComputeCompositeFogSimple(vCurPos - g_vecEyePos, vCurPos.y);
	return Output;
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