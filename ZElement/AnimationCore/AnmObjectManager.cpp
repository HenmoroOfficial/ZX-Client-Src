#include "stdafx.h"
#include "Animator.h"
#include "AnmObjectManager.h"

CAnmObjectManager::CAnmObjectManager()
{
    m_pCamera = 0;
    m_pPlayer = 0;
    m_pSelectedObject = 0;
}

void CAnmObjectManager::SetCamera(CAnmCamera *pCamera)
{
    m_pCamera = pCamera;
}

void CAnmObjectManager::SetPlayer(CAnmPlayer *pPlayer)
{
    m_pPlayer = pPlayer;
}

void CAnmObjectManager::CreateObject(const unsigned int uiID, const CAnmObject::ObjectType type)
{
    switch (type)
    {
        case CAnmObject::TYPE_NPC:
        {
            CAnmObject *pObject = new CAnmNPC(uiID);
            m_mObjectMap[uiID] = pObject;
            break;
        }

        case CAnmObject::TYPE_GFX:
        {
            CAnmObject *pObject = new CAnmGFX(uiID);
            m_mObjectMap[uiID] = pObject;
            break;
        }

        default:
            break;
    }
}

unsigned int CAnmObjectManager::CreateObject(const CAnmObject::ObjectType type)
{
    unsigned int uiID;
	std::map<unsigned int, CAnmObject*>::iterator itMap;
    for (unsigned int i=2; i<65535; ++i)
    {
        itMap = m_mObjectMap.find(i);
        if (itMap == m_mObjectMap.end())
        {
            uiID = i;
            break;
        }
    }
    CreateObject(uiID, type);
    return uiID;
}

unsigned int CAnmObjectManager::CopyObject(const unsigned int uiID)
{
    if (uiID < 2)
    {
        return 0;
    }
    CAnmObject *pObject = GetObject(uiID);
    if (!pObject)
    {
        return 0;
    }
    unsigned int uiCopyObjectID = CreateObject(pObject->GetObjectType());
    CAnmObject *pCopyObject = GetObject(uiCopyObjectID);
    if (pCopyObject)
    {
        pCopyObject->CopyDataFrom(pObject);
        return uiCopyObjectID;
    }
    return 0;
}

void CAnmObjectManager::RemoveObject(const unsigned int uiID)
{
    std::map<unsigned int, CAnmObject*>::iterator itMap = m_mObjectMap.find(uiID);
    if (itMap != m_mObjectMap.end())
    {
        if (itMap->second == m_pSelectedObject)
        {
            m_pSelectedObject = 0;
        }
        delete itMap->second;
        m_mObjectMap.erase(itMap);
    }
}

CAnmObject* CAnmObjectManager::GetObject(const unsigned int uiID)
{
    if (uiID == 0)
    {
        return m_pCamera;
    }

    if (uiID == 1)
    {
        return m_pPlayer;
    }

    std::map<unsigned int, CAnmObject*>::iterator itMap = m_mObjectMap.find(uiID);
    if (itMap != m_mObjectMap.end())
    {
        return itMap->second;
    }
    return 0;
}

CAnmObject* CAnmObjectManager::GetSelectedObject() const
{
    return m_pSelectedObject;
}

void CAnmObjectManager::SelectObject(const unsigned int uiID)
{
    m_pSelectedObject = GetObject(uiID);
}

void CAnmObjectManager::GetObjectIDList(std::vector<unsigned int> &vList)
{
    vList.clear();
	std::map<unsigned int, CAnmObject*>::iterator itMap;
    for (itMap=m_mObjectMap.begin(); itMap!=m_mObjectMap.end(); ++itMap)
    {
        vList.push_back(itMap->first);
    }
}

int CAnmObjectManager::GetObjectAmount() const
{
    return m_mObjectMap.size();
}

bool CAnmObjectManager::Tick(const DWORD dwDeltaTime)
{
    bool flag = true;
    if (!m_pCamera->Tick(dwDeltaTime))
    {
        flag = false;
    }
    if (g_Animator.IsUseHostPlayer())
    {
        if (!m_pPlayer->Tick(dwDeltaTime))
        {
            flag = false;
        }
    }
	std::map<unsigned int, CAnmObject*>::iterator itMap;
    for (itMap=m_mObjectMap.begin(); itMap!=m_mObjectMap.end(); ++itMap)
    {
        if (!itMap->second->Tick(dwDeltaTime))
        {
            flag = false;
        }
    }
    return flag;
}

bool CAnmObjectManager::Render(A3DViewport *viewport, bool bRenderShadow)
{
    bool flag = true;
    if (!m_pCamera->Render())
    {
        flag = false;
    }
    if (g_Animator.IsUseHostPlayer())
    {
        if (!m_pPlayer->Render(viewport, bRenderShadow))
        {
            flag = false;
        }
    }
	std::map<unsigned int, CAnmObject*>::iterator itMap;
    for (itMap=m_mObjectMap.begin(); itMap!=m_mObjectMap.end(); ++itMap)
    {
        if (!itMap->second->Render(viewport, bRenderShadow))
        {
            flag = false;
        }
    }
    return flag;
}

void CAnmObjectManager::Reset()
{
	std::map<unsigned int, CAnmObject*>::iterator itMap;
    for (itMap=m_mObjectMap.begin(); itMap!=m_mObjectMap.end(); ++itMap)
    {
        itMap->second->Reset();
    }
}

CAnmObjectManager::~CAnmObjectManager()
{
    m_pSelectedObject = 0;
	std::map<unsigned int, CAnmObject*>::iterator itMap;
    for (itMap=m_mObjectMap.begin(); itMap!=m_mObjectMap.end(); ++itMap)
    {
        delete itMap->second;
    }
    m_mObjectMap.clear();
}
