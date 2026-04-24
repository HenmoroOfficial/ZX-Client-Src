/*
 * FILE: DLGSETTINGEFFECT.CPP
 *
 * DESCRIPTION: settings of effect, for debug
 *
 * CREATED BY: hanwei, 2012/2/22
 *
 * HISTORY:
 *
 * Copyright (c) 2010 Hummingbird Studio, All Rights Reserved.
 */
//#include "StdAfx.h"
#include "DlgSettingEffect.h"
#include "ECViewport.h"
//#include "A3DViewport.h"
#include "EC_Configs.h"
#include "EC_Global.h"
#include "EC_Game.h"
//#include "DlgSettings.h"
//#include "EC_SafeStr.h"


///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////





// panel msg
AUI_BEGIN_COMMAND_MAP(CDlgSettingEffect, CDlgBase)
//AUI_ON_COMMAND(("Check_*"),						OnCommandOperate)
//AUI_ON_COMMAND(("Slider_*"),						OnCommandOperate)
AUI_ON_COMMAND(("*"),						OnCommandConfirm)
AUI_ON_COMMAND(NULL,						OnCommandConfirm)
AUI_END_COMMAND_MAP()


// windows msg
AUI_BEGIN_EVENT_MAP(CDlgSettingEffect, CDlgBase)
AUI_ON_EVENT("*",		WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,			WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_END_EVENT_MAP()



///////////////////////////////////////////////////////////////////////////
//	
//	Overrides AUIDialog
//	
///////////////////////////////////////////////////////////////////////////

CDlgSettingEffect::CDlgSettingEffect()
{
}


CDlgSettingEffect::~CDlgSettingEffect()
{
}


bool CDlgSettingEffect::OnInitDialog()
{
	return true;
}


void CDlgSettingEffect::OnShowDialog()
{
	UpdateData(true);
}

void CDlgSettingEffect::DoDataExchange(bool bSave)
{
	/*
	// Check_RenderGFX
	if (bSave)
		glb_GetConfigs()->SetRenderGFX(IsItemChecked(("Check_RenderGFX")));		
	else
		CheckItem(("Check_RenderGFX"), glb_GetConfigs()->IsRenderGFX());

	// Check_RenderGrass
	if (bSave)
		glb_GetConfigs()->SetRenderGrass(IsItemChecked(("Check_RenderGrass")));		
	else
		CheckItem(("Check_RenderGrass"), glb_GetConfigs()->IsRenderGrass());

	// Check_RenderLitModel
	if (bSave)
		glb_GetConfigs()->SetRenderLitModel(IsItemChecked(("Check_RenderLitModel")));		
	else
		CheckItem(("Check_RenderLitModel"), glb_GetConfigs()->IsRenderLitModel());

	// Check_RenderPostProcess
	if (bSave)
		glb_GetConfigs()->SetRenderPostProcess(IsItemChecked(("Check_RenderPostProcess")));		
	else
		CheckItem(("Check_RenderPostProcess"), glb_GetConfigs()->IsRenderPostProcess());

	// Check_RenderScene
	if (bSave)
		glb_GetConfigs()->SetRenderScene(IsItemChecked(("Check_RenderScene")));		
	else
		CheckItem(("Check_RenderScene"), glb_GetConfigs()->IsRenderScene());

	// Check_RenderSceneObiect
	if (bSave)
		glb_GetConfigs()->SetRenderSceneObject(IsItemChecked(("Check_RenderSceneObiect")));		
	else
		CheckItem(("Check_RenderSceneObiect"), glb_GetConfigs()->IsRenderSceneObject());

	// Check_RenderShadow
	if (bSave)
		glb_GetConfigs()->SetDebugRenderShadow(IsItemChecked(("Check_RenderShadow")));		
	else
		CheckItem(("Check_RenderShadow"), glb_GetConfigs()->IsDebugRenderShadow());

	// Check_RenderSkinModel
	if (bSave)
		glb_GetConfigs()->SetRenderSkinModel(IsItemChecked(("Check_RenderSkinModel")));		
	else
		CheckItem(("Check_RenderSkinModel"), glb_GetConfigs()->IsRenderSkinModel());

	// Check_RenderSky
	if (bSave)
		glb_GetConfigs()->SetRenderSky(IsItemChecked(("Check_RenderSky")));		
	else
		CheckItem(("Check_RenderSky"), glb_GetConfigs()->IsRenderSky());

	// Check_RenderTerrain
	if (bSave)
		glb_GetConfigs()->SetRenderTerrain(IsItemChecked(("Check_RenderTerrain")));		
	else
		CheckItem(("Check_RenderTerrain"), glb_GetConfigs()->IsRenderTerrain());

	// Check_RenderWater
	if (bSave)
		glb_GetConfigs()->SetRenderWater(IsItemChecked(("Check_RenderWater")));		
	else
		CheckItem(("Check_RenderWater"), glb_GetConfigs()->IsRenderWater());
*/
	if (bSave)
		if (IsItemChecked(("Chk_TestPostEffect")))
			GetGame()->GetConfigs()->SetPostEffectType(1);
		else
			GetGame()->GetConfigs()->SetPostEffectType(0);
	else
		CheckItem(("Chk_TestPostEffect"), GetGame()->GetConfigs()->GetPostEffectType()==1);

	A3DPOST_EFFECT_PARAM* pParam = &m_PostEffectParam;
	if (NULL == pParam) return;

	// 反走样
	if (bSave)
		if (IsItemChecked(("Check_HoldShape")))
			pParam->dwPostEffectMask |= A3DPOST_EFFECT_AA;
		else
			pParam->dwPostEffectMask &= ~A3DPOST_EFFECT_AA; 
	else
		CheckItem(("Check_HoldShape"), (pParam->dwPostEffectMask & A3DPOST_EFFECT_AA) != 0);

	// 全屏泛光
	int val = 0;
	if (bSave)
	{
		// 开启
		if (IsItemChecked(("Check_FullScreenLight")))
			pParam->dwPostEffectMask |= A3DPOST_EFFECT_Bloom;
		else
			pParam->dwPostEffectMask &= ~A3DPOST_EFFECT_Bloom;
		// 模糊
		if (GetDlgItem(("Edit_FullScreenLight1"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_FullScreenLight1"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 0.1f);
			pParam->bloomParam.fBlurSize = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_FullScreenLight1"), val);
			pParam->bloomParam.fBlurSize = ((float)val) / 1000;
		
		}
		// 高光阈值
		if (GetDlgItem(("Edit_FullScreenLight2"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_FullScreenLight2"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 1.0f);
			pParam->bloomParam.fBrightThreshold = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_FullScreenLight2"), val);
			pParam->bloomParam.fBrightThreshold = ((float)val) / 100;
		}		
		// 亮度
		if (GetDlgItem(("Edit_FullScreenLight3"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_FullScreenLight3"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 5.0f);
			pParam->bloomParam.fBrightScale = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_FullScreenLight3"), val);
			pParam->bloomParam.fBrightScale = ((float)val) / 20;
		}
	}
	{
		CheckItem(("Check_FullScreenLight"), (pParam->dwPostEffectMask & A3DPOST_EFFECT_Bloom) != 0);
		AWString str;
		val = (int)(pParam->bloomParam.fBlurSize * 1000);
		DDX_Slider(false, ("Slider_FullScreenLight1"), val);
		str.Format(_AL("%0.4f"), pParam->bloomParam.fBlurSize);
		SetItemText(("Edit_FullScreenLight1"), str);

		val = (int)(pParam->bloomParam.fBrightThreshold * 100);
		DDX_Slider(false, ("Slider_FullScreenLight2"), val);
		str.Format(_AL("%0.4f"), pParam->bloomParam.fBrightThreshold);
		SetItemText(("Edit_FullScreenLight2"), str);

		val = (int)(pParam->bloomParam.fBrightScale * 20);
		DDX_Slider(false, ("Slider_FullScreenLight3"), val);
		str.Format(_AL("%0.4f"), pParam->bloomParam.fBrightScale);
		SetItemText(("Edit_FullScreenLight3"), str);
	}	

	// 色彩校正
	if (bSave)
	{
		if (IsItemChecked(("Check_ColorCheck")))
			pParam->dwPostEffectMask |= A3DPOST_EFFECT_CC;
		else
			pParam->dwPostEffectMask &= ~A3DPOST_EFFECT_CC; 
		// 过渡
		if (GetDlgItem(("Edit_ColorCheck3"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_ColorCheck3"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 1.0f);
			pParam->ccParam.fFactor = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_ColorCheck3"), val);
			pParam->ccParam.fFactor = ((float)val) / 100;
		}		
		pParam->ccParam.szTex1 = WC2AS(GetItemText(("Edit_ColorCheck1")));
		pParam->ccParam.szTex2 = WC2AS(GetItemText(("Edit_ColorCheck2")));
	}
	{
		CheckItem(("Check_ColorCheck"), (pParam->dwPostEffectMask & A3DPOST_EFFECT_CC) != 0);
		val = (int)(pParam->ccParam.fFactor * 100);
		DDX_Slider(false, ("Slider_ColorCheck3"), val);
		AWString str;
		str.Format(_AL("%0.4f"), pParam->ccParam.fFactor);
		SetItemText(("Edit_ColorCheck3"), str);
		SetItemText(("Edit_ColorCheck1"), AS2WC(pParam->ccParam.szTex1));
		SetItemText(("Edit_ColorCheck2"), AS2WC(pParam->ccParam.szTex2));
	}

	// 天光
	if (bSave)
	{
		if (IsItemChecked(("Check_SkyLight")))
			pParam->dwPostEffectMask |= A3DPOST_EFFECT_GodRay;
		else
			pParam->dwPostEffectMask &= ~A3DPOST_EFFECT_GodRay; 
		// 深度范围
		if (GetDlgItem(("Edit_SkyLight1"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_SkyLight1"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 1.0f);
			pParam->godRayParam.fOcclusionDepthRange = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_SkyLight1"), val);
			pParam->godRayParam.fOcclusionDepthRange = ((float)val) / 100 * 2000;
		}
		// Bloom叠加强度
		if (GetDlgItem(("Edit_SkyLight2"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_SkyLight2"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 5.0f);
			pParam->godRayParam.fBloomScale = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_SkyLight2"), val);
			pParam->godRayParam.fBloomScale = ((float)val) / 10;
		}
		// Bloom阈值
		if (GetDlgItem(("Edit_SkyLight3"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_SkyLight3"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 1.0f);
			pParam->godRayParam.fBloomThreshold = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_SkyLight3"), val);
			pParam->godRayParam.fBloomThreshold = ((float)val) / 100;
		}
		// 屏幕混合阈值
		if (GetDlgItem(("Edit_SkyLight4"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_SkyLight4"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 1.0f);
			pParam->godRayParam.fScreenBlendThreshold = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_SkyLight4"), val);
			pParam->godRayParam.fScreenBlendThreshold = ((float)val) / 100;
		}
		// 颜色参数
		AWString num = GetItemText(("Edit_ColorR"));
		pParam->godRayParam.vBloomTintColor.x = num.ToFloat();
		num = GetItemText(("Edit_ColorG"));
		pParam->godRayParam.vBloomTintColor.y = num.ToFloat();
		num = GetItemText(("Edit_ColorB"));
		pParam->godRayParam.vBloomTintColor.z = num.ToFloat();
		// 暗化
		if (GetDlgItem(("Edit_SkyLight6"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_SkyLight6"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 1.0f);
			pParam->godRayParam.fOcclusionDarkness = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_SkyLight6"), val);
			pParam->godRayParam.fOcclusionDarkness = ((float)val) / 100;
		}
		// 可见角度
		if (GetDlgItem(("Edit_SkyLight7"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_SkyLight7"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 3.1415f);
			pParam->godRayParam.fVisibleAngle = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_SkyLight7"), val);
			pParam->godRayParam.fVisibleAngle = ((float)val) / 100;
		}
	}
	{
		AWString str;
		CheckItem(("Check_SkyLight"), (pParam->dwPostEffectMask & A3DPOST_EFFECT_GodRay) != 0);
		val = (int)(pParam->godRayParam.fOcclusionDepthRange * 100.f / 2000.f);
		DDX_Slider(false, ("Slider_SkyLight1"), val);
		str.Format(_AL("%0.1f"), pParam->godRayParam.fOcclusionDepthRange);
		SetItemText(("Edit_SkyLight1"), str);

		val = (int)(pParam->godRayParam.fBloomScale * 10);
		DDX_Slider(false, ("Slider_SkyLight2"), val);
		str.Format(_AL("%0.4f"), pParam->godRayParam.fBloomScale);
		SetItemText(("Edit_SkyLight2"), str);

		val = (int)(pParam->godRayParam.fBloomThreshold * 100);
		DDX_Slider(false, ("Slider_SkyLight3"), val);
		str.Format(_AL("%0.4f"), pParam->godRayParam.fBloomThreshold);
		SetItemText(("Edit_SkyLight3"), str);

		val = (int)(pParam->godRayParam.fScreenBlendThreshold * 100);
		DDX_Slider(false, ("Slider_SkyLight4"), val);
		str.Format(_AL("%0.4f"), pParam->godRayParam.fScreenBlendThreshold);
		SetItemText(("Edit_SkyLight4"), str);

		str.Format(_AL("%0.2f"), pParam->godRayParam.vBloomTintColor.x);
		SetItemText(("Edit_ColorR"), str);
		str.Format(_AL("%0.2f"), pParam->godRayParam.vBloomTintColor.y);
		SetItemText(("Edit_ColorG"), str);
		str.Format(_AL("%0.2f"), pParam->godRayParam.vBloomTintColor.z);
		SetItemText(("Edit_ColorB"), str);

		val = (int)(pParam->godRayParam.fOcclusionDarkness * 100);
		DDX_Slider(false, ("Slider_SkyLight6"), val);
		str.Format(_AL("%0.4f"), pParam->godRayParam.fOcclusionDarkness);
		SetItemText(("Edit_SkyLight6"), str);

		val = (int)(pParam->godRayParam.fVisibleAngle * 100);
		DDX_Slider(false, ("Slider_SkyLight7"), val);
		str.Format(_AL("%0.4f"), pParam->godRayParam.fVisibleAngle);
		SetItemText(("Edit_SkyLight7"), str);
	}

	// 径向模糊
	if (bSave)
	{
		if (IsItemChecked(("Check_RadialBlur")))
			pParam->dwPostEffectMask |= A3DPOST_EFFECT_RadialBlur;
		else
			pParam->dwPostEffectMask &= ~A3DPOST_EFFECT_RadialBlur; 
		// 模糊长度
		if (GetDlgItem(("Edit_RadialBlur2"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_RadialBlur2"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 1.00f);
			pParam->radiusBlurParam.fBlurLength = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_RadialBlur2"), val);
			pParam->radiusBlurParam.fBlurLength = ((float)val) / 100;
		}
		// 焦点半径
		if (GetDlgItem(("Edit_RadialBlur3"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_RadialBlur3"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 2.00f);
			pParam->radiusBlurParam.fInFocusRadius = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_RadialBlur3"), val);
			pParam->radiusBlurParam.fInFocusRadius = ((float)val) / 100;
		}
		// 模糊半径
		if (GetDlgItem(("Edit_RadialBlur4"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_RadialBlur4"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 3.00f);
			pParam->radiusBlurParam.fOutFocusRadius = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_RadialBlur4"), val);
			pParam->radiusBlurParam.fOutFocusRadius = ((float)val) / 100;
		}
		AWString num = GetItemText(("Edit_CoordinateX"));
		pParam->radiusBlurParam.fBlurCenterX = num.ToFloat();
		num = GetItemText(("Edit_CoordinateY"));
		pParam->radiusBlurParam.fBlurCenterY = num.ToFloat();
	}
	{
		AWString str;
		CheckItem(("Check_RadialBlur"), (pParam->dwPostEffectMask & A3DPOST_EFFECT_RadialBlur) != 0);

		val = (int)(pParam->radiusBlurParam.fBlurLength * 100);
		DDX_Slider(false, ("Slider_RadialBlur2"), val);
		str.Format(_AL("%0.4f"), pParam->radiusBlurParam.fBlurLength);
		SetItemText(("Edit_RadialBlur2"), str);

		val = (int)(pParam->radiusBlurParam.fInFocusRadius * 100);
		DDX_Slider(false, ("Slider_RadialBlur3"), val);
		str.Format(_AL("%0.4f"), pParam->radiusBlurParam.fInFocusRadius);
		SetItemText(("Edit_RadialBlur3"), str);

		val = (int)(pParam->radiusBlurParam.fOutFocusRadius * 100);
		DDX_Slider(false, ("Slider_RadialBlur4"), val);
		str.Format(_AL("%0.4f"), pParam->radiusBlurParam.fOutFocusRadius);
		SetItemText(("Edit_RadialBlur4"), str);

		str.Format(_AL("%0.2f"), pParam->radiusBlurParam.fBlurCenterX);
		SetItemText(("Edit_CoordinateX"), str);
		str.Format(_AL("%0.2f"), pParam->radiusBlurParam.fBlurCenterY);
		SetItemText(("Edit_CoordinateY"), str);
	}

	// 景深
	CECViewport* pViewport = g_pGame->GetViewport();
	A3DCamera* pCamera = pViewport->GetA3DCamera();
	float diff = pCamera->GetZBack() - pCamera->GetZFront();
	if (bSave)
	{
		if (IsItemChecked(("Check_FieldDepth")))
			pParam->dwPostEffectMask |= A3DPOST_EFFECT_Dof;
		else
			pParam->dwPostEffectMask &= ~A3DPOST_EFFECT_Dof;
		// 近模糊
		if (GetDlgItem(("Edit_FieldDepth1"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_FieldDepth1"));
			float val1 = num.ToFloat();
			a_Clamp(val1, pCamera->GetZFront(), pCamera->GetZBack());
			pParam->dofParam.fNearBlur = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_FieldDepth1"), val);
			pParam->dofParam.fNearBlur = (diff * (float)val) / 100 + pCamera->GetZFront();
		}
		// 近清晰
		if (GetDlgItem(("Edit_FieldDepth2"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_FieldDepth2"));
			float val1 = num.ToFloat();
			a_Clamp(val1, pCamera->GetZFront(), pCamera->GetZBack());
			pParam->dofParam.fNearFocus = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_FieldDepth2"), val);
			pParam->dofParam.fNearFocus = (diff * (float)val) / 100 + pCamera->GetZFront();
		}
		// 远清晰
		if (GetDlgItem(("Edit_FieldDepth3"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_FieldDepth3"));
			float val1 = num.ToFloat();
			a_Clamp(val1, pCamera->GetZFront(), pCamera->GetZBack());
			pParam->dofParam.fFarFocus = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_FieldDepth3"), val);
			pParam->dofParam.fFarFocus = (diff * (float)val) / 100 + pCamera->GetZFront();
		}
		// 远模糊
		if (GetDlgItem(("Edit_FieldDepth4"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_FieldDepth4"));
			float val1 = num.ToFloat();
			a_Clamp(val1, pCamera->GetZFront(), pCamera->GetZBack());
			pParam->dofParam.fFarBlur = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_FieldDepth4"), val);
			pParam->dofParam.fFarBlur = (diff * (float)val) / 100 + pCamera->GetZFront();
		}
		// 最大模糊量
		if (GetDlgItem(("Edit_FieldDepth5"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_FieldDepth5"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 1.0f);
			pParam->dofParam.fClampBlurFar = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_FieldDepth5"), val);
			pParam->dofParam.fClampBlurFar = ((float)val) / 100;
		}
	}
	{
		AWString str;
		CheckItem(("Check_FieldDepth"), (pParam->dwPostEffectMask & A3DPOST_EFFECT_Dof) != 0);
		a_Clamp(pParam->dofParam.fNearBlur, pCamera->GetZFront(), pCamera->GetZBack());

		val = (int)((100 * (pParam->dofParam.fNearBlur - pCamera->GetZFront())) / diff + 0.5f);
		DDX_Slider(false, ("Slider_FieldDepth1"), val);
		str.Format(_AL("%0.4f"), pParam->dofParam.fNearBlur);
		SetItemText(("Edit_FieldDepth1"), str);

		val = (int)((100 * (pParam->dofParam.fNearFocus - pCamera->GetZFront())) / diff + 0.5f);
		DDX_Slider(false, ("Slider_FieldDepth2"), val);
		str.Format(_AL("%0.4f"), pParam->dofParam.fNearFocus);
		SetItemText(("Edit_FieldDepth2"), str);

		val = (int)((100 * (pParam->dofParam.fFarFocus - pCamera->GetZFront())) / diff+ 0.5f);
		DDX_Slider(false, ("Slider_FieldDepth3"), val);
		str.Format(_AL("%0.4f"), pParam->dofParam.fFarFocus);
		SetItemText(("Edit_FieldDepth3"), str);

		val = (int)((100 * (pParam->dofParam.fFarBlur - pCamera->GetZFront())) / diff + 0.5f);
		DDX_Slider(false, ("Slider_FieldDepth4"), val);
		str.Format(_AL("%0.4f"), pParam->dofParam.fFarBlur);
		SetItemText(("Edit_FieldDepth4"), str);

		val = (int)(pParam->dofParam.fClampBlurFar * 100);
		DDX_Slider(false, ("Slider_FieldDepth5"), val);
		str.Format(_AL("%0.4f"), pParam->dofParam.fClampBlurFar);
		SetItemText(("Edit_FieldDepth5"), str);
	}


	// 运动模糊
	if (bSave)
	{
		if (IsItemChecked(("Check_Movement")))
			pParam->dwPostEffectMask |= A3DPOST_EFFECT_MotionBlur;
		else
			pParam->dwPostEffectMask &= ~A3DPOST_EFFECT_MotionBlur;
		// 最大模糊长度
		if (GetDlgItem(("Edit_Movement1"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_Movement1"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 0.1f);
			pParam->motionBlurParam.fBlurMax = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_Movement1"), val);
			pParam->motionBlurParam.fBlurMax = ((float)val) / 1000;
		}
		// 模糊长度
		if (GetFocus() == GetDlgItem(("Edit_Movement2")))
		{
			AWString num = GetItemText(("Edit_Movement2"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 0.1f);
			pParam->motionBlurParam.fBlurScale = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_Movement2"), val);
			pParam->motionBlurParam.fBlurScale = ((float)val) / 1000;
		}
		// 位移模糊权重
		if (GetFocus() == GetDlgItem(("Edit_Movement3")))
		{
			AWString num = GetItemText(("Edit_Movement3"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 10.0f);
			pParam->motionBlurParam.fBlurScalePosition = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_Movement3"), val);
			pParam->motionBlurParam.fBlurScalePosition = ((float)val) / 10;
		}
		// 旋转模糊权重
		if (GetFocus() == GetDlgItem(("Edit_Movement4")))
		{
			AWString num = GetItemText(("Edit_Movement4"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 10.0f);
			pParam->motionBlurParam.fBlurScaleRotation = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_Movement4"), val);
			pParam->motionBlurParam.fBlurScaleRotation = ((float)val) / 10;
		}
		// 近清晰位置
		if (GetFocus() == GetDlgItem(("Edit_Movement5")))
		{
			AWString num = GetItemText(("Edit_Movement5"));
			float val1 = num.ToFloat();
			a_Clamp(val1, pCamera->GetZFront(), pCamera->GetZBack());
			pParam->motionBlurParam.fInFocusDistance = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_Movement5"), val);
			pParam->motionBlurParam.fInFocusDistance = (diff * (float)val) / 100 + pCamera->GetZFront();
		}
		// 远清晰位置
		if (GetFocus() == GetDlgItem(("Edit_Movement6")))
		{
			AWString num = GetItemText(("Edit_Movement6"));
			float val1 = num.ToFloat();
			a_Clamp(val1, pCamera->GetZFront(), pCamera->GetZBack());
			pParam->motionBlurParam.fOutFocusDistance = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_Movement6"), val);
			pParam->motionBlurParam.fOutFocusDistance = (diff * (float)val) / 100 + pCamera->GetZFront();
		}		
	}
	{
		AWString str;
		CheckItem(("Check_Movement"), (pParam->dwPostEffectMask & A3DPOST_EFFECT_MotionBlur) != 0);
		
		val = (int)(pParam->motionBlurParam.fBlurMax * 1000);
		DDX_Slider(false, ("Slider_Movement1"), val);
		str.Format(_AL("%0.4f"), pParam->motionBlurParam.fBlurMax);
		SetItemText(("Edit_Movement1"), str);

		val = (int)(pParam->motionBlurParam.fBlurScale * 1000);
		DDX_Slider(false, ("Slider_Movement2"), val);
		str.Format(_AL("%0.4f"), pParam->motionBlurParam.fBlurScale);
		SetItemText(("Edit_Movement2"), str);

		val = (int)(pParam->motionBlurParam.fBlurScalePosition * 10);
		DDX_Slider(false, ("Slider_Movement3"), val);
		str.Format(_AL("%0.4f"), pParam->motionBlurParam.fBlurScalePosition);
		SetItemText(("Edit_Movement3"), str);

		val = (int)(pParam->motionBlurParam.fBlurScaleRotation * 10);
		DDX_Slider(false, ("Slider_Movement4"), val);
		str.Format(_AL("%0.4f"), pParam->motionBlurParam.fBlurScaleRotation);
		SetItemText(("Edit_Movement4"), str);

		val = (int)((100 * (pParam->motionBlurParam.fInFocusDistance - pCamera->GetZFront())) / diff + 0.5f);
		DDX_Slider(false, ("Slider_Movement5"), val);
		str.Format(_AL("%0.4f"), pParam->motionBlurParam.fInFocusDistance);
		SetItemText(("Edit_Movement5"), str);

		val = (int)((100 * (pParam->motionBlurParam.fOutFocusDistance - pCamera->GetZFront())) / diff + 0.5f);
		DDX_Slider(false, ("Slider_Movement6"), val);
		str.Format(_AL("%0.4f"), pParam->motionBlurParam.fOutFocusDistance);
		SetItemText(("Edit_Movement6"), str);
	}

	// FullGlow
	if (bSave)
	{
		if (IsItemChecked(("Check_FullGlow")))
			pParam->dwPostEffectMask |= A3DPOST_EFFECT_FullGlow;
		else
			pParam->dwPostEffectMask &= ~A3DPOST_EFFECT_FullGlow; 
		// 模糊距离
		if (GetDlgItem(("Edit_FullGlow1"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_FullGlow1"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 0.1f);
			pParam->fullGlowParam.fBlurSize = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_FullGlow1"), val);
			pParam->fullGlowParam.fBlurSize = ((float)val) / 1000;
		}
		// Glow等级
		if (GetDlgItem(("Edit_FullGlow3"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_FullGlow3"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 1.0f);
			pParam->fullGlowParam.fGlowLevel = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_FullGlow3"), val);
			pParam->fullGlowParam.fGlowLevel = ((float)val) / 100;
		}
		// Glow强度
		if (GetDlgItem(("Edit_FullGlow4"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_FullGlow4"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 1.0f);
			pParam->fullGlowParam.fGlowPower = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_FullGlow4"), val);
			pParam->fullGlowParam.fGlowPower = ((float)val) / 100;
		}
		AWString num = GetItemText(("Edit_FullGlowColorR"));
		pParam->fullGlowParam.vGlowColor.x = num.ToFloat();
		num = GetItemText(("Edit_FullGlowColorG"));
		pParam->fullGlowParam.vGlowColor.y = num.ToFloat();
		num = GetItemText(("Edit_FullGlowColorB"));
		pParam->fullGlowParam.vGlowColor.z = num.ToFloat();
	}
	{
		AWString str;
		CheckItem(("Check_FullGlow"), (pParam->dwPostEffectMask & A3DPOST_EFFECT_FullGlow) != 0);

		val = (int)(pParam->fullGlowParam.fBlurSize * 1000);
		DDX_Slider(false, ("Slider_FullGlow1"), val);
		str.Format(_AL("%0.4f"), pParam->fullGlowParam.fBlurSize);
		SetItemText(("Edit_FullGlow1"), str);

		val = (int)(pParam->fullGlowParam.fGlowLevel * 100);
		DDX_Slider(false, ("Slider_FullGlow3"), val);
		str.Format(_AL("%0.4f"), pParam->fullGlowParam.fGlowLevel);
		SetItemText(("Edit_FullGlow3"), str);

		val = (int)(pParam->fullGlowParam.fGlowPower * 100);
		DDX_Slider(false, ("Slider_FullGlow4"), val);
		str.Format(_AL("%0.4f"), pParam->fullGlowParam.fGlowPower);
		SetItemText(("Edit_FullGlow4"), str);

		str.Format(_AL("%0.2f"), pParam->fullGlowParam.vGlowColor.x);
		SetItemText(("Edit_FullGlowColorR"), str);
		str.Format(_AL("%0.2f"), pParam->fullGlowParam.vGlowColor.y);
		SetItemText(("Edit_FullGlowColorG"), str);
		str.Format(_AL("%0.2f"), pParam->fullGlowParam.vGlowColor.z);
		SetItemText(("Edit_FullGlowColorB"), str);
	}

	// 水下扭曲
	if (bSave)
	{
		// 开启
		if (IsItemChecked(("Check_WaterTwist")))
			pParam->dwPostEffectMask |= A3DPOST_EFFECT_Warp;
		else
			pParam->dwPostEffectMask &= ~A3DPOST_EFFECT_Warp;

		// 扭曲程度
		if (GetDlgItem(("Edit_WaterTwist1"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_WaterTwist1"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 0.1f);
			pParam->warpParam.fScale = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_WaterTwist1"), val);
			pParam->warpParam.fScale = ((float)val) / 1000;

		}

		// 扭曲速度
		if (GetDlgItem(("Edit_WaterTwist2"))->IsFocus())
		{
			AWString num = GetItemText(("Edit_WaterTwist2"));
			float val1 = num.ToFloat();
			a_Clamp(val1, 0.0f, 4.0f);
			pParam->warpParam.fSpeed = val1;
		}
		else
		{
			DDX_Slider(bSave, ("Slider_WaterTwist2"), val);
			pParam->warpParam.fSpeed = ((float)val) / 100;
		}		
	}
	{
		CheckItem(("Check_WaterTwist"), (pParam->dwPostEffectMask & A3DPOST_EFFECT_Warp) != 0);
		AWString str;
		val = (int)(pParam->warpParam.fScale * 1000);
		DDX_Slider(false, ("Slider_WaterTwist1"), val);
		str.Format(_AL("%0.4f"), pParam->warpParam.fScale);
		SetItemText(("Edit_WaterTwist1"), str);

		val = (int)(pParam->warpParam.fSpeed * 100);
		DDX_Slider(false, ("Slider_WaterTwist2"), val);
		str.Format(_AL("%0.4f"), pParam->warpParam.fSpeed);
		SetItemText(("Edit_WaterTwist2"), str);
	}	
}



///////////////////////////////////////////////////////////////////////////
//	
//	Msg and Interface Function
//	
///////////////////////////////////////////////////////////////////////////
void CDlgSettingEffect::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int zDelta = (short)HIWORD(wParam);
	if( zDelta > 0 )
		zDelta = -45;
	else
		zDelta = 45;
/*
	PAUISUBDIALOG pDlg = GetParentDlgControl();
	if (NULL != pDlg)
	{
		int idx = pDlg->GetVBarPos() + zDelta;
		a_Clamp(idx, 0, pDlg->GetVBarMax());
		pDlg->SetVBarPos(idx);
	}	
*/}



///////////////////////////////////////////////////////////////////////////
//	
//	Function called by others
//	
///////////////////////////////////////////////////////////////////////////	
/*void CDlgSettingEffect::InitSettingInfo(CDlgSettings* pSettings)
{

}
*/

void CDlgSettingEffect::SetToDefault()
{
	//UpdateView();
}

void CDlgSettingEffect::OnCommandConfirm(const char *szCommand)
{
	UpdateData(true);
}
