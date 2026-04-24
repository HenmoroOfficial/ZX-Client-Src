// File		: DlgRankList.cpp
// Creator	: Xiao Zhou
// Date		: 2006/8/14

#include "AUI\\CSplit.h"
#include "DlgRankList.h"
#include "DlgFriendSeeking.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "gettoptable_re.hpp"
#include "AWScriptFile.h"
#include "DlgCharacter.h"


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgRankList, CDlgBase)

AUI_ON_COMMAND("main*",		OnCommandButtonA)
AUI_ON_COMMAND("sub*",		OnCommandButtonB)
AUI_ON_COMMAND("last",		OnCommandLast)
AUI_ON_COMMAND("next",		OnCommandNext)
AUI_ON_COMMAND("head",		OnCommandHead)
AUI_ON_COMMAND("tail",		OnCommandTail)
AUI_ON_COMMAND("help",		OnCommandHelp)
AUI_ON_COMMAND("IDCANCEL",	OnCommandCancel)
AUI_ON_COMMAND("Btn_Character_Dungeon" ,	OnCommandSelf)

AUI_END_COMMAND_MAP()

CDlgRankList::CDlgRankList()
{
}

CDlgRankList::~CDlgRankList()
{
	m_RankDisp.Release();
	abase::hash_map<int, PageDesc*>::iterator it;
	for(it = m_mapTableContent.begin(); it != m_mapTableContent.end(); ++it )
	{
		it->second->clear();
		delete it->second;
	}
	m_mapTableContent.clear();
}

bool CDlgRankList::OnInitDialog()
{
	m_RankDisp.Init();
	m_pLst_All = (PAUILISTBOX)GetDlgItem("Lst_All");
	m_pTxt_Self = (PAUILABEL)GetDlgItem("Txt_Self");
	m_pLab_Self = (PAUILABEL)GetDlgItem("Lab_Self");
	m_pTxt_Page = (PAUILABEL)GetDlgItem("Txt_Page");
	m_pBtn_Desc1 = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Desc1");
	m_pBtn_Desc2 = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Desc2");
	m_pBtn_Self = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Character_Dungeon");
	char szName[20];
	int i;
	/*for(i = 0; i < CDLGRANKLIST_MAINTYPE_MAX; i++ )
	{
		sprintf(szName, "Btn_A%d", i);
		m_pBtn_A[i] = (PAUISTILLIMAGEBUTTON)GetDlgItem(szName);
	}
	*/
	for(i = 0; i < CDLGRANKLIST_MAINTYPE_MAX; i++ )
	{
		sprintf(szName, "Rdo_Rank%d", i);
		m_pRdo_A[i] = (PAUIRADIOBUTTON)GetDlgItem(szName);
	}
	for(i = 0; i < CDLGRANKLIST_SUBTYPE_MAX; i++ )
	{
		sprintf(szName, "Btn_B%d", i);
		m_pBtn_B[i] = (PAUISTILLIMAGEBUTTON)GetDlgItem(szName);
	}
	memset(m_idTable, 0, sizeof(m_idTable));
	return true;
}
void CDlgRankList::_LoadConfigInfo()
{
	AWScriptFile s;
	AString strName;
	if (GetHostPlayer()->GetSrcServerID()!=0)
	{
		strName = "configs\\toptable1.txt";
	}
	else
		strName = "configs\\toptable.txt";
	m_mapTableInfo.clear();
	if( s.Open(strName) )
	{
		int i = 0, j;
		while( s.PeekNextToken(true) )
		{
			int n = s.GetNextTokenAsInt(true);
			s.GetNextToken(true);
			//m_pBtn_A[i]->SetText(s.m_szToken);
			//m_pBtn_A[i]->Show(true);
			if(NULL != m_pRdo_A[i])
			{
			    m_pRdo_A[i]->SetText(s.m_szToken);
			    m_pRdo_A[i]->Show(true);
			}
			TableInfo info;
			for(j = 0; j < n; j++ )
			{
				int id = s.GetNextTokenAsInt(true);
				s.GetNextToken(true);
				info.strTableName = s.m_szToken;
				s.GetNextToken(true);
				info.strSelf = s.m_szToken;
				s.GetNextToken(true);
				info.strRankDesc1 = s.m_szToken;
				s.GetNextToken(true);
				info.strRankDesc2 = s.m_szToken;
				if( i < CDLGRANKLIST_MAINTYPE_MAX && j < CDLGRANKLIST_SUBTYPE_MAX )
				{
					info.nMaxPage = 0;
					info.nSelfRank = 0;
					info.nPageSize = 0;
					m_idTable[i][j] = id;
					m_mapTableInfo[id] = info;
				}
			}
			i++;
		}
		s.Close();
		for(j = i; j < CDLGRANKLIST_MAINTYPE_MAX; j++ )
		{
			//m_pBtn_A[j]->Show(false);
			if(m_pRdo_A[j])
			   m_pRdo_A[j]->Show(false);
		}
	}
	else
		AUI_ReportError(__LINE__, 1,"CDlgRankList::OnInitDialog(), toptable.txt or toptable1.txt error!");
}

