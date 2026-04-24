// DlgPostEffect.cpp : implementation file
//

#include "stdafx.h"
#include "ElementEditor.h"
#include "DlgPostEffect.h"
#include "Global.h"
#include "MainFrm.h"
#include "render.h"
#include "AFileDialogWrapper.h"

CDlgPostEffect* g_pPostEffectPanel = NULL;

IMPLEMENT_DYNAMIC(CDlgPostEffect, CDialog)

CDlgPostEffect::CDlgPostEffect(CWnd* pParent /*=NULL*/)
: CDialog(CDlgPostEffect::IDD, pParent)
{
	ASSERT(!g_pPostEffectPanel);
	g_pPostEffectPanel = this;
	int r = (int)(g_PostEffectParam.godRayParam.vBloomTintColor.x * 255);
	int g = (int)(g_PostEffectParam.godRayParam.vBloomTintColor.y * 255);
	int b = (int)(g_PostEffectParam.godRayParam.vBloomTintColor.z * 255);
	m_ctrlSelColorGodRay.SetColor(RGB(r,g,b));
	r = (int)(g_PostEffectParam.fullGlowParam.vGlowColor.x * 255);
	g = (int)(g_PostEffectParam.fullGlowParam.vGlowColor.y * 255);
	b = (int)(g_PostEffectParam.fullGlowParam.vGlowColor.z * 255);
	m_ctrlSelColorFullGlow.SetColor(RGB(r,g,b));
}

CDlgPostEffect::~CDlgPostEffect()
{
	g_pPostEffectPanel = NULL;
}

void CDlgPostEffect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPostEffect)
	DDX_Control(pDX, IDC_STATIC_GODRAY_COLOR, m_ctrlSelColorGodRay);
	DDX_Control(pDX, IDC_STATIC_FULLGLOW_COLOR, m_ctrlSelColorFullGlow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPostEffect, CDialog)
	ON_MESSAGE(WM_SELCOLCHANGED, OnColorChange)
	ON_BN_CLICKED(IDC_CHECK_POSTEFFECT, &CDlgPostEffect::OnBnClickedCheckPosteffect)
	ON_BN_CLICKED(IDC_CHECK_POST_AA, &CDlgPostEffect::OnBnClickedCheckPostAa)
	ON_BN_CLICKED(IDC_CHECK_POST_CC, &CDlgPostEffect::OnBnClickedCheckPostCC)
	ON_BN_CLICKED(IDC_CHECK_POST_DOF, &CDlgPostEffect::OnBnClickedCheckPostDOF)
	ON_BN_CLICKED(IDC_CHECK_POST_GODRAY, &CDlgPostEffect::OnBnClickedCheckPostGodRay)
	ON_BN_CLICKED(IDC_CHECK_POST_BLOOM, &CDlgPostEffect::OnBnClickedCheckPostBloom)
	ON_BN_CLICKED(IDC_CHECK_POST_FULLGLOW, &CDlgPostEffect::OnBnClickedCheckPostFullGlow)
	ON_BN_CLICKED(IDC_CHECK_POST_MOTION_BLUR, &CDlgPostEffect::OnBnClickedCheckPostMotionBlur)
	ON_BN_CLICKED(IDC_CHECK_POST_RADIUS_BLUR, &CDlgPostEffect::OnBnClickedCheckPostRadiusBlur)
	ON_BN_CLICKED(IDC_CHECK_POST_WRAP, &CDlgPostEffect::OnBnClickedCheckPostWrap)
	ON_BN_CLICKED(IDC_CHECK_POST_UNITED_WARP, &CDlgPostEffect::OnBnClickedCheckPostUnitedWarp)
	ON_BN_CLICKED(IDC_CHECK_POST_NIGHT, OnBnClickedCheckPostNight)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_DOF_NEAR_BLUR, &CDlgPostEffect::OnEnChangeEditDofNearBlur)
	ON_EN_CHANGE(IDC_EDIT_DOF_NEAR_FOCUS, &CDlgPostEffect::OnEnChangeEditDofNearFocus)
	ON_EN_CHANGE(IDC_EDIT_DOF_FAR_FOCUS, &CDlgPostEffect::OnEnChangeEditDofFarFocus)
	ON_EN_CHANGE(IDC_EDIT_DOF_FAR_BLUR, &CDlgPostEffect::OnEnChangeEditDofFarBlur)
	ON_EN_CHANGE(IDC_EDIT_RBLUR_CENTERX, &CDlgPostEffect::OnEnChangeEditRBlurCenterX)
	ON_EN_CHANGE(IDC_EDIT_RBLUR_CENTERY, &CDlgPostEffect::OnEnChangeEditRBlurCenterY)
	ON_CONTROL(STN_DBLCLK, IDC_STATIC_DN_BLOOM, OnLabelDBLClickNight)
	ON_CONTROL(STN_DBLCLK, IDC_STATIC_DN_FULLGLOW, OnLabelDBLClickNight)
	ON_EN_CHANGE(IDC_EDIT_POST_WARP_MASK, &CDlgPostEffect::OnEnChangeEditPostWarpMask)
	ON_BN_CLICKED(IDC_CHECK_POST_WARP_MASK, &CDlgPostEffect::OnBnClickedCheckPostWarpMask)
	ON_BN_CLICKED(IDC_BUTTON_POST_WARP_MASK_BROWSE, &CDlgPostEffect::OnBnClickedButtonPostWarpMaskBrowse)
	ON_BN_CLICKED(IDC_CHECK_POST_CC_AFTER_UI, &CDlgPostEffect::OnBnClickedCheckPostCcAfterUi)
	ON_BN_CLICKED(IDC_CHECK_POST_CC_PARAM1, &CDlgPostEffect::OnBnClickedCheckPostCcParam1)
	ON_EN_CHANGE(IDC_EDIT_POST_CC_TEX1, &CDlgPostEffect::OnEnChangeEditPostCcTex1)
	ON_BN_CLICKED(IDC_BUTTON_POST_CC_TEX1_BROWSE, &CDlgPostEffect::OnBnClickedButtonPostCcTex1Browse)
	ON_BN_CLICKED(IDC_CHECK_POST_CC_GRAY1, &CDlgPostEffect::OnBnClickedCheckPostCcGray1)
	ON_BN_CLICKED(IDC_CHECK_POST_CC_OPPO1, &CDlgPostEffect::OnBnClickedCheckPostCcOppo1)
	ON_EN_CHANGE(IDC_EDIT_POST_CC_BRIGHT1, &CDlgPostEffect::OnEnChangeEditPostCcBright1)
	ON_EN_CHANGE(IDC_EDIT_POST_CC_CONTRAST1, &CDlgPostEffect::OnEnChangeEditPostCcContrast1)
	ON_EN_CHANGE(IDC_EDIT_POST_CC_HUE1, &CDlgPostEffect::OnEnChangeEditPostCcHue1)
	ON_EN_CHANGE(IDC_EDIT_POST_CC_SATURATION1, &CDlgPostEffect::OnEnChangeEditPostCcSaturation1)
	ON_EN_CHANGE(IDC_EDIT_POST_CC_FACTOR, &CDlgPostEffect::OnEnChangeEditPostCcFactor)
	ON_BN_CLICKED(IDC_CHECK_POST_CC_PARAM2, &CDlgPostEffect::OnBnClickedCheckPostCcParam2)
	ON_EN_CHANGE(IDC_EDIT_POST_CC_TEX2, &CDlgPostEffect::OnEnChangeEditPostCcTex2)
	ON_BN_CLICKED(IDC_BUTTON_POST_CC_TEX2_BROWSE, &CDlgPostEffect::OnBnClickedButtonPostCcTex2Browse)
	ON_BN_CLICKED(IDC_CHECK_POST_CC_GRAY2, &CDlgPostEffect::OnBnClickedCheckPostCcGray2)
	ON_BN_CLICKED(IDC_CHECK_POST_CC_OPPO2, &CDlgPostEffect::OnBnClickedCheckPostCcOppo2)
	ON_EN_CHANGE(IDC_EDIT_POST_CC_BRIGHT2, &CDlgPostEffect::OnEnChangeEditPostCcBright2)
	ON_EN_CHANGE(IDC_EDIT_POST_CC_CONTRAST2, &CDlgPostEffect::OnEnChangeEditPostCcContrast2)
	ON_EN_CHANGE(IDC_EDIT_POST_CC_HUE2, &CDlgPostEffect::OnEnChangeEditPostCcHue2)
	ON_EN_CHANGE(IDC_EDIT_POST_CC_SATURATION2, &CDlgPostEffect::OnEnChangeEditPostCcSaturation2)
