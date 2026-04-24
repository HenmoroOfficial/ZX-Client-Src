#include "SmartCamera.h"

const A3DVECTOR3 SmartCamera::UNIT_LEFT(1.0f, 0.0f, 0.0f);
const A3DVECTOR3 SmartCamera::UNIT_UP(0.0f, 1.0f, 0.0f);
const A3DVECTOR3 SmartCamera::UNIT_DIR(0.0f, 0.0f, -1.0f);

SmartCamera::SmartCamera(A3DCamera *pCamera)
    : m_pA3DCamera(pCamera),
      m_bActive(false),
      m_bPaused(false),
      m_fSpeed(1.0f),
      m_vInitPos(0.0f),
      m_vLastPos(0.0f),
      m_vDestPos(0.0f),
      m_fMovingTimer(0.0f),
      m_bMoving(false),
      m_qInitOri(1.0f, 0.0f, 0.0f, 0.0f),
      m_qLastOri(1.0f, 0.0f, 0.0f, 0.0f),
      m_qDestOri(1.0f, 0.0f, 0.0f, 0.0f),
      m_fRotatingTimer(0.0f),
      m_bRotating(false),
      m_fInitFOV(0.0f),
      m_fLastFOV(0.0f),
      m_fDestFOV(0.0f),
      m_fFOVingTimer(0.0f),
      m_bFOVing(false)
{
}

SmartCamera::~SmartCamera()
{
}

bool SmartCamera::AddListener(Listener *listener)
{
    return m_Listeners.insert(listener).second;
}

bool SmartCamera::RemoveListener(Listener *listener)
{
    return m_Listeners.erase(listener) > 0;
}

void SmartCamera::ClearListeners()
{
    m_Listeners.clear();
}

void SmartCamera::Play()
{
    m_bActive = true;
    m_bPaused = false;
    m_fSpeed = 1.0f;
}

void SmartCamera::Stop()
{
	m_bActive = false;
    m_bPaused = false;
    m_fSpeed = 1.0f;
    m_bMoving = false;
    m_bRotating = false;
    m_bFOVing = false;
}

void SmartCamera::Pause()
{
    m_bPaused = true;
}

bool SmartCamera::Tick(const unsigned int delta)
{
    if (!m_bActive) return false;
    if (m_bPaused) return true;

    float dT = m_fSpeed * delta;

    bool bTickPos = _TickPos(dT);
    bool bTickOri = _TickOri(dT);
    bool bTickFOV = _TickFOV(dT);

    return (bTickPos || bTickOri || bTickFOV);
}

bool SmartCamera::IsActive() const
{
    return m_bActive;
}

bool SmartCamera::IsPlaying() const
{
    return (m_bActive && !m_bPaused);
}

bool SmartCamera::IsMoving() const
{
    return (m_bActive && m_bMoving);
}

bool SmartCamera::IsRotating() const
{
    return (m_bActive && m_bRotating);
}

bool SmartCamera::IsFOVing() const
{
    return (m_bActive && m_bFOVing);
}

void SmartCamera::SetSpeed(const float s)
{
    if (s > 0.0f) m_fSpeed = s;
}

float SmartCamera::GetSpeed() const
{
    return m_fSpeed;
}

void SmartCamera::SetInit()
{
    if (m_pA3DCamera)
    {
        SetInitPos(m_pA3DCamera->GetPos());
        SetInitOri(m_pA3DCamera->GetDir(), m_pA3DCamera->GetUp());
        SetInitFOV(m_pA3DCamera->GetFOV());
    }
}

void SmartCamera::SetInitPos(const A3DVECTOR3 &pos)
{
    m_vInitPos = pos;
    m_vLastPos = pos;
    m_vDestPos = pos;
}

A3DVECTOR3 SmartCamera::GetInitPos() const
{
    return m_vInitPos;
}

A3DVECTOR3 SmartCamera::GetPos() const
{
    return m_vLastPos;
}

A3DVECTOR3 SmartCamera::GetDestPos() const
{
    return m_vDestPos;
}

void SmartCamera::SetInitOri(const A3DQUATERNION &q)
{
    m_qInitOri = q;
    m_qLastOri = q;
    m_qDestOri = q;
}

void SmartCamera::SetInitOri(const A3DVECTOR3 &dir, const A3DVECTOR3 &up)
{
    SetInitOri(SmartCamera::GetOriFromDirAndUp(dir, up));
}

A3DQUATERNION SmartCamera::GetInitOri() const
{
    return m_qInitOri;
}

A3DQUATERNION SmartCamera::GetOri() const
{
    return m_qLastOri;
}

A3DQUATERNION SmartCamera::GetDestOri() const
{
    return m_qDestOri;
}

A3DVECTOR3 SmartCamera::GetInitDir() const
{
    return m_qInitOri ^ SmartCamera::UNIT_DIR;
}

A3DVECTOR3 SmartCamera::GetDir() const
{
    return m_qLastOri ^ SmartCamera::UNIT_DIR;
}

A3DVECTOR3 SmartCamera::GetDestDir() const
{
    return m_qDestOri ^ SmartCamera::UNIT_DIR;
}

A3DVECTOR3 SmartCamera::GetInitUp() const
{
    return m_qInitOri ^ SmartCamera::UNIT_UP;
}

A3DVECTOR3 SmartCamera::GetUp() const
{
    return m_qLastOri ^ SmartCamera::UNIT_UP;
}

A3DVECTOR3 SmartCamera::GetDestUp() const
{
    return m_qDestOri ^ SmartCamera::UNIT_UP;
}

