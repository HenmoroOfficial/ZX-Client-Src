#include "EC_Global.h"
#include "gnet.h"
#include "EC_NetPrtclObjectInfo.h"
#include "EC_GameSession.h"
#include "EC_GPDataType.h"
#include "EC_ElsePlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_HostPlayer.h"
#include "EC_ManNPC.h"
#include "EC_NPC.h"
#include "EC_ManMatter.h"
#include "EC_GameRecord.h"
#include "EC_GameUIMan.h"
#include "EC_UIManager.h"
#include "EC_IvtrItem.h"
#include "EC_Inventory.h"
#include "DlgRecord.h"
#include <AMSoundBufferMan.h>

extern game_record gr;

using namespace S2C;

void CECNetPrtclObjectInfo::Process_By_Object_Type(int idObject, CECGameSession* pSession)
{
	if (ISPLAYERID(idObject))
		Process_Player(pSession, idObject);
	else if (ISNPCID(idObject))
		Process_NPC(pSession, idObject);
	else if (ISMATTERID(idObject))
		Process_Matter(pSession, idObject);
}

CECNetProtocolBase* CECNetPrtclObjectMove::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclObjectMove)
}

bool CECNetPrtclObjectMove::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(cmd_object_move))
	{
		ASSERT(FALSE);
		return true;
	}

	Process_By_Object_Type(reinterpret_cast<cmd_object_move*>(m_pData)->id, pSession);
	return true;
}

void CECNetPrtclObjectMove::Process_Player(CECGameSession* pSession, int id)
{
	ECMSG msg;
	msg.dwMsg		= MSG_PM_PLAYERMOVE;
	msg.iManager	= MAN_PLAYER;
	msg.iSubID		= -1;
	msg.dwParam1	= (DWORD)m_pData;
	g_pGame->GetGameRun()->PostMessage(msg);
}

void CECNetPrtclObjectMove::Process_NPC(CECGameSession* pSession, int id)
{
	ECMSG msg;
	msg.dwMsg		= MSG_NM_NPCMOVE;
	msg.iManager	= MAN_NPC;
	msg.iSubID		= 0;
	msg.dwParam1	= (DWORD)m_pData;
	g_pGame->GetGameRun()->PostMessage(msg);
}

CECNetProtocolBase* CECNetPrtclObjectLeaveSlice::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclObjectLeaveSlice)
}

bool CECNetPrtclObjectLeaveSlice::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(cmd_leave_slice))
	{
		ASSERT(FALSE);
		return true;
	}

	Process_By_Object_Type(reinterpret_cast<cmd_leave_slice*>(m_pData)->id, pSession);
	return true;
}

void CECNetPrtclObjectLeaveSlice::Process_Player(CECGameSession* pSession, int id)
{
	ECMSG msg;
	msg.dwMsg		= MSG_PM_PLAYERRUNOUT;
	msg.iManager	= MAN_PLAYER;
	msg.iSubID		= -1;
	msg.dwParam1	= (DWORD)m_pData;
	g_pGame->GetGameRun()->PostMessage(msg);
}

void CECNetPrtclObjectLeaveSlice::Process_NPC(CECGameSession* pSession, int id)
{
	ECMSG msg;
	msg.dwMsg		= MSG_NM_NPCRUNOUT;
	msg.iManager	= MAN_NPC;
	msg.iSubID		= 0;
	msg.dwParam1	= (DWORD)m_pData;
	g_pGame->GetGameRun()->PostMessage(msg);
}

CECNetProtocolBase* CECNetPrtclObjectDisappear::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclObjectDisappear)
}

bool CECNetPrtclObjectDisappear::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(cmd_object_disappear))
	{
		ASSERT(FALSE);
		return true;
	}

	Process_By_Object_Type(reinterpret_cast<cmd_object_disappear*>(m_pData)->id, pSession);
	return true;
}

void CECNetPrtclObjectDisappear::Process_Player(CECGameSession* pSession, int id)
{
	ECMSG msg;
	msg.dwMsg		= MSG_PM_PLAYERDISAPPEAR;
	msg.iManager	= MAN_PLAYER;
	msg.iSubID		= -1;
	msg.dwParam1	= (DWORD)m_pData;
	g_pGame->GetGameRun()->PostMessage(msg);
}

