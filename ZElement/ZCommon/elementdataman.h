#ifndef _ELEMENTDATAMAN_H_
#define _ELEMENTDATAMAN_H_

#include <stdlib.h>
#include <stdio.h>
#include <vector.h>
#include <hashmap.h>

//#ifdef LINUX
//#include "exptypes.h"
//#else
//#include "../zelementdata/exptypes.h"
//
// #endif	//LINUX

#include "exptypes.h"


extern const char * DataTypeName[DT_MAX+1];

class elementdataman
{
public:
	elementdataman();
	~elementdataman();

	//////////////////////////////////////////////////////////////////////////
	
	void add_structure(unsigned int id, EQUIPMENT_ADDON & data);
	void add_structure(unsigned int id, EQUIPMENT_MAJOR_TYPE & data);
	void add_structure(unsigned int id, EQUIPMENT_SUB_TYPE & data);
	void add_structure(unsigned int id, EQUIPMENT_ESSENCE & data);
	void add_structure(unsigned int id, REFINE_TICKET_ESSENCE & data);
	void add_structure(unsigned int id, MEDICINE_MAJOR_TYPE & data);
	void add_structure(unsigned int id, MEDICINE_SUB_TYPE & data);
	void add_structure(unsigned int id, MEDICINE_ESSENCE & data);
	void add_structure(unsigned int id, MATERIAL_MAJOR_TYPE & data);
	void add_structure(unsigned int id, MATERIAL_SUB_TYPE & data);
	void add_structure(unsigned int id, MATERIAL_ESSENCE & data);
	void add_structure(unsigned int id, SKILLTOME_SUB_TYPE & data);
	void add_structure(unsigned int id, SKILLTOME_ESSENCE & data);
	void add_structure(unsigned int id, TRANSMITROLL_ESSENCE & data);
	void add_structure(unsigned int id, LUCKYROLL_ESSENCE & data);
	void add_structure(unsigned int id, TOWNSCROLL_ESSENCE & data);
	void add_structure(unsigned int id, REVIVESCROLL_ESSENCE & data);
	void add_structure(unsigned int id, TASKMATTER_ESSENCE & data);
	void add_structure(unsigned int id, DROPTABLE_TYPE & data);
	void add_structure(unsigned int id, DROPTABLE_ESSENCE & data);
	void add_structure(unsigned int id, MONSTER_TYPE & data);
	void add_structure(unsigned int id, MONSTER_ESSENCE & data);
	void add_structure(unsigned int id, NPC_TALK_SERVICE & data);
	void add_structure(unsigned int id, NPC_SELL_SERVICE & data);
	void add_structure(unsigned int id, NPC_BUY_SERVICE & data);
	void add_structure(unsigned int id, NPC_TASK_IN_SERVICE & data);
	void add_structure(unsigned int id, NPC_TASK_OUT_SERVICE & data);
	void add_structure(unsigned int id, NPC_TASK_MATTER_SERVICE & data);
	void add_structure(unsigned int id, NPC_HEAL_SERVICE & data);
	void add_structure(unsigned int id, NPC_TRANSMIT_SERVICE & data);
	void add_structure(unsigned int id, NPC_PROXY_SERVICE & data);
	void add_structure(unsigned int id, NPC_STORAGE_SERVICE & data);
	void add_structure(unsigned int id, NPC_TYPE & data);
	void add_structure(unsigned int id, NPC_ESSENCE & data);
	void add_structure(unsigned int id, talk_proc *  data);
	void add_structure(unsigned int id,  RECIPE_MAJOR_TYPE & data);
	void add_structure(unsigned int id,  RECIPE_SUB_TYPE & data);
	void add_structure(unsigned int id,  RECIPE_ESSENCE & data);
	void add_structure(unsigned int id,  ENEMY_FACTION_CONFIG & data);
	void add_structure(unsigned int id,  CHARACTER_CLASS_CONFIG & data);
	void add_structure(unsigned int id,  PARAM_ADJUST_CONFIG & data);
	void add_structure(unsigned int id,  PIE_LOVE_CONFIG & data);
	void add_structure(unsigned int id,  TASKDICE_ESSENCE & data);
	void add_structure(unsigned int id,	 TASKNORMALMATTER_ESSENCE & data);
	void add_structure(unsigned int id,	 PLAYER_LEVELEXP_CONFIG & data);
	void add_structure(unsigned int id,	 MINE_TYPE & data);
	void add_structure(unsigned int id,	 MINE_ESSENCE & data);
	void add_structure(unsigned int id,  GM_GENERATOR_TYPE & data);
	void add_structure(unsigned int id,  GM_GENERATOR_ESSENCE & data);
	void add_structure(unsigned int id,  FIREWORKS_ESSENCE & data);
	void add_structure(unsigned int id,  NPC_WAR_TOWERBUILD_SERVICE & data);
	void add_structure(unsigned int id,	 PLAYER_SECONDLEVEL_CONFIG & data);
	void add_structure(unsigned int id,  NPC_RESETPROP_SERVICE & data);
	void add_structure(unsigned int id,  ESTONE_ESSENCE & data);
	void add_structure(unsigned int id,  PSTONE_ESSENCE & data);
	void add_structure(unsigned int id,  SSTONE_ESSENCE & data);
	void add_structure(unsigned int id,  RECIPEROLL_MAJOR_TYPE & data);
	void add_structure(unsigned int id,  RECIPEROLL_SUB_TYPE & data);
	void add_structure(unsigned int id,  RECIPEROLL_ESSENCE & data);
	void add_structure(unsigned int id,  SUITE_ESSENCE & data);
	void add_structure(unsigned int id,  DOUBLE_EXP_ESSENCE & data);
	void add_structure(unsigned int id,  DESTROYING_ESSENCE & data);
	void add_structure(unsigned int id,  NPC_EQUIPBIND_SERVICE & data);
	void add_structure(unsigned int id,  NPC_EQUIPDESTROY_SERVICE & data);
	void add_structure(unsigned int id,  NPC_EQUIPUNDESTROY_SERVICE & data);
	void add_structure(unsigned int id,  SKILLMATTER_ESSENCE & data);
	void add_structure(unsigned int id,  VEHICLE_ESSENCE & data);
	void add_structure(unsigned int id,  COUPLE_JUMPTO_ESSENCE & data);
	void add_structure(unsigned int id,  LOTTERY_ESSENCE & data);
	void add_structure(unsigned int id,  CAMRECORDER_ESSENCE & data);
	void add_structure(unsigned int id,  TITLE_PROP_CONFIG & data);
	void add_structure(unsigned int id,	 SPECIAL_ID_CONFIG & data);
	void add_structure(unsigned int id,  TEXT_FIREWORKS_ESSENCE & data);
	void add_structure(unsigned int id,  TALISMAN_MAINPART_ESSENCE & data);
	void add_structure(unsigned int id,  TALISMAN_EXPFOOD_ESSENCE & data);
	void add_structure(unsigned int id,  TALISMAN_MERGEKATALYST_ESSENCE & data);
	void add_structure(unsigned int id,  TALISMAN_ENERGYFOOD_ESSENCE & data);
	void add_structure(unsigned int id,  SPEAKER_ESSENCE & data);
	void add_structure(unsigned int id,	 PLAYER_TALENT_CONFIG & data);
	void add_structure(unsigned int id,  POTENTIAL_TOME_ESSENCE & data);
	void add_structure(unsigned int id,  WAR_ROLE_CONFIG & data);
	void add_structure(unsigned int id,  NPC_WAR_BUY_ARCHER_SERVICE & data);
	void add_structure(unsigned int id,  SIEGE_ARTILLERY_SCROLL_ESSENCE & data);
	void add_structure(unsigned int id,  PET_BEDGE_ESSENCE & data);
	void add_structure(unsigned int id,  PET_FOOD_ESSENCE & data);
	void add_structure(unsigned int id,  PET_SKILL_ESSENCE & data);
	void add_structure(unsigned int id,  PET_ARMOR_ESSENCE & data);
	void add_structure(unsigned int id,  PET_AUTO_FOOD_ESSENCE & data);
	void add_structure(unsigned int id,  PET_REFINE_ESSENCE & data);
	void add_structure(unsigned int id,  PET_ASSIST_REFINE_ESSENCE & data);
	void add_structure(unsigned int id,	 RENASCENCE_SKILL_CONFIG & data);
	void add_structure(unsigned int id,  RENASCENCE_PROP_CONFIG & data);
	void add_structure(unsigned int id,  AIRCRAFT_ESSENCE & data);
	void add_structure(unsigned int id,  FLY_ENERGYFOOD_ESSENCE & data);
	void add_structure(unsigned int id,  ITEM_TRADE_CONFIG & data);
	void add_structure(unsigned int id,  BOOK_ESSENCE & data);
	void add_structure(unsigned int id,	 PLAYER_SKILL_POINT_CONFIG & data);
	void add_structure(unsigned int id,  OFFLINE_TRUSTEE_ESSENCE & data);
	void add_structure(unsigned int id,	 EQUIP_SOUL_ESSENCE & data);
	void add_structure(unsigned int id,  EQUIP_SOUL_MELD_SERVICE & data);
	void add_structure(unsigned int id,  SPECIAL_NAME_ITEM_ESSENCE & data);
	void add_structure(unsigned int id,  RECYCLE_ITEM_CONFIG & data);
	void add_structure(unsigned int id,  SCORE_TO_RANK_CONFIG & data);
	void add_structure(unsigned int id,  BATTLE_DROP_CONFIG & data);
	void add_structure(unsigned int id,  BATTLE_DEPRIVE_CONFIG & data);
	void add_structure(unsigned int id,  BATTLE_SCORE_CONFIG & data);
	void add_structure(unsigned int id,  GIFT_BAG_ESSENCE & data);
	void add_structure(unsigned int id,  VIP_CARD_ESSENCE & data);
	void add_structure(unsigned int id,  INSTANCING_BATTLE_CONFIG & data);
	void add_structure(unsigned int id,  CHANGE_SHAPE_CARD_ESSENCE & data);
	void add_structure(unsigned int id,  CHANGE_SHAPE_STONE_ESSENCE & data);
	void add_structure(unsigned int id,  CHANGE_SHAPE_PROP_CONFIG & data);
	void add_structure(unsigned int id,	 ORIGINAL_SHAPE_CONFIG & data);
	void add_structure(unsigned int id,  LIFE_SKILL_CONFIG & data);
	void add_structure(unsigned int id,  ARENA_DROPTABLE_CONFIG & data);
	void add_structure(unsigned int id,  MERCENARY_CREDENTIAL_ESSENCE & data);
	void add_structure(unsigned int id,	 TELEPORTATION_ESSENCE & data);
	void add_structure(unsigned int id,	 TELEPORTATION_STONE_ESSENCE & data);
	void add_structure(unsigned int id,	 COMBINE_SKILL_EDIT_CONFIG & data);
	void add_structure(unsigned int id,  UPGRADE_EQUIP_CONFIG & data);
	void add_structure(unsigned int id,  UPGRADE_EQUIP_CONFIG_1 & data);
	void add_structure(unsigned int id,  UPGRADE_EQUIP_STONE_ESSENCE & data);
	void add_structure(unsigned int id,  NPC_CONSIGN_SERVICE & data);
	void add_structure(unsigned int id,  DEITY_CHARACTER_CONFIG & data);
	void add_structure(unsigned int id,  LOTTERY2_ESSENCE & data);
	void add_structure(unsigned int id,  GEM_CONFIG & data);//Added 2011-03-14.
	void add_structure(unsigned int id,  GEM_ESSENCE & data);
	void add_structure(unsigned int id,  GEM_SEAL_ESSENCE & data);
	void add_structure(unsigned int id,  GEM_DUST_ESSENCE & data);
	void add_structure(unsigned int id,  GEM_EXTRACT_CONFIG & data);
	void add_structure(unsigned int id,  GENERAL_ARTICLE_ESSENCE & data);
	void add_structure(unsigned int id,  LOTTERY3_ESSENCE & data); //Added 2011-07-01.
	void add_structure(unsigned int id,	 TRANSCRIPTION_CONFIG& data); //Added 2011-07-13.
	void add_structure(unsigned int id,	 ONLINE_GIFT_BAG_CONFIG& data); //Added 2011-12-06.
	void add_structure(unsigned int id,  SMALL_ONLINE_GIFT_BAG_ESSENCE& data); //Added 2011-12-06.
	void add_structure(unsigned int id,  SCROLL_REGION_CONFIG& data); //Added 2012-03-20.
	void add_structure(unsigned int id,	 SCROLL_UNLOCK_ESSENCE& data);//Added 2012-03-20.
	void add_structure(unsigned int id,	 SCROLL_DIG_COUNT_INC_ESSENCE& data);	//Added 2012-03-22.
	void add_structure(unsigned int id,	 TOWER_TRANSCRIPTION_CONFIG& data);		//Added 2012-03-31.
	void add_structure(unsigned int id,  TOWER_TRANSCRIPTION_PROPERTY_CONFIG& data);//Added 2012-03-31.
	void add_structure(unsigned int id,  RUNE_DATA_CONFIG& data);	// 2012.4.17
	void add_structure(unsigned int id,  RUNE_COMB_PROPERTY& data);	// 2012.4.17
	void add_structure(unsigned int id,  RUNE_EQUIP_ESSENCE& data);	// 2012.4.17
	void add_structure(unsigned int id,  LITTLE_PET_UPGRADE_CONFIG& data);	//Added 2012-04-19.
	void add_structure(unsigned int id,  RUNE_SLOT_ESSENCE& data);
	void add_structure(unsigned int id,  DROP_INTERNAL_CONFIG& data);
	void add_structure(unsigned int id,  PK2012_GUESS_CONFIG& data);
	void add_structure(unsigned int id,	 COLLISION_RAID_TRANSFIGURE_CONFIG& data);	//Added 2012-08-02.
	void add_structure(unsigned int id,  BOOTH_FIGURE_ITEM_ESSENCE& data);	// 2012.8.13
	void add_structure(unsigned int id,  FLAG_BUFF_ITEM_ESSENCE& data);	// 2012.8.20
	void add_structure(unsigned int id,  NPC_REPUTATION_SHOP_SERVICE & data);
	void add_structure(unsigned int id,  NPC_UI_TRANSFER_SERVICE & data);
	void add_structure(unsigned int id,  UI_TRANSFER_CONFIG & data);
	void add_structure(unsigned int id,  XINGZUO_ITEM_ESSENCE& data);
	void add_structure(unsigned int id,  XINGZUO_ENERGY_ITEM_ESSENCE& data);
	void add_structure(unsigned int id,  COLLISION_RAID_AWARD_CONFIG & data);
	void add_structure(unsigned int id,  CASH_MEDIC_MERGE_ITEM_ESSENCE& data);
	void add_structure(unsigned int id,  BUFF_AREA_CONFIG & data);
	void add_structure(unsigned int id,  LIVENESS_CONFIG & data);
	void add_structure(unsigned int id,  CHALLENGE_2012_CONFIG & data);
	void add_structure(unsigned int id,  SALE_PROMOTION_ITEM_ESSENCE & data);
	void add_structure(unsigned int id,  GIFT_BAG_LOTTERY_DELIVER_ESSENCE & data);
	void add_structure(unsigned int id,  LOTTERY_TANGYUAN_ITEM_ESSENCE & data);
	void add_structure(unsigned int id,  GIFT_PACK_ITEM_ESSENCE & data);
	void add_structure(unsigned int id,  TASK_SPECIAL_AWARD_CONFIG & data);
	void add_structure(unsigned int id,  PROP_ADD_CONFIG & data);
	void add_structure(unsigned int id,  PROP_ADD_MATERIAL_ITEM_ESSENCE & data);
	void add_structure(unsigned int id,  PROP_ADD_ITEM_ESSENCE & data);
	void add_structure(unsigned int id,  KING_WAR_CONFIG & data);
	void add_structure(unsigned int id,  JINFASHEN_TO_MONEY_CONFIG & data);
	void add_structure(unsigned int id,  BATTLE_201304_CONFIG & data);
	void add_structure(unsigned int id,  RUNE2013_ITEM_ESSENCE & data);
	void add_structure(unsigned int id,  RUNE2013_CONFIG & data);
	void add_structure(unsigned int id,  BASHE_AWARD_CONFIG & data);
	void add_structure(unsigned int id,  ARENA_SEASON_TIME_CONFIG & data);
	void add_structure(unsigned int id,  PERSONAL_LEVEL_AWARD_CONFIG & data);
	void add_structure(unsigned int id,  TEAM_SEASON_AWARD_CONFIG & data);
	void add_structure(unsigned int id,	 WEEK_CURRNCY_AWARD_CONFIG & data);
	void add_structure(unsigned int id,  COLORANT_ITEM_ESSENCE & data);
	void add_structure(unsigned int id,  INTERACTION_OBJECT_ESSENCE & data);
	void add_structure(unsigned int id,	 INTERACTION_OBJECT_HOOK_CONFIG & data);
	void add_structure(unsigned int id,	 COLORANT_CONFIG & data);
	void add_structure(unsigned int id,  FACTION_TRANSFER_CONFIG & data);
	void add_structure(unsigned int id,  BUILDING_REGION_CONFIG & data);
	void add_structure(unsigned int id,  BUILDING_ESSENCE & data);
	void add_structure(unsigned int id,  REGION_BLOCK_ESSENCE & data);
	void add_structure(unsigned int id,  PHASE_CONFIG & data);
	void add_structure(unsigned int id,  CROSS6V6_ITEM_EXCHANGE_CONFIG & data);
	void add_structure(unsigned int id,  TRANSCRIPT_STRATEGY_CONFIG & data);
	void add_structure(unsigned int id,  FACTION_SHOP_CONFIG & data);
	void add_structure(unsigned int id,  FACTION_AUCTION_CONFIG & data);
	void add_structure(unsigned int id,  FACTION_MONEY_SHOP_CONFIG & data);
	void add_structure(unsigned int id,  COMPOUND_MINE_ESSENCE & data);
	void add_structure(unsigned int id,  COLORANT_DEFAULT_CONFIG & data);
	//////////////////////////////////////////////////////////////////////////
public:
	unsigned int get_data_id(ID_SPACE idspace, unsigned int index, DATA_TYPE & datatype);
	unsigned int get_first_data_id(ID_SPACE idspace, DATA_TYPE & datatype);
	unsigned int get_next_data_id(ID_SPACE idspace, DATA_TYPE & datatype);

