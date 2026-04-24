string entrynames = "ps_2_0 ps; vs_2_0 vs";	

#include "water_fft_common.h"

float4x4 g_lightViewMat : register(c0);
float3 g_PS_SunLightDir : register(c4); 	//太阳光方向 = float4(1,1,1);
float3 g_vCameraPos  : register(c5);
float g_fTime : register(c6);
float2 g_vCausticOffset : register(c7);
float g_fCausticLerp : register(c8);

float3 g_farConor0; //左上，右上，左下，右下
float3 g_farConor1; 
float3 g_farConor2; 
float3 g_farConor3; 
float4 g_vScreenSize;
float g_fFarPlane;
float g_fFovX_2;

//用户设置的参数:

sampler2D	oceanBumpMapSampler : register(s0);		//LINEAR Wrap
sampler2D	sceneDepthSampler 	: register(s1);		//POINT Clamp
sampler2D   causticSampler1		: register(s2);
sampler2D	causticSampler2		: register(s3);
sampler2D	screenDepthSampler  : register(s4);

struct VSOutPositionUVNormal
{
    float4 pos   : POSITION;   // vertex position 
    float2 uv    : TEXCOORD0;  // vertex texture coords 
    float3 n    : TEXCOORD1;
};

struct VSOutPositionUVNormal2
{
    float4 pos   : POSITION;   // vertex position 
    float2 uv    : TEXCOORD0;  // vertex texture coords 
    float3 n    : TEXCOORD1;
#if ((PROFILE == 3) || defined(_SDR_3_0))
    float4 screenSpace : VPOS;
#endif	
};

VSOutPositionUVNormal vs(float4 pos : POSITION,
                         float2 uv : TEXCOORD0,
                         float3 n : NORMAL)
{
    VSOutPositionUVNormal output;
    output.pos = pos;
    output.uv = uv;
    output.n = n;
    return output;
}

float4 ps(VSOutPositionUVNormal2 In) : COLOR0
{
    float4 output = 0;
	
#if ((PROFILE == 3) || defined(_SDR_3_0))
    float2 screenUV = ( In.screenSpace.xy) / g_vScreenSize.xy + g_vScreenSize.zw;	
#else
	float2 screenUV = In.uv;
#endif
		 
	//return float4(tex2D(screenDepthSampler, screenUV).g, 0, 0, 1);
	
	float fScreenDepth = tex2D(screenDepthSampler, float2(0.5, 0.5)).r;
	
	//fScreenDepth = max(fScreenDepth, 1);
	//return float4(fScreenDepth, 0,0,1);
	
    float depth01 = tex2D(sceneDepthSampler, screenUV).r;
	
    float depth = depth01 * g_fFarPlane;
	
	float bLeftEye = -1;
	
	
	float fLeftPoint = tex2D(screenDepthSampler, float2(0.4, 0.5)).g;
	float fRightPoint = tex2D(screenDepthSampler, float2(0.6, 0.5)).g;
	if( fRightPoint > fLeftPoint)
	{
		bLeftEye = 1;
	}
	
	float screenWidth_2 = fScreenDepth * g_fFovX_2;
	float leftSidePoint =  tex2D(screenDepthSampler, float2(0.0, 0.5)).r;
	float rightSidePoint = tex2D(screenDepthSampler, float2(1.0, 0.5)).r;
	
	float fSeparation;
	if(bLeftEye > 0)
	{
		fSeparation = rightSidePoint / (fScreenDepth - rightSidePoint) * 0.5;
		//return float4(rightSidePoint * 10, 0,0,1);
	}
	else
	{
		fSeparation = leftSidePoint / (fScreenDepth - leftSidePoint) * 0.5;
		//return float4(leftSidePoint * 10, 0,0,1);
	}
	
	//fSeparation = max(0.003, fSeparation);
	
	
    float2 uvFarPlane = screenUV;
	if(fScreenDepth == 0)
		;//return float4(1,1,0,1);
	else
	{
		float offset;

		offset = fSeparation * (depth - fScreenDepth) / depth * bLeftEye;
		
		/*if(bLeftEye > 0)
			return float4(1,0,0,1);
		else
			return float4(0,1,0,1);
		*/
		
		uvFarPlane.x += offset; //  * 0.5;// / 7.5591;
	}
	
	float3 worldP = lerp( lerp(g_farConor0.xyz, g_farConor1.xyz, uvFarPlane.x), lerp(g_farConor2.xyz, g_farConor3.xyz, uvFarPlane.x), uvFarPlane.y) 
					* depth01 + g_vCameraPos.xyz;
	
	//return float4(frac(worldP), 1);
	
	if(depth01 > 0.9)
		return float4(0,0,0,0);

	float alpha = 1;

	float3 cCaustic = ComputeCausticColor2(worldP, (float3x3)(g_lightViewMat), g_vCausticOffset, causticSampler1, causticSampler2, g_fCausticLerp);
	
	float3 cCausticOrig = cCaustic;  
  
	float fHighlightAtten = 1;
	//// Power-up caustic to make it sharper, and apply sun color                                       // 3 alu
	cCaustic = pow( cCaustic, 3  ) * fHighlightAtten;
	
	return float4(cCaustic, alpha);

}



