// File		: DlgPetService.cpp
// Creator	: Xiao Zhou
// Date		: 2008/3/26

#include "AFI.h"
#include "DlgPetService.h"
#include "DlgInventory.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "AUI\\AUICTranslate.h"
#include "elementdataman.h"
#include "LuaEvent.h"


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgPetService, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCANCEL)
AUI_ON_COMMAND("start",			OnCommandStart)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgPetService, CDlgBase)

AUI_ON_EVENT("Item_*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Item_*",		WM_LBUTTONDBLCLK,	OnEventLButtonDown)
//AUI_ON_EVENT("Item_Pet",		WM_RBUTTONDOWN,		OnEventLButtonDown)
//AUI_ON_EVENT("Item_Tool",		WM_LBUTTONDOWN,		OnEventLButtonDown)
//AUI_ON_EVENT("Item_Tool",		WM_LBUTTONDBLCLK,	OnEventLButtonDown)
//AUI_ON_EVENT("Item_Tool",		WM_RBUTTONDOWN,		OnEventLButtonDown)
//AUI_ON_EVENT("Item_Assistant",	WM_LBUTTONDOWN,		OnEventLButtonDown)
//AUI_ON_EVENT("Item_Assistant",	WM_LBUTTONDBLCLK,	OnEventLButtonDown)
//AUI_ON_EVENT("Item_Assistant",	WM_RBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgPetService::CDlgPetService()
{
}

CDlgPetService::~CDlgPetService()
{
}

bool CDlgPetService::OnInitDialog()
{
	m_pItemPet = (PAUIIMAGEPICTURE)GetDlgItem("Item_Pet");
	m_pItemTool1 = (PAUIIMAGEPICTURE)GetDlgItem("Item_Tool1");
	m_pItemAssitant = (PAUIIMAGEPICTURE)GetDlgItem("Item_Assistant");
	m_pItemTool = (PAUIIMAGEPICTURE)GetDlgItem("Item_Tool");
	m_pBtn_Enchase = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Enchase");
	m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
	m_pTxt_Money2 = (PAUILABEL)GetDlgItem("Txt_Money2");
	m_pTxt_Money3 = (PAUILABEL)GetDlgItem("Txt_Money3");
	m_pTxt_Money1->SetData(0);
	m_pTxt_Money1->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money2->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money3->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_iItemId = 0;
	if( stricmp(m_szName, "Win_PetRaise") == 0 )
		m_pTxt_Money1->SetData(GetGameRun()->GetSpecialIDConfig().fee_pet_gained);
	else if( stricmp(m_szName, "Win_PetChangeName") == 0 )
		m_pTxt_Money1->SetData(GetGameRun()->GetSpecialIDConfig().fee_pet_rename);
	else if( stricmp(m_szName, "Win_PetCombine") == 0 )
		m_pTxt_Money1->SetData(GetGameRun()->GetSpecialIDConfig().fee_pet_refine);
	else if( stricmp(m_szName, "Win_PetRelease") == 0 )
		m_pTxt_Money1->SetData(GetGameRun()->GetSpecialIDConfig().fee_pet_free);
	else if( stricmp(m_szName, "Win_PetEquipEnchanse") == 0 )
		m_pItemPet = (PAUIIMAGEPICTURE)GetDlgItem("Item_PetArmor");

	return true;
}

bool CDlgPetService::CheckItem(PAUIOBJECT pObj)
{
	CECInventory *pPack = GetHostPlayer()->GetPack();
	CECIvtrItem *pItem;
	AUICTranslate trans;
	if( pObj && pObj->GetDataPtr("ptr_CECIvtrItem") )
	{
		pItem = pPack->GetItem(pObj->GetData());
		if( !pItem )
		{
			OnEventLButtonDown(0, 0, pObj);
			return false;
		}
		else
		{
			const ACHAR * szDesc = pItem->GetDesc();
			if( szDesc )
				pObj->SetHint(trans.Translate(szDesc));
			else
				pObj->SetHint(_AL(""));
			return true;
		}
	}

	return false;
}

