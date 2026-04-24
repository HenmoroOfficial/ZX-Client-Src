// ObstructOperation.cpp: implementation of the CObstructOperation class.
//
//////////////////////////////////////////////////////////////////////

#include "global.h"
#include "A3D.h"
#include "elementeditor.h"

#include "BrushTreeOperation .h"
#include "MainFrm.h"
#include "TerrainLayer.h"

#include "Bitmap.h"
#include "SceneObjectManager.h"
#include "Render.h"
#include "TerrainRender.h"
#include "OrthoViewport.h"
#include "EL_Building.h"

#include "pathmap\BlockImage.h"
#include "ConvexHullData.h"
#include "HullPropertyDlg.h"

#include "PathMap/PlayerPassMapGenerator.h"
#include "PathMap/PlayerAirPassMapGenerator.h"
#include "Box3D.h"
#include "BrushTree.h"


//#define new A_DEBUG_NEW

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBrushTreeOperation::CBrushTreeOperation()
{
	m_pData = 0;
	m_bStartPaint = false;
	m_bEndAddLine = false;
	m_pBrush = new CTerrainStretchBrush();
	m_pBrush->SetBrushColor(A3DCOLORRGB(255,0,0));
	m_pData = new BYTE[OBS_MAP_WIDTH*OBS_MAP_WIDTH*4];
	ASSERT(m_pData);
	memset(m_pData,255,OBS_MAP_WIDTH*OBS_MAP_WIDTH*4);
	m_pSprite = 0;
	m_vHitPos.Clear();
}

CBrushTreeOperation::~CBrushTreeOperation()
{
}




bool CBrushTreeOperation::OnLButtonDown(int x, int y, DWORD dwFlags)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return true;
	CBrushTree *pBrushTree = pMap->GetBrushTree();

	A3DVECTOR3 vPos((float)x, (float)y, 1.0f);
	A3DCamera*pCamera = (A3DCamera*)(g_Render.GetA3DEngine())->GetActiveCamera();
	A3DViewport* pViewport =g_Render.GetA3DEngine()->GetActiveViewport();
	A3DVECTOR3 vStart((float)x, (float)y, 0.0f);
	pViewport->InvTransform(vStart,vStart);
	pViewport->InvTransform(vPos,vPos);
	
	CBrushTree::BrushNode *pNode = pBrushTree->Pickup(vPos);
	if(pNode == NULL) return true;

	if(m_nOperationType == BRUSH_CHECK)
	{
		pNode->RoI = true;
	}else if(m_nOperationType == BRUSH_UNCHECK)
	{
		pNode->RoI = false;
	}else if(m_nOperationType == BRUSH_SPLIT)
	{
		pBrushTree->Split(pNode);
	}else if( m_nOperationType == BRUSH_MERGE )
	{
		pBrushTree->Merge(pNode);
	}

	return true;
}

