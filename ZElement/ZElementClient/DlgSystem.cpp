// File		: DlgSystem.cpp
// Creator	: Xiao Zhou
// Date		: 2005/8/15

#include "DlgSystem.h"
#include "DlgSystem2.h"
#include "DlgSystem3.h"
#include "DlgSystem4.h"
#include "DlgCharacter.h"
#include "DlgInventory.h"
#include "DlgBooth.h"
#include "DlgFriendList.h"
#include "DlgPetPackage.h"
#include "DlgSkillMan.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameRecord.h"
#include "EC_HostPlayer.h"
#include "EC_Configs.h"
#include "DlgTrade.h"
#include "DlgShop.h"
#include "DlgQShop.h"
#include "DlgSkill.h"
#include "DlgTask.h"
#include "DlgMiniMap.h"
#include "DlgDynamicSystem.h"
#include "DlgKingWar.h"
#include "DlgCommonDlg.h"
#include "DlgDigTreas.h"
#include "DlgQuickBar.h"
#include "EC_HelpGraphGuide.h"
#include "EC_NewHelpGuide.h"
#include "globaldataman.h"
#include "LuaEvent.h"

extern game_record gr;

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSystem, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCANCAL)
AUI_ON_COMMAND("winventory",	OnCommandInventory)
AUI_ON_COMMAND("wquest",		OnCommandQuest)
AUI_ON_COMMAND("wcharacter",	OnCommandCharacter)
AUI_ON_COMMAND("wskill",		OnCommandSkill)
AUI_ON_COMMAND("wcommunity",	OnCommandCommunity)
AUI_ON_COMMAND("wmain2",		OnCommandMain2)
AUI_ON_COMMAND("wmain3",		OnCommandMain3)
AUI_ON_COMMAND("mini",			OnCommandMini)
AUI_ON_COMMAND("Btn_Minimize",	OnCommandMinimize)
AUI_ON_COMMAND("Btn_Main",		OnCommandMain)
AUI_ON_COMMAND("Btn_Qshop",		OnCommand_QShop)
AUI_ON_COMMAND("Btn_Talisman",	OnCommand_Talisman)
AUI_ON_COMMAND("Btn_All",		OnCommand_All)
AUI_ON_COMMAND("Btn_System",	OnCommand_System)

AUI_END_COMMAND_MAP()


CDlgSystem::CDlgSystem()
{
}

CDlgSystem::~CDlgSystem()
{
}

