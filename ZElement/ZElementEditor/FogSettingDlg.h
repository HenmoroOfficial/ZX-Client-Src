#pragma once
#include "NumEdit.h"
#include "SelColorWnd.h"
#include "afxwin.h"
class CSceneBoxArea;

// CFogSettingDlg dialog

class CFogSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CFogSettingDlg)

public:
	CFogSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFogSettingDlg();

	// Dialog Data
	enum { IDD = IDD_FOG };
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg HRESULT OnColorChange(WPARAM wParam, LPARAM lParam);

	static void UpdateDialog();

protected:
	CSelColorWnd	m_ctrlSelColorUp;
	CSelColorWnd	m_ctrlSelColorDown;

	bool IsDay();
	void UpdateControl();
	CSceneBoxArea* GetCurrentBox();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioDay();
	afx_msg void OnBnClickedRadioNight();
	afx_msg void OnEnChangeEditNear();
	afx_msg void OnEnChangeEditFar();
	afx_msg void OnEnChangeEditHeight1();

	void UpdateBox( CSceneBoxArea* pBox );
	afx_msg void OnEnChangeEditHeight2();
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnBnClickedButtonExport();
};
