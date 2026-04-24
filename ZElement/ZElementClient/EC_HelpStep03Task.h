/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-任务篇
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "EC_HelpStep00Base.h"


class CECHelpStep03Task : public CECHelpStepBase
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHelpStep03Task();
	virtual ~CECHelpStep03Task();

public:		//	Operations
	virtual void Help0(bool bStart=true);	// 追踪面板NPC闪烁
	virtual void Help1(bool bStart=true);	// 雏鹰展翅 闪烁
	virtual void Help2(bool bStart=true);	// 接任务闪烁	
	virtual void Help3(bool bStart=true);	// 任务面板按钮闪烁
	virtual void Help4(bool bStart=true);	// 任务列表提示	
};