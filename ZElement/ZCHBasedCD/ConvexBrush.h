/*
 * FILE: ConvexBrush.h
 *
 * DESCRIPTION: Convex hull brush used to collision detection, both capsule and AABB are supported 
 *
 * CREATED BY: YangXiao
 *
 * HISTORY: 
 *
 * Copyright (c) 2010 Perfect World, All Rights Reserved.
 */
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVEXBRUSH_H__3E373194_C7F6_4314_BC7B_BAC075BA4C87__INCLUDED_)
#define AFX_CONVEXBRUSH_H__3E373194_C7F6_4314_BC7B_BAC075BA4C87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConvexHullData.h"
#include "A3DFlatCollector.h"
#include <vector.h>

using namespace abase;

namespace CHBasedCD
{

	class AbstractBrushTraceInfo
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		//   In
		//////////////////////////////////////////////////////////////////////////
		A3DVECTOR3 vStart;    //	Start point         
		A3DVECTOR3 vDelta;	  //    Move delta
		
		float fDistEpsilon;   //Dist Epsilon
		A3DMATRIX4 mat;       //brush coordinate
		float fScale;         //brush scale

		//////////////////////////////////////////////////////////////////////////
		//	Out
		//////////////////////////////////////////////////////////////////////////
		bool		bStartSolid;	//	Collide something at start point
		bool		bAllSolid;		//	All in something
		CHalfSpace	ClipPlane;		//	Clip plane(local coordinate) 
		float		fFraction;		//	Fraction
		DWORD		dwUser1;		//	User defined data 1
		DWORD		dwUser2;		//	User defined data 2
		A3DAABB     BoundAABB;      // bound AABB world coordinate
		void *pUser1;               // user defined data

