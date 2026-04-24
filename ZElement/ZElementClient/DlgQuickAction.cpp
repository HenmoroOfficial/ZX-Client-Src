// Filename	: DlgQuickAction.cpp
// Creator	: Tom Zhou
// Date		: October 10, 2005

#include "AFI.h"
#include "DlgQuickAction.h"
#include "DlgReportToGM.h"
#include "DlgChat.h"
#include "DlgFlower.h"
//#include "DlgChannelChat.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_Friend.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_HostPlayer.h"
#include "EC_Shortcut.h"
#include "EC_ShortcutSet.h"
#include "EC_ElsePlayer.h"
#include "EC_Team.h"
#include "Network\\ids.hxx"
#include "AUI\\AUICommon.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIComboBox.h"
#include "DlgAskHelpToGM.h"
#include "DlgAchivMain.h"
#include "A3DDevice.h"
#include "itemdataman.h"
#include "EC_IvtrConsume.h"
#include "EC_Inventory.h"
#include "EC_Skill.h"
#include "EC_Game.h"
#include "LuaEvent.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgQuickAction, CDlgBase)

AUI_ON_COMMAND("SecretMsg",		OnCommand_SecretMsg)
AUI_ON_COMMAND("Report",		OnCommand_Report)
AUI_ON_COMMAND("Trade",			OnCommand_Trade)
AUI_ON_COMMAND("WhisperChat",	OnCommand_Whisper)
AUI_ON_COMMAND("AddFriend",		OnCommand_AddFriend)
AUI_ON_COMMAND("InviteGroup",	OnCommand_InviteGroup)
AUI_ON_COMMAND("InviteGuild",	OnCommand_InviteGuild)
AUI_ON_COMMAND("InviteFamily",	OnCommand_InviteFamily)
AUI_ON_COMMAND("Follow",		OnCommand_Follow)
AUI_ON_COMMAND("JoinChannel",	OnCommand_JoinChannel)
AUI_ON_COMMAND("InviteChannel",	OnCommand_InviteChannel)
AUI_ON_COMMAND("Bind",			OnCommand_Bind)
AUI_ON_COMMAND("Btn_InviteRide",OnCommand_QilinInvite)
AUI_ON_COMMAND("Duel",			OnCommand_Duel)
AUI_ON_COMMAND("EPEquip",		OnCommand_EPEquip)
AUI_ON_COMMAND("CopyName",		OnCommand_CopyName)
AUI_ON_COMMAND("Teacher",		OnCommand_Teacher)
AUI_ON_COMMAND("Shield",		OnCommand_Shield)
AUI_ON_COMMAND("RideTogether",	OnCommand_RideTogether)
AUI_ON_COMMAND("achievement",	OnCommand_Achievement)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Rose",		OnCommand_Rose)

AUI_END_COMMAND_MAP()

CDlgQuickAction::CDlgQuickAction()
{
}

CDlgQuickAction::~CDlgQuickAction()
{
}

