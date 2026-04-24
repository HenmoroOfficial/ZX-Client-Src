// Filename	: DlgAchievement.cpp
// Creator	: Fu Chonggang
// Date		: 2009/7/9


#include "AFI.h"
#include "AIniFile.h"
#include "AWIniFile.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\A3DFTFont.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIClockIcon.h"
#include "AUI\\AUILabel.h"
#include "AFileImage.h"
#include "DlgCommonDlg.h"
#include "DlgAchievement.h"
#include "DlgPetDetail.h"
#include "DlgKf2012Guess.h"
#include "DlgMiniMap.h"
#include "DlgBroadCastInGame.h"
#include "DlgAchivMain.h"
#include "DlgChat.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Skill.h"
#include "EC_GameSession.h"
#include "EC_IvtrConsume.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"
#include "EC_Inventory.h"
#include "EC_Configs.h"
#include "EC_HelpGraphGuide.h"
#include "EC_Achievement.h"
#include "gt_overlay.h"

#include "teamraidmappingsuccess_re.hpp"

#include "DlgCrssvrTeams.h"
#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgCommonDlg, CDlgBase)

AUI_ON_COMMAND("Btn_Detail",OnCommandDetail)
AUI_ON_COMMAND("confirm",	OnCommandConfirm)
AUI_ON_COMMAND("Btn_Max",	OnCommandGuess)
AUI_ON_COMMAND("Btn_Radio",	OnCommandRadio)
AUI_ON_COMMAND("IDCANCEL",	OnCommandCancel)

AUI_END_COMMAND_MAP()

CDlgCommonDlg::CDlgCommonDlg()
{
}

CDlgCommonDlg::~CDlgCommonDlg()
{
}

bool CDlgCommonDlg::OnInitDialog()
{

	return true;
}

void CDlgCommonDlg::OnShowDialog()
{
}

void CDlgCommonDlg::OnCommandDetail(const char* szCommand)
{
	if (m_szName=="Win_Achievement_Desc")
	{
		DWORD id = GetData("achivid");
		sAchievement *achiv = GetHostPlayer()->GetAchievementMan()->GetAchievement(id);
		CDlgAchivMain* pDlgAhiv = (CDlgAchivMain* )GetGameUIMan()->GetDialog("Win_Achievement");
		pDlgAhiv->ShowAchiv(achiv);
		pDlgAhiv->Show(true);

		return;
	}

	GetGameUIMan()->GetDialog("Win_PetDetail")->Show(true);
}

void CDlgCommonDlg::OnCommandConfirm(const char* szCommand)
{
	if( stricmp(m_szName, "Win_MessageSuccess") == 0 )
	{
		Show(false);
		GetGameUIMan()->GetDialog("Win_Success")->Show(true);
	}
}

void CDlgCommonDlg::OnCommandGuess(const char* szCommand)
{
	CDlgKf2012GuessBase *pDlg = (CDlgKf2012GuessBase*)GetGameUIMan()->GetDialog("Win_Kf2012Bet1");
	pDlg->ChangeShowStatus();
}

void CDlgCommonDlg::OnCommandRadio(const char* szCommand)
{
	ACString strText;
	AWIniFile iniFile;
	if(GetDlgItem("Gfx_1")) GetDlgItem("Gfx_1")->Show(false);
	if(iniFile.Open("configs\\broadcast.ini"))
	{
		strText = iniFile.GetValueAsString(_AL("Channel"), _AL("ChannelURL0"), _AL(""));
		iniFile.Close();
		if( strText.GetLength() > 0 )
		{
			CDlgBroadCastInGame* pDlgRadioInGame = (CDlgBroadCastInGame*)GetGameUIMan()->GetDialog("Win_Radio");
			pDlgRadioInGame->Show(true);
			pDlgRadioInGame->NavigateUrl(strText);
		}
	}
}

