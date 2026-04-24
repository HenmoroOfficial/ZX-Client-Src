// Filename	: DlgItemLock.cpp
// Creator	: Fuchonggang
// Date		: 2008/11/11

#include "AFI.h"
#include "A2DSprite.h"
#include "DlgItemLock.h"
#include "DlgMiniMap.h"
#include "AUI\\AUICTranslate.h"

#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "LuaEvent.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgItemLock, CDlgBase)

AUI_ON_COMMAND("lock",		OnCommandLock)
AUI_ON_COMMAND("unlock",	OnCommandUnLock)
AUI_ON_COMMAND("IDCANCEL",	OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgItemLock, CDlgBase)

AUI_ON_EVENT("Item_item",	WM_LBUTTONDOWN,	OnEventLButtonDown)

AUI_END_EVENT_MAP()


A2DSprite* CDlgItemLock::m_pA2DSpriteLock = NULL;

CDlgItemLock::CDlgItemLock()
{
}

CDlgItemLock::~CDlgItemLock()
{
}

bool CDlgItemLock::OnInitDialog()
{
	m_bLockType = false;	// 0: lock; 1: unlock
	m_iItemIndex= 0;
	m_iItemId	= 0;
	if( !m_pA2DSpriteLock )
	{
		m_pA2DSpriteLock = new A2DSprite;
		if( !m_pA2DSpriteLock ) return AUI_ReportError(__LINE__, __FILE__);

		bool bval = m_pA2DSpriteLock->Init(m_pA3DDevice, "InGame\\lock.tga", 0);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	}
	
	if ( 0==stricmp(GetName(), "Win_ItemLock") )
	{
		int iLockFee = GetGameRun()->GetSpecialIDConfig().fee_lock;
		ACHAR szText[20];
		a_sprintf(szText, _AL("%d"), iLockFee / 10000);
		GetGameUIMan()->GetDialog("Win_ItemLock")->GetDlgItem("Txt_Money1")->SetText(szText);
		a_sprintf(szText, _AL("%d"), (iLockFee - (iLockFee/10000) * 10000) / 100);
		GetGameUIMan()->GetDialog("Win_ItemLock")->GetDlgItem("Txt_Money2")->SetText(szText);
		a_sprintf(szText, _AL("%d"), iLockFee % 10000);
		GetGameUIMan()->GetDialog("Win_ItemLock")->GetDlgItem("Txt_Money3")->SetText(szText);
	}
	else if ( 0==stricmp(GetName(), "Win_ItemUnlock") )
	{
		int iLockFee = GetGameRun()->GetSpecialIDConfig().fee_unlock;
		ACHAR szText[20];
		a_sprintf(szText, _AL("%d"), iLockFee / 10000);
		GetGameUIMan()->GetDialog("Win_ItemUnlock")->GetDlgItem("Txt_Money1")->SetText(szText);
		a_sprintf(szText, _AL("%d"), (iLockFee - (iLockFee/10000) * 10000) / 100);
		GetGameUIMan()->GetDialog("Win_ItemUnlock")->GetDlgItem("Txt_Money2")->SetText(szText);
		a_sprintf(szText, _AL("%d"), iLockFee % 10000);
		GetGameUIMan()->GetDialog("Win_ItemUnlock")->GetDlgItem("Txt_Money3")->SetText(szText);
	}
	
	return true;
}

void CDlgItemLock::OnShowDialog()
{
	m_bLockType = false;	// 0: lock; 1: unlock
	m_iItemIndex= 0;
	m_iItemId	= 0;
	if ( 0==stricmp(GetName(), "Win_ItemLock") )
	{
		PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_ItemLock")->GetDlgItem("Item_item");
		pObj->SetCover(NULL, -1);
		pObj->SetCover(NULL, 0, 1);
		pObj->SetHint(_AL(""));
	}
	else if ( 0==stricmp(GetName(), "Win_ItemUnlock") )
	{
		PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_ItemUnlock")->GetDlgItem("Item_item");
		pObj->SetCover(NULL, -1);
		pObj->SetCover(NULL, 0, 1);
		pObj->SetHint(_AL(""));
	}
}


bool CDlgItemLock::Release()
{
	A3DRELEASE(m_pA2DSpriteLock)

	return CDlgBase::Release();
}

