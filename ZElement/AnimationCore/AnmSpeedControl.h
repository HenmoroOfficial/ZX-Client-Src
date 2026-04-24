#ifndef _ANM_SPEED_CONTROL_H_
#define _ANM_SPEED_CONTROL_H_

#include "AnmAction.h"

class CAnmSpeedControl : public CAnmAction
{
    public:
        explicit CAnmSpeedControl(const unsigned int uiID);

        void SetTotalFrames(const int iFrame);

        int GetTotalFrames() const;

        void SetSpeed(const float speed);

        float GetSpeed() const;

        virtual ActionType GetActionType() const;

        virtual void CopyDataFrom(CAnmAction *pAction);

        virtual bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        virtual DWORD GetTotalTime() const;

        virtual ~CAnmSpeedControl();

    private:
        CAnmSpeedControl();

    private:
        float m_fSpeed;
        int m_iTotalFrames;
};

#endif
