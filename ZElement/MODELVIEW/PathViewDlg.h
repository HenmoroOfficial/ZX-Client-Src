#if !defined(AFX_PATHVIEWDLG_H__42EE415D_4543_419B_BA2F_746C5EB255BB__INCLUDED_)
#define AFX_PATHVIEWDLG_H__42EE415D_4543_419B_BA2F_746C5EB255BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PathViewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPathViewDlg dialog

class CPathViewDlg : public CDialog
{
// Construction
public:
	CPathViewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPathViewDlg)
	enum { IDD = IDD_DIALOG_PATH_VIEW };
	CString	m_strPaths;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPathViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPathViewDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATHVIEWDLG_H__42EE415D_4543_419B_BA2F_746C5EB255BB__INCLUDED_)
