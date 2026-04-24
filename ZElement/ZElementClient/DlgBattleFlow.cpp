/********************************************************************
	created:	2013/4/10
	file name:	DlgFlowBattle.cpp
	author:		zhuyan
	
	purpose:    流水席
*********************************************************************/

#include "AFI.h"
#include "AIniFile.h"
#include "DlgBattleFlow.h"
#include "DlgInputNO.h"
#include "DlgChat.h"
#include "DlgMinimizeBar.h"
#include "DlgHost.h"
#include "EC_Model.h"

#include "EC_GameRun.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_UIManager.h"
#include "EC_World.h"
#include "EC_IvtrItem.h"
#include "EC_Team.h"
#include "EC_Resource.h"
#include "AUI\\AUICTranslate.h"
#include "A2DSprite.h"
#include "A3DGFXExMan.h"
#include "battlegetfield_re.hpp"
#include "globaldataman.h"
#include "elementdataman.h"
//#include "openbanquetjoin.hpp"

AUI_BEGIN_COMMAND_MAP(DlgBattleFlowInfoMax, CDlgBase)

AUI_ON_COMMAND("Btn_Minimize",		OnCommandMin)
AUI_ON_COMMAND("Btn_Exit",		OnCommandExit)	

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(DlgBattleFlowInfoMax, CDlgBase)


AUI_END_EVENT_MAP()


DlgBattleFlowInfoMax::DlgBattleFlowInfoMax()
{
	m_pKillCount = NULL;
	m_pScoreCount = NULL;
	m_pCurScore = NULL;
	m_pHighKill = NULL;
	m_pCurKill = NULL;
	m_pManyKill = NULL;
}

DlgBattleFlowInfoMax::~DlgBattleFlowInfoMax()
{

}

bool DlgBattleFlowInfoMax::OnInitDialog()
{
	m_pKillCount = (AUILabel *)GetDlgItem("Txt_Score01");
	m_pScoreCount = (AUILabel *)GetDlgItem("Txt_Score02");
	m_pCurScore = (AUILabel *)GetDlgItem("Txt_Score03");
	m_pHighKill = (AUILabel *)GetDlgItem("Txt_Score04");
	m_pCurKill = (AUILabel *)GetDlgItem("Txt_Score05");
	m_pManyKill = (AUILabel *)GetDlgItem("Txt_Score06");

	DATA_TYPE DataType;
	int idEssence = GetGame()->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, DataType);
    while(idEssence)
	{
		if(DataType == DT_BATTLE_201304_CONFIG)
			break;
		idEssence = GetGame()->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, DataType);
	}
	m_pConfig = (BATTLE_201304_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(idEssence, ID_SPACE_CONFIG, DataType);


	ACString szTemp;
	szTemp.Format(_AL("%d / %d"),0,m_pConfig->score_target);
	m_pScoreCount->SetText(szTemp);
	szTemp.Format(_AL("%d"),5);
	m_pCurScore->SetText(szTemp);
	szTemp.Format(_AL("%d"),0);
	m_pHighKill->SetText(szTemp);
	szTemp.Format(_AL("%d"),0);
	m_pCurKill->SetText(szTemp);
	szTemp.Format(_AL("%d"),0);
	m_pManyKill->SetText(szTemp);
	ACString szkill = GetGameUIMan()->GetStringFromTable(18003);
	szTemp.Format(szkill,0,0);
	m_pKillCount->SetText(szTemp);
	return true;
}

void DlgBattleFlowInfoMax::OnShowDialog()
{
	m_pIdToName.clear();
	GetHostPlayer()->CmdGetBattleInfo();
	GetGameSession()->c2s_CmdSendFlowGetField(GetGameRun()->GetCurLine());
}

void DlgBattleFlowInfoMax::OnHideDialog()
{
}


