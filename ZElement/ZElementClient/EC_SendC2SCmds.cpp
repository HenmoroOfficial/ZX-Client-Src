 /*
 * FILE: EC_SendC2SCmds.cpp
 *
 * DESCRIPTION: Functions used to C2S net commands
 *
 * CREATED BY: duyuxin, 2004/11/15
 *
 * HISTORY:
 *
 * Copyright (c) 2003 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_GameSession.h"
#include "EC_GPDataType.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_RoleTypes.h"
#include "EC_Utility.h"
#include "EC_HelpGraphGuide.h"
#include "defence/stackchecker.h"

#include "A3DTypes.h"
#include "AAssist.h"
#include "AChar.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

//	Send a nake command (commands that have id but havn't data)
static inline void _SendNakeCommand(int iCmd)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = iCmd;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

//	Send a pure data-buffer npc service
static void _SendDataBufNPCSev(int iSevice, int iAction, void* pDataBuf, int iDataSize)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int iAction;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT) + iDataSize;
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = iSevice;
	pCmd->len = sizeof (CONTENT) + iDataSize;

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->iAction = iAction;
	pData += sizeof (CONTENT);

	if (pDataBuf)
		memcpy(pData, pDataBuf, iDataSize);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

//	Send a empty data npc service
void _SendEmptyDataNPCSev(int iSevice)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = iSevice;
	pCmd->len = 0;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement
//	
///////////////////////////////////////////////////////////////////////////

void c2s_SendDebugCmd(int iCmd, int iNumParams, ...)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + iNumParams * sizeof (int);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = iCmd;

	if (iNumParams)
	{
		va_list vaList;
		va_start(vaList, iNumParams);

		int* pData = (int*)(pBuf + sizeof (cmd_header));
		for (int i=0; i < iNumParams; i++)
			*pData++ = va_arg(vaList, int);

		va_end(vaList);
	}

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendDebugCmd1(int iCmd, int iNumParams, ...)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header);
	if (iNumParams)
	{
		va_list vaList;
		va_start(vaList, iNumParams);
		
		for (int i=0; i < iNumParams; i+=2)
		{
			iSize += va_arg(vaList, int);
			va_arg(vaList, int);
		}
		
		va_end(vaList);
	}
	if (iSize>100||iSize<=0)
		return;

	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = iCmd;
	
	if (iNumParams)
	{
		va_list vaList;
		va_start(vaList, iNumParams);
		
		BYTE* pData = pBuf + sizeof (cmd_header);
		for (int i=0; i < iNumParams; i+=2)
		{
			int iType = va_arg(vaList, int);
			switch (iType)
			{
			case 1:
				*pData++ = va_arg(vaList, int);
				break;
			case 2:
				*((short*)pData) = va_arg(vaList, int);
				pData+=2;
				break;
			case 4:
				*((int*)pData) = va_arg(vaList, int);
				pData+=4;
				break;
			default:
				delete [] pBuf;
				return;
			}
		}
		
		va_end(vaList);
	}
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete [] pBuf;
}

//	Send C2S::PLAYER_MOVE command
void c2s_SendCmdPlayerMove(const A3DVECTOR3& vCurPos, const A3DVECTOR3& vDest, 
						   int iTime, float fSpeed, int iMoveMode, WORD wStamp)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_player_move);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = PLAYER_MOVE;

	cmd_player_move* pCmd = (cmd_player_move*)(pBuf + sizeof (cmd_header));
	pCmd->move_mode	= (BYTE)iMoveMode;
	pCmd->use_time	= (WORD)iTime;
	pCmd->vCurPos	= vCurPos;
	pCmd->vNextPos	= vDest;
	pCmd->sSpeed	= FLOATTOFIX8(fSpeed);
	pCmd->stamp		= wStamp;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

//	Send C2S::STOP_MOVE command
void c2s_SendCmdStopMove(const A3DVECTOR3& vDest, float fSpeed, int iMoveMode,
						 BYTE byDir, WORD wStamp, int iTime)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_stop_move);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = STOP_MOVE;

	cmd_stop_move* pCmd = (cmd_stop_move*)(pBuf + sizeof (cmd_header));
	pCmd->move_mode	= (BYTE)iMoveMode;
	pCmd->sSpeed	= FLOATTOFIX8(fSpeed);
	pCmd->dir		= byDir;
	pCmd->vCurPos	= vDest;
	pCmd->stamp		= wStamp;
	pCmd->use_time	= (WORD)iTime;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdStartMove()
{
	using namespace C2S;

	int iSize = sizeof (cmd_header);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = START_MOVE;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

//	Send C2S::SELECT_TARGET command
void c2s_SendCmdSelectTarget(int id)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_select_target);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = SELECT_TARGET;

	cmd_select_target* pCmd = (cmd_select_target*)(pBuf + sizeof (cmd_header));
	pCmd->id = id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

//	Send C2S::NORMAL_ATTACK command
void c2s_SendCmdNormalAttack(BYTE byPVPMask)
{
	StackChecker::ACTrace(1);

	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_normal_attack);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = NORMAL_ATTACK;

	cmd_normal_attack* pCmd = (cmd_normal_attack*)(pBuf + sizeof (cmd_header));
	pCmd->pvp_mask = byPVPMask;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	g_pGame->GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_5_1_ATTACK);

	delete [] pBuf;
}

//	Send C2S::PICKUP command
void c2s_SendCmdPickup(int idItem, int tid)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_pickup);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = PICKUP;

	cmd_pickup* pCmd = (cmd_pickup*)(pBuf + sizeof (cmd_header));
	pCmd->idItem	= idItem;
	pCmd->tid		= tid;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdUseItem(BYTE byPackage, BYTE bySlot, int tid, BYTE byCount)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_use_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = USE_ITEM;

	cmd_use_item* pCmd = (cmd_use_item*)(pBuf + sizeof (cmd_header));
	pCmd->where		= byPackage;
	pCmd->index		= bySlot;
	pCmd->item_id	= tid;
	pCmd->byCount	= byCount;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdUseItemWithTarget(BYTE byPackage, BYTE bySlot, int tid, BYTE byPVPMask)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_use_item_t);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = USE_ITEM_T;

	cmd_use_item_t* pCmd = (cmd_use_item_t*)(pBuf + sizeof (cmd_header));
	pCmd->where		= byPackage;
	pCmd->index		= bySlot;
	pCmd->item_id	= tid;
	pCmd->pvp_mask	= byPVPMask;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdThrowMoney(DWORD dwAmount)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_throw_money);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = THROW_MONEY;

	cmd_throw_money* pCmd = (cmd_throw_money*)(pBuf + sizeof (cmd_header));
	pCmd->amount = dwAmount;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdGetItemInfoList(int iPackage, int iCount, BYTE* aIndices)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_get_item_info_list) + iCount * sizeof (BYTE);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = GET_ITEM_INFO_LIST;

	BYTE* pTemp = pBuf + sizeof (cmd_header);
	cmd_get_item_info_list* pCmd = (cmd_get_item_info_list*)pTemp;
	pCmd->where	= (BYTE)iPackage;
	pCmd->count	= (BYTE)iCount;
	
	pTemp += sizeof (cmd_get_item_info_list);
	memcpy(pTemp, aIndices, iCount * sizeof (BYTE));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdGetTrashBoxData(bool bDetail)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_get_trashbox_info);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = GET_TRASHBOX_INFO;

	cmd_get_trashbox_info* pCmd = (cmd_get_trashbox_info*)(pBuf + sizeof (cmd_header));
	pCmd->detail = bDetail;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdExgTrashBoxItem(int iIndex1, int iIndex2)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_exg_trashbox_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = EXG_TRASHBOX_ITEM;

	cmd_exg_trashbox_item* pCmd = (cmd_exg_trashbox_item*)(pBuf + sizeof (cmd_header));
	pCmd->index1 = (BYTE)iIndex1;
	pCmd->index2 = (BYTE)iIndex2;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdMoveTrashBoxItem(int iSrc, int iDst, int iAmount)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_move_trashbox_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = MOVE_TRASHBOX_ITEM;

	cmd_move_trashbox_item* pCmd = (cmd_move_trashbox_item*)(pBuf + sizeof (cmd_header));
	pCmd->src		= (BYTE)iSrc;
	pCmd->dest		= (BYTE)iDst;
	pCmd->amount	= iAmount;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdExgTrashBoxIvtrItem(int iTrashIdx, int iIvtrIdx)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_exg_trashbox_ivtr);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = EXG_TRASHBOX_IVTR;

	cmd_exg_trashbox_ivtr* pCmd = (cmd_exg_trashbox_ivtr*)(pBuf + sizeof (cmd_header));
	pCmd->idx_inv	= (BYTE)iIvtrIdx;
	pCmd->idx_tra	= (BYTE)iTrashIdx;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdMoveTrashBoxToIvtr(int iTrashIdx, int iIvtrIdx, int iAmount)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_trashbox_item_to_ivtr);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = TRASHBOX_ITEM_TO_IVTR;

	cmd_trashbox_item_to_ivtr* pCmd = (cmd_trashbox_item_to_ivtr*)(pBuf + sizeof (cmd_header));
	pCmd->idx_tra	= (BYTE)iTrashIdx;
	pCmd->idx_inv	= (BYTE)iIvtrIdx;
	pCmd->amount	= iAmount;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdMoveIvtrToTrashBox(int iTrashIdx, int iIvtrIdx, int iAmount)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_ivtr_item_to_trashbox);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = IVTR_ITEM_TO_TRASHBOX;

	cmd_ivtr_item_to_trashbox* pCmd = (cmd_ivtr_item_to_trashbox*)(pBuf + sizeof (cmd_header));
	pCmd->idx_tra	= (BYTE)iTrashIdx;
	pCmd->idx_inv	= (BYTE)iIvtrIdx;
	pCmd->amount	= iAmount;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdExgTrashBoxMoney(int iTrashMoney, int iIvtrMoney)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_exg_trashbox_money);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = EXG_TRASHBOX_MONEY;

	cmd_exg_trashbox_money* pCmd = (cmd_exg_trashbox_money*)(pBuf + sizeof (cmd_header));
	pCmd->trashbox_money	= iTrashMoney;
	pCmd->inv_money			= iIvtrMoney;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

// 新增随身包裹
void c2s_SendCmdExgPocketItem(int iIndex1, int iIndex2)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_exg_pocket_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = EXG_POCKET_ITEM;

	cmd_exg_pocket_item* pCmd = (cmd_exg_pocket_item*)(pBuf + sizeof (cmd_header));
	pCmd->index1 = (BYTE)iIndex1;
	pCmd->index2 = (BYTE)iIndex2;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdMovePocketItem(int iSrc, int iDst, int iAmount)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_move_pocket_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = MOVE_POCKET_ITEM;

	cmd_move_pocket_item* pCmd = (cmd_move_pocket_item*)(pBuf + sizeof (cmd_header));
	pCmd->src		= (BYTE)iSrc;
	pCmd->dest		= (BYTE)iDst;
	pCmd->amount	= iAmount;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdExgPocketIvtrItem(int iPocIdx, int iIvtrIdx)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_exg_pocket_ivtr);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = EXG_POCKET_IVTR;

	cmd_exg_pocket_ivtr* pCmd = (cmd_exg_pocket_ivtr*)(pBuf + sizeof (cmd_header));
	pCmd->idx_inv	= (BYTE)iIvtrIdx;
	pCmd->idx_poc	= (BYTE)iPocIdx;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdMovePocketToIvtr(int iPocIdx, int iIvtrIdx, int iAmount)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_pocket_item_to_ivtr);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = POCKET_ITEM_TO_IVTR;

	cmd_pocket_item_to_ivtr* pCmd = (cmd_pocket_item_to_ivtr*)(pBuf + sizeof (cmd_header));
	pCmd->idx_poc	= (BYTE)iPocIdx;
	pCmd->idx_inv	= (BYTE)iIvtrIdx;
	pCmd->amount	= iAmount;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdMoveIvtrToPocket(int iPocIdx, int iIvtrIdx, int iAmount)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_ivtr_item_to_pocket);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = IVTR_ITEM_TO_POCKET;

	cmd_ivtr_item_to_pocket* pCmd = (cmd_ivtr_item_to_pocket*)(pBuf + sizeof (cmd_header));
	pCmd->idx_poc	= (BYTE)iPocIdx;
	pCmd->idx_inv	= (BYTE)iIvtrIdx;
	pCmd->amount	= iAmount;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdMovePocketAllToIvtr()
{
	using namespace C2S;

	int iSize = sizeof (cmd_header);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = POCKET_ITEM_ALL_TO_IVTR;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}


void c2s_SendCmdGetOtherEquipDetail(int idTarget)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_get_other_equip_detail);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = GET_OTHER_EQUIP_DETAIL;

	cmd_get_other_equip_detail* pCmd = (cmd_get_other_equip_detail*)(pBuf + sizeof (cmd_header));
	pCmd->target = idTarget;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}


void c2s_SendCmdGetCloneEquip(int idMaster, int idClone)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_get_clone_equip);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = GET_CLONE_EQUIP;

	cmd_get_clone_equip* pCmd = (cmd_get_clone_equip*)(pBuf + sizeof (cmd_header));
	pCmd->idMaster = idMaster;
	pCmd->idClone  = idClone;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}



//	Send C2S::REVIVE_VILLAGE command
void c2s_SendCmdReviveVillage()
{
	_SendNakeCommand(C2S::REVIVE_VILLAGE);
}

//	Send C2S::REVIVE_ITEM command
void c2s_SendCmdReviveItem()
{
	_SendNakeCommand(C2S::REVIVE_ITEM);
}

//	Send C2S::GOTO command data
void c2s_SendCmdGoto(float x, float y, float z)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_goto);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GOTO;

	cmd_goto* pCmd = (cmd_goto*)(pBuf + sizeof (cmd_header));
	pCmd->vDest.Set(x, y, z);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

//	Send C2S::GET_ITEM_INFO commadn data
void c2s_SendCmdGetItemInfo(BYTE byPackage, BYTE bySlot)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_get_item_info);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GET_ITEM_INFO;

	cmd_get_item_info* pCmd = (cmd_get_item_info*)(pBuf + sizeof (cmd_header));
	pCmd->byPackage	= byPackage;
	pCmd->bySlot	= bySlot;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

//	Send C2S::GET_IVTR commadn data
void c2s_SendCmdGetIvtrData(int iPackage)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_get_ivtr);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GET_IVTR;

	cmd_get_ivtr* pCmd = (cmd_get_ivtr*)(pBuf + sizeof (cmd_header));
	pCmd->byPackage	= (BYTE)iPackage;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

//	Send C2S::GET_IVTR_DETAIL commadn data
void c2s_SendCmdGetIvtrDetailData(int iPackage)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_get_ivtr_detail);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GET_IVTR_DETAIL;

	cmd_get_ivtr_detail* pCmd = (cmd_get_ivtr_detail*)(pBuf + sizeof (cmd_header));
	pCmd->byPackage	= (BYTE)iPackage;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

//	Send C2S::GET_EXT_PROP commadn data
void c2s_SendCmdGetExtProps()
{
	_SendNakeCommand(C2S::GET_EXT_PROP);
}

//	Send C2S::GET_SUMMON_PETPROP commadn data
void c2s_SendCmdGetPetProps()
{
	_SendNakeCommand(C2S::GET_SUMMON_PETPROP);
}

void c2s_SendCmdGetPartExtProps(int iPropIndex)
{
	using namespace C2S;

	int iCmd = 0;

	switch (iPropIndex)
	{
	case EXTPROPIDX_BASE:	iCmd = GET_EXT_PROP_BASE;	break;
	case EXTPROPIDX_MOVE:	iCmd = GET_EXT_PROP_MOVE;	break;
	case EXTPROPIDX_ATTACK:	iCmd = GET_EXT_PROP_ATK;	break;
	case EXTPROPIDX_DEF:	iCmd = GET_EXT_PROP_DEF;	break;
	default:
		ASSERT(0);
		return;
	}

	_SendNakeCommand(iCmd);
}

void c2s_SendCmdLearnSkill(int idSkill)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_learn_skill);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::LEARN_SKILL;

	cmd_learn_skill* pCmd = (cmd_learn_skill*)(pBuf + sizeof (cmd_header));
	pCmd->skill_id = idSkill;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdLogout(int iOutType, int iTrusteeMode)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_player_logout);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::LOGOUT;

	cmd_player_logout* pCmd = (cmd_player_logout*)(pBuf + sizeof (cmd_header));
	pCmd->iOutType = iOutType;
	pCmd->iTrusteeMode = iTrusteeMode;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdGetAllData(bool bpd, bool bed, bool btd, bool bfd)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_get_all_data);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GET_ALL_DATA;

	cmd_get_all_data* pCmd = (cmd_get_all_data*)(pBuf + sizeof (cmd_header));
	pCmd->byPack	= (BYTE)bpd;
	pCmd->byEquip	= (BYTE)bed;
	pCmd->byTask	= (BYTE)btd;
//	pCmd->byFashion = (BYTE)bfd;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdChargeEquipFlySword(int iEleIdx, int iCount)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_charge_equipped_flysword);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::CHARGE_E_FLYSWORD;

	cmd_charge_equipped_flysword* pCmd = (cmd_charge_equipped_flysword*)(pBuf + sizeof (cmd_header));
	pCmd->element_index	= iEleIdx;
	pCmd->count			= iCount;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdChargeFlySword(int iEleIdx, int iFSIdx, int iCount, int idFlySword)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_charge_flysword);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::CHARGE_FLYSWORD;

	cmd_charge_flysword* pCmd = (cmd_charge_flysword*)(pBuf + sizeof (cmd_header));
	pCmd->element_index		= iEleIdx;
	pCmd->flysword_index	= iFSIdx;
	pCmd->count				= iCount;
	pCmd->flysword_id		= idFlySword;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdCancelAction()
{
	_SendNakeCommand(C2S::CANCEL_ACTION);
}

void c2s_SendCmdSitDown()
{
	_SendNakeCommand(C2S::SIT_DOWN);
}

void c2s_SendCmdStandUp()
{
	_SendNakeCommand(C2S::CANCEL_ACTION);
}

void c2s_SendCmdEmoteAction(WORD wPose)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_emote_action);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::EMOTE_ACTION;

	cmd_emote_action* pCmd = (cmd_emote_action*)(pBuf + sizeof (cmd_header));
	pCmd->action = wPose;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdTaskNotify(const void* pData, DWORD dwDataSize)
{
	using namespace C2S;

	if (!pData || !dwDataSize)
		return;

	int iSize = sizeof (cmd_header) + sizeof (DWORD) + dwDataSize;
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::TASK_NOTIFY;

	cmd_task_notify* pCmd = (cmd_task_notify*)(pBuf + sizeof (cmd_header));
	pCmd->size = dwDataSize;

	memcpy(&pCmd->placeholder, pData, dwDataSize);
		
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdUnselect()
{
	_SendNakeCommand(C2S::UNSELECT);
}

void c2s_SendCmdContinueAction()
{
	_SendNakeCommand(C2S::CONTINUE_ACTION);
}

void c2s_SendCmdStopFall()
{
	_SendNakeCommand(C2S::STOP_FALL);
}

void c2s_SendCmdGatherMaterial(int idMatter, int iToolPack, int iToolIdx, int idTool, int idTask)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_gather_material);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GATHER_MATERIAL;

	cmd_gather_material* pCmd = (cmd_gather_material*)(pBuf + sizeof (cmd_header));
	pCmd->mid			= idMatter;
	pCmd->tool_pack		= (unsigned short)iToolPack;
	pCmd->tool_index	= (unsigned short)iToolIdx;
	pCmd->tool_type		= idTool;
	pCmd->id_task		= idTask;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdCastSkill(int idSkill, BYTE byPVPMask, int iNumTarget, int* aTargets, A3DVECTOR3* pPos)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_cast_skill) - sizeof (int) + 
				iNumTarget * sizeof (int);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::CAST_SKILL;

	cmd_cast_skill* pCmd = (cmd_cast_skill*)(pBuf + sizeof (cmd_header));
	pCmd->skill_id		= idSkill;
	pCmd->pvp_mask		= byPVPMask;
	pCmd->target_count	= iNumTarget;
	if ( pPos )
	{
		pCmd->cast_skill_pos = *pPos;
	}

	if (iNumTarget)
		memcpy(pCmd->targets, aTargets, iNumTarget * sizeof (int));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdCastInstantSkill(int idSkill, BYTE byPVPMask, int iNumTarget, int* aTargets, A3DVECTOR3* pPos)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_cast_skill) - sizeof (int) + 
				iNumTarget * sizeof (int);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::CAST_INSTANT_SKILL;

	cmd_cast_skill* pCmd = (cmd_cast_skill*)(pBuf + sizeof (cmd_header));
	pCmd->skill_id		= idSkill;
	pCmd->pvp_mask		= byPVPMask;
	pCmd->target_count	= iNumTarget;
	if ( pPos )
	{
		pCmd->cast_skill_pos = *pPos;
	}

	if (iNumTarget)
		memcpy(pCmd->targets, aTargets, iNumTarget * sizeof (int));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdCastApoiseSkill(int idSkill, BYTE bySkillType, int item_id, short item_index, BYTE force_attack, int iNumTarget, int* aTargets)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_cast_apoise_skill) - sizeof (int) + 
				iNumTarget * sizeof (int);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::CAST_APOISE_SKILL;

	cmd_cast_apoise_skill* pCmd = (cmd_cast_apoise_skill*)(pBuf + sizeof (cmd_header));
	pCmd->skill_id		= idSkill;
	pCmd->skill_type	= bySkillType;
	pCmd->item_id		= item_id;
	pCmd->item_index	= item_index;
	pCmd->force_attack  = force_attack;
	pCmd->target_count	= iNumTarget;

	if (iNumTarget)
		memcpy(pCmd->targets, aTargets, iNumTarget * sizeof (int));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdCastPosSkill(int idSkill, const A3DVECTOR3& vPos)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_cast_pos_skill);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::CAST_POS_SKILL;

	cmd_cast_pos_skill* pCmd = (cmd_cast_pos_skill*)(pBuf + sizeof (cmd_header));
	pCmd->skill_id	= idSkill;
	pCmd->pos		= vPos;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdCastTransfigureSkill(int idSkill, char skill_type,  BYTE byPVPMask, int iNumTarget, int* aTargets, A3DVECTOR3* pPos)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cast_transfigure_skill) - sizeof (int) + 
				iNumTarget * sizeof (int);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::CAST_TRANSFIGURE_SKILL;

	cast_transfigure_skill* pCmd = (cast_transfigure_skill*)(pBuf + sizeof (cmd_header));
	pCmd->skill_id		= idSkill;
	pCmd->skill_type	= skill_type;
	pCmd->pvp_mask		= byPVPMask;
	pCmd->target_count	= iNumTarget;
	if ( pPos )
	{
		pCmd->cast_skill_pos = *pPos;
	}

	if (iNumTarget)
		memcpy(pCmd->targets, aTargets, iNumTarget * sizeof (int));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdCastChargeSkill(int idSkill,  BYTE byPVPMask, int idTarget, const A3DVECTOR3& vPos, int iNumTarget, int* aTargets)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cast_charge_skill) - sizeof (int) + 
				iNumTarget * sizeof(int);

	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::CAST_CHARGE_SKILL;

	cast_charge_skill* pCmd = (cast_charge_skill*)(pBuf + sizeof (cmd_header));
	pCmd->skill_id		= idSkill;
	pCmd->force_attack	= byPVPMask;
	pCmd->charge_target = idTarget;
	pCmd->charge_pos	= vPos;
	pCmd->target_count	= iNumTarget;

	if (iNumTarget)
		memcpy(pCmd->targets, aTargets, iNumTarget * sizeof(int));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdTrickAction(int iAction)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_trick_action);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::TRICK_ACTION;

	cmd_trick_action* pCmd = (cmd_trick_action*)(pBuf + sizeof (cmd_header));
	pCmd->trick	= (BYTE)iAction;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdSetAdvData(int iData1, int iData2)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_set_adv_data);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SET_ADV_DATA;

	cmd_set_adv_data* pCmd = (cmd_set_adv_data*)(pBuf + sizeof (cmd_header));
	pCmd->data1	= iData1;
	pCmd->data2	= iData2;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdClearAdvData()
{
	_SendNakeCommand(C2S::CLR_ADV_DATA);
}

void c2s_SendCmdQueryPlayerInfo1(int iNum, int* aIDs)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_query_player_info_1) - sizeof (int) + 
				iNum * sizeof (int);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::QUERY_PLAYER_INFO_1;

	cmd_query_player_info_1* pCmd = (cmd_query_player_info_1*)(pBuf + sizeof (cmd_header));
	pCmd->count	= (WORD)iNum;

	if (iNum)
		memcpy(pCmd->id, aIDs, iNum * sizeof (int));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdQueryNPCInfo1(int iNum, int* aIDs)
{
	if (!iNum || !aIDs)
		return;

	using namespace C2S;

	static const int iNumLimit = 200;
	int iCount = 0;
	
	while (iCount < iNum)
	{
		int iNumSend = iNumLimit;
		if (iCount + iNumLimit > iNum)
			iNumSend = iNum - iCount;

		int iSize = sizeof (cmd_header) + sizeof (cmd_query_npc_info_1) - sizeof (int) + 
					iNumSend * sizeof (int);
		BYTE* pBuf = new BYTE [iSize];
		if (!pBuf)
			return;
		
		((cmd_header*)pBuf)->cmd = C2S::QUERY_NPC_INFO_1;

		cmd_query_npc_info_1* pCmd = (cmd_query_npc_info_1*)(pBuf + sizeof (cmd_header));
		pCmd->count	= (WORD)iNumSend;

		memcpy(pCmd->id, &aIDs[iCount], iNumSend * sizeof (int));
		iCount += iNumSend;

		g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

		delete [] pBuf;
	}
}

void c2s_SendCmdSessionEmote(int iAction)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_session_emote);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SESSION_EMOTE;

	cmd_session_emote* pCmd = (cmd_session_emote*)(pBuf + sizeof (cmd_header));
	pCmd->action = (BYTE)iAction;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdConEmoteRequest(int iAction, int idTarget)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_con_emote_request);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::CON_EMOTE_REQUEST;

	cmd_con_emote_request* pCmd = (cmd_con_emote_request*)(pBuf + sizeof (cmd_header));
	pCmd->action = (WORD)iAction;
	pCmd->target = idTarget;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdConEmoteReply(int iResult, int iAction, int idTarget)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_con_emote_reply);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::CON_EMOTE_REPLY;

	cmd_con_emote_reply* pCmd = (cmd_con_emote_reply*)(pBuf + sizeof (cmd_header));
	pCmd->result = (WORD)iResult;
	pCmd->action = (WORD)iAction;
	pCmd->target = idTarget;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdDeadMove(float y, int iTime, float fSpeed, int iMoveMode)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_dead_move);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::DEAD_MOVE;

	cmd_dead_move* pCmd = (cmd_dead_move*)(pBuf + sizeof (cmd_header));
	pCmd->y			= y;
	pCmd->move_mode	= (BYTE)iMoveMode;
	pCmd->use_time	= (WORD)iTime;
	pCmd->speed		= FLOATTOFIX8(fSpeed);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdDeadStopMove(float y, float fSpeed, BYTE byDir, int iMoveMode)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_dead_stop_move);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::DEAD_STOP_MOVE;

	cmd_dead_stop_move* pCmd = (cmd_dead_stop_move*)(pBuf + sizeof (cmd_header));
	pCmd->y			= y;
	pCmd->dir		= byDir;
	pCmd->move_mode	= (BYTE)iMoveMode;
	pCmd->speed		= FLOATTOFIX8(fSpeed);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdEnterSanctuary()
{
	_SendNakeCommand(C2S::ENTER_SANCTUARY);
}

void c2s_SendCmdOpenBooth(int iNumEntry, const void* szName, void* aEntries, int iBoothItemID, int iBoothItemSlot)
{
	using namespace C2S;

	enum
	{
		SIZE_NAMEBUF = 28
	};

	int iSize = sizeof (cmd_header) + 2 * sizeof(int) + sizeof (WORD) + SIZE_NAMEBUF + iNumEntry * sizeof (cmd_open_booth::entry_t);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::OPEN_BOOTH;

	cmd_open_booth* pCmd = (cmd_open_booth*)(pBuf + sizeof (cmd_header));
	pCmd->slot_index = iBoothItemSlot;
	pCmd->item_id = iBoothItemID;
	pCmd->count	= iNumEntry;

	//	Copy name
	int iNameSize = (wcslen((wchar_t*)szName) + 1) * sizeof (wchar_t);
	if (iNameSize > SIZE_NAMEBUF)
	{
		ASSERT(iNameSize <= SIZE_NAMEBUF);
		return;
	}

	memcpy(pCmd->name, szName, iNameSize);

	//	Copy entry data
	if (iNumEntry)
		memcpy(pCmd->list, aEntries, iNumEntry * sizeof (cmd_open_booth::entry_t));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdCloseBooth()
{
	_SendNakeCommand(C2S::CLOSE_BOOTH);
}

void c2s_SendCmdQueryBoothName(int iNum, int* aIDs)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (WORD) + iNum * sizeof (int);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::QUERY_BOOTH_NAME;

	cmd_query_booth_name* pCmd = (cmd_query_booth_name*)(pBuf + sizeof (cmd_header));
	pCmd->count	= (WORD)iNum;

	if (iNum)
		memcpy(pCmd->list, aIDs, iNum * sizeof (int));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdCompleteTravel()
{
	_SendNakeCommand(C2S::COMPLETE_TRAVEL);
}

void c2s_SendCmdEnalbePVP()
{
	_SendNakeCommand(C2S::ENABLE_PVP_STATE);
}

void c2s_SendCmdDisablePVP()
{
	_SendNakeCommand(C2S::DISABLE_PVP_STATE);
}

void c2s_SendCmdOpenBoothTest(int iBoothItemID, int iBoothItemSlot)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_open_booth_test);
	BYTE* pBuf = new BYTE [iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::OPEN_BOOTH_TEST;
	
	cmd_open_booth_test* pCmd = (cmd_open_booth_test*)(pBuf + sizeof (cmd_header));
	pCmd->slot_index = iBoothItemSlot;
	pCmd->item_id = iBoothItemID;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdFashionSwitch()
{
	_SendNakeCommand(C2S::SWITCH_FASHION_MODE);
}

void c2s_SendCmdEnterInstance(int iTransIdx, int idInst)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_enter_instance);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::ENTER_INSTANCE;

	cmd_enter_instance* pCmd = (cmd_enter_instance*)(pBuf + sizeof (cmd_header));
	pCmd->iTransIndex	= iTransIdx;
	pCmd->idInst		= idInst;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdRevivalAgree()
{
	_SendNakeCommand(C2S::REVIVAL_AGREE);
}

void c2s_SendCmdActiveRushFly(bool bActive)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_active_rush_fly);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::ACTIVE_RUSH_FLY;

	cmd_active_rush_fly* pCmd = (cmd_active_rush_fly*)(pBuf + sizeof (cmd_header));
	pCmd->is_active	= bActive ? 1 : 0;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdQueryDoubleExp()
{
	_SendNakeCommand(C2S::QUERY_DOUBLE_EXP);
}

void c2s_SendCmdDuelRequest(int idTarget)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_duel_request);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::DUEL_REQUEST;

	cmd_duel_request* pCmd = (cmd_duel_request*)(pBuf + sizeof (cmd_header));
	pCmd->target = idTarget;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdDuelReply(int idWho, int iReply)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_duel_reply);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::DUEL_REPLY;

	cmd_duel_reply* pCmd = (cmd_duel_reply*)(pBuf + sizeof (cmd_header));
	pCmd->who = idWho;
	pCmd->param = iReply;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdBindPlayerRequest(int idTarget)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_bind_player_request);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::BIND_PLAYER_REQUEST;

	cmd_bind_player_request* pCmd = (cmd_bind_player_request*)(pBuf + sizeof (cmd_header));
	pCmd->target = idTarget;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdBindPlayerInvite(int idTarget)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_bind_player_invite);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::BIND_PLAYER_INVITE;

	cmd_bind_player_invite* pCmd = (cmd_bind_player_invite*)(pBuf + sizeof (cmd_header));
	pCmd->target = idTarget;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdBindPlayerRequestRe(int idWho, int iReply)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_bind_player_request_reply);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::BIND_PLAYER_REQUEST_REPLY;

	cmd_bind_player_request_reply* pCmd = (cmd_bind_player_request_reply*)(pBuf + sizeof (cmd_header));
	pCmd->who = idWho;
	pCmd->param = iReply;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdBindPlayerInviteRe(int idWho, int iReply)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_bind_player_invite_reply);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::BIND_PLAYER_INVITE_REPLY;

	cmd_bind_player_invite_reply* pCmd = (cmd_bind_player_invite_reply*)(pBuf + sizeof (cmd_header));
	pCmd->who = idWho;
	pCmd->param = iReply;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdCancelBindPlayer()
{
	_SendNakeCommand(C2S::CANCEL_BIND_PLAYER);
}

// 多人骑乘
void c2s_SendCmdMultiBindPlayerInvite(int idTarget)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_bind_player_invite);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::MULTI_BIND_INVITE;

	cmd_bind_player_invite* pCmd = (cmd_bind_player_invite*)(pBuf + sizeof (cmd_header));
	pCmd->target = idTarget;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdMultiBindPlayerInviteRe(int idWho, int iReply)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_bind_player_invite_reply);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::MULTI_BIND_INVITE_REPLY;

	cmd_bind_player_invite_reply* pCmd = (cmd_bind_player_invite_reply*)(pBuf + sizeof (cmd_header));
	pCmd->who = idWho;
	pCmd->param = iReply;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdCancelMultiBindPlayer()
{
	_SendNakeCommand(C2S::MULTI_BIND_CANCEL);
}

void c2s_SendCmdMultiBindKick(int iPos)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_multi_bind_kick);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::MULTI_BIND_KICK;

	cmd_multi_bind_kick* pCmd = (cmd_multi_bind_kick*)(pBuf + sizeof (cmd_header));
	pCmd->pos = iPos;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdInteractWithObj(int idTarget, int idTask, int idTool )
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_interact);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::INTERACT_WITH_OBJ;

	cmd_interact* pCmd = (cmd_interact*)(pBuf + sizeof (cmd_header));
	pCmd->npc_id = idTarget;
	pCmd->task_id = idTask;
	pCmd->tool_type = idTool;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdCancelInteract(int idTarget)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_cancel_interact);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::CANCEL_INTERACT;

	cmd_cancel_interact* pCmd = (cmd_cancel_interact*)(pBuf + sizeof (cmd_header));
	pCmd->npc_id = idTarget;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdMallShopping(int iCount, const void* pGoods)
{
	using namespace C2S;

	int iSizeGoods = iCount * sizeof(mall_shopping::goods);
	int iSize = sizeof (cmd_header) + sizeof (int) + iSizeGoods;
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::MALL_SHOPPING;

	mall_shopping* pCmd = (mall_shopping*)(pBuf + sizeof (cmd_header));
	pCmd->count	= iCount;
	memcpy(pCmd->list, pGoods, iSizeGoods);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdMallShopping2(unsigned short id, unsigned short index, unsigned short slot, unsigned short count)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (mall_shopping_2);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::MALL_SHOPPING2;

	mall_shopping_2* pCmd = (mall_shopping_2*)(pBuf + sizeof (cmd_header));
	pCmd->goods_id		= id;
	pCmd->goods_index	= index;
	pCmd->goods_slot	= slot;
	pCmd->count			= count;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdSelectTitle(short title_id)
{
	using namespace C2S;

	const int iSize = sizeof (cmd_header) + sizeof (select_title);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SELECT_TITLE;

	select_title* pCmd = (select_title*)(pBuf + sizeof (cmd_header));
	pCmd->title_id = title_id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdLotteryCashing(int item_index)
{
	using namespace C2S;

	const int iSize = sizeof (cmd_header) + sizeof (lottery_cashing);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::LOTTERY_CASHING;

	lottery_cashing* pCmd = (lottery_cashing*)(pBuf + sizeof (cmd_header));
	pCmd->item_index = item_index;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdCheckRecord(int item_index)
{
	using namespace C2S;

	const int iSize = sizeof (cmd_header) + sizeof (check_record);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::CHECK_RECORD;

	check_record* pCmd = (check_record*)(pBuf + sizeof (cmd_header));
	pCmd->item_index = item_index;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdUseItemWithArg(unsigned char where, unsigned char count, unsigned short index, int item_id, const void* arg, size_t size)
{
	using namespace C2S;

	const int iSize = sizeof (cmd_header) + sizeof (use_item_with_arg) + size;
	BYTE* pBuf = new BYTE[iSize];

	((cmd_header*)pBuf)->cmd = C2S::USE_ITEM_WITH_ARG;

	use_item_with_arg* pCmd = (use_item_with_arg*)(pBuf + sizeof (cmd_header));
	pCmd->where = where;
	pCmd->count = count;
	pCmd->index = index;
	pCmd->item_id = item_id;
	memcpy(pCmd + 1, arg, size);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	delete[] pBuf;
}

void c2s_SendCmdCancelIceCrust()
{
	_SendNakeCommand(C2S::CANCEL_ICE_CRUST);
}

void c2s_SendCmdDebugDeliverCmd(WORD type, const char* szCmd)
{
	using namespace C2S;

	if (!szCmd || !szCmd[0])
		return;

	int iCmdLen = strlen(szCmd);

	int iSize = sizeof (cmd_header) + sizeof (cmd_debug_deliver_cmd) + iCmdLen;
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::DEBUG_DELIVER_CMD;

	cmd_debug_deliver_cmd* pCmd = (cmd_debug_deliver_cmd*)(pBuf + sizeof (cmd_header));
	pCmd->type = type;

	memcpy(pCmd+1, szCmd, iCmdLen);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdDebugGSCmd(const char* szCmd)
{
	using namespace C2S;

	if (!szCmd || !szCmd[0])
		return;

	int iCmdLen = strlen(szCmd);

	int iSize = sizeof (cmd_header) + iCmdLen;
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::DEBUG_GS_CMD;

	BYTE* pDst = pBuf + sizeof (cmd_header);
	memcpy(pDst, szCmd, iCmdLen);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdTeamInvite(int idPlayer)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_team_invite);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::TEAM_INVITE;

	cmd_team_invite* pCmd = (cmd_team_invite*)(pBuf + sizeof (cmd_header));
	pCmd->idPlayer	= idPlayer;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdTeamAgreeInvite(int idLeader, int iTeamSeq)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_team_agree_invite);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::TEAM_AGREE_INVITE;

	cmd_team_agree_invite* pCmd = (cmd_team_agree_invite*)(pBuf + sizeof (cmd_header));
	pCmd->idLeader	= idLeader;
	pCmd->team_seq	= iTeamSeq;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdTeamRejectInvite(int idLeader)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_team_reject_invite);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::TEAM_REJECT_INVITE;

	cmd_team_reject_invite* pCmd = (cmd_team_reject_invite*)(pBuf + sizeof (cmd_header));
	pCmd->idLeader = idLeader;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdTeamLeaveParty()
{
	_SendNakeCommand(C2S::TEAM_LEAVE_PARTY);
}

void c2s_SendCmdTeamKickMember(int idMember)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_team_kick_member);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::TEAM_KICK_MEMBER;

	cmd_team_kick_member* pCmd = (cmd_team_kick_member*)(pBuf + sizeof (cmd_header));
	pCmd->idMember = idMember;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdTeamSetPickupFlag(short sFlag)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_team_set_pickup);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::TEAM_SET_PICKUP;

	cmd_team_set_pickup* pCmd = (cmd_team_set_pickup*)(pBuf + sizeof (cmd_header));
	pCmd->pickup_flag = sFlag;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdTeamMemberPos(int iNumMem, int* aMemIDs)
{
	if (!iNumMem || !aMemIDs)
		return;

	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (unsigned short) + iNumMem * sizeof (int);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::TEAM_MEMBER_POS;

	cmd_team_member_pos* pCmd = (cmd_team_member_pos*)(pBuf + sizeof (cmd_header));
	pCmd->wMemCnt = (unsigned short)iNumMem;

	memcpy(pCmd->aMemIDs, aMemIDs, iNumMem * sizeof (int));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdTeamAssistSel(int idTeamMember)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_assist_sel);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::ASSIST_SELECT;

	cmd_assist_sel* pCmd = (cmd_assist_sel*)(pBuf + sizeof (cmd_header));
	pCmd->idTeamMember = idTeamMember;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdTeamAskJoin(int idTarget)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_team_ask_join);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::TEAM_ASK_TO_JOIN;

	cmd_team_ask_join* pCmd = (cmd_team_ask_join*)(pBuf + sizeof (cmd_header));
	pCmd->idTarget = idTarget;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdTeamReplyJoinAsk(int idAsker, bool bAgree)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_team_reply_join_ask);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::TEAM_REPLY_JOIN_ASK;

	cmd_team_reply_join_ask* pCmd = (cmd_team_reply_join_ask*)(pBuf + sizeof (cmd_header));
	pCmd->idAsker	= idAsker;
	pCmd->result	= bAgree ? 1 : 0;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdTeamChangeLeader(int idLeader)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_change_team_leader);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::CHANGE_TEAM_LEADER;

	cmd_change_team_leader* pCmd = (cmd_change_team_leader*)(pBuf + sizeof (cmd_header));
	pCmd->idLeader	= idLeader;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdTeamNotifyOwnPos()
{
	_SendNakeCommand(C2S::NOTIFY_POS_IN_TEAM);
}

void c2s_SendCmdMoveIvtrItem(int iSrc, int iDest, int iAmount)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_move_ivtr_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::MOVE_IVTR_ITEM;

	cmd_move_ivtr_item* pCmd = (cmd_move_ivtr_item*)(pBuf + sizeof (cmd_header));
	pCmd->src	 = iSrc;
	pCmd->dest	 = iDest;
	pCmd->amount = iAmount;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdExgIvtrItem(int iIndex1, int iIndex2)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_exg_ivtr_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::EXG_IVTR_ITEM;

	cmd_exg_ivtr_item* pCmd = (cmd_exg_ivtr_item*)(pBuf + sizeof (cmd_header));
	pCmd->index1 = iIndex1;
	pCmd->index2 = iIndex2;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdDropIvtrItem(int iIndex, int iAmount)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_drop_ivtr_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::DROP_IVTR_ITEM;

	cmd_drop_ivtr_item* pCmd = (cmd_drop_ivtr_item*)(pBuf + sizeof (cmd_header));
	pCmd->index	 = iIndex;
	pCmd->amount = iAmount;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdDropEquipItem(int iIndex)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_drop_equip_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::DROP_EQUIP_ITEM;

	cmd_drop_equip_item* pCmd = (cmd_drop_equip_item*)(pBuf + sizeof (cmd_header));
	pCmd->index = iIndex;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdExgEquipItem(int iIndex1, int iIndex2)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_exg_equip_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::EXG_EQUIP_ITEM;

	cmd_exg_equip_item* pCmd = (cmd_exg_equip_item*)(pBuf + sizeof (cmd_header));
	pCmd->idx1 = iIndex1;
	pCmd->idx2 = iIndex2;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdEquipItem(int iIvtrIdx, int iEquipIdx)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_equip_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::EQUIP_ITEM;

	cmd_equip_item* pCmd = (cmd_equip_item*)(pBuf + sizeof (cmd_header));
	pCmd->idx_inv	= iIvtrIdx;
	pCmd->idx_eq	= iEquipIdx;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdMoveItemToEquip(int iIvtrIdx, int iEquipIdx)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_move_item_to_equip);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::MOVE_ITEM_TO_EQUIP;

	cmd_move_item_to_equip* pCmd = (cmd_move_item_to_equip*)(pBuf + sizeof (cmd_header));
	pCmd->idx_inv	= iIvtrIdx;
	pCmd->idx_eq	= iEquipIdx;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdGetOtherEquip(int iNumID, int* aIDs)
{
	using namespace C2S;

	static const int iNumLimit = 250;
	int iCount = 0;

	while (iCount < iNumID)
	{
		int iNumSend = iNumLimit;
		if (iCount + iNumLimit > iNumID)
			iNumSend = iNumID - iCount;

		int iSize = sizeof (cmd_header) + sizeof (WORD) + iNumSend * sizeof (int);
		BYTE* pBuf = new BYTE [iSize];
		if (!pBuf)
			return;
		
		((cmd_header*)pBuf)->cmd = C2S::GET_OTHER_EQUIP;

		cmd_get_other_equip* pCmd = (cmd_get_other_equip*)(pBuf + sizeof (cmd_header));
		pCmd->size = (WORD)iNumSend;

		for (int i=0; i < iNumSend; i++)
			pCmd->idlist[i] = aIDs[iCount+i];

		g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

		delete [] pBuf;

		iCount += iNumSend;
	}
}

void c2s_SendCmdGetWealth(bool bpd, bool bed, bool btd)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_get_own_wealth);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GET_OWN_WEALTH;

	cmd_get_own_wealth* pCmd = (cmd_get_own_wealth*)(pBuf + sizeof (cmd_header));
	pCmd->byPack	= (BYTE)bpd;
	pCmd->byEquip	= (BYTE)bed;
	pCmd->byTask	= (BYTE)btd;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdPetSummon(int iPetIdx)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (summon_pet);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SUMMON_PET;

	summon_pet* pCmd = (summon_pet*)(pBuf + sizeof (cmd_header));
	pCmd->pet_index	= (unsigned char)iPetIdx;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdPetRecall(int iPetIdx)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (recall_pet);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::RECALL_PET;

	recall_pet* pCmd = (recall_pet*)(pBuf + sizeof (cmd_header));
	pCmd->pet_index	= (unsigned char)iPetIdx;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdPetBanish(int iPetIdx)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_banish_pet);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::BANISH_PET;

	cmd_banish_pet* pCmd = (cmd_banish_pet*)(pBuf + sizeof (cmd_header));
	pCmd->pet_index	= iPetIdx;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdProduceItem(int iRecipeId)
{
	using namespace C2S;

	const int iSize = sizeof(cmd_header) + sizeof(produce_item);
	BYTE buf[iSize];

	((cmd_header*)buf)->cmd = C2S::PRODUCE_ITEM;

	produce_item* pCmd = (produce_item*)(buf + sizeof(cmd_header));
	pCmd->recipe_id = iRecipeId;

	g_pGame->GetGameSession()->SendGameData(buf, iSize);
}

void c2s_SendCmdFuWenCompose(bool consume_extra_item, int extra_item_index)
{
	using namespace C2S;

	const int iSize = sizeof(cmd_header) + sizeof(fuwen_compose);
	BYTE buf[iSize];

	((cmd_header*)buf)->cmd = C2S::FUWEN_COMPOSE;

	fuwen_compose* pCmd = (fuwen_compose*)(buf + sizeof(cmd_header));
	pCmd->consume_extra_item = consume_extra_item;
	pCmd->extra_item_index = extra_item_index;

	g_pGame->GetGameSession()->SendGameData(buf, iSize);
}

void c2s_SendCmdFuWenUpgrade(int main_fuwen_index, int main_fuwen_where, int assist_count, int* assist_index)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(fuwen_upgrade) - sizeof (int) +
					  assist_count * sizeof(int);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::FUWEN_UPGRADE;

	fuwen_upgrade* pCmd = (fuwen_upgrade*)(pBuf + sizeof(cmd_header));
	pCmd->main_fuwen_index = main_fuwen_index;
	pCmd->main_fuwen_where = main_fuwen_where;
	pCmd->assist_count = assist_count;
	if (assist_count)
		memcpy(pCmd->assist_fuwen_index, assist_index, assist_count * sizeof(int));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdFuWenInstall(int src_index, int dst_index)
{
	using namespace C2S;

	const int iSize = sizeof(cmd_header) + sizeof(fuwen_install);
	BYTE buf[iSize];

	((cmd_header*)buf)->cmd = C2S::FUWEN_INSTALL;

	fuwen_install* pCmd = (fuwen_install*)(buf + sizeof(cmd_header));
	pCmd->src_index = src_index;
	pCmd->dst_index = dst_index;

	g_pGame->GetGameSession()->SendGameData(buf, iSize);
}

void c2s_SendCmdFuWenUninstall(int fuwen_index, int assist_index, int inv_index)
{
	using namespace C2S;

	const int iSize = sizeof(cmd_header) + sizeof(fuwen_uninstall);
	BYTE buf[iSize];

	((cmd_header*)buf)->cmd = C2S::FUWEN_UNINSTALL;

	fuwen_uninstall* pCmd = (fuwen_uninstall*)(buf + sizeof(cmd_header));
	pCmd->fuwen_index = fuwen_index;
	pCmd->assist_index = assist_index;
	pCmd->inv_index = inv_index;

	g_pGame->GetGameSession()->SendGameData(buf, iSize);
}

void c2s_SendCmdNPCSevHello(int nid)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_hello);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_HELLO;

	cmd_sevnpc_hello* pCmd = (cmd_sevnpc_hello*)(pBuf + sizeof (cmd_header));
	pCmd->id = nid;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevBuy(int iItemNum, C2S::npc_trade_item* aItems)
{
	if (!iItemNum || !aItems)
		return;

	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
         size_t money;
         size_t item_count;
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT) + 
				iItemNum * sizeof (npc_trade_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	BYTE* pTemp = pBuf + sizeof (cmd_header);
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)pTemp;
	pCmd->service_type = GP_NPCSEV_BUY;
	pCmd->len = sizeof (CONTENT) + iItemNum * sizeof (npc_trade_item);

	pTemp += sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pTemp;
	pContent->money	= 0;	//	Not use now
	pContent->item_count = iItemNum;

	//	Copy item data ...
	pTemp += sizeof (CONTENT);
	memcpy(pTemp, aItems, iItemNum * sizeof (npc_trade_item));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevSellBuyBack(char opType, int iItemNum, C2S::npc_trade_item* aItems)
{
	if (!iItemNum || !aItems)
		return;

	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		char   op_type;		// 0: sell item,  1:buy back
        size_t item_count;
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT) +
				iItemNum * sizeof (npc_trade_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	BYTE* pTemp = pBuf + sizeof (cmd_header);
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)pTemp;
	pCmd->service_type = GP_NPCSEV_SELL;
	pCmd->len = sizeof (CONTENT) + iItemNum * sizeof (npc_trade_item);

	pTemp += sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pTemp;
	pContent->op_type = opType;
	pContent->item_count = iItemNum;

	//	Copy item data ...
	pTemp += sizeof (CONTENT);
	memcpy(pTemp, aItems, iItemNum * sizeof (npc_trade_item));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevReputationBuy(int iItemNum, C2S::npc_trade_item* aItems)
{
	if (!iItemNum || !aItems)
		return;

	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
         size_t item_count;
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT) + 
				iItemNum * sizeof (npc_trade_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	BYTE* pTemp = pBuf + sizeof (cmd_header);
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)pTemp;
	pCmd->service_type = GP_NPCSEV_SHOP_REPUTATION;
	pCmd->len = sizeof (CONTENT) + iItemNum * sizeof (npc_trade_item);

	pTemp += sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pTemp;
	pContent->item_count = iItemNum;

	pTemp += sizeof (CONTENT);
	memcpy(pTemp, aItems, iItemNum * sizeof (npc_trade_item));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevRepair(BYTE byPackage, BYTE bySlot, int idItem)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int tid;
		BYTE byPackage;
		BYTE bySlot;
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_REPAIR;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->tid		= idItem;
	pContent->byPackage = byPackage;
	pContent->bySlot	= bySlot;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevRepairAll()
{
	c2s_SendCmdNPCSevRepair(0, 0, -1);
}

void c2s_SendCmdNPCSevHeal()
{
	_SendEmptyDataNPCSev(GP_NPCSEV_HEAL);
}

void c2s_SendCmdNPCSevTransmit(int iIndex)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int index;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TRANSMIT;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->index	= iIndex;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevEmbed(int item_index, int item_id, int stone_index, int rt_index)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
        int item_index;         //要镶嵌的装备在包裹栏中的位置索引
        int item_id;            //要镶嵌的装备的ID用于校验
        int stone_index;        //要嵌入的宝石在包裹栏中的位置索引
        int rt_index;           //镶嵌概率修改道具在包裹栏中的位置索引 -1 为无此物品
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_EMBED;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->item_index	= item_index;
	pContent->item_id		= item_id;
	pContent->stone_index	= stone_index;
	pContent->rt_index		= rt_index;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevClearEmbeddedChip(int iEquipIdx, int tidEquip, char uninstall_pstone)
{
	using namespace C2S;

#pragma pack(1)

	struct CONTENT
	{
         int iEquipIdx;
         int tidEquip;
		 char uninstall_pstone;
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_CLEAR_TESSERA;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->iEquipIdx	= iEquipIdx;
	pContent->tidEquip	= tidEquip;
	pContent->uninstall_pstone = uninstall_pstone;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevLearnSkill(int idSkill)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
         int idSkill;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_LEARN;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->idSkill = idSkill;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevMakeItem(int id)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
         int id;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_MAKEITEM;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->id = id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevResetPKValue(int value)
{
	using namespace C2S;

#pragma pack(1)

	struct CONTENT
	{
		int value;
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_RESET_PK_VALUE;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->value = value;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevAcceptTask(int idTask, int idTaskSet)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
         int idTask;
		 int idTaskSet;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TASK_ACCEPT;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->idTask = idTask;
	pContent->idTaskSet = idTaskSet;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevReturnTask(int idTask, int iChoice)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
         int idTask;
		 int iChoice;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TASK_RETURN;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->idTask = idTask;
	pContent->iChoice = iChoice;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevTaskMatter(int idTask)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
         int idTask;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TASK_MATTER;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->idTask = idTask;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevChgTrashPsw(const char* szOldPsw, const char* szNewPsw)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		unsigned short origin_size;
		unsigned short new_size;
	};   

#pragma pack()

	int iOldLen = 0;
	if (szOldPsw && szOldPsw[0])
		iOldLen = strlen(szOldPsw);

	int iNewLen = 0;
	if (szNewPsw && szNewPsw[0])
		iNewLen = strlen(szNewPsw);

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT) +
				iOldLen + iNewLen;
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	BYTE* pTemp = pBuf + sizeof (cmd_header);
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)pTemp;
	pCmd->service_type = GP_NPCSEV_TRASHPSW;
	pCmd->len = sizeof (CONTENT) + iOldLen + iNewLen;
	pTemp += sizeof (cmd_sevnpc_serve);

	CONTENT* pContent = (CONTENT*)pTemp;
	pContent->origin_size	= (WORD)iOldLen;
	pContent->new_size		= (WORD)iNewLen;
	pTemp += sizeof (CONTENT);

	if (iOldLen)
	{
		memcpy(pTemp, szOldPsw, iOldLen);
		pTemp += iOldLen;
	}

	if (iNewLen)
	{
		memcpy(pTemp, szNewPsw, iNewLen);
		pTemp += iNewLen;
	}

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevOpenTrash(const char* szPsw, bool bFaction)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		unsigned int psw_size;
	};   

#pragma pack()

	int iLen = 0;
	if (szPsw && szPsw[0])
		iLen = strlen(szPsw);

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT) + iLen;
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	BYTE* pTemp = pBuf + sizeof (cmd_header);
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)pTemp;
	pCmd->service_type = bFaction ? GP_NPCSEV_OPENFACTIONTRASH : GP_NPCSEV_OPENTRASH;
	pCmd->len = sizeof (CONTENT) + iLen;
	pTemp += sizeof (cmd_sevnpc_serve);

	CONTENT* pContent = (CONTENT*)pTemp;
	pContent->psw_size = iLen;
	pTemp += sizeof (CONTENT);

	if (iLen)
	{
		memcpy(pTemp, szPsw, iLen);
		pTemp += iLen;
	}

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevIdentify(int iSlot, int tidItem)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int iSlot;
		int tidItem;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_IDENTIFY;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->iSlot	= iSlot;
	pContent->tidItem = tidItem;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevFaction(int iAction, void* pDataBuf, int iDataSize)
{
	_SendDataBufNPCSev(GP_NPCSEV_FACTION, iAction, pDataBuf, iDataSize);
}

void c2s_SendCmdNPCSevWarChallenge(int iAction, void* pDataBuf, int iDataSize)
{
	_SendDataBufNPCSev(GP_NPCSEV_WAR_CHALLENGE, iAction, pDataBuf, iDataSize);
}

void c2s_SendCmdNPCSevTravel(int iIndex, int idLine)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
        int iIndex;		//	Index in travel service
        int idLine;		//	Line's ID
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TRAVEL;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->iIndex = iIndex;
	pContent->idLine = idLine;	

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevGetContent(int idSevice)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_get_content);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_GET_CONTENT;

	cmd_sevnpc_get_content* pCmd = (cmd_sevnpc_get_content*)(pBuf + sizeof (cmd_header));
	pCmd->service_id = idSevice;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevBoothBuy(int idBooth, int iItemNum, C2S::npc_booth_item* aItems)
{
	if (!iItemNum || !aItems)
		return;

	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int booth_id;
        size_t money;
        size_t item_count;
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT) + 
				iItemNum * sizeof (npc_booth_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	BYTE* pTemp = pBuf + sizeof (cmd_header);
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)pTemp;
	pCmd->service_type = GP_NPCSEV_BOOTHSELL;
	pCmd->len = sizeof (CONTENT) + iItemNum * sizeof (npc_booth_item);

	pTemp += sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pTemp;
	pContent->booth_id = idBooth;
	pContent->money	= 0;	//	Not use now
	pContent->item_count = iItemNum;

	//	Copy item data ...
	pTemp += sizeof (CONTENT);
	memcpy(pTemp, aItems, iItemNum * sizeof (npc_booth_item));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevBoothSell(int idBooth, int iItemNum, C2S::npc_booth_item* aItems)
{
	if (!iItemNum || !aItems)
		return;

	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int booth_id;
        size_t money;
        size_t item_count;
	};

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT) + 
				iItemNum * sizeof (npc_booth_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	BYTE* pTemp = pBuf + sizeof (cmd_header);
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)pTemp;
	pCmd->service_type = GP_NPCSEV_BOOTHBUY;
	pCmd->len = sizeof (CONTENT) + iItemNum * sizeof (npc_booth_item);

	pTemp += sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pTemp;
	pContent->booth_id = idBooth;
	pContent->money	= 0;	//	Not use now
	pContent->item_count = iItemNum;

	//	Copy item data ...
	pTemp += sizeof (CONTENT);
	memcpy(pTemp, aItems, iItemNum * sizeof (npc_booth_item));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevWaypoint()
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_WAYPOINT;
	pCmd->len = 0;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevForgetSkill(int idSkill)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
        int idSkill;	//	Index in travel service
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_FORGETSKILL;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->idSkill = idSkill;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevFaceChange(int iIvtrIdx, int idItem)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
        int iIvtrIdx;	//	Index in pack
		int idItem;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_FACECHANGE;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->iIvtrIdx	= iIvtrIdx;
	pContent->idItem	= idItem;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevMail(int iAction, void* pDataBuf, int iDataSize)
{
	_SendDataBufNPCSev(GP_NPCSEV_MAIL, iAction, pDataBuf, iDataSize);
}

void c2s_SendCmdNPCSevVendue(int iAction, void* pDataBuf, int iDataSize)
{
	_SendDataBufNPCSev(GP_NPCSEV_VENDUE, iAction, pDataBuf, iDataSize);
}

void c2s_SendCmdNPCSevDblExpTime(int iIndex)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int iIndex;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_DBLEXPTIME;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->iIndex = iIndex;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevHatchPet(int iIvtrIdx, int idEgg)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int iIvtrIdx;
		int idEgg;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_HATCHPET;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->iIvtrIdx = iIvtrIdx;
	pContent->idEgg = idEgg;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevRestorePet(int iPetIdx)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int iPetIdx;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_RESTOREPET;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->iPetIdx = iPetIdx;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevBattle(int iAction, void* pDataBuf, int iDataSize)
{
	_SendDataBufNPCSev(GP_NPCSEV_BATTLE, iAction, pDataBuf, iDataSize);
}

void c2s_SendCmdNPCSevBuildTower(int iTowerIdx, int idItem)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int iTowerIdx;
		int idItem;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_BUILDTOWER;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->iTowerIdx = iTowerIdx;
	pContent->idItem = idItem;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevLeaveBattle()
{
	_SendEmptyDataNPCSev(GP_NPCSEV_LEAVEBATTLE);
}

void c2s_SendCmdNPCSevReturnStatusPt(int iIndex, int idItem)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int iIndex;
		int idItem;
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_RETURNSTATUSPT;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->iIndex = iIndex;
	pContent->idItem = idItem;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdNPCSevAccountPoint(int iAction, void* pDataBuf, int iDataSize)
{
	_SendDataBufNPCSev(GP_NPCSEV_ACCOUNTPOINT, iAction, pDataBuf, iDataSize);
}

void c2s_SendCmdNPCSevBindItem(int iIndex, int idItem)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int iIndex;
		int idItem;
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_BINDITEM;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->iIndex = iIndex;
	pContent->idItem = idItem;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevDestroyBindItem(int iIndex, int idItem)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int iIndex;
		int idItem;
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_DESTROYBINDITEM;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->iIndex = iIndex;
	pContent->idItem = idItem;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevCancelDestroy(int iIndex, int idItem)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int iIndex;
		int idItem;
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_CANCELDESTROY;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->iIndex = iIndex;
	pContent->idItem = idItem;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

}

void c2s_SendCmdNPCSevRepairDamagedItem(int iIndex, int idItem)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int iIndex;
		int idItem;
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_REPAIR_DAMAGED_ITEM;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->iIndex = iIndex;
	pContent->idItem = idItem;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevStockTransaction(int nWithdraw, int nCash, int nMoney)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int iWithdraw;
		int iCash;
		int iMoney;
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_STOCK_TRANSACTION;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->iWithdraw = nWithdraw;
	pContent->iCash = nCash;
	pContent->iMoney = nMoney;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevStockOperation(int nType, void* pData, int nLen)
{
	_SendDataBufNPCSev(GP_NPCSEV_STOCK_OPERATION, nType, pData, nLen);
}

void c2s_SendCmdNPCSevTalismanRefine(int item_index, int item_id, int material_id)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
        int item_index;                 //物品 包裹栏索引
        int item_id;                    //法宝 id
        int material_id;                //血炼材料的ID
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TALISMAN_REFINE;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index	= item_index;
	pContent->item_id		= item_id;
	pContent->material_id	= material_id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevTalismanUpgrade(int item_index, int item_id)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int item_index;                 //物品 包裹栏索引
		int item_id;                    //法宝 id
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TALISMAN_UPGRADE;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index	= item_index;
	pContent->item_id		= item_id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevTalismanRemoveOwner(int item_index, int item_id)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int item_index;                 //物品 包裹栏索引
		int item_id;                    //法宝 id
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TALISMAN_REMOVE_OWNER;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index	= item_index;
	pContent->item_id		= item_id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevTalismanCombine(int item_index1, int item_id1, int item_index2, int item_id2, int catalyst_id)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int item_index1;                //法宝1 包裹栏索引
        int item_id1;                   //法宝1 id
        int item_index2;                //法宝2 包裹栏索引
        int item_id2;                   //法宝2 id
		int catalyst_id;                //法宝融合调整物品ID
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TALISMAN_COMBINE;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index1	= item_index1;
	pContent->item_id1		= item_id1;
	pContent->item_index2	= item_index2;
	pContent->item_id2		= item_id2;
	pContent->catalyst_id	= catalyst_id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevTalismanEnchant(int item_index1, int item_type1, int item_index2, int item_type2)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int item_index1;                // 物品 包裹栏索引 法宝
        int item_type1;                 // id
        int item_index2;                // 物品 包裹栏索引 法宝熔炼物品
        int item_type2;                 // id
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TALISMAN_ENCHANT;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index1	= item_index1;
	pContent->item_type1	= item_type1;
	pContent->item_index2	= item_index2;
	pContent->item_type2	= item_type2;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevTalismanHoleyLevelUp(int item_index, int item_id, int material_id)
{
	using namespace C2S;
	
#pragma pack(1)
	
	struct CONTENT
	{
		int item_index;					// 物品 包裹栏索引 法宝
        int item_id;					// id
        int material_id;                // 法宝飞升道具
	};   
	
#pragma pack()
	
	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;
	
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TALISMAN_HOLEYLEVELU;
	pCmd->len = sizeof (CONTENT);
	
	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index	= item_index;
	pContent->item_id		= item_id;
	pContent->material_id	= material_id;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevTalismanSkillRefine(int item_index1, int item_id1, int item_index2, 
										  int item_id2, int material_id)
{
	using namespace C2S;
	
#pragma pack(1)
	
	struct CONTENT
	{
		int item_index1;              // 包裹栏索引 法宝
        int item_id1;                 // id
        int item_index2;              // 包裹栏索引 法宝
        int item_id2;                 // id
		int material_id;			  // 融合道具
	};   
	
#pragma pack()
	
	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;
	
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TALISMAN_SKILLREFINE;
	pCmd->len = sizeof (CONTENT);
	
	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index1	= item_index1;
	pContent->item_id1		= item_id1;
	pContent->item_index2	= item_index2;
	pContent->item_id2		= item_id2;
	pContent->material_id	= material_id;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevTalismanSkillEmbed(int item_index1, int item_id1, int item_index2, 
			int item_id2, int material_index1, int material_id1, int material_index2, int material_id2)
{
	using namespace C2S;
	
#pragma pack(1)
	
	struct CONTENT
	{
		int item_index1;            // 包裹栏索引 法宝
        int item_id1;				// id
        int item_index2;            // 包裹栏索引 法宝
        int item_id2;               // id
        int material_id1;           // 副法宝 附件id
		int material_index1;		// 副法宝 附件index
        int material_id2;           // 副法宝 秘籍id
		int material_index2;		// 副法宝 秘籍index
	};   
	
#pragma pack()
	
	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;
	
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TALISMAN_EMBEDSKILL;
	pCmd->len = sizeof (CONTENT);
	
	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index1	= item_index1;
	pContent->item_id1		= item_id1;
	pContent->item_index2	= item_index2;
	pContent->item_id2		= item_id2;
	pContent->material_id1	= material_id1;
	pContent->material_index1	= material_index1;
	pContent->material_id2	= material_id2;
	pContent->material_index2	= material_index2;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

//装备血祭
void c2s_SendCmdNPCSevBleedSacrifice(int iIndex, int idItem)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int item_index;					// 物品 包裹栏索引 装备
        int item_id;					// id
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_BLEED_SACRIFICE;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index	= iIndex;
	pContent->item_id		= idItem;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}  
    
//镶嵌器魄	                               
void c2s_SendCmdNPCSevEmbedSoul(int item_index, int item_id, int soul_index, int soul_slot)	
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int item_index;					// 物品 包裹栏索引  装备
        int item_id;					// id
        int soul_index;					// 物品 包裹栏索引	器魄
        int soul_slot;					// 器魄要镶嵌的孔位
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_EMBED_SOUL;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index	= item_index;
	pContent->item_id		= item_id;
	pContent->soul_index	= soul_index;
	pContent->soul_slot		= soul_slot;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

//移除器魄
void c2s_SendCmdNPCSevClearSoulTessera(int item_index, int item_id, int soul_slot)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int item_index;				    // 物品 包裹栏索引 装备
        int item_id;			        // id
        int soul_slot;	                // 物品 要拆除的器魄孔位
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_CLEAR_SOUL_TESSERA;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index	= item_index;
	pContent->item_id		= item_id;
	pContent->soul_slot		= soul_slot;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

//恢复魂力			
void c2s_SendCmdNPCSevRefineSoul(int item_index, int item_id)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int item_index;					// 物品 包裹栏索引 装备
        int item_id;					// id
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_REFINE_SOUL;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index	= item_index;
	pContent->item_id		= item_id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

//装备分解
void c2s_SendCmdNPCSevBreakDownEquip(int item_index, int item_id)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int item_index;					// 物品 包裹栏索引 装备
        int item_id;					// id
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_BREAKDOWN_EQUIP;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index	= item_index;
	pContent->item_id		= item_id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

}

//器魄融合
void c2s_SendCmdNPCSevMeldSoul(int recipe_id, int territory_id)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int recipe_id;					// 配方id
		int territory_id;				// 领土id，这边加个这个id实在是太土了，没办法,后续的领土战的奖励有这个需求
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_MELD_SOUL;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->recipe_id	= recipe_id;
	pContent->territory_id = territory_id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

}

//宠物装备强化
void c2s_SendCmdNPCSevPetEquipEnchant(int item_index, int item_id, int stone_index)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int item_index;				    // 物品 包裹栏索引 宠物装备
        int item_id;			        // id
        int stone_index;	            // 物品 包裹栏索引 强化石
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_PET_EQUIP_ENCHANT;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index	= item_index;
	pContent->item_id		= item_id;
	pContent->stone_index	= stone_index;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

}

//形象改变，美容功能
void c2s_SendCmdNPCSevChangeFace(int faceid, int hairid, int earid, int tailid, int fashionid, int stone_index)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		unsigned char faceid;				// 新的脸型的序号
        unsigned char hairid;				// 新的发型的序号
		unsigned char earid;				// 新的耳朵的序号，只是适用于烈山
		unsigned char tailid;				// 新的尾巴的序号，只是适用于烈山
		unsigned char fashionid;			// 更换默认皮肤，fuchonggang 2011/8/04
        int stone_index;		// 物品 包裹栏索引 硫酸
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_CHANGE_FACE;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->faceid	= faceid;
	pContent->hairid	= hairid;
	pContent->earid		= earid;
	pContent->tailid	= tailid;
	pContent->fashionid	= fashionid;
	pContent->stone_index	= stone_index;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

}

//幻化石炼化
void c2s_SendCmdNPCSevRefineChangeCard(int item_index, int item_id, int stone_index, int stone_id, int sev_type)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int item_index;				// 幻化石序号
        int item_id;				// 幻化石id
        int stone_index;			// 强化石索引
		int stone_id;
		int sev_type;				// 炼化类型： 归元1，强化2， 契合3， 轮回4
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_REFINE_SHAPE_CARD;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index	= item_index;
	pContent->item_id		= item_id;
	pContent->stone_index	= stone_index;
	pContent->stone_id		= stone_id;
	pContent->sev_type		= sev_type;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

//幻化石恢复灵力
void c2s_SendCmdNPCSevRecoverCardWakan(int item_index, int item_id)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int item_index;				// 幻化石序号
        int item_id;				// 幻化石id
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_RECOVER_CARD_WAKAN;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index	= item_index;
	pContent->item_id		= item_id;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevArenaJoin(int map_id, int item_index)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int map_id;
		int item_index;
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_ARENA_JOIN;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->map_id = map_id;
	pContent->item_index = item_index;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

// 领土战宣战
void c2s_SendCmdNPCSevTerritoryChallenge(int faction_id, int territory_id, int item_id, int item_amount)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int faction_id;
        int territory_id;
		int item_id;
		int item_amount;
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TERRITORY_CHALLENGE;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->faction_id	= faction_id;
	pContent->territory_id	= territory_id;
	pContent->item_id		= item_id;
	pContent->item_amount	= item_amount;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

// 进入领土战战场
void c2s_SendCmdNPCSevTerritoryEnter(int faction_id, int territory_id, int is_assistant)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int faction_id;		// 如果是佣兵，则填写其所支持的帮派id
        int territory_id;
		char is_assistant;	// 0 不是佣兵，1是佣兵 
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TERRITORY_ENTER;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->faction_id	= faction_id;
	pContent->territory_id	= territory_id;
	pContent->is_assistant  = is_assistant;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

// 领土战奖励
void c2s_SendCmdNPCSevTerritoryAward(int faction_id, int territory_id, int reward_type, int item_id, int item_count, int money)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int faction_id;
		int territory_id;
        int reward_type;		// 1 佣兵凭证 2 竞标失败领取的宣战金 3 领土战胜利领取的宣战金 4 神器
		int item_id;
		int item_count;
		int money;				// 领取宣战金时候的余额（整数发的是1000金的银票）
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TERRITORY_AWARD;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->faction_id	= faction_id;
	pContent->territory_id  = territory_id;
	pContent->reward_type	= reward_type;
	pContent->item_id		= item_id;
	pContent->item_count	= item_count;
	pContent->money			= money;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

// 星盘充能服务
void c2s_SendCmdNPCSevTeleportationCharge(int item_index, int item_id, int stone_index, int stone_id)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int item_index;
		int item_id;
        int stone_index;
		int stone_id;
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_TELEPORTATION_CHARGE;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index	= item_index;
	pContent->item_id		= item_id;
	pContent->stone_index	= stone_index;
	pContent->stone_id		= stone_id;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

// 修复破损的物品特殊服务，适用于包裹和仓库中已经放满了破损物品的情况
void c2s_SendCmdNPCSevRepairDamagedItem2(int iIndex, int idItem)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int iIndex;
		int idItem;
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_REPAIR_DAMAGED_ITEM2;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->iIndex = iIndex;
	pContent->idItem = idItem;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

// 装备升级服务
void c2s_SendCmdNPCSevUpgradeEquip(int item_index, int item_id, int stone_index, int rt_index)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
        int item_index;         //要升级的装备在包裹栏中的位置索引
        int item_id;            //要升级的装备的ID用于校验
        int stone_index;        //装备升级模具在包裹栏中的位置索引
        int rt_index;           //装备升级概率修改道具在包裹栏中的位置索引 -1 为无此物品
	};   

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_UPGRADE_EQUIP;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->item_index= item_index;
	pContent->item_id	= item_id;
	pContent->stone_index= stone_index;
	pContent->rt_index	= rt_index;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
} 

void c2s_SendCmdNPCSevOnlineTrade(int type, int item_id, int item_index, int item_count, int money, int sold_time, int price, const ACHAR* szName)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		char	type;
        int		item_id;            //寄售物品的ID用于校验
        int		item_index;         //寄售物品在包裹栏中的位置索引
		short	item_count;			//寄售物品的数量
        int		money;				//寄售金钱数量
		int		sold_time;			//上架时间，分钟
		int		price;				//寄售价格，人民币（元）
		char	name_len;
		char	name[28];			//卖出玩家的名字
	};   

#pragma pack()

	int iNameLen = 0;
	if (szName || szName[0])
	{
		iNameLen = a_strlen(szName) * sizeof (ACHAR);
		a_ClampRoof(iNameLen, 28);
	}

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_ONLINE_TRADE;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->type		= type;
	pContent->item_id	= item_id;
	pContent->item_index= item_index;
	pContent->item_count= item_count;
	pContent->money		= money;
	pContent->sold_time	= sold_time;
	pContent->price		= price;
	pContent->name_len	= iNameLen;
	memset(pContent->name, 0, 28);
	memcpy(pContent->name, szName, iNameLen);
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevTransmitToCross()
{
	_SendEmptyDataNPCSev(GP_NPCSEV_TRANSMIT_TO_CROSS);
}

void c2s_SendCmdNPCSevTransmitToNormal()
{
	_SendEmptyDataNPCSev(GP_NPCSEV_TRANSMIT_TO_NORMAL);
}

void c2s_SendCmdNPCSevIdentifyGemSlots( int nItemSlot, int nItemTemplId )
{
	using namespace C2S;

#pragma pack(1)
	struct CONTENT
	{
		int nItemSlot;		//装备-饰品在包裹栏的位置索引
		int nItemTemplId;	//装备-饰品的模板ID，用于服务器的校验
	};
#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_IDENTIFY_GEM_SLOTS;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->nItemSlot		= nItemSlot;
	pContent->nItemTemplId	= nItemTemplId;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevRebuildGemSlots( int nItemSlot, int nItemTemplId, bool bLocked[3] )
{
	using namespace C2S;

#pragma pack(1)
	struct CONTENT
	{
		int		nItemSlot;		//装备-饰品在包裹栏的位置索引
		int		nItemTemplId;	//装备-饰品的模板ID，用于服务器的校验
		bool	bLocked[3];		//标明该插槽是否锁定，如果锁定则不进行重铸操作
	};	
#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_REBUILD_GEM_SLOTS;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->nItemSlot		= nItemSlot;
	pContent->nItemTemplId	= nItemTemplId;
	memcpy( &pContent->bLocked[0], &bLocked[0], 3*sizeof(bool) );//3 is a magic number .....

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevCustomizeGemSlots( int nItemSlot, int nItemTemplId, int nGemSealSlots[3], int nGemSealTemplId[3] )
{
	using namespace C2S;

#pragma pack(1)
	struct CONTENT
	{
		int		nItemSlot;				//装备-饰品在包裹栏的位置索引
		int		nItemTemplId;			//装备-饰品的模板ID，用于服务器的校验
		int		nGemSealTemplId[3];		//宝石魔印的模板ID，用于服务器的校验
		int		nGemSealSlots[3];		//宝石魔印在包裹栏中的位置索引
	};	
#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_CUSTOMIZE_GEM_SLOTS;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->nItemSlot		= nItemSlot;
	pContent->nItemTemplId	= nItemTemplId;
	memcpy( &pContent->nGemSealTemplId[0], &nGemSealTemplId[0], 3*sizeof(int) );//3 is a magic number .....
	memcpy( &pContent->nGemSealSlots[0], &nGemSealSlots[0], 3*sizeof(int) );//3 is a magic number .....

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevEmbedGems( int nItemSlot, int nItemTemplId, int nGemSlots[3], int nGemTemplId[3] )
{
	using namespace C2S;

#pragma pack(1)
	struct CONTENT
	{
		int		nItemSlot;				//装备-饰品在包裹栏的位置索引
		int		nItemTemplId;			//装备-饰品的模板ID，用于服务器的校验
		int		nGemTemplId[3];			//宝石的模板ID，用于服务器的校验
		int		nGemSlots[3];			//宝石在包裹栏中的位置索引
	};	
#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_EMBED_GEMS;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->nItemSlot		= nItemSlot;
	pContent->nItemTemplId	= nItemTemplId;
	memcpy( &pContent->nGemTemplId[0], &nGemTemplId[0], 3*sizeof(int) );//3 is a magic number .....
	memcpy( &pContent->nGemSlots[0], &nGemSlots[0], 3*sizeof(int) );//3 is a magic number .....

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdNPCSevRemoveGems( int nItemSlot, int nItemTemplId, bool bRemove[3] )
{
	using namespace C2S;

#pragma pack(1)
	struct CONTENT
	{
		int		nItemSlot;				//装备-饰品在包裹栏的位置索引
		int		nItemTemplId;			//装备-饰品的模板ID，用于服务器的校验
		bool	bRemove[3];				//表示对应的饰品的槽中的宝石是否拆除
	};	
#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_REMOVE_GEMS;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->nItemSlot		= nItemSlot;
	pContent->nItemTemplId	= nItemTemplId;
	memcpy( &pContent->bRemove[0], &bRemove[0], 3*sizeof(bool) );//3 is a magic number .....

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize); 
}

void c2s_SendCmdNPCSevUpgradeGems( int nGemSlot, int nGemTemplId, int nArticleSlots[12], int nArticleTemplId[12] )
{
	using namespace C2S;

#pragma pack(1)
	struct CONTENT
	{
		int		nGemSlot;				//宝石在包裹栏的位置索引
		int		nGemTemplId;			//宝石的模板ID，用于服务器的校验
		int		nArticleTemplId[12];	//升品道具的模板ID，用于服务器的校验
		int		nArticleSlots[12];		//升品道具在包裹栏中的位置索引
	};	
#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_UPGRADE_GEMS;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->nGemSlot		= nGemSlot;
	pContent->nGemTemplId	= nGemTemplId;
	memcpy( &pContent->nArticleTemplId[0], &nArticleTemplId[0], 12*sizeof(int) );//12 is a magic number .....
	memcpy( &pContent->nArticleSlots[0], &nArticleSlots[0], 12*sizeof(int) );//12 is a magic number .....

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize); 
}

void c2s_SendCmdNPCSevRefineGems( int nGemSlot, int nGemTemplId, int nArticleSlot, int nArticleTemplId )
{
	using namespace C2S;

#pragma pack(1)
	struct CONTENT
	{
		int		nGemSlot;				//宝石在包裹栏的位置索引
		int		nGemTemplId;			//宝石的模板ID，用于服务器的校验
		int		nArticleTemplId;		//精炼道具的模板ID，用于服务器的校验
		int		nArticleSlot;			//精炼道具在包裹栏中的位置索引
	};	
#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_REFINE_GEMS;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->nGemSlot			= nGemSlot;
	pContent->nGemTemplId		= nGemTemplId;
	pContent->nArticleTemplId	= nArticleTemplId;
	pContent->nArticleSlot		= nArticleSlot;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize); 
}

void c2s_SendCmdNPCSevExtractGems( int nGemSlot, int nGemTemplId )
{
	using namespace C2S;

#pragma pack(1)
	struct CONTENT
	{
		int		nGemSlot;				//宝石在包裹栏的位置索引
		int		nGemTemplId;			//宝石的模板ID，用于服务器的校验
	};	
#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_EXTRACT_GEMS;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->nGemSlot			= nGemSlot;
	pContent->nGemTemplId		= nGemTemplId;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize); 
}

void c2s_SendCmdNPCSevSmeltGems( int nSrcGemSlot, int nSrcGemTemplId, int nDestGemSlot, int nDestGemTemplId, int nArticleSlot, int nArticleTemplId )
{
	using namespace C2S;

#pragma pack(1)
	struct CONTENT
	{
		int		nSrcGemSlot;				//被熔炼宝石在包裹栏的位置索引
		int		nSrcGemTemplId;				//被熔炼宝石的模板ID，用于服务器的校验
		int		nDestGemSlot;				//熔炼后宝石在包裹栏的位置索引
		int		nDestGemTemplId;			//熔炼后宝石的模板ID，用于服务器的校验
		int		nArticleTemplId;			//熔炼道具的模板ID，用于服务器的校验
		int		nArticleSlot;				//熔炼道具在包裹栏中的位置索引
	};	
#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_SMELT_GEMS;
	pCmd->len = sizeof (CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->nSrcGemSlot			= nSrcGemSlot;
	pContent->nSrcGemTemplId		= nSrcGemTemplId;
	pContent->nDestGemSlot			= nDestGemSlot;
	pContent->nDestGemTemplId		= nDestGemTemplId;
	pContent->nArticleTemplId		= nArticleTemplId;
	pContent->nArticleSlot			= nArticleSlot;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize); 
}

/**
*@param: 地图ID号，准备开启副本房间的名称
*@return: void
*@usage: 发送消息给gs请求开启副本房间
*@date: 2011-07-18
**/
void c2s_SendCmdNPCSevOpenRaidRoom( int iMapId, int iRaidTemplateId, char cVote, const void* szName, char cDiffi )
{
	using namespace C2S;
	enum
	{
		MAX_NAME_LEN = 26,
	};

#pragma pack(1)
	struct CONTENT
	{
		int		map_id;						// 地图ID号，不同的类型的副本拥有不同的地图？
		int		raid_template_id;			// 副本模板ID Added 2011-07-20.
		char	vote;						// 是否投票踢人 Added 2012-07-24.
		char	difficulty;					// 难度
		size_t	room_name_len;				// 报名开启房间的名字长度
		char	room_name[1];				// 房间名字		
	};
#pragma pack()
	
	//首先,检查传入参数的合法性
	if ( iMapId < 0 || !szName )
	{
		return;
	}

	//获得副本房间名字的长度
	int iNameSize = (wcslen((wchar_t*)szName) + 1) * sizeof (wchar_t);
	if ( iNameSize > MAX_NAME_LEN ) // 是否需要严格限制房间名字长度?
	{
		ASSERT( false );
		return;
	}

	//计算总体的长度
	int iSize = sizeof(cmd_header) + sizeof (cmd_sevnpc_serve) + 2*sizeof(int) + 2*sizeof(char) + sizeof(size_t) + iNameSize;//分别计算协议每个数据项的长度之和

	//开辟数据空间
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )
	{
		return;
	}

	//存储相应的数据值
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof(cmd_header));
	pCmd->service_type = GP_OPEN_RAID_ROOM;
	pCmd->len = 2*sizeof(int) + 2*sizeof(char) + sizeof(size_t) + iNameSize;

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->map_id = iMapId;
	pContent->raid_template_id = iRaidTemplateId;
	pContent->vote = cVote;
	pContent->difficulty = cDiffi;
	pContent->room_name_len = iNameSize;
	memcpy( pContent->room_name, szName, iNameSize );

	//调用发送消息协议接口
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );

	//释放临时空间
	delete []pBuf;
}

