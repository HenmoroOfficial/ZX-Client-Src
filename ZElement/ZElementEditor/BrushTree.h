/********************************************************************
	created:	2008/04/17
	author:		kuiwu
	
	purpose:	convex hull tree
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

#include <A3DGeometry.h>
#include <vector.h>
#include <ARect.h>
#include "aabbcd.h"


using namespace CHBasedCD;

class AFile;
class AString;

class CBrushTree
{
public:
	enum
	{
		SPLIT_CHECK_NODE_SIZE = 0x1,
		SPLIT_CHECK_BRUSH_COUNT = 0x2,
	};
public:
	struct  BrushNode
	{
		A3DAABB    aabb;
		BrushNode *   children[4];
		abase::vector<CQBrush*>   brushes;
		bool					  RoI;

		BrushNode()
		{
			children[0]  = NULL;
			brushes.clear();
			RoI = true;
		}
		~BrushNode()
		{
			if (children[0])
			{
				for (int i = 0; i < 4; ++i)
				{
					delete children[i];
					children[i] = NULL;
				}
			}
			brushes.clear();
		}
		bool IsLeaf() const
		{
			return (children[0] == NULL);
		}
		bool IsRoI() const
		{
			return RoI;
		}
		bool IsIn(float x, float z)
		{
			A3DVECTOR3  pos(x, aabb.Center.y, z);
			return aabb.IsPointIn(pos);
		}

	};
public:
	CBrushTree();
	~CBrushTree();
	void Init(const ARectF& rect, float minNodeSize, int minBrushCount);

	void AddBrush(CQBrush * brush);
	void DelBrush(CQBrush * brush);
	void DelAllBrushes();
	
	void Build(int rule);
	void Release();
	void Split(BrushNode* node);
	void Merge(BrushNode* node);

	BrushNode * Pickup(const A3DVECTOR3& pos);
	void SetMinNodeSize(float minNodeSize);
	void SetMinBrushCount(int minBrushCount);

	void RetrieveLeaves(abase::vector<BrushNode*>& leaves);

	BrushNode * Check(AString& failReason);
	
	void Export(const char * szName, bool exportCHD = false);
	void Import(const char * szName);
private:
	void _Split(BrushNode* node,  bool recursive, int rule);
	bool _TestOverlap(BrushNode * node, CQBrush * brush);
	BrushNode * _Pickup(BrushNode * node, const A3DVECTOR3& pos);
	BrushNode * _GetParent(BrushNode * curNode, BrushNode * node);
	void _MergeBrushes(BrushNode * parent, int j);
	void _RetrieveLeaves(BrushNode * node, abase::vector<BrushNode*>& leaves);
	void _RetrieveNodes(BrushNode * curNode, int curLevel, int level, abase::vector<BrushNode*>& nodes);
	void _ExportNode(AFile& aFile, BrushNode * node);
	void _ImportNode(AFile& aFile, BrushNode * node, bool& isLeaf);
	void _ExportCHData(const char * szName);
	
	void _Build(BrushNode * node, int rule);
	void _AddBrush(BrushNode * node, CQBrush * brush);
	void _DelBrush(BrushNode * node, CQBrush * brush);
	void _DelAllBrushes(BrushNode * node);
	BrushNode * _Check(BrushNode * curNode, AString& failReason);
private:
	BrushNode				*  m_pRootNode;

	float					   m_fMinNodeSize;
	int						   m_nMinBrushCount;
};