// ImportProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementlocalize.h"
#include "ImportProgressDlg.h"
#include "DataManager.h"
#include "ElementDataView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportProgressDlg dialog


CImportProgressDlg::CImportProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportProgressDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pOtherData = NULL;
	m_szExpPath  = NULL;
}


void CImportProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportProgressDlg)
	DDX_Control(pDX, IDC_PROGRESS_TOTAL, m_ProgressTotal);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CImportProgressDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportProgressDlg message handlers

BOOL CImportProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_Progress.SetRange(0,100);
	m_Progress.SetPos(0);
	m_ProgressTotal.SetRange(0,100);
	m_ProgressTotal.SetPos(0);
	bUserBreak = false;
	m_bFinished = false;
	m_bStart = false;
	SetTimer(1,100,NULL);
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

DWORD CImportProgressDlg::ThreadProcImportGame(void *param)
{
	CImportProgressDlg *pDlg = (CImportProgressDlg *)param;
	if(pDlg->m_hWnd==NULL) return 0;
	pDlg->m_bComplete = pDlg->ImportGameData(param);
	pDlg->m_bFinished = true;
	return 0;
}


DWORD CImportProgressDlg::ThreadProcImport(void *param)
{
	CImportProgressDlg *pDlg = (CImportProgressDlg *)param;
	if(pDlg->m_hWnd==NULL) return 0;
	pDlg->m_bComplete = pDlg->ImportOthers(param);
	pDlg->m_bFinished = true;
	return 0;
}

DWORD CImportProgressDlg::ThreadProcExport(void *param)
{
	CImportProgressDlg *pDlg = (CImportProgressDlg *)param;
	if(pDlg->m_hWnd==NULL) return 0;
	pDlg->m_bComplete = pDlg->ExportGameData(param);
	pDlg->m_bFinished = true;
	return 0;
}

bool CImportProgressDlg::ImportGameData(void *param)
{
	return g_DataManager.ImportGameData(param, Progress);
}

bool CImportProgressDlg::ImportOthers(void *param)
{
	if(m_pOtherData==NULL) return false;
	return g_DataManager.Import(m_pOtherData, param, Progress);
}
bool CImportProgressDlg::ExportGameData(void *param)
{
	if(m_szExpPath==NULL) return false;
	return g_DataManager.Export(m_szExpPath, param, Progress);
}

void  CImportProgressDlg::OnCancel()
{
	if(IDYES==AfxMessageBox(_T("Do you want to terminate import process ? "),MB_YESNO|MB_ICONQUESTION))
		bUserBreak = true;
}

BOOL CALLBACK CImportProgressDlg::Progress(void *param, int per ,int totalper)
{
	CImportProgressDlg * pDlg = (CImportProgressDlg *)param;
	pDlg->m_Progress.SetPos(per);
	pDlg->m_ProgressTotal.SetPos(totalper);
	if(pDlg->bUserBreak) return FALSE;
	return TRUE;
}


void CImportProgressDlg::StartProcessData()
{
	DWORD dwThreadID;
	
	if(m_pOtherData!=NULL)
	{
		//因为在辅助线程中缺乏对消息的响应机制
		//所以不能用模式对话框来显示对话框
		//如果要用的话就必须用AfxBeginThread
		//否则运行必然报错
		AfxBeginThread((AFX_THREADPROC)ThreadProcImport,this);
		/*
		m_hThread = CreateThread(
		NULL,
		0,
		ThreadProcImport,
		this,
		0,
		&dwThreadID);*/
	}else if( m_szExpPath != NULL)
	{
		m_hThread = CreateThread(
		NULL,
		0,
		ThreadProcExport,
		this,
		0,
		&dwThreadID);
	}else
	{
		m_hThread = CreateThread(
		NULL,
		0,
		ThreadProcImportGame,
		this,
		0,
		&dwThreadID);
	}

}

BOOL CImportProgressDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::DestroyWindow();
}



void CImportProgressDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1) 
	{
		if(m_bFinished)
		{
			KillTimer(nIDEvent);
			if(!m_bComplete) MessageBox(_T("Faild to process game data !"));
			CDialog::OnOK();
		}
		if(!m_bStart) 
		{
			StartProcessData();
			m_bStart = true;
		}
	}
	CDialog::OnTimer(nIDEvent);
}