int CDlgCommonDlg::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_szName!="Win_ItemDesc" && m_szName!="Win_Achievement_Desc")
		return 0;

	if (m_szName == "Win_Achievement_Desc")
	{
		sAchievement* pAchievement = (sAchievement*)wParam;
		PAUIDIALOG pDlgAlign = (PAUIDIALOG)lParam;

		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem("Img_Achievement");
		PAUISTILLIMAGEBUTTON pDetail= (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Detail");
		PAUISTILLIMAGEBUTTON pClose= (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Close");
		PAUILABEL pDesc = (PAUILABEL)GetDlgItem("Txt_Desc");

		AUIOBJECT_SETPROPERTY p;
		sprintf(p.fn, "Ingame\\achivicon\\%d.dds", pAchievement->id);
		pImage->SetProperty("Image File", &p);
		

		ACString achivName = pAchievement->name;
		A3DCOLOR color;
		CDlgChat::TransformAchievementColorName(pAchievement, achivName, color);

		ACString timeStr;
		long t = pAchievement->iFinishTime;
		if (t>0)
		{
			t -= g_pGame->GetTimeZoneBias() * 60;
			tm *gtime = gmtime((const time_t*)&t);
			timeStr.Format(GetGameUIMan()->GetStringFromTable(8000),gtime->tm_year+1900,gtime->tm_mon+1,gtime->tm_mday,gtime->tm_hour,gtime->tm_min);
		}
		else if (t==0)
		{
			timeStr = GetGameUIMan()->GetStringFromTable(20121);
		}
		else
		{
			timeStr = GetGameUIMan()->GetStringFromTable(20120);
		}

		ACString desc;
		//pAchievement->GetStrDesc(desc);
		desc.Format(_AL("%s(%s)\r^%06x%s\r%s"),pAchievement->strOwnerName, timeStr, color & 0xffffff, achivName, pAchievement->strDesc);
		if(!desc.IsEmpty())
		{
			AUICTranslate trans;
			ACString szDesc = trans.Translate(desc);
			CSplit sp(szDesc);
			CSPLIT_STRING_VECTOR vec = sp.Split(_AL("\t"));
			if (vec.size()==1)
			{
				pDesc->SetText(_AL(""));
			}
			else
				pDesc->SetText(vec[1]);
			int nWidth, nHeight, nLines;
			A3DFTFont *pFont = pDesc->GetFont();
			pFont->Resize(pDesc->GetFontHeight() * GetGameUIMan()->GetWindowScale());
			AUI_GetTextRect(pFont, vec[0], nWidth, nHeight, nLines, 3);
			SIZE s = GetFrame()->GetSizeLimit();
			int W = max(nWidth, (pImage->GetSize().cx+pDetail->GetSize().cx+pClose->GetSize().cx)) + s.cx + 6;
			int H = pImage->GetSize().cy + s.cy + nHeight + 2;
			pDesc->SetPos(9, pImage->GetPos(true).y + pImage->GetSize().cy + 3);
			pDesc->SetText(vec[0]);

			int W1 = 0;
			int H1 = 0;
			if (vec.size()>1)
			{
				pDesc->SetPos(12+nWidth, pImage->GetPos(true).y + pImage->GetSize().cy + 3);
				AUI_GetTextRect(pFont, vec[1], nWidth, nHeight, nLines, 3);
				W1 = nWidth;
				H1 = nHeight;
			}

			W += W1;
			H = max(H, H1);

			pClose->SetPos(W - pClose->GetSize().cx, 0);
			SetSize(W, H);
			if (pDlgAlign)
				AlignTo(pDlgAlign, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
				AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);

			SetData(pAchievement->id, "achivid");
			Show(true);
		}

		return 0;
	}

	CECIvtrItem* pItem = NULL;
	PAUIDIALOG pDlgAlign = (PAUIDIALOG)lParam;
	if (strMsg=="set_item")
	{
		pItem = (CECIvtrItem*)wParam;
	}
	else if (strMsg=="set_id")
	{
		pItem = CECIvtrItem::CreateItem(wParam, 0, 1);
	}
	
	PAUISTILLIMAGEBUTTON pDetail= (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Detail");
	pDetail->Show(false);
	CDlgPetDetail* pDlgPetDetail = (CDlgPetDetail*)GetGameUIMan()->GetDialog("Win_PetDetail");
	if( pItem )
	{
		if (pItem->GetClassID()==CECIvtrItem::ICID_PET_BEDGE)
		{
			pDlgPetDetail->SetInfo(-1, (CECPetBedge*)pItem->Clone());
			pDetail->Show(true);
		}
		if(pItem->GetClassID()==CECIvtrItem::ICID_INDIVIDUALITY_ITEM)
		{
			pItem->SetNeedUpdate(false);
		}
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
		PAUISTILLIMAGEBUTTON pClose= (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Close");
		PAUILABEL pDesc = (PAUILABEL)GetDlgItem("Txt_Desc");
		PAUILABEL pDesc1 = (PAUILABEL)GetDlgItem("Txt_Desc1");
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		pImage->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
			GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY, strFile));
		if( pItem->GetDesc() )
		{
			AUICTranslate trans;
			ACString szDesc = trans.Translate(pItem->GetDesc());
			CSplit sp(szDesc);
			CSPLIT_STRING_VECTOR vec = sp.Split(_AL("\t"));
			if (vec.size()==1)
			{
				pDesc1->SetText(_AL(""));
			}
			else
				pDesc1->SetText(vec[1]);
			int nWidth, nHeight, nLines;
			A3DFTFont *pFont = pDesc->GetFont();
			pFont->Resize(pDesc->GetFontHeight() * GetGameUIMan()->GetWindowScale());
			AUI_GetTextRect(pFont, vec[0], nWidth, nHeight, nLines, 3);
			SIZE s = GetFrame()->GetSizeLimit();
			int W = max(nWidth, (pImage->GetSize().cx+pDetail->GetSize().cx+pClose->GetSize().cx)) + s.cx + 6;
			int H = pImage->GetSize().cy + s.cy + nHeight + 2;
			pDesc->SetPos(9, pImage->GetPos(true).y + pImage->GetSize().cy + 3);
			pDesc->SetText(vec[0]);
			
			int W1 = 0;
			int H1 = 0;
			if (vec.size()>1)
			{
				pDesc1->SetPos(12+nWidth, pImage->GetPos(true).y + pImage->GetSize().cy + 3);
				AUI_GetTextRect(pFont, vec[1], nWidth, nHeight, nLines, 3);
				W1 = nWidth;
				H1 = nHeight;
			}
			
			W += W1;
			H = max(H, H1);
			
			pClose->SetPos(W - pClose->GetSize().cx, 0);
			SetSize(W, H);
			if (pDlgAlign)
				AlignTo(pDlgAlign, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
				AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
			Show(true);
		}
	}
	
	if (strMsg=="set_id")
		delete pItem;
	
	return 0;
}

void CDlgCommonDlg::OnCommandCancel(const char* szCommand)
{
	Show(false);
}

CDlgKrGM::CDlgKrGM()
{
}

CDlgKrGM::~CDlgKrGM()
{
}

void CDlgKrGM::OnShowDialog()
{
	m_dwTime = GetTickCount();
}

void CDlgKrGM::OnTick()
{
	if ((GetTickCount()-m_dwTime) > 5000)
	{
		Show(false);
	}
}


AUI_BEGIN_COMMAND_MAP(CDlgMessageBoxYesNoTime, CDlgBase)

AUI_ON_COMMAND("Btn_Decide",	OnCommandConfirm)
AUI_ON_COMMAND("Btn_Exit",		OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)
AUI_ON_COMMAND("Btn_Confirm02",		OnCommandConfirm2)

AUI_ON_COMMAND("IDCANCEL",	OnCommandCancel)

AUI_END_COMMAND_MAP()

CDlgMessageBoxYesNoTime::CDlgMessageBoxYesNoTime()
{
}

CDlgMessageBoxYesNoTime::~CDlgMessageBoxYesNoTime()
{
}

bool CDlgMessageBoxYesNoTime::OnInitDialog()
{
	m_pLabTime = (PAUILABEL)GetDlgItem("Txt_TimeLeft");
	return true;
}

void CDlgMessageBoxYesNoTime::OnShowDialog()
{
	m_dwTime = GetTickCount();
}

void CDlgMessageBoxYesNoTime::OnTick()
{
	if ((GetTickCount()-m_dwTime) > m_dwTotalTime*1000)
	{
		Show(false);
	}
	ACString strText;
	strText.Format(_AL("%d"), m_dwTotalTime-((GetTickCount()-m_dwTime)/1000)); 
	m_pLabTime->SetText(strText);
	GetDlgItem("Btn_Exit")->Show(true);
	GetDlgItem("Btn_Decide")->Show(true);
	GetDlgItem("Btn_Confirm02")->Show(false);
	int isIscross = (int)GetDataPtr();
	if(isIscross == 1)
	{
		GetDlgItem("Btn_Confirm02")->Show(true);
		GetDlgItem("Btn_Exit")->Show(false);
		GetDlgItem("Btn_Decide")->Show(false);
	}
}

int CDlgMessageBoxYesNoTime::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	PAUILABEL pLabMsg = (PAUILABEL)GetDlgItem("Txt_1");
	pLabMsg->SetText(AS2AC(strMsg));
	m_dwTotalTime = wParam;
	m_iType	= lParam;
	Show(true);
	return 0;
}
void CDlgMessageBoxYesNoTime::OnCommandConfirm2(const char* szCommand)
{
	GetGameRun()->SaveConfigsToServer();
	switch (m_iType)
	{
	case TYPE_RAID_6V6:
		int isIscross = (int)GetDataPtr();
		if(isIscross == 1)
		{
			DlgCrssvrTeamsApply *pDlgApply = (DlgCrssvrTeamsApply*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupList");
			pDlgApply->Show(true);
		}
		TeamRaidMappingSuccess_Re p;
		p.roleid = GetHostPlayer()->GetCharacterID();
		p.raidroom_id = GetData();
		p.agree = 1;
		p.iscross = (int)GetDataPtr();
		GetGameSession()->SendNetData(p);
		GetGameUIMan()->MessageBox(17101);
		break;
	}

	Show(false);
}

void CDlgMessageBoxYesNoTime::OnCommandConfirm(const char* szCommand)
{
	GetGameRun()->SaveConfigsToServer();
	switch (m_iType)
	{
	case TYPE_RAID_ENTER:
		GetGameSession()->raid_enter(GetData(), (int)GetDataPtr());
		break;
	case TYPE_RAID_VOTE:
		GetGameSession()->raid_votes(GetData(), 1);
		break;
	case TYPE_RAID_6V6:
		int isIscross = (int)GetDataPtr();
		if(isIscross == 1)
		{
			DlgCrssvrTeamsApply *pDlgApply = (DlgCrssvrTeamsApply*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupList");
			pDlgApply->Show(true);
		}
		TeamRaidMappingSuccess_Re p;
		p.roleid = GetHostPlayer()->GetCharacterID();
		p.raidroom_id = GetData();
		p.agree = 1;
		p.iscross = (int)GetDataPtr();
		GetGameSession()->SendNetData(p);
		GetGameUIMan()->MessageBox(17101);
		break;
	}
	
	Show(false);
}

void CDlgMessageBoxYesNoTime::OnCommandCancel(const char* szCommand)
{
	switch (m_iType)
	{
	case TYPE_RAID_ENTER:
		GetGameSession()->raid_quit(GetData());
		break;
	case TYPE_RAID_VOTE:
		GetGameSession()->raid_votes(GetData(), 0);
		break;
	case TYPE_RAID_6V6:
		TeamRaidMappingSuccess_Re p;
		p.roleid = GetHostPlayer()->GetCharacterID();
		p.raidroom_id = GetData();
		p.agree = 0;
		GetGameSession()->SendNetData(p);
		break;
	}
	Show(false);
}



AUI_BEGIN_COMMAND_MAP(CDlgMessageBoxYesNoCheck, CDlgBase)

AUI_ON_COMMAND("Btn_Decide",	OnCommandConfirm)
AUI_ON_COMMAND("Btn_Exit",		OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_ON_COMMAND("IDCANCEL",	OnCommandCancel)

AUI_END_COMMAND_MAP()

int CDlgMessageBoxYesNoCheck::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	// TODO: 添加通用逻辑，目前只有帮助系统用到
	m_iType	= lParam;
	Show(true, true);
	return 0;
}

void CDlgMessageBoxYesNoCheck::OnCommandConfirm(const char* szCommand)
{
	switch (m_iType)
	{
	case TYPE_DISABLE_HELP:
		{
			PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_Never");
			if (pChk->IsChecked())
			{
				GetGameRun()->GetHelpGraphGuide()->SetState(CECHelpGraphGuide::HELP2012_CLOSE);
			}
			GetGameRun()->GetHelpGraphGuide()->SkipCurStage();
		}
		break;
	}
	Show(false);
}

void CDlgMessageBoxYesNoCheck::OnCommandCancel(const char* szCommand)
{
	switch (m_iType)
	{
	case TYPE_DISABLE_HELP:
		GetGameRun()->GetHelpGraphGuide()->ResumeHelp();
		break;
	}
	Show(false);
}


AUI_BEGIN_COMMAND_MAP(CDlgMessageBoxYesNoCheckYuanBao, CDlgBase)

AUI_ON_COMMAND("Btn_Confirm",		OnCommandConfirm)
AUI_ON_COMMAND("Chk_NPop",			OnCommandCheck)
AUI_ON_COMMAND("Btn_Cancel",		OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()

void CDlgMessageBoxYesNoCheckYuanBao::OnCommandConfirm(const char* szCommand)
{
	GetGameUIMan()->CommonMessage("Win_Lottery5", "confirm", 0, 0);
	Show(false);
}

void CDlgMessageBoxYesNoCheckYuanBao::OnCommandCheck(const char* szCommand)
{
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_NPop");
	EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
	setting.bStopPromptConsumeCash = pChk->IsChecked();
	GetGame()->GetConfigs()->SetLocalSettings(setting);
	GetGame()->GetConfigs()->SaveLocalSetting();
}

void CDlgMessageBoxYesNoCheckYuanBao::OnCommandCancel(const char* szCommand)
{
	Show(false);
}

void CDlgMessageBoxYesNoCheckYuanBao::OnShowDialog()
{
	PAUICHECKBOX pChk = (PAUICHECKBOX)GetDlgItem("Chk_NPop");
	EC_LOCAL_SETTING setting = GetGame()->GetConfigs()->GetLocalSettings();
	pChk->Check(setting.bStopPromptConsumeCash);
}


AUI_BEGIN_EVENT_MAP(CDlgSkillActive, CDlgBase)

AUI_ON_EVENT("Img_Skill",			WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgSkillActive::CDlgSkillActive()
{
}

CDlgSkillActive::~CDlgSkillActive()
{
}

bool CDlgSkillActive::OnInitDialog()
{
	m_pLabTime = (PAUILABEL)GetDlgItem("Lab_Time");
	m_pImgSkill = (PAUIIMAGEPICTURE)GetDlgItem("Img_Skill");
	return true;
}

void CDlgSkillActive::OnTick()
{
	if ( GetHostPlayer()->GetSkill(m_iId)==NULL || GetHostPlayer()->GetSkill(m_iId)->GetSkillLevel()==0)
	{
		m_iId		= 0;
		m_iTimeId	= 0;
		m_dwTime	= 0;
		m_iTotalTime= 0;
		Show(false);
		return;
	}
	if (m_dwTime==0)	// 时间信息未到
		return;

	ACString strText;
	int t = m_iTotalTime*1000 + m_dwTime - GetTickCount();
	strText.Format(_AL("%d"), t/1000);
	m_pLabTime->SetText(strText);
	AUIClockIcon *pClock;
	pClock = m_pImgSkill->GetClockIcon();
	pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
	if( m_pSkill && (m_pSkill->GetCoolingTime() > 0 ||
		GetHostPlayer()->GetPrepSkill() == m_pSkill ))
	{
		pClock->SetProgressRange(0, m_pSkill->GetCoolingTime());
		if( GetHostPlayer()->GetPrepSkill() == m_pSkill )
			pClock->SetProgressPos(0);
		else
			pClock->SetProgressPos(m_pSkill->GetCoolingTime() - m_pSkill->GetCoolingCnt());
	}

	if ( t<0 )
	{
		m_iId		= 0;
		m_iTimeId	= 0;
		m_dwTime	= 0;
		m_iTotalTime= 0;
		Show(false);
		return;
	}

}

// 2种信息先后顺序无法保证
int CDlgSkillActive::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="active")
	{
		CECSkill* pSkill = (CECSkill*)lParam;
		m_pSkill = pSkill;
		m_iId = pSkill->GetSkillID();
		
		AString strFile;
		af_GetFileTitle(pSkill->GetIconFile(), strFile);
		strFile.MakeLower();
		m_pImgSkill->SetCover(
			GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_SKILL),
			GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_SKILL, strFile));
		
		ACString strText;
		strText.Format(_AL("%d"), pSkill->GetSkillLevel());
		m_pImgSkill->SetText(strText);
		m_pImgSkill->SetDataPtr(pSkill,"ptr_CECSkill");
		m_pImgSkill->SetHint(pSkill->GetDesc());
		Show(true, false, false);
	}
	else if (strMsg=="time")
	{
		m_iTotalTime	= lParam;
		if ((int)wParam!=m_iId)
		{
			m_iTimeId	= wParam;
			m_dwTime = 0;
			return 0;
		}
		else
		{
			m_dwTime = GetTickCount();
		}
		
	}
	return 0;
}

void CDlgSkillActive::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECSkill") )
		return;

	CECSkill *pSkill = (CECSkill *)pObj->GetDataPtr("ptr_CECSkill");
	if( pSkill )
		GetHostPlayer()->ApplySkillShortcut(pSkill->GetSkillID());
}