void DlgBattleFlowInfoMax::UpDateFlowInfo(int kill_cnt, int death_cnt,int score,int m_highkill_cnt,int c_kill_cnt,int m_kill_cnt)
{

	ACString szTemp;
	szTemp.Format(_AL("%d / %d"),score,m_pConfig->score_target);
	m_pScoreCount->SetText(szTemp);
	int num = c_kill_cnt + 5;
	if(num <= 0)
	{
		num = 0;
	}
	if(num >= 20)
	{
		num = 20;
	}
	szTemp.Format(_AL("%d"),num);
	m_pCurScore->SetText(szTemp);
	szTemp.Format(_AL("%d"),m_highkill_cnt);
	m_pHighKill->SetText(szTemp);

	szTemp.Format(_AL("%d"),c_kill_cnt > 0 ? c_kill_cnt : 0);
	m_pCurKill->SetText(szTemp);
	szTemp.Format(_AL("%d"),m_kill_cnt);
	m_pManyKill->SetText(szTemp);
	ACString szkill = GetGameUIMan()->GetStringFromTable(18003);
	szTemp.Format(szkill,kill_cnt,abs(death_cnt));
	m_pKillCount->SetText(szTemp);
}

void DlgBattleFlowInfoMax::OnCommandMin(const char * szCommand)
{
	DlgBattleFlowInfoMin* pDlg = (DlgBattleFlowInfoMin*)GetGameUIMan()->GetDialog("Win_Battle_SeniorPK_ScoreMin");
	pDlg->Show(true);
	Show(false);
}

void DlgBattleFlowInfoMax::OnCommandExit(const char * szCommand)
{
	if(GetHostPlayer()->checkFlowBattleScore())
	{
		GetGameUIMan()->GetDialog("Win_Battle_SeniorPK_Exit01")->Show(true);
	}
	else
	{
		GetGameUIMan()->GetDialog("Win_Battle_SeniorPK_Exit02")->Show(true);
	}
}

void DlgBattleFlowInfoMax::UpdateFlowBattleInfo(void *pData)
{
	S2C::flow_battle_info *p = (S2C::flow_battle_info*)pData;
	UINT i;
	for(i = 0; i < (UINT)p->player_count; i++ )
	{
		player_info_in_flow_battle* pData = GetBattleInfo(p->info[i].id);
		if(pData)
		{
			pData->zone_id = p->info[i].zone_id;
			pData->level = p->info[i].level;
			pData->kill = p->info[i].kill;
			pData->death = p->info[i].death;
			pData->flow_score = p->info[i].flow_score;
			pData->cls = p->info[i].cls;
			pData->battle_faction = p->info[i].battle_faction;
		}
		else
		{
			player_info_in_flow_battle pInfo;
			pInfo.id = p->info[i].id;
			pInfo.zone_id = p->info[i].zone_id;
			pInfo.level = p->info[i].level;
			pInfo.kill = p->info[i].kill;
			pInfo.death = p->info[i].death;
			pInfo.flow_score = p->info[i].flow_score;
			pInfo.cls = p->info[i].cls;
			pInfo.battle_faction = p->info[i].battle_faction;
			m_pIdToName[pInfo.id] = pInfo;
		}
	}
	UpdateFlowList();
}

player_info_in_flow_battle* DlgBattleFlowInfoMax::GetBattleInfo(int nRoleId)
{
	abase::hash_map<int,player_info_in_flow_battle>::iterator pIter = m_pIdToName.find(nRoleId);
	if(pIter != m_pIdToName.end())
	{
		return &pIter->second;
	}
	return NULL;
}


void DlgBattleFlowInfoMax::addFlowName(int nRoleID,ACString szName)
{
	player_info_in_flow_battle* pData = GetBattleInfo(nRoleID);
	if(pData)
	{
		pData->name = szName;
	}
	else
	{
		player_info_in_flow_battle pInfo;
		pInfo.id = nRoleID;
		pInfo.name  = szName;
		pInfo.zone_id = 0;
		pInfo.level = 0;
		pInfo.kill = 0;
		pInfo.death = 0;
		pInfo.flow_score = 0;
		pInfo.cls = 0;
		pInfo.battle_faction = -1;
		m_pIdToName[pInfo.id] = pInfo;
	}
}

