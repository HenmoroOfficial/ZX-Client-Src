// Filename	:	DlgSkillzx2.cpp
// Creator	:	Fu Chonggang
// Date		:	Aug 27, 2009

#include "DlgSkillMan.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIRadioButton.h"
#include "DlgSkillTree.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_ElsePlayer.h"
#include "EC_ManPlayer.h"
#include "EC_World.h"
#include "EC_NewHelpGuide.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSkillMan, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Close",				OnCommand_close)
AUI_ON_COMMAND("Rdo_Desc",				OnCommand_desc)
AUI_ON_COMMAND("Btn_Talent",			OnCommand_talent)
AUI_ON_COMMAND("Rdo_Skilltree",			OnCommand_skilltree)
AUI_ON_COMMAND("Rdo_Taoism",			OnCommand_taoism)
AUI_ON_COMMAND("Rdo_Evil",				OnCommand_evil)
AUI_ON_COMMAND("Rdo_Buddhism",			OnCommand_buddhism)
AUI_ON_COMMAND("Rdo_Couple",			OnCommand_couple)
AUI_ON_COMMAND("Rdo_Family",			OnCommand_family)
AUI_ON_COMMAND("Rdo_Teacher",			OnCommand_teacher)
AUI_ON_COMMAND("Rdo_Talisman",			OnCommand_talisman)
AUI_ON_COMMAND("Rdo_Edit",				OnCommand_edit)
AUI_ON_COMMAND("Rdo_Life",				OnCommand_life)
AUI_ON_COMMAND("Rdo_Article",			OnCommand_article)
AUI_ON_COMMAND("RdoMain_*",				OnCommand_RdoType)
AUI_ON_COMMAND("Btn_Main5",				OnCommand_SkillSeq)
AUI_ON_COMMAND("Rdo_Fengshen_Taoism",	OnCommand_SoulTaoism)
AUI_ON_COMMAND("Rdo_Fengshen_Evil",		OnCommand_SoulEvil)
AUI_ON_COMMAND("Rdo_Fengshen_Buddhism",	OnCommand_SoulBuddhism)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgSkillMan, CDlgBase)


AUI_END_EVENT_MAP()

CDlgSkillMan::CDlgSkillMan()
{
}

CDlgSkillMan::~CDlgSkillMan()
{
}

bool CDlgSkillMan::Release()
{
	return CDlgBase::Release();
}

bool CDlgSkillMan::OnInitDialog()
{
	m_pTitle= (AUILabel *)GetDlgItem("Title");
	m_pBtn_Close= (AUIStillImageButton *)GetDlgItem("Btn_Close");
	m_pImg_Bar= (AUIImagePicture *)GetDlgItem("Img_Bar");
	m_pRdo_Desc= (AUIRadioButton *)GetDlgItem("Rdo_Desc");
	m_pBtn_Talent= (AUIStillImageButton *)GetDlgItem("Btn_Talent");
	m_pRdo_Skilltree= (AUIRadioButton *)GetDlgItem("Rdo_Skilltree");
	m_pRdo_Taoism= (AUIRadioButton *)GetDlgItem("Rdo_Taoism");
	m_pRdo_Evil= (AUIRadioButton *)GetDlgItem("Rdo_Evil");
	m_pRdo_Buddhism= (AUIRadioButton *)GetDlgItem("Rdo_Buddhism");
	m_pRdo_SoulTaoism= (AUIRadioButton *)GetDlgItem("Rdo_Fengshen_Taoism");
	m_pRdo_SoulEvil= (AUIRadioButton *)GetDlgItem("Rdo_Fengshen_Evil");
	m_pRdo_SoulBuddhism= (AUIRadioButton *)GetDlgItem("Rdo_Fengshen_Buddhism");
	m_pRdo_Couple= (AUIRadioButton *)GetDlgItem("Rdo_Couple");
	m_pRdo_Teacher= (AUIRadioButton *)GetDlgItem("Rdo_Teacher");
	m_pRdo_Talisman= (AUIRadioButton *)GetDlgItem("Rdo_Talisman");
	m_pRdo_Edit= (AUIRadioButton *)GetDlgItem("Rdo_Edit");
	m_pShape1= (AUIEditBox *)GetDlgItem("Shape1");
	m_pImg_Line2= (AUIImagePicture *)GetDlgItem("Img_Line2");
	m_pRdo_1= (AUIRadioButton *)GetDlgItem("Rdo_1");
	m_pRdo_2= (AUIRadioButton *)GetDlgItem("Rdo_2");
	m_pRdo_3= (AUIRadioButton *)GetDlgItem("Rdo_3");
	m_pRdo_4= (AUIRadioButton *)GetDlgItem("Rdo_4");
	m_pSubDLeft= (AUISubDialog *)GetDlgItem("SubLeft");
	m_pSubDRight= (AUISubDialog *)GetDlgItem("SubRight");
	m_pSubDMiddle= (AUISubDialog *)GetDlgItem("Sub_Fengshen");
	
	SetData(0);
	return true;
}

