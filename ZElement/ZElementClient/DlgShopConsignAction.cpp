// Filename	:	DlgShopConsignAction.cpp
// Creator	:	Lei Dongyu
// Date		:	2010/10/29
// Description:	Ñ°±¦ÍøÓÎÏ·ÄÚÉÌÆ·ä¯ÀÀÒ³ÃæÓÒ¼ü²Ëµ¥

#include "A3DDevice.h"

#include "EC_GameUIMan.h"
#include "EC_GameSession.h"

#include "DlgChat.h"
#include "DlgShopConsignAction.h"
#include "AUI\\AUIEditBox.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgShopConsignAction, CDlgBase)

AUI_ON_COMMAND("Btn_Cancel",		OnCommand_Cancel)
AUI_ON_COMMAND("Btn_OpenItemWebPage",	OnCommand_OpenWebSite)
AUI_ON_COMMAND("Btn_CopyItemWebPage",	OnCommand_CopyWebSite)
AUI_ON_COMMAND("Btn_MsgSeller",		OnCommand_MsgSeller)

AUI_END_COMMAND_MAP()

CDlgShopConsignAction::CDlgShopConsignAction()
{
	CDlgBase::CDlgBase();
}

CDlgShopConsignAction::~CDlgShopConsignAction()
{
}

void CDlgShopConsignAction::OnCommand_OpenWebSite(const char* szCommand)
{
	ACString url = buildURL(itemId);
	if(url)
		ShellExecuteA(NULL, "open", AC2AS(url), NULL, NULL, SW_SHOWNORMAL);
	Show(false);
}

void CDlgShopConsignAction::OnCommand_CopyWebSite(const char* szCommand)
{
	if( OpenClipboard(m_pA3DDevice->GetDeviceWnd()) )
	{
		ACString url = buildURL(itemId);
		HLOCAL hLocal = GlobalAlloc(LMEM_ZEROINIT, a_strlen(url) * 2 + 2);
		if( hLocal )
		{
			ACHAR *pUrl = (ACHAR *)GlobalLock(hLocal);
			a_strcpy(pUrl, url);
			EmptyClipboard();
#ifdef UNICODE
			SetClipboardData(CF_UNICODETEXT, pUrl);
#else
			SetClipboardData(CF_TEXT, pUrl);
#endif
			GlobalUnlock(hLocal);
		}

		CloseClipboard();
	}
	Show(false);
}

void CDlgShopConsignAction::OnCommand_MsgSeller(const char* szCommand)
{
	GetGameUIMan()->SwitchToPrivateChat(vendor);
	Show(false);
}

void CDlgShopConsignAction::OnCommand_Cancel(const char* szCommand)
{
	Show(false);
}

ACString CDlgShopConsignAction::buildURL(int64_t sn)
{
	ACString result;
	if(commodityid <= 0)
	{
		int zoneID = GetGameSession()->GetZoneID();
			//g_pGame->GetGameRun()->GetInstance(GetWorld()->GetInstanceID())->GetZoneID();
		if(zoneID) 
		{
			result.Format(GetStringFromTable(10282) , zoneID);
		}
		else
		{
			result.Format(GetStringFromTable(10280));
		}
		
	}
	else
	{
		result.Format(GetStringFromTable(10281), commodityid);
	}
	return result;
}

void CDlgShopConsignAction::SetData(int64_t s , ACString v , int id)
{
	itemId = s;
	vendor = v;
	commodityid = id;
}
