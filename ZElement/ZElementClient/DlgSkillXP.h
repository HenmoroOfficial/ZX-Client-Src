// Filename	:	DlgSkillXP.h
// Creator	:	Wu Weixin
// Date		:	2013/04/11
// Description:	xp¼¼ÄÜÀ¸

#pragma once

#include "DlgBase.h"

class AUIImagePicture;
class AUIProgress;
class AUIObject;

class CDlgSkillXP : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()

public:

	CDlgSkillXP();
	~CDlgSkillXP();

	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	virtual void OnTick();

	int Prof2Index(int prof);

	void OnEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

protected:

	void SetSkill();

	AUIImagePicture*	m_pImg_Xp;
	AUIProgress*		m_pPrgs_01;
	AUIObject*			m_pGfx_01;
};