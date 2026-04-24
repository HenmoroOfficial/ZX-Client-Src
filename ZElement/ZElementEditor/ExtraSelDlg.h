#if !defined(AFX_EXTRASELDLG_H__474C44C6_4626_451C_81F7_8F4928B9F4D7__INCLUDED_)
#define AFX_EXTRASELDLG_H__474C44C6_4626_451C_81F7_8F4928B9F4D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtraSelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExtraSelDlg dialog

class CExtraSelDlg : public CDialog
{
// Construction
public:
	CExtraSelDlg(CWnd* pParent = NULL);   // standard constructor
	int m_nExtraAreaID;
	void UpdateList();
	void Init( int nObjType) { m_nObjectType = nObjType; }
// Dialog Data
	//{{AFX_DATA(CExtraSelDlg)
	enum { IDD = IDD_DIALOG_EXTRA_SEL };
	CListBox	m_listSelExtra;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtraSelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nObjectType;
	// Generated message map functions
	//{{AFX_MSG(CExtraSelDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTRASELDLG_H__474C44C6_4626_451C_81F7_8F4928B9F4D7__INCLUDED_)
