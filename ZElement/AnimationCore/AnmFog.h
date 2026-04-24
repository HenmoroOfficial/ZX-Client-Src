#pragma once

#include "AnmAction.h"
//#include <A3DSceneRenderConfig.h>

class CAnmFog : public CAnmAction
{
public:

	//typedef A3DSceneRenderConfig::FOG_PARAMS _FOGPARMAS;

	//_FOGPARMAS	m_FogParams;
	DWORD		m_dwFogColor1;	// 上半球颜色
	DWORD		m_dwFogColor2;	// 下半球颜色
	int			m_iTotalFrames;

public:

	CAnmFog( UINT iID );

	int GetTotalFrames() const { return m_iTotalFrames; }
	void SetTotalFrames( int iTotalFrames ) { m_iTotalFrames = iTotalFrames > 0 ? iTotalFrames : 1; }

	virtual ActionType GetActionType() const { return TYPE_FOG; }

	virtual DWORD GetTotalTime() const { return TIME_PER_FRAME * m_iTotalFrames; }

	virtual void CopyDataFrom( CAnmAction* pAction );

	virtual bool Tick( const DWORD dwNowTime, CAnmObjectManager* pObjectManager );

	virtual void Reset();

protected:

	void UpdateFog();
};