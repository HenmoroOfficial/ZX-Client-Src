#include "stdafx.h"
#include "Animator.h"
#include "AnmTransparent.h"

CAnmTransparent::CAnmTransparent(const unsigned int uiID)
    : CAnmAction(uiID)
{
    m_fStartTransparent = 1.0f;
    m_fEndTransparent = 1.0f;
    m_iTotalFrames = 1;
}

void CAnmTransparent::SetStartTransparent(const float t)
{
    m_fStartTransparent = t;
}

float CAnmTransparent::GetStartTransparent() const
{
    return m_fStartTransparent;
}

void CAnmTransparent::SetEndTransparent(const float t)
{
    m_fEndTransparent = t;
}

float CAnmTransparent::GetEndTransparent() const
{
    return m_fEndTransparent;
}

void CAnmTransparent::SetTotalFrames(const int iFrame)
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

int CAnmTransparent::GetTotalFrames() const
{
    return m_iTotalFrames;
}

CAnmAction::ActionType CAnmTransparent::GetActionType() const
{
    return CAnmAction::TYPE_TRANSPARENT;
}

void CAnmTransparent::CopyDataFrom(CAnmAction *pAction)
{
    CAnmTransparent *pDataAction = dynamic_cast<CAnmTransparent*>(pAction);
    if (pDataAction)
    {
        SetStartTransparent(pDataAction->GetStartTransparent());
        SetEndTransparent(pDataAction->GetEndTransparent());
        SetTotalFrames(pDataAction->GetTotalFrames());
    }
}

bool CAnmTransparent::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
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
            pObject->SetVisible(true);

            // Set Transparent
            if (dT < dwTotalTime)
            {
                float transparent = m_fStartTransparent + (m_fEndTransparent - m_fStartTransparent) * dT / dwTotalTime;
                pObject->SetTransparent(transparent);
            }
            else
            {
                pObject->SetTransparent(m_fEndTransparent);
            }
        }
    }

    if (dT >= dwTotalTime)
    {
        m_bDead = true;
    }
    return IsDead();
}

DWORD CAnmTransparent::GetTotalTime() const
{
    return TIME_PER_FRAME * m_iTotalFrames;
}

CAnmTransparent::~CAnmTransparent()
{
}
