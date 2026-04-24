// Filename	:	DlgKfSelectLine.cpp
// Creator	:	Fu Chonggang
// Date		:	Sep 30, 2010

#include "DlgKfSelectLine.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_Secretary.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgKfSelectLine, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_ON_COMMAND("sortname",				OnCommand_sortname)
AUI_ON_COMMAND("sortstats",				OnCommand_sortstats)
AUI_ON_COMMAND("selectline",			OnCommand_selectline)
AUI_ON_COMMAND("Btn_Return",			OnCommand_return)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgKfSelectLine, CDlgBase)


AUI_END_EVENT_MAP()

CDlgKfSelectLine::CDlgKfSelectLine()
{
}

CDlgKfSelectLine::~CDlgKfSelectLine()
{
}

bool CDlgKfSelectLine::Release()
{
	return CDlgBase::Release();
}

bool CDlgKfSelectLine::OnInitDialog()
{
	m_pLst_Line = (AUIListBox *)GetDlgItem("Lst_Line");
	m_pBtn_SortName1 = (AUIStillImageButton *)GetDlgItem("Btn_SortName1");
	m_pBtn_SortStats1 = (AUIStillImageButton *)GetDlgItem("Btn_SortStats1");
	m_pBtn_Confirm = (AUIStillImageButton *)GetDlgItem("Btn_Confirm");
	m_pBtn_Cancel = (AUIStillImageButton *)GetDlgItem("Btn_Cancel");
	m_pTitle = (AUILabel *)GetDlgItem("Title");
	return true;
}

void CDlgKfSelectLine::OnShowDialog()
{
	CECGameRun::LineInfoVec lineinfo = GetGame()->GetGameRun()->GetLinesInfo();
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("Lst_Line");
	pList->ResetContent();
	int i;
	for(i = 0; i < (int)lineinfo.size(); i++ )
	{
		ACString strLoad;
		A3DCOLOR color;
		if( lineinfo[i].attribute < 50 )
		{
			strLoad = GetStringFromTable(6400);
			color = 0xFF00FF7F;
		}
		else if(  lineinfo[i].attribute < 90 )
		{
			strLoad = GetStringFromTable(6401);
			color = 0xFFFFFF7F;
		}
		else
		{
			strLoad = GetStringFromTable(6402);
			color = 0xFFFF0000;
		}
		
		pList->InsertString(i, lineinfo[i].name + _AL("\t") + strLoad);
		pList->SetItemTextColor(i, color, 1);
		pList->SetItemData(i, lineinfo[i].id);
	}
	for (i=0; i<pList->GetCount(); i++)
	{
		if ((int)pList->GetItemData(i)==GetGame()->GetGameRun()->GetCurLine())
		{
			pList->SetCurSel(i);
			break;
		}
	}
}

void CDlgKfSelectLine::OnTick()
{
}

bool CDlgKfSelectLine::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

int CDlgKfSelectLine::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	return 1;
}

void CDlgKfSelectLine::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgKfSelectLine::OnCommand_sortname(const char * szCommand)
{
}

void CDlgKfSelectLine::OnCommand_sortstats(const char * szCommand)
{
}

void CDlgKfSelectLine::OnCommand_selectline(const char * szCommand)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("Lst_Line");
	int nSel = pList->GetCurSel();
	if (nSel>=0 && nSel<pList->GetCount())
	{
		if (GetGame()->GetGameRun()->GetCurLine()==(int)pList->GetItemData(nSel))
			GetGameUIMan()->MessageBox(6000);
		else
		{
			GetGameRun()->SaveConfigsToServer();
			GetGameSession()->try_change_gs(pList->GetItemData(nSel));
			//GetGameUIMan()->ChangeGs(pList->GetItemData(nSel));
			Show(false);
		}
	}
}

void CDlgKfSelectLine::OnCommand_return(const char * szCommand)
{
//	if(GetHostPlayer()->GetBattleType() == BATTLE_TYPE_FLPW_CRSSVR )
	{
		GetGameSession()->c2s_CmdLogout(_PLAYER_LOGOUT_FULL);
	}
	GetGameRun()->SetLogoutFlag(2);
}