/**
*@param: 地图ID，选择报名的副本房间ID
*@return: void
*@usage: 请求服务器报名进入选择的副本房间
*@date: 2011-07-18.
**/
void c2s_SendCmdNPCSevJoinRaidRoom( int iMapId, int iRaidTemplateId, int iRoomId ,char cFaction)
{
	using namespace C2S;

#pragma pack(1)
	struct CONTENT
	{
		int		map_id;				// 地图ID号
		int		raid_template_id;	// 副本模板ID
		int		room_id;			// 申请进入的房间号
		char    raid_faction;
	};
#pragma pack()

	//首先检查输入参数的合法性
	if ( iMapId < 0 || iRoomId < 0 )
	{
		return;
	}

	//计算整个消息协议的长度
	int iSize = sizeof(cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof(CONTENT);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) // 分配空间失败
	{
		return;
	}

	//存储相应的协议消息内容
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)( pBuf + sizeof(cmd_header) );
	pCmd->service_type = GP_JOIN_RAID_ROOM;
	pCmd->len = sizeof(CONTENT);

	BYTE* pData = pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pData;
	pContent->map_id = iMapId;
	pContent->raid_template_id = iRaidTemplateId;
	pContent->room_id = iRoomId;
	pContent->raid_faction = cFaction;

	//调用协议发送接口
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );

	//释放空间
	delete []pBuf;
}

