/*
 * FILE: ElementClient.cpp
 *
 * DESCRIPTION: ElementClient, Main file of game
 *
 * CREATED BY: Duyuxin, 2003/12/11
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.
 */

#define _WIN32_WINDOWS 0x0410 

#include "EC_Global.h"
#include "EC_Configs.h"
#include "EC_Game.h"
#include "EC_SceneLoader.h"
#include "EC_Utility.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_InputCtrl.h"
#include "ACSWrapper.h"
#include "defence/defence.h"
#include "defence/infocollection.h"
#include "defence/APILoader.h"
#include "defence/cheaterkiller.h"
#include "defence/stackchecker.h"
#include "defence/imagechecker.h"
#include "EC_GameUIMan.h"
#include "EC_UIManager.h"
#include "EC_Model.h"
#include "EC_GameSession.h"
#include "AUI\AUIEditBox.h"
#include "gt_overlay.h"

#include <A3DPlatform.h>
#include <A3DTypes.h>
#include <AF.h>
#include <AFilePackGame.h>
#include <AFilePackMan.h>
#include <ddraw.h>
#include "iolib.h"
#include <A3DDevice.h>
#include <A3DCursor.h>

#include "elementpckdir.h"
#include "resource.h"

#include <stdlib.h>
#include <stdio.h>

#include "LuaBind.h"
#include "LuaState.h"
#include "LuaUtil.h"
#include "LuaScript.h"
#include "ScriptValue.h"
#include "ExplorerMan.h"	//Added 2012-05-24.


#include "CC_DataType.h"
#include "CC_API.h"

//#include "CWebPage.h"
#include <windows.h>
#define new A_DEBUG_NEW

//#define TEST_NO_RESPONSE

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////
#ifdef BACK_VERSION
D3DCAPS8		g_d3dcaps;
#else
D3DCAPS9		g_d3dcaps;
#endif

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
	int		iConsole;		//	Console enable flag
	int		iVerCheck;		//	1: Do version check
	int     iLuaDebug;		//  1: enable, 0: disable
	int		iLuaLog;
	int		iRaceTest;		//  美术新增种族测试开关
	ACHAR	szArea[256];	//  大区名字
	ACHAR	szUserName[32]; //  直接登陆用户名	
	ACHAR	szToken[32];	//  直接登陆密码	-->仅用于北美等海外直接登陆？
	ACHAR	szPassword[128];//	多平台登陆密码  -->目前用于国内多平台登陆？Added 2012-05-24
	bool	bArc;			//	是否从北美arc平台启动游戏
};

static s_CMDLINEPARAMS l_CmdParams;

static HINSTANCE l_hInstance = NULL;
static HWND	l_hMainWnd = NULL;
static const ACHAR* l_szClassName = _AL("ZElementClient Window");

static CECGame l_theGame;

CECConfigs g_GameCfgs;

static bool l_bHasAntiHackerMsg = false;
//static bool l_bHasKeyboardProtect = false;
ACString	l_AntiHackerMsg;
BOOL		g_bHookExitProcess = TRUE;

using namespace CC_SDK;
CC_HANDLE	l_hSDK;
HMODULE		l_hDll;

ACHAR _record_file[MAX_PATH];

//#define WM_BOSS_KEY_SHOW_GAME        (WM_USER + 200)
///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////

static bool _ParseCommandLine(ACHAR* szCmdLine);
static void _DefaultCmdParams();
static bool _InitGameApp();
static void _FinalizeGameApp();
static ATOM _RegisterWndClass(HINSTANCE hInstance);
static bool _CreateMainWnd(HINSTANCE hInstance, int nCmdShow, bool bFullScreen);
static LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static bool _InitNetwork();
static void _FinalizeNetwork();

static bool _CreateWebWnd(HINSTANCE hInstance, int nCmdShow, bool bFullScreen);
static LRESULT CALLBACK _WndWebProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// A running count of how many windows we have open that contain a browser object
unsigned char WindowCount = 0;
static HINSTANCE l_webdll = NULL;
HWND		 l_hWebWnd = NULL;
// The class name of our Window to host the browser. It can be anything of your choosing.
static const ACHAR*	ClassName = _AL("Browser Example");

// Where we store the pointers to CWebPage.dll's functions
//EmbedBrowserObjectPtr		lpEmbedBrowserObject;
//UnEmbedBrowserObjectPtr		lpUnEmbedBrowserObject;
//DisplayHTMLPagePtr			lpDisplayHTMLPage;
//DisplayHTMLStrPtr			lpDisplayHTMLStr;
// ResizeBrowserPtr			lpResizeBrowser;

#define ENABLE_UNKNOWN_EXIT_CHECK 
#ifdef ENABLE_UNKNOWN_EXIT_CHECK
	static bool l_bEnableExitCheck = false;
	void on_unknown_exit(void)
	{
		if( l_bEnableExitCheck )
		{
			const char * szErr = "abcdefghijklmn";
			memset((void *)szErr, 0, 4);
		}
	}
#endif

//	Get command line parameters
bool glb_IsConsoleEnable() { return l_CmdParams.iConsole ? true : false; }

//	clear files in log folder
bool prepare_file_structures()
{
	const char * szCachePath = "cache";
	// now try to create a cache dir
	if( _access(szCachePath, 0) == -1 )
	{
		if( !CreateDirectoryA(szCachePath, NULL) )
		{
			MessageBoxA(NULL, "无法创建cache目录，游戏无法运行！\n请确保硬盘有足够的剩余空间再运行本游戏！", "错误", MB_OK | MB_ICONSTOP);
			return false;
		}
	}

	const char * szScreenShotsPath = "screenshots";
	// now try to create a cache dir
	if( _access(szScreenShotsPath, 0) == -1 )
	{
		if( !CreateDirectoryA(szScreenShotsPath, NULL) )
		{
			MessageBoxA(NULL, "无法创建screenshots目录，游戏无法运行！\n请确保硬盘有足够的剩余空间再运行本游戏！", "错误", MB_OK | MB_ICONSTOP);
			return false;
		}
	}

	const char * szGameSavePath = "userdata\\gamesave";
	// now try to create a cache dir
	if( _access(szGameSavePath, 0) == -1 )
	{
		if( !CreateDirectoryA(szGameSavePath, NULL) )
		{
			MessageBoxA(NULL, "无法创建gamesave目录，游戏无法运行！\n请确保硬盘有足够的剩余空间再运行本游戏！", "错误", MB_OK | MB_ICONSTOP);
			return false;
		}
	}

	const char * szLogPath = "logs";
	// now try to create a cache dir
	if( _access(szLogPath, 0) == -1 )
	{
		if( !CreateDirectoryA(szLogPath, NULL) )
		{
			MessageBoxA(NULL, "无法创建logs目录，游戏无法运行！\n请确保硬盘有足够的剩余空间再运行本游戏！", "错误", MB_OK | MB_ICONSTOP);
			return false;
		}
	}

	// now remove logs generated in last game run
	WIN32_FIND_DATAA fd;
	HANDLE hFind = FindFirstFileA("logs\\*.*", &fd);
	if( INVALID_HANDLE_VALUE == hFind )
		return true;

	do
	{
		char szFileName[MAX_PATH];
		
		sprintf(szFileName, "logs\\%s", fd.cFileName);
		strlwr(szFileName);
		
		if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			// ignore folders
			continue;
		}
		else
		{
			//	we only delete .log and .dmp files in Logs folder
			const char * pszExt = strstr(szFileName, ".log");
			if( !pszExt )
			{
				pszExt = strstr(szFileName, ".dmp");
				if( !pszExt )
					pszExt = strstr(szFileName, ".bin");
			}

			if( pszExt && pszExt + 4 == szFileName + strlen(szFileName) )
			{
				DeleteFileA(szFileName);
			}
		}

	} while( FindNextFileA(hFind, &fd) );

	if( af_IsFileExist("logs\\dumpfile") )
		g_bTrojanDumpLastTime = true;
	
	return true;
}