void CECNetPrtclObjectDisappear::Process_NPC(CECGameSession* pSession, int id)
{
	ECMSG msg;
	msg.dwMsg		= MSG_NM_NPCDISAPPEAR;
	msg.iManager	= MAN_NPC;
	msg.iSubID		= 0;
	msg.dwParam1	= (DWORD)m_pData;
	g_pGame->GetGameRun()->PostMessage(msg);
}

void CECNetPrtclObjectDisappear::Process_Matter(CECGameSession* pSession, int id)
{
	ECMSG msg;
	msg.dwMsg		= MSG_MM_MATTERDISAPPEAR;
	msg.iManager	= MAN_MATTER;
	msg.iSubID		= 0;
	msg.dwParam1	= (DWORD)m_pData;
	g_pGame->GetGameRun()->PostMessage(msg);
}

CECNetProtocolBase* CECNetPrtclObjectOutOfSight::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclObjectOutOfSight)
}

bool CECNetPrtclObjectOutOfSight::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	cmd_out_of_sight_list cmd;

	if (!cmd.FillData(m_pData, m_nDataSize))
		return true;

	for (DWORD n = 0; n < cmd.uCount; n++)
		Process_By_Object_Type(cmd.id_list[n], pSession);

	return true;
}

void CECNetPrtclObjectOutOfSight::Process_Player(CECGameSession* pSession, int id)
{
	ECMSG msg;
	msg.dwMsg		= MSG_PM_PLAYEROUTOFVIEW;
	msg.iManager	= MAN_PLAYER;
	msg.iSubID		= -1;
	msg.dwParam1	= id;
	g_pGame->GetGameRun()->PostMessage(msg);
}

void CECNetPrtclObjectOutOfSight::Process_NPC(CECGameSession* pSession, int id)
{
	ECMSG msg;
	msg.dwMsg		= MSG_NM_NPCOUTOFVIEW;
	msg.iManager	= MAN_NPC;
	msg.iSubID		= 0;
	msg.dwParam1	= id;
	g_pGame->GetGameRun()->PostMessage(msg);
}

void CECNetPrtclObjectOutOfSight::Process_Matter(CECGameSession* pSession, int id)
{
	ECMSG msg;
	msg.dwMsg		= MSG_MM_MATTEROUTOFVIEW;
	msg.iManager	= MAN_MATTER;
	msg.iSubID		= 0;
	msg.dwParam1	= id;
	g_pGame->GetGameRun()->PostMessage(msg);
}
 
CECNetProtocolBase* CECNetPrtclObjectEnterDim::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclObjectEnterDim)
}

bool CECNetPrtclObjectEnterDim::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(enter_dim_state))
	{
		ASSERT(FALSE);
		return true;
	}

	Process_By_Object_Type(reinterpret_cast<enter_dim_state*>(m_pData)->id, pSession);

	return true;
}

void CECNetPrtclObjectEnterDim::Process_Player(CECGameSession* pSession, int id)
{
	enter_dim_state* p = (enter_dim_state*)m_pData;
	CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(p->id);
	if (pPlayer)
		pPlayer->EnterDimState(p->state);
}

void CECNetPrtclObjectEnterDim::Process_NPC(CECGameSession* pSession, int id)
{
	enter_dim_state* p = (enter_dim_state*)m_pData;
	CECNPC* pNPC = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(p->id);
	if (pNPC)
		pNPC->EnterDimState(p->state);
}

CECNetProtocolBase* CECNetPrtclRecordStart::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclRecordStart)
}

bool CECNetPrtclRecordStart::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(record_start))
	{
		assert(false);
		return true;
	}

	record_start* p = (record_start*)m_pData;
	gr.set_record_stamp(p->record_stamp);

	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	if (pUIMan)
	{
		AUIDialog* pDlg = pUIMan->GetDialog("Win_Record");

		if (pDlg)
			pDlg->Show(true);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcNPCServiceResult::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcNPCServiceResult)
}

