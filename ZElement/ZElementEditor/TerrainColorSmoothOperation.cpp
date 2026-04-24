#include "global.h"
#include "MainFrm.h"
#include "Terrain.h"
#include "TerrainRender.h"
#include "Render.h"
#include "TerrainColorSmoothOperation.h"
#include "Bitmap.h"
#include "MaskModifier.h"
#include "UndoMan.h"
#include "A3D.h"

bool CTerrainColorSmoothOperation::OnEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	return true;
}

void CTerrainColorSmoothOperation::Render(A3DViewport* pA3DViewport)
{
	if(m_pBrush)
		m_pBrush->Render(pA3DViewport);

}

void CTerrainColorSmoothOperation::Tick(DWORD dwTimeDelta)
{
	if(m_pBrush)
		m_pBrush->Tick(dwTimeDelta);
//	UpdateTerrainHeight();
}


bool CTerrainColorSmoothOperation::OnMouseMove(int x, int y, DWORD dwFlags) 
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
				UpdateColorMap();
				return false;
			}
		}
	}
	return true;
}

bool CTerrainColorSmoothOperation::OnLButtonDown(int x, int y, DWORD dwFlags) 
{ 
	
	m_bLMouseDown = true;
	m_ptOldPos.x = x;
	m_ptOldPos.y = y;
	m_rcUpdateRect.Clear();
	if(m_pUndoColorMap) 
	{
		delete m_pUndoColorMap;
		m_pUndoColorMap = NULL;
	}
	StartPaint(true);
	UpdateColorMap();
	return false; 
}

