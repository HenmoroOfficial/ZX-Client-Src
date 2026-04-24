// Filename	: DlgBuddyState.cpp
// Creator	: Xiao Zhou
// Date		: 2005/12/13

#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"

#include "DlgBuddyState.h"
#include "DlgMinimizeBar.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_ManPlayer.h"
#include "EC_World.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_HPWork.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBuddyState, CDlgBase)

AUI_ON_COMMAND("kiss",			OnCommand_Btn1)
AUI_ON_COMMAND("bindcancel",	OnCommand_Btn2)
AUI_ON_COMMAND("kick*",			OnCommand_Kick)
AUI_ON_COMMAND("minimize",		OnCommandMinimize)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_Cancel)

AUI_END_COMMAND_MAP()

CDlgBuddyState::CDlgBuddyState()
{
	m_pImg_Prof = NULL;
	m_pTxt_Name = NULL;
	m_pBtn_Kiss	= NULL;
	m_pBtn_Cancel = NULL;
	m_iQAState	= -1;
	m_bMinimized = false;
}

CDlgBuddyState::~CDlgBuddyState()
{
}

void CDlgBuddyState::OnCommand_Cancel(const char * szCommand)
{
	GetGameUIMan()->RespawnMessage();	
}

void CDlgBuddyState::OnCommand_Btn1(const char * szCommand)
{
	if (QA_BUDDYSTATE == m_iQAState)
		GetGameSession()->c2s_CmdEmoteAction(29);
	else if (QA_RIDETOGETHER == m_iQAState)
	{
		c2s_SendCmdCancelMultiBindPlayer();
	}
}

void CDlgBuddyState::OnCommand_Btn2(const char * szCommand)
{
	if (QA_BUDDYSTATE == m_iQAState)
	{
		GetGameSession()->c2s_CmdCancelBindPlayer();
	}
	else if (QA_RIDETOGETHER == m_iQAState)
	{
		c2s_SendCmdCancelMultiBindPlayer();
	}
}

void CDlgBuddyState::OnCommand_Kick(const char * szCommand)
{
	int index = atoi(szCommand + strlen("kick"));

	char szName[20];
	sprintf(szName, "Btn_KickOut%d", index);
	int idPlayer = GetDlgItem(szName)->GetData();
	GetHostPlayer()->CmdMultiBindKick(idPlayer);

}

void CDlgBuddyState::OnCommandMinimize(const char *szCommand)
{
	m_bMinimized = true;
	GetGameUIMan()->m_pDlgMinimizeBar[0]->MinimizeDialog(this, 
		GetStringFromTable(7105), CDlgMinimizeBar::MINIMIZEBAR_TYPE_RIDEKICK);
}

bool CDlgBuddyState::OnInitDialog()
{
	if (0 == stricmp(GetName(),"Win_RideTogether"))
	{
		DDX_Control("Img_Prof", m_pImg_Prof);
	}
	if (0 == stricmp(GetName(),"Win_BuddyState"))
	{
		DDX_Control("Img_Prof", m_pImg_Prof);
		DDX_Control("Txt_Name", m_pTxt_Name);
		DDX_Control("Btn_1", m_pBtn_Kiss);
		DDX_Control("Btn_2", m_pBtn_Cancel);
	}

	return true;
}

