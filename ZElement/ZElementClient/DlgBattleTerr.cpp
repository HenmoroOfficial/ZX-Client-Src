// File		: DlgBattleTerri.cpp
// Creator	: Fuchonggang
// Date		: 2010/03/10

#include "AUI\\AUIListBox.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIComboBox.h"
#include "DlgBattleTerr.h"
#include "DlgTerrWarMap.h"
#include "DlgHost.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_World.h"
#include "EC_GPDataType.h"
#include "EC_Algorithm.h"
#include "battlegetfield_re.hpp"
#include "battlegetlist_re.hpp"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_GameSession.h"
#include "EC_Faction.h"
#include "globaldataman.h"
#include "EC_Global.h"
#include "EC_UIManager.h"

#define new A_DEBUG_NEW

int CDlgBattleTerr::m_nTerrIndex = 0;
int CDlgBattleTerr::m_nTerrId = 0;
int CDlgBattleTerr::m_nItemId = 0;
int CDlgBattleTerr::m_nItemMin = 0;
int CDlgBattleTerr::m_nItemMax = 0;
int CDlgBattleTerr::m_nItemCur = 0;

AUI_BEGIN_COMMAND_MAP(CDlgBattleTerr, CDlgBase)

AUI_ON_COMMAND("Btn_Refresh",		OnCommandRefresh)
AUI_ON_COMMAND("Btn_Declare",		OnCommandDeclare)
AUI_ON_COMMAND("Btn_Add",			OnCommandAdd)
AUI_ON_COMMAND("Btn_Max",			OnCommandAddMax)
AUI_ON_COMMAND("Btn_AddConfirm",	OnCommandAddConfirm)
AUI_ON_COMMAND("Btn_Get",			OnCommandGet)
AUI_ON_COMMAND("Btn_Guild1",		OnCommandBattleJoin1)
AUI_ON_COMMAND("Btn_Guild2",		OnCommandBattleJoin2)
AUI_ON_COMMAND("Btn_Quit",			OnCommandBattleLeave)
AUI_ON_COMMAND("Btn_Close",			OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBattleTerr, CDlgBase)

AUI_END_EVENT_MAP()

CDlgBattleTerr::CDlgBattleTerr()
{
}

CDlgBattleTerr::~CDlgBattleTerr()
{
}

bool CDlgBattleTerr::OnInitDialog()
{
	if (m_szName == "Win_BattleListPK")
	{
	}
	else if (m_szName == "Win_BattleListPKAwait")
	{
	}
	else if (m_szName == "Win_BattleScorePK")
	{
	}
	return true;
}

