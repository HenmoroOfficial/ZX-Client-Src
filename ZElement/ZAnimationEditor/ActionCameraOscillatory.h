#if !defined(AFX_ACTIONCAMERAOSCILLATORY_H__284D6321_056F_4066_B687_EBD3724A500A__INCLUDED_)
#define AFX_ACTIONCAMERAOSCILLATORY_H__284D6321_056F_4066_B687_EBD3724A500A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActionCameraOscillatory.h : header file
//

#include "TrackNode.h"

/////////////////////////////////////////////////////////////////////////////
// CActionCameraOscillatory dialog

class CActionCameraOscillatory : public CDialog
{
    // Construction
public:
    CActionCameraOscillatory(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode);   // standard constructor
    
    // Dialog Data
    //{{AFX_DATA(CActionCameraOscillatory)
	enum { IDD = IDD_DLG_ACTION_CAMERA_OSCILLATORY };
    UINT	m_uiActionID;
    float	m_fStartTime;
    int		m_iTotalFrames;
	CString	m_sOscillatoryType;
    CString	m_sOscillatoryAxis;
	CString	m_sOscillatoryAttenuation;
	float	m_fDisplacement;
	float	m_fFrequency;
	//}}AFX_DATA
    
private:
    CTrackChannel *m_pChannel;
    CTrackNode *m_pNode;
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CActionCameraOscillatory)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    
    // Generated message map functions
    //{{AFX_MSG(CActionCameraOscillatory)
    afx_msg void OnButtonActionOk();
    afx_msg void OnButtonActionCancel();
    afx_msg void OnButtonActionRemove();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIONCAMERAOSCILLATORY_H__284D6321_056F_4066_B687_EBD3724A500A__INCLUDED_)
