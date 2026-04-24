// AnimationEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <afxadv.h>
#include "AM.h"
#include "AnimationEditor.h"
#include "EncStr.h"
#include "EC_Global.h"
#include "EC_Configs.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_SceneLoader.h"
#include "LuaBind.h"
#include "AnmSystem.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmAxes.h"
#include "AnmCommonActionManager.h"
#include "MiniDump.h"

#include <list>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////
ACString text, caption, hardError, missFile;


///////////////////////////////////////////////////////////////////////////
//
//	Reference to External variables and functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Local Types and Variables and Global variables
//
///////////////////////////////////////////////////////////////////////////

//	Parameters can be provided using program command line
struct s_CMDLINEPARAMS
{
	int		iFullScreen;	//	1: fullscreen; 0: windowed
	int		iRenderWid;		//	Render size
	int		iRenderHei;
	ACHAR	szIPAddr[256];	//	Link server IP address
	int		iPort;			//	Link server port
};
static s_CMDLINEPARAMS l_CmdParams;

static HINSTANCE l_hInstance = NULL;
static HWND	     l_hMainWnd  = NULL;
static const AWString l_szClassName = _EW("ZElementClient Window");

static bool l_bHasAntiHackerMsg = false;
ACString	l_AntiHackerMsg;
BOOL		g_bHookExitProcess = TRUE;

CECConfigs g_GameCfgs;

HANDLE g_hMainThread					= NULL;
HANDLE g_hRenderThread					= NULL;
HANDLE g_hExitGame						= NULL;
HANDLE g_hRenderEvent					= NULL;
HANDLE g_hToSuspendMain					= NULL;
DWORD g_dwMainThreadId					= 0;
volatile bool g_bMainSuspened			= false;
volatile bool g_bRenderSuspended		= true;
volatile bool g_bMultiThreadRenderMode	= false;
bool g_bIsWin98							= false;

CRITICAL_SECTION g_csRenderThread;
//D3DCAPS9		g_d3dcaps;

static DWORD WINAPI RenderThreadFunc(LPVOID lpParam)
{
    int iRet = -1;
    
    iRet = g_pGame->RunInRenderThread();
    
    g_hRenderThread = NULL;
    ::ResumeThread(g_hMainThread);
    return iRet;
}

inline void SuspendRenderThread()
{
    if (!g_hRenderThread)
        return;
    
    if (!g_bRenderSuspended)
    {
        ::EnterCriticalSection(&g_csRenderThread);
        ::SuspendThread(g_hRenderThread);
        g_bRenderSuspended = true;
        ::LeaveCriticalSection(&g_csRenderThread);
    }
}

inline void ResumeRenderThread()
{
    if (!g_hRenderThread || !g_bMultiThreadRenderMode)
        return;
    
    if (g_bRenderSuspended)
    {
        g_bRenderSuspended = false;
        ::ResumeThread(g_hRenderThread);
    }
}

void SwitchRenderThreadMode(bool bMultiThread)
{
    if (g_bIsWin98)
        return;
    
    g_bMultiThreadRenderMode = bMultiThread;
}

/////////////////////////////////////////////////////////////////////////////
// CAnimationEditorApp

BEGIN_MESSAGE_MAP(CAnimationEditorApp, CWinApp)
    //{{AFX_MSG_MAP(CAnimationEditorApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    ON_COMMAND(ID_FILE_NEW, OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
    ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_ANIMATION_PLAY, OnAnimationPlay)
	ON_COMMAND(ID_ANIMATION_RESET, OnAnimationReset)
	ON_UPDATE_COMMAND_UI(ID_ANIMATION_PLAY, OnUpdateAnimationPlay)
	ON_COMMAND(ID_ANIMATION_STOP, OnAnimationStop)
	ON_UPDATE_COMMAND_UI(ID_ANIMATION_STOP, OnUpdateAnimationStop)
	ON_COMMAND(ID_ANIMATION_PAUSE, OnAnimationPause)
	ON_UPDATE_COMMAND_UI(ID_ANIMATION_PAUSE, OnUpdateAnimationPause)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
   // ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateRecentFileMenu)
   // ON_COMMAND_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, OnOpenRecentFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimationEditorApp construction

CAnimationEditorApp::CAnimationEditorApp()
: m_bAppActive( FALSE )
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
    m_bKeyPressed = false;
    m_bChanged = false;
    m_sFileName = "";
    m_pMainWnd = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAnimationEditorApp object

CAnimationEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAnimationEditorApp initialization

BOOL CAnimationEditorApp::InitInstance()
{
//	_CrtSetBreakAlloc(56);

    AfxEnableControlContainer();
    
    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.
    
#ifdef _AFXDLL
    //Enable3dControls();			// Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
    // Change the registry key under which our settings are stored.
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization.
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	
	CMiniDump::Begin();

    // Init Game
    g_pGame = new CECGame;
    if (!g_pGame)
    {
        a_LogOutput(1, _EA("_InitGameApp(), Failed to create g_pGame"));
        return false;
    }

    // To create the main window, this code creates a new frame window
    // object and then sets it as the application's main window object.
    CMainFrame* pFrame = new CMainFrame;
    m_pMainWnd = pFrame;
    
    // create and load the frame with its resources
    
    pFrame->LoadFrame(IDR_MAINFRAME,
        WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
        NULL, NULL);
    
    // The one and only window has been initialized, so show and update it.
    pFrame->ShowWindow(SW_MAXIMIZE);
    pFrame->UpdateWindow();

    //	Build Game initialize structure

    InitializeCriticalSection(&g_csException);
    InitializeCriticalSection(&g_csRTDebug);
    InitializeCriticalSection(&g_csRenderThread);

    // Set app's path as Angelica work directory
    af_Initialize();
	
    char szWorkDir[128] = ".\\";
	GetCurrentDirectoryA(MAX_PATH, szWorkDir);

    /*GetModuleFileNameA(NULL, szWorkDir, MAX_PATH);
    char* pTemp = strrchr(szWorkDir, '\\');
    *(pTemp+1) = '\0';*/

    char* szCmdLine = GetCommandLineA();
    if (szCmdLine && szCmdLine[0])
    {
        if (strstr(szCmdLine, " -currentdir"))
            GetCurrentDirectoryA(MAX_PATH, szWorkDir);
    }

    af_SetBaseDir(szWorkDir);
    SetCurrentDirectoryA(szWorkDir);

    glb_InitLogSystem(_EA("AnimationEditor.log"));

    if (!g_GameCfgs.Init(_EA("Configs\\element_client.cfg"), _EA("RoleSettings\\AnimatorSettings.ini")))
    {
        a_LogOutput(1, _EA("_InitGameApp(), Failed to load configs file"));
        return false;
	}

    CECGame::GAMEINIT GameInit;
    memset(&GameInit, 0, sizeof (GameInit));
    ::DuplicateHandle(
        GetCurrentProcess(),
        GetCurrentThread(),
        GetCurrentProcess(),
        &g_hMainThread,
        0,
        FALSE,
        DUPLICATE_SAME_ACCESS
        );

    g_hToSuspendMain = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    g_dwMainThreadId = ::GetCurrentThreadId();
    GameInit.hInstance = m_hInstance;
    GameInit.hWnd = pFrame->GetAnimationView()->GetSafeHwnd();
    GameInit.bFullscreen = false;
    RECT Rect;
    GetClientRect(GameInit.hWnd, &Rect);
    GameInit.iRenderWid = Rect.right;
    GameInit.iRenderHei = Rect.bottom;


    if (!LuaBind::Init(false))
    {
        glb_ErrorOutput(ECERR_FAILEDTOCALL, "LuaInit", __LINE__);
        g_bHookExitProcess = FALSE;
        return FALSE;
	}

    if (!g_pGame->Init(GameInit))
    {
        glb_ErrorOutput(ECERR_FAILEDTOCALL, "GameInit", __LINE__);
        g_bHookExitProcess = FALSE;
        return FALSE;
	}

    DWORD dwThreadID;
    g_hRenderThread = ::CreateThread(NULL, 0, RenderThreadFunc, NULL, CREATE_SUSPENDED, &dwThreadID);
    g_hExitGame = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    g_hRenderEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    m_fAutoSaveTimer = 0.0f;

	if (!g_AnmSystem.Init(m_hInstance, AfxGetEditorView()->GetSafeHwnd()))
	{
		g_Log.Log("CAnimationEditorApp::InitInstance, failed to initialize animation system!");
		return FALSE;
	}

	//if (!g_pGame->GetGameRun()->JumpToInstance(1, AfxGetA3DCamera()->GetPos()))
	if(!g_Animator.LoadWorld(1, A3DVECTOR3(0.0f)))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "GameRunStart", __LINE__);
		g_bHookExitProcess = FALSE;
        return FALSE;
	}


	g_bHookExitProcess = FALSE;

    return TRUE;
}

