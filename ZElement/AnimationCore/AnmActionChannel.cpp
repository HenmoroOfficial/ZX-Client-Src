#include "stdafx.h"
#include "AnmActionChannel.h"

CAnmActionChannel::CAnmActionChannel(const unsigned int uiID)
{
    m_uiChannelID = uiID;
    m_uiObjectID = -1;
}

void CAnmActionChannel::SetChannelID(const unsigned int uiID)
{
    m_uiChannelID = uiID;
}

unsigned int CAnmActionChannel::GetChannelID() const
{
    return m_uiChannelID;
}

void CAnmActionChannel::BindObjectID(const unsigned int uiID)
{
    m_uiObjectID = uiID;
	std::map<unsigned int, CAnmAction*>::iterator itMap;
    for (itMap=m_mActionMap.begin(); itMap!=m_mActionMap.end(); ++itMap)
    {
        itMap->second->BindObjectID(uiID);
    }
}

unsigned int CAnmActionChannel::GetObjectID() const
{
    return m_uiObjectID;
}

void CAnmActionChannel::SetChannelName(const ACString name)
{
    m_sChannelName = name;
}

ACString CAnmActionChannel::GetChannelName() const
{
    return m_sChannelName;
}

void CAnmActionChannel::CreateAction(const unsigned int uiID, const CAnmAction::ActionType type)
{
    CAnmAction *pAction;
    switch (type)
    {
        case CAnmAction::TYPE_MOVE:
        {
            pAction = new CAnmMove(uiID);
            break;
        }

        case CAnmAction::TYPE_SCALE:
        {
            pAction = new CAnmScale(uiID);
            break;
        }

        case CAnmAction::TYPE_JUMP:
        {
            pAction = new CAnmJump(uiID);
            break;
        }

        case CAnmAction::TYPE_BEZIER_MOVE:
        {
            pAction = new CAnmBezierMove(uiID);
            break;
        }

        case CAnmAction::TYPE_MOTION:
        {
            pAction = new CAnmMotion(uiID);
            break;
        }

        case CAnmAction::TYPE_DISPLAY_HIDE:
        {
            pAction = new CAnmDisplayHide(uiID);
            break;
        }

        case CAnmAction::TYPE_DIALOGUE:
        {
            pAction = new CAnmDialogue(uiID);
            break;
        }

        case CAnmAction::TYPE_SOUND:
        {
            pAction = new CAnmSound(uiID);
            break;
        }

        case CAnmAction::TYPE_HOOK:
        {
            pAction = new CAnmHook(uiID);
            break;
        }

        case CAnmAction::TYPE_SPEED_CONTROL:
        {
            pAction = new CAnmSpeedControl(uiID);
            break;
        }
		case CAnmAction::TYPE_CAMERA_BIND:
		{
		    pAction = new CAnmCameraBind(uiID);	
			break;
		}
        case CAnmAction::TYPE_CAMERA_FILTER:
        {
            pAction = new CAnmCameraFilter(uiID);
            break;
        }

        case CAnmAction::TYPE_CAMERA_OSCILLATORY:
        {
            pAction = new CAnmCameraOscillatory(uiID);
            break;
        }

        case CAnmAction::TYPE_TRANSPARENT:
        {
            pAction = new CAnmTransparent(uiID);
            break;
        }

        case CAnmAction::TYPE_HIDE_WORLD_ACTION:
        {
            pAction = new CAnmHideWorldAction(uiID);
            break;
        }

		case CAnmAction::TYPE_ROTATE:
		{
			pAction = new CAnmRotate(uiID);
			break;
		}

		case CAnmAction::TYPE_FOV:
		{
			pAction = new CAnmFOV(uiID);
			break;
		}

        default:
            break;
    }
    m_mActionMap[uiID] = pAction;
}

unsigned int CAnmActionChannel::CreateAction(const CAnmAction::ActionType type)
{
    unsigned int uiID;
	std::map<unsigned int, CAnmAction*>::iterator itMap;
    for (unsigned int i=1; i<16777215; ++i)
    {
        itMap = m_mActionMap.find(i);
        if (itMap == m_mActionMap.end())
        {
            uiID = i;
            break;
        }
    }
    CreateAction(uiID, type);
    return uiID;
}

int CAnmActionChannel::GetActionsCount(const DWORD dwStartTime, const DWORD dwEndTime)
{
    int count = 0;
	std::map<unsigned int, CAnmAction*>::iterator itMap;
    for (itMap=m_mActionMap.begin(); itMap!=m_mActionMap.end(); ++itMap)
    {
        DWORD actionTime = itMap->second->GetStartTime();
        if (dwStartTime <= actionTime && actionTime <= dwEndTime)
        {
            ++count;
        }
    }
    return count;
}

