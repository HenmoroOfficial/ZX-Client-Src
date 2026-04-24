// Filename	: DlgQuitReason.cpp
// Creator	: Wu Weixin
// Date		: 2012/9/5

#include "DlgQuitReason.h"
#include "DlgDailyPrompLoginHint.h"
#include "EC_MsgDef.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUIStillImageButton.h"

AUI_BEGIN_COMMAND_MAP(CDlgQuitReason, CDlgBase)

AUI_ON_COMMAND("Btn_Confirm",	OnCommandConfirm)

AUI_END_COMMAND_MAP()


CDlgQuitReason::CDlgQuitReason()
{
}

CDlgQuitReason::~CDlgQuitReason()
{
}

bool CDlgQuitReason::OnInitDialog()
{
	m_pBtnConfirm = (AUIStillImageButton*)GetDlgItem("Btn_Confirm");

	AString szName;
	for (unsigned char i = 0; i < REASON_COUNT; ++ i)
	{
		szName.Format("Rdo_%d", i+1);
		m_pRdoReasons[i] = (AUIRadioButton*)GetDlgItem(szName);
	}

	return true;
}

void CDlgQuitReason::OnShowDialog()
{
	m_pBtnConfirm->Enable(false);

	for (unsigned char i = 0; i < REASON_COUNT; ++ i)
	{
		m_pRdoReasons[i]->Check(false);
	}
}

void CDlgQuitReason::OnTick()
{
	bool bChecked = false;
	for (unsigned char i = 0; i < REASON_COUNT; ++ i)
	{
		if (m_pRdoReasons[i]->IsChecked())
		{
			bChecked = true;
			break;
		}
	}
	m_pBtnConfirm->Enable(bChecked);
}

void CDlgQuitReason::OnCommandConfirm(const char* szCommand)
{
	AString szName;
	for (unsigned char i = 0; i < REASON_COUNT; ++ i)
	{
		if (m_pRdoReasons[i]->IsChecked())
		{
			g_pGame->GetGameSession()->c2s_SendCmdPlayerFirstExitReason(i);
			break;
		}
	}
	Show(false);
}
