// File		: DlgTeach.cpp
// Creator	: Xiao Zhou
// Date		: 2005/8/16

#include "AFI.h"
#include "AUI\\AUIImagePicture.h"
#include "A2DSprite.h"
#include "DlgTeach.h"
#include "DlgSkillTree.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_NPC.h"
#include "EC_ManNPC.h"
#include "EC_NPCServer.h"
#include "EC_Skill.h"
#include "EC_World.h"
#include "EC_Global.h"
#include "EC_Game.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgTeach, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",	OnCommandCANCAL)
AUI_ON_COMMAND("movel",		OnCommandMovel)
AUI_ON_COMMAND("mover",		OnCommandMover)
AUI_ON_COMMAND("skilltree",	OnCommandSkilltree)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgTeach, CDlgBase)

AUI_ON_EVENT("Skill_*",	WM_LBUTTONDOWN,	OnEventLButtonDown)

AUI_END_EVENT_MAP()


CDlgTeach::CDlgTeach()
		:m_nCurTeachPage(0)
{
	m_pTxtRestSP = NULL;
	for(int i = 0; i < CDLGTEACH_TEACHS_PP; i ++)
	{
		m_pSkillIcon[i] = NULL;
		m_pSkillName[i] = NULL;
		m_pSkillLevel[i] = NULL;
		m_pSkillCostSP[i] = NULL;
		m_pSkillCostMoney[i] = NULL;
	}
}

CDlgTeach::~CDlgTeach()
{
}

void CDlgTeach::DoDataExchange(bool bSave)
{
	CDlgBase::DoDataExchange(bSave);

	DDX_Control("Txt_RestSP", m_pTxtRestSP);
	char szName[40];
	for(int i = 0; i < CDLGTEACH_TEACHS_PP; i ++)
	{
		sprintf(szName, "Skill_%d", i + 1);
		DDX_Control(szName, m_pSkillIcon[i]);
		sprintf(szName, "Txt_SkillName%d", i + 1);
		DDX_Control(szName, m_pSkillName[i]);
		sprintf(szName, "Txt_SkillLevel%d", i + 1);
		DDX_Control(szName, m_pSkillLevel[i]);
		sprintf(szName, "Txt_CostSP%d", i + 1);
		DDX_Control(szName, m_pSkillCostSP[i]);
		sprintf(szName, "Txt_CostMoney%d", i + 1);
		DDX_Control(szName, m_pSkillCostMoney[i]);
	}
}

void CDlgTeach::OnCommandCANCAL(const char *szCommand)
{
	Show(false);
	GetHostPlayer()->EndNPCService();
	GetGameUIMan()->m_pCurNPCEssence = NULL;
}

void CDlgTeach::OnCommandMovel(const char *szCommand)
{
	if( m_nCurTeachPage > 0 )
		UpdateTeach( m_nCurTeachPage - 1);
}

void CDlgTeach::OnCommandMover(const char *szCommand)
{
	int nNumSkills = (int)GetDataPtr("int");
	int nNumPages = (nNumSkills + CDLGTEACH_TEACHS_PP - 1) / CDLGTEACH_TEACHS_PP;

	if( m_nCurTeachPage < nNumPages - 1 )
		UpdateTeach( m_nCurTeachPage + 1);
}

void CDlgTeach::OnCommandSkilltree(const char *szCommand)
{
//	CDlgSkillTree* pShow;
//	CECHostPlayer *pHost = GetHostPlayer();
//
//	pShow = GetGameUIMan()->m_pDlgSkillTree[pHost->GetProfession()];
//	pShow->Show(!pShow->IsShow());
}

void CDlgTeach::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !pObj->GetDataPtr("ptr_CECSkill") ) return;

	CECSkill *pSkill = (CECSkill *)pObj->GetDataPtr("ptr_CECSkill");
	int nCondition = GetHostPlayer()->CheckSkillLearnCondition(pSkill->GetSkillID(), true);
	int nCheckCode = 0;
	
	if( 1 == nCondition )
		nCheckCode = 270;
	else if( 6 == nCondition )
		nCheckCode = 527;
	else if( 7 == nCondition )
		nCheckCode = 541;
	else if( 8 == nCondition )
		nCheckCode = 271;
	else if( 9 == nCondition )
		nCheckCode = 556;
	else if( 10 == nCondition )
		nCheckCode = 557;
	PAUIDIALOG pMsgBox;
	if( nCheckCode == 0 )
	{
		GetGameUIMan()->MessageBox("Game_TeachSkill", GetGameUIMan()->GetStringFromTable(231),
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		void *ptr;
		AString szType;
		pObj->ForceGetDataPtr(ptr,szType);
		pMsgBox->SetDataPtr(ptr,szType);
	}
	else
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(nCheckCode),	MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetLife(3);
	}
}

