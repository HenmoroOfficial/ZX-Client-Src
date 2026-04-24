/*
 * FILE: EC_NPCServer.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/4
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_NPCServer.h"
#include "EC_Game.h"
#include "EC_Model.h"
#include "EC_Utility.h"
#include "EC_Skill.h"
#include "EC_PateText.h"
#include "EC_ImageRes.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_TaskInterface.h"
#include "EC_HostPlayer.h"
#include "EC_SceneLoader.h"
#include "EC_GPDataType.h"
#include "TaskTempl.h"
#include "TaskTemplMan.h"
#include "elementdataman.h"
#include "AUI\AUILuaManager.h"
#include "EC_UIManager.h"
#include "EC_CDR.h"
#include "EC_GameSession.h"
#include "Network\ids.hxx"
#include "globaldataman.h"
#include "EC_ManNPC.h"
#include "EL_Precinct.h"
#include "A3DViewport.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif

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

static const char* _male_statue_path[] =
{
	"Models\\NPCs\\NPCs\\ÄÐÊ¯Ïñ\\ÄÐÊ¯Ïñ.ecm",
	"Models\\NPCs\\NPCs\\ÄÐÌúÏñ\\ÄÐÌúÏñ.ecm",
	"Models\\NPCs\\NPCs\\ÄÐÍ­Ïñ\\ÄÐÍ­Ïñ.ecm",
	"Models\\NPCs\\NPCs\\ÄÐ½ðÏñ\\ÄÐ½ðÏñ.ecm",
	"Models\\NPCs\\NPCs\\ÄÐË®¾§Ïñ\\ÄÐË®¾§Ïñ.ecm",
};

static const char* _female_statue_path[] =
{
	"Models\\NPCs\\NPCs\\Å®Ê¯Ïñ\\Å®Ê¯Ïñ.ecm",
	"Models\\NPCs\\NPCs\\Å®ÌúÏñ\\Å®ÌúÏñ.ecm",
	"Models\\NPCs\\NPCs\\Å®Í­Ïñ\\Å®Í­Ïñ.ecm",
	"Models\\NPCs\\NPCs\\Å®½ðÏñ\\Å®½ðÏñ.ecm",
	"Models\\NPCs\\NPCs\\Å®Ë®¾§Ïñ\\Å®Ë®¾§Ïñ.ecm",
};

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

inline int _DecideStatueModelIndex(int occ_time, int cur_time)
{
	int diff = cur_time - occ_time;

	if (diff > 365 * 24 * 3600)
		return 4;
	else if (diff > 90 * 24 * 3600)
		return 3;
	else if (diff > 30 * 24 * 3600)
		return 2;
	else if (diff > 7 * 24 * 3600)
		return 1;
	else
		return 0;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECNPCServer
//	
///////////////////////////////////////////////////////////////////////////

CECNPCServer::CECNPCServer(CECNPCMan* pNPCMan) : CECNPC(pNPCMan)
{
	m_iCID			= OCID_SERVER;
	m_pDBEssence	= NULL;
	m_pMonEssence	= NULL;
	m_fTaxRate		= 0.05f;
	m_bQuestIcon	= false;
	m_iQuestIcon	= QI_NONE;
	m_bCityStatue	= false;
	m_bMaleStatue	= false;
	m_bStatueChanged= false;
	m_bLoadStatueModelFlag = false;
	m_bGetStatueNameFlag= false;
	m_nStatueIndex	= 0;
	m_nCityMasterID	= 0;
}

CECNPCServer::~CECNPCServer()
{
}

//	Initlaize object
bool CECNPCServer::Init(int tid, const S2C::info_npc& Info)
{
	if (!CECNPC::Init(tid, Info))
		return false;

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence = (NPC_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);

	//	Some server NPC data stored in another place, get it.
	m_pMonEssence = (MONSTER_ESSENCE*)pDB->get_data_ptr(m_pDBEssence->id_src_monster, ID_SPACE_ESSENCE, DataType);
	if (!m_pMonEssence)
	{
		if (!(m_pMonEssence = (MONSTER_ESSENCE*)pDB->get_data_ptr(4249, ID_SPACE_ESSENCE, DataType)))
		{
			a_LogOutput(1, "CECNPCServer::Init, server NPC reference to null monster data");
			return false;
		}
	}
	
	SetUseGroundNormal(m_pMonEssence->stand_mode == 0 ? true : false);
	m_fTouchRad	= m_pMonEssence->size;
	m_BasicProps.iLevel = m_pMonEssence->level;
	SPECIAL_ID_CONFIG& cfg = g_pGame->GetGameRun()->GetSpecialIDConfig();

	if (tid == cfg.id_male_war_statue || tid == cfg.id_female_war_statue)
	{
		m_bCityStatue = true;
		m_bMaleStatue = (tid == cfg.id_male_war_statue);
		g_pGame->GetGameSession()->war_infoget(1);
		g_pGame->GetGameRun()->SetMasterFlag(true);
	}
	else
	{
		//	Submit EC model loading request to loading thread
		QueueECModelForLoad(MTL_ECM_NPC, Info.nid, GetBornStamp(), GetServerPos(), 
			g_pGame->GetDataPath(m_pDBEssence->file_model), NULL, tid);
	}

//	float fExt = m_fTouchRad * 1.5f;
//	m_aabbDefPick.Extents.Set(fExt, fExt, fExt);
	m_pcdr->vExts = m_aabbDefPick.Extents;

	//	If NPC doesn't have specific name, use the name in database
	if (!(Info.state & GP_STATE_NPC_NAME))
	{
		m_strName = m_pDBEssence->name;
		if (m_pPateName)
			m_pPateName->SetText(m_strName, false);
	}
	m_strDesc = m_pDBEssence->desc;
	if (m_pPateDesc && m_strDesc != _AL(""))
		m_pPateDesc->SetText(m_strDesc, false);

	A3DVECTOR3 vPos = Info.pos;
	m_vServerDir = glb_DecompressDirH(Info.dir);
	SetPos(vPos);
	SetDirAndUp(m_vServerDir, g_vAxisY);

	StartWork(WT_NOTHING, WORK_STAND);

	return true;
}

//	Release object
void CECNPCServer::Release()
{
	int i;

	//	Release skill list
	for (i=0; i < m_aSkills.GetSize(); i++)
		delete m_aSkills[i];
	
	m_aSkills.RemoveAll();

	CECNPC::Release();
}

//	Build skill list
bool CECNPCServer::BuildSkillList()
{
	/*
	ASSERT(m_pDBEssence);

	if (!m_pDBEssence->id_skill_service || m_aSkills.GetSize())
		return true;

	DATA_TYPE DataType;
	NPC_SKILL_SERVICE* pSevData = (NPC_SKILL_SERVICE*)g_pGame->GetElementDataMan()->get_data_ptr(m_pDBEssence->id_skill_service, ID_SPACE_ESSENCE, DataType);
	if (!pSevData)
		return true;

	for (int i=0; i < 128; i++)
	{
		if (!pSevData->id_skills[i])
			break;

		CECSkill* pSkill = new CECSkill(pSevData->id_skills[i], 1);
		m_aSkills.Add(pSkill);
	}
	*/

	return true;
}

