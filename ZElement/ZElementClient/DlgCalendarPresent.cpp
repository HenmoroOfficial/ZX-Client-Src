// Filename	: DlgCalendarPresent.cpp
// Creator	: WuWeixin
// Date		: 2012/21/21

#include "DlgCalendarPresent.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUISubDialog.h"
#include "AUI\\AUIDef.h"
#include "AIniFile.h"
#include "EC_IvtrEquip.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_GameUIMan.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_IvtrConsume.h"
#include "TaskTemplMan.h"
#include "TaskTempl.h"
#include "EC_Achievement.h"

const int MAX_ELEMENT_COUNT = 20; // 模板中配置的最大台历项数目

/*
对直接领取（0+0）
前提任务（任务id）
前提成就（成就id）
消耗元宝（数量）
消耗虚拟币（数量）
消耗物品（物品id+数量）
区域友好度（区域id+友好度值）
网页抽奖
*/
enum SalePromotionItemCondition           
{           
    COND_TYPE_DIRECT = 0,           // 直接给奖励
    COND_TYPE_PRE_TASK,             // 前提任务
    COND_TYPE_PRE_ACHIEVEMENT,      // 前提成就
    COND_TYPE_USE_INGOT,            // 消耗元宝
    COND_TYPE_USE_MONEY,            // 消耗虚拟币
    COND_TYPE_USE_ITEM,             // 消耗物品
    COND_TYPE_USE_AREA_YHD,         // 消耗区域友好度
	COND_TYPE_LOTTERY,				// 参与网页抽奖
}; 

// 条件辅助类
class SalePromotionConditionUtil
{
public:
	SalePromotionConditionUtil(): m_nTaskCount(0), m_nAchievementCount(0), m_pTaskIds(NULL) {}
	~SalePromotionConditionUtil() { delete m_pTaskIds; }

	// 收到协议更新条件
	void UpdateCondition(const S2C::sale_promotion_info* pInfo)
	{
		int originLen = m_nTaskCount;
		m_nTaskCount = pInfo->task_count;
		if (originLen != m_nTaskCount)
		{
			delete m_pTaskIds;
			m_pTaskIds = new int[m_nTaskCount];
		}
		if (m_pTaskIds)
			memcpy(m_pTaskIds, pInfo->data, m_nTaskCount*sizeof(int));
	}

	// 设置条件字符串
	void SetConditionText(AUILabel* pTxtCond, int cond, int arg1, int arg2) const
	{
		PAUIDIALOG pDlg = pTxtCond->GetParent();
		if (!pDlg)
			return;
		
		ACString strText = _AL("");
		switch(cond)
		{
		case COND_TYPE_DIRECT:
			strText.Format(pDlg->GetStringFromTable(17400));
			break;
		case COND_TYPE_PRE_TASK:
			{
				ATaskTemplMan *pMan = g_pGame->GetTaskTemplateMan();
				if (ATaskTempl* pTask = pMan->GetTaskTemplByID(arg1))
					strText.Format(pDlg->GetStringFromTable(17401), pTask->GetName());
			}
			break;
		case COND_TYPE_PRE_ACHIEVEMENT:
			{
				CECAchievementMan* pAchMan = g_pGame->GetGameRun()->GetHostPlayer()->GetAchievementMan();
				if (sAchievement* pAch = pAchMan->GetAchievement(arg1))
					strText.Format(pDlg->GetStringFromTable(17402), pAch->name);
			}
			break;
		case COND_TYPE_USE_INGOT:
			if (arg1 >= 100)
				strText.Format(pDlg->GetStringFromTable(17403), arg1/100);
			else 
				strText.Format(pDlg->GetStringFromTable(17406), arg1);
			break;
		case COND_TYPE_USE_MONEY:
			strText.Format(pDlg->GetStringFromTable(17404), arg1);
			break;
		case COND_TYPE_USE_ITEM:
			if (CECIvtrItem *pItem = CECIvtrItem::CreateItem(arg1, 0, 1))
			{
				strText.Format(pDlg->GetStringFromTable(17405), arg2, pItem->GetName());
				delete pItem;
			}
			break;
		case COND_TYPE_USE_AREA_YHD:
			strText.Format(pDlg->GetStringFromTable(17410+arg1), arg2);
			break;
		case COND_TYPE_LOTTERY:
			strText.Format(pDlg->GetStringFromTable(17407));
			break;
		default: break;
		}
		pTxtCond->SetText(strText);
	}

