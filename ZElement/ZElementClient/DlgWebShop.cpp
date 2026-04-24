
#include "DlgWebShop.h"
#include "time.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_IvtrItem.h"
#include "EC_GameUIMan.h"
#include "AFI.h"
#include "AUI\\AUICTranslate.h"
#include "EC_HostPlayer.h"
#include "EC_GameRun.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgWebShopOrder, CDlgBase)

AUI_ON_COMMAND("Btn_Get",				OnCommand_GetItems)
AUI_ON_COMMAND("Btn_Link",				OnCommand_OpenLink)
AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgWebShopOrder, CDlgBase)

AUI_ON_EVENT("Lst_List",		WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgWebShopOrder::CDlgWebShopOrder()
{

}

CDlgWebShopOrder::~CDlgWebShopOrder()
{
}

bool CDlgWebShopOrder::OnInitDialog()
{
	m_pTxt_ItemNum = (PAUILABEL)GetDlgItem("Txt_Num");
	m_pTxt_ReqNum = (PAUILABEL)GetDlgItem("Txt_Space");
	m_pTxt_PackageName = (PAUILABEL)GetDlgItem("Txt_Name");
	m_pLst_Order = (PAUILISTBOX)GetDlgItem("Lst_List");
	m_pBtn_Get = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Get");
	m_bHasOrder = false;
	m_iReqNum = 0;
	char szItem[40];
	for(int i = 0; i < GIFT_PACKAGE_SIZE; i++)
	{
		sprintf(szItem, "Item_%02d", i + 1);
		m_pImgItem[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}
	
	m_vecOrder.clear();
	m_vecPackageName.clear();

	return true;
}

void CDlgWebShopOrder::OnShowDialog()
{

}

void CDlgWebShopOrder::OnTick()
{
	PAUIIMAGEPICTURE pObj;
	for(int i = 0; i < GIFT_PACKAGE_SIZE; i++)
	{
		pObj = m_pImgItem[i];
		if( !pObj ) continue;
		pObj->SetCover(NULL, -1);
		pObj->SetText(_AL(""));
		pObj->SetColor(A3DCOLORRGB(255, 255, 255));	
	}
	m_pBtn_Get->Enable(false);
	m_pTxt_ItemNum->SetText(_AL(""));
	m_pTxt_ReqNum->SetText(_AL(""));
	m_pTxt_PackageName->SetText(_AL(""));
	int nSel = m_pLst_Order->GetCurSel();
	AString strFile;
	AUICTranslate trans;
	ACHAR szText[40];
	if(nSel >=0 && nSel < m_pLst_Order->GetCount())
	{
		__int64 order_id = m_pLst_Order->GetItemData64(nSel);
		for(size_t i = 0; i < m_vecOrder.size(); i++)
		{
			if(order_id == m_vecOrder[i].order_id)
			{
				m_pBtn_Get->Enable(true);
				a_sprintf(szText, _AL("%d"), m_vecOrder[i].package[0].count);
				m_pTxt_ItemNum->SetText(szText);
				m_pTxt_PackageName->SetText(m_vecPackageName[i]);
				m_iReqNum = 0;
				for(int j = 0; j < GIFT_PACKAGE_SIZE; j++)
				{
					pObj = m_pImgItem[j];
					if( !pObj ) continue;
					if(j < m_vecOrder[i].package[0].good_count)
					{
						CECIvtrItem *pIvtrItem = CECIvtrItem::CreateItem(m_vecOrder[i].package[0].goods[j].goods_id, 0, 1);
						if(!pIvtrItem)
							continue;
						int iPileLimit = pIvtrItem->GetPileLimit();
						int itotalItems = m_vecOrder[i].package[0].count * m_vecOrder[i].package[0].goods[j].goods_count;
						if(iPileLimit != 0)
						{
							m_iReqNum += itotalItems / iPileLimit;
							if((itotalItems % iPileLimit) != 0)
								m_iReqNum += 1;
						}
						af_GetFileTitle(pIvtrItem->GetIconFile(), strFile);
						strFile.MakeLower();
						pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
						GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
						pIvtrItem->GetDetailDataFromLocal();
						const wchar_t* szDesc = pIvtrItem->GetDesc();
						if(szDesc)
							pObj->SetHint(trans.Translate(szDesc));
						else
							pObj->SetHint(pIvtrItem->GetName());
						if(m_vecOrder[i].package[0].goods[j].goods_count > 1)
						{
							a_sprintf(szText, _AL("%d"), m_vecOrder[i].package[0].goods[j].goods_count);
							pObj->SetText(szText);
						}
						else
							pObj->SetText(_AL(""));
						delete pIvtrItem;
					}
				}
				a_sprintf(szText, _AL("%d"), m_iReqNum);
				m_pTxt_ReqNum->SetText(szText);
				break;
			}
		}
	}	
}


void CDlgWebShopOrder::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgWebShopOrder::OnCommand_GetItems(const char * szCommand)
{
	if(GetHostPlayer()->GetPack()->GetEmptySlotNum() < m_iReqNum)
	{
		GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(7076), GP_CHAT_MISC);
		return;
	}
	int nSel = m_pLst_Order->GetCurSel();
	if(nSel >=0 && nSel < m_pLst_Order->GetCount())
	{
		GetGameSession()->c2s_CmdGetWebOrder(m_pLst_Order->GetItemData64(nSel));
	}
}

void CDlgWebShopOrder::OnCommand_OpenLink(const char * szCommand)
{
	GetGameSession()->open_url_by_token("webshop");
}

void CDlgWebShopOrder::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	
}

void CDlgWebShopOrder::AddOrderInfo(order_info order, ACString strPackageName)
{
	m_vecOrder.push_back(order);
	m_vecPackageName.push_back(strPackageName);
	if(order.status == 0)
	{
		ACString strText;
		ACString strTime;
		long t = order.timestamp;
        t -= g_pGame->GetTimeZoneBias() * 60;
		tm gtime = *gmtime((const time_t*)&t);
		strTime.Format(_AL("%d.%d.%d %d:%02d\t"), 1900 + gtime.tm_year, gtime.tm_mon + 1, gtime.tm_mday, gtime.tm_hour, gtime.tm_min);
		strText.Format(_AL("%s\t%d\t%s\t"),  strPackageName, order.package[0].count, strTime);
		int index = m_pLst_Order->AddString(strText) - 1;
		m_pLst_Order->SetItemData64(index, order.order_id);
		m_bHasOrder = true;
	}
}

void CDlgWebShopOrder::ClearOrderInfo()
{
	m_vecOrder.clear();
	m_vecPackageName.clear();
	m_bHasOrder = false;
	m_pLst_Order->ResetContent();
}

bool CDlgWebShopOrder::IsHasOrder()
{
	return m_bHasOrder;
}

