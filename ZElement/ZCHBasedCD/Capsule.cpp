// Capusule.cpp: implementation of the CCapsule class.
//
//////////////////////////////////////////////////////////////////////

#include "Capsule.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace CHBasedCD{

CCapsule::CCapsule()
{
	m_up = A3DVECTOR3(0, 1, 0);
	InitDir();
}

CCapsule::~CCapsule()
{

}

void CCapsule::InitDir()
{
	A3DVECTOR3 vAxisX(1, 0, 0);
	A3DVECTOR3 vAxisZ(0, 0, 1);

	m_right = CrossProduct(m_up, vAxisX);
	if (m_right.SquaredMagnitude() < 1E-4f)
		m_right = CrossProduct(m_up, vAxisZ);
	m_front = CrossProduct(m_right, m_up);

	m_centerHei = fabsf(m_up.y) * (m_height/2) + m_radius;
}

//render wired capsule
void CCapsule::RenderWire(A3DWireCollector* pWireCollector,  DWORD color)
{
	A3DCAPSULE cc;
	cc.vAxisY = m_up;
	cc.vAxisX = m_right;
	cc.vAxisZ = m_front;

	cc.fRadius = m_radius;
	cc.fHalfLen = m_height/2;
	cc.vCenter = m_pos;
	pWireCollector->AddCapsule(cc, color);
}

}