bool CBrushTreeOperation::OnLButtonUp(int x,int y, DWORD dwFlags)
{
	m_bStartPaint = false;
	/*
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		A3DVECTOR3 vPos((float)x, (float)y, 1.0f);
		A3DCamera*pCamera = (A3DCamera*)(g_Render.GetA3DEngine())->GetActiveCamera();
		A3DViewport* pViewport =g_Render.GetA3DEngine()->GetActiveViewport();
		A3DVECTOR3 vStart((float)x, (float)y, 0.0f);
		pViewport->InvTransform(vStart,vStart);
		pViewport->InvTransform(vPos,vPos);
		CSceneObjectManager *pMan = pMap->GetSceneObjectMan();
		
		if( m_nOperationType == OBSTRUCT_SET_SEED)
		{
			// now we start to generate the passmap according to the picked point!
			if(IDCANCEL ==AfxMessageBox("我们将根据你点选的位置扩展可达图，这可能将花费很长时间！确定这么做吗？",MB_OKCANCEL))
				return true;
			
			// firtly we ray trace with terrain
			CTerrain *pTerrain =  pMap->GetTerrain();
			CTerrainRender *pTR = pTerrain->GetRender();
			ARectF rc = pTerrain->GetTerrainArea();
			RAYTRACERT TraceRt;	//	Used to store trace result
			bool bTraceTerrain =pTR->RayTrace(vStart,vPos - vStart,1.0f,&TraceRt);
			
			// then we ray trace with building
			CSceneObjectManager *pMan = pMap->GetSceneObjectMan();
			A3DVECTOR3 vecHitPos,vecNormal;
			CConvexHullData *pTraceCHData = NULL;
			float fFraction = 1.0f;
			
			// we do the exact RayTrace, that is we trace the mesh of the building!
			bool bTraceBuilding = pMan->RayTraceStaticModel(vStart,vPos - vStart,vecHitPos,&fFraction,vecNormal,pTraceCHData,true);
			if(bTraceBuilding)
			{
				if(bTraceTerrain && TraceRt.fFraction < fFraction )
					vecHitPos = TraceRt.vHitPos;
			}
			else
			{
				if(bTraceTerrain)
					vecHitPos = TraceRt.vHitPos;
				else
					return true;	// trace nothing!
			}
			
			// now we get the Hit pos and start pass map expanding!
			PLAYER_SAMPLE_POS seedpos;
			pMap->GetPlayerAirPassMapGen()->Transf3DTo2D(vecHitPos,seedpos);
			
// 			if (!pMap->GetPlayerAirPassMapGen()->IsSeedPosValid(seedpos))
// 			{
// 				AfxMessageBox("你点选的位置不合法，请重新选择",MB_OK);
// 				return false;
// 			}
			pMap->GetPlayerAirPassMapGen()->SetCalScale(g_Configs.iBrushRadius);

			pMap->GetPlayerAirPassMapGen()->ExpandPassRegion(seedpos);
			
			m_vHitPos = vecHitPos;
			return true;
		}
	}
	*/
	return true;
}

