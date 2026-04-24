#pragma once
#include "afxwin.h"


// CLightMapSizeDlg dialog

class CLightMapSizeDlg : public CDialog
{
	DECLARE_DYNAMIC(CLightMapSizeDlg)

public:
	CLightMapSizeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightMapSizeDlg();

	// Dialog Data
	enum { IDD = IDD_LIGHTMAPSIZEDLG };

	int m_nSize;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_cmbSize;
	afx_msg void OnBnClickedButtonUnlock();
	CEdit m_edtPassword;
	afx_msg void OnBnClickedOk();
};
