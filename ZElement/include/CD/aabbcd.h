/*
 * FILE: aabbcd.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: wang kuiwu, 2005/8/6
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */
#ifndef  _CH_AABB_CD_H_
#define _CH_AABB_CD_H_

#include "HalfSpace.h"
#include "a3dgeometry.h"

#include "ConvexHullData.h"

class A3DFlatCollector;
class AFile;

const int    MAX_FACE_IN_HULL = 200;

namespace CHBasedCD
{

#ifndef NULL
#define NULL 0
#endif

//////////////////////////////////////////////////////////////////////////
// define a new compact Convex Hull data type which is used in ElementClient!
//////////////////////////////////////////////////////////////////////////
class BrushTraceInfo 
{
public:
	//////////////////////////////////////////////////////////////////////////
	//	In
	//////////////////////////////////////////////////////////////////////////
	// aabb's info
	A3DVECTOR3	vStart;			//	Start point
	A3DVECTOR3	vDelta;			//	Move delta
    A3DVECTOR3  vExtents; 	
	A3DAABB     BoundAABB;  
	// Dist Epsilon
#ifdef BACK_VERSION
	float		fDistEpsilon;		//	Dist Epsilon
	bool        bIsPoint;          //raytrace
#else
	float		fDistEpsilon;	    //	Dist Epsilon
	bool        bIsPoint;           //  raytrace
	bool        bSkipRayTrace;      //  AABB skip ray trace
#endif
	//////////////////////////////////////////////////////////////////////////
	//	Out
	//////////////////////////////////////////////////////////////////////////
	bool		bStartSolid;	//	Collide something at start point
	bool		bAllSolid;		//	All in something
	CHalfSpace	ClipPlane;		//	Clip plane
	int			iClipPlane;		//	Clip plane's index
	float		fFraction;		//	Fraction
	
#ifdef BACK_VERSION
	int			iUserData1;		//	User data 1
	int			iUserData2;		//	User data 2
#else
	DWORD		dwUser1;		//	User defined data 1
	DWORD		dwUser2;		//	User defined data 2
#endif

#ifdef BACK_VERSION
	void Init(const A3DVECTOR3& start, const A3DVECTOR3& delta,  const A3DVECTOR3& ext,  bool bRay = false, float epsilon = 0.03125f ){
#else
	void Init(const A3DVECTOR3& start, const A3DVECTOR3& delta,  const A3DVECTOR3& ext,  bool bRay = false, float epsilon = 0.03125f, bool bSkipRay=false){
#endif
		vStart = start;
		vDelta = delta;
		vExtents = ext;
		fDistEpsilon = epsilon;
		fFraction = 100.0f;
		bIsPoint = bRay;
		bStartSolid = false;
		bAllSolid = false;
#ifdef BACK_VERSION
		iUserData1 = 0;
		iUserData2 = 0;
#else
		bSkipRayTrace = bSkipRay;
		dwUser1 = 0;
		dwUser2 = 0;
#endif
		if (!bIsPoint) {
			BoundAABB.Clear();
			BoundAABB.AddVertex(vStart);
			BoundAABB.AddVertex(vStart + vExtents);
			BoundAABB.AddVertex(vStart - vExtents);
			BoundAABB.AddVertex(vStart+ vDelta);
			BoundAABB.AddVertex(vStart + vDelta + vExtents);
			BoundAABB.AddVertex(vStart + vDelta - vExtents);
			BoundAABB.CompleteCenterExts();
		}
		else{
			//@note : it cheats the caller. By Kuiwu[25/8/2005]
			//@todo : refine me. By Kuiwu[25/8/2005]
			fDistEpsilon = 1E-5f;
		}
	}
};

class CCDSide
{
public:
	CHalfSpace  plane;
	bool        bevel;

//operation
public:
	CCDSide(){};
	CCDSide(CHalfSpace& hs, bool bBevel): plane(hs), bevel(bBevel){
	}
	void Init(CHalfSpace& hs, bool bBevel){
		plane = hs;
		bevel = bBevel;
	}
	void Init(A3DVECTOR3& vNormal, float fDist, bool bBevel){
		plane.SetNormal(vNormal);
		plane.SetD(fDist);
		bevel = bBevel;
	}

};

class CCDBrush
{
	friend class CQBrush;

public:
	
