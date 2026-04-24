// Filename	: DlgBattleTeamCustom.cpp
// Creator	: Fu Chonggang
// Date		: 2012.7.30

#include "DlgBattleTeamCustom.h"
#include "DlgSkill.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Skill.h"
#include "EC_BattleDungeon.h"
#include "DlgMinimizeBar.h"
#include "DlgBattleDungeonApply.h"

#include "teamraidbeginchoosechar.hpp"
#include "teamraidupdatechar.hpp"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBattleTeamCustom, CDlgTab3D)

AUI_ON_COMMAND("Btn_Confirm",		OnCommandConfirm)
AUI_ON_COMMAND("Btn_ChangeSkill",	OnCommandSkillSel)
AUI_ON_COMMAND("minimize",			OnCommandMinimize)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgBattleTeamCustom, CDlgTab3D)

AUI_ON_EVENT("Item_*",			WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

const int CDlgBattleTeamCustom::ms_iSkillIds[6] = {3991, 3992, 3993, 3994, 3995, 3996};

CDlgBattleTeamCustom::CDlgBattleTeamCustom()
{
	m_fDeltaY = -150;
	m_iTimeToStart = 0;
}

CDlgBattleTeamCustom::~CDlgBattleTeamCustom()
{
}

bool CDlgBattleTeamCustom::OnInitDialog()
{
	bool bRet = CDlgTab3D::OnInitDialog();
	
	int i;
	for (i=0; i<6; ++i)
	{
		m_pSkills[i] = new CECSkill(ms_iSkillIds[i], 1);
	}
	m_iFigureNum = 0;
	ReleaseSprites();
	m_pConfig = &GetGameRun()->GetCollisionRaidTransfigureConfig();
	for (i=0; i<20; ++i)
	{
		if (m_pConfig->transfigure_info_list[i].transfigure_id==0)
			break;
		
		AddTextrure(GetGame()->GetDataPath(m_pConfig->transfigure_info_list[i].file_icon));
	}
	
	m_iFigureNum = i;

	m_pLabTime = (PAUILABEL)GetDlgItem("Txt_Time");
	OnSelOneItem();

	return bRet;
}

bool CDlgBattleTeamCustom::Release()
{
	int i;
	for (i=0; i<6; ++i)
	{
		delete m_pSkills[i];
	}
	return CDlgTab3D::Release();
}

void CDlgBattleTeamCustom::OnTick()
{
	m_iTimeToStart -= GetGame()->GetTickTime();
	ACString strText;
	strText.Format(_AL("%d"), m_iTimeToStart/1000);
	m_pLabTime->SetText(strText);
	CDlgTab3D::OnTick();
}

void CDlgBattleTeamCustom::OnCommandConfirm(const char * szCommand)
{
/*	TeamRaidUpdateChar p;
	p.raidroom_id = CECBattleDungeonMan::getSingletonPtr()->m_iRoomId;
	p.map_id = CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iMapId;
	unsigned int i;
	for (i=0; i<m_vecRoles.size(); ++i)
	{
		if(m_vecRoles[i].roleid == GetHostPlayer()->GetCharacterID())
		{
			p.roledata = m_vecRoles[i];
		}
	}
	p.roledata.character = m_iCurPos+1;
	CECSkill *pSkill = (CECSkill*)GetDlgItem("Img_SkillSelect1")->GetDataPtr("ptr_CECSkill");
	if (!pSkill)
	{
		return;
	}
	p.roledata.skillid1 = pSkill->GetSkillID();
	pSkill = (CECSkill*)GetDlgItem("Img_SkillSelect2")->GetDataPtr("ptr_CECSkill");
	if (!pSkill)
	{
		return;
	}
	p.roledata.skillid2 = pSkill->GetSkillID();
	GetGameSession()->SendNetData(p);
*/}

void CDlgBattleTeamCustom::OnCommandSkillSel(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_CharSelect_Skill");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgBattleTeamCustom::OnCommandMinimize(const char * szCommand)
{
	GetGameUIMan()->m_pDlgMinimizeBar[0]->MinimizeDialog(this, GetGameUIMan()->GetStringFromTable(15035), CDlgMinimizeBar::MINIMIZEBAR_TYPE_COLLISION6V6);
}

void CDlgBattleTeamCustom::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
}

int CDlgBattleTeamCustom::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	using namespace GNET;
	if (strMsg=="begin")
	{
		TeamRaidBeginChooseChar* p = (TeamRaidBeginChooseChar*)wParam;
		if (!IsShow())
		{
			Show(true);
		}
		m_vecRoles.clear();
		int i;
		for (i=0; i<(int)p->roles.roles.size(); ++i)
		{
			m_vecRoles.push_back(p->roles.roles[i]);
		}
		UpdateDisplay();
		CDlgBattleTeam6v6Skill* pDlgBattleTeam6v6Skill = (CDlgBattleTeam6v6Skill*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_CharSelect_Skill");
		pDlgBattleTeam6v6Skill->InitSkills();
		CECBattleDungeonMan::getSingletonPtr()->m_iRoomId = p->raidroom_id;
		CECBattleDungeonMan::getSingletonPtr()->InitFromMapId(p->map_id);
		
		// Ãû×Ö
		// PAUILABEL pLab = (PAUILABEL)GetDlgItem("Title");
		// pLab->SetText(CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_pConfigData->name);
		// Í¼Æ¬ÃèÊö
		int index = CDlgBattleDungeonApply::GetIndexFromTmplId(CECBattleDungeonMan::getSingletonPtr()->GetCurBattle()->m_iId);
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Map");
		AUIOBJECT_SETPROPERTY prop;
		sprintf(prop.fn, AC2AS(GetGameUIMan()->GetStringFromTable(index+1)));
		pImg->SetProperty("Image File", &prop);
		// ÎÄ×ÖÃèÊö
		PAUITEXTAREA pTxt = (PAUITEXTAREA)GetDlgItem("Txt_Intro");
		pTxt->SetText(GetGameUIMan()->GetStringFromTable(index));

		OnCommandConfirm("");

		m_iTimeToStart = 30000;
	}
	else if (strMsg=="single")
	{
		TeamRaidUpdateChar* p = (TeamRaidUpdateChar*)wParam;
		unsigned int i;
		for (i=0; i<m_vecRoles.size(); ++i)
		{
			if(m_vecRoles[i].roleid == p->roledata.roleid)
			{
				m_vecRoles[i] = p->roledata;
/*				AString strDebug;
				strDebug.Format("single role info:id:%d,team:%d,char:%d,skill1:%d,skill2:%d", p->roledata.roleid
					, p->roledata.team, p->roledata.character, p->roledata.skillid1, p->roledata.skillid2);
				OutputDebugStringA(strDebug);
*/				break;
			}
		}
		if(i==m_vecRoles.size())
			m_vecRoles.push_back(p->roledata);
		UpdateChars();
	}
	else if (strMsg=="apply_error")
	{
		ACString strText;
		strText.Format(GetGameUIMan()->GetStringFromTable(12264), GetGameRun()->GetPlayerName(wParam, true));
		GetGameUIMan()->MessageBox("", strText);
	}

	return 0;
}

void CDlgBattleTeamCustom::UpdateChars()
{
/*	int i;
	AString strName;
	PAUIIMAGEPICTURE pImg;
	int iTeam1 = 0;
	int iTeam2 = 0;
	unsigned int j;
	CDlgSkill *pDlgSkill = (CDlgSkill*)GetGameUIMan()->GetDialog("Win_Skill");
	CECSkill *pSkill;
	int hostTeam;
	for (j=0; j<m_vecRoles.size(); ++j)
	{
		GNET::TeamRaidRoleData &roleInfo = m_vecRoles[j];
		if (roleInfo.roleid==GetHostPlayer()->GetCharacterID())
		{
			hostTeam = roleInfo.team;
			break;
		}
	}
	for (j=0; j<m_vecRoles.size(); ++j)
	{
		GNET::TeamRaidRoleData &roleInfo = m_vecRoles[j];
		if (roleInfo.team==hostTeam)
		{
			strName.Format("Img_Char%02d", iTeam1+1);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			AUIOBJECT_SETPROPERTY p;
			AString strPath = GetGame()->GetDataPath(m_pConfig->transfigure_info_list[roleInfo.character-1].file_icon);
			if (strPath.GetLength())
			{
				strPath.CutLeft(strlen("surfaces\\"));
			}
			GenerateHeadPath(strPath);
			sprintf(p.fn, strPath);
			pImg->SetProperty("Image File", &p);
			pImg->SetHint(m_pConfig->transfigure_info_list[roleInfo.character-1].description);
			strName.Format("Img_CharSkill%02d", iTeam1*2+1);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pSkill = GetSkillById(roleInfo.skillid1);
			pDlgSkill->SetImage(pImg, pSkill);
			strName.Format("Img_CharSkill%02d", iTeam1*2+2);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pSkill = GetSkillById(roleInfo.skillid2);
			pDlgSkill->SetImage(pImg, pSkill);
			iTeam1++;
		}
		else
		{
			strName.Format("Img_Char%02d", iTeam2+1+6);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			AUIOBJECT_SETPROPERTY p;
			AString strPath = GetGame()->GetDataPath(m_pConfig->transfigure_info_list[roleInfo.character-1].file_icon);
			if (strPath.GetLength())
			{
				strPath.CutLeft(strlen("surfaces\\"));
			}
			GenerateHeadPath(strPath);
			sprintf(p.fn, strPath);
			pImg->SetProperty("Image File", &p);
			pImg->SetHint(m_pConfig->transfigure_info_list[roleInfo.character-1].description);
			strName.Format("Img_CharSkill%02d", iTeam2*2+1+12);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pDlgSkill->SetImage(pImg, GetSkillById(roleInfo.skillid1));
			strName.Format("Img_CharSkill%02d", iTeam2*2+2+12);
			pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
			pDlgSkill->SetImage(pImg, GetSkillById(roleInfo.skillid2));
			iTeam2++;
		}
	}
	for (i=iTeam1; i<6; ++i)
	{
		strName.Format("Img_Char%02d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->ClearCover();
		strName.Format("Img_CharSkill%02d", i*2+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->ClearCover();
		pImg->SetHint(_AL(""));
		strName.Format("Img_CharSkill%02d", i*2+2);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->ClearCover();
		pImg->SetHint(_AL(""));
	}
	for (i=iTeam2; i<6; ++i)
	{
		strName.Format("Img_Char%02d", i+1+6);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->ClearCover();
		strName.Format("Img_CharSkill%02d", i*2+1+12);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->ClearCover();
		pImg->SetHint(_AL(""));
		strName.Format("Img_CharSkill%02d", i*2+2+12);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->ClearCover();
		pImg->SetHint(_AL(""));
	}
*/}

void CDlgBattleTeamCustom::GenerateHeadPath(AString &strPath)
{
	int iIndex = strPath.ReverseFind('.');
	if (iIndex <= 0)
		return;

	AString strName = strPath.Left(iIndex);
	AString strExt = strPath.Right(strPath.GetLength()-iIndex);

	strPath = strName + "_head" + strExt;
}

void CDlgBattleTeamCustom::AddTextrures()
{
}

void CDlgBattleTeamCustom::UpdateDisplay()
{
	UpdateChars();
}

void CDlgBattleTeamCustom::OnSelOneItem()
{
	PAUITEXTAREA pTxt = (PAUITEXTAREA)GetDlgItem("Txt_CharIntro01");
	pTxt->SetText(m_pConfig->transfigure_info_list[m_iCurPos].description);
}

CECSkill* CDlgBattleTeamCustom::GetSkillById(int id)
{
	int i;
	for (i=0; i<6; ++i)
	{
		if (m_pSkills[i]->GetSkillID()==id)
			return m_pSkills[i];
	}

	return 0;
}


/************************************************************************
CDlgBattleTeam6v6Skill
************************************************************************/
AUI_BEGIN_COMMAND_MAP(CDlgBattleTeam6v6Skill, CDlgBase)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgBattleTeam6v6Skill, CDlgBase)

