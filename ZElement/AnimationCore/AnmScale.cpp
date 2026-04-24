#include "stdafx.h"
#include "Animator.h"
#include "AnmScale.h"

CAnmScale::CAnmScale(const unsigned int uiID)
    : CAnmAction(uiID)
{
    m_fStartScale = 1.0f;
    m_fEndScale = 1.0f;
    m_iTotalFrames = 1;
}

void CAnmScale::SetStartScale(const float s)
{
    m_fStartScale = s;
}

float CAnmScale::GetStartScale() const
{
    return m_fStartScale;
}

void CAnmScale::SetEndScale(const float s)
{
    m_fEndScale = s;
}

float CAnmScale::GetEndScale() const
{
    return m_fEndScale;
}

void CAnmScale::SetTotalFrames(const int iFrame)
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

int CAnmScale::GetTotalFrames() const
{
    return m_iTotalFrames;
}

CAnmAction::ActionType CAnmScale::GetActionType() const
{
    return CAnmAction::TYPE_SCALE;
}

void CAnmScale::CopyDataFrom(CAnmAction *pAction)
{
    CAnmScale *pDataAction = dynamic_cast<CAnmScale*>(pAction);
    if (pDataAction)
    {
        SetStartScale(pDataAction->GetStartScale());
        SetEndScale(pDataAction->GetEndScale());
        SetTotalFrames(pDataAction->GetTotalFrames());
    }
}

bool CAnmScale::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
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
            // Set scale
            if (dT < dwTotalTime)
            {
                float scale = m_fStartScale + (m_fEndScale - m_fStartScale) * dT / dwTotalTime;
                pObject->SetScale(scale);
            }
            else
            {
                pObject->SetScale(m_fEndScale);
            }
        }
    }

    if (dT >= dwTotalTime)
    {
        m_bDead = true;
    }
    return IsDead();
}

DWORD CAnmScale::GetTotalTime() const
{
    return TIME_PER_FRAME * m_iTotalFrames;
}

CAnmScale::~CAnmScale()
{
}
