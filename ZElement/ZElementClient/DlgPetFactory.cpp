// Filename	: DlgPetFactory.cpp
// Creator	: Xiao Zhou
// Date		: 2008/4/24

#include "AFI.h"
#include "AUI\\AUICTranslate.h"
#include "DlgPetFactory.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_Inventory.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "EC_Skill.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgPetFactory, CDlgBase)

AUI_ON_COMMAND("add",			OnCommand_add)
AUI_ON_COMMAND("minus",			OnCommand_minus)
AUI_ON_COMMAND("max",			OnCommand_max)
AUI_ON_COMMAND("begin",			OnCommand_begin)
AUI_ON_COMMAND("stop",			OnCommand_stop)
AUI_ON_COMMAND("prolist",		OnCommand_prolist)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

const int petfactory_recipelevel[] = 
{
	0,
	80,
	160,
	300,
	500,
	1000
};

CDlgPetFactory::CDlgPetFactory()
{
	m_idCurRecipe = 0;
	m_idProducing = 0;
	m_bFirstShow = true;
	memset(m_nLive, 0, sizeof(int) * 7);
}

CDlgPetFactory::~CDlgPetFactory()
{
}

bool CDlgPetFactory::OnInitDialog()
{
	int i;
	char szItem[40];

	m_pPrgs_1 = (PAUIPROGRESS)GetDlgItem("Prgs_1");
	m_pTxt_Number = (PAUIEDITBOX)GetDlgItem("Txt_Number");
	m_pTxt_out = (PAUILABEL)GetDlgItem("Txt_out");
	m_pTxt_CoolTime = (PAUILABEL)GetDlgItem("Txt_CoolTime");
	m_pItem_out = (PAUIIMAGEPICTURE)GetDlgItem("Item_out");
	m_pBtn_Produce = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Produce");
	m_pTv_Recipe = (PAUITREEVIEW)GetDlgItem("Tv_Recipe");

	for( i = 0; i < CDLGPETFACTORY_MAX_MATERIALS; i++ )
	{
		sprintf(szItem, "Txt_name%d", i + 1);
		m_pTxt_name[i] = (PAUILABEL)GetDlgItem(szItem);

		sprintf(szItem, "Txt_no%d", i + 1);
		m_pTxt_no[i] = (PAUILABEL)GetDlgItem(szItem);

		sprintf(szItem, "Item_%d", i + 1);
		m_pImgItem[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}

	for( i = 0; i < CDLGPETFACTORY_MAX_PRODUCTS; i++ )
	{
		sprintf(szItem, "Txt_Product%d", i + 1);
		m_pTxt_Product[i] = (PAUILABEL)GetDlgItem(szItem);

		sprintf(szItem, "Txt_Rate%d", i + 1);
		m_pTxt_Rate[i] = (PAUILABEL)GetDlgItem(szItem);

		sprintf(szItem, "Item_Product%d", i + 1);
		m_pItem_Product[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}

	m_pCombo_ProList = (PAUICOMBOBOX)GetDlgItem("Combo_ProList");
	if( m_pCombo_ProList )
	{
		m_pCombo_ProList->AddString(GetStringFromTable(5300));
		m_pCombo_ProList->AddString(GetStringFromTable(5301));
		ACHAR szText[20];
		for(i = 0; i < 6; i++)
		{
			a_sprintf(szText, GetStringFromTable(5303), i + 1);
			m_pCombo_ProList->AddString(szText);
		}
		m_pCombo_ProList->SetCurSel(0);
	}

	return true;
}

bool CDlgPetFactory::Render()
{
	UpdateMaxAmount();
	UpdateSelectItem();
	return CDlgBase::Render();
}

void CDlgPetFactory::OnTick()
{
	CECHostPlayer *pHost = GetHostPlayer();
	ACHAR szText[100];
	while( pHost->GetRecipeCount() > m_nRecipeSum )
	{
		AddOneRecipe(pHost->GetRecipeID(m_nRecipeSum));
		m_nRecipeSum++;
	}
	CECInventory *pPetPack = GetHostPlayer()->GetPetPack();
	memset(m_nLive, 0, sizeof(int) * 7);
	m_nLive[6] = GetHostPlayer()->GetPetConstruction();
	int num = 0;
	int addon = 0;
	float f = 1.0f;
	int i;
	for(i = 0; i < pPetPack->GetSize(); i++)
	{
		CECPetBedge *pPet = (CECPetBedge*)pPetPack->GetItem(i);
		if( pPet && pPet->GetEssence().level >= 60 )
		{
			num++;
			m_nLive[0] += pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::ACUITY];
			m_nLive[1] += pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::STAMINA];
			m_nLive[2] += pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::SAWY];
			m_nLive[3] += pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::DOWRY];
			m_nLive[4] += pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::WISDOM];
			m_nLive[5] += pPet->GetEssence().make_attr[itemdataman::_pet_bedge_essence::CNR];
			if( pPet->GetEssence().rank == 7 )
				addon += 1;
			else if( pPet->GetEssence().rank == 8 )
				addon += 3;
			else if( pPet->GetEssence().rank == 9 )
				addon += 5;
			else if( pPet->GetEssence().rank == 10 )
				addon += 10;
			else if( pPet->GetEssence().rank == 11 )
				addon += 15;
			else if( pPet->GetEssence().rank == 12 )
			{
				addon += 20;
				f = 1.15f;
			}
		}
	}
	for(i = 0; i < 7; i++)
	{
		char szName[20];
		sprintf(szName, "Txt_Live%d", i);
		if( i != 6 )
		{
			if( addon > 0 || f > 1.0f )
				a_sprintf(szText, _AL("%d^00ff00%+d"), m_nLive[i], int((m_nLive[i] + addon) * f) - m_nLive[i]);
			else
				a_sprintf(szText, _AL("%d"), m_nLive[i]);
			GetDlgItem(szName)->SetText(szText);
			m_nLive[i] = int((m_nLive[i] + addon) * f); 
		}
		else
		{
			a_sprintf(szText, _AL("%d"), m_nLive[i]);
			GetDlgItem(szName)->SetText(szText);
		}
	}
	a_sprintf(szText, _AL("%d/%d"), num, min(2 + GetHostPlayer()->GetBasicProps().iLevel / 5, 32));
	GetDlgItem("Txt_Num")->SetText(szText);
}

