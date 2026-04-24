#if !defined(AFX_DLGSELECTITEMS_H__15455A33_33C5_4B7D_8B4C_4B7E5E61974C__INCLUDED_)
#define AFX_DLGSELECTITEMS_H__15455A33_33C5_4B7D_8B4C_4B7E5E61974C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectItems.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectItems dialog
#include "common.h"

class CDlgSelectItems : public CDialog
{
// Construction
public:
	CDlgSelectItems(CWnd* pParent = NULL);   // standard constructor
	BOOL GetDataSwitch(int datachunk);

// Dialog Data
	//{{AFX_DATA(CDlgSelectItems)
	enum { IDD = IDD_SELECT_ITEMS };
	BOOL	m_bDataSwitch[CHUNK_NUM];
	BOOL	m_bCKEnableLabel;
	BOOL	m_bSelectAll;
	CString	m_csLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectItems)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectItems)
	virtual void OnOK();
	afx_msg void OnSelectAll();
	afx_msg void OnCkLabel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void EnableLabel(bool bEnable = true) { m_bEnableLabel = bEnable; }
protected:
	bool m_bEnableLabel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTITEMS_H__15455A33_33C5_4B7D_8B4C_4B7E5E61974C__INCLUDED_)
