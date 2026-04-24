// Filename	: DlgShopBuyBack.cpp
// Creator	: Fu Chonggang
// Date		: 2012.7.31

#include "DlgShopBuyBack.h"
#include "DlgSkill.h"
#include "AFI.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_IvtrRune.h"
#include "EC_SendC2SCmds.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgShopBuyBack, CDlgBase)

AUI_ON_COMMAND("Btn_Withdraw",		OnCommandBuyBack)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgShopBuyBack, CDlgBase)

AUI_ON_EVENT("Img_Sell*",			WM_LBUTTONDOWN,		OnEventLButtonDownSell)
AUI_ON_EVENT("Img_Sell*",			WM_LBUTTONDBLCLK,	OnEventLButtonDownSell)
AUI_ON_EVENT("Img_Sell*",			WM_RBUTTONDOWN,		OnEventLButtonDownSell)
AUI_ON_EVENT("Img_Withdraw*",		WM_LBUTTONDOWN,		OnEventLButtonDownBuy)
AUI_ON_EVENT("Img_Withdraw*",		WM_LBUTTONDBLCLK,	OnEventLButtonDownBuy)
AUI_ON_EVENT("Img_Withdraw*",		WM_RBUTTONDOWN,		OnEventLButtonDownBuy)

AUI_END_EVENT_MAP()

CDlgShopBuyBack::CDlgShopBuyBack()
{
}

CDlgShopBuyBack::~CDlgShopBuyBack()
{
}

