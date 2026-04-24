#include "stdafx.h"
#include "AnmObject.h"

float Clamp(float x)
{
    if (x > 1.0f)
        return 1.0f;
    if (x < -1.0f)
        return -1.0f;
    return x;
}

CAnmObject::CAnmObject(const unsigned int uiID)
    : m_uiObjectID(uiID),
      m_bVisible(true)
{
    m_sDisplayName = _AL("Object");
    m_vOffset = A3DVECTOR3(0.0f);
    m_qOrientation = A3DQUATERNION(1.0f, 0.0f, 0.0f, 0.0f);
}

unsigned int CAnmObject::GetObjectID() const
{
    return m_uiObjectID;
}

void CAnmObject::SetOffset(const A3DVECTOR3 &offset)
{
    SetPos(GetPos() - (m_qOrientation ^ (offset - m_vOffset)));
    m_vOffset = offset;
}

A3DVECTOR3 CAnmObject::GetOffset() const
{
    return m_vOffset;
}

void CAnmObject::SetVisible(const bool b)
{
    m_bVisible = b;
}

bool CAnmObject::IsVisible() const
{
    return m_bVisible;
}

bool CAnmObject::IsInAction() const
{
    return false;
}

void CAnmObject::SetDisplayName(const ACString &name)
{
    m_sDisplayName = name;
}

ACString CAnmObject::GetDisplayName() const
{
    return m_sDisplayName;
}

A3DQUATERNION CAnmObject::GetOrientation() const
{
    return m_qOrientation;
}

void CAnmObject::CopyDataFrom(CAnmObject *pObject)
{
    if (pObject)
    {
        m_sDisplayName = pObject->GetDisplayName();
        m_bVisible = pObject->IsVisible();
        m_vOffset = pObject->GetOffset();
        pObject->UpdateOrientation();
        m_qOrientation = pObject->GetOrientation();
    }
}

void CAnmObject::UpdateOrientation()
{
    _UpdateOrientation();
}

void CAnmObject::_UpdateOrientation()
{
    m_qOrientation = A3DQUATERNION(1.0f, 0.0f, 0.0f, 0.0f);
    A3DVECTOR3 dir = GetDir();
    dir.Normalize();
    A3DVECTOR3 up = GetUp();
    up.Normalize();
    A3DVECTOR3 yawD = dir;
    yawD.y = 0;
    yawD.Normalize();
    float angleYaw = (float)acos(Clamp(DotProduct(A3DVECTOR3(0.0f, 0.0f, -1.0f), yawD)));
    if (yawD.x > 0)
    {
        angleYaw = -angleYaw;
    }
    m_qOrientation = A3DQUATERNION(A3DVECTOR3(0.0f, 1.0f, 0.0f), angleYaw) * m_qOrientation;
    float anglePitch = (float)acos(Clamp(DotProduct(yawD, dir)));
    if (dir.y < 0)
    {
        anglePitch = -anglePitch;
    }
    m_qOrientation = m_qOrientation * A3DQUATERNION(A3DVECTOR3(1.0f, 0.0f, 0.0f), anglePitch);
    float angleRoll = (float)acos(Clamp(DotProduct(m_qOrientation ^ A3DVECTOR3(0.0f, 1.0f, 0.0f), up)));
    A3DVECTOR3 right = m_qOrientation ^ A3DVECTOR3(-1.0f, 0.0f, 0.0f);
    float dQ = DotProduct(right, up);
    if (dQ < 0)
    {
        angleRoll = -angleRoll;
    }
    m_qOrientation = m_qOrientation * A3DQUATERNION(A3DVECTOR3(0.0f, 0.0f, 1.0f), angleRoll);
}

CAnmObject::~CAnmObject()
{
}

bool CAnmObject::Load( const char *szFile )
{
	m_strLoadFile = szFile;
	return true;
}
