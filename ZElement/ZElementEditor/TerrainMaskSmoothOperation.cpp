#include "global.h"
#include "MainFrm.h"
#include "Terrain.h"
#include "TerrainRender.h"
#include "Render.h"
#include "TerrainMaskSmoothOperation.h"
#include "Bitmap.h"
#include "MaskModifier.h"
#include "UndoMan.h"
#include "A3D.h"

bool CTerrainMaskSmoothOperation::OnEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	return true;
}

void CTerrainMaskSmoothOperation::Render(A3DViewport* pA3DViewport)
{
	if(m_pBrush)
		m_pBrush->Render(pA3DViewport);

}

void CTerrainMaskSmoothOperation::Tick(DWORD dwTimeDelta)
{
	if(m_pBrush)
		m_pBrush->Tick(dwTimeDelta);
//	UpdateTerrainHeight();
}


bool CTerrainMaskSmoothOperation::OnMouseMove(int x, int y, DWORD dwFlags) 
{ 
	
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CTerrainRender *pTR = pMap->GetTerrain()->GetRender();
		if(pTR)
		{
			A3DVECTOR3 vPos((float)x, (float)y, 1.0f);
			A3DCamera*pCamera = (A3DCamera*)(g_Render.GetA3DEngine())->GetActiveCamera();
			A3DViewport* pViewport =g_Render.GetA3DEngine()->GetActiveViewport();
			A3DVECTOR3 vStart((float)x, (float)y, 0.0f);
			pViewport->InvTransform(vStart,vStart);
			pViewport->InvTransform(vPos,vPos);
			
			RAYTRACERT TraceRt;	//	Used to store trace result
			if(pTR->RayTrace(vStart,vPos - vStart,1.0f,&TraceRt))
				m_pBrush->SetCenter(TraceRt.vHitPos.x,TraceRt.vHitPos.z);
			if(m_bLMouseDown)
			{
				UpdateMaskMap();
				return false;
			}
		}
	}
	return true;
}

bool CTerrainMaskSmoothOperation::OnLButtonDown(int x, int y, DWORD dwFlags) 
{ 
	
	m_bLMouseDown = true;
	m_ptOldPos.x = x;
	m_ptOldPos.y = y;
	m_rcUpdateRect.Clear();
	if(m_pUndoAction) 
	{
		delete m_pUndoAction;
		m_pUndoAction = NULL;
	}
	StartPaint(true);
	UpdateMaskMap();
	return false; 
}

bool CTerrainMaskSmoothOperation::OnRButtonDown(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

bool CTerrainMaskSmoothOperation::OnLButtonUp(int x, int y, DWORD dwFlags) 
{ 
	m_bLMouseDown = false;
	StartPaint(false);

	if(m_pUndoAction)
	{
		m_pUndoAction->SetUpdateRect(m_rcUpdateRect,m_nCurrentLayer);
		g_UndoMan.AddUndoAction(m_pUndoAction);
		m_pUndoAction = NULL;
	}

	return false; 
}

bool CTerrainMaskSmoothOperation::OnRButtonUp(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

bool CTerrainMaskSmoothOperation::OnLButtonDbClk(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

bool CTerrainMaskSmoothOperation::OnRButtonDbClk(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

void CTerrainMaskSmoothOperation::SetCurrentLayer( int nLay)
{
	if(nLay<=0) 
	{
		m_nCurrentLayer = -1;
		return;
	}
	m_nCurrentLayer = nLay;

	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap == NULL) return;
	CTerrain *pTerrain = pMap->GetTerrain();
	if(pTerrain == NULL) return;
	CMaskModifier *pMaskModifier = pTerrain->GetMaskModifier();
	if(pMaskModifier == NULL) return;

	if(m_pUndoAction)
	{
		delete m_pUndoAction;
		m_pUndoAction = NULL;
	}
		
	if(!pMaskModifier->ChangeEditLayer(m_nCurrentLayer))  AfxMessageBox("CTerrainMaskSmoothOperation::SetCurrentLayer(),Changed layer failed, please close editor and save map!");
}

void CTerrainMaskSmoothOperation::UpdateMaskMap()
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap == NULL || m_nCurrentLayer == -1) return;
	CTerrain *pTerrain = pMap->GetTerrain();
	if(pTerrain == NULL) return;
	
	
	if(m_nCurrentLayer>pTerrain->GetLayerNum()-1 || m_nCurrentLayer<0)
		return;

	//update mask data
	CTerrainLayer *pLayer = pTerrain->GetLayer(m_nCurrentLayer);
	CMaskModifier *pMaskModifier = pTerrain->GetMaskModifier();
	if(pLayer)
	{
		CELBitmap *pMaskMap = pLayer->GetMaskBitmap();
		if(pMaskMap)
		{
			m_nMaskHeight = pMaskMap->GetHeight();
			m_nMaskWidth  = pMaskMap->GetWidth();
		}else
		{
			m_nMaskHeight = DEFMASK_SIZE;
			m_nMaskWidth = DEFMASK_SIZE;
			pLayer->CreateMaskMap(DEFMASK_SIZE);
		}

		if(m_pUndoAction == NULL)
		{
			//mask undo
			m_rcUpdateRect.Clear();
			m_pUndoAction = new CUndoMaskAction(m_nMaskWidth);
		}

		CalculateMask();
	}
}

void CTerrainMaskSmoothOperation::StartPaint(bool bStart)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap == NULL) return;
	CTerrain *pTerrain = pMap->GetTerrain();
	if(pTerrain == NULL) return;
	CMaskModifier *pMaskModifier = pTerrain->GetMaskModifier();
	if(pMaskModifier == NULL) return;

	if(bStart)
		pMaskModifier->BeginModify();
	else pMaskModifier->EndModify();

	if(pMaskModifier->HasModify())
		pMap->SetModifiedFlag(true);
	
}

