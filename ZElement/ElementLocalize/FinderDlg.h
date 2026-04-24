#if !defined(AFX_FINDERDLG_H__7907D460_5C4C_417D_B38E_C7F2FE4B379F__INCLUDED_)
#define AFX_FINDERDLG_H__7907D460_5C4C_417D_B38E_C7F2FE4B379F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FinderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFinderDlg dialog

enum eFindType
{
	eOrginText, eTransText, eGlobalID, eTempID
};

class CFinderDlg : public CDialog
{
// Construction
public:
	CFinderDlg(CWnd* pParent = NULL);   // standard constructor
	bool m_bUp;
	bool m_bMatchWhole;
	bool m_bOnlyNewItem;
	eFindType  m_nSelType;//0:org;1:trans;2:use id
// Dialog Data
	//{{AFX_DATA(CFinderDlg)
	enum { IDD = IDD_DIALOG_FINDER };
	CString	m_strText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFinderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateButton();

	// Generated message map functions
	//{{AFX_MSG(CFinderDlg)
	afx_msg void OnFindText();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioTrans();
	afx_msg void OnRadioOrg();
	afx_msg void OnRadioUp();
	afx_msg void OnRadioDown();
	afx_msg void OnCheckMatchWhole();
	afx_msg void OnCheckOnlyNewItem();
	afx_msg void OnRadioUseId();
	afx_msg void OnRadioUseTempId();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDERDLG_H__7907D460_5C4C_417D_B38E_C7F2FE4B379F__INCLUDED_)
