#include "stdafx.h"
#include "AnmBezierManager.h"

CAnmBezierManager::CAnmBezierManager(A3DWireCollector *pA3DWireCollector)
{
    m_pA3DWireCollector = pA3DWireCollector;
    m_pSelectedBezier = 0;
}

void CAnmBezierManager::CreateBezier(const unsigned int uiID)
{
    CAnmBezier *pBezier = new CAnmBezier(m_pA3DWireCollector);
    pBezier->SetID(uiID);
    m_mBezierMap[uiID] = pBezier;
}

unsigned int CAnmBezierManager::CreateBezier()
{
    unsigned int uiID;
	std::map<unsigned int, CAnmBezier*>::iterator itBezier;
    for (unsigned int i=0; i<65535; ++i)
    {
        itBezier = m_mBezierMap.find(i);
        if (itBezier == m_mBezierMap.end())
        {
            uiID = i;
            break;
        }
    }
    CreateBezier(uiID);
    return uiID;
}

void CAnmBezierManager::CopyBezier(const unsigned int uiID)
{
    unsigned int newID = CreateBezier();
    CAnmBezier *pBezier = GetBezier(uiID);
    CAnmBezier *pNewBezier = GetBezier(newID);
    if (pBezier && pNewBezier)
    {
        pBezier->CopyTo(pNewBezier);
    }
}

void CAnmBezierManager::RemoveBezier(const unsigned int uiID)
{
    std::map<unsigned int, CAnmBezier*>::iterator itBezier = m_mBezierMap.find(uiID);
    if (itBezier != m_mBezierMap.end())
    {
        if (itBezier->second == m_pSelectedBezier)
        {
            m_pSelectedBezier = 0;
        }
        delete itBezier->second;
        m_mBezierMap.erase(itBezier);
    }
}

CAnmBezier* CAnmBezierManager::GetBezier(const unsigned int uiID)
{
    std::map<unsigned int, CAnmBezier*>::iterator itBezier = m_mBezierMap.find(uiID);
    if (itBezier != m_mBezierMap.end())
    {
        return itBezier->second;
    }
    return 0;
}

CAnmBezier* CAnmBezierManager::GetSelectedBezier() const
{
    return m_pSelectedBezier;
}

void CAnmBezierManager::SelectBezier(const unsigned int uiID)
{
    m_pSelectedBezier = GetBezier(uiID);
}

void CAnmBezierManager::GetBezierIDList(std::vector<unsigned int> &vList)
{
    vList.clear();
	std::map<unsigned int, CAnmBezier*>::iterator itBezier;
    for (itBezier=m_mBezierMap.begin(); itBezier!=m_mBezierMap.end(); ++itBezier)
    {
        vList.push_back(itBezier->first);
    }
}

void CAnmBezierManager::SetAllVisable(const bool b)
{
	std::map<unsigned int, CAnmBezier*>::iterator itBezier;
    for (itBezier=m_mBezierMap.begin(); itBezier!=m_mBezierMap.end(); ++itBezier)
    {
        itBezier->second->SetVisable(b);
    }
}

void CAnmBezierManager::Render()
{
	std::map<unsigned int, CAnmBezier*>::iterator itBezier;
    for (itBezier=m_mBezierMap.begin(); itBezier!=m_mBezierMap.end(); ++itBezier)
    {
        if (itBezier->second->IsVisable())
        {
            itBezier->second->Render();
        }
        else if (m_pSelectedBezier && itBezier->second == m_pSelectedBezier)
        {
            m_pSelectedBezier->Render();
        }
    }
}

int CAnmBezierManager::GetBezierAmount() const
{
    return m_mBezierMap.size();
}

CAnmBezierManager::~CAnmBezierManager()
{
	std::map<unsigned int, CAnmBezier*>::iterator itBezier;
    for (itBezier=m_mBezierMap.begin(); itBezier!=m_mBezierMap.end(); ++itBezier)
    {
        delete itBezier->second;
    }
    m_pSelectedBezier = 0;
}
