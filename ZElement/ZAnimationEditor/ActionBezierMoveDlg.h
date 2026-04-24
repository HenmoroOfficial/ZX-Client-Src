#if !defined(AFX_ACTIONBEZIERMOVEDLG_H__D48FAC95_1BCA_413B_AB4A_7C365213B513__INCLUDED_)
#define AFX_ACTIONBEZIERMOVEDLG_H__D48FAC95_1BCA_413B_AB4A_7C365213B513__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionBezierMoveDlg.h : header file
//

#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionBezierMoveDlg dialog

class CActionBezierMoveDlg : public CDialog
{
    // Construction
public:
    CActionBezierMoveDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionBezierMoveDlg)
	enum { IDD = IDD_DLG_ACTION_BEZIER_MOVE };
    BOOL	m_bCloseTerrain;
    UINT	m_uiObjectID;
	UINT	m_uiActionID;
    UINT    m_uiBezierID;
    float	m_fStartTime;
	//}}AFX_DATA

private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionBezierMoveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionBezierMoveDlg)
    afx_msg void OnButtonUseOrientation();
    afx_msg void OnCloseTerrain();
	afx_msg void OnButtonActionOk();
	afx_msg void OnButtonActionCancel();
	afx_msg void OnButtonActionRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONBEZIERMOVEDLG_H__D48FAC95_1BCA_413B_AB4A_7C365213B513__INCLUDED_)
