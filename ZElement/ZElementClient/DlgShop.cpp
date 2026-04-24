// Filename	: DlgShop.cpp
// Creator	: Tom Zhou
// Date		: October 11, 2005

#include "AFI.h"
#include "AUI\\AUIComboBox.h"
#include "DlgShop.h"
#include "DlgInventory.h"
#include "DlgInputNO.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Resource.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_NPC.h"
#include "EC_ManNPC.h"
#include "EC_NPCServer.h"
#include "EC_DealInventory.h"
#include "EC_World.h"
#include "EC_Configs.h"

#define new A_DEBUG_NEW

const int MAX_PACKET_MONEY = 2000000000;

AUI_BEGIN_COMMAND_MAP(CDlgShop, CDlgBase)

AUI_ON_COMMAND("set*",					OnCommand_set)
AUI_ON_COMMAND("buy",					OnCommand_buy)
AUI_ON_COMMAND("sell",					OnCommand_sell)
AUI_ON_COMMAND("repair",				OnCommand_repair)
AUI_ON_COMMAND("left",					OnCommand_left)
AUI_ON_COMMAND("right",					OnCommand_right)
AUI_ON_COMMAND("repairall",				OnCommand_repairall)
AUI_ON_COMMAND("Btn_AutoSellSetting",	OnCommand_AutoSellSetting)
AUI_ON_COMMAND("Btn_AutoSell",			OnCommand_AutoSell)
AUI_ON_COMMAND("Btn_Confirm",			OnCommand_Confirm)
AUI_ON_COMMAND("Btn_Withdraw",			OnCommand_BuyBack)
AUI_ON_COMMAND("Btn_Cancel",			OnCommand_CANCEL)
AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgShop, CDlgBase)

