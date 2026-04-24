#include "EC_Global.h"
#include "EC_ManSkillGfx.h"
#include "EC_GFXCaster.h"
#include "EC_Model.h"
#include "EC_HostPlayer.h"
#include "EC_FixedMsg.h"
#include "EC_GameRun.h"
#include "EC_Sprite.h"
#include "A3DSkinModel.h"
#include "A3DSkeleton.h"

#include "LuaState.h"
#include "LuaUtil.h"
#include "LuaScript.h"
#include "ScriptValue.h"

namespace _SGC
{

A3DSkillGfxMan* AfxGetSkillGfxEventMan()
{
	return g_pGame->GetGameRun()->GetWorld()->GetSkillGfxMan()->GetPtr();
}

A3DSkillGfxComposerMan* AfxGetSkillGfxComposerMan()
{
	return g_pGame->GetGameRun()->GetSkillGfxComposerMan();
}

SkillGfxMan::SkillGfxMan(CECGameRun* pGameRun)
{
	m_pPlayerMan	= pGameRun->GetWorld()->GetPlayerMan();
	m_pNPCMan		= pGameRun->GetWorld()->GetNPCMan();
	SetDamageShowFunc(ShowDamageInfo);
	SetCasterShowFunc(ShowCasterInfo);
	SetSkillGfxTickFunc(SkillGfxTickFunc);
}

void WINAPI SkillGfxMan::ShowDamageInfo(__int64 idCaster, __int64 idTarget, int nDamage, int nDivisions, DWORD dwModifier)
{
	CECGameRun* pGameRun = g_pGame->GetGameRun();
	const ACHAR* szDstName = NULL;

	int img_attacker = 0;
	int img_defender = 0;
	// first of all see if we need do skill spec things.
	if (ISPLAYERID(idCaster) && (dwModifier & MOD_SKILL_SPEC) && (dwModifier & MOD_SKILL))
	{
		CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(static_cast<int>(idCaster));

		if (pPlayer)
		{
			abase::vector<CScriptValue> args;
			args.push_back(CScriptValue((double)pPlayer->GetProfession()));

			abase::vector<CScriptValue> results;
			LuaBind::Call(g_LuaStateMan.GetConfigState()->GetVM(), "title_dir", "GetSkillSpec", args, results);
			if( results.size() == 2 )
			{
				img_attacker = results[0].GetInt();
				img_defender = results[1].GetInt();
			}
		}
	}

	if (ISPLAYERID(idTarget))
	{
		CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(static_cast<int>(idTarget));

		if (pPlayer)
		{
			pPlayer->Damaged(nDamage, nDivisions, dwModifier);
			szDstName = pPlayer->GetName();

			if( img_defender )
				pPlayer->BubbleText(CECPlayer::BUBBLE_SKILL_SPEC, 1, 0, img_defender - 1);
		}
	}
	else if (ISNPCID(idTarget))
	{
		CECNPC* pNPC = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(static_cast<int>(idTarget));

		if (pNPC)
		{
			pNPC->Damaged(nDamage, nDivisions, dwModifier);
			szDstName = pNPC->GetName();

			if( img_defender )
				pNPC->BubbleText(CECNPC::BUBBLE_SKILL_SPEC, 1, img_defender - 1);
		}
	}
	
	// 竞技场中去掉伤害提示信息
	if(pGameRun->GetHostPlayer()->GetBattleType() == BATTLE_TYPE_ARENA ||
		pGameRun->GetHostPlayer()->GetBattleType() == BATTLE_TYPE_ARENAREBORN ||
		pGameRun->GetHostPlayer()->GetBattleType() == BATTLE_TYPE_CRSSVR)
		return;
	// 匿名地图中，去掉伤害提示信息
	if (pGameRun->GetHostPlayer()->IsInAnonymousMap())
		return;

	if (nDamage > 0)
	{
		int idHostPlayer = pGameRun->GetHostPlayer()->GetCharacterID();
		if (idCaster == idHostPlayer)
		{
			if (szDstName)
				pGameRun->AddFixedChannelMsg(FIXMSG_DODAMAGE, GP_CHAT_DAMAGE, szDstName, nDamage);
		}
		else if (idTarget == idHostPlayer)
		{
			//	Get caster name
			const ACHAR* szCasterName = NULL;
			CECObject* pCaster = pGameRun->GetWorld()->GetObject(static_cast<int>(idCaster), 0);
			if (pCaster)
			{
				if (ISNPCID(idCaster))
					szCasterName = ((CECNPC*)pCaster)->GetName();
				else if (ISPLAYERID(idCaster))
					szCasterName = ((CECPlayer*)pCaster)->GetName();
			}

			if (szCasterName)
				pGameRun->AddFixedChannelMsg(FIXMSG_BEDAMAGED, GP_CHAT_DAMAGE, szCasterName, nDamage);
		}
	}
}

void WINAPI SkillGfxMan::ShowCasterInfo(__int64 idCaster, DWORD dwModifier)
{
	CECGameRun* pGameRun = g_pGame->GetGameRun();
	
	int img_attacker = 0;
	int img_defender = 0;
	// first of all see if we need do skill spec things.
	if (ISPLAYERID(idCaster) && (dwModifier & MOD_SKILL_SPEC) && (dwModifier & MOD_SKILL))
	{
		CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(static_cast<int>(idCaster));

		if (pPlayer)
		{
			abase::vector<CScriptValue> args;
			args.push_back(CScriptValue((double)pPlayer->GetProfession()));

			abase::vector<CScriptValue> results;
			LuaBind::Call(g_LuaStateMan.GetConfigState()->GetVM(), "title_dir", "GetSkillSpec", args, results);
			if( results.size() == 2 )
			{
				img_attacker = results[0].GetInt();
				img_defender = results[1].GetInt();

				if( img_attacker )
					pPlayer->BubbleText(CECPlayer::BUBBLE_SKILL_SPEC, 1, 0, img_attacker - 1);
			}
		}
	}
}

void WINAPI SkillGfxMan::SkillGfxTickFunc(__int64 idCaster, int state, DWORD dwTickTime, const A3DVECTOR3* pDir, const A3DVECTOR3* pPos)
{
	if (ISPLAYERID(idCaster))
	{
		CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(static_cast<int>(idCaster));

		if (pPlayer)
		{
			CECSprite* pSprite = pPlayer->GetSpriteModel();

			if (pSprite && state == enumAttStateHit)
				pSprite->SetNoRender(false);
		}
	}
}

bool SkillGfxMan::GetPositionById(__int64 nID, A3DVECTOR3& vPos, GfxHitPos HitPos, const char* szHook, bool bRelHook, const A3DVECTOR3* pOffset, const char* szHanger, bool bChildHook)
{
	if (ISPLAYERID(nID))
	{
		int id32 = static_cast<int>(nID);
		CECPlayer* pPlayer = NULL;
		if(ISSPIRITPLAYERID(nID))
		{
			int nCharacterID = id32 & 0x03ffffff;		// character id
			int nSpiritIndex = (id32 & 0x0c000000)>>26;

			pPlayer = m_pPlayerMan->GetPlayer(nCharacterID);
			if(pPlayer)
			{
				CECPlayer* pSpiritPlayer = (CECPlayer*)pPlayer->GetSpiritModel(nSpiritIndex);
				if(pSpiritPlayer)
					pPlayer = pSpiritPlayer;
			}
		}
		else
			pPlayer = m_pPlayerMan->GetPlayer(id32);

		if (pPlayer)
		{
			if (szHook)
			{
				CECModel* pModel = pPlayer->GetPlayerModel();

				if (pModel)
				{
					if (szHanger && bChildHook)
						pModel = pModel->GetChildModel(szHanger);

					if (pModel)
					{
						A3DSkinModel* pSkin = pModel->GetA3DSkinModel();
						A3DSkeletonHook* pHook = pSkin->GetSkeletonHook(szHook, true);

						if (pHook)
						{
							if (bRelHook)
								vPos = pHook->GetAbsoluteTM() * (*pOffset);
							else
							{
								vPos = pSkin->GetAbsoluteTM() * (*pOffset);
								vPos = vPos - pSkin->GetAbsoluteTM().GetRow(3) + pHook->GetAbsoluteTM().GetRow(3);
							}

							return true;
						}
					}
				}
			}

			if (HitPos == enumHitBottom)
				vPos = pPlayer->GetPos();
			else
			{
				const A3DAABB& aabb = pPlayer->GetPlayerAABB();
				vPos = aabb.Center;
				vPos.y += aabb.Extents.y * .5f;
				// 如果是被攻击的玩家多人骑乘，只能攻击主骑，而且需要更精确的确定位置
				if (pPlayer->IsMultiRideLeader() && pPlayer->GetA3DSkinModel())
				{
					vPos = pPlayer->GetA3DSkinModel()->GetPos();
					vPos.y += aabb.Extents.y;
				}
			}

			if (pOffset)
				vPos += *pOffset;

			return true;
		}
	}
	else if (ISNPCID(nID))
	{
		CECNPC* pNPC = m_pNPCMan->GetNPCFromAll(static_cast<int>(nID));

		if (pNPC)
		{
			if (HitPos == enumHitBottom)
				vPos = pNPC->GetPos();
			else
			{
				const A3DAABB& aabb = pNPC->GetPickAABB();
				vPos = aabb.Center;
				vPos.y += aabb.Extents.y * .5f;
			}

			return true;
		}
	}

	return false;
}

bool SkillGfxMan::GetDirAndUpById(__int64 nId, A3DVECTOR3& vDir, A3DVECTOR3& vUp)
{
	if (ISPLAYERID(nId))
	{
		CECPlayer* pPlayer = m_pPlayerMan->GetPlayer(static_cast<int>(nId));

		if (pPlayer)
		{
			vDir = pPlayer->GetDir();
			vUp = pPlayer->GetUp();
			return true;
		}
	}
	else if (ISNPCID(nId))
	{
		CECNPC* pNPC = m_pNPCMan->GetNPCFromAll(static_cast<int>(nId));

		if (pNPC)
		{
			vDir = pNPC->GetDir();
			vUp = pNPC->GetUp();
			return true;
		}
	}

	return false;
}

float SkillGfxMan::GetTargetScale(__int64 nID)
{
	if (ISPLAYERID(nID))
	{
		CECPlayer* pPlayer = m_pPlayerMan->GetPlayer(static_cast<int>(nID));

		if (pPlayer)
		{
			const A3DAABB& aabb = pPlayer->GetPlayerAABB();
			return aabb.Maxs.y - aabb.Mins.y;
		}
	}
	else if (ISNPCID(nID))
	{
		CECNPC* pNPC = m_pNPCMan->GetNPCFromAll(static_cast<int>(nID));

		if (pNPC)
		{
			const A3DAABB& aabb = pNPC->GetPickAABB();
			return aabb.Maxs.y - aabb.Mins.y;
		}
	}

	return 1.0f;
}

A3DGFXEx* CECSkillGfxEvent::LoadHitGfx(A3DDevice* pDev, const char* szPath, DWORD dwModifier)
{
	if (dwModifier & MOD_NULLITY) szPath = "程序联入\\击中\\无效攻击击中.gfx";
	return AfxGetGFXExMan()->LoadGfx(pDev, szPath);
}

void CECSkillGfxEvent::PlayExtraHitGfx(DWORD dwModifier, const A3DVECTOR3& vPos)
{
	// now show some special hit gfx
//	if (dwModifier & MOD_PHYSIC_ATTACK_RUNE)
//		g_pGame->GetGFXCaster()->PlayAutoGFXEx("程序联入\\击中\\物攻符击中.gfx", 60000, vPos,
//			g_vAxisZ, g_vAxisY, 0.0f);

//	if (dwModifier & MOD_MAGIC_ATTACK_RUNE )
//		g_pGame->GetGFXCaster()->PlayAutoGFXEx("程序联入\\击中\\法攻符击中.gfx", 60000, vPos,
//			g_vAxisZ, g_vAxisY, 0.0f);
}

}

CECSkillGfxMan::CECSkillGfxMan(CECGameRun* pGameRun) :
CECManager(pGameRun),
m_GfxMan(pGameRun)
{
	m_GfxMan.Init(g_pGame->GetA3DDevice());
}
