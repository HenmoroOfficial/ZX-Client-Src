#include "stdafx.h"
#include "TrackGroup.h"
#include "AnmActionGroup.h"

const unsigned int CHANNEL_COLOR_LIST[8] = {0x202020, 0x202020,
                                            0x202020, 0x202020,
                                            0x202020, 0x202020,
                                            0x202020, 0x202020};
/*
const unsigned int CHANNEL_COLOR_LIST[8] = {0x602000, 0x002060,
                                            0x004000, 0x600020,
                                            0x006060, 0x201000,
                                            0x000020, 0x200040};
*/

CTrackGroup::CTrackGroup(CAnmActionGroup *pGroup)
    : m_pGroup(pGroup),
      m_iSeconds(3),
      m_iHScrollPos(0),
      m_bSelected(false),
      m_bFold(false),
      m_pSelectChannel(0),
      m_iX(0), m_iY(0),
      m_iHeight(40)
{
}

CAnmActionGroup* CTrackGroup::GetGroup() const
{
    return m_pGroup;
}

unsigned int CTrackGroup::GetGroupID() const
{
    return m_pGroup->GetGroupID();
}

void CTrackGroup::SetSeconds(int v)
{
    m_iSeconds = v;
    for (unsigned int i=0; i<m_vChannelList.size(); ++i)
    {
        m_vChannelList[i]->SetSeconds(m_iSeconds);
    }
}

int CTrackGroup::GetSeconds() const
{
    return m_iSeconds;
}

void CTrackGroup::SetGroupName(ACString name)
{
    m_pGroup->SetGroupName(name);
}

ACString CTrackGroup::GetGroupName() const
{
    return m_pGroup->GetGroupName();
}

void CTrackGroup::SetPos(int x, int y)
{
    m_iX = x;
    m_iY = y;
    for (unsigned int i=0; i<m_vChannelList.size(); ++i)
    {
        m_vChannelList[i]->SetPos(x+20, i*20+y+20);
    }
}

int CTrackGroup::GetChannelAmount() const
{
    return m_vChannelList.size();
}

void CTrackGroup::CreateChannel()
{
    unsigned int uiID = m_pGroup->CreateChannel(CAnmActionGroup::TYPE_NPC);
    CAnmActionChannel *pChannel = m_pGroup->GetChannel(uiID);
    pChannel->SetChannelName(_T("Í¨µÀ"));
    CTrackChannel *pTrackChannel = new CTrackChannel(pChannel);
    pTrackChannel->SetColor(CHANNEL_COLOR_LIST[rand()%8]);
    pTrackChannel->SetSeconds(m_iSeconds);
    m_vChannelList.push_back(pTrackChannel);
}

void CTrackGroup::AddChannel(CTrackChannel *pChannel)
{
    m_pGroup->AddChannel(pChannel->GetChannel());
    m_vChannelList.push_back(pChannel);
}

CTrackChannel* CTrackGroup::RemoveChannel(const unsigned int uiID)
{
    std::vector<CTrackChannel *>::iterator pos;
    for (pos=m_vChannelList.begin(); pos!=m_vChannelList.end(); ++pos)
    {
        if ((*pos)->GetChannelID() == uiID)
        {
            CTrackChannel *pChannel = (*pos);
            m_pGroup->RemoveChannel(uiID);
            m_vChannelList.erase(pos);
            return pChannel;
        }
    }
    return 0;
}

void CTrackGroup::DeleteChannel(const unsigned int uiID)
{
    std::vector<CTrackChannel *>::iterator pos;
    for (pos=m_vChannelList.begin(); pos!=m_vChannelList.end(); ++pos)
    {
        if ((*pos)->GetChannelID() == uiID)
        {
            m_pGroup->DeleteChannel(uiID);
            delete (*pos);
            m_vChannelList.erase(pos);
            m_pSelectChannel = 0;
            break;
        }
    }
}

CTrackChannel* CTrackGroup::GetChannelAt(int v) const
{
    if (v >= 0 && (unsigned int)v < m_vChannelList.size())
    {
        return m_vChannelList[v];
    }
    return 0;
}

CTrackChannel* CTrackGroup::GetSelectChannel() const
{
    return m_pSelectChannel;
}