void CAnmActionChannel::MoveActions(const DWORD dwStartTime, const DWORD dwEndTime, const DWORD dwOffset, const bool left)
{
    if (left && dwStartTime < dwOffset)
    {
        return;
    }
    std::vector<unsigned int> deleteList;
    DWORD deleteStartTime = left ? (dwStartTime - dwOffset) : (dwStartTime + dwOffset);
    DWORD deleteEndTime = left ? (dwEndTime - dwOffset) : (dwEndTime + dwOffset);
	std::map<unsigned int, CAnmAction*>::iterator itMap;
    for (itMap=m_mActionMap.begin(); itMap!=m_mActionMap.end(); ++itMap)
    {
        DWORD actionTime = itMap->second->GetStartTime();
        if ((deleteStartTime <= actionTime && actionTime <= deleteEndTime) &&
            (actionTime < dwStartTime || dwEndTime < actionTime))
        {
            deleteList.push_back(itMap->first);
        }
        if (dwStartTime <= actionTime && actionTime <= dwEndTime)
        {
            if (left)
            {
                itMap->second->SetStartTime(actionTime - dwOffset);
            }
            else
            {
                itMap->second->SetStartTime(actionTime + dwOffset);
            }
        }
    }
    for (size_t i=0; i<deleteList.size(); ++i)
    {
        DeleteAction(deleteList[i]);
    }
}

void CAnmActionChannel::DeleteAction(const unsigned int uiID)
{
    std::map<unsigned int, CAnmAction*>::iterator itMap = m_mActionMap.find(uiID);
    if (itMap != m_mActionMap.end())
    {
        delete itMap->second;
        m_mActionMap.erase(itMap);
    }
}

CAnmAction* CAnmActionChannel::GetAction(const unsigned int uiID)
{
    std::map<unsigned int, CAnmAction*>::iterator itMap = m_mActionMap.find(uiID);
    if (itMap != m_mActionMap.end())
    {
        return itMap->second;
    }
    return 0;
}

void CAnmActionChannel::GetActionIDList(std::vector<unsigned int> &vList)
{
    vList.clear();
	std::map<unsigned int, CAnmAction*>::iterator itMap;
    for (itMap=m_mActionMap.begin(); itMap!=m_mActionMap.end(); ++itMap)
    {
        vList.push_back(itMap->first);
    }
}

int CAnmActionChannel::GetActionAmount() const
{
    return m_mActionMap.size();
}

unsigned int CAnmActionChannel::GetChannelType() const
{
    return 2;
}

void CAnmActionChannel::CopyDataFrom(CAnmActionChannel *pChannel)
{
    if (pChannel)
    {
        std::vector<unsigned int> vList;
        pChannel->GetActionIDList(vList);
        for (size_t i=0; i<vList.size(); ++i)
        {
            CAnmAction *pAction = pChannel->GetAction(vList[i]);
            if (pAction)
            {
                unsigned int uiID = CreateAction(pAction->GetActionType());
                CAnmAction *pMyAction = GetAction(uiID);
                if (pMyAction)
                {
                    pMyAction->CopyDataFrom(pAction);
                    pMyAction->SetStartTime(pAction->GetStartTime());
                }
            }
        }
        SetChannelName(pChannel->GetChannelName());
        BindObjectID(pChannel->GetObjectID());
    }
}

void CAnmActionChannel::Reset()
{
	std::map<unsigned int, CAnmAction*>::iterator itMap;
    for (itMap=m_mActionMap.begin(); itMap!=m_mActionMap.end(); ++itMap)
    {
        itMap->second->Reset();
    }
}

bool CAnmActionChannel::Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager)
{
    bool flag = false;
	std::map<unsigned int, CAnmAction*>::iterator itMap;
    for (itMap=m_mActionMap.begin(); itMap!=m_mActionMap.end(); ++itMap)
    {
        if (itMap->second->Tick(dwNowTime, pObjectManager))
        {
            flag = true;
        }
    }
    return flag;
}

CAnmActionChannel::~CAnmActionChannel()
{
	std::map<unsigned int, CAnmAction*>::iterator itMap;
    for (itMap=m_mActionMap.begin(); itMap!=m_mActionMap.end(); ++itMap)
    {
        delete itMap->second;
    }
    m_mActionMap.clear();
}