const int	CDlgKrAntiwallow::ms_iTotalTime = 3000;

bool CDlgKrAntiwallow::OnInitDialog()
{
	m_pLabHint = (PAUILABEL)GetDlgItem("Lab_Warning");

	m_iShowTime = 0;
	return true;
}

void CDlgKrAntiwallow::OnTick()
{
	m_iShowTime += GetGame()->GetTickTime();
	if (m_iShowTime>ms_iTotalTime)
	{
		Show(false);
		m_iShowTime = 0;
	}

}

int CDlgKrAntiwallow::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="show")
	{
		m_iShowTime = 0;
		ACString strText;
		strText.Format(GetGameUIMan()->GetStringFromTable(12998), wParam);
		m_pLabHint->SetText(strText);
		Show(true);
	}

	return 0;
}


AUI_BEGIN_COMMAND_MAP(CDlgTreasureEnter, CDlgBase)

AUI_ON_COMMAND("Btn_LinkPage",	OnCommandConfirm)

AUI_END_COMMAND_MAP()

void CDlgTreasureEnter::OnCommandConfirm(const char* szCommand)
{
	AString strText = "";
	AIniFile iniFile;
	if(iniFile.Open("configs\\url.ini"))
	{
		strText = iniFile.GetValueAsString("ingame", "xunbao", "");
		iniFile.Close();
	}
	if( strText != "" )
		ShellExecuteA(NULL,"open", strText, NULL, NULL, SW_SHOWNORMAL);
}


