/********************************************************************
	created:	2005/08/11
	created:	11:8:2005   13:04
	file base:	DlgHelp
	file ext:	cpp
	author:		yaojun
	changed:	Fu Chonggang 2009-08-20
	
	purpose:	
*********************************************************************/

#include "EC_Global.h"
#include "DlgHelp.h"
#include <AWScriptFile.h>
#include <AFI.h>
#include <A2DSprite.h>
#include <A3DEngine.h>
#include <A3DViewport.h>
#include "AUI//AUITextArea.h"
#include "aui//AUIStillImageButton.h"
#include "aui//AUITreeView.h"
#include "aui//AUIImagePicture.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_Secretary.h"

#include "AWScriptFile.h"
#include "AUI\\CSplit.h"


#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

#define new A_DEBUG_NEW


AUI_BEGIN_COMMAND_MAP(CDlgHelp, CDlgBase)

AUI_ON_COMMAND("Rdo_Group1_*",	OnCommandRdoGroup1)
AUI_ON_COMMAND("Rdo_Group2_*",	OnCommandRdoGroup2)
AUI_ON_COMMAND("Btn_Prev",		OnCommandPrev)
AUI_ON_COMMAND("Btn_Next",		OnCommandNext)
AUI_ON_COMMAND("Btn_UpInfo",	OnCommandUpInfo)
AUI_ON_COMMAND("Btn_Close",		OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgHelp, CDlgBase)

AUI_ON_EVENT("Wp_html", WM_LBUTTONDOWN, OnLButtonDownHtml)

AUI_END_EVENT_MAP()



CDlgHelp::CDlgHelp()
{
}

CDlgHelp::~CDlgHelp()
{
}

bool CDlgHelp::OnInitDialog()
{
	if (!CDlgBase::OnInitDialog())
		return false;

	int i = 0, j = 0;
	AString strName;
	AWScriptFile s;
	if( s.Open("configs\\HelpIndex.txt") )
	{
		while( s.PeekNextToken(true) )
		{
			int n = s.GetNextTokenAsInt(true);
			s.GetNextToken(true);
			if (n<=0 || i>11)
			{
				break;
			}
			strName.Format("Rdo_Group1_%d",i+1);
			GetDlgItem(strName)->SetText(s.m_szToken);
			TableInfo info;
			for(j = 0; j < n; j++ )
			{
				int id = s.GetNextTokenAsInt(true);
				s.GetNextToken(true);
				info.strName = s.m_szToken;
				s.GetNextToken(true);
				info.strUrl = s.m_szToken;
				if( i < CDLGHELL_MAINTYPE_MAX && j < CDLGHELP_SUBTYPE_MAX )
				{
					m_idTable[i][j] = id;
					m_mapTableInfo[id] = info;
				}
			}
			for (;j<CDLGHELP_SUBTYPE_MAX;j++)
			{
				m_idTable[i][j] = 0;
			}
			i++;
		}
		s.Close();
	}
	else
		AUI_ReportError(__LINE__, 1,"CDlgRankList::OnInitDialog(), taptable.txt error!");

	for(;i<CDLGHELL_MAINTYPE_MAX;i++)
	{
		strName.Format("Rdo_Group1_%d",i+1);
		GetDlgItem(strName)->Show(false);
	}
	CheckRadioButton(1,1);
	OnCommandRdoGroup1("Rdo_Group1_1");
	

	return true;
}

void CDlgHelp::DoDataExchange(bool bSave)
{
	CDlgBase::DoDataExchange(bSave);

}

void CDlgHelp::OnTick()
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("win_explorer");
	if (!pDlg->IsShow())
	{
		CheckRadioButton(2, -1);
	}
}

bool CDlgHelp::Render()
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("win_explorer");
	
	return CDlgBase::Render();
}

void CDlgHelp::OnCommandRdoGroup1(const char *szCommand)
{
	int nSel = atoi(szCommand + strlen("Rdo_Group1_"))-1;
	int i;
	m_idCurTable = -1;
	AString strName;
	for(i = 0; i < CDLGHELP_SUBTYPE_MAX; i++ )
	{
		if (i>=10)
		{
			return;
		}
		strName.Format("Rdo_Group2_%d",i+1);
		if( m_idTable[nSel][i] != 0 )
		{
			GetDlgItem(strName)->Show(true);
			GetDlgItem(strName)->SetText(m_mapTableInfo[m_idTable[nSel][i]].strName);
		}
		else
			GetDlgItem(strName)->Show(false);
	}
	CheckRadioButton(2,1);
	OnCommandRdoGroup2("");
}

void CDlgHelp::OnCommandRdoGroup2(const char *szCommand)
{
	int mainType = GetCheckedRadioButton(1)-1;
	int subType = GetCheckedRadioButton(2)-1;
	int id = m_idTable[mainType][subType];
	if (id>0)
	{
		TableInfo info = m_mapTableInfo[id];
		AString strFullPath;
		if (a_strstr(info.strUrl,_AL("http")))
			strFullPath = AC2AS(info.strUrl);
		else
			af_GetFullPath(strFullPath, AC2AS(info.strUrl));
		CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
		pSecretary->SetURL(strFullPath, GetStringFromTable(9127));
	}
}

void CDlgHelp::OnCommandPrev(const char *szCommand)
{
	
}

void CDlgHelp::OnCommandNext(const char *szCommand)
{
}

void CDlgHelp::OnCommandUpInfo(const char *szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Help_LvUp");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgHelp::OnCommandCancel(const char *szCommand)
{
	Show(false);
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("win_explorer");
	if (pDlg->IsShow())
		pDlg->Show(false);
}

void CDlgHelp::OnLButtonDownHtml(WPARAM, LPARAM lParam, AUIObject *)
{

}
