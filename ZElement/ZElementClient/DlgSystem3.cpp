/********************************************************************
	created:	2005/08/11
	created:	11:8:2005   15:11
	file base:	DlgSystem3
	file ext:	cpp
	author:		yaojun
	
	purpose:	
*********************************************************************/

#include "DlgSystem3.h"
#include "DlgSetting.h"
#include "DlgAskHelpToGM.h"
#include "DlgHelp.h"
#include "DlgExit.h"
#include "DlgRecord.h"
#include "DlgHelpFariy.h"
#include "EC_GameUIMan.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"

#define new A_DEBUG_NEW


AUI_BEGIN_COMMAND_MAP(CDlgSystem3, CDlgBase)

AUI_ON_COMMAND("wgm", OnCommandGM)
AUI_ON_COMMAND("whelp", OnCommandHelp)
AUI_ON_COMMAND("wsetting", OnCommandSetting)
AUI_ON_COMMAND("wrecord", OnCommandRecord)
AUI_ON_COMMAND("repick", OnCommandRepick)
AUI_ON_COMMAND("quitgame", OnCommandQuit)

AUI_END_COMMAND_MAP()


CDlgSystem3::CDlgSystem3()
{

}

CDlgSystem3::~CDlgSystem3()
{

}

void CDlgSystem3::OnCommandGM(const char *szCommand)
{
	const char *szCountry = g_pGame->GetConfigs()->GetCountryRegion();
	if ( 0 == stricmp(szCountry, "kr") )
	{
		GetGameUIMan()->GetDialog("Win_Kr_Winmain3")->Show(true);
		return;
	}
	AUIDialog *pDlg = GetGameUIMan()->m_pDlgAskHelpToGM;
	if (pDlg->IsShow())
	{
		pDlg->Show(false);
	}
	else
	{
#ifndef _DEBUG
		{
			DWORD dwLastReportTime = pDlg->GetData();
			// GM can send msg without delta-time restriction
			if (GetTickCount() - dwLastReportTime < 300 * 1000 &&
				!g_pGame->GetGameRun()->GetHostPlayer()->IsGM())
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(612), GP_CHAT_MISC);
				return;
			}
		}
#endif
		pDlg->Show(true);
	}

}

void CDlgSystem3::OnCommandHelp(const char *szCommand)
{
	CDlgHelpFariy* pDlg = (CDlgHelpFariy*)GetGameUIMan()->GetDialog("Win_Help_Fairy");
	if (pDlg->IsShow())
	{
		pDlg->OnCommand_Close("");
	}
	else
		pDlg->Show(true);
}

void CDlgSystem3::OnCommandSetting(const char *szCommand)
{
	GetGameUIMan()->m_pDlgSettingCurrent->Show(
		!GetGameUIMan()->m_pDlgSettingCurrent->IsShow());
}

void CDlgSystem3::OnCommandRecord(const char *szCommand)
{
	if( GetGameUIMan()->m_pDlgRecord->IsShow() )
		GetGameUIMan()->m_pDlgRecord->OnCommandCancel("");
	else
		GetGameUIMan()->m_pDlgRecord->Show(true);
}

void CDlgSystem3::OnCommandRepick(const char *szCommand)
{
	PAUIDIALOG pDlgShow = NULL;
	if( GetGameUIMan()->m_pDlgExit->IsShow())
		pDlgShow = GetGameUIMan()->m_pDlgExit;
	else if( GetGameUIMan()->m_pDlgGameQuit->IsShow())
		pDlgShow = GetGameUIMan()->m_pDlgGameQuit;
	else if( GetGameUIMan()->GetDialog("Win_DailyPrompExit")->IsShow())
		pDlgShow = GetGameUIMan()->GetDialog("Win_DailyPrompExit");
	else if( GetGameUIMan()->GetDialog("Win_KfSelectLine")->IsShow())
		pDlgShow = GetGameUIMan()->GetDialog("Win_KfSelectLine");
	
	if (pDlgShow)
		pDlgShow->Show(false);
	if (GetHostPlayer()->GetSrcServerID()==0)
		GetGameUIMan()->m_pDlgGameQuit->Show(true);
	else
	{
		GetGameUIMan()->GetDialog("Win_KfSelectLine")->Show(true);
		GetGameSession()->get_line_info();
	}
}

void CDlgSystem3::OnCommandQuit(const char *szCommand)
{
	PAUIDIALOG pDlgShow = NULL;
	if( GetGameUIMan()->m_pDlgExit->IsShow())
		pDlgShow = GetGameUIMan()->m_pDlgExit;
	else if( GetGameUIMan()->m_pDlgGameQuit->IsShow())
		pDlgShow = GetGameUIMan()->m_pDlgGameQuit;
	else if( GetGameUIMan()->GetDialog("Win_DailyPrompExit")->IsShow())
		pDlgShow = GetGameUIMan()->GetDialog("Win_DailyPrompExit");
	else if( GetGameUIMan()->GetDialog("Win_KfSelectLine")->IsShow())
		pDlgShow = GetGameUIMan()->GetDialog("Win_KfSelectLine");
	
	if (pDlgShow)
		pDlgShow->Show(false);
	GetGameUIMan()->GetDialog("Win_DailyPrompExit")->Show(true);
//	GetGameUIMan()->m_pDlgExit->Show(true);

	const GNET::RoleInfo& info = g_pGame->GetGameRun()->GetSelectedRoleInfo();
	if (info.lastlogin_time == info.create_time) // µÚÒ»´ÎµÇÂ½
	{
		CECConfigs* pConfig = g_pGame->GetConfigs();
		if (!pConfig->GetLocalSettings().bDisableShowQuitReason)
		{
			EC_LOCAL_SETTING ls = pConfig->GetLocalSettings();
			ls.bDisableShowQuitReason = true;
			pConfig->SetLocalSettings(ls);
			pConfig->SaveLocalSetting();

			int tNow = g_pGame->GetServerGMTTime();
			int tCreateTime = g_pGame->GetGameRun()->GetSelectedRoleInfo().create_time;
			if (tCreateTime + 3600 > tNow)
				GetGameUIMan()->GetDialog("Win_QuitReason")->Show(true, true);
		}
	}
}