void DlgBattleFlowInfoMax::UpdateFlowList()
{
	ACString strText;
	PAUILISTBOX pListA = (PAUILISTBOX)GetDlgItem("Lst_A");
	PAUILISTBOX pListB = (PAUILISTBOX)GetDlgItem("Lst_B");
	pListA->ResetContent();
	pListB->ResetContent();
	abase::hash_map<int,player_info_in_flow_battle>::iterator pIter = m_pIdToName.begin();
	for( ; pIter != m_pIdToName.end(); ++pIter)
	{
		player_info_in_flow_battle* pInfo = &pIter->second;	
		if(pInfo)
		{
			const ACHAR *pszName = pInfo->name;


			CECGame::SERVERINFO* pServerInfo = GetGame()->GetSrcServerInfo(pInfo->zone_id);
			ACString szTemp;
			if(pServerInfo)
			{
				szTemp.Format(_AL("%s"), AS2AC(pServerInfo->szServerName) );
			}
			else
			{
				szTemp = _AL("无");
			}
			int id = pInfo->id;
			int nScore = 5 + pInfo->flow_score;
			if(nScore <= 0)
			{
				nScore = 0;
			}
			if(nScore >= 20)
			{
				nScore = 20;
			}
			if( !pszName )
			{ 
				strText.Format(_AL(" \t%d\t%s\t%s\t%d\t%d\t%d"),pInfo->level,GetStringFromTable(PROFESSION_START + pInfo->cls),szTemp,
						pInfo->kill,pInfo->death,nScore);
				//GetGameSession()->GetPlayerBriefInfo(1, &pInfo->id, 2);
			}
			else
				strText.Format(_AL("%s\t%d\t%s\t%s\t%d\t%d\t%d"), pszName,pInfo->level,
				GetStringFromTable(PROFESSION_START + pInfo->cls),szTemp,pInfo->kill, 
				pInfo->death,nScore);
			
			if( pInfo->battle_faction == 1 )
			{
				pListA->AddString(strText);
				pListA->SetItemData(pListA->GetCount() - 1, id);
			}
			else if( pInfo->battle_faction == 2 )
			{
				pListB->AddString(strText);
				pListB->SetItemData(pListB->GetCount() - 1, id);
			}
		}
	}
}

AUI_BEGIN_COMMAND_MAP(DlgBattleFlowInfoMin, CDlgBase)

AUI_ON_COMMAND("Btn_Minimize",		OnCommandMax)
AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(DlgBattleFlowInfoMin, CDlgBase)


AUI_END_EVENT_MAP()


DlgBattleFlowInfoMin::DlgBattleFlowInfoMin()
{
	m_pScore = NULL;
	m_pDie = NULL;
}

DlgBattleFlowInfoMin::~DlgBattleFlowInfoMin()
{

}

bool DlgBattleFlowInfoMin::OnInitDialog()
{
	m_pScore = (AUILabel *)GetDlgItem("Txt_Score02");
	m_pDie = (AUILabel *)GetDlgItem("Txt_Score01");
	
	DATA_TYPE DataType;
	int idEssence = GetGame()->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, DataType);
    while(idEssence)
	{
		if(DataType == DT_BATTLE_201304_CONFIG)
			break;
		idEssence = GetGame()->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, DataType);
	}
	m_pConfig = (BATTLE_201304_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(idEssence, ID_SPACE_CONFIG, DataType);
	
	ACString szTemp;
	szTemp.Format(_AL("%d / %d"),0,m_pConfig->score_target);
	m_pScore->SetText(szTemp);
	szTemp.Format(_AL("%d / %d"),0,0);
	m_pDie->SetText(szTemp);
	return true;
}

void DlgBattleFlowInfoMin::OnShowDialog()
{

}

void DlgBattleFlowInfoMin::OnHideDialog()
{
// 	DlgBattleFlowInfoMax* pDlg = (DlgBattleFlowInfoMax*)GetGameUIMan()->GetDialog("Win_Battle_SeniorPK_Score");
// 	//pDlg->UpDateFlowInfo();
// 	pDlg->Show(true);	
}



void DlgBattleFlowInfoMin::UpDateFlowInfo(int score,int kill_cnt, int death_cnt)
{
	
	ACString szTemp;
	szTemp.Format(_AL("%d / %d"),score,m_pConfig->score_target);
	m_pScore->SetText(szTemp);
	szTemp.Format(_AL("%d / %d"),kill_cnt,death_cnt);
	m_pDie->SetText(szTemp);
}

