#include "global.h"
#include "MainFrm.h"
#include "Terrain.h"
#include "TerrainRender.h"
#include "Render.h"
#include "TerrainMaskPaintOperation.h"
#include "Bitmap.h"
#include "MaskModifier.h"
#include "UndoMan.h"
#include "a3d.h"


//#define new A_DEBUG_NEW

bool CTerrainMaskPaintOperation::OnEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	return true;
}

void CTerrainMaskPaintOperation::Render(A3DViewport* pA3DViewport)
{
	if(m_pBrush)
		m_pBrush->Render(pA3DViewport);
	if(m_pBrush2)
	{
		int oldRad = g_Configs.iBrushRadius;
		g_Configs.iBrushRadius = g_Configs.iBrushRadius * (m_pBrush->GetBrushHard()/100.0f);
		m_pBrush2->Render(pA3DViewport);
		g_Configs.iBrushRadius = oldRad;
	}
}

void CTerrainMaskPaintOperation::Tick(DWORD dwTimeDelta)
{
	if(m_pBrush)
		m_pBrush->Tick(dwTimeDelta);
	if(m_pBrush2) m_pBrush2->Tick(dwTimeDelta);
//	UpdateTerrainHeight();
}


bool CTerrainMaskPaintOperation::OnMouseMove(int x, int y, DWORD dwFlags) 
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
			{
				m_pBrush->SetCenter(TraceRt.vHitPos.x,TraceRt.vHitPos.z);
				m_pBrush2->SetCenter(TraceRt.vHitPos.x,TraceRt.vHitPos.z);
			}
			if(m_bLMouseDown)
			{
				if(m_bDitherScalePress) g_Configs.iBrushRadius = m_nOldBrushRad + dwFlags/20;
				UpdateMaskMap(dwFlags);
				return false;
			}
		}
	}
	return true;
}

bool CTerrainMaskPaintOperation::OnWTPacket(int x, int y, DWORD button, DWORD param)
{

	return true;
}

bool CTerrainMaskPaintOperation::OnLButtonDown(int x, int y, DWORD dwFlags) 
{ 
	
	m_bLMouseDown = true;
	m_ptOldPos.x = x;
	m_ptOldPos.y = y;
	m_dwLastTime = a_GetTime();
	m_dwTimeUsed = m_dwFrequency + 1;
	//mask Undo
	StartPaint(true);

	//mask Undo
	if(m_pUndoAction) 
	{
		delete m_pUndoAction;
		m_pUndoAction = NULL;
	}

	if(m_bDitherScalePress) m_nOldBrushRad = g_Configs.iBrushRadius;
	UpdateMaskMap(dwFlags);
	return false; 
}

bool CTerrainMaskPaintOperation::OnRButtonDown(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

bool CTerrainMaskPaintOperation::OnLButtonUp(int x, int y, DWORD dwFlags) 
{ 
	if(m_bDitherScalePress) g_Configs.iBrushRadius = m_nOldBrushRad;
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

bool CTerrainMaskPaintOperation::OnRButtonUp(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

bool CTerrainMaskPaintOperation::OnLButtonDbClk(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

bool CTerrainMaskPaintOperation::OnRButtonDbClk(int x, int y, DWORD dwFlags) 
{ 
	return true; 
}

void CTerrainMaskPaintOperation::SetCurrentLayer( int nLay)
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

	if(!pMaskModifier->ChangeEditLayer(m_nCurrentLayer)) AfxMessageBox("CTerrainMaskPaintOperation::SetCurrentLayer(),Changed layer failed, please close editor and save map!");
	
}

void CTerrainMaskPaintOperation::UpdateMaskMap( int nPressure )
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap == NULL || m_nCurrentLayer== -1) return;
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
			m_rcUpdateRect.Clear();
			m_pUndoAction = new CUndoMaskAction(m_nMaskWidth);
		}

		CalculateMask(nPressure);
	}
}

void CTerrainMaskPaintOperation::StartPaint(bool bStart)
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

