#ifndef _TRACK_GROUP_H_
#define _TRACK_GROUP_H_

//#include "Animator/AnmActionGroup.h"
#include "TrackChannel.h"

class CAnmActionGroup;

class CTrackGroup
{
    public:
        explicit CTrackGroup(CAnmActionGroup *pGroup);

        CAnmActionGroup* GetGroup() const;

        unsigned int GetGroupID() const;

        void SetSeconds(int v);

        int GetSeconds() const;

        void SetGroupName(ACString name);

        ACString GetGroupName() const;

        void SetPos(int x, int y);

        int GetChannelAmount() const;

        void CreateChannel();

        void AddChannel(CTrackChannel *pChannel);

        CTrackChannel* RemoveChannel(const unsigned int uiID);

        void DeleteChannel(const unsigned int uiID);

        CTrackChannel* GetChannelAt(int v) const;

        CTrackChannel* GetSelectChannel() const;

        void OnHScroll(int v);

        void UpdateGroup();

        bool IsSelect() const;

        void SetFold(const bool b);

        bool IsFold() const;

        bool IsPointIn(int x, int y) const;

        bool OnLButtonDown(int x, int y);

        void OnDraw(CDC *pDC);

        ~CTrackGroup();

    private:
        CTrackGroup() {}

    private:
        CAnmActionGroup *m_pGroup;
        int m_iSeconds;
        int m_iHScrollPos;
        bool m_bSelected;
        bool m_bFold;
        int m_iX;
        int m_iY;
        int m_iHeight;
        std::vector<CTrackChannel*> m_vChannelList;
        CTrackChannel *m_pSelectChannel;
};

#endif
