#include "stdafx.h"
#include "AnmAxes.h"

CAnmAxes::CAnmAxes(A3DWireCollector *pA3DWireCollector)
{
    m_pA3DWireCollector = pA3DWireCollector;
    m_vPos = 0;
    m_eSelectedAxisType = TYPE_NONE;
    m_bVisable = false;
}

void CAnmAxes::Render()
{
    if (m_bVisable)
    {
        if (m_eSelectedAxisType == TYPE_X)
        {
            m_pA3DWireCollector->AddAABB(GetAxisAABB(TYPE_X), A3DCOLORRGB(255, 255, 255));
            m_pA3DWireCollector->Add3DLine(m_vPos, m_vPos+A3DVECTOR3(1.0f, 0.0f, 0.0f), A3DCOLORRGB(255, 255, 255));
        }
        else
        {
            m_pA3DWireCollector->AddAABB(GetAxisAABB(TYPE_X), A3DCOLORRGB(255, 0, 0));
            m_pA3DWireCollector->Add3DLine(m_vPos, m_vPos+A3DVECTOR3(1.0f, 0.0f, 0.0f), A3DCOLORRGB(255, 0, 0));
        }

        if (m_eSelectedAxisType == TYPE_Y)
        {
            m_pA3DWireCollector->AddAABB(GetAxisAABB(TYPE_Y), A3DCOLORRGB(255, 255, 255));
            m_pA3DWireCollector->Add3DLine(m_vPos, m_vPos+A3DVECTOR3(0.0f, 1.0f, 0.0f), A3DCOLORRGB(255, 255, 255));
        }
        else
        {
            m_pA3DWireCollector->AddAABB(GetAxisAABB(TYPE_Y), A3DCOLORRGB(0, 255, 0));
            m_pA3DWireCollector->Add3DLine(m_vPos, m_vPos+A3DVECTOR3(0.0f, 1.0f, 0.0f), A3DCOLORRGB(0, 255, 0));
        }

        if (m_eSelectedAxisType == TYPE_Z)
        {
            m_pA3DWireCollector->AddAABB(GetAxisAABB(TYPE_Z), A3DCOLORRGB(255, 255, 255));
            m_pA3DWireCollector->Add3DLine(m_vPos, m_vPos+A3DVECTOR3(0.0f, 0.0f, 1.0f), A3DCOLORRGB(255, 255, 255));
        }
        else
        {
            m_pA3DWireCollector->AddAABB(GetAxisAABB(TYPE_Z), A3DCOLORRGB(0, 0, 255));
            m_pA3DWireCollector->Add3DLine(m_vPos, m_vPos+A3DVECTOR3(0.0f, 0.0f, 1.0f), A3DCOLORRGB(0, 0, 255));
        }
    }
}

void CAnmAxes::SetPos(const A3DVECTOR3 &v)
{
    m_vPos = v;
}

A3DVECTOR3 CAnmAxes::GetPos() const
{
    return m_vPos;
}

void CAnmAxes::SelectAxis(const AxisType type)
{
    m_eSelectedAxisType = type;
}

CAnmAxes::AxisType CAnmAxes::GetSelectedAxis() const
{
    return m_eSelectedAxisType;
}

void CAnmAxes::SetVisable(const bool b)
{
    if (!b)
    {
        m_eSelectedAxisType = TYPE_NONE;
    }
    m_bVisable = b;
}

bool CAnmAxes::IsVisable() const
{
    return m_bVisable;
}

void CAnmAxes::MoveX(const float d)
{
    m_vPos.x += d;
}

void CAnmAxes::MoveY(const float d)
{
    m_vPos.y += d;
}

void CAnmAxes::MoveZ(const float d)
{
    m_vPos.z += d;
}

A3DAABB CAnmAxes::GetAxisAABB(const AxisType type)
{
    A3DVECTOR3 d(0.05f);

    switch (type)
    {
        case TYPE_X:
        {
            return A3DAABB(m_vPos+A3DVECTOR3(1.0f, 0.0f, 0.0f)-d, m_vPos+A3DVECTOR3(1.0f, 0.0f, 0.0f)+d);
        }

        case TYPE_Y:
        {
            return A3DAABB(m_vPos+A3DVECTOR3(0.0f, 1.0f, 0.0f)-d, m_vPos+A3DVECTOR3(0.0f, 1.0f, 0.0f)+d);
        }

        case TYPE_Z:
        {
            return A3DAABB(m_vPos+A3DVECTOR3(0.0f, 0.0f, 1.0f)-d, m_vPos+A3DVECTOR3(0.0f, 0.0f, 1.0f)+d);
        }

        default:
            break;
    }
    return A3DAABB(m_vPos-d, m_vPos+d);
}

CAnmAxes::~CAnmAxes()
{
}
