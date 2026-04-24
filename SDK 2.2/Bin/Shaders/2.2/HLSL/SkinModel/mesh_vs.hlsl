#include "..\fog\fog_common.h"
#include "Mesh_comm.h"
// DESCRIPTION:
// Vertex shader used to render A3DSkinMesh. this shader does below works:
// # vertex blending (4 blending matrix) and transform
// # compute light = env_ambient + dir_light(ambient & diffuse & specular)
// # output texture coordinates

// CREATED BY: duyuxin, 2003/10/28
// Copyright (c) 2003 Archosaur Studio, All Rights Reserved.

string entrynames = "vs_2_0 vs_main";

string macro0
<
	string name = "POINT_LIGHT_ENABLE";
	string desc = "点光源支持";
	string type = "PRG";
	string GroupID = "1";
>;

//string macro1
//<
//	string name = "SKINMESH_ENABLE";
//	string desc = "SkinMesh";
//	string type = "PRG";
//	string GroupID = "1";
//>; 

// 默认是 SKINMESH
string macro1
<
	string name = "RIGID_ENABLE";
	string desc = "Rigid支持";
	string type = "PRG";
	string GroupID = "2";
>;

string macro2
<
	string name = "TANGENT_AS_SRC";
	string desc = "以切线数据为顶点输入";
	string type = "PRG";
	string GroupID = "3";
>;


//------------------------------------------------------------------------------
// v0 = position
// v1 = blend weights
// v2 = blend indices
// v3 = normal
// v7 = texture coordinates
//------------------------------------------------------------------------------

//dcl_position v0
//dcl_blendweight v1
//dcl_blendindices v2
//dcl_normal v3
//dcl_texcoord v7

struct VS_INPUT
{
	float3 pos          : POSITION0;
#ifdef RIGID_ENABLE
#else
	float3 weight       : BLENDWEIGHT0;
	float4 blendindices : BLENDINDICES0;
#endif // RIGID_ENABLE
	float3 normal       : NORMAL0;
#ifdef TANGENT_AS_SRC
	float4 tangent      : TANGENT0;
#else
	float4 binormal     : TANGENT0;
#endif // TANGENT_AS_SRC
	float2 uv           : TEXCOORD0;
};

//------------------------------------------------------------------------------
// Constants specified by the app//
//
// >= c14  = palette of world matrices (from model space to world space)
// c13	  = point_light's attenuation
// c12	  = material ambient * point_light.ambient
// c11	  = material.diffuse * point_light.diffuse
// c10	  = point_light's position in world space
// c9	  = material.specular * dir_light.specular
// c8	  = material.diffuse * dir_light.diffuse
// c7	  = ambient color + emmisive color
// c6	  = eye's direction in camera space
// c2-c5   = projection matrix (from camera space to cuboid space)
// c1	  = dir_light's direction in camera space, c1.w is 0.5f
// c0	  = {1, power, 0.0, 765.01}//
//------------------------------------------------------------------------------
float4x3 matBlend0[70]   : register(c10);
//float4 g_vPtAtten : register(c13);
//float4 g_colPtAmbient : register(c12);
//float4 g_colPtDiffuse : register(c11);
//float4 g_vPtLightPos : register(c10);

float4 g_vPtAtten      : PointAttenRange;
float3 g_vPtLightPos   : PointPosInView;

//float4 specular : register(c9);
//float4 diffuse : register(c8);
//float4 c7              : register(c7);
//float4 EyeDir          : register(c6);
float4x4 matProjection : register(c2);
//float4 LightDir : register(c1);
float3 g_vLightDir     : LightDirInView;
//float4 c0              : register(c0);
float3 g_vecEyePos     : EyePos;
float4x4 matViewInv    : ViewInverse;
float   g_fExtend      : PosExtendToNormal;
//------------------------------------------------------------------------------
// oPos	  = Output position
// oD0	  = Diffuse
// oD1	  = Specular
// oT0	  = texture coordinates
//------------------------------------------------------------------------------

// ============ Transform position and normal from model space to world space ==========
VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT output;
	float4 r0;

#ifdef RIGID_ENABLE
	float4 pos = float4(input.pos, 1);
#else
	int4 blendindices = input.blendindices.zyxw * 255.01;
	float4 pos = float4(input.pos, 1);
	float4 blendweight = float4(input.weight.xyz, 0);

	blendweight.w = 1.0 - blendweight.x - blendweight.y - blendweight.z;
#endif // RIGID_ENABLE
	
	float4 blendPos;
	float4 blendNormal;
	float4 blendTangent;
	float4 blendBinormal;

#ifdef RIGID_ENABLE
	blendPos.xyz = mul(pos, (float4x3)matBlend0[0]);
	blendPos.w = 1;

	blendNormal.xyz = mul(input.normal, (float3x3)matBlend0[0]);
	blendNormal.xyz = normalize(blendNormal.xyz);
	blendNormal.w = 1;

#	ifdef TANGENT_AS_SRC
	blendTangent.xyz = mul(input.tangent.xyz, (float3x3)matBlend0[0]); // float3(1,0,0);
	blendTangent.w = 1;

	blendBinormal.xyz = cross(blendNormal.xyz, blendTangent.xyz) * input.tangent.w;;
