// Filename	: DlgPetEquip.cpp
// Creator	: Fu Chonggang
// Date		: 2009/1/06

#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUICTranslate.h"

#include "DlgSoul.h"
#include "DlgSkill.h"

#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "LuaEvent.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSoul, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",	OnCommand_CANCEL)
AUI_ON_COMMAND("start",		OnCommand_start)
AUI_ON_COMMAND("enchase",	OnCommand_enchase)
AUI_ON_COMMAND("split",		OnCommand_split)
AUI_ON_COMMAND("resume",	OnCommand_resume)
AUI_ON_COMMAND("depart",	OnCommand_depart)
AUI_ON_COMMAND("confirm",	OnCommand_confirm)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgSoul, CDlgBase)

AUI_ON_EVENT("Item_a",		WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_item",	WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_Soul",	WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_Equip",	WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_0*",		WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Img_*",		WM_LBUTTONDOWN,	OnEventLButtonDown)

AUI_END_EVENT_MAP()

int CDlgSoul::m_iOwnItemIndex = -1;
int CDlgSoul::m_iOwnItemId = -1;

CDlgSoul::CDlgSoul()
{
}

CDlgSoul::~CDlgSoul()
{
}

bool CDlgSoul::OnInitDialog()
{
	return true;
}

void CDlgSoul::OnShowDialog()
{
	m_pCurEquip = NULL;
	m_pItemSoul = NULL;
	if ( 0==stricmp(GetName(), "Win_EquipOwn") )
	{
		m_iOwnItemIndex	= -1;
		m_iOwnItemId	= -1;
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		if (pImage)
		{
			pImage->ClearCover();
			pImage->SetHint(_AL(""));
			SetDataPtr(NULL);
		}
		GetDlgItem("Btn_Start")->Enable(false);
		GetDlgItem("Btn_Start")->SetHint(_AL(""));
	}
	else if ( 0==stricmp(GetName(), "Win_EquipEnchase") )
	{
		m_iEnchaseItemIndex	= -1;
		m_iEnchaseItemId	= -1;
		m_iEnchaseSoulIndex	= -1;
		m_iEnchaseSoulId	= -1;
		m_iSoulSlot			= -1;
		m_iEnchaseFee		= 0;
		m_iSoulSlotSuit		= -1;
		m_pItemSoul =	NULL;
		GetDlgItem("Btn_Enchase")->Enable(false);
		GetDlgItem("Btn_Enchase")->SetHint(_AL(""));
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem("Item_Equip");
		if (pImage)
		{
			pImage->ClearCover();
			pImage->SetHint(_AL(""));
			SetDataPtr(NULL);
		}

		pImage = (PAUIIMAGEPICTURE)GetDlgItem("Item_Soul");
		if (pImage)
		{
			pImage->ClearCover();
			pImage->SetHint(_AL(""));
			SetDataPtr(NULL);
		}
		
		PAUILABEL pLabel = (PAUILABEL)GetDlgItem("Txt_Limit");
		pLabel->SetText(_AL("0"));
		pLabel = (PAUILABEL)GetDlgItem("Txt_BagNum");
		pLabel->SetText(_AL("0"));
		for (int i=0; i<5; i++ )
		{
			char szName[20];
			sprintf(szName, "Item_0%d", i+1);
			PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			sprintf(szName, "Lab_%d", i+1);
			PAUILABEL pLabel = (PAUILABEL)GetDlgItem(szName);
			pLabel->Show(true);
			pImage->SetCover(NULL,0,0);
			pImage->SetCover(NULL,0,1);
			pImage->SetHint(_AL(""));
		}
		GetDlgItem("Txt_Money1")->SetAlign(AUIFRAME_ALIGN_RIGHT);
		GetDlgItem("Txt_Money2")->SetAlign(AUIFRAME_ALIGN_RIGHT);
		GetDlgItem("Txt_Money3")->SetAlign(AUIFRAME_ALIGN_RIGHT);
		GetDlgItem("Txt_Money1")->SetText(_AL("0"));
		GetDlgItem("Txt_Money2")->SetText(_AL("0"));
		GetDlgItem("Txt_Money3")->SetText(_AL("0"));
	}
	else if ( 0==stricmp(GetName(), "Win_Spilt") )
	{
		m_iSplitItemIndex	= -1;
		m_iSplitItemId		= -1;
		m_iSoulSlot			= -1;
		m_iSplitFee			= 0;
		GetDlgItem("Btn_Spilt")->Enable(false);
		GetDlgItem("Btn_Spilt")->SetHint(_AL(""));
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem("Item_Equip");
		if (pImage)
		{
			pImage->ClearCover();
			pImage->SetHint(_AL(""));
			SetDataPtr(NULL);
		}
		for (int i=0; i<5; i++ )
		{
			char szName[20];
			sprintf(szName, "Img_%d", i+1);
			PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			sprintf(szName, "Lab_%d", i+1);
			PAUILABEL pLabel = (PAUILABEL)GetDlgItem(szName);
			pLabel->Show(true);
			pImage->SetCover(NULL,0,0);
			pImage->SetCover(NULL,0,1);
			pImage->SetHint(_AL(""));
		}
		GetDlgItem("Txt_Money1")->SetAlign(AUIFRAME_ALIGN_RIGHT);
		GetDlgItem("Txt_Money2")->SetAlign(AUIFRAME_ALIGN_RIGHT);
		GetDlgItem("Txt_Money3")->SetAlign(AUIFRAME_ALIGN_RIGHT);
		GetDlgItem("Txt_Money1")->SetText(_AL("0"));
		GetDlgItem("Txt_Money2")->SetText(_AL("0"));
		GetDlgItem("Txt_Money3")->SetText(_AL("0"));
	}
	else if ( 0==stricmp(GetName(), "Win_EquipResume") )
	{
		m_iResumeItemIndex	= -1;
		m_iResumeItemId		= -1;
		m_iResumeFee = GetGameRun()->GetSpecialIDConfig().fee_restore_soul;
		GetDlgItem("Txt_Money1")->SetText(_AL("0"));
		GetDlgItem("Txt_Money2")->SetText(_AL("0"));
		GetDlgItem("Txt_Money3")->SetText(_AL("0"));

		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		if (pImage)
		{
			pImage->ClearCover();
			pImage->SetHint(_AL(""));
			SetDataPtr(NULL);
		}
		GetDlgItem("Txt_Money1")->SetAlign(AUIFRAME_ALIGN_RIGHT);
		GetDlgItem("Txt_Money2")->SetAlign(AUIFRAME_ALIGN_RIGHT);
		GetDlgItem("Txt_Money3")->SetAlign(AUIFRAME_ALIGN_RIGHT);
		GetDlgItem("Btn_Resume")->SetHint(_AL(""));
	}
	else if ( 0==stricmp(GetName(), "Win_EquipDepart") )
	{
		m_iDepartItemIndex	= -1;
		m_iDepartItemId		= -1;
		m_iDepartFee		= 0;
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		if (pImage)
		{
			pImage->ClearCover();
			pImage->SetHint(_AL(""));
			SetDataPtr(NULL);
		}
		GetDlgItem("Txt_Money1")->SetAlign(AUIFRAME_ALIGN_RIGHT);
		GetDlgItem("Txt_Money2")->SetAlign(AUIFRAME_ALIGN_RIGHT);
		GetDlgItem("Txt_Money3")->SetAlign(AUIFRAME_ALIGN_RIGHT);
		GetDlgItem("Txt_Money1")->SetText(_AL("0"));
		GetDlgItem("Txt_Money2")->SetText(_AL("0"));
		GetDlgItem("Txt_Money3")->SetText(_AL("0"));
		GetDlgItem("Btn_Depart")->SetHint(_AL(""));
	}
}

void CDlgSoul::OnCommand_CANCEL(const char * szCommand)
{
	GetGameUIMan()->EndNPCService();
	Show(false);
	GetGameUIMan()->UnfreezeAllItems();
	if (0==stricmp(GetName(), "Win_Message8"))
	{
		GetGameUIMan()->GetDialog("Win_EquipOwn")->Show(false);
	}
}

void CDlgSoul::OnTick()
{
	int iMoney = GetHostPlayer()->GetMoneyAmount();

	const SPECIAL_ID_CONFIG& cfg = g_pGame->GetGameRun()->GetSpecialIDConfig();
	if ( 0==stricmp(GetName(), "Win_EquipOwn") )
	{
		if ( !m_pCurEquip )
		{
			GetDlgItem("Btn_Start")->Enable(false);
			GetDlgItem("Btn_Start")->SetHint(_AL(""));
		}
		else if( GetHostPlayer()->GetPack()->GetItemTotalNum(GetGameRun()->GetSpecialIDConfig().id_bleed_identity_host_item) < 1 )
		{
			GetDlgItem("Btn_Start")->Enable(false);
			GetDlgItem("Btn_Start")->SetHint(GetGameUIMan()->GetStringFromTable(7077));
		}
		else
		{
			GetDlgItem("Btn_Start")->Enable(true);
			GetDlgItem("Btn_Start")->SetHint(_AL(""));
		}
	}
	else if ( 0==stricmp(GetName(), "Win_EquipEnchase") )
	{
		if (iMoney<m_iEnchaseFee || !m_pCurEquip || !m_pItemSoul)
		{
			GetDlgItem("Btn_Enchase")->Enable(false);
			if (!m_pCurEquip)
			{
				GetDlgItem("Btn_Enchase")->SetHint(_AL(""));
			}
			else if (!m_pItemSoul)
			{
				GetDlgItem("Btn_Enchase")->SetHint(GetGameUIMan()->GetStringFromTable(7068));
			}
			else
			{
				GetDlgItem("Btn_Enchase")->SetHint(GetGameUIMan()->GetStringFromTable(7071));
			}
		}
		PAUIIMAGEPICTURE pImage;
		for (int i=0; i<5; i++)
		{
			char szName[20];
			sprintf(szName, "Item_0%d",i+1);
			pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			pImage->SetCover(NULL,0,1);
		}
		if (m_iSoulSlot>0 && m_pCurEquip)
		{
			char szName[20];
			sprintf(szName, "Item_0%d",m_iSoulSlot);
			pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			pImage->SetCover(CDlgSkill::m_pA2DSpriteSkillSelect, (GetTickCount() / 200) % 7, 1);
		}
		if (m_pCurEquip && m_pItemSoul && m_iSoulSlot == -1)
		{
			m_iSoulSlot = m_iSoulSlotSuit > 0 ? m_iSoulSlotSuit : 1;
			AString strName;
			strName.Format("Item_%02d", m_iSoulSlot);
			OnEventLButtonDown(0,0,GetDlgItem(strName));
		}
	}
	else if ( 0==stricmp(GetName(), "Win_Spilt") )
	{
		if (iMoney<m_iSplitFee || !m_pCurEquip)
		{
			GetDlgItem("Btn_Spilt")->Enable(false);
			if (iMoney<m_iSplitFee )
				GetDlgItem("Btn_Spilt")->SetHint(GetGameUIMan()->GetStringFromTable(7067));
			else
				GetDlgItem("Btn_Spilt")->SetHint(_AL(""));
		}
		PAUIIMAGEPICTURE pImage;
		for (int i=0; i<5; i++)
		{
			char szName[20];
			sprintf(szName, "Img_%d",i+1);
			pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			pImage->SetCover(NULL,0,1);
		}
		if (m_iSoulSlot>0 && m_pCurEquip)
		{
			char szName[20];
			sprintf(szName, "Img_%d",m_iSoulSlot);
			pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			pImage->SetCover(CDlgSkill::m_pA2DSpriteSkillSelect, (GetTickCount() / 200) % 7, 1);
		}
	}
	else if ( 0==stricmp(GetName(), "Win_EquipResume") )
	{
		if (iMoney<m_iResumeFeeTotal || !m_pCurEquip )
		{
			GetDlgItem("Btn_Resume")->Enable(false);
			if (iMoney<m_iResumeFeeTotal )
				GetDlgItem("Btn_Resume")->SetHint(GetGameUIMan()->GetStringFromTable(7079));
			else
			{
				GetDlgItem("Btn_Resume")->SetHint(_AL(""));
				GetDlgItem("Txt_Money1")->SetText(_AL("0"));
				GetDlgItem("Txt_Money2")->SetText(_AL("0"));
				GetDlgItem("Txt_Money3")->SetText(_AL("0"));
			}
		}
		else
		{
			CECIvtrEquip *pEquip = (CECIvtrEquip *) m_pCurEquip;
			IVTR_ESSENCE_EQUIPMENT_NEW ieen = pEquip->GetNewEssence();
			if ( ieen.max_spirit_power == ieen.cur_spirit_power )
			{
				GetDlgItem("Btn_Resume")->Enable(false);
				GetDlgItem("Btn_Resume")->SetHint(GetGameUIMan()->GetStringFromTable(7078));
			}
			else
			{
				GetDlgItem("Btn_Resume")->Enable(true);
				GetDlgItem("Btn_Resume")->SetHint(_AL(""));
			}

		}
	}
	else if ( 0==stricmp(GetName(), "Win_EquipDepart") )
	{
		if (iMoney<m_iDepartFee || !m_pCurEquip)
		{
			GetDlgItem("Btn_Depart")->Enable(false);
			if(iMoney<m_iDepartFee)
				GetDlgItem("Btn_Depart")->SetHint(GetGameUIMan()->GetStringFromTable(7075));
			else
				GetDlgItem("Btn_Depart")->SetHint(_AL(""));
		}
		else
		{
			CECIvtrEquip *pEquip = (CECIvtrEquip *) m_pCurEquip;
			IVTR_ESSENCE_EQUIPMENT_NEW ieen = pEquip->GetNewEssence();
			if ( ieen.max_spirit_power != ieen.cur_spirit_power || 0==ieen.max_spirit_power)
			{
				GetDlgItem("Btn_Depart")->Enable(false);
				GetDlgItem("Btn_Depart")->SetHint(GetGameUIMan()->GetStringFromTable(7073));
			}
			else if (HasSoul(m_pCurEquip))
			{
				GetDlgItem("Btn_Depart")->Enable(false);
				GetDlgItem("Btn_Depart")->SetHint(GetGameUIMan()->GetStringFromTable(7074));
			}
			else if (GetHostPlayer()->GetPack()->GetEmptySlotNum()<1)
			{
				GetDlgItem("Btn_Depart")->Enable(false);
				GetDlgItem("Btn_Depart")->SetHint(GetGameUIMan()->GetStringFromTable(7076));
			}
			else
			{
				GetDlgItem("Btn_Depart")->Enable(true);
				GetDlgItem("Btn_Depart")->SetHint(_AL(""));
			}
		}
	}
}


void CDlgSoul::SetItemToOwn(CECIvtrItem *pItem, int nSlot)
{
	AUICTranslate trans;
	CECIvtrItem *pItemOrg = (CECIvtrItem*)GetDataPtr();
	if(pItemOrg)
		pItemOrg->Freeze(false);

	SetDataPtr(pItem);
	pItem->Freeze(true);
	m_pCurEquip = pItem;
	m_iOwnItemIndex	= nSlot;
	m_iOwnItemId	= pItem->GetTemplateID();

	PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
	if (pObj)
	{
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		const wchar_t* szDesc = pItem->GetDesc();
		if( szDesc )
			pObj->SetHint(trans.Translate(szDesc));
		else
			pObj->SetHint(pItem->GetName());
	}
}

void CDlgSoul::SetItemEnchaseEquip(CECIvtrItem *pItem, int nSlot)
{
	AUICTranslate trans;
	CECIvtrItem *pItemOrg = (CECIvtrItem*)GetDataPtr();
	if(pItemOrg)
		pItemOrg->Freeze(false);
	
	SetDataPtr(pItem);
	pItem->Freeze(true);
	m_iEnchaseItemIndex	= nSlot;
	m_iEnchaseItemId	= pItem->GetTemplateID();

	PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_Equip");
	if (pObj)
	{
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		const wchar_t* szDesc = pItem->GetDesc();
		if( szDesc )
			pObj->SetHint(trans.Translate(szDesc));
		else
			pObj->SetHint(pItem->GetName());
	}

	m_pCurEquip = pItem;
	CECIvtrEquip *pEquip = (CECIvtrEquip *) pItem;
	IVTR_ESSENCE_EQUIPMENT_NEW ieen = pEquip->GetNewEssence();
	PAUILABEL pLabel = (PAUILABEL)GetDlgItem("Txt_Limit");
	ACHAR szText[20];
	a_sprintf(szText, _AL("%d"), ieen.max_spirit_power);
	pLabel->SetText(szText);

	pLabel = (PAUILABEL)GetDlgItem("Txt_BagNum");
	a_sprintf(szText, _AL("%d"), ieen.max_spirit_power-ieen.soul_used_spirit);
	pLabel->SetText(szText);
	for (int i=0; i<5; i++ )
	{
		char szName[20];
		sprintf(szName, "Item_0%d", i+1);
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
		sprintf(szName, "Lab_%d", i+1);
		if (ieen.soul[i] == 0)
		{
			GetDlgItem(szName)->Show(true);
			pImage->SetCover(NULL, 0);
			pImage->SetHint(_AL(""));
		}
		else
		{
			GetDlgItem(szName)->Show(false);
			CECIvtrItem* pIvtrItem = CECIvtrItem::CreateItem(ieen.soul[i], 0, 1);
			pIvtrItem->GetDetailDataFromLocal();
			AString strFile;
			af_GetFileTitle(pIvtrItem->GetIconFile(), strFile);
			strFile.MakeLower();
			pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
			const wchar_t* szDesc = pIvtrItem->GetDesc();
			if( szDesc )
				pImage->SetHint(trans.Translate(szDesc));
			else
				pImage->SetHint(pIvtrItem->GetName());
			delete pIvtrItem;
		}
	}
	m_iSoulSlot = -1;
}

void CDlgSoul::SetItemEnchaseSoul(CECIvtrItem *pItem, int nSlot)
{
	AUICTranslate trans;
	if (m_pItemSoul)
	{
		m_pItemSoul->Freeze(false);
	}
	m_pItemSoul = pItem;
	pItem->Freeze(true);
	m_iEnchaseSoulIndex	= nSlot;
	m_iEnchaseSoulId	= pItem->GetTemplateID();

	CECIvtrEquipSoul *pItemSoul = (CECIvtrEquipSoul*)pItem;
	m_iSoulSlotSuit = pItemSoul->GetDBEssence()->hole_type;
	m_iEnchaseFee = (int)pItemSoul->GetDBEssence()->fee_install;
	ACHAR szText[20];
	a_sprintf(szText, _AL("%d"), m_iEnchaseFee / 10000);
	GetDlgItem("Txt_Money1")->SetText(szText);
	a_sprintf(szText, _AL("%d"), (m_iEnchaseFee - (m_iEnchaseFee/10000) * 10000) / 100);
	GetDlgItem("Txt_Money2")->SetText(szText);
	a_sprintf(szText, _AL("%d"), m_iEnchaseFee % 100);
	GetDlgItem("Txt_Money3")->SetText(szText);

	PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_Soul");
	if (pObj)
	{
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		const wchar_t* szDesc = pItem->GetDesc();
		if( szDesc )
			pObj->SetHint(trans.Translate(szDesc));
		else
			pObj->SetHint(pItem->GetName());
	}

	m_iSoulSlot = -1;
	GetDlgItem("Btn_Enchase")->Enable(false);

}

void CDlgSoul::SetItemSplit(CECIvtrItem *pItem, int nSlot)
{
	AUICTranslate trans;
	CECIvtrItem *pItemOrg = (CECIvtrItem*)GetDataPtr();
	if(pItemOrg)
		pItemOrg->Freeze(false);

	SetDataPtr(pItem);
	pItem->Freeze(true);
	m_iSplitItemIndex	= nSlot;
	m_iSplitItemId		= pItem->GetTemplateID();

	m_iSoulSlot = -1;
	m_iSplitFee = 0;
	ACHAR szText[20];
	a_sprintf(szText, _AL("%d"), m_iSplitFee / 10000);
	GetDlgItem("Txt_Money1")->SetText(szText);
	a_sprintf(szText, _AL("%d"), (m_iSplitFee - (m_iSplitFee/10000) * 10000) / 100);
	GetDlgItem("Txt_Money2")->SetText(szText);
	a_sprintf(szText, _AL("%d"), m_iSplitFee % 100);
	GetDlgItem("Txt_Money3")->SetText(szText);

	PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_Equip");
	if (pObj)
	{
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		const wchar_t* szDesc = pItem->GetDesc();
		if( szDesc )
			pObj->SetHint(trans.Translate(szDesc));
		else
			pObj->SetHint(pItem->GetName());
	}

	GetDlgItem("Btn_Spilt")->SetHint(GetGameUIMan()->GetStringFromTable(7066));

	m_pCurEquip = pItem;
	CECIvtrEquip *pEquip = (CECIvtrEquip *) pItem;
	IVTR_ESSENCE_EQUIPMENT_NEW ieen = pEquip->GetNewEssence();
	for (int i=0; i<5; i++ )
	{
		char szName[20];
		sprintf(szName, "Img_%d", i+1);
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
		sprintf(szName, "Lab_%d", i+1);
		if (ieen.soul[i] == 0)
		{
			GetDlgItem(szName)->Show(true);
			pImage->SetCover(NULL, 0);
			pImage->SetHint(_AL(""));
		}
		else
		{
			GetDlgItem(szName)->Show(false);
			CECIvtrItem* pIvtrItem = CECIvtrItem::CreateItem(ieen.soul[i], 0, 1);
			pIvtrItem->GetDetailDataFromLocal();
			AString strFile;
			af_GetFileTitle(pIvtrItem->GetIconFile(), strFile);
			strFile.MakeLower();
			pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
			pImage->SetHint(_AL(""));
			const wchar_t* szDesc = pIvtrItem->GetDesc();
			if( szDesc )
				pImage->SetHint(trans.Translate(szDesc));
			else
				pImage->SetHint(pIvtrItem->GetName());
			delete pIvtrItem;
		}
	}
}

void CDlgSoul::SetItemResume(CECIvtrItem *pItem, int nSlot)
{
	AUICTranslate trans;
	CECIvtrItem *pItemOrg = (CECIvtrItem*)GetDataPtr();
	if(pItemOrg)
		pItemOrg->Freeze(false);

	SetDataPtr(pItem);
	pItem->Freeze(true);
	m_pCurEquip = pItem;
	m_iResumeItemIndex	= nSlot;
	m_iResumeItemId		= pItem->GetTemplateID();
	PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");

	CECIvtrEquip *pEquip = (CECIvtrEquip *) m_pCurEquip;
	int iLockFee = m_iResumeFee*(pEquip->GetNewEssence().max_spirit_power - pEquip->GetNewEssence().cur_spirit_power);
	m_iResumeFeeTotal = iLockFee;
	ACHAR szText[20];
	a_sprintf(szText, _AL("%d"), iLockFee / 10000);
	GetDlgItem("Txt_Money1")->SetText(szText);
	a_sprintf(szText, _AL("%d"), (iLockFee - (iLockFee/10000) * 10000) / 100);
	GetDlgItem("Txt_Money2")->SetText(szText);
	a_sprintf(szText, _AL("%d"), iLockFee % 100);
	GetDlgItem("Txt_Money3")->SetText(szText);

	if (pObj)
	{
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		const wchar_t* szDesc = pItem->GetDesc();
		if( szDesc )
			pObj->SetHint(trans.Translate(szDesc));
		else
			pObj->SetHint(pItem->GetName());
	}
}

void CDlgSoul::SetItemDepart(CECIvtrItem *pItem, int nSlot)
{
	if (pItem->GetExpireDate()!=0)
		return;

	AUICTranslate trans;
	CECIvtrItem *pItemOrg = (CECIvtrItem*)GetDataPtr();
	if(pItemOrg)
		pItemOrg->Freeze(false);
	
	SetDataPtr(pItem);
	pItem->Freeze(true);
	m_iDepartItemIndex	= nSlot;
	m_iDepartItemId		= pItem->GetTemplateID();

	m_pCurEquip = pItem;
	CECIvtrEquip *pItemSoul = (CECIvtrEquip*)pItem;
	m_iDepartFee = pItemSoul->GetDBEssence()->fee_estone;
	ACHAR szText[20];
	a_sprintf(szText, _AL("%d"), m_iDepartFee / 10000);
	GetDlgItem("Txt_Money1")->SetText(szText);
	a_sprintf(szText, _AL("%d"), (m_iDepartFee - (m_iDepartFee/10000) * 10000) / 100);
	GetDlgItem("Txt_Money2")->SetText(szText);
	a_sprintf(szText, _AL("%d"), m_iDepartFee % 100);
	GetDlgItem("Txt_Money3")->SetText(szText);

	PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
	if (pObj)
	{
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		const wchar_t* szDesc = pItem->GetDesc();
		if( szDesc )
			pObj->SetHint(trans.Translate(szDesc));
		else
			pObj->SetHint(pItem->GetName());
	}
}

void CDlgSoul::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if ( 0==stricmp(GetName(), "Win_EquipOwn") && m_pCurEquip)
	{
		CECIvtrItem *pItem = (CECIvtrItem *)GetDataPtr();
		if( pItem )
			pItem->Freeze(false);
		m_pCurEquip = NULL;
		PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		pObj->ClearCover();
		pObj->SetHint(_AL(""));
		SetDataPtr(NULL);
		m_iEnchaseFee = 0;
	}
	else if ( 0==stricmp(GetName(), "Win_EquipEnchase") && (m_pCurEquip || m_pItemSoul) )
	{
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pObj;
		if ( 0==stricmp( pImage->GetName(), "Item_Equip") && m_pCurEquip )
		{
			pImage->ClearCover();
			pImage->SetHint(_AL(""));
			CECIvtrItem *pItem = (CECIvtrItem *)GetDataPtr();
			if( pItem )
				pItem->Freeze(false);
			m_iSoulSlot = 0;
			m_pCurEquip = NULL;
			PAUILABEL pLabel = (PAUILABEL)GetDlgItem("Txt_Limit");
			pLabel->SetText(_AL("0"));
			pLabel = (PAUILABEL)GetDlgItem("Txt_BagNum");
			pLabel->SetText(_AL("0"));
			for (int i=0; i<5; i++)
			{
				char szName[20];
				sprintf(szName, "Lab_%d", i+1);
				GetDlgItem(szName)->Show(true);
				sprintf(szName, "Item_0%d", i+1);
				pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
				pImage->ClearCover();
				pImage->SetHint(_AL(""));
			}
		}
		else if ( 0==stricmp( pImage->GetName(), "Item_Soul") && m_pItemSoul )
		{
			pImage->ClearCover();
			pImage->SetHint(_AL(""));

			if( m_pItemSoul )
				m_pItemSoul->Freeze(false);
			m_iSplitFee = 0;
			m_pItemSoul = NULL;
			GetDlgItem("Txt_Money1")->SetText(_AL("0"));
			GetDlgItem("Txt_Money2")->SetText(_AL("0"));
			GetDlgItem("Txt_Money3")->SetText(_AL("0"));
		}
		else if (m_pCurEquip && m_pItemSoul)
		{
			CECIvtrEquip *pEquip = (CECIvtrEquip *) m_pCurEquip;
			IVTR_ESSENCE_EQUIPMENT_NEW ieen = pEquip->GetNewEssence();
			m_iSoulSlot = atoi(pObj->GetName() + strlen("Item_"));
			if ( ( 0==m_iSoulSlotSuit || m_iSoulSlot == m_iSoulSlotSuit ) &&
				 ( 0 == ieen.soul[m_iSoulSlot-1]) )
			{
				CECIvtrEquipSoul *pItemSoul = (CECIvtrEquipSoul*)m_pItemSoul;
				if ( CurrentAvailableSoul(m_pCurEquip) >= pItemSoul->GetDBEssence()->cost_soul)
				{
					GetDlgItem("Btn_Enchase")->Enable(true);
					GetDlgItem("Btn_Enchase")->SetHint(_AL(""));
				}
				else
					GetDlgItem("Btn_Enchase")->SetHint(GetGameUIMan()->GetStringFromTable(7072));
			}
			else
			{
				GetDlgItem("Btn_Enchase")->Enable(false);
				GetDlgItem("Btn_Enchase")->SetHint(GetGameUIMan()->GetStringFromTable(7069));
			}
			return;
		}
	}
	else if ( 0==stricmp(GetName(), "Win_Spilt") && m_pCurEquip )
	{
		if ( 0==stricmp( pObj->GetName(), "Item_Equip") )
		{
			CECIvtrItem *pItem = (CECIvtrItem *)GetDataPtr();
			if( pItem )
				pItem->Freeze(false);
			m_pCurEquip = NULL;
			PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem("Item_Equip");
			pImage->ClearCover();
			pImage->SetHint(_AL(""));
			SetDataPtr(NULL);
			m_iSoulSlot = 0;
			m_iSplitFee = 0;
			int i;
			for (i=0; i<5; i++)
			{
				char szName[20];
				sprintf(szName, "Img_%d", i+1);
				pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
				pImage->ClearCover();
				pImage->SetHint(_AL(""));
			}
			GetDlgItem("Txt_Money1")->SetText(_AL("0"));
			GetDlgItem("Txt_Money2")->SetText(_AL("0"));
			GetDlgItem("Txt_Money3")->SetText(_AL("0"));
			for (i=0; i<5; i++)
			{
				char szName[20];
				sprintf(szName, "Lab_%d", i+1);
				GetDlgItem(szName)->Show(true);
			}
		}
		else
		{
			m_iSoulSlot = atoi(pObj->GetName() + strlen("Img_"));
			CECIvtrEquip *pEquip = (CECIvtrEquip *) m_pCurEquip;
			IVTR_ESSENCE_EQUIPMENT_NEW ieen = pEquip->GetNewEssence();
			if ( ieen.soul[m_iSoulSlot-1] == 0 )
			{
				GetDlgItem("Btn_Spilt")->Enable(false);
				GetDlgItem("Btn_Spilt")->SetHint(GetGameUIMan()->GetStringFromTable(7066));
				GetDlgItem("Txt_Money1")->SetText(_AL("0"));
				GetDlgItem("Txt_Money2")->SetText(_AL("0"));
				GetDlgItem("Txt_Money3")->SetText(_AL("0"));
			}
			else
			{
				CECIvtrItem* pItem = CECIvtrItem::CreateItem(ieen.soul[m_iSoulSlot-1], 0, 1);
				CECIvtrEquipSoul *pItemSoul = (CECIvtrEquipSoul*)pItem;
				m_iSplitFee = (int)pItemSoul->GetDBEssence()->fee_uninstall;
				ACHAR szText[20];
				a_sprintf(szText, _AL("%d"), m_iSplitFee / 10000);
				GetDlgItem("Txt_Money1")->SetText(szText);
				a_sprintf(szText, _AL("%d"), (m_iSplitFee - (m_iSplitFee/10000) * 10000) / 100);
				GetDlgItem("Txt_Money2")->SetText(szText);
				a_sprintf(szText, _AL("%d"), m_iSplitFee % 100);
				GetDlgItem("Txt_Money3")->SetText(szText);
				delete pItem;

				GetDlgItem("Btn_Spilt")->Enable(true);
				GetDlgItem("Btn_Spilt")->SetHint(_AL(""));
			}
			return;
		}
	}
	else if ( 0==stricmp(GetName(), "Win_EquipResume") && m_pCurEquip )
	{
		CECIvtrItem *pItem = (CECIvtrItem *)GetDataPtr();
		if( pItem )
			pItem->Freeze(false);
		PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		pObj->ClearCover();
		pObj->SetHint(_AL(""));
		SetDataPtr(NULL);
		m_pCurEquip = NULL;
		GetDlgItem("Txt_Money1")->SetText(_AL("0"));
		GetDlgItem("Txt_Money2")->SetText(_AL("0"));
		GetDlgItem("Txt_Money3")->SetText(_AL("0"));
	}
	else if ( 0==stricmp(GetName(), "Win_EquipDepart") && m_pCurEquip )
	{
		CECIvtrItem *pItem = (CECIvtrItem *)GetDataPtr();
		if( pItem )
			pItem->Freeze(false);
		PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		pObj->ClearCover();
		pObj->SetHint(_AL(""));
		SetDataPtr(NULL);
		m_iDepartFee = 0;
		m_pCurEquip = NULL;
		GetDlgItem("Txt_Money1")->SetText(_AL("0"));
		GetDlgItem("Txt_Money2")->SetText(_AL("0"));
		GetDlgItem("Txt_Money3")->SetText(_AL("0"));
	}
}

