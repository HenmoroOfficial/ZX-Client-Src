// Filename	: DlgCharacter.cpp
// Creator	: Tom Zhou
// Date		: October 11, 2005

#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUIFrame.h"
#include "AUI\\AUICommon.h"
#include "AUI\\AUIManager.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUICTranslate.h"
#include "DlgCharacter.h"
#include "DlgChat.h"
#include "DlgSkill.h"
#include "DlgItemLock.h"
#include "DlgRuneMain.h"
#include "DlgRankList.h"
#include "DlgRemedyMetempsychosis.h"


#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_World.h"
#include "EC_HostPlayer.h"
#include "EC_Faction.h"
#include "elementdataman.h"
#include "EC_GameUIMisc.h"
#include "EC_LoginPlayer.h"
#include "EC_Model.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_IvtrRune.h"
#include "EC_Inventory.h"
#include "EC_GameUIMan.h"
#include "EC_FixedMsg.h"
#include "EC_BattleDungeonTower.h"
#include "roleinfo"
#include "ConfigFromLua.h"
#include "EC_Game.h"
#include "EC_ManPlayer.h"
#include "EC_HostPlayer.h"
#include "EC_SendC2SCmds.h"
#include "EC_Resource.h"
#include "LuaEvent.h"
#include "A2DSprite.h"
#include "EC_Configs.h"

#define new A_DEBUG_NEW

#define  DUNGEONTOWER_DAILYPROMP_ID 220

AUI_BEGIN_COMMAND_MAP(CDlgCharacter, CDlgFormatText)

AUI_ON_COMMAND("Rdo_Fash",			OnCommand_Fashion)
AUI_ON_COMMAND("Rdo_Equip",			OnCommand_Fashion)
AUI_ON_COMMAND("Combo_FashDress",	OnCommand_ChangeFashion)
AUI_ON_COMMAND("Chk_ShowEquip",		OnCommand_ShowEquip)
AUI_ON_COMMAND("Rdo_*",				OnCommand_Page)
AUI_ON_COMMAND("Credit_Rdo*",		OnCommand_Credit)
AUI_ON_COMMAND("Combo_Title",		OnCommand_SelectTitle)
AUI_ON_COMMAND("titleexp",			OnCommand_TitleExp)
AUI_ON_COMMAND("manahelp",			OnCommand_ManaHelp)
AUI_ON_COMMAND("credithelp",		OnCommand_CreditHelp)
AUI_ON_COMMAND("Btn_HideEquip",		OnCommand_HideSpecEquip)
AUI_ON_COMMAND("confirm",			OnCommand_Confirm)
AUI_ON_COMMAND("Btn_PersInfo",		OnCommand_SetInfo)
AUI_ON_COMMAND("Btn_Unfold",		OnCommand_Para)
AUI_ON_COMMAND("Btn_Award",			OnCommand_DungeonAward)
AUI_ON_COMMAND("Btn_Rank",			OnCommand_DungeonRank)
AUI_ON_COMMAND("Btn_Book",			OnCommand_DungeonMonster)
AUI_ON_COMMAND("Btn_Transfer",           OnCommand_DungeonFly)
AUI_ON_COMMAND("Btn_Cancel",		OnCommand_Cancel)
AUI_ON_COMMAND("IDCANCEL",			OnCommand_Cancel)
AUI_ON_COMMAND("Btn_Attrimakeup",			OnCommand_RemedyMetempsychosis)
AUI_ON_COMMAND("Btn_Equip_Rune",	onCommand_RoleRune)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgCharacter, CDlgFormatText)

AUI_ON_EVENT("Equip_*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Equip_*",		WM_LBUTTONUP,		OnEventLButtonUp)
AUI_ON_EVENT("Equip_*",		WM_RBUTTONDOWN,		OnEventLButtonDblclk)
AUI_ON_EVENT("Equip_*",		WM_LBUTTONDBLCLK,	OnEventLButtonDblclk)

AUI_END_EVENT_MAP()

CDlgCharacter::CDlgCharacter()
{
	m_nAngle = 0;
	m_pPlayer = NULL;
}

#define FASHION_DRESS_NUM 8

CDlgCharacter::~CDlgCharacter()
{
	A3DRELEASE(m_pPlayer);
	abase::vector<A2DSprite*>::iterator it;
	for(it=m_vecImageList.begin(); it!=m_vecImageList.end(); ++it)
	{
		A3DRELEASE(*it);
	}
}

