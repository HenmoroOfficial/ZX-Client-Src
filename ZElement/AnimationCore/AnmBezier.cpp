#include "stdafx.h"
#include "Animator.h"
#include "AnmAction.h"
#include "AnmBezier.h"

CAnmBezier::CAnmBezier(A3DWireCollector *pA3DWireCollector)
{
    m_pA3DWireCollector = pA3DWireCollector;
    m_uiID = -1;
    m_fLength = 0.0f;
    m_iTotalFrames = 0;
    m_bDirty = true;
    m_sDisplayName = L"Bezier Path";
    m_pSelectedPoint = 0;
    m_pSelectedSegment = 0;
    m_iSelectedSegmentAnchor = 0;
    m_bVisable = false;
}

A3DVECTOR3 CAnmBezier::Bezier(const DWORD time)
{
    float u;
    CAnmBezierSegment *pSegment = GetSegmentAndFractorFromTime(time, u);
    if (pSegment)
    {
        return pSegment->Bezier(u);
    }
    if (!m_lPointList.empty())
    {
        return m_lPointList.front()->GetPos();
    }
    return A3DVECTOR3(0.0f);
}

A3DQUATERNION CAnmBezier::Orientation(const DWORD time)
{
    float u;
    CAnmBezierSegment *pSegment = GetSegmentAndFractorFromTime(time, u);
    if (pSegment)
    {
        //float k = 0.5f * float(sin(u * PI - 0.5f * PI)) + 0.5f;
        return pSegment->Orientation(u);
    }
    if (!m_lPointList.empty())
    {
        return m_lPointList.front()->GetOrientation();
    }
    return A3DQUATERNION(1.0f, 0.0f, 0.0f, 0.0f);
}

float CAnmBezier::FOV(const DWORD time)
{
    float u;
    CAnmBezierSegment *pSegment = GetSegmentAndFractorFromTime(time, u);
    if (pSegment)
    {
        return pSegment->FOV(u);
    }
    if (!m_lPointList.empty())
    {
        return m_lPointList.front()->GetFOV();
    }
    return 0.0f;
}

void CAnmBezier::GetSlerp(const DWORD time, A3DVECTOR3 &p, A3DQUATERNION &q, float &f)
{
    float u;
    CAnmBezierSegment *pSegment = GetSegmentAndFractorFromTime(time, u);
    if (pSegment)
    {
        p = pSegment->Bezier(u);
        //float k = 0.5f * float(sin(u * PI - 0.5f * PI)) + 0.5f;
        q = pSegment->Orientation(u);
        f = pSegment->FOV(u);
        return;
    }
    if (!m_lPointList.empty())
    {
        p = m_lPointList.front()->GetPos();
        q = m_lPointList.front()->GetOrientation();
        f = m_lPointList.front()->GetFOV();
        return;
    }
    p = A3DVECTOR3(0.0f);
    q = A3DQUATERNION(1.0f, 0.0f, 0.0f, 0.0f);
    f = 0.0f;
}

void CAnmBezier::Render()
{
    if (m_bDirty)
    {
        ReCalculateLength();
    }

	std::list<CAnmBezierSegment*>::iterator itSL;
    for (itSL=m_lSegmentList.begin(); itSL!=m_lSegmentList.end(); ++itSL)
    {
        (*itSL)->Render();
    }
	std::list<CAnmBezierPoint*>::iterator itPL;
    for (itPL=m_lPointList.begin(); itPL!=m_lPointList.end(); ++itPL)
    {
        (*itPL)->Render();
    }

    if (GetPointAmount())
    {
        A3DVECTOR3 vPos = GetFirstPoint()->GetPos();
        A3DVECTOR3 vPosIn = vPos+A3DVECTOR3(0.0f, 0.1f, 0.0f);
        A3DVECTOR3 vPosOut;
        g_Animator.GetA3DViewport()->Transform(vPosIn, vPosOut);
        g_Animator.GetA3DEngine()->GetA3DFlatCollector()->AddRect_2D(int(vPosOut.x-20), int(vPosOut.y-5),
            int(vPosOut.x+20), int(vPosOut.y+6), 0x7F0000FF, vPosOut.z);
        ACString number;
        number.Format(_AL("BID:%u"), m_uiID);
        g_Animator.GetA3DEngine()->GetSystemFont()->TextOut3D(number,
            g_Animator.GetA3DViewport(), vPosIn, 0, A3DCOLORRGB(255, 255, 255));
    }
}

