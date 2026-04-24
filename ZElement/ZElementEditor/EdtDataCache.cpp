#include "EdtDataCache.h"
#include "Global.h"
#include "elementdataman.h"

CEdtDataCache::CEdtDataCache(void) : m_bInited(false)
{
}

CEdtDataCache::~CEdtDataCache(void)
{
}

void CEdtDataCache::InitData()
{
	if (m_bInited)
	{
		return;
	}
	m_vecNPCs.clear();
	m_vecMonsters.clear();
	m_vecInterObjects.clear();
	m_vecMines.clear();

	DATA_TYPE DataType = DT_INVALID;
	DWORD dwid = g_dataMan.get_first_data_id(ID_SPACE_ESSENCE, DataType);

	while (dwid)
	{
		const void* pData = g_dataMan.get_data_ptr(dwid, ID_SPACE_ESSENCE, DataType);

		switch (DataType)
		{
		case DT_MONSTER_ESSENCE:
			m_vecMonsters.push_back(dwid);
			break;
		case DT_NPC_ESSENCE:
			m_vecNPCs.push_back(dwid);
			break;
		case DT_INTERACTION_OBJECT_ESSENCE:
			m_vecInterObjects.push_back(dwid);
			break;
		case DT_MINE_ESSENCE:
			m_vecMines.push_back(dwid);
			break;
		case DT_REGION_BLOCK_ESSENCE:
			m_vecRegionBlocks.push_back(dwid);
			break;
		}

		//	Get next item
		dwid = g_dataMan.get_next_data_id(ID_SPACE_ESSENCE, DataType);
	}

	m_bInited = true;
}
