/*
 * FILE: EC_GPDataType.cpp
 *
 * DESCRIPTION: Game net command type
 *
 * CREATED BY: CuiMing, Duyuxin, 2006/2/7
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_GPDataType.h"
#include "ids.hxx"
#include "EC_RTDebug.h"
#ifdef _INVENTORY_DESC_MAN	//寻宝网 Added 2012-06-21.
#include "EC_Player_HintTool.h"
#else
#include "EC_Player.h"
#endif
#include "EC_GameRecord.h"
#include "AAssist.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define CATCH_FILLDATA_EXP(fn) \
	catch (CECException& e) \
	{ \
		ASSERT(0); \
		a_LogOutput(1, "%s::FillData error (%d)", #fn, e.GetType()); \
		return false; \
	}

///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////

extern game_record gr;

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
//	Implement
//	
///////////////////////////////////////////////////////////////////////////

/*	Fill structure data

	pData: original net data
	iBufLen: data buffer length of pData
	piRealLen (out): real length of filled data, can be NULL.
*/
bool S2C::info_player_1::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		cid			= dr.Read_int();
		pos.x		= dr.Read_float();
		pos.y		= dr.Read_float();
		pos.z		= dr.Read_float();
		crc_e		= dr.Read_WORD();
		dir			= dr.Read_BYTE();
		profession	= dr.Read_BYTE();
		level		= dr.Read_BYTE();
		reborn_count= dr.Read_BYTE();
		dt_level	= dr.Read_BYTE();
		pk_level	= dr.Read_BYTE();
		cultivation	= dr.Read_BYTE();
		reputation	= dr.Read_int();
		hide_equip	= dr.Read_int();
		state		= dr.Read_int();
		extra_state = dr.Read_int();

		//	Flexible data
		if (state & GP_STATE_ADV_MODE)
		{
			adv_d1 = dr.Read_DWORD();
			adv_d2 = dr.Read_DWORD();
		}
		
		if (state & GP_STATE_SHAPE)
			shape = dr.Read_BYTE();

		if (state & GP_STATE_EMOTE)
			emote = dr.Read_BYTE();
		else
			emote = CECPlayer::ACT_STAND;

		if (state & GP_STATE_EXTEND_PROPERTY)
			ext_states = dr.Read_INT64();
		
		if (state & GP_STATE_FACTION)
			id_faction	= dr.Read_int();

		if (state & GP_STATE_BOOTH)
		{
			crc_booth = dr.Read_BYTE();
			booth_item_id = dr.Read_int();
		}
		
		if (state & GP_STATE_EFFECT)
		{
			effect_num = dr.Read_BYTE();
			int iSize = effect_num * sizeof (short);
			memcpy(effects, dr.Read_Data(iSize), iSize);
		}

		if (state & GP_STATE_IN_MOUNT)
		{
			riding_pet_lvl	= dr.Read_BYTE();
			id_riding_pet	= dr.Read_int();
			bMultiRide		= dr.Read_BYTE();
		}

		if (state & GP_STATE_IN_BIND)
		{
			bind_type	= dr.Read_BYTE();
			id_buddy	= dr.Read_int();
			buddy_pos   = dr.Read_BYTE();
			bDisConnect = dr.Read_BYTE() ? true : false;
		}

		if (state & GP_STATE_TITLE)
		{
			title	= dr.Read_short();
		}

		if (state & GP_STATE_SPOUSE)
			id_spouse	= dr.Read_int();

		if (state & GP_STATE_FAMILY)
		{
			id_family	= dr.Read_int();
			fam_rank	= dr.Read_BYTE();
		}
		
		if (state & GP_STATE_SECT_MASTER_ID)
		{
			master_id = dr.Read_int();
		}

		if  (state & GP_STATE_IN_SJBATTLE)
		{
			military_rank = dr.Read_BYTE();
		}
		
		if (state & GP_STATE_IN_VIPSTATE)
		{
			vip_state  = dr.Read_BYTE();
		}
		
		if (state & GP_STATE_TRANSFIGURE)
		{
			id_transfiguredProp = dr.Read_int();
		}
		
		if (extra_state & GP_EXTRA_STATE_CARRIER)
		{
			id_carrier = dr.Read_int();
			rpos.x  = dr.Read_float();
			rpos.y  = dr.Read_float();
			rpos.z	= dr.Read_float();
			rdir	= dr.Read_BYTE();			
		}
		
		if (extra_state & GP_EXTRA_STATE_CIRCLE)
		{
			id_circle = dr.Read_int();
			circle_role = dr.Read_BYTE();
		}
		
		if (extra_state & GP_EXTRA_STATE_CROSSZONE)
		{
			id_orgserver = dr.Read_int();
		}

		//Added 2012-01-16.
		if ( extra_state & GP_EXTRA_STATE_VIP_AWARD )
		{
			vip_award_level = dr.Read_BYTE();
			is_hide_vip_level = dr.Read_BYTE();
		}
		//Added end.

		//Added 2012-08-01.
		if ( extra_state & GP_EXTRA_STATE_COLLISION_RAID )
		{
			collision_raid_transfigure_id = dr.Read_int();
			collision_raid_state = dr.Read_int();
		}
		//Added end.

		//Added 2012-08-16. 轩辕光暗状态
		if ( extra_state & GP_EXTRA_STATE_XY_DARK_LIGHT )
		{
			xuanyuan_dark_light_state = dr.Read_char();
		}

		if ( extra_state & GP_EXTRA_STATE_XY_SPIRITS )
		{
			for(int i=0;i<3;i++)
				dark_light_spirits[i] = dr.Read_char();
		}

		if ( extra_state & GP_EXTRA_STATE_MIRROR_IMAGE )
		{
			mirror_image_cnt = dr.Read_char();
		}

		//Added 2012-08-20. 改变飞剑颜色
		if ( extra_state & GP_EXTRA_STATE_CHANGE_WING_COLOR )
		{
			change_color = dr.Read_char();
		}
		//Added end.
		
		if ( extra_state & GP_EXTRA_STATE_DIR_VISIBLE_STATE )
		{
			visible_state_id = dr.Read_int();
			visible_state_dir = dr.Read_int();
			visible_state_on = (dr.Read_BYTE() ? true : false);
		}
		
		if( state & GP_STATE_FLY)
		{ 
			start_fly_type = (char)dr.Read_short();	//这里是一个坑！！服务器将这个值强转成short发过来，实际长度是short。
		}

		if ( extra_state & GP_EXTRA_STATE_EXTRA_EQUIP_EFFECT )
		{
			weapon_effect_level = dr.Read_char();
		}

		if(extra_state & GP_EXTRA_STATE_FLOW_BATTLE)
		{
			flow_battle_ckill = dr.Read_short();
		}
		
		if(extra_state & GP_EXTRA_STATE_PUPPET)
		{
			puppetid = dr.Read_char();
		}

		if (extra_state & GP_EXTRA_STATE_INTERACT)
		{
			interactor_id = dr.Read_int();
			interact_pos = dr.Read_char();
			interact_state = dr.Read_char();
		}

		if (extra_state & GP_EXTRA_STATE_ACTIVE_EMOTE)
		{
			active_emote_inviter = * (bool*) dr.Read_Data(sizeof(bool));
			active_emote_id = dr.Read_int();
			active_emote_type = dr.Read_int();
		}
		if (extra_state & GP_EXTRA_STATE_TRAVEL)
		{
			traval_vehicle_id= dr.Read_int();
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(info_player_1)

	return true;
}