bool CTerrainColorSmoothOperation::OnRButtonDown(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

bool CTerrainColorSmoothOperation::OnLButtonUp(int x, int y, DWORD dwFlags) 
{ 
	m_bLMouseDown = false;
	StartPaint(false);

	if(m_pUndoColorMap)
	{
		m_pUndoColorMap->SetUpdateRect(m_rcUpdateRect);
		g_UndoMan.AddUndoAction(m_pUndoColorMap);
		m_pUndoColorMap = NULL;
	}

	return false; 
}

bool CTerrainColorSmoothOperation::OnRButtonUp(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

bool CTerrainColorSmoothOperation::OnLButtonDbClk(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

bool CTerrainColorSmoothOperation::OnRButtonDbClk(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

void CTerrainColorSmoothOperation::UpdateColorMap()
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap == NULL) return;
	CTerrain *pTerrain = pMap->GetTerrain();
	if(pTerrain == NULL) return;
	
	//update mask data
	CMaskModifier *pMaskModifier = pTerrain->GetMaskModifier();

	CTerrain::VERTEXCOLOR* pVertexCol = pTerrain->GetColorMap();
	if(pVertexCol)
	{
		m_nVertexColHeight = pVertexCol->iHeight;
		m_nVertexColWidth  = pVertexCol->iWidth;

		if(m_pUndoColorMap == NULL)
		{
			//mask undo
			m_rcUpdateRect.Clear();
			m_pUndoColorMap = new CUndoColorMapAction();
		}

		CalculateColor();
	}
}

void CTerrainColorSmoothOperation::StartPaint(bool bStart)
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

extern A3DCOLOR LerpColor(const A3DCOLOR l, const A3DCOLOR r, float n);

void CTerrainColorSmoothOperation::CalculateColor()
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap == NULL) return;

	float fCenterX,fCenterZ;
	m_pBrush->GetCenter(fCenterX,fCenterZ);

	CTerrain *pTerrain =  pMap->GetTerrain();
	if(pTerrain == NULL) return;
	CTerrain::HEIGHTMAP* pHeightMap = pTerrain->GetHeightMap();

	if(pHeightMap == NULL) return;

	CTerrain::VERTEXCOLOR* pVertexCol = pMap->GetTerrain()->GetColorMap();
	if(pVertexCol==NULL) return;

	ARectF m_rcTerrain = pTerrain->GetTerrainArea();
	float m_fTileSize = pTerrain->GetTileSize();
	float m_fRadius = g_Configs.iBrushRadius;

	ARect<float> updateRect;
	updateRect.left = fCenterX - m_fRadius;
	updateRect.right = fCenterX + m_fRadius;
	updateRect.top = fCenterZ + m_fRadius;
	updateRect.bottom = fCenterZ - m_fRadius;

	float fMaskScaleX = (float)m_nVertexColWidth/((m_rcTerrain.right - m_rcTerrain.left)/m_fTileSize);
	float fMaskScaleZ = (float)m_nVertexColHeight/((m_rcTerrain.top - m_rcTerrain.bottom)/m_fTileSize);

	float fMaskGrid = (pHeightMap->iWidth - 1.0f) / m_nVertexColWidth;
	float fMaskLen = fMaskGrid * m_fTileSize;
	//int c, r;

	ARectI maskRect;
	maskRect.left = (int)((updateRect.left - m_rcTerrain.left) / fMaskLen);
	maskRect.top = (int)(-(updateRect.top - m_rcTerrain.top) / fMaskLen);
	maskRect.right = (int)((updateRect.right - m_rcTerrain.left) / fMaskLen) + 1;
	maskRect.bottom = (int)(-(updateRect.bottom - m_rcTerrain.top) / fMaskLen) + 1;

	m_rcUpdateRect |= maskRect;
	//Undo process
	if(m_pUndoColorMap) m_pUndoColorMap->AddData(maskRect);

	//取出原有数据
	int NumX = maskRect.Width();
	int NumZ = maskRect.Height();

	int nCenterX = NumX/2;
	int nCenterZ = NumZ/2;

	float fRadius = (float)nCenterX;
	float d;

	int aAvg = 0;
	int rAvg = 0;
	int gAvg = 0;
	int bAvg = 0;

	int i;
	int n;
	for( i = 0; i< NumZ; i++)
	{
		for( n = 0; n<NumX; n++)
		{
			int idx = (maskRect.top + i)*m_nVertexColWidth + maskRect.left + n;
			unsigned int oldClr = pVertexCol->pColor[idx];
			aAvg += A3DCOLOR_GETALPHA(oldClr);
			rAvg += A3DCOLOR_GETRED(oldClr);
			gAvg += A3DCOLOR_GETGREEN(oldClr);
			bAvg += A3DCOLOR_GETBLUE(oldClr);
		}
	}

	aAvg /= NumX*NumZ;
	rAvg /= NumX*NumZ;
	gAvg /= NumX*NumZ;
	bAvg /= NumX*NumZ;

	A3DCOLOR colorAvg = aAvg<<24 | rAvg<<16 | gAvg<<8 | bAvg;


	for( i = 0; i< NumZ; i++)
	{
		for( n = 0; n<NumX; n++)
		{
			d = sqrt(float((i-nCenterZ)*(i-nCenterZ)+(n-nCenterX)*(n-nCenterX)));

			if(g_Configs.bCircleBrush)
			{
				if(fRadius<=d) continue;
			}

			int idx = (maskRect.top + i)*m_nVertexColWidth + maskRect.left + n;
			unsigned int oldClr = pVertexCol->pColor[idx];

			int da = A3DCOLOR_GETALPHA(oldClr);
			int dr = A3DCOLOR_GETRED(oldClr);
			int dg = A3DCOLOR_GETGREEN(oldClr);
			int db = A3DCOLOR_GETBLUE(oldClr);

			A3DCOLOR curColor = LerpColor(colorAvg, oldClr, m_nSmoothFactory*0.01f);
			pVertexCol->pColor[idx] = curColor;
		}
	}

	a_Clamp(maskRect.left,0,m_nVertexColWidth-1);
	a_Clamp(maskRect.right,0,m_nVertexColWidth-1);
	a_Clamp(maskRect.top,0,m_nVertexColWidth-1);
	a_Clamp(maskRect.bottom,0,m_nVertexColWidth-1);
	pTerrain->UpdateVertexColorMapToRender(maskRect);
}

void CTerrainColorSmoothOperation::RecreateTerrainRender()
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