bool CDlgCharacter::OnInitDialog()
{
	
	if( m_szName == "Win_Character" )
	{
		int i;
		m_pTxt_Name = (PAUILABEL)GetDlgItem("Txt_Name");

		m_pBtn_LeftTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_LeftTurn");
		m_pBtn_RightTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_RightTurn");
		m_pImg_Char = (PAUIIMAGEPICTURE)GetDlgItem("Pic_Part");

		for(i = 0; i < SIZE_EQUIPIVTR; i++ )
		{
			char szItem[20];
			sprintf(szItem, "Equip_%02d", i);
			m_pImgEquip[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
		}

		ReLoadModel();
		
		m_pCombo_Fashion = (PAUICOMBOBOX)GetDlgItem("Combo_FashDress");
		m_pChk_ShowEquip = (PAUICHECKBOX)GetDlgItem("Chk_ShowEquip");

		for( i = 0; i < FASHION_DRESS_NUM; i++ )
		{
			ACHAR szText[20];
			a_sprintf(szText, GetStringFromTable(7332), i+1);
			m_pCombo_Fashion->AddString(szText);
		}
		
		m_iCultivation = 0;
	}
	else if( m_szName == "Win_CharInfo" )
	{
		m_pTxt_Faction = (PAUILABEL)GetDlgItem("Txt_Faction");
//		m_pTxt_Faction->SetAlign(AUIFRAME_ALIGN_CENTER);
		m_pTxt_Level = (PAUILABEL)GetDlgItem("Txt_Level");
//		m_pTxt_Level->SetAlign(AUIFRAME_ALIGN_CENTER);

		m_pTxt_PK = (PAUILABEL)GetDlgItem("Txt_PK");
		m_pTxt_Mate = (PAUILABEL)GetDlgItem("Txt_Mate");
		m_pTxt_Contribution = (PAUILABEL)GetDlgItem("Txt_Contribution");
		m_pTxt_Combine = (PAUILABEL)GetDlgItem("Txt_Combine");
		m_pTxt_Devotion = (PAUILABEL)GetDlgItem("Txt_Devotion");
		m_pTxt_Mana = (PAUILABEL)GetDlgItem("Txt_Mana");
		m_pTxt_Family = (PAUILABEL)GetDlgItem("Txt_Family");
//		m_pTxt_PK->SetAlign(AUIFRAME_ALIGN_RIGHT);
//		m_pTxt_Mate->SetAlign(AUIFRAME_ALIGN_RIGHT);
//		m_pTxt_Contribution->SetAlign(AUIFRAME_ALIGN_RIGHT);
//		m_pTxt_Combine->SetAlign(AUIFRAME_ALIGN_RIGHT);
//		m_pTxt_Devotion->SetAlign(AUIFRAME_ALIGN_RIGHT);
//		m_pTxt_Mana->SetAlign(AUIFRAME_ALIGN_CENTER);
//		m_pTxt_Family->SetAlign(AUIFRAME_ALIGN_CENTER);

		m_pLab_Level1			= (PAUILABEL)GetDlgItem("Lab_Lv1");
		m_pLab_Level2			= (PAUILABEL)GetDlgItem("Lab_Lv2");
		m_pLab_Level3			= (PAUILABEL)GetDlgItem("Lab_Lv3");
		m_pImg_Prof1			= (PAUIIMAGEPICTURE)GetDlgItem("Pic_Prof1");
		m_pImg_Prof2			= (PAUIIMAGEPICTURE)GetDlgItem("Pic_Prof2");
		m_pImg_Prof3			= (PAUIIMAGEPICTURE)GetDlgItem("Pic_Prof3");

		m_pPro_Mana = (PAUIPROGRESS)GetDlgItem("Pro_Mana");
		A2DSprite* pSprite = new A2DSprite;
		if( !pSprite ) return AUI_ReportError(__LINE__, __FILE__);
	
		bool bval = pSprite->Init(m_pA3DDevice, "icon\\Rose_2D.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	
		m_vecImageList.clear();
		m_vecImageList.push_back(pSprite);

		m_pListBox  = (AUIListBox*)GetDlgItem("Lst_Title");
		if(m_pListBox)
		{
			m_pListBox->SetImageList(&m_vecImageList);	
			m_pListBox->SetItemMask(0xffffffff);
		}

		m_pCombo_Title = (PAUICOMBOBOX)GetDlgItem("Combo_Title");
		m_pCombo_Title->SetImageList(&m_vecImageList);	
		m_pCombo_Title->SetItemMask(0xffffffff);
		m_pCombo_Title->AddString(GetStringFromTable(808));
		m_pCombo_Title->SetItemData(0, 0);
		m_dwLastSelectTitle = GetTickCount();
		m_nNewTitleId = -1;

		m_pAttrimakeup = (PAUICHECKBOX)GetDlgItem("Btn_Attrimakeup");
	}
	/*else if( m_szName == "Win_Char_Nature" )
	{
		m_pTxt_StunRes = (PAUILABEL)GetDlgItem("Txt_StunRes");
		m_pTxt_SilenceRes = (PAUILABEL)GetDlgItem("Txt_SilenceRes");
		m_pTxt_WeaknessRes = (PAUILABEL)GetDlgItem("Txt_WeaknessRes");
		m_pTxt_SleepRes = (PAUILABEL)GetDlgItem("Txt_SleepRes");
		m_pTxt_EnlaceRes = (PAUILABEL)GetDlgItem("Txt_EnlaceRes");
		m_pTxt_StunRes02 = (PAUILABEL)GetDlgItem("Txt_StunRes02");
		m_pTxt_SilenceRes02 = (PAUILABEL)GetDlgItem("Txt_SilenceRes02");
		m_pTxt_WeaknessRes02 = (PAUILABEL)GetDlgItem("Txt_WeaknessRes02");
		m_pTxt_SleepRes02 = (PAUILABEL)GetDlgItem("Txt_SleepRes02");
		m_pTxt_EnlaceRes02 = (PAUILABEL)GetDlgItem("Txt_EnlaceRes02");
		m_pTxt_StunRes03 = (PAUILABEL)GetDlgItem("Txt_StunRes03");
		m_pTxt_SilenceRes03 = (PAUILABEL)GetDlgItem("Txt_SilenceRes03");
		m_pTxt_WeaknessRes03 = (PAUILABEL)GetDlgItem("Txt_WeaknessRes03");
		m_pTxt_SleepRes03 = (PAUILABEL)GetDlgItem("Txt_SleepRes03");
		m_pTxt_EnlaceRes03 = (PAUILABEL)GetDlgItem("Txt_EnlaceRes03");

		m_pTxt_Exp = (PAUILABEL)GetDlgItem("Txt_Exp");
		m_pPrg_Exp = (PAUIPROGRESS)GetDlgItem("Prgs_Exp");
		m_pTxt_HP = (PAUILABEL)GetDlgItem("Txt_HP");
		m_pTxt_MP = (PAUILABEL)GetDlgItem("Txt_MP");
		m_pPrg_HP = (PAUIPROGRESS)GetDlgItem("Prgs_Hp");
		m_pPrg_MP = (PAUIPROGRESS)GetDlgItem("Prgs_Mp");
		m_pTxt_Damage = (PAUILABEL)GetDlgItem("Txt_Damage");
		m_pTxt_Defense = (PAUILABEL)GetDlgItem("Txt_Defense");
		m_pTxt_Definition = (PAUILABEL)GetDlgItem("Txt_Definition");
		m_pTxt_Evade = (PAUILABEL)GetDlgItem("Txt_Evade");
		m_pTxt_SkillDefinition = (PAUILABEL)GetDlgItem("Txt_SkillDefinition");
		m_pTxt_SkillEvade = (PAUILABEL)GetDlgItem("Txt_SkillEvade");
		m_pTxt_Critical = (PAUILABEL)GetDlgItem("Txt_Critical");
		m_pTxt_CriticalDamage = (PAUILABEL)GetDlgItem("Txt_CriticalDamage");
		m_pTxt_DeCritical = (PAUILABEL)GetDlgItem("Txt_DeCritical");
		m_pTxt_DeCriticalDamage = (PAUILABEL)GetDlgItem("Txt_DeCriticalDamage");

		m_pTxt_Exp->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_HP->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_MP->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Damage->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Defense->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Definition->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Evade->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_SkillDefinition->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_SkillEvade->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Critical->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_CriticalDamage->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_DeCritical->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_DeCriticalDamage->SetAlign(AUIFRAME_ALIGN_RIGHT);

		m_pTxt_DeDamageBuddhism	= (PAUILABEL)GetDlgItem("Txt_DeDamageBuddhism");
		m_pTxt_DeDamageEvil 	= (PAUILABEL)GetDlgItem("Txt_DeDamageEvil");
		m_pTxt_DeDamageTaoism	= (PAUILABEL)GetDlgItem("Txt_DeDamageTaoism");
		m_pTxt_DeDamageBuddhism->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_DeDamageEvil->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_DeDamageTaoism->SetAlign(AUIFRAME_ALIGN_RIGHT);

		GetDlgItem("Txt_PopHint01")->SetTransparent(true);
	}*/
	else if( m_szName == "Win_Char_Nature")
	{
		m_pTxt_Name = (PAUILABEL)GetDlgItem("Text_Name");

		m_pTxt_Faction = (PAUILABEL)GetDlgItem("Txt_Faction");
		m_pTxt_Level = (PAUILABEL)GetDlgItem("Txt_Level");
		

		m_pTxt_PK = (PAUILABEL)GetDlgItem("Txt_PK");
		m_pTxt_Mate = (PAUILABEL)GetDlgItem("Txt_Mate");
		m_pTxt_Contribution = (PAUILABEL)GetDlgItem("Txt_Contribution");
		m_pTxt_Combine = (PAUILABEL)GetDlgItem("Txt_Combine");
		m_pTxt_Devotion = (PAUILABEL)GetDlgItem("Txt_Devotion");
		m_pTxt_Mana = (PAUILABEL)GetDlgItem("Txt_Mana");
		m_pTxt_Family = (PAUILABEL)GetDlgItem("Txt_Family");

		m_pLab_Level1			= (PAUILABEL)GetDlgItem("Lab_Lv1");
		m_pLab_Level2			= (PAUILABEL)GetDlgItem("Lab_Lv2");
		m_pLab_Level3			= (PAUILABEL)GetDlgItem("Lab_Lv3");
		m_pImg_Prof1			= (PAUIIMAGEPICTURE)GetDlgItem("Pic_Prof1");
		m_pImg_Prof2			= (PAUIIMAGEPICTURE)GetDlgItem("Pic_Prof2");
		m_pImg_Prof3			= (PAUIIMAGEPICTURE)GetDlgItem("Pic_Prof3");

		m_pPro_Mana = (PAUIPROGRESS)GetDlgItem("Pro_Mana");
		A2DSprite* pSprite = new A2DSprite;
		if( !pSprite ) return AUI_ReportError(__LINE__, __FILE__);

		bool bval = pSprite->Init(m_pA3DDevice, "icon\\Rose_2D.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

		m_vecImageList.clear();
		m_vecImageList.push_back(pSprite);

		m_pListBox  = (AUIListBox*)GetDlgItem("Lst_Title");
		if(m_pListBox)
		{
			m_pListBox->SetImageList(&m_vecImageList);	
			m_pListBox->SetItemMask(0xffffffff);
		}

		m_pCombo_Title = (PAUICOMBOBOX)GetDlgItem("Combo_Title");
		m_pCombo_Title->SetImageList(&m_vecImageList);	
		m_pCombo_Title->SetItemMask(0xffffffff);
		m_pCombo_Title->AddString(GetStringFromTable(808));
		m_pCombo_Title->SetItemData(0, 0);
		m_dwLastSelectTitle = GetTickCount();
		m_nNewTitleId = -1;

		m_pAttrimakeup = (PAUICHECKBOX)GetDlgItem("Btn_Attrimakeup");

		m_pTxt_Exp = (PAUILABEL)GetDlgItem("Txt_Exp");
		m_pPrg_Exp = (PAUIPROGRESS)GetDlgItem("Prgs_Exp");
		m_pTxt_HP = (PAUILABEL)GetDlgItem("Txt_HP");
		m_pTxt_MP = (PAUILABEL)GetDlgItem("Txt_MP");
		m_pPrg_HP = (PAUIPROGRESS)GetDlgItem("Prgs_Hp");
		m_pPrg_MP = (PAUIPROGRESS)GetDlgItem("Prgs_Mp");

		m_pTxt_Exp->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_HP->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_MP->SetAlign(AUIFRAME_ALIGN_RIGHT);

		GetDlgItem("Txt_PopHint01")->SetTransparent(true);
	}
	else if( m_szName == "Win_Character_Nature_Sub01")
	{
		m_pTxt_Damage = (PAUILABEL)GetDlgItem("Txt_Damage");
		m_pTxt_Defense = (PAUILABEL)GetDlgItem("Txt_Defense");
		m_pTxt_Definition = (PAUILABEL)GetDlgItem("Txt_Definition");
		m_pTxt_Evade = (PAUILABEL)GetDlgItem("Txt_Evade");
		m_pTxt_Critical = (PAUILABEL)GetDlgItem("Txt_Critical");
		m_pTxt_CriticalDamage = (PAUILABEL)GetDlgItem("Txt_CriticalDamage");

		m_pTxt_Damage->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Defense->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Definition->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Evade->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Critical->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_CriticalDamage->SetAlign(AUIFRAME_ALIGN_RIGHT);

		
	}
	else if( m_szName == "Win_Character_Nature_Sub02")
	{
		m_pTxt_SkillDefinition = (PAUILABEL)GetDlgItem("Txt_SkillDefinition");
		m_pTxt_SkillEvade = (PAUILABEL)GetDlgItem("Txt_SkillEvade");
		m_pTxt_DeCritical = (PAUILABEL)GetDlgItem("Txt_DeCritical");
		m_pTxt_DeCriticalDamage = (PAUILABEL)GetDlgItem("Txt_DeCriticalDamage");
		m_pTxt_DeDamageBuddhism	= (PAUILABEL)GetDlgItem("Txt_DeDamageBuddhism");
		m_pTxt_DeDamageEvil 	= (PAUILABEL)GetDlgItem("Txt_DeDamageEvil");
		m_pTxt_DeDamageTaoism	= (PAUILABEL)GetDlgItem("Txt_DeDamageTaoism");

		m_pTxt_SkillDefinition->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_SkillEvade->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_DeCritical->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_DeCriticalDamage->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_DeDamageBuddhism->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_DeDamageEvil->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_DeDamageTaoism->SetAlign(AUIFRAME_ALIGN_RIGHT);
		
	}
	else if( m_szName == "Win_Character_Nature_Sub03")
	{
		m_pTxt_StunRes = (PAUILABEL)GetDlgItem("Txt_StunRes");
		m_pTxt_SilenceRes = (PAUILABEL)GetDlgItem("Txt_SilenceRes");
		m_pTxt_WeaknessRes = (PAUILABEL)GetDlgItem("Txt_WeaknessRes");
		m_pTxt_SleepRes = (PAUILABEL)GetDlgItem("Txt_SleepRes");
		m_pTxt_EnlaceRes = (PAUILABEL)GetDlgItem("Txt_EnlaceRes");
		m_pTxt_StunRes02 = (PAUILABEL)GetDlgItem("Txt_StunRes02");
		m_pTxt_SilenceRes02 = (PAUILABEL)GetDlgItem("Txt_SilenceRes02");
		m_pTxt_WeaknessRes02 = (PAUILABEL)GetDlgItem("Txt_WeaknessRes02");
		m_pTxt_SleepRes02 = (PAUILABEL)GetDlgItem("Txt_SleepRes02");
		m_pTxt_EnlaceRes02 = (PAUILABEL)GetDlgItem("Txt_EnlaceRes02");
		m_pTxt_StunRes03 = (PAUILABEL)GetDlgItem("Txt_StunRes03");
		m_pTxt_SilenceRes03 = (PAUILABEL)GetDlgItem("Txt_SilenceRes03");
		m_pTxt_WeaknessRes03 = (PAUILABEL)GetDlgItem("Txt_WeaknessRes03");
		m_pTxt_SleepRes03 = (PAUILABEL)GetDlgItem("Txt_SleepRes03");
		m_pTxt_EnlaceRes03 = (PAUILABEL)GetDlgItem("Txt_EnlaceRes03");
	}
	else if(m_szName == "Win_CharDungeon")
	{
	    UpdateTextArea((PAUITEXTAREA)GetDlgItem("Txt_Npc"));
	}
		   
	return true;
}

void CDlgCharacter::OnShowDialog()
{
	if( stricmp(m_szName, "Win_Character") == 0)
	{
		CheckRadioButton(1, 1);
		PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Char_Nature");
		pDlg->Show(true);
		pDlg->AlignTo(this,AUIDIALOG_ALIGN_OUTSIDE,AUIDIALOG_ALIGN_RIGHT,AUIDIALOG_ALIGN_INSIDE,AUIDIALOG_ALIGN_TOP);
		GetGameUIMan()->GetDialog("Win_Fengshen_Soul")->Show(false);

		if (g_pGame->GetConfigs()->GetLocalSettings().bAutoShowEquipRune || !g_pGame->GetConfigs()->GetLocalSettings().bFuWenPop1Showed)
		{
			pDlg = GetGameUIMan()->GetDialog("Win_Equip_Rune");
			pDlg->Show(true);
		}
	}
	else if (stricmp(m_szName, "Win_Char_Nature") == 0)
	{
		ShowRuneTipPop();
		if(GetHostPlayer()->GetBattleType() == BATTLE_TYPE_FLPW_CRSSVR)
		{
			if(GetDlgItem("Combo_Title"))
			  GetDlgItem("Combo_Title")->Enable(false);
		}

		CheckRadioButton(1,1);
		OnCommand_Page("Rdo_Nature01");
	}
	else if( stricmp(m_szName, "Win_Credit") == 0)
	{
		CheckRadioButton(1, 2);
		CheckRadioButton(2,1);
		GetGameUIMan()->GetDialog("Win_Fengshen_Soul")->Show(false);
	}
	else if( stricmp(m_szName, "Win_Char_TitleBox") == 0)
	{
		CheckRadioButton(1, 3);
		GetGameUIMan()->GetDialog("Win_Fengshen_Soul")->Show(false);
		UpdateTitle();
	}
	else if( stricmp(m_szName, "Win_CharInfo") == 0)
	{
		CheckRadioButton(1, 4);
		GetGameUIMan()->GetDialog("Win_Fengshen_Soul")->Show(false);
		
		if(GetHostPlayer()->GetBattleType() == BATTLE_TYPE_FLPW_CRSSVR)
		{
			GetDlgItem("Combo_Title")->Enable(false);
		}
	}
	else if( stricmp(m_szName, "Win_CharOther") == 0)
	{
		CheckRadioButton(1, 5);
		GetGameUIMan()->GetDialog("Win_Fengshen_Soul")->Show(false);
	}
	else if( stricmp(m_szName, "Win_CharDungeon") == 0)
	{
		UpdateDungeonTower();
	}
	else if( stricmp(m_szName, "Win_Credit") == 0)
	{
		CheckRadioButton(2, 1);
		GetGameUIMan()->GetDialog("Win_Fengshen_Soul")->Show(false);
	}
	else if( stricmp(m_szName, "Win_Credit2") == 0)
	{
		CheckRadioButton(2, 2);
		GetGameUIMan()->GetDialog("Win_Fengshen_Soul")->Show(false);
	}

}

void CDlgCharacter::OnTick()
{
	if (m_szName == "Win_Char_Nature")
	{
		AlignTo(GetGameUIMan()->m_pDlgCharacter,AUIDIALOG_ALIGN_OUTSIDE,AUIDIALOG_ALIGN_RIGHT,AUIDIALOG_ALIGN_INSIDE,AUIDIALOG_ALIGN_TOP);
	}
	if( m_szName == "Win_Character")
	{
		CheckRadioButton(2, GetHostPlayer()->InFashionMode() ? 2 : 1);
		UpdateEquip();
		CECHostPlayer *pHost = GetHostPlayer();

		if (m_pPlayer)
		{
			pHost->SyncDemonstrationPlayer(m_pPlayer);
			m_pPlayer->Stand();
			m_pPlayer->Tick(g_pGame->GetTickTime());

			if( m_pBtn_LeftTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
				m_nAngle = (m_nAngle + 360 - GetGame()->GetTickTime() / 6 ) % 360;
			else if( m_pBtn_RightTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
				m_nAngle = (m_nAngle + GetGame()->GetTickTime() / 6) % 360;

			m_pImg_Char->SetRenderCallback(
				PlayerRenderDemonstration,
				(DWORD)m_pPlayer,
				m_nAngle);
		}
		ACString strText;
		strText.Format(GetGameUIMan()->GetStringFromTable(792),pHost->GetBasicProps().iLevel);
		strText = strText + _AL(" ") + pHost->GetName();
		if (pHost->GetRebornInfo().size()>0)
		{
			strText = _AL(" ") + strText;
			strText = GetGameUIMan()->GetStringFromTable(7012) + strText;
		}
	    if(NULL != m_pTxt_Name)
		{
		  m_pTxt_Name->SetText(strText);
		  m_pTxt_Name->SetAlign(AUIFRAME_ALIGN_CENTER);
		}

	}
	else if( m_szName == "Win_CharInfo" )
	{
		RefreshHostInfo();
	}
	else if( m_szName == "Win_Char_Nature" )
	{
		RefreshHostInfo();
		RefreshHostNature();

		ACString strText;
		CECHostPlayer *pHost = GetHostPlayer();
		strText.Format(GetGameUIMan()->GetStringFromTable(792),pHost->GetBasicProps().iLevel);
		strText = strText + _AL(" ") + pHost->GetName();
		if (pHost->GetRebornInfo().size()>0)
		{
			strText = _AL(" ") + strText;
			strText = GetGameUIMan()->GetStringFromTable(7012) + strText;
		}
		if(NULL != m_pTxt_Name)
		{
			m_pTxt_Name->SetText(strText);
			m_pTxt_Name->SetAlign(AUIFRAME_ALIGN_CENTER);
		}
	}
	else if( m_szName == "Win_Character_Nature_Sub01")
	{
	    RefreshHostNatureBasic();
	}
	else if( m_szName == "Win_Character_Nature_Sub02")
	{
        RefreshHostNatureAdvanced();
	}
	else if( m_szName == "Win_Character_Nature_Sub03")
	{
        RefreshHostNatuerResistance();
	}
	else if( m_szName == "Win_CharOther" )
	{
		CheckRadioButton(2, GetHostPlayer()->InFashionMode() ? 2 : 1);
	}
	else if( m_szName == "Win_Credit"|| m_szName == "Win_Credit2" )
	{
		ACHAR szText[100];

		for(int i = 0; i < GetHostPlayer()->GetRegionReputationCount(); i++)
		{
			char szName[20];
			sprintf(szName, "Txt_Name%02d", i + 1);
			PAUILABEL pLabel = (PAUILABEL)GetDlgItem(szName);
			if( !pLabel )
				continue;
			
			pLabel->SetAlign(AUIFRAME_ALIGN_CENTER);
			int nReputation = GetHostPlayer()->GetRegionReputation(i);
			int level = 0;
			int nBase = 1140;
			const int *lvl = regionreputationlevel;

			if( i == 5 )
				nBase = 1150;
			else if( i == 6 )
			{
				nBase = 1160;
				lvl = affectionlevel;
			}
			else if( i == 7 )
			{
				nBase = 1170;
				lvl = literarytalentlevel;
			}
			else if( i == 8 )
			{
				lvl = teacherlevel;
			}
			else if(i == 22)
			{
				a_sprintf(szText, _AL("%d"), nReputation);
				pLabel->SetText(szText);
				continue;
			}
			else if(i == 23)
			{
				a_sprintf(szText, _AL("%d"), nReputation);
				pLabel->SetText(szText);
				continue;
			}
			else if(i == 24)
			{
				a_sprintf(szText, _AL("%d"), nReputation);
				pLabel->SetText(szText);
				continue;
			}
			else if(i == 25)
			{
				nBase = 1230;
				lvl = tianzong;
			}
			else if(i == 26)
			{
				nBase = 1240;
				lvl = dizong;
			}
			else if(i == 27)
			{
				nBase = 1250;
				lvl = renzong; 
			}
			while( nReputation >= lvl[level + 1] )
				level++;
			pLabel->SetText(GetStringFromTable(nBase + level));
			sprintf(szName, "Pro_%02d", i + 1);
			PAUIPROGRESS pProgress = (PAUIPROGRESS)GetDlgItem(szName);
			int nNowReputation = nReputation - lvl[level];
			int nMaxReputation = lvl[level + 1] - lvl[level];
			pProgress->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
			a_sprintf(szText, _AL("%d/%d"), nReputation, lvl[level + 1]);
			pProgress->SetHint(szText);
		}
		PAUILABEL pLabel = (PAUILABEL)GetDlgItem("Txt_Battlescore");
		if( pLabel )
		{
			pLabel->SetAlign(AUIFRAME_ALIGN_CENTER);
			int nReputation = GetHostPlayer()->GetBattleScore();
			int level = 0;
			const int *lvl = battlescorelevel;
			while( nReputation >= lvl[level + 1] )
				level++;
			pLabel->SetText(GetStringFromTable(1180 + level));
			PAUIPROGRESS pProgress = (PAUIPROGRESS)GetDlgItem("Pro_Battle");
			if( pProgress )
			{
				int nNowReputation = nReputation - lvl[level];
				int nMaxReputation = lvl[level + 1] - lvl[level];
				pProgress->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
				a_sprintf(szText, _AL("%d/%d"), nReputation, lvl[level + 1]);
				pProgress->SetHint(szText);
			}
		}
	}
	else if ( m_szName == "Win_Character_Hide" )
	{
		PAUICHECKBOX	pChk_HideHead = (PAUICHECKBOX)GetDlgItem("Chk_HideCap");
		PAUICHECKBOX	pChk_HideMask = (PAUICHECKBOX)GetDlgItem("Chk_HideMask");
		if (pChk_HideMask->IsShow())
		{
			pChk_HideMask->Enable(!pChk_HideHead->IsChecked());
		}
	}
}

void CDlgCharacter::OnCommand_Fashion(const char * szCommand)
{
	if( (0 == stricmp(szCommand, "Rdo_Fash") && !GetHostPlayer()->InFashionMode()) ||
		(0 == stricmp(szCommand, "Rdo_Equip") && GetHostPlayer()->InFashionMode())  )
	{
		GetGameSession()->c2s_CmdFashionSwitch();
	}
}

void CDlgCharacter::OnCommand_ChangeFashion(const char * szCommand)
{
	ChangeFashion(m_pCombo_Fashion->GetCurSel());
}

void CDlgCharacter::ChangeFashion(int index)
{
	int iSlot = -1;

	m_pCombo_Fashion->SetCurSel(index);
	CECInventory *pInventory = GetHostPlayer()->GetFashionPack();
	CECHostPlayer::FASHION_HOTKEY fHotkey = GetHostPlayer()->GetFashionHotkey(index);
	if ( fHotkey.id_head>=0 )
	{
		iSlot = pInventory->FindItem(fHotkey.id_head);
		if ( iSlot != -1)
		{
			if ( ((CECIvtrEquip*)pInventory->GetItem(iSlot))->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP )
			{
				PAUIDIALOG pMsgBox;
				GetGameUIMan()->MessageBox("Game_EquipBind3", GetStringFromTable(7331), 
					MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				pMsgBox->SetData(index);
				return;
			}
		}
	}
	if ( fHotkey.id_body>=0 )
	{
		iSlot = pInventory->FindItem(fHotkey.id_body);
		if ( iSlot != -1)
		{
			if ( ((CECIvtrEquip*)pInventory->GetItem(iSlot))->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP )
			{
				PAUIDIALOG pMsgBox;
				GetGameUIMan()->MessageBox("Game_EquipBind3", GetStringFromTable(7331), 
					MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				pMsgBox->SetData(index);
				return;
			}
		}
	}
	if ( fHotkey.id_shoe>=0 )
	{
		iSlot = pInventory->FindItem(fHotkey.id_shoe);
		if ( iSlot != -1)
		{
			if ( ((CECIvtrEquip*)pInventory->GetItem(iSlot))->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP )
			{
				PAUIDIALOG pMsgBox;
				GetGameUIMan()->MessageBox("Game_EquipBind3", GetStringFromTable(7331), 
					MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				pMsgBox->SetData(index);
				return;
			}
		}
	}
	UseFashionItems(index);
	// 播放换装特效
	if( fHotkey.id_head > 0 || fHotkey.id_body > 0 || fHotkey.id_shoe > 0)
	{
		GetHostPlayer()->GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_CHANGE_FASHION), NULL);
	}
	if (!GetHostPlayer()->InFashionMode())
		GetGameSession()->c2s_CmdFashionSwitch();
}

void CDlgCharacter::UseFashionItems(int index)
{
	int iSlot = -1;
	CECInventory *pInventory = GetHostPlayer()->GetPack();
	CECInventory *pInventoryFasion = GetHostPlayer()->GetFashionPack();
	CECHostPlayer::FASHION_HOTKEY fHotkey = GetHostPlayer()->GetFashionHotkey(index);
	if ( fHotkey.id_head>=0 )
	{
		iSlot = pInventoryFasion->FindItem(fHotkey.id_head);
		if ( iSlot != -1)
			GetHostPlayer()->UseItemInPack(IVTRTYPE_FASHION, iSlot);
//		else if(pInventory->FindItem(fHotkey.id_head)>=0)
//			GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, iSlot);
	}
	else if (m_pImgEquip[EQUIPIVTR_FASHION_HEAD]->GetDataPtr("ptr_CECIvtrItem"))
	{
		OnEventLButtonDblclk(0,0,m_pImgEquip[EQUIPIVTR_FASHION_HEAD]);
	}
	if ( fHotkey.id_body>=0 )
	{
		iSlot = pInventoryFasion->FindItem(fHotkey.id_body);
		if ( iSlot != -1)
			GetHostPlayer()->UseItemInPack(IVTRTYPE_FASHION, iSlot);
//		else if(pInventory->FindItem(fHotkey.id_body)>=0)
//			GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, iSlot);
	}
	else if (m_pImgEquip[EQUIPIVTR_FASHION_BODY]->GetDataPtr("ptr_CECIvtrItem"))
	{
		OnEventLButtonDblclk(0,0,m_pImgEquip[EQUIPIVTR_FASHION_BODY]);
	}
	if ( fHotkey.id_shoe>=0 )
	{
		iSlot = pInventoryFasion->FindItem(fHotkey.id_shoe);
		if ( iSlot != -1)
			GetHostPlayer()->UseItemInPack(IVTRTYPE_FASHION, iSlot);
//		else if(pInventory->FindItem(fHotkey.id_shoe)>=0)
//			GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, iSlot);
	}
	else if (m_pImgEquip[EQUIPIVTR_FASHION_FOOT]->GetDataPtr("ptr_CECIvtrItem"))
	{
		OnEventLButtonDblclk(0,0,m_pImgEquip[EQUIPIVTR_FASHION_FOOT]);
	}
}

void CDlgCharacter::OnCommand_ShowEquip(const char * szCommand)
{
	bool bCheck = !m_pChk_ShowEquip->IsChecked();
	int i;
	for(i = 0; i < SIZE_EQUIPIVTR; i++ )
	{
		char szItem[20];
		sprintf(szItem, "Equip_%02d", i);
		PAUIIMAGEPICTURE pDlg = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_Character")->GetDlgItem(szItem);
		if (pDlg)
			pDlg->Show(bCheck);
	}
}

void CDlgCharacter::OnCommand_Page(const char * szCommand)
{

	if(strstr(szCommand, "Rdo_Nature"))
	{
		this->GetDlgItem("Sub_01")->Show(GetCheckedRadioButton(1) == 1);
		this->GetDlgItem("Sub_02")->Show(GetCheckedRadioButton(1) == 2);
		this->GetDlgItem("Sub_03")->Show(GetCheckedRadioButton(1) == 3);
        return;
	}
	GetGameUIMan()->GetDialog("Win_Char_Nature")->Show(false);
	Show(false);
	PAUIDIALOG pDlg = NULL;
	switch(GetCheckedRadioButton(1))
	{
	case 1:
		pDlg = GetGameUIMan()->GetDialog("Win_Character");
		break;
	case 2:
		pDlg = GetGameUIMan()->GetDialog("Win_Credit");
		break;
	case 3:
		pDlg = GetGameUIMan()->GetDialog("Win_Char_TitleBox");
		break;
	case 4:
		pDlg = GetGameUIMan()->GetDialog("Win_CharInfo");
		break;
	case 5:
		pDlg = GetGameUIMan()->GetDialog("Win_Fengshen_Soul");
		break;
	}

	if( pDlg )
	{
		pDlg->Show(true);
		pDlg->SetPosEx(GetPos().x, GetPos().y);
	}
}

void CDlgCharacter::OnCommand_Credit(const char * szCommand)
{
	GetGameUIMan()->GetDialog("Win_Char_Nature")->Show(false);
	Show(false);
	PAUIDIALOG pDlg = NULL;

	switch(GetCheckedRadioButton(2))
	{
	case 1:
		pDlg = GetGameUIMan()->GetDialog("Win_Credit");
		break;
	case 2:
		pDlg = GetGameUIMan()->GetDialog("Win_Credit2");
		break;
	}

	if( pDlg )
	{
		pDlg->Show(true);
		pDlg->SetPosEx(GetPos().x, GetPos().y);
	}

}

void CDlgCharacter::OnCommand_HideSpecEquip(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Character_Hide");
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECHostPlayer* pPlayer = pMan->GetHostPlayer();
	PAUICHECKBOX	pChk_HideHead = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_HideCap");
	PAUICHECKBOX	pChk_HideBody = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_HideClothes");
	PAUICHECKBOX	pChk_HideFoot = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_HideShoes");
	PAUICHECKBOX	pChk_HideMask = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_HideMask");
	
	pChk_HideHead->Check(pPlayer->IsHeadHide());
	pChk_HideBody->Check(pPlayer->IsBodyHide());
	pChk_HideFoot->Check(pPlayer->IsFootHide());
	pChk_HideMask->Check(!pPlayer->IsMaskHide());
	pChk_HideMask->Show(GetHostPlayer()->GetProfession()>=56&&GetHostPlayer()->GetProfession()<=60);
	pDlg->Show(!pDlg->IsShow());
	LuaEvent::FirstHideEquip();
}

void CDlgCharacter::OnCommand_SelectTitle(const char * szCommand)
{
	m_nNewTitleId = m_pCombo_Title->GetItemData(m_pCombo_Title->GetCurSel());
	if( m_nNewTitleId == GetHostPlayer()->GetTitle() )
		m_nNewTitleId = -1;
}

void CDlgCharacter::OnCommand_TitleExp(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_TitleExp");
	//pDlg->Show(!pDlg->IsShow());
	AString strDlg = "Win_TitleExp";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgCharacter::OnCommand_ManaHelp(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Character_Mana_Help");
	//pDlg->Show(!pDlg->IsShow());
	AString strDlg = "Win_Character_Mana_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgCharacter::OnCommand_CreditHelp(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Credit_Help");
	//pDlg->Show(!pDlg->IsShow());
	AString strDlg = "Win_Credit_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgCharacter::OnCommand_Confirm(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Character_Hide");
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECHostPlayer* pPlayer = pMan->GetHostPlayer();
	PAUICHECKBOX	pChk_HideHead = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_HideCap");
	PAUICHECKBOX	pChk_HideBody = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_HideClothes");
	PAUICHECKBOX	pChk_HideFoot = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_HideShoes");
	PAUICHECKBOX	pChk_HideMask = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_HideMask");
	DWORD			dwFashionMask;
//	dwFashionMask = pPlayer->GetHideEquipMask();
	dwFashionMask = 0;
	if(pChk_HideHead->IsChecked())
	{
		dwFashionMask |= (1<<EQUIPIVTR_HEAD);
		dwFashionMask |= (1<<EQUIPIVTR_FASHION_HEAD);
	}
	else
	{
		dwFashionMask &= ~(1<<EQUIPIVTR_HEAD);
		dwFashionMask &= ~(1<<EQUIPIVTR_FASHION_HEAD);
	}
	if(pChk_HideBody->IsChecked())
	{
		dwFashionMask |= (1<<EQUIPIVTR_BODY);
		dwFashionMask |= (1<<EQUIPIVTR_FASHION_BODY);		
		dwFashionMask |= (1<<EQUIPIVTR_FASHION_LEG);
	}
	else
	{
		dwFashionMask &= ~(1<<EQUIPIVTR_BODY);
		dwFashionMask &= ~(1<<EQUIPIVTR_FASHION_BODY);
		dwFashionMask &= ~(1<<EQUIPIVTR_FASHION_LEG);
	}
	if(pChk_HideFoot->IsChecked())
	{
		dwFashionMask |= (1<<EQUIPIVTR_FOOT);
		dwFashionMask |= (1<<EQUIPIVTR_FASHION_FOOT);		
	}
	else
	{
		dwFashionMask &= ~(1<<EQUIPIVTR_FOOT);
		dwFashionMask &= ~(1<<EQUIPIVTR_FASHION_FOOT);		
	}
	if (pChk_HideMask->IsShow())
	{
		if(!pChk_HideMask->IsChecked())
		{
			dwFashionMask |= (1<<EQUIPIVTR_FASHION_EYE);
		}
		else
		{
			dwFashionMask &= ~(1<<EQUIPIVTR_FASHION_EYE);		
		}
	}

	pPlayer->CmdSetHideEquipMask(dwFashionMask);
	pDlg->Show(false);
}

void CDlgCharacter::OnCommand_SetInfo(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_PersInfo")->Show(true);
}

void CDlgCharacter::OnCommand_Para(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Char_Nature");
	pDlg->Show(!pDlg->IsShow());
	PAUIDIALOG pDlg2 = GetGameUIMan()->GetDialog("Win_Equip_Rune");
	if (pDlg2)
	{
		if (!pDlg->IsShow() && pDlg2->IsShow())
			pDlg2->Show(false);
		else if (pDlg->IsShow())
		{
			if (g_pGame->GetConfigs()->GetLocalSettings().bAutoShowEquipRune)
				pDlg2->Show(true);
		}
	}
}

void CDlgCharacter::OnCommand_DungeonAward(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Tower_Result");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgCharacter::OnCommand_DungeonRank(const char * szCommand)
{
	CDlgRankList* pDlg = (CDlgRankList*)GetGameUIMan()->GetDialog("Win_RankList");
	pDlg->Show(true);
	if(pDlg->IsShow())
	{
		pDlg->ShowTowerRank();
	}
}

void CDlgCharacter::OnCommand_DungeonMonster(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Character_Dungeon_Book");
	pDlg->Show(!pDlg->IsShow());
}
	
void CDlgCharacter::OnCommand_DungeonFly(const char *szCommand)
{
    GetGameUIMan()->CheckCanDailyPrompFly(DUNGEONTOWER_DAILYPROMP_ID);
}

void CDlgCharacter::OnCommand_Cancel(const char * szCommand)
{
	Show(false);
	if( m_szName == "Win_Character" )
		GetGameUIMan()->GetDialog("Win_Char_Nature")->Show(false);
}

void CDlgCharacter::OnCommand_RemedyMetempsychosis(const char * szCommand)
{
	DlgRemedyMetempsychosis* pDlg = (DlgRemedyMetempsychosis*)GetGameUIMan()->GetDialog("Win_Character_Attrimakeup");
	if(pDlg)
	{
		pDlg->Show(true);
	}
}

void CDlgCharacter::onCommand_RoleRune(const char * szCommand)
{
	AUIDialog* pDlg = GetGameUIMan()->GetDialog("Win_Equip_Rune");
	if (pDlg)
	{
		pDlg->Show(!pDlg->IsShow());
		CECConfigs* pConfig = g_pGame->GetConfigs();
		EC_LOCAL_SETTING ls = pConfig->GetLocalSettings();
		ls.bAutoShowEquipRune = pDlg->IsShow();
		ls.bFuWenPop1Showed = true;
		pConfig->SetLocalSettings(ls);
		pConfig->SaveLocalSetting();
		ShowRuneTipPop();
	}
}

void CDlgCharacter::UpdateTitle()
{
	PAUITREEVIEW pTree = (PAUITREEVIEW)GetDlgItem("Tv_Title");
	pTree->DeleteAllItems();
	abase::hash_map<int, bool> mapTitleID;
	mapTitleID.clear();
	DWORD i, j, k;
	int idTitle;
	for(i = 0; i < (DWORD)GetHostPlayer()->GetTitleCount(); i++ )
		mapTitleID[GetHostPlayer()->GetOneTitle(i)] = true;
	CConfigFromLua * pConfigFromLua = GetGame()->GetConfigFromLua();
	if( !pConfigFromLua )
		return;
	
	ACString strDesc, strHint;
	A3DCOLOR color;
	for(i = 0; i < pConfigFromLua->m_TitleDir[0].m_ArrVal.size(); i++ )
		if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_Type == CScriptValue::SVT_ARRAY )
		{
			if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[0].m_Type == CScriptValue::SVT_STRING )
			{
				//strDesc = pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[0].GetAWString();
				pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[0].RetrieveAWString(strDesc);
				P_AUITREEVIEW_ITEM pSubject = pTree->InsertItem(strDesc);
				for(j = 1; j < pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal.size(); j++ )
					if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_Type == CScriptValue::SVT_NUMBER )
					{
						idTitle = pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].GetInt();
						if( mapTitleID.find(idTitle) != mapTitleID.end() )
						{
							strDesc = GetTitle(idTitle, &color);
							strHint = GetTitleHint(idTitle);
						}
						else
						{
							color = A3DCOLORRGB(255, 255, 255);
							strDesc = _AL("????????");
							strHint = GetTitleHint(idTitle, true);
						}
						P_AUITREEVIEW_ITEM pItem = pTree->InsertItem(strDesc, pSubject);
						pTree->SetItemHint(pItem, strHint);
						pTree->SetItemTextColor(pItem, color);
					}
					else if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_Type == CScriptValue::SVT_ARRAY )
					{
						if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal[0].m_Type == CScriptValue::SVT_STRING )
						{
							idTitle = -1;
							//strHint = pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal[0].GetAWString();
							pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal[0].RetrieveAWString(strHint);
							strDesc = _AL("????????");
							color = A3DCOLORRGB(255, 255, 255);
							for(k = 1; k < pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal.size(); k++ )
								if( pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal[k].m_Type == CScriptValue::SVT_NUMBER )
								{
									idTitle = pConfigFromLua->m_TitleDir[0].m_ArrVal[i].m_ArrVal[j].m_ArrVal[k].GetInt();
									if( mapTitleID.find(idTitle) != mapTitleID.end() )
									{
										strDesc = GetTitle(idTitle, &color);
										strHint = GetTitleHint(idTitle);
										break;
									}
								}
								P_AUITREEVIEW_ITEM pItem = pTree->InsertItem(strDesc, pSubject);
								pTree->SetItemHint(pItem, strHint);
								pTree->SetItemTextColor(pItem, color);
						}
					}
			}
		}
}

