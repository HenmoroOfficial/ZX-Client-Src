/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-技能篇
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "EC_HelpStep00Base.h"


class CECHelpStep09Skill : public CECHelpStepBase
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHelpStep09Skill();
	virtual ~CECHelpStep09Skill();

public:		//	Operations
	
	void Help0(bool bStart=true);		// 技能图标闪烁
	void Help1(bool bStart=true);		// +技能闪烁
	void Help2(bool bStart=true);		// 提示拖到快捷栏
};