void c2s_SendCmdFactionPKJoinRaidRoom(int iMapId,int iRaidTemplateId,int iRoomId,char cFaction)
{
    using namespace C2S;

	if(iMapId < 0 || iRoomId < 0 )
		return;
	int iSize = sizeof(cmd_header) + sizeof(join_raid_room);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
	   return;
	((cmd_header*)pBuf)->cmd = C2S::JOIN_RAID_ROOM;
	join_raid_room* pCmd = (join_raid_room*)(pBuf + sizeof(cmd_header));
	pCmd->mapid = iMapId;
	pCmd->raid_template_id = iRaidTemplateId;
	pCmd->roomid = iRoomId;
	pCmd->raid_faction = cFaction;

	g_pGame->GetGameSession()->SendGameData(pBuf,iSize);

	delete []pBuf;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void c2s_SendCmdGMMoveToPlayer(int idPlayer)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (gm_cmd_moveto_player);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GM_MOVETO_PLAYER;

	gm_cmd_moveto_player* pCmd = (gm_cmd_moveto_player*)(pBuf + sizeof (cmd_header));
	pCmd->pid = idPlayer;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdGMCallInPlayer(int idPlayer)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (gm_cmd_callin_player);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GM_CALLIN_PLAYER;

	gm_cmd_callin_player* pCmd = (gm_cmd_callin_player*)(pBuf + sizeof (cmd_header));
	pCmd->pid = idPlayer;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdGMGenerate(int tid)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (gm_cmd_generate);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GM_GENERATE;

	gm_cmd_generate* pCmd = (gm_cmd_generate*)(pBuf + sizeof (cmd_header));
	pCmd->tid = tid;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdGMActiveSpawner(bool bActive, int id_spawner)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (gm_cmd_active_spawner);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GM_ACTIVE_SPAWNER;

	gm_cmd_active_spawner* pCmd = (gm_cmd_active_spawner*)(pBuf + sizeof (cmd_header));
	pCmd->is_active = bActive ? 1 : 0;
	pCmd->sp_id = id_spawner;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdGMGenerateMob(int idMob, int idVis, short sCount, short sLifeTime, 
						const ACHAR* szName)
{
	using namespace C2S;

	int iNameLen = 0;
	if (szName || szName[0])
	{
		iNameLen = a_strlen(szName) * sizeof (ACHAR);
		a_ClampRoof(iNameLen, 18);
	}

	int iSize = sizeof (cmd_header) + sizeof (gm_cmd_generate_mob) + iNameLen;
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GM_GENERATE_MOB;

	gm_cmd_generate_mob* pCmd = (gm_cmd_generate_mob*)(pBuf + sizeof (cmd_header));
	pCmd->mob_id = idMob;
	pCmd->vis_id = idVis;
	pCmd->count = sCount >= 1 ? sCount : 1;
	pCmd->life = sLifeTime;
	pCmd->name_len = iNameLen;

	//	Copy name
	memcpy(pCmd+1, szName, iNameLen);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdGMTriggerInvisible()
{
	_SendNakeCommand(C2S::GM_INVISIBLE);
}

void c2s_SendCmdGMTriggerInvincible()
{
	_SendNakeCommand(C2S::GM_INVINCIBLE);
}

void c2s_SendCmdGMTransferMap(int idMap, float x, float y, float z)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (gm_cmd_transfer_map);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GM_TRANSFER_MAP;

	gm_cmd_transfer_map* pCmd = (gm_cmd_transfer_map*)(pBuf + sizeof (cmd_header));
	pCmd->idMap = idMap;
	pCmd->x = x;
	pCmd->y = y;
	pCmd->z = z;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdBotBegin(char flag, char answer_question)
{
	using namespace C2S;

#pragma pack(1)

	struct CONTENT
	{
		char flag;
		char answer_question;
	};

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	((cmd_header*)pBuf)->cmd = C2S::BOT_BEGIN;

	CONTENT* pCmd = (CONTENT*)(pBuf + sizeof (cmd_header));
	pCmd->flag = flag;
	pCmd->answer_question = answer_question;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	g_pGame->GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_7_5_TALISMAN_START);
}

void c2s_SendCmdBotRun()
{
	_SendNakeCommand(C2S::BOT_RUN);
}

void c2s_SendCmdBattleGetScore()
{
	_SendNakeCommand(C2S::GET_BATTLE_SCORE);
}

void c2s_SendCmdBattleGetInfo()
{
	_SendNakeCommand(C2S::GET_BATTLE_INFO);
}

void c2s_SendCmdSetFashionMask(int mask)
{
	using namespace C2S;

#pragma pack(1)

	struct CONTENT
	{
		int mask;
	};

#pragma pack()

	const int iSize = sizeof (cmd_header) + sizeof (CONTENT);
	BYTE pBuf[iSize];
	((cmd_header*)pBuf)->cmd = C2S::SET_FASHION_MASK;

	CONTENT* pCmd = (CONTENT*)(pBuf + sizeof (cmd_header));
	pCmd->mask = mask;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
}

void c2s_SendCmdStartOnlineAgent()
{
	_SendNakeCommand(C2S::START_ONLINE_AGENT);
}

void c2s_SendCmdJoinInstancing(int map_id)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_join_instancing);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::JOIN_INSTANCING;

	cmd_join_instancing* pCmd = (cmd_join_instancing*)(pBuf + sizeof (cmd_header));
	pCmd->map_id = map_id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;	
}

