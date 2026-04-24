/*
 * FILE: EC_LoginPlayer.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/28
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_LoginPlayer.h"
#include "EC_IvtrTypes.h"
#include "EC_Model.h"
#include "EC_Resource.h"
#include "EC_ShadowRender.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrEquip.h"
#include "A3DSkinMan.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Sprite.h"
#include <A3DSkin.h>
#include <A3DMeshBase.h>
#include "elementdataman.h"
#include "EC_IvtrConsume.h"
#include "roleinfo"

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


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECLoginPlayer
//	
///////////////////////////////////////////////////////////////////////////

CECLoginPlayer::CECLoginPlayer(CECPlayerMan* pPlayerMan) : CECPlayer(pPlayerMan)
{
	m_iCID			= OCID_LOGINPLAYER;
	m_bCastShadow	= true;
	m_bTrusteeMode	= false;

	memset(m_aEquipItems, 0, sizeof (m_aEquipItems));
}

CECLoginPlayer::~CECLoginPlayer()
{
}

void CECLoginPlayer::Release()
{
	//	Release equipment items
	for (int i=0; i < IVTRSIZE_EQUIPPACK; i++)
	{
		if (m_aEquipItems[i])
		{
			delete m_aEquipItems[i];
			m_aEquipItems[i] = NULL;
		}
	}

	CECPlayer::Release();
}

//	Load player data
bool CECLoginPlayer::Load(GNET::RoleInfo& Info)
{
	using namespace GNET;
	
	m_PlayerInfo.cid		= Info.roleid;
	m_BasicProps.iLevel		= Info.level;

	m_iFaceid		= Info.faceid;
	m_iHairid		= Info.hairid;
	m_iEarid		= Info.earid;	// ROLEROLEROLE
	m_iTailid		= Info.tailid;	// ROLEROLEROLE
	m_iDefaultSkin	= Info.fashionid;
	m_iGender		= Info.gender;
//	m_iRace			= g_pGame->GetGameInit().iRaceTest;
	m_iRace			= CECPlayer::GetRaceByProf(Info.occupation);
	m_iPhysique		= CECPlayer::GetPhysiqueByProf(Info.occupation);
	m_BasicProps.iProfession = Info.occupation;		// this is used to display the profession of player
											// but when enter game, this data will be sent from gs.
	
	//	Get player name
	m_strName = ACString((const ACHAR*)Info.name.begin(), Info.name.size() / sizeof (ACHAR));

	int i;

	//	Create equipments
	for (i=0; i < (int)Info.equipment.size(); i++)
	{
		const GRoleInventory& Equip = Info.equipment[i];
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(Equip.id, Equip.expire_date, Equip.count);
		if (!pItem)
		{
			a_LogOutput(1, "CECLoginPlayer::Load, Failed to create equipment.");
			continue;
		}

		if( Equip.data.begin() && Equip.data.size() )
			pItem->SetItemInfo((BYTE*)Equip.data.begin(), Equip.data.size());
		pItem->UpdateInfoMD5();
		m_aEquipItems[Equip.pos] = pItem;
	}

	//	Build new equipments id array
	int aNewEquips[IVTRSIZE_EQUIPPACK];

	for (i=0; i < IVTRSIZE_EQUIPPACK; i++)
	{
		CECIvtrItem* pItem = m_aEquipItems[i];
		if (pItem)
		{
			aNewEquips[i] = pItem->GetTemplateID();
			if( i >= EQUIPIVTR_WEAPON && i <= EQUIPIVTR_FOOT )
			{
				CECIvtrEquip * pEquip = dynamic_cast<CECIvtrEquip *>(pItem);
				if (pEquip)
				{
					WORD num_estone = pEquip->GetEssence().num_estone;
					aNewEquips[i] |= (num_estone << 16) & 0x7fffffff;
				}
			}
			if( i >= EQUIPIVTR_FASHION_EYE && i <= EQUIPIVTR_FASHION_BACK 
				|| i == EQUIPIVTR_FASHION_WEAPON)
			{
				CECIvtrEquip * pEquip = dynamic_cast<CECIvtrEquip *>(pItem);
				if(pEquip && pEquip->GetNewEssenceForAstrology().reserved1 != 0)
				{
					CECIvtrIndividualityItem* pItem = 	(CECIvtrIndividualityItem*)CECIvtrItem::CreateItem(pEquip->GetNewEssenceForAstrology().reserved1, 0, 0);
					if(pItem && pItem->GetClassID() == CECIvtrItem::ICID_INDIVIDUALITY_ITEM)
					{
						WORD nColorId = pItem->GetDBEssence()->index;
						aNewEquips[i] |= (nColorId << 16) & 0x7fffffff;
						delete pItem;
					}
				}
			}
		}
		else
			aNewEquips[i] = 0;
	}

	//	Parse modes
	int* aModes = (int*)Info.charactermode.begin();
	int iNumMode = Info.charactermode.size() / 8;
	if (aModes && iNumMode)
	{
		for (i=0; i < iNumMode; i++)
		{
			int iKey = *aModes++;
			int iValue = *aModes++;

			switch (iKey)
			{
			case 1:	m_bFashionMode = iValue ? true : false;		break;
			case 2: /*m_RidingPet.id = iValue;*/ break;
			case 3: m_dwHideEquipMask = iValue; break;
			case 4: m_bTrusteeMode = iValue ? true : false; break;
			default:
				break;
			}
		}
	}


	//	Try to load player's skeleton
	if (!LoadPlayerSkeleton(aNewEquips, true))
	{
		a_LogOutput(1, "CECLoginPlayer::Load, Failed to load skeleton.");
		return false;
	}

	//	Parse custom status
	short* aStatus = (short*)Info.custom_status.begin();
	int iNumStatus = Info.custom_status.size() / sizeof (short);
	if (aStatus && iNumStatus)
	{
		for (i=0; i < iNumStatus; i++)
			ApplyEffect(aStatus[i], true);
	}

	return true;
}

