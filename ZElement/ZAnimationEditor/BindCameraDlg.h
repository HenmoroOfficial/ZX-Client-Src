#if !defined(AFX_BINDCAMERADLG_H__6140B31F_8B2C_4531_BEE1_5D13E5302A6A__INCLUDED_)
#define AFX_BINDCAMERADLG_H__6140B31F_8B2C_4531_BEE1_5D13E5302A6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BindCameraDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBindCameraDlg dialog

class CBindCameraDlg : public CDialog
{
// Construction
public:
	CBindCameraDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBindCameraDlg)
	enum { IDD = IDD_DLG_BIND_CAMERA };
	float	m_fOffsetX;
	float	m_fOffsetY;
	float	m_fOffsetZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBindCameraDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBindCameraDlg)
	afx_msg void OnButtonBindCameraOk();
	afx_msg void OnButtonBindCameraCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BINDCAMERADLG_H__6140B31F_8B2C_4531_BEE1_5D13E5302A6A__INCLUDED_)