void CDlgCharacter::UpdateDungeonTower()
{
	// 图片描述
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Pic");
	AUIOBJECT_SETPROPERTY p;
	sprintf(p.fn, AC2AS(GetGameUIMan()->GetStringFromTable(11159)));
	pImg->SetProperty("Image File", &p);
	// 文字描述
	PAUITEXTAREA pTxt = (PAUITEXTAREA)GetDlgItem("Txt_Intro");
	pTxt->SetText(GetGameUIMan()->GetStringFromTable(11158));
	
	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_Score");
	pLst->ResetContent();
	ACString strText, strTime;
	for (int i=0; i<10; ++i)
	{
		strText.Format(GetGameUIMan()->GetStringFromTable(13402), i*10+1, i*10+10);
		int iTime = GetHostPlayer()->GetBattleDungeonTower()->GetTimeUsed(i);
		if (GetHostPlayer()->GetBattleDungeonTower()->GetTowerLevel()<((i+1)*10-1))
		{
			iTime = 0;
		}
		strTime.Format(_AL("%d:%02d:%02d"), iTime/3600, (iTime%3600)/60, iTime%60);
		pLst->AddString(strText+_AL("\t")+strTime);
	}
}

void CDlgCharacter::UpdateEquip()
{
	PAUIIMAGEPICTURE pObj;
	CECIvtrItem *pItem, *pDrag = NULL;
	CECIvtrEquip *pEquip;
	ACHAR szText[100];
	PAUIDIALOG pDlgDrag = m_pAUIManager->GetDialog("DragDrop");
	bool bSpecEquip = false;

	if( pDlgDrag->IsShow() )
	{
		PAUIOBJECT pCell = pDlgDrag->GetDlgItem("Goods");
		PAUIOBJECT pObjSrc = (PAUIOBJECT)pCell->GetDataPtr("ptr_AUIObject");
		PAUIDIALOG pDlgSrc = pObjSrc->GetParent();

		if( stricmp(pDlgSrc->GetName(), "Win_Inventory") == 0 )
			pDrag = (CECIvtrItem *)pObjSrc->GetDataPtr("ptr_CECIvtrItem");
	}

	CECInventory *pInventory = GetHostPlayer()->GetEquipment();
	for(int i = 0; i < SIZE_EQUIPIVTR; i++ )
	{
		bSpecEquip = false;
		pObj = m_pImgEquip[i];
		if( !pObj ) continue;

		pItem = pInventory->GetItem(i);
		if( pItem )
		{
			pEquip = (CECIvtrEquip *)pItem;
			if( !GetHostPlayer()->CanUseEquipment(pEquip) )
				pObj->SetColor(A3DCOLORRGB(192, 0, 0));
			else if( i < EQUIPIVTR_FASHION_EYE )
			{
				bool bUseOut = false;
				bUseOut = pEquip->GetCurEndurance() ? false : true;

				if (bUseOut)
					pObj->SetColor(A3DCOLORRGB(128, 128, 128));
				else
					pObj->SetColor(A3DCOLORRGB(255, 255, 255));
			}
			else
				pObj->SetColor(A3DCOLORRGB(255, 255, 255));

			pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");
			pObj->SetData(CECGameUIMan::ICONS_INVENTORY);
			if( pItem->GetCount() > 1 )
			{
				a_sprintf(szText, _AL("%d"), pItem->GetCount());
				pObj->SetText(szText);
			}
			else
				pObj->SetText(_AL(""));

			AString strFile;
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
			WORD num_estone = pEquip->GetEssence().num_estone;
			if (pItem->GetClassID() == CECIvtrItem::ICID_EQUIP && num_estone >= 9)
				bSpecEquip = true;
			if (pItem->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART && ((CECIvtrTalismanMain*)pItem)->GetRebornCount()>0)
				bSpecEquip = true;
			if (pItem->GetClassID()==CECIvtrItem::ICID_RUNE && ((CECIvtrRune*)pItem)->GetEssence().quality >= 3)
				bSpecEquip = true;
			if( pDrag && pDrag->CanEquippedTo(i) )
				pObj->SetCover(GetGameUIMan()->m_pA2DSpriteMask, 0, 1);
			else if (bSpecEquip && pItem->IsLocked())
				pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquipLock, (GetTickCount() / 200) % 6, 1);
			else
			{
				if (bSpecEquip )
					pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquip, (GetTickCount() / 200) % 6, 1);
				else if (pItem->IsLocked() )
					pObj->SetCover(CDlgItemLock::m_pA2DSpriteLock, 0, 1);
				else
					pObj->SetCover(NULL, -1, 1);
			}
		}
		else
		{
			pObj->ClearCover();
			pObj->SetText(_AL(""));
			pObj->SetDataPtr(NULL);
			pObj->SetColor(A3DCOLORRGB(255, 255, 255));
			if (i==EQUIPIVTR_RUNE && !GetHostPlayer()->IsRuneEquipOpen())
			{
				pObj->SetHint(GetGameUIMan()->GetStringFromTable(363));
			}
			else
				pObj->SetHint(GetStringFromTable(310 + i));
		}
	}

	m_pImgEquip[EQUIPIVTR_RUNE]->FixFrame(GetHostPlayer()->IsRuneEquipOpen() ? 0:1);
}

