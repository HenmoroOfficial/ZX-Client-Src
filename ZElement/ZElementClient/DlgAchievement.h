// Filename	: DlgAchievement.cpp
// Creator	: Fu Chonggang
// Date		: 2009/7/9

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"

class CLuaState;
class CECPlayer;

class CDlgAchievement : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

	enum
	{
		ACHIEVEMENT_REWARD_MAX = 3
	};

public:
	CDlgAchievement();
	virtual ~CDlgAchievement();

	void OnCommandRdoType(const char* szCommand);
	void OnCommandCancel(const char* szCommand);
	void OnEventLButtonDown_Tv_Lst(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void SetTarget(int iTarget) { m_iTarget = iTarget; m_bSet = false;}

protected:
	virtual void OnTick();
	virtual	bool OnInitDialog();
	virtual void OnShowDialog();

	PAUILABEL				m_pLab_Point;
	PAUITREEVIEW			m_pTv_List;
	PAUITEXTAREA			m_pTxt_Desc;
	PAUITEXTAREA			m_pTxt_Cond;
	PAUIIMAGEPICTURE		m_pImg_Item[ACHIEVEMENT_REWARD_MAX];
	PAUILABEL				m_pLab_AwardBase;
	PAUILABEL				m_pLab_AwardMoney;
	int						m_iTarget;
	bool					m_bSet;
	CECPlayer				*m_pPlayer;
};