END_MESSAGE_MAP()

BOOL CDlgPostEffect::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString strText;

	//	Change edit boxes to flat style
	AUX_FlatEditBox((CEdit*)GetDlgItem(IDC_EDIT_DOF_FAR_BLUR));
	AUX_FlatEditBox((CEdit*)GetDlgItem(IDC_EDIT_DOF_NEAR_BLUR));
	AUX_FlatEditBox((CEdit*)GetDlgItem(IDC_EDIT_DOF_NEAR_FOCUS));
	AUX_FlatEditBox((CEdit*)GetDlgItem(IDC_EDIT_DOF_FAR_FOCUS));

	CButton *pButton = (CButton *)GetDlgItem(IDC_CHECK_POSTEFFECT);
	if(pButton) pButton->SetCheck(g_Configs.bShowPostEffect);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_POST_AA);
	if(pButton) pButton->SetCheck((g_PostEffectParam.dwPostEffectMask & A3DPOST_EFFECT_AA) != 0);

	//CC
	pButton = (CButton *)GetDlgItem(IDC_CHECK_POST_CC);
	if(pButton) pButton->SetCheck((g_PostEffectParam.dwPostEffectMask & A3DPOST_EFFECT_CC) != 0);
	((CButton*)GetDlgItem(IDC_CHECK_POST_CC_AFTER_UI))->SetCheck(g_PostEffectParam.ccParam.bAfterUI);
	((CButton*)GetDlgItem(IDC_CHECK_POST_CC_PARAM1))->SetCheck(g_PostEffectParam.ccParam.bUseManualParam1);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_TEX1))->SetWindowText(g_PostEffectParam.ccParam.szTex1);
	((CButton*)GetDlgItem(IDC_CHECK_POST_CC_GRAY1))->SetCheck(g_PostEffectParam.ccParam.manualParam1.bGrey);
	((CButton*)GetDlgItem(IDC_CHECK_POST_CC_OPPO1))->SetCheck(g_PostEffectParam.ccParam.manualParam1.bOpposition);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam1.nBright);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_BRIGHT1))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam1.nContrast);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_CONTRAST1))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam1.nHue);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_HUE1))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam1.nSaturation);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_SATURATION1))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.ccParam.fFactor);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_FACTOR))->SetWindowText(strText);
	((CButton*)GetDlgItem(IDC_CHECK_POST_CC_PARAM2))->SetCheck(g_PostEffectParam.ccParam.bUseManualParam2);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_TEX2))->SetWindowText(g_PostEffectParam.ccParam.szTex2);
	((CButton*)GetDlgItem(IDC_CHECK_POST_CC_GRAY2))->SetCheck(g_PostEffectParam.ccParam.manualParam2.bGrey);
	((CButton*)GetDlgItem(IDC_CHECK_POST_CC_OPPO2))->SetCheck(g_PostEffectParam.ccParam.manualParam2.bOpposition);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam2.nBright);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_BRIGHT2))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam2.nContrast);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_CONTRAST2))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam2.nHue);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_HUE2))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam2.nSaturation);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_SATURATION2))->SetWindowText(strText);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_POST_FULLGLOW);
	if(pButton) pButton->SetCheck((g_PostEffectParam.dwPostEffectMask & A3DPOST_EFFECT_FullGlow) != 0);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_POST_BLOOM);
	if(pButton) pButton->SetCheck((g_PostEffectParam.dwPostEffectMask & A3DPOST_EFFECT_Bloom) != 0);
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_BLOOM_BLUR);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.bloomParam.fBlurSize * 1000));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_BLOOM_THREDHOLD);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.bloomParam.fBrightThreshold * 100));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_BLOOM_BRIGHTNESS);
	pSlider->SetRange(0, 50);
	pSlider->SetPos((int)(g_PostEffectParam.bloomParam.fBrightScale * 10));

	pButton = (CButton *)GetDlgItem(IDC_CHECK_POST_DOF);
	if(pButton) pButton->SetCheck((g_PostEffectParam.dwPostEffectMask & A3DPOST_EFFECT_Dof) != 0);
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_DOF_BLUR);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.dofParam.fClampBlurFar * 100));
	strText.Format("%g", g_PostEffectParam.dofParam.fNearBlur);
	((CEdit*)GetDlgItem(IDC_EDIT_DOF_NEAR_BLUR))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.dofParam.fNearFocus);
	((CEdit*)GetDlgItem(IDC_EDIT_DOF_NEAR_FOCUS))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.dofParam.fFarFocus);
	((CEdit*)GetDlgItem(IDC_EDIT_DOF_FAR_FOCUS))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.dofParam.fFarBlur);
	((CEdit*)GetDlgItem(IDC_EDIT_DOF_FAR_BLUR))->SetWindowText(strText);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_POST_GODRAY);
	if(pButton) pButton->SetCheck((g_PostEffectParam.dwPostEffectMask & A3DPOST_EFFECT_GodRay) != 0);
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_GODRAY_INTENSITY);
	pSlider->SetRange(0, 500);
	pSlider->SetPos((int)(g_PostEffectParam.godRayParam.fBloomScale * 100));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_GODRAY_ANGLE);
	pSlider->SetRange(0, 180);
	pSlider->SetPos((int)(g_PostEffectParam.godRayParam.fVisibleAngle / A3D_PI * 180));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_GODRAY_THREDHOLD);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.godRayParam.fBloomThreshold * 100));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_GODRAY_DEPTH_THREDHOLD);
	pSlider->SetRange(0, 2000);
	pSlider->SetPos((int)(g_PostEffectParam.godRayParam.fOcclusionDepthRange));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_GODRAY_DARK_THREDHOLD);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.godRayParam.fScreenBlendThreshold * 100));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_GODRAY_DARKEN);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.godRayParam.fOcclusionDarkness * 100));
	m_ctrlSelColorGodRay.SetNotifyWindow(this);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_POST_RADIUS_BLUR);
	if(pButton) pButton->SetCheck((g_PostEffectParam.dwPostEffectMask & A3DPOST_EFFECT_RadialBlur) != 0);
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_RBLUR_LENGTH);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.radiusBlurParam.fBlurLength * 100));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_RBLUR_IN);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.radiusBlurParam.fInFocusRadius * 100));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_RBLUR_OUT);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.radiusBlurParam.fOutFocusRadius * 100));
	strText.Format("%g", g_PostEffectParam.radiusBlurParam.fBlurCenterX);
	((CEdit*)GetDlgItem(IDC_EDIT_RBLUR_CENTERX))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.radiusBlurParam.fBlurCenterY);
	((CEdit*)GetDlgItem(IDC_EDIT_RBLUR_CENTERY))->SetWindowText(strText);

	m_ctrlSelColorFullGlow.SetNotifyWindow(this);
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_FULLGLOW_RADIUS);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.fullGlowParam.fBlurSize * 1000));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_FULLGLOW_LEVEL);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.fullGlowParam.fGlowLevel * 100));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_FULLGLOW_POWER);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.fullGlowParam.fGlowPower * 100));

	pButton = (CButton *)GetDlgItem(IDC_CHECK_POST_MOTION_BLUR);
	if(pButton) pButton->SetCheck((g_PostEffectParam.dwPostEffectMask & A3DPOST_EFFECT_MotionBlur) != 0);
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_MBLUR_MAX);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.motionBlurParam.fBlurMax * 1000));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_MBLUR_SCALE);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.motionBlurParam.fBlurScale * 1000));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_MBLUR_POS);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.motionBlurParam.fBlurScalePosition * 10));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_MBLUR_ROT);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.motionBlurParam.fBlurScaleRotation * 10));

	pButton = (CButton *)GetDlgItem(IDC_CHECK_POST_WRAP);
	if(pButton) pButton->SetCheck((g_PostEffectParam.dwPostEffectMask & A3DPOST_EFFECT_Warp) != 0);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_POST_UNITED_WARP);
	if(pButton)
	{
		pButton->SetCheck((g_dwPostEffectAdditionalFlags & PostEffectAdditionalFlag_UnitedWarp) != 0);
		OnBnClickedCheckPostUnitedWarp();
	}
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_UNITED_WARP_SCALE);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.warpParam.fScale * 1000));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_UNITED_WARP_SPEED);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.warpParam.fSpeed * 100));
	((CButton *)GetDlgItem(IDC_CHECK_POST_WARP_MASK))->SetCheck(g_PostEffectParam.warpParam.bEnableMask);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_WARP_MASK))->SetWindowText(g_PostEffectParam.warpParam.strMaskTextureName);

	UpdateData(FALSE);

	return TRUE;
}

