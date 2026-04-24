// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ElementLocalize.h"

#include "MainFrm.h"

extern CString g_strWorkingPath;

#include "ElementLocalizeDoc.h"
#include "ElementLocalizeView.h"
#include "ElementDataView.h"
#include "DataManager.h"
#include "FinderDlg.h"

#include "Excel.h"
#include "DlgSelectItems.h"
#include "DlgToxmlOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char          g_szWorkDir[];
extern char			 g_szExpDir[];
extern int           g_nTotalTextNum;
extern int           g_nNewTextNum;
extern int           g_nDisuseTextNum;
extern int           g_nTransTextNum;


bool g_bModified = false;
CFinderDlg g_FinderDlg;


const TCHAR *g_szHeader = _T("\
<?xml version=\"1.0\"?>\r\n\
<?mso-application progid=\"Excel.Sheet\"?>\r\n\
<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\r\n\
xmlns:o=\"urn:schemas-microsoft-com:office:office\"\r\n\
xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\r\n\
xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\r\n\
xmlns:html=\"http://www.w3.org/TR/REC-html40\">\r\n\
<ExcelWorkbook xmlns=\"urn:schemas-microsoft-com:office:excel\">\r\n\
<ProtectStructure>False</ProtectStructure>\r\n\
<ProtectWindows>False</ProtectWindows>\r\n\
</ExcelWorkbook>\r\n\
<Styles>\r\n\
<Style ss:ID=\"Default\" ss:Name=\"Normal\">\r\n\
</Style>\r\n\
<Style ss:ID=\"s21\" ss:Name=\"s21\">\r\n\
<Alignment ss:WrapText=\"1\"/>\r\n\
</Style>\r\n\
</Styles>\r\n\
");

const TCHAR *g_szTail = _T("\
</Workbook>\r\n\
");

const TCHAR *g_szSheetTail = _T("\
</Table>\r\n\
</Worksheet>\r\n\
"); 

const TCHAR *g_szSheetHeader = _T("\
<Worksheet ss:Name=\"Localize%d\">\r\n\
<Table ss:DefaultColumnWidth=\"400\">\r\n\
");

int   CALLBACK   BrowseCallbackProc(HWND   hwnd,   UINT   msg,   LPARAM   lp,   LPARAM   pData)   
{
	if (msg == BFFM_INITIALIZED)   
	{
		::SendMessage(hwnd,BFFM_SETSELECTIONA,TRUE,(LPARAM)g_szExpDir);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_IMPORT_GAME_DATA, OnImportGameData)
	ON_COMMAND(ID_DATA_OPEN, OnDataOpen)
	ON_COMMAND(ID_DATA_SAVE, OnDataSave)
	ON_COMMAND(ID_DATA_SAVE_CUSTOM, OnDataSaveCustom)
	ON_COMMAND(ID_DATA_SAVE_AS, OnDataSaveAs)
	ON_COMMAND(ID_IMPORT_OTHER, OnImportOther)
	ON_COMMAND(ID_NEXT, OnNext)
	ON_COMMAND(ID_PREV, OnPrev)
	ON_COMMAND(ID_FIND_NEW_ITEM, OnFindNewItem)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_COMMAND(ID_FIND_TEXT, OnFindText)
	ON_COMMAND(ID_FIND_DOWN, OnFindDown)
	ON_COMMAND(ID_FIND_UP, OnFindUp)
	ON_COMMAND(ID_EXPORT_GAME_DATA, OnExportGameData)
	ON_COMMAND(ID_GO_SEL, OnGoSel)
	ON_COMMAND(ID_FIND_NEW_ITEMEX, OnFindNewItemex)
	ON_COMMAND(ID_STAT_ITEMS, OnStatItems)
	ON_COMMAND(ID_HELP_EXT, OnHelpExt)
	ON_COMMAND(ID_SAVE_TEXT_TO_FILE, OnSaveTextToFile)
	ON_COMMAND(ID_IMPORT_FROM_TEXT_FILE, OnImportFromTextFile)
	ON_COMMAND(ID_IMPORT_ORDER_TEXT, OnImportOrderText)
	ON_COMMAND(ID_CHECK_SYNAC, OnCheckSynac)
	ON_COMMAND(ID_AUTO_TO_BIG5, OnAutoToBig5)
	ON_COMMAND(ID_COPY_ORIGIN_TO_TRANS, OnCopyOrg2Trans)
	ON_COMMAND(ID_CLEAR_TRANS, OnClearTrans)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_TOTAL,
	ID_INDICATOR_DISUSE,
	ID_INDICATOR_NEW,
	ID_INDICATOR_TRANS,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_dwBkTime = 120;//minutes
	m_bDeleted = false;
	m_pDlgSelectItems = 0;
}

