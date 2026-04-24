#if !defined(AFX_ACTIONTRANSPARENTDLG_H__5C2B4565_3D67_40F5_A608_7AE9406E240E__INCLUDED_)
#define AFX_ACTIONTRANSPARENTDLG_H__5C2B4565_3D67_40F5_A608_7AE9406E240E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionTransparentDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CActionTransparentDlg dialog

class CActionTransparentDlg : public CDialog
{
    // Construction
public:
    CActionTransparentDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor
    
    // Dialog Data
    //{{AFX_DATA(CActionTransparentDlg)
    enum { IDD = IDD_DLG_ACTION_TRANSPARENT };
    UINT	m_uiObjectID;
    UINT	m_uiActionID;
    float	m_fStartTime;
    float	m_fStartTransparent;
    float	m_fEndTransparent;
    int		m_iTotalFrames;
    //}}AFX_DATA
    
private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CActionTransparentDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    
    // Generated message map functions
    //{{AFX_MSG(CActionTransparentDlg)
    afx_msg void OnButtonActionRemove();
    afx_msg void OnButtonActionOk();
    afx_msg void OnButtonActionCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONTRANSPARENTDLG_H__5C2B4565_3D67_40F5_A608_7AE9406E240E__INCLUDED_)