// 发布信息
void c2s_SendCmdPressSNSMessage(unsigned char msg_type, unsigned char occupation, unsigned char gender, 
								int level, int factionLevel,  const ACHAR* szMsg)
{
	using namespace C2S;
	int iMsgLen = 0;

	if (szMsg || szMsg[0])
	{
		iMsgLen = a_strlen(szMsg) * sizeof (ACHAR);
		a_ClampRoof(iMsgLen, 60);
	}

	int iSize = sizeof (cmd_header) + sizeof (cmd_press_sns_message) + iMsgLen;
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::PRESS_SNS_MESSAGE;

	cmd_press_sns_message* pCmd = (cmd_press_sns_message*)(pBuf + sizeof (cmd_header));
	pCmd->message_type = msg_type;
	pCmd->occupation  = occupation;
	pCmd->gender	= gender;
	pCmd->level		= level;
	pCmd->faction_level = factionLevel;
	pCmd->message_len = iMsgLen;
	// copy msg
	memcpy(pCmd+1, szMsg, iMsgLen);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;	
}

// 发布申请
void c2s_SendCmdApplySNSMessage(unsigned char msg_type, int msg_id, const ACHAR* szMsg)
{
	using namespace C2S;
	int iMsgLen = 0;

	if (szMsg || szMsg[0])
	{
		iMsgLen = a_strlen(szMsg) * sizeof (ACHAR);
		a_ClampRoof(iMsgLen, 60);
	}

	int iSize = sizeof (cmd_header) + sizeof (cmd_apply_sns_message) + iMsgLen;
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::APPLY_SNS_MESSAGE;

	cmd_apply_sns_message* pCmd = (cmd_apply_sns_message*)(pBuf + sizeof (cmd_header));
	pCmd->message_type = msg_type;
	pCmd->message_id  = msg_id;
	pCmd->message_len = iMsgLen;
	// copy msg
	memcpy(pCmd+1, szMsg, iMsgLen);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;	
}

