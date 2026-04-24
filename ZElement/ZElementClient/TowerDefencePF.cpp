/***************************************************
	file   : TowerDefencePF.cpp
	created: 2008/10/30
	author : liudong
	purpose: implementation of  TowerDefencePF.h。

 ****************************************************/
#include "TowerDefencePF.h"
#include <assert.h>
#include <memory.h>

//////////////////////////////////////////////////////////////////////////
// local macro
#define TD_DIST_INVALID -1
#define TD_DIST_BASE 0
#define TD_DIST_OFFSET_STRAIGHT 10
#define TD_DIST_OFFSET_DIAGONAL 14
#define TD_OPEN_MAP_INVALID -1
#define TD_NEIGHBOR_COUNT 8

//////////////////////////////////////////////////////////////////////////
// local variable
int NEIGHBOR_OFFSET[3*TD_NEIGHBOR_COUNT] =
{
    -1, 0, TD_DIST_OFFSET_STRAIGHT,
    -1, 1, TD_DIST_OFFSET_DIAGONAL,
     0, 1, TD_DIST_OFFSET_STRAIGHT,
     1, 1, TD_DIST_OFFSET_DIAGONAL,
     1, 0, TD_DIST_OFFSET_STRAIGHT,
     1,-1, TD_DIST_OFFSET_DIAGONAL,
     0,-1, TD_DIST_OFFSET_STRAIGHT,
    -1,-1, TD_DIST_OFFSET_DIAGONAL,
};

CTowerDefencePF CTowerDefencePF::m_instance;

CTowerDefencePF::CTowerDefencePF()
{
	m_pMap      = NULL;
	m_pOpenMap  = NULL;
	m_pCloseMap = NULL;
	m_pMapTemp  = NULL;
	m_pMapEntryExit = NULL;
    m_nX        = -1;
	m_nY        = -1;
	
}

CTowerDefencePF::~CTowerDefencePF()
{
	_Release();
}

void CTowerDefencePF::_Release()
{
	m_nX   = -1;
	m_nY   = -1;
	if (m_pMap) 
	{
		delete[] m_pMap;
		m_pMap = NULL;
	}
	if (m_pOpenMap) 
	{
		delete[] m_pOpenMap;
		m_pOpenMap = NULL;
	}
	if (m_pCloseMap) 
	{
		delete[] m_pCloseMap;
		m_pCloseMap = NULL;
	}
	if (m_pMapTemp) 
	{
		delete[] m_pMapTemp;
		m_pMapTemp = NULL;
	}
	if (m_pMapEntryExit) 
	{
		delete[] m_pMapEntryExit;
		m_pMapEntryExit = NULL;
	}
	int i;
	for (i=0; i<(int)m_vecEscapeMap.size(); ++i) 
	{
		int* pEscapeMap = m_vecEscapeMap[i];
		if (pEscapeMap) 
		{
			delete[] pEscapeMap;
		}
		int* pEscapeMapTemp = m_vecEscapeMapTemp[i];
		if (pEscapeMapTemp) 
		{
			delete[] pEscapeMapTemp;
		}
	}
	m_vecEscapeMap.clear();
	m_vecEscapeInfo.clear();
	m_vecEscapeMapTemp.clear();
}

