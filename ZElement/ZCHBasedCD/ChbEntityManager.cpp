/*
 * FILE: ChbEntityManager.h
 *
 * DESCRIPTION: convex hull brush entity (.chb) manager
 *
 * CREATED BY: YangXiao, 2010/11/10
 *
 * HISTORY: 
 *
 * Copyright (c) 2010 Perfect World, All Rights Reserved.
 */
//////////////////////////////////////////////////////////////////////

#include <AFile.h>
#include <AFI.h>

#include "ConvexHullDataSet.h"
#include "ChbEntityManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace CHBasedCD
{

CChbEntityManager::CChbEntityManager()
{
	InitializeCriticalSection(&_CriticalSection);
}

CChbEntityManager::~CChbEntityManager()
{
	Release();
	DeleteCriticalSection(&_CriticalSection);	
}

//release all the chb
void CChbEntityManager::Release()
{
	EnterCriticalSection(&_CriticalSection);
	abase::hash_map<AString, ChbEntityData*>::iterator iter; 
	for (iter = m_chbs.begin(); iter != m_chbs.end(); ++iter)
	{
		ChbEntityData* pEntity = iter->second;
		pEntity->_ref = 0;
		delete pEntity;
	}
	m_chbs.clear();
	LeaveCriticalSection(&_CriticalSection);
}

//load convex hull brush from file
CChbEntity* CChbEntityManager::LoadChb(const AString& strchb, bool bUseCollisionTree)
{	
	EnterCriticalSection(&_CriticalSection);
	abase::hash_map<AString, ChbEntityData*>::iterator iter; 
	iter = m_chbs.find(strchb);
	if (iter != m_chbs.end())
	{
		ChbEntityData* &pEntity = iter->second;
		pEntity->_ref++;
		LeaveCriticalSection(&_CriticalSection);
		return pEntity->_pEntity;//already load
	}

	ChbEntityData* pEntityData = new ChbEntityData;
	if (!pEntityData->_pEntity->Load(strchb, bUseCollisionTree))
	{
		delete pEntityData;
		LeaveCriticalSection(&_CriticalSection);
		return NULL;//load error
	}
	pEntityData->_ref = 1;
	m_chbs[strchb] = pEntityData;
	LeaveCriticalSection(&_CriticalSection);
	
	return pEntityData->_pEntity;
}

bool CChbEntityManager::Chf2Chb(const AString& strchf)
{
	AFile af;
	if (!af.Open(strchf, AFILE_BINARY|AFILE_OPENEXIST))
		return false;

	abase::vector<CConvexHullData*> _convexdata;

	int ChNum, i;
	DWORD readLen;
	const int IntSize = sizeof(int);
	
	af.Read(&ChNum, IntSize, &readLen);		
	//skip the mesh IDs
	for (i = 0; i < ChNum; i++)
	{
		abase::vector<int> MeshId;
		int MeshNum = 0;
		af.Read(&MeshNum, IntSize, &readLen);		
		af.Seek(IntSize*MeshNum, AFILE_SEEK_CUR);
	}		
	
	//read the convex hull data	
	for (i = 0; i < ChNum; i++)
	{
		CConvexHullData* pChData = new CConvexHullData;				
		
		if (pChData->LoadBinaryData(&af))
		{
			_convexdata.push_back(pChData);
		}
		else
		{
			break;
		}
	}
	af.Close();
	
	CChbEntity entity;
	entity.Import(_convexdata);

	AString strchb = strchf;
	af_ChangeFileExt(strchb, ".chb");

	for (i = 0; i < (int)_convexdata.size(); i++)
	{
		delete _convexdata[i];
	}
	
	return entity.Save(strchb);
}

//release convex hull brush
void CChbEntityManager::ReleaseChb(const AString& strchb)
{
	if (m_chbs.size() == 0 || m_chbs.find(strchb) == m_chbs.end())
	{
		return;//not exist
	}
	
	EnterCriticalSection(&_CriticalSection);
	ChbEntityData* pEntityData = m_chbs[strchb];
	pEntityData->_ref--;
	if (pEntityData->_ref == 0)
	{
		delete pEntityData;		
		m_chbs.erase(strchb);
	}
	LeaveCriticalSection(&_CriticalSection);
}


}