bool CDlgShopBuyBack::OnInitDialog()
{
	m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
	m_pTxt_Money2 = (PAUILABEL)GetDlgItem("Txt_Money2");
	m_pTxt_Money3 = (PAUILABEL)GetDlgItem("Txt_Money3");
	m_pTxt_Money1->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money2->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money3->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_BuyMoney1 = (PAUILABEL)GetDlgItem("Txt_WithdrawMoney1");
	m_pTxt_BuyMoney2 = (PAUILABEL)GetDlgItem("Txt_WithdrawMoney2");
	m_pTxt_BuyMoney3 = (PAUILABEL)GetDlgItem("Txt_WithdrawMoney3");
	m_pTxt_BuyMoney1->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_BuyMoney2->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_BuyMoney3->SetAlign(AUIFRAME_ALIGN_RIGHT);
	
	int i;
	AString strName;
	for( i = 0; i < IVTRSIZE_SELLBACKPACK; i++ )
	{
		strName.Format("Img_Sell%02d", i + 1);
		m_pImgInv[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
	}
	
	for( i = 0; i < IVTRSIZE_SELLBACKPACK; i++ )
	{
		strName.Format("Img_Withdraw%02d", i + 1);
		m_pImgBuy[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
	}

	m_pInvBuyBack = new CECInventory();
	if (!m_pInvBuyBack->Init(IVTRSIZE_SELLBACKPACK))
	{
		AUI_ReportError(__LINE__, __FILE__);
		return false;
	}
		
	return true;
}

void CDlgShopBuyBack::OnShowDialog()
{
	c2s_SendCmdGetRePurchaseInv();
	Reset();
}

bool CDlgShopBuyBack::Release()
{
	A3DRELEASE(m_pInvBuyBack);
	return CDlgBase::Release();
}

void CDlgShopBuyBack::OnTick()
{
	UpdateInventory();
}

void CDlgShopBuyBack::OnCommandBuyBack(const char * szCommand)
{
	GetHostPlayer()->BuyItemsBackFromNPC();
	Reset();
}

void CDlgShopBuyBack::Reset()
{
	GetHostPlayer()->GetSellBackPack()->UnfreezeAllItems();
	int i;
	for (i=0; i<IVTRSIZE_SELLBACKPACK; ++i)
	{
		m_pInvBuyBack->GetItem(i, true);
	}
}

void CDlgShopBuyBack::OnEventLButtonDownSell(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	if (!pItem)
		return;

	if (pItem->IsFrozen())
	{
		return;
	}

	pItem->Freeze(true);
	int iIndex = m_pInvBuyBack->SearchEmpty();
	m_pInvBuyBack->PutItem(iIndex, pItem);
}

void CDlgShopBuyBack::OnEventLButtonDownBuy(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	if (!pItem)
		return;

	if (!pItem->IsFrozen())
	{
		return;
	}
	pItem->Freeze(false);
	int i;
	for (i=0; i<IVTRSIZE_SELLBACKPACK; ++i)
	{
		if (m_pInvBuyBack->GetItem(i)==pItem)
		{
			m_pInvBuyBack->GetItem(i, true);
			break;
		}
	}
}

void CDlgShopBuyBack::UpdateInventory()
{
	int i;
	AString strFile;
	ACHAR szText[40];
	CECIvtrItem *pItem;
	PAUIIMAGEPICTURE pObj;
	INT64 nMoney = GetHostPlayer()->GetMoneyAmount();
	CECInventory *pInventory = GetHostPlayer()->GetSellBackPack();

	for( i = 0; i < IVTRSIZE_SELLBACKPACK; i++ )
	{
		pObj = m_pImgInv[i];

		if( i < pInventory->GetSize() )
		{
			pObj->Show(true);
			pObj->SetCover(NULL, -1, 1);

			pItem = pInventory->GetItem(i);
			if( pItem )
			{
				pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");
				if( pItem->GetCount() > 1 )
				{
					a_sprintf(szText, _AL("%d"), pItem->GetCount());
					pObj->SetText(szText);
				}
				else
					pObj->SetText(_AL(""));

				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				pObj->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
					GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY, strFile));
				pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
				
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
				pObj->SetDataPtr(NULL);
				pObj->SetColor(A3DCOLORRGB(255, 255, 255));
			}
		}
		else
			pObj->Show(false);
	}

	ACString strMoney;
	GetGameUIMan()->Int64ToStr(nMoney % 100, strMoney);
	m_pTxt_Money3->SetText(strMoney);
	GetGameUIMan()->Int64ToStr((nMoney % 10000) / 100, strMoney);
	m_pTxt_Money2->SetText(strMoney);
	GetGameUIMan()->Int64ToStr(nMoney / 10000, strMoney);
	m_pTxt_Money1->SetText(strMoney);

	
	nMoney = 0;
	int iNumBuyBack = 0;
	for( i = 0; i < IVTRSIZE_SELLBACKPACK; i++ )
	{
		pObj = m_pImgBuy[iNumBuyBack];
		if( !pObj ) continue;
		
		pItem = m_pInvBuyBack->GetItem(i);
		if( pItem)
		{
			pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");
			pObj->SetColor(A3DCOLORRGB(255, 255, 255));
			
			if( pItem->GetCount() > 1 )
			{
				a_sprintf(szText, _AL("%d"), pItem->GetCount());
				pObj->SetText(szText);
			}
			else
				pObj->SetText(_AL(""));
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			pObj->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
				GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY, strFile));
			
			nMoney += pItem->GetUnitPrice()*pItem->GetCount();
			iNumBuyBack++;
		}
		else
		{
			pObj->SetCover(NULL, -1);
			pObj->SetText(_AL(""));
			pObj->SetDataPtr(NULL);
		}
	}
	for (i=iNumBuyBack; i<IVTRSIZE_BUYPACK; ++i)
	{
		m_pImgBuy[i]->SetCover(NULL, -1);
		m_pImgBuy[i]->SetText(_AL(""));
		m_pImgBuy[i]->SetDataPtr(NULL);
	}
	
	GetGameUIMan()->Int64ToStr(nMoney % 100, strMoney);
	m_pTxt_BuyMoney3->SetText(strMoney);
	GetGameUIMan()->Int64ToStr((nMoney % 10000) / 100, strMoney);
	m_pTxt_BuyMoney2->SetText(strMoney);
	GetGameUIMan()->Int64ToStr(nMoney / 10000, strMoney);
	m_pTxt_BuyMoney1->SetText(strMoney);
}