bool CECLoginPlayer::Load(int physique, int profession, int face, int hair, int gender, const int* equips, bool bHideEquip/*true*/)
{
	m_iPhysique	  = physique;
	m_iFaceid = face;
	m_iHairid = hair;
	m_iGender = gender;
	m_BasicProps.iProfession = profession;

	if (!LoadPlayerSkeleton(equips, true,bHideEquip))
	{
		a_LogOutput(1, "CECLoginPlayer::Load, Failed to load skeleton.");
		return false;
	}

	return true;
}

void CECLoginPlayer::UpdateEquipments(const int* pEquipmentID, bool bFashion)
{
	if( !m_pBackupModel || !m_pBackupModel->GetA3DSkinModel())
		return;
	
	UpdateHideEquipMask(pEquipmentID);

	DWORD dwMask = 0; 
	int aTempEquips[SIZE_EQUIPIVTR];
	int* pOld;
	const int* pNew;
	
	int i;
    for( i = 0; i < SIZE_EQUIPIVTR; ++i)
	{
		if(m_dwHideEquipMaskInUse & (1<<i))
			aTempEquips[i] = 0;
		else
			aTempEquips[i] = pEquipmentID[i];
	}

	pOld = m_aEquips;
	pNew = aTempEquips;
	
	for (i = 0; i < SIZE_EQUIPIVTR; i++)
	{
		if (aTempEquips[i] != m_aEquips[i])
		{
			dwMask |= (1 << i);
		}
	}
	
	// 人物信息界面需要随时更新信息，这个函数会被频繁调用，所以加判断
	if (dwMask != 0)
	{
		for ( i = 0; i < SIZE_EQUIPIVTR; i++)
		{
			if (pNew[i] != pOld[i])
			{
				if (pOld[i] > 0)
					m_pBackupModel->OnScriptChangeEquip(pOld[i], 0, m_bFashionMode, GetEquipPathId(pOld[i], m_iGender, m_iPhysique));
				
				if (pNew[i] > 0)
					m_pBackupModel->OnScriptChangeEquip(pNew[i], 1, m_bFashionMode, GetEquipPathId(pNew[i], m_iGender, m_iPhysique));
				
				dwMask |= (1 << i);
				pOld[i] = pNew[i];
			}
		}

		//-------------------------------------------------------------------------
		//如果装备了时装武器，则不显示武器;如果不装备时装武器，重新显示武器。
		if ((dwMask & (1 << EQUIPIVTR_FASHION_WEAPON)) && pNew[EQUIPIVTR_FASHION_WEAPON] == 0)
		{
			dwMask &= ~(1 << EQUIPIVTR_FASHION_WEAPON);
			dwMask |= 1 << EQUIPIVTR_WEAPON;
		}
		else if ((dwMask & (1 << EQUIPIVTR_WEAPON)) && pNew[EQUIPIVTR_FASHION_WEAPON] != 0)
		{
			dwMask &= ~(1 << EQUIPIVTR_WEAPON);
		}

		QueueLoadEquips(m_aEquips, dwMask, true);
	}

	// 切换时装模式
	if ( m_bFashionMode != bFashion )
	{
		m_bFashionMode = bFashion;
		if (m_pBackupModel)
		{
			for (int i = 0; i < SIZE_EQUIPIVTR; i++)
			{
				if (m_aEquips[i] > 0)
					m_pBackupModel->OnScriptChangeEquip(m_aEquips[i], 1, m_bFashionMode, GetEquipPathId(m_aEquips[i], m_iGender, m_iPhysique));
			}
		}
	}
}