bool CDlgSystem::OnInitDialog()
{
	m_pChktemp = (PAUICHECKBOX)GetDlgItem("Chk_temp");
	m_pBtnInventory = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Inventory");
	m_pBtnCharacter = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Character");
	m_pBtnFriend = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Friend");
	m_pBtnFunction = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Function");
	m_pBtnSystem = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_System");
	m_pBtnShop= (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Shop");
	m_pFrameSystem2 = (PAUIEDITBOX)GetDlgItem("Frame_System2");
	
	if(m_szName=="Win_Main")
	{
		GetDlgItem("Shape_1")->Show(false);
		GetDlgItem("Txt_1")->Show(false);
		GetDlgItem("Img_1")->Show(false);
		GetDlgItem("Shape_2")->Show(false);
		GetDlgItem("Txt_2")->Show(false);
		GetDlgItem("Img_2")->Show(false);
		GetDlgItem("Shape_3")->Show(false);
		GetDlgItem("Txt_3")->Show(false);
		GetDlgItem("Img_3")->Show(false);
	}

	return true;
}

void CDlgSystem::OnShowDialog()
{
	/* if(stricmp(GetName(),"Win_Main") == 0)
	 {
		PAUIDIALOG pDlgClose = GetGameUIMan()->GetDialog("Win_MainClose");
		SIZE s = GetSize();
		A3DRECT rc = pDlgClose->GetRect();
		SetPosEx(rc.left - s.cx, rc.bottom - s.cy + 1);

	 }*/
}
void CDlgSystem::Resize(A3DRECT rcOld, A3DRECT rcNew)
{
	CDlgBase::Resize(rcOld,rcNew);
	/*if(stricmp(GetName(),"Win_Main") == 0)
	{
		PAUIDIALOG pDlgClose = GetGameUIMan()->GetDialog("Win_MainClose");
		SIZE s = GetSize();
		A3DRECT rc = pDlgClose->GetRect();
		SetPosEx(rc.left - s.cx, rc.bottom - s.cy + 1);

	}*/
}
void CDlgSystem::OnTick()
{
	if (stricmp(m_szName, "Win_Main")==0)
	{
		CDlgInventory *pDlgInventory = GetGameUIMan()->m_pDlgInventory;
		m_pBtnInventory->SetFlash(!pDlgInventory->IsShow() && pDlgInventory->EmphasizeButton());

        PAUIDIALOG pDlgClose = GetGameUIMan()->GetDialog("Win_MainClose");
		if(pDlgClose && pDlgClose->IsShow())
		{
		   GetGameUIMan()->InsertDialogBefore(pDlgClose,this);
		}

	}
}

void CDlgSystem::OnCommandCANCAL(const char *szCommand)
{
	GetGameUIMan()->RespawnMessage();
}

void CDlgSystem::OnCommandQuest(const char *szCommand)
{
	if( GetGameUIMan()->m_pDlgTrade->IsShow() ||
		GetGameUIMan()->m_pDlgShop->IsShow() )
		return;

	GetGameUIMan()->m_pDlgTask->ChangeShowStatus();
	LuaEvent::FirstViewTask();
	GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_3_4_TASK_UI);
	GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_11_1_TASK_UI);
}

void CDlgSystem::OnCommandInventory(const char *szCommand)
{
	if( GetGameUIMan()->m_pDlgTrade->IsShow() ||
		GetGameUIMan()->m_pDlgShop->IsShow() ||
		GetGameUIMan()->m_bReplayMode && !gr.get_replay_setting(REPLAY_INVENTORY) )
		return;
	CDlgInventory* pDlg = GetGameUIMan()->m_pDlgInventory;

	if(pDlg)
	{
		if( pDlg->IsShow() )
		{
			pDlg->Show(false);
			//GetGameUIMan()->m_pDlgCharacter->Show(false);
		}
		else
		{
			pDlg->Show(true);
			LuaEvent::FirstOpenPack();
			GetGameRun()->GetHelpGraphGuide()->OpenInv();

			{
				NewHelpGuide::StartGuide_Step_2(GUIDE_HELP_EQUIPWEAPON_3);
				NewHelpGuide::StartGuide_Step_2(GUIDE_HELP_USEGIFT_2);
				NewHelpGuide::StartGuide_Step_2(GUIDE_HELP_EQUIPTALISMAN_4);
				NewHelpGuide::StartGuide_Step_10(GUIDE_HELP_PET_11);
			
			}
//			if( !GetGameUIMan()->m_pDlgBooth2->IsShow() && 
//				(!GetGameUIMan()->m_bReplayMode || gr.get_replay_setting(REPLAY_SELF_INFO)) )
//			{
//				GetGameUIMan()->m_pDlgCharacter->Show(true);
//				GetGameSession()->c2s_CmdGetExtProps();
//			}
		}
	}
}

void CDlgSystem::OnCommandCommunity(const char *szCommand)
{
	if( GetGameUIMan()->m_pDlgTrade->IsShow() ||
		GetGameUIMan()->m_pDlgShop->IsShow() )
		return;
	
	CDlgSystem4* pDlg = GetGameUIMan()->m_pDlgSystem4;
	PAUIDIALOG pShowOld = GetGameUIMan()->m_pShowOld;
	if(pDlg)
	{
		if( pDlg->IsShow() )
		{
			pDlg->Show(false);
			pShowOld = NULL;
		}
		else
		{
			if( pShowOld ) pShowOld->Show(false);

			SIZE s = pDlg->GetSize();
			A3DRECT rc = GetRect();

			// old : pDlg->SetPos(rc.right - s.cx, rc.top - s.cy + 1);
			pDlg->SetPosEx(rc.left - s.cx, rc.bottom - s.cy + 1);

			pDlg->Show(true);
			GetGameUIMan()->m_pShowOld = pDlg;
		}
	}
}