// 投票
void c2s_SendCmdVoteSNSMessage(unsigned char msg_type, int msg_id, char vote_type)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_vote_sns_message);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::VOTE_SNS_MESSAGE;

	cmd_vote_sns_message* pCmd = (cmd_vote_sns_message*)(pBuf + sizeof (cmd_header));
	pCmd->message_type = msg_type;
	pCmd->message_id  = msg_id;
	pCmd->vote_type = vote_type;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;	
}

// 回复留言
void c2s_SendCmdResponseSNSMessage(unsigned char msg_type, int msg_id, short dst_index, const ACHAR* szMsg)
{
	using namespace C2S;
	int iMsgLen = 0;

	if (szMsg || szMsg[0])
	{
		iMsgLen = a_strlen(szMsg) * sizeof (ACHAR);
		a_ClampRoof(iMsgLen, 60);
	}

	int iSize = sizeof (cmd_header) + sizeof (cmd_response_sns_message) + iMsgLen;
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::RESPONSE_SNS_MESSAGE;

	cmd_response_sns_message* pCmd = (cmd_response_sns_message*)(pBuf + sizeof (cmd_header));
	pCmd->message_type = msg_type;
	pCmd->message_id  = msg_id;
	pCmd->dst_index   = dst_index;
	pCmd->message_len = iMsgLen;
	// copy msg
	memcpy(pCmd+1, szMsg, iMsgLen);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;	
}

