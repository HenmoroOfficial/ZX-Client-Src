/********************************************************************
	created:	2005/08/15
	created:	15:8:2005   13:22
	file name:	DlgSkill.cpp
	author:		yaojun
	
	purpose:	
*********************************************************************/


#include "AFI.h"
#include "A2DSprite.h"
#include "DlgSkill.h"
#include "DlgSkillMan.h"
#include "EC_Skill.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"
#include "EC_Algorithm.h"
#include "aui/AUIImagePicture.h"
#include "DlgSkillTree.h"
#include "DlgSkillEdit.h"
#include "EC_Game.h"
#include "EC_Configs.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "DlgSettingVideo.h"
#include "LuaEvent.h"
#include "EC_Inventory.h"
#include "EC_IvtrEquip.h"
#include "EC_SendC2SCmds.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW

AUI_BEGIN_EVENT_MAP(CDlgSkill, CDlgBase)

AUI_ON_EVENT("Initiative_*",		WM_LBUTTONDOWN,		OnEventLButtonDownInitiative)
AUI_ON_EVENT("FamilyPositive_*",	WM_LBUTTONDOWN,		OnEventLButtonDownInitiative)
AUI_ON_EVENT("Talisman_*",			WM_LBUTTONDOWN,		OnEventLButtonDownInitiative)
AUI_ON_EVENT("Talent_*",			WM_LBUTTONDOWN,		OnEventLButtonDownInitiative)
AUI_ON_EVENT("Initiative_*",		WM_RBUTTONDOWN,		OnEventRButtonDownInitiative)
AUI_ON_EVENT("Passive_*",			WM_RBUTTONDOWN,		OnEventRButtonDownInitiative)
AUI_ON_EVENT("TeachPositive_*",		WM_RBUTTONDOWN,		OnEventRButtonDownInitiative)
AUI_ON_EVENT("TeachPassive_*",		WM_RBUTTONDOWN,		OnEventRButtonDownInitiative)
AUI_ON_EVENT("ReSkill_*",			WM_RBUTTONDOWN,		OnEventRButtonDownInitiative)
AUI_ON_EVENT("GodSkill_Initiative*",WM_RBUTTONDOWN,		OnEventRButtonDownInitiative)
AUI_ON_EVENT("GodSkill_Passive*",	WM_RBUTTONDOWN,		OnEventRButtonDownInitiative)
AUI_ON_EVENT("Item_*",				WM_LBUTTONDOWN,		OnEventLButtonDownItem)
AUI_ON_EVENT("Item_*",				WM_LBUTTONDBLCLK,	OnEventLButtonDownItem)
AUI_ON_EVENT("GodSkill_Initiative*",WM_LBUTTONDOWN,		OnEventLButtonDownInitiative)
AUI_ON_EVENT("ReSkill_*",			WM_LBUTTONDOWN,		OnEventLButtonDownInitiative)
AUI_ON_EVENT("TeachPositive_*",		WM_LBUTTONDOWN,		OnEventLButtonDownInitiative)
AUI_ON_EVENT("Life_*",				WM_LBUTTONDOWN,		OnEventLButtonDownInitiative)
AUI_ON_EVENT("Article_*",			WM_LBUTTONDOWN,		OnEventLButtonDownInitiative)
AUI_ON_EVENT("CoupleSkill*",		WM_LBUTTONDOWN,		OnEventLButtonDownInitiative)
AUI_ON_EVENT("Equip_*",				WM_LBUTTONDOWN,		OnEventLButtonDownInitiative)

AUI_END_EVENT_MAP()


