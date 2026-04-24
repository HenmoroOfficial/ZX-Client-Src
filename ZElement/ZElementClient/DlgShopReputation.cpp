#include "AFI.h"
#include "AUI\\AUICTranslate.h"
#include "DlgShopReputation.h"
#include "DlgInventory.h"
#include "DlgInputNO.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_DealInventory.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgShopReputation, CDlgBase)

AUI_ON_COMMAND("set*",				OnCommand_set)
AUI_ON_COMMAND("buy",				OnCommand_buy)
AUI_ON_COMMAND("IDCANCEL",			OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgShopReputation, CDlgBase)

AUI_ON_EVENT("*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("*",		WM_LBUTTONDBLCLK,	OnEventLButtonDblclk)
AUI_ON_EVENT("*",		WM_RBUTTONUP,		OnEventRButtonUp)

AUI_END_EVENT_MAP()

CDlgShopReputation::CDlgShopReputation( )
{
}

CDlgShopReputation::~CDlgShopReputation()
{
}

void CDlgShopReputation::OnCommand_buy(const char * szCommand)
{	
	
	if(GetHostPlayer()->GetRegionReputation(TIANZONG_CONSUME) >= m_iRepu_Consume_Value[0]
		&& GetHostPlayer()->GetRegionReputation(DIZONG_CONSUME) >= m_iRepu_Consume_Value[1]
		&& GetHostPlayer()->GetRegionReputation(RENZONG_CONSUME) >= m_iRepu_Consume_Value[2]
		&& m_iFlag)
		GetHostPlayer()->BuyItemsFromShopReputation();		
}

void CDlgShopReputation::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	if (stricmp(m_szName, "Win_Shop_Credit")==0)
	{
		GetGameUIMan()->EndNPCService();
		GetGameUIMan()->m_nCursorState = CECGameUIMan::CURSOR_STATE_NORMAL;
		GetGameUIMan()->m_pDlgInventory->Show(false);
		GetGameUIMan()->GetDialog("Win_InventoryPet")->Show(false);
		if (!CDlgInventory::GetIsArranging())
		{
			GetGameUIMan()->UnfreezeAllItems();
		}
	}
}