bool CBrushTreeOperation::OnMouseMove(int x,int y,DWORD dwFlags)
{
	/*
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap &&
	   (m_nOperationType == OBSTRUCT_RED ||
	   m_nOperationType == OBSTRUCT_GREEN ||
	   m_nOperationType == OBSTRUCT_YELLOW ||
	   m_nOperationType == OBSTRUCT_RESET) )	
	{
		CTerrain *pTerrain =  pMap->GetTerrain();
		CTerrainRender *pTR = pTerrain->GetRender();
		ARectF rc = pTerrain->GetTerrainArea();
		A3DVECTOR3 vPos((float)x, (float)y, 1.0f);
		A3DCamera*pCamera = (A3DCamera*)(g_Render.GetA3DEngine())->GetActiveCamera();
		A3DViewport* pViewport =g_Render.GetA3DEngine()->GetActiveViewport();
		A3DVECTOR3 vStart((float)x, (float)y, 0.0f);
		pViewport->InvTransform(vStart,vStart);
		pViewport->InvTransform(vPos,vPos);
		
		RAYTRACERT TraceRt;	//	Used to store trace result
		bool bTraceTerrain = pTR->RayTrace(vStart,vPos - vStart,1.0f,&TraceRt);
		if(bTraceTerrain)
		{
			m_pBrush->SetCenter(TraceRt.vHitPos.x,TraceRt.vHitPos.z);
			if( m_bStartPaint)
			{
				UCHAR *pData = pMap->GetPlayerAirPassMapGen()->GetPassMap();
				if((m_nOperationType == OBSTRUCT_RED) ||
					(m_nOperationType == OBSTRUCT_GREEN) ||
					(m_nOperationType == OBSTRUCT_RESET) )
				{
					UpdateObsData();
				}
			}
			m_vLineEnd = TraceRt.vHitPos;
		}
		
		// a special handle for the case OBSTRUCT_YELLOW
		if(m_nOperationType == OBSTRUCT_YELLOW && m_bStartPaint)
		{
			// now we ray trace with building
			CSceneObjectManager *pMan = pMap->GetSceneObjectMan();
			A3DVECTOR3 vecHitPos,vecNormal;
			CConvexHullData *pTraceCHData = NULL;
			float fFraction = 1.0f;
			
			// we do the exact RayTrace, that is we trace the mesh of the building!
			//bool bTraceBuilding = pMan->RayTraceStaticModel(vStart,vPos - vStart,vecHitPos,&fFraction,vecNormal,pTraceCHData,true);
			// trace only convex hull [12/15/2006 kuiwu]
			bool bTraceBuilding = pMan->RayTraceStaticModel(vStart,vPos - vStart,vecHitPos,&fFraction,vecNormal,pTraceCHData);
			bool bTerrainFirst = false;
			if(bTraceBuilding)
			{
				if(bTraceTerrain && TraceRt.fFraction < fFraction )
				{
					vecHitPos = TraceRt.vHitPos;
					bTerrainFirst = true;
				}
				
			}
			else
			{
				if(bTraceTerrain)
				{
					vecHitPos = TraceRt.vHitPos;
					bTerrainFirst = true;
				}
				else
					return true;	// trace nothing!
			}
			
			if( bTerrainFirst)
			{
				vecHitPos.y = 0.0f;
			}
			else
			{

				CTerrain *pTerrain =  pMap->GetTerrain();
				vecHitPos.y -= pTerrain->GetPosHeight(vecHitPos);
			}

			PLAYER_SAMPLE_POS mapPos;
			pMap->GetPlayerAirPassMapGen()->Transf3DTo2D(vecHitPos,mapPos);
			pMap->GetPlayerAirPassMapGen()->SetPosReachable(mapPos);
		}
		
		m_pSprite->UpdateTextures(m_pData,OBS_MAP_WIDTH*3,A3DFMT_X8R8G8B8);
	}*/
	return true;
}

void CBrushTreeOperation::UpdateObsData()
{
	/*
	float m_fRadius = g_Configs.iBrushRadius;
	float fCenterX,fCenterZ;
	m_pBrush->GetCenter(fCenterX,fCenterZ);
	
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		pMap->SetModifiedFlag(true);
		CTerrain::HEIGHTMAP* pHeightMap = pMap->GetTerrain()->GetHeightMap();
		CTerrain *pTerrain =  pMap->GetTerrain();
		
		if(!pHeightMap) return;
		if(!pTerrain)	return;
		
		ARect<float> updateRect;
		updateRect.left = fCenterX - m_fRadius;
		updateRect.right = fCenterX + m_fRadius;
		updateRect.top = fCenterZ + m_fRadius;
		updateRect.bottom = fCenterZ - m_fRadius;
		float m_fTileSize = pTerrain->GetTileSize()/2.0f;
		ARectF m_rcTerrain = pTerrain->GetTerrainArea();

		int NumX = (updateRect.right - updateRect.left)/m_fTileSize;
		int NumZ = (updateRect.top - updateRect.bottom)/m_fTileSize;

		float fcx = NumX/2.0f;
		float fcz = NumZ/2.0f;
		
		float startZ = updateRect.bottom;
		for(int i=0; i<NumZ; i++)
		{
			float startX = updateRect.left;
			for(int n=0; n<NumX; n++)
			{
				if(g_Configs.bCircleBrush)
				{
					float deltaX = n - fcx;
					float deltaZ = i - fcz;
					
					float fDistance =  (float)sqrt(deltaX*deltaX+deltaZ*deltaZ);
					if(fDistance >= fcx) continue;
				}

				float dx = startX + n*m_fTileSize;
				float dz = startZ + i*m_fTileSize;
				float dy = pTerrain->GetPosHeight(A3DVECTOR3(dx,0,dz));
				//////////////////////////////////////////////////////////////////////////
				// revised by wenfeng, 05-8-19
				A3DVECTOR3 vpos = A3DVECTOR3(dx,0.0f,dz);

				PLAYER_SAMPLE_POS vsc;
				pMap->GetPlayerAirPassMapGen()->Transf3DTo2D(vpos,vsc);
				a_Clamp(vsc.u, 0, 1023);
				a_Clamp(vsc.v, 0, 1023);
				//////////////////////////////////////////////////////////////////////////
				// in new SetPosReachable method, vsc.h should be the delta height with terrain!
				if (m_nOperationType == OBSTRUCT_RED)
				{
					pMap->GetPlayerAirPassMapGen()->SetPosUnreachable(vsc);
				}
				else if (m_nOperationType == OBSTRUCT_GREEN)
				{
					pMap->GetPlayerAirPassMapGen()->SetPosReachable(vsc);
				}
				else if (m_nOperationType == OBSTRUCT_RESET)
				{
					pMap->GetPlayerAirPassMapGen()->Reset(vsc);
				}
			}
		}

	}
	*/
}