AUI_BEGIN_COMMAND_MAP(CDlgSkill, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",	OnCommandCancel)
AUI_ON_COMMAND("Btn_Cancel",OnCommandCancel)
AUI_ON_COMMAND("skilltree",	OnCommandSkillTree)
AUI_ON_COMMAND("skill",		OnCommandSkill)
AUI_ON_COMMAND("skillother",OnCommandSkillOther)
AUI_ON_COMMAND("skillspec",	OnCommandSkillSpec)
AUI_ON_COMMAND("fortune",	OnCommandSkillFortune)
AUI_ON_COMMAND("couple",	OnCommandSkillCouple)
AUI_ON_COMMAND("Btn_Next",	OnCommandSkillFortuneNext)
AUI_ON_COMMAND("Btn_Prev",	OnCommandSkillFortunePrev)

AUI_END_COMMAND_MAP()

A2DSprite* CDlgSkill::m_pA2DSpriteSkillSelect = NULL;
A2DSprite* CDlgSkill::m_pA2DSpriteSkillHilight = NULL;
A2DSprite* CDlgSkill::m_pA2DSpriteSkillSelectRound = NULL;
A2DSprite* CDlgSkill::m_pA2DSpriteSkillHilightRound = NULL;
A2DSprite* CDlgSkill::m_pA2DSpriteSkillSpecEquip = NULL;
A2DSprite* CDlgSkill::m_pA2DSpriteSkillSpecEquipLock = NULL;
A2DSprite* CDlgSkill::m_pA2DSpriteSkillRound3D = NULL;
int	CDlgSkill::m_idSkillSelect = 0;

CDlgSkill::CDlgSkill()
{
}

CDlgSkill::~CDlgSkill()
{
}

bool CDlgSkill::OnInitDialog()
{
	m_idSkillSelect = 0;
	
	bool bval;
	if( !m_pA2DSpriteSkillSelect )
	{
		m_pA2DSpriteSkillSelect = new A2DSprite;
		if( !m_pA2DSpriteSkillSelect ) return AUI_ReportError(__LINE__, __FILE__);

		bval = m_pA2DSpriteSkillSelect->Init(m_pA3DDevice, "InGame\\SkillSelect.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
		m_pA2DSpriteSkillSelect->SetLinearFilter(true);
		A3DRECT rc[7];
		int W = m_pA2DSpriteSkillSelect->GetWidth();
		int H = m_pA2DSpriteSkillSelect->GetHeight() / 7;
		for(int i = 0; i < 7; i++ )
			rc[i].SetRect(0, H * i, W, H * (i + 1));
		m_pA2DSpriteSkillSelect->ResetItems(7, rc);
	}
	if( !m_pA2DSpriteSkillHilight )
	{
		m_pA2DSpriteSkillHilight = new A2DSprite;
		if( !m_pA2DSpriteSkillHilight ) return AUI_ReportError(__LINE__, __FILE__);

		bval = m_pA2DSpriteSkillHilight->Init(m_pA3DDevice, "InGame\\SkillHilight.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
		m_pA2DSpriteSkillHilight->SetLinearFilter(true);
		A3DRECT rc[6];
		int W = m_pA2DSpriteSkillHilight->GetWidth();
		int H = m_pA2DSpriteSkillHilight->GetHeight() / 6;
		for(int i = 0; i < 6; i++ )
			rc[i].SetRect(0, H * i, W, H * (i + 1));
		m_pA2DSpriteSkillHilight->ResetItems(6, rc);
	}
	if( !m_pA2DSpriteSkillSelectRound )
	{
		m_pA2DSpriteSkillSelectRound = new A2DSprite;
		if( !m_pA2DSpriteSkillSelectRound ) return AUI_ReportError(__LINE__, __FILE__);
		
		bval = m_pA2DSpriteSkillSelectRound->Init(m_pA3DDevice, "InGame\\Fengshen_SkillSelect.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
		m_pA2DSpriteSkillSelectRound->SetLinearFilter(true);
		A3DRECT rc[7];
		int W = m_pA2DSpriteSkillSelectRound->GetWidth();
		int H = m_pA2DSpriteSkillSelectRound->GetHeight() / 7;
		for(int i = 0; i < 7; i++ )
			rc[i].SetRect(0, H * i, W, H * (i + 1));
		m_pA2DSpriteSkillSelectRound->ResetItems(7, rc);
	}
	if( !m_pA2DSpriteSkillHilightRound )
	{
		m_pA2DSpriteSkillHilightRound = new A2DSprite;
		if( !m_pA2DSpriteSkillHilightRound ) return AUI_ReportError(__LINE__, __FILE__);
		
		bval = m_pA2DSpriteSkillHilightRound->Init(m_pA3DDevice, "InGame\\Fengshen_SkillHilight.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
		m_pA2DSpriteSkillHilightRound->SetLinearFilter(true);
		A3DRECT rc[6];
		int W = m_pA2DSpriteSkillHilightRound->GetWidth();
		int H = m_pA2DSpriteSkillHilightRound->GetHeight() / 6;
		for(int i = 0; i < 6; i++ )
			rc[i].SetRect(0, H * i, W, H * (i + 1));
		m_pA2DSpriteSkillHilightRound->ResetItems(6, rc);
	}
	if( !m_pA2DSpriteSkillSpecEquip )
	{
		m_pA2DSpriteSkillSpecEquip = new A2DSprite;
		if( !m_pA2DSpriteSkillSpecEquip ) return AUI_ReportError(__LINE__, __FILE__);

		bval = m_pA2DSpriteSkillSpecEquip->Init(m_pA3DDevice, "InGame\\EquipSpec.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
		m_pA2DSpriteSkillSpecEquip->SetLinearFilter(true);
		A3DRECT rc[6];
		int W = m_pA2DSpriteSkillSpecEquip->GetWidth();
		int H = m_pA2DSpriteSkillSpecEquip->GetHeight() / 6;
		for(int i = 0; i < 6; i++ )
			rc[i].SetRect(0, H * i, W, H * (i + 1));
		m_pA2DSpriteSkillSpecEquip->ResetItems(6, rc);
	}
	if( !m_pA2DSpriteSkillSpecEquipLock )
	{
		m_pA2DSpriteSkillSpecEquipLock = new A2DSprite;
		if( !m_pA2DSpriteSkillSpecEquipLock ) return AUI_ReportError(__LINE__, __FILE__);

		bval = m_pA2DSpriteSkillSpecEquipLock->Init(m_pA3DDevice, "InGame\\EquipSpecLock.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
		m_pA2DSpriteSkillSpecEquipLock->SetLinearFilter(true);
		A3DRECT rc[6];
		int W = m_pA2DSpriteSkillSpecEquipLock->GetWidth();
		int H = m_pA2DSpriteSkillSpecEquipLock->GetHeight() / 6;
		for(int i = 0; i < 6; i++ )
			rc[i].SetRect(0, H * i, W, H * (i + 1));
		m_pA2DSpriteSkillSpecEquipLock->ResetItems(6, rc);
	}
	if( !m_pA2DSpriteSkillRound3D )
	{
		m_pA2DSpriteSkillRound3D = new A2DSprite;
		if( !m_pA2DSpriteSkillRound3D ) return AUI_ReportError(__LINE__, __FILE__);
		
		m_pA2DSpriteSkillRound3D->SetDynamicTex(true);
		bval = m_pA2DSpriteSkillRound3D->Init(m_pA3DDevice, "icon\\Fengshen_SkillCover.dds", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
		m_pA2DSpriteSkillRound3D->SetLinearFilter(true);
	}

	return true;
}

bool CDlgSkill::Release()
{
	A3DRELEASE(m_pA2DSpriteSkillSelect)
	A3DRELEASE(m_pA2DSpriteSkillHilight)
	A3DRELEASE(m_pA2DSpriteSkillSelectRound)
	A3DRELEASE(m_pA2DSpriteSkillHilightRound)
	A3DRELEASE(m_pA2DSpriteSkillSpecEquip)
	A3DRELEASE(m_pA2DSpriteSkillSpecEquipLock)
	A3DRELEASE(m_pA2DSpriteSkillRound3D)

	return CDlgBase::Release();
}

void CDlgSkill::UpdateLifePossitiveSkill(const SkillVector &sortedSkills)
{
	for (int i = 0; ; i++)
	{
		// get image
		AString strName;
		strName.Format("Life_%02d", i + 1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( !pImage ) break;
		
		if( i < (int)sortedSkills.size() )
		{
			CECSkill * pSkill = sortedSkills[i];
			SetImage(pImage, pSkill, true);
		}
		else
			SetImage(pImage, NULL, true);
	}
}

void CDlgSkill::UpdateLifePassiveSkill(const SkillVector &sortedSkills)
{
	for (int i = 0; ; i++)
	{
		// get image
		AString strName;
		strName.Format("Life_%02d", i + 8);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( !pImage ) break;
		
		if( i < (int)sortedSkills.size() )
		{
			CECSkill * pSkill = sortedSkills[i];
			SetImage(pImage, pSkill, false);
		}
		else
			SetImage(pImage, NULL, false);
	}
}

void CDlgSkill::UpdateEquipSkill()
{
	int i;
	for(i=0; ; ++i)
	{
		AString strName;
		strName.Format("Equip_%02d", i+1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( !pImage ) break;

		if (i<GetHostPlayer()->GetAddonPermanentSkillNum())
			SetImage(pImage, GetHostPlayer()->GetAddonPermanentSkillByIndex(i), true);
		else
			SetImage(pImage, NULL, true);
	}
}

void CDlgSkill::UpdatePositiveSkill(const SkillVector &sortedSkills)
{
	for (int i = 0; ; i++)
	{
		// get image
		AString strName;
		strName.Format("Initiative_%02d", i + 1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( !pImage ) break;
		
		if( i < (int)sortedSkills.size() )
		{
			CECSkill * pSkill = sortedSkills[i];
			SetImage(pImage, pSkill, true);
		}
		else
			SetImage(pImage, NULL, true);
	}
}

void CDlgSkill::UpdatePassiveSkill(const SkillVector &sortedSkills)
{
	for (int i = 0; ; i++)
	{
		AString strName;
		strName.Format("Passive_%02d", i + 1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if (!pImage) break;
		
		if (i < (int)sortedSkills.size())
			SetImage(pImage, sortedSkills[i], false);
		else
			SetImage(pImage, NULL, false);
	}

}

void CDlgSkill::UpdateTalismanSkill(const SkillVector &sortedSkills)
{
	for (int i = 0; ; i++)
	{
		// get image
		AString strName;
		strName.Format("Talisman_%02d", i + 1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( !pImage ) break;
		
		if( i < (int)sortedSkills.size() )
		{
			CECSkill * pSkill = sortedSkills[i];
			SetImage(pImage, pSkill, true);
		}
		else
			SetImage(pImage, NULL, true);
	}
}

void CDlgSkill::UpdateTalentSkill(const SkillVector &sortedSkills)
{
	for (int i = 0; ; i++)
	{
		// get image
		AString strName;
		strName.Format("Talent_%02d", i + 1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( !pImage ) break;
		
		if( i < (int)sortedSkills.size() )
		{
			CECSkill * pSkill = sortedSkills[i];
			SetImage(pImage, pSkill, true);
		}
		else
			SetImage(pImage, NULL, true);
	}
}

void CDlgSkill::UpdateFamilyPositiveSkill(const SkillVector &sortedSkills)
{
	for (int i = 0; ; i++)
	{
		// get image
		AString strName;
		strName.Format("FamilyPositive_%02d", i + 1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( !pImage ) break;
		
		if( i < (int)sortedSkills.size() )
		{
			CECSkill * pSkill = sortedSkills[i];
			SetImage(pImage, pSkill, true);
		}
		else
			SetImage(pImage, NULL, true);
	}
}

void CDlgSkill::UpdateFamilyPassiveSkill(const SkillVector &sortedSkills)
{
	for (int i = 0; ; i++)
	{
		// get image
		AString strName;
		strName.Format("FamilyPassive_%02d", i + 1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( !pImage ) break;
		
		if( i < (int)sortedSkills.size() )
		{
			CECSkill * pSkill = sortedSkills[i];
			SetImage(pImage, pSkill, false);
		}
		else
			SetImage(pImage, NULL, false);
	}
}

void CDlgSkill::UpdateSpouseSkill(const SkillVector &sortedSkills)
{
	for (int i = 0; ; i++)
	{
		// get image
		AString strName;
		strName.Format("CoupleSkill%02d", i + 1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( !pImage ) break;
		
		if( i < (int)sortedSkills.size() )
		{
			CECSkill * pSkill = sortedSkills[i];
			SetImage(pImage, pSkill, true);
		}
		else
			SetImage(pImage, NULL, true);
	}
}

void CDlgSkill::UpdateSectPositiveSkill(const SkillVector &sortedSkills)
{
	for (int i = 0; ; i++)
	{
		// get image
		AString strName;
		strName.Format("TeachPositive_%02d", i + 1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( !pImage ) break;
		
		if( i < (int)sortedSkills.size() )
		{
			CECSkill * pSkill = sortedSkills[i];
			SetImage(pImage, pSkill, true);
		}
		else
			SetImage(pImage, NULL, true);
	}
}

void CDlgSkill::UpdateSectPassiveSkill(const SkillVector &sortedSkills)
{
	for (int i = 0; ; i++)
	{
		// get image
		AString strName;
		strName.Format("TeachPassive_%02d", i + 1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( !pImage ) break;
		
		if( i < (int)sortedSkills.size() )
		{
			CECSkill * pSkill = sortedSkills[i];
			SetImage(pImage, pSkill, false);
		}
		else
			SetImage(pImage, NULL, false);
	}
}

void CDlgSkill::UpdateFlySkill(const SkillVector &sortedSkills)
{
	for (int i = 0; ; i++)
	{
		// get image
		AString strName;
		strName.Format("ReSkill_%02d", i + 1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( !pImage ) break;
		
		if( i < (int)sortedSkills.size() )
		{
			CECSkill * pSkill = sortedSkills[i];
			SetImage(pImage, pSkill, true);
			// 这里有4个技能，有可能有被动技能
			bool bPositive = pSkill->GetType() != CECSkill::TYPE_PASSIVE && pSkill->GetType() != CECSkill::TYPE_LIVE;
			if (!bPositive)
				pImage->SetDataPtr(NULL, "ptr_CECSkill");
		}
		else
			SetImage(pImage, NULL, true);
	}
}

void CDlgSkill::UpdateMysteryPositiveSkill(const SkillVector &sortedSkills)
{
	for (int i = 0; ; i++)
	{
		// get image
		AString strName;
		strName.Format("GodSkill_Initiative%02d", i + 1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( !pImage ) break;
		
		if( i < (int)sortedSkills.size() )
		{
			CECSkill * pSkill = sortedSkills[i];
			SetImage(pImage, pSkill, true);
		}
		else
			SetImage(pImage, NULL, true);
	}
}

void CDlgSkill::UpdateMysteryPassiveSkill(const SkillVector &sortedSkills)
{
	for (int i = 0; ; i++)
	{
		// get image
		AString strName;
		strName.Format("GodSkill_Passive%02d", i + 1);
		PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(GetDlgItem(strName));
		if( !pImage ) break;
		
		if( i < (int)sortedSkills.size() )
		{
			CECSkill * pSkill = sortedSkills[i];
			SetImage(pImage, pSkill, false);
		}
		else
			SetImage(pImage, NULL, false);
	}
}

class CompareCECSkillShowOrder
{
public:
	bool operator ()(CECSkill * p1, CECSkill * p2)
	{
		return p1->GetShowOrder() < p2->GetShowOrder();
	}
};

bool CDlgSkill::Tick(void)
{
	if( IsShow() ) UpdateView();

	return CDlgBase::Tick();
}

bool CDlgSkill::Render()
{
	return CDlgBase::Render();
}


void CDlgSkill::UpdateView()
{
	int i;
	SkillVector vecSkill;

	abase::hash_map<int, bool> mapEquipedIds;
	mapEquipedIds.clear();
	BuildEquipedSkill((CECIvtrEquip*)GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_NECK), mapEquipedIds);
	BuildEquipedSkill((CECIvtrEquip*)GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_FASHION_BODY), mapEquipedIds);

	for( i = 0; i < GetHostPlayer()->GetPositiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPositiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && 
			!pSkill->IsCategory(GNET::CATEGORY_TALISMAN) && 
			!pSkill->IsCategory(GNET::CATEGORY_FAMILY) && 
			!pSkill->IsCategory(GNET::CATEGORY_TALENT) &&
			!pSkill->IsCategory(GNET::CATEGORY_SECT) &&
			!pSkill->IsCategory(GNET::CATEGORY_MYSTERY) &&
			!pSkill->IsCategory(GNET::CATEGORY_REBORN) &&
			!pSkill->IsCategory(GNET::CATEGORY_SPOUSE) &&
			!pSkill->IsCategory(GNET::CATEGORY_LIFE) &&
			!pSkill->IsCategory(GNET::CATEGORY_DEITY)&&
			!pSkill->IsCategory(GNET::CATEGORY_TRIGGERED) )
		{
			// 如果不是装备附加技能 或者是装备附加技能并且身上穿着使技能生效，则在这儿显示，其实技能始终是可以用的！
			if ( !GetHostPlayer()->GetAddonPermanentSkillByID(pSkill->GetSkillID()) ||
			(GetHostPlayer()->GetAddonPermanentSkillByID(pSkill->GetSkillID())&&mapEquipedIds.find(pSkill->GetSkillID())!=mapEquipedIds.end()))
				vecSkill.push_back(pSkill);
		}
	}
	//BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	UpdatePositiveSkill(vecSkill);

	vecSkill.clear();
	for( i = 0; i < GetHostPlayer()->GetPassiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPassiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && 
			!pSkill->IsCategory(GNET::CATEGORY_TALISMAN) && 
			!pSkill->IsCategory(GNET::CATEGORY_FAMILY) && 
			!pSkill->IsCategory(GNET::CATEGORY_TALENT) &&
			!pSkill->IsCategory(GNET::CATEGORY_SECT) &&
			!pSkill->IsCategory(GNET::CATEGORY_MYSTERY) &&
			!pSkill->IsCategory(GNET::CATEGORY_REBORN) &&
			!pSkill->IsCategory(GNET::CATEGORY_SPOUSE) &&
			!pSkill->IsCategory(GNET::CATEGORY_LIFE) &&
			!pSkill->IsCategory(GNET::CATEGORY_DEITY))
			vecSkill.push_back(pSkill);
	}
	BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	UpdatePassiveSkill(vecSkill);

	vecSkill.clear();
	for( i = 0; i < GetHostPlayer()->GetPositiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPositiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_TALISMAN) )
			vecSkill.push_back(pSkill);
	}
	for( i = 0; i < GetHostPlayer()->GetPassiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPassiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_TALISMAN) )
			vecSkill.push_back(pSkill);
	}
	BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	UpdateTalismanSkill(vecSkill);

	vecSkill.clear();
	for( i = 0; i < GetHostPlayer()->GetPositiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPositiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_TALENT) )
			vecSkill.push_back(pSkill);
	}
	BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	UpdateTalentSkill(vecSkill);

	vecSkill.clear();
	for( i = 0; i < GetHostPlayer()->GetPositiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPositiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_FAMILY) )
			vecSkill.push_back(pSkill);
	}
	BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	UpdateFamilyPositiveSkill(vecSkill);
	
	vecSkill.clear();
	for( i = 0; i < GetHostPlayer()->GetPassiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPassiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_FAMILY) )
			vecSkill.push_back(pSkill);
	}
	BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	UpdateFamilyPassiveSkill(vecSkill);

	vecSkill.clear();
	for( i = 0; i < GetHostPlayer()->GetPositiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPositiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_SPOUSE) )
			vecSkill.push_back(pSkill);
	}
	for( i = 0; i < GetHostPlayer()->GetPassiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPassiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_SPOUSE) )
			vecSkill.push_back(pSkill);
	}
	BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	UpdateSpouseSkill(vecSkill);

	vecSkill.clear();
	for( i = 0; i < GetHostPlayer()->GetPositiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPositiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_SECT) )
			vecSkill.push_back(pSkill);
	}
	BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	UpdateSectPositiveSkill(vecSkill);

	vecSkill.clear();
	for( i = 0; i < GetHostPlayer()->GetPassiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPassiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_SECT) )
			vecSkill.push_back(pSkill);
	}
	BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	UpdateSectPassiveSkill(vecSkill);

	vecSkill.clear();
	for( i = 0; i < GetHostPlayer()->GetPositiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPositiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_REBORN) )
			vecSkill.push_back(pSkill);
	}
	for( i = 0; i < GetHostPlayer()->GetPassiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPassiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_REBORN) )
			vecSkill.push_back(pSkill);
	}
	BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	UpdateFlySkill(vecSkill);

	vecSkill.clear();
	int	iCultivation = GetHostPlayer()->GetCultivation();
	for( i = 0; i < GetHostPlayer()->GetPositiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPositiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_MYSTERY) )
		{
			if ( pSkill->GetSkillClass()==148 && iCultivation==1 || //enum OCCUPATION_TAOIST= 148 in skill.h
				pSkill->GetSkillClass()==149 && iCultivation==2 || //enum OCCUPATION_EVIL= 149
				pSkill->GetSkillClass()==150 && iCultivation==4  ) //enum OCCUPATION_BUDDHA= 150
				vecSkill.push_back(pSkill);
		}
	}
	BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	UpdateMysteryPositiveSkill(vecSkill);

	vecSkill.clear();
	for( i = 0; i < GetHostPlayer()->GetPassiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPassiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_MYSTERY) )
		{
			if ( pSkill->GetSkillClass()==148 && iCultivation==1 || //enum OCCUPATION_TAOIST= 148 in skill.h
				pSkill->GetSkillClass()==149 && iCultivation==2 || //enum OCCUPATION_EVIL= 149
				pSkill->GetSkillClass()==150 && iCultivation==4  ) //enum OCCUPATION_BUDDHA= 150
				vecSkill.push_back(pSkill);
		}
	}
	BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	UpdateMysteryPassiveSkill(vecSkill);

	vecSkill.clear();
	for( i = 0; i < GetHostPlayer()->GetPositiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPositiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_LIFE) )
			vecSkill.push_back(pSkill);
	}
	BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	UpdateLifePossitiveSkill(vecSkill);

	vecSkill.clear();
	for( i = 0; i < GetHostPlayer()->GetPassiveSkillNum(); i++ )
	{
		CECSkill * pSkill = GetHostPlayer()->GetPassiveSkillByIndex(i);
		if( pSkill->GetSkillLevel() > 0 && pSkill->IsCategory(GNET::CATEGORY_LIFE) )
			vecSkill.push_back(pSkill);
	}
	BubbleSort(vecSkill.begin(), vecSkill.end(), CompareCECSkillShowOrder());
	UpdateLifePassiveSkill(vecSkill);

	UpdateEquipSkill();
}

