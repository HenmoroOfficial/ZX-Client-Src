// Filename	: DlgBattleFactionWarControl.cpp
// Creator	: Wang Dongliang
// Date		: 2012/08/21
// Desc		: 帮战界面
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_World.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_Faction.h"
#include "EC_BattleDungeon.h"
#include "ExpTypes.h"
#include "elementdataman.h"
#include "DlgBattleFactionWarControl.h"
#include "DlgBattleFactionWarWatch.h"
#include "factionpkraidgetscore_re.hpp"
#include "factionpkraidbegin.hpp"
#include "factionpkraidstopfight.hpp"
#include "AUI//AUIListBox.h"
#include "AUI//AUILabel.h"

//帮战副本模板ID，写死
const int CDlgBattleFactionWarControl::m_itemplateId = 691;
//帮战副本地图ID
int CDlgBattleFactionWarControl::m_imapId = 0;
//帮战、观看人数上限
static  const int LIMITCOUNTOFPERSON = 50;
static  struct SFactionWarBegin 
{
	//副本地图ID
	int mapid;
	//副本房间ID
	int roomid;
	//所属阵营   0 无阵营 1 攻击方 2  防守方  3 观察者
	int cfaction;
	int iCombatFaction;
	//unsigned int attacker;
	//unsigned int defender;
	SFactionWarBegin()
	{
	   ResetInfo();
	}
	void ResetInfo()
	{
	   memset(this,0,sizeof(SFactionWarBegin));
	}
}s_factionWarBegin;
AUI_BEGIN_COMMAND_MAP(CDlgBattleFactionWarControl, CDlgBase)
AUI_ON_COMMAND("Btn_Enter",		OnCommand_Enter)
AUI_ON_COMMAND("Btn_Leave",	OnCommand_Leave)
AUI_ON_COMMAND("Btn_Close",		OnCommand_Close)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgBattleFactionWarControl,CDlgBase)
AUI_END_EVENT_MAP()

