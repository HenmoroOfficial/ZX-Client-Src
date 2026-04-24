// Filename	: DlgGameCasual.cpp
// Creator	: Fu Chonggang
// Date		: 2009/03/04

#include "AFI.h"
#include "DlgGameCasual.h"
#include "EC_GameUIMan.h"
#include "EC_GameRun.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgGameCasual, CDlgBase)

AUI_ON_COMMAND("tdgame",	OnCommand_TdGame)
AUI_ON_COMMAND("llk",		OnCommand_LlkGame)
AUI_ON_COMMAND("ddz",		OnCommand_DdzGame)
AUI_ON_COMMAND("wquest",		OnCommand_TouchGame)

AUI_END_COMMAND_MAP()

CDlgGameCasual::CDlgGameCasual()
{
}

CDlgGameCasual::~CDlgGameCasual()
{
}

void CDlgGameCasual::OnCommand_TdGame(const char * szCommand)
{
	Show(false);
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_TowerDefense");
	if (GetGameUIMan()->GetDialog("Win_TestTowergame")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_TestTowergamemin")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_TowerDefenseFail")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_TowerDefenseWin")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_TGllkGame")->IsShow() )
	{
	}
	else
		pDlg->Show(true);
}

void CDlgGameCasual::OnCommand_LlkGame(const char * szCommand)
{
	Show(false);
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_TGllkGame");
	if (GetGameUIMan()->GetDialog("Win_TestTowergame")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_TestTowergamemin")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_TowerDefenseFail")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_TowerDefenseWin")->IsShow() ||
		GetGameUIMan()->GetDialog("Win_TowerDefense")->IsShow() )
	{
	}
	else
		pDlg->Show(true);
}

void CDlgGameCasual::OnTick()
{
}

void CDlgGameCasual::OnCommand_DdzGame(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_GamePoker_Hall");
	pDlg->Show(!pDlg->IsShow());
}
void CDlgGameCasual::OnCommand_TouchGame(const char* szCommand)
{
   if(GetHostPlayer()->GetSrcServerID() != 0)
   {
	   GetGameUIMan()->MessageBox("", GetStringFromTable(17900), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	   return;
   }
   PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Game_Touch");
   if(pDlg->IsShow())
   {
     pDlg->Show(false);
	 return;
   }
   GetGameSession()->touch_getpassportcashadd(GetHostPlayer()->GetCharacterID());
   GetGameSession()->touch_getpoint(GetHostPlayer()->GetCharacterID());
}