BOOL CAnimationEditorApp::OnIdle(LONG lCount)
{
    //m_pMainWnd->GetAnimationView();
    static DWORD dwLastFrame = a_GetTime();
    DWORD dwTickTime = a_GetTime() - dwLastFrame;
    if (dwTickTime)
    {
        dwLastFrame = a_GetTime();
        // Limit time of a logic tick
        a_ClampRoof(dwTickTime, (DWORD)100);

        // Do logic tick
        ProcessKeyboard();

        SuspendRenderThread();

        if (::WaitForSingleObject(g_hToSuspendMain, 0) == WAIT_OBJECT_0)
		{
			::SuspendThread(::GetCurrentThread());
		}

        if (g_bMultiThreadRenderMode)
        {
            MSG msg;
            while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            
            
            ResumeRenderThread();
            g_bMainSuspened = true;
            ::SetEvent(g_hRenderEvent);
            ::SuspendThread(::GetCurrentThread());//             if (!g_pGame->GetA3DDevice()->GetAutoResetFlag() && g_pGame->GetA3DDevice()->GetNeedResetFlag())
            {
                SuspendLoadThread();
                SuspendRenderThread();
                
                if (!g_pGame->GetA3DDevice()->Reset())
                {
                    a_LogOutput(1, "CECGame::Run(), need reset the device, but we fail to call it!");
                }
                StartLoaderThread();
            }

            g_bMainSuspened = false;
        }
        else
        {
            g_pGame->RunInMainThread();
//             m_fAutoSaveTimer += dwTickTime;
//             float maxTime = 60000.0f * g_pGame->GetConfigs()->GetSystemSettings().iAutoSaveMinutes;
//             if (m_fAutoSaveTimer >= maxTime)
//             {
//                 AutoSave();
//             }
//			UpdateFrame();
		}
    }
	if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
		GetTrackPanel()->UpdateHScrollPos();
		GetTrackPanel()->InvalidateRect(NULL);
    }
    // Let window update menu item states
    CWinApp::OnIdle(lCount);
    return TRUE;
}

void CAnimationEditorApp::UpdateList()
{
    GetObjectList()->UpdateSelectObject();
    GetBezierList()->UpdateSelectBezier();
}

CObjectList* CAnimationEditorApp::GetObjectList()
{
    return ((CMainFrame*)m_pMainWnd)->GetObjectList();
}

CBezierList* CAnimationEditorApp::GetBezierList()
{
    return ((CMainFrame*)m_pMainWnd)->GetBezierList();
}

CTrackPanel* CAnimationEditorApp::GetTrackPanel()
{
    return ((CMainFrame*)m_pMainWnd)->GetTrackPanel();
}

