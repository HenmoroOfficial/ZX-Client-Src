#pragma once

#include "AnmAction.h"
#include <A3D.h>

class CAnmPlaySound : public CAnmAction
{
public:

	enum SoundType
	{
		SOUNDTYPE_MUSIC,
		SOUNDTYPE_SFX,
	};

public:

	BYTE		m_nType;
	AString		m_strFilePath;
	bool		m_bLoop;
	int			m_iFadeInTime;
	int			m_iFadeOutTime;
	int			m_iVolume;
	int			m_iTotalFrames;

public:

	CAnmPlaySound( UINT iID );

	int GetTotalFrames() const { return m_iTotalFrames; }
	void SetTotalFrames( int iTotalFrames ) { m_iTotalFrames = iTotalFrames > 0 ? iTotalFrames : 1; }

	virtual ActionType GetActionType() const { return TYPE_PLAY_SOUND; }

	virtual DWORD GetTotalTime() const { return TIME_PER_FRAME * m_iTotalFrames; }

	virtual void CopyDataFrom( CAnmAction* pAction );

	virtual bool Tick( const DWORD dwNowTime, CAnmObjectManager* pObjectManager );

	virtual void Reset();

private:

	bool	m_bStarted;
};