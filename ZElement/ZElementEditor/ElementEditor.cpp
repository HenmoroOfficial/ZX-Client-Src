  // ElementEditor.cpp : Defines the class behaviors for the application.
//

#include "Global.h"
#include "ElementEditor.h"
#include "MainFrm.h"
#include "ElementMap.h"
#include "ViewFrame.h"
#include "Render.h"
#include "Bitmap.h"
#include "UndoMan.h"
#include "ModelContainer.h"
#include "PropertyBox.h"
#include "MonsterFaction.h"
#include "VssOperation.h"
#include "elementdataman.h"
#include "DynamicObjectManDlg.h"
#include "globaldataman.h"
#include "OperationManager.h"
#include "NpcControllerManDlg.h"
#include "VssOptionDlg.h"
#include "EL_Region.h"


#include "AF.h"
#include "A3D.h"
#include "A3DGFXExMan.h"
#include "EL_BackMusic.h"
#include "EC_MD5Hash.h"

#include "LuaBind.h"
#include "EC_Model.h"
#include "LuaState.h"
#include "EC_ModelMan.h"
#include "MiniDump.h"


const char* _settings	= "Settings";
const char* _database	= "VSS_EDZ_DATABASE";
const char* _user_name	= "VSS_EDZ_USERNAME";
const char* _password	= "VSS_EDZ_PASSWORD_BIN";

bool g_bConfigInit = false;

void Encrypt(char *strPass)
{
	int len = strlen(strPass);
	int pass = 2005;
	for(int i = 0; i< len; i++)
	{
		strPass[i] ^= pass;
	}
}

void LoadConfigs()
{
	struct abase::_hash_function	strHash;
	DWORD dwWorkID = (DWORD)strHash(g_szWorkDir);

	char	regPath[256];
	sprintf(regPath, "%s\\%ud", _settings, dwWorkID);

	g_bConfigInit = true;
	strcpy(g_Configs.szServerPath, AfxGetApp()->GetProfileString(regPath, _database, ""));
	if( strlen(g_Configs.szServerPath) == 0 )
		g_bConfigInit = false;

	strcpy(g_Configs.szUserName, AfxGetApp()->GetProfileString(regPath, _user_name, ""));
	if( strlen(g_Configs.szUserName) == 0 )
		g_bConfigInit = false;

	memset(g_Configs.szUserPassword, 0, sizeof(g_Configs.szUserPassword));
	UINT n;
	BYTE* p;
	AfxGetApp()->GetProfileBinary(regPath, _password, &p, &n);
	if (p && n) strncpy(g_Configs.szUserPassword, (const char*)p, n);
	Encrypt(g_Configs.szUserPassword);
	if( strlen(g_Configs.szUserPassword) == 0 )
		g_bConfigInit = false;

}

void SaveConfigs()
{
	struct abase::_hash_function	strHash;
	DWORD dwWorkID = (DWORD) strHash(g_szWorkDir);

	char	regPath[256];
	sprintf(regPath, "%s\\%ud", _settings, dwWorkID);

	AfxGetApp()->WriteProfileString(regPath, _database, g_Configs.szServerPath);
	AfxGetApp()->WriteProfileString(regPath, _user_name,g_Configs.szUserName);
	char b[MAX_PATH];
	strcpy(b, g_Configs.szUserPassword);
	int nLen = strlen(g_Configs.szUserPassword);
	Encrypt(b);
	AfxGetApp()->WriteProfileBinary(regPath, _password, (unsigned char*)b, nLen);
}

//#define new A_DEBUG_NEW
#define MAX_LINE_LEN	1024

extern abase::vector<AString> _monster_faction;

inline bool ReadLine(FILE* fp, char* szLine)
{
	if (fgets(szLine, MAX_LINE_LEN, fp) == NULL) return false;
	szLine[strcspn(szLine, "\r\n")] = '\0';
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
	CString	m_strHistory;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strHistory = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_UPDATE_HISTORY, m_strHistory);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CElementEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// void CElementEditorApp::OnAppChangeVersion()
// {
// 	if(g_Configs.iIsOverseaVersion == 0)
// 	{
// 		g_Configs.iIsOverseaVersion = 1;
// 		AfxMessageBox("当前版本已更改为【海外版】!");
// 	}
// 	else //if(g_Configs.iIsOverseaVersion == 1)
// 	{
// 		g_Configs.iIsOverseaVersion = 0;
// 		AfxMessageBox("当前版本已更改为【国内版】!");
// 	}
// 
// 	// update windows title
// 	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
// 	pMainFrame->UpdateMainFrameTitle();
// }

/////////////////////////////////////////////////////////////////////////////
// CElementEditorApp

