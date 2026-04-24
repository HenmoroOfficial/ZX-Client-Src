#include "Stdafx.h"
#include "PostEffectParam.h"
#include "Global.h"

CPostEffectParam::CPostEffectParam()
{

}

CPostEffectParam::~CPostEffectParam()
{

}

bool	CPostEffectParam::SavePostParams()
{
	char szStr[256], szSect[128];
	AIniFile file;

	//Masks
	strcpy(szSect, "Masks");
	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.dwPostEffectMask);
	file.WriteStringValue(szSect, "Masks", szStr);

	strcpy(szSect, "AdditionalMasks");
	_snprintf(szStr, sizeof(szStr), "%d", g_dwPostEffectAdditionalFlags);
	file.WriteStringValue(szSect, "AdditionalMasks", szStr);

	//Bloom
	strcpy(szSect, "Bloom");
	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.bloomParam.fBlurSize);
	file.WriteStringValue(szSect, "BlurSize", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.bloomParam.fBrightThreshold);
	file.WriteStringValue(szSect, "BrightThreshold", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.bloomParam.fBrightScale);
	file.WriteStringValue(szSect, "BrightScale", szStr);

	//CC
	strcpy(szSect, "CC");
	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.ccParam.fFactor);
	file.WriteStringValue(szSect, "Factor", szStr);
	
	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.ccParam.bUseManualParam1?1:0);
	file.WriteStringValue(szSect, "UseManualParam1", szStr);

	_snprintf(szStr, sizeof(szStr), "%s", g_PostEffectParam.ccParam.szTex1);
	file.WriteStringValue(szSect, "Tex1", szStr);

	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.ccParam.manualParam1.nBright);
	file.WriteStringValue(szSect, "manual1_Bright", szStr);

	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.ccParam.manualParam1.nContrast);
	file.WriteStringValue(szSect, "manual1_Contrast", szStr);

	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.ccParam.manualParam1.nHue);
	file.WriteStringValue(szSect, "manual1_Hue", szStr);

	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.ccParam.manualParam1.nSaturation);
	file.WriteStringValue(szSect, "manual1_Saturation", szStr);

	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.ccParam.manualParam1.bGrey?1:0);
	file.WriteStringValue(szSect, "manual1_Grey", szStr);

	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.ccParam.manualParam1.bOpposition?1:0);
	file.WriteStringValue(szSect, "manual1_Opposition", szStr);

	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.ccParam.bUseManualParam2?1:0);
	file.WriteStringValue(szSect, "UseManualParam2", szStr);

	_snprintf(szStr, sizeof(szStr), "%s", g_PostEffectParam.ccParam.szTex2);
	file.WriteStringValue(szSect, "Tex2", szStr);

	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.ccParam.manualParam2.nBright);
	file.WriteStringValue(szSect, "manual2_Bright", szStr);

	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.ccParam.manualParam2.nContrast);
	file.WriteStringValue(szSect, "manual2_Contrast", szStr);

	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.ccParam.manualParam2.nHue);
	file.WriteStringValue(szSect, "manual2_Hue", szStr);

	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.ccParam.manualParam2.nSaturation);
	file.WriteStringValue(szSect, "manual2_Saturation", szStr);

	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.ccParam.manualParam2.bGrey?1:0);
	file.WriteStringValue(szSect, "manual2_Grey", szStr);

	_snprintf(szStr, sizeof(szStr), "%d", g_PostEffectParam.ccParam.manualParam2.bOpposition?1:0);
	file.WriteStringValue(szSect, "manual2_Opposition", szStr);

	//Dof
	strcpy(szSect, "Dof");
	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.dofParam.fNearBlur);
	file.WriteStringValue(szSect, "NearBlur", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.dofParam.fNearFocus);
	file.WriteStringValue(szSect, "NearFocus", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.dofParam.fFarFocus);
	file.WriteStringValue(szSect, "FarFocus", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.dofParam.fFarBlur);
	file.WriteStringValue(szSect, "FarBlur", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.dofParam.fClampBlurFar);
	file.WriteStringValue(szSect, "ClampBlurFar", szStr);

	//GodRay
	strcpy(szSect, "GodRay");
	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.godRayParam.fOcclusionDepthRange);
	file.WriteStringValue(szSect, "OcclusionDepthRange", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.godRayParam.fBloomScale);
	file.WriteStringValue(szSect, "BloomScale", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.godRayParam.fBloomThreshold);
	file.WriteStringValue(szSect, "BloomThreshold", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.godRayParam.fScreenBlendThreshold);
	file.WriteStringValue(szSect, "ScreenBlendThreshold", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.godRayParam.vBloomTintColor.x);
	file.WriteStringValue(szSect, "BloomTintColor_R", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.godRayParam.vBloomTintColor.y);
	file.WriteStringValue(szSect, "BloomTintColor_G", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.godRayParam.vBloomTintColor.z);
	file.WriteStringValue(szSect, "BloomTintColor_B", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.godRayParam.fOcclusionDarkness);
	file.WriteStringValue(szSect, "OcclusionDarkness", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.godRayParam.fVisibleAngle);
	file.WriteStringValue(szSect, "VisibleAngle", szStr);
	
	//RadiusBlur
	strcpy(szSect, "RadiusBlur");
	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.radiusBlurParam.fBlurCenterX);
	file.WriteStringValue(szSect, "BlurCenterX", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.radiusBlurParam.fBlurCenterY);
	file.WriteStringValue(szSect, "BlurCenterY", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.radiusBlurParam.fBlurLength);
	file.WriteStringValue(szSect, "BlurLength", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.radiusBlurParam.fInFocusRadius);
	file.WriteStringValue(szSect, "InFocusRadius", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.radiusBlurParam.fOutFocusRadius);
	file.WriteStringValue(szSect, "OutFocusRadius", szStr);

	//MotionBlur
	strcpy(szSect, "MotionBlur");
	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.motionBlurParam.fBlurMax);
	file.WriteStringValue(szSect, "BlurMax", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.motionBlurParam.fBlurScale);
	file.WriteStringValue(szSect, "BlurScale", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.motionBlurParam.fBlurScalePosition);
	file.WriteStringValue(szSect, "BlurScalePosition", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.motionBlurParam.fBlurScaleRotation);
	file.WriteStringValue(szSect, "BlurScaleRotation", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.motionBlurParam.fInFocusDistance);
	file.WriteStringValue(szSect, "InFocusDistance", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.motionBlurParam.fOutFocusDistance);
	file.WriteStringValue(szSect, "OutFocusDistance", szStr);

	//Warp
	strcpy(szSect, "Warp");
	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.warpParam.fScale);
	file.WriteStringValue(szSect, "Scale", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.warpParam.fSpeed);
	file.WriteStringValue(szSect, "Speed", szStr);

	//FullGlow
	strcpy(szSect, "FullGlow");
	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.fullGlowParam.fBlurSize);
	file.WriteStringValue(szSect, "BlurSize", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.fullGlowParam.fGlowLevel);
	file.WriteStringValue(szSect, "GlowLevel", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.fullGlowParam.fGlowPower);
	file.WriteStringValue(szSect, "GlowPower", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.fullGlowParam.vGlowColor.x);
	file.WriteStringValue(szSect, "GlowColor_R", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.fullGlowParam.vGlowColor.y);
	file.WriteStringValue(szSect, "GlowColor_G", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.fullGlowParam.vGlowColor.z);
	file.WriteStringValue(szSect, "GlowColor_B", szStr);

	_snprintf(szStr, sizeof(szStr), "%f", g_PostEffectParam.fullGlowParam.vGlowColor.w);
	file.WriteStringValue(szSect, "GlowColor_A", szStr);
	
	if(!file.Save("PostEffect_Params.ini"))
		return false;

	file.Close();
	return true;
}