void CDlgSkill::OnShowDialog()
{
	CDlgBase::OnShowDialog();

	UpdateView();

	/*
	bool bSkill;
	if( stricmp(m_szName, "Win_Skill") == 0 )
		bSkill = true;
	else
		bSkill = false;
	((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Skill"))->SetPushed(bSkill);
	((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_SkillOther"))->SetPushed(!bSkill);
	*/
// 	if( stricmp(m_szName, "Win_Skill") == 0 )
// 	{
// 		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Skill"))->SetPushed(true);
// 		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_SkillOther"))->SetPushed(false);
// 		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_SkillSpec"))->SetPushed(false);
// 	}
// 	else if( stricmp(m_szName, "Win_SkillOther") == 0 )
// 	{
// 		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Skill"))->SetPushed(false);
// 		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_SkillOther"))->SetPushed(true);
// 		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_SkillSpec"))->SetPushed(false);
// 	}
// 	else
// 	{
// 		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Skill"))->SetPushed(false);
// 		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_SkillOther"))->SetPushed(false);
// 		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_SkillSpec"))->SetPushed(true);
// 	}
	
	
}

void CDlgSkill::OnCommandCancel(const char *szCommand)
{
	if (stricmp(m_szName, "Win_SkillContinual")==0 ||
		stricmp(m_szName, "Win_SkillSpec")==0)
	{
		Show(false);
	}
	else
		GetGameUIMan()->m_pDlgSkillMan->OnCommand_CANCEL("");
}

