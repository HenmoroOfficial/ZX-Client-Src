/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-地图篇
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "EC_HelpStep00Base.h"


class CECHelpStep08Map : public CECHelpStepBase
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHelpStep08Map();
	virtual ~CECHelpStep08Map();

public:		//	Operations
	
	void Help0(bool bStart=true);	// 快捷键界面M
	void Help1(bool bStart=true);	// NPC列表箭头
};