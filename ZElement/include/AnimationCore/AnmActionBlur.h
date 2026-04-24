#pragma once

#include "AnmAction.h"
#include <EC_Model.h>

class CAnmActionBlur : public CAnmAction
{
public:

	ECMActionBlurInfo	m_Info;
	int					m_iTotalFrames;

public:

	CAnmActionBlur( UINT iID );

	int GetTotalFrames() const { return m_iTotalFrames; }
	void SetTotalFrames( int iTotalFrames ) { m_iTotalFrames = iTotalFrames > 0 ? iTotalFrames : 1; }

	virtual ActionType GetActionType() const { return TYPE_ACTION_BLUR; }

	virtual DWORD GetTotalTime() const { return TIME_PER_FRAME * m_iTotalFrames; }

	virtual void CopyDataFrom( CAnmAction* pAction );

	virtual bool Tick( const DWORD dwNowTime, CAnmObjectManager* pObjectManager );

	virtual void Reset();

private:

	bool m_bStarted;

private:

	bool StartActionBlur();
	bool StopActionBlur();
};