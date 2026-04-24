#if !defined(AFX_IMPORTFROMHISTROY_H__752149C8_52EF_48C4_BEC5_7EE1F890FA2E__INCLUDED_)
#define AFX_IMPORTFROMHISTROY_H__752149C8_52EF_48C4_BEC5_7EE1F890FA2E__INCLUDED_


#include "SceneObjectManager.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportFromHistroy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImportFromHistroy dialog

class CImportFromHistroy : public CDialog
{
// Construction
public:
	CImportFromHistroy(CWnd* pParent = NULL);   // standard constructor

	~CImportFromHistroy();

// Dialog Data
	//{{AFX_DATA(CImportFromHistroy)
	enum { IDD = IDD_IMPORT_FROM_HISTORY };
	CListCtrl	m_NPCInfoList;
	//}}AFX_DATA

	CSceneObjectManager* m_pHisSceneObjectMan;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportFromHistroy)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImportFromHistroy)
	virtual BOOL OnInitDialog();
	afx_msg void OnImportScene();
	afx_msg void OnCompareWithCurr();
	afx_msg void OnRefreshCurrScene();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTFROMHISTROY_H__752149C8_52EF_48C4_BEC5_7EE1F890FA2E__INCLUDED_)