AUI_BEGIN_COMMAND_MAP(CDlgHelpAttack, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",	OnCommandCancel)

AUI_END_COMMAND_MAP()

void CDlgHelpAttack::OnCommandCancel(const char* szCommand)
{
	GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_5_1_ATTACK);
	GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_7_3_TALISMAN_HPMP);
	GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_9_3_SKILL_SHORTCUT);
	Show(false);
}


AUI_BEGIN_COMMAND_MAP(CDlgHelpStart, CDlgBase)

AUI_ON_COMMAND("Btn_1",	OnCommandStart)
AUI_ON_COMMAND("Btn_2",	OnCommandStop)

AUI_END_COMMAND_MAP()

void CDlgHelpStart::OnCommandStart(const char* szCommand)
{
	GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_0_1_OPEN_ORNOT);
	GetGameRun()->GetHelpGraphGuide()->StartHelp(CECHelpGraphGuide::HELP2012_1_1_WASD_MOVE);
	Show(false);
}

void CDlgHelpStart::OnCommandStop(const char* szCommand)
{
	GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_0_1_OPEN_ORNOT);
	GetGameRun()->GetHelpGraphGuide()->SetState(CECHelpGraphGuide::HELP2012_CLOSE);
	Show(false);
}

AUI_BEGIN_EVENT_MAP(CDlgEventImmune, CDlgBase)

