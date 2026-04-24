#if !defined(AFX_ACTIONHOOKDLG_H__78F92319_87B8_447C_9E57_A8F7CDFA84E2__INCLUDED_)
#define AFX_ACTIONHOOKDLG_H__78F92319_87B8_447C_9E57_A8F7CDFA84E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionHookDlg.h : header file
//

#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionHookDlg dialog

class CActionHookDlg : public CDialog
{
// Construction
public:
	CActionHookDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);  // standard constructor

    void SetHookNames();

    void SetElseHookNames();

// Dialog Data
	//{{AFX_DATA(CActionHookDlg)
	enum { IDD = IDD_DLG_ACTION_HOOK };
    CComboBox	m_cHookName;
    CComboBox	m_cElseHookName;
    UINT	m_uiObjectID;
    UINT	m_uiActionID;
    int	    m_iTotalFrames;
    float	m_fStartTime;
    CString m_sHangerName;
    CString m_sHookName;
    UINT    m_uiHookObjectID;
    CString m_sElseHookName;
	//}}AFX_DATA

private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;
    std::vector<ACString> m_vHookNameList;
    std::vector<ACString> m_vElseHookNameList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionHookDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CActionHookDlg)
	afx_msg void OnButtonActionOk();
	afx_msg void OnButtonActionCancel();
	afx_msg void OnButtonActionRemove();
	afx_msg void OnDropdownHookName();
	afx_msg void OnDropdownElseHookName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONHOOKDLG_H__78F92319_87B8_447C_9E57_A8F7CDFA84E2__INCLUDED_)
