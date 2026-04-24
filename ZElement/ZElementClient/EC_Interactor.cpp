/*
 * FILE: EC_Interactor.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Li Bin, 2013.07.22
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_Interactor.h"
#include "EC_Game.h"
#include "EC_Model.h"
#include "EC_Utility.h"
#include "EC_PateText.h"
#include "EC_SceneLoader.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_GameRun.h"
#include "EC_ManPlayer.h"
#include "EC_World.h"
#include "EC_FixedMsg.h"
#include "EC_GPDataType.h"
#include "EC_CDR.h"
#include "EC_Player.h"
#include "DlgBase.h"
#include "elementdataman.h"

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

static const char* _hanger_interactor	= "Interactor%d";

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECInteractor
//	
///////////////////////////////////////////////////////////////////////////

CECInteractor::CECInteractor(CECNPCMan* pNPCMan) : CECNPC(pNPCMan)
{
	m_iCID				= OCID_INTERACTOR;
	m_pDBEssence		= NULL;
	m_iInteractiveNum	= 0;
	m_bShowUI			= false;
	m_bAttached			= false;
}

CECInteractor::~CECInteractor()
{
}

//	Initlaize object
bool CECInteractor::Init(int tid, const S2C::info_npc& Info)
{
	if (!CECNPC::Init(tid, Info))
		return false;

	for (int i = 0; i < MAX_INTERACTIVED_PLAYER_NUM; ++ i )
	{
		m_itInteractiveData[i].m_iPlayerID			= 0;
		m_itInteractiveData[i].m_sState				= NO_INTERACTIVE;
		m_itInteractiveData[i].m_pPlayerModel		= NULL;
		m_itInteractiveData[i].m_sPlayerAction		= "";
		m_itInteractiveData[i].m_sPlayerMoveAction	= "";
		m_itInteractiveData[i].m_sObjAction			= "";
		m_itInteractiveData[i].m_sObjMoveAction		= "";
	}

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (INTERACTION_OBJECT_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	m_pMonEssence = (MONSTER_ESSENCE*)pDB->get_data_ptr(m_pDBEssence->monster_id, ID_SPACE_ESSENCE, DataType);

	//	Submit EC model loading request to loading thread
	QueueECModelForLoad(MTL_ECM_NPC, Info.nid, GetBornStamp(), GetServerPos(), 
		g_pGame->GetDataPath(m_pDBEssence->file_matter), NULL, tid);

	SetUseGroundNormal(m_pMonEssence && m_pMonEssence->stand_mode == 0 ? true : false);

	m_strName = m_pDBEssence->name;

	if (m_pMonEssence)
	{
		m_fTouchRad	= m_pMonEssence->size;
		m_BasicProps.iLevel = m_pMonEssence->level;

		//	If NPC doesn't have specific name, use the name in database
		if (!(Info.state & GP_STATE_NPC_NAME))
		{
			m_strName = m_pMonEssence->name;
			if (m_pPateName)
				m_pPateName->SetText(m_strName, false);
		}
		m_strDesc = m_pMonEssence->monster_desc;
		if (m_pPateDesc && m_strDesc != _AL(""))
			m_pPateDesc->SetText(m_strDesc, false);
	}
	else
	{
		m_fTouchRad = 1.0f;
		m_BasicProps.iLevel = 0;
	}

	float fExt = m_fTouchRad * 1.5f;
	m_aabbDefPick.Extents.Set(fExt, fExt, fExt);
	m_pcdr->vExts = m_aabbDefPick.Extents;



	A3DVECTOR3 vPos = Info.pos;
	SetPos(vPos);
	SetDirAndUp(glb_DecompressDirH(Info.dir), g_vAxisY);

	StartWork(WT_NOTHING, WORK_STAND);

	return true;
}

//	Release object
void CECInteractor::Release()
{
	CECPlayerMan* pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();

	for (int i = 0; i < MAX_INTERACTIVED_PLAYER_NUM; ++ i)
	{
		if (m_itInteractiveData[i].m_iPlayerID == 0)
			continue;

		CECPlayer *pPlayer = pPlayerMan->GetPlayer(m_itInteractiveData[i].m_iPlayerID);
		if (pPlayer)
			pPlayer->CancelInteract(m_NPCInfo.nid);
		CancelInteract(m_itInteractiveData[i].m_iPlayerID);

		m_itInteractiveData[i].m_iPlayerID = 0;
	}

	CECNPC::Release();
}

//	Set loaded model to NPC object, this function is used in multithread loading process
bool CECInteractor::SetNPCLoadedResult(const EC_NPCLOADRESULT& Ret)
{
	CECNPC::SetNPCLoadedResult(Ret);
	return true;
}

//	Tick routine
bool CECInteractor::Tick(DWORD dwDeltaTime)
{
	//	Show UI.
	if (m_bShowUI)
	{
		CDlgBase* pDlg = (CDlgBase*) g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_InteractionOption");
		if (pDlg)
			pDlg->Show(true);
	}

	//	Update attach status.
	CECPlayerMan* pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	for (int i = 0; i < MAX_INTERACTIVED_PLAYER_NUM; ++ i)
	{
		if (m_itInteractiveData[i].m_iPlayerID == 0)
			continue;

		if (m_itInteractiveData[i].m_iAttachTime >= 0)
		{
			if (m_itInteractiveData[i].m_iAttachTime < (int) dwDeltaTime)
			{
				m_bAttached = true;

				if (IsSubject())
				{
					AttachPlayerModel(i);

					// Play actions.
					PlayModelAction(m_iAction, 1.0f, false);
				}
			}

			m_itInteractiveData[i].m_iAttachTime -= dwDeltaTime;
		}

		if (m_itInteractiveData[i].m_iDetachTime >= 0)
		{
			if (m_itInteractiveData[i].m_iDetachTime < (int) dwDeltaTime)
			{
				m_bAttached = false;

				if (IsSubject())
				{
					DetachPlayerModel(i);

					// Play actions.
					PlayModelAction(m_iAction, 1.0f, false);
				}
			}

			m_itInteractiveData[i].m_iDetachTime -= dwDeltaTime;
		}
	}

	CECNPC::Tick(dwDeltaTime);

	if (!IsSubject() || !IsInteractive())
		return true;

	//	If is render subject, tick npc model.
	if (IsRenderSubject())
	{
		if (m_pNPCModel)
		{
			m_pNPCModel->SetPos(GetPos());
			m_pNPCModel->Tick(dwDeltaTime);
		}
	}

	for (int i = 0; i < MAX_INTERACTIVED_PLAYER_NUM; ++ i)
	{
		if (m_itInteractiveData[i].m_iPlayerID == 0)
			continue;

		CECPlayer *pPlayer = pPlayerMan->GetPlayer(m_itInteractiveData[i].m_iPlayerID);
		pPlayer->SetPos(GetPos());

		//	If is not render subject, tick player model.
		if (!IsRenderSubject() && m_itInteractiveData[i].m_pPlayerModel)
			m_pNPCModel->Tick(dwDeltaTime);
	}

	if(m_idSummoner && m_strCompName.GetLength() == 0)
	{
		BuildCompleteName();
	}
	
	return true;
}

//	Render routine
bool CECInteractor::Render(CECViewport* pViewport, int iRenderFlag/* 0 */)
{
	// If not in interactive.
	if (!m_bAttached)
	{
		CECNPC::Render(pViewport, iRenderFlag);
		return true;
	}

	// If interactor is object, no render.
	if (!IsSubject())
		return true;

	CECModel* npcModel = m_pNPCModel;
	// If interactor is render object, replace the NPC model with player model for render.
	if (!IsRenderSubject())
		m_pNPCModel = m_itInteractiveData[0].m_pPlayerModel;

	CECNPC::Render(pViewport, iRenderFlag);

	// After render, restore npc model.
	m_pNPCModel = npcModel;

	return true;
}

