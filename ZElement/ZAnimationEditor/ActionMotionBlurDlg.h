#if !defined(AFX_ACTIONMOTIONBLURDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_)
#define AFX_ACTIONMOTIONBLURrDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionMotionBlurDlg.h : header file
//

#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionMotionBlurDlg dialog

class CActionMotionBlurDlg : public CDialog
{
// Construction
public:
	CActionMotionBlurDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionMotionBlurDlg)
	enum { IDD = IDD_DLG_ACTION_MOTION_BLUR };
    UINT	m_uiObjectID;
	UINT	m_uiActionID;
    float	m_fStartTime;
	float	m_fStartMotionBlurBlurMax;
	float	m_fStartMotionBlurBlurScale;
	float	m_fStartMotionBlurBlurScalePosition;
	float	m_fStartMotionBlurBlurScaleRotation;
	float	m_fStartMotionInFocusDistance;
	float	m_fStartMotionOutFocusDistance;
	float	m_fEndMotionBlurBlurMax;
	float	m_fEndMotionBlurBlurScale;
	float	m_fEndMotionBlurBlurScalePosition;
	float	m_fEndMotionBlurBlurScaleRotation;
	float	m_fEndMotionInFocusDistance;
	float	m_fEndMotionOutFocusDistance;
	int		m_iTotalFrames;
	//}}AFX_DATA

private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionMotionBlurDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionMotionBlurDlg)
	afx_msg void OnButtonActionRemove();
	afx_msg void OnButtonActionOk();
	afx_msg void OnButtonActionCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONMOTIONBLURDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_)