	// 判断条件是否完成
	bool IsConditionOk(int cond, int arg1, int arg2) const
	{
		CECHostPlayer* pHostPlayer = g_pGame->GetGameRun()->GetHostPlayer();
		switch(cond)
		{
		case COND_TYPE_DIRECT:			return true;
		case COND_TYPE_PRE_TASK:		return IsTaskFinish(arg1);
		case COND_TYPE_PRE_ACHIEVEMENT:	return IsAchievementFinish(arg1);
		case COND_TYPE_USE_INGOT:		return pHostPlayer->GetCash() >= arg1;
		case COND_TYPE_USE_MONEY:		return pHostPlayer->GetMoneyAmount() >= arg1;
		case COND_TYPE_USE_ITEM:		return pHostPlayer->GetPack()->GetItemTotalNum(arg1) >= arg2;
		case COND_TYPE_USE_AREA_YHD:	return pHostPlayer->GetRegionReputation(arg1) >= arg2;
		case COND_TYPE_LOTTERY:			return true;
		default: break;
		}
		return false;
	}

private:
	// 判断任务是否完成
	bool IsTaskFinish(int id) const
	{
		for (int i = 0; i < m_nTaskCount; ++ i)
		{
			if (m_pTaskIds[i] == id)
				return true;
		}
		return false;
	}

	// 判断成就是否完成
	bool IsAchievementFinish(int id) const
	{
		CECAchievementMan* pAchMan = g_pGame->GetGameRun()->GetHostPlayer()->GetAchievementMan();
		if (sAchievement* pAch = pAchMan->GetAchievement(id))
			return pAch->bFinish;
		return false;
	}

	int		m_nTaskCount;
	int		m_nAchievementCount;
	int*	m_pTaskIds;
};

////////////////////////////////////////////////////////////////////////////////////////
// CDlgCalendarPresent

AUI_BEGIN_COMMAND_MAP(CDlgCalendarItem, CDlgBase)
AUI_ON_COMMAND("Btn_Get",	OnCommand_Get)
AUI_ON_COMMAND("Btn_Get2",	OnCommand_Get2)
AUI_ON_COMMAND("Btn_Buy",	OnCommand_Get)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgCalendarItem, CDlgBase)
AUI_ON_EVENT("*",			WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,			WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_END_EVENT_MAP()





CDlgCalendarItem::CDlgCalendarItem()
: m_iSlot(-1), m_nEleIndex(-1), m_idItem(0), m_nUsedTimes(0)
{
}

CDlgCalendarItem::~CDlgCalendarItem()
{
}

bool CDlgCalendarItem::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	m_pTxt_Month = (AUILabel*)GetDlgItem("Txt_Month");
	m_pLab_Present = (AUILabel*)GetDlgItem("Lab_Present");
	m_pTxt_Times = (AUILabel*)GetDlgItem("Txt_1");
	m_pTxt_Cond = (AUILabel*)GetDlgItem("Txt_2");
	m_pTxt_Desc = (AUILabel*)GetDlgItem("Txt_3");
	m_pLab_PriceTitle = (AUILabel*)GetDlgItem("Lab_Price");
	m_pLab_Gets = (AUILabel*)GetDlgItem("Lab_Gets");
	m_pBtn_Get = (AUIStillImageButton*)GetDlgItem("Btn_Get");
	m_pBtn_Lottery = (AUIStillImageButton*)GetDlgItem("Btn_Get2");
	m_pBtn_Buy = (AUIStillImageButton*)GetDlgItem("Btn_Buy");
	m_pImg_Award1 = (AUIImagePicture*)GetDlgItem("Img_Award1");
	m_pImg_Award2 = (AUIImagePicture*)GetDlgItem("Img_Award2");
	m_pImg_Award3 = (AUIImagePicture*)GetDlgItem("Img_Award3");

	return true;
}

