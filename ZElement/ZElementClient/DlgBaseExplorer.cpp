/*
 * FILE: DlgBaseExplorer.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Wangrui, 2010/11/10
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "StdAfx.h"
#include "DlgBaseExplorer.h"
#include "ExplorerMan.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Global.h"
#include "getkdctoken_re.hpp"
#include <Wininet.h>

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define INRECT_PT(pt,rc) ((rc.left<=pt.x)&&(rc.right>=pt.x)&&(rc.top<=pt.y)&&(rc.bottom>=pt.y))
#define INRECT_XY(x,y,rc) ((rc.left<=x)&&(rc.right>=x)&&(rc.top<=y)&&(rc.bottom>=y))

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

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

BOOL CALLBACK _ThreadEnumChildMouseProcExp(HWND hwnd,LPARAM lParam)
{
	if (IsWindowVisible(hwnd))
	{
		LRESULT res;
		RECT rc,rc1;
		POINT pt,ptc;
		pt.x =0;
		pt.y =0;
		SimuMouseEvent* p_SLparam =(SimuMouseEvent *)lParam;
		GetWindowRect(hwnd,&rc);
		GetClientRect(hwnd,&rc1);
		ClientToScreen(hwnd,&pt);
		char str[32]={0,};
		if(INRECT_PT(p_SLparam->pt,rc))
		{		
			EnumChildWindows(hwnd,_ThreadEnumChildMouseProcExp,lParam);
			if(!p_SLparam->flag)
			{
				ptc.x = p_SLparam->pt.x - pt.x;
				ptc.y = p_SLparam->pt.y - pt.y;
				if(p_SLparam->message!=WM_MOUSEMOVE)
					::PostMessage(hwnd,WM_ACTIVATE,WA_CLICKACTIVE,0);
				if(INRECT_PT(ptc,rc1)) //在ClientRect 内
				{
					if(p_SLparam->message !=WM_MOUSEMOVE)
						PostMessage(hwnd,WM_MOUSEMOVE,0,MAKELPARAM(ptc.x,ptc.y));
					if(p_SLparam->message ==WM_LBUTTONUP)
					{
						p_SLparam->message =WM_LBUTTONDOWN;
						PostMessage(hwnd,p_SLparam->message,p_SLparam->wParam,MAKELPARAM(ptc.x,ptc.y));
						p_SLparam->message =WM_LBUTTONUP;
						PostMessage(hwnd,p_SLparam->message,p_SLparam->wParam,MAKELPARAM(ptc.x,ptc.y));
					}
					else
					{
						res= PostMessage(hwnd,p_SLparam->message,p_SLparam->wParam,MAKELPARAM(ptc.x,ptc.y));
					}
					
				}
				else //不在ClientRect内
				{
					res = SendMessage(hwnd,WM_NCHITTEST,0,MAKELPARAM(p_SLparam->pt.x,p_SLparam->pt.y));				
					if((p_SLparam->message == WM_LBUTTONDOWN)&&(res == HTMAXBUTTON))
					{
						SendMessage(hwnd,WM_SYSCOMMAND,SC_MAXIMIZE,MAKELPARAM(p_SLparam->pt.x,p_SLparam->pt.y));
						p_SLparam->flag = TRUE;
						return TRUE;
					}
					if((p_SLparam->message == WM_LBUTTONDOWN)&&(res == HTMINBUTTON))
					{
						SendMessage(hwnd,WM_SYSCOMMAND,SC_MINIMIZE,MAKELPARAM(p_SLparam->pt.x,p_SLparam->pt.y));
						p_SLparam->flag = TRUE;
						return TRUE;
					}
					if((p_SLparam->message == WM_LBUTTONDOWN)&&(res == HTCLOSE))
					{
						SendMessage(hwnd,WM_SYSCOMMAND,SC_CLOSE,MAKELPARAM(p_SLparam->pt.x,p_SLparam->pt.y));
						p_SLparam->flag = TRUE;
						return TRUE;
					}
					PostMessage(hwnd,p_SLparam->message+WM_NCMOUSEMOVE-WM_MOUSEMOVE,res,MAKELPARAM(p_SLparam->pt.x,p_SLparam->pt.y));
				}
				
			}
			p_SLparam->flag = TRUE;
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CDlgBaseExplorer
//	
///////////////////////////////////////////////////////////////////////////

AUI_BEGIN_COMMAND_MAP(CDlgBaseExplorer, CDlgBase)
AUI_ON_COMMAND("Btn_Prev", OnCommand_Prev)
AUI_ON_COMMAND("Btn_Next", OnCommand_Next)
AUI_ON_COMMAND("Btn_Refresh", OnCommand_Refresh)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBaseExplorer, CDlgBase)

AUI_END_EVENT_MAP()

CDlgBaseExplorer::CDlgBaseExplorer()
{
	m_bInTimer       = false;
	m_bHasOuterScroll = false;
	m_pWPic = NULL;
	m_pExplorer		 = NULL;
	m_iExplorerIdx   = -1;
	m_bNoDocScroll	 = true;
	m_iRefreshTime	 = 0;
}

CDlgBaseExplorer::~CDlgBaseExplorer()
{
	//MSDN documentation says "A thread cannot use DestroyWindow to destroy a window created by a different thread.
	OnHideDialog();	
}

bool CDlgBaseExplorer::MessageTranslate(AString Control, UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool bval = false;
	if (0==strcmp(Control, "WPic"))
	{
		PAUIMANAGER pAUIManager = GetAUIManager();
		switch( pAUIManager->GetWindowMessageType(msg) )
		{
		case WINDOWMESSAGE_TYPE_MOUSE:
			{
				//POINT ptPos = m_pWPic->GetPos();
				//int x = GET_X_LPARAM(lParam) - m_pWPic->GetOffX() - ptPos.x;
				//int y = GET_Y_LPARAM(lParam) - m_pWPic->GetOffY() - ptPos.y;
				//lParam = int(SAFE_DIVIDE(y,m_fScale)) << 16 | int(SAFE_DIVIDE(x,m_fScale));
				//MouseEvent(m_pWPic->GetHWnd(), msg, wParam, lParam);
				//keybd_event( 0, 0, 0, 0 );
				return true;
			}
			
		case WINDOWMESSAGE_TYPE_KEYBOARD:
			{
				//KeyEvent(m_pWPic->GetHWnd(), msg, wParam, lParam);
				//keybd_event( 0, 0, 0, 0 );
				return true;
			}
		}
		
	}

	return true;
}

bool CDlgBaseExplorer::PeekMessage_Filter(LPMSG lpMsg,HWND hWnd)
{
	return true;
}


bool CDlgBaseExplorer::OnInitDialog()
{
	m_pWPic = (PAUIWINDOWPICTURE)GetDlgItem("WPic");
	if (!m_pWPic)
	{
		return false;
	}
	m_pWPic->SetAcceptMouseMessage(false);
	//m_pWPic->SetBkTransparent(true);
	//m_pWPic->SetDisplayRect(false, A3DRECT(-100, -100, 0, 0));
	m_pWPic->SetIsReadOnly(true);
	m_pWPic->SetIsEnableSelection(false);
	if (m_szName=="win_explorer" /*|| m_szName=="Win_Login_explorer"*/)		// 登陆界面跟游戏里面共用 Modified 2012-05-21 -->多平台登陆窗口使用单独的Browser Index
	{
		m_iExplorerIdx = CExplorerMan::EXPLORER_DEFAULT;
	}
	else if (m_szName=="win_explorer_xf")
	{
		m_iExplorerIdx = CExplorerMan::EXPLORER_XIANFU;
	}
	else if (m_szName=="Win_Radio")
	{
		m_iExplorerIdx = CExplorerMan::EXPLORER_MUSIC_RADIO;
	}
	else if (m_szName=="Win_ZhuxianKnows")
	{
		m_iExplorerIdx = CExplorerMan::EXPLORER_KNOWS;
	}
	else if ( m_szName=="Win_Login_explorer" )			//Added 2012-05-21,多平台登陆界面
	{
		m_iExplorerIdx = CExplorerMan::EXPLORER_LOGIN_WND;
	}
	else if ( m_szName=="win_explorer_ChargeAward" )			//Added 2012-09-11，热点内容
	{
		m_iExplorerIdx = CExplorerMan::EXPLORER_WEBHOT;
	}
	else if(m_szName=="Win_Guild_Bbs")
	{
		m_iExplorerIdx = CExplorerMan::EXPLORER_GUILD_BBS;
	}
	else
		assert(0);

	return true;
}