bool CDlgShopReputation::OnInitDialog()
{
	int i;
	char szItem[40];
	
	for(i = 0; i < CDLGSHOPREPUTATION_ITEM_MAX; i++)
	{
		sprintf(szItem, "U_%02d", i + 1);
		m_pImgU[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}
	
	for(i = 0; i < IVTRSIZE_BUYPACK; i++)
	{
		sprintf(szItem, "Buy_%02d", i + 1);
		m_pImgBuy[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}

	for(i = 0; i < CDLGSHOPREPUTATION_PAGE_MAX; i++)
	{
		sprintf(szItem, "Btn_Set%d", i + 1);
		m_pBtnSet[i] = (PAUISTILLIMAGEBUTTON)GetDlgItem(szItem);		
	}
	m_pTxt_Reputation23 = (PAUILABEL)GetDlgItem("Txt_Credit23");
	m_pTxt_Reputation24 = (PAUILABEL)GetDlgItem("Txt_Credit24");
	m_pTxt_Reputation25 = (PAUILABEL)GetDlgItem("Txt_Credit25");
	m_pTxt_Reputation26 = (PAUILABEL)GetDlgItem("Txt_Credit26");
	m_pTxt_Reputation27 = (PAUILABEL)GetDlgItem("Txt_Credit27");
	m_pTxt_Reputation28 = (PAUILABEL)GetDlgItem("Txt_Credit28");
	m_pLab_Hint = (PAUILABEL)GetDlgItem("Lab_Hint");
	m_pLab_Hint2 = (PAUILABEL)GetDlgItem("Lab_Hint2"); 
	return true;
}

void CDlgShopReputation::OnShowDialog()
{

	if (stricmp(m_szName, "Win_Shop_Credit")==0)
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
		m_pTxt_Reputation26->SetText(_AL("0"));
		m_pTxt_Reputation27->SetText(_AL("0"));
		m_pTxt_Reputation28->SetText(_AL("0"));
		m_pLab_Hint2->Show(false);
	}

}

void CDlgShopReputation::OnCommand_set(const char * szCommand)
{
	int i;
	m_nCurShopSet = atoi(szCommand + strlen("set")) - 1;
	
	for(i = 0; i < 4; i++)
		m_pBtnSet[i]->Show(false);

	CECReputationInventory* pShop;
	for(i = 0; i < 4; i++)
	{
		pShop = GetHostPlayer()->GetReputationPack(i);
		const ACHAR* ShopName = pShop->GetName();
		if(ShopName[0] != '\0')
		{
			m_pBtnSet[i]->SetText(ShopName);
			m_pBtnSet[i]->Show(true);
			if(i == m_nCurShopSet)
				m_pBtnSet[i]->SetPushed(true);
			else
				m_pBtnSet[i]->SetPushed(false);
		}
	}
}

bool CDlgShopReputation::Tick()
{
	UpdateShop();
	return true;
}

int CDlgShopReputation::GetCurShopSet()
{
	return m_nCurShopSet;
}

bool CDlgShopReputation::UpdateShop()
{

	int i, j;
	AString strFile;
	ACHAR szText[40];
	CECIvtrItem *pItem;
	PAUIIMAGEPICTURE pObj;
	CECReputationInventory* pNPC;
	pNPC = GetHostPlayer()->GetReputationPack(m_nCurShopSet);
	
	m_iFlag = true;

	abase::vector<int> vecRegionRepu;
	for(i = 0; i < GetHostPlayer()->GetRegionReputationCount(); i++)
	{
		vecRegionRepu.push_back(GetHostPlayer()->GetRegionReputation(i));
	}
	
	int aa = GetHostPlayer()->GetRegionReputation(23);
	
	for(i = 0; i < CDLGSHOPREPUTATION_ITEM_MAX; i++)
	{
		pObj = m_pImgU[i];
		if(!pObj) continue;

		pItem = pNPC->GetItem(i);
		if(pItem)
		{
			pObj->SetDataPtr(pItem, "ptr_CECIvtrItem");
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
			pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
			pItem->GetDetailDataFromLocal();
			const ACHAR* szDesc = pItem->GetDesc();
			AUICTranslate trans;
			if(szDesc)
				pObj->SetHint(trans.Translate(szDesc));
			else
				pObj->SetHint(pItem->GetName());
			
			for(int j = 0; j < 3; j++)
				if(GetHostPlayer()->GetRegionReputation(pNPC->goods[i].repu_required_pre[j].repu_id_req) < pNPC->goods[i].repu_required_pre[j].repu_req_value)
			{
				pObj->SetColor(A3DCOLORRGB(255, 0, 0));
				break;
			}
		
		}
		else
		{
			pObj->SetCover(NULL, -1);
			pObj->SetDataPtr(NULL);
			pObj->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	}
	
	int iFlag = 0;
	int nTotalReputation[3] = {0};
	CECDealInventory* pDeal;
	pDeal = GetHostPlayer()->GetBuyPack();
	for(i = 0; i < IVTRSIZE_BUYPACK; i++)
	{
		pObj = m_pImgBuy[i];
		if(!pObj) continue;

		pItem = pDeal->GetItem(i);
		if(pItem)
		{
			pObj->SetDataPtr(pItem, "ptr_CECIvtrItem");
			pObj->SetColor(A3DCOLORRGB(255, 255, 255));

			const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(i);
			if(ii.iAmount > 1)
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

			for(j = 0; j < 3; j++)
			{
				int a, b;
				a = pDeal->GetItemInfo(i).repu_consume_value[j];
				b = pDeal->GetItemInfo(i).iAmount;
				if(pDeal->GetItemInfo(i).repu_id_consume[j] == TIANZONG_CONSUME)
					nTotalReputation[0] += pDeal->GetItemInfo(i).repu_consume_value[j] * pDeal->GetItemInfo(i).iAmount;
				else if(pDeal->GetItemInfo(i).repu_id_consume[j] == DIZONG_CONSUME)
					nTotalReputation[1] += pDeal->GetItemInfo(i).repu_consume_value[j] * pDeal->GetItemInfo(i).iAmount;
				else if(pDeal->GetItemInfo(i).repu_id_consume[j] == RENZONG_CONSUME)
					nTotalReputation[2] += pDeal->GetItemInfo(i).repu_consume_value[j] * pDeal->GetItemInfo(i).iAmount; 
				if(pDeal->GetItemInfo(i).repu_id_consume[j])
				{
					vecRegionRepu[pDeal->GetItemInfo(i).repu_id_consume[j]] -= pDeal->GetItemInfo(i).repu_consume_value[j] * pDeal->GetItemInfo(i).iAmount;
					if(vecRegionRepu[pDeal->GetItemInfo(i).repu_id_consume[j]] < 0)
						m_iFlag = false;
				}
			}
		}
		else
		{
			pObj->SetCover(NULL, -1);
			pObj->SetText(_AL(""));
			pObj->SetDataPtr(NULL);
		}
	}

	m_iRepu_Consume_Value[0] = nTotalReputation[0];
	m_iRepu_Consume_Value[1] = nTotalReputation[1];
	m_iRepu_Consume_Value[2] = nTotalReputation[2];

	m_pLab_Hint->Show(false);
	m_pTxt_Reputation23->SetColor(A3DCOLORRGB(255, 255, 255));
	m_pTxt_Reputation24->SetColor(A3DCOLORRGB(255, 255, 255));
	m_pTxt_Reputation25->SetColor(A3DCOLORRGB(255, 255, 255));

	if(GetHostPlayer()->GetRegionReputation(TIANZONG_CONSUME) < m_iRepu_Consume_Value[0])
	{
		m_pLab_Hint->Show(true);
		m_pTxt_Reputation23->SetColor(A3DCOLORRGB(255, 0, 0));
	}

	if(GetHostPlayer()->GetRegionReputation(DIZONG_CONSUME) < m_iRepu_Consume_Value[1])
	{
		m_pLab_Hint->Show(true);
		m_pTxt_Reputation24->SetColor(A3DCOLORRGB(255, 0, 0));
	}

	if(GetHostPlayer()->GetRegionReputation(RENZONG_CONSUME) < m_iRepu_Consume_Value[2])
	{
		m_pLab_Hint->Show(true);
		m_pTxt_Reputation25->SetColor(A3DCOLORRGB(255, 0, 0));
	}

	a_sprintf(szText, _AL("%d/%d"), m_iRepu_Consume_Value[0], GetHostPlayer()->GetRegionReputation(TIANZONG_CONSUME));
	m_pTxt_Reputation23->SetText(szText);

	a_sprintf(szText, _AL("%d/%d"), m_iRepu_Consume_Value[1], GetHostPlayer()->GetRegionReputation(DIZONG_CONSUME));
	m_pTxt_Reputation24->SetText(szText);
	
	a_sprintf(szText, _AL("%d/%d"), m_iRepu_Consume_Value[2], GetHostPlayer()->GetRegionReputation(RENZONG_CONSUME) );
	m_pTxt_Reputation25->SetText(szText);

	return true;
}

void CDlgShopReputation::OnEventLButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{

	if( !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;
	int iFlag, iFlag26, iFlag27, iFlag28;
	iFlag = 0; iFlag26 = 0; iFlag27 = 0; iFlag28 = 0;
	ACHAR szText[40];
	CECHostPlayer *pHost = GetHostPlayer();
	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	CECReputationInventory* pNPC;
	pNPC = GetHostPlayer()->GetReputationPack(m_nCurShopSet);

	if(strstr(pObj->GetName(), "U_"))
	{
		int iSrc = atoi(pObj->GetName() + strlen("U_")) - 1;
		m_pLab_Hint2->Show(false);
		
		for(int j = 0; j < 3; j++)
		{						
			a_sprintf(szText, _AL("%d"), pNPC->goods[iSrc].repu_required_pre[j].repu_req_value);
			if(pNPC->goods[iSrc].repu_required_pre[j].repu_id_req == 25)
			{
				m_pTxt_Reputation26->SetText(szText);
				iFlag26 = 1;
				if(GetHostPlayer()->GetRegionReputation(pNPC->goods[iSrc].repu_required_pre[j].repu_id_req) < pNPC->goods[iSrc].repu_required_pre[j].repu_req_value)
				{
					m_pTxt_Reputation26->SetColor(A3DCOLORRGB(255, 0, 0));
					iFlag = 1;
				}
				else
					m_pTxt_Reputation26->SetColor(A3DCOLORRGB(255, 255, 255));
			}
			else if(pNPC->goods[iSrc].repu_required_pre[j].repu_id_req == 26)
			{
				m_pTxt_Reputation27->SetText(szText);
				iFlag27 = 1;
				if(GetHostPlayer()->GetRegionReputation(pNPC->goods[iSrc].repu_required_pre[j].repu_id_req) < pNPC->goods[iSrc].repu_required_pre[j].repu_req_value)
				{
					m_pTxt_Reputation27->SetColor(A3DCOLORRGB(255, 0, 0));
					iFlag = 1;
				}
				else
					m_pTxt_Reputation27->SetColor(A3DCOLORRGB(255, 255, 255));
			}
			else if(pNPC->goods[iSrc].repu_required_pre[j].repu_id_req == 27)
			{
				iFlag28 = 1;
				m_pTxt_Reputation28->SetText(szText);
				if(GetHostPlayer()->GetRegionReputation(pNPC->goods[iSrc].repu_required_pre[j].repu_id_req) < pNPC->goods[iSrc].repu_required_pre[j].repu_req_value)
				{
					m_pTxt_Reputation28->SetColor(A3DCOLORRGB(255, 0, 0));
					iFlag = 1;
				}
				else
					m_pTxt_Reputation28->SetColor(A3DCOLORRGB(255, 255, 255));	
			}
		}
		// ·ÀÖ¹Ä£°åÊý¾ÝÌî´í
		if(!iFlag26)
		{
			m_pTxt_Reputation26->SetText(_AL("0"));
			m_pTxt_Reputation26->SetColor(A3DCOLORRGB(255, 255, 255));
		}
		if(!iFlag27)
		{
			m_pTxt_Reputation27->SetText(_AL("0"));
			m_pTxt_Reputation27->SetColor(A3DCOLORRGB(255, 255, 255));
		}
		if(!iFlag28)
		{
			m_pTxt_Reputation28->SetText(_AL("0"));
			m_pTxt_Reputation28->SetColor(A3DCOLORRGB(255, 255, 255));
		}
		if(iFlag)
		{
			m_pLab_Hint2->Show(true);
			return;
		}
	}

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
			nInputNOMode = CDlgInputNO::INPUTNO_REPUTATION_BUY_ADD;
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
		pEdit->SetText(szText);
		pShow->SetType(nInputNOMode);
	}
	else
	{
		if( strstr(pObj->GetName(), "U_") )
		{
			int iSrc;
			iSrc = atoi(pObj->GetName() + strlen("U_")) - 1;
			unsigned int repu_id[3], repu_consume[3];			
			for(int i = 0; i < 3; i++)
			{
				repu_id[i] = pNPC->goods[iSrc].repu_required_pre[i].repu_id_consume;
				repu_consume[i] = pNPC->goods[iSrc].repu_required_pre[i].repu_consume_value;
			}
			CECDealInventory *pDeal = pHost->GetBuyPack();
			iSrc = atoi(pObj->GetName() + strlen("U_")) + m_nCurShopSet * IVTRSIZE_NPCPACK - 1;
			pDeal->AddItemFree(pItem, iSrc, pItem->GetPileLimit(), repu_id, repu_consume);
		}
		else if( strstr(pObj->GetName(), "Buy_") )
		{
			CECDealInventory *pDeal = pHost->GetBuyPack();
			int iSrc = atoi(pObj->GetName() + strlen("Buy_")) - 1;

			const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(iSrc);
			pDeal->RemoveItemByIndex(iSrc, ii.iAmount);
		}
	}
	
}

void CDlgShopReputation::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{

	if( !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;
	CECHostPlayer *pHost = GetHostPlayer();
	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	ACHAR szText[40];
	CECReputationInventory* pNPC;
	pNPC = GetHostPlayer()->GetReputationPack(m_nCurShopSet);
	int iSrc, j;
	int iFlag, iFlag26, iFlag27, iFlag28;
	iFlag = 0; iFlag26 = 0; iFlag27 = 0; iFlag28 = 0;
	if( strstr(pObj->GetName(), "U_") )
	{		
		iSrc = atoi(pObj->GetName() + strlen("U_")) - 1;
		m_pLab_Hint2->Show(false);

		for(j = 0; j < 3; j++)
		{						
			a_sprintf(szText, _AL("%d"), pNPC->goods[iSrc].repu_required_pre[j].repu_req_value);
			if(pNPC->goods[iSrc].repu_required_pre[j].repu_id_req == 25)
			{
				m_pTxt_Reputation26->SetText(szText);
				iFlag26 = 1;
				if(GetHostPlayer()->GetRegionReputation(pNPC->goods[iSrc].repu_required_pre[j].repu_id_req) < pNPC->goods[iSrc].repu_required_pre[j].repu_req_value)
				{
					m_pTxt_Reputation26->SetColor(A3DCOLORRGB(255, 0, 0));
					iFlag = 1;
				}
				else
					m_pTxt_Reputation26->SetColor(A3DCOLORRGB(255, 255, 255));
			}
			else if(pNPC->goods[iSrc].repu_required_pre[j].repu_id_req == 26)
			{
				m_pTxt_Reputation27->SetText(szText);
				iFlag27 = 1;
				if(GetHostPlayer()->GetRegionReputation(pNPC->goods[iSrc].repu_required_pre[j].repu_id_req) < pNPC->goods[iSrc].repu_required_pre[j].repu_req_value)
				{
					m_pTxt_Reputation27->SetColor(A3DCOLORRGB(255, 0, 0));
					iFlag = 1;
				}
				else
					m_pTxt_Reputation27->SetColor(A3DCOLORRGB(255, 255, 255));
			}
			else if(pNPC->goods[iSrc].repu_required_pre[j].repu_id_req == 27)
			{
				iFlag28 = 1;
				m_pTxt_Reputation28->SetText(szText);
				if(GetHostPlayer()->GetRegionReputation(pNPC->goods[iSrc].repu_required_pre[j].repu_id_req) < pNPC->goods[iSrc].repu_required_pre[j].repu_req_value)
				{
					m_pTxt_Reputation28->SetColor(A3DCOLORRGB(255, 0, 0));
					iFlag = 1;
				}
				else
					m_pTxt_Reputation28->SetColor(A3DCOLORRGB(255, 255, 255));	
			}
		}
		if(!iFlag26)
		{
			m_pTxt_Reputation26->SetText(_AL("0"));
			m_pTxt_Reputation26->SetColor(A3DCOLORRGB(255, 255, 255));
		}
		if(!iFlag27)
		{
			m_pTxt_Reputation27->SetText(_AL("0"));
			m_pTxt_Reputation27->SetColor(A3DCOLORRGB(255, 255, 255));
		}
		if(!iFlag28)
		{
			m_pTxt_Reputation28->SetText(_AL("0"));
			m_pTxt_Reputation28->SetColor(A3DCOLORRGB(255, 255, 255));
		}
		if(iFlag)
		{
			m_pLab_Hint2->Show(true);
			return;
		}
			
		iSrc = atoi(pObj->GetName() + strlen("U_")) - 1;
		for(j = 0; j < 3; j++)
		{
			if(GetHostPlayer()->GetRegionReputation(pNPC->goods[iSrc].repu_required_pre[j].repu_id_req) < pNPC->goods[iSrc].repu_required_pre[j].repu_req_value)
				return;	
		}
		
		unsigned int repu_id[3], repu_consume[3];

		for(int i = 0; i < 3; i++)
		{
			repu_id[i] = pNPC->goods[iSrc].repu_required_pre[i].repu_id_consume;
			repu_consume[i] = pNPC->goods[iSrc].repu_required_pre[i].repu_consume_value;
		}
		CECDealInventory *pDeal = pHost->GetBuyPack();
		iSrc = atoi(pObj->GetName() + strlen("U_")) + m_nCurShopSet * IVTRSIZE_NPCPACK - 1;

		pDeal->AddItemFree(pItem, iSrc, pItem->GetPileLimit(), repu_id, repu_consume);
	}
	else if( strstr(pObj->GetName(), "Buy_") )
	{
		CECDealInventory *pDeal = pHost->GetBuyPack();
		int iSrc = atoi(pObj->GetName() + strlen("Buy_")) - 1;

		const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(iSrc);
		pDeal->RemoveItemByIndex(iSrc, ii.iAmount);
	}
}

void CDlgShopReputation::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT ptCapture = { GET_X_LPARAM(lParam) - p->X, GET_Y_LPARAM(lParam) - p->Y };

	GetGameUIMan()->InvokeDragDrop(this, pObj, ptCapture);
	GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
}


