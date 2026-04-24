// File		: DlgResetProp.cpp
// Creator	: Xiao Zhou
// Date		: 2006/1/12

#include "AFI.h"
#include "DlgResetProp.h"
#include "DlgInventory.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_IvtrItem.h"
#include "EC_HostPlayer.h"
#include "EC_GameSession.h"
#include "elementdataman.h"
#include "EC_Inventory.h"
#include "LuaEvent.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgResetProp, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommandConfirm)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgResetProp, CDlgBase)

AUI_ON_EVENT("Item_01",	WM_LBUTTONDOWN,	OnEventLButtonDown)

AUI_END_EVENT_MAP()


CDlgResetProp::CDlgResetProp()
{
}

CDlgResetProp::~CDlgResetProp()
{
}


void CDlgResetProp::OnCommandConfirm(const char *szCommand)
{
	CECIvtrItem *pItem = (CECIvtrItem *)GetDataPtr();
	if( pItem )
	{
		pItem->Freeze(false);
		GetGameSession()->c2s_CmdNPCSevReturnStatusPt(m_nPropEntryIndex, pItem->GetTemplateID());
		SetDataPtr(NULL);
		LuaEvent::FirstClearSkill();

		Show(false);
		GetGameUIMan()->EndNPCService();
		GetGameUIMan()->m_pDlgInventory->Show(false);
		GetGameUIMan()->UnfreezeAllItems();
	}
}

void CDlgResetProp::OnCommandCancel(const char *szCommand)
{
	OnEventLButtonDown(0, 0, NULL);
	Show(false);
	GetGameUIMan()->EndNPCService();
	GetGameUIMan()->m_pDlgInventory->Show(false);
	GetGameUIMan()->UnfreezeAllItems();
}

bool CDlgResetProp::OnInitDialog()
{
	m_pImg_Item = (PAUIIMAGEPICTURE)GetDlgItem("Item_01");
	m_pBtn_Confirm = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Reset");
	m_nPropEntryIndex = -1;
	OnEventLButtonDown(0, 0, NULL);

	return true;
}

void CDlgResetProp::OnShowDialog()
{
	OnEventLButtonDown(0, 0, NULL);
}

void CDlgResetProp::OnTick()
{
	m_pBtn_Confirm->Enable(GetDataPtr()!=0);
}

void CDlgResetProp::SetScroll(CECIvtrItem *pItem)
{
	DATA_TYPE DataType;
	NPC_RESETPROP_SERVICE *pData = (NPC_RESETPROP_SERVICE *)GetGame()->GetElementDataMan()->
		get_data_ptr(GetGameUIMan()->m_pCurNPCEssence->id_resetprop_service, ID_SPACE_ESSENCE, DataType);
	if( DataType != DT_NPC_RESETPROP_SERVICE ) return;

	int i = GetData( );
	if ( i >= 3 )
		return;

	int resetprop_type = 0;
	if ( i == 0 )
		resetprop_type = 1;
	else if ( i == 1 )
		resetprop_type = 0;
	else if ( i == 2 )
		resetprop_type = 2;

	bool needObj = false;
	for (int i = 0; i < 6; ++ i)
	{
		if ( resetprop_type == pData->prop_entry[i].resetprop_type && pItem->GetTemplateID() == pData->prop_entry[i].id_object_need )
		{
			m_nPropEntryIndex = i;
			needObj = true;
			break;
		}
	}

	if ( !needObj )
		return;

	OnEventLButtonDown(0, 0, NULL);

	/*
	int n = 0;
	for(i = 0; i < 6; i++)
	{
		if( pData->prop_entry[i].id_object_need && 
			pItem->GetTemplateID() == pData->prop_entry[i].id_object_need )
		{
			ACString szProp = _AL("");
			
			if( pData->prop_entry[i].resetprop_type == 0 )
			{
				szProp = GetStringFromTable(371);
			}
			else if( pData->prop_entry[i].resetprop_type == 1 )
			{
				szProp = GetStringFromTable(372);
			}
			else if( pData->prop_entry[i].resetprop_type == 2 )
			{
				szProp = GetStringFromTable(373);
			}
			
			n++;
			if(n == 5)
				break;
		}
	}
	*/

	AString strFile;
	af_GetFileTitle(pItem->GetIconFile(), strFile);
	strFile.MakeLower();
	m_pImg_Item->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
	m_pImg_Item->SetDataPtr(pItem, "ptr_CECIvtrItem");
	SetDataPtr(pItem);
	pItem->Freeze(true);
}

void CDlgResetProp::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	CECIvtrItem *pItem = (CECIvtrItem *)GetDataPtr();
	if( pItem )
		pItem->Freeze(false);
	m_pImg_Item->ClearCover();
	m_pImg_Item->SetHint(_AL(""));
	m_pImg_Item->SetDataPtr(NULL);
	SetDataPtr(NULL);
}