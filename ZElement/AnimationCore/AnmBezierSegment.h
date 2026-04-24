#ifndef _ANM_BEZIER_SEGMENT_H_
#define _ANM_BEZIER_SEGMENT_H_

#include "AnmBezierPoint.h"

static const int CALCULATE_DIVIDE = 32;

class CAnmBezierSegment
{
    public:
        explicit CAnmBezierSegment(A3DWireCollector *pA3DWireCollector);

        A3DVECTOR3 Bezier(const float u);

        A3DQUATERNION Orientation(const float u);

        float FOV(const float u);

        void Render();

        float GetLength();

        DWORD GetTotalTime() const;

        int GetTotalFrames() const;

        void SetID(const unsigned int uiID);

        unsigned int GetID() const;

        void SetAnchorA(const A3DVECTOR3 &v);

        A3DVECTOR3 GetAnchorA() const;

        void SetAnchorB(const A3DVECTOR3 &v);

        A3DVECTOR3 GetAnchorB() const;

        void SetHead(CAnmBezierPoint *pPoint);

        CAnmBezierPoint* GetHead() const;

        void SetTail(CAnmBezierPoint *pPoint);

        CAnmBezierPoint* GetTail() const;

        A3DAABB GetAnchorAAABB() const;

        A3DAABB GetAnchorBAABB() const;

        void ReCalculateLength();

        ~CAnmBezierSegment();

    private:
        CAnmBezierSegment();

        CAnmBezierSegment(const CAnmBezierSegment&);

        CAnmBezierSegment& operator = (const CAnmBezierSegment&);

    private:
        A3DWireCollector *m_pA3DWireCollector;
        unsigned int m_uiID;
        A3DVECTOR3 m_vAnchorA;
        A3DVECTOR3 m_vAnchorB;
        CAnmBezierPoint *m_pHead;
        CAnmBezierPoint *m_pTail;

        A3DCOLOR m_cSegmentColor;
        A3DCOLOR m_cAnchorColor;
        A3DVECTOR3 m_vTempPosArray[CALCULATE_DIVIDE+1];

        float m_fLength;
        int m_iTotalFrames;
        bool m_bDirty;
};

#endif
