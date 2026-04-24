// Filename	: DlgFengshenIdentify.cpp
// Creator	: WuWeixin
// Date		: 2012/8/24

#include "DlgFengshenIdentify.h"
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

AUI_BEGIN_COMMAND_MAP(CDlgFengshenIdentify, CDlgBase)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Cancel",	OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Confirm",	OnCommand_confirm)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgFengshenIdentify, CDlgBase)
AUI_END_EVENT_MAP()





CDlgFengshenIdentify::CDlgFengshenIdentify()
{
}

CDlgFengshenIdentify::~CDlgFengshenIdentify()
{
}

bool CDlgFengshenIdentify::OnInitDialog()
{
	m_pBtnConfirm = (AUIStillImageButton *)GetDlgItem("Btn_Confirm");
	m_pItem = (AUIImagePicture *)GetDlgItem("Img_Item");
	m_pTextMoney[0] = (AUILabel *)GetDlgItem("Txt_Money1");
	m_pTextMoney[1] = (AUILabel *)GetDlgItem("Txt_Money2");
	m_pTextMoney[2] = (AUILabel *)GetDlgItem("Txt_Money3");

	int fee = g_pGame->GetGameRun()->GetSpecialIDConfig().fee_xingzuo_identify;
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

void CDlgFengshenIdentify::OnHideDialog()
{
	if (m_pItem->GetDataPtr())
	{
		((CECIvtrItem*)m_pItem->GetDataPtr())->Freeze(false);
		m_pItem->SetDataPtr(NULL);
	}
	m_nItemIndex = 0;
	m_idItem = 0;
}

void CDlgFengshenIdentify::OnTick()
{
	if (!m_idItem)
		return;

	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(m_nItemIndex);
	if(!pItem)
	{
		Show(false);
		return;
	}

	CECIvtrAstrology *pAstrology = (CECIvtrAstrology*)pItem;
	if (pAstrology->GetEssence().level >= 0)
	{
		Show(false);
		pItem->Freeze(false);
	}
}

void CDlgFengshenIdentify::SetAstrology(int nItemIndex)
{
	//wParam 包裹栏位置
	CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(nItemIndex);
	if (!pItem || pItem->GetClassID() != CECIvtrItem::ICID_ASTROLOGY)
		return;

	CECIvtrAstrology* pAstrology = (CECIvtrAstrology*)pItem;
	if (pAstrology->GetEssence().level < 0) // 未鉴定的服务器不会发essence
	{
		GetGameUIMan()->SetIvtrInfo(m_pItem, pItem);
		m_pItem->SetData(nItemIndex);
		if (m_pItem->GetDataPtr())
		{
			((CECIvtrItem*)m_pItem->GetDataPtr())->Freeze(false);
		}
		m_pItem->SetDataPtr(pItem);
		pItem->Freeze(true);

		m_nItemIndex = nItemIndex;
		m_idItem = pItem->GetTemplateID();

		Show(true);
	}
	UpdateConfirmButton();
}

void CDlgFengshenIdentify::OnCommand_confirm(const char * szCommand)
{
	if (m_idItem == 0)
		return;

	if( g_pGame->GetGameRun()->GetSpecialIDConfig().fee_xingzuo_identify > GetHostPlayer()->GetMoneyAmount() )
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(16019),
				MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
	}

	g_pGame->GetGameSession()->c2s_CmdAstrologyIdentify(m_idItem, m_nItemIndex);
}

void CDlgFengshenIdentify::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(false);
}

void CDlgFengshenIdentify::UpdateConfirmButton()
{
	m_pBtnConfirm->Enable(m_idItem != 0);
}