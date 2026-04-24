/*
 * FILE: DLGSETTINGEFFECT.CPP
 *
 * DESCRIPTION: settings of effect, for debug
 *
 * CREATED BY: hanwei, 2012/2/22
 *
 * HISTORY:
 *
 * Copyright (c) 2010 Hummingbird Studio, All Rights Reserved.
 */
#include "DlgBase.h"
#include "ElementRender.h"


class CDlgSettingEffect : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgSettingEffect();
	virtual ~CDlgSettingEffect();

	void OnCommandConfirm(const char *szCommand);

	A3DPOST_EFFECT_PARAM* GetParam() { return &m_PostEffectParam; }

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void DoDataExchange(bool bSave);
	//virtual void InitSettingInfo(CDlgSettings* pSettings);
	virtual void SetToDefault();
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

private:

	A3DPOST_EFFECT_PARAM m_PostEffectParam;
};