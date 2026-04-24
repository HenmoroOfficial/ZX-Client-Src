/********************************************************************
	created:	2005/08/15
	created:	15:8:2005   15:04
	file name:	DlgSkillTree.cpp
	author:		yaojun
	
	purpose:	
*********************************************************************/

#include "AFI.h"
#include "DlgSkillTree.h"
#include "DlgSkill.h"
#include "DlgSkillMan.h"
#include "DlgSkillEdit1.h"
#include "DlgCharacter.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUICTranslate.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Skill.h"
#include "EC_World.h"
#include "EC_GameRun.h"
#include "EC_ManPlayer.h"
#include "EC_GameUIMisc.h"
#include "EC_HelpGraphGuide.h"
#include "LuaEvent.h"
#include "EC_NewHelpGuide.h"

const int PreProfession[] = {
	-1, 0, 1, 2, 0, 4, 5, 0, 7, 8, 0, 10, 11, 3, 13, 14, 6, 16, 17, 9, 19, 20, 12, 22, 23, 0, 25, 26, 27, 28,-1,-1,-1,-1,33,34,35,36,-1,-1,39,40,41,42,
//   0  1  2  3  4  5  6  7  8  9 10  11  12  13 14  15 16  17  18  19 20  21  22  23  24  25 26  27  28  29 30 31 32 33 34 35 36 37 38 39 40 41 42 43,
	-1,-1,45,46,47,48,-1,-1,51,52,53,54,-1,56,57,58,59,-1,-1,-1, 0,64,65,66,67,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
//	44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90
	-1,-1,-1,-1,-1,-1,96,97,98, 99, -1, -1, 102, 103, 104, 105, -1, -1, 108, 109, 110, 111
//	91 92 93 94 95 96 97 98 99 100  101 102 103  104  105  106  107 108 109  110  111  112
};

const int talent[] = {
	1, 4, 4, 4, 10, 10, 10, 1, 1, 1, 7, 7, 7, 4, 4, 4, 10, 10, 10, 1, 1, 1, 7, 7, 7, 13, 13, 13, 13, 13, 0,0, 0, 16,16,16,16,16,0, 20,20,20,20,20,
//  0  1  2  3  4   5   6   7  8  9  10 11 12 13 14 15 16  17  18  19 20 21 22 23 24 25  26  27  28  29 30 31 32 33 34 35 36 37 38 39 40 41 42 43
	0, 22,22,22,22,22,0, 25,25,25,25,25,34,34,34,34,34, 0, 0, 0,31,31,31,31,31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90
	0,  0, 0, 0, 0,28,28,28,28, 28, 0,  37, 37, 37, 37, 37, 0, 40, 40, 40, 40, 40,
//	91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112
};

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSkillTree, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)
AUI_ON_COMMAND("Btn_Next",		OnCommandFortNext)
AUI_ON_COMMAND("Btn_Prev",		OnCommandFortPrev)
AUI_ON_COMMAND("Btn_LMove",		OnCommandLeft)
AUI_ON_COMMAND("Btn_RMove",		OnCommandRight)
AUI_ON_COMMAND("Btn_Combo_*",	OnCommandComboEdit)

AUI_ON_COMMAND("Btn_ToProfTree",OnCommandChangeProfTree)
AUI_ON_COMMAND("Btn_ToProfTree2",OnCommandChangeProfTree2)
AUI_ON_COMMAND("Btn_ToProfTree3",OnCommandChangeProfTree3)

AUI_ON_COMMAND("Btn_Clean",		OnCommandResetProp)
AUI_ON_COMMAND("Btn_*",			OnCommandAdd)
AUI_ON_COMMAND("Rdo_*",			OnCommandRdoRace)
AUI_ON_COMMAND("tree",			OnCommandTree)
AUI_ON_COMMAND("talent*",		OnCommandTalent)
AUI_ON_COMMAND("power*",		OnCommandfortune)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgSkillTree, CDlgBase)

AUI_ON_EVENT("prof_*",		WM_LBUTTONDOWN,		OnEventLButtonDown_Prof)
AUI_ON_EVENT("prof_*",		WM_LBUTTONDBLCLK,	OnEventLButtonDown_Prof)
AUI_ON_EVENT("skill_*",		WM_LBUTTONDOWN,		OnEventLButtonDown_Skill)
AUI_ON_EVENT("skill_*",		WM_RBUTTONDOWN,		OnEventRButtonDown_Skill)

AUI_END_EVENT_MAP()

CDlgSkillTree* CDlgSkillTree::m_pSkillTreeActive = NULL;
CDlgSkillTree* CDlgSkillTree::m_pTalentTreeActive = NULL;
CDlgSkillTree* CDlgSkillTree::m_pFortuneActive = NULL;

CDlgSkillTree::CDlgSkillTree()
{
	m_pSkillTreeActive = NULL;
	m_pTalentTreeActive = NULL;
	m_pFortuneActive	= NULL;
}

CDlgSkillTree::~CDlgSkillTree()
{

}

