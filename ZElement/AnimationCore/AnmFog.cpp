#include "AnmFog.h"
#include "Animator.h"
#include <GfxCommonTypes.h>
#include "EC_FullGlowRender.h"
#include "EC_World.h"
#include "EC_SunMoon.h"

CAnmFog::CAnmFog( UINT iID )
	: CAnmAction( iID )
	, m_iTotalFrames(0)
{
	//m_FogParams.fStart			= 18.0f;
	//m_FogParams.fEnd				= 140.0f;
	//m_FogParams.fFogDensity1		= 0.79f;
	//m_FogParams.fFogDensity2		= 1.4f;
	//m_FogParams.fColorDensity1	= 0.73f;
	//m_FogParams.fColorDensity2	= 1.04f;
	//m_FogParams.fHorzDensity		= 1.89f;
	//m_FogParams.fHorzPower		= 0.92f;
	//m_FogParams.fHeightStart		= 17.0f;
	//m_FogParams.fHeightEnd		= 28.5f;
	//m_FogParams.fHeightDensity	= 4.52f;
	m_dwFogColor1				= 0xFF76E1E4;
	m_dwFogColor2				= 0xFF4D88CA;
}

void CAnmFog::CopyDataFrom( CAnmAction* pAction )
{
	CAnmFog* pFog = dynamic_cast< CAnmFog* >( pAction );
	ASSERT( pFog );

	//m_FogParams		= pFog->m_FogParams;
	m_dwFogColor1	= pFog->m_dwFogColor1;
	m_dwFogColor2	= pFog->m_dwFogColor2;
	m_iTotalFrames	= pFog->m_iTotalFrames;
}

bool CAnmFog::Tick( const DWORD dwNowTime, CAnmObjectManager* pObjectManager )
{
	if( dwNowTime < m_dwStartTime )
		return false;

	if( IsDead() )
		return false;

	DWORD dPassedTime = dwNowTime - m_dwStartTime;
	DWORD dwTotalTime = GetTotalTime();

	if( dPassedTime >= dwTotalTime )
	{
		m_bDead = true;
		g_Animator.SetUpdatingFogFlag( false );
		return true;
	}
	g_Animator.SetUpdatingFogFlag( true );
	UpdateFog();
	return false;	
}

void CAnmFog::Reset()
{
	CAnmAction::Reset();
	g_Animator.SetUpdatingFogFlag( false );
}

void CAnmFog::UpdateFog()
{
	//A3DSceneRenderConfig* pSceneRenderCfg = g_Animator.GetA3DEngine()->GetSceneRenderConfig();
	//if( ! pSceneRenderCfg )
		//return;

	//pSceneRenderCfg->SetFogParams( m_FogParams );
	//pSceneRenderCfg->SetFogColor( m_dwFogColor1 );
	//pSceneRenderCfg->SetFogColor( m_dwFogColor2 );

	g_Animator.GetA3DViewport()->SetClearColor( m_dwFogColor2 );

	CECFullGlowRender* pFullGlowRender = g_Animator.GetFullGlowRender();
	if( ! pFullGlowRender )
		return;

	pFullGlowRender->SetGlowType( FULLGLOW_TYPE_NULL, 0xffffffff );
	if( ! pFullGlowRender->IsGlowOn() )
		return;

	if( g_Animator.GetWorld()->GetSunMoon()->GetDNFactor() < 0.5f )
		pFullGlowRender->SetGlowType( FULLGLOW_TYPE_DAYLIGHT, 0xffffffff );
	else
		pFullGlowRender->SetGlowType( FULLGLOW_TYPE_NIGHT, 0xffffffff );
}