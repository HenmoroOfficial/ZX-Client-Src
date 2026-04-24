// WayPointGraph.cpp: implementation of the CWayPointGraph class.
//
//////////////////////////////////////////////////////////////////////


#include "global.h"
#include "WayPointGraph.h"
//#include "PathFinding2D.h"
#include "WayPointTable.h"
//#include "MoveMap.h"
#include "ElementMap.h"
#include "MainFrm.h"
#include "PathMap/PlayerPassMapGenerator.h"

//#define new A_DEBUG_NEW

#define  LOCAL_MAGNET_RANGE  5.0f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWayPointGraph::CWayPointGraph()
{

}

CWayPointGraph::~CWayPointGraph()
{
	Release();
}

bool CWayPointGraph::Init()
{
	return true;
}

void CWayPointGraph::Release()
{
	unsigned int i;
	m_WayEdges.clear();
	
	for (i = 0; i < m_WayPoints.size(); i++)
	{
		delete m_WayPoints[i];
	}

	m_WayPoints.clear();
}

bool CWayPointGraph::AddWayEdge(int x1, int z1, int x2, int z2)
{
	//unreachable?
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if (!pMap || !pMap->GetPlayerPassMapGen() || !pMap->GetPlayerPassMapGen()->GetPassMap())
	{
		return false;
	}
	CPlayerPassMapGenerator * pGen = pMap->GetPlayerPassMapGen();

	if (!pGen->IsPosReachable(x1, z1) || !pGen->IsPosReachable(x2, z2))
	{
		return false;
	}
	
	if (x1 == x2 && z1 == z2)
	{
		return false;
	}
	WayEdge  e;
	
	
	
	if (_FindWayEdge(x1, z1, x2, z2, LOCAL_MAGNET_RANGE) != m_WayEdges.end())
	{
		return false;
	}
	bool bNewV1, bNewV2;
	bNewV1 = true;
	bNewV2 = true;
	e.v1 = NULL;
	e.v2 = NULL;
	vector<WayPoint*>::iterator it;
	it = _FindWayPoint(x1, z1, LOCAL_MAGNET_RANGE);
	if (it != m_WayPoints.end())
	{
		e.v1 = *it;
		bNewV1 = false;
	}
	it = _FindWayPoint(x2, z2, LOCAL_MAGNET_RANGE);
	if (it != m_WayPoints.end())
	{
		e.v2 = *it;
		bNewV2 = false;
	}
	
	if (bNewV1)
	{
		e.v1 = new WayPoint;
		e.v1->x = x1;
		e.v1->y = z1;
		//m_WayPoints.push_back(e.v1);
	}
	if (bNewV2)
	{
		e.v2 = new WayPoint;
		e.v2->x = x2;
		e.v2->y = z2;
		//m_WayPoints.push_back(e.v2);
	}
	if (!_Connect(e.v1, e.v2, e.path))
	{
		if (bNewV1)
		{
			delete e.v1;
			e.v1 = NULL;
		}
		if (bNewV2)
		{
			delete e.v2;
			e.v2 = NULL;
		}
		return false;
	}

	if (bNewV1)
	{
		m_WayPoints.push_back(e.v1);
	}

	if (bNewV2)
	{
		m_WayPoints.push_back(e.v2);
	}
	e.length = GetEuclDist(e.v1->x, e.v1->y, e.v2->x, e.v2->y);
	m_WayEdges.push_back(e);

	return true;

}


bool CWayPointGraph::AddWayPoint(int x, int z)
{
	unsigned int i;
	//unreachable?
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if (!pMap || !pMap->GetPlayerPassMapGen() || !pMap->GetPlayerPassMapGen()->GetPassMap())
	{
		return false;
	}
	CPlayerPassMapGenerator * pGen = pMap->GetPlayerPassMapGen();
	
	if (!pGen->IsPosReachable(x, z))
	{
		return false;
	}

	//already exist?
	for (i = 0; i < m_WayPoints.size(); i++)
	{
		WayPoint * w= m_WayPoints[i];
		if (w->x == x && w->y == z)
		{
			return false;
		}
	}
	
	WayPoint * v1 = new WayPoint;
	v1->x = x;
	v1->y = z;
	WayEdge    e;
	e.v1 = v1;
	for (i = 0; i < m_WayPoints.size(); i++)
	{
		WayPoint * v2= m_WayPoints[i];
		
		if (_Connect(v1, v2, e.path))
		{
			e.v2 = v2;
			e.length = GetEuclDist(v1->x, v1->y, v2->x, v2->y);
			m_WayEdges.push_back(e);
		}
	}
	
	m_WayPoints.push_back(v1);
	return true;
}

