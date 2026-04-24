#if !defined(AFX_NEWACTIONDLG_H__6061C62C_97F6_435B_9855_10CD59E6BEAC__INCLUDED_)
#define AFX_NEWACTIONDLG_H__6061C62C_97F6_435B_9855_10CD59E6BEAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewActionDlg.h : header file
//

#include "TrackChannel.h"
#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CNewActionDlg dialog

class CNewActionDlg : public CDialog
{
// Construction
public:
	CNewActionDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewActionDlg)
	enum { IDD = IDD_DLG_NEW_ACTION };
	float	m_fStartTime;
	CString	m_sActionType;
	//}}AFX_DATA

private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewActionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewActionDlg)
	afx_msg void OnButtonNewActionOk();
	afx_msg void OnButtonNewActionCancel();
	afx_msg void OnSelchangeActionType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWNODEDLG_H__6061C62C_97F6_435B_9855_10CD59E6BEAC__INCLUDED_)
