#ifndef _ANM_BEZIER_H_
#define _ANM_BEZIER_H_

#include <vector>
#include <list>
#include <map>
#include "AnmBezierSegment.h"

static const int SEGMENT_LINK_FRACTION = 6;

class CAnmBezier
{
    public:
        explicit CAnmBezier(A3DWireCollector *pA3DWireCollector);

        A3DVECTOR3 Bezier(const DWORD time);

        A3DQUATERNION Orientation(const DWORD time);

        float FOV(const DWORD time);

        void GetSlerp(const DWORD time, A3DVECTOR3 &p, A3DQUATERNION &q, float &f);

        void Render();

        float GetLength();

        DWORD GetTotalTime() const;

        void AddPoint(const A3DVECTOR3 &v);

        void InsertPoint(const unsigned int uiID);

        void Move(const A3DVECTOR3 &v);

        void RemovePoint(CAnmBezierPoint *pPoint=0);

        void CopyTo(CAnmBezier *pBezier);

        CAnmBezierPoint* GetPoint(const unsigned int uiID);

        CAnmBezierPoint* GetFirstPoint();

        CAnmBezierSegment* GetSegment(const unsigned int uiID);

        CAnmBezierPoint* GetSelectedPoint() const;

        CAnmBezierSegment* GetSelectedSegment() const;

        int GetSelectedSegmentAnchor() const;

        void SelectPoint(const unsigned int uiID);

        void SelectSegment(const unsigned int uiID);

        void SelectSegmentAnchor(const int anchor);

        void GetPointIDList(std::vector<unsigned int> &vList);

        void GetSegmentIDList(std::vector<unsigned int> &vList);

        void SetID(const unsigned int uiID);

        unsigned int GetID() const;

        void SetDisplayName(const ACString &name);

        ACString GetDisplayName() const;

        int GetPointAmount() const;

        void SetPointTotalFrames(CAnmBezierPoint *pPoint, const int a);

        void SetPointPos(CAnmBezierPoint *pPoint, const A3DVECTOR3 &v);

        void SetSegmentAnchorPos(CAnmBezierSegment *pSegment, const A3DVECTOR3 &v, const int anchor);

        void ReCalculateLength();

        void SetVisable(const bool b);

        bool IsVisable() const;

        ~CAnmBezier();

    private:
        CAnmBezier();

        CAnmBezier(const CAnmBezier&);

        CAnmBezier& operator = (const CAnmBezier&);

        unsigned int GetNewPointID();

        unsigned int GetNewSegmentID();

        CAnmBezierSegment* GetSegmentAndFractorFromTime(const DWORD time, float &u);

    private:
        std::list<CAnmBezierPoint*> m_lPointList;
        std::map<unsigned int, CAnmBezierPoint*> m_mPointMap;

        std::list<CAnmBezierSegment*> m_lSegmentList;
        std::map<unsigned int, CAnmBezierSegment*> m_mSegmentMap;
        //std::map<unsigned int, CAnmBezierSegment*>::iterator m_pSMIterator;

        A3DWireCollector *m_pA3DWireCollector;
        unsigned int m_uiID;
        float m_fLength;
        int m_iTotalFrames;
        bool m_bDirty;
        ACString m_sDisplayName;

        CAnmBezierPoint *m_pSelectedPoint;
        CAnmBezierSegment *m_pSelectedSegment;
        int m_iSelectedSegmentAnchor;

        bool m_bVisable;
};

#endif
