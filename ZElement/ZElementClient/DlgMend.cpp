// Filename	: DlgMend.cpp
// Creator	: Fu Chonggang
// Date		: 2008/12/03

#include "AFI.h"
#include "AUI\\AUIImagePicture.h"
#include "DlgMend.h"
#include "DlgInventory.h"

#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgMend, CDlgBase)

AUI_ON_COMMAND("mend",			OnCommand_Mend)
AUI_ON_COMMAND("mendMoney",		OnCommand_MendMoney)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_Cancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgMend, CDlgBase)

AUI_ON_EVENT("Item_*",	WM_LBUTTONDOWN,	OnEventLButtonDown)

AUI_END_EVENT_MAP()


CDlgMend::CDlgMend()
{
	m_pItemBroke = NULL;
	m_pItemRepair= NULL;
	m_iIndexBroke = -1;
	m_iIndexRepair= -1;
}

CDlgMend::~CDlgMend()
{
}

bool CDlgMend::OnInitDialog()
{
	m_pImg_ItemBroke = (PAUIIMAGEPICTURE)GetDlgItem("Item_1");
	m_pImg_ItemRepair = (PAUIIMAGEPICTURE)GetDlgItem("Item_2");
	return true;
}

void CDlgMend::OnShowDialog()
{
}

void CDlgMend::OnTick()
{
	if (m_szName == "Win_MendMoney")
		GetDlgItem("Btn_Confirm")->Enable(m_pItemBroke!=NULL);
	else if (m_szName == "Win_Mend")
		GetDlgItem("Btn_Confirm")->Enable(m_pItemBroke!=NULL && m_pItemRepair!=NULL);
}

void CDlgMend::OnCommand_Mend(const char* szCommand)
{
	c2s_SendCmdNPCSevRepairDamagedItem(m_iIndexBroke, m_pItemBroke->GetTemplateID());
	OnEventLButtonDown(0, 0, m_pImg_ItemBroke);
	if (m_pImg_ItemRepair)
	{
		OnEventLButtonDown(0, 0, m_pImg_ItemRepair);
	}
}

void CDlgMend::OnCommand_MendMoney(const char* szCommand)
{
	const int moneyRequire = 500000000;
	if (GetHostPlayer()->GetMoneyAmount()>=moneyRequire)
	{
		c2s_SendCmdNPCSevRepairDamagedItem2(m_iIndexBroke, m_pItemBroke->GetTemplateID());
	}
	else
		GetGameUIMan()->MessageBox(7040);
}

void CDlgMend::OnCommand_Cancel(const char* szCommand)
{
	GetGameUIMan()->EndNPCService();
	Show(false);
	GetGameUIMan()->m_pDlgInventory->Show(false);
	OnEventLButtonDown(0, 0, m_pImg_ItemBroke);
	if (m_pImg_ItemRepair)
	{
		OnEventLButtonDown(0, 0, m_pImg_ItemRepair);
	}
}

void CDlgMend::SetBrokeItem(CECIvtrItem *pItem, int nSlot)
{
	m_pItemBroke = pItem;
	m_iIndexBroke= nSlot;
	pItem->Freeze(true);
	GetGameUIMan()->SetIvtrInfo(m_pImg_ItemBroke, pItem);
	if (m_szName == "Win_MendMoney")
	{
		GetDlgItem("Btn_Confirm")->Enable(true);
	}
	else
		GetDlgItem("Btn_Confirm")->Enable(m_pItemRepair!=NULL);

}

void CDlgMend::SetRepairItem(CECIvtrItem *pItem, int nSlot)
{
	m_pItemRepair = pItem;
	m_iIndexRepair= nSlot;
	pItem->Freeze(true);
	GetGameUIMan()->SetIvtrInfo(m_pImg_ItemRepair, pItem);
	GetDlgItem("Btn_Confirm")->Enable(m_pItemBroke!=NULL);
}


void CDlgMend::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if (pObj == m_pImg_ItemBroke )
	{
		if( m_pItemBroke )
		{
			m_pItemBroke->Freeze(false);
			m_pItemBroke = NULL;
			m_iIndexBroke= -1;
			m_pImg_ItemBroke->SetCover(NULL, -1);
			m_pImg_ItemBroke->SetHint(_AL(""));
			GetDlgItem("Btn_Confirm")->Enable(false);
		}
	}
	else if (pObj == m_pImg_ItemRepair )
	{
		if( m_pItemRepair )
		{
			m_pItemRepair->Freeze(false);
			m_pItemRepair = NULL;
			m_iIndexRepair= -1;
			m_pImg_ItemRepair->SetCover(NULL, -1);
			m_pImg_ItemRepair->SetHint(_AL(""));
			GetDlgItem("Btn_Confirm")->Enable(false);
		}
	}
}