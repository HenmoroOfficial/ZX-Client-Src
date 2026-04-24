#if !defined(AFX_WORLDDLG_H__9CBF6C6D_099F_4DF9_B29B_DA305B925282__INCLUDED_)
#define AFX_WORLDDLG_H__9CBF6C6D_099F_4DF9_B29B_DA305B925282__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorldDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWorldDlg dialog

class CWorldDlg : public CDialog
{
// Construction
public:
	explicit CWorldDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWorldDlg)
	enum { IDD = IDD_DLG_WORLD };
	int m_iWorldInstance;
    BOOL m_bSpecifyTime;
    int m_iHour;
    int m_iMinute;
    int m_iSecond;
    BOOL m_bSpecifyWeather;
    BOOL m_bRain;
    BOOL m_bSnow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorldDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWorldDlg)
	afx_msg void OnButtonWorldImport();
    afx_msg void OnButtonWorldOk();
	afx_msg void OnButtonWorldCancel();
	afx_msg void OnSpecifyTime();
	afx_msg void OnSpecifyWeather();
	afx_msg void OnRain();
	afx_msg void OnSnow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORLDDLG_H__9CBF6C6D_099F_4DF9_B29B_DA305B925282__INCLUDED_)
