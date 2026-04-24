/********************************************************************
	created:	2009/03/04
	file base:	DlgSystem4
	file ext:	cpp
	author:		Fu Chonggang
	
	purpose:	
*********************************************************************/
#include "DlgSystem4.h"
#include "DlgAction.h"
#include "DlgProduce.h"
#include "DlgArrangeTeam.h"
#include "DlgGuildMan.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_Faction.h"
#include "DlgBroadCast.h"
#include "DlgTrade.h"
#include "DlgShop.h"
#include "DlgTask.h"
#include "DlgRankList.h"
#include "DlgPetPackage.h"
#include "DlgTeacherMan.h"
#include "DlgFriendList.h"
#include "DlgGameCasual.h"
#include "DlgBaseExplorer.h"
#include "EC_HostPlayer.h"
#include "EC_HTC2SRawCmds.h"
#include "LuaEvent.h"
#include "DlgGuildMain.h"
#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSystem4, CDlgBase)

AUI_ON_COMMAND("wteam",			OnCommandTeam)
AUI_ON_COMMAND("wfaction",		OnCommandFaction)
AUI_ON_COMMAND("wteacher",		OnCommandTeacher)
AUI_ON_COMMAND("wfriendseek",	OnCommandFriendSeek)
AUI_ON_COMMAND("wfriend",		OnCommandFriend)
AUI_ON_COMMAND("wgame",			OnCommandGameCasual)
AUI_ON_COMMAND("Btn_Garden",	OnCommandGarden)
AUI_ON_COMMAND("Btn_ZhuxianKnows",	OnCommandKnows)

AUI_END_COMMAND_MAP()


CDlgSystem4::CDlgSystem4()
{

}

CDlgSystem4::~CDlgSystem4()
{

}

void CDlgSystem4::OnCommandTeam(const char *szCommand)
{
	int battleType = GetHostPlayer()->GetBattleType();
	if ( battleType!=BATTLE_TYPE_ARENA && battleType!=BATTLE_TYPE_ARENAREBORN && battleType!=BATTLE_TYPE_CRSSVR )
	{
		CDlgArrangeTeam* pDlg = GetGameUIMan()->m_pDlgArrangeTeam;
		bool bval = GetGameUIMan()->UpdateTeam(true);
		LuaEvent::FirstTeamSetting();
		if( !bval )
			AUI_ReportError(__LINE__, __FILE__);
		if( pDlg )
			pDlg->Show(!pDlg->IsShow());
	}
}

void CDlgSystem4::OnCommandFaction(const char * szCommand)
{
	if (GetHostPlayer()->GetSrcServerID()!=0)
		return;
	CDlgGuildMain* pDlg = (CDlgGuildMain*)GetGameUIMan()->GetDialog("Win_GuildMain");
	pDlg->Show(!pDlg->IsShow());
//	CDlgGuildMan *pDlg = GetGameUIMan()->m_pDlgFamilyMan;

	//if( GetHostPlayer()->GetFamilyID() )
	//{
	//	pDlg->Show(!pDlg->IsShow());
	//	if( pDlg->IsShow() )
	//	{
	//		if( pDlg->IsFirstOpen() )
	//		{
	//			GetGame()->GetFactionMan()->RefreshMemList(0, GetHostPlayer()->GetFamilyID(), true);
	//			pDlg->SetFirstOpen(false);
	//		}
	//		else
	//			GetGame()->GetFactionMan()->RefreshMemList(0, GetHostPlayer()->GetFamilyID(), false);
	//	}
	//}
	//else
	//	GetGameUIMan()->MessageBox("NoFamily", GetStringFromTable(870), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
}

void CDlgSystem4::OnCommandTeacher(const char * szCommand)
{
	if (GetHostPlayer()->GetSrcServerID()!=0)
		return;
	CDlgTeacherMan* pDlg = GetGameUIMan()->m_pDlgTeacherMan;
	int iLevel = GetHostPlayer()->GetBasicProps().iLevel;
	if( pDlg && (iLevel >= 105 || GetHostPlayer()->GetMaster() != 0) )
		pDlg->Show(!pDlg->IsShow());
}

void CDlgSystem4::OnCommandFriendSeek(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_FriendSeeking");
	pDlg->Show(!pDlg->IsShow());
	LuaEvent::FirstOpenSNS();
}

void CDlgSystem4::OnCommandFriend(const char *szCommand)
{
	int battleType = GetHostPlayer()->GetBattleType();
	if ( battleType!=BATTLE_TYPE_ARENA && battleType!=BATTLE_TYPE_ARENAREBORN && battleType!=BATTLE_TYPE_CRSSVR )
	{
		CDlgFriendList* pDlg = GetGameUIMan()->m_pDlgFriendList;
		if( pDlg )
			pDlg->Show(!pDlg->IsShow());
	}
}

void CDlgSystem4::OnCommandGameCasual(const char *szCommand)
{
	CDlgGameCasual* pDlg = (CDlgGameCasual*)GetGameUIMan()->GetDialog("Win_GameCasual");
	if( pDlg )
		pDlg->Show(!pDlg->IsShow());
}

void CDlgSystem4::OnCommandGarden(const char *szCommand)
{
	htc2s_SendCmdGetCompoList(GetHostPlayer()->GetCharacterID());
	GetGameUIMan()->GetDialog("Win_Option")->Show(true);
}

void CDlgSystem4::OnCommandKnows(const char *szCommand)
{
	CDlgBaseExplorer* pDlgExplorer = (CDlgBaseExplorer*)GetGameUIMan()->GetDialog("Win_ZhuxianKnows");
	pDlgExplorer->Show(!pDlgExplorer->IsShow());
}

