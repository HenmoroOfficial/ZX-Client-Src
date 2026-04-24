// Filename	:	DlgUpgrade.h
// Creator	:	Fu Chonggang
// Date		:	2012.5.19
// 每次更新内容

#pragma once

#include "DlgBase.h"

class AUITextArea;

class CDlgFormatText : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgFormatText();
	virtual ~CDlgFormatText();
	
	void OnEventLButtonDownText(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventMouseMoveText(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
protected:
	
	// 寻径格式： &location(周一仙,300,250,240,1)& 类似任务描述
	void UpdateTextArea(AUITextArea*);
};

class CDlgUpgrade : public CDlgFormatText  
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgUpgrade() {}
	virtual ~CDlgUpgrade() {}
	
	void OnCommandLink(const char* szCommand);
	void OnCommandCancel(const char* szCommand);
	
protected:
	virtual bool OnInitDialog();
};

