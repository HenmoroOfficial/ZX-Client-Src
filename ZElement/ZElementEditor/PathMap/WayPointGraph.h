/********************************************************************
	created:	2006/09/25
	author:		kuiwu
	
	purpose:	
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/

#pragma  once

#include <vector.h>
#include <hashtab.h>
using namespace abase;
#include <A3DTypes.h>


class CWayPointTable;

class CWayPointGraph  
{
public:
// 	typedef struct  
// 	{
// 		int x;
// 		int y;
// 	}WayPoint;

	typedef A3DPOINT2  WayPoint;

	typedef	struct  
	{
		WayPoint * v1;
		WayPoint * v2;
		float length;
		vector<A3DPOINT2> path;
	}WayEdge;


	CWayPointGraph();
	virtual ~CWayPointGraph();
	
	
	bool AddWayPoint(int x, int z);
	bool DelWayPoint(int x, int z);
	bool AddWayEdge(int x1, int z1, int x2, int z2);

	bool Init();
	void Release();

	vector<WayPoint *> * GetWayPoints() 
	{
		return &m_WayPoints;
	}
	vector<WayEdge> * GetWayEdges()
	{
		return &m_WayEdges;
	}
	void   Export(CWayPointTable * tbl);
	void   Import(CWayPointTable * tbl);
	void   Reset();
	
private:
	vector<WayPoint *>    m_WayPoints;
	vector<WayEdge>      m_WayEdges;

	void _CalTransTable(hashtab<int, WayPoint*, _hash_function>& p2i, int ** transtable);
	//Bresenham,  the path(line) is [v1, v2]
	bool _Connect(WayPoint * v1, WayPoint * v2, vector<A3DPOINT2>& path);
	
	int  _GetNextHop(int ** Pathtable, int i, int j);
	
	vector<WayEdge>::iterator   _FindWayEdge(int x1, int z1, int x2, int z2, float range = 0.01f);
	vector<WayPoint*>::iterator  _FindWayPoint(int x, int z, float range = 0.01f);
};


