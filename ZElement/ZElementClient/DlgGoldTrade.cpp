// File		: DlgGoldTrade.cpp
// Creator	: Xiao Zhou
// Date		: 2006/9/20

#include "DlgGoldTrade.h"
#include "DlgGoldAccount.h"
#include "DlgGoldInquire.h"
#include "DlgInputNO.h"
#include "DlgQShop.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\CSplit.h"
#include "stockaccount_re.hpp"
#include "stockbill_re.hpp"
#include "stockcommission_re.hpp"
#include "stocktransaction_re.hpp"
#include "stockcancel_re.hpp"
#include "LuaEvent.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgGoldTrade, CDlgBase)

AUI_ON_COMMAND("sell",			OnCommandSell)
AUI_ON_COMMAND("buy",			OnCommandBuy)
AUI_ON_COMMAND("choosemoney",	OnCommandChooseMoney)
AUI_ON_COMMAND("account*",		OnCommandAccount)
AUI_ON_COMMAND("inquire",		OnCommandInquire)
AUI_ON_COMMAND("confirm",		OnCommandConfirm)
AUI_ON_COMMAND("refresh",		OnCommandRefresh)
AUI_ON_COMMAND("pwd",			OnCommandPwd)
AUI_ON_COMMAND("unlock",		OnCommandUnlock)
AUI_ON_COMMAND("help",			OnCommandHelp)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgGoldTrade, CDlgBase)

AUI_ON_EVENT("Img_FrameMoney1",	WM_LBUTTONUP,	OnEventLButtonUp)

AUI_END_EVENT_MAP()

CDlgGoldTrade::CDlgGoldTrade()
{
	m_nBuy = 1;
	m_bLocked = false;
}

CDlgGoldTrade::~CDlgGoldTrade()
{
}