void CBrushTreeOperation::Render(A3DViewport* pA3DViewport)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	
	if(pMap)
	{
		CTerrain *pTerrain =  pMap->GetTerrain();
		ARectF rc = pTerrain->GetTerrainArea();
		
		CViewFrame *pFrame = AUX_GetMainFrame()->GetViewFrame();
		CRenderWnd *pRWnd = pFrame->GetRenderWnd();
		if(pRWnd->GetActiveViewport() == VIEW_PERSPECTIVE)
		{
			DrawObstruct();
		}else if(pRWnd->GetActiveViewport() == VIEW_XZ)
		{
			TransObstructToBitmap();

			float fScaleX,fScaleY;
			A3DVECTOR3 vLeftUp,vRightUp,vLeftBottom,vRightBottom;
			g_Render.GetA3DEngine()->GetActiveViewport()->Transform(A3DVECTOR3(rc.left,0,rc.top), vLeftUp);
			g_Render.GetA3DEngine()->GetActiveViewport()->Transform(A3DVECTOR3(rc.right,0,rc.top), vRightUp);
			g_Render.GetA3DEngine()->GetActiveViewport()->Transform(A3DVECTOR3(rc.left,0,rc.bottom), vLeftBottom);
			g_Render.GetA3DEngine()->GetActiveViewport()->Transform(A3DVECTOR3(rc.right,0,rc.bottom), vRightBottom);

			float w,h;
			w = vRightUp.x - vLeftUp.x + 1;
			h = vLeftBottom.y - vLeftUp.y + 1;
			
			m_pSprite->UpdateTextures(m_pData,OBS_MAP_WIDTH*4,A3DFMT_X8R8G8B8);

			float		x, y;
			int			nX, nY;

			float		wc = w / OBS_MAP_DIVIDE;
			float		hc = h / OBS_MAP_DIVIDE;

			y = vLeftUp.y;
			for(int i=0; i<OBS_MAP_DIVIDE; i++)
			{
				x = vLeftUp.x;
				nY = (int) y;
				for(int j=0; j<OBS_MAP_DIVIDE; j++)
				{
					nX = (int) x;
					m_pSprite->SetPosition(nX, nY);

					fScaleX = (float)(int(x + wc) - nX) / (OBS_MAP_WIDTH /OBS_MAP_DIVIDE);
					fScaleY = (float)(int(y + hc) - nY) / (OBS_MAP_WIDTH /OBS_MAP_DIVIDE);

					m_pSprite->SetScaleX(fScaleX);
					m_pSprite->SetScaleY(fScaleY);
					m_pSprite->SetCurrentItem(i * OBS_MAP_DIVIDE + j);
					m_pSprite->DrawToBack();	

					x += wc;
				}

				y += hc;
			}
			

			
#ifdef _DEBUG
			//debug
			AString str;
			str.Format("Sprite Position(x=%d,y=%d)",(int)vLeftUp.x,(int)vLeftUp.y);
			g_Render.TextOut(8,40,str,A3DCOLORRGB(255,255,255));
			str.Format("Sprite Scale   (sx=%f,sy=%f)",fScaleX,fScaleY);
			g_Render.TextOut(8,50,str,A3DCOLORRGB(255,255,255));


#endif 
		}
	}
}