bool CDlgSkillTree::OnInitDialog()
{
	m_pTxt_SkillPoint = (PAUILABEL)GetDlgItem("Lab_SkillPoint");
	m_pTxt_AddedSkillPoint = (PAUILABEL)GetDlgItem("Lab_AddedSkillPoint");
	m_pBtn_LMove = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_LMove");
	m_pBtn_RMove = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_RMove");
	
	if (m_szName=="Win_Skill00" ||
		m_szName=="Win_Skill33" ||
		m_szName=="Win_Skill39" ||
		m_szName=="Win_Skill45" ||
		m_szName=="Win_Skill51" ||
		m_szName=="Win_Skill56" ||
		m_szName=="Win_Skill96" )
	{
		if(GetDlgItem("Shape_1")) GetDlgItem("Shape_1")->Show(false);
		if(GetDlgItem("Img_1")) GetDlgItem("Img_1")->Show(false);
		if(GetDlgItem("Txt_1")) GetDlgItem("Txt_1")->Show(false);
	}

	return true;
}

bool CDlgSkillTree::Release()
{
	PAUIOBJECTLISTELEMENT pElement = GetFirstControl();
	while( pElement )
	{
		if( strstr(pElement->pThis->GetName(), "Skill_") )
		{
			int id = atoi(pElement->pThis->GetName() + strlen("Skill_"));
			PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(pElement->pThis);
			if( id == 0 )
			{
				pElement = pElement->pNext;
				continue;
			}
			if (pImage->GetData64("is_triggered"))
			{
				CECSkill* pSkill = (CECSkill*)pImage->GetDataPtr("ptr_CECSkill");
				if (pSkill->IsCategory(GNET::CATEGORY_TRIGGERED))
				{
					delete pSkill;
					pSkill = NULL;
					pImage->SetDataPtr(NULL, "ptr_CECSkill");
				}
			}
		}
		pElement = pElement->pNext;
	}
	return CDlgBase::Release();
}

