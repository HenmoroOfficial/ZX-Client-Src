/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-开始选择
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "EC_HelpStep00Base.h"


class CECHelpStep00Start : public CECHelpStepBase
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHelpStep00Start();
	virtual ~CECHelpStep00Start();

public:		//	Operations
	
	virtual void Help0(bool bStart=true);
	
};