//	Build complete name
void CECInteractor::BuildCompleteName()
{
	const ACHAR* szMasterName = g_pGame->GetGameRun()->GetPlayerName(m_idSummoner, false);
}
//	Get NPC name color
DWORD CECInteractor::GetNameColor()
{
	DWORD dwNameCol = A3DCOLORRGB(255, 192, 0);
	if (IsInBattleInvaderCamp())
	{
		//	In invader camp in battle
		dwNameCol = NAMECOL_BC_RED;
	}
	else if (IsInBattleDefenderCamp())
	{
		//	In defender camp in battle
		dwNameCol = NAMECOL_BC_BLUE;
	}
	else if (m_idSummoner)
	{
		CECPlayerMan* pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
		CECPlayer* pPlayer = pPlayerMan->GetPlayer(m_idSummoner);
		if (!pPlayer)
			dwNameCol = CECNPC::GetNameColor();
		else
			dwNameCol =  pPlayer->GetNameColor();	
	}
	else if (m_pMonEssence)
	{
		switch (m_pMonEssence->name_color)
		{
		case 0: {
			CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
			dwNameCol = pHost->DecideTargetNameCol(m_BasicProps.iLevel);
			break; }
		case 1:
			dwNameCol = 0xffec870e;
			break;
		case 2:
			dwNameCol = 0xffd9d919;
			break;
		case 3:
			dwNameCol = 0xff945305;
			break;
		case 4:
			dwNameCol = 0xff6ec3c9;
			break;
		case 5:
			dwNameCol = 0xff9370db;
			break;
		case 6:
			dwNameCol = 0xffff1cae;
			break;
		case 7:
			dwNameCol = 0xff3232cd;
			break;
		}
	}

	return dwNameCol;
}