void CDlgSystem::OnCommandCharacter(const char *szCommand)
{
	if( GetGameUIMan()->m_pDlgTrade->IsShow() ||
		GetGameUIMan()->m_pDlgShop->IsShow() ||
		GetGameUIMan()->m_bReplayMode && !gr.get_replay_setting(REPLAY_SELF_INFO) )
		return;
	
	PAUIDIALOG pDlg = GetGameUIMan()->m_pDlgCharacter;
	
	if( pDlg != NULL && !pDlg->IsShow() )
		GetGameSession()->c2s_CmdGetExtProps();

	pDlg = GetGameUIMan()->GetDialog("Win_Character");
	if (pDlg->IsShow())
	{
		pDlg->Show(false);
		GetGameUIMan()->GetDialog("Win_Char_Nature")->Show(false);
		GetGameUIMan()->GetDialog("Win_Equip_Rune")->Show(false);
		return;
	}
	pDlg = GetGameUIMan()->GetDialog("Win_Credit");
	if (pDlg->IsShow())
	{
		pDlg->Show(false);
		return;
	}
	pDlg = GetGameUIMan()->GetDialog("Win_Char_TitleBox");
	if (pDlg->IsShow())
	{
		pDlg->Show(false);
		return;
	}
	pDlg = GetGameUIMan()->GetDialog("Win_CharInfo");
	if (pDlg->IsShow())
	{
		pDlg->Show(false);
		return;
	}
	pDlg = GetGameUIMan()->GetDialog("Win_CharOther");
	if (pDlg->IsShow())
	{
		pDlg->Show(false);
		return;
	}
	pDlg = GetGameUIMan()->GetDialog("Win_Fengshen_Soul");
	if (pDlg->IsShow())
	{
		pDlg->Show(false);
		return;
	}
	GetGameUIMan()->m_pDlgCharacter->Show(true);
	GetGameUIMan()->GetDialog("Win_Char_Nature")->Show(true);
}

void CDlgSystem::OnCommandSkill(const char *szCommand)
{
	if( GetGameUIMan()->m_pDlgTrade->IsShow() ||
		GetGameUIMan()->m_pDlgShop->IsShow() ||
		GetGameUIMan()->m_bReplayMode && !gr.get_replay_setting(REPLAY_SKILL) )
		return;
	
	CDlgSkillMan *pSkillMan = (CDlgSkillMan*)GetGameUIMan()->GetDialog("Win_Skillzx2");
	if (pSkillMan->IsShow())
	{
		pSkillMan->OnCommand_close("");
	}
	else
	{
		pSkillMan->Show(true);
		GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_9_1_SKILL_ICON);
		NewHelpGuide::StartGuide_SkillAddPoint_2();
	}
// 	CDlgSkill* pDlgSkill	  = GetGameUIMan()->m_pDlgSkill;
// 	CDlgSkill* pDlgSkillOther = GetGameUIMan()->m_pDlgSkillOther;
// 	CDlgSkill* pDlgSkillSpec  = GetGameUIMan()->m_pDlgSkillSpec;
// 	if(pDlgSkill && pDlgSkillOther && pDlgSkillSpec)
// 	{
// 		if( pDlgSkill->IsShow() )
// 			pDlgSkill->OnCommandCancel("");
// 		else if( pDlgSkillOther->IsShow() )
// 			pDlgSkillOther->OnCommandCancel("");
// 		else if( pDlgSkillSpec->IsShow() )
// 			pDlgSkillSpec->OnCommandCancel("");
// 		else
// 		{
// 			pDlgSkill->Show(true);
// 			pDlgSkill->OnCommandSkillTree("");
// 		}
// 	}
}