CMainFrame::~CMainFrame()
{
	delete m_pDlgSelectItems;
	m_pDlgSelectItems = 0;
}

void CMainFrame::Replace()
{
	if(GetRightView())
		((CElementLocalizeView*)GetRightView())->Replace();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	SetTimer(101,1000*60,NULL);

	TCHAR szPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szPath);
	g_strWorkingPath = szPath;
	GetLocalTime(&m_LastBKTime);
	GetLocalTime(&m_LastWeekBKTime);
	
	LoadConfigs();
	m_pDlgSelectItems = new CDlgSelectItems(this);
	g_DataManager.SetDlgSelectItems(m_pDlgSelectItems);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~FWS_ADDTOTITLE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_wndSplitter.CreateStatic(this,1,2);
		
	RECT rc;
	GetClientRect(&rc);
	SIZE sizeInit;
	sizeInit.cx = (rc.right - rc.left)/3;
	sizeInit.cy = (rc.bottom - rc.top)/3;

	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CElementDataView), sizeInit,pContext);
	m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CElementLocalizeView), CSize(10, 10),pContext);	
	return TRUE;
}

CWnd *CMainFrame::GetLeftView()
{
	return m_wndSplitter.GetPane(0,0);
}
CWnd *CMainFrame::GetRightView()
{
	return m_wndSplitter.GetPane(0,1);
}

void CMainFrame::OnImportGameData() 
{
	m_pDlgSelectItems->EnableLabel(false);
	if(m_pDlgSelectItems->DoModal() != IDOK)
		return;
	m_wndImort.DoModal();
	//g_DataManager.TranslateToBig5();
	((CElementDataView*)GetLeftView())->UpdateTree();
	g_bModified = true;
	g_DataManager.Stat(g_nTotalTextNum,g_nDisuseTextNum,g_nNewTextNum,g_nTransTextNum);
	UpdateTextStatus();
}

void CMainFrame::OnDataOpen() 
{
	// TODO: Add your command handler code here
	TCHAR szFilter[] = _T("Element Localize Files (*.data)|*.data|Element Localize Backup Files (*.back)|*.back||");
	DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;
	CFileDialog	FileDia(TRUE, _T("*.data"), NULL, dwFlags, szFilter, NULL);

	if (FileDia.DoModal() == IDCANCEL)
		return;
	if(!g_DataManager.Load(FileDia.GetPathName()))
	{
		CString msg;
		msg.Format(_T("Faild to open file %s"),FileDia.GetPathName());
		AfxMessageBox(msg);
	}
	g_DataManager.Stat(g_nTotalTextNum,g_nDisuseTextNum,g_nNewTextNum,g_nTransTextNum);
	UpdateTextStatus();
	((CElementDataView*) GetLeftView())->UpdateTree();
	((CElementLocalizeView*) GetRightView())->Reset();
	g_bModified = true;
}

void CMainFrame::OnDataSave() 
{
	// TODO: Add your command handler code here
	if(!g_DataManager.Save(_T("Localize.data")))
		AfxMessageBox(_T("Faild to save file Localize.data"));
	g_bModified = false;
}

void CMainFrame::OnDataSaveCustom()
{
	m_pDlgSelectItems->EnableLabel(false);
	if (IDOK != m_pDlgSelectItems->DoModal())
		return;

	TCHAR szFilter[] = _T("Element Localize Files (*.data)|*.data||");
	DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;
	CFileDialog	FileDia(FALSE, _T("*.data"), NULL, dwFlags, szFilter, NULL);

	if (FileDia.DoModal() == IDCANCEL)
		return;
	
	if(!g_DataManager.Save(FileDia.GetPathName(), m_pDlgSelectItems))
		AfxMessageBox(_T("Faild to save file Localize.data"));
	g_bModified = false;
}

void CMainFrame::OnDataSaveAs() 
{
	// TODO: Add your command handler code here
	TCHAR szFilter[] = _T("Element Localize Files (*.data)|*.data||");
	DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;
	CFileDialog	FileDia(FALSE, _T("*.data"), NULL, dwFlags, szFilter, NULL);

	if (FileDia.DoModal() == IDCANCEL)
		return;
	
	g_DataManager.Save(FileDia.GetPathName());
}

