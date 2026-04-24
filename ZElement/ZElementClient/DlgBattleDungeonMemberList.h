/**
* @File DlgBattleDungeonMemberList.h
* @Brief 副本中玩家列表
* @Author Fu Chonggang
* @Date 2011-7-25 10:34:39
* @Copyright (c) 2011 ZhuXian Studio, All Rights Reserved. 
*/
#pragma once

#include "DlgBase.h"

class AUILabel;
class AUIListBox;

class CDlgBattleDungeonMemberList : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgBattleDungeonMemberList();
	virtual ~CDlgBattleDungeonMemberList();
	
	void OnCommandLeave(const char * szCommand);
	void OnCommandKick(const char * szCommand);
	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual void OnTick();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

private:
	AUILabel*	m_pLabTimeLeft;
	AUIListBox*	m_pLstMember;
};