void CDlgBaseExplorer::OnShowDialog()
{
	CDlgBase::OnShowDialog();

	CreateBrowser();
}

void CDlgBaseExplorer::OnHideDialog()
{
	CDlgBase::OnHideDialog();

	if (m_pExplorer)
	{
		m_pExplorer->HideBrowser();
		m_pExplorer->ReleaseUI();
	}
}

void CDlgBaseExplorer::CreateBrowser()
{
	CExplorerMan::GetInstance().CreateExplorer(m_iExplorerIdx, this);

	CExplorer* pExplorer = CExplorerMan::GetInstance().GetExplorer(m_iExplorerIdx);
	if (pExplorer)
	{
		m_pExplorer = pExplorer;
	}
}

bool CDlgBaseExplorer::FreeExplorer()
{
	m_pExplorer = NULL;

	return true;
}

void CDlgBaseExplorer::NavigateUrl(const WCHAR* Url)
{
	if (m_pExplorer)
	{
		m_pExplorer->NavigateUrl(Url);
	}
}

void CDlgBaseExplorer::NavigateUrl(const CHAR* Url)
{
	if (m_pExplorer)
	{
		m_pExplorer->NavigateUrl(Url);
	}
}

void CDlgBaseExplorer::GoForward()
{
	if (m_pExplorer)
	{
		m_pExplorer->GoForward();
	}	
}