void CDlgBattleTerr::OnShowDialog()
{
	ACString strText;
	vector<DOMAIN_INFO> * domainInfo = globaldata_getdomaininfos2();
	UINT nSize = (*domainInfo).size();
	if (m_szName == "Win_Battle_TerriWarAdd")
	{
		UINT index = GetData();
		DOMAIN_INFO di = (*domainInfo)[index];
		m_nTerrIndex = index;
		m_nTerrId = di.id;
		m_nItemId = di.challenge_itemid;
		m_nItemMin = di.item_lowerlimit;
		m_nItemMax = di.item_upperlimit;
		m_nItemCur = di.deposit;

		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Now");
		strText.Format(_AL("%d"), m_nItemCur);
		pLab->SetText(strText);
		pLab = (PAUILABEL)GetDlgItem("Txt_Name");
		pLab->SetText(di.name);
		pLab = (PAUILABEL)GetDlgItem("Txt_Lv");
		pLab->SetText(GetGameUIMan()->GetStringFromTable(9333+di.type));
		pLab = (PAUILABEL)GetDlgItem("Txt_Guild");
		Faction_Info* fInfoOwn =  GetGame()->GetFactionMan()->GetFaction(di.id_owner);
		pLab->SetText((di.id_owner>0 && fInfoOwn) ? fInfoOwn->GetName() : GetGameUIMan()->GetStringFromTable(780));
	}
	else if (m_szName == "Win_Battle_TerriDeclare")
	{
		UINT index = GetData();
		DOMAIN_INFO di = (*domainInfo)[index];
		m_nTerrIndex = index;
		m_nTerrId = di.id;
		m_nItemId = di.challenge_itemid;
		m_nItemMin = di.item_lowerlimit;
		m_nItemMax = di.item_upperlimit;
		m_nItemCur = di.deposit;

		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Name");
		pLab->SetText(di.name);
		pLab = (PAUILABEL)GetDlgItem("Txt_Lv");
		pLab->SetText(GetGameUIMan()->GetStringFromTable(9333+di.type));
		pLab = (PAUILABEL)GetDlgItem("Txt_Guild");
		Faction_Info* fInfoOwn =  GetGame()->GetFactionMan()->GetFaction(di.id_owner);
		pLab->SetText((di.id_owner>0 && fInfoOwn) ? fInfoOwn->GetName() : GetGameUIMan()->GetStringFromTable(780));		
	}
	else if (m_szName == "Win_Battle_TerriHire")
	{
		int DrawnNum = 0;
		int Num = 0;
	
		int idFaction = GetHostPlayer()->GetFactionID();
		vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
		int i;
		for(i = 0; i < (int)info->size(); i++ )
		{
			DOMAIN_INFO di = (*info)[i];
			if (idFaction==di.id_defender)
			{
				Num += di.assis_num;
				DrawnNum += di.assis_drawn_num;
			}
		}
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Got");
		strText.Format(_AL("%d"), DrawnNum);
		pLab->SetText(strText);
		pLab = (PAUILABEL)GetDlgItem("Txt_Available");
		strText.Format(_AL("%d"), Num - DrawnNum);
		pLab->SetText(strText);
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Num");
		pCombo->ResetContent();
		for (i=1; i<=(Num - DrawnNum); i++)
		{
			strText.Format(_AL("%d"), i);
			pCombo->AddString(strText);
		}
		pCombo->SetCurSel(0);
	}
	else if (m_szName == "Win_Battle_TerriWar")
	{
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Min");
		strText.Format(_AL("%d"), m_nItemMin);
		pLab->SetText(strText);
		pLab = (PAUILABEL)GetDlgItem("Txt_Max");
		strText.Format(_AL("%d"), m_nItemMax);
		pLab->SetText(strText);
		PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Edt_Num");
		strText.Format(_AL("%d"), m_nItemMin);
		pEdit->SetText(strText);
		
		DOMAIN_INFO di = (*domainInfo)[m_nTerrIndex];
		pLab = (PAUILABEL)GetDlgItem("Txt_Name");
		pLab->SetText(di.name);
		pLab = (PAUILABEL)GetDlgItem("Txt_Lv");
		pLab->SetText(GetGameUIMan()->GetStringFromTable(9333+di.type));
		pLab = (PAUILABEL)GetDlgItem("Txt_Guild");
		Faction_Info* fInfoOwn =  GetGame()->GetFactionMan()->GetFaction(di.id_owner);
		pLab->SetText((di.id_owner>0 && fInfoOwn) ? fInfoOwn->GetName() : GetGameUIMan()->GetStringFromTable(780));
	}
	else if (m_szName == "Win_Battle_TerriAddNum")
	{
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Min");
		strText.Format(_AL("%d"), m_nItemMin);
		pLab->SetText(strText);
		pLab = (PAUILABEL)GetDlgItem("Txt_Max");
		strText.Format(_AL("%d"), m_nItemMax);
		pLab->SetText(strText);
		pLab = (PAUILABEL)GetDlgItem("Txt_Cur");
		strText.Format(_AL("%d"), m_nItemCur);
		pLab->SetText(strText);
		PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Edt_Num");
		pEdit->SetText(_AL("1"));

		DOMAIN_INFO di = (*domainInfo)[m_nTerrIndex];
		pLab = (PAUILABEL)GetDlgItem("Txt_Name");
		pLab->SetText(di.name);
		pLab = (PAUILABEL)GetDlgItem("Txt_Lv");
		pLab->SetText(GetGameUIMan()->GetStringFromTable(9333+di.type));
		pLab = (PAUILABEL)GetDlgItem("Txt_Guild");
		Faction_Info* fInfoOwn =  GetGame()->GetFactionMan()->GetFaction(di.id_owner);
		pLab->SetText((di.id_owner>0 && fInfoOwn) ? fInfoOwn->GetName() : GetGameUIMan()->GetStringFromTable(780));
	}
	else if (m_szName == "Win_Battle_TerriWarArea")
	{
		PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("List_Member");
		pLst->ResetContent();
		int iFactionId = GetHostPlayer()->GetFactionID();
		
		for (UINT i=0; i<nSize; i++)
		{
			DOMAIN_INFO info = (*domainInfo)[i];
			if (info.id_defender == iFactionId && iFactionId > 0)
			{
				strText.Format(_AL("%s\t%s\t%s\t%d/%d"), info.name, GetGameUIMan()->GetStringFromTable(9333+info.type), 
					GetGameUIMan()->GetStringFromTable(9362+info.flag), info.defender_num, info.player_limit);
				pLst->AddString(strText);
			}
			else if (info.id_attacker == iFactionId && iFactionId > 0)
			{
				strText.Format(_AL("%s\t%s\t%s\t%d/%d"), info.name, GetGameUIMan()->GetStringFromTable(9333+info.type), 
					GetGameUIMan()->GetStringFromTable(9366+info.flag), info.attacker_num, info.player_limit);
				pLst->AddString(strText);
			}
			else if (GetHostPlayer()->GetTerritoryBattleInfo()>0 && GetHostPlayer()->GetTerritoryBattleInfo()==info.id)
			{
				strText.Format(_AL("%s\t%s\t%s\t%d/%d"), info.name, GetGameUIMan()->GetStringFromTable(9333+info.type), 
					GetGameUIMan()->GetStringFromTable(9358), GetHostPlayer()->GetBattleCamp()==GP_BATTLE_CAMP_INVADER ? info.attacker_num : info.defender_num, info.player_limit);
				pLst->AddString(strText);
			}
		}
		
		GetDlgItem("Btn_Quit")->Show(GetHostPlayer()->GetTerritoryBattleInfo()>=0);
	}
	else if (m_szName == "Win_Battle_TerriAward")
	{
		int idFaction = GetHostPlayer()->GetFactionID();
		PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_All");
		pLst->ResetContent();
		ACString strText, strMoney;
		for (UINT i=0; i<nSize; i++)
		{
			DOMAIN_INFO di = (*domainInfo)[i];
			if (di.id_owner==idFaction && di.deposit>0)
			{
				int item_count;
				int money;
				int idType;
				if (di.id_owner==di.id_attacker)
				{
					item_count = di.deposit*4/5;
					money = ((di.deposit*4)%5)*10000000/5;
					idType = 9337;
				}
				else
				{
					item_count = di.deposit/2;
					money = (di.deposit%2)*10000000/2;
					idType = 9338;
				}
				GetGameUIMan()->FormatMoney(money, strMoney);
				strText.Format(_AL("%s\t%s\t%d\t%s"), di.name, GetGameUIMan()->GetStringFromTable(idType), item_count, strMoney);
				pLst->AddString(strText);
				
				pLst->SetItemData(pLst->GetCount()-1, di.id, 0);
				pLst->SetItemData(pLst->GetCount()-1, di.challenge_itemid, 1);
				pLst->SetItemData(pLst->GetCount()-1, item_count, 2);
				pLst->SetItemData(pLst->GetCount()-1, money, 3);
			}
		}
	}
}