//	Carrier about

void c2s_SendCmdEnterCarrier(int idCarrier, const A3DVECTOR3& vRelPos, unsigned char cRelDir)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_enter_carrier);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = ENTER_CARRIER;

	cmd_enter_carrier* pCmd = (cmd_enter_carrier*)(pBuf + sizeof (cmd_header));
	pCmd->carrier_id = idCarrier;
	pCmd->rpos = vRelPos;
	pCmd->rdir = cRelDir;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;	
}

void c2s_SendCmdLeaveCarrier(int idCarrier, const A3DVECTOR3& vPos, unsigned char cDir)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_leave_carrier);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = LEAVE_CARRIER;

	cmd_leave_carrier* pCmd = (cmd_leave_carrier*)(pBuf + sizeof (cmd_header));
	pCmd->carrier_id = idCarrier;
	pCmd->pos = vPos;
	pCmd->dir = cDir;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;	
}

void c2s_SendCmdMoveOnCarrier(const A3DVECTOR3& vCurRelPos, const A3DVECTOR3& vNextRelPos, unsigned short wUseTime, float fSpeed, int iMoveMode, unsigned short wCmdSeq)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_move_on_carrier);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = MOVE_ON_CARRIER;

	cmd_move_on_carrier* pCmd = (cmd_move_on_carrier*)(pBuf + sizeof (cmd_header));
	pCmd->vCurRPos = vCurRelPos;
	pCmd->vNextRPos = vNextRelPos;
	pCmd->use_time = wUseTime;
	pCmd->sSpeed = FLOATTOFIX8(fSpeed);
	pCmd->move_mode = unsigned char(iMoveMode);
	pCmd->cmd_seq =wCmdSeq;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;	
}

void c2s_SendCmdStopOnCarrier(const A3DVECTOR3& vRelDest, float fSpeed, int iMoveMode, BYTE byRelDir, WORD wStamp, int iTime)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_stop_on_carrier);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = STOP_ON_CARRIER;

	cmd_stop_on_carrier* pCmd = (cmd_stop_on_carrier*)(pBuf + sizeof (cmd_header));
	pCmd->rCurPos = vRelDest;
	pCmd->rdir = byRelDir;
	pCmd->sSpeed = FLOATTOFIX8(fSpeed);
	pCmd->use_time = WORD(iTime);
	pCmd->move_mode = iMoveMode;
	pCmd->stamp = wStamp;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete [] pBuf;
}

// 新增时装包裹

void c2s_SendCmdUpdateFashionHotkey(int iNumEntry, void* aEntries)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (int) + iNumEntry * sizeof (cmd_update_fashion_hotkey::fashion_hotkey);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::UPDATE_FASHION_HOTKEY;

	cmd_update_fashion_hotkey* pCmd = (cmd_update_fashion_hotkey*)(pBuf + sizeof (cmd_header));
	pCmd->count	= iNumEntry;

	//	Copy entry data
	if (iNumEntry)
		memcpy(pCmd->list, aEntries, iNumEntry * sizeof (cmd_update_fashion_hotkey::fashion_hotkey));

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdExgFashionItem(int iIndex1, int iIndex2)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_exg_fashion_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = EXG_FASHION_ITEM;

	cmd_exg_fashion_item* pCmd = (cmd_exg_fashion_item*)(pBuf + sizeof (cmd_header));
	pCmd->index1 = (BYTE)iIndex1;
	pCmd->index2 = (BYTE)iIndex2;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}


void c2s_SendCmdExgIvtrFashionItem(int iIvtrIdx, int iFasIdx)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_exg_ivtr_fashion);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = EXG_IVTR_FASHION;

	cmd_exg_ivtr_fashion* pCmd = (cmd_exg_ivtr_fashion*)(pBuf + sizeof (cmd_header));
	pCmd->idx_ivr	= (BYTE)iIvtrIdx;
	pCmd->idx_fas	= (BYTE)iFasIdx;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdExgEquFashionItem(int iEquIdx, int iFasIdx)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (cmd_exg_equ_fashion);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = EXG_EQUIP_FASHION;

	cmd_exg_equ_fashion* pCmd = (cmd_exg_equ_fashion*)(pBuf + sizeof (cmd_header));
	pCmd->idx_equ	= (BYTE)iEquIdx;
	pCmd->idx_fas	= (BYTE)iFasIdx;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdHotkeyChangeFashion(int iKeyIdx, int iHeadIdx, int iBodyIdx, int iShoeIdx)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (hotkey_change_fashion);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = HOTKEY_CHANGE_FASHION;

	hotkey_change_fashion* pCmd = (hotkey_change_fashion*)(pBuf + sizeof (cmd_header));
	pCmd->key_index = (BYTE)iKeyIdx;
	pCmd->idx1	= (BYTE)iHeadIdx;
	pCmd->idx2	= (BYTE)iBodyIdx;
	pCmd->idx3  = (BYTE)iShoeIdx;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdExgMountWingItem(int iIndex1, int iIndex2)
{
	using namespace C2S;
	
	int iSize = sizeof (cmd_header) + sizeof (cmd_exg_fashion_item);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = EXCHANGE_MOUNT_WING_ITEM;
	
	exchange_mount_wing_item* pCmd = (exchange_mount_wing_item*)(pBuf + sizeof (cmd_header));
	pCmd->index1 = (BYTE)iIndex1;
	pCmd->index2 = (BYTE)iIndex2;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete [] pBuf;
}


void c2s_SendCmdExgIvtrMountWingItem(int iIvtrIdx, int iMvIdx)
{
	using namespace C2S;
	
	int iSize = sizeof (cmd_header) + sizeof (cmd_exg_ivtr_fashion);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = EXCHANGE_INVENTORY_MOUNTWING_ITEM;
	
	exchange_inventory_mountwing_item* pCmd = (exchange_inventory_mountwing_item*)(pBuf + sizeof (cmd_header));
	pCmd->idx_inv	= (BYTE)iIvtrIdx;
	pCmd->idx_mw	= (BYTE)iMvIdx;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete [] pBuf;
}

void c2s_SendCmdExgEquMountWingItem(int iEquIdx, int iMvIdx)
{
	using namespace C2S;
	
	int iSize = sizeof (cmd_header) + sizeof (cmd_exg_equ_fashion);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = EXCHANGE_EQUIPMENT_MOUNTWING_ITEM;
	
	exchange_equipment_mountwing_item* pCmd = (exchange_equipment_mountwing_item*)(pBuf + sizeof (cmd_header));
	pCmd->idx_equ	= (BYTE)iEquIdx;
	pCmd->idx_mw	= (BYTE)iMvIdx;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete [] pBuf;
}

void c2s_SendCmdBonusMallShopping(unsigned short id, unsigned short index, unsigned short slot, unsigned short count)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (mall_shopping_2);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::BONUSMALL_SHOPPING;

	mall_shopping_2* pCmd = (mall_shopping_2*)(pBuf + sizeof (cmd_header));
	pCmd->goods_id		= id;
	pCmd->goods_index	= index;
	pCmd->goods_slot	= slot;
	pCmd->count			= count;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdZoneMallShopping(unsigned short id, unsigned short index, unsigned short slot, unsigned short count)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (mall_shopping_2);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::ZONEMALL_SHOPPING;

	mall_shopping_2* pCmd = (mall_shopping_2*)(pBuf + sizeof (cmd_header));
	pCmd->goods_id		= id;
	pCmd->goods_index	= index;
	pCmd->goods_slot	= slot;
	pCmd->count			= count;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdQueryOthersAchievement(const unsigned int idPlayer)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (query_others_achievement);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::QUERY_OTHERS_ACHIEVEMENT;
	query_others_achievement* pCmd = (query_others_achievement*)(pBuf + sizeof (cmd_header));
	pCmd->target			= idPlayer;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	
	delete [] pBuf;
}

void c2s_SendCmdGetAchievementAward(const unsigned int idAchieve, const unsigned int idAward)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (get_achievement_award);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::GET_ACHIEVEMENT_AWARD;
	get_achievement_award* pCmd = (get_achievement_award*)(pBuf + sizeof (cmd_header));
	pCmd->achieve_id	= idAchieve;
	pCmd->award_id		= idAward;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdUpdatePVPMask(BYTE mask)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (update_pvp_mask);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::UPDATE_PVP_MASK;
	update_pvp_mask* pCmd = (update_pvp_mask*)(pBuf + sizeof (cmd_header));
	pCmd->pvp_mask			= mask;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdExchgHomeTownMoney(int money)
{
	using namespace C2S;

	int iSize = sizeof (cmd_header) + sizeof (exchg_hometown_money);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::EXCHG_HOMETOWN_MONEY;
	exchg_hometown_money* pCmd = (exchg_hometown_money*)(pBuf + sizeof (cmd_header));
	pCmd->amount			= money;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdGetServerTime()
{
	_SendNakeCommand(C2S::GET_SERVER_TIME);
}

void c2s_SendCmdTerritoryLeave()
{
	_SendNakeCommand(C2S::TERRITORY_LEAVE);
}


void c2s_SendCmdUpdateCombineSkill(int idSkill, int iNumElement, int* aElements)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (update_combine_skill) - sizeof(int) + iNumElement * sizeof(int);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::UPDATE_COMBINE_SKILL;
	update_combine_skill* pCmd = (update_combine_skill*)(pBuf + sizeof(cmd_header));
	pCmd->skill_id = idSkill;
	pCmd->element_count = iNumElement;
	
	if (iNumElement)
		memcpy(pCmd->element_id, aElements, iNumElement* sizeof(int));
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete[] pBuf;
}

void c2s_SendCmdUniqueBid(int nBidMoneyLowerLimit, int nBidMoneyUpperLimit )//Modified 2011-02-16.
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (unique_bid);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::UNIQUE_BID;
	unique_bid* pCmd = (unique_bid*)(pBuf + sizeof(cmd_header));
	pCmd->nBidMoneyLowerLimit = nBidMoneyLowerLimit;//Modified 2011-02-16.
	pCmd->nBidMoneyUpperLimit = nBidMoneyUpperLimit;//Added 2011-02-16.
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete[] pBuf;
}

void c2s_SendCmdUniqueBidGetAward()
{
	_SendNakeCommand(C2S::UNIQUE_BID_GET_AWARD);
}

void c2s_SendCmdTaskFlyPos(int idTask, int idNpc)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof (task_fly_pos);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::TASK_FLY_POS;
	task_fly_pos* pCmd = (task_fly_pos*)(pBuf + sizeof(cmd_header));
	pCmd->task_id = idTask;
	pCmd->npc_id = idNpc;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete[] pBuf;
}

void c2s_SendCmdTaskReachSiteFly(int idTask)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (task_fly_to_area);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::TASK_FLY_TO_AREA;
	task_fly_to_area* pCmd = (task_fly_to_area*)(pBuf + sizeof(cmd_header));
	pCmd->taskid = idTask;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete[] pBuf;


}

void c2s_SendCmdActivityFlyPos(int idActivity)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof (activity_fly_pos);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::ACTIVITY_FLY_POS;
	activity_fly_pos* pCmd = (activity_fly_pos*)(pBuf + sizeof(cmd_header));
	pCmd->activity_id = idActivity;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete[] pBuf;
}

void c2s_SendCmdDeityLevelUp()
{
	_SendNakeCommand(C2S::DEITY_LEVELUP);
}

/**
*@param: 阵眼（阵法发起者）id
*@return: void
*@usage: 发送消息给gs请求查询当前阵眼所在阵法的信息
**/
void c2s_SendCmdQueryCodInfo( int nCodSponsorId )
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(query_circleofdoom_info);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )//开辟内存空间失败
	{
		return;
	}

	//存储命令相应的数据值
	((cmd_header*)pBuf)->cmd = C2S::QUERY_CIRCLE_OF_DOOM_INFO;
	query_circleofdoom_info* pCmd = (query_circleofdoom_info*)(pBuf + sizeof(cmd_header));
	pCmd->sponsor_id = nCodSponsorId;

	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );

	delete []pBuf;
}

/**
*@param: 被拉扯的玩家id
*@return: void
*@usage: 发送消息给gs请求查询当前被拉扯相关信息
**/
void c2s_SendCmdQueryBeDraggedInfo( int nBeDraggedPlayerId )
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(query_be_dragged_info);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟内存空间失败
	{
		return;
	}

	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::QUERY_BE_DRAGGED_INFO;
	query_be_dragged_info* pCmd = (query_be_dragged_info*)(pBuf + sizeof(cmd_header));
	pCmd->player_be_dragged_id = nBeDraggedPlayerId;

	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );

	delete []pBuf;
}

/**
*@param: 被牵引的玩家id
*@return: void
*@usage: 发送消息给gs请求查询当前牵引技能相关信息
**/
void c2s_SendCmdQueryBePulledInfo( int nBePulledPlayerId )
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(query_be_pulled_info);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟内存空间失败
	{
		return;
	}

	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::QUERY_BE_PULLED_INFO;
	query_be_pulled_info* pCmd = (query_be_pulled_info*)(pBuf + sizeof(cmd_header));
	pCmd->player_be_pulled_id = nBePulledPlayerId;

	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );

	delete []pBuf;
}

void c2s_SendCmdQueryMallSaleTimeItem()
{
	_SendNakeCommand(C2S::GET_MALL_SALETIME_ITEM);
}

/**
*@param: nMapId:副本地图ID
*@return: void
*@usage: 向gs查询玩家进入某副本的次数
*/
void c2s_SendCmdQueryRaidEnterCount( int nMapId )
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(query_raid_enter_count);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}

	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::QUERY_RAID_ENTER_COUNT;
	query_raid_enter_count* pCmd = (query_raid_enter_count*)(pBuf + sizeof(cmd_header));
	pCmd->map_id = nMapId;

	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );

	delete []pBuf;
}

/**
*@param: void
*@return:void
*@usage: 玩家主动通知gs离开副本
**/
void c2s_SendCmdLeaveRaid( void )
{
	_SendNakeCommand( C2S::RAID_LEAVE );
}

/**
*@param: void
*@return: void
*@usage: 施法者取消拉扯技能
**/
void c2s_SendCmdCancelPulling( void )
{
	_SendNakeCommand( C2S::CANCEL_PULLING );
}

/**
*@param: void
*@return: void
*@usage: 通知gs获得进入副本玩家信息
**/
void c2s_SendCmdGetRaidInfo( void )
{
	_SendNakeCommand( C2S::GET_RAID_INFO );
}

/**
*@param: void
*@return: void
*@usage: 向gs请求获得当前可以领取的vip奖励信息
**/
void c2s_SendCmdGetVipAwardInfo( void )
{
	_SendNakeCommand( C2S::GET_VIP_AWARD_INFO );
}

/**
*@param: nAwardID：奖励模板id， nItemID：奖励物品id
*@return: void
*@usage: 客户端向gs请求领取某一奖励
**/
void c2s_SendCmdObtainVipAwardByID( int nAwardID, int nItemID )
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(obtain_vip_award_by_id);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}

	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::OBTAIN_VIP_AWARD_BY_ID;
	obtain_vip_award_by_id* pCmd = (obtain_vip_award_by_id*)(pBuf + sizeof(cmd_header));
	pCmd->award_id = nAwardID;
	pCmd->item_id = nItemID;

	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );

	delete []pBuf;
}

/**
*@param: nAwardID:奖励大礼包ID， nAwardIndex: 大礼包中所对应的需要发放的小礼包位置， nSmallBagID:小礼包ID, bTryObtainFlag:true-〉TRY_GET_ONLINE_AWARD， false-〉GET_ONLINE_AWARD
*@return: void
*@usage: 客户端向gs请求验证当前某在线奖励是否能够发放,以及领取某在线奖励（走同一条消息结构）
**/
void c2s_SendCmdObtainOnlineAward( int nAwardID, int nAwardIndex, int nSmallBagID, bool bTryObtainFlag )
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(obtain_online_award);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}

	//存储命令相关的数值
	if ( bTryObtainFlag )
	{
		((cmd_header*)pBuf)->cmd = C2S::TRY_GET_ONLINE_AWARD;
	}
	else
	{
		((cmd_header*)pBuf)->cmd = C2S::GET_ONLINE_AWARD;
	}
	obtain_online_award* pCmd = (obtain_online_award*)(pBuf + sizeof(cmd_header));
	pCmd->award_id				= nAwardID;
	pCmd->award_index			= nAwardIndex;
	pCmd->award_small_bag_id	= nSmallBagID;

	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );

	delete []pBuf;
}

