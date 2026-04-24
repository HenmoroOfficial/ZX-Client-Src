// Filename	: DlgBooth.cpp
// Creator	: Tom Zhou
// Date		: October 11, 2005

#include "AFI.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIEditbox.h"
#include "AUI\\AUICombobox.h"
#include "DlgBooth.h"
#include "DlgInputNO.h"
#include "DlgChat.h"
#include "DlgSkill.h"
#include "DlgItemLock.h"
#include "DlgMinimizeBar.h"

#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_DealInventory.h"
#include "EC_IvtrItem.h"
#include "EC_Resource.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_IvtrRune.h"

#define new A_DEBUG_NEW

extern const int MAX_AUCTION_PRICE = 2000000000;

AUI_BEGIN_COMMAND_MAP(CDlgBooth, CDlgBase)

AUI_ON_COMMAND("max",			OnCommand_max)
AUI_ON_COMMAND("minus",			OnCommand_minus)
AUI_ON_COMMAND("add",			OnCommand_add)
AUI_ON_COMMAND("buy",			OnCommand_buy)
AUI_ON_COMMAND("sell",			OnCommand_sell)
AUI_ON_COMMAND("reset",			OnCommand_reset)
AUI_ON_COMMAND("confirm",		OnCommand_confirm)
AUI_ON_COMMAND("sendmsg",		OnCommand_sendmsg)
AUI_ON_COMMAND("clearmsg",		OnCommand_clearmsg)
AUI_ON_COMMAND("Btn_Minimize",	OnCommand_Minimize)
AUI_ON_COMMAND("Btn_Close1",	OnCommand_CANCEL)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBooth, CDlgBase)

