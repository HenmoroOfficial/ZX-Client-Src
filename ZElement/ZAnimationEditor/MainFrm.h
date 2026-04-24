// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__85A92037_259B_4459_8511_65A3483B5707__INCLUDED_)
#define AFX_MAINFRM_H__85A92037_259B_4459_8511_65A3483B5707__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AnimationEditorView.h"
#include "CWTabCtrlBar.h"
#include "ObjectList.h"
#include "BezierList.h"
#include "TrackPanel.h"

class CMainFrame : public CFrameWnd
{
    
public:
    CMainFrame();
protected: 
    DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:
    CAnimationEditorView* GetAnimationView();

    CObjectList* GetObjectList();

    CBezierList* GetBezierList();

    CTrackPanel* GetTrackPanel();

    void SwitchWindow();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMainFrame)
	public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CMainFrame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

	CAnimationEditorView* GetAnimationEditorView() const { return m_pWndView; }

protected:  // control bar embedded members
    CStatusBar m_wndStatusBar;
    CToolBar m_wndToolBar;
    CCWTabCtrlBar m_wndObjectBar;
    CCWTabCtrlBar m_wndTrackBar;
    CImageList	m_ImageList;
    CAnimationEditorView *m_pWndView;
	CStatic  m_TxtJump;
    CEdit    m_EdtJump;
	CButton  m_BtnJump;
    bool m_bFullWindow;

// Generated message map functions
protected:
    //{{AFX_MSG(CMainFrame)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSetFocus(CWnd *pOldWnd);
    afx_msg void OnFileNew();
    afx_msg void OnFileOpen();
    afx_msg void OnFileSave();
	afx_msg void OnObjectList();
	afx_msg void OnUpdateObjectList(CCmdUI* pCmdUI);
	afx_msg void OnTrackPanel();
	afx_msg void OnUpdateTrackPanel(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnShowFps();
	afx_msg void OnUpdateShowFps(CCmdUI* pCmdUI);
	afx_msg void OnCalculateTotalTime();
	afx_msg void OnClose();
#ifdef BACK_VERSION
	afx_msg void OnActivateApp(BOOL bActive, HTASK task);
#else
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
#endif
	afx_msg void OnFrameJump();
	afx_msg void OnUpdateFrameJump(CCmdUI* pCmdUI);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__85A92037_259B_4459_8511_65A3483B5707__INCLUDED_)
