#ifndef _ANM_JUMP_H_
#define _ANM_JUMP_H_

#include "AnmAction.h"

class CAnmJump : public CAnmAction
{
    public:
        explicit CAnmJump(const unsigned int uiID);

        void SetStartPos(const A3DVECTOR3 &vPos);

        A3DVECTOR3 GetStartPos() const;

        void SetEndPos(const A3DVECTOR3 &vPos);

        A3DVECTOR3 GetEndPos() const;

        void SetTotalFrames(const int iFrame);

        int GetTotalFrames() const;

        virtual ActionType GetActionType() const;

        virtual void CopyDataFrom(CAnmAction *pAction);

        virtual bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        virtual DWORD GetTotalTime() const;

        virtual ~CAnmJump();

    private:
        CAnmJump();

    private:
        A3DVECTOR3 m_vStartPos;
        A3DVECTOR3 m_vEndPos;
        int m_iTotalFrames;

    public:
        static const float GRAVITY;
};

#endif
