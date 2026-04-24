//特别技能相关的界面
#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"

class CDlgSkillTeleport : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgSkillTeleport();
	virtual ~CDlgSkillTeleport();

	void OnCommandCancel(const char * szCommand);
	void OnCommandConfirm(const char * szCommand);
	void SetNpcId(int id);

protected:
	virtual void OnTick();

	int m_idNPC;
};

class CDlgSkillRemote : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgSkillRemote();
	virtual ~CDlgSkillRemote();
	
	void OnCommandConfirm(const char * szCommand);
	void SetTrapInfo(int iTrapId, int iTratTid, int iTime);
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	int			m_iTime;
	DWORD		m_dwStartTime;
	int			m_iTrapId;
	int			m_iTrapTid;
	AUILabel*	m_pLabTime;
};



