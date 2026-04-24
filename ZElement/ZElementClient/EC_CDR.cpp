 /*
 * FILE: EC_CDR.cpp
 *
 * DESCRIPTION:  A set of interfaces define the moving strategy of the avatar.
 *							Which focus on collision detection and give an appropriate response.
 *
 * CREATED BY: He wenfeng, 2005/3/8
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */


//#define  CDR_DEBUG

//#define   CDR_PROFILE


#include "EC_Global.h"
#include "EC_CDR.h"
#include "EC_ManOrnament.h"
#include "EC_ManNPC.h"
#include "EC_ManMatter.h"
#include "EC_World.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "A3DTerrain2.h"
#include <A3DCollision.h>
#include "a3dfuncs.h"
#include "EC_ManPlayer.h"
#include "EC_ElsePlayer.h"

#ifdef CDR_DEBUG
#include <A3DEngine.h>
#include <A3DFont.h>

char    msg[200];
#define OUTPUT_DEBUG_INFO(s)   OutputDebugStringA(s)
#endif




#include "aabbcd.h"


#define FLY_MAX_HEIGHT	800.0f		// 飞行的最大高度！
static float _max_fly_height = 800.f;

#define  CDR_BRUSH    0x1
#define  CDR_TERRAIN  0x2
#define  CDR_WATER    0x4


//#define  CDR_ACCUM_DIST

#define	 VEL_EPSILON     1E-4f	
#define  NORMAL_EPSILON	  1e-2f
#define  DIST_EPSILON    1e-4f
#define  VEL_MAX_SPEED   50.0f



//\todo  refactor the data&func to a class.  By kuiwu. [15/3/2006]
///////////////////////////////////////////////////////////////////////////
//data declaration


//hongkan

A3DVECTOR3 StepOnPoint;
A3DVECTOR3 StepOnNormal;
int StepOnSlope;

//@desc : used to retrieve support plane (ground or brush),  By Kuiwu[12/9/2005]
struct ground_trace_t 
{
	A3DVECTOR3	vStart;
	A3DVECTOR3	vExt;
	float		fDeltaY;   //down (-y)
	//float      fBump;
	A3DVECTOR3	vEnd;
	A3DVECTOR3	vHitNormal;
	bool		bSupport;  //false if ground missed

	int			iUserData1;		//	User data 1
	int			iUserData2;		//	User data 2

	ground_trace_t::ground_trace_t() :
	vStart(0.0f),
	vExt(0.0f),
	vEnd(0.0f),
	vHitNormal(0.0f)
	{
		fDeltaY		= 0.0f;
		bSupport	= false;
		iUserData1	= 0;
		iUserData2	= 0;
	}
};

struct move_t
{
	A3DVECTOR3 start;
	A3DVECTOR3 ext;
	A3DVECTOR3 velocity;
	float      t;
	float      slope;
	A3DVECTOR3  tpnormal;
	A3DVECTOR3  wishdir;
	float       wishspd;
	A3DVECTOR3  absvelocity;
	
	float       accel;

	A3DVECTOR3 end;
};
// change this array when some new submap can go!
const unsigned int available_maps[11][8] = 
{
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 0, 0, 0, 0, 0, 0},
};

const unsigned int available_maps4x4[4][4] = 
{
	{0, 0, 0, 0},
	{0, 1, 1, 0},
	{0, 1, 1, 0},
	{0, 0, 0, 0},
};

const unsigned int available_maps3x3[3][3] = 
{
	{0, 0, 0},
	{0, 1, 0},
	{0, 0, 0},
};

const unsigned int available_maps1x1[1][1] = 
{
	{1},
};

///////////////////////////////////////////////////////////////////////////
//func declaration
bool   AdjustPosNormal(const A3DVECTOR3& pos, const A3DVECTOR3& ext, const bool bPull, const float fStepHei, A3DVECTOR3& NewPos, A3DVECTOR3& NewNormal);
bool  RetrieveSupportPlane(ground_trace_t * pTrc);
bool  IsInWater(const A3DVECTOR3& pos, const A3DVECTOR3& ext);
void SlideMove(CDR_INFO& CDRInfo, float& expMove);
bool BlockTest(const A3DVECTOR3& pos,  float dExp);


#ifdef CDR_HL2_MOVE
//ref hl2
void ClipVelocity(const A3DVECTOR3& in, const A3DVECTOR3& normal, A3DVECTOR3& out, float overbounce);
//int  TrySlideMove(move_t& mv, bool canRaise, bool& clearVelY);
int  TrySlideMove(move_t& mv);
void StepUp(move_t& mv, float step);
void WalkMove(move_t& mv, float step);
void JumpFallMove(move_t& mv);
//void JumpFallMove(move_t& mv, bool& clearVelY);
void FullGroundMove(move_t& mv, float gravity, float step);
void CheckVelocity(A3DVECTOR3& vel);
void DisableUpVel(const A3DVECTOR3& in, const A3DVECTOR3& normal, A3DVECTOR3& out);
void Accelerate(const A3DVECTOR3& wishdir, float wishspd,   float accel, float t, A3DVECTOR3& vel);
void AirAccelerate(const A3DVECTOR3& wishdir, float wishspd,   float accel, float t, A3DVECTOR3& vel);
void Friction(move_t& mv);
bool CheckLowSteepSlope(move_t mv, A3DVECTOR3 startpoint, A3DVECTOR3 SlopeNormal);

#endif

//////////////////////////////////////////////////////////////////////////
// Note by wenfeng, 05-09-16
//		This function is only for the Big world but not applicable for the 
//		Instance world!
//
//////////////////////////////////////////////////////////////////////////
bool IsPosInAvailableMap(const A3DVECTOR3& vPos)
{
	return true;
// 	float	x, z;
// 	int		su, sv;
// 	
// 	x = vPos.x + 512.0f;
// 	z = 512.0f - vPos.z;
// 
// 	if( x <= 0.0f || x >= 1024.0f || z <= 0.0f || z >= 1024.0f )
// 		return false;
// 
// 	su = (int)(x / 1024.0f);
// 	sv = (int)(z / 1024.0f);
// 
// 	if( su != 0 || sv != 0  )
// 		return false;
// 	return available_maps1x1[sv][su] ? true : false;
}

void SetMaxFlyHeight(float f)
{
	_max_fly_height = f;
}

bool RayTraceEnv(
						  const A3DVECTOR3& vStart, 
						  const A3DVECTOR3& vDelta,
						  A3DVECTOR3& vHitPos,
						  A3DVECTOR3& vNormal,
						  float& fraction
						  )
{
	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();
	CECOrnamentMan* pOrnMan=pWorld->GetOrnamentMan();

	return pOrnMan->RayTrace(vStart,vDelta,vHitPos,&fraction,vNormal);
}

inline void GetTerrainInfo(const A3DVECTOR3& vPos, A3DVECTOR3& vPosOnSurface, A3DVECTOR3& vNormal)
{
	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();
	A3DTerrain2* pTerrain=pWorld->GetTerrain();
	vPosOnSurface=vPos;
	vPosOnSurface.y=pTerrain->GetPosHeight(vPos,&vNormal);
}

inline void GetWaterAreaInfo(const A3DVECTOR3& vPos, A3DVECTOR3& vPosOnSurface, A3DVECTOR3& vNormal)
{
	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();
	vPosOnSurface = vPos;
	vPosOnSurface.y = pWorld->GetWaterHeight(vPos);
	vNormal = g_vAxisY;
}

// 向下 Trace 地形和建筑，并返回第一个碰撞点的情况
bool VertRayTrace(const A3DVECTOR3& vPos, A3DVECTOR3& vHitPos, A3DVECTOR3& vHitNormal, float DeltaY)
{
	A3DVECTOR3 vTerrainPos, vTerrainNormal, vBuildingPos, vBuildingNormal;
	float fraction;

	GetTerrainInfo(vPos,vTerrainPos,vTerrainNormal);
	if(RayTraceEnv(vPos,-DeltaY*g_vAxisY,vBuildingPos,vBuildingNormal,fraction))
	{
		if(vBuildingPos.y>vTerrainPos.y)
		{
			// 与建筑发生了碰撞
			vHitPos=vBuildingPos;
			vHitNormal=vBuildingNormal;
			return true;
		}
	}
	
	//@note: kuiwu  fix the bug 2005/8/10

	//if(vTerrainPos.y<=vPos.y && vTerrainPos.y>=vPos.y-DeltaY)
	
	if (vTerrainPos.y > vPos.y || (vTerrainPos.y<=vPos.y && vTerrainPos.y>=vPos.y-DeltaY) ) 
	{
	
		vHitPos=vTerrainPos;
		vHitNormal=vTerrainNormal;
		return true;
	}
	return false;
}

// 仅仅向下 Trace 建筑
bool VertRayTraceEnv(const A3DVECTOR3& vPos, A3DVECTOR3& vHitPos, A3DVECTOR3& vHitNormal, float DeltaY)
{
	float fraction;
	return RayTraceEnv(vPos,-DeltaY*g_vAxisY,vHitPos,vHitNormal,fraction);
}

// Trace carrier NPCs
bool RayTraceCarrier(const A3DVECTOR3& vStart, const A3DVECTOR3& vDelta, A3DVECTOR3& vHitPos, A3DVECTOR3& vNormal, float& fraction )
{
	CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();
	BrushTraceInfo btInfo;
	btInfo.Init(vStart, vDelta, A3DVECTOR3(0.0f), true);
	bool bCollide =	pNPCMan->TraceWithBrush(&btInfo, true);

	if (bCollide)
	{
		vHitPos = vStart + vDelta * btInfo.fFraction;
		fraction = btInfo.fFraction;
		vNormal = btInfo.ClipPlane.GetNormal();	
	}
	
	return bCollide;
}

bool AABBCollideWithBrush(CHBasedCD::BrushTraceInfo * pInfo)
{
	CECWorld * pWorld = g_pGame->GetGameRun()->GetWorld();

	CECOrnamentMan* pOrnMan=pWorld->GetOrnamentMan();
	bool bBrush = pOrnMan->TraceWithBrush(pInfo);

	CHBasedCD::BrushTraceInfo info = *pInfo;
	CECMatterMan* pMatterMan = pWorld->GetMatterMan();
	if( pMatterMan->TraceWithBrush(&info) && info.fFraction < pInfo->fFraction )
	{
		*pInfo = info;
		bBrush = true;
	}

	info = *pInfo;
	CECNPCMan* pNPCMan = pWorld->GetNPCMan();
	if( pNPCMan->TraceWithBrush(&info) && info.fFraction < pInfo->fFraction )
	{
		*pInfo = info;
		bBrush = true;
	}

/*	info = *pInfo;
	CECPlayerMan* pPlayerMan = pWorld->GetPlayerMan();
	APtrArray<CECElsePlayer*> playerArray;
	if ( pPlayerMan && pPlayerMan->TraceWithBrush( &info, playerArray ) )
	{
		//
		for ( int i = 0; i < playerArray.GetSize(); ++i )
		{
			char msg[100];
			CECElsePlayer* pPlayer = playerArray[i];
			sprintf(msg, "player name=%s\n", pPlayer->GetName() );
			OutputDebugStringA(msg);
		}
	}
*/	//Canceled 2012-07-23.
	return bBrush;
}


//-----------------------------------------------------------------------------
//Added 2012-06-25.
void UpdateVelocityAfterCD( const A3DVECTOR3& vPos1, const A3DVECTOR3& vPos2, int nMass1, int nMass2, A3DVECTOR3& vVelocity1, A3DVECTOR3& vVelocity2 )
{
	if ( nMass1 + nMass2 == 0 )
	{
		return;
	}

	A3DVECTOR3 vNormalDir = vPos2 - vPos1;
	vNormalDir.Normalize();
	if ( DotProduct( vNormalDir, vVelocity1 ) < 0.0f )
	{
		vNormalDir = -vNormalDir;
	}
	A3DVECTOR3 vVelocityNormal1 = vNormalDir * DotProduct( vVelocity1, vNormalDir );
	A3DVECTOR3 vVelocityTangent1 = vVelocity1 - vVelocityNormal1;

	if ( DotProduct( vNormalDir, vVelocity2 ) < 0.0f )
	{
		vNormalDir = -vNormalDir;
	}	
	A3DVECTOR3 vVelocityNormal2 = vNormalDir * DotProduct( vVelocity2, vNormalDir );
	A3DVECTOR3 vVelocityTangent2 = vVelocity2 - vVelocityNormal2;

	A3DVECTOR3 vVelocityNormalNew1 = ( (float)( nMass1 - nMass2 ) / ( nMass1 + nMass2 ) ) * vVelocityNormal1 + ( (float)(2 * nMass2) / (nMass1 + nMass2) ) * vVelocityNormal2;
	A3DVECTOR3 vVelocityNormalNew2 = ( (float)( nMass2 - nMass1 ) / ( nMass1 + nMass2 ) ) * vVelocityNormal2 + ( (float)(2 * nMass1) / (nMass1 + nMass2) ) * vVelocityNormal1;

	vVelocity1 = vVelocityNormalNew1 + vVelocityTangent1;
	vVelocity2 = vVelocityNormalNew2 + vVelocityTangent2;
}

//Added 2012-09-03.
void CalculateDeltaDist( const A3DVECTOR3& vVelocity, float fAcceleration, const A3DVECTOR3& vClipVelocity, const A3DVECTOR3& vTPNormal, float fDaltaTime, A3DVECTOR3& vDeltaDist )
{
	A3DVECTOR3 vCurVelocity = vClipVelocity;
	A3DVECTOR3 vRealDirH = vVelocity;
	if (fabs(vRealDirH.y) > 0.0001)
	{
		vRealDirH.y = 0.0f;
		vRealDirH.Normalize();
	}

	float fSpeedH = vVelocity.Magnitude();

	if (fSpeedH < 0.0f)
	{
		vRealDirH = -vRealDirH;
		fSpeedH = -fSpeedH;
	}

	vCurVelocity.y = 0.0f;
	Accelerate(vRealDirH, fSpeedH, fAcceleration, fDaltaTime, vCurVelocity);
	vCurVelocity.y = 0.0f;
	if ( vCurVelocity.SquaredMagnitude() < 1E-4f )
	{
		vDeltaDist.Clear();
		return;
	}

	A3DVECTOR3 originVel(vCurVelocity);
	ClipVelocity(originVel, vTPNormal, vCurVelocity, 1.01f);
	
	vDeltaDist = vCurVelocity * fDaltaTime;
}
//-----------------------------------------------------------------------------