AUI_ON_EVENT("*",				WM_LBUTTONDOWN,		OnEventAllKinds)
AUI_ON_EVENT("NULL",			WM_LBUTTONDOWN,		OnEventAllKinds)
AUI_ON_EVENT("*",				WM_RBUTTONDOWN,		OnEventAllKinds)
AUI_ON_EVENT("NULL",			WM_RBUTTONDOWN,		OnEventAllKinds)
AUI_ON_EVENT("*",				WM_LBUTTONDBLCLK,	OnEventAllKinds)
AUI_ON_EVENT("NULL",			WM_LBUTTONDBLCLK,	OnEventAllKinds)
AUI_ON_EVENT("*",				WM_RBUTTONUP,		OnEventAllKinds)
AUI_ON_EVENT("NULL",			WM_RBUTTONUP,		OnEventAllKinds)

AUI_END_EVENT_MAP()

void CDlgEventImmune::OnEventAllKinds(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	GetGameUIMan()->RespawnMessage();
}



AUI_BEGIN_COMMAND_MAP(CDlgWebHot, CDlgBase)

AUI_ON_COMMAND("Btn_Max",	OnCommandOpen)

AUI_END_COMMAND_MAP()

void CDlgWebHot::OnCommandOpen(const char* szCommand)
{
	GetGameSession()->open_url_by_token("webhot");
}