void CDlgPetService::OnTick()
{
	int nMoney = m_pTxt_Money1->GetData();
	ACHAR szMoney[20];
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
	m_pTxt_Money3->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pTxt_Money2->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pTxt_Money1->SetText(szMoney);
	bool bEnoughMoney = false;
	if( nMoney > GetHostPlayer()->GetMoneyAmount() )
	{
		m_pTxt_Money1->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxt_Money2->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxt_Money3->SetColor(A3DCOLORRGB(255, 0, 0));
		bEnoughMoney = false;
	}
	else
	{
		m_pTxt_Money1->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pTxt_Money2->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pTxt_Money3->SetColor(A3DCOLORRGB(255, 255, 255));
		bEnoughMoney = true;
	}
	CheckItem(m_pItemPet);
	CheckItem(m_pItemTool);
	CheckItem(m_pItemAssitant);
	if( stricmp(m_szName, "Win_PetRaise") == 0 ||
		stricmp(m_szName, "Win_PetRelease") == 0 )
		m_pBtn_Enchase->Enable(bEnoughMoney && m_pItemPet->GetDataPtr("ptr_CECIvtrItem") != NULL);
	else if( stricmp(m_szName, "Win_PetChangeName") == 0 )
		m_pBtn_Enchase->Enable(bEnoughMoney && m_pItemPet->GetDataPtr("ptr_CECIvtrItem") != NULL && a_strlen(GetDlgItem("Edit_ChangeName")->GetText()) > 0);
	else if( stricmp(m_szName, "Win_PetCombine") == 0 )
		m_pBtn_Enchase->Enable(bEnoughMoney && m_pItemPet->GetDataPtr("ptr_CECIvtrItem") && m_pItemTool->GetDataPtr("ptr_CECIvtrItem"));
	else if( stricmp(m_szName, "Win_PetEquipEnchanse") == 0 )
		m_pBtn_Enchase->Enable(bEnoughMoney && m_pItemPet->GetDataPtr("ptr_CECIvtrItem") && m_pItemTool1->GetDataPtr("ptr_CECIvtrItem"));
}

void CDlgPetService::OnCommandCANCEL(const char * szCommand)
{
	Show(false);
	GetHostPlayer()->EndNPCService();
	GetGameUIMan()->m_pCurNPCEssence = NULL;
	GetGameUIMan()->m_pDlgInventory->Show(false);
	if( m_pItemPet )
		OnEventLButtonDown(0, 0, m_pItemPet);
	if( m_pItemTool )
		OnEventLButtonDown(0, 0, m_pItemTool);
	if( m_pItemAssitant )
		OnEventLButtonDown(0, 0, m_pItemAssitant);
	GetGameUIMan()->UnfreezeAllItems();
}

void CDlgPetService::OnCommandStart(const char * szCommand)
{
	PAUIDIALOG pMsgBox;
	CECHostPlayer *pHost = GetHostPlayer();

	if( !CheckItem(m_pItemPet) ) return;

	int nMoney = m_pTxt_Money1->GetData();
	if( nMoney > pHost->GetMoneyAmount() )
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(656), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		return;
	}
	
	CECIvtrItem *pIvtrA = (CECIvtrItem *)m_pItemPet->GetDataPtr("ptr_CECIvtrItem");

	CECPetBedge *pPet = (CECPetBedge*)pIvtrA;
	if( 0 == stricmp(GetName(), "Win_PetRaise") )
	{
		if( pPet )
		{
			GetGameSession()->c2s_CmdNPCSevPetAdopt(m_pItemPet->GetData());
			OnEventLButtonDown(0, 0, m_pItemPet);
		}
	}
	else if( 0 == stricmp(GetName(), "Win_PetChangeName") )
	{
		if( pPet )
		{
			ACString strName = GetDlgItem("Edit_ChangeName")->GetText();
			GetGameUIMan()->FilterBadWords(strName);
			if (CheckNameValid(strName.GetBuffer(0)))
			{
				GetGameSession()->c2s_CmdNPCSevRename(m_pItemPet->GetData(), strName);
				OnEventLButtonDown(0, 0, m_pItemPet);
				LuaEvent::FirstChangePetName();
			}
		}
	}
	else if( 0 == stricmp(GetName(), "Win_PetCombine") )
	{
		if( CheckItem(m_pItemTool) )
		{
			if( CheckItem(m_pItemAssitant) )
			{
				GetGameSession()->c2s_CmdNPCSevCombine(m_pItemPet->GetData(), m_pItemTool->GetData(), m_pItemAssitant->GetData());
				OnEventLButtonDown(0, 0, m_pItemAssitant);
			}
			else
				GetGameSession()->c2s_CmdNPCSevCombine(m_pItemPet->GetData(), m_pItemTool->GetData(), -1);
			OnEventLButtonDown(0, 0, m_pItemPet);
			OnEventLButtonDown(0, 0, m_pItemTool);
			LuaEvent::FirstChangePetShape();
		}
	}
	else if( 0 == stricmp(GetName(), "Win_PetRelease") )
	{
		if( pPet )
		{
			GetGameUIMan()->MessageBox("Game_PetFree",GetGameUIMan()->GetStringFromTable(7326), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
		}
	}
	else if( 0 == stricmp(GetName(), "Win_PetEquipEnchanse") )
	{
		int nMoney = GetHostPlayer()->GetMoneyAmount();
		if( (DWORD)nMoney >= m_pTxt_Money1->GetData() && m_pItemPet->GetDataPtr("ptr_CECIvtrItem") && m_pItemTool1->GetDataPtr("ptr_CECIvtrItem") )
		{
			GetGameSession()->c2s_CmdNPCSevPetEquipEnchant(m_pItemPet->GetData(), m_iItemId, m_pItemTool1->GetData());
			OnEventLButtonDown(0, 0, m_pItemPet);
			OnEventLButtonDown(0, 0, m_pItemTool1);
			LuaEvent::FirstEnhancePetEquip();
		}
	}
}

