string entrynames = "vs_2_0 vs_main";

float4x4 g_viewProjCurrent;
float4x4 g_viewProjLast;
float3 g_cameraPos;
float4  g_ScreenSize = float4(1024.f, 768.f, 0.5f / 1024.f, 0.5f / 768.f);

float4 HPosToScreenTC(float4 HPos)
{
	float4 ScrTC = HPos;
	ScrTC.xy = (HPos.xy * float2(1, -1) + HPos.ww) * 0.5;
	ScrTC.xy += g_ScreenSize.zw * HPos.w;
	return ScrTC;
}

struct MotionBlurVSOut
{
	float4 HPosition  : POSITION;
	float4 tcProj     : TEXCOORD0;
	float4 tcNewPos   : TEXCOORD1;
	float4 tcPrevPos  : TEXCOORD2;
};

MotionBlurVSOut vs_main(float4 pos : POSITION)
{
	MotionBlurVSOut OUT = (MotionBlurVSOut)0; 

	float4 vPos = pos;
	//±¾À´ÊÇ25
	const float g_sphereSize = 25;
	vPos.xyz = normalize(vPos.xyz) * g_sphereSize; // * motionBlurCamParams.w; // sphere size needs to be tweakable for setting blur strenght
	vPos.xyz += g_cameraPos;

	OUT.HPosition = mul(vPos, g_viewProjCurrent);  
	  
	float4 vNewPos = OUT.HPosition;
	float4 vPrevPos =  mul(vPos, g_viewProjLast);

	OUT.tcNewPos =  HPosToScreenTC(vNewPos);
	OUT.tcPrevPos = HPosToScreenTC(vPrevPos);  

	OUT.tcProj = OUT.tcNewPos;

	return OUT;
}