void CDlgSkill::OnCommandSkillTree(const char *szCommand)
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
	ASSERT(idProf >= 0 && idProf < NUM_PROFESSION);
	char szDlg[20];
	if (idProf>30)
	{
		return;
	}
	sprintf(szDlg, "Win_Skill%02d", idProf);
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog(szDlg);
	pDlg->Show(!pDlg->IsShow());
}

void CDlgSkill::OnCommandSkill(const char *szCommand)
{
	POINT pt = GetGameUIMan()->m_pDlgSkillOther->IsShow() ?
		GetGameUIMan()->m_pDlgSkillOther->GetPos() : GetGameUIMan()->m_pDlgSkillSpec->GetPos();
	GetGameUIMan()->m_pDlgSkillOther->Show(false);
	GetGameUIMan()->m_pDlgSkillSpec->Show(false);
	GetGameUIMan()->m_pDlgSkill->SetPosEx(pt.x, pt.y);
	GetGameUIMan()->m_pDlgSkill->Show(true);
}

void CDlgSkill::OnCommandSkillOther(const char * szCommand)
{
	POINT pt = GetGameUIMan()->m_pDlgSkill->IsShow() ?
		GetGameUIMan()->m_pDlgSkill->GetPos() : GetGameUIMan()->m_pDlgSkillSpec->GetPos();
	GetGameUIMan()->m_pDlgSkill->Show(false);
	GetGameUIMan()->m_pDlgSkillSpec->Show(false);
	GetGameUIMan()->m_pDlgSkillOther->SetPosEx(pt.x, pt.y);
	GetGameUIMan()->m_pDlgSkillOther->Show(true);
}

