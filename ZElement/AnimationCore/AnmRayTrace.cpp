#include "stdafx.h"
#include "EC_World.h"
#include "EC_ManOrnament.h"
#include "AnmRayTrace.h"
#include "AnmObjectManager.h"
#include "AnmBezierManager.h"
#include "AnmAxes.h"

CAnmRayTrace::CAnmRayTrace(CAnimator *pAnimater)
{
    m_pAnimator = pAnimater;
    m_eTargetType = TYPE_NONE;
    m_fFraction = 0.0f;
    m_uiTargetID = -1;
}

bool CAnmRayTrace::RayTrace(const int x, const int y, const FilterType type)
{
    A3DVECTOR3 vFrom = m_pAnimator->GetA3DCamera()->GetPos();
    A3DVECTOR3 vTo(float(x), float(y), 1.0f);
    A3DViewport* pViewport = m_pAnimator->GetA3DViewport();
    pViewport->InvTransform(vTo, vTo);
    return RayTrace(vFrom, vTo-vFrom, type);
}

bool CAnmRayTrace::RayTrace(const A3DVECTOR3 &vFrom, const A3DVECTOR3 &vDir, const FilterType type)
{
    float fMinFraction = 1.1f;
    m_eTargetType = TYPE_NONE;
    m_uiTargetID = -1;

    if (type & FILTER_TERRAIN)
    {
        CECWorld *pWorld = m_pAnimator->GetWorld();
        if (pWorld)
        {
            A3DTerrain2 *pTerrain = pWorld->GetTerrain();
            if (pTerrain)
            {
                if (pWorld->GetOrnamentMan()->RayTrace(vFrom, vDir, m_vHitPos, &fMinFraction, m_vHitNormal))
                {
                    m_eTargetType = TYPE_TERRAIN;
                    m_fFraction = fMinFraction;
                }
                RAYTRACERT traceRT;
                if (pTerrain->RayTrace(vFrom, vDir, 1.0f, &traceRT) &&
                    traceRT.fFraction < fMinFraction)
                {
                    fMinFraction = traceRT.fFraction;
                    m_eTargetType = TYPE_TERRAIN;
                    m_fFraction = fMinFraction;
                    m_vHitNormal = traceRT.vNormal;
                }
            }
        }
    }

    if (type & FILTER_OBJECT)
    {
        A3DVECTOR3 vTempPos, vTempNormal;
        float fTempFraction;

        CAnmObjectManager *pObjectManager = m_pAnimator->GetObjectManager();
        std::vector<unsigned int> objectList;
        pObjectManager->GetObjectIDList(objectList);
        CAnmObject *pObject = pObjectManager->GetSelectedObject();
        if (pObject)
        {
            const A3DAABB aabb = pObject->GetAABB();
            if (CLS_RayToAABB3(vFrom, vDir, aabb.Mins, aabb.Maxs, vTempPos,
                &fTempFraction, vTempNormal) &&
                fTempFraction < fMinFraction)
            {
                fMinFraction = fTempFraction;
                m_eTargetType = TYPE_OBJECT;
                m_fFraction = fTempFraction;
                m_vHitNormal = vTempNormal;
                m_uiTargetID = pObject->GetObjectID();
			}
        }
        else
        {
            for (unsigned int i=0; i<=objectList.size(); ++i)
            {
                if (i == objectList.size())
                {
                    pObject = pObjectManager->GetObject(1);
                }
                else
                {
                    pObject = pObjectManager->GetObject(objectList[i]);
                }
                const A3DAABB aabb = pObject->GetAABB();
                if (CLS_RayToAABB3(vFrom, vDir, aabb.Mins, aabb.Maxs, vTempPos,
                                   &fTempFraction, vTempNormal) &&
                                   fTempFraction < fMinFraction)
			    {
                    fMinFraction = fTempFraction;
                    m_eTargetType = TYPE_OBJECT;
                    m_fFraction = fTempFraction;
                    m_vHitNormal = vTempNormal;
                    m_uiTargetID = pObject->GetObjectID();
                    break;
			    }
            }
        }
    }

    if (type & FILTER_BEZIER)
    {
        A3DVECTOR3 vTempPos, vTempNormal;
        float fTempFraction;

        CAnmBezierManager *pBezierManager = m_pAnimator->GetBezierManager();
        CAnmBezier *pBezier = pBezierManager->GetSelectedBezier();
        if (pBezier)
        {
            CAnmBezierPoint *pBezierPoint = pBezier->GetSelectedPoint();
            if (pBezierPoint)
            {
                const A3DAABB aabb = pBezierPoint->GetAABB();
                if (CLS_RayToAABB3(vFrom, vDir, aabb.Mins, aabb.Maxs,
                    vTempPos, &fTempFraction, vTempNormal) &&
                    fTempFraction < fMinFraction)
                {
                    fMinFraction = fTempFraction;
                    m_eTargetType = TYPE_BEZIER_POINT;
                    m_fFraction = fTempFraction;
                    m_vHitNormal = vTempNormal;
                    m_uiTargetID = pBezierPoint->GetID();
                }
            }
            else
            {
                std::vector<unsigned int> vList;
                pBezier->GetSegmentIDList(vList);
                CAnmBezierSegment *pBezierSegment;
                bool bSelected = false;
				unsigned int i;
                for (i=0; i<vList.size(); ++i)
                {
                    pBezierSegment = pBezier->GetSegment(vList[i]);
                    const A3DAABB aabbA = pBezierSegment->GetAnchorAAABB();
                    if (CLS_RayToAABB3(vFrom, vDir, aabbA.Mins, aabbA.Maxs,
                        vTempPos, &fTempFraction, vTempNormal) &&
                        fTempFraction < fMinFraction)
                    {
                        fMinFraction = fTempFraction;
                        m_eTargetType = TYPE_BEZIER_SEGMENT_AHCHOR_A;
                        m_fFraction = fTempFraction;
                        m_vHitNormal = vTempNormal;
                        m_uiTargetID = pBezierSegment->GetID();
                        bSelected = true;
                        break;
                    }
                    
                    const A3DAABB aabbB = pBezierSegment->GetAnchorBAABB();
                    if (CLS_RayToAABB3(vFrom, vDir, aabbB.Mins, aabbB.Maxs,
                        vTempPos, &fTempFraction, vTempNormal) &&
                        fTempFraction < fMinFraction)
                    {
                        fMinFraction = fTempFraction;
                        m_eTargetType = TYPE_BEZIER_SEGMENT_AHCHOR_B;
                        m_fFraction = fTempFraction;
                        m_vHitNormal = vTempNormal;
                        m_uiTargetID = pBezierSegment->GetID();
                        bSelected = true;
                        break;
                    }
                }

                if (!bSelected)
                {
                    pBezier->GetPointIDList(vList);
                    for (i=0; i<vList.size(); ++i)
                    {
                        pBezierPoint = pBezier->GetPoint(vList[i]);
                        const A3DAABB aabb = pBezierPoint->GetAABB();
                        if (CLS_RayToAABB3(vFrom, vDir, aabb.Mins, aabb.Maxs,
                                           vTempPos, &fTempFraction, vTempNormal) &&
                                           fTempFraction < fMinFraction)
                        {
                            fMinFraction = fTempFraction;
                            m_eTargetType = TYPE_BEZIER_POINT;
                            m_fFraction = fTempFraction;
                            m_vHitNormal = vTempNormal;
                            m_uiTargetID = pBezierPoint->GetID();
                            break;
                        }
                    }
                }
            }
        }
    }

    CAnmAxes *pAxes = m_pAnimator->GetAxes();
    if (pAxes->IsVisable())
    {
        A3DVECTOR3 vTempPos, vTempNormal;
        float fTempFraction;
        bool bAxesFlag = false;

        const A3DAABB aabbX = pAxes->GetAxisAABB(CAnmAxes::TYPE_X);
        if (CLS_RayToAABB3(vFrom, vDir, aabbX.Mins, aabbX.Maxs,
                           vTempPos, &fTempFraction, vTempNormal) &&
                           fTempFraction < fMinFraction)
        {
            fMinFraction = fTempFraction;
            m_eTargetType = TYPE_AXES;
            m_fFraction = fTempFraction;
            m_vHitNormal = vTempNormal;
            m_uiTargetID = -1;
            pAxes->SelectAxis(CAnmAxes::TYPE_X);
            bAxesFlag = true;
        }

        const A3DAABB aabbY = pAxes->GetAxisAABB(CAnmAxes::TYPE_Y);
        if (CLS_RayToAABB3(vFrom, vDir, aabbY.Mins, aabbY.Maxs,
                           vTempPos, &fTempFraction, vTempNormal) &&
                           fTempFraction < fMinFraction)
        {
            fMinFraction = fTempFraction;
            m_eTargetType = TYPE_AXES;
            m_fFraction = fTempFraction;
            m_vHitNormal = vTempNormal;
            m_uiTargetID = -1;
            pAxes->SelectAxis(CAnmAxes::TYPE_Y);
            bAxesFlag = true;
        }

        const A3DAABB aabbZ = pAxes->GetAxisAABB(CAnmAxes::TYPE_Z);
        if (CLS_RayToAABB3(vFrom, vDir, aabbZ.Mins, aabbZ.Maxs,
                           vTempPos, &fTempFraction, vTempNormal) &&
                           fTempFraction < fMinFraction)
        {
            fMinFraction = fTempFraction;
            m_eTargetType = TYPE_AXES;
            m_fFraction = fTempFraction;
            m_vHitNormal = vTempNormal;
            m_uiTargetID = -1;
            pAxes->SelectAxis(CAnmAxes::TYPE_Z);
            bAxesFlag = true;
        }
        if (!bAxesFlag)
        {
            pAxes->SelectAxis(CAnmAxes::TYPE_NONE);
        }
    }

    if (fMinFraction != 1.1f)
    {
        m_vHitPos = vFrom + (m_fFraction * vDir);
        return true;
    }

    return false;
}

CAnmRayTrace::TargetType CAnmRayTrace::GetTargetType() const
{
    return m_eTargetType;
}

A3DVECTOR3 CAnmRayTrace::GetHitPos() const
{
    return m_vHitPos;
}

A3DVECTOR3 CAnmRayTrace::GetHitNormal() const
{
    return m_vHitNormal;
}

unsigned int CAnmRayTrace::GetTargetID() const
{
    return m_uiTargetID;
}

CAnmRayTrace::~CAnmRayTrace()
{
    m_pAnimator = 0;
}
