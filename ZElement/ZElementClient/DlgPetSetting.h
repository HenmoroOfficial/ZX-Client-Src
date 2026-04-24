// File		: DlgPetSetting.h
// Creator	: Xiao Zhou
// Date		: 2008/4/3

#pragma once

#include "DlgBase.h"

class CDlgPetSetting : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgPetSetting();
	virtual ~CDlgPetSetting();

	void OnCommandConfirm(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	int GetPetRankNumber(int rank, int index);
	
};
