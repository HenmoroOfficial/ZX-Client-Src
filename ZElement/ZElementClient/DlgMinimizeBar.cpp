// Filename	: DlgMinimizeBar.cpp
// Creator	: Xiao Zhou
// Date		: 2005/11/14

#include "DlgMinimizeBar.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUICommon.h"
#include "DlgSystem.h"
#include "DlgSystem2.h"
#include "DlgSystem3.h"
#include "DlgBooth.h"

#include "EC_HostPlayer.h"

int	CDlgMinimizeBar::m_nTotalBars;
bool CDlgMinimizeBar::m_bSystemMenuExpand;

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgMinimizeBar, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",	OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgMinimizeBar, CDlgBase)

AUI_ON_EVENT("*",	WM_LBUTTONDBLCLK,	OnEventLButtonDBCLK)
AUI_ON_EVENT(NULL,	WM_LBUTTONDBLCLK,	OnEventLButtonDBCLK)

AUI_END_EVENT_MAP()

CDlgMinimizeBar::CDlgMinimizeBar()
{
	m_pTxt_Subject = NULL;
	m_nTotalBars = 0;
	m_bSystemMenuExpand = false;
}

CDlgMinimizeBar::~CDlgMinimizeBar()
{
}

void CDlgMinimizeBar::OnCommandCancel(const char * szCommand)
{
	GetGameUIMan()->RespawnMessage();
}

bool CDlgMinimizeBar::OnInitDialog()
{
	DDX_Control("Txt_Subject", m_pTxt_Subject);
	m_pTxt_Subject->SetAlign(AUIFRAME_ALIGN_CENTER);
	SetCanMove(false);

	return true;
}

