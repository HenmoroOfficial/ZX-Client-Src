#include "..\fog\fog_common.h"

string entrynames = "vs_2_0 vs_main";

// DESCRIPTION:
// Vertex shader used to render A3DSkinMesh. this shader does below works:
// # vertex blending (4 blending matrix) and transform
// # compute light = env_ambient + dir_light(ambient & diffuse & specular)
// # output texture coordinates

// CREATED BY: duyuxin, 2003/10/28
// Copyright (c) 2003 Archosaur Studio, All Rights Reserved.

//vs.1.1

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
	float3 weight       : BLENDWEIGHT0;
	float4 blendindices : BLENDINDICES0;
	float3 normal       : NORMAL0;
	float4 tangent      : TANGENT0;
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
float4x3 c14[60] : register(c14);
float4 c13 : register(c13);
float4 c12 : register(c12);
float4 c11 : register(c11);
float4 c10 : register(c10);
float4 specular : register(c9);
float4 diffuse : register(c8);
float4 c7 : register(c7);
float4 EyeDir : register(c6);
float4x4 matProjection : register(c2);
//float4 LightDir : register(c1);
float3 g_vLightDir : LightDirInView;
float4 c0 : register(c0);
float3 g_vecEyePos : EyePos;
float4x4 matViewInv : ViewInverse;
//------------------------------------------------------------------------------
// oPos	  = Output position
// oD0	  = Diffuse
// oD1	  = Specular
// oT0	  = texture coordinates
//------------------------------------------------------------------------------

struct VS_OUTPUT
{
	float4 pos				: POSITION0;
	float2 uv				: TEXCOORD0;
	float3 ViewDirection	: TEXCOORD1;
    float3 LightDirection	: TEXCOORD2;
	float4 PtLightDirection : TEXCOORD3;
	float4 tangent : TEXCOORD4;
	float3 binormal : TEXCOORD5;
	float4 fogParam : TEXCOORD6;
};
// ============ Transform position and normal from model space to world space ==========
VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT output;

	//// Compensate for lack of UBYTE4 on Geforce3
	//mul r1, v2.zyxw, c0.wwww
	int4 blendindices = input.blendindices.zyxw * 255.01;
	float4 pos = float4(input.pos, 1);
	float4 blendweight = float4(input.weight.xyz, 0);
