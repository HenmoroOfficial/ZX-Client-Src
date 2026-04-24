// File		: DlgSystem2.h
// Creator	: Xiao Zhou
// Date		: 2005/8/15

#pragma once

#include "DlgBase.h"

class CDlgSystem2 : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgSystem2();
	virtual ~CDlgSystem2();

	void OnCommandAchievement(const char * szCommand);
	void OnCommandPet(const char * szCommand);
	void OnCommandBroadcast(const char * szCommand);
	void OnCommandAction(const char * szCommand);
	void OnCommandProduce(const char * szCommand);
	void OnCommandRankList(const char * szCommand);
	void OnCommandMailToFriend(const char * szCommand);
};
