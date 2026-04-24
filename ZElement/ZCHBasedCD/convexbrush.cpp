// ConvexBrush.cpp: implementation of the CConvexBrush class.
//
//////////////////////////////////////////////////////////////////////

#include "ConvexBrush.h"

#include "CombGenerator.h"
#include "aabbcd.h"

#include <A3DCollision.h>

#include <AFile.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace CHBasedCD
{

	int AABBBrushTraceInfo2::stype;
	int CapusuleBrushTraceInfo::stype;
	int RayBrushTraceInfo::stype;

	static DWORD version = 0x00000001;//current version
	float CConvexBrush::fShortEdge = 0.1f;

#define ASSERTREADSUCESS(len) { if (len == 0) return false; } 

	CConvexBrush::CConvexBrush()
	{
		
	}

	CConvexBrush::~CConvexBrush()
	{
	}

	void CConvexBrush::Release()
	{		
		m_nSides = 0;
		m_Sides.clear();
		m_Vertices.clear();
		m_edges.clear();
		m_vSharpVertices.clear();	
		m_faces.clear();
		m_dwUser1 = 0;
		m_dwUser2 = 0;
		m_dwUser3 = 0;
	}

	bool CConvexBrush::Load(AFile* InFile)
	{
		Release();
		DWORD ReadVersion;
		const int DWSize = sizeof(DWORD), IntSize = sizeof(int), VectorLen = sizeof(A3DVECTOR3), FloatSize = sizeof(float);
		DWORD readLen;
		InFile->Read(&ReadVersion, DWSize, &readLen);
		ASSERTREADSUCESS(readLen);
		if (ReadVersion == version)
		{
			m_dwVersion = version;

			int i, iNum, iRead;
			A3DVECTOR3 vRead;
			float fRead;
			//read vertex
			InFile->Read(&iNum, IntSize, &readLen);
			ASSERTREADSUCESS(readLen);
			for (i = 0; i < iNum; i++)
			{
				InFile->Read(&vRead, VectorLen, &readLen);
				m_Vertices.push_back(vRead);
			}
			//read sharp vertex
			InFile->Read(&iNum, IntSize, &readLen);
			ASSERTREADSUCESS(readLen);
			for (i = 0; i < iNum; i++)
			{
				InFile->Read(&iRead, IntSize, &readLen);
				m_vSharpVertices.push_back(iRead);
			}
			//read sides
			InFile->Read(&iNum, IntSize, &readLen);
			ASSERTREADSUCESS(readLen);
			m_nSides = iNum;
			SideInfo side;
			for (i = 0; i < iNum; i++)
			{
				InFile->Read(&vRead, VectorLen, &readLen);
				InFile->Read(&fRead, FloatSize, &readLen);

				InFile->Read(&iRead, IntSize, &readLen);
				side.hs.SetNormal(vRead);
				side.hs.SetD(fRead);
				side.type = (SideInfo::SideType)iRead;
				m_Sides.push_back(side);
			}
			//read edges
			InFile->Read(&iNum, IntSize, &readLen);
			ASSERTREADSUCESS(readLen);
			for( i = 0; i < iNum; i++)
			{
				EdgeInfo info;
				InFile->Read(&info.v1ID, IntSize, &readLen);
				InFile->Read(&info.v2ID, IntSize, &readLen);
				InFile->Read(&iRead, IntSize, &readLen);
				info.face1 = &m_Sides[iRead].hs;
				InFile->Read(&iRead, IntSize, &readLen);
				info.face2 = &m_Sides[iRead].hs;
				InFile->Read(&info.vDir, VectorLen, &readLen);
				InFile->Read(&info.fEdgelen, FloatSize, &readLen);
				m_edges.push_back(info);
			}
			//read aabb
			InFile->Read(&m_aabb, sizeof(A3DAABB), &readLen);
			ASSERTREADSUCESS(readLen);

			//read flag and user data
			InFile->Read(&m_dwCollisionFlag, DWSize, &readLen);
			InFile->Read(&m_dwUser1, DWSize, &readLen);
			InFile->Read(&m_dwUser2, DWSize, &readLen);
			InFile->Read(&m_dwUser3, DWSize, &readLen);

			//face info
			InFile->Read(&iRead, IntSize, &readLen);
			ASSERTREADSUCESS(readLen);
//#ifdef _DEBUG
			InFile->Read(&iNum, IntSize, &readLen);
			ASSERTREADSUCESS(readLen);
			abase::vector<int> faceIDs;
			for (i = 0; i < iNum; i++)
			{
				faceIDs.clear();
				int vNum;
				InFile->Read(&vNum, IntSize, &readLen);
				ASSERTREADSUCESS(readLen);
				for (int j = 0; j < vNum; j++)
				{
					InFile->Read(&iRead, IntSize, &readLen);
					faceIDs.push_back(iRead);
				}
				m_faces.push_back(faceIDs);
			}
// #else
// 			InFile->Seek(iRead, AFILE_SEEK_CUR);
// #endif
			return true;
		}
		else
			return false;
	}

	bool CConvexBrush::Save(AFile* OutFile)
	{
		const int DWSize = sizeof(DWORD), IntSize = sizeof(int), VectorLen = sizeof(A3DVECTOR3), FloatSize = sizeof(float);
		DWORD writeLen;
		OutFile->Write(&version, DWSize, &writeLen);
		
		int i, iNum = m_Vertices.size();

		//write vertex
		OutFile->Write(&iNum, IntSize, &writeLen);
		for (i = 0; i < iNum; i++)
		{
			OutFile->Write(&m_Vertices[i], VectorLen, &writeLen);
		}
		//write sharp vertex
		iNum = m_vSharpVertices.size();
		OutFile->Write(&iNum, IntSize, &writeLen);
		for (i = 0; i < iNum; i++)
		{
			OutFile->Write(&m_vSharpVertices[i], IntSize, &writeLen);
		}
		//write sides
		iNum = m_Sides.size();
		OutFile->Write(&iNum, IntSize, &writeLen);
		for (i = 0; i < iNum; i++)
		{
			A3DVECTOR3 Normal = m_Sides[i].hs.GetNormal();
			OutFile->Write(&Normal, VectorLen, &writeLen);
			float d = m_Sides[i].hs.GetDist();
			OutFile->Write(&d, FloatSize, &writeLen);
			int type = (int)m_Sides[i].type;
			OutFile->Write(&type, IntSize, &writeLen);
		}
		//write edges
		iNum = m_edges.size();
		OutFile->Write(&iNum, IntSize, &writeLen);
		for (i = 0; i < iNum; i++)
		{
			OutFile->Write(&m_edges[i].v1ID, IntSize, &writeLen);
			OutFile->Write(&m_edges[i].v2ID, IntSize, &writeLen);
			size_t k;
			for (k = 0; k < m_Sides.size(); k++)
			{
				if (m_edges[i].face1 == &m_Sides[k].hs)
					break;
			}
			if (k >= m_Sides.size()) 
				return false;
			OutFile->Write(&k, IntSize, &writeLen);
			for (k = 0; k < m_Sides.size(); k++)
			{
				if (m_edges[i].face2 == &m_Sides[k].hs)
					break;
			}
			if (k >= m_Sides.size()) 
				return false;
			OutFile->Write(&k, IntSize, &writeLen);

			OutFile->Write(&m_edges[i].vDir, VectorLen, &writeLen);
			OutFile->Write(&m_edges[i].fEdgelen, FloatSize, &writeLen);
		}

		//write AABB
		OutFile->Write(&m_aabb, sizeof(A3DAABB), &writeLen);
		
		//write flag and user data
		OutFile->Write(&m_dwCollisionFlag, DWSize, &writeLen);
		OutFile->Write(&m_dwUser1, DWSize, &writeLen);
		OutFile->Write(&m_dwUser2, DWSize, &writeLen);
		OutFile->Write(&m_dwUser3, DWSize, &writeLen);

		//write face info, for debug render
		int total = 1;//
		for ( i = 0; i < (int)m_faces.size(); i++)
			total += m_faces[i].size() + 1;
		total *= IntSize;
		OutFile->Write(&total, IntSize, &writeLen);
		iNum = m_faces.size();
		OutFile->Write(&iNum, IntSize, &writeLen);
		for (i = 0; i < (int)m_faces.size(); i++)
		{
			iNum = m_faces[i].size();
			OutFile->Write(&iNum, IntSize, &writeLen);
			for (int j = 0; j < (int)m_faces[i].size(); j++)
				OutFile->Write(&m_faces[i][j], IntSize, &writeLen);
		}
		return true;
	}

	void CConvexBrush::Import(CConvexHullData *pCH)
	{
#define FLOAT_EQUAL(a, b, epsilon)    (fabs((a) - (b)) < (epsilon))

		Release();
		int i;

		m_dwCollisionFlag = pCH->GetFlags();		
		ConvexHullInterpreter HullInter(pCH);
		
		for (i = 0; i < HullInter.GetVertexNum(); i++)
		{
			m_Vertices.push_back(HullInter.GetVertex(i));
		}
		
		int nFace = HullInter.GetFaceNum();
		assert(nFace > 0);
		m_nSides = nFace;		
		SideInfo side;
		
		abase::vector<int> FaceIndex;
		
		for (i = 0; i < nFace; i++) {
			A3DVECTOR3 vNormal(HullInter.GetFacePtr(i)->GetNormal());
			side.hs.SetNormal(vNormal);			
			side.hs.SetD(HullInter.GetFacePtr(i)->GetDist());
			side.type = SideInfo::PLANE;
			
			m_Sides.push_back(side);	
			FaceIndex.push_back(i);
		}
		
		bool re = HullInter.GetAABB(m_aabb);
		assert(re);
		
		//添加AABB主轴Bevel
		A3DVECTOR3  vNormal;
		int order = 0;
		float fDist;		
		
		for (int axis=0 ; axis <3 ; axis++){
			for (int dir=-1 ; dir <= 1 ; dir+=2, order++){
				// see if the plane is already present
				for (i=0; i< (int)m_Sides.size(); i++){
					vNormal = m_Sides[i].hs.GetNormal();
					if (FLOAT_EQUAL(dir, vNormal.m[axis], 1E-5f)) {
						break;
					}
				}
				
				if (i == m_Sides.size()){	
					// add a new side
					assert(m_Sides.size() < MAX_FACE_IN_HULL);
					vNormal.Clear();
					vNormal.m[axis] = (float)dir;
					if (dir == 1) {
						fDist = m_aabb.Maxs.m[axis];
					}
					else{
						fDist = -m_aabb.Mins.m[axis];
					}
					side.hs.SetNormal(vNormal);
					side.hs.SetD(fDist);
					side.type = SideInfo::PLANE;
					m_Sides.push_back(side);	
					FaceIndex.push_back(-1);
				}
				
				// if the plane is not in it canonical order, swap it
				if (i != order)	{
					//@note: kuiwu trust the compiler op=
					SideInfo sidetemp = m_Sides[order];
					m_Sides[order] = m_Sides[i];
					m_Sides[i] = sidetemp;
					abase::swap(FaceIndex[order], FaceIndex[i]);
				}
			}
		}
	
		//add Angle Bevel
		for (i = 0; i < HullInter.GetFaceNum(); i++)
		{
			CFace * pFace = HullInter.GetFacePtr(i);
			A3DVECTOR3 normal = pFace->GetNormal();
			int j;
			int vNum = pFace->GetVNum();
			for (j = 0; j < vNum; j++) 
			{
				A3DVECTOR3 v0, v1;
				int index0 = pFace->GetVID(j);
				v0 = HullInter.GetVertex(index0);
				int k = (j +1) % vNum;
				int index1 = pFace->GetVID(k);
				v1 = HullInter.GetVertex(index1);
				
				CFace* pNeigbor = HullInter.GetNeighborFace(pFace, index0, index1);
				if (pNeigbor == NULL)
					continue;
				A3DVECTOR3 normal1 = pNeigbor->GetNormal();
				if (DotProduct(normal, normal1) < -0.33333f)//锐角两面夹角小于一定角度,误差控制为(sqrt(3)-1)r
				{
					A3DVECTOR3 normal2(normal1 - normal);
					A3DVECTOR3 vDir(v1 - v0); 
					normal2 = CrossProduct(normal2, vDir);
					normal2.Normalize();
					if (DotProduct(normal2, normal1 + normal) < 0)
						normal = -normal2;
					else
						normal = normal2;
					
					float fDist = DotProduct(v0, normal);
					fDist = DotProduct(v1, normal);
					
					for (k = 0;  k < (int)m_Sides.size(); k++) {
						
						A3DVECTOR3 vHSNormal(m_Sides[k].hs.GetNormal());
						float      fHSDist = m_Sides[k].hs.GetDist();
						if (FLOAT_EQUAL(vHSNormal.x, normal.x, 0.01)
							&& FLOAT_EQUAL(vHSNormal.y, normal.y, 0.01)
							&& FLOAT_EQUAL(vHSNormal.z, normal.z, 0.01)
							&& FLOAT_EQUAL(fHSDist, fDist, 0.01)) {
							break;
						}
					}
					if (k < (int)m_Sides.size())
						continue;
					for(k = 0; k < (int)m_Vertices.size(); k++)
					{
						float d = DotProduct(m_Vertices[k], normal) - fDist;
						if (d > 1E-4) 
							break; // point in front						
					}
					if (k < (int)m_Vertices.size())
						continue;						
					
					assert(m_Sides.size() < MAX_FACE_IN_HULL);
										
					side.hs.SetNormal(normal);
					side.hs.SetD(fDist);
					side.type = SideInfo::ANGLEBEVEL;
					
					m_Sides.push_back(side);
				}
			}
		}

		
		//Update edge info
		int k1, k2, j;		
		for (int k3 = 0; k3 < HullInter.GetFaceNum(); k3++)
		{
			CFace* face = HullInter.GetFacePtr(k3);
			int vNum = face->GetVNum();
			for (j = 0; j < vNum; j++)
			{
				k1 = face->GetVID(j);
				k2 = face->GetVID((j+1)%vNum);

				if (k1 > k2)
					abase::swap(k1, k2);
				int n;
				for (n = 0; n < (int)m_edges.size(); n++)
				{
					if (m_edges[n].v1ID == k1 && m_edges[n].v2ID == k2)
						break;
				}
				if (n < (int)m_edges.size())
					continue;
				
				CFace *face1 = HullInter.GetNeighborFace(face, k1, k2);
				if (face1 == NULL)
					continue;
				
				EdgeInfo info;
				
				info.v1ID = k1;
				info.v2ID = k2; 
				
				A3DVECTOR3 vDir = m_Vertices[k1] - m_Vertices[k2];
				info.fEdgelen = vDir.Normalize();
				info.vDir = vDir;
				
				for (n = 0; n < (int)FaceIndex.size(); n++)
				{
					if (k3 == FaceIndex[n])
						break;
				}
				info.face1 = &m_Sides[n].hs;
				int faceId = HullInter.GetFaceIndex(face1);
				for (n = 0; n < (int)FaceIndex.size(); n++)
				{
					if (faceId == FaceIndex[n])
						break;
				}
				info.face2 = &m_Sides[n].hs;
				
				m_edges.push_back(info);
			}
		}				

		//update sharp vertex
		for (i = 0; i < HullInter.GetVertexNum(); i++)
		{
			abase::vector<CFace*> faceNeighbor = HullInter.GetNeighborFace(i);//顶点i的邻接面
			A3DVECTOR3 n = HullInter.GetVNormal(i);
			//该顶点的所有邻接面夹角为锐角时需要添加
			for (j = 0; j < (int)faceNeighbor.size(); j++)
			{
				float dpt = DotProduct(faceNeighbor[j]->GetNormal(), n);
				if (DotProduct(faceNeighbor[j]->GetNormal(), n) < 0.5773f)//1/sqrt(3)
					break;
			}
			if (j < (int)faceNeighbor.size())//该顶点为Sharp顶点，需特殊处理
			{
				int k;
				for (k = 0; k < (int)m_vSharpVertices.size(); k++)
					if (m_vSharpVertices[k] == i)
						break;
				if (k == m_vSharpVertices.size())
					m_vSharpVertices.push_back(i);
			}			
		}		

		m_nSides = m_Sides.size();

		//face info, for debug render 
		abase::vector<int>  faceIDs;
		for (i = 0; i < HullInter.GetFaceNum(); i++)
		{
			faceIDs.clear();
			CFace* pFace = HullInter.GetFacePtr(i);
			for (j = 0; j < pFace->GetVNum(); j++)
			{
				faceIDs.push_back(pFace->GetVID(j));
			}
			m_faces.push_back(faceIDs);
		}
#undef  FLOAT_EQUAL
	}

	A3DAABB CConvexBrush::GetTransformedAABB(const A3DMATRIX4& mat) const
	{
		A3DVECTOR3 *pV = new A3DVECTOR3[m_Vertices.size()];
		for (int i = 0; i < (int)m_Vertices.size(); i++)
		{
			pV[i] = m_Vertices[i] * mat;
		}
		A3DAABB aabb;
		aabb.Build(pV, m_Vertices.size());
		delete []pV;
		return aabb;
	}

	//trace
	bool CConvexBrush::BrushTrace(AbstractBrushTraceInfo *pInfo,  bool bCheckFlag ) const
	{
		if (CapusuleBrushTraceInfo::IsInstance(pInfo))
			return CapusuleBrushTrace((CapusuleBrushTraceInfo*)pInfo, bCheckFlag);
		
		if (AABBBrushTraceInfo2::IsInstance(pInfo))
			return AABBBrushTrace((AABBBrushTraceInfo2*)pInfo, bCheckFlag);
		
		if (RayBrushTraceInfo::IsInstance(pInfo))
			return RayBrushTrace((RayBrushTraceInfo*)pInfo, bCheckFlag);
		
		return false;
	}
	
	bool CConvexBrush::AABBBrushTrace(AABBBrushTraceInfo2 * pInfo, bool bCheckFlag) const
	{
		if (bCheckFlag && (m_dwCollisionFlag & CHDATA_SKIP_COLLISION))
			return false;

		A3DAABB aabb(m_aabb);
		float& fScale = pInfo->fScale;
		aabb.Center = m_aabb.Center * fScale;
		aabb.Extents = m_aabb.Extents * fScale;
		aabb.CompleteMinsMaxs();

		if (!CLS_AABBOBBOverlap(aabb, pInfo->obb))
			return false;

		A3DVECTOR3 &p1 = pInfo->p1;
		A3DVECTOR3 &p2 = pInfo->p2;
		A3DVECTOR3& xaxis = pInfo->xaxis;
		A3DVECTOR3& yaxis = pInfo->yaxis;
		A3DVECTOR3& zaxis = pInfo->zaxis;
		A3DMATRIX4&  matInverse = pInfo->matInverse;
		
		float& fscale = pInfo->fScale;
		
		const int& nSides = m_nSides;
		int			i;
		const CHalfSpace	*plane, *clipplane;
		float		dist;
		float		enterfrac, leavefrac;
		A3DVECTOR3	ofs;
		float		d1, d2;
		bool	getout, startout;
		float		f;
		
		
		assert(nSides > 0);
		
		enterfrac = -1;
		leavefrac = 1;
		clipplane = NULL;
		
		
		getout = false;
		startout = false;

		for (i=0; i<nSides; i++)
		{
			if (m_Sides[i].type == SideInfo::ANGLEBEVEL)//锐角Bevel跳过
				continue;

			plane = &(m_Sides[i].hs);
			
			A3DVECTOR3 vNormal = plane->GetNormal();
			
			int xindex = 0, yindex = 0, zindex = 0;

			if (DotProduct(vNormal, xaxis) < 0)
				xindex = 1;
			if (DotProduct(vNormal, yaxis) < 0)
				yindex = 1;
			if (DotProduct(vNormal, zaxis) < 0)
				zindex = 1;
			ofs = pInfo->vertexP[xindex][yindex][zindex];
			
			dist = plane->GetDist() * fscale - DotProduct (ofs, vNormal);
			
			d1 = DotProduct (p1, vNormal) - dist;
			d2 = DotProduct (p2, vNormal) - dist;
			
			if (d2 > 0)
				getout = true;	// endpoint is not in solid
			if (d1 > 0)
				startout = true;
			
			// if completely in front of face, no intersection
			if (d1 > 0 && d2 > 0)
				return false;
			
			if (d1 <= 0 && d2 <= 0)
				continue;
			
			// crosses face
			if (d1 > d2)
			{	// enter
				
				f = (d1 - pInfo->fDistEpsilon)/(d1-d2);
				if (f < 0.0f) {
					f = 0.0f;
				}
				if (f > enterfrac)
				{
					enterfrac = f;
					clipplane = plane;
				}
			}
			else
			{	// leave
				f = (d1+pInfo->fDistEpsilon) / (d1-d2);
				
				if (f > 1.0f) {
					f = 1.0f;
				}
				
				if (f < leavefrac)
					leavefrac = f;
			}
		}

		//生成动态Bevel 
		CHalfSpace hs;
		CHalfSpace HitPlane;//碰撞平面
		const abase::vector<EdgeInfo> *pEdges;
		abase::vector<EdgeInfo> Edges;
		pEdges = &Edges;
		
		if (!startout || enterfrac < leavefrac && enterfrac > -1 && enterfrac < 1)
		{
			pEdges = &m_edges;
			if (clipplane)
				HitPlane = *clipplane;
		}
		for (int k = 0; k < (int)pEdges->size(); k++)
		{
			EdgeInfo info = (*pEdges)[k];
			if ((*pEdges)[k].fEdgelen * fscale < fShortEdge)
				continue;

			for (int m = 0; m < 3; m++)
			{
				
				A3DVECTOR3 vNormal;
				
				switch(m)
				{
				case 0:
					vNormal = CrossProduct((*pEdges)[k].vDir, xaxis);
					break;
				case 1:
					vNormal = CrossProduct((*pEdges)[k].vDir, yaxis);
					break;
				case 2:
					vNormal = CrossProduct((*pEdges)[k].vDir, zaxis);
					break;
				}
				
				
				if (vNormal.SquaredMagnitude() < 1E-4)
					continue;
				hs.SetNormal(vNormal);
				vNormal = hs.GetNormal();

				hs.SetD(DotProduct(vNormal,m_Vertices[(*pEdges)[k].v2ID]));
				
				bool positive = true, negative =true;
				for (int k1 = 0; k1 < (int)m_Vertices.size(); k1++)
				{
					if (k1 != (*pEdges)[k].v1ID && k1 != (*pEdges)[k].v2ID)
					{
						float d = hs.DistV2Plane(m_Vertices[k1]);
						if (d > 1E-4f)				
						{
							positive = false;									
						}
						else if(d < -1E-4f)
						{
							negative = false;	
						}
						if (!positive && !negative)
							break;
					}
				}
				if (positive == negative)
					continue;
				if (!positive)
				{					
					vNormal = -vNormal;
					hs.Inverse();
				}

				int xindex = 0, yindex = 0, zindex = 0;
				
				if (DotProduct(vNormal, xaxis) < 0)
					xindex = 1;
				if (DotProduct(vNormal, yaxis) < 0)
					yindex = 1;
				if (DotProduct(vNormal, zaxis) < 0)
					zindex = 1;
				ofs = pInfo->vertexP[xindex][yindex][zindex];
				
				dist = hs.GetDist() * fscale - DotProduct (ofs, vNormal);
				
				d1 = DotProduct (p1, vNormal) - dist;
				d2 = DotProduct (p2, vNormal) - dist;
				
				if (d2 > 0)
					getout = true;	// endpoint is not in solid
				if (d1 > 0)
					startout = true;
				
				// if completely in front of face, no intersection
				if (d1 > 0 && d2 > 0)
					return false;
				
				if (d1 <= 0 && d2 <= 0)
					continue;
				
				// crosses face
				if (d1 > d2)
				{	// enter
					
					f = (d1 - pInfo->fDistEpsilon)/(d1-d2);
					if (f < 0.0f) {
						f = 0.0f;
					}
					if (f > enterfrac)
					{
						enterfrac = f;
						HitPlane = hs;
					}
				}
				else
				{	// leave
					f = (d1+pInfo->fDistEpsilon) / (d1-d2);
					
					if (f > 1.0f) {
						f = 1.0f;
					}
					
					if (f < leavefrac)
						leavefrac = f;
				}
				
			}
		}

		if (!startout)
		{	// original point was inside brush
			pInfo->bStartSolid= true;
			if (!getout)
				pInfo->bAllSolid = true;
			
			pInfo->fFraction  = 0.0f;
			
			return true;
		}
		
		if (enterfrac < leavefrac)
		{
			if (enterfrac > -1 && enterfrac < 1) 
			{
				if (enterfrac < 0)
					enterfrac = 0;
				pInfo->fFraction= enterfrac;
				pInfo->ClipPlane = HitPlane;
				
				return true;
			}
		}
		
	return false;
	}

	//ray brush collision
	bool CConvexBrush::RayBrushTrace(RayBrushTraceInfo* pInfo, bool bCheckFlag) const
	{
		if (bCheckFlag && (m_dwCollisionFlag & CHDATA_SKIP_RAYTRACE))
			return false;
		A3DAABB aabb(m_aabb);
		float& fScale = pInfo->fScale;
		aabb.Center = m_aabb.Center * fScale;
		aabb.Extents = m_aabb.Extents * fScale;
		aabb.CompleteMinsMaxs();
		
		if (!RayAABBCollision(pInfo, aabb))
			return false;
		const int& nSides = m_nSides;
		int			i;
		const CHalfSpace	*plane, *clipplane;
		float		dist;
		float		enterfrac, leavefrac;
		A3DVECTOR3&	p1 = pInfo->p1;
		A3DVECTOR3& p2 = pInfo->p2;
		float		d1, d2;
		bool	getout, startout;
		float		f;
		
		
		assert(nSides > 0);
		
		enterfrac = -1;
		leavefrac = 1;
		clipplane = NULL;
		
		
		getout = false;
		startout = false;	
		
		for (i=0; i<nSides; i++)
		{
			if (m_Sides[i].type == SideInfo::ANGLEBEVEL)//锐角Bevel跳过
				continue;

			plane = &(m_Sides[i].hs);
			dist = plane->GetDist() * fScale;
			
			A3DVECTOR3 vNormal(plane->GetNormal());
			d1 = DotProduct (p1, vNormal) - dist;
			d2 = DotProduct (p2, vNormal) - dist;
			
			if (d2 > 0)
				getout = true;	// endpoint is not in solid
			if (d1 > 0)
				startout = true;
			
			// if completely in front of face, no intersection
			if (d1 > 0 && d2 > 0)
				return false;
			
			if (d1 <= 0 && d2 <= 0)
				continue;
			
			// crosses face
			if (d1 > d2)
			{	// enter
				
				f = (d1 - pInfo->fDistEpsilon)/(d1-d2);
				if (f < 0.0f) {
					f = 0.0f;
				}
				if (f > enterfrac)
				{
					enterfrac = f;
					clipplane = plane;
				}
			}
			else
			{	// leave
				f = (d1+pInfo->fDistEpsilon) / (d1-d2);
				
				if (f > 1.0f) {
					f = 1.0f;
				}
				
				if (f < leavefrac)
					leavefrac = f;
			}
		}

		if (!startout)
		{	// original point was inside brush
			pInfo->bStartSolid= true;
			if (!getout)
				pInfo->bAllSolid = true;
			
			pInfo->fFraction  = 0.0f;
					
			return true;
		}
		
		if (enterfrac < leavefrac)
		{
			if (enterfrac > -1 && enterfrac < 1) 
			{
				if (enterfrac < 0)
					enterfrac = 0;
				pInfo->fFraction= enterfrac;
				pInfo->ClipPlane = *clipplane;
				
				return true;
			}
		}
		
		return false;
	}

	//
	bool CConvexBrush::CapusuleBrushTrace(CapusuleBrushTraceInfo * pInfo, bool bCheckFlag) const
	{
		if (bCheckFlag && (m_dwCollisionFlag & CHDATA_SKIP_COLLISION))
			return false;
		//胶囊与AABB碰撞测试
		A3DAABB aabb(m_aabb);
		float& fScale = pInfo->fScale;
		aabb.Center = m_aabb.Center * fScale;
		aabb.Extents = m_aabb.Extents * fScale;
		aabb.CompleteMinsMaxs();
		
		if (!CapsuleAABBCollision(pInfo, aabb))
			return false;
		
		
		assert(pInfo->fDistEpsilon >= 1E-2f);
		A3DVECTOR3 &p1 = pInfo->p1;
		A3DVECTOR3 &p2 = pInfo->p2;
		A3DVECTOR3& vUp = pInfo->vUp;
		A3DVECTOR3& right = pInfo->vRight;
		A3DVECTOR3& front = pInfo->vFront;
		A3DMATRIX4&  matInverse = pInfo->matInverse;
		
		A3DVECTOR3& Up = pInfo->Up;//起始点处，胶囊中心线上端点
		A3DVECTOR3& Down = pInfo->Down;//起始点处，胶囊中心线下端点
		
		
		
		const int& nSides = m_nSides;
		int			i;
		const CHalfSpace	*plane, *clipplane;
		float		dist;
		float		enterfrac, leavefrac;
		A3DVECTOR3	ofs;
		float		d1, d2;
		bool	getout, startout;
		float		f;
		
		
		assert(nSides > 0);
		
		enterfrac = -1;
		leavefrac = 1;
		clipplane = NULL;
		
		
		getout = false;
		startout = false;	
		
		for (i=0; i<nSides; i++)
		{
			
			plane = &(m_Sides[i].hs);
			
			A3DVECTOR3 vNormal = plane->GetNormal();
			
			if (DotProduct(vNormal, vUp) > 0)
			{
				ofs = Down;//此时下端点先碰撞
			}
			else
			{
				ofs = Up;
			}
			
			dist = DotProduct (ofs, vNormal);
			dist = plane->GetDist() * fScale - dist + pInfo->fRadius;
			
			d1 = DotProduct (p1, vNormal) - dist;
			d2 = DotProduct (p2, vNormal) - dist;
			
			if (d2 > 0)
				getout = true;	// endpoint is not in solid
			if (d1 > 0)
				startout = true;
			
			// if completely in front of face, no intersection
			if (d1 > 0 && d2 > 0)
				return false;
			
			if (d1 <= 0 && d2 <= 0)
				continue;
			
			// crosses face
			if (d1 > d2)
			{	// enter
				
				f = (d1 - pInfo->fDistEpsilon)/(d1-d2);
				if (f < 0.0f) {
					f = 0.0f;
				}
				if (f > enterfrac)
				{
					enterfrac = f;
					clipplane = plane;
				}
			}
			else
			{	// leave
				f = (d1+pInfo->fDistEpsilon) / (d1-d2);
				
				if (f > 1.0f) {
					f = 1.0f;
				}
				
				if (f < leavefrac)
					leavefrac = f;
			}
		}
		
		CHalfSpace hs;
		CHalfSpace HitPlane;//碰撞平面
		const abase::vector<EdgeInfo> *pEdges;
		abase::vector<EdgeInfo> Edges;
		pEdges = &Edges;
		
		if (!startout || enterfrac < leavefrac && enterfrac > -1 && enterfrac < 1)
		{
			pEdges = &m_edges;
			if (clipplane)
				HitPlane = *clipplane;
		}
		
		//检测与边碰撞情况
		int k;
		for(k = 0; k < (int)pEdges->size(); k++)
		{
			if ((*pEdges)[k].fEdgelen * fScale < fShortEdge)
				continue;
			//主轴方向相同的Bevel
			
			A3DVECTOR3 vNormal = CrossProduct((*pEdges)[k].vDir, vUp);
			
			A3DVECTOR3 vTest(m_Vertices[(*pEdges)[k].v1ID] + vUp);//测试点位置
			
			d1 = (*pEdges)[k].face1->DistV2Plane(vTest);
			d2 = (*pEdges)[k].face2->DistV2Plane(vTest);
			if (d1 < 0 && d2 < 0 || d1 > 0 && d2 > 0 || fabsf(d1) < 1E-3f || fabsf(d2) < 1E-3f)
				continue;
			
			if (vNormal.SquaredMagnitude() < 1E-4)
				continue;
			hs.SetNormal(vNormal);
			vNormal = hs.GetNormal();
			hs.SetD(DotProduct(vNormal, m_Vertices[(*pEdges)[k].v2ID]));
			
			bool positive = true, negative =true;
			for (int k1 = 0; k1 < (int)m_Vertices.size(); k1++)
			{
				if (k1 != (*pEdges)[k].v1ID && k1 != (*pEdges)[k].v2ID)
				{
					float d = hs.DistV2Plane(m_Vertices[k1]);
					if (d > 1E-4f)				
					{
						positive = false;									
					}
					else if(d < -1E-4f)
					{
						negative = false;	
					}
					if (!positive && !negative)
						break;
				}
			}
			if (positive == negative)
				continue;
			if (!positive)
			{
				vNormal = -vNormal;
				hs.Inverse();
			}
			
			dist = hs.GetDist() * fScale + pInfo->fRadius;
			
			d1 = DotProduct (p1, vNormal) - dist;
			d2 = DotProduct (p2, vNormal) - dist;
			
			if (d2 > 0)
				getout = true;	// endpoint is not in solid
			if (d1 > 0)
				startout = true;
			
			// if completely in front of face, no intersection
			if (d1 > 0 && d2 > 0)
				return false;
			
			if (d1 <= 0 && d2 <= 0)
				continue;
			
			// crosses face
			if (d1 > d2)
			{	// enter
				
				f = (d1 - pInfo->fDistEpsilon)/(d1-d2);
				if (f < 0.0f) {
					f = 0.0f;
				}
				if (f > enterfrac)
				{
					enterfrac = f;
					HitPlane = hs;
				}
			}
			else
			{	// leave
				f = (d1+pInfo->fDistEpsilon) / (d1-d2);
				
				if (f > 1.0f) {
					f = 1.0f;
				}
				
				if (f < leavefrac)
					leavefrac = f;
			}		
			
		}
		
		//检测与顶点(sharp)碰撞情况
		int n = 0;
		if (!startout || enterfrac < leavefrac && enterfrac > -1 && enterfrac < 1)
		{
			n = m_vSharpVertices.size();
		}
		for(k = 0; k < n; k++)
		{
			for (int m = 0; m < 2; m++)
			{			
				A3DVECTOR3 vNormal;
				switch(m)
				{
				case 0:
					vNormal = front;
					break;
				case 1:
					vNormal = right;
					break;
				}
				
				if (vNormal.SquaredMagnitude() < 1E-4)
					continue;
				hs.SetNormal(vNormal);
				vNormal = hs.GetNormal();
				hs.SetD(DotProduct(vNormal,m_Vertices[m_vSharpVertices[k]]));			
				
				bool positive = true, negative =true;
				for (int k1 = 0; k1 < (int)m_Vertices.size(); k1++)
				{
					float d = hs.DistV2Plane(m_Vertices[k1]);
					if (d > 1E-4f)				
					{
						positive = false;									
					}
					else if(d < -1E-4f)
					{
						negative = false;	
					}
					if (!positive && !negative)
						break;
				}
				if (positive == negative)
					continue;
				if (!positive)
				{
					vNormal = -vNormal;
					hs.Inverse();
				}
				
				if (DotProduct(vNormal, vUp) > 0)
				{
					ofs = Down;//此时下端点先碰撞
				}
				else
				{
					ofs = Up;
				}
				
				dist = DotProduct (ofs, vNormal);
				dist = hs.GetDist() * fScale - dist + pInfo->fRadius;
				
				d1 = DotProduct (p1, vNormal) - dist;
				d2 = DotProduct (p2, vNormal) - dist;
				
				if (d2 > 0)
					getout = true;	// endpoint is not in solid
				if (d1 > 0)
					startout = true;
				
				// if completely in front of face, no intersection
				if (d1 > 0 && d2 > 0)
					return false;
				
				if (d1 <= 0 && d2 <= 0)
					continue;
				
				// crosses face
				if (d1 > d2)
				{	// enter
					f = (d1 - pInfo->fDistEpsilon)/(d1-d2);
					if (f < 0.0f) {
						f = 0.0f;
					}
					if (f > enterfrac)
					{
						enterfrac = f;
						HitPlane = hs;
					}
				}
				else
				{	// leave
					f = (d1+pInfo->fDistEpsilon) / (d1-d2);
					
					if (f > 1.0f) {
						f = 1.0f;
					}
					
					if (f < leavefrac)
						leavefrac = f;
				}
				
			}
			
			
		}
		if (!startout)
		{	// original point was inside brush
			pInfo->bStartSolid= true;
			if (!getout)
				pInfo->bAllSolid = true;
			
			pInfo->fFraction  = 0.0f;
			
			
			return true;
		}
		
		if (enterfrac < leavefrac)
		{
			if (enterfrac > -1 && enterfrac < 1) 
			{
				if (enterfrac < 0)
					enterfrac = 0;
				pInfo->fFraction= enterfrac;
				pInfo->ClipPlane = HitPlane;
				
				return true;
			}
		}
		
		return false;
	}

//#ifdef _DEBUG
	void CConvexBrush::DebugRender(A3DFlatCollector* pFC, float fScale, DWORD dwFColor, const A3DVECTOR3& vRayDir, const A3DMATRIX4* pTransMatrix)
	{
		if(!pFC) return;
		
		if( m_faces.size() == 0 ) return;
			
		
		//绘制面片
		A3DVECTOR3 RayDir;
		//保留面片色彩dwFColor的alpha值
		DWORD ALPHA=dwFColor & 0xff000000;
		
		Normalize(vRayDir,RayDir);
		
		for(int i=0; i<(int)m_faces.size(); i++)
		{
			int VNum = m_faces[i].size();
			abase::vector<int>& faceIDs = m_faces[i];

			A3DVECTOR3* Vs=new A3DVECTOR3[VNum];
			int IDNum=(VNum-2)*3;
			WORD* IDs=new WORD[IDNum];
			int cur=0;
			//构造顶点和三角形索引
			for(int j=0;j<VNum;j++)
			{
				if(pTransMatrix)
					Vs[j] = m_Vertices[faceIDs[j]] * fScale * (*pTransMatrix);
				else
					Vs[j] = m_Vertices[faceIDs[j]] * fScale;
				if(j<VNum-2)
				{
					IDs[cur]=0;
					IDs[cur+1]=j+1;
					IDs[cur+2]=j+2;
					cur+=3;
				}
			}
			
			//利用光照计算颜色
			A3DCOLORVALUE faceColorV(dwFColor);
			//双向光源，为了区分不同的面片！by wenfeng
			//float fWeight=float(fabs(DotProduct(m_arrFaces[i]->GetNormal(),RayDir)*0.8f));
			// 改为单向光源。  By kuiwu. [21/3/2006]
			A3DVECTOR3 vNormal(CrossProduct(Vs[0] - Vs[1], Vs[1] - Vs[2]));
			vNormal.Normalize();
			float fdot =DotProduct(vNormal, RayDir);
			float fWeight = a_Max(0.0f, fdot) * 0.8f;
			
			faceColorV*=0.2f+fWeight;
			DWORD faceColor;

			if (m_dwCollisionFlag & CHDATA_SKIP_COLLISION)
				faceColor = 0x80800000;
			else
				faceColor=faceColorV.ToRGBAColor() & 0x00ffffff  | ALPHA;
			
			//绘制
			pFC->AddRenderData_3D(Vs,VNum,IDs,IDNum,faceColor);
			
			delete [] IDs;
			delete [] Vs;
		}
	}
//#endif

	CConvexBrush::CConvexBrush(const CConvexBrush& brush)
	{
		Assign(brush);
	}
	CConvexBrush& CConvexBrush::operator=(const CConvexBrush& brush)
	{
		if (&brush == this)
			return *this;
		
		Release();
		Assign(brush);
		return *this;
	}
	
	void CConvexBrush::Assign(const CConvexBrush& brush)
	{
		m_Vertices = brush.m_Vertices;
		m_vSharpVertices = brush.m_vSharpVertices;
		m_edges = brush.m_edges;
		m_Sides = brush.m_Sides;
		
		m_nSides = brush.m_nSides;
		
		m_aabb = brush.m_aabb;
		
		m_faces = brush.m_faces;
		
		
		m_dwVersion = brush.m_dwVersion;
		
		m_dwCollisionFlag = brush.m_dwCollisionFlag;
		m_dwUser1 = m_dwUser1;
		m_dwUser2 = m_dwUser2;
		m_dwUser3 = m_dwUser3;
		
		//update pointer in edgeinfo
		size_t i, j;
		for (i = 0; i < m_edges.size(); i++)
		{
			for (j = 0; j < brush.m_Sides.size(); j++)
			{
				if (&brush.m_Sides[j].hs == brush.m_edges[i].face1)
					break;
			}
			if (j >= brush.m_Sides.size())
				return;//err
			m_edges[i].face1 = &m_Sides[j].hs;
			
			for (j = 0; j < brush.m_Sides.size(); j++)
			{
				if (&brush.m_Sides[j].hs == brush.m_edges[i].face2)
					break;
			}
			if (j >= brush.m_Sides.size())
				return;//err
			m_edges[i].face2 = &m_Sides[j].hs;
		}
	}

	////////////////////////////////////ConvexHullInterpreter//////////////////////////////////////
	CConvexBrush::ConvexHullInterpreter::ConvexHullInterpreter(CConvexHullData* pConvexHull)
	{
		assert(pConvexHull);
		m_pConvexHull = pConvexHull;
	}
	CConvexBrush::ConvexHullInterpreter::~ConvexHullInterpreter()
	{
		
	}
	CFace* CConvexBrush::ConvexHullInterpreter::GetNeighborFace(CFace* face, int vID1, int vID2)
	{
		for (int i = 0; i < m_pConvexHull->GetFaceNum(); i++)
		{
			if (m_pConvexHull->GetFacePtr(i) == face)
				continue;
			int j;
			for (j = 0; j < m_pConvexHull->GetFacePtr(i)->GetVNum() - 1; j++)
			{
				if (m_pConvexHull->GetFacePtr(i)->GetVID(j) == vID1 && m_pConvexHull->GetFacePtr(i)->GetVID(j+1) == vID2
					|| m_pConvexHull->GetFacePtr(i)->GetVID(j) == vID2 && m_pConvexHull->GetFacePtr(i)->GetVID(j+1) == vID1)
				{
					return m_pConvexHull->GetFacePtr(i);
				}
			}
			if (m_pConvexHull->GetFacePtr(i)->GetVID(j) == vID1 && m_pConvexHull->GetFacePtr(i)->GetVID(0) == vID2
				|| m_pConvexHull->GetFacePtr(i)->GetVID(j) == vID2 && m_pConvexHull->GetFacePtr(i)->GetVID(0) == vID1)
			{
				return m_pConvexHull->GetFacePtr(i);
			}
		}
		return NULL;
	}
	abase::vector<CFace*> CConvexBrush::ConvexHullInterpreter::GetNeighborFace(int VID)
	{
		abase::vector<CFace*> re;
		for (int i = 0; i < m_pConvexHull->GetFaceNum(); i++)
		{
			for (int j = 0; j < m_pConvexHull->GetFacePtr(i)->GetVNum(); j++)
			{
				if (m_pConvexHull->GetFacePtr(i)->GetVID(j) == VID)
				{
					re.push_back(m_pConvexHull->GetFacePtr(i));
					break;
				}
			}
		}
		return re;
	}

	//get the normal of a vertex
	A3DVECTOR3 CConvexBrush::ConvexHullInterpreter::GetVNormal(int VID)
	{
		abase::vector<CFace*> re = GetNeighborFace(VID);
		A3DVECTOR3 n;
		n.Clear();
		for (int i = 0; i < (int)re.size(); i++)
		{
			n += re[i]->GetNormal();
		}
		
		n.Normalize();
		return (n);
	}
	int CConvexBrush::ConvexHullInterpreter::GetFaceIndex(CFace* face)
	{
		for (int i = 0; i < m_pConvexHull->GetFaceNum(); i++)
		{
			if (face == m_pConvexHull->GetFacePtr(i))
				return i;
		}
		return -1;
	}
}