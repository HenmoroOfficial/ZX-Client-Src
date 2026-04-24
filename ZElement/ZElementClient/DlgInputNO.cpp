// Filename	: DlgInputNO.cpp
// Creator	: Tom Zhou
// Date		: October 10, 2005

#include "DlgInputNO.h"
#include "DlgShop.h"
#include "DlgShopReputation.h"
#include "DlgTrade.h"
#include "DlgInventory.h"
#include "DlgStorage.h"
#include "DlgMailWrite.h"
#include "DlgQShop.h"
#include "EC_DealInventory.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_Global.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "EC_World.h"
#include "EC_Configs.h"
#include "globaldataman.h"

#include "EC_HTC2SRawCmds.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgInputNO, CDlgBase)

AUI_ON_COMMAND("max",				OnCommand_max)
//AUI_ON_COMMAND("add*",				OnCommand_add)
//AUI_ON_COMMAND("minus*",			OnCommand_minus)
AUI_ON_COMMAND("drop",				OnCommand_drop)
AUI_ON_COMMAND("confirm",			OnCommand_confirm)
AUI_ON_COMMAND("IDCANCEL",			OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgInputNO, CDlgBase)

AUI_ON_EVENT("*",		WM_KEYDOWN,			OnEventKeyDown_Cancel)
AUI_ON_EVENT("Btn_Add*",		WM_LBUTTONUP,	OnEventLButtonUp)
AUI_ON_EVENT("Btn_Minus*",		WM_LBUTTONUP,	OnEventLButtonUp)
AUI_ON_EVENT("DEFAULT_Txt_*",	WM_LBUTTONDOWN, OnEventLButtonDown)

AUI_END_EVENT_MAP()


CDlgInputNO::CDlgInputNO()
{
	m_nInputNO = INPUTNO_NULL;
	m_dwTimeClickStart = 0;
}

CDlgInputNO::~CDlgInputNO()
{
}

void CDlgInputNO::OnShowDialog()
{
	if( m_szName == "Win_InputNO" || m_szName == "Win_InputGNO" )
	{
		GetDlgItem("DEFAULT_Txt_No.")->SetText(_AL(""));
		if( m_szName == "Win_InputGNO" )
		{
			if ( CDlgQShop::GetShopType()==CDlgQShop::SHOP_TYPE_Q )
				GetDlgItem("Txt_Tip")->SetText(GetStringFromTable(7701));
			else if ( CDlgQShop::GetShopType()==CDlgQShop::SHOP_TYPE_POP )
				GetDlgItem("Txt_Tip")->SetText(GetStringFromTable(7702));
			else
				GetDlgItem("Txt_Tip")->SetText(GetStringFromTable(7699));
		}
	}
	if( m_szName == "Win_Message15" )
	{
		ACHAR szText[256];
		ACHAR szText1[256];
		CECIvtrItem::GetPriceText(szText, GetData());
		a_sprintf(szText1,GetStringFromTable(886),szText);
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_TextDrop");
		pLab->SetText(szText1);
	}
	else if( m_szName == "Win_InputMoney" || m_szName == "Win_InputMoney2" )
	{
		GetDlgItem("DEFAULT_Txt_Money1")->SetText(_AL(""));
		GetDlgItem("DEFAULT_Txt_Money2")->SetText(_AL("0"));
		if( m_szName == "Win_InputMoney" )
			GetDlgItem("DEFAULT_Txt_Money3")->SetText(_AL("0"));
	}
}