void CDlgBuddyState::OnTick()
{
	if( GetHostPlayer()->GetBuddyState() != 0 )
	{
		m_iQAState = QA_BUDDYSTATE;
		bool bMoving = false;
		if( GetHostPlayer()->IsHangerOn() )
		{
			CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetHostPlayer()->GetBuddyID());
			if( pPlayer )
				bMoving = pPlayer->IsPlayerMoving();
		}
		else
			bMoving = GetHostPlayer()->IsPlayerMoving();
		
		if( bMoving || GetHostPlayer()->GetSpouse() != GetHostPlayer()->GetBuddyID() || GetHostPlayer()->GetCoolTime(GP_CT_EMOTE, NULL) > 0 )
			m_pBtn_Kiss->Enable(false);
		else
			m_pBtn_Kiss->Enable(true);
	}
	else if( GetHostPlayer()->IsMultiRideMember() )
		m_iQAState = QA_RIDETOGETHER;
	else if( GetHostPlayer()->IsMultiRideLeader()&&GetHostPlayer()->IsMultiRideHasMember() )
	{
		m_iQAState = QA_RIDETOGETHER;
		const int* RideID = GetHostPlayer()->GetMultiRideMemberId();
		for (int i=1; i<6; i++)
		{
			char szName[20];
			sprintf(szName, "Txt_Name%d", i);
			PAUILABEL pName = (PAUILABEL)GetDlgItem(szName);
			sprintf(szName, "Btn_KickOut%d", i);
			PAUISTILLIMAGEBUTTON pKick = (PAUISTILLIMAGEBUTTON)GetDlgItem(szName);
			if ( 0 == RideID[i] )
			{
				pName->Show(false);
				pKick->Show(false);
			}
			else
			{
				const ACHAR *pszName = GetGameRun()->GetPlayerName(RideID[i], false);
				pName->SetText(pszName);
				pName->Show(true);
				pKick->Show(true);
				pKick->SetData(RideID[i]);
			}
		}
	}
	else
	{
		if (m_bMinimized)
		{
			m_bMinimized = false;
			GetGameUIMan()->m_pDlgMinimizeBar[0]->RestoreDialog(this);
		}
		m_iQAState = -1;
		Show(false);
	}

}

void CDlgBuddyState::SetBuddyID(int idPlayer, int iType)
{
	if (0 == stricmp(GetName(), "Win_BuddyState"))
	{
		CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(idPlayer);
		if( pPlayer )
		{
			m_pImg_Prof->SetHint(GetStringFromTable(PROFESSION_START + pPlayer->GetProfession()));
			m_pImg_Prof->FixFrame(pPlayer->GetProfession());
			m_pTxt_Name->SetText(pPlayer->GetName());
		}
		else
		{
			m_pTxt_Name->SetText(_AL(""));
			m_pImg_Prof->FixFrame(0);
		}
		if (iType == QA_BUDDYSTATE)
		{
			m_pBtn_Kiss->Show(true);
			m_pBtn_Kiss->SetText(GetStringFromTable(7100));
			m_pBtn_Cancel->Show(true);
			m_pBtn_Cancel->SetText(GetStringFromTable(7101));
		}
		else if (iType == QA_RIDETOGETHER)
		{
			m_pBtn_Kiss->Show(true);
			m_pBtn_Kiss->Enable(true);
			m_pBtn_Kiss->SetText(GetStringFromTable(7102));
			m_pBtn_Cancel->Show(false);
		}
		Show(true);
	}
	else if (0 == stricmp(GetName(), "Win_RideTogether"))
	{
		if (!m_bMinimized)
		{
			m_pImg_Prof->SetHint(GetStringFromTable(PROFESSION_START + GetHostPlayer()->GetProfession()));
			m_pImg_Prof->FixFrame(GetHostPlayer()->GetProfession());
			Show(true);
		}
	}
}


AUI_BEGIN_COMMAND_MAP(CDlgQilinBuddy, CDlgBase)

AUI_ON_COMMAND("QILINUP",		OnCommand_QilinUp)
AUI_ON_COMMAND("ABOLISH",		OnCommand_BindCancel)

AUI_END_COMMAND_MAP()

CDlgQilinBuddy::CDlgQilinBuddy()
{

}

CDlgQilinBuddy::~CDlgQilinBuddy()
{

}

void CDlgQilinBuddy::OnCommand_BindCancel(const char * szCommand)
{
	GetGameSession()->c2s_CmdCancelQilinBinding();
}


void CDlgQilinBuddy::OnCommand_QilinUp(const char * szCommand)
{
	GetHostPlayer()->ReConnectQilin();
}

void CDlgQilinBuddy::OnTick()
{
	CECHostPlayer* pHost = GetHostPlayer();
	
	if(!pHost)
		return;
	
	if(pHost->GetQilinConnectBuddy())
		GetDlgItem("ABOLISH")->Enable(true);
	else 
		GetDlgItem("ABOLISH")->Enable(false);

	if(pHost->GetQilinConnectBuddy() &&  pHost->GetQilinConnectHangedOn() &&  pHost->GetBuddyState() == 0 &&
		pHost->GetHPWorkMan()->GetCurWorkID() != CECHPWork::WORK_BE_MOVED && pHost->CanDo(CECHostPlayer::CANDO_SPELLMAGIC))
		GetDlgItem("QILINUP")->Enable(true);
	else
		GetDlgItem("QILINUP")->Enable(false);
}