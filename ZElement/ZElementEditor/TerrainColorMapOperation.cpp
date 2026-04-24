#include "Stdafx.h"
#include "TerrainColorMapOperation.h"
#include "TerrainStretchBrush.h"
#include "MainFrm.h"
#include "Render.h"
#include "TerrainRender.h"
#include "MaskModifier.h"
#include "Brush.h"
#include "ElementEditor.h"
#include "UndoMan.h"

#include <A3DTerrain2Blk.h>

extern CElementEditorApp theApp;

CTerrainColorMapOperation::CTerrainColorMapOperation(void)
{	
	m_iPower = 10;
	m_fOpacity = 1.0f;
	m_dwColor = COLORMAP_UNITWHITE;
	m_btAlpha = A3DCOLOR_GETALPHA(m_dwColor);
	m_PaintType = PAINT_COLOR;
	m_bLMouseDown = false;
	m_pBrush = new Brush;
	m_dwFrequency = 0;
	m_dwLastTime = 0;
	m_dwTimeUsed = 0;
	m_iPressure = 0;
	m_bPick = false;
	m_hPickCursor = NULL;
	m_pUndoColorMap = 0;
}

CTerrainColorMapOperation::~CTerrainColorMapOperation(void)
{
	delete m_pBrush;
	m_pBrush = 0;
}

void CTerrainColorMapOperation::Render(A3DViewport* pA3DViewport)
{
	if(m_pBrush)
		m_pBrush->Render(pA3DViewport);

}

void CTerrainColorMapOperation::Tick(DWORD dwTimeDelta)
{
	if(m_pBrush)
		m_pBrush->Tick(dwTimeDelta);
	if(m_bLMouseDown && !m_bPick)
	{
		UpdateMaskMap(m_iPressure);
	}
}


bool CTerrainColorMapOperation::OnMouseMove(int x, int y, DWORD dwFlags) 
{ 
	if (m_bPick)
		return true;
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
				UpdateMaskMap(dwFlags);
				return false;
			}
		}
	}
	return true;
}

void CTerrainColorMapOperation::PickColor(int x, int y)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(!pMap)
		return;
	CTerrainRender *pTR = pMap->GetTerrain()->GetRender();
	if(!pTR)
		return;
	A3DVECTOR3 vPos(x, y, 1.0f);
	A3DCamera* pCamera = (A3DCamera*)(g_Render.GetA3DEngine())->GetActiveCamera();
	if(!pCamera)
		return;
	A3DViewport* pViewport =g_Render.GetA3DEngine()->GetActiveViewport();
	if(!pViewport)
		return;
	A3DVECTOR3 vStart((float)x, (float)y, 0.0f);
	pViewport->InvTransform(vStart,vStart);
	pViewport->InvTransform(vPos,vPos);
	
	RAYTRACERT TraceRt;	//	Used to store trace result
	if(!pTR->RayTrace(vStart,vPos - vStart,1.0f,&TraceRt))
		return;
	float fx = TraceRt.vHitPos.x;
	float fz = TraceRt.vHitPos.z;
	CTerrain *pTerrain = pMap->GetTerrain();
	if(!pTerrain)
		return;
	CTerrain::VERTEXCOLOR* pColorMap = pTerrain->GetColorMap();
	ARectF rcArea = pTerrain->GetTerrainArea();
	int idxCol = ((fx-rcArea.left)/(rcArea.right-rcArea.left)*pColorMap->iWidth + 0.5f);
	int idxRow = ((fz-rcArea.top)/(rcArea.bottom-rcArea.top)*pColorMap->iHeight + 0.5f);
	m_dwColor = pColorMap->pColor[idxRow*pColorMap->iWidth+idxCol];
	AUX_GetMainFrame()->GetToolTabWnd()->SetColorMapPanelCurColor(m_dwColor);
}