void CDlgCharacter::RefreshHostInfo()
{
	bool bCrossServer = GetHostPlayer()->GetSrcServerID()!=0;
	m_pTxt_Faction->Show(!bCrossServer);
	m_pTxt_Family->Show(!bCrossServer);
	if(NULL != m_pTxt_Contribution)
	    m_pTxt_Contribution->Show(!bCrossServer);
	if(NULL != GetDlgItem("Lab_Contribution"))
	   GetDlgItem("Lab_Contribution")->Show(!bCrossServer);
	if(NULL != m_pTxt_Devotion)
	    m_pTxt_Devotion->Show(!bCrossServer);
	if(NULL != GetDlgItem("Lab_Devotion"))
	   GetDlgItem("Lab_Devotion")->Show(!bCrossServer);
	ACHAR szText[256];
	ACString strText;
	ROLEBASICPROP &rbp = GetHostPlayer()->GetBasicProps();
	ROLEEXTPROP &rep = GetHostPlayer()->GetExtendProps();

	Faction_Info *pInfo = NULL;
	int idFaction = GetHostPlayer()->GetFactionID();
	ACString strFaction = _AL("");
	CECFactionMan *pFMan = GetGame()->GetFactionMan();
	if( idFaction ) pInfo = pFMan->GetFaction(idFaction);
	if( pInfo )
	{
		ACHAR szFName[20];
		AUI_ConvertChatString(pInfo->GetName(), szFName);
		strFaction.Format(GetStringFromTable(768), szFName, 
			GetStringFromTable(1300 + GetHostPlayer()->GetFRoleID()));
	}
	m_pTxt_Faction->SetText(strFaction); 
	
	int idFamily = GetHostPlayer()->GetFamilyID();
	ACString strFamily = _AL("");
	if( idFamily ) pInfo = pFMan->GetFaction(idFamily, false, true);
	if( pInfo )
	{
		ACHAR szFName[20];
		AUI_ConvertChatString(pInfo->GetName(), szFName);
		strFamily = szFName;
	}
	m_pTxt_Family->SetText(strFamily);

	CECHostPlayer *pHostPlayer = GetHostPlayer();
	int iRebornCount = pHostPlayer->GetRebornInfo().size();
	/// 关闭 弥补飞升按钮
	m_pAttrimakeup->Show(false);
	if ( 1 == iRebornCount )
	{
		a_sprintf(szText, _AL("%d"), (pHostPlayer->GetRebornInfo())[0].level);
		m_pLab_Level1->SetText(szText);
		m_pImg_Prof1->FixFrame((pHostPlayer->GetRebornInfo())[0].prof);
		
		if((pHostPlayer->GetRebornInfo())[0].level < 150) // 2012-10-17 add 显示弥补飞升按钮 by zy 
		{
			/// 显示 弥补飞升按钮
			m_pAttrimakeup->Show(true);
		}
	}
	else if ( 2 == iRebornCount )
	{
		a_sprintf(szText, _AL("%d"), (pHostPlayer->GetRebornInfo())[0].level);
		m_pLab_Level1->SetText(szText);
		a_sprintf(szText, _AL("%d"), (pHostPlayer->GetRebornInfo())[1].level);
		m_pLab_Level2->SetText(szText);
		m_pImg_Prof1->FixFrame((pHostPlayer->GetRebornInfo())[0].prof);
		m_pImg_Prof2->FixFrame((pHostPlayer->GetRebornInfo())[1].prof);
	}
	else if ( 3 == iRebornCount )
	{
		a_sprintf(szText, _AL("%d"), (pHostPlayer->GetRebornInfo())[0].level);
		m_pLab_Level1->SetText(szText);
		a_sprintf(szText, _AL("%d"), (pHostPlayer->GetRebornInfo())[1].level);
		m_pLab_Level2->SetText(szText);
		a_sprintf(szText, _AL("%d"), (pHostPlayer->GetRebornInfo())[2].level);
		m_pLab_Level3->SetText(szText);
		m_pImg_Prof1->FixFrame((pHostPlayer->GetRebornInfo())[0].prof);
		m_pImg_Prof2->FixFrame((pHostPlayer->GetRebornInfo())[1].prof);
		m_pImg_Prof3->FixFrame((pHostPlayer->GetRebornInfo())[2].prof);
	}
	
	if ( 0 == iRebornCount )
	{
		strText.Format(GetStringFromTable(769), rbp.iLevel,
			m_pAUIManager->GetStringFromTable(PROFESSION_START + pHostPlayer->GetProfession()));
	}
	else
	{
		strText.Format(GetStringFromTable(7010), rbp.iLevel,
			m_pAUIManager->GetStringFromTable(PROFESSION_START + pHostPlayer->GetProfession()),
			GetStringFromTable(7000 +pHostPlayer->GetCultivation()));
		
	}
	
	m_pTxt_Level->SetText(strText);

	if ( 0 == iRebornCount )
	{
		m_pLab_Level1->Show(false);
		m_pImg_Prof1->Show(false);
		m_pLab_Level2->Show(false);
		m_pImg_Prof2->Show(false);
		m_pLab_Level3->Show(false);
		m_pImg_Prof3->Show(false);
	}
	else if ( 1 == iRebornCount )
	{
		m_pLab_Level1->Show(true);
		m_pImg_Prof1->Show(true);
		m_pLab_Level2->Show(false);
		m_pImg_Prof2->Show(false);
		m_pLab_Level3->Show(false);
		m_pImg_Prof3->Show(false);
	}
	else if ( 2 == iRebornCount )
	{
		m_pLab_Level1->Show(true);
		m_pImg_Prof1->Show(true);
		m_pLab_Level2->Show(true);
		m_pImg_Prof2->Show(true);
		m_pLab_Level3->Show(false);
		m_pImg_Prof3->Show(false);
	}

	if( GetHostPlayer()->GetSpouse() )
	{
		const ACHAR *szSpouseName = GetGameRun()->GetPlayerName(GetHostPlayer()->GetSpouse(), false);
		if( szSpouseName )
			m_pTxt_Mate->SetText(szSpouseName);
		else
			m_pTxt_Mate->SetText(_AL(""));
	}
	else
		m_pTxt_Mate->SetText(GetStringFromTable(780));

	strText.Format(_AL("%d"), GetHostPlayer()->GetPKValue());
	m_pTxt_PK->SetText(strText);
	m_pTxt_PK->SetColor(GetHostPlayer()->GetNameColor());
	m_pTxt_PK->SetColor(GetHostPlayer()->GetNameColor());

	a_sprintf(szText, _AL("%d"), GetHostPlayer()->GetContribution());
	if(NULL != m_pTxt_Contribution)
	   m_pTxt_Contribution->SetText(szText);
	a_sprintf(szText, _AL("%d"), GetHostPlayer()->GetTalismanValue());
	m_pTxt_Combine->SetText(szText);
	a_sprintf(szText, _AL("%d"), GetHostPlayer()->GetFamilyContrib());
	if(NULL != m_pTxt_Devotion)
	   m_pTxt_Devotion->SetText(szText);

	int nReputation = GetHostPlayer()->GetReputation();
	int level = 0;
	while( nReputation >= reputationlevel[level + 1] )
		level++;
	int nNowReputation = nReputation - reputationlevel[level];
	int nMaxReputation = reputationlevel[level + 1] - reputationlevel[level];
	m_pPro_Mana->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
	a_sprintf(szText, _AL("%d/%d"), nReputation, reputationlevel[level + 1]);
	m_pPro_Mana->SetHint(szText);
	m_pTxt_Mana->SetText(GetStringFromTable(1100 + level));

	int nCount = GetHostPlayer()->GetTitleCount();
	
	EditBoxItemBase itemImage(enumEIImage);
	itemImage.SetImageIndex(0);
	itemImage.SetImageFrame(0);
	ACString strTitle, state = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
	
	A3DCOLOR color;
	if( GetHostPlayer()->GetTitle() == 0 )
		m_pCombo_Title->SetCurSel(0);
	for(int i = 0; i < nCount; i++ )
	{
		int idTitle = GetHostPlayer()->GetOneTitle(i);
		int type = 0;
		strTitle = GetTitle(idTitle, &color, &type);
		if(type == 1)
			strTitle = state + strTitle;
		if( m_pCombo_Title->GetCount() - 1 <= i  )
		{
			m_pCombo_Title->AddString(strTitle);
			m_pCombo_Title->SetItemData(i + 1, idTitle);
			m_pCombo_Title->SetItemHint(i + 1, GetTitleHint(idTitle));
			m_pCombo_Title->SetItemTextColor(i + 1, color);
		}
		else if( strTitle != m_pCombo_Title->GetText(i + 1) )
		{
			m_pCombo_Title->SetText(i + 1, strTitle);
			m_pCombo_Title->SetItemData(i + 1, idTitle);
			m_pCombo_Title->SetItemHint(i + 1, GetTitleHint(idTitle));
			m_pCombo_Title->SetItemTextColor(i + 1, color);
		}
		if( idTitle == GetHostPlayer()->GetTitle() )
			m_pCombo_Title->SetCurSel(i + 1);
	}
	while( m_pCombo_Title->GetCount() > nCount + 1 )
		m_pCombo_Title->DeleteString(m_pCombo_Title->GetCount() - 1);
	DWORD dwTick = GetTickCount();
	if( m_nNewTitleId != -1 && DWORD(dwTick - m_dwLastSelectTitle) > 1000 )
	{
		m_dwLastSelectTitle = dwTick;
		GetGameSession()->c2s_SendCmdSelectTitle(m_nNewTitleId);
		m_nNewTitleId = -1;
	}
//	AUIOBJECT_SETPROPERTY p, pBack;
//	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Character");
//	PAUIIMAGEPICTURE pImageBackInfo = (PAUIIMAGEPICTURE)GetDlgItem("Img_InfoBack");
//	PAUIOBJECT m_pLab_Spec = pDlg->GetDlgItem("Shape4");
//	int	iCultivation = GetHostPlayer()->GetCultivation();
//
// 	if ( m_iCultivation != iCultivation )
// 	{
// 		m_iCultivation = iCultivation;
// 		if ( 0 == iCultivation )
// 		{
// 			sprintf(p.fn, "Window\\WindowStandard2.tga");
// 			sprintf(pBack.fn, "Window\\InfoBack.tga");
// 			m_pLab_Spec->Show(false);
// 		}
// 		else if ( 1 == iCultivation )
// 		{
// 			sprintf(p.fn, "Window\\WindowStandard2仙.tga");
// 			sprintf(pBack.fn, "Window\\InfoBack仙.tga");
// 			m_pLab_Spec->Show(true);
// 		}
// 		else if ( 2 == iCultivation )
// 		{
// 			sprintf(p.fn, "Window\\WindowStandard2魔.tga");
// 			sprintf(pBack.fn, "Window\\InfoBack魔.tga");
// 			m_pLab_Spec->Show(true);
// 		}
// 		else if ( 4 == iCultivation )
// 		{
// 			sprintf(p.fn, "Window\\WindowStandard2佛.tga");
// 			sprintf(pBack.fn, "Window\\InfoBack佛.tga");
// 			m_pLab_Spec->Show(true);
// 		}
// 		pDlg->SetProperty("Frame Image", &p);
// 		pImageBackInfo->SetProperty("Image File", &pBack);
// 	}
}
void CDlgCharacter::RefreshHostNatureBasic()
{
	ACHAR szText[256];
	ACString strText;
	ROLEBASICPROP &rbp = GetHostPlayer()->GetBasicProps();
	ROLEEXTPROP &rep = GetHostPlayer()->GetExtendProps();
	CECHostPlayer *pHostPlayer = GetHostPlayer();

	a_sprintf(szText, _AL("%d-%d"), rep.damage_low, rep.damage_high);
	m_pTxt_Damage->SetText(szText);

	a_sprintf(szText, _AL("%d"), rep.attack);
	m_pTxt_Definition->SetText(szText);

	a_sprintf(szText, _AL("%d"), rep.armor);
	m_pTxt_Evade->SetText(szText);


	a_sprintf(szText, _AL("%.1f%%"), rep.crit_rate / 10.0f);
	m_pTxt_Critical->SetText(szText);

	a_sprintf(szText, _AL("%.1f%%"), rep.crit_damage * 100 + 0.001);
	m_pTxt_CriticalDamage->SetText(szText);


	a_sprintf(szText, _AL("%d"), rep.defense);
	m_pTxt_Defense->SetText(szText);

}
void CDlgCharacter::RefreshHostNatureAdvanced()
{
	ACHAR szText[256];
	ACString strText;
	ROLEBASICPROP &rbp = GetHostPlayer()->GetBasicProps();
	ROLEEXTPROP &rep = GetHostPlayer()->GetExtendProps();
	CECHostPlayer *pHostPlayer = GetHostPlayer();

	a_sprintf(szText, _AL("%.1f"), (100 + float(rep.skill_attack_rate)/10));
	m_pTxt_SkillDefinition->SetText(szText);

	a_sprintf(szText, _AL("%.1f%%"), rep.anti_crit / 10.0f);
	m_pTxt_DeCritical->SetText(szText);

	a_sprintf(szText, _AL("%.1f"), float(rep.skill_armor_rate) / 10);
	m_pTxt_SkillEvade->SetText(szText);

	a_sprintf(szText, _AL("%.1f%%"), rep.anti_crit_damage * 100 + 0.001);
	m_pTxt_DeCriticalDamage->SetText(szText);

	a_sprintf(szText, _AL("%.1f%%"), rep.cult_defense[0] / 10.0f);
	m_pTxt_DeDamageTaoism->SetText(szText);
	a_sprintf(szText, _AL("%.1f%%"), rep.cult_defense[1] / 10.0f);
	m_pTxt_DeDamageEvil->SetText(szText);
	a_sprintf(szText, _AL("%.1f%%"), rep.cult_defense[2] / 10.0f);
	m_pTxt_DeDamageBuddhism->SetText(szText);
}
void CDlgCharacter::RefreshHostNatuerResistance()
{
	//ACHAR szText[256];
	ACString strText;
	ROLEBASICPROP &rbp = GetHostPlayer()->GetBasicProps();
	ROLEEXTPROP &rep = GetHostPlayer()->GetExtendProps();
	CECHostPlayer *pHostPlayer = GetHostPlayer();


	strText.Format(_AL("%d"), rep.resistance[0]);
	m_pTxt_StunRes->SetText(strText);
	strText.Format(_AL("%d"), rep.resistance[3]);
	m_pTxt_SilenceRes->SetText(strText);
	strText.Format(_AL("%d"), rep.resistance[1]);
	m_pTxt_WeaknessRes->SetText(strText);
	strText.Format(_AL("%d"), rep.resistance[4]);
	m_pTxt_SleepRes->SetText(strText);
	strText.Format(_AL("%d"), rep.resistance[2]);
	m_pTxt_EnlaceRes->SetText(strText);

	strText.Format(_AL("%d"), rep.resistance_tenaciy[0]);
	m_pTxt_StunRes02->SetText(strText);
	strText.Format(_AL("%d"), rep.resistance_tenaciy[3]);
	m_pTxt_SilenceRes02->SetText(strText);
	strText.Format(_AL("%d"), rep.resistance_tenaciy[1]);
	m_pTxt_WeaknessRes02->SetText(strText);
	strText.Format(_AL("%d"), rep.resistance_tenaciy[4]);
	m_pTxt_SleepRes02->SetText(strText);
	strText.Format(_AL("%d"), rep.resistance_tenaciy[2]);
	m_pTxt_EnlaceRes02->SetText(strText);

	strText.Format(_AL("%d"), rep.resistance_proficiency[0]);
	m_pTxt_StunRes03->SetText(strText);
	strText.Format(_AL("%d"), rep.resistance_proficiency[3]);
	m_pTxt_SilenceRes03->SetText(strText);
	strText.Format(_AL("%d"), rep.resistance_proficiency[1]);
	m_pTxt_WeaknessRes03->SetText(strText);
	strText.Format(_AL("%d"), rep.resistance_proficiency[4]);
	m_pTxt_SleepRes03->SetText(strText);
	strText.Format(_AL("%d"), rep.resistance_proficiency[2]);
	m_pTxt_EnlaceRes03->SetText(strText);

	ACString szTemp;
	ACString szHint;
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18412), rep.resistance[0]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18428), rep.resistance_tenaciy[0]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18327), (1.0 - 100.0 / (100 + rep.resistance_tenaciy[0])) * 100);
	szHint += szTemp;
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18423), rep.resistance_proficiency[0]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18332), rep.resistance_proficiency[0]);
	szHint += szTemp;

	GetDlgItem("Lab_StunRes")->SetHint(szHint);
	GetDlgItem("Lab_StunRes02")->SetHint(szHint);
	GetDlgItem("Lab_StunRes03")->SetHint(szHint);

	szHint = _AL("");
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18413), rep.resistance[3]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18429), rep.resistance_tenaciy[3]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18329), (1.0 - 100.0 / (100 + rep.resistance_tenaciy[3])) * 100);
	szHint += szTemp;
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18424), rep.resistance_proficiency[3]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18334), rep.resistance_proficiency[3]);
	szHint += szTemp;

	GetDlgItem("Lab_SilenceRes")->SetHint(szHint);
	GetDlgItem("Lab_SilenceRes02")->SetHint(szHint);
	GetDlgItem("Lab_SilenceRes03")->SetHint(szHint);

	szHint = _AL("");
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18415), rep.resistance[1]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18431), rep.resistance_tenaciy[1]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18328), (1.0 - 100.0 / (100 + rep.resistance_tenaciy[1])) * 100);
	szHint += szTemp;
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18426), rep.resistance_proficiency[1]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18333), rep.resistance_proficiency[1]);
	szHint += szTemp;

	GetDlgItem("Lab_WeaknessRes")->SetHint(szHint);
	GetDlgItem("Lab_WeaknessRes02")->SetHint(szHint);
	GetDlgItem("Lab_WeaknessRes03")->SetHint(szHint);

	szHint = _AL("");
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18414), rep.resistance[4]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18430), rep.resistance_tenaciy[4]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18330), (1.0 - 100.0 / (100 + rep.resistance_tenaciy[4])) * 100);
	szHint += szTemp;
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18425), rep.resistance_proficiency[4]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18335), rep.resistance_proficiency[4]);
	szHint += szTemp;

	GetDlgItem("Lab_SleepRes")->SetHint(szHint);
	GetDlgItem("Lab_SleepRes02")->SetHint(szHint);
	GetDlgItem("Lab_SleepRes03")->SetHint(szHint);

	szHint = _AL("");
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18416), rep.resistance[2]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18432), rep.resistance_tenaciy[2]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18331), (1.0 - 100.0 / (100 + rep.resistance_tenaciy[2])) * 100);
	szHint += szTemp;
	szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18427), rep.resistance_proficiency[2]);
	szHint += szTemp;
	szHint += _AL("\r");
	szTemp.Format(GetStringFromTable(18336), rep.resistance_proficiency[2]);
	szHint += szTemp;

	GetDlgItem("Lab_EnlaceRes")->SetHint(szHint);
	GetDlgItem("Lab_EnlaceRes")->SetTransparent(false);
	GetDlgItem("Lab_EnlaceRes02")->SetHint(szHint);
	GetDlgItem("Lab_EnlaceRes02")->SetTransparent(false);
	GetDlgItem("Lab_EnlaceRes03")->SetHint(szHint);
	GetDlgItem("Lab_EnlaceRes03")->SetTransparent(false);
}
void CDlgCharacter::RefreshHostNature()
{
	ACHAR szText[256];
	ACString strText;
	ROLEBASICPROP &rbp = GetHostPlayer()->GetBasicProps();
	ROLEEXTPROP &rep = GetHostPlayer()->GetExtendProps();
	CECHostPlayer *pHostPlayer = GetHostPlayer();

	GetDlgItem("Btn_Equip_Rune")->Show(pHostPlayer->GetBasicProps().iLevel >= 150 && pHostPlayer->GetRebornCount() >= 1);

	double maxEXP = GetHostPlayer()->GetLevelUpExp(rbp.iLevel + 1, pHostPlayer->GetRebornCount());
	double percent;
	if( maxEXP == 0 )
		percent = 0.0;
	else
	{
		percent = rbp.exp / maxEXP;
		if( percent > 0.9999 )
			percent = 0.9999;
	}
	a_sprintf(szText, _AL("%.02f%%"), percent * 100.0);
	m_pTxt_Exp->SetText(szText);
	m_pPrg_Exp->SetProgress(int(percent * AUIPROGRESS_MAX));
	
	a_sprintf(szText, _AL("%d/%d"), rbp.iCurHP, rep.max_hp);
	m_pTxt_HP->SetText(szText);
	m_pPrg_HP->SetProgress(int(double(rbp.iCurHP) * AUIPROGRESS_MAX / rep.max_hp));
	
	if(rbp.iProfession >= 108 && rbp.iProfession <= 112)
	{
		a_sprintf(szText, _AL("%d"),rep.max_mp);
		m_pTxt_MP->SetText(szText);
		m_pPrg_MP->SetProgress(0);
	}
	else
	{
		a_sprintf(szText, _AL("%d/%d"), rbp.iCurMP, rep.max_mp);
		m_pTxt_MP->SetText(szText);
		m_pPrg_MP->SetProgress(int(double(rbp.iCurMP) * AUIPROGRESS_MAX / rep.max_mp));
	}	
	//a_sprintf(szText, _AL("%d-%d"), rep.damage_low, rep.damage_high);
	//m_pTxt_Damage->SetText(szText);
	//
	//a_sprintf(szText, _AL("%d"), rep.attack);
	//m_pTxt_Definition->SetText(szText);
	//
	//a_sprintf(szText, _AL("%d"), rep.armor);
	//m_pTxt_Evade->SetText(szText);
	//
	//a_sprintf(szText, _AL("%.1f"), (100 + float(rep.skill_attack_rate)/10));
	//m_pTxt_SkillDefinition->SetText(szText);
	//
	//a_sprintf(szText, _AL("%.1f"), float(rep.skill_armor_rate) / 10);
	//m_pTxt_SkillEvade->SetText(szText);
	//
	//a_sprintf(szText, _AL("%.1f%%"), rep.crit_rate / 10.0f);
	//m_pTxt_Critical->SetText(szText);
	//
	//a_sprintf(szText, _AL("%.1f%%"), rep.crit_damage * 100 + 0.001);
	//m_pTxt_CriticalDamage->SetText(szText);
	//
	//a_sprintf(szText, _AL("%.1f%%"), rep.anti_crit / 10.0f);
	//m_pTxt_DeCritical->SetText(szText);
	//
	//a_sprintf(szText, _AL("%.1f%%"), rep.anti_crit_damage * 100 + 0.001);
	//m_pTxt_DeCriticalDamage->SetText(szText);
	//
	//a_sprintf(szText, _AL("%.1f%%"), rep.cult_defense[0] / 10.0f);
	//m_pTxt_DeDamageTaoism->SetText(szText);
	//a_sprintf(szText, _AL("%.1f%%"), rep.cult_defense[1] / 10.0f);
	//m_pTxt_DeDamageEvil->SetText(szText);
	//a_sprintf(szText, _AL("%.1f%%"), rep.cult_defense[2] / 10.0f);
	//m_pTxt_DeDamageBuddhism->SetText(szText);
	//
	//int nLevel = rbp.iLevel;
	//
	//a_sprintf(szText, _AL("%d"), rep.defense);
	//m_pTxt_Defense->SetText(szText);
	//strText.Format(_AL("%d"), rep.resistance[0]);
	//m_pTxt_StunRes->SetText(strText);
	//strText.Format(_AL("%d"), rep.resistance[3]);
	//m_pTxt_SilenceRes->SetText(strText);
	//strText.Format(_AL("%d"), rep.resistance[1]);
	//m_pTxt_WeaknessRes->SetText(strText);
	//strText.Format(_AL("%d"), rep.resistance[4]);
	//m_pTxt_SleepRes->SetText(strText);
	//strText.Format(_AL("%d"), rep.resistance[2]);
	//m_pTxt_EnlaceRes->SetText(strText);

	//strText.Format(_AL("%d"), rep.resistance_tenaciy[0]);
	//m_pTxt_StunRes02->SetText(strText);
	//strText.Format(_AL("%d"), rep.resistance_tenaciy[3]);
	//m_pTxt_SilenceRes02->SetText(strText);
	//strText.Format(_AL("%d"), rep.resistance_tenaciy[1]);
	//m_pTxt_WeaknessRes02->SetText(strText);
	//strText.Format(_AL("%d"), rep.resistance_tenaciy[4]);
	//m_pTxt_SleepRes02->SetText(strText);
	//strText.Format(_AL("%d"), rep.resistance_tenaciy[2]);
	//m_pTxt_EnlaceRes02->SetText(strText);

	//strText.Format(_AL("%d"), rep.resistance_proficiency[0]);
	//m_pTxt_StunRes03->SetText(strText);
	//strText.Format(_AL("%d"), rep.resistance_proficiency[3]);
	//m_pTxt_SilenceRes03->SetText(strText);
	//strText.Format(_AL("%d"), rep.resistance_proficiency[1]);
	//m_pTxt_WeaknessRes03->SetText(strText);
	//strText.Format(_AL("%d"), rep.resistance_proficiency[4]);
	//m_pTxt_SleepRes03->SetText(strText);
	//strText.Format(_AL("%d"), rep.resistance_proficiency[2]);
	//m_pTxt_EnlaceRes03->SetText(strText);

	//ACString szTemp;
	//ACString szHint;
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18412), rep.resistance[0]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18428), rep.resistance_tenaciy[0]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18327), (1.0 - 100.0 / (100 + rep.resistance_tenaciy[0])) * 100);
	//szHint += szTemp;
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18423), rep.resistance_proficiency[0]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18332), rep.resistance_proficiency[0]);
	//szHint += szTemp;
	////GetDlgItem("Img_StunRes")->SetHint(szHint);
	////GetDlgItem("Txt_StunRes")->SetHint(szHint);
	//GetDlgItem("Lab_StunRes")->SetHint(szHint);
	////GetDlgItem("Txt_StunRes02")->SetHint(szHint);
	//GetDlgItem("Lab_StunRes02")->SetHint(szHint);
	////GetDlgItem("Txt_StunRes03")->SetHint(szHint);
	//GetDlgItem("Lab_StunRes03")->SetHint(szHint);

	//szHint = _AL("");
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18413), rep.resistance[3]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18429), rep.resistance_tenaciy[3]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18329), (1.0 - 100.0 / (100 + rep.resistance_tenaciy[3])) * 100);
	//szHint += szTemp;
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18424), rep.resistance_proficiency[3]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18334), rep.resistance_proficiency[3]);
	//szHint += szTemp;
	////GetDlgItem("Img_SilenceRes")->SetHint(szHint);
	////GetDlgItem("Txt_SilenceRes")->SetHint(szHint);
	//GetDlgItem("Lab_SilenceRes")->SetHint(szHint);
	////GetDlgItem("Txt_SilenceRes02")->SetHint(szHint);
	//GetDlgItem("Lab_SilenceRes02")->SetHint(szHint);
	////GetDlgItem("Txt_SilenceRes03")->SetHint(szHint);
	//GetDlgItem("Lab_SilenceRes03")->SetHint(szHint);

	//szHint = _AL("");
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18415), rep.resistance[1]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18431), rep.resistance_tenaciy[1]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18328), (1.0 - 100.0 / (100 + rep.resistance_tenaciy[1])) * 100);
	//szHint += szTemp;
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18426), rep.resistance_proficiency[1]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18333), rep.resistance_proficiency[1]);
	//szHint += szTemp;
	////GetDlgItem("Img_WeaknessRes")->SetHint(szHint);
	////GetDlgItem("Txt_WeaknessRes")->SetHint(szHint);
	//GetDlgItem("Lab_WeaknessRes")->SetHint(szHint);
	////GetDlgItem("Txt_WeaknessRes02")->SetHint(szHint);
	//GetDlgItem("Lab_WeaknessRes02")->SetHint(szHint);
	////GetDlgItem("Txt_WeaknessRes03")->SetHint(szHint);
	//GetDlgItem("Lab_WeaknessRes03")->SetHint(szHint);

	//szHint = _AL("");
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18414), rep.resistance[4]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18430), rep.resistance_tenaciy[4]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18330), (1.0 - 100.0 / (100 + rep.resistance_tenaciy[4])) * 100);
	//szHint += szTemp;
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18425), rep.resistance_proficiency[4]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18335), rep.resistance_proficiency[4]);
	//szHint += szTemp;
	////GetDlgItem("Img_SleepRes")->SetHint(szHint);
	////GetDlgItem("Txt_SleepRes")->SetHint(szHint);
	//GetDlgItem("Lab_SleepRes")->SetHint(szHint);
	////GetDlgItem("Txt_SleepRes02")->SetHint(szHint);
	//GetDlgItem("Lab_SleepRes02")->SetHint(szHint);
	////GetDlgItem("Txt_SleepRes03")->SetHint(szHint);
	//GetDlgItem("Lab_SleepRes03")->SetHint(szHint);

	//szHint = _AL("");
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18416), rep.resistance[2]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18432), rep.resistance_tenaciy[2]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18331), (1.0 - 100.0 / (100 + rep.resistance_tenaciy[2])) * 100);
	//szHint += szTemp;
	//szTemp.Format(GetStringFromTable(18326), GetStringFromTable(18427), rep.resistance_proficiency[2]);
	//szHint += szTemp;
	//szHint += _AL("\r");
	//szTemp.Format(GetStringFromTable(18336), rep.resistance_proficiency[2]);
	//szHint += szTemp;
	////GetDlgItem("Img_EnlaceRes")->SetHint(szHint);
	////GetDlgItem("Txt_EnlaceRes")->SetHint(szHint);
	//GetDlgItem("Lab_EnlaceRes")->SetHint(szHint);
	//GetDlgItem("Lab_EnlaceRes")->SetTransparent(false);
	////GetDlgItem("Txt_EnlaceRes02")->SetHint(szHint);
	//GetDlgItem("Lab_EnlaceRes02")->SetHint(szHint);
	//GetDlgItem("Lab_EnlaceRes02")->SetTransparent(false);
	////GetDlgItem("Txt_EnlaceRes03")->SetHint(szHint);
	//GetDlgItem("Lab_EnlaceRes03")->SetHint(szHint);
	//GetDlgItem("Lab_EnlaceRes03")->SetTransparent(false);
}

