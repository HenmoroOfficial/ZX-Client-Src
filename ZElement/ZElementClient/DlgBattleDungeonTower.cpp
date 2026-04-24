// Filename	: DlgBattleDungeonTwoer.cpp
// Creator	: Fu Chonggang
// Date		: 2012.4.19

#include "AUI\\AUIListBox.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "DlgBattleDungeonTower.h"
#include "AFI.h"
#include "EC_BattleDungeon.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_BattleDungeonTower.h"
#include "elementdataman.h"
#include "ExpTypes.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattleDungeonTower, CDlgBase)

AUI_ON_COMMAND("Btn_Enter",		OnCommandEnter)
AUI_ON_COMMAND("IDCANCEL",		OnCommandClose)

AUI_END_COMMAND_MAP()

CDlgBattleDungeonTower::CDlgBattleDungeonTower()
{
}

CDlgBattleDungeonTower::~CDlgBattleDungeonTower()
{
}

bool CDlgBattleDungeonTower::OnInitDialog()
{
	
	return true;
}

void CDlgBattleDungeonTower::OnShowDialog()
{
	// 图片描述
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Pic");
	AUIOBJECT_SETPROPERTY p;
	sprintf(p.fn, AC2AS(GetGameUIMan()->GetStringFromTable(11159)));
	pImg->SetProperty("Image File", &p);
	// 文字描述
	PAUITEXTAREA pTxt = (PAUITEXTAREA)GetDlgItem("Txt_Intro");
	pTxt->SetText(GetGameUIMan()->GetStringFromTable(11158));

	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_Info");
	pLst->ResetContent();
	ACString strText;
	int iLevel = GetHostPlayer()->GetBattleDungeonTower()->GetTowerLevel();
	int iLvPerStage = GetHostPlayer()->GetBattleDungeonTower()->GetConfig()->tower_pass_count;
	if (iLevel<0)
	{
		iLevel = 0;
	}
	for (int i=0; i<(iLevel+iLvPerStage)/iLvPerStage; ++i)
	{
		strText.Format(GetGameUIMan()->GetStringFromTable(13401), i*iLvPerStage+1);
		pLst->AddString(strText);
		if (i>=6)
		{
			break;
		}
	}
	
	// 今天进入次数
	PAUILABEL pLabTimes = (PAUILABEL)GetDlgItem("Txt_Time");
	int iMapId = GetHostPlayer()->GetBattleDungeonTower()->GetMapId();
	strText.Format(_AL("%d/1"), CECBattleDungeonMan::getSingletonPtr()->m_mapDungeonEnterInfo[iMapId]);
	pLabTimes->SetText(strText);
	GetGameSession()->c2s_CmdQueryRaidEnterCount(iMapId);
}

void CDlgBattleDungeonTower::OnTick()
{
	GetDlgItem("Btn_Enter")->Enable(CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle()==0);
}

void CDlgBattleDungeonTower::OnCommandEnter(const char * szCommand)
{
	const TOWER_TRANSCRIPTION_CONFIG* pData = GetHostPlayer()->GetBattleDungeonTower()->GetConfig();
	if (CECBattleDungeonMan::getSingletonPtr()->m_mapDungeonEnterInfo[pData->map_id] >=
		pData->max_finish_count)
	{
		CECBattleDungeonMan::getSingletonPtr()->ReportError(ERR_RAID_CLIENT_ENTER_ENOUGHTIMES);
		return;
	}	
	else if (GetHostPlayer()->GetBasicProps().iLevel<pData->player_min_level || 
		GetHostPlayer()->GetBasicProps().iLevel>pData->player_max_level)
	{
		CECBattleDungeonMan::getSingletonPtr()->ReportError(ERR_RAID_CLIENT_PLAYER_LEVEL);
		return;
	}
	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_Info");
	if (pLst->GetCurSel()>=0)
	{
		GetGameSession()->c2s_CmdNPCSevOpenRaidRoom(GetHostPlayer()->GetBattleDungeonTower()->GetMapId(), 
			GetHostPlayer()->GetBattleDungeonTower()->TOWER_TEMP_ID, 0, _AL("a"), 0);
	}
}

void CDlgBattleDungeonTower::OnCommandClose(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->EndNPCService();
}

int CDlgBattleDungeonTower::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="enter_count")
	{
		ACString strText;
		PAUILABEL pLabTimes = (PAUILABEL)GetDlgItem("Txt_Time");
		int iMapId = GetHostPlayer()->GetBattleDungeonTower()->GetMapId();
		strText.Format(_AL("%d/1"), CECBattleDungeonMan::getSingletonPtr()->m_mapDungeonEnterInfo[iMapId]);
		pLabTimes->SetText(strText);

		return 1;
	}
	return 0;
}
