#if !defined(AFX_ACTIONROTATEDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_)
#define AFX_ACTIONROTATEDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionRotateDlg.h : header file
//

#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionRotateDlg dialog

class CActionRotateDlg : public CDialog
{
// Construction
public:
	CActionRotateDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionRotateDlg)
	enum { IDD = IDD_DLG_ACTION_ROTATE };
    UINT	m_uiObjectID;
	UINT	m_uiActionID;
    float	m_fStartTime;
	float	m_fStartRotateW;
	float	m_fStartRotateX;
	float	m_fStartRotateY;
	float	m_fStartRotateZ;
	float	m_fEndRotateW;
	float	m_fEndRotateX;
	float	m_fEndRotateY;
	float	m_fEndRotateZ;
	int		m_iTotalFrames;
	//}}AFX_DATA

private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionRotateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionRotateDlg)
    afx_msg void OnButtonSetStartRotate();
    afx_msg void OnButtonSetEndRotate();
    afx_msg void OnButtonUseSquad();
	afx_msg void OnButtonActionRemove();
	afx_msg void OnButtonActionOk();
	afx_msg void OnButtonActionCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONROTATEDLG_H__77822D6F_668C_4FC5_B4BE_AC49ADB99E69__INCLUDED_)
