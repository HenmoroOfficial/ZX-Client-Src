// Filename	: DlgQProduceTool.cpp
// Creator	: WuWeixin
// Date		: 2013/3/6

#include "DlgQProduceTool.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUIDef.h"
#include "elementdataman.h"
#include "ExpTypes.h"
#include "EC_Global.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_IvtrConsume.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "DlgInventory.h"
#include "EC_Algorithm.h"
#include "EC_GameSession.h"
#include "EC_GPDataType.h"


#define JINFASHEN_TO_MONEY_CONFIG_ID  748
#define PACK_SIZE 48
#define PACK_COL  6
#define PACK_ROW  8
#define TYPE_UNDEF    -1
#define TYPE_JINESHEN 0
#define TYPE_FASHEN   1


//////////////////////////////////////////////////////////////////////////

AUI_BEGIN_COMMAND_MAP(CDlgQProduceTool, CDlgBase)
AUI_ON_COMMAND("top",				OnCommand_Top)
AUI_ON_COMMAND("bottom",			OnCommand_Bottom)
AUI_ON_COMMAND("prec",				OnCommand_Prec)
AUI_ON_COMMAND("next",				OnCommand_Next)
AUI_ON_COMMAND("Btn_Min01",			OnCommand_Min1)
AUI_ON_COMMAND("Btn_Min02",			OnCommand_Min2)
AUI_ON_COMMAND("Btn_Min03",			OnCommand_Min3)
AUI_ON_COMMAND("Btn_Max01",			OnCommand_Max1)
AUI_ON_COMMAND("Btn_Max02",			OnCommand_Max2)
AUI_ON_COMMAND("Btn_Max03",			OnCommand_Max3)
AUI_ON_COMMAND("Btn_MinMoneyItem",	OnCommand_MinMoneyItem)
AUI_ON_COMMAND("Btn_MaxMoneyItem",	OnCommand_MaxMoneyItem)
AUI_ON_COMMAND("Btn_Input02",		OnCommand_SelectAll)
AUI_ON_COMMAND("Btn_Input01",		OnCommand_CancelAll)
AUI_ON_COMMAND("Btn_Cancel",		OnCommand_Close)
AUI_ON_COMMAND("Btn_Confirm",		OnCommand_Ok)
AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgQProduceTool, CDlgBase)
AUI_ON_EVENT("*",			WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,			WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT("Item_*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_Hover*",	WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Tv_1",		WM_LBUTTONDOWN,		OnEventLButtonDown_Tv_Lst)
AUI_END_EVENT_MAP()


CDlgQProduceTool::CDlgQProduceTool()
{
}

CDlgQProduceTool::~CDlgQProduceTool()
{
}

bool CDlgQProduceTool::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	m_idProduceItem = 0;
	m_nProduceType = TYPE_UNDEF;

	m_pScl_Item = (AUIScroll*)GetDlgItem("Scl_Item");
	m_pProduceList = (AUITreeView*)GetDlgItem("Tv_1");
	m_pNowCredit01 = (AUILabel*)GetDlgItem("Txt_NowCredit01");
	m_pNowCredit02 = (AUILabel*)GetDlgItem("Txt_NowCredit02");
	m_pNowCredit03 = (AUILabel*)GetDlgItem("Txt_NowCredit03");
	m_pNowMoneyItem = (AUILabel*)GetDlgItem("Txt_NowMoneyItem");
	m_pInput01 = (AUIEditBox*)GetDlgItem("Edt_Input01");
	m_pInput02 = (AUIEditBox*)GetDlgItem("Edt_Input02");
	m_pInput03 = (AUIEditBox*)GetDlgItem("Edt_Input03");
	m_pInputMoneyItem = (AUIEditBox*)GetDlgItem("Edt_InputMoneyItem");
	m_pReplaceZhaohua = (AUILabel*)GetDlgItem("Txt_Credit01");
	m_pNeedZhaohua = (AUILabel*)GetDlgItem("Txt_Credit02");
	m_pReplaceMoney1 = (AUILabel*)GetDlgItem("Txt_Money1");
	m_pNeedMoney1 = (AUILabel*)GetDlgItem("Txt_Money4");
	char szItem[40];
	for (int i = 0; i < 48; ++ i)
	{
		sprintf(szItem, "Item_%02d", i);
		m_pImgItems[i] = (AUIImagePicture*)GetDlgItem(szItem);
		sprintf(szItem, "Img_Hover%02d", i);
		m_pImgHovers[i] = (AUIImagePicture*)GetDlgItem(szItem);
		m_pImgHovers[i]->Show(false);
		m_pImgHovers[i]->SetTransparent(true);
	}

	// 初始化生产列表
	PAUITREEVIEW pTree = m_pProduceList;
	pTree->DeleteAllItems();
	pTree->SetText(GetStringFromTable(17720));
	P_AUITREEVIEW_ITEM pTreeItem1 = pTree->InsertItem(GetStringFromTable(17721));
	P_AUITREEVIEW_ITEM pTreeItem2 = pTree->InsertItem(GetStringFromTable(17722));
	P_AUITREEVIEW_ITEM pTreeItem = NULL;

	elementdataman* pDataMan = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	const EQUIPMENT_ESSENCE* pEssence = NULL;
	const void* pData = pDataMan->get_data_ptr(JINFASHEN_TO_MONEY_CONFIG_ID, ID_SPACE_CONFIG, DataType);
	if (pData && DataType == DT_JINFASHEN_TO_MONEY_CONFIG)
	{
		JINFASHEN_TO_MONEY_CONFIG* pConfig = (JINFASHEN_TO_MONEY_CONFIG*)pData;
		const int MAX_COUNT = 25;   // 每类最多有25个
		const int MAX_PRODUCE = 13; // 每类能生产的有13个
		int id;
		for (int i = 0; i < MAX_COUNT; ++ i)
		{
			id = pConfig->jinshen[i].id;
			if (id != 0)
			{
				JinFaShenMoneyConfig cfg;
				cfg.index = i;
				cfg.money = pConfig->jinshen[i].money;
				cfg.reputation = pConfig->jinshen[i].reputation;
				m_JinShenConfigs[id] = cfg;
				if (i < MAX_PRODUCE)
				{
					pData = pDataMan->get_data_ptr(id, ID_SPACE_ESSENCE, DataType);
					if (pData)
					{
						pEssence = (const EQUIPMENT_ESSENCE*)pData;
						pTreeItem = pTree->InsertItem(pEssence->name, pTreeItem1);
						pTree->SetItemData(pTreeItem, TYPE_JINESHEN);
						pTree->SetItemData64(pTreeItem, id);
					}
				}
			}

			id = pConfig->fashen[i].id; 
			if (id != 0)
			{
				JinFaShenMoneyConfig cfg;
				cfg.index = i;
				cfg.money = pConfig->fashen[i].money;
				cfg.reputation = pConfig->fashen[i].reputation;
				m_FaShenConfigs[id] = cfg;
				if (i < MAX_PRODUCE)
				{
					pData = pDataMan->get_data_ptr(id, ID_SPACE_ESSENCE, DataType);
					if (pData)
					{
						pEssence = (const EQUIPMENT_ESSENCE*)pData;
						pTreeItem = pTree->InsertItem(pEssence->name, pTreeItem2);
						pTree->SetItemData(pTreeItem, TYPE_FASHEN);
						pTree->SetItemData64(pTreeItem, id);
					}
				}
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	return true;
}

bool CDlgQProduceTool::Tick(void)
{
	CDlgBase::Tick();
/*
	int maxVal = MaxZhaohua(0);
	ACString szText = m_pInput01->GetText();
	if (maxVal < szText.ToInt())
	{
		szText.Format(_AL("%d"), maxVal);
		m_pInput01->SetText(szText);
	}
	
	maxVal = MaxZhaohua(1);
	szText = m_pInput02->GetText();
	if (maxVal < szText.ToInt())
	{
		szText.Format(_AL("%d"), maxVal);
		m_pInput02->SetText(szText);
	}

	
	maxVal = MaxZhaohua(2);
	szText = m_pInput03->GetText();
	if (maxVal < szText.ToInt())
	{
		szText.Format(_AL("%d"), maxVal);
		m_pInput03->SetText(szText);
	}
*/
	//UpdateJinFaShen();
	UpdateView();

	return true;
}

void CDlgQProduceTool::OnShowDialog()
{
	m_nNeedMoney = 0;
	m_nNeedZhaohua = 0;
	ClearNumber();
	UpdateJinFaShen();
	UpdateView();
}

void CDlgQProduceTool::ClearNumber()
{
	m_nCostMoneyItem = 0;
	m_nCostZhaohua[0] = 0;
	m_nCostZhaohua[1] = 0;
	m_nCostZhaohua[2] = 0;
	m_pInputMoneyItem->SetText(_AL(""));
	m_pInput01->SetText(_AL(""));
	m_pInput02->SetText(_AL(""));
	m_pInput03->SetText(_AL(""));
}

void CDlgQProduceTool::OnHideDialog()
{
	FreezeAll(false);
	CancelSelect();
	GetGameUIMan()->EndNPCService();
}

// 更新包裹金身法身
void CDlgQProduceTool::UpdateJinFaShen()
{
	if (!IsShow())
		return;

	FreezeAll(false);
	m_IvtrJinShen.clear();
	m_IvtrFaShen.clear();

	CECInventory* pInventory = GetHostPlayer()->GetPack();
	CECIvtrItem* pItem;
	int size = pInventory->GetSize();
	for (int i = 0; i < size; ++ i)
	{
		pItem = pInventory->GetItem(i);
		if (!pItem || pItem->GetClassID() != CECIvtrItem::ICID_EQUIP)
			continue;
		if (!pItem->CanEquippedTo(EQUIPIVTR_FASHION_LIP))
			continue;
		if (pItem->GetExpireDate() != 0) // 限时物品不参与抵扣
			continue;

		JinFaShen item;
		item.id = pItem->GetTemplateID();
		item.slot = i;
		item.select = false;

		if (m_JinShenConfigs.find(item.id) != m_JinShenConfigs.end())
		{
			item.index = m_JinShenConfigs[item.id].index;
			m_IvtrJinShen.push_back(item);
		}
		else if (m_FaShenConfigs.find(item.id) != m_FaShenConfigs.end())
		{
			item.index = m_FaShenConfigs[item.id].index;
			m_IvtrFaShen.push_back(item);
		}
	}

	FreezeAll(true);

	BubbleSort(m_IvtrJinShen.begin(), m_IvtrJinShen.end(), JinFaShen::CompareListByIdAndSlot());
	BubbleSort(m_IvtrFaShen.begin(), m_IvtrFaShen.end(), JinFaShen::CompareListByIdAndSlot());


	m_pScl_Item->SetScrollRange(0, (m_IvtrJinShen.size() + m_IvtrFaShen.size() - 1) / PACK_COL - PACK_SIZE / PACK_COL + 1);
	m_pScl_Item->Show(true);
	m_pScl_Item->SetBarLength(-1);
	m_pScl_Item->SetScrollStep(1);
	m_nFirstLine = 0;
}

// 更新显示
void CDlgQProduceTool::UpdateView()
{
	if (!IsShow())
		return;
	
	if( m_nFirstLine != m_pScl_Item->GetBarLevel() )
		m_nFirstLine = m_pScl_Item->GetBarLevel();
	int nStart = m_nFirstLine * PACK_COL;
	unsigned int iIvtr;
	unsigned int iImg = 0;
	unsigned int len = m_IvtrJinShen.size();
	AUIImagePicture* pImg = NULL;
	JinFaShen* pJinFaShen = NULL;
	CECIvtrItem* pItem;
	CECInventory* pInventory = GetHostPlayer()->GetPack();

	for (iIvtr = nStart; iIvtr < len && iImg < 48; ++ iIvtr, ++ iImg)
	{
		pJinFaShen = &m_IvtrJinShen[iIvtr];
		pItem = pInventory->GetItem(pJinFaShen->slot);
		
		pImg = m_pImgItems[iImg];
		pImg->SetData(TYPE_JINESHEN);
		pImg->SetData64(iImg);
		GetGameUIMan()->SetIvtrInfo(pImg, pItem);
		m_pImgHovers[iImg]->Show(pJinFaShen->select);
	}

	iIvtr -= len;
	len = m_IvtrFaShen.size();
	for (; iImg < PACK_SIZE && iIvtr < len; ++ iIvtr, ++ iImg)
	{
		pJinFaShen = &m_IvtrFaShen[iIvtr];
		pItem = pInventory->GetItem(pJinFaShen->slot);
		
		pImg = m_pImgItems[iImg];
		pImg->SetData(TYPE_FASHEN);
		pImg->SetData64(iImg);
		GetGameUIMan()->SetIvtrInfo(pImg, pItem);
		m_pImgHovers[iImg]->Show(pJinFaShen->select);
	}

	for (; iImg < PACK_SIZE; ++ iImg)
	{
		pImg = m_pImgItems[iImg];
		pImg->SetData(TYPE_UNDEF);
		pImg->SetData64(-1);
		GetGameUIMan()->SetIvtrInfo(pImg, NULL);
		m_pImgHovers[iImg]->Show(false);
	}

	UpdateCost();
	UpdateZhaohua();
	UpdateMoneyItem();
}

void CDlgQProduceTool::FreezeAll(bool bFreeze)
{
	CECInventory* pInventory = GetHostPlayer()->GetPack();
	CECIvtrItem* pItem = NULL;

	abase::vector<JinFaShen>::iterator it = m_IvtrJinShen.begin();
	abase::vector<JinFaShen>::iterator ite = m_IvtrJinShen.end();
	for (; it != ite; ++ it)
	{
		if (pItem = pInventory->GetItem(it->slot))
			pItem->Freeze(bFreeze);
	}

	it = m_IvtrFaShen.begin();
	ite = m_IvtrFaShen.end();
	for (; it != ite; ++ it)
	{
		if (pItem = pInventory->GetItem(it->slot))
			pItem->Freeze(bFreeze);
	}
}

// 取消所有选择
void CDlgQProduceTool::CancelSelect()
{
	abase::vector<JinFaShen>::iterator it = m_IvtrJinShen.begin();
	abase::vector<JinFaShen>::iterator ite = m_IvtrJinShen.end();
	for (; it != ite; ++ it)
		Select(*it, false);

	it = m_IvtrFaShen.begin();
	ite = m_IvtrFaShen.end();
	for (; it != ite; ++ it)
		Select(*it, false);
}

// 选择或取消一个金身、法身, bUpdateCost用于控制是否重新计算消耗和抵扣数值
void CDlgQProduceTool::Select(JinFaShen& jinFaShen, bool bSelect)
{
	CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(jinFaShen.slot);
	if (!pItem)
	{
		ASSERT(pItem);
		return;
	}
	
	jinFaShen.select = bSelect;
	UpdateView();
}

// 更新消耗和抵扣数值
void CDlgQProduceTool::UpdateCost()
{
	const JinFaShenMoneyConfig* pProduceConfig = GetCurrentProduceConfig();
	if (pProduceConfig)
	{
		ACString szText;
		int replaceMoney, replaceZhaoHua;
		GetJinFaShenMoneyAndValue(replaceMoney, replaceZhaoHua);

		m_nNeedMoney = max(0, pProduceConfig->money - replaceMoney);
		m_nNeedZhaohua = max(0, pProduceConfig->reputation - replaceZhaoHua);

		szText.Format(_AL("%d"), m_nNeedZhaohua - m_nCostZhaohua[0] - m_nCostZhaohua[1] - m_nCostZhaohua[2]);
		m_pNeedZhaohua->SetText(szText);

		szText.Format(_AL("%d"), m_nNeedMoney - m_nCostMoneyItem * 5000);
		m_pNeedMoney1->SetText(szText);

		szText.Format(_AL("%d"), replaceZhaoHua + m_nCostZhaohua[0] + m_nCostZhaohua[1] + m_nCostZhaohua[2]);
		m_pReplaceZhaohua->SetText(szText);
		
		szText.Format(_AL("%d"), replaceMoney + m_nCostMoneyItem * 5000);
		m_pReplaceMoney1->SetText(szText);

		return;
	}

	OnCommand_Min1(NULL);
	OnCommand_Min2(NULL);
	OnCommand_Min3(NULL);

	m_nNeedMoney = 0;
	m_nNeedZhaohua = 0;
	m_pNeedZhaohua->SetText(_AL(""));
	m_pNeedMoney1->SetText(_AL(""));
	m_pReplaceZhaohua->SetText(_AL(""));
	m_pReplaceMoney1->SetText(_AL(""));
}

// 更新当前身上的造化值
void CDlgQProduceTool::UpdateZhaohua()
{
	ACString szText;
	int num;
	szText.Format(_AL("%d"), GetHostPlayer()->GetRegionReputation(9));
	m_pNowCredit01->SetText(szText);

	szText.Format(_AL("%d"), GetHostPlayer()->GetRegionReputation(10));
	m_pNowCredit02->SetText(szText);

	szText.Format(_AL("%d"), GetHostPlayer()->GetRegionReputation(11));
	m_pNowCredit03->SetText(szText);

	szText = m_pInput01->GetText();
	num = szText.ToInt();
	if( num != m_nCostZhaohua[0] )
	{
		m_nCostZhaohua[0] = num;
		if( m_nCostZhaohua[0] > MaxZhaohua(0) )
			OnCommand_Max1(NULL);
	}

	szText = m_pInput02->GetText();
	num = szText.ToInt();
	if( num != m_nCostZhaohua[1] )
	{
		m_nCostZhaohua[1] = num;
		if( m_nCostZhaohua[1] > MaxZhaohua(1) )
			OnCommand_Max2(NULL);
	}

	szText = m_pInput03->GetText();
	num = szText.ToInt();
	if( num != m_nCostZhaohua[2] )
	{
		m_nCostZhaohua[2] = num;
		if( m_nCostZhaohua[2] > MaxZhaohua(2) )
			OnCommand_Max3(NULL);
	}
}

// 更新当前身上的5000金物品数量
void CDlgQProduceTool::UpdateMoneyItem()
{
	ACString szText;
	szText.Format(_AL("%d"), GetHostPlayer()->GetPack()->GetItemTotalNum(19681));
	m_pNowMoneyItem->SetText(szText);

	szText = m_pInputMoneyItem->GetText();
	int num = szText.ToInt();
	if( num != m_nCostMoneyItem )
	{
		m_nCostMoneyItem = num;
		if( m_nCostMoneyItem > MaxMoneyItem() )
			OnCommand_MaxMoneyItem(NULL);
	}

}

// 获取当前选中的金身、法身的抵扣值
void CDlgQProduceTool::GetJinFaShenMoneyAndValue(int& money, int& zhaohua)
{
	money = 0;
	zhaohua = 0;

	JinFaShenMoneyConfig* pConfig = NULL;
	abase::hash_map<int, JinFaShenMoneyConfig>::iterator it_cfg;
	abase::vector<JinFaShen>::iterator it = m_IvtrJinShen.begin();
	abase::vector<JinFaShen>::iterator ite = m_IvtrJinShen.end();
	for (; it != ite; ++ it)
	{
		JinFaShen& jinFaShen = *it;
		if (!jinFaShen.select)
			continue;

		it_cfg = m_JinShenConfigs.find(jinFaShen.id);
		if (it_cfg != m_JinShenConfigs.end())
		{
			pConfig = &(it_cfg->second);
			money += pConfig->money;
			zhaohua += pConfig->reputation;
		}
	}

	it = m_IvtrFaShen.begin();
	ite = m_IvtrFaShen.end();
	for (; it != ite; ++ it)
	{
		JinFaShen& jinFaShen = *it;
		if (!jinFaShen.select)
			continue;

		it_cfg = m_FaShenConfigs.find(jinFaShen.id);
		if (it_cfg != m_FaShenConfigs.end())
		{
			pConfig = &(it_cfg->second);
			money += pConfig->money;
			zhaohua += pConfig->reputation;
		}
	}
}

// 获取当前要生产的配置
const CDlgQProduceTool::JinFaShenMoneyConfig* CDlgQProduceTool::GetCurrentProduceConfig()
{
	if (m_nProduceType != TYPE_UNDEF)
	{
		const abase::hash_map<int, JinFaShenMoneyConfig>* pProduceConfigs = &(m_nProduceType == TYPE_JINESHEN ? m_JinShenConfigs : m_FaShenConfigs);
		abase::hash_map<int, JinFaShenMoneyConfig>::const_iterator it = pProduceConfigs->find(m_idProduceItem);
		if (it != pProduceConfigs->end())
			return &(it->second);
	}
	return NULL;
}

// 造化值max 0:仙 1:魔 2:佛
int CDlgQProduceTool::MaxZhaohua(int type)
{
	if (type < 0 || type >= 3)
	{
		ASSERT(false);
		return 0;
	}

	static const int ZHAOHUA_ID[3] = {9, 10, 11};
	int maxZhaohua = min(m_nNeedZhaohua - m_nCostZhaohua[0] - m_nCostZhaohua[1] - m_nCostZhaohua[2] + m_nCostZhaohua[type],
							GetHostPlayer()->GetRegionReputation(ZHAOHUA_ID[type]));
	return maxZhaohua;
}

int CDlgQProduceTool::MaxMoneyItem()
{
	int maxMoneyItem = min(m_nNeedMoney / 5000,
							GetHostPlayer()->GetPack()->GetItemTotalNum(19681));
	return maxMoneyItem;
}

void CDlgQProduceTool::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	int zDelta = (short)HIWORD(wParam);
	if( zDelta > 0 )
		zDelta = 1;
	else
		zDelta = -1;
	if( m_pScl_Item->IsShow() )
		m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() - zDelta);
}

void CDlgQProduceTool::OnCommand_Top(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(0);
}

void CDlgQProduceTool::OnCommand_Bottom(const char * szCommand)
{
	int count = m_IvtrFaShen.size() + m_IvtrJinShen.size();
	m_pScl_Item->SetBarLevel(count / 3 - 2);
}

void CDlgQProduceTool::OnCommand_Prec(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() - 3);
}

void CDlgQProduceTool::OnCommand_Next(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() + 3);
}