bool	CPostEffectParam::LoadPostParams()
{
	char szSect[128];
	int nValue = 0;
	float fValue = 0.0f;

	AIniFile file;
	if(!file.Open("PostEffect_Params.ini"))
		return false;

	//Masks
	strcpy(szSect, "Masks");
	int nFlags=0;
	LoadIntFromIni(&file,szSect,"Masks",nFlags);
	g_PostEffectParam.dwPostEffectMask = DWORD(nFlags);	

	strcpy(szSect, "AdditionalMasks");
	nFlags=0;
	LoadIntFromIni(&file,szSect,"AdditionalMasks",nFlags);
	g_dwPostEffectAdditionalFlags = DWORD(nFlags);	

	//Bloom
	strcpy(szSect, "Bloom");
	LoadFloatFromIni(&file,szSect,"BlurSize",g_PostEffectParam.bloomParam.fBlurSize);
	LoadFloatFromIni(&file,szSect,"BrightThreshold",g_PostEffectParam.bloomParam.fBrightThreshold);
	LoadFloatFromIni(&file,szSect,"BrightScale",g_PostEffectParam.bloomParam.fBrightScale);
	
	//CC
	strcpy(szSect, "CC");
	LoadFloatFromIni(&file,szSect,"Factor",g_PostEffectParam.ccParam.fFactor);

	LoadBoolFromIni(&file,szSect,"UseManualParam1",g_PostEffectParam.ccParam.bUseManualParam1);
	LoadStringFromIni(&file,szSect,"Tex1",g_PostEffectParam.ccParam.szTex1);
	LoadIntFromIni(&file,szSect,"manual1_Bright",g_PostEffectParam.ccParam.manualParam1.nBright);
	LoadIntFromIni(&file,szSect,"manual1_Contrast",g_PostEffectParam.ccParam.manualParam1.nContrast);
	LoadIntFromIni(&file,szSect,"manual1_Hue",g_PostEffectParam.ccParam.manualParam1.nHue);
	LoadIntFromIni(&file,szSect,"manual1_Saturation",g_PostEffectParam.ccParam.manualParam1.nSaturation);
	LoadBoolFromIni(&file,szSect,"manual1_Grey",g_PostEffectParam.ccParam.manualParam1.bGrey);
	LoadBoolFromIni(&file,szSect,"manual1_Opposition",g_PostEffectParam.ccParam.manualParam1.bOpposition);
	
	LoadBoolFromIni(&file,szSect,"UseManualParam2",g_PostEffectParam.ccParam.bUseManualParam2);
	LoadStringFromIni(&file,szSect,"Tex2",g_PostEffectParam.ccParam.szTex2);
	LoadIntFromIni(&file,szSect,"manual2_Bright",g_PostEffectParam.ccParam.manualParam2.nBright);
	LoadIntFromIni(&file,szSect,"manual2_Contrast",g_PostEffectParam.ccParam.manualParam2.nContrast);
	LoadIntFromIni(&file,szSect,"manual2_Hue",g_PostEffectParam.ccParam.manualParam2.nHue);
	LoadIntFromIni(&file,szSect,"manual2_Saturation",g_PostEffectParam.ccParam.manualParam2.nSaturation);
	LoadBoolFromIni(&file,szSect,"manual2_Grey",g_PostEffectParam.ccParam.manualParam2.bGrey);
	LoadBoolFromIni(&file,szSect,"manual2_Opposition",g_PostEffectParam.ccParam.manualParam2.bOpposition);		

	//Dof
	strcpy(szSect, "Dof");
	LoadFloatFromIni(&file,szSect,"NearBlur",g_PostEffectParam.dofParam.fNearBlur);
	LoadFloatFromIni(&file,szSect,"NearFocus",g_PostEffectParam.dofParam.fNearFocus);
	LoadFloatFromIni(&file,szSect,"FarFocus",g_PostEffectParam.dofParam.fFarFocus);
	LoadFloatFromIni(&file,szSect,"FarBlur",g_PostEffectParam.dofParam.fFarBlur);
	LoadFloatFromIni(&file,szSect,"ClampBlurFar",g_PostEffectParam.dofParam.fClampBlurFar);

	//GodRay
	strcpy(szSect, "GodRay");
	LoadFloatFromIni(&file,szSect,"OcclusionDepthRange",g_PostEffectParam.godRayParam.fOcclusionDepthRange);
	LoadFloatFromIni(&file,szSect,"BloomScale",g_PostEffectParam.godRayParam.fBloomScale);
	LoadFloatFromIni(&file,szSect,"BloomThreshold",g_PostEffectParam.godRayParam.fBloomThreshold);
	LoadFloatFromIni(&file,szSect,"ScreenBlendThreshold",g_PostEffectParam.godRayParam.fScreenBlendThreshold);

	LoadFloatFromIni(&file,szSect,"BloomTintColor_R",g_PostEffectParam.godRayParam.vBloomTintColor.x);
	LoadFloatFromIni(&file,szSect,"BloomTintColor_G",g_PostEffectParam.godRayParam.vBloomTintColor.y);
	LoadFloatFromIni(&file,szSect,"BloomTintColor_B",g_PostEffectParam.godRayParam.vBloomTintColor.z);

	LoadFloatFromIni(&file,szSect,"OcclusionDarkness",g_PostEffectParam.godRayParam.fOcclusionDarkness);
	LoadFloatFromIni(&file,szSect,"VisibleAngle",g_PostEffectParam.godRayParam.fVisibleAngle);

	//RadiusBlur
	strcpy(szSect, "RadiusBlur");
	LoadFloatFromIni(&file,szSect,"BlurCenterX",g_PostEffectParam.radiusBlurParam.fBlurCenterX);
	LoadFloatFromIni(&file,szSect,"BlurCenterY",g_PostEffectParam.radiusBlurParam.fBlurCenterY);
	LoadFloatFromIni(&file,szSect,"BlurLength",g_PostEffectParam.radiusBlurParam.fBlurLength);
	LoadFloatFromIni(&file,szSect,"InFocusRadius",g_PostEffectParam.radiusBlurParam.fInFocusRadius);
	LoadFloatFromIni(&file,szSect,"OutFocusRadius",g_PostEffectParam.radiusBlurParam.fOutFocusRadius);

	//MotionBlur
	strcpy(szSect, "MotionBlur");
	LoadFloatFromIni(&file,szSect,"BlurMax",g_PostEffectParam.motionBlurParam.fBlurMax);
	LoadFloatFromIni(&file,szSect,"BlurScale",g_PostEffectParam.motionBlurParam.fBlurScale);
	LoadFloatFromIni(&file,szSect,"BlurScalePosition",g_PostEffectParam.motionBlurParam.fBlurScalePosition);
	LoadFloatFromIni(&file,szSect,"BlurScaleRotation",g_PostEffectParam.motionBlurParam.fBlurScaleRotation);
	LoadFloatFromIni(&file,szSect,"InFocusDistance",g_PostEffectParam.motionBlurParam.fInFocusDistance);
	LoadFloatFromIni(&file,szSect,"OutFocusDistance",g_PostEffectParam.motionBlurParam.fOutFocusDistance);

	//Warp
	strcpy(szSect, "Warp");
	LoadFloatFromIni(&file,szSect,"Scale",g_PostEffectParam.warpParam.fScale);
	LoadFloatFromIni(&file,szSect,"Speed",g_PostEffectParam.warpParam.fSpeed);

	//FullGlow
	strcpy(szSect, "FullGlow");
	LoadFloatFromIni(&file,szSect,"BlurSize",g_PostEffectParam.fullGlowParam.fBlurSize);
	LoadFloatFromIni(&file,szSect,"GlowLevel",g_PostEffectParam.fullGlowParam.fGlowLevel);
	LoadFloatFromIni(&file,szSect,"GlowPower",g_PostEffectParam.fullGlowParam.fGlowPower);
	LoadFloatFromIni(&file,szSect,"GlowColor_R",g_PostEffectParam.fullGlowParam.vGlowColor.x);
	LoadFloatFromIni(&file,szSect,"GlowColor_G",g_PostEffectParam.fullGlowParam.vGlowColor.y);
	LoadFloatFromIni(&file,szSect,"GlowColor_B",g_PostEffectParam.fullGlowParam.vGlowColor.z);
	LoadFloatFromIni(&file,szSect,"GlowColor_A",g_PostEffectParam.fullGlowParam.vGlowColor.w);

	return true;
}

void 	CPostEffectParam::LoadBoolFromIni(AIniFile* pFileIni,const char* szSect,const char* szKey,bool& bValue)
{
	ASSERT(pFileIni);

	int nValue = bValue ? 1 : 0;
	nValue = pFileIni->GetValueAsInt(szSect, szKey, nValue);
	bValue = (nValue==0 ? false : true);
}

void 	CPostEffectParam::LoadIntFromIni(AIniFile* pFileIni,const char* szSect,const char* szKey,int& nValue)
{
	ASSERT(pFileIni);
	nValue = pFileIni->GetValueAsInt(szSect, szKey, nValue);
}

void 	CPostEffectParam::LoadFloatFromIni(AIniFile* pFileIni,const char* szSect,const char* szKey,float& fValue)
{
	ASSERT(pFileIni);
	fValue = pFileIni->GetValueAsFloat(szSect,szKey,fValue);
}

void 	CPostEffectParam::LoadStringFromIni(AIniFile* pFileIni,const char* szSect,const char* szKey,AString& strValue)
{
	ASSERT(pFileIni);
	strValue = pFileIni->GetValueAsString(szSect,szKey,strValue);
}