void CDlgSoul::OnCommand_start(const char * szCommand)
{
	GetGameUIMan()->GetDialog("Win_Message8")->Show(true);
}

void CDlgSoul::OnCommand_enchase(const char * szCommand)
{
	if (m_iSoulSlot>=0)
	{
		GetGameSession()->c2s_CmdNPCSevEmbedSoul(m_iEnchaseItemIndex,m_iEnchaseItemId,m_iEnchaseSoulIndex,m_iSoulSlot-1);
		PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_Equip");
		if (pObj)
		{
			pObj->SetCover(NULL, -1);
			pObj->SetHint(_AL(""));
		}
		pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_Soul");
		if(pObj)
		{
			pObj->SetCover(NULL, -1);
			pObj->SetHint(_AL(""));
		}
		for (int i=0; i<5; i++)
		{
			char szName[20];
			sprintf(szName, "Item_0%d",i+1);
			pObj = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			pObj->SetCover(NULL, -1);
			pObj->SetCover(NULL,0,1);
			pObj->SetHint(_AL(""));
			sprintf(szName, "Lab_%d",i+1);
			GetDlgItem(szName)->Show(true);
			
		}
		GetDlgItem("Txt_Money1")->SetText(_AL("0"));
		GetDlgItem("Txt_Money2")->SetText(_AL("0"));
		GetDlgItem("Txt_Money3")->SetText(_AL("0"));
		m_pCurEquip = NULL;
		m_pItemSoul = NULL;
		GetGameUIMan()->UnfreezeAllItems();
	}
}

