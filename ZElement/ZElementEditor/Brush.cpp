#include "Stdafx.h"
#include "Brush.h"
#include "ElementMap.h"
#include "Render.h"
#include "MainFrm.h"
#include <WinDef.h>

#include <A3DViewport.h>
#include <A3DMacros.h>
#include <A3DWireCollector.h>

#define MAX_NUM_SECT 36

const float pi_2 = A3D_2PI;
const float pi_delta = pi_2/MAX_NUM_SECT;
const float offset_y = 0.1f;

#define _getRed(clr)  clr & 0xff

#define _getGreen(clr) (clr>>8) & 0xff 

#define _getBlue(clr) (clr>>16) & 0xff 

Brush::Brush(void)
{
	m_dwColor = A3DCOLORRGB(255,255,0);
	m_iHard = 50;
	m_iDispelNum = 500;
	m_iDitherColor = 0;
	m_iDitherArea = 0;
	m_iDitherAngle = 0;
	m_iDitherCircle = 0;
	m_iDispel = 0;
	m_pData = 0;
	m_pResultData = 0;
	m_pTempData = 0;
	m_bPressureDispelNum = false;
	m_bPressureDitherColor = false;
	m_bPressureDitherArea = false;
	m_bPressureDitherAngle = false;
	m_bPressureDitherCircle = false;
	m_bTurnX = false;
	m_bTurnY = false;
	m_strBrushName = "д╛хок╒вс";
}

Brush::~Brush(void)
{
	releaseBrushData();
}

void Brush::releaseBrushData()
{
	delete []m_pData;
	m_pData = 0;
	delete []m_pResultData;
	m_pResultData = 0;
	delete []m_pTempData;
	m_pTempData = 0;
}

void Brush::Render(A3DViewport* pA3DViewport)
{
	if(g_Configs.bCircleBrush)
	{
		DrawCircle(g_Configs.iBrushRadius);
		if(m_iHard != 100 && m_iHard != 0)
			DrawCircle(g_Configs.iBrushRadius*m_iHard*0.01f);
	}
	else
	{
		DrawRect();
	}
}

void Brush::Tick(DWORD dwTimeDelta)
{
}

void Brush::SetCenter(float x, float z)
{ 
	m_fCenterX = x; m_fCenterY = z; 
};

void Brush::DrawRect()
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	if(!pMap->GetTerrain()) return;

	float fRadius = static_cast<float>(g_Configs.iBrushRadius);

	A3DVECTOR3 vertices[MAX_NUM_SECT];
	A3DVECTOR3 vLeftBottom = A3DVECTOR3(m_fCenterX - fRadius ,0, m_fCenterY - fRadius);
	A3DVECTOR3 vRightBottom = A3DVECTOR3(m_fCenterX -fRadius ,0, m_fCenterY +  fRadius);
	A3DVECTOR3 vRightUp = A3DVECTOR3(m_fCenterX + fRadius ,0,m_fCenterY + fRadius);
	A3DVECTOR3 vLeftUp = A3DVECTOR3(m_fCenterX + fRadius,0,m_fCenterY - fRadius);
	int n = 0;
	int Num = MAX_NUM_SECT/4;


	for(;n<Num;n++)
	{
		vertices[n] = vLeftBottom + (vRightBottom - vLeftBottom)*0.111111f*n;
		vertices[n].y = pMap->GetTerrain()->GetPosHeight(vertices[n]) + offset_y;//0.1f
	}

	for(;n<Num*2;n++)
	{
		vertices[n] = vRightBottom + (vRightUp - vRightBottom)*0.111111f*(n-Num);
		vertices[n].y = pMap->GetTerrain()->GetPosHeight(vertices[n]) + offset_y;//0.1f
	}

	for(;n<Num*3;n++)
	{
		vertices[n] = vRightUp + (vLeftUp - vRightUp)*0.111111f*(n-Num*2);
		vertices[n].y = pMap->GetTerrain()->GetPosHeight(vertices[n]) + offset_y;//0.1f
	}

	for(;n<Num*4;n++)
	{
		vertices[n] = vLeftUp + (vLeftBottom - vLeftUp)*0.111111f*(n-Num*3);
		vertices[n].y = pMap->GetTerrain()->GetPosHeight(vertices[n]) + offset_y;//0.1f
	}

	DrawLine(vertices,MAX_NUM_SECT);
}

void Brush::DrawCircle(float fRadius)
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	if(!pMap->GetTerrain()) return;

	A3DVECTOR3 vertices[MAX_NUM_SECT];

	//Calculate vertices array
	if(pMap && pMap->GetTerrain()->GetRender())
	{

		float angle = 0;
		//0 - pi2
		for(int i=0; i<MAX_NUM_SECT; i++)
		{
			vertices[i].x = m_fCenterX + fRadius*cos(angle);
			vertices[i].z = m_fCenterY + fRadius*sin(angle);
			vertices[i].y = pMap->GetTerrain()->GetPosHeight(vertices[i]) + offset_y;

			angle += pi_delta;
		}
	}

	//Render wire
	DrawLine(vertices,MAX_NUM_SECT);
}

