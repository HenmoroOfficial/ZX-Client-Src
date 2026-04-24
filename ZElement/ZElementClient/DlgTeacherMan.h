// File		: DlgTeacherMan.h
// Creator	: Xiao Zhou
// Date		: 2008/2/19

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"

namespace GNET
{
	class SectList_Re;
	class SectInvite;
	class SectRecruit_Re;
	class SectExpel_Re;
	class SectQuit;
}

class CDlgTeacherMan : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgTeacherMan();
	virtual ~CDlgTeacherMan();

	void OnCommandAdd(const char * szCommand);
	void OnCommandKick(const char * szCommand);
	void OnCommandInvite(const char * szCommand);
	void OnCommandRefresh(const char * szCommand);

	void OnEventRButtonUpMember(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDownSkill(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonDownSkill(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void OnSectListRe(const GNET::SectList_Re* p);
	void OnSectInvite(const GNET::SectInvite* p);
	void OnSectRecruitRe(const GNET::SectRecruit_Re* p);
	void OnSectExpelRe(const GNET::SectExpel_Re* p);
	void OnSectQuit(const GNET::SectQuit* p);
	
	bool HasTeacher() { return m_pList_Member->GetCount()>0; }	// 2012.4.14 有师徒关系不能寄售物品
	
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual bool Render();
	virtual void OnShowDialog();

	PAUILISTBOX	m_pList_Member;
	PAUISTILLIMAGEBUTTON m_pBtn_Kick;
	PAUISTILLIMAGEBUTTON m_pBtn_Invite;
	PAUISTILLIMAGEBUTTON m_pBtn_Refresh;
	PAUILABEL m_pTxt_TeacherName;
	PAUILABEL m_pTxt_Line;
	PAUILABEL m_pTxt_StuNum;
	DWORD m_dwNextRefresh;
};
