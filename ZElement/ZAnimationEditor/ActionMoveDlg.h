#if !defined(AFX_ACTIONMOVEDLG_H__D47A4EAA_D2DB_43D6_9E29_A6084CCF525C__INCLUDED_)
#define AFX_ACTIONMOVEDLG_H__D47A4EAA_D2DB_43D6_9E29_A6084CCF525C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionMoveDlg.h : header file
//

#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionMoveDlg dialog

class CActionMoveDlg : public CDialog
{
// Construction
public:
	CActionMoveDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionMoveDlg)
	enum { IDD = IDD_DLG_ACTION_MOVE };
	BOOL	m_bCloseTerrain;
	UINT	m_uiObjectID;
	UINT	m_uiActionID;
    float	m_fStartPosX;
    float	m_fStartPosY;
	float	m_fStartPosZ;
	float	m_fEndPosX;
	float	m_fEndPosY;
	float	m_fEndPosZ;
	float	m_fAccelerationPercent;
	int	    m_iTotalFrames;
	float	m_fStartTime;
	float	m_fOriW;
	float	m_fOriX;
	float	m_fOriY;
    float	m_fOriZ;
	float	m_fRotateAngle;
	CString	m_sRotateAxisType;
	//}}AFX_DATA

private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionMoveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionMoveDlg)
	afx_msg void OnButtonActionOk();
	afx_msg void OnButtonActionCancel();
	afx_msg void OnCloseTerrain();
	afx_msg void OnButtonSetStartPos();
	afx_msg void OnButtonSetEndPos();
	afx_msg void OnButtonActionRemove();
	afx_msg void OnButtonSetOrientation();
	afx_msg void OnSelchangeRotateAxisType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONMOVEDLG_H__D47A4EAA_D2DB_43D6_9E29_A6084CCF525C__INCLUDED_)