void CTerrainMaskPaintOperation::CalculateMask( int nPressure )
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap == NULL) return;

	DWORD currentTime = a_GetTime();
	DWORD timeDelta = currentTime - m_dwLastTime;
	m_dwLastTime = currentTime;
	
	int freq = m_dwFrequency;
	if(m_bFrequencyPress) 
	{
		freq = 500 - nPressure;
		if(freq < 0) freq = 0;
	}
	if(m_dwTimeUsed>freq)
	{
		m_dwTimeUsed = 0;
	}else
	{
		m_dwTimeUsed += timeDelta;
		return;
	}
	
	float fCenterX,fCenterZ;
	m_pBrush->GetCenter(fCenterX,fCenterZ);
	m_pBrush2->SetCenter(fCenterX,fCenterZ);
	
	CTerrain::HEIGHTMAP* pHeightMap = pMap->GetTerrain()->GetHeightMap();
	CTerrain *pTerrain =  pMap->GetTerrain();
	if(!pHeightMap || !pTerrain) return;
	CMaskModifier *pMaskModifier = pTerrain->GetMaskModifier();
	if(pMaskModifier==NULL) return;

	ARectF m_rcTerrain = pTerrain->GetTerrainArea();
	float m_fTileSize = pTerrain->GetTileSize();
	float m_fRadius = g_Configs.iBrushRadius;

	ARect<float> updateRect;
	updateRect.left = fCenterX - m_fRadius;
	updateRect.right = fCenterX + m_fRadius;
	updateRect.top = fCenterZ + m_fRadius;
	updateRect.bottom = fCenterZ - m_fRadius;

	float fMaskScaleX = (float)m_nMaskWidth/((m_rcTerrain.right - m_rcTerrain.left));
	float fMaskScaleZ = (float)m_nMaskHeight/((m_rcTerrain.top - m_rcTerrain.bottom));

	float fMaskGrid = (pHeightMap->iWidth - 1.0f) / m_nMaskWidth;
	float fMaskLen = fMaskGrid * m_fTileSize;
	
	ARectI maskRect;
	maskRect.left = (int)((updateRect.left - m_rcTerrain.left) / fMaskLen);
	maskRect.top = (int)(-(updateRect.top - m_rcTerrain.top) / fMaskLen);
	maskRect.right = (int)((updateRect.right - m_rcTerrain.left) / fMaskLen) + 1;
	maskRect.bottom = (int)(-(updateRect.bottom - m_rcTerrain.top) / fMaskLen) + 1;

	int nCenterX = (fCenterX - m_rcTerrain.left)*fMaskScaleX;
	int nCenterZ = -(m_rcTerrain.bottom + fCenterZ)*fMaskScaleZ;
	a_Clamp(nCenterX, 0, m_nMaskWidth);
	a_Clamp(nCenterZ, 0, m_nMaskWidth);

	a_Clamp(maskRect.left, 0, m_nMaskWidth-1);
	a_Clamp(maskRect.top, 0, m_nMaskWidth-1);
	a_Clamp(maskRect.right, 0, m_nMaskWidth);
	a_Clamp(maskRect.bottom, 0, m_nMaskWidth);
	maskRect.Normalize();
	nCenterX = nCenterX - maskRect.left;
	nCenterZ = nCenterZ - maskRect.top;
	m_rcUpdateRect |= maskRect;

	CTerrainLayer *pCurrentLay = pTerrain->GetLayer(m_nCurrentLayer);
	if(pCurrentLay == NULL) return;
	CELBitmap *pMaskBitmap = pCurrentLay->GetMaskBitmap();
	if(pMaskBitmap==NULL) return;
	CELBitmap::LOCKINFO lockInfor;
	pMaskBitmap->LockRect(maskRect,&lockInfor);
	//Undo process
	if(m_pUndoAction) m_pUndoAction->AddData(maskRect,lockInfor);




	int NumX = maskRect.Width();
	int NumZ = maskRect.Height();
	int i,n;
	//分配内存，等更新后释放
	int nSize = NumX*NumZ*sizeof(int); 
	if(!m_bUserBrush || m_pUserData==NULL)
	{
		int *pData = new int[NumX*NumZ];
		if(pData == NULL) return;
		memset(pData,0,nSize);
		double rnd = ((double(rand())/RAND_MAX)*m_nDitherGray);
		float r = (NumX/2 > NumZ/2)? NumX/2:NumZ/2;
		float a = (float)NumX/200.0f * m_pBrush->GetBrushHard();
		float d;
		for( i=0; i < NumZ; i++)
		{
			for( n = 0; n < NumX; n++)
			{
				d = sqrt(float((i-nCenterZ)*(i-nCenterZ)+(n-nCenterX)*(n-nCenterX)));
				double fGray = 1.0f;
				if(m_bDitherGrayPress) 
				{
					fGray = nPressure / 800.0f;
					if(fGray < 0.01f) fGray = 0.01f;	
				}else if(rnd!=0) fGray = 1.0/rnd;
				
				if(g_Configs.bCircleBrush)
				{
					if(r<d) continue;
					
					if(d < a)
					{
						if(m_bColorAdd)
							pData[i*NumX + n] = m_pBrush->GetBrushGray() * fGray;
						else pData[i*NumX + n] = -m_pBrush->GetBrushGray() * fGray;
					}
					else
					{
						if(m_bColorAdd)
							pData[i*NumX + n] = (m_pBrush->GetBrushGray() * (1.0f-(d - a)/(r - a)))*fGray;
						else pData[i*NumX + n] = (-m_pBrush->GetBrushGray() * (1.0f-(d - a)/(r - a)))*fGray;
					}
				}else
				{
					if(m_bColorAdd)
						pData[i*NumX + n] = m_pBrush->GetBrushGray()*fGray;
					else pData[i*NumX + n] = -m_pBrush->GetBrushGray()*fGray;
				}
			}
		}
		pMaskModifier->Modify(maskRect,pData);
		if(pData) delete []pData;
	}else
	{//Paint by user brush
		
		//rotate
		BYTE *pSrcData = new BYTE[m_nUserBrushW*m_nUserBrushH];
		memcpy(pSrcData,m_pUserData,m_nUserBrushW*m_nUserBrushH);
		double angle = (double(rand())/RAND_MAX)*m_nRotateAngleMax;
		if(m_bDitherAnglePress) angle = -nPressure/3;
		RoateData(pSrcData,m_nUserBrushW,m_nUserBrushH,angle);
		
		//turn
		if(m_bTurnX) TurnXData(pSrcData,m_nUserBrushW,m_nUserBrushH);
		if(m_bTurnY) TurnYData(pSrcData,m_nUserBrushW,m_nUserBrushH);
		
		//circle
		double circle = (double(rand())/RAND_MAX)*m_nCircleMax;
		if(m_bDitherCirclePress)
		{
			circle = nPressure/10;
			if(circle > 90) circle = 90;
		}
		
		CircleData(pSrcData,m_nUserBrushW,m_nUserBrushH,circle);
		
		//scale
		int sx = NumX;
		int sy = NumZ;
		if(!m_bDitherScalePress)
		{
			double rnd = ((double(rand())/RAND_MAX)*m_nScaleMax);
			double scale = 1 + rnd/30.0;
			sx = NumX * scale;
			sy = NumZ * scale;
			maskRect.right = maskRect.left + sx;
			maskRect.bottom = maskRect.top + sy;
		}
		
		int *pData = new int[sx*sy];
		if(pData == NULL) return;
		memset(pData,0,sx*sy*sizeof(int));
		TranslateData(pSrcData,m_nUserBrushW,m_nUserBrushH,pData,sx,sy,nPressure);

		//offset
		float xOffset,yOffset;
		xOffset = (double(rand())/RAND_MAX)*m_nOffetMax;
		yOffset = (double(rand())/RAND_MAX)*m_nOffetMax;
		maskRect.Offset(xOffset,yOffset);
		
		a_Clamp(maskRect.left, 0, m_nMaskWidth-1);
		a_Clamp(maskRect.top, 0, m_nMaskWidth-1);
		a_Clamp(maskRect.right, 0, m_nMaskWidth);
		a_Clamp(maskRect.bottom, 0, m_nMaskWidth);
		pMaskModifier->Modify(maskRect,pData);
		if(pSrcData) delete []pSrcData;
		if(pData) delete []pData;
	}
	

}

