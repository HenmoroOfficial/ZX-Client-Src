#ifndef ECMANSKILLGFX_H_
#define ECMANSKILLGFX_H_

#include "AList2.h"
#include "A3DGfxEx.h"
#include "A3DGfxExMan.h"
#include "A3DSkillGfxEvent.h"
#ifdef BACK_VERSION
#include "EC_Manager.h"
#else
#include "ECManager.h"
#include "ECManagerID.h"
#endif
#include "EC_Game.h"
#include "EC_World.h"
#include "EC_GameRun.h"
#include "EC_GPDataType.h"
#include "EC_Player.h"
#include "EC_ManPlayer.h"
#include "EC_ManNPC.h"
#include "EC_NPC.h"

extern CECGame* g_pGame;

namespace _SGC
{

class CECSkillGfxEvent : public A3DSkillGfxEvent
{
public:
	CECSkillGfxEvent(A3DSkillGfxMan* pMan, GfxMoveMode mode) : A3DSkillGfxEvent(pMan, mode)
	{
	}

	virtual ~CECSkillGfxEvent() {}

public:
	virtual A3DGFXEx* LoadHitGfx(A3DDevice* pDev, const char* szPath, DWORD dwModifier);
	virtual void PlayExtraHitGfx(DWORD dwModifier, const A3DVECTOR3& vPos);
};

class SkillGfxMan : public A3DSkillGfxMan
{
public:

	SkillGfxMan(CECGameRun* pGameRun);
	virtual ~SkillGfxMan() {}

protected:

	CECPlayerMan* m_pPlayerMan;
	CECNPCMan* m_pNPCMan;

	virtual A3DSkillGfxEvent* CreateOneEvent(GfxMoveMode mode)
	{
		return new CECSkillGfxEvent(this, mode);
	}

	static void WINAPI ShowCasterInfo(__int64 idCaster, DWORD dwModifier);
	static void WINAPI ShowDamageInfo(__int64 idCaster, __int64 idTarget, int nDamage, int nDivisions, DWORD dwModifier);
	static void WINAPI SkillGfxTickFunc(__int64 idCaster, int state, DWORD dwTickTime, const A3DVECTOR3* pDir, const A3DVECTOR3* pPos);

public:

	virtual bool GetPositionById(__int64 nId, A3DVECTOR3& vPos, GfxHitPos HitPos, const char* szHook, bool bRelHook, const A3DVECTOR3* pOffset, const char* szHanger, bool bChildHook);
	virtual bool GetDirAndUpById(__int64 nId, A3DVECTOR3& vDir, A3DVECTOR3& vUp);
	virtual float GetTargetScale(__int64 nTargetId);
};

}

#ifdef BACK_VERSION
class CECSkillGfxMan : public CECManager
#else
class CECSkillGfxMan : public ECManager
#endif
{
public:
	CECSkillGfxMan(CECGameRun* pGameRun);
	~CECSkillGfxMan() {}

protected:
	_SGC::SkillGfxMan m_GfxMan;

public:
	_SGC::A3DSkillGfxMan* GetPtr() { return &m_GfxMan; }
	virtual void Release() { m_GfxMan.Release(); }
	virtual bool Tick(DWORD dwDeltaTime) { return m_GfxMan.Tick(dwDeltaTime); }
	virtual bool Render(CECViewport* pViewport) { return m_GfxMan.Render(); }
#ifndef BACK_VERSION
	virtual bool RenderPost(ECViewport* pViewport) { return true; }
#endif
	bool AddSkillGfxEvent(
		int nHostID,
		int nTargetID,
		const char* szFlyGfx,
		const char* szHitGfx,
		DWORD dwFlyTimeSpan,
		bool bTraceTarget = true,
		_SGC::GfxMoveMode FlyMode = _SGC::enumLinearMove,
		int nFlyGfxCount = 1,
		DWORD dwInterval = 0,
		const _SGC::GFX_SKILL_PARAM* param = NULL,
		float fFlyGfxScale = 1.0f,
		float fHitGfxScale = 1.0f,
		DWORD dwModifier = 0
		)
	{
		return true;
	}
};

#endif
