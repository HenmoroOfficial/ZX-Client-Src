// Filename	: DlgAward.h
// Creator	: Tom Zhou
// Date		: October 11, 2005

#pragma once

#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "DlgBase.h"
#include "TaskTempl.h"

class CDlgAward : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgAward();
	virtual ~CDlgAward();

	void OnCommand_confirm(const char * szCommand);
	void OnCommand_select(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_back(const char * szCommand);
	void OnCommand_front(const char * szCommand);
	
	void ChooseAward(AWARD_DATA &ad);

protected:
	AWARD_DATA		m_Award;
	PAUISTILLIMAGEBUTTON m_pBtn_Confirm;

	PAUISTILLIMAGEBUTTON m_pBtn_Back;
	PAUISTILLIMAGEBUTTON m_pBtn_Front;

	unsigned long	m_ulAwardNum;
	unsigned long	m_ulValidPage;
	unsigned long	m_ulCurrPage;	//0,1,...

	virtual bool OnInitDialog();
	void ShowFeatAward();
};