void CTerrainMaskPaintOperation::TranslateData(BYTE *pSrc,int sw,int sh,int*pDst,int dw,int dh, int press)
{
	
	int width = dw;
	int height = dh;

	double rnd = ((double(rand())/RAND_MAX)*m_nDitherGray);
	double fGray = 1.0f;
	if(m_bDitherGrayPress) 
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
				 col = col * (m_pBrush->GetBrushHard())/100.0f;
				 if(m_bColorAdd) pDst[y*width + x] = col; 
				else pDst[y*width + x] = -col; 
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
					 col = col * ((m_pBrush->GetBrushHard())/100.0f) * fGray;
				     if(m_bColorAdd)
						pDst[y*width + x] = col; 
					 else pDst[y*width + x] = -col; 
				 }
			 }
		}
	}
}

void CTerrainMaskPaintOperation::RoateData(BYTE *pSrc,int sw,int sh,double d)
{
	BYTE* pDst = new BYTE[sw*sh];
	memset(pDst,0,sw*sh);
	
	for( int h = 0; h < sh; ++h)
	{
		for( int w = 0; w < sw; ++w)
		{
			int nDes,nCenterX,nCenterY;
			nCenterX = sh/2;
			nCenterY = sw/2;
			
			int x = w - nCenterX;
			int y = h - nCenterY;

			double dx2 = x*x;
			double dy2 = y*y;
			
			double r = sqrt(dx2 + dy2);
			double dgree = atan2((double)y,(double)x);
			if(y >= 0) dgree = dgree*(180/A3D_PI) - d;
			else dgree = dgree*(180/A3D_PI) + 360 - d;
			
			double a = cos(dgree*(A3D_PI/180));
			double b = sin(dgree*(A3D_PI/180));
			x =  r*a + nCenterX;
 			y = r*b + nCenterY;
			
			if(x < 0) continue;
			if(y < 0) continue;

			if(x >= sw) continue;
			if(y >= sh) continue;
			nDes = y*sw + x;
			pDst[h*sw + w] = pSrc[nDes];
		}
	}
	
	memcpy(pSrc,pDst,sw*sh);
	delete []pDst;
}

