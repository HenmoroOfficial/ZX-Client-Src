// Filename	:	DlgLevelUp.h
// Creator	:	Lei Dongyu
// Date		:	Mar 30, 2012

#pragma once


#include "DlgBase.h"

class AUILabel;
class AUIListBox;
class AUIStillImageButton;

class CDlgLevelUp : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
		
public:
	CDlgLevelUp();
	virtual ~CDlgLevelUp();

	virtual bool Release();
	
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_SkillWindow(const char * szCommand);
	void OnCommand_TalentWindow(const char * szCommand);
	void SetRebornCount(int i);
	int GetRebornCount();

	void SetClass(int);
	int GetClass();
	void SetLevel(int);
	int GetLevel();

	void SetRebornType(bool b);  // 2012-10-18 add √÷≤π∑……˝÷µ by zy	

	bool GetRebornType();// 2012-10-18 add √÷≤π∑……˝÷µ by zy	
	
	void SetRebornLevel(int i);  // 2012-10-18 add √÷≤π∑……˝÷µ by zy	
	int GetLRebornLevel(); // 2012-10-18 add √÷≤π∑……˝÷µ by zy	

	void SetRebornClass(int i);  // 2012-10-18 add √÷≤π∑……˝÷µ by zy	
	int GetLRebornClass(); // 2012-10-18 add √÷≤π∑……˝÷µ by zy	
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();
	
	AUIListBox *m_pLst_Skill;
	AUIListBox *m_pLst_Detail;
	AUIStillImageButton* m_pBtn_Skill;
	AUIStillImageButton* m_pBtn_Talent;
	AUILabel* m_pLab_LevelBefore;
	AUILabel* m_pLab_LevelAfter;

	int m_iOldLevel;
	int m_iOldClass;
	int m_iNewLevel;
	int m_iNewClass;
	int m_iRebornCount;
	int m_iOldRebornCount;
	

	bool m_bRebornType;
	int m_iRebornLevel;
	int m_iRebornClass;
};