AUI_ON_EVENT("Txt_MsgList",		WM_MOUSEMOVE,		OnEventMouseMove_MsgList)
AUI_ON_EVENT("Txt_MsgList",		WM_LBUTTONDOWN,		OnEventLButtonDown_MsgList)
AUI_ON_EVENT("Txt_MsgList",		WM_LBUTTONUP,		OnEventLButtonUp_MsgList)
AUI_ON_EVENT("Txt_MsgList",		WM_RBUTTONDOWN,		OnEventRButtonDown_MsgList)
AUI_ON_EVENT("SellItem_*",		WM_LBUTTONUP,		OnEventLButtonUp_SellItem)
AUI_ON_EVENT("SellItem_*",		WM_RBUTTONUP,		OnEventLButtonUp_SellItem)
AUI_ON_EVENT("BuyItem_*",		WM_LBUTTONUP,		OnEventLButtonUp_BuyItem)
AUI_ON_EVENT("BuyItem_*",		WM_RBUTTONUP,		OnEventLButtonUp_BuyItem)
AUI_ON_EVENT("*",				WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("SellItem_*",		WM_LBUTTONDBLCLK,	OnEventLButtonDblclk_SellItem)
AUI_ON_EVENT("Sell_*",			WM_LBUTTONDBLCLK,	OnEventLButtonDblclk_Sell)
AUI_ON_EVENT("Buy_*",			WM_LBUTTONDBLCLK,	OnEventLButtonDblclk_Buy)

AUI_END_EVENT_MAP()

CDlgBooth::CDlgBooth()
{
}

CDlgBooth::~CDlgBooth()
{
}

bool CDlgBooth::OnInitDialog()
{
	if( m_szName == "Win_PShop1" || m_szName == "Win_PShop2" )
	{
		m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
		m_pTxt_Money2 = (PAUILABEL)GetDlgItem("Txt_Money2");
		m_pTxt_Money3 = (PAUILABEL)GetDlgItem("Txt_Money3");
		m_pTxt_Money1->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Money2->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Money3->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_BuyMoney1 = (PAUILABEL)GetDlgItem("Txt_BuyMoney1");
		m_pTxt_BuyMoney2 = (PAUILABEL)GetDlgItem("Txt_BuyMoney2");
		m_pTxt_BuyMoney3 = (PAUILABEL)GetDlgItem("Txt_BuyMoney3");
		m_pTxt_BuyMoney1->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_BuyMoney2->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_BuyMoney3->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_SellMoney1 = (PAUILABEL)GetDlgItem("Txt_SellMoney1");
		m_pTxt_SellMoney2 = (PAUILABEL)GetDlgItem("Txt_SellMoney2");
		m_pTxt_SellMoney3 = (PAUILABEL)GetDlgItem("Txt_SellMoney3");
		m_pTxt_SellMoney1->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_SellMoney2->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_SellMoney3->SetAlign(AUIFRAME_ALIGN_RIGHT);
	}
	if( stricmp(m_szName, "Win_PShop1") == 0 )
		((PAUITEXTAREA)GetDlgItem("Txt_MsgList"))->SetMaxLines(200);

	return true;
}

void CDlgBooth::OnShowDialog()
{
	PAUIDIALOG pDlgPocket = GetGameUIMan()->GetDialog("Win_InventoryPet");
	if (pDlgPocket->IsShow())
	{
		pDlgPocket->Show(false);
	}
	pDlgPocket = GetGameUIMan()->GetDialog("Win_Inventory_Costume");
	if (pDlgPocket->IsShow())
	{
		pDlgPocket->Show(false);
	}
	
	PAUIDIALOG pInventoryExpand = GetGameUIMan()->GetDialog("Win_Inventory_Expand01");
	if(pInventoryExpand && pInventoryExpand->IsShow())
		pInventoryExpand->Show(false);
	
	pInventoryExpand = GetGameUIMan()->GetDialog("Win_Inventory_Expand02");
	if(pInventoryExpand && pInventoryExpand->IsShow())
		pInventoryExpand->Show(false);
}

void CDlgBooth::OnCommand_max(const char * szCommand)
{
	ACHAR szText[20];
	CECIvtrItem *pItem = (CECIvtrItem *)GetDataPtr("ptr_CECIvtrItem");
	if( !pItem ) return;

	if( GetData() == CDlgInputNO::INPUTNO_BUY_ADD )
		a_sprintf(szText, _AL("%d"), pItem->GetPileLimit());
	else
		a_sprintf(szText, _AL("%d"), pItem->GetCount());
	GetDlgItem("Txt_Amount")->SetText(szText);
}

void CDlgBooth::OnCommand_add(const char * szCommand)
{
	ACHAR szText[20];
	CECIvtrItem *pItem = (CECIvtrItem *)GetDataPtr("ptr_CECIvtrItem");
	if( !pItem ) return;

	PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Txt_Amount");
	int nAmount = a_atoi(pEdit->GetText());
	int nMax;
	if( GetData() == CDlgInputNO::INPUTNO_BUY_ADD )
		nMax = pItem->GetPileLimit();
	else
		nMax = pItem->GetCount();
	if( nAmount < nMax )
	{
		a_sprintf(szText, _AL("%d"), nAmount + 1);
		pEdit->SetText(szText);
	}
}

void CDlgBooth::OnCommand_minus(const char * szCommand)
{
	ACHAR szText[20];
	CECIvtrItem *pItem = (CECIvtrItem *)GetDataPtr("ptr_CECIvtrItem");
	if( !pItem ) return;

	PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Txt_Amount");
	int nAmount = a_atoi(pEdit->GetText());
	if( nAmount > 1 )
	{
		a_sprintf(szText, _AL("%d"), nAmount - 1);
		pEdit->SetText(szText);
	}
}

void CDlgBooth::OnCommand_buy(const char * szCommand)
{
	int nGold = GetHostPlayer()->GetMoneyAmount();
	int nCost = GetDlgItem("Txt_BuyMoney1")->GetData();

	if( nGold >= nCost )
		GetHostPlayer()->BuyItemsFromBooth();
	else
	{
		PAUIDIALOG pMsgBox;
		m_pAUIManager->MessageBox("", GetStringFromTable(222), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetLife(3);
	}
}

void CDlgBooth::OnCommand_sell(const char * szCommand)
{
	m_pAUIManager->MessageBox("Game_PSellOut", GetStringFromTable(243),
		MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
}

void CDlgBooth::OnCommand_reset(const char * szCommand)
{
	GetHostPlayer()->GetBoothBuyPack()->RemoveAllItems();
	GetHostPlayer()->GetBoothSellPack()->RemoveAllItems();
	GetGameUIMan()->UnfreezeAllItems();
}

void CDlgBooth::OnCommand_confirm(const char * szCommand)
{
	if( 0 == stricmp(GetName(), "Win_PShop1") )
	{
		ACString strName = GetDlgItem("DEFAULT_Txt_Name")->GetText();
		GetGameUIMan()->FilterBadWords(strName);
		GetDlgItem("DEFAULT_Txt_Name")->SetText(strName);
		GetHostPlayer()->OpenBooth(strName);
		GetDlgItem("Btn_Cancel")->Enable(false);
	}
	else if( 0 == stricmp(GetName(), "Win_InputNO2") )
	{
		PAUIDIALOG pMsgBox;
		CECIvtrItem *pItem = (CECIvtrItem *)GetDataPtr("ptr_CECIvtrItem");
		if( !pItem ) return;
		int iOrigin = (int)GetDlgItem("Txt_Money1")->GetData();
		int nPrice = a_atoi(GetDlgItem("Txt_Money1")->GetText()) * 10000
			+ a_atoi(GetDlgItem("Txt_Money2")->GetText()) * 100
			+ a_atoi(GetDlgItem("Txt_Money3")->GetText());
		int nCount = a_atoi(GetDlgItem("Txt_Amount")->GetText());
		__int64 nTotal = __int64(nPrice) * __int64(nCount);
		CECDealInventory *pBoothSell = GetHostPlayer()->GetBoothSellPack();
		CECDealInventory *pBoothBuy = GetHostPlayer()->GetBoothBuyPack();

		if( nPrice > MAX_AUCTION_PRICE || nTotal > MAX_AUCTION_PRICE )
		{
			m_pAUIManager->MessageBox("", GetStringFromTable(562), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			pMsgBox->SetLife(3);
			return;
		}
		if( nPrice < 1 || nCount < 1 )
		{
			m_pAUIManager->MessageBox("", GetStringFromTable(563), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			pMsgBox->SetLife(3);
			return;
		}
		if( (GetData() == CDlgInputNO::INPUTNO_SELL_ADD && nCount > pItem->GetCount()) ||
			(GetData() == CDlgInputNO::INPUTNO_BUY_ADD && nCount > pItem->GetPileLimit()) )
		{
			m_pAUIManager->MessageBox("", GetStringFromTable(564), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			pMsgBox->SetLife(3);
			return;
		}

		Show(false);
		if( GetData() == CDlgInputNO::INPUTNO_BUY_ADD )
		{
			a_Clamp(nCount, 1, pItem->GetPileLimit());
			pBoothBuy->AddBoothItem(pItem, iOrigin, nCount, nPrice);
		}
		else
		{
			a_Clamp(nCount, 1, pItem->GetCount());
			pBoothSell->AddBoothItem(pItem, iOrigin, nCount, nPrice);
		}
		pItem->Freeze(true);
	}
}

void CDlgBooth::OnCommand_sendmsg(const char * szCommand)
{
	PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Txt_Msg");
	if( !pEdit ) return;
	const ACHAR *szText = pEdit->GetText();
	CECGameSession *pSession = GetGameSession();
	const ACHAR *pszName = GetGameRun()->GetPlayerName(GetData("int"), true);
	if( !pszName ) return;

	ACString strTextOut;

	strTextOut.Format(_AL("%s!#"),szText);

	pSession->SendPrivateChatData(pszName, strTextOut);
	pEdit->SetText(_AL(""));
	ChangeFocus(NULL);
}

void CDlgBooth::OnCommand_clearmsg(const char * szCommand)
{
	PAUITEXTAREA pMsgList = (PAUITEXTAREA)GetDlgItem("Txt_MsgList");
	if( !pMsgList ) return;
	pMsgList->SetText(_AL(""));
}

void CDlgBooth::OnCommand_Minimize(const char * szCommand)
{
	GetGameUIMan()->m_pDlgMinimizeBar[0]->MinimizeDialog(this, 
		GetGameUIMan()->GetStringFromTable(3559), CDlgMinimizeBar::MINIMIZEBAR_TYPE_BOOTH);
}

void CDlgBooth::OnCommand_CANCEL(const char * szCommand)
{
	if( 0 == stricmp(GetName(), "Win_PShop1") )
	{
		if( GetDlgItem("DEFAULT_Txt_Name")->IsEnabled() )
		{
			OnCommand_clearmsg("clearmsg");
			PAUITEXTAREA pMsgList = (PAUITEXTAREA)GetDlgItem("Txt_MsgList");
			if( pMsgList )
				pMsgList->SetText(_AL(""));
			GetHostPlayer()->SetBoothState(0);
			m_pAUIManager->GetDialog("Win_Inventory")->Show(false);
			GetGameUIMan()->UnfreezeAllItems();
			Show(false);
		}
		else
			GetGameSession()->c2s_CmdCloseBooth();
	}
	else if( 0 == stricmp(GetName(), "Win_PShop2") )
	{
		Show(false);
		CECDealInventory *pBoothSell = GetHostPlayer()->GetEPBoothSellPack();
		CECDealInventory *pBoothBuy = GetHostPlayer()->GetEPBoothBuyPack();

		GetGameUIMan()->EndNPCService();
		pBoothBuy->UnfreezeAllItems();
		pBoothSell->UnfreezeAllItems();
		GetGameUIMan()->UnfreezeAllItems();
		m_pAUIManager->GetDialog("Win_Inventory")->Show(false);
	}
	else
		Show(false);
}

bool CDlgBooth::Tick(void)
{
	if( m_szName == "Win_PShop1" )
	{
		UpdateBooth();
		UpdateBooth1();
	}
	if( m_szName == "Win_PShop2" )
	{
		UpdateBooth();
		UpdateBooth2();
	}

	return CDlgBase::Tick();
}

bool CDlgBooth::UpdateBooth()
{
	int i;
	AString strFile;
	char szItem[40];
	ACHAR szText[40];
	CECIvtrItem *pItem;
	AUICTranslate trans;
	PAUIIMAGEPICTURE pObj;
	CECDealInventory *pDeal;
	__int64 nMoney1 = 0, nMoney2 = 0;
	PAUIDIALOG pDlgMouseOn;
	PAUIOBJECT pObjMouseOn;

	m_pAUIManager->GetMouseOn(&pDlgMouseOn, &pObjMouseOn);

	if( m_szName == "Win_PShop1" )
		pDeal = GetHostPlayer()->GetBoothSellPack();
	else
		pDeal = GetHostPlayer()->GetEPBoothSellPack();
	for( i = 0; i < pDeal->GetSize(); i++ )
	{
		sprintf(szItem, "SellItem_%02d", i + 1);
		pObj = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
		if( !pObj ) continue;

		pItem = pDeal->GetItem(i);
		pObj->SetColor(A3DCOLORRGB(255, 255, 255));
		pObj->SetCover(NULL, -1, 1);
		if( pItem )
		{
			pObj->SetDataPtr(pItem, "ptr_CECIvtrItem");

			const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(i);
			if( ii.iAmount > 1 )
			{
				a_sprintf(szText, _AL("%d"), ii.iAmount);
				pObj->SetText(szText);
			}
			else
				pObj->SetText(_AL(""));
			if( pDlgMouseOn && pDlgMouseOn == this &&
				pObjMouseOn && pObjMouseOn == pObj )
			{
				const wchar_t* szDesc = pItem->GetDesc();
				if( szDesc )
					pObj->SetHint(trans.Translate(szDesc));
				else
					pObj->SetHint(pItem->GetName());
			}
			pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
			
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
			nMoney1 += (__int64)pItem->GetUnitPrice() * (__int64)ii.iAmount;
			
			if (pItem->IsLocked())
			{
				pObj->SetCover(CDlgItemLock::m_pA2DSpriteLock, 0, 1);
			}
			
			if( ( pItem->GetClassID()==CECIvtrItem::ICID_EQUIP && ((CECIvtrEquip*)pItem)->GetEssence().num_estone>=9 ) ||
				( pItem->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART && ((CECIvtrTalismanMain*)pItem)->GetRebornCount()>0 ) ||
				( pItem->GetClassID()==CECIvtrItem::ICID_RUNE && ((CECIvtrRune*)pItem)->GetEssence().quality >= 3) )
			{
				if (pItem->IsLocked())
				{
					pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquipLock, (GetTickCount() / 200) % 6, 1);
				}
				else
				{
					pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquip, (GetTickCount() / 200) % 6, 1);
				}
			}
		}
		else
		{
			pObj->ClearCover();
			pObj->SetText(_AL(""));
			pObj->SetHint(_AL(""));
			pObj->SetDataPtr(NULL);
			pObj->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	}

	if( m_szName == "Win_PShop1" )
		pDeal = GetHostPlayer()->GetBoothBuyPack();
	else
		pDeal = GetHostPlayer()->GetEPBoothBuyPack();
	for( i = 0; i < pDeal->GetSize(); i++ )
	{
		sprintf(szItem, "BuyItem_%02d", i + 1);
		pObj = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
		if( !pObj ) continue;

		pItem = pDeal->GetItem(i);
		pObj->SetColor(A3DCOLORRGB(255, 255, 255));
		pObj->SetCover(NULL, -1, 1);
		if( pItem )
		{
			pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");

			const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(i);
			if( ii.iAmount > 1 )
			{
				a_sprintf(szText, _AL("%d"), ii.iAmount);
				pObj->SetText(szText);
			}
			else
				pObj->SetText(_AL(""));
			if( pDlgMouseOn && pDlgMouseOn == this &&
				pObjMouseOn && pObjMouseOn == pObj )
			{
				const wchar_t* szDesc = pItem->GetDesc(CECIvtrItem::DESC_BOOTHBUY);
				if( szDesc )
					pObj->SetHint(trans.Translate(szDesc));
				else
					pObj->SetHint(pItem->GetName());
			}
			pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
			
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
			nMoney2 += (__int64)pItem->GetUnitPrice() * (__int64)ii.iAmount;

			if (pItem->IsLocked())
			{
				pObj->SetCover(CDlgItemLock::m_pA2DSpriteLock, 0, 1);
			}
			
			if( ( pItem->GetClassID()==CECIvtrItem::ICID_EQUIP && ((CECIvtrEquip*)pItem)->GetEssence().num_estone>=9 ) ||
				( pItem->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART && ((CECIvtrTalismanMain*)pItem)->GetRebornCount()>0 ) ||
				( pItem->GetClassID()==CECIvtrItem::ICID_RUNE && ((CECIvtrRune*)pItem)->GetEssence().quality >= 3) )
			{
				if (pItem->IsLocked())
				{
					pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquipLock, (GetTickCount() / 200) % 6, 1);
				}
				else
				{
					pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquip, (GetTickCount() / 200) % 6, 1);
				}
			}
		}
		else
		{
			pObj->ClearCover();
			pObj->SetText(_AL(""));
			pObj->SetHint(_AL(""));
			pObj->SetDataPtr(NULL);
			pObj->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	}

	m_nMoney1 = nMoney1;
	m_nMoney2 = nMoney2;
	int nMoney = GetHostPlayer()->GetMoneyAmount();
	ACHAR szMoney[20];
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
	m_pTxt_Money3->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pTxt_Money2->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pTxt_Money1->SetText(szMoney);

	return true;
}

bool CDlgBooth::UpdateBooth1()
{
	if( !IsShow() ) return true;

	AUICTranslate trans;
	__int64 nMoney1 = m_nMoney1;
	__int64 nMoney2 = m_nMoney2;

	// Sold out.
	if( nMoney1 <= 0 && nMoney2 <= 0 &&
		!GetDlgItem("DEFAULT_Txt_Name")->IsEnabled() )
	{
		OnCommand_CANCEL("IDCANCEL");
		return true;
	}

	ACHAR szMoney[20];
	a_sprintf(szMoney, _AL("%d"), nMoney1 % 100);
	m_pTxt_SellMoney3->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney1 % 10000) / 100);
	m_pTxt_SellMoney2->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney1 / 10000);
	m_pTxt_SellMoney1->SetText(szMoney);
	if( nMoney1 + GetHostPlayer()->GetMoneyAmount() > MAX_AUCTION_PRICE )
	{
		m_pTxt_SellMoney1->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxt_SellMoney2->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxt_SellMoney3->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		m_pTxt_SellMoney1->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pTxt_SellMoney2->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pTxt_SellMoney3->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	a_sprintf(szMoney, _AL("%d"), nMoney2 % 100);
	m_pTxt_BuyMoney3->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney2 % 10000) / 100);
	m_pTxt_BuyMoney2->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney2 / 10000);
	m_pTxt_BuyMoney1->SetText(szMoney);
	if( nMoney2 > GetHostPlayer()->GetMoneyAmount() )
	{
		m_pTxt_BuyMoney1->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxt_BuyMoney2->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxt_BuyMoney3->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		m_pTxt_BuyMoney1->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pTxt_BuyMoney2->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pTxt_BuyMoney3->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	if( (nMoney1 > 0 || nMoney2 > 0) &&
		nMoney2 <= GetHostPlayer()->GetMoneyAmount() &&
		nMoney1 + GetHostPlayer()->GetMoneyAmount() <= MAX_AUCTION_PRICE &&
		GetDlgItem("DEFAULT_Txt_Name")->IsEnabled() &&
		a_strlen(GetDlgItem("DEFAULT_Txt_Name")->GetText()) > 0 )
	{
		GetDlgItem("Btn_Confirm")->Enable(true);
	}
	else
		GetDlgItem("Btn_Confirm")->Enable(false);

	return true;
}