bool CTowerDefencePF::Init(const int nX, const int nY, const abase::vector<EscapeInfo>& vecEscapeInfo)
{
	_Release();
	if (nX<=0 || 
		nY<=0 || 
		vecEscapeInfo.size()==0) 
	{
		assert(0);
		return false;
	}
	m_nX = nX;
	m_nY = nY;
	int i;
	for (i=0; i<(int)vecEscapeInfo.size(); ++i) 
	{
		const EscapeInfo& escapeInfo = vecEscapeInfo[i];
		if (escapeInfo.entry.size()==0 || escapeInfo.exit.size()==0) 
		{
			assert(0);
			m_nX   = -1;
			m_nY   = -1;
			return false;
		}
		int j;
		for (j=0; j<(int)escapeInfo.entry.size(); ++j) 
		{
			const APointI& pos = escapeInfo.entry[j];
			if (!_IsPosInMap(pos))
			{
				assert(0);
				m_nX   = -1;
			    m_nY   = -1;
				return false;
			}
		}
		for (j=0; j<(int)escapeInfo.exit.size(); ++j) 
		{
			const APointI& pos = escapeInfo.exit[j];
			if (!_IsPosInMap(pos))
			{
				assert(0);
				m_nX   = -1;
			    m_nY   = -1;
				return false;
			}
		}
	}

	m_vecEscapeInfo = vecEscapeInfo;

	const int NUM = m_nX*m_nY;
	for (i=0; i<(int)m_vecEscapeInfo.size(); ++i) 
	{
		int* pEscapeMap = new int[NUM];
		m_vecEscapeMap.push_back(pEscapeMap);
		int* pEscapeMapTemp = new int[NUM];
		m_vecEscapeMapTemp.push_back(pEscapeMapTemp);
	}
	m_pOpenMap  = new int[NUM];
	m_pCloseMap = new bool[NUM];
    m_pMap      = new CTowerDefencePF::PosType[NUM];
	m_pMapTemp  = new CTowerDefencePF::PosType[NUM];
	m_pMapEntryExit = new bool[NUM];
	for (i=0; i<NUM; i++) 
	{
		m_pMapEntryExit[i] = false;
	}
	for (i=0; i<(int)m_vecEscapeInfo.size(); ++i) 
	{
		EscapeInfo& info = m_vecEscapeInfo[i];
		int iPos;
		for(iPos=0; iPos<(int)info.entry.size(); iPos++)
		{
			int index = _GetPosIndex(info.entry[iPos]);
			m_pMapEntryExit[index] = true ;
		}
		for(iPos=0; iPos<(int)info.exit.size(); iPos++)
		{
			int index = _GetPosIndex(info.exit[iPos]);
			m_pMapEntryExit[index] = true;
		}
	}

    return ClearMap();
}

CTowerDefencePF::PosType CTowerDefencePF::GetPosType(const APointI& pos)
{
	if (!_IsPosInMap(pos)) 
	{
		assert(0);
		return CTowerDefencePF::PosBlock;
	}
    return m_pMap[_GetPosIndex(pos)] ;
}

bool CTowerDefencePF::GetNextEscapePos(APointI& nextPos, const APointI& pos, const int iEscapeMap)
{
	nextPos = pos;
	if (!_IsPosInMap(pos)) 
	{
		assert(0);
		return false;
	}
	const int POS_INDEX = _GetPosIndex(pos);
	if (CTowerDefencePF::PosRoad!=m_pMap[POS_INDEX]) 
	{
		assert(0);
		return false;
	}
	if (iEscapeMap<0 || iEscapeMap>=(int)m_vecEscapeMap.size())
	{
		assert(0);
		return false;
	}

    int* pEscapeMap = m_vecEscapeMap[iEscapeMap];
	const int DIST_CUR = pEscapeMap[POS_INDEX];
	if (DIST_CUR == TD_DIST_INVALID) 
	{
		assert(0);
		return false;
	}

	int distMin = DIST_CUR;
	int i;
	for (i=0; i<TD_NEIGHBOR_COUNT; ++i) 
	{
		if (_IsLineToNeighbor(pos,i)) 
		{
			APointI neighbor = _GetNeighbor(pos,i);
			int distOffset = _GetDistOffset(i);
			int indexNei = _GetPosIndex(neighbor);
			int distNei  = pEscapeMap[indexNei];
			assert (distNei!=TD_DIST_INVALID);
			if (distNei+distOffset==DIST_CUR && distNei<distMin) 
			{
			    distMin = distNei;
				nextPos = neighbor;
			}
		}
	}

    return (DIST_CUR!=distMin);
}

#ifdef _TD_EDITOR
int  CTowerDefencePF::GetDist(const APointI& pos, const int iEscapeMap)
{
	if (!_IsPosInMap(pos)) 
	{
		assert(0);
		return TD_DIST_INVALID;
	}
	if (iEscapeMap<0 || iEscapeMap>=(int)m_vecEscapeMap.size())
	{
		assert(0);
		return TD_DIST_INVALID;
	}

	int* pEscapeMap = m_vecEscapeMap[iEscapeMap];
	int indexMap = _GetPosIndex(pos);
    return pEscapeMap[indexMap];
}
#endif

