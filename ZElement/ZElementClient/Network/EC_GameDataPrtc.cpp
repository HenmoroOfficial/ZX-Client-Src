/*
 * FILE: EC_GameDataPrtc.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/12/22
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include <AFile.h>

#include "EC_Global.h"
#include "EC_GameSession.h"
#include "EC_Game.h"
#include "EC_GPDataType.h"
#include "EC_GameRun.h"
#include "EC_RTDebug.h"
#include "EC_MsgDef.h"
#ifdef BACK_VERSION
#include "EC_Manager.h"
#else
#include "ECManager.h"
#include "ECManagerID.h"
#endif
#include "EC_FunGamePrtcHandler.h"
#include "EC_HomeTownPrtcHandler.h"

#include "EC_Utility.h"
#include "AnmSystem.h"
#include "AFileImage.h"

#include "gnet.h"

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

DWORD l_totalCmdSize = 0;

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECGameSession
//	
///////////////////////////////////////////////////////////////////////////

/*	Calculate S2C command data size

	Return values:
	
		(DWORD)(-1): means iCmd is a unknown command.
		(DWORD)(-2): command size is hard to calculate, so we ignore it.
*/
DWORD CECGameSession::CalcS2CCmdDataSize(int iCmd, BYTE* pDataBuf, DWORD dwDataSize)
{
	using namespace S2C;

#define CHECK_VALID(type) \
{\
	if (!reinterpret_cast<type*>(pDataBuf)->CheckValid(dwDataSize, sz) || dwDataSize != sz) {\
		assert(0);}\
	else\
		dwSize = (DWORD)(-2);\
}

	DWORD dwSize = (DWORD)(-1);
	size_t sz;

	switch (iCmd)
	{

	case PLAYER_INFO_2:				break;
	case PLAYER_INFO_3:				break;
	case PLAYER_INFO_4:				break;
	case PLAYER_INFO_2_LIST:		break;
	case PLAYER_INFO_3_LIST:		break;
	case PLAYER_INFO_23_LIST:		break;

	case MATTER_INFO_LIST:
	
	//	CHECK_VALID(cmd_matter_info_list);
		dwSize = (DWORD)(-2);
		break;

	case NOTIFY_HOSTPOS:			dwSize = sizeof (cmd_notify_hostpos);		break;
	case MATTER_ENTER_WORLD:		dwSize = (DWORD)(-2);						break;
	case HOST_STOPATTACK:			dwSize = sizeof (cmd_host_stop_attack);		break;
	case HOST_ATTACKRESULT:			dwSize = sizeof (cmd_host_attack_result);	break;
	case ERROR_MESSAGE:				dwSize = sizeof (cmd_error_msg);			break;
	case HOST_ATTACKED:				dwSize = sizeof (cmd_host_attacked);		break;
	case PLAYER_DIED:				dwSize = sizeof (cmd_player_died);			break;
	case HOST_DIED:					dwSize = sizeof (cmd_host_died);			break;
	case PLAYER_REVIVE:				dwSize = sizeof (cmd_player_revive);		break;
	case PICKUP_MONEY:				dwSize = sizeof (cmd_pickup_money);			break;
	case PICKUP_ITEM:				dwSize = sizeof (cmd_pickup_item);			break;
	case PUT_ITEM:					dwSize = sizeof (put_item);					break;

	case OBJECT_STOP_MOVE:			dwSize = sizeof (cmd_object_stop_move);		break;
	case RECEIVE_EXP:				dwSize = sizeof (cmd_receive_exp);			break;
	case LEVEL_UP:					dwSize = sizeof (cmd_level_up);				break;
	case UNSELECT:					dwSize = 0;									break;
	case OWN_ITEM_INFO:

	//	CHECK_VALID(cmd_own_item_info)
		dwSize = (DWORD)(-2);
		break;

	case EMPTY_ITEM_SLOT:			dwSize = sizeof (cmd_empty_item_slot);		break;
	case OWN_IVTR_DATA:

	//	CHECK_VALID(cmd_own_ivtr_info)
		dwSize = (DWORD)(-2);
		break;

	case OWN_IVTR_DETAIL_DATA:

	//	CHECK_VALID(cmd_own_ivtr_detail_info)
		dwSize = (DWORD)(-2);
		break;

	case EXG_IVTR_ITEM:				dwSize = sizeof (cmd_exg_ivtr_item);		break;
	case MOVE_IVTR_ITEM:			dwSize = sizeof (cmd_move_ivtr_item);		break;
	case PLAYER_DROP_ITEM:			dwSize = sizeof (cmd_player_drop_item);		break;
	case EXG_EQUIP_ITEM:			dwSize = sizeof (cmd_exg_equip_item);		break;
	case EQUIP_ITEM:				dwSize = sizeof (cmd_equip_item);			break;
	case RECORD_CHECK_RESULT:		dwSize = sizeof	(record_check_result);		break;
	case MOVE_EQUIP_ITEM:			dwSize = sizeof (cmd_move_equip_item);		break;
	case OWN_EXT_PROP:				dwSize = sizeof (cmd_own_ext_prop);			break;
	case ADD_STATUS_POINT:			dwSize = sizeof (cmd_add_status_point);		break;
	case SELECT_TARGET:				dwSize = sizeof (cmd_select_target);		break;
	case PLAYER_EXT_PROP_BASE:		dwSize = sizeof (cmd_pep_base);				break;
	case PLAYER_EXT_PROP_MOVE:		dwSize = sizeof (cmd_pep_move);				break;
	case PLAYER_EXT_PROP_ATK:		dwSize = sizeof (cmd_pep_attack);			break;
	case PLAYER_EXT_PROP_DEF:		dwSize = sizeof (cmd_pep_def);				break;
	case TEAM_LEADER_INVITE:		dwSize = sizeof (cmd_team_leader_invite);	break;
	case TEAM_REJECT_INVITE:		dwSize = sizeof (cmd_team_reject_invite);	break;
	case TEAM_JOIN_TEAM:			dwSize = sizeof (cmd_team_join_team);		break;
	case TEAM_MEMBER_LEAVE:			dwSize = sizeof (cmd_team_member_leave);	break;
	case TEAM_LEAVE_PARTY:			dwSize = sizeof (cmd_team_leave_party);		break;
	case TEAM_NEW_MEMBER:			dwSize = sizeof (cmd_team_new_member);		break;
	case TEAM_LEADER_CACEL_PARTY:	dwSize = sizeof (cmd_team_leader_cancel_party);	break;

	case TEAM_MEMBER_DATA:

		CHECK_VALID(cmd_team_member_data);
		break;

	case TEAM_MEMBER_POS:			dwSize = sizeof (cmd_team_member_pos);		break;
	case EQUIP_DATA:

		CHECK_VALID(cmd_equip_data);
		break;

	case EQUIP_DATA_CHANGED:

		CHECK_VALID(cmd_equip_data_changed);
		break;
	case CLONE_PLAYER_EQUIP:

		dwSize = DWORD(-2);
		break;
	case EQUIP_DAMAGED:				dwSize = sizeof (cmd_equip_damaged);		break;
	case TEAM_MEMBER_PICKUP:		dwSize = sizeof (cmd_team_member_pickup);	break;
	case NPC_GREETING:				dwSize = sizeof (cmd_npc_greeting);			break;

	case NPC_SERVICE_CONTENT:

		CHECK_VALID(cmd_npc_sev_content)
		break;

	case PURCHASE_ITEM:

		CHECK_VALID(cmd_purchase_item)
		break;

	case ITEM_TO_MONEY:				dwSize = sizeof (cmd_item_to_money);		break;
	case REPAIR_ALL:				dwSize = sizeof (cmd_repair_all);			break;
	case REPAIR:					dwSize = sizeof (cmd_repair);				break;

	case RENEW:

		break;

	case SPEND_MONEY:				dwSize = sizeof (cmd_spend_money);			break;
	case GAIN_MONEY_IN_TRADE:		dwSize = sizeof (cmd_gain_money_in_trade);	break;
	case GAIN_ITEM_IN_TRADE:		dwSize = sizeof (cmd_gain_item_in_trade);	break;
	case GAIN_MONEY_AFTER_TRADE:	dwSize = sizeof (cmd_gain_money_after_trade);	break;
	case GAIN_ITEM_AFTER_TRADE:		dwSize = sizeof (cmd_gain_item_after_trade);	break;
	case GET_OWN_MONEY:				dwSize = sizeof (cmd_get_own_money);		break;
	case ATTACK_ONCE:				dwSize = sizeof (cmd_attack_once);			break;
	case HOST_START_ATTACK:			dwSize = sizeof (cmd_host_start_attack);	break;
	case OBJECT_CAST_SKILL:			dwSize = DWORD(-2);		break;
	case SKILL_INTERRUPTED:			dwSize = sizeof (cmd_skill_interrupted);	break;
	case SKILL_PERFORM:				dwSize = sizeof (cmd_skill_perform);		break;
	case SELF_SKILL_INTERRUPTED:	dwSize = sizeof (cmd_self_skill_interrupted);	break;
	case PLAYER_CHARGE:				dwSize = sizeof (player_charge);			break;

	case PLAYER_STOP_SKILL:			dwSize = sizeof (player_stop_skill);		break;	//Added 2011-08-30.
	case PLAYER_ENTER_RAID_COUNT:	dwSize = sizeof (player_enter_raid_count);	break;	//Added 2011-12-02.
	case MALL_ITEM_PRICE:
		CHECK_VALID(cmd_mall_item_price)
		break;

	case PLAYER_VIP_AWARD_INFO:		//Added 2012-01-04.
		CHECK_VALID(player_vip_award_info)
		break;

	case ONLINE_AWARD_INFO:			//Added 2012-02-16.
		CHECK_VALID(online_award_info)
		break;

	case ONLINE_AWARD_PERMIT:		dwSize = sizeof (online_award_permit);		break;	//Added 2012-02-16.
	case ZX_FIVE_ANNI_DATA:			dwSize = sizeof (zx_five_anni_data);		break;
	case TREASURE_INFO:				//Added 2012-03-22.
		CHECK_VALID(treasure_info)
		break;
	case TREASURE_REGION_UPGRADE_RESULT:	dwSize = sizeof(treasure_region_upgrade_result);	break;
	case TREASURE_REGION_UNLOCK_RESULT:		dwSize = sizeof(treasure_region_unlock_result);		break;
	case INC_TREASURE_DIG_COUNT:		dwSize = sizeof(inc_treasure_dig_count);		break;
	case TREASURE_REGION_DIG_RESULT:
		CHECK_VALID(treasure_region_dig_result)
		break;

	//Added 2012-04-17.
	case RAID_GLOBAL_VARIABLE:
		CHECK_VALID(raid_global_variable)
		break;
	case PLAYER_TOWER_DATA:
		CHECK_VALID(player_tower_data)
		break;
	case PLAYER_TOWER_MONSTER_LIST:
		CHECK_VALID(player_tower_monster_list)
		break;
	case RANDOM_TOWER_MONSTER_RESULT:	dwSize = sizeof(random_tower_monster_result);	break;
	case SET_TOWER_GIVE_SKILL:			dwSize = sizeof(set_tower_give_skill);	break;
	case ADD_TOWER_MONSTER:				dwSize = sizeof(add_tower_monster);	break;
	case SET_TOWER_AWARD_STATE:			dwSize = sizeof(set_tower_award_state);	break;
	//Added end.

	case SKILL_DATA:

		CHECK_VALID(cmd_skill_data)
		break;
	
	case HOST_USE_ITEM:				dwSize = sizeof (cmd_host_use_item);		break;
	case PLAYER_USE_ITEM_WITH_ARG:	dwSize = (DWORD)-2; break;
	case EMBED_ITEM:				dwSize = sizeof (cmd_embed_item);			break;
	case CLEAR_TESSERA:				dwSize = sizeof (cmd_clear_tessera);		break;
	case COST_SKILL_POINT:			dwSize = sizeof (cmd_cost_skill_point);		break;
	case LEARN_SKILL:				dwSize = sizeof (cmd_learn_skill);			break;
	case OBJECT_TAKEOFF:			dwSize = sizeof (cmd_object_takeoff);		break;
	case OBJECT_LANDING:			dwSize = sizeof (cmd_object_landing);		break;
	case FLYSWORD_TIME:				dwSize = sizeof (cmd_flysword_time);		break;
	case HOST_OBTAIN_ITEM:			dwSize = sizeof (cmd_host_obtain_item);		break;
	case PRODUCE_START:				dwSize = sizeof (cmd_produce_start);		break;
	case PRODUCE_ONCE:				dwSize = sizeof (cmd_produce_once);			break;
	case PRODUCE_END:				dwSize = 0;									break;
	case DECOMPOSE_START:			dwSize = sizeof (cmd_decompose_start);		break;
	case DECOMPOSE_END:				dwSize = 0;									break;
	case TASK_DATA:

		CHECK_VALID(cmd_task_data)
		break;

	case TASK_VAR_DATA:

		CHECK_VALID(cmd_task_var_data)
		break;

	case OBJECT_START_USE:			dwSize = sizeof (cmd_object_start_use);		break;
	case OBJECT_CANCEL_USE:			dwSize = sizeof (cmd_object_cancel_use);	break;
	case OBJECT_USE_ITEM:			dwSize = sizeof (cmd_object_use_item);		break;
	case OBJECT_START_USE_T:		dwSize = sizeof (cmd_object_start_use_t);	break;
	case OBJECT_USE_ITEM_WITH_ARG:	dwSize = (DWORD)-2;	break;
	case OBJECT_SIT_DOWN:			dwSize = sizeof (cmd_object_sit_down);		break;
	case OBJECT_STAND_UP:			dwSize = sizeof (cmd_object_stand_up);		break;
	case OBJECT_DO_EMOTE:			dwSize = sizeof (cmd_object_do_emote);		break;
	case SERVER_TIME:				dwSize = sizeof (cmd_server_time);			break;
	case OBJECT_ROOT:				dwSize = sizeof (cmd_object_root);			break;
	case HOST_DISPEL_ROOT:			dwSize = sizeof (cmd_host_dispel_root);		break;
	case OBJECT_ATTACK_RESULT:		dwSize = sizeof (cmd_object_atk_result);	break;
	case OBJECT_STARTATTACK:		dwSize = sizeof (cmd_object_start_attack);	break;
	case BE_HURT:					dwSize = sizeof (cmd_be_hurt);				break;
	case HURT_RESULT:				dwSize = sizeof (cmd_hurt_result);			break;
	case HOST_STOP_SKILL:			dwSize = sizeof (cmd_host_stop_skill);		break;
	case UPDATE_EXT_STATE:			dwSize = sizeof (cmd_update_ext_state);		break;
	case PLAYER_SCALE:				dwSize = sizeof (cmd_player_scale);			break; //Added 2011-06-28.
	case PLAYER_VIP_AWARD_UPDATE:	dwSize = sizeof (player_vip_award_update);	break; //Added 2012-01-16.
	case COLLISION_SPECIAL_STATE_INFO: dwSize = sizeof (cmd_collision_special_state_info);	break; //Added 2012-08-02.
	case PLAYER_DARK_LIGHT_VALUE:	dwSize = sizeof (cmd_player_dark_light_value);break;	//Added 2012-08-16.
	case PLAYER_DARK_LIGHT_STATE:	dwSize = sizeof	(cmd_player_dark_light_state);break;	//Added 2012-08-16.
	case PLAYER_WING_CHANGE_COLOR:	dwSize = sizeof (cmd_player_wing_change_color);break;	//Added 2012-08-20.
	case DIR_VISIBLE_STATE:			dwSize = sizeof (cmd_dir_visible_state);	break;		//Added 2012-09-05.
	case PLAYER_REMEDY_METEMPSYCHOSIS_LEVEL:			dwSize = 0;	break;		// 2012-10-17. add 注册消息结构 by zy
	case LIVENESS_NOTIFY:			dwSize = DWORD(-2);	break;		//Added 2012-11-7.		
	case PLAYER_DARK_LIGHT_SPIRIT:	dwSize = sizeof (cmd_player_dark_light_spirit);break;
	case MIRROR_IMAGE_CNT:			dwSize = sizeof (cmd_mirror_image_cnt);		break;

	case ICON_STATE_NOTIFY:			dwSize = DWORD(-2);							break;

	case PLAYER_GATHER_START:		dwSize = sizeof (cmd_player_gather_start);	break;
	case PLAYER_GATHER_STOP:		dwSize = sizeof (cmd_player_gather_stop);	break;
	case TRASHBOX_PWD_CHANGED:		dwSize = sizeof (cmd_trashbox_pwd_changed);	break;
	case TRASHBOX_PWD_STATE:		dwSize = sizeof (cmd_trashbox_pwd_state);	break;
	case TRASHBOX_OPEN:				dwSize = sizeof (cmd_trashbox_open);		break;
	case TRASHBOX_CLOSE:			dwSize = 0;									break;
	case TRASHBOX_WEALTH:			dwSize = sizeof (cmd_trashbox_wealth);		break;
	case EXG_TRASHBOX_ITEM:			dwSize = sizeof (cmd_exg_trashbox_item);	break;	
	case MOVE_TRASHBOX_ITEM:		dwSize = sizeof (cmd_move_trashbox_item);	break;
	case EXG_TRASHBOX_IVTR:			dwSize = sizeof (cmd_exg_trashbox_ivtr);	break;
	case IVTR_ITEM_TO_TRASH:		dwSize = sizeof (cmd_trash_item_to_ivtr);	break;
	case TRASH_ITEM_TO_IVTR:		dwSize = sizeof (cmd_ivty_item_to_trash);	break;
	case EXG_TRASH_MONEY:			dwSize = sizeof (cmd_exg_trash_money);		break;
	case ENCHANT_RESULT:			dwSize = sizeof (cmd_enchant_result);		break;
	case HOST_NOTIFY_ROOT:			dwSize = sizeof (cmd_host_notify_root);		break;
	case OBJECT_DO_ACTION:			dwSize = sizeof (cmd_obj_do_action);		break;
	case OBJECT_SKILL_ATTACK_RESULT:dwSize = sizeof (cmd_object_skill_attack_result); break;
	case HOST_SKILL_ATTACKED:		dwSize = sizeof (cmd_host_skill_attacked);	break;
	case PLAYER_SET_ADV_DATA:		dwSize = sizeof (cmd_player_set_adv_data);	break;
	case PLAYER_CLR_ADV_DATA:		dwSize = sizeof (cmd_player_clr_adv_data);	break;
	case PLAYER_IN_TEAM:			dwSize = sizeof (cmd_player_in_team);		break;
	case TEAM_ASK_TO_JOIN:			dwSize = sizeof (cmd_team_ask_join);		break;
	case OBJECT_EMOTE_RESTORE:		dwSize = sizeof (cmd_object_emote_restore);	break;
	case CON_EMOTE_REQUEST:			dwSize = sizeof (cmd_con_emote_request);	break;
	case DO_CONCURRENT_EMOTE:		dwSize = sizeof (cmd_do_concurrent_emote);	break;
	case MATTER_PICKUP:				dwSize = sizeof (cmd_matter_pickup);		break;
	case MAFIA_INFO_NOTIFY:			dwSize = sizeof (cmd_mafia_info_notify);	break;
	case MAFIA_TRADE_START:			dwSize = 0;									break;
	case MAFIA_TRADE_END:			dwSize = 0;									break;
	case TASK_DELIVER_ITEM:			dwSize = sizeof (cmd_task_deliver_item);	break;
	case TASK_DELIVER_REP:			dwSize = sizeof (cmd_task_deliver_rep);		break;
	case TASK_DELIVER_EXP:			dwSize = sizeof (cmd_task_deliver_exp);		break;
	case TASK_DELIVER_MONEY:		dwSize = sizeof (cmd_task_deliver_money);	break;
	case TASK_DELIVER_LEVEL2:		dwSize = sizeof (cmd_task_deliver_level2);	break;
	case ITEM_IDENTIFY:				dwSize = sizeof (cmd_item_identify);		break;
	case PLAYER_CHGSHAPE:			dwSize = sizeof (cmd_player_chgshape);		break;
	case ENTER_SANCTUARY:			dwSize = 0;									break;
	case LEAVE_SANCTUARY:			dwSize = 0;									break;

	case PLAYER_OPEN_BOOTH:

		CHECK_VALID(cmd_player_open_booth)
		break;

	case SELF_OPEN_BOOTH:

		CHECK_VALID(cmd_self_open_booth)
		break;

	case PLAYER_CLOSE_BOOTH:		dwSize = sizeof (cmd_player_close_booth);	break;
	case PLAYER_BOOTH_INFO:

		CHECK_VALID(cmd_player_booth_info)
		break;

	case BOOTH_TRADE_SUCCESS:		dwSize = sizeof (cmd_booth_trade_success);	break;

	case BOOTH_NAME:

		CHECK_VALID(cmd_booth_name)
		break;

	case PLAYER_START_TRAVEL:		dwSize = sizeof (cmd_player_start_travel);	break;
	case HOST_START_TRAVEL:			dwSize = sizeof (cmd_host_start_travel);	break;
	case PLAYER_END_TRAVEL:			dwSize = sizeof (cmd_player_end_travel);	break;
	case GM_INVINCIBLE:				dwSize = sizeof (gm_cmd_invincible);		break;
	case GM_INVISIBLE:				dwSize = sizeof (gm_cmd_invisible);			break;
	case HOST_CORRECT_POS:			dwSize = sizeof (cmd_host_correct_pos);		break;
	case OBJECT_CAST_INSTANT_SKILL:	dwSize = DWORD(-2);	 break;
	case ACTIVATE_WAYPOINT:			dwSize = sizeof (cmd_activate_waypoint);	break;

	case WAYPOINT_LIST:

		CHECK_VALID(cmd_waypoint_list)
		break;

	case UNFREEZE_IVTR_SLOT:		dwSize = sizeof (cmd_unfreeze_ivtr_slot);	break;
	case TEAM_INVITE_TIMEOUT:		dwSize = sizeof (cmd_team_invite_timeout);	break;
	case PLAYER_PVP_NO_PROTECT:		dwSize = sizeof (cmd_player_enable_pvp);	break;
	case PLAYER_DISABLE_PVP:		dwSize = sizeof (cmd_player_disable_pvp);	break;
	case HOST_PVP_COOLDOWN:			dwSize = sizeof (cmd_host_pvp_cooldown);	break;

	case COOLTIME_DATA:

		CHECK_VALID(cmd_cooltime_data)
		break;

	case SKILL_ABILITY:				dwSize = sizeof (cmd_skill_ability);		break;
	case OPEN_BOOTH_TEST:			dwSize = sizeof	(cmd_open_booth_test);		break;
	case BREATH_DATA:				dwSize = sizeof (cmd_breath_data);			break;
	case HOST_STOP_DIVE:			dwSize = 0;									break;
	case BOOTH_SELL_ITEM:			dwSize = sizeof (cmd_booth_sell_item);		break;
	case PLAYER_ENABLE_FASHION:		dwSize = sizeof (cmd_player_enable_fashion);break;
	case HOST_ENABLE_FREEPVP:		dwSize = sizeof (cmd_host_enable_freepvp);	break;
	case INVALID_OBJECT:			dwSize = sizeof (cmd_invalid_object);		break;
	case PLAYER_ENABLE_EFFECT:		dwSize = sizeof (cmd_player_enable_effect);	break;
	case PLAYER_DISABLE_EFFECT:		dwSize = sizeof (cmd_player_disable_effect);break;
	case REVIVAL_INQUIRE:			dwSize = sizeof (cmd_revival_inquire);		break;
	case SET_COOLDOWN:				dwSize = sizeof (cmd_set_cooldown);			break;
	case CHANGE_TEAM_LEADER:		dwSize = sizeof (cmd_change_team_leader);	break;
	case EXIT_INSTANCE:				dwSize = sizeof (cmd_exit_instance);		break;
	case CHANGE_FACE_START:			dwSize = sizeof (cmd_change_face_start);	break;
	case CHANGE_FACE_END:			dwSize = sizeof (cmd_change_face_end);		break;
	case PLAYER_CHG_FACE:			dwSize = sizeof (cmd_player_chg_face);		break;
	case OBJECT_CAST_POS_SKILL:		dwSize = sizeof (cmd_object_cast_pos_skill);break;
	case SET_MOVE_STAMP:			dwSize = sizeof (cmd_set_move_stamp);		break;
	case INST_DATA_CHECKOUT:		dwSize = sizeof (cmd_inst_data_checkout);	break;
	case HOST_RUSH_FLY:				dwSize = sizeof (cmd_host_rush_fly);		break;
	case TRASHBOX_SIZE:				dwSize = sizeof (cmd_trashbox_size);		break;
	case MAFIA_TRASHBOX_CAPACITY:	dwSize = sizeof (mafia_trashbox_capacity_notify); break;
	case PRODUCE_NULL:				dwSize = sizeof (cmd_produce_null);			break;
	case PVP_COMBAT:				dwSize = sizeof (cmd_pvp_combat);			break;
	case DOUBLE_EXP_TIME:			dwSize = sizeof (cmd_double_exp_time);		break;
	case AVAILABLE_DOUBLE_EXP_TIME:	dwSize = sizeof (cmd_available_double_exp_time); break;
	case DUEL_RECV_REQUEST:			dwSize = sizeof (cmd_duel_recv_request);	break;
	case DUEL_REJECT_REQUEST:		dwSize = sizeof (cmd_duel_reject_request);	break;
	case DUEL_PREPARE:				dwSize = sizeof (cmd_duel_prepare);			break;
	case DUEL_CANCEL:				dwSize = sizeof (cmd_duel_cancel);			break;
	case HOST_DUEL_START:			dwSize = sizeof (cmd_host_duel_start);		break;
	case DUEL_STOP:					dwSize = sizeof (cmd_duel_stop);			break;
	case DUEL_RESULT:				dwSize = sizeof (cmd_duel_result);			break;
	case PLAYER_BIND_REQUEST:		dwSize = sizeof (cmd_player_bind_request);	break;
	case PLAYER_BIND_INVITE:		dwSize = sizeof (cmd_player_bind_invite);	break;
	case PLAYER_BIND_REQUEST_REPLY:	dwSize = sizeof (cmd_player_bind_request_reply);	break;
	case PLAYER_BIND_INVITE_REPLY:	dwSize = sizeof (cmd_player_bind_invite_reply);		break;
	case PLAYER_BIND_START:			dwSize = sizeof (cmd_player_bind_start);	break;
	case PLAYER_BIND_STOP:			dwSize = sizeof (cmd_player_bind_stop);		break;
	case PLAYER_MOUNTING:			dwSize = sizeof (cmd_player_mounting);		break;
	case PLAYER_EQUIP_DETAIL:		dwSize = (DWORD)(-2);						break;
	case PLAYER_DUEL_START:			dwSize = sizeof (cmd_player_duel_start);	break;
	case PARIAH_TIME:				dwSize = sizeof (cmd_pariah_time);			break;
	case GAIN_PET:					dwSize = sizeof (cmd_gain_pet);				break;
	case FREE_PET:					dwSize = sizeof (cmd_free_pet);				break;
	case SUMMON_PET:				dwSize = sizeof (cmd_summon_pet);			break;
	case RECALL_PET:				dwSize = sizeof (cmd_recall_pet);			break;
	case PLAYER_START_PET_OP:		dwSize = sizeof (cmd_player_start_pet_op);	break;
	case PLAYER_STOP_PET_OP:		dwSize = 0;									break;
	case PET_RECEIVE_EXP:			dwSize = sizeof (cmd_pet_receive_exp);		break;
	case PET_LEVELUP:				dwSize = sizeof (cmd_pet_levelup);			break;
	case EQUIP_PET_BEDGE:			dwSize = sizeof (equip_pet_bedge);			break;
	case EQUIP_PET_EQUIP:			dwSize = sizeof (equip_pet_equip);			break;
	case PLAYER_SET_PET_STATUS:		dwSize = sizeof (player_set_pet_status);	break;
	case PLAYER_SET_PET_RANK:		dwSize = sizeof (player_set_pet_rank);		break;
	case PLAYER_COMBINE_PET:		dwSize = sizeof (player_combine_pet);		break;
	case PLAYER_UNCOMBINE_PET:		dwSize = sizeof (player_uncombine_pet);		break;
	case PLAYER_PET_HP_NOTIFY:		dwSize = sizeof (player_pet_hp_notify);		break;
	case PLAYER_PET_AGE_LIFE:		dwSize = sizeof (player_pet_age_life);		break;
	case PLAYER_PET_DEAD:			dwSize = sizeof (player_pet_dead);			break;
	case PLAYER_PET_AI_STATE:		dwSize = sizeof (player_pet_ai_state);		break;
	case PLAYER_PET_SET_AUTO_SKILL:	dwSize = sizeof (player_pet_set_auto_skill);break;
	case PLAYER_PET_SET_SKILL_COOLDOWN: dwSize = sizeof (player_pet_set_skill_cooldown); break;
	case PLAYER_MOVE_PET_BEDGE_ITEM:dwSize = sizeof (player_move_pet_bedge);	break;

	case PET_ROOM:
	{
		const cmd_pet_room* pCmd = (const cmd_pet_room*)pDataBuf;
		dwSize = sizeof (cmd_pet_room) + pCmd->count * (sizeof (int) + sizeof (info_pet));
		break;
	}
	case PET_ROOM_CAPACITY:			dwSize = sizeof (cmd_pet_room_capacity);	break;
	case PET_HONOR_POINT:			dwSize = sizeof (cmd_pet_honor_point);		break;
	case PET_HUNGER_GAUGE:			dwSize = sizeof (cmd_pet_hunger_gauge);		break;
	case HOST_ENTER_BATTLE:			dwSize = sizeof (cmd_host_enter_battle);	break;
	case BATTLE_LEAVE:				dwSize = 0;									break;
	case TANK_LEADER_NOTIFY:		dwSize = sizeof (cmd_tank_leader_notify);	break;
	case BATTLE_RESULT:				dwSize = sizeof (cmd_battle_result);		break;
	case BATTLE_SCORE:				dwSize = sizeof (battle_score);				break;
	case GAIN_BATTLE_SCORE:			dwSize = sizeof (gain_battle_score);		break;
	case SPEND_BATTLE_SCORE:		dwSize = sizeof (spend_battle_score);		break;
	case BATTLE_INFO:				dwSize = (DWORD)(-2);						break;
	case PLAYER_WALLOW_INFO:		dwSize = sizeof (player_wallow_info);		break;
	case ENTER_WAR:					dwSize = sizeof (enter_battlefield);		break;
	case EQUIP_SPIRIT_POWER_DECREASE: dwSize = sizeof(cmd_spirit_power_decrease); break;

	case PLAYER_MULTI_BIND_INVITE:	dwSize = sizeof (cmd_player_bind_invite);		break;
	case PLAYER_MULTI_BIND_INVITE_REPLY: dwSize = sizeof (cmd_player_bind_invite_reply);		break;
	case PLAYER_MULTI_BIND_JOIN:	dwSize = sizeof (cmd_player_multi_bind_join);	break;
	case PLAYER_MULTI_BIND_LEAVE:	dwSize = sizeof (cmd_player_multi_bind_leave);	break;
	case PLAYER_MULTI_BIND_KICK:	dwSize = sizeof (cmd_player_multi_bind_kick);   break;
	case EXG_POCKET_ITEM:			dwSize = sizeof (cmd_exg_pocket_item);			break;
	case MOVE_POCKET_ITEM:			dwSize = sizeof (cmd_move_pocket_item);			break;
	case EXG_IVTR_POCKET_ITEM:		dwSize = sizeof (cmd_exg_pocket_ivtr);			break;
	case IVTR_ITEM_TO_POCKET:		dwSize = sizeof (cmd_ivtr_item_to_pocket);		break;
	case POCKET_ITEM_TO_IVTR:		dwSize = sizeof (cmd_pocket_item_to_ivtr);		break;
	case POCKET_ITEM_ALL_TO_IVTR:	dwSize = 0;										break;
	case CHANGE_POCKET_SIZE:		dwSize = sizeof (cmd_change_pocket_size);		break;
	case NOTIFY_FASHION_HOTKEY:		dwSize = (DWORD)(-2);							break;
	case EXG_FASHION_ITEM:			dwSize = sizeof (cmd_exg_fashion_item);			break;	
	case EXG_IVTR_FASHION:			dwSize = sizeof (cmd_exg_ivtr_fashion);			break;									
	case EXG_EQUIP_FASHION:			dwSize = sizeof (cmd_exg_equ_fashion);			break;
	case HOTKEY_CHANGE_FASHION:		dwSize = sizeof (hotkey_change_fashion);		break;
	case PEEP_INFO:					dwSize = sizeof (peep_info);					break;
	case PLAYER_BONUS:				dwSize = sizeof (player_bonus);					break;
	case SELF_SJBATTLE_INFO:		dwSize = sizeof (self_sjbattle_info);			break;
	case GET_SJBATTLE_INFO:			dwSize = (DWORD)(-2);							break;
	case GET_ARENA_INFO:			dwSize = (DWORD)(-2);							break;
	case SJBATTLE_SCORE_CHANGE:		dwSize = sizeof(sjbattle_score_change);			break;
	case PLAYER_CHANGE_FACE:		dwSize = sizeof(player_change_face);			break;
	case CROSSSERVER_BATTLE_SCORE:  dwSize = sizeof(crossserver_battle_score);		break;
	case CRSSVR_TEAM_BATTLE_INFO:	dwSize = (DWORD)(-2);							break;		//Added 2011-05-19.
	case PLAYER_ACHIEVEMENT:        dwSize = (DWORD)(-2);							break;        
    case PLAYER_ACHIEVEMENT_FINISH: dwSize = sizeof(player_achievement_finish);		break;               
	case PLAYER_ACHIEVEMENT_ACTIVE: dwSize = (DWORD)(-2);							break;         
	case PLAYER_PREMISS_DATA:       dwSize = (DWORD)(-2);							break;   
    case PLAYER_PREMISS_FINISH:     dwSize = sizeof(player_premiss_finish);			break;
	case PLAYER_ACHIEVEMENT_MAP:    dwSize = (DWORD)(-2);							break;
	case ACHIEVEMENT_MESSAGE:       dwSize = (DWORD)(-2);							break;
	case SELF_INSTANCING_BATTLE_INFO:dwSize = sizeof(self_instancing_battle_info);  break;
	case INSTANCING_BATTLE_INFO:	dwSize = (DWORD)(-2);							break;
	case OBJECT_NOTIFY_RAID_POS:	dwSize = sizeof(cmd_notify_raid_pos);			break;		//Added 2011-07-27.
	case ENTER_RAID_COUNT:			dwSize = (DWORD)(-2);							break;		//Added 2011-07-27.
	case OBJECT_BE_MOVED:			dwSize = sizeof(cmd_object_be_moved);			break;		//Added 2011-07-27.
	case PLAYER_PULL_STATE:			dwSize = sizeof(cmd_player_pull_state);			break;		//Added 2011-07-28.
	case PLAYER_BE_PULLED_INFO:		dwSize = sizeof(cmd_player_be_pulled_info);		break;		//Added 2011-07-28.
	case RAID_PLAYER_INFO:			dwSize = (DWORD)(-2);							break;		//Added 2011-08-03.
	case SKILL_CONTINUE:			dwSize = sizeof(skill_continue);				break;
	case MAGIC_DURATION_DECREASE:	dwSize = sizeof(magic_duration_decrease);		break;
	case MAGIC_EXP_NOTIFY:			dwSize = sizeof(magic_exp_notify);				break;
	case TRANSFIGURE_SKILL_DATA:
		CHECK_VALID(cmd_skill_data);
		break;
	case COMBINE_SKILL_DATA:
		CHECK_VALID(cmd_skill_elems_data);
		break;

	case ADDON_PERMANENT_SKILL:		dwSize = sizeof(addon_permanent_skill);			break;		//Added 2012-02-20.
	case ADDON_PERMANENT_SKILL_DATA:															//Added 2012-02-20.
		CHECK_VALID(addon_permanent_skill_data);
		break;
	case PLAYER_ENTER_CARRIER:		dwSize = sizeof(cmd_player_enter_carrier);		break;
	case PLAYER_LEAVE_CARRIER:		dwSize = sizeof(cmd_player_leave_carrier);		break;
	case PLAYER_MOVE_ON_CARRIER:	dwSize = sizeof(cmd_player_move_on_carrier);	break;
	case PLAYER_STOP_ON_CARRIER:	dwSize = sizeof(cmd_player_stop_on_carrier);	break;
	case SKILL_PROFICIENCY:			dwSize = sizeof(cmd_skill_proficiency);			break;
	case CIRCLE_INFO_NOTIFY:		dwSize = sizeof(circle_info_notify);			break;
	case DELIVERY_EXP_NOTIFY:		dwSize = sizeof(receive_deliveryd_exp);			break;

	case LITTLEPET_INFO:			dwSize = sizeof(littlepet_info);				break;
	case RUNE_INFO:					dwSize = sizeof(rune_info);						break;
	case RUNE_REFINE_RESULT:		dwSize = sizeof(rune_refine_result);			break;
	case TOWER_LEVEL_PASS:			dwSize = sizeof(tower_level_pass);				break;
	case FILL_PLATFORM_MASK:		dwSize = sizeof(fill_platform_mask);			break;
	case PK_BET_DATA:				dwSize = sizeof(pk_bet_data);					break;
	case OBJECT_START_SPECIAL_MOVE:	dwSize = sizeof(cmd_object_start_special_move);	break;		//Added 2012-06-27.
	case OBJECT_SPECIAL_MOVE:		dwSize = sizeof(cmd_object_special_move);		break;		//Added 2012-06-27.
	case OBJECT_STOP_SPECIAL_MOVE:	dwSize = sizeof(cmd_object_stop_special_move);	break;		//Added 2012-06-27.
	case COLLISION_PLAYER_POS:		dwSize = sizeof(collision_player_pos);			break;
	case REPURCHASE_INV_DATA:
		//	CHECK_VALID(repurchase_inv_data)
		dwSize = (DWORD)(-2);
		break;
	case EXCHANGE_MOUNT_WING_ITEM:	dwSize = sizeof(exchange_mount_wing_item);	break;						//Added 2012-08-03.
	case EXCHANGE_INVENTORY_MOUNTWING_ITEM:	dwSize = sizeof(exchange_Inventory_mountwing_item);	break;		//Added 2012-08-03.
	case EXCHANGE_EQUIPMENT_MOUNTWING_ITEM:	dwSize = sizeof(exchange_equipment_mountwing_item);	break;		//Added 2012-08-03.
	case PLAYER_CHANGE_MOUNTWING_INV_SIZE:	dwSize = sizeof(player_change_mountwing_inv_size);	break;		//Added 2012-08-03.								break;
		
	case CHANGE_FACTION_NAME:
		CHECK_VALID(cmd_change_faction_name);
		break;

	case UI_TRANSFER_OPENED_NOTIFY: dwSize = (DWORD)(-2); break;
	case TALISMAN_REFINESKILL:		dwSize = sizeof(talisman_refineskill); break;

	case NOTIFY_ASTROLOGY_ENERGY:	dwSize = sizeof(cmd_notify_astrology_energy);	break;
	case ASTROLOGY_UPGRADE_RESULT:	dwSize = sizeof(cmd_astrology_upgrade_result);	break;
	case ASTROLOGY_DESTROY_RESULT:	dwSize = sizeof(cmd_astrology_destory_result);	break;
	case COLLISION_RAID_RESULT:		dwSize = sizeof(collision_raid_result);			break;
	case COLLISION_RAID_INFO:		dwSize = sizeof(collision_raid_info);			break;
	case ENTER_KINGDOM:             dwSize = (DWORD)(-2); break;
	case LEAVE_KINGDOM:             dwSize = sizeof(leave_kingdom); break;
	case KINGDOM_HALF_END:          dwSize = sizeof(kingdom_half_end);break;
    case KINGDOM_END_RESULT:        dwSize = sizeof(kingdom_end_result);break;
	case KINGDOM_KEY_NPC_INFO:      dwSize = sizeof(kingdom_key_npc_info);break;
	case KINGDOM_MAFIA_INFO:        dwSize = (DWORD)(-2); break;
	case PLAYER_BATH_INVITE:        dwSize = sizeof(player_bath_invite);break;
	case PLAYER_BATH_INVITE_REPLAY: dwSize = sizeof(player_bath_invite_reply);break;
	case PLAYER_BATH_START:         dwSize = sizeof(player_bath_start);break;
	case PLAYER_BATH_STOP:          dwSize = sizeof(player_bath_stop);break;
	case ENTER_BATH_WORLD:          dwSize = sizeof(enter_bath_world);break;
	case LEAVE_BATH_WORLD:          dwSize = 0;break;
	case BATH_GAIN_ITEM:            dwSize = sizeof(bath_gain_item);break;
	case KINGDOM_GATHER_MINE_START: dwSize = sizeof(kingdom_gather_mine_start);break;
	case MULTI_EXP_MODE:            dwSize = sizeof(multi_exp_mode);break;
	case ANCIENT_RAID_LEVEL_START:  dwSize = sizeof(ancient_raid_level_start);	break;
	case ANCIENT_RAID_LEVEL_END:    dwSize = sizeof(ancient_raid_level_end);	break;
	case ANCIENT_RAID_LEVEL_RESULT: dwSize = sizeof(ancient_raid_level_result); break;
	case ANCIENT_RAID_LEVEL_INFO:	dwSize = sizeof(ancient_raid_level_info);   break;
	case SALE_PROMOTION_INFO:		dwSize = (DWORD)-2;	break;
	case EVENT_START:				dwSize = sizeof(event_start);               break;
	case EVENT_STOP:				dwSize = sizeof(event_stop);				break;
	case EVENT_INFO_ONLINE:			dwSize = (DWORD)(-2);						break;		
	case CASH_GIFT_IDS:				dwSize = sizeof(cash_gift_ids);				break;
	case NOTIFY_CASH_GIFT:			dwSize = sizeof(notify_cash_gift);			break;
	case PLAYER_PROPADD:			dwSize = sizeof(player_propadd);			break;
	case PLAYER_GET_PROPADD_ITEM:	dwSize = sizeof(player_get_propadd_item);	break;
	case PLAYER_EXTRA_EQUIP_EFFECT:	dwSize = sizeof(player_extra_equip_effect);	break;
	case FUWEN_COMPOSE_INFO:		dwSize = sizeof(fuwen_compose_info);		break;
	case FUWEN_COMPOSE_END:			dwSize = 0;									break;
	case FUWEN_INSTALL_RESULT:		dwSize = sizeof(fuwen_install_result);		break;
	case FUWEN_UNINSTALL_RESULT:	dwSize = sizeof(fuwen_uninstall_result);	break;
	case CSFLOW_BATTLE_PLAYER_INFO: dwSize = sizeof(cs_flow_player_info);		break;
	case CSFLOW_BATTLE_INFO:		dwSize = (DWORD)(-2);						break;
	case EXCHANGE_ITEM_LIST_INFO:   dwSize = (DWORD)(-2);						break;
	case WEB_ORDER_INFO:			dwSize = (DWORD)(-2);						break;
	case BATTLE_OFF_LINE_INFO:		dwSize = (DWORD)(-2);						break;
	case CONTROL_TRAP_INFO:			dwSize = sizeof(control_trap_info);			break;
	case NPC_INVISIBLE:				dwSize = sizeof(npc_invisible);				break;
	case PUPPET_FORM:				dwSize = sizeof(puppet_form);				break;
	case TELEPORT_SKILL_INFO:		dwSize = sizeof(teleport_skill_info);		break;
	case PLAYER_START_INTERACT:		dwSize = sizeof(cmd_player_start_interact);	break;
	case PLAYER_IN_INTERACT:		dwSize = sizeof(cmd_player_in_interact);	break;
	case PLAYER_END_INTERACT:		dwSize = sizeof(cmd_player_end_interact);	break;
	case PLAYER_FINISH_INTERACT:	dwSize = sizeof(cmd_player_finish_interact);break;
	case PLAYER_CANCEL_INTERACT:	dwSize = sizeof(cmd_player_cancel_interact);break;
	case PHASE_INFO:				dwSize = (DWORD)(-2);						break;
	case PHASE_INFO_CHANGE:			dwSize = sizeof(phase_info_change);			break;
	case PLAYER_QILIN_INVITE:		dwSize = sizeof(player_qilin_invite);		break;
	case PLAYER_QILIN_INVITE_REPLY:	dwSize = sizeof(player_qilin_invite_reply); break;
	case PLAYER_QILIN_START:		dwSize = sizeof(player_qilin_start);		break;
	case PLAYER_QILIN_STOP:			dwSize = sizeof(player_qilin_stop);			break;
	case PLAYER_QILIN_DISCONNECT:	dwSize = sizeof(player_qilin_disconnet);	break;
	case PLAYER_QILIN_RECONNECT:	dwSize = sizeof(player_qilin_reconnect);	break;
	case FASHION_COLORANT_PRODUCE:	dwSize = sizeof(fashion_colorant_produce);break;		// 2013-8-5 add 注册消息结构 by zy 合成染色
	case FASHION_COLORANT_USE:	dwSize = sizeof(fashion_colorant_use);break;// 2013-8-5. add 注册消息结构 by zy 染色合成
	case FAC_BUILDING_ADD:	dwSize = sizeof(fac_building_add);break;		// 2013-8-5 add 注册消息结构 by zy 帮派建筑添加
	case FAC_BUILDING_UPGRADE_FAC:	dwSize = sizeof(fac_building_upgrade);break;// 2013-8-5. add 注册消息结构 by zy 帮派建筑升级
	case FAC_BUILDING_REMOVE:	dwSize = sizeof(fac_building_remove);break;		// 2013-8-5 add 注册消息结构 by zy 帮派建筑删除
	case FAC_BUILDING_COMPLETE:	dwSize = sizeof(fac_building_complete);break;// 2013-8-5. add 注册消息结构 by zy 帮派完整
	case PLAYER_FAC_BASE_INFO:	dwSize = (DWORD)(-2);break;// 2013-8-5. add 注册消息结构 by zy 帮派完整
	case DUNGEON_PROGRESS:			dwSize = (DWORD)(-2);			break;
	case FAC_BASE_PROP_CHANGE: 	dwSize = (DWORD)(-2);break;// 2013-8-5. add 注册消息结构 by zy 帮派完整
	case COMBINE_MINE_STATE_CHANGE: 	dwSize = sizeof(combine_mine_state_change);break;// 2013-8-5. add 注册消息结构 by zy 帮派完整
	case GET_FACBASE_MALL_INFO: 	dwSize = (DWORD)(-2);break;// 2013-8-5. add 注册消息结构 by zy 帮派完整
	case GET_FACTION_COUPON: 	dwSize = sizeof(get_faction_coupon);break;// 2013-8-5. add 注册消息结构 by zy 帮派完整
	case GET_FACBASE_MONEY_ITEMS_INFO: 	dwSize = (DWORD)(-2);break;// 2013-8-5. add 注册消息结构 by zy 帮派完整
	case PLAYER_FACBASE_AUCTION: 	dwSize = (DWORD)(-2);break;// 2013-8-5. add 注册消息结构 by zy帮派拍卖行
	case FACBASE_AUCTION_UPDATE: 	dwSize = (DWORD)(-2);break;// 2013-8-5. add 注册消息结构 by zy帮派拍卖行
	case FAC_COUPON_RETURN:			dwSize = sizeof(fac_coupon_return);break;// 2013-8-5. add 注册消息结构 by zy帮派退回金劵
	case GET_FACBASE_CASH_ITEM_SUCCESS: dwSize = sizeof(get_facbase_cash_item_success);break;// 2013-8-5. add 注册消息结构 by zy元宝商城购买成功
	case FACBASE_AUCTION_ADD_HISTORY: dwSize = sizeof(facbase_auction_add_history);break;// 2013-8-5. add 注册消息结构 by zy元宝商城购买成功
	case FACBASE_MSG_UPDATE: dwSize = (DWORD)(-2);break;// 2013-8-5. add 注册消息结构 by zy元宝商城购买成功
	case PLAYER_CHANGE_PUPPET_FORM:	 dwSize = sizeof(player_change_puppet_form); break;
	case BLOW_OFF:					 dwSize = sizeof(blow_off); break;	
	case BE_TAUNTED2:				 dwSize = sizeof(be_taunted2); break;
	case INVITE_ACTIVE_EMOTE:		dwSize = sizeof(invite_active_emote); break;
	case REPLY_ACTIVE_EMOTE:		dwSize = sizeof(reply_active_emote); break;
	case PLAY_ACTIVE_EMOTE:			dwSize = sizeof(play_active_emote); break;
	case STOP_ACTIVE_EMOTE:			dwSize = sizeof(stop_active_emote); break;
	case BATTLE_DUNGEON_SSK_INFO:	dwSize = sizeof(battle_dungeon_ssk_info);	break;
	case OBJECT_TRY_CHARGE:			dwSize = sizeof(object_try_charge); break;
	case OBJECT_BE_CHARGE_TO:		dwSize = sizeof(object_be_charge_to); break;
	case PLAYER_START_TRAVEL_AROUND:		dwSize = sizeof(player_start_travel_around); break;
	case PLAYER_STOP_TRAVEL_AROUND:			dwSize = sizeof(player_stop_travel_around);		break;
	case PLAYER_GET_SUMMON_PETPROP:		dwSize = sizeof(player_get_summon_petprop); break;
	case NOTIFY_BLOODPOOL_STATUS:	dwSize = sizeof(notify_bloodpool_status); break;
	default:
		break;
	}

	return dwSize;
}

