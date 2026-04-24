// Filename	: DlgFengshenAttach.cpp
// Creator	: WuWeixin
// Date		: 2012/8/24

#include "DlgFengshenAttach.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "EC_IvtrEquip.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_GameUIMan.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_IvtrConsume.h"

AUI_BEGIN_COMMAND_MAP(CDlgFengshenAttach, CDlgBase)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Cancel",	OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Confirm",	OnCommand_confirm)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgFengshenAttach, CDlgBase)
AUI_ON_EVENT("Img_*",		WM_LBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Img_*",		WM_RBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Item_*",		WM_LBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Item_*",		WM_RBUTTONDOWN,			OnEventLButtonDown)
AUI_END_EVENT_MAP()




CDlgFengshenAttach::CDlgFengshenAttach()
{
}

CDlgFengshenAttach::~CDlgFengshenAttach()
{
}

bool CDlgFengshenAttach::OnInitDialog()
{
	m_pBtnConfirm = (AUIStillImageButton *)GetDlgItem("Btn_Confirm");
	m_pItem1 = (AUIImagePicture *)GetDlgItem("Img_Item");
	m_pItem2 = (AUIImagePicture *)GetDlgItem("Item_d");
	m_pTextMoney[0] = (AUILabel *)GetDlgItem("Txt_Money1");
	m_pTextMoney[1] = (AUILabel *)GetDlgItem("Txt_Money2");
	m_pTextMoney[2] = (AUILabel *)GetDlgItem("Txt_Money3");

	int fee = g_pGame->GetGameRun()->GetSpecialIDConfig().fee_xingzuo_add;
	int split_fee[3];
	split_fee[0] = (fee / 10000);
	split_fee[1] = (fee / 100) % 100;
	split_fee[2] = fee % 100;
	
	for (int i = 0; i < 3; ++ i)
	{
		ACString szText;
		szText.Format(_AL("%d"), split_fee[i]);
		m_pTextMoney[i]->SetText(szText);
	}

	m_pBtnConfirm->Enable(false);

	return true;
}

void CDlgFengshenAttach::OnShowDialog()
{
	m_nItemIndex = 0;
	m_idItem = 0;
	m_nStoneIndex = 0;
	m_idStone = 0;
	m_equipMask = 0;
}

void CDlgFengshenAttach::OnTick()
{
	if (!IsShow())
		return;
	
	if (m_idItem)
	{
		if (CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(m_nItemIndex))
		{
			if (pItem->GetClassID() == CECIvtrItem::ICID_EQUIP)
			{
				CECIvtrEquip* pEquip = (CECIvtrEquip*)pItem;
				if (pEquip->GetNewEssenceForAstrology().hole[0].astrology_id != 0)
				{
					GetGameUIMan()->MessageBox(16034);
					OnEventLButtonDown(0, 0, m_pItem1);
				}
			}
		}
	}
}

int CDlgFengshenAttach::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg == "set_equip")
	{
		//wParam 包裹栏位置
		if (CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam))
		{
			if (pItem->GetClassID() == CECIvtrItem::ICID_EQUIP)
			{
				CECIvtrEquip* pEquip = (CECIvtrEquip*)pItem;
				if (pEquip->GetNewEssenceForAstrology().curr_hole > 0 &&
					pEquip->GetNewEssenceForAstrology().hole[0].astrology_id == 0) // 已打孔且未镶嵌的
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

					m_equipMask = pEquip->GetDBEssence()->equip_mask;

					return 0;
				}
			}
		}
		return 1;
	}
	else if (strMsg == "set_props")
	{
		CECIvtrEquip* pEquip = (CECIvtrEquip*)m_pItem1->GetDataPtr();
		if (pEquip==NULL)
		{
			GetGameUIMan()->MessageBox(16038);
			return 1;
		}

		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam);
		if (!pItem || pItem->GetClassID() != CECIvtrItem::ICID_ASTROLOGY)
			return 1;

		CECIvtrAstrology* pAstrology = (CECIvtrAstrology*)pItem;
		if (pAstrology->GetEssence().level < 0) // 未鉴定
			return 1;
		
		if ( (pAstrology->GetDBEssence()->equip_mask & m_equipMask) == 0 ) // 装备与星座镶嵌部位不匹配
			return 1;

		if (CECIvtrAstrology::GetEquipLevelLimit(pAstrology->GetEssence().level) > pEquip->GetLevelRequirement())
		{
			GetGameUIMan()->MessageBox(16033);
			return 1;
		}

		if (pEquip->GetDBEssence()->renascence_count == 0 && pAstrology->GetDBEssence()->reborn_count > 0)
		{
			GetGameUIMan()->MessageBox(16031);
			return 1;
		}

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

void CDlgFengshenAttach::OnCommand_confirm(const char * szCommand)
{
	if (m_idItem == 0 || m_idStone == 0)
		return;

	if( g_pGame->GetGameRun()->GetSpecialIDConfig().fee_xingzuo_add > GetHostPlayer()->GetMoneyAmount() )
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(16019),
				MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
	}

	// 目前只开放一个孔
	g_pGame->GetGameSession()->c2s_CmdNPCSevAstrologyInstall(m_idItem, m_nItemIndex, m_idStone, m_nStoneIndex, 0);

	//OnCommand_CANCEL(NULL);
}

void CDlgFengshenAttach::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(false);
	GetGameUIMan()->EndNPCService();
	OnEventLButtonDown(0, 0, m_pItem1);
}

void CDlgFengshenAttach::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
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

void CDlgFengshenAttach::UpdateConfirmButton()
{
	m_pBtnConfirm->Enable(!(m_idItem == 0 || m_idStone == 0));
}