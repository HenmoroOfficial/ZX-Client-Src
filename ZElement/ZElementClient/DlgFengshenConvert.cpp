// Filename	: DlgFengshenConvert.cpp
// Creator	: WuWeixin
// Date		: 2012/8/24

#include "DlgFengshenConvert.h"
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


AUI_BEGIN_COMMAND_MAP(CDlgFengshenConvert, CDlgBase)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Cancel",	OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Confirm",	OnCommand_confirm)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgFengshenConvert, CDlgBase)
AUI_ON_EVENT("Img_*",		WM_LBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Img_*",		WM_RBUTTONDOWN,			OnEventLButtonDown)
AUI_END_EVENT_MAP()

const int CDlgFengshenConvert::ms_nMaxEnergy = 100000;

static const int PRECIOUS_SCORE = 300; 

CDlgFengshenConvert::CDlgFengshenConvert()
{
}

CDlgFengshenConvert::~CDlgFengshenConvert()
{
}

bool CDlgFengshenConvert::OnInitDialog()
{
	m_pBtnConfirm = (AUIStillImageButton *)GetDlgItem("Btn_Confirm");
	m_pItem = (AUIImagePicture *)GetDlgItem("Img_Item");
	m_pTextNum = (AUILabel *)GetDlgItem("Txt_Num");

	return true;
}

void CDlgFengshenConvert::OnShowDialog()
{
	m_nItemIndex = 0;
	m_idItem = 0;
}

void CDlgFengshenConvert::OnTick()
{
	if (!IsShow())
		return;

	int energy = GetHostPlayer()->GetAstrologyEnergy();
	ACString szText;
	szText.Format(_AL("%d"), energy);
	m_pTextNum->SetText(szText);

	if (m_idItem != 0)
	{
		CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(m_nItemIndex);
		if(!pItem)
		{
			PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)m_pItem;
			pImg->SetCover(NULL, -1, 0);
			pImg->SetHint(_AL(""));
			pImg->SetData(-1);
			pImg->SetDataPtr(NULL);
			m_nItemIndex = 0;
			m_idItem = 0;
			UpdateConfirmButton();
		}
	}
}

int CDlgFengshenConvert::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg == "set_equip")
	{
		//wParam °ü¹üÀ¸Î»ÖÃ
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam);
		if (!pItem || pItem->GetClassID() != CECIvtrItem::ICID_ASTROLOGY)
			return 1;

		CECIvtrAstrology* pAstrology = (CECIvtrAstrology*)pItem;
		if (pAstrology->GetEssence().level >= 0)
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

void CDlgFengshenConvert::OnCommand_confirm(const char * szCommand)
{
	if (m_idItem == 0)
		return;

	CECHostPlayer* pHostPlayer = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHostPlayer->GetAstrologyEnergy() >= ms_nMaxEnergy)
	{
		GetGameUIMan()->MessageBox(16026);
		return;
	}

	if (pHostPlayer->IsSafeLock())
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
	
	if (CECIvtrAstrology* pAStrology = (CECIvtrAstrology*)pItem)
	{
		if (pAStrology->GetEssence().score >= PRECIOUS_SCORE)
		{
			GetGameUIMan()->MessageBox("Game_AstrologyDestory", GetStringFromTable(10627), MB_OKCANCEL);
			return;
		}
	}
	
	SendCmd();
}

void CDlgFengshenConvert::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	OnEventLButtonDown(0, 0, m_pItem);
}

void CDlgFengshenConvert::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
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

void CDlgFengshenConvert::UpdateConfirmButton()
{
	m_pBtnConfirm->Enable(m_idItem != 0);
}

void CDlgFengshenConvert::SendCmd()
{
	g_pGame->GetGameSession()->c2s_CmdAstrologyDestory(m_idItem, m_nItemIndex);
}