//	Game data handler
void CECGameSession::ProcessGameData(const GNET::Octets& Data)
{
	using namespace S2C;

	DWORD dwDataSize = Data.size();

	if (dwDataSize < sizeof(cmd_header))
	{
		assert(0);
		return;
	}

	dwDataSize -= sizeof(cmd_header);

	//	Get common command header
	BYTE* pDataBuf = (BYTE*)Data.begin();
	cmd_header* pCmdHeader = (cmd_header*)pDataBuf;
	pDataBuf += sizeof (cmd_header);

	//	Check command size
	DWORD dwCmdSize = CalcS2CCmdDataSize(pCmdHeader->cmd, pDataBuf, dwDataSize);

	// log the command and data
	LogS2CCmdData(pCmdHeader->cmd, (BYTE *)Data.begin(), Data.size());

	//	Size == -1 means we haven't handled this command
	if (dwCmdSize == (DWORD)(-1))
	{
		g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, _AL("SERVER - Unknown server command (%d)"), pCmdHeader->cmd);
		return;
	}
	else if (dwCmdSize != (DWORD)(-2))
	{
		ASSERT(dwCmdSize == dwDataSize);
		if (dwCmdSize != dwDataSize)
			return;
	}

	CECGameRun* pGameRun = g_pGame->GetGameRun();


	switch (pCmdHeader->cmd)
	{
	case PLAYER_INFO_2:
	case PLAYER_INFO_3:
	case PLAYER_INFO_4:
	case PLAYER_INFO_2_LIST:
	case PLAYER_INFO_3_LIST:
	case PLAYER_INFO_23_LIST:

		break;

	case MATTER_INFO_LIST:
	
		pGameRun->PostMessage(MSG_MM_MATTERINFO, MAN_MATTER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	
	case NOTIFY_HOSTPOS:
	case OBJECT_NOTIFY_RAID_POS:	//跳入副本地图使用了新的协议消息，但仍然沿用了统一的处理函数Added 2011-07-29.
		pGameRun->PostMessage(MSG_HST_GOTO, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case MATTER_ENTER_WORLD:
		
		pGameRun->PostMessage(MSG_MM_MATTERENTWORLD, MAN_MATTER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;

	case COMBINE_MINE_STATE_CHANGE:

		pGameRun->PostMessage(MSG_MM_MINECHANGESTATE, MAN_MATTER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;

	case HOST_START_ATTACK:

		pGameRun->PostMessage(MSG_HST_STARTATTACK, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case HOST_STOPATTACK:

		pGameRun->PostMessage(MSG_HST_STOPATTACK, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case HOST_ATTACKRESULT:
		
		pGameRun->PostMessage(MSG_HST_ATKRESULT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case ERROR_MESSAGE:
	{
		cmd_error_msg* pCmd = (cmd_error_msg*)pDataBuf;
		if (pCmd->iMessage)
		{
			const wchar_t* szMsg = m_ErrorMsgs.GetWideString(pCmd->iMessage);
			if (!szMsg)
				g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, _AL("SERVER - unknown error !"));
			else if (pCmd->iMessage != 2 && !g_pGame->GetGameRun()->GetAnmSystem()->IsPlaying())
				g_pGame->GetGameRun()->AddChatMessage(szMsg, GP_CHAT_MISC);
		}

		if (pCmd->iMessage == 2)
		{
			//	Attack target is too far
			pGameRun->PostMessage(MSG_HST_TARGETISFAR, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		}
		else if (pCmd->iMessage == 20)
		{
			//	Failed to cast skill
			pGameRun->PostMessage(MSG_PM_CASTSKILL, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		}
		else if (pCmd->iMessage == 190)
		{
			pGameRun->PostMessage(MSG_HOST_SELL_ROLE_SUCCESS, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		}
		else if(pCmd->iMessage == 230)
			pGameRun->PostMessage(MSG_HST_EXCHANGE_ITEM_LIST_INFO, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);

		break;
	}
	case HOST_ATTACKED:

		pGameRun->PostMessage(MSG_HST_ATTACKED, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;
		
	case PLAYER_DIED:

		pGameRun->PostMessage(MSG_PM_PLAYERDIED, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case HOST_DIED:

		pGameRun->PostMessage(MSG_HST_DIED, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_REVIVE:

		pGameRun->PostMessage(MSG_PM_PLAYERREVIVE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PICKUP_MONEY:

		pGameRun->PostMessage(MSG_HST_PICKUPMONEY, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PICKUP_ITEM:
	case HOST_OBTAIN_ITEM:
	case PRODUCE_ONCE:
	case TASK_DELIVER_ITEM:

		pGameRun->PostMessage(MSG_HST_PICKUPITEM, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;
		
	case PUT_ITEM:
		
		pGameRun->PostMessage(MSG_HST_ITEMOPERATION, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case OBJECT_STOP_MOVE:
	{
		cmd_object_stop_move* pCmd = (cmd_object_stop_move*)pDataBuf;
		if (ISPLAYERID(pCmd->id))
			pGameRun->PostMessage(MSG_PM_PLAYERSTOPMOVE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		else if (ISNPCID(pCmd->id))
			pGameRun->PostMessage(MSG_NM_NPCSTOPMOVE, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
	
		break;
	}

	//-------------------------------------------------------------------------
	//Added 2012-06-27.
	case OBJECT_START_SPECIAL_MOVE:
	{
		cmd_object_start_special_move* pCmd = (cmd_object_start_special_move*)pDataBuf;
		if ( ISPLAYERID(pCmd->id) )
		{
			pGameRun->PostMessage(MSG_PM_SPECIAL_MOVE_OPT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		}
		break;
	}
	case OBJECT_SPECIAL_MOVE:
	{
		cmd_object_special_move* pCmd = (cmd_object_special_move*)pDataBuf;
		if ( ISPLAYERID(pCmd->id) )
		{
			pGameRun->PostMessage(MSG_PM_SPECIAL_MOVE_OPT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		}
		break;
	}
	case OBJECT_STOP_SPECIAL_MOVE:
	{
		cmd_object_stop_special_move* pCmd = (cmd_object_stop_special_move*)pDataBuf;
		if ( ISPLAYERID(pCmd->id) )
		{
			pGameRun->PostMessage(MSG_PM_SPECIAL_MOVE_OPT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		}
		break;
	}
	case COLLISION_PLAYER_POS:
	{
		collision_player_pos* pCmd = (collision_player_pos*)pDataBuf;
		if ( ISPLAYERID(pCmd->id) )
		{
			pGameRun->PostMessage(MSG_PM_SPECIAL_MOVE_OPT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		}
		break;
	}
	//-------------------------------------------------------------------------

	case RECEIVE_EXP:

		pGameRun->PostMessage(MSG_HST_RECEIVEEXP, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case LEVEL_UP:
	{
		cmd_level_up* pCmd = (cmd_level_up*)pDataBuf;

		if (ISPLAYERID(pCmd->idPlayer))
			pGameRun->PostMessage(MSG_PM_PLAYERLEVELUP, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);

		break;
	}

	case SELECT_TARGET:
	case UNSELECT:

		pGameRun->PostMessage(MSG_HST_SELTARGET, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case OWN_ITEM_INFO:
	case EMPTY_ITEM_SLOT:
	case EQUIP_SPIRIT_POWER_DECREASE:
	case MAGIC_DURATION_DECREASE:
	case MAGIC_EXP_NOTIFY:

		pGameRun->PostMessage(MSG_HST_OWNITEMINFO, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;

	case OWN_IVTR_DATA:
	case OWN_IVTR_DETAIL_DATA:
	case GET_OWN_MONEY:
	case REPURCHASE_INV_DATA:

		pGameRun->PostMessage(MSG_HST_IVTRINFO, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;

	case EXG_IVTR_ITEM:
	case MOVE_IVTR_ITEM:
	case PLAYER_DROP_ITEM:
	case EXG_EQUIP_ITEM:
	case EQUIP_ITEM:
	case MOVE_EQUIP_ITEM:
	case UNFREEZE_IVTR_SLOT:

		pGameRun->PostMessage(MSG_HST_ITEMOPERATION, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;
		
	case RECORD_CHECK_RESULT:

		pGameRun->PostMessage(MSG_HST_CHECK_RECORD, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case OWN_EXT_PROP:

		pGameRun->PostMessage(MSG_HST_OWNEXTPROP, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_EXT_PROP_BASE:
	case PLAYER_EXT_PROP_MOVE:
	case PLAYER_EXT_PROP_ATK:
	case PLAYER_EXT_PROP_DEF:

		pGameRun->PostMessage(MSG_PM_PLAYEREXTPROP, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case ADD_STATUS_POINT:

		pGameRun->PostMessage(MSG_HST_ADDSTATUSPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case TEAM_LEADER_INVITE:

		pGameRun->PostMessage(MSG_HST_TEAMINVITE, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case TEAM_REJECT_INVITE:

		pGameRun->PostMessage(MSG_HST_TEAMREJECT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case TEAM_JOIN_TEAM:

		pGameRun->PostMessage(MSG_HST_JOINTEAM, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case TEAM_MEMBER_LEAVE:

		pGameRun->PostMessage(MSG_PM_LEAVETEAM, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case TEAM_LEADER_CACEL_PARTY:
		
		break;

	case TEAM_LEAVE_PARTY:

		pGameRun->PostMessage(MSG_HST_LEAVETEAM, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case TEAM_NEW_MEMBER:

		pGameRun->PostMessage(MSG_HST_NEWTEAMMEM, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;
		
	case TEAM_MEMBER_DATA:

		pGameRun->PostMessage(MSG_HST_TEAMMEMBERDATA, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case TEAM_MEMBER_POS:

		pGameRun->PostMessage(MSG_HST_TEAMMEMPOS, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case TEAM_ASK_TO_JOIN:
		
		pGameRun->PostMessage(MSG_HST_ASKTOJOINTEAM, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case EQUIP_DATA:
	case EQUIP_DATA_CHANGED:
	case CLONE_PLAYER_EQUIP:

		pGameRun->PostMessage(MSG_PM_PLAYEREQUIPDATA, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;

	case EQUIP_DAMAGED:

		pGameRun->PostMessage(MSG_HST_EQUIPDAMAGED, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;
		
	case TEAM_MEMBER_PICKUP:

		pGameRun->PostMessage(MSG_HST_TEAMMEMPICKUP, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case NPC_GREETING:

		pGameRun->PostMessage(MSG_HST_NPCGREETING, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case REPAIR_ALL:
	case REPAIR:

		pGameRun->PostMessage(MSG_HST_REPAIR, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PURCHASE_ITEM:

		pGameRun->PostMessage(MSG_HST_PURCHASEITEMS, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case ITEM_TO_MONEY:

		pGameRun->PostMessage(MSG_HST_ITEMTOMONEY, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case SPEND_MONEY:

		pGameRun->PostMessage(MSG_HST_SPENDMONEY, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case GAIN_MONEY_IN_TRADE:
	case GAIN_ITEM_IN_TRADE:
	case GAIN_MONEY_AFTER_TRADE:
	case GAIN_ITEM_AFTER_TRADE:

		pGameRun->PostMessage(MSG_HST_GAINITEM, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case HOST_USE_ITEM:

		pGameRun->PostMessage(MSG_HST_USEITEM, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_USE_ITEM_WITH_ARG:

		pGameRun->PostMessage(MSG_HST_USEITEMWITHDATA, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case SKILL_DATA:

		pGameRun->PostMessage(MSG_HST_SKILLDATA, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case EMBED_ITEM:

		pGameRun->PostMessage(MSG_HST_EMBEDITEM, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case CLEAR_TESSERA:

		pGameRun->PostMessage(MSG_HST_CLEARTESSERA, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case COST_SKILL_POINT:

		pGameRun->PostMessage(MSG_HST_COSTSKILLPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case LEARN_SKILL:
		
		pGameRun->PostMessage(MSG_HST_LEARNSKILL, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case OBJECT_TAKEOFF:
	{
		cmd_object_takeoff* pCmd = (cmd_object_takeoff*)pDataBuf;
		if (ISPLAYERID(pCmd->object_id))
			pGameRun->PostMessage(MSG_PM_PLAYERFLY, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		
		break;
	}
	case OBJECT_LANDING:
	{
		cmd_object_landing* pCmd = (cmd_object_landing*)pDataBuf;
		if (ISPLAYERID(pCmd->object_id))
			pGameRun->PostMessage(MSG_PM_PLAYERFLY, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		
		break;
	}
	case HOST_RUSH_FLY:
		
		pGameRun->PostMessage(MSG_PM_PLAYERFLY, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case FLYSWORD_TIME:
	
		pGameRun->PostMessage(MSG_HST_FLYSWORDTIME, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case OBJECT_CAST_SKILL:
	case OBJECT_CAST_INSTANT_SKILL:
	case OBJECT_CAST_POS_SKILL:
	{
		if (pGameRun->IsAnmPlaying())
			break;
		cmd_object_cast_skill* pCmd = (cmd_object_cast_skill*)pDataBuf;
		if (ISPLAYERID(pCmd->caster))
			pGameRun->PostMessage(MSG_PM_CASTSKILL, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		else if (ISNPCID(pCmd->caster))
			pGameRun->PostMessage(MSG_NM_NPCCASTSKILL, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd);

		break;
	}
	
	case PLAYER_STOP_SKILL:	//通知ElsePlayer停止当前技能Added 2011-08-30.
	{
		pGameRun->PostMessage(MSG_PM_CASTSKILL, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;
	}

	case SKILL_INTERRUPTED:
	{
		cmd_skill_interrupted* pCmd = (cmd_skill_interrupted*)pDataBuf;
		if (ISPLAYERID(pCmd->caster))
			pGameRun->PostMessage(MSG_PM_CASTSKILL, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		else if (ISNPCID(pCmd->caster))
			pGameRun->PostMessage(MSG_NM_NPCCASTSKILL, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd);

		break;
	}
	case SKILL_CONTINUE:
	{
		skill_continue* pCmd = (skill_continue*)pDataBuf;
		if (ISPLAYERID(pCmd->caster))
			pGameRun->PostMessage(MSG_PM_CASTSKILL, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		else if (ISNPCID(pCmd->caster))
			pGameRun->PostMessage(MSG_NM_NPCCASTSKILL, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd);

		break;
	}
	case PLAYER_CHARGE:
	{
		player_charge* pCmd = (player_charge*)pDataBuf;
		if (ISPLAYERID(pCmd->id))
			pGameRun->PostMessage(MSG_PM_CASTSKILL, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		else if (ISNPCID(pCmd->id))
			pGameRun->PostMessage(MSG_NM_NPCATKRESULT, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;
	}
	case HOST_STOP_SKILL:
	case SELF_SKILL_INTERRUPTED:
	case SKILL_PERFORM:
	case SKILL_PROFICIENCY:

		pGameRun->PostMessage(MSG_PM_CASTSKILL, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case NPC_SERVICE_CONTENT:
	case RENEW:

		break;

	case PRODUCE_START:
	case PRODUCE_END:
	case PRODUCE_NULL:

		pGameRun->PostMessage(MSG_HST_PRODUCEITEM, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case DECOMPOSE_START:
	case DECOMPOSE_END:

		pGameRun->PostMessage(MSG_HST_BREAKITEM, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case TASK_DATA:
	case TASK_VAR_DATA:

		pGameRun->PostMessage(MSG_HST_TASKDATA, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case OBJECT_START_USE:
	case OBJECT_CANCEL_USE:
	case OBJECT_USE_ITEM:
	case OBJECT_START_USE_T:
		
		pGameRun->PostMessage(MSG_PM_PLAYERUSEITEM, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case OBJECT_USE_ITEM_WITH_ARG:

		pGameRun->PostMessage(MSG_PM_USEITEMWITHDATA, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case OBJECT_SIT_DOWN:
	case OBJECT_STAND_UP:

		pGameRun->PostMessage(MSG_PM_PLAYERSITDOWN, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case OBJECT_DO_EMOTE:
	case OBJECT_EMOTE_RESTORE:

		pGameRun->PostMessage(MSG_PM_PLAYERDOEMOTE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case OBJECT_ROOT:
	{
		cmd_object_root* pCmd = (cmd_object_root*)pDataBuf;
		if (ISPLAYERID(pCmd->id))
			pGameRun->PostMessage(MSG_PM_PLAYERROOT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		else if (ISNPCID(pCmd->id))
			pGameRun->PostMessage(MSG_NM_NPCROOT, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd);

		break;
	}
	case HOST_NOTIFY_ROOT:	
	case HOST_DISPEL_ROOT:

		pGameRun->PostMessage(MSG_HST_ROOTNOTIFY, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case OBJECT_ATTACK_RESULT:
	{
		cmd_object_atk_result* pCmd = (cmd_object_atk_result*)pDataBuf;
		if (ISPLAYERID(pCmd->attacker_id))
			pGameRun->PostMessage(MSG_PM_PLAYERATKRESULT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		else if (ISNPCID(pCmd->attacker_id))
			pGameRun->PostMessage(MSG_NM_NPCATKRESULT, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd);

		break;
	}
	case OBJECT_STARTATTACK:
	{
		cmd_object_start_attack* pCmd = (cmd_object_start_attack*)pDataBuf;
		if (ISPLAYERID(pCmd->idAttacker))
			pGameRun->PostMessage(MSG_PM_STARTATTACK, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		else if (ISNPCID(pCmd->idAttacker))
			pGameRun->PostMessage(MSG_NM_STARTATTACK, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd);

		break;
	}
	case ENCHANT_RESULT:
	{
		cmd_enchant_result* pCmd = (cmd_enchant_result*)pDataBuf;
		if (ISPLAYERID(pCmd->caster))
			pGameRun->PostMessage(MSG_PM_ENCHANTRESULT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		else if (ISNPCID(pCmd->caster))
			pGameRun->PostMessage(MSG_NM_ENCHANTRESULT, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd);

		break;
	}
	case BE_HURT:
	case HURT_RESULT:

		pGameRun->PostMessage(MSG_HST_HURTRESULT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case UPDATE_EXT_STATE:
	{
		cmd_update_ext_state* pCmd = (cmd_update_ext_state*)pDataBuf;
		if (ISPLAYERID(pCmd->id))
			pGameRun->PostMessage(MSG_PM_PLAYEREXTSTATE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		else if (ISNPCID(pCmd->id))
			pGameRun->PostMessage(MSG_NM_NPCEXTSTATE, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd);

		break;
	}
	case ICON_STATE_NOTIFY:
	{
		cmd_icon_state_notify* pCmd = (cmd_icon_state_notify*)pDataBuf;
		if (ISPLAYERID(pCmd->id))
			pGameRun->PostMessage(MSG_PM_PLAYEREXTSTATE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		else if (ISNPCID(pCmd->id))
			pGameRun->PostMessage(MSG_NM_NPCEXTSTATE, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);

		break;
	}
	case PLAYER_SCALE:
	{
		cmd_player_scale* pCmd = (cmd_player_scale*)pDataBuf;
		if ( ISPLAYERID(pCmd->player_id) )
		{
			pGameRun->PostMessage(MSG_PM_PLAYERSCALESHAPE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		}

		break;
	}
	case PLAYER_VIP_AWARD_UPDATE: //Added 2012-01-16.收到VIP奖励等级变化协议
	{
		player_vip_award_update* pCmd = (player_vip_award_update*)pDataBuf;
		if ( ISPLAYERID(pCmd->role_id) )
		{
			pGameRun->PostMessage( MSG_PM_PLAYERVIPAWARDUPDATE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		}
		break;
	}
	case COLLISION_SPECIAL_STATE_INFO:	//Added 2012-08-02.收到碰撞技能攻击效果协议
	{
		cmd_collision_special_state_info* pCmd = (cmd_collision_special_state_info*)pDataBuf;
		if ( ISPLAYERID(pCmd->role_id) )
		{
			pGameRun->PostMessage( MSG_PM_COLLISION_SPECIAL_STATE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		}
		break;
	}
	case PLAYER_DARK_LIGHT_VALUE:	//Added 2012-08-16. 收到轩辕关于光暗值以及相关状态时的协议
	case PLAYER_DARK_LIGHT_STATE:	
	{
		pGameRun->PostMessage( MSG_PM_XY_DARK_LIGHT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;
	}

	case DIR_VISIBLE_STATE:		//Added 2012-09-05. 处理十字区域技能释放后的状态效果
	{
		cmd_dir_visible_state* pCmd = (cmd_dir_visible_state*)pDataBuf;
		if (ISPLAYERID(pCmd->role_id))
		{
			pGameRun->PostMessage( MSG_PM_DIR_VISIBLE_STATE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		}
		else if ( ISNPCID(pCmd->role_id) )
		{
			pGameRun->PostMessage(MSG_NM_DIR_VISIBLE_STATE, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		}
		break;
	}

	case PLAYER_REMEDY_METEMPSYCHOSIS_LEVEL:	//2012-10-17 add  添加服务器回执消息 by zy
	{
		pGameRun->PostMessage( MSG_HST_PLAYER_REMEDY_METEMPSYCHOSIS_LEVEL, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd );
		break;		
	}
	
	case LIVENESS_NOTIFY:	//2012-11-7 add  添加服务器回执消息 by zy
		{
			pGameRun->PostMessage( MSG_HST_LIVENESS_NOTIFY, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd );
			break;		
		}
	case PLAYER_WING_CHANGE_COLOR:	//Added 2012-08-20.
	{
		pGameRun->PostMessage( MSG_PM_WING_CHANGE_COLOR, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;
	}

	case PLAYER_DARK_LIGHT_SPIRIT:

		pGameRun->PostMessage( MSG_PM_XY_DARK_LIGHT_SPIRIT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;

	case MIRROR_IMAGE_CNT:

		pGameRun->PostMessage( MSG_PM_MIRROR_IMAGE_CNT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;

	case ATTACK_ONCE:

		pGameRun->PostMessage(MSG_HST_ATTACKONCE, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_GATHER_START:
	case PLAYER_GATHER_STOP:

		pGameRun->PostMessage(MSG_PM_PLAYERGATHER, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case TRASHBOX_PWD_CHANGED:
	case TRASHBOX_PWD_STATE:
	case TRASHBOX_OPEN:
	case TRASHBOX_CLOSE:
	case TRASHBOX_WEALTH:
	case EXG_TRASH_MONEY:
	case EXG_TRASHBOX_ITEM:
	case MOVE_TRASHBOX_ITEM:
	case EXG_TRASHBOX_IVTR:
	case IVTR_ITEM_TO_TRASH:
	case TRASH_ITEM_TO_IVTR:
	case TRASHBOX_SIZE:
	case MAFIA_TRASHBOX_CAPACITY:

		pGameRun->PostMessage(MSG_HST_TRASHBOXOP, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case OBJECT_DO_ACTION:
	{
		cmd_obj_do_action* pCmd = (cmd_obj_do_action*)pDataBuf;
		if (ISPLAYERID(pCmd->id))
			pGameRun->PostMessage(MSG_PM_PLAYERDOACTION, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);

		break;
	}

	case OBJECT_SKILL_ATTACK_RESULT:
	{
		cmd_object_skill_attack_result* pCmd = (cmd_object_skill_attack_result*)pDataBuf;
		if (ISPLAYERID(pCmd->attacker_id))
			pGameRun->PostMessage(MSG_PM_PLAYERSKILLRESULT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		else if (ISNPCID(pCmd->attacker_id))
			pGameRun->PostMessage(MSG_NM_NPCSKILLRESULT, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd);

		break;
	}
	case HOST_SKILL_ATTACKED:

		pGameRun->PostMessage(MSG_HST_SKILLATTACKED, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_SET_ADV_DATA:
	case PLAYER_CLR_ADV_DATA:
		
		pGameRun->PostMessage(MSG_PM_PLAYERADVDATA, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;
		
	case PLAYER_IN_TEAM:
		
		pGameRun->PostMessage(MSG_PM_PLAYERINTEAM, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case CON_EMOTE_REQUEST:
		
		pGameRun->PostMessage(MSG_PM_DOCONEMOTE, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case DO_CONCURRENT_EMOTE:
		
		pGameRun->PostMessage(MSG_PM_DOCONEMOTE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case MATTER_PICKUP:

		pGameRun->PostMessage(MSG_PM_PICKUPMATTER, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case MAFIA_INFO_NOTIFY:
	case MAFIA_TRADE_START:
	case MAFIA_TRADE_END:
	case CHANGE_FACTION_NAME:

		g_pGame->GetGameRun()->PostMessage(MSG_HST_FACTION, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case TASK_DELIVER_REP:
	case TASK_DELIVER_EXP:
	case TASK_DELIVER_MONEY:
		
		pGameRun->PostMessage(MSG_HST_TASKDELIVER, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case TASK_DELIVER_LEVEL2:

		pGameRun->PostMessage(MSG_PM_PLAYERLEVEL2, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case ITEM_IDENTIFY:
		
		pGameRun->PostMessage(MSG_HST_ITEMIDENTIFY, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_CHGSHAPE:
		
		pGameRun->PostMessage(MSG_PM_PLAYERCHGSHAPE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case ENTER_SANCTUARY:
	case LEAVE_SANCTUARY:

		pGameRun->PostMessage(MSG_HST_SANCTUARY, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case SELF_OPEN_BOOTH:
	case PLAYER_OPEN_BOOTH:
	case PLAYER_CLOSE_BOOTH:
	case PLAYER_BOOTH_INFO:
	case BOOTH_TRADE_SUCCESS:
	case BOOTH_NAME:
	case OPEN_BOOTH_TEST:
	case BOOTH_SELL_ITEM:

		pGameRun->PostMessage(MSG_PM_BOOTHOPT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_START_TRAVEL:
	case HOST_START_TRAVEL:
	case PLAYER_END_TRAVEL:

		pGameRun->PostMessage(MSG_PM_PLAYERTRAVEL, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case GM_INVINCIBLE:
	case GM_INVISIBLE:

		pGameRun->PostMessage(MSG_HST_GMOPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case HOST_CORRECT_POS:

		pGameRun->PostMessage(MSG_HST_CORRECTPOS, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case ACTIVATE_WAYPOINT:
	case WAYPOINT_LIST:	

		pGameRun->PostMessage(MSG_HST_WAYPOINT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case TEAM_INVITE_TIMEOUT:

	//	pGameRun->PostMessage(MSG_HST_TEAMINVITETO, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_PVP_NO_PROTECT:
	case PLAYER_DISABLE_PVP:
	case HOST_PVP_COOLDOWN:
	case HOST_ENABLE_FREEPVP:
	case PVP_COMBAT:

		pGameRun->PostMessage(MSG_PM_PLAYERPVP, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case COOLTIME_DATA:

		pGameRun->PostMessage(MSG_HST_COOLTIMEDATA, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case SKILL_ABILITY:

		pGameRun->PostMessage(MSG_HST_SKILLABILITY, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case BREATH_DATA:
	case HOST_STOP_DIVE:

		pGameRun->PostMessage(MSG_HST_BREATHDATA, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_ENABLE_FASHION:

		pGameRun->PostMessage(MSG_PM_FASHIONENABLE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case INVALID_OBJECT:
	{
		cmd_invalid_object* pCmd = (cmd_invalid_object*)pDataBuf;
		if (ISPLAYERID(pCmd->id))
			pGameRun->PostMessage(MSG_PM_INVALIDOBJECT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		else if (ISNPCID(pCmd->id))
			pGameRun->PostMessage(MSG_NM_INVALIDOBJECT, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		else if (ISMATTERID(pCmd->id))
			pGameRun->PostMessage(MSG_MM_INVALIDOBJECT, MAN_MATTER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);

		break;
	}
	case PLAYER_ENABLE_EFFECT:
	case PLAYER_DISABLE_EFFECT:

		pGameRun->PostMessage(MSG_PM_PLAYEREFFECT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case REVIVAL_INQUIRE:

		pGameRun->PostMessage(MSG_HST_REVIVALINQUIRE, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case SET_COOLDOWN:

		pGameRun->PostMessage(MSG_HST_SETCOOLTIME, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case CHANGE_TEAM_LEADER:

		pGameRun->PostMessage(MSG_HST_CHGTEAMLEADER, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case EXIT_INSTANCE:

		pGameRun->PostMessage(MSG_HST_EXITINSTANCE, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case CHANGE_FACE_START:
	case CHANGE_FACE_END:
		
		pGameRun->PostMessage(MSG_HST_CHANGEFACE, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_CHG_FACE:

		pGameRun->PostMessage(MSG_PM_CHANGEFACE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case SET_MOVE_STAMP:
		
		pGameRun->PostMessage(MSG_HST_SETMOVESTAMP, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case INST_DATA_CHECKOUT:
		
		pGameRun->PostMessage(MSG_INSTCHECKOUT, -1, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case DOUBLE_EXP_TIME:
	case AVAILABLE_DOUBLE_EXP_TIME:
	case MULTI_EXP_MODE:
		
		pGameRun->PostMessage(MSG_DOUBLETIME, -1, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case DUEL_RECV_REQUEST:
	case DUEL_REJECT_REQUEST:
	case DUEL_PREPARE:
	case DUEL_CANCEL:
	case HOST_DUEL_START:
	case DUEL_STOP:
	case PLAYER_DUEL_START:
		
		pGameRun->PostMessage(MSG_PM_PLAYERDUELOPT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case DUEL_RESULT:

		pGameRun->PostMessage(MSG_PM_PLAYERDUELRLT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_BIND_REQUEST:
	case PLAYER_BIND_INVITE:
	case PLAYER_BIND_REQUEST_REPLY:
	case PLAYER_BIND_INVITE_REPLY:
	case PLAYER_BIND_STOP:
		
		pGameRun->PostMessage(MSG_PM_PLAYERBINDOPT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;
		
	case PLAYER_MULTI_BIND_INVITE:
	case PLAYER_MULTI_BIND_INVITE_REPLY:
	case PLAYER_MULTI_BIND_JOIN:
	case PLAYER_MULTI_BIND_LEAVE:
	case PLAYER_MULTI_BIND_KICK:

		pGameRun->PostMessage(MSG_PM_MULTIBINDOPT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_BIND_START:

		pGameRun->PostMessage(MSG_PM_PLAYERBINDSTART, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;
	case PLAYER_BATH_INVITE:
	case PLAYER_BATH_INVITE_REPLAY:
	case PLAYER_BATH_STOP:
	case ENTER_BATH_WORLD:
	case LEAVE_BATH_WORLD:
	    pGameRun->PostMessage(MSG_PM_BATH_OPT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
	    break;
	case BATH_GAIN_ITEM:
		pGameRun->PostMessage(MSG_PM_BATH_GETAWARD,MAN_PLAYER,-1,(DWORD)pDataBuf,pCmdHeader->cmd);
		break;
	case PLAYER_BATH_START:
		pGameRun->PostMessage(MSG_PM_BATH_START,MAN_PLAYER,-1,(DWORD)pDataBuf,pCmdHeader->cmd);
		break;

	case PLAYER_MOUNTING:
		
		pGameRun->PostMessage(MSG_PM_PLAYERMOUNT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_EQUIP_DETAIL:
		
		pGameRun->PostMessage(MSG_HST_VIEWOTHEREQUIP, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PARIAH_TIME:

		pGameRun->PostMessage(MSG_HST_PARIAHTIME, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case GAIN_PET:
	case FREE_PET:
	case SUMMON_PET:
	case RECALL_PET:
	case PLAYER_START_PET_OP:
	case PLAYER_STOP_PET_OP:
	case PET_RECEIVE_EXP:
	case PET_LEVELUP:
	case PET_ROOM:
	case PET_ROOM_CAPACITY:
	case PET_HONOR_POINT:
	case PET_HUNGER_GAUGE:
	case EQUIP_PET_BEDGE:
	case EQUIP_PET_EQUIP:
	case PLAYER_SET_PET_STATUS:
	case PLAYER_SET_PET_RANK:
	case PLAYER_COMBINE_PET:
	case PLAYER_UNCOMBINE_PET:
	case PLAYER_PET_HP_NOTIFY:
	case PLAYER_PET_AGE_LIFE:
	case PLAYER_PET_DEAD:
	case PLAYER_PET_AI_STATE:
	case PLAYER_PET_SET_AUTO_SKILL:
	case PLAYER_PET_SET_SKILL_COOLDOWN:
	case PLAYER_MOVE_PET_BEDGE_ITEM:

		pGameRun->PostMessage(MSG_HST_PETOPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case HOST_ENTER_BATTLE:
	case BATTLE_RESULT:
	case BATTLE_INFO:
	case BATTLE_SCORE:
	case GAIN_BATTLE_SCORE:
	case SPEND_BATTLE_SCORE:
	case BATTLE_PLAYER_INFO:
	case BATTLE_LEAVE:
	case SELF_SJBATTLE_INFO:	
	case GET_SJBATTLE_INFO:		
	case GET_ARENA_INFO:
	case SJBATTLE_SCORE_CHANGE:
	case CROSSSERVER_BATTLE_SCORE: 
	case CRSSVR_TEAM_BATTLE_INFO:		//Added 2011-05-19.
	case CSFLOW_BATTLE_INFO:			// 2013-4-17 add by zy 流水席信息
		
		pGameRun->PostMessage(MSG_HST_BATTLEOPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;

	case TANK_LEADER_NOTIFY:
		
		pGameRun->PostMessage(MSG_PM_TANKLEADER, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_WALLOW_INFO:

		((player_wallow_info*)pDataBuf)->play_time = g_pGame->GetServerGMTTime() - ((player_wallow_info*)pDataBuf)->play_time;
		pGameRun->SetWallowInfo(*(player_wallow_info*)pDataBuf);
		break;

	case EXG_POCKET_ITEM:
	case MOVE_POCKET_ITEM:
	case EXG_IVTR_POCKET_ITEM:
	case IVTR_ITEM_TO_POCKET:
	case POCKET_ITEM_TO_IVTR:
	case CHANGE_POCKET_SIZE:
	case POCKET_ITEM_ALL_TO_IVTR:
		pGameRun->PostMessage(MSG_HST_POCKETOPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case NOTIFY_FASHION_HOTKEY:
	case EXG_FASHION_ITEM:
	case EXG_IVTR_FASHION:
	case EXG_EQUIP_FASHION:
	case HOTKEY_CHANGE_FASHION:
		pGameRun->PostMessage(MSG_HST_FASHIONPACKOPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
		
	case EXCHANGE_MOUNT_WING_ITEM:
	case EXCHANGE_INVENTORY_MOUNTWING_ITEM:
	case EXCHANGE_EQUIPMENT_MOUNTWING_ITEM:
	case PLAYER_CHANGE_MOUNTWING_INV_SIZE:
		pGameRun->PostMessage(MSG_HST_MOUNTWINGPACKOPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
		
	case PLAYER_ACHIEVEMENT:
    case PLAYER_ACHIEVEMENT_FINISH:
	case PLAYER_ACHIEVEMENT_ACTIVE:
	case PLAYER_PREMISS_DATA:      
    case PLAYER_PREMISS_FINISH:    
	case ACHIEVEMENT_MESSAGE:
	case OBTAIN_ACHIEVE_AWARD:
		pGameRun->PostMessage(MSG_HST_ACHIEVEMENT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case SELF_INSTANCING_BATTLE_INFO:
	case INSTANCING_BATTLE_INFO:
		pGameRun->PostMessage(MSG_HST_INSTANCINGOPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
		
	case ENTER_RAID_COUNT: //处理gs发来的副本相关的消息协议Added 2011-07-26.
	case RAID_PLAYER_INFO: //进入副本玩家信息 Added 2011-08-03.
	case PLAYER_ENTER_RAID_COUNT: //gs发来的玩家进入单个副本的次数 Added 2011-12-02.
		pGameRun->PostMessage(MSG_HST_RAIDOPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;			//Added end.
	case OBJECT_BE_MOVED: //Added 2011-07-28.
		pGameRun->PostMessage(MSG_HST_BE_MOVED, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PLAYER_PULL_STATE:	//Added 2011-07-28.
	case PLAYER_BE_PULLED_INFO:
		pGameRun->PostMessage(MSG_HST_PULL_OPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case TRANSFIGURE_SKILL_DATA:
		pGameRun->PostMessage(MSG_HST_TSSKILLDATA, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case COMBINE_SKILL_DATA:
		pGameRun->PostMessage(MSG_HST_COMBINE_SKILLDATA, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case ADDON_PERMANENT_SKILL:		//Added 2012-02-20.
	case ADDON_PERMANENT_SKILL_DATA:
		pGameRun->PostMessage(MSG_HST_ADDON_PERMANENT_SKILL, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PLAYER_ENTER_CARRIER:
	case PLAYER_LEAVE_CARRIER:
	case PLAYER_MOVE_ON_CARRIER:
	case PLAYER_STOP_ON_CARRIER:
		pGameRun->PostMessage(MSG_PM_CARRIER, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;
	case CIRCLE_INFO_NOTIFY:
	case DELIVERY_EXP_NOTIFY:
		pGameRun->PostMessage(MSG_HST_CIRCLE, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case MALL_ITEM_PRICE:
		pGameRun->PostMessage(MSG_GSHOP_INFO, -1, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case PLAYER_VIP_AWARD_INFO: //Added 2012-01-04.
		pGameRun->PostMessage(MSG_VIP_AWARD_INFO, -1, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;

	case ONLINE_AWARD_INFO:		//Added 2012-02-16.
	case ONLINE_AWARD_PERMIT:
		pGameRun->PostMessage( MSG_HST_ONLINE_AWARD, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;

	case ZX_FIVE_ANNI_DATA:
		pGameRun->PostMessage( MSG_HOST_5YEAR_ANNI, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;

	case TREASURE_INFO:		//诛仙古卷挖宝系列s2c相关协议处理 Added 2012-03-22.
	case TREASURE_REGION_UPGRADE_RESULT:
	case TREASURE_REGION_UNLOCK_RESULT:
	case TREASURE_REGION_DIG_RESULT:
	case INC_TREASURE_DIG_COUNT:
		pGameRun->PostMessage( MSG_HST_TREASURE_REGION_OPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;

	//趴塔副本相关协议处理 Added 2012-04-17.
	case RAID_GLOBAL_VARIABLE:
	case RANDOM_TOWER_MONSTER_RESULT:
	case PLAYER_TOWER_DATA:
	case PLAYER_TOWER_MONSTER_LIST:
	case SET_TOWER_GIVE_SKILL:
	case ADD_TOWER_MONSTER:
	case SET_TOWER_AWARD_STATE:
	case TOWER_LEVEL_PASS:
		pGameRun->PostMessage( MSG_HST_TOWER_RAID_OPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;

	case LITTLEPET_INFO:
		pGameRun->PostMessage( MSG_HST_LITTLEPET, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;
	case RUNE_INFO:
	case RUNE_REFINE_RESULT:
		pGameRun->PostMessage( MSG_HOST_RUNEINFO, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;
		
	case FILL_PLATFORM_MASK:
		pGameRun->PostMessage( MSG_PLATFORM_INFO, -1, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
		break;
		
	case PK_BET_DATA:
		pGameRun->PostMessage( MSG_HST_UI_FUNCTION, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;

	case UI_TRANSFER_OPENED_NOTIFY:
		pGameRun->PostMessage( MSG_HST_UI_TRANSFER_OPEN, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
		
	case TALISMAN_REFINESKILL:
		pGameRun->PostMessage( MSG_HST_UI_FUNCTION, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;

	case NOTIFY_ASTROLOGY_ENERGY:
		pGameRun->PostMessage( MSG_HST_NOTIFY_ASTROLOGY_ENERGY, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case ASTROLOGY_UPGRADE_RESULT:	
		pGameRun->PostMessage( MSG_HST_ASTROLOGY_UPGRADE_RESULT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case ASTROLOGY_DESTROY_RESULT:	
		pGameRun->PostMessage( MSG_HST_ASTROLOGY_DESTORY_RESULT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
		
	case COLLISION_RAID_RESULT:
	case COLLISION_RAID_INFO:
	case CASH_GIFT_IDS:
	case NOTIFY_CASH_GIFT:
		pGameRun->PostMessage( MSG_HST_UI_FUNCTION, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;
	case  ENTER_KINGDOM:
	case  LEAVE_KINGDOM:
	case  KINGDOM_HALF_END:
	case  KINGDOM_END_RESULT:
	case  KINGDOM_MAFIA_INFO:
	case  KINGDOM_KEY_NPC_INFO:
	case  KINGDOM_GATHER_MINE_START:
		pGameRun->PostMessage( MSG_HST_KINGDOM_OPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;	
	case ANCIENT_RAID_LEVEL_START:
	case ANCIENT_RAID_LEVEL_END:
	case ANCIENT_RAID_LEVEL_RESULT:
	case ANCIENT_RAID_LEVEL_INFO:
		pGameRun->PostMessage(MSG_HST_ANCIENT_RAID_OPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case SALE_PROMOTION_INFO:
		pGameRun->PostMessage(MSG_HST_SALE_PROMOTION_INFO, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case EVENT_START:
	case EVENT_STOP:
	case EVENT_INFO_ONLINE:
		pGameRun->PostMessage(MSG_HST_EVENT_OPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PLAYER_PROPADD:
	case PLAYER_GET_PROPADD_ITEM:
		pGameRun->PostMessage(MSG_HST_PROPADD_RESULT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PLAYER_EXTRA_EQUIP_EFFECT:
		pGameRun->PostMessage(MSG_PM_PLAYERINFO, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case FUWEN_COMPOSE_INFO:
	case FUWEN_COMPOSE_END:
	case FUWEN_INSTALL_RESULT:
	case FUWEN_UNINSTALL_RESULT:
		pGameRun->PostMessage(MSG_HST_FUWEN2013_OPT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case CSFLOW_BATTLE_PLAYER_INFO:
		pGameRun->PostMessage(MSG_CSFLOW_BATTLE_PLAYER_INFO, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case EXCHANGE_ITEM_LIST_INFO:
		pGameRun->PostMessage(MSG_HST_EXCHANGE_ITEM_LIST_INFO, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case WEB_ORDER_INFO:
		pGameRun->PostMessage(MSG_HST_WEB_ORDER_INFO, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case BATTLE_OFF_LINE_INFO:
		pGameRun->PostMessage(MSG_HST_BATTLE_OFF_LINE_INFO, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case CONTROL_TRAP_INFO:
		pGameRun->PostMessage(MSG_HST_CONTROL_TRAP_INFO, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case NPC_INVISIBLE:
		pGameRun->PostMessage(MSG_HST_NPC_INVISIBLE, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PUPPET_FORM:
		pGameRun->PostMessage(MSG_PM_PUPPET_FORM, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case TELEPORT_SKILL_INFO:
		pGameRun->PostMessage(MSG_PM_TELEPORT_SKILL_INFO, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PLAYER_START_INTERACT:
		pGameRun->PostMessage(MSG_PM_START_INTERACT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PLAYER_IN_INTERACT:
		pGameRun->PostMessage(MSG_PM_IN_INTERACT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PLAYER_END_INTERACT:
		pGameRun->PostMessage(MSG_PM_END_INTERACT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PLAYER_FINISH_INTERACT:
		pGameRun->PostMessage(MSG_PM_FINISH_INTERACT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PLAYER_CANCEL_INTERACT:
		pGameRun->PostMessage(MSG_PM_CANCEL_INTERACT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PHASE_INFO:
	case PHASE_INFO_CHANGE:
		pGameRun->PostMessage(MSG_HST_PHASE_INFO, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case FASHION_COLORANT_PRODUCE:
	case FASHION_COLORANT_USE:
		pGameRun->PostMessage( MSG_PM_FASHION_COLORANT, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;
	case FAC_BUILDING_ADD:
	case FAC_BUILDING_UPGRADE_FAC:	
	case FAC_BUILDING_REMOVE:	
	case FAC_BUILDING_COMPLETE:
	case PLAYER_FAC_BASE_INFO:
	case FAC_BASE_PROP_CHANGE:
	case GET_FACBASE_MALL_INFO:
	case GET_FACTION_COUPON:
	case GET_FACBASE_MONEY_ITEMS_INFO:
	case PLAYER_FACBASE_AUCTION:
	case FACBASE_AUCTION_UPDATE:
	case FAC_COUPON_RETURN:
	case GET_FACBASE_CASH_ITEM_SUCCESS:
	case FACBASE_AUCTION_ADD_HISTORY:
	case FACBASE_MSG_UPDATE:
		pGameRun->PostMessage( MSG_PM_FAC_BUILDING, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize );
		break;
	case PLAYER_QILIN_INVITE:
	case PLAYER_QILIN_INVITE_REPLY:
	case PLAYER_QILIN_STOP:
	case PLAYER_QILIN_DISCONNECT:
	case PLAYER_QILIN_RECONNECT:
		pGameRun->PostMessage(MSG_PM_QILIN_BINDOPT, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PLAYER_QILIN_START:
		pGameRun->PostMessage(MSG_PM_QILIN_BIND_START, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PLAYER_CHANGE_PUPPET_FORM:
		pGameRun->PostMessage(MSG_PM_PLAYER_CHANGE_PUPPET_FORM, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case BLOW_OFF:
		{
			blow_off* pCmd = (blow_off*)pDataBuf;
			if (ISPLAYERID(pCmd->roleid))
				pGameRun->PostMessage(MSG_PM_PLAYER_BLOW_OFF, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
			else if (ISNPCID(pCmd->roleid))
				pGameRun->PostMessage(MSG_NM_BLEWOFF, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd);
			break;
		}
	case BE_TAUNTED2:
		pGameRun->PostMessage(MSG_HST_PLAYER_BE_TAUNTED2, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case INVITE_ACTIVE_EMOTE:
		pGameRun->PostMessage(MSG_HST_INVITE_ACTIVE_EMOTE, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);\
		break;
	case REPLY_ACTIVE_EMOTE:
		pGameRun->PostMessage(MSG_HST_REPLY_ACTIVE_EMOTE, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PLAY_ACTIVE_EMOTE:
		pGameRun->PostMessage(MSG_PM_PLAY_ACTIVE_EMOTE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case STOP_ACTIVE_EMOTE:
		pGameRun->PostMessage(MSG_PM_STOP_ACTIVE_EMOTE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case OBJECT_TRY_CHARGE:
		pGameRun->PostMessage(MSG_HST_BE_CHARGE_PRE, MAN_PLAYER, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case OBJECT_BE_CHARGE_TO:
		pGameRun->PostMessage(MSG_PM_BE_CHARGE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case PLAYER_START_TRAVEL_AROUND:
	case PLAYER_STOP_TRAVEL_AROUND:
		pGameRun->PostMessage(MSG_PM_TRAVEL_FREE, MAN_PLAYER, -1, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;

	case PLAYER_GET_SUMMON_PETPROP:
		pGameRun->PostMessage(MSG_NM_NPCINFO, MAN_NPC, 0, (DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	case NOTIFY_BLOODPOOL_STATUS:
		pGameRun->PostMessage(MSG_PM_BLOODPOOL_STATUS, MAN_PLAYER, -1,(DWORD)pDataBuf, pCmdHeader->cmd, dwDataSize);
		break;
	default:

		break;
	}
}

														   
//	Log S2C command data
void CECGameSession::LogS2CCmdData(int iCmd, BYTE* pDataBuf, int nSize)
{
	return;

	if( !m_pS2CCmdLog )
		return;

	using namespace S2C;
	m_nS2CCmdSize += nSize;

	int iPrintSize = nSize;
	a_ClampRoof(iPrintSize, 600);

	char* szStr = new char [iPrintSize * 3 + 1];
	int iCount = 0;
	for(int i=0; i < iPrintSize; i++)
	{
		sprintf(&szStr[iCount], "%02x ", pDataBuf[i]);
		iCount += 3;
	}

	m_pS2CCmdLog->Log("total: %d, size: %d, cmd: %d, data: %s", m_nS2CCmdSize, nSize, iCmd, szStr);

	delete [] szStr;

	// output to binary log
	if( !m_pS2CCmdBinLog )
		return;

	DWORD dwWriteLen;
	m_pS2CCmdBinLog->Write(&nSize, sizeof(nSize), &dwWriteLen);
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	m_pS2CCmdBinLog->Write(&sysTime, sizeof(sysTime), &dwWriteLen);
	m_pS2CCmdBinLog->Write(pDataBuf, nSize, &dwWriteLen);
	m_pS2CCmdBinLog->Flush();
	return;
}

void CECGameSession::ProcessFunGameData(GNET::Protocol*p)
{
	m_pFunGamePrtcHandler->Process(p);
}

void CECGameSession::ProcessHomeTownData(GNET::Protocol* p)
{
	m_pHomeTownPrtcHandler->Process(p);
}
