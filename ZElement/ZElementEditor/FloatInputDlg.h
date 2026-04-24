#if !defined(AFX_FLOATINPUTDLG_H__5C5D1528_490D_4DE3_A4A0_296018321158__INCLUDED_)
#define AFX_FLOATINPUTDLG_H__5C5D1528_490D_4DE3_A4A0_296018321158__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FloatInputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFloatInputDlg dialog

class CFloatInputDlg : public CDialog
{
// Construction
public:
	CFloatInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFloatInputDlg)
	enum { IDD = IDD_DIALOG_FLOAT_INPUT };
	float	m_fFloat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFloatInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFloatInputDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOATINPUTDLG_H__5C5D1528_490D_4DE3_A4A0_296018321158__INCLUDED_)