void SmartCamera::SetInitFOV(const float &fov)
{
    m_fInitFOV = fov;
    m_fLastFOV = fov;
    m_fDestFOV = fov;
}

float SmartCamera::GetInitFOV() const
{
    return m_fInitFOV;
}

float SmartCamera::GetFOV() const
{
    return m_fLastFOV;
}

float SmartCamera::GetDestFOV() const
{
    return m_fDestFOV;
}

void SmartCamera::MoveTo(const A3DVECTOR3 &pos, const unsigned int useTime)
{
    m_vDestPos = pos;
    m_fMovingTimer = float(useTime);
    m_bMoving = true;
}

void SmartCamera::RotateTo(const A3DQUATERNION &q, const unsigned int useTime)
{
    m_qDestOri = q;
    m_fRotatingTimer = float(useTime);
    m_bRotating = true;
}

void SmartCamera::RotateTo(const A3DVECTOR3 &dir, const A3DVECTOR3 &up, const unsigned int useTime)
{
    RotateTo(SmartCamera::GetOriFromDirAndUp(dir, up), useTime);
}

void SmartCamera::FOVTo(const float &fov, const unsigned int useTime)
{
    m_fDestFOV = fov;
    m_fFOVingTimer = float(useTime);
    m_bFOVing = true;
}

bool SmartCamera::_TickPos(const float delta)
{
    if (!m_bMoving) return false;

    if (delta < m_fMovingTimer)
    {
        float u = delta / m_fMovingTimer;
        m_vLastPos += u * (m_vDestPos - m_vLastPos);
        m_fMovingTimer -= delta;
    }
    else
    {
        m_vLastPos = m_vDestPos;
        m_fMovingTimer = 0.0f;
        m_bMoving = false;
        _FireOnDestPos();
    }

    return m_bMoving;
}

bool SmartCamera::_TickOri(const float delta)
{
    if (!m_bRotating) return false;

    if (delta < m_fRotatingTimer)
    {
        float u = delta / m_fRotatingTimer;
        m_qLastOri = SLERPQuad(m_qLastOri, m_qDestOri, u);
        m_fRotatingTimer -= delta;
    }
    else
    {
        m_qLastOri = m_qDestOri;
        m_fRotatingTimer = 0.0f;
        m_bRotating = false;
        _FireOnDestOri();
    }

    return m_bRotating;
}

bool SmartCamera::_TickFOV(const float delta)
{
    if (!m_bFOVing) return false;

    if (delta < m_fFOVingTimer)
    {
        float u = delta / m_fFOVingTimer;
        m_fLastFOV += u * (m_fDestFOV - m_fLastFOV);
        m_fFOVingTimer -= delta;
    }
    else
    {
        m_fLastFOV = m_fDestFOV;
        m_fFOVingTimer = 0.0f;
        m_bFOVing = false;
        _FireOnDestFOV();
    }

    return m_bFOVing;
}

float SmartCamera::Clamp(const float a)
{
    if (a < -1.0f) return -1.0f;
    if (a > 1.0f) return 1.0f;
    return a;
}

A3DQUATERNION SmartCamera::GetOriFromDirAndUp(const A3DVECTOR3 &dir, const A3DVECTOR3 &up)
{
    A3DVECTOR3 vDir = dir;
    vDir.Normalize();
    A3DVECTOR3 vLeft = CrossProduct(vDir, up);
    vLeft.Normalize();
    A3DVECTOR3 vUp = CrossProduct(vLeft, vDir);
    vUp.Normalize();

    A3DVECTOR3 yawD = vDir;
    yawD.y = 0.0f;
    yawD.Normalize();

    float angleYaw = (float)acos(SmartCamera::Clamp(DotProduct(SmartCamera::UNIT_DIR, yawD)));
    if (yawD.x > 0.0f) angleYaw = -angleYaw;
    A3DQUATERNION q(SmartCamera::UNIT_UP, angleYaw);

    float anglePitch = (float)acos(Clamp(DotProduct(yawD, vDir)));
    if (vDir.y < 0.0f) anglePitch = -anglePitch;
    q = q * A3DQUATERNION(SmartCamera::UNIT_LEFT, anglePitch);

    float angleRoll = (float)acos(Clamp(DotProduct(q ^ SmartCamera::UNIT_UP, vUp)));
    vLeft = q ^ SmartCamera::UNIT_LEFT;
    float dQ = DotProduct(vUp, vLeft);
    if (dQ < 0.0f) angleRoll = -angleRoll;
    q = q * A3DQUATERNION(SmartCamera::UNIT_DIR, angleRoll);

    return q;
}

void SmartCamera::_FireOnDestPos()
{
    ListenersIterator it, iend;
    iend = m_Listeners.end();
    for (it=m_Listeners.begin(); it!=iend; ++it)
    {
        if ((*it)) (*it)->OnDestPos(this, m_vDestPos);
    }
}

void SmartCamera::_FireOnDestOri()
{
    ListenersIterator it, iend;
    iend = m_Listeners.end();
    for (it=m_Listeners.begin(); it!=iend; ++it)
    {
        if ((*it)) (*it)->OnDestOri(this, m_qDestOri);
    }
}

void SmartCamera::_FireOnDestFOV()
{
    ListenersIterator it, iend;
    iend = m_Listeners.end();
    for (it=m_Listeners.begin(); it!=iend; ++it)
    {
        if ((*it)) (*it)->OnDestFOV(this, m_fDestFOV);
    }
}