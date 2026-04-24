#pragma once

#include <A3D.h>
#include <set>

class SmartCamera
{
public:
	class Listener
	{
	public:
		Listener() {}
		virtual ~Listener() {}
		virtual void OnDestPos(SmartCamera *camera, const A3DVECTOR3 &vPos) {}
		virtual void OnDestOri(SmartCamera *camera, const A3DQUATERNION &qOri) {}
		virtual void OnDestFOV(SmartCamera *camera, const float fov) {}
	};

	typedef std::set<Listener*> Listeners;
	typedef Listeners::iterator ListenersIterator;
	typedef Listeners::const_iterator ListenersConstIterator;

public:
	explicit SmartCamera(A3DCamera *pCamera);
	virtual ~SmartCamera();

	virtual bool AddListener(Listener *listener);
	virtual bool RemoveListener(Listener *listener);
	virtual void ClearListeners();

	virtual void Play();
	virtual void Stop();
	virtual void Pause();

	virtual bool Tick(const unsigned int delta);

	bool IsActive() const;
	bool IsPlaying() const;

	bool IsMoving() const;
	bool IsRotating() const;
	bool IsFOVing() const;

	void SetSpeed(const float s);
	float GetSpeed() const;

	void SetInit();

	void SetInitPos(const A3DVECTOR3 &pos);
	A3DVECTOR3 GetInitPos() const;
	A3DVECTOR3 GetPos() const;
	A3DVECTOR3 GetDestPos() const;

	void SetInitOri(const A3DQUATERNION &q);
	void SetInitOri(const A3DVECTOR3 &dir, const A3DVECTOR3 &up);
	A3DQUATERNION GetInitOri() const;
	A3DQUATERNION GetOri() const;
	A3DQUATERNION GetDestOri() const;
	A3DVECTOR3 GetInitDir() const;
	A3DVECTOR3 GetDir() const;
	A3DVECTOR3 GetDestDir() const;
	A3DVECTOR3 GetInitUp() const;
	A3DVECTOR3 GetUp() const;
	A3DVECTOR3 GetDestUp() const;

	void SetInitFOV(const float &fov);
	float GetInitFOV() const;
	float GetFOV() const;
	float GetDestFOV() const;

	void MoveTo(const A3DVECTOR3 &pos, const unsigned int useTime);
	void RotateTo(const A3DQUATERNION &q, const unsigned int useTime);
	void RotateTo(const A3DVECTOR3 &dir, const A3DVECTOR3 &up, const unsigned int useTime);
	void FOVTo(const float &fov, const unsigned int useTime);

protected:
	virtual bool _TickPos(const float delta);
	virtual bool _TickOri(const float delta);
	virtual bool _TickFOV(const float delta);

	void _FireOnDestPos();
	void _FireOnDestOri();
	void _FireOnDestFOV();

private:
	SmartCamera();
	SmartCamera(const SmartCamera &);
	SmartCamera& operator = (const SmartCamera &);

public:
	static float Clamp(const float a);
	static A3DQUATERNION GetOriFromDirAndUp(const A3DVECTOR3 &dir, const A3DVECTOR3 &up);

protected:
	A3DCamera *m_pA3DCamera;
	Listeners m_Listeners;
	bool m_bActive;
	bool m_bPaused;

	float m_fSpeed;

	A3DVECTOR3 m_vInitPos;
	A3DVECTOR3 m_vLastPos;
	A3DVECTOR3 m_vDestPos;
	float m_fMovingTimer;
	bool m_bMoving;

	A3DQUATERNION m_qInitOri;
	A3DQUATERNION m_qLastOri;
	A3DQUATERNION m_qDestOri;
	float m_fRotatingTimer;
	bool m_bRotating;

	float m_fInitFOV;
	float m_fLastFOV;
	float m_fDestFOV;
	float m_fFOVingTimer;
	bool m_bFOVing;

public:
	static const A3DVECTOR3 UNIT_LEFT;
	static const A3DVECTOR3 UNIT_UP;
	static const A3DVECTOR3 UNIT_DIR;
};