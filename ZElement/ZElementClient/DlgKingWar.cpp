// Filename	: DlgKingWar.cpp
// Creator	: Wang Dongliang
// Date		: 2013/02/28
// Desc		: 国王战相关界面
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIVerticalText.h"
#include "AUI\\AUITextArea.h"
#include "aui\\AUISubDialog.h"
#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"
#include "A3DFuncs.h"
#include "A2DSprite.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_ElsePlayer.h"
#include "EC_ManPlayer.h"
#include "EC_World.h"
#include "EC_Inventory.h"
#include "DlgKingWar.h"
#include "DlgInputNO.h"
#include "DlgBattleFactionWarControl.h"
#include "DlgTask.h"
#include "EC_GameUIMan.h"
#include "EC_IvtrItem.h"
#include "elementdataman.h"
#include "globaldataman.h"
#include "EC_GPDataType.h"
#include "EC_GameSession.h"
#include "EC_GameUIMisc.h"
#include "EC_Algorithm.h"
#include "EC_Faction.h"
#include "EC_TaskInterface.h"
#include "TaskTemplMan.h"
#include "kingdombattleenter_re.hpp"
#include "kingdombattleleave_re.hpp"
#include "kingdomappoint_re.hpp"
#include "kingdomdischarge_re.hpp"
#include "kingdomgetinfo_re.hpp"
#include "kingdomsetgameattri_re.hpp"
#include "kingdomannounce_re.hpp"
#include "kingdomtitlechange.hpp"
#include "kingdompointchange.hpp"
#include "kingdomkingchange.hpp"
#include "getkingtask_re.hpp"
#include "kingcallguards_re.hpp"
#include "kinggetreward_re.hpp"

#define  KING_WHITE_TASK  28506
#define  KING_GREEN_TASK  28507
#define  KING_BLUE_TASK  28508
#define  KING_ORANGE_TASK  28509



static int GetTaskIdByType(int type)
{
   switch(type)
   {
   case 1:  return  KING_WHITE_TASK;
   case 2:  return  KING_GREEN_TASK;
   case 3:  return  KING_BLUE_TASK;
   case 4:  return  KING_ORANGE_TASK;
   default: return -1;
   }
}

AUI_BEGIN_COMMAND_MAP(CDlgKingMin, CDlgBase)
AUI_ON_COMMAND("Btn_Max",		OnCommand_Open)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingMin,CDlgBase)
AUI_END_EVENT_MAP()

enum
{
   EGETINFO_REASON_OPENDLG,
   EGETINFO_REASON_UPDATEDLG,
};
static int l_iGetKingInfoReason = EGETINFO_REASON_OPENDLG;


class CompareBattleInfo
{
public:
	CDlgKingBattleInfo3::SortType m_St;
	bool	m_bInc;
	CompareBattleInfo(CDlgKingBattleInfo3::SortType st, bool bInc=true) : m_St(st),m_bInc(bInc){}
	bool operator ()(CDlgKingBattleInfo3::SPlayerInfo & p1, CDlgKingBattleInfo3::SPlayerInfo & p2)
	{
		bool bRet = true;
		switch (m_St)
		{
		case CDlgKingBattleInfo3::ST_RANKKILL:
			bRet=p1.kill < p2.kill;
			break;
		case CDlgKingBattleInfo3::ST_RANKDEATH:
			bRet= p1.death < p2.death;
			break;
		}
		if(m_bInc)
			return bRet;
		else
			return !bRet;
	}
};

//打开国王信息界面
void CDlgKingMin::OnCommand_Open(const char * szCommand)
{
	if(GetGameUIMan()->GetDialog("Win_King_Info")->IsShow())
	{
	   GetGameUIMan()->GetDialog("Win_King_Info")->Show(false);  
	}
	else
	{
		l_iGetKingInfoReason = EGETINFO_REASON_OPENDLG;
	   //发送打开国王界面的请求
		GetGameSession()->kingwar_getinfo(GetHostPlayer()->GetCharacterID());
		GetGameSession()->kingwar_getkingtask(GetHostPlayer()->GetCharacterID());
	}
}
void CDlgKingMin::OnTick()
{
    if (GetHostPlayer()->GetKingdomKingId() != 0)
    {
         GetDlgItem("Btn_Max")->Enable(true);
		 GetDlgItem("Btn_Max")->SetHint(GetStringFromTable(15647));
    }
	else
	{
		GetDlgItem("Btn_Max")->Enable(false);
		GetDlgItem("Btn_Max")->SetHint(GetStringFromTable(15648));
	}
}

AUI_BEGIN_COMMAND_MAP(CDlgKingNameChange, CDlgBase)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingNameChange,CDlgBase)
AUI_END_EVENT_MAP()

AUI_BEGIN_COMMAND_MAP(CDlgKingChangeSlogan, CDlgBase)
AUI_ON_COMMAND("Btn_Confirm",		    OnCommand_Confirm)
AUI_ON_COMMAND("Btn_Cancel",		    OnCommand_Cancel)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingChangeSlogan,CDlgBase)
AUI_END_EVENT_MAP()

void CDlgKingChangeSlogan::OnShowDialog()
{
   PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("Edt_Input");
   pEdt->SetText(_AL(""));
   ACString strTxtScore;
   strTxtScore.Format(_AL("%d"),490);
   ((PAUILABEL)(GetDlgItem("Txt_Score")))->SetText(strTxtScore);
} 
void CDlgKingChangeSlogan::OnTick()
{
	if(CDlgKingInfo::m_KingBaseInfo.nSelfTitle != EKINGTITLE_KING)
		this->Show(false);
}
void CDlgKingChangeSlogan::OnCommand_Confirm(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("Edt_Input");
	if (pEdt->GetText()==0 || a_stricmp(pEdt->GetText(),_AL("")) == 0)
	{
		return;
	}
	if(CDlgKingInfo::m_KingBaseInfo.nCountryScore < 490)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(15637), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	ACString szText;
	szText.Format(GetStringFromTable(15620),490);
	GetGameUIMan()->MessageBox("KingChangeSlogan",szText,MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
	this->Show(false);
}
void CDlgKingChangeSlogan::OnCommand_Cancel(const char * szCommand)
{
   this->Show(false);
}
void CDlgKingChangeSlogan::ProcessSloganChange(bool isAgree)
{
   if(isAgree)
   {
       //发送宣旨请求
	   PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("Edt_Input");
	   if (pEdt->GetText()==0 || a_stricmp(pEdt->GetText(),_AL("")) == 0)
	   {
		   return;
	   }
	   ACString strText  = pEdt->GetText();
	   GetGameUIMan()->FilterBadWords(strText);
	   GetGameSession()->kingwar_annouce(GetHostPlayer()->GetCharacterID(),strText);
   }
}
AUI_BEGIN_COMMAND_MAP(CDlgKingGrantOfficial, CDlgBase)
AUI_ON_COMMAND("Btn_Confirm",		    OnCommand_Confirm)
AUI_ON_COMMAND("Btn_Cancel",		    OnCommand_Cancel)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingGrantOfficial,CDlgBase)
AUI_END_EVENT_MAP()
void CDlgKingGrantOfficial::OnShowDialog()
{
   int vocation = this->GetData();
   assert(vocation == EKINGTITLE_GENERAL || vocation == EKINGTITLE_MINISTER || vocation == EKINGTITLE_GUARD);
   //vocation转为描述ID
   if(vocation == EKINGTITLE_GENERAL)
   {
      vocation = 15621;
   }
   else if(vocation == EKINGTITLE_MINISTER)
   {
      vocation = 15622;
   }
   else if(vocation == EKINGTITLE_GUARD)
   {
      vocation = 15649;
   }
  (((PAUILABEL)(GetDlgItem("Txt_1")))->SetText(GetGameUIMan()->GetStringFromTable(vocation)));
  PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("Edt_Input");
  pEdt->SetText(_AL(""));
}
void CDlgKingGrantOfficial::OnCommand_Confirm(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("Edt_Input");
	if (pEdt->GetText()==0 || a_stricmp(pEdt->GetText(),_AL("")) == 0)
	{
		return;
	}
	//发送职位授予请求
	GetGameSession()->kingwar_appoint(GetHostPlayer()->GetCharacterID(),
	                                     pEdt->GetText(),
										(char)(this->GetData()));
	this->Show(false);
}
void CDlgKingGrantOfficial::OnCommand_Cancel(const char * szCommand)
{
    this->Show(false);
}
void CDlgKingGrantOfficial::OnTick()
{
	if(CDlgKingInfo::m_KingBaseInfo.nSelfTitle != EKINGTITLE_KING)
		this->Show(false);
}

AUI_BEGIN_COMMAND_MAP(CDlgKingGrantRightContext, CDlgBase)
AUI_ON_COMMAND("Btn_Withdraw",		    OnCommand_Dismiss)
//AUI_ON_COMMAND("Btn_cancel",		    OnCommand_Cancel)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingGrantRightContext,CDlgBase)
AUI_END_EVENT_MAP()
void CDlgKingGrantRightContext::OnCommand_Dismiss(const char * szCommand)
{
   char title;
   ACString name;
   if(CDlgKingInfo::GetCadidateTitleByRoleId(this->GetData(),title,name))
   {
	   ACString szText;
	   int vocation = 0;
	   if(title == EKINGTITLE_GENERAL)
	   {
	      vocation = 15629;
	   }
	   else if(title == EKINGTITLE_MINISTER)
       {
		  vocation = 15630;
	   }
	   else if(title == EKINGTITLE_GUARD)
	   {
	      vocation = 15650;
	   }
	   szText.Format(GetStringFromTable(vocation),name);
	   PAUIDIALOG pDlg = NULL;
	   GetGameUIMan()->MessageBox("KingDismiss",szText,MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160),&pDlg);
	   pDlg->SetData(this->GetData());
   }
   this->Show(false);
}
/*void CDlgKingGrantRightContext::OnCommand_Cancel(const char * szCommand)
{
   this->Show(false);
}*/
void CDlgKingGrantRightContext::ProcessKingDismiss(bool isAgree,int roleid)
{
	if(isAgree)
	{
		char title;
		ACString name;
		if(CDlgKingInfo::GetCadidateTitleByRoleId(roleid,title,name))
		{
			GetGameSession()->kingwar_discharge(GetHostPlayer()->GetCharacterID()
												,roleid
												,title);
		}
	}
}
void CDlgKingGrantRightContext::OnTick()
{
	if(CDlgKingInfo::m_KingBaseInfo.nSelfTitle != EKINGTITLE_KING)
		this->Show(false);
}

AUI_BEGIN_COMMAND_MAP(CDlgKingGrantRight, CDlgBase)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingGrantRight,CDlgBase)
AUI_END_EVENT_MAP()


AUI_BEGIN_COMMAND_MAP(CDlgKingInfo, CDlgBase)
AUI_ON_COMMAND("IDCANCEL",		    OnCommand_Close)
AUI_ON_COMMAND("Btn_Change",		OnCommand_ChangeCountryName)
AUI_ON_COMMAND("Btn_KingChat",		OnCommand_ChangeSlogan)
AUI_ON_COMMAND("Btn_Right01",		OnCommand_GrantOfficial)
AUI_ON_COMMAND("Btn_Right02",		OnCommand_UseScores)
AUI_ON_COMMAND("Btn_Right03",		OnCommand_Award)
AUI_ON_COMMAND("Btn_Right04",		OnCommand_PostTask)
AUI_ON_COMMAND("Btn_Right05",		OnCommand_RemoteBag)
AUI_ON_COMMAND("Btn_Right06",		OnCommand_TriExp)
AUI_ON_COMMAND("Btn_Right07",		OnCommand_DoubleDrop)
AUI_ON_COMMAND("Btn_Right08",		OnCommand_ViewTask)
AUI_ON_COMMAND("Btn_Transfer",		OnCommand_Transmit)
AUI_ON_COMMAND("Btn_Shower",		OnCommand_OpenBathroom)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingInfo,CDlgBase)
AUI_END_EVENT_MAP()