void CDlgSkill::OnCommandSkillSpec(const char * szCommand)
{
	POINT pt = GetGameUIMan()->m_pDlgSkill->IsShow() ?
		GetGameUIMan()->m_pDlgSkill->GetPos() : GetGameUIMan()->m_pDlgSkillOther->GetPos();
	GetGameUIMan()->m_pDlgSkill->Show(false);
	GetGameUIMan()->m_pDlgSkillOther->Show(false);
	GetGameUIMan()->m_pDlgSkillSpec->SetPosEx(pt.x, pt.y);
	GetGameUIMan()->m_pDlgSkillSpec->Show(true);
}

void CDlgSkill::OnCommandSkillFortune(const char * szCommand)
{
	if( CDlgSkillTree::m_pFortuneActive && (CDlgSkillTree::m_pFortuneActive->IsShow()) )
		CDlgSkillTree::m_pFortuneActive->Show(false);
	else
	{
		if ( 0==GetHostPlayer()->GetCultivation() || 1==GetHostPlayer()->GetCultivation() )
		{
			CDlgSkillTree::m_pFortuneActive = (CDlgSkillTree*)GetGameUIMan()->GetDialog("Win_Taoism");
		}
		else if ( 2==GetHostPlayer()->GetCultivation() )
		{
			CDlgSkillTree::m_pFortuneActive = (CDlgSkillTree*)GetGameUIMan()->GetDialog("Win_Evil");
		}
		else if ( 4==GetHostPlayer()->GetCultivation() )
		{
			CDlgSkillTree::m_pFortuneActive = (CDlgSkillTree*)GetGameUIMan()->GetDialog("Win_Buddhism");
		}
		CDlgSkillTree::m_pFortuneActive->Show(!CDlgSkillTree::m_pFortuneActive->IsShow());
	}
}

