#pragma once

#include "NumEdit.h"
#include "SelColorWnd.h"
#include "afxwin.h"

class CDlgPostEffect : public CDialog
{
	DECLARE_DYNAMIC(CDlgPostEffect)

public:
	CDlgPostEffect(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPostEffect();

	// Dialog Data
	enum { IDD = IDD_DIALOG_POSTEFFECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg HRESULT OnColorChange(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	CSelColorWnd	m_ctrlSelColorGodRay;
	CSelColorWnd	m_ctrlSelColorFullGlow;
public:
	afx_msg void OnBnClickedCheckPosteffect();
	afx_msg void OnBnClickedCheckPostAa();
	afx_msg void OnBnClickedCheckPostBloom();
	afx_msg void OnBnClickedCheckPostCC();
	afx_msg void OnBnClickedCheckPostDOF();
	afx_msg void OnBnClickedCheckPostGodRay();
	afx_msg void OnBnClickedCheckPostFullGlow();
	afx_msg void OnBnClickedCheckPostMotionBlur();
	afx_msg void OnBnClickedCheckPostRadiusBlur();
	afx_msg void OnBnClickedCheckPostWrap();
	afx_msg void OnBnClickedCheckPostUnitedWarp();
	afx_msg void OnBnClickedCheckPostNight();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditDofNearBlur();
	afx_msg void OnEnChangeEditDofNearFocus();
	afx_msg void OnEnChangeEditDofFarBlur();
	afx_msg void OnEnChangeEditDofFarFocus();
	afx_msg void OnEnChangeEditRBlurCenterX();
	afx_msg void OnEnChangeEditRBlurCenterY();
	afx_msg void OnLabelDBLClickNight();

public:
	static bool IsNightMode();
	static void RefreshUIData();

protected:
	void SwitchDayNight(bool bIsNight);
	void RefreshUIDataProjSpec();
public:
	afx_msg void OnEnChangeEditPostWarpMask();
	afx_msg void OnBnClickedCheckPostWarpMask();
	afx_msg void OnBnClickedButtonPostWarpMaskBrowse();
	afx_msg void OnBnClickedCheckPostCcAfterUi();
	afx_msg void OnBnClickedCheckPostCcParam1();
	afx_msg void OnEnChangeEditPostCcTex1();
	afx_msg void OnBnClickedButtonPostCcTex1Browse();
	afx_msg void OnBnClickedCheckPostCcGray1();
	afx_msg void OnBnClickedCheckPostCcOppo1();
	afx_msg void OnEnChangeEditPostCcBright1();
	afx_msg void OnEnChangeEditPostCcContrast1();
	afx_msg void OnEnChangeEditPostCcHue1();
	afx_msg void OnEnChangeEditPostCcSaturation1();
	afx_msg void OnEnChangeEditPostCcFactor();
	afx_msg void OnBnClickedCheckPostCcParam2();
	afx_msg void OnEnChangeEditPostCcTex2();
	afx_msg void OnBnClickedButtonPostCcTex2Browse();
	afx_msg void OnBnClickedCheckPostCcGray2();
	afx_msg void OnBnClickedCheckPostCcOppo2();
	afx_msg void OnEnChangeEditPostCcBright2();
	afx_msg void OnEnChangeEditPostCcContrast2();
	afx_msg void OnEnChangeEditPostCcHue2();
	afx_msg void OnEnChangeEditPostCcSaturation2();
};
