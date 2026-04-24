// Filename	: EC_CollisionText.cpp
// Creator	: Fu Chonggang
// Date		: 2012.10.18

#include "EC_CollisionTest.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "A3DEngine.h"
#include "A3DWireCollector.h"
#include "A3DMacros.h"

DispColliInfo g_DispColliInfo;

void DispColliInfo::Add(A3DVECTOR3 p1,
						A3DVECTOR3 p2,
						A3DVECTOR3 v1,
						A3DVECTOR3 v2,
						A3DVECTOR3 v12,
						A3DVECTOR3 v22)
{
	m_vecInfos.push_back( ColliInfo(p1, p2, v1, v2, v12, v22) );
	m_iIndex=m_vecInfos.size()-1;
}


void DispColliInfo::Render()
{
	static int actionTime = 0;
	static int dwLastTick = 0;
	if (actionTime>500)
	{
		if (GetAsyncKeyState('9')&0x8000)
		{
			actionTime = 0;
			m_iIndex++;
			if (m_iIndex>=m_vecInfos.size())
				m_iIndex = 0;
		}
		else if (GetAsyncKeyState('8')&0x8000)
		{
			actionTime = 0;
			m_iIndex--;
			if (m_iIndex<0)
				m_iIndex = m_vecInfos.size()-1;
		}
		else if (GetAsyncKeyState('6')&0x8000)
		{
			actionTime = 0;
			m_iIndex = 0;
			m_vecInfos.clear();
		}
	}
	int dwTick = GetTickCount();
	actionTime += (dwTick-dwLastTick);
	dwLastTick = dwTick;
	if (m_iIndex>=m_vecInfos.size())
	{
		m_iIndex=m_vecInfos.size()-1;
	}
	if (m_iIndex<0 || m_iIndex>=m_vecInfos.size())
	{
		return;
	}

	RenderInfo(m_vecInfos[m_iIndex]);
}

A3DVECTOR3 xz2xy(A3DVECTOR3& a)
{
	return A3DVECTOR3(a.x, a.z, 0);
}

void DispColliInfo::RenderInfo(ColliInfo &dispInfo)
{
	A3DWireCollector *pWC = g_pGame->GetA3DEngine()->GetA3DWireCollector();
	
	A3DVECTOR3 c = dispInfo.p1-A3DVECTOR3(3, 0, 3);
	A3DVECTOR3 p1 = (dispInfo.p1 - c)*100;
	A3DVECTOR3 p2 = (dispInfo.p2 - c)*100;
	DWORD colP1 = A3DCOLORRGB(234, 159, 90);
	DWORD colP2 = A3DCOLORRGB(234, 255, 90);
	DWORD colArrow1 = A3DCOLORRGB(111, 111, 255);
	DWORD colArrow2 = A3DCOLORRGB(255, 111, 211);
	pWC->AddRect_2D(p1.x-30, p1.z-30, p1.x+30, p1.z+30, colP1);
	pWC->AddRect_2D(p2.x-30, p2.z-30, p2.x+30, p2.z+30, colP1);
	
	pWC->AddLine_2D(xz2xy(p1), xz2xy(p1+dispInfo.v1*30), colArrow1);
	pWC->AddLine_2D(xz2xy(p2), xz2xy(p2+dispInfo.v2*30), colArrow1);
	pWC->AddLine_2D(xz2xy(p1), xz2xy(p1+dispInfo.v12*30), colArrow2);
	pWC->AddLine_2D(xz2xy(p2), xz2xy(p2+dispInfo.v22*30), colArrow2);
	pWC->Flush_2D();
}