HRESULT CDlgPostEffect::OnColorChange(WPARAM wParam, LPARAM lParam)
{
	BYTE r, g, b;

	if (wParam == (WPARAM)m_ctrlSelColorGodRay.m_hWnd)
	{
		DWORD dwCol = m_ctrlSelColorGodRay.GetColor();
		r = GetRValue(dwCol);
		g = GetGValue(dwCol);
		b = GetBValue(dwCol);
		g_PostEffectParam.godRayParam.vBloomTintColor.x = r / 255.0f;
		g_PostEffectParam.godRayParam.vBloomTintColor.y = g / 255.0f;
		g_PostEffectParam.godRayParam.vBloomTintColor.z = b / 255.0f;
	}
	else if (wParam == (WPARAM)m_ctrlSelColorFullGlow.m_hWnd)
	{
		DWORD dwCol = m_ctrlSelColorFullGlow.GetColor();
		r = GetRValue(dwCol);
		g = GetGValue(dwCol);
		b = GetBValue(dwCol);
		if(IsNightMode())
		{
			g_PostEffectParamNight.fullGlowParam.vGlowColor.x = r / 255.0f;
			g_PostEffectParamNight.fullGlowParam.vGlowColor.y = g / 255.0f;
			g_PostEffectParamNight.fullGlowParam.vGlowColor.z = b / 255.0f;
		}
		else
		{
			g_PostEffectParam.fullGlowParam.vGlowColor.x = r / 255.0f;
			g_PostEffectParam.fullGlowParam.vGlowColor.y = g / 255.0f;
			g_PostEffectParam.fullGlowParam.vGlowColor.z = b / 255.0f;
		}
	}

	return S_OK;

}
// CDlgPostEffect message handlers



