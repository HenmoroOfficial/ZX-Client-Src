#if !defined(AFX_LISTBAR_H__A732BC21_1A29_49CC_AB05_43E66D617338__INCLUDED_)
#define AFX_LISTBAR_H__A732BC21_1A29_49CC_AB05_43E66D617338__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListBar.h : header file
//

#include "ListDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CListBar dialog

class CListBar : public CDialogBar
{
// Construction
public:
	CListBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CListBar)
	enum { IDD = IDD_DIALOG_LIST_BAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CListDlg m_ListDlg;
	// Generated message map functions
	//{{AFX_MSG(CListBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTBAR_H__A732BC21_1A29_49CC_AB05_43E66D617338__INCLUDED_)
