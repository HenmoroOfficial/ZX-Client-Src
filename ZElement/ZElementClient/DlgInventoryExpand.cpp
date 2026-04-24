#include "AFI.h"
#include "AUI\\AUICTranslate.h"
#include "DlgInventoryExpand.h"
#include "DlgInventory.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_GPDataType.h"
#include "globaldataman.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgInventoryExPand, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)
AUI_ON_COMMAND("start",			OnCommandConfirm)
AUI_ON_COMMAND("Btn_Add",		OnCommandAdd)
AUI_ON_COMMAND("Btn_Minus",		OnCommandMinus)
AUI_ON_COMMAND("Btn_Cancel",    OnCommandCancel)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgInventoryExPand, CDlgBase)

AUI_END_EVENT_MAP()

CDlgInventoryExPand::CDlgInventoryExPand()
{
}

CDlgInventoryExPand::~CDlgInventoryExPand()
{
}

bool CDlgInventoryExPand::OnInitDialog()
{
	m_pImg_Item1 = (PAUIIMAGEPICTURE)GetDlgItem("Item_01");
	m_pImg_Item2 = (PAUIIMAGEPICTURE)GetDlgItem("Item_02");
	m_pBtn_Confirm = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Confirm");
	m_pTxt_Num = (PAUILABEL)GetDlgItem("Txt_NumNow");
	m_pEdit_Input = (PAUIEDITBOX)GetDlgItem("Edt_Input");
	m_pEdit_Input->SetIsReadOnly(true);
	m_pTxt_Item1Num = (PAUILABEL)GetDlgItem("Txt_Num01");
	m_pTxt_Item2Num = (PAUILABEL)GetDlgItem("Txt_Num02");
	int i;
	if(m_szName == "Win_Inventory_Expand02")
	{
		for(i = 60; i <= 96; i += 12)
			m_vecInventoryNum.push_back(i);
		for(i = 102; i <= 252; i += 6)
			m_vecInventoryNum.push_back(i);
	}
	else if(m_szName == "Win_Inventory_Expand01")
	{
		for(i = 36; i <= 96; i += 12)
			m_vecInventoryNum.push_back(i);
		for(i = 102; i <= 252; i += 6)
			m_vecInventoryNum.push_back(i);
	}

	return true;
}

void CDlgInventoryExPand::OnShowDialog()
{
	ACHAR szText[40];
	AString strFile;
	CECIvtrItem *pItem;
	const ACHAR * szDesc;
	AUICTranslate trans;
	if(m_szName == "Win_Inventory_Expand02")
	{
		int nSize = GetHostPlayer()->GetPack()->GetSize();
		a_sprintf(szText, _AL("%d"), nSize);
		m_pEdit_Input->SetText(szText);
	}
	else if(m_szName == "Win_Inventory_Expand01")
	{
		int nSize = GetHostPlayer()->GetTrashBox()->GetSize();
		a_sprintf(szText, _AL("%d"), nSize);
		m_pEdit_Input->SetText(szText);		
	}

	if(m_szName == "Win_Inventory_Expand02")	
		pItem = CECIvtrItem::CreateItem(4012, 0, 1);
	else if(m_szName == "Win_Inventory_Expand01")
		pItem = CECIvtrItem::CreateItem(4011, 0, 1);
	if(pItem)
	{
		pItem->GetDetailDataFromLocal();
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		m_pImg_Item1->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		szDesc = pItem->GetDesc();
		if( szDesc )
			m_pImg_Item1->SetHint(trans.Translate(szDesc));
		else
			m_pImg_Item1->SetHint(_AL(""));
		delete pItem;
	}

	if(m_szName == "Win_Inventory_Expand02")	
		pItem = CECIvtrItem::CreateItem(13904, 0, 1);
	else if(m_szName == "Win_Inventory_Expand01")
	pItem = CECIvtrItem::CreateItem(13903, 0, 1);
	if(pItem)
	{
		pItem->GetDetailDataFromLocal();
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		m_pImg_Item2->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		szDesc = pItem->GetDesc();
		if( szDesc )
			m_pImg_Item2->SetHint(trans.Translate(szDesc));
		else
			m_pImg_Item2->SetHint(_AL(""));
		delete pItem;
	}
}

void CDlgInventoryExPand::OnCommandCancel(const char * szCommand)
{
	Show(false);
}


