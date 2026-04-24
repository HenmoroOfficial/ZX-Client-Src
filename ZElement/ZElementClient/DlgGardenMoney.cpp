// Filename	: DlgGardenMoney.cpp
// Creator	: Fu Chonggang
// Date		: 2009/08/21

#include "AFI.h"
#include "AUI\\AUILabel.h"
#include "DlgGardenMoney.h"
#include "DlgModule.h"
#include "DlgGardenInventory.h"
#include "EC_GameUIMan.h"
#include "EC_HTPDataType.h"
#include "EC_HTC2SRawCmds.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "hometowntemplate.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgGardenMoney, CDlgBase)

AUI_ON_COMMAND("Btn_Gold2System",	OnCommand_Gold2System)
AUI_ON_COMMAND("Btn_Garden2Money",	OnCommand_Garden2Money)
AUI_ON_COMMAND("Btn_Confirm1",		OnCommand_Confirm1)
AUI_ON_COMMAND("Btn_Confirm2",		OnCommand_Confirm2)
AUI_ON_COMMAND("Btn_Confirm3",		OnCommand_Confirm3)
AUI_ON_COMMAND("Btn_BuyGold",		OnCommand_BuyGold)
AUI_ON_COMMAND("Btn_BuyMoney",		OnCommand_BuyMoney)
AUI_ON_COMMAND("Btn_Close",			OnCommand_Close)

AUI_END_COMMAND_MAP()

using namespace HT_S2C;
using namespace HT_S2C::CMD;

CDlgGardenMoney::CDlgGardenMoney()
{
}

CDlgGardenMoney::~CDlgGardenMoney()
{
}

bool CDlgGardenMoney::OnInitDialog()
{
	HOMETOWN::HometownTemplate* homeTemplate = GetGame()->GetHomeTownDataMan();
	const FARM_CONFIG *farmConfig = homeTemplate->get_farm_config();
	m_iMoneyLocal = farmConfig->money_local;
	m_iMoneyUniversal = farmConfig->money_universal;
	ACString strText;
	if (stricmp(m_szName, "Win_Garden_Exchange1") == 0)
	{
		m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
		m_pTxt_Money2 = (PAUILABEL)GetDlgItem("Txt_Money2");
		m_pTxt_Money3 = (PAUILABEL)GetDlgItem("Txt_Money3");
		m_pTxt_Money1->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Money1->SetText(_AL("0"));
		m_pTxt_Money2->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Money2->SetText(_AL("0"));
		m_pTxt_Money3->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Money3->SetText(_AL("0"));
	}
	else if (stricmp(m_szName, "Win_Garden_Exchange2") == 0)
	{
		m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
		strText.Format(_AL("1:%d"),m_iMoneyLocal);
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Rate");
		pLab->SetText(strText);
	}
	else if (stricmp(m_szName, "Win_Garden_Exchange3") == 0)
	{
		m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
		strText.Format(_AL("1:%d"),m_iMoneyUniversal);
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Rate");
		pLab->SetText(strText);
	}
	
	return true;
}

void CDlgGardenMoney::OnCommand_Gold2System(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Garden_Exchange1")->Show(true);
}

void CDlgGardenMoney::OnCommand_Garden2Money(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Garden_Exchange2")->Show(true);
}

void CDlgGardenMoney::OnCommand_Confirm1(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Edt_Num");
	int iMoney = a_atoi(pEdt->GetText());
	if (iMoney<=500)
		GetGameSession()->c2s_CmdExgHomeTownMoney(iMoney);
	else
		GetGameUIMan()->MessageBox("",GetGameUIMan()->GetStringFromTable(9100),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
}

void CDlgGardenMoney::OnCommand_Confirm2(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Edt_Num");
	htc2s_SendCmdExgMoney(1,a_atoi(pEdt->GetText()));
}

void CDlgGardenMoney::OnCommand_Confirm3(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Edt_Num");
	int iMoney = a_atoi(pEdt->GetText());
	if (iMoney<=100)
		htc2s_SendCmdExgMoney(0,iMoney);
	else
		GetGameUIMan()->MessageBox("",GetGameUIMan()->GetStringFromTable(9101),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
}

void CDlgGardenMoney::OnCommand_BuyGold(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Garden_Exchange2")->Show(true);
}

void CDlgGardenMoney::OnCommand_BuyMoney(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Garden_Exchange3")->Show(true);
}

void CDlgGardenMoney::OnCommand_Close(const char * szCommand)
{
	Show(false);
}

void CDlgGardenMoney::OnTick()
{
	if (GetGameUIMan()->GetActiveDialog() != this)
	{
		Show(false);
	}
	if (stricmp(m_szName, "Win_Garden_ExchangeMain") == 0)
	{
		PAUILABEL pLabMoney = (PAUILABEL)GetDlgItem("Lab_Money");
		INT64 iMoney = CDlgModule::GetModuleMoney();
		ACString strMoney;
		GetGameUIMan()->Int64ToStr(iMoney, strMoney);
		pLabMoney->SetText(strMoney);
		return;
	}
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Edt_Num");
	int iMoney = a_atoi(pEdt->GetText());
	ACString strText;
	if (stricmp(m_szName, "Win_Garden_Exchange1") == 0)
	{
		strText.Format(_AL("%d"), iMoney);
		m_pTxt_Money1->SetText(strText);
	}
	else if (stricmp(m_szName, "Win_Garden_Exchange2") == 0)
	{
		iMoney *= m_iMoneyLocal;
		strText.Format(_AL("%d"), iMoney);
		m_pTxt_Money1->SetText(strText);
	}
	else if (stricmp(m_szName, "Win_Garden_Exchange3") == 0)
	{
		iMoney *= m_iMoneyUniversal;
		strText.Format(_AL("%d"), iMoney);
		m_pTxt_Money1->SetText(strText);
	}
	if (GetGameUIMan()->GetActiveDialog() != this)
	{
		Show(false);
	}
}
