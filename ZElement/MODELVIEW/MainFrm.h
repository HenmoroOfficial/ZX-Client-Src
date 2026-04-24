// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__BCDA5E32_A0F8_4CED_8F06_E854660A8352__INCLUDED_)
#define AFX_MAINFRM_H__BCDA5E32_A0F8_4CED_8F06_E854660A8352__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"
#include "ListBar.h"
#include "AnimationBar.h"
#include "CharactorBar.h"
#include "PartDlg.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
	CChildView *GetChildView(){ return &m_wndView; } 
	CAnimationBar *GetAnimationBar(){ return &m_wndAnimationBar; }
	CCharactorBar *GetCharactorBar(){ return &m_wndCharactorBar; }
	CPartDlg* GetPartDlg(){return &m_wndPartDlg;} 
protected:
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:
	CPartDlg      m_wndPartDlg;
	CCharactorBar m_wndCharactorBar;
	bool IsCreated(){ return m_bCreated; }
	//	Application is active ?
	bool AppIsActive() { return m_bActive; }
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CChildView  m_wndView;
	CListBar    m_wndListBar;
	CAnimationBar m_wndAnimationBar;
	
	bool          m_bCreated;
	bool		  m_bActive;		//	App active flag
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnViewModelPath();
	afx_msg void OnUseDefualtCamPos();
	afx_msg void OnUpdateUseDefualtCamPos(CCmdUI* pCmdUI);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__BCDA5E32_A0F8_4CED_8F06_E854660A8352__INCLUDED_)
