// File		: DlgSiege.h
// Creator	: Xiao Zhou
// Date		: 2008/2/19

#pragma once

#include "DlgBase.h"

namespace GNET {
	class SiegeInfoGet_Re;
	class SiegeChallenge_Re;
	class SiegeSetAssistant_Re;
	class SiegeEnter_Re;
	class SiegeTop_Re;
}
namespace S2C {
	struct war_end;
}

class CDlgSiege : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgSiege();
	virtual ~CDlgSiege();

	void OnCommandSetAssist(const char * szCommand);
	void OnCommandConfirmAssist(const char * szCommand);
	void OnCommandChoice(const char * szCommand);
	void OnCommandChallenge(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	void OnCommandRefreshSiegeInfo(const char * szCommand);
	void OnCommandRefreshSiegeScore(const char * szCommand);

	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	// From NPC
	void OnWarInfoGetService();
	void OnWarEnterService();

	// From Server
	void OnWarInfoGetRe(const GNET::SiegeInfoGet_Re* p);
	void OnWarChallengeRe(const GNET::SiegeChallenge_Re* p);
	void OnWarAssistRe(const GNET::SiegeSetAssistant_Re* p);
	void OnWarEnterRe(const GNET::SiegeEnter_Re* p);	
	void OnWarTopRe(const GNET::SiegeTop_Re* p);
	void OnWarEnd(const S2C::war_end* p);
	
protected:
	virtual void OnTick();
	virtual bool OnInitDialog();

	void UpdateSiegeList(const GNET::SiegeInfoGet_Re* p);
	void UpdateGuildSiege(const GNET::SiegeInfoGet_Re* p);
	static DWORD m_dwLastJoinTime;
	static bool	 m_bCanJoin;
};
