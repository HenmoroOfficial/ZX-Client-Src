// Filename	: DlgReplayEdit.cpp
// Creator	: Xiao Zhou
// Date		: 2006/10/12

#include "DlgReplayEdit.h"
#include "DlgReplay.h"
#include "DlgInputName.h"
#include "EC_GameUIMan.h"
#include "EC_GameRecord.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "AUI\\AUIDef.h"
#include "A2DSprite.h"
#include "A3DGDI.h"
#include "EC_Resource.h"

abase::vector<VECTOR_SECTION> CDlgReplayEdit::m_vecSteps;
int CDlgReplayEdit::m_nCurStep;
int CDlgReplayEdit::m_nSelStart;
int CDlgReplayEdit::m_nSelEnd;
int CDlgReplayEdit::m_nClick;
bool CDlgReplayEdit::m_bPreview = false;
int CDlgReplayEdit::m_nPreviewSection;
int CDlgReplayEdit::m_nPreviewSecTime;
float CDlgReplayEdit::m_fScale;

#define new A_DEBUG_NEW

extern game_record gr;
extern bool save_edited_record(const wchar_t* title);

AUI_BEGIN_COMMAND_MAP(CDlgReplayEdit, CDlgBase)

AUI_ON_COMMAND("export",		OnCommandExport)
AUI_ON_COMMAND("preview",		OnCommandPreview)
AUI_ON_COMMAND("undo",			OnCommandUndo)
AUI_ON_COMMAND("redo",			OnCommandRedo)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgReplayEdit, CDlgBase)

AUI_ON_EVENT("Img_Record",		WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Img_Record",		WM_MOUSEMOVE,	OnEventMouseMove)
AUI_ON_EVENT("Img_Record",		WM_LBUTTONUP,	OnEventLButtonUp)
AUI_ON_EVENT("*",				WM_MOUSEWHEEL,	OnEventMouseWheel)
AUI_ON_EVENT(NULL,				WM_MOUSEWHEEL,	OnEventMouseWheel)
AUI_ON_EVENT("*",				WM_KEYDOWN,		OnEventKeyDown)
AUI_ON_EVENT(NULL,				WM_KEYDOWN,		OnEventKeyDown)

AUI_END_EVENT_MAP()

CDlgReplayEdit::CDlgReplayEdit()
{
}

CDlgReplayEdit::~CDlgReplayEdit()
{
	m_Frame.Release();
}

void CDlgReplayEdit::Export(ACString strRecordName)
{
	record_edit_info_vec vec;
	int i;
	for(i = 0; i < (int)m_vecSteps[m_nCurStep].size(); i++ )
	{
		record_edit_info info;
		info.start_second = m_vecSteps[m_nCurStep][i].start;
		info.end_second = m_vecSteps[m_nCurStep][i].end;
		vec.push_back(info);
	}

	gr.set_edit_info(vec);
	save_edited_record(ACString(gr.get_title()) + strRecordName);
}

void CDlgReplayEdit::OnCommandExport(const char * szCommand)
{
	CDlgInputName* pDlg = GetGameUIMan()->m_pDlgInputName;
	pDlg->m_pTxtInput->SetText(_AL(""));
	pDlg->SetData(CDlgInputName::INPUTNAME_EDIT_RECORD);
	pDlg->Show(true);
}

void CDlgReplayEdit::OnCommandPreview(const char * szCommand)
{
	m_bPreview = !m_bPreview;
	if( m_bPreview )
		gr.set_seek_time((m_vecSteps[m_nCurStep][m_nPreviewSection].start + m_nPreviewSecTime) * 1000);
	else
		gr.set_paused(true);
}

void CDlgReplayEdit::OnCommandUndo(const char * szCommand)
{
	if( m_nCurStep > 0 )
	{
		m_nCurStep--;
		m_nSelStart = m_nSelEnd = -1;
	}
}

void CDlgReplayEdit::OnCommandRedo(const char * szCommand)
{
	if( m_nCurStep < (int)m_vecSteps.size() - 1)
	{
		m_nCurStep++;
		m_nSelStart = m_nSelEnd = -1;
	}
}

void CDlgReplayEdit::OnEventKeyDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( wParam == VK_DELETE && m_nSelStart != -1 )
	{
		if( !gr.is_paused() )
		{
			gr.set_paused(true);
			m_bPreview = false;
		}
		VECTOR_SECTION newvecsec;
		int i;
		for(i = 0;i < (int)m_vecSteps[m_nCurStep].size(); i++)
			if( i < m_nSelStart || i > m_nSelEnd )
				newvecsec.push_back(m_vecSteps[m_nCurStep][i]);
		m_nCurStep++;
		m_vecSteps.push_back(newvecsec);
		m_nSelStart = m_nSelEnd = -1;
	}
}