bool CTowerDefencePF::_CheckNPCParams(const abase::vector<TDPosEx>* pVecNPC)
{
	if (!pVecNPC)
	{
		return true;
	}
	int i;
	for (i=0; i<(int)pVecNPC->size(); ++i)
	{
		const TDPosEx& npc = (*pVecNPC)[i];
		if (!_IsPosInMap(npc.pos)) 
		{
			return false;
		}
		CTowerDefencePF::PosType posTypeNPC = GetPosType(npc.pos);
		if (CTowerDefencePF::PosRoad!=posTypeNPC) 
		{
			return false;
		}
		if (npc.iEscape<0 || npc.iEscape>= (int)m_vecEscapeInfo.size())
		{
			return false;
		}
	}
	return true;
}

bool CTowerDefencePF::_CheckNPCDist(const abase::vector<TDPosEx>* pVecNPC)
{
	if (!pVecNPC)
	{
		return true;
	}
	int i;
	for (i=0; i<(int)pVecNPC->size(); ++i)
	{
		const TDPosEx& npc = (*pVecNPC)[i];
		int* pEscapeMap = m_vecEscapeMap[npc.iEscape];
		assert(pEscapeMap);
		int index = _GetPosIndex(npc.pos);
		if (TD_DIST_INVALID==pEscapeMap[index])
		{
			return false;
		}
	}
	return true;
}

bool CTowerDefencePF::SetPosType(const APointI& pos, const CTowerDefencePF::PosType posType, const abase::vector<TDPosEx>* pVecNPC)
{ 
	if (posType<CTowerDefencePF::PosRoad ||posType>=CTowerDefencePF::PosNum)
	{
		assert(0);
		return false;
	}
	if (!_IsPosInMap(pos) || _IsPosEntryExit(pos)) 
	{
		assert(0);
		return false;
	}
	CTowerDefencePF::PosType posTypeOri = GetPosType(pos);
	if (posTypeOri==posType) 
	{
		assert(0);
		return false;
	}
	if (CTowerDefencePF::PosRoad!=posType && !_CheckNPCParams(pVecNPC))
	{
		assert(0);
		return false;
	}

	_StoreCurMap();
	m_pMap[_GetPosIndex(pos)] = posType;
	bool bResult = _CreateAllEscapeMap();
	if (bResult && CTowerDefencePF::PosRoad!=posType && !_CheckNPCDist(pVecNPC))
	{
         bResult = false;
	}
	if (!bResult) 
	{
		_RestoreMap();
	}

    return bResult;
}

bool CTowerDefencePF::SetPosType(const abase::vector<APointI>& vecPos, const CTowerDefencePF::PosType posType, const abase::vector<TDPosEx>* pVecNPC)
{
	if (posType<CTowerDefencePF::PosRoad ||posType>=CTowerDefencePF::PosNum)
	{
		assert(0);
		return false;
	}
	if (vecPos.size()==0)
	{
		assert(0);
		return false;
	}
	int i;
	for (i=0; i<(int)vecPos.size(); ++i)
	{
		const APointI& pos = vecPos[i];
		if (!_IsPosInMap(pos) || _IsPosEntryExit(pos)) 
		{
			assert(0);
			return false;
		}
	}
	if (CTowerDefencePF::PosRoad!=posType && !_CheckNPCParams(pVecNPC))
	{
		assert(0);
		return false;
	}

	_StoreCurMap();
	for (i=0; i<(int)vecPos.size(); ++i)
	{
		const APointI& pos = vecPos[i];
		m_pMap[_GetPosIndex(pos)] = posType;
	}
	bool bResult = _CreateAllEscapeMap();
	if (bResult && CTowerDefencePF::PosRoad!=posType && !_CheckNPCDist(pVecNPC))
	{
         bResult = false;
	}
	if (!bResult) 
	{
		_RestoreMap();
	}
	
	return bResult;
}

bool CTowerDefencePF::CanBlockPos(const APointI& pos, const abase::vector<TDPosEx>* pVecNPC)
{
	if (!_IsPosInMap(pos) || _IsPosEntryExit(pos)) 
	{
		assert(0);
		return false;
	}
   	CTowerDefencePF::PosType posTypeOri = GetPosType(pos);
	if (CTowerDefencePF::PosRoad!=posTypeOri) 
	{
		assert(0);
		return false;
	}
	if (!_CheckNPCParams(pVecNPC))
	{
		assert(0);
		return false;
	}

	_StoreCurMap();
	m_pMap[_GetPosIndex(pos)] = CTowerDefencePF::PosBlock;
	bool bResult = _CreateAllEscapeMap();
	if (bResult && !_CheckNPCDist(pVecNPC))
	{
         bResult = false;
	}
	_RestoreMap();
    return bResult;
}

