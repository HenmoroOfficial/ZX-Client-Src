// File		: DlgTeamMain.h
// Creator	: Xiao Zhou
// Date		: 2005/8/17

#pragma once

#include "DlgBase.h"

class CDlgTeamMain : public CDlgBase  
{
	friend class CDlgWorldMap;

	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgTeamMain();
	virtual ~CDlgTeamMain();

	void OnCommandCANCAL(const char *szCommand);
	void OnCommandMinimize(const char *szCommand);
	void OnCommandArrow(const char *szCommand);
	void OnCommandGTEnterTeam(const char *szCommand);

	bool UpdateTeamInfo();
	bool IsShowTeamArrow() { return m_bShowTeamArrow; }
	
	virtual void OnShowDialog();
private:
	bool m_bShowTeamArrow;

	enum
	{
		TEAM_STATUS_OPEN = 0,
		TEAM_STATUS_CLOSE
	};
};
