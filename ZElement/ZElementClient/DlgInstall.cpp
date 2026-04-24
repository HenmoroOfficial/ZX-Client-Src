// File		: DlgInstall.cpp
// Creator	: Xiao Zhou
// Date		: 2005/8/16

#include "AFI.h"
#include "DlgInstall.h"
#include "DlgInventory.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrStone.h"
#include "LuaEvent.h"
#include "EC_NewHelpGuide.h"
#include "AUI\\AUICTranslate.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgInstall, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCANCEL)
AUI_ON_COMMAND("start",			OnCommandStart)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgInstall, CDlgBase)

AUI_ON_EVENT("Item_a", WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_a", WM_LBUTTONDBLCLK,OnEventLButtonDown)
AUI_ON_EVENT("Item_a", WM_RBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_b", WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_b", WM_LBUTTONDBLCLK,OnEventLButtonDown)
AUI_ON_EVENT("Item_b", WM_RBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_c", WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Item_c", WM_LBUTTONDBLCLK,OnEventLButtonDown)
AUI_ON_EVENT("Item_c", WM_RBUTTONDOWN,	OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgInstall::CDlgInstall()
{
}

CDlgInstall::~CDlgInstall()
{
}

bool CDlgInstall::OnInitDialog()
{
	m_pItema = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
	m_pItemb = (PAUIIMAGEPICTURE)GetDlgItem("Item_b");
	m_pItemc = (PAUIIMAGEPICTURE)GetDlgItem("Item_c");
	m_pBtn_Enchase = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Enchase");
	m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
	m_pTxt_Money2 = (PAUILABEL)GetDlgItem("Txt_Money2");
	m_pTxt_Money3 = (PAUILABEL)GetDlgItem("Txt_Money3");
	m_pTxt_Money1->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money2->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money3->SetAlign(AUIFRAME_ALIGN_RIGHT);

	return true;
}

void CDlgInstall::OnTick()
{
	int nMoney = 0;
	if( stricmp(m_szName, "Win_Enchase") == 0 )
	{
		if (m_pItema->GetData()>0)
		{
			if (NULL == GetHostPlayer()->GetPack()->GetItem(m_pItema->GetData()))
				OnEventLButtonDown(0, 0, m_pItema);
		}
		if (m_pItemb->GetData()>0)
		{
			if (NULL == GetHostPlayer()->GetPack()->GetItem(m_pItemb->GetData()))
				OnEventLButtonDown(0, 0, m_pItemb);
		}
		if( m_pItema->GetDataPtr("ptr_CECIvtrItem") && m_pItemb->GetDataPtr("ptr_CECIvtrItem") )
		{
			m_pBtn_Enchase->Enable(true);
			CECIvtrEquip *pEquipA = (CECIvtrEquip *)m_pItema->GetDataPtr("ptr_CECIvtrItem");
			CECIvtrItem *pItemB = (CECIvtrItem *)m_pItemb->GetDataPtr("ptr_CECIvtrItem");
			if( pItemB->GetClassID() == CECIvtrItem::ICID_ESTONE )
				nMoney = pEquipA->GetDBEssence()->fee_estone;
			else if( pItemB->GetClassID() == CECIvtrItem::ICID_PSTONE )
				nMoney = pEquipA->GetDBEssence()->fee_install_pstone;
			else
				nMoney = pEquipA->GetDBEssence()->fee_install_sstone;
		}
		else
			m_pBtn_Enchase->Enable(false);
	}
	else
	{
		if( m_pItema->GetDataPtr("ptr_CECIvtrItem") )
		{
			m_pBtn_Enchase->Enable(true);
			CECIvtrEquip *pEquipA = (CECIvtrEquip *)m_pItema->GetDataPtr("ptr_CECIvtrItem");
			if( stricmp(m_szName, "Win_Disenchase1") == 0 )
				nMoney = pEquipA->GetDBEssence()->fee_uninstall_pstone;
			else
				nMoney = pEquipA->GetDBEssence()->fee_uninstall_sstone;
		}
		else
			m_pBtn_Enchase->Enable(false);
	}
	ACHAR szMoney[20];
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
	m_pTxt_Money3->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pTxt_Money2->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pTxt_Money1->SetText(szMoney);
	m_pTxt_Money1->SetData(nMoney);
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
}

void CDlgInstall::OnCommandCANCEL(const char * szCommand)
{
	Show(false);
	GetHostPlayer()->EndNPCService();
	GetGameUIMan()->m_pCurNPCEssence = NULL;
	GetGameUIMan()->m_pDlgInventory->Show(false);
	OnEventLButtonDown(0, 0, m_pItema);
	if( m_pItemb )
		OnEventLButtonDown(0, 0, m_pItemb);
	if( m_pItemc )
		OnEventLButtonDown(0, 0, m_pItemc);
	GetGameUIMan()->UnfreezeAllItems();
}

void CDlgInstall::OnCommandStart(const char * szCommand)
{
	PAUIDIALOG pMsgBox;
	CECHostPlayer *pHost = GetHostPlayer();

	if( !m_pItema->GetDataPtr("ptr_CECIvtrItem") ) return;

	if( 0 == stricmp(GetName(), "Win_Enchase") )
	{
		NewHelpGuide::FinishGuide(GUIDE_HELP_REFINING_5);
	}

	int nMoney = m_pTxt_Money1->GetData();
	if( nMoney > pHost->GetMoneyAmount() )
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(656), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		return;
	}
	
	CECIvtrItem *pIvtrA = (CECIvtrItem *)m_pItema->GetDataPtr("ptr_CECIvtrItem");

	CECIvtrEquip *pEquipA = (CECIvtrEquip *)pIvtrA;
	if( 0 == stricmp(GetName(), "Win_Enchase") )
	{
		CECIvtrItem *pIvtrB = (CECIvtrItem *)m_pItemb->GetDataPtr("ptr_CECIvtrItem");
		if( !pIvtrB ) return;
		const IVTR_ESSENCE_EQUIPMENT& essence = pEquipA->GetEssence();
		if( pIvtrB->GetClassID() == CECIvtrItem::ICID_ESTONE && essence.num_estone == 20 )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(224), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			return;
		}
		if( pIvtrB->GetClassID() == CECIvtrItem::ICID_PSTONE && essence.num_pstone == 10 )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(225), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			return;
		}
		if( pIvtrB->GetClassID() == CECIvtrItem::ICID_PSTONE && 
			essence.num_pstone > 0 && essence.id_pstone != pIvtrB->GetTemplateID() )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(227), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			return;
		}

		if( m_pItemc->GetDataPtr("ptr_CECIvtrItem") )
			GetGameSession()->c2s_CmdNPCSevEmbed(
				m_pItema->GetData(), pIvtrA->GetTemplateID(),
				m_pItemb->GetData(), m_pItemc->GetData());
		else
			GetGameSession()->c2s_CmdNPCSevEmbed(
				m_pItema->GetData(), pIvtrA->GetTemplateID(),
				m_pItemb->GetData(), -1);
		LuaEvent::FirstHarden();
		OnEventLButtonDown(0, 0, m_pItema);
		OnEventLButtonDown(0, 0, m_pItemb);
		OnEventLButtonDown(0, 0, m_pItemc);