AUI_BEGIN_COMMAND_MAP(CDlgHelpGraphGuide, CDlgBase)

AUI_ON_COMMAND("Btn_Max",	OnCommandOpen)

AUI_END_COMMAND_MAP()

void CDlgHelpGraphGuide::OnCommandOpen(const char* szCommand)
{
	CDlgMiniMap * pDlgMiniMap = (CDlgMiniMap*)GetGameUIMan()->GetDialog("Win_MiniMap");
	pDlgMiniMap->OnCommand_NewHelp("");
}


AUI_BEGIN_COMMAND_MAP(CDlgGT, CDlgBase)

AUI_ON_COMMAND("Btn_Gt",	OnCommand_GT)

AUI_END_COMMAND_MAP()

void CDlgGT::OnCommand_GT(const char* szCommand)
{
	if (GetGameRun()->IsGTLogin())
	{
		overlay::GTOverlay::Instance().ClickGTIcon();
	}
	else
		GetGame()->GetGameSession()->get_platform_token("gt", 24, 2400);
}

AUI_BEGIN_COMMAND_MAP(CDlgKnowsMin, CDlgBase)

AUI_ON_COMMAND("Btn_Max",	OnCommandOpen)

AUI_END_COMMAND_MAP()

void CDlgKnowsMin::OnCommandOpen(const char* szCommand)
{
	CDlgBaseExplorer* pDlgExplorer = (CDlgBaseExplorer*)GetGameUIMan()->GetDialog("Win_ZhuxianKnows");
	pDlgExplorer->Show(!pDlgExplorer->IsShow());
}