bool CDlgBooth::UpdateBooth2()
{
	if( !IsShow() ) return true;

	AString strFile;
	char szItem[40];
	ACHAR szText[40];
	int i, nTotalMoney;
	CECIvtrItem *pItem;
	AUICTranslate trans;
	PAUIIMAGEPICTURE pObj;
	CECDealInventory *pDeal;
	PAUIDIALOG pDlgMouseOn;
	PAUIOBJECT pObjMouseOn;

	m_pAUIManager->GetMouseOn(&pDlgMouseOn, &pObjMouseOn);

	// Buy.
	nTotalMoney = 0;
	pDeal = GetHostPlayer()->GetBuyPack();
	for( i = 0; i < IVTRSIZE_BUYPACK; i++ )
	{
		sprintf(szItem, "Buy_%02d", i + 1);
		pObj = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
		if( !pObj ) continue;

		pObj->SetCover(NULL, -1, 1);
		pItem = pDeal->GetItem(i);
		if( pItem )
		{
			pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");

			const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(i);
			if( ii.iAmount > 1 )
			{
				a_sprintf(szText, _AL("%d"), ii.iAmount);
				pObj->SetText(szText);
			}
			else
				pObj->SetText(_AL(""));
			if( pDlgMouseOn && pDlgMouseOn == this &&
				pObjMouseOn && pObjMouseOn == pObj )
			{
				const wchar_t* szDesc = pItem->GetDesc();
				if( szDesc )
					pObj->SetHint(trans.Translate(szDesc));
				else
					pObj->SetHint(pItem->GetName());
			}

			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
			pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
			
			if (pItem->IsLocked())
			{
				pObj->SetCover(CDlgItemLock::m_pA2DSpriteLock, 0, 1);
			}
						
			if( ( pItem->GetClassID()==CECIvtrItem::ICID_EQUIP && ((CECIvtrEquip*)pItem)->GetEssence().num_estone>=9 ) ||
				( pItem->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART && ((CECIvtrTalismanMain*)pItem)->GetRebornCount()>0 ) ||
				( pItem->GetClassID()==CECIvtrItem::ICID_RUNE && ((CECIvtrRune*)pItem)->GetEssence().quality >= 3) )
			{
				if (pItem->IsLocked())
				{
					pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquipLock, (GetTickCount() / 200) % 6, 1);
				}
				else
				{
					pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquip, (GetTickCount() / 200) % 6, 1);
				}
			}

			nTotalMoney += pItem->GetUnitPrice() * ii.iAmount;
		}
		else
		{
			pObj->ClearCover();
			pObj->SetText(_AL(""));
			pObj->SetHint(_AL(""));
			pObj->SetDataPtr(NULL);
			pObj->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	}

	ACHAR szMoney[20];
	a_sprintf(szMoney, _AL("%d"), nTotalMoney % 100);
	m_pTxt_BuyMoney3->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nTotalMoney % 10000) / 100);
	m_pTxt_BuyMoney2->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nTotalMoney / 10000);
	m_pTxt_BuyMoney1->SetText(szMoney);
	m_pTxt_BuyMoney1->SetData(nTotalMoney);
	if( nTotalMoney > GetHostPlayer()->GetMoneyAmount() )
	{
		m_pTxt_BuyMoney1->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxt_BuyMoney2->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxt_BuyMoney3->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		m_pTxt_BuyMoney1->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pTxt_BuyMoney2->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pTxt_BuyMoney3->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	// Sell.
	nTotalMoney = 0;
	pDeal = GetHostPlayer()->GetSellPack();
	for( i = 0; i < IVTRSIZE_SELLPACK; i++ )
	{
		sprintf(szItem, "Sell_%02d", i + 1);
		pObj = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
		if( !pObj ) continue;

		pObj->SetCover(NULL, -1, 1);
		pItem = pDeal->GetItem(i);
		if( pItem )
		{
			pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");

			const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(i);
			if( ii.iAmount > 1 )
			{
				a_sprintf(szText, _AL("%d"), ii.iAmount);
				pObj->SetText(szText);
			}
			else
				pObj->SetText(_AL(""));
			if( pDlgMouseOn && pDlgMouseOn == this &&
				pObjMouseOn && pObjMouseOn == pObj )
			{
				const wchar_t* szDesc = pItem->GetDesc();
				if( szDesc )
					pObj->SetHint(trans.Translate(szDesc));
				else
					pObj->SetHint(pItem->GetName());
			}

			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
			pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);

			if (pItem->IsLocked())
			{
				pObj->SetCover(CDlgItemLock::m_pA2DSpriteLock, 0, 1);
			}
			
			if( ( pItem->GetClassID()==CECIvtrItem::ICID_EQUIP && ((CECIvtrEquip*)pItem)->GetEssence().num_estone>=9 ) ||
				( pItem->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART && ((CECIvtrTalismanMain*)pItem)->GetRebornCount()>0 ) ||
				( pItem->GetClassID()==CECIvtrItem::ICID_RUNE && ((CECIvtrRune*)pItem)->GetEssence().quality >= 3) )
			{
				if (pItem->IsLocked())
				{
					pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquipLock, (GetTickCount() / 200) % 6, 1);
				}
				else
				{
					pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquip, (GetTickCount() / 200) % 6, 1);
				}
			}

			nTotalMoney += pItem->GetUnitPrice() * ii.iAmount;
		}
		else
		{
			pObj->ClearCover();
			pObj->SetText(_AL(""));
			pObj->SetHint(_AL(""));
			pObj->SetDataPtr(NULL);
			pObj->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	}

	a_sprintf(szMoney, _AL("%d"), nTotalMoney % 100);
	m_pTxt_SellMoney3->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nTotalMoney % 10000) / 100);
	m_pTxt_SellMoney2->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nTotalMoney / 10000);
	m_pTxt_SellMoney1->SetText(szMoney);

	return true;
}

