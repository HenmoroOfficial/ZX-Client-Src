#if !defined(AFX_IMPORTTIMEDLG_H__21F2F3EA_CBEA_48CD_8AED_25E08A674D86__INCLUDED_)
#define AFX_IMPORTTIMEDLG_H__21F2F3EA_CBEA_48CD_8AED_25E08A674D86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportTimeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImportTimeDlg dialog

#include "SceneBoxArea.h"

class CImportTimeDlg : public CDialog
{
// Construction
public:
	CImportTimeDlg(CWnd* pParent = NULL);   // standard constructor
	TIME_CTR m_Time; 
// Dialog Data
	//{{AFX_DATA(CImportTimeDlg)
	enum { IDD = IDD_DIALOG_IMPORT_TIME };
	CComboBox	m_cbYear;
	CComboBox	m_cbWeek;
	CComboBox	m_cbMouth;
	CComboBox	m_cbMinu;
	CComboBox	m_cbHour;
	CComboBox	m_cbDay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitCombox();
	// Generated message map functions
	//{{AFX_MSG(CImportTimeDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTTIMEDLG_H__21F2F3EA_CBEA_48CD_8AED_25E08A674D86__INCLUDED_)