void CDlgBaseExplorer::GoBackward()
{
	if (m_pExplorer)
	{
		m_pExplorer->GoBackward();
	}	
}

void CDlgBaseExplorer::Refresh()
{
	if (m_iRefreshTime<3000)	// 3s刷新冷却周期
	{
		return;
	}
	m_iRefreshTime = 0;
	if (m_pExplorer)
	{
		m_pExplorer->Refresh();
	}	
}

void CDlgBaseExplorer::GetCurrentUrl()
{
	if (m_pExplorer && m_pExplorer->GetBrowser())
	{
		m_pExplorer->GetBrowser()->GetCurrentUrl();
	}
}

void CDlgBaseExplorer::OnWinTimer()
{
	if(m_bInTimer)
		return;

	m_bInTimer = false;
}

void CDlgBaseExplorer::OnTick()
{	
	m_iRefreshTime += GetGame()->GetTickTime();

	if (m_pExplorer)
	{
		m_pExplorer->GetBrowser()->OnTick();
	}
}

void CDlgBaseExplorer::MouseEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG xPos,yPos;
	RECT rc;
	xPos = (LONG)GET_X_LPARAM(lParam);
	yPos = (LONG)GET_Y_LPARAM(lParam);
	if (IsWindowVisible(hwnd))
	{
		GetWindowRect(hwnd, &rc);
		xPos += rc.left;
		yPos += rc.top;
		
		if (INRECT_XY(xPos,yPos,rc))
		{
			MouseEventX(hwnd, uMsg, wParam, MAKELPARAM(xPos,yPos));   //传入的是屏幕坐标
			return;
		}
	}
}