void CDlgSkill::OnCommandSkillCouple(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Couple");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgSkill::OnCommandSkillFortuneNext(const char * szCommand)
{
	Show(false);
	AString strName = GetName();
	strName += "2";
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog(strName);
	pDlg->Show(true);
}

void CDlgSkill::OnCommandSkillFortunePrev(const char * szCommand)
{
	Show(false);
	AString strName = GetName();
	strName.CutRight(1);
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog(strName);
	pDlg->Show(true);
}

void CDlgSkill::OnEventLButtonDownInitiative(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
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

void CDlgSkill::OnEventRButtonDownInitiative(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECSkill") )
		return;

	CECSkill *pSkill = (CECSkill*)pObj->GetDataPtr("ptr_CECSkill");
	SelectSkill(pSkill->GetSkillID());
}

void CDlgSkill::OnEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( pObj->GetData() == 0 )
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

void CDlgSkill::SetImage(AUIImagePicture *pImage, CECSkill *pSkill, bool bPositiveSkill)
{
	ASSERT(pImage);
	//2010.6.30 被动技能改为同主动技能类似的显示
	bPositiveSkill = true;

	// get clock icon
	AUIClockIcon *pClock = pImage->GetClockIcon();

	if (pSkill)
	{
		AString strFile, strFileRound;
		af_GetFileTitle(pSkill->GetIconFile(), strFile);
		strFile.MakeLower();
		pImage->SetData(0, "IsRound");
		//技能圆形图标后缀
		if (strstr(pImage->GetParent()->GetName(), "Win_Skill_Fengshen") && strFile.Right(4)==".dds")
		{
			strFileRound = strFile;
			strFileRound.CutRight(4);
			strFileRound += "_0.dds";
			if (GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_SKILL, strFileRound)>=0)
			{
				strFile = strFileRound;
				pImage->SetCover(m_pA2DSpriteSkillRound3D, 0, 2);
				pImage->SetData(999, "IsRound");
			}
		}
		pImage->SetCover(
			GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_SKILL],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_SKILL][strFile]);
		pImage->SetDataPtr(pSkill,"ptr_CECSkill");
		pImage->SetHint(pSkill->GetDesc());
		if( stricmp(pImage->GetParent()->GetName(), "Win_Skill") == 0 ||
			stricmp(pImage->GetParent()->GetName(), "Win_SkillOther") == 0 ||
			stricmp(pImage->GetParent()->GetName(), "Win_SkillSpec") == 0 ||
			stricmp(pImage->GetParent()->GetName(), "Win_TeacherManage") == 0)
		{
			bool b = false;
			int i;
			for(i = 0; i < pSkill->GetTalentSize(); i++)
				if( pSkill->GetTalent()[i] == CDlgSkill::GetSelectSkill() )
					b = true;
			CECSkill *pSkill1 = new CECSkill(CDlgSkill::GetSelectSkill(), 1);
			for(i = 0; i < pSkill1->GetTalentSize(); i++ )
				if( pSkill->GetSkillID() == pSkill1->GetTalent()[i] )
					b = true;
			delete pSkill1;
			if( b )
				pImage->SetCover(GetA2DSpriteSkillHilight(pImage->GetData("IsRound")>0), (GetTickCount() / 200) % 6, 1);
			else if( pSkill->GetSkillID() == GetSelectSkill() )
				pImage->SetCover(GetA2DSpriteSkillSelect(pImage->GetData("IsRound")>0), (GetTickCount() / 200) % 7, 1);
			else
				pImage->SetCover(NULL, 0, 1);
		}
		if( bPositiveSkill )
		{
			if( pSkill->ReadyToCast() && GetHostPlayer()->GetPrepSkill() != pSkill )
			{
				if( !GetHostPlayer()->CheckSkillCastCondition(pSkill) )
					pImage->SetColor(A3DCOLORRGB(255, 255, 255));
				else
					pImage->SetColor(A3DCOLORRGB(128, 128, 128));
			}
			else
				pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
			if( pSkill->GetCoolingTime() > 0 || GetHostPlayer()->GetPrepSkill() == pSkill )
			{
				pClock->SetProgressRange(0, pSkill->GetCoolingTime());
				if( GetHostPlayer()->GetPrepSkill() == pSkill )
					pClock->SetProgressPos(0);
				else
					pClock->SetProgressPos(pSkill->GetCoolingTime() - pSkill->GetCoolingCnt());
			}
		}
	}
	else
	{
		pImage->SetCover(NULL, -1);
		pImage->SetDataPtr(NULL);
		pImage->SetData(0, "IsRound");
		pImage->SetHint(_AL(""));
		if( bPositiveSkill )
		{
			pImage->SetColor(A3DCOLORRGB(255, 255, 255));
			pClock->SetProgressRange(0, 1);
			pClock->SetProgressPos(1);
		}
	}
}

