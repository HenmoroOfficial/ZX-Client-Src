// Filename	: DlgPetEquip.cpp
// Creator	: Fu Chonggang
// Date		: 2009/1/06

#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"
#include "DlgSoulCombine.h"

#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_Global.h"
#include "EC_GameRun.h"
#include "EC_Game.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSoulCombine, CDlgBase)

AUI_ON_COMMAND("add",			OnCommand_add)
AUI_ON_COMMAND("minus",			OnCommand_minus)
AUI_ON_COMMAND("max",			OnCommand_max)
AUI_ON_COMMAND("begin",			OnCommand_begin)
AUI_ON_COMMAND("stop",			OnCommand_stop)
AUI_ON_COMMAND("prolist",		OnCommand_prolist)
AUI_ON_COMMAND("help",			OnCommand_help)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

CDlgSoulCombine::CDlgSoulCombine()
{
	m_idCurRecipe = 0;
	m_idProducing = 0;
}

CDlgSoulCombine::~CDlgSoulCombine()
{
}

bool CDlgSoulCombine::OnInitDialog()
{
	int i;
	char szItem[40];

	m_pPrgs_1 = (PAUIPROGRESS)GetDlgItem("Prgs_1");
	m_pTxt_Number = (PAUIEDITBOX)GetDlgItem("Txt_Number");
	m_pTxt_lab[0] = (PAUILABEL)GetDlgItem("Txt_Rate1");
	m_pTxt_lab[1] = (PAUILABEL)GetDlgItem("Txt_Rate2");
	m_pTxt_lab[2] = (PAUILABEL)GetDlgItem("Txt_Rate3");
	m_pTxt_out[0] = (PAUILABEL)GetDlgItem("Txt_Product1");
	m_pTxt_out[1] = (PAUILABEL)GetDlgItem("Txt_Product2");
	m_pTxt_out[2] = (PAUILABEL)GetDlgItem("Txt_Product3");
	m_pItem_out[0] = (PAUIIMAGEPICTURE)GetDlgItem("Item_Product1");
	m_pItem_out[1] = (PAUIIMAGEPICTURE)GetDlgItem("Item_Product2");
	m_pItem_out[2] = (PAUIIMAGEPICTURE)GetDlgItem("Item_Product3");
	m_pBtn_Produce = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Produce");
	m_pTv_Recipe = (PAUITREEVIEW)GetDlgItem("Tv_Recipe");
	m_pCombo_ProList = (PAUICOMBOBOX)GetDlgItem("Combo_Prolist");
	if( m_pCombo_ProList )
	{
		for(i = 0; i < 11; i++)
		{
			m_pCombo_ProList->AddString(GetStringFromTable(7080+i));
		}
		m_pCombo_ProList->SetCurSel(0);
	}

	for( i = 0; i < CDLGSOULCOMBINE_MAX_MATERIALS; i++ )
	{
		sprintf(szItem, "Txt_no%d", i + 1);
		m_pTxt_no[i] = (PAUILABEL)GetDlgItem(szItem);

		sprintf(szItem, "Txt_name%d", i + 1);
		m_pTxt_name[i] = (PAUILABEL)GetDlgItem(szItem);

		sprintf(szItem, "Item_%d", i + 1);
		m_pImgItem[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}
	return true;
}

bool CDlgSoulCombine::Render()
{
	UpdateMaxAmount();
	UpdateSelectItem();
	return CDlgBase::Render();
}

void CDlgSoulCombine::OnTick()
{
}

void CDlgSoulCombine::OnShowDialog()
{
	GetHostPlayer()->SetProducing(true);
	OnCommand_prolist("");
}

void CDlgSoulCombine::OnCommand_add(const char * szCommand)
{
	if( m_pPrgs_1->GetData() > 0 ) return;

	ACHAR szText[40];
	int nMax = m_pTxt_Number->GetData();
	int nNum = a_atoi(m_pTxt_Number->GetText());
	if( nNum < nMax )
		nNum++;
	a_sprintf(szText, _AL("%d"), nNum);
	m_pTxt_Number->SetText(szText);
}

void CDlgSoulCombine::OnCommand_minus(const char * szCommand)
{
	if( m_pPrgs_1->GetData() > 0 ) return;

	ACHAR szText[40];
	int nMax = m_pTxt_Number->GetData();
	int nNum = a_atoi(m_pTxt_Number->GetText());
	if( nNum > 1 )
		nNum--;
	a_sprintf(szText, _AL("%d"), nNum);
	m_pTxt_Number->SetText(szText);
}

void CDlgSoulCombine::OnCommand_max(const char * szCommand)
{
	if( m_pPrgs_1->GetData() > 0 ) return;

	ACHAR szText[40];
	int nMax = m_pTxt_Number->GetData();
	a_sprintf(szText, _AL("%d"), nMax);
	m_pTxt_Number->SetText(szText);
	OnCommand_begin("");
}

void CDlgSoulCombine::OnCommand_begin(const char * szCommand)
{
	if( m_pPrgs_1->GetData() > 0 ) return;
	if( m_pItem_out[0]->GetData() == 0 ) return;
	
	int nNum = a_atoi(m_pTxt_Number->GetText());
	if( nNum > 0 )
	{
		m_idProducing = m_idCurRecipe;
		GetGameSession()->c2s_CmdNPCSevMeldSoul(m_pItem_out[0]->GetData(), m_idTerritory);
		m_pBtn_Produce->Enable(false);
	}
}

void CDlgSoulCombine::OnCommand_stop(const char * szCommand)
{
	PAUIPROGRESS pProgress = (PAUIPROGRESS)m_pPrgs_1;

	if( m_idProducing > 0 )
		GetGameSession()->c2s_CmdCancelAction();
	m_idProducing = 0;
}

void CDlgSoulCombine::OnCommand_prolist(const char * szCommand)
{
	int i;
	for(i = 0; i < CDLGSOULCOMBINE_MAX_MATERIALS; i++ )
	{
		m_pImgItem[i]->Show(false);
		m_pTxt_no[i]->Show(false);
		m_pTxt_name[i]->Show(false);
	}
	for (i=0; i<3; i++)
	{
		m_pTxt_lab[i]->Show(false);
		m_pTxt_out[i]->Show(false);
		m_pItem_out[i]->Show(false);
	}


	m_pTv_Recipe->DeleteAllItems();
	LoadAvailableSoulRecipe();
/*	m_nRecipeSum = GetHostPlayer()->GetRecipeCount();
	for(i = 0; i < m_nRecipeSum; i++ )
		AddOneRecipe(GetHostPlayer()->GetRecipeID(i));
	if( m_pCombo_ProList->GetCurSel() == 1 )
		CleanUpRecipe();
*/}

void CDlgSoulCombine::OnCommand_help(const char * szCommand)
{
	//GetGameUIMan()->GetDialog("Win_Produce_Icon_Help")->Show(true);
	AString strDlg = "Win_Produce_Icon_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgSoulCombine::OnCommand_CANCEL(const char * szCommand)
{
	GetGameUIMan()->EndNPCService();

	PAUIPROGRESS pProgress = (PAUIPROGRESS)m_pPrgs_1;

	m_idProducing = 0;
	if( pProgress->GetData() > 0 )
		GetGameSession()->c2s_CmdCancelAction();
	else
	{		     
		Show(false);
		GetHostPlayer()->SetProducing(false);
	}
}


void CDlgSoulCombine::UpdateSelectItem()
{
	elementdataman *pDataMan = g_pGame->GetElementDataMan();
	P_AUITREEVIEW_ITEM pItem = m_pTv_Recipe->GetSelectedItem();
	ACHAR szText[20];
	int nMax, nNum;

	if( pItem )
	{
		if( m_pTv_Recipe->GetFirstChildItem(pItem) != NULL )
			m_idCurRecipe = 0;
		else
		{
			UpdateRecipe(m_pTv_Recipe->GetItemData(pItem));
			nMax = (int)m_pTv_Recipe->GetItemDataPtr(pItem);
			m_pTxt_Number->SetData(nMax);
			if( m_idCurRecipe != m_pTv_Recipe->GetItemData(pItem) )
			{
				m_idCurRecipe = m_pTv_Recipe->GetItemData(pItem);
				if( m_idCurRecipe != 0 )
				{
					if( nMax > 0 )
						nNum = 1;
					else
						nNum = 0;
					a_sprintf(szText, _AL("%d"), nNum);
					m_pTxt_Number->SetText(szText);
					DATA_TYPE datatype;
					RECIPE_ESSENCE* pRecipe = (RECIPE_ESSENCE*)pDataMan->get_data_ptr(m_idCurRecipe, ID_SPACE_RECIPE, datatype);
					if( datatype == DT_RECIPE_ESSENCE )
					{
						a_sprintf(szText, _AL("%d"), nNum);
						m_pTxt_out[0]->SetText(szText);
					}
				}
			}
		}
	}
	else
		m_idCurRecipe = 0;
	if( m_idProducing != 0 && m_idCurRecipe != m_idProducing )
	{
		OnCommand_stop("");
	}

	if( m_idCurRecipe == 0 )
	{
		for(int j = 0; j < CDLGSOULCOMBINE_MAX_MATERIALS; j++ )
		{
			m_pImgItem[j]->Show(false);
			m_pTxt_no[j]->Show(false);
			m_pTxt_name[j]->Show(false);
		}
		for (int i=0; i<3; i++)
		{
			m_pTxt_lab[i]->Show(false);
			m_pTxt_out[i]->Show(false);
			m_pTxt_out[i]->SetText(_AL(""));
			m_pItem_out[i]->ClearCover();
			m_pItem_out[i]->Show(false);
		}
		m_pTxt_Number->SetData(0);
		m_pTxt_Number->SetText(_AL("0"));
		m_pTxt_Number->Enable(false);
		m_pBtn_Produce->Enable(false);
//		AUIClockIcon *pClock = m_pItem_out[0]->GetClockIcon();
//		pClock->SetProgressRange(0, 1);
//		pClock->SetProgressPos(1);
	}
	else
	{
		nMax = m_pTxt_Number->GetData();
		int nNum = a_atoi(m_pTxt_Number->GetText());
		if( nNum < 0 )
			nNum = 0;
		if( nNum > nMax )
			nNum = nMax;
		if( m_pPrgs_1->GetData() == 0 && nNum > 0 && m_pItem_out[0]->GetData() != 0 )
			m_pBtn_Produce->Enable(true);
		else
			m_pBtn_Produce->Enable(false);
		m_pTxt_Number->Enable(true);
	}
}

void CDlgSoulCombine::UpdateMaxAmount()
{
	const RECIPE_ESSENCE *pRecipe;
	DATA_TYPE datatype;
	CECInventory *pPack = GetHostPlayer()->GetPack();
	elementdataman *pDataMan = g_pGame->GetElementDataMan();

	const VEC_AUITREEVIEW_ITEM vecItems = m_pTv_Recipe->GetDisplayItems();

	for(DWORD i = 0; i < vecItems.size(); i++ )		
	{
		P_AUITREEVIEW_ITEM pItem = vecItems[i];
		if( m_pTv_Recipe->GetItemData(pItem) != 0 )
		{
			pRecipe = (RECIPE_ESSENCE*)pDataMan->get_data_ptr(m_pTv_Recipe->GetItemData(pItem), ID_SPACE_RECIPE, datatype);
			if( datatype == DT_RECIPE_ESSENCE )
			{
//				m_pTv_Recipe->SetItemTextColor(pItem, A3DCOLORRGB(255, 255, 255));
				int nMax, nOne;
				nMax = 9999;
				for(int i = 0; i < CDLGSOULCOMBINE_MAX_MATERIALS; i++ )
					if( pRecipe->materials[i].id != 0 )
					{
						nOne = pPack->GetItemTotalNum(pRecipe->materials[i].id) / pRecipe->materials[i].num;
						if( nOne < nMax )
							nMax = nOne;
					}
				m_pTv_Recipe->SetItemDataPtr(pItem, (void*)nMax);
				if( nMax > 0 )
				{
					ACString strText;
					strText.Format(_AL("%s [%d]"), pRecipe->name, nMax);
					m_pTv_Recipe->SetItemText(pItem, strText);
				}
				else
					m_pTv_Recipe->SetItemText(pItem, pRecipe->name);
			}
		}
	}
}

void CDlgSoulCombine::AddOneRecipe(int idRecipe)
{
	const RECIPE_ESSENCE *pRecipe;
	DATA_TYPE datatype;
	elementdataman *pDataMan = g_pGame->GetElementDataMan();
	CECHostPlayer *pHost = GetHostPlayer();
	CECInventory *pPack = GetHostPlayer()->GetPack();
	P_AUITREEVIEW_ITEM pItem;
	pRecipe = (RECIPE_ESSENCE*)pDataMan->get_data_ptr(idRecipe, ID_SPACE_RECIPE, datatype);
	if( m_pCombo_ProList->GetCurSel() < 1 || pRecipe->level == m_pCombo_ProList->GetCurSel() )
	{
		P_AUITREEVIEW_ITEM pRootItem = m_pTv_Recipe->GetRootItem();
		P_AUITREEVIEW_ITEM pMajorItem = m_pTv_Recipe->GetFirstChildItem(pRootItem);
		while(pMajorItem != NULL && m_pTv_Recipe->GetItemData(pMajorItem) != pRecipe->id_major_type )
			pMajorItem = m_pTv_Recipe->GetNextSiblingItem(pMajorItem);
		if( !pMajorItem )
		{
			const RECIPE_MAJOR_TYPE *pMajorRecipe = (RECIPE_MAJOR_TYPE*)pDataMan->get_data_ptr(
				pRecipe->id_major_type, ID_SPACE_RECIPE, datatype);
			if( datatype != DT_RECIPE_MAJOR_TYPE )
				return;

			pMajorItem = m_pTv_Recipe->InsertItem(pMajorRecipe->name, pRootItem);
			m_pTv_Recipe->SetItemData(pMajorItem, pRecipe->id_major_type);
		}

		P_AUITREEVIEW_ITEM pSubItem = m_pTv_Recipe->GetFirstChildItem(pMajorItem);
		while(pSubItem != NULL && m_pTv_Recipe->GetItemData(pSubItem) != pRecipe->id_sub_type )
			pSubItem = m_pTv_Recipe->GetNextSiblingItem(pSubItem);
		if( !pSubItem )
		{
			const RECIPE_SUB_TYPE *pSubRecipe = (RECIPE_SUB_TYPE*)pDataMan->get_data_ptr(
				pRecipe->id_sub_type, ID_SPACE_RECIPE, datatype);
			if( datatype != DT_RECIPE_SUB_TYPE )
				return;

			pSubItem = m_pTv_Recipe->InsertItem(pSubRecipe->name, pMajorItem);
			m_pTv_Recipe->SetItemData(pSubItem, pRecipe->id_sub_type);
		}

		int nLevel = pRecipe->level;
		int idEuqipMask = 0;
		int nGender = 0;
		int nLevel1 = 0;
		int idEuqipMask1 = 0;
		int nGender1 = 0;
		bool bEquip = false;
		pItem = m_pTv_Recipe->GetFirstChildItem(pSubItem);
		while(pItem != NULL)
		{
			//if( a_stricmp(m_pTv_Recipe->GetItemText(pItem), pRecipe->name) < 0 )
			//	break;
			pItem = m_pTv_Recipe->GetNextSiblingItem(pItem);
		}

		if( pItem == NULL )
			pItem = m_pTv_Recipe->InsertItem(pRecipe->name, pSubItem);
		else if( pItem == m_pTv_Recipe->GetFirstChildItem(pSubItem) )
			pItem = m_pTv_Recipe->InsertItem(pRecipe->name, pSubItem, AUITREEVIEW_FIRST);
		else
			pItem = m_pTv_Recipe->InsertItem(pRecipe->name, pSubItem, m_pTv_Recipe->GetPrevSiblingItem(pItem));

		m_pTv_Recipe->SetItemData(pItem, idRecipe);
	}
}

void CDlgSoulCombine::UpdateRecipe(int idRecipe)
{
	const RECIPE_ESSENCE *pRecipe;
	DATA_TYPE datatype;
	elementdataman *pDataMan = g_pGame->GetElementDataMan();
	CECHostPlayer *pHost = GetHostPlayer();
	CECInventory *pPack = GetHostPlayer()->GetPack();
	m_pItem_out[0]->SetData(idRecipe);
	pRecipe = (RECIPE_ESSENCE*)pDataMan->get_data_ptr(idRecipe, ID_SPACE_RECIPE, datatype);
	if( datatype == DT_RECIPE_ESSENCE )
	{
		CECIvtrItem *pItem = CECIvtrItem::CreateItem(pRecipe->products[0].id_to_make, 0, 1);
		pItem->GetDetailDataFromLocal();
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		m_pItem_out[0]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		const wchar_t* szDesc = pItem->GetDesc();
		AUICTranslate trans;
		if( szDesc )
			m_pItem_out[0]->SetHint(trans.Translate(szDesc) + GetGameUIMan()->GetCompareEquipDesc(pItem));
		else
			m_pItem_out[0]->SetHint(pItem->GetName());
		m_pTxt_out[0]->SetText(pItem->GetName());

		AUIClockIcon *pClock;
		pClock = m_pItem_out[0]->GetClockIcon();
		int nMax = 0;
		int nCool = pRecipe->cool_type == 0 ? 0 : GetHostPlayer()->GetProduceCoolTime(pRecipe->cool_type, &nMax);
		if( nCool > 0 )
		{
			pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
			pClock->SetProgressRange(0, nMax);
			pClock->SetProgressPos(nMax - nCool);
			m_pItem_out[0]->SetData(0);
			ACString strText;
			strText.Format(GetStringFromTable(1009), int((nCool - 1)  / 1000) + 1);
		}
		else
		{
			pClock->SetProgressRange(0, 1);
			pClock->SetProgressPos(1);
		}
		m_pTxt_lab[0]->Show(true);
		m_pTxt_out[0]->Show(true);
		m_pItem_out[0]->Show(true);

		delete pItem;


		if (0!=pRecipe->products[1].id_to_make)
		{
			pItem = CECIvtrItem::CreateItem(pRecipe->products[1].id_to_make, 0, 1);
			pItem->GetDetailDataFromLocal();
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			m_pItem_out[1]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
			szDesc = pItem->GetDesc();
			if( szDesc )
				m_pItem_out[1]->SetHint(trans.Translate(szDesc) + GetGameUIMan()->GetCompareEquipDesc(pItem));
			else
				m_pItem_out[1]->SetHint(pItem->GetName());

			pClock = m_pItem_out[1]->GetClockIcon();
			nMax = 0;
			nCool = pRecipe->cool_type == 0 ? 0 : GetHostPlayer()->GetProduceCoolTime(pRecipe->cool_type, &nMax);
			if( nCool > 0 )
			{
				pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
				pClock->SetProgressRange(0, nMax);
				pClock->SetProgressPos(nMax - nCool);
				m_pItem_out[1]->SetData(0);
				ACString strText;
				strText.Format(GetStringFromTable(1009), int((nCool - 1)  / 1000) + 1);
			}
			else
			{
				pClock->SetProgressRange(0, 1);
				pClock->SetProgressPos(1);
			}
			
			delete pItem;
			m_pTxt_lab[1]->Show(true);
			m_pTxt_out[1]->Show(true);
			m_pItem_out[1]->Show(true);
		}
		else
		{
			m_pTxt_lab[1]->Show(false);
			m_pTxt_out[1]->Show(false);
			m_pItem_out[1]->Show(false);
		}

		if (0!=pRecipe->products[2].id_to_make)
		{
			pItem = CECIvtrItem::CreateItem(pRecipe->products[2].id_to_make, 0, 1);
			pItem->GetDetailDataFromLocal();
			af_GetFileTitle(pItem->GetIconFile(), strFile);
			strFile.MakeLower();
			m_pItem_out[2]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
				GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
			szDesc = pItem->GetDesc();
			if( szDesc )
				m_pItem_out[2]->SetHint(trans.Translate(szDesc) + GetGameUIMan()->GetCompareEquipDesc(pItem));
			else
				m_pItem_out[2]->SetHint(pItem->GetName());

			pClock = m_pItem_out[2]->GetClockIcon();
			nMax = 0;
			nCool = pRecipe->cool_type == 0 ? 0 : GetHostPlayer()->GetProduceCoolTime(pRecipe->cool_type, &nMax);
			if( nCool > 0 )
			{
				pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
				pClock->SetProgressRange(0, nMax);
				pClock->SetProgressPos(nMax - nCool);
				m_pItem_out[2]->SetData(0);
				ACString strText;
				strText.Format(GetStringFromTable(1009), int((nCool - 1)  / 1000) + 1);
			}
			else
			{
				pClock->SetProgressRange(0, 1);
				pClock->SetProgressPos(1);
			}

			delete pItem;
			m_pTxt_lab[2]->Show(true);
			m_pTxt_out[2]->Show(true);
			m_pItem_out[2]->Show(true);
		}
		else
		{
			m_pTxt_lab[2]->Show(false);
			m_pTxt_out[2]->Show(false);
			m_pItem_out[2]->Show(false);
		}

		ACHAR szText[20];
		int nNeed;
		int j, k = 0;
		for(j = 0; j < CDLGSOULCOMBINE_MAX_MATERIALS; j++ )
		{
			if( pRecipe->materials[j].id <= 0 || pRecipe->materials[j].num <= 0 )
				continue;

			m_pImgItem[k]->Show(true);
			CECIvtrItem *pItem = CECIvtrItem::CreateItem(pRecipe->materials[j].id, 0, pRecipe->materials[j].num);
			if( pItem )
			{
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				m_pImgItem[k]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
				const wchar_t* szDesc = pItem->GetDesc();
				if( szDesc )
					m_pImgItem[k]->SetHint(trans.Translate(szDesc));
				else
					m_pImgItem[k]->SetHint(pItem->GetName());
				m_pTxt_name[k]->SetText(pItem->GetName());
				delete pItem;
			}
			else
				ASSERT(false);

			nNeed = pRecipe->materials[j].num;
			nMax = pPack->GetItemTotalNum(pRecipe->materials[j].id);
			a_sprintf(szText, _AL("%d/%d"), nMax, nNeed);
			m_pTxt_no[k]->Show(true);
			m_pTxt_no[k]->SetText(szText);
			m_pTxt_name[k]->Show(true);

			if( nMax < nNeed )
				m_pTxt_no[k]->SetColor(A3DCOLORRGB(255, 0, 0));
			else
				m_pTxt_no[k]->SetColor(A3DCOLORRGB(255, 255, 255));
			if( nMax == 0 )
				m_pImgItem[k]->SetColor(A3DCOLORRGB(128, 128, 128));
			else
				m_pImgItem[k]->SetColor(A3DCOLORRGB(255, 255, 255));
			k++;
		}
		for (;k<CDLGSOULCOMBINE_MAX_MATERIALS;k++)
		{
			m_pImgItem[k]->Show(false);
			m_pTxt_no[k]->Show(false);
			m_pTxt_name[k]->Show(false);
		}
	}
}

void CDlgSoulCombine::CleanUpRecipe()
{
	DATA_TYPE datatype;
	elementdataman *pDataMan = g_pGame->GetElementDataMan();
	CECHostPlayer *pHost = GetHostPlayer();
	CECInventory *pPack = GetHostPlayer()->GetPack();
	P_AUITREEVIEW_ITEM pRootItem = m_pTv_Recipe->GetRootItem();
	P_AUITREEVIEW_ITEM pMajorItem = m_pTv_Recipe->GetFirstChildItem(pRootItem);
	while(pMajorItem != NULL )
	{
		P_AUITREEVIEW_ITEM pSubItem = m_pTv_Recipe->GetFirstChildItem(pMajorItem);
		while(pSubItem != NULL)
		{
			P_AUITREEVIEW_ITEM pItem = m_pTv_Recipe->GetFirstChildItem(pSubItem);
			while(pItem != NULL)
			{
				int idRecipe = m_pTv_Recipe->GetItemData(pItem);
				RECIPE_ESSENCE* pRecipe = (RECIPE_ESSENCE*)pDataMan->get_data_ptr(idRecipe, ID_SPACE_RECIPE, datatype);
				if( pRecipe && datatype == DT_RECIPE_ESSENCE )
				{
					int j;
					bool bDelete = false;
					for(j = 0; j < CDLGSOULCOMBINE_MAX_MATERIALS; j++ )
					{
						if( pRecipe->materials[j].id <= 0 || pRecipe->materials[j].num <= 0 )
							continue;

						if( pRecipe->materials[j].num > pPack->GetItemTotalNum(pRecipe->materials[j].id) )
						{
							bDelete = true;
							break;
						}
					}
					if( bDelete )
					{
						P_AUITREEVIEW_ITEM pOldItem = pItem;
						pItem = m_pTv_Recipe->GetNextSiblingItem(pItem);
						m_pTv_Recipe->DeleteItem(pOldItem);
						continue;
					}
				}

				pItem = m_pTv_Recipe->GetNextSiblingItem(pItem);
			}
			if( m_pTv_Recipe->GetFirstChildItem(pSubItem) == NULL )
			{
				P_AUITREEVIEW_ITEM pOldItem = pSubItem;
				pSubItem = m_pTv_Recipe->GetNextSiblingItem(pSubItem);
				m_pTv_Recipe->DeleteItem(pOldItem);
				continue;
			}
			pSubItem = m_pTv_Recipe->GetNextSiblingItem(pSubItem);
		}
		if( m_pTv_Recipe->GetFirstChildItem(pMajorItem) == NULL )
		{
			P_AUITREEVIEW_ITEM pOldItem = pMajorItem;
			pMajorItem = m_pTv_Recipe->GetNextSiblingItem(pMajorItem);
			m_pTv_Recipe->DeleteItem(pOldItem);
			continue;
		}
		pMajorItem = m_pTv_Recipe->GetNextSiblingItem(pMajorItem);
	}
}

void CDlgSoulCombine::ProduceOne()
{
	int nNum = a_atoi(m_pTxt_Number->GetText());
	m_pPrgs_1->SetData(0);
	m_pPrgs_1->SetAutoProgress(0, 0, 0);
	if( m_idProducing == m_idCurRecipe )
	{
		nNum--;
		ACHAR szText[20];
		a_sprintf(szText, _AL("%d"), nNum);
		m_pTxt_Number->SetText(szText);
		if( nNum > 0 && m_pTv_Recipe->GetSelectedItem() )
		{
			const RECIPE_ESSENCE *pRecipe;
			DATA_TYPE datatype;
			elementdataman *pDataMan = g_pGame->GetElementDataMan();
			pRecipe = (RECIPE_ESSENCE*)pDataMan->get_data_ptr(m_pTv_Recipe->GetItemData(m_pTv_Recipe->GetSelectedItem()), ID_SPACE_RECIPE, datatype);
			if( pRecipe && datatype == DT_RECIPE_ESSENCE )
			{
				if( pRecipe->cool_type == 0 )
					OnCommand_begin("");
			}
		}
		else
			m_idProducing = 0;
	}
	if( m_pCombo_ProList->GetCurSel() == 1 )
		CleanUpRecipe();
}

void CDlgSoulCombine::ProduceEnd()
{
	m_pPrgs_1->SetData(0);
	m_pPrgs_1->SetAutoProgress(0, 0, 0);
}

void CDlgSoulCombine::LoadAvailableSoulRecipe()
{
	const EQUIP_SOUL_MELD_SERVICE *pRecipe;
	DATA_TYPE datatype;
	if (!(GetGameUIMan()->m_pCurNPCEssence))
	{
		return;
	}
	NPC_ESSENCE *pEssence = GetGameUIMan()->m_pCurNPCEssence;
	int idEssence = pEssence->id_equip_soul_service;
	m_idTerritory = pEssence->id_territory;
	elementdataman *pDataMan = g_pGame->GetElementDataMan();
	pRecipe = (EQUIP_SOUL_MELD_SERVICE*)pDataMan->get_data_ptr(idEssence, ID_SPACE_ESSENCE, datatype);
	if (pRecipe)
	{
		for (int i=0; i<500; i++)
		{
			if (pRecipe->prescription[i]!=0)
			{
				AddOneRecipe(pRecipe->prescription[i]);
			}
			else
				return;
		}
	}
	/*
	DATA_TYPE dt;
	const RECIPE_ESSENCE *pRecipe;
	elementdataman *pDataMan = g_pGame->GetElementDataMan();
	int idEssence = g_pGame->GetElementDataMan()->get_first_data_id(ID_SPACE_RECIPE, dt);

	while (idEssence)
	{
		if (dt == DT_RECIPE_ESSENCE)
		{
			pRecipe = (RECIPE_ESSENCE*)pDataMan->get_data_ptr(idEssence, ID_SPACE_RECIPE, dt);
			if (pRecipe->recipe_class == 2)
			{
				AddOneRecipe(idEssence);
			}
		}

		idEssence = g_pGame->GetElementDataMan()->get_next_data_id(ID_SPACE_RECIPE, dt);
	}*/
}