bool CheckHardwareCaps(const char *& pszInfo)
{
	pszInfo = "未知的错误";

	// first see if NetMeeting is running, if so just pop up a warning.
	if( FindWindow(_AL("MPWClass"), NULL) )
	{
		MessageBoxA(NULL, "游戏程序检测到您的机器上有NetMeeting正在运行！\n"
			"由于NetMeeting运行时其他程序无法使用显卡的三维加速功能，所以请先关闭NetMeeting 后再进行游戏！", 
			"警告", MB_OK | MB_ICONINFORMATION);
	}

	HRESULT hval;

	// first get vertex shader version to determine whether or not create sw vp device
#ifdef BACK_VERSION
	IDirect3D8 *	pD3D;
	pD3D = Direct3DCreate8(D3D_SDK_VERSION);
#else
	IDirect3D9 *	pD3D;
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
#endif
	if( pD3D == NULL )
	{
		pszInfo = "无法创建Direct3D对象，请检查DirectX版本";
		return false;
	}
	pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &g_d3dcaps);
	if( g_d3dcaps.MaxSimultaneousTextures < 2 || g_d3dcaps.MaxTextureBlendStages < 2 )
	{
		pszInfo = "显卡至少要支持两层纹理的多层纹理映射！请检查您的显卡驱动并运行dxdiag来检测显卡的Direct3D加速是否正常！";
		return false;
	}

	D3DFORMAT fmtTarget16 = D3DFMT_UNKNOWN;
	D3DFORMAT fmtTarget32 = D3DFMT_UNKNOWN;
	hval = pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_R5G6B5, D3DFMT_R5G6B5, false);
	if( D3D_OK != hval )
	{
		hval = pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X1R5G5B5, D3DFMT_X1R5G5B5, false);
		if( D3D_OK != hval )
		{
			pszInfo = "无法获得16位模式的显示格式，请检查您的显卡驱动以及DirectX的版本";
			return false;
		}
		else
			fmtTarget16 = D3DFMT_X1R5G5B5;
	}
	else
		fmtTarget16 = D3DFMT_R5G6B5;

	hval = pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false);
	if( D3D_OK == hval )
		fmtTarget32 = D3DFMT_X8R8G8B8;

	D3DFORMAT fmtTarget = fmtTarget16;
	if( fmtTarget32 != D3DFMT_UNKNOWN )
		fmtTarget = fmtTarget32;
	
	/*
	// first check if DXT1 and DXT3 is supported;
	hval = pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, fmtTarget, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT1);
	if( D3D_OK != hval )
	{
		pszInfo = "贴图格式DXT1不被支持，显卡硬件性能不符合要求";
		return false;
	}

	hval = pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, fmtTarget, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT3);
	if( D3D_OK != hval )
	{
		pszInfo = "贴图格式DXT3不被支持，显卡硬件性能不符合要求";
		return false;
	}*/
	
	pD3D->Release(); 

#ifdef BACK_VERSION
	// now query video memory through ddraw interface
	IDirectDraw * pDD;
	IDirectDraw2 * pDD2;
	DirectDrawCreate(NULL, &pDD, NULL);
	hval = pDD->QueryInterface(IID_IDirectDraw2, (LPVOID *) &pDD2);
	if( DD_OK != hval )
	{
		pszInfo = "无法创建DirectDraw2对象，请检查DirectX版本";
		return false;
	}
	pDD->Release();

	DDSCAPS		ddscaps;
	ddscaps.dwCaps = DDSCAPS_LOCALVIDMEM;
	DWORD dwLocalTotal, dwLocalFree;
    hval = pDD2->GetAvailableVidMem(&ddscaps, &dwLocalTotal, &dwLocalFree);
	if( DD_OK != hval )
	{
		pszInfo = "无法获取显存大小，请检查DirectX版本";
		return false;
	}
	ddscaps.dwCaps = DDSCAPS_TEXTURE;
	DWORD dwTextureTotal, dwTextureFree;
    hval = pDD2->GetAvailableVidMem(&ddscaps, &dwTextureTotal, &dwTextureFree);
	if( DD_OK != hval )
	{
		pszInfo = "无法获取显存大小，请检查DirectX版本";
		return false;
	}

	pDD2->Release();

	const char * szTexQuality[] = {"高", "中", "低"};
	char  szInfo[256];

	int iTexDetail = g_GameCfgs.GetSystemSettings().iTexDetail;
	iTexDetail = min(iTexDetail, 2);
	iTexDetail = max(iTexDetail, 0);
	if( dwLocalFree < 16 * 1024 * 1024 )
	{
		if( iTexDetail != 2 )
		{
			sprintf(szInfo, "您的显卡的当前剩余显存小于16M，而您的贴图精度选择了[%s]，这将使您在游戏中很卡！建议您将贴图精度调低后再进行游戏！\n\n是否让系统自动选择合适的贴图精度来进行游戏？", szTexQuality[iTexDetail]);
			if( IDYES == MessageBoxA(NULL, szInfo, "贴图精度过高", MB_YESNO | MB_ICONQUESTION) )
			{
				g_GameCfgs.GetSystemSettingsPtr()->iTexDetail = 2;
			}
		}
	}
	else if( dwLocalFree < 32 * 1024 * 1024 )
	{
		if( iTexDetail != 2 )
		{
			sprintf(szInfo, "您的显卡的当前剩余显存小于32M，而您的贴图精度选择了[%s]，这将使您在游戏中很卡！建议您将贴图精度调低后再进行游戏！\n\n是否让系统自动选择合适的贴图精度来进行游戏？", szTexQuality[iTexDetail]);
			if( IDYES == MessageBoxA(NULL, szInfo, "贴图精度过高", MB_YESNO | MB_ICONQUESTION) )
			{
				g_GameCfgs.GetSystemSettingsPtr()->iTexDetail = 2;
			}
		}
	}
	else if( dwLocalFree < 64 * 1024 * 1024 )
	{
		if( iTexDetail == 0 )
		{
			sprintf(szInfo, "您的显卡的当前剩余显存小于64M，而您的贴图精度选择了[%s]，这将使您在游戏中很卡！建议您将贴图精度调低后再进行游戏！\n\n是否让系统自动选择合适的贴图精度来进行游戏？", szTexQuality[iTexDetail]);
			if( IDYES == MessageBoxA(NULL, szInfo, "贴图精度过高", MB_YESNO | MB_ICONQUESTION) )
			{
				g_GameCfgs.GetSystemSettingsPtr()->iTexDetail = 1;
			}
		}
	}

	DWORD dwMaxTextureNeeded = DWORD(16 * 1024 * 1024 * pow(2, 2 - g_GameCfgs.GetSystemSettingsPtr()->iTexDetail));
	if( dwTextureFree < dwMaxTextureNeeded )
	{
		sprintf(szInfo, "您的系统剩余的可用于贴图的显存小于16M，在游戏的过程中可能会导致花屏、死机等严重的错误！您是否还要进入游戏？");
		if( IDYES != MessageBoxA(NULL, szInfo, "贴图精度过高", MB_YESNO | MB_ICONQUESTION) )
		{
			pszInfo = "系统剩余的可用于贴图的显存小于16M，用户取消了游戏运行。";
			return false;		
		}		
	}
#endif

	return true;
}

bool CheckImportantFiles(const char *& pszInfo)
{
	const char * szImportantFiles[] = {
//		"Models\\Players\\形象\\大体型男\\躯干\\大体型男.ecm",
		"Models\\Players\\形象\\男\\躯干\\男.ecm",
		"Models\\Players\\形象\\女\\躯干\\女.ecm",
		"font\\FZL2JW.ttf",
		"font\\FZXH1JW.ttf",
	};

	for(int i=0; i<sizeof(szImportantFiles) / sizeof(const char *); i++)
	{
		AFileImage file;

		if( !af_IsFileExist(szImportantFiles[i]) )
		{
			pszInfo = szImportantFiles[i];
			return false;
		}

		file.Close();
	}

	return true;
}

