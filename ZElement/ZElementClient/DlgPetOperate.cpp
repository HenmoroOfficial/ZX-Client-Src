// Filename	: .cpp
// Creator	: Fu Chonggang
// Date		: 

#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "DlgPetOperate.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "EC_NewHelpGuide.h"

#ifdef BACK_VERSION
#include "EC_Manager.h"
#else
#include "ECManager.h"
#include "ECManagerID.h"
#endif
#include "LuaEvent.h"

#define new A_DEBUG_NEW


AUI_BEGIN_COMMAND_MAP(CDlgPetOperate, CDlgItemSvrBase)

AUI_ON_COMMAND("Btn_Cancel",		OnCommandStopOpt)

AUI_END_COMMAND_MAP()

CDlgPetOperate::CDlgPetOperate()
{
}

CDlgPetOperate::~CDlgPetOperate()
{
}

bool CDlgPetOperate::OnInitDialog()
{
	bool bRet = CDlgItemSvrBase::OnInitDialog();
	if (m_szName=="Win_Pet_Tame")
		m_iMoney = GetGameRun()->GetSpecialIDConfig().fee_pet_gained;
	else if (m_szName=="Win_Pet_Release")
		m_iMoney = GetGameRun()->GetSpecialIDConfig().fee_pet_free;
	
	return bRet;
}

void CDlgPetOperate::OnCommandStopOpt(const char *szCommand)
{
	GetGameRun()->PostMessage(MSG_HST_PRESSCANCEL, MAN_PLAYER, 0);
}

void CDlgPetOperate::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if(pItem->GetClassID() != CECIvtrItem::ICID_PET_BEDGE)
		return;

	if (!pImg)
		pImg = m_pImg_Item;

	if (pImg!=m_pImg_Item)
		return;
	
	OnEventLButtonDown(0, 0, pImg);
	pItem->Freeze(true);
	GetGameUIMan()->SetIvtrInfo(pImg, pItem);
	m_pImg_Item->SetData(nSlot);
	m_pImg_Item->SetDataPtr(pItem, "ptr_CECIvtrItem");
	m_pBtn_Confirm->Enable(true);
}

void CDlgPetOperate::ServerStart()
{
	CECIvtrItem *pItem = (CECIvtrItem *)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");	
	CECPetBedge *pPet = (CECPetBedge*)pItem;

	if(!pPet)
		return;

	if( 0 == stricmp(GetName(), "Win_Pet_Tame") )
	{
		GetGameSession()->c2s_CmdNPCSevPetAdopt(m_pImg_Item->GetData());
		OnEventLButtonDown(0, 0, m_pImg_Item);
		NewHelpGuide::StartGuide_Step_5(GUIDE_HELP_PET_11);
	}
	else if( 0 == stricmp(GetName(), "Win_Pet_Release") )
	{
		GetGameUIMan()->MessageBox("Game_PetFree",GetGameUIMan()->GetStringFromTable(7326), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
	}
}

void CDlgPetOperate::Reset()
{
	UpdateMoney();
}

void CDlgPetOperate::UpdateDisplay()
{
	if (!m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))
	{
		Reset();
	}
}

void CDlgPetOperate::PetFree()
{
	CECIvtrItem *pIvtrA = (CECIvtrItem *)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
	
	CECPetBedge *pPet = (CECPetBedge*)pIvtrA;
	if( pPet )
	{
		GetGameSession()->c2s_CmdNPCSevFree(m_pImg_Item->GetData());
		OnEventLButtonDown(0, 0, m_pImg_Item);
	}
}



AUI_BEGIN_COMMAND_MAP(CDlgPetReName, CDlgItemSvrBase)

AUI_ON_COMMAND("Btn_Cancel",		OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgPetReName, CDlgItemSvrBase)

AUI_ON_EVENT("Img_*",	WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Img_*",	WM_RBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_*",	WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_*",	WM_RBUTTONDOWN,	OnEventLButtonDown)

AUI_END_EVENT_MAP()

void CDlgPetReName::OnCommandCancel(const char *szCommand)
{
	Show(false);
}

void CDlgPetReName::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
}

