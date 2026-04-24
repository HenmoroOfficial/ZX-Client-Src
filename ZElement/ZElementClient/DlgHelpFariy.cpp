// Filename	: DlgHelpFariy.cpp
// Creator	: Fu Chonggang
// Date		: 2009/08/19

#include "AFI.h"
#include "AWScriptFile.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "DlgHelpFariy.h"
#include "DlgHelpMemo.h"
#include "DlgMinimizeBar.h"
#include "EC_GameUIMan.h"
#include "EC_Global.h"


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgHelpFariy, CDlgBase)

AUI_ON_COMMAND("Btn_Help",		OnCommand_Help)
AUI_ON_COMMAND("Btn_Search",	OnCommand_Search)
AUI_ON_COMMAND("Btn_Memo",		OnCommand_Memo)
AUI_ON_COMMAND("Btn_Set",		OnCommand_Set)
AUI_ON_COMMAND("Btn_DressUp",	OnCommand_Change)
AUI_ON_COMMAND("Btn_Hide",		OnCommand_Hide)
AUI_ON_COMMAND("Btn_Close",		OnCommand_Close)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_IDCANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgHelpFariy, CDlgBase)

AUI_ON_EVENT("Img_Assistant",		WM_LBUTTONDOWN,		OnEventLButtonDownAssist)
AUI_ON_EVENT("Img_Assistant",		WM_LBUTTONUP,		OnEventLButtonUpAssist)
AUI_ON_EVENT("Img_Assistant",		WM_RBUTTONDOWN,		OnEventLButtonDblClkDownAssist)
AUI_ON_EVENT("Img_Assistant",		WM_LBUTTONDBLCLK,	OnEventLButtonDblClkDownAssist)

AUI_END_EVENT_MAP()

UINT CDlgHelpFariy::m_iCurIndex = 0;
DWORD CDlgHelpFariy::m_dwTimeRemind = 0;
DWORD CDlgHelpFariy::m_dwTimeClose  = 0;
bool CDlgHelpFariy::m_bRemind = 0;
abase::vector<ACString>	CDlgHelpFariy::m_HelpMsgs;

CDlgHelpFariy::CDlgHelpFariy()
{
	m_pBtn_Help		= NULL;
	m_pBtn_Search	= NULL;
	m_pBtn_Memo		= NULL;
	m_pBtn_Set		= NULL;
	m_pBtn_Change	= NULL;
	m_pBtn_Hide		= NULL;
	m_pBtn_Close	= NULL;

	m_pLab_Hint		= NULL;
	m_pImgAssistant	= NULL;
}

CDlgHelpFariy::~CDlgHelpFariy()
{
}

void CDlgHelpFariy::OnCommand_Help(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Help_Cyclo");
	if(pDlg)
		pDlg->Show(!pDlg->IsShow());
}

void CDlgHelpFariy::OnCommand_Search(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Help_SearchResult");
	if(pDlg)
		pDlg->Show(!pDlg->IsShow());
	if (pDlg->IsShow())
		GetGameUIMan()->GetDialog("Win_Help_Cyclo")->Show(false);
}

