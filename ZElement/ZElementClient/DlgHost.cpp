// Filename	: DlgHost.cpp
// Creator	: Tom Zhou
// Date		: October 11, 2005

#include "AUI\\AUICommon.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "DlgHost.h"
#include "DlgAutoLock.h"
#include "DlgSetting.h"
#include "DlgTeamContext.h"
#include "DlgPKSetting.h"
#include "DlgHostPet.h"
#include "DlgPetAction.h"
#include "DlgBattleFactionWarControl.h"
#include "DlgBattleFlow.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Team.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_Skill.h"
#include "EC_Pet.h"
#include "EC_BattleDungeon.h"
#include "EC_Helper.h"
#include "EC_AutoHelper.h"
#include "EC_OnlineAward.h"
#include "DlgKingWar.h"
#include "LuaEvent.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgHost, CDlgBase)

AUI_ON_COMMAND("closeHP",		OnCommand_closeHP)
AUI_ON_COMMAND("sight",			OnCommand_sight)
AUI_ON_COMMAND("player",		OnCommand_player)
AUI_ON_COMMAND("mapmonster",	OnCommand_mapmonster)
AUI_ON_COMMAND("surround",		OnCommand_surround)
AUI_ON_COMMAND("headhp",		OnCommand_headhp)
AUI_ON_COMMAND("headmp",		OnCommand_headmp)
AUI_ON_COMMAND("headexp",		OnCommand_headexp)
AUI_ON_COMMAND("lock",			OnCommand_lock)
AUI_ON_COMMAND("stop",			OnCommand_stop)
AUI_ON_COMMAND("close",			OnCommand_close)
AUI_ON_COMMAND("pk",			OnCommand_pk)
AUI_ON_COMMAND("battleinfo",	OnCommand_battleinfo)
AUI_ON_COMMAND("battlescore",	OnCommand_battlescore)
AUI_ON_COMMAND("siegescore",	OnCommand_siegescore)
AUI_ON_COMMAND("Btn_BattleTerri",	OnCommand_TerrWarInfo)
AUI_ON_COMMAND("Btn_AwardOnline",	OnCommand_AwardOnline)
AUI_ON_COMMAND("Btn_Accout",	OnCommand_AccountComplete)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_GuildWar",OnCommand_GuildWar)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgHost, CDlgBase)

AUI_ON_EVENT("Pic_HeadFrame",		WM_MOUSEMOVE,		OnEventMouseMove)
AUI_ON_EVENT("Pic_HeadFrame",		WM_LBUTTONDOWN,		OnEventLButtonDown_Size)
AUI_ON_EVENT("Pic_HeadFrame",		WM_LBUTTONUP,		OnEventLButtonUp_Size)
AUI_ON_EVENT("Pic_HeadFrame",		WM_RBUTTONUP,		OnEventRButtonUp_Size)

AUI_END_EVENT_MAP()

EC_SYSTEM_SETTING CDlgHost::m_ss;
EC_SYSTEM_SETTING CDlgHost::m_ssTemp;
EC_VIDEO_SETTING CDlgHost::m_vs;
EC_VIDEO_SETTING CDlgHost::m_vsTemp;

CDlgHost::CDlgHost()
{
	m_nMouseOffset = 0;
	m_nMouseOffsetThis = 0;
	m_bIsStart = false;
	m_nSendTime = 60000;
}

CDlgHost::~CDlgHost()
{
}

