// Filename	:	DlgShopConsignAction.h
// Creator	:	Lei Dongyu
// Date		:	2010/10/29
// Description:	寻宝网游戏内商品浏览页面右键菜单

#pragma once

#include "DlgBase.h"
#include "gnbyteorder.h"

class CDlgShopConsignAction : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()

public:

	CDlgShopConsignAction();
	~CDlgShopConsignAction();

//	void OnShowDialog();

	void OnCommand_OpenWebSite(const char* szCommand);
	void OnCommand_CopyWebSite(const char* szCommand);
	void OnCommand_MsgSeller(const char* szCommand);
	void OnCommand_Cancel(const char* szCommand);

	void SetData(int64_t, ACString,int);
//	void SetID(int64_t);
//	void SetVendor(ACString);
	ACString buildURL(int64_t);

protected:
	int64_t itemId;
	ACString vendor;
	int		commodityid;
	
};