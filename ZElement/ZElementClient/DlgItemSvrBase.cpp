// File		: DlgEquipBind.cpp
// Creator	: Xiao Zhou
// Date		: 2006/7/19

#include "AFI.h"
#include "AUI\\AUICTranslate.h"
#include "DlgItemSvrBase.h"
#include "DlgInventory.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "EC_Inventory.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrStone.h"
#include "LuaEvent.h"
#include "EC_NewHelpGuide.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgItemSvrBase, CDlgBase)

AUI_ON_COMMAND("Btn_Confirm",		OnCommandConfirm)
AUI_ON_COMMAND("Btn_Enchase",		OnCommandConfirm)
AUI_ON_COMMAND("start",				OnCommandConfirm)
AUI_ON_COMMAND("Btn_Cancel",		OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgItemSvrBase, CDlgBase)

AUI_ON_EVENT("Img_*",	WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Img_*",	WM_RBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_*",	WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_*",	WM_RBUTTONDOWN,	OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgItemSvrBase::CDlgItemSvrBase()
{
	m_pImg_Item		= NULL;
	m_pBtn_Confirm	= NULL;
	m_pTxt_Bronze	= NULL;
	m_pTxt_Silver	= NULL;
	m_pTxt_Gold		= NULL;
}

CDlgItemSvrBase::~CDlgItemSvrBase()
{
}

bool CDlgItemSvrBase::OnInitDialog()
{
	m_pImg_Item = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
	if (!m_pImg_Item)
		m_pImg_Item = (PAUIIMAGEPICTURE)GetDlgItem("Item_1");
	m_pBtn_Confirm = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Confirm");
	if (!m_pBtn_Confirm)
		m_pBtn_Confirm = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Enchase");
	m_pTxt_Bronze = (PAUILABEL)GetDlgItem("Txt_Bronze");
	if (!m_pTxt_Bronze)
		m_pTxt_Bronze = (PAUILABEL)GetDlgItem("Txt_Money3");
	m_pTxt_Silver = (PAUILABEL)GetDlgItem("Txt_Silver");
	if (!m_pTxt_Silver)
		m_pTxt_Silver = (PAUILABEL)GetDlgItem("Txt_Money2");
	m_pTxt_Gold = (PAUILABEL)GetDlgItem("Txt_Gold");
	if (!m_pTxt_Gold)
		m_pTxt_Gold = (PAUILABEL)GetDlgItem("Txt_Money1");

	m_iMoney = 0;

	return true;
}

void CDlgItemSvrBase::OnShowDialog()
{
	Reset();
}

void CDlgItemSvrBase::OnTick()
{
	PAUIDIALOG pDlgMouseOn;
	PAUIOBJECT pObjMouseOn;
	GetGameUIMan()->GetMouseOn(&pDlgMouseOn, &pObjMouseOn);
	if ( pDlgMouseOn && pDlgMouseOn==this && pObjMouseOn &&
		pObjMouseOn->GetType()==AUIOBJECT_TYPE_IMAGEPICTURE && pObjMouseOn->GetDataPtr("ptr_CECIvtrItem"))
	{
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(pObjMouseOn->GetData());
		if (pItem)
		{
			AUICTranslate trans;
			const wchar_t* szDesc = pItem->GetDesc();
			if( szDesc )
				pObjMouseOn->SetHint(trans.Translate(szDesc));
			else
				pObjMouseOn->SetHint(pItem->GetName());
		}
		else
		{
			pObjMouseOn->SetData(-1);
			pObjMouseOn->SetDataPtr(0, "ptr_CECIvtrItem");
			((PAUIIMAGEPICTURE)pObjMouseOn)->ClearCover();
			pObjMouseOn->SetText(_AL(""));
			pObjMouseOn->SetHint(_AL(""));
			UpdateDisplay();
		}
	}
	if (!m_pImg_Item->GetDataPtr("ptr_CECIvtrItem")&&m_pBtn_Confirm->IsEnabled())
	{
		m_pBtn_Confirm->Enable(false);
		if(m_pTxt_Bronze)
			m_pTxt_Bronze->SetText(_AL("0"));
		if(m_pTxt_Silver)
			m_pTxt_Silver->SetText(_AL("0"));
		if(m_pTxt_Silver)
			m_pTxt_Gold->SetText(_AL("0"));
	}

	UpdateMoney();
}

void CDlgItemSvrBase::OnCommandConfirm(const char *szCommand)
{
	ServerStart();
}

void CDlgItemSvrBase::OnCommandCancel(const char *szCommand)
{
	OnEventLButtonDown(0, 0, m_pImg_Item);
	GetGameUIMan()->EndNPCService();
	Show(false);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(false);
	GetGameUIMan()->UnfreezeAllItems();
	Reset();

	if (m_szName=="Win_Pet_Tame")
	{
		NewHelpGuide::StartGuide_Step_6(GUIDE_HELP_PET_11);
	}
}

void CDlgItemSvrBase::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if(!pObj || pObj->GetType()!=AUIOBJECT_TYPE_IMAGEPICTURE)
		return;
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pObj;
	CECIvtrItem *pItem = (CECIvtrItem *)pImg->GetDataPtr("ptr_CECIvtrItem");
	if( !pItem || (pItem->GetClassID()==CECIvtrItem::ICID_RUNE&&pObj==m_pImg_Item) )
		return;
	
	if(pItem->GetClassID()==CECIvtrItem::ICID_GEM)
		((CECIvtrGem*)pItem)->SetSuitShapeType(false);
	pItem->Freeze(false);
	pImg->ClearCover();
	pImg->SetText(_AL(""));
	pImg->SetHint(_AL(""));
	pImg->SetData(0xFFFFFFFF);
	pImg->SetDataPtr(0, "ptr_CECIvtrItem");
	UpdateDisplay();
}

bool CDlgItemSvrBase::UpdateMoney()
{
	if (!m_pTxt_Bronze)
	{
		return false;
	}
	bool bRet = true;
	ACHAR szMoney[20];
	a_sprintf(szMoney, _AL("%d"), m_iMoney % 100);
	m_pTxt_Bronze->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (m_iMoney % 10000) / 100);
	m_pTxt_Silver->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), m_iMoney / 10000);
	m_pTxt_Gold->SetText(szMoney);
	if (m_iMoney>0 && m_iMoney>GetHostPlayer()->GetMoneyAmount())
	{
		m_pTxt_Bronze->SetColor(A3DCOLORRGB(255,0,0));
		m_pTxt_Silver->SetColor(A3DCOLORRGB(255,0,0));
		m_pTxt_Gold->SetColor(A3DCOLORRGB(255,0,0));
		bRet = false;
	}
	else
	{	
		m_pTxt_Bronze->SetColor(A3DCOLORRGB(255,255,255));
		m_pTxt_Silver->SetColor(A3DCOLORRGB(255,255,255));
		m_pTxt_Gold->SetColor(A3DCOLORRGB(255,255,255));
	}

	return bRet;
}