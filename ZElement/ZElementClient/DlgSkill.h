/********************************************************************
	created:	2005/08/15
	created:	15:8:2005   13:22
	file name:	DlgSkill.h
	author:		yaojun
	
	purpose:	
*********************************************************************/


#pragma once

#include <vector.h>
#include <hashmap.h>
#include "DlgBase.h"

class CECSkill;
class AUIImagePicture;
class A2DSprite;
class CECIvtrEquip;

class CDlgSkill : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()

protected:
	typedef abase::vector<CECSkill *> SkillVector;

	void UpdatePositiveSkill(const SkillVector &sortedSkills);
	void UpdatePassiveSkill(const SkillVector &sortedSkills);
	void UpdateTalismanSkill(const SkillVector &sortedSkills);
	void UpdateTalentSkill(const SkillVector &sortedSkills);
	void UpdateFamilyPositiveSkill(const SkillVector &sortedSkills);
	void UpdateFamilyPassiveSkill(const SkillVector &sortedSkills);
	void UpdateSpouseSkill(const SkillVector &sortedSkills);
	void UpdateSectPositiveSkill(const SkillVector &sortedSkills);
	void UpdateSectPassiveSkill(const SkillVector &sortedSkills);
	void UpdateFlySkill(const SkillVector &sortedSkills);
	void UpdateMysteryPositiveSkill(const SkillVector &sortedSkills);
	void UpdateMysteryPassiveSkill(const SkillVector &sortedSkills);
	void UpdateLifePossitiveSkill(const SkillVector &sortedSkills);
	void UpdateLifePassiveSkill(const SkillVector &sortedSkills);
	void UpdateEquipSkill();

	virtual void OnShowDialog();

	static int		m_idSkillSelect;

public:
	CDlgSkill();
	virtual ~CDlgSkill();
	virtual bool Tick(void);
	virtual bool Render();
	virtual bool OnInitDialog();
	virtual bool Release();

	static A2DSprite* GetA2DSpriteSkillSelect(bool bBound=false) { return bBound ? m_pA2DSpriteSkillSelectRound : m_pA2DSpriteSkillSelect; }
	static A2DSprite* GetA2DSpriteSkillHilight(bool bBound=false) { return bBound ? m_pA2DSpriteSkillHilightRound : m_pA2DSpriteSkillHilight; }
	static A2DSprite* m_pA2DSpriteSkillSelect;
	static A2DSprite* m_pA2DSpriteSkillHilight;
	static A2DSprite* m_pA2DSpriteSkillSelectRound;
	static A2DSprite* m_pA2DSpriteSkillHilightRound;
	static A2DSprite* m_pA2DSpriteSkillSpecEquip;
	static A2DSprite* m_pA2DSpriteSkillSpecEquipLock;
	static A2DSprite* m_pA2DSpriteSkillRound3D;

	void SetImage(AUIImagePicture *pImage, CECSkill *pSkill, bool bPositiveSkill = false);
	static void SelectSkill(int id);
	static int GetSelectSkill();
	
	void OnEventLButtonDownInitiative(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonDownInitiative(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void OnCommandCancel(const char * szCommand);
	void OnCommandSkillTree(const char * szCommand);
	void OnCommandSkill(const char * szCommand);
	void OnCommandSkillOther(const char * szCommand);
	void OnCommandSkillSpec(const char * szCommand);
	void OnCommandSkillFortune(const char * szCommand);
	void OnCommandSkillCouple(const char * szCommand);
	void OnCommandSkillFortuneNext(const char * szCommand);
	void OnCommandSkillFortunePrev(const char * szCommand);

	void UpdateView();

	/**
	建一个表，包含身上穿的时装首饰激活的套装技能
	这个功能以后应该会改掉
	现在的套装技能都是3个都有生效，现在基于这个假设，简化代码
	*/
	void BuildEquipedSkill(CECIvtrEquip *pEquip, abase::hash_map<int, bool>& mapIds);
};