bool CWayPointGraph::DelWayPoint(int x, int z)
{
	
	vector<WayPoint * >::iterator it;
	for (it = m_WayPoints.begin(); it != m_WayPoints.end(); it++)
	{
		if ((*it)->x == x && (*it)->y == z)
		{
			break;
		}
	}
	if (it == m_WayPoints.end())
	{
		return false;
	}
	WayPoint * w = *it;
	m_WayPoints.erase(it);
	unsigned int i;
	vector<WayEdge> tmpEdge(m_WayEdges);
	m_WayEdges.clear();

	for (i = 0; i < tmpEdge.size(); i++)
	{
		if (tmpEdge[i].v1 == w || tmpEdge[i].v2 == w)
		{
			continue;
		}
		m_WayEdges.push_back(tmpEdge[i]);
	}
	
	return true;
}

void CWayPointGraph::Import(CWayPointTable * tbl)
{
#define LOCAL_GET_I(key)		((WORD)((DWORD)(key) & 0xffff))	
#define LOCAL_GET_J(key)		((WORD)((DWORD)(key) >> 16))

	Release();
	if (!tbl)
	{
		return;
	}
	CWayPointTable::EdgeTable * pEdgeTable = tbl->GetEdgeTable();
	if (!pEdgeTable)
	{
		return;
	}
	vector<A3DPOINT2> * pWays = tbl->GetWayPoint();
	CWayPointTable::EdgeTable::iterator it;
	for (it = pEdgeTable->begin(); it != pEdgeTable->end(); ++it)
	{
		unsigned int key = *(it.key());
		int i = LOCAL_GET_I(key);
		int j = LOCAL_GET_J(key);
		AddWayEdge(pWays->at(i).x, pWays->at(i).y, pWays->at(j).x, pWays->at(j).y);
	}

#undef  LOCAL_GET_I
#undef  LOCAL_GET_J
}

void CWayPointGraph::Export(CWayPointTable * tbl)
{
	assert(tbl);
	if (m_WayPoints.empty())
	{
		return;
	}

	int count = m_WayPoints.size();
	
	int ** transtable;
	transtable = new int *[count];
	int i;
	for (i = 0; i < count; i++)
	{
		transtable[i] = new int[count];
	}

	hashtab<int, WayPoint*, _hash_function>  p2i(256);
	for (i = 0; i < count; i++)
	{
		p2i.put(m_WayPoints[i], i);
	}
	_CalTransTable(p2i, transtable);
	
	vector<WayPoint> waypoints;
	for (i = 0; i < count; i++)
	{
		waypoints.push_back(*m_WayPoints[i]);
	}

	tbl->Set(waypoints, transtable);

	//edges
	for (i = 0; i < (int)m_WayEdges.size(); i++)
	{
		hashtab<int, WayPoint*, _hash_function>::iterator it;
		WayPoint * v1 = m_WayEdges[i].v1;
		it = p2i.find(v1);
		int iv1 = *(it.value());
		
		WayPoint * v2 = m_WayEdges[i].v2;
		it = p2i.find(v2);
		int iv2 = *(it.value());
		
		tbl->AddEdge(iv1, iv2, m_WayEdges[i].path);
	}

	for (i = 0; i < count; i++)
	{
		delete[]	transtable[i];
	}
	delete[]  transtable;
}

//dynamic programming to improve performance
void CWayPointGraph::_CalTransTable(hashtab<int, WayPoint*, _hash_function>& p2i, int ** transtable)
{
	assert(transtable);
	int count = m_WayPoints.size();
	int i, j, k;


	


#define  PF_DIST_MAX     1E6
	//init
	float ** Dist;
	int ** Pathtable;
	Dist = new float *[count];
	Pathtable = new int *[count];
	for (i = 0; i < count; i++)
	{
		Dist[i] = new float[count];
		Pathtable[i] = new int[count];
	}
	for (i = 0; i < count; i++)
	{
		for (j = 0; j < count; j++)
		{
			Dist[i][j] = (i != j)? (PF_DIST_MAX): (0.0f);
			Pathtable[i][j] = (i != j)? (-1): (i);
		}
	}
	for (k = 0; k < (int)m_WayEdges.size(); k++)
	{
		WayPoint * v1 = m_WayEdges[k].v1;
		WayPoint * v2 = m_WayEdges[k].v2;
		hashtab<int, WayPoint*, _hash_function>::iterator it;
		it = p2i.find(v1);
		i = *(it.value());
		it = p2i.find(v2);
		j = *(it.value());

		Dist[i][j]  = Dist[j][i] = m_WayEdges[k].length;
		Pathtable[i][j] = j;
		Pathtable[j][i] = i;
	}

	//do the magic
	for (k = 0; k < count; k++)
	{
		for (i = 0; i < count; i++)
		{
			for (j = 0; j < count; j++)
			{
				if (Dist[i][j] > Dist[i][k] + Dist[k][j] )
				{
					Dist[i][j] = Dist[i][k] + Dist[k][j];
					Pathtable[i][j] = k;
				}
				
			}
		}
	}


	//convert to transtable
	for (i = 0; i < count; i++)
	{
		for (j = 0; j < count; j++)
		{
			transtable[i][j] = _GetNextHop(Pathtable, i, j);
		}
	}


	//release
	for (i = 0; i < count; i++)
	{
		delete[] Dist[i];
		delete[] Pathtable[i];
	}
	delete[] Dist;
	delete[] Pathtable;
	
#undef  PF_DIST_MAX
}

