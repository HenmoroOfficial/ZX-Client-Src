#if !defined(AFX_SECTPLAYERDLG_H__9D3F1CBC_88A9_44E5_A275_7C0B18959C3D__INCLUDED_)
#define AFX_SECTPLAYERDLG_H__9D3F1CBC_88A9_44E5_A275_7C0B18959C3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SectPlayerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSectPlayerDlg dialog

class CSectPlayerDlg : public CDialog
{
// Construction
public:
	CSectPlayerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSectPlayerDlg)
	enum { IDD = IDD_DLG_SECTPLAYER };
	CComboBox	m_comboGender;
	CComboBox	m_comboSect;
	BOOL	m_bShowWeapon;
	BOOL	m_bUsePlayer;
	CString m_sPlayerFileNameBack;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSectPlayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSectPlayerDlg)
	afx_msg void OnButtonSectplayerOk();
	afx_msg void OnButtonSectplayerCancel();
	afx_msg void OnButtonSectplayerEdit();
	virtual BOOL OnInitDialog();
	afx_msg void OnUseHostSectplayer();
	afx_msg void OnShowHostSectplayerWeapon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECTPLAYERDLG_H__9D3F1CBC_88A9_44E5_A275_7C0B18959C3D__INCLUDED_)