void CECLoginPlayer::UpdateEquipmentsSkin( int iDarkLightState )
{
	//判断参数的合法性
	if ( iDarkLightState < 0 || iDarkLightState > 1 )	//目前就是0 和1，策划乱！
	{
		return;
	}

	//更新光明黑暗状态值
	g_nLoginPlayerDarkLightState	=	iDarkLightState;
	
	//设置强制换装掩码
	DWORD dwForceChangeMask = 0;
	dwForceChangeMask |= ( 1 << EQUIPIVTR_HEAD );
	dwForceChangeMask |= ( 1 << EQUIPIVTR_BODY );
	dwForceChangeMask |= ( 1 << EQUIPIVTR_FOOT );
	SetForceChangeMask( dwForceChangeMask );

	//更新装备ID列表
/*	int aNewEquips[IVTRSIZE_EQUIPPACK];	//--------------------
	for ( int i=0; i < IVTRSIZE_EQUIPPACK; ++i )
	{
		CECIvtrItem* pItem = m_aEquipItems[i];
		if (pItem)
		{
			aNewEquips[i] = pItem->GetTemplateID();
			if( i >= EQUIPIVTR_WEAPON && i <= EQUIPIVTR_FOOT )
			{
				CECIvtrEquip * pEquip = (CECIvtrEquip *) pItem;
				WORD num_estone = pEquip->GetEssence().num_estone;
				aNewEquips[i] |= (num_estone << 16) & 0x7fffffff;
			}
		}
		else
			aNewEquips[i] = 0;
	}
*/	//-------------------------------------------------直接使用player中的m_aEquips，不用再计算
	ShowEquipments(m_aEquips, false, true);//立即加载，不要等多线程

	//重新清除全局变量，以免影响游戏内HostPlayer等玩家的换装
	g_nLoginPlayerDarkLightState = -1;
}


void CECLoginPlayer::UpdateLieShan(int iEarid, int iTailid)
{
	if(iEarid != m_iEarid)
	{
		m_iEarid = iEarid;
		ChangeEar(iEarid);
	}
	if(iTailid != m_iTailid)
	{
		m_iTailid = iTailid;
		ChangeTail(iTailid);
	}
}

void CECLoginPlayer::UpdateFace(int iFaceid, int iHairid, int iSkin)
{
	if(iFaceid != m_iFaceid)
	{
		m_iFaceid = iFaceid;
		ChangeFace(iFaceid);
	}
	if(iHairid != m_iHairid)
	{
		m_iHairid = iHairid;
		ChangeHair(iHairid);
	}
	if (iSkin != m_iDefaultSkin)
	{
		ChangeDefaultSkin(iSkin);
	}
}