	public:
		virtual int* GetType() = 0; 
		//设置Brush的坐标系和碰撞者的Up方向。CapsuleBrushTraceInfo时参数Up表示Capsule的向上方向，其他无意义
		virtual void SetTransMat(const A3DMATRIX4& mat, const A3DVECTOR3& Up = A3DVECTOR3(0, 1, 0)) = 0;
	};
	class RayBrushTraceInfo: public AbstractBrushTraceInfo
	{
		static int stype;
		virtual int* GetType() { return &stype; }
	public:
		static bool IsInstance(AbstractBrushTraceInfo* pInfo) { return &stype == pInfo->GetType(); }
	public:
		

	private:
		//////////////////////////////////////////////////////////////////////////
		//	interim
		//////////////////////////////////////////////////////////////////////////
		A3DVECTOR3 p1;//start position (brush coordinate )
		A3DVECTOR3 p2;//end position   (brush coordinate )
		
	public:
		
		
		friend class CConvexBrush;
		friend bool RayAABBCollision(RayBrushTraceInfo* pInfo, const A3DAABB& aabb);

		//start, delta world coordinate; mat convex hull brush translation and rotation(without scale)
		void Init(const A3DVECTOR3& start, const A3DVECTOR3& delta, const A3DMATRIX4& mat = A3DMATRIX4(A3DMATRIX4::IDENTITY),  
			float scale = 1, float epsilon = 1E-5f )
		{
			vStart = start;
			vDelta = delta;
			fDistEpsilon = epsilon;
			fScale = scale;			
			dwUser1 = 0;
			dwUser2 = 0;
			pUser1 = NULL;
			SetTransMat(mat, A3DVECTOR3());
		}

		//该矩阵需保证等比缩放
		virtual void SetTransMat(const A3DMATRIX4& mat, const A3DVECTOR3& )
		{
			this->mat = mat;
			A3DMATRIX4 matInverse(mat);
			fScale = matInverse.GetCol(0).Magnitude();
			for(int i=0;i<3;i++)
				for(int j=0;j<3;j++)
			matInverse.m[i][j]=matInverse.m[i][j]/fScale;
			matInverse.InverseTM();

			p1 = vStart,p2 = vStart+vDelta;
			//转换到brush坐标系下
			p1 = p1 * matInverse;
			p2 = p2 * matInverse;
		}

	};
	class AABBBrushTraceInfo2: public AbstractBrushTraceInfo 
	{
		static int stype;
		virtual int* GetType() { return &stype; }
	public:
		static bool IsInstance(AbstractBrushTraceInfo* pInfo) { return &stype == pInfo->GetType(); }
	public:
		A3DVECTOR3  vExtents; 	

	private:
		//////////////////////////////////////////////////////////////////////////
		//	interim
		//////////////////////////////////////////////////////////////////////////		
		A3DMATRIX4 matInverse;//逆
		A3DVECTOR3 xaxis;//brush坐标系下AABB的轴向
		A3DVECTOR3 yaxis;
		A3DVECTOR3 zaxis;
		A3DVECTOR3 vertexP[2][2][2];//AABB8个顶点，x,y,z三个轴的负向及正向的点
		A3DVECTOR3 p1;//起点
		A3DVECTOR3 p2;//终点		
		

	public:
		//////////////////////////////////////////////////////////////////////////
		//	Out
		//////////////////////////////////////////////////////////////////////////
		A3DOBB      obb;            //  OBB from the movement(under brush coordinate) 
		
		friend class CConvexBrush;
		
		//start, delta world coordinate; mat convex hull brush translation and rotation(without scale)
		void Init(const A3DVECTOR3& start, const A3DVECTOR3& delta,  const A3DVECTOR3& ext, const A3DMATRIX4& mat = A3DMATRIX4(A3DMATRIX4::IDENTITY),  
			float scale = 1, float epsilon = 0.03125f ){
			vStart = start;
			vDelta = delta;
			vExtents = ext;
			fDistEpsilon = epsilon;
			fFraction = 100.0f;
			bStartSolid = false;
			bAllSolid = false;
			dwUser1 = 0;
			dwUser2 = 0;
			pUser1 = NULL;
			fScale = scale;
			
			SetTransMat(mat, A3DVECTOR3());
			
		}
		virtual void SetTransMat(const A3DMATRIX4& mat, const A3DVECTOR3& )
		{
			//yx
			this->mat = mat;
			matInverse = mat;
			fScale = matInverse.GetCol(0).Magnitude();
			int i, j;
			for(i=0;i<3;i++)
				for(j=0;j<3;j++)
					matInverse.m[i][j]=matInverse.m[i][j]/fScale;
			matInverse.InverseTM();

			p1 = vStart,p2 = vStart+vDelta;
			//转换到brush坐标系下
			p1 = p1 * matInverse;
			p2 = p2 * matInverse;
			
			BoundAABB.Clear();
			BoundAABB.AddVertex(vStart );
			BoundAABB.AddVertex((vStart + vExtents));
			BoundAABB.AddVertex((vStart - vExtents));
			BoundAABB.AddVertex((vStart+ vDelta));
			BoundAABB.AddVertex((vStart + vDelta + vExtents) );
			BoundAABB.AddVertex((vStart + vDelta - vExtents) );
			BoundAABB.CompleteCenterExts();
			BoundAABB.Extents += A3DVECTOR3(fDistEpsilon, fDistEpsilon, fDistEpsilon);
			BoundAABB.CompleteMinsMaxs();
			
			obb.Center = BoundAABB.Center * matInverse;
			
			matInverse._41 = 0;
			matInverse._42 = 0;
			matInverse._43 = 0;
			xaxis = A3DVECTOR3(1,0,0);
			yaxis = A3DVECTOR3(0,1,0);
			zaxis = A3DVECTOR3(0,0,1);
			xaxis = xaxis * matInverse;
			yaxis = yaxis * matInverse;
			zaxis = zaxis * matInverse;
			
			
			obb.XAxis = xaxis;
			obb.YAxis = yaxis;
			obb.ZAxis = zaxis;
			obb.Extents = BoundAABB.Extents;
			
			obb.CompleteExtAxis();
			
			for (i = 0; i < 2; i++)
				for (j = 0; j < 2; j++)
					for (int k = 0; k < 2; k++)
					{
						vertexP[i][j][k] = vExtents;
						if ( i == 0)
							vertexP[i][j][k].x = -vertexP[i][j][k].x;
						if(j == 0)
							vertexP[i][j][k].y = -vertexP[i][j][k].y;
						if (k == 0)
							vertexP[i][j][k].z = -vertexP[i][j][k].z;
						
						vertexP[i][j][k] = vertexP[i][j][k]*matInverse;
					}
		}
	};

	class CapusuleBrushTraceInfo: public AbstractBrushTraceInfo 
	{
		static int stype;
		virtual int* GetType() { return &stype; }
	public:
		static bool IsInstance(AbstractBrushTraceInfo* pInfo) { return &stype == pInfo->GetType(); }
	public:
		//////////////////////////////////////////////////////////////////////////
		//	In
		//////////////////////////////////////////////////////////////////////////
		float  fRadius; //Capsule radius 
		float fHeight;//Capsule height
		A3DVECTOR3 vUp;//Capsule up Direction

	private:
		//////////////////////////////////////////////////////////////////////////
		//	interim
		//////////////////////////////////////////////////////////////////////////
		A3DVECTOR3 vRight;
		A3DVECTOR3 vFront;
		A3DMATRIX4 matInverse;
		A3DVECTOR3 p2;//移动终点p2 Brush坐标系下
		A3DVECTOR3 p1;//初始点p1 Brush坐标系下
		float fHalfHeight;
		A3DVECTOR3 Up;//capsule上端点
		A3DVECTOR3 Down;//capsule下端点
		A3DVECTOR3 StartTop;//胶囊中心线的Up
		A3DVECTOR3 StartDown;
		A3DVECTOR3 EndTop;
		A3DVECTOR3 EndDown;

	public:
		//////////////////////////////////////////////////////////////////////////
		//	Out
		//////////////////////////////////////////////////////////////////////////
		
		//start, delta world coordinate; height: height for cylinder part; mat convex hull brush translation and rotation(without scale)
		void Init(const A3DVECTOR3& start, const A3DVECTOR3& delta,  float radius,  float height, const A3DMATRIX4& mat = A3DMATRIX4(A3DMATRIX4::IDENTITY), 
			const A3DVECTOR3& up = A3DVECTOR3(0, 1, 0), float scale = 1, float epsilon = 0.03125f ){
			vStart = start;
			vDelta = delta;
			fRadius = radius;
			fDistEpsilon = epsilon;
			fFraction = 100.0f;
			fHeight = height;
			bStartSolid = false;
			bAllSolid = false;
			dwUser1 = 0;
			dwUser2 = 0;
			pUser1 = NULL;

			fScale = scale;
			
			this->vUp = up;
			SetTransMat(mat, up);
			
		}
		virtual void SetTransMat(const A3DMATRIX4& mat, const A3DVECTOR3& up)
		{
			this->mat = mat;
			this->vUp = up;
			matInverse = mat;
			matInverse.InverseTM();
			p1 = vStart,p2 = vStart+vDelta;
			//转换到brush坐标系下
			p1 = p1 * matInverse;
			p2 = p2 * matInverse;
			matInverse._41 = 0;
			matInverse._42 = 0;
			matInverse._43 = 0;
			vUp = vUp * matInverse;
			
			A3DVECTOR3 g_vAxisX(1,0,0);
			A3DVECTOR3 g_vAxisZ(0,0,1);
			
			vUp.Normalize();
			vRight = CrossProduct(vUp, g_vAxisX);
			if (vRight.SquaredMagnitude() < 1E-4f)
				vRight = CrossProduct(vUp, g_vAxisZ);
			vFront = CrossProduct(vRight, vUp);
			fHalfHeight = fHeight*0.5f;
			
			Up = vUp * fHalfHeight;//起始点处，胶囊中心线上端点
			Down = -Up;//起始点处，胶囊中心线下端点
			
			StartTop = p1 + Up, StartDown = p1 + Down, EndTop = p2 + Up, EndDown = p2 + Down;
			
			A3DVECTOR3 vExtents(fRadius, fRadius + fHalfHeight, fRadius);
			A3DOBB obb;
			obb.Clear();
			obb.Center = vStart;
			obb.Extents = vExtents;
			obb.XAxis = vRight;
			obb.ZAxis = vFront;
			obb.YAxis = vUp;
			obb.CompleteExtAxis();			
			BoundAABB.Build(obb);
			
			obb.Center = vStart + vDelta;
			obb.CompleteExtAxis();
			A3DAABB aabb;
			aabb.Build(obb);
			BoundAABB.Merge(aabb);
			BoundAABB.Extents += A3DVECTOR3(fDistEpsilon, fDistEpsilon, fDistEpsilon);
			BoundAABB.CompleteMinsMaxs();
		}
		friend class CConvexBrush;
		friend bool CapsuleAABBCollision(CapusuleBrushTraceInfo* pInfo, const A3DAABB& aabb);
};