void CDlgSkillTree::OnShowDialog()
{
	if( stricmp(GetName(), "Win_SkillProfTree") == 0 )
	{
// 		if( m_pTalentTreeActive && m_pTalentTreeActive->IsShow() )
// 			m_pTalentTreeActive->Show(false);
		m_pTalentTreeActive = this;
	}
	else if( strstr(GetName(), "Win_Talent_") )
	{
// 		if( m_pTalentTreeActive && m_pTalentTreeActive->IsShow() )
// 			m_pTalentTreeActive->Show(false);
		m_pTalentTreeActive = this;
		CheckRadioButton(1, atoi(GetName() + strlen("Win_Talent_") ));
	}
	else if( strstr(GetName(), "Win_SkillCouple") )
	{
	}
	else if( strstr(GetName(), "Win_Skill_Taoism2") )
	{
		const char *szName = GetName();
		int id = atoi(szName+strlen("Win_Skill_Taoism2_"));
		CheckRadioButton(1,id);
		if(id == 13 || id == 14)
			CheckRadioButton(2, 3);
		else if((id >= 1 && id <= 5) || id == 8)
			CheckRadioButton(2, 1);
		else if(id == 6 || id == 7 || (id >= 9 && id <= 12) )
			CheckRadioButton(2, 2);
	}
	else if( strstr(GetName(), "Win_Skill_Evil2") )
	{
		const char *szName = GetName();
		int id = atoi(szName+strlen("Win_Skill_Evil2_"));
		CheckRadioButton(1,id);
		if(id == 13 || id == 14)
			CheckRadioButton(2, 3);
		else if((id >= 1 && id <= 5) || id == 8)
			CheckRadioButton(2, 1);
		else if(id == 6 || id == 7 || (id >= 9 && id <= 12) )
			CheckRadioButton(2, 2);
	}
	else if( strstr(GetName(), "Win_Skill_Buddhism2") )
	{
		const char *szName = GetName();
		int id = atoi(szName+strlen("Win_Skill_Buddhism2_"));
		CheckRadioButton(1,id);
		if(id == 13 || id == 14)
			CheckRadioButton(2, 3);
		else if((id >= 1 && id <= 5) || id == 8)
			CheckRadioButton(2, 1);
		else if(id == 6 || id == 7 || (id >= 9 && id <= 12) )
			CheckRadioButton(2, 2);
	}
	else if( strstr(GetName(), "Win_SkillProfTree") )
	{
	}
	else if( strstr(GetName(), "Win_Skill_Fengshen") )
	{
	}
	else if( strstr(GetName(), "Win_Skill") )
	{
		if( m_pSkillTreeActive && m_pSkillTreeActive->IsShow() && m_pSkillTreeActive!=this)
			m_pSkillTreeActive->Show(false);
		m_pSkillTreeActive = this;
		int idProf;
		if( GetGameUIMan()->GetDialog("Win_Skillzx2")->GetData()>0 )
		{
			CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetGameUIMan()->GetDialog("Win_Skillzx2")->GetData());
			if( !pPlayer )
				return;
			idProf = pPlayer->GetProfession();
		}
		else
			idProf = GetHostPlayer()->GetProfession();
		int nProf = idProf;
		bool bHostProf = false;
		while( nProf >= 0 )
		{
			if( nProf == atoi(GetName() + strlen("Win_Skill")) )
			{
				bHostProf = true;
				break;
			}
			nProf = PreProfession[nProf];
		}
		if( !bHostProf )
		{
			m_pBtn_LMove->Show(false);
			m_pBtn_RMove->Show(false);
		}
		else
		{
			m_pBtn_LMove->Show(nProf > 0 && nProf!=33 && nProf!= 39 && nProf!= 45 && nProf!= 51 && nProf!= 56 && nProf!= 96 && nProf != 102 && nProf != 108);
			m_pBtn_RMove->Show(nProf < idProf);
		}
	}
	else if( strstr(GetName(), "Win_Taoism") || 
			strstr(GetName(), "Win_Evil") || 
			strstr(GetName(), "Win_Buddhism"))
	{
// 		if( m_pFortuneActive && m_pFortuneActive->IsShow() )
// 			m_pFortuneActive->Show(false);
// 		m_pFortuneActive = this;

		PAUILABEL pLabel;
		PAUIPROGRESS pPrgs;
		const int *lvl = regionreputationlevel;
		int level = 0;
		int nBase = 1140;
		int nNowReputation;
		int nMaxReputation;
		int nReputation;
		ACHAR szText[100];
		if (strstr(GetName(), "Win_Buddhism"))
		{
			CheckRadioButton(1, 3 );
			pPrgs= (PAUIPROGRESS)GetDlgItem("Prgs_Buddhism");
			pLabel = (PAUILABEL)GetDlgItem("Txt_BuddhismTitle");
			nReputation = GetHostPlayer()->GetRegionReputation(11);
			while( nReputation >= lvl[level + 1] )
				level++;
			nNowReputation = nReputation - lvl[level];
			nMaxReputation = lvl[level + 1] - lvl[level];
			pPrgs->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
			pLabel->SetText(GetStringFromTable(nBase + level));
			pLabel->SetAlign(AUIFRAME_ALIGN_CENTER);
			a_sprintf(szText, _AL("%d/%d"), nReputation, lvl[level + 1]);
			pPrgs->SetHint(szText);
		}
		else if (strstr(GetName(), "Win_Evil"))
		{
			CheckRadioButton(1, 2 );
			pPrgs	= (PAUIPROGRESS)GetDlgItem("Prgs_Evil");
			pLabel = (PAUILABEL)GetDlgItem("Txt_EvilTitle");

			nReputation = GetHostPlayer()->GetRegionReputation(10);
			while( nReputation >= lvl[level + 1] )
				level++;
			nNowReputation = nReputation - lvl[level];
			nMaxReputation = lvl[level + 1] - lvl[level];
			pPrgs->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
			pLabel->SetText(GetStringFromTable(nBase + level));
			pLabel->SetAlign(AUIFRAME_ALIGN_CENTER);
			a_sprintf(szText, _AL("%d/%d"), nReputation, lvl[level + 1]);
			pPrgs->SetHint(szText);
		}
		else if (strstr(GetName(), "Win_Taoism"))
		{
			CheckRadioButton(1, 1 );
			pPrgs	= (PAUIPROGRESS)GetDlgItem("Prgs_Taoism");
			pLabel = (PAUILABEL)GetDlgItem("Txt_TaoismTitle");
			nReputation = GetHostPlayer()->GetRegionReputation(9);
			while( nReputation >= lvl[level + 1] )
				level++;
			nNowReputation = nReputation - lvl[level];
			nMaxReputation = lvl[level + 1] - lvl[level];
			pPrgs->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
			pLabel->SetText(GetStringFromTable(nBase + level));
			pLabel->SetAlign(AUIFRAME_ALIGN_CENTER);
			a_sprintf(szText, _AL("%d/%d"), nReputation, lvl[level + 1]);
			pPrgs->SetHint(szText);
		}
		else if (strstr(GetName(), "Win_Couple"))
		{
			pLabel = (PAUILABEL)GetDlgItem("Lab_LoveNo");
			pLabel->SetAlign(AUIFRAME_ALIGN_CENTER);
		}
	}
}

