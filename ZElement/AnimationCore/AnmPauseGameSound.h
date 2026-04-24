#pragma once

#include "AnmAction.h"
#include <A3D.h>

class CAnmPauseGameSound : public CAnmAction
{
public:

	enum SoundType
	{
		SOUNDTYPE_MUSIC = 0x1,
		SOUNDTYPE_SFX	= 0x2,
	};

public:

	BYTE	m_nType;
	int		m_iTotalFrames;

public:

	CAnmPauseGameSound( UINT iID );

	int GetTotalFrames() const { return m_iTotalFrames; }
	void SetTotalFrames( int iTotalFrames ) { m_iTotalFrames = iTotalFrames > 0 ? iTotalFrames : 1; }

	virtual ActionType GetActionType() const { return TYPE_PAUSE_GAME_SOUND; }

	virtual DWORD GetTotalTime() const { return TIME_PER_FRAME * m_iTotalFrames; }

	virtual void CopyDataFrom( CAnmAction* pAction );

	virtual bool Tick( const DWORD dwNowTime, CAnmObjectManager* pObjectManager );

	virtual void Reset();

private:

	bool	m_bStarted;
};