bool CheckUpdateVersion()
{
	int packversion = 99999999;
	char fn[1024];
	for(int i = 0; i < sizeof(g_szPckDir) / sizeof(const char*); i++)
	{
		strcpy(fn, g_szPckDir[i]);
		strcpy(fn + strlen(g_szPckDir[i]), "\\version.sw");
		AFilePackGame *fPack = (AFilePackGame*)g_AFilePackMan.GetFilePck(fn);
		unsigned char *fbuf = new unsigned char[100];
		DWORD fsize = 100;
		if(fPack && fPack->ReadFile(fn, fbuf, &fsize))
		{
			int n;
			sscanf((char*)fbuf,"%d",&n);
			if(n < packversion)
				packversion = n;
		}
		delete []fbuf;
	}
	if( packversion == 99999999 )
		return true;

	FILE *f = fopen("..\\config\\element\\version.sw", "r");
	if( f )
	{
		int elementversion;
		fscanf(f, "%d", &elementversion);
		fclose(f);
		if( packversion != elementversion )	
			return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////
//
//	Implement 
//
///////////////////////////////////////////////////////////////////////////

HANDLE g_hMainThread					= NULL;
HANDLE g_hRenderThread					= NULL;
HANDLE g_hDefenceThread					= NULL;
HANDLE g_hCreateDumpThread				= NULL;
HANDLE g_hExitGame						= NULL;
HANDLE g_hRenderEvent					= NULL;
HANDLE g_hToSuspendMain					= NULL;
HANDLE g_hWaittoHandleACP				= NULL;
HANDLE g_hCrossAnimateThread			= NULL;
DWORD g_dwMainThreadId					= 0;
DWORD g_dwLastRenderTime				= 0;
volatile bool g_bMainSuspened			= false;
volatile bool g_bRenderSuspended		= true;
volatile bool g_bMultiThreadRenderMode	= false;
bool g_bIsWin98							= false;
static bool _defence_thread_init		= false;
extern volatile time_t g_DenfenceTimeMark;
extern volatile bool g_bHostInGame;
extern int defence_max_check_interval;

CRITICAL_SECTION g_csRenderThread;
CRITICAL_SECTION g_csDefence;
extern CRITICAL_SECTION	locker;

static DWORD WINAPI DefenceThreadFunc(LPVOID lpParam)
{
	// Do nothing on defence thread
	__try
	{
		g_pGame->GetGameRun()->CheckDefenceCode();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		a_LogOutput(1, "exception occured in defence thread!");
	}

	return 0;
}

void VerifyDefenceThread()
{
	if (!_defence_thread_init)
		return;

	if (::WaitForSingleObject(g_hExitGame, 0) != WAIT_OBJECT_0)
	{
		if (g_hDefenceThread && ::WaitForSingleObject(g_hDefenceThread, 0) == WAIT_OBJECT_0)
		{
			a_LogOutput(1, "want to terminte by will 1");
			g_bHookExitProcess = FALSE;
			::ExitProcess(0);
		}
	}
		
	if (!g_bHostInGame || g_bExceptionOccured)
		return;

	if (g_DenfenceTimeMark == 0)
		return;

	time_t t = time(NULL) - g_DenfenceTimeMark;

#ifndef _DEBUG
	if (t > defence_max_check_interval)
	{
		a_LogOutput(1, "want to terminte by will 2");
		g_pGame->GetGameSession()->defencetimeoutnotify(g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID());
	    Sleep(5000);
		g_bHookExitProcess = FALSE;
		::ExitProcess(0);
	}
#endif
}

static DWORD WINAPI RenderThreadFunc(LPVOID lpParam)
{
	int iRet = -1;

#ifndef _NOMINIDUMP	
	__try
	{
#endif

		iRet = l_theGame.RunInRenderThread();

#ifndef _NOMINIDUMP	
	}
	__except (glb_HandleException(GetExceptionInformation()))
	{
		::OutputDebugString(_AL("Exception occurred...\n"));
		a_LogOutput(1, "Exception occurred in render thread... mini dumped!");
		::ExitProcess(-1);
	}
#endif

	g_hRenderThread = NULL;
	::SetEvent(g_hWaittoHandleACP); //In renderthreadfunc
//	::ResumeThread(g_hMainThread);
	return iRet;
}

#ifdef TEST_NO_RESPONSE

static DWORD WINAPI CreateDumpThreadFunc(LPVOID lpParam)
{
	int iRet = -1;

#ifndef _NOMINIDUMP	
	__try
	{
#endif
		
		while(1)
		{
			if(g_dwLastRenderTime != 0 && a_GetTime() - g_dwLastRenderTime > 10000)
			{
				int* p = NULL;
				*p = 0;
			}
			Sleep(5000);
		}

#ifndef _NOMINIDUMP	
	}
	__except (glb_HandleException(GetExceptionInformation()))
	{
		::OutputDebugString(_AL("Exception occurred...\n"));
		a_LogOutput(1, "Exception occurred in render thread, no response... mini dumped!");
		::ExitProcess(-1);
	}
#endif

	g_hCreateDumpThread = NULL;
	return iRet;
}

#endif

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

	/*
	if (bMultiThread)
		::SetThreadPriority(g_hMainThread, THREAD_PRIORITY_HIGHEST);
	else
		::SetThreadPriority(g_hMainThread, THREAD_PRIORITY_NORMAL);
	*/
}

static void WINAPI ShowAntiHackerMsg(WORD id)
{
	const char* szMsg = NetDLL::GetString(id);
	if (szMsg == NULL) return;

	l_AntiHackerMsg = AS2AC(szMsg);
	l_bHasAntiHackerMsg = true;
}

extern DWORD WINAPI GetCodeSegment(char * buffer, unsigned int& size);
DWORD WINAPI GetCodeSegment(char * buffer, unsigned int& size)
{
	// method 1
	unsigned int buffer_len = size;
	size = 0;

	AFileImage file;
	if( file.Open("interfaces\\IconList_Guild.dds", AFILE_OPENEXIST | AFILE_BINARY | AFILE_TEMPMEMORY) )
	{
		if( buffer_len < file.GetFileLength() )
			return 0;

		memcpy(buffer, file.GetFileBuffer(), file.GetFileLength());
		size = file.GetFileLength();
	}

	return 0;

	/*
	// method 2
	unsigned int buffer_len = size;
	size = 0;

	AFileImage file;
	if( !file.Open("interfaces\\IconList_Guild.dds", AFILE_OPENEXIST | AFILE_BINARY | AFILE_TEMPMEMORY) )
		return;
	
	if( buffer_len < file.GetFileLength() )
		return;

	memcpy(buffer, file.GetFileBuffer(), file.GetFileLength());
	size = file.GetFileLength();
	*/
}

extern void stack_trace(LPCVOID sv0, LPCVOID sv1);
static void WINAPI StackTrace(LPCVOID sv0, LPCVOID sv1)
{
	stack_trace(sv0, sv1);
}
extern DWORD WINAPI connect_tcpudp(DWORD ip, DWORD param1);
extern DWORD WINAPI send_tcpudp(SOCKET s, const char* buff, int size);
extern DWORD WINAPI recv_tcpudp(SOCKET s, char* buff, int size);
extern DWORD WINAPI close_tcpudp(SOCKET s);
extern DWORD WINAPI ioctl_tcpudp(SOCKET s, long p1, u_long* p2);
extern DWORD WINAPI ip_socket();
extern DWORD WINAPI send_ip(DWORD s, DWORD packet, DWORD size);
extern DWORD WINAPI send_tcp(DWORD s, DWORD packet, DWORD size);
extern DWORD WINAPI send_udp(DWORD s, DWORD packet, DWORD size);
extern DWORD WINAPI send_sync(DWORD ip, DWORD p);
extern DWORD WINAPI send_ping(DWORD ip, DWORD p);
extern DWORD WINAPI GetBestSourceIP( DWORD dst_ip);
extern DWORD WINAPI alloc_mem(void ** buffer, int size_to_alloc);
extern DWORD WINAPI free_mem(void * buffer);

DWORD WINAPI alloc_mem(void ** buffer, int size_to_alloc)
{
	*buffer = a_malloc(size_to_alloc);
	return 0;
}

DWORD WINAPI free_mem(void * buffer)
{
	a_free(buffer);
	return 0;
}

#include "EC_GameRecord.h"

game_record gr;

void _start_replay()
{
	l_theGame.GetGameRun()->SetSelectedRoleInfo(GNET::RoleInfo(gr.get_char_id(), gr.get_gender(), gr.get_face(), gr.get_hair(),gr.get_ear(), gr.get_tail(), 
		gr.get_occupation(), 1, GNET::Octets(gr.get_name(), gr.get_name_len())));
	l_theGame.GetGameRun()->StartGame(gr.get_instance(), gr.get_host_pos());
	l_theGame.GetGameSession()->SetCharacterID(gr.get_char_id());
	gr.set_seek_adjust(game_record::enum_seek_start);
}

bool _start_replay(const char* szFile)
{
	if (!gr.open_record(szFile))
		return false;

	gr.set_state(game_record::enum_gr_replay);
	_start_replay();
	return true;
}

static const char _record_name[] = "诛仙游戏录像";

void _setup_record_registry()
{
	HKEY hKey;
	DWORD dw;
	char path[MAX_PATH];
	char cmd[MAX_PATH+64];
	char value[MAX_PATH+64];
	::GetModuleFileNameA(NULL, path, MAX_PATH);

	RegCreateKeyExA(HKEY_CLASSES_ROOT, ".zgr", 0, "", 0, KEY_WRITE, NULL, &hKey, &dw);
	RegSetValueExA(hKey, NULL, 0, REG_SZ, (const BYTE*)"ZPWGameRecord", sizeof("ZPWGameRecord"));
	RegCloseKey(hKey);

	RegCreateKeyExA(HKEY_CLASSES_ROOT, "ZPWGameRecord", 0, "", 0, KEY_WRITE, NULL, &hKey, &dw);
	RegSetValueExA(hKey, NULL, 0, REG_SZ, (const BYTE*)_record_name, sizeof(_record_name));
	RegCloseKey(hKey);

	RegCreateKeyExA(HKEY_CLASSES_ROOT, "ZPWGameRecord\\DefaultIcon", 0, "", 0, KEY_WRITE, NULL, &hKey, &dw);
	strcpy(cmd, path);
	strcat(cmd, ",0");
	RegSetValueExA(hKey, NULL, 0, REG_SZ, (const BYTE*)cmd, strlen(cmd)+1);
	RegCloseKey(hKey);

	RegCreateKeyExA(HKEY_CLASSES_ROOT, "ZPWGameRecord\\shell", 0, "", 0, KEY_WRITE, NULL, &hKey, &dw);
	RegCloseKey(hKey);

	RegCreateKeyExA(HKEY_CLASSES_ROOT, "ZPWGameRecord\\shell\\open", 0, "", 0, KEY_WRITE, NULL, &hKey, &dw);
	RegCloseKey(hKey);

	RegCreateKeyExA(HKEY_CLASSES_ROOT, "ZPWGameRecord\\shell\\open\\command", 0, "", 0, KEY_WRITE | KEY_READ, NULL, &hKey, &dw);
	dw = sizeof(value);
	RegQueryValueExA(hKey, NULL, NULL, NULL, (LPBYTE)value, &dw);
	strcpy(cmd, path);
	strcat(cmd, " \"replay:?%1?\"");

	if (strstr(value, cmd) != value)
		RegSetValueExA(hKey, NULL, 0, REG_SZ, (const BYTE*)cmd, strlen(cmd)+1);

	RegCloseKey(hKey);
}

int lua_glb_CalcFileMd5(lua_State * L)
{
	AString strFileName;
	unsigned char md5[16];
	memset(md5, 0, sizeof(md5));
	AString strMd5;

	if (!LuaBind::CheckValue(LuaBind::TypeWrapper<const char*>(), L, 1, strFileName))
		goto RET;

	if( !glb_CalcFileMD5(strFileName, md5) )
		goto RET;
	
RET:
	for(int i=0; i<16; i++)
	{
		AString a;
		a.Format("%02x", md5[i]);
		strMd5 += a;
	}
	LuaBind::Push(LuaBind::TypeWrapper<const char *>(), L, (const char*)strMd5);
	return 1;
}

const struct luaL_reg GlobalApi[]	=
{
	REG_API(lua_glb_CalcFileMd5),
	{NULL, NULL}
};

// void InitGlobalApi(lua_State * L)
// {
// 	luaL_register(L, "GlobalApi", GlobalApi);
// 	lua_pop(L, 1);
// 
// 	// now preload the global lua file
// 	g_LuaStateMan.GetConfigState()->Lock();
// 
// 	AString strText = "";
// 	AIniFile iniFile;
// 	if(iniFile.Open("configs\\url.ini"))
// 	{
// 		strText = iniFile.GetValueAsString("CONFIG", "global_api", "");
// 		iniFile.Close();
// 	}
// 	CLuaScript tmpScript(strText, g_LuaStateMan.GetConfigState());
// 	CLuaScript * pScript = g_LuaFileMan.Register(tmpScript);
// 
// 	if (pScript->Execute() == CLuaScript::EXECUTE_FAIL)
// 	{
// 		assert(0);
// 		g_LuaFileMan.UnRegister(pScript->GetName(), pScript->GetLuaState());
// 		AString msg;
// 		msg.Format("preload %s fail\n", strText);
// 		LUA_DEBUG_INFO(msg);
// 		g_LuaStateMan.GetConfigState()->Unlock();
// 		return;
// 	}
// 	
// 	g_LuaStateMan.GetConfigState()->Unlock();
// }


void InitGlobalApi(lua_State * L)
{
	luaL_register(L, "GlobalApi", GlobalApi);
	lua_pop(L, 1);

	// now preload the global lua file
	g_LuaStateMan.GetConfigState()->Lock();

	AString strText = "";
	AIniFile iniFile;
	if(iniFile.Open("configs\\url.ini"))
	{
		strText = iniFile.GetValueAsString("CONFIG", "global_api", "");
		iniFile.Close();
	}

	CLuaScript * pScript = g_LuaStateMan.GetConfigState()->RegisterFile(strText);
// 	if (!pScript)
// 	{
// 		g_LuaStateMan.GetConfigState()->Unlock();
// 		return;
// 	}

	
	g_LuaStateMan.GetConfigState()->Unlock();
}


bool CheckMapMD5(const char * szMap)
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	CScriptString str;
	str.SetAString(szMap);
	args.push_back(CScriptValue(str));
	//args.push_back(CScriptValue(szMap));
	LuaBind::Call(g_LuaStateMan.GetConfigState()->GetVM(), "lua_glb", "check_md5", args, results);
	if( results.size() != 1 || !results[0].GetBool() )
		return false;

	return true;
}