void CDlgPetFactory::OnShowDialog()
{
	GetGameUIMan()->GetDialog("Win_Produce")->Show(false);
	GetHostPlayer()->SetProducing(true);
	if( m_bFirstShow )
		OnCommand_prolist("");
}

void CDlgPetFactory::OnCommand_add(const char * szCommand)
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

void CDlgPetFactory::OnCommand_minus(const char * szCommand)
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

void CDlgPetFactory::OnCommand_max(const char * szCommand)
{
	if( m_pPrgs_1->GetData() > 0 ) return;

	ACHAR szText[40];
	int nMax = m_pTxt_Number->GetData();
	a_sprintf(szText, _AL("%d"), nMax);
	m_pTxt_Number->SetText(szText);
	OnCommand_begin("");
}

void CDlgPetFactory::OnCommand_begin(const char * szCommand)
{
	if( m_pPrgs_1->GetData() > 0 ) return;
	if( m_pItem_out->GetData() == 0 ) return;
	
	int nNum = a_atoi(m_pTxt_Number->GetText());
	if( nNum > 0 )
	{
		m_idProducing = m_idCurRecipe;
		GetGameSession()->c2s_CmdProduceItem(m_pItem_out->GetData());
		m_pBtn_Produce->Enable(false);
	}
}

void CDlgPetFactory::OnCommand_stop(const char * szCommand)
{
	PAUIPROGRESS pProgress = (PAUIPROGRESS)m_pPrgs_1;

	m_idProducing = 0;
	if( pProgress->GetData() > 0 )
		GetGameSession()->c2s_CmdCancelAction();
}

