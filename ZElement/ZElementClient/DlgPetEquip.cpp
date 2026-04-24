// Filename	: DlgPetEquip.cpp
// Creator	: Xiao Zhou
// Date		: 2008/4/11

#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "DlgPetEquip.h"
#include "DlgChat.h"
#include "DlgPetPackage.h"
#include "DlgPetDetail.h"
#include "DlgSkill.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrConsume.h"
#include "EC_Inventory.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgPetEquip, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",			OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgPetEquip, CDlgBase)

AUI_ON_EVENT("Img_Equip*",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_Equip*",		WM_RBUTTONDOWN,		OnEventLButtonDblclk)
AUI_ON_EVENT("Img_Equip*",		WM_LBUTTONDBLCLK,	OnEventLButtonDblclk)

AUI_END_EVENT_MAP()

CDlgPetEquip::CDlgPetEquip()
{
}

CDlgPetEquip::~CDlgPetEquip()
{
}

bool CDlgPetEquip::OnInitDialog()
{
	for(int i = 0; i < IVTRSIZE_PETEQUIP; i++ )
	{
		char szItem[20];
		sprintf(szItem, "Img_Equip%02d", i + 1);
		m_pImgEquip[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}

	return true;
}

void CDlgPetEquip::OnTick()
{
	PAUIIMAGEPICTURE pObj;
	CECIvtrItem *pItem, *pDrag = NULL;
	CECPetArmor *pEquip;
	ACHAR szText[100];
	PAUIDIALOG pDlgDrag = m_pAUIManager->GetDialog("DragDrop");

	if( pDlgDrag->IsShow() )
	{
		PAUIOBJECT pCell = pDlgDrag->GetDlgItem("Goods");
		PAUIOBJECT pObjSrc = (PAUIOBJECT)pCell->GetDataPtr("ptr_AUIObject");
		PAUIDIALOG pDlgSrc = pObjSrc->GetParent();

		if( stricmp(pDlgSrc->GetName(), "Win_Inventory") == 0 )
			pDrag = (CECIvtrItem *)pObjSrc->GetDataPtr("ptr_CECIvtrItem");
	}
	CECInventory *pInventory = NULL;
	if( GetData()>0 )
		pInventory = GetHostPlayer()->GetEPPetEquipPack();

	if( pInventory == NULL )
		pInventory = GetHostPlayer()->GetPetEquipPack();
	for(int i = 0; i < IVTRSIZE_PETEQUIP; i++ )
	{
		pObj = m_pImgEquip[i];
		if( !pObj ) continue;

		pItem = pInventory->GetItem(i);
		if( pItem )
		{
			pEquip = (CECPetArmor *)pItem;
			if( pItem->GetCount() > 1 )
			{
				a_sprintf(szText, _AL("%d"), pItem->GetCount());
				pObj->SetText(szText);
			}
			else
				pObj->SetText(_AL(""));

			AString strFile;
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
			pObj->SetDataPtr(pItem, "ptr_CECIvtrItem");
		}
		else
		{
			pObj->ClearCover();
			pObj->SetText(_AL(""));
			pObj->SetDataPtr(NULL);
			pObj->SetColor(A3DCOLORRGB(255, 255, 255));
			pObj->SetHint(GetStringFromTable(310 + i));
		}

		if( GetData()>0 )
			pObj->SetCover(NULL, -1, 1);
		else
		{
			CECIvtrItem *pPet = NULL;
			int nSlot = GetGameUIMan()->m_pDlgPetPackage->m_pLst_Pet->GetCurSel();
			if( nSlot < GetHostPlayer()->GetPetPack()->GetSize() )
				pPet = (CECPetBedge *)GetHostPlayer()->GetPetPack()->GetItem(nSlot);
			if( pDrag && (pDrag->GetClassID() == CECIvtrItem::ICID_PET_ARMOR) && ((CECPetArmor*)pDrag)->GetDBEssence()->equip_location == i )
				pObj->SetCover(GetGameUIMan()->m_pA2DSpriteMask, 0, 1);
			else
			{
				if( pPet && (GetGameUIMan()->m_pDlgPetDetail->m_PetPropadded[nSlot].equip_mask & (1 << i)) )
					pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillHilight, 0, 1);
				else
					pObj->SetCover(NULL, -1, 1);
			}
		}
	}
}

void CDlgPetEquip::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;

	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");

	if( AUI_PRESS(VK_CONTROL) || AUI_PRESS(VK_SHIFT) )
	{
		if( pItem->VerifyInfoMD5() )
		{
			CDlgChat* pChat = GetGameUIMan()->m_pDlgChat;
			PAUIEDITBOX pEdit = (PAUIEDITBOX)pChat->GetDlgItem("DEFAULT_Txt_Speech");
			ACString szName, strText;
			szName = pItem->GetName();
			strText.Format(GetStringFromTable(939), szName);

			/*
			if( pEdit->GetItemCount(enumEIIvtrlItem) < 1 )
			{
				ACString strColor = pItem->GetColor();
				if( strColor.GetLength() >= 7 && strColor[0] == '^' )
				{
					A3DCOLOR color = 0;
					for( int j = 0; j < 6; j++ )
					{
						ACHAR nValue = strColor[j + 1];
						if( nValue >= '0' && nValue <= '9' )
							nValue -= '0';
						else if( nValue >= 'a' && nValue <= 'z' )
							nValue = nValue - 'a' + 0xA;
						else if( nValue >= 'A' && nValue <= 'Z' )
							nValue = nValue - 'A' + 0xA;
						else
							nValue = 0;
						color = color * 0x10 + nValue;
					}
					color |= 0xFF000000;
					pEdit->AppendItem(enumEIIvtrlItem, color, strText, pItem->Serialize());
				}
				else
					pEdit->AppendItem(enumEIIvtrlItem, A3DCOLORRGB(255, 255, 255), strText, pItem->Serialize());
			}
			*/
			GetGameUIMan()->BringWindowToTop(pChat);
			pChat->ChangeFocus(pEdit);
		}
	}
	else if( !pItem->IsFrozen() )
	{
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();

		GetGameUIMan()->m_ptLButtonDown.x = GET_X_LPARAM(lParam) - p->X;
		GetGameUIMan()->m_ptLButtonDown.y = GET_Y_LPARAM(lParam) - p->Y;
		GetGameUIMan()->InvokeDragDrop(this, pObj, GetGameUIMan()->m_ptLButtonDown);
		GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
	}
}

void CDlgPetEquip::OnEventLButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( GetData()>0 )
		return;
	
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;
	if( AUI_PRESS(VK_CONTROL) )
		return;

	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	if( pItem )
	{
		int iSlot = atoi(pObj->GetName() + strlen("Img_Equip")) - 1;
		GetHostPlayer()->UseItemInPack(IVTRTYPE_PET_EQUIP, iSlot);
	}
}

void CDlgPetEquip::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	SetData(0);
}