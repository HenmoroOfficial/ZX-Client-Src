#if !defined(AFX_ACTIONCAMERAFILTERDLG_H__49CEE60B_2AF8_4A72_8BD4_1D696B7B8382__INCLUDED_)
#define AFX_ACTIONCAMERAFILTERDLG_H__49CEE60B_2AF8_4A72_8BD4_1D696B7B8382__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionCameraFilterDlg.h : header file
//

#include "AFXCTL.H"
#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionCameraFilterDlg dialog

class CActionCameraFilterDlg : public CDialog
{
    // Construction
public:
    CActionCameraFilterDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor
    
    // Dialog Data
    //{{AFX_DATA(CActionCameraFilterDlg)
	enum { IDD = IDD_DLG_ACTION_CAMERA_FILTER };
    UINT	m_uiObjectID;
    UINT	m_uiActionID;
    float	m_fStartTime;
    int		m_iTotalFrames;
    UINT    m_uiColor;
	CString	m_sFilterType;
	float	m_fTransparent;
	//}}AFX_DATA
    
private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CActionCameraFilterDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    
    // Generated message map functions
    //{{AFX_MSG(CActionCameraFilterDlg)
    afx_msg void OnButtonActionRemove();
    afx_msg void OnButtonActionOk();
    afx_msg void OnButtonActionCancel();
	afx_msg void OnButtonChooseColor();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeFilterType();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONCAMERAFILTERDLG_H__49CEE60B_2AF8_4A72_8BD4_1D696B7B8382__INCLUDED_)
