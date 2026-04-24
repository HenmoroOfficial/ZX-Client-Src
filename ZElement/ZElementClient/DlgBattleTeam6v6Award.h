// Filename	: DlgBattleTeam6v6Award.h
// Creator	: Fu Chonggang
// Date		: 

#pragma once

#include "DlgBase.h"

class AUIObject;
class A2DSprite;

class CDlgBattleTeam6v6Award : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgBattleTeam6v6Award();
	virtual ~CDlgBattleTeam6v6Award();

	void OnCommand_GetDaily(const char * szCommand);
	void OnCommand_GetNormal(const char * szCommand);
	
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnTick();

	void BuildAwardsFromTmpl();

private:	
	int m_iScore_a;
	int m_iScore_b;
	int m_iWin_num;
	int m_iLost_num;
	int m_iDraw_num;
	int m_iDaily_award_timestamp;		// 上次领取每日奖励时间戳
	int m_iLast_collision_timestamp;	// 上次参加碰撞战场时间戳
	int m_iDayWin;

	int	m_iCurSel;

	AUIObject *m_pObjDaily;
	AUIObject *m_pObjScore;

	A2DSprite *m_pSprite;
};
