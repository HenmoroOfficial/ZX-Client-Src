// Filename	: DlgShopExchange.cpp
// Creator	: Xiao Zhou
// Date		: 2008/7/3

#include "AFI.h"
#include "AUI\\AUICTranslate.h"
#include "DlgShopExchange.h"
#include "DlgInventory.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_HostPlayer.h"
#include "elementdataman.h"
#include "EC_GPDataType.h"
#include "EC_GameSession.h"
#include "LuaEvent.h"

#define new A_DEBUG_NEW

int CDlgShopExchange::m_nShopIndex = 0;
int CDlgShopExchange::m_nShopSet = 0;
int CDlgShopExchange::m_nCurSel = 0;

AUI_BEGIN_COMMAND_MAP(CDlgShopExchange, CDlgBase)

AUI_ON_COMMAND("set*",			OnCommand_set)
AUI_ON_COMMAND("exchange",		OnCommand_exchange)
AUI_ON_COMMAND("max",			OnCommand_max)
AUI_ON_COMMAND("minus",			OnCommand_minus)
AUI_ON_COMMAND("add",			OnCommand_add)
AUI_ON_COMMAND("confirm",		OnCommand_confirm)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgShopExchange, CDlgBase)

AUI_ON_EVENT("U_*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("U_*",		WM_LBUTTONDBLCLK,	OnEventRButtonUp)
AUI_ON_EVENT("U_*",		WM_RBUTTONUP,		OnEventRButtonUp)

AUI_END_EVENT_MAP()

CDlgShopExchange::CDlgShopExchange()
{
}

CDlgShopExchange::~CDlgShopExchange()
{
}

bool CDlgShopExchange::OnInitDialog()
{
	int i;
	char szItem[40];

	for( i = 0; i < CDLGSHOPEXCHANGE_ITEM_MAX; i++ )
	{
		sprintf(szItem, "U_%02d", i + 1);
		m_pImgU[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}

	for( i = 0; i < CDLGSHOPEXCHANGE_PAGE_MAX; i++ )
	{
		sprintf(szItem, "Btn_Set%d", i + 1);
		m_pBtnSet[i] = (PAUISTILLIMAGEBUTTON)GetDlgItem(szItem);
	}

	for( i = 0; i < 2; i++ )
	{
		sprintf(szItem, "Img_ItemReq%d", i + 1);
		m_pImg_ItemReq[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
		sprintf(szItem, "Txt_ItemReq%d", i + 1);
		m_pTxt_ItemReq[i] = (PAUILABEL)GetDlgItem(szItem);
		sprintf(szItem, "Txt_CreditReq%d", i + 1);
		m_pTxt_CreditReq[i] = (PAUILABEL)GetDlgItem(szItem);
	}
	m_pTxt_SpecialReq = (PAUILABEL)GetDlgItem("Txt_ItemReqBattle");

	m_pImg_Item = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
	m_pTxt_Amount = (PAUIEDITBOX)GetDlgItem("Txt_Amount");
	m_pTxt_Name = (PAUILABEL)GetDlgItem("Txt_Name");
	if( m_pTxt_Amount )
		m_pTxt_Amount->SetIsNumberOnly(true);

	return true;
}

void CDlgShopExchange::OnShowDialog()
{
	LuaEvent::FirstExchangeEquip();
}

void CDlgShopExchange::OnCommand_set(const char * szCommand)
{
	int i;
	for(i = 0; i < CDLGSHOPEXCHANGE_PAGE_MAX; i++)
		m_pBtnSet[i]->Show(false);

	for(i = 0; i < CDLGSHOPEXCHANGE_ITEM_MAX; i++)
	{
		CECIvtrItem *pItem = (CECIvtrItem*)m_pImgU[i]->GetDataPtr("ptr_CECIvtrItem");
		if( pItem )
			delete pItem;
		m_pImgU[i]->ClearCover();
		m_pImgU[i]->SetData(0);
		m_pImgU[i]->SetDataPtr(NULL);
		m_pImgU[i]->SetHint(_AL(""));
		m_pImgU[i]->SetText(_AL(""));
		m_pImgU[i]->SetColor(A3DCOLORRGB(255, 255, 255));
	}

	m_nShopSet = atoi(szCommand + strlen("set")) - 1;
	SelectItem(-1);

	DATA_TYPE DataType;
	ITEM_TRADE_CONFIG *pData = (ITEM_TRADE_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(GetGameUIMan()->m_pCurNPCEssence->id_item_trade_service[m_nShopIndex], ID_SPACE_ESSENCE, DataType);
	if (!pData || DataType != DT_ITEM_TRADE_CONFIG)
		return;

	m_nCurSel = -1;
	for(i = 0; i < CDLGSHOPEXCHANGE_PAGE_MAX; i++)
		if( pData->pages[i].page_title[0] != '\0' )
		{
			m_pBtnSet[i]->SetText(pData->pages[i].page_title);
			m_pBtnSet[i]->Show(true);
			if( i == m_nShopSet )
				m_pBtnSet[i]->SetPushed(true);
			else
				m_pBtnSet[i]->SetPushed(false);
		}
	for(i = 0; i < CDLGSHOPEXCHANGE_ITEM_MAX; i++)
	{
		CECIvtrItem *pItem;
		if( pData->pages[m_nShopSet].goods[i].id_goods )
		{
			pItem = CECIvtrItem::CreateItem(pData->pages[m_nShopSet].goods[i].id_goods, 0, 1);
			if( pItem )
			{
				AString strFile;
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				m_pImgU[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
				pItem->GetDetailDataFromLocal();
				int iNumGoods = pData->pages[m_nShopSet].goods[i].goods_num;
				if( iNumGoods > 1 )
				{
					ACHAR szText[100];
					a_sprintf(szText, _AL("%d"), iNumGoods);
					m_pImgU[i]->SetText(szText);
				}
				else
					m_pImgU[i]->SetText(_AL(""));

				const ACHAR* szDesc = pItem->GetDesc();
				AUICTranslate trans;
				if( szDesc )
					m_pImgU[i]->SetHint(trans.Translate(szDesc));
				else
					m_pImgU[i]->SetHint(pItem->GetName());
				m_pImgU[i]->SetDataPtr(pItem, "ptr_CECIvtrItem");
				m_pImgU[i]->SetData(i);
				m_pImgU[i]->SetColor(GetGameUIMan()->GetItemColor(pItem));
			}
		}
	}
}

void CDlgShopExchange::OnCommand_exchange(const char * szCommand)
{
	if( !GetDlgItem("Btn_Exchange")->IsEnabled() )
		return;

	CDlgShopExchange* pDlg = (CDlgShopExchange*)GetGameUIMan()->GetDialog("Win_ShopExchange_InputNO");
	pDlg->UpdateItemInfo(m_nCurSel);
	pDlg->Show(true, true);
	CECIvtrItem *pItem = (CECIvtrItem*)m_pImgU[m_nCurSel]->GetDataPtr("ptr_CECIvtrItem");
	if( pItem )
	{
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		pDlg->m_pImg_Item->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		ACString strText = m_pImgU[m_nCurSel]->GetText();
		pDlg->m_pImg_Item->SetText(strText);
		int iNum = 1;
		if (strText.GetLength()>0)
		{
			iNum = strText.ToInt();
		}
		pItem->GetDetailDataFromLocal();
		const ACHAR* szDesc = pItem->GetDesc();
		AUICTranslate trans;
		if( szDesc )
			pDlg->m_pImg_Item->SetHint(trans.Translate(szDesc));
		else
			pDlg->m_pImg_Item->SetHint(pItem->GetName());
		pDlg->m_pTxt_Name->SetText(pItem->GetName());
		pDlg->m_pImg_Item->SetDataPtr(pItem, "ptr_CECIvtrItem");
		pDlg->m_pTxt_Amount->SetText(_AL("1"));
		pDlg->SetData(pItem->GetPileLimit()/iNum);
	}
}

void CDlgShopExchange::OnCommand_max(const char * szCommand)
{
	ACHAR szText[20];
	a_sprintf(szText, _AL("%d"), m_pTxt_Amount->GetData());
	m_pTxt_Amount->SetText(szText);
}

void CDlgShopExchange::OnCommand_add(const char * szCommand)
{
	ACHAR szText[20];
	int nCount = a_atoi(m_pTxt_Amount->GetText());
	if( nCount < (int)m_pTxt_Amount->GetData() )
		nCount++;
	a_sprintf(szText, _AL("%d"), nCount);
	m_pTxt_Amount->SetText(szText);
}

void CDlgShopExchange::OnCommand_minus(const char * szCommand)
{
	ACHAR szText[20];
	int nCount = a_atoi(m_pTxt_Amount->GetText());
	if( nCount > 1 )
		nCount--;
	a_sprintf(szText, _AL("%d"), nCount);
	m_pTxt_Amount->SetText(szText);
}

void CDlgShopExchange::OnCommand_confirm(const char * szCommand)
{
	CECIvtrItem *pItem = (CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
	if( pItem )
	{
		C2S::item_trade it;
		it.item_type = pItem->GetTemplateID();
		it.index = m_nShopSet * 48 + m_nCurSel;
		it.count = a_atoi(m_pTxt_Amount->GetText());
		GetGameSession()->c2s_CmdNPCSevItemTrade(m_nShopIndex, &it, 1);
		Show(false);
	}
}

void CDlgShopExchange::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	if( stricmp(m_szName, "Win_ShopExchange") == 0 )
	{
		GetGameUIMan()->EndNPCService();
		GetGameUIMan()->m_pDlgInventory->Show(false);
		int i;
		for(i = 0; i < CDLGSHOPEXCHANGE_ITEM_MAX; i++)
		{
			CECIvtrItem *pItem = (CECIvtrItem*)m_pImgU[i]->GetDataPtr("ptr_CECIvtrItem");
			if( pItem )
				delete pItem;
			m_pImgU[i]->SetDataPtr(NULL);
		}
	}
}

void CDlgShopExchange::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT ptCapture = { GET_X_LPARAM(lParam) - p->X, GET_Y_LPARAM(lParam) - p->Y };

	GetGameUIMan()->InvokeDragDrop(this, pObj, ptCapture);
	GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
	SelectItem(atoi(pObj->GetName() + strlen("U_")) - 1);
}

void CDlgShopExchange::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;

	SelectItem(atoi(pObj->GetName() + strlen("U_")) - 1);
	OnCommand_exchange("");
	((CDlgShopExchange*)GetGameUIMan()->GetDialog("Win_ShopExchange_InputNO"))->OnCommand_max("");
}

void CDlgShopExchange::OnTick()
{
	if( stricmp(m_szName, "Win_ShopExchange_InputNO") == 0 )
	{
		int nCount = a_atoi(m_pTxt_Amount->GetText());
		UpdateItemInfo(nCount);
	}
	else
		UpdateItemInfo();
}

void CDlgShopExchange::SelectItem(int nSel)
{
	if( m_nCurSel != -1 )
		m_pImgU[m_nCurSel]->SetCover(NULL, 0, 1);
	m_nCurSel = nSel;
	if( m_nCurSel != -1 )
	{
		m_pImgU[m_nCurSel]->SetCover(GetGameUIMan()->m_pA2DSpriteMask, 0, 1);
		GetDlgItem("Btn_Exchange")->Enable(true);
	}
	else
		GetDlgItem("Btn_Exchange")->Enable(false);
	UpdateItemInfo();
}

void CDlgShopExchange::UpdateItemInfo(int nCount)
{
	int i;
	for(i = 0 ; i < 2; i++)
	{
		m_pImg_ItemReq[i]->Show(false);
		m_pImg_ItemReq[i]->ClearCover();
		m_pImg_ItemReq[i]->SetHint(_AL(""));
		m_pTxt_ItemReq[i]->SetText(_AL(""));
		m_pTxt_CreditReq[i]->SetText(_AL(""));
	}
	m_pTxt_SpecialReq->SetText(_AL(""));

	if( m_nCurSel == -1 )
		return;

	DATA_TYPE DataType;
	NPC_ESSENCE	*pCurNPCEssence = GetGameUIMan()->m_pCurNPCEssence;
	ITEM_TRADE_CONFIG *pData = NULL;
	if (pCurNPCEssence)
	{
		pData = (ITEM_TRADE_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(pCurNPCEssence->id_item_trade_service[m_nShopIndex], ID_SPACE_ESSENCE, DataType);

	}
	if (!pData || DataType != DT_ITEM_TRADE_CONFIG)
		return;
	
	int nMax = 9999;
	bool bEnable = true;
	if( nCount == 0 )
	{
		nCount = 1;
		bEnable = false;
	}
	for(i = 0 ; i < 2; i++)
	{
		ACHAR szText[100];
		if( pData->pages[m_nShopSet].goods[m_nCurSel].item_required[i].id && pData->pages[m_nShopSet].goods[m_nCurSel].item_required[i].count > 0 )
		{
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(pData->pages[m_nShopSet].goods[m_nCurSel].item_required[i].id, 0, pData->pages[m_nShopSet].goods[m_nCurSel].item_required[i].count * nCount);
			if( pItem )
			{
				AString strFile;
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				m_pImg_ItemReq[i]->Show(true);
				m_pImg_ItemReq[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
				pItem->GetDetailDataFromLocal();

				const ACHAR* szDesc = pItem->GetDesc();
				AUICTranslate trans;
				if( szDesc )
					m_pImg_ItemReq[i]->SetHint(trans.Translate(szDesc));
				else
					m_pImg_ItemReq[i]->SetHint(pItem->GetName());
				if( pData->pages[m_nShopSet].goods[m_nCurSel].item_required[i].count * nCount > 1 )
				{
					a_sprintf(szText, _AL("%d"), pData->pages[m_nShopSet].goods[m_nCurSel].item_required[i].count * nCount);
					m_pImg_ItemReq[i]->SetText(szText);
				}
				else
					m_pImg_ItemReq[i]->SetText(_AL(""));
				m_pTxt_ItemReq[i]->SetText(pItem->GetName());
				delete pItem;
				int nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(pData->pages[m_nShopSet].goods[m_nCurSel].item_required[i].id, true) / pData->pages[m_nShopSet].goods[m_nCurSel].item_required[i].count;
				if( nNum >= nCount )
					m_pTxt_ItemReq[i]->SetColor(A3DCOLORRGB(255, 255, 255));
				else
				{
					m_pTxt_ItemReq[i]->SetColor(A3DCOLORRGB(255, 0, 0));
					bEnable = false;
				}
				a_ClampRoof(nMax, nNum);
			}
		}
		if( pData->pages[m_nShopSet].goods[m_nCurSel].repu_required[i].id && pData->pages[m_nShopSet].goods[m_nCurSel].repu_required[i].count > 0 )
		{
			a_sprintf(szText, GetStringFromTable(6600 + pData->pages[m_nShopSet].goods[m_nCurSel].repu_required[i].id), 
				pData->pages[m_nShopSet].goods[m_nCurSel].repu_required[i].count * nCount);
			m_pTxt_CreditReq[i]->SetText(szText);
			int nNum = GetHostPlayer()->GetRegionReputation(pData->pages[m_nShopSet].goods[m_nCurSel].repu_required[i].id) / pData->pages[m_nShopSet].goods[m_nCurSel].repu_required[i].count;
			if( nNum >= nCount )
				m_pTxt_CreditReq[i]->SetColor(A3DCOLORRGB(255, 255, 255));
			else
			{
				m_pTxt_CreditReq[i]->SetColor(A3DCOLORRGB(255, 0, 0));
				bEnable = false;
			}
			a_ClampRoof(nMax, nNum);
		}
	}
	if( pData->pages[m_nShopSet].goods[m_nCurSel].special_required[0].type && pData->pages[m_nShopSet].goods[m_nCurSel].special_required[0].count > 0 )
	{
		ACHAR szText[100];
		a_sprintf(szText, GetStringFromTable(6599), 
			pData->pages[m_nShopSet].goods[m_nCurSel].special_required[0].count * nCount);
		m_pTxt_SpecialReq->SetText(szText);
		int nNum = GetHostPlayer()->GetBattleScore() / pData->pages[m_nShopSet].goods[m_nCurSel].special_required[0].count;
		if( nNum >= nCount )
			m_pTxt_SpecialReq->SetColor(A3DCOLORRGB(255, 255, 255));
		else
		{
			m_pTxt_SpecialReq->SetColor(A3DCOLORRGB(255, 0, 0));
			bEnable = false;
		}
		a_ClampRoof(nMax, nNum);
	}
	PAUIOBJECT pButton = GetDlgItem("Btn_Exchange");
	if( pButton )
		pButton->Enable(bEnable);
	if( m_pTxt_Amount )
		m_pTxt_Amount->SetData(min((int)GetData(), nMax));
}
