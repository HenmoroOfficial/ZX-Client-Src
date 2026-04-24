// 这个类没啥用，有空的时候删掉
#pragma once

#include "vector.h"

typedef abase::vector<int> VecIds;

class CEdtDataCache
{
public:
	CEdtDataCache(void);
	~CEdtDataCache(void);

	VecIds& GetNPCs() { return m_vecNPCs; }
	VecIds& GetMonsters() { return m_vecMonsters; }
	VecIds& GetInterObjects() { return m_vecInterObjects; }
	VecIds& GetMines() { return m_vecMines; }
	VecIds& GetRegionBlocks() { return m_vecRegionBlocks; }

	void InitData();
protected:

private:
	VecIds	m_vecNPCs;
	VecIds	m_vecMonsters;
	VecIds	m_vecInterObjects;
	VecIds	m_vecMines;
	VecIds	m_vecRegionBlocks;

	bool m_bInited;
};