void CDlgPostEffect::OnBnClickedCheckPosteffect()
{
	g_Configs.bShowPostEffect = !g_Configs.bShowPostEffect;
}

void CDlgPostEffect::OnBnClickedCheckPostAa()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_AA))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.dwPostEffectMask = bCheck ? (g_PostEffectParam.dwPostEffectMask | A3DPOST_EFFECT_AA) : (g_PostEffectParam.dwPostEffectMask & ~A3DPOST_EFFECT_AA);
}

void CDlgPostEffect::OnBnClickedCheckPostCC()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_CC))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.dwPostEffectMask = bCheck ? (g_PostEffectParam.dwPostEffectMask | A3DPOST_EFFECT_CC) : (g_PostEffectParam.dwPostEffectMask & ~A3DPOST_EFFECT_CC);
}

void CDlgPostEffect::OnBnClickedCheckPostDOF()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_DOF))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.dwPostEffectMask = bCheck ? (g_PostEffectParam.dwPostEffectMask | A3DPOST_EFFECT_Dof) : (g_PostEffectParam.dwPostEffectMask & ~A3DPOST_EFFECT_Dof);
}

void CDlgPostEffect::OnBnClickedCheckPostGodRay()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_GODRAY))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.dwPostEffectMask = bCheck ? (g_PostEffectParam.dwPostEffectMask | A3DPOST_EFFECT_GodRay) : (g_PostEffectParam.dwPostEffectMask & ~A3DPOST_EFFECT_GodRay);
}

void CDlgPostEffect::OnBnClickedCheckPostBloom()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_BLOOM))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.dwPostEffectMask = bCheck ? (g_PostEffectParam.dwPostEffectMask | A3DPOST_EFFECT_Bloom) : (g_PostEffectParam.dwPostEffectMask & ~A3DPOST_EFFECT_Bloom);
}

void CDlgPostEffect::OnBnClickedCheckPostFullGlow()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_FULLGLOW))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.dwPostEffectMask = bCheck ? (g_PostEffectParam.dwPostEffectMask | A3DPOST_EFFECT_FullGlow) : (g_PostEffectParam.dwPostEffectMask & ~A3DPOST_EFFECT_FullGlow);
}

void CDlgPostEffect::OnBnClickedCheckPostMotionBlur()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_MOTION_BLUR))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.dwPostEffectMask = bCheck ? (g_PostEffectParam.dwPostEffectMask | A3DPOST_EFFECT_MotionBlur) : (g_PostEffectParam.dwPostEffectMask & ~A3DPOST_EFFECT_MotionBlur);
}
void CDlgPostEffect::OnBnClickedCheckPostRadiusBlur()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_RADIUS_BLUR))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.dwPostEffectMask = bCheck ? (g_PostEffectParam.dwPostEffectMask | A3DPOST_EFFECT_RadialBlur) : (g_PostEffectParam.dwPostEffectMask & ~A3DPOST_EFFECT_RadialBlur);
}
void CDlgPostEffect::OnBnClickedCheckPostWrap()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_WRAP))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.dwPostEffectMask = bCheck ? (g_PostEffectParam.dwPostEffectMask | A3DPOST_EFFECT_Warp) : (g_PostEffectParam.dwPostEffectMask & ~A3DPOST_EFFECT_Warp);
}
void CDlgPostEffect::OnBnClickedCheckPostUnitedWarp()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_UNITED_WARP))->GetCheck() == BST_CHECKED;
	g_dwPostEffectAdditionalFlags = bCheck ? (g_dwPostEffectAdditionalFlags | PostEffectAdditionalFlag_UnitedWarp) : (g_dwPostEffectAdditionalFlags & ~PostEffectAdditionalFlag_UnitedWarp);
	((CButton*)GetDlgItem(IDC_CHECK_POST_WRAP))->EnableWindow(!bCheck);
}