extern const char* _head_skin[2][2];
bool CECLoginPlayer::ChangeFace(int iFaceid)
{
	if( !m_pPlayerModel )
		return false;

//	static const char* head_skin[] = 
//	{
//		"Models\\Players\\形象\\男\\头\\男头%02d.ski",
//		"Models\\Players\\形象\\女\\头\\女头%02d.ski"
// 	};

	m_iFaceid = iFaceid;
	CheckIndexBound(m_iFaceid);
	char szPath[MAX_PATH];
	sprintf(szPath, _head_skin[m_iPhysique][m_iGender], FaceHairId2FileIndex(m_iFaceid, GetProfession()));
	A3DSkin* pHeadSkin = g_pGame->LoadA3DSkin(szPath, true);

	A3DSkin* pOldSkin = m_pPlayerModel->GetA3DSkinModel()->GetA3DSkin(SKIN_HEAD_INDEX);
	g_pGame->ReleaseA3DSkin(pOldSkin);

	m_pPlayerModel->ReplaceSkin(SKIN_HEAD_INDEX, pHeadSkin, false);
	return true;
}

extern const char* _hair_skin[2][2];
bool CECLoginPlayer::ChangeHair(int iHairid)
{
	if( !m_pPlayerModel )
		return false;

//	const char* hair_skin[] =
//	{
//		"Models\\Players\\形象\\男\\头发\\男头发%02d.ski",
//		"Models\\Players\\形象\\女\\头发\\女头发%02d.ski"
// 	};

	m_iHairid = iHairid;
	CheckIndexBound(m_iHairid);
	char szPath[MAX_PATH];
	sprintf(szPath, _hair_skin[m_iPhysique][m_iGender], FaceHairId2FileIndex(m_iHairid, GetProfession()));
	A3DSkin* pHairSkin = g_pGame->LoadA3DSkin(szPath, true);

	if( pHairSkin )
	{
		for(int i=0; i<pHairSkin->GetSkinMeshNum(); i++)
		{
			A3DSkinMeshRef& Ref = pHairSkin->GetSkinMeshRef(i);
			if (Ref.pMesh->GetTextureIndex() == 0) // if it is use the hair texture, just set to use alpha
			{
				Ref.fTrans = 0.0f;
			}
		}
	}
	A3DSkin* pOldSkin = m_pPlayerModel->GetA3DSkinModel()->GetA3DSkin(SKIN_HAIR_INDEX);
	g_pGame->ReleaseA3DSkin(pOldSkin);

	m_pPlayerModel->ReplaceSkin(SKIN_HAIR_INDEX, pHairSkin, false);
	return true;
}

extern const char* _ear_skin[2];
extern const char* _optimal_ear_skin[2];
bool CECLoginPlayer::ChangeEar(int iEarid)
{
	if( !m_pPlayerModel )
		return false;

	m_iEarid = iEarid;
	char szPath[MAX_PATH];
	int level = m_BasicProps.iLevel/45;

	if (m_iEarid & 0x80)
		sprintf(szPath, _optimal_ear_skin[m_iGender], (m_iEarid & 0x7f) + 1);
	else
		sprintf(szPath, _ear_skin[m_iGender], level+1, m_iEarid+1);

	A3DSkin* pEarSkin = g_pGame->LoadA3DSkin(szPath, true);

	if( pEarSkin )
	{
		for(int i=0; i<pEarSkin->GetSkinMeshNum(); i++)
		{
			A3DSkinMeshRef& Ref = pEarSkin->GetSkinMeshRef(i);
			if (Ref.pMesh->GetTextureIndex() == 0) // if it is use the hair texture, just set to use alpha
			{
				Ref.fTrans = 0.0f;
			}
		}
	}
	A3DSkin* pOldSkin = m_pPlayerModel->GetA3DSkinModel()->GetA3DSkin(SKIN_EAR_INDEX);
	g_pGame->ReleaseA3DSkin(pOldSkin);

	m_pPlayerModel->ReplaceSkin(SKIN_EAR_INDEX, pEarSkin, false);
	return true;
}

