#if !defined(AFX_DLGPROGRESS_H__7DA17985_BDD8_4A28_90B0_9C4159B437A7__INCLUDED_)
#define AFX_DLGPROGRESS_H__7DA17985_BDD8_4A28_90B0_9C4159B437A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog
#include "common.h"
#define MAX_MSG 512

class CDlgProgress : public CDialog
{
// Construction
public:
	CDlgProgress(THD_INFO* pInfo, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProgress)
	enum { IDD = IDD_DIALOG_PROG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:
	int  iPercent;
	CProgressCtrl* pProg;
	TCHAR szMsg[MAX_MSG];

	THD_INFO* m_pThdInfo;

	// Generated message map functions
	//{{AFX_MSG(CDlgProgress)
		// NOTE: the ClassWizard will add member functions here
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROGRESS_H__7DA17985_BDD8_4A28_90B0_9C4159B437A7__INCLUDED_)
