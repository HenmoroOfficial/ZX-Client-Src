// ElementLocalizeView.h : interface of the CElementLocalizeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTLOCALIZEVIEW_H__D7C8DC91_5450_46CD_9208_9EA38F5EC2DF__INCLUDED_)
#define AFX_ELEMENTLOCALIZEVIEW_H__D7C8DC91_5450_46CD_9208_9EA38F5EC2DF__INCLUDED_

class CTextBase;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CElementLocalizeView : public CFormView
{
protected: // create from serialization only
	CElementLocalizeView();
	DECLARE_DYNCREATE(CElementLocalizeView)

public:
	//{{AFX_DATA(CElementLocalizeView)
	enum { IDD = IDD_ELEMENTLOCALIZE_FORM };
	CStatic	m_Flag;
	CString	m_strOrg;
	CString	m_strTrans;
	//}}AFX_DATA

// Attributes
public:
	CElementLocalizeDoc* GetDocument();

// Operations
public:
	void ChangeTextBase(CTextBase *pTextBase,HTREEITEM hSelItem);
	void Replace(){ OnButtonReplace(); }
	HTREEITEM GetCurItem(){ return m_hSelItem; }
	void Reset(){ m_hSelItem = NULL; m_pText = NULL; m_strTrans = _T(""); m_strOrg = _T(""); UpdateData(FALSE);}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElementLocalizeView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CElementLocalizeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CTextBase *m_pText;
	HTREEITEM m_hSelItem;
	HICON m_hDel;
	HICON m_hNew;
	HICON m_hNormal;

// Generated message map functions
protected:
	void UpdateTransInfo();
	bool ReplaceText();
	//{{AFX_MSG(CElementLocalizeView)
	afx_msg void OnButtonReplace();
	afx_msg void OnChangeEditTrans();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonAutoTrans();
	afx_msg void OnButtonReplaceNext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ElementLocalizeView.cpp
inline CElementLocalizeDoc* CElementLocalizeView::GetDocument()
   { return (CElementLocalizeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMENTLOCALIZEVIEW_H__D7C8DC91_5450_46CD_9208_9EA38F5EC2DF__INCLUDED_)
