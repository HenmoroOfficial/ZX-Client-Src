// File		: DlgBattleSong.cpp
// Creator	: Fuchonggang
// Date		: 2009/04/28

#include "AUI\\AUIListBox.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIImagePicture.h"
#include "DlgBattleSong.h"
#include "DlgHost.h"
#include "EC_Algorithm.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_World.h"
#include "EC_GPDataType.h"
#include "battlegetfield_re.hpp"
#include "battlegetlist_re.hpp"
#include "EC_HostPlayer.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattleSong, CDlgBase)

AUI_ON_COMMAND("giveup",		OnCommandGiveup)
AUI_ON_COMMAND("confirm",		OnCommandConfirm)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_ON_COMMAND("sortname",		OnCommand_SortName)
AUI_ON_COMMAND("sortoccupation",OnCommand_SortOccupation)
AUI_ON_COMMAND("sortlevel",		OnCommand_SortLevel)
AUI_ON_COMMAND("tail",			OnCommand_Tail)
AUI_ON_COMMAND("head",			OnCommand_Head)
AUI_ON_COMMAND("next",			OnCommand_Next)
AUI_ON_COMMAND("last",			OnCommand_Last)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBattleSong, CDlgBase)

AUI_ON_EVENT("List_Battle",	WM_LBUTTONDOWN,	OnEventLButtonDownBattle)

AUI_END_EVENT_MAP()

const int SongBattleInstNum = 4;
AUILabel *CDlgBattleSong::m_pLabTime;

AUILabel *CDlgBattleSong::m_pLabPoint;
AUILabel *CDlgBattleSong::m_pLabType;
AUILabel *CDlgBattleSong::m_pLabKill;
AUILabel *CDlgBattleSong::m_pLabRank;
AUILabel *CDlgBattleSong::m_pLabChuPoint;
AUILabel *CDlgBattleSong::m_pLabHanPoint;
AUILabel *CDlgBattleSong::m_pLabChuNum;
AUILabel *CDlgBattleSong::m_pLabHanNum;
AUIListBox *CDlgBattleSong::m_pLstScore;

CDlgBattleSong::CDlgBattleSong()
{
}

CDlgBattleSong::~CDlgBattleSong()
{
}

void CDlgBattleSong::OnCommandGiveup(const char * szCommand)
{
	GetGameSession()->battle_leave();
}

bool CDlgBattleSong::OnInitDialog()
{
	if (m_szName == "Win_BattleSong")
	{
		m_pLstSongBattle = (PAUILISTBOX)GetDlgItem("List_Battle");
		m_pLstSongBattle->ResetContent();
	}
	else if (m_szName == "Win_BattleSongScore")
	{
		m_pLabPoint	= (PAUILABEL)GetDlgItem("Txt_PerScore");
		m_pLabType	= (PAUILABEL)GetDlgItem("Txt_PerTitle");
		m_pLabKill	= (PAUILABEL)GetDlgItem("Txt_PerCombo");
		m_pLabRank	= (PAUILABEL)GetDlgItem("Txt_PerRank");
		m_pLabChuPoint	= (PAUILABEL)GetDlgItem("Txt_CampScore1");
		m_pLabHanPoint	= (PAUILABEL)GetDlgItem("Txt_CampScore2");
		m_pLabChuNum	= (PAUILABEL)GetDlgItem("Txt_Member1");
		m_pLabHanNum	= (PAUILABEL)GetDlgItem("Txt_Member2");
		m_pLstScore	= (PAUILISTBOX)GetDlgItem("Lst_All");
	}
	else if (m_szName == "Win_BattleSongTime")
	{
		m_pLstSongBattle = (PAUILISTBOX)GetDlgItem("Lst_Await");
		m_pLstSongBattle->SetLineSpace(3);

		m_pLabTime = (PAUILABEL)GetDlgItem("Lab_2");
	}
	return true;
}

void CDlgBattleSong::OnShowDialog()
{
	if (m_szName == "Win_BattleSong")
	{
		OnEventLButtonDownBattle(0,0,NULL);
	}
	else if (m_szName == "Win_BattleSongScore")
	{
	}
	else if (m_szName == "Win_BattleSongTime")
	{
	}
}

