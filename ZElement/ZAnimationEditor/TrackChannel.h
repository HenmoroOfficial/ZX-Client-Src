#ifndef _TRACK_CHANNEL_H_
#define _TRACK_CHANNEL_H_

//#include "Animator/AnmActionChannel.h"
#include "TrackNode.h"

class CAnmActionChannel;

class CTrackChannel
{
    public:
        explicit CTrackChannel(CAnmActionChannel *pChannel);

        unsigned int GetChannelID() const;

        void SetChannel(CAnmActionChannel *pChannel);
            
        CAnmActionChannel* GetChannel() const;

        void DeleteAction(const unsigned int uiID);

        void SetObjectID(const unsigned int uiID);

        unsigned int GetObjectID() const;

        void SetSeconds(int v);

        int GetSeconds() const;

        void SetChannelName(ACString name);

        ACString GetChannelName() const;

        void SetNodeAmount(int v);

        int GetNodeAmount() const;

        void SetColor(unsigned int v);

        unsigned int GetColor() const;

        void SetNodeAt(CTrackNode *node, int v);

        CTrackNode* GetNodeAt(int v) const;

        void SetPos(int x, int y);

        CTrackNode* GetSelectNode() const;

        void OnHScroll(int v);

        void UpdateChannel();

        bool IsSelect() const;

        bool IsPointIn(int x, int y) const;

        bool OnLButtonDown(int x, int y);

        void OnDraw(CDC *pDC);

        ~CTrackChannel();

    private:
        CTrackChannel() {}

    private:
        CAnmActionChannel *m_pChannel;
        int m_iSeconds;
        int m_iHScrollPos;
        bool m_bSelected;
        int m_iX;
        int m_iY;
        int m_iHeight;
        int m_iNodeAmount;
        unsigned int m_uiColor;
        std::vector<CTrackNode*> m_vNodeList;
        CTrackNode *m_pSelectNode;
};

#endif
