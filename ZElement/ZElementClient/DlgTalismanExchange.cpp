// Filename	: DlgTalismanExchange.cpp
// Creator	: Fu Chonggang
// Date		: 2012.8.18

#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUILabel.h"
#include "DlgTalismanExchange.h"
#include "DlgShopExchange.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_GameSession.h"
#include "EC_GPDataType.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_Skill.h"
#include "elementdataman.h"
#include "ExpTypes.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgTalismanExchange, CDlgBase)

AUI_ON_COMMAND("max",			OnCommandMax)
AUI_ON_COMMAND("start",			OnCommandExchange)
AUI_ON_COMMAND("confirm",		OnCommandConfirm)

AUI_END_COMMAND_MAP()

CDlgTalismanExchange::CDlgTalismanExchange()
{
}
CDlgTalismanExchange::~CDlgTalismanExchange()
{
}

bool CDlgTalismanExchange::OnInitDialog()
{
	m_mapMediumInfo.clear();
	m_mapMediumInfo[	4704	]=MediumInfo(	6338	,	19418	,	1	);
	m_mapMediumInfo[	4705	]=MediumInfo(	6336	,	19418	,	1	);
	m_mapMediumInfo[	4706	]=MediumInfo(	6339	,	19418	,	1	);
	m_mapMediumInfo[	4707	]=MediumInfo(	6337	,	19418	,	1	);
	m_mapMediumInfo[	17583	]=MediumInfo(	17855	,	19418	,	1	);
	m_mapMediumInfo[	45525	]=MediumInfo(	49372	,	19418	,	1	);
	m_mapMediumInfo[	30068	]=MediumInfo(	31505	,	19418	,	1	);
	m_mapMediumInfo[	30072	]=MediumInfo(	31509	,	19418	,	1	);
	m_mapMediumInfo[	38883	]=MediumInfo(	38891	,	19418	,	1	);
	m_mapMediumInfo[	38887	]=MediumInfo(	38895	,	19418	,	1	);
	m_mapMediumInfo[	45529	]=MediumInfo(	49376	,	19418	,	1	);
	m_mapMediumInfo[	53383	]=MediumInfo(	54047	,	19418	,	1	);
	m_mapMediumInfo[	4708	]=MediumInfo(	6342	,	19418	,	1	);
	m_mapMediumInfo[	4709	]=MediumInfo(	6340	,	19418	,	1	);
	m_mapMediumInfo[	4710	]=MediumInfo(	6343	,	19418	,	1	);
	m_mapMediumInfo[	4711	]=MediumInfo(	6341	,	19418	,	1	);
	m_mapMediumInfo[	17584	]=MediumInfo(	17856	,	19418	,	1	);
	m_mapMediumInfo[	45526	]=MediumInfo(	49373	,	19418	,	1	);
	m_mapMediumInfo[	30069	]=MediumInfo(	31506	,	19418	,	1	);
	m_mapMediumInfo[	30073	]=MediumInfo(	31510	,	19418	,	1	);
	m_mapMediumInfo[	38884	]=MediumInfo(	38892	,	19418	,	1	);
	m_mapMediumInfo[	38888	]=MediumInfo(	38896	,	19418	,	1	);
	m_mapMediumInfo[	45530	]=MediumInfo(	49377	,	19418	,	1	);
	m_mapMediumInfo[	53384	]=MediumInfo(	54048	,	19418	,	1	);
	m_mapMediumInfo[	10866	]=MediumInfo(	10914	,	19418	,	2	);
	m_mapMediumInfo[	10871	]=MediumInfo(	10919	,	19418	,	2	);
	m_mapMediumInfo[	10874	]=MediumInfo(	10922	,	19418	,	2	);
	m_mapMediumInfo[	10870	]=MediumInfo(	10918	,	19418	,	2	);
	m_mapMediumInfo[	17585	]=MediumInfo(	17858	,	19418	,	2	);
	m_mapMediumInfo[	45527	]=MediumInfo(	49374	,	19418	,	2	);
	m_mapMediumInfo[	30070	]=MediumInfo(	31507	,	19418	,	2	);
	m_mapMediumInfo[	30074	]=MediumInfo(	31511	,	19418	,	2	);
	m_mapMediumInfo[	38885	]=MediumInfo(	38893	,	19418	,	2	);
	m_mapMediumInfo[	38889	]=MediumInfo(	38897	,	19418	,	2	);
	m_mapMediumInfo[	45531	]=MediumInfo(	49378	,	19418	,	2	);
	m_mapMediumInfo[	53385	]=MediumInfo(	54049	,	19418	,	2	);
	m_mapMediumInfo[	13773	]=MediumInfo(	13794	,	13897	,	25	);
	m_mapMediumInfo[	13774	]=MediumInfo(	13798	,	13897	,	25	);
	m_mapMediumInfo[	13772	]=MediumInfo(	13796	,	13897	,	25	);
	m_mapMediumInfo[	13771	]=MediumInfo(	13795	,	13897	,	25	);
	m_mapMediumInfo[	17586	]=MediumInfo(	17859	,	13897	,	25	);
	m_mapMediumInfo[	45528	]=MediumInfo(	49375	,	13897	,	25	);
	m_mapMediumInfo[	30071	]=MediumInfo(	31508	,	13897	,	25	);
	m_mapMediumInfo[	30075	]=MediumInfo(	31512	,	13897	,	25	);
	m_mapMediumInfo[	38886	]=MediumInfo(	38894	,	13897	,	25	);
	m_mapMediumInfo[	38890	]=MediumInfo(	38898	,	13897	,	25	);
	m_mapMediumInfo[	45532	]=MediumInfo(	49379	,	13897	,	25	);
	m_mapMediumInfo[	53386	]=MediumInfo(	54050	,	13897	,	25	);
	m_mapMediumInfo[	4713	]=MediumInfo(	6471	,	19418	,	1	);
	m_mapMediumInfo[	4712	]=MediumInfo(	6344	,	19418	,	1	);
	m_mapMediumInfo[	4714	]=MediumInfo(	6472	,	19418	,	1	);
	m_mapMediumInfo[	4715	]=MediumInfo(	6470	,	19418	,	1	);
	m_mapMediumInfo[	13316	]=MediumInfo(	13359	,	19418	,	2	);
	m_mapMediumInfo[	22121	]=MediumInfo(	22128	,	19418	,	25	);
	m_mapMediumInfo[	10873	]=MediumInfo(	10921	,	19418	,	2	);
	m_mapMediumInfo[	10869	]=MediumInfo(	10917	,	19418	,	2	);
	m_mapMediumInfo[	10867	]=MediumInfo(	10915	,	19418	,	10	);
	m_mapMediumInfo[	15868	]=MediumInfo(	15867	,	19418	,	10	);
	m_mapMediumInfo[	17829	]=MediumInfo(	17833	,	19418	,	10	);
	m_mapMediumInfo[	23353	]=MediumInfo(	23356	,	19418	,	30	);
	m_mapMediumInfo[	10868	]=MediumInfo(	10916	,	19418	,	10	);
	m_mapMediumInfo[	10872	]=MediumInfo(	10920	,	19418	,	10	);
	m_mapMediumInfo[	20090	]=MediumInfo(	20094	,	19418	,	10	);
	m_mapMediumInfo[	20491	]=MediumInfo(	20501	,	19418	,	10	);
	m_mapMediumInfo[	25732	]=MediumInfo(	25739	,	19418	,	10	);
	m_mapMediumInfo[	10875	]=MediumInfo(	10923	,	19418	,	50	);
	m_mapMediumInfo[	10876	]=MediumInfo(	10924	,	19418	,	50	);
	m_mapMediumInfo[	10877	]=MediumInfo(	10925	,	19418	,	250	);
	m_mapMediumInfo[	43475	]=MediumInfo(	43480	,	19418	,	250	);
	m_mapMediumInfo[	58798	]=MediumInfo(	58842	,	19418	,	1	);
	m_mapMediumInfo[	58799	]=MediumInfo(	58843	,	19418	,	1	);
	m_mapMediumInfo[	58800	]=MediumInfo(	58844	,	19418	,	2	);
	m_mapMediumInfo[	58801	]=MediumInfo(	58845	,	19418	,	25	);
	m_mapMediumInfo[	58802	]=MediumInfo(	58846	,	19418	,	1	);
	m_mapMediumInfo[	58803	]=MediumInfo(	58847	,	19418	,	1	);
	m_mapMediumInfo[	58804	]=MediumInfo(	58848	,	19418	,	2	);
	m_mapMediumInfo[	58805	]=MediumInfo(	58849	,	19418	,	25	);



	m_iItemRequire = 0;
	m_iItemExchangeNum = 0;
	return true;
}