void CDlgQProduceTool::OnCommand_Min1(const char * szCommand)
{
	m_nCostZhaohua[0] = 0;
	m_pInput01->SetText(_AL(""));
}

void CDlgQProduceTool::OnCommand_Min2(const char * szCommand)
{
	m_nCostZhaohua[1] = 0;
	m_pInput02->SetText(_AL(""));
}

void CDlgQProduceTool::OnCommand_Min3(const char * szCommand)
{
	m_nCostZhaohua[2] = 0;
	m_pInput03->SetText(_AL(""));
}

void CDlgQProduceTool::OnCommand_Max1(const char * szCommand)
{
	int val = MaxZhaohua(0);
	ACString szText;
	szText.Format(_AL("%d"), val);
	m_pInput01->SetText(szText);
	m_nCostZhaohua[0] = val;
}

void CDlgQProduceTool::OnCommand_Max2(const char * szCommand)
{
	int val = MaxZhaohua(1);
	ACString szText;
	szText.Format(_AL("%d"), val);
	m_pInput02->SetText(szText);
	m_nCostZhaohua[1] = val;
}

void CDlgQProduceTool::OnCommand_Max3(const char * szCommand)
{
	int val = MaxZhaohua(2);
	ACString szText;
	szText.Format(_AL("%d"), val);
	m_pInput03->SetText(szText);
	m_nCostZhaohua[2] = val;
}

