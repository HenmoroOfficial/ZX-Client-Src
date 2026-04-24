 /*
 * FILE: PassMapGenerator.cpp
 *
 * DESCRIPTION:  A class which generates the pass map of NPCs
 *							The pass map will be loaded in the server for NPCs' moving strategy
 *
 * CREATED BY: He wenfeng, 2005/3/25
 *
 * HISTORY:  make it suitable to generate player pass map   [10/12/2006 kuiwu]
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "StdAfx.h"
#include "PlayerPassMapGenerator.h"
#include "Global.h"
#include "Terrain.h"
#include "MainFrm.h"
#include "SceneObjectManager.h"
#include "TerrainWater.h"
#include <queue>
#include <stdlib.h>

#include "EL_Building.h"
#include "BlockImage.h"
#include "BitImage.h"
#include "CDWithCH.h"
#include "aabbcd.h"
using namespace CHBasedCD;
using namespace std;


#define FLOATTOINT(x) ((int) floor((x)+0.5f))						// 四舍五入



#define PASS_MAP(u,v)  m_arrPassMap[(v)*m_iWidth+u]
#define HEIGHT_MAP(u,v)  m_arrHeightMap[(v)*m_iWidth+u]
#define SURFACE_MAP(u,v)  m_arrSurface[(v)*m_iWidth +u]
#define VALID_POS_IN_MAP(u,v) (u >= 0 && u < m_iWidth && v >= 0 && v < m_iLength)

#define RAISE_STEP 2.0f

#define PASSMAP_VIRGIN 0
#define PASSMAP_REACHABLE 1
#define PASSMAP_UNREACHABLE 2

// 45 degree slope thresh!
#define SLOPE_THRESH 0.707f	
#define HEIGHT_STEP_THRESH 1.0f 

#define SEEDPOS_SLOPE_Y_THRESH 0.866f		// 种子点的斜率阈值，坡度不超过30 degree

#define  EXT_XZ    0.35f
#define  EXT_Y     0.8f

//////////////////////////////////////////////////////////////////////
// Some structs and data types
//////////////////////////////////////////////////////////////////////

int NeighborD[] =
{
		-1,  0,   //left
		+1,  0,   //right
		0,   +1,  //top
		0,   -1,  //bottom
		+1,  +1,  //topright
		+1,  -1,  //bottomright
		-1,  +1,  //topleft
		-1,  -1   //bottomleft
};


//////////////////////////////////////////////////////////////////////
// Some global functions which depend on this project!
//////////////////////////////////////////////////////////////////////

// CD with the static model
static bool SphereCollideWithEnv(
						  const A3DVECTOR3& vCenter, 
						  const float fRadius,
						  const A3DVECTOR3& vDelta,
						  A3DVECTOR3& vFinalPos,	/* vFinalPos is not the hit pos, but the new Centor of Sphere after movement!*/
						  A3DVECTOR3& vNormal,
						  float& fraction,
						  CConvexHullData* & pCDCHData
						  )
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	ASSERT(pMap);
	CSceneObjectManager *pSOMan =  pMap->GetSceneObjectMan();
	A3DVECTOR3 vHitPos;
	if(pSOMan->SphereTraceStaticModel(vCenter,fRadius,vDelta,vHitPos,&fraction,vNormal,pCDCHData))
	{
		vFinalPos=vHitPos+fRadius*vNormal;
		return true;
	}
	else
		return false;
}

// RT with the static model
static bool RayTraceEnv(
						  const A3DVECTOR3& vStart, 
						  const A3DVECTOR3& vDelta,
						  A3DVECTOR3& vHitPos,
						  A3DVECTOR3& vNormal,
						  float& fraction,
						  CConvexHullData* & pTraceCHData
						  )
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	ASSERT(pMap);
	CSceneObjectManager *pSOMan =  pMap->GetSceneObjectMan();
	return pSOMan->RayTraceStaticModel(vStart,vDelta,vHitPos,&fraction,vNormal, pTraceCHData);
}

static bool VertRayTraceEnv(const A3DVECTOR3& vPos, A3DVECTOR3& vHitPos, A3DVECTOR3& vHitNormal, float DeltaY, CConvexHullData* & pTraceCHData)
{
	float fraction;
	return RayTraceEnv(vPos,-DeltaY*g_vAxisY,vHitPos,vHitNormal,fraction, pTraceCHData);
}

static inline void GetTerrainInfo(const A3DVECTOR3& vPos, A3DVECTOR3& vPosOnSurface, A3DVECTOR3& vNormal)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	ASSERT(pMap);
	CTerrain *pTerrain =  pMap->GetTerrain();
	vPosOnSurface=vPos;
	vPosOnSurface.y=pTerrain->GetPosHeight(vPos);
	pTerrain->GetPosNormal(vPos,vNormal);
}