void CMainFrame::OnImportOther() 
{
	// TODO: Add your command handler code here
	CString msg;
	TCHAR szFilter[] = _T("Element Localize Files (*.data)|*.data||");
	DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;
	CFileDialog	FileDia(TRUE, _T("*.data"), NULL, dwFlags, szFilter, NULL);

	if (FileDia.DoModal() == IDCANCEL)
		return;
	BeginWaitCursor();
	CDataManager dataMan;
	if(!dataMan.Load(FileDia.GetPathName()))
	{
		EndWaitCursor();
		msg.Format(_T("Faild to load file %s"),FileDia.GetPathName());
		AfxMessageBox(msg);	
		return;
	}
	EndWaitCursor();

	m_pDlgSelectItems->EnableLabel(false);
	if (IDOK != m_pDlgSelectItems->DoModal())
		return;

	CImportProgressDlg dlg;
	dlg.m_pOtherData = &dataMan;
	dlg.DoModal();
	((CElementDataView*) GetLeftView())->UpdateTree();
	g_bModified = true;
	
	g_DataManager.Stat(g_nTotalTextNum,g_nDisuseTextNum,g_nNewTextNum,g_nTransTextNum);
	UpdateTextStatus();
}

void CMainFrame::OnNext() 
{
	// TODO: Add your command handler code here
	BeginWaitCursor();
	((CElementDataView*) GetLeftView())->NextItem();
	EndWaitCursor();
}

void CMainFrame::OnPrev() 
{
	// TODO: Add your command handler code here
	BeginWaitCursor();
	((CElementDataView*) GetLeftView())->PrevItem();
	EndWaitCursor();
}

void CMainFrame::OnFindNewItem() 
{
	// TODO: Add your command handler code here
	BeginWaitCursor();
	bool r = ((CElementDataView*) GetLeftView())->FindNewItem();
	EndWaitCursor();
	if(!r) AfxMessageBox(_T("Cannot find a new item!"));

}


void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==101)
	{
		AutoBackupData();
	}
	CFrameWnd::OnTimer(nIDEvent);
}

INT64 CMainFrame::GetDiskFree()
{
	INT64 i64FreeBytesToCaller,i64TotalBytes,i64FreeBytes;
	CString str = g_strWorkingPath;;
    str = str.Left(2);
	HRESULT fResult = GetDiskFreeSpaceEx (str,
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);
	return i64FreeBytesToCaller;
}