bool CDlgInputNO::OnInitDialog()
{
	if( m_szName == "Win_InputNO" || m_szName == "Win_InputGNO" )
	{
		((PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_No."))->SetIsNumberOnly(true);
	}
	else if( m_szName == "Win_InputMoney" || m_szName == "Win_InputMoney2" )
	{
		((PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Money1"))->SetIsNumberOnly(true);
		((PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Money2"))->SetIsNumberOnly(true);
		if( m_szName == "Win_InputMoney" )
			((PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Money3"))->SetIsNumberOnly(true);
	}
	else if( m_szName == "Win_Message15" )
	{
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_TextDrop");
		pLab->SetAlign(AUIFRAME_ALIGN_CENTER);
	}

	return true;
}

void CDlgInputNO::OnTick()
{
	if( m_szName == "Win_InputNO" || m_szName == "Win_InputGNO" )
	{
		if( GetFocus() != GetDlgItem("DEFAULT_Txt_No.") && a_atoi(GetDlgItem("DEFAULT_Txt_No.")->GetText()) == 0 )
			GetDlgItem("DEFAULT_Txt_No.")->SetText(_AL("0"));
		if( m_szName == "Win_InputGNO" )
		{
			int nNum = a_atoi(GetDlgItem("DEFAULT_Txt_No.")->GetText());
			vector<GSHOP_ITEM> *items;
			ACString strText;
			bool bDisableConsumeScore = GetGame()->GetConfigs()->IsConsumeScoreDisable();
			GetDlgItem("Lab_ConsumeScore")->Show(!bDisableConsumeScore && CDlgQShop::SHOP_TYPE_Q==CDlgQShop::GetShopType());
			GetDlgItem("Txt_ConsumeScore")->Show(!bDisableConsumeScore && CDlgQShop::SHOP_TYPE_Q==CDlgQShop::GetShopType());
			int index = GetDlgItem("Txt_Confirm")->GetData();
			if ( CDlgQShop::SHOP_TYPE_Q==CDlgQShop::GetShopType() )
			{
				items = globaldata_getgshopitems();
				strText.Format(GetStringFromTable(810), GetGameUIMan()->m_pDlgQShop->GetCashText(GetData() * nNum), GetText());
				GetDlgItem("Txt_Off")->Show(GetDlgItem("Txt_Off")->GetData()>0);

				PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_ConsumeScore");
				ACString strText;
				strText.Format(_AL("%d"), GetData() * nNum / 10);
				pLab->SetText(strText);
			}
			else if ( CDlgQShop::SHOP_TYPE_POP==CDlgQShop::GetShopType() )
			{
				items = globaldata_getgshopitems2();
				strText.Format(GetStringFromTable(810), GetGameUIMan()->m_pDlgQShop->GetCashText(GetData() * nNum), GetText());
				GetDlgItem("Txt_Off")->Show(false);
			}
			else if ( CDlgQShop::SHOP_TYPE_CROSSSERVER==CDlgQShop::GetShopType() )
			{
				items = globaldata_getgshopitems3();
				ACString strItem;
				strItem.Format(_AL("%d"), GetData() * nNum);
				strText.Format(GetStringFromTable(810), strItem, GetText());
				GetDlgItem("Txt_Off")->Show(false);
			}
			GetDlgItem("Txt_Confirm")->SetText(strText);
			GetDlgItem("Txt_Confirm")->SetAlign(AUIFRAME_ALIGN_CENTER);

			strText.Format(GetStringFromTable(7393), GetGameUIMan()->m_pDlgQShop->GetCashText(GetDlgItem("Txt_Off")->GetData() * nNum));
			GetDlgItem("Txt_Off")->SetText(strText);
			GetDlgItem("Txt_Off")->SetAlign(AUIFRAME_ALIGN_CENTER);
			strText.Format(GetStringFromTable(850), (*items)[index].num, (*items)[index].num * nNum);
			GetDlgItem("Txt_TotalNum")->SetText(strText);
			if( nNum <= (int)GetDlgItem("DEFAULT_Txt_No.")->GetData() )
			{
				GetDlgItem("Btn_Confirm")->Enable(true);
				GetDlgItem("Txt_Tip")->Show(false);
			}
			else
			{
				GetDlgItem("Btn_Confirm")->Enable(false);
				GetDlgItem("Txt_Tip")->Show(true);
			}
		}
		else
		{
			if( ((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Add"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
				OnCommand_add("add");
			else if( ((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Minus"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
				OnCommand_minus("minus");
			else
				m_dwTimeClickStart = 0;
		}
	}
	else if( m_szName == "Win_InputMoney" || m_szName == "Win_InputMoney2" )
	{
		if( ((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Add1"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
			OnCommand_add("add1");
		else if( ((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Minus1"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
			OnCommand_minus("minus1");
		else if( ((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Add2"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
			OnCommand_add("add2");
		else if( ((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Minus2"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
			OnCommand_minus("minus2");
		else if( m_szName == "Win_InputMoney" )
		{
			if( ((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Add3"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
				OnCommand_add("add3");
			else if( ((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Minus3"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
				OnCommand_minus("minus3");
			else
				m_dwTimeClickStart = 0;
		}
		else
			m_dwTimeClickStart = 0;
		if( GetFocus() != GetDlgItem("DEFAULT_Txt_Money1") && a_atoi(GetDlgItem("DEFAULT_Txt_Money1")->GetText()) == 0 )
			GetDlgItem("DEFAULT_Txt_Money1")->SetText(_AL("0"));
		if( GetFocus() != GetDlgItem("DEFAULT_Txt_Money2") && a_atoi(GetDlgItem("DEFAULT_Txt_Money2")->GetText()) == 0 )
			GetDlgItem("DEFAULT_Txt_Money2")->SetText(_AL("0"));
		if( m_szName == "Win_InputMoney" )
		{
			if( GetFocus() != GetDlgItem("DEFAULT_Txt_Money3") && a_atoi(GetDlgItem("DEFAULT_Txt_Money3")->GetText()) == 0 )
				GetDlgItem("DEFAULT_Txt_Money3")->SetText(_AL("0"));
		}
	}
}

void CDlgInputNO::OnEventKeyDown_Cancel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( m_szName == "Win_Message15" )
	{
		if ( wParam == VK_RETURN )
		{
			OnCommand_CANCEL("");
		}
	}
}

void CDlgInputNO::OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUIEDITBOX pEdit;
	if( m_szName == "Win_InputNO" || m_szName == "Win_InputGNO" )
		pEdit = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_No.");
	else if( m_szName == "Win_InputMoney" || m_szName == "Win_InputMoney2" )
	{
		if( strstr(pObj->GetName(), "Btn_Add") )
		{
			int i = atoi(pObj->GetName() + strlen("Btn_Add"));
			char szName[20];
			sprintf(szName, "DEFAULT_Txt_Money%d", i);
			pEdit = (PAUIEDITBOX)GetDlgItem(szName);
		}
		else
		{
			int i = atoi(pObj->GetName() + strlen("Btn_Minus"));
			char szName[20];
			sprintf(szName, "DEFAULT_Txt_Money%d", i);
			pEdit = (PAUIEDITBOX)GetDlgItem(szName);
		}
	}
	ChangeFocus(pEdit);
}

void CDlgInputNO::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( a_atoi(pObj->GetText()) == 0 )
		pObj->SetText(_AL(""));
}

void CDlgInputNO::OnCommand_max(const char * szCommand)
{
	ACHAR szText[40];
	int nMax;

	if( m_szName == "Win_InputNO" || m_szName == "Win_InputGNO" )
	{
		nMax = (int)GetDlgItem("DEFAULT_Txt_No.")->GetData();
		a_sprintf(szText, _AL("%d"), nMax);
		GetDlgItem("DEFAULT_Txt_No.")->SetText(szText);
	}
	else if( m_szName == "Win_InputMoney" || m_szName == "Win_InputMoney2" )
	{
		nMax = (int)GetDlgItem("DEFAULT_Txt_Money1")->GetData();
		int nMoney;
		nMoney = nMax / 10000;
		a_sprintf(szText, _AL("%d"), nMoney);
		GetDlgItem("DEFAULT_Txt_Money1")->SetText(szText);
		nMoney = (nMax / 100) % 100;
		a_sprintf(szText, _AL("%d"), nMoney);
		GetDlgItem("DEFAULT_Txt_Money2")->SetText(szText);
		if( m_szName == "Win_InputMoney" )
		{
			nMoney = nMax % 100;
			a_sprintf(szText, _AL("%d"), nMoney);
			GetDlgItem("DEFAULT_Txt_Money3")->SetText(szText);
		}
	}
}

void CDlgInputNO::OnCommand_add(const char * szCommand)
{
	if( m_dwTimeClickStart == 0 )
		m_dwTimeClickStart = GetTickCount();
	else if( GetTickCount() - m_dwTimeClickStart < 500 )
		return;

	ACHAR szText[40];
	PAUIEDITBOX pEdit;
	if( m_szName == "Win_InputNO" || m_szName == "Win_InputGNO" )
	{
		pEdit = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_No.");
		int nMax = (int)pEdit->GetData();
		int nNum = a_atoi(pEdit->GetText());

		if( nNum < nMax ) nNum++;
		a_sprintf(szText, _AL("%d"), nNum);
		pEdit->SetText(szText);
	}
	else if( m_szName == "Win_InputMoney" || m_szName == "Win_InputMoney2" )
	{
		int nMax = (int)GetDlgItem("DEFAULT_Txt_Money1")->GetData();
		int i = atoi(szCommand + strlen("add"));
		char szName[20];
		sprintf(szName, "DEFAULT_Txt_Money%d", i);
		pEdit = (PAUIEDITBOX)GetDlgItem(szName);
		int nMoney = a_atoi(pEdit->GetText());
		nMoney++;
		if( i == 1 )
			a_ClampRoof(nMoney, 20000);
		else
			nMoney = nMoney % 100;
		a_sprintf(szText, _AL("%d"), nMoney);
		pEdit->SetText(szText);
		int nNum;
		if( m_szName == "Win_InputMoney" )
		{
			nNum = a_atoi(GetDlgItem("DEFAULT_Txt_Money1")->GetText()) * 10000
				+ a_atoi(GetDlgItem("DEFAULT_Txt_Money2")->GetText()) * 100
				+ a_atoi(GetDlgItem("DEFAULT_Txt_Money3")->GetText());
		}
		else
		{
			nNum = a_atoi(GetDlgItem("DEFAULT_Txt_Money1")->GetText()) * 10000
				+ a_atoi(GetDlgItem("DEFAULT_Txt_Money2")->GetText()) * 100;
		}
		if( nNum > nMax )
			OnCommand_max("");
	}
}

void CDlgInputNO::OnCommand_minus(const char * szCommand)
{
	if( m_dwTimeClickStart == 0 )
		m_dwTimeClickStart = GetTickCount();
	else if( GetTickCount() - m_dwTimeClickStart < 500 )
		return;

	ACHAR szText[40];
	PAUIEDITBOX pEdit;
	if( m_szName == "Win_InputNO" || m_szName == "Win_InputGNO" )
	{
		pEdit = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_No.");
		int nMax = (int)pEdit->GetData();
		int nNum = a_atoi(pEdit->GetText());

		if( nNum > 1 ) nNum--;
		a_sprintf(szText, _AL("%d"), nNum);
		pEdit->SetText(szText);
	}
	else if( m_szName == "Win_InputMoney" || m_szName == "Win_InputMoney2" )
	{
		int i = atoi(szCommand + strlen("minus"));
		char szName[20];
		sprintf(szName, "DEFAULT_Txt_Money%d", i);
		pEdit = (PAUIEDITBOX)GetDlgItem(szName);
		int nMoney = a_atoi(pEdit->GetText());
		nMoney--;
		if( i == 1 )
			a_ClampFloor(nMoney, 0);
		else
			nMoney = (nMoney + 100) % 100;
		a_sprintf(szText, _AL("%d"), nMoney);
		pEdit->SetText(szText);
	}
}


void CDlgInputNO::OnCommand_drop(const char * szCommand)
{
	Show(false);
	GetGameSession()->c2s_CmdThrowMoney(GetData());
}

void CDlgInputNO::OnCommand_confirm(const char * szCommand)
{
	int nInputNO = m_nInputNO;
	int nMax, nNum;
	if( m_szName == "Win_InputNO" || m_szName == "Win_InputGNO" )
	{
		nMax = (int)GetDlgItem("DEFAULT_Txt_No.")->GetData();
		nNum = a_atoi(GetDlgItem("DEFAULT_Txt_No.")->GetText());
	}
	else if( m_szName == "Win_InputMoney" || m_szName == "Win_InputMoney2" )
	{
		nMax = (int)GetDlgItem("DEFAULT_Txt_Money1")->GetData();
		if( m_szName == "Win_InputMoney" )
		{
			nNum = a_atoi(GetDlgItem("DEFAULT_Txt_Money1")->GetText()) * 10000
					+ a_atoi(GetDlgItem("DEFAULT_Txt_Money2")->GetText()) * 100
					+ a_atoi(GetDlgItem("DEFAULT_Txt_Money3")->GetText());
		}
		else
		{
			nNum = a_atoi(GetDlgItem("DEFAULT_Txt_Money1")->GetText()) * 10000
					+ a_atoi(GetDlgItem("DEFAULT_Txt_Money2")->GetText()) * 100;
		}
	}
	if( m_szName == "Win_InputGNO" && nNum > nMax ) return;
	if( nNum < 0 ) return;
	if( nInputNO != INPUTNO_ATTACH_MONEY && nNum == 0 ) return;
	a_ClampRoof(nNum, nMax);

	CECInventory *pPack = GetHostPlayer()->GetPack();
	CECDealInventory *pBuy = GetHostPlayer()->GetBuyPack();
	CECDealInventory *pSell = GetHostPlayer()->GetSellPack();
	CECDealInventory *pDeal = GetHostPlayer()->GetDealPack();
	CECDealInventory *pBoothBuy = GetHostPlayer()->GetEPBoothBuyPack();
	CECDealInventory *pBoothSell = GetHostPlayer()->GetEPBoothSellPack();
	CECReputationInventory *pShopReputation;
	
	if( nInputNO == INPUTNO_TRADE_MONEY )
	{
		int idTrade = GetGameUIMan()->m_idTrade;
		int nTradeMoney = GetGameUIMan()->m_pDlgTrade->m_pTxt_Money1I->GetData();

		if( nNum > nTradeMoney )
			GetGameSession()->trade_AddGoods(idTrade, 0, 0, 0, nNum - nTradeMoney);
		else if( nNum < nTradeMoney )
			GetGameSession()->trade_RemoveGoods(idTrade, 0, 0, 0, nTradeMoney - nNum);
	}
	else if( nInputNO == INPUTNO_TRADE_MOVE )
	{
		int idTrade = GetGameUIMan()->m_idTrade;
		PAUIOBJECT pObjSrc = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		CECIvtrItem *pItem = (CECIvtrItem *)pObjSrc->GetDataPtr("ptr_CECIvtrItem");
		int idItem = pItem->GetTemplateID();
		int iSrc = atoi(pObjSrc->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventory->GetFirstItem();
		PAUIOBJECT pObjDst = (PAUIOBJECT)GetData("ptr_AUIObject");
		int iDst;
		if( pObjDst )
			iDst = atoi(pObjDst->GetName() + strlen("Item_")) + GetGameUIMan()->m_pDlgInventory->GetFirstItem();
		else
		{
			CECInventory *pPack = GetHostPlayer()->GetPack();
			iDst = pPack->SearchEmpty();
		}
		
		GetGameSession()->trade_MoveItem(idTrade, idItem, iSrc, iDst, nNum);
	}
	else if( nInputNO == INPUTNO_TRADE_ADD )
	{
		int idTrade = GetGameUIMan()->m_idTrade;
		PAUIOBJECT pObj = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
		int idItem = pItem->GetTemplateID();
		int iSrc = atoi(pObj->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventory->GetFirstItem();

		GetGameSession()->trade_AddGoods(idTrade, idItem, iSrc, nNum, 0);
		pItem->Freeze(true);
	}
	else if( nInputNO == INPUTNO_BUY_ADD )
	{
		int iSrc, iOrigin;
		PAUIOBJECT pObj = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");

		if( m_pAUIManager->GetDialog("Win_Shop")->IsShow() )
		{
			PAUIDIALOG pShop = m_pAUIManager->GetDialog("Win_Shop");
			int nCurShopSet = GetGameUIMan()->m_pDlgShop->GetCurShopSet();

			iSrc = atoi(pObj->GetName() + strlen("U_")) + (nCurShopSet - 1) * IVTRSIZE_NPCPACK - 1;
			pBuy->AddItemFree(pItem, iSrc, nNum);
		}
		else
		{
			iSrc = atoi(pObj->GetName() + strlen("SellItem_")) - 1;
			iOrigin = pBoothSell->GetItemInfo(iSrc).iOrigin;
			pBuy->AddBoothItem(pItem, iOrigin, nNum, pItem->GetUnitPrice());
			pItem->Freeze(true);
		}
	}
	else if( nInputNO == INPUTNO_BUY_REMOVE )
	{
		PAUIOBJECT pObj = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
		int iSrc = atoi(pObj->GetName() + strlen("Buy_")) - 1;

		pBuy->RemoveItemByIndex(iSrc, nNum);
	}
	else if( nInputNO == INPUTNO_SELL_ADD )
	{
		int iSrc, iDst, iOrigin, nIndex;
		PAUIOBJECT pObjSrc = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		CECIvtrItem *pIvtrSrc = (CECIvtrItem *)pObjSrc->GetDataPtr("ptr_CECIvtrItem");

		if( m_pAUIManager->GetDialog("Win_Shop")->IsShow() )
		{
			iSrc = atoi(pObjSrc->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventory->GetFirstItem();
			pSell->AddItem(pIvtrSrc, iSrc, nNum);
			pIvtrSrc->Freeze(true);
		}
		else
		{
			PAUIOBJECT pObjDst = (PAUIOBJECT)GetData("ptr_AUIObject");
			CECIvtrItem *pIvtrDst = (CECIvtrItem *)pObjDst->GetDataPtr("ptr_CECIvtrItem");

			iSrc = atoi(pObjSrc->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventory->GetFirstItem();
			iDst = atoi(pObjDst->GetName() + strlen("BuyItem_")) - 1;
			iOrigin = pBoothBuy->GetItemInfo(iDst).iOrigin;
			nIndex = pSell->AddBoothItem(pIvtrSrc, iOrigin, nNum, pIvtrDst->GetUnitPrice());
			pSell->SetItemInfoFlag(nIndex, iSrc);
			pIvtrSrc->Freeze(true);
			pIvtrDst->Freeze(true);
		}
	}
	else if( nInputNO == INPUTNO_SELL_REMOVE )
	{
		PAUIOBJECT pObj = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
		int iSrc = atoi(pObj->GetName() + strlen("Sell_")) - 1;
		const CECDealInventory::ITEMINFO &ii = pSell->GetItemInfo(iSrc);

		if( ii.iAmount == nNum ) pItem->Freeze(false);
		pSell->RemoveItemByIndex(iSrc, nNum);
	}
	else if( nInputNO == INPUTNO_DROP_ITEM )
	{
		PAUIOBJECT pObj = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		int iSrc = atoi(pObj->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventory->GetFirstItem();

		GetGameSession()->c2s_CmdDropIvtrItem(iSrc, nNum);
	}
	else if( nInputNO == INPUTNO_DROP_MONEY )
	{
		ACString strText;
		ACHAR szText[256];
		CECIvtrItem::GetPriceText(szText, nNum);
		CDlgInputNO *pMsgBox = (CDlgInputNO*)GetGameUIMan()->GetDialog("Win_Message15");
		pMsgBox->SetData(nNum);
		pMsgBox->Show(true);
	}
	else if( nInputNO == INPUTNO_MOVE_ITEM )
	{
		PAUIOBJECT pObjSrc = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		int iSrc = atoi(pObjSrc->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventory->GetFirstItem();
		PAUIOBJECT pObjDst = (PAUIOBJECT)GetData("ptr_AUIObject");
		int iDst;
		if( pObjDst )
			iDst = atoi(pObjDst->GetName() + strlen("Item_")) + GetGameUIMan()->m_pDlgInventory->GetFirstItem();
		else
		{
			CECInventory *pPack = GetHostPlayer()->GetPack();
			iDst = pPack->SearchEmpty();
		}

		if( iDst != -1 )
			GetGameSession()->c2s_CmdMoveIvtrItem(iSrc, iDst, nNum);
	}
	else if( nInputNO == INPUTNO_STORAGE_TRASH_MONEY )
	{
		GetGameSession()->c2s_CmdExgTrashBoxMoney(nNum, 0);
	}
	else if( nInputNO == INPUTNO_STORAGE_IVTR_MONEY )
	{
		GetGameSession()->c2s_CmdExgTrashBoxMoney(0, nNum);
	}
	else if( nInputNO == INPUTNO_STORAGE_GET_ITEMS )
	{
		PAUIOBJECT pObjSrc = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		int iSrc = atoi(pObjSrc->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgStorage->GetFirstItem();
		PAUIOBJECT pObjDst = (PAUIOBJECT)GetData("ptr_AUIObject");
		int iDst = atoi(pObjDst->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventory->GetFirstItem();

		GetGameSession()->c2s_CmdMoveTrashBoxToIvtr(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), iSrc, iDst, nNum);
	}
	else if( nInputNO == INPUTNO_STORAGE_PUT_ITEMS )
	{
		PAUIOBJECT pObjSrc = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		int iSrc = atoi(pObjSrc->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventory->GetFirstItem();
		PAUIOBJECT pObjDst = (PAUIOBJECT)GetData("ptr_AUIObject");
		int iDst = atoi(pObjDst->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgStorage->GetFirstItem();

		GetGameSession()->c2s_CmdMoveIvtrToTrashBox(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), iDst, iSrc, nNum);
	}
	else if( nInputNO == INPUTNO_STORAGE_MOVE_ITEMS )
	{
		PAUIOBJECT pObjSrc = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		int iSrc = atoi(pObjSrc->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgStorage->GetFirstItem();
		PAUIOBJECT pObjDst = (PAUIOBJECT)GetData("ptr_AUIObject");
		int iDst = atoi(pObjDst->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgStorage->GetFirstItem();

		GetGameSession()->c2s_CmdMoveTrashBoxItem(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), iSrc, iDst, nNum);
	}
	else if( nInputNO == INPUTNO_PETINVO_GET_ITEMS )
	{
		PAUIOBJECT pObjSrc = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		int iSrc = atoi(pObjSrc->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventoryPocket->GetFirstItem();
		PAUIOBJECT pObjDst = (PAUIOBJECT)GetData("ptr_AUIObject");
		int iDst = atoi(pObjDst->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventory->GetFirstItem();

		GetGameSession()->c2s_CmdMovePocketToIvtr(iSrc, iDst, nNum);
	}
	else if( nInputNO == INPUTNO_PETINVO_PUT_ITEMS )
	{
		PAUIOBJECT pObjSrc = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		int iSrc = atoi(pObjSrc->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventory->GetFirstItem();
		PAUIOBJECT pObjDst = (PAUIOBJECT)GetData("ptr_AUIObject");
		int iDst = atoi(pObjDst->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventoryPocket->GetFirstItem();

		GetGameSession()->c2s_CmdMoveIvtrToPocket(iDst, iSrc, nNum);
	}
	else if( nInputNO == INPUTNO_PETINVO_MOVE_ITEMS )
	{
		PAUIOBJECT pObjSrc = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		int iSrc = atoi(pObjSrc->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventoryPocket->GetFirstItem();
		PAUIOBJECT pObjDst = (PAUIOBJECT)GetData("ptr_AUIObject");
		int iDst;
		if( pObjDst )
			iDst = atoi(pObjDst->GetName() + strlen("Item_")) + GetGameUIMan()->m_pDlgInventoryPocket->GetFirstItem();
		else
		{
			CECInventory *pPack = GetHostPlayer()->GetPocketPack();
			iDst = pPack->SearchEmpty();
		}

		if( iDst != -1 )
			GetGameSession()->c2s_CmdMovePocketItem(iSrc, iDst, nNum);
	}
	else if( nInputNO == INPUTNO_MOVE_MAILATTACH )
	{
		PAUIOBJECT pObjSrc = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		CECIvtrItem *pItem = (CECIvtrItem *)pObjSrc->GetDataPtr("ptr_CECIvtrItem");
		pItem->Freeze(true);
		int iSrc = atoi(pObjSrc->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventory->GetFirstItem();
		
		((CDlgMailWrite *)GetGameUIMan()->GetDialog("Win_MailWrite"))->SetAttachItem(
			pItem, nNum, iSrc);
	}
	else if( nInputNO == INPUTNO_ATTACH_MONEY )
	{
		PAUILABEL pLabel = (PAUILABEL)GetGameUIMan()->GetDialog("Win_MailWrite")->GetDlgItem("Txt_Money1");
		pLabel->SetData(nNum);
	}
	else if( nInputNO == INPUTNO_GOLDTRADE_MONEY )
	{
		PAUILABEL pLabel = (PAUILABEL)GetGameUIMan()->GetDialog("Win_GoldTrade")->GetDlgItem("Txt_Money11");
		pLabel->SetData(nNum);
	}
	else if( nInputNO == INPUTNO_GOLDACCOUNT_MONEY )
	{
		PAUILABEL pLabel = (PAUILABEL)GetGameUIMan()->GetDialog("Win_GoldAccount")->GetDlgItem("Txt_Money1");
		pLabel->SetData(nNum);
	}
	else if( nInputNO == INPUTNO_QSHOPITEM_BUY )
	{
		int yuanbaoCnt = GetData() * nNum;
		PAUIOBJECT pObj = GetGameUIMan()->m_pDlgInputGNO->GetDlgItem("Txt_Confirm");

		if(yuanbaoCnt < 100000)	// 单位为银元宝
			GetGameSession()->c2s_SendCmdMallShopping2((int)pObj->GetDataPtr(), (int)pObj->GetData(), 0, nNum);
		else	// 弹出确认消息框
		{
			GetGameUIMan()->MessageBox("Game_1000yuanbao", GetStringFromTable(15130), MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
			return;// do not call Show(false);
		}
	}
	else if( nInputNO == INPUTNO_QSHOPITEM_BONUSBUY )
	{
		PAUIOBJECT pObj = GetGameUIMan()->m_pDlgInputGNO->GetDlgItem("Txt_Confirm");
		GetGameSession()->c2s_CmdBonusMallShopping((int)pObj->GetDataPtr(), (int)pObj->GetData(), 0, nNum);
	}
	else if( nInputNO == INPUTNO_QSHOPITEM_CSSBUY )
	{
		PAUIOBJECT pObj = GetGameUIMan()->m_pDlgInputGNO->GetDlgItem("Txt_Confirm");
		GetGameSession()->c2s_CmdZoneMallShopping((int)pObj->GetDataPtr(), (int)pObj->GetData(), 0, nNum);
	}
	else if( nInputNO == INPUTNO_GARDENQSHOPITEM_BUY )
	{
		htc2s_SendCmdBuyItem(GetData("ptr_AUIObject"), nNum);
	}
	else if( nInputNO == INPUTNO_GARDENQSHOPITEM_SELL )
	{
		htc2s_SendCmdSellItem((int)GetDataPtr("ptr_AUIObject"), GetData("ptr_AUIObject"), nNum);
	}
	else if( nInputNO == INPUTNO_COSIGN )
	{
		PAUIOBJECT pObjSrc = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		int iSrc = atoi(pObjSrc->GetName() + strlen("Item_"))
					+ GetGameUIMan()->m_pDlgInventory->GetFirstItem();
		GetGameUIMan()->CommonMessage("Win_ShopConsign", "set_item", iSrc, nNum);
	}
	else if( nInputNO == INPUTNO_LITTLEPET)
	{
		int tid = (int)GetData();
		if(tid != 0)
		{
			GetGameSession()->c2s_CmdFeedLittlePet(tid , nNum);
		}
	}
	else if(nInputNO == INPUTNO_REPUTATION_BUY_ADD)
	{
		int iSrc;
		PAUIOBJECT pObj = (PAUIOBJECT)GetDataPtr("ptr_AUIObject");
		CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");

		if(m_pAUIManager->GetDialog("Win_Shop_Credit")->IsShow())
		{
			int nCurShopSet = GetGameUIMan()->m_pDlgShopReputation->GetCurShopSet();
			unsigned int requ_id[3], requ_consume[3];
			pShopReputation = GetHostPlayer()->GetReputationPack(nCurShopSet);
			
			iSrc = atoi(pObj->GetName() + strlen("U_")) - 1;
			for(int i = 0; i < 3; i++)
			{
				requ_id[i] = pShopReputation->goods[iSrc].repu_required_pre[i].repu_id_consume;
				requ_consume[i] = pShopReputation->goods[iSrc].repu_required_pre[i].repu_consume_value;
			}

			iSrc = atoi(pObj->GetName() + strlen("U_")) + nCurShopSet * IVTRSIZE_NPCPACK - 1;
			pBuy->AddItemFree(pItem, iSrc, nNum, requ_id, requ_consume);
		}
	}	
	else if(INPUTNO_GUILD_SHOP == nInputNO)
	{
		nNum = a_atoi(GetDlgItem("DEFAULT_Txt_No.")->GetText());
		nMax = (int)GetDlgItem("DEFAULT_Txt_No.")->GetData();
		int nItemCost = (int)GetDlgItem("DEFAULT_Txt_No.")->GetDataPtr();
		if(nNum > nMax)
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(20249), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
		else if(nNum > nItemCost)
		{
			c2s_SendCmdBigOnFacbase(GetData(),(int)GetDataPtr(),nNum,GetHostPlayer()->GetName());
		}
		else
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(20245), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}

	}
	Show(false);
}

void CDlgInputNO::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgInputNO::SetType(int nType)
{
	m_nInputNO = nType;
}