void CTrackGroup::OnHScroll(int v)
{
    m_iHScrollPos = v;
    for (unsigned int i=0; i<m_vChannelList.size(); ++i)
    {
        m_vChannelList[i]->OnHScroll(v);
    }
}

void CTrackGroup::UpdateGroup()
{
    std::vector<unsigned int> vList;
    m_pGroup->GetChannelIDList(vList);
    for (unsigned int i=0; i<vList.size(); ++i)
    {
        unsigned int uiID = vList[i];
        CAnmActionChannel *pChannel = m_pGroup->GetChannel(uiID);

 		CTrackChannel *pTrackChannel = new CTrackChannel(pChannel);
		pTrackChannel->SetColor(CHANNEL_COLOR_LIST[rand()%8]);
		pTrackChannel->SetSeconds(m_iSeconds);
		pTrackChannel->OnHScroll(m_iHScrollPos);
		pTrackChannel->UpdateChannel();
 		m_vChannelList.push_back(pTrackChannel);		
    }
}

bool CTrackGroup::IsSelect() const
{
    return m_bSelected;
}

void CTrackGroup::SetFold(const bool b)
{
    m_bFold = b;
}

bool CTrackGroup::IsFold() const
{
    return m_bFold;
}

bool CTrackGroup::IsPointIn(int x, int y) const
{
    int pos = 20*m_iSeconds-m_iHScrollPos;
    if (IsFold())
    {
        if (x>=m_iX+1 && x<=m_iX+10*pos+120 &&
            y>=m_iY+1 && y<=m_iY+20)
        {
            return true;
        }
    }
    else
    {
        if (x>=m_iX+1 && x<=m_iX+10*pos+120 &&
            y>=m_iY+1 && (unsigned int)y<=m_iY+20*m_vChannelList.size()+20)
        {
            return true;
        }
    }
    return false;
}

bool CTrackGroup::OnLButtonDown(int x, int y)
{
    m_pSelectChannel = 0;
    if (!IsFold())
    {
        for (unsigned int i=0; i<m_vChannelList.size(); ++i)
        {
            if (m_vChannelList[i]->OnLButtonDown(x, y))
            {
                m_pSelectChannel = m_vChannelList[i];
            }
        }
    }
    if (IsPointIn(x, y))
    {
        m_bSelected = true;
    }
    else
    {
        m_bSelected = false;
    }
    return m_bSelected;
}

void CTrackGroup::OnDraw(CDC *pDC)
{
    if (m_iX>=0 && m_iY>=30)
    {
        if (m_bSelected)
        {
            pDC->SetBkColor(0x0000FF);
        }
        else
        {
            pDC->SetBkColor(0x303030);
        }
        pDC->SetTextColor(0xFFFFFF);
        ACString text;
        text.Format(_T(" (%u)"), m_pGroup->GetChannelAmount());
        if (IsFold())
        {
            pDC->TextOut(m_iX, m_iY, CString(_T("+ ") + m_pGroup->GetGroupName() + text));
        }
        else
        {
            pDC->TextOut(m_iX, m_iY, CString(_T("- ") + m_pGroup->GetGroupName() + text));
        }
        if (!IsFold() && m_vChannelList.size() > 0)
        {
            CPen pen(PS_SOLID, 1, 0x00FFFF);
            CPen *oldPen = pDC->SelectObject(&pen);
            pDC->MoveTo(m_iX+10, m_iY+16);
            pDC->LineTo(m_iX+10, m_iY+20*m_vChannelList.size()+10);
            for (unsigned int i=0; i<m_vChannelList.size(); ++i)
            {
                pDC->MoveTo(m_iX+10, m_iY+20*i+30);
                pDC->LineTo(m_iX+20, m_iY+20*i+30);
            }
            pDC->SelectObject(oldPen);
        }
    }
    if (!IsFold())
    {
        for (unsigned int i=0; i<m_vChannelList.size(); ++i)
        {
            m_vChannelList[i]->OnDraw(pDC);
        }
    }
}

CTrackGroup::~CTrackGroup()
{
    for (unsigned int i=0; i<m_vChannelList.size(); ++i)
    {
        delete m_vChannelList[i];
    }
    m_vChannelList.clear();
    m_pSelectChannel = 0;
}