void CDlgQProduceTool::OnCommand_MinMoneyItem(const char * szCommand)
{
	m_nCostMoneyItem = 0;
	m_pInputMoneyItem->SetText(_AL(""));
}

void CDlgQProduceTool::OnCommand_MaxMoneyItem(const char * szCommand)
{
	int val = MaxMoneyItem();
	ACString szText;
	szText.Format(_AL("%d"), val);
	m_pInputMoneyItem->SetText(szText);
	m_nCostMoneyItem = val;
}


void CDlgQProduceTool::OnCommand_SelectAll(const char * szCommand)
{
	abase::vector<JinFaShen>::iterator it = m_IvtrJinShen.begin();
	abase::vector<JinFaShen>::iterator ite = m_IvtrJinShen.end();
	for (; it != ite; ++ it)
		Select(*it, true);

	it = m_IvtrFaShen.begin();
	ite = m_IvtrFaShen.end();
	for (; it != ite; ++ it)
		Select(*it, true);

	UpdateView();
}

void CDlgQProduceTool::OnCommand_CancelAll(const char * szCommand)
{
	CancelSelect();
	UpdateView();
}

void CDlgQProduceTool::OnCommand_Close(const char * szCommand)
{
	Show(false);
}

void CDlgQProduceTool::OnCommand_Ok(const char * szCommand)
{
	const JinFaShenMoneyConfig* pProduceConfig = GetCurrentProduceConfig();
	if (!pProduceConfig)
	{
		GetGameUIMan()->MessageBox(17723);
		return;
	}

	int replaceMoney = 0;
	int replaceZhaohua = 0;
	GetJinFaShenMoneyAndValue(replaceMoney, replaceZhaohua);

	int totalMoney = m_nCostMoneyItem * 5000 + replaceMoney;
	int totalZhaohua = replaceZhaohua+ m_nCostZhaohua[0] + m_nCostZhaohua[1] + m_nCostZhaohua[2];

	if (GetHostPlayer()->GetMoneyAmount() / 10000 + m_nCostMoneyItem * 5000 < m_nNeedMoney)
	{
		GetGameUIMan()->MessageBox(17724);
		return;
	}
	if (totalMoney > pProduceConfig->money)
	{
		GetGameUIMan()->MessageBox(17726);
		return;
	}
	if (totalZhaohua < pProduceConfig->reputation)
	{
		GetGameUIMan()->MessageBox(17725);
		return;
	}
	if (totalZhaohua > pProduceConfig->reputation)
	{
		GetGameUIMan()->MessageBox(17726);
		return;
	}

	abase::vector<C2S::npc_produce_jinfashen_material> materials;
	unsigned int i = 0;
	unsigned int len = m_IvtrJinShen.size();
	for (; i < len; ++ i)
	{
		const JinFaShen& jinFaShen = m_IvtrJinShen[i];
		if (!jinFaShen.select)
			continue;

		C2S::npc_produce_jinfashen_material material;
		material.config_idx = jinFaShen.index;
		material.item_id = jinFaShen.id;
		material.item_idx = jinFaShen.slot;
		material.type = TYPE_JINESHEN;
		materials.push_back(material);
	}

	i = 0;
	len = m_IvtrFaShen.size();
	for (; i < len; ++ i)
	{
		const JinFaShen& jinFaShen = m_IvtrFaShen[i];
		if (!jinFaShen.select)
			continue;

		C2S::npc_produce_jinfashen_material material;
		material.config_idx = jinFaShen.index;
		material.item_id = jinFaShen.id;
		material.item_idx = jinFaShen.slot;
		material.type = TYPE_FASHEN;
		materials.push_back(material);
	}

	g_pGame->GetGameSession()->c2s_CmdNPCSevProduceJinFaShen(
				(char)m_nProduceType, 
				(char)pProduceConfig->index,
				m_nCostZhaohua,
				m_nCostMoneyItem,
				(short)materials.size(),
				materials.begin());

	Show(false);
}

