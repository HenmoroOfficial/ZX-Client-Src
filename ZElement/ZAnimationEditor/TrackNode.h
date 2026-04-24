#ifndef _TRACK_NODE_H_
#define _TRACK_NODE_H_

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//#include "Animator/AnmAction.h"

class CAnmAction;
class CTrackChannel;

class CTrackNode
{
    public:
        explicit CTrackNode(CAnmAction *pAction,CTrackChannel* pChannel);

        void SetAction(CAnmAction *pAction);

        CAnmAction* GetAction() const;

        unsigned int GetActionID() const;

        void SetObjectID(const unsigned int v);
        
        unsigned int GetObjectID() const;

        void SetStartTime(DWORD dwTime);
        
        DWORD GetStartTime() const;

        void SetWidth(int v);

        int GetWidth() const;

        void SetHeight(int v);

        int GetHeight() const;

        void SetPos(int x, int y);

        void SetColor(unsigned int v);

        unsigned int GetColor() const;

		void SetSelected(bool isSelected);

        bool IsSelect() const;

        bool IsPointIn(int x, int y) const;

		bool IsInRect(const CRect& rect) const;

		bool IsIntersectRect(const CRect& rect) const;

		CTrackChannel* GetTrackChannel()const;

		void SetTrackChannel(CTrackChannel* pChannel);

        bool OnLButtonDown(int x, int y);

        void OnDraw(CDC *pDC);

        void DrawActionTime(CDC *pDC);

        ~CTrackNode();

    private:
        CTrackNode() {}

    private:
		CTrackChannel* m_pChannel;
        CAnmAction *m_pAction;
        DWORD m_dwStartTime;
        unsigned int m_uiColor;
        bool m_bSelected;
        int m_iX;
        int m_iY;
        int m_iWidth;
        int m_iHeight;
};

#endif
