// Filename	: DlgIcons.h
// Creator	: Fu Chonggang
// Date		: 2013/4/22

#include "DlgIcons.h"
#include "AUI\\AUICheckBox.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"

AUI_BEGIN_COMMAND_MAP(CDlgIcons, CDlgBase)

AUI_ON_COMMAND("Chk_Fold",	OnCommandMinimize)

AUI_END_COMMAND_MAP()

const int	CDlgIcons::m_isTime = 200;

void CDlgIcons::OnCommandMinimize(const char* szCommand)
{
	// 如果随时可以点击逻辑会比较复杂
	if (m_bAction)
	{
		PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_Fold");
		pChk->Check(!pChk->IsChecked());
		return;
	}
	BuildValidIcons();
	m_bAction = true;
	m_dwActionTime = GetTickCount();
}

void CDlgIcons::BuildValidIcons()
{
	const char *szDlgs[] = {
		"Win_ZhuxianExplore_Min",
		"Win_Battle_Dungeon_6v6_Min",
		"Win_Activity_Min",
		"Win_Rose_Min",
		"Win_Kf2012Bet_Min",
		"Win_AniversaryMail5_Min",
		"Win_Explorer_Popu_Min",
		"Win_ZhuxianKnows_Min",
		"Win_Help_GameGuide_Min",
		"Win_King_Min",
		"Win_Game_Touch_Min",
		"Win_Radio_Min",	
		"Win_Battle_Dungeon_6v6_Min_Expand",	
	};

	if (GetGameUIMan()->GetDialog(szDlgs[0])->IsShow())
	{
		PAUIDIALOG pDlg = this;
		m_vecDlgs.clear();
		int i;
		for (i=0; i<sizeof(szDlgs)/sizeof(char*); ++i)
		{
			if (GetGameUIMan()->GetDialog(szDlgs[i])->IsShow())
			{
				DlgPos info;
				info.pDlg = GetGameUIMan()->GetDialog(szDlgs[i]);
				info.pDlg->SetCanOutOfWindow(true);
				info.pDlg->AlignTo(pDlg,
					AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
					AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
				info.x = info.pDlg->GetPos().x;
				m_vecDlgs.push_back(info);
				pDlg = info.pDlg;
			}
		}
	}
}

void CDlgIcons::OnTick()
{
	if (!m_bAction)
		return;

	DWORD dwTime = GetTickCount();
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_Fold");
	int iScreenWidth = GetGameUIMan()->GetRect().Width();
	unsigned int i;
	if (!pChk->IsChecked())
	{
		if (dwTime>(m_dwActionTime+m_isTime))
		{
			for (i=0; i<m_vecDlgs.size(); ++i)
			{
				m_vecDlgs[i].pDlg->SetPosEx(m_vecDlgs[i].x, m_vecDlgs[i].pDlg->GetPos().y);
			}
			m_bAction = false;
		}
		else
		{
			for (i=0; i<m_vecDlgs.size(); ++i)
			{
				m_vecDlgs[i].pDlg->SetPosEx(iScreenWidth - (iScreenWidth-m_vecDlgs[i].x)*(dwTime-m_dwActionTime)/m_isTime, m_vecDlgs[i].pDlg->GetPos().y);
			}
		}
	}
	else
	{
		if (dwTime>(m_dwActionTime+m_isTime))
		{
			for (i=0; i<m_vecDlgs.size(); ++i)
			{
				m_vecDlgs[i].pDlg->SetPosEx(iScreenWidth, m_vecDlgs[i].pDlg->GetPos().y);
			}
			m_bAction = false;
		}
		else
		{
			for (i=0; i<m_vecDlgs.size(); ++i)
			{
				m_vecDlgs[i].pDlg->SetPosEx(m_vecDlgs[i].x + (iScreenWidth-m_vecDlgs[i].x)*(dwTime-m_dwActionTime)/m_isTime, m_vecDlgs[i].pDlg->GetPos().y);
			}
		}
	}
}