	unsigned int get_data_num(ID_SPACE idspace);

	DATA_TYPE get_data_type(unsigned int id, ID_SPACE idspace);
	const void * get_data_ptr(unsigned int id, ID_SPACE idspace, DATA_TYPE & datatype);

	int save_data(const char * pathname);
	int load_data(const char * pathname);
	
protected:

	template<class T>
	class array
	{
	public:
		inline size_t size() { return _v.size(); }
		inline void push_back(const T & x) { _v.push_back(x); }
		inline const T & operator [](size_t pos) const { return _v[pos]; }
		inline T & operator [](size_t pos) {return _v[pos];}
		int save(FILE * file)
		{
			size_t ts = sizeof(T);
			fwrite(&ts, sizeof(unsigned int), 1, file);
			size_t s = _v.size();
			fwrite(&s, sizeof(unsigned int), 1, file);
			if(s>0)		fwrite(&(_v[0]), sizeof(T), s, file);
			return 0;
		}
		int load(FILE * file)
		{
			size_t ts;
			if(fread(&ts, sizeof(unsigned int), 1, file) != 1)	return -1;
			if(ts != sizeof(T))
			{
				return -1;
			}
			size_t s;
			if(fread(&s, sizeof(unsigned int), 1, file) != 1)	return -1;
			if(s>0)
			{
				_v.reserve(s);
				T data;
				unsigned int i;
				for(i=0; i<s; i++)
				{
					if(fread(&data, sizeof(T), 1, file) != 1) 	return -1;
					_v.push_back(data);
				}
			}
			return 0;		
		}	
	protected:
		abase::vector<T> _v;
	};