void CDlgCalendarItem::OnHideDialog()
{
	m_iSlot = -1;
	m_nEleIndex = -1;
	m_idItem = 0;
	m_nUsedTimes = 0;
}

void CDlgCalendarItem::OnTick()
{
	UpdateItemView();
}

void CDlgCalendarItem::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	CDlgCalendarPresent* pDlg = (CDlgCalendarPresent*)GetGameUIMan()->GetDialog("Win_CalendarPresent");
	pDlg->OnEventMouseWheel(wParam, lParam, NULL);
}

// 填充界面并显示
void CDlgCalendarItem::SetItem(int iSlot, const SALE_PROMOTION_ITEM_ESSENCE* pEssence, int index, int usedtimes)
{
	if (index < 0 || index >= MAX_ELEMENT_COUNT)
	{
		//Show(false);
		return;
	}

	m_iSlot = iSlot;
	m_nEleIndex = index;
	m_idItem = pEssence->id;
	m_nUsedTimes = usedtimes;
	m_pMsgBox = NULL;

	CDlgCalendarPresent* pDlg = (CDlgCalendarPresent*)GetGameUIMan()->GetDialog("Win_CalendarPresent");
	const SalePromotionConditionUtil& conditionUtil = pDlg->GetConditionUtil();
	const SALE_PROMOTION_ITEM_ESSENCE::ELEMENT& element = pEssence->element_list[m_nEleIndex];
	time_t timeZoneBiasDiff = g_pGame->GetTimeZoneBias() * 60;
	time_t beginTime = element.valid_time_start - timeZoneBiasDiff;
	time_t endTime   = element.valid_time_end - timeZoneBiasDiff;

	ACString strText, strText2;
	tm *timeTM = gmtime(&beginTime);
	strText2.Format(_AL("%d.%2d.%2d"),  timeTM->tm_year+1900, timeTM->tm_mon+1, timeTM->tm_mday);
	timeTM = gmtime(&endTime);
	strText.Format(_AL("%s - %d.%2d.%2d"), strText2, timeTM->tm_year+1900, timeTM->tm_mon+1, timeTM->tm_mday);
	m_pTxt_Month->SetText(strText);

	if (element.award_item1 > 0 && element.award_item2 > 0)
	{
		SetAwardItem(m_pImg_Award1, element.award_item1, element.award_item1_count, 
			element.award_item1_valid_time, element.award_item1_is_bind);
		SetAwardItem(m_pImg_Award2, element.award_item2, element.award_item2_count, 
			element.award_item2_valid_time, element.award_item2_is_bind);
	}
	else
	{
		SetAwardItem(m_pImg_Award3, element.award_item1, element.award_item1_count, 
			element.award_item1_valid_time, element.award_item1_is_bind);
	}

	if (element.condition_type == COND_TYPE_USE_MONEY || element.condition_type == COND_TYPE_USE_INGOT)
		m_pLab_PriceTitle->SetText(GetStringFromTable(17450));
	else if (element.condition_type == COND_TYPE_USE_ITEM)
		m_pLab_PriceTitle->SetText(GetStringFromTable(17451));
	if (element.condition_type == COND_TYPE_USE_MONEY || element.condition_type == COND_TYPE_USE_INGOT || element.condition_type == COND_TYPE_USE_ITEM)
		conditionUtil.SetConditionText(m_pTxt_Cond, element.condition_type, element.condition_arg1, element.condition_arg2);

	strText.Format(_AL("%d"), element.get_count - m_nUsedTimes);
	m_pTxt_Times->SetText(strText);
	m_pTxt_Desc->SetText(element.desc);
}

