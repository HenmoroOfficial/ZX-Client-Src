// Filename	:	DlgMarkAstro.cpp
// Creator	:	Fu Chonggang
// Date		:	Apr 23, 2010

#include "DlgMarkAstro.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"

#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_GameSession.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgMarkAstro, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_ON_COMMAND("start",					OnCommand_start)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgMarkAstro, CDlgBase)

AUI_ON_EVENT("Item_a", WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_d", WM_LBUTTONDOWN,	OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgMarkAstro::CDlgMarkAstro()
{
}

CDlgMarkAstro::~CDlgMarkAstro()
{
}

bool CDlgMarkAstro::Release()
{
	return CDlgBase::Release();
}

bool CDlgMarkAstro::OnInitDialog()
{
	m_pTitle = (AUILabel *)GetDlgItem("Title");
	m_pItem_a = (AUIImagePicture *)GetDlgItem("Item_a");
	m_pItem_d = (AUIImagePicture *)GetDlgItem("Item_d");
	m_pBtn_Charge = (AUIStillImageButton *)GetDlgItem("Btn_Charge");
	m_pLab_Hint = (AUILabel *)GetDlgItem("Lab_Hint");
	m_pLab_Astrolabe = (AUILabel *)GetDlgItem("Lab_Astrolabe");
	m_pLab_Medium = (AUILabel *)GetDlgItem("Lab_Medium");
	return true;
}

void CDlgMarkAstro::OnShowDialog()
{
}

void CDlgMarkAstro::OnTick()
{
	m_pBtn_Charge->Enable(m_pItem_a->GetDataPtr("ptr_CECIvtrItem") && m_pItem_d->GetDataPtr("ptr_CECIvtrItem"));
}

bool CDlgMarkAstro::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

int CDlgMarkAstro::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	return 1;
}

void CDlgMarkAstro::OnCommand_CANCEL(const char * szCommand)
{
	GetGameUIMan()->EndNPCService();
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(false);
	OnEventLButtonDown(0, 0, m_pItem_a);
	OnEventLButtonDown(0, 0, m_pItem_d);
	Show(false);
}

void CDlgMarkAstro::OnCommand_start(const char * szCommand)
{
	int item_index	= m_pItem_a->GetData();
	int item_id		= GetHostPlayer()->GetPack()->GetItem(item_index)->GetTemplateID();
	int stone_index	= m_pItem_d->GetData();
	int stone_id	= GetHostPlayer()->GetPack()->GetItem(stone_index)->GetTemplateID();

	GetGameSession()->c2s_CmdNPCSevTeleportationCharge(item_index, item_id, stone_index, stone_id);
	OnEventLButtonDown(0, 0, m_pItem_a);
	OnEventLButtonDown(0, 0, m_pItem_d);
}

void CDlgMarkAstro::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (pObj->GetDataPtr("ptr_CECIvtrItem"))
	{
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pObj;
		pImg->ClearCover();
		pImg->SetData(-1);
		CECIvtrItem *pIvtrSrc = (CECIvtrItem*)pObj->GetDataPtr("ptr_CECIvtrItem");
		pIvtrSrc->Freeze(false);
		pImg->SetDataPtr(NULL, "ptr_CECIvtrItem");
	}
}

void CDlgMarkAstro::DragDropItem(int iSrc, PAUIOBJECT pObjOver)
{
	if( !pObjOver )
		return;
	
	CECIvtrItem *pIvtrSrc = GetHostPlayer()->GetPack()->GetItem(iSrc);
	
	if( 0 == stricmp(pObjOver->GetName(), "Item_a") && pIvtrSrc->GetClassID() != CECIvtrItem::ICID_TELEPORTATION )
		return;
	
	if( 0 == stricmp(pObjOver->GetName(), "Item_d") && pIvtrSrc->GetClassID() != CECIvtrItem::ICID_TELEPORTATION_STONE )
		return;
	
	OnEventLButtonDown(0, 0, pObjOver);
	GetGameUIMan()->SetIvtrInfo((PAUIIMAGEPICTURE)pObjOver, pIvtrSrc);
	
	pIvtrSrc->Freeze(true);
	pObjOver->SetData(iSrc);
	pObjOver->SetDataPtr(pIvtrSrc, "ptr_CECIvtrItem");
}