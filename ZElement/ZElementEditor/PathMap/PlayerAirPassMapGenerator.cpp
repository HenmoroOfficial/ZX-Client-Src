/********************************************************************
	created:	2008/04/01
	author:		kuiwu
	
	purpose:	player air move map
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/


#include "StdAfx.h"
#include "PlayerAirPassMapGenerator.h"
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


//////////////////////////////////////////////////////////////////////
// Some global functions which depend on this project!
//////////////////////////////////////////////////////////////////////


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





//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayerAirPassMapGenerator::CPlayerAirPassMapGenerator()
{
	m_arrPassMap=NULL;
	m_arrSurface = NULL;
	// initial values
	m_fSampleStep = 1;
	m_iWidth =0;
	m_iLength =0;

	
}

CPlayerAirPassMapGenerator::~CPlayerAirPassMapGenerator()
{
	Release();

}

void CPlayerAirPassMapGenerator::Release()
{
	if(m_arrPassMap) 
	{
		delete [] m_arrPassMap;
		m_arrPassMap=NULL;
	}


	if (m_arrSurface)
	{
		delete[] m_arrSurface;
		m_arrSurface = NULL;
	}
}

void CPlayerAirPassMapGenerator::SetSampleInfo(float fss, int iTWidth, int iTLength)
{
	Release();
	m_fSampleStep=fss;
	m_iWidth=FLOATTOINT(iTWidth/fss);
	m_iLength=FLOATTOINT(iTLength/fss);
	int iSize=m_iWidth*m_iLength;

	// 分配空间，按照行优先（u优先）存储
	m_arrPassMap=new UCHAR [iSize];


	// 初始化区域 对于m_arrPassMap而言，0表示未计算，1表示可达，2表示不可达
	memset(m_arrPassMap, PASSMAP_VIRGIN, iSize*sizeof(UCHAR));


	m_arrSurface = new Surface_Info [iSize];
	for (int i = 0; i < iSize; ++i)
	{
		m_arrSurface[i].h = -1.0f;
		m_arrSurface[i].n.Clear();
	}
}



void CPlayerAirPassMapGenerator::Transf2DTo3D(const PLAYER_SAMPLE_POS& sPos, A3DVECTOR3& vPos)
{
	vPos.y = sPos.h;
	vPos.x = (sPos.u + 0.5f - m_iWidth * 0.5f ) * m_fSampleStep;
	vPos.z = (sPos.v + 0.5f -m_iLength * 0.5f ) * m_fSampleStep;
}

void CPlayerAirPassMapGenerator::Transf3DTo2D(const A3DVECTOR3& vPos, PLAYER_SAMPLE_POS& sPos)
{
	sPos.h = vPos.y;
	float fssRecip = 1.0f/m_fSampleStep;
	sPos.u = FLOATTOINT( vPos.x * fssRecip + m_iWidth * 0.5f - 0.5f );
	sPos.v = FLOATTOINT( vPos.z * fssRecip + m_iLength * 0.5f - 0.5f );
}


void CPlayerAirPassMapGenerator::FillUnreachableArea()
{
	for(int i=0; i < m_iLength; i++)
		for(int j=0; j < m_iWidth; j++)
		{
			if(PASS_MAP(j,i) == PASSMAP_VIRGIN )
				PASS_MAP(j, i) = PASSMAP_UNREACHABLE;
		}
}


bool CPlayerAirPassMapGenerator::ExportMaps(CBitImage* pReachableImage)
{
	if( pReachableImage ==NULL)
		return false;

	// before exporting, we should set all virgin area to unreachable area!
	FillUnreachableArea();

	pReachableImage->Init(m_arrPassMap, m_iWidth, m_iLength, (UCHAR)PASSMAP_REACHABLE, m_fSampleStep);

	
	return true;
}

void CPlayerAirPassMapGenerator::SetPosReachable(const PLAYER_SAMPLE_POS& sPos)
{
	PASS_MAP(sPos.u, sPos.v) = PASSMAP_REACHABLE;

#define SET_DELTAHEIGHT_DIRECTLY 1

#if SET_DELTAHEIGHT_DIRECTLY
	
	// 对高度采用直接设置的方案！


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

void CPlayerAirPassMapGenerator::Reset(const PLAYER_SAMPLE_POS& sPos)
{
	PASS_MAP(sPos.u, sPos.v) = PASSMAP_VIRGIN;

	SURFACE_MAP(sPos.u, sPos.v).h = -1.0f;
	SURFACE_MAP(sPos.u, sPos.v).n.Clear();
}

void CPlayerAirPassMapGenerator::SetPosUnreachable(const PLAYER_SAMPLE_POS& sPos)
{
	PASS_MAP(sPos.u, sPos.v) = PASSMAP_UNREACHABLE;

	SURFACE_MAP(sPos.u, sPos.v).h = -1.0f;
	SURFACE_MAP(sPos.u, sPos.v).n.Clear();

}

bool CPlayerAirPassMapGenerator::ImportMaps(const CBitImage* pReachableImage)
{
	if( pReachableImage ==NULL )
		return false;

	Release();
	// Verify the two images has the same parameters
	int ri_w,ri_l;
	pReachableImage->GetImageSize(ri_w, ri_l);


	m_fSampleStep=pReachableImage->GetPixelSize();
	m_iWidth=ri_w;
	m_iLength=ri_l;
	int iSize=m_iWidth*m_iLength;

	// 分配空间，按照行优先（u优先）存储
	m_arrPassMap=new UCHAR [iSize];

	int i;
	// Start copy the data
	for(i=0; i < m_iLength; i++ )				// step on v
		for(int j=0; j < m_iWidth; j++ )			//  step on u
		{
			PASS_MAP(j, i) = pReachableImage->GetPixel(j, i)? PASSMAP_REACHABLE:PASSMAP_UNREACHABLE;

		}

 	m_arrSurface = new Surface_Info [iSize];
	for ( i = 0; i < iSize; ++i)
	{
		m_arrSurface[i].h = -1.0f;
		m_arrSurface[i].n.Clear();
	}

	return true;

}


bool CPlayerAirPassMapGenerator::IsPosReachable(int u, int v)
{
	if(!m_arrPassMap) return true;
	return (PASS_MAP(u,v) == PASSMAP_REACHABLE);
}

float CPlayerAirPassMapGenerator::GetPosDeltaHeight(const PLAYER_SAMPLE_POS& sPos)
{
	return 0.0f;
}

bool CPlayerAirPassMapGenerator::IsPosReachable(const PLAYER_SAMPLE_POS& sPos)
{
	return IsPosReachable(sPos.u, sPos.v);
}


bool CPlayerAirPassMapGenerator::IsPosReachable(const A3DVECTOR3 & vPos)
{
	PLAYER_SAMPLE_POS sPos;
	Transf3DTo2D(vPos, sPos);
	return IsPosReachable(sPos);
}

float CPlayerAirPassMapGenerator::GetPosDeltaHeight(const A3DVECTOR3 & vPos)
{
	PLAYER_SAMPLE_POS sPos;
	Transf3DTo2D(vPos, sPos);
	return GetPosDeltaHeight(sPos);
}

