// Filename	:	DlgSkillzx2.h
// Creator	:	Fu Chonggang
// Date		:	Aug 27, 2009

#pragma once


#include "DlgBase.h"
#include "AUI\\AUISubDialog.h"

class AUILabel;
class AUIStillImageButton;
class AUIImagePicture;
class AUIEditBox;
class AUIRadioButton;

class CDlgSkillMan : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgSkillMan();
	virtual ~CDlgSkillMan();

	virtual int OnChangeUIMan(BYTE bUserChanged, BYTE bRoleChanged, BYTE bLineChanged);
	virtual bool Release();

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnCommand_RdoType(const char * szCommand);

	void OnCommand_desc(const char * szCommand);
	void OnCommand_talent(const char * szCommand);
	void OnCommand_skilltree(const char * szCommand);

	void OnCommand_taoism(const char * szCommand);
	void OnCommand_evil(const char * szCommand);
	void OnCommand_buddhism(const char * szCommand);

	void OnCommand_couple(const char * szCommand);
	void OnCommand_family(const char * szCommand);
	void OnCommand_teacher(const char * szCommand);

	void OnCommand_talisman(const char * szCommand);
	void OnCommand_edit(const char * szCommand);
	void OnCommand_life(const char * szCommand);
	void OnCommand_article(const char * szCommand);

	void OnCommand_SoulTaoism(const char * szCommand);
	void OnCommand_SoulEvil(const char * szCommand);
	void OnCommand_SoulBuddhism(const char * szCommand);
	
	void OnCommand_SkillSeq(const char * szCommand);

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_close(const char * szCommand);
	
	PAUIDIALOG GetDialogLeft() { return m_pSubDLeft->GetSubDialog(); }
	void SetDialogLeft(PAUIDIALOG pDlg) { m_pSubDLeft->SetDialog(pDlg); }	
	PAUIDIALOG GetDialogRight() { return m_pSubDRight->GetSubDialog(); }
	void SetDialogRight(PAUIDIALOG pDlg) { m_pSubDRight->SetDialog(pDlg); }

	static int	Prof2Index(int prof);
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();

	AUILabel *m_pTitle;
	AUIStillImageButton *m_pBtn_Close;
	AUIImagePicture *m_pImg_Bar;

	AUIRadioButton *m_pRdo_Desc;
	AUIStillImageButton *m_pBtn_Talent;
	AUIRadioButton *m_pRdo_Skilltree;

	AUIRadioButton *m_pRdo_Taoism;
	AUIRadioButton *m_pRdo_Evil;
	AUIRadioButton *m_pRdo_Buddhism;

	AUIRadioButton *m_pRdo_Couple;
	AUIRadioButton *m_pRdo_Teacher;

	AUIRadioButton *m_pRdo_Talisman;	// 法宝、生活技能
	AUIRadioButton *m_pRdo_Edit;		// 时装技能
	
	AUIRadioButton *m_pRdo_SoulTaoism;
	AUIRadioButton *m_pRdo_SoulEvil;
	AUIRadioButton *m_pRdo_SoulBuddhism;

	AUIEditBox *m_pShape1;
	AUIImagePicture *m_pImg_Line2;
	AUIRadioButton *m_pRdo_1;
	AUIRadioButton *m_pRdo_2;
	AUIRadioButton *m_pRdo_3;
	AUIRadioButton *m_pRdo_4;

	AUISubDialog   *m_pSubDLeft;
	AUISubDialog   *m_pSubDRight;
	AUISubDialog   *m_pSubDMiddle;

	PAUIDIALOG m_pDlgLeft;
	PAUIDIALOG m_pDlgRight;
	PAUIDIALOG m_pDlgMiddle;
};
