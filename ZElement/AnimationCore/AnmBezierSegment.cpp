#include "stdafx.h"
#include "AnmAction.h"
#include "AnmBezierSegment.h"

CAnmBezierSegment::CAnmBezierSegment(A3DWireCollector *pA3DWireCollector)
{
    m_pA3DWireCollector = pA3DWireCollector;
    m_uiID = -1;
    m_vAnchorA = A3DVECTOR3(0.0f);
    m_vAnchorB = A3DVECTOR3(0.0f);
    m_pHead = 0;
    m_pTail = 0;
    m_cSegmentColor = A3DCOLORRGB(255, 255, 0);
    m_cAnchorColor = A3DCOLORRGB(0, 255, 0);
    m_fLength = 0.0f;
    m_iTotalFrames = 1;
    m_bDirty = true;
}

A3DVECTOR3 CAnmBezierSegment::Bezier(const float u)
{
    A3DVECTOR3 vPos;
    A3DVECTOR3 vHeadPos = m_pHead->GetPos();
    A3DVECTOR3 vTailPos = m_pTail->GetPos();
    float a, b, c;
    for (int i=0; i<3; ++i)
    {
        a = vTailPos.m[i] - vHeadPos.m[i] + 3*m_vAnchorA.m[i] - 3*m_vAnchorB.m[i];
        b = 3*vHeadPos.m[i] - 6*m_vAnchorA.m[i] + 3*m_vAnchorB.m[i];
        c = 3*m_vAnchorA.m[i] - 3*vHeadPos.m[i];
        vPos.m[i] = (a * u*u*u) + (b * u*u) + (c * u) + vHeadPos.m[i];
    }
    return vPos;
}

A3DQUATERNION CAnmBezierSegment::Orientation(const float u)
{
    return SLERPQuad(m_pHead->GetOrientation(), m_pTail->GetOrientation(), u);
}

float CAnmBezierSegment::FOV(const float u)
{
    return m_pHead->GetFOV() + u * (m_pTail->GetFOV() - m_pHead->GetFOV());
}

void CAnmBezierSegment::Render()
{
    if (m_bDirty)
    {
        ReCalculateLength();
    }
    for (int i=1; i<=CALCULATE_DIVIDE; ++i)
    {
        m_pA3DWireCollector->Add3DLine(m_vTempPosArray[i-1], m_vTempPosArray[i], m_cSegmentColor);
    }

    m_pA3DWireCollector->Add3DLine(m_pHead->GetPos(), m_vAnchorA, m_cAnchorColor);
    m_pA3DWireCollector->Add3DLine(m_vAnchorB, m_pTail->GetPos(), m_cAnchorColor);

    m_pA3DWireCollector->AddAABB(GetAnchorAAABB(), m_cSegmentColor);
    m_pA3DWireCollector->AddAABB(GetAnchorBAABB(), m_cSegmentColor);
}

float CAnmBezierSegment::GetLength()
{
    if (m_bDirty)
    {
        ReCalculateLength();
    }
    return m_fLength;
}

DWORD CAnmBezierSegment::GetTotalTime() const
{
    return m_iTotalFrames * CAnmAction::TIME_PER_FRAME;
}

int CAnmBezierSegment::GetTotalFrames() const
{
    return m_iTotalFrames;
}

void CAnmBezierSegment::SetID(const unsigned int uiID)
{
    m_uiID = uiID;
}

unsigned int CAnmBezierSegment::GetID() const
{
    return m_uiID;
}

void CAnmBezierSegment::SetAnchorA(const A3DVECTOR3 &v)
{
    m_vAnchorA = v;
    m_bDirty = true;
}

A3DVECTOR3 CAnmBezierSegment::GetAnchorA() const
{
    return m_vAnchorA;
}

void CAnmBezierSegment::SetAnchorB(const A3DVECTOR3 &v)
{
    m_vAnchorB = v;
    m_bDirty = true;
}

A3DVECTOR3 CAnmBezierSegment::GetAnchorB() const
{
    return m_vAnchorB;
}

void CAnmBezierSegment::SetHead(CAnmBezierPoint *pPoint)
{
    m_pHead = pPoint;
    m_bDirty = true;
}

CAnmBezierPoint* CAnmBezierSegment::GetHead() const
{
    return m_pHead;
}

void CAnmBezierSegment::SetTail(CAnmBezierPoint *pPoint)
{
    m_pTail = pPoint;
    m_bDirty = true;
}

CAnmBezierPoint* CAnmBezierSegment::GetTail() const
{
    return m_pTail;
}

A3DAABB CAnmBezierSegment::GetAnchorAAABB() const
{
    A3DVECTOR3 d(0.02f);
    return A3DAABB(m_vAnchorA-d, m_vAnchorA+d);
}

A3DAABB CAnmBezierSegment::GetAnchorBAABB() const
{
    A3DVECTOR3 d(0.02f);
    return A3DAABB(m_vAnchorB-d, m_vAnchorB+d);
}

CAnmBezierSegment::~CAnmBezierSegment()
{
}

void CAnmBezierSegment::ReCalculateLength()
{
    A3DVECTOR3 vOldPos = Bezier(0.0f);
    A3DVECTOR3 vNewPos;
    m_vTempPosArray[0] = vOldPos;
    m_fLength = 0.0f;
    for (size_t i=1; i<=CALCULATE_DIVIDE; ++i)
    {
        vNewPos = Bezier((float)i / CALCULATE_DIVIDE);
        A3DVECTOR3 vTemp = vNewPos - vOldPos;
        m_fLength += vTemp.Magnitude();
        vOldPos = vNewPos;
        m_vTempPosArray[i] = vOldPos;
    }
    m_iTotalFrames = m_pHead->GetTotalFrames();
    m_bDirty = false;
}
