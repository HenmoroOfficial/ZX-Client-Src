#ifndef _ANM_ACTION_GROUP_H_
#define _ANM_ACTION_GROUP_H_

#include "AnmActionChannel.h"

class CAnmActionGroup
{
    public:
        enum ActionChannelType
        {
            TYPE_CAMERA = 0,
            TYPE_PLAYER = 1,
            TYPE_NPC = 2,
        };

    public:
        explicit CAnmActionGroup(const unsigned int uiID);

        unsigned int GetGroupID() const;

        void SetGroupName(const ACString name);

        ACString GetGroupName() const;

        void CreateChannel(const unsigned int uiID, const ActionChannelType type);

        unsigned int CreateChannel(const ActionChannelType type);

        void AddChannel(CAnmActionChannel *pChannel);

        CAnmActionChannel* RemoveChannel(const unsigned int uiID);

        int GetActionsCount(const DWORD dwStartTime, const DWORD dwEndTime);

        void MoveActions(const DWORD dwStartTime, const DWORD dwEndTime, const DWORD dwOffset, const bool left=false);

        void DeleteChannel(const unsigned int uiID);

        CAnmActionChannel* GetChannel(const unsigned int uiID);

        void GetChannelIDList(std::vector<unsigned int> &vList);

        int GetChannelAmount() const;

        void SwapChannels(const unsigned int uiID1, const unsigned int uiID2);

        bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        void Reset();

        ~CAnmActionGroup();

    private:
        CAnmActionGroup() {}

    private:
        unsigned int m_uiGroupID;
        ACString m_sGroupName;
        std::map<unsigned int, CAnmActionChannel*> m_mChannelMap;
};

#endif
