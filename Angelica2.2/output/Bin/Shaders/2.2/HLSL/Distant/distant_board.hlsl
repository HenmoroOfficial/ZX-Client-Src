#include "..\fog\fog_common.h"

string entrynames = "vs_2_0 vs_main;ps_2_0 ps_main;vs_3_0 vs_main;ps_3_0 ps_main";

string macro0
<
	string name = "bPixelFog";
	string desc = "是否逐像素计算雾";
	string type = "PRG;SDR_3_0";
	string GroupID = "1";
>;

string macro1
<
	string name = "bHighQuality";
	string desc = "是否开启高级光照";
	string type = "PRG";
	string GroupID = "2";
>;

string macro2
<
	string name = "bFixedDepth";
	string desc = "是否固定深度，远景片";
	string type = "PRG";
	string GroupID = "3";
>;

float4x4 g_matWorld : register(c0);
float4x4 g_matViewProj;
float3   g_vEyePos : EyePos;
float3   g_vLightDir : LightDir;

struct vsInput
{
    float4 Pos: POSITION;
    float3 Normal: NORMAL;
    float2 Tex: TEXCOORD0;
    float4 Tangent: TANGENT;
};

struct vsOutput
{
    float4 Pos: POSITION;
    float2 Tex: TEXCOORD0;
    float3 LightTS: TEXCOORD1;
    float3 CameraTS: TEXCOORD2;
#ifdef bPixelFog
    float3 WorldPos: TEXCOORD3;
    float3 WorldNormal: TEXCOORD4;
#else
    float4 Fog: TEXCOORD3;
#endif        
};

vsOutput vs_main(vsInput Input)
{
    vsOutput Output;

    float4 vWorldPos = mul(Input.Pos, g_matWorld);
    //vWorldPos.w = 1;
    float3 vCamera = normalize(g_vEyePos - vWorldPos.xyz);

    float4 vTangent = Input.Tangent;
    float3 vWorldNormal = normalize(mul(Input.Normal, (float3x3)(g_matWorld)));
    float3 vWorldTangent = normalize(mul(vTangent.xyz, (float3x3)(g_matWorld)));

    float3 vBinormal = -normalize(cross(vWorldNormal, vWorldTangent) * vTangent.w);
    float3x3 matTBN = { vWorldTangent, vBinormal, vWorldNormal };
    
    Output.Pos = mul(vWorldPos, g_matViewProj);
    Output.Tex = Input.Tex;
    Output.LightTS = mul(matTBN, -g_vLightDir);
    Output.CameraTS = mul(matTBN, vCamera);
    
#ifdef bPixelFog
    Output.WorldPos = vWorldPos.xyz;
    Output.WorldNormal = vWorldNormal;
#else
    Output.Fog = ComputeCompositeFogSimple(vCamera, vWorldPos.y);
#endif    
    return Output;    
}

sampler g_DiffuseSampler : register(s0);
sampler g_NormalSampler : register(s1);
sampler g_SpecularSampler : register(s2);
sampler g_ShadowSampler : register(s3);
float4 g_vDepthScale : register(c0);
float4 g_vSpecular : register(c1);

float4 g_vLightColor : Diffuse;
float4 g_vAmbient1 : Ambient0;
float4 g_vAmbient2 : Ambient1;

float4 ps_main(vsOutput Input

#ifdef bFixedDepth
, out float fDepth: DEPTH0
#endif

) : COLOR0
{
	float3 vLight = normalize(Input.LightTS);
	float3 vCamera = normalize(Input.CameraTS);
    
  	float4 colorOrg = tex2D(g_DiffuseSampler, Input.Tex);
    clip(colorOrg.a - 0.01);
    
    
#ifdef bHighQuality
    // 视差映射
	float height = tex2D(g_NormalSampler, Input.Tex).a;
    
    float2 heightScale = g_vDepthScale.xy * vCamera.xy;// / vCamera.z;
    float2 deltaTex = height * heightScale;
    float2 disTex = Input.Tex + deltaTex;
    
    // 修正
/**
    for (int i = 0; i < 0; i++)
    {
    	float height2 = tex2D(g_NormalSampler, disTex).a;
        if (height2 < height)
        {
            float fScale = height / (2 * height - height2);
            height *= fScale;
            deltaTex *= fScale;
        }
        disTex = Input.Tex + deltaTex;
    }    
**/
    
    //return float4(frac(disTex * 20), 0, 1);
	
	// 光照计算
	float4 color = tex2D(g_DiffuseSampler, disTex);
	//color = lerp(colorOrg, color, color.a);
	//color.rgb = 1;
	float4 normal = tex2D(g_NormalSampler, disTex);
	float3 vNormal = normalize(normal.xyz * 2 - 1);
	float shadow = tex2D(g_ShadowSampler, disTex).r;
	//shadow = lerp(1, shadow, color.a);
    //return float4(shadow, shadow, shadow, 1);

	color.a = colorOrg.a;

#else
    float4 color = colorOrg;
    return float4(1, 0, 1, 0);
	float3 vNormal = float3(0, 0, 1);
	float shadow = 1;
	float height = 0;
#endif

	float  fNdotL = dot(vNormal, vLight);
	//return float4(fNdotL, fNdotL, fNdotL, 1);
	
	float nd1 = saturate(fNdotL) * shadow;
	if (fNdotL > 0)
	{
	    fNdotL = lerp(0, fNdotL, shadow);
	}
	float nd2 = 1 - abs(fNdotL);
	float nd3 = saturate(-fNdotL);
	float3 lightColor = g_vLightColor.rgb * nd1 + g_vAmbient1.rgb * nd2 + g_vAmbient2.rgb * nd3;
	
	color.rgb = lightColor * color.rgb;
	
#ifdef bHighQuality	
	float3 vHalf = normalize(vCamera + vLight);
	float  fNdotH = saturate(dot(vNormal, vHalf));
	float4 specColor = tex2D(g_SpecularSampler, disTex);
    color.rgb += shadow * pow(fNdotH, g_vSpecular.a) * specColor.rgb * g_vSpecular.rgb * g_vLightColor.rgb;
#endif

    // 雾
#ifdef bPixelFog 	
    float3 vWorldPos = Input.WorldPos + Input.WorldNormal * height * g_vDepthScale.z;
	float4 fog = ComputeCompositeFogSimple(vWorldPos.xyz - g_vEyePos, vWorldPos.y);
#else
	float4 fog = Input.Fog;
#endif

#ifdef bFixedDepth
    fDepth = 0.999999f;
    //return float4(1, 0, 0, 1);
#endif

	color.rgb = lerp(fog.rgb, color.rgb, fog.w);

	return color;
}
