#if !defined(AFX_ACTIONSCALEDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_)
#define AFX_ACTIONSCALEDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionScaleDlg.h : header file
//

#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionScaleDlg dialog

class CActionScaleDlg : public CDialog
{
// Construction
public:
	CActionScaleDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionScaleDlg)
	enum { IDD = IDD_DLG_ACTION_SCALE };
    UINT	m_uiObjectID;
	UINT	m_uiActionID;
    float	m_fStartTime;
	float	m_fStartScale;
	float	m_fEndScale;
	int		m_iTotalFrames;
	//}}AFX_DATA

private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionScaleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionScaleDlg)
	afx_msg void OnButtonActionRemove();
	afx_msg void OnButtonActionOk();
	afx_msg void OnButtonActionCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONSCALEDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_)
