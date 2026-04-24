#include "stdafx.h"
#include "AnmPlayer.h"
#include "Animator.h"

CAnmPlayer::CAnmPlayer()
    : CAnmNPC(1)
{
    m_bUseHostPlayer = false;
}

void CAnmPlayer::SetHostPlayerModel(CECModel *pModel)
{
    if (pModel)
    {
		g_Animator.UnloadObject(this);

        m_pModel = pModel;
        m_pSkin = pModel->GetA3DSkin(0);
        m_bUseHostPlayer = true;
    }
}

CAnmPlayer::~CAnmPlayer()
{
    if (m_bUseHostPlayer)
    {
        m_pModel = 0;
        m_pSkin = 0;
    }
    else
    {
        g_Animator.UnloadObject(this);
    }
}

void CAnmPlayer::PlayAction(const ACString &actionName, const int iBlendTime, const bool bShowGFX)
{
    if (m_pModel && actionName != _AL(""))
    {
        //a_LogOutput(1, "Play action");
        m_sActionName = actionName;
		m_pModel->PlayActionByName(1, WC2AS(actionName), 1, false, iBlendTime, true, 0, !bShowGFX);
        //m_pModel->PlayActionByName(WC2AS(actionName), 1.0f,
        //    true, iBlendTime, true, 0, !bShowGFX);

		m_eActState = ESTATE_ACT_PLAY;
    }
}

void CAnmPlayer::StopAction()
{
    m_fSpeed = 1.0f;
    m_fTickTime = 0.0f;
	m_sActionName = _AL("");
    if (m_pModel)
    {
        //a_LogOutput(1, "Stop action");
        // ´úÌæm_pModel->StopAllActions();
        m_pModel->ResetMaterialScale();
        //m_pModel->StopChannelAction(0, true, true);
		m_pModel->StopChannelAction(1, true, true);
	    m_pModel->StopChildrenAct();
    }
	m_eActState = ESTATE_ACT_STOP;
}