void CDlgBooth::OnEventLButtonUp_SellItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;
	if( 0 != stricmp(GetName(), "Win_PShop1") )
		return;
	if( !GetDlgItem("DEFAULT_Txt_Name")->IsEnabled() )
		return;

	int nIndex = atoi(pObj->GetName() + strlen("SellItem_")) - 1;
	CECDealInventory *pDeal = GetHostPlayer()->GetBoothSellPack();
	const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(nIndex);

	CECIvtrItem *pItem =  GetHostPlayer()->GetPack()->GetItem(ii.iOrigin);
	if( pItem )
		pItem->Freeze(false);
	pDeal->RemoveItemByIndex(nIndex, -1);
}

void CDlgBooth::OnEventLButtonUp_BuyItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;
	if( 0 != stricmp(GetName(), "Win_PShop1") )
		return;
	if( !GetDlgItem("DEFAULT_Txt_Name")->IsEnabled() )
		return;

	int nIndex = atoi(pObj->GetName() + strlen("BuyItem_")) - 1;
	CECDealInventory *pDeal = GetHostPlayer()->GetBoothBuyPack();
	const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(nIndex);

	CECIvtrItem *pItem =  GetHostPlayer()->GetPack()->GetItem(ii.iOrigin);
	if( pItem )
		pItem->Freeze(false);
	pDeal->RemoveItemByIndex(nIndex, -1);
}