float CAnmBezier::GetLength()
{
    if (m_bDirty)
    {
        ReCalculateLength();
    }
    return m_fLength;
}

DWORD CAnmBezier::GetTotalTime() const
{
    return m_iTotalFrames * CAnmAction::TIME_PER_FRAME;
}

void CAnmBezier::AddPoint(const A3DVECTOR3 &v)
{
    CAnmBezierPoint *pPoint = new CAnmBezierPoint(m_pA3DWireCollector);

    pPoint->SetPos(v);

    if (!m_lPointList.empty())
    {
        CAnmBezierSegment *pSegment = new CAnmBezierSegment(m_pA3DWireCollector);
        CAnmBezierPoint *pLastPoint = m_lPointList.back();
        pLastPoint->SetNextSegment(pSegment);
        pSegment->SetHead(pLastPoint);
        pSegment->SetTail(pPoint);
        pPoint->SetPreSegment(pSegment);

        A3DVECTOR3 vLastPointPos = pLastPoint->GetPos();

        if (m_lSegmentList.empty())
        {
            pSegment->SetAnchorA(vLastPointPos + (v-vLastPointPos)/SEGMENT_LINK_FRACTION);
        }
        else
        {
            CAnmBezierSegment *pLastSegment = m_lSegmentList.back();
            A3DVECTOR3 vLastSegmentHead = pLastSegment->GetHead()->GetPos();
            A3DVECTOR3 vD = (v - vLastSegmentHead)/SEGMENT_LINK_FRACTION;

            pLastSegment->SetAnchorB(vLastPointPos - vD);
            pSegment->SetAnchorA(vLastPointPos + vD);
        }
        pSegment->SetAnchorB(v + (vLastPointPos-v)/SEGMENT_LINK_FRACTION);
        unsigned int uiID = GetNewSegmentID();
        pSegment->SetID(uiID);
        m_lSegmentList.push_back(pSegment);
        m_mSegmentMap[uiID] = pSegment;
    }
    unsigned int uiID = GetNewPointID();
    pPoint->SetID(uiID);
    m_lPointList.push_back(pPoint);
    m_mPointMap[uiID] = pPoint;
    m_bDirty = true;
}

void CAnmBezier::InsertPoint(const unsigned int uiID)
{
    CAnmBezierPoint *pPrePoint = GetPoint(uiID);
    if (pPrePoint)
    {
        CAnmBezierSegment *pSegment = pPrePoint->GetNextSegment();
        if (pSegment)
        {
            A3DVECTOR3 vPos = pSegment->Bezier(0.5f);
            A3DQUATERNION qOrientation = pSegment->Orientation(0.5f);
            CAnmBezierPoint *pNewPoint = new CAnmBezierPoint(m_pA3DWireCollector);
            unsigned int uiID = GetNewPointID();
            pNewPoint->SetID(uiID);
            m_mPointMap[uiID] = pNewPoint;
            pNewPoint->SetPos(vPos);
            pNewPoint->SetOrientation(qOrientation);
            pNewPoint->SetPreSegment(pSegment);
            DWORD dwFrames = pPrePoint->GetTotalFrames() / 2;
            pNewPoint->SetTotalFrames(dwFrames);
            pPrePoint->SetTotalFrames(pPrePoint->GetTotalFrames() - dwFrames);
            CAnmBezierPoint *pNextPoint = pSegment->GetTail();
            CAnmBezierSegment *pNewSegment = new CAnmBezierSegment(m_pA3DWireCollector);
            uiID = GetNewSegmentID();
            pNewSegment->SetID(uiID);
            m_mSegmentMap[uiID] = pNewSegment;
            pNewPoint->SetNextSegment(pNewSegment);
            pNextPoint->SetPreSegment(pNewSegment);
            pNewSegment->SetHead(pNewPoint);
            pNewSegment->SetTail(pNextPoint);
            pNewSegment->SetAnchorB(pSegment->GetAnchorB());

            A3DVECTOR3 vOffset = (pSegment->Bezier(0.6f) - pSegment->Bezier(0.4f)) / SEGMENT_LINK_FRACTION;
            pSegment->SetTail(pNewPoint);
            pSegment->SetAnchorB(vPos - vOffset);
            pNewSegment->SetAnchorA(vPos + vOffset);

			std::list<CAnmBezierPoint*>::iterator itPL;
            for (itPL=m_lPointList.begin(); itPL!=m_lPointList.end(); ++itPL)
            {
                if ((*itPL)->GetID() == pPrePoint->GetID())
                {
                    itPL++;
                    m_lPointList.insert(itPL, pNewPoint);
                    break;
                }
            }

			std::list<CAnmBezierSegment*>::iterator itSL;
            for (itSL=m_lSegmentList.begin(); itSL!=m_lSegmentList.end(); ++itSL)
            {
                if ((*itSL)->GetID() == pSegment->GetID())
                {
                    itSL++;
                    m_lSegmentList.insert(itSL, pNewSegment);
                    break;
                }
            }
        }
    }
}

