#include "stdafx.h"
#include "AnmAction.h"

const int CAnmAction::TIME_PER_FRAME = 50;

CAnmAction::CAnmAction(const unsigned int uiID)
{
    m_uiActionID = uiID;
    m_uiObjectID = -1;
    m_dwStartTime = 0;
    m_bDead = false;
}

unsigned int CAnmAction::GetActionID() const
{
    return m_uiActionID;
}

bool CAnmAction::IsDead() const
{
    return m_bDead;
}

void CAnmAction::SetDead(const bool b)
{
    m_bDead = b;
}

void CAnmAction::Reset()
{
    m_bDead = false;
}

void CAnmAction::BindObjectID(const unsigned int uiID)
{
    m_uiObjectID = uiID;
}

unsigned int CAnmAction::GetObjectID() const
{
    return m_uiObjectID;
}

void CAnmAction::SetStartTime(const DWORD dwTime)
{
    m_dwStartTime = dwTime;
}

DWORD CAnmAction::GetStartTime() const
{
    return m_dwStartTime;
}

CAnmAction::~CAnmAction()
{
}