void CDlgBooth::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	if( !pItem || pItem->IsFrozen())
		return;
	if( 0 == stricmp(GetName(), "Win_PShop1") )
		return;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT ptCapture = { GET_X_LPARAM(lParam) - p->X, GET_Y_LPARAM(lParam) - p->Y };

	GetGameUIMan()->InvokeDragDrop(this, pObj, ptCapture);
	GetGameUIMan()->PlayItemSound(pItem, false);
}

void CDlgBooth::OnEventLButtonDblclk_SellItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	if( !pItem || pItem->IsFrozen() )
		return;
	if( m_szName == "Win_PShop1" )
		return;

	if( pItem->GetPileLimit() > 1 && pItem->GetCount() > 1 )
	{
		ACHAR szText[40];
		PAUIOBJECT pEdit = GetGameUIMan()->m_pDlgInputNO->GetDlgItem("DEFAULT_Txt_No.");

		pEdit->SetData(pItem->GetCount());
		a_sprintf(szText, _AL("%d"), 1);
		pEdit->SetText(szText);

		GetGameUIMan()->m_pDlgInputNO->SetDataPtr(pObj,"ptr_AUIObject");
		GetGameUIMan()->m_pDlgInputNO->Show(true, true);
		GetGameUIMan()->m_pDlgInputNO->SetType(CDlgInputNO::INPUTNO_BUY_ADD);
	}
	else
	{
		CECDealInventory *pBuy = GetHostPlayer()->GetBuyPack();
		CECDealInventory *pSell = GetHostPlayer()->GetEPBoothSellPack();
		int iSrc = atoi(pObj->GetName() + strlen("SellItem_")) - 1;
		int iOrigin = pSell->GetItemInfo(iSrc).iOrigin;

		pBuy->AddBoothItem(pItem, iOrigin, 1, pItem->GetUnitPrice());
		pItem->Freeze(true);
	}
}