void CDlgCharacter::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;

	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	CDlgRuneMain *pDlgRuneMain = (CDlgRuneMain*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying");
	if (pItem && pItem->GetClassID()==CECIvtrItem::ICID_RUNE && pDlgRuneMain->IsInService())
	{
		return;
	}

	if( AUI_PRESS(VK_CONTROL) || AUI_PRESS(VK_SHIFT) )
	{
		if( pItem->VerifyInfoMD5() )
		{
			CDlgChat* pChat = GetGameUIMan()->m_pDlgChat;
			PAUIEDITBOX pEdit = (PAUIEDITBOX)pChat->GetDlgItem("DEFAULT_Txt_Speech");
			ACString strName;
			A3DCOLOR color;
			CDlgChat::TransformItemColorName(pItem, strName, color);
			WORD wSlot = (WORD)atoi(pObj->GetName() + strlen("Equip_"));
			if( pEdit->GetItemCount(enumEIIvtrlItem) < 1 && pEdit->GetItemCount(enumEITask) < 1 )
			{
				if (pEdit->AppendItem(enumEIIvtrlItem, color, strName, L""))
					pEdit->SetIvtrItemLocInfo(IVTRTYPE_EQUIPPACK, wSlot);
			}
			GetGameUIMan()->BringWindowToTop(pChat);
			pChat->ChangeFocus(pEdit);
		}
	}
	else if( !pItem->IsFrozen() )
	{
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();

		GetGameUIMan()->m_ptLButtonDown.x = GET_X_LPARAM(lParam) - p->X;
		GetGameUIMan()->m_ptLButtonDown.y = GET_Y_LPARAM(lParam) - p->Y;
		GetGameUIMan()->InvokeDragDrop(this, pObj, GetGameUIMan()->m_ptLButtonDown);
		GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
	}
}

