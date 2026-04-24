#include "stdafx.h"
#include "TrackChannel.h"
#include "AnmActionChannel.h"

CTrackChannel::CTrackChannel(CAnmActionChannel *pChannel)
    : m_pChannel(pChannel),
      m_iSeconds(3),
      m_iHScrollPos(0),
      m_bSelected(false),
      m_iX(0), m_iY(0),
      m_iHeight(20),
      m_iNodeAmount(60),
      m_pSelectNode(0),
      m_uiColor(0x000000)
{
    SetSeconds(3);
}

unsigned int CTrackChannel::GetChannelID() const
{
   return m_pChannel->GetChannelID();
}

void CTrackChannel::SetChannel(CAnmActionChannel *pChannel)
{
    m_pChannel = pChannel;
    for (unsigned int i=0; i<m_vNodeList.size(); ++i)
    {
        m_vNodeList[i]->SetAction(0);
    }
    UpdateChannel();
}

CAnmActionChannel* CTrackChannel::GetChannel() const
{
    return m_pChannel;
}

void CTrackChannel::DeleteAction(const unsigned int uiID)
{
    std::vector<CTrackNode *>::iterator pos;
    for (pos=m_vNodeList.begin(); pos!=m_vNodeList.end(); ++pos)
    {
        if ((*pos)->GetActionID() == uiID)
        {
            m_pChannel->DeleteAction(uiID);
            (*pos)->SetAction(0);
            m_pSelectNode = 0;
            break;
        }
    }
}

void CTrackChannel::SetObjectID(const unsigned int uiID)
{
    m_pChannel->BindObjectID(uiID);
}

unsigned int CTrackChannel::GetObjectID() const
{
    return m_pChannel->GetObjectID();
}

void CTrackChannel::SetSeconds(int v)
{
    m_iSeconds = v;
    SetNodeAmount(20 * m_iSeconds);
}

int CTrackChannel::GetSeconds() const
{
    return m_iSeconds;
}

void CTrackChannel::SetChannelName(ACString name)
{
    m_pChannel->SetChannelName(name);
}

ACString CTrackChannel::GetChannelName() const
{
    return m_pChannel->GetChannelName();
}
int CTrackChannel::GetNodeAmount() const
{
    return m_iNodeAmount;
}
void CTrackChannel::SetNodeAmount(int v)
{
    m_iNodeAmount = v;
    int s = m_vNodeList.size();
    int d = s - v;
    if (d > 0)
    {
        for (int i=0; i<d; ++i)
        {
            if (m_vNodeList[m_vNodeList.size()-1] != 0)
            {
                delete m_vNodeList[m_vNodeList.size()-1];
                m_vNodeList[m_vNodeList.size()-1] = 0;
            }
            m_vNodeList.pop_back();
        }
    }
    else
    {
        for (int i=0; i>d; --i)
        {
            CTrackNode *pNode = new CTrackNode(0,this);
            pNode->SetColor(m_uiColor);
            pNode->SetStartTime(50*m_vNodeList.size());
            m_vNodeList.push_back(pNode);
        }
    }
}

void CTrackChannel::SetColor(unsigned int v)
{
    m_uiColor = v;
    for (unsigned int i=0; i<m_vNodeList.size(); ++i)
    {
        m_vNodeList[i]->SetColor(m_uiColor);
    }
}

unsigned int CTrackChannel::GetColor() const
{
    return m_uiColor;
}

void CTrackChannel::SetNodeAt(CTrackNode *node, int v)
{
    if (v < m_iNodeAmount)
    {
        if (m_vNodeList[v] == 0)
        {
            m_vNodeList[v] = node;
        }
    }
}

CTrackNode* CTrackChannel::GetNodeAt(int v) const
{
    if (v < m_iNodeAmount)
    {
        return m_vNodeList[v];
    }
    return 0;
}

void CTrackChannel::SetPos(int x, int y)
{
    m_iX = x;
    m_iY = y;
    for (unsigned int i=0; i<m_vNodeList.size(); ++i)
    {
        if (m_vNodeList[i] != 0)
        {
            m_vNodeList[i]->SetPos(10*(i-m_iHScrollPos)+x+100, y);
        }
    }
}

CTrackNode* CTrackChannel::GetSelectNode() const
{
    return m_pSelectNode;
}

void CTrackChannel::OnHScroll(int v)
{
    m_iHScrollPos = v;
}

void CTrackChannel::UpdateChannel()
{   
    std::vector<unsigned int> vList;
    m_pChannel->GetActionIDList(vList);
    for (unsigned int i=0; i<vList.size(); ++i)
    {
        unsigned int uiID = vList[i];
        CAnmAction *pAction = m_pChannel->GetAction(uiID);
        DWORD dwStartTime = pAction->GetStartTime();
        unsigned int pos = dwStartTime/50;
        if (pos < m_vNodeList.size())
        {
            m_vNodeList[pos]->SetAction(pAction);
        }
    }
}

bool CTrackChannel::IsSelect() const
{
    return m_bSelected;
}

bool CTrackChannel::IsPointIn(int x, int y) const
{
    int pos = 20*m_iSeconds-m_iHScrollPos;
    if (x>=m_iX && x<=m_iX+10*pos+100 &&
        y>=m_iY && y<=m_iY+m_iHeight-1)
    {
        return true;
    }
    return false;
}

bool CTrackChannel::OnLButtonDown(int x, int y)
{
    m_pSelectNode = 0;
    unsigned int drawNodeCount = m_vNodeList.size() - m_iHScrollPos;
    if (drawNodeCount > 200)
    {
        drawNodeCount = 200;
    }
    for (unsigned int i=m_iHScrollPos; i<m_iHScrollPos + drawNodeCount; ++i)
    {
        if (m_vNodeList[i]->OnLButtonDown(x, y))
        {
            m_pSelectNode = m_vNodeList[i];
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

void CTrackChannel::OnDraw(CDC *pDC)
{
	int i;
    if (m_iX>=0 && m_iY>=30)
    {
        int pos = 20*m_iSeconds-m_iHScrollPos;
        // Draw background
        if (m_bSelected)
        {
            pDC->FillSolidRect(m_iX, m_iY, 100, m_iHeight-1, 0x0000FF);
        }
        else
        {
            pDC->FillSolidRect(m_iX, m_iY, 100, m_iHeight-1, m_uiColor >> 1);
        }
        pDC->SetTextColor(0xFFFFFF);
        pDC->TextOut(m_iX, m_iY, CString(m_pChannel->GetChannelName()));
    
        // Draw nodes
        int drawNodeCount = m_vNodeList.size() - m_iHScrollPos;
        if (drawNodeCount > 200)
        {
            drawNodeCount = 200;
        }
        for (i=m_iHScrollPos; i<m_iHScrollPos + drawNodeCount; ++i)
        {
            m_vNodeList[i]->OnDraw(pDC);
        }
        for (i=0; i<(int)m_vNodeList.size(); ++i)
        {
            m_vNodeList[i]->DrawActionTime(pDC);
        }
    }
}

CTrackChannel::~CTrackChannel()
{
    for (unsigned int i=0; i<m_vNodeList.size(); ++i)
    {
        delete m_vNodeList[i];
    }
    m_vNodeList.clear();
    m_pSelectNode = 0;
}

