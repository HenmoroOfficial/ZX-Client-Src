// TaskDataSmallToolDlg.h : header file
//

#if !defined(AFX_TASKDATASMALLTOOLDLG_H__15FF170F_7560_4FD2_9FAE_1CF8C4EDB28A__INCLUDED_)
#define AFX_TASKDATASMALLTOOLDLG_H__15FF170F_7560_4FD2_9FAE_1CF8C4EDB28A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ATaskTemplMan;
/////////////////////////////////////////////////////////////////////////////
// CTaskDataSmallToolDlg dialog

class CTaskDataSmallToolDlg : public CDialog
{
// Construction
public:
	CTaskDataSmallToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTaskDataSmallToolDlg)
	enum { IDD = IDD_TASKDATASMALLTOOL_DIALOG };
	CString	m_strResult;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskDataSmallToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	//-------------------------------------------
	ATaskTemplMan* m_pTaskTemplMan;
	//-------------------------------------------

	// Generated message map functions
	//{{AFX_MSG(CTaskDataSmallToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonReadData1();
	afx_msg void OnButtonAnalyze();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKDATASMALLTOOLDLG_H__15FF170F_7560_4FD2_9FAE_1CF8C4EDB28A__INCLUDED_)