void DlgBattleFlowInfoMin::OnCommandMax(const char * szCommand)
{
	DlgBattleFlowInfoMax* pDlg = (DlgBattleFlowInfoMax*)GetGameUIMan()->GetDialog("Win_Battle_SeniorPK_Score");
	//pDlg->UpDateFlowInfo();
	pDlg->Show(true);
	Show(false);
}



AUI_BEGIN_COMMAND_MAP(DlgBattleFlowInfoWaitMax, CDlgBase)

AUI_ON_COMMAND("Btn_Exit",		OnCommandLeave)

AUI_ON_COMMAND("Btn_Minimize",		OnCommandWaitMin)


AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(DlgBattleFlowInfoWaitMax, CDlgBase)


AUI_END_EVENT_MAP()


DlgBattleFlowInfoWaitMax::DlgBattleFlowInfoWaitMax()
{
	m_pTime = NULL;
	m_pPos = NULL;
	m_nOldPos = 0;
}

DlgBattleFlowInfoWaitMax::~DlgBattleFlowInfoWaitMax()
{

}

bool DlgBattleFlowInfoWaitMax::OnInitDialog()
{
	m_pTime = (AUILabel *)GetDlgItem("Txt_Time");
	m_pPos = (AUILabel *)GetDlgItem("Txt_Rank");
//	m_pWaitTime = (AUILabel *)GetDlgItem("Txt_Time02");
//	m_dwOldWaitTime = 0;
	return true;
}

void DlgBattleFlowInfoWaitMax::UpDateFlowWait(int iPos,int nline, int mapId)
{

	ACString szTemp;
	szTemp.Format(_AL("%d"),iPos);
	m_pPos->SetText(szTemp);
	m_nApplyLine = nline;
	m_nMapId = mapId;

	if(m_nOldPos == 0 && iPos != 0)
	{
		szTemp.Format(_AL("10:00"));
		m_pTime->SetText(szTemp);
	}
	else if(m_nOldPos > iPos)
	{
		int dwTime = iPos / ((float)(m_nOldPos - iPos) / 60);
		if((dwTime / 60) > 30)
		{
			szTemp.Format(GetStringFromTable(18008));
		}
		else
		{
			szTemp.Format(_AL("%d:%d"),dwTime / 60 ,dwTime % 60);
		}
		m_pTime->SetText(szTemp);

	}
	m_nOldPos = iPos;
	GetHostPlayer()->setIsBattleWait(true);
	((CDlgHost*)GetGameUIMan()->GetDialog("Win_CharHead"))->SetSendFlowQueueCount(GetTickCount());
}


void DlgBattleFlowInfoWaitMax::OnCommandLeave(const char * szCommand)
{
	GetGameSession()->c2s_CmdSendFlowApplyCancel(m_nApplyLine,m_nMapId,1);
	GetHostPlayer()->setIsBattleWait(false);
	Show(false);
	m_nOldPos = 0;
}

void DlgBattleFlowInfoWaitMax::OnCommandWaitMin(const char * szCommand)
{
	GetGameUIMan()->m_pDlgMinimizeBar[0]->MinimizeDialog(this, 
					GetStringFromTable(18004), CDlgMinimizeBar::MINIMIZEBAR_TYPE_FlowBattle);	
}


void DlgBattleFlowInfoWaitMax::UpdateBattleFlowWaitTime(DWORD dwTime)
{
// 	int iTime;
// 	iTime = (dwTime - m_dwOldWaitTime)/1000;
// 	ACString szTemp;
// 	if((iTime / 60) > 30)
// 	{
// 		szTemp.Format(GetStringFromTable(18008));
// 	}
// 	else
// 	{
// 		szTemp.Format(_AL("%d:%d"),iTime / 60 ,iTime % 60);
// 	}
// 	if(m_pWaitTime)
// 	{
// 		m_pWaitTime->SetText(szTemp);
// 	}

}


AUI_BEGIN_COMMAND_MAP(DlgBattleFlowInfoPrompt, CDlgBase)

