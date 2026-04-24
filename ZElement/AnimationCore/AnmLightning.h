#pragma once

#include "AnmAction.h"

class CAnmLightning : public CAnmAction
{
public:

	int		m_iTimes;			// 快速连闪的次数
	int		m_iInterval;		// 快速连闪后的等待时长
	int		m_iRand;			// 随机参数 [ Elapse, Elapse + RandElapse ]
	int		m_iOnceTime;		// 单次闪的时长
	int		m_iOnceInterval;	// 单次闪后的等待时长
	float	m_fStartEnergy;		// 开始能量
	float	m_fEndEnergy;		// 结束能量
	float	m_fLum;				// 亮度

	int		m_iTotalFrames;

	DWORD	m_dwLastTime;
	int		m_iCount;
	int		m_iOnceElapseTime;
	DWORD	m_dwInterval;
	DWORD	m_dwElaspeTime;

public:

	CAnmLightning( UINT iID );

	int GetTotalFrames() const { return m_iTotalFrames; }
	void SetTotalFrames( int iTotalFrames ) { m_iTotalFrames = iTotalFrames > 0 ? iTotalFrames : 1; }

	virtual ActionType GetActionType() const { return TYPE_LIGHTNING; }

	virtual DWORD GetTotalTime() const { return TIME_PER_FRAME * m_iTotalFrames; }

	virtual void CopyDataFrom( CAnmAction* pAction );

	virtual bool Tick( const DWORD dwNowTime, CAnmObjectManager* pObjectManager );

	virtual void Reset();
};