void CAnmBezier::Move(const A3DVECTOR3 &v)
{
	std::list<CAnmBezierPoint*>::iterator itPL;
    for (itPL=m_lPointList.begin(); itPL!=m_lPointList.end(); ++itPL)
    {
        A3DVECTOR3 pos = (*itPL)->GetPos();
        (*itPL)->SetPos(pos + v);
    }
	std::list<CAnmBezierSegment*>::iterator itSL;
    for (itSL=m_lSegmentList.begin(); itSL!=m_lSegmentList.end(); ++itSL)
    {
        A3DVECTOR3 pos = (*itSL)->GetAnchorA();
        (*itSL)->SetAnchorA(pos + v);
        pos = (*itSL)->GetAnchorB();
        (*itSL)->SetAnchorB(pos + v);
    }
    m_bDirty = true;
}

void CAnmBezier::RemovePoint(CAnmBezierPoint *pPoint)
{
    if (!pPoint)
    {
        if (m_lPointList.empty())
        {
            return;
        }

        pPoint = m_lPointList.back();

        if (m_pSelectedPoint == pPoint)
        {
            m_pSelectedPoint = 0;
        }

        CAnmBezierSegment *pSegment = pPoint->GetPreSegment();
        if (pSegment)
        {
            CAnmBezierPoint *pPrePoint = pSegment->GetHead();
            if (pPrePoint)
            {
                pPrePoint->SetNextSegment(0);
            }
            m_mSegmentMap.erase(pSegment->GetID());
            m_lSegmentList.pop_back();
            delete pSegment;
        }
        if (!m_lPointList.empty())
        {
            m_mPointMap.erase(pPoint->GetID());
            m_lPointList.pop_back();
            delete pPoint;
        }
        m_bDirty = true;
    }
    else
    {
        if (m_lPointList.empty())
        {
            return;
        }

        if (m_pSelectedPoint == pPoint)
        {
            m_pSelectedPoint = 0;
        }

        CAnmBezierSegment *pNextSegment = pPoint->GetNextSegment();
        if (pNextSegment)
        {
            CAnmBezierSegment *pPreSegment = pPoint->GetPreSegment();
            if (pPreSegment)
            {
                CAnmBezierPoint *pNextPoint = pNextSegment->GetTail();
                pPreSegment->SetTail(pNextPoint);
                pPreSegment->SetAnchorB(pNextSegment->GetAnchorB());
                pNextPoint->SetPreSegment(pPreSegment);
            }
            else
            {
                CAnmBezierPoint *pNextPoint = pNextSegment->GetTail();
                pNextPoint->SetPreSegment(0);
            }
            m_mPointMap.erase(pPoint->GetID());
            m_mSegmentMap.erase(pNextSegment->GetID());
            m_lPointList.remove(pPoint);
            m_lSegmentList.remove(pNextSegment);
            delete pPoint;
            delete pNextSegment;
        }
        else
        {
            RemovePoint();
            return;
        }
    }
}

