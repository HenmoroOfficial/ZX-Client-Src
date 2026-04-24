#if !defined(AFX_BEZIERLIST_H__A285B77D_01D2_46DE_A2BA_BD23697772BB__INCLUDED_)
#define AFX_BEZIERLIST_H__A285B77D_01D2_46DE_A2BA_BD23697772BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BezierList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBezierList form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CBezierList : public CFormView
{
protected:
	CBezierList();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBezierList)

// Form Data
public:
	//{{AFX_DATA(CBezierList)
	enum { IDD = IDD_BEZIER_LIST };
    CListBox	m_BezierList;
    CListBox	m_BezierNodeList;
	//}}AFX_DATA

// Attributes
public:

private:
    A3DVECTOR3 m_vPointPos;
    A3DQUATERNION m_qPointOrientation;

// Operations
public:
    void UpdateList();
    void UpdateSelectBezier();
    void UpdateNodeList();
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CBezierList)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    virtual ~CBezierList();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    
    // Generated message map functions
    //{{AFX_MSG(CBezierList)
    afx_msg void OnSelchangeBezierList();
    afx_msg void OnButtonNewBezier();
    afx_msg void OnButtonCopyBezier();
    afx_msg void OnButtonRemoveBezier();
    afx_msg void OnDblclkBezierList();
	afx_msg void OnDblclkBezierNodeList();
	afx_msg void OnSelchangeBezierNodeList();
	afx_msg void OnButtonDisplayBezier();
	afx_msg void OnButtonHideBezier();
	afx_msg void OnButtonCopyBezierPointInfo();
	afx_msg void OnButtonPasteBezierPointInfo();
	afx_msg void OnButtonInsertBezierPoint();
	afx_msg void OnButtonRemoveBezierPoint();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEZIERLIST_H__A285B77D_01D2_46DE_A2BA_BD23697772BB__INCLUDED_)