float CECInteractor::GetSize()
{
	return m_pMonEssence->size;
}

bool CECInteractor::StartInteract(int playerid, int pos)
{
	if (pos >= MAX_INTERACTIVED_PLAYER_NUM || (m_itInteractiveData[pos].m_iPlayerID != 0 && m_itInteractiveData->m_iPlayerID != playerid))
		return false;

	DATA_TYPE DataType;
	INTERACTION_OBJECT_HOOK_CONFIG* pHookData = (INTERACTION_OBJECT_HOOK_CONFIG*) g_pGame->GetElementDataMan()->get_data_ptr(m_pDBEssence->hook[pos], ID_SPACE_CONFIG, DataType);
	if (DataType != DT_INTERACTION_OBJECT_HOOK_CONFIG)
		return false;

	CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(playerid);
	if (!pPlayer)
		return false;

	// Stop old interaction.
	if (m_itInteractiveData[pos].m_iPlayerID == 0)
	{
		m_iInteractiveNum ++;
	}
	else if (m_itInteractiveData[pos].m_iPlayerID != playerid)
	{
		CancelInteract(m_itInteractiveData[pos].m_iPlayerID);
		m_iInteractiveNum ++;
	}

	m_itInteractiveData[pos].m_iPlayerID			= playerid;
	m_itInteractiveData[pos].m_pPlayerModel			= NULL;
	m_itInteractiveData[pos].m_sState				= START_INTERACT;
	m_itInteractiveData[pos].m_sPlayerAction		= WC2AS(pHookData->start_character_action);
	m_itInteractiveData[pos].m_sPlayerMoveAction	= "";
	m_itInteractiveData[pos].m_sObjAction			= WC2AS(pHookData->start_object_action);
	m_itInteractiveData[pos].m_sObjMoveAction		= "";
	m_itInteractiveData[pos].m_iAttachTime			= pHookData->prepare_time;
	m_itInteractiveData[pos].m_iDetachTime			= -1;

	//	Show cancel UI.
	if (pPlayer->IsHostPlayer() && pHookData->showbutton)
	{
		m_bShowUI = true;
		CDlgBase* pDlg = (CDlgBase*) g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_InteractionOption");
		pDlg->GetDlgItem("confirm")->SetText(pHookData->buttontext);
		pDlg->Show(m_bShowUI);
	}

	if (!IsSubject())
		return true;

	//	Play actions.
	PlayModelAction(m_iAction, 1.0f, false);

	return true;
}

