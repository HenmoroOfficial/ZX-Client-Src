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




class CWayPointTable  
{
public:
	typedef A3DPOINT2 WayPoint;
	typedef 	hashtab<vector<A3DPOINT2>* , unsigned int, _hash_function>  EdgeTable;

	CWayPointTable();
	virtual ~CWayPointTable();
	bool Init();
	void Release();
	void Set(const vector<WayPoint>& waypoints, int ** transtable);
	/**
	 * \brief get the nearest waypoint in the given range
	 * \param[in]
	 * \param[out]
	 * \return  the waypoint id, or -1 if not find
	 * \note  currently using Manh dist
	 * \warning
	 * \todo   
	 * \author kuiwu 
	 * \date 26/9/2006
	 * \see 
	 */
	int  GetNearestWayPoint(int x, int z, float range);
	A3DPOINT2 GetWayPoint(int id)
	{
		return m_WayPoints[id];
	}
	vector<WayPoint> * GetWayPoint()
	{
		return &m_WayPoints;
	}
	EdgeTable * GetEdgeTable()
	{
		return &m_EdgeTable;
	}
	bool GetPath(int i, int j, vector<A3DPOINT2>& path);
	bool Load(const char * szName);
	bool Save(const char * szName);
	
	void AddEdge(int i, int j, const vector<A3DPOINT2>& path);
	
	bool CheckTransTable();

private:
	vector<WayPoint>     m_WayPoints;
	int    **            m_TransTable;
	EdgeTable            m_EdgeTable;	

	bool _AppendPath(int i, int j, vector<A3DPOINT2>& path);
};

static inline float GetManhDist(int x1, int z1, int x2, int z2)
{
	 return (fabs(x1- x2) + fabs(z1 - z2));
};
static inline float GetEuclDist(int x1, int z1, int x2, int z2)
{
	return (sqrtf((x1 - x2)*(x1 -x2) + (z1 -z2)*(z1 -z2)));
};