CDlgKingInfo::SKingWarBaseInfo   CDlgKingInfo::m_KingBaseInfo;
bool CDlgKingInfo::OnInitDialog()
{
	m_pSubDlgOfficial = (PAUISUBDIALOG)GetDlgItem("Sub_Right01");
	m_pSubDlgShop     = (PAUISUBDIALOG)GetDlgItem("Sub_Right02");
	m_pSubDlgAward    = (PAUISUBDIALOG)GetDlgItem("Sub_Right03");
	m_pSubDlgReleaseTask =(PAUISUBDIALOG)GetDlgItem("Sub_Right04");
	m_pSubDlgAcceptTask = (PAUISUBDIALOG)GetDlgItem("Sub_Right05");
	m_iCurPage = EKING_PAGE_OFFICIAL;
//	m_dwCallGuardTime = 0;
	m_dwBathApplayTime = 0;
	return true;
}
void CDlgKingInfo::OnShowDialog()
{
	SetKingBaseInfo();
	SetKingBaseState();
	m_pSubDlgOfficial->Show(m_iCurPage == EKING_PAGE_OFFICIAL);
	m_pSubDlgShop->Show(m_iCurPage == EKING_PAGE_SHOP);
	m_pSubDlgAward->Show(m_iCurPage == EKING_PAGE_AWARD);
	m_pSubDlgReleaseTask->Show(m_iCurPage == EKING_PAGE_TASK && m_KingBaseInfo.nSelfTitle == EKINGTITLE_KING);
	m_pSubDlgAcceptTask->Show(m_iCurPage == EKING_PAGE_TASK && m_KingBaseInfo.nSelfTitle != EKINGTITLE_KING);

}
void CDlgKingInfo::OnTick()
{
	if(GetHostPlayer()->GetKingdomKingId() == 0)
	{
		this->Show(false);
		return;
	}
    if (m_KingBaseInfo.nSelfScore != GetHostPlayer()->GetRegionReputation(28))
	{
        m_KingBaseInfo.nSelfScore = GetHostPlayer()->GetRegionReputation(28);
		ACString  strTxt;
		//国家贡献度
		strTxt.Format(_AL("%d"),m_KingBaseInfo.nSelfScore);
	    ((PAUILABEL)GetDlgItem("Txt_Score"))->SetText(strTxt);
	}
	//冷却泡澡申请
	if(m_dwBathApplayTime != 0)
	{
		DWORD dwTickTime  = GetTickCount() - m_dwBathApplayTime;
		if(dwTickTime <= 1*60*1000)
		{
			GetDlgItem("Btn_Shower")->Enable(false);
			GetDlgItem("Btn_Shower")->SetHint(GetGameUIMan()->GetStringFromTable(15680));
		}
		else
		{
			m_dwBathApplayTime = 0;
			GetDlgItem("Btn_Shower")->Enable(true);
			GetDlgItem("Btn_Shower")->SetHint(_AL(""));
		}
	}
	//冷却亲卫传送
    if(GetHostPlayer()->GetCoolTime(COOLDOWN_INDEX_KING_CALL_GUARD) <=0)
	{
		GetDlgItem("Btn_Transfer")->Enable(true);
		GetDlgItem("Btn_Transfer")->SetHint(_AL(""));
	}
	else
	{
		GetDlgItem("Btn_Transfer")->Enable(false);
		GetDlgItem("Btn_Transfer")->SetHint(GetGameUIMan()->GetStringFromTable(15681));
	}	
}
void CDlgKingInfo::OnCommand_Close(const char * szCommand)
{
   this->Show(false);
}
void CDlgKingInfo::SetKingBaseInfo()
{
	ACString strTxt;
	//国号名字(三月版本填写帮派名)
	//((PAUILABEL)GetDlgItem("Lab_Kindomname"))->SetText(m_KingBaseInfo.strCountryName);
	((PAUILABEL)GetDlgItem("Lab_Kindomname"))->SetText(m_KingBaseInfo.strFactionName);
	//国家积分
	strTxt.Format(_AL("%d"),m_KingBaseInfo.nCountryScore);
	((PAUILABEL)GetDlgItem("Txt_Kindomscore"))->SetText(strTxt);
	//国王名字
	((PAUILABEL)GetDlgItem("Txt_Kingname"))->SetText(m_KingBaseInfo.strKingName);
	//天后名字
	((PAUILABEL)GetDlgItem("Txt_Queenname"))->SetText(m_KingBaseInfo.strQueenName);
	//圣旨内容
	((PAUILABEL)GetDlgItem("Txt_KingChat"))->SetText(m_KingBaseInfo.strSlogan);
	//国家贡献度
	strTxt.Format(_AL("%d"),m_KingBaseInfo.nSelfScore);
	((PAUILABEL)GetDlgItem("Txt_Score"))->SetText(strTxt);
}
//刷新职位信息
void CDlgKingInfo::RefreshKingOfficialInfo()
{
   CDlgKingOfficialInfo* pDlg = (CDlgKingOfficialInfo*)GetGameUIMan()->GetDialog("Win_King_SubRight01");
   pDlg->RefreshKingOfficailState();
}
//刷新积分兑换信息
void CDlgKingInfo::RefreshKingShopInfo()
{
   CDlgKingShop* pDlg = (CDlgKingShop*)GetGameUIMan()->GetDialog("Win_King_SubRight02");
   pDlg->RefreshKingShopState();
}
//刷新奖励信息
void CDlgKingInfo::RefreshKingAwardInfo()
{
   CDlgKingContinueAward* pDlg = (CDlgKingContinueAward*)GetGameUIMan()->GetDialog("Win_King_SubRight03");
   pDlg->RefreshKingAwardState();
}
//刷新任务信息
void CDlgKingInfo::RefreshKingTaskInfo()
{
	if(m_KingBaseInfo.nSelfTitle == EKINGTITLE_KING)
	{
		if(!m_pSubDlgReleaseTask->IsShow())
			m_pSubDlgReleaseTask->Show(true);
		if(m_pSubDlgAcceptTask->IsShow())
			m_pSubDlgAcceptTask->Show(false);
	}
	else
	{
		if(m_pSubDlgReleaseTask->IsShow())
			m_pSubDlgReleaseTask->Show(false);
		if(!m_pSubDlgAcceptTask->IsShow())
			m_pSubDlgAcceptTask->Show(true);
	}
}
//职位改变时刷新界面信息
void CDlgKingInfo::UpdateKingInfoState()
{
  SetKingBaseInfo();
  SetKingBaseState();
  if(m_iCurPage == EKING_PAGE_OFFICIAL)
  {
      RefreshKingOfficialInfo();
  }
  else if(m_iCurPage == EKING_PAGE_SHOP)
  {
      RefreshKingShopInfo();
  }
  else if (m_iCurPage == EKING_PAGE_AWARD)
  {
	  RefreshKingAwardInfo();
  }
  else if(m_iCurPage == EKING_PAGE_TASK)
  {
     RefreshKingTaskInfo();
  }
}
//使用物品信息填充图片
void CDlgKingInfo::SetImageInfoByItem(CECIvtrItem* pItem,AUIImagePicture* pObj ,bool bUsePic)
{
	if(bUsePic)
	{
   		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
	}
	pItem->GetDetailDataFromLocal();
	int iNumGoods = pItem->GetCount();
	if( iNumGoods > 1 )
	{
		ACHAR szText[100];
		a_sprintf(szText, _AL("%d"), iNumGoods);
		pObj->SetText(szText);
	}
	else
		pObj->SetText(_AL(""));
	const ACHAR* szDesc = pItem->GetDesc();
	AUICTranslate trans;
	if( szDesc )
		pObj->SetHint(trans.Translate(szDesc));
	else
		pObj->SetHint(pItem->GetName());
	pObj->SetDataPtr(pItem, "ptr_CECIvtrItem");
}
void CDlgKingInfo::SetKingBaseState()
{

	//三月版本隐藏国号更改
	GetDlgItem("Btn_Change")->Show(false);
    //根据角色职位的不同设置按钮状态
    AUIStillImageButton* pBtn = (AUIStillImageButton*)GetDlgItem("Btn_KingChat");
	pBtn->Enable(false);
	pBtn->SetText(GetGameUIMan()->GetStringFromTable(15631));
	AUIStillImageButton* pBtn2 = (AUIStillImageButton*)GetDlgItem("Btn_Right01");
	pBtn2->SetText(GetGameUIMan()->GetStringFromTable(15642));
	GetDlgItem("Btn_Right04")->Show(false);
	GetDlgItem("Btn_Right05")->Show(true);
	GetDlgItem("Btn_Right06")->Show(true);
	GetDlgItem("Btn_Right07")->Show(true);
	GetDlgItem("Btn_Right08")->Show(true);
	//GetDlgItem("Btn_Right04")->Enable(true);
	GetDlgItem("Btn_Right05")->Enable(true);
	GetDlgItem("Btn_Right06")->Enable(true);
	GetDlgItem("Btn_Right07")->Enable(true);
	//GetDlgItem("Btn_Right08")->Enable(true);
	GetDlgItem("Btn_Transfer")->Show(false);
	GetDlgItem("Btn_Shower")->Show(false);
	if(m_KingBaseInfo.nSelfTitle == EKINGTITLE_KING)
	{
		pBtn->Enable(true);
	    pBtn->SetText(GetGameUIMan()->GetStringFromTable(15632));
		GetDlgItem("Btn_Right04")->Show(true);
		//五月版本开放任务发布
		//GetDlgItem("Btn_Right04")->Enable(true);
		GetDlgItem("Btn_Right08")->Show(false);
        GetDlgItem("Btn_Transfer")->Show(true);

	}
	else if(m_KingBaseInfo.nSelfTitle == EKINGTITLE_QUEEN   ||
		    m_KingBaseInfo.nSelfTitle == EKINGTITLE_GENERAL || 
			m_KingBaseInfo.nSelfTitle == EKINGTITLE_GUARD   ||
			m_KingBaseInfo.nSelfTitle == EKINGTITLE_MINISTER)
	{
		//五月开放任务查看
		//GetDlgItem("Btn_Right08")->Show(false);
		//GetDlgItem("Btn_Right04")->Show(true);
		//GetDlgItem("Btn_Right04")->Enable(false);
		//禁用三倍经验
		GetDlgItem("Btn_Right06")->Enable(false);
		//禁用双倍掉落
		GetDlgItem("Btn_Right07")->Enable(false);

		if(m_KingBaseInfo.nSelfTitle == EKINGTITLE_QUEEN)
        {
		   GetDlgItem("Btn_Shower")->Show(true);
		}

	}
	else if(m_KingBaseInfo.nSelfTitle == EKINGTITLE_MEMBER)
	{
		//隐藏三倍经验
		GetDlgItem("Btn_Right06")->Show(false);
		//隐藏双倍掉落
		GetDlgItem("Btn_Right07")->Show(false);
		//五月版本开放查看任务
        //GetDlgItem("Btn_Right08")->Enable(false);
	}
	else if(m_KingBaseInfo.nSelfTitle == EKINGTITLE_NONE)
	{
		//隐藏远程包裹
		GetDlgItem("Btn_Right05")->Show(false);
		//隐藏三倍经验
		GetDlgItem("Btn_Right06")->Show(false);
		//隐藏双倍掉落
		GetDlgItem("Btn_Right07")->Show(false);
		//五月版本开放查看任务
       // GetDlgItem("Btn_Right08")->Enable(false);
		pBtn2->SetText(GetGameUIMan()->GetStringFromTable(15643));
		
	}
}
//更改国号
void CDlgKingInfo::OnCommand_ChangeCountryName(const char * szCommand)
{

}
//职位授予
void CDlgKingInfo::OnCommand_GrantOfficial(const char * szCommand)
{
	if(m_iCurPage == EKING_PAGE_OFFICIAL) return;
   	m_pSubDlgOfficial->Show(true);
	m_pSubDlgShop->Show(false);
	m_pSubDlgAward->Show(false);
	m_pSubDlgAcceptTask->Show(false);
	m_pSubDlgReleaseTask->Show(false);
	m_iCurPage = EKING_PAGE_OFFICIAL;
}
//连任奖励
void CDlgKingInfo::OnCommand_Award(const char * szCommand)
{
	if(m_iCurPage == EKING_PAGE_AWARD) return;
	m_pSubDlgOfficial->Show(false);
	m_pSubDlgShop->Show(false);
	m_pSubDlgAcceptTask->Show(false);
	m_pSubDlgReleaseTask->Show(false);
	m_pSubDlgAward->Show(true);
	m_iCurPage = EKING_PAGE_AWARD;
}
//任务发布(五月版本)
void CDlgKingInfo::OnCommand_PostTask(const char * szCommand)
{
    if(m_iCurPage == EKING_PAGE_TASK) return;
	GetGameSession()->kingwar_getkingtask(GetHostPlayer()->GetCharacterID());
	m_pSubDlgOfficial->Show(false);
	m_pSubDlgShop->Show(false);
	m_pSubDlgAward->Show(false);
	m_pSubDlgAcceptTask->Show(false);
	m_pSubDlgReleaseTask->Show(true);
	m_iCurPage = EKING_PAGE_TASK;

}
//查看任务(五月版本)
void CDlgKingInfo::OnCommand_ViewTask(const char * szCommand)
{
	if(m_iCurPage == EKING_PAGE_TASK) return;
	GetGameSession()->kingwar_getkingtask(GetHostPlayer()->GetCharacterID());
	m_pSubDlgOfficial->Show(false);
	m_pSubDlgShop->Show(false);
	m_pSubDlgAward->Show(false);
	m_pSubDlgReleaseTask->Show(false);
	m_pSubDlgAcceptTask->Show(true);
	m_iCurPage = EKING_PAGE_TASK;
}
//亲卫传送
void CDlgKingInfo::OnCommand_Transmit(const char* szCommand)
{
	if(m_KingBaseInfo.vecGuards.size() <= 0)
	{
	   GetGameUIMan()->MessageBox("",GetStringFromTable(15678),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	   return;
	}
	bool canTransmit = true;
	//战场中
	if(GetHostPlayer()->GetBattleType() != -1)
	{
        canTransmit = false;
	}
	//副本中
	else if(GetHostPlayer()->GetRaidBattleInfo() > 0)
	{
        CDlgBattleFactionWarControl* pDlgFacWar = (CDlgBattleFactionWarControl*)(GetGameUIMan()->GetDialog("Win_Battle_GuildWar_Applylist"));
        
		canTransmit = pDlgFacWar->IsInFactionPKRaidMap();
		 
	}
	if(!canTransmit)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(15652),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	//发送亲卫传送请求
	GetGameSession()->c2s_SendCmdKingTryCallGuard();

}
//泡澡
void CDlgKingInfo::OnCommand_OpenBathroom(const char* szCommand)
{
   tm tmNow = g_pGame->GetServerLocalTime();
   if (tmNow.tm_wday == 6 && tmNow.tm_hour >= 18 && tmNow.tm_hour <= 21)
   {
	    if(!((tmNow.tm_hour == 18 && tmNow.tm_min < 30) || (tmNow.tm_hour == 21 && tmNow.tm_min > 30)))
		{
          GetGameUIMan()->MessageBox("",GetStringFromTable(15661),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		  return;
		}
   }
   GetGameUIMan()->MessageBox("KingOpenBathroom",
	                         GetStringFromTable(15654),
							 MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
}
//远程包裹
void CDlgKingInfo::OnCommand_RemoteBag(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_InputString");
	if(pDlg->IsShow()) return;
	if( GetHostPlayer()->TrashBoxHasPsw() )
	{
		pDlg->GetDlgItem("DEFAULT_Txt_Input")->SetText(_AL(""));
		pDlg->Show(true, false);
		pDlg->SetData(1);
	}
	else
	{
	    g_pGame->GetGameSession()->c2s_CmdOpenTrashBox("");
	}
}
//积分使用
void CDlgKingInfo::OnCommand_UseScores(const char * szCommand)
{
	if(m_iCurPage == EKING_PAGE_SHOP) return;
	m_pSubDlgOfficial->Show(false);
	m_pSubDlgShop->Show(true);
	m_pSubDlgAward->Show(false);
	m_pSubDlgAcceptTask->Show(false);
	m_pSubDlgReleaseTask->Show(false);
	m_iCurPage = EKING_PAGE_SHOP;
}
//圣旨发布
void CDlgKingInfo::OnCommand_ChangeSlogan(const char * szCommand)
{
	GetGameUIMan()->GetDialog("Win_KingChat")->Show(true,true);
}
//三倍经验
void CDlgKingInfo::OnCommand_TriExp(const char * szCommand)
{
   OnGameAttri(1);
}
//双倍掉落
void CDlgKingInfo::OnCommand_DoubleDrop(const char * szCommand)
{
   OnGameAttri(2);
}
//响应三倍经验与双倍掉落
void CDlgKingInfo::OnGameAttri(int flag)
{
	ACString szText;
	if(1 == flag)
	 szText.Format(GetStringFromTable(15626),990);
	else if(2 == flag)
     szText.Format(GetStringFromTable(15627),9990);
	PAUIDIALOG pDlg = NULL;
	GetGameUIMan()->MessageBox("KingGameAttri",szText,MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160),&pDlg);
	pDlg->SetData(flag);
}
void CDlgKingInfo::ProcessGameAttri(bool isAgree,int flag)
{
   if(isAgree)
   {
      GetGameSession()->kingwar_gameattri(GetHostPlayer()->GetCharacterID(),flag);
   }
}
void CDlgKingInfo::ProcessOpenBathroom(bool isAgree)
{
   if(isAgree)
   {
      //发送开启泡澡请求
	  GetGameSession()->kingwar_openbath(GetHostPlayer()->GetCharacterID());
	  m_dwBathApplayTime = GetTickCount();
   }
}
bool CDlgKingInfo::GetCadidateTitleByRoleId(int roleid,char& title,ACString& name)
{
    for(int i=0;i<(int)m_KingBaseInfo.vecGenerals.size();++i)
	{
	   if(roleid == m_KingBaseInfo.vecGenerals[i].roleid)
	   {
	       title = EKINGTITLE_GENERAL;
		   name = m_KingBaseInfo.vecGenerals[i].name;
		   return true;
	   }
	}
	for(int j=0;j<(int)m_KingBaseInfo.vecChancellors.size();++j)
	{
	   if (roleid == m_KingBaseInfo.vecChancellors[j].roleid)
	   {
		   title = EKINGTITLE_MINISTER;
		   name = m_KingBaseInfo.vecChancellors[j].name;
		   return true;
	   }
	}
	for (int k=0;k<(int)m_KingBaseInfo.vecGuards.size();++k)
	{
		if(roleid == m_KingBaseInfo.vecGuards[k].roleid)
		{
		   title = EKINGTITLE_GUARD;
		   name = m_KingBaseInfo.vecGuards[k].name;
		   return true;
		}
	}
	return false;
}
//授予职位
void  CDlgKingInfo::GrantOfficial(int roleid,char title,bool isOnline,const ACString& name)
{      
      SOfficailInfo info;
	  info.name = name;
	  info.roleid = roleid;
	  info.isOnline = isOnline;
      if(EKINGTITLE_GENERAL == title)
	  {
	     m_KingBaseInfo.vecGenerals.push_back(info);
	  }
	  else if(EKINGTITLE_MINISTER == title)
	  {
	      m_KingBaseInfo.vecChancellors.push_back(info);
	  }
	  else if(EKINGTITLE_GUARD == title)
	  {
	      m_KingBaseInfo.vecGuards.push_back(info);
	  }
	  RefreshKingOfficialInfo();
}
//解除职位
void  CDlgKingInfo::DismissOfficial(int roleid,char title)
{
	abase::vector<SOfficailInfo> vecOfficialInfo;
	if(EKINGTITLE_GENERAL == title)
	{
	  vecOfficialInfo = m_KingBaseInfo.vecGenerals;
	  m_KingBaseInfo.vecGenerals.clear();
	  for (int i=0;i<(int)vecOfficialInfo.size();++i)
	  {
		  if(roleid != vecOfficialInfo[i].roleid)
			  m_KingBaseInfo.vecGenerals.push_back(vecOfficialInfo[i]);
	  }
	}
	else if(EKINGTITLE_MINISTER == title)
	{
	  vecOfficialInfo = m_KingBaseInfo.vecChancellors;
	  m_KingBaseInfo.vecChancellors.clear();
	  for (int i=0;i<(int)vecOfficialInfo.size();++i)
	  {
		  if(roleid != vecOfficialInfo[i].roleid)
			  m_KingBaseInfo.vecChancellors.push_back(vecOfficialInfo[i]);
	  }
	}
	else if(EKINGTITLE_GUARD == title)
	{
		vecOfficialInfo = m_KingBaseInfo.vecGuards;
		m_KingBaseInfo.vecGuards.clear();
		for (int i=0;i<(int)vecOfficialInfo.size();++i)
		{
			if(roleid != vecOfficialInfo[i].roleid)
				m_KingBaseInfo.vecGuards.push_back(vecOfficialInfo[i]);
	  }
	}
	RefreshKingOfficialInfo();

}
int  CDlgKingInfo::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	//默认打开的是职位授予的界面
	if(strMsg == "getinfo")
	{
		KingdomGetInfo_Re* p = (KingdomGetInfo_Re*)wParam;
        if(p->retcode == ERR_SUCCESS )
		{
           m_KingBaseInfo.nSelfTitle = p->info.self_title;
		   m_KingBaseInfo.nPrivilegesMask = p->info.privileges_mask;
		   m_KingBaseInfo.strCountryName = ACString((const ACHAR*)(p->info.kingdomname.begin()),p->info.kingdomname.size()/sizeof(ACHAR));
		   m_KingBaseInfo.nKing = p->info.king.roleid;
		   m_KingBaseInfo.strKingName = ACString((const ACHAR*)(p->info.king.rolename.begin()),p->info.king.rolename.size()/sizeof(ACHAR));
		   m_KingBaseInfo.nQueen = p->info.queen.roleid;
           m_KingBaseInfo.strQueenName = ACString((const ACHAR*)(p->info.queen.rolename.begin()),p->info.queen.rolename.size()/sizeof(ACHAR));
		   m_KingBaseInfo.strSlogan =  ACString((const ACHAR*)(p->info.announce.begin()),p->info.announce.size()/sizeof(ACHAR));
		   m_KingBaseInfo.nCountryScore = p->info.points;
		   m_KingBaseInfo.nContinue = p->info.win_times;
		   m_KingBaseInfo.reward_mask    = (unsigned int)(p->info.reward_mask);
		   m_KingBaseInfo.nSelfScore = GetHostPlayer()->GetRegionReputation(28);
		   m_KingBaseInfo.nFaction = p->info.king_faction;
           m_KingBaseInfo.strFactionName = ACString((const ACHAR*)(p->info.kingfacname.begin()),p->info.kingfacname.size()/sizeof(ACHAR));
		   m_KingBaseInfo.vecGenerals.clear();
		   m_KingBaseInfo.vecChancellors.clear();
		   m_KingBaseInfo.vecGuards.clear();
		   
		   for (int i=0;i<(int)p->info.functionaries.size();++i)
		   {
			   SOfficailInfo info;
			   info.roleid = p->info.functionaries[i].roleid;
			   info.privilegesmask = p->info.functionaries[i].privileges_mask;
			   info.isOnline = p->info.functionaries[i].online;
			   info.name = ACString((const ACHAR*)(p->info.functionaries[i].rolename.begin()),p->info.functionaries[i].rolename.size()/sizeof(ACHAR));
              if(p->info.functionaries[i].title == EKINGTITLE_GENERAL)
			  {
                    m_KingBaseInfo.vecGenerals.push_back(info);
			  }
			  else if(p->info.functionaries[i].title == EKINGTITLE_MINISTER)
			  {
                    m_KingBaseInfo.vecChancellors.push_back(info);
			  }
			  else if(p->info.functionaries[i].title == EKINGTITLE_GUARD)
			  {
                    m_KingBaseInfo.vecGuards.push_back(info);
			  }
		   }

           if(l_iGetKingInfoReason == EGETINFO_REASON_OPENDLG)
		   {
		       GetGameUIMan()->GetDialog("Win_King_Info")->Show(true);
		   }
		   else if(l_iGetKingInfoReason == EGETINFO_REASON_UPDATEDLG)
		   {
			   UpdateKingInfoState();
		   }
		}
	}
	else if(strMsg == "announce")
	{
		KingdomAnnounce_Re* p = (KingdomAnnounce_Re*)wParam;
        if(p->retcode == ERR_SUCCESS )
		{
		   m_KingBaseInfo.strSlogan = ACString((const ACHAR*)(p->announce.begin()),p->announce.size()/sizeof(ACHAR));
           //圣旨内容
	       ((PAUILABEL)GetDlgItem("Txt_KingChat"))->SetText(m_KingBaseInfo.strSlogan);
		}
	}
	else if(strMsg == "appoint")
	{
		KingdomAppoint_Re* p = (KingdomAppoint_Re*)wParam;
        if(p->retcode == ERR_SUCCESS )
		{
			ACString strName;
			strName = ACString((const ACHAR*)(p->candidate_name.begin()),p->candidate_name.size()/sizeof(ACHAR));
			GrantOfficial(p->candidate,p->title,p->online,strName);
            
		}
	}
	else if(strMsg == "discharge")
	{
		KingdomDischarge_Re* p = (KingdomDischarge_Re*)wParam;
        if(p->retcode == ERR_SUCCESS )
		{
			DismissOfficial(p->candidate,p->title);
		}
	}
	else if(strMsg == "setgameattri")
	{
		KingdomSetGameAttri_Re* p = (KingdomSetGameAttri_Re*)wParam;
        if(p->retcode == ERR_SUCCESS )
		{
			
		}
	}
	else if (strMsg == "titlechanged")
	{
        KingdomTitleChange* p = (KingdomTitleChange*)wParam;
		GetHostPlayer()->SetKingdomTitle(p->self_title);
		if(this->IsShow())
		{
		   l_iGetKingInfoReason = EGETINFO_REASON_UPDATEDLG;
		   //重新获取
		   GetGameSession()->kingwar_getinfo(GetHostPlayer()->GetCharacterID());
		   GetGameSession()->kingwar_getkingtask(GetHostPlayer()->GetCharacterID());
		}
		
	}
	else if (strMsg == "pointchanged")
	{
		KingdomPointChange* p = (KingdomPointChange*)wParam;
		GetHostPlayer()->SetKingdomPoints(p->points);
		if(this->IsShow())
		{
			m_KingBaseInfo.nCountryScore = p->points;
			ACString strTxt;
			//国家积分
			strTxt.Format(_AL("%d"),m_KingBaseInfo.nCountryScore);
			((PAUILABEL)GetDlgItem("Txt_Kindomscore"))->SetText(strTxt);
		}

	}
	else if (strMsg == "kingchanged")
	{
		KingdomKingChange* p = (KingdomKingChange*)wParam;
		GetHostPlayer()->SetKingdomKingId(p->newking);
		if(this->IsShow())
		{
			l_iGetKingInfoReason = EGETINFO_REASON_UPDATEDLG;
			//重新获取
		    GetGameSession()->kingwar_getinfo(GetHostPlayer()->GetCharacterID());
			GetGameSession()->kingwar_getkingtask(GetHostPlayer()->GetCharacterID());
		}
	}
	else if(strMsg == "gettask")
	{
	   GetKingTask_Re*  p = (GetKingTask_Re*)wParam;
	   if(p->retcode == ERR_SUCCESS )
	   {
           CDlgKingTask* pDlg = m_KingBaseInfo.nSelfTitle == EKINGTITLE_KING ? 
			                    (CDlgKingTask*)GetGameUIMan()->GetDialog("Win_King_SubRight04") : 
		                        (CDlgKingTask*)GetGameUIMan()->GetDialog("Win_King_SubRight05"); 
		   
		   pDlg->RefreshTaskState(p->task_status,p->task_type,p->complete_times,p->task_issue_time);
	   }
		
	}
	else if(strMsg == "callguard")
	{
        KingCallGuards_Re* p = (KingCallGuards_Re*)wParam;
		if(p->retcode == ERR_SUCCESS )
		{ 
           //m_dwCallGuardTime = GetTickCount();
		   ACString strMsg;
		   if(p->remain_times > 0)
		   {
			  strMsg.Format(GetGameUIMan()->GetStringFromTable(15679),p->remain_times);
		   }
		   else
		   {
		      strMsg = GetGameUIMan()->GetStringFromTable(15682);
		   }
		   GetGameUIMan()->MessageBox("", strMsg, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
	}
	else if(strMsg == "getaward")
	{
		//KingGetReward_Re* p = (KingGetReward_Re*)wParam;
		//if(p->retcode == ERR_SUCCESS)
		{
			if(this->IsShow())
			{
				l_iGetKingInfoReason = EGETINFO_REASON_UPDATEDLG;
				//重新获取
				GetGameSession()->kingwar_getinfo(GetHostPlayer()->GetCharacterID());
				GetGameSession()->kingwar_getkingtask(GetHostPlayer()->GetCharacterID());
			}
		}
	}


   return 0;
}
AUI_BEGIN_COMMAND_MAP(CDlgKingRankList, CDlgBase)
AUI_ON_COMMAND("IDCANCEL",		    OnCommand_Close)
AUI_ON_COMMAND("Btn_Prev",		    OnCommand_PrefPage)
AUI_ON_COMMAND("Btn_Next",		    OnCommand_NextPage)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingRankList,CDlgBase)
AUI_END_EVENT_MAP()

CDlgKingRankList::CDlgKingRankList()
:m_curPage(0)
,m_pageCount(0)
{
   memset(&m_RankInfo,0,sizeof(m_RankInfo));
}
void CDlgKingRankList::OnShowDialog()
{
  m_curPage = 0;
  m_pageCount = m_RankInfo.ranklist.size()/ELINESOFONEPAGE + (m_RankInfo.ranklist.size()%ELINESOFONEPAGE == 0 ? 0 : 1);
  RefreshSelfInfo();
  RefreshRankListInfo();
  RefreshPageInfo();
}
void CDlgKingRankList::OnCommand_Close(const char * szCommand)
{
  this->Show(false);
}
void CDlgKingRankList::OnCommand_PrefPage(const char * szCommand)
{
	if(m_curPage <= 0 ) return;
	--m_curPage;
   RefreshRankListInfo();
   RefreshPageInfo();
}
void CDlgKingRankList::OnCommand_NextPage(const char * szCommand)
{
	if(m_curPage >= m_pageCount-1) return;
	++m_curPage;
    RefreshRankListInfo();
    RefreshPageInfo();
}
void  CDlgKingRankList::RefreshSelfInfo()
{
    ACString strInfo;
	strInfo.Format(_AL("%d"),m_RankInfo.selfrank);
	((PAUILABEL)GetDlgItem("Txt_Rank"))->SetText(strInfo);
	strInfo.Format(_AL("%d"),m_RankInfo.selfscore);
	((PAUILABEL)GetDlgItem("Txt_Rank"))->SetText(strInfo);
	

}
void  CDlgKingRankList::RefreshRankListInfo()
{
	PAUILISTBOX pListRankList = (PAUILISTBOX)GetDlgItem("Lst_Member");
    pListRankList->ResetContent();
	int startIndex = m_curPage*ELINESOFONEPAGE;
	for (int i = startIndex,j=0; i < (int)m_RankInfo.ranklist.size();++i,++j)
	{
		if(j >= ELINESOFONEPAGE) break;
		ACString strText;
		strText.Format(_AL("%d\t%s\t%d")
			           ,i+1
			           ,m_RankInfo.ranklist[i].rolename
					   ,m_RankInfo.ranklist[i].score);
		pListRankList->AddString(strText);
		
   }
}
void  CDlgKingRankList::RefreshPageInfo()
{
	ACString strPageInfo;
	//“0”页码显示为“1”
	if(m_pageCount == 0) m_pageCount = 1;
	strPageInfo.Format(_AL("%d/%d"),m_curPage+1,m_pageCount);
	((PAUILABEL)GetDlgItem("Txt_Page"))->SetText(strPageInfo);
}


AUI_BEGIN_COMMAND_MAP(CDlgKingBattleInfo, CDlgBase)
AUI_ON_COMMAND("Btn_Exit",		    OnCommand_ExitKingWar)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingBattleInfo,CDlgBase)
AUI_END_EVENT_MAP()
void CDlgKingBattleInfo::OnShowDialog()
{
	if( 0 == stricmp(GetName(), "Win_King_BattleInfo") )
	{
	  ((PAUILABEL)GetDlgItem("Txt_Pro00"))->SetText(_AL(""));
	  ((PAUILABEL)GetDlgItem("Txt_01"))->SetText(_AL(""));
	  ((PAUILABEL)GetDlgItem("Txt_02"))->SetText(_AL(""));
	  ((PAUIPROGRESS)GetDlgItem("Pro_00"))->SetRangeMax(100);
	  ((PAUIPROGRESS)GetDlgItem("Pro_00"))->SetProgress(0);
	  ((PAUILABEL)GetDlgItem("Txt_LeftTime"))->SetText(_AL(""));
	}
	else if( 0 == stricmp(GetName(), "Win_King_BattleInfo2") )
	{
       ((PAUILABEL)GetDlgItem("Txt_Time"))->SetText(_AL(""));
	}
}
void CDlgKingBattleInfo::SetEndTime(int endTime)
{
   m_nEndTime = endTime;
}
void CDlgKingBattleInfo::OnCommand_ExitKingWar(const char * szCommand)
{
	GetGameUIMan()->MessageBox("KingWarLeave"
		,GetGameUIMan()->GetStringFromTable(15641)
		,MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
}
void CDlgKingBattleInfo::RefreshState(int curnum,int maxnum,int numA,int numB)
{
	if( 0 == stricmp(GetName(), "Win_King_BattleInfo") )
	{
		float ratio = float(curnum)/(float)(maxnum)*100.f;
		ACString strTxt;
		strTxt.Format(_AL("%.2f%s"),ratio,_AL("%"));
		((PAUILABEL)GetDlgItem("Txt_Pro00"))->SetText(strTxt);
		strTxt.Format(_AL("%d"),numA);
		((PAUILABEL)GetDlgItem("Txt_01"))->SetText(strTxt);
		strTxt.Format(_AL("%d"),numB);
		((PAUILABEL)GetDlgItem("Txt_02"))->SetText(strTxt);

		((PAUIPROGRESS)GetDlgItem("Pro_00"))->SetRangeMax(maxnum);
		((PAUIPROGRESS)GetDlgItem("Pro_00"))->SetProgress(curnum);
	}
}
void CDlgKingBattleInfo::OnTick()
{
	ACString strTime = GetTimeText(m_nEndTime-GetGame()->GetServerGMTTime());
	if( 0 == stricmp(GetName(), "Win_King_BattleInfo") )
	{
       ((PAUILABEL)GetDlgItem("Txt_LeftTime"))->SetText(strTime);
	   if(GetHostPlayer()->GetKingdomWarType() != 1 && GetHostPlayer()->GetKingdomWarType() != 3)
	       this->Show(false);
	}
	else if( 0 == stricmp(GetName(), "Win_King_BattleInfo2") )
	{
	   ((PAUILABEL)GetDlgItem("Txt_Time"))->SetText(strTime);
	   if( GetHostPlayer()->GetKingdomWarType() != 2)
		 this->Show(false);
	}
}
AUI_BEGIN_COMMAND_MAP(CDlgKingBattleInfo3, CDlgBase)
AUI_ON_COMMAND("IDCANCEL",		    OnCommand_Close)
AUI_ON_COMMAND("Btn_Kill10",		OnCommand_ViewRankKill)
AUI_ON_COMMAND("Btn_Die10",		    OnCommand_ViewRankDeath)
AUI_ON_COMMAND("Btn_Guild01",		OnCommand_ViewDefender)
AUI_ON_COMMAND("Btn_Guild02",		OnCommand_ViewAttacker1)
AUI_ON_COMMAND("Btn_Guild03",		OnCommand_ViewAttacker2)
AUI_ON_COMMAND("Btn_Guild04",		OnCommand_ViewAttacker3)
AUI_ON_COMMAND("Btn_Guild05",		OnCommand_ViewAttacker4)
AUI_ON_COMMAND("Btn_Exit",		    OnCommand_ExitKingWar)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingBattleInfo3,CDlgBase)
AUI_END_EVENT_MAP()
bool CDlgKingBattleInfo3::OnInitDialog()
{
	m_pListMembers = (PAUILISTBOX)GetDlgItem("Lst_Info");
	m_pBtnDefender = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Guild01");
	m_pBtnAttacker1 = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Guild02");
	m_pBtnAttacker2 = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Guild03");
	m_pBtnAttacker3 = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Guild04");
	m_pBtnAttacker4 = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Guild05");
	m_nEndTime = 0;
	m_cCurPage = EBATTALE_PAGE_KILL;
	m_nDefender = 0;
	m_nAttacker1 = 0;
	m_nAttacker2 = 0;
	m_nAttacker3 = 0;
	m_nAttacker4 = 0;
	m_vecPlayerInfo.clear();
   return true;
}
void CDlgKingBattleInfo3::OnShowDialog()
{
	RefreshCurPage();
}
void CDlgKingBattleInfo3::SetMafiaIds(int defender,int attackercount,int *attackers)
{
   OnInitDialog();
   m_nDefender = defender;
   if(attackercount > 0)
	  m_nAttacker1 = attackers[0];
   if(attackercount > 1)
	  m_nAttacker2 = attackers[1];
   if(attackercount > 2)
	  m_nAttacker3 = attackers[2];
   if(attackercount > 3)
	  m_nAttacker4 = attackers[3];

   GetDlgItem("Btn_Guild01")->Enable(m_nDefender != 0);
   GetDlgItem("Btn_Guild02")->Show(m_nAttacker1 != 0);
   GetDlgItem("Btn_Guild03")->Show(m_nAttacker2 != 0);
   GetDlgItem("Btn_Guild04")->Show(m_nAttacker3 != 0);
   GetDlgItem("Btn_Guild05")->Show(m_nAttacker4 != 0);
  
   SetFactionName(m_pBtnDefender,m_nDefender,true);
   if(m_nAttacker1 != 0)
      SetFactionName(m_pBtnAttacker1,m_nAttacker1);
   if(m_nAttacker2 != 0)
      SetFactionName(m_pBtnAttacker2,m_nAttacker2);
   if(m_nAttacker3 != 0)
      SetFactionName(m_pBtnAttacker3,m_nAttacker3);
   if(m_nAttacker4 != 0)
      SetFactionName(m_pBtnAttacker4,m_nAttacker4);
}
void CDlgKingBattleInfo3::SetFactionName(AUIStillImageButton* pBtn,int iFacId,bool bDefender)
{
	ACString factionName = _AL("---");
	int      fid = iFacId;
    Faction_Info *pFInfo = NULL;

	if(iFacId != 0)
	{
		pFInfo = GetGame()->GetFactionMan()->GetFaction(iFacId,true);
		if(NULL != pFInfo)
		{
			factionName = pFInfo->GetName();
			fid = 0;
		}
	}
	//守方为NPC
	else if(bDefender)
	{
	    factionName = GetGameUIMan()->GetStringFromTable(15640);
	}
	pBtn->SetText(factionName);
	pBtn->SetData(fid);

}
void CDlgKingBattleInfo3::GetFactionName(AUIStillImageButton* pBtn)
{
	if(!pBtn->IsShow())  return;
    Faction_Info *pFInfo = NULL;
	if (pBtn->GetData() != 0)
	{
		pFInfo = GetGame()->GetFactionMan()->GetFaction(pBtn->GetData(), true);
		if (NULL != pFInfo) 
		{
			pBtn->SetText(pFInfo->GetName());
			pBtn->SetData(0);
		}
	}
}
void CDlgKingBattleInfo3::SetEndTime(int endTime,int wartype)
{
   m_nEndTime = endTime;
   if(wartype == 2)
   {
	   GetDlgItem("Btn_Guild01")->Enable(false);
	   if(m_cCurPage = EBATTALE_PAGE_DEFENDER)
	     m_pListMembers->ResetContent();
   }
   else
   {
      GetDlgItem("Btn_Guild01")->Enable(m_nDefender != 0);
   }
}
void CDlgKingBattleInfo3::SetMafiaInfo(S2C::kingdom_mafia_info* info)
{
	m_vecPlayerInfo.clear();
	for (int i = 0;i < info->player_count; ++i)
	{
		SPlayerInfo playerinfo;
		playerinfo.roleid  = info->info[i].role_id;
		playerinfo.mafiaid = info->info[i].mafia_id;
		playerinfo.kill    = info->info[i].kill_count;
		playerinfo.death   = info->info[i].death_count;
		m_vecPlayerInfo.push_back(playerinfo);
	}	 	
}
void CDlgKingBattleInfo3::OnCommand_Close(const char * szCommand)
{
    this->Show(false);
}
void CDlgKingBattleInfo3::OnCommand_ViewRankKill(const char * szCommand)
{
    if (m_cCurPage == EBATTALE_PAGE_KILL)
        return;
	 m_cCurPage = EBATTALE_PAGE_KILL;
	 RefreshCurPage();
}
void CDlgKingBattleInfo3::OnCommand_ViewRankDeath(const char * szCommand)
{
	if (m_cCurPage == EBATTALE_PAGE_DEATH)
        return;
	 m_cCurPage = EBATTALE_PAGE_DEATH;
	 RefreshCurPage();
}
void CDlgKingBattleInfo3::OnCommand_ViewDefender(const char * szCommand)
{
	if (m_cCurPage == EBATTALE_PAGE_DEFENDER)
        return;
	m_cCurPage = EBATTALE_PAGE_DEFENDER;
	RefreshCurPage();
}
void CDlgKingBattleInfo3::OnCommand_ViewAttacker1(const char * szCommand)
{
	if (m_cCurPage == EBATTALE_PAGE_ATTACKER1)
        return;
	m_cCurPage = EBATTALE_PAGE_ATTACKER1;
	RefreshCurPage();
}
void CDlgKingBattleInfo3::OnCommand_ViewAttacker2(const char * szCommand)
{
	if (m_cCurPage == EBATTALE_PAGE_ATTACKER2)
        return;
	m_cCurPage = EBATTALE_PAGE_ATTACKER2;
	RefreshCurPage();
}
void CDlgKingBattleInfo3::OnCommand_ViewAttacker3(const char * szCommand)
{
	if (m_cCurPage == EBATTALE_PAGE_ATTACKER3)
        return;
	m_cCurPage = EBATTALE_PAGE_ATTACKER3;
	RefreshCurPage();
}
void CDlgKingBattleInfo3::OnCommand_ViewAttacker4(const char * szCommand)
{
	if (m_cCurPage == EBATTALE_PAGE_ATTACKER4)
        return;
	m_cCurPage = EBATTALE_PAGE_ATTACKER4;
	RefreshCurPage();
}
void CDlgKingBattleInfo3::RefreshCurPage()
{
	if(m_cCurPage != EBATTALE_PAGE_DEATH)
	  BubbleSort(m_vecPlayerInfo.begin(), m_vecPlayerInfo.end(), CompareBattleInfo(ST_RANKKILL,false));
    else
	  BubbleSort(m_vecPlayerInfo.begin(), m_vecPlayerInfo.end(), CompareBattleInfo(ST_RANKDEATH,false));
	abase::vector<SPlayerInfo>  playerinfos;
	playerinfos.clear();
	if(m_cCurPage == EBATTALE_PAGE_KILL)
	{
		for (int i = 0;i<ERANK_COUNT_KILL;++i)
		{
			if((int)m_vecPlayerInfo.size() > i)
				playerinfos.push_back(m_vecPlayerInfo[i]);
			else
				break;
		}
	}
	else if(m_cCurPage == EBATTALE_PAGE_DEATH)
	{
		for (int i = 0;i<ERANK_COUNT_DEATH;++i)
		{
			if((int)m_vecPlayerInfo.size() > i)
				playerinfos.push_back(m_vecPlayerInfo[i]);
			else
				break;
		}
	}
	else if(m_cCurPage == EBATTALE_PAGE_DEFENDER)
	{
		for (int i = 0; i < (int)m_vecPlayerInfo.size();++i)
		{
			if(m_nDefender == m_vecPlayerInfo[i].mafiaid)
				playerinfos.push_back(m_vecPlayerInfo[i]);
		}
	}
	else if(m_cCurPage == EBATTALE_PAGE_ATTACKER1)
	{
		for (int i = 0; i < (int)m_vecPlayerInfo.size();++i)
		{
			if(m_nAttacker1 == m_vecPlayerInfo[i].mafiaid)
				playerinfos.push_back(m_vecPlayerInfo[i]);
		}
	}
	else if(m_cCurPage == EBATTALE_PAGE_ATTACKER2)
	{
		for (int i = 0; i < (int)m_vecPlayerInfo.size();++i)
		{
			if(m_nAttacker2 == m_vecPlayerInfo[i].mafiaid)
				playerinfos.push_back(m_vecPlayerInfo[i]);
		}
	}
	else if(m_cCurPage == EBATTALE_PAGE_ATTACKER3)
	{
		for (int i = 0; i < (int)m_vecPlayerInfo.size();++i)
		{
			if(m_nAttacker3 == m_vecPlayerInfo[i].mafiaid)
				playerinfos.push_back(m_vecPlayerInfo[i]);
		}
	}
	else if(m_cCurPage == EBATTALE_PAGE_ATTACKER4)
	{
		for (int i = 0; i < (int)m_vecPlayerInfo.size();++i)
		{
			if(m_nAttacker4 == m_vecPlayerInfo[i].mafiaid)
				playerinfos.push_back(m_vecPlayerInfo[i]);
		}
	}
	DisPlayCurPagePlayerInfo(playerinfos);

}
void CDlgKingBattleInfo3::OnCommand_ExitKingWar(const char * szCommand)
{
    GetGameUIMan()->MessageBox("KingWarLeave"
		                       ,GetGameUIMan()->GetStringFromTable(15641)
							   ,MB_OKCANCEL, 
							   A3DCOLORRGBA(255, 255, 255, 160));
}
void CDlgKingBattleInfo3::ProcessKingWarLeave(bool isAgree)
{
   if (isAgree)
   {
	   GetGameSession()->c2s_CmdKingdomLeave();
   }
}
void CDlgKingBattleInfo3::DisPlayCurPagePlayerInfo(abase::vector<SPlayerInfo>& playinfos)
{
    m_pListMembers->ResetContent();
    ACString strText;
	for (int i = 0; i < (int)(playinfos.size());++i)
	{
       const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(playinfos[i].roleid,false);
	
	   if(NULL == pszName)
	   {
		   strText.Format(_AL("---\t%d\t%d"),playinfos[i].kill,playinfos[i].death);
		   GetGameSession()->GetPlayerBriefInfo(1,&playinfos[i].roleid,2);
	   }
	   else
	   {
		   strText.Format(_AL("%s\t%d\t%d"),
			   pszName,
			   playinfos[i].kill,
			   playinfos[i].death);
		   
	   }
	   m_pListMembers->AddString(strText);
	   m_pListMembers->SetItemData(i,playinfos[i].roleid);
	   if(NULL == pszName)
	   {
		   m_pListMembers->SetItemData(i,playinfos[i].roleid,1);
	   }
	}

}
void CDlgKingBattleInfo3::OnTick()
{
	if(GetHostPlayer()->GetKingdomWarType() != 1 && GetHostPlayer()->GetKingdomWarType() != 2)
	     this->Show(false);
	ACString strTime = GetTimeText(m_nEndTime-GetGame()->GetServerGMTTime());
	((PAUILABEL)GetDlgItem("Txt_LeftTime"))->SetText(strTime);
	GetFactionName(m_pBtnDefender);
	GetFactionName(m_pBtnAttacker1);
	GetFactionName(m_pBtnAttacker2);
	GetFactionName(m_pBtnAttacker3);
	GetFactionName(m_pBtnAttacker4);

	for (int i = 0;i<m_pListMembers->GetCount();++i)
	{
		if(m_pListMembers->GetItemData(i,1) != 0)
		{
			const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(m_pListMembers->GetItemData(i,1),false);
			if(NULL != pszName)
			{
				ACString strText = m_pListMembers->GetText(i);
				strText = pszName + strText.Mid(3);
				m_pListMembers->SetText(i,strText);
				m_pListMembers->SetItemData(i,0,1);
			}
			else
			{
				int id = m_pListMembers->GetItemData(i,1);
				GetGameSession()->GetPlayerBriefInfo(1,&id,2);
			}
			
		}
	 }
}


AUI_BEGIN_COMMAND_MAP(CDlgKingOfficialInfo, CDlgBase)
AUI_ON_COMMAND("Btn_Authorize01",		    OnCommand_GrantGeneral)
AUI_ON_COMMAND("Btn_Authorize02",		    OnCommand_GrantChancellor)
AUI_ON_COMMAND("Btn_Authorize03",		    OnCommand_GrantGuard)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingOfficialInfo,CDlgBase)
AUI_ON_EVENT("Lst_Member01",			WM_RBUTTONUP,	OnEventRButtonUpLstGenerals)
AUI_ON_EVENT("Lst_Member02",			WM_RBUTTONUP,	OnEventRButtonUpLstChancellors)
AUI_ON_EVENT("Lst_Member03",			WM_RBUTTONUP,	OnEventRButtonUpLstGuards)
AUI_END_EVENT_MAP()
void CDlgKingOfficialInfo::OnShowDialog()
{
    SetKingOfficialInfo();
}
void CDlgKingOfficialInfo::RefreshKingOfficailState()
{
    SetKingOfficialInfo();
}
//职位信息
void CDlgKingOfficialInfo::SetKingOfficialInfo()
{
	AUIListBox* pList = NULL;
	pList = (AUIListBox*)GetDlgItem("Lst_Member01");
	pList->ResetContent();
	for (int i=0; i<(int)CDlgKingInfo::m_KingBaseInfo.vecGenerals.size();++i)
	{
		pList->AddString(CDlgKingInfo::m_KingBaseInfo.vecGenerals[i].name);
		pList->SetItemTextColor(pList->GetCount()-1,CDlgKingInfo::m_KingBaseInfo.vecGenerals[i].isOnline ? 
			                                        A3DCOLORRGB(152,227,252) : 
		                                            A3DCOLORRGB(127,127,127));
	}
	pList = (AUIListBox*)GetDlgItem("Lst_Member02");
	pList->ResetContent();
	for (int j=0; j<(int)CDlgKingInfo::m_KingBaseInfo.vecChancellors.size();++j)
	{
		pList->AddString(CDlgKingInfo::m_KingBaseInfo.vecChancellors[j].name);
		pList->SetItemTextColor(pList->GetCount()-1,CDlgKingInfo::m_KingBaseInfo.vecChancellors[j].isOnline ? 
			                                        A3DCOLORRGB(152,227,252) : 
		                                            A3DCOLORRGB(127,127,127));
	}
	pList = (AUIListBox*)GetDlgItem("Lst_Member03");
	pList->ResetContent();
	for (int k=0; k<(int)CDlgKingInfo::m_KingBaseInfo.vecGuards.size();++k)
	{
		pList->AddString(CDlgKingInfo::m_KingBaseInfo.vecGuards[k].name);
		pList->SetItemTextColor(pList->GetCount()-1,CDlgKingInfo::m_KingBaseInfo.vecGuards[k].isOnline ? 
			                                        A3DCOLORRGB(152,227,252) : 
		                                            A3DCOLORRGB(127,127,127));
	}
	
	GetDlgItem("Btn_Authorize01")->Show(CDlgKingInfo::m_KingBaseInfo.nSelfTitle 
		                                == EKINGTITLE_KING);
	GetDlgItem("Btn_Authorize02")->Show(CDlgKingInfo::m_KingBaseInfo.nSelfTitle 
		                                == EKINGTITLE_KING);
	GetDlgItem("Btn_Authorize03")->Show(CDlgKingInfo::m_KingBaseInfo.nSelfTitle 
		                                == EKINGTITLE_KING);

	
}
//授予将军
void CDlgKingOfficialInfo::OnCommand_GrantGeneral(const char * szCommand)
{
    PopUpGrantOfficial(EKINGTITLE_GENERAL);
}
//授予宰相
void CDlgKingOfficialInfo::OnCommand_GrantChancellor(const char * szCommand)
{
    PopUpGrantOfficial(EKINGTITLE_MINISTER);
}
//授予亲卫
void CDlgKingOfficialInfo::OnCommand_GrantGuard(const char* szCommand)
{
   PopUpGrantOfficial(EKINGTITLE_GUARD);
}
//将军右键菜单
void CDlgKingOfficialInfo::OnEventRButtonUpLstGenerals(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if(CDlgKingInfo::m_KingBaseInfo.nSelfTitle != EKINGTITLE_KING)
		return;
    PopUpGrantRightMenu(wParam,lParam,EKINGTITLE_GENERAL,pObj);
}
//宰相右键菜单
void CDlgKingOfficialInfo::OnEventRButtonUpLstChancellors(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if(CDlgKingInfo::m_KingBaseInfo.nSelfTitle != EKINGTITLE_KING)
		return;
    PopUpGrantRightMenu(wParam,lParam,EKINGTITLE_MINISTER,pObj);
}
//亲卫右键菜单
void CDlgKingOfficialInfo::OnEventRButtonUpLstGuards(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if(CDlgKingInfo::m_KingBaseInfo.nSelfTitle != EKINGTITLE_KING)
		return;
    PopUpGrantRightMenu(wParam,lParam,EKINGTITLE_GUARD,pObj);
}
//弹出授予权利菜单
void CDlgKingOfficialInfo::PopUpGrantRightMenu(WPARAM wParam, LPARAM lParam, int vocation, AUIObject * pObj)
{
	AUIListBox* pLst = (AUIListBox*)pObj;
	POINT ptPos = pObj->GetPos();
	int nCurSel = pLst->GetCurSel();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	if (nCurSel>=0 && nCurSel<pLst->GetCount() && pLst->GetHitArea(x, y) == AUILISTBOX_RECT_TEXT)
	{
		x = GET_X_LPARAM(lParam) - p->X;
		y = GET_Y_LPARAM(lParam) - p->Y;
		PAUIDIALOG pDlg = NULL;
		pDlg = GetGameUIMan()->GetDialog("Win_KingOption");
		GetGameUIMan()->BringWindowToTop(pDlg);
		//int roleid = vocation == EKINGTITLE_GENERAL ?  
		//CDlgKingInfo::m_KingBaseInfo.vecGenerals[pLst->GetCurSel()].roleid : 
		//CDlgKingInfo::m_KingBaseInfo.vecChancellors[pLst->GetCurSel()].roleid;
		int roleid = 0;
		if(vocation == EKINGTITLE_GENERAL)
		{
		    roleid  = CDlgKingInfo::m_KingBaseInfo.vecGenerals[pLst->GetCurSel()].roleid;
		}
		else if(vocation == EKINGTITLE_MINISTER)
		{
		    roleid  = CDlgKingInfo::m_KingBaseInfo.vecChancellors[pLst->GetCurSel()].roleid;
		}
		else if (vocation == EKINGTITLE_GUARD)
		{
            roleid  = CDlgKingInfo::m_KingBaseInfo.vecGuards[pLst->GetCurSel()].roleid;
		}
		pDlg->SetData(roleid);
		pDlg->SetPosEx(x,y);
		pDlg->Show(true);
	}
}
//弹出职位授予界面
void CDlgKingOfficialInfo::PopUpGrantOfficial(int vocation)
{
	PAUIDIALOG pDlg = NULL;
	pDlg = GetGameUIMan()->GetDialog("Win_KingAuthorize");
	pDlg->SetData(vocation);
	pDlg->Show(true,true);
}
AUI_BEGIN_COMMAND_MAP(CDlgKingContinueAward, CDlgBase)
AUI_ON_COMMAND("Btn_GetAward",		    OnCommand_GetAward)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingContinueAward,CDlgBase)
AUI_END_EVENT_MAP()

