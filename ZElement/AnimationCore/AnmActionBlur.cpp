#include "AnmActionBlur.h"
#include "Animator.h"
#include "AnmNPC.h"
#include "AnmObjectManager.h"

CAnmActionBlur::CAnmActionBlur( UINT iID )
	: CAnmAction( iID )
	, m_iTotalFrames(0)
	, m_bStarted( false )
{
	m_Info.m_Colors.push_back( 0x100000FF );
	m_Info.m_Colors.push_back( 0x100000FF );
	m_Info.m_Colors.push_back( 0x20009BFF );
	m_Info.m_Colors.push_back( 0x309B9BFF );
	m_Info.m_Colors.push_back( 0x409BFFFF );
	m_Info.m_dwTotalTime = 0xffffffff;
	m_Info.m_dwInterval = 25;
	m_Info.m_bApplyToChildren = false;
	m_Info.m_bHighlight = false;
	m_Info.m_bRoot = true;
	m_Info.m_bStopWhenActChange = false;
}

void CAnmActionBlur::CopyDataFrom( CAnmAction* pAction )
{
	CAnmActionBlur* pActionBlur = dynamic_cast< CAnmActionBlur* >( pAction );
	ASSERT( pActionBlur );

	m_Info			= pActionBlur->m_Info;
	m_iTotalFrames	= pActionBlur->m_iTotalFrames;
}

bool CAnmActionBlur::Tick( const DWORD dwNowTime, CAnmObjectManager* pObjectManager )
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
			StopActionBlur();
			m_bStarted = false;
		}
		return true;
	}
	if( ! m_bStarted )
	{
		StartActionBlur();
		m_bStarted = true;
	}
	return false;	
}

void CAnmActionBlur::Reset()
{
	CAnmAction::Reset();

	if( m_bStarted )
	{
		StopActionBlur();
		m_bStarted = false;
	}
}

bool CAnmActionBlur::StartActionBlur()
{
	CAnmObjectManager* pObjectManager = g_Animator.GetObjectManager();
	if( ! pObjectManager )
		return false;

	CAnmObject* pObject = pObjectManager->GetObject( m_uiObjectID );
	if( ! pObject )
		return false;

	CAnmNPC* pNPC = dynamic_cast< CAnmNPC* >( pObject );
	if( ! pNPC || ! pNPC->m_pModel )
		return false;

	ECMActionBlurInfo* pInfo = new ECMActionBlurInfo( m_Info );
	pNPC->m_pModel->SetMotionBlurInfo( pInfo );
	return true;
}

bool CAnmActionBlur::StopActionBlur()
{
	CAnmObjectManager* pObjectManager = g_Animator.GetObjectManager();
	if( ! pObjectManager )
		return false;

	CAnmObject* pObject = pObjectManager->GetObject( m_uiObjectID );
	if( ! pObject )
		return false;

	CAnmNPC* pNPC = dynamic_cast< CAnmNPC* >( pObject );
	if( ! pNPC || ! pNPC->m_pModel )
		return false;

	pNPC->m_pModel->RemoveMotionBlurInfo();
	return true;
}