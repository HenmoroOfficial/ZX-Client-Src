#include "A3DMacros.h"
#include "EC_Global.h"
#include "gnet.h"
#include "EC_NetPrtclPlayerInfo.h"
#include "EC_GameSession.h"
#include "EC_GPDataType.h"
#include "EC_ElsePlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_HostPlayer.h"
#include "EC_Skill.h"
#include "EC_FixedMsg.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_IvtrConsume.h"
#include "DlgAutoLock.h"
#include "DlgLottery.h"
#include "DlgHost.h"
#include "DlgCityBuilding.h"
#include "DlgMiniMap.h"
#include "DlgPetDetail.h"
#include "DlgSiege.h"
#include "DlgExit.h"
#include "DlgQShop.h"
#include "DlgQShopItem.h"
#include "DlgCharacter.h"
#include "DlgInventoryCostume.h"
#include "DlgInventoryRide.h"
#include "DlgFaceOff.h"
#include "DlgPKMsg.h"
#include "DlgBattleFactionWarControl.h"
#include "DlgDungeonAncient.h"
#include "DlgBattleDungeonSsk.h"
#include "DlgDungeonStep.h"
#include "ConfigFromLua.h"
#include "EC_NPC.h"
#include "EC_ManNPC.h"
#include "EC_Achievement.h"
#include "EC_BattleDungeon.h"

#include "EC_Utility.h"
#include "AFileImage.h"
#include "EC_Helper.h"
#include "EC_AutoHelper.h"
#include "LuaEvent.h"

using namespace S2C;

CECNetProtocolBase* CECNetPrtclPlayerInfo::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerInfo)
}

bool CECNetPrtclPlayerInfo::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	g_pGame->GetGameRun()->PostMessage(MSG_PM_PLAYERINFO, MAN_PLAYER, -1, (DWORD)m_pData, m_Cmd, m_nDataSize);
	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerInfo00::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerInfo00)
}

bool CECNetPrtclPlayerInfo00::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(cmd_player_info_00))
	{
		assert(false);
		return true;
	}

	g_pGame->GetGameRun()->PostMessage(MSG_PM_PLAYERINFO, MAN_PLAYER, -1, (DWORD)m_pData, m_Cmd, m_nDataSize);
	return true;
}

CECNetProtocolBase* CECNetPrtclSelfInfo1::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclSelfInfo1)
}

bool CECNetPrtclSelfInfo1::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	pSession->DoOvertimeCheck(false, CECGameSession::OT_ENTERGAME, 0);
	g_pGame->GetGameRun()->PostMessage(MSG_PM_PLAYERINFO, MAN_PLAYER, -1, (DWORD)m_pData, m_Cmd, m_nDataSize);
	return true;
}

CECNetProtocolBase* CECNetPrtclSelfInfo00::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclSelfInfo00)
}

bool CECNetPrtclSelfInfo00::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(cmd_self_info_00))
	{
		assert(false);
		return true;
	}

	g_pGame->GetGameRun()->PostMessage(MSG_HST_INFO00, MAN_PLAYER, 0, (DWORD)m_pData, m_Cmd, m_nDataSize);
	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerList::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerList)
}

bool CECNetPrtclPlayerList::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	g_pGame->GetGameRun()->PostMessage(MSG_PM_PLAYERINFO, MAN_PLAYER, -1, (DWORD)m_pData, m_Cmd, m_nDataSize);
	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerLeaveWorld::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerLeaveWorld)
}

bool CECNetPrtclPlayerLeaveWorld::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(cmd_player_leave_world))
	{
		assert(false);
		return true;
	}

	g_pGame->GetGameRun()->PostMessage(MSG_PM_PLAYEREXIT, MAN_PLAYER, -1, (int)m_pData, m_Cmd);
	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerRecipeData::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerRecipeData)
}

bool CECNetPrtclPlayerRecipeData::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize < sizeof(unsigned short))
	{
		assert(false);
		return true;
	}

	const player_recipe_data* pData = (const player_recipe_data*)m_pData;
	int sz = (pData->count + 1) * sizeof(unsigned short);

	if (m_nDataSize != sz)
	{
		assert(false);
		return true;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
	{
		pHost->ClearRecipeData();

		for (unsigned short i = 0; i < pData->count; i++)
			pHost->AddOneRecipe(pData->buf[i]);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerLearnRecipe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerLearnRecipe)
}

bool CECNetPrtclPlayerLearnRecipe::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_learn_recipe))
	{
		assert(false);
		return true;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
		pHost->AddOneRecipe(((const player_learn_recipe*)m_pData)->recipe_id);

	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerProduceInfo::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerProduceInfo)
}

bool CECNetPrtclPlayerProduceInfo::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_produce_skill_info))
	{
		assert(false);
		return true;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
	{
		const player_produce_skill_info* pInfo = (const player_produce_skill_info*)m_pData;

		if (pHost->GetProduceSkillLevel() >= 0 && pInfo->level != pHost->GetProduceSkillLevel())
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_PROD_LEVELUP, pInfo->level);
		
		pHost->SetProduceSkillLevel(pInfo->level);
		pHost->SetProduceSkillExp(pInfo->exp);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerRefineResult::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerRefineResult)
}

bool CECNetPrtclPlayerRefineResult::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_refine_result))
	{
		assert(false);
		return true;
	}
	
	player_refine_result* pResult = (player_refine_result*)m_pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->MessageBox("", pGameUI->GetStringFromTable(pResult->result + 302), 
		MB_OK, A3DCOLORRGB(255, 255, 255));


	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerStartOprt::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerStartOprt)
}

bool CECNetPrtclPlayerStartOprt::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_start_general_operation))
	{
		assert(false);
		return true;
	}

	player_start_general_operation* pOprt = (player_start_general_operation*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(pOprt->player_id);

	if (pPlayer)
		pPlayer->StartGeneralOprt(pOprt->op_id, pOprt->duration);

	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerStopOprt::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerStopOprt)
}

bool CECNetPrtclPlayerStopOprt::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_stop_general_operation))
	{
		assert(false);
		return true;
	}

	player_stop_general_operation* pOprt = (player_stop_general_operation*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(pOprt->player_id);

	if (pPlayer)
		pPlayer->StopGeneralOprt(pOprt->op_id);

	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerPKLevel::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerPKLevel)
}

bool CECNetPrtclPlayerPKLevel::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(pk_level_notify))
	{
		assert(false);
		return true;
	}

	pk_level_notify* p = (pk_level_notify*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(p->id);

	if (pPlayer)
	{
		if (pPlayer->IsHostPlayer())
		{
			if (pPlayer->GetBasicProps().iPKLevel < p->pk_level)
				LuaEvent::FirstIncPKValue();
			else if(pPlayer->GetBasicProps().iPKLevel > p->pk_level)
				LuaEvent::FirstDecPKValue();
		}
		pPlayer->GetBasicProps().iPKLevel = p->pk_level;
		CECPlayer* pClone = pMan->GetPlayer(pPlayer->GetCloneID());
		if (pClone)
			pClone->GetBasicProps().iPKLevel = p->pk_level;

	}
	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerPKValue::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerPKValue)
}

bool CECNetPrtclPlayerPKValue::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(pk_value_notify))
	{
		assert(false);
		return true;
	}

	pk_value_notify* p = (pk_value_notify*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
	{
		pHost->SetPKValue(p->value);
	}
	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerClass::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerClass)
}