void CDlgBooth::OnEventLButtonDblclk_Sell(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( 0 == stricmp(GetName(), "Win_PShop1") )
		return;

	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	if( !pItem ) return;

	CECDealInventory *pDeal = GetHostPlayer()->GetSellPack();
	int iSrc = atoi(pObj->GetName() + strlen("Sell_")) - 1;
	const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(iSrc);
	int nIndex = GetHostPlayer()->GetEPBoothBuyPack()->GetItemIndex(ii.iOrigin);

	pDeal->RemoveItemByIndex(iSrc, pItem->GetCount());

	pItem =  GetHostPlayer()->GetPack()->GetItem(ii.iFlag);
	if( pItem )
		pItem->Freeze(false);
	pItem = GetHostPlayer()->GetEPBoothBuyPack()->GetItem(nIndex);
	if( pItem )
		pItem->Freeze(false);
}

void CDlgBooth::OnEventLButtonDblclk_Buy(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( m_szName == "Win_PShop1" )
		return;

	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	if( !pItem ) return;

	CECDealInventory *pDeal = GetHostPlayer()->GetBuyPack();
	int iSrc = atoi(pObj->GetName() + strlen("Buy_")) - 1;
	int iOrigin = pDeal->GetItemInfo(iSrc).iOrigin;
	int nIndex = GetHostPlayer()->GetEPBoothSellPack()->GetItemIndex(iOrigin);

	pDeal->RemoveItemByIndex(iSrc, pItem->GetCount());
	pItem = GetHostPlayer()->GetEPBoothSellPack()->GetItem(nIndex);
	if( pItem )
		pItem->Freeze(false);
}

