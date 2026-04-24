// Filename	: DlgBattleDungeonTowerScore.h
// Creator	: Fu Chonggang
// Date		: 2012.4.19
// Desc		: 爬塔副本积分详细情况

#pragma once

#include "DlgBase.h"

class AUIStillImageButton;

class CDlgBattleDungeonTowerScore : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgBattleDungeonTowerScore();
	virtual ~CDlgBattleDungeonTowerScore();

	void OnCommandMainOnce(const char * szCommand);
	void OnCommandOnce(const char * szCommand);
	void OnCommandPrefOnce(const char * szCommand);
	void OnCommandNextOnce(const char * szCommand);
	void OnCommandGetOnce(const char * szCommand);

	void OnCommandMainDaily(const char * szCommand);
	void OnCommandDaily(const char * szCommand);
	void OnCommandPrefDaily(const char * szCommand);
	void OnCommandNextDaily(const char * szCommand);	
	void OnCommandGetDaily(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	int GetLvBaseOnce();
	int GetLvBaseDaily();
	void UpdateBaseDisplayOnce();
	void UpdateBaseDisplayDaily();
	void UpdateDisplay();

	int	m_iStartLvOnceIndex;
	int	m_iStartLvDailyIndex;
	int m_iStartLvDailyToday;	// -9-0
	AUIStillImageButton*	m_pBtnPrevOnce;
	AUIStillImageButton*	m_pBtnNextOnce;
	AUIStillImageButton*	m_pBtnPrevDaily;
	AUIStillImageButton*	m_pBtnNextDaily;

	int m_iLevelDaily;
	int m_iLevelOnce;
	static const int ms_iMaxLevel;
};