AUI_ON_EVENT("*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("*",		WM_LBUTTONDBLCLK,	OnEventLButtonDblclk)
AUI_ON_EVENT("*",		WM_RBUTTONUP,		OnEventRButtonUp)

AUI_END_EVENT_MAP()

CDlgShop::CDlgShop()
{
	m_nCurShopSet = 1;
	m_nFirstSet = 0;
}

CDlgShop::~CDlgShop()
{
}

bool CDlgShop::OnInitDialog()
{
	int i;
	char szItem[40];

	if (stricmp(m_szName, "Win_Shop")==0)
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
		
		for( i = 0; i < IVTRSIZE_NPCPACK; i++ )
		{
			sprintf(szItem, "U_%02d", i + 1);
			m_pImgU[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
		}
		
		for( i = 0; i < IVTRSIZE_BUYPACK; i++ )
		{
			sprintf(szItem, "Buy_%02d", i + 1);
			m_pImgBuy[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
		}
		
		for( i = 0; i < IVTRSIZE_SELLPACK; i++ )
		{
			sprintf(szItem, "Sell_%02d", i + 1);
			m_pImgSell[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
		}
		
		for( i = 0; i < NUM_NPCIVTR; i++ )
		{
			sprintf(szItem, "Btn_Set%d", i + 1);
			m_pBtnSet[i] = (PAUISTILLIMAGEBUTTON)GetDlgItem(szItem);
		}

		m_bIsAutoSelling = false;
	}
	else
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_1");
		for (i=0; i<10; i++)
		{
			pCombo->AddString(GetStringFromTable(9981+i));
		}
		pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_2");
		for (i=0; i<10; i++)
		{
			pCombo->AddString(GetStringFromTable(9981+i));
		}
	}

	return true;
}

void CDlgShop::OnShowDialog()
{
	if (stricmp(m_szName, "Win_Shop")==0)
	{
		PAUIDIALOG pDlgPocket = GetGameUIMan()->GetDialog("Win_InventoryPet");
		if (GetHostPlayer()->GetPocketPack()->GetSize()>0)
		{
			pDlgPocket->Show(true);
		}
		pDlgPocket = GetGameUIMan()->GetDialog("Win_Inventory_Costume");
		if (pDlgPocket->IsShow())
		{
			pDlgPocket->Show(false);
		}
	}
	else
	{
		EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
		((PAUICHECKBOX)GetDlgItem("Chk_1"))->Check(setting.auto_sell.bRarity);
		((PAUICOMBOBOX)GetDlgItem("Combo_1"))->SetCurSel(setting.auto_sell.nRarityLevel);
		((PAUICHECKBOX)GetDlgItem("Chk_2"))->Check(setting.auto_sell.bMaterial);
		((PAUICOMBOBOX)GetDlgItem("Combo_2"))->SetCurSel(setting.auto_sell.nMaterialLevel);
		((PAUICHECKBOX)GetDlgItem("Chk_3"))->Check(setting.auto_sell.bAffuse);
		((PAUICHECKBOX)GetDlgItem("Chk_4"))->Check(setting.auto_sell.bArrangeAfterSell);
	}
}

void CDlgShop::OnCommand_set(const char * szCommand)
{
	UpdateShop(atoi(szCommand + strlen("set")) + m_nFirstSet);
}

void CDlgShop::OnCommand_left(const char * szCommand)
{
	if( m_nFirstSet > 0 )
	{
		m_nFirstSet--;
		UpdateSetButton();
	}
}

void CDlgShop::OnCommand_right(const char * szCommand)
{
	CECNPCInventory *pDeal = GetHostPlayer()->GetNPCSevPack(m_nFirstSet + 4);
	if( a_strlen(pDeal->GetName()) > 0 )
	{
		m_nFirstSet++;
		UpdateSetButton();
	}
}

void CDlgShop::OnCommand_buy(const char * szCommand)
{
	int nGold = GetHostPlayer()->GetMoneyAmount();
	int nCost = m_pTxt_BuyMoney1->GetData();
	if( nGold >= nCost )
	{
		GetHostPlayer()->BuyItemsFromNPC();
	}
	else
	{
		PAUIDIALOG pMsgBox;
		m_pAUIManager->MessageBox("", m_pAUIManager->GetStringFromTable(222),
			MB_OK, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetLife(3);
	}
}

void CDlgShop::OnCommand_sell(const char * szCommand)
{
	if (m_pTxt_SellMoney1->GetColor() == A3DCOLORRGB(255, 0, 0))
	{
		m_pAUIManager->MessageBox("", m_pAUIManager->GetStringFromTable(8485),
		MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if(GetHostPlayer()->GetSellPack()->SearchEmpty()==0)
		return;
	m_pAUIManager->MessageBox("Game_SellOut", m_pAUIManager->GetStringFromTable(243),
		MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
}

void CDlgShop::OnCommand_repair(const char * szCommand)
{
	GetGameUIMan()->m_nCursorState = CECGameUIMan::CURSOR_STATE_REPAIRING;
	GetGame()->ChangeCursor(RES_CUR_REPAIR);
}

void CDlgShop::OnCommand_repairall(const char * szCommand)
{
	CECInventory *pPackEquip = GetHostPlayer()->GetPack(IVTRTYPE_EQUIPPACK);

	if( pPackEquip->HasDisrepairItem() )
	{
		ACString strText;

		strText.Format(m_pAUIManager->GetStringFromTable(242),
			max(pPackEquip->GetRepairAllItemCost(), 1));
		m_pAUIManager->MessageBox("Game_RepairAll", strText, MB_OKCANCEL,
			A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
	{
		m_pAUIManager->MessageBox("", m_pAUIManager->GetStringFromTable(252),
			MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
}

void CDlgShop::OnCommand_AutoSellSetting(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Shop_AutoSell");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgShop::OnCommand_AutoSell(const char * szCommand)
{
	if (CDlgInventory::GetIsArranging())
		return;
	EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
	if (!setting.auto_sell.bRarity && !setting.auto_sell.bMaterial && !setting.auto_sell.bAffuse)
	{
		GetGameUIMan()->MessageBox("", m_pAUIManager->GetStringFromTable(9170),
			MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	CECDealInventory *pSell = GetHostPlayer()->GetSellPack();
	pSell->RemoveAllItems();
	CECInventory *pInventory = GetHostPlayer()->GetPack();
	int nIvtrSize = pInventory->GetSize();
	int i;
	int nItems = 0;
	for (i=0; i<nIvtrSize; i++)
	{
		CECIvtrItem *pItem = pInventory->GetItem(i);
		if ( pItem)
		{
			pItem->Freeze(true);
		}
	}
	for (i=0; i<nIvtrSize; i++)
	{
		CECIvtrItem *pItem = pInventory->GetItem(i);
		if ( !pItem || !pItem->IsSellable() || pItem->IsLocked() || GetGame()->IsValuableItem(pItem->GetTemplateID()))
		{
			continue;
		}
		if( pItem->GetStateFlag() & ITEM_STATE_BINDED )
			continue;
		if (pItem->IsEquipment() && ((CECIvtrEquip*)pItem)->IsTalismanMain() && 
				((CECIvtrTalismanMain*)pItem)->GetEssence().ess.master_id != 0)
			continue;

		if (setting.auto_sell.bRarity)
		{
			int level = GetGame()->GetAutoPickItemLevel(0, pItem->GetTemplateID());
			if ( level>0 && level <= (setting.auto_sell.nRarityLevel+1) )
			{
				nItems++;
				pSell->RemoveAllItems();
				pSell->AddItem(pItem, i, pItem->GetCount());
				if (!SellItems())
				{
					nItems--;
					break;
				}
			}
		}
		if (setting.auto_sell.bMaterial)
		{
			int level = GetGame()->GetAutoPickItemLevel(1, pItem->GetTemplateID());
			if ( level>0 && level <= (setting.auto_sell.nMaterialLevel+1) )
			{
				nItems++;
				pSell->RemoveAllItems();
				pSell->AddItem(pItem, i, pItem->GetCount());
				if (!SellItems())
				{
					nItems--;
					break;
				}
			}
		}
		if (setting.auto_sell.bAffuse)
		{
			int level = GetGame()->GetAutoPickItemLevel(3, pItem->GetTemplateID());
			if ( level>0 )
			{
				nItems++;
				pSell->RemoveAllItems();
				pSell->AddItem(pItem, i, pItem->GetCount());
				if (!SellItems())
				{
					nItems--;
					break;
				}
			}
		}
	}
	GetGameUIMan()->UnfreezeAllItems();
	if (nItems>0)
	{
		m_bIsAutoSelling = true;
	}
}

void CDlgShop::OnCommand_Confirm(const char * szCommand)
{
	EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
	setting.auto_sell.bRarity = ((PAUICHECKBOX)GetDlgItem("Chk_1"))->IsChecked();
	setting.auto_sell.nRarityLevel = ((PAUICOMBOBOX)GetDlgItem("Combo_1"))->GetCurSel();
	setting.auto_sell.bMaterial = ((PAUICHECKBOX)GetDlgItem("Chk_2"))->IsChecked();
	setting.auto_sell.nMaterialLevel = ((PAUICOMBOBOX)GetDlgItem("Combo_2"))->GetCurSel();
	setting.auto_sell.bAffuse = ((PAUICHECKBOX)GetDlgItem("Chk_3"))->IsChecked();
	setting.auto_sell.bArrangeAfterSell = ((PAUICHECKBOX)GetDlgItem("Chk_4"))->IsChecked();
	GetGame()->GetConfigs()->SetLocalSettings(setting);
	Show(false);
}

void CDlgShop::OnCommand_BuyBack(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Shop_Withdraw");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgShop::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	if (stricmp(m_szName, "Win_Shop")==0)
	{
		GetGameUIMan()->EndNPCService();
		GetGameUIMan()->m_nCursorState = CECGameUIMan::CURSOR_STATE_NORMAL;
		GetGameUIMan()->m_pDlgInventory->Show(false);
		GetGameUIMan()->GetDialog("Win_InventoryPet")->Show(false);
		GetGameUIMan()->GetDialog("Win_Shop_Withdraw")->Show(false);
		if (!CDlgInventory::GetIsArranging())
		{
			GetGameUIMan()->UnfreezeAllItems();
		}
	}
}

bool CDlgShop::Tick(void)
{
	if (stricmp(m_szName, "Win_Shop")==0)
	{
		UpdateShop();
		if (m_bIsAutoSelling)
		{
			EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
			CECInventory *pInventory = GetHostPlayer()->GetPack();
			int nIvtrSize = pInventory->GetSize();
			bool bOk = true;
			for (int i=0; i<nIvtrSize; i++)
			{
				CECIvtrItem *pItem = pInventory->GetItem(i);
				if ( !pItem || !pItem->IsSellable() || pItem->IsLocked() || GetGame()->IsValuableItem(pItem->GetTemplateID()))
				{
					continue;
				}
				if( pItem->GetStateFlag() & ITEM_STATE_BINDED )
					continue;
				if (pItem->IsEquipment() && ((CECIvtrEquip*)pItem)->IsTalismanMain() && 
					((CECIvtrTalismanMain*)pItem)->GetEssence().ess.master_id != 0)
					continue;
				
				if (setting.auto_sell.bRarity)
				{
					int level = GetGame()->GetAutoPickItemLevel(0, pItem->GetTemplateID());
					if ( level>0 && level <= (setting.auto_sell.nRarityLevel+1) )
					{
						bOk = false;
					}
				}
				if (setting.auto_sell.bMaterial)
				{
					int level = GetGame()->GetAutoPickItemLevel(1, pItem->GetTemplateID());
					if ( level>0 && level <= (setting.auto_sell.nMaterialLevel+1) )
					{
						bOk = false;
					}
				}
				if (setting.auto_sell.bAffuse)
				{
					int level = GetGame()->GetAutoPickItemLevel(3, pItem->GetTemplateID());
					if ( level>0 )
					{
						bOk = false;
					}
				}
			}
			if (bOk)
			{
				m_bIsAutoSelling = false;
				EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
				if (setting.auto_sell.bArrangeAfterSell)
					GetGameUIMan()->m_pDlgInventory->OnCommand_AutoArrangeItems("");
			}

		}
	}

	return CDlgBase::Tick();
}

void CDlgShop::UpdateSetButton()
{
	int nTotalSets = 0;
	CECNPCInventory *pDeal;
	PAUISTILLIMAGEBUTTON pButton;
	int nMaxSets = 0;
	int i;
	for( i = 0; i < NUM_NPCIVTR; i++ )
	{
		pDeal = GetHostPlayer()->GetNPCSevPack(i);
		if( a_strlen(pDeal->GetName()) <= 0 ) continue;
		nMaxSets++;
	}
	for( i = 0; i < NUM_NPCIVTR - m_nFirstSet; i++ )
	{
		pDeal = GetHostPlayer()->GetNPCSevPack(m_nFirstSet + i);
		if( a_strlen(pDeal->GetName()) <= 0 ) continue;

		pButton = m_pBtnSet[nTotalSets];
		if( !pButton )
			break;
		pButton->SetText(pDeal->GetName());
		pButton->SetData((DWORD)m_nFirstSet + i);
		if( m_nCurShopSet == m_nFirstSet + i + 1 )
			pButton->SetPushed(true);
		else
			pButton->SetPushed(false);
		pButton->Show(true);

		nTotalSets++;
	}
	for( i = nTotalSets; i < NUM_NPCIVTR; i++ )
		if( m_pBtnSet[i] )
			m_pBtnSet[i]->Show(false);
	GetDlgItem("Btn_LeftTurn")->Show(m_nFirstSet > 0);
	GetDlgItem("Btn_RightTurn")->Show(m_nFirstSet + 4 < nMaxSets);
}

bool CDlgShop::UpdateShop(int nSet)
{
	int i;
	AString strFile;
	ACHAR szText[40];
	CECIvtrItem *pItem;
	PAUIIMAGEPICTURE pObj;

	if( nSet > 0 )		// NPC
	{
		CECNPCInventory *pDeal;
		PAUISTILLIMAGEBUTTON pButton;

		if( nSet == 1 )		// build buttons when nSet is 1.
		{
			m_nFirstSet = 0;
			UpdateSetButton();
		}

		if( m_nCurShopSet - m_nFirstSet - 1 >= 0 &&
			m_nCurShopSet - m_nFirstSet - 1 < NUM_NPCIVTR )
		{
			pButton = m_pBtnSet[m_nCurShopSet - m_nFirstSet - 1];
			if( pButton )
				pButton->SetPushed(false);
		}
		
		m_nCurShopSet = nSet;
		
		if( m_nCurShopSet - m_nFirstSet - 1 >= 0 &&
			m_nCurShopSet - m_nFirstSet - 1 < NUM_NPCIVTR )
		{
			pButton = m_pBtnSet[m_nCurShopSet - m_nFirstSet - 1];
			if( pButton )
				pButton->SetPushed(true);
		}

		pDeal = GetHostPlayer()->GetNPCSevPack(pButton->GetData());
		for( i = 0; i < IVTRSIZE_NPCPACK; i++ )
		{
			pObj = m_pImgU[i];
			if( !pObj ) continue;

			pItem = pDeal->GetItem(i);
			if( pItem )
			{
				pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);

				pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
			}
			else
			{
				pObj->SetHint(_AL(""));
				pObj->SetCover(NULL, -1);
				pObj->SetDataPtr(NULL);
				pObj->SetColor(A3DCOLORRGB(255, 255, 255));
			}
			pObj->SetHint(_AL(""));
		}
	}
	else				// Player
	{
		INT64 nMoney = 0;
		int nPacketMoney = GetHostPlayer()->GetMoneyAmount();
		ACString strMoney;
		strMoney.Format(_AL("%d"), nPacketMoney % 100);
		m_pTxt_Money3->SetText(strMoney);
		strMoney.Format(_AL("%d"), (nPacketMoney % 10000) / 100);
		m_pTxt_Money2->SetText(strMoney);
		strMoney.Format(_AL("%d"), nPacketMoney / 10000);
		m_pTxt_Money1->SetText(strMoney);

		INT64 nTotalMoney;
		CECDealInventory *pDeal = GetHostPlayer()->GetDealPack();

		int idServiceNPC = GetHostPlayer()->GetCurServiceNPC();
		CECNPC *pNPC = GetWorld()->GetNPCMan()->GetNPC(idServiceNPC);
		CECNPCServer *pServer = (CECNPCServer *)pNPC;

		// Buy.
		nTotalMoney = 0;
		pDeal = GetHostPlayer()->GetBuyPack();
		for( i = 0; i < IVTRSIZE_BUYPACK; i++ )
		{
			pObj = m_pImgBuy[i];
			if( !pObj ) continue;

			pItem = pDeal->GetItem(i);
			if( pItem )
			{
				pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");
				pObj->SetColor(A3DCOLORRGB(255, 255, 255));

				const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(i);
				if( ii.iAmount > 1 )
				{
					a_sprintf(szText, _AL("%d"), ii.iAmount);
					pObj->SetText(szText);
				}
				else
					pObj->SetText(_AL(""));
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);

				nTotalMoney += CECIvtrItem::GetScaledPrice(pItem->GetShopPrice(), ii.iAmount, 
					CECIvtrItem::SCALE_BUY, pItem->GetPriceScale());
			}
			else
			{
				pObj->SetHint(_AL(""));
				pObj->SetCover(NULL, -1);
				pObj->SetText(_AL(""));
				pObj->SetDataPtr(NULL);
			}
		}

		nMoney = nTotalMoney;
		GetGameUIMan()->Int64ToStr(nMoney % 100, strMoney);
		m_pTxt_BuyMoney3->SetText(strMoney);
		GetGameUIMan()->Int64ToStr((nMoney % 10000) / 100, strMoney);
		m_pTxt_BuyMoney2->SetText(strMoney);
		GetGameUIMan()->Int64ToStr(nMoney / 10000, strMoney);
		m_pTxt_BuyMoney1->SetText(strMoney);
//		m_pTxt_BuyMoney1->SetData64(nTotalMoney);

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
			pObj = m_pImgSell[i];
			if( !pObj ) continue;

			pItem = pDeal->GetItem(i);
			if( pItem )
			{
				pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");
				pObj->SetColor(A3DCOLORRGB(255, 255, 255));

				const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(i);
				if( ii.iAmount > 1 )
				{
					a_sprintf(szText, _AL("%d"), ii.iAmount);
					pObj->SetText(szText);
				}
				else
					pObj->SetText(_AL(""));
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);

				//	We must conside endurance for equipments like weapon, armor,
				//	decoration
				if (pItem->IsEquipment())
					nTotalMoney += pItem->GetScaledPrice();
				else
				{
					nTotalMoney += CECIvtrItem::GetScaledPrice(pItem->GetUnitPrice(), 
						ii.iAmount, CECIvtrItem::SCALE_SELL, PLAYER_PRICE_SCALE);
				}
			}
			else
			{
				pObj->SetCover(NULL, -1);
				pObj->SetText(_AL(""));
				pObj->SetDataPtr(NULL);
			}
		}

		nMoney = nTotalMoney;
		GetGameUIMan()->Int64ToStr(nMoney % 100, strMoney);
		m_pTxt_SellMoney3->SetText(strMoney);
		GetGameUIMan()->Int64ToStr((nMoney % 10000) / 100, strMoney);
		m_pTxt_SellMoney2->SetText(strMoney);
		GetGameUIMan()->Int64ToStr(nMoney / 10000, strMoney);
		m_pTxt_SellMoney1->SetText(strMoney);
//		m_pTxt_SellMoney1->SetData64(nMoney);

		if( nMoney + nPacketMoney > MAX_PACKET_MONEY )
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
	}

	return true;
}

int CDlgShop::GetCurShopSet()
{
	return m_nCurShopSet;
}

void CDlgShop::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT ptCapture = { GET_X_LPARAM(lParam) - p->X, GET_Y_LPARAM(lParam) - p->Y };

	GetGameUIMan()->InvokeDragDrop(this, pObj, ptCapture);
	GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
}

void CDlgShop::OnEventLButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;

	ACHAR szText[40];
	CECHostPlayer *pHost = GetHostPlayer();
	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");

	if( pItem->GetPileLimit() > 1 )
	{
		int nInputNOMode = 0;
		CDlgInputNO *pShow = GetGameUIMan()->m_pDlgInputNO;
		PAUIOBJECT pEdit = pShow->GetDlgItem("DEFAULT_Txt_No.");

		pShow->Show(true, true);
		pShow->SetDataPtr(pObj,"ptr_AUIObject");
		pShow->SetData(GetData());

		if( strstr(pObj->GetName(), "U_") )
		{
			pEdit->SetData(pItem->GetPileLimit());
			a_sprintf(szText, _AL("%d"), 1);
			nInputNOMode = CDlgInputNO::INPUTNO_BUY_ADD;
		}
		else if( strstr(pObj->GetName(), "Buy_") )
		{
			CECDealInventory *pDeal = pHost->GetBuyPack();
			int iSrc = atoi(pObj->GetName() + strlen("Buy_")) - 1;
			const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(iSrc);

			pEdit->SetData(ii.iAmount);
			a_sprintf(szText, _AL("%d"), ii.iAmount);
			nInputNOMode = CDlgInputNO::INPUTNO_BUY_REMOVE;
		}
		else if( strstr(pObj->GetName(), "Sell_") )
		{
			CECDealInventory *pDeal = pHost->GetSellPack();
			int iSrc = atoi(pObj->GetName() + strlen("Sell_")) - 1;
			const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(iSrc);

			pEdit->SetData(ii.iAmount);
			a_sprintf(szText, _AL("%d"), ii.iAmount);
			nInputNOMode = CDlgInputNO::INPUTNO_SELL_REMOVE;
		}

		pEdit->SetText(szText);
		pShow->SetType(nInputNOMode);
	}
	else
	{
		if( strstr(pObj->GetName(), "U_") )
		{
			int iSrc;
			CECDealInventory *pDeal = pHost->GetBuyPack();
			iSrc = atoi(pObj->GetName() + strlen("U_")) + (m_nCurShopSet - 1) * IVTRSIZE_NPCPACK - 1;

			pDeal->AddItemFree(pItem, iSrc, pItem->GetPileLimit());
		}
		else if( strstr(pObj->GetName(), "Buy_") )
		{
			CECDealInventory *pDeal = pHost->GetBuyPack();
			int iSrc = atoi(pObj->GetName() + strlen("Buy_")) - 1;

			const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(iSrc);
			pDeal->RemoveItemByIndex(iSrc, ii.iAmount);
		}
		else if( strstr(pObj->GetName(), "Sell_") )
		{
			CECDealInventory *pDeal = pHost->GetSellPack();
			int iSrc = atoi(pObj->GetName() + strlen("Sell_")) - 1;

			pDeal->RemoveItemByIndex(iSrc, pItem->GetCount());
			pItem->Freeze(false);
		}
	}
}

void CDlgShop::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;

	CECHostPlayer *pHost = GetHostPlayer();
	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");

	if( strstr(pObj->GetName(), "U_") )
	{
		int iSrc;
		CECDealInventory *pDeal = pHost->GetBuyPack();
		iSrc = atoi(pObj->GetName() + strlen("U_")) + (m_nCurShopSet - 1) * IVTRSIZE_NPCPACK - 1;

		pDeal->AddItemFree(pItem, iSrc, pItem->GetPileLimit());
	}
	else if( strstr(pObj->GetName(), "Buy_") )
	{
		CECDealInventory *pDeal = pHost->GetBuyPack();
		int iSrc = atoi(pObj->GetName() + strlen("Buy_")) - 1;

		const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(iSrc);
		pDeal->RemoveItemByIndex(iSrc, ii.iAmount);
	}
	else if( strstr(pObj->GetName(), "Sell_") )
	{
		CECDealInventory *pDeal = pHost->GetSellPack();
		int iSrc = atoi(pObj->GetName() + strlen("Sell_")) - 1;

		pDeal->RemoveItemByIndex(iSrc, pItem->GetCount());
		pItem->Freeze(false);
	}
}

bool CDlgShop::SellItems()
{
	if (m_pTxt_SellMoney1->GetColor() == A3DCOLORRGB(255, 0, 0))
	{
		m_pAUIManager->MessageBox("", m_pAUIManager->GetStringFromTable(8485),
			MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return false;
	}

	GetHostPlayer()->SellItemsToNPC();
	return true;
}