AUI_ON_EVENT("Img_Cover*",			WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_Skill*",			WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgBattleTeam6v6Skill::CDlgBattleTeam6v6Skill()
{
}

CDlgBattleTeam6v6Skill::~CDlgBattleTeam6v6Skill()
{
}

bool CDlgBattleTeam6v6Skill::OnInitDialog()
{
	int i;
	for (i=0; i<6; ++i)
	{
		AString strName;
		strName.Format("Img_Skill%02d", i+1);
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Img_Cover%02d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->Show(false);
	}
	return true;
}

void CDlgBattleTeam6v6Skill::OnShowDialog()
{
	CDlgBattleTeamCustom *pDlg = (CDlgBattleTeamCustom*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_CharSelect");
	CDlgSkill *pDlgSkill = (CDlgSkill*)GetGameUIMan()->GetDialog("Win_Skill");
	int i;

	PAUIIMAGEPICTURE pImg;
	for (i=0; i<6; ++i)
	{
		AString strName;
		strName.Format("Img_Skill%02d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pDlgSkill->SetImage(pImg, pDlg->GetSkill(i));
		strName.Format("Img_Cover%02d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->SetHint(pDlg->GetSkill(i)->GetDesc());
		pImg->Show(false);
	}
}

void CDlgBattleTeam6v6Skill::OnTick()
{
}

void CDlgBattleTeam6v6Skill::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	CDlgBattleTeamCustom *pDlg = (CDlgBattleTeamCustom*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_CharSelect");
	if (strstr(pObj->GetName(), "Img_Cover"))
	{
		return;
	}
	else if (strstr(pObj->GetName(), "Img_Skill"))
	{
		AString strName;
		int index = atoi(pObj->GetName()+strlen("Img_Skill"));
		strName.Format("Img_Cover%02d", index);
		GetDlgItem(strName)->Show(true);
	}
	int i;
	int iSkillSel[2] = {0};
	int iNumSkillSel = 0;
	PAUIIMAGEPICTURE pImg[2];
	CDlgSkill *pDlgSkill = (CDlgSkill*)GetGameUIMan()->GetDialog("Win_Skill");
	pImg[0] = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_CharSelect")->GetDlgItem("Img_SkillSelect1");
	pImg[1] = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_CharSelect")->GetDlgItem("Img_SkillSelect2");
	for (i=0; i<6; ++i)
	{
		AString strName;
		strName.Format("Img_Cover%02d", i+1);
		if (GetDlgItem(strName)->IsShow())
		{
			pDlgSkill->SetImage(pImg[iNumSkillSel], pDlg->GetSkill(i));
			iNumSkillSel++;
			if (iNumSkillSel==2)
				break;
		}
	}
	if (iNumSkillSel==2)
	{
		Show(false);
	}
}

void CDlgBattleTeam6v6Skill::InitSkills()
{
	GetDlgItem("Img_Cover01")->Show(false);
	GetDlgItem("Img_Cover02")->Show(false);
	GetDlgItem("Img_Cover03")->Show(false);
	GetDlgItem("Img_Cover04")->Show(false);
	GetDlgItem("Img_Cover05")->Show(false);
	GetDlgItem("Img_Cover06")->Show(false);
	OnEventLButtonDown(0, 0, GetDlgItem("Img_Skill01"));
	OnEventLButtonDown(0, 0, GetDlgItem("Img_Skill02"));

}