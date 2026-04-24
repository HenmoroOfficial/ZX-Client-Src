// Filename	: DlgTaskList.h
// Creator	: Xiao Zhou
// Date		: 2006/6/1

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIListBox.h"
#include "DlgTaskBase.h"

class CDlgTaskList : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgTaskList();
	virtual ~CDlgTaskList();

	void OnCommand_Accept(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

protected:
	PAUILISTBOX m_pLst_Quest;
	PAUITEXTAREA m_pTxt_Content;
	PAUILABEL m_pTxt_BaseAward;
	PAUILABEL m_pTxt_ItemAward;
	PAUIIMAGEPICTURE m_pImg_Item[CDlgTaskBase::CDLGTASK_AWARDITEM_MAX];
	
	int m_idSelTask;

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
};
