#if !defined(AFX_ACTIONMOTIONDLG_H__EC50D12F_D2E7_4F4A_991F_403495EBD0AE__INCLUDED_)
#define AFX_ACTIONMOTIONDLG_H__EC50D12F_D2E7_4F4A_991F_403495EBD0AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionMotionDlg.h : header file
//

#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionMotionDlg dialog

class CActionMotionDlg : public CDialog
{
// Construction
public:
	CActionMotionDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor

    void SetActionNames();

// Dialog Data
	//{{AFX_DATA(CActionMotionDlg)
	enum { IDD = IDD_DLG_ACTION_MOTION };
    CComboBox	m_cActionName;
    UINT	m_uiObjectID;
    UINT	m_uiActionID;
    CString m_sMotionType;
    int     m_iStartFrame;
    int	    m_iBlendFrames;
    int	    m_iTotalFrames;
    float	m_fStartTime;
	CString	m_sActionName;
	BOOL	m_bShowGFX;
	float	m_fSpeed;
	BOOL	m_bHostPlayer;
	//}}AFX_DATA

private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;
    std::vector<ACString> m_vActionNameList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionMotionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionMotionDlg)
    afx_msg void OnButtonActionOk();
    afx_msg void OnButtonActionCancel();
    afx_msg void OnSelchangeMotionType();
    afx_msg void OnDropdownActionName();
    afx_msg void OnSelchangeActionName();
	afx_msg void OnButtonActionRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONMOTIONDLG_H__EC50D12F_D2E7_4F4A_991F_403495EBD0AE__INCLUDED_)
