// ElementLocalize.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ElementLocalize.h"

#include "MainFrm.h"
#include "ElementLocalizeDoc.h"
#include "ElementLocalizeView.h"
#include "DataManager.h"
#include "ElementDataView.h"
#include "elementpckdir.h"
#include "LuaBind.h"
#include "VssOperation.h"
#include <af.h>
#include "AFilePackMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ALog		  g_Log;
CVssOperation g_VSS;
bool          g_bLinkVss = true;
char          g_szWorkDir[MAX_PATH];
char          g_szExpDir[MAX_PATH];

int           g_nTotalTextNum = 0;
int           g_nNewTextNum = 0;
int           g_nDisuseTextNum = 0;
int           g_nTransTextNum = 0;

#define PCK_NUM 3
const char* szFilter[] = 
{
	"configs.pck",
	"interfaces.pck",
	"help\\script.pck"
};

/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeApp

BEGIN_MESSAGE_MAP(CElementLocalizeApp, CWinApp)
	//{{AFX_MSG_MAP(CElementLocalizeApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeApp construction

CElementLocalizeApp::CElementLocalizeApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CElementLocalizeApp object

CElementLocalizeApp theApp;

CString     g_strWorkingPath;

/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeApp initialization

BOOL CElementLocalizeApp::InitInstance()
{
	AfxEnableControlContainer();
	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CElementLocalizeDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CElementLocalizeView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	LuaBind::Init();
	//	Set work path
	af_Initialize();
	GetCurrentDirectoryA(MAX_PATH, g_szWorkDir);
	if(!OpenPackage(g_szWorkDir)) return FALSE;

	g_Log.Init("ElementLocalize.log", "Log for element localize ...");
	g_DataManager.InitElementDataTypes();
	g_DataManager.InitHomeTownDataTypes();
	g_DataManager.Load(_T("Localize.data"));
	if(g_DataManager.GetVersion() < 0x10000002) 
		AfxMessageBox(_T("Please import game data at once for update localize!!"));
	g_DataManager.Stat(g_nTotalTextNum,g_nDisuseTextNum,g_nNewTextNum,g_nTransTextNum);
	((CMainFrame*)m_pMainWnd)->UpdateTextStatus();
	((CElementDataView*) ((CMainFrame*)m_pMainWnd)->GetLeftView())->UpdateTree();
	
	return TRUE;
}

bool CElementLocalizeApp::OpenPackage(char *szWorkDir)
{
	g_AFilePackMan.CloseAllPackages();
	af_SetBaseDir(szWorkDir);
	// now open all file packages
	for(int i=0; i<sizeof(g_szPckDir) / sizeof(const char*); i++)
	{
		bool bFound = false;
		char szPckFile[MAX_PATH];
		sprintf(szPckFile, "%s.pck", g_szPckDir[i]);
		for( int i = 0; i < PCK_NUM; ++i)
		{
			if(stricmp(szFilter[i],szPckFile)==0)
			{
				bFound = true;
				break;
			}
		}
		if(bFound)
		{
			if( !g_AFilePackMan.OpenFilePackage(szPckFile) )
			{
				//char szInfo[1024];
				//sprintf(szInfo, "Pakage [%s] is broken !", szPckFile);
				//MessageBoxA(NULL, szInfo,NULL, MB_ICONSTOP | MB_OK);
				//return false;
			}
		}
	}
	return true;
}

bool CElementLocalizeApp::ClearPackage(char *szWorkDir)
{
	g_AFilePackMan.CloseAllPackages();
	af_SetBaseDir(szWorkDir);
	// now open all file packages
	for(int i=0; i<sizeof(g_szPckDir) / sizeof(const char*); i++)
	{
		bool bFound = false;
		char szPckFile[MAX_PATH];
		sprintf(szPckFile, "%s.pck", g_szPckDir[i]);
		for( int i = 0; i < PCK_NUM; ++i)
		{
			if(stricmp(szFilter[i],szPckFile)==0)
			{
				bFound = true;
				break;
			}
		}
		if(bFound)
		{
			CleanUpPckRedundancy(szPckFile);
		}
	}
	OpenPackage(szWorkDir);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CElementLocalizeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeApp message handlers


int CElementLocalizeApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	LuaBind::Release();
	g_Log.Release();
	af_Finalize();
	g_DataManager.DestoryHomeTownDataTypes();
	g_DataManager.DestoryElementDataTypes();
	return CWinApp::ExitInstance();
}

BOOL CElementLocalizeApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//如果CTRL按下，就不清除原列表
	SHORT s = GetKeyState(VK_LCONTROL);
	if(s < 0)
	{
		if(pMsg->message == WM_KEYDOWN)
		{
			if(pMsg->wParam == VK_RETURN)
			{
				((CMainFrame*)AfxGetMainWnd())->Replace();
				return TRUE;
			}
		}
	}
	return CWinApp::PreTranslateMessage(pMsg);
}