extern const char* _tail_skin[2];
extern const char* _optimal_tail_skin[2];
bool CECLoginPlayer::ChangeTail(int iTailid)
{
	if( !m_pPlayerModel )
		return false;

	m_iTailid = iTailid;
	char szPath[MAX_PATH];
	int level = m_BasicProps.iLevel/45;

	if (m_iTailid & 0x80)
		sprintf(szPath, _optimal_tail_skin[m_iGender], (m_iTailid & 0x7f) + 1);
	else
		sprintf(szPath, _tail_skin[m_iGender], level+1, m_iTailid+1);

	A3DSkin* pTailSkin = g_pGame->LoadA3DSkin(szPath, true);

	if( pTailSkin )
	{
		for(int i=0; i<pTailSkin->GetSkinMeshNum(); i++)
		{
			A3DSkinMeshRef& Ref = pTailSkin->GetSkinMeshRef(i);
			if (Ref.pMesh->GetTextureIndex() == 0) // if it is use the hair texture, just set to use alpha
			{
				Ref.fTrans = 0.0f;
			}
		}
	}
	A3DSkin* pOldSkin = m_pPlayerModel->GetA3DSkinModel()->GetA3DSkin(SKIN_TAIL_INDEX);
	g_pGame->ReleaseA3DSkin(pOldSkin);

	m_pPlayerModel->ReplaceSkin(SKIN_TAIL_INDEX, pTailSkin, false);
	return true;
}

bool CECLoginPlayer::ChangeDefaultSkin(int iSkin)
{
	if ( m_iDefaultSkin!=iSkin )
	{
		m_iDefaultSkin = iSkin;
		DWORD dwMask = 0; 
		dwMask |= (1 << EQUIPIVTR_HEAD);
		dwMask |= (1 << EQUIPIVTR_BODY);
		dwMask |= (1 << EQUIPIVTR_FOOT);
		dwMask |= (1 << EQUIPIVTR_FASHION_HEAD);
		dwMask |= (1 << EQUIPIVTR_FASHION_BODY);
		dwMask |= (1 << EQUIPIVTR_FASHION_LEG);
		dwMask |= (1 << EQUIPIVTR_FASHION_FOOT);
		
		QueueLoadEquips(m_aEquips, dwMask, true);
	}

	return true;
}

//	Tick routine
bool CECLoginPlayer::Tick(DWORD dwDeltaTime)
{
	CECPlayer::Tick(dwDeltaTime);

	CECModel* pModel = m_pPetModel ? m_pPetModel : m_pPlayerModel;

	if (pModel)
		pModel->Tick(dwDeltaTime);

	return true;
}

//	Render routine
bool CECLoginPlayer::Render(CECViewport* pViewport, int iRenderFlag/* 0 */)
{
	m_PateContent.iVisible = 0;
	
	if( !UpdateCurSkins() )
		return true;

	if (iRenderFlag == RD_REFLECT)
		return RenderForReflect(pViewport);

	CECModel* pModel = m_pPetModel ? m_pPetModel : m_pPlayerModel;

	if (pModel)
	{

		if( iRenderFlag == RD_BLOOM )
		{
			RenderEquipBloom(pViewport);
			return true;
		}
		else
			pModel->Render(pViewport->GetA3DViewport());
		
		/*
		if (m_bCastShadow && g_pGame->GetShadowRender())
		{
			CECPlayer * pPlayer = this;
			g_pGame->GetShadowRender()->AddShadower(m_aabb.Center, m_aabb, SHADOW_RECEIVER_ALL, PlayerRenderForShadow, pPlayer);
		}*/
	}

	if( m_pSprite )
		m_pSprite->Render(pViewport);

	return true;
}

//	Render for refelction
bool CECLoginPlayer::RenderForReflect(CECViewport* pViewport)
{
	return true;
}

void CECLoginPlayer::Stand(bool restart)
{
	PlayAction(CECPlayer::ACT_STAND, 1.0f, restart);
}

void CECLoginPlayer::Fly(bool restart)
{
	SetMoveEnv(MOVEENV_AIR);
	PlayAction(GetMoveStandAction(false), 1.0f, restart);
}

void CECLoginPlayer::StandForCustomize()
{
//	PlayAction(CECPlayer::ACT_CUSTOMIZE, 1.0f, false);
	if(m_pPlayerModel)
		m_pPlayerModel->GetA3DSkinModel()->PlayActionByName("站立_通用", 0);
/*	
	if (m_pTransfiguredModel)
		m_pTransfiguredModel->StopChannelAction(ACT_CHANNEL_BODY, true);
	else if (m_pChangedModel)
		m_pChangedModel->StopChannelAction(ACT_CHANNEL_BODY, true);
	else if (m_pPetModel && m_pPlayerModel)
	{	
		m_pPetModel->StopChannelAction(ACT_CHANNEL_BODY, true);
		m_pPlayerModel->StopChannelAction(ACT_CHANNEL_BODY, true);
	}
	else if (m_pPlayerModel)
		m_pPlayerModel->StopChannelAction(ACT_CHANNEL_BODY, true);
*/}

