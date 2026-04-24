// Filename	: DlgQProduceTool.h
// Creator	: WuWeixin
// Date		: 2013/3/6

#pragma once

#include "DlgBase.h"
#include <vector.h>
#include <hashmap.h>

class AUIStillImageButton;
class AUIImagePicture;
class AUIScroll;
class AUITreeView;
class AUILabel;
class AUIEditBox;


class CDlgQProduceTool : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	CDlgQProduceTool();
	virtual ~CDlgQProduceTool();

	virtual bool Tick(void);

	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_Tv_Lst(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	void OnCommand_Top(const char * szCommand);
	void OnCommand_Bottom(const char * szCommand);
	void OnCommand_Next(const char * szCommand);
	void OnCommand_Prec(const char * szCommand);
	void OnCommand_Min1(const char * szCommand);
	void OnCommand_Min2(const char * szCommand);
	void OnCommand_Min3(const char * szCommand);
	void OnCommand_Max1(const char * szCommand);
	void OnCommand_Max2(const char * szCommand);
	void OnCommand_Max3(const char * szCommand);
	void OnCommand_MinMoneyItem(const char * szCommand);
	void OnCommand_MaxMoneyItem(const char * szCommand);
	void OnCommand_SelectAll(const char * szCommand);
	void OnCommand_CancelAll(const char * szCommand);
	void OnCommand_Close(const char * szCommand);
	void OnCommand_Ok(const char * szCommand);
	
protected:
	struct JinFaShenMoneyConfig
	{
		int index;
		int money;				//
		int reputation;			// 仙魔佛造化
	};

	struct JinFaShen 
	{
		int  id;		// 模板id
		int  index;		// 配置表中的索引
		int  slot;		// 位置
		bool select;	// 当前是否选中

		struct CompareListByIdAndSlot
		{
		public:
			bool operator ()(const JinFaShen& p1, const JinFaShen& p2)
			{
				if (p1.index < p2.index)
					return false;
				return p1.slot < p2.slot;
			}
		};
	};

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();

	// 更新包裹金身法身
	void UpdateJinFaShen();

	// 更新显示
	void UpdateView();

	// 清楚输入的数量
	void ClearNumber();

	// 取消所有选择
	void CancelSelect();

	// 选择或取消一个金身、法身, bUpdateCost用于控制是否重新计算消耗和抵扣数值
	void Select(JinFaShen& jinFaShen, bool bSelect);

	// 更新消耗和抵扣数值
	void UpdateCost();

	// 获取当前要生产的配置
	const JinFaShenMoneyConfig* GetCurrentProduceConfig();

	// 获取当前选中的金身、法身的抵扣值
	void GetJinFaShenMoneyAndValue(int& money, int& zhaohua);
	
	// 更新当前身上的造化值
	void UpdateZhaohua();

	// 更新当前身上的5000金物品数量
	void UpdateMoneyItem();

	// 造化值max 0:仙 1:魔 2:佛
	int MaxZhaohua(int type);
	
	// 5000金物品max
	int MaxMoneyItem();

	void FreezeAll(bool bFreeze);


	abase::hash_map<int, JinFaShenMoneyConfig> m_JinShenConfigs;
	abase::hash_map<int, JinFaShenMoneyConfig> m_FaShenConfigs;
	abase::vector<JinFaShen> m_IvtrJinShen;	// 包裹中的金身
	abase::vector<JinFaShen> m_IvtrFaShen;	// 包裹中的法身

	int				m_nFirstLine;

	AUIScroll		*m_pScl_Item;
	AUITreeView		*m_pProduceList;
	AUIImagePicture *m_pImgItems[48];
	AUIImagePicture *m_pImgHovers[48];
	AUILabel		*m_pNowCredit01;
	AUILabel		*m_pNowCredit02;
	AUILabel		*m_pNowCredit03;
	AUILabel		*m_pNowMoneyItem;
	AUIEditBox		*m_pInput01;
	AUIEditBox		*m_pInput02;
	AUIEditBox		*m_pInput03;
	AUIEditBox		*m_pInputMoneyItem;
	AUILabel		*m_pReplaceZhaohua;
	AUILabel		*m_pReplaceMoney1;
	AUILabel		*m_pNeedZhaohua;
	AUILabel		*m_pNeedMoney1;

	int				m_idProduceItem;
	int				m_nProduceType;
	int				m_nNeedMoney;
	int				m_nNeedZhaohua;
	int				m_nCostMoneyItem; // 所要使用的自己身上的5000金银票数量
	int				m_nCostZhaohua[3]; // 所要使用的自己身上的造化值
};
