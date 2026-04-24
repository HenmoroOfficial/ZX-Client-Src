#include "AnmLock.h"
#include "Animator.h"
#include "AnmObject.h"
#include "AnmGFX.h"
#include "AnmObjectManager.h"

CAnmLock::CAnmLock( UINT iID )
	: CAnmAction( iID )
	, m_nType( LOCKTYPE_NONE )
	, m_iTotalFrames(0)
{
	m_ptOffset.x = 0;
	m_ptOffset.y = 0;
}

void CAnmLock::CopyDataFrom( CAnmAction* pAction )
{
	CAnmLock* pLock = dynamic_cast< CAnmLock* >( pAction );
	ASSERT( pLock );

	m_nType			= pLock->m_nType;
	m_ptOffset		= pLock->m_ptOffset;
	m_iTotalFrames	= pLock->m_iTotalFrames;
}

bool CAnmLock::Tick( const DWORD dwNowTime, CAnmObjectManager* pObjectManager )
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
		return true;
	}

	switch( m_nType )
	{
	case LOCKTYPE_RAINSNOW:
		{
			CAnmObject* pObject = pObjectManager->GetObject( m_uiObjectID );
			if( ! pObject )
				return false;

			CAnmGFX* pGFX = dynamic_cast< CAnmGFX* >( pObject );
			if( ! pGFX || ! pGFX->m_pA3DGFX )
				return false;

			A3DCamera* pA3DCamera = g_Animator.GetA3DCamera();
			A3DVECTOR3 vDirH = pA3DCamera->GetDirH();
			A3DVECTOR3 vPos = pA3DCamera->GetPos();
			vPos += vDirH * 2.0f;
			vPos.y -= 5.0f;
			pGFX->m_pA3DGFX->SetPos( vPos );
			pGFX->m_pA3DGFX->SetDirAndUp( -vDirH, A3DVECTOR3(0,1,0) );
		}
		break;

	case LOCKTYPE_SUBTITLE:
		{
			CAnmObject* pObject = pObjectManager->GetObject( m_uiObjectID );
			if( ! pObject )
				return false;

			CAnmGFX* pGFX = dynamic_cast< CAnmGFX* >( pObject );
			if( ! pGFX || ! pGFX->m_pA3DGFX )
				return false;

			A3DDEVFMT fmt = g_Animator.GetA3DDevice()->GetDevFormat();
			float fScale = min( fmt.nWidth / 800.0f, fmt.nHeight / 600.0f );
			pGFX->m_pA3DGFX->SetScale( fScale );
			A3DMATRIX4 m = a3d_Translate( fmt.nWidth * m_ptOffset.x, fmt.nHeight * m_ptOffset.y, 0.0f );
			pGFX->m_pA3DGFX->SetParentTM(m);
		}
		break;

	default:
		ASSERT(0);
		break;
	}
	return false;	
}