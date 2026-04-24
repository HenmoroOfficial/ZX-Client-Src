#ifndef _ANM_RAY_TRACE_H_
#define _ANM_RAY_TRACE_H_

#include "Animator.h"

class CAnmRayTrace
{
    public:
        enum TargetType
        {
            TYPE_NONE = 0,
            TYPE_TERRAIN,
            TYPE_AXES,
            TYPE_OBJECT,
            TYPE_BEZIER_POINT,
            TYPE_BEZIER_SEGMENT_AHCHOR_A,
            TYPE_BEZIER_SEGMENT_AHCHOR_B,
        };

        enum FilterType
        {
            FILTER_ALL = 0xFFFF,
            FILTER_TERRAIN = 0x0001,
            FILTER_OBJECT = 0x0002,
            FILTER_BEZIER = 0x0004,
            FILTER_OBJECT_AND_TERRAIN = 0x0003,
            FILTER_BEZIER_AND_TERRAIN = 0x0005,
        };

    public:
        explicit CAnmRayTrace(CAnimator *pAnimator);

        bool RayTrace(const int x, const int y, const FilterType type=FILTER_ALL);

        bool RayTrace(const A3DVECTOR3 &vFrom, const A3DVECTOR3 &vDir, const FilterType type=FILTER_ALL);

        TargetType GetTargetType() const;

        A3DVECTOR3 GetHitPos() const;

        A3DVECTOR3 GetHitNormal() const;

        unsigned int GetTargetID() const;

        ~CAnmRayTrace();

    private:
        CAnmRayTrace();

        CAnmRayTrace(const CAnmRayTrace&);

        CAnmRayTrace& operator = (const CAnmRayTrace);

    private:
        CAnimator *m_pAnimator;
        TargetType m_eTargetType;
        float m_fFraction;
        A3DVECTOR3 m_vHitPos;
        A3DVECTOR3 m_vHitNormal;
        unsigned int m_uiTargetID;
};

#endif