void CDlgReplayEdit::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( gr.get_total_time() < 600000 )
	{
		m_pScl_Record->Show(false);
		m_fScale = 1.0f;
		return;
	}

	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	int zDelta = (short)HIWORD(wParam);
	if( zDelta > 0 )
		m_fScale *= 1.25f;
	else if( zDelta < 0 )
		m_fScale *= 0.8f;
	if( m_fScale > 1.0f )
		m_fScale = 1.0f;
	else if( gr.get_total_time() * m_fScale < 600000 )
		m_fScale = 600000.0f / gr.get_total_time();
	
	m_pScl_Record->Show(m_fScale != 1.0f);
	m_pScl_Record->SetScrollRange(0 , int(gr.get_total_time() * (1 - m_fScale) / 1000));
	m_pScl_Record->SetBarLength(int(gr.get_total_time() * m_fScale / 1000));
}

void CDlgReplayEdit::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !gr.is_paused() )
	{
		if( !m_pRdo[2]->IsChecked() )
		{
			gr.set_paused(true);
			m_bPreview = false;
		}
	}
	m_bDrag = false;
	SetCapture(false);
	SetCaptureObject(m_pImg_Record);
	int x = GET_X_LPARAM(lParam);
	int sec, pos;
	int i;
	m_nClick = -1;
	if( !GetRecordPos(x, sec, pos) )
	{
		m_nSelStart = m_nSelEnd = -1;
		return;
	}
	else if( AUI_PRESS(VK_CONTROL) || m_pRdo[1]->IsChecked() )
	{
		GetRecordPos(x, sec, pos, true);
		if( pos > 0 && pos < m_vecSteps[m_nCurStep][sec].end - m_vecSteps[m_nCurStep][sec].start )
		{
			VECTOR_SECTION newvecsec;
			for(i = 0;i < sec; i++)
				newvecsec.push_back(m_vecSteps[m_nCurStep][i]);
			SECTION newsec;
			newsec.start = m_vecSteps[m_nCurStep][sec].start;
			newsec.end = m_vecSteps[m_nCurStep][sec].start + pos;
			newvecsec.push_back(newsec);
			newsec.start = m_vecSteps[m_nCurStep][sec].start + pos;
			newsec.end = m_vecSteps[m_nCurStep][sec].end;
			newvecsec.push_back(newsec);
			for(i = sec + 1;i < (int)m_vecSteps[m_nCurStep].size(); i++)
				newvecsec.push_back(m_vecSteps[m_nCurStep][i]);
			m_nCurStep++;
			while( (int)m_vecSteps.size() > m_nCurStep )
				m_vecSteps.pop_back();
			m_vecSteps.push_back(newvecsec);
		}
	}
	else if( m_pRdo[0]->IsChecked() && AUI_PRESS(VK_SHIFT) && m_nSelStart != -1 )
	{
		m_nSelEnd = sec;
	}
	else if( AUI_PRESS(VK_MENU) || m_pRdo[2]->IsChecked() )
	{
		m_nPreviewSection = sec;
		m_nPreviewSecTime = pos;
		if( !gr.is_paused() )
			gr.set_seek_time((m_vecSteps[m_nCurStep][m_nPreviewSection].start + m_nPreviewSecTime) * 1000);
	}
	else
	{
		m_nClick = sec;
		if( m_nSelStart == m_nSelEnd )
		{
			if( m_nSelStart == sec )
				m_nClick = -1;
			m_nSelStart = m_nSelEnd = sec;
		}
	}
	m_nDragStartX = x;
}

