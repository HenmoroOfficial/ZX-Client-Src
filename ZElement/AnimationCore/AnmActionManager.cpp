#include "AnmActionManager.h"
#include "Animator.h"
#include "AnmAction.h"
#include "AnmSpeedControl.h"
#include "AnmActionGroup.h"

CAnmActionManager::CAnmActionManager()
{
    /*
    CreateGroup(0);
    */
    m_ActiveActionTreeRoot.mpAction = 0;
    m_ActiveActionTreeRoot.mEndTime = 0;
    m_ActiveActionTreeRoot.mpLeftNode = 0;
    m_ActiveActionTreeRoot.mpRightNode = 0;
}

void CAnmActionManager::CreateGroup(const unsigned int uiID)
{
    CAnmActionGroup *pGroup = new CAnmActionGroup(uiID);
    m_mGroupMap[uiID] = pGroup;
}

unsigned int CAnmActionManager::CreateGroup()
{
    unsigned int uiID;
	std::map<unsigned int, CAnmActionGroup*>::iterator itMap;
    for (unsigned int i=1; i<65535; ++i)
    {
        itMap = m_mGroupMap.find(i);
        if (itMap == m_mGroupMap.end())
        {
            uiID = i;
            break;
        }
    }
    CreateGroup(uiID);
    return uiID;
}

void CAnmActionManager::DeleteGroup(const unsigned int uiID)
{
	std::map<unsigned int, CAnmActionGroup*>::iterator itMap;
    itMap = m_mGroupMap.find(uiID);
    if (itMap != m_mGroupMap.end())
    {
        delete itMap->second;
        m_mGroupMap.erase(itMap);
    }
}

CAnmActionGroup* CAnmActionManager::GetGroup(const unsigned int uiID)
{
	std::map<unsigned int, CAnmActionGroup*>::iterator itMap;
    itMap = m_mGroupMap.find(uiID);
    if (itMap != m_mGroupMap.end())
    {
        return itMap->second;
    }
    return 0;
}

void CAnmActionManager::GetGroupIDList(std::vector<unsigned int> &vList)
{
    vList.clear();
	std::map<unsigned int, CAnmActionGroup*>::iterator itMap;
    for (itMap=m_mGroupMap.begin(); itMap!=m_mGroupMap.end(); ++itMap)
    {
        vList.push_back(itMap->first);
    }
}

int CAnmActionManager::GetGroupAmount() const
{
    return m_mGroupMap.size();
}

int CAnmActionManager::GetActionsCount(const DWORD dwStartTime, const DWORD dwEndTime)
{
    int count = 0;
	std::map<unsigned int, CAnmActionGroup*>::iterator itMap;
    for (itMap=m_mGroupMap.begin(); itMap!=m_mGroupMap.end(); ++itMap)
    {
        count += itMap->second->GetActionsCount(dwStartTime, dwEndTime);
    }
    return count;
}

void CAnmActionManager::MoveActions(const DWORD dwStartTime, const DWORD dwEndTime, const DWORD dwOffset, const bool left)
{
    if (left && dwStartTime < dwOffset)
    {
        return;
    }
	std::map<unsigned int, CAnmActionGroup*>::iterator itMap;
    for (itMap=m_mGroupMap.begin(); itMap!=m_mGroupMap.end(); ++itMap)
    {
        itMap->second->MoveActions(dwStartTime, dwEndTime, dwOffset, left);
    }
}