void CAnimationEditorApp::ProcessKeyboard()
{
    if (m_pMainWnd && m_pMainWnd->IsTopParentActive())
    {
        if (GetAsyncKeyState(VK_F1) & 0x8000)
        {
            g_Animator.GetAxes()->SelectAxis(CAnmAxes::TYPE_X);
        }
        else if (GetAsyncKeyState(VK_F2) & 0x8000)
        {
            g_Animator.GetAxes()->SelectAxis(CAnmAxes::TYPE_Y);
        }
        else if (GetAsyncKeyState(VK_F3) & 0x8000)
        {
            g_Animator.GetAxes()->SelectAxis(CAnmAxes::TYPE_Z);
        }
        else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            g_Animator.GetAxes()->SelectAxis(CAnmAxes::TYPE_NONE);
        }
        else if (GetAsyncKeyState(VK_F4) & 0x8000)
        {
            ((CMainFrame*)m_pMainWnd)->GetAnimationView()->KeyboardMouseMoveOperation();
        }

        if (GetAsyncKeyState('W') & 0x8000)
        {
            if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                g_Animator.MoveCamera('W', true);
            else
                g_Animator.MoveCamera('W');
            m_bKeyPressed = true;
        }
        else if (GetAsyncKeyState('S') & 0x8000)
        {
            if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                g_Animator.MoveCamera('S', true);
            else
                g_Animator.MoveCamera('S');
            m_bKeyPressed = true;
        }
        else if (GetAsyncKeyState('A') & 0x8000)
        {
            if (!g_Animator.IsBindCamera())
            {
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                    g_Animator.MoveCamera('A', true);
                else
                    g_Animator.MoveCamera('A');
                m_bKeyPressed = true;
            }
        }
        else if (GetAsyncKeyState('D') & 0x8000)
        {
            if (!g_Animator.IsBindCamera())
            {
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                    g_Animator.MoveCamera('D', true);
                else
                    g_Animator.MoveCamera('D');
                m_bKeyPressed = true;
            }
        }
        else if ((GetAsyncKeyState('E') & 0x8000) | (GetAsyncKeyState(VK_SPACE) & 0x8000))
        {
            if (!g_Animator.IsBindCamera())
            {
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                    g_Animator.MoveCamera('E', true);
                else
                    g_Animator.MoveCamera('E');
                m_bKeyPressed = true;
            }
        }
        else if (GetAsyncKeyState('Q') & 0x8000)
        {
            if (!g_Animator.IsBindCamera())
            {
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                    g_Animator.MoveCamera('Q', true);
                else
                    g_Animator.MoveCamera('Q');
                m_bKeyPressed = true;
            }
        }
        else if (GetAsyncKeyState('Z') & 0x8000)
        {
            if (!g_Animator.IsBindCamera() && !(GetAsyncKeyState(VK_CONTROL) & 0x8000))
            {
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                    g_Animator.MoveCamera('Z', true);
                else
                    g_Animator.MoveCamera('Z');
                m_bKeyPressed = true;
            }
        }
        else if (GetAsyncKeyState('X') & 0x8000)
        {
            if (!g_Animator.IsBindCamera())
            {
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                    g_Animator.MoveCamera('X', true);
                else
                    g_Animator.MoveCamera('X');
                m_bKeyPressed = true;
            }
        }
		else if (GetAsyncKeyState('V') & 0x8000)
        {
            if (!g_Animator.IsBindCamera())
            {
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                    g_Animator.MoveCamera('V', true);
                else
                    g_Animator.MoveCamera('V');
                m_bKeyPressed = true;
            }
        }
		else if (GetAsyncKeyState('B') & 0x8000)
        {
            if (!g_Animator.IsBindCamera())
            {
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                    g_Animator.MoveCamera('B', true);
                else
                    g_Animator.MoveCamera('B');
                m_bKeyPressed = true;
            }
        }
		else if (GetAsyncKeyState('N') & 0x8000)
        {
            if (!g_Animator.IsBindCamera())
            {
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                    g_Animator.MoveCamera('N', true);
                else
                    g_Animator.MoveCamera('N');
                m_bKeyPressed = true;
            }
        }
		else if (GetAsyncKeyState('M') & 0x8000)
        {
            if (!g_Animator.IsBindCamera())
            {
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                    g_Animator.MoveCamera('M', true);
                else
                    g_Animator.MoveCamera('M');
                m_bKeyPressed = true;
            }
        }

        else if (GetAsyncKeyState(VK_DELETE) & 0x8000)
        {
            GetTrackPanel()->DeleteAction();
        }
        else if (GetAsyncKeyState(VK_OEM_3) & 0x8000)
        {
            ((CMainFrame*)m_pMainWnd)->SwitchWindow();
        }
        else if (GetAsyncKeyState('1') & 0x8000)
        {
            OnAnimationReset();
        }
        else if (GetAsyncKeyState('2') & 0x8000)
        {
            if (g_Animator.GetState() != CAnimator::STATE_PLAYING)
            {
                OnAnimationPlay();
            }
        }
        else if (GetAsyncKeyState('3') & 0x8000)
        {
            if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
            {
                OnAnimationPause();
            }
        }
        else if (GetAsyncKeyState('4') & 0x8000)
        {
            if (g_Animator.GetState() != CAnimator::STATE_WAITING)
            {
                OnAnimationStop();
            }
        }
        else if (GetAsyncKeyState('P') & 0x8000)
        {
            if (!m_bKeyPressed)
            {
                if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
                {
                    OnAnimationPause();
                }
                else
                {
                    OnAnimationPlay();
                }
                m_bKeyPressed = true;
            }
        }
        else if (GetAsyncKeyState('C') & 0x8000)
        {
            if (!m_bKeyPressed && !(GetAsyncKeyState(VK_CONTROL) & 0x8000))
            {
                ((CMainFrame*)m_pMainWnd)->GetAnimationView()->OnSelection();
                m_bKeyPressed = true;
            }
        }
        else if (GetAsyncKeyState('M') & 0x8000)
        {
            if (!m_bKeyPressed)
            {
                ((CMainFrame*)m_pMainWnd)->GetAnimationView()->OnTranslation();
                m_bKeyPressed = true;
            }
        }
        else if (GetAsyncKeyState('T') & 0x8000)
        {
            if (!m_bKeyPressed)
            {
                ((CMainFrame*)m_pMainWnd)->GetAnimationView()->OnTranslationTerrain();
                m_bKeyPressed = true;
            }
        }
        else if (GetAsyncKeyState('R') & 0x8000)
        {
            if (!m_bKeyPressed)
            {
                ((CMainFrame*)m_pMainWnd)->GetAnimationView()->OnRotation();
                m_bKeyPressed = true;
            }
        }
        else if (GetAsyncKeyState('Y') & 0x8000)
        {
            if (!m_bKeyPressed && !(GetAsyncKeyState(VK_CONTROL) & 0x8000))
            {
                ((CMainFrame*)m_pMainWnd)->GetAnimationView()->OnRotationTerrain();
                m_bKeyPressed = true;
            }
        }
        else if (GetAsyncKeyState('B') & 0x8000)
        {
            if (!m_bKeyPressed)
            {
                ((CMainFrame*)m_pMainWnd)->GetAnimationView()->OnBezierState();
                m_bKeyPressed = true;
            }
        }
        else
        {
            g_Animator.StopCamera();
            m_bKeyPressed = false;
        }
    }
    else
    {
         g_Animator.StopCamera();
         m_bKeyPressed = false;
    }
}