void CDlgBaseExplorer::MouseEventX(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG xPos,yPos,sxpos,sypos,nxpos,nypos;
	LRESULT res;
	nxpos= sxpos =xPos = (LONG)GET_X_LPARAM(lParam); //
	nypos= sypos =yPos = (LONG)GET_Y_LPARAM(lParam); //	
	
	SimuMouseEvent SLparam;
	RECT w_crc;
	POINT w_pt,w_ptc;
	w_pt.x = 0;
	w_pt.y = 0;
	w_ptc.x = 0;
	w_ptc.y = 0;
	
	SLparam.message = uMsg;
	SLparam.wParam = wParam;
	SLparam.flag = FALSE;
	
	SLparam.pt.x = sxpos;
	SLparam.pt.y = sypos;
	if((!IsIconic(hwnd)))
		EnumChildWindows(hwnd,_ThreadEnumChildMouseProcExp,(LPARAM)&SLparam);
	res =0;
	if(!SLparam.flag)
	{	
		::ClientToScreen(hwnd,&w_pt);
		::GetClientRect (hwnd,&w_crc);
		w_ptc.x = sxpos - w_pt.x;
		w_ptc.y = sypos - w_pt.y;
		if(INRECT_PT(w_ptc,w_crc)) //在ClientRect 内
		{
			::PostMessage(hwnd, uMsg,wParam,MAKELPARAM(w_ptc.x,w_ptc.y));
		}
		else //不在ClientRect 内
		{
			SetActiveWindow(hwnd);
			res = SendMessage(hwnd,WM_NCHITTEST,0,MAKELPARAM(sxpos,sypos));
			
			if((uMsg == WM_LBUTTONDOWN)&&(res == HTMAXBUTTON))
			{
				if(!IsZoomed(hwnd))
					SendMessage(hwnd,WM_SYSCOMMAND,SC_MAXIMIZE,MAKELPARAM(sxpos,sypos));
				else
					SendMessage(hwnd,WM_SYSCOMMAND,SC_RESTORE,MAKELPARAM(sxpos,sypos));
				return;
			}
			if((uMsg == WM_LBUTTONDOWN)&&(res == HTMINBUTTON))
			{
				if(!IsIconic(hwnd))
					SendMessage(hwnd,WM_SYSCOMMAND,SC_MINIMIZE,MAKELPARAM(sxpos,sypos));
				else
				{
					SendMessage(hwnd,WM_SYSCOMMAND,SC_RESTORE,MAKELPARAM(sxpos,sypos));
				}
				return ;
			}
			
			if((uMsg == WM_LBUTTONDOWN)&&(res == HTCLOSE))
			{
				
				PostMessage(hwnd,WM_SYSCOMMAND,SC_CLOSE,MAKELPARAM(sxpos,sypos));
				return ;
			}
			if((res==HTMENU)&&(uMsg == WM_LBUTTONDOWN))
			{
				MENUBARINFO info;
				GetMenuBarInfo(hwnd,OBJID_MENU,0,&info);
				if(!info.fBarFocused)
				{	
					PostMessage(hwnd,WM_SYSKEYDOWN,VK_MENU,0X20380001);
					PostMessage(hwnd,WM_SYSKEYUP  ,VK_MENU,0XC0380001);	
				}
			}
			PostMessage(hwnd,uMsg+WM_NCMOUSEMOVE-WM_MOUSEMOVE,res,MAKELPARAM(sxpos,sypos));							
			if((uMsg == WM_MOUSEMOVE))
			{
				PostMessage(hwnd,WM_SETCURSOR,(WPARAM)hwnd,res);
				return ;
			}
		}
	}		
}

HWND CDlgBaseExplorer::GetCaretWindowPos(LPRECT lprect)
{
	return NULL;
}

