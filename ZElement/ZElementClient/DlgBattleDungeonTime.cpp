// Filename	: DlgBattleDungeonTime.cpp
// Creator	: Fu Chonggang
// Date		: 2012.9.12

#include "DlgBattleDungeonTime.h"
#include "AUI\\AUILabel.h"
#include "EC_GameUIMan.h"
#include "EC_BattleDungeon.h"
#include "EC_Game.h"
#include "EC_GameUIMisc.h"

#define new A_DEBUG_NEW

CDlgBattleDungeonTime::CDlgBattleDungeonTime()
{
}

CDlgBattleDungeonTime::~CDlgBattleDungeonTime()
{
}

bool CDlgBattleDungeonTime::OnInitDialog()
{
	m_pLabTime = (PAUILABEL)GetDlgItem("Txt_TimeLeft");
	
	return true;
}

void CDlgBattleDungeonTime::OnTick()
{
	ACString strTime = GetTimeText(CECBattleDungeonMan::getSingletonPtr()->m_iEndTime-GetGame()->GetServerGMTTime());
	m_pLabTime->SetText(strTime);
}