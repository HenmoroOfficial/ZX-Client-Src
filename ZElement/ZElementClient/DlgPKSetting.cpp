// Filename	: DlgPKSetting.cpp
// Creator	: Xiao Zhou
// Date		: 2005/11/18

#include "DlgPKSetting.h"
#include "DlgMiniMap.h"
#include "DlgSetting.h"
#include "EC_Game.h"
#include "EC_Configs.h"
#include "EC_GameUIMan.h"
#include "EC_Global.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_GameSession.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgPKSetting, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommand_Confirm)

AUI_END_COMMAND_MAP()

CDlgPKSetting::CDlgPKSetting()
{
	m_pChk_Bprotect = NULL;
	m_pChk_Wprotect = NULL;
	m_pChk_Gprotect = NULL;
	m_pChk_Rprotect = NULL;
	m_pChk_Zprotect = NULL;
}

CDlgPKSetting::~CDlgPKSetting()
{
}

void CDlgPKSetting::OnCommand_Confirm(const char * szCommand)
{
	EC_GAME_SETTING gs = GetGame()->GetConfigs()->GetGameSettings();
	if (m_szName=="Win_PKSetting")
	{
		gs.bAtk_NoMafia = m_pChk_Gprotect->IsChecked();
		gs.bAtk_NoWhite = m_pChk_Wprotect->IsChecked();
		gs.bAtk_NoBlue = m_pChk_Bprotect->IsChecked();
		gs.bAtk_NoRed = m_pChk_Rprotect->IsChecked();

	} 
	else
		gs.bAtk_NoZone = m_pChk_Zprotect->IsChecked();

	int nTabCheck = GetCheckedRadioButton(1);
	if(nTabCheck == 1)
		GetHostPlayer()->SetTabSelOrder(CECHostPlayer::TABSEL_ALL);
	else if(nTabCheck == 2)
		GetHostPlayer()->SetTabSelOrder(CECHostPlayer::TABSEL_MONSTER);
	else if(nTabCheck == 3)
		GetHostPlayer()->SetTabSelOrder(CECHostPlayer::TABSEL_PLAYER);

	GetGame()->GetConfigs()->SetGameSettings(gs);
	BYTE pvp_mask = GetHostPlayer()->BuildPVPMask(false);
	GetGameSession()->c2s_CmdUpatePVPMask(pvp_mask);

	// to fix some bugs
	if(GetHostPlayer()->GetSelectedTarget())
		GetGameSession()->c2s_CmdUnselect();

	Show(false);
}

bool CDlgPKSetting::OnInitDialog()
{
	if (m_szName=="Win_PKSetting")
	{
		DDX_Control("Chk_Bprotect", m_pChk_Bprotect);
		DDX_Control("Chk_Rprotect", m_pChk_Rprotect);
		DDX_Control("Chk_Gprotect", m_pChk_Gprotect);
		DDX_Control("Chk_Wprotect", m_pChk_Wprotect);
	}
	else
		DDX_Control("Chk_SameServerprotect", m_pChk_Zprotect);

	return true;
}

void CDlgPKSetting::OnShowDialog()
{
	if (m_szName=="Win_PKSetting")
	{
		m_pChk_Gprotect->Check(GetGame()->GetConfigs()->GetGameSettings().bAtk_NoMafia);
		m_pChk_Wprotect->Check(GetGame()->GetConfigs()->GetGameSettings().bAtk_NoWhite);
		m_pChk_Rprotect->Check(GetGame()->GetConfigs()->GetGameSettings().bAtk_NoRed);
		m_pChk_Bprotect->Check(GetGame()->GetConfigs()->GetGameSettings().bAtk_NoBlue);
		if (g_pGame->GetGameRun()->IsNoPKLine())
		{
			m_pChk_Wprotect->Check(true);
			m_pChk_Wprotect->Enable(false);
		}
		else
			m_pChk_Wprotect->Enable(true);
	}
	else
		m_pChk_Zprotect->Check(GetGame()->GetConfigs()->GetGameSettings().bAtk_NoZone);

	int iTabOrder = GetHostPlayer()->GetTabSelOrder();
	if(iTabOrder == CECHostPlayer::TABSEL_ALL)
		CheckRadioButton(1, 1);	
	else if(iTabOrder == CECHostPlayer::TABSEL_MONSTER)
		CheckRadioButton(1, 2);	
	else if(iTabOrder == CECHostPlayer::TABSEL_PLAYER)
		CheckRadioButton(1, 3);	
}
