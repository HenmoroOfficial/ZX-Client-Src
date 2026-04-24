// File		: DlgBattleTerri.h
// Creator	: Fuchonggang
// Date		: 2010/03/10

#pragma once

#include "DlgBase.h"

class AUITextArea;
class AUIStillImageButton;
class AUILabel;
class AUIListBox;
class AUIProgress;
class CDlgBattleTerr : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgBattleTerr();
	virtual ~CDlgBattleTerr();
	
	void OnCommandDeclare(const char * szCommand);
	void OnCommandAdd(const char * szCommand);
	void OnCommandAddMax(const char * szCommand);
	void OnCommandAddConfirm(const char * szCommand);
	void OnCommandGet(const char * szCommand);
	void OnCommandBattleJoin1(const char * szCommand);
	void OnCommandBattleJoin2(const char * szCommand);
	void OnCommandBattleLeave(const char * szCommand);
	void OnCommandRefresh(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
protected:
	virtual void OnTick();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

private:
	static int m_nTerrIndex;
	static int m_nTerrId;
	static int m_nItemId;
	static int m_nItemMin;
	static int m_nItemMax;
	static int m_nItemCur;
};

struct _DOMAIN_INFO;
class CDlgBattleTerrInfo : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	enum
	{
		STATUS_NONE = 0,		// --
		STATUS_DECLARE,			// 宣战中
		STATUS_DECLARED,		// 被宣战
		STATUS_DECLARESUCCESS,	// 宣战成功
		STATUS_FIGHTING,		// 战斗进行
		STATUS_DEFENCERWIN,		// 守方胜
		STATUS_ATTACKERWIN,		// 攻方胜
		STATUS_AWARD,			// 神器降临
		STATUS_WIN,				// 胜利
	};
	struct BattleTerrInfo
	{
		int			nId;
		ACString	strName;
		int			nLevel;
		int			nState;
		int			nTimeStart;
		int			nTimeEnd;
		int			nDefencer;
		int			nAttacker;
	};

	CDlgBattleTerrInfo();
	virtual ~CDlgBattleTerrInfo();
	
	void OnCommandRefresh(const char * szCommand);
	void OnCommandHome(const char * szCommand);
	void OnCommandEnd(const char * szCommand);
	void OnCommandNext(const char * szCommand);
	void OnCommandLast(const char * szCommand);

	void OnCommandTerriDetail(const char * szCommand);
	void OnCommandGuildDetail(const char * szCommand);
	void OnCommandSortName(const char * szCommand);
	void OnCommandSortLevel(const char * szCommand);
	void OnCommandSortStatus(const char * szCommand);
	void OnCommandSortWarTime(const char * szCommand);
	void OnCommandSortDefencer(const char * szCommand);
	void OnCommandSortAttacker(const char * szCommand);
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	void UpdateInfo();
	void UpdateList();
	inline int CalStatus(_DOMAIN_INFO&);
	
private:
	AUIListBox*		m_pLst;
	AUIProgress*	m_pPro_GuildTerrScore;
	AUILabel*		m_pTxt_GuildTerrScore;
	AUILabel*		m_pTxt_1;
	AUILabel*		m_pTxt_2;
	AUILabel*		m_pTxt_3;

	abase::vector<BattleTerrInfo> m_vecInfo;
	int m_nPageCur;
	int m_nPageTotal;
	static DWORD m_dwRefreshTime;
};