bool CTowerDefencePF::CanBlockPos(const abase::vector<APointI>& vecPos, const abase::vector<TDPosEx>* pVecNPC)
{
	if (vecPos.size()==0)
	{
		assert(0);
		return false;
	}
	int i;
	for (i=0; i<(int)vecPos.size(); ++i)
	{
		const APointI& pos = vecPos[i];
		if (!_IsPosInMap(pos) || _IsPosEntryExit(pos)) 
		{
			assert(0);
			return false;
		}
		CTowerDefencePF::PosType posTypeOri = GetPosType(pos);
		if (CTowerDefencePF::PosRoad != posTypeOri) 
		{
			assert(0);
			return false;
		}
	}
	if (!_CheckNPCParams(pVecNPC))
	{
		assert(0);
		return false;
	}

	_StoreCurMap();
	for (i=0; i<(int)vecPos.size(); ++i)
	{
		const APointI& pos = vecPos[i];
		m_pMap[_GetPosIndex(pos)] = CTowerDefencePF::PosBlock;
	}
	bool bResult = _CreateAllEscapeMap();
	if (bResult && !_CheckNPCDist(pVecNPC))
	{
         bResult = false;
	}
	_RestoreMap();
	
	return bResult;
}

void CTowerDefencePF::_StoreCurMap()
{
	const int NUM = m_nX*m_nY;
	int i;
	for (i=0; i<(int)m_vecEscapeMap.size(); ++i) 
	{
		int* pEscapeMap     = m_vecEscapeMap[i];
		int* pEscapeMapTemp = m_vecEscapeMapTemp[i];
		memcpy(pEscapeMapTemp,pEscapeMap,NUM*sizeof(CTowerDefencePF::PosType));
	}
	memcpy(m_pMapTemp,m_pMap,NUM*sizeof(int));

}
void CTowerDefencePF::_RestoreMap()
{
	const int NUM = m_nX*m_nY;
	int i;
	for (i=0; i<(int)m_vecEscapeMap.size(); ++i) 
	{
		int* pEscapeMap     = m_vecEscapeMap[i];
		int* pEscapeMapTemp = m_vecEscapeMapTemp[i];
		memcpy(pEscapeMap,pEscapeMapTemp,NUM*sizeof(CTowerDefencePF::PosType));
	}
	memcpy(m_pMap,m_pMapTemp,NUM*sizeof(int));
}

bool CTowerDefencePF::ClearMap()
{
	if (!m_pMap) 
	{
	    assert(0);
	    return false;
	}

	const int NUM = m_nX*m_nY;
	int i;
	for (i=0; i<NUM; ++i) 
	{
	    m_pMap[i] = CTowerDefencePF::PosRoad;
	}

	//block the edge.
	for (int x=0; x<m_nX; ++x) 
	{
		m_pMap[_GetPosIndex(APointI(x,0))] = CTowerDefencePF::PosBlock;
		m_pMap[_GetPosIndex(APointI(x,m_nY-1))] = CTowerDefencePF::PosBlock;
	}
	for (int y=0; y<m_nY; ++y) 
	{
		m_pMap[_GetPosIndex(APointI(0,y))] = CTowerDefencePF::PosBlock;
		m_pMap[_GetPosIndex(APointI(m_nX-1,y))] = CTowerDefencePF::PosBlock;
	}
    //unblock entry and exit positions.
	for (i=0; i<(int)m_vecEscapeInfo.size(); ++i) 
	{
		EscapeInfo& info = m_vecEscapeInfo[i];
		int iPos;
		for(iPos=0; iPos<(int)info.entry.size(); iPos++)
		{
			int index = _GetPosIndex(info.entry[iPos]);
			m_pMap[index] = CTowerDefencePF::PosRoad;
		}
		for(iPos=0; iPos<(int)info.exit.size(); iPos++)
		{
			int index = _GetPosIndex(info.exit[iPos]);
			m_pMap[index] = CTowerDefencePF::PosRoad;
		}
	}
   
   return _CreateAllEscapeMap();
}

bool CTowerDefencePF::_CreateAllEscapeMap()
{
	int i;
	for (i=0; i<(int)m_vecEscapeInfo.size(); ++i) 
	{
		if (!_CreateEscapeMap(i)) 
		{
			return false;
		}
	}
	return true;
}

