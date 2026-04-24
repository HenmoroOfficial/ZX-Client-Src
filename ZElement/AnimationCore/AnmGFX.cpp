#include "stdafx.h"
#include "EC_GFXCaster.h"
#include "AnmGFX.h"
#include "Animator.h"

CAnmGFX::CAnmGFX(unsigned int uiID)
    : CAnmObject(uiID)
{
    m_pA3DGFX = 0;
    m_sGFXName = _AL("");
    m_AABB.Extents = A3DVECTOR3(0.5f);
    m_fSpeed = 1.0f;
    m_fScale = 1.0f;
    m_fTransparent = 1.0f;
    m_fTickTime = 0.0f;
}

CAnmObject::ObjectType CAnmGFX::GetObjectType() const
{
    return CAnmObject::TYPE_GFX;
}

bool CAnmGFX::Load(const char *szFile)
{
    return CAnmObject::Load(szFile) && g_Animator.LoadObject(this);
}

ACString CAnmGFX::GetName() const
{
    return m_sGFXName;
}

AString CAnmGFX::GetGFXPath() const
{
    if (m_sGFXName.GetLength() > 4)
    {
        return WC2AS(m_sGFXName.Right(m_sGFXName.GetLength() - 4));
    }
    return AString("");
}

void CAnmGFX::GetActionList(std::vector<ACString> &vList) const
{
    vList.clear();
}

bool CAnmGFX::Tick(const DWORD dwDeltaTime)
{
    if (m_pA3DGFX)
    {
        m_fTickTime += m_fSpeed * float(dwDeltaTime);
        DWORD tickTime = DWORD(m_fTickTime);
        m_fTickTime -= tickTime;
        return g_Animator.TickObject( this, tickTime );
    }
    return true;
}

bool CAnmGFX::Render(A3DViewport *viewport, bool bRenderShadow)
{
    if( ! m_pA3DGFX )
		return false;
	if( ! IsVisible() )
		return true;
	if( m_pA3DGFX->GetState() == ST_STOP )
		return true;
#ifdef BACK_VERSION
	return g_Animator.RenderObject( this, bRenderShadow );
#else
	return g_Animator.RenderObject( this, viewport, bRenderShadow );
#endif
}

void CAnmGFX::PlayAction(const ACString &actionName)
{
    if (m_pA3DGFX)
    {
        m_pA3DGFX->Stop();
        m_pA3DGFX->Reset();
        m_pA3DGFX->Start(true);
    }
}

void CAnmGFX::StopAction()
{
    m_fSpeed = 1.0f;
    m_fTickTime = 0.0f;
    if (m_pA3DGFX)
    {
        m_pA3DGFX->Stop();
    }
}
void CAnmGFX::PauseAction()
{
    if(m_pA3DGFX)
	{
	  m_pA3DGFX->Pause();
	}
}
DWORD CAnmGFX::GetActionTimeSpan() const
{
	DWORD time = 0;
	if(m_pA3DGFX && !m_pA3DGFX->IsInfinite())
	{
		int elementCount = m_pA3DGFX->GetElementCount();
		for (int i=0;i<elementCount;++i)
		{
			DWORD  temp = 0;
			A3DGFXElement* pElement = m_pA3DGFX->GetElement(i);
			int keypointCount = pElement->GetKeyPointCount();
			for (int j=0;j<keypointCount;++j)
			{
				A3DGFXKeyPoint* pKeyPoint = pElement->GetKeyPoint(j);
				temp+=pKeyPoint->GetTimeSpan();
			}
			if(temp > time)
				time = temp;
		}
	}
    return time;
}
bool CAnmGFX::IsInAction() const
{
    if (m_pA3DGFX)
    {
        if (m_pA3DGFX->GetState() == ST_STOP)
        {
            return false;
        }
        return true;
    }
    return false;
}

void CAnmGFX::Reset()
{
    m_fSpeed = 1.0f;
    m_fTickTime = 0.0f;
    if (m_pA3DGFX)
    {
        m_pA3DGFX->Stop();
        m_pA3DGFX->Reset();
    }
    SetVisible(true);
    SetScale(1.0f);
    SetTransparent(1.0f);
}

void CAnmGFX::SetActionSpeed(const float speed)
{
    m_fSpeed = speed;
}

float CAnmGFX::GetActionSpeed() const
{
    return m_fSpeed;
}

void CAnmGFX::SetPos(const A3DVECTOR3 &vPos)
{
    if (m_pA3DGFX)
    {
        m_pA3DGFX->SetPos(vPos - (m_qOrientation ^ m_vOffset));
    }
}

A3DVECTOR3 CAnmGFX::GetPos() const
{
    if (m_pA3DGFX)
    {
        return m_pA3DGFX->GetPos() + (m_qOrientation ^ m_vOffset);
    }
    return A3DVECTOR3(0.0f);
}

void CAnmGFX::SetDirAndUp(const A3DVECTOR3 &vDir, const A3DVECTOR3 &vUp)
{
    if (m_pA3DGFX)
    {
        A3DVECTOR3 v = vDir;
        v.Normalize();
        A3DVECTOR3 u = vUp;
        u.Normalize();

        A3DVECTOR3 pos = GetPos();
        
        m_pA3DGFX->SetDirAndUp(v, u);
        m_qOrientation = m_pA3DGFX->GetDir();
        
        SetPos(pos);
    }
}

A3DVECTOR3 CAnmGFX::GetDir() const
{
    if (m_pA3DGFX)
    {
        return m_pA3DGFX->GetDir() ^ A3DVECTOR3(0.0f, 0.0f, 1.0f);
    }
    return A3DVECTOR3(0.0f, 0.0f, 1.0f);
}

A3DVECTOR3 CAnmGFX::GetUp() const
{
    if (m_pA3DGFX)
    {
        return m_pA3DGFX->GetDir() ^ A3DVECTOR3(0.0f, 1.0f, 0.0f);
    }
    return A3DVECTOR3(0.0f, 1.0f, 0.0f);
}

void CAnmGFX::SetScale(const float s)
{
    m_fScale = s;
}

float CAnmGFX::GetScale() const
{
    return m_fScale;
}

void CAnmGFX::SetTransparent(const float t)
{
    m_fTransparent = t;
}

float CAnmGFX::GetTransparent() const
{
    return m_fTransparent;
}

const A3DAABB& CAnmGFX::GetAABB() const
{
    m_AABB.Center = GetPos();
    m_AABB.Center.y += 0.5f;
    m_AABB.CompleteMinsMaxs();
    return m_AABB;
}

void CAnmGFX::CopyDataFrom(CAnmObject *pObject)
{
    if (pObject)
    {
        CAnmGFX *pGFX = dynamic_cast<CAnmGFX*>(pObject);
        if (pGFX)
        {
            CAnmObject::CopyDataFrom(pObject);
            Load(pGFX->GetLoadFile());
            SetPos(pGFX->GetPos());
            SetDirAndUp(pGFX->GetDir(), pGFX->GetUp());
            SetScale(pGFX->GetScale());
        }
    }
}

CAnmGFX::~CAnmGFX()
{
	g_Animator.UnloadObject(this);
}