void CDlgKingContinueAward::OnShowDialog()
{
   SetKingAwardInfo();
}
void CDlgKingContinueAward::RefreshKingAwardState()
{
   SetKingAwardInfo();
}
//连任奖励信息
void CDlgKingContinueAward::SetKingAwardInfo()
{
	DATA_TYPE DataType;
	int idEssence = g_pGame->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, DataType);
    while(idEssence)
	{
		if(DataType == DT_KING_WAR_CONFIG)
			break;
		idEssence = g_pGame->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, DataType);
	}
	KING_WAR_CONFIG *pData = (KING_WAR_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(idEssence, ID_SPACE_CONFIG, DataType);
	if (pData &&  DataType == DT_KING_WAR_CONFIG)
	{
		if(pData->king_continue_item)
		{
			CECIvtrItem *pItem = CECIvtrItem::CreateItem(pData->king_continue_item, 0, pData->king_continue_item_num);
			if(pItem)
			{
				AUIImagePicture* pObj =(AUIImagePicture*)GetDlgItem("Img_Award");
				CDlgKingInfo* pDlg = (CDlgKingInfo*)(GetGameUIMan()->GetDialog("Win_King_Info"));
				pDlg->SetImageInfoByItem(pItem,pObj,false);
				delete pItem;
				pItem =NULL;
			}
		}
	}
	ACString strTxt;
	strTxt.Format(GetGameUIMan()->GetStringFromTable(15628),CDlgKingInfo::m_KingBaseInfo.nContinue);
    ((PAUILABEL)(GetDlgItem("Txt_Time")))->SetText(strTxt);
	//星星的显示
	char szItem[40];
	/*int nContinue = 0;
	if(CDlgKingInfo::m_KingBaseInfo.nContinue > 0)
	{
	nContinue = CDlgKingInfo::m_KingBaseInfo.nContinue%6;
	if(nContinue == 0)
	nContinue = 6;
	}
	*/
	for (int i = 1; i<=6; ++i)
	{
		sprintf(szItem, "Img_Time%02d", i);
		AUIObject* pObj = GetDlgItem(szItem);
		if(CDlgKingInfo::m_KingBaseInfo.nContinue >= i)
		{
		   pObj->Show(true);
		}
		else
		{
		   pObj->Show(false);
		}
	}
	GetDlgItem("Btn_GetAward")->Enable(false);
	
	if(CDlgKingInfo::m_KingBaseInfo.nSelfTitle == EKINGTITLE_KING)
	{
		int nContinueAward = CDlgKingInfo::m_KingBaseInfo.nContinue/6;
		unsigned int nContinueMask = (unsigned int)(pow((float)2,(float)nContinueAward)-1);
		if(nContinueAward > 0 && (nContinueMask != (nContinueMask & CDlgKingInfo::m_KingBaseInfo.reward_mask)))
		{
			GetDlgItem("Btn_GetAward")->Enable(true);
		}
	}
}
void CDlgKingContinueAward::OnCommand_GetAward(const char * szCommand)
{
	GetGameSession()->c2s_SendCmdKingGetReward();
	GetDlgItem("Btn_GetAward")->Enable(false);

}