void CDlgSoul::OnCommand_split(const char * szCommand)
{
	if (m_iSoulSlot>=0)
	{
		GetGameSession()->c2s_CmdNPCSevClearSoulTessera(m_iSplitItemIndex, m_iSplitItemId,m_iSoulSlot-1);
		PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_Equip");
		if (pObj)
		{
			pObj->SetCover(NULL, -1);
			pObj->SetHint(_AL(""));
		}
		for (int i=0; i<5; i++)
		{
			char szName[20];
			sprintf(szName, "Img_%d",i+1);
			pObj = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			pObj->SetCover(NULL, -1);
			pObj->SetCover(NULL,0,1);
			pObj->SetHint(_AL(""));
		}

		GetDlgItem("Txt_Money1")->SetText(_AL("0"));
		GetDlgItem("Txt_Money2")->SetText(_AL("0"));
		GetDlgItem("Txt_Money3")->SetText(_AL("0"));
		m_pCurEquip = NULL;
		GetGameUIMan()->UnfreezeAllItems();
	}
}

void CDlgSoul::OnCommand_resume(const char * szCommand)
{
	if (m_iResumeItemIndex>=0)
	{
		GetGameSession()->c2s_CmdNPCSevRefineSoul(m_iResumeItemIndex, m_iResumeItemId);
		PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		if (pObj)
		{
			pObj->SetCover(NULL, -1);
			pObj->SetHint(_AL(""));
		}
		m_pCurEquip = NULL;
		GetGameUIMan()->UnfreezeAllItems();
	}
}