void CDlgSkillTree::OnTick()
{
	if( stricmp(GetName(), "Win_SkillProfTree") == 0 )
	{
		PAUIOBJECTLISTELEMENT pElement = GetFirstControl();
	
		while( pElement )
		{
			if( strstr(pElement->pThis->GetName(), "Prof_") )
			{
				pElement->pThis->SetHint(GetStringFromTable(PROFESSION_START + 
					atoi(pElement->pThis->GetName() + strlen("Prof_"))));
			}
			pElement = pElement->pNext;
		}

		return;
	}

	ACString strText;
	PAUIOBJECTLISTELEMENT pElement = GetFirstControl();
	
	ROLEBASICPROP &rbp = GetHostPlayer()->GetBasicProps();
	if( strstr(GetName(), "Win_Talent_") )
	{
		strText.Format(GetStringFromTable(921), GetHostPlayer()->GetTalentValue());
		if( GetGameUIMan()->GetDialog("Win_Skillzx2")->GetData()>0 )
			m_pTxt_SkillPoint->SetText(_AL(""));
		else
			m_pTxt_SkillPoint->SetText(strText);
	}
	else if( strstr(GetName(), "Win_Teacher") )
	{
		{
			PAUIPROGRESS pProgress = (PAUIPROGRESS)GetDlgItem("Pro_Reputation");
			int level = 0;
			int reputation = GetHostPlayer()->GetRegionReputation(8);
			while( reputation >= teacherlevel[level + 1] )
				level++;
			GetDlgItem("Txt_Reputation")->SetText(GetStringFromTable(1140 + level));
			int nNowReputation = reputation - teacherlevel[level];
			int nMaxReputation = teacherlevel[level + 1] - teacherlevel[level];
			pProgress->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
			strText.Format(_AL("%d/%d"), reputation, teacherlevel[level + 1]);
			pProgress->SetHint(strText);
		}
	}
	else if( strstr(GetName(), "Win_Skill_Taoism2") ||
		 strstr(GetName(), "Win_Taoism") || 
		strstr(GetName(), "Win_Evil") || 
		strstr(GetName(), "Win_Buddhism") || 
		strstr(GetName(), "Win_Skill_Taoism") || 
		strstr(GetName(), "Win_Skill_Evil") || 
		strstr(GetName(), "Win_Skill_Buddhism") || 
		strstr(GetName(), "Win_Couple") ||
		strstr(GetName(), "Win_Skill_Evil2") ||
		strstr(GetName(), "Win_Skill_Buddhism2") )
	{
		m_pFortuneActive = this;
		
		PAUILABEL pLabel;
		PAUIPROGRESS pPrgs;
		const int *lvl = regionreputationlevel;
		int level = 0;
		int nBase = 1140;
		int nNowReputation;
		int nMaxReputation;
		int nReputation;
		ACHAR szText[100];
		if (stricmp(GetName(), "Win_Buddhism")==0)
		{
			pPrgs= (PAUIPROGRESS)GetDlgItem("Prgs_Buddhism");
			pLabel = (PAUILABEL)GetDlgItem("Txt_BuddhismTitle");
			nReputation = GetHostPlayer()->GetRegionReputation(11);
			while( nReputation >= lvl[level + 1] )
				level++;
			nNowReputation = nReputation - lvl[level];
			nMaxReputation = lvl[level + 1] - lvl[level];
			pPrgs->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
			pLabel->SetText(GetStringFromTable(nBase + level));
			a_sprintf(szText, _AL("%d/%d"), nReputation, lvl[level + 1]);
			pPrgs->SetHint(szText);
		}
		else if (stricmp(GetName(), "Win_Evil")==0)
		{
			pPrgs	= (PAUIPROGRESS)GetDlgItem("Prgs_Evil");
			pLabel = (PAUILABEL)GetDlgItem("Txt_EvilTitle");
			
			nReputation = GetHostPlayer()->GetRegionReputation(10);
			while( nReputation >= lvl[level + 1] )
				level++;
			nNowReputation = nReputation - lvl[level];
			nMaxReputation = lvl[level + 1] - lvl[level];
			pPrgs->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
			pLabel->SetText(GetStringFromTable(nBase + level));
			a_sprintf(szText, _AL("%d/%d"), nReputation, lvl[level + 1]);
			pPrgs->SetHint(szText);
		}
		else if (stricmp(GetName(), "Win_Taoism")==0)
		{
			pPrgs	= (PAUIPROGRESS)GetDlgItem("Prgs_Taoism");
			pLabel = (PAUILABEL)GetDlgItem("Txt_TaoismTitle");
			nReputation = GetHostPlayer()->GetRegionReputation(9);
			while( nReputation >= lvl[level + 1] )
				level++;
			nNowReputation = nReputation - lvl[level];
			nMaxReputation = lvl[level + 1] - lvl[level];
			pPrgs->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
			pLabel->SetText(GetStringFromTable(nBase + level));
			a_sprintf(szText, _AL("%d/%d"), nReputation, lvl[level + 1]);
			pPrgs->SetHint(szText);
		}
		else if (strstr(GetName(), "Win_Couple"))
		{
			nBase = 1160;
			const int *lv2 = affectionlevel;
			pPrgs	= (PAUIPROGRESS)GetDlgItem("Pro_Love");
			pLabel = (PAUILABEL)GetDlgItem("Lab_LoveNo");
			nReputation = GetHostPlayer()->GetRegionReputation(6);
			while( nReputation >= lv2[level + 1] )
				level++;
			nNowReputation = nReputation - lv2[level];
			nMaxReputation = lv2[level + 1] - lv2[level];
			pPrgs->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
			pLabel->SetText(GetStringFromTable(nBase + level));
			a_sprintf(szText, _AL("%d/%d"), nReputation, lv2[level + 1]);
			pPrgs->SetHint(szText);
		}
	}
	else if( strstr(GetName(), "Win_SkillProfTree") )
	{
	}
	else if( strstr(GetName(), "Win_Skill_Fengshen") )
	{
		PAUILABEL pLabExp = (PAUILABEL)GetDlgItem("Txt_Exp");
		strText.Format(_AL("%.0f"), rbp.deityExp);
		pLabExp->SetText(strText);
	}
	else if( strstr(GetName(), "Win_Skill") )
	{
		strText.Format(GetStringFromTable(764), rbp.iStatusPt);
		if( GetGameUIMan()->GetDialog("Win_Skillzx2")->GetData()>0 )
			m_pTxt_SkillPoint->SetText(_AL(""));
		else
			m_pTxt_SkillPoint->SetText(strText);
	}


	int nAddedPoint = 0;

	int nTalentSize = 0;
	short idTalent[10];
	if( CDlgSkill::GetSelectSkill() )
	{
		CECSkill *pSkill = new CECSkill(CDlgSkill::GetSelectSkill(), 1);
		nTalentSize = pSkill->GetTalentSize();
		for(int i = 0; i < nTalentSize; i++ )
			idTalent[i] = pSkill->GetTalent()[i];
		delete pSkill;
	}

	while( pElement )
	{
		if( strstr(pElement->pThis->GetName(), "Skill_") )
		{
			int id = atoi(pElement->pThis->GetName() + strlen("Skill_"));
			PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(pElement->pThis);
			if( id == 0 )
			{
				pImage->ClearCover();
				pImage->SetData(0, "IsRound");
				pImage->SetDataPtr(NULL);
				pElement = pElement->pNext;
				continue;
			}
			
			CECSkill* pSkill = NULL;
			if( GetGameUIMan()->GetDialog("Win_Skillzx2")->GetData()>0 )
				pSkill = GetHostPlayer()->GetElsePlayerSkillByID(id);
			else
				pSkill = GetHostPlayer()->GetSkill(id);
			bool bNewSkill = false;
			if( !pSkill || pSkill->GetSkillLevel() == 0 )
			{
				pSkill = new CECSkill(id, 0);
				bNewSkill = true;
				strText.Format(_AL("%d"), 0);
				if(pSkill->IsCategory(GNET::CATEGORY_TRIGGERED))
				{
					CECSkill* pSkillReady = (CECSkill*)pImage->GetDataPtr("ptr_CECSkill");
					if (pSkillReady)
					{
						delete pSkill;
						pSkill = pSkillReady;
					}
					GetGameUIMan()->m_pDlgSkill->SetImage(pImage, pSkill, true);
					pImage->SetData64(999, "is_triggered");
				}
				else
				{					
					pImage->SetData(0, "IsRound");
					pImage->SetDataPtr(NULL);
					pImage->SetColor(A3DCOLORRGB(128, 128, 128));
				}
			}
			else
			{
				CECSkill* pSkillData = (CECSkill*)pImage->GetDataPtr("ptr_CECSkill");
				nAddedPoint += pSkill->GetSkillLevel();
				strText.Format(_AL("%d"), pSkill->GetSkillLevel());
				bool bPositive = pSkill->GetType() != CECSkill::TYPE_PASSIVE && pSkill->GetType() != CECSkill::TYPE_LIVE;
				pImage->SetColor(A3DCOLORRGB(255, 255, 255));
				GetGameUIMan()->m_pDlgSkill->SetImage(pImage, pSkill, bPositive);
				if( !bPositive )
					pImage->SetDataPtr(NULL);
				else if (pSkill->IsCategory(GNET::CATEGORY_TRIGGERED))
				{
					if(pSkillData)
						pImage->SetDataPtr(pSkillData, "ptr_CECSkill");
					else
						pImage->SetDataPtr(new CECSkill(pSkill->GetSkillID(), pSkill->GetSkillLevel()), "ptr_CECSkill");
					pImage->SetData64(999, "is_triggered");
				}
			}
			pImage->SetText(strText);
			int nCondition = GetHostPlayer()->CheckSkillLearnCondition(id, true);
			char szName[20];
			sprintf(szName, "Btn_%02d", id);
			if( GetDlgItem(szName) )
			{
				if( GetGameUIMan()->GetDialog("Win_Skillzx2")->GetData()>0 )
					GetDlgItem(szName)->Show(false);
				else if (GetHostPlayer()->GetMaster()!=GetHostPlayer()->GetCharacterID() && m_szName == "Win_Teacher")
					GetDlgItem(szName)->Show(false);
				else
				{
					if (pSkill->IsUseProficiency())
						GetDlgItem(szName)->Show(nCondition == 0);
					else if( pSkill->IsCategory(GNET::CATEGORY_TALENT) )
						GetDlgItem(szName)->Show(GetHostPlayer()->GetTalentValue() > 0 && nCondition == 0);
					else if( pSkill->IsCategory(GNET::CATEGORY_MYSTERY) || pSkill->IsCategory(GNET::CATEGORY_SPOUSE) 
						|| pSkill->IsCategory(GNET::CATEGORY_SECT) || pSkill->IsCategory(GNET::CATEGORY_DEITY))
						GetDlgItem(szName)->Show( nCondition == 0);
					else
						GetDlgItem(szName)->Show(rbp.iStatusPt > 0 && nCondition == 0);
				}
			}
			if( pSkill )
			{
				if( bNewSkill )
				{
					AString strFile, strFileRound;
					af_GetFileTitle(pSkill->GetIconFile(), strFile);
					strFile.MakeLower();
					
					//技能圆形图标后缀
					if (strstr(GetName(), "Win_Skill_Fengshen") && strFile.Right(4)==".dds")
					{
						strFileRound = strFile;
						strFileRound.CutRight(4);
						strFileRound += "_0.dds";
						if (GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_SKILL, strFileRound)>=0)
						{
							strFile = strFileRound;
							pImage->SetCover(CDlgSkill::m_pA2DSpriteSkillRound3D, 0, 2);
							pImage->SetData(999, "IsRound");
						}
					}
					pImage->SetCover(
						GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILL],
						GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_SKILL][strFile]);
					const wchar_t* szDesc = pSkill->GetDesc();
					AUICTranslate trans;
					if( szDesc )
						pImage->SetHint(trans.Translate(szDesc));
					else
						pImage->SetHint(_AL(""));
				}
				if( pSkill->GetSkillID() == CDlgSkill::GetSelectSkill() )
					pImage->SetCover(CDlgSkill::GetA2DSpriteSkillSelect(pImage->GetData("IsRound")>0), (GetTickCount() / 200) % 7, 1);
				else
				{
					bool b = false;
					int i;
					for(i = 0; i < nTalentSize; i++)
						if( idTalent[i] == pSkill->GetSkillID() )
							b = true;
					for(i = 0; i < pSkill->GetTalentSize(); i++)
						if( pSkill->GetTalent()[i] == CDlgSkill::GetSelectSkill() )
							b = true;
					if( b )
						pImage->SetCover(CDlgSkill::GetA2DSpriteSkillHilight(pImage->GetData("IsRound")>0), (GetTickCount() / 200) % 6, 1);
					else
						pImage->SetCover(NULL, 0, 1);
				}
				if( bNewSkill && !pSkill->IsCategory(GNET::CATEGORY_TRIGGERED) )
				{
					delete pSkill;
					pSkill = NULL;
				}
			}
			else
			{
				pImage->ClearCover();
				pImage->SetText(_AL(""));
				pImage->SetHint(_AL(""));
			}
		}
		
		pElement = pElement->pNext;
	}
	if( strstr(GetName(), "Win_Skill") && m_pTxt_AddedSkillPoint)
	{
		strText.Format(GetStringFromTable(1003), nAddedPoint);
		m_pTxt_AddedSkillPoint->SetText(strText);
	}
	if (GetDlgItem("Btn_Clean"))
	{
		GetDlgItem("Btn_Clean")->Show(GetHostPlayer()->GetBasicProps().iLevel<=90);
	}
}