class CConvexBrush  
{		
public:
    
	//短边阈值，特别短的边不需要加Bevel处理
	static float fShortEdge;

	//trace
	bool BrushTrace(AbstractBrushTraceInfo *pInfo, bool bCheckFlag = true) const;
	
	

	//import from convex hull data	
	void Import(CConvexHullData *pCH);

	//release all the resource
	void Release();

	//load and save
	bool Load(AFile* InFile);
	bool Save(AFile* OutFile);

	//get brush aabb (brush coordinate)
	const A3DAABB& GetAABB() { return m_aabb; }

	A3DAABB GetTransformedAABB(const A3DMATRIX4& mat) const;

	//construction and deconstruction
	CConvexBrush();
	virtual ~CConvexBrush();

	//user define data
	void SetUserData1(DWORD dwUser1){ m_dwUser1 = dwUser1; }
	DWORD GetUserData1() { return m_dwUser1; }
	void SetUserData2(DWORD dwUser2){ m_dwUser2 = dwUser2; }
	DWORD GetUserData2() { return m_dwUser2; }
	void SetUserData3(DWORD dwUser3){ m_dwUser3 = dwUser3; }
	DWORD GetUserData3() { return m_dwUser3; }

	//collision flag
	void SetCollisionFlag(DWORD collisionFlag) { m_dwCollisionFlag = collisionFlag; }
	DWORD GetCollisionFlag() { return m_dwCollisionFlag; }