void CDlgPetFactory::OnCommand_prolist(const char * szCommand)
{
	int i;
	for(i = 0; i < CDLGPETFACTORY_MAX_MATERIALS; i++ )
	{
		m_pImgItem[i]->Show(false);
		m_pTxt_no[i]->Show(false);
		m_pTxt_name[i]->Show(false);
	}
	m_pTv_Recipe->DeleteAllItems();
	m_nRecipeSum = GetHostPlayer()->GetRecipeCount();
	for(i = 0; i < m_nRecipeSum; i++ )
		AddOneRecipe(GetHostPlayer()->GetRecipeID(i));
	if( m_pCombo_ProList->GetCurSel() == 1 )
		CleanUpRecipe();
	m_bFirstShow = false;
}

void CDlgPetFactory::OnCommand_CANCEL(const char * szCommand)
{
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

void CDlgPetFactory::UpdateSelectItem()
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
						m_pTxt_out->SetText(szText);
					}
				}
			}
		}
	}
	else
		m_idCurRecipe = 0;
	if( m_idProducing != 0 && m_idCurRecipe != m_idProducing )
	{
		m_idProducing = 0;
		OnCommand_stop("");
	}

	if( m_idCurRecipe == 0 )
	{
		int j;
		for(j = 0; j < CDLGPETFACTORY_MAX_MATERIALS; j++ )
		{
			m_pImgItem[j]->Show(false);
			m_pTxt_no[j]->Show(false);
			m_pTxt_name[j]->Show(false);
		}
		for(j = 0; j < CDLGPETFACTORY_MAX_PRODUCTS; j++ )
		{
			m_pItem_Product[j]->Show(false);
			m_pTxt_Product[j]->Show(false);
			m_pTxt_Rate[j]->Show(false);
		}
		for(j = 0; j < 7; j++)
		{
			char szName[20];
			sprintf(szName, "Txt_ReqLive%d", j);
			GetDlgItem(szName)->SetText(_AL(""));
		}
		m_pTxt_Number->SetData(0);
		m_pTxt_Number->SetText(_AL("0"));
		m_pTxt_Number->Enable(false);
		m_pBtn_Produce->Enable(false);
		m_pItem_out->ClearCover();
		AUIClockIcon *pClock = m_pItem_out->GetClockIcon();
		pClock->SetProgressRange(0, 1);
		pClock->SetProgressPos(1);
		m_pTxt_out->SetText(_AL(""));
		m_pTxt_CoolTime->SetText(_AL(""));
	}
	else
	{
		nMax = m_pTxt_Number->GetData();
		int nNum = a_atoi(m_pTxt_Number->GetText());
		if( nNum < 0 )
			nNum = 0;
		if( nNum > nMax )
			nNum = nMax;
		DATA_TYPE datatype;
		RECIPE_ESSENCE* pRecipe = (RECIPE_ESSENCE*)pDataMan->get_data_ptr(m_idCurRecipe, ID_SPACE_RECIPE, datatype);
		if( m_pPrgs_1->GetData() == 0 && nNum > 0  && m_pItem_out->GetData() != 0 )
			m_pBtn_Produce->Enable(true);
		else
			m_pBtn_Produce->Enable(false);
		m_pTxt_Number->Enable(true);
	}
}

void CDlgPetFactory::UpdateMaxAmount()
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
				if( pRecipe->level > GetHostPlayer()->GetProduceSkillLevel() )
				{
					m_pTv_Recipe->SetItemText(pItem, pRecipe->name);
					m_pTv_Recipe->SetItemTextColor(pItem, A3DCOLORRGB(255, 0, 0));
					m_pTv_Recipe->SetItemDataPtr(pItem, (void*)0);
				}
				else
				{
					m_pTv_Recipe->SetItemTextColor(pItem, A3DCOLORRGB(255, 255, 255));
					int nMax, nOne;
					nMax = 9999;
					for(int i = 0; i < CDLGPETFACTORY_MAX_MATERIALS; i++ )
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
}

