// Filename	: DlgBattleDungeonTime.h
// Creator	: Fu Chonggang
// Date		: 2012.9.12

#pragma once

#include "DlgBase.h"

class AUILabel;

class CDlgBattleDungeonTime : public CDlgBase  
{
public:
	CDlgBattleDungeonTime();
	virtual ~CDlgBattleDungeonTime();

protected:
	virtual bool OnInitDialog();
	virtual void OnTick();

	AUILabel *m_pLabTime;
};
