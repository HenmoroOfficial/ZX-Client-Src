/*
 * FILE: EC_GameUIMan.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include <windowsx.h>

#include "A3DEngine.h"
#include "EC_StringTab.h"
#include "AUI\\AUILuaManager.h"
class CECGameUIMan : public AUILuaManager
{
public:		//	Constructor and Destructor
	CECGameUIMan(){};
	virtual ~CECGameUIMan(){};

	void Int64ToStr(INT64 num, ACString& str);
};