// 清空界面
void CDlgCalendarItem::ClearItem()
{
	m_iSlot = -1;
	m_pTxt_Month->SetText(_AL(""));
	m_pImg_Award1->Show(false);
	m_pImg_Award2->Show(false);
	m_pImg_Award3->Show(false);
	m_pLab_Present->Show(false);
	m_pBtn_Get->Show(false);
	m_pBtn_Lottery->Show(false);
	m_pBtn_Buy->Show(false);
	m_pLab_Gets->Show(false);
	m_pTxt_Times->Show(false);
	m_pLab_PriceTitle->Show(false);
	m_pTxt_Cond->Show(false);
	m_pTxt_Desc->Show(false);
}

// 更新物品显示
void CDlgCalendarItem::UpdateItemView()
{
	if (m_idItem > 0 && m_iSlot >= 0)
	{
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(m_iSlot);
		if (!pItem || pItem->GetClassID() != CECIvtrItem::ICID_SALE_PROMOTION_ITEM)
			return;
		
		CDlgCalendarPresent* pDlg = (CDlgCalendarPresent*)GetGameUIMan()->GetDialog("Win_CalendarPresent");
		const SalePromotionConditionUtil& conditionUtil = pDlg->GetConditionUtil();
		CECSalePromotionItem* pSalePromotionItem = (CECSalePromotionItem*)pItem;
		const IVTR_SALE_PROMOTION_ITEM_ESSENCE& itemEssence = pSalePromotionItem->GetEssence();
		const SALE_PROMOTION_ITEM_ESSENCE* pEssence = pSalePromotionItem->GetDBEssence();
		const SALE_PROMOTION_ITEM_ESSENCE::ELEMENT& element = pEssence->element_list[m_nEleIndex];
		time_t timeZoneBiasDiff = g_pGame->GetTimeZoneBias() * 60;
		time_t serverTime = g_pGame->GetServerGMTTime();
		bool bValidTime = serverTime >= element.valid_time_start && serverTime <= element.valid_time_end;
		bool bTwoItem = element.award_item1 > 0 && element.award_item2 > 0;
		bool bCanGet = bValidTime && element.get_count > m_nUsedTimes;
		bool bCondOk = conditionUtil.IsConditionOk(element.condition_type, element.condition_arg1, element.condition_arg2);

		ACString strText;
		m_nUsedTimes = itemEssence.usedcount[m_nEleIndex];
		strText.Format(_AL("%d"), element.get_count - m_nUsedTimes);
		m_pTxt_Times->SetText(strText);

		switch (element.condition_type)
		{
		case COND_TYPE_USE_INGOT:
		case COND_TYPE_USE_MONEY:
			m_pImg_Award1->Show(bTwoItem);
			m_pImg_Award2->Show(bTwoItem);
			m_pImg_Award3->Show(!bTwoItem);
			m_pLab_Present->Show(bTwoItem);
			m_pBtn_Get->Show(false);
			m_pBtn_Lottery->Show(false);
			m_pBtn_Buy->Show(true);
			m_pLab_Gets->Show(true);
			m_pTxt_Times->Show(true);
			m_pLab_PriceTitle->Show(true);
			m_pTxt_Cond->Show(true);
			m_pTxt_Desc->Show(true);
			m_pBtn_Buy->Enable(bCanGet && bCondOk);	
			m_pLab_Gets->SetText(GetStringFromTable(17452));
			if (bCanGet && !bCondOk)
				m_pBtn_Buy->SetHint(GetStringFromTable(element.condition_type == COND_TYPE_USE_INGOT ? 8397 : 12249));
			else
				m_pBtn_Buy->SetHint(_AL(""));
			break;
		case COND_TYPE_LOTTERY:
			m_pImg_Award1->Show(false);
			m_pImg_Award2->Show(false);
			m_pImg_Award3->Show(false);
			m_pLab_Present->Show(false);
			m_pBtn_Get->Show(false);
			m_pBtn_Lottery->Show(true);
			m_pBtn_Buy->Show(false);
			m_pLab_Gets->Show(false);
			m_pTxt_Times->Show(false);
			m_pLab_PriceTitle->Show(false);
			m_pTxt_Cond->Show(false);
			m_pTxt_Desc->Show(true);
			m_pBtn_Lottery->Enable(bValidTime);
			break;
		default:
			m_pImg_Award1->Show(bTwoItem);
			m_pImg_Award2->Show(bTwoItem);
			m_pImg_Award3->Show(!bTwoItem);
			m_pLab_Present->Show(bTwoItem);
			m_pBtn_Get->Show(true);
			m_pBtn_Lottery->Show(false);
			m_pBtn_Buy->Show(false);
			m_pLab_Gets->Show(true);
			m_pTxt_Times->Show(true);
			m_pTxt_Desc->Show(true);
			m_pLab_PriceTitle->Show(COND_TYPE_USE_ITEM == element.condition_type);
			m_pTxt_Cond->Show(COND_TYPE_USE_ITEM == element.condition_type);
			m_pBtn_Get->Enable(bCanGet && bCondOk);	
			if (COND_TYPE_DIRECT == element.condition_type)
			{
				m_pLab_Gets->SetText(GetStringFromTable(17456));
				m_pBtn_Get->SetText(GetStringFromTable(17454));
			}
			else
			{
				m_pLab_Gets->SetText(GetStringFromTable(17453));
				m_pBtn_Get->SetText(GetStringFromTable(17455));
			}
			if (bCanGet && !bCondOk)
				m_pBtn_Get->SetHint(COND_TYPE_USE_ITEM == element.condition_type ? GetStringFromTable(17409) : GetStringFromTable(17408));
			else
				m_pBtn_Get->SetHint(_AL(""));
			break;
		}
	}
}
	
