// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__40A7761D_01BE_4AF3_A8FB_6BF27522B3CC__INCLUDED_)
#define AFX_MAINFRM_H__40A7761D_01BE_4AF3_A8FB_6BF27522B3CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImportProgressDlg.h"

class CDlgSelectItems;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
protected:
	CSplitterWnd m_wndSplitter;
	CImportProgressDlg m_wndImort;
// Operations
public:
	CWnd *GetLeftView();
	CWnd *GetRightView();
	void Replace();
	void UpdateTextStatus();
	
	void SetTotalNum(int nTotal);
	void SetDisuseNum( int nDisuse);
	void SetNewNum( int nNew);
	void SetTransNum( int nTrans);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CDlgSelectItems*			m_pDlgSelectItems;
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	void        AutoBackupData();
	bool        AutoDeleteBackup();
	bool        IsBackup();
	bool        IsWeekBackup();
	void        SaveConfigs();
	void        LoadConfigs();
	INT64       GetDiskFree();
	
	DWORD       m_dwBkTime;//间隔
	SYSTEMTIME  m_LastBKTime;//上次时间
	SYSTEMTIME  m_LastWeekBKTime;
	bool        m_bDeleted;
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnImportGameData();
	afx_msg void OnDataOpen();
	afx_msg void OnDataSave();
	afx_msg void OnDataSaveCustom();
	afx_msg void OnDataSaveAs();
	afx_msg void OnImportOther();
	afx_msg void OnNext();
	afx_msg void OnPrev();
	afx_msg void OnFindNewItem();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnFindText();
	afx_msg void OnFindDown();
	afx_msg void OnFindUp();
	afx_msg void OnExportGameData();
	afx_msg void OnGoSel();
	afx_msg void OnFindNewItemex();
	afx_msg void OnStatItems();
	afx_msg void OnHelpExt();
	afx_msg void OnSaveTextToFile();
	afx_msg void OnImportFromTextFile();
	afx_msg void OnImportOrderText();
	afx_msg void OnCheckSynac();
	afx_msg void OnAutoToBig5();
	afx_msg void OnCopyOrg2Trans();
	afx_msg void OnClearTrans();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__40A7761D_01BE_4AF3_A8FB_6BF27522B3CC__INCLUDED_)
