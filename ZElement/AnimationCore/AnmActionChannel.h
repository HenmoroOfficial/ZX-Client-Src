#ifndef _ANM_ACTION_CHANNEL_H_
#define _ANM_ACTION_CHANNEL_H_

#include "AnmMove.h"
#include "AnmScale.h"
#include "AnmTransparent.h"
#include "AnmMotion.h"
#include "AnmJump.h"
#include "AnmBezierMove.h"
#include "AnmDisplayHide.h"
#include "AnmDialogue.h"
#include "AnmSound.h"
#include "AnmHook.h"
#include "AnmSpeedControl.h"
#include "AnmCameraBind.h"
#include "AnmCameraFilter.h"
#include "AnmCameraOscillatory.h"
#include "AnmHideWorldAction.h"
#include "AnmRotate.h"
#include "AnmFOV.h"
#include <map>
#include <vector>

class CAnmActionChannel
{
    public:
        explicit CAnmActionChannel(const unsigned int uiID);

        void SetChannelID(const unsigned int uiID);

        unsigned int GetChannelID() const;

        void BindObjectID(const unsigned int uiID);

        unsigned int GetObjectID() const;

        void SetChannelName(const ACString name);

        ACString GetChannelName() const;

        void CreateAction(const unsigned int uiID, const CAnmAction::ActionType type);

        unsigned int CreateAction(const CAnmAction::ActionType type);

        int GetActionsCount(const DWORD dwStartTime, const DWORD dwEndTime);

        void MoveActions(const DWORD dwStartTime, const DWORD dwEndTime, const DWORD dwOffset, const bool left=false);

        void DeleteAction(const unsigned int uiID);

        CAnmAction* GetAction(const unsigned int uiID);

        void GetActionIDList(std::vector<unsigned int> &vList);

        int GetActionAmount() const;

        unsigned int GetChannelType() const;

        void CopyDataFrom(CAnmActionChannel *pChannel);

        void Reset();

        bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        ~CAnmActionChannel();

    private:
        CAnmActionChannel() {}

    private:
        unsigned int m_uiChannelID;
        unsigned int m_uiObjectID;
        ACString m_sChannelName;
        std::map<unsigned int, CAnmAction*> m_mActionMap;
};

#endif
