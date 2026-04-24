#ifndef _ANM_BEZIER_MOVE_H_
#define _ANM_BEZIER_MOVE_H_

#include "AnmAction.h"

class CAnmBezierMove : public CAnmAction
{
    public:
        explicit CAnmBezierMove(const unsigned int uiID);

        void SetBezierID(const unsigned int uiID);

        unsigned int GetBezierID() const;

        void SetCloseTerrain(const bool b);

        bool GetCloseTerrain() const;

        virtual ActionType GetActionType() const;

        virtual void CopyDataFrom(CAnmAction *pAction);

        virtual bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        virtual DWORD GetTotalTime() const;

        virtual ~CAnmBezierMove();

    private:
        CAnmBezierMove();

    private:
        unsigned int m_uiBezierID;
        bool m_bCloseTerrain;
};

#endif
