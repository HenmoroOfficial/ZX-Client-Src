// Filename	: DlgReplay.cpp
// Creator	: Xiao Zhou
// Date		: 2006/10/12

#include "DlgReplay.h"
#include "DlgReplayEdit.h"
#include "EC_GameUIMan.h"
#include "EC_GameRecord.h"
#include "EC_Game.h"

#include <AM.h>

#define new A_DEBUG_NEW

extern game_record gr;

AUI_BEGIN_COMMAND_MAP(CDlgReplay, CDlgBase)

AUI_ON_COMMAND("speed*",		OnCommandSpeed)
AUI_ON_COMMAND("play",			OnCommandPlay)
AUI_ON_COMMAND("pause",			OnCommandPause)
AUI_ON_COMMAND("start",			OnCommandStart)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgReplay, CDlgBase)

AUI_ON_EVENT("Slider_Time",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_Camera",		WM_LBUTTONDBLCLK,	OnEventLButtonDBCLKCamera)

AUI_END_EVENT_MAP()

CDlgReplay::CDlgReplay()
{
}

CDlgReplay::~CDlgReplay()
{
	g_pGame->GetA3DEngine()->GetAMSoundEngine()->SetSFXPlaySpeed(1.0f);
	g_pGame->GetA3DEngine()->GetAMSoundEngine()->GetAMSoundBufferMan()->PauseSounds(false, AMSoundBufferMan::SND_ALL);
}

void CDlgReplay::OnCommandStart(const char * szCommand)
{
	gr.set_paused(false);
	gr.set_seek_time(0);
}

void CDlgReplay::OnCommandSpeed(const char * szCommand)
{
	float speed = float(atof(szCommand + strlen("speed")));
	gr.set_play_speed(speed);
	g_pGame->GetA3DEngine()->GetAMSoundEngine()->SetSFXPlaySpeed(speed);
}

void CDlgReplay::OnCommandPlay(const char * szCommand)
{
	gr.set_paused(false);
	g_pGame->GetA3DEngine()->GetAMSoundEngine()->GetAMSoundBufferMan()->PauseSounds(false, AMSoundBufferMan::SND_ALL);
	CheckRadioButton(2, 1);
}

void CDlgReplay::OnCommandPause(const char * szCommand)
{
	gr.set_paused(true);
	g_pGame->GetA3DEngine()->GetAMSoundEngine()->GetAMSoundBufferMan()->PauseSounds(true, AMSoundBufferMan::SND_ALL);
	CheckRadioButton(2, 2);
}

void CDlgReplay::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( m_pObjCapture == m_pSlider_Time )
		m_bDrag = true;
	else
	{
		m_pSlider_Time->SetDragging(true);
		m_pSlider_Time->OnDlgItemMessage(WM_MOUSEMOVE, wParam, lParam);
		m_bDrag = true;
//		int nNextLevel;
//		if( m_nLevel < m_pSlider_Time->GetLevel() )
//			nNextLevel = (m_nLevel / 60 + 1) * 60;
//		else
//			nNextLevel = (m_nLevel / 60 - 1) * 60;
//		a_Clamp(nNextLevel, 0, int(gr.get_total_time() / 1000));
//		gr.set_seek_time(nNextLevel * 1000);
	}
}

void CDlgReplay::OnEventLButtonDBCLKCamera(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !gr.is_edited_record() )
	{
		GetGameUIMan()->m_pDlgReplayEdit->InitEdit();
		OnCommandPause("");
		GetGameUIMan()->m_pDlgReplayEdit->Show(true);
	}
}

bool CDlgReplay::OnInitDialog()
{
	m_pSlider_Time = (PAUISLIDER)GetDlgItem("Slider_Time");
	m_pTxt_Time = (PAUILABEL)GetDlgItem("Txt_Time");

	return true;
}

void CDlgReplay::OnTick()
{
	m_pSlider_Time->Enable(gr.get_total_time() > 0);
	if( m_bDrag )
	{
		if( m_pObjCapture != m_pSlider_Time )
		{
			gr.set_seek_time(m_pSlider_Time->GetLevel() * 1000);
			m_bDrag = false;
			return;
		}
	}

	ACHAR szText[100];
	int totalTime = gr.get_total_time() / 1000;
	int curTime;
	if( m_bDrag )
		curTime = m_pSlider_Time->GetLevel();
	else
		curTime = gr.get_replay_time() / 1000;
	a_sprintf(szText, _AL("%02d:%02d:%02d/%02d:%02d:%02d"),
		curTime / 3600, curTime / 60 % 60, curTime % 60,
		totalTime / 3600, totalTime / 60 % 60, totalTime % 60);
	m_pTxt_Time->SetText(szText);
	m_nLevel = int(gr.get_replay_time() / 1000);
	if( !m_bDrag )
		m_pSlider_Time->SetLevel(m_nLevel);
	GetDlgItem("Rdo_Play")->Enable(!GetGameUIMan()->m_pDlgReplayEdit->IsShow());
	GetDlgItem("Rdo_Pause")->Enable(!GetGameUIMan()->m_pDlgReplayEdit->IsShow());
}

void CDlgReplay::OnShowDialog()
{
	GetDlgItem("Txt_RecordName")->SetAlign(AUIFRAME_ALIGN_CENTER);
	GetDlgItem("Txt_RecordName")->SetText(gr.get_title());
	m_pSlider_Time->SetTotal(gr.get_total_time() / 1000);
	if( gr.get_play_speed() == 0.25f )
		CheckRadioButton(1, 5);
	else if( gr.get_play_speed() == 0.5f )
		CheckRadioButton(1, 6);
	else
		CheckRadioButton(1, int(gr.get_play_speed()));
	CheckRadioButton(2, int(gr.is_paused()) + 1);
	m_bDrag = false;
	if( CDlgReplayEdit::m_bPreview && gr.is_seeking() )
	{
		GetGameUIMan()->m_pDlgReplayEdit->Show(true);
	}
}
