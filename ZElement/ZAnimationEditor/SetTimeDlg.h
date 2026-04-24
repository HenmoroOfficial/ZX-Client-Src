#if !defined(AFX_SETTIMEDLG_H__50A5514D_E806_474D_A505_391A7C9CB9AE__INCLUDED_)
#define AFX_SETTIMEDLG_H__50A5514D_E806_474D_A505_391A7C9CB9AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTimeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetTimeDlg dialog

class CSetTimeDlg : public CDialog
{
// Construction
public:
	CSetTimeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetTimeDlg)
	enum { IDD = IDD_DLG_SET_TIME };
	DWORD	m_dwTotalTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetTimeDlg)
	afx_msg void OnButtonSetTimeOk();
	afx_msg void OnButtonSetTimeCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTIMEDLG_H__50A5514D_E806_474D_A505_391A7C9CB9AE__INCLUDED_)
