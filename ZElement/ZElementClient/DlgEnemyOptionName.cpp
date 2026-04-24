// File		: DlgEnemyOptionName.cpp
// Creator	: Fu Chonggang
// Date		: 2009/6/17

#include "AUI\\AUICommon.h"
#include "AUI\\AUIEditBox.h"
#include "DlgChat.h"
#include "DlgEnemyOptionName.h"
#include "DlgFriendList.h"
#include "DlgFriendHistory.h"
#include "DlgFriendGroup.h"
//#include "DlgChannelChat.h"
#include "DlgFriendChat.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_Friend.h"
#include "EC_HostPlayer.h"
#include "Network\\gnetdef.h"
#include "A3DDevice.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgEnemyOptionName, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCANCAL)
AUI_ON_COMMAND("Whisper",		OnCommandWhisper)
AUI_ON_COMMAND("Update",		OnCommandUpdate)
AUI_ON_COMMAND("DeleteEnemy",	OnCommandDeleteEnemy)
AUI_ON_COMMAND("CopyName",		OnCommandCopyName)

AUI_END_COMMAND_MAP()

CDlgEnemyOptionName::CDlgEnemyOptionName()
{
}

CDlgEnemyOptionName::~CDlgEnemyOptionName()
{
}

void CDlgEnemyOptionName::OnCommandCANCAL(const char *szCommand)
{
	Show(false);
}

void CDlgEnemyOptionName::OnCommandWhisper(const char *szCommand)
{
	CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
	PAUILISTBOX pLst = pDlgFriend->m_pLstEnemyList;
	int index = pLst->GetCurSel();
	if(index<0 || index>=pLst->GetCount())	return;
	ACString strName = pLst->GetText(index);
	int index1= strName.Find(_AL("\t"));
	strName = strName.Left(index1);
	GetGameUIMan()->SwitchToPrivateChat(strName);
	Show(false);
}

void CDlgEnemyOptionName::OnCommandDeleteEnemy(const char *szCommand)
{
	GetGameUIMan()->m_pDlgFriendList->OnCommandRemove("");
	Show(false);
}

void CDlgEnemyOptionName::OnCommandUpdate(const char *szCommand)
{
	GetGameSession()->enemies_get(GetHostPlayer()->GetCharacterID(), 0);
	Show(false);
}

void CDlgEnemyOptionName::OnCommandCopyName(const char * szCommand)
{
	CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
	PAUILISTBOX pLst = pDlgFriend->m_pLstEnemyList;
	int index = pLst->GetCurSel();
	if(index<0)	return;
	ACString strName = pLst->GetText(index);
	int index1=  strName.Find(_AL("\t"));
	strName = strName.Left(index1);
	const ACHAR * szName = strName;

	if( OpenClipboard(m_pA3DDevice->GetDeviceWnd()) )
	{
		HLOCAL hLocal = GlobalAlloc(LMEM_ZEROINIT, a_strlen(szName) * 2 + 2);
		if( hLocal )
		{
			ACHAR *pszName = (ACHAR *)GlobalLock(hLocal);
			a_strcpy(pszName, szName);
			EmptyClipboard();
#ifdef UNICODE
			SetClipboardData(CF_UNICODETEXT, pszName);
#else
			SetClipboardData(CF_TEXT, pszName);
#endif
			GlobalUnlock(hLocal);
		}

		CloseClipboard();
	}
	Show(false);
}

void CDlgEnemyOptionName::OnTick()
{
	CECGameUIMan * pGameUI = GetGameUIMan();
	if (pGameUI->GetActiveDialog()!=this && !pGameUI->GetDialog("Win_GamePoker_Hall")->IsShow() && !pGameUI->GetDialog("Win_GamePoker_Match")->IsShow())
		Show(false);
}