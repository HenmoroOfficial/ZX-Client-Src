// File		: DlgTalisman.h
// Creator	: Xiao Zhou
// Date		: 2007/4/25

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIImagePicture.h"
#include <vector.h>

class CECSkill;
class CECShortcut;

#define CDLGTALISMAN_SKILL_MAX 2
#define CDLGTALISMAN_SKILL_ADDON_MAX 14

class CDlgTalisman : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgTalisman();
	virtual ~CDlgTalisman();

	void OnCommandOther(const char* szCommand);
	void OnCommandBodyChange(const char* szCommand);
	void OnCommandSetting(const char* szCommand);
	void OnCommandZoarium(const char* szCommand);
	void OnCommandFly(const char* szCommand);
	void OnCommandAddMonster(const char* szCommand);
	void OnCommandDeleteMonster(const char* szCommand);
	void OnCommandDeleteAllMonster(const char* szCommand);
	void OnCommandAddMatter(const char* szCommand);
	void OnCommandDeleteMatter(const char* szCommand);
	void OnCommandDeleteAllMatter(const char* szCommand);
	void OnCommandQuickPick(const char* szCommand);
	void OnCommandChangeType(const char* szCommand);
	void OnCommandShield(const char* szCommand);
	void OnCommandConfirm(const char* szCommand);
	void OnCommandConfirmPick(const char* szCommand);
	void OnCommandCancel(const char* szCommand);
	
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
	int UseTalismanSkill(int nSlot);
	void UseTalismanItem(int nSlot);
	
	int CheckTalismanSkill(int nSlot);

	void SetTalismanItem(AUIObject* pObj, CECShortcut *pSC);
	void SetTalismanItem(char* szName, CECShortcut *pSC);
	
	void UpdateMovedItemSC(int tidItem, int iSrcIvtr, int iSrcSlot, int iDstIvtr, int iDstSlot);
	void UpdateRemovedItemSC(int tidItem, int iIvtr, int iSlot, int iSameItem);
	void UpdateExchangedItemSC(int tidItem1, int iIvtr1, int iSlot1, int tidItem2, int iIvtr2, int iSlot2);

	// 法宝技能快捷栏变身技能特殊处理
	void UpdateRemovedSkill(int id);	// 去掉技能
	// 原来id的技能替换为pSkill
	void UpdateExchangeSkill(int id, CECSkill *pSkill);

	void SetSkillSel(int index) { m_iSkillIndex=index; }

protected:
	typedef abase::vector<CECSkill *> SkillVector;

	virtual void OnTick();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual bool Release();

	int GetSkillByAddon(int idAddOn);

	PAUIPROGRESS m_pPrgs_Power;
	PAUIPROGRESS m_pPrgs_Exp;
	PAUIPROGRESS m_pPrgs_FlierExp;
	PAUIIMAGEPICTURE m_pImg_Skill[CDLGTALISMAN_SKILL_ADDON_MAX];
	DWORD		m_dwChangeCoolTime;
	BOOL		m_bCanChange;

	int			m_iSkillIndex;
};

class CDlgTalismanSkillSel : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
		
public:
	CDlgTalismanSkillSel() {}
	virtual ~CDlgTalismanSkillSel() {}
		
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
protected:
	virtual void OnTick();
};