//	Set loaded model to NPC object, this function is used in multithread loading process
bool CECNPCServer::SetNPCLoadedResult(const EC_NPCLOADRESULT& Ret)
{
	CECNPC::SetNPCLoadedResult(Ret);
	m_bLoadStatueModelFlag = false;
	return true;
}

//	Tick routine
bool CECNPCServer::Tick(DWORD dwDeltaTime)
{
	CECNPC::Tick(dwDeltaTime);

	//	Update quest icon
	UpdateCurQuestIcon();

	if (m_bCityStatue)
		UpdateStatueModel();

	return true;
}

//	Render routine
bool CECNPCServer::Render(CECViewport* pViewport, int iRenderFlag/* 0 */)
{
	CECNPC::Render(pViewport, iRenderFlag);

	if (iRenderFlag == RD_NORMAL)
	{
		//	Render quest icon on head
		if (m_bQuestIcon && m_pNPCModel)
			RenderQuestIcon(pViewport);
	}

	return true;
}

bool CECNPCServer::RenderHighLight(CECViewport* pViewport)
{
	CECNPC::RenderHighLight(pViewport);

	//	Render quest icon on head
	if (m_bQuestIcon && m_pNPCModel)
		RenderQuestIcon(pViewport);

	return true;
}

//	Render quest icon on header
bool CECNPCServer::RenderQuestIcon(CECViewport* pViewport)
{
	if (IsHide() || m_iQuestIcon < 0)
		return true;

	if (!FillPateContent(pViewport))
		return true;

	int iIcon;
	DWORD dwCol;

	switch (m_iQuestIcon)
	{
	case QI_OUT:	iIcon = 0;	dwCol = A3DCOLORRGB(255, 255, 0);	break;
	case QI_OUT_N:	iIcon = 2;	dwCol = A3DCOLORRGB(200, 200, 200);	break;
	case QI_IN:		iIcon = 1;	dwCol = A3DCOLORRGB(255, 255, 0);	break;
	case QI_IN_N:	iIcon = 3;	dwCol = A3DCOLORRGB(200, 200, 200);	break;
	case QI_OUT_K:	iIcon = 4;	dwCol = A3DCOLORRGB(255, 128, 0);	break;
	case QI_IN_K:	iIcon = 5;	dwCol = A3DCOLORRGB(255, 128, 0);	break;
	}

	dwCol = A3DCOLORRGB(255, 255, 255);	//	Force to use image color

	CECImageRes* pImageRes = g_pGame->GetImageRes();
	int x, y=m_PateContent.iCurY, cx, cy;
	
	AUIManager* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIManPtr();
	assert(pUIMan);
	float fScale = pUIMan->GetWindowScale();

	pImageRes->GetImageItemSize(CECImageRes::IMG_PATEQUEST, iIcon, &cx, &cy);
	cx = int(cx * fScale);
	cy = int(cy * fScale);
	x = m_PateContent.iBaseX - (cx >> 1);
	y -= cy + 2;
	pImageRes->DrawImage(CECImageRes::IMG_PATEQUEST, x, y, dwCol, iIcon, m_PateContent.z, m_PateContent.rhw, cx, cy);

	m_PateContent.iCurY = y;

	return true;
}

