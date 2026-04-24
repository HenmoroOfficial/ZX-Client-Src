// File		: DlgBattleSong.h
// Creator	: Fuchonggang
// Date		: 2009/04/28

#pragma once

#include "DlgBase.h"

class AUILabel;
class AUIListBox;
class CDlgBattleSong : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
public:
	struct BattleFieldInfo
	{
		ACString strName;
		unsigned char occupaton;
		unsigned int level;
	};

public:
	CDlgBattleSong();
	virtual ~CDlgBattleSong();

	void OnCommandGiveup(const char * szCommand);
	void OnCommandConfirm(const char * szCommand);
	void OnCommand_SortName(const char * szCommand);
	void OnCommand_SortOccupation(const char * szCommand);
	void OnCommand_SortLevel(const char * szCommand);
	void OnCommand_Tail(const char * szCommand);
	void OnCommand_Head(const char * szCommand);
	void OnCommand_Next(const char * szCommand);
	void OnCommand_Last(const char * szCommand);
	void OnCommandCancel(const char * szCommand);

	void OnEventLButtonDownBattle(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
	void UpdateSongBattleInfo(void *pData);
	void UpdateBattleList(void *pData);
	
	void UpdateSongSelfInfo(void *pData);
	void UpdateSongBattleScore(void *pData);

	int GetMapID() { return m_iMapId; }

protected:
	virtual void OnTick();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	void RefreshBattleInfo();

private:
	int			m_iMapId;
	abase::vector<BattleFieldInfo>	m_vecBattleFieldInfo;
	int			m_iCurPageBattleField;
	int			m_iMaxPageBattleField;

	AUIListBox *m_pLstSongBattle;
	static AUILabel *m_pLabTime;

	static AUILabel *m_pLabPoint;
	static AUILabel *m_pLabType;
	static AUILabel *m_pLabKill;
	static AUILabel *m_pLabRank;
	static AUILabel *m_pLabChuPoint;
	static AUILabel *m_pLabHanPoint;
	static AUILabel *m_pLabChuNum;
	static AUILabel *m_pLabHanNum;
	static AUIListBox *m_pLstScore;
};