AUI_BEGIN_COMMAND_MAP(CDlgKingShop, CDlgBase)
AUI_ON_COMMAND("Btn_Buy*",		    OnCommand_Exchange)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingShop,CDlgBase)
AUI_ON_EVENT("*",				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_END_EVENT_MAP()

bool CDlgKingShop::OnInitDialog()
{

	m_pImagePresent = NULL;
	m_pSclItem    = (PAUISCROLL)GetDlgItem("Scl_Item");
	
	char szItem[40];
	
	for( int i = 0; i < CECDLGKINGSHOP_PACKMAX; ++i)
	{
		sprintf(szItem, "Item_%02d", i+1);
		m_pImgItem[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
		sprintf(szItem,"Lab_Score%02d",i+1);
		m_pLableItem[i] = (PAUILABEL)GetDlgItem(szItem);
		sprintf(szItem,"Txt_Score%02d",i+1);
		m_pTxtItem[i] = (PAUILABEL)GetDlgItem(szItem);
		sprintf(szItem,"Btn_Buy%02d",i+1);
		m_pButtonItem[i] = (AUIStillImageButton*)GetDlgItem(szItem);
		
	}

   SetBuyPointsInfo();
   return true;
}
bool CDlgKingShop::Release()
{
	A3DRELEASE(m_pImagePresent)
	return CDlgBase::Release();
}
void CDlgKingShop::OnShowDialog()
{
     SetScoreExchangeInfo();
}
void CDlgKingShop::SetBuyPointsInfo()
{
	//商城物品索引
	int itemindex = globaldata_getgshopitemindex_by_id(55898);
	if(itemindex == -1)
	     return;
	m_pImagePresent = new A2DSprite;
	if( !m_pImagePresent ) { AUI_ReportError(__LINE__, __FILE__); return;}
	GetGameUIMan()->AddImageListItem(m_pImagePresent, m_nHintImageIndex);

 
   vector<GSHOP_ITEM> *items = globaldata_getgshopitems();
   CECIvtrItem *pItem = CECIvtrItem::CreateItem((*items)[itemindex].id, 0, (*items)[itemindex].num);
   PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_00");

   if((*items)[itemindex].bHasPresent)
   {
	   m_pImagePresent->Release();
	   AString strName;
	   if (strlen((*items)[itemindex].Present_file_icon) > strlen("Surfaces\\"))
	   {
		   strName = (*items)[itemindex].Present_file_icon + strlen("Surfaces\\");
	   }
	   else
		   strName = "1\\没图标.dds";
	   bool bval = m_pImagePresent->Init(m_pA3DDevice, strName, AUI_COLORKEY);
	   if( !bval ) { AUI_ReportError(__LINE__, __FILE__); return;}
	}
   if(pItem)
   {
	   ACString strHint, strTemp;
	   strHint = (const wchar_t*)(*items)[itemindex].desc;
	   if ((*items)[itemindex].bHasPresent)
	   {
		   strHint += _AL("\f");
		   strTemp.Format(GetGameUIMan()->GetStringFromTable(9900), (*items)[itemindex].szPresentName);
		   strHint += strTemp;
		   EditBoxItemBase item(enumEIImage);
		   item.SetImageIndex(m_nHintImageIndex);
		   strHint += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
		   strTemp.Format(_AL("x%d\r"), (*items)[itemindex].Presentcount);
		   strHint += strTemp;
		   strHint += (const wchar_t*)(*items)[itemindex].Presentdesc;
		}
	   pObj->SetHint(strHint);

	   AUIOBJECT_SETPROPERTY p;
	   strcpy(p.fn, (*items)[itemindex].icon + strlen("Surfaces\\"));
	   pObj->SetProperty("Image File", &p);

	   delete pItem;
   }

}
void CDlgKingShop::SetScoreExchangeInfo()
{
	m_nFirstLine = 0;
    m_vecItems.clear();
	m_vecItemScores.clear();
	//从数据中读取物品数据
	DATA_TYPE DataType;
	int idEssence = g_pGame->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, DataType);
    while(idEssence)
	{
		if(DataType == DT_KING_WAR_CONFIG)
			break;
		idEssence = g_pGame->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, DataType);
	}
	KING_WAR_CONFIG *pData = (KING_WAR_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(idEssence, ID_SPACE_CONFIG, DataType);
	if (pData &&  DataType == DT_KING_WAR_CONFIG)
	{
		   //三月份统一显示为国家积分物品
	       //if(CDlgKingInfo::m_KingBaseInfo.nSelfTitle == EKINGTITLE_KING)
		   {
			   for(int i=0;i<20;++i)
			   {
				   CECIvtrItem *pItem = NULL;
				   int score = 0;
				   if (pData->king_shop[i].id)
				   {
                       pItem = CECIvtrItem::CreateItem(pData->king_shop[i].id, 0, pData->king_shop[i].num);
					   if(pItem)
					   {
						   m_vecItems.push_back(pItem);
						   m_vecItemScores.push_back(pData->king_shop[i].score);
					   }
				   }
				   
			   }
			   
		   }
		   /*else
		   {
			   for(int i=0;i<20;++i)
			   {
				   CECIvtrItem *pItem = NULL;
				   if (pData->normal_shop[i].id)
				   {
                       pItem = CECIvtrItem::CreateItem(pData->normal_shop[i].id, 0, pData->normal_shop[i].num);
					   if(pItem)
					   {
						   m_vecItems.push_back(pItem);
						   m_vecItemScores.push_back(pData->normal_shop[i].score);
					   }
				   }
				   
			   }
		   }*/
	}
	
	int nIvtrSize = m_vecItems.size();
	if(nIvtrSize > CECDLGKINGSHOP_PACKMAX)
	{
		m_pSclItem->SetScrollRange(0, (nIvtrSize - 1) / CECDLGKINGSHOP_PACKLINEMAX - 
			CECDLGKINGSHOP_PACKMAX / CECDLGKINGSHOP_PACKLINEMAX + 1);
		m_pSclItem->Show(true);
		m_pSclItem->SetBarLength(-1);
		m_pSclItem->SetScrollStep(1);
		m_pSclItem->SetBarLevel(0);
	}
	else
	{
		m_pSclItem->SetScrollRange(0, 0);
		m_pSclItem->SetBarLevel(0);
		m_pSclItem->Show(false);
	}
	
	RefreshShopInfo();
}
void CDlgKingShop::OnHideDialog()
{
	for (int i=0;i<(int)m_vecItems.size();++i)
	{
		CECIvtrItem* pItem = m_vecItems[i];
		if(pItem) delete pItem;
	}
	m_vecItems.clear();
	m_vecItemScores.clear();
}

void CDlgKingShop::OnCommand_Exchange(const char * szCommand)
{
	if(0 == stricmp(szCommand,"Btn_Buy")) 
	{
		OnCommand_BuyPoints(szCommand);
		return;
	}
	int index = atoi(szCommand+strlen("Btn_Buy"))-1;
	assert(index>=0 && index<CECDLGKINGSHOP_PACKMAX);
	CECIvtrItem* pItem = (CECIvtrItem*)(m_pImgItem[index]->GetDataPtr("ptr_CECIvtrItem"));
    assert(pItem != NULL);
	index = m_nFirstLine * CECDLGKINGSHOP_PACKLINEMAX +index;
	if(CDlgKingInfo::m_KingBaseInfo.nCountryScore < m_vecItemScores[index])
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(15634), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	int flag = CDlgKingInfo::m_KingBaseInfo.nSelfTitle == EKINGTITLE_KING ? 1 : 2;
	//发送兑换物品请求
	//弹出提示框
	m_iCurBuyIndex = index;
	ACString szText;
	szText.Format(GetStringFromTable(15633),m_vecItemScores[index],pItem->GetName());
	PAUIDIALOG pDlg = NULL;
	GetGameUIMan()->MessageBox("KingShopBuy",szText,MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160),&pDlg);
	pDlg->SetData(flag);
}
void CDlgKingShop::ProcessKingShopExchange(bool isAgree,int flag)
{
   if (isAgree)
   {
      GetGameSession()->c2s_CmdBuyKingdomItem(flag,m_iCurBuyIndex);
   }
}
void CDlgKingShop::OnCommand_BuyPoints(const char * szCommand)
{
	//商城物品索引
	int itemindex = globaldata_getgshopitemindex_by_id(55898);
	if(itemindex == -1)
	     return;
	vector<GSHOP_ITEM> *items = globaldata_getgshopitems();
	int price =(*items)[itemindex].buy[0].price;
    CECIvtrItem *pItem = NULL;
    pItem = CECIvtrItem::CreateItem((*items)[itemindex].id, 0, (*items)[itemindex].num);
   if(pItem)
   {
	   if(price > GetHostPlayer()->GetCash())
	   {
		   GetGameUIMan()->MessageBox("", GetStringFromTable(8397), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		    delete pItem;
		   return;
	   }

	   {
		  ACHAR szText[40];
		  PAUIOBJECT pEdit = GetGameUIMan()->m_pDlgInputGNO->GetDlgItem("DEFAULT_Txt_No.");
		  pEdit->SetData(GetHostPlayer()->GetCash() / price );
		  GetGameUIMan()->m_pDlgInputGNO->SetType(CDlgInputNO::INPUTNO_QSHOPITEM_BUY);
		  GetGameUIMan()->m_pDlgInputGNO->Show(true, true);
		  a_sprintf(szText, _AL("%d"), 1);
		  pEdit->SetText(szText);
		  GetGameUIMan()->m_pDlgInputGNO->SetData(price);
		  GetGameUIMan()->m_pDlgInputGNO->SetText(pItem->GetName());
		  PAUIOBJECT pObj = GetGameUIMan()->m_pDlgInputGNO->GetDlgItem("Txt_Confirm");
		  pObj->SetData(itemindex);
		  pObj->SetDataPtr((void*)(*items)[itemindex].id);
		  GetGameUIMan()->m_pDlgInputGNO->GetDlgItem("Txt_Off")->SetData(((*items)[itemindex].buy[0].price)*((*items)[itemindex].bonus)/100);
	   }
	   delete pItem;

   }
   
}
void CDlgKingShop::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int zDelta = (short)HIWORD(wParam);
	if( zDelta > 0 )
		zDelta = 1;
	else
		zDelta = -1;
	if( m_pSclItem->IsShow() )
		m_pSclItem->SetBarLevel(m_pSclItem->GetBarLevel() - zDelta);

}
void CDlgKingShop::OnTick()
{
	if( m_nFirstLine != m_pSclItem->GetBarLevel() )
	{
		m_nFirstLine = m_pSclItem->GetBarLevel();
		RefreshShopInfo();
	}
}
void CDlgKingShop::RefreshShopInfo()
{
	PAUIIMAGEPICTURE pObj = NULL;
	CECIvtrItem *pItem = NULL;
	
	int nStart = m_nFirstLine * CECDLGKINGSHOP_PACKLINEMAX;
	
	for( int i = 0; i < CECDLGKINGSHOP_PACKMAX; i++ )
	{
		m_pLableItem[i]->Show(false);
		m_pTxtItem[i]->Show(false);
		m_pButtonItem[i]->Show(false);
		m_pImgItem[i]->Show(false);
		
		pObj = m_pImgItem[i];
		pObj->ClearCover();
		pObj->SetDataPtr(NULL);
		pObj->SetHint(_AL(""));
		pObj->SetText(_AL(""));
		pObj->SetColor(A3DCOLORRGB(255, 255, 255));
		
		if( i + nStart < (int)m_vecItems.size() )
		{	
			m_pButtonItem[i]->Show(true);
			m_pLableItem[i]->Show(true);
			m_pTxtItem[i]->Show(true);
			m_pImgItem[i]->Show(true);
            pItem = m_vecItems[i+nStart];
			CDlgKingInfo* pDlg = (CDlgKingInfo*)(GetGameUIMan()->GetDialog("Win_King_Info"));
			pDlg->SetImageInfoByItem(pItem,pObj);
			ACHAR szText[100];
			a_sprintf(szText, _AL("%d"), m_vecItemScores[i + nStart]);
			m_pTxtItem[i]->SetText(szText);
            //三月版本，只有国王可以使用
			if(CDlgKingInfo::m_KingBaseInfo.nSelfTitle != EKINGTITLE_KING)
               m_pButtonItem[i]->Enable(false);
			else
               m_pButtonItem[i]->Enable(true);
			
		}
		
	}
}