bool CDlgHost::OnInitDialog()
{
	int i;
	char szItem[20];

	m_pImg_VIP = (PAUIIMAGEPICTURE)GetDlgItem("Img_VIP");
	m_pImg_Head = (PAUIIMAGEPICTURE)GetDlgItem("Pic_HeadFrame");
	m_pImgProfession = (PAUIIMAGEPICTURE)GetDlgItem("Pic_Prof");
// 	m_pImgCombatMask = (PAUIIMAGEPICTURE)GetDlgItem("CombatMask");
	m_pTxt_Name = (PAUILABEL)GetDlgItem("Txt_Name");
	// m_pTxt_Prof = (PAUILABEL)GetDlgItem("Txt_Prof");
	m_pTxt_LV = (PAUILABEL)GetDlgItem("Txt_LV");
	m_pPrgs_HP = (PAUIPROGRESS)GetDlgItem("Prgs_HP");
	m_pPrgs_MP = (PAUIPROGRESS)GetDlgItem("Prgs_MP");
	m_pPrgs_MP = (PAUIPROGRESS)GetDlgItem("Prgs_MP");
	m_pPrgs_DP = (PAUIPROGRESS)GetDlgItem("Prgs_Dp");
	if (m_pPrgs_DP) m_pPrgs_DP->Show(false);
	// m_pPrgs_EXP = (PAUIPROGRESS)GetDlgItem("Prgs_EXP");
	// m_pPic_MinusExp = (PAUIIMAGEPICTURE)GetDlgItem("Pic_MinusExp");
	m_pTxt_HP = (PAUILABEL)GetDlgItem("Txt_HP");
	m_pTxt_MP = (PAUILABEL)GetDlgItem("Txt_MP");
	m_pTxt_DP = (PAUILABEL)GetDlgItem("Txt_DP");
	if( m_pTxt_HP )
		m_pTxt_HP->SetAlign(AUIFRAME_ALIGN_CENTER);
	if( m_pTxt_MP )
		m_pTxt_MP->SetAlign(AUIFRAME_ALIGN_CENTER);
 	if( m_pTxt_DP )
	{
		m_pTxt_DP->Show(false);
 		m_pTxt_DP->SetAlign(AUIFRAME_ALIGN_CENTER);
	}
	// m_pTXT_EXP = (PAUILABEL)GetDlgItem("TXT_EXP");
	m_pImg_Leader = (PAUIIMAGEPICTURE)GetDlgItem("Img_Leader");
	m_pChk_Player = (PAUICHECKBOX)GetDlgItem("Chk_Player");
	m_pChk_Surround = (PAUICHECKBOX)GetDlgItem("Chk_Surround");
	m_pChk_Sight = (PAUICHECKBOX)GetDlgItem("Chk_Sight");
	m_pChk_MapMonster = (PAUICHECKBOX)GetDlgItem("Chk_MapMonster");
	m_pChk_BattleInfo = (PAUICHECKBOX)GetDlgItem("Chk_BattleInfo");
	m_pChk_BattleScore = (PAUICHECKBOX)GetDlgItem("Chk_BattleScore");
	m_pChk_SiegeScore = (PAUICHECKBOX)GetDlgItem("Chk_SiegeScore");
//	m_pChk_EquipCompare = (PAUICHECKBOX)GetDlgItem("Chk_EquipCompare");

// 	if( m_pImgCombatMask )
// 		m_pImgCombatMask->SetTransparent(true);

	for( i = 0; ; i++ )
	{
		sprintf(szItem, "Img_P%don", i + 1);
		if( !GetDlgItem(szItem) ) break;
		m_vecImgAPOn.push_back((PAUIIMAGEPICTURE)GetDlgItem(szItem));

		sprintf(szItem, "Img_P%doff", i + 1);
		if( !GetDlgItem(szItem) ) break;
		m_vecImgAPOff.push_back((PAUIIMAGEPICTURE)GetDlgItem(szItem));
	}
	if( m_pImg_Head )
		m_pImg_Head->SetAcceptMouseMessage(true);

	m_nBattleState = BATTLE_STATE_NORMAL;
	m_nBattleTime = 0;
	m_nBattleLineID = 0;
	m_nBattleMapID = 0;
	m_iCultivation = -1;
	m_bFengsheng = 0;
	m_bHighLevel = 0;
	m_bGuideSkillStartFlag = false;
	if (m_szName == "Win_CharHead")
	{
		for (i=0; i<5; i++)
		{
			AString strName;
			strName.Format("Img_HG%d", i+1);
			m_pImgSkillColor[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		}
	}
	m_pBtnTerr = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_BattleTerri");
	if(m_pBtnTerr)
		m_pBtnTerr->Show(false);
	m_pBtnAwardOnline = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_AwardOnline");
	if(m_pBtnAwardOnline)
	{
		m_pBtnAwardOnline->SetData(1, "state_cantake");
		bool bShowFlash = true;

		m_pBtnAwardOnline->SetFlash(bShowFlash);
		PAUIIMAGEPICTURE pShapeHint = (PAUIIMAGEPICTURE)GetDlgItem("Shape_Hint01");
		pShapeHint->Show(bShowFlash);		
		PAUIIMAGEPICTURE pImageHint = (PAUIIMAGEPICTURE)GetDlgItem("Img_Hint01");
		pImageHint->Show(bShowFlash);
		PAUILABEL pLabelHint = (PAUILABEL)GetDlgItem("Txt_Hint01");
		pLabelHint->Show(bShowFlash);
	}

	return true;
}

void CDlgHost::OnTick()
{
	if (m_pBtnTerr)		// charhead 界面
	{
		if(GetHostPlayer()->GetTerritoryBattleInfo()>0 || GetHostPlayer()->GetRaidBattleInfo()>0)
		{
			if(!m_pBtnTerr->IsShow())
			{
				m_pBtnTerr->Show(true);
				if(GetHostPlayer()->GetTerritoryBattleInfo()>0)
					m_pBtnTerr->SetHint(GetGameUIMan()->GetStringFromTable(9323));
				else
					m_pBtnTerr->SetHint(GetGameUIMan()->GetStringFromTable(11129));
			}
		}
		else
			m_pBtnTerr->Show(false);

		OnlineAwardManager* pOnlineMan = GetHostPlayer()->GetOnlineAwardManager();
		OnlineAwardInfoVector awardVector;
		pOnlineMan->GetOnlineAwardInfos(awardVector);
		int i;
		int iCanTake = 0;
		for (i=0; i<(int)awardVector.size(); ++i)
		{
			if (awardVector[i].m_nRemainTime<=0)
			{
				iCanTake = 1;
			}
		}
		if ((int)m_pBtnAwardOnline->GetData("state_cantake")!=iCanTake)
		{
			m_pBtnAwardOnline->SetData(iCanTake, "state_cantake");
			AUIOBJECT_SETPROPERTY p;
			strcpy(p.fn, iCanTake==1 ? "Button\\AwardOnlineHover.tga" : "Button\\AwardOnlineDisable.tga");
			m_pBtnAwardOnline->SetProperty("Up Frame File",&p);

			bool bShowFlash = (iCanTake==1)?true:false;

			m_pBtnAwardOnline->SetFlash(bShowFlash);
			PAUIIMAGEPICTURE pShapeHint = (PAUIIMAGEPICTURE)GetDlgItem("Shape_Hint01");
			pShapeHint->Show(bShowFlash);		
			PAUIIMAGEPICTURE pImageHint = (PAUIIMAGEPICTURE)GetDlgItem("Img_Hint01");
			pImageHint->Show(bShowFlash);
			PAUILABEL pLabelHint = (PAUILABEL)GetDlgItem("Txt_Hint01");
			pLabelHint->Show(bShowFlash);
		}
	}

	if (m_pChk_BattleScore&&m_pChk_BattleScore->IsShow()&& GetHostPlayer()->GetBattleType()==BATTLE_TYPE_CHALLENGE )
	{
		ACString strText;
		int nTime = GetBattleTime() - GetGame()->GetServerGMTTime() - 60;	// deliver 提前1分钟通知结束告诉gs清理战场，只有这个战场
		if (nTime<0)
		{
			nTime = 0;
		}
		strText.Format(GetGameUIMan()->GetStringFromTable(1710), nTime/60, nTime%60);
		m_pChk_BattleScore->SetHint(strText);
	}
	if (m_szName=="Win_CharHead_Prof11")
	{
		static int iLastValue = 1;
		ACString strText;
		PAUILABEL pLabNum = (PAUILABEL)GetDlgItem("Txt_Num");
		int iValue = GetHostPlayer()->GetBlackAndLightValue();
		PAUIPROGRESS pPrgLeft = (PAUIPROGRESS)GetDlgItem("Prgs_Left");
		PAUIPROGRESS pPrgRight = (PAUIPROGRESS)GetDlgItem("Prgs_Right");
		pPrgLeft->SetProgress(abs(iValue));
		pPrgRight->SetProgress(abs(iValue));

		if (iLastValue*iValue<0)
		{
			iLastValue = iValue;
			AUIOBJECT_SETPROPERTY p;
			if(iValue>=0)
			{
				sprintf(p.fn, "Progress\\Prgs_Chenhuang_FillImgA.tga");
			}
			else
			{
				sprintf(p.fn, "Progress\\Prgs_Chenhuang_FillImgB.tga");
			}
			pPrgLeft->SetProperty("Fill Image File", &p);
			pPrgRight->SetProperty("Fill Image File", &p);
		}
		if(iValue>=0)
		{
			strText.Format(_AL("^efd621%d"), iValue);
		}
		else
		{
			strText.Format(_AL("^d32f05%d"), -iValue);
		}


		pLabNum->SetText(strText);
		int i;
		AString strName;
		PAUIIMAGEPICTURE pImg;
		for (i=0; i<3; ++i)
		{
			strName.Format("Img_HG%d", i+1);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pImg->FixFrame(GetHostPlayer()->GetDarkLightSpirit(i));
			pImg->SetHint(GetGameUIMan()->GetStringFromTable(15040+GetHostPlayer()->GetDarkLightSpirit(i)));
		}
	}
	if(m_szName == "Win_CharHead_Prof13")
	{
		ACString strText;
		PAUILABEL pLabNum = (PAUILABEL)GetDlgItem("Txt_Num");
		PAUIPROGRESS pPrgInk = (PAUIPROGRESS)GetDlgItem("Prgs_Ink");
		int iValue = GetHostPlayer()->GetBasicProps().ip;
		pPrgInk->SetProgress(int(double(iValue) *  AUIPROGRESS_MAX / 100));
		strText.Format(_AL("%d"), iValue);
		pLabNum->SetText(strText);
	}
	if(m_szName == "Win_Charhead_HpPrgs")
	{
		CECHostPlayer *pHost = GetHostPlayer();
		if(pHost)
		{
			ACString strText;
			bool bBloodPool = pHost->GetBloodPoolState();
			int  iCurBloodPool = pHost->GetCurBloodPool();
			int  iMaxBloodPool = max(pHost->GetMaxBloodPool(), 1);

			if(bBloodPool)
			{
				PAUIPROGRESS pPrgs_Blood = (PAUIPROGRESS)GetDlgItem("Prgs_Hp");
				pPrgs_Blood->SetProgress(int(double(iCurBloodPool) * AUIPROGRESS_MAX / iMaxBloodPool));
				PAUILABEL pLabTxt = (PAUILABEL)GetDlgItem("Txt_Hp");
				strText.Format(_AL("%d/%d"), iCurBloodPool, iMaxBloodPool);
				pLabTxt->SetText(strText);
			}
			else
			{
				Show(false);
			}
		}	
	}
	if(m_szName == "Win_CharHead")
	{
		if(GetHostPlayer()->GetSrcServerID()!=0)
		{
           GetDlgItem("Btn_GuildWar")->Show(false);
		}
		//帮战的双方成员或者观看帮战的玩家
		else if(CDlgBattleFactionWarControl::GetCombatFaction() !=0 || GetHostPlayer()->GetRaidBattleInfo() == CDlgBattleFactionWarControl::GetFactionPKRaidMapId())
	    {
		     GetDlgItem("Btn_GuildWar")->Show(true);
			 if(GetHostPlayer()->GetRaidBattleInfo() == CDlgBattleFactionWarControl::GetFactionPKRaidMapId())
				 GetDlgItem("Btn_GuildWar")->SetHint(GetStringFromTable(15540));
			 else
				 GetDlgItem("Btn_GuildWar")->SetHint(GetStringFromTable(15541));
		}
		else
		   GetDlgItem("Btn_GuildWar")->Show(false);
	}
	
	if(GetHostPlayer()->getIsBattleWait() == true)
	{
		DWORD dwTimeNow = GetTickCount();
		if(m_bIsStart == false)
		{
			if ((dwTimeNow - m_dwTimeSend) >= m_nSendTime)
			{
				m_bIsStart = true;
				m_dwTimeSend = GetTickCount();
				
			}
// 			DlgBattleFlowInfoWaitMax* pDlg = (DlgBattleFlowInfoWaitMax*)GetGameUIMan()->GetDialog("Win_Battle_SeniorPK_WaitLine");
// 			if(pDlg)
// 			{
// 				pDlg->UpdateBattleFlowWaitTime(GetTickCount());
// 			}
		}
		else if(m_bIsStart == true)
		{
			GetGameSession()->c2s_CmdSendFlowQueueCount();
			m_dwTimeSend = GetTickCount();
			m_bIsStart = false;
		}
	}

}

void CDlgHost::OnCommand_closeHP(const char * szCommand)
{
	bool bCheck = ((PAUICHECKBOX)GetDlgItem("Chk_CloseHP"))->IsChecked();

	GetGameUIMan()->GetDialog("Win_CharHead")->Show(!bCheck);
}

void CDlgHost::OnCommand_pk(const char * szCommand)
{
	CDlgPKSetting* pDlgPKSetting;
	if (GetHostPlayer()->GetSrcServerID()==0)
		pDlgPKSetting = GetGameUIMan()->m_pDlgPKSetting;
	else 
		pDlgPKSetting = (CDlgPKSetting*)GetGameUIMan()->GetDialog("Win_KfpkPKSetting");
	if( !pDlgPKSetting->IsShow() )
	{
		pDlgPKSetting->Show(true);
		LuaEvent::FirstPKSetting();
	}
	else
		pDlgPKSetting->OnCommand_Confirm("");
}

void CDlgHost::OnCommand_battleinfo(const char * szCommand)
{
	if ( CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle() )
	{
		GetGameSession()->raid_getroom(CECBattleDungeonMan::getSingletonPtr()->m_iRoomId);
	}
	if ( GetHostPlayer()->GetBattleInstanceState()==1 )
	{
		GetGameSession()->instancing_getfield(m_nInstanceLineID, m_nInstanceMapID);
	}
	if ( GetHostPlayer()->GetApplyBattleType()>=0 )
		GetGameSession()->battle_getfield(m_nBattleLineID, m_nBattleMapID);
}

void CDlgHost::OnCommand_battlescore(const char * szCommand)
{
	int battleType = GetHostPlayer()->GetBattleType();
	if ( battleType==BATTLE_TYPE_SONJIN || battleType==BATTLE_TYPE_SONJINREBORN )
	{
		GetGameUIMan()->GetDialog("Win_BattleSongScore")->Show(true);
	}
	else if ( battleType==BATTLE_TYPE_INSTANCING )
		GetGameUIMan()->GetDialog("Win_BattleRaidStage")->Show(true);
	else if ( battleType==BATTLE_TYPE_CHALLENGE )
	{
		// 挑战战场只需要显示退出时间，悬浮
		return;
	}
	else if(GetHostPlayer()->GetKingdomWarType() == 1 || GetHostPlayer()->GetKingdomWarType() == 2)
	{
        GetGameUIMan()->GetDialog("Win_King_BattleInfo3")->Show(true);
	    return;
	}
	CECHostPlayer *pHost = GetHostPlayer();
	pHost->CmdGetBattleInfo();
}

void CDlgHost::OnCommand_siegescore(const char * szCommand)
{
	GetGameSession()->war_top(1);
}

void CDlgHost::OnCommand_TerrWarInfo(const char * szCommand)
{
	if ( GetHostPlayer()->GetTerritoryBattleInfo()>0)
	{
		GetGameSession()->territory_get_map();
		GetGameUIMan()->GetDialog("Win_Battle_TerriWarArea")->Show(true);
	}
	else
	{
		if (CECBattleDungeonMan::getSingletonPtr()->GetCurBattle() &&
			CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iId==534)	// 梦回河阳城
		{
			PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Heyang");
			pDlg->Show(!pDlg->IsShow());
		}
		else if (CECBattleDungeonMan::getSingletonPtr()->GetCurBattle() &&
			CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iId==642)	// 6v6 副本
		{
			PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_Record");
			pDlg->Show(!pDlg->IsShow());
		}
		else if (CECBattleDungeonMan::getSingletonPtr()->GetCurBattle() &&
			CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iId==795)	// 跨服6v6 副本
		{
			PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_Record");
			pDlg->Show(!pDlg->IsShow());
		}
		else
		{
			c2s_SendCmdGetRaidInfo();
			GetGameUIMan()->GetDialog("Win_Battle_DungeonMemberList")->Show(true);
		}
	}
}

void CDlgHost::OnCommand_AwardOnline(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_AwardOnline");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgHost::OnCommand_AccountComplete(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_AccountInfo_Award");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgHost::OnCommand_sight(const char * szCommand)
{
	PAUICHECKBOX pCheck = (PAUICHECKBOX)GetDlgItem("Chk_Sight");

	m_ss = g_pGame->GetConfigs()->GetSystemSettings();
	m_vs = g_pGame->GetConfigs()->GetVideoSettings();

	if( pCheck->IsChecked() )
	{
		m_ssTemp = g_pGame->GetConfigs()->GetSystemSettings();
		m_vsTemp = g_pGame->GetConfigs()->GetVideoSettings();
		
		m_ss.nLevel = 0;
		m_ss.nSight = 0;
		m_ss.nWaterEffect = 0;
		m_ss.nTreeDetail = 0;
		m_ss.nGrassDetail = 0;
		m_ss.nCloudDetail = 0;
		m_ss.bShadow = false;
		m_ss.bMipMapBias = false;
		m_ss.bFullGlow = false;
		m_ss.bSimpleTerrain = true;
		m_ss.bSpaceWarp = false;
		m_ss.bSunFlare = false;
		m_ss.bVSync = false;

		m_vs.bShowCustomize = false;
		m_vs.bModelLimit = true;
		m_vs.nDistance = 0;

		g_pGame->GetConfigs()->SetSystemSettings(m_ss);
		g_pGame->GetConfigs()->SetVideoSettings(m_vs);
	}
	else
	{
		g_pGame->GetConfigs()->SetSystemSettings(m_ssTemp);
		g_pGame->GetConfigs()->SetVideoSettings(m_vsTemp);
	}
//	GetGameUIMan()->m_pDlgSettingCurrent->UpdateView();
}

void CDlgHost::OnCommand_player(const char * szCommand)
{
	m_vs = g_pGame->GetConfigs()->GetVideoSettings();
	m_vs.bPlayerHeadText = !m_vs.bPlayerHeadText;
	g_pGame->GetConfigs()->SetVideoSettings(m_vs);
//	GetGameUIMan()->m_pDlgSettingCurrent->UpdateView();
}

void CDlgHost::OnCommand_mapmonster(const char * szCommand)
{
	m_vs = g_pGame->GetConfigs()->GetVideoSettings();
	m_vs.bMapMonster = !m_vs.bMapMonster;
	g_pGame->GetConfigs()->SetVideoSettings(m_vs);
//	GetGameUIMan()->m_pDlgSettingCurrent->UpdateView();
}

void CDlgHost::OnCommand_surround(const char * szCommand)
{
	m_vs = g_pGame->GetConfigs()->GetVideoSettings();
	m_vs.bModelLimit = !m_vs.bModelLimit;
	g_pGame->GetConfigs()->SetVideoSettings(m_vs);
//	GetGameUIMan()->m_pDlgSettingCurrent->UpdateView();
}

void CDlgHost::OnCommand_headhp(const char * szCommand)
{
	m_vs = g_pGame->GetConfigs()->GetVideoSettings();
	m_vs.bPlayerHP = !m_vs.bPlayerHP;
	g_pGame->GetConfigs()->SetVideoSettings(m_vs);
//	GetGameUIMan()->m_pDlgSettingCurrent->UpdateView();
}

void CDlgHost::OnCommand_headmp(const char * szCommand)
{
	m_vs = g_pGame->GetConfigs()->GetVideoSettings();
	m_vs.bPlayerMP = !m_vs.bPlayerMP;
	g_pGame->GetConfigs()->SetVideoSettings(m_vs);
//	GetGameUIMan()->m_pDlgSettingCurrent->UpdateView();
}

void CDlgHost::OnCommand_headexp(const char * szCommand)
{
	m_vs = g_pGame->GetConfigs()->GetVideoSettings();
	m_vs.bPlayerEXP = !m_vs.bPlayerEXP;
	g_pGame->GetConfigs()->SetVideoSettings(m_vs);
//	GetGameUIMan()->m_pDlgSettingCurrent->UpdateView();
}

void CDlgHost::OnCommand_lock(const char * szCommand)
{
	GetGameUIMan()->GetDialog("Win_AutoLock")->Show(true);
}

void CDlgHost::OnCommand_stop(const char * szCommand)
{
	if( m_szName == "MagicProgress2" )			// Power full.
	{
		CECSkill *pCurSkill = GetHostPlayer()->GetCurSkill();
		pCurSkill->EndCharging();
		GetGameSession()->c2s_CmdContinueAction();
	}
	else if( m_szName == "MagicProgress3" )		// Stop array.
	{
		GetGameSession()->c2s_CmdCancelAction();
	}
}

void CDlgHost::OnCommand_close(const char * szCommand)
{
	if( stricmp(m_szName, "Win_CharHead") == 0 )
	{
		Show(false);
		GetGameUIMan()->GetDialog("Win_CharHeadClose")->Show(true);
		if( GetHostPlayer()->GetCurPetIndex() != -1 )
		{
			GetGameUIMan()->GetDialog("Win_PetHead")->Show(false);
			GetGameUIMan()->GetDialog("Win_PetHeadClose")->Show(false);
		}
	}
	else
	{
		Show(false);
		GetGameUIMan()->GetDialog("Win_CharHead")->Show(true);
		if( GetHostPlayer()->GetCurPetIndex() != -1 )
		{
			GetGameUIMan()->GetDialog("Win_PetHead")->Show(true);
		}
	}
}

void CDlgHost::OnCommand_CANCEL(const char * szCommand)
{
	m_pAUIManager->RespawnMessage();
}
void CDlgHost::OnCommand_GuildWar(const char* szCommand)
{
	if( stricmp(m_szName, "Win_CharHead") == 0 )
	{
	   if(!GetGameUIMan()->GetDialog("Win_Battle_GuildWar_Applylist")->IsShow())
	   {
		   //GetGameUIMan()->GetDialog("Win_Battle_GuildWar_Applylist")->Show(true);
		   CDlgBattleFactionWarControl* pDlgBFWControl = (CDlgBattleFactionWarControl*)(GetGameUIMan()->GetDialog("Win_Battle_GuildWar_Applylist"));
		   pDlgBFWControl->GetFactionWarInfo();
	   }
	}
}
void CDlgHost::OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( (wParam & MK_LBUTTON) )
	{
		m_nMouseOffsetThis = GET_X_LPARAM(lParam) - m_nMouseLastX;
		if( m_nMouseOffset + m_nMouseOffsetThis > 70 )
			m_nMouseOffsetThis = 70 - m_nMouseOffset;
		else if( m_nMouseOffset + m_nMouseOffsetThis < -70 )
			m_nMouseOffsetThis = -70 - m_nMouseOffset;
	}
}

