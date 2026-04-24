#if !defined(AFX_INITDIALOGBAR_H__76B0E762_5887_11D2_BBFD_0000C02346E5__INCLUDED_)
#define AFX_INITDIALOGBAR_H__76B0E762_5887_11D2_BBFD_0000C02346E5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// InitDialogBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInitDialogBar dialog

class CInitDialogBar : public CDialogBar
{
// Construction
public:
	CInitDialogBar();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInitDialogBar)
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInitDialogBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	BOOL Create(CWnd * pParentWnd, UINT nIDTemplate, UINT nStyle, UINT	nID);
	BOOL Create(CWnd * pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID);



// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInitDialogBar)
	//}}AFX_MSG
	virtual BOOL OnInitDialogBar();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INITDIALOGBAR_H__76B0E762_5887_11D2_BBFD_0000C02346E5__INCLUDED_)
