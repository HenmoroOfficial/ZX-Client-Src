#include "AnmPlaySound.h"
#include "Animator.h"

CAnmPlaySound::CAnmPlaySound( UINT iID )
	: CAnmAction( iID )
	, m_nType( SOUNDTYPE_MUSIC )
	, m_bLoop( false )
	, m_iFadeInTime(0)
	, m_iFadeOutTime(0)
	, m_iVolume( 100 )
	, m_iTotalFrames(0)
	, m_bStarted( false )
{
}

void CAnmPlaySound::CopyDataFrom( CAnmAction* pAction )
{
	CAnmPlaySound* pPlaySound = dynamic_cast< CAnmPlaySound* >( pAction );
	ASSERT( pPlaySound );

	m_nType			= pPlaySound->m_nType;
	m_strFilePath	= pPlaySound->m_strFilePath;
	m_bLoop			= pPlaySound->m_bLoop;
	m_iFadeInTime	= pPlaySound->m_iFadeInTime;
	m_iFadeOutTime	= pPlaySound->m_iFadeOutTime;
	m_iVolume		= pPlaySound->m_iVolume;
	m_iTotalFrames	= pPlaySound->m_iTotalFrames;
}

bool CAnmPlaySound::Tick( const DWORD dwNowTime, CAnmObjectManager* pObjectManager )
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
			g_Animator.StopSound( m_nType, m_strFilePath, m_iFadeOutTime );
			m_bStarted = false;
		}
		return true;
	}
	if( ! m_bStarted )
	{
		if( dwNowTime == 1 ) // TOFIX
			return false;

		g_Animator.PlaySound( m_nType, m_strFilePath, m_bLoop, m_iFadeInTime, m_iVolume );
		m_bStarted = true;
	}
	return false;	
}

void CAnmPlaySound::Reset()
{
	CAnmAction::Reset();

	if( m_bStarted )
	{
		switch( m_nType )
		{
		case SOUNDTYPE_MUSIC:
			g_Animator.StopSound( m_nType, m_strFilePath, m_iFadeOutTime );
			break;
		}
		m_bStarted = false;
	}
}