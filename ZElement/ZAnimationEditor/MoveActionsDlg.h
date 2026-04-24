#if !defined(AFX_MOVEACTIONSDLG_H__B54EBBEA_48F5_4B7C_9E35_0C7C5FEE890F__INCLUDED_)
#define AFX_MOVEACTIONSDLG_H__B54EBBEA_48F5_4B7C_9E35_0C7C5FEE890F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoveActionsDlg.h : header file
//

#include "TrackGroup.h"

/////////////////////////////////////////////////////////////////////////////
// CMoveActionsDlg dialog

class CMoveActionsDlg : public CDialog
{
// Construction
public:
	CMoveActionsDlg(CWnd* pParent, CTrackGroup *pGroup);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMoveActionsDlg)
	enum { IDD = IDD_DLG_MOVE_ACTIONS };
	DWORD	m_dwStartFrame;
	DWORD	m_dwEndFrame;
	DWORD	m_dwMoveToFrame;
	CString	m_sLog;
	CString	m_sMoveOperation;
	//}}AFX_DATA

private:
    CTrackGroup *m_pGroup;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMoveActionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMoveActionsDlg)
	afx_msg void OnButtonMoveActionsOk();
	afx_msg void OnButtonMoveActionsCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVEACTIONSDLG_H__B54EBBEA_48F5_4B7C_9E35_0C7C5FEE890F__INCLUDED_)