//@desc :used to trace the environment, brush&terrain&water  By Kuiwu[8/10/2005]
struct env_trace_t
{
	A3DVECTOR3 vStart;  // brush start
	A3DVECTOR3 vExt;
	A3DVECTOR3 vDelta;
	A3DVECTOR3 vTerStart;
	A3DVECTOR3 vWatStart;
	DWORD      dwCheckFlag;
	bool       bWaterSolid;

	float      fFraction;
	A3DVECTOR3 vHitNormal;
	bool       bStartSolid;     //start in solid
	DWORD      dwClsFlag;       //collision flag

	int			iUserData1;		//	User data 1
	int			iUserData2;		//	User data 2

	env_trace_t::env_trace_t() :
	vStart(0.0f),
	vExt(0.0f),
	vDelta(0.0f),
	vTerStart(0.0f),
	vWatStart(0.0f),
	vHitNormal(0.0f)
	{
		dwCheckFlag	= 0;
		bWaterSolid	= false;
		fFraction	= 1.0f;
		bStartSolid	= false;
		dwClsFlag	= 0;
		iUserData1	= 0;
		iUserData2	= 0;
	}
};


/*
 * ray: origin + t* dir,  triangle:  p(u,v) = (1-u -v)vert[0] + u*vert[1] + v*vert[2]
 * @desc :
 * @param vDir: normalized direction     
 * @param bCull: cull back face if true
 * @return :
 * @note:
 * @todo:  
 * @author: kuiwu [8/10/2005]
 * @ref:  Tomas Moller's JGT code
 */
bool   RayTriangleIntersect(const  A3DVECTOR3& vOrigin, const A3DVECTOR3& vDir,   A3DVECTOR3 *  vert[3], float& t, float& u, float& v, bool bCull)
{
	// find vectors for two edges sharing vert0 
	A3DVECTOR3 edge1( (*vert[1]) - (*vert[0]));
	A3DVECTOR3 edge2( (*vert[2]) - (*vert[0]));
	// begin calculating determinant - also used to calculate U parameter 
	A3DVECTOR3 pvec = CrossProduct(vDir, edge2);
	// if determinant is near zero, ray lies in plane of triangle 
	float det = DotProduct(edge1, pvec);
#define   LOCAL_EPSILON   1e-5f
	if (bCull)
	{
		if(det<LOCAL_EPSILON)
			return false;
		// From here, det is > 0. 
		// Calculate distance from vert0 to ray origin
		A3DVECTOR3 tvec(vOrigin - (*vert[0]));
		// Calculate U parameter and test bounds
		u = DotProduct(tvec, pvec);
		if ((u  < 0.0f) || (u > det))
			return false;
		
		// prepare to test V parameter 
		A3DVECTOR3 qvec = CrossProduct(tvec, edge1);
		// calculate V parameter and test bounds 
		v = DotProduct(vDir, qvec);
		if ((v < 0.0f) || (u + v > det))
			return false;
		
	     // calculate t, ray intersects triangle 
		t = DotProduct(edge2, qvec) ;
		// Det > 0 so we can early exit here
		// Intersection point is valid if distance is positive 
		// (else it can just be a face behind the orig point)
		if (t < 0.0f)
		{
			return false;
		}
		
		float OneOverDet = 1.0f / det;
		 
		t *= OneOverDet;
		u *= OneOverDet;
		v *= OneOverDet;
	}
	else
	{
		if(det>-LOCAL_EPSILON && det<LOCAL_EPSILON)
			return false;
		float OneOverDet = 1.0f / det;
		// Calculate distance from vert0 to ray origin
		A3DVECTOR3 tvec(vOrigin - (*vert[0]));
		// calculate U parameter and test bounds 
		u = DotProduct(tvec, pvec)* OneOverDet;
		if ((u < 0.0f) || (u > 1.0f))
			return false;
		// prepare to test V parameter 
		A3DVECTOR3 qvec = CrossProduct(tvec, edge1);
		// calculate V parameter and test bounds 
		v = DotProduct(vDir, qvec) * OneOverDet;
	    if ((v < 0.0f) || (u + v > 1.0f))
			return false;
		// calculate t, ray intersects triangle 
		t = DotProduct(edge2, qvec) * OneOverDet;
	}

#undef    LOCAL_EPSILON
	
	return true;
}



/*
 *
 * @desc :
 * @param :     
 * @return :
 * @note:
 * @todo:   
 * @author: kuiwu [9/10/2005]
 * @ref:
 */
bool SegmentTriangleIntersect( const A3DVECTOR3& vStart, const A3DVECTOR3& vDelta,  A3DVECTOR3 * vert[3], float& fFraction, bool bCull)
{
	float dist;
	A3DVECTOR3 vDir(vDelta);

	dist = vDir.Normalize();
	if (dist < 1E-5f)
	{
		assert(0 && "too small dist!");
		fFraction = 0.0f;
		return true;
	}

	float t, u, v;

	if (RayTriangleIntersect(vStart, vDir, vert, t, u, v, bCull) && (t>=0.0f) && (t <= dist))
	{
		//fFraction = t/dist;
		//fFraction = a_Max( 0.0f, fFraction -1E-4f); //put back
		fFraction  = (t- 5E-4f)/dist;
		a_ClampFloor(fFraction, 0.0f);
		return true;
	}

	return false;
}



/*
 *
 * @desc :
 * @param  bWaterSolid:   true, treat water and water plane as a solid; 
						  otherwise treat air and water plane as a solid.
 * @return :
 * @note:
 * @todo:   
 * @author: kuiwu [10/10/2005]
 * @ref:
 */
bool CollideWithWater(const A3DVECTOR3& vStart, const A3DVECTOR3& vDelta, bool bWaterSolid, float& fFraction, A3DVECTOR3& vHitNormal, bool& bStart )
{


#define LOCAL_EPSILON  1E-4f
	if (bWaterSolid && (vDelta.y > LOCAL_EPSILON ))
	{
		return false;
	}

	if ( !bWaterSolid && (vDelta.y < -LOCAL_EPSILON))
	{
		return false;
	}

	if (vDelta.y < LOCAL_EPSILON && vDelta.y > -LOCAL_EPSILON)
	{ //parallel  the water plane
		return false;
	}
	

	CECWorld * pWorld = g_pGame->GetGameRun()->GetWorld();
	float h0 = pWorld->GetWaterHeight(vStart);
	float h1 = pWorld->GetWaterHeight(vStart+vDelta);
	float hWater = a_Max(h0, h1);
	fFraction = 100.0f;

	vHitNormal = g_vAxisY;
	bStart = false;
	
	float t=  (hWater - vStart.y)/vDelta.y;
	if(t >= 0.0f && t<= 1.0f)
	{
		fFraction = a_Max(0.0f , t - 1E-2f);
		if (bWaterSolid && h0 > vStart.y)
		{
			fFraction = 0.0f;
			bStart = true;
		}

		if (!bWaterSolid && h0 < vStart.y)
		{
			fFraction = 0.0f;
			bStart = true;
		}

#ifdef CDR_DEBUG
	sprintf(msg, "water test startH %f deltaH %f fFraction %f h0 %f h1 %f\n", vStart.y, vDelta.y, fFraction, h0, h1);
	OUTPUT_DEBUG_INFO(msg);
#endif
		return true;
	}

#undef  LOCAL_EPSILON
	return false;
}

/*
 *
 * @desc :
 * @param :     
 * @return :
 * @note:
 * @todo:   
 * @author: kuiwu [9/10/2005]
 * @ref:
 */
bool CollideWithTerrain(const A3DVECTOR3& vStart, const A3DVECTOR3& vDelta, float& fFraction, A3DVECTOR3& vHitNormal, bool& bStart)
{

	CECWorld * pWorld = g_pGame->GetGameRun()->GetWorld();
	A3DTerrain2* pTerrain=pWorld->GetTerrain();
	bStart = false;
	float h1 = pTerrain->GetPosHeight(vStart, &vHitNormal);
	if (h1 > vStart.y + 1E-4f)
	{//start under terrain
		bStart = true;
		fFraction = 0.0f;
		return true;
	}


	int nWid, nHei; // in grid, 2 meters
	//float fMag = vDelta.Magnitude();
	float   fMag  = (vDelta.x > 0)? (vDelta.x): (-vDelta.x);
	nWid = (int)ceil(fMag / 2.0f);
	nWid = a_Max(3, nWid);
	fMag =(vDelta.z > 0)? (vDelta.z): (-vDelta.z);
	nHei= (int)ceil(fMag / 2.0f);
	nHei = a_Max(3, nHei);
	
	int nTriangles = nWid * nHei*2;
	A3DVECTOR3 * pVerts = new A3DVECTOR3[(nWid +1) * (nHei +1)];
	assert(pVerts != NULL);
	memset(pVerts, 0, sizeof(A3DVECTOR3) * (nWid + 1) * (nHei + 1));
	WORD * pIndices = new WORD[nTriangles*3];
	assert(pIndices != NULL);
	memset(pIndices, 0, sizeof(WORD) * nTriangles * 3);
	if (!pTerrain->GetFacesOfArea(vStart, nWid, nHei, pVerts, pIndices))
	{
		delete[] pVerts;
		delete[] pIndices;
		return false;
	}
	int i;
	A3DVECTOR3 * vert[3];
	//@note : Here init the fraction.  By Kuiwu[9/10/2005]
	fFraction = 100.0f;
	float tmpFraction = fFraction;

	for (i = 0; i < nTriangles; i++)
	{
		vert[0] = pVerts +pIndices[i*3];
		vert[1] = pVerts +pIndices[i*3+1];
		vert[2] = pVerts +pIndices[i*3+2];
		A3DVECTOR3 vPt;
	
		//@note: Tomas Moller's JGT code  : By Kuiwu[9/10/2005]
		//@note: discard the engine version because it put back the hit point too much. By Kuiwu[13/10/2005]
		
		//review: Tomas treats  ray lay in(or nearly) triangle as non-intersect
		//        And in some cases(rarely indeed) this may lead to penetrate a very small dist. 
		//		  The engine version seems to work well and the putback is acceptable.	
		//		  (it rarely happens,  so thoroughly test is needed)			 [8/30/2006 kuiwu]
		if(CLS_RayToTriangle(vStart, vDelta, *vert[0], *vert[1], *vert[2], vPt, true, &tmpFraction)
			&& (tmpFraction <= 1.0f)  && (tmpFraction < fFraction))
// 		if (SegmentTriangleIntersect(vStart, vDelta, vert, tmpFraction, true) 
// 			&& (tmpFraction < fFraction))
		{
			//get the triangle normal
			A3DVECTOR3 vEdge1((*vert[1]) - (*vert[0]) );
			A3DVECTOR3 vEdge2((*vert[2]) - (*vert[0]) );
			vHitNormal = CrossProduct(vEdge1, vEdge2);
			vHitNormal.Normalize();

			//@note : may be redundant, but to assure.  By Kuiwu[17/10/2005]
			A3DVECTOR3 vDir;
			Normalize(vDelta, vDir);
			if (DotProduct(vHitNormal, vDir) > 0.01f)
			{//leave the hit plane
				assert(0 && "hit a plane with same direction!");
				continue;
			}
			
			fFraction = a_Max(0.0f, tmpFraction);
		}
		
	}
	delete[] pVerts;
	delete[] pIndices;

// #ifdef CDR_DEBUG
// 	if (fFraction <= 1.0f)
// 	{
// 		A3DVECTOR3 vHitPos(vStart + vDelta * fFraction);
// 		A3DVECTOR3 vTerNormal;
// 		float fH2 = pTerrain->GetPosHeight(vHitPos, &vTerNormal);
// 		sprintf(msg, "terrain hit %f %f %f height %f trcNormal %f %f %f fFraction %f\n", vHitPos.x, vHitPos.y, vHitPos.z, fH2,
// 				 vTerNormal.x, vTerNormal.y, vTerNormal.z, fFraction);
// 		OUTPUT_DEBUG_INFO(msg);
// 
// 		//vHitNormal = vTerNormal;
// 	}
// 	else
// 	{
// 		A3DVECTOR3 vEnd(vStart + vDelta);
// 		float fH2 = pTerrain->GetPosHeight(vEnd);
// 		sprintf(msg, "terrain test clear %f %f %f height %f\n", vEnd.x, vEnd.y, vEnd.z,fH2);
// 		OUTPUT_DEBUG_INFO(msg);
// 	}
// #endif
	return (fFraction <= 1.0f);

}
/*
 * collision detect with environment, including brush, terrain and water
 * @desc :
 * @param :     
 * @return :
 * @note:
 * @todo:   
 * @author: kuiwu [8/10/2005]
 * @ref:
 */