void CAnmActionManager::Prepare()
{
    //a_LogOutput(1, "Before Prepare");
    Reset();

    ActionVector activeActionList;
    // Push back all the action into active action list
	std::map<unsigned int, CAnmActionGroup*>::iterator itMap;
    for (itMap=m_mGroupMap.begin(); itMap!=m_mGroupMap.end(); ++itMap)
    {
        std::vector<unsigned int> vChannelIDList;
        itMap->second->GetChannelIDList(vChannelIDList);
        for (size_t iC=0; iC<vChannelIDList.size(); ++iC)
        {
            CAnmActionChannel *pChannel = itMap->second->GetChannel(vChannelIDList[iC]);
            std::vector<unsigned int> vActionIDList;
            pChannel->GetActionIDList(vActionIDList);
            for (size_t iA=0; iA<vActionIDList.size(); ++iA)
            {
                CAnmAction *pAction = pChannel->GetAction(vActionIDList[iA]);
                if (pAction)
                {
                    activeActionList.push_back(pAction);
                }
            }
        }
    }

    // sort the active action list
    std::sort(activeActionList.begin(), activeActionList.end(), CAnmActionManager::ActionCompareStartTime);

    // Move actions from Active action list into Prepare
	ActionVectorIterator itActive;
    for (itActive=activeActionList.begin(); itActive!=activeActionList.end(); ++itActive)
    {
        m_lPrepareActionList.push_back(*itActive);
    }
    //a_LogOutput(1, "Ater Prepare");
    activeActionList.clear();
}

void CAnmActionManager::Reset()
{
	std::map<unsigned int, CAnmActionGroup*>::iterator itMap;
    for (itMap=m_mGroupMap.begin(); itMap!=m_mGroupMap.end(); ++itMap)
    {
        itMap->second->Reset();
    }
    m_lPrepareActionList.clear();
    _ClearActiveActionTree();
    m_ActiveActionTreeRoot.mpAction = 0;
    m_ActiveActionTreeRoot.mEndTime = g_Animator.GetTotalTime() / 2;
    m_ActiveActionTreeRoot.mpLeftNode = 0;
    m_ActiveActionTreeRoot.mpRightNode = 0;
}

float CAnmActionManager::CalculateTotalTime()
{
    float fTotalTime = float(g_Animator.GetTotalTime());
	std::map<unsigned int, CAnmActionGroup*>::iterator itMap;
    for (itMap=m_mGroupMap.begin(); itMap!=m_mGroupMap.end(); ++itMap)
    {
        std::vector<unsigned int> vChannelIDList;
        itMap->second->GetChannelIDList(vChannelIDList);
        for (size_t iC=0; iC<vChannelIDList.size(); ++iC)
        {
            CAnmActionChannel *pChannel = itMap->second->GetChannel(vChannelIDList[iC]);
            std::vector<unsigned int> vActionIDList;
            pChannel->GetActionIDList(vActionIDList);
            for (size_t iA=0; iA<vActionIDList.size(); ++iA)
            {
                CAnmAction *pAction = pChannel->GetAction(vActionIDList[iA]);
                if (pAction && pAction->GetActionType() == CAnmAction::TYPE_SPEED_CONTROL)
                {
                    CAnmSpeedControl *pSpeedControl = dynamic_cast<CAnmSpeedControl*>(pAction);
                    float speed = pSpeedControl->GetSpeed();
                    fTotalTime += float(pSpeedControl->GetTotalTime()) * ((1.0f - speed) / speed);
                }
            }
        }
    }
    return fTotalTime / 1000.0f;
}

bool CAnmActionManager::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    // Add Actions into Active Action Tree
    //a_LogOutput(1, "Tick Node");
    while (!m_lPrepareActionList.empty())
    {
        if ((*m_lPrepareActionList.begin())->GetStartTime() <= dwNowTime)
        {
            _InsertActiveAction((*m_lPrepareActionList.begin()));
            m_lPrepareActionList.pop_front();
        }
        else
        {
            break;
        }
    }

    // Tick Active Action Tree and Remove the dead Actions
    return _TickActiveActionTree(dwNowTime, pObjectManager);
}

CAnmActionManager::~CAnmActionManager()
{
    m_lPrepareActionList.clear();
    _ClearActiveActionTree();
    m_ActiveActionTreeRoot.mpAction = 0;
    m_ActiveActionTreeRoot.mEndTime = 0;
    m_ActiveActionTreeRoot.mpLeftNode = 0;
    m_ActiveActionTreeRoot.mpRightNode = 0;
	std::map<unsigned int, CAnmActionGroup*>::iterator itMap;
    for (itMap=m_mGroupMap.begin(); itMap!=m_mGroupMap.end(); ++itMap)
    {
        delete itMap->second;
    }
    m_mGroupMap.clear();
}