void CDlgBattleTerr::OnTick()
{
	if (m_szName == "Win_Battle_TerriWar")
	{
		PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Edt_Num");
		if (pEdit->GetText()!=_AL(""))
		{
			ACString strText;
			int nNum = a_atoi(pEdit->GetText());
			if (nNum>m_nItemMax)
			{
				strText.Format(_AL("%d"), m_nItemMax);
				pEdit->SetText(strText);
			}
		}
	}
	if (m_szName == "Win_Battle_TerriAddNum")
	{
		PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Edt_Num");
		if (pEdit->GetText()!=_AL(""))
		{
			ACString strText;
			int nNum = a_atoi(pEdit->GetText());
			if (nNum>(m_nItemMax-m_nItemCur))
			{
				strText.Format(_AL("%d"), m_nItemMax-m_nItemCur);
				pEdit->SetText(strText);
			}
		}
	}
}

void CDlgBattleTerr::OnCommandDeclare(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Battle_TerriWar")->Show(true);
}

void CDlgBattleTerr::OnCommandAdd(const char * szCommand)
{
	vector<DOMAIN_INFO> * domainInfo = globaldata_getdomaininfos2();
	UINT index = GetData();
	m_nTerrId = (*domainInfo)[index].id;
	m_nItemId = (*domainInfo)[index].challenge_itemid;
	m_nItemMin = (*domainInfo)[index].item_lowerlimit;
	m_nItemMax = (*domainInfo)[index].item_upperlimit;
	m_nItemCur = (*domainInfo)[index].deposit;
	Show(false);
	GetGameUIMan()->GetDialog("Win_Battle_TerriAddNum")->Show(true);
}

void CDlgBattleTerr::OnCommandAddMax(const char * szCommand)
{
	ACString strText;
	PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Edt_Num");
	int iMax = GetHostPlayer()->GetPack()->GetItemTotalNum(m_nItemId);
	if (iMax>m_nItemMin)
	{
		if (m_szName == "Win_Battle_TerriAddNum" && iMax>(m_nItemMax-m_nItemCur))
		{
			strText.Format(_AL("%d"), m_nItemMax-m_nItemCur);
			pEdit->SetText(strText);
		}
		else if (m_szName == "Win_Battle_TerriWar" && iMax>m_nItemMax)
		{
			strText.Format(_AL("%d"), m_nItemMax);
			pEdit->SetText(strText);
		}
	}
}

void CDlgBattleTerr::OnCommandAddConfirm(const char * szCommand)
{
	PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Edt_Num");
	int nNum = 0;
	if (pEdit->GetText()!=_AL(""))
	{
		nNum += a_atoi(pEdit->GetText());
	}
	if (m_nItemCur==m_nItemMax)
		GetGameUIMan()->MessageBox(9347);
	else if (nNum<m_nItemMin && m_szName=="Win_Battle_TerriWar")
		GetGameUIMan()->MessageBox(9357);
	else if (GetHostPlayer()->GetPack()->GetItemTotalNum(m_nItemId)>=nNum)
	{
		GetGameSession()->c2s_CmdNPCSevTerritoryChallenge(GetHostPlayer()->GetFactionID(), m_nTerrId, m_nItemId, nNum);
		Show(false);
	}
	else
		GetGameUIMan()->MessageBox(9345);
}

