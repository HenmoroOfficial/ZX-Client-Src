/********************************************************************
	created:	2008/04/17
	author:		kuiwu
	
	purpose:	convex hull tree
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

#include "BrushTree.h"
#include <AAssist.h>
#include <A3DCollision.h>
#include <AFile.h>


const float MIN_NODE_SIZE = 16.0f;
const int	MIN_BRUSH_COUNT = 20;
const float EXTENDS_Y = 5000.0f;

const DWORD BRUSH_TREE_FILE_MAGIC = (DWORD)(('b'<<24)| ('t'<<16)|('r'<<8)|('f'));
const DWORD BRUSH_TREE_FILE_VERSION = 1;

const DWORD CHDATA_FILE_MAGIC = (DWORD)(('c'<<24)| ('h'<<16)|('d'<<8)|('f'));
const DWORD CHDATA_FILE_VERSION = 1;


CQBrush ** linear_find(abase::vector<CQBrush*>& vec, CQBrush * target)
{
	abase::vector<CQBrush*>::iterator  it;
	for (it = vec.begin(); it != vec.end(); ++it)
	{
		if (*it == target)
		{
			break;
		}
	}

	return it;
}


CBrushTree::CBrushTree()
{
	m_pRootNode = NULL;
	m_nMinBrushCount = MIN_BRUSH_COUNT;
	m_fMinNodeSize = MIN_NODE_SIZE;
}

CBrushTree::~CBrushTree()
{
	Release();
}

void CBrushTree::Init(const ARectF& rect, float minNodeSize, int minBrushCount)
{
	SetMinNodeSize(minNodeSize);
	SetMinBrushCount(minBrushCount);

	m_pRootNode = new BrushNode;
	A3DVECTOR3 center,  ext;
	center.x  = rect.CenterPoint().x;
	center.y  = 0.0f;
	center.z  = rect.CenterPoint().y;
	ext.x =  rect.Width()*0.5f;
	ext.y =  EXTENDS_Y;
	ext.z =  rect.Height()*0.5f;
	m_pRootNode->aabb.Center = center;
	m_pRootNode->aabb.Extents = ext;
	m_pRootNode->aabb.CompleteMinsMaxs();
}

void CBrushTree::Release()
{
	if (m_pRootNode)
	{
		delete m_pRootNode;
		m_pRootNode = NULL;
	}
}

void CBrushTree::AddBrush(CQBrush * brush)
{
	if (!m_pRootNode)
	{
		return;
	}

	_AddBrush(m_pRootNode, brush);	
}

void CBrushTree::_AddBrush(BrushNode * node, CQBrush * brush)
{
	if (!_TestOverlap(node, brush))
	{
		return;
	}
	abase::vector<CQBrush*>::iterator it = linear_find(node->brushes, brush);
	if (it != node->brushes.end())
	{
		return;
	}

	node->brushes.push_back(brush);
	if (node->IsLeaf())
	{
		return;
	}
	int j;
	for (j = 0; j < 4; ++j)
	{
		_AddBrush(node->children[j], brush);
	}
}

void CBrushTree::DelAllBrushes()
{
	if (!m_pRootNode)
	{
		return;
	}
	_DelAllBrushes(m_pRootNode);
}

void CBrushTree::_DelAllBrushes(BrushNode * node)
{
	node->brushes.clear();
	if (node->IsLeaf())
	{
		return;
	}
	int j;
	for (j = 0; j < 4; ++j)
	{
		_DelAllBrushes(node->children[j]);
	}
}

void CBrushTree::DelBrush(CQBrush * brush)
{
	if (!m_pRootNode)
	{
		return;
	}

	_DelBrush(m_pRootNode, brush);
}

void CBrushTree::_DelBrush(BrushNode * node, CQBrush * brush)
{
	if (!_TestOverlap(node, brush))
	{
		return;
	}

	abase::vector<CQBrush*>::iterator it = linear_find(node->brushes, brush);
	if (it == node->brushes.end())
	{
		return;
	}

	node->brushes.erase(it);
	if (node->IsLeaf())
	{
		return;
	}

	int j;
	for (j = 0; j < 4; ++j)
	{
		_DelBrush(node->children[j], brush);
	}
}

void CBrushTree::SetMinBrushCount(int minBrushCount)
{
   m_nMinBrushCount= a_Max(minBrushCount, MIN_BRUSH_COUNT);
}

void CBrushTree::SetMinNodeSize(float minNodeSize)
{
	m_fMinNodeSize = a_Max(minNodeSize, MIN_NODE_SIZE);
}


void CBrushTree::Build(int rule)
{
	if (!m_pRootNode)
	{
		return;
	}
	
	_Build(m_pRootNode, rule);
}

void CBrushTree::_Build(BrushNode * node, int rule)
{

	//check brush
	int i;
	abase::vector<CQBrush *> pass_brushes;
	for (i = 0; i < (int)node->brushes.size(); ++i) 
	{
		if (_TestOverlap(node, node->brushes[i]))
		{
			pass_brushes.push_back(node->brushes[i]);
		}
	}
	node->brushes = pass_brushes;

	if (node->IsLeaf())
	{
		_Split(node, true, rule);
		return;
	}

	int j;
	for (j = 0; j < 4; ++j)
	{
		_Build(node->children[j], rule);
	}
}



//   ----------------------
//	 |          |         |
//   |     1    |     2   |
//   |          |         |
//   ----------------------
//	 |          |         |
//   |     0    |     3   |
//   |          |         |
//   ----------------------
void CBrushTree::_Split(BrushNode* node, bool recursive, int rule)
{
	assert(node->IsLeaf());
	if (!node->IsRoI())
	{
		return;
	}

	if (((rule & SPLIT_CHECK_NODE_SIZE) == SPLIT_CHECK_NODE_SIZE) &&  node->aabb.Extents.x * 2 < m_fMinNodeSize + 0.1f)
	{
		return;
	}

	if (((rule & SPLIT_CHECK_BRUSH_COUNT) == SPLIT_CHECK_BRUSH_COUNT) &&  node->brushes.size() <= m_nMinBrushCount)
	{
		return;
	}

	
	assert(node->children[0] == NULL);

	A3DVECTOR3 child_ext = node->aabb.Extents * 0.5f;
	child_ext.y = EXTENDS_Y;
	
	node->children[0] = new BrushNode;
	node->children[0]->aabb.Center.x   = node->aabb.Center.x - child_ext.x;
	node->children[0]->aabb.Center.y   = 0.0f;
	node->children[0]->aabb.Center.z   = node->aabb.Center.z - child_ext.z;
	node->children[0]->aabb.Extents = child_ext;
	node->children[0]->aabb.CompleteMinsMaxs();
	node->children[0]->RoI = node->RoI;

	node->children[1] = new BrushNode;
	node->children[1]->aabb.Center.x   = node->aabb.Center.x - child_ext.x;
	node->children[1]->aabb.Center.y   = 0.0f;
	node->children[1]->aabb.Center.z   = node->aabb.Center.z + child_ext.z;
	node->children[1]->aabb.Extents = child_ext;
	node->children[1]->aabb.CompleteMinsMaxs();
	node->children[1]->RoI = node->RoI;

	node->children[2] = new BrushNode;
	node->children[2]->aabb.Center.x   = node->aabb.Center.x + child_ext.x;
	node->children[2]->aabb.Center.y   = 0.0f;
	node->children[2]->aabb.Center.z   = node->aabb.Center.z + child_ext.z;
	node->children[2]->aabb.Extents = child_ext;
	node->children[2]->aabb.CompleteMinsMaxs();
	node->children[2]->RoI = node->RoI;

	node->children[3] = new BrushNode;
	node->children[3]->aabb.Center.x   = node->aabb.Center.x + child_ext.x;
	node->children[3]->aabb.Center.y   = 0.0f;
	node->children[3]->aabb.Center.z   = node->aabb.Center.z - child_ext.z;
	node->children[3]->aabb.Extents = child_ext;
	node->children[3]->aabb.CompleteMinsMaxs();
	node->children[3]->RoI = node->RoI;


	int i, j;
	for (i = 0; i < (int)node->brushes.size(); ++i)
	{
		CQBrush * brush = node->brushes[i];
		for (j = 0; j < 4; ++j)
		{
			BrushNode * child = node->children[j];
			if (_TestOverlap(child, brush))
			{
				child->brushes.push_back(brush);
			}
		}
	}

	if (recursive)
	{
		for (j = 0; j < 4; ++j)
		{
			BrushNode * child = node->children[j];
			_Split(child, recursive, rule);
		}
	}
}

bool CBrushTree::_TestOverlap(BrushNode * node, CQBrush * brush)
{
#ifdef _ANGELICA22
	return false;
#else
	if (!CLS_AABBAABBOverlap(node->aabb.Center, node->aabb.Extents, brush->GetCHData()->GetAABB().Center, brush->GetCHData()->GetAABB().Extents))
	{
		return false;
	}

	BrushTraceInfo trc_info;
	trc_info.Init(node->aabb.Center, A3DVECTOR3(0.0f), node->aabb.Extents);
	return brush->Trace(&trc_info);
#endif
}

void CBrushTree::Split(BrushNode* node)
{
	_Split(node, false, SPLIT_CHECK_NODE_SIZE|SPLIT_CHECK_BRUSH_COUNT);
}


void CBrushTree::Merge(BrushNode* node)
{
	assert(node->IsLeaf());

	BrushNode * parent = _GetParent(m_pRootNode, node);

	if (!parent)
	{
		return;
	}

	int j;

	for (j = 0; j < 4; ++j)
	{
		if (!parent->children[j]->IsLeaf())
		{
			return;
		}
	}



	//merge brushes
	parent->brushes.clear();

	for (j = 0; j < 4; ++j)
	{
		_MergeBrushes(parent, j);
	}

	for (j = 0; j < 4; ++j)
	{
		delete parent->children[j];
		parent->children[j] = NULL;
	}
}

void CBrushTree::_MergeBrushes(BrushNode * parent, int j)
{
	BrushNode * node = parent->children[j];
	assert(node->IsLeaf());
	int i, k;
	for (i = 0; i < (int)node->brushes.size(); ++i)
	{
		CQBrush * brush = node->brushes[i];
		bool bNewbee = true;
		for (k = 0; k < j; ++k)
		{
			BrushNode * brother = parent->children[k];
			if (_TestOverlap(brother, brush))
			{
				bNewbee = false;
				break;
			}
		}

		if (bNewbee)
		{
			parent->brushes.push_back(brush);
		}
	}
}

CBrushTree::BrushNode * CBrushTree::_GetParent(BrushNode * curNode, BrushNode * node)
{
	if (node == curNode || curNode->IsLeaf())
	{
		return NULL;
	}
	
	int j;
	for (j = 0; j < 4; ++j)
	{
		BrushNode * child = curNode->children[j];
		if (child == node)
		{
			return curNode;
		}
	}
	for (j = 0; j < 4; ++j)
	{
		BrushNode * parent = _GetParent(curNode->children[j], node);
		if (parent)
		{
			return parent;
		}
	}

	return NULL;
}

CBrushTree::BrushNode * CBrushTree::Pickup(const A3DVECTOR3& pos)
{
	if (!m_pRootNode)
	{
		return NULL;
	}
	return _Pickup(m_pRootNode, pos);	
}

CBrushTree::BrushNode * CBrushTree::_Pickup(BrushNode * node, const A3DVECTOR3& pos)
{
	if (node->IsLeaf() )
	{
		if (node->IsIn(pos.x, pos.z))
		{
			return node;
		}
		else
		{
			return NULL;
		}
	}

	int j;
	for (j = 0; j < 4; ++j)
	{
		BrushNode * child = node->children[j];

		BrushNode * pick = _Pickup(child, pos);
		if (pick != NULL)
		{
			return pick;
		}
	}

	return NULL;
}


void CBrushTree::RetrieveLeaves(abase::vector<BrushNode*>& leaves)
{

	_RetrieveLeaves(m_pRootNode, leaves);
}

void CBrushTree::_RetrieveLeaves(BrushNode * node,  abase::vector<BrushNode*>& leaves)
{
	if (node->IsLeaf())
	{
		leaves.push_back(node);
		return;
	}

	int j;
	for (j = 0; j < 4; ++j)
	{
		BrushNode * child = node->children[j];
		_RetrieveLeaves(child, leaves);		
	}

}


void CBrushTree::Export(const char * szName, bool exportCHD)
{
	if (!m_pRootNode)
	{
		return;
	}

	AFile aFile;
	if (!aFile.Open(szName, AFILE_CREATENEW | AFILE_BINARY | AFILE_NOHEAD))
	{
		return;
	}

	DWORD write_len;
	
	DWORD flag = BRUSH_TREE_FILE_MAGIC;
	aFile.Write(&flag, sizeof(DWORD), &write_len);
	flag = BRUSH_TREE_FILE_VERSION;
	aFile.Write(&flag, sizeof(DWORD), &write_len);


	//write brush
	int count = (int) m_pRootNode->brushes.size();
	aFile.Write(&count, sizeof(int), &write_len);
	int i;
	for (i = 0; i < count; ++i)
	{
		CQBrush * qBrush = m_pRootNode->brushes[i];
		CCDBrush cdBrush;
		qBrush->Export(&cdBrush);
		cdBrush.Save(&aFile);
	}

	//write treenode
	abase::vector<BrushNode *>  nodes;
	nodes.push_back(m_pRootNode);
	while (!nodes.empty())
	{
		abase::vector<BrushNode *> children;

		for (i =0; i < (int)nodes.size(); ++i)
		{
			BrushNode * cur_node = nodes[i];
			_ExportNode(aFile, cur_node);
			
			if (!cur_node->IsLeaf())
			{
				int j;
				for (j = 0; j < 4; ++j)
				{
					BrushNode * child = cur_node->children[j];
					children.push_back(child);	
				}
			}
		}
		nodes = children;
	}

	if (exportCHD)
	{
		//export ch data
		char path[256];
		strcpy(path, szName);
		char * pdest = strrchr(path, '.');
		if (pdest != NULL)
		{
			pdest++;
			*pdest = 'c';
			pdest++;
			*pdest = 'h';
			pdest++;
			*pdest = 'd';
			pdest++;
			*pdest = '\0';
		}
		else
		{
			strcat(path, ".chd");
		}

		_ExportCHData(path);
	}


}


void CBrushTree::Import(const char * szName)
{
	if (!m_pRootNode)
	{
		return;
	}

	AFile aFile;
	if (!aFile.Open(szName, AFILE_OPENEXIST | AFILE_BINARY | AFILE_NOHEAD))
	{
		return;
	}

	DWORD read_len;
	
	DWORD flag;
	aFile.Read(&flag, sizeof(DWORD), &read_len);
	if (flag != BRUSH_TREE_FILE_MAGIC)
	{
		return;
	}
	aFile.Read(&flag, sizeof(DWORD), &read_len);
	if (flag != BRUSH_TREE_FILE_VERSION)
	{
		return;
	}

	//read brush
	int count;
	aFile.Read(&count, sizeof(int), &read_len);
	int i;
	for (i = 0; i < count; ++i)
	{
		CCDBrush cdBrush;
		cdBrush.Load(&aFile);
	}

	//read treenode
	abase::vector<BrushNode *>  nodes;
	nodes.push_back(m_pRootNode);
	while (!nodes.empty())
	{
		abase::vector<BrushNode *> children;

		for (i =0; i < (int)nodes.size(); ++i)
		{
			BrushNode * cur_node = nodes[i];
			bool is_leaf;
			_ImportNode(aFile, cur_node, is_leaf);
			
			if (!is_leaf)
			{
				int j;
				
				if (cur_node->IsLeaf())
				{
					for (j = 0; j < 4; ++j)
					{
						cur_node->children[j] = new BrushNode;
					}
				}


				for (j = 0; j < 4; ++j)
				{
					BrushNode * child = cur_node->children[j];
					children.push_back(child);	
				}
			}
		}
		nodes = children;
	}

}

void CBrushTree::_ExportNode(AFile& aFile, BrushNode * node)
{
	DWORD write_len;
	aFile.Write(&node->aabb.Center.x, sizeof(float), &write_len);
	aFile.Write(&node->aabb.Center.z, sizeof(float), &write_len);
	aFile.Write(&node->aabb.Extents.x, sizeof(float), &write_len);
	aFile.Write(&node->aabb.Extents.z, sizeof(float), &write_len);
	bool b;
	b = node->IsRoI();
	aFile.Write(&b, sizeof(bool), &write_len);
	b = node->IsLeaf();
	aFile.Write(&b, sizeof(bool), &write_len);
}

void CBrushTree::_ImportNode(AFile& aFile, BrushNode * node, bool& isLeaf)
{
	DWORD read_len;
	aFile.Read(&node->aabb.Center.x, sizeof(float), &read_len);
	aFile.Read(&node->aabb.Center.z, sizeof(float), &read_len);
	aFile.Read(&node->aabb.Extents.x, sizeof(float), &read_len);
	aFile.Read(&node->aabb.Extents.z, sizeof(float), &read_len);

	node->aabb.Center.y = 0;
	node->aabb.Extents.y = EXTENDS_Y;
	node->aabb.CompleteMinsMaxs();
	

	bool b;
	aFile.Read(&b, sizeof(bool), &read_len);
	node->RoI = b;
	aFile.Read(&b, sizeof(bool), &read_len);
	isLeaf = b;

	if (!node->IsRoI() && !node->IsLeaf())
	{
		assert(isLeaf);
		//remove child
		int j;
		for (j = 0; j < 4; ++j)
		{
			BrushNode * child = node->children[j];
			delete node->children[j];
			node->children[j] = NULL;
		}
	}
}


void CBrushTree::_RetrieveNodes(BrushNode * curNode, int curLevel, int level, abase::vector<BrushNode*>& nodes)
{
	if (curLevel == level)
	{
		nodes.push_back(curNode);
		return;
	}
	else if (curLevel > level)
	{
		return;
	}

	if (curNode->IsLeaf())
	{
		return;
	}

	int j;
	for (j = 0; j < 4; ++j)
	{
		BrushNode * child = curNode->children[j];
		_RetrieveNodes(child, curLevel+1, level, nodes);
	}
}

void CBrushTree::_ExportCHData(const char * szName)
{
	AFile aFile;
	if (!aFile.Open(szName, AFILE_CREATENEW | AFILE_BINARY))
	{
		return;
	}

	DWORD write_len;
	
	DWORD flag = CHDATA_FILE_MAGIC;
	aFile.Write(&flag, sizeof(DWORD), &write_len);
	flag = CHDATA_FILE_VERSION;
	aFile.Write(&flag, sizeof(DWORD), &write_len);


	//write ch data
	int count = (int) m_pRootNode->brushes.size();
	aFile.Write(&count, sizeof(int), &write_len);
	int i;
	for (i = 0; i < count; ++i)
	{
		CQBrush * qBrush = m_pRootNode->brushes[i];
		qBrush->GetCHData()->SaveBinaryData(&aFile);
	}

}


CBrushTree::BrushNode * CBrushTree::Check(AString& failReason)
{
	if (!m_pRootNode)
	{
		return NULL;
	}
	return _Check(m_pRootNode, failReason);
}

CBrushTree::BrushNode * CBrushTree::_Check(BrushNode * curNode, AString& failReason)
{
	if (!curNode->IsRoI() && !curNode->IsLeaf())
	{
		failReason.Format("node center %f %f %f size %f non-RoI but not leaf!\r\nPlease set RoI or merge.", 
				curNode->aabb.Center.x, curNode->aabb.Center.y, curNode->aabb.Center.z, 
				curNode->aabb.Extents.x);
		return curNode;
	}

	if (curNode->IsLeaf())
	{
		if (curNode->IsRoI() && (int)curNode->brushes.size() > m_nMinBrushCount &&  curNode->aabb.Extents.x * 2 > m_fMinNodeSize + 0.1f)
		{
			failReason.Format("leaf node center %f %f %f size %f convexhull %d, expected(size < %f or hull <%d)!\r\nPlease set non-RoI or split.", 
				curNode->aabb.Center.x, curNode->aabb.Center.y, curNode->aabb.Center.z, 
				curNode->aabb.Extents.x, curNode->brushes.size(), m_fMinNodeSize, m_nMinBrushCount);
			return curNode;
		}
	}
	else
	{
		int j;
		for (j =0; j < 4; ++j)
		{
			BrushNode * fail_node  = _Check(curNode->children[j], failReason);
			if (fail_node)
			{
				return fail_node;
			}
		}
	}

	return NULL;
}