// 填充界面并显示
void CDlgCalendarItem::SetAwardItem(AUIImagePicture* pImg, int id, int count, int valid_time, int is_bind)
{
	CECIvtrItem *pItem = CECIvtrItem::CreateItem(id, 0, count);
	if (pItem)
	{
		pItem->SetNeedUpdate(false);
		pItem->GetDetailDataFromLocal();
		GetGameUIMan()->SetIvtrInfo(pImg, pItem, count);
		delete pItem;
		
/*		if (count > 1)
		{
			ACString szText;
			szText.Format(_AL("%d"), count);
			pImg->SetText(szText);
		}*/
	}
}

void CDlgCalendarItem::OnCommand_Get(const char * szCommand)
{
	CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(m_iSlot);
	if (!pItem || pItem->GetClassID() != CECIvtrItem::ICID_SALE_PROMOTION_ITEM)
			return;
		
	CDlgCalendarPresent* pDlg = (CDlgCalendarPresent*)GetGameUIMan()->GetDialog("Win_CalendarPresent");
	const SalePromotionConditionUtil& conditionUtil = pDlg->GetConditionUtil();
	CECSalePromotionItem* pSalePromotionItem = (CECSalePromotionItem*)pItem;
	const IVTR_SALE_PROMOTION_ITEM_ESSENCE& itemEssence = pSalePromotionItem->GetEssence();
	const SALE_PROMOTION_ITEM_ESSENCE* pEssence = pSalePromotionItem->GetDBEssence();
	const SALE_PROMOTION_ITEM_ESSENCE::ELEMENT& element = pEssence->element_list[m_nEleIndex];
	if (element.condition_type == COND_TYPE_USE_INGOT)
	{
		int cost = element.condition_arg1;
		ACString szText;
		if (element.award_item1 > 0 && element.award_item2 > 0)
		{
			CECIvtrItem *pItem1 = CECIvtrItem::CreateItem(element.award_item1, 0, 1);
			CECIvtrItem *pItem2 = CECIvtrItem::CreateItem(element.award_item2, 0, 1);
			if (pItem1 && pItem2)
			{
				if (cost >= 100)
					szText.Format(GetStringFromTable(17459), cost / 100, pItem1->GetName(), pItem2->GetName());
				else
					szText.Format(GetStringFromTable(17460), cost, pItem1->GetName(), pItem2->GetName());

				GetGameUIMan()->MessageBox("Game_CalendarPresent_Use_Ingot", szText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &m_pMsgBox);
				delete pItem1;
				delete pItem2;
				return;
			}
			delete pItem1;
			delete pItem2;
		}
		else if (element.award_item1 > 0)
		{
			CECIvtrItem *pItem1 = CECIvtrItem::CreateItem(element.award_item1, 0, 1);
			if (pItem1)
			{
				if (cost >= 100)
					szText.Format(GetStringFromTable(17457), cost / 100, pItem1->GetName());
				else
					szText.Format(GetStringFromTable(17458), cost, pItem1->GetName());

				GetGameUIMan()->MessageBox("Game_CalendarPresent_Use_Ingot", szText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &m_pMsgBox);
				delete pItem1;
				return;
			}
		}
	}

	GetItem();
}

