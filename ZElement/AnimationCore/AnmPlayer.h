#ifndef _ANM_PLAYER_H_
#define _ANM_PLAYER_H_

#include "AnmNPC.h"

class CAnmPlayer : public CAnmNPC
{
    public:
        CAnmPlayer();

        void SetHostPlayerModel(CECModel *pModel);

        virtual ~CAnmPlayer();

		virtual void PlayAction(const ACString &actionName, const int iBlendTime, const bool bShowGFX);

        virtual void StopAction();

    private:
        bool m_bUseHostPlayer;
};

#endif
