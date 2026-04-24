#if !defined(AFX_ACTIONDISPLAYHIDEDLG_H__31AF3955_BD4A_483C_BD88_32F8784D1382__INCLUDED_)
#define AFX_ACTIONDISPLAYHIDEDLG_H__31AF3955_BD4A_483C_BD88_32F8784D1382__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionDisplayHideDlg.h : header file
//

#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionDisplayHideDlg dialog

class CActionDisplayHideDlg : public CDialog
{
// Construction
public:
	CActionDisplayHideDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionDisplayHideDlg)
	enum { IDD = IDD_DLG_ACTION_DISPLAY_HIDE };
    UINT	m_uiObjectID;
    UINT	m_uiActionID;
    CString m_sVisableType;
    float	m_fStartTime;
	//}}AFX_DATA

private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionDisplayHideDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionDisplayHideDlg)
    afx_msg void OnButtonActionOk();
    afx_msg void OnButtonActionCancel();
    afx_msg void OnSelchangeVisableType();
	afx_msg void OnButtonActionRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONDISPLAYHIDEDLG_H__31AF3955_BD4A_483C_BD88_32F8784D1382__INCLUDED_)
