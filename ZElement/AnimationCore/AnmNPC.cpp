#include "stdafx.h"
#include "EC_ShadowRender.h"
#include "EC_Resource.h"
#include "EC_Utility.h"
#include "AnmNPC.h"
#include "AnmGFX.h"
#include "Animator.h"
#include "AnmCommonActionManager.h"

//	NPC's render for shadow call back function
#ifdef BACK_VERSION
bool AnmNPCRenderForShadow(A3DViewport * pViewport, void * pArg)
{
    A3DSkinModel * pModel = (A3DSkinModel *) pArg;
    
    if( !pModel )
        return false;
    
    g_Animator.GetA3DDevice()->SetZTestEnable(true);
    g_Animator.GetA3DDevice()->SetZWriteEnable(true);
    g_Animator.GetA3DDevice()->SetAlphaTestEnable(true);
    g_Animator.GetA3DDevice()->SetAlphaBlendEnable(false);
    g_Animator.GetA3DDevice()->SetAlphaFunction(A3DCMP_GREATEREQUAL);
    g_Animator.GetA3DDevice()->SetAlphaRef(84);
    
    pModel->RenderAtOnce(pViewport, A3DSkinModel::RAO_NOMATERIAL, false);
    
    g_Animator.GetA3DDevice()->SetAlphaBlendEnable(true);
    g_Animator.GetA3DDevice()->SetAlphaTestEnable(false);
    g_Animator.GetA3DDevice()->SetZTestEnable(true);
    g_Animator.GetA3DDevice()->SetZWriteEnable(true);
    return true;
}
#endif

CAnmNPC::CAnmNPC(unsigned int uiID)
    : CAnmObject(uiID)
{
    m_pModel = 0;
    m_pSkin = 0;
    m_sModelName = _AL("");
    m_sActionName = _AL("");
    m_AABB.Extents = A3DVECTOR3(0.5f);
    m_bHooked = false;
    m_fSpeed = 1.0f;
    m_fTickTime = 0.0f;
	m_eActState = ESTATE_ACT_STOP;
}

CAnmObject::ObjectType CAnmNPC::GetObjectType() const
{
    return CAnmObject::TYPE_NPC;
}

bool CAnmNPC::Load(const char *szFile)
{
    return CAnmObject::Load(szFile) && g_Animator.LoadObject(this);
}

ACString CAnmNPC::GetName() const
{
    return m_sModelName;
}

void CAnmNPC::GetActionList(std::vector<ACString> &vList) const
{
    vList.clear();
    if (m_pModel)
    {
        int n = m_pModel->GetComActCount();
        for (int i=0; i<n; ++i)
        {
            vList.push_back(AS2WC((m_pModel->GetComActByIndex(i))->GetName()));
        }
    }
}

void CAnmNPC::GetHookList(std::vector<ACString> &vList) const
{
    vList.clear();
    if (m_pModel)
    {
        A3DSkeleton *pSkeleton = m_pModel->GetA3DSkinModel()->GetSkeleton();
        int n = pSkeleton->GetHookNum();
        for (int i=0; i<n; ++i)
        {
            vList.push_back(AS2WC((pSkeleton->GetHook(i))->GetName()));
        }
    }
}

void CAnmNPC::AddChildModel(const ACString &hangerName,
                            const ACString &hookName,
                            CAnmObject *pObject,
                            const ACString &elseHookName)
{
    if (m_pModel && pObject)
    {
        if (GetObjectID() != pObject->GetObjectID() &&
            pObject->GetObjectType() == CAnmObject::TYPE_NPC)
        {
            CAnmNPC *pNPC = dynamic_cast<CAnmNPC*>(pObject);
            if (pNPC->m_pModel && !pNPC->IsHooked())
            {
                m_pModel->AddChildModel(WC2AS(hangerName), false, WC2AS(hookName), pNPC->m_pModel, WC2AS(elseHookName));
            }
        }
    }
}

void CAnmNPC::RemoveChildModel(const ACString &hangerName)
{
    if (m_pModel)
    {
        CECModel *pModel = m_pModel->GetChildModel(WC2AS(hangerName));
        if (pModel)
        {
            m_pModel->RemoveChildModel(WC2AS(hangerName), false);
        }
    }
}

void CAnmNPC::PlayGFX(const ACString &hookName, CAnmGFX *pGFX)
{
    if (m_pModel && pGFX)
    {
        m_pModel->PlayGfx(pGFX->GetGFXPath(),
                          WC2AS(hookName), pGFX->GetScale());
    }
}

