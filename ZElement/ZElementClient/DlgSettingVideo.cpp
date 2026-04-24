/********************************************************************
	created:	2005/08/11
	created:	11:8:2005   17:49
	file name:	DlgSettingVideo.cpp
	author:		yaojun
	
	purpose:	
*********************************************************************/

#include "DlgSettingVideo.h"
#include "DlgHost.h"
#include "aui/AUICheckBox.h"
#include "aui/AUIStillImageButton.h"
#include "aui/AUISlider.h"

#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_Global.h"
#include "EC_HostInputFilter.h"
#include "EC_InputCtrl.h"
#include "EC_Secretary.h"
#include "EC_HostPlayer.h"
#include "EC_SendC2SCmds.h"
#include "LuaEvent.h"


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSettingVideo, CDlgSetting)

AUI_ON_COMMAND("left",	OnCommandBossKeyLeft)
AUI_ON_COMMAND("right",	OnCommandBossKeyRight)
AUI_ON_COMMAND("bosskey",OnCommandBossKeyEnable)
AUI_ON_COMMAND("Chk_All", OnCommandCheckAll)
AUI_ON_COMMAND("Chk_None", OnCommandCheckNone)

AUI_END_COMMAND_MAP()


CDlgSettingVideo::CDlgSettingVideo()
{
	m_pChkBossKeyEnable	= NULL;
	m_pTxtBossKey		= NULL;
	m_pBtnBossKeyLeft	= NULL;
	m_pBtnBossKeyRight	= NULL;

	memset(&m_VideoSetting, 0, sizeof(m_VideoSetting));
}

CDlgSettingVideo::~CDlgSettingVideo()
{

}

void CDlgSettingVideo::DoDataExchange(bool bSave)
{
	CDlgSetting::DoDataExchange(bSave);

	DDX_CheckBox(bSave, "Chk_ReverseMouse", m_GameSetting.bReverseWheel);
	DDX_CheckBox(bSave, "Chk_SwitchAD", m_GameSetting.bTurnaround);
	DDX_Control("Chk_BossKey", m_pChkBossKeyEnable);
	DDX_Control("Txt_BossKey", m_pTxtBossKey);
	m_pTxtBossKey->SetAlign(AUIFRAME_ALIGN_CENTER);
	DDX_Control("Btn_LeftTurn", m_pBtnBossKeyLeft);
	DDX_Control("Btn_RightTurn", m_pBtnBossKeyRight);
	DDX_CheckBox(bSave, "Chk_OtherPlayer", m_VideoSetting.bPlayerHeadText);
	DDX_CheckBox(bSave, "Chk_Name", m_VideoSetting.bPlayerName);
	DDX_CheckBox(bSave, "Chk_Title", m_VideoSetting.bPlayerTitle);
	DDX_CheckBox(bSave, "Chk_Faction", m_VideoSetting.bPlayerFaction);
	DDX_CheckBox(bSave, "Chk_Class", m_VideoSetting.bPlayerCircle);
	DDX_CheckBox(bSave, "Chk_Family", m_VideoSetting.bPlayerFamily);
	DDX_CheckBox(bSave, "Chk_Player", m_VideoSetting.bPlayerShop);
	DDX_CheckBox(bSave, "Chk_Talk", m_VideoSetting.bPlayerTalk);
	DDX_CheckBox(bSave, "Chk_Talkbubble", m_VideoSetting.bPlayerBubble);
	DDX_CheckBox(bSave, "Chk_Monster", m_VideoSetting.bMonsterName);
	DDX_CheckBox(bSave, "Chk_NPC", m_VideoSetting.bNPCName);
	DDX_CheckBox(bSave, "Chk_SelfName", m_VideoSetting.bPlayerSelfName);
	DDX_CheckBox(bSave, "Chk_Item", m_VideoSetting.bGoodsName);
	DDX_CheckBox(bSave, "Chk_Personize", m_VideoSetting.bShowCustomize);
	DDX_CheckBox(bSave, "Chk_MapNPC", m_VideoSetting.bMapNPC);
	DDX_CheckBox(bSave, "Chk_MapMonster", m_VideoSetting.bMapMonster);
	DDX_CheckBox(bSave, "Chk_MapPlayer", m_VideoSetting.bMapPlayer);
	DDX_CheckBox(bSave, "Chk_EquipCompare", m_GameSetting.bEquipCompare);
	DDX_CheckBox(bSave, "Chk_Secretary", m_GameSetting.bHideSecretary);
	DDX_CheckBox(bSave, "Chk_Hp", m_VideoSetting.bPlayerHP);
	DDX_CheckBox(bSave, "Chk_SameServer", m_VideoSetting.bPlayerServer);
	DDX_Slider(bSave, "Sld_Playernamesize", m_VideoSetting.cNameSize);
	
	if( bSave )
	{
		m_VideoSetting.bPlayerMP = ((PAUICHECKBOX)GetDlgItem("Chk_Mp"))->IsChecked();
		m_VideoSetting.bPlayerDP = ((PAUICHECKBOX)GetDlgItem("Chk_Dp"))->IsChecked();
		m_VideoSetting.bPlayerEnergy = ((PAUICHECKBOX)GetDlgItem("Chk_EquipAttach"))->IsChecked();
		bool bCheck;
		DDX_CheckBox(bSave, "Chk_Secretary", bCheck);
		GetGameRun()->GetSecretary()->SetHelpGuideEnable(!bCheck);
		
		DDX_CheckBox(bSave, "Chk_Lottery5_PopupY", bCheck);

		EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
		setting.bStopPromptConsumeCash = !bCheck;
		GetGame()->GetConfigs()->SetLocalSettings(setting);
		GetGame()->GetConfigs()->SaveLocalSetting();

		m_cWindowScale = ((AUISlider*)GetDlgItem("Scl_Ratio"))->GetLevel();
	}
	else
	{
		PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_Vip");
		CECHostPlayer *pHost = GetHostPlayer();
		pChk->Enable(pHost&&(pHost->GetVipLv()==6));
		pChk->Check(pHost&&(pHost->IsHideVip()==0));
		//c2s_SendCmdHideVipLevel(pChk->IsChecked()?1:0);
		((PAUICHECKBOX)GetDlgItem("Chk_Mp"))->Check(m_VideoSetting.bPlayerMP);
		((PAUICHECKBOX)GetDlgItem("Chk_Dp"))->Check(m_VideoSetting.bPlayerDP);
		((PAUICHECKBOX)GetDlgItem("Chk_EquipAttach"))->Check(m_VideoSetting.bPlayerEnergy);
		((PAUICHECKBOX)GetDlgItem("Chk_Lottery5_PopupY"))->Check(!GetGame()->GetConfigs()->GetLocalSettings().bStopPromptConsumeCash);
		m_pChkBossKeyEnable->Check((m_VideoSetting.cBossKey&0x80)>0);
		m_iBossKey = m_VideoSetting.cBossKey & 0x7f;
		UpdateBossKey();// boss key只需要更新显示，保存会在相应按钮的同时处理
		m_cWindowScale = GetGameUIMan()->GetUIWindowScale();
		((AUISlider*)GetDlgItem("Scl_Ratio"))->SetLevel(m_cWindowScale);
	}
}