void CTerrainMaskSmoothOperation::CalculateMask()
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap == NULL) return;
	
	float fCenterX,fCenterZ;
	m_pBrush->GetCenter(fCenterX,fCenterZ);
	
	CTerrain::HEIGHTMAP* pHeightMap = pMap->GetTerrain()->GetHeightMap();
	CTerrain *pTerrain =  pMap->GetTerrain();
	if(!pHeightMap || !pTerrain) return;
	
	ARectF m_rcTerrain = pTerrain->GetTerrainArea();
	float m_fTileSize = pTerrain->GetTileSize();
	float m_fRadius = g_Configs.iBrushRadius;

	ARect<float> updateRect;
	updateRect.left = fCenterX - m_fRadius;
	updateRect.right = fCenterX + m_fRadius;
	updateRect.top = fCenterZ + m_fRadius;
	updateRect.bottom = fCenterZ - m_fRadius;

	float fMaskScaleX = (float)m_nMaskWidth/((m_rcTerrain.right - m_rcTerrain.left)/m_fTileSize);
	float fMaskScaleZ = (float)m_nMaskHeight/((m_rcTerrain.top - m_rcTerrain.bottom)/m_fTileSize);

	float fMaskGrid = (pHeightMap->iWidth - 1.0f) / m_nMaskWidth;
	float fMaskLen = fMaskGrid * m_fTileSize;
	//int c, r;
	
	ARectI maskRect;
	maskRect.left = (int)((updateRect.left - m_rcTerrain.left) / fMaskLen);
	maskRect.top = (int)(-(updateRect.top - m_rcTerrain.top) / fMaskLen);
	maskRect.right = (int)((updateRect.right - m_rcTerrain.left) / fMaskLen) + 1;
	maskRect.bottom = (int)(-(updateRect.bottom - m_rcTerrain.top) / fMaskLen) + 1;

	a_Clamp(maskRect.left, 0, m_nMaskWidth - 1);
	a_Clamp(maskRect.top, 0, m_nMaskWidth - 1);
	a_Clamp(maskRect.right, 0, m_nMaskWidth);
	a_Clamp(maskRect.bottom, 0, m_nMaskWidth);
	
	maskRect.Normalize();
	m_rcUpdateRect |= maskRect;

	CTerrainLayer *pCurrentLay = pTerrain->GetLayer(m_nCurrentLayer);
	if(pCurrentLay == NULL) return;
	CELBitmap *pMaskBitmap = pCurrentLay->GetMaskBitmap();
	if(pMaskBitmap==NULL) return;
	CELBitmap::LOCKINFO lockInfor;
	pMaskBitmap->LockRect(maskRect,&lockInfor);
	//Undo process
	if(m_pUndoAction) m_pUndoAction->AddData(maskRect,lockInfor);


	//取出原有数据
	int NumX = maskRect.Width();
	int NumZ = maskRect.Height();
	int nSize = NumX*NumZ*sizeof(int); 
	int n,i;
	float avgGray = 0;

	for( i = 0; i< lockInfor.iHeight; i++)
	{
		for( n = 0; n<lockInfor.iWidth; n++)
		{
			avgGray += lockInfor.pData[i*lockInfor.iPitch + n];
				
			//mask undo
			/*
			if(m_pUndoAction)
			{
				int h = maskRect.top + i;
				int w = maskRect.left + n;
			
				m_pUndoAction->AddGrid(w,h,lockInfor.pData[i*lockInfor.iPitch + n]);
			}*/
		}
	}
	avgGray = avgGray/(lockInfor.iWidth*lockInfor.iHeight);


	
	int *pData = new int[NumX*NumZ];
	if(pData == NULL) return;
	memset(pData,0,nSize);


	int nCenterX = NumX/2;
	int nCenterZ = NumZ/2;

	float r = nCenterX;
	float a = (float)NumX/100.0f * m_pBrush->GetBrushHard();
	float d;
	
	for(i=0; i < NumZ; i++)
	{
		for(n = 0; n < NumX; n++)
		{
			d = sqrt(float((i-nCenterZ)*(i-nCenterZ)+(n-nCenterX)*(n-nCenterX)));
				
			if(g_Configs.bCircleBrush)
			{
				if(r<=d) continue;
			}
	
			pData[i*NumX + n] = (int)((avgGray - lockInfor.pData[i*lockInfor.iPitch + n])*m_nSmoothFactory*0.01f);
		}
	}
	
	
	CMaskModifier *pMaskModifier = pTerrain->GetMaskModifier();
	if(pMaskModifier)
		pMaskModifier->Modify(maskRect,pData);

	if(pData) delete []pData;
}

void CTerrainMaskSmoothOperation::RecreateTerrainRender()
{
	
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap != NULL)
	{
		CTerrain *pTerrain = pMap->GetTerrain();
		if(pTerrain != NULL)
		{
			CMaskModifier *pMaskModifier = pTerrain->GetMaskModifier();
			if(pMaskModifier != NULL)
				if(pMaskModifier->HasModify())
					pMap->RecreateTerrainRender(false,false);
		}
	}
}
