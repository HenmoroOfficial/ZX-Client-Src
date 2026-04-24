#if !defined(AFX_TRACKCHANNELDLG_H__87D398EA_8C36_4EAE_BE61_950CAD446EB3__INCLUDED_)
#define AFX_TRACKCHANNELDLG_H__87D398EA_8C36_4EAE_BE61_950CAD446EB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrackChannelDlg.h : header file
//

#include "TrackGroup.h"
#include "TrackChannel.h"

/////////////////////////////////////////////////////////////////////////////
// CTrackChannelDlg dialog

class CTrackChannelDlg : public CDialog
{
// Construction
public:
	CTrackChannelDlg(CWnd* pParent, CTrackGroup *pGroup, CTrackChannel *pChannel);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTrackChannelDlg)
	enum { IDD = IDD_DLG_CHANNEL };
	CString	m_sName;
	UINT	m_uiObjectID;
	UINT	m_uiID;
	//}}AFX_DATA
private:
    CTrackGroup *m_pGroup;
    CTrackChannel *m_pChannel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrackChannelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrackChannelDlg)
	afx_msg void OnButtonChannelOk();
	afx_msg void OnButtonChannelCancel();
	afx_msg void OnButtonChannelRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACKCHANNELDLG_H__87D398EA_8C36_4EAE_BE61_950CAD446EB3__INCLUDED_)