/*	Update current qeust icon index

	Return value:

		0: Has task can be checked out
		1: Has task can't be checked out
		2: Has task which is completed
		3: Has task which isn't completed
		4: Has key task can be checked out
		5: Has key task can be checked in
*/
void CECNPCServer::UpdateCurQuestIcon()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (!pHost || !pHost->GetTaskInterface())
		return;

	ATaskTemplMan* pTempMan = g_pGame->GetTaskTemplateMan();
	elementdataman* pDataMan = g_pGame->GetElementDataMan();
	CECTaskInterface* pTask = pHost->GetTaskInterface();

	enum
	{
		TASK_COMPLETE	= 0x01,
		TASK_INCOMPLETE	= 0x02,
		TASK_CANGET		= 0x04,
		TASK_CANNOTGET	= 0x08,
		TASK_COMPLETE_K	= 0x10,		//	Key task
		TASK_CANGET_K	= 0x20,		//	Key task
	};

	m_iQuestIcon = QI_NONE;
	
	if( m_pDBEssence->domain_related == 1 )
	{
		if( g_pGame->GetGameRun()->GetHostPlayer()->GetFRoleID() == GNET::_R_UNMEMBER )
			return;
		else
		{
			CECNPCMan *pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();
			CECNPC *pNPC = pNPCMan->GetNPC(g_pGame->GetGameRun()->GetHostPlayer()->GetCurServiceNPC());
			if( pNPC )
			{
				A3DVECTOR3 pos = pNPC->GetServerPos();
				CECWorld *pWorld = g_pGame->GetGameRun()->GetWorld();
				CELPrecinct* pPrecinct = pWorld->GetPrecinctSet()->IsPointIn(
					pos.x, pos.z, pWorld->GetInstanceID());
				if( pPrecinct )
				{
					vector<DOMAIN_INFO> *info = globaldata_getdomaininfos();
					int idFaction = -1;
					int idDomain = pPrecinct->GetDomainID();
					DWORD j;
					for(j = 0; j < info->size(); j++ )
					{
						if( (*info)[j].id == idDomain )
						{
							idFaction = (*info)[j].id_owner;
							break;
						}
					}

					if( g_pGame->GetGameRun()->GetHostPlayer()->GetFactionID() != idFaction )
						return;
				}
			}
		}
	}

	DATA_TYPE DataType;
	int i, iFlags = 0;

	//	Check in tasks at first
	if (m_pDBEssence->id_task_in_service)
	{
		const void* pData = pDataMan->get_data_ptr(m_pDBEssence->id_task_in_service, ID_SPACE_ESSENCE, DataType);
		if (!pData || DataType != DT_NPC_TASK_IN_SERVICE)
		{
			ASSERT(pData);
			return;
		}

		NPC_TASK_IN_SERVICE* aInServices = (NPC_TASK_IN_SERVICE*)pData;
		for (i=0; i < SIZE_OF_ARRAY(aInServices->id_tasks); i++)
		{
			int idTask = aInServices->id_tasks[i];
			if (idTask <= 0 || !pTask->HasTask(idTask))
				continue;

			ATaskTempl* pTaskTemp = pTempMan->GetTaskTemplByID(idTask);

			if (pTask->CanFinishTask(idTask))
			{
				if ( pTaskTemp && pTaskTemp->IsKeyTask() )
				{
					m_iQuestIcon = QI_IN_K;
					return;
				}
				else
					iFlags |= TASK_COMPLETE;
			}
			else
			{
				iFlags |= TASK_INCOMPLETE;
			}
		}
	}

	//	Then Check out tasks
	if (m_pDBEssence->id_task_out_service)
	{
		const void* pData = pDataMan->get_data_ptr(m_pDBEssence->id_task_out_service, ID_SPACE_ESSENCE, DataType);
		if (!pData || DataType != DT_NPC_TASK_OUT_SERVICE)
		{
			ASSERT(pData);
			return;
		}

		NPC_TASK_OUT_SERVICE* aOutServices = (NPC_TASK_OUT_SERVICE*)pData;
		unsigned int a_idTask[512];
		int task_count;

		if (aOutServices->id_task_set != 0)
		{
			const unsigned short* pStorage = pTask->GetStorageTasks(aOutServices->id_task_set);
			task_count = 0;

			if (pStorage)
			{
				for (int i = 0; i < SIZE_OF_ARRAY(aOutServices->id_tasks); i++)
				{
					int id = aOutServices->id_tasks[i];

					if (id == 0)
						continue;

					for (int j = 0; j < TASK_STORAGE_LEN; j++)
					{
						if (pStorage[j] == id)
						{
							a_idTask[task_count] = id;
							task_count++;
						}
					}
				}
			}
		}
		else
		{
			memcpy(a_idTask, aOutServices->id_tasks, sizeof(aOutServices->id_tasks));
			task_count = SIZE_OF_ARRAY(aOutServices->id_tasks);
		}

		for (i=0; i < task_count; i++)
		{
			int idTask = a_idTask[i];
			if (idTask <= 0 || !pTask->CanShowTask(idTask))
				continue;

			ATaskTempl* pTaskTemp = pTempMan->GetTaskTemplByID(idTask);

			if (!pTask->CanDeliverTask(idTask))
			{
				if ( pTaskTemp && pTaskTemp->IsKeyTask() )
				{
					m_iQuestIcon = QI_OUT_K;
					return;
				}
				else
					iFlags |= TASK_CANGET;
			}
			else
				iFlags |= TASK_CANNOTGET;
		}
	}
	//	If there is some completed task, return now
	if (iFlags & TASK_COMPLETE)
		m_iQuestIcon = QI_IN;
	else if (iFlags & TASK_CANGET)
		m_iQuestIcon = QI_OUT;
	else if (iFlags & TASK_INCOMPLETE)
		m_iQuestIcon = QI_IN_N;
	else if (iFlags & TASK_CANNOTGET)
		m_iQuestIcon = QI_OUT_N;
}