bool CDlgSkillTree::Render()
{
	return CDlgBase::Render();
}

void CDlgSkillTree::OnCommandAdd(const char * szCommand)
{
	int id = atoi(szCommand + strlen("Btn_"));
	int nCondition = GetHostPlayer()->CheckSkillLearnCondition(id, true);
	if( nCondition == 0 )
	{
		GetGameSession()->c2s_CmdLearnSkill(id);
		LuaEvent::FirstAddSkillPoint();
		GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_9_2_SKILL_ADD);
		if(id == 218  || 
		   id == 1843 ||
		   id == 1883 ||
		   id == 2411 ||
		   id == 2459 ||
		   id == 3997 ||
		   id == 3455 ||
		   id == 4771 ||
		   id == 4838)
		NewHelpGuide::StartGuide_SkillAddPoint_3();
	}
}

void CDlgSkillTree::OnCommandRdoRace(const char * szCommand)
{
	int race;
	AString strName;
	PAUIDIALOG pDlg;

	if(strstr(szCommand, "Race"))
	{
		int type = atoi(szCommand + strlen("Rdo_Race0"));
		if(type == 1)
			race = 1;
		else if(type == 2)
			race = 6;
		else if(type == 3)
			race = 13;
		else 
			return;
	}
	else
	{
		race = atoi(szCommand + strlen("Rdo_"));
	}

	if (strstr(m_szName, "Win_Skill_Buddhism2"))
		strName.Format("Win_Skill_Buddhism2_%d", race);
	else if (strstr(m_szName, "Win_Skill_Evil2"))
		strName.Format("Win_Skill_Evil2_%d", race);
	else if (strstr(m_szName, "Win_Skill_Taoism2"))
		strName.Format("Win_Skill_Taoism2_%d", race);
	pDlg = GetGameUIMan()->GetDialog(strName);
	if (GetGameUIMan()->m_pDlgSkillMan->GetDialogRight() == this)
	{
		GetGameUIMan()->m_pDlgSkillMan->SetDialogRight(pDlg);
	}
	Show(false);
	if (pDlg)
		pDlg->Show(true);
}

