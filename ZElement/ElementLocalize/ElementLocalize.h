// ElementLocalize.h : main header file for the ELEMENTLOCALIZE application
//

#if !defined(AFX_ELEMENTLOCALIZE_H__270418A4_EAE1_4D78_B54E_38B8F57F1042__INCLUDED_)
#define AFX_ELEMENTLOCALIZE_H__270418A4_EAE1_4D78_B54E_38B8F57F1042__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeApp:
// See ElementLocalize.cpp for the implementation of this class
//

class CElementLocalizeApp : public CWinApp
{
public:
	CElementLocalizeApp();
	bool OpenPackage(char *szWorkDir);
	bool ClearPackage(char *szWorkDir);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElementLocalizeApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CElementLocalizeApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMENTLOCALIZE_H__270418A4_EAE1_4D78_B54E_38B8F57F1042__INCLUDED_)