void CDlgPostEffect::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CSliderCtrl* pSlider = (CSliderCtrl*) pScrollBar;

	switch (pSlider->GetDlgCtrlID())
	{
	case IDC_SLIDER_DOF_BLUR:
		g_PostEffectParam.dofParam.fClampBlurFar = pSlider->GetPos() / 100.0f;
		return;
	case IDC_SLIDER_GODRAY_INTENSITY:
		g_PostEffectParam.godRayParam.fBloomScale = pSlider->GetPos() / 100.0f;
		return;
	case IDC_SLIDER_GODRAY_ANGLE:
		g_PostEffectParam.godRayParam.fVisibleAngle = pSlider->GetPos() / 180.0f * A3D_PI;
		return;
	case IDC_SLIDER_GODRAY_THREDHOLD:
		g_PostEffectParam.godRayParam.fBloomThreshold = pSlider->GetPos() / 100.0f;
		return;
	case IDC_SLIDER_GODRAY_DEPTH_THREDHOLD:
		g_PostEffectParam.godRayParam.fOcclusionDepthRange = pSlider->GetPos();
		return;
	case IDC_SLIDER_GODRAY_DARK_THREDHOLD:
		g_PostEffectParam.godRayParam.fScreenBlendThreshold = pSlider->GetPos() / 100.0f;
		return;
	case IDC_SLIDER_GODRAY_DARKEN:
		g_PostEffectParam.godRayParam.fOcclusionDarkness = pSlider->GetPos() / 100.0f;
		return;
	case IDC_SLIDER_RBLUR_LENGTH:
		g_PostEffectParam.radiusBlurParam.fBlurLength = pSlider->GetPos() / 100.0f;
		return;
	case IDC_SLIDER_RBLUR_IN:
		g_PostEffectParam.radiusBlurParam.fInFocusRadius = pSlider->GetPos() / 100.0f;
		return;
	case IDC_SLIDER_RBLUR_OUT:
		g_PostEffectParam.radiusBlurParam.fOutFocusRadius = pSlider->GetPos() / 100.0f;
		return;
	case IDC_SLIDER_MBLUR_MAX:
		g_PostEffectParam.motionBlurParam.fBlurMax = pSlider->GetPos() / 1000.0f;
		return;
	case IDC_SLIDER_MBLUR_SCALE:
		g_PostEffectParam.motionBlurParam.fBlurScale = pSlider->GetPos() / 1000.0f;
		return;
	case IDC_SLIDER_MBLUR_POS:
		g_PostEffectParam.motionBlurParam.fBlurScalePosition = pSlider->GetPos() / 10.0f;
		return;
	case IDC_SLIDER_MBLUR_ROT:
		g_PostEffectParam.motionBlurParam.fBlurScaleRotation = pSlider->GetPos() / 10.0f;
		return;
/*	case IDC_SLIDER_WRAP_SCALE:
		g_PostEffectParam.warpParam.fScale = pSlider->GetPos() / 1000.0f;
		return;
	case IDC_SLIDER_WRAP_SPEED:
		g_PostEffectParam.warpParam.fSpeed = pSlider->GetPos() / 100.0f;
		return;
*/	case IDC_SLIDER_UNITED_WARP_SCALE:
		g_PostEffectParam.warpParam.fScale = pSlider->GetPos() / 1000.0f;
		return;
	case IDC_SLIDER_UNITED_WARP_SPEED:
		g_PostEffectParam.warpParam.fSpeed = pSlider->GetPos() / 100.0f;
		return;
	}
	if(IsNightMode())
	{
		switch (pSlider->GetDlgCtrlID())
		{
		case IDC_SLIDER_BLOOM_BLUR:
			g_PostEffectParamNight.bloomParam.fBlurSize = pSlider->GetPos() / 1000.0f;
			return;
		case IDC_SLIDER_BLOOM_THREDHOLD:
			g_PostEffectParamNight.bloomParam.fBrightThreshold = pSlider->GetPos() / 100.0f;
			return;
		case IDC_SLIDER_BLOOM_BRIGHTNESS:
			g_PostEffectParamNight.bloomParam.fBrightScale = pSlider->GetPos() / 10.0f;
			return;
		case IDC_SLIDER_FULLGLOW_RADIUS:
			g_PostEffectParamNight.fullGlowParam.fBlurSize = pSlider->GetPos() / 1000.0f;
			return;
		case IDC_SLIDER_FULLGLOW_LEVEL:
			g_PostEffectParamNight.fullGlowParam.fGlowLevel = pSlider->GetPos() / 100.0f;
			return;
		case IDC_SLIDER_FULLGLOW_POWER:
			g_PostEffectParamNight.fullGlowParam.fGlowPower = pSlider->GetPos() / 100.0f;
			return;
		}
	}
	else
	{
		switch (pSlider->GetDlgCtrlID())
		{
		case IDC_SLIDER_BLOOM_BLUR:
			g_PostEffectParam.bloomParam.fBlurSize = pSlider->GetPos() / 1000.0f;
			return;
		case IDC_SLIDER_BLOOM_THREDHOLD:
			g_PostEffectParam.bloomParam.fBrightThreshold = pSlider->GetPos() / 100.0f;
			return;
		case IDC_SLIDER_BLOOM_BRIGHTNESS:
			g_PostEffectParam.bloomParam.fBrightScale = pSlider->GetPos() / 10.0f;
			return;
		case IDC_SLIDER_FULLGLOW_RADIUS:
			g_PostEffectParam.fullGlowParam.fBlurSize = pSlider->GetPos() / 1000.0f;
			return;
		case IDC_SLIDER_FULLGLOW_LEVEL:
			g_PostEffectParam.fullGlowParam.fGlowLevel = pSlider->GetPos() / 100.0f;
			return;
		case IDC_SLIDER_FULLGLOW_POWER:
			g_PostEffectParam.fullGlowParam.fGlowPower = pSlider->GetPos() / 100.0f;
			return;
		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDlgPostEffect::OnEnChangeEditDofNearBlur()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_DOF_NEAR_BLUR))->GetWindowText(strText);
	g_PostEffectParam.dofParam.fNearBlur = (float) atof(strText);
}