bool CollideWithEnv(env_trace_t * pEnvTrc)
{
#ifdef CDR_PROFILE
	static int frames  = 0;
	static DWORD  tsum = 0;
	//DWORD tstart =	(DWORD)(ACounter::GetMicroSecondNow());
	DWORD tstart =	(DWORD)(ACounter::GetMicroSecondNow()/1000);
#endif

	pEnvTrc->fFraction = 100.0f;
	pEnvTrc->bStartSolid = false;
	pEnvTrc->dwClsFlag = 0;

	if ((pEnvTrc->dwCheckFlag & CDR_TERRAIN) == CDR_TERRAIN )
	{
		float fFraction;
		A3DVECTOR3 vTerNormal;
		bool bStart;
		if(CollideWithTerrain(pEnvTrc->vTerStart, pEnvTrc->vDelta, fFraction,  vTerNormal, bStart) 
			&& (fFraction < pEnvTrc->fFraction ))
		{
			assert(fFraction >= 0.0f);
			//pEnvTrc->fFraction = a_Max(0.0f, fFraction - 1E-4f);
			pEnvTrc->fFraction = fFraction ;
			pEnvTrc->vHitNormal = vTerNormal;
			pEnvTrc->bStartSolid = bStart;
			pEnvTrc->dwClsFlag = CDR_TERRAIN;
//#ifdef CDR_DEBUG
//			A3DVECTOR3 vHitPos(pEnvTrc->vTerStart + pEnvTrc->vDelta * fFraction);
//			sprintf(msg, "collide terrain, fraction %f pos %f %f %f normal %f %f %f bStart %d\n", fFraction, vHitPos.x, vHitPos.y, vHitPos.z, vTerNormal.x, vTerNormal.y, vTerNormal.z, bStart);
//			OUTPUT_DEBUG_INFO(msg);
//#endif
		}

	}

	if ((pEnvTrc->dwCheckFlag & CDR_BRUSH) == CDR_BRUSH)
	{
		BrushTraceInfo  bruInfo;
		bruInfo.Init(pEnvTrc->vStart, pEnvTrc->vDelta, pEnvTrc->vExt);
		if (AABBCollideWithBrush(&bruInfo)&& bruInfo.fFraction < pEnvTrc->fFraction)
		{
			pEnvTrc->fFraction = bruInfo.fFraction;
			pEnvTrc->vHitNormal = bruInfo.ClipPlane.GetNormal();
			pEnvTrc->bStartSolid = bruInfo.bStartSolid;
			pEnvTrc->dwClsFlag = CDR_BRUSH;
#ifdef BACK_VERSION
			pEnvTrc->iUserData1 = bruInfo.iUserData1;
			pEnvTrc->iUserData2 = bruInfo.iUserData2;
#else
			pEnvTrc->iUserData1 = bruInfo.dwUser1;
			pEnvTrc->iUserData2 = bruInfo.dwUser2;
#endif
		}
	}

	
	if ((pEnvTrc->dwCheckFlag & CDR_WATER) == CDR_WATER)
	{
		float fFraction;
		A3DVECTOR3 vWatNormal;
		bool bStart;
		//@todo : TBD: use center or foot? By Kuiwu[10/10/2005]
		if (CollideWithWater(pEnvTrc->vWatStart, pEnvTrc->vDelta, pEnvTrc->bWaterSolid, fFraction, vWatNormal, bStart)
			&& fFraction < pEnvTrc->fFraction)
		{
			pEnvTrc->fFraction = fFraction;
			pEnvTrc->vHitNormal = vWatNormal;
			pEnvTrc->bStartSolid = bStart;
			pEnvTrc->dwClsFlag = CDR_WATER;
// #ifdef CDR_DEBUG
// 			A3DVECTOR3 vHitPos(pEnvTrc->vWatStart + pEnvTrc->vDelta * fFraction);
// 			sprintf(msg, "collide water, fraction %f pos %f %f %f \n", fFraction, vHitPos.x, vHitPos.y, vHitPos.z);
// 			OUTPUT_DEBUG_INFO(msg);
// #endif

		}
	}

	//assert(!pEnvTrc->bStartSolid);
	//return (pEnvTrc->fFraction < 1.0f+ 1E-4f);
	bool bCollide = (pEnvTrc->fFraction < 1.0f+ 1E-4f);
	pEnvTrc->fFraction = a_Min(pEnvTrc->fFraction,  1.0f);

#ifdef CDR_PROFILE
	//DWORD tend = (DWORD)(ACounter::GetMicroSecondNow());
	DWORD tend = (DWORD)(ACounter::GetMicroSecondNow()/1000);
	tsum += (tend - tstart);
	frames++;
	if (frames > 1000)
	{
		char msg[200];
		double  tconsume = tsum / (double)frames; 
		//tconsume /= 1000.0f;
		sprintf(msg, "collide %f ms\n", tconsume);
		OutputDebugStringA(msg);
		frames = 0;
		tsum = 0;
	}
#endif
	return bCollide;
}
/*
 * rescue from solid, currently do NOT handle all in solid. 
 * @desc : It mostly occurred during the ground trace, so here a simple strategy is 
		   applied: only rescue in the y direction (up, more exactly).
 * @param :     
 * @return :
 * @note:  
 * @todo: try to handle all in solid and rescue from different directions.  
 * @author: kuiwu [12/9/2005]
 * @ref: method RetrieveSupportPlane
 */
bool RescueFromSolid(BrushTraceInfo * pTrcInfo)
{
#define MAX_TRY		10
#define STEP_SIZE   0.1f
	int nTry = 0;
	A3DVECTOR3 vStart(pTrcInfo->vStart);
	A3DVECTOR3 vExt(pTrcInfo->vExtents);

	while (nTry < MAX_TRY)
	{
		vStart.y += STEP_SIZE;
		pTrcInfo->Init(vStart, A3DVECTOR3(0.0f), vExt);
		if (!AABBCollideWithBrush(pTrcInfo) ||  !pTrcInfo->bStartSolid)
		{
			break;	
		}
		nTry++;
	}

	if (nTry < MAX_TRY)
	{
		pTrcInfo->Init(vStart, A3DVECTOR3(0.0f, -(MAX_TRY*STEP_SIZE), 0.0f), vExt);
		AABBCollideWithBrush(pTrcInfo);
		return true;
	}

#undef  STEP_SIZE	
#undef  MAX_TRY
	return false;
}


/**
 * \brief adjust pos and retrieve the support plane
 * \param[in]
 * \param[out]
 * \return   false if the adjusting fails.
 * \note  
 * \warning
 * \todo   give me a better name! 
 * \author kuiwu 
 * \date 15/3/2006
 * \see 
 */
bool  AdjustPosNormal(const A3DVECTOR3& pos, const A3DVECTOR3& ext,  const bool bPull, const float fDownTrc, A3DVECTOR3& NewPos, A3DVECTOR3& NewNormal)
{

	NewNormal.Clear();
	ground_trace_t groundTrc;
	groundTrc.vStart = pos;
	groundTrc.vExt = ext;
	//groundTrc.fDeltaY = (bPull)? (fStepHei + SMALL_VALLEY) : (SMALL_VALLEY);
	groundTrc.fDeltaY = fDownTrc;
	if (!RetrieveSupportPlane(&groundTrc))
	{
		return false; 
	}

	if (groundTrc.bSupport)
	{
		NewPos = groundTrc.vEnd;
		NewNormal = groundTrc.vHitNormal;
	}
	else if (bPull)
	{

		NewPos.y -= fDownTrc;
#ifdef CDR_DEBUG
		sprintf(msg, "pull without supporting\n");
		OUTPUT_DEBUG_INFO(msg);
#endif	
	}
	
#ifdef CDR_DEBUG
// 	A3DVECTOR3 vTerpos, vTernorm;
// 	GetTerrainInfo(pos, vTerpos, vTernorm);
// 	sprintf(msg, "ter %f %f %f normal %f %f %f\n", vTerpos.x, vTerpos.y+ ext.y, vTerpos.z,
// 		vTernorm.x, vTernorm.y, vTernorm.z);
// 	OUTPUT_DEBUG_INFO(msg);
#endif
	
	return true;
}


/*
 *
 * @desc :
 * @param :     
 * @return : false if some error occurs during the trace, e.g. start in solid...
 * @note:  ASSUME that ext is positive!!!
 * @todo:   
 * @author: kuiwu [12/9/2005]
 * @ref:
 */
bool  RetrieveSupportPlane(ground_trace_t * pTrc)
{

	//@note : pre-condition. By Kuiwu[19/9/2005]
	assert(pTrc && pTrc->vExt.x > 0.0f 
			&& pTrc->vExt.y >0.0f && pTrc->vExt.z > 0.0f
			&& "invalid input");
	
	env_trace_t  trcInfo;
	trcInfo.dwCheckFlag = CDR_TERRAIN | CDR_BRUSH;
	trcInfo.vExt = pTrc->vExt;

	trcInfo.vStart = pTrc->vStart;
	//trcInfo.vStart.y += pTrc->fBump;  //bump up, similar to hl2
	trcInfo.vDelta = A3DVECTOR3(0.0f, -(pTrc->fDeltaY), 0.0f);
	trcInfo.vTerStart=pTrc->vStart;
	trcInfo.vTerStart.y -= trcInfo.vExt.y; //foot
	bool bClear = !CollideWithEnv(&trcInfo);	

	if (trcInfo.bStartSolid)
	{
		return false;
	}

	if (bClear)
	{
		pTrc->bSupport = false;
		return true;
	}

	pTrc->vEnd = trcInfo.vStart + trcInfo.vDelta* trcInfo.fFraction;
	pTrc->bSupport = true;
	pTrc->vHitNormal = trcInfo.vHitNormal;
	pTrc->iUserData1 = trcInfo.iUserData1;
	pTrc->iUserData2 = trcInfo.iUserData2;

	return true;
}


/*
 * @desc :
 * @param  vHitPos: the hit pos if collision occur, otherwise (vCenter - vDelta).
 * @return : 
 * @note: 
 * @todo:   
 * @author: kuiwu 
 * @ref: 
 */
bool VertAABBTrace(const A3DVECTOR3& vCenter, const A3DVECTOR3& vExt, CDR_TRACE_OUTPUT& Out, float DeltaY/* =100.0f */)
{
	A3DVECTOR3 vTerrainPos, vTerrainNormal;
	GetTerrainInfo(vCenter, vTerrainPos,vTerrainNormal);

	ground_trace_t  groundTrc;

	groundTrc.vStart = vCenter;
	groundTrc.vExt   = vExt;
	groundTrc.fDeltaY = a_Min(vCenter.y - vExt.y - vTerrainPos.y+ 0.5f, DeltaY);
	//groundTrc.fBump = 0.001f;
	if (!RetrieveSupportPlane(&groundTrc) || (!groundTrc.bSupport))
	{
		Out.vHitPos = vTerrainPos;
		Out.vHitPos.y += (vExt.y+ 1E-4f);
		Out.vHitNormal = vTerrainNormal;
		Out.fFaction = -1.0f;	//	Set to invalid value
		return false;
	}

	assert(groundTrc.bSupport);

	Out.vHitPos = groundTrc.vEnd;
	Out.vHitNormal = groundTrc.vHitNormal;
	Out.fFaction = -1.0f;	//	Set to invalid value
	Out.iUserData1 = groundTrc.iUserData1;
	Out.iUserData2 = groundTrc.iUserData2;
	return true;

}

bool AABBTrace(const A3DVECTOR3& vCenter, const A3DVECTOR3& vExt, const A3DVECTOR3& vDelta, A3DVECTOR3& vEndPos, float& fFaction)
{
	env_trace_t tmpInfo;

	tmpInfo.vStart = vCenter;
	tmpInfo.vTerStart = vCenter;
	tmpInfo.vTerStart.y -= vExt.y;
	tmpInfo.vExt = vExt;
	tmpInfo.vDelta = vDelta;
	tmpInfo.dwCheckFlag = CDR_BRUSH | CDR_TERRAIN; 
	bool bClear = !CollideWithEnv(&tmpInfo);

	if (tmpInfo.bStartSolid)
	{
		vEndPos	= vCenter;
		fFaction = 0.0f;
		return true;
	}

	//	If we made it all the way, then copy trace end
	//  as new player position.
	if (bClear)
	{
		vEndPos = vCenter + vDelta;
		fFaction = 1.0f;
		return false;
	}
	else
	{
		fFaction = tmpInfo.fFraction;
		vEndPos = vCenter + vDelta * fFaction;
		return true;
	}
}

/**
 * \brief blocking test with statistics.
 *		  treat x,y,z as independence variable to avoid saving the history and make the 
 *		  computation balanced in every frame.
 * \param[in]
 * \param[out]
 * \return
 * \note
 * \warning
 * \todo   
 * \author kuiwu 
 * \date 5/6/2006
 * \see 
 */

bool BlockTest(const A3DVECTOR3& pos, float dExp)
{
	bool bBlocked = false;


	static  int nFrames = 0;
	static  double expX = 0.0;
	static  double expY = 0.0;
	static  double expZ = 0.0;
	static  double expX2 = 0.0;
	static  double expY2 = 0.0;
	static  double expZ2 = 0.0;
	static  double mean = 0.0f;
	expX += pos.x;
	expY += pos.y;
	expZ += pos.z;
	expX2 += pos.x*pos.x;
	expY2 += pos.y*pos.y;
	expZ2 += pos.z*pos.z;
	//mean += dExp;
	++nFrames;
#define  STATISTIC_FRAMES      20
//#define  BLOCK_FRAC_THRESHOLD       0.05
	if (nFrames >= STATISTIC_FRAMES)
	{
		double varX, varY, varZ;
		double invFrames = 1.0/nFrames;
		expX    *= invFrames;
		expY    *= invFrames;
		expZ    *= invFrames;
		
		expX2    *= invFrames;
		expY2    *= invFrames;
		expZ2    *= invFrames;
		
		//mean     *= invFrames;
		
		varX = expX2 - expX*expX;
		varY = expY2 - expY*expY;
		varZ = expZ2 - expZ*expZ;
		double maxVar = a_Max(varX, varY, varZ);
		//double threshold = mean * BLOCK_FRAC_THRESHOLD;
		double threshold  = 2E-4;
		//a_Clamp(threshold, 1E-4, 2E-4);
		
		bBlocked = (maxVar < threshold);
		
		expX = expY = expZ = 0.0;
		expX2 = expY2 = expZ2 = 0.0;
		mean = 0.0;
		nFrames = 0;
#ifdef CDR_DEBUG
		sprintf(msg, "var %f %f %f threshold %f block %d \n", varX, varY, varZ, threshold, bBlocked);
		OUTPUT_DEBUG_INFO(msg);
#endif
	}


	return bBlocked;

}
#ifdef  CDR_HL2_MOVE