void CAnmNPC::RemoveGFX(const ACString &hookName, CAnmGFX *pGFX)
{
    if (m_pModel && pGFX)
    {
        m_pModel->RemoveGfx(pGFX->GetGFXPath(), WC2AS(hookName));
    }
}

void CAnmNPC::SetHooked(const bool hooked)
{
    m_bHooked = hooked;
}

bool CAnmNPC::IsHooked() const
{
    return m_bHooked;
}

void CAnmNPC::SetActionSpeed(const float speed)
{
    m_fSpeed = speed;
}

float CAnmNPC::GetActionSpeed() const
{
    return m_fSpeed;
}

void CAnmNPC::UseShader(const int iShader)
{
   
}

bool CAnmNPC::Tick(const DWORD dwDeltaTime)
{
    if (m_pModel)
    {
        if (!m_bHooked)
        {
			if(m_eActState == ESTATE_ACT_PLAY)
			{
				m_fTickTime += m_fSpeed * float(dwDeltaTime);

				if(m_fSpeed < 0.0f)
				{
					DWORD time = GetActionTimeSpan();
					m_fTickTime = (float)(time) + m_fTickTime;
					if(m_fTickTime < 0.0f)
					 m_fTickTime = 0.0f;
					
				}
				DWORD tickTime = DWORD(m_fTickTime);
				m_fTickTime -= tickTime;
				return g_Animator.TickObject( this, tickTime );


			}
			else
			{
                return g_Animator.TickObject( this, 0 );
			}
        }
    }
    return true;
}

bool CAnmNPC::Render(A3DViewport *viewport, bool bRenderShadow)
{
	if( ! m_pModel )
		return true;
	if( ! IsVisible() )
		return true;
	if( m_bHooked )
		return true;
#ifdef BACK_VERSION
	return g_Animator.RenderObject( this, bRenderShadow );
#else
	return g_Animator.RenderObject( this, viewport, bRenderShadow );
#endif
}

void CAnmNPC::PlayAction(const ACString &actionName)
{
    if (m_pModel && actionName != _AL(""))
    {
        if (m_pModel->IsPlayingComAct())
        {
            if (m_sActionName != actionName)
            {
                StopAction();
                m_sActionName = actionName;
                m_pModel->PlayActionByName(WC2AS(actionName), 1.0f,
                        true, 0, false, 0, true);
            }
        }
        else
        {
            A3DSkeleton *pModel = m_pModel->GetA3DSkinModel()->GetSkeleton();
            m_sActionName = actionName;
            m_pModel->PlayActionByName(WC2AS(actionName), 1.0f,
                    true, 200, true, 0, true);
        }
		m_eActState = ESTATE_ACT_PLAY;
    }
}

void CAnmNPC::PlayAction(const ACString &actionName, const int iBlendTime, const bool bShowGFX)
{
    if (m_pModel && actionName != _AL(""))
    {
        //a_LogOutput(1, "Play action");
		if (GetObjectID() == 1) 
		{
			const ACString* pRealName = g_Animator.GetCommonActionManager()->GetHostPlayerRealAction(actionName);
			if (pRealName)
				m_sActionName = *pRealName;
			else
				m_sActionName = actionName;
		}
        else
		{
			m_sActionName = actionName;
		}
        m_pModel->PlayActionByName(WC2AS(m_sActionName), 1.0f,
            true, iBlendTime, true, 0, !bShowGFX);

		m_eActState = ESTATE_ACT_PLAY;
    }
}

