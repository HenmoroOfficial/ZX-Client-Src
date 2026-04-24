#include "stdafx.h"
#include "AnmHook.h"
#include "Animator.h"

CAnmHook::CAnmHook(const unsigned int uiID)
    : CAnmAction(uiID)
{
    m_iTotalFrames = 1;
    m_sHangerName = _AL("");
    m_sHookName = _AL("");
    m_uiHookObjectID = 0;
    m_sElseHookName = _AL("");
    m_bStartAction = false;
}

void CAnmHook::SetTotalFrames(const int iFrame)
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

int CAnmHook::GetTotalFrames() const
{
    return m_iTotalFrames;
}

void CAnmHook::SetHangerName(const ACString &name)
{
    m_sHangerName = name;
}

ACString CAnmHook::GetHangerName() const
{
    return m_sHangerName;
}

void CAnmHook::SetHookName(const ACString &name)
{
    m_sHookName = name;
}

ACString CAnmHook::GetHookName() const
{
    return m_sHookName;
}

void CAnmHook::SetHookObjectID(const unsigned int uiID)
{
    m_uiHookObjectID = uiID;
    m_sElseHookName = _AL("");
}

unsigned int CAnmHook::GetHookObjectID() const
{
    return m_uiHookObjectID;
}

void CAnmHook::SetElseHookName(const ACString &name)
{
    m_sElseHookName = name;
}

ACString CAnmHook::GetElseHookName() const
{
    return m_sElseHookName;
}

CAnmAction::ActionType CAnmHook::GetActionType() const
{
    return CAnmAction::TYPE_HOOK;
}

void CAnmHook::CopyDataFrom(CAnmAction *pAction)
{
    CAnmHook *pDataAction = dynamic_cast<CAnmHook*>(pAction);
    if (pDataAction)
    {
        SetHangerName(pDataAction->GetHangerName());
        SetHookName(pDataAction->GetHookName());
        SetHookObjectID(pDataAction->GetHookObjectID());
        SetElseHookName(pDataAction->GetElseHookName());
        SetTotalFrames(pDataAction->GetTotalFrames());
    }
}

bool CAnmHook::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    if (IsDead())
    {
        return false;
    }
    
    DWORD dT = dwNowTime - m_dwStartTime;
    DWORD dwTotalTime = GetTotalTime();
    CAnmObject *pObject = pObjectManager->GetObject(m_uiObjectID);
    CAnmObject *pElseObject = pObjectManager->GetObject(m_uiHookObjectID);
    if (pObject && pElseObject)
    {
        if (pObject->GetObjectType() == CAnmObject::TYPE_NPC &&
            pElseObject->GetObjectType() == CAnmObject::TYPE_NPC)
        {
            CAnmNPC *pNPC = dynamic_cast<CAnmNPC*>(pObject);
            if (dwNowTime >= m_dwStartTime)
            {
                if (!m_bStartAction)
                {
                    pNPC->AddChildModel(m_sHangerName, m_sHookName, pElseObject, m_sElseHookName);
                    CAnmNPC *pElseNPC = dynamic_cast<CAnmNPC*>(pElseObject);
                    pElseNPC->SetHooked(true);
                    m_bStartAction = true;
                }

                if (dT >= dwTotalTime)
                {
                    pNPC->RemoveChildModel(m_sHangerName);
                    CAnmNPC *pElseNPC = dynamic_cast<CAnmNPC*>(pElseObject);
                    pElseNPC->SetHooked(false);
                }
            }
        }
        else if (pObject->GetObjectType() == CAnmObject::TYPE_NPC &&
                 pElseObject->GetObjectType() == CAnmObject::TYPE_GFX)
        {
            CAnmNPC *pNPC = dynamic_cast<CAnmNPC*>(pObject);
            CAnmGFX *pGFX = dynamic_cast<CAnmGFX*>(pElseObject);
            if (dwNowTime >= m_dwStartTime)
            {
                if (!m_bStartAction)
                {
                    pNPC->PlayGFX(m_sHookName, pGFX);
                    m_bStartAction = true;
                }
                
                if (dT >= dwTotalTime)
                {
                    pNPC->RemoveGFX(m_sHookName, pGFX);
 
                }
            }
        }
    }

    if (dT >= dwTotalTime)
    {
        m_bDead = true;
    }
    return IsDead();
}

DWORD CAnmHook::GetTotalTime() const
{
    return TIME_PER_FRAME * m_iTotalFrames;
}

void CAnmHook::Reset()
{
    CAnmAction::Reset();
    m_bStartAction = false;
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetObject(m_uiObjectID);
    CAnmObject *pElseObject = g_Animator.GetObjectManager()->GetObject(m_uiHookObjectID);
    if (pObject && pElseObject)
    {
        if (pObject->GetObjectType() == CAnmObject::TYPE_NPC &&
            pElseObject->GetObjectType() == CAnmObject::TYPE_NPC)
        {
            CAnmNPC *pNPC = dynamic_cast<CAnmNPC*>(pObject);
            pNPC->RemoveChildModel(m_sHangerName);
            CAnmNPC *pElseNPC = dynamic_cast<CAnmNPC*>(pElseObject);
            pElseNPC->SetHooked(false);
        }
        else if (pObject->GetObjectType() == CAnmObject::TYPE_NPC &&
                 pElseObject->GetObjectType() == CAnmObject::TYPE_GFX)
        {
            CAnmNPC *pNPC = dynamic_cast<CAnmNPC*>(pObject);
            CAnmGFX *pGFX = dynamic_cast<CAnmGFX*>(pElseObject);
            pNPC->RemoveGFX(m_sHookName, pGFX);
        }
    }
}

CAnmHook::~CAnmHook()
{
    Reset();
}
