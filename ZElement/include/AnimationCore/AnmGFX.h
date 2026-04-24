#ifndef _ANM_GFX_H_
#define _ANM_GFX_H_

#include "A3DGFXEx.h"
#include "A3DGFXExMan.h"
#include "AnmObject.h"

class CAnmGFX : public CAnmObject
{
    public:
        explicit CAnmGFX(unsigned int uiID);

        virtual ObjectType GetObjectType() const;

        virtual bool Load(const char *szFile);

        virtual ACString GetName() const;

        AString GetGFXPath() const;

        virtual void GetActionList(std::vector<ACString> &vList) const;

        virtual bool Tick(const DWORD dwDeltaTime);

        virtual bool Render(A3DViewport *viewport, bool bRenderShadow);

        virtual void PlayAction(const ACString &actionName=L"");

        virtual void StopAction();

        virtual bool IsInAction() const;

		virtual void PauseAction();

		virtual DWORD GetActionTimeSpan() const; 

        virtual void Reset();

        void SetActionSpeed(const float speed);
        
        float GetActionSpeed() const;

        virtual void SetPos(const A3DVECTOR3 &vPos);

        virtual A3DVECTOR3 GetPos() const;

        virtual void SetDirAndUp(const A3DVECTOR3 &vDir, const A3DVECTOR3 &vUp=A3DVECTOR3(0.0f, 1.0f, 0.0f));

        virtual A3DVECTOR3 GetDir() const;

        virtual A3DVECTOR3 GetUp() const;

        virtual void SetScale(const float s);
        
        virtual float GetScale() const;

        virtual void SetTransparent(const float t);
        
        virtual float GetTransparent() const;

        virtual const A3DAABB& GetAABB() const;

        virtual void CopyDataFrom(CAnmObject *pObject);

        virtual ~CAnmGFX();

    private:
        CAnmGFX();

	public:
		A3DGFXEx *m_pA3DGFX;
		float m_fScale;
		float m_fTransparent;

    private:
        ACString m_sGFXName;
        mutable A3DAABB m_AABB;
        float m_fSpeed;
        float m_fTickTime;
};

#endif