void CDlgCalendarItem::OnCommand_Get2(const char * szCommand)
{
	GetItem();

	AString strText = "";
	AIniFile iniFile;
	if(iniFile.Open("configs\\url.ini"))
	{
		char buffer[64];
		sprintf(buffer, "calendar_lottery_%d_%d", m_idItem, m_nEleIndex+1);
		strText = iniFile.GetValueAsString("ingame", buffer, "");
		iniFile.Close();
	}
	if( strText != "" )
		ShellExecuteA(NULL,"open", strText, NULL, NULL, SW_SHOWNORMAL);
}

// 发送协议获取物品
void CDlgCalendarItem::GetItem()
{
	GetGameSession()->c2s_CmdUseItemWithArg(IVTRTYPE_PACK, 1, (unsigned short)m_iSlot, m_idItem, &m_nEleIndex, sizeof(int));
}

////////////////////////////////////////////////////////////////////////////////////////
// CDlgCalendarPresent

AUI_BEGIN_COMMAND_MAP(CDlgCalendarPresent, CDlgBase)
AUI_ON_COMMAND("top",				OnCommand_Top)
AUI_ON_COMMAND("bottom",			OnCommand_Bottom)
AUI_ON_COMMAND("prec",				OnCommand_Prec)
AUI_ON_COMMAND("next",				OnCommand_Next)
AUI_ON_COMMAND("Btn_Get",			OnCommand_GetExtra)
AUI_END_COMMAND_MAP()



AUI_BEGIN_EVENT_MAP(CDlgCalendarPresent, CDlgBase)
AUI_ON_EVENT("*",		WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,		WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_END_EVENT_MAP()



CDlgCalendarPresent::CDlgCalendarPresent()
: m_nItemIndex(-1), m_idItem(0), m_nEleCount(0)
{
}

CDlgCalendarPresent::~CDlgCalendarPresent()
{
	delete m_pConditionUtil;
}

bool CDlgCalendarPresent::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	m_pConditionUtil = new SalePromotionConditionUtil;

	m_pScl_Item = (AUIScroll*)GetDlgItem("Scr_1");
	m_pImg_ExtraAward = (AUIImagePicture*)GetDlgItem("Img_Award1");
	m_pTxt_GetTimes = (AUILabel*)GetDlgItem("Txt_1");
	m_pTitle = (AUILabel*)GetDlgItem("Title");
	GetDlgItem("Lab_Add")->Enable(false);
	m_pBtn_GetExtraAward = (AUIStillImageButton*)GetDlgItem("Btn_Get");

	AString strName;
	for (int i = 0; i < CALENDARITEM_PERPAGE; ++ i)
	{
		strName.Format("Sub_CalendarItemDefault%d", i+1);
		m_pSubItems[i] = (AUISubDialog*)GetDlgItem(strName);
	}

	return true;
}

