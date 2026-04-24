#include "AnmColorMap.h"
#include "Animator.h"
#include "EC_World.h"

CAnmColorMap::CAnmColorMap( UINT iID )
	: CAnmAction( iID )
	, m_iTotalFrames(0)
	, m_bStarted( false )
{
}

void CAnmColorMap::CopyDataFrom( CAnmAction* pAction )
{
	CAnmColorMap* pColorMap = dynamic_cast< CAnmColorMap* >( pAction );
	ASSERT( pColorMap );

	m_strColorMap	= pColorMap->m_strColorMap;
	m_iTotalFrames	= pColorMap->m_iTotalFrames;
}

bool CAnmColorMap::Tick( const DWORD dwNowTime, CAnmObjectManager* pObjectManager )
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
			CECWorld* pWorld = g_Animator.GetWorld();
			//if( pWorld )
				//pWorld->ChangeColorMap( m_strBackupColorMap );
			m_bStarted = false;
		}
		return true;
	}
	if( ! m_bStarted )
	{
		CECWorld* pWorld = g_Animator.GetWorld();
		if( pWorld )
		{
			//m_strBackupColorMap = pWorld->GetCurColorMap();
			//pWorld->ChangeColorMap( m_strColorMap );
		}
		m_bStarted = true;
	}
	return false;	
}

void CAnmColorMap::Reset()
{
	CAnmAction::Reset();

	if( m_bStarted )
	{
		CECWorld* pWorld = g_Animator.GetWorld();
		if( ! pWorld )
			return;
		//pWorld->ChangeColorMap( m_strBackupColorMap );
		m_bStarted = false;
	}
}