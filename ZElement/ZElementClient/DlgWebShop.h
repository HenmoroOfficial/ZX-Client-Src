
#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"

#define GIFT_PACKAGE_SIZE 16
#define ORDER_SIZE 5
struct order_goods
{
	int goods_id;
	int goods_count;
	int goods_flag;
	int goods_time;
};

struct order_package
{
	int package_id;
	int count;
	int price;
	int good_count;
	order_goods goods[GIFT_PACKAGE_SIZE]; //一个礼包中最大的物品数量
};

struct order_info
{
	__int64 order_id;
	int pay_type;
	int status;
	int timestamp;
	int package_count;
	order_package package[ORDER_SIZE]; //一个订单中最大礼包的数量
};

class CDlgWebShopOrder : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgWebShopOrder();
	virtual ~CDlgWebShopOrder();

	void OnCommand_GetItems(const char * szCommand);
	void OnCommand_OpenLink(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	void AddOrderInfo(order_info order, ACString strPackageName);
	void ClearOrderInfo();
	bool IsHasOrder();

protected:
	PAUILABEL		 m_pTxt_ItemNum;
	PAUILABEL		 m_pTxt_ReqNum;
	PAUILABEL		 m_pTxt_PackageName;
	PAUIIMAGEPICTURE m_pImgItem[GIFT_PACKAGE_SIZE];
	PAUILISTBOX		m_pLst_Order;
	PAUISTILLIMAGEBUTTON m_pBtn_Get;
	order_info      m_OrderCur;
	bool			m_bHasOrder;
	int				m_iReqNum;
	abase::vector<order_info> m_vecOrder;
	abase::vector<ACString> m_vecPackageName;
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
};
