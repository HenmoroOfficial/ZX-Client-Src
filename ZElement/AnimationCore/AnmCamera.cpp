#include "stdafx.h"
#include "AnmCamera.h"
#include "Animator.h"

CAnmCamera::CAnmCamera()
    : CAnmObject(0)
{
    m_vPos = A3DVECTOR3(0.0f, 100.0f, 0.0f);
    m_vCameraOffset = A3DVECTOR3(0.0f);
    m_pAABB = new A3DAABB(A3DVECTOR3(0.0f), A3DVECTOR3(0.0f));
}

bool CAnmCamera::Init()
{
    m_vPos = A3DVECTOR3(0.0f, 100.0f, 0.0f);
    return true;
}

void CAnmCamera::Release()
{
    delete m_pAABB;
}

CAnmObject::ObjectType CAnmCamera::GetObjectType() const
{
    return CAnmObject::TYPE_CAMERA;
}

bool CAnmCamera::Load(const char *szFile)
{
    return true;
}

ACString CAnmCamera::GetName() const
{
    return _AL("ÉãÏñ»ú");
}

void CAnmCamera::GetActionList(std::vector<ACString> &vList) const
{
    vList.clear();
}

bool CAnmCamera::Render(A3DViewport *viewport, bool bRenderShadow)
{
    return true;
}

void CAnmCamera::Reset()
{
    m_vCameraOffset = A3DVECTOR3(0.0f);
}

void CAnmCamera::SetPos(const A3DVECTOR3 &vPos)
{
    m_vPos = vPos;
    g_Animator.GetA3DCamera()->SetPos(vPos);
}

A3DVECTOR3 CAnmCamera::GetPos() const
{
    return g_Animator.GetA3DCamera()->GetPos();
}

void CAnmCamera::SetCameraOffset(const A3DVECTOR3 &vOffset)
{
    m_vCameraOffset = vOffset;
}

A3DVECTOR3 CAnmCamera::GetCameraOffset() const
{
    return m_vCameraOffset;
}

void CAnmCamera::SetDirAndUp(const A3DVECTOR3 &vDir, const A3DVECTOR3 &vUp)
{
    g_Animator.GetA3DCamera()->SetDirAndUp(vDir, vUp);
    UpdateOrientation();
}

A3DVECTOR3 CAnmCamera::GetDir() const
{
    return g_Animator.GetA3DCamera()->GetDir();
}

A3DVECTOR3 CAnmCamera::GetUp() const
{
    return g_Animator.GetA3DCamera()->GetUp();
}

A3DVECTOR3 CAnmCamera::GetLeft() const
{
    return g_Animator.GetA3DCamera()->GetLeft();
}

void CAnmCamera::SetScale(const float s)
{
}

float CAnmCamera::GetScale() const
{
    return 1.0f;
}

void CAnmCamera::SetTransparent(const float t)
{
}

float CAnmCamera::GetTransparent() const
{
    return 1.0f;
}

const A3DAABB& CAnmCamera::GetAABB() const
{
    return *m_pAABB;
}

bool CAnmCamera::Tick(const DWORD dwDeltaTime)
{
    g_Animator.GetA3DCamera()->SetPos(m_vPos + m_vCameraOffset);
    g_Animator.GetA3DCamera()->UpdateEar();
    g_Animator.GetA3DCamera()->Update();
    m_vCameraOffset = A3DVECTOR3(0.0f);
    return true;
}

bool CAnmCamera::Render() const
{
    //m_pA3DViewport->Active();
	//m_pA3DViewport->ClearDevice();
    return true;
}

void CAnmCamera::SetFOV(const float d)
{
    g_Animator.GetA3DCamera()->SetFOV(d);
    g_Animator.GetA3DCamera()->SetFOVSet(d);
}

float CAnmCamera::GetFOV() const
{
    return g_Animator.GetA3DCamera()->GetFOV();
}

A3DCamera* CAnmCamera::GetA3DCamera() const
{
    return g_Animator.GetA3DCamera();
}

A3DViewport* CAnmCamera::GetA3DViewport() const
{
    return g_Animator.GetA3DViewport();
}

CAnmCamera::~CAnmCamera()
{
    Release();
}