void CDlgHost::OnEventLButtonDown_Size(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	SetCaptureObject(pObj);
	m_nMouseLastX = GET_X_LPARAM(lParam);
	m_nMouseOffset += m_nMouseOffsetThis;
	m_nMouseOffsetThis = 0;

}

void CDlgHost::OnEventLButtonUp_Size(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	SetCaptureObject(NULL);
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	if( GetRect().PtInRect(x, y) )
	{
		GetGameSession()->c2s_CmdSelectTarget(GetHostPlayer()->GetCharacterID());
	}
}

void CDlgHost::OnEventRButtonUp_Size(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	CECTeam *pTeam = GetHostPlayer()->GetTeam();
	if( !pTeam )
	{
		PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Charhead_Copyname");
		pDlg->SetPosEx(x, y);
		pDlg->Show(true);

		return;
	}
	else
	{		
		CDlgTeamContext *pMenu;
		
		if( pTeam->GetLeaderID() == GetHostPlayer()->GetCharacterID() )
			pMenu = GetGameUIMan()->m_pDlgTeamMe1;
		else
			pMenu = GetGameUIMan()->m_pDlgTeamMe2;
		
		pMenu->SetData(GetHostPlayer()->GetCharacterID());
		pMenu->SetPosEx(x, y);
		
		pMenu->Show(true);
	}
}

