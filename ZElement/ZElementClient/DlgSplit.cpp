// Filename	: DlgSplit.cpp
// Creator	: Tom Zhou
// Date		: October 12, 2005

#include "DlgSplit.h"
#include "DlgInventory.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_IvtrItem.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSplit, CDlgBase)

AUI_ON_COMMAND("begin",			OnCommand_begin)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgSplit, CDlgBase)

AUI_ON_EVENT("Item_a",		WM_LBUTTONDOWN,		OnEventLButtonDown_Item_a)

AUI_END_EVENT_MAP()

CDlgSplit::CDlgSplit()
{
}

CDlgSplit::~CDlgSplit()
{
}

bool CDlgSplit::OnInitDialog()
{
	m_pTxt_no1 = (PAUILABEL)GetDlgItem("Txt_no1");
	m_pTxt_no2 = (PAUILABEL)GetDlgItem("Txt_no2");
	m_pTxt_Gold = (PAUILABEL)GetDlgItem("Txt_Gold");
	m_pPrgs_1 = (PAUIPROGRESS)GetDlgItem("Prgs_1");
	m_pImgItem_a = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
	m_pImgItem_b = (PAUIIMAGEPICTURE)GetDlgItem("Item_b");
	m_pBtn_Start = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Start");
	m_pBtn_Cancel = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Cancel");

	return true;
}

void CDlgSplit::OnCommand_begin(const char * szCommand)
{
}

void CDlgSplit::OnCommand_CANCEL(const char * szCommand)
{
	if( m_pPrgs_1->GetProgress() <= 0 )
	{
		Show(false);
		GetGameUIMan()->EndNPCService();
		GetGameUIMan()->m_pDlgInventory->Show(false);
		GetGameUIMan()->UnfreezeAllItems();
	}
	else
		GetGameSession()->c2s_CmdCancelAction();
}

void CDlgSplit::OnEventLButtonDown_Item_a(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();

	GetGameUIMan()->m_ptLButtonDown.x = GET_X_LPARAM(lParam) - p->X;
	GetGameUIMan()->m_ptLButtonDown.y = GET_Y_LPARAM(lParam) - p->Y;
	GetGameUIMan()->InvokeDragDrop(this, pObj, GetGameUIMan()->m_ptLButtonDown);
	GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
}