void CDlgPostEffect::OnEnChangeEditDofNearFocus()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_DOF_NEAR_FOCUS))->GetWindowText(strText);
	g_PostEffectParam.dofParam.fNearFocus = (float) atof(strText);
}

void CDlgPostEffect::OnEnChangeEditDofFarBlur()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_DOF_FAR_BLUR))->GetWindowText(strText);
	g_PostEffectParam.dofParam.fFarBlur = (float) atof(strText);
}

void CDlgPostEffect::OnEnChangeEditDofFarFocus()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_DOF_FAR_FOCUS))->GetWindowText(strText);
	g_PostEffectParam.dofParam.fFarFocus = (float) atof(strText);
}

void CDlgPostEffect::OnEnChangeEditRBlurCenterX()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_RBLUR_CENTERX))->GetWindowText(strText);
	g_PostEffectParam.radiusBlurParam.fBlurCenterX = (float) atof(strText);
}

void CDlgPostEffect::OnEnChangeEditRBlurCenterY()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_RBLUR_CENTERY))->GetWindowText(strText);
	g_PostEffectParam.radiusBlurParam.fBlurCenterY = (float) atof(strText);
}

void CDlgPostEffect::OnBnClickedCheckPostNight()
{
	SwitchDayNight(((CButton*)GetDlgItem(IDC_CHECK_POST_NIGHT))->GetCheck());
}

void CDlgPostEffect::OnLabelDBLClickNight()
{
	SwitchDayNight(!IsNightMode());
}

void CDlgPostEffect::SwitchDayNight(bool bIsNight)
{
	if(bIsNight)
	{
		((CButton*)GetDlgItem(IDC_CHECK_POST_NIGHT))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_STATIC_DN_CC)->SetWindowText("<Ò¹¼ä>");
		GetDlgItem(IDC_STATIC_DN_BLOOM)->SetWindowText("<Ò¹¼ä>");
		GetDlgItem(IDC_STATIC_DN_FULLGLOW)->SetWindowText("<Ò¹¼ä>");
		// Bloom
		CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_BLOOM_BLUR);
		pSlider->SetRange(0, 100);
		pSlider->SetPos((int)(g_PostEffectParamNight.bloomParam.fBlurSize * 1000));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_BLOOM_THREDHOLD);
		pSlider->SetRange(0, 100);
		pSlider->SetPos((int)(g_PostEffectParamNight.bloomParam.fBrightThreshold * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_BLOOM_BRIGHTNESS);
		pSlider->SetRange(0, 50);
		pSlider->SetPos((int)(g_PostEffectParamNight.bloomParam.fBrightScale * 10));
		// FullGlow
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_FULLGLOW_RADIUS);
		pSlider->SetRange(0, 100);
		pSlider->SetPos((int)(g_PostEffectParamNight.fullGlowParam.fBlurSize * 1000));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_FULLGLOW_LEVEL);
		pSlider->SetRange(0, 100);
		pSlider->SetPos((int)(g_PostEffectParamNight.fullGlowParam.fGlowLevel * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_FULLGLOW_POWER);
		pSlider->SetRange(0, 100);
		pSlider->SetPos((int)(g_PostEffectParamNight.fullGlowParam.fGlowPower * 100));
		int r = (int)(g_PostEffectParamNight.fullGlowParam.vGlowColor.x * 255);
		int g = (int)(g_PostEffectParamNight.fullGlowParam.vGlowColor.y * 255);
		int b = (int)(g_PostEffectParamNight.fullGlowParam.vGlowColor.z * 255);
		m_ctrlSelColorFullGlow.SetColor(RGB(r,g,b));
		((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_TEX1))->SetWindowText(g_PostEffectParamNight.ccParam.szTex1);
		((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_TEX2))->SetWindowText(g_PostEffectParamNight.ccParam.szTex2);
		CString strText;
		strText.Format("%g", g_PostEffectParamNight.ccParam.fFactor);
		((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_FACTOR))->SetWindowText(strText);
		((CButton*)GetDlgItem(IDC_CHECK_POST_CC_AFTER_UI))->SetCheck(g_PostEffectParamNight.ccParam.bAfterUI);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_POST_NIGHT))->SetCheck(BST_UNCHECKED);
		GetDlgItem(IDC_STATIC_DN_CC)->SetWindowText("<°×Öç>");
		GetDlgItem(IDC_STATIC_DN_BLOOM)->SetWindowText("<°×Öç>");
		GetDlgItem(IDC_STATIC_DN_FULLGLOW)->SetWindowText("<°×Öç>");
		// Bloom
		CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_BLOOM_BLUR);
		pSlider->SetRange(0, 100);
		pSlider->SetPos((int)(g_PostEffectParam.bloomParam.fBlurSize * 1000));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_BLOOM_THREDHOLD);
		pSlider->SetRange(0, 100);
		pSlider->SetPos((int)(g_PostEffectParam.bloomParam.fBrightThreshold * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_BLOOM_BRIGHTNESS);
		pSlider->SetRange(0, 50);
		pSlider->SetPos((int)(g_PostEffectParam.bloomParam.fBrightScale * 10));
		// FullGlow
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_FULLGLOW_RADIUS);
		pSlider->SetRange(0, 100);
		pSlider->SetPos((int)(g_PostEffectParam.fullGlowParam.fBlurSize * 1000));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_FULLGLOW_LEVEL);
		pSlider->SetRange(0, 100);
		pSlider->SetPos((int)(g_PostEffectParam.fullGlowParam.fGlowLevel * 100));
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_FULLGLOW_POWER);
		pSlider->SetRange(0, 100);
		pSlider->SetPos((int)(g_PostEffectParam.fullGlowParam.fGlowPower * 100));
		int r = (int)(g_PostEffectParam.fullGlowParam.vGlowColor.x * 255);
		int g = (int)(g_PostEffectParam.fullGlowParam.vGlowColor.y * 255);
		int b = (int)(g_PostEffectParam.fullGlowParam.vGlowColor.z * 255);
		m_ctrlSelColorFullGlow.SetColor(RGB(r,g,b));
		((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_TEX1))->SetWindowText(g_PostEffectParam.ccParam.szTex1);
		((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_TEX2))->SetWindowText(g_PostEffectParam.ccParam.szTex2);
		CString strText;
		strText.Format("%g", g_PostEffectParam.ccParam.fFactor);
		((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_FACTOR))->SetWindowText(strText);
		((CButton*)GetDlgItem(IDC_CHECK_POST_CC_AFTER_UI))->SetCheck(g_PostEffectParam.ccParam.bAfterUI);
	}
	AUX_GetMainFrame()->SwitchDayNight(bIsNight);
}