void CECLoginPlayer::Run()
{
	PlayAction(CECPlayer::ACT_RUN, 1.0f);
}

void CECLoginPlayer::StandUp()
{
	if (m_iRace == RACE_HUMAN)
		PlayAction(CECPlayer::ACT_STANDUP, 1.0f);
	else
		PlayAction(CECPlayer::ACT_LOGIN_STANDUP, 1.0f);
}

void CECLoginPlayer::SitDown()
{
	if (m_iRace == RACE_HUMAN)
		PlayAction(CECPlayer::ACT_SITDOWN, 1.0f);
	else
		PlayAction(CECPlayer::ACT_LOGIN_SITDOWN, 1.0f);
}

void CECLoginPlayer::ShowPose()
{
	PlayAction(CECPlayer::ACT_LOGIN_POSE, 1.0f);
}

//	Search the full suite
int CECLoginPlayer::SearchFullSuite()
{
	DATA_TYPE DataType = DT_INVALID;
	elementdataman* pDataMan = g_pGame->GetElementDataMan();
	CECGame::SuiteEquipTable& SuiteMapTab = g_pGame->GetSuiteEquipTable();
	
	int i;
	typedef abase::hashtab<int, int, abase::_hash_function> SuiteTable;
	SuiteTable SuiteTab(32);

	//	Check how many suits of equipment host have dressed
	for (i=0; i < SIZE_EQUIPIVTR; i++)
	{
		int idEquip = m_aEquips[i] & 0xffff;
		if (!idEquip)
			continue;
		
		CECGame::SuiteEquipTable::pair_type pair = SuiteMapTab.get(idEquip);
		if (!pair.second)
			continue;

		int idSuite = *pair.first;

		const void* pData = pDataMan->get_data_ptr(idSuite, ID_SPACE_ESSENCE, DataType);
		if (DataType != DT_SUITE_ESSENCE)
			continue;

		const SUITE_ESSENCE* pSuiteEss = (const SUITE_ESSENCE*)pData;
		if (pSuiteEss->file_gfx[0])
			SuiteTab.put(idSuite, pSuiteEss->max_equips);
	}

	if (!SuiteTab.size())
		return 0;	//	No suite was found

	//	Check whether suite is full
	SuiteTable::iterator it = SuiteTab.begin();
	for (; it != SuiteTab.end(); ++it)
	{
		int idSuite = *it.key();
		int iMaxNum = *it.value();

		if (iMaxNum == GetEquippedSuiteItem(idSuite))
			return idSuite;
	}

	return 0;
}

//	Get number of equipped items of specified suite
int CECLoginPlayer::GetEquippedSuiteItem(int idSuite, int* aItems/* NULL */)
{
	CECGame::SuiteEquipTable& SuiteTab = g_pGame->GetSuiteEquipTable();

	int i, iItemCnt = 0;

	for (i=0; i < SIZE_EQUIPIVTR; i++)
	{
		int idEquip = m_aEquips[i] & 0xffff;
		if (!idEquip)
			continue;

		CECGame::SuiteEquipTable::pair_type pair = SuiteTab.get(idEquip);
		if (!pair.second || idSuite != *pair.first)
			continue;

		if (aItems)
			aItems[iItemCnt] = idEquip;
		
		iItemCnt++;
	}

	return iItemCnt;
}

void CECLoginPlayer::OnWeaponChanged()
{
	// replay current action to yield weapon change result
	if(m_pPlayerModel)
		PlayAction(m_pPlayerModel->GetCurActionUserData(ACT_CHANNEL_BODY), 1.0f, true);
}

void CECLoginPlayer::SyncBasicProps(ROLEBASICPROP& roleProp)
{
	m_BasicProps = roleProp;
}

void CECLoginPlayer::RideOnPet(int id, int iLev,int type)
{
	CECPlayer::RideOnPet(id, iLev, type);
	const char* szPetPath = GetRidingPetFileName(id, iLev);
	m_CandPet.id = 0;
	m_CandPet.iLevel = 0;
	QueueLoadPetModel(szPetPath, true);
}