void OnGroundMove(CDR_INFO& CDRInfo)
{
#ifdef CDR_DEBUG
	sprintf(msg, "===================ground move hl2====================\n");
	OUTPUT_DEBUG_INFO(msg);
	sprintf(msg, "in %f %f %f xoz %f %f vel %f %f %f t %f tpn %f %f %f\n",
		  CDRInfo.vCenter.x, CDRInfo.vCenter.y, CDRInfo.vCenter.z, 
		  CDRInfo.vXOZVelDir.x , CDRInfo.vXOZVelDir.z,
		  CDRInfo.vClipVel.x, CDRInfo.vClipVel.y, CDRInfo.vClipVel.z,
		  CDRInfo.t, CDRInfo.vTPNormal.x, CDRInfo.vTPNormal.y,
		  CDRInfo.vTPNormal.z);
	OUTPUT_DEBUG_INFO(msg);
#endif	

#ifdef CDR_PROFILE
	static int frames  = 0;
	static DWORD  tsum = 0;
	//DWORD tstart =	(DWORD)(ACounter::GetMicroSecondNow());
	DWORD tstart =	(DWORD)(ACounter::GetMicroSecondNow()/1000);
#endif

   move_t  mv;
   mv.start = CDRInfo.vCenter;
   mv.ext   = CDRInfo.vExtent;
   mv.tpnormal = CDRInfo.vTPNormal;
   mv.slope    = CDRInfo.fSlopeThresh;
   mv.t        = CDRInfo.t;
   mv.wishdir  = CDRInfo.vXOZVelDir;
   mv.wishspd  = CDRInfo.fSpeed;
   //mv.accel    = 1.5f ;  //tune it. 
   //mv.accel    = 2.0f ;  //tune it. 
   //mv.accel    = 3.0f ;  //tune it. 
   mv.accel    = 10.0f ;  //tune it. 
   //还需要取上一次移动的速度
   mv.velocity = CDRInfo.vClipVel;

   //CheckParameters(mv);
   
   FullGroundMove(mv,  CDRInfo.fGravityAccel, CDRInfo.fStepHeight);
	
	CDRInfo.vClipVel = mv.velocity;
    CDRInfo.fMoveDist = Magnitude(mv.end - CDRInfo.vCenter);
	//bool bStatBlock = BlockTest(CDRInfo.vCenter, 0.05f);
	//CDRInfo.bBlocked =  (CDRInfo.fMoveDist < DIST_EPSILON) || bStatBlock;
	CDRInfo.bBlocked =  (CDRInfo.fMoveDist < DIST_EPSILON);

	CDRInfo.vCenter  = mv.end;
	CDRInfo.vTPNormal = mv.tpnormal;
		

	CDRInfo.vAbsVelocity = mv.absvelocity;


	CDRInfo.bCanStay = CDRInfo.vTPNormal.y >= CDRInfo.fSlopeThresh;
	CDRInfo.bOnSurface = !(CDRInfo.vTPNormal.MaxMember() < NORMAL_EPSILON 
						 && CDRInfo.vTPNormal.MinMember() > -NORMAL_EPSILON);
	
	if (CDRInfo.bBlocked)
	{
		CDRInfo.vClipVel.Clear();
	}

#ifdef CDR_PROFILE
	//DWORD tend = (DWORD)(ACounter::GetMicroSecondNow());
	DWORD tend = (DWORD)(ACounter::GetMicroSecondNow()/1000);
	tsum += (tend - tstart);
	frames++;
	if (frames > 1000)
	{
		char msg[200];
		double  tconsume = tsum / (double)frames; 
		//tconsume /= 1000.0f;
		sprintf(msg, "new %f ms\n", tconsume);
		OutputDebugStringA(msg);
		frames = 0;
		tsum = 0;
	}
	

#endif

#ifdef CDR_DEBUG
 	sprintf(msg, "out %f %f %f tpn %f %f %f vel %f %f %f move %f block %d stay %d surface %d\n", 
		CDRInfo.vCenter.x, CDRInfo.vCenter.y, CDRInfo.vCenter.z,  
		CDRInfo.vTPNormal.x, CDRInfo.vTPNormal.y, CDRInfo.vTPNormal.z, 
		CDRInfo.vClipVel.x, CDRInfo.vClipVel.y, CDRInfo.vClipVel.z,
					 CDRInfo.fMoveDist, CDRInfo.bBlocked, CDRInfo.bCanStay, CDRInfo.bOnSurface);
 	OUTPUT_DEBUG_INFO(msg);
	//assert(!IsInWater(CDRInfo.vCenter, CDRInfo.vExtent));
#endif
}


#else
void OnGroundMove(CDR_INFO& CDRInfo)
{
#ifdef CDR_DEBUG
	sprintf(msg, "=====================ground move======================\n");
	OUTPUT_DEBUG_INFO(msg);
	sprintf(msg, "in %f %f %f xozspeed %f %f  yspeed %f t %f tpnormal %f %f %f\n",
		  CDRInfo.vCenter.x, CDRInfo.vCenter.y, CDRInfo.vCenter.z, CDRInfo.vXOZVelDir.x * CDRInfo.fSpeed, CDRInfo.vXOZVelDir.z * CDRInfo.fSpeed,
		  CDRInfo.fYVel, CDRInfo.t, CDRInfo.vTPNormal.x, CDRInfo.vTPNormal.y,
		  CDRInfo.vTPNormal.z);
	OUTPUT_DEBUG_INFO(msg);
#endif	


	if ((CDRInfo.fYVel < VEL_EPSILON) && (CDRInfo.fYVel > -VEL_EPSILON) 
		&& (CDRInfo.fSpeed < VEL_EPSILON) && (CDRInfo.fSpeed > -VEL_EPSILON) )
	{
		CDRInfo.fYVel -= CDRInfo.fGravityAccel * CDRInfo.t;

#ifdef CDR_DEBUG
		sprintf(msg, "no speed\n");
		OUTPUT_DEBUG_INFO(msg);
#endif	
		return;
	}

#ifdef CDR_PROFILE
	static int frames  = 0;
	static DWORD  tsum = 0;
	DWORD tstart =	(DWORD)(ACounter::GetMicroSecondNow());
#endif

	float expMove; 
 	SlideMove(CDRInfo, expMove);
	//bool bStatBlock = BlockTest(CDRInfo.vCenter, expMove);
	//CDRInfo.bBlocked =  (CDRInfo.fMoveDist < DIST_EPSILON) || bStatBlock;
	CDRInfo.bBlocked =  (CDRInfo.fMoveDist < DIST_EPSILON);
 	if (CDRInfo.bBlocked)
 	{
 		CDRInfo.fYVel = 0.0f;
 	}

	CDRInfo.bCanStay = CDRInfo.vTPNormal.y >= CDRInfo.fSlopeThresh;
	CDRInfo.bOnSurface = !(CDRInfo.vTPNormal.MaxMember() < NORMAL_EPSILON 
						 && CDRInfo.vTPNormal.MinMember() > -NORMAL_EPSILON);
#ifdef CDR_PROFILE
	DWORD tend = (DWORD)(ACounter::GetMicroSecondNow());
	tsum += (tend - tstart);
	frames++;
	if (frames > 1000)
	{
		char msg[200];
		double  tconsume = tsum / (double)frames; 
		tconsume /= 1000.0f;
		sprintf(msg, "old %f ms\n", tconsume);
		OutputDebugStringA(msg);
		frames = 0;
		tsum = 0;
	}
	

#endif
	
#ifdef CDR_DEBUG
 	sprintf(msg, "out %f %f %f tpnormal %f %f %f fyvel %f move %f block %d\n", CDRInfo.vCenter.x, CDRInfo.vCenter.y, CDRInfo.vCenter.z,  CDRInfo.vTPNormal.x, CDRInfo.vTPNormal.y, CDRInfo.vTPNormal.z, CDRInfo.fYVel, CDRInfo.fMoveDist, CDRInfo.bBlocked);
 	OUTPUT_DEBUG_INFO(msg);
	//assert(!IsInWater(CDRInfo.vCenter, CDRInfo.vExtent));
#endif
}


/*
 * add terrain-collision detection 
 * @desc :
 * @param :     
 * @return :
 * @note: 
 * @todo:  
 * @author: kuiwu [29/9/2005]
 * @history : Add moving dist. By Kuiwu[13/12/2005]
 * @ref: 
 */
void SlideMove(CDR_INFO& CDRInfo, float& expMove)
{
	//assert(0);


#define  MAX_TRY         4
#define  VEL_REFLECT     0.3f	



	//test clear gravity
	//CDRInfo.fGravityAccel = 0.0f;


	A3DVECTOR3 vNormal(CDRInfo.vTPNormal);
	bool bFreeFall = (vNormal.y < CDRInfo.fSlopeThresh);

	//clear move dist
	CDRInfo.fMoveDist = 0.0f;

	float fYVel = CDRInfo.fYVel;  //save the y velocity;

	//@todo : refine the speed to determine the jumping state. By Kuiwu[14/9/2005]
	bool bJump = (fYVel > 0.5f);

	A3DVECTOR3 vVelocity(CDRInfo.fSpeed*CDRInfo.vXOZVelDir + fYVel*g_vAxisY );
	
	
	if (bFreeFall) 
	{
		vVelocity -= (CDRInfo.fGravityAccel* CDRInfo.t *g_vAxisY);
		fYVel -= CDRInfo.fGravityAccel* CDRInfo.t;
	}
	A3DVECTOR3 vVelDir(vVelocity);
	float fVelSpeed = vVelDir.Normalize();
	if (!bFreeFall)
	{
		a_ClampRoof(fVelSpeed, VEL_MAX_SPEED);
	}
	vVelocity = vVelDir * fVelSpeed;
	//@note : clip the velocity or dir? By Kuiwu[8/9/2005]
	float dtp = DotProduct(vVelDir, vNormal);
	if (dtp < 0.0f || !bJump)
	{	
		vVelocity = (vVelDir - CDRInfo.vTPNormal * dtp - CDRInfo.vTPNormal*dtp * 0.01f) * fVelSpeed;
		//vVelocity = (vVelDir - vNormal * dtp ) * fVelSpeed;
	}


	CDRInfo.vAbsVelocity=vVelocity;	

	A3DVECTOR3 vStart(CDRInfo.vCenter);
	A3DVECTOR3 vExt(CDRInfo.vExtent);
	float  fTime = CDRInfo.t;
	A3DVECTOR3 vDelta;
	bool bClear = true; 
	int nTry = 0;
	env_trace_t  trcInfo;
	trcInfo.dwCheckFlag = CDR_TERRAIN | CDR_BRUSH;
	trcInfo.vExt = vExt;
	A3DVECTOR3  vFinalPos;
	bool bPull = false;
	bool bTryPull = false;
	float fDownTrc = 0.01f;  //used to retrieve support plane

	expMove = fVelSpeed * fTime;  //set the expected move dist

	while (nTry < MAX_TRY) 
	{
		vFinalPos = vStart;

		// @note: adjust the y vel to prevent moving to steep slope.  By kuiwu. [30/5/2006]
		if (vNormal.y < CDRInfo.fSlopeThresh && vVelocity.y > 0.0f && !bJump)
		{
			vVelocity.y = 0.0f;
		}

// 		dtp = DotProduct(vVelocity, CDRInfo.vAbsVelocity);
// #ifdef CDR_DEBUG
// 		sprintf(msg, "vel dtp %f \n", dtp);
// 		OUTPUT_DEBUG_INFO(msg);
// #endif
// 		if (dtp <= 0.0f)
// 		{
// 			break;
// 		}
		vDelta = vVelocity * fTime;
		float fDeltaDist = vDelta.Magnitude();
		if (fDeltaDist < DIST_EPSILON ) 
		{
#ifdef CDR_DEBUG
			sprintf(msg, "no delta %f\n", fDeltaDist);
			OUTPUT_DEBUG_INFO(msg);
#endif
			break;
		}

		trcInfo.vStart = vStart;
		trcInfo.vDelta = vDelta;
		trcInfo.vTerStart = vStart;
		trcInfo.vTerStart.y -= vExt.y; //foot
		bClear = !CollideWithEnv(&trcInfo);	
#ifdef CDR_DEBUG
		sprintf(msg, "start %f %f %f delta %f %f %f frac %f nomal %f %f %f bStart %d\n", 
				vStart.x,  vStart.y, vStart.z,
				vDelta.x, vDelta.y, vDelta.z,
				trcInfo.fFraction, trcInfo.vHitNormal.x, trcInfo.vHitNormal.y, trcInfo.vHitNormal.z, trcInfo.bStartSolid);
		OUTPUT_DEBUG_INFO(msg);
#endif
		++nTry;

		if (bClear || trcInfo.bStartSolid) 
		{
			vFinalPos = vStart + vDelta;
			break;
		}
// #define    CDR_SAFE_FRAC   0.0001f		
// 		if (trcInfo.fFraction < CDR_SAFE_FRAC)
// 		{
// #ifdef CDR_DEBUG
// 			sprintf(msg, "move too small\n");
// 			OUTPUT_DEBUG_INFO(msg);
// #endif
// 			break;
// 		}
// 		trcInfo.fFraction -= CDR_SAFE_FRAC;

		vStart += vDelta*trcInfo.fFraction;
		vFinalPos = vStart;
		fTime -= fTime*trcInfo.fFraction;
		vNormal = trcInfo.vHitNormal;
		if (!bFreeFall && !bTryPull && !bJump )
		{
			env_trace_t tmpInfo;
			tmpInfo.vStart = vStart;
			tmpInfo.vDelta = A3DVECTOR3(0.0f, CDRInfo.fStepHeight, 0.0f);
			tmpInfo.vExt = vExt;
			//@note : need check terrain?? By Kuiwu[8/10/2005]
			tmpInfo.dwCheckFlag = CDR_BRUSH | CDR_TERRAIN;  
			tmpInfo.vTerStart = vStart;
			tmpInfo.vTerStart.y -= vExt.y;

			bPull = !CollideWithEnv(&tmpInfo);
			if (bPull) 
			{
				vStart.y += CDRInfo.fStepHeight;
				//vDelta = vVelocity * fTime;
				vDelta = vVelocity ;
				tmpInfo.vStart =vStart;
				tmpInfo.vDelta = vDelta;
				tmpInfo.vTerStart = vStart;
				tmpInfo.vTerStart.y -= vExt.y;
				bool bMove = !CollideWithEnv(&tmpInfo);
				if (!bMove)
				{
					vDelta *= tmpInfo.fFraction;
				}
				
				if (vDelta.SquaredMagnitude() < vExt.x* vExt.x *4 )
				{
					vStart.y -= CDRInfo.fStepHeight;
					bPull = false;
				}
				else //pull up 
				{
					fDownTrc = CDRInfo.fStepHeight + 0.01f;  //pull down later.
				}
			}
			bTryPull = true; 
	
		}

		if (!bPull) 
		{

			fVelSpeed = Normalize(vVelocity, vVelDir);
			fVelSpeed *= (1- nTry * 0.1f);
			dtp = DotProduct(vNormal, vVelDir);

#ifdef CDR_DEBUG
			sprintf(msg, "vel %f %f %f dtp %f", vVelocity.x, vVelocity.y, vVelocity.z, dtp);
			OUTPUT_DEBUG_INFO(msg);
#endif


			float fRelSpeed = a_Min(fVelSpeed, 5.0f);
			
			
			//if(( dtp < 1E-4f) && (dtp >= 0.0f)  )
			if(vNormal.y < CDRInfo.fSlopeThresh)
			{
				//vVelocity += vNormal* VEL_REFLECT * fRelSpeed;
				vVelocity = (vVelDir - vNormal * dtp ) * fVelSpeed  + vNormal * 0.03f * fRelSpeed; 
				//fDownTrc = 0.1f;
			}
			else
			{
				vVelocity = (vVelDir - vNormal * dtp ) * fVelSpeed  - vNormal *dtp * VEL_REFLECT * fRelSpeed; 
				//vVelocity = (vVelDir - vNormal * dtp - vNormal*dtp * VEL_REFLECT) * fVelSpeed; 
			}
// 			if (dtp < 0.0f)
// 			{
// 				vVelocity = (vVelDir - vNormal * dtp ) * fVelSpeed;
// 			}

#ifdef CDR_DEBUG
			Normalize(vVelocity, vVelDir);
			dtp = DotProduct(vNormal, vVelDir);
			sprintf(msg, " new %f %f %f dtp %f \n", vVelocity.x, vVelocity.y, vVelocity.z, dtp);
			OUTPUT_DEBUG_INFO(msg);
#endif



			//CDRInfo.fYVel = vVelocity.y;
			if (fYVel > VEL_EPSILON) 
			{
				if ((vNormal.y >= CDRInfo.fSlopeThresh || vNormal.y < -NORMAL_EPSILON))
				{
					fYVel = 0.0f;
				}
			} 
			else if(fYVel < -VEL_EPSILON)
			{
				if( (vNormal.y >= CDRInfo.fSlopeThresh))
				{
					fYVel = 0.0f;
					fDownTrc = 0.2f;  //expect a support plane
				}
			}
			else
			{
				//@note : additional handle something???  By Kuiwu[13/9/2005]
			}
		}

	}

#ifdef CDR_DEBUG
	sprintf(msg, "before adjust %f %f %f fYVel %f\n", vFinalPos.x, vFinalPos.y, vFinalPos.z, fYVel);
	OUTPUT_DEBUG_INFO(msg);
#endif
	//@note : prevent moving to the invalid area. By Kuiwu[20/9/2005]
	//if (IsInWater(vFinalPos, vExt) || !IsPosInAvailableMap(vFinalPos))
	if ( !IsPosInAvailableMap(vFinalPos))
	{

#ifdef CDR_DEBUG
	sprintf(msg, "unavailable pos\n");
	OUTPUT_DEBUG_INFO(msg);
#endif
		return;
	}


	A3DVECTOR3 vTPNormal;
	vTPNormal.Clear();
	if (!bJump)
	{
		A3DVECTOR3 vTempPos(vFinalPos);
		if (!AdjustPosNormal(vTempPos, vExt, bPull, fDownTrc, vFinalPos, vTPNormal))
		{//do NOT change position
			return ;
		}
		if ((fYVel > 0.0f) ||(vTPNormal.y >= CDRInfo.fSlopeThresh && fYVel < 0.0f))
		{
			fYVel = 0.0f;
		}
	}
	vDelta = vFinalPos - CDRInfo.vCenter;
	CDRInfo.fMoveDist = vDelta.Magnitude();
	CDRInfo.bBlocked = CDRInfo.fMoveDist < DIST_EPSILON ;

	CDRInfo.vCenter = vFinalPos;
	CDRInfo.fYVel =	fYVel;  
	CDRInfo.vTPNormal = vTPNormal;


	
#undef  MAX_TRY
#undef  VEL_REFLECT

}
#endif