void CAnmNPC::StopAction()
{
    m_fSpeed = 1.0f;
    m_fTickTime = 0.0f;
	m_sActionName = _AL("");
    if (m_pModel)
    {
        //a_LogOutput(1, "Stop action");
        // ´úÌæm_pModel->StopAllActions();
        m_pModel->ResetMaterialScale();
        m_pModel->StopChannelAction(0, true, true);
	    m_pModel->StopChildrenAct();
    }

	m_eActState = ESTATE_ACT_STOP;
}
void CAnmNPC::PauseAction()
{
    m_eActState = ESTATE_ACT_PAUSE;
}
bool CAnmNPC::IsInAction() const
{
    if (m_pModel)
    {
        if (m_pModel->IsPlayingComAct() && !IsActionFinished())
        {
            return true;
        }
    }
    return false;
}
bool CAnmNPC::IsActionFinished() const
{
   if(m_pModel)
   {
       A3DCombActDynData*  pActData = m_pModel->GetCurComAct();
	   if(pActData && (pActData->IsComActInfinite() || !pActData->IsActionFinished()))
		   return false;  
   }
   return true;
}
bool CAnmNPC::IsActionInfinite() const
{
   if(m_pModel)
   {
      A3DCombActDynData* pActData = m_pModel->GetCurComAct();
	  if(pActData)
		return pActData->IsComActInfinite(); 
   }
   return false;
}
DWORD CAnmNPC::GetActionTimeSpan() const
{
  if(m_pModel)
  {
    A3DCombActDynData* pActData = m_pModel->GetCurComAct();
	if(pActData)
	  return pActData->GetComActTimeSpan();
  }
  return 0;
}
void CAnmNPC::Reset()
{
    StopAction();
    SetVisible(true);
    SetScale(1.0f);
    SetTransparent(1.0f);
    UseShader(0);
}

void CAnmNPC::SetPos(const A3DVECTOR3 &vPos)
{
    if (m_pModel)
    {
        m_pModel->SetPos(vPos - (m_qOrientation ^ m_vOffset));
    }
}

A3DVECTOR3 CAnmNPC::GetPos() const
{
    if (m_pModel)
    {
        return m_pModel->GetPos() + (m_qOrientation ^ m_vOffset);
    }
    return A3DVECTOR3(0.0f);
}

void CAnmNPC::SetDirAndUp(const A3DVECTOR3 &vDir, const A3DVECTOR3 &vUp)
{
    if (m_pModel)
    {
        A3DVECTOR3 v = vDir;
        if (vUp == A3DVECTOR3(0.0f, 1.0f, 0.0f))
        {
            v.y = 0.0f;
        }
        v.Normalize();
        A3DVECTOR3 u = vUp;
        u.Normalize();
        m_pModel->SetDirAndUp(v, vUp);

        A3DVECTOR3 pos = GetPos();

        UpdateOrientation();
            
        SetPos(pos);
    }
}

A3DVECTOR3 CAnmNPC::GetDir() const
{
    if (m_pModel)
    {
        return m_pModel->GetDir();
    }
    return A3DVECTOR3(0.0f, 0.0f, -1.0f);
}

A3DVECTOR3 CAnmNPC::GetUp() const
{
    if (m_pModel)
    {
        return m_pModel->GetUp();
    }
    return A3DVECTOR3(0.0f, 1.0f, 0.0f);
}

void CAnmNPC::SetScale(const float s)
{
    if (m_pModel)
    {
        m_pModel->ScaleAllRootBonesAndGfx(s);
    }
}

float CAnmNPC::GetScale() const
{
    if (m_pModel)
    {
        return m_pModel->GetGfxScale();
    }
    return 1.0f;
}

void CAnmNPC::SetTransparent(const float t)
{
    if (m_pModel)
    {
        m_pModel->SetTransparent(1.0f - t);
    }
}

float CAnmNPC::GetTransparent() const
{
    if (m_pModel)
    {
        return 1.0f - m_pModel->GetTransparent();
    }
    return 1.0f;
}

const A3DAABB& CAnmNPC::GetAABB() const
{
    if (m_pModel)
    {
        return m_pModel->GetA3DSkinModel()->GetModelAABB();
    }
    m_AABB.Center = GetPos();
    m_AABB.CompleteMinsMaxs();
    return m_AABB;
}

void CAnmNPC::CopyDataFrom(CAnmObject *pObject)
{
    if (pObject)
    {
        CAnmNPC *pNPC = dynamic_cast<CAnmNPC*>(pObject);
        if (pNPC)
        {
            CAnmObject::CopyDataFrom(pObject);
            Load(pNPC->GetLoadFile());
            SetPos(pNPC->GetPos());
            SetDirAndUp(pNPC->GetDir(), pNPC->GetUp());
            SetScale(pNPC->GetScale());
        }
    }
}

int CAnmNPC::GetShaderIndex(const char* szShader)
{
    AString strShader = szShader;
    if (strShader.GetLength() < 5)
        return 0;

    strShader = strShader.Right(5);
    strShader = strShader[0];
    int iIndex = strShader.ToInt();

    return iIndex;
}

CAnmNPC::~CAnmNPC()
{
   g_Animator.UnloadObject(this);
}

