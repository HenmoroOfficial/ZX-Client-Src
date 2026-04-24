#ifndef _ANM_HIDE_WORLD_ACTION_H_
#define _ANM_HIDE_WORLD_ACTION_H_

#include "AnmAction.h"

class CAnmHideWorldAction : public CAnmAction
{
    public:
        enum HideWorldType
        {
            HWT_NONE = 0,
            HWT_HIDE_ANIMATION_MODEL,
            HWT_HIDE_WORLD,
        };

    public:
        explicit CAnmHideWorldAction(const unsigned int uiID);

        void SetHideWorldType(const unsigned int t);
        
        unsigned int GetHideWorldType() const;

        void SetColor(const unsigned int color);
        
        unsigned int GetColor() const;

        void SetTotalFrames(const int iFrame);

        int GetTotalFrames() const;

        virtual ActionType GetActionType() const;

        virtual void CopyDataFrom(CAnmAction *pAction);

        virtual bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        virtual DWORD GetTotalTime() const;

        virtual ~CAnmHideWorldAction();

    private:
        CAnmHideWorldAction();

    private:
        unsigned int m_uiHideWorldType;
        unsigned int m_uiColor;
        int m_iTotalFrames;
};

#endif