bool CheckDirectXInstalledState()
{
	if( af_IsFileExist("DirectX/DXSETUP.exe") )
	{
		ACHAR szValue[300];
		DWORD dwCount = 300;
		CRegKey cReg;
		cReg.Create(HKEY_CURRENT_USER,TEXT("Software\\PWRD\\zhuxian2"));
		if( cReg.QueryStringValue(TEXT("DX9CInstalled"), szValue, &dwCount) != ERROR_SUCCESS)
		{
			MessageBoxA(NULL, "游戏运行需要新版本DirectX驱动程序的支持，点击确定按钮后将自动安装DirectX。\r\n请您在DirectX安装完成后再次启动游戏。", "错误", MB_OK | MB_ICONSTOP);
			OSVERSIONINFO osvi;
			ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			if(::GetVersionEx(&osvi) != 0)
			{
				SHELLEXECUTEINFO sei={sizeof(SHELLEXECUTEINFO)};
				if( osvi.dwMajorVersion < 6 )
					sei.lpVerb=_AL("open");
				else
					sei.lpVerb=_AL("runas");

				sei.lpFile=_AL("DirectX\\DXSETUP.exe");//add  application  which you want to run as administrator here
				sei.nShow=SW_SHOWNORMAL;//without this,the windows will be hiden
				if(!ShellExecuteEx(&sei)){
					DWORD dwStatus=GetLastError();
					if(dwStatus==ERROR_CANCELLED){
						MessageBoxA(NULL,"DirectX安装取消", NULL, MB_OK);
					}
				}
				else
				{
					cReg.SetStringValue(TEXT("DX9CInstalled"), TEXT("1"));
					cReg.Close();
				}
			}
			return  false;
		}
		cReg.Close();
	}

	return true;
}

//	WinMain
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, ACHAR* lpCmdLine, int nCmdShow)
{
	/*
	if (!a_strstr(lpCmdLine, _AL("game:zpw")))
	{
		MessageBoxA(NULL, "请运行Launcher来启动游戏", "错误", MB_OK | MB_ICONSTOP);
		return -1;
	}*/

	if( !CheckDirectXInstalledState() )
		return 0;

	UINT nCode = GetPrivateProfileIntA("Patcher", "Updated", 1, ".\\patcher.ini");
	if( nCode == 0 )
	{
		MessageBoxA(NULL, "客户端的最近一次更新没有完成，这将导致无法进入游戏等严重问题！\r\n现在将启动自动更新程序进行检查！", "错误", MB_OK | MB_ICONSTOP);
		ShellExecute(NULL,TEXT("open"),TEXT("..\\patcher\\patcher.exe"),NULL,TEXT("..\\patcher"),SW_SHOWNORMAL);
		return 0;
	}

	_setup_record_registry();

	//	Parse command line
	if (!_ParseCommandLine(lpCmdLine))
		return -1;

	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION  pi;
	memset(&pi, 0, sizeof(pi));
	CreateProcess(L"reportbugs\\pwprotector.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	StackChecker::ACStackCheckInit();
	ImageChecker::ACImageCheckerInit();

	// first of all, make the file structures prepared
	if( !prepare_file_structures() )
		return -1;

#ifdef ENABLE_UNKNOWN_EXIT_CHECK
	l_bEnableExitCheck = true;
	atexit(on_unknown_exit);
#endif

	//	Initialize critical section
	InitializeCriticalSection(&g_csException);
	InitializeCriticalSection(&g_csSession);
	InitializeCriticalSection(&g_csRTDebug);
	InitializeCriticalSection(&g_csRenderThread);
	InitializeCriticalSection(&g_csDefence);

#ifndef _NOMINIDUMP

	__try
	{

#endif

#ifdef BACK_VERSION
	if (!overlay::GTOverlay::Instance().EnterMain(overlay::kGameRenderDX81))
#else
	if (!overlay::GTOverlay::Instance().EnterMain(overlay::kGameRenderDX9))
#endif
	{
		ShellExecute(NULL,TEXT("open"),TEXT("gt_updater.exe"),NULL,TEXT(".\\gt\\"),SW_SHOWNORMAL);
	}		

	//	Initialize game, create main window
	if (!_InitGameApp())
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "wWinMain", __LINE__);
		return -1;
	}

	if( !CheckUpdateVersion() )
	{
		MessageBoxA(NULL, "客户端的最近一次更新没有完成，这将导致无法进入游戏等严重问题！\r\n现在将启动自动更新程序进行检查！", "错误", MB_OK | MB_ICONSTOP);
#ifdef ENABLE_UNKNOWN_EXIT_CHECK
		l_bEnableExitCheck = false;
#endif
		ShellExecute(NULL,TEXT("open"),TEXT("..\\patcher\\patcher.exe"),NULL,TEXT("..\\patcher"),SW_SHOWNORMAL);
		return -1;
	}
	
	// then check hardware requirements.
	const char * szInfo = "";
	if (!CheckHardwareCaps(szInfo))
	{
		MessageBoxA(NULL, szInfo, "对不起，由于您的系统硬件或系统软件的问题，无法运行本游戏!", MB_OK | MB_ICONSTOP);
#ifdef ENABLE_UNKNOWN_EXIT_CHECK
		l_bEnableExitCheck = false;
#endif
		return -1;
	}

	const char * szMissedFile = "";
	if (!CheckImportantFiles(szMissedFile))
	{
		MessageBoxA(NULL, "对不起，由于您机器上安装的游戏资源缺少了一个重要的文件，游戏无法运行！请重新安装游戏客户端！", szMissedFile, MB_OK | MB_ICONSTOP);
#ifdef ENABLE_UNKNOWN_EXIT_CHECK
		l_bEnableExitCheck = false;
#endif
		return -1;
	}

	//	Build Game initialize structure
	CECGame::GAMEINIT GameInit;
	memset(&GameInit, 0, sizeof (GameInit));
	GameInit.iType = -1;

	if( l_CmdParams.iFullScreen == -1 )
		GameInit.bFullscreen	= g_GameCfgs.GetSystemSettings().bFullScreen;
	else
		GameInit.bFullscreen	= l_CmdParams.iFullScreen ? true : false;
	if( l_CmdParams.iRenderWid == -1 )
		GameInit.iRenderWid		= g_GameCfgs.GetSystemSettings().iRndWidth;
	else
		GameInit.iRenderWid		= l_CmdParams.iRenderHei;
	if( l_CmdParams.iRenderHei == -1 )
		GameInit.iRenderHei		= g_GameCfgs.GetSystemSettings().iRndHeight;
	else
		GameInit.iRenderHei		= l_CmdParams.iRenderHei;
	
	GameInit.bVerCheck = l_CmdParams.iVerCheck ? true : false;
	GameInit.iRaceTest = l_CmdParams.iRaceTest; 

	if (LOBYTE(LOWORD(GetVersion())) < 5)
		g_bIsWin98 = true;

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
	SwitchRenderThreadMode(!GameInit.bFullscreen);

	//	Register window class
	if (!_RegisterWndClass(hInstance))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "wWinMain", __LINE__);
		return -1;
	}

	//	Create main window
	if (!_CreateMainWnd(hInstance, nCmdShow, GameInit.bFullscreen))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "wWinMain", __LINE__);
		return -1;
	}