void CDlgCharacter::OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") || pObj!=m_pImgEquip[EQUIPIVTR_RUNE] )
		return;

	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Equip_Yuanying");
	pDlg->SetData(EQUIPIVTR_RUNE, "rune_index");
	pDlg->SetDataPtr(pItem, "CECIvtrRune_Ptr");
	pDlg->Show(true);
}

void CDlgCharacter::OnEventLButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") || GetGameUIMan()->m_nCursorState != CECGameUIMan::CURSOR_STATE_NORMAL)
		return;
	if( AUI_PRESS(VK_CONTROL) )
		return;

	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");

	int iSrc = -1, iDst = -1;
	CECInventory *pInventory;
	iSrc = atoi(pObj->GetName() + strlen("Equip_"));
	
	CDlgRuneMain *pDlgRuneMain = (CDlgRuneMain*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying");
	if (iSrc==23 && pDlgRuneMain->IsInService())
	{
		return;
	}

	if (pItem->GetClassID() == CECIvtrItem::ICID_EQUIP	&&
		((CECIvtrEquip *)pItem)->GetDBEssence()->equip_mask & (EQUIP_MASK_FASHION_EYE  |
																EQUIP_MASK_FASHION_HEAD|
																EQUIP_MASK_FASHION_BODY|
																EQUIP_MASK_FASHION_FOOT))
	{
		pInventory = GetHostPlayer()->GetFashionPack();
		iDst = pInventory->SearchEmpty();
		if ( -1!=iDst )
		{
			GetGameSession()->c2s_CmdExgEquFashionItem(iSrc, iDst);
			return;
		}

	}
	else if ( pItem->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART &&
		((CECIvtrTalismanMain*)pItem)->GetDBEssence()->is_aircraft )
	{		
		pInventory = GetHostPlayer()->GetPack(IVTRTYPE_MOUNTWING);
		iDst = pInventory->SearchEmpty();
		if ( -1!=iDst )
		{
			GetGameSession()->c2s_CmdExgEquMountWingItem(iSrc, iDst);
			return;
		}
	}

	pInventory = GetHostPlayer()->GetPack();
	iDst = pInventory->SearchEmpty();
	if ( -1!=iDst )
		GetGameSession()->c2s_CmdEquipItem(iDst, iSrc);
}

