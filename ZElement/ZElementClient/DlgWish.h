#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIEditBox.h"
#include "EC_IvtrTypes.h"

class CDlgWish : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgWish();
	virtual ~CDlgWish();

	void OnCommand_wish(const char * szCommand);
protected:
	AUIEditBox* m_pEdt;
	abase::vector<ACString>	m_vecSearchWord;


	virtual bool OnInitDialog();
	virtual void OnShowDialog();
};