void CDlgBattleSong::OnTick()
{
	if (m_szName == "Win_BattleSong")
	{
		int nSel = m_pLstSongBattle->GetCurSel();
		int gs_id = m_pLstSongBattle->GetItemData(nSel, 1);
		if( gs_id == GetGameRun()->GetCurLine())
			GetDlgItem("Btn_Confirm")->Enable(true);
		else
			GetDlgItem("Btn_Confirm")->Enable(false);
	}
	else if (m_szName == "Win_BattleSongScore")
	{
		ACString strText;
		int nTime, nHour, nMin, nSec;
		nTime = GetGameUIMan()->m_pDlgHost->GetBattleTime() - GetGame()->GetServerGMTTime();
		if( nTime > 0 )
		{
			nHour = nTime / 3600;
			nMin = (nTime % 3600) / 60;
			nSec = nTime%60;
			if( nHour != 0 )
				strText.Format(GetStringFromTable(1603), nHour);
			else if( nMin != 0 )
				strText.Format(GetStringFromTable(1602), nMin);
			else
				strText.Format(GetStringFromTable(1601), nSec);
			GetDlgItem("Txt_TimeLeft")->SetText(strText);
			GetDlgItem("Txt_TimeLeft")->Show(true);
		}
		else
			GetDlgItem("Txt_TimeLeft")->Show(false);
	}
	else if (m_szName == "Win_BattleSongTime")
	{
		ACHAR szText[256];
		int nTime = GetGameUIMan()->m_pDlgHost->GetBattleTime() - GetGame()->GetServerGMTTime();
		if( nTime > 0 )
		{
			a_sprintf(szText,GetStringFromTable(7703),nTime/60,nTime%60);
			m_pLabTime->SetText(szText);
			if (nTime<5*60)
			{
				GetDlgItem("Btn_Leave")->Enable(false);
			}
			else
			{
				GetDlgItem("Btn_Leave")->Enable(true);
			}
		}
		else
		{
			a_sprintf(szText,GetStringFromTable(7703),0,0);
			m_pLabTime->SetText(szText);
		}
	}
}

