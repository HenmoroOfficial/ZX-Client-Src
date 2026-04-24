#include "stdafx.h"
#include "TrackNode.h"
#include "TrackChannel.h"
#include "Animator.h"
#include "AnmActionManager.h"
#include "AnmBezierManager.h"
#include "AnmAction.h"
#include "AnmBezierMove.h"

CTrackNode::CTrackNode(CAnmAction *pAction,CTrackChannel* pChannel)
    : m_pAction(pAction),
      m_bSelected(false),
      m_dwStartTime(0),
      m_iX(0), m_iY(0),
      m_iWidth(10), m_iHeight(20),
      m_uiColor(0xFFFFFF),
	  m_pChannel(pChannel)
{
}

void CTrackNode::SetAction(CAnmAction *pAction)
{
    m_pAction = pAction;
}

CAnmAction* CTrackNode::GetAction() const
{
    return m_pAction;
}

unsigned int CTrackNode::GetActionID() const
{
    if (m_pAction)
    {
        return m_pAction->GetActionID();
    }
    return -1;
}

void CTrackNode::SetObjectID(unsigned int uiID)
{
    if (m_pAction)
    {
        m_pAction->BindObjectID(uiID);
    }
}

unsigned int CTrackNode::GetObjectID() const
{
    if (m_pAction)
    {
        return m_pAction->GetObjectID();
    }
    return -1;
}
CTrackChannel* CTrackNode::GetTrackChannel() const
{
   return m_pChannel;
}
void CTrackNode::SetTrackChannel(CTrackChannel* pChannel)
{
  m_pChannel = pChannel;
}
void CTrackNode::SetStartTime(DWORD dwTime)
{
    m_dwStartTime = dwTime;
    if (m_pAction)
    {
        m_pAction->SetStartTime(dwTime);
    }
}

DWORD CTrackNode::GetStartTime() const
{
    if (m_pAction)
    {
        return m_pAction->GetStartTime();
    }
    return m_dwStartTime;
}

void CTrackNode::SetWidth(int v)
{
    m_iWidth = v;
}

int CTrackNode::GetWidth() const
{
    return m_iWidth;
}

void CTrackNode::SetHeight(int v)
{
    m_iHeight = v;
}

int CTrackNode::GetHeight() const
{
    return m_iHeight;
}

void CTrackNode::SetPos(int x, int y)
{
    m_iX = x;
    m_iY = y;
}

void CTrackNode::SetColor(unsigned int v)
{
    m_uiColor = v;
}
void CTrackNode::SetSelected(bool isSelected)
{
   m_bSelected = isSelected;
}
unsigned int CTrackNode::GetColor() const
{
    return m_uiColor;
}

bool CTrackNode::IsSelect() const
{
    return m_bSelected;
}

bool CTrackNode::IsPointIn(int x, int y) const
{
    if (x>=m_iX+1 && x<=m_iX+m_iWidth &&
        y>=m_iY && y<=m_iY+m_iHeight-1)
    {
        return true;
    }
    return false;
}
bool CTrackNode::IsInRect(const CRect& rect) const
{
    if(m_iX >= rect.left && m_iX + m_iWidth <= rect.right &&
		m_iY >= rect.top && m_iY+m_iHeight-1 <= rect.bottom)
		return true;
	return false;
}

bool CTrackNode::IsIntersectRect(const CRect& rect) const
{
   CRect dest;
   CRect src1 = CRect(m_iX+1,m_iY,m_iX+m_iWidth,m_iY+m_iHeight-1);
   return dest.IntersectRect(&src1,rect);
}
bool CTrackNode::OnLButtonDown(int x, int y)
{
    if (IsPointIn(x, y) && x >= 120)
    {
        m_bSelected = true;
    }
    else
    {
        m_bSelected = false;
    }
    return m_bSelected;
}

