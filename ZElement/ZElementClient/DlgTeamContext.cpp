// Filename	: DlgTeamContext.cpp
// Creator	: Tom Zhou
// Date		: September 1, 2005

#include "AUI\\AUIComboBox.h"
#include "A3DDevice.h"
#include "DlgTeamContext.h"
#include "DlgArrangeTeam.h"
#include "DlgChat.h"
#include "EC_Friend.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Team.h"

#ifdef BACK_VERSION
#include "EC_Manager.h"
#else
#include "ECManager.h"
#include "ECManagerID.h"
#endif

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgTeamContext, CDlgBase)

AUI_ON_COMMAND("OpenT",				OnCommand_OpenT)
AUI_ON_COMMAND("GroupLeave",		OnCommand_GroupLeave)
AUI_ON_COMMAND("GroupDisband",		OnCommand_GroupLeave)
AUI_ON_COMMAND("Whisper",			OnCommand_Whisper)
AUI_ON_COMMAND("MakeLeader",		OnCommand_MakeLeader)
AUI_ON_COMMAND("Kick",				OnCommand_Kick)
AUI_ON_COMMAND("Trade",				OnCommand_Trade)
AUI_ON_COMMAND("Friend",			OnCommand_Friend)
AUI_ON_COMMAND("Follow",			OnCommand_Follow)
AUI_ON_COMMAND("Assist",			OnCommand_Assist)
AUI_ON_COMMAND("Btn_CopyName",		OnCommand_CopyName)

AUI_END_COMMAND_MAP()

CDlgTeamContext::CDlgTeamContext()
{
}

CDlgTeamContext::~CDlgTeamContext()
{
}

void CDlgTeamContext::OnCommand_OpenT(const char * szCommand)
{
	GetGameUIMan()->UpdateTeam(true);
	GetGameUIMan()->m_pDlgArrangeTeam->Show(true);
	Show(false);
}

void CDlgTeamContext::OnCommand_GroupLeave(const char * szCommand)
{
	CECTeam *pTeam = GetHostPlayer()->GetTeam();
	if( !pTeam ) return;

	if( pTeam->GetLeaderID() == GetHostPlayer()->GetCharacterID() )
	{
		GetGameUIMan()->MessageBox("Game_TeamDisband", GetStringFromTable(235),
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
		GetGameSession()->c2s_CmdTeamLeaveParty();
	Show(false);
}

void CDlgTeamContext::OnCommand_Whisper(const char * szCommand)
{
	int idTarget = GetData();
	const ACHAR *pszName = GetGameRun()->GetPlayerName(idTarget, true);
	GetGameUIMan()->SwitchToPrivateChat(pszName);
	Show(false);
}

void CDlgTeamContext::OnCommand_MakeLeader(const char * szCommand)
{
	GetGameSession()->c2s_CmdTeamChangeLeader(GetData());
	Show(false);
}

void CDlgTeamContext::OnCommand_Kick(const char * szCommand)
{
	GetGameSession()->c2s_CmdTeamKickMember(GetData());
	Show(false);
}

void CDlgTeamContext::OnCommand_Trade(const char * szCommand)
{
	int battleType = GetHostPlayer()->GetBattleType();
	if(battleType != BATTLE_TYPE_ARENA && battleType != BATTLE_TYPE_ARENAREBORN && battleType != BATTLE_TYPE_CRSSVR)
	{
		GetGameSession()->trade_Start(GetData());
		GetGameUIMan()->AddChatMessage(GetStringFromTable(212), GP_CHAT_MISC);
	}
	Show(false);
}

void CDlgTeamContext::OnCommand_Friend(const char * szCommand)
{
	int idTarget = GetData();
	const ACHAR *pszName = GetGameRun()->GetPlayerName(idTarget, true);
	CECFriendMan::FRIEND* pFriend = GetHostPlayer()->GetFriendMan()->GetFriendByID(idTarget);

	//	In friend list, one player ID may have two names, so check name here !
	if( pFriend && !a_strcmp(pszName, pFriend->GetName()) )
		GetGameUIMan()->AddChatMessage(GetStringFromTable(565), GP_CHAT_MISC);
	else
	{
		GetGameSession()->friend_Add(idTarget, pszName);
		GetGameUIMan()->AddChatMessage(GetStringFromTable(533), GP_CHAT_MISC);
	}
	Show(false);
}

void CDlgTeamContext::OnCommand_Follow(const char * szCommand)
{
	GetGameRun()->PostMessage(MSG_HST_FOLLOW, MAN_PLAYER, 0, GetData());
	Show(false);
}

void CDlgTeamContext::OnCommand_Assist(const char * szCommand)
{
	GetGameSession()->c2s_CmdTeamAssistSel(GetData());
	Show(false);
}

void CDlgTeamContext::OnCommand_CopyName(const char * szCommand)
{
	if( OpenClipboard(m_pA3DDevice->GetDeviceWnd()) )
	{
		int idTarget = GetData();
		const ACHAR *szName = GetGameRun()->GetPlayerName(idTarget, true);
		
		if (szName[1]==_AL(' '))	// GT图标后面有一个空格
			szName += 2;
		
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

void CDlgTeamContext::OnTick()
{
	CECGameUIMan * pGameUI = GetGameUIMan();
	if (pGameUI->GetActiveDialog()!=this && !pGameUI->GetDialog("Win_GamePoker_Hall")->IsShow() && !pGameUI->GetDialog("Win_GamePoker_Match")->IsShow())
		Show(false);
}
