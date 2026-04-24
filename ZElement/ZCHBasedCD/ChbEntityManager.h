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

#if !defined(AFX_CHFMANAGER_H__36788DEC_42B8_4444_B5D5_A5CFA154F4F2__INCLUDED_)
#define AFX_CHFMANAGER_H__36788DEC_42B8_4444_B5D5_A5CFA154F4F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector.h>
#include <hashmap.h>

#include "ConvexBrush.h"

#include "ChbEntity.h"

namespace CHBasedCD{


class CChbEntityManager  
{
protected:
	
	struct ChbEntityData{//
		CChbEntity               *_pEntity;
		int                      _ref;
		
		ChbEntityData(){ _ref = 0; _pEntity = new CChbEntity;}
		~ChbEntityData()
		{
			assert(_ref == 0);
			delete _pEntity;
		}
	};

	CRITICAL_SECTION _CriticalSection;

	abase::hash_map<AString, ChbEntityData*> m_chbs;
public:	

	//release all the entity. even the entity is being used
	void Release();

	//load convex hull data from file 
	//return value: pointer to CChbEntity, which can be used to collision detection. you can use it until ReleaseChb is called. LoadChb and ReleaseChb should be in pair. 
	CChbEntity* LoadChb(const AString& strchb, bool bUseCollisionTree = true);

	//convert chf(engine 2) to chb
	static bool Chf2Chb(const AString& strchf);
	
	//release convex hull brush
	//from now on, you can not use pointer from LoadChb(). LoadChb and ReleaseChb should be in pair.
	void ReleaseChb(const AString& strchb);
	
	
	//construction and deconstruction
	CChbEntityManager();
	virtual ~CChbEntityManager();

};
}
#endif // !defined(AFX_CHFMANAGER_H__36788DEC_42B8_4444_B5D5_A5CFA154F4F2__INCLUDED_)