ACString CDlgCharacter::GetTitleHint(int idTitle, bool bOnlyDesc)
{
	ACString strDesc = _AL("");
	if( !bOnlyDesc )
	{
		strDesc += GetTitle(idTitle, NULL);
		CECGame::TitlePropTable::iterator iter = GetGame()->GetTitlePropTable().find(idTitle);
		if( iter != GetGame()->GetTitlePropTable().end() )
		{
			elementdataman *pDataMan = GetGame()->GetElementDataMan();
			DATA_TYPE dt;
			TITLE_PROP_CONFIG *pTitle = (TITLE_PROP_CONFIG *)pDataMan->get_data_ptr(
				(*iter.value()), ID_SPACE_CONFIG, dt);
			if( dt == DT_TITLE_PROP_CONFIG && pTitle )
			{
				strDesc += _AL("^8080ff");
				for(int i = 0; i < 3; i++)
					if( pTitle->id_addons[i] )
					{
						EQUIPMENT_ADDON *pEquipAddon = (EQUIPMENT_ADDON *)pDataMan->get_data_ptr(
							pTitle->id_addons[i], ID_SPACE_ADDON, dt);
						if( dt == DT_EQUIPMENT_ADDON && pEquipAddon )
						{
							strDesc += _AL("\r");
							strDesc += CECIvtrEquip::GetEquipAddonDesc(pEquipAddon->id, pEquipAddon->type, pEquipAddon->param1, pEquipAddon->param2, pEquipAddon->param3);
						}
					}
			}
		}
	}

	CConfigFromLua * pConfigFromLua = GetGame()->GetConfigFromLua();
	if( pConfigFromLua )
	{
		CConfigFromLua::TitleDescTab::iterator it = pConfigFromLua->m_TitleDesc.find(idTitle);
		if( it != pConfigFromLua->m_TitleDesc.end() )
		{
			if( !bOnlyDesc )
				strDesc += _AL("\r");
			AUICTranslate trans;
			strDesc += trans.Translate(*it.value());
		}
	}
	return strDesc;
}

