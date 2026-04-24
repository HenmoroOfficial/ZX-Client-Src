// Filename	: DlgFengshenErase.h
// Creator	: WuWeixin
// Date		: 2012/8/24

#pragma once

#include "DlgBase.h"

class AUIStillImageButton;
class AUIImagePicture;
class AUILabel;

class CDlgFengshenErase : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	CDlgFengshenErase();
	virtual ~CDlgFengshenErase();

	// set_equip: 放置装备
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
	void OnCommand_CANCEL(const char * szCommand);

	// "摘除"按钮事件, 在发送摘除协议前, 先检查金钱和包裹空间
	void OnCommand_confirm(const char * szCommand);

	// 卸下装备
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	// 刷新"摘除"按钮, 在条件不符合时按钮呈禁用状态
	void UpdateConfirmButton();

	AUIStillImageButton *	m_pBtnConfirm;
	AUIImagePicture *		m_pItem;
	AUILabel *				m_pTextMoney[3];

	int m_nItemIndex;
	int m_idItem;
};
