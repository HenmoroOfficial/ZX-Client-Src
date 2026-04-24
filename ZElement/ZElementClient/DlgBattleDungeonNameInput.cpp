#include "AUI\\AUIEditbox.h"
#include "AUI\\AUICheckBox.h"
#include "DlgBattleDungeonNameInput.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"
#include "EC_BattleDungeon.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattleDungeonNameInput, CDlgBase)

AUI_ON_COMMAND("Btn_Confim",		OnCommandConfirm)

AUI_END_COMMAND_MAP()

CDlgBattleDungeonNameInput::CDlgBattleDungeonNameInput()
{
}

CDlgBattleDungeonNameInput::~CDlgBattleDungeonNameInput()
{
}

void CDlgBattleDungeonNameInput::OnShowDialog()
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Edt_Input");
	ACString strName = pEdt->GetText();
	if (strName.IsEmpty())
	{
		pEdt->SetText(ACString(GetHostPlayer()->GetName())+ACString(GetGameUIMan()->GetStringFromTable(12260)));
	}
	if (GetDlgItem("Chk_Leader"))
		GetDlgItem("Chk_Leader")->Show(CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iId!=534);	// 梦回河阳城不需要管理功能
}

void CDlgBattleDungeonNameInput::OnCommandConfirm(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Edt_Input");
	ACString strName = pEdt->GetText();
	if (strName.IsEmpty())
	{
		strName = ACString(GetHostPlayer()->GetName())+ACString(GetGameUIMan()->GetStringFromTable(12260));
	}
	// 开启副本

	char cLevel = 0;
	if (m_szName=="Win_Battle_Dungeon_SiX_Room")
	{
		cLevel = GetCheckedRadioButton(1);
	}

	PAUICHECKBOX pChkVote = (PAUICHECKBOX)GetDlgItem("Chk_Leader");
	GetGameSession()->c2s_CmdNPCSevOpenRaidRoom(CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iMapId, 
		CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iId, (pChkVote&&pChkVote->IsChecked()) ? 1:0, strName, cLevel);
	CECBattleDungeonMan::getSingletonPtr()->JoinBattle(-1);
	Show(false);
}