	int GetVertexNum() const { return m_Vertices.size(); }
	A3DVECTOR3 GetVertex(int index) const { assert(index >= 0 && index < (int)m_Vertices.size()); return m_Vertices[index]; }

//#ifdef _DEBUG
	void DebugRender(A3DFlatCollector* pFC, float fScale, DWORD dwFColor=0xa0ffff00, const A3DVECTOR3& vRayDir=A3DVECTOR3(1.0f,1.0f,1.0f), const A3DMATRIX4* pTransMatrix=NULL);
//#endif

	CConvexBrush(const CConvexBrush& brush);
	CConvexBrush& operator=(const CConvexBrush& brush);

protected:

	//assign value
	void Assign(const CConvexBrush& brush);

	//capsule brush collision
	bool CapusuleBrushTrace(CapusuleBrushTraceInfo * pInfo, bool bCheckFlag = true) const;
	
	//AABB brush collision
	bool AABBBrushTrace(AABBBrushTraceInfo2 * pInfo, bool bCheckFlag = true) const;
	
	//ray brush collision
	bool RayBrushTrace(RayBrushTraceInfo* pInfo, bool bCheckFlag = true) const;

	struct EdgeInfo{// edge and his neighbor face
		int v1ID;
		int v2ID;
		CHalfSpace* face1;
		CHalfSpace* face2;
		A3DVECTOR3 vDir;
		float fEdgelen;
	};

	struct SideInfo{
		enum SideType{ PLANE = 0, ANGLEBEVEL };
		CHalfSpace hs;
		SideType type;//
	};
	
	abase::vector<A3DVECTOR3> m_Vertices;		                   //vertex
	abase::vector<int> m_vSharpVertices;                           //sharp vertex
	abase::vector<EdgeInfo>   m_edges;                             //edge info
	abase::vector<SideInfo>   m_Sides;                             //halfspace used to collision detection
		
	int                m_nSides;								   //num of halfspace 
	A3DAABB m_aabb;                                                //aabb

	typedef abase::vector<int> FACEVLIST;//face vertex list
	abase::vector<FACEVLIST>    m_faces;

	DWORD   m_dwVersion;

	DWORD   m_dwCollisionFlag;                                    //collision flag inherit from convexhulldata
	DWORD   m_dwUser1;
	DWORD   m_dwUser2;
	DWORD   m_dwUser3;

	class ConvexHullInterpreter: private CConvexHullData{

	private:
		ConvexHullInterpreter(CConvexHullData* pConvexHull);
		~ConvexHullInterpreter();
		//find neighbor face
		CFace* GetNeighborFace(CFace* face, int vID1, int vID2);
		