bool CTowerDefencePF::_CreateEscapeMap(const int iEscapeMap)
{
	abase::vector<APointI>& entry = m_vecEscapeInfo[iEscapeMap].entry;
	abase::vector<APointI>& exit  = m_vecEscapeInfo[iEscapeMap].exit;
	int* pEscapeMap = m_vecEscapeMap[iEscapeMap];

	const int NUM = m_nX*m_nY;
	abase::vector<APointI> vecOpen,vecTemp;
	vecOpen.reserve(NUM);
	vecTemp.reserve(NUM);

	int i;
	for (i=0; i<NUM; ++i) 
	{
		pEscapeMap[i] = TD_DIST_INVALID;
	}
	for (i=0; i<NUM; ++i) 
	{
		m_pOpenMap[i] = TD_OPEN_MAP_INVALID;
	}
	for (i=0; i<NUM; ++i) 
	{
		m_pCloseMap[i] = false ;
	}

	for (i=0; i<(int)exit.size(); ++i) 
	{
		int index = _GetPosIndex(exit[i]);
		if (CTowerDefencePF::PosRoad==m_pMap[index])
		{
			pEscapeMap[index] = TD_DIST_BASE;
			vecOpen.push_back(exit[i]);
		}
	}

	while (vecOpen.size()>0) 
	{
		vecTemp.clear();
		int i;
		for (i=0; i<(int)vecOpen.size(); ++i) 
		{
			m_pCloseMap[_GetPosIndex(vecOpen[i])] = true;
		}
		for (i=0; i<(int)vecOpen.size(); ++i) 
		{
			_CheckHeuristicAroundPos(vecOpen[i],pEscapeMap,vecTemp);
		}
		vecOpen = vecTemp;//todo: liudong 手动赋值能快一点？
	}

	bool bReachTarget = false;
	for (i=0; i<(int)entry.size(); ++i) 
	{
	    int indexMap = _GetPosIndex(entry[i]);
		if (TD_DIST_INVALID != pEscapeMap[indexMap]) 
		{
			bReachTarget = true;
			break;
		}
	}

	return bReachTarget;
}

APointI CTowerDefencePF::_GetNeighbor(const APointI& pos,const int dir)
{
	APointI neighbor;
	neighbor.x   = pos.x + NEIGHBOR_OFFSET[3*dir];
	neighbor.y   = pos.y + NEIGHBOR_OFFSET[3*dir+1];
	return neighbor;
}

int CTowerDefencePF::_GetDistOffset(const int dir)
{
	return NEIGHBOR_OFFSET[3*dir+2];
}

void CTowerDefencePF::_CheckHeuristicAroundPos(const APointI& pos, int* pEscapeMap, abase::vector<APointI>& vecOpen)
{
	int distCur = pEscapeMap[_GetPosIndex(pos)];
	int i;
	for (i=0; i<TD_NEIGHBOR_COUNT; ++i) 
	{
		if (_IsLineToNeighbor(pos,i)) 
		{
			APointI neighbor = _GetNeighbor(pos,i);
			int distOffset = _GetDistOffset(i);
			int indexNei = _GetPosIndex(neighbor);
			if (!m_pCloseMap[indexNei]) 
			{
				int distTemp = distCur+distOffset;
				int distNei  = pEscapeMap[indexNei];
				if (TD_DIST_INVALID==distNei || distNei>distTemp) 
				{
					pEscapeMap[indexNei] = distTemp;
					int indexOpen = m_pOpenMap[indexNei];
					if (TD_OPEN_MAP_INVALID==indexOpen) 
					{
						m_pOpenMap[indexNei] = vecOpen.size();
						vecOpen.push_back(neighbor);
					}else
					{
						vecOpen[indexOpen] = neighbor;
					}
				}
			}
		}
	}
}

