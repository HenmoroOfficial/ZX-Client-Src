#include "stdafx.h"
#include "Animator.h"
#include "AnmSpeedControl.h"

CAnmSpeedControl::CAnmSpeedControl(const unsigned int uiID)
    : CAnmAction(uiID)
{
    m_fSpeed = 1.0f;
    m_iTotalFrames = 1;
}

void CAnmSpeedControl::SetTotalFrames(const int iFrame)
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

int CAnmSpeedControl::GetTotalFrames() const
{
    return m_iTotalFrames;
}

void CAnmSpeedControl::SetSpeed(const float speed)
{
    m_fSpeed = speed;
}

float CAnmSpeedControl::GetSpeed() const
{
    return m_fSpeed;
}

CAnmAction::ActionType CAnmSpeedControl::GetActionType() const
{
    return CAnmAction::TYPE_SPEED_CONTROL;
}

void CAnmSpeedControl::CopyDataFrom(CAnmAction *pAction)
{
    CAnmSpeedControl *pDataAction = dynamic_cast<CAnmSpeedControl*>(pAction);
    if (pDataAction)
    {
        SetSpeed(pDataAction->GetSpeed());
        SetTotalFrames(pDataAction->GetTotalFrames());
    }
}

bool CAnmSpeedControl::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    if (IsDead())
    {
        return false;
    }

    DWORD dT = dwNowTime - m_dwStartTime;
    DWORD dwTotalTime = GetTotalTime();

    if (dwNowTime >= m_dwStartTime)
    {
        if (dT < dwTotalTime)
        {
            g_Animator.SetSpeed(m_fSpeed);
        }
        else
        {
            g_Animator.SetSpeed(1.0f);
        }
    }

    if (dT >= dwTotalTime)
    {
        m_bDead = true;
    }
    return IsDead();
}

DWORD CAnmSpeedControl::GetTotalTime() const
{
    return TIME_PER_FRAME * m_iTotalFrames;
}

CAnmSpeedControl::~CAnmSpeedControl()
{
}