void CMainFrame::AutoBackupData()
{
	if(IsBackup())
	{
		INT64 n64 = GetDiskFree();
		if(n64 < 1024*1024* 300)
			AfxMessageBox(_T("Warning: Not enough disk space for backup!"));

		CString strDest;
		SYSTEMTIME st;
		GetLocalTime(&st);
		CreateDirectory(_T("Backup"), NULL);
		strDest.Format(_T("Backup\\%d-%d-%d-%d-%d.back"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute);
		if(!g_DataManager.Save(strDest))
			AfxMessageBox(_T("Backup data faild!"));
		else m_LastBKTime = st;
	}

	if(IsWeekBackup())
	{
		INT64 n64 = GetDiskFree();
		if(n64 < 1024*1024* 300)
			AfxMessageBox(_T("Warning: Not enough disk space for backup!"));

		CString strDest;
		SYSTEMTIME st;
		GetLocalTime(&st);
		CreateDirectory(_T("Backup"), NULL);
		CreateDirectory(_T("Backup\\Week"), NULL);
		strDest.Format(_T("Backup\\Week\\%d-%d-%d-%d-%d.back"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute);
		if(!g_DataManager.Save(strDest))
			AfxMessageBox(_T("Backup data faild!"));
		else m_LastWeekBKTime = st;
	}

	if(!m_bDeleted)
	{
		AutoDeleteBackup();
		m_bDeleted = true;
	}
}

bool CMainFrame::IsBackup()
{
	SYSTEMTIME st;
	GetLocalTime(&st);	
	CTime t1(st);
	CTime t2(m_LastBKTime);
	CTimeSpan delta = t1 - t2;

	if((DWORD)delta.GetTotalMinutes() > m_dwBkTime) return true;
	return false;
}

bool CMainFrame::IsWeekBackup()
{
	SYSTEMTIME st;
	GetLocalTime(&st);	
	CTime t1(st);
	CTime t2(m_LastWeekBKTime);
	CTimeSpan delta = t1 - t2;

	if((DWORD)delta.GetDays() > 7) return true;
	return false;
}

bool  CMainFrame::AutoDeleteBackup()
{
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	CFileFind finder;
	CString fileName,ext;
	CString strWildcard = g_strWorkingPath;
	strWildcard +=_T("\\Backup\\*.*");
	BOOL bWorking = finder.FindFile(strWildcard);
	int n = 0;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		
		//是"."或是".." 跳过.
		if (finder.IsDots())
			continue;
		
		// 是文件夹，继续搜寻.
		if (finder.IsDirectory())
		{
			continue;
		}
		// 是文件.
		fileName = finder.GetFilePath();
		ext = finder.GetFileTitle();
		SYSTEMTIME temp;
		TCHAR szBuffer[128];
		_stprintf(szBuffer,ext);
		_stscanf(szBuffer,_T("%d-%d-%d-%d-%d"),&temp.wYear,&temp.wMonth,&temp.wDay,&temp.wHour,&temp.wMinute);
		CTime t1(curTime);
		CTime t2(temp);
		CTimeSpan delta = t1 - t2;
		if(delta.GetDays() > 7)
		{
			DeleteFile(fileName);
		}
		
	}
	finder.Close();
	return true;
}

void CMainFrame::SaveConfigs()
{
	TCHAR szIniPath[MAX_PATH];
	_stprintf(szIniPath, _T("%s\\Localize.ini"),g_strWorkingPath);
	TCHAR szStr[256], szSect[128];
	//	General section
	lstrcpy(szSect, _T("General"));
	_stprintf(szStr, _T("%u"), m_dwBkTime);
	WritePrivateProfileString(szSect, _T("bk_alternation_time"), szStr, szIniPath);
	_stprintf(szStr,_T("%d-%d-%d-%d-%d"),m_LastBKTime.wYear,m_LastBKTime.wMonth,m_LastBKTime.wDay,m_LastBKTime.wHour,m_LastBKTime.wMinute);
	WritePrivateProfileString(szSect, _T("bk_last_time"), szStr, szIniPath);
	_stprintf(szStr,_T("%d-%d-%d-%d-%d"),m_LastWeekBKTime.wYear,m_LastWeekBKTime.wMonth,m_LastWeekBKTime.wDay,m_LastWeekBKTime.wHour,m_LastWeekBKTime.wMinute);
	WritePrivateProfileString(szSect, _T("bk_last_week_time"), szStr, szIniPath);

	int nLen = 	MultiByteToWideChar(CP_ACP,0,g_szExpDir,-1,NULL,0);
	TCHAR* pszWExportDir = new TCHAR[nLen];
	MultiByteToWideChar(CP_ACP,0,g_szExpDir,-1,pszWExportDir,nLen);
	_stprintf(szStr, _T("%s"), pszWExportDir);
	WritePrivateProfileString(szSect, _T("Export_Dist_Dir"), szStr, szIniPath);
	delete pszWExportDir;
}

void CMainFrame::LoadConfigs()
{
	
	TCHAR szIniPath[MAX_PATH];
	_stprintf(szIniPath, _T("%s\\Localize.ini"),g_strWorkingPath);
	TCHAR szStr[256], szSect[128];
	//	General section
	lstrcpy(szSect, _T("General"));
	GetPrivateProfileString(szSect, _T("bk_alternation_time"), szStr, szStr, 256, szIniPath);
	m_dwBkTime = (DWORD)_ttoi(szStr);
	
	if(m_dwBkTime < 30)  m_dwBkTime = 30;

	GetPrivateProfileString(szSect, _T("bk_last_time"), szStr, szStr, 256, szIniPath);
	_stscanf(szStr,_T("%d-%d-%d-%d-%d"),&m_LastBKTime.wYear,&m_LastBKTime.wMonth,&m_LastBKTime.wDay,&m_LastBKTime.wHour,&m_LastBKTime.wMinute);
	GetPrivateProfileString(szSect, _T("bk_last_week_time"), szStr, szStr, 256, szIniPath);
	_stscanf(szStr,_T("%d-%d-%d-%d-%d"),&m_LastWeekBKTime.wYear,&m_LastWeekBKTime.wMonth,&m_LastWeekBKTime.wDay,&m_LastWeekBKTime.wHour,&m_LastWeekBKTime.wMinute);

	GetPrivateProfileString(szSect, _T("Export_Dist_Dir"), szStr, szStr, 256, szIniPath);
	int nLen = 	WideCharToMultiByte(CP_ACP,0,szStr,-1,NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,szStr,-1,g_szExpDir,nLen,NULL,NULL);
}


BOOL CMainFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	SaveConfigs();
	return CFrameWnd::DestroyWindow();
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(g_bModified)
	{
		int nRet = AfxMessageBox(_T("Save changes to disk ?"),MB_YESNOCANCEL|MB_ICONQUESTION);
		if( IDYES == nRet )
			OnDataSave(); 
		else if( IDCANCEL == nRet )
			return;
	}
	CFrameWnd::OnClose();
}