void CDlgKingShop::RefreshKingShopState()
{
    RefreshShopInfo();
}


AUI_BEGIN_COMMAND_MAP(CDlgKingAnnounce, CDlgBase)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingAnnounce,CDlgBase)
AUI_END_EVENT_MAP()

void CDlgKingAnnounce::OnShowDialog()
{
	m_dwStartTime = GetTickCount();
}
void CDlgKingAnnounce::OnTick() 
{
	DWORD dwTickTime  = GetTickCount() - m_dwStartTime;
	if(dwTickTime >= 10000)
		this->Show(false);
}
void CDlgKingAnnounce::SetMessage(const ACString& message)
{
   ((PAUIVERTICALTEXT)GetDlgItem("Message"))->SetText(message);
   if(GetGameUIMan()->GetDialog("Win_King_Info")->IsShow())
   {
	 CDlgKingInfo::m_KingBaseInfo.strSlogan = message;
	 //圣旨内容
	((PAUILABEL)(GetGameUIMan()->GetDialog("Win_King_Info")->GetDlgItem("Txt_KingChat")))->SetText(message);

   }
}
AUI_BEGIN_COMMAND_MAP(CDlgKingBathInvite, CDlgBase)
AUI_ON_COMMAND("Btn_Invite",		    OnCommand_InviteBath)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingBathInvite,CDlgBase)
AUI_END_EVENT_MAP()
bool CDlgKingBathInvite::OnInitDialog()
{
	 m_StartTime  = 0;
	 m_nLeftTimes = 0;
	 m_dwLastTime = 0;
	 m_pLabel = (AUILabel*)GetDlgItem("Txt_Time");
	 m_pBtn = (AUIStillImageButton*)GetDlgItem("Btn_Invite");
	 return true;
}
int  CDlgKingBathInvite::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if(strMsg == "enter")
	{
	    m_nLeftTimes = wParam;
		m_dwLastTime = 0;

		if(!(GetGameUIMan()->GetDialog("Win_King_ShowerInfo")->IsShow()))
            GetGameUIMan()->GetDialog("Win_King_ShowerInfo")->Show(true);
	}
	else if(strMsg == "start")
	{
		m_StartTime = GetTickCount();
		if(m_nLeftTimes <= 0) return 0;
		--m_nLeftTimes;
		m_dwLastTime = GetTickCount();
	}
	else if(strMsg = "end")
	{
		if(m_nLeftTimes > 0)
		{ 
			ACString strMsg;
			strMsg.Format(GetGameUIMan()->GetStringFromTable(15675),m_nLeftTimes);
			GetGameUIMan()->AddChatMessage(strMsg, GP_CHAT_MISC);
		}
	}
	else if(strMsg == "leave")
	{
	}
    return 0;
}
DWORD CDlgKingBathInvite::GetBathElapse()
{
   return GetTickCount()-m_StartTime;
}
void CDlgKingBathInvite::OnTick()
{
	bool IsEable = true;
	int  iMsg = -1;

	assert(GetData() != 0);
	CECElsePlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetElsePlayer(GetData());
	//assert(pPlayer != NULL);
	if(pPlayer == NULL)
	{
	   this->SetData(0);
       this->Show(false);
	   return;
	}
	A3DVECTOR3 vDist = pPlayer->GetServerPos() - GetHostPlayer()->GetPos();
	//距离太远
	if (vDist.Magnitude()  >= 2.8f)
	{
        IsEable = false;
		iMsg = 15672;
	}

	//变身状态下不能被邀请
	if(pPlayer->IsShapeChanged() && pPlayer->GetShapeChangedId() != SHAPE_HUMAN)
	{
	    IsEable = false;
		iMsg = 15684;
	}

	//冷却
	if(m_dwLastTime != 0)
	{
		DWORD dwTickTime  = GetTickCount() - m_dwLastTime;
		if(dwTickTime <= 3*60*1000)
		{
			IsEable = false;
			iMsg = 15673;
		}
		else
		{
			m_dwLastTime = 0;
		}
	}
	//使用中
	//if(GetHostPlayer()->IsBathing())
	//{
	//	IsEable = false;
	//	iMsg = 15674;
		
	//}
	//次数用完
	if(m_nLeftTimes <= 0)
	{
		IsEable = false;
		iMsg = 15671;
	}

	m_pBtn->Enable(IsEable);
	if(iMsg != -1)
	 m_pBtn->SetHint(GetGameUIMan()->GetStringFromTable(iMsg));
	else
	 m_pBtn->SetHint(_AL(""));
	ACString strMsg;
    strMsg.Format(_AL("%d/5"),m_nLeftTimes);
	m_pLabel->SetText(strMsg);
}
void CDlgKingBathInvite::OnCommand_InviteBath(const char* szCommand)
{
	//判断变身状态特殊处理
	if(GetHostPlayer()->IsShapeChanged() && GetHostPlayer()->GetShapeChangedId() != SHAPE_HUMAN)
	{
        GetGameUIMan()->MessageBox("", GetStringFromTable(15685), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
   //发送搓澡请求
	if (!GetHostPlayer()->CanDo(CECHostPlayer::CANDO_BATH))
	    return ;
	GetGameSession()->c2s_SendCmdKingBathInvite(GetData());
	//m_dwLastTime = GetTickCount();
}
void CDlgKingBathInvite::OnCommand_Close(const char* szCommand)
{
	this->Show(false);
}

AUI_BEGIN_COMMAND_MAP(CDlgKingTask, CDlgBase)
AUI_ON_COMMAND("Btn_Release*",		    OnCommand_Dliver)
AUI_ON_COMMAND("Btn_Get*",		    OnCommand_Accept)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingTask,CDlgBase)
AUI_END_EVENT_MAP()
bool CDlgKingTask::OnInitDialog()
{
	m_TaskType = 0;
	m_TaskStatus = -1;
    return true;
}
void CDlgKingTask::OnShowDialog()
{
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();	
	CECTaskInterface *pTaskInterafce = GetHostPlayer()->GetTaskInterface();
	ATaskTempl *pTemp = NULL;
	PAUITEXTAREA pTxt_Content = NULL;
	
	pTemp = pMan->GetTaskTemplByID(KING_WHITE_TASK);
	if( !pTemp )
		return ;
	pTxt_Content = (PAUITEXTAREA)GetDlgItem("Txt_01");
	pTxt_Content->SetText(GetGameUIMan()->m_pDlgTask->FormatTaskText(pTemp->GetDescription()));
	
	pTemp = pMan->GetTaskTemplByID(KING_GREEN_TASK);
	if( !pTemp )
		return ;
	pTxt_Content = (PAUITEXTAREA)GetDlgItem("Txt_02");
	pTxt_Content->SetText(GetGameUIMan()->m_pDlgTask->FormatTaskText(pTemp->GetDescription()));
	
	pTemp = pMan->GetTaskTemplByID(KING_BLUE_TASK);
	if( !pTemp )
		return ;
	pTxt_Content = (PAUITEXTAREA)GetDlgItem("Txt_03");
	pTxt_Content->SetText(GetGameUIMan()->m_pDlgTask->FormatTaskText(pTemp->GetDescription()));
	
	pTemp = pMan->GetTaskTemplByID(KING_ORANGE_TASK);
	if( !pTemp )
		return ;
	pTxt_Content = (PAUITEXTAREA)GetDlgItem("Txt_04");
	pTxt_Content->SetText(GetGameUIMan()->m_pDlgTask->FormatTaskText(pTemp->GetDescription()));
}
void CDlgKingTask::OnCommand_Dliver(const char * szCommand)
{
    int type = atoi(szCommand+strlen("Btn_Release"));
	GetGameSession()->c2s_SendCmdDeliverKingTask(type);
	//GetGameSession()->kingwar_getkingtask(GetHostPlayer()->GetCharacterID());
}
void CDlgKingTask::OnCommand_Accept(const char * szCommand)
{
    int type = atoi(szCommand+strlen("Btn_Get"));
	GetGameSession()->c2s_SendCmdReceiveKingTask(type);
	//GetGameSession()->kingwar_getkingtask(GetHostPlayer()->GetCharacterID());
}
void CDlgKingTask::OnTick()
{
	if( 0 == stricmp(GetName(), "Win_King_SubRight04") )
	{
		ACString strTxt;
		int nCash = GetHostPlayer()->GetCash();
		strTxt.Format(GetGameUIMan()->GetStringFromTable(1403), nCash / 100, nCash % 100);
		((PAUILABEL)GetDlgItem("Txt_Money"))->SetText(strTxt);
    }
	if(m_TaskStatus == 1 && m_TaskType >= 1 && m_TaskType <= 4)
	{
		ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();	
	    CECTaskInterface *pTaskInterafce = GetHostPlayer()->GetTaskInterface();
		ActiveTaskList* pTaskActive =static_cast<ActiveTaskList*>(pTaskInterafce->GetActiveTaskList());
		ATaskTempl *pTemp = pMan->GetTaskTemplByID(GetTaskIdByType(m_TaskType));
		assert(pTemp != NULL);
		AString strName;
		strName.Format("Btn_Get%02d",m_TaskType);
		GetDlgItem(strName)->Enable(NULL == pTaskActive->GetEntry(GetTaskIdByType(m_TaskType)) &&
			                        pTemp->CheckFinishCount(pTaskInterafce) == 0);
	}
	GetDlgItem("Lab_QuestStart")->Show(m_TaskStatus == 1);
	GetDlgItem("Txt_QuestStart")->Show(m_TaskStatus == 1);
	GetDlgItem("Lab_QuestEnd")->Show(m_TaskStatus == 1);
	GetDlgItem("Txt_QuestEnd")->Show(m_TaskStatus == 1);
	

}
void CDlgKingTask::RefreshTaskState(int status,int type,int times,int issuetime)
{
	m_TaskType = type;
	m_TaskStatus = status;
	if( 0 == stricmp(GetName(), "Win_King_SubRight04") )
	{
		ACString strTxt;
		strTxt.Format(_AL("%d"),times);
		((PAUILABEL)GetDlgItem("Txt_Num"))->SetText(strTxt);

		GetDlgItem("Btn_Release01")->Show(status == 0);
		GetDlgItem("Btn_Release02")->Show(status == 0);
		GetDlgItem("Btn_Release03")->Show(status == 0);
		GetDlgItem("Btn_Release04")->Show(status == 0);
		GetDlgItem("Btn_Get01")->Show(status != 0);
		GetDlgItem("Btn_Get02")->Show(status != 0);
		GetDlgItem("Btn_Get03")->Show(status != 0);
		GetDlgItem("Btn_Get04")->Show(status != 0);
		GetDlgItem("Btn_Get01")->Enable(status == 1 && type == 1);
		GetDlgItem("Btn_Get02")->Enable(status == 1 && type == 2);
		GetDlgItem("Btn_Get03")->Enable(status == 1 && type == 3);
		GetDlgItem("Btn_Get04")->Enable(status == 1 && type == 4);
		
	}
	else if(0 == stricmp(GetName(),"Win_King_SubRight05"))
	{
		GetDlgItem("Btn_Get01")->Enable(status == 1 && type == 1);
		GetDlgItem("Btn_Get02")->Enable(status == 1 && type == 2);
		GetDlgItem("Btn_Get03")->Enable(status == 1 && type == 3);
		GetDlgItem("Btn_Get04")->Enable(status == 1 && type == 4);
	}

	if(m_TaskStatus == 1)
	{
		ACString strText;
		long t = issuetime;
        t -= g_pGame->GetTimeZoneBias() * 60;
		tm gtime = *gmtime((const time_t*)&t);
		strText.Format(_AL("%d-%02d-%02d-%02d:%02d"),gtime.tm_year+1900,gtime.tm_mon+1,gtime.tm_mday,gtime.tm_hour,gtime.tm_min);
	    GetDlgItem("Txt_QuestStart")->SetText(strText);
		t = issuetime + 6*24*60*60;
		t -= g_pGame->GetTimeZoneBias() * 60;
		gtime = *gmtime((const time_t*)&t);
		strText.Format(_AL("%d-%02d-%02d-%02d:%02d"),gtime.tm_year+1900,gtime.tm_mon+1,gtime.tm_mday,gtime.tm_hour,gtime.tm_min);	
        GetDlgItem("Txt_QuestEnd")->SetText(strText);	
	}
}

AUI_BEGIN_COMMAND_MAP(CDlgKingBathTip, CDlgBase)
AUI_ON_COMMAND("IDCANCEL",		    OnCommand_Close)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgKingBathTip,CDlgBase)
AUI_END_EVENT_MAP()
void CDlgKingBathTip::OnTick()
{
   if(GetHostPlayer()->GetBathType() == -1)
	   this->Show(false);
}
void CDlgKingBathTip::OnCommand_Close(const char* szCommand)
{
   this->Show(false);
}
