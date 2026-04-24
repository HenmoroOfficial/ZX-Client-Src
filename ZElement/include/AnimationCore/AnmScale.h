#ifndef _ANM_SCALE_H_
#define _ANM_SCALE_H_

#include "AnmAction.h"

class CAnmScale : public CAnmAction
{
    public:
        explicit CAnmScale(const unsigned int uiID);

        void SetStartScale(const float s);
        
        float GetStartScale() const;

        void SetEndScale(const float s);
        
        float GetEndScale() const;

        void SetTotalFrames(const int iFrame);

        int GetTotalFrames() const;

        virtual ActionType GetActionType() const;

        virtual void CopyDataFrom(CAnmAction *pAction);

        virtual bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        virtual DWORD GetTotalTime() const;

        virtual ~CAnmScale();

    private:
        CAnmScale();

    private:
        float m_fStartScale;
        float m_fEndScale;
        int m_iTotalFrames;
};

#endif