void CDlgSystem::OnCommandMain2(const char *szCommand)
{
	if( GetGameUIMan()->m_pDlgTrade->IsShow() ||
		GetGameUIMan()->m_pDlgShop->IsShow() )
		return;

	CDlgSystem2* pDlg = GetGameUIMan()->m_pDlgSystem2;
	PAUIDIALOG pShowOld = GetGameUIMan()->m_pShowOld;
	if(pDlg)
	{
		if( pDlg->IsShow() )
		{
			pDlg->Show(false);
			pShowOld = NULL;
		}
		else
		{
			if( pShowOld ) pShowOld->Show(false);

			SIZE s = pDlg->GetSize();
			A3DRECT rc = GetRect();

			// old : pDlg->SetPos(rc.right - s.cx, rc.top - s.cy + 1);
			pDlg->SetPosEx(rc.left - s.cx, rc.bottom - s.cy + 1);

			pDlg->Show(true);
			GetGameUIMan()->m_pShowOld = pDlg;
		}
	}
}

void CDlgSystem::OnCommandMain3(const char *szCommand)
{
	if( GetGameUIMan()->m_pDlgTrade->IsShow() ||
		GetGameUIMan()->m_pDlgShop->IsShow() )
		return;
	
	CDlgSystem3* pDlg = GetGameUIMan()->m_pDlgSystem3;
	PAUIDIALOG pShowOld = GetGameUIMan()->m_pShowOld;
	if(pDlg)
	{
		if( pDlg->IsShow() )
		{
			pDlg->Show(false);
			pShowOld = NULL;
		}
		else
		{
			if( pShowOld ) pShowOld->Show(false);

			SIZE s = pDlg->GetSize();
			A3DRECT rc = GetRect();

			// old : pDlg->SetPos(rc.right - s.cx, rc.top - s.cy + 1);
			pDlg->SetPosEx(rc.left - s.cx, rc.bottom - s.cy + 1);

			pDlg->Show(true);
			GetGameUIMan()->m_pShowOld = pDlg;

		}
	}
}

void CDlgSystem::OnCommandMini(const char *szCommand)
{
	if( GetGameUIMan()->m_pDlgTrade->IsShow() ||
		GetGameUIMan()->m_pDlgShop->IsShow() )
		return;

	m_pBtnInventory->Show(!m_pChktemp->IsChecked());
	m_pBtnCharacter->Show(!m_pChktemp->IsChecked());
	m_pBtnFriend->Show(!m_pChktemp->IsChecked());
	m_pBtnSystem->Show(!m_pChktemp->IsChecked());
	m_pBtnFunction->Show(!m_pChktemp->IsChecked());
	m_pBtnShop->Show(!m_pChktemp->IsChecked());
	m_pFrameSystem2->Show(!m_pChktemp->IsChecked());
}

void CDlgSystem::OnCommandMinimize(const char *szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Main2")->Show(false);
	GetGameUIMan()->GetDialog("Win_Main3")->Show(false);
	GetGameUIMan()->GetDialog("Win_Main4")->Show(false);
	GetGameUIMan()->GetDialog("Win_MainClose")->Show(true);
}

void CDlgSystem::OnCommandMain(const char *szCommand)
{
	//Show(false);
	//GetGameUIMan()->GetDialog("Win_Main")->Show(true);

	bool bShow = GetGameUIMan()->GetDialog("Win_Main")->IsShow();
	GetGameUIMan()->GetDialog("Win_Main")->Show(!bShow);
    
	if(!bShow)
	{
		NewHelpGuide::StartGuide_Step_7(GUIDE_HELP_PET_11);
	}
}