void CDlgGoldTrade::OnCommandHelp(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_GoldTrade_Help");
	//pDlg->Show(!pDlg->IsShow());
	AString strDlg = "Win_GoldTrade_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgGoldTrade::OnCommandChooseMoney(const char * szCommand)
{
	CECHostPlayer *pHost = GetHostPlayer();
	CDlgInputNO* pShow = GetGameUIMan()->m_pDlgInputMoney2;
	PAUIOBJECT pEdit = pShow->GetDlgItem("DEFAULT_Txt_Money1");
	pEdit->SetData(20000000);

	pShow->Show(true, true);
	pShow->SetType(CDlgInputNO::INPUTNO_GOLDTRADE_MONEY);
}

void CDlgGoldTrade::OnCommandAccount(const char * szCommand)
{
	GetGameUIMan()->m_pDlgGoldAccount->Show(true, true);
	if( stricmp(szCommand, "accountsave") == 0 )
	{
		GetGameUIMan()->m_pDlgGoldAccount->GetDlgItem("Title")->Show(true);
		GetGameUIMan()->m_pDlgGoldAccount->GetDlgItem("Title01")->Show(false);
		GetGameUIMan()->m_pDlgGoldAccount->GetDlgItem("Lab_Gold")->Show(true);
		GetGameUIMan()->m_pDlgGoldAccount->GetDlgItem("Lab_Gold01")->Show(false);
		GetGameUIMan()->m_pDlgGoldAccount->GetDlgItem("Lab_Money")->Show(true);
		GetGameUIMan()->m_pDlgGoldAccount->GetDlgItem("Lab_Money01")->Show(false);
		GetGameUIMan()->m_pDlgGoldAccount->m_pTxt_Money3->SetData(GetHostPlayer()->GetMoneyAmount());
		GetGameUIMan()->m_pDlgGoldAccount->m_pTxt_Gold->SetData(GetHostPlayer()->GetCash());
		GetGameUIMan()->m_pDlgGoldAccount->SetData(0);
	}
	else
	{
		GetGameUIMan()->m_pDlgGoldAccount->GetDlgItem("Title")->Show(false);
		GetGameUIMan()->m_pDlgGoldAccount->GetDlgItem("Title01")->Show(true);
		GetGameUIMan()->m_pDlgGoldAccount->GetDlgItem("Lab_Gold")->Show(false);
		GetGameUIMan()->m_pDlgGoldAccount->GetDlgItem("Lab_Gold01")->Show(true);
		GetGameUIMan()->m_pDlgGoldAccount->GetDlgItem("Lab_Money")->Show(false);
		GetGameUIMan()->m_pDlgGoldAccount->GetDlgItem("Lab_Money01")->Show(true);
		GetGameUIMan()->m_pDlgGoldAccount->m_pTxt_Money3->SetData(m_pTxt_Money41->GetData());
		GetGameUIMan()->m_pDlgGoldAccount->m_pTxt_Gold->SetData(m_pTxt_Gold->GetData());
		GetGameUIMan()->m_pDlgGoldAccount->SetData(1);
	}
}

void CDlgGoldTrade::OnCommandInquire(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->m_pDlgGoldInquire->Show(true);
	GetGameUIMan()->m_pDlgGoldInquire->SetPosEx(GetPos().x, GetPos().y);
}

void CDlgGoldTrade::OnCommandConfirm(const char * szCommand)
{
	int nNum = a_atoi(m_pTxt_Num->GetText());
	if( m_pTxt_Money11->GetData() > 0 && nNum > 0 )
	{
		PAUIDIALOG pMsgBox;
		ACHAR szPrice[256];
		CECIvtrItem::GetPriceText(szPrice, m_pTxt_Money11->GetData());
		ACString strText;
		if( m_nBuy == 1 )
		{
			strText.Format(GetStringFromTable(841), szPrice, nNum);
			GetGameUIMan()->MessageBox("Game_GoldBuy", strText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		}
		else
		{
			strText.Format(GetStringFromTable(842), szPrice, nNum);
			GetGameUIMan()->MessageBox("Game_GoldSell", strText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		}
		pMsgBox->SetData(m_pTxt_Money11->GetData());
		pMsgBox->SetDataPtr((void*)nNum);
	}
}

void CDlgGoldTrade::OnCommandRefresh(const char * szCommand)
{
	GetGameSession()->stock_account();
}

void CDlgGoldTrade::OnCommandPwd(const char * szCommand)
{
	GetGameUIMan()->GetDialog("Win_GoldPwdChange")->Show(true);
}

void CDlgGoldTrade::OnCommandUnlock(const char * szCommand)
{
	GetGameUIMan()->GetDialog("Win_GoldPwdInput")->Show(true);
}

void CDlgGoldTrade::OnCommandBuy(const char * szCommand)
{
	m_nBuy = 1;
	GetDlgItem("Lab_Sell01")->Show(false);
	GetDlgItem("Lab_Sella1")->Show(true);
	GetDlgItem("Lab_Sell04")->Show(false);
	GetDlgItem("Lab_Sella4")->Show(true);
	((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Buy"))->SetPushed(true);
	((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Sell"))->SetPushed(false);
}

void CDlgGoldTrade::OnCommandSell(const char * szCommand)
{
	m_nBuy = 0;
	GetDlgItem("Lab_Sell01")->Show(true);
	GetDlgItem("Lab_Sella1")->Show(false);
	GetDlgItem("Lab_Sell04")->Show(true);
	GetDlgItem("Lab_Sella4")->Show(false);
	((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Buy"))->SetPushed(false);
	((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Sell"))->SetPushed(true);
}

void CDlgGoldTrade::OnCommandCancel(const char * szCommand)
{
	GetGameSession()->cash_Lock(1, _AL(""));
	GetGameUIMan()->EndNPCService();
	Show(false);
}

void CDlgGoldTrade::OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	OnCommandChooseMoney("");
}

bool CDlgGoldTrade::OnInitDialog()
{
	m_pLst_Sell = (PAUILISTBOX)GetDlgItem("Lst_Sell");
	m_pLst_Buy = (PAUILISTBOX)GetDlgItem("Lst_Buy");
	m_pTxt_Num = (PAUIEDITBOX)GetDlgItem("Txt_Num");
	m_pTxt_Num->SetIsNumberOnly(true);
	m_pTxt_Gold = (PAUILABEL)GetDlgItem("Txt_Gold");
	m_pTxt_Gold1 = (PAUILABEL)GetDlgItem("Txt_Gold1");
	m_pTxt_Money11 = (PAUILABEL)GetDlgItem("Txt_Money11");
	m_pTxt_Money12 = (PAUILABEL)GetDlgItem("Txt_Money12");
//	m_pTxt_Money13 = (PAUILABEL)GetDlgItem("Txt_Money13");
	m_pTxt_Money11->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money12->SetAlign(AUIFRAME_ALIGN_RIGHT);
//	m_pTxt_Money13->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money21 = (PAUILABEL)GetDlgItem("Txt_Money21");
	m_pTxt_Money22 = (PAUILABEL)GetDlgItem("Txt_Money22");
	m_pTxt_Money23 = (PAUILABEL)GetDlgItem("Txt_Money23");
	m_pTxt_Money21->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money22->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money23->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money41 = (PAUILABEL)GetDlgItem("Txt_Money41");
	m_pTxt_Money42 = (PAUILABEL)GetDlgItem("Txt_Money42");
	m_pTxt_Money43 = (PAUILABEL)GetDlgItem("Txt_Money43");
	m_pTxt_Money41->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money42->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money43->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money51 = (PAUILABEL)GetDlgItem("Txt_Money51");
	m_pTxt_Money52 = (PAUILABEL)GetDlgItem("Txt_Money52");
	m_pTxt_Money53 = (PAUILABEL)GetDlgItem("Txt_Money53");
	m_pTxt_Money51->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money52->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money53->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Poundage = (PAUILABEL)GetDlgItem("Txt_Poundage");

	return true;
}

void CDlgGoldTrade::OnShowDialog()
{
	m_vecAccountOrders.clear();
	GetGameSession()->stock_account();
	m_pLst_Sell->ResetContent();
	m_pLst_Buy->ResetContent();
	m_pTxt_Money11->SetData(0);
	m_pTxt_Money41->SetData(0);
	m_pTxt_Num->SetText(_AL(""));
	m_pTxt_Num->SetData(0);
	m_pTxt_Gold->SetData(0);
	OnCommandBuy("");
	LuaEvent::FirstCashTrade();
}

void CDlgGoldTrade::OnTick()
{
	int nNum = a_atoi(m_pTxt_Num->GetText());
	if( nNum > 1000 )
	{
		m_pTxt_Num->SetText(_AL("1000"));
		nNum = 1000;
	}
	INT64 nMoney = m_pTxt_Money11->GetData();
	ACHAR szMoney[256];
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
//	m_pTxt_Money13->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pTxt_Money12->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pTxt_Money11->SetText(szMoney);
	
	nMoney *= nNum;
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
	m_pTxt_Money23->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pTxt_Money22->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pTxt_Money21->SetText(szMoney);

	if( m_nBuy == 1 )
	{
		nMoney = int(nMoney * 0.02);
		CECIvtrItem::GetPriceText(szMoney, nMoney);
		m_pTxt_Poundage->SetText(szMoney);
	}
	else
	{
		nMoney = nNum * 2;
		m_pTxt_Poundage->SetText(GetGameUIMan()->m_pDlgQShop->GetCashText(nMoney));
	}

	nMoney = m_pTxt_Money41->GetData();
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
	m_pTxt_Money43->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pTxt_Money42->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pTxt_Money41->SetText(szMoney);

	nMoney = GetHostPlayer()->GetMoneyAmount();
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
	m_pTxt_Money53->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pTxt_Money52->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pTxt_Money51->SetText(szMoney);

	m_pTxt_Gold->SetText(GetGameUIMan()->m_pDlgQShop->GetCashText(m_pTxt_Gold->GetData()));
	m_pTxt_Gold1->SetText(GetGameUIMan()->m_pDlgQShop->GetCashText(GetHostPlayer()->GetCash()));
}

void CDlgGoldTrade::GoldAction(int idAction, void *pData)
{
	if( idAction == CDlgGoldTrade::GOLD_ACTION_ACCOUNT_RE )
	{
		StockAccount_Re *p = (StockAccount_Re *)pData;

		SetLocked(p->locked != 0);
		m_pTxt_Gold->SetData(p->cash);
		m_pTxt_Money41->SetData(p->money);
		PAUILISTBOX pListBuy2 = GetGameUIMan()->m_pDlgGoldInquire->m_pLst_Buy;
		PAUILISTBOX pListSell2 = GetGameUIMan()->m_pDlgGoldInquire->m_pLst_Sell;
		m_pLst_Buy->ResetContent();
		m_pLst_Sell->ResetContent();
		pListBuy2->ResetContent();
		pListSell2->ResetContent();
		DWORD i;
		ACString strText, strGold;
		for(i = 0; i < p->prices.size(); i++ )
		{
			strGold.Format(GetStringFromTable(834), p->prices[i].volume);
			if( p->prices[i].price > 0 )
			{
				strText.Format(_AL("%s\t%s\t%s"), GetMoneyText(p->prices[i].price), GetStringFromTable(833), strGold);
				m_pLst_Sell->AddString(strText);
				pListSell2->AddString(strText);
				m_pLst_Sell->SetItemTextColor(m_pLst_Sell->GetCount() - 1, A3DCOLORRGB(255, 255, 0));
				pListSell2->SetItemTextColor(pListSell2->GetCount() - 1, A3DCOLORRGB(255, 255, 0));
			}
			else
			{
				strText.Format(_AL("%s\t%s\t%s"), GetMoneyText(-p->prices[i].price), GetStringFromTable(832), strGold);
				m_pLst_Buy->AddString(strText);
				pListBuy2->AddString(strText);
				m_pLst_Buy->SetItemTextColor(m_pLst_Buy->GetCount() - 1, A3DCOLORRGB(0, 255, 0));
				pListBuy2->SetItemTextColor(pListBuy2->GetCount() - 1, A3DCOLORRGB(0, 255, 0));
			}
		}
	}
	else if( idAction == CDlgGoldTrade::GOLD_ACTION_BILL_RE )
	{
		StockBill_Re *p = (StockBill_Re *)pData;

		m_vecAccountOrders.clear();
		DWORD i;
		ACString strText, strTime;
		for(i = 0; i < p->orders.size(); i++ )
		{
			StockInfo info;
			info.price = p->orders[i].price;
			info.volume = p->orders[i].volume;
			info.tid = p->orders[i].tid;
			info.time = 0;
			m_vecAccountOrders.push_back(info);
		}
		PAUILISTBOX pList =	GetGameUIMan()->m_pDlgGoldInquire->m_pLst_Inquire;
		pList->ResetContent();
		for(i = 0; i < m_vecAccountOrders.size(); i++ )
		{
			if( m_vecAccountOrders[i].price > 0 )
			{
				strText.Format(_AL("%d\t%s\t%d\t%s\t%s\t%s"), m_vecAccountOrders[i].tid, 
					GetStringFromTable(818), 
					m_vecAccountOrders[i].volume, GetMoneyText(m_vecAccountOrders[i].price), _AL("---"),
					GetStringFromTable(821));
			}
			else
			{
				strText.Format(_AL("%d\t%s\t%d\t%s\t%s\t%s"), m_vecAccountOrders[i].tid, 
					GetStringFromTable(817), 
					m_vecAccountOrders[i].volume, GetMoneyText(-m_vecAccountOrders[i].price), _AL("---"),
					GetStringFromTable(821));
			}
			pList->AddString(strText);
			pList->SetItemData(pList->GetCount() - 1, m_vecAccountOrders[i].tid);
			pList->SetItemDataPtr(pList->GetCount() - 1, (void*)m_vecAccountOrders[i].price);
			if( m_vecAccountOrders[i].price > 0 )
				pList->SetItemTextColor(pList->GetCount() - 1, A3DCOLORRGB(255, 255, 0));
			else
				pList->SetItemTextColor(pList->GetCount() - 1, A3DCOLORRGB(0, 255, 0));
		}
		int n = pList->GetCount();
		for(i = 0; i < p->logs.size(); i++ )
		{
			long stime = p->logs[i].time;
			stime -= GetGame()->GetTimeZoneBias() * 60; // localtime = UTC - bias, in which bias is in minutes
			tm *gtime = gmtime((const time_t*)&stime);
			strTime.Format(GetStringFromTable(781), 
				gtime->tm_mon + 1, gtime->tm_mday, gtime->tm_hour, gtime->tm_min);
			ACString strPrice, strTotalPrice;
			StockLog &log = p->logs[i];
			if( p->logs[i].result == 1 )
			{
				strPrice = GetMoneyText(abs(p->logs[i].cost));
				strTotalPrice = GetStringFromTable(844);
			}
			else if( p->logs[i].result == 2 )
			{
				strPrice = GetMoneyText(abs(p->logs[i].cost));
				strTotalPrice = GetStringFromTable(845);
			}
			else
			{
				strPrice = _AL("---");
				strTotalPrice = GetMoneyText(abs(p->logs[i].cost));
			}
			if( p->logs[i].cost < 0 )
			{
				strText.Format(_AL("%d\t%s\t%d\t%s\t%s\t%s"), p->logs[i].tid, 
					GetStringFromTable(817), 
					p->logs[i].volume, strPrice, strTotalPrice,
					strTime);
			}
			else
			{
				strText.Format(_AL("%d\t%s\t%d\t%s\t%s\t%s"), p->logs[i].tid, 
					GetStringFromTable(818), 
					p->logs[i].volume, strPrice, strTotalPrice,
					strTime);
			}
			pList->InsertString(n, strText);
			if( p->logs[i].cost < 0 )
				pList->SetItemTextColor(n, A3DCOLORRGB(0, 255, 0));
			else
				pList->SetItemTextColor(n, A3DCOLORRGB(255, 255, 0));
		}
	}
	else if( idAction == CDlgGoldTrade::GOLD_ACTION_COMMISSION_RE )
	{
		StockCommission_Re *p = (StockCommission_Re *)pData;
		if( !CheckRetcode(p->retcode, 819) )
			return;

		m_pTxt_Gold->SetData(p->cash);
		m_pTxt_Money41->SetData(p->money);
		PAUILISTBOX pListBuy2 = GetGameUIMan()->m_pDlgGoldInquire->m_pLst_Buy;
		PAUILISTBOX pListSell2 = GetGameUIMan()->m_pDlgGoldInquire->m_pLst_Sell;
		m_pLst_Buy->ResetContent();
		m_pLst_Sell->ResetContent();
		pListBuy2->ResetContent();
		pListSell2->ResetContent();
		DWORD i;
		ACString strText, strGold;
		for(i = 0; i < p->prices.size(); i++ )
		{
			strGold.Format(GetStringFromTable(834), p->prices[i].volume);
			if( p->prices[i].price > 0 )
			{
				strText.Format(_AL("%s\t%s\t%s"), GetMoneyText(p->prices[i].price), GetStringFromTable(833), strGold);
				m_pLst_Sell->AddString(strText);
				pListSell2->AddString(strText);
				m_pLst_Sell->SetItemTextColor(m_pLst_Sell->GetCount() - 1, A3DCOLORRGB(255, 255, 0));
				pListSell2->SetItemTextColor(pListSell2->GetCount() - 1, A3DCOLORRGB(255, 255, 0));
			}
			else
			{
				strText.Format(_AL("%s\t%s\t%s"), GetMoneyText(-p->prices[i].price), GetStringFromTable(832), strGold);
				m_pLst_Buy->AddString(strText);
				pListBuy2->AddString(strText);
				m_pLst_Buy->SetItemTextColor(m_pLst_Buy->GetCount() - 1, A3DCOLORRGB(0, 255, 0));
				pListBuy2->SetItemTextColor(pListBuy2->GetCount() - 1, A3DCOLORRGB(0, 255, 0));
			}
		}
	}
	else if( idAction == CDlgGoldTrade::GOLD_ACTION_TRANSACTION_RE )
	{
		StockTransaction_Re *p = (StockTransaction_Re *)pData;
		if( !CheckRetcode(p->retcode, 820) )
			return;

		m_pTxt_Gold->SetData(p->cash);
		m_pTxt_Money41->SetData(p->money);
	}
	else if( idAction == CDlgGoldTrade::GOLD_ACTION_CANCEL_RE )
	{
		StockCancel_Re *p = (StockCancel_Re *)pData;
		if( !CheckRetcode(p->retcode, 822) )
			return;

		PAUILISTBOX pList =	GetGameUIMan()->m_pDlgGoldInquire->m_pLst_Inquire;
		for(int i = 0; i < pList->GetCount(); i++ )
			if( pList->GetItemData(i) == p->tid )
			{
				CSplit s(pList->GetText(i));
				CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
				vec[4] = GetStringFromTable(844);
				ACString strText;
				strText.Format(_AL("%s\t%s\t%s\t%s\t%s\t%s"), vec[0], vec[1], vec[2], vec[3], vec[4], vec[5]);
				pList->SetText(i, strText);
				break;
			}
	}
}

ACString CDlgGoldTrade::GetMoneyText(int nMoney)
{
	ACString strText = _AL("");
	ACString strTemp;
	if( nMoney >= 10000 )
	{
		strTemp.Format(GetStringFromTable(1701), nMoney / 10000);
		strText += strTemp;
	}
	nMoney = nMoney % 10000;
	if( strText == _AL("") || nMoney >= 100 )
	{
		strTemp.Format(GetStringFromTable(1702), nMoney / 100);
		strText += strTemp;
	}
	return strText;
}

bool CDlgGoldTrade::CheckRetcode(int retcode, int defaulErrMsg)
{
	if( retcode == ERR_SUCCESS )
		return true;
	if( retcode == ERR_STOCK_CLOSED )
		GetGameUIMan()->MessageBox("", GetStringFromTable(835), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	else if( retcode == ERR_STOCK_ACCOUNTBUSY )
		GetGameUIMan()->MessageBox("", GetStringFromTable(836), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	else if( retcode == ERR_STOCK_INVALIDINPUT )
		GetGameUIMan()->MessageBox("", GetStringFromTable(837), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	else if( retcode == ERR_STOCK_OVERFLOW )
		GetGameUIMan()->MessageBox("", GetStringFromTable(838), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	else if( retcode == ERR_STOCK_DATABASE )
		GetGameUIMan()->MessageBox("", GetStringFromTable(839), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	else if( retcode == ERR_STOCK_NOTENOUGH )
		GetGameUIMan()->MessageBox("", GetStringFromTable(840), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	else
		GetGameUIMan()->MessageBox("", GetStringFromTable(defaulErrMsg), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	return false;
}

void CDlgGoldTrade::SetLocked(bool bLock)
{
	m_bLocked = bLock;
	GetDlgItem("Btn_Pwd")->Show(!bLock);
	GetDlgItem("Btn_Consign")->Enable(!bLock);
	GetDlgItem("Btn_In")->Enable(!bLock);
	GetDlgItem("Btn_Out")->Enable(!bLock);
	GetDlgItem("Btn_Unlock")->Show(bLock);
	GetDlgItem("Txt_Lock")->Show(bLock);
	GetGameUIMan()->GetDialog("Win_GoldInquire")->GetDlgItem("Btn_GiveUp")->Show(!bLock);
	GetGameUIMan()->GetDialog("Win_GoldInquire")->GetDlgItem("Btn_Unlock")->Show(bLock);
	GetGameUIMan()->GetDialog("Win_GoldInquire")->GetDlgItem("Txt_Lock")->Show(bLock);
}