//void OnGroundMove(CDR_INFO& CDRInfo)
//{
//
//	
//	//OnGroundMoveBigAABB(CDRInfo);
//	OnGroundMoveBigAABB2(CDRInfo);
//}



/*
 *
 * @desc :
 * @param :     
 * @return :
 * @note:
 * @todo:   
 * @author: kuiwu [10/10/2005]
 * @ref:
 */
void AirMove(ON_AIR_CDR_INFO& awmInfo)
{

#define  DIST_EPSILON    1e-4f
#define  MAX_TRY         4
#define  VEL_REFLECT     0.0f

#ifdef CDR_DEBUG
	sprintf(msg, "=====================air move======================\n");
	OUTPUT_DEBUG_INFO(msg);
	sprintf(msg, "input center %f %f %f veldir %f %f %f speed %f t %f \n", 
			awmInfo.vCenter.x, awmInfo.vCenter.y, awmInfo.vCenter.z,
			awmInfo.vVelDir.x, awmInfo.vVelDir.y, awmInfo.vVelDir.z,
			awmInfo.fSpeed, awmInfo.t);
	OUTPUT_DEBUG_INFO(msg);
#endif
	//@note : pre-condition. By Kuiwu[20/9/2005]
	assert(awmInfo.fSpeed >= 0.0f && awmInfo.t >= 0.0f && "invalid input");

	float fTime = awmInfo.t;
	//@todo : is it necessary to clamp the speed? By Kuiwu[20/9/2005]
	float fSpeed = awmInfo.fSpeed;
	if (fSpeed * fTime < DIST_EPSILON)
	{
		//@todo : set the output param. By Kuiwu[20/9/2005]
		return;
	}

	A3DVECTOR3 vStart(awmInfo.vCenter);
	A3DVECTOR3 vExt(awmInfo.vExtent);
	A3DVECTOR3 vVelDir(awmInfo.vVelDir);
	float dtp;
	A3DVECTOR3 vVelocity(vVelDir * fSpeed);

	if ((dtp = DotProduct(vVelDir, awmInfo.vTPNormal)) < 0.0f)
	{
		
		//vVelocity = (vVelDir - awmInfo.vTPNormal * dtp - awmInfo.vTPNormal*dtp * 0.01f) * fSpeed;
		vVelocity = (vVelDir - awmInfo.vTPNormal * dtp ) * fSpeed;

#ifdef CDR_DEBUG
		sprintf(msg, "dtp %f clipvel %f %f %f supportnormal %f %f %f\n", 
			dtp, vVelocity.x, vVelocity.y, vVelocity.z,
			awmInfo.vTPNormal.x, awmInfo.vTPNormal.y, awmInfo.vTPNormal.z);
		OUTPUT_DEBUG_INFO(msg);
#endif

	}

	A3DVECTOR3 vDelta(vVelocity*fTime), vNormal, vFinalPos(vStart);
	int nTry = 0;
	bool bClear = true;
	env_trace_t  trcInfo;
	trcInfo.bWaterSolid = true;
	trcInfo.dwCheckFlag = CDR_TERRAIN | CDR_BRUSH | CDR_WATER;
	trcInfo.vExt = vExt;

	while (nTry < MAX_TRY) 
	{
		if (vDelta.SquaredMagnitude() < DIST_EPSILON ) 
		{
			break;
		}
		trcInfo.vStart = vStart;
		trcInfo.vDelta = vDelta;
		trcInfo.vTerStart = vStart;
		trcInfo.vTerStart.y -= vExt.y;
		trcInfo.vWatStart = vStart;
		trcInfo.vWatStart.y -= vExt.y;
		bClear = !CollideWithEnv(&trcInfo);	

		++nTry;
		if (bClear || trcInfo.bStartSolid) 
		{
			vFinalPos = vStart + vDelta;
			break;
		}
		vStart += vDelta*trcInfo.fFraction;
		vFinalPos = vStart;
		fTime -= fTime*trcInfo.fFraction;
		vNormal = trcInfo.vHitNormal;
		fSpeed = Normalize(vVelocity, vVelDir);
		fSpeed *= (1- nTry * 0.1f);
		dtp = DotProduct(vNormal, vVelDir);
		vVelocity = (vVelDir - vNormal * dtp - vNormal*dtp * VEL_REFLECT) * fSpeed; 
		vDelta = vVelocity * fTime;

	}

#ifdef CDR_DEBUG
	sprintf(msg, "after collision detect %f %f %f bClear %d nTry %d\n", vFinalPos.x, vFinalPos.y, vFinalPos.z, bClear, nTry);
	OUTPUT_DEBUG_INFO(msg);
#endif
	//@note : prevent moving to the invalid area. By Kuiwu[20/9/2005]
	if (!IsPosInAvailableMap(vFinalPos))
	{
		//@todo : set some flag to notify the caller? By Kuiwu[20/9/2005]
		return;
	}

	//too high
	if (vFinalPos.y > _max_fly_height  - 2.0f)
	{
		return;
	}

	//see if meet height thresh
	ground_trace_t groundTrc;
	groundTrc.vStart = vFinalPos;
	groundTrc.vExt = vExt;
	groundTrc.fDeltaY = awmInfo.fHeightThresh + 0.1f;
	
	if (!RetrieveSupportPlane(&groundTrc))
	{
#ifdef CDR_DEBUG
		OUTPUT_DEBUG_INFO("retrive support plane fail\n");
		//assert(0 && "retrive support plane fail");
#endif
		return;
	}

	A3DVECTOR3 vTpNormal(0.0f);
	A3DVECTOR3 vOverTp(vFinalPos);
	bool bAdjust = false;
	awmInfo.bMeetHeightThresh = true;

	float fHWater = g_pGame->GetGameRun()->GetWorld()->GetWaterHeight(vFinalPos);
	if (groundTrc.bSupport)
	{
		bAdjust = true;
		vOverTp = groundTrc.vEnd;
		vTpNormal = groundTrc.vHitNormal;
		if (fHWater > groundTrc.vEnd.y)
		{
			vOverTp.y = fHWater;
			vTpNormal = g_vAxisY;
		}
	}
	else if (vFinalPos.y < fHWater + awmInfo.fHeightThresh )
	{
		bAdjust = true;
		vOverTp = vFinalPos;
		vOverTp.y = fHWater;
		vTpNormal = g_vAxisY;
	}
	
	if (bAdjust &&  (vOverTp.y + awmInfo.fHeightThresh > vFinalPos.y) )
	{
		BrushTraceInfo  brushTrc;
		vDelta.Clear();
		vDelta.y = vOverTp.y + awmInfo.fHeightThresh - vFinalPos.y;
		float fAllow = (float)fabs(awmInfo.vCenter.y - vFinalPos.y)+ 0.001f;
		fAllow = a_Max(fAllow, 0.15f);
		a_ClampRoof(vDelta.y, fAllow);
#ifdef CDR_DEBUG
		sprintf(msg, "adjust %f\n", vDelta.y);
		OUTPUT_DEBUG_INFO(msg);
#endif

		brushTrc.Init(vFinalPos, vDelta, vExt);
		if (AABBCollideWithBrush(&brushTrc))
		{
			vFinalPos += (vDelta * brushTrc.fFraction);
		}
		else
		{
			vFinalPos += vDelta;
		}
		awmInfo.bMeetHeightThresh = (vFinalPos.y - vOverTp.y > awmInfo.fHeightThresh);
	}
	
	awmInfo.vCenter = vFinalPos;
	awmInfo.vTPNormal = vTpNormal;

#ifdef CDR_DEBUG
	sprintf(msg, "out center %f %f %f tpnormal %f %f %f meetthresh %d\n", awmInfo.vCenter.x, awmInfo.vCenter.y, awmInfo.vCenter.z, awmInfo.vTPNormal.x, awmInfo.vTPNormal.y, awmInfo.vTPNormal.z, awmInfo.bMeetHeightThresh);
	OUTPUT_DEBUG_INFO(msg);
#endif	

	
#undef  DIST_EPSILON
#undef  MAX_TRY
#undef  VEL_REFLECT
}

/*
 *
 * @desc :
 * @param :     
 * @return :
 * @note:
 * @todo:   
 * @author: kuiwu [10/10/2005]
 * @ref:
 */
