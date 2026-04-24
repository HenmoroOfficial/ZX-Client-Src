/********************************************************************
	created:	2005/08/15
	created:	15:8:2005   15:03
	file name:	DlgSkillTree.h
	author:		yaojun
	
	purpose:	
*********************************************************************/


#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIProgress.h"

class CDlgSkillTree : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
		
public:
	CDlgSkillTree();
	virtual ~CDlgSkillTree();
	
	void OnCommandFortNext(const char * szCommand);
	void OnCommandFortPrev(const char * szCommand);
	void OnCommandLeft(const char * szCommand);
	void OnCommandRight(const char * szCommand);
	void OnCommandAdd(const char * szCommand);
	void OnCommandRdoRace(const char * szCommand);
	void OnCommandTree(const char * szCommand);
	void OnCommandTalent(const char * szCommand);
	void OnCommandfortune(const char * szCommand);
	void OnCommandComboEdit(const char * szCommand);
	void OnCommandCancel(const char * szCommand);

	void OnCommandChangeProfTree(const char * szCommand);
	void OnCommandChangeProfTree2(const char * szCommand);
	void OnCommandChangeProfTree3(const char * szCommand);
	
	void OnCommandResetProp(const char * szCommand);

	void OnEventLButtonDown_Prof(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDown_Skill(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonDown_Skill(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	static CDlgSkillTree* m_pSkillTreeActive;
	static CDlgSkillTree* m_pTalentTreeActive;
	static CDlgSkillTree* m_pFortuneActive;

protected:
	virtual void OnTick();
	virtual bool Render();
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnShowDialog();

	PAUILABEL				m_pTxt_SkillPoint;
	PAUILABEL				m_pTxt_AddedSkillPoint;
	PAUISTILLIMAGEBUTTON	m_pBtn_LMove;
	PAUISTILLIMAGEBUTTON	m_pBtn_RMove;
};

extern const int talent[];
