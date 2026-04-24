// Filename	: DlgBattleTeamCustom.h
// Creator	: Fu Chonggang
// Date		: 2012.7.30

#pragma once

#include "DlgTab3D.h"
#include "teamraidroledata"
#include "vector.h"

class AUILabel;
class CECSkill;
struct COLLISION_RAID_TRANSFIGURE_CONFIG;

class CDlgBattleTeamCustom : public CDlgTab3D  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgBattleTeamCustom();
	virtual ~CDlgBattleTeamCustom();

	void OnCommandConfirm(const char * szCommand);
	void OnCommandSkillSel(const char * szCommand);
	void OnCommandMinimize(const char * szCommand);
	
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	virtual void AddTextrures();
	virtual void UpdateDisplay();
	virtual void OnSelOneItem();

	CECSkill* GetSkill(int index) { return m_pSkills[index]; }
	CECSkill* GetSkillById(int id);
	
protected:
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnTick();

	void UpdateChars();
	void GenerateHeadPath(AString &strPath);

	abase::vector<GNET::TeamRaidRoleData> m_vecRoles;
	COLLISION_RAID_TRANSFIGURE_CONFIG	*m_pConfig;
	int			m_iFigureNum;				// 模型总数
	int			m_iTimeToStart;
	AUILabel	*m_pLabTime;
	
	CECSkill	*m_pSkills[6];
	static const int ms_iSkillIds[6];
};


class CDlgBattleTeam6v6Skill : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
	
public:
	CDlgBattleTeam6v6Skill();
	virtual ~CDlgBattleTeam6v6Skill();
	
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void InitSkills();
	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
};