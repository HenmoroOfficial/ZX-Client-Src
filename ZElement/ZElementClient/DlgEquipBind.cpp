// File		: DlgEquipBind.cpp
// Creator	: Xiao Zhou
// Date		: 2006/7/19

#include "AFI.h"
#include "DlgEquipBind.h"
#include "DlgInventory.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "EC_Inventory.h"
#include "EC_IvtrEquip.h"
#include "LuaEvent.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgEquipBind, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommandConfirm)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgEquipBind, CDlgBase)

AUI_ON_EVENT("Img_Item",	WM_LBUTTONDOWN,	OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgEquipBind::CDlgEquipBind()
{
	m_pTxt_Name = NULL;
	m_pImg_Item = NULL;
	m_pBtn_Confirm = NULL;
}

CDlgEquipBind::~CDlgEquipBind()
{
}

bool CDlgEquipBind::OnInitDialog()
{
	DDX_Control("Txt_Name", m_pTxt_Name);
	DDX_Control("Img_Item", m_pImg_Item);
	DDX_Control("Btn_Confirm", m_pBtn_Confirm);

	return true;
}

void CDlgEquipBind::OnCommandConfirm(const char *szCommand)
{
	CECIvtrEquip* pEquip = (CECIvtrEquip*)GetDataPtr();
	if( pEquip ) 
	{
		GetGameSession()->c2s_CmdNPCSevBindItem(m_nSolt, pEquip->GetTemplateID());
		OnEventLButtonDown(0, 0, NULL);
		LuaEvent::FirstBindEquip();
	}
}

void CDlgEquipBind::OnCommandCancel(const char *szCommand)
{
	OnEventLButtonDown(0, 0, NULL);
	GetGameUIMan()->EndNPCService();
	Show(false);
	GetGameUIMan()->m_pDlgInventory->Show(false);
	GetGameUIMan()->UnfreezeAllItems();
}

void CDlgEquipBind::OnShowDialog()
{
	OnEventLButtonDown(0, 0, NULL);
}

void CDlgEquipBind::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	CECIvtrItem *pItem = (CECIvtrItem *)GetDataPtr();
	if( pItem )
		pItem->Freeze(false);
	m_pImg_Item->ClearCover();
	m_pImg_Item->SetHint(_AL(""));
	m_pTxt_Name->SetText(_AL(""));
	SetDataPtr(NULL);
}

void CDlgEquipBind::SetEquip(CECIvtrItem *pItem, int nSlot)
{
	CECIvtrEquip* pEquip = (CECIvtrEquip*)pItem;
	OnEventLButtonDown(0, 0, NULL);
	SetDataPtr(pEquip);
	m_nSolt = nSlot;
	pItem->Freeze(true);
	AString strFile;
	af_GetFileTitle(pEquip->GetIconFile(), strFile);
	strFile.MakeLower();
	m_pImg_Item->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
	m_pTxt_Name->SetText(pEquip->GetName());
}