void CDlgSkillMan::OnShowDialog()
{
	CheckRadioButton(1,1);
	OnCommand_RdoType("RdoMain_1");
	m_pTitle->SetText(GetGameUIMan()->GetStringFromTable(9300));
	if (GetData()>0)
	{
		CECElsePlayer *pPlayer = GetWorld()->GetPlayerMan()->GetElsePlayer(GetData());
		if (pPlayer)
		{
			ACString strText;
			strText.Format(GetGameUIMan()->GetStringFromTable(9301), pPlayer->GetName());
			m_pTitle->SetText(strText);
		}
	}
}

void CDlgSkillMan::OnTick()
{
	if( GetData()>0 )
	{
		GetDlgItem("Btn_Main5")->Enable(false);
	}
	else
	{
		GetDlgItem("Btn_Main5")->Enable(true);
		m_pTitle->SetText(GetGameUIMan()->GetStringFromTable(9300));
	}
}

bool CDlgSkillMan::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

int CDlgSkillMan::OnChangeUIMan(BYTE bUserChanged, BYTE bRoleChanged, BYTE bLineChanged)
{
	if (IsShow())
		Show(false);
	return 1;
}

void CDlgSkillMan::OnCommand_CANCEL(const char * szCommand)
{
	GetGameUIMan()->m_pDlgSkillMan->OnCommand_close("");
}

void CDlgSkillMan::OnCommand_close(const char * szCommand)
{
	SetData(0);
	Show(false);
	GetGameUIMan()->GetDialog("Win_SkillComboEdit")->Show(false);
	NewHelpGuide::FinishGuide_SkillAddPoint();
}

void CDlgSkillMan::OnCommand_desc(const char * szCommand)
{
	PAUIDIALOG pDlgLeft = NULL;
	PAUIDIALOG pDlgRight = NULL;

	int idProf;
	if( GetData()>0 )
	{
		CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetData());
		if( !pPlayer )
			return;
		idProf = pPlayer->GetProfession();
	}
	else
	{
		idProf = GetHostPlayer()->GetProfession();
		pDlgLeft = GetGameUIMan()->GetDialog("Win_Skill");
	}
	ASSERT(idProf >= 0 && idProf < NUM_PROFESSION);
	char szDlg[20];
	if (idProf>=NUM_PROFESSION)
	{
		return;
	}
	sprintf(szDlg, "Win_Skill%02d", idProf);
	pDlgRight = GetGameUIMan()->GetDialog(szDlg);

	m_pSubDLeft->SetDialog(pDlgLeft);
	m_pSubDRight->SetDialog(pDlgRight);
}

