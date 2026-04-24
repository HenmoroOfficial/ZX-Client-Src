/********************************************************************
	created:	2006/10/08
	author:		kuiwu
	
	purpose:	
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/

#ifndef _MOVE_AGENT_H_
#define _MOVE_AGENT_H_



#include <vector.h>
using namespace abase;
#include <A3DTypes.h>

#include "PfConstant.h"

namespace AutoMove
{


class CMoveMap;
class COptimizePath;
class CPfClu;

class CMoveAgent  
{
public:
	CMoveAgent();
	~CMoveAgent();
	bool IsReady() const;
	bool Load(const char * szPath);
	void Release();
	CMoveMap * GetMoveMap() const
	{
		return m_pMoveMap;
	}
	bool SetStartEnd(const A3DPOINT2& ptStart, int iLayerStart, const A3DPOINT2& ptGoal, int iLayerGoal);
	int  WhichLayer(const A3DPOINT2& ptMap, float dH);
	int  GetMinPassableLayer(const A3DPOINT2& ptMap) const;
	int  GetAllPassableLayers(const A3DPOINT2& ptMap,int* pLayers = NULL)const;
	A3DPOINT2 TransWld2Map(const A3DVECTOR3& vWld);

	bool Search(vector<PathNode> * pPath = NULL);
	void CreateOptimizer();
	vector<PathNode> * Get2DPath() const;
	/**
	 * \brief retrieve the 3d path
	 * \param[in]
	 * \param[out]
	 * \return
	 * \note  the path node's y is the delta height from the terrain
	 * \warning
	 * \todo   
	 * \author kuiwu 
	 * \date 6/6/2008
	 * \see 
	 */
	void Retrieve3DPath(vector<A3DVECTOR3>& path, int from = 0) const;
	int  GetPathCount() const;
	A3DVECTOR3 Get3DPathNode(const int index) const;

	/**
	 * \brief  optimize the path if necessary
	 * \param[in] moveIndex,  the current move index in the global path
	 * \param[in,out] remain3DPath, the remaining 3d path(excluding  moveIndex)after optimization if need optimize and the input param is not null.
	 * \return  true if need optimize
	 * \note   the 2d path will be updated if need optimize
	 * \warning
	 * \todo   
	 * \author kuiwu 
	 * \date 21/3/2007
	 * \see 
	 */
	bool Optimize(const int moveIndex, vector<A3DVECTOR3>* remain3DPath);
	int  GetState() const
	{
		return m_iStat;
	}
	void SetPfAlg(int alg);
	int GetPfAlg() const
	{
		return m_iPfAlg;
	}

	int  GetOptimizeCatchCount() const;
	
private:
	bool          _CheckStart();
	bool          _CheckEnd();
	bool          _CheckThrough();
private:
	CMoveMap   * m_pMoveMap;
	CPfClu     * m_pPfAlg;
	int          m_iStat;
	COptimizePath * m_pPathOptimizer;
	int			 m_iPfAlg;
	A3DPOINT2    m_ptStart;
	int          m_iLayerStart;
	A3DPOINT2    m_ptGoal;
	int          m_iLayerGoal;
};

}

//for test
extern AutoMove::CMoveAgent  g_AutoMoveAgent;

#endif