bool CECNetPrtclPlayerClass::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_change_class))
	{
		assert(false);
		return true;
	}

	player_change_class* p = (player_change_class*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(p->id);

	if (!pPlayer)
		return true;

	int old_cls = pPlayer->GetBasicProps().iProfession;
	pPlayer->GetBasicProps().iProfession = p->new_cls;
	CECPlayer::SECT_LEVEL2_INFO* pInfo = CECPlayer::GetSectInfoByProf(old_cls);
	CECPlayer::SECT_LEVEL2_INFO* pInfo2 = CECPlayer::GetSectInfoByProf(p->new_cls);
	bool bChange = false;
	ASSERT(pInfo && pInfo2);
	if (!pInfo || !pInfo2)
		return true;
	// 同一个体型也需要进行模型的重新加载的情况，怀光和烈山，以及太昊与九黎 Modified 2011-10-20
	bChange = (	(pInfo->mask_sect & (1<<SECT_11)) && (pInfo2->mask_sect & (1<<SECT_13))) || 
				((pInfo->mask_sect & (1<<SECT_13)) && (pInfo2->mask_sect & (1<<SECT_11)) ||
				((pInfo->mask_sect & (1<<SECT_10)) && (pInfo2->mask_sect & (1<<SECT_12))) ||
				((pInfo->mask_sect & (1<<SECT_12)) && (pInfo2->mask_sect & (1<<SECT_10))) ||
				((pInfo->mask_sect & (1<<SECT_14)) && (pInfo2->mask_sect & (1<<SECT_15))) ||	//新增轩辕和天华 中体型
				((pInfo->mask_sect & (1<<SECT_15)) && (pInfo2->mask_sect & (1<<SECT_14))));		//新增轩辕和天华 中体型
		
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( pHost == pPlayer)
	{
		LuaEvent::FirstJoinSect(p->new_cls);
		if (CECPlayer::GetPhysiqueByProf(old_cls) != CECPlayer::GetPhysiqueByProf(p->new_cls) || bChange)
		{
			g_pGame->GetGameRun()->UpdateSelectedRoleRace(p->new_cls);
			pPlayer->UpdateRace();
			pPlayer->ReloadModel();
			pHost->UpdateEquipSkins(true);
			CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			if (pGameUI)
			{
				CDlgCharacter* pDlgCharacter = (CDlgCharacter*)pGameUI->GetDialog("Win_Character");
				if (pDlgCharacter)
					pDlgCharacter->ReLoadModel();
				CDlgInventoryCostume * pDlgCostume = (CDlgInventoryCostume*)pGameUI->GetDialog("Win_Inventory_Costume");
				if (pDlgCostume)
					pDlgCostume->ReLoadModel();
				CDlgQShop* pDlgQShop = (CDlgQShop*)pGameUI->GetDialog("Win_QShop");
				if (pDlgQShop)
					pDlgQShop->ReLoadModel();
				CDlgQShop* pDlgQShopPop = (CDlgQShop*)pGameUI->GetDialog("Win_QShopPop");
				if (pDlgQShopPop)
					pDlgQShopPop->ReLoadModel();
				CDlgFaceOff* pDlgFaceOff = (CDlgFaceOff*)pGameUI->GetDialog("Win_ImageChange");
				if (pDlgFaceOff)
					pDlgFaceOff->ReLoadModel();
				CDlgInventoryRide* pDlgInvRide = (CDlgInventoryRide*)pGameUI->GetDialog("Win_Inventory_Vechile");
				if (pDlgInvRide)
					pDlgInvRide->ReLoadModel();
				pGameUI->GetDialog("Win_CharHead_Prof11")->Show(p->new_cls>=56 && p->new_cls<=60);
				pGameUI->GetDialog("Win_CharHead_Prof11")->Show(p->new_cls >= 102 && p->new_cls <= 106);
			}
		}
	}
	else    // ElsePlayer 
	{
		if (CECPlayer::GetPhysiqueByProf(old_cls) != CECPlayer::GetPhysiqueByProf(p->new_cls) || bChange)
		{
			CECElsePlayer* pElsePlayer = (CECElsePlayer*)pPlayer;
			pElsePlayer->UpdateRace();
			pElsePlayer->ReloadModel();
			pElsePlayer->LoadPlayerEquipments();
		}
	}
	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerCash::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerCash)
}

bool CECNetPrtclPlayerCash::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_cash))
	{
		assert(false);
		return true;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	player_cash* p = (player_cash*)m_pData;

	if (pHost)
	{
		pHost->SetCash(p->cash_amount);

		if (p->used_cash >= 0)
			pHost->SetCashConsumed(p->used_cash);
		if (p->cash_add >= 0)
			pHost->SetCashAdd(p->cash_add);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerSkillAddon::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerSkillAddon)
}

bool CECNetPrtclPlayerSkillAddon::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_skill_addon))
	{
		assert(false);
		return true;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	player_skill_addon* p = (player_skill_addon*)m_pData;

	if (pHost)
	{
		CECSkill* pSkill = pHost->GetSkill(p->skill);

		if (!pSkill)
			pSkill = pHost->InsertAddonSkill(p->skill, 0);

		//添加一个map结构存储一些SkillWrapper需要的技能信息Added 2011-05-05.
		//每次换法宝或者其他装备时，技能也随之改变。无效的技能的level为0.
		GNET::ElementSkill::SetExtraSkillLevel( p->skill, p->level );
//		GNET::ElementSkill::SetLevel( p->skill, p->level );//SetLevel接口调用了SkillWrapper::SetLevel，而该函数修改了SkillWrapper的静态Map中存放的职业技能的level
														   //而ExtraSkill 和 AddonSkill是武器和法宝等装备后产生的技能，这个技能的level不能直接影响职业技能的level。Modified 2011-09-22.
		//Added end.

		pSkill->SetAddonLevel(p->level);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerSkillCommonAddon::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerSkillCommonAddon)
}

bool CECNetPrtclPlayerSkillCommonAddon::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_skill_common_addon))
	{
		assert(false);
		return true;
	}

	int nNum;
	CECSkill* pSkill;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	player_skill_common_addon* p = (player_skill_common_addon*)m_pData;

	if (pHost)
	{
		nNum = pHost->GetPositiveSkillNum();

		int i;
		for (i = 0; i < nNum; i++)
		{
			pSkill = pHost->GetPositiveSkillByIndex(i);
			pSkill->SetCommonLevel(p->addon);
		}

		nNum = pHost->GetPassiveSkillNum();

		for (i = 0; i < nNum; i++)
		{
			pSkill = pHost->GetPassiveSkillByIndex(i);
			pSkill->SetCommonLevel(p->addon);
		}
	}

	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerExtraSkill::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerExtraSkill)
}

bool CECNetPrtclPlayerExtraSkill::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_extra_skill))
	{
		assert(false);
		return true;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	player_extra_skill* p = (player_extra_skill*)m_pData;

	if (pHost)
	{
		CECSkill* pSkill = pHost->GetSkill(p->skill);

		if (!pSkill)
			pSkill = pHost->InsertAddonSkill(p->skill, p->level);

		//添加一个map结构存储一些SkillWrapper需要的技能信息Added 2011-05-05.
		//每次换法宝或者其他装备时，技能也随之改变。无效的技能的level为0.
		GNET::ElementSkill::SetExtraSkillLevel( p->skill, p->level );
//		GNET::ElementSkill::SetLevel( p->skill, p->level );//SetLevel接口调用了SkillWrapper::SetLevel，而该函数修改了SkillWrapper的静态Map中存放的职业技能的level
														   //而ExtraSkill 和 AddonSkill是武器和法宝等装备后产生的技能，这个技能的level不能直接影响职业技能的level。Modified 2011-09-22.
		//Added end.

		pSkill->SetLevel(p->level);
		pSkill->SetExtra(true);
		if (p->level>0 && pSkill->IsCategory(GNET::CATEGORY_TRIGGERED))
		{
			CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_Skill_Activate", "active", 0, (int)pSkill);
		}
	}

	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerTitleList::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerTitleList)
}

bool CECNetPrtclPlayerTitleList::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	player_title_list* p = (player_title_list*)m_pData;
	size_t sz;

	if (!p->CheckValid(m_nDataSize, sz))
	{
		assert(false);
		return true;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
	{
		pHost->RemoveAllTitles();

		for (size_t i = 0; i < p->count; i++)
			pHost->AddOneTitle(p->title[i]);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerAddTitle::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerAddTitle)
}

bool CECNetPrtclPlayerAddTitle::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_add_title))
	{
		assert(false);
		return true;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
	{
		int title = ((player_add_title*)m_pData)->title;

		if (!pHost->HasTitle(title))
		{
			pHost->AddOneTitle(title);

			if( title > 1000 )
			{
				// 普通称谓，从策划的表里取名字
				CConfigFromLua * pConfigFromLua = g_pGame->GetConfigFromLua();
				if( pConfigFromLua )
				{
					CConfigFromLua::TitleDefTab::iterator it = pConfigFromLua->m_TitleDef.find(title);
					if( it != pConfigFromLua->m_TitleDef.end() )
					{
						ACString szTitle = *it.value();
						pHost->FormatTitle(szTitle);
						
						// see if it begins with a color definition
						if( szTitle.GetLength() >= 7 )
						{
							if( szTitle[0] == _AL('^') )
							{
								szTitle = szTitle.Right(szTitle.GetLength() - 7);
							}
						}
						// 过滤图形称号中的图片路径
						if(szTitle.Find(_AL("#image")) >=0 || szTitle.Find(_AL("&image")) >=0 || szTitle.Find(_AL("#anim")) >=0 || szTitle.Find(_AL("&anim")) >=0)
						{
							int i = 0;
							while(szTitle[i] != _AL('#') && szTitle[i] != _AL('&')) i++;
							szTitle = szTitle.Left(i);
						}
						g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GOTTITLE, (const wchar_t *)szTitle);
					}
				}
			}
		}
	}

	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerDelTitle::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerDelTitle)
}

