#if !defined(AFX_INPUTID_H__690E2E7D_B240_4292_940A_74A36F3C243A__INCLUDED_)
#define AFX_INPUTID_H__690E2E7D_B240_4292_940A_74A36F3C243A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputID.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputID dialog

class CInputID : public CDialog
{
// Construction
public:
	CInputID(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputID)
	enum { IDD = IDD_INPUT_ID };
	int		m_nGlobalID;
	//}}AFX_DATA

	int	m_nID;

	bool m_bShowGlobalID;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputID)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputID)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTID_H__690E2E7D_B240_4292_940A_74A36F3C243A__INCLUDED_)
