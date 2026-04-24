// Filename	: DlgFengshenConvert.h
// Creator	: WuWeixin
// Date		: 2012/8/24

#pragma once

#include "DlgBase.h"

class AUIStillImageButton;
class AUIImagePicture;
class AUILabel;

class CDlgFengshenConvert : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	CDlgFengshenConvert();
	virtual ~CDlgFengshenConvert();
	
	// set_equip: 放置星座
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
	void OnCommand_CANCEL(const char * szCommand);

	// "粉碎"按钮事件, 在发送粉碎协议之前, 先判断主角精力值是否已达上限, 当前星座是否珍贵
	void OnCommand_confirm(const char * szCommand);

	// 卸下星座物品
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	// 确定粉碎后, 调用以发送粉碎协议
	void SendCmd();
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	// 在tick中刷新精力值, 当粉碎成功后, 清除星座图标
	virtual void OnTick();

	// 刷新"粉碎"按钮, 在条件不符合时按钮呈禁用状态
	void UpdateConfirmButton();

	AUIStillImageButton *	m_pBtnConfirm;
	AUIImagePicture *		m_pItem;
	AUILabel *				m_pTextNum;

	int m_nItemIndex;
	int m_idItem;

public:
	// 精力值上限, 与服务器约定写死, 如修改需双方同时修改, 目前值为100000
	static const int ms_nMaxEnergy;
};