void CDlgSkillMan::OnCommand_talent(const char * szCommand)
{
	PAUIDIALOG pDlgRight = NULL;
	int idProf;
	if( GetData()>0 )
	{
		CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetData());
		if( !pPlayer )
			return;
		idProf = pPlayer->GetProfession();
	}
	else
	{
		idProf = GetHostPlayer()->GetProfession();
	}
	for (int i=0; i<CECGAMEUIMAN_MAX_PROFRESSION; i++)
	{
		PAUIDIALOG pDlg = GetGameUIMan()->m_pDlgSkillTree[i];
		if ( pDlg && pDlg->IsShow())
		{
			idProf = atoi(pDlg->GetName() + strlen("Win_Skill"));
		}
	}
	int idTalent = talent[idProf]; 
	char szName[30];
	sprintf(szName, "Win_Talent_%02d", idTalent);
	pDlgRight = GetGameUIMan()->GetDialog(szName);

	if(NULL != pDlgRight)
	{
	   pDlgRight->Show(!pDlgRight->IsShow());
	   pDlgRight->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	}
}

void CDlgSkillMan::OnCommand_skilltree(const char * szCommand)
{
	PAUIDIALOG pDlgLeft = NULL;
	PAUIDIALOG pDlgRight = NULL;

	int idProf;
	if( GetData()>0 )
	{
		CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetData());
		if( !pPlayer )
			return;
		idProf = pPlayer->GetProfession();
	}
	else
		idProf = GetHostPlayer()->GetProfession();
	ASSERT(idProf >= 0 && idProf < NUM_PROFESSION);
	if ((idProf>=0 && idProf<=PROF_31)||(idProf>=PROF_65 && idProf<=PROF_69))
		pDlgLeft = GetGameUIMan()->GetDialog("Win_SkillProfTree");
	else if((idProf>=PROF_103 && idProf<=PROF_107)||(idProf>=PROF_109 && idProf<=PROF_113))
		pDlgLeft = GetGameUIMan()->GetDialog("Win_SkillProfTree3");
	else
		pDlgLeft = GetGameUIMan()->GetDialog("Win_SkillProfTree2");
	AString strDlg;
	strDlg.Format("Win_Skill%02d", idProf);
	pDlgRight = GetGameUIMan()->GetDialog(strDlg);

	m_pSubDLeft->SetDialog(pDlgLeft);
	m_pSubDRight->SetDialog(pDlgRight);
}

void CDlgSkillMan::OnCommand_taoism(const char * szCommand)
{
	PAUIDIALOG pDlgLeft = GetGameUIMan()->GetDialog("Win_Taoism");
	PAUIDIALOG pDlgRight = NULL;
	PAUIDIALOG pDlgSkill = GetGameUIMan()->GetDialog("Win_SkillSpec");
	
	AString strName;
	int iProf;
	if( GetData()>0 )
	{
		CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetData());
		if( !pPlayer )
			return;
		iProf = pPlayer->GetProfession();
	}
	else
	{
		iProf = GetHostPlayer()->GetProfession();
	}
	if (pDlgRight&&pDlgRight->GetCheckedRadioButton(1)>0)
		iProf = pDlgRight->GetCheckedRadioButton(1);
	else
	{
		iProf = Prof2Index(iProf);
	}
	strName.Format("Win_Skill_Taoism2_%d",iProf);
	pDlgRight = GetGameUIMan()->GetDialog(strName);

	if (GetData()==0)
	{
		pDlgSkill->Show(true);
		pDlgSkill->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	}
	m_pSubDLeft->SetDialog(pDlgLeft);
	m_pSubDRight->SetDialog(pDlgRight);
}

void CDlgSkillMan::OnCommand_evil(const char * szCommand)
{
	PAUIDIALOG pDlgLeft = GetGameUIMan()->GetDialog("Win_Evil");
	PAUIDIALOG pDlgRight = NULL;
	PAUIDIALOG pDlgSkill = GetGameUIMan()->GetDialog("Win_SkillSpec");
	
	AString strName;
	int iProf;
	if( GetData()>0 )
	{
		CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetData());
		if( !pPlayer )
			return;
		iProf = pPlayer->GetProfession();
	}
	else
	{
		iProf = GetHostPlayer()->GetProfession();
	}
	if (pDlgRight&&pDlgRight->GetCheckedRadioButton(1)>0)
		iProf = pDlgRight->GetCheckedRadioButton(1);
	else
	{
		iProf = Prof2Index(iProf);
	}
	strName.Format("Win_Skill_Evil2_%d",iProf);
	pDlgRight = GetGameUIMan()->GetDialog(strName);
	
	if (GetData()==0)
	{
		pDlgSkill->Show(true);
		pDlgSkill->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	}
	m_pSubDLeft->SetDialog(pDlgLeft);
	m_pSubDRight->SetDialog(pDlgRight);
}