void WaterMove(ON_AIR_CDR_INFO& awmInfo)
{
#define  DIST_EPSILON    1e-4f
#define  MAX_TRY         4
#define  VEL_REFLECT     0.1f

#ifdef CDR_DEBUG
	sprintf(msg, "=====================water move======================\n");
	OUTPUT_DEBUG_INFO(msg);
	sprintf(msg, "input center %f %f %f veldir %f %f %f speed %f t %f \n", 
			awmInfo.vCenter.x, awmInfo.vCenter.y, awmInfo.vCenter.z,
			awmInfo.vVelDir.x, awmInfo.vVelDir.y, awmInfo.vVelDir.z,
			awmInfo.fSpeed, awmInfo.t);
	OUTPUT_DEBUG_INFO(msg);
#endif
	//@note : pre-condition. By Kuiwu[20/9/2005]
	assert(awmInfo.fSpeed >= 0.0f && awmInfo.t >= 0.0f && "invalid input");

	float fTime = awmInfo.t;
	//@todo : is it necessary to clamp the speed? By Kuiwu[20/9/2005]
	float fSpeed = awmInfo.fSpeed;
	if (fSpeed * fTime < DIST_EPSILON)
	{
		//@todo : set the output param. By Kuiwu[20/9/2005]
		return;
	}

	A3DVECTOR3 vStart(awmInfo.vCenter);
	A3DVECTOR3 vExt(awmInfo.vExtent);
	A3DVECTOR3 vVelDir(awmInfo.vVelDir);
	float dtp;
	A3DVECTOR3 vVelocity(vVelDir * fSpeed);

	if ((dtp = DotProduct(vVelDir, awmInfo.vTPNormal)) < 0.0f)
	{
		
		vVelocity = (vVelDir - awmInfo.vTPNormal * dtp - awmInfo.vTPNormal*dtp * 0.01f) * fSpeed;
		//vVelocity = (vVelDir - awmInfo.vTPNormal * dtp ) * fSpeed;

#ifdef CDR_DEBUG
		sprintf(msg, "dtp %f clipvel %f %f %f supportnormal %f %f %f\n", 
			dtp, vVelocity.x, vVelocity.y, vVelocity.z,
			awmInfo.vTPNormal.x, awmInfo.vTPNormal.y, awmInfo.vTPNormal.z);
		OUTPUT_DEBUG_INFO(msg);
#endif

	}

	A3DVECTOR3 vDelta(vVelocity*fTime), vNormal, vFinalPos(vStart);
	int nTry = 0;
	bool bClear = true;
	env_trace_t  trcInfo;
	trcInfo.bWaterSolid = false;
	trcInfo.dwCheckFlag = CDR_TERRAIN | CDR_BRUSH | CDR_WATER ;
	trcInfo.vExt = vExt;

	while (nTry < MAX_TRY) 
	{
		if (vDelta.SquaredMagnitude() < DIST_EPSILON ) 
		{
			break;
		}
		trcInfo.vStart = vStart;
		trcInfo.vDelta = vDelta;
		trcInfo.vTerStart = vStart;
		trcInfo.vTerStart.y -= vExt.y;
		trcInfo.vWatStart = vStart;
		trcInfo.vWatStart.y += awmInfo.fUnderWaterDistThresh; //shoulder


		bClear = !CollideWithEnv(&trcInfo);	

		++nTry;
		if (bClear || (trcInfo.bStartSolid && ((trcInfo.dwClsFlag & CDR_WATER)!= CDR_WATER)) )
		{
			vFinalPos = vStart + vDelta;


			break;
		}
		vStart += vDelta*trcInfo.fFraction;
		fTime -= fTime*trcInfo.fFraction;
		fSpeed = Normalize(vVelocity, vVelDir);
		fSpeed *= (1- nTry * 0.1f);
		if ((trcInfo.dwClsFlag & CDR_WATER) == CDR_WATER)
		{
			if (trcInfo.bStartSolid)
			{//rescue from solid
			 //@note : it may cause some problems. By Kuiwu[11/10/2005]
	  		 float fHWater = g_pGame->GetGameRun()->GetWorld()->GetWaterHeight(vStart);
			 vStart.y = fHWater;
			}
			vVelDir.y = 0.0f;
			vVelocity = vVelDir * fSpeed;
		}
		else
		{
			vNormal = trcInfo.vHitNormal;
			dtp = DotProduct(vNormal, vVelDir);
			vVelocity = (vVelDir - vNormal * dtp - vNormal*dtp * VEL_REFLECT) * fSpeed; 
		}
		vDelta = vVelocity * fTime;
		vFinalPos = vStart;

	}

#ifdef CDR_DEBUG
	sprintf(msg, "after collision detect %f %f %f bClear %d nTry %d\n", vFinalPos.x, vFinalPos.y, vFinalPos.z, bClear, nTry);
	OUTPUT_DEBUG_INFO(msg);
#endif
	//@note : prevent moving to the invalid area. By Kuiwu[20/9/2005]
	if (!IsPosInAvailableMap(vFinalPos))
	{
		//@todo : set some flag to notify the caller? By Kuiwu[20/9/2005]
		return;
	}
	

	//see if meet height thresh
	ground_trace_t groundTrc;
	groundTrc.vStart = vFinalPos;
	groundTrc.vExt = vExt;
	groundTrc.fDeltaY = awmInfo.fHeightThresh + 0.1f;
	
	if (!RetrieveSupportPlane(&groundTrc))
	{
#ifdef CDR_DEBUG
		OUTPUT_DEBUG_INFO("retrive support plane fail\n");
		assert(0 && "retrive support plane fail");
#endif
		return;
	}

	A3DVECTOR3 vTpNormal(0.0f);
	A3DVECTOR3 vOverTp(vFinalPos);
	bool bAdjust = false;
	awmInfo.bMeetHeightThresh = true;

	if (groundTrc.bSupport)
	{
		bAdjust = true;
		vOverTp = groundTrc.vEnd;
		vTpNormal = groundTrc.vHitNormal;
	}

	if (bAdjust &&  (vOverTp.y + awmInfo.fHeightThresh > vFinalPos.y) )
	{
		float fHWater = g_pGame->GetGameRun()->GetWorld()->GetWaterHeight(vFinalPos);
		float fAllow = fHWater - awmInfo.fUnderWaterDistThresh - vFinalPos.y;
		if (fAllow > 1E-4f)
		{
			vDelta.Clear();
			vDelta.y = vOverTp.y + awmInfo.fHeightThresh - vFinalPos.y;
			fAllow = a_Min(fAllow, 0.15f);
			a_ClampRoof(vDelta.y, fAllow);
			BrushTraceInfo  brushTrc;
#ifdef CDR_DEBUG
		sprintf(msg, "adjust %f\n", vDelta.y);
		OUTPUT_DEBUG_INFO(msg);
#endif
			brushTrc.Init(vFinalPos, vDelta, vExt);
			if (AABBCollideWithBrush(&brushTrc))
			{
				vFinalPos += (vDelta * brushTrc.fFraction);
			}
			else
			{
				vFinalPos += vDelta;
			}
			awmInfo.bMeetHeightThresh = (vFinalPos.y - vOverTp.y > awmInfo.fHeightThresh);
		}
	}
	
	awmInfo.vCenter = vFinalPos;
	awmInfo.vTPNormal = vTpNormal;

#ifdef CDR_DEBUG
	sprintf(msg, "out center %f %f %f tpnormal %f %f %f meetthresh %d\n", awmInfo.vCenter.x, awmInfo.vCenter.y, awmInfo.vCenter.z, awmInfo.vTPNormal.x, awmInfo.vTPNormal.y, awmInfo.vTPNormal.z, awmInfo.bMeetHeightThresh);
	OUTPUT_DEBUG_INFO(msg);
#endif	

	
#undef  DIST_EPSILON
#undef  MAX_TRY
#undef  VEL_REFLECT
}
/*
 * 
 * @desc : refactor version of OnAirMoveBigAABB, add collision detect with water & terrain
 * @param :     
 * @return :
 * @note:
 * @todo:   
 * @author: kuiwu [30/9/2005]
 * @ref:
 */
void OnAirMove(ON_AIR_CDR_INFO& awmInfo)
{
	//assert(0 && "Not ready yet");

	if (awmInfo.bOnAir)
	{
		AirMove(awmInfo);
	}
	else
	{
		WaterMove(awmInfo);
	}
}

//void OnAirMove(ON_AIR_CDR_INFO& OnAirCDRInfo)
//{
//	
//	//OnAirMoveBigAABB(OnAirCDRInfo);
//	OnAirMoveBigAABB2(OnAirCDRInfo);
//}

void OtherPlayerMove(OtherPlayer_Move_Info& OPMoveInfo)
{
	A3DVECTOR3 vDelta = OPMoveInfo.t * OPMoveInfo.vVelocity;
	OPMoveInfo.vCenter += vDelta;

	OPMoveInfo.vecGroundNormal = g_vAxisY;
	A3DVECTOR3 vGroundPos, vNormal;

	//	Now, we directly interpolate the pos, and we don't use bTraceGround
	if (OPMoveInfo.bTestTrnOnly)
	{
		GetTerrainInfo(OPMoveInfo.vCenter, vGroundPos, vNormal);
		vGroundPos.y += OPMoveInfo.vExts.y;
	
		if (OPMoveInfo.vCenter.y < vGroundPos.y + 0.1f)
			OPMoveInfo.vecGroundNormal = vNormal;

		//	verify not below the terrain
		if (OPMoveInfo.vCenter.y < vGroundPos.y)
			OPMoveInfo.vCenter.y = vGroundPos.y;
	}
	else
	{
		if (VertRayTrace(OPMoveInfo.vCenter, vGroundPos, vNormal, 3.0f))
		{
			OPMoveInfo.vecGroundNormal = vNormal;

			vGroundPos.y += OPMoveInfo.vExts.y;
		
			//	verify not below the ground
			if (OPMoveInfo.vCenter.y < vGroundPos.y)
				OPMoveInfo.vCenter.y = vGroundPos.y;
		}
	}
}

bool RayTrace(const A3DVECTOR3& vStart, const A3DVECTOR3& vDelta, A3DVECTOR3& vHitPos, A3DVECTOR3& vNormal, float& fraction)
{
	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();
	A3DTerrain2* pTerrain = pWorld->GetTerrain();
	RAYTRACERT rt;
	A3DVECTOR3 vCarHitPos, vEnvHitPos, vCarNormal, vEnvNormal;
	float fCarFrac = 1.0f, fEnvFrac = 1.0f;
	rt.fFraction = 1.0f;

	bool bTraceCarrier = RayTraceCarrier(vStart, vDelta, vCarHitPos, vCarNormal, fCarFrac);
	bool bTraceEnv = RayTraceEnv(vStart, vDelta, vEnvHitPos, vEnvNormal, fEnvFrac);
	bool bTraceTerrain = pTerrain->RayTrace(vStart, vDelta, 1.0f, &rt);

	if (!bTraceCarrier && !bTraceEnv && !bTraceTerrain)
		return false;

	//	collision happens
	if (bTraceCarrier)
	{
		if (bTraceEnv && fEnvFrac < fCarFrac)
		{
			vHitPos = vEnvHitPos;
			vNormal = vEnvNormal;
			fraction = fEnvFrac;
		}
		else
		{
			vHitPos = vCarHitPos;
			vNormal = vCarNormal;
			fraction = fCarFrac;
		}

		if (bTraceTerrain && rt.fFraction < fraction)
		{
			vHitPos = rt.vHitPos;
			vNormal = rt.vNormal;
			fraction = rt.fFraction;
		}
	}
	else if (bTraceEnv)
	{
		if (bTraceTerrain && rt.fFraction < fEnvFrac)
		{
			vHitPos = rt.vHitPos;
			vNormal = rt.vNormal;
			fraction = rt.fFraction;
		}
		else
		{
			vHitPos = vEnvHitPos;
			vNormal = vEnvNormal;
			fraction = fEnvFrac;
		}
	}
	else	//	Only hit terrain
	{
		vHitPos = rt.vHitPos;
		vNormal = rt.vNormal;
		fraction = rt.fFraction;
	}

	return true;	
}


/**
 * \brief check whether the pos is in water. 
 * \param[in]
 * \param[out]
 * \return
 * \note	"Football"  Rule!
 * \warning
 * \todo   
 * \author kuiwu 
 * \date 24/3/2006
 * \see 
 */
bool IsInWater(const A3DVECTOR3& pos, const A3DVECTOR3& ext)
{
	float fHWater = g_pGame->GetGameRun()->GetWorld()->GetWaterHeight(pos);
	
	CDR_TRACE_OUTPUT out;
	VertAABBTrace(pos, ext, out, 10000.0f);
#define OVER_GROUND 0.5f

#ifdef CDR_DEBUG

	if (fHWater > out.vHitPos.y + OVER_GROUND )
	{
		_OutputDebugInfo("inwater %f pos %f %f %f  hit %f %f %f", fHWater, pos.x, pos.y, pos.z,
			 out.vHitPos.x, out.vHitPos.y, out.vHitPos.z);
	}
#endif

	return  (fHWater > out.vHitPos.y + OVER_GROUND );
#undef  OVER_GROUND		
}

#ifdef  CDR_HL2_MOVE
 
/**
 * \brief 
 * \param[in]
 * \param[out]
 * \return
 * \note 与hl2的做法不同，需要考虑dot(in, normal) > 0的情况
 * \warning
 * \todo   
 * \author kuiwu 
 * \date 6/6/2006
 * \see 
 */
void ClipVelocity(const A3DVECTOR3& in, const A3DVECTOR3& normal, A3DVECTOR3& out, float overbounce)
{

	float	backoff;

#define DTP_EPSILON   0.001f

	a_Clamp(overbounce, 1.0f, 1.5f); 
	A3DVECTOR3 indir;
	float inspd;

	inspd = Normalize(in, indir);

	float dtp = DotProduct (indir, normal);

	if (dtp > 0 && dtp < DTP_EPSILON)
	{
		out = in;
		return;
	}
	
	out = in - normal * dtp * inspd;

	backoff = fabs(dtp) * (overbounce - 1.0f)*inspd;
	a_ClampFloor(backoff, DTP_EPSILON);
	out += normal*backoff;
	



// 	for (int i=0 ; i<3 ; i++)
// 	{
// 		// If out velocity is too small, zero it out.
// 		if (out.m[i] > -VEL_EPSILON && out.m[i] < VEL_EPSILON)
// 			out.m[i] = 0;
// 	}

#ifdef CDR_DEBUG
	float newdtp = DotProduct(out, normal);
	assert(newdtp > 0.0f);
#endif
}


/**
 * \brief 
 * \param[in]
 * \param[out]
 * \return
 * \note
 * \warning
 * \todo   
 * \author kuiwu 
 * \date 6/6/2006
 * \see 
 */
