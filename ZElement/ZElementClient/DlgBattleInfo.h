// File		: DlgBattleInfo.h
// Creator	: Xiao Zhou
// Date		: 2007/8/6

#pragma once

#include "DlgBase.h"

class AUILabel;
class AUIImagePicture;

class CDlgBattleInfo : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgBattleInfo();
	virtual ~CDlgBattleInfo();

	void OnCommandGiveup(const char * szCommand);
	void OnCommandChoice(const char * szCommand);
	void OnCommandJoin(const char * szCommand);
	void OnCommandRefresh(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	void OnCommandExit(const char * szCommand);

	void OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEvent(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void UpdateBattleInfo(void *pData);
	void UpdateBattleScorePK2013(void *pData);
	void UpdateBattleScore(void *pData);
	void UpdateBattleScorePK2011(void *pData);
	void UpdateBattleList(void *pData, int type = -1);
	void UpdataCrossServerBattleScore(void *pData);
	int GetJoinType() { return m_iJoinType; }
	void SetJoinType(int iType) { m_iJoinType = iType; }

	void UpDateBattleOffLineInfo(void* pData);
	bool m_bisBattleUIType;
	void UpDataPKScoreBattleInfo(void* pData);
	void UpdataCrossServerPKBattleScore(void *pData);
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual void OnShowDialog();
	int m_iMaxNum;
	
	AUILabel *m_pTxtScore01;				// 我方分数
	AUILabel *m_pTxtScore02;				// 敌方分数
	AUILabel *m_pTxtPlayer01;				// 我方玩家人数
	AUILabel *m_pTxtPlayer02;				// 敌方玩家人数
	AUILabel *m_pTxtKillOwn;				// 我方杀人总数
	AUILabel *m_pTxtKillEnemy;				// 敌方杀人总数
	AUILabel *m_pTxtOffLineOwn;				// 我方离线总数
	AUILabel *m_pTxtOffLineEnemy;				// 敌方离线总数
	AUILabel *m_pTxtKill;					// 个人击杀
	AUILabel *m_pTxtDead;					// 个人死亡
	AUILabel *m_TxtTimeLeft;				// 时间


	AUILabel *m_pLabKillOwn;
	AUILabel *m_pLabKillEnemy;
	AUILabel *m_pLabKill;
	AUILabel *m_pLabDead;
	AUILabel *m_pLabTimeLeft;


	static int m_iJoinType; //1-进攻， 2-防守， 3-观战
	int m_iMinPlayerDeduceScore;

	static int	m_iEnterTimes;
	static int	m_iBattleType;

	int m_nSlefAttacker_count;
};

class CDlgBattlePkScore : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
public:
	enum
	{
		TOTAL_ROUND_NUM2011 = 5,
	};
public:
	CDlgBattlePkScore();
	virtual ~CDlgBattlePkScore();
	
	void OnCommandExpand(const char * szCommand);
	
	void UpdateBattleScore2011OnLine(void *pData);
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	
	AUILabel *m_pLabScoreStage;
	AUILabel *m_pLabStageNo;
	AUILabel *m_pLabScoreCurRound;
	AUILabel *m_pLabTime;
	AUILabel *m_pLabRedScore[TOTAL_ROUND_NUM2011];
	AUILabel *m_pLabBlueScore[TOTAL_ROUND_NUM2011];
	AUIImagePicture	*m_pImgState;
	
	static int	m_iTimeEnd;
};

class CDlgBattlePkScoreResult2011 : public CDlgBase  
{
public:
	CDlgBattlePkScoreResult2011();
	virtual ~CDlgBattlePkScoreResult2011();

protected:
	virtual void OnShowDialog();
	virtual void OnTick();

	UINT		m_iShowTime;
	bool		m_bNormal;
	int			m_iVip;
	static const int	SHOW_TIME;
	static const int	APPEAR_TIME;
};