void CAnmBezier::CopyTo(CAnmBezier *pBezier)
{
    if (pBezier)
    {
        pBezier->SetDisplayName(GetDisplayName() + ACString(_AL("Copy")));
        // Copy the points
		std::list<CAnmBezierPoint*>::iterator itPL;
        for (itPL=m_lPointList.begin(); itPL!=m_lPointList.end(); ++itPL)
        {
            CAnmBezierPoint *pPoint = *itPL;
            pBezier->AddPoint(pPoint->GetPos());
        }

        // Copy the points data
        std::vector<unsigned int> vList;
        pBezier->GetPointIDList(vList);
        itPL = m_lPointList.begin();
		unsigned int i;
        for (i=0; i<vList.size(); ++i)
        {
            CAnmBezierPoint *pPoint = (*itPL);
            CAnmBezierPoint *pNewPoint = pBezier->GetPoint(vList[i]);
            if (pPoint && pNewPoint)
            {
                pNewPoint->SetOrientation(pPoint->GetOrientation());
                pNewPoint->SetFOV(pPoint->GetFOV());
                pNewPoint->SetTotalFrames(pPoint->GetTotalFrames());
            }
            ++itPL;
        }

        // Copy the segments data
        pBezier->GetSegmentIDList(vList);
        std::list<CAnmBezierSegment*>::iterator itSL = m_lSegmentList.begin();
        for (i=0; i<vList.size(); ++i)
        {
            CAnmBezierSegment *pSegment = (*itSL);
            CAnmBezierSegment *pNewSegment = pBezier->GetSegment(vList[i]);
            if (pSegment && pNewSegment)
            {
                pNewSegment->SetAnchorA(pSegment->GetAnchorA());
                pNewSegment->SetAnchorB(pSegment->GetAnchorB());
            }
            ++itSL;
        }
    }
}

CAnmBezierPoint* CAnmBezier::GetPoint(const unsigned int uiID)
{
    std::map<unsigned int, CAnmBezierPoint*>::iterator itPM = m_mPointMap.find(uiID);
    if (itPM != m_mPointMap.end())
    {
        return itPM->second;
    }
    return 0;
}

CAnmBezierPoint* CAnmBezier::GetFirstPoint()
{
    if (!m_lPointList.empty())
    {
        return m_lPointList.front();
    }
    return 0;
}

CAnmBezierSegment* CAnmBezier::GetSegment(const unsigned int uiID)
{
    std::map<unsigned int, CAnmBezierSegment*>::iterator itSM = m_mSegmentMap.find(uiID);
    if (itSM != m_mSegmentMap.end())
    {
        return itSM->second;
    }
    return 0;
}

CAnmBezierPoint* CAnmBezier::GetSelectedPoint() const
{
    return m_pSelectedPoint;
}

CAnmBezierSegment* CAnmBezier::GetSelectedSegment() const
{
    return m_pSelectedSegment;
}

int CAnmBezier::GetSelectedSegmentAnchor() const
{
    return m_iSelectedSegmentAnchor;
}

void CAnmBezier::SelectPoint(const unsigned int uiID)
{
    m_pSelectedPoint = GetPoint(uiID);
}

void CAnmBezier::SelectSegment(const unsigned int uiID)
{
    m_pSelectedSegment = GetSegment(uiID);
}

void CAnmBezier::SelectSegmentAnchor(const int anchor)
{
    if (anchor >= 0 && anchor < 3)
    {
        m_iSelectedSegmentAnchor = anchor;
    }
}

void CAnmBezier::GetPointIDList(std::vector<unsigned int> &vList)
{
    vList.clear();
	std::list<CAnmBezierPoint*>::iterator itPL;
    for (itPL=m_lPointList.begin(); itPL!=m_lPointList.end(); ++itPL)
    {
        vList.push_back((*itPL)->GetID());
    }
}

void CAnmBezier::GetSegmentIDList(std::vector<unsigned int> &vList)
{
    vList.clear();
	std::list<CAnmBezierSegment*>::iterator itSL;
    for (itSL=m_lSegmentList.begin(); itSL!=m_lSegmentList.end(); ++itSL)
    {
        vList.push_back((*itSL)->GetID());
    }
}

void CAnmBezier::SetID(const unsigned int uiID)
{
    m_uiID = uiID;
}

unsigned int CAnmBezier::GetID() const
{
    return m_uiID;
}

void CAnmBezier::SetDisplayName(const ACString &name)
{
    m_sDisplayName = name;
}

ACString CAnmBezier::GetDisplayName() const
{
    return m_sDisplayName;
}

int CAnmBezier::GetPointAmount() const
{
    return m_lPointList.size();
}

void CAnmBezier::SetPointTotalFrames(CAnmBezierPoint *pPoint, const int a)
{
    pPoint->SetTotalFrames(a);
    CAnmBezierSegment *pSegment;
    pSegment = pPoint->GetNextSegment();
    if (pSegment)
    {
        pSegment->SetHead(pPoint);
    }
    m_bDirty = true;
}

