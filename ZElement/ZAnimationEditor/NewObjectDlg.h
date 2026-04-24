#if !defined(AFX_NEWOBJECTDLG_H__E5A2877E_EFEA_49EC_90FD_00C7BDC019C4__INCLUDED_)
#define AFX_NEWOBJECTDLG_H__E5A2877E_EFEA_49EC_90FD_00C7BDC019C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewObjectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewObjectDlg dialog

class CNewObjectDlg : public CDialog
{
// Construction
public:
	CNewObjectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewObjectDlg)
	enum { IDD = IDD_DLG_NEW_OBJECT };
	CString	m_sObjectFileName;
	CString	m_sObjectType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewObjectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewObjectDlg)
	afx_msg void OnButtonNewObjectImport();
	afx_msg void OnButtonNewObjectOk();
	afx_msg void OnButtonNewObjectCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWOBJECTDLG_H__E5A2877E_EFEA_49EC_90FD_00C7BDC019C4__INCLUDED_)
