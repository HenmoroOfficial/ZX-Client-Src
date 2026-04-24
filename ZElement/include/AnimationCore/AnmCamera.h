#ifndef _ANM_CAMERA_H_
#define _ANM_CAMERA_H_

#include <A3D.h>
#include <AM.h>
#include "AnmObject.h"

class CAnmCamera : public CAnmObject
{
    public:
        CAnmCamera();

        bool Init();

        void Release();

        virtual ObjectType GetObjectType() const;

        virtual bool Load(const char *szFile);

        virtual ACString GetName() const;

        virtual void GetActionList(std::vector<ACString> &vList) const;

        virtual bool Tick(const DWORD dwDeltaTime);

        virtual bool Render(A3DViewport *viewport, bool bRenderShadow);

        virtual void PlayAction(const ACString &actionName) {}

        virtual void StopAction() {}

		virtual void PauseAction() {};
		
		virtual DWORD GetActionTimeSpan() const {return 0;} 

        virtual void Reset();

        virtual void SetPos(const A3DVECTOR3 &vPos);

        virtual A3DVECTOR3 GetPos() const;

        void SetCameraOffset(const A3DVECTOR3 &vOffset);
        
        A3DVECTOR3 GetCameraOffset() const;

        virtual void SetDirAndUp(const A3DVECTOR3 &vDir, const A3DVECTOR3 &vUp=A3DVECTOR3(0.0f, 1.0f, 0.0f));

        virtual A3DVECTOR3 GetDir() const;

        virtual A3DVECTOR3 GetUp() const;

        A3DVECTOR3 GetLeft() const;

        virtual void SetScale(const float s);
        
        virtual float GetScale() const;

        virtual void SetTransparent(const float t);
        
        virtual float GetTransparent() const;

        virtual const A3DAABB& GetAABB() const;

        bool Render() const;

        void SetFOV(const float d);

        float GetFOV() const;

        A3DCamera* GetA3DCamera() const;

        A3DViewport* GetA3DViewport() const;

        virtual ~CAnmCamera();

    private:
        A3DVECTOR3 m_vPos;
        A3DVECTOR3 m_vCameraOffset;
        A3DAABB *m_pAABB;
};

#endif