void CBrushTreeOperation::Init(int type)
{
	//Release();
	if(m_pSprite==0) m_pSprite = new A2DSprite; 
	m_pSprite->Release();
	m_nOperationType = type;
	//m_pData = new BYTE[OBS_MAP_WIDTH*OBS_MAP_WIDTH*3];
	//ASSERT(m_pData);
	//memset(m_pData,255,OBS_MAP_WIDTH*OBS_MAP_WIDTH*3);
	A3DRECT rc[OBS_MAP_DIVIDE*OBS_MAP_DIVIDE];
	int nWidth = OBS_MAP_WIDTH/OBS_MAP_DIVIDE;
	for(int i = 0; i < OBS_MAP_DIVIDE; i++)
		for( int j = 0; j < OBS_MAP_DIVIDE; j++)
			rc[i*OBS_MAP_DIVIDE + j] = A3DRECT(nWidth*j,i*nWidth,nWidth*(j+1),nWidth*(i+1));
	m_pSprite->InitWithoutSurface(g_Render.GetA3DDevice(),OBS_MAP_WIDTH,OBS_MAP_WIDTH,A3DFMT_X8R8G8B8,OBS_MAP_DIVIDE*OBS_MAP_DIVIDE,rc);
	m_pSprite->SetColor(A3DCOLORRGBA(255,255,255,120));

	m_bEndAddLine = false;
}

void CBrushTreeOperation::SaveObsToFile(CString strPathName)
{
	if(m_pData==NULL) return;
	
	CELBitmap bt;
	BYTE *pDst = bt.GetBitmapData();
	if(pDst==NULL) return;
	
	bt.CreateBitmap(OBS_MAP_WIDTH,OBS_MAP_WIDTH,32,NULL);
	for( int i = 0; i < OBS_MAP_WIDTH; i++)
	{
		for( int j = 0; j < OBS_MAP_WIDTH; j++)
		{
			pDst[i*OBS_MAP_WIDTH*4 + j*4] = m_pData[i*OBS_MAP_WIDTH*4 + j*4];
		}
	}
	bt.SaveToFile(strPathName);
}
	
void CBrushTreeOperation::DrawGrid(A3DVECTOR3 v1, A3DVECTOR3 v2, A3DVECTOR3 v3, A3DVECTOR3 v4,int nSel)
{
	DWORD clr;
	if(nSel == 0) clr = A3DCOLORRGB(0,0,255);
	else if(nSel == 1)
	{
		clr =A3DCOLORRGB(0,255,0);
	}
	else clr = A3DCOLORRGB(255,0,0);
	DrawLine(v1,v2,clr);
	//DrawLine(v2,v3,clr);
	//DrawLine(v3,v4,clr);
	DrawLine(v4,v1,clr);
}

void CBrushTreeOperation::DrawLine(A3DVECTOR3 v1,A3DVECTOR3 v2, DWORD clr)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CTerrain::HEIGHTMAP* pHeightMap = pMap->GetTerrain()->GetHeightMap();
		CTerrain *pTerrain =  pMap->GetTerrain();
		ARectF   m_rcTerrain = pTerrain->GetTerrainArea();
		
		A3DVECTOR3 pVertices[2];
		WORD pIndices[2];
		
		pVertices[0] = A3DVECTOR3(v1.x,v1.y,v1.z);//test
		pVertices[1] = A3DVECTOR3(v2.x,v2.y,v2.z);//test
		
		pIndices[0] = 0;
		pIndices[1] = 1;
		
		A3DWireCollector *pWireCollector = g_Render.GetA3DEngine()->GetA3DWireCollector();
		if(pWireCollector)
		{
			pWireCollector->AddRenderData_3D(pVertices,2,pIndices,2,clr);
		}
	} 
}