//	if (!_CreateWebWnd(hInstance, nCmdShow, false))
//	{
//		glb_ErrorOutput(ECERR_FAILEDTOCALL, "wWinMain", __LINE__);
//		return -1;
//  	}

	//---------------------------------------------------------------
	CExplorerMan::GetInstance().PreCreateExplorer( l_hMainWnd );
	//---------------------------------------------------------------
	
	GameInit.hInstance		= l_hInstance;
	GameInit.hWnd			= l_hMainWnd;
	
	if (l_CmdParams.szIPAddr[0])
	{
	#ifdef UNICODE
		a_WideCharToMultiByte(l_CmdParams.szIPAddr, 256, GameInit.szIP, 256);
	#else
		strcpy(GameInit.szIP, l_CmdParams.szIPAddr);
	#endif

		GameInit.iPort = l_CmdParams.iPort;
	}
	
	if (l_CmdParams.szUserName[0] &&l_CmdParams.szToken[0])
	{
	#ifdef UNICODE
		a_WideCharToMultiByte(l_CmdParams.szUserName, 32, GameInit.szUserName, 32);
		a_WideCharToMultiByte(l_CmdParams.szToken, 32, GameInit.szToken, 32);
		a_WideCharToMultiByte(l_CmdParams.szArea, 256, GameInit.szArea, 256);
	#else
		strcpy(GameInit.szUserName, l_CmdParams.szUserName);
		strcpy(GameInit.szToken, l_CmdParams.szToken);
		strcpy(GameInit.szArea, l_CmdParams.szArea);
	#endif

	}

	if ( l_CmdParams.szUserName[0] && l_CmdParams.szPassword[0] )
	{
#ifdef UNICODE
		a_WideCharToMultiByte(l_CmdParams.szUserName, 32, GameInit.szUserName, 32);
		a_WideCharToMultiByte(l_CmdParams.szPassword, 128, GameInit.szPassword, 128);
#else
		strcpy(GameInit.szUserName, l_CmdParams.szUserName);
		strcpy(GameInit.szPassword, l_CmdParams.szPassword);
#endif
	}

	LuaBind::EnableLog(l_CmdParams.iLuaLog != 0);

	//init lua
	if (!LuaBind::Init(l_CmdParams.iLuaDebug == 1))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "wWinMain", __LINE__);
		return -1;
	}
		

	// init ecm lua api
	InitECMApi(g_LuaStateMan.GetConfigState());

	//	Initialize game
	if (!l_theGame.Init(GameInit))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "wWinMain", __LINE__);
		g_bHookExitProcess = FALSE;
		return -1;
	}

//	if( stricmp(g_GameCfgs.GetCountryRegion(), "kr") == 0 )
//		l_bHasKeyboardProtect = true;

	typedef CC_HANDLE (*CCINIT)(unsigned int nPartnerId, unsigned int nAppId, D3D_VERSION nVersion);
	typedef HRESULT (*CCUNINIT)(const CC_HANDLE hSDK);	

	if( l_CmdParams.bArc &&
		(stricmp(g_GameCfgs.GetCountryRegion(), "us") == 0 ||
		stricmp(g_GameCfgs.GetCountryRegion(), "fr") == 0 ||
		stricmp(g_GameCfgs.GetCountryRegion(), "de") == 0))
	{		
		l_hDll = LoadLibrary(_AL("CoreClientSDK.dll"));
		bool bLoad = false;

		if(l_hDll != 0)
		{
			CCINIT ccinitproc;
			ccinitproc = (CCINIT)GetProcAddress(l_hDll, "CC_Init");
			if(ccinitproc != NULL)
			{
				l_hSDK = ccinitproc(0, 0, D3D8);
			}
		}
		else
		{
// 			MessageBoxA(g_pGame->GetGameInit().hWnd, "CoreClientSDK.dll not found!", NULL, MB_OK | MB_ICONERROR);
// 			g_bHookExitProcess = FALSE;
// 			::ExitProcess(-1);
// 			return -1;
		}
	}

	if (gr.get_state() != game_record::enum_gr_replay)
	{
		if (!l_theGame.GetGameRun()->StartLogin(true))
		{
			g_bHookExitProcess = FALSE;
			return -2;
		}
	}
	else
		_start_replay();

	DWORD dwThreadID;
	g_hRenderThread = ::CreateThread(NULL, 0, RenderThreadFunc, NULL, CREATE_SUSPENDED, &dwThreadID);
	g_hDefenceThread = ::CreateThread(NULL, 0, DefenceThreadFunc, NULL, CREATE_SUSPENDED, &dwThreadID);
#ifdef TEST_NO_RESPONSE
	g_hCreateDumpThread = ::CreateThread(NULL, 0, CreateDumpThreadFunc, NULL, CREATE_SUSPENDED, &dwThreadID);
#endif
	g_hExitGame = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	g_hRenderEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hWaittoHandleACP = ::CreateEvent(NULL, FALSE, FALSE, NULL); 

	NetDLL::Register(-1, static_cast<LPVOID>(ShowAntiHackerMsg));
	NetDLL::Register(-2, static_cast<LPVOID>(StackTrace));
	NetDLL::Register(-3, static_cast<LPVOID>(connect_tcpudp));
	NetDLL::Register(-4, static_cast<LPVOID>(send_tcpudp));
	NetDLL::Register(-5, static_cast<LPVOID>(recv_tcpudp));
	NetDLL::Register(-6, static_cast<LPVOID>(close_tcpudp));
	NetDLL::Register(-7, static_cast<LPVOID>(ioctl_tcpudp));
	NetDLL::Register(-8, static_cast<LPVOID>(ip_socket));
	NetDLL::Register(-9, static_cast<LPVOID>(send_ip));
	NetDLL::Register(-10, static_cast<LPVOID>(send_tcp));
	NetDLL::Register(-11, static_cast<LPVOID>(send_udp));
	NetDLL::Register(-12, static_cast<LPVOID>(send_sync));
	NetDLL::Register(-13, static_cast<LPVOID>(send_ping));
	NetDLL::Register(-14, static_cast<LPVOID>(GetBestSourceIP));
	NetDLL::Register(-15, static_cast<LPVOID>(GetCodeSegment));
	NetDLL::Register(-16, static_cast<LPVOID>(alloc_mem));
	NetDLL::Register(-17, static_cast<LPVOID>(free_mem));
	NetDLL::Register(-18, static_cast<LPVOID>(NetDLL::AppendSpyInfo));
	NetDLL::Register(-19, static_cast<LPVOID>(CheaterKiller::AppendCheatInfo));
	NetDLL::Register(-20, static_cast<LPVOID>(StackChecker::GetStackCheckData));
	NetDLL::Register(-21, static_cast<LPVOID>(StackChecker::UpdateStackCheckCode));
	NetDLL::Register(-22, static_cast<LPVOID>(ImageChecker::UpdateImageChecker));

#ifdef USING_DEFENCE
#ifdef BACK_VERSION
	D3DADAPTER_IDENTIFIER8 idAdapter;
	l_theGame.GetA3DEngine()->GetD3D()->GetAdapterIdentifier(D3DADAPTER_DEFAULT, D3DENUM_NO_WHQL_LEVEL, &idAdapter);