void CDlgCalendarPresent::OnShowDialog()
{
	if (m_nItemIndex < 0 || m_idItem <= 0)
	{
		Show(false);
		return;
	}

	InitCalendar();
}

void CDlgCalendarPresent::OnHideDialog()
{
	CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(m_nItemIndex);
	if (pItem)
		pItem->Freeze(false);

	m_nItemIndex = -1;
	m_idItem = 0;
	m_nEleCount = 0;
}

void CDlgCalendarPresent::OnTick()
{
	if (CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(m_nItemIndex))
	{
		CECSalePromotionItem* pSalePromotionItem = (CECSalePromotionItem*)pItem;
		const SALE_PROMOTION_ITEM_ESSENCE* pEssence = pSalePromotionItem->GetDBEssence();
		const IVTR_SALE_PROMOTION_ITEM_ESSENCE& itemEssence = pSalePromotionItem->GetEssence();
		ACString strText;
		strText.Format(_AL("%d"), itemEssence.extra_count);
		m_pTxt_GetTimes->SetText(strText);
		m_pBtn_GetExtraAward->Enable(pEssence->extra_award_id != 0 && itemEssence.is_get_extra_award== 0 && itemEssence.extra_count >= pEssence->extra_award_need_count);
	}

	ScrollView();
}

void CDlgCalendarPresent::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
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

void CDlgCalendarPresent::OnCommand_Top(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(0);
}

void CDlgCalendarPresent::OnCommand_Bottom(const char * szCommand)
{
		if (m_nEleCount % 3 == 0)
			m_pScl_Item->SetBarLevel(m_nEleCount / 3 - 2);
		else
			m_pScl_Item->SetBarLevel(m_nEleCount / 3 - 1);
}

void CDlgCalendarPresent::OnCommand_Prec(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() - 3);
}

void CDlgCalendarPresent::OnCommand_Next(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() + 3);
}

void CDlgCalendarPresent::OnCommand_GetExtra(const char* szCommand)
{
	int index = -1;
	GetGameSession()->c2s_CmdUseItemWithArg(IVTRTYPE_PACK, 1, (unsigned short)m_nItemIndex, m_idItem, &index, sizeof(int));
}

// 初始化台历
void CDlgCalendarPresent::InitCalendar()
{
	CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(m_nItemIndex);
	if (!pItem || pItem->GetClassID() != CECIvtrItem::ICID_SALE_PROMOTION_ITEM)
	{
		Show(false);
		return;
	}

	CECSalePromotionItem* pSalePromotionItem = (CECSalePromotionItem*)pItem;
	const SALE_PROMOTION_ITEM_ESSENCE* pEssence = pSalePromotionItem->GetDBEssence();
	const SALE_PROMOTION_ITEM_ESSENCE::ELEMENT* element_list = pEssence->element_list;

	m_pTitle->SetText(pEssence->ui_name);

	int i;
	for (i = 0; i < MAX_ELEMENT_COUNT; ++ i)
	{
		const SALE_PROMOTION_ITEM_ESSENCE::ELEMENT& element = element_list[i];
		if (element.valid_time_start == 0)
			break;
	}
	m_nEleCount = i;

	if (m_nEleCount > CALENDARITEM_PERPAGE)
	{
		if (m_nEleCount % 3 == 0)
			m_pScl_Item->SetScrollRange(0, m_nEleCount / 3 - 2);
		else
			m_pScl_Item->SetScrollRange(0, m_nEleCount / 3 - 1);
		m_pScl_Item->Show(true);
	}
	else
	{
		m_pScl_Item->SetScrollRange(0, 0);
		m_pScl_Item->Show(false);
	}
	m_pScl_Item->SetBarLevel(0);
	m_pScl_Item->SetBarLength(-1);
	m_pScl_Item->SetScrollStep(1);
	m_nStartLine = -1;

	pItem->Freeze(true);

	if (pEssence->extra_award_id != 0)
	{
		if (CECIvtrItem *pItem = CECIvtrItem::CreateItem(pEssence->extra_award_id, 0, pEssence->extra_award_num))
		{
			pItem->GetDetailDataFromLocal();
			GetGameUIMan()->SetIvtrInfo(m_pImg_ExtraAward, pItem);
			delete pItem;
		}
	}

	bool bShowTitleComp = pEssence->extra_award_id > 0;
	GetDlgItem("Txt_1")->Show(bShowTitleComp);
	GetDlgItem("Lab_Add")->Show(bShowTitleComp);
	GetDlgItem("Img_Award1")->Show(bShowTitleComp);
	GetDlgItem("Btn_Get")->Show(bShowTitleComp);
	if(pEssence->id == 57990)
	{
		GetDlgItem("Txt_57990")->Show(true);
		GetDlgItem("Txt_57990")->SetText(GetStringFromTable(19800));
	}
	else if(pEssence->id > 58300 && pEssence->id < 58305)
	{
		GetDlgItem("Txt_57990")->Show(true);
		GetDlgItem("Txt_57990")->SetText(GetStringFromTable(19801));
	}
	else
	{
		GetDlgItem("Txt_57990")->Show(false);
	}
}


