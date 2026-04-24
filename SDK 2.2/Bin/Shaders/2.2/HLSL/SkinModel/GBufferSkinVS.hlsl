string entrynames = "vs_2_0 vs_main";

// 默认是 SKINMESH
string macro1
<
	string name = "_RIGID_MESH_";
	string desc = "Rigid支持";
	string type = "PRG";
	string GroupID = "1";
>;

string macro2
<
	string name = "_SILHOUETTE_DEPTH_";
	string desc = "输出渲染勾边需要的深度";
	string type = "PRG";
	string GroupID = "2";
>;


#ifdef _RIGID_MESH_
struct VS_INPUT
{
	float3 pos          : POSITION0;
	float3 normal       : NORMAL0;
	float2 uv           : TEXCOORD0;
};
#else
struct VS_INPUT
{
	float3 pos          : POSITION0;
	float3 weight       : BLENDWEIGHT0;
	float4 blendindices : BLENDINDICES0;
	float3 normal       : NORMAL0;
	float2 uv           : TEXCOORD0;
};
#endif


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
float4x3 c14[70] : register(c10);
//float4 c13 : register(c13);
//float4 c12 : register(c12);
//float4 c11 : register(c11);
//float4 c10 : register(c10);
float4 specular : register(c9);
float4 diffuse : register(c8);
float4 c7 : register(c7);
float4 EyeDir : register(c6);
float4x4 matProjection : register(c2);
float4 LightDir : register(c1);
float4 c0 : register(c0);

struct VS_OUTPUT
{
	float4 pos       : POSITION0;
	float2 uv        : TEXCOORD0;
	float2 normalCS  : TEXCOORD1;	// Camera Space
	float2 	depthZW	   : TEXCOORD2;	//非线性深度
	float3 	normal	 : TEXCOORD3;
};

VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT output;
	
	float4 pos = float4(input.pos, 1);
	float4 r0;
	float4 compos;
	float4 comnormal;
	
#ifdef _RIGID_MESH_
	compos.xyz = mul(pos, c14[0]);
	compos.w = 1;
	comnormal.xyz = mul(input.normal, (float3x3)c14[0]);
	comnormal.xyz = normalize(comnormal.xyz);
	comnormal.w = 1;
	
#else
	int4 blendindices = input.blendindices.zyxw * 255.01;
	float4 blendweight = float4(input.weight.xyz, 0);
	
	blendweight.w = 1.0 - blendweight.x - blendweight.y - blendweight.z;


	compos.xyz = 
		mul(pos, c14[blendindices.x]) * blendweight.x +
		mul(pos, c14[blendindices.y]) * blendweight.y +
		mul(pos, c14[blendindices.z]) * blendweight.z +
		mul(pos, c14[blendindices.w]) * blendweight.w ;
	compos.w = 1;

	comnormal.xyz = 
		mul(input.normal, (float3x3)c14[blendindices.x]) * blendweight.x +
		mul(input.normal, (float3x3)c14[blendindices.y]) * blendweight.y +
		mul(input.normal, (float3x3)c14[blendindices.z]) * blendweight.z +
		mul(input.normal, (float3x3)c14[blendindices.w]) * blendweight.w ;
	comnormal.xyz = normalize(comnormal.xyz);
	comnormal.w = 1;
#endif
	
	output.normal = float3(0,0,0);
	output.pos = mul(compos, matProjection);

	output.depthZW = float2(output.pos.z, output.pos.w);
	//// output texture layer 1 to use the camera space normal 
	//mad oT1.x, r5.x, c1.w, c1.w
	output.normalCS.x = 0.5 + (comnormal.x * 0.5);
	//mad oT1.y, r5.y, -c1.w, c1.w
	output.normalCS.y = 0.5 - (comnormal.y * 0.5);

	output.uv = input.uv;

	return output;
}