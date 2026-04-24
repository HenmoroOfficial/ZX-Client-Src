#ifndef _ANM_AXES_H_
#define _ANM_AXES_H_

#include <A3D.h>

class CAnmAxes
{
    public:
        enum AxisType
        {
            TYPE_NONE = 0,
            TYPE_X,
            TYPE_Y,
            TYPE_Z,
        };

    public:
        explicit CAnmAxes(A3DWireCollector *pA3DWireCollector);

        void Render();

        void SetPos(const A3DVECTOR3 &v);

        A3DVECTOR3 GetPos() const;

        void SelectAxis(const AxisType type);

        AxisType GetSelectedAxis() const;

        void SetVisable(const bool b);

        bool IsVisable() const;

        void MoveX(const float d);

        void MoveY(const float d);

        void MoveZ(const float d);

        A3DAABB GetAxisAABB(const AxisType type);

        ~CAnmAxes();

    private:
        CAnmAxes();

        CAnmAxes(const CAnmAxes&);

        CAnmAxes& operator = (const CAnmAxes&);

    private:
        A3DWireCollector *m_pA3DWireCollector;
        A3DVECTOR3 m_vPos;
        AxisType m_eSelectedAxisType;
        bool m_bVisable;
};

#endif
