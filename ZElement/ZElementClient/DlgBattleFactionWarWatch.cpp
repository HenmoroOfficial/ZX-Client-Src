// Filename	: DlgBattleFactionWarWatch.cpp
// Creator	: Wang Dongliang
// Date		: 2012/08/21
// Desc		: 帮战观战界面
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_World.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_Faction.h"
#include "EC_BattleDungeon.h"
#include "DlgBattleFactionWarWatch.h"
#include  "DlgGuildMan.h"
#include "DlgBattleFactionWarControl.h"
#include "factionpkraidgetlist_re.hpp"
#include "factionwithdrawbonus_re.hpp"
#include "AUI//AUILabel.h"
#include "AUI//AUIListBox.h"

//帮战列表每页显示条目数上限
static  const int LINESOFONEPAGE = 15;

//当前申请观看的帮战副本房间号
static  int  s_ApplyRoomId = 0;

SFactionWarInfo::SFactionWarInfo()
:roomid(0)
,data1(0)
,data2(0)
{
}
void  SFactionWarInfo::SetData(int index,int data)
{
	if(index == 1) 
	{
		data1 = data;
	}
	else if(index == 2)
	{
        data2 = data;
	 }
}
int SFactionWarInfo::GetData(int index)
{
	if(index == 1)
	{
		return data1;
	}
	else if(index == 2)
	{
		return data2;
	}
	return 0;
}
AUI_BEGIN_COMMAND_MAP(CDlgBattleFactionWarWatch, CDlgBase)
AUI_ON_COMMAND("Btn_Enter",		OnCommand_ApplyWatch)
AUI_ON_COMMAND("Btn_Prev",	OnCommand_PrevPage)
AUI_ON_COMMAND("Btn_Next",		OnCommand_NextPage)
AUI_ON_COMMAND("Btn_Close",	OnCommand_Close)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgBattleFactionWarWatch,CDlgBase)
AUI_END_EVENT_MAP()

