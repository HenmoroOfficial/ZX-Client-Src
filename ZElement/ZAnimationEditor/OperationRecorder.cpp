#include "stdafx.h"
#include "OperationRecorder.h"

COperationRecorder::COperationRecorder()
{
    m_pCurPos = m_lOperationList.end();
}

void COperationRecorder::RecordObject(const unsigned int id,
                                     const A3DVECTOR3 &p,
                                     const A3DVECTOR3 &d,
                                     const A3DVECTOR3 &u)
{
    ClearBackRecords();

    Operation *op = new Operation();
    op->target = OT_OBJECT;
    op->objectID = id;
    op->bezierID = 0;
    op->bezierPointID = 0;
    op->bezierSegmentID = 0;
    op->bezierAnchor = 0;
    op->position = p;
    op->direction = d;
    op->up = u;
    op->orientation = A3DQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);

    m_lOperationList.push_back(op);
    ClearExcessRecords();
    m_pCurPos = m_lOperationList.end();
}

void COperationRecorder::RecordBezierPoint(const unsigned int bID,
                                          const unsigned int pID,
                                          const A3DVECTOR3 &p,
                                          const A3DQUATERNION &q)
{
    ClearBackRecords();

    Operation *op = new Operation();
    op->target = OT_BEZIER_POINT;
    op->objectID = 0;
    op->bezierID = bID;
    op->bezierPointID = pID;
    op->bezierSegmentID = 0;
    op->bezierAnchor = 0;
    op->position = p;
    op->direction = A3DVECTOR3(0.0f);
    op->up = A3DVECTOR3(0.0f);
    op->orientation = q;

    m_lOperationList.push_back(op);
    ClearExcessRecords();
    m_pCurPos = m_lOperationList.end();
}

void COperationRecorder::RecordBezierAnchor(const unsigned int bID,
                                           const unsigned int sID,
                                           const int anchor,
                                           const A3DVECTOR3 &p)
{
    ClearBackRecords();

    Operation *op = new Operation();
    op->target = OT_BEZIER_ANCHOR;
    op->objectID = 0;
    op->bezierID = bID;
    op->bezierPointID = 0;
    op->bezierSegmentID = sID;
    op->bezierAnchor = anchor;
    op->position = p;
    op->direction = A3DVECTOR3(0.0f);
    op->up = A3DVECTOR3(0.0f);
    op->orientation = A3DQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);

    m_lOperationList.push_back(op);
    ClearExcessRecords();
    m_pCurPos = m_lOperationList.end();
}

bool COperationRecorder::CanUnDo() const
{
    if (m_lOperationList.size() > 0 && m_pCurPos != m_lOperationList.begin())
    {
        return true;
    }
    return false;
}

bool COperationRecorder::CanReDo() const
{
    if (m_pCurPos != m_lOperationList.end())
    {
        return true;
    }
    return false;
}

Operation* COperationRecorder::UnDo()
{
    if (CanUnDo())
    {
        --m_pCurPos;
        return (*m_pCurPos);
    }
    return 0;
}

Operation* COperationRecorder::ReDo()
{
    if (CanReDo())
    {
        Operation *op = (*m_pCurPos);
        ++m_pCurPos;
        return op;
    }
    return 0;
}

void COperationRecorder::ClearExcessRecords()
{
    while (m_lOperationList.size() > ANM_MAX_OPERATION_RECORD)
    {
        m_pIt = m_lOperationList.begin();
        delete (*m_pIt);
        m_lOperationList.pop_front();
    }
}

void COperationRecorder::ClearBackRecords()
{
    m_pIt = m_pCurPos;
    while (m_pIt != m_lOperationList.end())
    {
        delete (*m_pIt);
        ++m_pIt;
    }
    m_lOperationList.erase(m_pCurPos, m_lOperationList.end());
    m_pCurPos = m_lOperationList.end();
}

void COperationRecorder::ClearAllRecords()
{
    m_pIt = m_lOperationList.begin();
    while (m_pIt != m_lOperationList.end())
    {
        delete (*m_pIt);
        ++m_pIt;
    }
    m_lOperationList.clear();
    m_pCurPos = m_lOperationList.end();
}

COperationRecorder::~COperationRecorder()
{
    ClearAllRecords();
}

