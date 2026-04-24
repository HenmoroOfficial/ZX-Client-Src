// raochao, 2013/10/12

struct VS_OUTPUT
{
	float4 pos : POSITION0;
};

string entrynames = "vs_2_0 vs_main";

// 默认是 SKINMESH
string macro0
<
	string name = "RIGID_ENABLE";
	string desc = "Rigid支持";
	string type = "PRG";
	string GroupID = "1";
>;

struct VS_INPUT
{
	float3 pos          : POSITION0;
#ifdef RIGID_ENABLE
#else
	float3 weight       : BLENDWEIGHT0;
	float4 blendindices : BLENDINDICES0;
#endif // RIGID_ENABLE
	float3 normal       : NORMAL0;
};


float4x3 matBlend0[70]   : register(c10);	// These blend matrices are WorldMatrix * ViewMatrix
float4x4 matProjection : register(c2);
float   g_fExtend      : PosExtendToNormal;


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

#ifdef RIGID_ENABLE
	blendPos.xyz = mul(pos, (float4x3)matBlend0[0]);
	blendPos.w = 1;

	blendNormal.xyz = mul(input.normal, (float3x3)matBlend0[0]);
	blendNormal.xyz = normalize(blendNormal.xyz);
	blendNormal.w = 1;
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
#endif // #ifdef RIGID_ENABLE
	blendPos.xyz += blendNormal.xyz * g_fExtend;

	output.pos = mul(blendPos, matProjection);
	output.pos.z += g_fExtend / output.pos.w;

	return output;
}