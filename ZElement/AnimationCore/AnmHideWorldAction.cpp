#include "stdafx.h"
#include "Animator.h"
#include "AnmHideWorldAction.h"

CAnmHideWorldAction::CAnmHideWorldAction(const unsigned int uiID)
    : CAnmAction(uiID)
{
    m_uiHideWorldType = (unsigned int)HWT_HIDE_ANIMATION_MODEL;
    m_uiColor = 0xFF000000;
    m_iTotalFrames = 1;
}

void CAnmHideWorldAction::SetHideWorldType(const unsigned int t)
{
    m_uiHideWorldType = t;
}

unsigned int CAnmHideWorldAction::GetHideWorldType() const
{
    return m_uiHideWorldType;
}

void CAnmHideWorldAction::SetColor(const unsigned int color)
{
    m_uiColor = color;
}

unsigned int CAnmHideWorldAction::GetColor() const
{
    return m_uiColor;
}

void CAnmHideWorldAction::SetTotalFrames(const int iFrame)
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

int CAnmHideWorldAction::GetTotalFrames() const
{
    return m_iTotalFrames;
}

CAnmAction::ActionType CAnmHideWorldAction::GetActionType() const
{
    return CAnmAction::TYPE_HIDE_WORLD_ACTION;
}

void CAnmHideWorldAction::CopyDataFrom(CAnmAction *pAction)
{
    CAnmHideWorldAction *pDataAction = dynamic_cast<CAnmHideWorldAction*>(pAction);
    if (pDataAction)
    {
        SetHideWorldType(pDataAction->GetHideWorldType());
        SetTotalFrames(pDataAction->GetTotalFrames());
    }
}

bool CAnmHideWorldAction::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    if (IsDead())
    {
        return false;
    }

    DWORD dT = dwNowTime - m_dwStartTime;
    DWORD dwTotalTime = GetTotalTime();
    if (dwNowTime >= m_dwStartTime)
    {
        // Set Hide World Type
        if (dT < dwTotalTime)
        {
            g_Animator.SetHideWorldType(m_uiHideWorldType);
            if (m_uiHideWorldType == HWT_HIDE_WORLD)
            {
                g_Animator.SetBackgroundColor(m_uiColor);
            }
        }
    }

    if (dT >= dwTotalTime)
    {
        m_bDead = true;
    }
    return IsDead();
}

DWORD CAnmHideWorldAction::GetTotalTime() const
{
    return TIME_PER_FRAME * m_iTotalFrames;
}

CAnmHideWorldAction::~CAnmHideWorldAction()
{
}