//	Get way point ID bound with this NPC
DWORD CECNPCServer::GetWayPointID()
{
	DWORD dwID = 0;
	if (m_pDBEssence->combined_services & 0x08)
		dwID = m_pDBEssence->id_to_discover;

	return dwID;
}

void CECNPCServer::RestorServerDir()
{
	PlayModelAction(ACT_STAND, 1.0f);
	SetDestDirAndUp(m_vServerDir, g_vAxisY, 300);
}

void CECNPCServer::StartGreeting()
{
	PlayModelAction(ACT_NPC_GREETING, 1.0f);
}

//	Get NPC name color
DWORD CECNPCServer::GetNameColor()
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

	return dwNameCol;
}

//	Is monster in invader camp in battle ?
bool CECNPCServer::IsInBattleInvaderCamp()
{ 
	return (m_pMonEssence && (m_pMonEssence->faction & ((1 << 4) | (1 << 6)))) ? true : false; 
}

//	Is monster in defender camp in battle ?
bool CECNPCServer::IsInBattleDefenderCamp()
{
	return (m_pMonEssence && (m_pMonEssence->faction & ((1 << 5) | (1 << 7)))) ? true : false; 
}

bool CECNPCServer::IsAttackable()
{
	return m_pDBEssence->attack_rule != 0;
}