void CDlgBattleTerr::OnCommandGet(const char * szCommand)
{
	if (m_szName == "Win_Battle_TerriHire")
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Num");
		GetGameSession()->c2s_CmdNPCSevTerritoryAward(GetHostPlayer()->GetFactionID(), 0, 1, 34129, pCombo->GetCurSel()+1, 0);
	}
	else if (m_szName == "Win_Battle_TerriAward")
	{
		int idFaction = GetHostPlayer()->GetFactionID();
		PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_All");
		int money = 0;
		for (int i=0; i<pLst->GetCount(); i++)
		{
			money += pLst->GetItemData(i,3);
		}
		if (GetHostPlayer()->GetPack()->SearchEmpty()<0)
		{
			GetGameUIMan()->MessageBox(704);
		}
		else if ((money+GetHostPlayer()->GetMoneyAmount())>2000000000)
		{
			GetGameUIMan()->MessageBox(704);
		}
		else
		{
			int itemCount = 0;
			int moneyCount = 0;
			for (int i=0; i<pLst->GetCount(); i++)
			{
				itemCount += pLst->GetItemData(i,2);
				moneyCount += pLst->GetItemData(i,3);
			}
			GetGameSession()->c2s_CmdNPCSevTerritoryAward(idFaction, 0, 3, pLst->GetItemData(0,1), itemCount, moneyCount);
			Show(false);
		}
	}
}

void CDlgBattleTerr::OnCommandBattleJoin1(const char * szCommand)
{
	GetGameSession()->c2s_CmdNPCSevTerritoryEnter(GetDlgItem("Btn_Guild1")->GetData(), GetData(), 1);
}

void CDlgBattleTerr::OnCommandBattleJoin2(const char * szCommand)
{
	GetGameSession()->c2s_CmdNPCSevTerritoryEnter(GetDlgItem("Btn_Guild2")->GetData(), GetData(), 1);
}

void CDlgBattleTerr::OnCommandBattleLeave(const char * szCommand)
{
	PAUIDIALOG pDlg;
	GetGameUIMan()->MessageBox("Game_BattleTerrLeave", GetGameUIMan()->GetStringFromTable(9339), MB_YESNO, A3DCOLORRGBA(255,255,255,166), &pDlg);
}

void CDlgBattleTerr::OnCommandCancel(const char * szCommand)
{
	GetGameUIMan()->EndNPCService();
	Show(false);
}

void CDlgBattleTerr::OnCommandRefresh(const char * szCommand)
{
	GetGameSession()->territory_get_map();
}

DWORD CDlgBattleTerrInfo::m_dwRefreshTime = 0;

AUI_BEGIN_COMMAND_MAP(CDlgBattleTerrInfo, CDlgBase)

AUI_ON_COMMAND("Btn_Refresh",		OnCommandRefresh)
AUI_ON_COMMAND("Btn_Top",			OnCommandHome)
AUI_ON_COMMAND("Btn_Bottom",		OnCommandEnd)
AUI_ON_COMMAND("Btn_Next",			OnCommandNext)
AUI_ON_COMMAND("Btn_Prec",			OnCommandLast)

AUI_ON_COMMAND("Btn_TerriDetail",		OnCommandTerriDetail)
AUI_ON_COMMAND("Btn_TerriDetailGuild",	OnCommandGuildDetail)
AUI_ON_COMMAND("Btn_Name",			OnCommandSortName)
AUI_ON_COMMAND("Btn_Lv",			OnCommandSortLevel)
AUI_ON_COMMAND("Btn_Status",		OnCommandSortStatus)
AUI_ON_COMMAND("Btn_WarTime",		OnCommandSortWarTime)
AUI_ON_COMMAND("Btn_Defender",		OnCommandSortDefencer)
AUI_ON_COMMAND("Btn_Attacker",		OnCommandSortAttacker)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBattleTerrInfo, CDlgBase)

AUI_END_EVENT_MAP()

const int terrscorelevel[] =
{
	0,
		10,
		20,
		40,
		70,
		100,
		130,
		160,
		190,
		230,
		270,
		310,
		350,
		390,
		760,
		INT_MAX
};

CDlgBattleTerrInfo::CDlgBattleTerrInfo()
{
	m_pLst = NULL;
	m_pPro_GuildTerrScore = NULL;
	m_pTxt_GuildTerrScore = NULL;
	m_pTxt_1 = NULL;
	m_pTxt_2 = NULL;
	m_pTxt_3 = NULL;
}