void CDlgSystem::OnCommand_QShop(const char * szCommand)
{
    GetGameUIMan()->m_pDlgQuickBar8->OnCommand_QShop("wshop");
}
void CDlgSystem::OnCommand_Talisman(const char* szCommand)
{
	EC_GAME_SETTING setting = GetGame()->GetConfigs()->GetGameSettings();
	setting.bShowTalismanDlg = !setting.bShowTalismanDlg;
	GetGame()->GetConfigs()->SetGameSettings(setting);
}
void CDlgSystem::OnCommand_System(const char * szCommand)
{
	if( GetGameUIMan()->m_pDlgTrade->IsShow() ||
		GetGameUIMan()->m_pDlgShop->IsShow() )
		return;
    bool bShow = GetGameUIMan()->GetDialog("Winmain_System")->IsShow();
	GetGameUIMan()->GetDialog("Winmain_System")->Show(!bShow);
}
void CDlgSystem::OnCommand_All(const char * szCommand)
{
	if( GetGameUIMan()->m_pDlgTrade->IsShow() ||
		GetGameUIMan()->m_pDlgShop->IsShow() )
		return;
	bool bShow = GetGameUIMan()->GetDialog("Win_WinmainAll")->IsShow();
	GetGameUIMan()->GetDialog("Win_WinmainAll")->Show(!bShow);

	if(!bShow)
	{
		NewHelpGuide::StartGuide_Step_8(GUIDE_HELP_PET_11);
	}
}
AUI_BEGIN_COMMAND_MAP(CDlgSystemAll, CDlgBase)
AUI_ON_COMMAND("Btn_*",			OnCommandFunction)
AUI_ON_COMMAND("Battle_Dungeon_6V6_Min",			OnCommandFunction)
AUI_END_COMMAND_MAP()
bool CDlgSystemAll::OnInitDialog()
{
	GetDlgItem("Btn_Group")->Show(false);
	GetDlgItem("Btn_6V6")->Show(false);
	return true;
}
void CDlgSystemAll::OnCommandFunction(const char * szCommand)
{
   AString strFun = szCommand ;
   
   CDlgSystem2* pSysTem2 = GetGameUIMan()->m_pDlgSystem2;
   CDlgSystem3* pSystem3 = GetGameUIMan()->m_pDlgSystem3;
   CDlgSystem4* pSystem4 = GetGameUIMan()->m_pDlgSystem4;
   CDlgMiniMap* pMiniMap = GetGameUIMan()->m_pDlgMiniMap;
   //组队
   if(strFun == "Btn_Team")
   {
		pSystem4->OnCommandTeam("");
   }	
   //师徒
   else if(strFun == "Btn_Teacher")
   {
		pSystem4->OnCommandTeacher("");
   }
   //好友
   else if(strFun == "Btn_Friend")
   {
		pSystem4->OnCommandFriend("");
   }
   //征友
   else if(strFun == "Btn_FriendSeeking")
   {
       pSystem4->OnCommandFriendSeek("");
   }
   //家园
   else if(strFun == "Btn_Garden")
   {
       pSystem4->OnCommandGarden("");
   }
   //小游戏
   else if(strFun == "Btn_Game")
   {
       pSystem4->OnCommandGameCasual("");
   }
   //家族、帮会
   else if(strFun == "Btn_Guild")
   {
       pSystem4->OnCommandFaction("");
   }
   //成就
   else if(strFun == "Btn_Success")
   {
       pSysTem2->OnCommandAchievement("");
   }
   //排行榜
   else if(strFun == "Btn_Rank")
   {
		pSysTem2->OnCommandRankList("");
   }
   //宠物
   else if(strFun == "Btn_Pet")
   {
        pSysTem2->OnCommandPet("");
   }
   //生产
   else if(strFun == "Btn_Produce")
   {
        pSysTem2->OnCommandProduce("");
   }
   //动作
   else if(strFun == "Btn_Action")
   {
        pSysTem2->OnCommandAction("");
   }
   //电台
   else if(strFun == "Btn_Radio")
   {
        pSysTem2->OnCommandBroadcast("");
   }
   //鸿雁传书
   else if(strFun == "Btn_MailtoFriend")
   {
       pSysTem2->OnCommandMailToFriend("");
   }
   //爬塔
   else if(strFun == "Btn_Battle_Dungeon_Tower")
   {
       pMiniMap->OnCommand_Tower("");
   }
   //炼火炉
   else if (strFun == "Btn_AttiUp")
   {
	   pMiniMap->OnCommand_AttiUp("");
   }
   //活跃度
   else if(strFun == "Btn_Activity")
   {
       CDlgDynamicMin* pDynamicMin = (CDlgDynamicMin*)(GetGameUIMan()->GetDialog("Win_Activity_Min"));
	   pDynamicMin->OnCommandOpen("");
   }
   //群雄逐鹿
   else if(strFun == "Battle_Dungeon_6V6_Min")
   {
	   GetDlgItem("Btn_Group")->Show(!GetDlgItem("Btn_Group")->IsShow());
	   GetDlgItem("Btn_6V6")->Show(!GetDlgItem("Btn_6V6")->IsShow());
   }
   //原本服玩法
   else if(strFun == "Btn_6V6")
   {
       pMiniMap->OnCommand_Group("");
   }
   //战队玩法
   else if(strFun == "Btn_Group")
   {
       pMiniMap->OnCommand_6V6("");
   }
   //天帝战
   else if(strFun == "Btn_King_Info")
   {
      CDlgKingMin* pKingMin = (CDlgKingMin*)(GetGameUIMan()->GetDialog("Win_King_Min"));
	  pKingMin->OnCommand_Open("");
   }
   //三界商行
   else if(strFun == "Btn_Auction")
   {
      pMiniMap->OnCommand_Auction("");
   }
   //诛仙启示录
   else if(strFun == "Btn_Menology")
   {
      pMiniMap->OnCommand_daily("");
   }
   //VIP奖励
   else if(strFun == "Btn_Vip")
   {
      pMiniMap->OnCommand_Vip("");
   }
   //GM首页
   else if(strFun == "Btn_Gmbbs")
   {
      pMiniMap->OnCommand_GMBbs("");
   }
   //新手帮助
   else if(strFun == "Btn_NewerHelp")
   {
      pMiniMap->OnCommand_NewHelp("");
   }
   //主题站
   else if(strFun == "Btn_ThemeLink")
   {
      pMiniMap->OnCommand_ThemeLink("");
   }
   //更新说明
   else if(strFun == "Btn_UpdateDiary")
   {
      pMiniMap->OnCommand_UpdateLog("");
   }
   //热门活动
   else if(strFun == "Btn_Explorer_Popu")
   {
      CDlgWebHot* pWebHot = (CDlgWebHot*)(GetGameUIMan()->GetDialog("Win_Explorer_Popu_Min"));
	  pWebHot->OnCommandOpen("");
   }
   //诛仙挖宝
   else if (strFun == "Btn_ZhuxianExplore")
   {
     CDlgDlgTreasMin* pTreasMin = (CDlgDlgTreasMin*)(GetGameUIMan()->GetDialog("Win_ZhuxianExplore_Min"));
	 pTreasMin->OnCommand_Active("");
   }
   //在线客服
   else if(strFun == "Btn_GM")
   {
      pSystem3->OnCommandGM("");
   }
   //系统设置
   else if(strFun == "Btn_Setting")
   {
      pSystem3->OnCommandSetting("");
   }
   //返回选人
   else if(strFun == "Btn_Repick")
   {
      pSystem3->OnCommandRepick("");
   }
   //退出游戏
   else if(strFun == "Btn_Quit")
   {
      pSystem3->OnCommandQuit("");
   }
   //Show(false);

}