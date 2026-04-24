#ifndef _ANM_TRANSPARENT_H_
#define _ANM_TRANSPARENT_H_

#include "AnmAction.h"

class CAnmTransparent : public CAnmAction
{
    public:
        explicit CAnmTransparent(const unsigned int uiID);

        void SetStartTransparent(const float t);
        
        float GetStartTransparent() const;

        void SetEndTransparent(const float t);
        
        float GetEndTransparent() const;

        void SetTotalFrames(const int iFrame);

        int GetTotalFrames() const;

        virtual ActionType GetActionType() const;

        virtual void CopyDataFrom(CAnmAction *pAction);

        virtual bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        virtual DWORD GetTotalTime() const;

        virtual ~CAnmTransparent();

    private:
        CAnmTransparent();

    private:
        float m_fStartTransparent;
        float m_fEndTransparent;
        int m_iTotalFrames;
};

#endif
