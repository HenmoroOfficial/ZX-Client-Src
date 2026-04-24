// Filename	: DlgFengshenUpgrade.h
// Creator	: WuWeixin
// Date		: 2012/8/24

#pragma once

#include "DlgBase.h"

class AUIStillImageButton;
class AUILabel;


class CDlgFengshenUpgrade : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	CDlgFengshenUpgrade();
	virtual ~CDlgFengshenUpgrade();
	
	bool SetAstrology(int nItemIndex);
	
	void OnCommand_CANCEL(const char * szCommand);

	// "升级"按钮事件
	void OnCommand_upgrade(const char * szCommand);

	// 刷新界面显示
	void UpdateProps();
	
	// 玩家确认升级, 发送协议
	void SendCmd();

protected:
	virtual bool OnInitDialog();
	virtual void OnHideDialog();

	// 检查包裹中是否存在升级道具, 以刷新"升级"按钮
	virtual void OnTick();

	// 清除界面属性显示
	void ClearProps();

	// 刷新"升级"按钮，包裹中缺少升级道具时按钮呈禁用状态
	void UpdateUpgradeButton();

	AUILabel *		m_pTitle;
	AUILabel *		m_pTxt_Part;
	AUILabel *		m_pLab_Type;
	AUILabel *		m_pTxt_Lv;
	AUILabel *		m_pTxt_EquipLv_A;
	AUILabel *		m_Lab_Nature_A[5];
	AUILabel *		m_Txt_Nature_A[5];
	AUILabel *		m_pTxt_EquipLv_B;
	AUILabel *		m_Lab_Nature_B[5];
	AUILabel *		m_Txt_Nature_B[5];
	AUILabel *		m_pTxt_Score;
	AUIStillImageButton * m_pBtn_Upgrade;

	int m_nItemIndex;
	int m_idItem;
	int m_nStoneIndex;
	int m_idStone;
};