bool CDlgSettingVideo::OnInitDialog()
{
	if (!CDlgSetting::OnInitDialog())
		return false;

	// init diaplay
	UpdateView();

	//Is there a better way ?
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_Player"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_OtherPlayer"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_Name"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_Faction"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_Title"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_Monster"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_NPC"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_Item"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_Family"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_SelfName"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_Talk"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_Talkbubble"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_Personize"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_Hp"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_Mp"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_Class"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_SameServer"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_Dp"));
	m_pVideoCheckList.push_back((AUICheckBox*)GetDlgItem("Chk_EquipAttach"));

	m_pCheckAll = (AUICheckBox*)GetDlgItem("Chk_All");
	m_pCheckNone = (AUICheckBox*)GetDlgItem("Chk_None");
	return true;
}

void CDlgSettingVideo::OnShowDialog()
{
	CDlgSetting::OnShowDialog();

	// init control
	CheckRadioButton(1,3);

	// init diaplay
	UpdateView();
	
}

void CDlgSettingVideo::SetToDefault()
{
	GetGame()->GetConfigs()->DefaultUserSettings(NULL, &m_VideoSetting, NULL);
	
	UpdateData(false);
}

void CDlgSettingVideo::Apply()
{
	UpdateData(true);
	
	// save setting
	GetGame()->GetConfigs()->SetVideoSettings(m_VideoSetting);
	GetGame()->GetConfigs()->SetGameSettings(m_GameSetting);
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_Vip");
	c2s_SendCmdHideVipLevel(pChk->IsChecked()?0:1);

	if(m_cWindowScale != GetGameUIMan()->GetUIWindowScale())
		GetGameUIMan()->SetUIWindowScale(m_cWindowScale);
	
	// todo : the following lines should be rewritten
//	GetGameUIMan()->m_pDlgHost->m_pChk_Player->Check(m_VideoSetting.bPlayerHeadText);
//	GetGameUIMan()->m_pDlgHost->m_pChk_MapMonster->Check(m_VideoSetting.bMapMonster);
//	GetGameUIMan()->m_pDlgHost->m_pChk_Surround->Check(m_VideoSetting.bModelLimit);
//	GetGameUIMan()->m_pDlgHost->m_pChk_Sight->Check(false);
}

