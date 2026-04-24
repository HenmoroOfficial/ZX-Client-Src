// File		: DlgGoldAccount.h
// Creator	: Xiao Zhou
// Date		: 2006/9/20

#include "DlgGoldAccount.h"
#include "DlgGoldTrade.h"
#include "DlgGoldInquire.h"
#include "DlgInputNO.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgGoldAccount, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommandConfirm)
AUI_ON_COMMAND("choosemoney",	OnCommandChooseMoney)
AUI_ON_COMMAND("goldmax",		OnCommandGoldMax)
AUI_ON_COMMAND("moneymax",		OnCommandMoneyMax)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgGoldAccount, CDlgBase)

AUI_ON_EVENT("Img_FrameMoney",	WM_LBUTTONUP,	OnEventLButtonUp)

AUI_END_EVENT_MAP()


CDlgGoldAccount::CDlgGoldAccount()
{
}

CDlgGoldAccount::~CDlgGoldAccount()
{
}

void CDlgGoldAccount::OnCommandConfirm(const char * szCommand)
{
	int nCash = a_atoi(m_pTxt_Gold->GetText()) * 100 + a_atoi(m_pTxt_Silver->GetText());
	if( nCash > (int)m_pTxt_Gold->GetData() )
		nCash = m_pTxt_Gold->GetData();
	int nMoney = m_pTxt_Money1->GetData();
	if( nMoney > (int)m_pTxt_Money3->GetData() )
		nMoney = m_pTxt_Money3->GetData();
	if( nMoney != 0 || nCash != 0 )
	{
		GetGameSession()->c2s_CmdNPCSevStockTransaction(GetData(), nCash, nMoney);
	}
	Show(false);
}

void CDlgGoldAccount::OnCommandChooseMoney(const char * szCommand)
{
	CECHostPlayer *pHost = GetHostPlayer();
	CDlgInputNO* pShow = GetGameUIMan()->m_pDlgInputMoney;
	PAUIOBJECT pEdit = pShow->GetDlgItem("DEFAULT_Txt_Money1");
	pEdit->SetData(m_pTxt_Money3->GetData());

	pShow->Show(true, true);
	pShow->SetType(CDlgInputNO::INPUTNO_GOLDACCOUNT_MONEY);
}

void CDlgGoldAccount::OnCommandGoldMax(const char * szCommand)
{
	int nCash = m_pTxt_Gold->GetData();
	ACHAR szText[20];
	a_sprintf(szText, _AL("%d"), nCash / 100);
	m_pTxt_Gold->SetText(szText);
	a_sprintf(szText, _AL("%d"), nCash % 100);
	m_pTxt_Silver->SetText(szText);
}

void CDlgGoldAccount::OnCommandMoneyMax(const char * szCommand)
{
	m_pTxt_Money1->SetData(m_pTxt_Money3->GetData());
}

void CDlgGoldAccount::OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	OnCommandChooseMoney("");
}

bool CDlgGoldAccount::OnInitDialog()
{
	m_pTxt_Gold = (PAUIEDITBOX)GetDlgItem("Txt_Gold");
	m_pTxt_Gold->IsNumberOnly();
	m_pTxt_Silver = (PAUIEDITBOX)GetDlgItem("Txt_Silver");
	m_pTxt_Silver->IsNumberOnly();
	m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
	m_pTxt_Money2 = (PAUILABEL)GetDlgItem("Txt_Money2");
	m_pTxt_Money3 = (PAUILABEL)GetDlgItem("Txt_Money3");
	m_pTxt_Money1->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money2->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money3->SetAlign(AUIFRAME_ALIGN_RIGHT);

	return true;
}

void CDlgGoldAccount::OnShowDialog()
{
	m_pTxt_Money1->SetData(0);
	m_pTxt_Gold->SetText(_AL(""));
	m_pTxt_Silver->SetText(_AL(""));
}

void CDlgGoldAccount::OnTick()
{
	int nMoney;
	ACHAR szMoney[20];
	
	nMoney = m_pTxt_Money1->GetData();
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
	m_pTxt_Money3->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pTxt_Money2->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pTxt_Money1->SetText(szMoney);
}