bool CECInteractor::InInteract(int playerid, int pos)
{
	if (pos >= MAX_INTERACTIVED_PLAYER_NUM || (m_itInteractiveData[pos].m_iPlayerID != 0 && m_itInteractiveData->m_iPlayerID != playerid))
		return false;

	DATA_TYPE DataType;
	INTERACTION_OBJECT_HOOK_CONFIG* pHookData = (INTERACTION_OBJECT_HOOK_CONFIG*) g_pGame->GetElementDataMan()->get_data_ptr(m_pDBEssence->hook[pos], ID_SPACE_CONFIG, DataType);
	if (DataType != DT_INTERACTION_OBJECT_HOOK_CONFIG)
		return false;

	CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(playerid);
	if (!pPlayer)
		return false;

	// Stop old interaction.
	if (m_itInteractiveData[pos].m_iPlayerID == 0)
	{
		m_iInteractiveNum ++;
	}
	else if (m_itInteractiveData[pos].m_iPlayerID != playerid)
	{
		CancelInteract(m_itInteractiveData[pos].m_iPlayerID);
		m_iInteractiveNum ++;
	}

	m_itInteractiveData[pos].m_iPlayerID			= playerid;
	m_itInteractiveData[pos].m_pPlayerModel			= NULL;
	m_itInteractiveData[pos].m_sState				= IN_INTERACT;
	m_itInteractiveData[pos].m_sPlayerAction		= WC2AS(pHookData->static_character_action);
	m_itInteractiveData[pos].m_sPlayerMoveAction	= WC2AS(pHookData->move_character_action);
	m_itInteractiveData[pos].m_sObjAction			= WC2AS(pHookData->static_object_action);
	m_itInteractiveData[pos].m_sObjMoveAction		= WC2AS(pHookData->move_object_action);
	m_itInteractiveData[pos].m_iAttachTime			= 0;
	m_itInteractiveData[pos].m_iDetachTime			= -1;


	//	Show cancel UI.
	if (pPlayer->IsHostPlayer() && pHookData->showbutton)
	{
		m_bShowUI = true;
		CDlgBase* pDlg = (CDlgBase*) g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_InteractionOption");
		pDlg->GetDlgItem("confirm")->SetText(pHookData->buttontext);
		pDlg->Show(m_bShowUI);
	}

	if (!IsSubject())
		return true;

	// Play actions.
	PlayModelAction(m_iAction, 1.0f, false);

	//	Set player's position.
	pPlayer->SetPos(GetServerPos());

	return true;
}

