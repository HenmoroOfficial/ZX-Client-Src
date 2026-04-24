#if !defined(AFX_ACTIONHIDEWORLDDLG_H__DC6FC74B_E0AB_409F_A804_910C6610836A__INCLUDED_)
#define AFX_ACTIONHIDEWORLDDLG_H__DC6FC74B_E0AB_409F_A804_910C6610836A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionHideWorldDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CActionHideWorldDlg dialog

class CActionHideWorldDlg : public CDialog
{
    // Construction
public:
    CActionHideWorldDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor
    
    // Dialog Data
    //{{AFX_DATA(CActionHideWorldDlg)
	enum { IDD = IDD_DLG_ACTION_HIDE_WORLD };
    UINT	m_uiObjectID;
    UINT	m_uiActionID;
    float	m_fStartTime;
    int		m_iTotalFrames;
    CString	m_sHideWorldType;
    UINT    m_uiColor;
	//}}AFX_DATA
    
private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CActionHideWorldDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    
    // Generated message map functions
    //{{AFX_MSG(CActionHideWorldDlg)
    afx_msg void OnButtonActionRemove();
    afx_msg void OnButtonActionOk();
    afx_msg void OnButtonActionCancel();
    afx_msg void OnButtonChooseColor();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONHIDEWORLDDLG_H__DC6FC74B_E0AB_409F_A804_910C6610836A__INCLUDED_)
