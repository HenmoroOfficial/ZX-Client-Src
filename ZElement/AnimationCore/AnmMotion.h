#ifndef _ANM_MOTION_H_
#define _ANM_MOTION_H_

#include "AnmAction.h"

class CAnmMotion : public CAnmAction
{
    public:
        enum MotionType
        {
            MT_ONCE = 0,
            MT_LOOP,
            MT_ONE_FRAME
        };

    public:
        explicit CAnmMotion(const unsigned int uiID);

        void SetStartMotionFrame(const int iFrame);

        int GetStartMotionFrame() const;

        void SetBlendFrames(const int iFrame);
        
        int GetBlendFrames() const;

        void SetTotalFrames(const int iFrame);

        int GetTotalFrames() const;

        void SetMotionType(const MotionType &mt);

        MotionType GetMotionType() const;

        void SetActionName(const ACString &name);

        ACString GetActionName() const;

        void SetActionSpeed(const float speed);
        
        float GetActionSpeed() const;

        void SetShowGFX(const bool b);

        bool IsShowGFX() const;

        void PlayAction(CAnmObject *pObject);

        virtual ActionType GetActionType() const;

        virtual void CopyDataFrom(CAnmAction *pAction);

        virtual bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        virtual DWORD GetTotalTime() const;

        virtual void Reset();

        virtual ~CAnmMotion();

    private:
        CAnmMotion();

	protected:
		void ProcessSingleFrame(CAnmObject* pObject);
		void ProcessReversePlay(CAnmObject* pObject,DWORD dT,bool isInfinite);

    private:
        int m_iStartMotionFrame;
        int m_iBlendFrames;
        int m_iTotalFrames;
        MotionType m_eMotionType;
        ACString m_sActionName;
        float m_fSpeed;
        bool m_bShowGFX;
        bool m_bStartAction;
};

#endif