bool CTerrainColorMapOperation::OnLButtonDown(int x, int y, DWORD dwFlags) 
{
	m_bLMouseDown = true;
	m_ptOldPos.x = x;
	m_ptOldPos.y = y;
	if(m_bPick)
	{
		PickColor(x, y);
		return true;
	}
	m_dwLastTime = a_GetTime();
	m_dwTimeUsed = m_dwFrequency + 1;

	if(m_pUndoColorMap) 
	{
		delete m_pUndoColorMap;
		m_pUndoColorMap = NULL;
	}
	m_rcUpdateRect.Clear();

	UpdateMaskMap(dwFlags);
	return true;
}

bool CTerrainColorMapOperation::OnRButtonDown(int x, int y, DWORD dwFlags) 
{
	return true;
}

bool CTerrainColorMapOperation::OnLButtonUp(int x, int y, DWORD dwFlags) 
{ 
	m_bLMouseDown = false;

	if(m_pUndoColorMap)
	{
		m_pUndoColorMap->SetUpdateRect(m_rcUpdateRect);
		g_UndoMan.AddUndoAction(m_pUndoColorMap);
		m_pUndoColorMap = NULL;
	}

	return false; 
}

bool CTerrainColorMapOperation::OnRButtonUp(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

bool CTerrainColorMapOperation::OnLButtonDbClk(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

bool CTerrainColorMapOperation::OnRButtonDbClk(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

void CTerrainColorMapOperation::OnKeyDown(DWORD dwChar, DWORD dwRepCnt, DWORD dwFlags)
{
	if(dwChar == VK_CONTROL)
	{
		if(!m_hPickCursor)
			m_hPickCursor = ::LoadCursor(theApp.m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_TUBULARIS));
		m_bPick = true;
		AUX_GetMainFrame()->GetViewFrame()->GetRenderWnd()->SetCapture();
		SetCursor(m_hPickCursor);
	}
}

void CTerrainColorMapOperation::OnKeyUp(DWORD dwChar, DWORD dwRepCnt, DWORD dwFlags)
{
	if(dwChar == VK_CONTROL)
	{
		m_bPick = false;
		ReleaseCapture();
		SetCursor(NULL);
	}
}

void CTerrainColorMapOperation::UpdateMaskMap(unsigned int iPressure)
{
	if(iPressure > 1023)
		iPressure = m_iPressure;
	m_iPressure = iPressure;
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap == NULL ) return;
	CTerrain *pTerrain = pMap->GetTerrain();
	if(pTerrain == NULL) return;


	CTerrain::VERTEXCOLOR* pVertexCol = pTerrain->GetColorMap();
	if(pVertexCol)
	{
		m_nVertexColHeight = pVertexCol->iHeight;
		m_nVertexColWidth  = pVertexCol->iWidth;

		if(m_pUndoColorMap == NULL)
		{
			m_rcUpdateRect.Clear();
			m_pUndoColorMap = new CUndoColorMapAction();
		}

		CalculateMask(iPressure);
	}
}

A3DCOLOR LerpColor(const A3DCOLOR l, const A3DCOLOR r, float n)
{
	return A3DCOLORVALUE(A3DCOLORVALUE(l) * n + A3DCOLORVALUE(r) * (1.0f - n)).ToRGBAColor();
}

void CTerrainColorMapOperation::CalculateMask(unsigned int iPressure)
{	
	DWORD currentTime = a_GetTime();
	DWORD timeDelta = currentTime - m_dwLastTime;
	m_dwLastTime = currentTime;

	int freq = m_dwFrequency;
	if(m_pBrush->IsPressureDispelNum()) 
	{
		freq = 500 - iPressure;
		if(freq < 0) freq = 0;
	}
	else
	{
		freq = 500 - m_pBrush->GetDispelNum();
	}
	if(m_dwTimeUsed>freq)
	{
		m_dwTimeUsed = 0;
	}
	else
	{
		m_dwTimeUsed += timeDelta;
		return;
	}

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

	int a = m_btAlpha;
	int r = A3DCOLOR_GETRED(m_dwColor);
	int g = A3DCOLOR_GETGREEN(m_dwColor);
	int b = A3DCOLOR_GETBLUE(m_dwColor);

	if (m_pBrush->IsDefaultBrush() || !m_pBrush->GetBrushData())
	{
		for(int i=0; i < NumZ; i++)
		{
			for(int n = 0; n < NumX; n++)
			{
				if(g_Configs.bCircleBrush)
					d = sqrt(float((i-nCenterZ)*(i-nCenterZ)+(n-nCenterX)*(n-nCenterX)));
				else
					d = max(abs(i-nCenterZ), abs(n-nCenterX));

				if(g_Configs.bCircleBrush)
				{
					if(fRadius<=d) continue;
				}
				if(maskRect.top + i < 0 || maskRect.top + i >= m_nVertexColWidth) continue;
				if(maskRect.left + n < 0 || maskRect.left + n >= m_nVertexColWidth) continue;

				if(m_PaintType == 1)
				{// Add color
					int idx = (maskRect.top + i)*m_nVertexColWidth + maskRect.left + n;
					unsigned int oldClr = pVertexCol->pColor[idx];

					int da = A3DCOLOR_GETALPHA(oldClr);
					int dr = A3DCOLOR_GETRED(oldClr);
					int dg = A3DCOLOR_GETGREEN(oldClr);
					int db = A3DCOLOR_GETBLUE(oldClr);

					d = (1.0f - d/fRadius);
					da = a*d*(m_iPower/100.0f) + da;
					dr = r*d*(m_iPower/100.0f) + dr;
					dg = g*d*(m_iPower/100.0f) + dg;
					db = b*d*(m_iPower/100.0f) + db;

					a_Clamp(da,0,(int)m_btAlpha);
					a_Clamp(dr,0,255);
					a_Clamp(dg,0,255);
					a_Clamp(db,0,255);



					unsigned int newClr = da << 24 | dr << 16 | dg << 8 | db;
					pVertexCol->pColor[idx] = newClr;
				}else if(m_PaintType == 0)
				{// Set color
					int idx = (maskRect.top + i)*m_nVertexColWidth + maskRect.left + n;
					unsigned int oldClr = pVertexCol->pColor[idx];

					// 				float xp = (float)n/NumX;
					// 				float yp = (float)i/NumZ;
					// 				xp = (xp-0.5f)*2.0f;
					// 				yp = (yp-0.5f)*2.0f;
					// 				xp = xp<0?(xp*hard-1.0f):(xp*hard+1.0f);
					// 				yp = yp<0?(yp*hard-1.0f):(yp*hard+1.0f);
					// 				float p = 1/sqrt(xp*xp+yp*yp);
					// we should clamp the factor so that square brushes will not apply a negative factor
					float fFinalFactor = (1-d/fRadius);
					a_Clamp(fFinalFactor, 0.0f, 1.0f);
					A3DCOLOR curColor = LerpColor(m_dwColor, oldClr, fFinalFactor*m_pBrush->GetHard()*0.01f);
					pVertexCol->pColor[idx] = curColor;

				}
			}
		}
	}
	else
	{		
		m_pBrush->TransformBrush(iPressure);	

		//scale
		int sx = NumX;
		int sy = NumZ;
		double scale = 1.0f;
		if(!m_pBrush->IsPressureDitherArea())
		{
			double rnd = ((double(rand())/RAND_MAX)*m_pBrush->GetDitherArea());
			scale = 1 + rnd/30.0;
		}
		else
		{
			double rnd = ((double(rand())/RAND_MAX)*iPressure/10.23);
			scale = 1 + rnd/30.0;
		}

		a_Clamp(scale, 1.0, 5.0);
		
		sx = NumX * scale;
		sy = NumZ * scale;
		maskRect.right = maskRect.left + sx;
		maskRect.bottom = maskRect.top + sy;

		int *pData = new int[sx*sy];
		if(pData == NULL) return;
		memset(pData,0,sx*sy*sizeof(int));
		TranslateData(m_pBrush->GetBrushResultData(),m_pBrush->GetWidth(),m_pBrush->GetHeight(),pData,sx,sy,iPressure);

		//offset
		float xOffset,yOffset;
		xOffset = (double(rand())/RAND_MAX)*m_pBrush->GetDispel();
		yOffset = (double(rand())/RAND_MAX)*m_pBrush->GetDispel();
		maskRect.Offset(xOffset,yOffset);

		a_Clamp(maskRect.left,0,m_nVertexColWidth-1);
		a_Clamp(maskRect.right,0,m_nVertexColWidth-1);
		a_Clamp(maskRect.top,0,m_nVertexColWidth-1);
		a_Clamp(maskRect.bottom,0,m_nVertexColWidth-1);
		for(int i=0; i < sy; i++)
		{
			for(int n = 0; n < sx; n++)
			{				
				int idxData = i*sx+n;
				float p = pData[idxData]/255.0f;
				int idx = (maskRect.top + i)*m_nVertexColWidth + maskRect.left + n;
				unsigned int oldClr = pVertexCol->pColor[idx];
				if(m_PaintType == 0)
				{					
					A3DCOLOR curColor = LerpColor(m_dwColor, oldClr, p);	
					pVertexCol->pColor[idx] = curColor;
				}
				else if(m_PaintType == 1)
				{
					int a = m_btAlpha;
					int r = A3DCOLOR_GETRED(m_dwColor);
					int g = A3DCOLOR_GETGREEN(m_dwColor);
					int b = A3DCOLOR_GETBLUE(m_dwColor);

					int da = A3DCOLOR_GETALPHA(oldClr);
					int dr = A3DCOLOR_GETRED(oldClr);
					int dg = A3DCOLOR_GETGREEN(oldClr);
					int db = A3DCOLOR_GETBLUE(oldClr);
		
					da = a*(m_iPower/100.0f)*p + da;
					dr = r*(m_iPower/100.0f)*p + dr;
					dg = g*(m_iPower/100.0f)*p + dg;
					db = b*(m_iPower/100.0f)*p + db;

					a_Clamp(da,0,(int)m_btAlpha);
					a_Clamp(dr,0,255);
					a_Clamp(dg,0,255);
					a_Clamp(db,0,255);

					unsigned int newClr = da << 24 | dr << 16 | dg << 8 | db;
					pVertexCol->pColor[idx] = newClr;
				}
			}
		}
		if(pData) delete []pData;
	}
	if(m_PaintType == 2)
	{// Blur color
		for(int i=0; i < NumZ; i++)
		{
			for(int n = 0; n < NumX; n++)
			{
				int idx[9];
				int count = 0;
				int pa,pr,pg,pb;
				pa = pr = pg = pb = 0;
				idx[0] = (maskRect.top + i)*m_nVertexColWidth + maskRect.left + n;
				idx[1] = (maskRect.top + i)*m_nVertexColWidth + maskRect.left + n + 1;
				idx[2] = (maskRect.top + i + 1)*m_nVertexColWidth + maskRect.left + n;
				idx[3] = (maskRect.top + i + 1)*m_nVertexColWidth + maskRect.left + n + 1;
				idx[4] = (maskRect.top + i)*m_nVertexColWidth + maskRect.left + n -1;
				idx[5] = (maskRect.top + i - 1)*m_nVertexColWidth + maskRect.left + n;
				idx[6] = (maskRect.top + i - 1)*m_nVertexColWidth + maskRect.left + n - 1;
				idx[7] = (maskRect.top + i + 1)*m_nVertexColWidth + maskRect.left + n - 1;
				idx[8] = (maskRect.top + i - 1)*m_nVertexColWidth + maskRect.left + n + 1;
				for( int i= 0; i < 9; ++i)
				{
					if(idx[i] >= 0 && idx[i] < m_nVertexColWidth*m_nVertexColWidth)
					{
						unsigned int clr = pVertexCol->pColor[idx[i]];
						pa += A3DCOLOR_GETALPHA(clr);
						pr += A3DCOLOR_GETRED(clr);
						pg += A3DCOLOR_GETGREEN(clr);
						pb += A3DCOLOR_GETBLUE(clr);
						if(i == 0)
						{
							a = pa;
							r = pr;
							g = pg;
							b = pb;
						}
						count++;
					}
				}
				if(count > 0)
				{
					pa = pa/count;
					pr = pr/count;
					pg = pg/count;
					pb = pb/count;

					if(idx[0] < m_nVertexColWidth*m_nVertexColWidth)
					{
						pa = (pa - A3DCOLOR_GETALPHA(pVertexCol->pColor[idx[0]]))*(m_iPower/100.0f) + A3DCOLOR_GETALPHA(pVertexCol->pColor[idx[0]]);
						pr = (pr - A3DCOLOR_GETRED(pVertexCol->pColor[idx[0]]))*(m_iPower/100.0f) + A3DCOLOR_GETRED(pVertexCol->pColor[idx[0]]);
						pg = (pg - A3DCOLOR_GETGREEN(pVertexCol->pColor[idx[0]]))*(m_iPower/100.0f) + A3DCOLOR_GETGREEN(pVertexCol->pColor[idx[0]]);
						pb = (pb - A3DCOLOR_GETBLUE(pVertexCol->pColor[idx[0]]))*(m_iPower/100.0f) + A3DCOLOR_GETBLUE(pVertexCol->pColor[idx[0]]);


						unsigned int newClr = pa << 24 | pr << 16 | pg << 8 | pb;
						pVertexCol->pColor[idx[0]] = newClr;
					}
				}
			}
		}
	}

	
	
	a_Clamp(maskRect.left,0,m_nVertexColWidth-1);
	a_Clamp(maskRect.right,0,m_nVertexColWidth-1);
	a_Clamp(maskRect.top,0,m_nVertexColWidth-1);
	a_Clamp(maskRect.bottom,0,m_nVertexColWidth-1);
	pTerrain->UpdateVertexColorMapToRender(maskRect);

}

void CTerrainColorMapOperation::TranslateData(BYTE *pSrc,int sw,int sh,int*pDst,int dw,int dh, int press)
{

	int width = dw;
	int height = dh;

	double rnd = ((double(rand())/RAND_MAX)*m_pBrush->GetDitherColor());
	double fGray = 1.0f;
	if(m_pBrush->IsPressureDitherColor()) 
	{
		fGray = press / 400.0f;
		if(fGray < 0.01f) fGray = 0.01f;
		if(fGray > 1.0f) fGray = 1.0f;
	}else if(rnd!=0) fGray = 1.0/rnd;

	double widthScale = (double)sw/(double)width;
	double heightScale = (double)sh/(double)height;
	for( int x = 0; x < width; ++x)
	{
		for( int y = 0; y < height; ++y)
		{
			double xx = x * widthScale; 
			double yy = y * heightScale;

			if(widthScale <= 0.2 || widthScale >5)  
			{
				int col = pSrc[int(yy)*sw + int(xx)];
				col = col * (m_pBrush->GetHard())/100.0f;
				pDst[y*width + x] = col; 
			}else
			{
				int a = int(xx);  
				int b = int(yy);
				if (a + 1 >= sw || b + 1 >= sh)
					pDst[y*width + x] = pSrc[int(yy)*sw + int(xx)];
				else
				{

					double x11 = double(pSrc[b*sw + a]); 
					double x12 = double(pSrc[(b+1)*sw + a]); 
					double x21 = double(pSrc[b*sw + a + 1]);  
					double x22 = double(pSrc[(b+1)*sw + a + 1]);       
					int col = int( (b+1-yy) * ((xx-a)*x21 + (a+1-xx)*x11) + (yy-b) * ((xx-a)*x22 +(a+1-xx) * x12) );
					col = col * ((m_pBrush->GetHard())/100.0f) * fGray;
					pDst[y*width + x] = col; 
				}
			}
		}
	}
}

void CTerrainColorMapOperation::RecreateTerrainRender()
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