void CTerrainMaskPaintOperation::TurnXData(BYTE *pSrc,int sw,int sh)
{
	for( int h = 0; h < sh; ++h)
	{
		for( int w = 0; w < sw/2; ++w)
		{
			int a = pSrc[h*sw + w];
			pSrc[h*sw + w] = pSrc[h*sw + sw - w - 1];
			pSrc[h*sw + sw - w - 1] = a;
		}
	}
}
void CTerrainMaskPaintOperation::TurnYData(BYTE *pSrc,int sw,int sh)
{
	for( int h = 0; h < sh/2; ++h)
	{
		for( int w = 0; w < sw; ++w)
		{
			int a = pSrc[h*sw + w];
			pSrc[h*sw + w] = pSrc[(sh-h-1)*sw + w];
			pSrc[(sh-h-1)*sw + w] = a;
		}
	}
}

void CTerrainMaskPaintOperation::CircleData(BYTE *pSrc,int sw,int sh, double d)
{
	BYTE* pTemp = new BYTE[sw*sh];
	memset(pTemp,0,sw*sh);
	for( int h = 0; h < sh; ++h)
	{
		for( int w = 0; w < sw; ++w)
		{
			int y  = (h - sh/2)/cos(d*(A3D_PI/180)) + sh/2;
			if(y <= 0) y = 1;
			if(y >= sh - 2) y = sh - 2;
			
			int a = pSrc[y*sw + w];
			int b = pSrc[y*sw + w + 1];
			int c = pSrc[(y+1)*sw + w];
			int d = pSrc[(y+1)*sw + w + 1];

			
			pTemp[h*sw + w] = (a + b + c + d)/4;
		}
	}
	memcpy(pSrc,pTemp,sw*sh);
	if(pTemp) delete []pTemp;
}

//--------------------------------------------------------------------
//Name: RecreateTerrainRender()
//Desc: 重新计算地形渲染数据
//--------------------------------------------------------------------
void CTerrainMaskPaintOperation::RecreateTerrainRender()
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
