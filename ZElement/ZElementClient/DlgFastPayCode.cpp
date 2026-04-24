// Filename	: DlgFastPay.cpp
// Creator	: ice
// Date		: 2012/07/06
// Desc		: 快捷充值--短信验证码

#include "DlgFastPayCode.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagepicture.h"
#include "afi.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMisc.h"
#include "EC_GameUIMan.h"
#include "DlgFastPay.h"

#include "A2DSprite.h"
#include "ExpTypes.h"

#define new A_DEBUG_NEW

extern CECGame* g_pGame;

AUI_BEGIN_COMMAND_MAP(CDlgFastPayCode, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommand_Confirm)
AUI_ON_COMMAND("Btn_Get",		OnCommand_Get)


AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgFastPayCode, CDlgBase)


AUI_END_EVENT_MAP()

CDlgFastPayCode::CDlgFastPayCode()
{
	m_nSeconds = 60;
	m_dwRefreshTime = 0;
}

CDlgFastPayCode::~CDlgFastPayCode()
{
}

bool CDlgFastPayCode::OnInitDialog()
{
	if (!CDlgBase::OnInitDialog())
		return false;

	m_pLabelSeconds = (AUILabel*)GetDlgItem("Lab_Time");
	m_pBtnGet = (AUIStillImageButton*)GetDlgItem("Btn_Get");
	m_pBtnGet->Enable(false);

	return true;
}
bool CDlgFastPayCode::Tick(void)
{
	DWORD dwTimeNow = GetTickCount();
	int nSecondsLeft = 60 - (dwTimeNow - m_dwRefreshTime)/1000;	
	if(nSecondsLeft <= 0)
	{
		m_pLabelSeconds->SetText(_AL(""));
		m_pBtnGet->Enable(true);
	}
	else if(nSecondsLeft > 0)
	{
		ACString strSeconds;
		strSeconds.Format(GetGameUIMan()->GetStringFromTable(1601), nSecondsLeft);
		m_pLabelSeconds->SetText(strSeconds);
		m_pBtnGet->Enable(false);
	}

	return true;
}

void CDlgFastPayCode::OnCommand_Confirm(const char * szCommand)
{	
	Show(false);
}

void CDlgFastPayCode::OnCommand_Get(const char * szCommand)
{
	CDlgFastPay* pDlg = (CDlgFastPay*)GetGameUIMan()->GetDialog("Win_Qshop_QuickInputMoney");
	if(pDlg)
		pDlg->SendFastPayCmd();

	m_dwRefreshTime = GetTickCount();

	m_pBtnGet->Enable(false);
}

void CDlgFastPayCode::OnShowDialog()
{
	m_dwRefreshTime = GetTickCount();
}

bool CDlgFastPayCode::Release()
{
	return CDlgBase::Release();
}




