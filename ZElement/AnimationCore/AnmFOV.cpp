#include "Animator.h"
#include "AnmFOV.h"
#include "AnmCamera.h"
#include "AnmObjectManager.h"

CAnmFOV::CAnmFOV(const unsigned int uiID)
    : CAnmAction(uiID)
{
    m_fStartFOV = 1.0f;
    m_fEndFOV = 1.0f;
    m_iTotalFrames = 1;
}

CAnmFOV::~CAnmFOV()
{
}

void CAnmFOV::SetStartFOV(const float fov)
{
    m_fStartFOV = fov;
}

float CAnmFOV::GetStartFOV() const
{
    return m_fStartFOV;
}

void CAnmFOV::SetEndFOV(const float fov)
{
    m_fEndFOV = fov;
}

float CAnmFOV::GetEndFOV() const
{
    return m_fEndFOV;
}

void CAnmFOV::SetTotalFrames(const int iFrame)
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

int CAnmFOV::GetTotalFrames() const
{
    return m_iTotalFrames;
}

CAnmAction::ActionType CAnmFOV::GetActionType() const
{
    return CAnmAction::TYPE_FOV;
}

void CAnmFOV::CopyDataFrom(CAnmAction *pAction)
{
    CAnmFOV *pDataAction = dynamic_cast<CAnmFOV*>(pAction);
    if (pDataAction)
    {
        SetStartFOV(pDataAction->GetStartFOV());
        SetEndFOV(pDataAction->GetEndFOV());
        SetTotalFrames(pDataAction->GetTotalFrames());
    }
}

bool CAnmFOV::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    if (IsDead())
    {
        return false;
    }
    DWORD dT = dwNowTime - m_dwStartTime;
    DWORD dwTotalTime = GetTotalTime();
    CAnmObject *pObject = pObjectManager->GetObject(m_uiObjectID);
    if (pObject && pObject->GetObjectType() == CAnmObject::TYPE_CAMERA)
    {
        CAnmCamera *pCamera = dynamic_cast<CAnmCamera*>(pObject);
        {
            if (dwNowTime >= m_dwStartTime)
            {
                // Set FOV
                if (dT < dwTotalTime)
                {
                    float fFOV = m_fStartFOV + (m_fEndFOV - m_fStartFOV) * dT / dwTotalTime;
                    pCamera->SetFOV(fFOV);
                }
                else
                {
                    pCamera->SetFOV(m_fEndFOV);
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

DWORD CAnmFOV::GetTotalTime() const
{
    return TIME_PER_FRAME * m_iTotalFrames;
}