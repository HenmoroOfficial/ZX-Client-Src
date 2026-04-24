#if !defined(AFX_TRACKGROUPDLG_H__5FA04BCF_6D50_4B57_8A89_D25DF8559666__INCLUDED_)
#define AFX_TRACKGROUPDLG_H__5FA04BCF_6D50_4B57_8A89_D25DF8559666__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrackGroupDlg.h : header file
//

#include "TrackGroup.h"

/////////////////////////////////////////////////////////////////////////////
// CTrackGroupDlg dialog

class CTrackGroupDlg : public CDialog
{
// Construction
public:
	CTrackGroupDlg(CWnd* pParent, CTrackPanel *pPanel, CTrackGroup *pGroup);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTrackGroupDlg)
	enum { IDD = IDD_DLG_GROUP };
	UINT	m_uiID;
	CString	m_sName;
	//}}AFX_DATA


private:
    CTrackPanel *m_pPanel;
    CTrackGroup *m_pGroup;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrackGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrackGroupDlg)
	afx_msg void OnButtonGroupCreateChannel();
	afx_msg void OnButtonGroupRemove();
	afx_msg void OnButtonGroupOk();
	afx_msg void OnButtonGroupCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACKGROUPDLG_H__5FA04BCF_6D50_4B57_8A89_D25DF8559666__INCLUDED_)
