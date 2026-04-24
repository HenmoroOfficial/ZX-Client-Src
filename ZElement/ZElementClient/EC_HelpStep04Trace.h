/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-NPC寻径自动寻径
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "EC_HelpStep00Base.h"


class CECHelpStep04Trace : public CECHelpStepBase
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHelpStep04Trace();
	virtual ~CECHelpStep04Trace();

public:		//	Operations
	
	virtual void Help0(bool bStart=true);	// 提示点击寻径
	
};