void CDlgHelpFariy::OnCommand_Memo(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Help_Memo");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgHelpFariy::OnCommand_Set(const char * szCommand)
{
	return;
	if (m_pBtn_Change->IsShow())
	{
		m_pBtn_Change->Show(false);
		m_pBtn_Hide->Show(false);
		m_pBtn_Close->Show(false);
	}
	else
	{
		m_pBtn_Change->Show(true);
		m_pBtn_Hide->Show(true);
		m_pBtn_Close->Show(true);
	}
}

void CDlgHelpFariy::OnCommand_Change(const char * szCommand)
{
}

void CDlgHelpFariy::OnCommand_Hide(const char * szCommand)
{
	m_bRemind = false;
	GetGameUIMan()->m_pDlgMinimizeBar[0]->MinimizeDialog(this, 
		GetGameUIMan()->GetStringFromTable(8595), CDlgMinimizeBar::MINIMIZEBAR_TYPE_SECRETARY);
}

void CDlgHelpFariy::OnCommand_IDCANCEL(const char * szCommand)
{
	CDlgHelpMemo *pDlgHelpMemo = (CDlgHelpMemo*)GetGameUIMan()->GetDialog("Win_Help_Memo");
	if(!pDlgHelpMemo->CloseOneReminding())
		OnCommand_Close("");
}

void CDlgHelpFariy::OnCommand_Close(const char * szCommand)
{
	if (m_dwTimeClose==0xFFFFFFFF)
		m_dwTimeClose = GetTickCount();
	m_pBtn_Help->Show(false);
	m_pBtn_Search->Show(false);
	m_pBtn_Memo->Show(false);
	m_pBtn_Set->Show(false);
//	m_pBtn_Change->Show(false);
	m_pBtn_Hide->Show(false);
	m_pBtn_Close->Show(false);
	m_pLab_Hint->SetText(GetStringFromTable(9123));
}

void CDlgHelpFariy::OnEventLButtonDownAssist(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	m_iX = GET_X_LPARAM(lParam) - p->X;
	m_iY = GET_Y_LPARAM(lParam) - p->Y;
}
void CDlgHelpFariy::OnEventLButtonUpAssist(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	if (abs(x-m_iX)<3 && abs(y-m_iY)<3)
	{
		if (m_pBtn_Help->IsShow())
		{
			m_pBtn_Help->Show(false);
			m_pBtn_Search->Show(false);
			m_pBtn_Memo->Show(false);
// 			m_pBtn_Set->Show(false);
// 			m_pBtn_Change->Show(false);
// 			m_pBtn_Hide->Show(false);
// 			m_pBtn_Close->Show(false);
		}
		else
		{
			m_pBtn_Help->Show(true);
			m_pBtn_Search->Show(true);
			m_pBtn_Memo->Show(true);
			//m_pBtn_Set->Show(true);
		}
	}
}

void CDlgHelpFariy::OnEventLButtonDblClkDownAssist(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	CDlgHelpMemo *pDlgHelpMemo = (CDlgHelpMemo*)GetGameUIMan()->GetDialog("Win_Help_Memo");
	if(!pDlgHelpMemo->CloseOneReminding())
		OnCommand_Close("");
}

void CDlgHelpFariy::OnTick()
{
	static DWORD dwTime = GetTickCount();
	DWORD dwTimeNow = GetTickCount();
	const static int iRemind = 30;
	if (stricmp(m_szName,"Win_Help_Fairy")==0)
	{
// 		switch (m_iState)
// 		{
// 		case ACTIONSTATE_SHOW:
// 			{
// 				if (dwTimeNow > (dwTime+350))
// 				{
// 					dwTime = dwTimeNow;
// 					m_pImgAssistant->FixFrame(m_iCurFrame++);
// 					if (m_iCurFrame>11)
// 					{
// 						m_iState = ACTIONSTATE_FALLOW;
// 					}
// 					break;
// 				}
// 			}
// 		case ACTIONSTATE_FALLOW:
// 			{
// 				if (dwTimeNow > (dwTime+250))
// 				{
// 					dwTime = dwTimeNow;
// 					m_pImgAssistant->FixFrame(m_iCurFrame++);
// 					if (m_iCurFrame>5)
// 					{
// 						m_iCurFrame = 0;
// 					}
// 					break;
// 				}
// 			}
// 		case ACTIONSTATE_HIDE:
// 			break;
// 		default :
// 			assert(0);
// 		}
		if (m_bRemind)
		{
			if (dwTimeNow-dwTime>iRemind*1000)
			{
				m_dwTimeRemind = dwTimeNow;
				dwTime = dwTimeNow;
				m_pLab_Hint->Show(true);
				m_pLab_Hint->SetText(m_HelpMsgs[m_iCurIndex]);
				m_iCurIndex++;
				if (m_iCurIndex>=m_HelpMsgs.size())
					m_iCurIndex = 0;
			}
// 			if (dwTimeNow-m_dwTimeRemind>15000)
// 			{
// 				m_pLab_Hint->Show(false);
// 			}
		}
		else
		{
			m_pLab_Hint->Show(false);
		}
		if (dwTimeNow > m_dwTimeClose)
		{
			if (GetDlgItem("Img_Assistant")->GetAlpha()==0)
				Show(false);
			int alpha = 255-(dwTimeNow - m_dwTimeClose)/10;
			if (alpha<0)
				alpha = 0;
			GetDlgItem("Img_Assistant")->SetAlpha(alpha);
			GetDlgItem("Img_Eye")->SetAlpha(alpha);
		}
		
	}
}

bool CDlgHelpFariy::OnInitDialog()
{
	if (stricmp(m_szName,"Win_Help_Fairy")==0)
	{
		DDX_Control("Btn_Help", m_pBtn_Help);
		DDX_Control("Btn_Search", m_pBtn_Search);
		DDX_Control("Btn_Memo", m_pBtn_Memo);
		DDX_Control("Btn_Set", m_pBtn_Set);
		DDX_Control("Btn_DressUp", m_pBtn_Change);
		DDX_Control("Btn_Hide", m_pBtn_Hide);
		DDX_Control("Btn_Close", m_pBtn_Close);
		DDX_Control("Txt_Hint", m_pLab_Hint);
		DDX_Control("Img_Assistant", m_pImgAssistant);
		AUIOBJECT_SETPROPERTY p;
		m_pImgAssistant->GetProperty("Frames Number",&p);
		//m_pImgAssistant->FixFrame(6);
		m_iNumFrames = p.i;
		m_iCurFrame  = 6;
		m_iState = ACTIONSTATE_MIN;
		m_pBtn_Set->Show(false);
		m_pBtn_Change->Show(false);

		AWScriptFile s;
		m_HelpMsgs.clear();
		if( s.Open("surfaces\\ingame\\HelpFairy.txt") )
		{
			while( !s.IsEnd() )
			{
				if( !s.GetNextToken(true) )
					break;

				m_HelpMsgs.push_back(s.m_szToken);
			}
		}
		else
			AUI_ReportError(__LINE__, 1,"CDlgHelpFariy::OnInitDialog(), help string file error!");
	}
	return true;
}

void CDlgHelpFariy::OnShowDialog()
{
	if (stricmp(m_szName,"Win_Help_Fairy")==0)
	{
		m_iState = ACTIONSTATE_SHOW;
		m_iCurFrame = 6;
		m_bRemind = true;
		m_pBtn_Help->Show(false);
		m_pBtn_Search->Show(false);
		m_pBtn_Memo->Show(false);
		m_pBtn_Set->Show(false);
		m_pBtn_Change->Show(false);
		m_pBtn_Hide->Show(true);
		m_pBtn_Close->Show(true);
		m_dwTimeRemind = GetTickCount();
		m_pLab_Hint->Show(true);
		m_pLab_Hint->SetText(m_HelpMsgs[m_iCurIndex]);
		m_iCurIndex++;
		if (m_iCurIndex>=m_HelpMsgs.size())
			m_iCurIndex = 0;
		m_dwTimeClose = 0xFFFFFFFF;
		GetDlgItem("Img_Assistant")->SetAlpha(255);
		GetDlgItem("Img_Eye")->SetAlpha(255);
	}
}
