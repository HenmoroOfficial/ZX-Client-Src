#ifndef _ANM_DIALOGUE_H_
#define _ANM_DIALOGUE_H_

#include "AnmAction.h"

class CAnmDialogue : public CAnmAction
{
    public:
        explicit CAnmDialogue(const unsigned int uiID);

        void SetDialogue(const ACString &st);

        ACString GetDialogue() const;

        void SetTotalFrames(const int iFrame);

        int GetTotalFrames() const;

        virtual ActionType GetActionType() const;

        virtual void CopyDataFrom(CAnmAction *pAction);

        virtual bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        virtual DWORD GetTotalTime() const;

        virtual ~CAnmDialogue();

    private:
        CAnmDialogue();

    private:
        ACString m_sDialogue;
        int m_iTotalFrames;
};

#endif

