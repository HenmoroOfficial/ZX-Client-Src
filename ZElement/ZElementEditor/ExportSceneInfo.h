#if !defined(AFX_EXPORTSCENEINFO_H__EC09367C_5EFB_4FCA_BE1D_C40F2C331B0E__INCLUDED_)
#define AFX_EXPORTSCENEINFO_H__EC09367C_5EFB_4FCA_BE1D_C40F2C331B0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportSceneInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExportSceneInfo dialog

class CExportSceneInfo : public CDialog
{
// Construction
public:
	CExportSceneInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExportSceneInfo)
	enum { IDD = IDD_DIALOG_EXPORT_INFO };
	BOOL	m_bExportNPC;
	BOOL	m_bExportGather;
	CString	m_strPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportSceneInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExportSceneInfo)
	afx_msg void OnButtonOpen();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTSCENEINFO_H__EC09367C_5EFB_4FCA_BE1D_C40F2C331B0E__INCLUDED_)