void CDlgPetService::PetFree()
{
	CECIvtrItem *pIvtrA = (CECIvtrItem *)m_pItemPet->GetDataPtr("ptr_CECIvtrItem");

	CECPetBedge *pPet = (CECPetBedge*)pIvtrA;
	if( pPet )
	{
		GetGameSession()->c2s_CmdNPCSevFree(m_pItemPet->GetData());
		OnEventLButtonDown(0, 0, m_pItemPet);
	}
}

void CDlgPetService::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !pObj || !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;
	PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pObj;
	CECInventory *pPack = GetHostPlayer()->GetPack();
	CECIvtrItem *pItem = pPack->GetItem(pObj->GetData());
	if( pItem )
		pItem->Freeze(false);
	pImage->SetDataPtr(NULL);
	pImage->SetData(0);
	pImage->SetHint(_AL(""));
	pImage->SetCover(NULL, -1);
	GetGameUIMan()->PlayItemSound(pItem, false);
	if( 0 == stricmp(pObj->GetName(), "m_pItemPet"))
	{
		m_iItemId = 0;
	}
}

void CDlgPetService::DragDropItem(int iSrc, PAUIOBJECT pObjOver)
{
	if( !pObjOver )
		return;

	CECIvtrItem *pIvtrSrc = GetHostPlayer()->GetPack()->GetItem(iSrc);

	if( 0 == stricmp(pObjOver->GetName(), "Item_Pet") && pIvtrSrc->GetClassID() != CECIvtrItem::ICID_PET_BEDGE )
		return;
	
	if( 0 == stricmp(pObjOver->GetName(), "Item_Tool") && pIvtrSrc->GetClassID() != CECIvtrItem::ICID_PET_REFINE )
		return;

	if( 0 == stricmp(pObjOver->GetName(), "Item_Assistant") && pIvtrSrc->GetClassID() != CECIvtrItem::ICID_PET_ASSIST_REFINE )
		return;

	if( 0 == stricmp(pObjOver->GetName(), "Item_PetArmor") && pIvtrSrc->GetClassID() != CECIvtrItem::ICID_PET_ARMOR )
		return;

	OnEventLButtonDown(0, 0, pObjOver);
	AString strFile;
	af_GetFileTitle(pIvtrSrc->GetIconFile(), strFile);
	strFile.MakeLower();
	PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pObjOver;
	pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);

	GetGameUIMan()->PlayItemSound(pIvtrSrc, true);
	pIvtrSrc->Freeze(true);
	pObjOver->SetData(iSrc);
	pObjOver->SetDataPtr(pIvtrSrc, "ptr_CECIvtrItem");

	if( 0 == stricmp(pObjOver->GetName(), "Item_PetArmor"))
	{
		CECPetArmor *pPetArmor = (CECPetArmor*)pIvtrSrc;
		int nFee = pPetArmor->GetDBEssence()->fee_enhance;
		ACHAR szMoney[20];
		a_sprintf(szMoney, _AL("%d"), nFee % 100);
		m_pTxt_Money3->SetText(szMoney);
		a_sprintf(szMoney, _AL("%d"), (nFee % 10000) / 100);
		m_pTxt_Money2->SetText(szMoney);
		a_sprintf(szMoney, _AL("%d"), nFee / 10000);
		m_pTxt_Money1->SetText(szMoney);
		m_pTxt_Money1->SetData(nFee);
		m_iItemId = pPetArmor->GetTemplateID();
	}
}

bool CDlgPetService::CheckNameValid(ACHAR* szText)
{
	int iLength = a_strlen(szText);
	for (int i=0; i<iLength; i++)
	{
		if (szText[i] == '^' || szText[i] == '\\' || szText[i] == '&')
		{
			GetGameUIMan()->AddChatMessage(GetStringFromTable(9180), GP_CHAT_MISC);
			return false;
		}
	}
	return true;
}