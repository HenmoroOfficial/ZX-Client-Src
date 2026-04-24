// AnimationEditor.h : main header file for the ANIMATIONEDITOR application
//

#if !defined(AFX_ANIMATIONEDITOR_H__92546B1E_A8A4_4A2B_BA30_D4963470298B__INCLUDED_)
#define AFX_ANIMATIONEDITOR_H__92546B1E_A8A4_4A2B_BA30_D4963470298B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CAnimationEditorApp:
// See AnimationEditor.cpp for the implementation of this class
//


class CAnimationEditorApp : public CWinApp
{
public:
    CAnimationEditorApp();

public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAnimationEditorApp)
	public:
    virtual BOOL InitInstance();
    virtual BOOL OnIdle(LONG lCount);
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

public:
    void UpdateList();
    CObjectList* GetObjectList();
    CBezierList* GetBezierList();
    CTrackPanel* GetTrackPanel();
    void OnChanged();
    bool IsChanged();
    void ResetChanged();
    bool AskSaveWork();
    void AutoSave();

// Implementation
public:
    //{{AFX_MSG(CAnimationEditorApp)
    afx_msg void OnAppAbout();
    afx_msg void OnFileNew();
    afx_msg void OnFileOpen();
    afx_msg void OnFileSave();
	afx_msg void OnAnimationPlay();
	afx_msg void OnAnimationReset();
	afx_msg void OnUpdateAnimationPlay(CCmdUI* pCmdUI);
	afx_msg void OnAnimationStop();
	afx_msg void OnUpdateAnimationStop(CCmdUI* pCmdUI);
	afx_msg void OnAnimationPause();
	afx_msg void OnUpdateAnimationPause(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAs();
    afx_msg void OnUpdateRecentFileMenu(CCmdUI* pCmdUI);
    afx_msg void OnOpenRecentFile(UINT nID);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void ProcessKeyboard();

	void UpdateFrame();

private:
    CString m_sFileName;
    bool m_bChanged;
    bool m_bKeyPressed;
    float m_fAutoSaveTimer;

public:	
	BOOL				m_bAppActive;
	CList<CString,CString>		m_lRecentFiles;
};

extern CAnimationEditorApp theApp;

inline CMainFrame* AfxGetMainFrame() { return ( CMainFrame* )AfxGetMainWnd(); }
inline CAnimationEditorView* AfxGetEditorView()
{
	return AfxGetMainFrame()->GetAnimationEditorView();
}


AWString g_aGetRelativePath(const CString &fullPath);
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMATIONEDITOR_H__92546B1E_A8A4_4A2B_BA30_D4963470298B__INCLUDED_)