BEGIN_MESSAGE_MAP(CElementEditorApp, CWinApp)
	//{{AFX_MSG_MAP(CElementEditorApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
//	ON_COMMAND(ID_SET_VERSION, OnAppChangeVersion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElementEditorApp construction

CElementEditorApp::CElementEditorApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CElementEditorApp object

CElementEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CElementEditorApp initialization

VOID Aux_DumpWorkFunc()
{
	CElementMap* pMap = AUX_GetMainFrame()->GetMap();
	if(pMap && (!g_bReadOnly))
	{
		if(IDYES==AfxMessageBox("发生致命错误，程序将立即退出，你需要尝试保存当前正编辑的地图吗？\n 请把生成的dmp文件发送给相关人员以方便查找问题原因！",MB_ICONSTOP | MB_YESNO))
		{
			
			// Backup old map files
			CString strMapName = pMap->GetMapName();
			
			struct tm *tm_;
			time_t  time_;
			time(&time_);
			tm_= localtime(&time_);
			
			char Date[64];
			wsprintf(Date,"DUMPBK_%d_%d_%d_%d_%d",tm_->tm_year + 1900,tm_->tm_mon + 1 ,tm_->tm_mday,tm_->tm_hour,tm_->tm_min);
			CString strDst,strSrc;
			strDst.Format("Editmaps\\%s\\%s",g_Configs.szCurProjectName,Date);
			strSrc.Format("Editmaps\\%s\\%s",g_Configs.szCurProjectName,strMapName);
			
			AUX_CopyFolder(strDst,strSrc,false);
			
			// Save new map
			CString strFile = g_szWorkDir;
			strFile += pMap->GetMapFile();
			pMap->Save(strFile);
		}
	}else
	{
		AfxMessageBox("发生致命错误，程序将立即退出，请把生成的dmp文件发送给相关人员以方便查找问题原因！",MB_ICONSTOP | MB_OK);
	}
};

BOOL CElementEditorApp::InitInstance()
{
	AfxEnableControlContainer();
	__fnDumpWork = Aux_DumpWorkFunc;
	CMiniDump::Begin();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	if (!InitApp())
		return FALSE;

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("ElementEditor"));
	
	//  Load vss user information
	LoadConfigs();

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	// The one and only window has been initialized, so show and update it.
	pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pFrame->UpdateWindow();
	
	if(!InitVss()) return FALSE;
	//	Create A3D engine after main frame shown, because before that render window's
	//	size is zero.
	if (!g_Render.Init(m_hInstance, pFrame->GetViewFrame()->GetRenderWnd()->GetSafeHwnd()))
	{
		g_Log.Log("Failed to initlaize render");
		return FALSE;
	}
	
	//	Initialize device object
	if (!pFrame->InitDeviceObjects())
	{
		g_Log.Log("Failed to initlaize device objects");
		return FALSE;
	}
	
	AfxGetGFXExMan()->Init(g_Render.GetA3DDevice());
	if(!g_BackMusic.Init(g_Render.GetA3DEngine()->GetAMSoundEngine()))
	{
		g_Log.Log("Failed to initlaize back music");
		return false;
	}
	g_BackMusic.SetSFXVolume(g_Configs.iSfxVolume);
	g_BackMusic.SetVolume(g_Configs.iMusicVolume);
	//	Initlaize undo manager
	if (!g_UndoMan.Init())
	{
		g_Log.Log("Failed to initlaize undo manager");
		return FALSE;
	}

	//CPropertyBox 不得不在这儿初始化它
	CPropertyBox::g_pA3DEngine = g_Render.GetA3DEngine();
	//加载鼠标指针
	AUX_LoadCursor();
	
	g_MD5Hash.init();
	g_MD5Hash.Update("ZPWDATA", 7);

	AUX_LoadDataPath();
	
	//Load tempalte data
	AUX_LoadTemplateData();
	
	//Load Domain data
	if(!globaldata_load())
	{
		AfxMessageBox("Failed to globaldata_load()");
		g_Log.Log("Failed to globaldata_load()");
	}

	//Load Dynamic object
	if(!g_DynamicObjectMan.Load())
	{
		AfxMessageBox("Failed to load dynamic object data!");
		g_Log.Log("CElementEditorApp::InitInstance(): g_DynamicObjectMan.Load() failed!");
	}


	//Load Controller object
	if(!g_ControllerObjectMan.Load())
	{
		AfxMessageBox("Failed to load controller object data!");
		g_Log.Log("CElementEditorApp::InitInstance(): g_ControllerObjectMan.Load() failed!");
	}
	
	//Load mounster faction
	FILE* fp = fopen("BaseData\\monster_faction.dat", "rb");
	if (fp)
	{
		_monster_faction.clear();
		char line[MAX_LINE_LEN];
		while (ReadLine(fp, line) && strlen(line) != 0)
			_monster_faction.push_back(AString(line));
		fclose(fp);
	}else
	{
		AfxMessageBox("Open BaseData\\monster_faction.dat file failed!");
		g_Log.Log("Open BaseData\\monster_faction.dat file failed!");
	}
	
	//  Get project'name for server
	if(!GetProjectName())
		return FALSE;

	pFrame->UpdateMainFrameTitle();

	g_VSS.SetWorkFolder("ResTable");
	g_VSS.GetFile("ResTable\\UpdateHistory.txt");
	OnAppAbout();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CElementEditorApp message handlers

