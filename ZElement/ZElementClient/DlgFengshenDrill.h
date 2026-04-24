// Filename	: DlgFengshenDrill.h
// Creator	: WuWeixin
// Date		: 2012/8/24

#pragma once

#include "DlgBase.h"

class AUIStillImageButton;
class AUIImagePicture;

class CDlgFengshenDrill : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	CDlgFengshenDrill();
	virtual ~CDlgFengshenDrill();

	// set_equip: 放置装备,  set_props: 放置道具
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
	void OnCommand_CANCEL(const char * szCommand);

	// "打孔"按钮事件, 弹msgbox提示玩家需消耗道具, 确认是否打孔
	void OnCommand_confirm(const char * szCommand);

	// 卸下装备或打孔道具
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	// 确定打孔后, 调用以发送打孔协议
	void SendCmd();
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	// 刷新"打孔"按钮, 在条件不符合时按钮呈禁用状态
	void UpdateConfirmButton();

	AUIStillImageButton *	m_pBtnConfirm;
	AUIImagePicture *		m_pItem1;
	AUIImagePicture *		m_pItem2;

	int m_nItemIndex;
	int m_idItem;
	int m_nStoneIndex;
	int m_idStone;
};
