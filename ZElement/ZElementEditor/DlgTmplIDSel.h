#pragma once
#include "afxwin.h"


// CDlgTmplIDSel dialog

class CDlgTmplIDSel : public CDialog
{
	DECLARE_DYNAMIC(CDlgTmplIDSel)

public:
	enum
	{
		TMPL_REGION_BLOCK_ESSENCE,
		TMPL_NUM,
	};

public:
	CDlgTmplIDSel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTmplIDSel();

	DWORD m_dwSelId;
	CString m_strName;

// Dialog Data
	enum { IDD = IDD_TMPL_ID_SEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboType();

	CComboBox m_comboType;
	CComboBox m_comboList;
	virtual BOOL OnInitDialog();

};
