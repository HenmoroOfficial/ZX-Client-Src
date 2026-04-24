/*
 * FILE: ExplorerMan.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Wangrui, 2010/11/9
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "StdAfx.h"
#include "windowsx.h"
#include "ExplorerMan.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "DlgBaseExplorer.h"
#include <htmlhelp.h>
#include "msgtransfun.h"
#include "windowsx.h"
#include "Wininet.h"
#include <urlmon.h>

#pragma comment(lib, "htmlhelp.lib")
#pragma comment(lib, "urlmon.lib")

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#undef  TRACE //to avoid: warning C4005: 'TRACE' : macro redefinition
#define TRACE 
#define WAIT_TIME 500
typedef HRESULT  (WINAPI* TYP_URLDownloadToFileW)(LPUNKNOWN,LPCWSTR,LPCWSTR,DWORD,LPBINDSTATUSCALLBACK);
TYP_URLDownloadToFileW xURLDownloadToFileW = URLDownloadToFileW; //IE 7.0 的URLMON 无法获取下载文件的大小
#define AGENT_NAME "Microsoft Internet Explorer"

#ifdef _ALPHA_BUILD
#define BEGIN_BROWSER_CALL __try {
#define END_BROWSER_CALL } __except (EXCEPTION_EXECUTE_HANDLER) \
	{\
		ASSERT(0);\
		char msg[2*MAX_PATH];\
		char szFileTitle[MAX_PATH];\
		sprintf(msg, "%s", __FILE__);\
		GetFileTitleA(msg, szFileTitle, MAX_PATH);\
		sprintf(msg, "浏览器异常: %s,%d, 请截屏发给客户端程序进行处理！", szFileTitle, __LINE__);\
		MessageBoxA(NULL, "Browser Error!!", msg, MB_OK);\
	}
#else
#define BEGIN_BROWSER_CALL __try {
#define END_BROWSER_CALL } __except (EXCEPTION_EXECUTE_HANDLER){}
#endif

BOOL GetUrlFileSize(const WCHAR* Url, UINT* plength)   
{   
	HINTERNET   m_Session=::InternetOpenA(AGENT_NAME,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);   
	HINTERNET   hHttpFile=NULL;   
	char    szSizeBuffer[32]="\0";   
	DWORD   dwLengthSizeBuffer= sizeof(szSizeBuffer);   
	UINT    dwFileSize =0;   
	BOOL    bSuccessful=FALSE;   
	//Opening   the   Url   and   getting   a   Handle   for   HTTP   file   
	hHttpFile = InternetOpenUrlW(m_Session,(const WCHAR*)Url,NULL,0,0,0);   
	if(hHttpFile)   
	{
		//Getting   the   size   of   HTTP   Files   
		BOOL   bQuery  =  ::HttpQueryInfoA(hHttpFile,HTTP_QUERY_CONTENT_LENGTH,szSizeBuffer, &dwLengthSizeBuffer, NULL);   
		if(bQuery==TRUE)  
		{
			//Allocating   the   memory   space   for   HTTP   file   contents   
			dwFileSize=atol(szSizeBuffer);  
			bSuccessful = true;
		}
		::InternetCloseHandle(hHttpFile);   //   Close   the   connection.     
	}
	else   
	{   
		//Connection   failed.   
		bSuccessful = FALSE;   
	}   
	*plength = dwFileSize;   
	::InternetCloseHandle(m_Session);   
	return   bSuccessful;   
}   

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

const AWString g_strBlankUrl = _AL("about:blank");

//////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CBSCallbackImpl
//	
///////////////////////////////////////////////////////////////////////////

CBSCallbackImpl::CBSCallbackImpl(CExplorer* pExplorer, HANDLE hEventStop)
{
	m_pExplorer = pExplorer;  // the window handle to display status
	m_hEventStop = hEventStop;  // the event object to signal to stop
	m_ulObjRefCount = 1;
	m_filesize = 0;
}

// IUnknown
STDMETHODIMP CBSCallbackImpl::QueryInterface(REFIID riid, void **ppvObject)
{
	TRACE(_T("IUnknown::QueryInterface\n"));
	
	*ppvObject = NULL;
	
	// IUnknown
	if (::IsEqualIID(riid, __uuidof(IUnknown)))
	{
		TRACE(_T("IUnknown::QueryInterface(IUnknown)\n"));
		
		*ppvObject = this;
	}
	// IBindStatusCallback
	else if (::IsEqualIID(riid, __uuidof(IBindStatusCallback)))
	{
		TRACE(_T("IUnknown::QueryInterface(IBindStatusCallback)\n"));
		
		*ppvObject = static_cast<IBindStatusCallback *>(this);
	}
	
	if (*ppvObject)
	{
		(*reinterpret_cast<LPUNKNOWN *>(ppvObject))->AddRef();
		
		return S_OK;
	}
	
	return E_NOINTERFACE;
}                                             

STDMETHODIMP_(ULONG) CBSCallbackImpl::AddRef()
{
	TRACE(_T("IUnknown::AddRef\n"));
	
	return ++m_ulObjRefCount;
}

STDMETHODIMP_(ULONG) CBSCallbackImpl::Release()
{
	TRACE(_T("IUnknown::Release\n"));
	
	return --m_ulObjRefCount;
}

// IBindStatusCallback
STDMETHODIMP CBSCallbackImpl::OnStartBinding(DWORD, IBinding *)
{
	TRACE(_T("IBindStatusCallback::OnStartBinding\n"));
	
	return S_OK;
}

STDMETHODIMP CBSCallbackImpl::GetPriority(LONG *)
{
	TRACE(_T("IBindStatusCallback::GetPriority\n"));
	
	return E_NOTIMPL;
}

STDMETHODIMP CBSCallbackImpl::OnLowResource(DWORD)
{
	TRACE(_T("IBindStatusCallback::OnLowResource\n"));
	
	return S_OK;
}

STDMETHODIMP CBSCallbackImpl::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText)
{
#ifdef _DEBUG
	static const LPCTSTR plpszStatus[] = 
	{
	   	    _T("BINDSTATUS_FINDINGRESOURCE"),  // 1
			_T("BINDSTATUS_CONNECTING"),
			_T("BINDSTATUS_REDIRECTING"),
			_T("BINDSTATUS_BEGINDOWNLOADDATA"),
			_T("BINDSTATUS_DOWNLOADINGDATA"),
			_T("BINDSTATUS_ENDDOWNLOADDATA"),
			_T("BINDSTATUS_BEGINDOWNLOADCOMPONENTS"),
			_T("BINDSTATUS_INSTALLINGCOMPONENTS"),
			_T("BINDSTATUS_ENDDOWNLOADCOMPONENTS"),
			_T("BINDSTATUS_USINGCACHEDCOPY"),
			_T("BINDSTATUS_SENDINGREQUEST"),
			_T("BINDSTATUS_CLASSIDAVAILABLE"),
			_T("BINDSTATUS_MIMETYPEAVAILABLE"),
			_T("BINDSTATUS_CACHEFILENAMEAVAILABLE"),
			_T("BINDSTATUS_BEGINSYNCOPERATION"),
			_T("BINDSTATUS_ENDSYNCOPERATION"),
			_T("BINDSTATUS_BEGINUPLOADDATA"),
			_T("BINDSTATUS_UPLOADINGDATA"),
			_T("BINDSTATUS_ENDUPLOADINGDATA"),
			_T("BINDSTATUS_PROTOCOLCLASSID"),
			_T("BINDSTATUS_ENCODING"),
			_T("BINDSTATUS_VERFIEDMIMETYPEAVAILABLE"),
			_T("BINDSTATUS_CLASSINSTALLLOCATION"),
			_T("BINDSTATUS_DECODING"),
			_T("BINDSTATUS_LOADINGMIMEHANDLER"),
			_T("BINDSTATUS_CONTENTDISPOSITIONATTACH"),
			_T("BINDSTATUS_FILTERREPORTMIMETYPE"),
			_T("BINDSTATUS_CLSIDCANINSTANTIATE"),
			_T("BINDSTATUS_IUNKNOWNAVAILABLE"),
			_T("BINDSTATUS_DIRECTBIND"),
			_T("BINDSTATUS_RAWMIMETYPE"),
			_T("BINDSTATUS_PROXYDETECTING"),
			_T("BINDSTATUS_ACCEPTRANGES"),
			_T("???")  // unknown
	};
#endif
	
	TRACE(_T("IBindStatusCallback::OnProgress\n"));
	
	TRACE(_T("ulProgress: %lu, ulProgressMax: %lu\n"),  ulProgress, ulProgressMax);
	
	TRACE(_T("ulStatusCode: %lu "), ulStatusCode);
	
	if(ulStatusCode == BINDSTATUS_CONNECTING)
	{
		m_filesize = 0;
	}
	if (ulStatusCode < UF_BINDSTATUS_FIRST ||
		ulStatusCode > UF_BINDSTATUS_LAST)
	{
		ulStatusCode = UF_BINDSTATUS_LAST + 1;
	}
	if(((ulStatusCode ==BINDSTATUS_DOWNLOADINGDATA)||(ulStatusCode ==BINDSTATUS_CACHEFILENAMEAVAILABLE))&&(m_filesize ==0))
	{
		if(ulProgress == ulProgressMax)
		{
		   if(!m_pExplorer->GetDownloadingUrlFileSize(&m_filesize))
			  m_filesize = 1;
		}
	}
#ifdef _DEBUG
	TRACE(_T("(%s), szStatusText: %ls\n"),
		plpszStatus[ulStatusCode - UF_BINDSTATUS_FIRST],
		szStatusText);
#endif
	
	if (m_pExplorer)
	{
		if(ulProgress == ulProgressMax)
		{
			if((m_filesize>ulProgressMax)&&(m_filesize>1))
			{
				ulProgressMax = m_filesize;
			}
		}
		m_pExplorer->SetDownloadStatus(ulProgress, ulProgressMax, ulStatusCode, szStatusText );
	}
	
	if (m_hEventStop)
	{
		if (::WaitForSingleObject(m_hEventStop, 0) == WAIT_OBJECT_0)
		{
			TRACE(_T("Canceled.\n"));
			return E_ABORT;  // canceled by the user
		}
	}
	
	return S_OK;
}

STDMETHODIMP CBSCallbackImpl::OnStopBinding(HRESULT, LPCWSTR)
{
	TRACE(_T("IBindStatusCallback::OnStopBinding\n"));
	
	return S_OK;
}

STDMETHODIMP CBSCallbackImpl::GetBindInfo(DWORD *, BINDINFO *)
{
	TRACE(_T("IBindStatusCallback::GetBindInfo\n"));
	
	return S_OK;
}

STDMETHODIMP CBSCallbackImpl::OnDataAvailable(DWORD, DWORD, FORMATETC *, STGMEDIUM *)
{
	TRACE(_T("IBindStatusCallback::OnDataAvailable\n"));
	
	return S_OK;
}

STDMETHODIMP CBSCallbackImpl::OnObjectAvailable(REFIID, IUnknown *)
{
	TRACE(_T("IBindStatusCallback::OnObjectAvailable\n"));
	
	return S_OK;
}


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CExplorerMan
//	
///////////////////////////////////////////////////////////////////////////

CExplorerMan CExplorerMan::s_instance;

/*CExplorerMan::CExplorerMan()
{
	memset(m_aExplorers, 0, sizeof(m_aExplorers));
}
*/
CExplorerMan::~CExplorerMan()
{
	for (int i=EXPLORER_DEFAULT; i<EXPLORER_NUM; i++)
	{
		if (m_aExplorers[i])
		{
			delete m_aExplorers[i];
			m_aExplorers[i] = NULL;
		}
	}

	CPWBrowser::TermContainer();
}

