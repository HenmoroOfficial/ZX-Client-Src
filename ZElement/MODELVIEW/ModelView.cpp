// ModelView.cpp : Defines the class behaviors for the application.
//

#include "Global.h"
#include "ModelView.h"
#include "Render.h"
#include "a3d.h"
#include "af.h"
#include "A3DGFXExMan.h"
#include "MainFrm.h"
#include "..\ZElementData\DataPathMan.h"
#include "elementpckdir.h"
#include "MODELVIEWDataMan.h"
#include "MODELVIEWSkill.h"
#include "..\ZElementClient\EC_StringTab.h"
#include "A3DSkillGfxComposer.h"
#include "LuaBind.h"
#include "LuaState.h"
#include "EC_Model.h"

//Modified by MaYuanzheng 2010.8.18
#include "..\ZElementData\EC_MD5Hash.h"  //add a head file
//MaYuanzheng's Modify end

#define new A_DEBUG_NEW



/////////////////////////////////////////////////////////////////////////////
// CModelViewApp

BEGIN_MESSAGE_MAP(CModelViewApp, CWinApp)
	//{{AFX_MSG_MAP(CModelViewApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelViewApp construction

CModelViewApp::CModelViewApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CModelViewApp object

CModelViewApp theApp;

CECStringTab g_SkillActs;

void glb_ErrorOutput(int,char const *,int){}

void InitSkillGfxComposer()
{
	unsigned int idSkill = 0;
	// Init gfx composer man
	while (true)
	{
		idSkill = GNET::ElementSkill::NextSkill(idSkill);
		if (idSkill == 0)
			break;
		SKILL_ELEMENT temp;
		temp.id = idSkill;
		strcpy(temp.szName,WC2AS(GNET::ElementSkill::GetName(idSkill)));
		g_ListSkill.push_back(temp);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CModelViewApp initialization

//modified by MaYuanzheng 2010.8.17
// change the first two param's type from int to clientid_t
static void WINAPI show_damage(clientid_t nCaster, clientid_t idTarget, int nDamage, int nDivisions,DWORD dwModifier)
{

}
//MaYuanzheng's modify end

void AfxSkillGfxShowCaster(int idCaster, DWORD dwModifier)
{
}


BOOL CModelViewApp::InitInstance()
{
	AfxEnableControlContainer();
	
	LuaBind::Init();
	InitECMApi(g_LuaStateMan.GetConfigState());

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//	Get current directory
	char szAppPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szAppPath);
	strcat(szAppPath, "\\");
	strcpy(g_szIniFile, szAppPath);
	strcat(g_szIniFile, "ModelView.ini");
	strcpy(g_szWorkDir,szAppPath);
	g_Log.Init("ModelView.log","Model view log ...");
	//	Set work path
	af_Initialize();
	af_SetBaseDir(g_szWorkDir);
	
	char szBuffer[64];
	memset(szBuffer,0,64);
	GetPrivateProfileString("Options","path_list", szBuffer, szBuffer, 64, g_szIniFile);

	//modified by MaYuanzheng 2010.8.17
	//g_bUsedPath = (int)atof(szBuffer); //cancelled by MYZ
	//g_bUsedPath = ((int)atof(szBuffer))?true:false;//add by MYZ
	//MaYuanzheng's modify end

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	//Modified by MaYuanzheng 2010.8.18	
	g_MD5Hash.init();//add a init
	g_MD5Hash.Update("ZPWDATA", 7);// add a update
	
    // loading dataPath first instead of loading data first 
	if(0==g_DataPathMan.Load("data\\path.data"))
	{
		g_Log.Log("CModelViewApp::InitInstance(), Failed to load path.data");
		AfxMessageBox("CModelViewApp::InitInstance(), Failed to load path.data",MB_ICONSTOP);
		return false;
	}

	if(-1==g_DataMan.load_data("data\\elements.data"))
	{
		g_Log.Log("CModelViewApp::InitInstance(), Failed to load elements.data");
		AfxMessageBox("CModelViewApp::InitInstance(), Failed to load elements.data",MB_ICONSTOP);
		return false;
	}

	//MaYuanzheng's modify end


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

	

	// now open all file packages
	for(int i=0; i<sizeof(g_szPckDir) / sizeof(const char*); i++)
	{
		char szPckFile[MAX_PATH];
		sprintf(szPckFile, "%s.pck", g_szPckDir[i]);
		if( !g_AFilePackMan.OpenFilePackage(szPckFile) )
		{
			/*
			if( !strstr(GetCommandLineA(), "sepfile") )
			{
				char szInfo[1024];
				sprintf(szInfo, "文件包 [%s] 被损坏，无法打开，请重新安装游戏客户端！", szPckFile);
				MessageBoxA(NULL, szInfo, "文件包打开失败", MB_ICONSTOP | MB_OK);
				return false;
			}*/
		}
	}
	
		//	Create A3D engine after main frame shown, because before that render window's
	//	size is zero.
	if (!g_Render.Init(AfxGetInstanceHandle(), pFrame->GetChildView()->GetSafeHwnd()))
	{
		g_Log.Log("Failed to initlaize render");
		return FALSE;
	}
	
	if (!g_SkillActs.Init("Configs\\skillacts.txt", false))
		AfxMessageBox("无法打开skillacts.txt");
	
	InitSkillGfxComposer();
	AfxGetGFXExMan()->Init(g_Render.GetA3DDevice());
	//modified by MaYuanzheng 2010.8.17
	_SGC::AfxGetSkillGfxEventMan()->Init(g_Render.GetA3DDevice());//add a namespace
	_SGC::AfxGetSkillGfxEventMan()->SetDamageShowFunc(show_damage);//add a namespace
	//MaYuanzheng's modify end
	
	//	Initialize device object
	A3DDevice* pA3DDevice = g_Render.GetA3DDevice();
	pA3DDevice->SetFogEnable(false);
	pFrame->GetChildView()->Init();

	// 初始化技能表窗口
	pFrame->GetAnimationBar()->ClearSkill();
	pFrame->GetAnimationBar()->SetSkillDefaultSel();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CModelViewApp message handlers





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
void CModelViewApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CModelViewApp message handlers

int CModelViewApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	char szBuffer[64];
	sprintf(szBuffer,"%d",g_bUsedPath);
	WritePrivateProfileString("Options","path_list", szBuffer , g_szIniFile);
	
	g_DataPathMan.Release();
	g_Render.Release();
	g_Log.Release();
	af_Finalize();
	g_SkillActs.Release();

	LuaBind::Release();
	return CWinApp::ExitInstance();
}

BOOL CModelViewApp::OnIdle(LONG lCount) 
{
	CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;
	ASSERT(pMainFrame);

	if (!pMainFrame->AppIsActive())
		return CWinApp::OnIdle(lCount);
	
	if(m_pMainWnd) ((CMainFrame*)m_pMainWnd)->GetChildView()->Render();
	//	Let window update menu item states
	CWinApp::OnIdle(lCount);
	return TRUE;
}

BOOL CModelViewApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	/*
	if(pMsg->message == WM_MOUSEWHEEL)
	{
		int zDelta = 0;
		WORD d = HIWORD(pMsg->wParam);
		if(d & 0x8000)
			zDelta = -(d & (~0x8000))/240;
		else zDelta = d;

		((CMainFrame*)m_pMainWnd)->GetChildView()->MouseWheel(zDelta);
		return TRUE;
	}
	*/
	return CWinApp::PreTranslateMessage(pMsg);
}
