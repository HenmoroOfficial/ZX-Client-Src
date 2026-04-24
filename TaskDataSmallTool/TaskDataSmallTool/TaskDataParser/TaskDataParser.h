// TaskDataParser.h : main header file for the TASKDATAPARSER DLL
//

#if !defined(AFX_TASKDATAPARSER_H__E135AE81_2C29_4D9E_9049_095F7AA8724E__INCLUDED_)
#define AFX_TASKDATAPARSER_H__E135AE81_2C29_4D9E_9049_095F7AA8724E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTaskDataParserApp
// See TaskDataParser.cpp for the implementation of this class
//

class CTaskDataParserApp : public CWinApp
{
public:
	CTaskDataParserApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskDataParserApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTaskDataParserApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKDATAPARSER_H__E135AE81_2C29_4D9E_9049_095F7AA8724E__INCLUDED_)
