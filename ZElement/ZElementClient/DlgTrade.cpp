// File		: DlgTrade.cpp
// Creator	: Xiao Zhou
// Date		: 2005/8/16

#include "AFI.h"
#include "AUI\\AUIEditBox.h"
#include "DlgTrade.h"
#include "DlgInputNO.h"
#include "DlgShop.h"
#include "DlgInventory.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_DealInventory.h"
#include "EC_IvtrItem.h"
#include "Network\\gnetdef.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgTrade, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCANCEL)
AUI_ON_COMMAND("lock",			OnCommandLock)
AUI_ON_COMMAND("complete",		OnCommandComplete)
AUI_ON_COMMAND("choosemoney",	OnCommandChoosemoney)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgTrade, CDlgBase)

AUI_ON_EVENT("Img_FrameMoneyI",	WM_LBUTTONDOWN,		OnEventLButtonDown_Money)
AUI_ON_EVENT("I_*",				WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("I_*",				WM_LBUTTONDBLCLK,	OnEventLButtonDBLCLK)
AUI_ON_EVENT("I_*",				WM_RBUTTONUP,		OnEventLButtonDBLCLK)

AUI_END_EVENT_MAP()

CDlgTrade::CDlgTrade()
{
}

CDlgTrade::~CDlgTrade()
{
}

bool CDlgTrade::OnInitDialog()
{
	m_pBtnLock = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Lock");
	m_pBtnLock2 = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Lock2");
	m_pBtnConfirm = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Confirm");
	m_pBtnConfirm2 = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Confirm2");
	m_pBtnLock = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Lock");
	m_pBtnLock = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Lock");
	m_pBtnLock = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Lock");
	m_pTxt_Money1U = (PAUILABEL)GetDlgItem("Txt_Money1U");
	m_pTxt_Money2U = (PAUILABEL)GetDlgItem("Txt_Money2U");
	m_pTxt_Money3U = (PAUILABEL)GetDlgItem("Txt_Money3U");
	m_pTxt_Money1U->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money2U->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money3U->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money1I = (PAUILABEL)GetDlgItem("Txt_Money1I");
	m_pTxt_Money2I = (PAUILABEL)GetDlgItem("Txt_Money2I");
	m_pTxt_Money3I = (PAUILABEL)GetDlgItem("Txt_Money3I");
	m_pTxt_Money1I->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money2I->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money3I->SetAlign(AUIFRAME_ALIGN_RIGHT);
	char szItem[40];
	for( int i = 0; i < IVTRSIZE_DEALPACK; i++ )
	{
		sprintf(szItem, "U_%02d", i + 1);
		m_pYourItem[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
		sprintf(szItem, "I_%02d", i + 1);
		m_pMyItem[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}

	return true;
}

void CDlgTrade::OnShowDialog()
{
	PAUIDIALOG pDlgPocket = GetGameUIMan()->GetDialog("Win_InventoryPet");
	if (pDlgPocket->IsShow())
	{
		pDlgPocket->Show(false);
	}
	pDlgPocket = GetGameUIMan()->GetDialog("Win_Inventory_Costume");
	if (pDlgPocket->IsShow())
	{
		pDlgPocket->Show(false);
	}
	
	PAUIDIALOG pInventoryExpand = GetGameUIMan()->GetDialog("Win_Inventory_Expand01");
	if(pInventoryExpand && pInventoryExpand->IsShow())
		pInventoryExpand->Show(false);
	
	pInventoryExpand = GetGameUIMan()->GetDialog("Win_Inventory_Expand02");
	if(pInventoryExpand && pInventoryExpand->IsShow())
		pInventoryExpand->Show(false);
}

void CDlgTrade::OnCommandCANCEL(const char * szCommand)
{
	GetGameSession()->trade_Cancel((int)GetDataPtr("int"));
}

void CDlgTrade::OnCommandLock(const char * szCommand)
{
	GetGameSession()->trade_Submit((int)GetDataPtr("int"));
}

void CDlgTrade::OnCommandComplete(const char * szCommand)
{
	CECHostPlayer *pHost = GetHostPlayer();
	GetGameUIMan()->UnfreezeAllItems();
	GetGameSession()->trade_Confirm((int)GetDataPtr("int"));
}

void CDlgTrade::OnCommandChoosemoney(const char * szCommand)
{
	if( !m_pBtnLock->IsShow() )
		return;

	CECHostPlayer *pHost = GetHostPlayer();
	CDlgInputNO* pShow = GetGameUIMan()->m_pDlgInputMoney;
	PAUIOBJECT pEdit = pShow->GetDlgItem("DEFAULT_Txt_Money1");
	pEdit->SetData(GetHostPlayer()->GetMoneyAmount());

	pShow->Show(true, true);
	pShow->SetType(CDlgInputNO::INPUTNO_TRADE_MONEY);
}

void CDlgTrade::OnEventLButtonDown_Money(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	OnCommandChoosemoney("");
}

void CDlgTrade::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") || !m_pBtnLock->IsShow() || !m_pBtnLock->IsEnabled() )
		return;

	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT ptCapture = { GET_X_LPARAM(lParam) - p->X, GET_Y_LPARAM(lParam) - p->Y };

	GetGameUIMan()->InvokeDragDrop(this, pObj, ptCapture);
	GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
}

void CDlgTrade::OnEventLButtonDBLCLK(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") || !m_pBtnLock->IsShow() )
		return;

	int idTrade = (int)GetDataPtr("int");
	CECIvtrItem *pIvtrSrc = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	int idItem = pIvtrSrc->GetTemplateID();
	int iSrc = atoi(pObj->GetName() + strlen("I_")) - 1;
	CECHostPlayer *pHost = GetHostPlayer();
	CECDealInventory *pDeal = pHost->GetDealPack();
	const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(iSrc);

	GetGameSession()->trade_RemoveGoods(idTrade, idItem, ii.iOrigin, ii.iAmount, 0);
	pIvtrSrc->Freeze(false);
	
}

bool CDlgTrade::Tick(void)
{
	if( IsShow() ) UpdateTradeInfo();

	return CDlgBase::Tick();
}

bool CDlgTrade::UpdateTradeInfo()
{
	int i;
	AString strFile;
	ACHAR szText[40];
	CECIvtrItem *pItem;
	PAUIIMAGEPICTURE pObj;
	CECHostPlayer *pHost = GetHostPlayer();

	CECDealInventory *pEPDeal = pHost->GetEPDealPack();
	for( i = 0; i < IVTRSIZE_DEALPACK; i++ )
	{
		pObj = m_pYourItem[i];
		if( !pObj ) continue;

		pItem = pEPDeal->GetItem(i);
		if( pItem )
		{
			pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");
			pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
			if( pItem->GetCount() > 1 )
			{
				a_sprintf(szText, _AL("%d"), pItem->GetCount());
				pObj->SetText(szText);
			}
			else
				pObj->SetText(_AL(""));
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		}
		else
		{
			pObj->SetCover(NULL, -1);
			pObj->SetText(_AL(""));
			pObj->SetDataPtr(NULL);
			pObj->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	}
	int nMoney = pEPDeal->GetMoney();
	ACHAR szMoney[20];
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
	m_pTxt_Money3U->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pTxt_Money2U->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pTxt_Money1U->SetText(szMoney);

	CECDealInventory *pDeal = pHost->GetDealPack();
	for( i = 0; i < IVTRSIZE_DEALPACK; i++ )
	{
		pObj = m_pMyItem[i];
		if( !pObj ) continue;

		pItem = pDeal->GetItem(i);
		if( pItem )
		{
			pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");
			pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
			const CECDealInventory::ITEMINFO &ii = pDeal->GetItemInfo(i);
			if( ii.iAmount > 1 )
			{
				a_sprintf(szText, _AL("%d"), ii.iAmount);
				pObj->SetText(szText);
			}
			else
				pObj->SetText(_AL(""));
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		}
		else
		{
			pObj->SetCover(NULL, -1);
			pObj->SetText(_AL(""));
			pObj->SetDataPtr(NULL);
			pObj->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	}
	nMoney = pDeal->GetMoney();
	szMoney[20];
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
	m_pTxt_Money3I->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pTxt_Money2I->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pTxt_Money1I->SetText(szMoney);
	m_pTxt_Money1I->SetData(pDeal->GetMoney());

	return true;
}

void CDlgTrade::TradeAction(int idPlayer, int idTrade, int idAction, int nCode)
{
	CECHostPlayer *pHost = GetHostPlayer();
	PAUIDIALOG pInventory = GetGameUIMan()->m_pDlgInventory;

	if( idAction == TRADE_ACTION_ALTER )
	{
		bool bMsgBox = false;
		if( pHost->GetCharacterID() != idPlayer && !m_pBtnLock->IsEnabled() )
			bMsgBox = true;

		m_pBtnLock->Show(true);
		m_pBtnLock->Enable(true);
		m_pBtnConfirm->Show(false);

		m_pBtnLock2->Show(true);
		m_pBtnLock2->Enable(true);
		m_pBtnConfirm2->Show(false);
		if( bMsgBox )
			GetGameUIMan()->MessageBox("", GetStringFromTable(230), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if( idAction == TRADE_ACTION_LOCK )
	{
		if( nCode == GNET::ERR_TRADE_READY )
		{
			m_pBtnLock2->Show(false);
			m_pBtnConfirm2->Show(true);
			m_pBtnConfirm2->Enable(true);

			m_pBtnLock->Show(false);
			m_pBtnConfirm->Show(true);
			m_pBtnConfirm->Enable(true);
		}
		else if( nCode == GNET::ERR_TRADE_READY_HALF )
		{
			if( pHost->GetCharacterID() == idPlayer )
			{
				m_pBtnLock->Enable(false);
				GetGameUIMan()->AddChatMessage(GetStringFromTable(219), GP_CHAT_MISC);
			}
			else
				m_pBtnLock2->Enable(false);
		}
	}
	else if( idAction == TRADE_ACTION_UNLOCK )
	{
		m_pBtnLock->Show(true);
		m_pBtnLock->Enable(true);
		m_pBtnConfirm->Show(false);

		m_pBtnLock2->Show(true);
		m_pBtnLock2->Enable(true);
		m_pBtnConfirm2->Show(false);
		
		GetGameUIMan()->AddChatMessage(GetStringFromTable(275), GP_CHAT_MISC);
	}
	else if( idAction == TRADE_ACTION_CANCEL )
	{
		if( pHost->GetCharacterID() != idPlayer )
			GetGameUIMan()->AddChatMessage(GetStringFromTable(215), GP_CHAT_MISC);
		m_idTradeAction = idAction;
	}
	else if( idAction == TRADE_ACTION_DEAL )
	{
		if( pHost->GetCharacterID() == idPlayer )
		{
			if( nCode == GNET::ERR_TRADE_HALFDONE )
			{
				m_pBtnConfirm->Enable(false);
				GetGameUIMan()->AddChatMessage(GetStringFromTable(216), GP_CHAT_MISC);
			}
			else if( nCode == GNET::ERR_TRADE_DONE )
				m_pBtnConfirm->Show(false);
		}
		else
		{
			if( nCode == GNET::ERR_TRADE_HALFDONE )
				m_pBtnConfirm2->Enable(false);
			else if( nCode == GNET::ERR_TRADE_DONE )
				m_pBtnConfirm2->Show(false);
		}
		m_idTradeAction = idAction;
	}
	else if( idAction == TRADE_ACTION_END )
	{
		Show(false);
		pInventory->Show(false);
		if( nCode == _TRADE_END_NORMAL )
		{
			if( m_idTradeAction == TRADE_ACTION_DEAL )
				GetGameUIMan()->AddChatMessage(GetStringFromTable(217), GP_CHAT_MISC);
		}
		else
			GetGameUIMan()->AddChatMessage(GetStringFromTable(218), GP_CHAT_MISC);

		m_idTradeAction = TRADE_ACTION_NONE;
	}
}
