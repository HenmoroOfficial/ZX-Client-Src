// Filename	: DlgFlyProgress.cpp
// Creator	: Fu Chonggang
// Date		: 2012-11-27

#include "DlgFlyProgress.h"
#include "AFI.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIProgress.h"
#include "EC_GameUIMan.h"
#include "getcommondata_re.hpp"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgFlyProgress, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

const int CDlgFlyProgress::ms_ValueMax = 100000;
const int CDlgFlyProgress::ms_ValuePerHour = 130;

CDlgFlyProgress::CDlgFlyProgress()
{
}

CDlgFlyProgress::~CDlgFlyProgress()
{
}

void CDlgFlyProgress::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->EndNPCService();
}

int CDlgFlyProgress::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	GNET::GetCommonData_Re *p = (GNET::GetCommonData_Re*)wParam;
	if (p->retcode!=0)
	{
		return 1;
	}
	GetDlgItem("Txt_Hint")->Show(p->value>=ms_ValueMax);
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Day");
	pLab->Show(p->value<ms_ValueMax);
	int iHours = (ms_ValueMax-p->value)/ms_ValuePerHour;
	ACString strText;
	if (iHours%24==0)
		strText.Format(GetGameUIMan()->GetStringFromTable(1604), iHours/24);
	else
		strText.Format(GetGameUIMan()->GetStringFromTable(1610), iHours/24, iHours%24);
	pLab->SetText(strText);
	PAUIPROGRESS pPrg = (PAUIPROGRESS)GetDlgItem("Prgs_Day");
	pPrg->SetProgress(AUIPROGRESS_MAX*p->value/ms_ValueMax);
	if (p->value>ms_ValueMax)
		strText = _AL("100%");
	else
		strText.Format(_AL("%.1f%%"), 100.0f*p->value/ms_ValueMax);
	pLab = (PAUILABEL)GetDlgItem("Txt_PrgsDay");
	pLab->SetText(strText);

	return 0;
}