static inline bool IsUnderWater(const A3DVECTOR3& vecPos)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	ASSERT(pMap);
#ifdef _ANGELICA22
	A3DTerrainWater2 * pWater=pMap->GetTerrainWater()->GetA3DTerrainWater();
#else
	A3DTerrainWater * pWater=pMap->GetTerrainWater()->GetA3DTerrainWater();
#endif // #ifdef _ANGELICA22
	return pWater->IsUnderWater(vecPos);
}


static bool AABBCollideWithEnv(BrushTraceInfo * pInfo )
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	ASSERT(pMap);
	CSceneObjectManager *pSOMan =  pMap->GetSceneObjectMan();
	return pSOMan->AABBTraceStaticModel(pInfo);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayerPassMapGenerator::CPlayerPassMapGenerator()
{
	m_arrPassMap=NULL;
	m_arrHeightMap=NULL;
	m_arrSurface = NULL;
	// initial values
	m_fSampleStep = 1;
	m_iWidth =0;
	m_iLength =0;
	m_CalScale = 20;



	
}

CPlayerPassMapGenerator::~CPlayerPassMapGenerator()
{
	Release();

}

void CPlayerPassMapGenerator::Release()
{
	if(m_arrPassMap) 
	{
		delete [] m_arrPassMap;
		m_arrPassMap=NULL;
	}

	if(m_arrHeightMap)
	{
		delete [] m_arrHeightMap;
		m_arrHeightMap=NULL;
	}

	if (m_arrSurface)
	{
		delete[] m_arrSurface;
		m_arrSurface = NULL;
	}
}

bool CPlayerPassMapGenerator::IsPosReachable(const PLAYER_SAMPLE_POS& sPos)
{
	return IsPosReachable(sPos.u, sPos.v);
}

bool CPlayerPassMapGenerator::IsPosReachable(const A3DVECTOR3 & vPos)
{
	PLAYER_SAMPLE_POS sPos;
	Transf3DTo2D(vPos, sPos);
	return IsPosReachable(sPos);
}

float  CPlayerPassMapGenerator::GetPosDeltaHeight(const A3DVECTOR3 & vPos)
{
	PLAYER_SAMPLE_POS sPos;
	Transf3DTo2D(vPos, sPos);
	return GetPosDeltaHeight(sPos);
}



void CPlayerPassMapGenerator::SetSampleInfo(float fss, int iTWidth, int iTLength)
{
	Release();
	m_fSampleStep=fss;
	m_iWidth=FLOATTOINT(iTWidth/fss);
	m_iLength=FLOATTOINT(iTLength/fss);
	int iSize=m_iWidth*m_iLength;

	// 分配空间，按照行优先（u优先）存储
	m_arrPassMap=new UCHAR [iSize];
	m_arrHeightMap=new FIX16 [iSize];

	// 初始化区域 对于m_arrPassMap而言，0表示未计算，1表示可达，2表示不可达
	memset(m_arrPassMap, PASSMAP_VIRGIN, iSize*sizeof(UCHAR));
	memset(m_arrHeightMap, 0, iSize*sizeof(FIX16));

	m_arrSurface = new Surface_Info [iSize];
	for (int i = 0; i < iSize; ++i)
	{
		m_arrSurface[i].h = -1.0f;
		m_arrSurface[i].n.Clear();
	}
	m_CalScale = 1;
}

bool CPlayerPassMapGenerator::_GetSurfaceInfo(PLAYER_SAMPLE_POS& sPos, A3DVECTOR3& normal)
{
	static A3DVECTOR3  vExt(EXT_XZ, EXT_Y, EXT_XZ);

	if (SURFACE_MAP(sPos.u, sPos.v).h >= 0.0f)
	{
		sPos.h = SURFACE_MAP(sPos.u, sPos.v).h;
		normal = SURFACE_MAP(sPos.u, sPos.v).n;
		return true;
	}

	//undetermined yet
	A3DVECTOR3 vTestPos;
	Transf2DTo3D(sPos, vTestPos);
	vTestPos.y += (vExt.y + HEIGHT_STEP_THRESH + 0.2f); // 做一个抬高，从而便于向下RayTrace
	A3DVECTOR3 vSurfacePos;
	GetTerrainInfo(vTestPos, vSurfacePos, normal);
	BrushTraceInfo info;
	float dH = 0.0f;

	// 竖直向下做RayTrace
	info.Init(vTestPos, A3DVECTOR3(0.0f, -100.0f, 0.0f), vExt);
	if (AABBCollideWithEnv(&info))
	{
		if (info.fFraction < 1E-5f)
		{//终点不合法
			return false;
		}
		A3DVECTOR3 vHitPos(info.vStart + info.vDelta * info.fFraction);
		vHitPos.y -= vExt.y;
		if (vHitPos.y > vSurfacePos.y)
		{
			dH = vHitPos.y - vSurfacePos.y;
			vSurfacePos.y = vHitPos.y;
			normal = info.ClipPlane.GetNormal();

		}
	}
	sPos.h = vSurfacePos.y;
	SURFACE_MAP(sPos.u, sPos.v).h = vSurfacePos.y;
	SURFACE_MAP(sPos.u, sPos.v).n = normal;
	if(fabs(dH) < 0.001f)
			HEIGHT_MAP(sPos.u,sPos.v)=0;
	else
			HEIGHT_MAP(sPos.u,sPos.v)=FLOATTOFIX16(dH);

	
	return true;
}

