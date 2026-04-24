// Filename	: DlgRecord.cpp
// Creator	: Xiao Zhou
// Date		: 2006/10/23

#include "DlgRecord.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_GameRecord.h"
#include "EC_HostPlayer.h"
#include "AUI\\AUICheckBox.h"

#define new A_DEBUG_NEW

extern game_record gr;

AUI_BEGIN_COMMAND_MAP(CDlgRecord, CDlgBase)

AUI_ON_COMMAND("start",			OnCommandStart)
AUI_ON_COMMAND("end",			OnCommandEnd)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

CDlgRecord::CDlgRecord()
{
}

CDlgRecord::~CDlgRecord()
{
}

void CDlgRecord::OnCommandStart(const char * szCommand)
{
	for(int i = 0; ;i++)
	{
		char szName[100];
		sprintf(szName, "Chk_%d", i + 1);
		if( !GetDlgItem(szName) )
			break;

		gr.set_replay_setting(i, !((PAUICHECKBOX)GetDlgItem(szName))->IsChecked());
	}
	gr.set_title(GetDlgItem("Txt_RecordName")->GetText());
	gr.apply_cur_setting();
	gr.set_start_record_flag();
	Show(false);
	GetGameUIMan()->m_pDlgRecordEnd->Show(true);
	GetGameUIMan()->m_pDlgRecordEnd->AlignTo((PAUIDIALOG)GetGameUIMan()->m_pDlgHost, 
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
	GetGameUIMan()->m_pDlgRecordEnd->SetCanMove(false);
}

void CDlgRecord::OnCommandEnd(const char * szCommand)
{
	GetGameUIMan()->MessageBox("Game_StopRecord", GetStringFromTable(823),
		MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
}

void CDlgRecord::OnCommandCancel(const char * szCommand)
{
	if( m_szName == "Win_RecordEnd" )
	{
		ALISTPOSITION pos;
		pos = GetGameUIMan()->m_DlgZOrder.Find(this);
		if( !pos ) return;

		GetGameUIMan()->m_DlgZOrder.RemoveAt(pos);
		GetGameUIMan()->m_DlgZOrder.AddTail(this);

		if( GetGameUIMan()->m_DlgZOrder.GetCount() > 0 )
		{
			PAUIDIALOG pDlg = GetGameUIMan()->m_DlgZOrder.GetHead();
			if( pDlg )
				GetGameUIMan()->BringWindowToTop(pDlg);
		}
	}
	else
		Show(false);
}

void CDlgRecord::EndRecord()
{
	gr.set_end_record_flag();
	Show(false);
}

void CDlgRecord::OnTick()
{
	if( m_szName == "Win_RecordEnd" )
	{
		ACHAR szText[100];
		int totalTime = gr.get_total_time() / 1000;
		a_sprintf(szText, _AL("%02d:%02d:%02d"),
			totalTime / 3600, totalTime / 60 % 60, totalTime % 60);
		GetDlgItem("Txt_Time")->SetText(szText);
	}
}

void CDlgRecord::OnShowDialog()
{
	if( m_szName == "Win_Record" )
	{
		if( !IsShow() )
		{
			ACString strText;
			strText.Format(GetStringFromTable(830), GetHostPlayer()->GetName());
			GetDlgItem("Txt_RecordName")->SetText(strText);
			gr.set_replay_setting(REPLAY_PRIVATE_CHAT, false);
			gr.set_replay_setting(REPLAY_WORLD_CHAT, true);
			gr.set_replay_setting(REPLAY_COMMON_CHAT, true);
			gr.set_replay_setting(REPLAY_INVENTORY, false);
			gr.set_replay_setting(REPLAY_SKILL, false);
			gr.set_replay_setting(REPLAY_SELF_INFO, false);
			gr.apply_cur_setting();
			for(int i = 0; ;i++)
			{
				char szName[100];
				sprintf(szName, "Chk_%d", i + 1);
				if( !GetDlgItem(szName) )
					break;

				((PAUICHECKBOX)GetDlgItem(szName))->Check(!gr.get_replay_setting(i));
			}
		}
	}
}