bool CECNetPrtclPlayerDelTitle::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_del_title))
	{
		assert(false);
		return true;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
		pHost->DelOneTitle(((player_del_title*)m_pData)->title);

	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerRegionReputation::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerRegionReputation)
}

bool CECNetPrtclPlayerRegionReputation::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	player_region_reputation* p = (player_region_reputation*)m_pData;
	size_t sz;

	if (!p->CheckValid(m_nDataSize, sz))
	{
		assert(false);
		return true;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
		pHost->AddRegionReputations(p->count, p->reputation);

	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerChangeRegionReputation::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerChangeRegionReputation)
}

bool CECNetPrtclPlayerChangeRegionReputation::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_change_region_reputation))
	{
		assert(false);
		return true;
	}

	player_change_region_reputation* p = (player_change_region_reputation*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost && p->index >= 0 && p->index < pHost->GetRegionReputationCount())
	{
		pHost->UpdateRegionReputation(p->index, p->new_reputation);
		LuaEvent::FirstIncReputation();
	}	
	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerChangeTitle::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerChangeTitle)
}

bool CECNetPrtclPlayerChangeTitle::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_change_title))
	{
		assert(false);
		return true;
	}

	player_change_title* p = (player_change_title*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(p->who);

	if (pPlayer)
		pPlayer->SetTitle(p->title);

	return true;
}

CECNetProtocolBase* CECNetPrtclPlayerReputation::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclPlayerReputation)
}

bool CECNetPrtclPlayerReputation::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(cmd_player_reputation))
	{
		assert(false);
		return true;
	}

	cmd_player_reputation* p = (cmd_player_reputation*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(p->who);

	if (pPlayer)
	{
		if (pPlayer->IsHostPlayer())
			LuaEvent::FirstObtainErrantry();
		pPlayer->SetReputation(p->reputation);
	}
	return true;
}

CECNetProtocolBase* CECNetPrtclChangeInventorySize::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclChangeInventorySize)
}

bool CECNetPrtclChangeInventorySize::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_change_inventory_size))
	{
		assert(false);
		return true;
	}

	player_change_inventory_size* p = (player_change_inventory_size*)m_pData;
	g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_NEW_INVENTORY_SIZE, p->new_size);

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
	{
		pHost->GetPack()->Resize(p->new_size);
		LuaEvent::FirstExpandPack();
	}
	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerBindSuccess::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerBindSuccess)
}

bool CECNetPrtcPlayerBindSuccess::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_bind_success))
	{
		assert(false);
		return true;
	}

	player_bind_success* p = (player_bind_success*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
	{
		CECIvtrItem* pItem = pHost->GetPack()->GetItem(p->inv_index);

		if (pItem)
			g_pGame->GetGameRun()->AddFixedMessage(p->item_state == ITEM_STATE_BINDED ? FIXMSG_ITEMBIND : FIXMSG_BINDAFTEREQUIP, pItem->GetName());
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerChangeSpouse::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerChangeSpouse)
}

bool CECNetPrtcPlayerChangeSpouse::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_change_spouse))
	{
		assert(false);
		return true;
	}

	player_change_spouse* p = (player_change_spouse*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(p->who);

	if (pPlayer)
	{
		// See if need add a fixed message here.
		if( pPlayer == g_pGame->GetGameRun()->GetHostPlayer() && pPlayer->GetSpouse() && p->spouse == 0 )
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GOTDIVORCED, g_pGame->GetGameRun()->GetPlayerName(pPlayer->GetSpouse(), true));
		else if( pPlayer == g_pGame->GetGameRun()->GetHostPlayer() && pPlayer->GetSpouse() == 0 && p->spouse )
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GOTMARRIED, g_pGame->GetGameRun()->GetPlayerName(p->spouse, true));

		if(pPlayer == g_pGame->GetGameRun()->GetHostPlayer())
			LuaEvent::FirstMarry();
		pPlayer->SetSpouse(p->spouse);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerInvaderState::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerInvaderState)
}

bool CECNetPrtcPlayerInvaderState::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_invader_state))
	{
		assert(false);
		return true;
	}

	player_invader_state* p = (player_invader_state*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(p->who);

	if (pPlayer)
	{
		if (p->state)
			pPlayer->SetInvaderState();
		else
			pPlayer->ClearInvaderState();
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerContribution::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerContribution)
}

bool CECNetPrtcPlayerContribution::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_mafia_contribution))
	{
		assert(false);
		return true;
	}

	player_mafia_contribution* p = (player_mafia_contribution*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
	{
		pHost->SetContribution(p->contribution);
		pHost->SetFamilyContrib(p->family_contrib);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerLotteryBonus::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerLotteryBonus)
}

bool CECNetPrtcPlayerLotteryBonus::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(lottery_bonus))
	{
		assert(false);
		return true;
	}

	// 老彩票，已基本不用 2010-12-29 fuchonggang
	lottery_bonus* p = (lottery_bonus*)m_pData;
	
	CDlgLottery* pDlgLottery = (CDlgLottery*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Lottery");
	pDlgLottery->SetAward(p->bonus_level, p->bonus_item, p->bonus_count, p->bonus_money);

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerLottery2Bonus::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerLottery2Bonus)
}

bool CECNetPrtcPlayerLottery2Bonus::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(lottery2_bonus))
	{
		assert(false);
		return true;
	}

	lottery2_bonus* p = (lottery2_bonus*)m_pData;
	CECIvtrItem *pItem = CECIvtrItem::CreateItem(p->lottery_id, 0, 1);
	if (pItem)
	{
		if (pItem->GetClassID()==CECIvtrItem::ICID_LOTTERY2)
		{
			CECIvtrLottery2 *pLottery2 = (CECIvtrLottery2*)pItem;
			if (pLottery2->GetDBEssence()->type==1)
				g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->CommonMessage("Win_Lottery2", "set_award", p->bonux_index, p->bonus_level);
			else
				g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->CommonMessage("Win_Lottery3", "set_award", p->bonux_index, 0);
		}
		delete pItem;		
	}
		
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//CECNetPrtcPlayerLottery3Bonus协议相关实现函数
//主要包括：Create函数，Process函数(彩票奖励)
///////////////////////////////////////////////////////////////////////////////
CECNetProtocolBase* CECNetPrtcPlayerLottery3Bonus::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerLottery3Bonus)
}

bool CECNetPrtcPlayerLottery3Bonus::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(lottery3_bonus))
	{
		assert(false);
		return true;
	}

	lottery3_bonus* p = (lottery3_bonus*)m_pData;
	CECIvtrItem *pItem = CECIvtrItem::CreateItem(p->lottery_id, 0, 1);
	if (pItem)
	{
		if (pItem->GetClassID()==CECIvtrItem::ICID_LOTTERY3)
		{
			CECIvtrLottery3 *pLottery3 = (CECIvtrLottery3*)pItem;
			if (pLottery3->GetDBEssence()->type==1)
				g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->CommonMessage("Win_Lottery2", "set_award", p->bonus_index, p->bonus_level);
			else
				g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->CommonMessage("Win_Lottery3", "set_award", p->bonus_index, 0);
		}
		delete pItem;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//CECNetPrtcPlayerEnterRaid协议类相关函数实现
//主要包括：Create函数和Process函数（处理gs通知客户端玩家进入副本）
///////////////////////////////////////////////////////////////////////////////
CECNetProtocolBase* CECNetPrtcPlayerEnterRaid::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerEnterRaid)
}