void CExplorerMan::Release()
{

}

bool CExplorerMan::CreateExplorer(int iIdx, CDlgBaseExplorer* pDlg)
{
	if (!pDlg || iIdx<=EXPLORER_ERROR || iIdx>=EXPLORER_NUM)
	{
		return false;
	}

	if (!m_aExplorers[iIdx])
	{
		m_aExplorers[iIdx] = new CExplorer(g_pGame->GetGameInit().hWnd);
	}
	
	m_aExplorers[iIdx]->BindUI(pDlg);
	if (!m_aExplorers[iIdx]->Create())
	{
		if (m_aExplorers[iIdx]->ReleaseUI())
		{
			delete m_aExplorers[iIdx];
			m_aExplorers[iIdx] = NULL;
		}
		
		return false;
	}

	return true;
}

bool CExplorerMan::PreCreateExplorer( HWND hParentWnd )
{
	//if ( hParentWnd )


	if(!xURLDownloadToFileW)
	{
		HMODULE hUrlMon= NULL;//GetModuleHandleA("urlmon.dll");
		int i=10;
		while((hUrlMon)&&(i>0))
		{
			i--;
			FreeLibrary(hUrlMon);
			hUrlMon= GetModuleHandleA("urlmon.dll");
		}

		if(!hUrlMon)
		{

			char path[512] ={0,};
			GetModuleFileNameA(NULL,path,256);
			char* name1 = strrchr(path,'\\');
			memset(name1,0,name1-path+256);
			strcpy(name1,"\\urlmon.dll");
			hUrlMon = LoadLibraryA(path);

			if(!hUrlMon)
			{
				hUrlMon = LoadLibraryA("urlmon.dll");
			}
		}
		xURLDownloadToFileW = (TYP_URLDownloadToFileW)GetProcAddress(hUrlMon,"URLDownloadToFileW");
	}


	bool bResult = true;
	for ( int i = EXPLORER_DEFAULT; i < EXPLORER_NUM; ++i )
	{
		if ( !m_aExplorers[i] )
		{
			m_aExplorers[i] = new CExplorer( hParentWnd );
		}

		//
		if ( !m_aExplorers[i]->PreCreate() )
		{
			delete m_aExplorers[i];
			m_aExplorers[i] = NULL;
			bResult = false;
		}

		//Hide the Browser.
		m_aExplorers[i]->HideBrowser();
	}

	//
	return bResult;
}