void CDlgSoul::OnCommand_depart(const char * szCommand)
{
	if (m_iDepartItemIndex>=0)
	{
		GetGameSession()->c2s_CmdNPCSevBreakDownEquip(m_iDepartItemIndex, m_iDepartItemId);
		PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
		if (pObj)
		{
			pObj->SetCover(NULL, -1);
			pObj->SetHint(_AL(""));
		}
		m_pCurEquip = NULL;
		GetGameUIMan()->UnfreezeAllItems();
		GetDlgItem("Txt_Money1")->SetText(_AL("0"));
		GetDlgItem("Txt_Money2")->SetText(_AL("0"));
		GetDlgItem("Txt_Money3")->SetText(_AL("0"));
		LuaEvent::FirstDepartEquip();
	}
}

void CDlgSoul::OnCommand_confirm(const char * szCommand)
{
	Show(false);
	GetGameSession()->c2s_CmdNPCSevBleedSacrifice(m_iOwnItemIndex, m_iOwnItemId);
	CDlgSoul *pDlg = (CDlgSoul*)GetGameUIMan()->GetDialog("Win_EquipOwn");
	PAUIIMAGEPICTURE pItem = (PAUIIMAGEPICTURE)pDlg->GetDlgItem("Item_a");
	pItem->SetCover(NULL, -1);
	pItem->SetHint(_AL(""));
	PAUIOBJECT pObj = pDlg->GetDlgItem("Btn_Start");
	pObj->Enable(false);
	pObj->SetHint(_AL(""));
	pDlg->ClearObject();
	GetGameUIMan()->UnfreezeAllItems();
	LuaEvent::FirstBleedScarifice();
}

