#include "stdafx.h"
#include "Animator.h"
#include "AnmJump.h"

const float CAnmJump::GRAVITY = 0.00000980665f;

CAnmJump::CAnmJump(const unsigned int uiID)
    : CAnmAction(uiID)
{
    m_vStartPos = A3DVECTOR3(0.0);
    m_vEndPos = A3DVECTOR3(0.0);
    m_iTotalFrames = 1;
}

void CAnmJump::SetStartPos(const A3DVECTOR3 &vPos)
{
    m_vStartPos = vPos;
}

A3DVECTOR3 CAnmJump::GetStartPos() const
{
    return m_vStartPos;
}

void CAnmJump::SetEndPos(const A3DVECTOR3 &vPos)
{
    m_vEndPos = vPos;
}

A3DVECTOR3 CAnmJump::GetEndPos() const
{
    return m_vEndPos;
}

void CAnmJump::SetTotalFrames(const int iFrame)
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

int CAnmJump::GetTotalFrames() const
{
    return m_iTotalFrames;
}

CAnmAction::ActionType CAnmJump::GetActionType() const
{
    return CAnmAction::TYPE_JUMP;
}

void CAnmJump::CopyDataFrom(CAnmAction *pAction)
{
    CAnmJump *pDataAction = dynamic_cast<CAnmJump*>(pAction);
    if (pDataAction)
    {
        SetStartPos(pDataAction->GetStartPos());
        SetEndPos(pDataAction->GetEndPos());
        SetTotalFrames(pDataAction->GetTotalFrames());
    }
}

bool CAnmJump::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    if (IsDead())
    {
        return false;
    }
    
    DWORD dT = dwNowTime - m_dwStartTime;
    DWORD dwTotalTime = GetTotalTime();
    CAnmObject *pObject = pObjectManager->GetObject(m_uiObjectID);
    A3DVECTOR3 vPos;
    if (pObject)
    {
        if (dwNowTime >= m_dwStartTime)
        {
            A3DVECTOR3 vdS = m_vEndPos - m_vStartPos;

            if (0.5f * dwTotalTime * dwTotalTime * GRAVITY < -vdS.y)
            {
                float fVelocity = vdS.Magnitude() / dwTotalTime;
                float dS = fVelocity * (float)dT;
                float k = dS / vdS.Magnitude();
                vPos.x = m_vStartPos.x + k * vdS.x;
                vPos.y = m_vStartPos.y + k * vdS.y;
                vPos.z = m_vStartPos.z + k * vdS.z;
            }
            else
            {
                // G * (t1 * t1 - t2 * t2) = 2 * DH
                // t1 + t2 = T
                float fFirstPartTime = (0.5f * dwTotalTime) + (vdS.y / (GRAVITY * dwTotalTime));
                float fStartVelocity = fFirstPartTime * GRAVITY;
                float fNowVelocity = fStartVelocity - dT * GRAVITY;
                float fH = (fStartVelocity + fNowVelocity) * dT * 0.5f;

                if (vdS.Magnitude() > 1e-8)
                {
                    float k = dT / (float)dwTotalTime;
                    vPos.x = m_vStartPos.x + k * vdS.x;
                    vPos.y = m_vStartPos.y + fH;
                    vPos.z = m_vStartPos.z + k * vdS.z;
                }
                else
                {
                    vPos.x = m_vEndPos.x;
                    vPos.y = m_vStartPos.y + fH;
                    vPos.z = m_vEndPos.z;
                }
            }

            if (dT >= dwTotalTime)
            {
                vPos = m_vEndPos;
            }
            pObject->SetPos(vPos);
        }
    }

    if (dT >= dwTotalTime)
    {
        m_bDead = true;
    }
    return IsDead();
}

DWORD CAnmJump::GetTotalTime() const
{
    return TIME_PER_FRAME * m_iTotalFrames;
}

CAnmJump::~CAnmJump()
{
}