CDlgBattleTerrInfo::~CDlgBattleTerrInfo()
{
}

bool CDlgBattleTerrInfo::OnInitDialog()
{
	DDX_Control("List_Member",		m_pLst);
	if (m_szName == "Win_Battle_TerriDetailGuild")
	{
		DDX_Control("Pro_LandOccupy",	m_pPro_GuildTerrScore);
		DDX_Control("Txt_LandOccupy",	m_pTxt_GuildTerrScore);
		DDX_Control("Txt_1",			m_pTxt_1);
		DDX_Control("Txt_2",			m_pTxt_2);
		DDX_Control("Txt_3",			m_pTxt_3);
	}
	m_dwRefreshTime = 0;
	return true;
}

const int lineperpage = 20;

void CDlgBattleTerrInfo::OnShowDialog()
{
	UpdateInfo();
}

void CDlgBattleTerrInfo::OnTick()
{
	GetDlgItem("Btn_Refresh")->Enable(GetTickCount()-m_dwRefreshTime>3000);
}

void CDlgBattleTerrInfo::OnCommandRefresh(const char * szCommand)
{
	m_dwRefreshTime = GetTickCount();
	GetGameSession()->territory_get_map();
}

void CDlgBattleTerrInfo::OnCommandHome(const char * szCommand)
{
	m_nPageCur = 0;
	UpdateList();
}

void CDlgBattleTerrInfo::OnCommandEnd(const char * szCommand)
{
	m_nPageCur = m_nPageTotal-1;
	UpdateList();
}

void CDlgBattleTerrInfo::OnCommandNext(const char * szCommand)
{
	m_nPageCur++;
	if (m_nPageCur>=m_nPageTotal)
		m_nPageCur = m_nPageTotal-1;
	UpdateList();
}

void CDlgBattleTerrInfo::OnCommandLast(const char * szCommand)
{
	m_nPageCur--;
	if (m_nPageCur<0)
		m_nPageCur = 0;
	UpdateList();
}
	
void CDlgBattleTerrInfo::OnCommandTerriDetail(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Battle_TerriDetail")->Show(true);
}

void CDlgBattleTerrInfo::OnCommandGuildDetail(const char * szCommand)
{
	int idFaction = GetHostPlayer()->GetFactionID();
	if (idFaction>0)
	{
		Show(false);
		GetGameUIMan()->GetDialog("Win_Battle_TerriDetailGuild")->Show(true);
	}
	else
		GetGameUIMan()->MessageBox(7349);
}

class CompareBattleTerrShowOrderByNameAscent
{
public:
	bool operator ()(CDlgBattleTerrInfo::BattleTerrInfo & p1, CDlgBattleTerrInfo::BattleTerrInfo & p2)
	{
		return p1.nId > p2.nId;
	}
};
class CompareBattleTerrShowOrderByNameDescent
{
public:
	bool operator ()(CDlgBattleTerrInfo::BattleTerrInfo & p1, CDlgBattleTerrInfo::BattleTerrInfo & p2)
	{
		return p1.nId < p2.nId;
	}
};

void CDlgBattleTerrInfo::OnCommandSortName(const char * szCommand)
{
	static int bAscent = true;
	if (bAscent)
		BubbleSort(m_vecInfo.begin(), m_vecInfo.end(), CompareBattleTerrShowOrderByNameAscent());
	else
		BubbleSort(m_vecInfo.begin(), m_vecInfo.end(), CompareBattleTerrShowOrderByNameDescent());
	bAscent = !bAscent;
	UpdateList();
}

class CompareBattleTerrShowOrderByLevelAscent
{
public:
	bool operator ()(CDlgBattleTerrInfo::BattleTerrInfo & p1, CDlgBattleTerrInfo::BattleTerrInfo & p2)
	{
		return p1.nLevel > p2.nLevel;
	}
};
class CompareBattleTerrShowOrderByLevelDescent
{
public:
	bool operator ()(CDlgBattleTerrInfo::BattleTerrInfo & p1, CDlgBattleTerrInfo::BattleTerrInfo & p2)
	{
		return p1.nLevel < p2.nLevel;
	}
};

void CDlgBattleTerrInfo::OnCommandSortLevel(const char * szCommand)
{
	static int bAscent = true;
	if (bAscent)
		BubbleSort(m_vecInfo.begin(), m_vecInfo.end(), CompareBattleTerrShowOrderByLevelAscent());
	else
		BubbleSort(m_vecInfo.begin(), m_vecInfo.end(), CompareBattleTerrShowOrderByLevelDescent());
	bAscent = !bAscent;
	UpdateList();
}