S2C::cmd_player_info_1_list::~cmd_player_info_1_list()
{
	if (players)
		delete [] players;
}

bool S2C::cmd_player_info_1_list::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		int iRemainLen = iBufLen;

		count = dr.Read_WORD();
		iRemainLen -= sizeof (WORD);

		if (count)
		{
			if (!(players = new info_player_1[count]))
				return false;

			int i, iReadLen;
			void* pReadData = dr.GetCurAddr();

			for (i=0; i < count; i++)
			{
				if (!players[i].FillData(pReadData, iRemainLen, &iReadLen))
				{
					delete [] players;
					players = NULL;
					return false;
				}

				pReadData = dr.Offset(iReadLen, SEEK_CUR);
				iRemainLen -= iReadLen;
			}
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(cmd_player_info_1_list)

	return true;
}

bool S2C::info_npc::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		nid		= dr.Read_int();
		tid		= dr.Read_int();
		pos.x	= dr.Read_float();
		pos.y	= dr.Read_float();
		pos.z	= dr.Read_float();
		seed	= dr.Read_WORD();
		dir		= dr.Read_BYTE();
		state	= dr.Read_int();

		//	Flexible data
		if (state & GP_STATE_EXTEND_PROPERTY)
			ext_states = dr.Read_INT64();

		if (state & GP_STATE_NPC_PET)
		{
			id_master = dr.Read_int();
			pet_star = dr.Read_char();
			shape = dr.Read_char();
			face = dr.Read_char();
		}

		if (state & GP_STATE_NPC_NAME)
		{
			BYTE byLen = dr.Read_BYTE();
			memcpy(name, dr.Read_Data(byLen), a_Min((int)byLen, 30));
		}
		
		if (state & GP_STATE_NPC_SUMMON)
		{
			id_summoner = dr.Read_int();
		}
		
		if (state & GP_STATE_NPC_OWNER)
		{
			owner_type	= dr.Read_BYTE();
			owner_id	= dr.Read_int();
		}
		
		if ( state & GP_STATE_NPC_DIR_VISIBLE )
		{
			visible_state_id	= dr.Read_int();
			visible_state_dir	= dr.Read_int();
			visible_state_on	= (dr.Read_BYTE() ? true : false);
		}

		if(state & GP_STATE_NPC_PHASE)
		{
			phase_id = dr.Read_short();
		}

		if(state & GP_STATE_NPC_TELEPORT1 || state & GP_STATE_NPC_TELEPORT2)
		{	
			teleportnum = dr.Read_BYTE();
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(info_npc)

	return true;
}

