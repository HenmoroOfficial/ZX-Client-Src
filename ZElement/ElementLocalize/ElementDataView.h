#if !defined(AFX_ELEMENTDATAVIEW_H__C1E95F8B_078D_492B_9448_80269A6FAB55__INCLUDED_)
#define AFX_ELEMENTDATAVIEW_H__C1E95F8B_078D_492B_9448_80269A6FAB55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElementDataView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CElementDataView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "DataManager.h"

class CTemplateBase;
class CTemplateManager;

class CElementDataView : public CFormView
{
protected:
	CElementDataView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CElementDataView)

// Form Data
public:
	//{{AFX_DATA(CElementDataView)
	enum { IDD = IDD_DIALOG_TREE_VIEW };
	CTreeCtrl	m_treeData;
	//}}AFX_DATA

// Attributes
public:
protected:
	CImageList m_ImageList;
	int m_nTextItem;

// Operations
public:
	void UpdateTree();
	void ChangeItemStatus(HTREEITEM hItem, bool bNew);
	bool FindNewItem();
	bool FindNewItemEx();
	void NextItem();
	void PrevItem();

	bool FindByID();
	bool FindUp(bool bNoCur = false);
	bool FindDown(bool bNoCur = false);
	bool FindDown_Ex(bool bNoCur = false);
	CString   GetPath(HTREEITEM hItem);
	void      GoCurItem();
	HTREEITEM GetNextTextItem(HTREEITEM hItem, HTREEITEM hRootItem);
	const HTREEITEM* GetItems() {return m_hItems;}
	int GetTextItemNum() {return m_nTextItem;}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElementDataView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HTREEITEM m_hItems[CHUNK_NUM];
	
	HTREEITEM EnumFindUp(HTREEITEM hParent);
	HTREEITEM EnumFindDown(HTREEITEM hParent);

	HTREEITEM EnumFindByID(HTREEITEM hParent, unsigned int id);

	HTREEITEM FindNextItem(HTREEITEM hCurItem);
	HTREEITEM FindNextItem(HTREEITEM hCurItem, HTREEITEM hRootItem); //limit in root
	HTREEITEM FindLastItem(HTREEITEM hCurItem);

	HTREEITEM FindNextRoot(HTREEITEM hCurItem);
	HTREEITEM FindLastRoot(HTREEITEM hCurItem);

	void UpdateElementPart(HTREEITEM hParent,CTemplateManager *pMan,bool bSetTemplateID = true);
	int EnumSubTemplate(HTREEITEM hParent,HTREEITEM hPart, HTREEITEM hTemplate, CTemplateBase *pParentTempl);

	virtual ~CElementDataView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CElementDataView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangedTreeData(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMENTDATAVIEW_H__C1E95F8B_078D_492B_9448_80269A6FAB55__INCLUDED_)