void CDlgSkillMan::OnCommand_buddhism(const char * szCommand)
{
	PAUIDIALOG pDlgLeft = GetGameUIMan()->GetDialog("Win_Buddhism");
	PAUIDIALOG pDlgRight = NULL;
	PAUIDIALOG pDlgSkill = GetGameUIMan()->GetDialog("Win_SkillSpec");
	
	AString strName;
	int iProf;
	if( GetData()>0 )
	{
		CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetData());
		if( !pPlayer )
			return;
		iProf = pPlayer->GetProfession();
	}
	else
	{
		iProf = GetHostPlayer()->GetProfession();
	}
	if (pDlgRight&&pDlgRight->GetCheckedRadioButton(1)>0)
		iProf = pDlgRight->GetCheckedRadioButton(1);
	else
	{
		iProf = Prof2Index(iProf);
	}
	strName.Format("Win_Skill_Buddhism2_%d",iProf);
	pDlgRight = GetGameUIMan()->GetDialog(strName);
	
	if (GetData()==0)
	{
		pDlgSkill->Show(true);
		pDlgSkill->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	}
	m_pSubDLeft->SetDialog(pDlgLeft);
	m_pSubDRight->SetDialog(pDlgRight);
}

int	CDlgSkillMan::Prof2Index(int prof)
{
	int iProf = 0;
	if (prof<=12)
		iProf = (prof+2)/3;
	else if (prof<=24)
		iProf = (prof-12+2)/3;
	else if (prof<=30)
		iProf = 5;
	else if (prof<=38)
		iProf = 6;
	else if (prof<=44)
		iProf = 7;
 	else if (prof<=50)
 		iProf = 10;
	else if (prof<=55)
		iProf = 12;
	else if (prof<=60)
 		iProf = 11;
	else if (prof<=69)
		iProf = 8;
	else if (prof<=101)
 		iProf = 9;
	else if(prof <= 106)
		iProf = 14;
	else if(prof <= 112)
		iProf = 13;
	return iProf;		
}

void CDlgSkillMan::OnCommand_couple(const char * szCommand)
{
	PAUIDIALOG pDlgLeft = NULL;
	PAUIDIALOG pDlgRight = NULL;
	
	if( GetData()==0 )
		pDlgLeft = GetGameUIMan()->GetDialog("Win_SkillSocial");
	pDlgRight = GetGameUIMan()->GetDialog("Win_Couple");
	
	m_pSubDLeft->SetDialog(pDlgLeft);
	m_pSubDRight->SetDialog(pDlgRight);
}

void CDlgSkillMan::OnCommand_family(const char * szCommand)
{
	PAUIDIALOG pDlgLeft = NULL;
	PAUIDIALOG pDlgRight = NULL;
	
	if( GetData()==0 )
		pDlgLeft = GetGameUIMan()->GetDialog("Win_SkillSocial");
	pDlgRight = NULL;
	
	m_pSubDLeft->SetDialog(pDlgLeft);
	m_pSubDRight->SetDialog(pDlgRight);
}

void CDlgSkillMan::OnCommand_teacher(const char * szCommand)
{
	PAUIDIALOG pDlgLeft = NULL;
	PAUIDIALOG pDlgRight = NULL;
	
	if( GetData()==0 )
		pDlgLeft = GetGameUIMan()->GetDialog("Win_SkillSocial");
	pDlgRight = GetGameUIMan()->GetDialog("Win_Teacher");
	
	m_pSubDLeft->SetDialog(pDlgLeft);
	m_pSubDRight->SetDialog(pDlgRight);
}