void CDlgSkill::SelectSkill(int id)
{
	if( id != m_idSkillSelect )
		m_idSkillSelect = id;
	else
		m_idSkillSelect = 0;
}

int CDlgSkill::GetSelectSkill()
{
	return m_idSkillSelect;
}

void CDlgSkill::BuildEquipedSkill(CECIvtrEquip *pEquip, abase::hash_map<int, bool>& mapIds)
{
	if (!pEquip)
		return;
	
	int idSuite = pEquip->GetSuiteID();
	if (!idSuite)
		return;

	DATA_TYPE DataType = DT_INVALID;
	elementdataman* pDataMan = GetGame()->GetElementDataMan();
	const void* pData = pDataMan->get_data_ptr(idSuite, ID_SPACE_ESSENCE, DataType);
	if (DataType != DT_SUITE_ESSENCE)
		return;
	
	enum
	{
		MAX_NUM = 14
	};
	const SUITE_ESSENCE* pSuiteEss = (const SUITE_ESSENCE*)pData;
	int iItemCnt, aEquipped[MAX_NUM];
	iItemCnt = GetHostPlayer()->GetEquippedSuiteItem(idSuite, aEquipped);
	for (int i=2; i<=pSuiteEss->max_equips; i++)
	{
		int idProp = (int)pSuiteEss->addons[i-2].id;
		if (!idProp)
			continue;
		
		pData = pDataMan->get_data_ptr(idProp, ID_SPACE_ADDON, DataType);
		if (DataType != DT_EQUIPMENT_ADDON)
		{
			ASSERT(DataType == DT_EQUIPMENT_ADDON);
			continue;
		}
		 
		const EQUIPMENT_ADDON* pAddOn = (const EQUIPMENT_ADDON*)pData;
		// 61是永久时装技能
		if (pAddOn->type==61 && i<=iItemCnt)
		{
			mapIds[pAddOn->param1] = true;
		}
	}
	
}