bool CDlgBaseExplorer::SendHTTPByIE(LPCSTR   URL,LPCSTR   headers,const   BYTE   *Post,   DWORD   postLength)
{	
    HRESULT   hr;   
    USES_CONVERSION;
    
	
    IWebBrowserApp*   pWBApp   =   NULL;   //   Derived   from   IWebBrowser   
    BSTR   bstrURL   =   NULL,   bstrHeaders   =   NULL;   
    VARIANT   vFlags   =   {0},   
		vTargetFrameName   =   {0},   
		vPostData   =   {0},   
		vHeaders   =   {0};   
    __try
	{     
		if   (FAILED(hr   =   CoInitialize(NULL)))   
		{   
			return   FALSE;   
		}                 
		if   (FAILED(hr   =   CoCreateInstance(CLSID_InternetExplorer,   
			NULL,   
			CLSCTX_SERVER,   
			IID_IWebBrowserApp,   
			(LPVOID*)&pWBApp)))   
		{   
			goto   Error;   
		}     
		bstrURL   =   SysAllocString(A2W(URL));   
		if (!bstrURL)   
		{   
			goto   Error;   
		}       
		if(headers!=NULL){   
			bstrHeaders   =   SysAllocString(A2W(headers));   
			if   (!bstrHeaders)   
			{   
				goto   Error;   
			}           
			V_VT(&vHeaders)   =   VT_BSTR;   
			V_BSTR(&vHeaders)   =   bstrHeaders;               
		}   
		if(Post!=NULL){   
			LPSAFEARRAY   psa; 
			LPSTR   pPostData;  
			LPVARIANT   pvPostData = &vPostData;
			VariantInit(&vPostData); 
			psa   =   SafeArrayCreateVector(VT_UI1,   0,   postLength);   
			if (psa)
			{
				hr   =   SafeArrayAccessData(psa,   (LPVOID*)&pPostData);   
				memcpy(pPostData,   (LPCTSTR)Post,   postLength);   
				hr   =   SafeArrayUnaccessData(psa);   
				V_VT(pvPostData)   =   VT_ARRAY   |   VT_UI1;     
				V_ARRAY(pvPostData)   =   psa;   
				return   NOERROR;   
			}
		}   
		hr   =   pWBApp->Navigate(bstrURL,   &vFlags,   
			&vTargetFrameName,   &vPostData,   &vHeaders);   
		//pWBApp->put_Visible(VARIANT_TRUE);     
Error:;   
    }   
	
    __finally{   
		
		if   (bstrURL)   SysFreeString(bstrURL);   
		if   (bstrHeaders)   SysFreeString(bstrHeaders);   
		if(Post!=NULL)VariantClear(&vPostData);   
		if   (pWBApp){   
			pWBApp->Release();   
			pWBApp=NULL;   
		}   
		CoUninitialize();   
    }   
	return   TRUE;   
}

DWORD CDlgBaseExplorer::OpenURL(void *url)
{
	HINTERNET   hInternet = ::InternetOpenA("Microsoft Internet Explorer",INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,0);  
	HINTERNET	hUrlFile = ::InternetOpenUrlA(hInternet, (char*)url, 0, 0, 0, 0);

	char buffer[1024];
	DWORD dwBytesRead = 0;
	int iRead = ::InternetReadFile(hUrlFile, buffer, sizeof(buffer), &dwBytesRead);
	if(dwBytesRead>=1024)
		dwBytesRead = 1023;
	buffer[dwBytesRead] = 0;
	//a_LogOutput(1, "ReadData:%s", buffer);
	::InternetCloseHandle(hUrlFile);
	::InternetCloseHandle(hInternet);
	return 0;
}
BOOL CDlgBaseExplorer::AddDownloadUrl(WCHAR *FileUrl)
{
	return TRUE;
}
BOOL CDlgBaseExplorer::InitTaskList()
{
	return TRUE;
}
bool CDlgBaseExplorer::GetDownloadState(abase::vector<AWString>& vecTaskDone, abase::vector<AWString>& vecTaskToDo, AWString& curDownFile, int &curDownProgress)
{
	if (NULL != m_pExplorer)
	{
		return m_pExplorer->GetDownloadState(vecTaskDone, vecTaskToDo, curDownFile, curDownProgress);			
	}
	return false;
}
double CDlgBaseExplorer::GetDownloadRate()
{
	if (NULL != m_pExplorer)
	{
		return m_pExplorer->GetDownloadRate();
	}
	return 0.0;
}
bool CDlgBaseExplorer::IsDownloading()
{
	if (NULL != m_pExplorer)
	{
		return m_pExplorer->IsDownloading();
	}
	return false;
}
void CDlgBaseExplorer::CancelCurDownLoad()
{
	if (NULL != m_pExplorer)
	{
		m_pExplorer->CancelCurDownLoad();
	}
}
void CDlgBaseExplorer::OnCommand_Prev(const char *szCommand)
{
	GoBackward();
}

