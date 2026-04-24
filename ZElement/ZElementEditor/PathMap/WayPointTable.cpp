// WayPointTable.cpp: implementation of the CWayPointTable class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning (disable: 4284)

#include "WayPointTable.h"
#include <AFileImage.h>
//#define new A_DEBUG_NEW

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



#define MAKE_EDGE_KEY(i, j)    ((DWORD)(((WORD)((i) & 0xffff)) | ((DWORD)((WORD)((j) & 0xffff))) << 16))

CWayPointTable::CWayPointTable()
:m_EdgeTable(128)
{
	m_TransTable = NULL;
}

CWayPointTable::~CWayPointTable()
{
	Release();
}

bool CWayPointTable::Init()
{
	return true;
}

void CWayPointTable::Release()
{
	if (m_TransTable)
	{
		int count = m_WayPoints.size();
		int i;
		for (i = 0; i < count; i++)
		{
			if (m_TransTable[i])
			{
				delete[] m_TransTable[i];
			}
		}
		delete[] m_TransTable;
		m_TransTable = NULL;
	}
	m_WayPoints.clear();

	EdgeTable::iterator  it;
	for (it = m_EdgeTable.begin(); it != m_EdgeTable.end(); ++it)
	{
		vector<A3DPOINT2> * path = *(it.value());
		delete path;
	}
	m_EdgeTable.clear();

}

void CWayPointTable::Set(const vector<WayPoint>& waypoints, int ** transtable)
{
	Release();
	m_WayPoints = waypoints;

	int count = m_WayPoints.size();
	assert(count > 0);
	
	m_TransTable = new int *[count];
	int i, j;
	for (i = 0; i < count; i++)
	{
		m_TransTable[i] = new int[count];
		for (j = 0; j < count; j++)
		{
			m_TransTable[i][j] = transtable[i][j];
		}
	}
	
}


int CWayPointTable::GetNearestWayPoint(int x, int z, float range)
{
	int iMin = -1;
	float fMinDist, fDist;
	int i;
	fMinDist = 1E6;	
	for (i = 0; i < (int)m_WayPoints.size(); i++)
	{
		fDist = GetManhDist(x, z, m_WayPoints[i].x, m_WayPoints[i].y);
		if (fDist < range && fDist < fMinDist)
		{
			fMinDist = fDist;
			iMin = i;
		}
	}
	return iMin;
}

bool CWayPointTable::GetPath(int i, int j, vector<A3DPOINT2>& path)
{
	path.clear();
	path.push_back(m_WayPoints[i]);
	if (i == j)
	{
		return true;
	}
	int k =i;
	while (m_TransTable[k][j] != j )
	{
		if (m_TransTable[k][j] == -1)
		{
			return false;
		}
		int next = m_TransTable[k][j];
		_AppendPath(k, next, path);
		k = next;
	}
	_AppendPath(k, j, path);
	
	return true;
}

bool CWayPointTable::Load(const char * szFile)
{
	Release();

	AFileImage   fileimg;
	if (!fileimg.Open(szFile, AFILE_BINARY|AFILE_OPENEXIST))
	{
		return false;
	}
	
	unsigned long readlen;
	int count;
	fileimg.Read(&count, sizeof(int), &readlen);
	int i;
	for (i = 0; i < count; i++)
	{
		A3DPOINT2 waypoint;
		fileimg.Read(&waypoint, sizeof(A3DPOINT2), &readlen);
		m_WayPoints.push_back(waypoint);
	}

	m_TransTable = new int*[count];
	for (i = 0; i < count; i++)
	{
		m_TransTable[i] = new int[count];
		fileimg.Read(m_TransTable[i], sizeof(int)*count, &readlen);
	}

	//edge
	fileimg.Read(&count, sizeof(int), &readlen);
	for (i = 0; i < count; i++)
	{
		unsigned int key;
		fileimg.Read(&key, sizeof(unsigned int), &readlen);
		unsigned int veccount;
		fileimg.Read(&veccount, sizeof(unsigned int), &readlen);
		vector<A3DPOINT2> * val = new vector<A3DPOINT2>;
		for (unsigned int j = 0; j < veccount; j++)
		{
			A3DPOINT2  w;
			fileimg.Read(&w, sizeof(A3DPOINT2), &readlen);
			val->push_back(w);
		}
		m_EdgeTable.put(key, val);
	}
	return true;
}

bool CWayPointTable::Save(const char * szFile)
{
	AFile   fileimg;
	if (!fileimg.Open(szFile, AFILE_BINARY|AFILE_CREATENEW))
	{
		return false;
	}
	unsigned long  writelen;
	int   count = m_WayPoints.size();	
	fileimg.Write(&count, sizeof(int), &writelen);
	int i;
	for (i = 0; i < count; i++)
	{
		fileimg.Write(&m_WayPoints[i], sizeof(A3DPOINT2), &writelen);
	}

	for (i = 0; i < count; i++)
	{
		fileimg.Write(m_TransTable[i], sizeof(int)*count, &writelen);
	}
	
	//edge
	count = m_EdgeTable.size();
	fileimg.Write(&count, sizeof(int), &writelen);
	EdgeTable::iterator it;
	for (it = m_EdgeTable.begin(); it != m_EdgeTable.end(); ++it)
	{
		unsigned int key = *(it.key());
		fileimg.Write(&key, sizeof(unsigned int), &writelen);
		vector<A3DPOINT2> * val = *(it.value());
		unsigned int veccount = val->size();
		fileimg.Write(&veccount, sizeof(unsigned int), &writelen);
		vector<A3DPOINT2>::iterator   itvec;
		for (itvec = val->begin(); itvec != val->end(); ++itvec)
		{
			fileimg.Write(itvec, sizeof(A3DPOINT2), &writelen);
		}
	}

	fileimg.Close();
	return true;
}

void CWayPointTable::AddEdge(int i, int j, const vector<A3DPOINT2>& path)
{
	assert(i >= 0 && j >= 0);
	unsigned int key = MAKE_EDGE_KEY(i, j);	
	vector<A3DPOINT2> * val = new vector<A3DPOINT2>(path);
	m_EdgeTable.put(key, val);
}

bool CWayPointTable::_AppendPath(int i, int j, vector<A3DPOINT2>& path)
{
	unsigned int key = MAKE_EDGE_KEY(i, j);	
	vector<A3DPOINT2> * val;
	EdgeTable::pair_type pair =	m_EdgeTable.get(key);
	int k;
	if (!pair.second)
	{
		key = MAKE_EDGE_KEY(j, i);
		pair = m_EdgeTable.get(key);
		assert(pair.second);

	}
	val = *pair.first;

	if (path.back() == val->front())
	{
		path.pop_back();
		for (k = 0; k < (int)val->size(); k++)
		{
			path.push_back(val->at(k));	
		}
	}
	else
	{
		assert(path.back() == val->back());
		path.pop_back();
		for (k = (int)val->size()-1; k >= 0; k--)
		{
			path.push_back(val->at(k));	
		}
	}

	return true;
}

bool CWayPointTable::CheckTransTable()
{
	if (!m_TransTable)
	{
		return false;
	}
	int count = m_WayPoints.size();
	int i, j;
	for (i = 0; i < count; i++)
	{
		for (j = 0; j < count; j++)
		{
			if (m_TransTable[i][j] < 0 || m_TransTable[i][j]>=count)
			{
				return false;
			}
		}
	}
	return true;
}
