#include "stdafx.h"
#include "Animator.h"
#include "AnmCameraFilter.h"

CAnmCameraFilter::CAnmCameraFilter(const unsigned int uiID)
    : CAnmAction(uiID)
{
    m_uiColor = 0xFF000000;
    m_uiFilterType = (unsigned int)FILTER_TYPE_SOLID;
    m_fTransparent = 1.0f;
    m_iTotalFrames = 1;
	m_bFirstTick = true;
}

void CAnmCameraFilter::SetColor(const unsigned int color)
{
    m_uiColor = color;
}
        
unsigned int CAnmCameraFilter::GetColor() const
{
    return m_uiColor;
}

void CAnmCameraFilter::SetFilterType(const unsigned int t)
{
    m_uiFilterType = t;
}

unsigned int CAnmCameraFilter::GetFilterType() const
{
    return m_uiFilterType;
}

void CAnmCameraFilter::SetTransparent(const float f)
{
    if (f < 0.0f)
    {
        m_fTransparent = 0.0f;
    }
    else if (f > 1.0f)
    {
        m_fTransparent = 1.0f;
    }
    else
    {
        m_fTransparent = f;
    }
}

float CAnmCameraFilter::GetTransparent() const
{
    return m_fTransparent;
}

void CAnmCameraFilter::SetTotalFrames(const int iFrame)
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

int CAnmCameraFilter::GetTotalFrames() const
{
    return m_iTotalFrames;
}

CAnmAction::ActionType CAnmCameraFilter::GetActionType() const
{
    return CAnmAction::TYPE_CAMERA_FILTER;
}

void CAnmCameraFilter::CopyDataFrom(CAnmAction *pAction)
{
    CAnmCameraFilter *pDataAction = dynamic_cast<CAnmCameraFilter*>(pAction);
    if (pDataAction)
    {
        SetColor(pDataAction->GetColor());
        SetFilterType(pDataAction->GetFilterType());
        SetTransparent(pDataAction->GetTransparent());
        SetTotalFrames(pDataAction->GetTotalFrames());
		m_bFirstTick = true;
    }
}

bool CAnmCameraFilter::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    if (IsDead())
    {
        return false;
    }

    DWORD dT = dwNowTime - m_dwStartTime;
    DWORD dwTotalTime = GetTotalTime();
    if (dwNowTime >= m_dwStartTime)
    {
        if (dT < dwTotalTime)
        {
            float p = (float)dT / dwTotalTime;
            switch (m_uiFilterType)
            {
                case FILTER_TYPE_SOLID:
                {
                    unsigned int c = unsigned int(255.0f * m_fTransparent);
                    c = (c << 24) | 0xFFFFFF;
                    g_Animator.SetMaskColor(c & m_uiColor);
                    break;
                }
                case FILTER_TYPE_TWEENING_SOLID:
                {
                    unsigned int c = unsigned int(255.0f * m_fTransparent * p);
                    c = (c << 24) | 0xFFFFFF;
                    g_Animator.SetMaskColor(c & m_uiColor);
                    break;
                }
                case FILTER_TYPE_TWEENING_TRANSPARENT:
                {
                    unsigned int c = unsigned int(255.0f * m_fTransparent * (1.0f - p));
                    c = (c << 24) | 0xFFFFFF;
                    g_Animator.SetMaskColor(c & m_uiColor);
                    break;
                }
                case FILTER_TYPE_SLOW_BLACK_WHITE:
                {
                    g_Animator.SetBlackWhiteType(CAnimator::BWT_SLOW);
                    g_Animator.SetMonoColor(m_uiColor);
                    break;
                }
                case FILTER_TYPE_FAST_BLACK_WHITE:
                {
                    g_Animator.SetBlackWhiteType(CAnimator::BWT_FAST);
                    g_Animator.SetMonoColor(m_uiColor);
                    break;
                }
                case FILTER_TYPE_MOTION_BLUR:
                {
                    g_Animator.SetMotionBlurLevel(m_fTransparent);
                    break;
                }
				case FILTER_TYPE_TWEENING_FRAME:
				{
					unsigned int c = unsigned int(255.0f * m_fTransparent * (1.0f - p));
                    c = (c << 24) | 0xFFFFFF;
                    g_Animator.SetMaskColor(c & m_uiColor);
					g_Animator.SetBlendFrame(true);
					if(m_bFirstTick)
					{
					  g_Animator.GeneratePreFrame();
					  m_bFirstTick = false;
					}
					break;
				}
                default:
                    break;
            }
        }
    }

    if (dT >= dwTotalTime)
    {
        m_bDead = true;
    }
    return IsDead();
}

DWORD CAnmCameraFilter::GetTotalTime() const
{
    return TIME_PER_FRAME * m_iTotalFrames;
}

CAnmCameraFilter::~CAnmCameraFilter()
{
}
void CAnmCameraFilter::Reset()
{
  m_bFirstTick = true;
  CAnmAction::Reset();
}


