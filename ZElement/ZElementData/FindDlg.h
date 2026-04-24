#if !defined(AFX_FINDDLG_H__22C2E157_F548_420F_AAEB_5516C5B7CE21__INCLUDED_)
#define AFX_FINDDLG_H__22C2E157_F548_420F_AAEB_5516C5B7CE21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FindDlg dialog

class FindDlg : public CDialog
{
// Construction
public:
	FindDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(FindDlg)
	enum { IDD = IDD_FIND_DIALOG };
	CString	m_strText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FindDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FindDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDDLG_H__22C2E157_F548_420F_AAEB_5516C5B7CE21__INCLUDED_)