S2C::cmd_npc_info_list::~cmd_npc_info_list()
{
	if (npcs)
		delete [] npcs;
}

bool S2C::cmd_npc_info_list::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		int iRemainLen = iBufLen;

		count = dr.Read_WORD();
		iRemainLen -= sizeof (WORD);

		if (count)
		{
			if (!(npcs = new info_npc[count]))
				return false;

			int i, iReadLen;
			void* pReadData = dr.GetCurAddr();

			for (i=0; i < count; i++)
			{
				if (!npcs[i].FillData(pReadData, iRemainLen, &iReadLen))
				{
					delete [] npcs;
					npcs = NULL;
					return false;
				}

				pReadData = dr.Offset(iReadLen, SEEK_CUR);
				iRemainLen -= iReadLen;
			}
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(cmd_npc_info_list)

	return true;
}

bool S2C::info_matter::FillData(const void* pData, int iBufLen, int* piRealLen/* =NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		mid		= dr.Read_int();
		tid		= dr.Read_int();
		dropper_id = dr.Read_int();
		pos.x	= dr.Read_float();
		pos.y	= dr.Read_float();
		pos.z	= dr.Read_float();
		dir0	= dr.Read_BYTE();
		dir1	= dr.Read_BYTE();
		rad		= dr.Read_BYTE();
		state	= dr.Read_BYTE();
		flag	= dr.Read_BYTE();

		if (state & GP_STATE_MATTER_OWNER)
		{
			owner_type	= dr.Read_BYTE();
			owner_id	= dr.Read_int();
		}

		if(state & GP_STATE_MATTER_PHASE)
		{
			phase_id = dr.Read_short();
		}

		if(state & GP_STATE_COMBINE_MINE)
		{
			mine_state	= dr.Read_int();
		}
		
		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(info_matter)

	return true;
}

bool S2C::cmd_matter_info_list::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		int iRemainLen = iBufLen;
		CECDataReader dr((void*)pData, iBufLen);
		count = dr.Read_WORD();
		iRemainLen -= sizeof(WORD);

		if (count)
		{
			matters = new info_matter[count];

// 			if (gr.get_state() == game_record::enum_gr_replay && gr.get_cur_version() < 16)
// 			{
// 
// #pragma pack(1)
// 
// 				struct _old
// 				{
// 					int mid;
// 					int tid;
// 					A3DVECTOR3 pos;
// 					unsigned char dir0;
// 					unsigned char dir1;
// 					unsigned char rad;
// 					unsigned char state;
// 				};
// 
// #pragma pack()
// 
// 				size_t sz = count * sizeof(_old);
// 				_old* _old_data = (_old*)dr.Read_Data(sz);
// 
// 				for (unsigned short i = 0; i < count; i++)
// 				{
// 					memcpy(&matters[i], &_old_data[i], sizeof(_old));
// 					matters[i].flag = 0;
// 				}
// 			}
// 			else
//			{
				
				int i, iReadLen;
				void* pReadData = dr.GetCurAddr();

				for (i=0; i < count; i++)
				{
					if (!matters[i].FillData(pReadData, iRemainLen, &iReadLen))
					{
						delete [] matters;
						matters = NULL;
						return false;
					}

					pReadData = dr.Offset(iReadLen, SEEK_CUR);
					iRemainLen -= iReadLen;
				}

// 				size_t sz = count * sizeof(info_matter);
// 				memcpy(matters, dr.Read_Data(sz), sz);
//			}
//		}
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(cmd_matter_info_list)

	return true;
}

bool S2C::cmd_out_of_sight_list::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		uCount = dr.Read_DWORD();
		id_list = uCount ? (int*)dr.Read_Data(uCount * sizeof (int)) : NULL;

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(cmd_out_of_sight_list)

	return true;
}

bool S2C::cmd_own_ivtr_info::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		byPackage		= dr.Read_BYTE();
		ivtr_size		= dr.Read_BYTE();
		content_length	= dr.Read_DWORD();
		content			= content_length ? dr.Read_Data(content_length) : NULL;

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(cmd_own_ivtr_info)

	return true;
}

bool S2C::cmd_own_ivtr_detail_info::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		byPackage		= dr.Read_BYTE();
		ivtr_size		= dr.Read_BYTE();
		content_length	= dr.Read_DWORD();
		content			= content_length ? dr.Read_Data(content_length) : NULL;

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(cmd_own_ivtr_detail_info)

	return true;
}

bool S2C::cmd_own_item_info::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		byPackage		= dr.Read_BYTE();
		bySlot			= dr.Read_BYTE();
		type			= dr.Read_int();
		expire_date		= dr.Read_int();
		state			= dr.Read_int();
		count			= dr.Read_DWORD();
		crc				= dr.Read_WORD();
		content_length	= dr.Read_WORD();
		content			= content_length ? dr.Read_Data(content_length) : NULL;

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(cmd_own_item_info)

	return true;
}

bool S2C::cmd_battle_info::FillData(const void* pData, int iBufLen, int* piRealLen)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		attacker_building_left = dr.Read_BYTE();
		defender_building_left = dr.Read_BYTE();
		player_count = dr.Read_BYTE();
		
		if (player_count)
		{
			info = new player_info_in_battle[player_count];
			size_t sz = player_count * sizeof(player_info_in_battle);
			memcpy(info, dr.Read_Data(sz), sz);
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(cmd_battle_info)

	return true;
}

bool S2C::flow_battle_info::FillData(const void* pData, int iBufLen, int* piRealLen)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		player_count = dr.Read_BYTE();
		
		if (player_count)
		{
			info = new player_info_in_flow_battle[player_count];
			size_t sz = player_count * sizeof(player_info_in_flow_battle);
			memcpy(info, dr.Read_Data(sz), sz);
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(flow_battle_info)

	return true;
}


bool S2C::battle_off_line_info::FillData(const void* pData, int iBufLen, int* piRealLen)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		off_line_count = dr.Read_BYTE();
		
		if (off_line_count)
		{
			info = new battle_off_line_list[off_line_count];
			size_t sz = off_line_count * sizeof(battle_off_line_list);
			memcpy(info, dr.Read_Data(sz), sz);
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(battle_off_line_info)

	return true;
}

bool S2C::player_fac_base_info::FillData(const void* pData, int iBufLen, int* piRealLen)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		pValue.grass = dr.Read_int();
		pValue.mine = dr.Read_int();
		pValue.monster_food = dr.Read_int();
		pValue.monster_core = dr.Read_int();
		pValue.money = dr.Read_int();
		pValue.task_id = dr.Read_int();
		pValue.task_count = dr.Read_int();
		pValue.task_need = dr.Read_int();
		msg_size = dr.Read_int();
		if(msg_size)
		{
			msg = new char[msg_size];
			size_t sz = msg_size * sizeof(char);
			memcpy(msg, dr.Read_Data(sz), sz);
		}
		field_count = dr.Read_BYTE();
		if (field_count)
		{
			info = new fac_field[field_count];
			size_t sz = field_count * sizeof(fac_field);
			memcpy(info, dr.Read_Data(sz), sz);
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(player_fac_base_info)

	return true;
}


bool S2C::player_facbase_auction::FillData(const void* pData, int iBufLen, int* piRealLen)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		return_coupon = dr.Read_int();
		size = dr.Read_int();

		if (size)
		{
			listItem = new fac_base_auc_item[size];
			size_t sz = size * sizeof(fac_base_auc_item);
			memcpy(listItem, dr.Read_Data(sz), sz);
		}
		historysize = dr.Read_int();
		if (historysize)
		{
			phistoryList = new fac_base_auc_history[historysize];
			size_t sz = historysize * sizeof(fac_base_auc_history);
			memcpy(phistoryList, dr.Read_Data(sz), sz);
		}
		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(player_facbase_auction)

		return true;
}

bool S2C::cmd_sjbattle_info::FillData(const void* pData, int iBufLen, int* piRealLen/* =NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		attacker_score = dr.Read_int();
		defender_score = dr.Read_int();
		attacker_count = dr.Read_int();
		defender_count = dr.Read_int();
		player_count = dr.Read_int();
		
		if (player_count)
		{
			info = new player_info_in_battle[player_count];
			size_t sz = player_count * sizeof(player_info_in_battle);
			memcpy(info, dr.Read_Data(sz), sz);
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(cmd_sjbattle_info)

	return true;
}

bool S2C::arena_info::FillData(const void* pData, int iBufLen, int* piRealLen/* =NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		kill_count = dr.Read_int();
		score = dr.Read_int();
		apply_count = dr.Read_int();
		remain_count = dr.Read_int();
		player_count = dr.Read_int();
		
		if (player_count)
		{
			info = new player_info_in_arena[player_count];
			size_t sz = player_count * sizeof(player_info_in_arena);
			memcpy(info, dr.Read_Data(sz), sz);
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(arena_info)

	return true;
}

bool S2C::war_construction_info::FillData(const void* pData, int iBufLen, int* piRealLen)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		res_a = dr.Read_int();
		res_b = dr.Read_int();
		count = dr.Read_BYTE();

		if (count)
		{
			nodes = new node_t[count];
			size_t sz = count * sizeof(node_t);
			memcpy(nodes, dr.Read_Data(sz), sz);
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(war_construction_info)

	return true;
}

bool S2C::war_contribution_info::FillData(const void* pData, int iBufLen, int* piRealLen)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		page = dr.Read_BYTE();
		max_page = dr.Read_BYTE();
		count = dr.Read_BYTE();

		if (count)
		{
			nodes = new node_t[count];
			size_t sz = count * sizeof(node_t);
			memcpy(nodes, dr.Read_Data(sz), sz);
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(war_contribution_info)

	return true;
}

bool S2C::war_info::FillData(const void* pData, int iBufLen, int* piRealLen)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		count = dr.Read_BYTE();

		if (count)
		{
			nodes = new node_t[count];
			size_t sz = count * sizeof(node_t);
			memcpy(nodes, dr.Read_Data(sz), sz);
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(war_info)

	return true;
}

bool S2C::player_reborn_info::FillData(const void* pData, int iBufLen, int* piRealLen)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		count = dr.Read_WORD();

		if (count)
		{
			info = new info_t[count];
			size_t sz = count * sizeof(info_t);
			memcpy(info, dr.Read_Data(sz), sz);
		}

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(player_reborn_info)

	return true;
}

bool S2C::cmd_icon_state_notify::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		id = dr.Read_int();
		count = dr.Read_WORD();
		buff_list = count ? (_buff_t*)dr.Read_Data(count * sizeof (_buff_t)) : NULL;

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(cmd_icon_state_notify)

	return true;
}

bool S2C::self_buff_notify::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		id = dr.Read_int();
		count = dr.Read_WORD();
		buff_list = count ? (_buff_t*)dr.Read_Data(count * sizeof (_buff_t)) : NULL;

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(self_buff_notify)

	return true;
}

bool S2C::notify_fashion_hotkey::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof (*this));

	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		count = dr.Read_int();
		hotkey_list = count ? (fashion_hotkey*)dr.Read_Data(count * sizeof (fashion_hotkey)) : NULL;

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(notify_fashion_hotkey)

	return true;	
}

bool S2C::clone_player_equip::FillData(const void* pData, int iBufLen, int* piRealLen/* =NULL */)
{
	memset(this, 0, sizeof(*this));
	try
	{
		CECDataReader dr((void*)pData, iBufLen);

		int iRemainLen = iBufLen;
		
		id = dr.Read_int();
		iRemainLen -= sizeof(int);

		int iReadLen;
		void* pReadData = dr.GetCurAddr();

		if (!info.FillData(pReadData, iRemainLen, &iReadLen))
		{
			ASSERT(0);
			return false;
		}

		pReadData = dr.Offset(iReadLen, SEEK_CUR);
		iRemainLen -= iReadLen;
		
		crc = dr.Read_short();
		iRemainLen -= sizeof(short);
		mask = dr.Read_int();
		iRemainLen -= sizeof(int);
		data = (int*)dr.Read_Data(iRemainLen);
		

		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(clone_player_equip)

	return true;
}

bool S2C::repurchase_inv_data::FillData(const void* pData, int iBufLen, int* piRealLen/* NULL */)
{
	memset(this, 0, sizeof (*this));
	
	try
	{
		CECDataReader dr((void*)pData, iBufLen);
		
		ivtr_size		= dr.Read_BYTE();
		content_length	= dr.Read_DWORD();
		content			= content_length ? dr.Read_Data(content_length) : NULL;
		
		if (piRealLen)
			*piRealLen = dr.GetPos();
	}
	CATCH_FILLDATA_EXP(repurchase_inv_data)
		
	return true;
}