int CPlayerPassMapGenerator::_Check(const PLAYER_SAMPLE_POS& sPos, int neiU, int neiV, int& neiStat)
{
	assert(PASS_MAP(sPos.u, sPos.v) == PASSMAP_VIRGIN);
	static A3DVECTOR3  vExt(EXT_XZ, EXT_Y, EXT_XZ);

	PLAYER_SAMPLE_POS sNeiPos;
	sNeiPos.u = neiU;
	sNeiPos.v = neiV;
	sNeiPos.h = sPos.h;
	A3DVECTOR3 neiNormal;
	//取neighbor的surface info 失败
	//高度差太大，坡度大
	//邻近于一个陡坡
	if (!_GetSurfaceInfo(sNeiPos, neiNormal)
		|| fabs(sNeiPos.h - sPos.h) >HEIGHT_STEP_THRESH * m_fSampleStep
		|| neiNormal.y < SLOPE_THRESH)
	{
		neiStat = PASSMAP_UNREACHABLE;
		return PASSMAP_UNREACHABLE;
	}

	// 开始进行碰撞检测
	BrushTraceInfo info;
	//neighbor -> pos
	A3DVECTOR3 vPos, vNeiPos, vDelta;
	Transf2DTo3D(sPos, vPos);
	Transf2DTo3D(sNeiPos, vNeiPos);
	vPos.y += (vExt.y + 0.01f);
	vNeiPos.y += (vExt.y + 0.01f);
	vDelta = vPos - vNeiPos;
	info.Init(vNeiPos, vDelta, vExt);
	
	if(!AABBCollideWithEnv(&info))
	{
		//keep neiStat unchanged
		return PASSMAP_REACHABLE;  // can from this neighbor
	}

	//step up
	vNeiPos.y += m_fPFStepHeight;
	//vDelta.y = 0.0f;
	info.Init(vNeiPos, vDelta, vExt);
	if (!AABBCollideWithEnv(&info))
	{
		//keep neiStat unchanged
		return PASSMAP_REACHABLE;  // can from this neighbor
	}	
	neiStat   = PASSMAP_UNREACHABLE;
	return   PASSMAP_UNREACHABLE;

}


void CPlayerPassMapGenerator::Transf2DTo3D(const PLAYER_SAMPLE_POS& sPos, A3DVECTOR3& vPos)
{
	vPos.y = sPos.h;
	vPos.x = (sPos.u + 0.5f - m_iWidth * 0.5f ) * m_fSampleStep;
	vPos.z = (sPos.v + 0.5f -m_iLength * 0.5f ) * m_fSampleStep;
}

void CPlayerPassMapGenerator::Transf3DTo2D(const A3DVECTOR3& vPos, PLAYER_SAMPLE_POS& sPos)
{
	sPos.h = vPos.y;
	float fssRecip = 1.0f/m_fSampleStep;
	sPos.u = FLOATTOINT( vPos.x * fssRecip + m_iWidth * 0.5f - 0.5f );
	sPos.v = FLOATTOINT( vPos.z * fssRecip + m_iLength * 0.5f - 0.5f );
}

void CPlayerPassMapGenerator::ExpandPassRegion2(const PLAYER_SAMPLE_POS& posSeed)
{
	ExpandPassRegion(posSeed);
}

