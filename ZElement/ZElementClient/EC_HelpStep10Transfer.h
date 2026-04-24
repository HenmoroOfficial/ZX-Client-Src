/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-飞天服务
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "EC_HelpStep00Base.h"


class CECHelpStep010Transfer : public CECHelpStepBase
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHelpStep010Transfer();
	virtual ~CECHelpStep010Transfer();

public:		//	Operations
	void Help0(bool bStart=true);	// 怎么做？不一定在NPC旁边
	void Help1(bool bStart=true);		// 青云山 闪烁
};