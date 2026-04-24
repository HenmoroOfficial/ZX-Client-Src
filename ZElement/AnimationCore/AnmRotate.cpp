#include "Animator.h"
#include "AnmRotate.h"
#include "AnmObjectManager.h"

CAnmRotate::CAnmRotate(const unsigned int uiID)
    : CAnmAction(uiID)
{
    m_qStartRotate = A3DQUATERNION(1.0f, 0.0f, 0.0f, 0.0f);
    m_qEndRotate = A3DQUATERNION(1.0f, 0.0f, 0.0f, 0.0f);
    m_iTotalFrames = 1;
}

CAnmRotate::~CAnmRotate()
{
}

void CAnmRotate::SetStartRotate(const A3DQUATERNION &q)
{
    m_qStartRotate = q;
}

A3DQUATERNION CAnmRotate::GetStartRotate() const
{
    return m_qStartRotate;
}

void CAnmRotate::SetEndRotate(const A3DQUATERNION &q)
{
    m_qEndRotate = q;
}

A3DQUATERNION CAnmRotate::GetEndRotate() const
{
    return m_qEndRotate;
}

void CAnmRotate::SetTotalFrames(const int iFrame)
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

int CAnmRotate::GetTotalFrames() const
{
    return m_iTotalFrames;
}

CAnmAction::ActionType CAnmRotate::GetActionType() const
{
    return CAnmAction::TYPE_ROTATE;
}

void CAnmRotate::CopyDataFrom(CAnmAction *pAction)
{
    CAnmRotate *pDataAction = dynamic_cast<CAnmRotate*>(pAction);
    if (pDataAction)
    {
        SetStartRotate(pDataAction->GetStartRotate());
        SetEndRotate(pDataAction->GetEndRotate());
        SetTotalFrames(pDataAction->GetTotalFrames());
    }
}

bool CAnmRotate::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
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
            // Set Rotate
            A3DVECTOR3 vDir(0.0f, 0.0f, -1.0f);
            A3DVECTOR3 vUp(0.0f, 1.0f, 0.0f);
            A3DQUATERNION qOri = m_qEndRotate;
            if (dT < dwTotalTime)
            {
                qOri = SLERPQuad(m_qStartRotate , m_qEndRotate, float(dT) / dwTotalTime);
            }
            vDir = qOri ^ vDir;
            vUp = qOri ^ vUp;
            pObject->SetDirAndUp(vDir, vUp);
        }
    }

    if (dT >= dwTotalTime)
    {
        m_bDead = true;
    }
    return IsDead();
}

DWORD CAnmRotate::GetTotalTime() const
{
    return TIME_PER_FRAME * m_iTotalFrames;
}