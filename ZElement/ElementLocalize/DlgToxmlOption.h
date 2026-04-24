#if !defined(AFX_DLGTOXMLOPTION_H__50386BF5_61A0_47ED_B3F9_379264980076__INCLUDED_)
#define AFX_DLGTOXMLOPTION_H__50386BF5_61A0_47ED_B3F9_379264980076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgToxmlOption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgToxmlOption dialog

class CDlgToxmlOption : public CDialog
{
// Construction
public:
	CDlgToxmlOption(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgToxmlOption)
	enum { IDD = IDD_DIALOG_TOXML };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	void GetOptionInfo(bool& bOnlyUntrans, bool& bIncludeID)
	{
		bOnlyUntrans = m_bOnlyUntrans;
		bIncludeID = m_bIncludeID;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgToxmlOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL



// Implementation
protected:
	virtual void OnOK();

	bool m_bOnlyUntrans;
	bool m_bIncludeID;

	// Generated message map functions
	//{{AFX_MSG(CDlgToxmlOption)
	afx_msg void OnCheckUntrans();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTOXMLOPTION_H__50386BF5_61A0_47ED_B3F9_379264980076__INCLUDED_)