bool CECNetPrtcNPCServiceResult::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(npc_service_result))
	{
		assert(false);
		return true;
	}

	npc_service_result* p = (npc_service_result*)m_pData;

	if (p->type == 0)
	{
		CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pUIMan->EndNPCService();
		pUIMan->GetDialog("Win_NPC")->Show(false);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcBattleFlagChange::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcBattleFlagChange)
}

bool CECNetPrtcBattleFlagChange::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(battle_flag_change))
	{
		assert(false);
		return true;
	}

	CECMatterMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetMatterMan();
	pMan->OnBattleFlagChange((const battle_flag_change*)m_pData);
	return true;
}

CECNetProtocolBase* CECNetPrtcBattleStart::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcBattleStart)
}

bool CECNetPrtcBattleStart::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	g_pGame->GetGameRun()->GetHostPlayer()->ResetGuildWarStartCounts();
	g_pGame->GetAMSoundBufferMan()->Play2DAutoSound("SFX\\程序联入\\全地图音效－开栅门.wav");
	return true;
}

CECNetProtocolBase* CECNetPrtcBattleFlagChangeStatus::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcBattleFlagChangeStatus)
}

bool CECNetPrtcBattleFlagChangeStatus::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(battle_flag_change_status))
	{
		assert(false);
		return true;
	}

	battle_flag_change_status* p = (battle_flag_change_status*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost->GetBattleCamp() == GP_BATTLE_CAMP_INVADER)
	{
		if (p->new_state == 2)
			g_pGame->GetAMSoundBufferMan()->Play2DAutoSound("SFX\\程序联入\\全地图音效－我方被开旗子.wav");
		else if (p->new_state == 1)
			g_pGame->GetAMSoundBufferMan()->Play2DAutoSound("SFX\\程序联入\\全地图音效－我方开了一个旗子1.wav");
	}
	else if (pHost->GetBattleCamp() == GP_BATTLE_CAMP_DEFENDER)
	{
		if (p->new_state == 1)
			g_pGame->GetAMSoundBufferMan()->Play2DAutoSound("SFX\\程序联入\\全地图音效－我方被开旗子.wav");
		else if (p->new_state == 2)
			g_pGame->GetAMSoundBufferMan()->Play2DAutoSound("SFX\\程序联入\\全地图音效－我方开了一个旗子1.wav");
	}

	return true;
}

/*
CECNetProtocolBase* CECNetPrtcPVPMode::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPVPMode)
}

bool CECNetPrtcPVPMode::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(pvp_mode))
	{
		assert(false);
		return true;
	}

	pvp_mode* p = (pvp_mode*)m_pData;
	g_pGame->GetGameRun()->SetPVPMode(p->mode != 0);

	return true;
}
*/

CECNetProtocolBase* CECNetPrtcAntiCheatBonus::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcAntiCheatBonus)
}

bool CECNetPrtcAntiCheatBonus::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(anti_cheat_bonus))
	{
		assert(false);
		return true;
	}

	anti_cheat_bonus* p = (anti_cheat_bonus*)m_pData;
	CECGameUIMan* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	if (pUIMan)
		pUIMan->AddChatMessage(pUIMan->GetStringFromTable(p->bonus_type + 906), GP_CHAT_SYSTEM);

	return true;
}

CECNetProtocolBase* CECNetPrtcObjectSpecialState::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcObjectSpecialState)
}

bool CECNetPrtcObjectSpecialState::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	if (m_nDataSize != sizeof(object_special_state))
	{
		assert(false);
		return true;
	}
	Process_By_Object_Type(reinterpret_cast<object_special_state*>(m_pData)->id, pSession);

	return true;
}

void CECNetPrtcObjectSpecialState::Process_Player(CECGameSession* pSession, int id)
{
	object_special_state* p = (object_special_state*)m_pData;
	CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(p->id);
	if (pPlayer)
		pPlayer->UpdateSpecialState(p->state, p->on == 0? false: true);
}

void CECNetPrtcObjectSpecialState::Process_NPC(CECGameSession* pSession, int id)
{
	object_special_state* p = (object_special_state*)m_pData;
	CECNPC* pNPC = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(p->id);
	if (pNPC)
		pNPC->UpdateSpecialState(p->state, p->on == 0? false: true);
}