void CDlgSkillMan::OnCommand_talisman(const char * szCommand)
{
	PAUIDIALOG pDlgLeft = NULL;
	PAUIDIALOG pDlgRight = NULL;
	
	if( GetData()==0 )
		pDlgLeft = GetGameUIMan()->GetDialog("Win_SkillOther");
	pDlgRight = GetGameUIMan()->GetDialog("Win_PetCatch");
	
	m_pSubDLeft->SetDialog(pDlgLeft);
	m_pSubDRight->SetDialog(pDlgRight);
}

void CDlgSkillMan::OnCommand_edit(const char * szCommand)
{
	PAUIDIALOG pDlgLeft = NULL;
	PAUIDIALOG pDlgRight = NULL;
	
	if( GetData()==0 )
		pDlgLeft = GetGameUIMan()->GetDialog("Win_SkillEquip");
	pDlgRight = NULL;
	
	m_pSubDLeft->SetDialog(pDlgLeft);
	m_pSubDRight->SetDialog(pDlgRight);
}

void CDlgSkillMan::OnCommand_life(const char * szCommand)
{
	PAUIDIALOG pDlgLeft = NULL;
	PAUIDIALOG pDlgRight = NULL;
	
	if( GetData()==0 )
		pDlgLeft = GetGameUIMan()->GetDialog("Win_SkillOther");
	pDlgRight = NULL;
	
	m_pSubDLeft->SetDialog(pDlgLeft);
	m_pSubDRight->SetDialog(pDlgRight);
}

void CDlgSkillMan::OnCommand_article(const char * szCommand)
{
	PAUIDIALOG pDlgLeft = NULL;
	PAUIDIALOG pDlgRight = NULL;
	
	if( GetData()==0 )
		pDlgLeft = GetGameUIMan()->GetDialog("Win_SkillOther");
	//pDlgRight = GetGameUIMan()->GetDialog("Win_SkillEdit");
	
	m_pSubDLeft->SetDialog(pDlgLeft);
	m_pSubDRight->SetDialog(pDlgRight);
}

void CDlgSkillMan::OnCommand_SoulTaoism(const char * szCommand)
{
	int iProf;
	if( GetData()>0 )
	{
		CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetData());
		if( !pPlayer )
			return;
		iProf = pPlayer->GetProfession();
	}
	else
	{
		iProf = GetHostPlayer()->GetProfession();
	}

	AString strName;
	strName.Format("Win_Skill_Fengshen_Taoism%d", Prof2Index(iProf));
	PAUIDIALOG pDlgMiddle = GetGameUIMan()->GetDialog(strName);
	if (!pDlgMiddle)
		return;

	m_pSubDMiddle->SetDialog(pDlgMiddle);
}

void CDlgSkillMan::OnCommand_SoulEvil(const char * szCommand)
{
	int iProf;
	if( GetData()>0 )
	{
		CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetData());
		if( !pPlayer )
			return;
		iProf = pPlayer->GetProfession();
	}
	else
	{
		iProf = GetHostPlayer()->GetProfession();
	}

	AString strName;
	strName.Format("Win_Skill_Fengshen_Evil%d", Prof2Index(iProf));
	PAUIDIALOG pDlgMiddle = GetGameUIMan()->GetDialog(strName);
	if (!pDlgMiddle)
		return;
	
	m_pSubDMiddle->SetDialog(pDlgMiddle);
}

void CDlgSkillMan::OnCommand_SoulBuddhism(const char * szCommand)
{
	int iProf;
	if( GetData()>0 )
	{
		CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetData());
		if( !pPlayer )
			return;
		iProf = pPlayer->GetProfession();
	}
	else
	{
		iProf = GetHostPlayer()->GetProfession();
	}

	AString strName;
	strName.Format("Win_Skill_Fengshen_Buddhism%d", Prof2Index(iProf));
	PAUIDIALOG pDlgMiddle = GetGameUIMan()->GetDialog(strName);
	if (!pDlgMiddle)
		return;
	
	m_pSubDMiddle->SetDialog(pDlgMiddle);
}

