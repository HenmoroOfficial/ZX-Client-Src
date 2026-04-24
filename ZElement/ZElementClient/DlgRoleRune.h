// Filename	:	DlgRoleRune.h
// Creator	:	Wu Weixin
// Date		:	2013/04/11
// Description:	符文界面

#pragma once

#include "DlgBase.h"

class AUIImagePicture;
class AUILabel;
class AUIScroll;
class AUICheckBox;
class CECIvtrRune2013;

//////////////////////////////////////////////////////////////////////////
// 符文界面
class CDlgRoleRune : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:

	CDlgRoleRune();
	~CDlgRoleRune();

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	virtual void OnTick();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
	// 移除符文
	void OnCommand_Move(const char * szCommand);

	void OnCommand_Help(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	// 更新显示
	void UpdateView();

	// 显示融合特效
	void ShowEnchantingGfx();

protected:

	void ShowRuneTipPop(); // 飞升150后，第一次打开时需要显示符文泡泡提示

	int		m_nCurrentSize; // 当前开放的格子数
	int		m_nCurrentSel;	// 当前选中的符文位置
	int		m_iSlotRune;	// 当前要镶嵌的符文包裹位置
	int		m_iSlotPack;	// 当前要镶嵌的符文槽位置 

	AUIImagePicture*	m_pItem[6];
	AUIObject*			m_pImg_Hover[6];
	AUIObject*			m_pShape[6];
	AUILabel*			m_pLab[6];
	AUILabel*			m_pTxt[6];
};


//////////////////////////////////////////////////////////////////////////
// 符文合成界面
class CDlgRoleRuneCombine : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:

	CDlgRoleRuneCombine();
	~CDlgRoleRuneCombine();

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	virtual bool Tick(void);

	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnCommand_Top(const char * szCommand);
	void OnCommand_Bottom(const char * szCommand);
	void OnCommand_Next(const char * szCommand);
	void OnCommand_Prec(const char * szCommand);
	void OnCommand_Help(const char * szCommand);

	// 合成一个
	void OnCommand_One(const char * szCommand);
	
	// 合成全部
	void OnCommand_All(const char * szCommand);

	// 取消
	void OnCommand_Cancel(const char * szCommand);

	// 更新包裹碎片
	void UpdateRuneFragment();

	// 清除碎片信息
	void ClearRuneFragment();

	// 更新显示
	void UpdateView();

	// 向服务器请求合成一个, 服务器合成后通知客户端, 也需要调用这个方法
	void ProduceOne();

	// 更新合成次数
	void UpdateCombineTimes(int times, int extraTimes);

protected:
	void ShowRuneTipPop(); // 飞升150后，第一次打开时需要显示符文泡泡提示

	int					m_nCombineTiems; // 当天合成次数
	int					m_nExtraTimes;	 // 额外合成次数

	int					m_nFirstLine;
	int					m_nTotalFragmentsCount;
	int					m_nProduceCount;
	int					m_nExtItemCount;  // 额外道具个数
	bool				m_bProducing;
	bool				m_bNeedUpdateItem;
	abase::vector<int>	m_vecFragmentIndies;

	AUIImagePicture* m_pImgItems[36];
	AUIImagePicture* m_pImgItemMatter; // 符文碎片
	AUILabel*		 m_pTxt_Time;
	AUILabel*		 m_pTxt_TimeLimit;
	AUIScroll*		 m_pScl_Item;
};


//////////////////////////////////////////////////////////////////////////
// 符文融合界面
class CDlgRoleRuneEnchanting : public CDlgBase
{
	friend class CDlgRoleRuneEnchantingExp;

	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

	struct RuneItem 
	{
		int  m_nSlot;
		int  m_nQuality; // 符文品质 0:初等 1:中等 2:高等 3:极品
		int	 m_nLevel;	 // 
		int  m_nExp;	 // 经验
		bool m_bRune;    // true: 符文, false: 碎片
		bool m_bSel;
	};

public:

	CDlgRoleRuneEnchanting();
	~CDlgRoleRuneEnchanting();

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	virtual void OnTick();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnCommand_Top(const char * szCommand);
	void OnCommand_Bottom(const char * szCommand);
	void OnCommand_Next(const char * szCommand);
	void OnCommand_Prec(const char * szCommand);

	// 打开界面对指定符文进行升级, nWhere: 0-包裹, IVTRTYPE_FUWEN-符文装备包
	void EnchantingRune(int nIvtrIndex, int nWhere = 0);

	// 融合
	void OnCommandEnchanting(const char* szCommand);
	
	// 取消
	void OnCommand_Cancel(const char * szCommand);

	// 选择
	void OnCommand_FreeSel(const char * szCommand);
	void OnCommand_SelLv1Rune(const char * szCommand);
	void OnCommand_SelLv2Rune(const char * szCommand);
	void OnCommand_SelFragment(const char * szCommand);

	void OnCommand_Help(const char * szCommand);

protected:
	// 更新包裹物品
	void UpdateRuneItem();

	// 更新物品显示
	void UpdateView();

	// 更新经验显示
	void UpdateExp();
	
	// 清除物品
	void ClearRuneItem();

	// 发送融合协议
	void Enchanting();

	void ShowRuneTipPop(); // 飞升150后，第一次打开时需要显示符文泡泡提示

protected:
	int						m_nFirstLine;
	int						m_nMainRuneIndex;
	int						m_nMainRuneWhere; // 0-包裹, 11-符文装备包
	abase::vector<RuneItem> m_vecItems;		// 包裹中所有碎片、符文(不包含主符文)
	abase::vector<int>		m_vecSelSlots;  // 选中的融合物品位置
	bool					m_bNeedUpdateItem;

	AUIImagePicture* m_pImgItems[36];
	AUIImagePicture* m_pImgHovers[36];
	AUIImagePicture* m_pImgMain; // 主符文
	AUILabel*		 m_pTxt_Exp;
	AUIScroll*		 m_pScl_Item;
	AUICheckBox*	 m_pChk_01;
	AUICheckBox*	 m_pChk_02;
	AUICheckBox*	 m_pChk_03;
	AUICheckBox*	 m_pChk_04;
};


//////////////////////////////////////////////////////////////////////////
// 符文融合经验提醒
class CDlgRoleRuneEnchantingExp : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
public:

	CDlgRoleRuneEnchantingExp();
	~CDlgRoleRuneEnchantingExp();

	virtual bool OnInitDialog();

	void ShowExpTip(int totalExp, int needExp);

	void OnCommandYes(const char* szCommand);
	void OnCommandNo(const char * szCommand);

protected:
	AUILabel* m_pTxt_Exp01; // 预期获得经验
	AUILabel* m_pTxt_Exp02; // 满级所需经验
	AUILabel* m_pTxt_Exp03;	// 浪费经验
};