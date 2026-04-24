// File		: DlgSystem2.h
// Creator	: Xiao Zhou
// Date		: 2005/8/15

#include "AWIniFile.h"
#include "DlgSystem2.h"
#include "DlgAction.h"
#include "DlgProduce.h"
#include "DlgArrangeTeam.h"
#include "DlgGuildMan.h"
#include "DlgAchivMain.h"
#include "DlgFrdCallBck.h"
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
#include "DlgBroadCastInGame.h"
#include "EC_HostPlayer.h"
#include "EC_NewHelpGuide.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSystem2, CDlgBase)

AUI_ON_COMMAND("wachiv",		OnCommandAchievement)
AUI_ON_COMMAND("wpet",			OnCommandPet)
AUI_ON_COMMAND("wranklist",		OnCommandRankList)
AUI_ON_COMMAND("waction",		OnCommandAction)
AUI_ON_COMMAND("wproduce",		OnCommandProduce)
AUI_ON_COMMAND("wbroadcast",	OnCommandBroadcast)
AUI_ON_COMMAND("Btn_MailtoFriend",OnCommandMailToFriend)

AUI_END_COMMAND_MAP()

CDlgSystem2::CDlgSystem2()
{
}

CDlgSystem2::~CDlgSystem2()
{
}

void CDlgSystem2::OnCommandAchievement(const char *szCommand)
{
	CDlgAchivMain* pDlgAchievement = (CDlgAchivMain*)GetGameUIMan()->GetDialog("Win_Achievement");
 	pDlgAchievement->SetTarget(0);
 	pDlgAchievement->Show(!pDlgAchievement->IsShow());
}

void CDlgSystem2::OnCommandPet(const char *szCommand)
{
	CDlgPetPackage* pDlg = GetGameUIMan()->m_pDlgPetPackage;
	if( pDlg )
	{
		if(!pDlg->IsShow())
		{
			NewHelpGuide::StartGuide_Step_9(GUIDE_HELP_PET_11);
		}
		pDlg->Show(!pDlg->IsShow());
	}
}

void CDlgSystem2::OnCommandRankList(const char *szCommand)
{
	CDlgRankList* pDlg = GetGameUIMan()->m_pDlgRankList;
	if( pDlg )
	{
		if( pDlg->IsShow() )
			pDlg->OnCommand("IDCANCEL");
		else
			pDlg->Show(true);
	}
}

void CDlgSystem2::OnCommandAction(const char *szCommand)
{
	if( GetGameUIMan()->m_pDlgAction )
		GetGameUIMan()->m_pDlgAction->Show(!GetGameUIMan()->m_pDlgAction->IsShow());
}

void CDlgSystem2::OnCommandProduce(const char *szCommand)
{
	CDlgProduce* pDlg = GetGameUIMan()->m_pDlgProduce;
	if( pDlg && GetHostPlayer()->GetProduceSkillLevel() > 0 )
	{
		if( pDlg->IsShow() )
			pDlg->OnCommand_CANCEL("");
		else if ( GetHostPlayer()->CanDoProduce() && !GetHostPlayer()->IsPlayerMoving() )
		{	pDlg->SetCanAutoSell(false);
			pDlg->ShowAutoSell(false);
			pDlg->Show(true);
		}
	}
}

void CDlgSystem2::OnCommandBroadcast(const char * szCommand)
{
	ACString strText;
	AWIniFile iniFile;
	if(iniFile.Open("configs\\broadcast.ini"))
	{
		strText = iniFile.GetValueAsString(_AL("Channel"), _AL("ChannelURL0"), _AL(""));
		iniFile.Close();
		if( strText.GetLength() > 0 )
		{
			CDlgBroadCastInGame* pDlgRadioInGame = (CDlgBroadCastInGame*)GetGameUIMan()->GetDialog("Win_Radio");
			pDlgRadioInGame->Show(true);
			pDlgRadioInGame->NavigateUrl(strText);
		}
	}
}

void CDlgSystem2::OnCommandMailToFriend(const char * szCommand)
{
	CDlgFrdCallBck* pDlg = GetGameUIMan()->m_pDlgFrdCallBck;
	if (pDlg) {
		pDlg->Show(!pDlg->IsShow());
	}
}