void CDlgSkillMan::OnCommand_SkillSeq(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_SkillContinual");
	pDlg->Show(!pDlg->IsShow());
	pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
}

void CDlgSkillMan::OnCommand_RdoType(const char * szCommand)
{
	int index = atoi(szCommand + strlen("RdoMain_"));
	m_pRdo_Desc->Show(false);
	m_pBtn_Talent->Show(false);
	m_pRdo_Skilltree->Show(false);
	
	m_pRdo_Taoism->Show(false);
	m_pRdo_Evil->Show(false);
	m_pRdo_Buddhism->Show(false);
	
	m_pRdo_Couple->Show(false);
	m_pRdo_Teacher->Show(false);
	
	m_pRdo_Talisman->Show(false);
	m_pRdo_Edit->Show(false);

	m_pRdo_SoulTaoism->Show(false);
	m_pRdo_SoulEvil->Show(false);
	m_pRdo_SoulBuddhism->Show(false);

	GetDlgItem("Lab_Hint1")->Show(false);
	GetDlgItem("Lab_Hint2")->Show(false);

	m_pSubDLeft->SetDialog(NULL);
	m_pSubDRight->SetDialog(NULL);
	m_pSubDMiddle->SetDialog(NULL);
	GetGameUIMan()->GetDialog("Win_SkillSpec")->Show(false);
	switch (index)
	{
	case 1:
		{
			OnCommand_desc("");
			CheckRadioButton(2,1);
			
			m_pRdo_Desc->Show(true);
			m_pBtn_Talent->Show(true);
			m_pRdo_Skilltree->Show(true);
			break;
		}
		
	case 2:
		{
			int	iCultivation;
			
			if( GetData()>0 )
			{
				CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetData());
				if( !pPlayer )
					return;
				iCultivation = pPlayer->GetCultivation();
			}
			else
			{
				iCultivation = GetHostPlayer()->GetCultivation();
			}
			if (iCultivation==2)
			{
				OnCommand_evil("");
				CheckRadioButton(3,2);
			}
			else if (iCultivation==4)
			{
				OnCommand_buddhism("");
				CheckRadioButton(3,3);
			}
			else
			{
				OnCommand_taoism("");
				CheckRadioButton(3,1);
			}
			GetDlgItem("Lab_Hint1")->Show(iCultivation==0);
			m_pRdo_Taoism->Show(true);
			m_pRdo_Evil->Show(true);
			m_pRdo_Buddhism->Show(true);
			break;
		}
		
	case 3:
		{
			OnCommand_couple("");
			CheckRadioButton(4,1);
			m_pRdo_Couple->Show(true);
			m_pRdo_Teacher->Show(true);
			break;
		}
		
	case 4:
		{
			OnCommand_talisman("");
			CheckRadioButton(5,1);
			m_pRdo_Talisman->Show(true);
			m_pRdo_Edit->Show(true);
			break;
		}
	case 5:
		{
			int iCult;
			if( GetData()>0 )
			{
				CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetData());
				if( !pPlayer )
					return;
				iCult = pPlayer->GetCultivation();
			}
			else
			{
				iCult = GetHostPlayer()->GetCultivation();
			}
			if (iCult==1)
			{
				OnCommand_SoulTaoism("");
				CheckRadioButton(6,1);
			}
			else if (iCult==2)
			{
				OnCommand_SoulEvil("");
				CheckRadioButton(6,2);
			}
			else if (iCult==4)
			{
				OnCommand_SoulBuddhism("");
				CheckRadioButton(6,3);
			}
			GetDlgItem("Lab_Hint2")->Show(iCult==0||GetHostPlayer()->GetBasicProps().iDeityLevel==0);
			m_pRdo_SoulTaoism->Show(true);
			m_pRdo_SoulEvil->Show(true);
			m_pRdo_SoulBuddhism->Show(true);
			break;
		}
	}
}