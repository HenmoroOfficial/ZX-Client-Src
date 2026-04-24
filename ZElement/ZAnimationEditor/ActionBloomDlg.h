#if !defined(AFX_ACTIONBLOOMDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_)
#define AFX_ACTIONBLOOMDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionBloomDlg.h : header file
//

#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionBloomDlg dialog

class CActionBloomDlg : public CDialog
{
// Construction
public:
	CActionBloomDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionBloomDlg)
	enum { IDD = IDD_DLG_ACTION_BLOOM };
    UINT	m_uiObjectID;
	UINT	m_uiActionID;
    float	m_fStartTime;
	float	m_fStartBloomBlurSize;
	float	m_fStartBloomBrightThreshold;
	float	m_fStartBloomBrightScale;
	float	m_fEndBloomBlurSize;
	float	m_fEndBloomBrightThreshold;
	float	m_fEndBloomBrightScale;
	int		m_iTotalFrames;
	//}}AFX_DATA

private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionBloomDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionBloomDlg)
	afx_msg void OnButtonActionRemove();
	afx_msg void OnButtonActionOk();
	afx_msg void OnButtonActionCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONBLOOMDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_)
