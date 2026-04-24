#pragma once

#include "AnmAction.h"

class CAnmColorMap : public CAnmAction
{
public:

	AString m_strColorMap;
	int		m_iTotalFrames;

public:

	CAnmColorMap( UINT iID );

	int GetTotalFrames() const { return m_iTotalFrames; }
	void SetTotalFrames( int iTotalFrames ) { m_iTotalFrames = iTotalFrames > 0 ? iTotalFrames : 1; }

	virtual ActionType GetActionType() const { return TYPE_COLORMAP; }

	virtual DWORD GetTotalTime() const { return TIME_PER_FRAME * m_iTotalFrames; }

	virtual void CopyDataFrom( CAnmAction* pAction );

	virtual bool Tick( const DWORD dwNowTime, CAnmObjectManager* pObjectManager );

	virtual void Reset();

private:

	AString	m_strBackupColorMap;
	bool	m_bStarted;
};