	void setup_hash_map();

	void add_id_index(ID_SPACE idspace, unsigned int id, DATA_TYPE type, unsigned int pos, void * start);
	void add_id_index_essence(unsigned int id, DATA_TYPE type, unsigned int pos, void * start);
	void add_id_index_addon(unsigned int id, DATA_TYPE type, unsigned int pos, void * start);
	void add_id_index_talk(unsigned int id, DATA_TYPE type, unsigned int pos, void * start);
	void add_id_index_recipe(unsigned int id, DATA_TYPE type, unsigned int pos, void * start);
	void add_id_index_config(unsigned int id, DATA_TYPE type, unsigned int pos, void * start);
	
	static void NormalizeRandom(float* r, int n);

	array<EQUIPMENT_ADDON>			equipment_addon_array;
	array<EQUIPMENT_MAJOR_TYPE>		equipment_major_type_array;
	array<EQUIPMENT_SUB_TYPE>		equipment_sub_type_array;
	array<EQUIPMENT_ESSENCE>		equipment_essence_array;
	array<REFINE_TICKET_ESSENCE>	refine_ticket_essence_array;
	array<MEDICINE_MAJOR_TYPE>		medicine_major_type_array;
	array<MEDICINE_SUB_TYPE>		medicine_sub_type_array;
	array<MEDICINE_ESSENCE>			medicine_essence_array;
	array<MATERIAL_MAJOR_TYPE>		material_major_type_array;
	array<MATERIAL_SUB_TYPE>		material_sub_type_array;
	array<MATERIAL_ESSENCE>			material_essence_array;
	array<SKILLTOME_SUB_TYPE>		skilltome_sub_type_array;
	array<SKILLTOME_ESSENCE>		skilltome_essence_array;
	array<TRANSMITROLL_ESSENCE>		transmitroll_essence_array;
	array<LUCKYROLL_ESSENCE>		luckyroll_essence_array;
	array<TOWNSCROLL_ESSENCE>		townscroll_essence_array;
	array<REVIVESCROLL_ESSENCE>		revivescroll_essence_array;
	array<TASKMATTER_ESSENCE>		taskmatter_essence_array;
	array<DROPTABLE_TYPE>			droptable_type_array;
	array<DROPTABLE_ESSENCE>		droptable_essence_array;
	array<MONSTER_TYPE>				monster_type_array;
	array<MONSTER_ESSENCE>			monster_essence_array;		
	array<NPC_TALK_SERVICE>			npc_talk_service_array;
	array<NPC_SELL_SERVICE>			npc_sell_service_array;
	array<NPC_BUY_SERVICE>			npc_buy_service_array;
	array<NPC_TASK_IN_SERVICE>		npc_task_in_service_array;
	array<NPC_TASK_OUT_SERVICE>		npc_task_out_service_array;
	array<NPC_TASK_MATTER_SERVICE>	npc_task_matter_service_array;
	array<NPC_HEAL_SERVICE>			npc_heal_service_array;
	array<NPC_TRANSMIT_SERVICE>		npc_transmit_service_array;
	array<NPC_PROXY_SERVICE>		npc_proxy_service_array;
	array<NPC_STORAGE_SERVICE>		npc_storage_service_array;
	array<NPC_TYPE>					npc_type_array;
	array<NPC_ESSENCE>				npc_essence_array;
	array<talk_proc*>				talk_proc_array;
	array<RECIPE_MAJOR_TYPE>		recipe_major_type_array;
	array<RECIPE_SUB_TYPE>			recipe_sub_type_array;
	array<RECIPE_ESSENCE>			recipe_essence_array;
	array<ENEMY_FACTION_CONFIG>		enemy_faction_config_array;
	array<CHARACTER_CLASS_CONFIG>	character_class_config_array;
	array<PARAM_ADJUST_CONFIG>		param_adjust_config_array;
	array<PIE_LOVE_CONFIG>			pie_love_config_array;
	array<TASKDICE_ESSENCE>			taskdice_essence_array;
	array<TASKNORMALMATTER_ESSENCE>	tasknormalmatter_essence_array;
	array<PLAYER_LEVELEXP_CONFIG>	player_levelexp_config_array;
	array<MINE_TYPE>				mine_type_array;
	array<MINE_ESSENCE>				mine_essence_array;
	array<GM_GENERATOR_TYPE>		gm_generator_type_array;
	array<GM_GENERATOR_ESSENCE>		gm_generator_essence_array;
	array<FIREWORKS_ESSENCE>		fireworks_essence_array;
	array<NPC_WAR_TOWERBUILD_SERVICE> npc_war_towerbuild_service_array;
	array<PLAYER_SECONDLEVEL_CONFIG>	player_secondlevel_config_array;
	array<NPC_RESETPROP_SERVICE>	npc_resetprop_service_array;
	array<ESTONE_ESSENCE>			estone_essence_array;
	array<PSTONE_ESSENCE>			pstone_essence_array;
	array<SSTONE_ESSENCE>			sstone_essence_array;
	array<RECIPEROLL_MAJOR_TYPE>	reciperoll_major_type_array;
	array<RECIPEROLL_SUB_TYPE>		reciperoll_sub_type_array;
	array<RECIPEROLL_ESSENCE>		reciperoll_essence_array;
	array<SUITE_ESSENCE>			suite_essence_array;
	array<DOUBLE_EXP_ESSENCE>		double_exp_essence_array;
	array<DESTROYING_ESSENCE>		destroying_essence_array;
	array<NPC_EQUIPBIND_SERVICE>	npc_equipbind_service_array;
	array<NPC_EQUIPDESTROY_SERVICE>	npc_equipdestroy_service_array;
	array<NPC_EQUIPUNDESTROY_SERVICE> npc_equipundestroy_service_array;
	array<SKILLMATTER_ESSENCE>		skillmatter_essence_array;
	array<VEHICLE_ESSENCE>			vehicle_essence_array;
	array<COUPLE_JUMPTO_ESSENCE>	couple_jumpto_essence_array;
	array<LOTTERY_ESSENCE>			lottery_essence_array;
	array<CAMRECORDER_ESSENCE>		camrecorder_essence_array;
	array<TITLE_PROP_CONFIG>		title_prop_config_array;
	array<SPECIAL_ID_CONFIG>		special_id_config_array;
	array<TEXT_FIREWORKS_ESSENCE>	text_fireworks_essence_array;
	array<TALISMAN_MAINPART_ESSENCE> talisman_mainpart_essence_array;
	array<TALISMAN_EXPFOOD_ESSENCE> talisman_expfood_essence_array;
	array<TALISMAN_MERGEKATALYST_ESSENCE> talisman_mergekatalyst_essence_array;
	array<TALISMAN_ENERGYFOOD_ESSENCE> talisman_energyfood_essence_array;
	array<SPEAKER_ESSENCE>			speaker_essence_array;
	array<PLAYER_TALENT_CONFIG>		player_talent_config_array;
	array<POTENTIAL_TOME_ESSENCE>	potential_tome_essence_array;
	array<WAR_ROLE_CONFIG>			war_role_config_array;
	array<NPC_WAR_BUY_ARCHER_SERVICE> npc_war_buy_archer_service_array;
	array<SIEGE_ARTILLERY_SCROLL_ESSENCE> siege_artillery_scroll_essence_array;
	array<PET_BEDGE_ESSENCE>		pet_bedge_essence_array;
	array<PET_FOOD_ESSENCE>			pet_food_essence_array;
	array<PET_SKILL_ESSENCE>		pet_skill_essence_array;
	array<PET_ARMOR_ESSENCE>		pet_armor_essence_array;
	array<PET_AUTO_FOOD_ESSENCE>	pet_auto_food_essence_array;
	array<PET_REFINE_ESSENCE>		pet_refine_essence_array;
	array<PET_ASSIST_REFINE_ESSENCE>pet_assist_refine_essence_array;
	array<RENASCENCE_SKILL_CONFIG>	renasence_skill_config_array;
	array<RENASCENCE_PROP_CONFIG>	renasence_prop_config_array;
	array<AIRCRAFT_ESSENCE>			aircraft_essence_array;
	array<FLY_ENERGYFOOD_ESSENCE>	fly_energyfood_essence_array;
	array<ITEM_TRADE_CONFIG>		item_trade_config_array;
	array<BOOK_ESSENCE>				book_essence_array;
	array<PLAYER_SKILL_POINT_CONFIG>player_skill_point_config_array;
	array<OFFLINE_TRUSTEE_ESSENCE>	offline_trustee_essence_array;
	array<EQUIP_SOUL_ESSENCE>		equip_soul_essence_array;
	array<EQUIP_SOUL_MELD_SERVICE>  equip_soul_meld_service_array;
	array<SPECIAL_NAME_ITEM_ESSENCE> special_name_item_essence_array;
	array<RECYCLE_ITEM_CONFIG>		recycle_item_config_array;
	array<SCORE_TO_RANK_CONFIG>		score_to_rank_config_array;
	array<BATTLE_DROP_CONFIG>		battle_drop_config_array;
	array<BATTLE_DEPRIVE_CONFIG>    battle_deprive_config_array;
	array<BATTLE_SCORE_CONFIG>		battle_score_config_array;
	array<GIFT_BAG_ESSENCE>			gift_bag_essence_array;
	array<VIP_CARD_ESSENCE>			vip_card_essence_array;
	array<INSTANCING_BATTLE_CONFIG> instancing_battle_config_array;
	array<CHANGE_SHAPE_CARD_ESSENCE> change_shape_card_essence_array;
	array<CHANGE_SHAPE_STONE_ESSENCE> change_shape_stone_essence_array;
	array<CHANGE_SHAPE_PROP_CONFIG>	  change_shape_prop_config_array;
	array<ORIGINAL_SHAPE_CONFIG>	original_shape_config_array;
	array<LIFE_SKILL_CONFIG>		life_skill_config_array;
	array<ARENA_DROPTABLE_CONFIG>	arena_droptable_config_array;
	array<MERCENARY_CREDENTIAL_ESSENCE> mercenary_credential_essence_array;
	array<TELEPORTATION_ESSENCE>    teleportation_essence_array;
	array<TELEPORTATION_STONE_ESSENCE> teleportation_stone_essence_array;
	array<COMBINE_SKILL_EDIT_CONFIG> combine_skill_edit_config_array;
	array<UPGRADE_EQUIP_CONFIG>		upgrade_equip_config_array;
	array<UPGRADE_EQUIP_CONFIG_1>		upgrade_equip_config_1_array;
	array<UPGRADE_EQUIP_STONE_ESSENCE>	upgrade_equip_stone_essence_array;
	array<NPC_CONSIGN_SERVICE>		npc_consign_service_array;
	array<DEITY_CHARACTER_CONFIG>  deity_character_config_array;
	array<LOTTERY2_ESSENCE>			lottery2_essence_array;
	array<GEM_CONFIG>				gem_config_array;//Added 2011-03-14.
	array<GEM_ESSENCE>				gem_essence_array;
	array<GEM_SEAL_ESSENCE>			gem_seal_essence_array;
	array<GEM_DUST_ESSENCE>			gem_dust_essence_array;
	array<GEM_EXTRACT_CONFIG>		gem_extract_config_array;
	array<GENERAL_ARTICLE_ESSENCE>	general_article_essence_array;
	array<LOTTERY3_ESSENCE>			lottery3_essence_array; //Added 2011-07-01.
	array<TRANSCRIPTION_CONFIG>		transcription_config_array; //Added 2011-07-13.
	array<ONLINE_GIFT_BAG_CONFIG>	online_gift_bag_config_array;	//Added 2011-12-06.
	array<SMALL_ONLINE_GIFT_BAG_ESSENCE>	small_online_gift_bag_essence_array; //Added 2011-12-06.
	array<SCROLL_REGION_CONFIG>		scroll_region_config_array;		//Added 2012-03-20.
	array<SCROLL_UNLOCK_ESSENCE>	scroll_unlock_essence_array;	//Added 2012-03-20.
	array<SCROLL_DIG_COUNT_INC_ESSENCE>	scroll_dig_count_inc_essence_array;	//Added 2012-03-22.
	array<TOWER_TRANSCRIPTION_CONFIG>	tower_transcription_config_array;	//Added 2012-03-31.
	array<TOWER_TRANSCRIPTION_PROPERTY_CONFIG>	tower_transcription_property_config_array;	//Added 2012-03-31.
	array<RUNE_DATA_CONFIG>			rune_data_config_array;	// 2012.04.17
	array<RUNE_COMB_PROPERTY>		rune_comb_property_array;	// 2012.04.17
	array<RUNE_EQUIP_ESSENCE>				rune_equip_array;	// 2012.04.17
	array<LITTLE_PET_UPGRADE_CONFIG>	little_pet_upgrade_config_array;	//Added 2012-04-19.
	array<RUNE_SLOT_ESSENCE>		rune_slot_array;	//Added 2012-04-21.
	array<DROP_INTERNAL_CONFIG>		drop_interval_config_array;
	array<PK2012_GUESS_CONFIG>		pk2012_guess_config_array;
	array<COLLISION_RAID_TRANSFIGURE_CONFIG>	collision_raid_transfigure_config_array;	//Added 2012-08-02.
	array<BOOTH_FIGURE_ITEM_ESSENCE>		booth_figure_item_array;	//Added 2012-08-13.
	array<FLAG_BUFF_ITEM_ESSENCE>		flag_buff_item_array;	//Added 2012-08-20.
	array<NPC_REPUTATION_SHOP_SERVICE>		npc_reputation_shop_service_array;
	array<NPC_UI_TRANSFER_SERVICE>		npc_ui_transfer_service_array;
	array<UI_TRANSFER_CONFIG>	ui_transfer_config_array;
	array<XINGZUO_ITEM_ESSENCE>		xingzuo_item_essence_array;
	array<XINGZUO_ENERGY_ITEM_ESSENCE>		xingzuo_energy_item_essence_array;
	array<COLLISION_RAID_AWARD_CONFIG>		collision_raid_award_config_array;
	array<CASH_MEDIC_MERGE_ITEM_ESSENCE>	cash_medic_merge_item_essence_array;
	array<BUFF_AREA_CONFIG>					buff_area_config_array;
	array<LIVENESS_CONFIG>					liveness_config_array;
	array<CHALLENGE_2012_CONFIG>			challenge_2012_config_array;
	array<SALE_PROMOTION_ITEM_ESSENCE>		sale_promotion_item_essence_array;
	array<GIFT_BAG_LOTTERY_DELIVER_ESSENCE>	gift_bag_lottery_deliver_essence_array;
	array<LOTTERY_TANGYUAN_ITEM_ESSENCE>	lottery_tangyuan_item_essence_array;
	array<GIFT_PACK_ITEM_ESSENCE>			gift_pack_item_essence_array;
	array<TASK_SPECIAL_AWARD_CONFIG>		task_special_award_config_array;
	array<PROP_ADD_CONFIG>					prop_add_config_array;
	array<PROP_ADD_MATERIAL_ITEM_ESSENCE>	prop_add_material_item_essence_array;
	array<PROP_ADD_ITEM_ESSENCE>			prop_add_item_essence_array;
	array<KING_WAR_CONFIG>					king_war_config_array;
	array<JINFASHEN_TO_MONEY_CONFIG>		jinfashen_to_money_config_array;
	array<BATTLE_201304_CONFIG>				battle_201304_config_array;
	array<RUNE2013_ITEM_ESSENCE>			rune2013_item_essence_array;
	array<RUNE2013_CONFIG>					rune2013_config_array;
	array<BASHE_AWARD_CONFIG>				bashe_award_config_array;
	array<ARENA_SEASON_TIME_CONFIG>			arena_season_time_config_array;
	array<PERSONAL_LEVEL_AWARD_CONFIG>		personal_level_award_config_array;
	array<TEAM_SEASON_AWARD_CONFIG>			team_season_award_config_array;
	array<WEEK_CURRNCY_AWARD_CONFIG>		week_currency_award_config_array;
	array<COLORANT_ITEM_ESSENCE>			colorant_item_essence_array;
	array<INTERACTION_OBJECT_ESSENCE>		interaction_object_essence_array;
	array<INTERACTION_OBJECT_HOOK_CONFIG>	interaction_object_hook_config_array;
	array<COLORANT_CONFIG>					colorant_config_array;
	array<FACTION_TRANSFER_CONFIG>			faction_transfer_config_array;
	array<BUILDING_REGION_CONFIG>			building_region_config_array;
	array<BUILDING_ESSENCE>					building_essence_array;
	array<REGION_BLOCK_ESSENCE>				region_block_essence_array;
	array<PHASE_CONFIG>						phase_config_array;
	array<CROSS6V6_ITEM_EXCHANGE_CONFIG>    cross6v6_item_exchange_config_array;
	array<TRANSCRIPT_STRATEGY_CONFIG>		transcript_strategy_config_array;
	array<FACTION_SHOP_CONFIG>				faction_shop_config_array;
	array<FACTION_AUCTION_CONFIG>			faction_auction_config_array;
	array<FACTION_MONEY_SHOP_CONFIG>		faction_money_shop_config_array;
	array<COMPOUND_MINE_ESSENCE>			compound_mine_essence_array;
	array<COLORANT_DEFAULT_CONFIG>          colorant_default_config_array;
	// the global hash_map for each id space
private:
	struct LOCATION
	{
		DATA_TYPE		type;
		unsigned int	pos;
	};
	typedef abase::hash_map<unsigned int, LOCATION> IDToLOCATIONMap;
	
	IDToLOCATIONMap				essence_id_index_map;
	IDToLOCATIONMap				addon_id_index_map;
	IDToLOCATIONMap				talk_id_index_map;
	IDToLOCATIONMap				recipe_id_index_map;
	IDToLOCATIONMap				config_id_index_map;


	IDToLOCATIONMap::iterator	esssence_id_index_itr;
	IDToLOCATIONMap::iterator	addon_id_index_itr;
	IDToLOCATIONMap::iterator	talk_id_index_itr;
	IDToLOCATIONMap::iterator	recipe_id_index_itr;
	IDToLOCATIONMap::iterator	config_id_index_itr;

	array<void *>				start_ptr_array;
	array<size_t>				type_size_array;
};

#endif