bool CECNetPrtcPlayerEnterRaid::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(cmd_enter_raid))
	{
		assert(false);
		return true;
	}

	cmd_enter_raid* pCmd = (cmd_enter_raid*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost)
	{
		pHost->SetRaidBattleInfo(pCmd->raid_id);
		pHost->SetBattleCamp(pCmd->raid_faction);
		CECBattleDungeonMan::getSingletonPtr()->m_iEndTime = pCmd->end_timestamp;
		CECBattleDungeonMan::getSingletonPtr()->InitFromMapId(pCmd->raid_id);	// 清除副本信息，以便重新加载
		if (pCmd->raid_id==534)	// 爬塔副本id，写死
		{		
			CECBattleDungeonMan::getSingletonPtr()->SetInTower(true);
			CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->GetDialog("Win_Battle_Dungeon_Tower_CharScore")->Show(true);
		}
		else if (pCmd->raid_id==536)	// 四象宫副本，写死
		{
			CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->GetDialog("Win_Battle_Dungeon_SiX_Time")->Show(true);
			pGameUI->GetDialog("Win_Battle_Dungeon_Step_Min")->Show(true);
		}
		else if (pCmd->raid_id==530)	// 焚香谷副本
		{
			CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->GetDialog("Win_Battle_Dungeon_Step_Min")->Show(true);
		}
		else if(pCmd->raid_id == CDlgBattleFactionWarControl::GetFactionPKRaidMapId())//帮战副本
		{
		    CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			//关闭帮战申请界面、观战界面和参战申请界面
			if(pGameUI->GetDialog("Win_Battle_GuildWar_Apply")->IsShow())
				pGameUI->GetDialog("Win_Battle_GuildWar_Apply")->Show(false);
			if(pGameUI->GetDialog("Win_Battle_GuildWarList")->IsShow())
				pGameUI->GetDialog("Win_Battle_GuildWarList")->Show(false);
			if(pGameUI->GetDialog("Win_Battle_GuildWar_Applylist")->IsShow())
				pGameUI->GetDialog("Win_Battle_GuildWar_Applylist")->Show(false);
			//记下当前阵营
			CDlgBattleFactionWarControl* pDlgBFWControl = (CDlgBattleFactionWarControl*)(pGameUI->GetDialog("Win_Battle_GuildWar_Applylist"));
            pDlgBFWControl->SetCurrentFaction(pCmd->raid_faction);
		}
		else if(pCmd->raid_id == 538)      //上古挑战副本
		{
			CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->GetDialog("Win_Dungeon_Ling_Mini")->Show(true);
			pGameUI->GetDialog("Win_Battle_Dungeon_Step_Min")->Show(true);
		}
		else if (pCmd->raid_id == 545 || pCmd->raid_id == 546)	//十神宝窟副本
		{
			CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->GetDialog("Win_Battle_Dungeon_Ssk_Prgs")->Show(true);
		}
		else if(pCmd->raid_id == 544)	//河阳灾变副本
		{
			CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->GetDialog("Win_Battle_Dungeon_Step_Min")->Show(true);
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//CECNetPrtcPlayerDungeonProgress协议类相关函数实现
//主要包括：Create函数和Process函数（处理gs通知客户端玩家进入副本）
///////////////////////////////////////////////////////////////////////////////
CECNetProtocolBase* CECNetPrtcDungeonProgress::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcDungeonProgress)
}

bool CECNetPrtcDungeonProgress::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	using namespace S2C;

	dungeon_progress* pCmd = (dungeon_progress*)m_pData;
	size_t sz;

	if(!pCmd || !pCmd->CheckValid(m_nDataSize, sz))
	{
		assert(false);
		return false;
	}

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgDungeonStep *pMinDlg = (CDlgDungeonStep*) pGameUI->GetDialog("Win_Battle_Dungeon_Step_Min");
	CDlgDungeonStep *pMaxDlg = (CDlgDungeonStep*) pGameUI->GetDialog("Win_Battle_Dungeon_Step_Max");

	pMinDlg->UpdateDungeonProgress(pCmd->steps, pCmd->step_cnt);
	pMaxDlg->UpdateDungeonProgress(pCmd->steps, pCmd->step_cnt);

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//CECNetPrtcPlayerLeaveRaid协议相关函数实现
//主要包括：Create函数和Process函数（处理gs通知客户端离开副本）
////////////////////////////////////////////Z///////////////////////////////////
CECNetProtocolBase* CECNetPrtcPlayerLeaveRaid::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerLeaveRaid)
}

bool CECNetPrtcPlayerLeaveRaid::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != 0)//只有命令号，没有协议内容
	{
		assert(false);
		return true;
	}

	//收到该协议后的处理逻辑
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost)
	{
		pHost->SetRaidBattleInfo(0);
		pHost->SetBattleCamp(0);

		// Hide dungeon step UI.
		CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->GetDialog("Win_Battle_Dungeon_Step_Min")->Show(false);
		pGameUI->GetDialog("Win_Battle_Dungeon_Step_Max")->Show(false);
		pGameUI->GetDialog("Win_Battle_Dungeon_Step_Detail")->Show(false);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//CECNetPrtcPlayerTriggerSkillTime协议相关函数实现
//主要包括：Create函数和Process函数（处理gs通知客户端关于触发技能时间相关信息）
///////////////////////////////////////////////////////////////////////////////
CECNetProtocolBase* CECNetPrtcPlayerTriggerSkillTime::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerTriggerSkillTime)
}

bool CECNetPrtcPlayerTriggerSkillTime::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	//判断协议大小合法性
	if (m_nDataSize != sizeof(trigger_skill_time))
	{
		assert(false);
		return true;
	}

	//收到该协议后的处理逻辑
	trigger_skill_time* pCmd = (trigger_skill_time*)m_pData;
	
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost)	
	{
		CECSkill* pSkill = pHost->GetSkill(pCmd->skill_id);
		if(pSkill) // 如果技能存在，则为需要触发的技能，需特殊处理
		{
			pSkill->SetTriggerSkillCnt((int)pCmd->time*1000);
		}
	}	

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->CommonMessage("Win_Skill_Activate", "time", pCmd->skill_id, pCmd->time);	

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//CECNetPrtcPlayerEnterCollisionRaid协议类相关函数实现
//主要包括：Create函数和Process函数（处理gs通知客户端玩家进入副本）
///////////////////////////////////////////////////////////////////////////////
CECNetProtocolBase* CECNetPrtcPlayerEnterCollisionRaid::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerEnterCollisionRaid)
}

bool CECNetPrtcPlayerEnterCollisionRaid::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(cmd_enter_collision_raid))
	{
		assert(false);
		return true;
	}

	cmd_enter_collision_raid* pCmd = (cmd_enter_collision_raid*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( pHost/* && pHost->GetCharacterID() == pCmd->id*/ )	//主角玩家单独处理逻辑
	{
		pHost->SetRaidBattleInfo(pCmd->raid_id);
		pHost->SetBattleCamp(pCmd->raid_faction);
		//=====================================================================
		//TODO:
		CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->GetDialog("Win_Battle_Dungeon_6v6_Score")->Show(true);
		CECBattleDungeonMan::getSingletonPtr()->m_iEndTime = pCmd->end_timestamp;
		CECBattleDungeonMan::getSingletonPtr()->m_iFaction = pCmd->raid_faction;
		CECBattleDungeonMan::getSingletonPtr()->InitFromMapId(pCmd->raid_id);	// 清除副本信息，以便重新加载
		//!!!变身!!!这个时候，其实HostPlayer还没有真正建立起来，只是new了对象，self_info_1协议还没收到！！！！
//		pHost->NewTransfigure( pCmd->transfigure_id, true );
//		pHost->InsertTransfigureSkill(pCmd->skill_id[0], 1);
//		pHost->InsertTransfigureSkill(pCmd->skill_id[1], 1);
	}

	//-------------------------------------------------------------------------
/*	CECPlayerMan* pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	if ( !pPlayerMan )
	{
		assert(false);
		return true;
	}

	CECPlayer* pPlayer = pPlayerMan->GetPlayer( pCmd->id );
	if ( pPlayer )
	{
		pPlayer->NewTransfigure( pCmd->transfigure_id, true );
	}
*/
	//-------------------------------------------------------------------------
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//CECNetPrtcPlayerCollisionRaidScore协议类相关函数实现
//主要包括：Create函数和Process函数（处理gs通知客户端玩家进入副本）
///////////////////////////////////////////////////////////////////////////////
CECNetProtocolBase* CECNetPrtcPlayerCollisionRaidScore::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerCollisionRaidScore)
}

bool CECNetPrtcPlayerCollisionRaidScore::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	
	cmd_collision_raid_score* pCmd = (cmd_collision_raid_score*)m_pData;
	size_t sz;
	
	if (!pCmd->CheckValid(m_nDataSize, sz))
	{
		assert(false);
		return true;
	}
	
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->CommonMessage("Win_Battle_Dungeon_6v6_Record", "score", (unsigned int)pCmd, 0);
	return true;
}


CECNetProtocolBase* CECNetPrtcPlayerStartSelectMonster::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerStartSelectMonster)
}