bool CDlgTeach::UpdateTeach(int nPage)
{
	CECWorld *pWorld = GetWorld();
	CECHostPlayer *pHost = GetHostPlayer();

	int idServiceNPC = pHost->GetCurServiceNPC();
	if( 0 == idServiceNPC ) return true;

	CECNPC *pNPC = pWorld->GetNPCMan()->GetNPC(idServiceNPC);
	if( !pNPC || !pNPC->IsServerNPC() ) return true;

	CECNPCServer *pServer = (CECNPCServer *)pNPC;
	int nNumSkills = pServer->GetSkillNum();

	CECSkill *pSkill;
	int i, idSkill, nCondition;
	abase::vector<CECSkill *> vecSkillAvailable;
	int aConditions[128];

	for( i = 0; i < nNumSkills; i++ )
	{
		pSkill = pServer->GetSkill(i);
		idSkill = pSkill->GetSkillID();
		nCondition = pHost->CheckSkillLearnCondition(idSkill, true);
		if( 0 == nCondition || 1 == nCondition || 6 == nCondition ||
			7 == nCondition || /*8 == nCondition ||*/ 9 == nCondition ||
			10 == nCondition )
		{
			aConditions[vecSkillAvailable.size()] = nCondition;
			vecSkillAvailable.push_back(pSkill);
		}
	}
	nNumSkills = vecSkillAvailable.size();

	char szName[40];
	AString strFile;
	ACHAR szText[40];
	CECSkill *pSkillHost;
	PAUIIMAGEPICTURE pImage;
	int idxSkill, nSkillLevel;

	if( nPage >= 0 )
	{
		m_nCurTeachPage = nPage;
	}
	else
	{
		m_nCurTeachPage = max(0, min(m_nCurTeachPage, (nNumSkills
			+ CDLGTEACH_TEACHS_PP - 1) / CDLGTEACH_TEACHS_PP - 1));
	}
	SetDataPtr((void *)nNumSkills,"int");

	ROLEBASICPROP &rbp = pHost->GetBasicProps();

	a_sprintf(szText, _AL("%d"), 0);
	m_pTxtRestSP->SetText(szText);

	for( i = 0; i < CDLGTEACH_TEACHS_PP; i++ )
	{
		pImage = m_pSkillIcon[i];

		idxSkill = m_nCurTeachPage * CDLGTEACH_TEACHS_PP + i;
		if( idxSkill < nNumSkills )
		{
			pSkill = vecSkillAvailable[idxSkill];
			idSkill = pSkill->GetSkillID();
			pSkillHost = pHost->GetSkill(idSkill);
			if( pSkillHost )
				nSkillLevel = pSkillHost->GetSkillLevel() + 1;
			else
				nSkillLevel = 1;

			af_GetFileTitle(pSkill->GetIconFile(), strFile);
			strFile.MakeLower();
			pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILL],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_SKILL][strFile]);
			pImage->SetHint(pSkill->GetDesc(nSkillLevel));
			pImage->SetDataPtr(pSkill,"ptr_CECSkill");

			m_pSkillName[i]->SetText(g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10));

			if (aConditions[idxSkill])
				pImage->SetColor(A3DCOLORRGB(128, 128, 128));
			else
				pImage->SetColor(A3DCOLORRGB(255, 255, 255));

			a_sprintf(szText, _AL("%d"), nSkillLevel);
			m_pSkillLevel[i]->SetText(szText);
		}
		else
		{
			pImage->SetHint(_AL(""));
			pImage->SetDataPtr(NULL);
			pImage->SetCover(NULL, -1);
			pImage->SetColor(A3DCOLORRGB(255, 255, 255));

			sprintf(szName, "Txt_SkillName%d", i + 1);
			m_pSkillName[i]->SetText(_AL(""));

			sprintf(szName, "Txt_SkillLevel%d", i + 1);
			m_pSkillLevel[i]->SetText(_AL(""));

			sprintf(szName, "Txt_CostSP%d", i + 1);
			m_pSkillCostSP[i]->SetText(_AL(""));

			sprintf(szName, "Txt_CostMoney%d", i + 1);
			m_pSkillCostMoney[i]->SetText(_AL(""));
		}
	}

	return true;
}