void CDlgPetFactory::AddOneRecipe(int idRecipe)
{
	const RECIPE_ESSENCE *pRecipe;
	DATA_TYPE datatype;
	elementdataman *pDataMan = g_pGame->GetElementDataMan();
	CECHostPlayer *pHost = GetHostPlayer();
	CECInventory *pPack = GetHostPlayer()->GetPack();
	P_AUITREEVIEW_ITEM pItem;
	pRecipe = (RECIPE_ESSENCE*)pDataMan->get_data_ptr(idRecipe, ID_SPACE_RECIPE, datatype);
	if( datatype == DT_RECIPE_ESSENCE && pRecipe->recipe_class == 1 )
	{
		int level = 0;
		for(int i = 0; i < 6; i++)
			if( petfactory_recipelevel[i] == pRecipe->required[6] )
			{
				level = i;
				break;
			}
		if( m_pCombo_ProList->GetCurSel() > 1 && level + 1 != m_pCombo_ProList->GetCurSel() - 1)
			return;

		CECIvtrItem *pIvtrItem = CECIvtrItem::CreateItem(pRecipe->products[0].id_to_make, 0, 1);
		if( !pIvtrItem )
			return;

		P_AUITREEVIEW_ITEM pRootItem = m_pTv_Recipe->GetRootItem();
		P_AUITREEVIEW_ITEM pMajorItem = m_pTv_Recipe->GetFirstChildItem(pRootItem);
		while(pMajorItem != NULL && m_pTv_Recipe->GetItemData(pMajorItem) < pRecipe->id_major_type )
			pMajorItem = m_pTv_Recipe->GetNextSiblingItem(pMajorItem);
		if( !pMajorItem || m_pTv_Recipe->GetItemData(pMajorItem) != pRecipe->id_major_type )
		{
			const RECIPE_MAJOR_TYPE *pMajorRecipe = (RECIPE_MAJOR_TYPE*)pDataMan->get_data_ptr(
				pRecipe->id_major_type, ID_SPACE_RECIPE, datatype);
			if( datatype != DT_RECIPE_MAJOR_TYPE )
				return;

			if( pMajorItem == NULL )
				pMajorItem = m_pTv_Recipe->InsertItem(pMajorRecipe->name, pRootItem);
			else if( pMajorItem == m_pTv_Recipe->GetFirstChildItem(pRootItem) )
				pMajorItem = m_pTv_Recipe->InsertItem(pMajorRecipe->name, pRootItem, AUITREEVIEW_FIRST);
			else
				pMajorItem = m_pTv_Recipe->InsertItem(pMajorRecipe->name, pRootItem, m_pTv_Recipe->GetPrevSiblingItem(pMajorItem));

			m_pTv_Recipe->SetItemData(pMajorItem, pRecipe->id_major_type);
		}

		P_AUITREEVIEW_ITEM pSubItem = m_pTv_Recipe->GetFirstChildItem(pMajorItem);
		while(pSubItem != NULL && m_pTv_Recipe->GetItemData(pSubItem) < pRecipe->id_sub_type )
			pSubItem = m_pTv_Recipe->GetNextSiblingItem(pSubItem);
		if( !pSubItem || m_pTv_Recipe->GetItemData(pSubItem) != pRecipe->id_sub_type )
		{
			const RECIPE_SUB_TYPE *pSubRecipe = (RECIPE_SUB_TYPE*)pDataMan->get_data_ptr(
				pRecipe->id_sub_type, ID_SPACE_RECIPE, datatype);
			if( datatype != DT_RECIPE_SUB_TYPE )
				return;

			if( pSubItem == NULL )
				pSubItem = m_pTv_Recipe->InsertItem(pSubRecipe->name, pMajorItem);
			else if( pSubItem == m_pTv_Recipe->GetFirstChildItem(pMajorItem) )
				pSubItem = m_pTv_Recipe->InsertItem(pSubRecipe->name, pMajorItem, AUITREEVIEW_FIRST);
			else
				pSubItem = m_pTv_Recipe->InsertItem(pSubRecipe->name, pMajorItem, m_pTv_Recipe->GetPrevSiblingItem(pSubItem));

			m_pTv_Recipe->SetItemData(pSubItem, pRecipe->id_sub_type);
		}

		int nLevel = pRecipe->level;
		int idEuqipMask = 0;
		int nGender = 0;
		int nLevel1 = 0;
		int idEuqipMask1 = 0;
		int nGender1 = 0;
		bool bEquip = false;
		if( pIvtrItem->GetClassID() == CECIvtrItem::ICID_EQUIP )
		{
			CECIvtrEquip *pIvtrEquip = (CECIvtrEquip*)pIvtrItem;
			idEuqipMask = pIvtrEquip->GetEquipMask();
			nGender = pIvtrEquip->GetDBEssence()->require_gender;
			bEquip = true;
		}
		pItem = m_pTv_Recipe->GetFirstChildItem(pSubItem);
		while(pItem != NULL)
		{
			int idRecipe1 =  m_pTv_Recipe->GetItemData(pItem);
			RECIPE_ESSENCE* pRecipe1 = (RECIPE_ESSENCE*)pDataMan->get_data_ptr(idRecipe1, ID_SPACE_RECIPE, datatype);

			if( bEquip )
			{
				CECIvtrItem *pIvtrItem1 = CECIvtrItem::CreateItem(pRecipe1->products[0].id_to_make, 0, 1);
				CECIvtrEquip *pIvtrEquip1 = (CECIvtrEquip*)pIvtrItem1;
				idEuqipMask1 = pIvtrEquip1->GetEquipMask();
				nGender1 = pIvtrEquip1->GetDBEssence()->require_gender;
				delete pIvtrItem1;
			}
			
			if( datatype == DT_RECIPE_ESSENCE )
				nLevel1 = pRecipe1->level;
			else
				nLevel1 = 100;
			if( nLevel < nLevel1 )
				break;
			else if( nLevel == nLevel1 )
			{
				if( idEuqipMask < idEuqipMask1 )
					break;
				else if( idEuqipMask == idEuqipMask1 )
				{
					if( nGender < nGender1 )
						break;
					else if( nGender == nGender1 )
					{
						if( a_stricmp(m_pTv_Recipe->GetItemText(pItem), pRecipe->name) < 0 )
							break;
					}
				}
			}
			pItem = m_pTv_Recipe->GetNextSiblingItem(pItem);
		}

		if( pItem == NULL )
			pItem = m_pTv_Recipe->InsertItem(pRecipe->name, pSubItem);
		else if( pItem == m_pTv_Recipe->GetFirstChildItem(pSubItem) )
			pItem = m_pTv_Recipe->InsertItem(pRecipe->name, pSubItem, AUITREEVIEW_FIRST);
		else
			pItem = m_pTv_Recipe->InsertItem(pRecipe->name, pSubItem, m_pTv_Recipe->GetPrevSiblingItem(pItem));

		m_pTv_Recipe->SetItemData(pItem, idRecipe);
		delete pIvtrItem;
	}
}