void CDlgReplayEdit::OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int x = GET_X_LPARAM(lParam);
	int sec, pos;
	if( GetRecordPos(x, sec, pos) )
	{
		ACHAR szText[100];
		if( m_pRdo[0]->IsChecked() )
		{
			int t1 = m_vecSteps[m_nCurStep][sec].start;
			int t2 = m_vecSteps[m_nCurStep][sec].end;
			a_sprintf(szText, _AL("%02d:%02d:%02d--%02d:%02d:%02d"), 
				t1 / 3600, t1  / 60 % 60, t1 % 60, t2 / 3600, t2  / 60 % 60, t2 % 60); 
		}
		else
		{
			int t1 = m_vecSteps[m_nCurStep][sec].start + pos;
			a_sprintf(szText, _AL("%02d:%02d:%02d"), t1 / 3600, t1  / 60 % 60, t1 % 60); 
		}
		m_pImg_Record->SetHint(szText);
	}

	if( !gr.is_paused() )
		return;

	if( AUI_PRESS(VK_LBUTTON) && GetCaptureObject() == m_pImg_Record )
	{
		int x = GET_X_LPARAM(lParam);
		m_nDragCurX = x;
		if( m_nSelStart != -1 && abs(m_nDragCurX - m_nDragStartX) >= 3)
			m_bDrag = true;
	}
}

void CDlgReplayEdit::OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !gr.is_paused() )
		return;
	
	int x = GET_X_LPARAM(lParam);
	int sec, pos;
	if( m_bDrag )
	{
		A3DRECT rc;
		rc.left = m_pImg_Record->GetPos().x;
		rc.top = m_pImg_Record->GetPos().y;
		rc.right = rc.left + m_pImg_Record->GetSize().cx;
		rc.bottom = rc.top + m_pImg_Record->GetSize().cy;
		int W = rc.Width();
		if( m_pScl_Record->IsShow() )
		{
			rc.left -= W * m_pScl_Record->GetBarLevel() / m_pScl_Record->GetBarLength();
			rc.right += W * (m_pScl_Record->GetScrollEnd() - m_pScl_Record->GetBarLevel()) / m_pScl_Record->GetBarLength();
		}
		int min = (gr.get_total_time() - 1) / 1000 + 1;
		if( x < m_nDragStartX )
		{
			pos = int(float(min) * (m_nDragStartX - x) / rc.Width());
			for(sec = m_nSelStart - 1; sec >= 0; sec--)
			{
				if( pos < m_vecSteps[m_nCurStep][sec].end - m_vecSteps[m_nCurStep][sec].start )
					break;
				else
					pos -= m_vecSteps[m_nCurStep][sec].end - m_vecSteps[m_nCurStep][sec].start;
			}
			sec++;
		}
		else
		{
			pos = int(float(min) * (x - m_nDragStartX) / rc.Width());
			for(sec = m_nSelEnd + 1; sec < (int)m_vecSteps[m_nCurStep].size(); sec++)
			{
				if( pos < m_vecSteps[m_nCurStep][sec].end - m_vecSteps[m_nCurStep][sec].start )
					break;
				else
					pos -= m_vecSteps[m_nCurStep][sec].end - m_vecSteps[m_nCurStep][sec].start;
			}
			sec--;
		}

		if( sec < m_nSelStart || sec > m_nSelEnd )
		{
			if( sec < 0 )
				sec = 0;
			int left, right;
			if( m_nSelStart < m_nSelEnd )
			{
				left = m_nSelStart;
				right = m_nSelEnd;
			}
			else
			{
				left = m_nSelEnd;
				right = m_nSelStart;
			}
			VECTOR_SECTION newvecsec;
			int n = right - left;
			int i;
			if( sec < left )
			{
				for(i = 0;i < sec; i++)
					newvecsec.push_back(m_vecSteps[m_nCurStep][i]);
				for(i = left;i <= right; i++)
					newvecsec.push_back(m_vecSteps[m_nCurStep][i]);
				for(i = sec;i < left; i++)
					newvecsec.push_back(m_vecSteps[m_nCurStep][i]);
				for(i = right + 1;i < (int)m_vecSteps[m_nCurStep].size(); i++)
					newvecsec.push_back(m_vecSteps[m_nCurStep][i]);
				m_nSelStart = sec;
				m_nSelEnd = sec + n;
				m_nCurStep++;
				while( (int)m_vecSteps.size() > m_nCurStep )
					m_vecSteps.pop_back();
				m_vecSteps.push_back(newvecsec);
			}
			else if( sec > right )
			{
				for(i = 0;i < left; i++)
					newvecsec.push_back(m_vecSteps[m_nCurStep][i]);
				for(i = right + 1;i <= sec; i++)
					newvecsec.push_back(m_vecSteps[m_nCurStep][i]);
				for(i = left;i <= right; i++)
					newvecsec.push_back(m_vecSteps[m_nCurStep][i]);
				for(i = sec + 1;i < (int)m_vecSteps[m_nCurStep].size(); i++)
					newvecsec.push_back(m_vecSteps[m_nCurStep][i]);
				m_nSelEnd = sec;
				m_nSelStart = m_nSelEnd - n;
				m_nCurStep++;
				while( (int)m_vecSteps.size() > m_nCurStep )
					m_vecSteps.pop_back();
				m_vecSteps.push_back(newvecsec);
			}
		}
	}
	else
	{
		GetRecordPos(x, sec, pos);
		if( m_nClick != -1 )
			m_nSelStart = m_nSelEnd = sec;
		else if( m_nSelStart == sec && m_nSelEnd == sec )
				m_nSelStart = m_nSelEnd = -1;
	}
	m_bDrag = false;
	m_nDragStartX = -1;
	SetCaptureObject(NULL);
}

