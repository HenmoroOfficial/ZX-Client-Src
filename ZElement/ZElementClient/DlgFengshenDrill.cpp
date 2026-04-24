// Filename	: DlgFengshenDrill.cpp
// Creator	: WuWeixin
// Date		: 2012/8/24

#include "DlgFengshenDrill.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"
#include "EC_IvtrEquip.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_GameUIMan.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"

AUI_BEGIN_COMMAND_MAP(CDlgFengshenDrill, CDlgBase)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Confirm",	OnCommand_confirm)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgFengshenDrill, CDlgBase)
AUI_ON_EVENT("Img_*",		WM_LBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Img_*",		WM_RBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Item_*",		WM_LBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Item_*",		WM_RBUTTONDOWN,			OnEventLButtonDown)
AUI_END_EVENT_MAP()


static const int EQUIP_LEVEL_LIMIT = 60; // 允许打孔的最低等级


CDlgFengshenDrill::CDlgFengshenDrill()
{
}

CDlgFengshenDrill::~CDlgFengshenDrill()
{
}

bool CDlgFengshenDrill::OnInitDialog()
{
	m_pBtnConfirm = (AUIStillImageButton *)GetDlgItem("Btn_Confirm");
	m_pItem1 = (AUIImagePicture *)GetDlgItem("Img_Item");
	m_pItem2 = (AUIImagePicture *)GetDlgItem("Item_d");

	m_pBtnConfirm->Enable(false);

	return true;
}

void CDlgFengshenDrill::OnShowDialog()
{
	m_nItemIndex = 0;
	m_idItem = 0;
	m_nStoneIndex = 0;
	m_idStone = 0;
}

int CDlgFengshenDrill::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg == "set_equip")
	{
		//wParam 包裹栏位置
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam);
		if (pItem && pItem->GetClassID() == CECIvtrItem::ICID_EQUIP)
		{
			CECIvtrEquip* pEquip = (CECIvtrEquip*)pItem;
			if (pEquip->GetDBEssence()->require_level >= EQUIP_LEVEL_LIMIT
				&& pEquip->GetNewEssenceForAstrology().curr_hole < 1) // 目前只开一个孔
			{
				GetGameUIMan()->SetIvtrInfo(m_pItem1, pItem);
				m_pItem1->SetData(wParam);
				if (m_pItem1->GetDataPtr())
				{
					((CECIvtrItem*)m_pItem1->GetDataPtr())->Freeze(false);
				}
				m_pItem1->SetDataPtr(pItem);
				pItem->Freeze(true);

				m_nItemIndex = wParam;
				m_idItem = pItem->GetTemplateID();

				return 0;
			}
		}
		return 1;
	}
	else if (strMsg == "set_props")
	{
		if (m_pItem1->GetDataPtr()==NULL)
		{
			GetGameUIMan()->MessageBox(16017);
			return 1;
		}

		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam);
		if (!pItem)
			return 1;

		bool bHoleProp = false;
		for (int i = 0; i < 3; ++ i)
		{
			if (pItem->GetTemplateID() == g_pGame->GetGameRun()->GetSpecialIDConfig().id_equip_hole[i])
			{
				bHoleProp = true;
				break;
			}
		}

		if (!bHoleProp)
			return 1;

		pItem->Freeze(true);
		m_pItem2->SetData(wParam);
		if (m_pItem2->GetDataPtr())
		{
			((CECIvtrItem*)m_pItem2->GetDataPtr())->Freeze(false);
		}
		m_pItem2->SetDataPtr(pItem);
		GetGameUIMan()->SetIvtrInfo(m_pItem2, pItem);
	
		m_nStoneIndex = wParam;
		m_idStone = pItem->GetTemplateID();
	}

	UpdateConfirmButton();
	
	return 0;
}

void CDlgFengshenDrill::OnCommand_confirm(const char * szCommand)
{
	if (m_idItem == 0 || m_idStone == 0)
		return;

	if (CECIvtrEquip* pEquip = (CECIvtrEquip*)m_pItem1->GetDataPtr())
	{
		bool bEquipBound = !!(pEquip->GetStateFlag() & ITEM_STATE_BINDED);
		if (!bEquipBound) // 非绑定装备要提示
		{
			GetGameUIMan()->MessageBox("Game_FengshenDrill", GetStringFromTable(16018),
						MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
		}
		else
		{
			SendCmd();
		}
	}
}

void CDlgFengshenDrill::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(false);
	GetGameUIMan()->EndNPCService();
	OnEventLButtonDown(0, 0, m_pItem1);
}

void CDlgFengshenDrill::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pObj;
	if (pObj->GetDataPtr())
	{
		((CECIvtrItem*)pObj->GetDataPtr())->Freeze(false);
	}
	pImg->SetCover(NULL, -1, 0);
	pImg->SetHint(_AL(""));
	pImg->SetData(-1);
	pImg->SetDataPtr(NULL);

	if (pObj==m_pItem1)
	{
		m_nItemIndex = 0;
		m_idItem = 0;
		OnEventLButtonDown(0, 0, m_pItem2);
	}
	else if (pObj==m_pItem2)
	{
		m_nStoneIndex = 0;
		m_idStone = 0;
	}
	UpdateConfirmButton();
}

void CDlgFengshenDrill::UpdateConfirmButton()
{
	m_pBtnConfirm->Enable(!(m_idItem == 0 || m_idStone == 0));
}

void CDlgFengshenDrill::SendCmd()
{
	if (m_idItem == 0 || m_idStone == 0)
		return;

	g_pGame->GetGameSession()->c2s_CmdNPCSevEquipSlot(m_idItem, m_nItemIndex, m_idStone, m_nStoneIndex);

	OnCommand_CANCEL(NULL);
}