CDlgBattleFactionWarWatch::CDlgBattleFactionWarWatch()
:m_pLstMembers(NULL)
,m_pLabelPage(NULL)
,m_curPage(0)
,m_pageCount(0)
,m_ApplyIndex(-1)
{
	m_factionwarinfo.clear();
}
CDlgBattleFactionWarWatch::~CDlgBattleFactionWarWatch()
{
	
}
int  CDlgBattleFactionWarWatch::GetCurRoomId()
{
  return  s_ApplyRoomId;
}
bool CDlgBattleFactionWarWatch::OnInitDialog()
{
	m_pLstMembers = (PAUILISTBOX)GetDlgItem("List_MemberA");
	m_pLabelPage  = (PAUILABEL)GetDlgItem("Txt_Page");
	return true;
}
void CDlgBattleFactionWarWatch::OnShowDialog()
{
}
void CDlgBattleFactionWarWatch::GetFactionWarList()
{
	m_curPage = 0;
	m_pageCount = 1;
	RefreshMemberList();
	RefreshPageInfo();
    GetGameSession()->factionpkraid_getlist(GetHostPlayer()->GetCharacterID(),CDlgBattleFactionWarControl::GetFactionPKRaidMapId());
}
void CDlgBattleFactionWarWatch::OnHideDialog()
{
    GetGameUIMan()->EndNPCService();
	m_factionwarinfo.clear();
}
int CDlgBattleFactionWarWatch::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
   if(strMsg == "getlist")
   {
      FactionPkRaidGetList_Re* p = (FactionPkRaidGetList_Re*)wParam;
	  if(p->retcode == ERR_SUCCESS)
	  {

      assert(CDlgBattleFactionWarControl::GetFactionPKRaidMapId() == p->map_id);
	  m_pLstMembers->ResetContent();
	  m_factionwarinfo.clear();

	  for (int i=0;i<(int)p->fields.size();++i)
	  {
          SFactionWarInfo  factionInfo;
		  factionInfo.roomid = p->fields[i].raidroom_id;
		 
		  Faction_Info *pFInfo = NULL;
		  pFInfo = GetGame()->GetFactionMan()->GetFaction(p->fields[i].attacker,true);
		  ACString strText = _AL("---");
		  if(NULL == pFInfo)
		  {
			 factionInfo.SetData(1,p->fields[i].attacker);
		  }
		  else
		  {
			 strText = pFInfo->GetName();
		  }
		  factionInfo.attackerInfo.Format(_AL("%s\t%d\t%d\t%d"),
				                             strText,
											 (int)(p->fields[i].attacker_level+1),
											 (int)(p->fields[i].attacker_num),
		  							         (int)(p->fields[i].watcher_num));
	

          strText = _AL("---");
		  pFInfo = GetGame()->GetFactionMan()->GetFaction(p->fields[i].defender,true);
		  if(NULL == pFInfo)
		  {
		     factionInfo.SetData(2,p->fields[i].defender);
		  }
		  else
		  {
		     strText = pFInfo->GetName();
		  }

		  factionInfo.defenderInfo.Format(_AL("%s\t%d\t%d"),
			                                strText,
			                                (int)(p->fields[i].defender_level+1),
										    (int)(p->fields[i].defender_num));

		  m_factionwarinfo.push_back(factionInfo);
		  if (i < LINESOFONEPAGE)
		  {
			  strText = factionInfo.attackerInfo + _AL("\t");
			  strText = strText + factionInfo.defenderInfo;
              m_pLstMembers->AddString(strText);
		  }
	  }

     m_curPage = 0;
	 m_pageCount = m_factionwarinfo.size()/LINESOFONEPAGE + (m_factionwarinfo.size()%LINESOFONEPAGE == 0 ? 0 : 1);
     RefreshPageInfo();	 

	 this->Show(true);

	 }
	 else
	 {
		CECBattleDungeonMan::getSingletonPtr()->ReportFactionPKError(p->retcode);
		this->OnHideDialog();
   
	 }
   
   }
   else if(strMsg == "withdrawbonus")
   {
      FactionWithDrawBonus_Re* p = (FactionWithDrawBonus_Re*)wParam;
	  if(p->retcode == ERR_SUCCESS)
	  {
          //刷新帮派界面
		   CDlgGuildMan *pDlg = GetGameUIMan()->m_pDlgGuildMan;
		   if(pDlg->IsShow())
		   {
			   ACString strText = _AL("");
			   strText.Format(GetStringFromTable(16001),0);
		       ((PAUILABEL)(pDlg->GetDlgItem("Txt_GetGuildwarMoney")))->SetText(strText);
		   }
		   
          GetGameUIMan()->MessageBox("",GetStringFromTable(15546),MB_OK,A3DCOLORRGBA(255, 255, 255, 160));
	  }
	  else
	  {
	      if(p->retcode == ERR_FC_MAILBOXFULL)
		  {
		    GetGameUIMan()->MessageBox("",GetStringFromTable(15544),MB_OK,A3DCOLORRGBA(255, 255, 255, 160));
		  }
		  else if(p->retcode == ERR_FC_FACTION_INEXIST)
		  {
		    GetGameUIMan()->MessageBox("",GetStringFromTable(15545),MB_OK,A3DCOLORRGBA(255, 255, 255, 160));
		  }

	  }

   }
   return 0;
}
void CDlgBattleFactionWarWatch::OnCommand_ApplyWatch(const char * szCommand)
{
   if(m_pLstMembers->GetCurSel() >= 0 && m_pLstMembers->GetCurSel() < m_pLstMembers->GetCount())
   {
       int index = m_pLstMembers->GetCurSel() + m_curPage * LINESOFONEPAGE;
	   if(index < (int)m_factionwarinfo.size())
	   { 
         m_ApplyIndex = index;
		 GetGameUIMan()->MessageBox("FactionPkWatchApplay",GetStringFromTable(15515),MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
	   }
   }
}
void CDlgBattleFactionWarWatch::OnCommand_PrevPage(const char * szCommand)
{
   if(m_curPage <= 0 ) return;
   --m_curPage;
   RefreshMemberList();
   RefreshPageInfo();

}
void CDlgBattleFactionWarWatch::OnCommand_NextPage(const char * szCommand)
{
   if(m_curPage >= m_pageCount-1) return;
   ++m_curPage;
   RefreshMemberList();
   RefreshPageInfo();
}
void CDlgBattleFactionWarWatch::OnCommand_Close(const char * szCommand)
{
	this->Show(false);
}
void CDlgBattleFactionWarWatch::RefreshMemberList()
{
   m_pLstMembers->ResetContent();
   int startIndex = m_curPage*LINESOFONEPAGE;

   for (int i = startIndex,j=0; i < (int)m_factionwarinfo.size();++i,++j)
   {
	   if(j >= LINESOFONEPAGE) break;
       ACString strText;
	   strText = m_factionwarinfo[i].attackerInfo + _AL("\t");
	   strText = strText + m_factionwarinfo[i].defenderInfo;
	   m_pLstMembers->AddString(strText);

   }
}
void CDlgBattleFactionWarWatch::OnTick()
{
	if(GetHostPlayer()->GetSrcServerID()!=0)
	{	 
		this->Show(false);
		return;
	}
   bool isChanged = false;
   Faction_Info *pFInfo = NULL;
   for (int i=0;i<(int)m_factionwarinfo.size();++i)
   {
	  
	   if(m_factionwarinfo[i].GetData(1)!=0)
	   {
		   pFInfo = GetGame()->GetFactionMan()->GetFaction(m_factionwarinfo[i].GetData(1),true);
		   if(NULL != pFInfo)
		   {
			   m_factionwarinfo[i].attackerInfo = pFInfo->GetName() + m_factionwarinfo[i].attackerInfo.Mid(3);
			   m_factionwarinfo[i].SetData(1,0);
			   isChanged = true;
		   }
           
	   }
       
	   if(m_factionwarinfo[i].GetData(2)!=0)
	   {
            pFInfo = GetGame()->GetFactionMan()->GetFaction(m_factionwarinfo[i].GetData(2),true);
			if(NULL != pFInfo)
			{
				m_factionwarinfo[i].defenderInfo = pFInfo->GetName() +  m_factionwarinfo[i].defenderInfo.Mid(3);
				m_factionwarinfo[i].SetData(2,0);
				isChanged = true;
			}
	   }  

   }

   if(isChanged)
   {
	   RefreshMemberList();
   }
}
void CDlgBattleFactionWarWatch::RefreshPageInfo()
{
	ACString strPageInfo;
	//“0”页码显示为“1”
	if(m_pageCount == 0) m_pageCount = 1;
	strPageInfo.Format(_AL("%d/%d"),m_curPage+1,m_pageCount);
	m_pLabelPage->SetText(strPageInfo);
}
void CDlgBattleFactionWarWatch::ProcessFactionWarWatchApply(bool isAgree)
{
	if(isAgree)
	{
       if(GetHostPlayer()->GetMoneyAmount() < 500000)
	   {
		   GetGameUIMan()->MessageBox("", GetStringFromTable(15516),
							MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		   return;
	   }
	   s_ApplyRoomId = m_factionwarinfo[m_ApplyIndex].roomid;
	   GetGameSession()->c2s_CmdFactionPKJoinRaidRoom(CDlgBattleFactionWarControl::GetFactionPKRaidMapId(),
		                                              CDlgBattleFactionWarControl::GetFactionPKRaidTemplateId(), 
													  s_ApplyRoomId,
													  RF_VISITOR);
	   

	}
}