void CAnmBezier::SetPointPos(CAnmBezierPoint *pPoint, const A3DVECTOR3 &v)
{
    A3DVECTOR3 dv = v - pPoint->GetPos();
    pPoint->SetPos(v);
    CAnmBezierSegment *pSegment;
    pSegment = pPoint->GetPreSegment();
    if (pSegment)
    {
        pSegment->SetAnchorB(pSegment->GetAnchorB() + dv);
    }
    pSegment = pPoint->GetNextSegment();
    if (pSegment)
    {
        pSegment->SetAnchorA(pSegment->GetAnchorA() + dv);
    }

    m_bDirty = true;
}

void CAnmBezier::SetSegmentAnchorPos(CAnmBezierSegment *pSegment, const A3DVECTOR3 &v, const int anchor)
{
    A3DVECTOR3 dv = v;
    CAnmBezierSegment *pOtherSegment;
    if (anchor == 1)
    {
        dv -= pSegment->GetAnchorA();
        pSegment->SetAnchorA(v);
        pOtherSegment = pSegment->GetHead()->GetPreSegment();
        if (pOtherSegment)
        {
            pOtherSegment->SetAnchorB(pOtherSegment->GetAnchorB() - dv);
        }
    }
    else if (anchor == 2)
    {
        dv -= pSegment->GetAnchorB();
        pSegment->SetAnchorB(v);
        pOtherSegment = pSegment->GetTail()->GetNextSegment();
        if (pOtherSegment)
        {
            pOtherSegment->SetAnchorA(pOtherSegment->GetAnchorA() - dv);
        }
    }
    m_bDirty = true;
}

CAnmBezier::~CAnmBezier()
{
    m_pSelectedPoint = 0;
    m_pSelectedSegment = 0;
    m_iSelectedSegmentAnchor = 0;
	std::list<CAnmBezierSegment*>::iterator itSL;
    for (itSL=m_lSegmentList.begin(); itSL!=m_lSegmentList.end(); ++itSL)
    {
        delete (*itSL);
    }
    m_lSegmentList.clear();
    m_mSegmentMap.clear();

	std::list<CAnmBezierPoint*>::iterator itPL;
    for (itPL=m_lPointList.begin(); itPL!=m_lPointList.end(); ++itPL)
    {
        delete (*itPL);
    }
    m_lPointList.clear();
    m_mPointMap.clear();
}

void CAnmBezier::ReCalculateLength()
{
    m_fLength = 0.0f;
    m_iTotalFrames = 0;
	std::list<CAnmBezierSegment*>::iterator itSL;
    for (itSL=m_lSegmentList.begin(); itSL!=m_lSegmentList.end(); ++itSL)
    {
        (*itSL)->ReCalculateLength();
        m_fLength += (*itSL)->GetLength();
        m_iTotalFrames += (*itSL)->GetTotalFrames();
    }
    m_bDirty = false;
}

unsigned int CAnmBezier::GetNewPointID()
{
    unsigned int uiID;
	std::map<unsigned int, CAnmBezierPoint*>::iterator itPM;
    for (unsigned int i=0; i<65535; ++i)
    {
        itPM = m_mPointMap.find(i);
        if (itPM == m_mPointMap.end())
        {
            uiID = i;
            break;
        }
    }
    return uiID;
}

unsigned int CAnmBezier::GetNewSegmentID()
{
    unsigned int uiID;
	std::map<unsigned int, CAnmBezierSegment*>::iterator itSM;
    for (unsigned int i=0; i<65535; ++i)
    {
        itSM = m_mSegmentMap.find(i);
        if (itSM == m_mSegmentMap.end())
        {
            uiID = i;
            break;
        }
    }
    return uiID;
}

CAnmBezierSegment* CAnmBezier::GetSegmentAndFractorFromTime(const DWORD time, float &u)
{
    DWORD dwT = 0;
	std::list<CAnmBezierSegment*>::iterator itSL;
    for (itSL=m_lSegmentList.begin(); itSL!=m_lSegmentList.end(); ++itSL)
    {
        DWORD tempT = (*itSL)->GetTotalTime();
        if (dwT + tempT >= time)
        {
            u = float(time - dwT) / tempT;
            return (*itSL);
        }
        dwT += tempT;
    }
    return 0;
}

void CAnmBezier::SetVisable(const bool b)
{
    m_bVisable = b;
}

bool CAnmBezier::IsVisable() const
{
    return m_bVisable;
}
