// File		: DlgArrangeTeamSetting.cpp
// Creator	: Fu Chonggang
// Date		: 2009/10/16

//#include "AUI\\AUICheckBox.h"
#include "DlgArrangeTeamSetting.h"
#include "EC_GameUIMan.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_World.h"
#include "EC_HostPlayer.h"
#include "EC_Team.h"
#include "EC_ElsePlayer.h"
#include "EC_ManPlayer.h"
#include "EC_Configs.h"
#include "EC_Game.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgArrangeTeamSetting, CDlgBase)

AUI_ON_COMMAND("Btn_Add",			OnCommandAdd)
AUI_ON_COMMAND("Btn_DeleteOne",		OnCommandDeleteOne)
AUI_ON_COMMAND("Btn_DeleteAll",		OnCommandDeleteAll)
AUI_ON_COMMAND("Chk_1",				OnCommandAutoAll)
AUI_ON_COMMAND("Chk_3",				OnCommandAutoGuild)
AUI_ON_COMMAND("Btn_Confirm",		OnCommandConfirm)
AUI_ON_COMMAND("Btn_Cancel",		OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgArrangeTeamSetting, CDlgBase)

AUI_END_EVENT_MAP()

const int CDlgArrangeTeamSetting::NAMECOUNT = 5;

CDlgArrangeTeamSetting::CDlgArrangeTeamSetting()
{
	m_pLstName = NULL;
	m_pTxtName2 = NULL;
	m_pChkAll = NULL;
	m_pChkFriend = NULL;
	m_pChkGuildServer = NULL;
	m_pChkFamily = NULL;
	m_pLabAll = NULL;
	m_pLabFriend = NULL;
	m_pLabGuildServer = NULL;
	m_pLabFamily = NULL;
}

CDlgArrangeTeamSetting::~CDlgArrangeTeamSetting()
{
}

void CDlgArrangeTeamSetting::DoDataExchange(bool bSave)
{
	CDlgBase::DoDataExchange(bSave);

	DDX_Control("Lst_Info", m_pLstName);
	DDX_Control("Txt_Name2", m_pTxtName2);
	DDX_Control("Chk_1", m_pChkAll);
	DDX_Control("Chk_2", m_pChkFriend);
	DDX_Control("Chk_3", m_pChkGuildServer);
	DDX_Control("Chk_4", m_pChkFamily);
	//DDX_Control("Lab_1", m_pLabAll);
	//DDX_Control("Lab_2", m_pLabFriend);
	//DDX_Control("Lab_3", m_pLabGuildServer);
	//DDX_Control("Lab_4", m_pLabFamily);
}

void CDlgArrangeTeamSetting::OnCommandAdd(const char *szCommand)
{
	if (m_pLstName->GetCount()<EC_AUTOTEAM_LEN)
	{
		m_pLstName->AddString(m_pTxtName2->GetText());
	}
	else
		GetGameUIMan()->MessageBox("", GetStringFromTable(9164), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
}

void CDlgArrangeTeamSetting::OnCommandDeleteOne(const char *szCommand)
{
	if (m_pLstName->GetCurSel()>=0)
	{
		m_pLstName->DeleteString(m_pLstName->GetCurSel());
	}
}

void CDlgArrangeTeamSetting::OnCommandDeleteAll(const char *szCommand)
{
	m_pLstName->ResetContent();
}

void CDlgArrangeTeamSetting::OnCommandAutoAll(const char *szCommand)
{
	bool bEnable = !m_pChkAll->IsChecked();
	m_pChkFriend->Enable(bEnable);
	m_pChkGuildServer->Enable(bEnable);
	m_pChkFamily->Enable(bEnable);
	m_pLstName->Enable(bEnable);
	m_pTxtName2->Enable(bEnable);
	GetDlgItem("Btn_Add")->Enable(bEnable);
	GetDlgItem("Btn_DeleteOne")->Enable(bEnable);
	GetDlgItem("Btn_DeleteAll")->Enable(bEnable);
}

void CDlgArrangeTeamSetting::OnCommandAutoGuild(const char *szCommand)
{
	m_pChkFamily->Enable(!m_pChkGuildServer->IsChecked());
}

void CDlgArrangeTeamSetting::OnCommandConfirm(const char *szCommand)
{
	EC_AUTOTEAM_SETTING setting = GetGame()->GetConfigs()->GetAutoTeamSettings();
	setting.bAutoAll = m_pChkAll->IsChecked();
	setting.bAutoFriend = m_pChkFriend->IsChecked();
	int nStringBase;
	if (GetHostPlayer()->GetSrcServerID()==0)
	{
		setting.bAutoGuild = m_pChkGuildServer->IsChecked();
		setting.bAutoFamily = m_pChkFamily->IsChecked();
		nStringBase=0;
	}
	else
	{
		setting.bAutoServer = m_pChkGuildServer->IsChecked();
		nStringBase=NAMECOUNT;
	}
	int i;
	for (i=0; i<m_pLstName->GetCount(); i++)
	{
		a_sprintf(setting.szName[i+nStringBase], m_pLstName->GetText(i));
	}
	for (i=m_pLstName->GetCount(); i+nStringBase<EC_AUTOTEAM_LEN; i++)
	{
		a_sprintf(setting.szName[i+nStringBase], _AL(""));
	}
	GetGame()->GetConfigs()->SetAutoTeamSettings(setting);
	Show(false);
}

void CDlgArrangeTeamSetting::OnCommandCancel(const char *szCommand)
{
	Show(false);
}

void CDlgArrangeTeamSetting::OnShowDialog()
{
	m_pLstName->ResetContent();
	EC_AUTOTEAM_SETTING setting = GetGame()->GetConfigs()->GetAutoTeamSettings();
	int nStringBase;
	if (GetHostPlayer()->GetSrcServerID()==0)
	{
		m_pChkGuildServer->Check(setting.bAutoGuild);
		m_pChkFamily->Check(setting.bAutoFamily);
		nStringBase=0;
	}
	else
	{
		m_pChkGuildServer->Check(setting.bAutoServer);
		nStringBase=NAMECOUNT;
	}
	for (int i=0; i<NAMECOUNT;/*EC_AUTOTEAM_LEN*/ i++)
	{
		if (setting.szName[i+nStringBase][0])
		{
			m_pLstName->AddString(setting.szName[i+nStringBase]);
		}
	}

	m_pChkAll->Check(setting.bAutoAll);
	m_pChkFriend->Check(setting.bAutoFriend);

	m_pChkFamily->Show(GetHostPlayer()->GetSrcServerID()==0);
	if(NULL != m_pLabFamily)
	   m_pLabFamily->Show(GetHostPlayer()->GetSrcServerID()==0);
	m_pChkGuildServer->Show(GetHostPlayer()->GetSrcServerID()==0);
	m_pChkGuildServer->SetText(GetGameUIMan()->GetStringFromTable(GetHostPlayer()->GetSrcServerID()==0 ? 8800 : 8801));
	if(NULL != m_pLabGuildServer)
	{
	  m_pLabGuildServer->Show(GetHostPlayer()->GetSrcServerID()==0);
	  m_pLabGuildServer->SetText(GetGameUIMan()->GetStringFromTable(GetHostPlayer()->GetSrcServerID()==0 ? 8800 : 8801));
	}
}
