// Filename	: DlgDungeonStep.cpp
// Creator	: Li Bin
// Date		: 2013/08/14

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "EC_GPDataType.h"

class CLuaState;
class CECPlayer;

class CDlgDungeonStep : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	enum
	{
		MAX_STEP_NUM = 8
	};

public:
	CDlgDungeonStep();
	virtual ~CDlgDungeonStep();

	void OnCommandMaximize(const char* szCommand);
	void OnCommandMinimize(const char* szCommand);
	void OnCommandDetail(const char* szCommand);

	void OnEventKeyDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void UpdateDungeonProgress(S2C::dungeon_progress::step_type steps[], DWORD count);

protected:
	virtual	bool OnInitDialog();

	PAUILABEL	mMinLabStep;
	PAUILABEL	mMaxLabSteps[MAX_STEP_NUM];
	DWORD		m_dwDungeonProg;

};