void CDlgInventoryExPand::OnTick()
{
	ACHAR szText[40];
	int nSize;
	if(m_szName == "Win_Inventory_Expand02")
		 nSize = GetHostPlayer()->GetPack()->GetSize();
	else if(m_szName == "Win_Inventory_Expand01")
		nSize = GetHostPlayer()->GetTrashBox()->GetSize();
	a_sprintf(szText, _AL("%d"), nSize);
	m_pTxt_Num->SetText(szText);
	int nNum = a_atoi(m_pEdit_Input->GetText());	
	if(nSize != INVENTORY_SIZEMAX && nNum <= nSize)
		for(int i = 0; i < m_vecInventoryNum.size(); i++)
		{
			if(nSize < m_vecInventoryNum[i])
			{
				nNum = m_vecInventoryNum[i];
				a_sprintf(szText, _AL("%d"), nNum);
				m_pEdit_Input->SetText(szText);
				break;
			}
		}

	if(m_szName == "Win_Inventory_Expand02")
		CalcExpandInventory(nSize, nNum);
	else if(m_szName == "Win_Inventory_Expand01")
		CalcExpandStorage(nSize, nNum);
}


void CDlgInventoryExPand::OnCommandConfirm(const char * szCommand)
{
	using namespace C2S;
	
	resize_inv::Item_Info iteminfo[INVENTORY_SIZEMAX];
	memset(iteminfo, 0, INVENTORY_SIZEMAX * sizeof(resize_inv::Item_Info));
	PAUIDIALOG pMsgBox = NULL;
	ACString strText;
	int nMoney;
	vector<GSHOP_ITEM> *items;
	items = globaldata_getgshopitems();
	CECInventory* pPack = GetHostPlayer()->GetPack();
	int nItem1TotalNum, nItem2TotalNum, nItem1ID, nItem2ID;
	int nCount, nItem1Num, nItem2Num;
	char type;	
	if(m_szName == "Win_Inventory_Expand02")
	{
		type = 0;
		nItem1TotalNum = pPack->GetItemTotalNum(4012);
		nItem2TotalNum = pPack->GetItemTotalNum(13904);
		nItem1ID = 4012;
		nItem2ID = 13904;
	}
	else if(m_szName == "Win_Inventory_Expand01")
	{
	
		type = 1;
		nItem1TotalNum = pPack->GetItemTotalNum(4011);
		nItem2TotalNum = pPack->GetItemTotalNum(13903);
		nItem1ID = 4011;
		nItem2ID = 13903;
	}
	int nNewSize = a_atoi(m_pEdit_Input->GetText());;
	if(m_nItem1Num > 0 && m_nItem2Num <= 0)
	{
		if(nItem1TotalNum >= m_nItem1Num)
		{
			nItem1Num = m_nItem1Num;
			nCount = 0;
			for(int i = 0; i < pPack->GetSize(); i++)
			{
				CECIvtrItem *pItem = pPack->GetItem(i);
				if(!pItem)
					continue;
				if(pItem->IsLocked() || pItem->IsFrozen() || pItem->GetCount() <= 0)
					continue;
				if(pItem->GetTemplateID() == nItem1ID)
				{
					if(nItem1Num > pItem->GetCount())
					{
						iteminfo[nCount].item_idx = i;
						iteminfo[nCount].item_id  = nItem1ID;
						iteminfo[nCount].item_cnt = pItem->GetCount();
						nCount++;
						nItem1Num -= pItem->GetCount();
					}
					else
					{
						iteminfo[nCount].item_idx = i;
						iteminfo[nCount].item_id = nItem1ID;
						iteminfo[nCount].item_cnt = nItem1Num;
						nCount++;
						nItem1Num -= pItem->GetCount();
						break;
					}
				}
			}
			if(nItem1Num <= 0)
				GetGameSession()->c2s_CmdResizeInventory(type, nNewSize, nCount, iteminfo);
			OnCommandCancel("");
		}
		else
		{

			m_ExpandItem_info.iCount = 1;
			m_ExpandItem_info.cost_item[0].itemid = nItem1ID;
			m_ExpandItem_info.cost_item[0].itemindex = globaldata_getgshopitemindex_by_id(nItem1ID);
			m_ExpandItem_info.cost_item[0].itemcnt = m_nItem1Num - nItem1TotalNum;
			nMoney = (*items)[m_ExpandItem_info.cost_item[0].itemindex].buy[0].price * m_ExpandItem_info.cost_item[0].itemcnt / 100;
			if(type)
				strText.Format(GetStringFromTable(18700), nMoney, m_ExpandItem_info.cost_item[0].itemcnt);
			else
				strText.Format(GetStringFromTable(18703), nMoney, m_ExpandItem_info.cost_item[0].itemcnt);
			m_ExpandItem_info.iMoney = nMoney;
			m_ExpandItem_info.index = 0;
			GetGameUIMan()->MessageBox("Inventory_Expand", strText,
							MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			m_ExpandItem_info.iMoney = nMoney;
			if(pMsgBox)
				pMsgBox->SetDataPtr((void*)&m_ExpandItem_info, "Expand");
		}
	}
	else if(m_nItem2Num > 0 && m_nItem1Num <= 0)
	{
		if(nItem2TotalNum >= m_nItem2Num)
		{
			nItem2Num = m_nItem2Num;
			nCount = 0;
			for(int i = 0; i < pPack->GetSize(); i++)
			{
				CECIvtrItem *pItem = pPack->GetItem(i);
				if(!pItem)
					continue;
				if(pItem->IsLocked() || pItem->IsFrozen() || pItem->GetCount() <=0)
					continue;
				if(pItem->GetTemplateID() == nItem2ID)
				{
					if(nItem2Num > pItem->GetCount())
					{
						iteminfo[nCount].item_idx = i;
						iteminfo[nCount].item_id  = nItem2ID;
						iteminfo[nCount].item_cnt = pItem->GetCount();
						nCount++;
						nItem2Num -= pItem->GetCount();
					}
					else
					{
						iteminfo[nCount].item_idx = i;
						iteminfo[nCount].item_id = nItem2ID;
						iteminfo[nCount].item_cnt = nItem2Num;
						nCount++;
						nItem2Num -= pItem->GetCount();
						break;
					}
				}

			}
			if(nItem2Num <= 0)
				GetGameSession()->c2s_CmdResizeInventory(type, nNewSize, nCount, iteminfo);
			OnCommandCancel("");
		}
		else
		{
			m_ExpandItem_info.iCount = 1;
			m_ExpandItem_info.cost_item[0].itemid = nItem2ID;
			m_ExpandItem_info.cost_item[0].itemindex = globaldata_getgshopitemindex_by_id(nItem2ID);
			m_ExpandItem_info.cost_item[0].itemcnt = m_nItem2Num - nItem2TotalNum;
			nMoney = (*items)[m_ExpandItem_info.cost_item[0].itemindex].buy[0].price * m_ExpandItem_info.cost_item[0].itemcnt / 100;
			if(type)
				strText.Format(GetStringFromTable(18701), nMoney, m_ExpandItem_info.cost_item[0].itemcnt);
			else
				strText.Format(GetStringFromTable(18704), nMoney, m_ExpandItem_info.cost_item[0].itemcnt);
			m_ExpandItem_info.iMoney = nMoney;
			m_ExpandItem_info.index = 0;
			GetGameUIMan()->MessageBox("Inventory_Expand", strText,
							MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			if(pMsgBox)
				pMsgBox->SetDataPtr((void*)&m_ExpandItem_info, "Expand");
		}
	}
	else if(m_nItem1Num > 0 && m_nItem2Num > 0)
	{
		if(nItem1TotalNum >= m_nItem1Num && nItem2TotalNum >= m_nItem2Num)
		{
			nItem1Num = m_nItem1Num;
			nItem2Num = m_nItem2Num;
			nCount = 0;
			for(int i = 0; i < pPack->GetSize(); i++)
			{
				CECIvtrItem* pItem = pPack->GetItem(i);
				if(!pItem)
					continue;
				if(pItem->IsLocked() || pItem->IsFrozen() || pItem->GetCount() <= 0)
					continue;
				if(pItem->GetTemplateID() == nItem1ID)
				{
					if(nItem1Num > 0)
					{
						if(nItem1Num > pItem->GetCount())
						{
							iteminfo[nCount].item_idx = i;
							iteminfo[nCount].item_id = nItem1ID;
							iteminfo[nCount].item_cnt = pItem->GetCount();
							nCount++;
							nItem1Num -= pItem->GetCount();
						}
						else 
						{
							iteminfo[nCount].item_idx = i;
							iteminfo[nCount].item_id = nItem1ID;
							iteminfo[nCount].item_cnt = nItem1Num;
							nCount++;
							nItem1Num -= pItem->GetCount();
						}

					}
				}
				else if(pItem->GetTemplateID() == nItem2ID)
				{
					if(nItem2Num > 0)
					{
						if(nItem2Num > pItem->GetCount())
						{
							iteminfo[nCount].item_idx = i;
							iteminfo[nCount].item_id = nItem2ID;
							iteminfo[nCount].item_cnt = pItem->GetCount();
							nCount++;
							nItem2Num -= pItem->GetCount();
						}
						else
						{
							iteminfo[nCount].item_idx = i;
							iteminfo[nCount].item_id = nItem2ID;
							iteminfo[nCount].item_cnt = nItem2Num;
							nCount++;
							nItem2Num -= pItem->GetCount();
						}
					}
				}
				if(nItem1Num <= 0 && nItem2Num <=0)
					break;
			}
			if(nItem1Num <= 0 && nItem2Num <=0)
			GetGameSession()->c2s_CmdResizeInventory(type, nNewSize, nCount, iteminfo);
			OnCommandCancel("");
		}
		else
		{
			m_ExpandItem_info.iCount = 0;
			int nMoney0 = 0, nMoney1 = 0;
 			if(m_nItem1Num > nItem1TotalNum)
			{
				m_ExpandItem_info.iCount++;
				m_ExpandItem_info.cost_item[0].itemid = nItem1ID;
				m_ExpandItem_info.cost_item[0].itemindex = globaldata_getgshopitemindex_by_id(nItem1ID);
				m_ExpandItem_info.cost_item[0].itemcnt = m_nItem1Num - nItem1TotalNum;
				m_ExpandItem_info.index = 0;
				nMoney0 = (*items)[m_ExpandItem_info.cost_item[0].itemindex].buy[0].price * m_ExpandItem_info.cost_item[0].itemcnt / 100;
			}
			if(m_nItem2Num > nItem2TotalNum)
			{
				m_ExpandItem_info.iCount++;
				m_ExpandItem_info.cost_item[1].itemid = nItem2ID;
				m_ExpandItem_info.cost_item[1].itemindex = globaldata_getgshopitemindex_by_id(nItem2ID);
				m_ExpandItem_info.cost_item[1].itemcnt = m_nItem2Num - nItem2TotalNum;
				m_ExpandItem_info.index = 1;
				nMoney1 = (*items)[m_ExpandItem_info.cost_item[1].itemindex].buy[0].price * m_ExpandItem_info.cost_item[1].itemcnt / 100;
			}

 			if(type)
			{
				if(m_ExpandItem_info.iCount == 2)
				{
					nMoney = nMoney0 + nMoney1;
					strText.Format(GetStringFromTable(18702), nMoney, m_ExpandItem_info.cost_item[0].itemcnt, m_ExpandItem_info.cost_item[1].itemcnt);
				}
				else if(m_ExpandItem_info.iCount == 1)
				{
					if(m_nItem1Num > nItem1TotalNum)
					{
						nMoney = nMoney0;
						strText.Format(GetStringFromTable(18700), nMoney, m_ExpandItem_info.cost_item[0].itemcnt);
					}
					else if(m_nItem2Num > nItem2TotalNum)
					{
						nMoney = nMoney1;
						strText.Format(GetStringFromTable(18701), nMoney, m_ExpandItem_info.cost_item[1].itemcnt);
					}
				}
			}
			else
			{
				if(m_ExpandItem_info.iCount == 2)
				{
					nMoney = nMoney0 + nMoney1;
					strText.Format(GetStringFromTable(18705), nMoney, m_ExpandItem_info.cost_item[0].itemcnt, m_ExpandItem_info.cost_item[1].itemcnt);
				}
				else if(m_ExpandItem_info.iCount == 1)
				{
					if(m_nItem1Num > nItem1TotalNum)
					{
						nMoney = nMoney0;
						strText.Format(GetStringFromTable(18703), nMoney, m_ExpandItem_info.cost_item[0].itemcnt);
					}
					else if(m_nItem2Num > nItem2TotalNum)
					{
						nMoney = nMoney1;
						strText.Format(GetStringFromTable(18704), nMoney, m_ExpandItem_info.cost_item[1].itemcnt);
					}
				}
			}
			m_ExpandItem_info.iMoney = nMoney;
			GetGameUIMan()->MessageBox("Inventory_Expand", strText,
					MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			if(pMsgBox)
				pMsgBox->SetDataPtr((void*)&m_ExpandItem_info, "Expand");			
		}
	}

}

void CDlgInventoryExPand::OnCommandAdd(const char * szCommand)
{
	ACHAR szText[40];
	int nNum = a_atoi(m_pEdit_Input->GetText());
	if(nNum < INVENTORY_SIZEMAX)
	{
		for(int i = 0; i < m_vecInventoryNum.size(); i++)
		{
			if(nNum < m_vecInventoryNum[i])
			{
				nNum = m_vecInventoryNum[i];
				a_sprintf(szText, _AL("%d"), nNum);
				m_pEdit_Input->SetText(szText);
				break;
			}
		}
	}
}

void CDlgInventoryExPand::OnCommandMinus(const char * szCommmand)
{
	ACHAR szText[40];
	int nNum = a_atoi(m_pEdit_Input->GetText());
	int nSize;
	int nMin;
	if(m_szName == "Win_Inventory_Expand02")
	{
		nMin = 60;
		nSize = GetHostPlayer()->GetPack()->GetSize();
	}
	else if(m_szName == "Win_Inventory_Expand01")
	{
		nMin = 36;
		nSize = GetHostPlayer()->GetTrashBox()->GetSize();
	}
	if(nNum > nSize && nNum >= nMin)
	{
		for(int i = m_vecInventoryNum.size() - 1; i >=0; i--)
			if(nNum > m_vecInventoryNum[i])
			{
				nNum = m_vecInventoryNum[i];
				a_sprintf(szText, _AL("%d"), nNum);
				m_pEdit_Input->SetText(szText);
				break;
			}
	}
}

void CDlgInventoryExPand::CalcExpandInventory(int nSize, int nNum)
{
	m_nItem1Num = -1;
	m_nItem2Num = -1;
	ACHAR szText[40];
	if(nSize < INVENTORY_SIZEMAX && nNum >= nSize && nNum <= INVENTORY_SIZEMAX)
	{
		if(nSize < 60 && nNum <= 96)
			m_nItem1Num = (nNum - 60) / 12 + 1;
		else if(nSize < 60 && nNum > 96)
		{
			m_nItem1Num = 4;
			m_nItem2Num = (nNum - 102) / 6 + 1;
		}
		else if(nSize >= 60 && nSize <= 96 && nNum <= 96)
			m_nItem1Num = (nNum - nSize) / 12;
		else if(nSize >= 60 && nSize <=96 && nNum > 96)
		{
			m_nItem1Num = (96 - nSize) / 12;
			m_nItem2Num = (nNum - 102) / 6 + 1;
		}
		else if(nSize > 96)
			m_nItem2Num = (nNum - nSize) / 6;
		
		if(m_nItem1Num > 0)
		{
			a_sprintf(szText, _AL("%d"), m_nItem1Num);
			m_pTxt_Item1Num->SetText(szText);
		}
		else 
			m_pTxt_Item1Num->SetText(_AL("0"));

		if(m_nItem2Num > 0)
		{
			a_sprintf(szText, _AL("%d"), m_nItem2Num);
			m_pTxt_Item2Num->SetText(szText);
		}
		else
			m_pTxt_Item2Num->SetText(_AL("0"));
	}
	else
	{
		m_pTxt_Item1Num->SetText(_AL("0"));
		m_pTxt_Item2Num->SetText(_AL("0"));
	}
}

void CDlgInventoryExPand::CalcExpandStorage(int nSize, int nNum)
{
	m_nItem1Num = -1;
	m_nItem2Num = -1;
	ACHAR szText[40];
	if(nSize < INVENTORY_SIZEMAX && nNum >= nSize && nNum <= INVENTORY_SIZEMAX)
	{
		if(nSize < 36 && nNum <= 96)
			m_nItem1Num = (nNum - 36) / 12 + 1;
		else if(nSize < 36 && nNum > 96)
		{
			m_nItem1Num = 6;
			m_nItem2Num = (nNum - 102) / 6 + 1;
		}
		else if(nSize >= 36 && nSize <= 96 && nNum <= 96)
			m_nItem1Num = (nNum - nSize) / 12;
		else if(nSize >= 36 && nSize <=96 && nNum > 96)
		{
			m_nItem1Num = (96 - nSize) / 12;
			m_nItem2Num = (nNum - 102) / 6 + 1;
		}
		else if(nSize > 96)
			m_nItem2Num = (nNum - nSize) / 6;
		
		if(m_nItem1Num > 0)
		{
			a_sprintf(szText, _AL("%d"), m_nItem1Num);
			m_pTxt_Item1Num->SetText(szText);
		}
		else 
			m_pTxt_Item1Num->SetText(_AL("0"));

		if(m_nItem2Num > 0)
		{
			a_sprintf(szText, _AL("%d"), m_nItem2Num);
			m_pTxt_Item2Num->SetText(szText);
		}
		else
			m_pTxt_Item2Num->SetText(_AL("0"));
	}
	else
	{
		m_pTxt_Item1Num->SetText(_AL("0"));
		m_pTxt_Item2Num->SetText(_AL("0"));
	}
}