AUI_ON_COMMAND("Btn_Enter",		OnCommandEnter)
AUI_ON_COMMAND("Btn_Exit",		OnCommandLeave)
AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(DlgBattleFlowInfoPrompt, CDlgBase)


AUI_END_EVENT_MAP()


DlgBattleFlowInfoPrompt::DlgBattleFlowInfoPrompt()
{
	m_nTime = 0;
	m_bBegin = false;
}

DlgBattleFlowInfoPrompt::~DlgBattleFlowInfoPrompt()
{

}

bool DlgBattleFlowInfoPrompt::OnInitDialog()
{

	return true;
}


void DlgBattleFlowInfoPrompt::OnTick()
{
	if(m_bBegin)
	{
		ACHAR szText[200];
		int iTime;
		if (GetTickCount()>m_nTime)
		{
			iTime = 0;
		}
		else
		{
			iTime = (m_nTime-GetTickCount())/1000;
		}
		if(iTime <= 0)
		{
			OnCommandLeave("");
		}
		a_sprintf(szText, GetStringFromTable(18007), iTime);
		GetDlgItem("Txt_Timeleft")->SetText(szText);

// 		ACString strText;
// 		DWORD dwTimeNow = GetTickCount();
// 		int nTime = m_nTime - dwTimeNow;
// 		if( nTime > 0 )
// 		{
// 			strText.Format(_AL("%d"), nTime);
// 			GetDlgItem("Txt_Timeleft")->SetText(strText);
// 			GetDlgItem("Txt_Timeleft")->Show(true);
// 		}
// 		else
// 		{
// 			GetDlgItem("Txt_Timeleft")->Show(false);
// 		}
	}
}

void DlgBattleFlowInfoPrompt::OnCommandEnter(const char * szCommand)
{
		//摆摊
	if(GetHostPlayer()->GetBoothState() != 0)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(18010),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	//战斗中
	else if(GetHostPlayer()->IsFighting())
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(18011),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	else if(GetHostPlayer()->IsDead())
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(18013),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	else if(GetHostPlayer()->IsTrading())
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(18012),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	GetGameSession()->c2s_CmdSendFlowBattleEnter(m_nApplyLine,m_nMapId,m_niscross);
	((DlgBattleFlowInfoWaitMax*)GetGameUIMan()->GetDialog("Win_Battle_SeniorPK_WaitLine"))->Show(false);
	Show(false);
}

void DlgBattleFlowInfoPrompt::OnCommandLeave(const char * szCommand)
{
	GetGameSession()->c2s_CmdSendFlowApplyCancel(m_nApplyLine,m_nMapId,0);
	((DlgBattleFlowInfoWaitMax*)GetGameUIMan()->GetDialog("Win_Battle_SeniorPK_WaitLine"))->Show(false);
	Show(false);
}

void DlgBattleFlowInfoPrompt::UpDateTime(int iTime,int nline, int mapId,int iscross)
{
	m_nTime = iTime;
	m_bBegin = true;
	m_nApplyLine = nline;
	m_nMapId = mapId;
	m_niscross = iscross;
	GetHostPlayer()->setIsBattleWait(false);
}





AUI_BEGIN_COMMAND_MAP(DlgBattleFlowInfoReachExit, CDlgBase)

AUI_ON_COMMAND("Btn_Apply",		OnCommandLeave)
AUI_ON_COMMAND("Btn_Cancel",		OnCommandWaitLeave)
AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(DlgBattleFlowInfoReachExit, CDlgBase)


AUI_END_EVENT_MAP()


DlgBattleFlowInfoReachExit::DlgBattleFlowInfoReachExit()
{

}

DlgBattleFlowInfoReachExit::~DlgBattleFlowInfoReachExit()
{

}

bool DlgBattleFlowInfoReachExit::OnInitDialog()
{

	return true;
}


void DlgBattleFlowInfoReachExit::OnCommandWaitLeave(const char * szCommand)
{
	Show(false);
}

void DlgBattleFlowInfoReachExit::OnCommandLeave(const char * szCommand)
{
	c2s_SendCmdFlowBattleLeave();
}


AUI_BEGIN_COMMAND_MAP(DlgBattleFlowInfoNoReachExit, CDlgBase)