void CDlgSkillTree::OnCommandTree(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_SkillProfTree");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgSkillTree::OnCommandTalent(const char * szCommand)
{
	int idProf;
	if( GetGameUIMan()->GetDialog("Win_Skillzx2")->GetData()>0 )
	{
		CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetGameUIMan()->GetDialog("Win_Skillzx2")->GetData());
		if( !pPlayer )
			return;
		idProf = pPlayer->GetProfession();
	}
	else
		idProf = GetHostPlayer()->GetProfession();
	int idTalent = atoi(szCommand + strlen("talent"));
	if( idTalent == 0 )
		idTalent = talent[idProf]; 
	char szName[30];
	sprintf(szName, "Win_Talent_%02d", idTalent);
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog(szName);
	if( pDlg && pDlg != this )
	{
		if (GetGameUIMan()->m_pDlgSkillMan->GetDialogRight() == this)
		{
			GetGameUIMan()->m_pDlgSkillMan->SetDialogRight(pDlg);
		}
		Show(false);
		pDlg->Show(true);
		pDlg->AlignTo(this, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
				AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
	}
}

void CDlgSkillTree::OnCommandfortune(const char * szCommand)
{
	int idProf;
	if( GetGameUIMan()->GetDialog("Win_Skillzx2")->GetData()>0 )
	{
		CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetGameUIMan()->GetDialog("Win_Skillzx2")->GetData());
		if( !pPlayer )
			return;
		idProf = pPlayer->GetProfession();
	}
	else
		idProf = GetHostPlayer()->GetProfession();
	int id = atoi(szCommand + strlen("power"));
	PAUIDIALOG pDlg;
	if (3==id)
	{
		pDlg = GetGameUIMan()->GetDialog("Win_Buddhism");
	}
	else if (2==id)
	{
		pDlg = GetGameUIMan()->GetDialog("Win_Evil");
	}
	else if (1==id)
	{
		pDlg = GetGameUIMan()->GetDialog("Win_Taoism");
	}
	if( pDlg && pDlg != this )
		pDlg->Show(!pDlg->IsShow());
}