void CDlgQProduceTool::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	UINT64 temp = pObj->GetData64();
	int index = (int)temp;
	if (index < 0)
		return;

	index += m_nFirstLine * PACK_COL;
	
	JinFaShen* pJinFaShen = NULL;
	if (index < (int)m_IvtrJinShen.size())
		pJinFaShen = &m_IvtrJinShen[index];
	else
	{
		index -= m_IvtrJinShen.size();
		if (index < (int)m_IvtrFaShen.size())
			pJinFaShen = &m_IvtrFaShen[index];
	}

	if (!pJinFaShen)
		return;

	Select(*pJinFaShen, !pJinFaShen->select);
	ClearNumber();
}

void CDlgQProduceTool::OnEventLButtonDown_Tv_Lst(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	P_AUITREEVIEW_ITEM pItem = m_pProduceList->GetSelectedItem();
	if (pItem && m_pProduceList->GetParentItem(pItem) != m_pProduceList->GetRootItem())
	{
		UINT64 temp = m_pProduceList->GetItemData64(pItem);
		int id = (int)temp;
		if (id != m_idProduceItem)
		{
			m_idProduceItem = id;
			m_nProduceType = m_pProduceList->GetItemData(pItem);
			OnCommand_CancelAll(NULL);			
			ClearNumber();
			//UpdateCost();
		}
		return;
	}
	m_idProduceItem = 0;
	m_nProduceType = 0;
	//UpdateCost();
}

