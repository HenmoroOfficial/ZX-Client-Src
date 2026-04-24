/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-装备使用篇
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "EC_HelpStep00Base.h"
#include "vector.h"

class CECHelpStep02Equip : public CECHelpStepBase
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHelpStep02Equip();
	virtual ~CECHelpStep02Equip();

public:		//	Operations
	void UseItem(int id, int index=-1);

	virtual void Help0(bool bStart=true);	// 包裹图标闪烁
	virtual void Help1(bool bStart=true);	// 箭头指向百宝箱
	virtual void Help2(bool bStart=true);	// 箭头指向武器提示右键装备

private:
	abase::vector<int> m_vecTreasureChests;
	abase::vector<int> m_vecWeapons;

	// 不同的百宝箱采用不同的物品
	int m_iIdIndex;
};