void Brush::DrawLine(A3DVECTOR3 *pVertices,DWORD dwNum)
{
	A3DWireCollector *pWireCollector = g_Render.GetA3DEngine()->GetA3DWireCollector();

	WORD pIndices[MAX_NUM_SECT*2];
	int n = 0;
	for(int i=0; i<MAX_NUM_SECT; i++)
	{
		pIndices[n] = i;
		pIndices[n+1] = (i+1)%(MAX_NUM_SECT);
		n += 2;
	}

	if(pWireCollector)
	{
		pWireCollector->AddRenderData_3D(pVertices,dwNum,pIndices,MAX_NUM_SECT*2,m_dwColor);
	}
}

void Brush::SetBrushData(COLORREF* pColorData, int iWidth, int iHeight)
{
	releaseBrushData();

	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_pData = new BYTE[iWidth * iHeight];
	for(int i=0; i<iHeight; i++)
	{
		for(int j=0; j<iWidth; j++)
		{
			COLORREF color = pColorData[i * iWidth + j];
			int ir = _getRed(color);
			int ig = _getGreen(color);
			int ib = _getBlue(color);
			m_pData[i * iWidth + j] = (int)(0.33f*ir + 0.53f*ig + 0.13f*ib);
		}
	}
	m_pResultData = new BYTE[iWidth * iHeight];
	m_pTempData = new BYTE[iWidth * iHeight];
}

void Brush::TransformBrush(int iPressure)
{	
	rotateData(iPressure);
	turnXData();
	turnYData();
	circleData(iPressure);
}

void Brush::rotateData(int iPressure)
{
	memcpy(m_pResultData, m_pData, m_iWidth * m_iHeight);
	float fAngle = 0.0f;
	if(m_bPressureDitherAngle)
		fAngle = -iPressure/3;
	else
		fAngle = ((float)rand())/RAND_MAX*m_iDitherAngle;
	if(fAngle == 0.0f)
		return;

	memset(m_pTempData, 0, m_iWidth * m_iHeight);

	for( int h = 0; h < m_iHeight; ++h)
	{
		for( int w = 0; w < m_iWidth; ++w)
		{
			int nDes,nCenterX,nCenterY;
			nCenterX = m_iHeight/2;
			nCenterY = m_iWidth/2;

			int x = w - nCenterX;
			int y = h - nCenterY;

			double dx2 = x*x;
			double dy2 = y*y;

			double r = sqrt(dx2 + dy2);
			double dgree = atan2((double)y,(double)x);
			if(y >= 0) dgree = dgree*(180/A3D_PI) - fAngle;
			else dgree = dgree*(180/A3D_PI) + 360 - fAngle;

			double a = cos(dgree*(A3D_PI/180));
			double b = sin(dgree*(A3D_PI/180));
			x =  r*a + nCenterX;
			y = r*b + nCenterY;

			if(x < 0) continue;
			if(y < 0) continue;

			if(x >= m_iWidth) continue;
			if(y >= m_iHeight) continue;
			nDes = y*m_iWidth + x;
			m_pTempData[h*m_iWidth + w] = m_pResultData[nDes];
		}
	}

	memcpy(m_pResultData, m_pTempData, m_iWidth*m_iHeight);
}

void Brush::turnXData()
{
	if(!m_bTurnX)
		return;
	for( int h = 0; h < m_iHeight; ++h)
	{
		for( int w = 0; w < m_iWidth/2; ++w)
		{
			int a = m_pResultData[h*m_iWidth + w];
			m_pResultData[h*m_iWidth + w] = m_pResultData[h*m_iWidth + m_iWidth - w - 1];
			m_pResultData[h*m_iWidth + m_iWidth - w - 1] = a;
		}
	}
}
void Brush::turnYData()
{
	if(!m_bTurnY)
		return;
	for( int h = 0; h < m_iHeight/2; ++h)
	{
		for( int w = 0; w < m_iWidth; ++w)
		{
			int a = m_pResultData[h*m_iWidth + w];
			m_pResultData[h*m_iWidth + w] = m_pResultData[(m_iHeight-h-1)*m_iWidth + w];
			m_pResultData[(m_iHeight-h-1)*m_iWidth + w] = a;
		}
	}
}

void Brush::circleData(int iPressure)
{
	float fCircle = 0.0f;
	if(m_bPressureDitherCircle)
	{
		fCircle = iPressure * 0.1f;
		if(fCircle > 90.0f)
			fCircle = 90.0f;
	}
	else
	{
		fCircle = ((float)rand())/RAND_MAX*m_iDitherCircle;
	}

	memset(m_pTempData, 0, m_iWidth * m_iHeight);
	for( int h = 0; h < m_iHeight; ++h)
	{
		for( int w = 0; w < m_iWidth; ++w)
		{
			int y  = (h - m_iHeight/2)/cos(fCircle*(A3D_PI/180)) + m_iHeight/2;
			if(y <= 0) y = 1;
			if(y >= m_iHeight - 2) y = m_iHeight - 2;

			int ww = w;

			if (ww > m_iWidth - 2)
				ww = m_iWidth - 2;

			int a = m_pResultData[y*m_iWidth + ww];
			int b = m_pResultData[y*m_iWidth + ww + 1];
			int c = m_pResultData[(y+1)*m_iWidth + ww];
			int d = m_pResultData[(y+1)*m_iWidth + ww + 1];


			m_pTempData[h*m_iWidth + w] = (a + b + c + d)/4;
		}
	}
	memcpy(m_pResultData, m_pTempData, m_iWidth*m_iHeight);
}