//		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(228),
//			MB_OK, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
//		pMsgBox->SetLife(3);
	}
	else if( 0 == stricmp(GetName(), "Win_Disenchase1") )
	{
		const IVTR_ESSENCE_EQUIPMENT& essence = pEquipA->GetEssence();
		if( essence.num_pstone == 0 )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(223), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			return;
		}
		GetGameUIMan()->MessageBox("Game_Disenchase1", GetGameUIMan()->GetStringFromTable(300),
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if( 0 == stricmp(GetName(), "Win_Disenchase2") )
	{
		// IVTR_ESSENCE_EQUIPMENT essence = pEquipA->GetEssence();
		if( 1 )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(228), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			return;
		}
		GetGameUIMan()->MessageBox("Game_Disenchase2", GetGameUIMan()->GetStringFromTable(301),
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
	}
}

void CDlgInstall::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !pObj || !pObj->GetDataPtr("ptr_CECIvtrItem") ) return;
	PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)pObj;
	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");
	pItem->Freeze(false);
	pImage->SetDataPtr(NULL, "ptr_CECIvtrItem");
	pImage->SetData(0);
	pImage->SetHint(_AL(""));
	pImage->SetCover(NULL, -1);
	GetGameUIMan()->PlayItemSound(pItem, false);
}

void CDlgInstall::DragDropItem(int iSrc, PAUIOBJECT pObjOver)
{
	if( !pObjOver )
		return;

	CECIvtrItem *pIvtrSrc = GetHostPlayer()->GetPack()->GetItem(iSrc);

	if( 0 == stricmp(pObjOver->GetName(), "Item_a") && 
		(!pIvtrSrc->IsEquipment() || !((CECIvtrEquip*)pIvtrSrc)->GetDBEssence()) )
		return;
	
	if( 0 == stricmp(pObjOver->GetName(), "Item_b") && 
		pIvtrSrc->GetClassID() != CECIvtrItem::ICID_ESTONE && 
		pIvtrSrc->GetClassID() != CECIvtrItem::ICID_SSTONE &&
		pIvtrSrc->GetClassID() != CECIvtrItem::ICID_PSTONE )
		return;

	if( 0 == stricmp(pObjOver->GetName(), "Item_c") && 
		pIvtrSrc->GetClassID() != CECIvtrItem::ICID_REFINETICKET )
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
	const ACHAR * szDesc = pIvtrSrc->GetDesc();
	AUICTranslate trans;
	if( szDesc )
		pObjOver->SetHint(trans.Translate(szDesc));
	else
		pObjOver->SetHint(_AL(""));
}