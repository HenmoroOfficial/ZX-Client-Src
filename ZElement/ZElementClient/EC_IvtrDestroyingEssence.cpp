#include "EC_Global.h"
#include "EC_IvtrDestroyingEssence.h"
#ifndef _INVENTORY_DESC_MAN //Added by Ford.W 2010-07-22.
#include "EC_Game.h"
#include "EC_GameRun.h"
#else
#include "EC_Game_HintTool.h"
#include "EC_GameRun_HintTool.h"
#endif
#include "EC_RTDebug.h"
#include "EC_FixedMsg.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW

CECIvtrDestroyingEssence::CECIvtrDestroyingEssence(int tid, int expire_date) : CECIvtrItem(tid, expire_date)
{
	m_iCID	= ICID_DESTROYINGESSENCE;

	memset(&m_Essence, 0, sizeof (m_Essence));

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (DESTROYING_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iEquipMask	= 0;
	m_bEmbeddable	= true;
}

CECIvtrDestroyingEssence::CECIvtrDestroyingEssence(const CECIvtrDestroyingEssence& s) : CECIvtrItem(s)
{
	m_Essence		= s.m_Essence;
	m_pDBEssence	= s.m_pDBEssence;
}

CECIvtrDestroyingEssence::~CECIvtrDestroyingEssence()
{
}

//	Set item detail information
bool CECIvtrDestroyingEssence::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrItem::SetItemInfo(pInfoData, iDataLen);

	try
	{
		CECDataReader dr(pInfoData, iDataLen);

		m_Essence = *(IVTR_ESSENCE_DESTROYING*)dr.Read_Data(sizeof (IVTR_ESSENCE_DESTROYING));
	}
	catch (CECException& e)
	{
		ASSERT(0);
		a_LogOutput(1, "CECIvtrDestroyingEssence::SetItemInfo, data read error (%d)", e.GetType());
		return false;
	}

	return true;
}

//	Get item icon file name
const char* CECIvtrDestroyingEssence::GetIconFile()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_icon);
}

//	Get item name
const wchar_t* CECIvtrDestroyingEssence::GetName()
{
	return m_pDBEssence->name;
}

//	Get item description text
const wchar_t* CECIvtrDestroyingEssence::GetNormalDesc(bool bRepair)
{
	if (m_bNeedUpdate)
		return NULL;

	m_strDesc = _AL("");

	//	Try to build item description
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	int white = ITEMDESC_COL_WHITE;

	CECIvtrItem* pItem = CECIvtrItem::CreateItem(m_Essence.tid, 0, 1);

	if (pItem)
	{
		const SPECIAL_ID_CONFIG& cfg = g_pGame->GetGameRun()->GetSpecialIDConfig();

		if (m_pDBEssence->id == cfg.id_destroying_matter)
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_DESTROYINGNAME), pItem->GetName());
		else if (m_pDBEssence->id == cfg.unlocking_item_id)
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_UNLOCKING_ITEM), pItem->GetName());
		else if (m_pDBEssence->id == cfg.id_damaged_item)
			AddDescText(white, true, pDescTab->GetWideString(ITEMDESC_DAMAGED_ITEM), pItem->GetName());

		delete pItem;
	}

	AddStateDesc(pDescTab);
	AddExpireTimeDesc();

	//	Extend description
	AddExtDescText();

	return m_strDesc;
}

//	Get drop model for shown
const char * CECIvtrDestroyingEssence::GetDropModel()
{
	return g_pGame->GetDataPath(m_pDBEssence->file_matter);
}

ACString CECIvtrDestroyingEssence::_Serialize() const
{
	return ItemMakeString(&m_Essence);
}

bool CECIvtrDestroyingEssence::_Unserialize(const ACHAR* szText)
{
	ItemFromString(szText, &m_Essence);
	m_bNeedUpdate = false;
	return true;
}
