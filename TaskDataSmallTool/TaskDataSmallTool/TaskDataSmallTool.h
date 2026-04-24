// TaskDataSmallTool.h : main header file for the TASKDATASMALLTOOL application
//

#if !defined(AFX_TASKDATASMALLTOOL_H__2CB0F01D_EFB3_4FC5_BC15_AF9922541F2B__INCLUDED_)
#define AFX_TASKDATASMALLTOOL_H__2CB0F01D_EFB3_4FC5_BC15_AF9922541F2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTaskDataSmallToolApp:
// See TaskDataSmallTool.cpp for the implementation of this class
//

class CTaskDataSmallToolApp : public CWinApp
{
public:
	CTaskDataSmallToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskDataSmallToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTaskDataSmallToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKDATASMALLTOOL_H__2CB0F01D_EFB3_4FC5_BC15_AF9922541F2B__INCLUDED_)
