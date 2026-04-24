#include "stdafx.h"
#include "Animator.h"
#include "AnmCameraOscillatory.h"
#include "AUTOCA/Utilities.h"

CAnmCameraOscillatory::CAnmCameraOscillatory(const unsigned int uiID)
    : CAnmAction(uiID)
{
    m_uiOscillatoryType = (unsigned int)OSCILLATORY_TYPE_DIRECTION;
    m_iOscillatoryAxis = OSCILLATORY_AXIS_Y;
    m_iAttenuation = OSCILLATORY_ATTENUATION_DOWN;
    m_fDisplacement = 1.0f;
    m_fFrequency = 20.0f;
    m_iTotalFrames = 1;
}

void CAnmCameraOscillatory::SetOscillatoryType(const unsigned int t)
{
    m_uiOscillatoryType = t;
}

unsigned int CAnmCameraOscillatory::GetOscillatoryType() const
{
    return m_uiOscillatoryType;
}

void CAnmCameraOscillatory::SetOscillatoryAxis(const int a)
{
    if (a >= OSCILLATORY_AXIS_X && a <= OSCILLATORY_AXIS_Z)
    {
        m_iOscillatoryAxis = a;
    }
}

int CAnmCameraOscillatory::GetOscillatoryAxis() const
{
    return m_iOscillatoryAxis;
}

void CAnmCameraOscillatory::SetDisplacement(const float f)
{
    m_fDisplacement = f;
}

float CAnmCameraOscillatory::GetDisplacement() const
{
    return m_fDisplacement;
}

void CAnmCameraOscillatory::SetFrequency(const float f)
{
    m_fFrequency = f;
}

float CAnmCameraOscillatory::GetFrequency() const
{
    return m_fFrequency;
}

void CAnmCameraOscillatory::SetAttenuation(const int a)
{
    if (a >= OSCILLATORY_ATTENUATION_NONE && a <= OSCILLATORY_ATTENUATION_DOWN)
    {
        m_iAttenuation = a;
    }
}

int CAnmCameraOscillatory::GetAttenuation() const
{
    return m_iAttenuation;
}

void CAnmCameraOscillatory::SetTotalFrames(const int iFrame)
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

int CAnmCameraOscillatory::GetTotalFrames() const
{
    return m_iTotalFrames;
}

CAnmAction::ActionType CAnmCameraOscillatory::GetActionType() const
{
    return CAnmAction::TYPE_CAMERA_OSCILLATORY;
}

void CAnmCameraOscillatory::CopyDataFrom(CAnmAction *pAction)
{
    CAnmCameraOscillatory *pDataAction = dynamic_cast<CAnmCameraOscillatory*>(pAction);
    if (pDataAction)
    {
        SetOscillatoryType(pDataAction->GetOscillatoryType());
        SetOscillatoryAxis(pDataAction->GetOscillatoryAxis());
        SetAttenuation(pDataAction->GetAttenuation());
        SetDisplacement(pDataAction->GetDisplacement());
        SetFrequency(pDataAction->GetFrequency());
        SetTotalFrames(pDataAction->GetTotalFrames());
    }
}

bool CAnmCameraOscillatory::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
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
        if (dwNowTime >= m_dwStartTime)
        {
            if (dT < dwTotalTime)
            {
                float p = (float)dT / dwTotalTime;
                switch (m_uiOscillatoryType)
                {
                    case OSCILLATORY_TYPE_DIRECTION:
                    {
                        A3DVECTOR3 vOffset = pCamera->GetUp();
                        if (m_iOscillatoryAxis == OSCILLATORY_AXIS_X)
                        {
                            vOffset = pCamera->GetLeft();
                        }
                        else if (m_iOscillatoryAxis == OSCILLATORY_AXIS_Z)
                        {
                            vOffset = pCamera->GetDir();
                        }
                        vOffset *= m_fDisplacement;
                        float f = sin(m_fFrequency * float(dT) * PI * 0.002f);
                        if (m_iAttenuation == OSCILLATORY_ATTENUATION_DOWN)
                        {
                            f *= (1.0f - p);
                        }
                        else if (m_iAttenuation == OSCILLATORY_ATTENUATION_UP)
                        {
                            f *= p;
                        }
                        pCamera->SetCameraOffset(f * vOffset);
                        break;
                    }
                    case OSCILLATORY_TYPE_RANDOM:
                    {
                        A3DVECTOR3 vLeft = randomUnitVector();
                        A3DVECTOR3 vOffset = m_fDisplacement * vLeft;
                        float f = sin(m_fFrequency * float(dT) * PI * 0.002f);
                        if (m_iAttenuation == OSCILLATORY_ATTENUATION_DOWN)
                        {
                            f *= (1.0f - p);
                        }
                        else if (m_iAttenuation == OSCILLATORY_ATTENUATION_UP)
                        {
                            f *= p;
                        }
                        pCamera->SetCameraOffset(f * vOffset);
                        break;
                    }
                    default:
                        break;
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

DWORD CAnmCameraOscillatory::GetTotalTime() const
{
    return TIME_PER_FRAME * m_iTotalFrames;
}

CAnmCameraOscillatory::~CAnmCameraOscillatory()
{
}


