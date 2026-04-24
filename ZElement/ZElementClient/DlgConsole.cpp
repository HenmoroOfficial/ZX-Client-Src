// File		: DlgConsole.cpp
// Creator	: Xiao Zhou
// Date		: 2005/8/16

#include "DlgConsole.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUIConsole.h"
#include "AScriptFile.h"
#include "EC_Game.h"

#define new A_DEBUG_NEW

const AString CDlgConsole::SCRIPT_NAME = "cmdscript.txt";

AUI_BEGIN_COMMAND_MAP(CDlgConsole, CDlgBase)

AUI_ON_COMMAND("Command_Edit",	OnCommandEdit)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgConsole, CDlgBase)

AUI_ON_EVENT("*",	WM_KEYDOWN,	OnEventKeyDown)
AUI_ON_EVENT("*",	WM_CHAR,	OnEventChar)

AUI_END_EVENT_MAP()


CDlgConsole::CDlgConsole()
{
	m_pCommand = NULL;
	m_pCommandEdit = NULL;
}

CDlgConsole::~CDlgConsole()
{
}

void CDlgConsole::DoDataExchange(bool bSave)
{
	CDlgBase::DoDataExchange(bSave);

	DDX_Control("DEFAULT_Command", m_pCommand);
	DDX_Control("Command_Edit", m_pCommandEdit);
}

bool CDlgConsole::OnInitDialog()
{
	((PAUICONSOLE)GetDlgItem("Command_Edit"))->
		SetEditBox(GetDlgItem("DEFAULT_Command"));
	((PAUICONSOLE)GetDlgItem("Command_Edit"))->FitScreen();
	return true;
}

void CDlgConsole::OnShowDialog()
{
	m_vecCmds.clear();
}

void CDlgConsole::OnTick()
{
	if (GetGameUIMan()->GetActiveDialog()!=this)
	{
		GetGameUIMan()->BringWindowToTop(this);
		ChangeFocus(m_pCommandEdit);
	}

	if (m_iRunTimes>0 && m_vecCmds.size()>0)
	{
		m_iTickTime += (int)GetGame()->GetTickTime();
		if(m_vecCmds[m_iCurIndex].t < m_iTickTime)
		{
			m_pCommandEdit->AddLine(AS2AC(m_vecCmds[m_iCurIndex].cmd), A3DCOLORRGB(0, 255, 255));
			GetGameUIMan()->OnDlgCmd_Console(AS2AC(m_vecCmds[m_iCurIndex].cmd));
			if(m_vecCmds.empty())
				m_pCommandEdit->AddLine(_AL("script run end"), A3DCOLORRGB(0, 255, 0));
			m_iCurIndex++;
			m_iTickTime = 0;
		}

		if (m_iCurIndex>=m_vecCmds.size())
		{
			m_iCurIndex = 0;
			m_iRunTimes--;
		}
	}
}

void CDlgConsole::OnCommandEdit(const char *szCommand)
{
	ACString strCmd = m_pCommand->GetText();

	m_pCommandEdit->SetCommandLine(strCmd);
	m_pCommandEdit->OnDlgItemMessage(WM_CHAR, ACHAR('\r'), 0);

	GetGameUIMan()->OnDlgCmd_Console(strCmd);
	m_pCommand->SetText(_AL(""));
}

void CDlgConsole::OnEventKeyDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int nVirtKey = int(wParam);

	if( nVirtKey == VK_ESCAPE )
	{
		Show(false);
		return;
	}
	else if( nVirtKey == VK_UP || nVirtKey == VK_DOWN )
	{
		m_pCommandEdit->OnDlgItemMessage(WM_KEYDOWN, wParam, lParam);
		m_pCommand->SetText(m_pCommandEdit->GetCommandLine());
	}
	else if( nVirtKey == VK_PRIOR || nVirtKey == VK_NEXT )
	{
		m_pCommandEdit->OnDlgItemMessage(WM_KEYDOWN, wParam, lParam);
	}

	ChangeFocus(m_pCommand);
}

void CDlgConsole::OnEventChar(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( ACHAR(wParam) == ACHAR('~') )
	{
		Show(false);
		m_pCommand->SetText(_AL(""));
		return;
	}

	ChangeFocus(m_pCommand);
}

int CDlgConsole::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="run")
	{
		m_iRunTimes = wParam;
		if (m_iRunTimes==0)
		{
			m_iRunTimes = 1;
		}
		m_iTickTime = 0;
		m_iCurIndex = 0;
		m_vecCmds.clear();
		AScriptFile sfile;
		if (!sfile.Open(SCRIPT_NAME))
			return 1;
		
		while (sfile.PeekNextToken(true))
		{
			CMD_INFO info;
			
			sfile.GetNextToken(true);
			info.t = atoi(sfile.m_szToken);
			sfile.GetNextToken(true);
			info.cmd = sfile.m_szToken;

			m_vecCmds.push_back(info);
		}
		ACString strBeginInfo;
		strBeginInfo = ACString(_AL("script run begin from file: ")) + AS2AC(SCRIPT_NAME);
		m_pCommandEdit->AddLine(strBeginInfo, A3DCOLORRGB(0, 255, 0));
		
		sfile.Close();
	}

	return 0;
}