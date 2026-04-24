// Filename	: DlgIdentify.cpp
// Creator	: Tom Zhou
// Date		: October 11, 2005

#include "DlgIdentify.h"
#include "DlgInventory.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_IvtrEquip.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgIdentify, CDlgBase)

AUI_ON_COMMAND("start",			OnCommand_start)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgIdentify, CDlgBase)

AUI_ON_EVENT("Item",		WM_LBUTTONDOWN,		OnEventLButtonDown_Item)

AUI_END_EVENT_MAP()

CDlgIdentify::CDlgIdentify()
{
}

CDlgIdentify::~CDlgIdentify()
{
}

bool CDlgIdentify::OnInitDialog()
{
	m_pImgItem = (PAUIIMAGEPICTURE)GetDlgItem("Item");
	m_pBtn_Confirm = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Confirm");

	return true;
}

void CDlgIdentify::OnCommand_start(const char * szCommand)
{
	int iSrc = int(m_pImgItem->GetData());
	CECIvtrItem *pIvtr = (CECIvtrItem *)m_pImgItem->GetDataPtr("ptr_CECIvtrItem");
	if( !pIvtr ) return;
	int idItem = pIvtr->GetTemplateID();
	
	if( pIvtr->IsEquipment() && false /*((CECIvtrEquip *)pIvtr)->HasRandomProp()*/ )
	{
		GetGameSession()->c2s_CmdNPCSevIdentify(iSrc, idItem);
		m_pBtn_Confirm->Enable(false);
	}
	else
	{
		m_pAUIManager->MessageBox("", m_pAUIManager->GetStringFromTable(504),
			MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
}

void CDlgIdentify::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->EndNPCService();
	GetGameUIMan()->m_pDlgInventory->Show(false);
	GetGameUIMan()->UnfreezeAllItems();
}

void CDlgIdentify::OnEventLButtonDown_Item(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();

	GetGameUIMan()->m_ptLButtonDown.x = GET_X_LPARAM(lParam) - p->X;
	GetGameUIMan()->m_ptLButtonDown.y = GET_Y_LPARAM(lParam) - p->Y;
	GetGameUIMan()->InvokeDragDrop(this, pObj, GetGameUIMan()->m_ptLButtonDown);
	GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
}
