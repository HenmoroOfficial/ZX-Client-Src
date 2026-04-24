/********************************************************************
	created:	2005/08/15
	created:	15:8:2005   18:00
	file name:	DlgRevive.cpp
	author:		yaojun
	
	purpose:	
*********************************************************************/

#include "AUI\\AUIObject.h"
#include "DlgRevive.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgRevive, AUIDialog)

AUI_ON_COMMAND("confirm", OnCommandConfirm)
AUI_ON_COMMAND("back", OnCommandBack)
AUI_ON_COMMAND("accept", OnCommandAccept)
AUI_ON_COMMAND("IDCANCEL", OnCommandCancel)

AUI_END_COMMAND_MAP()

CDlgRevive::CDlgRevive()
{
}

CDlgRevive::~CDlgRevive()
{
}

bool CDlgRevive::OnInitDialog()
{
	m_pTxt_D = (PAUILABEL)GetDlgItem("Txt_D");
	//m_pBtn_Back = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Back");
	m_pBtn_Decide = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Decide");
	m_pBtn_Accept = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Accept");
	
	m_dwReviveTime = 0;

	return true;
}

void CDlgRevive::OnCommandConfirm(const char *szCommand)
{
	GetGameSession()->c2s_CmdReviveVillage();
}

void CDlgRevive::OnCommandBack(const char *szCommand)
{
	//	Revive with soul stone, check cool time at first
	CECHostPlayer* pHost = GetHostPlayer();
	//if (pHost && !pHost->GetCoolTime(GP_CT_SOUL_STONE))
	//	GetGameSession()->c2s_CmdReviveItem();
}

void CDlgRevive::OnCommandAccept(const char *szCommand)
{
	GetGameSession()->c2s_CmdRevivalAgree();
}

void CDlgRevive::OnCommandCancel(const char *szCommand)
{
}

void CDlgRevive::OnTick()
{
	if( !GetHostPlayer()->IsDead() )
		Show(false);
	m_pBtn_Accept->Enable(GetHostPlayer()->GetReviveLostExp() >= 0.0f ? true : false);

	int iBattleType = GetHostPlayer()->GetBattleType();
	if( GetHostPlayer()->GetKingdomWarType() != 0 || ( GetHostPlayer()->IsInBattle() && 
		( iBattleType==BATTLE_TYPE_SONJIN || iBattleType==BATTLE_TYPE_SONJINREBORN ||
		iBattleType==BATTLE_TYPE_NORMAL || iBattleType==BATTLE_TYPE_REBORN)) )
	{
		ACHAR szText[200];
		int iTime;
		if (GetTickCount()>m_dwReviveTime)
		{
			m_pBtn_Decide->Enable(true);
			iTime = 0;
		}
		else
		{
			m_pBtn_Decide->Enable(false);
			iTime = (m_dwReviveTime-GetTickCount())/1000;
		}
		a_sprintf(szText, GetStringFromTable(7700), iTime);
		m_pTxt_D->SetText(szText);
	}
}

void CDlgRevive::BattleOut()
{
	if( GetHostPlayer()->GetBasicProps().iLevel < 10 )
		m_pTxt_D->SetText(GetStringFromTable(274));
	else
		m_pTxt_D->SetText(GetStringFromTable(a_Random(283, 292)));
	m_pBtn_Decide->Enable(true);
}