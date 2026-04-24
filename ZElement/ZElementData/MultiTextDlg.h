#if !defined(AFX_MULTITEXTDLG_H__76998FCF_B9CA_4A65_AE1D_0F09FC3CE458__INCLUDED_)
#define AFX_MULTITEXTDLG_H__76998FCF_B9CA_4A65_AE1D_0F09FC3CE458__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MultiTextDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMultiTextDlg dialog

class CMultiTextDlg : public CDialog
{
// Construction
public:
	CMultiTextDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMultiTextDlg)
	enum { IDD = IDD_DIALOG_TREASURE_DESC_EDIT };
	CString	m_strText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiTextDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK(){};
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMultiTextDlg)
	afx_msg void OnModifyed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTITEXTDLG_H__76998FCF_B9CA_4A65_AE1D_0F09FC3CE458__INCLUDED_)
