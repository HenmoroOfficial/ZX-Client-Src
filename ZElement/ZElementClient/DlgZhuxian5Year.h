// Filename	: DlgZhuxian5Year.h
// Creator	: Fu Chonggang
// Date		: 2012/3/16
// Desc		: 5周年每日签到

#pragma once

#include "DlgBase.h"

class CDlgZhuxian5Year : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgZhuxian5Year();
	virtual ~CDlgZhuxian5Year();

	void OnCommand_Active(const char * szCommand);
	void OnCommand_Get(const char * szCommand);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	void UpdateDisplay();

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	int		m_iStars;			// 当前多少星
	int		m_iState;			// 0:初始	1:点击恭喜按钮	2:点击领奖按钮
	static const int ms_MaxStars;
};

class CDlgZhuxian5YearMin : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgZhuxian5YearMin();
	virtual ~CDlgZhuxian5YearMin();
	void OnCommand_Active(const char * szCommand);
};