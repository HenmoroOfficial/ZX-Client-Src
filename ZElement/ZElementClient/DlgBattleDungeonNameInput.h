/**
* @File DlgBattleDungeonNameInput.h
* @Brief 输入开启服务名字
* @Author Fu Chonggang
* @Date 2011-7-25 10:31:19
* @Copyright (c) 2011 ZhuXian Studio, All Rights Reserved. 
*/
#pragma once

#include "DlgBase.h"

class CDlgBattleDungeonNameInput : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgBattleDungeonNameInput();
	virtual ~CDlgBattleDungeonNameInput();
	
	void OnCommandConfirm(const char * szCommand);
	virtual void OnShowDialog();
};