void CMainFrame::OnFindText() 
{
	// TODO: Add your command handler code here
	if(g_FinderDlg.DoModal()==IDOK)
	{
		CString msg;
		if((int)g_FinderDlg.m_nSelType<=eTransText)
			msg.Format(_T("Cannot find the string '%s'"),g_FinderDlg.m_strText);
		else
			msg.Format(_T("Cannot find the template by id(%s)"),g_FinderDlg.m_strText);
		
		if(g_FinderDlg.m_bUp)
		{
			BeginWaitCursor();
			bool r = ((CElementDataView*) GetLeftView())->FindUp();
			EndWaitCursor();
			if(!r) AfxMessageBox(msg);
			
		}else
		{
			BeginWaitCursor();
			bool r = ((CElementDataView*) GetLeftView())->FindDown();
			EndWaitCursor();
			if(!r) AfxMessageBox(msg);
		}		
	}
}

void CMainFrame::OnFindDown() 
{
	// TODO: Add your command handler code here
	if(g_FinderDlg.m_strText.IsEmpty()) return;
	
	CString msg;
	if((int)g_FinderDlg.m_nSelType<=eTransText)
		msg.Format(_T("Cannot find the string '%s'"),g_FinderDlg.m_strText);
	else
		msg.Format(_T("Cannot find the template by id(%s)"),g_FinderDlg.m_strText);
	
	BeginWaitCursor();
	bool r = ((CElementDataView*) GetLeftView())->FindDown(true);
	EndWaitCursor();
	if(!r) AfxMessageBox(msg);
}

void CMainFrame::OnFindUp()
{
	if(g_FinderDlg.m_strText.IsEmpty()) return;
	
	CString msg;
	if((int)g_FinderDlg.m_nSelType<=eTransText)
		msg.Format(_T("Cannot find the string '%s'"),g_FinderDlg.m_strText);
	else
		msg.Format(_T("Cannot find the template by id(%s)"),g_FinderDlg.m_strText);

	BeginWaitCursor();
	bool r = ((CElementDataView*) GetLeftView())->FindUp(true);
	EndWaitCursor();
	if(!r) AfxMessageBox(msg);	
}



void CMainFrame::OnExportGameData() 
{
	// TODO: Add your command handler code here
	BROWSEINFOA Info;
	memset(&Info, 0, sizeof (Info));

	static char szPath[MAX_PATH] = "\0";
	if (!szPath[0])
		strcpy(szPath, g_szWorkDir);

	Info.hwndOwner		= m_hWnd;
	Info.pidlRoot		= NULL;
	Info.pszDisplayName	= szPath;
	Info.lpszTitle		= "Export path select :";
	Info.lpfn			= BrowseCallbackProc;
	Info.ulFlags		= BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_DONTGOBELOWDOMAIN;	
	
	LPITEMIDLIST pPIDL;
	if (!(pPIDL = SHBrowseForFolderA(&Info)))
		return;

	SHGetPathFromIDListA(pPIDL, szPath);

	strcpy(g_szExpDir,szPath);

	CImportProgressDlg dlg;
	dlg.m_szExpPath = g_szExpDir;
	SetCurrentDirectoryA(g_szExpDir);
	if (!((CElementLocalizeApp*)AfxGetApp())->ClearPackage(g_szExpDir))
	{		
		OnExportGameData();
		return;
	}
	SaveConfigs();
	m_pDlgSelectItems->EnableLabel(false);
	if(m_pDlgSelectItems->DoModal() != IDOK)
		return;
	dlg.DoModal();
	SetCurrentDirectoryA(g_szWorkDir);
	((CElementLocalizeApp*)AfxGetApp())->OpenPackage(g_szWorkDir);
}

