#if !defined(AFX_ACTIONJUMPDLG_H__8D7F2EE2_A610_416E_84D1_9841E114F1F0__INCLUDED_)
#define AFX_ACTIONJUMPDLG_H__8D7F2EE2_A610_416E_84D1_9841E114F1F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionJumpDlg.h : header file
//

#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionJumpDlg dialog

class CActionJumpDlg : public CDialog
{
// Construction
public:
	CActionJumpDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CActionJumpDlg)
	enum { IDD = IDD_DLG_ACTION_JUMP };
    UINT	m_uiObjectID;
    UINT	m_uiActionID;
    float	m_fEndPosX;
    float	m_fEndPosY;
    float	m_fEndPosZ;
    float	m_fStartPosX;
    float	m_fStartPosY;
    float	m_fStartPosZ;
    int	    m_iTotalFrames;
    float	m_fStartTime;
	//}}AFX_DATA

private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActionJumpDlg)
	protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
        
// Implementation
protected:
        
    // Generated message map functions
	//{{AFX_MSG(CActionJumpDlg)
    afx_msg void OnButtonActionOk();
    afx_msg void OnButtonActionCancel();
    afx_msg void OnCloseTerrain();
    afx_msg void OnButtonSetStartPos();
    afx_msg void OnButtonSetEndPos();
	afx_msg void OnButtonActionRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONJUMPDLG_H__8D7F2EE2_A610_416E_84D1_9841E114F1F0__INCLUDED_)
