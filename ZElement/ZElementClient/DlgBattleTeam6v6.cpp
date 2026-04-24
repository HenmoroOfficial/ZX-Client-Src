// Filename	: DlgBattleTeam6v6.cpp
// Creator	: Fu Chonggang
// Date		: 

#include "DlgBattleTeam6v6.h"
#include "DlgBattleDungeonApply.h"
#include "AFI.h"
#include "EC_GameUIMan.h"
#include "EC_SendC2SCmds.h"
#include "EC_BattleDungeon.h"
#include "EC_HostPlayer.h"
#include "EC_GameSession.h"
#include "EC_Team.h"
#include "EC_Game.h"

#define new A_DEBUG_NEW


/************************************************************************
CDlgBattleTeam6v6ApplyMin
************************************************************************/
AUI_BEGIN_COMMAND_MAP(CDlgBattleTeam6v6ApplyMin, CDlgBase)

AUI_ON_COMMAND("Btn_Max",		OnCommandOpen)
AUI_ON_COMMAND("Btn_quit",		OnCommandQuit)

AUI_END_COMMAND_MAP()

void CDlgBattleTeam6v6ApplyMin::OnCommandOpen(const char * szCommand)
{
	PAUIDIALOG	pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_MapSelect");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgBattleTeam6v6ApplyMin::OnCommandQuit(const char * szCommand)
{
	GetGameSession()->raid_quit_6v6(/*CECBattleDungeonMan::getSingletonPtr()->m_iRoomId*/);
}


AUI_BEGIN_COMMAND_MAP(CDlgBattleTeam6v6Apply, CDlgTab3D)

AUI_ON_COMMAND("Btn_Self",		OnCommandOpenSelf)
AUI_ON_COMMAND("Btn_Team",		OnCommandOpenTeam)
AUI_ON_COMMAND("Btn_Award",		OnCommandAward)
AUI_ON_COMMAND("Btn_quit",		OnCommandQuit)

AUI_END_COMMAND_MAP()


CDlgBattleTeam6v6Apply::CDlgBattleTeam6v6Apply()
{
	m_vecTmplIds.push_back(642);
}

CDlgBattleTeam6v6Apply::~CDlgBattleTeam6v6Apply()
{
}

bool CDlgBattleTeam6v6Apply::OnInitDialog()
{
	m_pObjSelf = GetDlgItem("Btn_Self");
	m_pObjTeam = GetDlgItem("Btn_Team");
	GetDlgItem("Txt_Wait")->Show(false);
	GetDlgItem("Btn_quit")->Show(false);
	return CDlgTab3D::OnInitDialog();
}

void CDlgBattleTeam6v6Apply::OnTick()
{	
	CECTeam *pTeam = GetHostPlayer()->GetTeam();
	m_pObjSelf->Enable(!pTeam);
	m_pObjTeam->Enable(pTeam && pTeam->GetLeader() && (pTeam->GetLeader()->GetCharacterID()==GetHostPlayer()->GetCharacterID()));
	GetDlgItem("Txt_Wait")->Show(GetDlgItem("Btn_quit")->IsShow());
	CDlgTab3D::OnTick();
}

void CDlgBattleTeam6v6Apply::OnCommandOpenSelf(const char * szCommand)
{
	if (HostInBattle())
	{
		CECBattleDungeonMan::getSingletonPtr()->ReportError(ERR_RAID_6V6_SIGNUP_IN_BATTLE);
		return;
	}
	if(GetGame()->GetGuildEnter() == true)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20051), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	c2s_SendCmd6v6CollisionApply(535,//CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iMapId,
		642, //CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iId,
		false);
}

void CDlgBattleTeam6v6Apply::OnCommandOpenTeam(const char * szCommand)
{
	if (HostInBattle())
	{
		CECBattleDungeonMan::getSingletonPtr()->ReportError(ERR_RAID_6V6_SIGNUP_IN_BATTLE);
		return;
	}
	if(GetGame()->GetGuildEnter() == true)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20051), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	CECTeam *pTeam = GetHostPlayer()->GetTeam();
	m_pObjSelf->Enable(!pTeam);
	m_pObjTeam->Enable(pTeam && (pTeam->GetLeader()->GetCharacterID()==GetHostPlayer()->GetCharacterID()));

	int iTest = -1;
	int i;
	for (i=0; i < pTeam->GetMemberNum(); i++)
	{
		CECTeamMember* pMember = pTeam->GetMemberByIndex(i);
		if (!pMember->IsInfoReady())
			break;
		
		if (iTest!=-1 && iTest!=pMember->GetRebornCnt())
			break;

		iTest = pMember->GetRebornCnt();
	}
	if (i==pTeam->GetMemberNum())
	{
		c2s_SendCmd6v6CollisionApply(535, //CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iMapId, 
			642, //CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iId, 
			true);
	}
	else
	{
		GetGameUIMan()->MessageBox(14001);
	}
}

void CDlgBattleTeam6v6Apply::OnCommandAward(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6V6_Award");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgBattleTeam6v6Apply::OnCommandQuit(const char * szCommand)
{
	GetGameSession()->raid_quit_6v6(/*CECBattleDungeonMan::getSingletonPtr()->m_iRoomId*/);
}

void CDlgBattleTeam6v6Apply::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
}

void CDlgBattleTeam6v6Apply::AddTextrures()
{
	unsigned int i;
	for (i=0; i<m_vecTmplIds.size(); ++i)
	{
		int index = CDlgBattleDungeonApply::GetIndexFromTmplId(m_vecTmplIds[i]);
		AddTextrure(AString("Surfaces\\") + AC2AS(GetGameUIMan()->GetStringFromTable(index+1)));
	}
}

void CDlgBattleTeam6v6Apply::OnSelOneItem()
{

}

bool CDlgBattleTeam6v6Apply::HostSignUpBattle()
{
	if ( CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle() )
		return true;
	
	if ( GetHostPlayer()->GetBattleInstanceState()==1 )
		return true;
	
	if ( GetHostPlayer()->GetApplyBattleType()>=0 )
		return true;
	
	return false;
}

bool CDlgBattleTeam6v6Apply::HostInBattle()
{
	if (GetHostPlayer()->GetBattleType()>=0)
		return true;
	
	if (GetHostPlayer()->GetTerritoryBattleInfo()>0)
		return true;
	
	if (GetHostPlayer()->GetRaidBattleInfo()>0)
		return true;
	
	return false;
}
