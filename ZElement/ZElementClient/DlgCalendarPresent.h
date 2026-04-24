// Filename	: DlgCalendarPresent.h
// Creator	: WuWeixin
// Date		: 2012/12/21

#pragma once

#include "DlgBase.h"

class AUIStillImageButton;
class AUIImagePicture;
class AUILabel;
class AUIScroll;
struct SALE_PROMOTION_ITEM_ESSENCE;
class SalePromotionConditionUtil;
namespace S2C { struct sale_promotion_info; }

// 台历项子界面, 用于显示一项具体的内容
class CDlgCalendarItem : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	CDlgCalendarItem();
	virtual ~CDlgCalendarItem();

	void OnCommand_Get(const char * szCommand);
	void OnCommand_Get2(const char * szCommand);

	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	// 填充界面并显示
	void SetItem(int iSlot, const SALE_PROMOTION_ITEM_ESSENCE* pEssence, int index, int usedtimes);

	// 清空界面
	void ClearItem();

	// 是否该界面发出消耗元宝领奖励的需求
	bool IsQueryCost(LPARAM lParam) { return (LPARAM)m_pMsgBox == lParam; }

	// 发送协议获取物品
	void GetItem();

protected:
	virtual bool OnInitDialog();
	virtual void OnHideDialog();
	virtual void OnTick();

	// 设置奖励物品
	void SetAwardItem(AUIImagePicture* pImg, int id, int count, int valid_time, int is_bind);

	// 更新物品显示
	void UpdateItemView();


	AUILabel			*m_pTxt_Month;		// 标题日期
	AUILabel			*m_pLab_Present;	// "赠"
	AUILabel			*m_pTxt_Times;		// 领取次数
	AUILabel			*m_pTxt_Cond;		// 领取条件
	AUILabel			*m_pTxt_Desc;		// 说明
	AUILabel			*m_pLab_Gets;		// 领取次数文本控件: "可领取：    次"
	AUILabel			*m_pLab_PriceTitle;	// "价格"标题
	AUIStillImageButton	*m_pBtn_Get;		// "领取"按钮
	AUIStillImageButton *m_pBtn_Lottery;	// "抽奖"按钮
	AUIStillImageButton *m_pBtn_Buy;		// "购买"按钮
	AUIImagePicture		*m_pImg_Award1;
	AUIImagePicture		*m_pImg_Award2;
	AUIImagePicture		*m_pImg_Award3;
	AUIDialog*			m_pMsgBox;

	int					m_nUsedTimes;		// 已领取次数
	int					m_iSlot;			// 台历道具在包裹中的位置
	int					m_idItem;			// 台历道具id
	int					m_nEleIndex;		// 当前台历项索引位置
};


//////////////////////////////////////////////////////////////////////////

#define CALENDARITEM_PERPAGE 6

// 台历主界面
class CDlgCalendarPresent : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	CDlgCalendarPresent();
	virtual ~CDlgCalendarPresent();

	void OnCommand_Top(const char * szCommand);
	void OnCommand_Bottom(const char * szCommand);
	void OnCommand_Next(const char * szCommand);
	void OnCommand_Prec(const char * szCommand);
	void OnCommand_GetExtra(const char* szCommand);

	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	// 设置台历道具并打开界面
	void OpenSalePromotionItem(int index, int id);

	// 更新已完成的前提条件(任务+成就)
	void UpdateConditionInfo(const S2C::sale_promotion_info* pInfo);
	
	// 获取前提条件辅助工具
	const SalePromotionConditionUtil& GetConditionUtil() const;

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	virtual void OnTick();

	// 初始化台历
	void InitCalendar();

	// 滚动
	void ScrollView();

	AUIScroll			*m_pScl_Item;
	AUISubDialog		*m_pSubItems[CALENDARITEM_PERPAGE];
	AUIImagePicture		*m_pImg_ExtraAward;
	AUILabel			*m_pTxt_GetTimes;
	AUILabel			*m_pTitle;
	AUIStillImageButton	*m_pBtn_GetExtraAward;

	int m_idItem;
	int m_nItemIndex;
	int m_nEleCount;
	int m_nStartLine;
	SalePromotionConditionUtil *m_pConditionUtil;
};
