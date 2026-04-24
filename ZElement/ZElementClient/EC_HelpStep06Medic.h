/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-药品篇
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "EC_HelpStep00Base.h"
#include "vector.h"


class CECHelpStep06Medic : public CECHelpStepBase
{
public:		//	Types

public:		//	Constructor and Destructor

	CECHelpStep06Medic();
	virtual ~CECHelpStep06Medic();

public:		//	Operations
	void UseItem(int id, int index=-1);
	
	void Help0(bool bStart);	// 包裹图标闪烁
	void Help1(bool bStart);	// 指向药品的箭头提示使用
	
private:
	abase::vector<int>	m_vecMedics;
};