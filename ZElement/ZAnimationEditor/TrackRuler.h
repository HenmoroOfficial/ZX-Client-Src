#ifndef _TRACK_RULER_H_
#define _TRACK_RULER_H_

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CTrackRuler
{
    public:
        CTrackRuler();

        void SetSeconds(int v);

        int GetSeconds() const;

        void SetHeight(int v);

        int GetHeight() const;

        void SetPos(int x, int y);

        void OnHScroll(int v);

        void OnDraw(CDC *pDC);
        
        bool OnLButtonDown(int x, int y);

        int GetTimePos(int x);

        void SetPressed(const bool b);

        bool IsPressed() const;

        ~CTrackRuler();

    private:
        int m_iSeconds;
        int m_iHScrollPos;
        int m_iX;
        int m_iY;
        int m_iHeight;
        bool m_bPressed;
};

#endif
