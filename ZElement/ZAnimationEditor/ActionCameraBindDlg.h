#if !defined(AFX_ACTIONCAMERABINDDLG_H__708B0FA1_64D0_454E_A616_E1250342B6BC__INCLUDED_)
#define AFX_ACTIONCAMERABINDDLG_H__708B0FA1_64D0_454E_A616_E1250342B6BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionCameraBindDlg.h : header file
//
#include "TrackNode.h"
/////////////////////////////////////////////////////////////////////////////
// CActionCameraBindDlg dialog

class CActionCameraBindDlg : public CDialog
{
// Construction
public:
	CActionCameraBindDlg(CWnd* pParent,CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionCameraBindDlg)
	enum { IDD = IDD_DLG_ACTION_CAMERA_BIND };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	UINT	m_uiObjectID;
    UINT	m_uiActionID;
    float	m_fStartTime;
    int		m_iTotalFrames;
    UINT    m_uiFollowObj;
	float	m_fOffSetX;
    float	m_fOffSetY;
	float	m_fOffSetZ;
private:
	CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionCameraBindDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionCameraBindDlg)
	afx_msg void OnButtonActionOk();
	afx_msg void OnButtonActionCancel();
	afx_msg void OnButtonActionRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONCAMERABINDDLG_H__708B0FA1_64D0_454E_A616_E1250342B6BC__INCLUDED_)
