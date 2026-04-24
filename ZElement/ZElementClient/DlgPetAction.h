// File		: DlgPetAction.h
// Creator	: Xiao Zhou
// Date		: 2006/3/22

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "vector.h"

class CECShortcut;

class CDlgPetAction : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgPetAction();
	virtual ~CDlgPetAction();

	//	Moving mode
	enum
	{
		MOVE_FOLLOW = 0,
		MOVE_STAND,
	};

	//	Attacking mode
	enum
	{
		ATK_DEFENSE = 0,
		ATK_POSITIVE,
		ATK_PASSIVE,
	};
	
	void OnCommandAttack(const char * szCommand);
	void OnCommandFollow(const char * szCommand);
	void OnCommandStop(const char * szCommand);
	void OnCommandOffensive(const char * szCommand);
	void OnCommandDefensive(const char * szCommand);
	void OnCommandCombat(const char * szCommand);
	void OnCommandSave(const char * szCommand);
	void OnCommandDefault(const char * szCommand);
	void OnCommandOther(const char * szCommand);
	void OnCommandPetSetting(const char * szCommand);
	void OnCommandCancel(const char * szCommand);
	
	void CastSkill(int nIndex, bool bAutoCast);
	void SetAutoFood(CECShortcut *pSC);

	void OnEventLButtonDown_Skill(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonDown_Skill(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown_AutoFood(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDblclk_AutoFood(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
	static void SetPetAIState(char stay_state, char aggro_state);
	static void SetAutoCastID(int id, unsigned char flag);
	static void ClearAutoCastID();

	static abase::vector<int>	m_vecAutoCastID;

protected:
	virtual void OnTick();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual bool Release();

	static bool IsSkillAutoCast(int id);

	PAUIIMAGEPICTURE		m_pImg_Skill[8];
	static int				m_idLastTarget;
	static int				m_idLastSkill;
	static DWORD			m_dwLastSkillTime;
	static int				m_nStayState;
	static int				m_nAggroState;
};
