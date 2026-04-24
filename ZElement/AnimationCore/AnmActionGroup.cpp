#include "stdafx.h"
#include "AnmActionGroup.h"

CAnmActionGroup::CAnmActionGroup(const unsigned int uiID)
{
    m_uiGroupID = uiID;
}

unsigned int CAnmActionGroup::GetGroupID() const
{
    return m_uiGroupID;
}

void CAnmActionGroup::SetGroupName(const ACString name)
{
    m_sGroupName = name;
}

ACString CAnmActionGroup::GetGroupName() const
{
    return m_sGroupName;
}

void CAnmActionGroup::CreateChannel(const unsigned int uiID, const ActionChannelType type)
{
    switch (type)
    {
        case TYPE_CAMERA:
        {
            break;
        }

        case TYPE_PLAYER:
        {
            break;
        }

        case TYPE_NPC:
        {
            CAnmActionChannel *pChannel = new CAnmActionChannel(uiID);
            m_mChannelMap[uiID] = pChannel;
        }

        default:
            break;
    }
}

unsigned int CAnmActionGroup::CreateChannel(const ActionChannelType type)
{
    unsigned int uiID;
	std::map<unsigned int, CAnmActionChannel*>::iterator itMap;
    for (unsigned int i=2; i<65535; ++i)
    {
        itMap = m_mChannelMap.find(i);
        if (itMap == m_mChannelMap.end())
        {
            uiID = i;
            break;
        }
    }
    CreateChannel(uiID, type);
    return uiID;
}

void CAnmActionGroup::AddChannel(CAnmActionChannel *pChannel)
{
    unsigned int uiID;
	std::map<unsigned int, CAnmActionChannel*>::iterator itMap;
    for (unsigned int i=2; i<65535; ++i)
    {
        itMap = m_mChannelMap.find(i);
        if (itMap == m_mChannelMap.end())
        {
            uiID = i;
            pChannel->SetChannelID(uiID);
            m_mChannelMap[uiID] = pChannel;
            break;
        }
    }
}

CAnmActionChannel* CAnmActionGroup::RemoveChannel(const unsigned int uiID)
{
    std::map<unsigned int, CAnmActionChannel*>::iterator itMap = m_mChannelMap.find(uiID);
    if (itMap != m_mChannelMap.end())
    {
        CAnmActionChannel *pChannel = itMap->second;
        m_mChannelMap.erase(itMap);
        return pChannel;
    }
    return 0;
}

int CAnmActionGroup::GetActionsCount(const DWORD dwStartTime, const DWORD dwEndTime)
{
    int count = 0;
	std::map<unsigned int, CAnmActionChannel*>::iterator itMap;
    for (itMap=m_mChannelMap.begin(); itMap!=m_mChannelMap.end(); ++itMap)
    {
        count += itMap->second->GetActionsCount(dwStartTime, dwEndTime);
    }
    return count;
}

void CAnmActionGroup::MoveActions(const DWORD dwStartTime, const DWORD dwEndTime, const DWORD dwOffset, const bool left)
{
    if (left && dwStartTime < dwOffset)
    {
        return;
    }
	std::map<unsigned int, CAnmActionChannel*>::iterator itMap;
    for (itMap=m_mChannelMap.begin(); itMap!=m_mChannelMap.end(); ++itMap)
    {
        itMap->second->MoveActions(dwStartTime, dwEndTime, dwOffset, left);
    }
}

void CAnmActionGroup::DeleteChannel(const unsigned int uiID)
{
    std::map<unsigned int, CAnmActionChannel*>::iterator itMap = m_mChannelMap.find(uiID);
    if (itMap != m_mChannelMap.end())
    {
        delete itMap->second;
        m_mChannelMap.erase(itMap);
    }
}

CAnmActionChannel* CAnmActionGroup::GetChannel(const unsigned int uiID)
{
    std::map<unsigned int, CAnmActionChannel*>::iterator itMap = m_mChannelMap.find(uiID);
    if (itMap != m_mChannelMap.end())
    {
        return itMap->second;
    }
    return 0;
}

void CAnmActionGroup::GetChannelIDList(std::vector<unsigned int> &vList)
{
    vList.clear();
	std::map<unsigned int, CAnmActionChannel*>::iterator itMap;
    for (itMap=m_mChannelMap.begin(); itMap!=m_mChannelMap.end(); ++itMap)
    {
        vList.push_back(itMap->first);
    }
}

int CAnmActionGroup::GetChannelAmount() const
{
    return m_mChannelMap.size();
}

void CAnmActionGroup::SwapChannels(const unsigned int uiID1, const unsigned int uiID2)
{
    if (uiID1 != uiID2)
    {
        CAnmActionChannel *pChannel1 = GetChannel(uiID1);
        CAnmActionChannel *pChannel2 = GetChannel(uiID2);
        if (pChannel1 && pChannel2)
        {
            pChannel1->SetChannelID(uiID2);
            pChannel2->SetChannelID(uiID1);
            m_mChannelMap[uiID1] = pChannel2;
            m_mChannelMap[uiID2] = pChannel1;
        }
    }
}

void CAnmActionGroup::Reset()
{
	std::map<unsigned int, CAnmActionChannel*>::iterator itMap;
    for (itMap=m_mChannelMap.begin(); itMap!=m_mChannelMap.end(); ++itMap)
    {
        itMap->second->Reset();
    }
}

bool CAnmActionGroup::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    bool flag = false;
	std::map<unsigned int, CAnmActionChannel*>::iterator itMap;
    for (itMap=m_mChannelMap.begin(); itMap!=m_mChannelMap.end(); ++itMap)
    {
        if (itMap->second->Tick(dwNowTime, pObjectManager))
        {
            flag = true;
        }
    }
    return flag;
}

CAnmActionGroup::~CAnmActionGroup()
{
	std::map<unsigned int, CAnmActionChannel*>::iterator itMap;
    for (itMap=m_mChannelMap.begin(); itMap!=m_mChannelMap.end(); ++itMap)
    {
        delete itMap->second;
    }
    m_mChannelMap.clear();
}