bool CECNPCServer::IsHumanCamp()
{
	return (m_pMonEssence && (m_pMonEssence->faction & (1<<16) )) ? true : false;
}

bool CECNPCServer::IsOrcCamp()
{
	return (m_pMonEssence && (m_pMonEssence->faction & (1<<17) )) ? true : false;
}

void CECNPCServer::UpdateStatueModel()
{
	if (m_bLoadStatueModelFlag || g_pGame->GetGameRun()->GetMasterFlag())
		return;

	if (!m_bStatueChanged)
	{
		int master = g_pGame->GetGameRun()->GetCityOwnerMaster();

		if (master == 0)
			return;

		if (master != m_nCityMasterID)
		{
			m_nCityMasterID = master;
			m_nStatueIndex = _DecideStatueModelIndex(g_pGame->GetGameRun()->GetCityOccupyTime(), g_pGame->GetServerGMTTime());
			m_bStatueChanged = true;
		}
		else
		{
			int statue_index = _DecideStatueModelIndex(g_pGame->GetGameRun()->GetCityOccupyTime(), g_pGame->GetServerGMTTime());

			if (statue_index != m_nStatueIndex)
			{
				m_nStatueIndex = statue_index;
				m_bStatueChanged = true;
			}
		}
	}

	if (m_bStatueChanged)
	{
		const ACHAR* name = g_pGame->GetGameRun()->GetPlayerName(m_nCityMasterID, false);

		if (name)
		{
			m_strName = name;
			wcscpy(m_pDBEssence->name, name);

			if (m_pPateName)
				m_pPateName->SetText(m_strName, false);

			QueueECModelForLoad(MTL_ECM_NPC, m_NPCInfo.nid, GetBornStamp(), GetServerPos(), m_bMaleStatue ? _male_statue_path[m_nStatueIndex] : _female_statue_path[m_nStatueIndex], NULL, m_NPCInfo.tid);
			m_bLoadStatueModelFlag = true;
			m_bStatueChanged = false;
			m_bGetStatueNameFlag = false;
		}
		else if (!m_bGetStatueNameFlag)
		{
			g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &m_nCityMasterID, 2);
			m_bGetStatueNameFlag = true;
		}
	}
}