void CDlgPetReName::ServerStart()
{
	ACString strName = GetDlgItem("Edit_ChangeName")->GetText();
	GetGameUIMan()->FilterBadWords(strName);
	if (CheckNameValid(strName.GetBuffer(0)))
	{
		GetGameSession()->c2s_CmdNPCSevRename(GetData(), strName);
		Show(false);
		LuaEvent::FirstChangePetName();
	}
	m_iMoney = GetGameRun()->GetSpecialIDConfig().fee_pet_rename;
}

void CDlgPetReName::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if (!pImg)
	{
		pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
	}

	pItem->Freeze(true);
	GetGameUIMan()->SetIvtrInfo(pImg, pItem);
	
	m_pImg_Item->SetData(nSlot);
	m_pImg_Item->SetDataPtr(pItem, "ptr_CECIvtrItem");
	m_pBtn_Confirm->Enable(true);
	m_iMoney = GetGameRun()->GetSpecialIDConfig().fee_pet_rename;
	SetData(nSlot);
}

bool CDlgPetReName::CheckNameValid(ACHAR* szText)
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

void CDlgPetReName::Reset()
{
	GetDlgItem("Edit_ChangeName")->SetText(_AL(""));
}



AUI_BEGIN_COMMAND_MAP(CDlgPetEquipService, CDlgItemSvrBase)

AUI_ON_COMMAND("Btn_Cancel",		OnCommandStopOpt)

AUI_END_COMMAND_MAP()

void CDlgPetEquipService::OnCommandStopOpt(const char *szCommand)
{
	GetGameRun()->PostMessage(MSG_HST_PRESSCANCEL, MAN_PLAYER, 0);
}

void CDlgPetEquipService::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if (!pImg && pItem)
	{
		if(pItem->GetClassID()==CECIvtrItem::ICID_PET_ARMOR)
			pImg = m_pImg_Item;
		else if(pItem->GetClassID()==CECIvtrItem::ICID_MATERIAL)
			pImg = (PAUIIMAGEPICTURE)GetDlgItem("Item_Tool1");
	}
	if (!pImg)
		return;

	if( 0 == stricmp(pImg->GetName(), "Img_Item") && pItem->GetClassID() == CECIvtrItem::ICID_PET_ARMOR )
	{
		OnEventLButtonDown(0, 0, pImg);
		GetGameUIMan()->SetIvtrInfo(pImg, pItem);
		pItem->Freeze(true);
		pImg->SetData(nSlot);
		pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
		CECPetArmor *pPetArmor = (CECPetArmor*)pItem;
		m_iMoney = pPetArmor->GetDBEssence()->fee_enhance;
		UpdateMoney();
		m_pBtn_Confirm->Enable(GetDlgItem("Item_Tool1")->GetDataPtr("ptr_CECIvtrItem")!=NULL);
	}
	else if( 0 == stricmp(pImg->GetName(), "Item_Tool1") )
	{
		OnEventLButtonDown(0, 0, pImg);
		GetGameUIMan()->SetIvtrInfo(pImg, pItem);
		pItem->Freeze(true);
		pImg->SetData(nSlot);
		pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
		m_pBtn_Confirm->Enable(GetDlgItem("Img_Item")->GetDataPtr("ptr_CECIvtrItem")!=NULL);
	}
}


void CDlgPetEquipService::ServerStart()
{	
	int nMoney = GetHostPlayer()->GetMoneyAmount();
	if( nMoney >= m_iMoney && GetDlgItem("Img_Item")->GetDataPtr("ptr_CECIvtrItem") && GetDlgItem("Item_Tool1")->GetDataPtr("ptr_CECIvtrItem") )
	{
		CECIvtrItem *pItem = (CECIvtrItem*)GetDlgItem("Img_Item")->GetDataPtr("ptr_CECIvtrItem");
		GetGameSession()->c2s_CmdNPCSevPetEquipEnchant(GetDlgItem("Img_Item")->GetData(), pItem->GetTemplateID(), GetDlgItem("Item_Tool1")->GetData());
		OnEventLButtonDown(0, 0, GetDlgItem("Img_Item"));
		OnEventLButtonDown(0, 0, GetDlgItem("Item_Tool1"));
		LuaEvent::FirstEnhancePetEquip();
	}
}

void CDlgPetEquipService::Reset()
{
	OnEventLButtonDown(0, 0, GetDlgItem("Img_Item"));
	OnEventLButtonDown(0, 0, GetDlgItem("Item_Tool1"));
	UpdateMoney();	
}
