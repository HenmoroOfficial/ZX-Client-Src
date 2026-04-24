#include "stdafx.h"
#include "Animator.h"
#include "AnmBezierManager.h"
#include "AnmBezierMove.h"

CAnmBezierMove::CAnmBezierMove(const unsigned int uiID)
    : CAnmAction(uiID)
{
    m_uiBezierID = -1;
    m_bCloseTerrain = false;
}

void CAnmBezierMove::SetBezierID(const unsigned int uiID)
{
    m_uiBezierID = uiID;
}

unsigned int CAnmBezierMove::GetBezierID() const
{
    return m_uiBezierID;
}

void CAnmBezierMove::SetCloseTerrain(const bool b)
{
    m_bCloseTerrain = b;
}

bool CAnmBezierMove::GetCloseTerrain() const
{
    return m_bCloseTerrain;
}

CAnmAction::ActionType CAnmBezierMove::GetActionType() const
{
    return CAnmAction::TYPE_BEZIER_MOVE;
}

void CAnmBezierMove::CopyDataFrom(CAnmAction *pAction)
{
    CAnmBezierMove *pDataAction = dynamic_cast<CAnmBezierMove*>(pAction);
    if (pDataAction)
    {
        SetBezierID(pDataAction->GetBezierID());
        SetCloseTerrain(pDataAction->GetCloseTerrain());
    }
}

bool CAnmBezierMove::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    if (IsDead())
    {
        return false;
    }

    DWORD dT = dwNowTime - m_dwStartTime;
    DWORD dwTotalTime = GetTotalTime();
    CAnmObject *pObject = pObjectManager->GetObject(m_uiObjectID);
    CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetBezier(m_uiBezierID);
    if (pObject && pBezier)
    {
        if (dwNowTime >= m_dwStartTime)
        {
            if (dT < dwTotalTime)
            {
                A3DVECTOR3 vPos;
                A3DQUATERNION qOri;
                float fFOV;
                pBezier->GetSlerp(dT, vPos, qOri, fFOV);

                if (m_bCloseTerrain)
                {
                    vPos.y = g_Animator.GetPosHeight(vPos);
                }
                pObject->SetPos(vPos);

                A3DVECTOR3 vDir(0.0f, 0.0f, -1.0f);
                A3DVECTOR3 vUp(0.0f, 1.0f, 0.0f);
                vDir = qOri ^ vDir;
                vUp = qOri ^ vUp;
                pObject->SetDirAndUp(vDir, vUp);

                if (pObject->GetObjectType() == CAnmObject::TYPE_CAMERA)
                {
                    dynamic_cast<CAnmCamera*>(pObject)->SetFOV(fFOV);
                }
            }
            else
            {
                dT -= dT - dwTotalTime;
                A3DVECTOR3 vPos;
                A3DQUATERNION qOri;
                float fFOV;
                pBezier->GetSlerp(dT, vPos, qOri, fFOV);
                
                if (m_bCloseTerrain)
                {
                    vPos.y = g_Animator.GetPosHeight(vPos);
                }
                pObject->SetPos(vPos);
                
                A3DVECTOR3 vDir(0.0f, 0.0f, -1.0f);
                A3DVECTOR3 vUp(0.0f, 1.0f, 0.0f);
                vDir = qOri ^ vDir;
                vUp = qOri ^ vUp;
                pObject->SetDirAndUp(vDir, vUp);
                
                if (pObject->GetObjectType() == CAnmObject::TYPE_CAMERA)
                {
                    dynamic_cast<CAnmCamera*>(pObject)->SetFOV(fFOV);
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

DWORD CAnmBezierMove::GetTotalTime() const
{
    CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetBezier(m_uiBezierID);
    if (pBezier)
    {
        return pBezier->GetTotalTime();
    }
    return 0;
}

CAnmBezierMove::~CAnmBezierMove()
{
}