bool CDlgReplayEdit::OnInitDialog()
{
	CheckRadioButton(0, 2);
	m_pScl_Record = (PAUISCROLL)GetDlgItem("Scl_Record");
	if( !m_pScl_Record ) return false;
	m_pImg_Record = (PAUIIMAGEPICTURE)GetDlgItem("Img_Record");
	if( !m_pImg_Record ) return false;
	m_pImg_Record->SetAcceptMouseMessage(true);
	m_pBtn_Undo = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Undo");
	if( !m_pBtn_Undo ) return false;
	m_pBtn_Redo = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Redo");
	if( !m_pBtn_Redo ) return false;
	m_pBtn_Preview = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Preview");
	if( !m_pBtn_Preview ) return false;
	m_pBtn_StopPreview = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_StopPreview");
	if( !m_pBtn_StopPreview ) return false;
	char szName[20];
	for(int i = 0; i < 3; i++ )
	{
		sprintf(szName, "Rdo_%d", i);
		m_pRdo[i] = (PAUIRADIOBUTTON)GetDlgItem(szName);
		if( !m_pRdo[i] )
			return false;
	}
	if( !m_Frame.Init(g_pGame->GetA3DEngine(), g_pGame->GetA3DDevice(), "ingame\\trackframe.tga") )
		return false;
	
	m_pImg_Record->SetHintFollow(true);
	SetHintDelay(0);

	return true;
}

void CDlgReplayEdit::OnTick()
{
	m_pBtn_Preview->Enable(!m_bPreview);
	m_pBtn_StopPreview->Enable(m_bPreview);
	m_pBtn_Undo->Enable(!m_bPreview && m_nCurStep > 0);
	m_pBtn_Redo->Enable(!m_bPreview && m_nCurStep < (int)m_vecSteps.size() - 1);
// 	if( GetGameUIMan()->m_pObjMouseOn == m_pImg_Record )
// 	{
// 		if( m_pRdo[1]->IsChecked() )
// 			GetGame()->ChangeCursor(RES_CUR_REC_CUT);
// 		else if( m_pRdo[2]->IsChecked() )
// 			GetGame()->ChangeCursor(RES_CUR_REC_REVIEW);
// 		else
// 			GetGame()->ChangeCursor(RES_CUR_NORMAL);
// 	}
// 	else
// 		GetGame()->ChangeCursor(RES_CUR_NORMAL);
}

void CDlgReplayEdit::OnShowDialog()
{
	OnEventMouseWheel(0, 0, NULL);
}

