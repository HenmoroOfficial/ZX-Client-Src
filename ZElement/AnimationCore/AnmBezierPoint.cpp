#include "stdafx.h"
#include "Animator.h"
#include "AnmBezierPoint.h"

CAnmBezierPoint::CAnmBezierPoint(A3DWireCollector *pA3DWireCollector)
{
    m_pA3DWireCollector = pA3DWireCollector;
    m_uiID = -1;
    m_vPos = A3DVECTOR3(0);
    m_cColor = A3DCOLORRGB(255, 0, 255);
    m_iTotalFrames = 1;
    m_fFOV = DEG2RAD(65.0f);
    m_pPreSegment = 0;
    m_pNextSegment = 0;
    m_qOrientation = A3DQUATERNION(1.0f, 0.0f, 0.0f, 0.0f);

    float f = -0.03f;
    m_aFrustum[0] = A3DVECTOR3(-f, -f, f);
    m_aFrustum[1] = A3DVECTOR3(f, -f, f);
    m_aFrustum[2] = A3DVECTOR3(f, f, f);
    m_aFrustum[3] = A3DVECTOR3(-f, f, f);
    f = -0.05f;
    m_aFrustum[4] = A3DVECTOR3(-f, -f, 2*f);
    m_aFrustum[5] = A3DVECTOR3(f, -f, 2*f);
    m_aFrustum[6] = A3DVECTOR3(f, f, 2*f);
    m_aFrustum[7] = A3DVECTOR3(-f, f, 2*f);
}

void CAnmBezierPoint::Render()
{
    m_pA3DWireCollector->AddAABB(GetAABB(), m_cColor);
    A3DVECTOR3 aFrustum[8];
    for (unsigned int i=0; i<8; ++i)
    {
        aFrustum[i] = m_qOrientation ^ m_aFrustum[i];
        aFrustum[i] += m_vPos;
    }
    m_pA3DWireCollector->Add3DLine(aFrustum[0], aFrustum[1], m_cColor);
    m_pA3DWireCollector->Add3DLine(aFrustum[1], aFrustum[2], m_cColor);
    m_pA3DWireCollector->Add3DLine(aFrustum[2], aFrustum[3], m_cColor);
    m_pA3DWireCollector->Add3DLine(aFrustum[3], aFrustum[0], m_cColor);

    m_pA3DWireCollector->Add3DLine(aFrustum[0], aFrustum[4], m_cColor);
    m_pA3DWireCollector->Add3DLine(aFrustum[1], aFrustum[5], m_cColor);
    m_pA3DWireCollector->Add3DLine(aFrustum[2], aFrustum[6], m_cColor);
    m_pA3DWireCollector->Add3DLine(aFrustum[3], aFrustum[7], m_cColor);

    m_pA3DWireCollector->Add3DLine(aFrustum[4], aFrustum[5], A3DCOLORRGB(0, 255, 0));
    m_pA3DWireCollector->Add3DLine(aFrustum[5], aFrustum[6], m_cColor);
    m_pA3DWireCollector->Add3DLine(aFrustum[6], aFrustum[7], A3DCOLORRGB(0, 0, 255));
    m_pA3DWireCollector->Add3DLine(aFrustum[7], aFrustum[4], m_cColor);
    A3DVECTOR3 vPosIn = m_vPos+A3DVECTOR3(0.0f, 0.05f, 0.0f);
    A3DVECTOR3 vPosOut;
    g_Animator.GetA3DViewport()->Transform(vPosIn, vPosOut);
    g_Animator.GetA3DEngine()->GetA3DFlatCollector()->AddRect_2D(int(vPosOut.x-20), int(vPosOut.y-5),
        int(vPosOut.x+20), int(vPosOut.y+6), 0x7F3F007F, vPosOut.z);
    ACString number;
    number.Format(_AL("%u"), m_uiID);
    g_Animator.GetA3DEngine()->GetSystemFont()->TextOut3D(number,
            g_Animator.GetA3DViewport(), vPosIn, 0, A3DCOLORRGB(255, 255, 255));
}

void CAnmBezierPoint::SetID(const unsigned int uiID)
{
    m_uiID = uiID;
}

unsigned int CAnmBezierPoint::GetID() const
{
    return m_uiID;
}

void CAnmBezierPoint::SetPos(const A3DVECTOR3 &vPos)
{
    m_vPos = vPos;
}

A3DVECTOR3 CAnmBezierPoint::GetPos() const
{
    return m_vPos;
}

void CAnmBezierPoint::SetOrientation(const A3DQUATERNION &q)
{
    m_qOrientation = q;
}

A3DQUATERNION CAnmBezierPoint::GetOrientation() const
{
    return m_qOrientation;
}

void CAnmBezierPoint::YawDelta(const float a)
{
    A3DQUATERNION q = A3DQUATERNION(A3DVECTOR3(0.0f, 1.0f, 0.0f), a);
    m_qOrientation = q * m_qOrientation;
}

void CAnmBezierPoint::PitchDelta(const float a)
{
    A3DQUATERNION q = A3DQUATERNION(A3DVECTOR3(1.0f, 0.0f, 0.0f), a);
    m_qOrientation = m_qOrientation * q;
}

void CAnmBezierPoint::RollDelta(const float a)
{
    A3DQUATERNION q = A3DQUATERNION(A3DVECTOR3(0.0f, 0.0f, 1.0f), a);
    m_qOrientation = m_qOrientation * q;
}

void CAnmBezierPoint::SetTotalFrames(const int iFrame)
{
    if (iFrame > 0)
    {
        m_iTotalFrames = iFrame;
    }
    else
    {
        m_iTotalFrames = 1;
    }
}

int CAnmBezierPoint::GetTotalFrames() const
{
    return m_iTotalFrames;
}

void CAnmBezierPoint::SetFOV(const float d)
{
    m_fFOV = d;
}

float CAnmBezierPoint::GetFOV() const
{
    return m_fFOV;
}

A3DAABB CAnmBezierPoint::GetAABB() const
{
    A3DVECTOR3 d(0.03f);
    return A3DAABB(m_vPos-d, m_vPos+d);
}

void CAnmBezierPoint::SetColor(const A3DCOLOR &color)
{
    m_cColor = color;
}

A3DCOLOR CAnmBezierPoint::GetColor() const
{
    return m_cColor;
}

void CAnmBezierPoint::SetPreSegment(CAnmBezierSegment *pSegment)
{
    m_pPreSegment = pSegment;
}

CAnmBezierSegment* CAnmBezierPoint::GetPreSegment() const
{
    return m_pPreSegment;
}

void CAnmBezierPoint::SetNextSegment(CAnmBezierSegment *pSegment)
{
    m_pNextSegment = pSegment;
}

CAnmBezierSegment* CAnmBezierPoint::GetNextSegment() const
{
    return m_pNextSegment;
}

CAnmBezierPoint::~CAnmBezierPoint()
{
}

