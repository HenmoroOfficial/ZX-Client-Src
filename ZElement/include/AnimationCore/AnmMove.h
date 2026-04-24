#ifndef _ANM_MOVE_H_
#define _ANM_MOVE_H_

#include "AnmAction.h"

class CAnmMove : public CAnmAction
{
    public:
        enum RotateAxex
        {
            ROTATE_AXIS_X = 0,
            ROTATE_AXIS_Y = 1,
            ROTATE_AXIS_Z = 2
        };

    public:
        explicit CAnmMove(const unsigned int uiID);

        void SetStartPos(const A3DVECTOR3 &vPos);

        A3DVECTOR3 GetStartPos() const;

        void SetEndPos(const A3DVECTOR3 &vPos);

        A3DVECTOR3 GetEndPos() const;

        void SetOrientation(const A3DQUATERNION &qOri);

        A3DQUATERNION GetOrientation() const;

        void SetRotateAxis(const int a);

        int GetRotateAxis() const;

        void SetAngle(const float a);
        
        float GetAngle() const;

        void SetAccelerationPercent(const float fA);

        float GetAccelerationPercent() const;

        void SetTotalFrames(const int iFrame);

        int GetTotalFrames() const;

        void SetCloseTerrain(const bool b);

        bool GetCloseTerrain() const;

        virtual ActionType GetActionType() const;

        virtual void CopyDataFrom(CAnmAction *pAction);

        virtual bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        virtual DWORD GetTotalTime() const;

        virtual ~CAnmMove();

    private:
        CAnmMove();

        float GetVelocity() const;

    private:
        A3DVECTOR3 m_vStartPos;
        A3DVECTOR3 m_vEndPos;
        A3DQUATERNION m_qOrientation;
        int m_iRotateAxis;
        float m_fAngle;
        float m_fAccelerationPercent;
        int m_iTotalFrames;
        bool m_bCloseTerrain;
};

#endif
