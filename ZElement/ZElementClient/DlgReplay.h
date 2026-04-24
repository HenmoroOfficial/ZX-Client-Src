// Filename	: DlgReplay.h
// Creator	: Xiao Zhou
// Date		: 2006/10/12

#pragma once

#include "DlgBase.h"
#include "AUI\\AUISlider.h"
#include "AUI\\AUILabel.h"

class CDlgReplay : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgReplay();
	virtual ~CDlgReplay();

	void OnCommandSpeed(const char * szCommand);
	void OnCommandPlay(const char * szCommand);
	void OnCommandPause(const char * szCommand);
	void OnCommandStart(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDBCLKCamera(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

protected:
	virtual void OnTick();
	virtual void OnShowDialog();
	virtual bool OnInitDialog();

	PAUISLIDER		m_pSlider_Time;
	PAUILABEL		m_pTxt_Time;
	int				m_nLevel;
	bool			m_bDrag;
};