bool CDlgQuickAction::OnInitDialog()
{
	m_pInviteGuild = (PAUISTILLIMAGEBUTTON)GetDlgItem("InviteGuild");
	m_pInviteFamily = (PAUISTILLIMAGEBUTTON)GetDlgItem("InviteFamily");
//	m_pInviteChannel = (PAUISTILLIMAGEBUTTON)GetDlgItem("InviteChannel");
	m_pInviteGroup = (PAUISTILLIMAGEBUTTON)GetDlgItem("InviteGroup");
	m_pDuel = (PAUISTILLIMAGEBUTTON)GetDlgItem("Duel");
	m_pBind = (PAUISTILLIMAGEBUTTON)GetDlgItem("Bind");
	m_pQilinBind = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_InviteRide");
	m_pTrade = (PAUISTILLIMAGEBUTTON)GetDlgItem("Trade");
	m_pTeacher = (PAUISTILLIMAGEBUTTON)GetDlgItem("Teacher");
	m_pRideTogether = (PAUISTILLIMAGEBUTTON)GetDlgItem("RideTogether");
	m_pFollow = (PAUISTILLIMAGEBUTTON)GetDlgItem("Follow");
	m_pRose= (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Rose");
	return true;
}

void CDlgQuickAction::OnShowDialog()
{
	m_pInviteGuild->Enable(false);
	m_pInviteFamily->Enable(false);
//	m_pInviteChannel->Enable(false);
	m_pDuel->Enable(false);
	m_pBind->Enable(false);
	m_pQilinBind->Enable(false);
	m_pTrade->Enable(false);
	m_pRideTogether->Enable(false);
	LuaEvent::FirstAddFriend();
	tm tmNow = GetGame()->GetServerLocalTime();
	m_pRose->Enable(false);
	if (checkFlowerTime(tmNow))
	{
		if(GetHostPlayer()->GetSrcServerID() == 0)
		{
			m_pRose->Enable(true);
		}
	}
	
}

void CDlgQuickAction::OnCommand_SecretMsg(const char * szCommand)
{
	int idTarget = GetData();
	const ACHAR *pszName = GetGameRun()->GetPlayerName(idTarget, true);
	GetGameUIMan()->SwitchToPrivateChat(pszName);
	Show(false);
}

void CDlgQuickAction::OnCommand_Report(const char * szCommand)
{
	CDlgAskHelpToGM *pDlg = GetGameUIMan()->m_pDlgAskHelpToGM;

	DWORD dwLastReportTime = pDlg->GetData("tick_time");
	pDlg->SetDataPtr((void*)GetData(), "rold_id");

#ifndef _DEBUG
	if( (GetTickCount() - dwLastReportTime) / 1000 <= 5 * 60 )
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(532), GP_CHAT_MISC);
		Show(false);
		return;
	}
#endif

	const ACHAR *pszName = GetGameRun()->GetPlayerName(GetData(), true);
	ACHAR szText[40];
	AUI_ConvertChatString(pszName, szText);
	
	ACString msg;
	msg.Format(GetStringFromTable(6007), szText);
	pDlg->ShowWithHelpMessage(msg, true);

	Show(false);
}

void CDlgQuickAction::OnCommand_Trade(const char * szCommand)
{
	Show(false);
	int battleType = GetHostPlayer()->GetBattleType();
	if(battleType != BATTLE_TYPE_ARENA && battleType != BATTLE_TYPE_ARENAREBORN && battleType!=BATTLE_TYPE_CRSSVR)
	{
		GetGameSession()->trade_Start(GetData());
		GetGameUIMan()->AddChatMessage(GetStringFromTable(212), GP_CHAT_MISC);
		LuaEvent::FirstTrade();
	}
}

void CDlgQuickAction::OnCommand_AddFriend(const char * szCommand)
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

void CDlgQuickAction::OnCommand_InviteGroup(const char * szCommand)
{
	int idTarget = GetData();
	CECElsePlayer *pPlayer = GetWorld()->GetPlayerMan()->GetElsePlayer(idTarget);

	const ACHAR *pszName = GetGameRun()->GetPlayerName(idTarget, true);
	if( GetHostPlayer()->GetTeam() && pPlayer && pPlayer->GetInTeamState() != 0 )
		GetGameUIMan()->AddChatMessage(GetStringFromTable(687), GP_CHAT_MISC);
	else if (GetHostPlayer()->GetSrcServerID()!=0 && pPlayer && pPlayer->GetSrcServerID()!=GetHostPlayer()->GetSrcServerID()
		&& GetHostPlayer()->GetBattleType() == -1)// 2012-9-14 add by zy 添加战场判断
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(3301), GP_CHAT_MISC);
	}
	else if( GetHostPlayer()->GetTeam() == NULL && pPlayer && pPlayer->GetInTeamState() != 0 )
	{
		GetGameSession()->c2s_CmdTeamAskJoin(idTarget);
		GetGameUIMan()->AddChatMessage(GetStringFromTable(685), GP_CHAT_MISC);
	}
	else
	{
		GetGameSession()->c2s_CmdTeamInvite(idTarget);
		GetGameUIMan()->AddChatMessage(GetStringFromTable(203), GP_CHAT_MISC);
	}

	Show(false);
}

void CDlgQuickAction::OnCommand_InviteGuild(const char * szCommand)
{
	int idTarget = GetData();
	

	GetGameSession()->faction_accept_join(idTarget, 0);
	GetGameUIMan()->AddChatMessage(GetStringFromTable(601), GP_CHAT_MISC);

	Show(false);
}