int CElementEditorApp::ExitInstance() 
{
	SaveConfigs();
	AfxGetGFXExMan()->Release();
	g_OperationContainer.Release();
	g_BackMusic.Release();
	_monster_faction.clear();
	g_UndoMan.Release();
	g_Render.Release();
	g_Log.Release();
	g_LogVss.Release();
	AfxGetECModelMan()->Release();
	LuaBind::Release();
	af_Finalize();

	CMiniDump::End();

	return CWinApp::ExitInstance();
}

//#include "EL_Region.h"

//	Do some initial work
bool CElementEditorApp::InitApp()
{
	//	Get current directory
	char szAppPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szAppPath);
	strcat(szAppPath, "\\");

	strcpy(g_szIniFile, szAppPath);
	strcat(g_szIniFile, "ElementEditor.ini");
	strcpy(g_szDefTexFile, "Temp\\notex.bmp");

	//	Get work path
	GetPrivateProfileString("Path", "WorkPath", szAppPath, g_szWorkDir, MAX_PATH, g_szIniFile);

	//	Set work path
	af_Initialize();
	af_SetBaseDir(g_szWorkDir);

	//	Test code
	CELRegionSet r;
	char sz[MAX_PATH];
	sprintf(sz, "%s%s", g_szWorkDir, "\\region.sev");
	r.Load(sz);

	//	Open log file
	g_Log.Init("ElementEditor.log", "Element editor log ...");
	g_LogVss.Init("Vss.log","Element vss log ...");

	/* check if WinTab available. */
	if (!WTInfo(0, 0, NULL)) 
	{
		g_Log.Log("--WinTab Services Not Available.");
		g_bWTPacket = false;
	}

	//	Create temporary directory
	sprintf(szAppPath, "%sTemp", g_szWorkDir);
	AUX_CreateDirectory(szAppPath);
	sprintf(szAppPath, "%s%s\\%s", g_szWorkDir, g_szEditMapDir, g_szTempMap);
	AUX_CreateDirectory(szAppPath);

	//	Create default texture
	if (!CreateDefaultTexture())
		return false;

	//	Set default configs
	AUX_DefaultConfigs();
	//	Load configs from .ini file
	AUX_LoadConfigs();

	sprintf(g_szEditMapDir, "EditMaps\\%s",g_Configs.szCurProjectName);
	sprintf(g_szTempMap, "EditorTempMap");
	
	//	Initialize random number generator
	srand((unsigned)time(NULL));

	LuaBind::Init();
	InitECMApi(g_LuaStateMan.GetConfigState());

	return true;
}

bool CElementEditorApp::InitVss()
{
	while(1)
	{
		if (!SetEnvironmentVariable("SSDIR", g_Configs.szServerPath)
			||	!SetEnvironmentVariable("SSUSER", g_Configs.szUserName)
			|| !SetEnvironmentVariable("SSPWD", g_Configs.szUserPassword))
			return false;
		
		g_VSS.InitVss(AUX_VssLog);
		g_VSS.SetUserName(g_Configs.szUserName);
		g_VSS.SetPassword(g_Configs.szUserPassword);
		g_VSS.SetServerPath(g_Configs.szServerPath);
		
		CString _fileType = g_VSS.GetFileTypes("s_e_project.ini");
		if(!g_VSS.TestVssConnected(_fileType))
		{
			AfxMessageBox("No VSS database!");
			CVssOptionDlg dlg;
			if(IDOK!=dlg.DoModal()) return FALSE;
		}else
		{
			//user name or password is not right.
			if( _fileType.Find(_T("Username"))>=0 && _fileType.Find(_T("Password"))>=0 )
			{
				AfxMessageBox("User name or Password is wrong!");
				CVssOptionDlg dlg;
				if(IDOK!=dlg.DoModal()) return FALSE;	
			}else return true;
		}
	}
	
	return true;
}