#else
	D3DADAPTER_IDENTIFIER9 idAdapter;
	l_theGame.GetA3DEngine()->GetD3D()->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &idAdapter);
#endif
	Collector::Engine::GetInstance().SetAdpaterDescription(idAdapter.Description);
	::ResumeThread(g_hDefenceThread);
	_defence_thread_init = true;
#endif

#ifdef TEST_NO_RESPONSE
	::ResumeThread(g_hCreateDumpThread);
#endif

	while (1)
	{
		if (g_dwFatalErrorFlag == FATAL_ERROR_LOAD_BUILDING)
		{
			MessageBoxA(g_pGame->GetGameInit().hWnd, "加载建筑失败，程序即将退出！", NULL, MB_OK | MB_ICONERROR);
			a_LogOutput(1, "exit process because failed to load building");
			g_bHookExitProcess = FALSE;
			::ExitProcess(-2);
		}
		else if(g_dwFatalErrorFlag == FATAL_ERROR_WRONG_CONFIGDATA)
		{
			MessageBoxA(g_pGame->GetGameInit().hWnd, "读取资源数据出错，程序即将退出！", NULL, MB_OK | MB_ICONERROR);
			a_LogOutput(1, "exit process because wrong config data");
			g_bHookExitProcess = FALSE;
			::ExitProcess(-3);
		}
		
		if (g_bExceptionOccured)
		{
			a_LogOutput(1, "exit process because exception occurs");
			::ExitProcess(-1);
		}

		SuspendRenderThread();

		// toggle ime state in main thread
		AUIEditBox::ToggleIME();

		// flash toolbar button when needs
		static BOOL bMsgBeeped = FALSE;
		if (g_pGame->GetGameRun()->GetFlashWindow())
		{
			static DWORD dwLastFlash = 0;
			DWORD dwCurrentTick = ::GetTickCount();

			if (dwCurrentTick - dwLastFlash > 500)
			{
				FLASHWINFO fi;
				fi.cbSize	= sizeof(fi);
				fi.hwnd		= g_pGame->GetGameInit().hWnd;
				fi.dwFlags	= FLASHW_TRAY;
				fi.uCount	= 1;
				fi.dwTimeout= 500;
				FlashWindowEx(&fi);

				dwLastFlash = dwCurrentTick;
			}

			if (!bMsgBeeped)
			{
				::MessageBeep(MB_ICONASTERISK);
				bMsgBeeped = TRUE;
			}
		}
		else
			bMsgBeeped = FALSE;

		// Korea Version: Keyboard Protect
/*		if( l_bHasKeyboardProtect )
		{
			static BOOL bKeyboardProtect = FALSE;
			if( g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan() == CECUIManager::UIMAN_LOGIN )
			{
				if( !bKeyboardProtect )
				{
					bKeyboardProtect = TRUE;
					typedef BOOL (WINAPI *AUTOSTART)(char *pClassList, BOOL bExtend, int nOption);
					typedef DWORD (WINAPI *EXOPTION)(int nType, char *pOption1, char *pOption2, char *pOption3);
					typedef void (WINAPI *SETIMAGEDIR)(char *pImageDir);
					#define KDFINJ_OPTIONEX2_GENTLEDRIVER	0x00000010
					#define KDFINJ_EXTYPE_OPTIONEX2			22
					#define KDFINJ_EXTYPE_SUBCLASSING		23

					HMODULE hDll;
					hDll = LoadLibrary(_AL("kdfapi2.dll"));
					bool bLoad = false;
					if(hDll != 0)
					{
						SETIMAGEDIR kdfSetImageDir = (SETIMAGEDIR)GetProcAddress(hDll,"kdfSetImageDir");
						if(kdfSetImageDir != 0)
							kdfSetImageDir("kdfimage");

						AUTOSTART kdfAutoStart = (AUTOSTART)GetProcAddress(hDll,"kdfAutoStart");
						if(kdfAutoStart != 0)
						{
							kdfAutoStart(NULL,  NULL,  0x22030001);
							EXOPTION kdfExOption = (EXOPTION)GetProcAddress(hDll,"kdfExOption");
							if(kdfExOption != 0)
							{
								DWORD dwParam2nd = KDFINJ_OPTIONEX2_GENTLEDRIVER;
								kdfExOption(KDFINJ_EXTYPE_OPTIONEX2, (char*)&dwParam2nd,  "1",  NULL);
								kdfExOption(KDFINJ_EXTYPE_SUBCLASSING, (char*)l_hMainWnd,  NULL,  NULL);
								bLoad = true;
							}
						}
					}
					if( !bLoad )
					{
						MessageBoxA(g_pGame->GetGameInit().hWnd, "Failed to load kdfapi2.dll", NULL, MB_OK | MB_ICONERROR);
						a_LogOutput(1, "exit process because failed to load kdfapi2.dll");
						g_bHookExitProcess = FALSE;
						::ExitProcess(-4);
					}
				}
			}
			else
			{
				if( bKeyboardProtect )
				{
					bKeyboardProtect = FALSE;
					typedef void (WINAPI *AUTOSTARTCLEAN)(DWORD dwHandle);

					HMODULE hDll;
					hDll = LoadLibrary(_AL("kdfapi2.dll"));
					if(hDll != 0)
					{
						AUTOSTARTCLEAN kdfAutoStartClean = (AUTOSTARTCLEAN)GetProcAddress(hDll,"kdfAutoStartCleanB");
						if(kdfAutoStartClean != 0)
							kdfAutoStartClean(NULL);
					}
				}
			}
		}
*/
//		if( g_pGame->GetGameRun()->GetTrayFlashFlag())
//		{
//			static DWORD dwLastFlash2 = 0;
//			DWORD dwCurrentTick = ::GetTickCount();
//
//			if( dwCurrentTick - dwLastFlash2 > 300)
//			{
//				NOTIFYICONDATA nid; 
//				nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
//				nid.hWnd = l_hMainWnd;
//				nid.uID=IDI_MAINICON; 
//				nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ; 
//				nid.uCallbackMessage=WM_BOSS_KEY_SHOW_GAME;
//				nid.hIcon=LoadIcon((HINSTANCE)GetWindowLong(l_hMainWnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDI_MAINICON)); 
//				wchar_t titleName[MAX_PATH];
//				int num = GetWindowText(l_hMainWnd,titleName,MAX_PATH);
//				if (num>0 && num<sizeof(nid.szTip)/sizeof(wchar_t))
//				{
//					titleName[num] = _L('\0');
//					wcscpy(nid.szTip,titleName);
//				}else
//				{
//					wcscpy(nid.szTip,_L("Game"));
//				}
//				Shell_NotifyIcon(NIM_MODIFY,&nid);
//				dwLastFlash2 = dwCurrentTick;
//			}
// 		}

		if (::WaitForSingleObject(g_hToSuspendMain, 0) == WAIT_OBJECT_0)
		{
			WaitForSingleObjectEx(g_hWaittoHandleACP,INFINITE,true);
//			::SuspendThread(::GetCurrentThread());
		}


		// see if need popup an antihacker message
		if( l_bHasAntiHackerMsg )
		{
			CECUIManager* pUI = g_pGame->GetGameRun()->GetUIManager();
			if (pUI)
			{
				CECGameUIMan* pGameUI = pUI->GetInGameUIMan();
				if (pGameUI) pGameUI->ShowErrorMsg(l_AntiHackerMsg);
			}
			l_bHasAntiHackerMsg = false;
		}

		if (g_bMultiThreadRenderMode)
		{
			MSG msg;
			while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					goto _exit;

				//-------------------------------------------------------------
				if ( CExplorerMan::GetInstance().PreTranslateMessage(msg) )
				{
					continue;
				}
				//-------------------------------------------------------------
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			if (!g_pGame->GetA3DDevice()->GetAutoResetFlag() && g_pGame->GetA3DDevice()->GetNeedResetFlag() && g_pGame->GetA3DDevice()->GetCanResetFlag())
			{
				SuspendLoadThread();
				SuspendRenderThread();

				if (!g_pGame->GetA3DDevice()->Reset())
					a_LogOutput(1, "CECGame::Run(), need reset the device, but we fail to call it!");

				StartLoaderThread();
			}

			if (!g_bMultiThreadRenderMode)
				continue;

			ResumeRenderThread();
			g_bMainSuspened = true;
			::SetEvent(g_hRenderEvent);
			
			WaitForSingleObjectEx(g_hWaittoHandleACP,INFINITE,true);
			//::SuspendThread(::GetCurrentThread());
			g_bMainSuspened = false;
		}
		else if (!l_theGame.RunInMainThread())
			break;
	}

_exit:
	
	if( stricmp(g_GameCfgs.GetCountryRegion(), "us") == 0 ||
		stricmp(g_GameCfgs.GetCountryRegion(), "fr") == 0 ||
		stricmp(g_GameCfgs.GetCountryRegion(), "de") == 0)
	{

		if(l_hDll)
		{
			CCUNINIT ccuninitproc;
			ccuninitproc = (CCUNINIT)GetProcAddress(l_hDll, "CC_UnInit");
			if(ccuninitproc != NULL)
			{
				ccuninitproc(l_hSDK);
			}

			FreeLibrary(l_hDll);
		}
	}