void CDlgRankList::OnShowDialog()
{
	_LoadConfigInfo();
	m_pTxt_Page->Show(false);
	m_pTxt_Self->SetText(_AL(""));
	m_pLst_All->ResetContent();
	m_pBtn_Self->Show(false);
	m_idCurTable = -1;
	m_nPage = 0;
	int i;
	//for(i = 0; i < CDLGRANKLIST_MAINTYPE_MAX; i++ )
	//	m_pBtn_A[i]->SetPushed(false);
	for(i = 0; i < CDLGRANKLIST_MAINTYPE_MAX; i++ )
	{
		if(NULL != m_pRdo_A[i])
		   m_pRdo_A[i]->Check(false);
	}
	for(i = 0; i < CDLGRANKLIST_SUBTYPE_MAX; i++ )
		m_pBtn_B[i]->Show(false);
	m_pBtn_Desc1->SetText(_AL(""));
	m_pBtn_Desc2->SetText(_AL(""));
	for(i = 0; i < (int)m_mapTableInfo.size(); i++ )
	{
		m_mapTableInfo[i].nMaxPage = 0;
	}
	abase::hash_map<int, PageDesc*>::iterator it;
	for(it = m_mapTableContent.begin(); it != m_mapTableContent.end(); ++it )
	{
		it->second->clear();
		delete it->second;
	}
	m_mapTableContent.clear();
}

void CDlgRankList::OnCommandCancel(const char * szCommand)
{
	Show(false);
}

void CDlgRankList::OnCommandLast(const char * szCommand)
{
	if( m_idCurTable != -1 && m_nPage > 0 )
	{
		m_nPage--;
		Update(m_idCurTable, m_nPage);
	}
}

void CDlgRankList::OnCommandNext(const char * szCommand)
{
	if( m_idCurTable != -1 && m_nPage < m_mapTableInfo[m_idCurTable].nMaxPage - 1 )
	{
		m_nPage++;
		Update(m_idCurTable, m_nPage);
	}
}

void CDlgRankList::OnCommandHead(const char * szCommand)
{
	if( m_idCurTable != -1 && m_nPage > 0 )
	{
		m_nPage = 0;
		Update(m_idCurTable, m_nPage);
	}
}

void CDlgRankList::OnCommandTail(const char * szCommand)
{
	if( m_idCurTable != -1 && m_nPage < m_mapTableInfo[m_idCurTable].nMaxPage - 1 )
	{
		m_nPage = 0;
		Update(m_idCurTable, m_nPage);
		if( m_mapTableContent.find(m_idCurTable) != m_mapTableContent.end() )
		{
			m_nPage = m_mapTableInfo[m_idCurTable].nMaxPage - 1;
			Update(m_idCurTable, m_nPage);
		}
	}
}

