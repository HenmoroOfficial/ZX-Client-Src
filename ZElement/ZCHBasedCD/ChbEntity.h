/*
 * FILE: ChbEntity.h
 *
 * DESCRIPTION: convex hull brush scene (.chb) 
 *
 * CREATED BY: YangXiao, 2010/11/10
 *
 * HISTORY: 
 *
 * Copyright (c) 2010 Perfect World, All Rights Reserved.
 */
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHBENTITY_H__77E7C00C_FF26_45FF_8AC3_C4C4AA59D982__INCLUDED_)
#define AFX_CHBENTITY_H__77E7C00C_FF26_45FF_8AC3_C4C4AA59D982__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector.h>

#include "ConvexBrush.h"

namespace CHBasedCD
{

class EntityCollisionTree;

class CChbEntity  
{
	abase::vector<CConvexBrush*> m_convexBrush;
	A3DAABB m_aabb;
	EntityCollisionTree* m_pTree;//tree for collision detection 

public:

	//load and save
	bool Load(const AString& strFile, bool bUseCollisionTree = true);

	bool Save(const AString& strFile);

	//capsule brush collision
	bool CapusuleTrace(CapusuleBrushTraceInfo * pInfo, bool bCheckFlag = true);
	
	//AABB brush collision
	bool AABBTrace(AABBBrushTraceInfo2 * pInfo, bool bCheckFlag = true) const;
	
	//ray brush collision
	bool RayTrace(RayBrushTraceInfo* pInfo, bool bCheckFlag = true) const;

	//release all the resource
	void Release();

	//import from convex hull data	
	void Import(const abase::vector<CConvexHullData*>& pCHs);

	//set/clear collision flag CHDATA_SKIP_COLLISION, CHDATA_SKIP_RAYTRACE (CConvexHullData for detail)
	void SetFlag(DWORD dwFlag)
	{
		for (int i = 0; i < (int)m_convexBrush.size(); i++)
			m_convexBrush[i]->SetCollisionFlag(m_convexBrush[i]->GetCollisionFlag() | dwFlag);
	}
	void ClearFlag(DWORD dwFlag)
	{
		for (int i = 0; i < (int)m_convexBrush.size(); i++)
			m_convexBrush[i]->SetCollisionFlag(m_convexBrush[i]->GetCollisionFlag() & (~dwFlag));
	}

	A3DAABB GetAABB() const { return m_aabb; }

	//get convex brush count
	int GetBrushCount(){ return m_convexBrush.size(); }

	//get i-th convex brush, make sure i between 0~GetBrushCount()-1, we do not check it in release version
	CConvexBrush* GetBrush(int i){ assert(i >= 0 && i < (int)m_convexBrush.size()); return m_convexBrush[i]; }

	//Get brush num
	int GetBrushNum() { return m_convexBrush.size(); }

	//get all the brush
	const abase::vector<CConvexBrush*>& GetBrushes() { return m_convexBrush; }
	
	//construction and deconstruction
	CChbEntity();
	virtual ~CChbEntity();

	//compute aabb given world matrix
	inline A3DAABB GetTransformedAABB(const A3DMATRIX4& mat) const;

	//debug render
//#ifdef _DEBUG
	void DebugRender(A3DFlatCollector* pFC, float fScale = 1, DWORD dwFColor=0xa0ffff00, const A3DVECTOR3& vRayDir=A3DVECTOR3(1.0f,1.0f,1.0f), const A3DMATRIX4* pTransMatrix=NULL);
//#endif

};

A3DAABB CChbEntity::GetTransformedAABB(const A3DMATRIX4& mat) const
{
	AArray<A3DVECTOR3,const A3DVECTOR3&> vs;	
	for ( int i = 0; i < (int)m_convexBrush.size(); i++)
	{
		for (int j = 0; j < (int)m_convexBrush[i]->GetVertexNum(); j++ )
		{
			vs.Add(m_convexBrush[i]->GetVertex(j) * mat);
		}
	}
	A3DAABB aabb;
	aabb.Build(vs.GetData(), vs.GetSize());
	return aabb;
}

}

#endif // !defined(AFX_CHBENTITY_H__77E7C00C_FF26_45FF_8AC3_C4C4AA59D982__INCLUDED_)
