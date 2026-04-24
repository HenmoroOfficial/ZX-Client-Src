/*
 * FILE: A3DPostSunMoon.cpp
 *
 * DESCRIPTION: äÖÈ¾Ì«ÑôºÍÔÂÁÁ
 *
 * CREATED BY: Yaochunhui, 2012/4/6
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#include "A3DPostSunMoon.h"
#include "A3DSunMoon.h"
#include "A3DPostSystem.h"

extern ALog g_A3DErrLog;

A3DPostSunMoon::~A3DPostSunMoon()
{

}

bool A3DPostSunMoon::Init(A3DPostSystem* pSystem)
{
    m_pSystem = pSystem;

    return true;
}

void A3DPostSunMoon::Release()
{
    m_pSunMoon = NULL;
}

bool A3DPostSunMoon::IsHardwareSupport(A3DDevice* pA3DDev)
{
    return true;
}

bool A3DPostSunMoon::Render(A3DViewport* pViewport)
{
    A3DRenderTarget* pSrc = m_pSystem->GetSrcRT();
    pSrc->ApplyToDevice(0);

    A3DDevice* pDev = m_pSystem->GetA3DDevice();

    //ÉèÖÃäÖÈ¾×´Ì¬
    pDev->SetAlphaBlendEnable(true);
    pDev->SetZTestEnable(true);
    pDev->SetZWriteEnable(false);
    pDev->SetFaceCull(A3DCULL_NONE);
    pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
    pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);

    if (m_pSunMoon)
    {
        m_pSunMoon->Render(pViewport);
    }
    pSrc->WithdrawFromDevice();
    pDev->SetZTestEnable(false);
    pDev->SetZWriteEnable(false);
    return true;
}

A3DPOST_EFFECT A3DPostSunMoon::GetEffectType() const
{
	return A3DPOST_EFFECT_SunMoon;
}

A3DRenderTarget* A3DPostSunMoon::ApplyEffect( A3DViewport* pViewport, A3DRenderTarget* pSrcRT )
{
    pSrcRT->ApplyToDevice(0);

    A3DDevice* pDev = m_pSystem->GetA3DDevice();

    //ÉèÖÃäÖÈ¾×´Ì¬
    pDev->SetAlphaBlendEnable(true);
    pDev->SetZTestEnable(true);
    pDev->SetZWriteEnable(false);
    pDev->SetFaceCull(A3DCULL_NONE);
    pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
    pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);

    if (m_pSunMoon)
    {
        m_pSunMoon->Render(pViewport);
    }
    pSrcRT->WithdrawFromDevice();
    pDev->SetZTestEnable(false);
    pDev->SetZWriteEnable(false);
    return pSrcRT;
}