void CAnmActionManager::_ClearActiveActionTree()
{
    if (m_ActiveActionTreeRoot.mpLeftNode)
    {
        _RemoveActiveActionNode(m_ActiveActionTreeRoot.mpLeftNode);
    }
    if (m_ActiveActionTreeRoot.mpRightNode)
    {
        _RemoveActiveActionNode(m_ActiveActionTreeRoot.mpRightNode);
    }
}

void CAnmActionManager::_RemoveActiveActionNode(ActionNode *&rpActionNode)
{
    if (rpActionNode)
    {
        if (rpActionNode->mpLeftNode)
        {
            _RemoveActiveActionNode(rpActionNode->mpLeftNode);
        }
        if (rpActionNode->mpRightNode)
        {
            _RemoveActiveActionNode(rpActionNode->mpRightNode);
        }
        //a_LogOutput(1, "Remove Node");
        delete rpActionNode;
        rpActionNode = 0;
    }
}

void CAnmActionManager::_InsertActiveAction(CAnmAction *pAction)
{
    if (pAction)
    {
        DWORD endTime = pAction->GetStartTime() + pAction->GetTotalTime();
        ActionNode **ppActionNode = 0;
        if (endTime < m_ActiveActionTreeRoot.mEndTime)
        {
            ppActionNode = &(m_ActiveActionTreeRoot.mpLeftNode);
        }
        else
        {
            ppActionNode = &(m_ActiveActionTreeRoot.mpRightNode);
        }

        while (*ppActionNode)
        {
            if (endTime < (*ppActionNode)->mEndTime)
            {
                ppActionNode = &((*ppActionNode)->mpLeftNode);
            }
            else
            {
                ppActionNode = &((*ppActionNode)->mpRightNode);
            }
        }

        if (*ppActionNode == 0)
        {
            *ppActionNode = new ActionNode();
            (*ppActionNode)->mpAction = pAction;
            (*ppActionNode)->mEndTime = endTime;
            (*ppActionNode)->mpLeftNode = 0;
            (*ppActionNode)->mpRightNode = 0;
        }
    }
}

bool CAnmActionManager::_TickActiveActionTree(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    bool flag = false;
    if (m_ActiveActionTreeRoot.mpLeftNode)
    {
        if (_TickActiveActionNode(m_ActiveActionTreeRoot.mpLeftNode, dwNowTime, pObjectManager))
        {
            flag = true;
        }
    }
    if (m_ActiveActionTreeRoot.mpRightNode)
    {
        if (_TickActiveActionNode(m_ActiveActionTreeRoot.mpRightNode, dwNowTime, pObjectManager))
        {
            flag = true;
        }
    }
    return flag;
}

bool CAnmActionManager::_TickActiveActionNode(ActionNode *&rpActionNode, const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    bool flag = false;
    bool removeActionNode = false;
    if (rpActionNode)
    {
        if (rpActionNode->mpLeftNode)
        {
            if (_TickActiveActionNode(rpActionNode->mpLeftNode, dwNowTime, pObjectManager))
            {
                flag = true;
            }
        }
        if (rpActionNode->mpAction)
        {
            if (!g_Animator.IsDisabledCamera() || rpActionNode->mpAction->GetObjectID() != 0)
            {
                rpActionNode->mpAction->Tick(dwNowTime, pObjectManager);
            }
            else
            {
                if (dwNowTime > rpActionNode->mEndTime)
                {
                    rpActionNode->mpAction->SetDead(true);
                }
            }
            removeActionNode = rpActionNode->mpAction->IsDead();
            if (!removeActionNode)
            {
                flag = true;
            }
        }
        if (rpActionNode->mpRightNode)
        {
            if (_TickActiveActionNode(rpActionNode->mpRightNode, dwNowTime, pObjectManager))
            {
                flag = true;
            }
        }

        if (removeActionNode)
        {
            _RemoveActiveActionNode(rpActionNode->mpLeftNode);
            ActionNode *pActionNode = rpActionNode->mpRightNode;
            delete rpActionNode;
            rpActionNode = pActionNode;
        }
    }
    return flag;
}

bool CAnmActionManager::ActionCompareStartTime(const CAnmAction * const &a, const CAnmAction * const &b)
{
	return a->GetStartTime() < b->GetStartTime();
} 