void CPlayerPassMapGenerator::ExpandPassRegion(const PLAYER_SAMPLE_POS& posSeed)
{

	// 越界
	if(!VALID_POS_IN_MAP(posSeed.u, posSeed.v)) return;

	// 已计算过
	if(PASS_MAP(posSeed.u, posSeed.v)!=PASSMAP_VIRGIN) return;

	
	
	PLAYER_SAMPLE_POS curPos;
	curPos = posSeed;
	A3DVECTOR3 vNormal;
	
	queue<PLAYER_SAMPLE_POS> quePos;
	quePos.push(curPos);
	
	int maxCount;
	maxCount = (int) ((m_iWidth/m_fSampleStep +1) * (m_iLength/m_fSampleStep+1) * m_CalScale * 0.01f);
	//maxCount = 200;
	int counter=0;
	while(!quePos.empty()  && counter <=maxCount )
	{
		// 弹出
		curPos=quePos.front();
		quePos.pop();
		//assert(PASS_MAP(curPos.u, curPos.v) == PASSMAP_VIRGIN);
		if (PASS_MAP(curPos.u, curPos.v) != PASSMAP_VIRGIN)
		{
			continue;
		}
		if (!_GetSurfaceInfo(curPos,vNormal ) 
			|| vNormal.y < SLOPE_THRESH)
		{
			PASS_MAP(curPos.u, curPos.v) = PASSMAP_UNREACHABLE;
			counter++;
			continue;
		}

		int curStat, neiStat;
		curStat = PASS_MAP(curPos.u, curPos.v);
		int i;
		for (i = 0; i < 8; i++)
		{
			int neiU, neiV;
			neiU = curPos.u + NeighborD[i*2];
			neiV = curPos.v + NeighborD[i*2+1];

			if (!VALID_POS_IN_MAP(neiU, neiV))
			{
				continue;
			}
			neiStat = PASS_MAP(neiU, neiV);
			int newNeiStat = neiStat;
			curStat = _Check(curPos, neiU, neiV, newNeiStat);
			if (neiStat != newNeiStat)
			{
				PASS_MAP(neiU, neiV) = newNeiStat;
				counter++;
			}
			if (curStat == PASSMAP_UNREACHABLE)
			{
				break;
			}
		}
		
		if (curStat != PASSMAP_VIRGIN)
		{
			PASS_MAP(curPos.u, curPos.v) = curStat;
			counter++;
		}

		//only expand reachable		
		if (curStat == PASSMAP_REACHABLE)
		{
			for (i = 0; i < 8; i++)
			{
				int neiU, neiV;
				neiU = curPos.u + NeighborD[i*2];
				neiV = curPos.v + NeighborD[i*2+1];

				if (!VALID_POS_IN_MAP(neiU, neiV) || (PASS_MAP(neiU, neiV) != PASSMAP_VIRGIN))
				{
					continue;
				}

				PLAYER_SAMPLE_POS sNeiPos;
				sNeiPos.u = neiU;
				sNeiPos.v = neiV;
				sNeiPos.h = curPos.h; 
				quePos.push(sNeiPos);
			}
		}
	}

}


void CPlayerPassMapGenerator::ExpandPassRegion1(const PLAYER_SAMPLE_POS& posSeed)
{

	// 越界
	if(!VALID_POS_IN_MAP(posSeed.u, posSeed.v)) return;

	// 已计算过
	if(PASS_MAP(posSeed.u, posSeed.v)!=PASSMAP_VIRGIN) return;

	A3DVECTOR3 vCurPos,vTerrainPos, vTerrainNormal;
	float fDeltaH;	
	PLAYER_SAMPLE_POS curPos;
	curPos = posSeed;
	BrushTraceInfo info;
	Transf2DTo3D(posSeed,vCurPos);
	vCurPos.y += RAISE_STEP;
	info.Init(vCurPos, A3DVECTOR3(0.0f, -10.0f, 0.0f), A3DVECTOR3(EXT_XZ, EXT_Y, EXT_XZ));	
	GetTerrainInfo(vCurPos, vTerrainPos, vTerrainNormal);
	curPos.h = vTerrainPos.y;
	if (AABBCollideWithEnv(&info))
	{
		A3DVECTOR3 vHit(info.vStart + info.vDelta * info.fFraction);
		if (vHit.y - info.vExtents.y > vTerrainPos.y)
		{
			curPos.h = vHit.y - info.vExtents.y;
		}
	}

	queue<PLAYER_SAMPLE_POS> quePos;
	quePos.push(curPos);
	
	int maxCount;
	maxCount = (int) ((m_iWidth/m_fSampleStep +1) * (m_iLength/m_fSampleStep+1) * m_CalScale * 0.01f);
	int counter=0;
	//while(!quePos.empty() ) // && counter <=30000 )
	while(!quePos.empty()  && counter <=maxCount )
	{

		// 弹出
		curPos=quePos.front();
		quePos.pop();
		if(PASS_MAP(curPos.u,curPos.v)==PASSMAP_REACHABLE)		//说明已经计算！
			continue;

		PASS_MAP(curPos.u,curPos.v)=PASSMAP_REACHABLE;
		
		Transf2DTo3D(curPos,vCurPos);
		GetTerrainInfo(vCurPos, vTerrainPos, vTerrainNormal);
		fDeltaH = curPos.h - vTerrainPos.y;
		if(fabs(fDeltaH) < 0.001f)
			HEIGHT_MAP(curPos.u,curPos.v)=0;
		else
			HEIGHT_MAP(curPos.u,curPos.v)=FLOATTOFIX16(fDeltaH);
		
		// for debug
		counter++;
		
		// 依次遍历4邻域
		PLAYER_SAMPLE_POS nbPos=curPos;
		// 右邻域
		nbPos.u+=1;
		if(VALID_POS_IN_MAP(nbPos.u,nbPos.v)			//合法
			&& PASS_MAP(nbPos.u,nbPos.v)==PASSMAP_VIRGIN	)			// 未计算
			
		{
			if( CanPFMove(curPos,nbPos) )
				quePos.push(nbPos);
			else
				PASS_MAP(nbPos.u,nbPos.v)=PASSMAP_UNREACHABLE;
		}

		// 左邻域
		nbPos=curPos;
		nbPos.u-=1;
		if(VALID_POS_IN_MAP(nbPos.u,nbPos.v)			//合法
			&& PASS_MAP(nbPos.u,nbPos.v)==PASSMAP_VIRGIN	)			// 未计算
			
		{
			if( CanPFMove(curPos,nbPos) )
				quePos.push(nbPos);
			else
				PASS_MAP(nbPos.u,nbPos.v)=PASSMAP_UNREACHABLE;
		}
		
		// 上邻域
		nbPos=curPos;
		nbPos.v+=1;
		if(VALID_POS_IN_MAP(nbPos.u,nbPos.v)			//合法
			&& PASS_MAP(nbPos.u,nbPos.v)==PASSMAP_VIRGIN	)			// 未计算
			
		{
			if( CanPFMove(curPos,nbPos) )
				quePos.push(nbPos);
			else
				PASS_MAP(nbPos.u,nbPos.v)=PASSMAP_UNREACHABLE;
		}
		
		// 下邻域
		nbPos=curPos;
		nbPos.v-=1;
		if(VALID_POS_IN_MAP(nbPos.u,nbPos.v)			//合法
			&& PASS_MAP(nbPos.u,nbPos.v)==PASSMAP_VIRGIN	)			// 未计算
			
		{
			if( CanPFMove(curPos,nbPos) )
				quePos.push(nbPos);
			else
				PASS_MAP(nbPos.u,nbPos.v)=PASSMAP_UNREACHABLE;
		}

	}

}

