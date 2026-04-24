#include "AnmPauseGameSound.h"
#include "Animator.h"

CAnmPauseGameSound::CAnmPauseGameSound( UINT iID )
	: CAnmAction( iID )
	, m_nType( SOUNDTYPE_MUSIC )
	, m_iTotalFrames(0)
	, m_bStarted( false )
{
}

void CAnmPauseGameSound::CopyDataFrom( CAnmAction* pAction )
{
	CAnmPauseGameSound* pPauseGameSound = dynamic_cast< CAnmPauseGameSound* >( pAction );
	ASSERT( pPauseGameSound );

	m_nType			= pPauseGameSound->m_nType;
	m_iTotalFrames	= pPauseGameSound->m_iTotalFrames;
}

bool CAnmPauseGameSound::Tick( const DWORD dwNowTime, CAnmObjectManager* pObjectManager )
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
		if( m_bStarted )
		{
			g_Animator.ResumeGameSound( m_nType );
			m_bStarted = false;
		}
		return true;
	}
	if( ! m_bStarted )
	{
		g_Animator.PauseGameSound( m_nType );		
		m_bStarted = true;
	}
	return false;	
}

void CAnmPauseGameSound::Reset()
{
	CAnmAction::Reset();

	if( m_bStarted )
	{
		g_Animator.ResumeGameSound( m_nType );
		m_bStarted = false;
	}
}