//	Create a default texture
bool CElementEditorApp::CreateDefaultTexture()
{
	char szFile[MAX_PATH];
	sprintf(szFile, "%s%s", g_szWorkDir, g_szDefTexFile);

	if (AUX_FileExist(szFile))
		return true;
	
	CELBitmap Bmp;
	if (!Bmp.CreateBitmap(128, 128, 32, NULL))
	{
		g_Log.Log("CElementEditorApp::CreateDefaultTexture, Failed to create bitmap");
		return false;
	}

	//	Fill bitmap with some color
	DWORD dwCol = RGB(0, 192, 0) | 0xff000000;
	BYTE* pLineData = Bmp.GetBitmapData();

	for (int i=0; i < Bmp.GetHeight(); i++)
	{
		DWORD* pPixel = (DWORD*)pLineData;
		pLineData += Bmp.GetPitch();

		for (int j=0; j < Bmp.GetWidth(); j++)
			*pPixel++ = dwCol;
	}

	if (!Bmp.SaveToFile(szFile))
	{
		g_Log.Log("CElementEditorApp::CreateDefaultTexture, Failed to save bitmap");
		return false;
	}

	return true;
}

BOOL CElementEditorApp::OnIdle(LONG lCount) 
{
	CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;
	ASSERT(pMainFrame);

	if (!pMainFrame->AppIsActive())
		return CWinApp::OnIdle(lCount);

	pMainFrame->Tick();

	//	Let window update menu item states
	CWinApp::OnIdle(lCount);

	return TRUE;
}

void CElementEditorApp::OnAppExit() 
{
	CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;
	if (!pMainFrame->OnAppExit())
		return;
	CWinApp::OnAppExit();
}

//  Get project's name from server
bool CElementEditorApp::GetProjectName()
{
	// Get projlist 
	CString str;
	strcpy(g_szProject,"");
	str.Format("%s",g_szWorkDir);
	g_VSS.SetProjectPath("");
	g_VSS.SetWorkFolder(str);

	char szPath[256];
	sprintf(szPath,"%ss_e_project.ini",g_szWorkDir);

	// set file read only
	bool bFileReadOnly = AUX_FileIsReadOnly(szPath);
	if(!bFileReadOnly) 
	{
		DWORD flag = GetFileAttributes(szPath);
		flag ^= FILE_ATTRIBUTE_READONLY;
		if(!SetFileAttributes(szPath,flag))
		{
			g_Log.Log("Faild to set file attributes (%s)",szPath);
			return false;
		}
	}
	
	if(!g_VSS.GetFile("s_e_project.ini"))
	{
		CString strMsg;
		strMsg.Format("编辑器可能还没有连接到服务器上或是服务器上面没有s_e_project.ini文件!\r'\n%s", g_VSS.GetLastCommandResult());
		AfxMessageBox(strMsg);
		return false;
	}
	
	GetPrivateProfileString("Editor Project", "ProjectName", g_szProject, g_szProject, 256, szPath);
	CString msg(g_szProject);
	if(msg.IsEmpty())
	{
		AfxMessageBox("警告: 编辑器没项目配置文件或是项目名为空!");
		return false;
	}
	
	char	szVersion[32];
	szVersion[0] = '\0';
	GetPrivateProfileString("Editor Project", "Version", szVersion, szVersion, 32, szPath);
	if( stricmp(g_szVersion, szVersion) != 0 )
	{
		AfxMessageBox("错误: ElementEditor.exe 不是当前最新版本，请更新后再进行使用!");
		return false;
	}

	// oversea version
	char	szOversea[32];
	szOversea[0] = '\0';
	GetPrivateProfileString("Editor Project", "Overseas", szOversea, szOversea, 32, szPath);
	g_Configs.iIsOverseaVersion = (int)atoi(szOversea);
	
	return true;
}




BOOL CElementEditorApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_HOME)
		{
			g_Configs.fCameraMove += 3.0f;
		}else if( pMsg->wParam == VK_END)
		{
			g_Configs.fCameraMove -= 3.0f;
			if(g_Configs.fCameraMove < 3.0f) g_Configs.fCameraMove = 3.0f;
		}
	}
	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strVersion;

	strVersion.Format("当前版本: %s %s\r\n", g_szVersion, g_szCompileTime);
	m_strHistory += strVersion;
	m_strHistory += "\r\n";

	AFile file;
	if (!file.Open("ResTable\\UpdateHistory.txt", AFILE_OPENEXIST | AFILE_TEXT))
		return FALSE;

	char	szLine[1024];
	DWORD	dwRead;
	DWORD	dwVersion = 0;

	while(file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead))
	{
		m_strHistory += szLine;
		m_strHistory += "\r\n";
	}

	UpdateData(FALSE);

	file.Close();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