bool CECInteractor::EndInteract(int playerid, int pos)
{
	if (pos >= MAX_INTERACTIVED_PLAYER_NUM || (m_itInteractiveData[pos].m_iPlayerID != 0 && m_itInteractiveData->m_iPlayerID != playerid))
		return false;

	DATA_TYPE DataType;
	INTERACTION_OBJECT_HOOK_CONFIG* pHookData = (INTERACTION_OBJECT_HOOK_CONFIG*) g_pGame->GetElementDataMan()->get_data_ptr(m_pDBEssence->hook[pos], ID_SPACE_CONFIG, DataType);
	if (DataType != DT_INTERACTION_OBJECT_HOOK_CONFIG)
		return false;

	CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(playerid);
	if (!pPlayer)
		return false;

	// Stop old interaction.
	if (m_itInteractiveData[pos].m_iPlayerID == 0)
	{
		m_iInteractiveNum ++;
	}
	else if (m_itInteractiveData[pos].m_iPlayerID != playerid)
	{
		CancelInteract(m_itInteractiveData[pos].m_iPlayerID);
		m_iInteractiveNum ++;
	}

	m_itInteractiveData[pos].m_iPlayerID			= playerid;
	m_itInteractiveData[pos].m_pPlayerModel			= NULL;
	m_itInteractiveData[pos].m_sState				= END_INTERACT;
	m_itInteractiveData[pos].m_sPlayerAction		= WC2AS(pHookData->end_character_action);
	m_itInteractiveData[pos].m_sPlayerMoveAction	= "";
	m_itInteractiveData[pos].m_sObjAction			= WC2AS(pHookData->end_object_action);
	m_itInteractiveData[pos].m_sObjMoveAction		= "";
	m_itInteractiveData[pos].m_iAttachTime			= 0;
	m_itInteractiveData[pos].m_iDetachTime			= 500;


	if (!IsSubject())
		return true;

	// Play actions.
	PlayModelAction(m_iAction, 1.0f, false);

	//	Set player's position.
	pPlayer->SetPos(GetServerPos());

	return true;
}

bool CECInteractor::FinishInteract(int playerid, int pos)
{
	for (int i = 0; i < MAX_INTERACTIVED_PLAYER_NUM; ++ i)
	{
		if (m_itInteractiveData[i].m_iPlayerID != playerid)
			continue;

		//	Detach model.
		DetachPlayerModel(i);

		//	Detach player.
		m_itInteractiveData[pos].m_iPlayerID			= 0;
		m_itInteractiveData[pos].m_sState				= NO_INTERACTIVE;
		m_itInteractiveData[pos].m_sState				= NO_INTERACTIVE;
		m_itInteractiveData[pos].m_sPlayerAction		= "";
		m_itInteractiveData[pos].m_sPlayerMoveAction	= "";
		m_itInteractiveData[pos].m_sObjAction			= "";
		m_itInteractiveData[pos].m_sObjMoveAction		= "";

		m_iInteractiveNum --;
		if (m_iInteractiveNum <= 0)
		{
			m_iInteractiveNum = 0;
			m_bAttached = false;
		}

		// Play actions.
		PlayModelAction(m_iAction, 1.0f, false);

		CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(playerid);
		if (!pPlayer)
			return false;

		if (!IsSubject())
		{
			SetPos(GetPos());
			SetDirAndUp(GetDir(), GetUp());
		}

		if (pPlayer->IsHostPlayer())
		{
			DATA_TYPE DataType;
			INTERACTION_OBJECT_HOOK_CONFIG* pHookData = (INTERACTION_OBJECT_HOOK_CONFIG*) g_pGame->GetElementDataMan()->get_data_ptr(m_pDBEssence->hook[pos], ID_SPACE_CONFIG, DataType);
			if (DataType != DT_INTERACTION_OBJECT_HOOK_CONFIG)
				return false;

			if (pHookData->showbutton)
			{
				m_bShowUI = false;
				CDlgBase* pDlg = (CDlgBase*) g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_InteractionOption");
				if (pDlg)
					pDlg->Show(m_bShowUI);
			}
			
		}

		return true;
	}

	return false;
}

