#include "stdafx.h"
#include "Animator.h"
#include "AnmCameraBind.h"

CAnmCameraBind::CAnmCameraBind(const unsigned int uiID)
               :CAnmAction(uiID),
			   m_iFllowObj(-1),
			   m_iTotalFrames(1),
			   m_vDistance(0,0,0)
{
  
}
CAnmCameraBind::~CAnmCameraBind()
{
 
}
void CAnmCameraBind::SetFllowObj(const unsigned int iObj)
{
   m_iFllowObj = iObj;
}
unsigned int CAnmCameraBind::GetFllowObj() const
{
   return m_iFllowObj;
}
void CAnmCameraBind::SetDistance(const A3DVECTOR3& vDis)
{
   m_vDistance = vDis;
}
A3DVECTOR3 CAnmCameraBind::GetDistance() const
{
  return m_vDistance;
}
void CAnmCameraBind::SetTotalFrames(const int iFrame)
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

int CAnmCameraBind::GetTotalFrames() const
{
    return m_iTotalFrames;
}
CAnmAction::ActionType CAnmCameraBind::GetActionType() const
{
	return CAnmAction::TYPE_CAMERA_BIND;
}

void CAnmCameraBind::CopyDataFrom(CAnmAction *pAction)
{
	CAnmCameraBind *pDataAction = dynamic_cast<CAnmCameraBind*>(pAction);
    if (pDataAction)
    {
		SetFllowObj(pDataAction->GetFllowObj());
		SetTotalFrames(pDataAction->GetTotalFrames());
		SetDistance(pDataAction->GetDistance());
    }
}

bool CAnmCameraBind::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
	if (IsDead())
    {
        return false;
    }
	
    DWORD dT = dwNowTime - m_dwStartTime;
    DWORD dwTotalTime = GetTotalTime();
    if (dwNowTime >= m_dwStartTime)
    {
        //if (dT < dwTotalTime)
        {
            CAnmObject* pCamera = g_Animator.GetObjectManager()->GetObject(0);
			CAnmObject* pFllow  = g_Animator.GetObjectManager()->GetObject(m_iFllowObj);
			if(pCamera && pFllow)
			{
				A3DVECTOR3 vDir = pFllow->GetDir();
				A3DVECTOR3 vUp  = pFllow->GetUp();
				A3DVECTOR3 vRight;
				vRight.CrossProduct(vUp,vDir);
				vRight.Normalize();
				A3DVECTOR3 vDis;
	            vDis = m_vDistance.x*vRight + m_vDistance.y*vUp + m_vDistance.z*vDir;
				//pCamera->SetPos(pFllow->GetPos()+vDis);
				//pCamera->SetDirAndUp(-vDis);
				A3DVECTOR3 vPos = pFllow->GetPos() +vDis;
				A3DVECTOR3 vCam = pCamera->GetPos();
                pCamera->SetDirAndUp(vPos - vCam);
			}
        }
    }
	
    if (dT >= dwTotalTime)
    {
        m_bDead = true;
    }
    return IsDead();
}
DWORD CAnmCameraBind::GetTotalTime() const
{
  return TIME_PER_FRAME * m_iTotalFrames;
}