void CDlgBattleSong::OnCommandConfirm(const char * szCommand)
{
	ACHAR szText[256];
	a_sprintf(szText, GetStringFromTable(7550),GetStringFromTable(7398+m_iMapId));
	GetGameUIMan()->MessageBox("Game_BattleSong", szText,
					MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
}

class SortBattleFieldByName
{
public:
	bool operator ()(CDlgBattleSong::BattleFieldInfo & p1, CDlgBattleSong::BattleFieldInfo & p2)
	{
		return p1.strName > p2.strName;
	}
};

class SortBattleFieldByOccupation
{
public:
	bool operator ()(CDlgBattleSong::BattleFieldInfo & p1, CDlgBattleSong::BattleFieldInfo & p2)
	{
		return p1.occupaton > p2.occupaton;
	}
};

class SortBattleFieldByLevel
{
public:
	bool operator ()(CDlgBattleSong::BattleFieldInfo & p1, CDlgBattleSong::BattleFieldInfo & p2)
	{
		return p1.level > p2.level;
	}
};

void CDlgBattleSong::OnCommand_SortName(const char * szCommand)
{
	BubbleSort(m_vecBattleFieldInfo.begin(),m_vecBattleFieldInfo.end(),SortBattleFieldByName());
	m_iCurPageBattleField = 0;
	RefreshBattleInfo();
}

void CDlgBattleSong::OnCommand_SortOccupation(const char * szCommand)
{
	BubbleSort(m_vecBattleFieldInfo.begin(),m_vecBattleFieldInfo.end(),SortBattleFieldByOccupation());
	m_iCurPageBattleField = 0;
	RefreshBattleInfo();
}

void CDlgBattleSong::OnCommand_SortLevel(const char * szCommand)
{
	BubbleSort(m_vecBattleFieldInfo.begin(),m_vecBattleFieldInfo.end(),SortBattleFieldByLevel());
	m_iCurPageBattleField = 0;
	RefreshBattleInfo();
}

void CDlgBattleSong::OnCommand_Tail(const char * szCommand)
{
	m_iCurPageBattleField = m_iMaxPageBattleField;
	RefreshBattleInfo();
}

void CDlgBattleSong::OnCommand_Head(const char * szCommand)
{
	m_iCurPageBattleField = 0;
	RefreshBattleInfo();
}

void CDlgBattleSong::OnCommand_Next(const char * szCommand)
{
	m_iCurPageBattleField++;
	if (m_iCurPageBattleField>m_iMaxPageBattleField)
	{
		m_iCurPageBattleField = m_iMaxPageBattleField;
	}
	RefreshBattleInfo();
}

void CDlgBattleSong::OnCommand_Last(const char * szCommand)
{
	m_iCurPageBattleField--;
	if (m_iCurPageBattleField<0)
	{
		m_iCurPageBattleField = 0;
	}
	RefreshBattleInfo();
}

void CDlgBattleSong::OnCommandCancel(const char * szCommand)
{
	GetGameUIMan()->EndNPCService();
	Show(false);
}

void CDlgBattleSong::OnEventLButtonDownBattle(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	PAUILISTBOX pList = m_pLstSongBattle;
	int index = pList->GetCurSel();
	m_iMapId = pList->GetItemData(index,0);
	
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Map");
	AUIOBJECT_SETPROPERTY p;
	strcpy(p.fn, AC2AS(GetStringFromTable(7438+m_iMapId)));
	pImg->SetProperty("Image File", &p);

	PAUITEXTAREA pTxt = (PAUITEXTAREA)GetDlgItem("Txt_BattleInfo");
	pTxt->SetText(GetStringFromTable(7488+m_iMapId));
}

void CDlgBattleSong::UpdateSongBattleInfo(void *pData)
{
	BattleGetField_Re *p = (BattleGetField_Re*)pData;
	if( p->retcode != ERR_SUCCESS )
		return;
	
	ACHAR szText[256];
	PAUILABEL pNum = (PAUILABEL)GetDlgItem("Lab_3");
	a_sprintf(szText,GetStringFromTable(7704),p->red_team.size()+p->blue_team.size());
	pNum->SetText(szText);
	GetGameUIMan()->m_pDlgHost->SetBattleTime(p->starttime);

	ACString strText;
	PAUILISTBOX pList = m_pLstSongBattle;
	pList->ResetContent();
	int i;
	m_vecBattleFieldInfo.clear();
	for(i = 0; i < (int)p->red_team.size(); i++ )
	{
		BattleFieldInfo info;
		ACString strName = ACString((const ACHAR*)p->red_team[i].name.begin(), p->red_team[i].name.size() / sizeof (ACHAR));
		info.strName = strName;
		info.occupaton = p->red_team[i].occupation;
		info.level = p->red_team[i].level;
		m_vecBattleFieldInfo.push_back(info);
	}
	for(i = 0; i < (int)p->blue_team.size(); i++ )
	{
		BattleFieldInfo info;
		ACString strName = ACString((const ACHAR*)p->blue_team[i].name.begin(), p->blue_team[i].name.size() / sizeof (ACHAR));
		info.strName = strName;
		info.occupaton = p->blue_team[i].occupation;
		info.level = p->blue_team[i].level;
		m_vecBattleFieldInfo.push_back(info);
	}
	m_iCurPageBattleField = 0;
	int iSize = m_vecBattleFieldInfo.size();
	if (iSize>0)
	{
		m_iMaxPageBattleField = (iSize-1)/12;
	}
	else
		m_iMaxPageBattleField = 0;
	RefreshBattleInfo();
	Show(true);
}

void CDlgBattleSong::UpdateBattleList(void *pData)
{
	BattleGetList_Re *p = (BattleGetList_Re*)pData;
	if( p->retcode != ERR_SUCCESS )
		return;

	int mapid;
	int k = 0;
	m_pLstSongBattle->ResetContent();
	for(UINT i = 0; i < (int)p->fields.size(); i++ )
	{
		ACString strLine = GetGameUIMan()->GetLine(p->fields[i].gs_id);
		mapid = p->fields[i].map_id;
		if( p->fields[i].gs_id == GetGameRun()->GetCurLine() )
		{
			m_pLstSongBattle->InsertString(k,strLine + GetStringFromTable(7388+mapid));
			m_pLstSongBattle->SetItemData(k, mapid, 0);
			m_pLstSongBattle->SetItemData(k, p->fields[i].gs_id, 1);
			k++;
		}
		else
		{
			m_pLstSongBattle->AddString(strLine + GetStringFromTable(7388+mapid));
			m_pLstSongBattle->SetItemData(m_pLstSongBattle->GetCount() - 1, mapid, 0);
			m_pLstSongBattle->SetItemData(k, p->fields[i].gs_id, 1);
		}
	}

	Show(true);
}

void CDlgBattleSong::UpdateSongSelfInfo(void *pData)
{
	S2C::self_sjbattle_info *p = (S2C::self_sjbattle_info*)pData;

	ACHAR szText[20];
	ACString strRank = GetHostPlayer()->GetMilitaryRankName(p->rank);
	a_sprintf(szText,_AL("%d"),p->battle_score);
	m_pLabPoint->SetText(szText);
	a_sprintf(szText,_AL("%s"),strRank);
	m_pLabType->SetText(szText);
	a_sprintf(szText,_AL("%d"),p->conKill);
	m_pLabKill->SetText(szText);
	a_sprintf(szText,_AL("%d"),p->postion);
	m_pLabRank->SetText(szText);
}

void CDlgBattleSong::UpdateSongBattleScore(void *pData)
{
	S2C::cmd_sjbattle_info *p = (S2C::cmd_sjbattle_info*)pData;

	int i;
	ACString strText;
	ACHAR szText[20];
	a_sprintf(szText,_AL("%d"),p->attacker_score);
	m_pLabChuPoint->SetText(szText);
	a_sprintf(szText,_AL("%d"),p->defender_score);
	m_pLabHanPoint->SetText(szText);
	a_sprintf(szText,_AL("%d"),p->defender_count);
	m_pLabHanNum->SetText(szText);
	a_sprintf(szText,_AL("%d"),p->attacker_count);
	m_pLabChuNum->SetText(szText);
	
	m_pLstScore->ResetContent();
	int iNumRed=0, iNumBlue=0;
	for(i = 0; i < (int)p->player_count; i++ )
	{
		const ACHAR *pszName = GetGameRun()->GetPlayerName(p->info[i].id, false);
		int id = p->info[i].id;
		ACString strRank = GetHostPlayer()->GetMilitaryRankName(p->info[i].rank);
		if( !pszName )
		{
			strText.Format(_AL(" \t%s\t%d\t%s\t%d"), GetStringFromTable(7380+p->info[i].faction), p->info[i].score,
				strRank, p->info[i].con_kill);
			GetGameSession()->GetPlayerBriefInfo(1, &p->info[i].id, 2);
		}
		else
			strText.Format(_AL("%s\t%s\t%d\t%s\t%d"), pszName, GetStringFromTable(7380+p->info[i].faction), p->info[i].score,
			strRank, p->info[i].con_kill);
		m_pLstScore->AddString(strText);

	}
	Show(true);
}

void CDlgBattleSong::RefreshBattleInfo()
{
	m_pLstSongBattle->ResetContent();
	ACString strText;
	BattleFieldInfo info;
	int iSize = m_vecBattleFieldInfo.size();
	for (int i=0; i<12; i++)
	{
		if (m_iCurPageBattleField*12+i>=iSize)
		{
			break;
		}
		info = m_vecBattleFieldInfo[m_iCurPageBattleField*12+i];
		strText.Format(_AL("%s\t%s\t%d"), info.strName, GetStringFromTable(PROFESSION_START+info.occupaton), info.level);
		m_pLstSongBattle->AddString(strText);
	}
	PAUILABEL pPage = (PAUILABEL)GetDlgItem("Lab_Page");
	strText.Format(GetStringFromTable(809), m_iCurPageBattleField + 1, m_iMaxPageBattleField+1);
	pPage->SetText(strText);
}