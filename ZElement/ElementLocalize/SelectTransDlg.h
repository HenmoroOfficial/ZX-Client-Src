#if !defined(AFX_SELECTTRANSDLG_H__D5897621_70F6_4B49_8519_ED8804B30C67__INCLUDED_)
#define AFX_SELECTTRANSDLG_H__D5897621_70F6_4B49_8519_ED8804B30C67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectTransDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectTransDlg dialog

class CSelectTransDlg : public CDialog
{
// Construction
public:
	CSelectTransDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectTransDlg)
	enum { IDD = IDD_DIALOG_SELECT_TRANS_TEXT };
	CString	m_strOrg;
	CString	m_strTrans1;
	CString	m_strTrans2;
	CString	m_strOrg2;
	//}}AFX_DATA

	bool IsApplyAll(){ return m_bApplyAll; }
	bool IsSelectedOne(){ return m_bSelOne; }
	bool IsReplaceOrg() { return m_bReplaceOrg; }
	void SetData(CString org1, CString org2, CString trans1, CString trans2);
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectTransDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool  m_bApplyAll;
	bool  m_bSelOne;
	bool  m_bReplaceOrg;

	// Generated message map functions
	//{{AFX_MSG(CSelectTransDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioOne();
	afx_msg void OnRadioTwo();
	afx_msg void OnCheckApplyall();
	afx_msg void OnReplaceOrg();

	void UpdateCtrls();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTTRANSDLG_H__D5897621_70F6_4B49_8519_ED8804B30C67__INCLUDED_)
