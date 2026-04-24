/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-法宝篇
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "EC_HelpStep00Base.h"
#include "vector.h"

class CECHelpStep07Talisman : public CECHelpStepBase
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHelpStep07Talisman();
	virtual ~CECHelpStep07Talisman();

public:		//	Operations
	void UseItem(int id, int index=-1);
	
	void Help0(bool bStart=true);		// 包裹按钮闪烁
	void Help1(bool bStart=true);		// 指向法宝箭头
	void Help2(bool bStart=true);		// HPMP图片
	void Help3(bool bStart=true);		// 天人合一按钮箭头
	
private:
	abase::vector<int>	m_vecTalismans;
};