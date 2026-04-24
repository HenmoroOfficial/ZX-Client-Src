/*******************************************************************
FILE: .cpp

CREATED BY: Ma Yuanzheng , 2011.12.15

HISTORY: 

DESCRIPTION: 

Copyright (c) 2011 ZhuXian Studio, All Rights Reserved.
*******************************************************************/
#include "AIniFile.h"
#include "afi.h"
#include "A3DVector.h"
#include "AArray.h"
#include "AUI\\AUICommon.h"
#include "AUI\\AXMLFile.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIProgress.h"
#include "EC_Game.h"
#include "EC_Global.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_UIGameAcupoint.h"
#include "DlgGameAcupoint.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#ifdef UNICODE
#define a_atof		_wtof
#else
#define a_atof		atof
#endif	//	UNICODE


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////
// 

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement
//	
///////////////////////////////////////////////////////////////////////////
CECGameAcupoint::CECGameAcupoint( )
{
	m_nCurrentStage = 0;
	m_phGamePhase = GAME_WAIT;
	m_dwLastRefreshTime = -1;
}

CECGameAcupoint::~CECGameAcupoint( )
{
	for (int i = 0; i <m_aStages.GetSize(); ++ i)
	{
		if (m_aStages[i].m_aCheckPoints)
		{
			delete m_aStages[i].m_aCheckPoints;
			m_aStages[i].m_aCheckPoints = NULL;
		}
	}
}

const AArray<CECGameAcupoint::CheckPoint>*	CECGameAcupoint::GetCheckPoints( ) const
{
	if ( m_nCurrentStage >= m_aStages.GetSize( ) )
		return NULL;

	return m_aStages[m_nCurrentStage].m_aCheckPoints;
}

bool CECGameAcupoint::StartGame( int stageIndex, int waitTime )
{
	if ( stageIndex >= m_aStages.GetSize( ) )
		return false;

	m_nCurrentStage = stageIndex;
	
	const Stage& stage = m_aStages[m_nCurrentStage];
	if ( stage.m_aCheckPoints->GetSize( ) < 2 )
		return false;

	for ( int i = 0; i < stage.m_aCheckPoints->GetSize( ); ++ i )
	{
		if ( (*stage.m_aCheckPoints)[i].m_bIsKeyCheckPoint )
			(*stage.m_aCheckPoints)[i].m_sStatus = SEALED;
	}

	m_ptRusherPos = (*stage.m_aCheckPoints)[0].m_ptPoint;
	m_nGameLives = m_nMaxGameLives = 3;
	m_nNextCheckPoint = 1;
	m_bInteractivePunish = false;
	m_dwLastRefreshTime = GetTickCount( );

	m_phGamePhase = GAME_WAIT;
	m_dWaitingTime = waitTime;

	return true;
}

void CECGameAcupoint::PauseGame( bool pause )
{
	if ( pause && m_phGamePhase == GAME_PLAY )
		m_phGamePhase = GAME_PAUSE;
	else if ( !pause && m_phGamePhase == GAME_PAUSE )
		m_phGamePhase = GAME_PLAY;
}

void CECGameAcupoint::EndGame( bool win )
{
	m_phGamePhase = win ? GAME_WIN : GAME_OVER;
}

