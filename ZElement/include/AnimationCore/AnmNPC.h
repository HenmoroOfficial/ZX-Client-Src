#ifndef _ANM_NPC_H_
#define _ANM_NPC_H_

#include "EC_Model.h"
#include "A3DCombinedAction.h"
#include "AnmObject.h"

#define MAX_ANIMATION_SKIN_TEXTURE 3

class CAnmGFX;

class CAnmNPC : public CAnmObject
{
    public:
		enum 
		{
            ESTATE_ACT_STOP,
		    ESTATE_ACT_PLAY,
			ESTATE_ACT_PAUSE,
			ESTATE_ACT_NUM,
		};
        explicit CAnmNPC(unsigned int uiID);

        virtual ObjectType GetObjectType() const;

        virtual bool Load(const char *szFile);

        virtual ACString GetName() const;

        virtual void GetActionList(std::vector<ACString> &vList) const;

        void GetHookList(std::vector<ACString> &vList) const;

        void AddChildModel(const ACString &hangerName,
                           const ACString &hookName,
                           CAnmObject *pObject,
                           const ACString &elseHookName);

        void RemoveChildModel(const ACString &hangerName);

        void PlayGFX(const ACString &hookName, CAnmGFX *pGFX);

        void RemoveGFX(const ACString &hookName, CAnmGFX *pGFX);

        void SetHooked(const bool hooked);

        bool IsHooked() const;

        void SetActionSpeed(const float speed);

        float GetActionSpeed() const;

        void UseShader(const int iShader);

        virtual bool Tick(const DWORD dwDeltaTime);

        virtual bool Render(A3DViewport *viewport, bool bRenderShadow);

        virtual void PlayAction(const ACString &actionName);

        void PlayAction(const ACString &actionName, const int iBlendTime, const bool bShowGFX);

        virtual void StopAction();

		virtual void PauseAction();

        virtual bool IsInAction() const;

		virtual bool IsActionFinished() const;

		virtual bool IsActionInfinite() const;

		virtual DWORD GetActionTimeSpan() const; 

        virtual void Reset();

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

        virtual ~CAnmNPC();

    private:
        CAnmNPC();

    public:
        static int GetShaderIndex(const char* szShader);

    public:
        CECModel *m_pModel;
        bool m_bHooked;

	protected:
		A3DSkin *m_pSkin;
        int m_iShaders[MAX_ANIMATION_SKIN_TEXTURE];
        ACString m_sModelName;
        ACString m_sActionName;
        mutable A3DAABB m_AABB;
        float m_fSpeed;
        float m_fTickTime;
		int   m_eActState;
};

#endif