// 滚动
void CDlgCalendarPresent::ScrollView()
{
	if( m_nStartLine != m_pScl_Item->GetBarLevel() )
	{
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(m_nItemIndex);
		if (!pItem || pItem->GetClassID() != CECIvtrItem::ICID_SALE_PROMOTION_ITEM)
			return;

		CECSalePromotionItem* pSalePromotionItem = (CECSalePromotionItem*)pItem;
		const SALE_PROMOTION_ITEM_ESSENCE* pEssence = pSalePromotionItem->GetDBEssence();
		const IVTR_SALE_PROMOTION_ITEM_ESSENCE& itemEssence = pSalePromotionItem->GetEssence();
	
		int i;
		int w0 = 0;
		m_nStartLine = m_pScl_Item->GetBarLevel();
		int nStart = m_nStartLine * 3;
		int nEnd = nStart + CALENDARITEM_PERPAGE;
		if( nEnd > m_nEleCount )
			nEnd = m_nEleCount;
		

		CDlgCalendarItem* pDlg;
		for(i = nStart; i < nEnd; i++)
		{
			pDlg = dynamic_cast<CDlgCalendarItem*>(m_pSubItems[w0]->GetSubDialog());
			pDlg->SetItem(m_nItemIndex, pEssence, i, itemEssence.usedcount[i]);
			m_pSubItems[w0++]->Show(true);
		}
		for(i = w0; i < CALENDARITEM_PERPAGE; i++)
		{
			pDlg = dynamic_cast<CDlgCalendarItem*>(m_pSubItems[i]->GetSubDialog());
			pDlg->ClearItem();
		}
	}
}

// 设置台历道具并打开界面
void CDlgCalendarPresent::OpenSalePromotionItem(int index, int id)
{
	OnHideDialog();
	m_nItemIndex = index;
	m_idItem = id;
	Show(true);
}

// 更新已完成的前提条件(任务+成就)
void CDlgCalendarPresent::UpdateConditionInfo(const S2C::sale_promotion_info* pInfo)
{
	m_pConditionUtil->UpdateCondition(pInfo);
}

// 获取前提条件辅助工具
const SalePromotionConditionUtil& CDlgCalendarPresent::GetConditionUtil() const
{
	return *m_pConditionUtil;
}

int CDlgCalendarPresent::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg == "getitem")
	{
		CDlgCalendarItem* pDlg;
		for(int i = 0; i < CALENDARITEM_PERPAGE; i++)
		{
			if (pDlg = dynamic_cast<CDlgCalendarItem*>(m_pSubItems[i]->GetSubDialog()))
			{
				if (pDlg->IsQueryCost(lParam))
				{
					pDlg->GetItem();
					return 0;
				}
			}
		}
	}
	return 1;
}
