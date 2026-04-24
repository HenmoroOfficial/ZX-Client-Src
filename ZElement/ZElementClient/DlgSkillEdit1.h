// File		: DlgSkillEdit1.h
// Creator	: Fu Chonggang
// Date		: 2010/3/29
// 天华组合技能编辑

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "EC_Configs.h"

class CECSkill;

class CDlgSkillComboEdit : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

	enum
	{
		COMBOEDIT_ELEMENT = 7,
		COMBOEDIT_NUM = 10,
	};
public:
	CDlgSkillComboEdit();
	virtual ~CDlgSkillComboEdit();

	void EditSkill(int id, int level);
	void DragDropItem(PAUIDIALOG pDlgSrc, PAUIOBJECT pObjSrc, PAUIOBJECT pObjOver);

	void OnCommandConfirm(const char * szCommand);

	void OnEventLButtonDown_Item(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDown_Icon(WPARAM wParam, LPARAM lParam, AUIObject * pObj);


protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual bool Release();

	PAUIIMAGEPICTURE	m_pImgSkillElement[COMBOEDIT_ELEMENT];
	PAUIIMAGEPICTURE	m_pImgSkillEdit[COMBOEDIT_NUM];
	PAUISTILLIMAGEBUTTON m_pBtnConfirm;
	PAUILABEL			m_pLabTime;
	CECSkill*			m_pSkill;
	int					m_nTime;

	int					m_nEleNum;
	int					m_nEditNum;
};