bool CDlgPostEffect::IsNightMode()
{
	return ((CButton*)g_pPostEffectPanel->GetDlgItem(IDC_CHECK_POST_NIGHT))->GetCheck();
}

void CDlgPostEffect::RefreshUIDataProjSpec()
{
	// ¾°Éî
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_DOF_BLUR);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.dofParam.fClampBlurFar * 100));
	CString strText;
	strText.Format("%g", g_PostEffectParam.dofParam.fNearBlur);
	((CEdit*)GetDlgItem(IDC_EDIT_DOF_NEAR_BLUR))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.dofParam.fNearFocus);
	((CEdit*)GetDlgItem(IDC_EDIT_DOF_NEAR_FOCUS))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.dofParam.fFarFocus);
	((CEdit*)GetDlgItem(IDC_EDIT_DOF_FAR_FOCUS))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.dofParam.fFarBlur);
	((CEdit*)GetDlgItem(IDC_EDIT_DOF_FAR_BLUR))->SetWindowText(strText);

	// È«ÆÁÅ¤Çú
	((CButton*)GetDlgItem(IDC_CHECK_POST_UNITED_WARP))->SetCheck((g_dwPostEffectAdditionalFlags & PostEffectAdditionalFlag_UnitedWarp) != 0);
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_UNITED_WARP_SCALE);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.warpParam.fScale * 1000));
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_UNITED_WARP_SPEED);
	pSlider->SetRange(0, 100);
	pSlider->SetPos((int)(g_PostEffectParam.warpParam.fSpeed * 100));
	((CButton *)GetDlgItem(IDC_CHECK_POST_WARP_MASK))->SetCheck(g_PostEffectParam.warpParam.bEnableMask);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_WARP_MASK))->SetWindowText(g_PostEffectParam.warpParam.strMaskTextureName);
	OnBnClickedCheckPostUnitedWarp();

	// CC
/*	//((CButton*)GetDlgItem(IDC_CHECK_POST_CC_AFTER_UI))->SetCheck(g_PostEffectParam.ccParam.bAfterUI);
	((CButton*)GetDlgItem(IDC_CHECK_POST_CC_PARAM1))->SetCheck(g_PostEffectParam.ccParam.bUseManualParam1);
	//((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_TEX1))->SetWindowText(g_PostEffectParam.ccParam.szTex1);
	((CButton*)GetDlgItem(IDC_CHECK_POST_CC_GRAY1))->SetCheck(g_PostEffectParam.ccParam.manualParam1.bGrey);
	((CButton*)GetDlgItem(IDC_CHECK_POST_CC_OPPO1))->SetCheck(g_PostEffectParam.ccParam.manualParam1.bOpposition);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam1.nBright);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_BRIGHT1))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam1.nContrast);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_CONTRAST1))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam1.nHue);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_HUE1))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam1.nSaturation);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_SATURATION1))->SetWindowText(strText);
	//strText.Format("%g", g_PostEffectParam.ccParam.fFactor);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_FACTOR))->SetWindowText(strText);
	((CButton*)GetDlgItem(IDC_CHECK_POST_CC_PARAM2))->SetCheck(g_PostEffectParam.ccParam.bUseManualParam2);
	//((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_TEX2))->SetWindowText(g_PostEffectParam.ccParam.szTex2);
	((CButton*)GetDlgItem(IDC_CHECK_POST_CC_GRAY2))->SetCheck(g_PostEffectParam.ccParam.manualParam2.bGrey);
	((CButton*)GetDlgItem(IDC_CHECK_POST_CC_OPPO2))->SetCheck(g_PostEffectParam.ccParam.manualParam2.bOpposition);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam2.nBright);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_BRIGHT2))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam2.nContrast);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_CONTRAST2))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam2.nHue);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_HUE2))->SetWindowText(strText);
	strText.Format("%g", g_PostEffectParam.ccParam.manualParam2.nSaturation);
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_SATURATION2))->SetWindowText(strText);
*/
}

void CDlgPostEffect::RefreshUIData()
{
	g_pPostEffectPanel->RefreshUIDataProjSpec();
	g_pPostEffectPanel->OnBnClickedCheckPostNight();
}

void CDlgPostEffect::OnEnChangeEditPostWarpMask()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_POST_WARP_MASK))->GetWindowText(strText);
	g_PostEffectParam.warpParam.strMaskTextureName = strText;
}

void CDlgPostEffect::OnBnClickedCheckPostWarpMask()
{
	g_PostEffectParam.warpParam.bEnableMask = ((CButton*)GetDlgItem(IDC_CHECK_POST_WARP_MASK))->GetCheck();
}

