#if !defined(AFX_PLAYERDLG_H__77952C83_855A_49B2_AE55_6D88DDFC03B1__INCLUDED_)
#define AFX_PLAYERDLG_H__77952C83_855A_49B2_AE55_6D88DDFC03B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg dialog

class CPlayerDlg : public CDialog
{
// Construction
public:
	CPlayerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlayerDlg)
	enum { IDD = IDD_DLG_PLAYER };
	CString	m_sPlayerFileName;
    CString	m_sPlayerFileNameBack;
	BOOL	m_bUseHostPlayer;
	BOOL	m_bShowHostPlayerWeapon;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayerDlg)
    afx_msg void OnButtonPlayerImport();
    afx_msg void OnButtonPlayerOk();
	afx_msg void OnButtonPlayerCancel();
	afx_msg void OnUseHostPlayer();
	afx_msg void OnShowHostPlayerWeapon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERDLG_H__77952C83_855A_49B2_AE55_6D88DDFC03B1__INCLUDED_)