void CTrackNode::OnDraw(CDC *pDC)
{
    unsigned int color = m_uiColor;
    if (m_bSelected)
    {
        color = 0x0000FF;
    }
    else if (m_pAction)
    {
        unsigned int type = m_pAction->GetActionType();
        if (type == CAnmAction::TYPE_MOVE)
        {
            color = 0xF00000;
        }
        else if (type == CAnmAction::TYPE_ROTATE)
        {
            color = 0x0000F0;
        }
        else if (type == CAnmAction::TYPE_SCALE)
        {
            color = 0x00F000;
        }
        else if (type == CAnmAction::TYPE_FOV)
        {
            color = 0xFFFF00;
        }
        else if (type == CAnmAction::TYPE_JUMP)
        {
            color = 0xFFFF00;
        }
        else if (type == CAnmAction::TYPE_MOTION)
        {
            color = 0x00A0FF;
        }
        else if (type == CAnmAction::TYPE_BEZIER_MOVE)
        {
            color = 0xFFA000;
        }
        else if (type == CAnmAction::TYPE_DISPLAY_HIDE)
        {
            color = 0x808080;
        }
        else if (type == CAnmAction::TYPE_DIALOGUE)
        {
            color = 0x00FFFF;
        }
        else if (type == CAnmAction::TYPE_SOUND)
        {
            color = 0x40FF40;
        }
        else if (type == CAnmAction::TYPE_HOOK)
        {
            color = 0x4040FF;
        }
        else if (type == CAnmAction::TYPE_SPEED_CONTROL)
        {
            color = 0xF000F0;
        }
        else if (type == CAnmAction::TYPE_CAMERA_FILTER)
        {
            color = 0xFF4040;
        }
        else if (type == CAnmAction::TYPE_CAMERA_OSCILLATORY)
        {
            color = 0xFF4080;
        }
        else if (type == CAnmAction::TYPE_TRANSPARENT)
        {
            color = 0xA0A0A0;
        }
        else if (type == CAnmAction::TYPE_HIDE_WORLD_ACTION)
        {
            color = 0xE0E0E0;
        }
		else if(type == CAnmAction::TYPE_CAMERA_BIND)
		{
			color = 0xC0C0C0;
		}
        else
        {
            color = 0xA0A0A0;
        }
    }
    pDC->FillSolidRect(m_iX+1, m_iY, m_iWidth-1, m_iHeight-1, color);
}

void CTrackNode::DrawActionTime(CDC *pDC)
{
    if (m_pAction)
    {
        int x = m_iX + 5;
        int iWidth = m_pAction->GetTotalTime() / 5 - 10;
        if (x < 120)
        {
            x = 120;
            iWidth -= x - m_iX - 5;
        }
        if (iWidth >= 0)
        {
            CPen pen;
			CPen* oldPen;
            if (m_bSelected)
            {
                pen.CreatePen(PS_SOLID, 1, 0x0000FF);
                oldPen = pDC->SelectObject(&pen);
            }
            else
            {
                pen.CreatePen(PS_SOLID, 1, 0x808080);
                oldPen = pDC->SelectObject(&pen);
            }
            pDC->MoveTo(x, m_iY+10);
            pDC->LineTo(x+iWidth, m_iY+10);

            pDC->SelectObject(oldPen);
            pDC->Ellipse(x-3, m_iY+7, x+4, m_iY+14);
            pDC->Ellipse(x+iWidth-3, m_iY+7, x+iWidth+4, m_iY+14);
        }
        if (m_pAction->GetActionType() == CAnmAction::TYPE_BEZIER_MOVE)
        {
            CAnmBezierMove *pBezierMove = dynamic_cast<CAnmBezierMove*>(m_pAction);
            unsigned int uiID = pBezierMove->GetBezierID();
            CAnmBezier *pBezier = g_Animator.GetBezierManager()->GetBezier(uiID);
            if (pBezier)
            {
                std::vector<unsigned int> vList;
                pBezier->GetSegmentIDList(vList);
                int iDw = 0;
                int vSize = vList.size();
                for (int i=0; i<vSize-1; ++i)
                {
                    CAnmBezierSegment *pBezierSegment = pBezier->GetSegment(vList[i]);
                    iDw += pBezierSegment->GetTotalTime() / 5;
                    iWidth = iDw - 5;
                    if (m_iX + 5)
                    {
                        x = 120;
                        iWidth -= x - m_iX - 5;
                    }
                    if (iWidth > 0)
                    {
                        CPen pen;
						CPen* oldPen;
                        if (pBezierSegment->GetTail() == pBezier->GetSelectedPoint())
                        {
                            pen.CreatePen(PS_SOLID, 3, 0x0000FF);
                            oldPen = pDC->SelectObject(&pen);
                        }
                        CString text;
                        text.Format(_T("%u"), i+1);
                        pDC->TextOut(x+iWidth+3, m_iY+3, text);
                        pDC->Ellipse(x+iWidth-3, m_iY+7, x+iWidth+4, m_iY+14);
                        if (pBezierSegment->GetTail() == pBezier->GetSelectedPoint())
                        {
                            pDC->SelectObject(oldPen);
                        }
                    }
                }
            }
        }
    }
}

CTrackNode::~CTrackNode()
{
}