bool CDlgReplayEdit::Render()
{
	if( !CDlgBase::Render() )
		return false;
	int i;
	A3DRECT rc;
	A3DPOINT2 p1, p2;
	rc.left = m_pImg_Record->GetPos().x;
	rc.top = m_pImg_Record->GetPos().y;
	rc.right = rc.left + m_pImg_Record->GetSize().cx;
	rc.bottom = rc.top + m_pImg_Record->GetSize().cy;
	int len = 0;
	int totaltime = (gr.get_total_time() - 1) / 1000 + 1;
	A3DViewport *pViewPort = m_pA3DEngine->GetActiveViewport();
	A3DViewport *pNewViewPort =new A3DViewport;
	A3DVIEWPORTPARAM parm = *pViewPort->GetParam();
	parm.X = rc.left;
	parm.Y = rc.top - 10;
	parm.Width = rc.Width();
	parm.Height = rc.Height() + 13;
	if( !pNewViewPort->Init(g_pGame->GetA3DDevice(), &parm, false, false, 0) )
		return true;
	pNewViewPort->SetParam(&parm);
	pNewViewPort->Active();

	int W = rc.Width();
	if( m_pScl_Record->IsShow() )
	{
		rc.left -= W * m_pScl_Record->GetBarLevel() / m_pScl_Record->GetBarLength();
		rc.right += W * (m_pScl_Record->GetScrollEnd() - m_pScl_Record->GetBarLevel()) / m_pScl_Record->GetBarLength();
	}
	for(i = 0; i < totaltime; i += 3600 )
	{
		A3DPOINT2 p1, p2;
		p1.Set(rc.left + rc.Width() * i / totaltime, rc.top - 2);
		p2.Set(rc.left + rc.Width() * i / totaltime, rc.top - 10);
		g_pA3DGDI->Draw2DLine(p1, p2, A3DCOLORRGB(255, 255, 255));
	}
	for(i = 0; i < totaltime; i += 600 )
	{
		A3DPOINT2 p1, p2;
		p1.Set(rc.left + rc.Width() * i / totaltime, rc.top - 2);
		p2.Set(rc.left + rc.Width() * i / totaltime, rc.top - 7);
		g_pA3DGDI->Draw2DLine(p1, p2, A3DCOLORRGB(255, 255, 255));
	}
	for(i = 0; i < totaltime; i += 60 )
	{
		p1.Set(rc.left + rc.Width() * i / totaltime, rc.top - 2);
		p2.Set(rc.left + rc.Width() * i / totaltime, rc.top - 4);
		g_pA3DGDI->Draw2DLine(p1, p2, A3DCOLORRGB(255, 255, 255));
	}
	
	int left, right;
	if( m_nSelStart < m_nSelEnd )
	{
		left = m_nSelStart;
		right = m_nSelEnd;
	}
	else
	{
		left = m_nSelEnd;
		right = m_nSelStart;
	}
	for(i = 0;i < (int)m_vecSteps[m_nCurStep].size(); i++)
	{
		if( i >= left && i <= right )
		{
			m_Frame.GetA2DSprite()->SetColor(A3DCOLORRGB(128, 128, 128));
			m_Frame.Render(rc.left + len * rc.Width() / totaltime, rc.top,
				(m_vecSteps[m_nCurStep][i].end - m_vecSteps[m_nCurStep][i].start) * rc.Width() / totaltime, rc.Height(),
				NULL, 0, NULL, 0, 0, 0, 255);
		}
		else
		{
			m_Frame.GetA2DSprite()->SetColor(A3DCOLORRGB(255, 255, 255));
			m_Frame.Render(rc.left + len * rc.Width() / totaltime, rc.top,
				(m_vecSteps[m_nCurStep][i].end - m_vecSteps[m_nCurStep][i].start) * rc.Width() / totaltime, rc.Height(),
				NULL, 0, NULL, 0, 0, 0, 255);
		}
		if( i == m_nPreviewSection )
		{
			p1.Set(rc.left + (len + m_nPreviewSecTime) * rc.Width() / totaltime, rc.top - 3);
			p2.Set(rc.left + (len + m_nPreviewSecTime) * rc.Width() / totaltime, rc.bottom + 3);
			g_pA3DGDI->Draw2DLine(p1, p2, A3DCOLORRGB(255, 0, 0));
		}
		len += m_vecSteps[m_nCurStep][i].end - m_vecSteps[m_nCurStep][i].start;
	}
	if( m_bDrag )
	{
		len = 0;
		for(i = 0;i < (int)m_vecSteps[m_nCurStep].size(); i++)
		{
			if( i >= left && i <= right )
			{
				m_Frame.GetA2DSprite()->SetColor(A3DCOLORRGB(255, 255, 255));
				m_Frame.Render(m_nDragCurX - m_nDragStartX + rc.left + len * rc.Width() / totaltime, rc.top,
					(m_vecSteps[m_nCurStep][i].end - m_vecSteps[m_nCurStep][i].start) * rc.Width() / totaltime, rc.Height(),
					NULL, 0, NULL, 0, 0, 0, 196);
			}
			len += m_vecSteps[m_nCurStep][i].end - m_vecSteps[m_nCurStep][i].start;
		}
	}

	m_pA3DEngine->FlushInternalSpriteBuffer();
	pViewPort->Active();
	A3DRELEASE(pNewViewPort);

	if( m_bPreview )
	{
		int sec = gr.get_real_time() / 1000;
		m_nPreviewSecTime = sec - m_vecSteps[m_nCurStep][m_nPreviewSection].start;
		if( sec >= m_vecSteps[m_nCurStep][m_nPreviewSection].end )
		{
			m_nPreviewSection++;
			if( m_nPreviewSection >= (int)m_vecSteps[m_nCurStep].size() )
				m_bPreview = false;
			else
			{
				m_nPreviewSecTime = 0;
				gr.set_seek_time(m_vecSteps[m_nCurStep][m_nPreviewSection].start * 1000);
			}
		}
	}

	m_pBtn_Redo->Enable(gr.is_paused());
	m_pBtn_Undo->Enable(gr.is_paused());

	return true;
}