void CDlgBaseExplorer::OnCommand_Next(const char *szCommand)
{
	GoForward();
}

void CDlgBaseExplorer::OnCommand_Refresh(const char *szCommand)
{
	Refresh();
}


const int CDlgZhuxianKnows::ms_TimeCircle = 1800000;

bool CDlgZhuxianKnows::OnInitDialog()
{
	bool bRet = CDlgBaseExplorer::OnInitDialog();
	m_iTime = ms_TimeCircle;
	return bRet;
}

void CDlgZhuxianKnows::OnShowDialog()
{
	CDlgBaseExplorer::OnShowDialog();
	m_iTime = ms_TimeCircle+1;
}

void CDlgZhuxianKnows::OnTick()
{
	CDlgBaseExplorer::OnTick();
	if (m_iTime>ms_TimeCircle)
	{
		m_iTime = 0;
		GetGame()->GetGameSession()->get_kdctoken();
	}

	m_iTime += GetGame()->GetTickTime();
}

int CDlgZhuxianKnows::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="set_token")
	{
		GNET::GetKDCToken_Re* p = (GNET::GetKDCToken_Re*)wParam;
		if (p->retcode==0)
		{
			m_strToken =  AString((const char*)p->token.begin(), p->token.size() / sizeof (char));
			NavigateUrl(ACString(GetStringFromTable(12994))+AS2AC(m_strToken));
		}
		else
		{
			ACHAR workPath[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, workPath);
			NavigateUrl(ACString(workPath)+GetStringFromTable(12993));
		}
	}
	return 0;
}


/*
多平台接入
*/

AUI_BEGIN_COMMAND_MAP(CDlgLoginOtherPlatform, CDlgBase)

AUI_ON_COMMAND("Btn_Refresh",				OnCommand_Refresh)
AUI_ON_COMMAND("IDCANCEL",					OnCommand_IDCANCEL)

AUI_END_COMMAND_MAP()

void CDlgLoginOtherPlatform::OnCommand_Refresh(const char * szCommand)
{
	Refresh();
}

void CDlgLoginOtherPlatform::OnCommand_IDCANCEL(const char * szCommand)
{
	Show(false);
	
	m_pAUIManager->GetDialog("Win_LoginLogOut")->Show(true);
	m_pAUIManager->GetDialog("Win_Login_Link")->Show(true);
	m_pAUIManager->GetDialog("Win_Softkb")->Show(false);
	m_pAUIManager->GetDialog("Win_LoginWait")->Show(false);
	m_pAUIManager->GetDialog("Win_Login")->Show(true);
}

void CDlgLoginOtherPlatform::OnTick()
{
	CDlgBaseExplorer::OnTick();
	ACString strName, strPass;
	bool bRet = CExplorerInfoParser(m_pExplorer, strName, strPass);
	if (!bRet)
		return;
	if( strName.GetLength() > 0 && strPass.GetLength() > 0 )
	{
		CECGameSession *pSession = g_pGame->GetGameSession();
//		strName.MakeLower();
		pSession->SetUserName(AC2AS(strName));
		pSession->SetUserPassword(AC2AS(strPass));
		pSession->SetKickUserFlag(true);
		pSession->Open();
		
		//			GetDialog("Win_Login")->Show(false);
		g_pGame->GetGameRun()->ResetWallowInfo();
		g_pGame->GetGameRun()->SetLastLoginId(0);
		m_pAUIManager->GetDialog("Win_LoginWait")->Show(true);
		Show(false);
	}
}

void CDlgLoginOtherPlatform::OnShowDialog()
{
	CDlgBaseExplorer::OnShowDialog();

	//设置浏览器silent属性保证在创建浏览器之后就立即实现
	if ( m_pExplorer )
	{
		m_pExplorer->SetWebBrowserSilent( false );
	}
}

/*void CDlgLoginOtherPlatform::OnHideDialog()
{
	CDlgBase::OnHideDialog();
	
	if ( m_pExplorer )
	{
		m_pExplorer = NULL;
	}

	CExplorerMan::GetInstance().RemoveExplorer( m_iExplorerIdx );
}*/