//*

	float4 r0;
	//// first compute the last blending weight
	//dp3 r0.w, v1.xyz, c0.xxx
	// -- r0.w = dot(input.weight.xyz, c0.xxx);
	//add r0.w, -r0.w, c0.x
	// -- r0.w = -r0.w + c0.x;
	
	
	blendweight.w = 1.0 - blendweight.x - blendweight.y - blendweight.z;

	//float4 r4;
	//float4 r5;
	//// Set 1
	//mov a0.x, r1.x
	//m4x3 r4, v0, c[a0.x + 14]
	//r4.xyz = 
	//m3x3 r5, v3, c[a0.x + 14]
	//r5.xyz = mul(input.normal, (float3x3)c14[blendindices.x]) * blendweight.x;

	//// Initialize r4.w & r5.w
	//mov r4.w, c0.x
	//r4.w = 1;
	//mov r5.w, c0.x
	//r5.w = 1;

	//// blend them
	//mul r4, r4, v1.xxxx
	//r4 = r4 * blendweight.x;
	//mul r5, r5, v1.xxxx
	//r5 = r5 * blendweight.x;

	//float4 r2;
	//float4 r3;
	//// Set 2
	//mov a0.x, r1.y
	//m4x3 r2, v0, c[a0.x + 14]
	// -- r2.xyz = ;
	//m3x3 r3, v3, c[a0.x + 14]
	//r3.xyz = mul(input.normal, c14[blendindices.y]) * blendweight.y;

	//// Initialize r2.w & r3.w
	//mov r2.w, c0.x		
	// -- r2.w = 1;
	//mov r3.w, c0.x
	//r3.w = 1;

	//// add them in
	//mad r4, r2, v1.yyyy, r4
	// -- r4 = (r2) + r4;
	//mad r5, r3, v1.yyyy, r5
	//r5 = (r3) + r5;
	//		
	//// Set 3
	//mov a0.x, r1.z
	//m4x3 r2, v0, c[a0.x + 14]
	// -- r2.xyz = ;
	//m3x3 r3, v3, c[a0.x + 14]
	//r3.xyz = mul(input.normal, c14[blendindices.z]) * blendweight.z;

	//// add them in
	//mad r4, r2, v1.zzzz, r4
	// -- r4 = r2 + r4;
	//mad r5, r3, v1.zzzz, r5
	//r5 = r3 + r5;

	//// Set 4
	//mov a0.x, r1.w
	//m4x3 r2, v0, c[a0.x + 14]
	// -- r2.xyz = ;
	//m3x3 r3, v3, c[a0.x + 14]
	//r3.xyz = mul(input.normal, c14[blendindices.w]) * blendweight.w;

	//// add them in
	//mad r4, r2, r0.wwww, r4
	// -- r4 = r2 + r4;
	//mad r5, r3, r0.wwww, r5
	//r5 = r3 + r5;

	//// Transfrom position from camera space to cuboid space and output it
	//mov r4.w, c0.x
	//r4.w = 1;
	//m4x4 r1, r4, c2
	//mov oPos, r1
	float4 blendPos;
	float4 blendNormal;
	float4 blendTangent;
	float4 blendBinormal;
	blendPos.xyz = 
		mul(pos, c14[blendindices.x]) * blendweight.x +
		mul(pos, c14[blendindices.y]) * blendweight.y +
		mul(pos, c14[blendindices.z]) * blendweight.z +
		mul(pos, c14[blendindices.w]) * blendweight.w ;
	blendPos.w = 1;

	blendNormal.xyz = 
		mul(input.normal, (float3x3)c14[blendindices.x]) * blendweight.x +
		mul(input.normal, (float3x3)c14[blendindices.y]) * blendweight.y +
		mul(input.normal, (float3x3)c14[blendindices.z]) * blendweight.z +
		mul(input.normal, (float3x3)c14[blendindices.w]) * blendweight.w ;
	blendNormal.xyz = normalize(blendNormal.xyz);
	blendNormal.w = 1;

	blendTangent.xyz = 
		mul(input.tangent.xyz, (float3x3)c14[blendindices.x]) * blendweight.x +
		mul(input.tangent.xyz, (float3x3)c14[blendindices.y]) * blendweight.y +
		mul(input.tangent.xyz, (float3x3)c14[blendindices.z]) * blendweight.z +
		mul(input.tangent.xyz, (float3x3)c14[blendindices.w]) * blendweight.w ;
	blendTangent.xyz = normalize(blendTangent.xyz);
	blendTangent.w = 1;

	blendBinormal.xyz = cross(blendNormal.xyz, blendTangent.xyz) * input.tangent.w;

	output.pos = mul(blendPos, matProjection);
	//mov oFog, c0.x
	// TODO: ?????

	//// ============ Calculate directional light's diffuse and specular ============

	//// normalize normal
	//dp3 r5.w, r5, r5
	//r5.w = dot(r5.xyz, r5.xyz);
	//rsq r5.w, r5.w
	//r5.w = 1.0 / sqrt(r5.w);
	//mul r5, r5, r5.w
	//r5 = r5 * r5.w;
	float3 LightDirection = -g_vLightDir.xyz;
	//float3 EyeDirection = -EyeDir;
	float3 EyeDirection = normalize(-blendPos.xyz);


	//blendNormal.xyz = normalize(blendNormal.xyz);
	//blendTangent.xyz = normalize(blendTangent.xyz);
	//blendBinormal.xyz = normalize(blendBinormal.xyz);

	output.ViewDirection.x  = dot( blendTangent.xyz, EyeDirection );
	output.ViewDirection.y  = dot( blendBinormal.xyz, EyeDirection );
	output.ViewDirection.z  = dot( blendNormal.xyz, EyeDirection );
   
	output.LightDirection.x  = dot( blendTangent.xyz, LightDirection );
	output.LightDirection.y  = dot( blendBinormal.xyz, LightDirection );
	output.LightDirection.z  = dot( blendNormal.xyz, LightDirection );
	
	output.tangent.xyz = blendTangent.xyz;
	output.tangent.w = input.tangent.w;
	output.binormal.xyz = blendBinormal.xyz;
	
	////// output texture layer 1 to use the camera space normal 
	////mad oT1.x, r5.x, c1.w, c1.w
	//// -- output.normalCS.x = 0.5 + (blendNormal.x * 0.5);
	////mad oT1.y, r5.y, -c1.w, c1.w
	//// -- output.normalCS.y = 0.5 - (blendNormal.y * 0.5);

	//float4 r1;
	//float4 r2;
	////float4 r5;
	////mov r4, -c1
	////r4 = -LightDir;
	////dp3 r1.x, r5, r4     	// normal dot light (N * L)
	//r1.x = dot(blendNormal.xyz, -LightDir.xyz);
	////add r2, -c6, r4		// normalized half vector H = L + V
	//r2 = -EyeDir - LightDir;

	////// renormalize H
	////dp3 r2.w, r2, r2
	////r2.w = dot(r2.xyz, r2.xyz);
	////rsq r2.w, r2.w
	////r2.w = 1.0 / sqrt(r2.w);
	////mul r2, r2, r2.w
	////r2 = r2 * r2.w;
	//r2.xyz = normalize(r2.xyz);
	//r2.w = 1.0;

	////dp3 r1.y, r5, r2	// N dot H
	//r1.y = dot(blendNormal.xyz, r2.xyz);

	////// compute specular and clamp values (lit)
	////// r1.x - N dot L
	////// r1.y - N dot H
	////// r1.w - specular power n
	////mov r1.w, c0.y
	////r1.w = c0.y;
	////lit r1, r1
	//r1 = lit(r1.x, r1.y, c0.y);

	////mul r0.rgb, r1.y, c8.rgb  // Multiply with diffuse, don't effect alpha value
	//r0.rgb = r1.y * diffuse.rgb;
	////mov r0.a, c8.a
	//r0.a = diffuse.a;
	////add r0, r0, c7        	// Add in ambient
	//r0 = r0 + c7;
	////min oD0, r0, c0.x     	// output diffuse, clamp if > 1
	//output.diffuse = min(r0, c0.xxxx);

	////float4 r4;
	////mul r4, r1.z, c9    	// Multiply with specular
	////r4 = r1.z * specular;
	////mov r4.w, c0.x			// Set fog factor to 1
	////r4.w = c0.x;
	////min oD1, r4, c0.x     	// output specular, clamp if > 1
	//output.specular.xyz = min(r1.z * specular.xyz, c0.xxx);
	//output.specular.w = 1;

	//// Copy texture coordinate
	//mov oT0, v7
	output.PtLightDirection = float4(0,1,0, 0);
	output.uv = input.uv;
	//output.normal = input.normal;
	//output.tangent = input.tangent.xyz;
	//output.binormal = cross(output.tangent, output.normal) * input.tangent.w;
/*/
	
	float4 p;
	p.xyz = mul(pos, c14[0]);
	p.w = 1;
	output.pos = mul(p, matProjection);

	output.diffuse = float4(blendweight.xyz * 4, 1);
	output.specular = 0;
	output.uv = 0;//input.uv;
	output.unknown = 1;
//*/
    float worldPosY = dot(matViewInv[1].xyz, blendPos.xyz) + g_vecEyePos.y;
    output.fogParam = ComputeFog(output.pos.w, g_vecEyePos.y, worldPosY);

	return output;
}