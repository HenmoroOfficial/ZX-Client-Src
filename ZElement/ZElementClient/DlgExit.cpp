// Filename	: DlgExit.cpp
// Creator	: Tom Zhou
// Date		: October 27, 2005

#include "AUI\\AUILabel.h"
#include "DlgExit.h"
#include "EC_MsgDef.h"
#include "EC_GameUIMan.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_Game.h"
#include "EC_HostPlayer.h"
#include "EC_Global.h"
#include "Network\\gnetdef.h"
#include "EC_Secretary.h"
#include "gt_overlay.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgExit, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommand_confirm)

AUI_END_COMMAND_MAP()

CDlgExit::CDlgExit()
{
}

CDlgExit::~CDlgExit()
{
}

bool CDlgExit::OnInitDialog()
{
	m_pChk_Forcequit = (PAUICHECKBOX)GetDlgItem("Chk_Forcequit");
	if ( 0 == stricmp(GetName(), "Win_Message2") )
	{
		m_pChk_ExitOffExp = (PAUICHECKBOX)GetDlgItem("Chk_OffExp");
	}
	else if ( 0 == stricmp(GetName(), "Win_Message7") )
	{
		m_pChk_GameQuitOffExp = (PAUICHECKBOX)GetDlgItem("Chk_OffExp");
	}
	else if ( 0 == stricmp(GetName(), "Win_Message6") )
	{
		GetDlgItem("Txt_TimeUse")->SetAlign(AUIFRAME_ALIGN_RIGHT); 
		GetDlgItem("Txt_TimeLeft")->SetAlign(AUIFRAME_ALIGN_RIGHT);
		GetDlgItem("Txt_Exp")->SetAlign(AUIFRAME_ALIGN_RIGHT);
	}

	return true;
}

void CDlgExit::OnShowDialog()
{
	if ( 0 == stricmp(GetName(), "Win_Message2") )
	{
		m_pChk_ExitOffExp->Check(false);
		if (GetHostPlayer()->GetAgentTime()<=0)
		{
			m_pChk_ExitOffExp->Enable(false);
		}
		else
		{
			m_pChk_ExitOffExp->Enable(true);
		}
	}
	else if ( 0 == stricmp(GetName(), "Win_Message7") )
	{
		m_pChk_GameQuitOffExp->Check(false);
		if (GetHostPlayer()->GetAgentTime()<=0)
		{
			m_pChk_GameQuitOffExp->Enable(false);
		}
		else
		{
			m_pChk_GameQuitOffExp->Enable(true);
		}
	}
}

void CDlgExit::OnCommand_confirm(const char * szCommand)
{
	if ( 0 == stricmp(GetName(), "Win_Message2") )
	{
		if( m_pChk_Forcequit->IsChecked() )
			GetGameRun()->PostMessage(MSG_EXITGAME, -1, 0);
		else if( GetGameSession()->IsConnected() )
		{
			GetGameRun()->SaveConfigsToServer();
			if( m_pChk_ExitOffExp->IsChecked() )
			{
				GetGameSession()->c2s_CmdLogout(_PLAYER_LOGOUT_FULL, 1);
			}
			else
			{
				GetGameSession()->c2s_CmdLogout(_PLAYER_LOGOUT_FULL);
			}
		}
		else
			GetGameRun()->SetLogoutFlag(0);
		
		overlay::GTOverlay::Instance().Logout();
		Show(false);
	}
	else if ( 0 == stricmp(GetName(), "Win_Message7") )
	{
		// 设置fov为正常水平，否则登陆界面不正常
		GetHostPlayer()->GetCameraCtrl()->SceneryMode_Set(false);
		if( GetGameSession()->IsConnected() )
		{
			g_pGame->GetGameRun()->SaveConfigsToServer();
			if( m_pChk_GameQuitOffExp->IsChecked() )
			{
				GetGameSession()->c2s_CmdLogout(_PLAYER_LOGOUT_HALF, 1);
			}
			else
			{
				GetGameSession()->c2s_CmdLogout(_PLAYER_LOGOUT_HALF);
			}
		}
		else
			g_pGame->GetGameRun()->SetLogoutFlag(2);
		g_pGame->SetGuildEnter(false);
		overlay::GTOverlay::Instance().Logout();
		Show(false);

	}
	else if ( 0 == stricmp(GetName(), "Win_Message6") )
	{
		Show(false);
	}
	else if ( 0 == strcmp(GetName(), "Win_InteractionOption") )
	{
		g_pGame->GetGameSession()->c2s_CmdCancelInteract(g_pGame->GetGameRun()->GetHostPlayer()->GetInteractorInfo().id);
	}

}

void CDlgExit::ShowAgentInfo(int bonus_time, int left_time, double bonus_exp)
{
	PAUILABEL pLabBonusTime = (PAUILABEL)GetDlgItem("Txt_TimeUse"); 
	PAUILABEL pLabLeftTime = (PAUILABEL)GetDlgItem("Txt_TimeLeft");
	PAUILABEL pLabBonusExp = (PAUILABEL)GetDlgItem("Txt_Exp");

	ACHAR szText[20];
	a_sprintf(szText, _AL("%d"), bonus_time);
	pLabBonusTime->SetText(szText);
	a_sprintf(szText, _AL("%d"), left_time);
	pLabLeftTime->SetText(szText);
	a_sprintf(szText, _AL("%6.0f"), bonus_exp);
	pLabBonusExp->SetText(szText);
	Show(true);
}