int CAnimationEditorApp::ExitInstance()
{
	g_AnmSystem.Release();

    ::SetEvent(g_hExitGame);

    if (g_hRenderThread)
    {
        ::SetEvent(g_hRenderEvent);
        g_bMultiThreadRenderMode = true;
        ResumeRenderThread();
        ::WaitForSingleObject(g_hRenderThread, INFINITE);
        CloseHandle(g_hRenderThread);
    }
    
	g_pGame->Release();
    delete g_pGame;
    g_pGame = 0;

	LuaBind::Release();

    glb_CloseLogSystem();
    af_Finalize();

    DeleteCriticalSection(&g_csException);
    DeleteCriticalSection(&g_csRTDebug);
    DeleteCriticalSection(&g_csRenderThread);
    CloseHandle(g_hMainThread);
    CloseHandle(g_hExitGame);
    CloseHandle(g_hRenderEvent);
	CloseHandle(g_hToSuspendMain);

	CMiniDump::End();

    return CWinApp::ExitInstance();
}

void CAnimationEditorApp::OnChanged()
{
    m_bChanged = true;
}

bool CAnimationEditorApp::IsChanged()
{
    return m_bChanged;
}

void CAnimationEditorApp::ResetChanged()
{
    m_bChanged = false;
}

bool CAnimationEditorApp::AskSaveWork()
{
    if (!IsChanged())
    {
        return true;
    }
    int nBox = MessageBox(m_pMainWnd->GetSafeHwnd(), _T("确定是否要保存当前动画?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        OnFileSave();
        return true;
    }
    else if (IDNO == nBox)
    {
        return true;
    }
    return false;
}

void CAnimationEditorApp::AutoSave()
{
    if (m_sFileName != "")
    {
        g_Animator.SaveFile(m_sFileName + _T(".back"));
        m_fAutoSaveTimer = 0.0f;
    }
    else
    {
        g_Animator.SaveFile(_T("videos\\animations\\_AutoSave.back"));
        m_fAutoSaveTimer = 0.0f;
    }
}

void CAnimationEditorApp::OnFileNew() 
{
    // TODO: Add your command handler code here
    bool res = AskSaveWork();
    if (res)
    {
        ((CMainFrame*)m_pMainWnd)->GetAnimationView()->ClearOperation();
		g_Animator.Release();
        g_Animator.Init(&g_AnmSystem);
        GetObjectList()->UpdateList();
        GetBezierList()->UpdateList();
        GetTrackPanel()->UpdatePanel();
        m_sFileName = "";
        ResetChanged();
        m_pMainWnd->SetWindowText(_T("AnimationEditor"));
        m_fAutoSaveTimer = 0.0f;
    }
}

void CAnimationEditorApp::OnFileOpen() 
{
    // TODO: Add your command handler code here
    bool res = AskSaveWork();
    if (res)
    {
        CFileDialog dlg(TRUE, NULL, NULL,
        OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
        _T("Animation files(*.anm)|*.anm|All files(*.*)|*.*||"), NULL);
        CString strDirPath;
        strDirPath.Format(_T("%s"), _T("videos\\animations"));
        dlg.m_ofn.lpstrInitialDir = strDirPath;
        if (dlg.DoModal()!=IDOK)
            return;
        m_sFileName = dlg.GetPathName();


	    CWaitCursor cursor;

        ((CMainFrame*)m_pMainWnd)->GetAnimationView()->ClearOperation();
        g_Animator.Release();
        g_Animator.Init(&g_AnmSystem);
        g_Animator.LoadFileFromEditor(m_sFileName, A3DVECTOR3(0.0f));
  		CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
		CAnmCommonActionManager* pCommonActionMgr = g_Animator.GetCommonActionManager();
        CAnmObject *pPlayer = pObjectManager->GetObject(1);
		if (pPlayer)
		{
			if (pCommonActionMgr->IsEnable())
				pPlayer->Load(WC2AS(pCommonActionMgr->GetHostPlayerModel()));
			else
				pPlayer->Load("models\\Players\\形象\\男\\躯干\\男.ecm");
			CAnmObject *pCamera = pObjectManager->GetObject(0);
			pPlayer->SetPos(pCamera->GetPos() + pCamera->GetDir());
		}   
        GetObjectList()->UpdateList();
		GetBezierList()->UpdateList();
		GetTrackPanel()->UpdatePanel();
    
        ResetChanged();
        m_pMainWnd->SetWindowText(_T("AnimationEditor  ") + m_sFileName);
 //      g_pGame->GetConfigs()->PushRecentFileName(std::string(WC2AS(m_sFileName)));
        m_fAutoSaveTimer = 0.0f;
    }
}

void CAnimationEditorApp::OnFileSave()
{
    if (m_sFileName == "")
    {
        OnFileSaveAs();
    }
    else
    {
        CWaitCursor cursor;
        g_Animator.SaveFile(m_sFileName);
        ResetChanged();
        m_fAutoSaveTimer = 0.0f;
    }
}

void CAnimationEditorApp::OnFileSaveAs() 
{
    // TODO: Add your command handler code here
    CFileDialog dlg(FALSE, NULL, NULL,
        OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
        _T("Animation files(*.anm)|*.anm|All files(*.*)|*.*||"), NULL);
    CString strDirPath;
    strDirPath.Format(_T("%s"), _T("videos\\animations"));
    dlg.m_ofn.lpstrInitialDir = strDirPath;
    if (dlg.DoModal()!=IDOK)
        return;
    CWaitCursor cursor;
    m_sFileName = dlg.GetPathName() + _T(".anm");
    g_Animator.SaveFile(m_sFileName);
    m_pMainWnd->SetWindowText(_T("AnimationEditor  ") + m_sFileName);
//    g_pGame->GetConfigs()->PushRecentFileName(std::string(WC2AS(m_sFileName)));
    m_fAutoSaveTimer = 0.0f;
}

void CAnimationEditorApp::OnUpdateRecentFileMenu(CCmdUI* pCmdUI)
{
    CMenu *pMenu = pCmdUI->m_pMenu;
    int subMenuCount = pMenu->GetMenuItemCount();
    CString menuName;
    for (int i=0; i<subMenuCount; ++i)
    {
        if (pMenu->GetMenuString(i, menuName, MF_BYPOSITION) && menuName == _T("最近打开"))
        {
            pMenu->DeleteMenu(i, MF_BYPOSITION);
            break;
        }
    }

//     CMenu *pPopupMenu = new CMenu();
//     pPopupMenu->CreateMenu();
//     int recentFileCount = g_pGame->GetConfigs()->m_lRecentFiles.size();
//     std::list<std::string>::iterator it = g_pGame->GetConfigs()->m_lRecentFiles.begin();
//     for (int i=0; i<recentFileCount,it!=g_pGame->GetConfigs()->m_lRecentFiles.end(); ++i,++it)
//     {
//         pPopupMenu->AppendMenu(MF_STRING, ID_FILE_MRU_FILE1 + i + 1, CString(it->c_str()));
//     }
//     pMenu->AppendMenu(MF_POPUP, (UINT_PTR)pPopupMenu->operator HMENU(), _T("最近打开"));
    return;
}

void CAnimationEditorApp::OnOpenRecentFile(UINT nID)
{
    const int index = nID - ID_FILE_MRU_FILE2;
//     if (index >= 0 && UINT(index) < g_pGame->GetConfigs()->m_lRecentFiles.size())
//     {
//         std::list<std::string>::iterator it = g_pGame->GetConfigs()->m_lRecentFiles.begin();
//         std::advance(it, index);
//         if ((*it) != "")
//         {
//             m_sFileName = AS2WC(it->c_str());
// 
//             CWaitCursor cursor;
// 
//             ((CMainFrame*)m_pMainWnd)->GetAnimationView()->ClearOperation();
//             g_Animator.Release();
//             g_Animator.Init();
//             g_Animator.LoadFile(m_sFileName, A3DVECTOR3(0.0f));
//             CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
//             CAnmObject *pPlayer = pObjectManager->GetObject(1);
//             pPlayer->Load(_T("models\\Players\\形象\\男\\躯干\\男.ecm"));
//             CAnmObject *pCamera = pObjectManager->GetObject(0);
//             pPlayer->SetPos(pCamera->GetPos() + pCamera->GetDir());
//             GetObjectList()->UpdateList();
//             GetBezierList()->UpdateList();
//             GetTrackPanel()->UpdatePanel();
//             ResetChanged();
//             m_pMainWnd->SetWindowText(_T("AnimationEditor  ") + m_sFileName);
//             g_pGame->GetConfigs()->PushRecentFileName((*it));
//             m_fAutoSaveTimer = 0.0f;
//         }
//     }
    return;
}

/////////////////////////////////////////////////////////////////////////////
// CAnimationEditorApp message handlers

void CAnimationEditorApp::OnAnimationPlay() 
{
    // TODO: Add your command handler code here
    g_Animator.Play();
}

void CAnimationEditorApp::OnAnimationStop() 
{
    // TODO: Add your command handler code here
     g_Animator.Stop();
     g_AnmSystem.m_fPlayedTime = 0.0f;
}

void CAnimationEditorApp::OnAnimationPause() 
{
    // TODO: Add your command handler code here
    g_Animator.Pause();
}

void CAnimationEditorApp::OnUpdateAnimationPlay(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        pCmdUI->SetCheck(1);
    }
    else
    {
        pCmdUI->SetCheck(0);
    }
}