bool CECNetPrtcPlayerStartSelectMonster::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	
	//判断协议大小合法性
	if (m_nDataSize != sizeof(random_tower_monster))
	{
		assert(false);
		return true;
	}
	
	//通知客户端弹出随机选怪界面
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	random_tower_monster* pCmd = (random_tower_monster*)m_pData;
	//这个时候hostplayer还没有init，所以先存在界面上
	pGameUI->GetDialog("Win_Battle_Dungeon_Tower_Random")->SetData(pCmd->level, "level");
	if(!pGameUI->GetDialog("Win_Battle_Dungeon_Tower_CharScore")->IsShow())
		pGameUI->GetDialog("Win_Battle_Dungeon_Tower_CharScore")->Show(true);
	pGameUI->GetDialog("Win_Battle_Dungeon_Tower_Random")->Show(true);	// OnShowDialog()中做些处理
	if (pCmd->reenter)
		pGameUI->GetDialog("Win_Battle_Dungeon_Tower_Random")->Show(false);
	
	return true;
}


CECNetProtocolBase* CECNetPrtcPlayerTowerGiveSkills::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerTowerGiveSkills)
}

bool CECNetPrtcPlayerTowerGiveSkills::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	
	player_tower_give_skills* p = (player_tower_give_skills*)m_pData;
	size_t sz;

	//判断协议大小合法性
	if (!p->CheckValid(m_nDataSize, sz))
	{
		assert(false);
		return true;
	}
	
	//进入副本通知：趴塔副本技能手牌
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->CommonMessage("Win_Battle_Dungeon_Tower_LuckySkill", "PLAYER_TOWER_GIVE_SKILLS", (unsigned int)p, 0);
	//TODO:For FuChonggang
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//CECNetPrtcDungeonSskInfo协议类相关函数实现
//主要包括：Create函数和Process函数（处理gs通知客户端玩家十神宝窟副本的信息）
///////////////////////////////////////////////////////////////////////////////
CECNetProtocolBase* CECNetPrtcDungeonSskInfo::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcDungeonSskInfo)
}

bool CECNetPrtcDungeonSskInfo::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	battle_dungeon_ssk_info* pCmd = (battle_dungeon_ssk_info*)m_pData;

	if (pCmd == NULL)
		return false;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgBattleDungeonSsk* pInfo = (CDlgBattleDungeonSsk*)pGameUI->GetDialog("Win_Battle_Dungeon_Ssk_Prgs");	
	if(pInfo)
		pInfo->UpdateInfo(pCmd->tower_index, pCmd->monster_index, pCmd->award_index, pCmd->award_id, pCmd->score);

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//CECNetPrtcPlayerGemNotify协议相关实现函数
//主要包括：create函数，公共接口Process函数
///////////////////////////////////////////////////////////////////////////////
CECNetProtocolBase* CECNetPrtcPlayerGemNotify::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerGemNotify)
}

bool CECNetPrtcPlayerGemNotify::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(gem_notify))
	{
		assert(false);
		return false;
	}

	gem_notify* p = (gem_notify*)m_pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if(p->msg<50)
		pGameUI->MessageBox(2000+p->msg);
	else
		pGameUI->AddChatMessage(pGameUI->GetStringFromTable(2000+p->msg), GP_CHAT_MISC);

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//CECNetPrtcCircleOfDoomStartup协议相关实现函数
//主要包括create函数，公共接口Process函数。主要处理广播消息
///////////////////////////////////////////////////////////////////////////////
CECNetProtocolBase* CECNetPrtcCircleOfDoomStartup::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcCircleOfDoomStartup);
}

bool CECNetPrtcCircleOfDoomStartup::Process(CECGameSession* pSession)
{
	PreProcess(pSession);//预处理

	//判断协议的合法性
	if (m_nDataSize != sizeof(cmd_circleofdoom_startup))
	{
		assert(false);
		return false;
	}

	//获得消息结构和消息内容
	cmd_circleofdoom_startup* pCod = (cmd_circleofdoom_startup*)m_pData;

	//根据发起者id获得该玩家名称和位置
	CECPlayerMan *pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	if ( !pPlayerMan )
	{
		return false;
	}

	CECPlayer *pPlayer = (CECPlayer *)pPlayerMan->GetPlayer(pCod->sponsor_id);
	ACString strPlayerName;
	A3DVECTOR3 vecPos(0.0f);
	static const int nCODTypes[] = 
	{
		FIXMSG_COD_TYPE_TEAM,	//组队
		FIXMSG_COD_TYPE_FACTION, //帮派
		FIXMSG_COD_TYPE_FAMILY,  //家族
	};

	//如果该玩家存在，显示开始结阵广播消息
	if ( pPlayer )
	{
		strPlayerName = pPlayer->GetName();
		vecPos = pPlayer->GetPos();

		//添加自动寻径 目标位置链接文字
		ACString strText;
		ACHAR strName[100];
		a_sprintf( strName, _AL("【%d, %d】"), (int)vecPos.x, (int)vecPos.z );

		// 对strName进行校验 [注]：发到聊天栏中的坐标信息需要进行校验，防止攻击，因此程序中添加的信息也需要校验
		BYTE  checknum = 0;
		BYTE  ch, cl;  // 校验位的高位和低位
		for (int i =0;strName[i] != '\0';  ++i)
		{
			checknum =  checknum^strName[i];
		}
		ch = (checknum >> 4) & 0x0F;
		cl = checknum & 0x0F;
		ch += 'w';
		cl += 'm';
		if (!strName[0])
		{
			ch = 'z';
			ch = 'x';
		}
		int checkcode = (cl << 16) | ch;
		// 得到最终的校验码

		EditBoxItemBase item(enumEICoord);
		item.SetName(strName);
		ACHAR szPos[100];
		a_sprintf(szPos, _AL("%d %d %d %d %d"), 
						(int)vecPos.x, 
						(int)vecPos.y, 
						(int)vecPos.z, 
						g_pGame->GetGameRun()->GetWorld()->GetInstanceID(),
						checkcode
						);
		item.SetInfo(szPos);
		item.SetColor(A3DCOLORRGBA(108, 251, 75,  255));
		strText = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();

		CECStringTab* pMsgTab = g_pGame->GetFixedMsgTab();
		const wchar_t* wstrCodTypeName = _AL("");
		if ( pMsgTab )
		{
			wstrCodTypeName = pMsgTab->GetWideString( nCODTypes[pCod->faction_type - 1] );
		}
		g_pGame->GetGameRun()->AddFixedMessage( FIXMSG_CIRCLE_OF_DOOM_STARTUP, strPlayerName,  wstrCodTypeName,
												strText);
	}
	
	//
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//CECNetPrtcCircleOfDoomInfo协议相关的函数实现
//主要实现该类的创建函数和Process函数，处理客户端在收到gs发来的阵法信息后的更新
///////////////////////////////////////////////////////////////////////////////
CECNetProtocolBase* CECNetPrtcCircleOfDoomInfo::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcCircleOfDoomInfo);
}

bool CECNetPrtcCircleOfDoomInfo::Process(CECGameSession* pSession)
{
	PreProcess(pSession);//预处理

	//判断协议的合法性, 如果阵法成员个数为0，那么gs将不传成员列表。
	if ( m_nDataSize < (sizeof(cmd_circleofdoom_info) - sizeof(int)) )
	{
		assert(false);
		return false;
	}
	
	//获得消息结构和消息内容
	cmd_circleofdoom_info* pCodInfo = (cmd_circleofdoom_info*)m_pData;

	//获得HostPlayer
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( pHost )
	{
		pHost->UpdateCircleOfDoom( pCodInfo->skill_id, pCodInfo->sponsor_id, pCodInfo->member_count, pCodInfo->member_id );
	}
	else
	{
		return false;
	}

	//成功处理
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//CECNetPrtcCircleOfDoomStop协议相关函数实现
//主要完成对该类的_Create函数和Process函数的处理，在收到gs消息后结束当前阵法
///////////////////////////////////////////////////////////////////////////////
CECNetProtocolBase* CECNetPrtcCircleOfDoomStop::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcCircleOfDoomStop);	
}

bool CECNetPrtcCircleOfDoomStop::Process(CECGameSession* pSession)
{
	PreProcess(pSession);//预处理

	//判断协议的合法性
	if ( m_nDataSize != sizeof(cmd_circleofdoom_stop) )
	{
		assert(false);
		return false;
	}

	//获得消息结构和消息内容
	cmd_circleofdoom_stop* pCodStop = (cmd_circleofdoom_stop*)m_pData;

	//取得HostPlayer
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( pHost )
	{
		pHost->ReleaseCircleofDoom( pCodStop->sponsor_id );
	}
	else
	{
		return false;
	}

	//
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//CECNetPrtcJailCircleArea协议相关函数实现
//主要完成对该协议类的_Create函数和Process函数的实现，收到协议后，添加、删除、
//全部清除囚牢相关信息
///////////////////////////////////////////////////////////////////////////////
CECNetProtocolBase* CECNetPrtcJailCircleArea::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcJailCircleArea);
}

