#if !defined(AFX_OBJECTLIST_H__265A46E9_5935_48CF_93DC_5F75A2EF7AF3__INCLUDED_)
#define AFX_OBJECTLIST_H__265A46E9_5935_48CF_93DC_5F75A2EF7AF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObjectList form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CObjectList : public CFormView
{
protected:
	CObjectList();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CObjectList)

// Form Data
public:
	//{{AFX_DATA(CObjectList)
	enum { IDD = IDD_OBJECT_LIST };
	CListBox	m_ObjectList;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
    void UpdateList();
    void UpdateSelectObject();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CObjectList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CObjectList)
	afx_msg void OnButtonWorld();
    afx_msg void OnSelchangeObjectList();
    afx_msg void OnButtonNewObject();
	afx_msg void OnButtonRemoveObject();
	afx_msg void OnButtonPlayer();
	afx_msg void OnDblclkObjectList();
	afx_msg void OnButtonCopyObject();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTLIST_H__265A46E9_5935_48CF_93DC_5F75A2EF7AF3__INCLUDED_)
