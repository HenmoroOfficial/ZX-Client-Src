#pragma once

#include "AnmAction.h"
#include <A3D.h>

class CAnmLock : public CAnmAction
{
public:

	enum LockType
	{
		LOCKTYPE_NONE,
		LOCKTYPE_RAINSNOW,
		LOCKTYPE_SUBTITLE,
	};

public:

	BYTE		m_nType;
	APointF		m_ptOffset;
	int			m_iTotalFrames;

public:

	CAnmLock( UINT iID );

	int GetTotalFrames() const { return m_iTotalFrames; }
	void SetTotalFrames( int iTotalFrames ) { m_iTotalFrames = iTotalFrames > 0 ? iTotalFrames : 1; }

	virtual ActionType GetActionType() const { return TYPE_LOCK; }

	virtual DWORD GetTotalTime() const { return TIME_PER_FRAME * m_iTotalFrames; }

	virtual void CopyDataFrom( CAnmAction* pAction );

	virtual bool Tick( const DWORD dwNowTime, CAnmObjectManager* pObjectManager );
};