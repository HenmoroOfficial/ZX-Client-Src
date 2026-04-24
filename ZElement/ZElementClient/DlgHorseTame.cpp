// File		: DlgHorseTame.cpp
// Creator	: Xiao Zhou
// Date		: 2008/5/7

#include "AFI.h"
#include "DlgHorseTame.h"
#include "DlgInventory.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "AUI\\AUICTranslate.h"
#include "elementdataman.h"


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgHorseTame, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCANCEL)
AUI_ON_COMMAND("start",			OnCommandStart)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgHorseTame, CDlgBase)

AUI_ON_EVENT("Item_Horse",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Item_Horse",		WM_LBUTTONDBLCLK,	OnEventLButtonDown)
AUI_ON_EVENT("Item_Horse",		WM_RBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Item_Assistant",	WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Item_Assistant",	WM_LBUTTONDBLCLK,	OnEventLButtonDown)
AUI_ON_EVENT("Item_Assistant",	WM_RBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgHorseTame::CDlgHorseTame()
{
}

CDlgHorseTame::~CDlgHorseTame()
{
}

bool CDlgHorseTame::OnInitDialog()
{
	m_pItemHorse = (PAUIIMAGEPICTURE)GetDlgItem("Item_Horse");
	m_pItemAssitant = (PAUIIMAGEPICTURE)GetDlgItem("Item_Assistant");
	m_pBtn_Enchase = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Enchase");
	m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
	m_pTxt_Money2 = (PAUILABEL)GetDlgItem("Txt_Money2");
	m_pTxt_Money3 = (PAUILABEL)GetDlgItem("Txt_Money3");
	m_pTxt_Money1->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money2->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money3->SetAlign(AUIFRAME_ALIGN_RIGHT);
	if( stricmp(m_szName, "Win_PetRaise") == 0 )
		m_pTxt_Money1->SetData(GetGameRun()->GetSpecialIDConfig().fee_pet_gained);
	else if( stricmp(m_szName, "Win_PetChangeName") == 0 )
		m_pTxt_Money1->SetData(GetGameRun()->GetSpecialIDConfig().fee_pet_rename);
	else if( stricmp(m_szName, "Win_PetCombine") == 0 )
		m_pTxt_Money1->SetData(GetGameRun()->GetSpecialIDConfig().fee_pet_refine);
	else if( stricmp(m_szName, "Win_PetRelease") == 0 )
		m_pTxt_Money1->SetData(GetGameRun()->GetSpecialIDConfig().fee_pet_free);

	return true;
}

void CDlgHorseTame::CheckItem(PAUIOBJECT pObj)
{
	CECInventory *pPack = GetHostPlayer()->GetPack();
	CECIvtrItem *pItem;
	AUICTranslate trans;
	if( pObj && pObj->GetDataPtr("ptr_CECIvtrItem") )
	{
		pItem = pPack->GetItem(pObj->GetData());
		if( !pItem )
			OnEventLButtonDown(0, 0, pObj);
		else
		{
			const ACHAR * szDesc = pItem->GetDesc();
			if( szDesc )
				pObj->SetHint(trans.Translate(szDesc));
			else
				pObj->SetHint(_AL(""));
		}
	}
}

void CDlgHorseTame::OnTick()
{
	int nMoney = m_pTxt_Money1->GetData();
	ACHAR szMoney[20];
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
	m_pTxt_Money3->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pTxt_Money2->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pTxt_Money1->SetText(szMoney);
	if( nMoney > GetHostPlayer()->GetMoneyAmount() )
	{
		m_pTxt_Money1->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxt_Money2->SetColor(A3DCOLORRGB(255, 0, 0));
		m_pTxt_Money3->SetColor(A3DCOLORRGB(255, 0, 0));
	}
	else
	{
		m_pTxt_Money1->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pTxt_Money2->SetColor(A3DCOLORRGB(255, 255, 255));
		m_pTxt_Money3->SetColor(A3DCOLORRGB(255, 255, 255));
	}
	CheckItem(m_pItemHorse);
	CheckItem(m_pItemAssitant);
	m_pBtn_Enchase->Enable(m_pItemHorse->GetDataPtr("ptr_CECIvtrItem") != NULL && m_pItemAssitant->GetDataPtr("ptr_CECIvtrItem") != NULL);
}

void CDlgHorseTame::OnCommandCANCEL(const char * szCommand)
{
	Show(false);
	GetHostPlayer()->EndNPCService();
	GetGameUIMan()->m_pCurNPCEssence = NULL;
	GetGameUIMan()->m_pDlgInventory->Show(false);
	if( m_pItemHorse )
		OnEventLButtonDown(0, 0, m_pItemHorse);
	if( m_pItemAssitant )
		OnEventLButtonDown(0, 0, m_pItemAssitant);
	GetGameUIMan()->UnfreezeAllItems();
	m_pTxt_Money1->SetData(0);
}

void CDlgHorseTame::OnCommandStart(const char * szCommand)
{
	CECHostPlayer *pHost = GetHostPlayer();

	if( !m_pItemHorse->GetDataPtr("ptr_CECIvtrItem") || !m_pItemAssitant->GetDataPtr("ptr_CECIvtrItem") ) return;

	GetGameSession()->c2s_CmdNPCSevVehicleUpgrade(m_pItemHorse->GetData(), m_pItemAssitant->GetData());
	OnEventLButtonDown(0, 0, m_pItemHorse);
	OnEventLButtonDown(0, 0, m_pItemAssitant);
}

void CDlgHorseTame::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !pObj || !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;
	PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pObj;
	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	pItem->Freeze(false);
	pImage->SetDataPtr(NULL);
	pImage->SetData(0);
	pImage->SetHint(_AL(""));
	pImage->SetCover(NULL, -1);
	GetGameUIMan()->PlayItemSound(pItem, false);
}

void CDlgHorseTame::DragDropItem(int iSrc, PAUIOBJECT pObjOver)
{
	if( !pObjOver )
		return;

	CECIvtrItem *pIvtrSrc = GetHostPlayer()->GetPack()->GetItem(iSrc);

	if( 0 == stricmp(pObjOver->GetName(), "Item_Horse") && pIvtrSrc->GetClassID() != CECIvtrItem::ICID_VEHICLE )
		return;
	
	if( 0 == stricmp(pObjOver->GetName(), "Item_Assistant") && pIvtrSrc->GetTemplateID() != GetGameRun()->GetSpecialIDConfig().id_vehicle_upgrade_item )
		return;

	OnEventLButtonDown(0, 0, pObjOver);
	AString strFile;
	af_GetFileTitle(pIvtrSrc->GetIconFile(), strFile);
	strFile.MakeLower();
	PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pObjOver;
	pImage->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);

	GetGameUIMan()->PlayItemSound(pIvtrSrc, true);
	pIvtrSrc->Freeze(true);
	pObjOver->SetData(iSrc);
	pObjOver->SetDataPtr(pIvtrSrc, "ptr_CECIvtrItem");
}