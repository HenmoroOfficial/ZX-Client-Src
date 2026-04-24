#ifndef _ANM_OBJECT_H_
#define _ANM_OBJECT_H_

#include <A3D.h>
#include <AF.h>
#include <vector>

float Clamp(float x);

class CAnmObject
{
    public:
        enum ObjectType
        {
            TYPE_CAMERA = 0,
            TYPE_PLAYER,
            TYPE_NPC,
            TYPE_GFX,
        };

		enum LoadState
		{
			LOADSTATE_NONE = 0,
				LOADSTATE_NEEDLOAD,
				LOADSTATE_LOADWAIT,
				LOADSTATE_LOADBEGIN,
				LOADSTATE_LOADEND,
				LOADSTATE_NEEDRELEASE,
		};

    public:
        explicit CAnmObject(const unsigned int uiID);

        unsigned int GetObjectID() const;

        void SetVisible(const bool b);

        bool IsVisible() const;

        void SetDisplayName(const ACString &name);

        ACString GetDisplayName() const;

        virtual ~CAnmObject();

        virtual ObjectType GetObjectType() const = 0;

        virtual bool Load(const char *szFile);

		virtual void OnLoaded() {}

        virtual ACString GetName() const = 0;

        virtual void GetActionList(std::vector<ACString> &vList) const = 0;

        virtual bool Tick(const DWORD dwDeltaTime) = 0;

        virtual bool Render(A3DViewport *viewport, bool bRenderShadow) = 0;

        virtual void PlayAction(const ACString &actionName) = 0;

        virtual void StopAction() = 0;

		virtual void PauseAction() = 0;

		virtual DWORD GetActionTimeSpan() const = 0; 

        virtual bool IsInAction() const;

        virtual void Reset() {};

        virtual void SetPos(const A3DVECTOR3 &vPos) = 0;

        virtual A3DVECTOR3 GetPos() const = 0;

        virtual void SetOffset(const A3DVECTOR3 &offset);

        virtual A3DVECTOR3 GetOffset() const;

        virtual void SetDirAndUp(const A3DVECTOR3 &vDir, const A3DVECTOR3 &vUp=A3DVECTOR3(0.0f, 1.0f, 0.0f)) = 0;

        virtual A3DVECTOR3 GetDir() const = 0;

        virtual A3DVECTOR3 GetUp() const = 0;

        virtual A3DQUATERNION GetOrientation() const;

        virtual void SetScale(const float s) = 0;

        virtual float GetScale() const = 0;

        virtual void SetTransparent(const float t) = 0;
        
        virtual float GetTransparent() const = 0;

        virtual const A3DAABB& GetAABB() const = 0;

        virtual void CopyDataFrom(CAnmObject *pObject);

        void UpdateOrientation();

		void SetLoadFile( const char* szFile ) { m_strLoadFile = szFile; }
		
		const char* GetLoadFile() const { return m_strLoadFile; }

		void SetLoadState( DWORD dwLoadState ) { m_dwLoadState = dwLoadState; }
		
		DWORD GetLoadState() const { return m_dwLoadState; }

    protected:
        void _UpdateOrientation();

    private:
        CAnmObject();

    protected:
        unsigned int m_uiObjectID;
        A3DVECTOR3 m_vOffset;
        A3DQUATERNION m_qOrientation;
        bool m_bVisible;
        ACString m_sDisplayName;
		AString m_strLoadFile;
		DWORD m_dwLoadState;
};

#endif