bool CTowerDefencePF::_IsLineToNeighbor(const APointI& pos, const int dir)
{
	APointI neighbor = _GetNeighbor(pos,dir);
	if (!_IsPosInMap(neighbor) || CTowerDefencePF::PosRoad!=m_pMap[_GetPosIndex(neighbor)]) 
	{
		return false;
	}
	if ((dir%2)==0) 
	{
        //straight neighbor
	    return true;
	}else
	{
		//diagonal neighbor
	    int dirPrev = (dir-1+TD_NEIGHBOR_COUNT)%TD_NEIGHBOR_COUNT;
		int dirNext = (dir+1)%TD_NEIGHBOR_COUNT;
	    APointI neiPrev = _GetNeighbor(pos,dirPrev);
		APointI neiNext = _GetNeighbor(pos,dirNext);
		if (CTowerDefencePF::PosRoad!=m_pMap[_GetPosIndex(neiPrev)] || CTowerDefencePF::PosRoad!=m_pMap[_GetPosIndex(neiNext)]) 
		{
			//cut corner
			return false;
		}	
		return true;
	}
}

bool CTowerDefencePF::_IsPosInMap(const APointI& pos)
{
	if (pos.x>=0 && pos.x<m_nX && pos.y>=0 && pos.y<m_nY) 
	{
		return true;
	}else
	{
		return false;
	}
}

int CTowerDefencePF::_GetPosIndex(const APointI& pos)
{
    return m_nX*pos.y+pos.x;
}

void CTowerDefencePF::_GetPosFromIndex(APointI& pos, const int index)
{
    pos.x = index%m_nX;
	pos.y = index/m_nX;
}

bool CTowerDefencePF::_IsPosEntryExit(const APointI& pos)
{
	return m_pMapEntryExit[_GetPosIndex(pos)];
}

//////////////////////////////////////////////////////////////////////////
//CAStarPF implement

//////////////////////////////////////////////////////////////////////////

CAStarPF CAStarPF::m_instance;

CAStarPF::CAStarPF()
{
	m_iRow = 0;
	m_iCol = 0;
	m_pMap = NULL;
}

CAStarPF::~CAStarPF()
{
	Release();
}


bool CAStarPF::Init(const int row, const int col, const abase::vector<APointI>& vecBlocks)
{
	Release();
	assert(row>=0);
	assert(col>=0);
	
	m_iRow = row; 
	m_iCol = col;
	int NUM = m_iRow*m_iCol;
	m_pMap = new int[NUM];
	int i;
	for (i = 0; i< NUM; ++i)
		m_pMap[i] = 0;
	for(i = 0; i<(int)vecBlocks.size(); ++i)
	{
		assert(IsPosInMap(vecBlocks[i].x, vecBlocks[i].y));
		m_pMap[GetPosIndex(vecBlocks[i])] = 1;
	}
	return true;
}

int CAStarPF::GetPosIndex(const APointI& pos)
{
	return m_iCol*pos.y + pos.x;
}

int CAStarPF::GetPosIndex(int x, int y)
{
	return m_iCol*y + x;
}

bool CAStarPF::SetBlockPos(const abase::vector<APointI>& vecBlocks)
{
	int index;
	for(int i = 0; i < (int)vecBlocks.size(); ++i)
	{
		index = GetPosIndex(vecBlocks[i]);
		if(m_pMap[index] == 1)
			assert(0);
		m_pMap[index] = 1;
	}
	return true;
}

bool CAStarPF::SetClearPos(const abase::vector<APointI>& vecBlocks)
{
	int index;
	for(int i = 0; i < (int)vecBlocks.size(); ++i)
	{
		index = GetPosIndex(vecBlocks[i]);
		if(m_pMap[index] == 0)
			assert(0);
		m_pMap[index] = 0;
	}
	return true;
}

bool CAStarPF::IsBlockPos(const APointI& pos)
{
	if(m_pMap[GetPosIndex(pos)] == 1)
		return true;
	return false;
}

bool CAStarPF::ClearMap()
{
	for(int i = 0; i< m_iRow*m_iCol; ++i)
		m_pMap[i] = 0;
	ClearPathSet();
	return true;
}

bool CAStarPF::FindPath(const APointI& start, const APointI& end, abase::vector<APointI>& vecPath)
{
	ClearPathSet();
	// 起点和终点必须是地图里面的点
	m_end = end;
	m_start = start;
	if( (!IsPosInMap(start.x, start.y)) || (!IsPosInMap(end.x, end.y)) )
	{
		assert(0);
		return false;
	}
	// 起点必须是被阻塞的点，终点必须是没有被阻塞的点
	if( (!IsBlockPos(start)) || (IsBlockPos(end)) )
	{
		assert(0);
		return false;
	}
	PathNode* pNode = new PathNode(start);
	pNode->evaluate_h = GetEvaluateH(start);
	pNode->evaluate_f = pNode->evaluate_g + pNode->evaluate_h;
	m_openSet[GetPosIndex(start)] = pNode;
	
	if( 0 == pNode->evaluate_f)
	{
		// 起点与终点重合了
		assert(0);
		//Finish();
		return true;
	}
	PathNode* pLeast = GetLeastNode();
	while(pLeast)
	{
		if(DealNextNode(pLeast))
		{
			Finish(vecPath);
			return true;
		}
		pLeast = GetLeastNode();
	}
	
	return true;
}

