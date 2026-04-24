/*
 * FILE: ChbEntity.h
 *
 * DESCRIPTION: convex brush scene (.chb) 
 *
 * CREATED BY: YangXiao, 2010/11/10
 *
 * HISTORY: 
 *
 * Copyright (c) 2010 Perfect World, All Rights Reserved.
 */
//////////////////////////////////////////////////////////////////////

#include <AFile.h>
#include <A3DCollision.h>

#include "ChbEntity.h"
#include "ConvexHullDataSet.h"
#include <A3DWireCollector.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace CHBasedCD
{

//////////////////////////////////////////////////////////////////////////
	inline bool AABBAABBOverlap(const A3DAABB& aabb1, const A3DAABB& aabb2)
	{
		if (aabb1.Mins.x > aabb2.Maxs.x || aabb2.Mins.x > aabb1.Maxs.x)
			return false;
		if (aabb1.Mins.y > aabb2.Maxs.y || aabb2.Mins.y > aabb1.Maxs.y)
			return false;
		if (aabb1.Mins.z > aabb2.Maxs.z || aabb2.Mins.z > aabb1.Maxs.z)
			return false;
		return true;
	}
typedef abase::vector<CConvexBrush*>							BrushVec;
struct EntityCollisionTreeNode
{
	A3DAABB aabb;
	EntityCollisionTreeNode* children[8];
	BrushVec brushes;
	EntityCollisionTreeNode()
	{
		aabb.Clear();
		for (int i = 0; i < 8; i++)
		{
			children[i] = NULL;
		}
	}
	~EntityCollisionTreeNode()
	{
		if (children[0])
		{
			for (int i = 0; i < 8; ++i)
			{
				delete children[i];
				children[i] = NULL;
			}
		}
	}
	bool isLeaf() const { return children[0] == NULL; }
};
class EntityCollisionTree
{
	EntityCollisionTreeNode* m_pRoot;
	int m_MinBrushInNode;//min num of brushes in node
	float m_MinNodeSize;//min node size, x-extents

	bool m_bCheckCHFlags;//check collision flag?

	void _Split(EntityCollisionTreeNode* pNode);

	bool _Trace(EntityCollisionTreeNode* pNode, AbstractBrushTraceInfo* pInfo);
	void _Render(EntityCollisionTreeNode* pNode, A3DWireCollector* pWC, const A3DMATRIX4& mat);

public:
	EntityCollisionTree(const BrushVec& brushes);
	EntityCollisionTree();
	~EntityCollisionTree();
	void Release();
	void Build(const BrushVec& brushes, int minBrushInNode = 16, float minNodeSize = 16);

	bool Trace(AbstractBrushTraceInfo* pInfo, bool bCheckCHFlags = true);

	int GetMinBrushInNode() const { return m_MinBrushInNode; }
	float GetMinNodeSize() const { return m_MinNodeSize; }

	void Render(A3DWireCollector* pWC, const A3DMATRIX4& mat);
};
EntityCollisionTree::EntityCollisionTree()
{
	m_pRoot = NULL;
}
EntityCollisionTree::EntityCollisionTree(const BrushVec& brushes)
{

	Build(brushes);
}
EntityCollisionTree::~EntityCollisionTree()
{
	Release();
}
void EntityCollisionTree::Build(const BrushVec& brushes,  int minBrushInNode, float minNodeSize)
{
	Release();

	m_MinBrushInNode = minBrushInNode;
	m_MinNodeSize = minNodeSize;
	m_pRoot = new EntityCollisionTreeNode;

	A3DAABB aabb;
	aabb.Clear();

	size_t i;
	for (i = 0; i < brushes.size(); i++)
	{
		aabb.Merge(brushes[i]->GetAABB());
		m_pRoot->brushes.push_back(brushes[i]);
	}
	m_pRoot->aabb = aabb;

	_Split(m_pRoot);
}
void EntityCollisionTree::_Split(EntityCollisionTreeNode* pNode)
{	
	if ((int)pNode->brushes.size() < m_MinBrushInNode || pNode->aabb.Extents.x * 2 < m_MinNodeSize + 0.1f)
		return;

	A3DVECTOR3 child_ext = pNode->aabb.Extents * 0.5f;
	float candidatex[] = {pNode->aabb.Center.x - child_ext.x, pNode->aabb.Center.x + child_ext.x};
	float candidatey[] = {pNode->aabb.Center.y - child_ext.y, pNode->aabb.Center.y + child_ext.y};
	float candidatez[] = {pNode->aabb.Center.z - child_ext.z, pNode->aabb.Center.z + child_ext.z};

	for (int i = 0; i < 8; i++)
	{
		pNode->children[i] = new EntityCollisionTreeNode;
		pNode->children[i]->aabb.Center = A3DVECTOR3(candidatex[i&1], candidatey[(i&2)>>1], candidatez[(i&4)>>2]);
		pNode->children[i]->aabb.Extents = child_ext;
		pNode->children[i]->aabb.CompleteMinsMaxs();

		//divide brushes into child node
		for (size_t j = 0; j < pNode->brushes.size(); j++)
		{
			if (AABBAABBOverlap(pNode->brushes[j]->GetAABB(), pNode->children[i]->aabb))
				pNode->children[i]->brushes.push_back(pNode->brushes[j]);
		}

		_Split(pNode->children[i]);
	}

	//remove brushes of parent node
	pNode->brushes.swap(BrushVec());

}
void EntityCollisionTree::Release()
{
	if (m_pRoot)
	{
		delete m_pRoot;
		m_pRoot = NULL;
	}
}

bool EntityCollisionTree::Trace(AbstractBrushTraceInfo* pInfo, bool bCheckCHFlags)
{
	m_bCheckCHFlags = bCheckCHFlags;
	return _Trace(m_pRoot, pInfo);
}
bool EntityCollisionTree::_Trace(EntityCollisionTreeNode* pNode, AbstractBrushTraceInfo* pInfo)
{
	bool bIsRay = false;
	A3DAABB aabb(pNode->aabb);
	aabb.Center = aabb.Center * pInfo->fScale;
	aabb.Extents = aabb.Extents * pInfo->fScale;
	aabb.CompleteMinsMaxs();

	if (CapusuleBrushTraceInfo::IsInstance(pInfo))
	{
		if (!CapsuleAABBCollision((CapusuleBrushTraceInfo*)pInfo, aabb))
			return false;
	}
	else if (AABBBrushTraceInfo2::IsInstance(pInfo))
	{
		AABBBrushTraceInfo2* pAABBTraceInfo = (AABBBrushTraceInfo2*)pInfo;
		if (!CLS_AABBOBBOverlap(aabb.Center, aabb.Extents, pAABBTraceInfo->obb))
			return false;
	}
	else
	{
		bIsRay = true;
		if (!RayAABBCollision((RayBrushTraceInfo*)pInfo, aabb))
			return false;
	}

	bool bCollide = false;
	bool		bStartSolid ;	
	bool		bAllSolid ;		

	float fFraction = 100.0f ;
	CHalfSpace hs;
	if (pNode->isLeaf())
	{
		int i;
		for (i = 0; i < (int)pNode->brushes.size(); ++i)
		{
			CConvexBrush* pBrush = pNode->brushes[i];
						

			if (pBrush->BrushTrace(pInfo, m_bCheckCHFlags) && (pInfo->fFraction < fFraction)) 
			{
				//update the saving info
				bStartSolid = pInfo->bStartSolid;
				bAllSolid = pInfo->bAllSolid;

				fFraction = pInfo->fFraction;
				hs = pInfo->ClipPlane;					
				bCollide = true;
				if (bStartSolid)
					pInfo->dwUser2 = (DWORD)pBrush;
	
				if (pInfo->fFraction == 0.0f)
					//if (bStartSolid)
				{
					break;
				}
			}
		}
	}
	else
	{
		for (int j = 0; j < 8; j++)
		{
			if (_Trace(pNode->children[j], pInfo) && pInfo->fFraction < fFraction)
			{
				bStartSolid = pInfo->bStartSolid;
				bAllSolid = pInfo->bAllSolid;

				fFraction = pInfo->fFraction;
				hs = pInfo->ClipPlane;					
				bCollide = true;
				if (pInfo->fFraction == 0.0f)
					//if (bStartSolid)
				{
					break;
				}
			}
		}
	}

	if (bCollide)
	{
		//set back
		pInfo->bStartSolid = bStartSolid;
		pInfo->bAllSolid = bAllSolid;

		pInfo->fFraction = fFraction;
		pInfo->ClipPlane = hs;
	}
	return bCollide;	
}

void EntityCollisionTree::_Render(EntityCollisionTreeNode* pNode, A3DWireCollector* pWC, const A3DMATRIX4& mat)
{
	if (pNode == NULL)
		return;

	pWC->AddAABB(pNode->aabb, 0xff800000, &mat);
	for (int i = 0; i < 8; i++)
	{
		_Render(pNode->children[i], pWC, mat);
	}
}

void EntityCollisionTree::Render(A3DWireCollector* pWC, const A3DMATRIX4& mat)
{
	_Render(m_pRoot, pWC, mat);

}
//////////////////////////////////////////////////////////////////////////

CChbEntity::CChbEntity()
{
	m_aabb.Clear();
	m_pTree = NULL;
}

CChbEntity::~CChbEntity()
{
	Release();
}

//load chb file
bool CChbEntity::Load(const AString& strFile, bool bUseCollisionTree)
{
	Release();

	CConvexHullDataSet dataset;
	CHFSAVERESULT re;
	if (!dataset.Load(strFile, true, re))
		return false;
	for (int k = 0; k < dataset.GetModelNum(); k++ )
	{
		CHBMODEL* pModel = dataset.GetCHBModelByIndex(k);
		for (int i = 0; i < (int)pModel->m_pCHBrushes.size(); i++)
		{
			CConvexBrush* pConvexBrush = new CConvexBrush(*(pModel->m_pCHBrushes[i]));
			m_convexBrush.push_back(pConvexBrush);
		}
	}
	
	m_aabb = dataset.GetAABB();
	if (bUseCollisionTree)
	{
		m_pTree = new EntityCollisionTree;
		m_pTree->Build(m_convexBrush);
	}

	return true;
}

bool CChbEntity::Save(const AString& strFile)
{
	CConvexHullDataSet convexset;
	CHBMODEL model;
	strcpy(model.m_sName, "no name"); 
	for (int i = 0; i < (int)m_convexBrush.size(); i++)
	{
		model.m_pCHBrushes.push_back(new CConvexBrush(*m_convexBrush[i]));
	}
	convexset.AddModel(model);
	CHFSAVERESULT res;
	return convexset.Save(strFile, res);	
}

//capsule brush collision
bool CChbEntity::CapusuleTrace(CapusuleBrushTraceInfo * pInfo, bool bCheckFlag)
{
	if (m_pTree)
	{
		return m_pTree->Trace(pInfo, bCheckFlag);
	}
	bool		bStartSolid;	//	Collide something at start point
	bool		bAllSolid;		//	All in something
	CHalfSpace	ClipPlane;		//	Clip plane
	float		fFraction = 100;		//	Fraction
	for ( int i = 0; i < (int)m_convexBrush.size(); i++ )
	{
		//if (m_convexBrush[i]->CapusuleBrushTrace(pInfo, bCheckFlag) && pInfo->fFraction < fFraction)
		if (m_convexBrush[i]->BrushTrace(pInfo, bCheckFlag) && pInfo->fFraction < fFraction)
		{
			bStartSolid = pInfo->bStartSolid;
			bAllSolid = pInfo->bAllSolid;
			ClipPlane = pInfo->ClipPlane;
			fFraction = pInfo->fFraction;
			if (fFraction == 0)
			{
				if (bStartSolid)
					pInfo->dwUser2 = (DWORD)m_convexBrush[i];
				break;
			}
		}
	}
	if (fFraction < 100)
	{
		pInfo->bAllSolid = bAllSolid;
		pInfo->bStartSolid = bStartSolid;
		pInfo->ClipPlane = ClipPlane;
		pInfo->fFraction = fFraction;
		return true;
	}
	return false;
}

//AABB brush collision
bool CChbEntity::AABBTrace(AABBBrushTraceInfo2 * pInfo, bool bCheckFlag) const
{
	if (m_pTree)
	{
		return m_pTree->Trace(pInfo, bCheckFlag);
	}
	bool		bStartSolid;	//	Collide something at start point
	bool		bAllSolid;		//	All in something
	CHalfSpace	ClipPlane;		//	Clip plane
	float		fFraction = 100;		//	Fraction
	for ( int i = 0; i < (int)m_convexBrush.size(); i++ )
	{
		//if (m_convexBrush[i]->AABBBrushTrace(pInfo, bCheckFlag) && pInfo->fFraction < fFraction)
		if (m_convexBrush[i]->BrushTrace(pInfo, bCheckFlag) && pInfo->fFraction < fFraction)
		{
			bStartSolid = pInfo->bStartSolid;
			bAllSolid = pInfo->bAllSolid;
			ClipPlane = pInfo->ClipPlane;
			fFraction = pInfo->fFraction;
			if (fFraction == 0)
			{
				if (bStartSolid)
					pInfo->dwUser2 = (DWORD)m_convexBrush[i];
				break;
			}
		}
	}
	if (fFraction < 100)
	{
		pInfo->bAllSolid = bAllSolid;
		pInfo->bStartSolid = bStartSolid;
		pInfo->ClipPlane = ClipPlane;
		pInfo->fFraction = fFraction;
		return true;
	}
	return false;
}

//ray brush collision
bool CChbEntity::RayTrace(RayBrushTraceInfo* pInfo, bool bCheckFlag) const
{
	if (m_pTree)
	{
		return m_pTree->Trace(pInfo, bCheckFlag);
	}
	A3DAABB aabb(m_aabb);
	float& fScale = pInfo->fScale;
	aabb.Center = m_aabb.Center * fScale;
	aabb.Extents = m_aabb.Extents * fScale;
	aabb.CompleteMinsMaxs();
	
	
	if (!RayAABBCollision(pInfo, aabb))
		return false;

	bool		bStartSolid;	//	Collide something at start point
	bool		bAllSolid;		//	All in something
	CHalfSpace	ClipPlane;		//	Clip plane
	float		fFraction = 100;		//	Fraction
	for ( int i = 0; i < (int)m_convexBrush.size(); i++ )
	{
		//if (m_convexBrush[i]->RayBrushTrace(pInfo, bCheckFlag) && pInfo->fFraction < fFraction)
		if (m_convexBrush[i]->BrushTrace(pInfo, bCheckFlag) && pInfo->fFraction < fFraction)
		{
			bStartSolid = pInfo->bStartSolid;
			bAllSolid = pInfo->bAllSolid;
			ClipPlane = pInfo->ClipPlane;
			fFraction = pInfo->fFraction;
		}
	}
	if (fFraction < 100)
	{
		pInfo->bAllSolid = bAllSolid;
		pInfo->bStartSolid = bStartSolid;
		pInfo->ClipPlane = ClipPlane;
		pInfo->fFraction = fFraction;
		return true;
	}
	return false;
}

//release all the resource
void CChbEntity::Release()
{
	m_aabb.Clear();
	for (int i = 0; i < (int)m_convexBrush.size(); i++)
	{
		delete m_convexBrush[i];
	}
	m_convexBrush.clear();
	
	if (m_pTree)
	{
		delete m_pTree;
		m_pTree = NULL;
	}
	
}

//init from convex hull data	
void CChbEntity::Import(const abase::vector<CConvexHullData*>& pCHs)
{
	Release();
	CConvexBrush* pConvexBrush;
	for (int i = 0; i < (int)pCHs.size(); i++)
	{
		pConvexBrush = new CConvexBrush;
		pConvexBrush->Import(pCHs[i]);
		m_convexBrush.push_back(pConvexBrush);
		m_aabb.Merge(pCHs[i]->GetAABB());
	}
}

void CChbEntity::DebugRender(A3DFlatCollector* pFC, float fScale, DWORD dwFColor, const A3DVECTOR3& vRayDir, const A3DMATRIX4* pTransMatrix)
{
	for ( int i = 0; i < (int)m_convexBrush.size(); i++ )
	{
		m_convexBrush[i]->DebugRender(pFC, fScale, dwFColor, vRayDir, pTransMatrix);
	}
}

}
