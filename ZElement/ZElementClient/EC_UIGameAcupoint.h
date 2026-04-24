/*******************************************************************
FILE: EC_UIGameRush.h

CREATED BY: Li Bin, 2013.12.13

HISTORY: 

DESCRIPTION: 

Copyright (c) 2011 ZhuXian Studio, All Rights Reserved.
*******************************************************************/
#include <deque>

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class
//	
///////////////////////////////////////////////////////////////////////////

class AXMLFile;
class A3DVECTOR3;

class CECGameAcupoint
{
public:
	enum GAME_PHASE
	{
		GAME_WAIT,
		GAME_PLAY,
		GAME_PAUSE,
		GAME_WIN,
		GAME_OVER
	};

	enum CHECK_POINT_STATUS
	{
		SEALED,
		ACTIVE,
		PASS,
		DISABLE
	};

	struct CheckPoint
	{
		bool				m_bIsKeyCheckPoint;
		A3DVECTOR3			m_ptPoint;
		CHECK_POINT_STATUS	m_sStatus;
	};

	struct Stage
	{
		AString				m_strName;
		int					m_nAcupointNum;
		float				m_fMoveSpeed;
		float				m_fCheckRange;
		AArray<CheckPoint>	*m_aCheckPoints;
	};

public:
	CECGameAcupoint( );
	~CECGameAcupoint( );

	GAME_PHASE					GetGamePhase( ) const { return m_phGamePhase; }
	const A3DVECTOR3&			GetRusherPos( ) const { return m_ptRusherPos; }
	const AArray<CheckPoint>*	GetCheckPoints( ) const;
	int							GetCurrentStage( ) const { return m_nCurrentStage; }
	DWORD						GetWaitingTime( ) const  { return m_dWaitingTime; }
	int							GetMaxLives( ) const	{ return m_nMaxGameLives; }
	int							GetLives( ) const	{ return m_nGameLives; }

	void AddStage( Stage stage ) { m_aStages.Add( stage ); }
	bool StartGame( int stageIndex, int waitTime = 0 );
	void PauseGame( bool pause );
	void EndGame( bool win );

	void Tick( );

	bool InteractCheckPoint( );

private:
	bool CheckWin( );

	void ResealCheckPoints( );
	void ActiveCheckPoints( );

private:
	AString			m_szName;
	GAME_PHASE		m_phGamePhase;

	DWORD			m_dWaitingTime;
	DWORD			m_dwLastRefreshTime;
	DWORD			m_dwElpase;

	int				m_nCurrentStage;
	AArray<Stage>	m_aStages;

	A3DVECTOR3		m_ptRusherPos;
	int				m_nGameLives;
	int				m_nMaxGameLives;
	int				m_nNextCheckPoint;
	bool			m_bInteractivePunish;
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////