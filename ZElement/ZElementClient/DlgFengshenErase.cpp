// Filename	: DlgFengshenErase.cpp
// Creator	: WuWeixin
// Date		: 2012/8/24
#include "DlgFengshenErase.h"
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

AUI_BEGIN_COMMAND_MAP(CDlgFengshenErase, CDlgBase)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Cancel",	OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Confirm",	OnCommand_confirm)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgFengshenErase, CDlgBase)
AUI_ON_EVENT("Img_*",		WM_LBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Img_*",		WM_RBUTTONDOWN,			OnEventLButtonDown)
AUI_END_EVENT_MAP()




CDlgFengshenErase::CDlgFengshenErase()
{
}

CDlgFengshenErase::~CDlgFengshenErase()
{
}

bool CDlgFengshenErase::OnInitDialog()
{
	m_pBtnConfirm = (AUIStillImageButton *)GetDlgItem("Btn_Confirm");
	m_pItem = (AUIImagePicture *)GetDlgItem("Img_Item");
	m_pTextMoney[0] = (AUILabel *)GetDlgItem("Txt_Money1");
	m_pTextMoney[1] = (AUILabel *)GetDlgItem("Txt_Money2");
	m_pTextMoney[2] = (AUILabel *)GetDlgItem("Txt_Money3");

	int fee = g_pGame->GetGameRun()->GetSpecialIDConfig().fee_xingzuo_remove;
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

void CDlgFengshenErase::OnShowDialog()
{
	m_nItemIndex = 0;
	m_idItem = 0;
}

void CDlgFengshenErase::OnTick()
{
	if (!IsShow())
		return;
	
	if (m_idItem)
	{
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(m_nItemIndex);
		if (pItem && pItem->GetClassID() == CECIvtrItem::ICID_EQUIP)
		{
			CECIvtrEquip* pEquip = (CECIvtrEquip*)pItem;
			if (pEquip->GetNewEssenceForAstrology().hole[0].astrology_id == 0)
			{
				GetGameUIMan()->MessageBox(16035);
				OnEventLButtonDown(0, 0, m_pItem);
			}
		}
	}
}

int CDlgFengshenErase::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg == "set_equip")
	{
		//wParam 包裹栏位置
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam);
		if (!pItem || pItem->GetClassID() != CECIvtrItem::ICID_EQUIP)
			return 1;

		CECIvtrEquip* pEquip = (CECIvtrEquip*)pItem;
		if (pEquip->GetNewEssenceForAstrology().hole[0].astrology_id != 0) // 已镶嵌的
		{
			GetGameUIMan()->SetIvtrInfo(m_pItem, pItem);
			m_pItem->SetData(wParam);
			if (m_pItem->GetDataPtr())
			{
				((CECIvtrItem*)m_pItem->GetDataPtr())->Freeze(false);
			}
			m_pItem->SetDataPtr(pItem);
			pItem->Freeze(true);

			m_nItemIndex = wParam;
			m_idItem = pItem->GetTemplateID();
		}
	}
	UpdateConfirmButton();
	return 0;
}

void CDlgFengshenErase::OnCommand_confirm(const char * szCommand)
{
	if (m_idItem == 0)
		return;

	if (GetHostPlayer()->GetPack()->SearchEmpty() < 0)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(16020),
				MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	if( g_pGame->GetGameRun()->GetSpecialIDConfig().fee_xingzuo_remove > GetHostPlayer()->GetMoneyAmount() )
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(16019),
				MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
	}

	
	if (g_pGame->GetGameRun()->GetHostPlayer()->IsSafeLock())
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(16036), GP_CHAT_SYSTEM);
		return;
	}

	CECIvtrItem* pItem = (CECIvtrItem*)m_pItem->GetDataPtr();
	if (!pItem)
		return;

	if (pItem->IsLocked())
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(16037), GP_CHAT_SYSTEM);
		return;
	}

	// 目前只开放一个孔
	g_pGame->GetGameSession()->c2s_CmdNPCSevAstrologyUninstall(m_idItem, m_nItemIndex, 0);
}

void CDlgFengshenErase::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(false);
	GetGameUIMan()->EndNPCService();
	OnEventLButtonDown(0, 0, m_pItem);
}

void CDlgFengshenErase::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
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

	if (pObj==m_pItem)
	{
		m_nItemIndex = 0;
		m_idItem = 0;
	}

	UpdateConfirmButton();
}

void CDlgFengshenErase::UpdateConfirmButton()
{
	m_pBtnConfirm->Enable(m_idItem != 0);
}