void CECGameAcupoint::Tick( )
{
	DWORD dwTime = GetTickCount( );
	DWORD elapse = dwTime - m_dwLastRefreshTime;
	m_dwLastRefreshTime = dwTime;

	if ( m_phGamePhase == GAME_WAIT )
	{
		if ( m_dWaitingTime > elapse )
		{
			m_dWaitingTime -= elapse;
			return;
		}

		m_dWaitingTime = 0;
		m_phGamePhase = GAME_PLAY;
	}

	if ( m_phGamePhase != GAME_PLAY )
		return;

	if ( m_nCurrentStage >= m_aStages.GetSize( ) )
		return;

	if ( m_nGameLives <= 0 )
	{
		EndGame( false );
		return;
	}

	const Stage& stage = m_aStages[m_nCurrentStage];

	float moveDis = stage.m_fMoveSpeed * elapse / 1000.0f;
	
	A3DVECTOR3 dir = (*stage.m_aCheckPoints)[m_nNextCheckPoint].m_ptPoint - m_ptRusherPos;
	float dis = dir.Normalize( );
	
	while ( moveDis > dis )
	{
		if ( (*stage.m_aCheckPoints)[m_nNextCheckPoint].m_sStatus == ACTIVE )
			(*stage.m_aCheckPoints)[m_nNextCheckPoint].m_sStatus = DISABLE;

		m_ptRusherPos = (*stage.m_aCheckPoints)[m_nNextCheckPoint].m_ptPoint;
		m_nNextCheckPoint ++;
		moveDis -= dis;

		if ( m_nNextCheckPoint >= stage.m_aCheckPoints->GetSize( ) )
		{
			m_ptRusherPos = (*stage.m_aCheckPoints)[0].m_ptPoint;
			m_nNextCheckPoint = 1;
			m_bInteractivePunish = false;
			m_nGameLives --;

			ResealCheckPoints( );
			ActiveCheckPoints( );
			return;
		}

		dir = (*stage.m_aCheckPoints)[m_nNextCheckPoint].m_ptPoint - m_ptRusherPos;
		dis = dir.Normalize( );
	}

	m_ptRusherPos += dir * moveDis;
	ActiveCheckPoints( );
}

bool CECGameAcupoint::InteractCheckPoint( )
{
	if ( m_nCurrentStage >= m_aStages.GetSize( ) )
		return false;

	const Stage& stage = m_aStages[m_nCurrentStage];

	if ( m_nNextCheckPoint >= stage.m_aCheckPoints->GetSize( ) )
		return false;

	CheckPoint& cp = (*stage.m_aCheckPoints)[m_nNextCheckPoint];
	if (!cp.m_bIsKeyCheckPoint)
		return false;
	
	if ( cp.m_sStatus != ACTIVE )
	{
		if ( cp.m_sStatus == SEALED )
			cp.m_sStatus = DISABLE;

		if ( !m_bInteractivePunish )
		{
			m_nGameLives --;
			m_bInteractivePunish = true;
		}

		return false;
	}

	cp.m_sStatus = PASS;

	if ( CheckWin( ) )
		EndGame(true);

	return true;
}

bool CECGameAcupoint::CheckWin( )
{
	if( m_nCurrentStage >= m_aStages.GetSize( ) )
		return false;

	const Stage& stage = m_aStages[m_nCurrentStage];

	for ( int i = 0; i < stage.m_aCheckPoints->GetSize( ); ++ i )
	{
		CheckPoint& cp = (*stage.m_aCheckPoints)[i];
		if ( cp.m_bIsKeyCheckPoint && cp.m_sStatus != PASS )
			return false;
	}

	return true;
}

void CECGameAcupoint::ResealCheckPoints( )
{
	if ( m_nCurrentStage >= m_aStages.GetSize( ) )
		return;

	const Stage& stage = m_aStages[m_nCurrentStage];

	for ( int i = 0; i < stage.m_aCheckPoints->GetSize( ); ++ i )
	{
		CheckPoint& cp = (*stage.m_aCheckPoints)[i];
		if ( cp.m_bIsKeyCheckPoint && cp.m_sStatus == DISABLE )
			cp.m_sStatus = SEALED;
	}
}

void CECGameAcupoint::ActiveCheckPoints( )
{
	if ( m_nCurrentStage >= m_aStages.GetSize( ) )
		return;

	const Stage& stage = m_aStages[m_nCurrentStage];

	if ( m_nNextCheckPoint >= stage.m_aCheckPoints->GetSize( ) )
		return;

	CheckPoint& cp = (*stage.m_aCheckPoints)[m_nNextCheckPoint];
	if ( !cp.m_bIsKeyCheckPoint || cp.m_sStatus != SEALED )
		return;

	if ( (m_ptRusherPos - cp.m_ptPoint).Magnitude( ) <= stage.m_fCheckRange )
		cp.m_sStatus = ACTIVE;
}