bool CAStarPF::Release()
{
	if(m_pMap)
		delete[] m_pMap;
	m_pMap = NULL;
	
	ClearPathSet();
	return true;
}

void CAStarPF::ClearPathSet()
{
	PathSet::iterator it = m_openSet.begin();
	for(; it != m_openSet.end(); ++it)
		delete it->second;
	m_openSet.clear();
	it = m_closeSet.begin();
	for(; it != m_closeSet.end(); ++it)
		delete it->second;
	m_closeSet.clear();	
}

int CAStarPF::GetEvaluateH(int x, int y)
{
	return abs(x - m_end.x) + abs(y - m_end.y);
}

int CAStarPF::GetEvaluateH(const APointI& pos)
{
	return abs(pos.x - m_end.x) + abs(pos.y - m_end.y);
}

void CAStarPF::Finish(abase::vector<APointI>& vecPath)
{
	// 取出路径，此时路径为倒序
	PathSet::iterator it = m_openSet.find(GetPosIndex(m_end));
	PathNode* pEnd = it->second;
	if(pEnd)
	{
		PathNode* pNode = pEnd;
		while(pNode)
		{
			vecPath.push_back(pNode->pos);
			pNode = pNode->pParent;
		}
	}
	// 将起始点和终点的阻塞状态重置
	m_pMap[GetPosIndex(m_end)] = 1;
	m_pMap[GetPosIndex(m_start)] = 0;
}

CAStarPF::PathNode* CAStarPF::GetLeastNode()
{
	PathSet::iterator it = m_openSet.begin();
	PathNode* pLeast = NULL;
	for(; it!= m_openSet.end(); ++it )
	{
		PathNode* pNode = it->second;
		if(!pLeast)
			pLeast = pNode;
		else
		{	
			if(pLeast->evaluate_f > pNode->evaluate_f)
				pLeast = pNode;
		}
	}
	return pLeast;
}

bool CAStarPF::DealNextNode(PathNode* pNode)
{
	if(!pNode)
	{
		assert(0);
		return false;
	}
	int x = pNode->pos.x;
	int y = pNode->pos.y;

	if(_DealNextNode(x, y-1, pNode)) { return true; }
	if(_DealNextNode(x-1, y, pNode)) { return true; }
	if(_DealNextNode(x, y+1, pNode)) { return true; }
	if(_DealNextNode(x+1, y, pNode)) { return true; }
	
	m_openSet.erase(GetPosIndex(pNode->pos));
	m_closeSet[GetPosIndex(pNode->pos)] = pNode;
	return false;
}

bool CAStarPF::_DealNextNode(int x, int y, PathNode* pFrom)
{
	int index = GetPosIndex(x,y);
	// 该点是障碍点
	if(!m_pMap || !IsPosInMap(x, y) || m_pMap[index] == 1)
		return false;
	// 该点在关闭点集合中
	if(m_closeSet.find(index) != m_closeSet.end())
		return false;
	
	PathSet::iterator it;
	PathNode* pNode = NULL;
	if((it = m_openSet.find(index)) != m_openSet.end())
	{
		pNode = it->second;
		if(pNode->evaluate_g <= pFrom->evaluate_g + 1)
			return false;
	}
	else
	{
		pNode = new PathNode(x,y);
		pNode->evaluate_h = GetEvaluateH(x,y);
		m_openSet[index] = pNode;
	}
	pNode->evaluate_g = pFrom->evaluate_g + 1;
	pNode->evaluate_f = pNode->evaluate_g + pNode->evaluate_h;
	pNode->pParent = pFrom;

	if(pNode->evaluate_h == 0)
		return true;

	return false;
}

bool CAStarPF::IsPosInMap(int x, int y)
{
	if(x <0 || x >= m_iCol || y <0 || y >= m_iRow)
		return false;
	return true;
}