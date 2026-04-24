#include "stdafx.h"
#include "Animator.h"
#include "AnmMove.h"

CAnmMove::CAnmMove(const unsigned int uiID)
    : CAnmAction(uiID)
{
    m_vStartPos = A3DVECTOR3(0.0f);
    m_vEndPos = A3DVECTOR3(0.0f);
    m_qOrientation = A3DQUATERNION(1.0f, 0.0f, 0.0f, 0.0f);
    m_iRotateAxis = ROTATE_AXIS_Y;
    m_fAngle = 0.0f;
    m_fAccelerationPercent = 0.0f;
    m_iTotalFrames = 1;
    m_bCloseTerrain = true;
}

void CAnmMove::SetStartPos(const A3DVECTOR3 &vPos)
{
    m_vStartPos = vPos;
}

A3DVECTOR3 CAnmMove::GetStartPos() const
{
    return m_vStartPos;
}

void CAnmMove::SetEndPos(const A3DVECTOR3 &vPos)
{
    m_vEndPos = vPos;
}

A3DVECTOR3 CAnmMove::GetEndPos() const
{
    return m_vEndPos;
}

void CAnmMove::SetOrientation(const A3DQUATERNION &qOri)
{
    m_qOrientation = qOri;
}

A3DQUATERNION CAnmMove::GetOrientation() const
{
    return m_qOrientation;
}

void CAnmMove::SetRotateAxis(const int a)
{
    if (a >= ROTATE_AXIS_X && a <= ROTATE_AXIS_Z)
    {
        m_iRotateAxis = a;
    }
}

int CAnmMove::GetRotateAxis() const
{
    return m_iRotateAxis;
}

void CAnmMove::SetAngle(const float a)
{
    m_fAngle = a;
}

float CAnmMove::GetAngle() const
{
    return m_fAngle;
}

void CAnmMove::SetAccelerationPercent(const float fA)
{
    m_fAccelerationPercent = fA;
}

float CAnmMove::GetAccelerationPercent() const
{
    return m_fAccelerationPercent;
}

void CAnmMove::SetTotalFrames(const int iFrame)
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

int CAnmMove::GetTotalFrames() const
{
    return m_iTotalFrames;
}

void CAnmMove::SetCloseTerrain(const bool b)
{
    m_bCloseTerrain = b;
}

bool CAnmMove::GetCloseTerrain() const
{
    return m_bCloseTerrain;
}

CAnmAction::ActionType CAnmMove::GetActionType() const
{
    return CAnmAction::TYPE_MOVE;
}

void CAnmMove::CopyDataFrom(CAnmAction *pAction)
{
    CAnmMove *pDataAction = dynamic_cast<CAnmMove*>(pAction);
    if (pDataAction)
    {
        SetStartPos(pDataAction->GetStartPos());
        SetEndPos(pDataAction->GetEndPos());
        SetOrientation(pDataAction->GetOrientation());
        SetRotateAxis(pDataAction->GetRotateAxis());
        SetAngle(pDataAction->GetAngle());
        SetAccelerationPercent(pDataAction->GetAccelerationPercent());
        SetCloseTerrain(pDataAction->GetCloseTerrain());
        SetTotalFrames(pDataAction->GetTotalFrames());
    }
}

bool CAnmMove::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    if (IsDead())
    {
        return false;
    }

    DWORD dT = dwNowTime - m_dwStartTime;
    DWORD dwTotalTime = GetTotalTime();
    CAnmObject *pObject = pObjectManager->GetObject(m_uiObjectID);
    if (pObject)
    {
        if (dwNowTime >= m_dwStartTime)
        {
            A3DVECTOR3 vdS = m_vEndPos - m_vStartPos;
            A3DVECTOR3 vPos;
            float angle;

            if (dT < dwTotalTime)
            {
                float fVelocity = GetVelocity();
                float fDV = fVelocity * m_fAccelerationPercent;
                float fAcceleration = 2.0f * fDV / dwTotalTime;
                float fStartVelocity = fVelocity - fDV;
                float dS = (fStartVelocity + fAcceleration * (float)dT * 0.5f) * (float)dT;
                if (vdS.Magnitude() > 1e-8)
                {
                    float k = dS / vdS.Magnitude();
                    vPos.x = m_vStartPos.x + k * vdS.x;
                    vPos.y = m_vStartPos.y + k * vdS.y;
                    vPos.z = m_vStartPos.z + k * vdS.z;
                }
                else
                {
                    vPos = m_vEndPos;
                }
                // Set rotation
                angle = m_fAngle * dT / dwTotalTime;
            }
            else
            {
                vPos = m_vEndPos;
                angle = m_fAngle;
            }

            if (m_bCloseTerrain)
            {
                vPos.y = g_Animator.GetPosHeight(vPos);
            }
            pObject->SetPos(vPos);

            // Set rotation
            A3DQUATERNION qRotate;
            if (m_iRotateAxis == ROTATE_AXIS_X)
            {
                qRotate.ConvertFromAxisAngle(A3DVECTOR3(1.0f, 0.0f, 0.0f), angle * 0.0174532925f);
            }
            else if (m_iRotateAxis == ROTATE_AXIS_Y)
            {
                qRotate.ConvertFromAxisAngle(A3DVECTOR3(0.0f, 1.0f, 0.0f), angle * 0.0174532925f);
            }
            else if (m_iRotateAxis == ROTATE_AXIS_Z)
            {
                qRotate.ConvertFromAxisAngle(A3DVECTOR3(0.0f, 0.0f, 1.0f), angle * 0.0174532925f);
            }
            A3DQUATERNION qOri = m_qOrientation * qRotate;
            pObject->SetDirAndUp(qOri ^ A3DVECTOR3(0.0f, 0.0f, -1.0f), qOri ^ A3DVECTOR3(0.0f, 1.0f, 0.0f));
        }
    }

    if (dT >= dwTotalTime)
    {
        m_bDead = true;
    }
    return IsDead();
}

DWORD CAnmMove::GetTotalTime() const
{
    return TIME_PER_FRAME * m_iTotalFrames;
}

CAnmMove::~CAnmMove()
{
}

float CAnmMove::GetVelocity() const
{
    A3DVECTOR3 vS = m_vEndPos - m_vStartPos;
    float t = m_iTotalFrames * (float)TIME_PER_FRAME;
    return vS.Magnitude() / t;
}