void CAnimationEditorApp::OnUpdateAnimationStop(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    if (g_Animator.GetState() == CAnimator::STATE_WAITING)
    {
        pCmdUI->SetCheck(1);
    }
    else
    {
        pCmdUI->SetCheck(0);
    }
}

void CAnimationEditorApp::OnUpdateAnimationPause(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    if (g_Animator.GetState() != CAnimator::STATE_PLAYING)
    {
        pCmdUI->SetCheck(1);
    }
    else
    {
        pCmdUI->SetCheck(0);
    }
}

void CAnimationEditorApp::OnAnimationReset() 
{
    // TODO: Add your command handler code here
    g_Animator.Stop();
	g_AnmSystem.m_fPlayedTime = 0.0f;
    g_Animator.Play();
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CAnimationEditorApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

void CAnimationEditorApp::UpdateFrame()
{
	DWORD tick = ::GetTickCount();
	static DWORD s_dwLastFrame;
	
	DWORD dwTickTime = s_dwLastFrame ? tick - s_dwLastFrame : 0;
	s_dwLastFrame = tick;
	
	a_ClampRoof( dwTickTime, (DWORD)80 );
	
	if( g_Animator.GetState() == CAnimator::STATE_PLAYING )
		GetTrackPanel()->Invalidate();
}

AWString g_aGetRelativePath(const CString &fullPath)
{
    AString basePath = af_GetBaseDir();
    AString relativePath = WC2AS(AWString(fullPath));
    relativePath.CutLeft(basePath.GetLength() + 1);
    return AS2WC(relativePath);
}