void CDlgPostEffect::OnBnClickedButtonPostWarpMaskBrowse()
{
	AFileDialogWrapper FileDlg(g_Render.GetA3DEngine()->GetA3DDevice(), true, "", "Ñ¡ÔñÎÆÀí", "*.dds", 0);

	if (FileDlg.DoModal() != IDOK)
		return; 

	AString strName = FileDlg.GetRelativePathName();
	((CEdit*)GetDlgItem(IDC_EDIT_POST_WARP_MASK))->SetWindowText(strName);
}

void CDlgPostEffect::OnBnClickedCheckPostCcAfterUi()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_CC_AFTER_UI))->GetCheck() == BST_CHECKED;
	if(IsNightMode())
	{
		g_PostEffectParamNight.ccParam.bAfterUI = bCheck;
	}
	else
		g_PostEffectParam.ccParam.bAfterUI = bCheck;
}

void CDlgPostEffect::OnBnClickedCheckPostCcParam1()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_CC_PARAM1))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.ccParam.bUseManualParam1 = bCheck;
}

void CDlgPostEffect::OnEnChangeEditPostCcTex1()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_TEX1))->GetWindowText(strText);
	if(IsNightMode())
	{
		g_PostEffectParamNight.ccParam.szTex1 = strText;
	}
	else
		g_PostEffectParam.ccParam.szTex1 = strText;
}

void CDlgPostEffect::OnBnClickedButtonPostCcTex1Browse()
{
	AFileDialogWrapper FileDlg(g_Render.GetA3DEngine()->GetA3DDevice(), true, "", "Ñ¡ÔñÎÆÀí", "*.dds", 0);

	if (FileDlg.DoModal() != IDOK)
		return; 

	AString strName = FileDlg.GetRelativePathName();
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_TEX1))->SetWindowText(strName);
}

void CDlgPostEffect::OnBnClickedCheckPostCcGray1()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_CC_GRAY1))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.ccParam.manualParam1.bGrey = bCheck;
}

void CDlgPostEffect::OnBnClickedCheckPostCcOppo1()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_CC_OPPO1))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.ccParam.manualParam1.bOpposition = bCheck;
}

void CDlgPostEffect::OnEnChangeEditPostCcBright1()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_BRIGHT1))->GetWindowText(strText);
	g_PostEffectParam.ccParam.manualParam1.nBright = atoi(strText);
}

void CDlgPostEffect::OnEnChangeEditPostCcContrast1()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_CONTRAST1))->GetWindowText(strText);
	g_PostEffectParam.ccParam.manualParam1.nContrast = atoi(strText);
}

void CDlgPostEffect::OnEnChangeEditPostCcHue1()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_HUE1))->GetWindowText(strText);
	g_PostEffectParam.ccParam.manualParam1.nHue = atoi(strText);
}

void CDlgPostEffect::OnEnChangeEditPostCcSaturation1()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_SATURATION1))->GetWindowText(strText);
	g_PostEffectParam.ccParam.manualParam1.nSaturation = atoi(strText);
}

void CDlgPostEffect::OnEnChangeEditPostCcFactor()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_FACTOR))->GetWindowText(strText);
	if(IsNightMode())
	{
		g_PostEffectParamNight.ccParam.fFactor = (float)atof(strText);
	}
	else
		g_PostEffectParam.ccParam.fFactor = (float)atof(strText);
}

void CDlgPostEffect::OnBnClickedCheckPostCcParam2()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_CC_PARAM2))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.ccParam.bUseManualParam2 = bCheck;
}

void CDlgPostEffect::OnEnChangeEditPostCcTex2()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_TEX2))->GetWindowText(strText);
	if(IsNightMode())
		g_PostEffectParamNight.ccParam.szTex2 = strText;
	else
		g_PostEffectParam.ccParam.szTex2 = strText;
}

void CDlgPostEffect::OnBnClickedButtonPostCcTex2Browse()
{
	AFileDialogWrapper FileDlg(g_Render.GetA3DEngine()->GetA3DDevice(), true, "", "Ñ¡ÔñÎÆÀí", "*.dds", 0);

	if (FileDlg.DoModal() != IDOK)
		return; 

	AString strName = FileDlg.GetRelativePathName();
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_TEX2))->SetWindowText(strName);
}

void CDlgPostEffect::OnBnClickedCheckPostCcGray2()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_CC_GRAY2))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.ccParam.manualParam2.bGrey = bCheck;
}

void CDlgPostEffect::OnBnClickedCheckPostCcOppo2()
{
	bool bCheck = ((CButton*)GetDlgItem(IDC_CHECK_POST_CC_OPPO2))->GetCheck() == BST_CHECKED;
	g_PostEffectParam.ccParam.manualParam2.bOpposition = bCheck;
}

void CDlgPostEffect::OnEnChangeEditPostCcBright2()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_BRIGHT2))->GetWindowText(strText);
	g_PostEffectParam.ccParam.manualParam2.nBright = atoi(strText);
}

void CDlgPostEffect::OnEnChangeEditPostCcContrast2()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_CONTRAST2))->GetWindowText(strText);
	g_PostEffectParam.ccParam.manualParam2.nContrast = atoi(strText);
}

void CDlgPostEffect::OnEnChangeEditPostCcHue2()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_HUE2))->GetWindowText(strText);
	g_PostEffectParam.ccParam.manualParam2.nHue = atoi(strText);
}

void CDlgPostEffect::OnEnChangeEditPostCcSaturation2()
{
	CString strText;
	((CEdit*)GetDlgItem(IDC_EDIT_POST_CC_SATURATION2))->GetWindowText(strText);
	g_PostEffectParam.ccParam.manualParam2.nSaturation = atoi(strText);
}