		//find all neighbor face of a vertex
		abase::vector<CFace*> GetNeighborFace(int VID);
		
		//get the normal of a vertex
		A3DVECTOR3 GetVNormal(int VID);
		
		//get face index
		int GetFaceIndex(CFace* face);

		int GetVertexNum() const { return m_pConvexHull->GetVertexNum(); }
		int GetFaceNum() const { return m_pConvexHull->GetFaceNum();}
		CFace& GetFace(int fid) const { return m_pConvexHull->GetFace(fid);	}
		CFace* GetFacePtr(int fid)  const { return m_pConvexHull->GetFacePtr(fid);}
		bool GetAABB(A3DAABB& aabb) { return m_pConvexHull->GetAABB(aabb); }
		A3DVECTOR3 GetVertex(int vid) const { return m_pConvexHull->GetVertex(vid);}

	private:
		CConvexHullData* m_pConvexHull;//
		friend class CConvexBrush;
	};

};

inline bool CapsuleAABBCollision(CapusuleBrushTraceInfo* pInfo, const A3DAABB& aabb)
{
	float epslon = pInfo->fRadius + pInfo->fDistEpsilon;
	float left = aabb.Mins.x - epslon, right = aabb.Maxs.x + epslon;
	float front = aabb.Mins.z - epslon, back = aabb.Maxs.z + epslon;
	float top = aabb.Maxs.y + epslon, bottom = aabb.Mins.y - epslon;

	if(pInfo->StartTop.x > right && pInfo->EndTop.x > right && pInfo->StartDown.x > right && pInfo->EndDown.x > right 
		||pInfo->StartTop.x < left && pInfo->EndTop.x < left && pInfo->StartDown.x < left && pInfo->EndDown.x < left
		||pInfo->StartTop.y > top && pInfo->EndTop.y > top && pInfo->StartDown.y > top && pInfo->EndDown.y > top
		||pInfo->StartTop.y < bottom && pInfo->EndTop.y < bottom && pInfo->StartDown.y < bottom && pInfo->EndDown.y < bottom
		||pInfo->StartTop.z < front && pInfo->EndTop.z < front && pInfo->StartDown.z < front && pInfo->EndDown.z < front
		||pInfo->StartTop.z > back && pInfo->EndTop.z > back && pInfo->StartDown.z > back && pInfo->EndDown.z > back)
		return false;

	return true;
}

inline bool RayAABBCollision(RayBrushTraceInfo* pInfo, const A3DAABB& aabb)
{
	float left = aabb.Mins.x - pInfo->fDistEpsilon, right = aabb.Maxs.x + pInfo->fDistEpsilon;
	float front = aabb.Mins.z - pInfo->fDistEpsilon, back = aabb.Maxs.z + pInfo->fDistEpsilon;
	float top = aabb.Maxs.y + pInfo->fDistEpsilon, bottom = aabb.Mins.y - pInfo->fDistEpsilon;
	if (pInfo->p1.x > right && pInfo->p2.x > right || pInfo->p1.x < left && pInfo->p2.x < left
		||pInfo->p1.y > top && pInfo->p2.y > top || pInfo->p1.y < bottom && pInfo->p2.y < bottom
		||pInfo->p1.z > back && pInfo->p2.z > back || pInfo->p1.z < front && pInfo->p2.z < front)
		return false;
	return true;
}

/*从一个矩阵分解出旋转及平移矩阵RotTrans和缩放因子scale
传入参数： AbsMatrix包括旋转平移及缩放，注意：必须是三轴等比缩放
传出参数： RotAndTrans旋转及平移部分
传出参数： fScale缩放因子
其中:

  A3DMATRIX4 Scale; Scale.Scale(fScale, fScale, fScale);
  AbsMatrix =Scale * RotAndTrans; 
*/
inline void DecomposeMatrix(const A3DMATRIX4& AbsMatrix, A3DMATRIX4& RotAndTrans, float& fScale)
{
	fScale = AbsMatrix.GetCol(0).Magnitude();
	
	RotAndTrans = AbsMatrix; 
	
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			RotAndTrans.m[i][j]=RotAndTrans.m[i][j]/fScale;
}

}
#endif // !defined(AFX_CONVEXBRUSH_H__3E373194_C7F6_4314_BC7B_BAC075BA4C87__INCLUDED_)