void CDlgItemLock::OnCommandLock(const char* szCommand)
{
	if (!m_bLockType)
	{
		int iLockFee = GetGameRun()->GetSpecialIDConfig().fee_lock;
		if ( GetHostPlayer()->GetMoneyAmount() < iLockFee )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(7032), MB_OK,
					A3DCOLORRGBA(255, 255, 255, 160));
		}
		else if ( GetHostPlayer()->GetPack()->SearchEmpty() < 0 )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(9889), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160));
		}
		else
		{
			GetGameSession()->c2s_CmdNPCSevLockItem(m_iItemIndex, m_iItemId);
			PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_ItemLock")->GetDlgItem("Item_item");
			pObj->SetCover(NULL, -1);
			GetGameUIMan()->UnfreezeAllItems();
			LuaEvent::FirstLockItem();
		}
	}
}

void CDlgItemLock::OnCommandUnLock(const char* szCommand)
{
	if (m_bLockType)
	{
		int iLockFee = GetGameRun()->GetSpecialIDConfig().fee_unlock;
		if ( GetHostPlayer()->GetMoneyAmount() < iLockFee )
		{
			PAUIDIALOG pMsgBox;
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(7033), MB_OK,
					A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		}
		else if( GetHostPlayer()->GetPack()->GetItemTotalNum(GetGameRun()->GetSpecialIDConfig().unlock_item_id) < 1 )
		{
			PAUIDIALOG pMsgBox;
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(7034), MB_OK,
					A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		}
		else
		{
			PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_ItemUnlock")->GetDlgItem("Item_item");
			GetGameSession()->c2s_CmdNPCSevUnlockItem(m_iItemIndex, m_iItemId);
			pObj->SetCover(NULL, -1);
			pObj->SetCover(NULL,0,1);
			GetGameUIMan()->UnfreezeAllItems();
		}
	}
}

void CDlgItemLock::OnCommand_CANCEL(const char * szCommand)
{
	GetGameUIMan()->EndNPCService();
	if ( 0==stricmp(GetName(), "Win_ItemLock") )
	{
		GetGameUIMan()->GetDialog("Win_ItemLock")->Show(false);
	}
	else if ( 0==stricmp(GetName(), "Win_ItemUnlock") )
	{
		GetGameUIMan()->GetDialog("Win_ItemUnlock")->Show(false);
	}
	GetGameUIMan()->UnfreezeAllItems();
}

void CDlgItemLock::SetItemLock(CECIvtrItem *pItem, int nSlot)
{
	AUICTranslate trans;
	SetDataPtr(pItem);
	pItem->Freeze(true);
	m_bLockType = false;	// 0: lock; 1: unlock
	m_iItemIndex= nSlot;
	m_iItemId	= pItem->GetTemplateID();
	PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_ItemLock")->GetDlgItem("Item_item");
	AString strFile;
	af_GetFileTitle(pItem->GetIconFile(), strFile);
	strFile.MakeLower();
	pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
	const wchar_t* szDesc = pItem->GetDesc();
	if( szDesc )
		pObj->SetHint(trans.Translate(szDesc));
	else
		pObj->SetHint(pItem->GetName());
}

void CDlgItemLock::SetItemUnLock(CECIvtrItem *pItem, int nSlot)
{
	AUICTranslate trans;
	SetDataPtr(pItem);
	pItem->Freeze(true);
	m_bLockType = true;	// 0: lock; 1: unlock
	m_iItemIndex= nSlot;
	m_iItemId	= pItem->GetTemplateID();
	PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetGameUIMan()->GetDialog("Win_ItemUnlock")->GetDlgItem("Item_item");
	AString strFile;
	af_GetFileTitle(pItem->GetIconFile(), strFile);
	strFile.MakeLower();
	pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
	pObj->SetCover(m_pA2DSpriteLock,0,1);
	const wchar_t* szDesc = pItem->GetDesc();
	if( szDesc )
		pObj->SetHint(trans.Translate(szDesc));
	else
		pObj->SetHint(pItem->GetName());
}


void CDlgItemLock::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if ( 0==stricmp(GetName(), "Win_ItemLock") )
	{
		CECIvtrItem *pItem = (CECIvtrItem *)GetDataPtr();
		if( pItem )
			pItem->Freeze(false);
		PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_item");
		pObj->ClearCover();
		pObj->SetHint(_AL(""));
		pObj->SetText(_AL(""));
		SetDataPtr(NULL);
	}
	else if ( 0==stricmp(GetName(), "Win_ItemUnlock") )
	{
		CECIvtrItem *pItem = (CECIvtrItem *)GetDataPtr();
		if( pItem )
			pItem->Freeze(false);
		PAUIIMAGEPICTURE pObj = (PAUIIMAGEPICTURE)GetDlgItem("Item_item");
		pObj->ClearCover();
		pObj->SetHint(_AL(""));
		pObj->SetText(_AL(""));
		SetDataPtr(NULL);

	}
}