void CDlgSkillTree::OnCommandResetProp(const char * szCommand)
{
	if (GetHostPlayer()->GetBasicProps().iLevel>90 )
	{
		GetGameUIMan()->MessageBox(15001);
		return;
	}
	if (strstr(m_szName, "Win_Skill"))
	{
		if (GetHostPlayer()->GetCoolTime(COOLDOWN_INDEX_RESET_SKILL_PROP)<=0)
		{
			GetGameUIMan()->MessageBox("Game_ResetSkill", GetGameUIMan()->GetStringFromTable(15004), MB_YESNO);
		}
		else
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(15002)+GetTimeText(GetHostPlayer()->GetCoolTime(COOLDOWN_INDEX_RESET_SKILL_PROP)/1000));
		}
	}
	else
	{
		if (GetHostPlayer()->GetCoolTime(COOLDOWN_INDEX_RESET_SKILL_PROP_TALENT)<=0)
			GetGameUIMan()->MessageBox("Game_ResetTalent", GetGameUIMan()->GetStringFromTable(15005), MB_YESNO);
		else
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(15002)+GetTimeText(GetHostPlayer()->GetCoolTime(COOLDOWN_INDEX_RESET_SKILL_PROP_TALENT)/1000));
		}
	}
}

void CDlgSkillTree::OnCommandChangeProfTree(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_SkillProfTree2");
	GetGameUIMan()->m_pDlgSkillMan->SetDialogLeft(pDlg);

	Show(false);
	pDlg->Show(true);
}

void CDlgSkillTree::OnCommandChangeProfTree2(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_SkillProfTree");
	GetGameUIMan()->m_pDlgSkillMan->SetDialogLeft(pDlg);

	Show(false);
	pDlg->Show(true);
}
void CDlgSkillTree::OnCommandChangeProfTree3(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_SkillProfTree3");
	GetGameUIMan()->m_pDlgSkillMan->SetDialogLeft(pDlg);

	Show(false);
	pDlg->Show(true);
}
void CDlgSkillTree::OnCommandComboEdit(const char * szCommand)
{
	int id = atoi(szCommand + strlen("Btn_Combo_"));
	CECSkill* pSkill = NULL;
	if( GetGameUIMan()->GetDialog("Win_Skillzx2")->GetData()>0 )
		return;

	pSkill = GetHostPlayer()->GetSkill(id);
	bool bNewSkill = false;
	if( !pSkill || pSkill->GetSkillLevel() == 0 )
		return;
	CDlgSkillComboEdit* pDlgSkillComboEdit = (CDlgSkillComboEdit*)GetGameUIMan()->GetDialog("Win_SkillComboEdit");
	pDlgSkillComboEdit->EditSkill(id, pSkill->GetSkillLevel());
}

void CDlgSkillTree::OnCommandFortNext(const char * szCommand)
{
	if (this == GetGameUIMan()->GetDialog("Win_Buddhism"))
	{
		if (GetGameUIMan()->m_pDlgSkillMan->GetDialogRight() == this)
		{
			Show(false);
			GetGameUIMan()->m_pDlgSkillMan->SetDialogRight(NULL);
			GetGameUIMan()->m_pDlgSkillMan->OnCommand_buddhism("");
		}
	}
	else if (this == GetGameUIMan()->GetDialog("Win_Evil"))
	{
		if (GetGameUIMan()->m_pDlgSkillMan->GetDialogRight() == this)
		{
			Show(false);
			GetGameUIMan()->m_pDlgSkillMan->SetDialogRight(NULL);
			GetGameUIMan()->m_pDlgSkillMan->OnCommand_evil("");
		}
	}
	else if (this == GetGameUIMan()->GetDialog("Win_Taoism"))
	{
		if (GetGameUIMan()->m_pDlgSkillMan->GetDialogRight() == this)
		{
			Show(false);
			GetGameUIMan()->m_pDlgSkillMan->SetDialogRight(NULL);
			GetGameUIMan()->m_pDlgSkillMan->OnCommand_taoism("");
		}
	}
}

