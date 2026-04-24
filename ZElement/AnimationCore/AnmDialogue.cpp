#include "stdafx.h"
#include "AnmDialogue.h"
#include "Animator.h"

CAnmDialogue::CAnmDialogue(const unsigned int uiID)
    : CAnmAction(uiID)
{
    m_sDialogue = _AL("");
    m_iTotalFrames = 1;
}

void CAnmDialogue::SetDialogue(const ACString &st)
{
    m_sDialogue = st;
}

ACString CAnmDialogue::GetDialogue() const
{
    return m_sDialogue;
}

void CAnmDialogue::SetTotalFrames(const int iFrame)
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

int CAnmDialogue::GetTotalFrames() const
{
    return m_iTotalFrames;
}

CAnmAction::ActionType CAnmDialogue::GetActionType() const
{
    return CAnmAction::TYPE_DIALOGUE;
}

void CAnmDialogue::CopyDataFrom(CAnmAction *pAction)
{
    CAnmDialogue *pDataAction = dynamic_cast<CAnmDialogue*>(pAction);
    if (pDataAction)
    {
        SetDialogue(pDataAction->GetDialogue());
        SetTotalFrames(pDataAction->GetTotalFrames());
    }
}

bool CAnmDialogue::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    if (IsDead())
    {
        return false;
    }

    DWORD dT = dwNowTime - m_dwStartTime;
    DWORD dwTotalTime = GetTotalTime();
    if (dwNowTime >= m_dwStartTime)
    {
        g_Animator.SetDialogue(m_sDialogue);
    }

    if (dT >= dwTotalTime)
    {
        m_bDead = true;
    }
    return IsDead();
}

DWORD CAnmDialogue::GetTotalTime() const
{
    return TIME_PER_FRAME * m_iTotalFrames;
}

CAnmDialogue::~CAnmDialogue()
{
}