bool CECNetPrtcJailCircleArea::Process(CECGameSession* pSession)
{
	PreProcess(pSession);//预处理

	//判断协议的合法性
	if ( m_nDataSize != sizeof(cmd_player_jail_circle_area) )
	{
		assert(false);
		return false;
	}

	//获得消息结构和消息内容
	cmd_player_jail_circle_area* pJail = (cmd_player_jail_circle_area*)m_pData;

	//取得HostPlayer
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( pHost )
	{
		switch ( pJail->mode )
		{
			case 0:	//添加一个囚牢信息
			{
				CECHostPlayer::JAIL_CIRCLE_AREA JailCircleArea;
				memset( &JailCircleArea, 0, sizeof(JailCircleArea) );
				JailCircleArea.jail_type	= pJail->type;
				JailCircleArea.jail_radius	= pJail->radius;
				JailCircleArea.jail_center	= pJail->center;
				pHost->AddJailCircleArea( pJail->id, JailCircleArea );
				break;
			}
			case 1: //删除一个囚牢信息
			{
				pHost->RemoveJailCircleArea( pJail->id );
				break;
			}
			case 2:	//删除全部囚牢信息
			{
				pHost->ClearAllJailCircleArea();
				break;
			}
			default:
				break;

		}
	}

	//
	return true;
}


CECNetProtocolBase* CECNetPrtcPlayerChangeName::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerChangeName)
}

bool CECNetPrtcPlayerChangeName::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	
	ECMSG msg;
	msg.dwMsg		= MSG_PM_CHANGENAME;
	msg.iManager	= MAN_PLAYER;
	msg.iSubID		= -1;
	msg.dwParam1	= (DWORD)m_pData;
	msg.dwParam2	= m_Cmd;
	msg.dwParam3	= m_nDataSize;
	
	g_pGame->GetGameRun()->PostMessage(msg);
	return true;
	return true;
}

CECNetProtocolBase* CECNetPrtcServerTime::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcServerTime)
}

bool CECNetPrtcServerTime::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(cmd_server_time))
	{
		assert(false);
		return true;
	}

	cmd_server_time* pCmd = (cmd_server_time*)m_pData;
	g_pGame->SetServerTime(pCmd->time, pCmd->timezone_bias);

	AFileImage luaFile;
	if( !luaFile.Open("interfaces\\script\\config\\global_api.lua", AFILE_OPENEXIST | AFILE_BINARY | AFILE_TEMPMEMORY) )
	{
		g_dwFatalErrorFlag = FATAL_ERROR_WRONG_CONFIGDATA;
		return true;
	}

	char szLine[1024];
	DWORD dwRead;
	luaFile.ReadLine(szLine, 1024, &dwRead);
	int version;
	sscanf(szLine, "--%d", &version);

	if( version != pCmd->lua_version )
	{
		g_dwFatalErrorFlag = FATAL_ERROR_WRONG_CONFIGDATA;	
	}

	luaFile.Close();
	g_pGame->SetLUAConfigVersion(version);

	return true;
}

CECNetProtocolBase* CECNetPrtcTalismanExpNotify::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcTalismanExpNotify)
}

bool CECNetPrtcTalismanExpNotify::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(talisman_exp_notify))
	{
		assert(false);
		return true;
	}

	talisman_exp_notify* pCmd = (talisman_exp_notify*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost)
	{
		CECInventory * pPack = pHost->GetPack(pCmd->where);
		if( pPack )
		{
			CECIvtrItem * pItem = pPack->GetItem(pCmd->index);
			if( pItem && pItem->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART )
			{
				CECIvtrTalismanMain * pTalisman = (CECIvtrTalismanMain *) pItem;
				pTalisman->SetNewExp(pCmd->new_exp);
			}
		}
	}
	
	return true;
}

CECNetProtocolBase* CECNetPrtcTalismanMergeNotify::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcTalismanMergeNotify)
}

bool CECNetPrtcTalismanMergeNotify::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(talisman_merge_notify))
	{
		assert(false);
		return true;
	}

	talisman_merge_notify* pCmd = (talisman_merge_notify*)m_pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if( pGameUI )
	{
		ACString strText;
		if( g_pGame->GetGameRun()->GetPlayerName(pCmd->id_player, false) )
		{
			CECIvtrItem *pItem1 = CECIvtrItem::CreateItem(pCmd->id_t1, 0, 1);
			CECIvtrItem *pItem2 = CECIvtrItem::CreateItem(pCmd->id_t2, 0, 1);
			CECIvtrItem *pItem3 = CECIvtrItem::CreateItem(pCmd->id_t_result, 0, 1);
			if( pItem1 && pItem2 && pItem3 )
			{
				strText.Format(pGameUI->GetStringFromTable(890), /*g_pGame->GetGameRun()->GetPlayerName(pCmd->id_player, false),*/
					pItem1->GetName(), pItem2->GetName(), pItem3->GetName());
				pGameUI->AddChatMessage(strText, GP_CHAT_LOCAL);
			}
			if( pItem1 )
				delete pItem1;
			if( pItem2 )
				delete pItem2;
			if( pItem3 )
				delete pItem3;
		}
	}
	return true;
}

CECNetProtocolBase* CECNetPrtcTalismanValueNotify::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcTalismanValueNotify)
}

bool CECNetPrtcTalismanValueNotify::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(talisman_value_notify))
	{
		assert(false);
		return true;
	}

	talisman_value_notify* pCmd = (talisman_value_notify*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
		pHost->SetTalismanValue(pCmd->value);

	return true;
}

CECNetProtocolBase* CECNetPrtcBotBeginNotify::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcBotBeginNotify)
}

bool CECNetPrtcBotBeginNotify::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(bot_begin_notify))
	{
		assert(false);
		return true;
	}

	bot_begin_notify* pCmd = (bot_begin_notify*)m_pData;

	if (pCmd->flag)
	{
		g_pGame->GetGameRun()->GetHelper()->Start();
	}
	else
		g_pGame->GetGameRun()->GetHelper()->Stop();

	return true;
}

CECNetProtocolBase* CECNetPrtcTalentNotify::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcTalentNotify)
}

bool CECNetPrtcTalentNotify::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(talent_notify))
	{
		assert(false);
		return true;
	}

	talent_notify* pCmd = (talent_notify*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
		pHost->SetTalentValue(pCmd->value);

	return true;
}

CECNetProtocolBase* CECNetPrtcNotifySafeLock::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcNotifySafeLock)
}

bool CECNetPrtcNotifySafeLock::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(notify_safe_lock))
	{
		assert(false);
		return true;
	}

	notify_safe_lock* pCmd = (notify_safe_lock*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
		pHost->SetSafeLock(pCmd->active != 0);

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgAutoLock*)pGameUI->GetDialog("Win_AutoLock"))->InitAutoLockInfo(pCmd);
	
	return true;
}

CECNetProtocolBase* CECNetPrtcEnterWar::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcEnterWar)
}

bool CECNetPrtcEnterWar::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	
	enter_battlefield *pCmd = (enter_battlefield*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost)
	{
		CECHostPlayer::WAR_INFO info;
		info.iBattleID = pCmd->battle_id;
		info.iWarAttackFID = pCmd->faction_id_attacker;
		info.iWarAssistFID = pCmd->faction_id_attacker_assistant;
		info.iWarDefenceFID = pCmd->faction_id_defender;
		info.iEndTime = pCmd->end_timestamp;
		pHost->SetWarInfo(info);
		pHost->SetIsInWar(true);
	}
	return true;
}

CECNetProtocolBase* CECNetPrtcLeaveWar::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcLeaveWar)
}

bool CECNetPrtcLeaveWar::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost) {
		pHost->SetBattleCamp(GP_BATTLE_CAMP_NONE);
		pHost->SetIsInWar(false);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcWarInfo::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcWarInfo)
}

bool CECNetPrtcWarInfo::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	war_info info;

	if (!info.FillData(m_pData, m_nDataSize))
	{
		assert(false);
		return false;
	}

	for(int i = 0; i < info.count; i++)
		CDlgMiniMap::SetWarBuildStatus(info.nodes[i].key, info.nodes[i].status, info.nodes[i].pos, info.nodes[i].hp);

	return true;
}

CECNetProtocolBase* CECNetPrtcWarStart::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcWarStart)
}

bool CECNetPrtcWarStart::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(war_start))
	{
		assert(false);
		return true;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
		pHost->ResetGuildWarStartCounts();

	return true;
}

CECNetProtocolBase* CECNetPrtcWarEnd::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcWarEnd)
}