void CDlgSkillTree::OnCommandFortPrev(const char * szCommand)
{
	PAUIDIALOG pDlg;
	if (strstr(m_szName, "Win_Skill_Buddhism2"))
	{
		pDlg = GetGameUIMan()->GetDialog("Win_Buddhism");
		if (GetGameUIMan()->m_pDlgSkillMan->GetDialogRight() == this)
		{
			GetGameUIMan()->m_pDlgSkillMan->SetDialogRight(pDlg);
		}
		Show(false);
		pDlg->Show(true);
	}
	else if (strstr(m_szName, "Win_Skill_Evil2"))
	{
		pDlg = GetGameUIMan()->GetDialog("Win_Evil");
		if (GetGameUIMan()->m_pDlgSkillMan->GetDialogRight() == this)
		{
			GetGameUIMan()->m_pDlgSkillMan->SetDialogRight(pDlg);
		}
		Show(false);
		pDlg->Show(true);
	}
	else if (strstr(m_szName, "Win_Skill_Taoism2"))
	{
		pDlg = GetGameUIMan()->GetDialog("Win_Taoism");
		if (GetGameUIMan()->m_pDlgSkillMan->GetDialogRight() == this)
		{
			GetGameUIMan()->m_pDlgSkillMan->SetDialogRight(pDlg);
		}
		Show(false);
		pDlg->Show(true);
	}
}

void CDlgSkillTree::OnCommandLeft(const char * szCommand)
{
	int index = atoi(GetName() + strlen("Win_Skill"));
	int idProf = PreProfession[atoi(GetName() + strlen("Win_Skill"))];
	if( idProf >= 0 )
	{
		if (GetGameUIMan()->m_pDlgSkillMan->GetDialogRight() == this)
		{
			GetGameUIMan()->m_pDlgSkillMan->SetDialogRight(GetGameUIMan()->m_pDlgSkillTree[idProf]);
		}
		Show(false);
		GetGameUIMan()->m_pDlgSkillTree[idProf]->Show(true);
	}
}

void CDlgSkillTree::OnCommandRight(const char * szCommand)
{
	int nProf = atoi(GetName() + strlen("Win_Skill"));
	int idProf;
	if( GetGameUIMan()->GetDialog("Win_Skillzx2")->GetData()>0 )
	{
		CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(GetGameUIMan()->GetDialog("Win_Skillzx2")->GetData());
		if( !pPlayer )
			return;
		idProf = pPlayer->GetProfession();
	}
	else
		idProf = GetHostPlayer()->GetProfession();
	bool bHostProf = false;
	while( idProf > 0 )
	{
		if( PreProfession[idProf] == nProf )
		{
			if (GetGameUIMan()->m_pDlgSkillMan->GetDialogRight() == this)
			{
				GetGameUIMan()->m_pDlgSkillMan->SetDialogRight(GetGameUIMan()->m_pDlgSkillTree[idProf]);
			}
			Show(false);
			GetGameUIMan()->m_pDlgSkillTree[idProf]->Show(true);
			break;
		}
		idProf = PreProfession[idProf];
	}
}
void CDlgSkillTree::OnCommandCancel(const char * szCommand)
{
	if (strstr(GetName(), "Win_Talent_"))
		Show(false);
	else
		GetGameUIMan()->m_pDlgSkillMan->OnCommand_CANCEL("");
}

void CDlgSkillTree::OnEventLButtonDown_Prof(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int idProf = atoi(pObj->GetName() + strlen("Prof_"));
	if (GetGameUIMan()->m_pDlgSkillMan->GetDialogRight())
	{
		GetGameUIMan()->m_pDlgSkillMan->GetDialogRight()->Show(false);
		GetGameUIMan()->m_pDlgSkillMan->SetDialogRight(GetGameUIMan()->m_pDlgSkillTree[idProf]);
	}
	GetGameUIMan()->m_pDlgSkillTree[idProf]->Show(true);
}

void CDlgSkillTree::OnEventLButtonDown_Skill(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( GetGameUIMan()->GetDialog("Win_Skillzx2")->GetData()>0 )
		return;
	
	if( !pObj->GetDataPtr("ptr_CECSkill") )
		return;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT pt = 
	{
		GET_X_LPARAM(lParam) - p->X,
		GET_Y_LPARAM(lParam) - p->Y,
	};
	
	GetGameUIMan()->m_ptLButtonDown = pt;
	GetGameUIMan()->InvokeDragDrop(this, pObj, pt);
}

void CDlgSkillTree::OnEventRButtonDown_Skill(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	CDlgSkill::SelectSkill(atoi(pObj->GetName() + strlen("skill_")));
}