void CBrushTreeOperation::DrawObstruct()
{
	float fWidth = 1.0f;
	int   nShowTitle = 20;

	//RayTrace(const A3DVECTOR3& vStart, const A3DVECTOR3& vVel, float fTime, RAYTRACERT* pTraceRt);
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CTerrain *pTerrain =  pMap->GetTerrain();
		
		// Trace the center of FOV
		A3DVECTOR3 vCameraPos = g_Render.GetA3DEngine()->GetActiveCamera()->GetPos();
		A3DVECTOR3 vCameraDir = g_Render.GetA3DEngine()->GetActiveCamera()->GetDir();
		RAYTRACERT rt;
		pTerrain->RayTrace(vCameraPos, 1000.0f*vCameraDir, 1.0f, &rt);
		A3DVECTOR3 cPos=rt.vHitPos;

		float size = pTerrain->GetTerrainSize();
		ARectF rc = pTerrain->GetTerrainArea();
		UCHAR *pData = pMap->GetPlayerAirPassMapGen()->GetPassMap();
		if(pData==NULL) return;
		int nCenterX = (int)((cPos.x - rc.left)/fWidth);
		int nCenterZ = (int)((rc.top - cPos.z)/fWidth);

		int nTotal  = (rc.right - rc.left)/fWidth;

		for( int i = -nShowTitle; i < nShowTitle; i++)
		{
			int h = nCenterX + i;
			if(h >= 0 && h < nTotal)
			{
				for( int j = -nShowTitle; j < nShowTitle; j++)
				{
					int v = nCenterZ + j;
					if( v>=0 && v < nTotal)
					{
						A3DVECTOR3 leftup,rightup,leftbottom,rightbottom;
						leftup.x = rc.left + h*fWidth;
						leftup.z = rc.top - v*fWidth;
						float height = pTerrain->GetPosHeight(leftup);
						leftup.y = height + pTerrain->GetYOffset() + 0.1f + g_Configs.fAirPassMapDelta;

						rightup = leftup + A3DVECTOR3(fWidth-0.01f,0.0f,0.0f);
						leftbottom = leftup + A3DVECTOR3(0.0f,0.0f,-fWidth+0.01f);
						rightbottom = leftup + A3DVECTOR3(fWidth-0.01f,0.0f,-fWidth+0.01f);
					
						height = pTerrain->GetPosHeight(rightup);
						rightup.y = height + pTerrain->GetYOffset() + 0.1f + g_Configs.fAirPassMapDelta;
						height = pTerrain->GetPosHeight(leftbottom);
						leftbottom.y = height + pTerrain->GetYOffset() + 0.1f + g_Configs.fAirPassMapDelta;
						height = pTerrain->GetPosHeight(rightbottom);
						rightbottom.y = height + pTerrain->GetYOffset() + 0.1f + g_Configs.fAirPassMapDelta;
						if(g_Render.GetA3DEngine()->GetActiveCamera()->PointInViewFrustum(leftup))
						{
							int col = OBS_MAP_WIDTH-v-1;
							int row = h;
							//*********************************
							// revised by wenfeng, 06-1-13
							DrawGrid(leftup,rightup,rightbottom,leftbottom, pData[(OBS_MAP_WIDTH-v-1)* OBS_MAP_WIDTH + h]);
						}
					}
				}
			}
		}
#ifndef  _ANGELICA22
		A3DWireCollector *pWireCollector = g_Render.GetA3DEngine()->GetA3DWireCollector();
		if(pWireCollector) pWireCollector->Flush();
#endif
	}
}

