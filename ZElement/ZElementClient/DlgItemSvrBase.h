// File		: DlgItemSvrBase
// Creator	: Fu Chonggang
// Date		: 2011/3/15

#pragma once

#include "DlgBase.h"

class CECIvtrItem;
class AUIImagePicture;
class AUIStillImageButton;
class AUILabel;

class CDlgItemSvrBase : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgItemSvrBase();
	virtual ~CDlgItemSvrBase();

	void OnCommandConfirm(const char *szCommand);
	void OnCommandCancel(const char * szCommand);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	virtual void SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg=NULL) = 0;
	virtual void ServerStart() = 0;
	virtual void Reset() {}
	virtual void UpdateDisplay() {}

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	bool UpdateMoney();

	AUIStillImageButton		*m_pBtn_Confirm;
	AUIImagePicture			*m_pImg_Item;
	AUILabel				*m_pTxt_Bronze;
	AUILabel				*m_pTxt_Silver;
	AUILabel				*m_pTxt_Gold;
	int						m_iMoney;
};
