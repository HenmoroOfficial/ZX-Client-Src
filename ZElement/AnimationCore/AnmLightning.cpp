#include "AnmLightning.h"
#include "Animator.h"
#include <GfxCommonTypes.h>
#include "EC_FullGlowRender.h"

CAnmLightning::CAnmLightning( UINT iID )
	: CAnmAction( iID )
	, m_iTimes(3)
	, m_iInterval(5000)
	, m_iRand(2000)
	, m_iOnceTime(350)
	, m_iOnceInterval(200)
	, m_fStartEnergy(2.0f)
	, m_fEndEnergy(0)
	, m_fLum(0.1f)
	, m_iTotalFrames(0)
	, m_dwLastTime(0)
	, m_iCount(0)
	, m_iOnceElapseTime(0)
	, m_dwInterval(0)
	, m_dwElaspeTime(0)
{
}

void CAnmLightning::CopyDataFrom( CAnmAction* pAction )
{
	CAnmLightning* pLightning = dynamic_cast< CAnmLightning* >( pAction );
	ASSERT( pLightning );

	m_iTimes			= pLightning->m_iTimes;
	m_iInterval			= pLightning->m_iInterval;
	m_iRand				= pLightning->m_iRand;
	m_iOnceTime			= pLightning->m_iOnceTime;
	m_iOnceInterval		= pLightning->m_iOnceInterval;
	m_fStartEnergy		= pLightning->m_fStartEnergy;
	m_fEndEnergy		= pLightning->m_fEndEnergy;
	m_fLum				= pLightning->m_fLum;
	m_iTotalFrames		= pLightning->m_iTotalFrames;
	m_dwLastTime		= pLightning->m_dwLastTime;
	m_iCount			= pLightning->m_iCount;
	m_iOnceElapseTime	= pLightning->m_iOnceElapseTime;
	m_dwInterval		= pLightning->m_dwInterval;
	m_dwElaspeTime		= pLightning->m_dwElaspeTime;
}

bool CAnmLightning::Tick( const DWORD dwNowTime, CAnmObjectManager* pObjectManager )
{
	DWORD dwDeltaTime = dwNowTime - m_dwLastTime;
	m_dwLastTime = dwNowTime;

	if( dwNowTime < m_dwStartTime )
		return false;

	if( IsDead() )
		return false;

	DWORD dPassedTime = dwNowTime - m_dwStartTime;
	DWORD dwTotalTime = GetTotalTime();
	
	if( dPassedTime >= dwTotalTime )
	{
		m_bDead = true;
		m_dwLastTime		= 0;
		m_iCount			= 0;
		m_iOnceElapseTime	= 0;
		m_dwInterval		= 0;
		m_dwElaspeTime		= 0;
		return true;
	}
	if( m_iCount < m_iTimes )
	{
		m_iOnceElapseTime += dwDeltaTime;
		if( m_iOnceElapseTime >= m_iOnceInterval ) 
		{
			//g_Animator.GetFullGlowRender()->GetPresenter()->SetFlashPower( m_fLum, m_fStartEnergy, m_fEndEnergy, m_iOnceTime );
			m_iOnceElapseTime = 0;
			++ m_iCount;
		}
	}
	else
	{
		if( ! m_dwInterval )
		{
			m_dwInterval = m_iInterval + a_Random( 0, m_iRand );
			m_dwElaspeTime = 0;
		}
		else
			m_dwElaspeTime += dwDeltaTime;

		if( m_dwElaspeTime >= m_dwInterval )
		{
			m_iCount			= 0;
			m_iOnceElapseTime	= m_iOnceInterval + 1;
			m_dwInterval		= 0;
			m_dwElaspeTime		= 0;
		}
	}
	return false;	
}

void CAnmLightning::Reset()
{
	CAnmAction::Reset();
	m_dwLastTime		= 0;
	m_iCount			= 0;
	m_iOnceElapseTime	= 0;
	m_dwInterval		= 0;
	m_dwElaspeTime		= 0;
}