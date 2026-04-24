#if !defined(AFX_BEZIERDLG_H__727583E9_EA52_43DF_A4B4_AC3D85F0E60E__INCLUDED_)
#define AFX_BEZIERDLG_H__727583E9_EA52_43DF_A4B4_AC3D85F0E60E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BezierDlg.h : header file
//

//#include "Animator/AnmBezier.h"
class CAnmBezier;

/////////////////////////////////////////////////////////////////////////////
// CBezierDlg dialog

class CBezierDlg : public CDialog
{
// Construction
public:
	CBezierDlg(CWnd* pParent, CAnmBezier *pBezier);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBezierDlg)
	enum { IDD = IDD_DLG_BEZIER };
	UINT	m_uiID;
	CString	m_sDisplayName;
	BOOL	m_bVisable;
	//}}AFX_DATA

private:
    CAnmBezier *m_pBezier;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBezierDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBezierDlg)
	afx_msg void OnButtonBezierOk();
	afx_msg void OnButtonBezierCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEZIERDLG_H__727583E9_EA52_43DF_A4B4_AC3D85F0E60E__INCLUDED_)
