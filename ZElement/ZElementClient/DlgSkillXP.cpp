// Filename	:	DlgSkillXP.cpp
// Creator	:	Wu Weixin
// Date		:	2013/04/11
// Description:	xp技能栏

#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "DlgSkillXP.h"
#include "DlgDragDrop.h"
#include "ExpTypes.h"
#include "EC_HostPlayer.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "elementdataman.h"
#include "EC_GameUIMan.h"
#include "EC_Skill.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIObject.h"

#define new A_DEBUG_NEW

AUI_BEGIN_EVENT_MAP(CDlgSkillXP, CDlgBase)
AUI_ON_EVENT("Img_Xp",				WM_LBUTTONDOWN,		OnEventLButtonDownItem)
AUI_END_EVENT_MAP()

const char *szProgressImage[] =
{
	"progress\\Xp_PrgsFillImg00.tga",
	"progress\\Xp_PrgsFillImg01.tga",
	"progress\\Xp_PrgsFillImg02.tga",
	"progress\\Xp_PrgsFillImg03.tga",
	"progress\\Xp_PrgsFillImg04.tga",
	"progress\\Xp_PrgsFillImg05.tga",
	"progress\\Xp_PrgsFillImg06.tga",
	"progress\\Xp_PrgsFillImg07.tga",
	"progress\\Xp_PrgsFillImg08.tga",
	"progress\\Xp_PrgsFillImg09.tga",
	"progress\\Xp_PrgsFillImg10.tga",
	"progress\\Xp_PrgsFillImg11.tga",
	"progress\\Xp_PrgsFillImg12.tga",
};

CDlgSkillXP::CDlgSkillXP()
{
}

CDlgSkillXP::~CDlgSkillXP()
{
}

bool CDlgSkillXP::OnInitDialog()
{
	CDlgBase::OnInitDialog();

	m_pImg_Xp = (AUIImagePicture*)GetDlgItem("Img_Xp");
	m_pPrgs_01 = (AUIProgress*)GetDlgItem("Prgs_01");
	m_pGfx_01 = (AUIObject*)GetDlgItem("Gfx_01");

	m_pPrgs_01->SetTransparent(true);
	
	this->SetCanEscClose(false);

	return true;
}

void CDlgSkillXP::OnShowDialog()
{
	SetCanMove(false);
	AlignTo(GetGameUIMan()->GetDialog("Win_QuickBar8H_1"), AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_RIGHT, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_TOP);
}

void CDlgSkillXP::OnTick()
{
	SetSkill();
}

int	CDlgSkillXP::Prof2Index(int prof)
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
	return iProf;		
}

void CDlgSkillXP::SetSkill()
{
	int idProf = GetHostPlayer()->GetProfession();
	elementdataman* pDataman = g_pGame->GetElementDataMan();
	DATA_TYPE dtType;

	// 获取职业模板
	int id = pDataman->get_first_data_id(ID_SPACE_CONFIG , dtType);
	while(id)
	{
		if(dtType == DT_CHARACTER_CLASS_CONFIG)
		{
			const CHARACTER_CLASS_CONFIG* pTempConfig = (const CHARACTER_CLASS_CONFIG*)pDataman->get_data_ptr(id , ID_SPACE_CONFIG , dtType);
			if((int)pTempConfig->character_class_id == idProf)
			{
				if (CECSkill* pSkill = GetHostPlayer()->GetSkill(pTempConfig->xp_skill))
				{
					int idxProf = Prof2Index(idProf);

					//pSkill->GetIconFile()
					//技能圆形图标
					m_pImg_Xp->SetData(0, "IsRound");
					AString strFile;
					strFile.Format("skill_xp%02d.dds", idxProf);
					m_pImg_Xp->SetCover(
						GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_XPSKILL],
						GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_XPSKILL][strFile]);
					m_pImg_Xp->SetDataPtr(pSkill,"ptr_CECSkill");
					m_pImg_Xp->SetHint(pSkill->GetDesc());

					int nCoolingTime = pSkill->GetCoolingTime();
					int nCoolingCnt = pSkill->GetCoolingCnt();
					if (nCoolingTime > 0)
					{
						m_pPrgs_01->SetRangeMax(nCoolingTime);
						m_pPrgs_01->SetProgress(nCoolingTime - nCoolingCnt);
					}

					m_pGfx_01->Show(0 == nCoolingCnt);

					AUIOBJECT_SETPROPERTY p;
					sprintf(p.fn, szProgressImage[idxProf]);
					m_pPrgs_01->SetProperty("Fill Image File", &p);

					return;
				}
				break;
			}
		}
		id = pDataman->get_next_data_id(ID_SPACE_CONFIG , dtType);
	}

	// 什么都没找到, 关了界面
	Show(false);
}

void CDlgSkillXP::OnEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
//	if( pObj->GetData() == 0 )
//		return;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT pt = 
	{
		GET_X_LPARAM(lParam) - p->X,
		GET_Y_LPARAM(lParam) - p->Y,
	};
	
	GetGameUIMan()->m_pDlgDragDrop->GetDlgItem("Goods")->SetSize(80, 80);
	GetGameUIMan()->m_ptLButtonDown = pt;
	GetGameUIMan()->InvokeDragDrop(this, pObj, pt);
}