void CDlgSettingVideo::UpdateView()
{
	// read setting
	m_VideoSetting = GetGame()->GetConfigs()->GetVideoSettings();
	m_GameSetting = GetGame()->GetConfigs()->GetGameSettings();
	
	// show setting
	UpdateData(false);
}

void CDlgSettingVideo::OnCommandBossKeyLeft(const char *szCommand)
{
	m_iBossKey = BOSSKEY_ALTWAVE;
	UpdateBossKey();
}

void CDlgSettingVideo::OnCommandBossKeyRight(const char *szCommand)
{
	m_iBossKey = BOSSKEY_NUMMULT;
	UpdateBossKey();
}

void CDlgSettingVideo::OnCommandBossKeyEnable(const char *szCommand)
{
	UpdateBossKey();
}

void CDlgSettingVideo::UpdateBossKey()
{
	char cBossKey = m_VideoSetting.cBossKey;
	if (m_pChkBossKeyEnable->IsChecked())
	{
		cBossKey = (char)0x80;
		if (m_iBossKey == BOSSKEY_ALTWAVE)
		{
			m_pTxtBossKey->SetText(_AL("alt+~"));
			m_pBtnBossKeyLeft->Enable(false);
			m_pBtnBossKeyRight->Enable(true);
			g_pGame->GetGameRun()->GetHostInputFilter()->RegisterBossKeys(VK_OEM_3, EC_KSF_ALT);
		}
		else
		{
			m_pTxtBossKey->SetText(GetStringFromTable(8470));
			m_pBtnBossKeyLeft->Enable(true);
			m_pBtnBossKeyRight->Enable(false);
			g_pGame->GetGameRun()->GetHostInputFilter()->RegisterBossKeys(VK_MULTIPLY, 0);
			cBossKey |= 0x1;
		}
		if(GetGameRun()->GetSecretary()->CheckEventState(CECSecretary::FIRST_BOSSKEY))
			LuaEvent::FirstBossKey();
	}
	else
	{
		g_pGame->GetGameRun()->GetHostInputFilter()->RegisterBossKeys(0, 0);
		m_pTxtBossKey->SetText(GetStringFromTable(780));
		m_pBtnBossKeyLeft->Enable(false);
		m_pBtnBossKeyRight->Enable(false);
		cBossKey &= 0x7f;
	}
	
	m_VideoSetting.cBossKey = cBossKey;
}

void CDlgSettingVideo::CheckAll(bool isCheck)
{
	abase::vector<AUICheckBox*>::iterator it = m_pVideoCheckList.begin();
	while(it != m_pVideoCheckList.end())
	{
		(*it)->Check(isCheck);
		it++;
	}
}

void CDlgSettingVideo::CheckNone(bool isCheck)
{
	CheckAll(!isCheck);
}

bool CDlgSettingVideo::isCheckAll(bool isCheck)
{
	abase::vector<AUICheckBox*>::iterator it = m_pVideoCheckList.begin();
	while(it != m_pVideoCheckList.end())
	{
		if((*it)->IsChecked() != isCheck)
		{
			return false;
		}
		it++;
	}
	return true;
}

bool CDlgSettingVideo::isCheckNone()
{
	return isCheckAll(false);
}

void CDlgSettingVideo::OnCommandCheckAll(const char* szCommand)
{
	if(m_pCheckAll->IsChecked())
	{
		CheckAll();
		m_pCheckNone->Check(false);
	}
	else
	{
		m_pCheckAll->Check(true);
	}
}

void CDlgSettingVideo::OnCommandCheckNone(const char* szCommand)
{
	if(m_pCheckNone->IsChecked())
	{
		CheckNone();
		m_pCheckAll->Check(false);
	}
	else
	{
		m_pCheckNone->Check(true);
	}
}

void CDlgSettingVideo::OnTick()
{
	if(isCheckAll())
	{
		m_pCheckAll->Check(true);
		m_pCheckNone->Check(false);
	}
	else if(isCheckNone())
	{
		m_pCheckAll->Check(false);
		m_pCheckNone->Check(true);
	}
	else 
	{
		if(m_pCheckAll->IsChecked())
			m_pCheckAll->Check(false);
		if(m_pCheckNone->IsChecked())
			m_pCheckNone->Check(false);
	}
}