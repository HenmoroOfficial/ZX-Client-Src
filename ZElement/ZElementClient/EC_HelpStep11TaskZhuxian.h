/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-诛仙任务
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "EC_HelpStep00Base.h"


class CECHelpStep11TaskZhuxian : public CECHelpStepBase
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHelpStep11TaskZhuxian();
	virtual ~CECHelpStep11TaskZhuxian();

public:		//	Operations
	
	void Help0(bool bStart=true);	// 任务按钮闪烁
	void Help1(bool bStart=true);	// 诛仙任务按钮闪烁
	void Help2(bool bStart=true);	// 青云篇闪烁
	void Help3(bool bStart=true);	// 青云篇第一个任务闪烁
	void Help4(bool bStart=true);	// 领取任务按钮闪烁
};