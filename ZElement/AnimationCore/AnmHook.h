#ifndef _ANM_HOOK_H_
#define _ANM_HOOK_H_

#include "AnmAction.h"

class CAnmHook : public CAnmAction
{
    public:
        explicit CAnmHook(const unsigned int uiID);

        void SetTotalFrames(const int iFrame);

        int GetTotalFrames() const;

        void SetHangerName(const ACString &name);

        ACString GetHangerName() const;

        void SetHookName(const ACString &name);

        ACString GetHookName() const;

        void SetHookObjectID(const unsigned int uiID);

        unsigned int GetHookObjectID() const;

        void SetElseHookName(const ACString &name);

        ACString GetElseHookName() const;

        virtual ActionType GetActionType() const;

        virtual void CopyDataFrom(CAnmAction *pAction);

        virtual bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        virtual DWORD GetTotalTime() const;

        virtual void Reset();

        virtual ~CAnmHook();

    private:
        CAnmHook();

    private:
        int m_iTotalFrames;
        ACString m_sHangerName;
        ACString m_sHookName;
        unsigned int m_uiHookObjectID;
        ACString m_sElseHookName;
        bool m_bStartAction;
};

#endif