void CDlgSoul::ClearObject()
{
	m_pCurEquip = NULL;
	m_pItemSoul = NULL;
}

int CDlgSoul::CurrentAvailableSoul(CECIvtrItem *pItem)
{
	CECIvtrEquip *pEquip = (CECIvtrEquip *)pItem;
	IVTR_ESSENCE_EQUIPMENT_NEW ieen = pEquip->GetNewEssence();
	int res = ieen.max_spirit_power;
	for (int i=0; i<5; i++)
	{
		if (ieen.soul[i] == 0)
		{
			continue;
		}
		CECIvtrItem* pIvtrItem = CECIvtrItem::CreateItem(ieen.soul[i], 0, 1);
		CECIvtrEquipSoul *pItemSoul = (CECIvtrEquipSoul*)pIvtrItem;
		res -= pItemSoul->GetDBEssence()->cost_soul;
	}
	return res;
}

bool CDlgSoul::HasSoul(CECIvtrItem *pItem)
{
	CECIvtrEquip *pEquip = (CECIvtrEquip *)pItem;
	IVTR_ESSENCE_EQUIPMENT_NEW ieen = pEquip->GetNewEssence();
	for (int i=0; i<5; i++)
	{
		if (ieen.soul[i]>0)
			return true;
	}
	return false;
}
