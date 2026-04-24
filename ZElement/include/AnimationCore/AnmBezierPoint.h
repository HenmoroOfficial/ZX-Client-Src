#ifndef _ANM_BEZIER_POINT_H_
#define _ANM_BEZIER_POINT_H_

#include <A3D.h>

class CAnmBezierSegment;

class CAnmBezierPoint
{
    public:
        explicit CAnmBezierPoint(A3DWireCollector *pA3DWireCollector);

        void Render();

        void SetID(const unsigned int uiID);

        unsigned int GetID() const;

        void SetPos(const A3DVECTOR3 &vPos);

        A3DVECTOR3 GetPos() const;

        void SetOrientation(const A3DQUATERNION &q);

        A3DQUATERNION GetOrientation() const;

        void YawDelta(const float a);

        void PitchDelta(const float a);

        void RollDelta(const float a);

        void SetTotalFrames(const int iFrame);

        int GetTotalFrames() const;

        void SetFOV(const float v);

        float GetFOV() const;

        A3DAABB GetAABB() const;

        void SetColor(const A3DCOLOR &color);

        A3DCOLOR GetColor() const;

        void SetPreSegment(CAnmBezierSegment *pSegment);

        CAnmBezierSegment* GetPreSegment() const;

        void SetNextSegment(CAnmBezierSegment *pSegment);

        CAnmBezierSegment* GetNextSegment() const;

        ~CAnmBezierPoint();

    private:
        CAnmBezierPoint();

        CAnmBezierPoint(const CAnmBezierPoint&);

        CAnmBezierPoint& operator = (const CAnmBezierPoint&);

    private:
        A3DWireCollector *m_pA3DWireCollector;
        unsigned int m_uiID;
        A3DVECTOR3 m_vPos;
        A3DCOLOR m_cColor;
        CAnmBezierSegment *m_pPreSegment;
        CAnmBezierSegment *m_pNextSegment;

        A3DQUATERNION m_qOrientation;
        A3DVECTOR3 m_aFrustum[8];
        int m_iTotalFrames;
        float m_fFOV;
};

#endif