AUI_ON_COMMAND("Btn_Apply",		OnCommandLeave)
AUI_ON_COMMAND("Btn_Cancel",		OnCommandWaitLeave)
AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(DlgBattleFlowInfoNoReachExit, CDlgBase)


AUI_END_EVENT_MAP()


DlgBattleFlowInfoNoReachExit::DlgBattleFlowInfoNoReachExit()
{

}

DlgBattleFlowInfoNoReachExit::~DlgBattleFlowInfoNoReachExit()
{

}

bool DlgBattleFlowInfoNoReachExit::OnInitDialog()
{


	

	return true;
}


void DlgBattleFlowInfoNoReachExit::OnShowDialog()
{
	int id = 0;
	if(GetHostPlayer()->GetRace()== RACE_HUMAN)
	{
		id = 57943;
		
	}
	else
	{
		id = 57964;
	}
	CECIvtrItem* pItem = CECIvtrItem::CreateItem(id, 0,1);
	AUICTranslate trans;
	AString strFile;
	PAUIIMAGEPICTURE pPicture = (PAUIIMAGEPICTURE)GetDlgItem("Img_Award");
	if (pItem )
	{
		pItem->SetNeedUpdate(false);
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		pPicture->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
			GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
		pPicture->SetCover(NULL, 0, 1);
		pPicture->SetHint(trans.Translate(pItem->GetDesc()));
		delete pItem;
	}
}

void DlgBattleFlowInfoNoReachExit::OnCommandWaitLeave(const char * szCommand)
{
	Show(false);
}

void DlgBattleFlowInfoNoReachExit::OnCommandLeave(const char * szCommand)
{
	c2s_SendCmdFlowBattleLeave();
}


AUI_BEGIN_COMMAND_MAP(DlgBattleFlowInfoApply, CDlgBase)
AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(DlgBattleFlowInfoApply, CDlgBase)


AUI_END_EVENT_MAP()


DlgBattleFlowInfoApply::DlgBattleFlowInfoApply()
{
m_pBattleConfig = NULL;
}

DlgBattleFlowInfoApply::~DlgBattleFlowInfoApply()
{

}

bool DlgBattleFlowInfoApply::OnInitDialog()
{
	DATA_TYPE DataType;
	int idEssence = GetGame()->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, DataType);
    while(idEssence)
	{
		if(DataType == DT_BATTLE_201304_CONFIG)
			break;
		idEssence = GetGame()->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, DataType);
	}
	m_pBattleConfig = (BATTLE_201304_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(idEssence, ID_SPACE_CONFIG, DataType);

	return true;
}


void DlgBattleFlowInfoApply::SendFolwInfoApply()
{

	int nIsTeam = 0;
	
	bool isSend = true;
	
	if(GetHostPlayer()->IsTeamMember(GetHostPlayer()->GetCharacterID())) 
	{
		if(GetHostPlayer()->GetTeam()->GetLeaderID() == GetHostPlayer()->GetCharacterID())
		{
			nIsTeam = 1;
			for(int nIndex = 0; nIndex < GetHostPlayer()->GetTeam()->GetMemberNum(); nIndex++)
			{
				CECTeamMember*pData =  GetHostPlayer()->GetTeam()->GetMemberByIndex(nIndex);
				if(pData->GetRebornCnt() < m_pBattleConfig->renascence_count
					&& pData->GetLevel() < m_pBattleConfig->require_level)
				{
					isSend = false;
					break;
				}
			}	
			if(isSend == false)
			{
				GetGameUIMan()->MessageBox("", GetStringFromTable(18006), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			}
			else
			{
				c2s_SendCmdFlowBattleApply();
			}
		}
		else
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(18002), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
	}
	else
	{
		if(GetHostPlayer()->GetRebornCount() >= m_pBattleConfig->renascence_count
			&& GetHostPlayer()->GetBasicProps().iLevel >= m_pBattleConfig->require_level)
		{
			c2s_SendCmdFlowBattleApply();
		}
		else
		{
			GetGameUIMan()->EndNPCService();
			GetGameUIMan()->MessageBox("", GetStringFromTable(18005),
				MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
	}

}