void CDlgQuickAction::OnCommand_InviteFamily(const char * szCommand)
{
	int idTarget = GetData();
	const ACHAR *pszName = GetGameRun()->GetPlayerName(idTarget, true);

	GetGameSession()->faction_accept_join(idTarget, 1);
	GetGameUIMan()->AddChatMessage(GetStringFromTable(858), GP_CHAT_MISC);

	Show(false);
}

void CDlgQuickAction::OnCommand_Follow(const char * szCommand)
{
	int idTarget = GetData();
	const ACHAR *pszName = GetGameRun()->GetPlayerName(idTarget, true);

	GetGameRun()->PostMessage(MSG_HST_FOLLOW, MAN_PLAYER, 0, idTarget);

	Show(false);
}

void CDlgQuickAction::OnCommand_JoinChannel(const char * szCommand)
{
// 	int idTarget = GetData();
// 	const ACHAR *pszName = GetGameRun()->GetPlayerName(idTarget, true);
// 	Show(false);
// 	if( GetGameUIMan()->m_pDlgChannelChat->GetRoomID() != 0 &&
// 		GetGameUIMan()->m_pDlgChannelChat->IsCreator() )
// 	{
// 		PAUIDIALOG pMsgBox;
// 		m_pAUIManager->MessageBox("Game_ChannelJoinAsCreator", GetStringFromTable(654), MB_YESNO,
// 			A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
// 		pMsgBox->SetText(pszName);
// 	}
// 	else
// 		GetGameSession()->chatroom_Join(pszName, _AL(""));

}

void CDlgQuickAction::OnCommand_InviteChannel(const char * szCommand)
{
// 	int idTarget = GetData();
// 	int idRoom = GetGameUIMan()->m_pDlgChannelChat->GetRoomID();
// 	if( idRoom != 0 )
// 	{
// 		GetGameSession()->chatroom_Invite(idRoom, idTarget);
// 		GetGameUIMan()->AddChatMessage(GetStringFromTable(633), GP_CHAT_MISC);
// 	}
// 
	Show(false);
}

void CDlgQuickAction::OnCommand_Bind(const char * szCommand)
{
	GetHostPlayer()->CmdBindBuddy(GetData());
	LuaEvent::FirstHug();
	Show(false);
}

void CDlgQuickAction::OnCommand_QilinInvite(const char * szCommand)
{
	GetHostPlayer()->CmdBindQilinBuddy(GetData());
	Show(false);
}

void CDlgQuickAction::OnCommand_RideTogether(const char * szCommand)
{
	GetHostPlayer()->CmdBindMultiBuddy(GetData());
	Show(false);
}

void CDlgQuickAction::OnCommand_Achievement(const char * szCommand)
{
 	GetGameSession()->c2s_CmdQueryOthersAchievement(GetData());
 	//CDlgAchivMain* pDlgAchievement = (CDlgAchivMain*)GetGameUIMan()->GetDialog("Win_Achievement");
 	//pDlgAchievement->SetTarget(GetData());
	GetGameUIMan()->GetDialog("Win_Achievement_Other")->SetData(GetData());
	Show(false);
}

void CDlgQuickAction::OnCommand_Duel(const char * szCommand)
{
	int idTarget = GetData();
	CECElsePlayer *pPlayer = GetWorld()->GetPlayerMan()->GetElsePlayer(idTarget);
	if( pPlayer )
	{
		if( GetHostPlayer()->GetBasicProps().iLevel < 10 )
			GetGameUIMan()->AddChatMessage(GetStringFromTable(871), GP_CHAT_MISC);
		else if( pPlayer->GetBasicProps().iLevel < 10 )
			GetGameUIMan()->AddChatMessage(GetStringFromTable(872), GP_CHAT_MISC);
		else
			GetHostPlayer()->CmdAskDuel(idTarget);
		Show(false);
	}
}

void CDlgQuickAction::OnCommand_EPEquip(const char * szCommand)
{
	GetHostPlayer()->CmdViewOtherEquips(GetData());
	Show(false);
}