void CDlgTalismanExchange::OnShowDialog()
{
	if (!GetGameUIMan()->GetNpcEssence())
		return;

	int iTalismanId = GetData("talisman_id");
	if (m_mapMediumInfo.find(iTalismanId)==m_mapMediumInfo.end())
	{
		GetGameUIMan()->MessageBox(15034);
		Show(false);
		return;
	}

	int iItemId = m_mapMediumInfo[iTalismanId].iMediumId;
	m_iPropId = m_mapMediumInfo[iTalismanId].iPropId;

	CDlgShopExchange::SetShopIndex(0);
	DATA_TYPE DataType;
	ITEM_TRADE_CONFIG *pData = (ITEM_TRADE_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(GetGameUIMan()->GetNpcEssence()->id_item_trade_service[0], ID_SPACE_ESSENCE, DataType);
	if (!pData || DataType != DT_ITEM_TRADE_CONFIG)
		return;

	PAUIIMAGEPICTURE pImgItem	= (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
	PAUIIMAGEPICTURE pImgStone	= (PAUIIMAGEPICTURE)GetDlgItem("Item_b");
	PAUILABEL pLabName;
	int i, j;
	ACString strText;
	for (i=0; i<4; ++i)
	{
		for (j=0; j<48; ++j)
		{
			if( (int)pData->pages[i].goods[j].id_goods == iItemId )
			{
				CECIvtrItem *pItem;
				m_iItemRequire = pData->pages[i].goods[j].item_required[0].count;
				pItem = CECIvtrItem::CreateItem(pData->pages[i].goods[j].item_required[0].id, 0, 1);
				if (pItem)
				{
					GetGameUIMan()->SetIvtrInfo(pImgItem, pItem);
					pLabName = (PAUILABEL)GetDlgItem("Lab_Itema");
					pLabName->SetText(pItem->GetName());
					delete pItem;
				}
				strText.Format(_AL("%d"), m_iItemRequire);
				pImgItem->SetText(strText);
				strText.Format(_AL("%d"), 1);
				pImgStone->SetText(strText);

				m_iItemExchangeNum = 1;
				UpdateDisplay();
				m_iShopSet = i;
				m_iSetIndex = j;

				pItem = CECIvtrItem::CreateItem(iItemId, 0, 1);
				if (pItem)
				{
					GetGameUIMan()->SetIvtrInfo(pImgStone, iItemId);
					pLabName = (PAUILABEL)GetDlgItem("Lab_Itemb");
					pLabName->SetText(pItem->GetName());
					delete pItem;
				}
				PAUILABEL pLabPercent = (PAUILABEL)GetDlgItem("Txt_Percent");
				strText.Format(_AL("%d:1"), pData->pages[i].goods[j].item_required[0].count);
				pLabPercent->SetText(strText);
				break;
			}
		}
	}
}

void CDlgTalismanExchange::OnTick()
{
/*	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("Edit_Num1");
	int iNum = a_atoi(pEdt->GetText());
	pEdt = (PAUIEDITBOX)GetDlgItem("Edit_Num2");
	ACString strText;
	strText.Format(_AL("%d"), iNum*5);
	pEdt->SetText(strText);
*/}

void CDlgTalismanExchange::OnCommandMax(const char * szCommand)
{
	m_iItemExchangeNum = GetHostPlayer()->GetPack()->GetItemTotalNum(m_iPropId)/m_iItemRequire;
	UpdateDisplay();
}

void CDlgTalismanExchange::OnCommandExchange(const char * szCommand)
{
	if (GetHostPlayer()->GetPack()->GetItemTotalNum(m_iPropId) >= m_iItemExchangeNum*m_iItemRequire )
	{
		int iTalismanId = GetData("talisman_id");
		C2S::item_trade it;
		it.item_type = m_mapMediumInfo[iTalismanId].iMediumId;
		it.index = m_iShopSet * 48 + m_iSetIndex;
		it.count = m_iItemExchangeNum;
		GetGameSession()->c2s_CmdNPCSevItemTrade(0, &it, 1);
	}
	else
		GetGameUIMan()->MessageBox(15031);
}

void CDlgTalismanExchange::OnCommandConfirm(const char * szCommand)
{
/*	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("Edit_Num1");
	int iNum = a_atoi(pEdt->GetText());

	if ( GetHostPlayer()->GetPack()->GetItemTotalNum(ms_iIdJuBaoPen)<iNum )
	{
		GetGameUIMan()->MessageBox(15031);
		return;
	}

	CDlgShopExchange::SetShopIndex(0);
	DATA_TYPE DataType;
	ITEM_TRADE_CONFIG *pData = (ITEM_TRADE_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(GetGameUIMan()->GetNpcEssence()->id_item_trade_service[0], ID_SPACE_ESSENCE, DataType);
	if (!pData || DataType != DT_ITEM_TRADE_CONFIG)
		return;
	
	int i, j;
	ACString strText;
	for (i=0; i<4; ++i)
	{
		for (j=0; j<48; ++j)
		{
			if( pData->pages[i].goods[j].id_goods == ms_iIdLinBaoMiCen )
			{
				C2S::item_trade it;
				it.item_type = ms_iIdLinBaoMiCen;
				it.index = i * 48 + j;
				it.count = iNum;
				GetGameSession()->c2s_CmdNPCSevItemTrade(0, &it, 1);
			}
		}
	}
*/
}

void CDlgTalismanExchange::UpdateDisplay()
{
	PAUIIMAGEPICTURE pImgItem	= (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
	PAUIIMAGEPICTURE pImgStone	= (PAUIIMAGEPICTURE)GetDlgItem("Item_b");
	ACString strText;
	strText.Format(_AL("%d"), m_iItemRequire*m_iItemExchangeNum);
	pImgItem->SetText(strText);
	strText.Format(_AL("%d"), m_iItemExchangeNum);
	pImgStone->SetText(strText);
}


AUI_BEGIN_COMMAND_MAP(CDlgTalismanCombineSkillSelect, CDlgBase)

AUI_ON_COMMAND("Btn_Save",			OnCommandAccept)
AUI_ON_COMMAND("Btn_Exit",			OnCommandRefuse)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()

void CDlgTalismanCombineSkillSelect::OnCommandAccept(const char * szCommand)
{
	GetGameSession()->c2s_CmdTalismanSkillRefineResult(GetData("item_index"), (int)GetDataPtr("item_id"), 1);
	OnCommandCancel("");
}

void CDlgTalismanCombineSkillSelect::OnCommandRefuse(const char * szCommand)
{
	GetGameSession()->c2s_CmdTalismanSkillRefineResult(GetData("item_index"), (int)GetDataPtr("item_id"), 0);
	OnCommandCancel("");
}

void CDlgTalismanCombineSkillSelect::OnCommandCancel(const char * szCommand)
{
	Show(false);
	GetHostPlayer()->GetPack()->GetItem(GetData("item_index"))->Freeze(false);
	SetData(-1, "item_index");
}

int CDlgTalismanCombineSkillSelect::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="refine_finish")
	{
		int index = wParam;
		int id = lParam;
		GetHostPlayer()->GetPack()->GetItem(index)->Freeze(true);
		SetData(index, "item_index");
		SetDataPtr((void*)id, "item_id");
		m_iRefineFinishId = id;
		Show(true);
	}
	else if (strMsg=="start")
	{
		int index = wParam;
		int id = lParam;
		GetHostPlayer()->GetPack()->GetItem(index)->Freeze(true);
		SetData(index, "item_index");
		SetDataPtr((void*)id, "item_id");
		CECIvtrTalismanMain *pItem = (CECIvtrTalismanMain*)GetHostPlayer()->GetPack()->GetItem(GetData("item_index"));
		if (pItem->GetEssence().addons2012.size()>0 && 
			pItem->GetEssence().addons2012[CECIvtrTalismanMain::TYPE_REFINE_TMP_1]>0)
		{
			Show(true);
			return 0;
		}
		else
			return 1;
	}

	return 0;
}

void CDlgTalismanCombineSkillSelect::OnShowDialog()
{
	UpdateDisplay();
}

void CDlgTalismanCombineSkillSelect::OnTick()
{
	if (m_iRefineFinishId>0)
	{
		UpdateDisplay();
	}

}

void CDlgTalismanCombineSkillSelect::UpdateDisplay()
{
	CECIvtrTalismanMain *pItem = (CECIvtrTalismanMain*)GetHostPlayer()->GetPack()->GetItem(GetData("item_index"));
	if (pItem->NeedUpdate())
	{
		return;
	}
	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("List_CombineSkil");
	pLst->SetAutoWrap(true);
	pLst->SetAutoIndent(false);
	int i;
	ACString strText;
	int iId1, iId2, iLevel;
	CECSkill *pSkill1, *pSkill2;
	pLst->ResetContent();
	for (i=0; i<8; ++i)
	{
		iId1 = pItem->GetEssence().addons2012[CECIvtrTalismanMain::TYPE_REFINE_1+i];
		iId2 = pItem->GetEssence().addons2012[CECIvtrTalismanMain::TYPE_REFINE_TMP_1+i];
		iLevel =  pItem->GetFixSkillLevel();
		pSkill1 = iId1>0 ? new CECSkill(iId1, iLevel) : 0;
		pSkill2 = iId2>0 ? new CECSkill(iId2, iLevel) : 0;
		if (!pSkill1 && !pSkill2)
			break;
		strText.Format(_AL("%s\t%s"), pSkill1 ? pSkill1->GetDesc(iLevel, false):_AL(" "), pSkill2 ? pSkill2->GetDesc(iLevel, false):_AL(" "));
		pLst->AddString(strText);
		if (pSkill1) delete pSkill1;
		if (pSkill2) delete pSkill2;
	}
	if (i>0)
	{
		m_iRefineFinishId = 0;
	}
}