bool CDlgBooth::GetNameLinkMouseOn(int x, int y,
	PAUIOBJECT pObj, P_AUITEXTAREA_NAME_LINK pLink)
{
	bool bClickedChatPart = false;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT ptPos = pObj->GetPos();
	PAUITEXTAREA pText = (PAUITEXTAREA)pObj;

	GetGame()->ChangeCursor(RES_CUR_NORMAL);
	if( pText->GetHitArea(x - ptPos.x, y - ptPos.y) == AUITEXTAREA_RECT_FRAME )
	{
		int i;
		abase::vector<AUITEXTAREA_NAME_LINK> &vecNameLink = pText->GetNameLink();

		x += p->X;
		y += p->Y;
		for( i = 0; i < (int)vecNameLink.size(); i++ )
		{
			if( vecNameLink[i].rc.PtInRect(x, y) )
			{
				GetGame()->ChangeCursor(RES_CUR_HAND);
				*pLink = vecNameLink[i];
				break;
			}
		}
		bClickedChatPart = true;
	}

	return bClickedChatPart;
}

void CDlgBooth::OnEventMouseMove_MsgList(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	AUITEXTAREA_NAME_LINK Link;
	
	GetNameLinkMouseOn(x, y, pObj, &Link);
}

void CDlgBooth::OnEventLButtonDown_MsgList(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;

	AUITEXTAREA_NAME_LINK Link;
	GetNameLinkMouseOn(x, y, pObj, &Link);
	if( Link.strName.GetLength() == 0 ) return;

	GetGameUIMan()->SwitchToPrivateChat(Link.strName,false,true);
}