bool CDlgHost::Render(void)
{
	if( m_szName == "Win_CharHead" )
		RefreshHostStats();
	if( m_szName == "Win_CharHead" || m_szName == "Win_CharHeadClose")
		RefreshStatusBar();

	return CDlgBase::Render();
}

void CDlgHost::RefreshHostStats()
{
	PAUIDIALOG pDlg = this;
//	bool bCheck = ((PAUICHECKBOX)GetGameUIMan()->GetDialog(
//		"Win_HideHP")->GetDlgItem("Chk_CloseHP"))->IsChecked();
//	if( pDlg->IsShow() == bCheck ) pDlg->Show(!bCheck);
//	if( !pDlg->IsShow() ) return;
	
	CECHostPlayer *pHost = GetHostPlayer();
	const ROLEEXTPROP& ep = pHost->GetExtendProps();
	const ROLEBASICPROP& bp = pHost->GetBasicProps();
	GetDlgItem("Img_Combat")->Show(pHost->IsFighting());

	CECPlayer::SECT_LEVEL2_INFO * pInfo = CECPlayer::GetSectInfoByProf(bp.iProfession);
	bool bHightLevel = (bp.iLevel > 150) && (pHost->GetRebornCount() > 0);

	if (GetHostPlayer()->GetCultivation()!=m_iCultivation || m_bFengsheng!=bp.iDeityLevel>0 || m_bHighLevel != bHightLevel)
	{
		AUIOBJECT_SETPROPERTY p;
		m_iCultivation = GetHostPlayer()->GetCultivation();
		m_bHighLevel = bHightLevel;
		m_bFengsheng = bp.iDeityLevel>0;
		m_pPrgs_DP->Show(m_bFengsheng);
		m_pTxt_DP->Show(m_bFengsheng);
		if(m_bHighLevel)
		{
			if(m_bFengsheng)
			{
				if ( 0 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead03.tga");
				}
				else if ( 1 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead_FengshenTaoism03.tga");
				}
				else if ( 2 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead_FengshenEvil03.tga");
				}
				else if ( 4 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead_FengshenBuddhism03.tga");
				}
			}
			else
			{
				if ( 0 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead03.tga");
				}
				else if ( 1 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead_Taoism03.tga");
				}
				else if ( 2 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead_Evil03.tga");
				}
				else if ( 4 == m_iCultivation )
				{
					sprintf(p.fn, "Window\\CharHead_Buddhism03.tga");
				}
			}
		}
		else if (m_bFengsheng)
		{
			if ( 0 == m_iCultivation )
			{
				sprintf(p.fn, "Window\\CharHead1.tga");
			}
			else if ( 1 == m_iCultivation )
			{
				sprintf(p.fn, "Window\\CharHead_FengshenTaoism.tga");
			}
			else if ( 2 == m_iCultivation )
			{
				sprintf(p.fn, "Window\\CharHead_FengshenEvil.tga");
			}
			else if ( 4 == m_iCultivation )
			{
				sprintf(p.fn, "Window\\CharHead_FengshenBuddhism.tga");
			}
		}
		else
		{
			if ( 0 == m_iCultivation )
			{
				sprintf(p.fn, "Window\\CharHead1.tga");
			}
			else if ( 1 == m_iCultivation )
			{
				sprintf(p.fn, "Window\\CharHead_Taoism.tga");
			}
			else if ( 2 == m_iCultivation )
			{
				sprintf(p.fn, "Window\\CharHead_Evil.tga");
			}
			else if ( 4 == m_iCultivation )
			{
				sprintf(p.fn, "Window\\CharHead_Buddhism.tga");
			}
		}
		PAUIIMAGEPICTURE pImgBack = (PAUIIMAGEPICTURE)pDlg->GetDlgItem("Img_Bg");
		pImgBack->SetProperty("Image File", &p);
	}

	m_vs = g_pGame->GetConfigs()->GetVideoSettings();
//	((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_HeadEXP"))->SetPushed(m_vs.bPlayerEXP);
//	((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_HeadHP"))->SetPushed(m_vs.bPlayerHP);
//	((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_HeadMP"))->SetPushed(m_vs.bPlayerMP);
	
	bool bTianhua = GetHostPlayer()->GetProfession()>=45 && GetHostPlayer()->GetProfession()<=50;
	for (int i=0; i<5; i++)
	{
		m_pImgSkillColor[i]->Show(bTianhua);
	}

	EC_GAME_SETTING gs = GetGame()->GetConfigs()->GetGameSettings();
	if (GetHostPlayer()->GetSrcServerID()==0)
		((PAUICHECKBOX)GetDlgItem("Chk_SwitchPK"))->Check(!(gs.bAtk_NoWhite && gs.bAtk_NoBlue && gs.bAtk_NoRed));
	else
		((PAUICHECKBOX)GetDlgItem("Chk_SwitchPK"))->Check(!gs.bAtk_NoZone);

	int iFrameIndex = -1;
	ACString strText;
	bHightLevel = (bp.iLevel > 150) && (pHost->GetRebornCount() > 0) && (pInfo && pInfo->level2 == 5);
	if(bHightLevel && pInfo)
	{
		if((pInfo->mask_sect & (1<<SECT_2)))
		{
			iFrameIndex = 15;
			strText = GetStringFromTable(18750);
		}
		else if(pInfo->mask_sect & (1<<SECT_3))
		{
			iFrameIndex = 18;
			strText = GetStringFromTable(18751);
		}
		else if(pInfo->mask_sect & (1<<SECT_4))
		{
			iFrameIndex = 21;
			strText = GetStringFromTable(18752);
		}
		else if(pInfo->mask_sect & (1<<SECT_5))
		{
			iFrameIndex = 24;
			strText = GetStringFromTable(18753);
		}
		else if(pInfo->mask_sect & (1<<SECT_6))
		{
			iFrameIndex = 30;
			strText = GetStringFromTable(18754);
		}
		else if(pInfo->mask_sect & (1<<SECT_7))
		{
			iFrameIndex = 70;
			strText = GetStringFromTable(18755);
		}
		else if(pInfo->mask_sect & (1<<SECT_10))
		{
			iFrameIndex = 38;
			strText = GetStringFromTable(18756);
		}
		else if(pInfo->mask_sect & (1<<SECT_11))
		{
			iFrameIndex = 44;
			strText = GetStringFromTable(18757);
		}
		else if(pInfo->mask_sect & (1<<SECT_12))
		{
			iFrameIndex = 101;
			strText = GetStringFromTable(18758);
		}
		else if(pInfo->mask_sect & (1<<SECT_13))
		{
			iFrameIndex = 50;
			strText = GetStringFromTable(18759);
		}
		else if(pInfo->mask_sect & (1<<SECT_14))
		{
			iFrameIndex = 62;
			strText = GetStringFromTable(18760);
		}
		else if(pInfo->mask_sect & (1<<SECT_15))
		{
			iFrameIndex = 61;
			strText = GetStringFromTable(18761);
		}
	}
	else
	{
		iFrameIndex = GetHostPlayer()->GetProfession();
		strText = GetStringFromTable(PROFESSION_START + GetHostPlayer()->GetProfession());
	}
	m_pImgProfession->FixFrame(iFrameIndex);
	m_pImgProfession->SetHint(strText);
	ACHAR szText[80];

	int nLevel = bp.iLevel;
	int nHP = bp.iCurHP;
	int nMaxHP = max(ep.max_hp, 1);
	int nMP = bp.iCurMP;
	int nrage = bp.irage;
	int nMaxMP = max(ep.max_mp, 1);
	int nDP = bp.iCurDP;
	int nMaxDP = max(ep.max_dp, 1);
	double EXP = bp.exp;
	double maxEXP = max(pHost->GetLevelUpExp(nLevel + 1, pHost->GetRebornCount()), 1.0);

	if( pHost->IsAllResReady() )
	{
		m_pImg_Head->SetRenderCallback(PlayerRenderPortrait,
			(DWORD)pHost, m_nMouseOffset + m_nMouseOffsetThis);
	}
	else
		m_pImg_Head->SetRenderCallback(NULL, 0);

	m_pTxt_Name->SetAlign(AUIFRAME_ALIGN_CENTER);
	AUI_ConvertChatString(pHost->GetName(), szText);
	m_pTxt_Name->SetText(szText);
	m_pTxt_Name->SetColor(pHost->GetNameColor());
	
//	m_pTxt_Prof->SetAlign(AUIFRAME_ALIGN_CENTER);
//	m_pTxt_Prof->SetText(GetStringFromTable(101 + pHost->GetProfession()));

	a_sprintf(szText, _AL("%d"), nLevel);
	m_pTxt_LV->SetAlign(AUIFRAME_ALIGN_CENTER);
	m_pTxt_LV->SetText(szText);

	m_pPrgs_HP->SetProgress(int(double(nHP) * AUIPROGRESS_MAX / nMaxHP));
	a_sprintf(szText, _AL("%d/%d"), nHP, nMaxHP);
	m_pTxt_HP->SetText(szText);

	if(pInfo && (pInfo->mask_sect & (1 << SECT_17)))
	{
		AUIOBJECT_SETPROPERTY p;
		sprintf(p.fn, "zhuxian3\\Progress\\Prgs_Ap.tga");
		m_pPrgs_MP->SetProperty("Fill Image File", &p);
		m_pPrgs_MP->SetProgress(int(double(nrage) * AUIPROGRESS_MAX / nMaxHP));
		a_sprintf(szText, _AL("%d/%d"), nrage, nMaxHP);
		m_pTxt_MP->SetText(szText);
	}
	else
	{
		AUIOBJECT_SETPROPERTY p;
		sprintf(p.fn, "zhuxian3\\Progress\\Prgs_Mp.tga");
		m_pPrgs_MP->SetProperty("Fill Image File", &p);
		m_pPrgs_MP->SetProgress(int(double(nMP) * AUIPROGRESS_MAX / nMaxMP));
		a_sprintf(szText, _AL("%d/%d"), nMP, nMaxMP);
		m_pTxt_MP->SetText(szText);
	}

	m_pPrgs_DP->SetProgress(int(double(nDP) * AUIPROGRESS_MAX / nMaxDP));
	a_sprintf(szText, _AL("%d/%d"), nDP, nMaxDP);
	m_pTxt_DP->SetText(szText);

	if ( m_nBattleState==BATTLE_STATE_INBATTLE || m_nInstanceState==BATTLE_STATE_INBATTLE )
	{
		m_pChk_BattleInfo->Show(false);
		//m_pChk_BattleScore->Check(m_nBattleTime < GetGame()->GetServerGMTTime() - 60);
		m_pChk_BattleScore->Check(false);
		m_pChk_BattleScore->Show(true);
	}
	else if ( m_nBattleState==BATTLE_STATE_INTEAM || 
		m_nInstanceState==BATTLE_STATE_INTEAM ||
		CECBattleDungeonMan::getSingletonPtr()->GetInTeamBattle())
	{
		m_pChk_BattleInfo->Show(true);
		//m_pChk_BattleInfo->Check(m_nBattleTime < GetGame()->GetServerGMTTime() - 60);
		m_pChk_BattleInfo->Check(false);
		m_pChk_BattleScore->Show(false);
	}
	//国战主战场
	else if(GetHostPlayer()->GetKingdomWarType() == 1 || GetHostPlayer()->GetKingdomWarType() == 2)
	{
		m_pChk_BattleScore->Show(true);
		m_pChk_BattleScore->Check(false);
		m_pChk_BattleScore->SetHint(_AL(""));
	}
	else
	{
		m_pChk_BattleInfo->Show(false);
		m_pChk_BattleScore->Show(false);
	}
    

	const CECHostPlayer::WAR_INFO& warInfo = pHost->GetWarInfo();	
	m_pChk_SiegeScore->Show(pHost->IsInWar() && warInfo.iWarAttackFID);
	m_pChk_SiegeScore->Check(GetGameUIMan()->GetDialog("Win_SiegeScore")->IsShow());

	double percent;
	if( maxEXP == 0 )
		percent = 0.0;
	else
	{
		percent = (max(EXP, 0)) / maxEXP;
		if( percent > 0.999 )
			percent = 0.999;
	}
	
	((PAUICHECKBOX)GetDlgItem("Chk_AutoLock"))->Check(GetGameUIMan()->m_pDlgAutoLock->IsLocked());	

//	m_pPic_MinusExp->Show(nEXP < 0 ? true : false);
//
//	a_sprintf(szText, _AL("%4.1f%c"), (double(max(0, nEXP)) * 1000 / nMaxEXP) / 10.0f, '%');
//	m_pTXT_EXP->SetText(szText);
//
//	a_sprintf(szText, _AL("%d/%d"), nEXP, nMaxEXP);
//	m_pTXT_EXP->SetHint(szText);
//	m_pPic_MinusExp->SetHint(szText);
//
//	int nNumOff = nMaxAP / 100;
//	for( i = 0; i < (int)m_vecImgAPOff.size(); i++ )
//		m_vecImgAPOff[i]->Show(i < nNumOff ? true : false);
//
//	int nNumOn = nAP / 100;
//	for( i = 0; i < (int)m_vecImgAPOn.size(); i++ )
//		m_vecImgAPOn[i]->Show(i < nNumOn ? true : false);
//
//
//	CECTeam *pTeam = pHost->GetTeam();
//	if( pTeam && pTeam->GetLeaderID() == pHost->GetCharacterID() )
//		m_pImg_Leader->Show(true);
//	else
//		m_pImg_Leader->Show(false);
//

	// pet status
	if( GetHostPlayer()->GetCurPetIndex() != -1 )
	{
		if( !GetGameUIMan()->m_pDlgHostPet->IsShow() && !GetGameUIMan()->GetDialog("Win_PetHeadClose")->IsShow() )
		{
			GetGameUIMan()->m_pDlgHostPet->Show(true, false, false);
			GetGameUIMan()->m_pDlgHostPet->AlignTo(this, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
					AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
			GetGameUIMan()->GetDialog("Win_PetHeadClose")->AlignTo(this, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
					AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
//			GetGameUIMan()->m_pDlgHostPet->SetCanMove(false);
		}
		if( !GetGameUIMan()->m_pDlgPetSkill->IsShow() && !GetGameUIMan()->m_pDlgPetSkill->IsShow() )
		{
			GetGameUIMan()->m_pDlgPetSkill->Show(true, false, false);
		}
	}
	else
	{
		GetGameUIMan()->m_pDlgHostPet->Show(false);
		GetGameUIMan()->m_pDlgPetSkill->Show(false);
	}
	
 	CECHostPlayer *pPlayer = GetHostPlayer();
	m_pImg_VIP->Show(CECPlayer::NO_VIP_STATE != pPlayer->GetVipState());
}

void CDlgHost::RefreshStatusBar()
{
	PAUIPROGRESS pProgress;
	ACString strText;
	CECHostPlayer *pHost = GetHostPlayer();

	// Oxigen bar.
	const CECHostPlayer::BREATHDATA &bd = pHost->GetBreathData();
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Prgs1");
	if( bd.bDiving )
	{
		int nCurTime = bd.iCurBreath, nMaxTime = max(bd.iMaxBreath, 1), nPercent;

		if( !pDlg->IsShow() )
			pDlg->Show(true);

		pProgress = (PAUIPROGRESS)pDlg->GetDlgItem("Prgs");
		pProgress->SetProgress(AUIPROGRESS_MAX * nCurTime / nMaxTime);

		nPercent = nCurTime * 100 / nMaxTime;
		if( nPercent <= 2 )
			pDlg->Blink(150);
		else if( nPercent <= 5 )
			pDlg->Blink(250);
		else if( nPercent <= 10 )
			pDlg->Blink(500);
		else
			pDlg->Blink(0);

		strText.Format(_AL("%d%c"), nPercent, '%');
		pDlg->GetDlgItem("Txt_info")->SetText(strText);
	}
	else
	{
		pDlg->Blink(0);
		if( pDlg->IsShow() )
			pDlg->Show(false);
	}

	// Gather bar.
	CECCounter counter = pHost->GetGatherCnt();
	pDlg = GetGameUIMan()->GetDialog("Win_Prgs2");
	if( pHost->IsGathering() || pHost->IsOperating() || pHost->IsBathing())
	{
		int nCur = counter.GetCounter();
		int nMax = max(counter.GetPeriod(), 1);

		if( !pDlg->IsShow() )
			pDlg->Show(true);

		pProgress = (PAUIPROGRESS)pDlg->GetDlgItem("Prgs");
		PAUILABEL pLabel = (PAUILABEL)pDlg->GetDlgItem("1");
		if(pHost->IsBathing())
		{
			PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_King_ShowerInvite");
			CDlgKingBathInvite* pBathInvite = (CDlgKingBathInvite*)pDlg;
			nCur = pBathInvite->GetBathElapse();
		}

		ACString percent;
		percent.Format(_AL("%d%%"), AUIPROGRESS_MAX * nCur / nMax);
		pLabel->SetText(percent);
		PAUILABEL pLabName = (PAUILABEL)pDlg->GetDlgItem("name");
		if(pHost->IsOperating())
			pLabName->SetText(GetGameUIMan()->GetStringFromTable(4550-27+pHost->GetOptId()));
		else
			pLabName->SetText(_AL(""));
		pProgress->SetProgress(AUIPROGRESS_MAX * nCur / nMax);
	}
	else
	{
		if( pDlg->IsShow() )
			pDlg->Show(false);
	}

	bool bShow = false;
	CECSkill *pCurSkill = pHost->GetCurSkill();
	counter = pHost->GetIncantCnt();

	CECCounter performCounter = pHost->GetPerformCnt();

	int nCur, nMax;
	// Power bar.
	pDlg = GetGameUIMan()->GetDialog("MagicProgress2");
	if( pCurSkill && pCurSkill->IsCharging() && !bShow )
	{
		bShow = true;
		nCur = pCurSkill->GetChargingCnt();
		nMax = max(pCurSkill->GetChargingMax(), 1);

		if( !pDlg->IsShow() )
		{
			pDlg->AlignTo(GetGameUIMan()->GetDialog("MagicProgress1"), AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
				AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
			pDlg->Show(true, false, !GetGameRun()->GetHelper()->IsRunning());
		}

		pProgress = (PAUIPROGRESS)pDlg->GetDlgItem("Prgs");
		pProgress->SetProgress(AUIPROGRESS_MAX * nCur / nMax);

		strText.Format(GetStringFromTable(605), g_pGame->GetSkillDesc()->GetWideString(pCurSkill->GetSkillID() * 10));
		pDlg->GetDlgItem("Txt")->SetText(strText);
		pDlg->GetDlgItem("Txt")->SetAlign(AUIFRAME_ALIGN_CENTER);

		// 显示蓄力技能的时间
		float fTime = (float)nCur/1000.0f;
		ACString strTime;
		strTime.Format(_AL("%.1f"), fTime);
		pDlg->GetDlgItem("Txt_Time")->SetText(strTime);
	}
	else
	{
		if( pDlg->IsShow() )
			pDlg->Show(false);
	}

	// Array bar.
	pDlg = GetGameUIMan()->GetDialog("MagicProgress3");
	if( pHost->IsSpellingDurativeSkill() && pCurSkill && !bShow )
	{
		bShow = true;
		const ROLEEXTPROP& ep = pHost->GetExtendProps();
		const ROLEBASICPROP& bp = pHost->GetBasicProps();

		int nMP = bp.iCurMP;
		int nMaxMP = max(ep.max_mp, 1);
		nCur = nMP;
		nMax = nMaxMP;

		if( !pDlg->IsShow() )
			pDlg->Show(true, false, !GetGameRun()->GetHelper()->IsRunning());

		pProgress = (PAUIPROGRESS)pDlg->GetDlgItem("Prgs");
		pProgress->SetProgress(AUIPROGRESS_MAX * nCur / nMax);

		strText.Format(GetStringFromTable(606), g_pGame->GetSkillDesc()->GetWideString(pCurSkill->GetSkillID() * 10));
		pDlg->GetDlgItem("Txt")->SetText(strText);
		pDlg->GetDlgItem("Txt")->SetAlign(AUIFRAME_ALIGN_CENTER);
	}
	else
	{
		if( pDlg->IsShow() )
			pDlg->Show(false);
	}

	// Incant bar.
	pDlg = GetGameUIMan()->GetDialog("MagicProgress1");
	DWORD dwCurTime = 0, dwMaxTime = 0;
	bool bUseItem = false;
	// 如果是蓄力技能，进度条在吟唱完毕之后就消失，其他技能则等技能差不多释放完毕才消失
	// 如果是引导技能，进度条吟唱完后，逆向从右到左
	if( ( (bUseItem = pHost->GetUsingItemTimeCnt(dwCurTime, dwMaxTime)) || 
		pHost->IsOperatingPet() || (pHost->IsSpellingMagic() && pCurSkill && !pCurSkill->IsChargeable())
		|| (pHost->IsSpellingMagic() && pCurSkill && pCurSkill->IsChargeable() && !counter.IsFull()) ) && !bShow )
	{
		bShow = true;
		if( bUseItem )
		{
			nCur = dwCurTime;
			nMax = dwMaxTime;
			pDlg->GetDlgItem("Txt")->SetText(GetStringFromTable(726));
		}
		else if( pHost->IsOperatingPet() )
		{
			nCur = pHost->GetPetOptTime().GetCounter();
			nMax = max(pHost->GetPetOptTime().GetPeriod(), 1);
			pDlg->GetDlgItem("Txt")->SetText(GetStringFromTable(4320 + pHost->IsOperatingPet()));
		}
		else
		{
			nCur = counter.GetCounter();
			nMax = max(counter.GetPeriod(), 1);

			if(counter.IsFull())
			{
				if(pCurSkill->IsGuide())	// 引导类技能
				{
					if(performCounter.IsFull() && !m_bGuideSkillStartFlag)
					{
						pHost->ResetPerfomCnt();	 // 开始逆向
						m_bGuideSkillStartFlag = true;
					}
										
					nCur = performCounter.GetCounter();
					nMax = max(performCounter.GetPeriod(), 1);
					nCur = nMax - nCur;
				}
			}

			pDlg->GetDlgItem("Txt")->SetText(g_pGame->GetSkillDesc()->GetWideString(pCurSkill->GetSkillID() * 10));
		}

		if( !pDlg->IsShow() )
		{
//			pDlg->AlignTo(GetGameUIMan()->GetDialog("MagicProgress2"), AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
//				AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
			pDlg->Show(true, false, !GetGameRun()->GetHelper()->IsRunning());
		}

		pProgress = (PAUIPROGRESS)pDlg->GetDlgItem("Prgs");
		pProgress->SetProgress(AUIPROGRESS_MAX * nCur / nMax);

		pDlg->GetDlgItem("Txt")->SetAlign(AUIFRAME_ALIGN_CENTER);
	}
	else
	{
//		if(performCounter.IsFull())
//		{
			if( pDlg->IsShow() )
			{
				pDlg->Show(false);
				m_bGuideSkillStartFlag = false;
			}
//		}
//		else
//		{
//			pProgress = (PAUIPROGRESS)pDlg->GetDlgItem("Prgs");
//			
//			nCur = performCounter.GetCounter();
//			nMax = max(performCounter.GetPeriod(), 1);
//			pProgress->SetProgress(AUIPROGRESS_MAX - AUIPROGRESS_MAX * nCur / nMax);
//		}
	}
//	EC_VIDEO_SETTING vs = GetGame()->GetConfigs()->GetVideoSettings();
//	m_pChk_Player->Check(vs.bPlayerHeadText);
//	m_pChk_MapMonster->Check(vs.bMapMonster);
//	m_pChk_Surround->Check(vs.bModelLimit);
}

void CDlgHost::SetInstanceInfo(int nState, int nTime, int nLineID, int nMapID)
{
	m_nInstanceState = nState;
	m_nInstanceTime = nTime;
	m_nInstanceLineID = nLineID;
	m_nInstanceMapID = nMapID;
	m_pChk_BattleScore->SetHint(_AL(""));
}

void CDlgHost::SetBattleInfo(int nState, int nTime, int nLineID, int nFlag, int nMapID )
{
	m_nBattleState = nState;
	m_nBattleTime = nTime;
	m_nBattleLineID = nLineID;
	m_nBattleMapID = nMapID;
	m_pChk_BattleScore->SetHint(_AL(""));
	GetHostPlayer()->SetBattleFlag(nFlag);

}

void CDlgHost::UpdateSkillInfo(bool bStart, int num, char* content)
{
	int i;
	AUIOBJECT_SETPROPERTY p;
	if (!bStart)
	{
		for (i=0; i<5; i++)
		{
			strcpy(p.fn,"icon\\Charhead_HG1.tga");
			m_pImgSkillColor[i]->SetProperty("Image File",&p);
		}
	}
	else
	{
		for (i=0; i<5; i++)
		{
			if (content[i]>0)
			{
				sprintf(p.fn,"icon\\Charhead_HG%d.tga", content[i]+1);
				m_pImgSkillColor[i]->SetProperty("Image File",&p);
			}
			else
			{
				strcpy(p.fn,"icon\\Charhead_HG1.tga");
				m_pImgSkillColor[i]->SetProperty("Image File",&p);
			}
		}
	}
}
