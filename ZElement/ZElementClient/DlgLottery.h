// Filename	: DlgLottery.h
// Creator	: Xiao Zhou
// Date		: 2006/10/18

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"

class CLuaState;

class CDlgLottery : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgLottery();
	virtual ~CDlgLottery();

	void OnCommandCancel(const char* szCommand);
	void OnCommandSelect(const char* szCommand);
	void OnCommandGetAward(const char* szCommand);

	void SetAward(int iLevel, int idItem, int iCount, int iMoney);

protected:
	virtual void OnTick();
	virtual	bool OnInitDialog();
	virtual void OnShowDialog();
	virtual	bool Release();

	PAUISTILLIMAGEBUTTON	m_pBtn_Select;
	PAUISTILLIMAGEBUTTON	m_pBtn_Select1;
	PAUISTILLIMAGEBUTTON	m_pBtn_GetAward;
	PAUILABEL				m_pTxt_AwardLevel;
	PAUILABEL				m_pTxt_Award;
	PAUILABEL				m_pTxt_AwardMoney;
	PAUILABEL				m_pTxt_Title;
	PAUIIMAGEPICTURE		m_pImg_Item;
	PAUIIMAGEPICTURE		m_pImg_Word[3];

	CLuaState * m_pState;
};