//	ShowWindow(l_hWebWnd, SW_HIDE);
//	(*lpUnEmbedBrowserObject)(l_hWebWnd);
// 	FreeLibrary(l_webdll);
	_defence_thread_init = false;
	::SetEvent(g_hExitGame);

	if (g_hDefenceThread)
		::WaitForSingleObject(g_hDefenceThread,INFINITE);

	if (g_hRenderThread)
	{
		::SetEvent(g_hRenderEvent);
		g_bMultiThreadRenderMode = true;
		ResumeRenderThread();
		::WaitForSingleObject(g_hRenderThread, INFINITE);
	}

	l_theGame.Release();

	LuaBind::Release();
	
	//	Finalize game
	_FinalizeGameApp();
#ifndef _NOMINIDUMP

	}	//	__try

	__except (glb_HandleException(GetExceptionInformation()))
	{
		::OutputDebugString(_AL("Exception occurred...\n"));
		a_LogOutput(1, "Exception occurred... mini dumped!");
		::ExitProcess(-1);
	}

#endif

	DeleteCriticalSection(&g_csException);
	DeleteCriticalSection(&g_csSession);
	DeleteCriticalSection(&g_csRTDebug);
	DeleteCriticalSection(&g_csRenderThread);
	DeleteCriticalSection(&g_csDefence);
	CloseHandle(g_hMainThread);
	CloseHandle(g_hExitGame);
	CloseHandle(g_hRenderEvent);
	CloseHandle(g_hToSuspendMain);

	if (gr.get_state() == game_record::enum_gr_replay)
		gr.close_record();

#ifdef ENABLE_UNKNOWN_EXIT_CHECK
	l_bEnableExitCheck = false;
#endif

	g_bHookExitProcess = FALSE;
	return 0;
}

bool _ParseCommandLine(ACHAR* szCmdLine)
{
	ACHAR* szCmd;

	//	Set default value for command line parameters
	_DefaultCmdParams();

#ifdef _DEBUG
	if ((szCmd = a_strstr(szCmdLine, _AL("fullscreen:"))))
		a_sscanf(szCmd, _AL("fullscreen:%d"), &l_CmdParams.iFullScreen);
#endif
	
	if ((szCmd = a_strstr(szCmdLine, _AL("rendersize:"))))
		a_sscanf(szCmd, _AL("rendersize:%dx%d"), &l_CmdParams.iRenderWid, &l_CmdParams.iRenderHei);

	if ((szCmd = a_strstr(szCmdLine, _AL("ip:"))))
		a_sscanf(szCmd, _AL("ip:%d:%s"), &l_CmdParams.iPort, l_CmdParams.szIPAddr);

	if ((szCmd = a_strstr(szCmdLine, _AL("console:"))))
		a_sscanf(szCmd, _AL("console:%d"), &l_CmdParams.iConsole);

	if ((szCmd = a_strstr(szCmdLine, _AL("noverchk"))))
		l_CmdParams.iVerCheck = 0;

	if ((szCmd = a_strstr(szCmdLine, _AL("luadebug:"))))
		a_sscanf(szCmd, _AL("luadebug:%d"), &l_CmdParams.iLuaDebug);

	if ((szCmd = a_strstr(szCmdLine, _AL("lualog:"))))
		a_sscanf(szCmd, _AL("lualog:%d"), &l_CmdParams.iLuaLog);
    
	if ((szCmd = a_strstr(szCmdLine, _AL("orctest:"))))
		a_sscanf(szCmd, _AL("orctest:%d"), &l_CmdParams.iRaceTest);

	if ((szCmd = a_strstr(szCmdLine, _AL("area:"))))
		a_sscanf(szCmd, _AL("area:%s"), l_CmdParams.szArea);

	if ((szCmd = a_strstr(szCmdLine, _AL("user:"))))
		a_sscanf(szCmd, _AL("user:%s"), l_CmdParams.szUserName);

	if ((szCmd = a_strstr(szCmdLine, _AL("token:"))))
		a_sscanf(szCmd, _AL("token:%s"), l_CmdParams.szToken);

	if ((szCmd = a_strstr(szCmdLine, _AL("pwd:"))))		//Added 2012-05-24.
	{
		a_sscanf(szCmd, _AL("pwd:%s"), l_CmdParams.szPassword);
	}

	if ((szCmd = a_strstr(szCmdLine, _AL("arc:1"))))
		l_CmdParams.bArc = true;

	const char* szCmdA;
	if ((szCmdA = strstr(::GetCommandLineA(), "replay:")))
	{
		const char* head = strstr(szCmdA, "?");

		if (!head)
			return false;

		const char* end = strstr(head+1, "?");

		if (!end)
			return false;

		char path[MAX_PATH];
		memset(path, 0, sizeof(path));

		if (strstr(::GetCommandLineA(), "nochangedir") == NULL)
		{
			char szModule[MAX_PATH];
			char szPath[MAX_PATH];
			GetModuleFileNameA(NULL, szModule, MAX_PATH);
			af_GetFilePath(szModule, szPath, MAX_PATH);
			::SetCurrentDirectoryA(szPath);
		}

		strncpy(path, head + 1, end - head - 1);
		gr.set_state(game_record::enum_gr_replay);

		if (!gr.open_record(path))
		{
			::MessageBoxA(NULL, "录像文件版本不对，无法回放！", "Error", MB_OK);
			return false;
		}

		a_strcpy(_record_file, AS2AC(path));
	}

	return true;
}

void _DefaultCmdParams()
{
	memset(&l_CmdParams, 0, sizeof (l_CmdParams));

	l_CmdParams.iFullScreen	= -1;
	l_CmdParams.iRenderWid	= -1;
	l_CmdParams.iRenderHei	= -1;
	l_CmdParams.iConsole	= 0;
	l_CmdParams.iVerCheck	= 1;
	l_CmdParams.iLuaLog		= 0;
	l_CmdParams.iRaceTest	= 0;

	l_CmdParams.szIPAddr[0]	= '\0';
	l_CmdParams.iPort		= 0;
	l_CmdParams.szUserName[0] = '\0';
	l_CmdParams.szToken[0]	= '\0';
	l_CmdParams.szArea[0]   = '\0';
	l_CmdParams.szPassword[0] = '\0';
	l_CmdParams.bArc = false;
}

bool _InitGameApp()
{
	af_Initialize();

	//	Initialize log system
	glb_InitLogSystem("EC.log");

	char szFile[MAX_PATH];

	//	Set current directory as work directory
	GetCurrentDirectoryA(MAX_PATH, g_szWorkDir);

	strcpy(g_szIniFile, g_szWorkDir);
	strcat(g_szIniFile, "\\ElementClient.ini");

	af_SetBaseDir(g_szWorkDir);

	// now open all file packages
	for(int i=0; i<sizeof(g_szPckDir) / sizeof(const char*); i++)
	{
		char szPckFile[MAX_PATH];
		sprintf(szPckFile, "%s.pck", g_szPckDir[i]);
		if( !g_AFilePackMan.OpenFilePackageInGame(szPckFile) )
		{
			if( !strstr(GetCommandLineA(), "sepfile") )
			{
				char szInfo[1024];
				sprintf(szInfo, "文件包 [%s] 被损坏，无法打开，请重新安装游戏客户端！", szPckFile);
				MessageBoxA(NULL, szInfo, "文件包打开失败", MB_ICONSTOP | MB_OK);
				return false;
			}
		}
	}

	//	Open file package if it exists
	GetPrivateProfileStringA("PathFile", "PackageFile", "null", szFile, MAX_PATH, g_szIniFile);

	/*
	if (stricmp(szFile, "null"))
	{
		if (!af_OpenFilePackage(szFile))
		{
			MessageBox(NULL, _AL("Failed to open file package"), _AL("Error"), MB_OK);
			return false;
		}
	}*/

	//	Initlaize network module
	if (!_InitNetwork())
	{
		MessageBox(NULL, _AL("Failed initialize network module"), _AL("Error"), MB_OK);
		return false;
	}

	//	Load game configs
	if (!g_GameCfgs.Init("Configs\\element_client.cfg", "userdata\\SystemSettings.ini"))
	{
		a_LogOutput(1, "_InitGameApp(), Failed to load configs file");
		return false;
	}

	//	Set language code page
	a_SetCharCodePage((DWORD)g_GameCfgs.GetLanCodePage());

	return true;
}

void _FinalizeGameApp()
{
	//	Close network module
	_FinalizeNetwork();

	//	Close log system
	glb_CloseLogSystem();

	af_Finalize();
}

ATOM _RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof (WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)_WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= l_szClassName;
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