void CDlgPetFactory::UpdateRecipe(int idRecipe)
{
	const RECIPE_ESSENCE *pRecipe;
	DATA_TYPE datatype;
	elementdataman *pDataMan = g_pGame->GetElementDataMan();
	CECHostPlayer *pHost = GetHostPlayer();
	CECInventory *pPack = GetHostPlayer()->GetPack();
	m_pItem_out->SetData(idRecipe);
	pRecipe = (RECIPE_ESSENCE*)pDataMan->get_data_ptr(idRecipe, ID_SPACE_RECIPE, datatype);
	if( datatype == DT_RECIPE_ESSENCE )
	{
		CECIvtrItem *pItem = CECIvtrItem::CreateItem(pRecipe->products[0].id_to_make, 0, 1);
		pItem->GetDetailDataFromLocal();
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		m_pItem_out->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		AUIClockIcon *pClock;
		pClock = m_pItem_out->GetClockIcon();
		int nMax = 0;
		int nCool = pRecipe->cool_type == 0 ? 0 : GetHostPlayer()->GetProduceCoolTime(pRecipe->cool_type, &nMax);
		if( nCool > 0 )
		{
			pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
			pClock->SetProgressRange(0, nMax);
			pClock->SetProgressPos(nMax - nCool);
			m_pItem_out->SetData(0);
			ACString strText;
			strText.Format(GetStringFromTable(1009), int((nCool - 1)  / 1000) + 1);
			m_pTxt_CoolTime->SetText(strText);
		}
		else
		{
			pClock->SetProgressRange(0, 1);
			pClock->SetProgressPos(1);
			m_pTxt_CoolTime->SetText(_AL(""));
		}

		const wchar_t* szDesc = pItem->GetDesc();
		AUICTranslate trans;
		if( szDesc )
			m_pItem_out->SetHint(trans.Translate(szDesc));
		else
			m_pItem_out->SetHint(pItem->GetName());
		m_pTxt_out->SetText(pItem->GetName());
		delete pItem;

		ACHAR szText[20];
		int nNeed;
		int j, k = 0;
		for(j = 0; j < CDLGPETFACTORY_MAX_MATERIALS; j++ )
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
				AUICTranslate trans;
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
		for(j = k; j < CDLGPETFACTORY_MAX_MATERIALS; j++ )
		{
			m_pTxt_name[j]->Show(false);
			m_pImgItem[j]->Show(false);
			m_pTxt_no[j]->Show(false);
		}

		k = 0;
		for(j = 0; j < CDLGPETFACTORY_MAX_PRODUCTS; j++ )
		{
			if( pRecipe->products[j].id_to_make <= 0 || pRecipe->products[j].probability <= 0 )
				continue;

			m_pItem_Product[k]->Show(true);
			m_pTxt_Product[k]->Show(true);
			m_pTxt_Rate[k]->Show(true);
			CECIvtrItem *pItem = CECIvtrItem::CreateItem(pRecipe->products[j].id_to_make, 0, 1);
			if( pItem )
			{
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				m_pItem_Product[k]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
				const wchar_t* szDesc = pItem->GetDesc();
				AUICTranslate trans;
				if( szDesc )
					m_pItem_Product[k]->SetHint(trans.Translate(szDesc));
				else
					m_pItem_Product[k]->SetHint(pItem->GetName());
				m_pTxt_Product[k]->SetText(pItem->GetName());
//				a_sprintf(szText, _AL("%.1f%%"), pRecipe->products[j].probability * 100);
//				m_pTxt_Rate[k]->SetText(szText);
				delete pItem;
			}
			k++;
		}
		for(j = k; j < CDLGPETFACTORY_MAX_PRODUCTS; j++ )
		{
			m_pItem_Product[j]->Show(false);
			m_pTxt_Product[j]->Show(false);
			m_pTxt_Rate[j]->Show(false);
		}
		for(j = 0; j < 7; j++)
		{
			char szName[20];
			sprintf(szName, "Txt_ReqLive%d", j);
			a_sprintf(szText, _AL("%d"), pRecipe->required[j]);
			GetDlgItem(szName)->SetText(szText);
			if( pRecipe->required[j] > m_nLive[j] )
				GetDlgItem(szName)->SetColor(A3DCOLORRGB(255, 0, 0));
			else
				GetDlgItem(szName)->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	}
}

void CDlgPetFactory::CleanUpRecipe()
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
					for(j = 0; j < CDLGPETFACTORY_MAX_MATERIALS; j++ )
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

void CDlgPetFactory::ProduceOne()
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

void CDlgPetFactory::ProduceEnd()
{
	m_pPrgs_1->SetData(0);
	m_pPrgs_1->SetAutoProgress(0, 0, 0);
}