bool CPlayerPassMapGenerator::CanPFMove(const PLAYER_SAMPLE_POS& posFrom, PLAYER_SAMPLE_POS& posTo)
{
#define  LOCAL_EPSILON   0.01f
	A3DVECTOR3 vPFCenter,vFinalPos, vExt(EXT_XZ, EXT_Y, EXT_XZ); 
	Transf2DTo3D(posFrom, vPFCenter);
	vPFCenter.y=posFrom.h + vExt.y +LOCAL_EPSILON;
	
	A3DVECTOR3 vDelta((posTo.u-posFrom.u)*m_fSampleStep,0.0f,(posTo.v-posFrom.v)*m_fSampleStep);
	A3DVECTOR3 vTerrainPos,vTerrainNormal;
	
	// 测试是否再水下，目前不能下水！
	vFinalPos = vPFCenter + vDelta;
	GetTerrainInfo(vFinalPos, vTerrainPos, vTerrainNormal);
	if(IsUnderWater(vTerrainPos))			
		return false;
	

	BrushTraceInfo info;

	// 对vFinalPos竖直向下做RayTrace
	vFinalPos.y+= 1.2f;						// 做一个抬高，从而便于向下RayTrace
	info.Init(vFinalPos, A3DVECTOR3(0.0f, -100.0f, 0.0f), vExt);
	if (AABBCollideWithEnv(&info))
	{
		if (info.fFraction < 1E-5f)
		{//终点不合法
			return false;
		}
		A3DVECTOR3 vHitPos(info.vStart + info.vDelta * info.fFraction);
		vHitPos.y -= vExt.y;
		if (vHitPos.y > vTerrainPos.y)
		{
			vTerrainPos = vHitPos;
			vTerrainNormal = info.ClipPlane.GetNormal();
		}
	}


	// 此时，trace到的第一个位置为vTerrainPos
	if(fabs(vTerrainPos.y-posFrom.h) < HEIGHT_STEP_THRESH * m_fSampleStep 
		&& vTerrainNormal.y > SLOPE_THRESH )
	{
		posTo.h=vTerrainPos.y;
	}
	else
		return false;

	// 开始进行碰撞检测
	vFinalPos.y = posTo.h + vExt.y + LOCAL_EPSILON;
	vDelta.y = vFinalPos.y - vPFCenter.y;
	info.Init(vPFCenter, vDelta, vExt);
	
	if(!AABBCollideWithEnv(&info))
	{
		return true;
	}
	//step up
	vPFCenter.y = posFrom.h + vExt.y + m_fPFStepHeight;
	//vDelta.y = 0.0f;
	info.Init(vPFCenter, vDelta, vExt);
	if (!AABBCollideWithEnv(&info))
	{
		return true;
	}

#undef  LOCAL_EPSILON
	return false;
}

PLAYER_SAMPLE_POS CPlayerPassMapGenerator::GenSeedPos()
{
	PLAYER_SAMPLE_POS seed;
	seed.u = rand() % m_iWidth;
	seed.v = rand() % m_iLength;
	seed.h = 0.0f;

	return seed;
}

