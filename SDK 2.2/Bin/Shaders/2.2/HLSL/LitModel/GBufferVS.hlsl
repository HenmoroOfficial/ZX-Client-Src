string entrynames = "vs_2_0 vs_main";	

float4x4 matWorldView;
float4x4 matWorldViewProj;

void vs_main(in float4 	position  : POSITION,
						 in float3  normal		: NORMAL0,
						 in float2  texCoord  : TEXCOORD0,
						out float4 	oPosition : POSITION,
						out float2 	oDepthZW		: TEXCOORD0,
						out float3 	oNormal		: TEXCOORD1,
						out float2  oTexCoord	: TEXCOORD2)
{
	oPosition = mul(position, matWorldViewProj);
	oDepthZW = float2(oPosition.z, oPosition.w);
	oNormal = normal;
	oTexCoord = texCoord;
}
