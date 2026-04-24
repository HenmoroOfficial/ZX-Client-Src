#ifndef _ANM_CAMERA_OSCILLATORY_H_
#define _ANM_CAMERA_OSCILLATORY_H_

#include "AnmAction.h"

class CAnmCameraOscillatory : public CAnmAction
{
    public:
        enum AnmOscillatoryType
        {
            OSCILLATORY_TYPE_DIRECTION = 0,
            OSCILLATORY_TYPE_RANDOM
        };

        enum OscillatoryAxex
        {
            OSCILLATORY_AXIS_X = 0,
            OSCILLATORY_AXIS_Y = 1,
            OSCILLATORY_AXIS_Z = 2
        };

        enum OscillatoryAttenuation
        {
            OSCILLATORY_ATTENUATION_NONE = 0,
            OSCILLATORY_ATTENUATION_UP = 1,
            OSCILLATORY_ATTENUATION_DOWN = 2
        };

    public:
        explicit CAnmCameraOscillatory(const unsigned int uiID);

        void SetOscillatoryType(const unsigned int t);
        
        unsigned int GetOscillatoryType() const;

        void SetOscillatoryAxis(const int a);
 
        int GetOscillatoryAxis() const;

        void SetDisplacement(const float f);

        float GetDisplacement() const;

        void SetFrequency(const float f);

        float GetFrequency() const;

        void SetAttenuation(const int a);

        int GetAttenuation() const;

        void SetTotalFrames(const int iFrame);

        int GetTotalFrames() const;

        virtual ActionType GetActionType() const;

        virtual void CopyDataFrom(CAnmAction *pAction);

        virtual bool Tick(const DWORD dwNowTime, CAnmObjectManager *pObjectManager);

        virtual DWORD GetTotalTime() const;

        virtual ~CAnmCameraOscillatory();

    private:
        CAnmCameraOscillatory();

    private:
        unsigned int m_uiOscillatoryType;
        int m_iOscillatoryAxis;
        int m_iAttenuation;
        float m_fDisplacement;
        float m_fFrequency;
        int m_iTotalFrames;
};

#endif