//////////////////////////////////////////////////////////////////////////
// In:		posSeed, only the u, v is valid
// Out:	   posSeed, if return true , then posSeed.h is the terrain height of the pos!
//////////////////////////////////////////////////////////////////////////
bool CPlayerPassMapGenerator::IsSeedPosValid(PLAYER_SAMPLE_POS& posSeed)
{
	// 合法位置，且未计算过
	if(!VALID_POS_IN_MAP(posSeed.u,posSeed.v) || PASS_MAP(posSeed.u,posSeed.v) > PASSMAP_VIRGIN	)
		return false;
	
	// 地形状况 1. 斜率阈值要求 2. 不在水下 3. 无障碍物在其上
	A3DVECTOR3 vSeedPos, vTPos, vTN;
	Transf2DTo3D(posSeed, vSeedPos);
	GetTerrainInfo(vSeedPos,vTPos, vTN);

	if(vTN.y < SEEDPOS_SLOPE_Y_THRESH || IsUnderWater(vTPos) )
		return false;

	// Trace the environment
	vSeedPos.y=vTPos.y+300.0f;
	A3DVECTOR3 vHitPos,vNormal;
	CConvexHullData* pTraceCHData=NULL;
	if(VertRayTraceEnv(vSeedPos, vHitPos, vNormal, 310.0f, pTraceCHData))
		return false;

	posSeed.h = vTPos.y;
	return true;
}

void CPlayerPassMapGenerator::ComputePassMap()
{
	PLAYER_SAMPLE_POS seed;
	while(SearchValidSeed(20, seed))
	{
		ExpandPassRegion(seed);
	}

	// 计算完后，将所有的未计算区域置为不可达
	FillUnreachableArea();

	// 对不可达区域进行膨胀
	InflateUnreachableArea();
}

bool CPlayerPassMapGenerator::SearchValidSeed(int iTryTimes, PLAYER_SAMPLE_POS& seed)
{
	for(int i=0;i<iTryTimes;i++)
	{
		seed=GenSeedPos();
		if(IsSeedPosValid(seed))
			return true;
	}

	return false;
}

void CPlayerPassMapGenerator::FillUnreachableArea()
{
	for(int i=0; i < m_iLength; i++)
		for(int j=0; j < m_iWidth; j++)
		{
			if(PASS_MAP(j,i) == PASSMAP_VIRGIN )
				PASS_MAP(j, i) = PASSMAP_UNREACHABLE;
		}
}

//////////////////////////////////////////////////////////////////////////
// 目前的方案为在如下三种情况满足时
// 1.左右邻域都不可达 2.上下邻域都不可达 3. 对角线邻域不可达
// 则当前点也置为不可达
//////////////////////////////////////////////////////////////////////////
void CPlayerPassMapGenerator::InflateUnreachableArea()
{
	// new a buffer
	int iSize = m_iLength*m_iWidth;
	UCHAR *bufBak = new UCHAR[iSize];
	memcpy(bufBak, m_arrPassMap, iSize);

//////////////////////////////////////////////////////////////////////////
#define  BUFBAK(u,v)  bufBak[(v)*m_iWidth+u]
//////////////////////////////////////////////////////////////////////////

	// 循环时不考虑最外层一个象素的边
	// 膨胀1: 填充须边
	int i;
	for(i=1; i < m_iLength-1; i++)
		for(int j=1; j < m_iWidth-1; j++)
		{
			// 考察8邻域
			if(		 BUFBAK(j+1,i) == PASSMAP_UNREACHABLE && BUFBAK(j-1, i) == PASSMAP_UNREACHABLE
				|| BUFBAK(j,i+1) == PASSMAP_UNREACHABLE && BUFBAK(j, i-1) == PASSMAP_UNREACHABLE
				|| BUFBAK(j+1,i+1) == PASSMAP_UNREACHABLE && BUFBAK(j-1, i-1) == PASSMAP_UNREACHABLE
				|| BUFBAK(j-1,i+1) == PASSMAP_UNREACHABLE && BUFBAK(j+1, i-1) == PASSMAP_UNREACHABLE )
			{
				PASS_MAP(j,i)=PASSMAP_UNREACHABLE;
			}

		}
	
	// 膨胀2：膨胀对角线方向的单列元素，该处理是以2*2的grid为单位！
	memcpy(bufBak, m_arrPassMap, iSize);

	// 对角线 "/"
	for(i=0; i < m_iLength-2; i++)						// Z
		for(int j=0; j < m_iWidth-2; j++)				// X
		{
			
			if(	BUFBAK(j, i) == PASSMAP_UNREACHABLE && BUFBAK(j+1, i+1) == PASSMAP_UNREACHABLE
				&& BUFBAK(j, i+1) == PASSMAP_REACHABLE && BUFBAK(j+1, i) == PASSMAP_REACHABLE )
			{
				PASS_MAP(j+1,i) =PASSMAP_UNREACHABLE;
				PASS_MAP(j, i+1) = PASSMAP_UNREACHABLE;
			}
		}

	for(i=0; i < m_iLength-2; i++)						// Z
		for(int j=0; j < m_iWidth-2; j++)				// X
		{
			
			if(	BUFBAK(j, i+1) == PASSMAP_UNREACHABLE && BUFBAK(j+1, i) == PASSMAP_UNREACHABLE
				&& BUFBAK(j, i) == PASSMAP_REACHABLE && BUFBAK(j+1, i+1) == PASSMAP_REACHABLE )
			{
				PASS_MAP(j,i) =PASSMAP_UNREACHABLE;
				PASS_MAP(j+1, i+1) = PASSMAP_UNREACHABLE;
			}
		}

	//*
	// 最后做一个腐蚀，将8邻域中不可达邻域数<3的情况剔除
	memcpy(bufBak, m_arrPassMap, iSize);
	for(i=1; i < m_iLength-2; i++)
		for(int j=1; j < m_iWidth-2; j++)
		{
			if(BUFBAK(j, i)!=PASSMAP_UNREACHABLE) continue;
			
			// 考察8邻域
			int counter = 0;
			if( BUFBAK(j+1,i) == PASSMAP_REACHABLE ) counter ++;
			if( BUFBAK(j-1, i) == PASSMAP_REACHABLE) counter ++;
			if( BUFBAK(j,i+1) == PASSMAP_REACHABLE ) counter ++;
			if( BUFBAK(j, i-1) == PASSMAP_REACHABLE) counter ++;
			if( BUFBAK(j+1,i+1) == PASSMAP_REACHABLE ) counter ++;
			if( BUFBAK(j-1, i+1) == PASSMAP_REACHABLE) counter ++;
			if( BUFBAK(j+1,i-1) == PASSMAP_REACHABLE ) counter ++;
			if( BUFBAK(j-1, i-1) == PASSMAP_REACHABLE) counter ++;
			if(counter > 5  )
			{
				// 8-邻域中仅有2个元素不可达，则该点可达！
				PASS_MAP(j,i)=PASSMAP_REACHABLE;
			}

		}
	
	//*/

	delete [] bufBak;

}