//根据是否有战斗在进行
class CompareBattleTerrShowOrderByStatusAscent
{
public:
	bool operator ()(CDlgBattleTerrInfo::BattleTerrInfo & p1, CDlgBattleTerrInfo::BattleTerrInfo & p2)
	{
		return p1.nState > p2.nState;
	}
};
class CompareBattleTerrShowOrderByStatusDescent
{
public:
	bool operator ()(CDlgBattleTerrInfo::BattleTerrInfo & p1, CDlgBattleTerrInfo::BattleTerrInfo & p2)
	{
		return p1.nState < p2.nState;
	}
};

void CDlgBattleTerrInfo::OnCommandSortStatus(const char * szCommand)
{
	static int bAscent = true;
	if (bAscent)
		BubbleSort(m_vecInfo.begin(), m_vecInfo.end(), CompareBattleTerrShowOrderByStatusAscent());
	else
		BubbleSort(m_vecInfo.begin(), m_vecInfo.end(), CompareBattleTerrShowOrderByStatusDescent());
	bAscent = !bAscent;
	UpdateList();
}

class CompareBattleTerrShowOrderByTimeAscent
{
public:
	bool operator ()(CDlgBattleTerrInfo::BattleTerrInfo & p1, CDlgBattleTerrInfo::BattleTerrInfo & p2)
	{
		return p1.nTimeStart > p2.nTimeStart;
	}
};
class CompareBattleTerrShowOrderByTimeDescent
{
public:
	bool operator ()(CDlgBattleTerrInfo::BattleTerrInfo & p1, CDlgBattleTerrInfo::BattleTerrInfo & p2)
	{
		return p1.nTimeStart < p2.nTimeStart;
	}
};

void CDlgBattleTerrInfo::OnCommandSortWarTime(const char * szCommand)
{
	CDlgTerrWarMap* pDlgTerrMap = (CDlgTerrWarMap*)GetGameUIMan()->GetDialog("WorldMap_TerriSub");
	int iStatus = pDlgTerrMap->GetStatus();
	if ((iStatus & (CDlgTerrWarMap::ST_ASSISGET|CDlgTerrWarMap::ST_FIGHT))==0)
	{
		return;
	}
	static int bAscent = true;
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
	if (bAscent)
		BubbleSort(m_vecInfo.begin(), m_vecInfo.end(), CompareBattleTerrShowOrderByTimeAscent());
	else
		BubbleSort(m_vecInfo.begin(), m_vecInfo.end(), CompareBattleTerrShowOrderByTimeDescent());
	bAscent = !bAscent;
	UpdateList();
}

class CompareBattleTerrShowOrderByDefencerAscent
{
public:
	bool operator ()(CDlgBattleTerrInfo::BattleTerrInfo & p1, CDlgBattleTerrInfo::BattleTerrInfo & p2)
	{
		return p1.nDefencer > p2.nDefencer;
	}
};
class CompareBattleTerrShowOrderByDefencerDescent
{
public:
	bool operator ()(CDlgBattleTerrInfo::BattleTerrInfo & p1, CDlgBattleTerrInfo::BattleTerrInfo & p2)
	{
		return p1.nDefencer < p2.nDefencer;
	}
};

void CDlgBattleTerrInfo::OnCommandSortDefencer(const char * szCommand)
{
	static int bAscent = true;
	if (bAscent)
		BubbleSort(m_vecInfo.begin(), m_vecInfo.end(), CompareBattleTerrShowOrderByDefencerAscent());
	else
		BubbleSort(m_vecInfo.begin(), m_vecInfo.end(), CompareBattleTerrShowOrderByDefencerDescent());
	bAscent = !bAscent;
	UpdateList();
}

class CompareBattleTerrShowOrderByAttackerAscent
{
public:
	bool operator ()(CDlgBattleTerrInfo::BattleTerrInfo & p1, CDlgBattleTerrInfo::BattleTerrInfo & p2)
	{
		return p1.nAttacker > p2.nAttacker;
	}
};
class CompareBattleTerrShowOrderByAttackerDescent
{
public:
	bool operator ()(CDlgBattleTerrInfo::BattleTerrInfo & p1, CDlgBattleTerrInfo::BattleTerrInfo & p2)
	{
		return p1.nAttacker < p2.nAttacker;
	}
};

void CDlgBattleTerrInfo::OnCommandSortAttacker(const char * szCommand)
{
	static int bAscent = true;
	if (bAscent)
		BubbleSort(m_vecInfo.begin(), m_vecInfo.end(), CompareBattleTerrShowOrderByAttackerAscent());
	else
		BubbleSort(m_vecInfo.begin(), m_vecInfo.end(), CompareBattleTerrShowOrderByAttackerDescent());
	bAscent = !bAscent;
	UpdateList();
}