bool CECNetPrtcWarEnd::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(war_end))
	{
		assert(false);
		return true;
	}
	
	war_end* p = (war_end*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
	{
		if (p->result == 1)
		{
			if (pHost->GetBattleCamp() == GP_BATTLE_CAMP_INVADER)
				pHost->ResetGuildWarWinCounts();
			else if (pHost->GetBattleCamp() == GP_BATTLE_CAMP_DEFENDER)
				pHost->ResetGuildWarLostCounts();
		}
		else
		{
			if (pHost->GetBattleCamp() == GP_BATTLE_CAMP_DEFENDER)
				pHost->ResetGuildWarWinCounts();
			else if (pHost->GetBattleCamp() == GP_BATTLE_CAMP_INVADER)
				pHost->ResetGuildWarLostCounts();
		}
	}

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (pGameUI)
		((CDlgSiege*)pGameUI->GetDialog("Win_SiegeInfo"))->OnWarEnd(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcWarConstructionInfo::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcWarConstructionInfo)
}

bool CECNetPrtcWarConstructionInfo::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	war_construction_info info;

	if (!info.FillData(m_pData, m_nDataSize))
	{
		assert(false);
		return false;
	}
	
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (pGameUI)
		((CDlgCityBuilding*)pGameUI->GetDialog("Win_CityBuilding"))->OnGetConstructionInfo(&info);

	return true;
}

CECNetProtocolBase* CECNetPrtcWarContributionInfo::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcWarContributionInfo)
}

bool CECNetPrtcWarContributionInfo::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	war_contribution_info info;

	if (!info.FillData(m_pData, m_nDataSize))
	{
		assert(false);
		return false;
	}

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (pGameUI)
		((CDlgCityBuilding*)pGameUI->GetDialog("Win_CityBuilding"))->OnGetContributionInfo(&info);

	return true;
}

CECNetProtocolBase* CECNetPrtcWarBuildingStatusChange::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcWarBuildingStatusChange)
}

bool CECNetPrtcWarBuildingStatusChange::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(war_building_status_change))
	{
		assert(false);
		return true;
	}

	war_building_status_change* p = (war_building_status_change*)m_pData;
	CDlgMiniMap::SetWarBuildStatus(p->key, p->status, p->pos, p->hp);
	
	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerRebornInfo::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerRebornInfo)
}

bool CECNetPrtcPlayerRebornInfo::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	player_reborn_info info;

	if (!info.FillData(m_pData, m_nDataSize))
	{
		assert(false);
		return false;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
	{
		pHost->SetRebornCount(info.count);
		CECHostPlayer::RebornInfoVec& v = pHost->GetRebornInfo();
		v.clear();

		for (unsigned short i = 0; i < info.count; i++)
		{
			CECHostPlayer::REBORN_INFO r;
			r.prof = info.info[i].prof;
			r.level = info.info[i].level;
			v.push_back(r);
		}
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcScriptMessage::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcScriptMessage)
}

bool CECNetPrtcScriptMessage::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(script_message))
	{
		assert(false);
		return true;
	}

	script_message* p = (script_message*)m_pData;
	CECStringTab* pTab = g_pGame->GetPetTalkMsgs();
	const wchar_t* sz = pTab->GetWideString(p->msg_id);

	if (sz)
	{
		if (p->pet_id)
		{
			CECNPC* pPet = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(p->pet_id);

			if (pPet)
			{
				CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();

				ACHAR szText[80];
				AUI_ConvertChatString(pPet->GetName(), szText);

				ACString str;
				str.Format(pStrTab->GetWideString(FIXMSG_CHAT), szText, pTab->GetWideString(p->msg_id));
				g_pGame->GetGameRun()->AddChatMessage(str, (char)p->channel_id, p->pet_id ? p->pet_id : -1);	
				return true;
			}
		}

		g_pGame->GetGameRun()->AddChatMessage(pTab->GetWideString(p->msg_id), (char)p->channel_id, p->pet_id ? p->pet_id : -1);	
		if (p->msg_id==9  || p->msg_id==11 || p->msg_id==22 || p->msg_id==25 ||
			p->msg_id==26 || p->msg_id==29 || p->msg_id==32 ||
			p->msg_id==38 || p->msg_id==41 || p->msg_id==44 ||
			p->msg_id==47 || p->msg_id==50 || p->msg_id==53 ||
			p->msg_id==56 || p->msg_id==59 || p->msg_id==62 ||
			p->msg_id==91)// 不能继续修炼
		{
			CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->CommonMessage("Win_Pet_Detail", "stop_opt", 0, 0);
		}
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerPetCivilization::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerPetCivilization)
}

bool CECNetPrtcPlayerPetCivilization::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_pet_civilization))
	{
		assert(false);
		return true;
	}

	player_pet_civilization* p = (player_pet_civilization*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
	{
		pHost->SetPetCivilization(p->civilization);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerPetConstruction::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerPetConstruction)
}

bool CECNetPrtcPlayerPetConstruction::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_pet_construction))
	{
		assert(false);
		return true;
	}

	player_pet_construction* p = (player_pet_construction*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
	{
		pHost->SetPetConstruction(p->construction);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerStartFly::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerStartFly)
}

bool CECNetPrtcPlayerStartFly::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_start_fly))
	{
		assert(false);
		return true;
	}

	player_start_fly* p = (player_start_fly*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(p->who);

	if (pPlayer)
	{
		ECMSG msg;
		msg.dwMsg = MSG_PM_PLAYERFLY;
		msg.dwParam2 = (p->active ? OBJECT_TAKEOFF : OBJECT_LANDING);
		msg.dwParam3 = p->type;
		pPlayer->ProcessMessage(msg);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerPetPropAdded::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerPetPropAdded)
}

bool CECNetPrtcPlayerPetPropAdded::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_pet_prop_added))
	{
		assert(false);
		return true;
	}

	player_pet_prop_added* p = (player_pet_prop_added*)m_pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgPetDetail*)pGameUI->GetDialog("Win_PetDetail"))->SetPropAdded(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerSectBecomeDisciple::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerSectBecomeDisciple)
}

bool CECNetPrtcPlayerSectBecomeDisciple::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(sect_become_disciple))
	{
		assert(false);
		return true;
	}

	sect_become_disciple* p = (sect_become_disciple*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(p->who);

	if (pPlayer)
		pPlayer->SetMaster(p->master_id);

	if( pPlayer == g_pGame->GetGameRun()->GetHostPlayer())
		LuaEvent::FirstBeStudent();

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerCultivationNotify::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerCultivationNotify)
}

bool CECNetPrtcPlayerCultivationNotify::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(cultivation_notify))
	{
		assert(false);
		return true;
	}

	cultivation_notify* p = (cultivation_notify*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(p->who);

	if (pPlayer)
	{
		pPlayer->UpdateDeityStateGFX(pPlayer->GetCultivation(), p->new_cult);
		pPlayer->SetCultivation(p->new_cult);
	}	
	return true;
}

CECNetProtocolBase* CECNetPrtcOfflineAgentBonus::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcOfflineAgentBonus)
}

bool CECNetPrtcOfflineAgentBonus::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(offline_agent_bonus))
	{
		assert(false);
		return true;
	}

	offline_agent_bonus* p = (offline_agent_bonus*)m_pData;

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgExit*)pGameUI->GetDialog("Win_Message6"))->ShowAgentInfo(p->bonus_time,p->left_time,p->bonus_exp);
	return true;
}

CECNetProtocolBase* CECNetPrtcOfflineAgentTime::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcOfflineAgentTime)
}

bool CECNetPrtcOfflineAgentTime::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(offline_agent_time))
	{
		assert(false);
		return true;
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	offline_agent_time* p = (offline_agent_time*)m_pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	ACHAR szMsg[1024];
	ACString strMsg = _AL("");
	strMsg.Format( pGameUI->GetStringFromTable(7310), p->left_time );

	if (pHost)
	{
		pHost->SetAgentTime(p->left_time);
		if(strMsg != _AL("") && p->left_time > 0)
		{
			AUI_ConvertChatString(strMsg, szMsg);
			pGameUI->AddChatMessage(szMsg, GP_CHAT_MISC);
		}		
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcSelfBuffNotify::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSelfBuffNotify)
}

bool CECNetPrtcSelfBuffNotify::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	self_buff_notify cmd;

	if (!cmd.FillData(m_pData, m_nDataSize))
		return false;
	
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
		pHost->SetBuffState(&cmd);

	return true;
}

CECNetProtocolBase* CECNetPrtcPeepInfoNotify::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPeepInfoNotify)
}