ACString CDlgCharacter::GetTitle(int idTitle, A3DCOLOR *pColor, int *ptype /*NULL*/)
{
	ACString strTitle = _AL("......");
	// see if is a special title
	if( idTitle < 1000 )
	{
		if( idTitle == 1 ) // 威望固定称谓
		{
			int level = GetHostPlayer()->GetReputationLevel(GetHostPlayer()->GetReputation());
			strTitle = GetStringFromTable(level + 1100);
		}
	}
	else // 普通称谓，从策划的表里取名字
	{
		CConfigFromLua * pConfigFromLua = GetGame()->GetConfigFromLua();
		if( pConfigFromLua )
		{
			CConfigFromLua::TitleDefTab::iterator it = pConfigFromLua->m_TitleDef.find(idTitle);
			if( it != pConfigFromLua->m_TitleDef.end() )
			{
				strTitle = *it.value();
				GetHostPlayer()->FormatTitle(strTitle);
			}
		}
	}
	if( pColor && strTitle.GetLength() > 0 && strTitle[0] == '^' )
	{
		A3DCOLOR color = 0;
		for(int j = 0; j < 6; j++ )
		{
			int nValue = strTitle[j + 1];
			if( nValue >= '0' && nValue <= '9' )
				nValue -= '0';
			else if( nValue >= 'a' && nValue <= 'z' )
				nValue = nValue - 'a' + 0xA;
			else if( nValue >= 'A' && nValue <= 'Z' )
				nValue = nValue - 'A' + 0xA;
			else
				nValue = 0;
			*pColor = *pColor * 0x10 + nValue;
		}
		*pColor |= 0xFF000000;
		strTitle.CutLeft(7);
	}
	else if( pColor )
		*pColor = A3DCOLORRGB(255, 255, 255);
	
	if(strTitle.Find(_AL("#image")) >=0 || strTitle.Find(_AL("&image")) >=0 || strTitle.Find(_AL("#anim")) >=0 || strTitle.Find(_AL("&anim")) >=0)
	{
		int i = 0;
		while(strTitle[i] != _AL('#') && strTitle[i] != _AL('&')) i++;
		strTitle = strTitle.Left(i);
		if(ptype)
			*ptype = 1;
	}

	return strTitle;
}

void CDlgCharacter::ReLoadModel()
{
	A3DRELEASE(m_pPlayer);
	m_pPlayer = new CECLoginPlayer(g_pGame->GetGameRun()->GetWorld()->GetPlayerMan());
	const GNET::RoleInfo& info = g_pGame->GetGameRun()->GetSelectedRoleInfo();
	if (!m_pPlayer->Load(CECPlayer::GetPhysiqueByProf(info.occupation),info.occupation, info.faceid, info.hairid, info.gender, NULL))
	{
		m_pPlayer->Release();
		delete m_pPlayer;
		m_pPlayer = 0;
		return;
	}
	m_pPlayer->SetPos(A3DVECTOR3(0));
	m_pPlayer->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
	m_pPlayer->GetPlayerModel()->SetAutoUpdateFlag(false);
//	memset(m_aEquips, 0, sizeof(m_aEquips));
}

int CDlgCharacter::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="set_flash")
	{
		PAUIOBJECT pObj = GetGameUIMan()->GetDialog("Win_Character")->GetDlgItem("Rdo_5");
		pObj->SetFlash(true);
		pObj = GetGameUIMan()->GetDialog("Win_Credit")->GetDlgItem("Rdo_5");
		pObj->SetFlash(true);
		pObj = GetGameUIMan()->GetDialog("Win_Char_TitleBox")->GetDlgItem("Rdo_5");
		pObj->SetFlash(true);
		pObj = GetGameUIMan()->GetDialog("Win_CharInfo")->GetDlgItem("Rdo_5");
		pObj->SetFlash(true);
		pObj = GetGameUIMan()->GetDialog("Win_Fengshen_Soul")->GetDlgItem("Rdo_5");
		pObj->SetFlash(true);
	}
	else if (strMsg=="stop_flash")
	{
		PAUIOBJECT pObj = GetGameUIMan()->GetDialog("Win_Character")->GetDlgItem("Rdo_5");
		pObj->SetFlash(false);
		pObj = GetGameUIMan()->GetDialog("Win_Credit")->GetDlgItem("Rdo_5");
		pObj->SetFlash(false);
		pObj = GetGameUIMan()->GetDialog("Win_Char_TitleBox")->GetDlgItem("Rdo_5");
		pObj->SetFlash(false);
		pObj = GetGameUIMan()->GetDialog("Win_CharInfo")->GetDlgItem("Rdo_5");
		pObj->SetFlash(false);
		pObj = GetGameUIMan()->GetDialog("Win_Fengshen_Soul")->GetDlgItem("Rdo_5");
		pObj->SetFlash(false);
	}
	return 1;
}

// 飞升150后，第一次打开时需要显示符文泡泡提示
void CDlgCharacter::ShowRuneTipPop()
{
	CECHostPlayer* pHostPlayer = GetHostPlayer();
	bool needShow = !g_pGame->GetConfigs()->GetLocalSettings().bFuWenPop1Showed;
	needShow = needShow && (pHostPlayer->GetBasicProps().iLevel >= 150 && pHostPlayer->GetRebornCount() >= 1);
	GetDlgItem("Img_Arrow01")->Show(needShow);
	GetDlgItem("Txt_PopHint01")->Show(needShow);
	GetDlgItem("Shape_PopHint01")->Show(needShow);
}