void CDlgRankList::OnCommandHelp(const char * szCommand)
{
	//GetGameUIMan()->GetDialog("Win_RankList_Help")->Show(true);
	AString strDlg = "Win_RankList_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgRankList::OnCommandButtonA(const char * szCommand)
{
	int nSel = atoi(szCommand + strlen("main"));
	int i;
	//for(i = 0; i < CDLGRANKLIST_MAINTYPE_MAX; i++ )
		//m_pBtn_A[i]->SetPushed(i == nSel);
	for(i = 0; i < CDLGRANKLIST_MAINTYPE_MAX; i++ )
	{
		if(NULL != m_pRdo_A[i])
			m_pRdo_A[i]->Check(i == nSel);
	}
	
	m_idCurTable = -1;
	for(i = 0; i < CDLGRANKLIST_SUBTYPE_MAX; i++ )
		if( m_idTable[nSel][i] != 0 )
		{
			m_pBtn_B[i]->Show(true);
			char szCommand[20];
			sprintf(szCommand, "sub%d", m_idTable[nSel][i]);
			m_pBtn_B[i]->SetCommand(szCommand);
			m_pBtn_B[i]->SetText(m_mapTableInfo[m_idTable[nSel][i]].strTableName);
			m_pBtn_B[i]->SetPushed(false);
		}
		else
			m_pBtn_B[i]->Show(false);
}

void CDlgRankList::OnCommandButtonB(const char * szCommand)
{
	m_idCurTable = atoi(szCommand + strlen("sub"));
	m_nPage = 0;
	for(int i = 0; i < CDLGRANKLIST_SUBTYPE_MAX; i++ )
		m_pBtn_B[i]->SetPushed(strcmp(m_pBtn_B[i]->GetCommand(), szCommand) == 0);
	Update(m_idCurTable, m_nPage);
	if(m_idCurTable == 95)
	{
		m_pBtn_Self->Show(true);
	}
	else
	{
		m_pBtn_Self->Show(false);
	}
}

void CDlgRankList::Update(int nTable, int nPage)
{
	if( m_mapTableContent.find(nTable) == m_mapTableContent.end() )
	{
		PageDesc *pagedesc = new PageDesc;
		pagedesc->clear();
		m_mapTableContent[nTable] = pagedesc;
	}
	if( m_mapTableContent[nTable]->find(nPage) != m_mapTableContent[nTable]->end() )
	{
		ACString strText;
		m_pLst_All->ResetContent();
		ACString strPage = (*m_mapTableContent[nTable])[nPage];
		while( strPage.GetLength() != 0 )
		{
			int i = strPage.Find(_AL('\r'));
			AddStringToList(strPage.Left(i));
			strPage.CutLeft(i + 1);
		}
		m_pBtn_Desc1->SetText(m_mapTableInfo[nTable].strRankDesc1);
		m_pBtn_Desc2->SetText(m_mapTableInfo[nTable].strRankDesc2);
		m_pLab_Self->SetText(m_mapTableInfo[nTable].strSelf);
		strText.Format(_AL("%d"), m_mapTableInfo[nTable].nSelfRank);
		m_pTxt_Self->SetText(strText);
		strText.Format(GetStringFromTable(809), nPage + 1, m_mapTableInfo[nTable].nMaxPage);
		m_pTxt_Page->SetText(strText);
		m_pTxt_Page->Show(true);
	}
	else
		GetGameSession()->toptable_get(nTable, nPage, nPage == 0);
}

ACString CDlgRankList::GetCorresText(int nTable, int nPage)
{
	if( m_mapTableContent.find(nTable) == m_mapTableContent.end() )
	{
		PageDesc *pagedesc = new PageDesc;
		pagedesc->clear();
		m_mapTableContent[nTable] = pagedesc;
	}
	ACString strPage;
	if( m_mapTableContent[nTable]->find(nPage) != m_mapTableContent[nTable]->end() )
	{
		strPage = (*m_mapTableContent[nTable])[nPage];
		return strPage;
	}
	else
		GetGameSession()->toptable_get(nTable, nPage, nPage == 0);
	return strPage;
}

void CDlgRankList::RankListAction(void *pData)
{
	GetTopTable_Re *p = (GetTopTable_Re*)pData;
	DWORD i;
	m_pLst_All->ResetContent();
	ACString strTemp;
	if( p->pageid == 0 )
	{
		m_mapTableInfo[p->tableid].nPageSize = p->items.size();
		m_mapTableInfo[p->tableid].nMaxPage = (p->maxsize - 1) / 20 + 1;
		m_mapTableInfo[p->tableid].nSelfRank = p->selfrank;
	}
	ACString strPage = _AL("");
	ACString strOldRank;
	for( i = 0 ; i < p->items.size(); i++ )
	{
		int nCurRank = p->pageid * m_mapTableInfo[p->tableid].nPageSize + i + 1;
		ACString strText;
		ACString strName = ACString((const ACHAR*)p->items[i].name.begin(), p->items[i].name.size() / sizeof(ACHAR));
		if( strName[0] == _AL('^') )
			strName = _AL('^') + strName;
		int tabid = p->tableid;
		int oldRank = p->items[i].oldrank;
		int oldOccu = p->items[i].occupation;
//		int oldvalue = p->items[i].value;
		int occu = p->items[i].occupation;
		int64_t v = p->items[i].value;
		int low = v & 0xffffffff;	//爬塔用时
		int high = v >> 32;			//爬塔层数
		
		if(p->tableid == 95)		//爬塔副本
		{
			m_RankDisp.GetDisp(p->tableid, nCurRank, p->items[i].oldrank, 
				strName,p->items[i].occupation, high + 1,low,
				strText);
		}
		else
		{
			m_RankDisp.GetDisp(p->tableid, nCurRank, p->items[i].oldrank, 
				strName,
				p->items[i].occupation,
				p->items[i].value, 0 ,
			strText);
		}
		AddStringToList(strText);
		strPage += strText;
		strPage += _AL('\r');
//		A3DCOLOR color = A3DCOLORRGB(255, 255, 255);
//		if( p->items[i].oldrank < 0 )
//		{
//			strOldRank = GetStringFromTable(788);
//			color = A3DCOLORRGB(0, 255, 0);
//		}
//		else
//		{
//			if( p->items[i].oldrank == nCurRank )
//				strOldRank = _AL("--");
//			else if( p->items[i].oldrank > nCurRank )
//			{
//				strOldRank.Format(_AL("↑%d"), p->items[i].oldrank - nCurRank);
//				color = A3DCOLORRGB(0, 255, 0);
//			}
//			else
//			{
//				strOldRank.Format(_AL("↓%d"), nCurRank - p->items[i].oldrank);
//				color = A3DCOLORRGB(255, 255, 0);
//			}
//		}
//		
//		a_sprintf(szText, _AL("%d\t%s\t%s\t%s\t%I64d"), 
//			nCurRank,
//			strOldRank, 
//			ACString((const ACHAR*)p->items[i].name.begin(), p->items[i].name.size() / sizeof(ACHAR)), 
//			GetStringFromTable(101 + p->items[i].occupation),
//			p->items[i].value);
//		m_pLst_All->AddString(szText);
//		m_pLst_All->SetItemTextColor(i, color, 1);
//		strPage += szText;
//		strPage += _AL('\r');
	}
	
	if( m_mapTableContent.find(p->tableid) == m_mapTableContent.end() )
	{
		PageDesc *pagedesc = new PageDesc;
		pagedesc->clear();
		m_mapTableContent[p->tableid] = pagedesc;
	}
	(*m_mapTableContent[p->tableid])[p->pageid] = strPage;
	if (p->tableid>=80 && p->tableid<=83)
	{
		((CDlgFriendSeek*)GetGameUIMan()->GetDialog("Win_FriendSeekingPerCharm"))->AddCharmRank();
		return;
	}
	m_pBtn_Desc1->SetText(m_mapTableInfo[p->tableid].strRankDesc1);
	m_pBtn_Desc2->SetText(m_mapTableInfo[p->tableid].strRankDesc2);
	m_pLab_Self->SetText(m_mapTableInfo[p->tableid].strSelf);

	if( p->pageid == 0 )
		strTemp.Format(_AL("%d"), p->selfrank);
	else
		strTemp.Format(_AL("%d"), m_mapTableInfo[p->tableid].nSelfRank);
	m_pTxt_Self->SetText(strTemp);
	strTemp.Format(GetStringFromTable(809), 
		p->pageid + 1, m_mapTableInfo[p->tableid].nMaxPage);
	m_pTxt_Page->SetText(strTemp);
	m_pTxt_Page->Show(true);
}

void CDlgRankList::AddStringToList(const ACHAR* szText)
{
	CSplit s(szText);
	CSplit::VectorAWString vec = s.Split(_AL("\t"));
	ACString strText = _AL("");
	A3DCOLOR color[6];
	DWORD i;
	for(i = 0; i < vec.size(); i++ )
	{
		if( vec[i].GetLength() > 1 && vec[i][0] == _AL('^') )
		{
			if( vec[i][1] ==  _AL('^') )
			{
				vec[i].CutLeft(1);
				color[i] = 0xFFFFFFFF;
			}
			else if( vec[i].GetLength() > 7 )
			{
				color[i] = 0;
				for(int j = 1; j < 7; j++ )
					if( vec[i][j] >= '0' && vec[i][j] <= '9' )
						color[i] = color[i] * 16 + vec[i][j] - '0';
					else if( vec[i][j] >= 'A' && vec[i][j] <= 'F' )
						color[i] = color[i] * 16 + vec[i][j] - 'A' + 10;
					else if( vec[i][j] >= 'a' && vec[i][j] <= 'f' )
						color[i] = color[i] * 16 + vec[i][j] - 'f' + 10;
				color[i] = color[i] | 0xFF000000;
				vec[i].CutLeft(7);
				if( vec[i].GetLength() > 1 && vec[i][0] ==  _AL('^'))
					vec[i].CutLeft(1);
			}
		}
		else
			color[i] = 0xFFFFFFFF;
		strText += vec[i] + _AL('\t');
	}
	m_pLst_All->AddString(strText);
	for(i = 0; i < 6; i++ )
		m_pLst_All->SetItemTextColor(m_pLst_All->GetCount() - 1, color[i], i);
}

void CDlgRankList::ShowTowerRank()
{
	OnCommandButtonA("main0");
	OnCommandButtonB("sub95");
}

void CDlgRankList::OnCommandSelf(const char * szCommand)
{
	if(m_idCurTable != 95)
	{
		return;
	}
	AUIDialog* pDlg = GetGameUIMan()->GetDialog("Win_CharDungeon");
	pDlg->Show(true);
}