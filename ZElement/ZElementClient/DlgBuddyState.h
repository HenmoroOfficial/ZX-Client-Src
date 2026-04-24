// Filename	: DlgBuddyState.h
// Creator	: Xiao Zhou
// Date		: 2005/12/13

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"

class CDlgBuddyState : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	
	enum
	{
		QA_BUDDYSTATE,
		QA_RIDETOGETHER,
	};
	
	CDlgBuddyState();
	virtual ~CDlgBuddyState();

	void OnCommand_Cancel(const char * szCommand);
	void OnCommand_Btn1(const char * szCommand);
	void OnCommand_Btn2(const char * szCommand);
	void OnCommand_Kick(const char * szCommand);
	void OnCommandMinimize(const char * szCommand);

	void SetBuddyID(int idPlayer, int iType);
	bool IsRMinimized() { return m_bMinimized; }

protected:
	virtual void OnTick();
	virtual bool OnInitDialog();

	bool				m_bMinimized;
	int					m_iQAState;
	PAUIIMAGEPICTURE	m_pImg_Prof;
	PAUILABEL			m_pTxt_Name;
	PAUISTILLIMAGEBUTTON	m_pBtn_Kiss;
	PAUISTILLIMAGEBUTTON	m_pBtn_Cancel;
};


class CDlgQilinBuddy : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();

public:

	CDlgQilinBuddy();
	virtual ~CDlgQilinBuddy();
	
	void OnCommand_BindCancel(const char * szCommand);
	void OnCommand_QilinUp(const char * szCommand);

protected:
	virtual void OnTick();
};