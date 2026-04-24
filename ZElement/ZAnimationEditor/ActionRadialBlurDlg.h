#if !defined(AFX_ACTIONRADIALBLURDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_)
#define AFX_ACTIONRADIALBLURDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionRadialBlurDlg.h : header file
//

#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionRadialBlurDlg dialog

class CActionRadialBlurDlg : public CDialog
{
// Construction
public:
	CActionRadialBlurDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionRadialBlurDlg)
	enum { IDD = IDD_DLG_ACTION_RADIAL_BLUR };
    UINT	m_uiObjectID;
	UINT	m_uiActionID;
    float	m_fStartTime;
	float	m_fStartRadialBlurBlurCenterX;
	float	m_fStartRadialBlurBlurCenterY;
	float	m_fStartRadialBlurBlurLength;
	float	m_fStartRadialBlurInFocusRadius;
	float	m_fStartRadialBlurOutFocusRadius;
	float	m_fEndRadialBlurBlurCenterX;
	float	m_fEndRadialBlurBlurCenterY;
	float	m_fEndRadialBlurBlurLength;
	float	m_fEndRadialBlurInFocusRadius;
	float	m_fEndRadialBlurOutFocusRadius;
	int		m_iTotalFrames;
	//}}AFX_DATA

private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionRadialBlurDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionRadialBlurDlg)
	afx_msg void OnButtonActionRemove();
	afx_msg void OnButtonActionOk();
	afx_msg void OnButtonActionCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONRADIALBLURDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_)
