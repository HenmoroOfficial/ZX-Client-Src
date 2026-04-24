#if !defined(AFX_ACTIONDIALOGUEDLG_H__49D8A2F4_FA89_4FD4_970C_2A81347877F5__INCLUDED_)
#define AFX_ACTIONDIALOGUEDLG_H__49D8A2F4_FA89_4FD4_970C_2A81347877F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionDialogueDlg.h : header file
//

#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionDialogueDlg dialog

class CActionDialogueDlg : public CDialog
{
// Construction
public:
	CActionDialogueDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionDialogueDlg)
	enum { IDD = IDD_DLG_ACTION_DIALOGUE };
    UINT	m_uiObjectID;
    UINT	m_uiActionID;
    CString m_sDialogue;
    int	    m_iTotalFrames;
    float	m_fStartTime;
	//}}AFX_DATA

private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionDialogueDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionDialogueDlg)
    afx_msg void OnButtonActionOk();
    afx_msg void OnButtonActionCancel();
	afx_msg void OnButtonActionRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONDIALOGUEDLG_H__49D8A2F4_FA89_4FD4_970C_2A81347877F5__INCLUDED_)