void CMainFrame::OnGoSel() 
{
	// TODO: Add your command handler code here
	((CElementDataView*) GetLeftView())->GoCurItem();
}

void CMainFrame::OnFindNewItemex() 
{
	// TODO: Add your command handler code here
	BeginWaitCursor();
	bool r = ((CElementDataView*) GetLeftView())->FindNewItemEx();
	EndWaitCursor();
	if(!r) AfxMessageBox(_T("Cannot find a new item!"));
}

void CMainFrame::SetTotalNum(int nTotal)
{
	CString str;
	str.Format(_T("Total text: %d"), nTotal);
	m_wndStatusBar.SetPaneText(1, str, TRUE);
}

void CMainFrame::SetDisuseNum( int nDisuse)
{
	CString str;
	str.Format(_T("Disuse text: %d"), nDisuse);
	m_wndStatusBar.SetPaneText(2, str, TRUE);
}


void CMainFrame::SetNewNum( int nNew)
{
	CString str;
	str.Format(_T("New text: %d"), nNew);
	m_wndStatusBar.SetPaneText(3, str, TRUE);
}

void CMainFrame::SetTransNum( int nTrans)
{
	CString str;
	str.Format(_T("Trans text: %d"), nTrans);
	m_wndStatusBar.SetPaneText(4, str, TRUE);
}

void CMainFrame::UpdateTextStatus()
{
	SetTotalNum(g_nTotalTextNum);
	SetDisuseNum(g_nDisuseTextNum);
	SetNewNum(g_nNewTextNum);
	SetTransNum(g_nTransTextNum);

}

void CMainFrame::OnStatItems() 
{
	// TODO: Add your command handler code here
	BeginWaitCursor();
	g_DataManager.Stat(g_nTotalTextNum,g_nDisuseTextNum,g_nNewTextNum,g_nTransTextNum);
	UpdateTextStatus();
	EndWaitCursor();
}

void CMainFrame::OnHelpExt() 
{
	// TODO: Add your command handler code here
/*	if(ERROR_FILE_NOT_FOUND==(int)ShellExecute(NULL,_T("open") ,_T("LocalizeHelp.chm"),NULL,NULL,SW_SHOWNORMAL ))
		MessageBox(_T("Cannot find LocalizeHelp.chm !"));*/

	CString strInfo;
	CString strCodeVersion, strDataVersion, strUpdateInfo, strToolUpdateInfo, strToolVersion;
	strToolVersion = _T("2013-03-27");
	strCodeVersion = _T("2013-03-11");
	strDataVersion = _T("2013-03-11");
	strUpdateInfo = _T("");
	strToolUpdateInfo = _T("版本更新");

	strInfo.Format(_T("工具版本:%s\r\n\r\n代码版本:%s\r\n\r\n数据版本:%s\r\n\r\n数据更新信息:\r\n%s\r\n\r\n工具更新信息:\r\n%s"), 
		strToolVersion, strCodeVersion, strDataVersion, strUpdateInfo, strToolUpdateInfo);
	MessageBox(strInfo, _T("工具版本信息"));
}

void CMainFrame::OnSaveTextToFile() 
{
	// TODO: Add your command handler code here
	TCHAR szFilter[] = _T("XML files (*.xml)|*.xml||");
	DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
	CFileDialog	FileDia(FALSE, _T("*.xml"), NULL, dwFlags, szFilter, NULL);

	if (FileDia.DoModal() == IDCANCEL)
		return;
	
	char szPath[MAX_PATH];

#ifdef _UNICODE
	TCHAR szOrg[MAX_PATH];
	wcscpy(szOrg,FileDia.GetPathName());
	BOOL b = true;
	int r1=WideCharToMultiByte(CP_ACP,0,szOrg,MAX_PATH,szPath,MAX_PATH,"",&b);
#else 
	strcpy(szPath,FileDia.GetPathName());
#endif
	m_pDlgSelectItems->EnableLabel(true);
	
	CDlgToxmlOption dlgOption;
	if (IDOK != dlgOption.DoModal())
		return;

	bool bOnlyExportUntrans = false;
	bool bIncludeID = false;
	dlgOption.GetOptionInfo(bOnlyExportUntrans, bIncludeID);

	if(m_pDlgSelectItems->DoModal() != IDOK)
		return;
	if(!g_DataManager.ExportAllText(szPath,bOnlyExportUntrans,bIncludeID))
		MessageBox(_T("Export failed!"));
	else
		MessageBox(_T("Export ok!"));
}

