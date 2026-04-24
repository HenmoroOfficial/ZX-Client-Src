#if !defined(AFX_ACTIONSOUNDDLG_H__87CE766C_1566_4A68_8F6E_DA779AF03E64__INCLUDED_)
#define AFX_ACTIONSOUNDDLG_H__87CE766C_1566_4A68_8F6E_DA779AF03E64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionSoundDlg.h : header file
//

#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionSoundDlg dialog

class CActionSoundDlg : public CDialog
{
// Construction
public:
	CActionSoundDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionSoundDlg)
	enum { IDD = IDD_DLG_ACTION_SOUND };
	UINT	m_uiActionID;
	CString	m_sSoundFileName;
	float	m_fStartTime;
	int		m_iTotalFrames;
	DWORD	m_dwVolume;
	int		m_iStartSeconds;
	//}}AFX_DATA

private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionSoundDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionSoundDlg)
	afx_msg void OnButtonSoundImport();
	afx_msg void OnButtonActionRemove();
	afx_msg void OnButtonActionOk();
	afx_msg void OnButtonActionCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONSOUNDDLG_H__87CE766C_1566_4A68_8F6E_DA779AF03E64__INCLUDED_)