int TrySlideMove(move_t& mv)
{
#define  MAX_TRY  4
	A3DVECTOR3 planes[MAX_TRY +1];
	int        numplanes;
	A3DVECTOR3 primal_velocity, original_velocity, new_velocity, original_pos;
	float      time_left, allFraction;
	int        nTry;
	env_trace_t  trcInfo;
	A3DVECTOR3   delta;
	bool         bClear ;

	trcInfo.dwCheckFlag = CDR_TERRAIN | CDR_BRUSH;
	trcInfo.vExt = mv.ext;
	primal_velocity = mv.velocity;
	original_velocity = mv.velocity;
	original_pos = mv.start;
	new_velocity.Clear();

	time_left =         mv.t;
	allFraction    = 0.0f;
	nTry   = 0;
	numplanes = 0;


	for (nTry = 0; nTry < MAX_TRY; nTry++)
	{
		mv.end = mv.start;
		delta =  mv.velocity * time_left;
		
		float fDeltaDist = delta.Magnitude();
		if (fDeltaDist < 1E-4 ) 
		{
#ifdef CDR_DEBUG
			sprintf(msg, "no delta %f\n", fDeltaDist);
			OUTPUT_DEBUG_INFO(msg);
#endif
			break;
		}

		trcInfo.vStart = mv.start;
		trcInfo.vDelta = delta;
		trcInfo.vTerStart = mv.start;
		trcInfo.vTerStart.y -= mv.ext.y; //foot

		bClear = !CollideWithEnv(&trcInfo);	


#ifdef CDR_DEBUG
		sprintf(msg, "v %f %f %f  frac %f n %f %f %f\n", 
				mv.velocity.x,	mv.velocity.y , mv.velocity.z,
				trcInfo.fFraction, trcInfo.vHitNormal.x, trcInfo.vHitNormal.y, trcInfo.vHitNormal.z);
		OUTPUT_DEBUG_INFO(msg);
#endif
	
		// If we started in a solid object, or we were in solid space
		//  the whole way, zero out our velocity 
		if (trcInfo.bStartSolid) 
		{
			// entity is trapped in another solid
			mv.end = original_pos;
			mv.velocity.Clear();
			return 0;
		}

		allFraction += trcInfo.fFraction;
		time_left -= time_left*trcInfo.fFraction;
		mv.start +=  delta * trcInfo.fFraction;
		mv.end    = mv.start;
		//moved some portion of the total distance
		if (trcInfo.fFraction > 1E-3)
		{
			original_velocity = mv.velocity;
			numplanes = 0;
		}
		
		// If we covered the entire distance, we are done
		//  and can return.
		if (trcInfo.fFraction == 1)
		{
			 break;		// moved the entire distance
		}

		planes[numplanes] = trcInfo.vHitNormal;
		++numplanes;

		// modify original_velocity so it parallels all of the clip planes
		//

		// relfect player velocity 
		// Only give this a try for first impact plane because you can get yourself stuck in an acute corner by jumping in place
		//  and pressing forward and nobody was really using this bounce/reflection feature anyway...
		//if (numplanes == 1 && mv.tpnormal.IsZero())
		if (numplanes == 1 && mv.tpnormal.y < mv.slope)
		{
			if (planes[0].y > mv.slope)
			{
				//floor or slope
				//	ClipVelocity(original_velocity, planes[0], new_velocity, 1.0f);
					ClipVelocity(original_velocity, planes[0], new_velocity, 1.05f);
			}
			else
			{
				//陡坡
				ClipVelocity(original_velocity, planes[0], new_velocity, 1.08f);
				
			}
			mv.velocity = new_velocity;
			original_velocity = new_velocity;
		}
		else
		{
			int i;
			for (i=0 ; i < numplanes ; ++i)
			{
				//ClipVelocity (original_velocity, planes[i],	mv.velocity,	1.0f);
				ClipVelocity (original_velocity, planes[i],	mv.velocity,	1.05f);
				int j;
				for (j=0 ; j<numplanes ; ++j)
				{
					if (j != i)
					{
						// Are we now moving against this plane?
						if (DotProduct(planes[j], mv.velocity) < 0)
							break;	// not ok
					}
				}
				if (j == numplanes)  // Didn't have to clip, so we're ok
					break;
			}
			
			// Did we go all the way through plane set
			if (i != numplanes)
			{	// go along this plane
				// velocity is set in clipping call, no need to set again.
				;  
			}
			else
			{	// go along the crease
				if (numplanes != 2)
				{
					//对于复杂的场景，crease有三个或三个以上的plane, 清除velocity,
					//会造成不能移动???
					mv.velocity.Clear();
					break;
				}
				//只有两个面，计算一个新的速度方向
				A3DVECTOR3 dir;
				dir =	CrossProduct(planes[0], planes[1]);
				//将原velocity投影到新方向
				float dtp = DotProduct(dir, mv.velocity);
				mv.velocity = dir * dtp;
				//A3DVECTOR3 out(planes[0] + planes[1]);
				//out.Normalize();
				//mv.velocity += out * 0.05f;
#ifdef CDR_DEBUG
				sprintf(msg, "crease ");
				OUTPUT_DEBUG_INFO(msg);
#endif
			}

			//
			// if original velocity is against the original velocity, stop dead
			// to avoid tiny occilations in sloping corners
			//
			//这样做有效防止爬到陡坡上吗？
			float dtp =  DotProduct(mv.velocity, primal_velocity);
			if (dtp <= 0)
			{
	#ifdef CDR_DEBUG
					sprintf(msg, "disobey primal vel %f\n", dtp);
					OUTPUT_DEBUG_INFO(msg);
	#endif
					mv.velocity.Clear();
					break;
			}
		}
	}
	
	if ( allFraction == 0 )
	{
		mv.velocity.Clear();
		
	}

#ifdef CDR_DEBUG
	sprintf(msg, "slide %f %f %f v %f %f %f\n", 
			mv.end.x, mv.end.y, mv.end.z,
			mv.velocity.x, mv.velocity.y, mv.velocity.z);
	OUTPUT_DEBUG_INFO(msg);
#endif
	
	return 0;
#undef   MAX_TRY
}

/**
 * \brief try to  step up to stairs
 * \param[in]
 * \param[out]
 * \return
 * \note
 * \warning
 * \todo   
 * \author kuiwu 
 * \date 8/6/2006
 * \see 
 */
void StepUp(move_t& mv, float step)
{
	A3DVECTOR3 origin = mv.start;  // save the input pos [6/8/2006 kuiwu]
		
	env_trace_t tmpInfo;
	tmpInfo.vExt = mv.ext;
	//@note : need check terrain?? By Kuiwu[8/10/2005]
	tmpInfo.dwCheckFlag = CDR_BRUSH | CDR_TERRAIN;  

	//up
	tmpInfo.vStart = mv.start;
	tmpInfo.vDelta = A3DVECTOR3(0.0f, step, 0.0f);
	tmpInfo.vTerStart = tmpInfo.vStart;
	tmpInfo.vTerStart.y -= tmpInfo.vExt.y;
	bool bClear = !CollideWithEnv(&tmpInfo);
	if (tmpInfo.bStartSolid)
	{
		mv.end = origin;
		return;
	}
	if (bClear)
	{
		mv.start += tmpInfo.vDelta;
	}
	else
	{
		mv.start +=  tmpInfo.vDelta*tmpInfo.fFraction;
	}

	
	//forward
	TrySlideMove(mv);

	//down
	tmpInfo.vStart = mv.end;
	tmpInfo.vDelta = A3DVECTOR3(0.0f, -step, 0.0f);
	tmpInfo.vTerStart = tmpInfo.vStart;
	tmpInfo.vTerStart.y -= tmpInfo.vExt.y;
	bClear = !CollideWithEnv(&tmpInfo);
	if (tmpInfo.bStartSolid)
	{
		mv.end = origin;
		StepOnPoint = mv.end;
		return;
	}

	if (bClear)
	{
		mv.end += tmpInfo.vDelta;
		StepOnPoint = mv.end;
		return;
	}
	mv.end += tmpInfo.vDelta * tmpInfo.fFraction;
    StepOnPoint =  mv.end; 
	if (tmpInfo.vHitNormal.y < mv.slope)
	{
		//steep
		mv.end = origin;
		StepOnSlope = 1;
		StepOnNormal = tmpInfo.vHitNormal;
		return;
	}
}

/**
 * \brief walk move, tpnormal > slope
 * \param[in]
 * \param[out]
 * \return
 * \note
 * \warning
 * \todo   
 * \author kuiwu 
 * \date 8/6/2006
 * \see 
 */
void WalkMove(move_t& mv, float step)
{

	A3DVECTOR3 wishdir(mv.wishdir);
	// Zero out y part of velocity
	wishdir.y = 0.0f;
	float  wishspeed = mv.wishspd;
	
// 	float max_wishspeed   = 5.0f;
// 	//
// 	// Clamp to server defined max speed
// 	//
// 	if (wishspeed > max_wishspeed)
// 	{
// 		wishspeed = max_wishspeed;
// 	}
	// Set pmove velocity
	mv.velocity.y = 0.0f; // Zero out y part of velocity
	Accelerate(wishdir, wishspeed, mv.accel, mv.t, mv.velocity);
	mv.velocity.y = 0.0f;
	mv.absvelocity = mv.velocity;
	if (mv.velocity.SquaredMagnitude() < 1E-4f)
	{
		mv.velocity.Clear();
		mv.end = mv.start;
		return;
	}
	



	A3DVECTOR3 origin(mv.start);   //save original
	A3DVECTOR3 originvel(mv.velocity);
	

	// first try just moving to the destination	
	//note: 这里与hl2稍有不同，对velocity按当前tpnormal clip，避免额外的碰撞
	ClipVelocity(originvel, mv.tpnormal, mv.velocity, 1.01f);

	
	env_trace_t tmpInfo;
	tmpInfo.vExt = mv.ext;
	tmpInfo.dwCheckFlag = CDR_BRUSH | CDR_TERRAIN;  
	tmpInfo.vStart = mv.start;
	tmpInfo.vDelta = mv.velocity * mv.t;
	tmpInfo.vTerStart = tmpInfo.vStart;
	tmpInfo.vTerStart.y -= tmpInfo.vExt.y;
	bool bClear = !CollideWithEnv(&tmpInfo);

	if (tmpInfo.bStartSolid)
	{
		mv.velocity.Clear();
		mv.end = origin;
		return;
	}
	// If we made it all the way, then copy trace end
	//  as new player position.
	if (bClear)
	{
		mv.velocity = originvel;
		mv.end = tmpInfo.vStart + tmpInfo.vDelta;
		return;
	}


	// Try sliding forward both on ground and up,
	//  take the move that goes farthest
	A3DVECTOR3 down(origin), downvel(originvel);
	A3DVECTOR3 up(origin);
	//mv.velocity = originvel;  //需要恢复原速度吗？
	TrySlideMove(mv);  //down slide move
	
	// Copy the results out
	down = mv.end;
	downvel = mv.velocity;
	
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	PosPair PairBack = pHost->GetEndPosPair();

	// Reset original values.
	mv.start = origin;
	mv.velocity = originvel;
	StepUp(mv, step);   //up move
	
	up = mv.end;

	// decide which one went farther
	float downdist = (down.x - origin.x) * (down.x - origin.x)
				   + (down.z - origin.z) * (down.z - origin.z);
	float updist   = (up.x - origin.x) * (up.x - origin.x)
				   + (up.z - origin.z) * (up.z - origin.z);
	
#ifdef CDR_DEBUG
// 	sprintf(msg, "down %f %f %f up %f %f %f downdist %f updist %f\n",
// 			down.x, down.y, down.z, up.x, up.y, up.z, downdist, updist);
// 	OUTPUT_DEBUG_INFO(msg);
#endif
	if (downdist > updist)
	{
		mv.end = down;
		mv.velocity = downvel;
		pHost->SetEndPosPair(PairBack); //hongkan
	}
	else
	{
		mv.end = up;
		mv.velocity.y = downvel.y;  // copy z value from slide move???
	}
	
}
/**
 * \brief jump, fall and slide in steep slope.
 * \param[in]
 * \param[out]
 * \return
 * \note
 * \warning
 * \todo   
 * \author kuiwu 
 * \date 8/6/2006
 * \see 
 */
void JumpFallMove(move_t& mv)
{
	A3DVECTOR3		wishdir;
	float		wishspeed;

	wishdir = mv.wishdir;
	wishspeed = mv.wishspd;
	//
	// clamp to server defined max speed
	//
// 	if (wishspeed > 5.0f)
// 	{
// 		wishspeed = 5.0f;
// 	}
	assert(wishdir.y == 0.0f);
	AirAccelerate(wishdir, wishspeed, mv.accel, mv.t, mv.velocity);
	mv.absvelocity = mv.velocity;
	TrySlideMove(mv);
}

/**
 * \brief 
 * \param[in]
 * \param[out]
 * \return
 * \note
 * \warning
 * \todo   
 * \author kuiwu 
 * \date 8/6/2006
 * \see 
 */