void c2s_SendFiveAnniRequest(char type)
{	
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(get_online_award);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	((cmd_header*)pBuf)->cmd = C2S::FIVE_ANNI_REQUEST;

	get_online_award* pCmd = (get_online_award*)(pBuf + sizeof(cmd_header));
	pCmd->request				= type;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdUpgradeTreasureRegion( int nItemID, int nItemSlot, int nRegionID, int nRegionPos )
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(upgrade_treasure_region);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}

	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::UPGRADE_TREASURE_REGION;
	upgrade_treasure_region* pCmd = (upgrade_treasure_region*)(pBuf + sizeof(cmd_header));
	pCmd->item_id		= nItemID;
	pCmd->item_slot		= nItemSlot;
	pCmd->region_id		= nRegionID;
	pCmd->region_pos	= nRegionPos;

	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdUnlockTreasureRegion( int nItemID, int nItemSlot, int nRegionID, int nRegionPos )
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(unlock_treasure_region);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}

	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::UNLOCK_TREASURE_REGION;
	unlock_treasure_region* pCmd = (unlock_treasure_region*)(pBuf + sizeof(cmd_header));
	pCmd->item_id		=	nItemID;
	pCmd->item_slot		=	nItemSlot;
	pCmd->region_id		=	nRegionID;
	pCmd->region_pos	=	nRegionPos;

	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdDigTreasureRegion( int nRegionID, int nRegionPos )
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(dig_treasure_region);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}

	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::DIG_TREASURE_REGION;
	dig_treasure_region* pCmd = (dig_treasure_region*)(pBuf + sizeof(cmd_header));
	pCmd->region_id		=	nRegionID;
	pCmd->region_pos	=	nRegionPos;

	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdStartRandomTowerMonster( char index )
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(start_random_tower_monster);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::START_RANDOM_TOWER_MONSTER;
	start_random_tower_monster* pCmd = (start_random_tower_monster*)(pBuf + sizeof(cmd_header));
	pCmd->click_index		=	index;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdAdoptLittlePet()
{
	_SendNakeCommand( C2S::ADOPT_LITTLEPET );
}

void c2s_SendCmdFeedLittlePet(int nItemId, int nItemCount)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(feed_littlepet);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::FEED_LITTLEPET;
	feed_littlepet* pCmd = (feed_littlepet*)(pBuf + sizeof(cmd_header));
	pCmd->item_id	=	nItemId;
	pCmd->item_count		=	nItemCount;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;

}

void c2s_SendCmdGetLittlePetAward()
{
	_SendNakeCommand( C2S::GET_LITTLEPET_AWARD );
}

void c2s_SendCmdRuneIdentify(int nRuneIndex, int nAssistIndex)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(rune_identify);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::RUNE_IDENTIFY;
	rune_identify* pCmd = (rune_identify*)(pBuf + sizeof(cmd_header));
	pCmd->rune_index	=	nRuneIndex;
	pCmd->assist_index	=	nAssistIndex;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdRuneCombine(int nRuneIndex, int nAssistIndex)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(rune_combine);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::RUNE_COMBINE;
	rune_combine* pCmd = (rune_combine*)(pBuf + sizeof(cmd_header));
	pCmd->rune_index	=	nRuneIndex;
	pCmd->assist_index	=	nAssistIndex;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdRuneRefine(int nRuneIndex, int nAssistIndex)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(rune_refine);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::RUNE_REFINE;
	rune_refine* pCmd = (rune_refine*)(pBuf + sizeof(cmd_header));
	pCmd->rune_index	=	nRuneIndex;
	pCmd->assist_index	=	nAssistIndex;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdRuneReset(int nRuneIndex, int nAssistIndex)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(rune_reset);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::RUNE_RESET;
	rune_reset* pCmd = (rune_reset*)(pBuf + sizeof(cmd_header));
	pCmd->rune_index	=	nRuneIndex;
	pCmd->assist_index	=	nAssistIndex;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdRuneDecompose(int nRuneIndex)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(rune_decompose);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::RUNE_DECOMPOSE;
	rune_decompose* pCmd = (rune_decompose*)(pBuf + sizeof(cmd_header));
	pCmd->rune_index	=	nRuneIndex;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdRuneLevelUp(int nRuneIndex)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(rune_levelup);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::RUNE_LEVELUP;
	rune_levelup* pCmd = (rune_levelup*)(pBuf + sizeof(cmd_header));
	pCmd->rune_index	=	nRuneIndex;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdRuneChangeSlot(int nRuneIndex, int nAssistIndex)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(rune_change_slot);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::RUNE_CHANGE_SLOT;
	rune_change_slot* pCmd = (rune_change_slot*)(pBuf + sizeof(cmd_header));
	pCmd->rune_index	=	nRuneIndex;
	pCmd->assist_index	=	nAssistIndex;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdRuneEraseSlot(int nRuneIndex, int nStoneIndex)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(rune_erase_slot);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::RUNE_ERASE_SLOT;
	rune_erase_slot* pCmd = (rune_erase_slot*)(pBuf + sizeof(cmd_header));
	pCmd->rune_index	=	nRuneIndex;
	pCmd->stone_index	=	nStoneIndex;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdTowerReward(int nTmpId, int nLevel, int type)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(tower_reward);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::TOWER_REWARD;
	tower_reward* pCmd = (tower_reward*)(pBuf + sizeof(cmd_header));
	pCmd->raid_template_id	=	nTmpId;
	pCmd->level	=	nLevel;
	pCmd->type	=	type;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}


// 领取特殊任务奖励
void c2s_SendCmdGetTaskAward(int type)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(get_task_award);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::GET_TASK_AWARD;
	get_task_award* pCmd = (get_task_award*)(pBuf + sizeof(cmd_header));
	pCmd->type	=	type;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

// 镶嵌符文
void c2s_SendCmdInstallSlot(int nRuneIndex, int nSlotIndex, int nStoneIndex, int nStoneId)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(rune_install_slot);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::RUNE_INSTALL_SLOT;
	rune_install_slot* pCmd = (rune_install_slot*)(pBuf + sizeof(cmd_header));
	pCmd->rune_index	=	nRuneIndex;
	pCmd->slot_index	=	nSlotIndex;
	pCmd->stone_index	=	nStoneIndex;
	pCmd->stone_id		=	nStoneId;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

// 元魂洗练是否接收结果
void c2s_SendCmdRefineAction(bool bAccept, int nIndex)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(rune_refine_action);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::RUNE_REFINE_ACTION;
	rune_refine_action* pCmd = (rune_refine_action*)(pBuf + sizeof(cmd_header));
	pCmd->accept_result	=	bAccept;
	pCmd->rune_index	=	nIndex;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdResetSkillPropFree(unsigned char opcode)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(reset_skill_prop);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::RESET_SKILL_PROP;
	reset_skill_prop* pCmd = (reset_skill_prop*)(pBuf + sizeof(cmd_header));
	pCmd->opcode	=	opcode;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdCancelTop3Guess(int area)
{
	using namespace C2S;	

	int iSize = sizeof(cmd_header) + sizeof(pk_top3_guess);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::PK_TOP3_GUESS;
	pk_top3_guess* pCmd = (pk_top3_guess*)(pBuf + sizeof(cmd_header));
	pCmd->cancel	= true;
	pCmd->area		= area;
	pCmd->cnt		= 1;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;	
}

void c2s_SendCmdPK1stGuessReward()
{
	_SendNakeCommand(C2S::PK_1ST_GUESS_REWARD);
}

void c2s_SendCmdPK1stGuessResultReward()
{
	_SendNakeCommand(C2S::PK_1ST_GUESS_RESULT_REWARD);	
}

void c2s_SendCmdPKTop3GuessReward()
{
	_SendNakeCommand(C2S::PK_TOP3_GUESS_REWARD);	
}

void c2s_SendCmdPKTop3GuessResultReward(char type)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(pk_top3_guess_result_reward);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::PK_TOP3_GUESS_RESULT_REWARD;
	pk_top3_guess_result_reward* pCmd = (pk_top3_guess_result_reward*)(pBuf + sizeof(cmd_header));
	pCmd->type	=	type;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;	
}

void c2s_SendCmdGetPlayerBetData()
{
	_SendNakeCommand(C2S::GET_PLAYER_BET_DATA);	
}


void c2s_SendCmdStartSpecialMove( const A3DVECTOR3& velocity, const A3DVECTOR3& acceleration, const A3DVECTOR3& vCurPos, char collisionState )
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(start_special_move);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}

	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::START_SPECIAL_MOVE;
	start_special_move* pCmd = (start_special_move*)(pBuf + sizeof(cmd_header));
	pCmd->velocity			= velocity;
	pCmd->acceleration		= acceleration;
	pCmd->cur_pos			= vCurPos;			//新增加开始移动时的位置同步
	pCmd->collision_state	= collisionState;
	pCmd->time_stamp		= g_pGame->GetServerGMTTime();

	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdSpecialMove( const A3DVECTOR3& velocity, const A3DVECTOR3& acceleration, const A3DVECTOR3& curPos, unsigned short useTime, unsigned short stamp, char collisionState )
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(special_move);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}

	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::SPECIAL_MOVE;
	special_move* pCmd = (special_move*)(pBuf + sizeof(cmd_header));
	pCmd->velocity			= velocity;
	pCmd->acceleration		= acceleration;
	pCmd->cur_pos			= curPos;
//	pCmd->use_time			= useTime;	//Canceled 2012-09-01. No Use!
	pCmd->stamp				= stamp;
	pCmd->collision_state	= collisionState;
	pCmd->time_stamp		= g_pGame->GetServerGMTTime();	//新增加发放协议的时间戳，用于对时

	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdStopSpecialMove( unsigned char curDir, const A3DVECTOR3& vCurPos, unsigned short stamp )
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(stop_special_move);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}

	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::STOP_SPECIAL_MOVE;
	stop_special_move* pCmd = (stop_special_move*)(pBuf + sizeof(cmd_header));
	pCmd->cur_dir		= curDir;
	pCmd->cur_pos		= vCurPos;
	pCmd->stamp			= stamp;

	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdChangeWingColor( unsigned char cColorIndex )
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(change_wing_color);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}

	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::CHANGE_WING_COLOR;
	change_wing_color* pCmd = (change_wing_color*)(pBuf + sizeof(cmd_header));
	pCmd->wing_color		= cColorIndex;

	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmd6v6CollisionApply(int mapId, int tmplId, bool isTeam,bool isCross)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(collision_battleground_apply);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::COLLISION_BATTLEGROUND_APPLY;
	collision_battleground_apply* pCmd = (collision_battleground_apply*)(pBuf + sizeof(cmd_header));
	pCmd->map_id		= mapId;
	pCmd->raid_template_id = tmplId;
	pCmd->is_team		= isTeam;
	pCmd->is_cross = isCross;
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdGetRePurchaseInv()
{
	_SendNakeCommand(C2S::GET_REPURCHASE_INV_DATA);
}

void c2s_SendCmdHideVipLevel(char cHide)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(hide_vip_level);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::HIDE_VIP_LEVEL;
	hide_vip_level* pCmd = (hide_vip_level*)(pBuf + sizeof(cmd_header));
	pCmd->is_hide		= cHide;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

// 星座鉴定
void c2s_SendCmdAstrologyIdentify(int item_id, int item_index)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(astrology_identify);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::ASTROLOGY_IDENTIFY;
	astrology_identify* pCmd = (astrology_identify*)(pBuf + sizeof(cmd_header));
	pCmd->item_id = item_id;
	pCmd->item_index = item_index;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

// 星座升级
void c2s_SendCmdAstrologyUpgrade(int item_id, int item_index, int stone_id, int stone_index)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(astrology_upgrade);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::ASTROLOGY_UPGRADE;
	astrology_upgrade* pCmd = (astrology_upgrade*)(pBuf + sizeof(cmd_header));
	pCmd->item_id = item_id;
	pCmd->item_index = item_index;
	pCmd->stone_id = stone_id;
	pCmd->stone_index = stone_index;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

// 星座粉碎
void c2s_SendCmdAstrologyDestory(int item_id, int item_index)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(astrology_destory);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::ASTROLOGY_DESTORY;
	astrology_destory* pCmd = (astrology_destory*)(pBuf + sizeof(cmd_header));
	pCmd->item_id = item_id;
	pCmd->item_index = item_index;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdNPCSevTransmitServe()
{
	_SendEmptyDataNPCSev(GP_NPCSEV_TRANSFER_SERVE);

}

void c2s_SendCmdNPCSevBattleLeaveTransmitServe()
{
	_SendEmptyDataNPCSev(GP_NPCSEV_TRANSFER_SERVE_BATTLE_LEAVE);
}

// 金身法身快速制作
void c2s_SendCmdNPCSevProduceJinFaShen(char type, char produce_idx, int zhaohua[3], short money_item_count, short material_cnt, C2S::npc_produce_jinfashen_material* materials)
{
	if (!materials && material_cnt != 0)
		return;

	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		char  produce_type;
		char  produce_idx;	// 0: sell item,  1:buy back
        int   zhaohua[3];	// 0:仙 1:佛 2:魔
        short money_item_count;	// 5000金物品消耗数量
		short material_cnt;
	};
#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT) +
				material_cnt * sizeof (npc_produce_jinfashen_material);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	BYTE* pTemp = pBuf + sizeof (cmd_header);
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)pTemp;
	pCmd->service_type = GP_NPCSEV_PRODUCE_JINFASHEN;
	pCmd->len = sizeof (CONTENT) + material_cnt * sizeof (npc_produce_jinfashen_material);

	pTemp += sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pTemp;
	pContent->produce_type = type;
	pContent->produce_idx = produce_idx;
	pContent->zhaohua[0] = zhaohua[0];
	pContent->zhaohua[1] = zhaohua[1];
	pContent->zhaohua[2] = zhaohua[2];
	pContent->money_item_count = money_item_count;
	pContent->material_cnt = material_cnt;

	//	Copy item data ...
	if (material_cnt > 0)
	{
		pTemp += sizeof (CONTENT);
		memcpy(pTemp, materials, material_cnt * sizeof (npc_produce_jinfashen_material));
	}

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

// 宠物飞升
void c2s_SendCmdNPCSevPetFly(int iPetIndex, int iAssistIndex)
{
	using namespace C2S;

#pragma pack(1)

	struct CONTENT
	{
		int pet_index;  //物品 包裹栏索引
		int assist_index;
	};
#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	BYTE* pTemp = pBuf + sizeof (cmd_header);
	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)pTemp;
	pCmd->service_type = GP_NPCSEV_PET_FLY;
	pCmd->len = sizeof (CONTENT);

	pTemp += sizeof (cmd_sevnpc_serve);
	CONTENT* pContent = (CONTENT*)pTemp;
	pContent->pet_index = iPetIndex;
	pContent->assist_index = iAssistIndex;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdTalismanSkillRefineResult(int item_index1, int item_id1, char result)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(talisman_refineskill_result);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::TALISMAN_REFINESKILL_RESULT;
	talisman_refineskill_result* pCmd = (talisman_refineskill_result*)(pBuf + sizeof(cmd_header));
	pCmd->index		= item_index1;
	pCmd->id		= item_id1;
	pCmd->result	= result;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

void c2s_SendCmdGetCollisionAward(bool daily, int index)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(get_collision_award);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::GET_COLLISION_AWARD;
	get_collision_award* pCmd = (get_collision_award*)(pBuf + sizeof(cmd_header));
	pCmd->daily_award		= daily;
	pCmd->award_index		= index;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}

// 首次登陆不足1小时下线原因
void c2s_SendCmdPlayerFirstExitReason(unsigned char reason)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(player_first_exit_reason);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::PLAYER_FIRST_EXIT_REASON;
	player_first_exit_reason* pCmd = (player_first_exit_reason*)(pBuf + sizeof(cmd_header));
	pCmd->reason = reason;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}

// 基础属性增益丹药合成
void c2s_SendCmdGenPropAddItem(int material_id, short material_index, short material_count)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(gen_prop_add_item);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::GEN_PROP_ADD_ITEM;
	gen_prop_add_item* pCmd = (gen_prop_add_item*)(pBuf + sizeof(cmd_header));
	pCmd->material_id = material_id;
	pCmd->material_idx = material_index;
	pCmd->material_cnt = material_count;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

// 基础属性增益丹药重炼
void c2s_SendCmdRebuildPropAddItem(int item_id, short item_index, int forge_id, short forge_index)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(rebuild_prop_add_item);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) //开辟空间失败
	{
		return;
	}
	
	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::REBUILD_PROP_ADD_ITEM;
	rebuild_prop_add_item* pCmd = (rebuild_prop_add_item*)(pBuf + sizeof(cmd_header));
	pCmd->itemId = item_id;
	pCmd->itemIdx = item_index;
	pCmd->forgeId = forge_id;
	pCmd->forgeIdx = forge_index;
	
	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );
	
	delete []pBuf;
}

// 获取基础属性增益
void c2s_SendCmdGetPropAdd()
{
	_SendNakeCommand(C2S::GET_PROPADD);
}

void c2s_SendCmdMergeMedicine(int bottle_id,int bottle_index,int potion_id,int potion_index)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(merge_medicine);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )
	{
		return;
	}
	
	((cmd_header*)pBuf)->cmd = C2S::MERGE_POTION;
	merge_medicine* pCmd = (merge_medicine*)(pBuf + sizeof(cmd_header));
	pCmd->bottle_id = bottle_id;
	pCmd->bottle_index = bottle_index;
	pCmd->potion_id = potion_id;
	pCmd->potion_index = potion_index;
	
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}


void c2s_SendCmdRemedyMetempsyChosisLevel(int nItemIndex)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(remedy_metempsychosis_level);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )
	{
		return;
	}
	
	((cmd_header*)pBuf)->cmd = C2S::REMEDY_METEMPSYCHOSIS_LEVEL;
	remedy_metempsychosis_level* pCmd = (remedy_metempsychosis_level*)(pBuf + sizeof(cmd_header));
	pCmd->item_index = nItemIndex;

	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}

void c2s_SendCmdGetCollisionPlayerPos(int nPlayerId)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(get_collision_player_pos);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )
	{
		return;
	}
	
	((cmd_header*)pBuf)->cmd = C2S::GET_COLLISION_PLAYER_POS;
	get_collision_player_pos* pCmd = (get_collision_player_pos*)(pBuf + sizeof(cmd_header));
	pCmd->role_id = nPlayerId;
	
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}


void c2s_SendCmdLiveness(int nItemIndex)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(take_liveness_award);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )
	{
		return;
	}
	
	((cmd_header*)pBuf)->cmd = C2S::TAKE_LIVENESS_AWARD;
	take_liveness_award* pCmd = (take_liveness_award*)(pBuf + sizeof(cmd_header));
	pCmd->grade = nItemIndex;

	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}
void c2s_SendCmdKingdomLeave()
{
   	using namespace C2S;
	
	int iSize = sizeof(cmd_header);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )
	{
		return;
	}
	((cmd_header*)pBuf)->cmd = C2S::KINGDOM_LEAVE;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	delete []pBuf;
}
//亲卫传送
void c2s_SendCmdKingTryCallGuard()
{
    _SendNakeCommand(C2S::KING_TRY_CALL_GUARD);
}
//搓澡邀请
void c2s_SendCmdKingBathInvite(int who)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(bath_invite);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )
	{
		return;
	}
	
	((cmd_header*)pBuf)->cmd = C2S::BATH_INVITE;
	bath_invite* pCmd = (bath_invite*)(pBuf + sizeof(cmd_header));
	pCmd->who = who;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}