CDlgBattleFactionWarControl::CDlgBattleFactionWarControl()
:m_pLstAttack(NULL)
,m_pLstDefend(NULL)
,m_pLabelAttacker(NULL)
,m_pLabelDefender(NULL)
,m_nLeftTime(0)
,m_dwLastRefreshTime(0)
,m_cFaction(0)
{
}
CDlgBattleFactionWarControl::~CDlgBattleFactionWarControl()
{
	
}
int CDlgBattleFactionWarControl::GetCombatFaction()
{
  return s_factionWarBegin.iCombatFaction;
}
void CDlgBattleFactionWarControl::ResetFactionWarInfo()
{
  s_factionWarBegin.ResetInfo();
}
bool CDlgBattleFactionWarControl::OnInitDialog()
{
	elementdataman* pElementDataMan = g_pGame->GetElementDataMan();
	DATA_TYPE dt;
	TRANSCRIPTION_CONFIG* pConfigData = (TRANSCRIPTION_CONFIG*)pElementDataMan->get_data_ptr(
		m_itemplateId, ID_SPACE_CONFIG,dt); 
	if (dt != DT_TRANSCRIPTION_CONFIG)
	{
		pConfigData = NULL;
	}
	
	if (pConfigData)
	{
		m_imapId = pConfigData->map_id;
	}
	else
		m_imapId = 0;

	m_pLstAttack = (PAUILISTBOX)GetDlgItem("List_MemberA");
	m_pLstDefend = (PAUILISTBOX)GetDlgItem("List_MemberB");
	m_pLabelAttacker = (PAUILABEL)GetDlgItem("Txt_CampA");
	m_pLabelDefender = (PAUILABEL)GetDlgItem("Txt_CampB");
	return true;
}
bool CDlgBattleFactionWarControl::IsInFactionPKRaidMap()
{
    if(NULL != GetHostPlayer())
	{
	  return GetHostPlayer()->GetRaidBattleInfo() == m_imapId;
	}
	return false;
}
void CDlgBattleFactionWarControl::OnShowDialog()
{
}
void CDlgBattleFactionWarControl::GetFactionWarInfo()
{
	//在帮战副本外，且有帮战
    if(this->GetCombatFaction() != 0 && !this->IsInFactionPKRaidMap())
	{
		GetGameSession()->factionpkraid_getscore(GetHostPlayer()->GetCharacterID(),
			                                 s_factionWarBegin.mapid,
											 s_factionWarBegin.roomid);
	}
	//在帮战副本中,写死
	else if(this->IsInFactionPKRaidMap())
	{
		//观战者
		if(m_cFaction != 1 && m_cFaction != 2)
		{
		   int roomId = CDlgBattleFactionWarWatch::GetCurRoomId();
	 	   assert(-1 != roomId);
		   GetGameSession()->factionpkraid_getscore(GetHostPlayer()->GetCharacterID(),
			                                        GetHostPlayer()->GetRaidBattleInfo(),
			                                         roomId);
		}
		//参战者
		else
		{
			GetGameSession()->factionpkraid_getscore(GetHostPlayer()->GetCharacterID(),
				                                     s_factionWarBegin.mapid,
											         s_factionWarBegin.roomid);
		}
	}
	else
    {
		assert(0);
	}
}
void CDlgBattleFactionWarControl::OnHideDialog()
{
	//通过NPC方式打开界面
	if(this->GetData("openstyle") == 1)
	{
		this->SetData(0,"openstyle");
		GetGameUIMan()->EndNPCService();
	}
}
bool CDlgBattleFactionWarControl::CheckFactionWarState()
{
	 if(!GetHostPlayer()->GetFactionID())
	 {
		 GetGameUIMan()->MessageBox("",GetStringFromTable(15504),MB_OK,A3DCOLORRGBA(255, 255, 255, 160));
	     return false;
	 }
     if(0 == this->GetCombatFaction())
	 {
		 GetGameUIMan()->MessageBox("",GetStringFromTable(15505),MB_OK,A3DCOLORRGBA(255, 255, 255, 160));
	     return false;
	 }
	 return true;
}
void CDlgBattleFactionWarControl::OnCommand_Enter(const char * szCommand)
{
   if(GetHostPlayer()->IsInBattle())
   {
       GetGameUIMan()->MessageBox("",GetStringFromTable(15509),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
       return;
   }
   GetGameSession()->c2s_CmdFactionPKJoinRaidRoom(s_factionWarBegin.mapid,
	                                           m_itemplateId, 
											   s_factionWarBegin.roomid,
											   s_factionWarBegin.cfaction);
}
void CDlgBattleFactionWarControl::OnCommand_Leave(const char * szCommand)
{
	assert(this->IsInFactionPKRaidMap());
    GetGameSession()->c2s_CmdLeaveRaid();
}
void CDlgBattleFactionWarControl::OnCommand_Close(const char * szCommand)
{
    this->Show(false);
}
int CDlgBattleFactionWarControl::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
  if(strMsg == "getscore")
  {
    FactionPkRaidGetScore_Re* p = (FactionPkRaidGetScore_Re*)wParam;
   if(p->retcode == ERR_SUCCESS )
   {

	 m_pLstAttack->ResetContent();
	 m_pLstDefend->ResetContent();

	 int myid = GetHostPlayer()->GetFactionID();
	 if(myid == p->attacker)
	 {
	      ((PAUILABEL)GetDlgItem("Txt_Camp"))->SetText(GetStringFromTable(15513));
	 }
	 else if(myid == p->defender)
	 {
	      ((PAUILABEL)GetDlgItem("Txt_Camp"))->SetText(GetStringFromTable(15514));
	 }
	 else 
	 {     assert(this->IsInFactionPKRaidMap());
	      ((PAUILABEL)GetDlgItem("Txt_Camp"))->SetText(GetStringFromTable(15517));
	 }
	 //判断下进入帮派副本中时，所属的阵营
	 if(this->IsInFactionPKRaidMap())
	 {
		 //观察者
		 if (m_cFaction != 1 && m_cFaction != 2)
		 {
            ((PAUILABEL)GetDlgItem("Txt_Camp"))->SetText(GetStringFromTable(15517));
		 }
	 }
	 ACString factionName;
	 int      fid = 0;
	 Faction_Info *pFInfo = NULL;

     //攻击方名称
	 factionName = _AL("---");
	 fid =  p->attacker;
     pFInfo = GetGame()->GetFactionMan()->GetFaction(p->attacker,true);
	 if(NULL != pFInfo)
	 {
	    factionName = pFInfo->GetName();
		fid = 0;
	 }
	 m_pLabelAttacker->SetText(factionName);
	 m_pLabelAttacker->SetData(fid);
     //防守方名称
	 factionName = _AL("---");
	 fid = p->defender;
	 pFInfo = GetGame()->GetFactionMan()->GetFaction(p->defender,true);
	 if(NULL != pFInfo)
	 {
	    factionName = pFInfo->GetName();
		fid = 0;
	 }
	 m_pLabelDefender->SetText(factionName);
	 m_pLabelDefender->SetData(fid);

     ACString strText;
	  int totalkills = 0;  //总击杀数
	 
	  //攻击方战况列表
	 for (int i = 0; i< (int)(p->attack_score.size());++i)
	 {
       
		 const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(p->attack_score[i].roleid,false);

		 if(NULL == pszName)
		 {
			strText.Format(_AL("---\t%d\t%d"),p->attack_score[i].kills,p->attack_score[i].deaths);
			GetGameSession()->GetPlayerBriefInfo(1,&p->attack_score[i].roleid,2);
		 }
		 else
		 {
			strText.Format(_AL("%s\t%d\t%d"),
				            pszName,
							p->attack_score[i].kills,
							p->attack_score[i].deaths);

		 }
         m_pLstAttack->AddString(strText);
		 m_pLstAttack->SetItemData(i,p->attack_score[i].roleid);
		 if(NULL == pszName)
		 {
		    m_pLstAttack->SetItemData(i,p->attack_score[i].roleid,1);
		 }

		 totalkills += p->attack_score[i].kills;
		 
	 }
	 //攻击方击杀总数
	 strText.Format(_AL("%d"),totalkills);
	 ((PAUILABEL)GetDlgItem("Txt_KillA"))->SetText(strText);

	 strText.Format(_AL("%d/%d"),p->attacker_num,LIMITCOUNTOFPERSON);
	 ((PAUILABEL)GetDlgItem("Txt_NumA"))->SetText(strText);


	 totalkills = 0;
	 //防守方战况列表
	 for (int j = 0; j< (int)(p->defender_score.size());++j)
	 {
		 
		 const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(p->defender_score[j].roleid,false);
		 
		 if(NULL == pszName)
		 {
			 strText.Format(_AL("---\t%d\t%d"),p->defender_score[j].kills,p->defender_score[j].deaths);
			 GetGameSession()->GetPlayerBriefInfo(1,&p->defender_score[j].roleid,2);
		 }
		 else
		 {
			 strText.Format(_AL("%s\t%d\t%d"),
				 pszName,
				 p->defender_score[j].kills,
				 p->defender_score[j].deaths);
			 
		 }
         m_pLstDefend->AddString(strText);
		 m_pLstDefend->SetItemData(j,p->defender_score[j].roleid);
		 if(NULL == pszName)
		 {
			 m_pLstDefend->SetItemData(j,p->defender_score[j].roleid,1);
		 }
		 
		 totalkills += p->defender_score[j].kills;
		 
	 }
	 //防守方击杀总数
	 strText.Format(_AL("%d"),totalkills);
	 ((PAUILABEL)GetDlgItem("Txt_KillB"))->SetText(strText);
	 strText.Format(_AL("%d/%d"),p->defender_num,LIMITCOUNTOFPERSON);
	 ((PAUILABEL)GetDlgItem("Txt_NumB"))->SetText(strText);
     
	 strText.Format(_AL("%d/%d"),p->watcher_num,LIMITCOUNTOFPERSON);
     ((PAUILABEL)GetDlgItem("Txt_NumC"))->SetText(strText);

	 strText.Format(GetStringFromTable(16001),p->bonus/10000);
     ((PAUILABEL)GetDlgItem("Txt_MoneyC"))->SetText(strText);

	 m_nLeftTime =  p->remain_time * 1000;
	 m_dwLastRefreshTime = GetTickCount();

	 this->Show(true);

	}
	else
	{
		CECBattleDungeonMan::getSingletonPtr()->ReportFactionPKError(p->retcode);
		this->OnHideDialog();

	}
     

  }
  else if(strMsg == "begin")
  {
    FactionPkRaidBegin* p = (FactionPkRaidBegin*)wParam;
	if(p->retcode == ERR_SUCCESS)
	{
	   
	  if(GetGameUIMan()->GetDialog("Win_Battle_GuildWar_Apply")->IsShow())
	  GetGameUIMan()->GetDialog("Win_Battle_GuildWar_Apply")->Show(false);
	  
	  if((unsigned int)GetHostPlayer()->GetFactionID() == p->attacker)
	  {
		  s_factionWarBegin.cfaction = RF_ATTACKER;
		  s_factionWarBegin.iCombatFaction = p->defender;
	  }
	  else if((unsigned int)GetHostPlayer()->GetFactionID() == p->defender)
	  {
		  s_factionWarBegin.cfaction = RF_DEFENDER;
		  s_factionWarBegin.iCombatFaction = p->attacker;
	  }
	  else
	  {
		  assert(0);
		  return 0;
	  }
	  s_factionWarBegin.mapid = p->map_id;
	  s_factionWarBegin.roomid = p->raidroom_id;
	  ACString attackerName = ACString((const ACHAR*)p->attacker_name.begin(), p->attacker_name.size() / sizeof (ACHAR));
	  ACString defenderName = ACString((const ACHAR*)p->defender_name.begin(), p->defender_name.size() / sizeof (ACHAR));
	  ACString strMsg;
	  strMsg.Format(GetStringFromTable(15508),attackerName,defenderName);
	  GetGameUIMan()->MessageBox("FactionPkBegin",strMsg,MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));


	}
	else
	{
	   CECBattleDungeonMan::getSingletonPtr()->ReportFactionPKError(p->retcode);
	}
	  
  }
  else if(strMsg == "stopfight")
  {
	  FactionPkRaidStopFight* p = (FactionPkRaidStopFight*)wParam;
	  s_factionWarBegin.iCombatFaction = 0;
	  if(this->IsShow())
		  this->Show(false);
  }
  
  return 0;
}
void CDlgBattleFactionWarControl::OnTick()
{
	 if(GetHostPlayer()->GetSrcServerID()!=0 ||(this->GetCombatFaction() == 0 && !this->IsInFactionPKRaidMap()))
	 {	 
	    this->Show(false);
		return;
	 }


	 GetDlgItem("Btn_Enter")->Enable(true);
	 GetDlgItem("Btn_Leave")->Enable(false);

	 //判断当前状态
    if(this->GetCombatFaction()!=0)
	{
	   
	  if(GetHostPlayer()->GetSrcServerID()!=0 || GetHostPlayer()->GetBattleType() != -1 || GetHostPlayer()->IsShapeChanged())
	  {
	    GetDlgItem("Btn_Enter")->Enable(false);
		GetDlgItem("Btn_Leave")->Enable(false);
	  }
	  else if (GetHostPlayer()->GetRaidBattleInfo() >0)
	  {
		   if(this->IsInFactionPKRaidMap())
		   {
			   GetDlgItem("Btn_Enter")->Enable(false);
		       GetDlgItem("Btn_Leave")->Enable(true);
		   }
		   else
		   {
			   GetDlgItem("Btn_Enter")->Enable(false);
		       GetDlgItem("Btn_Leave")->Enable(false);
		   }

	  }
	}
	else
	{
		GetDlgItem("Btn_Enter")->Enable(false);
		GetDlgItem("Btn_Leave")->Enable(true);
	}


	 DWORD dwTime = GetTickCount();
	 m_nLeftTime -= dwTime - m_dwLastRefreshTime;
	 m_dwLastRefreshTime = dwTime;
	 if(m_nLeftTime < 0)
		m_nLeftTime = 0;
	 ACString strText;
	 strText.Format(_AL("%d"),m_nLeftTime/60000);
	 ((PAUILABEL)GetDlgItem("Txt_Time"))->SetText(strText);
      

	 Faction_Info *pFInfo = NULL;
	 if (m_pLabelAttacker->GetData() != 0)
	 {
		 pFInfo = GetGame()->GetFactionMan()->GetFaction(m_pLabelAttacker->GetData(), true);
		 if (NULL != pFInfo) 
		 {
			 m_pLabelAttacker->SetText(pFInfo->GetName());
			 m_pLabelAttacker->SetData(0);
		 }
	 }
	 if(m_pLabelDefender->GetData() != 0)
	 {
	     pFInfo = GetGame()->GetFactionMan()->GetFaction(m_pLabelDefender->GetData(),true);
		 if(NULL != pFInfo)
		 {
			 m_pLabelDefender->SetText(pFInfo->GetName());
			 m_pLabelDefender->SetData(0);
		 }
	 }
	 for (int i = 0;i<m_pLstAttack->GetCount();++i)
	 {
		 if(m_pLstAttack->GetItemData(i,1) != 0)
		 {
              const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(m_pLstAttack->GetItemData(i,1),false);
			  if(NULL != pszName)
			  {
				  ACString strText = m_pLstAttack->GetText(i);
				  strText = pszName + strText.Mid(3);
				  m_pLstAttack->SetText(i,strText);
			      m_pLstAttack->SetItemData(i,0,1);
			  }
			  else
			  {
				  int id = m_pLstAttack->GetItemData(i,1);
			      GetGameSession()->GetPlayerBriefInfo(1,&id,2);
			  }

		 }
	 }

	 for (int j = 0;j<m_pLstDefend->GetCount();++j)
	 {
		 if(m_pLstDefend->GetItemData(j,1) != 0)
		 {
			 const ACHAR* pszName = g_pGame->GetGameRun()->GetPlayerName(m_pLstDefend->GetItemData(j,1),false);
			 if(NULL != pszName)
			 {
				 ACString strText = m_pLstDefend->GetText(j);
				 strText = pszName + strText.Mid(3);
				 m_pLstDefend->SetText(j,strText);
				 m_pLstDefend->SetItemData(j,0,1);
			 }
			 else
			 {
				 int id = m_pLstDefend->GetItemData(j,1);
				 GetGameSession()->GetPlayerBriefInfo(1,&id,2);
			 }
		 }
	 }


}
void CDlgBattleFactionWarControl::ProcessFactionWarBegin(bool isAgree)
{
  if(isAgree)
  {

	  //判断当前状态
	  if(GetHostPlayer()->GetSrcServerID()!=0)
	  {
		  GetGameUIMan()->MessageBox("",GetStringFromTable(15510),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		  return;
	  }	
	  else if(GetHostPlayer()->GetBattleType() != -1)
	  {
          GetGameUIMan()->MessageBox("",GetStringFromTable(15511),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		  return;
	  }
	  else if (GetHostPlayer()->GetRaidBattleInfo() > 0)
	  {
		  if(this->IsInFactionPKRaidMap())
		  {
			  assert(0);
		  }
		  else
		  {
			  GetGameUIMan()->MessageBox("",GetStringFromTable(15512),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		  }
		  return;
		  
	 }
	 else if(GetHostPlayer()->IsShapeChanged())
	 {
		  GetGameUIMan()->MessageBox("",GetStringFromTable(15542),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		  return;
	 }


	  if(GetHostPlayer()->IsInBattle())
	  {
		  GetGameUIMan()->MessageBox("",GetStringFromTable(15509),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		  return;
	  }
      GetGameSession()->c2s_CmdFactionPKJoinRaidRoom(s_factionWarBegin.mapid,
		                                             m_itemplateId, 
		                                             s_factionWarBegin.roomid,
											         s_factionWarBegin.cfaction);
  }
}


