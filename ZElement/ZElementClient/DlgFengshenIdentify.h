// Filename	: DlgFengshenIdentify.h
// Creator	: WuWeixin
// Date		: 2012/8/24

#pragma once

#include "DlgBase.h"

class AUIStillImageButton;
class AUIImagePicture;
class AUILabel;


class CDlgFengshenIdentify : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	CDlgFengshenIdentify();
	virtual ~CDlgFengshenIdentify();
	
	void OnCommand_CANCEL(const char * szCommand);

	// 鉴定
	void OnCommand_confirm(const char * szCommand);

	// 放置星座
	void SetAstrology(int nItemIndex);

protected:
	virtual bool OnInitDialog();
	virtual void OnHideDialog();

	// 在鉴定结束时, 关闭鉴定窗口
	virtual void OnTick();

	// 刷新"鉴定"按钮, 在条件不符合时按钮呈禁用状态
	void UpdateConfirmButton();

	AUIStillImageButton *	m_pBtnConfirm;
	AUIImagePicture *		m_pItem;
	AUILabel *				m_pTextMoney[3];

	int m_nItemIndex;
	int m_idItem;
};
