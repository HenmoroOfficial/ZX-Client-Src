// Filename	: DlgPetMain.h
// Creator	: Fu Chonggang
// Date		: 2012-6-20

#pragma once

#include "DlgItemSvrBase.h"

class CDlgPetMain : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgPetMain();
	virtual ~CDlgPetMain();
	
	void OnCommandTame(const char* szCommand);			// 宠物驯养
	void OnCommandPetUp(const char* szCommand);			// 宠物修炼
	void OnCommandRelease(const char* szCommand);		// 宠物放生
	void OnCommandEquipEnchase(const char* szCommand);	// 宠物强化
	void OnCommandShopExchange(const char* szCommand);	// 宠物兑换
	void OnCommandShop(const char* szCommand);			// 宠物商店
	void OnCommandFly(const char* szCommand);			// 宠物飞升
	void OnCommandCancel(const char* szCommand);
protected:
};
