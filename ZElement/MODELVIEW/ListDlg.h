#if !defined(AFX_LISTDLG_H__1F72F986_C77B_487A_9CB8_3EC4B6725E9F__INCLUDED_)
#define AFX_LISTDLG_H__1F72F986_C77B_487A_9CB8_3EC4B6725E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListDlg.h : header file
//
#include "MltiTree.h"



/////////////////////////////////////////////////////////////////////////////
// CListDlg dialog

class CListDlg : public CDialog
{
// Construction
public:
	CListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CListDlg)
	enum { IDD = IDD_DIALOG_LIST };
	CTreeCtrl	m_pathNameTree;
	CTabCtrl	m_tabSel;
	CTreeCtrl	m_nameTree;
	//}}AFX_DATA

	void LoadList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK(){}
	virtual void OnCancel(){}
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CListDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnClickTreeObject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeObject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusTreeObject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTabSel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreePathname(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void ChangeList();
	void LoadOneList(CTreeCtrl&);

//Attributes
protected:
	//DATA_TYPE m_ObjectType;//跟踪列表中当前处理对象的类型

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTDLG_H__1F72F986_C77B_487A_9CB8_3EC4B6725E9F__INCLUDED_)
