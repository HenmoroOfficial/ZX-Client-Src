#ifndef _LITMODEL_COMMON_H_
#define _LITMODEL_COMMON_H_

float4 LitModelLightMapParam 	: GlobalLightMapParam;
    
struct VS_OUTPUT
{
	float4 pos      						: POSITION0;
	float3 normal							: TEXCOORD4;
	float4 diffuse  						: TEXCOORD5;
	float2 uv       						: TEXCOORD0;
	float4 fogParam                         : TEXCOORD1;

#ifdef _LIGHT_MAP_	    
	float2 uvlm								: TEXCOORD2;
#endif

#ifdef _SHADOW_MAP_	    
	float4 shadowProj						: TEXCOORD3;
#endif	    
};

string macro0
<
	string name = "_LIGHT_MAP_";
	string desc = "Has Light Map";
	string type = "PRG";
	string GroupID = "1";
>;

string macro1
<
	string name = "_VERT_LIT_";
	string desc = "Has Vertex Color";
	string type = "PRG";
	string GroupID = "1";
>;

string macro2
<
	string name = "_REAL_TIME_";
	string desc = "Real Time lighting";
	string type = "PRG";
	string GroupID = "1";
>;

string macroShadow
<
	string name = "_SHADOW_MAP_";
	string desc = "Use Shadow Map";
	string type = "PRG";
	string GroupID = "2";
>;

string macroHWPCF
<
	string name = "_HWPCF_";
	string desc = "Use hardware PCF";
	string type = "PRG";
	string GroupID = "3";
>;


string macroShadowHigh
<
	string name = "_SHADOW_HIGH_";
	string desc = "High quality shadow";
	string type = "PRG";
	string GroupID = "4";
>;

string macroDetailMap
<
    string name = "_DETAIL_MAP_";
    string desc = "Use detail map";
    string type = "PRG";
    string GroupID = "5";
>;

#endif // _LITMODEL_COMMON_H_