bool CECNetPrtcPeepInfoNotify::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(peep_info))
	{
		assert(false);
		return true;
	}
	
	peep_info* p = (peep_info*)m_pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	ACHAR szMsg[1024];
	ACString strMsg = _AL("");
	strMsg.Format( pGameUI->GetStringFromTable(7350), g_pGame->GetGameRun()->GetPlayerName(p->id, true));

	if(strMsg != _AL(""))
	{
		AUI_ConvertChatString(strMsg, szMsg);
		pGameUI->AddChatMessage(szMsg, GP_CHAT_MISC);
	}		

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerBonus::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerBonus)
}

bool CECNetPrtcPlayerBonus::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_bonus))
	{
		assert(false);
		return true;
	}
	
	player_bonus* p = (player_bonus*)m_pData;

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if(pHost)
	{
		//Added 2011-03-10.
		if( -2 == p->bonus_used )//暂时设置为-2标示。表示该协议仅仅是通过DeliverBonus接口发过来的
		{
			int nAwardBonus = p->bonus_amount - pHost->GetBonusAmount(); //获得当前奖励的鸿利
			int nGold = nAwardBonus/100, nSilver = nAwardBonus%100;
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GET_BONUS, nGold, nSilver);
		}
		//Added end.
		pHost->SetBonus(p->bonus_amount, p->bonus_used);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcMilitaryRankChange::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcMilitaryRankChange)
}

bool CECNetPrtcMilitaryRankChange::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(military_rank_change))
	{
		assert(false);
		return true;
	}
	
	military_rank_change* p = (military_rank_change*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(p->tid);
	if(pPlayer)
	{
		pPlayer->SetMilitaryRank(p->rank);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerChangeFace::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerChangeFace)
}

bool CECNetPrtcPlayerChangeFace::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_change_face))
	{
		assert(false);
		return true;
	}
	
	player_change_face* p = (player_change_face*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(p->tid);
	if(pPlayer)
	{
		pPlayer->ChangeFace(p->faceid, p->hairid, p->fashionid);
		if (pPlayer->GetPhysique() == PHYSIQUE_SMALL)
			pPlayer->ChangeFeature(p->earid, p->tailid);
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		if ( pHost && p->tid==pHost->GetCharacterID() ) //主角
		{			
			GNET::RoleInfo ri = g_pGame->GetGameRun()->GetSelectedRoleInfo();
			ri.faceid = p->faceid;
			ri.hairid = p->hairid;
			ri.fashionid = p->fashionid;
			ri.earid = p->earid;
			ri.tailid = p->tailid;
			g_pGame->GetGameRun()->SetSelectedRoleInfo(ri);
		}
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerChangeVIPState::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerChangeVIPState)
}

bool CECNetPrtcPlayerChangeVIPState::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(player_change_vipstate))
	{
		assert(false);
		return true;
	}
	
	player_change_vipstate* p = (player_change_vipstate*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(p->tid);
	if(pPlayer)
	{
		pPlayer->ChangeVipState(p->vip_state);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerReceiveBonusExp::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerReceiveBonusExp)
}

bool CECNetPrtcPlayerReceiveBonusExp::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	if (m_nDataSize != sizeof(cmd_receive_bonus_exp))
	{
		assert(false);
		return true;
	}
	
	cmd_receive_bonus_exp* p = (cmd_receive_bonus_exp*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if(pHost)
	{
		pHost->ReceiveBonusExp(p->exp);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerAchievementMap::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerAchievementMap)
}

bool CECNetPrtcPlayerAchievementMap::Process(CECGameSession* pSession)
{
	PreProcess(pSession);

	player_achievement_map* p = (player_achievement_map*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetElsePlayer(p->target);
	if(pPlayer)
	{
		CECAchievementMan* pMan = pPlayer->GetAchievementMan();
		if (pMan)
		{
			pMan->UpdateData(p,m_nDataSize);
			CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->GetDialog("Win_Achievement_Other")->Show(true);
			pGameUI->GetDialog("Win_Achievement")->Show(true);
		}
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerTransfigure::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerTransfigure)
}

bool CECNetPrtcPlayerTransfigure::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	if (m_nDataSize != sizeof(player_start_transfigure))
	{
		assert(false);
		return true;
	}
	player_start_transfigure* p = (player_start_transfigure*)m_pData;
	CECPlayerMan* pMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	CECPlayer* pPlayer = pMan->GetPlayer(p->who);
	if(pPlayer)
	{
		if (p->active)
		{
			pPlayer->Transfigure(p->template_id, true);
			if (pPlayer->IsHostPlayer())
				((CECHostPlayer*)pPlayer)->SetTransfigureProp(p->template_id, p->level, p->expLevel);
		}
		else
		{
			pPlayer->Transfigure(0, true);
			if (pPlayer->IsHostPlayer())
				((CECHostPlayer*)pPlayer)->ClearTransfigureProp();
		}
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcNotifyMallStrategy::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcNotifyMallStrategy)
}

bool CECNetPrtcNotifyMallStrategy::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	if (m_nDataSize != sizeof(notify_mall_strategy))
	{
		assert(false);
		return true;
	}
	//最高8位是打折方案， 次高8位是出售方案
	notify_mall_strategy* p = (notify_mall_strategy*)m_pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgQShopItem::SetPropType(p->mall_strategy);
	CDlgQShop *pQshop = (CDlgQShop*)pGameUI->GetDialog("Win_QShop");
	pQshop->Refresh();

	return true;
}

CECNetProtocolBase* CECNetPrtcLocalPKMessage::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcLocalPKMessage)
}

bool CECNetPrtcLocalPKMessage::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	if (m_nDataSize != sizeof(local_pk_message))
	{
		assert(false);
		return true;
	}
	local_pk_message* p = (local_pk_message*)m_pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost)
	{
		if (pHost->GetBattleType() == BATTLE_TYPE_ARENA || pHost->GetBattleType() == BATTLE_TYPE_ARENAREBORN || pHost->GetBattleType() == BATTLE_TYPE_CRSSVR )
			return true;
		
		CDlgPKMsg *pDlgPKMsg = (CDlgPKMsg*)pGameUI->GetDialog("Win_Pkmsg");
		pDlgPKMsg->SetInfo(p->killer, p->deader);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcUpdateComboState::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcUpdateComboState)
}

bool CECNetPrtcUpdateComboState::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	if (m_nDataSize != sizeof(update_combo_state))
	{
		assert(false);
		return true;
	}
	update_combo_state* p = (update_combo_state*)m_pData;
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgHost* pDlgHost = (CDlgHost*)pGameUI->GetDialog("Win_CharHead");
	pDlgHost->UpdateSkillInfo(p->is_start!=0, 5, p->color);

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerInvisible::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerInvisible)
}

bool CECNetPrtcPlayerInvisible::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	if (m_nDataSize != sizeof(enter_invisible_state))
	{
		assert(false);
		return true;
	}
	enter_invisible_state* p = (enter_invisible_state*)m_pData;

	CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(p->id);

	if (pPlayer)
		pPlayer->EnterInvisibleState(p->state);

	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerEnterTerritoryBattle::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerEnterTerritoryBattle)
}

bool CECNetPrtcPlayerEnterTerritoryBattle::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	if (m_nDataSize != sizeof(enter_territory_battle))
	{
		assert(false);
		return true;
	}
	enter_territory_battle* p = (enter_territory_battle*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost)
	{
		pHost->SetBattleCamp(p->faction);
		pHost->SetTerritoryBattleInfo(p->battle_id);
	}	
	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerLeaveTerritoryBattle::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerLeaveTerritoryBattle)
}

bool CECNetPrtcPlayerLeaveTerritoryBattle::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost)
	{
		pHost->SetBattleCamp(GP_BATTLE_CAMP_NONE);
		pHost->SetTerritoryBattleInfo(0);
	}
	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerLastLogoutTime::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerLastLogoutTime)
}

bool CECNetPrtcPlayerLastLogoutTime::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	if (m_nDataSize != sizeof(last_logout_time))
	{
		assert(false);
		return true;
	}
	last_logout_time* p = (last_logout_time*)m_pData;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost)
		pHost->SetLastLogoutTime(p->time);
	return true;
}

CECNetProtocolBase* CECNetPrtcPlayerDeityLevelUp::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPlayerDeityLevelUp)
}

bool CECNetPrtcPlayerDeityLevelUp::Process(CECGameSession* pSession)
{
	PreProcess(pSession);
	if (m_nDataSize != sizeof(deity_levelup))
	{
		assert(false);
		return true;
	}
	deity_levelup* p = (deity_levelup*)m_pData;
	CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(p->idPlayer);
	if (pPlayer)
		pPlayer->DeityLevelUp(p->success == 0? false: true, p->iLevel);
	return true;
}