bool CECInteractor::CancelInteract(int playerid)
{
	for (int i = 0; i < MAX_INTERACTIVED_PLAYER_NUM; ++ i)
	{
		if (m_itInteractiveData[i].m_iPlayerID != playerid)
			continue;

		//	Detach model.
		DetachPlayerModel(i);

		//	Detach player.
		m_itInteractiveData[i].m_iPlayerID			= 0;
		m_itInteractiveData[i].m_sState				= NO_INTERACTIVE;
		m_itInteractiveData[i].m_sState				= NO_INTERACTIVE;
		m_itInteractiveData[i].m_sPlayerAction		= "";
		m_itInteractiveData[i].m_sPlayerMoveAction	= "";
		m_itInteractiveData[i].m_sObjAction			= "";
		m_itInteractiveData[i].m_sObjMoveAction		= "";

		m_iInteractiveNum --;
		if (m_iInteractiveNum <= 0)
		{
			m_iInteractiveNum = 0;
			m_bAttached = false;
		}

		// Play actions.
		PlayModelAction(m_iAction, 1.0f, false);

		CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(playerid);
		if (!pPlayer)
			return false;

		if (!IsSubject())
		{
			SetPos(GetPos());
			SetDirAndUp(GetDir(), GetUp());
		}

		if (pPlayer->IsHostPlayer())
		{
			DATA_TYPE DataType;
			INTERACTION_OBJECT_HOOK_CONFIG* pHookData = (INTERACTION_OBJECT_HOOK_CONFIG*) g_pGame->GetElementDataMan()->get_data_ptr(m_pDBEssence->hook[i], ID_SPACE_CONFIG, DataType);
			if (DataType != DT_INTERACTION_OBJECT_HOOK_CONFIG)
				return false;

			if (pHookData->showbutton)
			{
				m_bShowUI = false;
				CECGameUIMan *gameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
				if (gameUI && gameUI->GetDialog("Win_InteractionOption"))
					gameUI->GetDialog("Win_InteractionOption")->Show(m_bShowUI);
			}
		}

		return true;
	}

	return false;
}

void CECInteractor::AttachPlayerModel(int pos)
{
	if (!IsSubject() || pos >= MAX_INTERACTIVED_PLAYER_NUM)
		return;

	DATA_TYPE DataType;
	INTERACTION_OBJECT_HOOK_CONFIG* pHookData = (INTERACTION_OBJECT_HOOK_CONFIG*) g_pGame->GetElementDataMan()->get_data_ptr(m_pDBEssence->hook[pos], ID_SPACE_CONFIG, DataType);
	if (DataType != DT_INTERACTION_OBJECT_HOOK_CONFIG)
		return;

	CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(m_itInteractiveData[pos].m_iPlayerID);
	if (!pPlayer || m_itInteractiveData[pos].m_pPlayerModel != NULL)
		return;

	//	Set player's position.
	pPlayer->SetPos(GetServerPos());

	//	Attach model.
	m_itInteractiveData[pos].m_pPlayerModel = pPlayer->GetPlayerModel();

	AString hangerName;
	hangerName.Format(_hanger_interactor, pos);
	if (IsRenderSubject())
	{
		m_pNPCModel->AddChildModel(hangerName, false, WC2AS(pHookData->object_hook_name), m_itInteractiveData[pos].m_pPlayerModel, WC2AS(pHookData->character_hook_name));
		m_pNPCModel->GetA3DSkinModel()->Update(0);
	}
	else
	{
		m_itInteractiveData[pos].m_pPlayerModel->AddChildModel(hangerName, false, WC2AS(pHookData->character_hook_name), m_pNPCModel, WC2AS(pHookData->object_hook_name));
		m_itInteractiveData[pos].m_pPlayerModel->GetA3DSkinModel()->Update(0);
	}
}

void CECInteractor::DetachPlayerModel(int pos)
{
	if (pos >= MAX_INTERACTIVED_PLAYER_NUM || m_itInteractiveData[pos].m_iPlayerID == 0)
		return;

	SetPos(GetPos());
	SetDirAndUp(GetDir(), GetUp());

	AString hangerName;
	hangerName.Format(_hanger_interactor, pos);
	if (IsRenderSubject())
	{
		if (m_pNPCModel)
			m_pNPCModel->RemoveChildModel(hangerName, false);
	}
	else
	{
		if (m_itInteractiveData[pos].m_pPlayerModel)
			m_itInteractiveData[pos].m_pPlayerModel->RemoveChildModel(hangerName, false);
	}

	m_itInteractiveData[pos].m_pPlayerModel = NULL;
}