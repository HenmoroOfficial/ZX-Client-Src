#if !defined(AFX_UPDATELOGDLG_H__EF972FBD_44D6_47BC_AFC6_B72846BFB18D__INCLUDED_)
#define AFX_UPDATELOGDLG_H__EF972FBD_44D6_47BC_AFC6_B72846BFB18D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UpdateLogDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// UpdateLogDlg dialog

class UpdateLogDlg : public CDialog
{
// Construction
public:
	UpdateLogDlg(CWnd* pParent = NULL);   // standard constructor
	void UpdateLog();
// Dialog Data
	//{{AFX_DATA(UpdateLogDlg)
	enum { IDD = IDD_DIALOG_UPDATELOG };
	CString	m_strUpdateLog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(UpdateLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(UpdateLogDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPDATELOGDLG_H__EF972FBD_44D6_47BC_AFC6_B72846BFB18D__INCLUDED_)
