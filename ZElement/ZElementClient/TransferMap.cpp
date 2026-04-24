/********************************************************************
	created:	2007/04/09
	author:		kuiwu
	
	purpose:	tranfer info between maps
	Copyright (C) 2007 - All Rights Reserved
*********************************************************************/

#include "TransferMap.h"
#include "AFileImage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace AutoMove
{



CTransferMap::CTransferMap()
{
	m_TransTab = NULL;
}

CTransferMap::~CTransferMap()
{
	
	Release();
	
}


void CTransferMap::Release()
{

	unsigned int count = m_Id.size();
	if (count > 0 && m_TransTab)
	{
		unsigned	int i;
		for (i = 0; i < count; i++)
		{
			if (m_TransTab[i])
			{
				delete[] m_TransTab[i];
			}
		}
		delete[] m_TransTab;
		m_TransTab = NULL;
	}

	m_Id.clear();
	m_TransEdge.clear();
}

bool CTransferMap::Load(const char * szFilePath)
{
	Release();

	AFileImage aFile;
	if( !aFile.Open(szFilePath, AFILE_OPENEXIST | AFILE_TEMPMEMORY) )
	{
		return false;
	}
	char line[300];
	DWORD len;
	char seps[]   = " (),\t\n";
	bool  done = false;
	char * token;
	while (aFile.ReadLine(line, 300, &len))
	{
		token =strtok(line, seps);
		if (token && token[0] == 'x')
		{
			done = true;
			break;
		}
	}
	
	if (!done)
	{
		Release();
		return false;
	}

	while (token = strtok(NULL, seps))
	{
		int id = atoi(token);
		m_Id.push_back(id);
	}

	unsigned int count = m_Id.size();
	if (count == 0)
	{
		Release();
		return false;
	}

	unsigned int i;
	m_TransTab = new int*[count];
	memset(m_TransTab, 0, sizeof(int*) * count);

	i = 0;
	while (i < count)
	{
		if (!aFile.ReadLine(line, 300, &len))
		{
			Release();
			return false;
		}
		m_TransTab[i] = new int[count];
		token =strtok(line, seps);
		unsigned	int j = 0;
		while (token = strtok(NULL, seps))
		{
			int id = atoi(token);
			m_TransTab[i][j] = id;
			j++;
		}

		if (j != count)
		{
			Release();
			return false;
		}
		i++;
	}

	
	while (aFile.ReadLine(line, 300, &len))
	{
		token =strtok(line, seps);
		if (!token)
		{
			continue;
		}
		TransEdge  e;
		e.id1 = atoi(token);
		token = strtok(NULL, seps);
		e.pos1.x = (float)atof(token);
		token = strtok(NULL, seps);
		e.pos1.y = (float)atof(token);
		token = strtok(NULL, seps);
		e.pos1.z = (float)atof(token);
		token = strtok(NULL, seps);
		e.id2 = atoi(token);
		
		m_TransEdge.push_back(e);
		
	}
	
	
	return  true;
}


bool CTransferMap::GetTransferInfo(int from, int to, A3DVECTOR3& throughPos)
{
	int from_index, to_index;
	
	from_index = _GetIndex(from);
	to_index = _GetIndex(to);

	if (from_index < 0 || to_index < 0)
	{
		return false;
	}

	int mid = m_TransTab[from_index][to_index];

	unsigned int i;
	for (i = 0; i < m_TransEdge.size(); i++)
	{
		if (m_TransEdge[i].id1 == from && m_TransEdge[i].id2 == mid)
		{
			throughPos = m_TransEdge[i].pos1;
			return true;
		}
		

	}
	
	
	return false;
}


int CTransferMap::_GetIndex(int id)
{
	unsigned int i;
	for (i = 0; i < m_Id.size(); i++)
	{
		if (m_Id[i] == id)
		{
			return i;
		}
	}

	return -1;
}

} //end of namespace