//搓澡邀请回复
void c2s_SendCmdKingBathInviteReply(int who,int param)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(bath_invite_reply);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )
	{
		return;
	}
	
	((cmd_header*)pBuf)->cmd = C2S::BATH_INVITE_REPLY;
	bath_invite_reply* pCmd = (bath_invite_reply*)(pBuf + sizeof(cmd_header));
	pCmd->who = who;
	pCmd->param = param;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}
//发布国王任务
void c2s_SendCmdDeliverKingTask(int type)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(deliver_kingdom_task);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )
	{
		return;
	}
	
	((cmd_header*)pBuf)->cmd = C2S::DELIVER_KINGDOM_TASK;
	deliver_kingdom_task* pCmd = (deliver_kingdom_task*)(pBuf + sizeof(cmd_header));
	pCmd->task_type = type;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}
//接受国王任务
void c2s_SendCmdReceiveKingTask(int type)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof(receive_kingdom_task);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )
	{
		return;
	}
	
	((cmd_header*)pBuf)->cmd = C2S::RECEIVE_KINGDOM_TASK;
	receive_kingdom_task* pCmd = (receive_kingdom_task*)(pBuf + sizeof(cmd_header));
	pCmd->tasktype = type;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}
//传到国战NPC位置
void c2s_SendCmdKingFlyNPCPos()
{
    _SendNakeCommand(C2S::KINGDOM_FLY_BATH_POS);
}
//领取连任奖励
void c2s_SendCmdKingGetReward()
{
	_SendNakeCommand(C2S::GET_KING_REWARD);
}
void c2s_SendCmdAncientLevelAward(int level)
{	
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (ancient_raid_level_award);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::ANCIENT_RAID_LEVEL_AWARD;
	
	ancient_raid_level_award* pCmd = (ancient_raid_level_award*)(pBuf + sizeof(cmd_header));
	pCmd->level = level;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}

void c2s_SendCmdAncientLevelResult()
{
	_SendNakeCommand(C2S::GET_RAID_LEVEL_RESULT);
}
void c2s_CmdOpenTrashBox(const char* szPsw)
{
	using namespace C2S;
	
#pragma pack(1)
	
	struct open_trashbox
	{
		unsigned int psw_size;
	};   
	
#pragma pack()
	
	int iLen = 0;
	if (szPsw && szPsw[0])
		iLen = strlen(szPsw);
	
	int iSize = sizeof (cmd_header) + sizeof (open_trashbox) + iLen;
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::OPEN_TRASHBOX;
	
	BYTE* pTemp = pBuf + sizeof (cmd_header);
	open_trashbox* pContent = (open_trashbox*)pTemp;
	pContent->psw_size = iLen;
	pTemp += sizeof (open_trashbox);
	
	if (iLen)
	{
		memcpy(pTemp, szPsw, iLen);
		pTemp += iLen;
	}
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete [] pBuf;
}
void c2s_SendCmdGiftLotteryDeliver(int index, int id)
{
	using namespace C2S;
	
	int iSize = sizeof(cmd_header) + sizeof (deliver_gift_bag);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::DELIVER_GIFT_BAG;
	
	deliver_gift_bag* pCmd = (deliver_gift_bag*)(pBuf + sizeof(cmd_header));
	pCmd->index = index;
	pCmd->item_id = id;
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}

void c2s_SendCmdGetCashGiftAward()
{
	_SendNakeCommand(C2S::GET_CASH_GIFT_AWARD);
}

void c2s_SendCmdFlowBattleLeave()
{
	_SendNakeCommand(C2S::FLOW_BATTLE_LEAVE);
}

void c2s_SendCmdFlowBattleApply()
{
	_SendNakeCommand(C2S::REGISTER_FLOW_BATTLE);
}

void c2s_SendCmdResizeInventory(char type, int newsize, int itemnum, void* item)
{
	using namespace C2S;
	int iSize = sizeof(cmd_header) + sizeof(char) + 2 * sizeof(int) + itemnum * sizeof(resize_inv::Item_Info);

	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;
	((cmd_header*)pBuf)->cmd = RESIZE_INVENTORY;

	resize_inv* pCmd = (resize_inv*)(pBuf + sizeof(cmd_header));
	pCmd->type = type;
	pCmd->new_size = newsize;
	pCmd->cnt = itemnum;
	if(itemnum)
		memcpy(pCmd->cost_item, item, itemnum * sizeof(resize_inv::Item_Info));
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	delete [] pBuf;
}

void c2s_SendCmdArrangeInventory(char page)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (arrange_inventory);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::ARRANGE_INVENTORY;
	
	arrange_inventory* pCmd = (arrange_inventory*)(pBuf + sizeof(cmd_header));
	pCmd->page = page;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}

void c2s_SendCmdArrangeTrashBox(char page)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (arrange_trashbox);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::ARRANGE_TRASHBOX;
	
	arrange_trashbox* pCmd = (arrange_trashbox*)(pBuf + sizeof(cmd_header));
	pCmd->page = page;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}

void c2s_SendCmdGetWebOrder(__int64 order_id)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (get_web_order);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::GET_WEB_ORDER;
	
	get_web_order* pCmd = (get_web_order*)(pBuf + sizeof(cmd_header));
	pCmd->order_id = order_id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}

void c2s_SendCmdCustomsizeRune(int type, int count, void *prop)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + 2 * sizeof(int) + count * sizeof(int);
	
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::CUSTOMIZE_RUNE;

	customize_rune* pCmd = (customize_rune*)(pBuf + sizeof(cmd_header));
	pCmd->type = type;
	pCmd->count = count;

	if(count)
		memcpy(pCmd->prop, prop, count * sizeof(int));
	
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdControltrap(int id, int tid)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(control_trap);

	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::CONTROL_TRAP;

	control_trap* pCmd = (control_trap*)(pBuf + sizeof(cmd_header));
	pCmd->id = id;
	pCmd->tid = tid;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}

void c2s_SendCmdSummonTeleport(int npc_id)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(summon_teleport);

	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::SUMMON_TELEPORT;

	summon_teleport* pCmd = (summon_teleport*)(pBuf + sizeof(cmd_header));
	pCmd->npc_id = npc_id;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}

void c2s_SendLookBattleInfo(int mapid)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (get_Pkbattle_info);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::LOOK_BATTLE_INFO;
	
	get_Pkbattle_info* pCmd = (get_Pkbattle_info*)(pBuf + sizeof(cmd_header));
	pCmd->map_id = mapid;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);
	
	delete []pBuf;
}

void c2s_SendCmdQilinInvite(int who)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(qilin_invite);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )
	{
		return;
	}

	((cmd_header*)pBuf)->cmd = C2S::QILIN_INVITE;
	qilin_invite* pCmd = (qilin_invite*)(pBuf + sizeof(cmd_header));
	pCmd->who = who;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}

void c2s_SendCmdQilinInviteReply(int who,int param)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(qilin_invite_reply);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )
	{
		return;
	}

	((cmd_header*)pBuf)->cmd = C2S::QILIN_INVITE_REPLY;
	qilin_invite_reply* pCmd = (qilin_invite_reply*)(pBuf + sizeof(cmd_header));
	pCmd->who = who;
	pCmd->param = param;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}

void c2s_SendCmdCancelQilinBinding()
{
	_SendNakeCommand(C2S::QILIN_CANCEL);
}

void c2s_SendCmdQilinUp()
{
	_SendNakeCommand(C2S::QILIN_RECONNECT);
}

void c2s_SendCmdQilinDown()
{
	_SendNakeCommand(C2S::QILIN_DISCONNECT);
}

void c2s_SendCmdPuppetFormChange()
{
	_SendNakeCommand(C2S::PUPPET_FORM_CHANGE);
}

void c2s_SendCmdInviteActiveEmote(int playerid, int type)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(invite_active_emote);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )
	{
		return;
	}

	((cmd_header*)pBuf)->cmd = C2S::INVITE_ACTIVE_EMOTE;
	invite_active_emote* pCmd = (invite_active_emote*)(pBuf + sizeof(cmd_header));
	pCmd->playerid = playerid;
	pCmd->type = type;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}

void c2s_SendCmdReplyActiveEmote(int playerid, int type, int answer)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(reply_active_emote);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf )
	{
		return;
	}

	((cmd_header*)pBuf)->cmd = C2S::REPLY_ACTIVE_EMOTE;
	reply_active_emote* pCmd = (reply_active_emote*)(pBuf + sizeof(cmd_header));
	pCmd->playerid = playerid;
	pCmd->type = type;
	pCmd->answer = answer;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}

void c2s_SendCmdStopActiveEmote()
{
	_SendNakeCommand(C2S::STOP_ACTIVE_EMOTE);
}

void c2s_SendCmdZaibian()
{
	_SendNakeCommand(C2S::ZAIBIAN_RAID);
}

void c2s_SendCreateCrossvrTeam(const ACHAR* szName)
{
	using namespace C2S;
	//	Copy name
	int iNameLen = 0;
	if (szName || szName[0])
	{
		iNameLen = a_strlen(szName) * sizeof (ACHAR);
		a_ClampRoof(iNameLen, 18);
	}
	int iSize = sizeof (cmd_header) + sizeof (create_crossvr_tem) + iNameLen;
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::CREATE_CROSSVR_TEAM;
	create_crossvr_tem* pCmd = (create_crossvr_tem*)(pBuf + sizeof (cmd_header));
	pCmd->name_len = iNameLen;

	//	Copy name
	memcpy(pCmd+1, szName, iNameLen);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}



void c2s_SendFashionColorantCombine(int nPos1,int nPos2,int nPos3)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (fashion_colorant_combine);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::FASHION_COLORANT_COMBINE;

	fashion_colorant_combine* pCmd = (fashion_colorant_combine*)(pBuf + sizeof(cmd_header));
	pCmd->colorant_index1 = nPos1;
	pCmd->colorant_index2 = nPos2;
	pCmd->colorant_index3 = nPos3;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}

void c2s_SendChangeFashionColor(int nPackType, int nPackPos,int nPos)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (fashion_add_color);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::FASHION_ADD_ITEM_COLOR;

	fashion_add_color* pCmd = (fashion_add_color*)(pBuf + sizeof(cmd_header));
	pCmd->colorant_index = nPos;
	pCmd->nPackType = nPackType;
	pCmd->fashion_item_index = nPackPos;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}


void c2s_SendEndter_Faction_Base( )
{
	_SendNakeCommand( C2S::ENTER_FACTION_BASE );
}


// 创建帮派基地建筑
void c2s_SendAddFacBuild(int fid,int field_index,int building_tid)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (add_fac_building);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::ADD_FAC_BUILDING;

	add_fac_building* pCmd = (add_fac_building*)(pBuf + sizeof(cmd_header));
	pCmd->fid = fid;
	pCmd->field_index = field_index;
	pCmd->building_tid = building_tid;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}
// 升级帮派基地建筑
void c2s_SendUpGradeFacBuild(int fid,int field_index,int building_tid,int nlevel)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (upgrade_fac_building);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::UPGRADE_FAC_BUILDING;

	upgrade_fac_building* pCmd = (upgrade_fac_building*)(pBuf + sizeof(cmd_header));
	pCmd->fid = fid;
	pCmd->field_index = field_index;
	pCmd->building_tid = building_tid;
	pCmd->cur_level = nlevel;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}
// 删除帮派基地建筑
void c2s_SendRemoveFacBuild(int fid,int field_index,int building_tid)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (remove_fac_building);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::REMOVE_FAC_BUILDING;

	remove_fac_building* pCmd = (remove_fac_building*)(pBuf + sizeof(cmd_header));
	pCmd->fid = fid;
	pCmd->field_index = field_index;
	pCmd->building_tid = building_tid;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}

void c2s_SendCmdFacBaseInfo()
{
	_SendNakeCommand(C2S::GET_FAC_BASE_INFO);
}

void c2s_SendCmdCS_6v6Award(int nType, int nId)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (get_cs_6v6_award);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::GET_CS_6V6_AWARD;

	get_cs_6v6_award* pCmd = (get_cs_6v6_award*)(pBuf + sizeof(cmd_header));
	pCmd->award_type = nType;
	pCmd->award_level = nId;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}

// 请求帮派商城信息
void c2s_SendCmdMallInfo()
{
	_SendNakeCommand(C2S::GET_FACBASE_MALL_INFO);
}
// 商城购物
void c2s_SendCmdShopMallInfo(int item_index,int item_id,int cnt)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (shop_from_facbase_mall);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::SHOP_FROM_FACBASE_MALL;

	shop_from_facbase_mall* pCmd = (shop_from_facbase_mall*)(pBuf + sizeof(cmd_header));
	pCmd->item_index = item_index;
	pCmd->item_id = item_id;
	pCmd->cnt = cnt;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}
//帮派捐献
void c2s_SendCmdFacbaseMoney(int nMoney)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (contribute_facbase_money);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::CONTRIBUTE_FACBASE_MONEY;

	contribute_facbase_money* pCmd = (contribute_facbase_money*)(pBuf + sizeof(cmd_header));
	pCmd->cnt = nMoney;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}

// 元宝商城 物品index 到映射时间冷却关系
void c2s_SendCmdFacbaseMoneyItemInfo()
{
	_SendNakeCommand(C2S::GET_FACBASE_MONEY_ITEMS_INFO);
}

// 帮派基地竞拍
void c2s_SendCmdBigOnFacbase(int item_index,int item_id,int coupon,const ACHAR* szName)
{
	using namespace C2S;

	int iNameLen = 0;
	if (szName || szName[0])
	{
		iNameLen = a_strlen(szName) * sizeof (ACHAR);
		a_ClampRoof(iNameLen, 18);
	}

	//	Copy name
	int iSize = sizeof(cmd_header) + sizeof (bid_on_facbase) + iNameLen;
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::BID_ON_FACBASE;

	bid_on_facbase* pCmd = (bid_on_facbase*)(pBuf + sizeof(cmd_header));
	pCmd->item_index = item_index;
	pCmd->item_id = item_id;
	pCmd->coupon = coupon;
	pCmd->name_len = iNameLen;
	memcpy(pCmd+1, szName, iNameLen);
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);


	delete []pBuf;
}
/// 元宝商城购买物品
void c2s_SendCmdBuyFacbaseCashItem(int item_index)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (buy_facbase_cash_item);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::BUY_FACBASE_CASH_ITEM;

	buy_facbase_cash_item* pCmd = (buy_facbase_cash_item*)(pBuf + sizeof(cmd_header));
	pCmd->item_index = item_index;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}

void c2s_SendCmdCS_6v6_ExchangeAward(int index,int item_id,int currency_id)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (exchange_cs_6v6_award);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::EXCHANGE_CS_6V6_AWARD;

	exchange_cs_6v6_award* pCmd = (exchange_cs_6v6_award*)(pBuf + sizeof(cmd_header));
	pCmd->index = index;
	pCmd->item_id = item_id;
	pCmd->currency_id = currency_id;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}

void c2s_SendCmdPostFacBaseMsg(int nFacId,const ACHAR* szMsg)
{
	using namespace C2S;
	//	Copy name
	int iNameLen = 0;
	if (szMsg || szMsg[0])
	{
		iNameLen = a_strlen(szMsg) * sizeof (ACHAR);
		a_ClampRoof(iNameLen, 100);
	}
	int iSize = sizeof (cmd_header) + sizeof (post_fac_base_msg) + iNameLen;
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::POST_FAC_BASE_MSG;
	post_fac_base_msg* pCmd = (post_fac_base_msg*)(pBuf + sizeof (cmd_header));
	pCmd->msg_len = iNameLen;
	pCmd->fid = nFacId;
	//	Copy name
	memcpy(pCmd+1, szMsg, iNameLen);

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdFacBaseTransfer(int nIndex)
{

	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (fac_base_transfer);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::FAC_BASE_TRANSFER;

	fac_base_transfer* pCmd = (fac_base_transfer*)(pBuf + sizeof(cmd_header));
	pCmd->region_index = nIndex;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;

}

void c2s_SendCmdExchangeCs6v6Money(int nType, int nCount)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (exchange_cs_6v6_money);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::EXCHANGE_CS_6V6_MONEY;

	exchange_cs_6v6_money* pCmd = (exchange_cs_6v6_money*)(pBuf + sizeof(cmd_header));
	pCmd->type = nType;
	pCmd->count = nCount;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}

void c2s_SendCmdLevelFaction()
{
_SendNakeCommand(C2S::LEACE_FACTION_BASE);
}

void c2s_SendCmdCharegePos(int type,int target_id, A3DVECTOR3& vPos)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof(object_chartge_to);
	BYTE* pBuf = new BYTE[iSize];
	if ( !pBuf ) 
	{
		return;
	}

	//存储命令相关的数值
	((cmd_header*)pBuf)->cmd = C2S::OBJECT_CHARGE_TO;
	object_chartge_to* pCmd = (object_chartge_to*)(pBuf + sizeof(cmd_header));
	pCmd->type		= type;
	pCmd->target_id	= target_id;
	pCmd->destpos	= vPos;

	g_pGame->GetGameSession()->SendGameData( pBuf, iSize );

	delete []pBuf;
}

void c2s_SendCmdOueSideFacBaseInfo()
{
	using namespace C2S;

	int iSize = sizeof (cmd_header);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = GET_FAC_BASE_INFO;
	g_pGame->GetGameSession()->SendFacBaseDataSend(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdGetFacBaseAuction()
{
	using namespace C2S;

	int iSize = sizeof (cmd_header);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = GET_FACBASE_AUCTION;
	g_pGame->GetGameSession()->SendFacBaseDataSend(pBuf, iSize);

	delete [] pBuf;
}

void c2s_SendCmdWithdrawFacBaseCoupon()
{
	_SendNakeCommand(C2S::WITHDRAW_FACBASE_COUPON);
}

void c2s_SendCmdWithdrawFacBaseAuction(int nItemIndex)
{
	using namespace C2S;

	int iSize = sizeof(cmd_header) + sizeof (withdraw_facbase_auction);
	BYTE* pBuf = new BYTE[iSize];
	if(!pBuf)
		return;

	((cmd_header*)pBuf)->cmd = C2S::WITHDRAW_FACBASE_AUCTION;

	withdraw_facbase_auction* pCmd = (withdraw_facbase_auction*)(pBuf + sizeof(cmd_header));
	pCmd->item_index = nItemIndex;
	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete []pBuf;
}

void c2s_SendCmdStopFreeFly()
{
	_SendNakeCommand(C2S::STOP_TRAVEL_AROUND);
}

