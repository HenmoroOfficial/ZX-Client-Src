#if !defined(AFX_TREASUREMSKDLG_H__507EC7B7_DF58_478C_BF13_8F451DE2A946__INCLUDED_)
#define AFX_TREASUREMSKDLG_H__507EC7B7_DF58_478C_BF13_8F451DE2A946__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreasureMskDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreasureMskDlg dialog

class CTreasureMskDlg : public CDialog
{
// Construction
public:
	CTreasureMskDlg(CWnd* pParent = NULL);   // standard constructor
	BYTE m_byteMask;
// Dialog Data
	//{{AFX_DATA(CTreasureMskDlg)
	enum { IDD = IDD_DIALOG_TREASURE_MASK };
	//}}AFX_DATA
	BOOL	m_bCheck[8];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreasureMskDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTreasureMskDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnMaskSelAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREASUREMSKDLG_H__507EC7B7_DF58_478C_BF13_8F451DE2A946__INCLUDED_)