void CDlgQuickAction::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgQuickAction::OnCommand_CopyName(const char * szCommand)
{
	if( OpenClipboard(m_pA3DDevice->GetDeviceWnd()) )
	{
		int idTarget = GetData();
		const ACHAR * szName = GetGameRun()->GetPlayerName(idTarget, true);	
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

void CDlgQuickAction::OnCommand_Teacher(const char * szCommand)
{
	Show(false);
	int idTarget = GetData();
	GetGameSession()->sect_recruit(idTarget);
	GetGameUIMan()->AddChatMessage(GetStringFromTable(999), GP_CHAT_MISC);
}

void CDlgQuickAction::OnCommand_Shield(const char * szCommand)
{
	Show(false);
	DWORD idTarget = GetData();
	PAUIDIALOG pMsgBox;
	ACString szText;
	const ACHAR *pszName = GetGameRun()->GetPlayerName(idTarget, true);
	szText.Format(m_pAUIManager->GetStringFromTable(499),pszName);
	
	m_pAUIManager->MessageBox("Game_ShieldPlayer", szText,
		MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
	pMsgBox->SetData(idTarget);
}

void CDlgQuickAction::OnCommand_Whisper(const char * szCommand)
{
	Show(false);

	PAUIDIALOG pDlgWhisper = GetGameUIMan()->m_pDlgChatWhisper;
	if( pDlgWhisper->IsShow() && pDlgWhisper->GetData() == GetData() ) return;

	pDlgWhisper->Show(true);
	pDlgWhisper->SetData(GetData());
	pDlgWhisper->GetDlgItem("DEFAULT_Txt_Speech")->SetText(_AL(""));

	PAUITEXTAREA pText = (PAUITEXTAREA)pDlgWhisper->GetDlgItem("Txt_Chat");
	pText->SetText(_AL(""));

	ACHAR szUser[40];
	ACString strText;
	PAUIOBJECT pName = pDlgWhisper->GetDlgItem("Txt_ToWho");
	const ACHAR *pszName = GetGameRun()->GetPlayerName(GetData(), true);
	AUI_ConvertChatString(pszName, szUser);
	strText.Format(GetStringFromTable(548), szUser);
	pName->SetText(strText);
}

void CDlgQuickAction::OnCommand_Rose(const char * szCommand)
{
	Show(false);
	int idTarget = GetData();
	const ACHAR *pszName = GetGameRun()->GetPlayerName(idTarget, true);
	DlgFlowers*	pDlg = (DlgFlowers*)GetGameUIMan()->GetDialog("Win_Rose");
	if(pDlg)
	{
		pDlg->Show(true);
		pDlg->UpdateUI(idTarget,pszName);
	}
}

void CDlgQuickAction::OnTick()
{
	int nSrcServerId = GetHostPlayer()->GetSrcServerID();	
	int idTarget = GetData();
	CECElsePlayer *pPlayer = GetWorld()->GetPlayerMan()->GetElsePlayer(idTarget);

	int idFRole = GetHostPlayer()->GetFRoleID();
	int idFaction = GetHostPlayer()->GetFactionID();
	int idFamily = GetHostPlayer()->GetFamilyID();
	if( pPlayer &&
		(pPlayer->GetFamilyID() > 0 || idFamily == 0 || idFRole == GNET::_R_MEMBER) )
		m_pInviteFamily->Enable(false);
	else
	{
		if( idFamily != 0 && idFRole <= GNET::_R_HEADER )
			m_pInviteFamily->Enable(true);
		else
			m_pInviteFamily->Enable(false);
	}
	
	if( pPlayer &&
		(pPlayer->GetFactionID() > 0 || pPlayer->GetFamilyID() == 0 || 
		idFaction == 0 || idFRole >= GNET::_R_HEADER ) )
		m_pInviteGuild->Enable(false);
	else
	{
		if( idFaction != 0 && idFRole < GNET::_R_HEADER )
			m_pInviteGuild->Enable(true);
		else
			m_pInviteGuild->Enable(false);
	}

//	if( GetGameUIMan()->m_pDlgChannelChat->GetRoomID() != 0 )
//		m_pInviteChannel->Enable(true);
//	else
//		m_pInviteChannel->Enable(false);

	if( pPlayer && 
		pPlayer->GetGender() != GetHostPlayer()->GetGender() && 
		GetHostPlayer()->GetBuddyState() == 0 && 
		!(pPlayer->IsMultiRideMember() || pPlayer->IsMultiRideLeader() ) &&
		!(GetHostPlayer()->IsMultiRideMember() || GetHostPlayer()->IsMultiRideLeader()) && 0==GetHostPlayer()->GetCarrierID() &&
		!(pPlayer->GetProfession() >= 108 && pPlayer->GetProfession() <= 112 && pPlayer->GetShapeChangedId() == 0)  &&
		!(GetHostPlayer()->GetProfession() >= 108 && GetHostPlayer()->GetProfession() <= 112 && GetHostPlayer()->GetShapeChangedId() == 0) &&
		!((GetHostPlayer()->GetProfession() >= 102 && GetHostPlayer()->GetProfession() <= 106) && (pPlayer->GetProfession() < 102 || pPlayer->GetProfession() > 106)) &&
		!((GetHostPlayer()->GetProfession() < 102 || GetHostPlayer()->GetProfession() > 106) && (pPlayer->GetProfession() >= 102 && pPlayer->GetProfession() <= 106))
		)
		m_pBind->Enable(true);
	else
		m_pBind->Enable(false);

	if(pPlayer && GetHostPlayer()->GetQilinConnectBuddy() == 0 && pPlayer->GetQilinConnectBuddy() == 0 &&
		!(pPlayer->IsMultiRideMember() || pPlayer->IsMultiRideLeader()) &&  !(pPlayer->GetProfession() >= 108 && pPlayer->GetProfession() <= 112) &&
		!(GetHostPlayer()->IsMultiRideMember() || GetHostPlayer()->IsMultiRideLeader()) && GetHostPlayer()->IsTeamMember(idTarget) &&
		0 == GetHostPlayer()->GetCarrierID() && GetHostPlayer()->GetShapeChangedId() == SHAPE_QILIN && pPlayer->GetShapeChangedId() == 0 &&
		GetHostPlayer()->GetBuddyState() == 0 && pPlayer->GetBuddyState() == 0)
		m_pQilinBind->Enable(true);
	else
		m_pQilinBind->Enable(false);
		

	if( pPlayer && GetHostPlayer()->IsMultiRideLeader() && !(GetHostPlayer()->IsMultiRideFull()) && 0==GetHostPlayer()->GetCarrierID() &&
		!(pPlayer->GetProfession() >= 108 && pPlayer->GetProfession() <= 112 && pPlayer->GetShapeChangedId() == 0) &&
		!(GetHostPlayer()->GetProfession() >= 108 && GetHostPlayer()->GetProfession() <= 112 && GetHostPlayer()->GetShapeChangedId() == 0))
		m_pRideTogether->Enable(true);
	else
		m_pRideTogether->Enable(false);
	
	if( GetHostPlayer()->GetTeam() == NULL && pPlayer && pPlayer->GetInTeamState() != 0 )
		m_pInviteGroup->SetText(GetStringFromTable(794));
	else
		m_pInviteGroup->SetText(GetStringFromTable(793));
	
	if( GetHostPlayer()->GetTeam() && 
		(GetHostPlayer()->GetTeam()->GetLeaderID() != GetHostPlayer()->GetCharacterID() ||
		pPlayer && pPlayer->GetInTeamState() != 0) )
	{
		m_pInviteGroup->Enable(false);
		m_pInviteGroup->SetHint(_AL(""));
	}
	else if ( nSrcServerId!=0 && pPlayer && nSrcServerId!=pPlayer->GetSrcServerID()
		     && GetHostPlayer()->GetBattleType() == -1)// 2012-9-14 add by zy 添加战场判断
	{
		m_pInviteGroup->Enable(false);
		m_pInviteGroup->SetHint(GetGameUIMan()->GetStringFromTable(5310));
	}
	else
	{
		m_pInviteGroup->Enable(true);
		m_pInviteGroup->SetHint(_AL(""));
	}

	m_pFollow->Enable(0==GetHostPlayer()->GetCarrierID());
	m_pDuel->Enable(pPlayer && !GetHostPlayer()->IsInDuel() && !pPlayer->IsInDuel() && 0==GetHostPlayer()->GetCarrierID());
	m_pTrade->Enable(pPlayer && !GetHostPlayer()->IsTrading() && 0==GetHostPlayer()->GetCarrierID() && nSrcServerId==pPlayer->GetSrcServerID());//跨服同一服务器或都不在跨服
	m_pTrade->SetHint((!pPlayer || nSrcServerId==pPlayer->GetSrcServerID())?_AL("") : GetStringFromTable(5311));
	m_pTeacher->Enable(GetHostPlayer()->GetBasicProps().iLevel >= 105 && (!pPlayer || pPlayer->GetBasicProps().iLevel <= 75 && pPlayer->GetMaster() == 0));
}


