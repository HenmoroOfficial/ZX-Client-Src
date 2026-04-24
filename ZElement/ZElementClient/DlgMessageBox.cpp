// Filename	: DlgMessageBox.cpp
// Creator	: Fu Chonggang
// Date		: 2012.11.26

#include "AFI.h"
#include "AUI\\AUILabel.h"
#include "DlgMessageBox.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_GameSession.h"

#include "teamraidmappingsuccess_re.hpp"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgMessageBoxYesTime, CDlgBase)

AUI_ON_COMMAND("Btn_Decide",		OnCommandConfirm)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()


CDlgMessageBoxYesTime::CDlgMessageBoxYesTime()
{
}

CDlgMessageBoxYesTime::~CDlgMessageBoxYesTime()
{
}

bool CDlgMessageBoxYesTime::OnInitDialog()
{
	m_pLabTime = (PAUILABEL)GetDlgItem("Txt_TimeLeft");
	return true;
}

void CDlgMessageBoxYesTime::OnShowDialog()
{
	m_dwTime = GetTickCount();
}

void CDlgMessageBoxYesTime::OnTick()
{
	if ((GetTickCount()-m_dwTime) > m_dwTotalTime*1000)
	{
		OnCommandConfirm("");
	}
	ACString strText;
	strText.Format(_AL("%d"), m_dwTotalTime-((GetTickCount()-m_dwTime)/1000)); 
	m_pLabTime->SetText(strText);
}

void CDlgMessageBoxYesTime::OnCommandConfirm(const char * szCommand)
{
	TeamRaidMappingSuccess_Re p;
	p.roleid = GetHostPlayer()->GetCharacterID();
	p.raidroom_id = GetData();
	GetGameSession()->SendNetData(p);
	Show(false);
}

void CDlgMessageBoxYesTime::OnCommandCancel(const char * szCommand)
{
}

int CDlgMessageBoxYesTime::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
//	PAUILABEL pLabMsg = (PAUILABEL)GetDlgItem("Txt_1");
//	pLabMsg->SetText(AS2AC(strMsg));
	m_dwTotalTime = wParam;
//	m_iType	= lParam;
	Show(true, true);
	return 0;
}