bool CPlayerPassMapGenerator::ExportMaps(CBitImage* pReachableImage, CBlockImage<FIX16>* pDeltaHImage)
{
	if( pReachableImage ==NULL || pDeltaHImage == NULL )
		return false;

	// before exporting, we should set all virgin area to unreachable area!
	FillUnreachableArea();
	_CleanDHMap();
	pReachableImage->Init(m_arrPassMap, m_iWidth, m_iLength, (UCHAR)PASSMAP_REACHABLE, m_fSampleStep);
	pDeltaHImage->Init(m_arrHeightMap, m_iWidth, m_iLength, m_fSampleStep);
	
	return true;
}

void CPlayerPassMapGenerator::SetPosReachable(const PLAYER_SAMPLE_POS& sPos)
{
	PASS_MAP(sPos.u, sPos.v) = PASSMAP_REACHABLE;

#define SET_DELTAHEIGHT_DIRECTLY 1

#if SET_DELTAHEIGHT_DIRECTLY
	
	// 对高度采用直接设置的方案！
	HEIGHT_MAP(sPos.u, sPos.v) = FLOATTOFIX16(sPos.h);

#else

	// 对高度采用插值的方法求解！
	float fDHSum = 0.0f;

	// 依次遍历8邻域
	int u,v, counter=0;

	u=sPos.u+1;
	v=sPos.v;
	if(VALID_POS_IN_MAP(u,v) && PASS_MAP(u,v) == PASSMAP_REACHABLE)
	{
		fDHSum+= FIX16TOFLOAT(HEIGHT_MAP(u,v));
		counter++;
	}

	u=sPos.u+1;
	v=sPos.v+1;
	if(VALID_POS_IN_MAP(u,v) && PASS_MAP(u,v) == PASSMAP_REACHABLE)
	{
		fDHSum+= FIX16TOFLOAT(HEIGHT_MAP(u,v));
		counter++;
	}

	u=sPos.u;
	v=sPos.v+1;
	if(VALID_POS_IN_MAP(u,v) && PASS_MAP(u,v) == PASSMAP_REACHABLE)
	{
		fDHSum+= FIX16TOFLOAT(HEIGHT_MAP(u,v));
		counter++;
	}

	u=sPos.u-1;
	v=sPos.v+1;
	if(VALID_POS_IN_MAP(u,v) && PASS_MAP(u,v) == PASSMAP_REACHABLE)
	{
		fDHSum+= FIX16TOFLOAT(HEIGHT_MAP(u,v));
		counter++;
	}

	u=sPos.u-1;
	v=sPos.v;
	if(VALID_POS_IN_MAP(u,v) && PASS_MAP(u,v) == PASSMAP_REACHABLE)
	{
		fDHSum+= FIX16TOFLOAT(HEIGHT_MAP(u,v));
		counter++;
	}

	u=sPos.u-1;
	v=sPos.v-1;
	if(VALID_POS_IN_MAP(u,v) && PASS_MAP(u,v) == PASSMAP_REACHABLE)
	{
		fDHSum+= FIX16TOFLOAT(HEIGHT_MAP(u,v));
		counter++;
	}

	u=sPos.u;
	v=sPos.v-1;
	if(VALID_POS_IN_MAP(u,v) && PASS_MAP(u,v) == PASSMAP_REACHABLE)
	{
		fDHSum+= FIX16TOFLOAT(HEIGHT_MAP(u,v));
		counter++;
	}

	u=sPos.u+1;
	v=sPos.v-1;
	if(VALID_POS_IN_MAP(u,v) && PASS_MAP(u,v) == PASSMAP_REACHABLE)
	{
		fDHSum+= FIX16TOFLOAT(HEIGHT_MAP(u,v));
		counter++;
	}
	
	fDHSum/=counter;
	HEIGHT_MAP(sPos.u, sPos.v) = FLOATTOFIX16(fDHSum);

#endif

}

