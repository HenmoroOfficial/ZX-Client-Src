// Filename	: DlgFaceLift.cpp
// Creator	: Tom Zhou
// Date		: October 30, 2005

#include "DlgFaceLift.h"
#include "DlgInventory.h"
#include "EC_World.h"
#include "EC_GameUIMan.h"
#include "EC_Global.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_ManNPC.h"
#include "EC_NPC.h"
#include "EC_IvtrItem.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgFaceLift, CDlgBase)

AUI_ON_COMMAND("start",			OnCommand_start)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

CDlgFaceLift::CDlgFaceLift()
{
}

CDlgFaceLift::~CDlgFaceLift()
{
}

bool CDlgFaceLift::OnInitDialog()
{
	m_pImgItem = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");

	return true;
}

void CDlgFaceLift::OnCommand_start(const char * szCommand)
{
	int idTarget = GetHostPlayer()->GetSelectedTarget();
	CECNPC *pNPC = GetWorld()->GetNPCMan()->GetNPC(idTarget);
	if( !pNPC ) return;

	CECIvtrItem *pItem = (CECIvtrItem *)m_pImgItem->GetDataPtr("ptr_CECIvtrItem");
	if( !pItem ) return;

	if( GetHostPlayer()->IsRidingOnPet() || GetHostPlayer()->GetAttachMode() != CECPlayer::enumAttachNone )
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(688), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	
	GetGameUIMan()->UnfreezeAllItems();
	GetGameSession()->c2s_CmdNPCSevFaceChange(
		m_pImgItem->GetData(), pItem->GetTemplateID());
	GetGameUIMan()->m_pDlgInventory->Show(false);
	GetGameUIMan()->EndNPCService();
	m_pImgItem->ClearCover();
	Show(false);
}

void CDlgFaceLift::OnCommand_CANCEL(const char * szCommand)
{
	GetGameUIMan()->UnfreezeAllItems();
	GetGameUIMan()->m_pDlgInventory->Show(false);
	GetGameUIMan()->EndNPCService();
	m_pImgItem->ClearCover();
	Show(false);
}