void CBrushTreeOperation::Release()
{
	if(m_pData) delete []m_pData;
	if(m_pBrush) delete m_pBrush;
	if(m_pSprite) 
	{
		m_pSprite->Release();
		delete m_pSprite;
	}
	m_pData = NULL;
}

void CBrushTreeOperation::TransObstructToBitmap()
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CBrushTree *pTree = pMap->GetBrushTree();
	
	abase::vector<CBrushTree::BrushNode*> listLeaves;
	pTree->RetrieveLeaves(listLeaves);
	for( int i = 0; i < listLeaves.size(); ++i)
	{
		CBrushTree::BrushNode *pNode = listLeaves[i];
		ARectF rc;
		rc.left = pNode->aabb.Mins.x;
		rc.right = pNode->aabb.Maxs.x;
		rc.top = pNode->aabb.Mins.z;
		rc.bottom = pNode->aabb.Maxs.z;

		UpdateBitmap(rc,pNode->IsRoI());
		
		float top = rc.top;
		rc.top = rc.bottom;
		rc.bottom = top;

		
		A3DVECTOR3 vStart,vEnd;
		g_Render.GetA3DEngine()->GetActiveViewport()->Transform(A3DVECTOR3(rc.left,0,rc.top), vStart);
		g_Render.GetA3DEngine()->GetActiveViewport()->Transform(A3DVECTOR3(rc.right,0,rc.bottom), vEnd);
		
		rc.left = vStart.x;
		rc.right = vEnd.x;
		rc.top = vStart.y;
		rc.bottom = vEnd.y;
		
		g_pA3DGDI->Draw2DLine(A3DPOINT2(rc.left,rc.top),A3DPOINT2(rc.right,rc.top),A3DCOLORRGB(0,255,255) );
		g_pA3DGDI->Draw2DLine(A3DPOINT2(rc.right,rc.top),A3DPOINT2(rc.right,rc.bottom),A3DCOLORRGB(0,255,255) );
		g_pA3DGDI->Draw2DLine(A3DPOINT2(rc.right,rc.bottom),A3DPOINT2(rc.left,rc.bottom),A3DCOLORRGB(0,255,255) );
		g_pA3DGDI->Draw2DLine(A3DPOINT2(rc.left,rc.bottom),A3DPOINT2(rc.left,rc.top),A3DCOLORRGB(0,255,255) );

		
	}

}

void CBrushTreeOperation::UpdateBitmap(ARectF rc, bool bCheck)
{
	float fxstart = rc.left - ( -512.0f);
	float fzstart = rc.top - ( -512.0f);

	float fxlen = rc.right - ( -512.0f);
	float fzlen = rc.bottom - ( -512.0f);

	int ixs = fxstart;
	int izs = fzstart;
	int ixl = fxlen - ixs;
	int izl = fzlen - izs;

	a_Clamp(ixs,0,OBS_MAP_WIDTH);
	a_Clamp(izs,0,OBS_MAP_WIDTH);
	a_Clamp(ixl,0,OBS_MAP_WIDTH);
	a_Clamp(izl,0,OBS_MAP_WIDTH);
	

	for( int i = 0; i < ixl; ++i)
	{
		for( int j = 0; j < izl; ++j)
		{
			int idx = (izs + i)*OBS_MAP_WIDTH*4 + (ixs + j)*4;
			
			if(bCheck)
			{
				m_pData[idx + 0] = 0;
				m_pData[idx + 1] = 0;
				m_pData[idx + 2] = 255;
				m_pData[idx + 3] = 255;
			}else 
			{
				m_pData[idx + 0] = 255;
				m_pData[idx + 1] = 0;
				m_pData[idx + 2] = 0;
				m_pData[idx + 3] = 255;
			}
		}
	}
}

