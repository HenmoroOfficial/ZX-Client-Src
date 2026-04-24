// Filename	: DlgAttriUp.h
// Creator	: WuWeixin
// Date		: 2013/2/22

#pragma once

#include "DlgBase.h"
#include <vector.h>

class AUIStillImageButton;
class AUIImagePicture;
class AUILabel;
class AUIProgress;
class AUIStillImageButton;
class A2DSprite;
struct PROP_ADD_ITEM_ESSENCE;
struct PROP_ADD_MATERIAL_ITEM_ESSENCE;

// 增益属性显示界面
class CDlgAttriUpNature : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
	enum
	{
		PROP_ADD_ATTR_COUNT = 12,  // 目前使用12条属性增益
	};
public:
	CDlgAttriUpNature();
	virtual ~CDlgAttriUpNature();

	// 预览吃丹后属性
	void PreviewPropAddItem(int id);

	// 更新进度显示, m_idPreviewItem为0时紫条不显示, 不为0时, 紫条显示吃丹后属性增长情况
	void UpdatePropAddProgress();
	
protected:
	virtual bool OnInitDialog();
	virtual void OnHideDialog();
	virtual void OnShowDialog();

	// 设置整型属性
	void SetIntValue(int type, int value, int preadd);

	// 设置浮点型属性(value、preadd均为千分数整型表示)
	void SetFloatValue(int type, int value, int preadd);

	AUILabel* m_propValues[PROP_ADD_ATTR_COUNT];
	AUILabel* m_propPercents[PROP_ADD_ATTR_COUNT];
	AUIProgress* m_propProgress[PROP_ADD_ATTR_COUNT];
	AUIProgress* m_propPreviewProgress[PROP_ADD_ATTR_COUNT];	// 吃丹预览进度条, 为动态生成, 需释放
	int m_idPreviewItem;
};


//////////////////////////////////////////////////////////////////////////
// 重炼界面
class CDlgAttriUpEnchase : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	CDlgAttriUpEnchase();
	virtual ~CDlgAttriUpEnchase();

	void OnCommand_Confirm(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	// 设置所需材料类型和数量
	void SetForgesAndCount(const int* idMaterials, int nLen, int nNeedCount);

	// 放置炼丹材料
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	// 清除玩家操作
	void Clear();

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();

	void ErrorForgeMsg();

	AUIImagePicture* m_pMaterial;
	AUIStillImageButton* m_pBtnConfirm;
	
	int m_nNeedForgeCount;				// 所需重炼材料数
	abase::vector<int> m_Forges;		// 所需重炼材料类型, 满足其中之一即可
	int m_idForge;		// 玩家放入的重炼材料id
	int m_nForgeIndex;	// 玩家放入的重炼材料位置
};


//////////////////////////////////////////////////////////////////////////
// 炼丹炉界面

class CDlgAttriUp : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	CDlgAttriUp();
	virtual ~CDlgAttriUp();

	void OnCommand_Start(const char * szCommand);
	void OnCommand_Stop(const char * szCommand);
	void OnCommand_Retry(const char * szCommand);
	void OnCommand_Get(const char * szCommand);
	void OnCommand_Expand(const char * szCommand);
	void OnCommand_Help(const char * szCommand);

	// 放置炼丹材料或丹药
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	// 设置材料
	void SetForgeItem(int idForge, int nIndex);

	// 合成结果
	void OnPropAddItemResult(int item_id, int item_index);

	// 获取重炼材料id
	int GetForgeItemId() const { return m_idForge; }

	// 获取重炼材料位置
	int GetForgeItemIndex() const { return m_nForgeIndex; }


protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	virtual void OnTick();
	virtual	bool Release();

	// 根据原材料配置, 显示产出物品
	void SetProduceItem(const PROP_ADD_MATERIAL_ITEM_ESSENCE& essence);

	// 开始转动效果
	void StartWheel();

	// 取下材料
	void TakeOffItem();


	enum
	{
		PROP_ADD_ITEM_PRODUCE_COUNT = 6,  // 6个产出类型
	};

	AUIImagePicture* m_pItem0;
	AUIImagePicture* m_pItemProduce[PROP_ADD_ITEM_PRODUCE_COUNT];
	AUIImagePicture* m_pImgHover[PROP_ADD_ITEM_PRODUCE_COUNT];
	AUIStillImageButton* m_pBtn_Start;
	AUIStillImageButton* m_pBtn_Stop;
	AUIStillImageButton* m_pBtn_Retry;
	AUIStillImageButton* m_pBtn_Get;
	A2DSprite*           m_pSpriteCover;

	int m_nItemType;
	int m_nItemIndex;
	int m_idItem;
	int m_nItemCount;
	int m_nForgeIndex;
	int m_idForge;
	int m_idNeedForges[3];
	int   m_nCurrentTurn;	// 转动效果中, 当前高亮的格子的index
	DWORD m_tTurnToNext;	// 用于控制转盘效果 
	bool  m_bWheelRunning;
	int   m_idResult;		// 服务器返回的结果
	int	  m_nResultIndex;	// 服务器返回的结果
	DWORD m_tStartWheelTime;
};