#	else
	blendBinormal.xyz = mul(input.binormal.xyz, (float3x3)matBlend0[0]); // float3(1,0,0);
	blendBinormal.w = 1;

	blendTangent.xyz = cross(blendBinormal.xyz, blendNormal.xyz) * input.binormal.w;;
#	endif // TANGENT_AS_SRC

#else

	blendPos.xyz = 
		mul(pos, (float4x3)matBlend0[blendindices.x]) * blendweight.x +
		mul(pos, (float4x3)matBlend0[blendindices.y]) * blendweight.y +
		mul(pos, (float4x3)matBlend0[blendindices.z]) * blendweight.z +
		mul(pos, (float4x3)matBlend0[blendindices.w]) * blendweight.w ;
	blendPos.w = 1;

	blendNormal.xyz = 
		mul(input.normal, (float3x3)matBlend0[blendindices.x]) * blendweight.x +
		mul(input.normal, (float3x3)matBlend0[blendindices.y]) * blendweight.y +
		mul(input.normal, (float3x3)matBlend0[blendindices.z]) * blendweight.z +
		mul(input.normal, (float3x3)matBlend0[blendindices.w]) * blendweight.w ;
	blendNormal.xyz = normalize(blendNormal.xyz);
	blendNormal.w = 1;

#	ifdef TANGENT_AS_SRC
	blendTangent.xyz = 
		mul(input.tangent.xyz, (float3x3)matBlend0[blendindices.x]) * blendweight.x +
		mul(input.tangent.xyz, (float3x3)matBlend0[blendindices.y]) * blendweight.y +
		mul(input.tangent.xyz, (float3x3)matBlend0[blendindices.z]) * blendweight.z +
		mul(input.tangent.xyz, (float3x3)matBlend0[blendindices.w]) * blendweight.w ;

	blendTangent.xyz = normalize(blendTangent.xyz);
	blendTangent.w = 1;

	blendBinormal.xyz = cross(blendNormal.xyz, blendTangent.xyz) * input.tangent.w;
#	else
	blendBinormal.xyz = 
		mul(input.binormal.xyz, (float3x3)matBlend0[blendindices.x]) * blendweight.x +
		mul(input.binormal.xyz, (float3x3)matBlend0[blendindices.y]) * blendweight.y +
		mul(input.binormal.xyz, (float3x3)matBlend0[blendindices.z]) * blendweight.z +
		mul(input.binormal.xyz, (float3x3)matBlend0[blendindices.w]) * blendweight.w ;

	blendBinormal.xyz = normalize(blendBinormal.xyz);

	blendTangent.xyz = cross(blendBinormal.xyz, blendNormal.xyz) * input.binormal.w;
#	endif // TANGENT_AS_SRC

#endif // #ifdef RIGID_ENABLE
	blendPos.xyz += blendNormal.xyz * g_fExtend;
	
	output.pos = mul(blendPos, matProjection);
	output.pos.z += g_fExtend / output.pos.w;

	float3 LightDirection = -g_vLightDir.xyz;
	float3 EyeDirection = normalize(-blendPos.xyz);

#ifdef POINT_LIGHT_ENABLE
	float3 vDeltaPtLight = g_vPtLightPos - blendPos.xyz;
#endif // POINT_LIGHT_ENABLE

	output.ViewDir.x  = dot( blendTangent.xyz, EyeDirection );
	output.ViewDir.y  = dot( blendBinormal.xyz, EyeDirection );
	output.ViewDir.z  = dot( blendNormal.xyz, EyeDirection );
   
	output.LightDir.x  = dot( blendTangent.xyz, LightDirection );
	output.LightDir.y  = dot( blendBinormal.xyz, LightDirection );
	output.LightDir.z  = dot( blendNormal.xyz, LightDirection );

	output.tangent.xyz  = blendTangent.xyz;
#ifdef TANGENT_AS_SRC
	output.tangent.w    = input.tangent.w;
#else
	output.tangent.w    = input.binormal.w;
#endif // TANGENT_AS_SRC
	output.binormal.xyz = blendBinormal.xyz;
	
#ifdef POINT_LIGHT_ENABLE
	output.PtLightDir.x  = dot( blendTangent.xyz, vDeltaPtLight );
	output.PtLightDir.y  = dot( blendBinormal.xyz, vDeltaPtLight );
	output.PtLightDir.z  = dot( blendNormal.xyz, vDeltaPtLight );

	float d = length(vDeltaPtLight);
	float e = (d / g_vPtAtten.z) * g_vPtAtten.y;
	output.PtLightDir.w = g_vPtAtten.x / exp(e * e);
#else
	output.PtLightDir = float4(0,1,0,0);
#endif // POINT_LIGHT_ENABLE

	// uv
	output.uvBase = input.uv;

	// Use view space normal as the addtional uv to sample highlight texture while rendering with GFX.
	output.highlightUV = blendNormal.xz * float2( 0.5f, -0.5f ) + 0.5f;
    
    float4 worldPos = mul(blendPos, matViewInv);
	// 雾
	output.fogParam = ComputeCompositeFogSimple(worldPos.xyz - g_vecEyePos, worldPos.y);

	return output;
}