int CWayPointGraph::_GetNextHop(int ** Pathtable, int i, int j)
{
	while (Pathtable[i][j] != j && Pathtable[i][j] != -1)
	{
		j = Pathtable[i][j];
	}

	return Pathtable[i][j];
}



bool CWayPointGraph::_Connect(WayPoint * v1, WayPoint * v2, vector<A3DPOINT2>& path)
{
#define LOCAL_SIGN(x) ( (x>0)? 1 : ((x<0)?-1:0) )
#define LOCAL_ABS(x) ((x>0)?x:(-x))

	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if (!pMap || !pMap->GetPlayerPassMapGen() || !pMap->GetPlayerPassMapGen()->GetPassMap())
	{
		return false;
	}
	CPlayerPassMapGenerator * pGen = pMap->GetPlayerPassMapGen();

	int x,y,dx,dy,s1,s2;	
	
	int x1, y1, x2, y2;
	x1 = v1->x;
	y1 = v1->y;
	x2 = v2->x;
	y2 = v2->y;

	x = x1;
	y = y1;
	dx = x2 - x1;
	dy = y2 - y1;
	s1 = LOCAL_SIGN(dx);
	s2 = LOCAL_SIGN(dy);
	dx = LOCAL_ABS(dx);
	dy = LOCAL_ABS(dy);

	// Interchange ?
	bool bInterchange = false;
	if( dy > dx )
	{
		int tmp = dx;
		dx = dy;
		dy = tmp;
		bInterchange = true;
	}

	// Init the error term
	int e = 2*dy-dx;
	
	path.clear();
	A3DPOINT2 pos;

	// Start loop, now dx >= dy
	for(int i=0; i<dx; i++)
	{
		pos.x = x;
		pos.y = y;
		if (!pGen->IsPosReachable(pos.x, pos.y))
		{
			return false;
		}
		path.push_back(pos);
		while( e > 0 )
		{
			if(bInterchange)
				x += s1;
			else
				y += s2;

			e -= 2*dx;
		}

		if(bInterchange)
			y += s2;
		else
			x += s1;

		e += 2*dy;
	}

	pos.x = x2;
	pos.y = y2;
	if (!pGen->IsPosReachable(pos.x, pos.y))
	{
		return false;
	}
	path.push_back(pos);
#undef LOCAL_SIGN
#undef LOCAL_ABS

	return true;
}

vector<CWayPointGraph::WayPoint*>::iterator CWayPointGraph::_FindWayPoint(int x, int z, float range)
{
	vector<WayPoint*>::iterator it;
	for (it = m_WayPoints.begin(); it != m_WayPoints.end(); ++it)
	{
		if (GetEuclDist( (*it)->x, (*it)->y, x, z) < range)
		{
			break;
		}
	}

	return it;
}

vector<CWayPointGraph::WayEdge>::iterator CWayPointGraph::_FindWayEdge(int x1, int z1, int x2, int z2, float range)
{
	vector<WayEdge>::iterator it;
	for (it = m_WayEdges.begin(); it != m_WayEdges.end(); ++it)
	{
		if ((GetEuclDist(it->v1->x, it->v1->y, x1, z1) < range 
			&& GetEuclDist(it->v2->x, it->v2->y, x2, z2) < range )
			||(GetEuclDist(it->v1->x, it->v1->y, x2, z2) < range 
			&& GetEuclDist(it->v2->x, it->v2->y, x1, z1) < range ))
		{
			break;
		}
	}
	return it;
}

void CWayPointGraph::Reset()
{
	unsigned int i;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if (!pMap || !pMap->GetPlayerPassMapGen() || !pMap->GetPlayerPassMapGen()->GetPassMap())
	{
		return ;
	}
	CPlayerPassMapGenerator * pGen = pMap->GetPlayerPassMapGen();
	//waypoint
	vector<WayPoint *> tmpway(m_WayPoints);
	m_WayPoints.clear();
	for (i = 0; i < tmpway.size(); i++)
	{
		if (!pGen->IsPosReachable(tmpway[i]->x, tmpway[i]->y))
		{
			delete tmpway[i];
			tmpway[i] = NULL;
		}
		else
		{
			m_WayPoints.push_back(tmpway[i]);
		}
	}

	//wayedge
	vector<WayEdge> tmpedge(m_WayEdges);
	m_WayEdges.clear();

	
	vector<A3DPOINT2> path;

	for ( i = 0; i < tmpedge.size(); i++)
	{
		
		if ((_FindWayPoint(tmpedge[i].v1->x, tmpedge[i].v1->y) != m_WayPoints.end())&&
			(_FindWayPoint(tmpedge[i].v2->x, tmpedge[i].v2->y) != m_WayPoints.end())&&
			_Connect(tmpedge[i].v1, tmpedge[i].v2, path))
		{
			m_WayEdges.push_back(tmpedge[i]);
		}
	}

}

#undef  LOCAL_MAGNET_RANGE