bool _CreateWebWnd(HINSTANCE hInstance, int nCmdShow, bool bFullScreen)
{

//	WNDCLASSEX		wc;
//	if ((l_webdll = LoadLibraryA("cwebpage.dll")))
//	{
//		// Get pointers to the EmbedBrowserObject, DisplayHTMLPage, DisplayHTMLStr, and UnEmbedBrowserObject
//		// functions, and store them in some globals.
//
//		// Get the address of the EmbedBrowserObject() function. NOTE: Only Reginald has this one
//		lpEmbedBrowserObject = (EmbedBrowserObjectPtr)::GetProcAddress((HMODULE)l_webdll, "EmbedBrowserObject");
//		GetInt2 pn2 = (GetInt2)::GetProcAddress(l_webdll,"GetInt2");
//		int n2 = (pn2)();
//
//		// Get the address of the UnEmbedBrowserObject() function. NOTE: Only Reginald has this one
//		lpUnEmbedBrowserObject = (UnEmbedBrowserObjectPtr)::GetProcAddress((HMODULE)l_webdll, "UnEmbedBrowserObject");
//
//		// Get the address of the DisplayHTMLPagePtr() function
//		lpDisplayHTMLPage = (DisplayHTMLPagePtr)::GetProcAddress((HMODULE)l_webdll, "DisplayHTMLPage");
//
//		// Get the address of the DisplayHTMLStr() function
//		lpDisplayHTMLStr = (DisplayHTMLStrPtr)::GetProcAddress((HMODULE)l_webdll, "DisplayHTMLStr");
//
//		lpResizeBrowser = (ResizeBrowserPtr)::GetProcAddress((HMODULE)l_webdll, "ResizeBrowser");
//		
//		ZeroMemory(&wc, sizeof(WNDCLASSEX));
//		wc.cbSize = sizeof(WNDCLASSEX);
//		wc.hInstance = hInstance;
//		wc.lpfnWndProc = (WNDPROC)_WndWebProc;
//		wc.lpszClassName = ClassName;
// 		RegisterClassEx(&wc);
//
//		HWND hWnd;
//		int iRenderWid = g_GameCfgs.GetSystemSettings().iRndWidth*3/4;
//		int iRenderHei = g_GameCfgs.GetSystemSettings().iRndHeight*3/4;
//		// Create another window with another browser object embedded in it.
////		if ((hWnd = CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST, ClassName, _AL("Microsoft's web site"), WS_OVERLAPPEDWINDOW,
////						0, 0, iRenderWid, iRenderHei,
//// 						NULL, NULL, hInstance, 0)))
//		if ((hWnd = CreateWindowEx(WS_EX_TOPMOST, ClassName, _AL("Microsoft's web site"), WS_CHILD|WS_CAPTION|WS_CLIPSIBLINGS,
//						0, 0, iRenderWid, iRenderHei,
//						l_hMainWnd, NULL, hInstance, 0)))
//		{
//			l_hWebWnd = hWnd;
////			// For this window, display a URL. This could also be a HTML file on disk such as "c:\\myfile.htm".
////			(lpDisplayHTMLPage)(l_hWebWnd, "http://www.cnblogs.com/JustinYoung/articles/game_xeno_tactic2.html");
//			(lpDisplayHTMLPage)(l_hWebWnd, "http://www.wanmei.com/");
//
//			// Show the window.
////			ShowWindow(l_hWebWnd, nCmdShow);
////			UpdateWindow(l_hWebWnd);
//		}
//		
//		//	Adjust window position
//		RECT rcWnd = {0, 0, iRenderWid, iRenderHei};
//		AdjustWindowRect(&rcWnd, WS_OVERLAPPEDWINDOW, FALSE);
//	
//		int w = rcWnd.right - rcWnd.left;
//		int h = rcWnd.bottom - rcWnd.top;
//		int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
//		int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;
//
////		MoveWindow(l_hWebWnd, x, y, w, h, FALSE);
//		
//		w = w*2;
//		h = h*2;
//		x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
//		y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;
////		SetWindowPos(l_hMainWnd, l_hWebWnd, x,y,w, h, SWP_SHOWWINDOW);
////		SetWindowPos(l_hWebWnd, HWND_TOPMOST, x, y, w, h, SWP_NOMOVE);
// 	}
	return true;
}

bool _CreateMainWnd(HINSTANCE hInstance, int nCmdShow, bool bFullScreen)
{
	l_hInstance = hInstance;
	int iRenderWid = g_GameCfgs.GetSystemSettings().iRndWidth;
	int iRenderHei = g_GameCfgs.GetSystemSettings().iRndHeight;

	
	DWORD dwStyles = WS_POPUP;
	if (!bFullScreen)
	{
//		dwStyles |= WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME | WS_CLIPCHILDREN;
		dwStyles |= WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME;
	}

	DWORD dwExStyle = 0;
	if( bFullScreen )
		dwExStyle = WS_EX_TOPMOST;

	ACHAR szTitle[MAX_PATH+64];

	if (gr.get_state() == game_record::enum_gr_replay)
	{
		a_strcpy(szTitle, AS2AC(g_GameCfgs.GetWindowReplayTitle()));
		a_strcat(szTitle, _record_file);
	}
	else
		a_strcpy(szTitle, AS2AC(g_GameCfgs.GetWindowTitle()));

	HWND hWnd = CreateWindowEx(dwExStyle, l_szClassName, szTitle, dwStyles,
					0, 0, iRenderWid, iRenderHei, NULL, NULL, hInstance, NULL);
	if (!hWnd)						    
		return false;
	
	l_hMainWnd = hWnd;

	if (!bFullScreen)
	{
		//	Adjust window position
		RECT rcWnd = {0, 0, iRenderWid, iRenderHei};
		AdjustWindowRect(&rcWnd, dwStyles, FALSE);
	
		int w = rcWnd.right - rcWnd.left;
		int h = rcWnd.bottom - rcWnd.top;
		int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
		int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

		MoveWindow(l_hMainWnd, x, y, w, h, FALSE);
	}

	//	Show main window
	ShowWindow(l_hMainWnd, nCmdShow);
	UpdateWindow(l_hMainWnd);
		
	//	we force set foreground window to ensure our main window is activated
	SetForegroundWindow(l_hMainWnd);
	return true;
}

LRESULT CALLBACK _WndWebProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch( message)
	{
	case WM_SIZE:
			// Resize the browser object to fit the window
//		if(lpResizeBrowser)	
// 			(lpResizeBrowser)(hWnd, LOWORD(lParam), HIWORD(lParam));
		return 0;
		break;
	case WM_CREATE:
		// Embed the browser object into our host window. We need do this only
		// once. Note that the browser object will start calling some of our
		// IOleInPlaceFrame and IOleClientSite functions as soon as we start
		// calling browser object functions in EmbedBrowserObject().
//		if(lpEmbedBrowserObject)
//		{
//		
//		if ((lpEmbedBrowserObject)(hWnd)) return(-1);
//
//		// Another window created with an embedded browser object
//		++WindowCount;
// 		}
		return(0);
		break;

	case WM_QUIT:
	case WM_CLOSE:
	case WM_DESTROY:
		ShowWindow(l_hWebWnd, SW_HIDE);
		return(0);
		//	Do nothing to mask ALT-F4
		break;
	case WM_NCLBUTTONDOWN:
		return 0;
 		break;
	}

	return(DefWindowProc(hWnd, message, wParam, lParam));
}

LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool bCaptured = false;

	if (message == WM_RBUTTONDOWN)
	{
		::SetCapture(l_hMainWnd);
		bCaptured = true;
	}
	else if (message == WM_RBUTTONUP && bCaptured)
	{
		::ReleaseCapture();
		bCaptured = false;
	}

	if (bCaptured && message == WM_MOUSEMOVE)
	{
		A3DDevice* pDev = g_pGame->GetA3DDevice();

		if (pDev->GetShowCursor())
		{
			A3DCursor* pCursor = pDev->GetCursor();
			if (pCursor) pCursor->OnSysSetCursor();
		}
		else
			SetCursor(NULL);
	}

	SuspendRenderThread();

	::EnterCriticalSection(&g_csRenderThread);
	bool b = false;
	if( g_pGame )
		b = g_pGame->WndProc(hWnd, message, wParam, lParam);
	::LeaveCriticalSection(&g_csRenderThread);

	if (g_bMultiThreadRenderMode)
		ResumeRenderThread();

	return b ? 0 : DefWindowProc(hWnd, message, wParam, lParam);
}

//	Initlaize network module
bool _InitNetwork()
{
	using namespace GNET;

	//	Initialize windows socket module
#ifdef USING_DEFENCE
	Collector::Startup();
#endif

#ifndef USING_TRACER
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		a_LogOutput(1, "_InitNetwork, Cannot initialize Windows Sockets !");
    	return false;
	}
#endif

	AString strFile = g_szWorkDir;
	strFile += "\\dbserver.conf";
	GNET::Conf::GetInstance(strFile);
	GNET::PollIO::Init();

	return true;
}

//	Finalize network module
void _FinalizeNetwork()
{
	using namespace GNET;

	PollIO::Close();
	Sleep(1000);

#ifdef USING_DEFENCE
	Collector::Cleanup();
#endif

#ifndef USING_TRACER
	WSACleanup();
#endif
}
