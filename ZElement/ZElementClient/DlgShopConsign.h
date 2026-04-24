// Filename	:	DlgShopConsign.h
// Creator	:	Fu Chonggang
// Date		:	Aug 3, 2010

#pragma once


#include "DlgBase.h"
#include "vector.h"
#include "hashmap.h"
class AUIEditBox;
class AUILabel;
class AUIRadioButton;
class AUIStillImageButton;
class AUIImagePicture;
class AUIComboBox;
class AUICheckBox;
class AUIListBox;

#include "gconsignitem"

class CDlgShopConsign : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
public:
	enum ConsignState
	{
		STATE_PRE_POST,                                 //预寄售,未与平台同步
		STATE_POST,                                     //寄售成功,与平台同步完毕,下架
		STATE_PRE_CANCEL_POST,                          //预取消寄售,未与平台同步
		STATE_SHOW,                                     //公示
		STATE_SELL,                                     //上架
		STATE_SOLD,
		STATE_EXPIRE,
		STATE_POST_FAIL,
		STATE_POST_CANCEL,
		STATE_POST_FORCE_CANCEL
	};
public:
	CDlgShopConsign();
	virtual ~CDlgShopConsign();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	virtual bool Release();

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_cancelconsign(const char * szCommand);
	void OnCommand_clear(const char * szCommand);
	void OnCommand_website(const char * szCommand);
	void OnCommand_confirm(const char * szCommand);
	void OnCommand_listname(const char * szCommand);
	void OnCommand_listtime(const char * szCommand);
	void OnCommand_listprice(const char * szCommand);
	void OnCommand_ConsignItem(const char * szCommand);
	void OnCommand_ConsignMoney(const char * szCommand);
	void OnCommand_ConsignChar(const char * szCommand);
	void OnCommand_EnterName(const char * szCommand);
	void OnCommand_Refresh(const char * szCommand);
	void OnCommand_Help(const char * szCommand);
	void OnCommand_AllList(const char * szCommand);
	void OnCommand_CharList(const char * szCommand);

	bool CanBeConsigned(int tid);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();

	AUIStillImageButton *m_pBtn_CancelConsign;
	AUIImagePicture *m_pImg_Goods;
	AUIImagePicture *m_pImg_Head;
	AUILabel *m_pLab_Goods;
	AUIEditBox *m_pEdt_PriceNum;
	AUIComboBox *m_pCombo_Day;
	AUIEditBox *m_pEdt_BuyerName;
	AUIStillImageButton *m_pBtn_Confirm;
	AUIStillImageButton *m_pBtn_ListName;
	AUIStillImageButton *m_pBtn_ListTime;
	AUIStillImageButton *m_pBtn_ListPrice;
	AUIStillImageButton *m_pBtn_ListBuyer;
	AUIListBox *m_pLst_History;
	AUIEditBox *m_pEdt_GoldNum;
	AUILabel *m_pLab_Gold;

	abase::vector<GConsignItem> m_ConsignItems;
	abase::hash_map<int, int>	m_mapItems;
	int m_nItemIndex;
	
	static const int CONSIGN_PRICE;
	static const int GOLD_TRADE_MIN;
	
	bool IsSameUserId(ACString str);
};