void CDlgBattleTerrInfo::UpdateInfo()
{
	vector<DOMAIN_INFO> *info = globaldata_getdomaininfos2();
	m_vecInfo.clear();
	if (m_szName == "Win_Battle_TerriDetail")
	{
		m_nPageCur = 0;
		m_nPageTotal = ((*info).size()+lineperpage-1)/lineperpage;
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_TerriDetail"))->SetPushed(true);
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_TerriDetailGuild"))->SetPushed(false);
		CDlgTerrWarMap* pDlgTerrMap = (CDlgTerrWarMap*)GetGameUIMan()->GetDialog("WorldMap_TerriSub");
		int iStatus = pDlgTerrMap->GetStatus();
		for (UINT i=0; i<(*info).size(); i++)
		{
			DOMAIN_INFO di = (*info)[i];
			BattleTerrInfo info;
			info.nId		= di.id;
			info.strName	= di.name;
			info.nLevel		= di.type;
			info.nState		= CalStatus(di);
			info.nTimeStart	= di.battle_time_start;
			info.nTimeEnd	= di.battle_time_end;
			info.nDefencer	= (iStatus&CDlgTerrWarMap::ST_FIGHT) ? di.id_defender : di.id_owner;
			if (info.nDefencer==0)
				info.nDefencer = di.id_owner;
			info.nAttacker	= (iStatus&(CDlgTerrWarMap::ST_FIGHT|CDlgTerrWarMap::ST_ASSISGET))>0 ? di.id_attacker : 0;
			m_vecInfo.push_back(info);
		}
	}
	else if (m_szName == "Win_Battle_TerriDetailGuild")
	{
		ACString strText;
		PAUILABEL pLabTitle = (PAUILABEL)GetDlgItem("Title");
		CECFactionMan *pFMan = GetGame()->GetFactionMan();
		int idFaction = GetHostPlayer()->GetFactionID();
		Faction_Info *pInfo = pFMan->GetFaction(GetHostPlayer()->GetFactionID());
		ACHAR szFName[20];
		AUI_ConvertChatString(pInfo->GetName(), szFName);
		strText.Format(GetGameUIMan()->GetStringFromTable(9359), szFName);
		pLabTitle->SetText(strText);
		m_nPageCur = 0;
		m_nPageTotal = 0;
		int num[3] = {0, 0, 0};
		m_vecInfo.clear();
		for (UINT i=0; i<(*info).size(); i++)
		{
			if ((*info)[i].id_owner == idFaction || (*info)[i].id_attacker == idFaction)
			{
				m_nPageTotal++;
				num[(*info)[i].type-1]++;
				DOMAIN_INFO di = (*info)[i];
				BattleTerrInfo info;
				info.nId		= di.id;
				info.strName	= di.name;
				info.nLevel		= di.type;
				info.nState		= CalStatus(di);
				info.nTimeStart	= di.battle_time_start;
				info.nTimeEnd	= di.battle_time_end;
				info.nDefencer	= di.id_defender;
				info.nAttacker	= di.id_attacker;
				m_vecInfo.push_back(info);
			}
		}
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_3");
		strText.Format(_AL("%d"), num[2]);
		pLab->SetText(strText);
		pLab = (PAUILABEL)GetDlgItem("Txt_2");
		strText.Format(_AL("%d"), num[1]);
		pLab->SetText(strText);
		pLab = (PAUILABEL)GetDlgItem("Txt_1");
		strText.Format(_AL("%d"), num[0]);
		pLab->SetText(strText);
		m_nPageTotal = (m_nPageTotal+lineperpage-1)/lineperpage;
		GetDlgItem("Btn_Top")->Enable(false);
		GetDlgItem("Btn_Prec")->Enable(false);
		GetDlgItem("Btn_Next")->Enable(m_nPageTotal>1);
		GetDlgItem("Btn_Bottom")->Enable(m_nPageTotal>1);
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_TerriDetail"))->SetPushed(false);
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_TerriDetailGuild"))->SetPushed(true);

		int nTerr = pInfo->GetTerritoryScore();
		int level = 0;
		while( nTerr >= terrscorelevel[level + 1] )
			level++;
		int nNowTerr = nTerr - terrscorelevel[level];
		int nMaxTerr = terrscorelevel[level + 1] - terrscorelevel[level];
		m_pPro_GuildTerrScore->SetProgress(int(double(nNowTerr) / nMaxTerr * AUIPROGRESS_MAX));
		strText.Format(_AL("%d/%d"), nTerr, terrscorelevel[level + 1]);
		m_pPro_GuildTerrScore->SetHint(strText);
		m_pTxt_GuildTerrScore->SetText(GetStringFromTable(1210 + level));
	}
	
	UpdateList();
}