void CPlayerPassMapGenerator::Reset(const PLAYER_SAMPLE_POS& sPos)
{
	PASS_MAP(sPos.u, sPos.v) = PASSMAP_VIRGIN;
	HEIGHT_MAP(sPos.u, sPos.v) = 0;
	SURFACE_MAP(sPos.u, sPos.v).h = -1.0f;
	SURFACE_MAP(sPos.u, sPos.v).n.Clear();
}

void CPlayerPassMapGenerator::SetPosUnreachable(const PLAYER_SAMPLE_POS& sPos)
{
	PASS_MAP(sPos.u, sPos.v) = PASSMAP_UNREACHABLE;
	HEIGHT_MAP(sPos.u, sPos.v) = 0;
	SURFACE_MAP(sPos.u, sPos.v).h = -1.0f;
	SURFACE_MAP(sPos.u, sPos.v).n.Clear();

}

bool CPlayerPassMapGenerator::ImportMaps(const CBitImage* pReachableImage, const CBlockImage<FIX16>* pDeltaHImage)
{
	if( pReachableImage ==NULL || pDeltaHImage == NULL )
		return false;

	Release();
	// Verify the two images has the same parameters
	int ri_w,ri_l,dhi_w,dhi_l;
	pReachableImage->GetImageSize(ri_w, ri_l);
	pDeltaHImage->GetImageSize(dhi_w, dhi_l);
	if(!(ri_w==dhi_w && ri_l==dhi_l && pReachableImage->GetPixelSize() == pDeltaHImage->GetPixelSize()))
		return false;

	m_fSampleStep=pReachableImage->GetPixelSize();
	m_iWidth=ri_w;
	m_iLength=ri_l;
	int iSize=m_iWidth*m_iLength;

	// 分配空间，按照行优先（u优先）存储
	m_arrPassMap=new UCHAR [iSize];
	m_arrHeightMap=new FIX16 [iSize];
	
	int i;
	// Start copy the data
	for(i=0; i < m_iLength; i++ )				// step on v
		for(int j=0; j < m_iWidth; j++ )			//  step on u
		{
			PASS_MAP(j, i) = pReachableImage->GetPixel(j, i)? PASSMAP_REACHABLE:PASSMAP_UNREACHABLE;
			HEIGHT_MAP(j,i) =pDeltaHImage->GetPixel(j, i);
		}

 	m_arrSurface = new Surface_Info [iSize];
	for ( i = 0; i < iSize; ++i)
	{
		m_arrSurface[i].h = -1.0f;
		m_arrSurface[i].n.Clear();
	}

	return true;

}


bool CPlayerPassMapGenerator::IsPosReachable(int u, int v)
{
	if(!m_arrPassMap) return true;
	return (PASS_MAP(u,v) == PASSMAP_REACHABLE);
}

float CPlayerPassMapGenerator::GetPosDeltaHeight(const PLAYER_SAMPLE_POS& sPos)
{
	if(!m_arrHeightMap) return 0;

	if(!IsPosReachable(sPos)) return 0;

	FIX16 f16DeltaHeight = HEIGHT_MAP(sPos.u,sPos.v);

	return FIX16TOFLOAT(f16DeltaHeight);
}






void CPlayerPassMapGenerator::_CleanDHMap()
{
	int iSize=m_iWidth*m_iLength;
	
	int i;
	for (i = 0; i < iSize; ++i)
	{
		if (m_arrPassMap[i] == PASSMAP_UNREACHABLE)
		{
			m_arrHeightMap[i] = 0;
		}
	}
}