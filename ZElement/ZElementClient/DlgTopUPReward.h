/********************************************************************
	created:	2012/8/31
	file base:	CDlgTopUPReward
	file ext:	h
	author:		zy
	
	purpose:	≥‰÷µΩ±¿¯ΩÁ√Ê
*********************************************************************/

#pragma once

#include "hashmap.h"
#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"

class CECIvtrItem;

#define TOPUPREWARD_A_GROUP_MAX 9
#define TOPUPREWARD_B_GROUP_MAX 10
#define TOPUPREWARD_C_GROUP_MAX 13
#define TOPUPREWARD_D_GROUP_MAX 11
#define TOPUPREWARD_E_GROUP_MAX 28

/************************************************************************
CDlgTopUPRewardMin
************************************************************************/
class CDlgTopUPRewardMin : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
public:
	CDlgTopUPRewardMin() {}
	virtual ~CDlgTopUPRewardMin() {}

	void OnCommandOpen(const char * szCommand);

};


class CDlgTopUPReward : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgTopUPReward();
	virtual ~CDlgTopUPReward();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	PAUIIMAGEPICTURE GetGroupItem(int nID,const char *pszItemName);
private:
};