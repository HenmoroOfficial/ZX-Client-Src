// Filename	: DlgCharacterDungeonAward.h
// Creator	: Fu Chonggang
// Date		: 2012.4.19
// Desc		: 人物爬塔副本奖励界面

#pragma once

#include "DlgBase.h"

class AUIStillImageButton;
class AUIListBox;

class CDlgCharacterDungeonAward : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgCharacterDungeonAward();
	virtual ~CDlgCharacterDungeonAward();
	
	void OnCommandLast(const char * szCommand);
	void OnCommandNext(const char * szCommand);
	void OnCommandHead(const char * szCommand);
	void OnCommandTail(const char * szCommand);
	void OnCommandAward(const char * szCommand);
	void OnCommandAwardType(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	void UpdateAward();

private:
	int m_iCurPage;
	static const int ms_iNumPerPage;
	static const int ms_iNumTotal;

	AUIListBox*				m_pLstAward;
	AUIStillImageButton*	m_pBtnGet;
};
