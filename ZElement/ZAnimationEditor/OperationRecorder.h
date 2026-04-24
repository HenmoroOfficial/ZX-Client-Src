#ifndef _ANM_OPERATION_RECORDER_H_
#define _ANM_OPERATION_RECORDER_H_

#include <A3D.h>
#include <list>

const size_t ANM_MAX_OPERATION_RECORD = 0x200;

enum OperationTarget
{
    OT_OBJECT = 0,
    OT_BEZIER_POINT,
    OT_BEZIER_ANCHOR,
};


struct Operation
{
    OperationTarget target;
    unsigned int objectID;
    unsigned int bezierID;
    unsigned int bezierPointID;
    unsigned int bezierSegmentID;
    int bezierAnchor;
    A3DVECTOR3 position;
    A3DVECTOR3 direction;
    A3DVECTOR3 up;
    A3DQUATERNION orientation;
};

class COperationRecorder
{
    public:
        COperationRecorder();

        void RecordObject(const unsigned int id,
                          const A3DVECTOR3 &p,
                          const A3DVECTOR3 &d,
                          const A3DVECTOR3 &u);

        void RecordBezierPoint(const unsigned int bID,
                               const unsigned int pID,
                               const A3DVECTOR3 &p,
                               const A3DQUATERNION &q);

        void RecordBezierAnchor(const unsigned int bID,
                                const unsigned int sID,
                                const int anchor,
                                const A3DVECTOR3 &p);

        bool CanUnDo() const;

        bool CanReDo() const;

        Operation* UnDo();

        Operation* ReDo();

        void ClearExcessRecords();

        void ClearBackRecords();

        void ClearAllRecords();

        ~COperationRecorder();

    private:
        std::list<Operation*> m_lOperationList;
        mutable std::list<Operation*>::iterator m_pCurPos;
        mutable std::list<Operation*>::iterator m_pIt;
};

#endif