void CDlgBooth::OnEventLButtonUp_MsgList(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;

	AUITEXTAREA_NAME_LINK Link;
	GetNameLinkMouseOn(x, y, pObj, &Link);
	if( Link.strName.GetLength() == 0 ) return;

	GetGameUIMan()->SwitchToPrivateChat(Link.strName,true,true);
}

void CDlgBooth::OnEventRButtonDown_MsgList(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;

	AUITEXTAREA_NAME_LINK Link;
	GetNameLinkMouseOn(x, y, pObj, &Link);
	if( Link.strName.GetLength() == 0 ) return;

	GNET::Privilege *pPrivilege = GetGame()->GetPrivilege();
	int idPlayer = GetGameRun()->GetPlayerID(Link.strName);

	if( idPlayer > 0 )
	{
		x -= p->X;
		y -= p->Y + GetGameUIMan()->GetDialog("Win_QuickAction")->GetSize().cy;
		GetGameUIMan()->PopupPlayerContextMenu(idPlayer, x, y);
	}
}

void CDlgBooth::AddBoothMessage(const ACHAR *pszMsg)
{
	if( m_szName != "Win_PShop1" )
		return;
	ACHAR *pMsg = (ACHAR*)pszMsg;
	int i = 0;
	int p1 = 0,p2 = 0;
	int j = 0;
	while( pMsg[j] != '\0')
	{
		if( pMsg[j] == '^' )
		{
			j++;
			if( pMsg[j] == '\0' )
				break;
		}
		else if( pMsg[j] == '&')
		{
			i++;
			if( i == 1 )
				p1 = j;
			if( i == 2 )
			{
				p2 = j;
				ACHAR szPlayer[100];
				a_strncpy(szPlayer, pMsg + p1 , p2 - p1 + 1);
				szPlayer[p2 - p1 + 1] = '\0';
				PAUITEXTAREA pMsgList = (PAUITEXTAREA)GetDlgItem("Txt_MsgList");
				pMsg = a_strstr(pMsg + j, _AL("£º"));
				if( pMsg != NULL)
				{
					ACString strBoothMsg;
					strBoothMsg.Format(GetStringFromTable(770), szPlayer, pMsg + 1);
					int nMsgLen = strBoothMsg.GetLength();
					strBoothMsg[nMsgLen - 2] = '\r';
					if( pMsgList )
						pMsgList->AppendText(strBoothMsg.Left(nMsgLen - 1));
				}
				break;
			}
		}
		j++;
	}
}

void CDlgBooth::TickBoothMinimize()
{
	UpdateBooth();
	if( m_nMoney1 <= 0 && m_nMoney2 <= 0 &&
		!GetDlgItem("DEFAULT_Txt_Name")->IsEnabled() )
	{
		OnCommand_CANCEL("IDCANCEL");
		GetGameUIMan()->m_pDlgMinimizeBar[0]->RestoreDialog(this);
	}
}