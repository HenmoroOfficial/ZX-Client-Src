// Filename	: DlgProclaim.cpp
// Creator	: Tom Zhou
// Date		: September 8, 2005

#include "DlgProclaim.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgProclaim, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommand_confirm)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

CDlgProclaim::CDlgProclaim()
{
}

CDlgProclaim::~CDlgProclaim()
{
}

bool CDlgProclaim::OnInitDialog()
{
	m_pDEFAULT_Txt_Input = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Input");

	return true;
}

void CDlgProclaim::OnCommand_confirm(const char * szCommand)
{
	ACString strText = m_pDEFAULT_Txt_Input->GetText();
	if(GetData() != 2)
	{
		GetGameSession()->faction_change_proclaim(strText, GetData());
	}
	else
	{
		c2s_SendCmdPostFacBaseMsg(GetHostPlayer()->GetFactionID(),strText);
	}
	m_pDEFAULT_Txt_Input->SetText(_AL(""));
	Show(false);
}

void CDlgProclaim::OnCommand_CANCEL(const char * szCommand)
{
	m_pDEFAULT_Txt_Input->SetText(_AL(""));
	Show(false);
}