void CMainFrame::OnImportFromTextFile() 
{
	// TODO: Add your command handler code here
	TCHAR szFilter[] = _T("XML files (*.xml)|*.xml||");
	DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
	CFileDialog	FileDia(TRUE, _T("*.xml"), NULL, dwFlags, szFilter, NULL);

	bool bAccordID = false;
	if (IDOK == MessageBox(_T("Import By UniqueID?"), _T("Message"), MB_OKCANCEL))
		bAccordID = true;

	m_pDlgSelectItems->EnableLabel(false);
	if(m_pDlgSelectItems->DoModal() != IDOK)
		return;
	if (FileDia.DoModal() == IDCANCEL)
		return;
	
	char szPath[MAX_PATH];

#ifdef _UNICODE
	TCHAR szOrg[MAX_PATH];
	wcscpy(szOrg,FileDia.GetPathName());
	BOOL b = true;
	int r1=WideCharToMultiByte(CP_ACP,0,szOrg,MAX_PATH,szPath,MAX_PATH,"",&b);
#else 
	strcpy(szPath,FileDia.GetPathName());
#endif
	CString temp;
	GetWindowText(temp);
	if(!g_DataManager.ImportFromText(szPath,this, bAccordID))
		MessageBox(_T("Failed to import!"));
	SetWindowText(temp);
	UpdateTextStatus();
	((CElementDataView*) GetLeftView())->UpdateTree();

	g_bModified = true;
}

void CMainFrame::OnImportOrderText()
{
	// TODO: Add your command handler code here
	TCHAR szFilter[] = _T("XML files (*.xml)|*.xml||");
	DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
	CFileDialog	FileDia(TRUE, _T("*.xml"), NULL, dwFlags, szFilter, NULL);
	
	m_pDlgSelectItems->EnableLabel(false);
	if(m_pDlgSelectItems->DoModal() != IDOK)
		return;

	if (FileDia.DoModal() == IDCANCEL)
		return;
	
	char szPath[MAX_PATH];
#ifdef _UNICODE
	TCHAR szOrg[MAX_PATH];
	wcscpy(szOrg,FileDia.GetPathName());
	BOOL b = true;
	int r1=WideCharToMultiByte(CP_ACP,0,szOrg,MAX_PATH,szPath,MAX_PATH,"",&b);
#else 
	strcpy(szPath,FileDia.GetPathName());
#endif
	CString temp;
	GetWindowText(temp);
	if(!g_DataManager.ImportFromOrderText(szPath,this))
		MessageBox(_T("Failed to import!"));
	else
		MessageBox(_T("Import ok"));
	SetWindowText(temp);
	UpdateTextStatus();
	((CElementDataView*) GetLeftView())->UpdateTree();
	g_bModified = true;
}

void CMainFrame::OnCheckSynac() 
{
	// TODO: Add your command handler code here
	BeginWaitCursor();
	g_DataManager.TranslateToBig5();
	
	g_DataManager.Stat(g_nTotalTextNum,g_nDisuseTextNum,g_nNewTextNum,g_nTransTextNum);
	UpdateTextStatus();
	((CElementDataView*) GetLeftView())->UpdateTree();
	((CElementLocalizeView*) GetRightView())->Reset();
	
	EndWaitCursor();
}

void CMainFrame::OnAutoToBig5()
{
	g_DataManager.TranslateToBig5();
	((CElementDataView*) GetLeftView())->UpdateTree();
}

void CMainFrame::OnClearTrans()
{
	if (IDYES != AfxMessageBox(_T("Are you sure to clear all translations?"), MB_YESNO))
		return;

	g_DataManager.ClearAllTrans();
	((CElementDataView*) GetLeftView())->UpdateTree();
}

void CMainFrame::OnCopyOrg2Trans()
{
	if (IDYES != AfxMessageBox(_T("Are you sure to copy all origin text to translations?"), MB_YESNO))
		return;
	
	g_DataManager.CopyOrg2Trans();
	((CElementDataView*) GetLeftView())->UpdateTree();
}