int CDlgBattleTerrInfo::CalStatus(_DOMAIN_INFO& di)
{
	CDlgTerrWarMap* pDlgTerrMap = (CDlgTerrWarMap*)GetGameUIMan()->GetDialog("WorldMap_TerriSub");
	int idFaction = GetHostPlayer()->GetFactionID();
	int iStatus = pDlgTerrMap->GetStatus();
	int nStatus = STATUS_NONE;
	if (di.random_award>0)
	{
		nStatus = STATUS_AWARD;
	}
	else if (iStatus & CDlgTerrWarMap::ST_BID)
	{
		nStatus = STATUS_DECLARE;
	}
	else if ( (iStatus & CDlgTerrWarMap::ST_ASSISGET) && di.id_attacker>0)
	{
		nStatus = STATUS_DECLARED;
		if (m_szName == "Win_Battle_TerriDetailGuild" && di.id_attacker == idFaction)
			nStatus = STATUS_DECLARESUCCESS;
	}
	else if ( (iStatus & CDlgTerrWarMap::ST_FIGHT) && di.id_attacker>0)
	{
		if (m_szName == "Win_Battle_TerriDetailGuild")
		{
			if (di.flag==2 && di.id_attacker==idFaction)
			{
				nStatus = STATUS_WIN;
			}
		}
		else
		{
			if(di.flag>=0)
				nStatus = STATUS_FIGHTING+di.flag;
		}
	}
	return nStatus;
}

void CDlgBattleTerrInfo::UpdateList()
{
	ACString strText;
	UINT nSize = m_vecInfo.size();	
	int idFaction = GetHostPlayer()->GetFactionID();
	CDlgTerrWarMap* pDlgTerrMap = (CDlgTerrWarMap*)GetGameUIMan()->GetDialog("WorldMap_TerriSub");
	int iStatus = pDlgTerrMap->GetStatus();
	m_pLst->ResetContent();
	GetDlgItem("Btn_Top")->Enable(m_nPageCur>0);
	GetDlgItem("Btn_Prec")->Enable(m_nPageCur>0);
	GetDlgItem("Btn_Next")->Enable(m_nPageCur<(m_nPageTotal-1));
	GetDlgItem("Btn_Bottom")->Enable(m_nPageCur<(m_nPageTotal-1));
	for (UINT i=m_nPageCur*lineperpage; i<nSize; i++)
	{
		BattleTerrInfo info = m_vecInfo[i];
		ACString strText, strStatus(_AL("--")), strTime(_AL("--")), strDefence(_AL("--")), strAttack(_AL("--"));
		switch (info.nState)
		{
		case STATUS_DECLARE:
			strStatus = GetGameUIMan()->GetStringFromTable(9441);
			break;
		case STATUS_DECLARED:
			strStatus = GetGameUIMan()->GetStringFromTable(9338);
			break;
		case STATUS_DECLARESUCCESS:
			strStatus = GetGameUIMan()->GetStringFromTable(9446);
			break;
		case STATUS_FIGHTING:
			strStatus = GetGameUIMan()->GetStringFromTable(9442);
			break;
		case STATUS_DEFENCERWIN:
			strStatus = GetGameUIMan()->GetStringFromTable(9443);
			break;
		case STATUS_ATTACKERWIN:
			strStatus = GetGameUIMan()->GetStringFromTable(9444);
			break;
		case STATUS_AWARD:
			strStatus = GetGameUIMan()->GetStringFromTable(9445);
			break;
		case STATUS_WIN:
			strStatus = GetGameUIMan()->GetStringFromTable(9447);
			break;
		}
		if ( info.nTimeStart!=0 && info.nTimeEnd!=0)
		{
			const long battleTime = info.nTimeStart - GetGame()->GetTimeZoneBias() * 60;
			tm *gtime = gmtime((const time_t*)&battleTime);
			int startHour = gtime->tm_hour;
			int startMin = gtime->tm_min;
			const long battleTimeEnd = info.nTimeEnd - GetGame()->GetTimeZoneBias() * 60;
			tm *gtimeEnd = gmtime((const time_t*)&battleTimeEnd);
			strTime.Format(GetGameUIMan()->GetStringFromTable(9330), GetGameUIMan()->GetStringFromTable(9890+gtime->tm_wday),
				startHour, startMin, gtimeEnd->tm_hour, gtimeEnd->tm_min);
		}
		if (info.nDefencer>0)
		{
			Faction_Info* fInfo =  GetGame()->GetFactionMan()->GetFaction(info.nDefencer, true);
			if (fInfo)
				strDefence = fInfo->GetName();
		}
		if (info.nAttacker>0)
		{
			Faction_Info* fInfo =  GetGame()->GetFactionMan()->GetFaction(info.nAttacker, true);
			if (fInfo)
				strAttack = fInfo->GetName();
		}
		
		strText.Format(_AL("%s\t%s\t%s\t%s\t%s\t%s"), info.strName, GetGameUIMan()->GetStringFromTable(9333+info.nLevel), 
			strStatus, strTime, strDefence, strAttack);
		m_pLst->AddString(strText);
		if (m_pLst->GetCount()>=lineperpage)
		{
			break;
		}
	}
}