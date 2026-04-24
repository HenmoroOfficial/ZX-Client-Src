// File		: DlgSkillEdit.h
// Creator	: Xiao Zhou
// Date		: 2005/12/20

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIImagePicture.h"
#include "EC_Configs.h"

class CDlgSkillEdit : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgSkillEdit();
	virtual ~CDlgSkillEdit();

	void DragDropItem(PAUIDIALOG pDlgSrc, PAUIOBJECT pObjSrc, PAUIOBJECT pObjOver);
	void SelectSkill(int n);
	void SelectComboSkill(int n);

	void OnCommandConfirm(const char * szCommand);
	void OnCommandMoveLeft(const char * szCommand);
	void OnCommandMoveRight(const char * szCommand);
	void OnCommandEdit(const char * szCommand);
	void OnCommandDelete(const char * szCommand);
	void OnCommandNew(const char * szCommand);
	void OnCommandCancel(const char * szCommand);

	void OnEventLButtonDown_Item(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDown_Icon(WPARAM wParam, LPARAM lParam, AUIObject * pObj);


protected:
	virtual void OnShowDialog();
	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual bool Render();

	void SetSpecialIcon(PAUIIMAGEPICTURE pImage, int iType);
	void UpdateSkill();
	void UpdateComboSkill();

	PAUIIMAGEPICTURE	m_pImg_SkillCont[EC_COMBOSKILL_NUM];
	PAUIIMAGEPICTURE	m_pImg_Item;
	PAUIEDITBOX			m_pTxt_Name;
	int					m_nItemSelect;
	int					m_nIcon;
	int					m_nComboSelect;
};