void CDlgMinimizeBar::OnTick()
{
	if( m_bFlash && (GetTickCount() / 600) % 2 )
		m_pTxt_Subject->SetColor(A3DCOLORRGB(128, 128, 128));
	else
		switch(GetData())
		{
		case MINIMIZEBAR_TYPE_FRIEND:
			m_pTxt_Subject->SetColor(A3DCOLORRGB(0, 255, 0));
			break;
		case MINIMIZEBAR_TYPE_WHISPER:
			m_pTxt_Subject->SetColor(A3DCOLORRGB(255, 74, 176));
			break;
		case MINIMIZEBAR_TYPE_CHATROOM:
			m_pTxt_Subject->SetColor(A3DCOLORRGB(255, 255, 255));
			break;
		case MINIMIZEBAR_TYPE_QUESTION:
			m_pTxt_Subject->SetColor(A3DCOLORRGB(255, 0, 0));
			break;
		case MINIMIZEBAR_TYPE_RIDEKICK:
			{
				if (!GetHostPlayer()->IsMultiRideLeader())
				{
					OnEventLButtonDBCLK(0,0,NULL);
				}
				m_pTxt_Subject->SetColor(A3DCOLORRGB(0, 255, 0));
			}
			break;
		case MINIMIZEBAR_TYPE_GARDEN:
			m_pTxt_Subject->SetColor(A3DCOLORRGB(0, 255, 0));
			break;
		case MINIMIZEBAR_TYPE_BROADCAST:
			m_pTxt_Subject->SetColor(A3DCOLORRGB(122, 234, 255));
			break;
		case MINIMIZEBAR_TYPE_BOOTH:
			{
				m_pTxt_Subject->SetColor(A3DCOLORRGB(122, 234, 255));
				CDlgBooth* pBooth = (CDlgBooth*)GetGameUIMan()->GetDialog("Win_PShop1");
				pBooth->TickBoothMinimize();
				break;
			}
		default:
			m_pTxt_Subject->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	
	PAUIDIALOG pDlg = (PAUIDIALOG)GetDataPtr();
	if (pDlg && pDlg->IsShow())
	{
		Show(false);
	}
}

void CDlgMinimizeBar::OnEventLButtonDBCLK(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUIDIALOG pDlg = (PAUIDIALOG)GetDataPtr();
	if( pDlg )
	{
		int i = atoi(GetName() + strlen("Win_MinimizeBar"));
		int j;
		for( j = i; j < m_nTotalBars - 1; j++ )
		{
			GetGameUIMan()->m_pDlgMinimizeBar[j]->SetDataPtr(
				GetGameUIMan()->m_pDlgMinimizeBar[j + 1]->GetDataPtr());
			GetGameUIMan()->m_pDlgMinimizeBar[j]->m_pTxt_Subject->SetText(
				GetGameUIMan()->m_pDlgMinimizeBar[j + 1]->m_pTxt_Subject->GetText());
			GetGameUIMan()->m_pDlgMinimizeBar[j]->m_bFlash = 
				GetGameUIMan()->m_pDlgMinimizeBar[j + 1]->m_bFlash;
			GetGameUIMan()->m_pDlgMinimizeBar[j]->SetData(GetGameUIMan()->m_pDlgMinimizeBar[j + 1]->GetData());
		}
		m_nTotalBars--;
		GetGameUIMan()->m_pDlgMinimizeBar[m_nTotalBars]->Show(false);
		pDlg->Show(true);
	}
}

void CDlgMinimizeBar::RestoreDialog(PAUIDIALOG pDlg)
{
	int i, j;
	for(i = 0; i < m_nTotalBars; i++ )
		if( GetGameUIMan()->m_pDlgMinimizeBar[i]->GetDataPtr() == pDlg )
		{
			for( j = i; j < m_nTotalBars - 1; j++ )
			{
				GetGameUIMan()->m_pDlgMinimizeBar[j]->SetDataPtr(
					GetGameUIMan()->m_pDlgMinimizeBar[j + 1]->GetDataPtr());
				GetGameUIMan()->m_pDlgMinimizeBar[j]->m_pTxt_Subject->SetText(
					GetGameUIMan()->m_pDlgMinimizeBar[j + 1]->m_pTxt_Subject->GetText());
				GetGameUIMan()->m_pDlgMinimizeBar[j]->m_bFlash = 
					GetGameUIMan()->m_pDlgMinimizeBar[j + 1]->m_bFlash;
				GetGameUIMan()->m_pDlgMinimizeBar[j]->SetData(GetGameUIMan()->m_pDlgMinimizeBar[j + 1]->GetData());
			}
			m_nTotalBars--;
			GetGameUIMan()->m_pDlgMinimizeBar[m_nTotalBars]->Show(false);
			break;
		}
}

void CDlgMinimizeBar::FlashDialog(PAUIDIALOG pDlg)
{
	int i;
	for(i = 0; i < m_nTotalBars; i++ )
		if( GetGameUIMan()->m_pDlgMinimizeBar[i]->GetDataPtr() == pDlg )
		{
			GetGameUIMan()->m_pDlgMinimizeBar[i]->m_bFlash = true;
			break;
		}
}

void CDlgMinimizeBar::MinimizeDialog(PAUIDIALOG pDlg, const ACHAR * szSubject, int nType)
{
	if( m_nTotalBars < CECGAMEUIMAN_MAX_MINIMIZEWINDOWS )
	{
		CDlgMinimizeBar* pNew = GetGameUIMan()->m_pDlgMinimizeBar[m_nTotalBars];
		ACString strSubject;
		switch(nType)
		{
		case MINIMIZEBAR_TYPE_FRIEND:
			pNew->m_pTxt_Subject->SetColor(A3DCOLORRGB(0, 255, 0));
			break;
		case MINIMIZEBAR_TYPE_WHISPER:
			pNew->m_pTxt_Subject->SetColor(A3DCOLORRGB(255, 74, 176));
			break;
		case MINIMIZEBAR_TYPE_CHATROOM:
			pNew->m_pTxt_Subject->SetColor(A3DCOLORRGB(255, 255, 255));
			break;
		case MINIMIZEBAR_TYPE_QUESTION:
			pNew->m_pTxt_Subject->SetColor(A3DCOLORRGB(255, 0, 0));
			break;
		case MINIMIZEBAR_TYPE_RIDEKICK:
			pNew->m_pTxt_Subject->SetColor(A3DCOLORRGB(0, 255, 0));
			break;
		default:
			pNew->m_pTxt_Subject->SetColor(A3DCOLORRGB(255, 255, 255));
		}
		int nWidth, nHeight, nLines;
		AUI_GetTextRect(GetGameUIMan()->GetDefaultFont(), szSubject, nWidth, nHeight, nLines);
		if( nWidth <= pNew->m_pTxt_Subject->GetSize().cx )
			strSubject = szSubject;
		else
		{
			ACString szText = szSubject;
			int i;
			AUI_GetTextRect(m_pTxt_Subject->GetFont(), _AL("..."), 
				nWidth, nHeight, nLines);
			int nWidthAdd = nWidth;
			for( i = a_strlen(szSubject) - 1; i > 0; i-- )
			{
				AUI_GetTextRect(m_pTxt_Subject->GetFont(), szText.Left(i), 
					nWidth, nHeight, nLines);
				if( nWidth + nWidthAdd <= pNew->m_pTxt_Subject->GetSize().cx )
				{
					strSubject = szText.Left(i) + _AL("...");
					break;
				}
			}
		}
		pNew->m_pTxt_Subject->SetText(strSubject);
		pNew->m_bFlash = false;
		pNew->SetDataPtr(pDlg);
		pNew->SetData(nType);
		pNew->Show(true);
		pDlg->Show(false);
		m_bSystemMenuExpand = !(GetGameUIMan()->m_pDlgSystem2->IsShow() || 
							  GetGameUIMan()->m_pDlgSystem3->IsShow());
		m_nTotalBars++;
		ArrangeMinimizeBar();
	}
}

void CDlgMinimizeBar::ArrangeMinimizeBar()
{
	int i;
	bool b = GetGameUIMan()->m_pDlgSystem2->IsShow() || 
			 GetGameUIMan()->m_pDlgSystem3->IsShow();
	if( b != m_bSystemMenuExpand )
	{
		CDlgMinimizeBar *pLast = GetGameUIMan()->m_pDlgMinimizeBar[0];
		pLast->AlignTo(GetGameUIMan()->m_pDlgSystem,
				AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT,
				AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_TOP);
		CDlgMinimizeBar *pThis;
		for(i = 1; i < m_nTotalBars ; i++ )
		{
			pThis = GetGameUIMan()->m_pDlgMinimizeBar[i];
			pThis->AlignTo(pLast,
				AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT,
				AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_TOP);
			pLast = pThis;
		}
	}
}