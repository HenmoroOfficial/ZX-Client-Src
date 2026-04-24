/*--------------------------------------------------------------------------------------
 Copyright (c) 2012 Perfect World Entertainment Inc, All Rights Reserved.
 Created by LiChen 2012/8/10

 Description:
 This shader is used to draw custom mirror plane meshes. It has two different versions:
 One is shading with per vertex color, another is shading with pre-baked light map.
--------------------------------------------------------------------------------------*/

string entrynames = "vs_2_0 vs_main; ps_2_0 ps_main";

string macro0
<
	string name = "_USE_LIGHT_MAP_";
	string desc = "是否使用光照贴图";
	string type = "PRG";
	string GroupID = "1";
>;

string macro1
<
	string name = "_USE_ALPHA_MASK_TEXTURE_";
	string desc = "是否使用 Alpha 蒙板纹理";
	string type = "PRG";
	string GroupID = "2";
>;


float4x4 g_matWVP			: WorldViewProjection;
float4x4 g_matITWVP			: ITWorldViewProjection;
float4 g_LightMapParam		: GlobalLightMapParam;

float g_fAlphaMask;										// Alpha test comparsion value for alpha mask texture
float g_UVOffsetMultipler;								// UV offset multipler applied to noise UV
float g_fFakeReflectPower;								// Power of fake reflection.
float3 g_ViewDirection		: ViewDirection;			// Camera view direction in world space
float3 g_ReflectionNormal;								// Normal of reflection plane


sampler2D g_ReflectionSampler		: register(s0);		// Real-time reflection texture
sampler2D g_AlphaMaskSampler		: register(s1);		// Pre-baked lightmap texture in the night
sampler2D g_LightMapDaySampler		: register(s2);		// Pre-baked lightmap texture in the day
sampler2D g_LightMapNightSampler	: register(s3);		// Pre-baked lightmap texture in the night
sampler2D g_UVNoiseSampler			: register(s4);		// Noise map to offset reflection sampling UV
sampler2D g_FakeReflectSampler		: register(s5);		// A fake reflection texture provided by artist.


struct VS_INPUT
{
	float3 Pos      			: POSITION0;
	float3 Normal				: NORMAL0;
	float4 DiffuseDay			: COLOR0;		// Pre-baked diffuse lighting color in the day
	float4 DiffuseNight			: COLOR1;		// Pre-baked diffuse lighting color in the night
	float2 UV					: TEXCOORD0;	// Texture coordination to sample custom texture
	float2 LightMapUV			: TEXCOORD1;	// Texture coordination to sample light map texture
};

struct VS_OUTPUT
{
	float4 Pos      			: POSITION0;

#ifndef _USE_LIGHT_MAP_

	float4 DiffuseDay			: COLOR0;		// Pre-baked diffuse lighting color in the day
	float4 DiffuseNight			: COLOR1;		// Pre-baked diffuse lighting color in the night

#endif /* #ifdef _USE_LIGHT_MAP_ */

	float3 Normal				: TEXCOORD0;	// Because ps2.0 doesn't support NORMAL0 in PS, so we use TEXCOORD instead.
	float3 ReflectionUV			: TEXCOORD1;	// Texture coordination to sample real-time reflection texture
	float2 UV					: TEXCOORD2;	// Texture coordination to sample custom texture

#ifdef _USE_LIGHT_MAP_

	float2 LightMapUV			: TEXCOORD3;	// Texture coordination to sample light map texture

#endif /* #ifdef _USE_LIGHT_MAP_ */
};


VS_OUTPUT vs_main( VS_INPUT input )
{
	VS_OUTPUT output;

	output.Pos = mul( float4( input.Pos, 1.0f ), g_matWVP );
	output.Normal = mul( input.Normal, (float3x3)g_matITWVP );
	output.UV = input.UV;

#ifdef _USE_LIGHT_MAP_

	output.LightMapUV = input.LightMapUV;

#else /* #ifdef _USE_LIGHT_MAP_ */

	output.DiffuseDay = input.DiffuseDay;
	output.DiffuseNight = input.DiffuseNight;

#endif /* #ifdef _USE_LIGHT_MAP_ */

	output.ReflectionUV.xy = output.Pos.xy * float2( 0.5f, -0.5f ) + output.Pos.ww * float2( 0.5f, 0.5f );
	output.ReflectionUV.z = output.Pos.w;

	return output;
}


float4 ps_main( VS_OUTPUT input ) : COLOR0
{
#ifdef _USE_ALPHA_MASK_TEXTURE_

	float fAlphaMask = tex2D( g_AlphaMaskSampler, input.UV ).a;
	clip( fAlphaMask - g_fAlphaMask );

#endif /* #ifdef _USE_ALPHA_MASK_TEXTURE_ */

	float2 ReflectionUV = input.ReflectionUV.xy / input.ReflectionUV.z;

	//First We Need Add some Noise On UV

	//1. Get a Normal Map As Noise
	//Use A and G Channel
	float2 UVNoise = tex2D( g_UVNoiseSampler, input.UV ).ag * 2.0f - 1.0f;

	//2. ViewDir As View Offset 
	//   g_UVOffsetMultipler: FOR ART
	float2 UVOffset = g_ViewDirection.xy * 0.01f;

	//Sample Reflection Texture Use New UV
	ReflectionUV += UVNoise * g_UVOffsetMultipler - UVOffset;
	float4 Reflection = tex2D( g_ReflectionSampler, ReflectionUV );

	//let's add some effects for Reflection
	//1. Desaturate a little bit
	float3 Gray = dot( Reflection.rgb, float3(0.3f, 0.59f, 0.11f) );
	Reflection.rgb = lerp( Reflection.rgb, Gray, 0.9f );

	//2. Contrast
	Reflection.rgb = Reflection.rgb * 0.9f + 0.05f;

#ifdef _USE_LIGHT_MAP_

	float4 DiffuseDay = tex2D( g_LightMapDaySampler, input.LightMapUV ) * g_LightMapParam.x;
	float4 DiffuseNight = tex2D( g_LightMapNightSampler, input.LightMapUV ) * g_LightMapParam.y;

#else /* #ifdef _USE_LIGHT_MAP_ */

	float4 DiffuseDay = input.DiffuseDay * g_LightMapParam.x;
	float4 DiffuseNight = input.DiffuseNight * g_LightMapParam.y;

#endif /* #ifdef _USE_LIGHT_MAP_ */

	float4 Diffuse = lerp( DiffuseDay, DiffuseNight, g_LightMapParam.w );

	//Final, Need add a fake glass reflect
	float3 CameraReflection = reflect( g_ViewDirection, g_ReflectionNormal );
	float4 FakeReflectTex = tex2D( g_FakeReflectSampler, CameraReflection.xy );
	float4 Output = Reflection + Diffuse + FakeReflectTex * g_fFakeReflectPower;
	return Output;
}
