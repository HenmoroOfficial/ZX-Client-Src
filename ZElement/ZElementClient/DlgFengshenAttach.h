// Filename	: DlgFengshenAttach.h
// Creator	: WuWeixin
// Date		: 2012/8/24

#pragma once

#include "DlgBase.h"

class AUIStillImageButton;
class AUIImagePicture;
class AUILabel;

class CDlgFengshenAttach : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	CDlgFengshenAttach();
	virtual ~CDlgFengshenAttach();
	
	// set_equip: 放置装备, set_props: 放置镶嵌道具
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
	void OnCommand_CANCEL(const char * szCommand);

	// "镶嵌"按钮事件, 先判断金钱是否满足, 满足则发协议
	void OnCommand_confirm(const char * szCommand);

	// 卸下装备或镶嵌道具
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	// 刷新"镶嵌"按钮, 在条件不符合时按钮呈禁用状态
	void UpdateConfirmButton();

	AUIStillImageButton *	m_pBtnConfirm;
	AUIImagePicture *		m_pItem1;
	AUIImagePicture *		m_pItem2;
	AUILabel *				m_pTextMoney[3];

	int m_nItemIndex;
	int m_idItem;
	int m_nStoneIndex;
	int m_idStone;
	int m_equipMask;
};
