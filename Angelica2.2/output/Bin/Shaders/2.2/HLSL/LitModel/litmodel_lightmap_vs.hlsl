/*vs.1.1
dcl_position v0
dcl_texcoord0 v3
dcl_texcoord1 v4

m4x4 oPos, v0, c0
//m4x4 oT0, v0, c4
mov oT0, v3
mov oT1, v4
*/

#include "..\Shadow\Shadow_common.h"
#include "..\fog\fog_common.h"

string entrynames = "vs_2_0 vs_main";	

float4x4 matViewProj : register(c0);
float4x4 matLightWVP			: register(c4);
float4x4 matWorld : register(c8);
float3 g_vecEyePos;

void vs_main(in float4 	position  							: POSITION,
			 in float2  texCoord0 				: TEXCOORD0,
			 in float2	texCoord1				: TEXCOORD1,
			 out float4 oPosition 				: POSITION,
			out float2 	oCoord0					: TEXCOORD0,
			out float2 	oCoord1					: TEXCOORD1,
			out	float4	oShadowProj				: TEXCOORD2,
			out float4  oFogParam				: TEXCOORD3)
{
    float4 vWorldPos = mul(matWorld, position);
	oPosition = mul(vWorldPos, matViewProj);
	oCoord0 = texCoord0;
	oCoord1 = texCoord1;
	
	oShadowProj = glb_ComputeShadowProj(position, matLightWVP);
	oFogParam = ComputeCompositeFogSimple(vWorldPos.xyz - g_vecEyePos, vWorldPos.y);

}