void FullGroundMove(move_t& mv, float gravity, float step)
{
	A3DVECTOR3 origin(mv.start), originvel(mv.velocity);

	mv.velocity.y -= gravity * mv.t* 0.5f;  // start 0.5 and end 0.5?

	// check and clamp velocity
	//CheckVelocity(mv.velocity);
	
	
	// Fricion is handled before we add in any base velocity. That way, if we are on a conveyor, 
	//  we don't slow when standing still, relative to the conveyor.
	//if (player->GetGroundEntity() != NULL)
	if (mv.tpnormal.y > mv.slope)
	{
		//清除竖直方向速度
		mv.velocity.y = 0.0f;
	}

	// Make sure velocity is valid.
	//CheckVelocity(mv.velocity);

	if (mv.tpnormal.y > mv.slope)
	{
		WalkMove(mv,  step);
	}
	else
	{
		JumpFallMove(mv);
	}

	if (!IsPosInAvailableMap(mv.end))
	{
		mv.end = origin;
		mv.velocity = originvel;
		return;
	}
#ifdef CDR_DEBUG
// 	sprintf(msg, "move %f %f %f\n", 
// 		mv.end.x, mv.end.y, mv.end.z);
// 	OUTPUT_DEBUG_INFO(msg);
#endif
	//TODO: tune this value
	if (originvel.y  > 1.0f)
	{
		//jump 
		mv.tpnormal.Clear();
	}
	else
	{
		ground_trace_t groundTrc;
		groundTrc.vStart = mv.end;
		groundTrc.vExt = mv.ext;
		groundTrc.fDeltaY = 0.08f;
		//groundTrc.fBump = 0.001f;
		if (!RetrieveSupportPlane(&groundTrc))
		{
			mv.end = origin;
			mv.velocity = originvel;
			if (mv.tpnormal.y < mv.slope)
			{
				mv.tpnormal = A3DVECTOR3(0.0f, 1.0f, 0.0f);
			}
#ifdef CDR_DEBUG
			sprintf(msg, "tpn fail %f %f %f\n", 
				groundTrc.vStart.x, groundTrc.vStart.y, groundTrc.vStart.z);
			OUTPUT_DEBUG_INFO(msg);
#endif
			return;  
		}		
		if (groundTrc.bSupport)
		{
			mv.end = groundTrc.vEnd;
			mv.tpnormal = groundTrc.vHitNormal;
		}
		else
		{
			mv.tpnormal.Clear();
		}
		
	}

	//== added by hongkan to deal with low steep slope problem==//
	if (((mv.tpnormal.y <= mv.slope && mv.tpnormal.y > 0) && (( mv.absvelocity.x*mv.tpnormal.x + mv.absvelocity.z*mv.tpnormal.z)<0))
	   || (StepOnSlope == 1))
	{
        A3DVECTOR3 SlopeNormal;
		
		if (mv.tpnormal.y <= mv.slope && mv.tpnormal.y > 0) 
		{
            SlopeNormal = mv.tpnormal;
		}
		else
		{
            SlopeNormal = StepOnNormal;
		}

		if (CheckLowSteepSlope(mv, origin, SlopeNormal))
		{
			mv.tpnormal = A3DVECTOR3(0.0f, 1.0f, 0.0f);
            if (!StepOnPoint.IsZero()) mv.end = StepOnPoint;
      	}
	}
    StepOnSlope = 0;
    StepOnPoint.Clear();
	//==========================================================//

	mv.velocity.y -= gravity * mv.t* 0.5f;  // start 0.5 and end 0.5?
	if (mv.tpnormal.y > mv.slope)
	{
		mv.velocity.y = 0.0f;
	}
#ifdef CDR_DEBUG
	sprintf(msg, "full %f %f %f\n", 
		mv.velocity.x, mv.velocity.y, mv.velocity.z);
	OUTPUT_DEBUG_INFO(msg);
#endif

}

void CheckVelocity(A3DVECTOR3& vel)
{
	int i;
	for ( i  = 0; i < 3; ++i)
	{
		if (vel.m[i] > VEL_MAX_SPEED)
		{
			vel.m[i] = VEL_MAX_SPEED;
		}
		if (vel.m[i] < -VEL_MAX_SPEED)
		{
			vel.m[i] = -VEL_MAX_SPEED;
		}
	}
}
/**
 * \brief disable the up velocity, and try to give a new velocity
 * \param[in]
 * \param[out]
 * \return
 * \note
 * \warning
 * \todo   
 * \author kuiwu 
 * \date 8/6/2006
 * \see 
 */
void DisableUpVel(const A3DVECTOR3& in, const A3DVECTOR3& normal, A3DVECTOR3& out)
{
	//assert(in.y > 0.0f && normal.y < 0.71f);
	A3DVECTOR3 hor(CrossProduct(normal, -g_vAxisY)); //horizontal
	float dtp = DotProduct(in, hor);
	out = hor * dtp;
	//avoid collision again
	A3DVECTOR3 fakeNormal(CrossProduct(hor, -g_vAxisY));
	float backoff = DotProduct(fakeNormal, normal) * 0.01f;  //quake2 bounce
	out.x += backoff*fakeNormal.x;
	out.z += backoff*fakeNormal.z;

#ifdef CDR_DEBUG
	sprintf(msg, "disableup %f %f %f out %f %f %f\n", in.x,in.y, in.z, out.x, out.y, out.z);
	OUTPUT_DEBUG_INFO(msg);
#endif
}



void Accelerate(const A3DVECTOR3& wishdir, float wishspeed, float accel, float t,  A3DVECTOR3& vel)
{
	float addspeed, accelspeed, currentspeed;

	// See if we are changing direction a bit
	currentspeed = DotProduct(vel, wishdir);

	// Reduce wishspeed by the amount of veer.
	addspeed = wishspeed - currentspeed;

	// If not going to add any speed, done.
	if (addspeed <= 0)
	{
		vel = wishdir * wishspeed;  //不要超过wishspeed
		return;
	}

	// Determine amount of acceleration.
	accelspeed = accel * t * wishspeed;

	// Cap at addspeed
	if (accelspeed > addspeed)
		accelspeed = addspeed;
#ifdef CDR_DEBUG
	sprintf(msg, "old %f %f %f", vel.x, vel.y, vel.z);
	OUTPUT_DEBUG_INFO(msg);
#endif
	// Adjust velocity.
	vel = wishdir * (currentspeed + accelspeed);
	//vel +=  wishdir * accelspeed;
#ifdef CDR_DEBUG
	sprintf(msg, "new %f %f %f accel %f new %f\n", vel.x, vel.y, vel.z,
		currentspeed, currentspeed+ accelspeed);
	OUTPUT_DEBUG_INFO(msg);
#endif
}


void AirAccelerate(const A3DVECTOR3& wishdir, float wishspeed, float accel, float t, A3DVECTOR3& vel)
{

	float addspeed, accelspeed, currentspeed;
	float wishspd;

	wishspd = wishspeed;
	

	// Cap speed
//#define  AIR_MAX_SPEED  0.3f
#define  AIR_MAX_SPEED  0.35f
	// 令 g= 9.8,  t = 0.02 (50 fps)
	if (wishspd > AIR_MAX_SPEED)
		wishspd = AIR_MAX_SPEED;

	// Determine veer amount
	currentspeed =  DotProduct(vel, wishdir);

	// See how much to add
	addspeed = wishspd - currentspeed;

	// If not adding any, done.
	if (addspeed <= 0)
		return;

	// Determine acceleration speed after acceleration
	accelspeed = accel * wishspeed * t;

	// Cap it
	if (accelspeed > addspeed)
		accelspeed = addspeed;
	
	// Adjust pmove vel.
	//vel += wishdir * (accelspeed);
	
	vel.x = wishdir.x * (currentspeed + accelspeed);
	vel.z = wishdir.z * (currentspeed + accelspeed);

}

void Friction(move_t& mv)
{
	float	speed, newspeed, control;
	float	friction;
	float	drop;
	A3DVECTOR3 newvel;

	

	// Calculate speed
	speed  = mv.velocity.Magnitude();
	// If too slow, return
	// hl2 取 0.1 , 0.1/190 *5 = 0.002f
	if (speed < 0.002f)
	{
		//是否需要清除速度？
		return;
	}

	drop = 0;

	// apply ground friction
	if (mv.tpnormal.y > mv.slope )  // On an entity that is the ground
	{
		//hl2 向下探测取ground entity 的surface 信息，一般是3 or 4
		// 3 /190 * 5 = 0.08f
		friction = 0.08f;
		// Bleed off some speed, but if we have less than the bleed
		//  threshhold, bleed the theshold amount.
		// hl2的stop = 100,  100/190 *5 = 2.5f
		control = (speed < 2.5f) ?
			2.5f : speed;

		// Add the amount to the drop amount.
		drop += control*friction*mv.t;
	}

	// scale the velocity
	newspeed = speed - drop;
	if (newspeed < 0)
		newspeed = 0;

	// Determine proportion of old speed we are using.
	newspeed /= speed;

	// Adjust velocity according to proportion.

	mv.velocity *= newspeed;
}


#endif

bool CheckLowSteepSlope(move_t mv , A3DVECTOR3 startpoint, A3DVECTOR3 SlopeNormal ) //added by hongkan 10.31 newly modified
{
     	//hongkan 08.10.21
        //======check whether the steep slope is low enough to be mounted=====//
		assert(SlopeNormal.y<0.707 && SlopeNormal.y > 0);

        if (( mv.absvelocity.x*SlopeNormal.x + mv.absvelocity.z*SlopeNormal.z)>=0) //速度并不朝向要check的坡 08.11.18 hongkan 
		   return false;//hongkan 08.11.18
		
		
		float step=0.65f;
	    env_trace_t steep_tmpInfo;
       	steep_tmpInfo.vExt = mv.ext;
       	steep_tmpInfo.dwCheckFlag = CDR_BRUSH | CDR_TERRAIN;  

       	//up
       	mv.start=steep_tmpInfo.vStart = startpoint;
       	steep_tmpInfo.vDelta = A3DVECTOR3(0.0f, step, 0.0f);
       	steep_tmpInfo.vTerStart = steep_tmpInfo.vStart;
       	steep_tmpInfo.vTerStart.y -= steep_tmpInfo.vExt.y;
       	bool steep_bClear = !CollideWithEnv(&steep_tmpInfo);

   		if (steep_bClear)
   		{
   			mv.start += steep_tmpInfo.vDelta;
   		}
   		else
   		{
   			mv.start +=  steep_tmpInfo.vDelta*steep_tmpInfo.fFraction;
   		}

	
   		//forward  正面向坡(根据坡的normal) 
        SlopeNormal.y = 0;
		SlopeNormal = -SlopeNormal; 
		SlopeNormal.Normalize();
   		SlopeNormal*=(step+0.1f);//fixed length,水平前探步长 +0.1m hongkan modified 08.11.19

		steep_tmpInfo.vStart = mv.start;
		steep_tmpInfo.vDelta = SlopeNormal;
		steep_tmpInfo.vTerStart = mv.start;
		steep_tmpInfo.vTerStart.y -= steep_tmpInfo.vExt.y; 

		steep_bClear = !CollideWithEnv(&steep_tmpInfo); 

   		if (steep_bClear)
   		{
   		    //forward  (根据主角速度 08.11.18 hongkan ) 
			mv.absvelocity.y = 0;
	     	mv.absvelocity.Normalize();
    		mv.absvelocity*=(step+0.1f);//fixed length,水平前探步长

    		steep_tmpInfo.vStart = mv.start;
    		steep_tmpInfo.vDelta = mv.absvelocity;
    		steep_tmpInfo.vTerStart = mv.start;
    		steep_tmpInfo.vTerStart.y -= steep_tmpInfo.vExt.y; 
    		steep_bClear = !CollideWithEnv(&steep_tmpInfo); 
   		    if (steep_bClear)
			{
 	            mv.start += steep_tmpInfo.vDelta; 
   	        	//down
                steep_tmpInfo.vStart = mv.start;
                steep_tmpInfo.vDelta =  A3DVECTOR3(0.0f, - step, 0.0f); 
                steep_tmpInfo.vTerStart = mv.start;
                steep_tmpInfo.vTerStart.y -= steep_tmpInfo.vExt.y; 
                steep_bClear = !CollideWithEnv(&steep_tmpInfo); 
				
   		        if (steep_bClear)
				{
					return(1);
				}
				else 
				{
                    if ( (steep_tmpInfo.vHitNormal.y < mv.slope) && ((mv.absvelocity.x*steep_tmpInfo.vHitNormal.x + mv.absvelocity.z*steep_tmpInfo.vHitNormal.z)<0 ) )
					{
				        return(0);
					}
                    else 
					{
			        	return(1);
					}
				}
			}
         	else
			{
                if (steep_tmpInfo.vHitNormal.y < mv.slope)
				{
				    return(0);
				}
                else 
				{
			    	return(1);
				}
			}
   		}
   		else
   		{
            if (steep_tmpInfo.vHitNormal.y < mv.slope)
			{
				return(0);
			}
            else 
			{
     		    //forward  (根据主角速度) 
    			mv.absvelocity.y = 0;
    	     	mv.absvelocity.Normalize();
        		mv.absvelocity*=(step+0.1f);//fixed length,水平前探步长

        		steep_tmpInfo.vStart = mv.start;
        		steep_tmpInfo.vDelta = mv.absvelocity;
        		steep_tmpInfo.vTerStart = mv.start;
        		steep_tmpInfo.vTerStart.y -= steep_tmpInfo.vExt.y; 
        		steep_bClear = !CollideWithEnv(&steep_tmpInfo); 
     		    if (steep_bClear)
				{
 	                mv.start += steep_tmpInfo.vDelta; 
   	            	//down
                    steep_tmpInfo.vStart = mv.start;
                    steep_tmpInfo.vDelta =  A3DVECTOR3(0.0f, - step, 0.0f);
                    steep_tmpInfo.vTerStart = mv.start;
                    steep_tmpInfo.vTerStart.y -= steep_tmpInfo.vExt.y; 
                    steep_bClear = !CollideWithEnv(&steep_tmpInfo); 
				
   	    	        if (steep_bClear)
					{
	    				return(1);
					}
    				else 
					{
                        if ( (steep_tmpInfo.vHitNormal.y < mv.slope) && ((mv.absvelocity.x*steep_tmpInfo.vHitNormal.x + mv.absvelocity.z*steep_tmpInfo.vHitNormal.z)<0 ) )
						{
				            return(0);
						}
                        else 
						{
			            	return(1);
						}
					}
				}
            	else
				{
                     if (steep_tmpInfo.vHitNormal.y < mv.slope)
					 {
		    		    return(0);
					 }
                    else 
					{
		      	    	return(1);
					}
				}
			}
   		}

}

#undef  CDR_BRUSH
#undef  CDR_TERRAIN
#undef  CDR_WATER

