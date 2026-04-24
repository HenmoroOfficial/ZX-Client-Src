#if !defined(AFX_IMPORTPROGRESSDLG_H__E8DF88F5_E81D_412D_A35E_AD3BC0187A48__INCLUDED_)
#define AFX_IMPORTPROGRESSDLG_H__E8DF88F5_E81D_412D_A35E_AD3BC0187A48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportProgressDlg.h : header file
//

class CDataManager;

/////////////////////////////////////////////////////////////////////////////
// CImportProgressDlg dialog

class CImportProgressDlg : public CDialog
{
// Construction
public:
	CImportProgressDlg(CWnd* pParent = NULL);   // standard constructor
	void StartProcessData();
	static DWORD WINAPI ThreadProcImportGame(void *param);
	static DWORD WINAPI ThreadProcImport(void *param);
	static DWORD WINAPI ThreadProcExport(void *param);
	static BOOL CALLBACK Progress(void *param, int per ,int totalper);
	bool bUserBreak;
	bool m_bFinished;
	bool m_bComplete;

	CDataManager *m_pOtherData;
	char*        m_szExpPath;
// Dialog Data
	//{{AFX_DATA(CImportProgressDlg)
	enum { IDD = IDD_DIALOG_IMPORT_GAME_DATA };
	CProgressCtrl	m_ProgressTotal;
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportProgressDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK(){};
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HANDLE m_hThread;
	
	bool ImportGameData(void *param);
	bool ImportOthers(void *param);
	bool ExportGameData(void *param);
	bool m_bStart;
	
	// Generated message map functions
	//{{AFX_MSG(CImportProgressDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTPROGRESSDLG_H__E8DF88F5_E81D_412D_A35E_AD3BC0187A48__INCLUDED_)
