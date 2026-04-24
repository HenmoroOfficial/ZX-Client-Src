#include "stdafx.h"
#include "TrackRuler.h"
#include "Animator.h"

CTrackRuler::CTrackRuler()
    : m_iSeconds(3),
      m_iHScrollPos(0),
      m_iX(0), m_iY(0),
      m_iHeight(30),
      m_bPressed(false)
{
}

void CTrackRuler::SetSeconds(int v)
{
    m_iSeconds = v;
}

int CTrackRuler::GetSeconds() const
{
    return m_iSeconds;
}

void CTrackRuler::SetHeight(int v)
{
    m_iHeight = v;
}

int CTrackRuler::GetHeight() const
{
    return m_iHeight;
}

void CTrackRuler::SetPos(int x, int y)
{
    m_iX = x;
    m_iY = y;
}

void CTrackRuler::OnHScroll(int v)
{
    m_iHScrollPos = v;
}

void CTrackRuler::OnDraw(CDC *pDC)
{
    int pos = 0;
    pDC->FillSolidRect(m_iX, m_iY, 200*m_iSeconds, m_iHeight, 0xA0A0A0);
    pDC->SetTextColor(0x000000);
    for (int i=0; i<m_iSeconds; ++i)
    {
        if (pos >= m_iHScrollPos)
        {
            int dx = m_iX + 10 * (20*i - m_iHScrollPos);
            CString text;
            text.Format(_T("%u"), i * 20 + 1);
            pDC->TextOut(dx, m_iY, text);
            pDC->MoveTo(dx, m_iY+m_iHeight-1);
            pDC->LineTo(dx, m_iY+m_iHeight-30);
        }
        for (int j=1; j<20; ++j)
        {
            if (pos >= m_iHScrollPos)
            {
                int dx = m_iX + 10 * (20*i + j - m_iHScrollPos);
                if (j % 5 == 0)
                {
                    pDC->MoveTo(dx, m_iY+m_iHeight-1);
                    pDC->LineTo(dx, m_iY+m_iHeight-15);
                }
                else
                {
                    pDC->MoveTo(dx, m_iY+m_iHeight-3);
                    pDC->LineTo(dx, m_iY+m_iHeight-10);
                }
            }
            ++pos;
        }
        ++pos;
    }
    int dx = m_iX + 10 * (20*m_iSeconds - m_iHScrollPos);
    CString text;
    text.Format(_T("%u"), m_iSeconds * 20 + 1);
    pDC->TextOut(dx, m_iY, text);
    pDC->MoveTo(dx, m_iY+m_iHeight-1);
    pDC->LineTo(dx, m_iY+m_iHeight-30);
}

bool CTrackRuler::OnLButtonDown(int x, int y)
{
    if (m_iX <= x && m_iY <= y && y <= m_iHeight)
    {
        m_bPressed = true;
        return true;
    }
    m_bPressed = false;
    return false;
}

int CTrackRuler::GetTimePos(int x)
{
    if (m_iX <= x)
    {
        int dwTime = 50 * (((x - m_iX) / 10) + m_iHScrollPos);
        return dwTime;
    }
    return -1;
}

void CTrackRuler::SetPressed(const bool b)
{
    m_bPressed = b;
}

bool CTrackRuler::IsPressed() const
{
    return m_bPressed;
}

CTrackRuler::~CTrackRuler()
{
}