bool CExplorerMan::RemoveExplorer( int nIndex )
{
	if ( nIndex <= EXPLORER_ERROR || nIndex >= EXPLORER_NUM )
	{
		return false;
	}

	if ( m_aExplorers[nIndex] )
	{
		delete m_aExplorers[nIndex];
		m_aExplorers[nIndex] = NULL;
	}

	return true;
}

bool CExplorerMan::PreTranslateMessage( const MSG& msg )
{
	for ( int i = EXPLORER_DEFAULT; i < EXPLORER_NUM; ++i )
	{
		if ( m_aExplorers[i] )
		{
			CPWBrowser* pPWBrowser = m_aExplorers[i]->GetBrowser();
			if ( pPWBrowser && IsWindow(pPWBrowser->GetHandle()) )
			{
				if ( (msg.message < WM_KEYFIRST || msg.message > WM_KEYLAST) &&
				(msg.message < WM_MOUSEFIRST || msg.message > WM_MOUSELAST) )
				{
					continue;
				}

				HWND hwnd1 = msg.hwnd;
				HWND hParent = hwnd1;
				HWND hBrowser = pPWBrowser->GetHandle();
				int  nLoop = 0;
				const int MAX_LOOP = 4;
				if(msg.hwnd == pPWBrowser->GetHandle())
				{
					continue;
				}

				while ( IsWindow(hParent) && ( nLoop < MAX_LOOP ) )
				{
					hParent = GetAncestor(hParent,GA_PARENT);
					if ( hParent == pPWBrowser->GetCtrl() )
					{
						break;
					}
					nLoop++;
				}

				if ( hParent == pPWBrowser->GetCtrl() )
				{
					LRESULT lTranslated = ::SendMessage(pPWBrowser->GetCtrl(), WM_FORWARDMSG, 0, (LPARAM)&msg);
					return (lTranslated ? true : false);
				}
			}
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CExplorer
//	
///////////////////////////////////////////////////////////////////////////

CExplorer::CExplorer(HWND hWnd)
{
	m_pDlgBind = NULL;
	m_bInTimer = false;
	m_pBrowser = NULL;
	m_nWndHeight = 0;
	m_RectDisplay.Clear();
	m_hGameInitWnd = hWnd;
	m_bDrawOutside = false;
	m_bNoDocScroll = false;

	m_bDownLoading         = false;
	m_bUpdateDownData      = false;
	m_bUpdateDownLoadState = false;
	m_bCancelCurDownLoad   = false;

	//m_pDlg = NULL;
	//m_bInTimer = false;
	//m_pBrowser = NULL;
	m_hDownloadThread = NULL;	
	m_dwDownloadThreadId = 0;	

	m_mutex = CreateMutex(NULL,FALSE,NULL);
	m_event = CreateEvent(NULL,FALSE,FALSE,NULL);
	OnInitDialog();
	InitTaskList();
}

CExplorer::~CExplorer()
{
	_MyRelease();
}

void CExplorer::_MyRelease()
{
	BEGIN_BROWSER_CALL
	//MSDN documentation says "A thread cannot use DestroyWindow to destroy a window created by a different thread.

	if(m_hDownloadThread)
	{
		// (m_hDownloadThread);
		// 等待线程退出
		SetEvent(m_event);
		WaitForSingleObject(m_hDownloadThread,INFINITE);
	}
	CloseHandle(m_mutex);
	CloseHandle(m_event);

	if(m_pBrowser)
	{
		ReleaseUI();
		if(m_pBrowser->CanDelete(GetCurrentThreadId()))
		{
			delete m_pBrowser;
		}
		else
		{
			m_pBrowser->DelayDestroy();
		}
		m_pBrowser = NULL;
	}
	END_BROWSER_CALL
}
bool CExplorer::OnInitDialog()
{
	memset(m_wsPath,0,MAX_PATH*sizeof(WCHAR));
	GetCurrentDirectory(MAX_PATH,m_wsPath);
	if(m_wsPath[wcslen(m_wsPath)-1]!=_T('\\'))
	{
		wcscat(m_wsPath,_T("\\MyMusics\\"));
	}
	else
	{
		wcscat(m_wsPath,_T("MyMusics\\"));
	}
	return true;
}
BOOL CExplorer::InitTaskList()
{
	m_vecTaskDone.clear();
	m_vecTaskToDo.clear();
	return TRUE;
}
void CExplorer::SetUIScrollHeight(int nHeight)
{
	if (m_pDlgBind && m_pDlgBind->HasOuterScroll())
		m_pDlgBind->InitScroll(nHeight, m_nWndHeight);
}

void CExplorer::NavigateUrl(const CHAR* Url)
{
	BEGIN_BROWSER_CALL
	if (m_pBrowser)
		m_pBrowser->NavigateUrl(Url);
	END_BROWSER_CALL
}

void CExplorer::NavigateUrl(const WCHAR* Url)
{
	BEGIN_BROWSER_CALL
	if (m_pBrowser)
		m_pBrowser->NavigateUrl(Url);
	END_BROWSER_CALL
}

void CExplorer::GoForward()
{
	BEGIN_BROWSER_CALL
	if (m_pBrowser)
		m_pBrowser->GoForward();
	END_BROWSER_CALL
}

void CExplorer::GoBackward()
{
	BEGIN_BROWSER_CALL
	if (m_pBrowser)
		m_pBrowser->GoBackward();
	END_BROWSER_CALL
}

void CExplorer::Refresh()
{
	BEGIN_BROWSER_CALL
	if (m_pBrowser)
		m_pBrowser->Refresh();
	END_BROWSER_CALL
}

void CExplorer::HideBrowser()
{
	BEGIN_BROWSER_CALL
	if (m_pBrowser)
	{
		m_pBrowser->ShowWindow(SW_HIDE);
	}
	END_BROWSER_CALL
}

void CExplorer::ShowBrowser()
{
	BEGIN_BROWSER_CALL
	if (m_pBrowser)
	{
		m_pBrowser->ShowWindow(SW_SHOW);
	}
	END_BROWSER_CALL
}

void CExplorer::_SyncOuterScrollBar()
{
	if (!m_pDlgBind || !m_pDlgBind->HasOuterScroll())
	{
		return;
	}

	HWND pWnd = m_pBrowser->m_hWnd;
	HWND pForeWnd = GetForegroundWindow();
	if (pForeWnd==pWnd)
	{
		int nScrollPos = m_pBrowser->GetScrollPos();
		m_pDlgBind->UpdateUIScroll(nScrollPos);
	}
	else
	{
		int nScrollPos = m_pDlgBind->GetScrollTop();
		m_pBrowser->MoveScrollPos(nScrollPos);
	}
}

void CExplorer::OnWinTimer()
{
	BEGIN_BROWSER_CALL
	if (m_bInTimer || !m_pBrowser)
	{
		return;
	}

	if(!::IsWindowVisible(m_hGameInitWnd) || ::IsIconic(m_hGameInitWnd))
	{
		HideBrowser();
		return;
	}

	m_bInTimer = true;
	if (m_pDlgBind)
	{
		PAUIWINDOWPICTURE pWPic = m_pDlgBind->GetWPic();
		if (!pWPic)
		{
			return;
		}

		_SyncOuterScrollBar();
		if(m_pDlgBind->IsActive())
		{
			RECT rc;
			SIZE sz = pWPic->GetSize();
			POINT pt = pWPic->GetPos();
			POINT ptc ={0,};
			RECT rc1;
			::GetClientRect(m_hGameInitWnd, &rc1);
			::ClientToScreen(m_hGameInitWnd, &ptc);
			rc.top =pt.y +rc1.top+ptc.y;
			rc.bottom =pt.y+sz.cy +rc1.top+ptc.y;
			rc.left = pt.x +rc1.left+ptc.x;
			rc.right = pt.x+sz.cx +rc1.left+ptc.x;
			if(m_pBrowser->IsReady())
			{
				if(::IsWindow(m_pBrowser->m_hWnd))
				{
					m_pBrowser->SetWindowPos(HWND_TOP, &rc, SWP_ASYNCWINDOWPOS|SWP_NOACTIVATE);
					m_pBrowser->SetActive(true);
					if (pWPic->IsExternalDraw())
					{
					//	m_pBrowser->SetTransparent(255);
					}
				}
			}
			ShowBrowser();
			if (pWPic->IsFocus())
			{
				if(GetActiveWindow()==m_hGameInitWnd)
				{
					::SetActiveWindow(m_pBrowser->m_hWnd);
				}
			}
		}
		else
		{
			if(m_pDlgBind->IsShow())
			{			
				PAUIWINDOWPICTURE pWPic = m_pDlgBind->GetWPic();
				RECT rc;
				SIZE sz = pWPic->GetSize();
				POINT pt = pWPic->GetPos();
				POINT ptc = {0,};
				RECT rc1;
				::GetClientRect(m_hGameInitWnd, &rc1);
				::ClientToScreen(m_hGameInitWnd, &ptc);
				rc.top =pt.y +rc1.top+ptc.y;
				rc.bottom =pt.y+sz.cy +rc1.top+ptc.y;
				rc.left = pt.x +rc1.left+ptc.x;
				rc.right = pt.x+sz.cx +rc1.left+ptc.x;
				if(m_pBrowser->IsReady())
				{
					if(::IsWindow(m_pBrowser->m_hWnd))
					{
						ShowBrowser();
						m_pBrowser->SetWindowPos(HWND_TOP,&rc,SWP_ASYNCWINDOWPOS|SWP_NOACTIVATE);
						m_pBrowser->SetActive(false);
						if (pWPic->IsExternalDraw())
						{
//							m_pBrowser->SetTransparent(255);
						}
					}
				}
			}
			else
			{
				if (pWPic->IsExternalDraw())
				{
//					m_pBrowser->SetTransparent(0);
				}
				HideBrowser();
			}
		}
	}
	else
	{
		HideBrowser();
	}

	m_bInTimer = false;
	END_BROWSER_CALL	
}

void CExplorer::OnDocBodyInnerText(const AWString& strText)
{
	if (m_pDlgBind)
	{
		m_pDlgBind->OnDocBodyInnerText(strText);
	}
}

void CExplorer::SetDrawOutside(bool bOutside)
{
	BEGIN_BROWSER_CALL
	m_bDrawOutside = bOutside;
	if (m_pBrowser)
	{
	//	m_pBrowser->SetUseOuterTrans(true);
	}
	END_BROWSER_CALL
}

void CExplorer::SetBrwoserTrans(BYTE byTrans)
{
	BEGIN_BROWSER_CALL
	if (m_pBrowser)
	{
//		m_pBrowser->SetTransparent(byTrans);
	}
	END_BROWSER_CALL
}

void CExplorer::SetNoDocScroll(bool bNoScroll)
{
	m_bNoDocScroll = bNoScroll;
}

bool CExplorer::Create()
{
	bool bResult = false;
	BEGIN_BROWSER_CALL
	bResult = _Create();
	END_BROWSER_CALL
	return bResult;
}

bool CExplorer::PreCreate()
{
	bool bResult = false;
	if ( !m_pBrowser && (GetWindowThreadProcessId(m_hGameInitWnd, NULL) == GetCurrentThreadId()) )
	{
		m_pBrowser = new CPWBrowser(this);
//		m_pBrowser->SetNoDocScroll(m_bNoDocScroll);
		RECT rcCreate = {0, 0, 400, 400};
/*		RECT rcCreate, rcClient;
		SIZE szPic = pWPic->GetSize();
		POINT ptPic = pWPic->GetPos();
		POINT ptCliToScr ={0,};
		::GetClientRect(g_pGame->GetGameInit().hWnd, &rcClient);
		::ClientToScreen(g_pGame->GetGameInit().hWnd, &ptCliToScr);
		rcCreate.top = ptPic.y + rcClient.top + ptCliToScr.y;
		rcCreate.bottom = ptPic.y + szPic.cy + rcClient.top + ptCliToScr.y;
		rcCreate.left = ptPic.x + rcClient.left + ptCliToScr.x;
		rcCreate.right = ptPic.x + szPic.cx + rcClient.left + ptCliToScr.x;*/
		if(m_pBrowser->Create(m_hGameInitWnd, rcCreate))
		{
//			pWPic->SetHWnd(m_pBrowser->m_hWnd);
//			pWPic->SetMessageHandler(m_pDlgBind);
			if (m_pBrowser->m_hWnd)
			{
/*				cx = GetSystemMetrics(SM_CXBORDER);
				cy = GetSystemMetrics(SM_CYBORDER);
				cxVScroll = m_bNoDocScroll ? 0 : GetSystemMetrics(SM_CXVSCROLL);
				cyHScroll = m_bNoDocScroll ? 0 : GetSystemMetrics(SM_CYHSCROLL);
				m_RectDisplay.top = 0 + cy + 1;
				m_RectDisplay.left = 0 + cx + 2;
				m_RectDisplay.right = pWPic->GetSize().cx - cxVScroll - 3;
				m_RectDisplay.bottom = pWPic->GetSize().cy - cyHScroll - 2;
				//pWPic->SetDisplayRect(false, m_RectDisplay);
				m_nWndHeight = m_RectDisplay.Height();*/
			}
			//
			bResult = true;
		}
	}

	//
	return bResult;
}

bool CExplorer::_Create()
{
	//创建浏览器与程序UI同一线程（有问题，假如浏览器窗口死掉，造成程序死掉）
	int cx, cy, cxVScroll, cyHScroll;
	if (!m_pBrowser)
	{
		if(m_pDlgBind)
		{
			if(GetWindowThreadProcessId(m_hGameInitWnd, NULL)==GetCurrentThreadId())
			{
				m_pBrowser = new CPWBrowser(this);
				PAUIWINDOWPICTURE pWPic = m_pDlgBind->GetWPic();
				if (!pWPic)
					return false;
//				m_pBrowser->SetUseOuterTrans(pWPic->IsExternalDraw());
//				m_pBrowser->SetNoDocScroll(m_bNoDocScroll);
				RECT rcCreate, rcClient;
				SIZE szPic = pWPic->GetSize();
				POINT ptPic = pWPic->GetPos();
				POINT ptCliToScr ={0,};
				::GetClientRect(g_pGame->GetGameInit().hWnd, &rcClient);
				::ClientToScreen(g_pGame->GetGameInit().hWnd, &ptCliToScr);
				rcCreate.top = ptPic.y + rcClient.top + ptCliToScr.y;
				rcCreate.bottom = ptPic.y + szPic.cy + rcClient.top + ptCliToScr.y;
				rcCreate.left = ptPic.x + rcClient.left + ptCliToScr.x;
				rcCreate.right = ptPic.x + szPic.cx + rcClient.left + ptCliToScr.x;
				
				if(m_pBrowser->Create(m_hGameInitWnd, rcCreate))
				{
					pWPic->SetHWnd(m_pBrowser->m_hWnd);
					pWPic->SetMessageHandler(m_pDlgBind);
					if (m_pBrowser->m_hWnd)
					{
						cx = GetSystemMetrics(SM_CXBORDER);
						cy = GetSystemMetrics(SM_CYBORDER);
						cxVScroll = m_bNoDocScroll ? 0 : GetSystemMetrics(SM_CXVSCROLL);
						cyHScroll = m_bNoDocScroll ? 0 : GetSystemMetrics(SM_CYHSCROLL);
						m_RectDisplay.top = 0 + cy + 1;
						m_RectDisplay.left = 0 + cx + 2;
						m_RectDisplay.right = pWPic->GetSize().cx - cxVScroll - 3;
						m_RectDisplay.bottom = pWPic->GetSize().cy - cyHScroll - 2;
						//pWPic->SetDisplayRect(false, m_RectDisplay);
						m_nWndHeight = m_RectDisplay.Height();
					}
				}	
			}
			else
			{
				return false;
			}
		}
		else
		{
			if(GetWindowThreadProcessId(m_hGameInitWnd, NULL)==GetCurrentThreadId())
			{
				m_pBrowser = new CPWBrowser(this);
				RECT rc;
				rc.left = -100;
				rc.right = 0;
				rc.bottom = -100;
				rc.top =0;
				m_pBrowser->Create(m_hGameInitWnd,rc);
				m_pBrowser->ShowWindow(SW_HIDE);
			}
			//return false;
		}
	}
	else if (m_pDlgBind)
	{
		PAUIWINDOWPICTURE pWPic = m_pDlgBind->GetWPic();
		if (!pWPic)
			return false;
		pWPic->SetHWnd(m_pBrowser->m_hWnd);
		pWPic->SetMessageHandler(m_pDlgBind);
		if (m_pBrowser->m_hWnd)
		{
			cx = GetSystemMetrics(SM_CXBORDER);
			cy = GetSystemMetrics(SM_CYBORDER);
			cxVScroll = m_bNoDocScroll ? 0 : GetSystemMetrics(SM_CXVSCROLL);
			cyHScroll = m_bNoDocScroll ? 0 : GetSystemMetrics(SM_CYHSCROLL);
			m_RectDisplay.top = 0 + cy + 1;
			m_RectDisplay.left = 0 + cx + 2;
			m_RectDisplay.right = pWPic->GetSize().cx - cxVScroll - 3;
			m_RectDisplay.bottom = pWPic->GetSize().cy - cyHScroll - 2;
			//pWPic->SetDisplayRect(false, m_RectDisplay);
			m_nWndHeight = m_RectDisplay.Height();
		}
	}

	if (m_pBrowser)
	{	
		if (m_pBrowser->IsWindow())
		{
			m_pBrowser->SetExplorer(this);
		}
	}

	return true;
}

bool CExplorer::BindUI(CDlgBaseExplorer* pDlg)
{
	if(!ReleaseUI())
		return false;
	
	m_pDlgBind = pDlg;
	m_bNoDocScroll = pDlg ? pDlg->IsNoDocScroll() : false;
	
	return true;
}

bool CExplorer::ReleaseUI()
{
	if (!m_pDlgBind)
		return true;

	if(m_pDlgBind->FreeExplorer())
	{
		m_pDlgBind= NULL;
		return true;
	}
	
	return true;
}

void CExplorer::OnTick()
{
	if (m_bUpdateDownData)
	{
		m_bUpdateDownData = false;
		//LuaEvent::UpdateExplorerDownloadStatus();
	}	
	if (m_bUpdateDownLoadState)
	{
		m_bUpdateDownLoadState = false;
		//LuaEvent::UpdateDownLoadStatus();
	}	
}

void CExplorer::SetWebBrowserSilent( bool bSilent )
{
	if ( m_pBrowser )
	{
//		m_pBrowser->SetWebBrowserSilent( bSilent );
	}
}
BOOL CExplorer::AddDownloadUrl(WCHAR *FileUrl)
{
	DWORD Res =0; 
	Res = WaitForSingleObject(m_mutex,WAIT_TIME);	
	DOWNLOADER_TASK task;
	switch (Res) 
	{
	case WAIT_FAILED:
		//提示错误发生，无法添加任务
		break;
	case WAIT_TIMEOUT:
		//提示阻塞，无法添加任务
		break;
	case WAIT_OBJECT_0:
	case WAIT_ABANDONED:
		WCHAR *name;
		name = wcsrchr(FileUrl,_T('/'));
		if(name==NULL)
		{

		}
		else
		{
			name ++;
			task.url = FileUrl;
			task.localfile = AWString(m_wsPath);
			task.localfile+=name;
			if(m_vecTaskToDo.size() > 0)
			{
				m_vecTaskToDo.insert(m_vecTaskToDo.end(), task);
			}
			else
			{
				m_vecTaskToDo.insert(m_vecTaskToDo.begin(), task);
			}
			m_bUpdateDownData = true;

			DWORD index;
			index = m_vecTaskToDo.size()-1;
			if((m_hDownloadThread==NULL)&&(m_dwDownloadThreadId==0))
			{
				m_hDownloadThread = CreateThread(NULL,0,CExplorer::DownloadThread,(LPVOID)this,0,&m_dwDownloadThreadId);
			}
		}
		ReleaseMutex(m_mutex);
		break;
	default:
		break;
	}
	return TRUE;
}

unsigned long WINAPI  CExplorer::DownloadThread(LPVOID pDlgExplorer)
{
	CExplorer *hDlgExplorer = (CExplorer *)pDlgExplorer;
	int iRet = -1;
	__try
	{
		hDlgExplorer->ThreadRun();
		iRet = 0;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		hDlgExplorer->m_hDownloadThread = NULL;
		hDlgExplorer->m_dwDownloadThreadId =0;
	}
	return iRet;
}

void CExplorer::ThreadRun()
{
	for(;;)
	{
		DWORD Res = WaitForSingleObject(m_mutex,INFINITE);
		switch(Res)
		{
		case WAIT_OBJECT_0:		
			if(m_vecTaskToDo.size()>0)
			{

				DOWNLOADER_TASK *ptask = m_vecTaskToDo.begin();
				DOWNLOADING_TASK task;
				task.url = ptask->url;
				task.localfile = ptask->localfile;
				m_currentTask = task;
				//如果有下载列表，就开始下载
				//Sleep(1000);
				CBSCallbackImpl bsc(this, m_event);				
				m_vecTaskToDo.erase(m_vecTaskToDo.begin());								
				ReleaseMutex(m_mutex);

				m_bDownLoading         = true;
				m_bUpdateDownLoadState = true;

				const HRESULT hr = xURLDownloadToFileW(NULL,
					task.url,(LPWSTR)((LPCWSTR)task.localfile),
					0,
					&bsc);
				if(hr == E_ABORT)
				{
					ReleaseMutex(m_mutex);
					if (!m_bCancelCurDownLoad)
					{
						return;
					}					
				}
				m_bDownLoading         = false;
				m_bUpdateDownLoadState = true;

				if (!m_bCancelCurDownLoad)
				{
					if(m_vecTaskDone.size()>0)
					{
						DOWNLOADER_TASK taskx;
						taskx.url = task.url;
						taskx.localfile = task.url;
						taskx.id = 0;
						m_vecTaskDone.insert(m_vecTaskDone.end(), taskx);
					}
					else
					{
						DOWNLOADER_TASK taskx;
						taskx.url = task.url;
						taskx.localfile = task.url;
						taskx.id = m_vecTaskDone.size();
						m_vecTaskDone.insert(m_vecTaskDone.begin(), taskx);
					}
				}
				else
				{
					m_bCancelCurDownLoad = false;
					_wremove(task.localfile.GetBuffer(0));
				}

				m_bUpdateDownData = true;
			}
			else
			{
				m_dwDownloadThreadId =0;
				m_hDownloadThread =NULL;
				ReleaseMutex(m_mutex);
				return;
				//ExitThread(0);
			}		
			break;
		case WAIT_ABANDONED:
			m_dwDownloadThreadId =0;
			m_hDownloadThread =NULL;
			ReleaseMutex(m_mutex);
			ExitThread(-1); //不明错误
			break;
		case  WAIT_FAILED:
			m_dwDownloadThreadId =0;
			m_hDownloadThread =NULL;
			ExitThread(-1); //不明错误
			break;
		}
	}
}


void CExplorer::SetDownloadStatus(ULONG ulProgress,ULONG ulProgressMax,LONG ulStatusCode,LPCWSTR szStatusText)
{
	m_currentTask.status.ulProgress = ulProgress;
	m_currentTask.status.ulProgressMax = ulProgressMax;
	m_currentTask.status.ulStatusCode = ulStatusCode;
	memset(m_szDownStatusText,0,sizeof(WCHAR)*STATUS_TXT_LEN);
	if(szStatusText!=NULL)
	{		
		if(wcslen(szStatusText)<STATUS_TXT_LEN-1)
		{
			wcscpy(m_szDownStatusText,szStatusText);
		}
		else
		{
			memcpy(m_szDownStatusText,szStatusText,sizeof(WCHAR)*(STATUS_TXT_LEN-1));
		}
	}	
}
BOOL CExplorer::GetDownloadingUrlFileSize(UINT* plength)
{
	return GetUrlFileSize(m_currentTask.url,plength);
}
bool CExplorer::GetDownloadState(abase::vector<AWString>& vecTaskDone, abase::vector<AWString>& vecTaskToDo, AWString& curDownFile, int &curDownProgress)
{
	DWORD res = WaitForSingleObject(m_mutex, IGNORE);
	if ((res== WAIT_OBJECT_0)||(res==WAIT_ABANDONED))
	{
		int index = 0;
		AWString str;
		abase::vector<DOWNLOADER_TASK>::const_iterator iter = m_vecTaskDone.begin();
		for (; iter != m_vecTaskDone.end(); ++iter)
		{
			index = (*iter).localfile.ReverseFind(L'/');
			if (index >= 0)
			{
				str = (*iter).localfile.Right((*iter).localfile.GetLength() - index - 1);
				vecTaskDone.push_back(str);
			}				
		}
		for (iter = m_vecTaskToDo.begin(); iter != m_vecTaskToDo.end(); ++iter)
		{
			index = (*iter).localfile.ReverseFind(L'\\');
			if (index >= 0)
			{
				str = (*iter).localfile.Right((*iter).localfile.GetLength() - index - 1);
				vecTaskToDo.push_back(str);
			}
		}

		index = m_currentTask.localfile.ReverseFind(L'\\');
		if (index >= 0)
		{
			curDownFile = m_currentTask.localfile.Right(m_currentTask.localfile.GetLength() - index - 1);
		}
		curDownProgress = (m_currentTask.status.ulProgressMax > 0.001)?m_currentTask.status.ulProgress/m_currentTask.status.ulProgressMax:0;

		ReleaseMutex(m_mutex);
		return true;
	}
	return false;
}

double CExplorer::GetDownloadRate()
{	
	double rate = 0.0;
	if (m_currentTask.status.ulProgressMax > 0)
	{
		rate = (double)m_currentTask.status.ulProgress / (double)m_currentTask.status.ulProgressMax;
	}
	return rate;
}

bool CExplorer::IsDownloading()
{
	return m_bDownLoading;
}

void CExplorer::CancelCurDownLoad()
{
	m_bCancelCurDownLoad = true;
	::SetEvent(m_event);
}

//-----------------------------------------------------------------------------
bool CExplorerInfoParser( CExplorer* pExplorer, ACString& strName, ACString& strPassword )
{
	//首先判断参数合法性
	if ( !pExplorer )
	{
		return false;
	}

	//先清除name 和password
	strName = _AL("");
	strPassword = _AL("");

	HRESULT hr = E_FAIL;
	CComPtr<IHTMLElementCollection> pAllElement = NULL;
	ACString strAgent, strUserID, strTicket;
	CPWBrowser* pPWBrowser = pExplorer->GetBrowser();
	if ( !pPWBrowser )
	{
		return false;
	}

/*	if ( !pPWBrowser->IsDocReady() )
	{
		return false;
	}
*/
	//--------------------------------------------------
	IDispatch* pDispDoc = NULL;
	IWebBrowser2* pWebBrowser = pPWBrowser->GetWebBrowser();
	if ( !pWebBrowser )
	{
		return false;
	}
	
	//===========================================
	bool bIsDocReady = false;
	READYSTATE rstate;
	VARIANT_BOOL bBusy;
	HRESULT hr1 = pWebBrowser->get_ReadyState(&rstate);
	HRESULT hr2 = pWebBrowser->get_Busy(&bBusy);
	if ( SUCCEEDED(hr1) && SUCCEEDED(hr2) )
	{
		if ( (rstate==READYSTATE_COMPLETE)&&(!bBusy) )
		{
			bIsDocReady = true;
		}
	}
	if ( !bIsDocReady )
	{
		return false;
	}
	//===========================================
	pWebBrowser->QueryInterface( IID_IDispatch, (void**)&pDispDoc);
//	IDispatch* pDispDoc = pPWBrowser->GetWebIDisPatch();
	if ( !pDispDoc )
	{
		return false;
	}

	IWebBrowser2* pWeb =NULL;
	pDispDoc->QueryInterface( IID_IWebBrowser2, (void**)&pWeb);
	if ( !pWeb )
	{
		return false;
	}

	IHTMLDocument *pDocument = NULL;
	pWeb->get_Document((IDispatch**)&pDocument);
	if ( !pDocument )
	{
		return false;
	}

	IHTMLDocument2* pDoc = NULL;
	pDocument->QueryInterface( IID_IHTMLDocument2, (void**)&pDoc );
	//--------------------------------------------------
	//IHTMLDocument2* pDoc = pPWBrowser->GetDocument2();
	if ( !pDoc )
	{
		return false;
	}

	hr = pDoc->get_all( &pAllElement );
	if ( !pAllElement )
	{
		return false;
	}

	long nItemCount = 0;
	IDispatch* ppvdispOption;
	IHTMLInputElement* ppvOption;
	ACString strKey, strValue;
	hr = pAllElement->get_length( &nItemCount );
	for ( int i = 0; i < nItemCount; ++i )
	{
		VARIANT index;
		index.vt		= VT_I4;
		index.intVal	= i;
		hr = pAllElement->item( index, index, &ppvdispOption );
		
		if ( ppvdispOption )
		{
			hr = ppvdispOption->QueryInterface( IID_IHTMLInputElement, (void**)&ppvOption );
			ppvdispOption->Release();
			
			if ( ppvOption )
			{
				BSTR pszText;
				hr = ppvOption->get_name(&(pszText));
				strKey = pszText;
				hr = ppvOption->get_value( &(pszText) );
				strValue = pszText;
				if ( strKey == _AL("agent") )
				{
					strAgent = strValue;
				}
				else if ( strKey == _AL("userId") )
				{
					strUserID = strValue;
				}
				else if ( strKey == _AL("ticket") )
				{
					strTicket = strValue;
				}

				ppvOption->Release();
			}
		}
	}

	//解析到了相关数据，则按照格式返回username 和password
	if ( !strAgent.IsEmpty() && !strUserID.IsEmpty() && !strTicket.IsEmpty() )
	{
		strName.Format( _AL("%s@%s@sso"), strUserID, strAgent );
		strPassword = strTicket;
		//---------------------------------------
		TCHAR pszBuf[] =_T("about:blank");
		CComVariant vempty, vUrl(pszBuf);
		pWebBrowser->Navigate2( &vUrl, &vempty, &vempty, &vempty, &vempty );	
		//---------------------------------------
		return true;
	}

	//
	return false;
}
//-----------------------------------------------------------------------------

