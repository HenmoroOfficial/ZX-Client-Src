// Filename	: DlgBattleFactionWarApply.cpp
// Creator	: Wang Dongliang
// Date		: 2012/08/21
// Desc		: 帮战申请界面
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "Network\\ids.hxx"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_World.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_Faction.h"
#include "EC_BattleDungeon.h"
#include "DlgBattleFactionWarApply.h"
#include "factionpkraidchallenge_re.hpp"
#include "factionpkraidchallengecancel_re.hpp"
#include "factionpkraidinvite.hpp"
#include "gnetdef.h"
#include "EC_GameUIMisc.h"
#include "DlgBattleFactionWarControl.h"

AUI_BEGIN_COMMAND_MAP(CDlgBattleFactionWarApply, CDlgBase)
AUI_ON_COMMAND("Btn_Apply",		OnCommand_Apply)
AUI_ON_COMMAND("Btn_Cancel",	OnCommand_Cancel)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgBattleFactionWarApply,CDlgBase)
AUI_END_EVENT_MAP()

CDlgBattleFactionWarApply::CDlgBattleFactionWarApply()
:m_pEditName(NULL)
,m_pLabelTime(NULL) 
,m_pBtnApply(NULL)
,m_startTime(0)
,m_IsApplying(false)
{
	memset(&m_factionWarInvite,0,sizeof(m_factionWarInvite));
}
CDlgBattleFactionWarApply::~CDlgBattleFactionWarApply()
{
 
}
void CDlgBattleFactionWarApply::ResetData()
{
	m_startTime = 0;
    m_IsApplying = false;
	m_pBtnApply->Enable(true);
	m_pEditName->Enable(true);
	m_pEditName->SetText(_AL(""));
	m_pLabelTime->SetText(_AL(""));
	GetDlgItem("Lab_Time")->Show(false);
	m_pLabelTime->Show(false);
}
bool CDlgBattleFactionWarApply::OnInitDialog()
{
   m_pEditName = (PAUIEDITBOX)GetDlgItem("Edt_Name");
   m_pLabelTime = (PAUILABEL)GetDlgItem("Txt_Time");
   m_pBtnApply  =(PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Apply");

   return true;
}
void CDlgBattleFactionWarApply::OnShowDialog()
{
    ResetData();
}
void CDlgBattleFactionWarApply::OnHideDialog()
{
	//通过NPC方式打开界面
	if(this->GetData("openstyle") == 1)
	{
		this->SetData(0,"openstyle");
		GetGameUIMan()->EndNPCService();
	}
	ResetData();
}
bool CDlgBattleFactionWarApply::CheckApplyCondition()
{	
	if(GetHostPlayer()->GetFactionID() ==0 || (GetHostPlayer()->GetFRoleID() != GNET::_R_MASTER && 
		GetHostPlayer()->GetFRoleID() != GNET::_R_VICEMASTER))
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(15503),MB_OK,A3DCOLORRGBA(255, 255, 255, 160));
	    return false;
	}
	Faction_Info *pInfo = GetGame()->GetFactionMan()->GetFaction(GetHostPlayer()->GetFactionID());
	if( NULL != pInfo && pInfo->GetLevel() < 1 )
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(888), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return false;
	}

	if(0 != CDlgBattleFactionWarControl::GetCombatFaction())
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(15543),MB_OK,A3DCOLORRGBA(255, 255, 255, 160));
		return false;
	}
    return true;

}
void CDlgBattleFactionWarApply::OnCommand_Apply(const char * szCommand)
{
	ACString strName = m_pEditName->GetText();
	if( strName.GetLength() <= 0 ) return;
	GetGameSession()->factionpkraid_challenge(strName,
		                                      GetHostPlayer()->GetCharacterID(),
											  CDlgBattleFactionWarControl::GetFactionPKRaidMapId());
    m_pBtnApply->Enable(false);
	m_pEditName->Enable(false);
	m_pLabelTime->Show(true);
	GetDlgItem("Lab_Time")->Show(true);
	m_startTime = GetTickCount();
	m_IsApplying = true;

}
void CDlgBattleFactionWarApply::OnCommand_Cancel(const char * szCommand)
{
    this->Show(false);
}
void CDlgBattleFactionWarApply::OnTick()
{	
	if(GetHostPlayer()->GetSrcServerID()!=0)
	{	 
		OnCommand_Cancel("");
		return;
	}

   //剩余时间显示
	if(m_pLabelTime->IsShow())
	{
	  //倒计时300秒写死
	  int leftTime = 300*1000-(GetTickCount()-m_startTime);
	  if(leftTime <=0 )
	  {
		  leftTime = 0;
	  }
 	  ACString strTime = GetTimeText(leftTime/1000);
	  m_pLabelTime->SetText(strTime);
	}
}
int CDlgBattleFactionWarApply::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
  if(strMsg == "challenge")
  {

     FactionPkRaidChallenge_Re* p = (FactionPkRaidChallenge_Re*)wParam;
	 if(p->retcode == ERR_SUCCESS)
	 {
		 if(this->IsShow())
		  this->Show(false);
	 }
	 else
	 {
		CECBattleDungeonMan::getSingletonPtr()->ReportFactionPKError(p->retcode);
		ResetData();
	 }


  }
  else if(strMsg == "challengecancel")
  { 
	 //暂不处理
     //FactionPkRaidChallengeCancel_Re* p = (FactionPkRaidChallengeCancel_Re*)wParam;
  }
  else if(strMsg == "invite")
  {
     FactionPkRaidInvite* p = (FactionPkRaidInvite*)wParam;
	 m_factionWarInvite.mapid = p->map_id;
	 m_factionWarInvite.dstid = p->dst_factionid;
	 m_factionWarInvite.inviterid = p->inviter_factionid;
	 ACString inviterName = ACString((const ACHAR*)p->inviter_faction_name.begin(), p->inviter_faction_name.size() / sizeof (ACHAR));
	 ACString dstName = ACString((const ACHAR*)p->dst_faction_name.begin(), p->dst_faction_name.size() / sizeof (ACHAR));
	 ACString strMsg;
	 strMsg.Format(GetStringFromTable(15507),inviterName,dstName);
	 GetGameUIMan()->MessageBox("FactionPkInvite",strMsg,MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));


  }

  return 0;
}

void CDlgBattleFactionWarApply::ProcessFactionWarInvite(bool isAgree)
{
    GetGameSession()->factionpkraid_invite(GetHostPlayer()->GetCharacterID(),
		                                   m_factionWarInvite.mapid,
										   isAgree,
										   m_factionWarInvite.inviterid,
										   m_factionWarInvite.dstid);
}
void CDlgBattleFactionWarApply::ApplyFactionWarByTargetHead(const ACHAR* name)
{
	
	if(this->IsShow())
	{
	   if(m_IsApplying)
	   {
		   GetGameUIMan()->MessageBox("",GetStringFromTable(15525),MB_OK,A3DCOLORRGBA(255, 255, 255, 160));
		   return;
	   }
	   this->OnHideDialog();
	}
	else
	{
	   this->Show(true);
	}
    m_pEditName->SetText(name);
	this->OnCommand_Apply("");


}