	// constructor, deconstructor and releaser
	CCDBrush():m_pSides(NULL),m_nSides(0),m_dwReserved(0){}
	~CCDBrush(){ Release();}

	void Release() 
	{
		if(m_pSides)
		{
			delete[] m_pSides;
			m_pSides = NULL;
		}
		m_nSides = 0;
	}

	CCDBrush * Clone();

	A3DAABB GetAABB()const
	{
		return m_aabb;
	}

	DWORD GetReservedDWORD()
	{
		return m_dwReserved;
	}

	void SetReservedDWORD(DWORD dwReserved)
	{
		m_dwReserved = dwReserved;	
	}

	// Load and save method
	bool Load(AFile * pFileToLoad);
	bool Save(AFile * pFileToSave);
	/*
	 *	trace with aabb or ray
	 *  @param  pInfo: the trace info (both input and output) @ref class BrushTraceInfo
	 *  @return  false if no collision 
	 */
	bool Trace(BrushTraceInfo * pInfo);

#ifndef BACK_VERSION
	int GetSides()  { return m_nSides; }
#endif

private:

	CCDSide             * m_pSides;
	int					m_nSides;
	
	A3DAABB				m_aabb;

	DWORD				m_dwReserved;
};


class  CQBrush
{
public:
#ifdef BACK_VERSION
	CQBrush();
#else
	CQBrush(){
		m_pCHData = NULL;
		m_nQPlane = 0;
	}
#endif
	~CQBrush();
	void Release();
#ifdef BACK_VERSION
	void AddBrushBevels(CConvexHullData * pCHData);
#else
	bool AddBrushBevels(CConvexHullData * pCHData);
#endif
	CConvexHullData * GetCHData(){
		return m_pCHData;
	}
	int GetSideNum(){
		return m_nQPlane;
	}
	
	CHalfSpace * GetSide(int i){
		return m_pQPlane[i].pHS;
	}

	bool IsBevel(int i){
		return m_pQPlane[i].bBevel;
	}

	void Transform(const A3DMATRIX4& mtxTrans);
	
	//////////////////////////////////////////////////////////////////////////
	// render routines
	static void RenderPlane(A3DFlatCollector* pFC, const A3DVECTOR3& vPos, const A3DVECTOR3& vNormal, DWORD dwColor, float fUpExtent, float fRightExtent);
	void Render(A3DFlatCollector* pFC,  bool bRenderAABBBevels = false, DWORD dwBevelCol = 0x80ff0000,float fUpExtents = 1.0f,float fRightExtents=2.0f, bool bRenderBevels= true, bool bRenderCH = true);

	void Export(CCDBrush *pCDBrush);
#ifdef BACK_VERSION
	bool Trace(BrushTraceInfo * pInfo);
#endif
private:
	CConvexHullData * m_pCHData;
	
	struct QPlane {
		CHalfSpace * pHS;
		int          iCHIndex;
		bool         bBevel;
		//////////////////////////////////////////////////////////////////////////
		//debug
		A3DVECTOR3    vert;
		//////////////////////////////////////////////////////////////////////////
	};
	
	QPlane  m_pQPlane[MAX_FACE_IN_HULL];
	int    m_nQPlane;

#ifdef BACK_VERSION
	bool   m_bCDReady;
	CCDBrush * m_pCDBrush;
#endif


	void _FlushCH();


};

#ifndef BACK_VERSION

//////////////////////////////////////////////////////////////////////////
// Trace the CQBrush!
//////////////////////////////////////////////////////////////////////////
/*
 *	@note: kuiwu, ugly but for compatibility!
 */
class  AABBBrushTraceInfo:public BrushTraceInfo
{
public:
	//////////////////////////////////////////////////////////////////////////
	//	In
	//////////////////////////////////////////////////////////////////////////
	// Convex hull data
	CQBrush *pBrush;
	
	void Init(const A3DVECTOR3& start, const A3DVECTOR3& delta,  const A3DVECTOR3& ext,  CQBrush * brush = NULL, bool ray = false, float epsilon = 0.03125f ){
		pBrush = brush;
		BrushTraceInfo::Init(start, delta, ext, ray, epsilon);
	}
};

bool ClipAABBToBrush(AABBBrushTraceInfo& TraceInfo);
#endif

}

#endif