void CDlgReplayEdit::InitEdit()
{
	m_vecSteps.clear();
	m_nCurStep = 0;
	VECTOR_SECTION vecsecton;
	SECTION sec;
	sec.start = 0;
	sec.end = (gr.get_total_time() - 1) / 1000 + 1;
	vecsecton.push_back(sec);
	m_vecSteps.push_back(vecsecton);
	m_nSelStart = m_nSelEnd = m_nClick = -1;
	m_bDrag = false;
	m_bPreview = false;
	m_nPreviewSection = 0;
	m_nPreviewSecTime = 0;
	m_fScale = 1.0f;
}

bool CDlgReplayEdit::GetRecordPos(int x, int &sec, int &pos, bool cut)
{
	A3DRECT rc;
	rc.left = m_pImg_Record->GetPos().x;
	rc.top = m_pImg_Record->GetPos().y;
	rc.right = rc.left + m_pImg_Record->GetSize().cx;
	rc.bottom = rc.top + m_pImg_Record->GetSize().cy;
	int W = rc.Width();
	if( m_pScl_Record->IsShow() )
	{
		rc.left -= W * m_pScl_Record->GetBarLevel() / m_pScl_Record->GetBarLength();
		rc.right += W * (m_pScl_Record->GetScrollEnd() - m_pScl_Record->GetBarLevel()) / m_pScl_Record->GetBarLength();
	}
	int min = (gr.get_total_time() - 1) / 1000 + 1;
	if( cut )
		pos = int(float(min) * (x - rc.left) / rc.Width() + 0.5f);
	else
		pos = int(float(min) * (x - rc.left) / rc.Width());
	if( x < rc.left )
	{
		sec = -1;
		return false;
	}
	for(sec = 0;sec < (int)m_vecSteps[m_nCurStep].size(); sec++)
	{
		if( pos < m_vecSteps[m_nCurStep][sec].end - m_vecSteps[m_nCurStep][sec].start)
			return true;
		else
			pos -= m_vecSteps[m_nCurStep][sec].end - m_vecSteps[m_nCurStep][sec].start;
	}
	return false;
}