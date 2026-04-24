#include "elementdataman.h"
using namespace abase;

#include <stdio.h>
#include <math.h>
#include <time.h>

#ifdef _WINDOWS

#include "EC_MD5Hash.h"

#endif

const char * DataTypeName[DT_MAX+1] =
{
	"DT_INVALID",		
	"DT_EQUIPMENT_ADDON",
	"DT_EQUIPMENT_MAJOR_TYPE",
	"DT_EQUIPMENT_SUB_TYPE",
	"DT_EQUIPMENT_ESSENCE",
	"DT_REFINE_TICKET_ESSENCE",
	"DT_MEDICINE_MAJOR_TYPE",
	"DT_MEDICINE_SUB_TYPE",
	"DT_MEDICINE_ESSENCE",
	"DT_MATERIAL_MAJOR_TYPE",
	"DT_MATERIAL_SUB_TYPE",
	"DT_MATERIAL_ESSENCE",
	"DT_SKILLTOME_SUB_TYPE",
	"DT_SKILLTOME_ESSENCE",	
	"DT_TRANSMITROLL_ESSENCE",
	"DT_LUCKYROLL_ESSENCE",
	"DT_TOWNSCROLL_ESSENCE",
	"DT_REVIVESCROLL_ESSENCE",
	"DT_TASKMATTER_ESSENCE",
	"DT_DROPTABLE_TYPE",
	"DT_DROPTABLE_ESSENCE",
	"DT_MONSTER_TYPE",
	"DT_MONSTER_ESSENCE",	
	"DT_NPC_TALK_SERVICE",
	"DT_NPC_SELL_SERVICE",
	"DT_NPC_BUY_SERVICE",
	"DT_NPC_TASK_IN_SERVICE",
	"DT_NPC_TASK_OUT_SERVICE",
	"DT_NPC_TASK_MATTER_SERVICE",
	"DT_NPC_HEAL_SERVICE",
	"DT_NPC_TRANSMIT_SERVICE",	
	"DT_NPC_PROXY_SERVICE",
	"DT_NPC_STORAGE_SERVICE",
	"DT_NPC_TYPE",
	"DT_NPC_ESSENCE",
	"DT_TALK_PROC",
	"DT_RECIPE_MAJOR_TYPE",
	"DT_RECIPE_SUB_TYPE",
	"DT_RECIPE_ESSENCE",
	"DT_ENEMY_FACTION_CONFIG",	
	"DT_CHARACTER_CLASS_CONFIG",
	"DT_PARAM_ADJUST_CONFIG",
	"DT_PIE_LOVE_CONFIG",
	"DT_TASKDICE_ESSENCE",
	"DT_TASKNORMALMATTER_ESSENCE",
	"DT_PLAYER_LEVELEXP_CONFIG",
	"DT_MINE_TYPE",
	"DT_MINE_ESSENCE",
	"DT_GM_GENERATOR_TYPE",
	"DT_GM_GENERATOR_ESSENCE",
	"DT_FIREWORKS_ESSENCE",
	"DT_NPC_WAR_TOWERBUILD_SERVICE",
	"DT_PLAYER_SECONDLEVEL_CONFIG",
	"DT_NPC_RESETPROP_SERVICE",
	"DT_ESTONE_ESSENCE",
	"DT_PSTONE_ESSENCE",
	"DT_SSTONE_ESSENCE",
	"DT_RECIPEROLL_MAJOR_TYPE",
	"DT_RECIPEROLL_SUB_TYPE",
	"DT_RECIPEROLL_ESSENCE",
	"DT_SUITE_ESSENCE",
	"DT_DOUBLE_EXP_ESSENCE",
	"DT_DESTROYING_ESSENCE",
	"DT_NPC_EQUIPBIND_SERVICE",
	"DT_NPC_EQUIPDESTROY_SERVICE",
	"DT_NPC_EQUIPUNDESTROY_SERVICE",
	"DT_SKILLMATTER_ESSENCE",
	"DT_VEHICLE_ESSENCE",
	"DT_COUPLE_JUMPTO_ESSENCE",
	"DT_LOTTERY_ESSENCE",
	"DT_CAMRECORDER_ESSENCE",
	"DT_TITLE_PROP_CONFIG",
	"DT_SPECIAL_ID_CONFIG",
	"DT_TEXT_FIREWORKS_ESSENCE",
	"DT_TALISMAN_MAINPART_ESSENCE",
	"DT_TALISMAN_EXPFOOD_ESSENCE",
	"DT_TALISMAN_MERGEKATALYST_ESSENCE",
	"DT_TALISMAN_ENERGYFOOD_ESSENCE",
	"DT_SPEAKER_ESSENCE",
	"DT_PLAYER_TALENT_CONFIG",
	"DT_POTENTIAL_TOME_ESSENCE",
	"DT_WAR_ROLE_CONFIG",
	"DT_NPC_WAR_BUY_ARCHER_SERVICE",
	"DT_SIEGE_ARTILLERY_SCROLL_ESSENCE",
	"DT_PET_BEDGE_ESSENCE",
	"DT_PET_FOOD_ESSENCE",
	"DT_PET_SKILL_ESSENCE",
	"DT_PET_ARMOR_ESSENCE",
	"DT_AUTO_FOOD_ESSENCE",
	"DT_PET_REFINE_ESSENCE",
	"DT_PET_ASSIST_REFINE_ESSENCE",
	"DT_RENASCENCE_SKILL_CONFIG",
	"DT_RENASCENCE_PROP_CONFIG",
	"DT_AIRCRAFT_ESSENCE",
	"DT_FLY_ENERGYFOOD_ESSENCE",
	"DT_ITEM_TRADE_CONFIG",
	"DT_BOOK_ESSENCE",
	"DT_PLAYER_SKILL_POINT_CONFIG",
	"DT_OFFLINE_TRUSTEE_ESSENCE",
	"DT_EQUIP_SOUL_ESSENCE",
	"DT_EQUIP_SOUL_MELD_SERVICE",
	"DT_SPECIAL_NAME_ITEM_ESSENCE",
	"DT_RECYCLE_ITEM_CONFIG",
	"DT_SCORE_TO_RANK_CONFIG",
	"DT_BATTLE_DROP_CONFIG",
	"DT_BATTLE_DEPRIVE_CONFIG",
	"DT_BATTLE_SCORE_CONFIG",
	"DT_GIFT_BAG_ESSENCE",
	"DT_VIP_CARD_ESSENCE",
	"DT_INSTANCING_BATTLE_CONFIG",
	"DT_CHANGE_SHAPE_CARD_ESSENCE",
	"DT_CHANGE_SHAPE_STONE_ESSENCE",
	"DT_CHANGE_SHAPE_PROP_CONFIG",
	"DT_ORIGINAL_SHAPE_CONFIG",
	"DT_LIFE_SKILL_CONFIG",
	"DT_ARENA_DROPTABLE_CONFIG",
	"DT_MERCENARY_CREDENTIAL_ESSENCE",
	"DT_TELEPORTATION_ESSENCE",
	"DT_TELEPORTATION_STONE_ESSENCE",
	"DT_COMBINE_SKILL_EDIT_CONFIG",
	"DT_UPGRADE_EQUIP_CONFIG",
	"DT_UPGRADE_EQUIP_CONFIG_1",
	"DT_UPGRADE_EQUIP_STONE_ESSENCE",
	"DT_NPC_CONSIGN_SERVICE",
	"DT_DEITY_CHARACTER_CONFIG",
	"DT_LOTTERY2_ESSENCE",
	"DT_GEM_CONFIG",
	"DT_GEM_ESSENCE",
	"DT_GEM_SEAL_ESSENCE",
	"DT_GEM_DUST_ESSENCE",
	"DT_GEM_EXTRACT_CONFIG",
	"DT_GENERAL_ARTICLE_ESSENCE",
	"DT_LOTTERY3_ESSENCE",		//Added 2011-07-01.
	"DT_TRANSCRIPTION_CONFIG",	//Added 2011-07-13. 
	"DT_ONLINE_GIFT_BAG_CONFIG",//Added 2011-12-06.
	"DT_SMALL_ONLINE_GIFT_BAG_ESSENCE", //Added 2011-12-06.
	"DT_SCROLL_REGION_CONFIG", //Added 2012-03-20.
	"DT_SCROLL_UNLOCK_ESSENCE",//Added 2012-03-20.
	"DT_SCROLL_DIG_COUNT_INC_ESSENCE",	//Added 2012-03-22.
	"DT_TOWER_TRANSCRIPTION_CONFIG",	//Added 2012-03-31.
	"DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG",//Added 2012-03-31.
	"DT_RUNE_DATA_CONFIG",//Added 2012.4.17.
	"DT_RUNE_COMB_PROPERTY",//Added 2012.4.17.
	"DT_RUNE_EQUIP_ESSENCE",//Added 2012.4.17.
	"DT_LITTLE_PET_UPGRADE_CONFIG", //Added 2012-04-19.
	"DT_RUNE_SLOT_ESSENCE", //Added 2012-04-21.
	"DT_DROP_INTERNAL_CONFIG",
	"DT_PK2012_GUESS_CONFIG",
	"DT_COLLISION_RAID_TRANSFIGURE_CONFIG",	//Added 2012-08-02.
	"DT_BOOTH_FIGURE_ITEM_ESSENCE",
	"DT_FLAG_BUFF_ITEM_ESSENCE",
	"DT_NPC_REPUTATION_SHOP_SERVICE",
	"DT_NPC_UI_TRANSFER_SERVICE",
	"DT_UI_TRANSFER_CONFIG",
	"DT_XINGZUO_ITEM_ESSENCE",
	"DT_XINGZUO_ENERGY_ITEM_ESSENCE",
	"DT_COLLISION_RAID_AWARD_CONFIG",
	"DT_CASH_MEDIC_MERGE_ITEM_ESSENCE",
	"DT_BUFF_AREA_CONFIG",
	"DT_LIVENESS_CONFIG",
	"DT_CHALLENGE_2012_CONFIG",
	"DT_SALE_PROMOTION_ITEM_ESSENCE",
	"DT_GIFT_BAG_LOTTERY_DELIVER_ESSENCE",
	"DT_LOTTERY_TANGYUAN_ITEM_ESSENCE",
	"DT_TASK_SPECIAL_AWARD_CONFIG",
	"DT_GIFT_PACK_ITEM_ESSENCE",	
	"DT_PROP_ADD_CONFIG",
	"DT_PROP_ADD_MATERIAL_ITEM_ESSENCE",
	"DT_PROP_ADD_ITEM_ESSENCE",
	"DT_KING_WAR_CONFIG",
	"DT_JINFASHEN_TO_MONEY_CONFIG",
	"DT_BATTLE_201304_CONFIG",
	"DT_RUNE2013_ITEM_ESSENCE",
	"DT_RUNE2013_CONFIG",
	"DT_BASHE_AWARD_CONFIG",
	"DT_ARENA_SEASON_TIME_CONFIG",
	"DT_PERSONAL_LEVEL_AWARD_CONFIG",
	"DT_TEAM_SEASON_AWARD_CONFIG",
	"DT_WEEK_CURRNCY_AWARD_CONFIG",
	"DT_COLORANT_ITEM_ESSENCE"
	"DT_INTERACTION_OBJECT_ESSENCE",
	"DT_INTERACTION_OBJECT_HOOK_CONFIG",
	"DT_COLORANT_CONFIG",
	"DT_FACTION_TRANSFER_CONFIG",
	"DT_BUILDING_REGION_CONFIG",
	"DT_BUILDING_ESSENCE",
	"DT_REGION_BLOCK_ESSENCE",
	"DT_PHASE_CONFIG",
	"DT_CROSS6V6_ITEM_EXCHANGE_CONFIG",
	"DT_TRANSCRIPT_STRATEGY_CONFIG",
	"DT_FACTION_SHOP_CONFIG",
	"DT_FACTION_AUCTION_CONFIG",
	"DT_FACTION_MONEY_SHOP_CONFIG",
	"DT_COMPOUND_MINE_ESSENCE",
	"DT_COLORANT_DEFAULT_CONFIG",
	"DT_MAX",
};

void eliminate_zero_item(unsigned char * data, size_t stride, size_t count)
{
	unsigned char * d = data;
	unsigned char * p = data;

	for(unsigned int i=0; i<count; i++)
	{
		int flag = *(int *)d;
		if( flag == 0 )
		{
			d += stride;
			continue;
		}

		if( p != d )
			memcpy(p, d, stride);

		d += stride;
		p += stride;
	}

	if( data + stride * count - p )
		memset(p, 0, data + stride * count - p);
}

void elementdataman::NormalizeRandom(float* r, int n)
{
	double t=0;
	float * slider = r;
	for(int i=0; i<n; i++)
	{
		t += *slider;
		slider++;
	}
	if(t>0.00001)
	{
		slider = r;
		for(int i=0; i<n; i++)
		{
			*slider /= t;
			slider++;
		}
	}
	else
	{
		memset(r, 0, sizeof(float) * n);
		*r = 1.f;
	}
}

elementdataman::elementdataman()
{
	unsigned int i;
	for(i=0; i<DT_MAX; i++)
	{
		start_ptr_array.push_back(NULL);
	}

	type_size_array.push_back(0);
	type_size_array.push_back(sizeof(EQUIPMENT_ADDON));
	type_size_array.push_back(sizeof(EQUIPMENT_MAJOR_TYPE));
	type_size_array.push_back(sizeof(EQUIPMENT_SUB_TYPE));
	type_size_array.push_back(sizeof(EQUIPMENT_ESSENCE));
	type_size_array.push_back(sizeof(REFINE_TICKET_ESSENCE));
	type_size_array.push_back(sizeof(MEDICINE_MAJOR_TYPE));
	type_size_array.push_back(sizeof(MEDICINE_SUB_TYPE));
	type_size_array.push_back(sizeof(MEDICINE_ESSENCE));
	type_size_array.push_back(sizeof(MATERIAL_MAJOR_TYPE));
	type_size_array.push_back(sizeof(MATERIAL_SUB_TYPE));
	type_size_array.push_back(sizeof(MATERIAL_ESSENCE));
	type_size_array.push_back(sizeof(SKILLTOME_SUB_TYPE));
	type_size_array.push_back(sizeof(SKILLTOME_ESSENCE));		
	type_size_array.push_back(sizeof(TRANSMITROLL_ESSENCE));
	type_size_array.push_back(sizeof(LUCKYROLL_ESSENCE));
	type_size_array.push_back(sizeof(TOWNSCROLL_ESSENCE));
	type_size_array.push_back(sizeof(REVIVESCROLL_ESSENCE));
	type_size_array.push_back(sizeof(TASKMATTER_ESSENCE));
	type_size_array.push_back(sizeof(DROPTABLE_TYPE));
	type_size_array.push_back(sizeof(DROPTABLE_ESSENCE));
	type_size_array.push_back(sizeof(MONSTER_TYPE));
	type_size_array.push_back(sizeof(MONSTER_ESSENCE));	
	type_size_array.push_back(sizeof(NPC_TALK_SERVICE));
	type_size_array.push_back(sizeof(NPC_SELL_SERVICE));
	type_size_array.push_back(sizeof(NPC_BUY_SERVICE));
	type_size_array.push_back(sizeof(NPC_TASK_IN_SERVICE));
	type_size_array.push_back(sizeof(NPC_TASK_OUT_SERVICE));
	type_size_array.push_back(sizeof(NPC_TASK_MATTER_SERVICE));
	type_size_array.push_back(sizeof(NPC_HEAL_SERVICE));
	type_size_array.push_back(sizeof(NPC_TRANSMIT_SERVICE));
	type_size_array.push_back(sizeof(NPC_PROXY_SERVICE));
	type_size_array.push_back(sizeof(NPC_STORAGE_SERVICE));
	type_size_array.push_back(sizeof(NPC_TYPE));	
	type_size_array.push_back(sizeof(NPC_ESSENCE));
	type_size_array.push_back(sizeof(talk_proc*));
	type_size_array.push_back(sizeof(RECIPE_MAJOR_TYPE));
	type_size_array.push_back(sizeof(RECIPE_SUB_TYPE));
	type_size_array.push_back(sizeof(RECIPE_ESSENCE));
	type_size_array.push_back(sizeof(ENEMY_FACTION_CONFIG));
	type_size_array.push_back(sizeof(CHARACTER_CLASS_CONFIG));
	type_size_array.push_back(sizeof(PARAM_ADJUST_CONFIG));
	type_size_array.push_back(sizeof(PIE_LOVE_CONFIG));
	type_size_array.push_back(sizeof(TASKDICE_ESSENCE));
	type_size_array.push_back(sizeof(TASKNORMALMATTER_ESSENCE));
	type_size_array.push_back(sizeof(PLAYER_LEVELEXP_CONFIG));
	type_size_array.push_back(sizeof(MINE_TYPE));
	type_size_array.push_back(sizeof(MINE_ESSENCE));
	type_size_array.push_back(sizeof(GM_GENERATOR_TYPE));
	type_size_array.push_back(sizeof(GM_GENERATOR_ESSENCE));
	type_size_array.push_back(sizeof(FIREWORKS_ESSENCE));
	type_size_array.push_back(sizeof(NPC_WAR_TOWERBUILD_SERVICE));
	type_size_array.push_back(sizeof(PLAYER_SECONDLEVEL_CONFIG));
	type_size_array.push_back(sizeof(NPC_RESETPROP_SERVICE));
	type_size_array.push_back(sizeof(ESTONE_ESSENCE));
	type_size_array.push_back(sizeof(PSTONE_ESSENCE));
	type_size_array.push_back(sizeof(SSTONE_ESSENCE));
	type_size_array.push_back(sizeof(RECIPEROLL_MAJOR_TYPE));
	type_size_array.push_back(sizeof(RECIPEROLL_SUB_TYPE));
	type_size_array.push_back(sizeof(RECIPEROLL_ESSENCE));
	type_size_array.push_back(sizeof(SUITE_ESSENCE));
	type_size_array.push_back(sizeof(DOUBLE_EXP_ESSENCE));
	type_size_array.push_back(sizeof(DESTROYING_ESSENCE));
	type_size_array.push_back(sizeof(NPC_EQUIPBIND_SERVICE));
	type_size_array.push_back(sizeof(NPC_EQUIPDESTROY_SERVICE));
	type_size_array.push_back(sizeof(NPC_EQUIPUNDESTROY_SERVICE));
	type_size_array.push_back(sizeof(SKILLMATTER_ESSENCE));
	type_size_array.push_back(sizeof(VEHICLE_ESSENCE));
	type_size_array.push_back(sizeof(COUPLE_JUMPTO_ESSENCE));
	type_size_array.push_back(sizeof(LOTTERY_ESSENCE));
	type_size_array.push_back(sizeof(CAMRECORDER_ESSENCE));
	type_size_array.push_back(sizeof(TITLE_PROP_CONFIG));
	type_size_array.push_back(sizeof(SPECIAL_ID_CONFIG));
	type_size_array.push_back(sizeof(TEXT_FIREWORKS_ESSENCE));
	type_size_array.push_back(sizeof(TALISMAN_MAINPART_ESSENCE));
	type_size_array.push_back(sizeof(TALISMAN_EXPFOOD_ESSENCE));
	type_size_array.push_back(sizeof(TALISMAN_MERGEKATALYST_ESSENCE));
	type_size_array.push_back(sizeof(TALISMAN_ENERGYFOOD_ESSENCE));
	type_size_array.push_back(sizeof(SPEAKER_ESSENCE));
	type_size_array.push_back(sizeof(PLAYER_TALENT_CONFIG));
	type_size_array.push_back(sizeof(POTENTIAL_TOME_ESSENCE));
	type_size_array.push_back(sizeof(WAR_ROLE_CONFIG));
	type_size_array.push_back(sizeof(NPC_WAR_BUY_ARCHER_SERVICE));
	type_size_array.push_back(sizeof(SIEGE_ARTILLERY_SCROLL_ESSENCE));
	type_size_array.push_back(sizeof(PET_BEDGE_ESSENCE));
	type_size_array.push_back(sizeof(PET_FOOD_ESSENCE));
	type_size_array.push_back(sizeof(PET_SKILL_ESSENCE));
	type_size_array.push_back(sizeof(PET_ARMOR_ESSENCE));
	type_size_array.push_back(sizeof(PET_AUTO_FOOD_ESSENCE));
	type_size_array.push_back(sizeof(PET_REFINE_ESSENCE));
	type_size_array.push_back(sizeof(PET_ASSIST_REFINE_ESSENCE));
	type_size_array.push_back(sizeof(RENASCENCE_SKILL_CONFIG));
	type_size_array.push_back(sizeof(RENASCENCE_PROP_CONFIG));
	type_size_array.push_back(sizeof(AIRCRAFT_ESSENCE));
	type_size_array.push_back(sizeof(FLY_ENERGYFOOD_ESSENCE));
	type_size_array.push_back(sizeof(ITEM_TRADE_CONFIG));
	type_size_array.push_back(sizeof(BOOK_ESSENCE));
	type_size_array.push_back(sizeof(PLAYER_SKILL_POINT_CONFIG));
	type_size_array.push_back(sizeof(OFFLINE_TRUSTEE_ESSENCE));
	type_size_array.push_back(sizeof(EQUIP_SOUL_ESSENCE));
	type_size_array.push_back(sizeof(EQUIP_SOUL_MELD_SERVICE));
	type_size_array.push_back(sizeof(SPECIAL_NAME_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(RECYCLE_ITEM_CONFIG));
	type_size_array.push_back(sizeof(SCORE_TO_RANK_CONFIG));
	type_size_array.push_back(sizeof(BATTLE_DROP_CONFIG));
	type_size_array.push_back(sizeof(BATTLE_DEPRIVE_CONFIG));
	type_size_array.push_back(sizeof(BATTLE_SCORE_CONFIG));
	type_size_array.push_back(sizeof(GIFT_BAG_ESSENCE));
	type_size_array.push_back(sizeof(VIP_CARD_ESSENCE));
	type_size_array.push_back(sizeof(INSTANCING_BATTLE_CONFIG));
	type_size_array.push_back(sizeof(CHANGE_SHAPE_CARD_ESSENCE));
	type_size_array.push_back(sizeof(CHANGE_SHAPE_STONE_ESSENCE));
	type_size_array.push_back(sizeof(CHANGE_SHAPE_PROP_CONFIG));
	type_size_array.push_back(sizeof(ORIGINAL_SHAPE_CONFIG));
	type_size_array.push_back(sizeof(LIFE_SKILL_CONFIG));
	type_size_array.push_back(sizeof(ARENA_DROPTABLE_CONFIG));
	type_size_array.push_back(sizeof(MERCENARY_CREDENTIAL_ESSENCE));
	type_size_array.push_back(sizeof(TELEPORTATION_ESSENCE));
	type_size_array.push_back(sizeof(TELEPORTATION_STONE_ESSENCE));
	type_size_array.push_back(sizeof(COMBINE_SKILL_EDIT_CONFIG));
	type_size_array.push_back(sizeof(UPGRADE_EQUIP_CONFIG));
	type_size_array.push_back(sizeof(UPGRADE_EQUIP_CONFIG_1));
	type_size_array.push_back(sizeof(UPGRADE_EQUIP_STONE_ESSENCE));
	type_size_array.push_back(sizeof(NPC_CONSIGN_SERVICE));
	type_size_array.push_back(sizeof(DEITY_CHARACTER_CONFIG));
	type_size_array.push_back(sizeof(LOTTERY2_ESSENCE));
	type_size_array.push_back(sizeof(GEM_CONFIG));//Added 2011-03-14.
	type_size_array.push_back(sizeof(GEM_ESSENCE));
	type_size_array.push_back(sizeof(GEM_SEAL_ESSENCE));
	type_size_array.push_back(sizeof(GEM_DUST_ESSENCE));
	type_size_array.push_back(sizeof(GEM_EXTRACT_CONFIG));
	type_size_array.push_back(sizeof(GENERAL_ARTICLE_ESSENCE));
	type_size_array.push_back(sizeof(LOTTERY3_ESSENCE)); //Added 2011-07-01.
	type_size_array.push_back(sizeof(TRANSCRIPTION_CONFIG)); //Added 2011-07-13.
	type_size_array.push_back(sizeof(ONLINE_GIFT_BAG_CONFIG)); //Added 2011-12-06.
	type_size_array.push_back(sizeof(SMALL_ONLINE_GIFT_BAG_ESSENCE)); //Added 2011-12-06.
	type_size_array.push_back(sizeof(SCROLL_REGION_CONFIG));	//Added 2012-03-20.
	type_size_array.push_back(sizeof(SCROLL_UNLOCK_ESSENCE));	//Added 2012-03-20.
	type_size_array.push_back(sizeof(SCROLL_DIG_COUNT_INC_ESSENCE));	//Added 2012-03-22.
	type_size_array.push_back(sizeof(TOWER_TRANSCRIPTION_CONFIG));		//Added 2012-03-31.
	type_size_array.push_back(sizeof(TOWER_TRANSCRIPTION_PROPERTY_CONFIG));//Added 2012-03-31.
	type_size_array.push_back(sizeof(RUNE_DATA_CONFIG));//Added 2012.04.17.
	type_size_array.push_back(sizeof(RUNE_COMB_PROPERTY));//Added 2012.04.17.
	type_size_array.push_back(sizeof(RUNE_EQUIP_ESSENCE));//Added 2012.04.17.
	type_size_array.push_back(sizeof(LITTLE_PET_UPGRADE_CONFIG));	//Added 2012-04-19.
	type_size_array.push_back(sizeof(RUNE_SLOT_ESSENCE));
	type_size_array.push_back(sizeof(DROP_INTERNAL_CONFIG));
	type_size_array.push_back(sizeof(PK2012_GUESS_CONFIG));
	type_size_array.push_back(sizeof(COLLISION_RAID_TRANSFIGURE_CONFIG));	//Added 2012-08-02.
	type_size_array.push_back(sizeof(BOOTH_FIGURE_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(FLAG_BUFF_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(NPC_REPUTATION_SHOP_SERVICE));
	type_size_array.push_back(sizeof(NPC_UI_TRANSFER_SERVICE));
	type_size_array.push_back(sizeof(UI_TRANSFER_CONFIG));
	type_size_array.push_back(sizeof(XINGZUO_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(XINGZUO_ENERGY_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(COLLISION_RAID_AWARD_CONFIG));
	type_size_array.push_back(sizeof(CASH_MEDIC_MERGE_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(BUFF_AREA_CONFIG));
	type_size_array.push_back(sizeof(LIVENESS_CONFIG));
	type_size_array.push_back(sizeof(CHALLENGE_2012_CONFIG));
	type_size_array.push_back(sizeof(SALE_PROMOTION_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(GIFT_BAG_LOTTERY_DELIVER_ESSENCE));
	type_size_array.push_back(sizeof(LOTTERY_TANGYUAN_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(TASK_SPECIAL_AWARD_CONFIG));
	type_size_array.push_back(sizeof(GIFT_PACK_ITEM_ESSENCE));	
	type_size_array.push_back(sizeof(PROP_ADD_MATERIAL_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(PROP_ADD_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(PROP_ADD_CONFIG));
	type_size_array.push_back(sizeof(KING_WAR_CONFIG));
	type_size_array.push_back(sizeof(JINFASHEN_TO_MONEY_CONFIG));
	type_size_array.push_back(sizeof(BATTLE_201304_CONFIG));
	type_size_array.push_back(sizeof(RUNE2013_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(RUNE2013_CONFIG));
	type_size_array.push_back(sizeof(BASHE_AWARD_CONFIG));
	type_size_array.push_back(sizeof(ARENA_SEASON_TIME_CONFIG));
	type_size_array.push_back(sizeof(PERSONAL_LEVEL_AWARD_CONFIG));
	type_size_array.push_back(sizeof(TEAM_SEASON_AWARD_CONFIG));
	type_size_array.push_back(sizeof(WEEK_CURRNCY_AWARD_CONFIG));
	type_size_array.push_back(sizeof(COLORANT_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(INTERACTION_OBJECT_ESSENCE));
	type_size_array.push_back(sizeof(INTERACTION_OBJECT_HOOK_CONFIG));
	type_size_array.push_back(sizeof(COLORANT_CONFIG));
	type_size_array.push_back(sizeof(FACTION_TRANSFER_CONFIG));
	type_size_array.push_back(sizeof(BUILDING_REGION_CONFIG));
	type_size_array.push_back(sizeof(BUILDING_ESSENCE));
	type_size_array.push_back(sizeof(REGION_BLOCK_ESSENCE));
	type_size_array.push_back(sizeof(PHASE_CONFIG));
	type_size_array.push_back(sizeof(CROSS6V6_ITEM_EXCHANGE_CONFIG));
	type_size_array.push_back(sizeof(TRANSCRIPT_STRATEGY_CONFIG));
	type_size_array.push_back(sizeof(FACTION_SHOP_CONFIG));
	type_size_array.push_back(sizeof(FACTION_AUCTION_CONFIG));
	type_size_array.push_back(sizeof(FACTION_MONEY_SHOP_CONFIG));
	type_size_array.push_back(sizeof(COMPOUND_MINE_ESSENCE));
	type_size_array.push_back(sizeof(COLORANT_DEFAULT_CONFIG));
	type_size_array.push_back(sizeof(0));	//DT_MAX
}

elementdataman::~elementdataman()
{
	for(unsigned int i=0; i<talk_proc_array.size(); i++)
	{
		delete talk_proc_array[i];
	}
}

void elementdataman::add_structure(unsigned int id, EQUIPMENT_ADDON & data)
{
	equipment_addon_array.push_back(data);
	unsigned int pos = equipment_addon_array.size()-1;
	add_id_index(ID_SPACE_ADDON, id, DT_EQUIPMENT_ADDON, pos, &(equipment_addon_array[0]));
}

void elementdataman::add_structure(unsigned int id, EQUIPMENT_MAJOR_TYPE & data)
{
	equipment_major_type_array.push_back(data);
	unsigned int pos = equipment_major_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_EQUIPMENT_MAJOR_TYPE, pos, &(equipment_major_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, EQUIPMENT_SUB_TYPE & data)
{
	equipment_sub_type_array.push_back(data);
	unsigned int pos = equipment_sub_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_EQUIPMENT_SUB_TYPE, pos, &(equipment_sub_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, EQUIPMENT_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	equipment_essence_array.push_back(data);
	unsigned int pos = equipment_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_EQUIPMENT_ESSENCE, pos, &(equipment_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, REFINE_TICKET_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	refine_ticket_essence_array.push_back(data);
	unsigned int pos = refine_ticket_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_REFINE_TICKET_ESSENCE, pos, &(refine_ticket_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, OFFLINE_TRUSTEE_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	offline_trustee_essence_array.push_back(data);
	unsigned int pos = offline_trustee_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_OFFLINE_TRUSTEE_ESSENCE, pos, &(offline_trustee_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, LOTTERY_ESSENCE & data)
{
	data.pile_num_max = 1;
	if( data.dice_count == 0 ) data.dice_count = 1;

	data.proc_type &= ~(1<<7);

	lottery_essence_array.push_back(data);
	unsigned int pos = lottery_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_LOTTERY_ESSENCE, pos, &(lottery_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, LOTTERY2_ESSENCE & data)
{
//	data.pile_num_max = 1;
	if( data.dice_count != 1 ) data.dice_count = 1;

	data.proc_type &= ~(1<<7);

	lottery2_essence_array.push_back(data);
	unsigned int pos = lottery2_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_LOTTERY2_ESSENCE, pos, &(lottery2_essence_array[0]));
}

//Added 2011-07-01.
void elementdataman::add_structure(unsigned int id, LOTTERY3_ESSENCE & data)
{
//	data.pile_num_max = 1;
	if ( data.dice_count != 1 )
	{
		data.dice_count = 1;
	}

	data.proc_type &= ~(1<<7);

	lottery3_essence_array.push_back(data);
	unsigned int pos = lottery3_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_LOTTERY3_ESSENCE, pos, &(lottery3_essence_array[0]));
}

//Added 2011-03-14.
void elementdataman::add_structure(unsigned int id,  GEM_CONFIG & data)
{
	//首先将模板data插入到对应的数组gem_config_array中
	gem_config_array.push_back(data);
	unsigned int pos = gem_config_array.size() - 1;

	//然后添加索引，记录该config模板data的id所对应的pos
	add_id_index(ID_SPACE_CONFIG, id, DT_GEM_CONFIG, pos, &(gem_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,  GEM_ESSENCE & data)
{
	//首先处理一些基本的数据逻辑
	if( data.pile_num_max == 0 )
	{
		data.pile_num_max = 1;
	}
	data.proc_type &= ~(1<<7);

	//然后将模板data插入到相应的数组gem_essence_array中
	gem_essence_array.push_back(data);
	unsigned int pos = gem_essence_array.size() - 1;

	//最后添加索引，记录该essence模板data的id所对应的pos
	add_id_index(ID_SPACE_ESSENCE, id, DT_GEM_ESSENCE, pos, &(gem_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,  GEM_SEAL_ESSENCE & data)
{
	//首先处理一些基本的数据逻辑
	if( data.pile_num_max == 0 )
	{
		data.pile_num_max = 1;
	}
	data.proc_type &= ~(1<<7);

	//然后将模板data插入到相应的数组gem_seal_essence_array中
	gem_seal_essence_array.push_back(data);
	unsigned int pos = gem_seal_essence_array.size() - 1;

	//最后添加索引，将记录该essence模板data的id所对应的pos
	add_id_index(ID_SPACE_ESSENCE, id, DT_GEM_SEAL_ESSENCE, pos, &(gem_seal_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,  GEM_DUST_ESSENCE & data)
{
	//首先处理一些基本的数据逻辑
	if( data.pile_num_max == 0 )
	{
		data.pile_num_max = 1;
	}
	data.proc_type &= ~(1<<7);

	//然后将模板data插入到相应的数组gem_dust_essence_array中
	gem_dust_essence_array.push_back(data);
	unsigned int pos = gem_dust_essence_array.size() - 1;

	//最后添加索引，记录该essence模板data的id所对应的pos
	add_id_index(ID_SPACE_ESSENCE, id, DT_GEM_DUST_ESSENCE, pos, &(gem_dust_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,  GEM_EXTRACT_CONFIG & data)
{
	//首先，将模板data插入到相应的数组gem_extract_config_array中
	gem_extract_config_array.push_back(data);
	unsigned int pos = gem_extract_config_array.size() - 1;

	//最后添加索引，记录该config模板data的id所对应的pos
	add_id_index(ID_SPACE_CONFIG, id, DT_GEM_EXTRACT_CONFIG, pos, &(gem_extract_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,  GENERAL_ARTICLE_ESSENCE & data)
{
	//首先，将模板data插入到相应的数组general_article_essence_array中
	general_article_essence_array.push_back(data);
	unsigned int pos = general_article_essence_array.size() - 1;

	//最后添加索引，记录该essence模板data的id所对应的pos
	add_id_index(ID_SPACE_ESSENCE, id, DT_GENERAL_ARTICLE_ESSENCE, pos, &(general_article_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SMALL_ONLINE_GIFT_BAG_ESSENCE & data)
{
	//首先处理一些基本的数据逻辑
	if( data.pile_num_max == 0 )
	{
		data.pile_num_max = 1;
	}
	data.proc_type &= ~(1<<7);

	//其次，将模板data插入到相应的数组small_online_gift_bag_essence_array中
	small_online_gift_bag_essence_array.push_back(data);
	unsigned int pos = small_online_gift_bag_essence_array.size() - 1;

	//最后添加索引，记录该essence模板data的id所对应的位置pos
	add_id_index(ID_SPACE_ESSENCE, id, DT_SMALL_ONLINE_GIFT_BAG_ESSENCE, pos, &(small_online_gift_bag_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SCROLL_UNLOCK_ESSENCE & data)
{
	//首先处理一些基本的数据逻辑
	if( data.pile_num_max == 0 )
	{
		data.pile_num_max = 1;
	}
	data.proc_type &= ~(1<<7);

	//其次，将模板data插入到相应的数组scroll_unlock_essence_array中
	scroll_unlock_essence_array.push_back(data);
	unsigned int pos = scroll_unlock_essence_array.size() - 1;

	//最后添加索引，记录该essence模板data的id所对应的位置pos
	add_id_index(ID_SPACE_ESSENCE, id, DT_SCROLL_UNLOCK_ESSENCE, pos, &(scroll_unlock_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SCROLL_DIG_COUNT_INC_ESSENCE & data)
{
	//首先处理一些基本的数据逻辑
	if( data.pile_num_max == 0 )
	{
		data.pile_num_max = 1;
	}
	data.proc_type &= ~(1<<7);

	//其次，将模板data插入到相应的数组scroll_dig_count_inc_essence_array中
	scroll_dig_count_inc_essence_array.push_back(data);
	unsigned int pos = scroll_dig_count_inc_essence_array.size() - 1;

	//最后添加索引，记录该essence模板data的id所对应的位置pos
	add_id_index(ID_SPACE_ESSENCE, id, DT_SCROLL_DIG_COUNT_INC_ESSENCE, pos, &(scroll_dig_count_inc_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, ONLINE_GIFT_BAG_CONFIG & data)
{
	//首先，将模板data插入到相应的数组online_gift_bag_config_array中
	online_gift_bag_config_array.push_back(data);
	unsigned int pos = online_gift_bag_config_array.size() - 1;

	//最后添加索引，记录该config模板data的id所对应的位置pos
	add_id_index(ID_SPACE_CONFIG, id, DT_ONLINE_GIFT_BAG_CONFIG, pos, &(online_gift_bag_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, SCROLL_REGION_CONFIG & data)
{
	//首先，将模板data插入到相应的数组scroll_region_config_array中
	scroll_region_config_array.push_back(data);
	unsigned int pos = scroll_region_config_array.size() - 1;

	//最后添加索引，记录该config模板data的id所对应的位置pos
	add_id_index(ID_SPACE_CONFIG, id, DT_SCROLL_REGION_CONFIG, pos, &(scroll_region_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, CAMRECORDER_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	camrecorder_essence_array.push_back(data);
	unsigned int pos = camrecorder_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_CAMRECORDER_ESSENCE, pos, &(camrecorder_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, MEDICINE_MAJOR_TYPE & data)
{
	medicine_major_type_array.push_back(data);

	unsigned int pos = medicine_major_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MEDICINE_MAJOR_TYPE, pos, &(medicine_major_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, MEDICINE_SUB_TYPE & data)
{
	medicine_sub_type_array.push_back(data);

	unsigned int pos = medicine_sub_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MEDICINE_SUB_TYPE, pos, &(medicine_sub_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, MEDICINE_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	medicine_essence_array.push_back(data);

	unsigned int pos = medicine_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MEDICINE_ESSENCE, pos, &(medicine_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, MATERIAL_MAJOR_TYPE & data)
{
	material_major_type_array.push_back(data);

	unsigned int pos = material_major_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MATERIAL_MAJOR_TYPE, pos, &(material_major_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, MATERIAL_SUB_TYPE & data)
{
	material_sub_type_array.push_back(data);

	unsigned int pos = material_sub_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MATERIAL_SUB_TYPE, pos, &(material_sub_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, MATERIAL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	material_essence_array.push_back(data);

	unsigned int pos = material_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MATERIAL_ESSENCE, pos, &(material_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, DESTROYING_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	destroying_essence_array.push_back(data);
	
	unsigned int pos = destroying_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_DESTROYING_ESSENCE, pos, &(destroying_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SKILLTOME_SUB_TYPE & data)
{
	skilltome_sub_type_array.push_back(data);
	
	unsigned int pos = skilltome_sub_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SKILLTOME_SUB_TYPE, pos, &(skilltome_sub_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, SKILLTOME_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	skilltome_essence_array.push_back(data);
	
	unsigned int pos = skilltome_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SKILLTOME_ESSENCE, pos, &(skilltome_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TRANSMITROLL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	transmitroll_essence_array.push_back(data);

	unsigned int pos = transmitroll_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TRANSMITROLL_ESSENCE, pos, &(transmitroll_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, LUCKYROLL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	luckyroll_essence_array.push_back(data);

	unsigned int pos = luckyroll_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_LUCKYROLL_ESSENCE, pos, &(luckyroll_essence_array[0]));
}

	
void elementdataman::add_structure(unsigned int id, TOWNSCROLL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	townscroll_essence_array.push_back(data);

	unsigned int pos = townscroll_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TOWNSCROLL_ESSENCE, pos, &(townscroll_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, REVIVESCROLL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	revivescroll_essence_array.push_back(data);

	unsigned int pos = revivescroll_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_REVIVESCROLL_ESSENCE, pos, &(revivescroll_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SIEGE_ARTILLERY_SCROLL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	siege_artillery_scroll_essence_array.push_back(data);

	unsigned int pos = siege_artillery_scroll_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SIEGE_ARTILLERY_SCROLL_ESSENCE, pos, &(siege_artillery_scroll_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TASKMATTER_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	taskmatter_essence_array.push_back(data);

	unsigned int pos = taskmatter_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TASKMATTER_ESSENCE, pos, &(taskmatter_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, DROPTABLE_TYPE & data)
{
	droptable_type_array.push_back(data);

	unsigned int pos = droptable_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_DROPTABLE_TYPE, pos, &(droptable_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, DROPTABLE_ESSENCE & data)
{
	int i=0;
	float r[64];
	for(i=0; i<5; i++)	r[i] = data.num_to_drop[i].probability;
	NormalizeRandom(r, 5);
	for(i=0; i<5; i++)	data.num_to_drop[i].probability = r[i];

	//eliminate_zero_item((unsigned char*) data.drops, sizeof(unsigned int)+sizeof(float), 64);

	for(i=0; i<64; i++)	r[i] = data.drops[i].probability;
	NormalizeRandom(r, 64);
	for(i=0; i<64; i++)	data.drops[i].probability = r[i];

	droptable_essence_array.push_back(data);

	unsigned int pos = droptable_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_DROPTABLE_ESSENCE, pos, &(droptable_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, MONSTER_TYPE & data)
{
	monster_type_array.push_back(data);

	unsigned int pos = monster_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MONSTER_TYPE, pos, &(monster_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, MONSTER_ESSENCE & data)
{
	eliminate_zero_item((unsigned char*) data.skills, sizeof(unsigned int)+sizeof(int), 32);

	if( data.drop_times < 0 ) data.drop_times = 0;
	if( data.drop_times > 100 ) data.drop_times = 100;

	monster_essence_array.push_back(data);

	unsigned int pos = monster_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MONSTER_ESSENCE, pos, &(monster_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_TALK_SERVICE & data)
{
	npc_talk_service_array.push_back(data);

	unsigned int pos = npc_talk_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_TALK_SERVICE, pos, &(npc_talk_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_SELL_SERVICE & data)
{
	npc_sell_service_array.push_back(data);
	unsigned int pos = npc_sell_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_SELL_SERVICE, pos, &(npc_sell_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_BUY_SERVICE & data)
{
	npc_buy_service_array.push_back(data);

	unsigned int pos = npc_buy_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_BUY_SERVICE, pos, &(npc_buy_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_TASK_IN_SERVICE & data)
{
	eliminate_zero_item((unsigned char*) data.id_tasks, sizeof(int), 32);

	npc_task_in_service_array.push_back(data);

	unsigned int pos = npc_task_in_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_TASK_IN_SERVICE, pos, &(npc_task_in_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_TASK_OUT_SERVICE & data)
{
	eliminate_zero_item((unsigned char*) data.id_tasks, sizeof(int), 32);
	
	npc_task_out_service_array.push_back(data);

	unsigned int pos = npc_task_out_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_TASK_OUT_SERVICE, pos, &(npc_task_out_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_TASK_MATTER_SERVICE & data)
{
	eliminate_zero_item((unsigned char*) data.tasks, 4*(sizeof(unsigned int)+sizeof(int))+sizeof(int), 16);
	npc_task_matter_service_array.push_back(data);

	unsigned int pos = npc_task_matter_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_TASK_MATTER_SERVICE, pos, &(npc_task_matter_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_HEAL_SERVICE & data)
{
	npc_heal_service_array.push_back(data);

	unsigned int pos = npc_heal_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_HEAL_SERVICE, pos, &(npc_heal_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_TRANSMIT_SERVICE & data)
{
	eliminate_zero_item((unsigned char*) data.targets, sizeof(namechar) * 16 + sizeof(int) * 3 + sizeof(float) * 3, 16);
	npc_transmit_service_array.push_back(data);

	unsigned int pos = npc_transmit_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_TRANSMIT_SERVICE, pos, &(npc_transmit_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_PROXY_SERVICE & data)
{
	npc_proxy_service_array.push_back(data);

	unsigned int pos = npc_proxy_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_PROXY_SERVICE, pos, &(npc_proxy_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_STORAGE_SERVICE & data)
{
	npc_storage_service_array.push_back(data);

	unsigned int pos = npc_storage_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_STORAGE_SERVICE, pos, &(npc_storage_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_EQUIPBIND_SERVICE & data)
{
	npc_equipbind_service_array.push_back(data);
	
	unsigned int pos = npc_equipbind_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_EQUIPBIND_SERVICE, pos, &(npc_equipbind_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_EQUIPDESTROY_SERVICE & data)
{
	npc_equipdestroy_service_array.push_back(data);
	
	unsigned int pos = npc_equipdestroy_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_EQUIPDESTROY_SERVICE, pos, &(npc_equipdestroy_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_WAR_BUY_ARCHER_SERVICE & data)
{
	npc_war_buy_archer_service_array.push_back(data);
	
	unsigned int pos = npc_war_buy_archer_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_WAR_BUY_ARCHER_SERVICE, pos, &(npc_war_buy_archer_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_EQUIPUNDESTROY_SERVICE & data)
{
	npc_equipundestroy_service_array.push_back(data);
	
	unsigned int pos = npc_equipundestroy_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_EQUIPUNDESTROY_SERVICE, pos, &(npc_equipundestroy_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_CONSIGN_SERVICE & data)
{
	npc_consign_service_array.push_back(data);
	
	unsigned int pos = npc_consign_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_CONSIGN_SERVICE, pos, &(npc_consign_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_REPUTATION_SHOP_SERVICE & data)
{
	npc_reputation_shop_service_array.push_back(data);
	
	unsigned int pos = npc_reputation_shop_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_REPUTATION_SHOP_SERVICE, pos, &(npc_reputation_shop_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_UI_TRANSFER_SERVICE & data)
{
	npc_ui_transfer_service_array.push_back(data);
	
	unsigned int pos = npc_ui_transfer_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_UI_TRANSFER_SERVICE, pos, &(npc_ui_transfer_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_TYPE & data)
{
	npc_type_array.push_back(data);

	unsigned int pos = npc_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_TYPE, pos, &(npc_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_ESSENCE & data)
{
	npc_essence_array.push_back(data);

	unsigned int pos = npc_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_ESSENCE, pos, &(npc_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, EQUIP_SOUL_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	equip_soul_essence_array.push_back(data);
	unsigned int pos = equip_soul_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_EQUIP_SOUL_ESSENCE, pos, &(equip_soul_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, EQUIP_SOUL_MELD_SERVICE & data)
{
	//if(data.pile_num_max == 0) data.pile_num_max = 1;

	//data.proc_type &= ~(1<<7);

	equip_soul_meld_service_array.push_back(data);
	unsigned int pos = equip_soul_meld_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_EQUIP_SOUL_MELD_SERVICE, pos, &(equip_soul_meld_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, SPECIAL_NAME_ITEM_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	special_name_item_essence_array.push_back(data);
	unsigned int pos = special_name_item_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SPECIAL_NAME_ITEM_ESSENCE, pos, &(special_name_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, GIFT_BAG_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	gift_bag_essence_array.push_back(data);
	unsigned int pos = gift_bag_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_GIFT_BAG_ESSENCE, pos, &(gift_bag_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, VIP_CARD_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	vip_card_essence_array.push_back(data);
	unsigned int pos = vip_card_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_VIP_CARD_ESSENCE, pos, &(vip_card_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, COLLISION_RAID_AWARD_CONFIG & data)
{
	collision_raid_award_config_array.push_back(data);
	
	unsigned int pos = collision_raid_award_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_COLLISION_RAID_AWARD_CONFIG, pos, &(collision_raid_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BUFF_AREA_CONFIG & data)
{
	buff_area_config_array.push_back(data);
	
	unsigned int pos = buff_area_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BUFF_AREA_CONFIG, pos, &(buff_area_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, LIVENESS_CONFIG & data)
{
	liveness_config_array.push_back(data);
	
	unsigned int pos = liveness_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_LIVENESS_CONFIG, pos, &(liveness_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, CHALLENGE_2012_CONFIG & data)
{
	challenge_2012_config_array.push_back(data);
	
	unsigned int pos = challenge_2012_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_CHALLENGE_2012_CONFIG, pos, &(challenge_2012_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, SALE_PROMOTION_ITEM_ESSENCE & data)
{
	sale_promotion_item_essence_array.push_back(data);
	
	unsigned int pos = sale_promotion_item_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SALE_PROMOTION_ITEM_ESSENCE, pos, &(sale_promotion_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, GIFT_BAG_LOTTERY_DELIVER_ESSENCE & data)
{
	gift_bag_lottery_deliver_essence_array.push_back(data);
	
	unsigned int pos = gift_bag_lottery_deliver_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_GIFT_BAG_LOTTERY_DELIVER_ESSENCE, pos, &(gift_bag_lottery_deliver_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, LOTTERY_TANGYUAN_ITEM_ESSENCE & data)
{
	lottery_tangyuan_item_essence_array.push_back(data);
	
	unsigned int pos = lottery_tangyuan_item_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_LOTTERY_TANGYUAN_ITEM_ESSENCE, pos, &(lottery_tangyuan_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, GIFT_PACK_ITEM_ESSENCE & data)
{
	gift_pack_item_essence_array.push_back(data);
	
	unsigned int pos = gift_pack_item_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_GIFT_PACK_ITEM_ESSENCE, pos, &(gift_pack_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PROP_ADD_MATERIAL_ITEM_ESSENCE & data)
{
	prop_add_material_item_essence_array.push_back(data);
	
	unsigned int pos = prop_add_material_item_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PROP_ADD_MATERIAL_ITEM_ESSENCE, pos, &(prop_add_material_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PROP_ADD_ITEM_ESSENCE & data)
{
	prop_add_item_essence_array.push_back(data);
	
	unsigned int pos = prop_add_item_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PROP_ADD_ITEM_ESSENCE, pos, &(prop_add_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TASK_SPECIAL_AWARD_CONFIG & data)
{
	task_special_award_config_array.push_back(data);
	
	unsigned int pos = task_special_award_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TASK_SPECIAL_AWARD_CONFIG, pos, &(task_special_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, PROP_ADD_CONFIG & data)
{
	prop_add_config_array.push_back(data);
	
	unsigned int pos = prop_add_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PROP_ADD_CONFIG, pos, &(prop_add_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, KING_WAR_CONFIG & data)
{
	king_war_config_array.push_back(data);
	
	unsigned int pos = king_war_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_KING_WAR_CONFIG, pos, &(king_war_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, JINFASHEN_TO_MONEY_CONFIG & data)
{
	jinfashen_to_money_config_array.push_back(data);
	
	unsigned int pos = jinfashen_to_money_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_JINFASHEN_TO_MONEY_CONFIG, pos, &(jinfashen_to_money_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BATTLE_201304_CONFIG & data)
{
	battle_201304_config_array.push_back(data);
	
	unsigned int pos = battle_201304_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BATTLE_201304_CONFIG, pos, &(battle_201304_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, RUNE2013_ITEM_ESSENCE & data)
{
	rune2013_item_essence_array.push_back(data);
	
	unsigned int pos = rune2013_item_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_RUNE2013_ITEM_ESSENCE, pos, &(rune2013_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, COLORANT_ITEM_ESSENCE & data)
{
	colorant_item_essence_array.push_back(data);

	unsigned int pos = colorant_item_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_COLORANT_ITEM_ESSENCE, pos, &(colorant_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, INTERACTION_OBJECT_ESSENCE& data)
{
	interaction_object_essence_array.push_back(data);
	unsigned int pos = interaction_object_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_INTERACTION_OBJECT_ESSENCE, pos, &(interaction_object_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, BUILDING_ESSENCE & data)
{
	building_essence_array.push_back(data);
	unsigned int pos = building_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BUILDING_ESSENCE, pos, &(building_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, REGION_BLOCK_ESSENCE & data)
{
	region_block_essence_array.push_back(data);
	unsigned int pos = region_block_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_REGION_BLOCK_ESSENCE, pos, &(region_block_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, RUNE2013_CONFIG & data)
{
	rune2013_config_array.push_back(data);
	
	unsigned int pos = rune2013_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RUNE2013_CONFIG, pos, &(rune2013_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BASHE_AWARD_CONFIG & data)
{
	bashe_award_config_array.push_back(data);
	
	unsigned int pos = bashe_award_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BASHE_AWARD_CONFIG, pos, &(bashe_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, ARENA_SEASON_TIME_CONFIG & data)
{
	arena_season_time_config_array.push_back(data);
	unsigned int pos = arena_season_time_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_ARENA_SEASON_TIME_CONFIG, pos, &(arena_season_time_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, PERSONAL_LEVEL_AWARD_CONFIG & data)
{
	personal_level_award_config_array.push_back(data);
	unsigned int pos = personal_level_award_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PERSONAL_LEVEL_AWARD_CONFIG, pos, &(personal_level_award_config_array[0]));		
}

void elementdataman::add_structure(unsigned int id, TEAM_SEASON_AWARD_CONFIG & data)
{
	team_season_award_config_array.push_back(data);
	unsigned int pos = team_season_award_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TEAM_SEASON_AWARD_CONFIG, pos, &(team_season_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, WEEK_CURRNCY_AWARD_CONFIG & data)
{
	week_currency_award_config_array.push_back(data);
	unsigned int pos = week_currency_award_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_WEEK_CURRNCY_AWARD_CONFIG, pos, &(week_currency_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, INTERACTION_OBJECT_HOOK_CONFIG & data)
{
	interaction_object_hook_config_array.push_back(data);
	unsigned int pos = interaction_object_hook_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_INTERACTION_OBJECT_HOOK_CONFIG, pos, &(interaction_object_hook_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, COLORANT_CONFIG & data)
{
	colorant_config_array.push_back(data);
	unsigned int pos = colorant_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_COLORANT_CONFIG, pos, &(colorant_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FACTION_TRANSFER_CONFIG & data)
{
	faction_transfer_config_array.push_back(data);
	unsigned int pos = faction_transfer_config_array.size() -1; 
	add_id_index(ID_SPACE_CONFIG, id, DT_FACTION_TRANSFER_CONFIG, pos, &(faction_transfer_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BUILDING_REGION_CONFIG & data)
{
	building_region_config_array.push_back(data);
	unsigned int pos = building_region_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BUILDING_REGION_CONFIG, pos, &(building_region_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, PHASE_CONFIG & data)
{
	phase_config_array.push_back(data);
	unsigned int pos = phase_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PHASE_CONFIG, pos, &(phase_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, TRANSCRIPT_STRATEGY_CONFIG & data)
{
	transcript_strategy_config_array.push_back(data);
	unsigned int pos = transcript_strategy_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TRANSCRIPT_STRATEGY_CONFIG, pos, &(transcript_strategy_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FACTION_SHOP_CONFIG & data)
{
	faction_shop_config_array.push_back(data);
	unsigned int pos = faction_shop_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FACTION_SHOP_CONFIG, pos, &(faction_shop_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FACTION_AUCTION_CONFIG & data)
{
	faction_auction_config_array.push_back(data);
	unsigned int pos = faction_auction_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FACTION_AUCTION_CONFIG, pos, &(faction_auction_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FACTION_MONEY_SHOP_CONFIG & data)
{
	faction_money_shop_config_array.push_back(data);
	unsigned int pos = faction_money_shop_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FACTION_MONEY_SHOP_CONFIG, pos, &(faction_money_shop_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, COLORANT_DEFAULT_CONFIG & data)
{
	colorant_default_config_array.push_back(data);
	unsigned int pos = colorant_default_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_COLORANT_DEFAULT_CONFIG, pos, &(colorant_default_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, COMPOUND_MINE_ESSENCE & data)
{
	compound_mine_essence_array.push_back(data);
	unsigned int pos = compound_mine_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_COMPOUND_MINE_ESSENCE, pos, &(compound_mine_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, CROSS6V6_ITEM_EXCHANGE_CONFIG & data)
{
	cross6v6_item_exchange_config_array.push_back(data);
	unsigned int pos = cross6v6_item_exchange_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CROSS6V6_ITEM_EXCHANGE_CONFIG, pos, &(cross6v6_item_exchange_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, CHANGE_SHAPE_CARD_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	change_shape_card_essence_array.push_back(data);
	unsigned int pos = change_shape_card_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_CHANGE_SHAPE_CARD_ESSENCE, pos, &(change_shape_card_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, CHANGE_SHAPE_STONE_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	change_shape_stone_essence_array.push_back(data);
	unsigned int pos = change_shape_stone_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_CHANGE_SHAPE_STONE_ESSENCE, pos, &(change_shape_stone_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, MERCENARY_CREDENTIAL_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	mercenary_credential_essence_array.push_back(data);
	unsigned int pos = mercenary_credential_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MERCENARY_CREDENTIAL_ESSENCE, pos, &(mercenary_credential_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TELEPORTATION_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	teleportation_essence_array.push_back(data);
	unsigned int pos = teleportation_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TELEPORTATION_ESSENCE, pos, &(teleportation_essence_array[0]));
}


void elementdataman::add_structure(unsigned int id, TELEPORTATION_STONE_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	teleportation_stone_essence_array.push_back(data);
	unsigned int pos = teleportation_stone_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TELEPORTATION_STONE_ESSENCE, pos, &(teleportation_stone_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, UPGRADE_EQUIP_STONE_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	upgrade_equip_stone_essence_array.push_back(data);
	unsigned int pos = upgrade_equip_stone_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_UPGRADE_EQUIP_STONE_ESSENCE, pos, &(upgrade_equip_stone_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, talk_proc * data)
{
	talk_proc_array.push_back(data);

	unsigned int pos = talk_proc_array.size()-1;
	add_id_index(ID_SPACE_TALK, id, DT_TALK_PROC, pos, &(talk_proc_array[0]));
}

void elementdataman::add_structure(unsigned int id,  RECIPE_MAJOR_TYPE & data)
{
	recipe_major_type_array.push_back(data);

	unsigned int pos = recipe_major_type_array.size()-1;
	add_id_index(ID_SPACE_RECIPE, id, DT_RECIPE_MAJOR_TYPE, pos, &(recipe_major_type_array[0]));
}

void elementdataman::add_structure(unsigned int id,  RECIPE_SUB_TYPE & data)
{
	recipe_sub_type_array.push_back(data);

	unsigned int pos = recipe_sub_type_array.size()-1;
	add_id_index(ID_SPACE_RECIPE, id, DT_RECIPE_SUB_TYPE, pos, &(recipe_sub_type_array[0]));
}

void elementdataman::add_structure(unsigned int id,  RECIPE_ESSENCE & data)
{
	recipe_essence_array.push_back(data);

	unsigned int pos = recipe_essence_array.size()-1;
	add_id_index(ID_SPACE_RECIPE, id, DT_RECIPE_ESSENCE, pos, &(recipe_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, ENEMY_FACTION_CONFIG & data)
{
	enemy_faction_config_array.push_back(data);

	unsigned int pos = enemy_faction_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_ENEMY_FACTION_CONFIG, pos, &(enemy_faction_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,  CHARACTER_CLASS_CONFIG & data)
{
	character_class_config_array.push_back(data);

	unsigned int pos = character_class_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CHARACTER_CLASS_CONFIG, pos, &(character_class_config_array[0]));

}

void elementdataman::add_structure(unsigned int id,  PARAM_ADJUST_CONFIG & data)
{
	eliminate_zero_item((unsigned char*) data.level_diff_adjust, sizeof(int)+sizeof(float)*5, 16);
	eliminate_zero_item((unsigned char*) data.level_diff_produce, sizeof(int)+sizeof(float), 9);
	param_adjust_config_array.push_back(data);

	unsigned int pos = param_adjust_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PARAM_ADJUST_CONFIG, pos, &(param_adjust_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,  PIE_LOVE_CONFIG & data)
{
	pie_love_config_array.push_back(data);

	unsigned int pos = pie_love_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PIE_LOVE_CONFIG, pos, &(pie_love_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,  TASKDICE_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	int i;
	float r[16];
	for(i=0; i<8; i++)	r[i] = data.task_lists[i].probability;
	NormalizeRandom(r, 8);
	for(i=0; i<8; i++)	data.task_lists[i].probability = r[i];

	taskdice_essence_array.push_back(data);

	unsigned int pos = taskdice_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TASKDICE_ESSENCE, pos, &(taskdice_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,  TASKNORMALMATTER_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	tasknormalmatter_essence_array.push_back(data);

	unsigned int pos = tasknormalmatter_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TASKNORMALMATTER_ESSENCE, pos, &(tasknormalmatter_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 PLAYER_LEVELEXP_CONFIG & data)
{
	player_levelexp_config_array.push_back(data);

	unsigned int pos = player_levelexp_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PLAYER_LEVELEXP_CONFIG, pos, &(player_levelexp_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 PLAYER_TALENT_CONFIG & data)
{
	player_talent_config_array.push_back(data);

	unsigned int pos = player_talent_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PLAYER_TALENT_CONFIG, pos, &(player_talent_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 PLAYER_SKILL_POINT_CONFIG & data)
{
	player_skill_point_config_array.push_back(data);

	unsigned int pos = player_skill_point_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PLAYER_SKILL_POINT_CONFIG, pos, &(player_skill_point_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 MINE_TYPE & data)
{
	mine_type_array.push_back(data);

	unsigned int pos = mine_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MINE_TYPE, pos, &(mine_type_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 MINE_ESSENCE & data)
{
	int		i;

	float r[16];
	for(i=0; i<16; i++)	r[i] = data.materials[i].probability;
	NormalizeRandom(r, 16);
	for(i=0; i<16; i++)	data.materials[i].probability = r[i];
	
	r[0] = data.probability1;
	r[1] = data.probability2;
	NormalizeRandom(r, 2);
	data.probability1 = r[0];
	data.probability2 = r[1];

	eliminate_zero_item((unsigned char*) data.npcgen, sizeof(unsigned int) + sizeof(int) + sizeof(float), 4);
	eliminate_zero_item((unsigned char*) data.aggros, sizeof(unsigned int) + sizeof(float) + sizeof(int), 1);

	mine_essence_array.push_back(data);

	unsigned int pos = mine_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MINE_ESSENCE, pos, &(mine_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, WAR_ROLE_CONFIG & data)
{
	war_role_config_array.push_back(data);

	unsigned int pos = war_role_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_WAR_ROLE_CONFIG, pos, &(war_role_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, GM_GENERATOR_TYPE & data)
{
	gm_generator_type_array.push_back(data);

	unsigned int pos = gm_generator_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_GM_GENERATOR_TYPE, pos, &(gm_generator_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, GM_GENERATOR_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	gm_generator_essence_array.push_back(data);

	unsigned int pos = gm_generator_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_GM_GENERATOR_ESSENCE, pos, &(gm_generator_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, FIREWORKS_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	fireworks_essence_array.push_back(data);

	unsigned int pos = fireworks_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_FIREWORKS_ESSENCE, pos, &(fireworks_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TEXT_FIREWORKS_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	text_fireworks_essence_array.push_back(data);

	unsigned int pos = text_fireworks_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TEXT_FIREWORKS_ESSENCE, pos, &(text_fireworks_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TALISMAN_MAINPART_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	talisman_mainpart_essence_array.push_back(data);

	unsigned int pos = talisman_mainpart_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TALISMAN_MAINPART_ESSENCE, pos, &(talisman_mainpart_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TALISMAN_EXPFOOD_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	talisman_expfood_essence_array.push_back(data);

	unsigned int pos = talisman_expfood_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TALISMAN_EXPFOOD_ESSENCE, pos, &(talisman_expfood_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TALISMAN_ENERGYFOOD_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	talisman_energyfood_essence_array.push_back(data);

	unsigned int pos = talisman_energyfood_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TALISMAN_ENERGYFOOD_ESSENCE, pos, &(talisman_energyfood_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SPEAKER_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	speaker_essence_array.push_back(data);

	unsigned int pos = speaker_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SPEAKER_ESSENCE, pos, &(speaker_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, BOOK_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	book_essence_array.push_back(data);

	unsigned int pos = book_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BOOK_ESSENCE, pos, &(book_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TALISMAN_MERGEKATALYST_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	talisman_mergekatalyst_essence_array.push_back(data);

	unsigned int pos = talisman_mergekatalyst_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TALISMAN_MERGEKATALYST_ESSENCE, pos, &(talisman_mergekatalyst_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_WAR_TOWERBUILD_SERVICE & data)
{
	eliminate_zero_item((unsigned char*)data.build_info, sizeof(int)+sizeof(int)*4, 4);

	npc_war_towerbuild_service_array.push_back(data);

	unsigned int pos = npc_war_towerbuild_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_WAR_TOWERBUILD_SERVICE, pos, &(npc_war_towerbuild_service_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 PLAYER_SECONDLEVEL_CONFIG & data)
{
	player_secondlevel_config_array.push_back(data);

	unsigned int pos = player_secondlevel_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PLAYER_SECONDLEVEL_CONFIG, pos, &(player_secondlevel_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 TITLE_PROP_CONFIG & data)
{
	title_prop_config_array.push_back(data);

	eliminate_zero_item((unsigned char*)data.id_addons, sizeof(int), 3);

	unsigned int pos = title_prop_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TITLE_PROP_CONFIG, pos, &(title_prop_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 SPECIAL_ID_CONFIG & data)
{
	special_id_config_array.push_back(data);

	unsigned int pos = special_id_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_SPECIAL_ID_CONFIG, pos, &(special_id_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 RENASCENCE_SKILL_CONFIG & data)
{
	renasence_skill_config_array.push_back(data);

	unsigned int pos = renasence_skill_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RENASCENCE_SKILL_CONFIG, pos, &(renasence_skill_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 RENASCENCE_PROP_CONFIG & data)
{
	renasence_prop_config_array.push_back(data);

	unsigned int pos = renasence_prop_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RENASCENCE_PROP_CONFIG, pos, &(renasence_prop_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 RECYCLE_ITEM_CONFIG & data)
{
	recycle_item_config_array.push_back(data);

	unsigned int pos = recycle_item_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RECYCLE_ITEM_CONFIG, pos, &(recycle_item_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 SCORE_TO_RANK_CONFIG & data)
{
	score_to_rank_config_array.push_back(data);

	unsigned int pos = score_to_rank_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_SCORE_TO_RANK_CONFIG, pos, &(score_to_rank_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 BATTLE_DROP_CONFIG & data)
{
	battle_drop_config_array.push_back(data);

	unsigned int pos = battle_drop_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BATTLE_DROP_CONFIG, pos, &(battle_drop_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 BATTLE_DEPRIVE_CONFIG & data)
{
	battle_deprive_config_array.push_back(data);

	unsigned int pos = battle_deprive_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BATTLE_DEPRIVE_CONFIG, pos, &(battle_deprive_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 BATTLE_SCORE_CONFIG & data)
{
	battle_score_config_array.push_back(data);

	unsigned int pos = battle_score_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BATTLE_SCORE_CONFIG, pos, &(battle_score_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 INSTANCING_BATTLE_CONFIG & data)
{
	instancing_battle_config_array.push_back(data);

	unsigned int pos = instancing_battle_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_INSTANCING_BATTLE_CONFIG, pos, &(instancing_battle_config_array[0]));
}

///////////////////////////////////////////////////////////////////////////////
//添加副本模板数据到模板列表中，并建立相应的索引。
//Added 2011-07-13.
///////////////////////////////////////////////////////////////////////////////
void elementdataman::add_structure(unsigned int id,	 TRANSCRIPTION_CONFIG & data)
{
	//将副本模板数据存入模板列表中
	transcription_config_array.push_back(data);

	//创建相应的索引信息
	unsigned int pos = transcription_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TRANSCRIPTION_CONFIG, pos, &(transcription_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 TOWER_TRANSCRIPTION_CONFIG & data)
{
	//将副本模板数据存入模板列表中
	tower_transcription_config_array.push_back(data);

	//创建相应的索引信息
	unsigned int pos = tower_transcription_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TOWER_TRANSCRIPTION_CONFIG, pos, &(tower_transcription_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 TOWER_TRANSCRIPTION_PROPERTY_CONFIG & data)
{
	//将副本模板数据存入模板列表中
	tower_transcription_property_config_array.push_back(data);

	//创建相应的索引信息
	unsigned int pos = tower_transcription_property_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG, pos, &(tower_transcription_property_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 LITTLE_PET_UPGRADE_CONFIG & data)
{
	//将诛小仙模板数据存入模板列表中
	little_pet_upgrade_config_array.push_back(data);

	//创建相应的索引信息
	unsigned int pos = little_pet_upgrade_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_LITTLE_PET_UPGRADE_CONFIG, pos, &(little_pet_upgrade_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 COLLISION_RAID_TRANSFIGURE_CONFIG & data)
{
	//将碰撞副本变身配置模板数据存入模板列表中
	collision_raid_transfigure_config_array.push_back(data);

	//创建相应的索引信息
	unsigned int pos = collision_raid_transfigure_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_COLLISION_RAID_TRANSFIGURE_CONFIG, pos, &(collision_raid_transfigure_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 UI_TRANSFER_CONFIG & data)
{
	ui_transfer_config_array.push_back(data);
	
	//创建相应的索引信息
	unsigned int pos = ui_transfer_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_UI_TRANSFER_CONFIG, pos, &(ui_transfer_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 DROP_INTERNAL_CONFIG & data)
{
	//将诛小仙模板数据存入模板列表中
	drop_interval_config_array.push_back(data);
	
	//创建相应的索引信息
	unsigned int pos = drop_interval_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_DROP_INTERNAL_CONFIG, pos, &(drop_interval_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 PK2012_GUESS_CONFIG & data)
{
	//将诛小仙模板数据存入模板列表中
	pk2012_guess_config_array.push_back(data);
	
	//创建相应的索引信息
	unsigned int pos = pk2012_guess_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PK2012_GUESS_CONFIG, pos, &(pk2012_guess_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 RUNE_DATA_CONFIG & data)
{
	rune_data_config_array.push_back(data);
	
	unsigned int pos = rune_data_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RUNE_DATA_CONFIG, pos, &(rune_data_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 RUNE_COMB_PROPERTY & data)
{
	rune_comb_property_array.push_back(data);
	
	unsigned int pos = rune_comb_property_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RUNE_COMB_PROPERTY, pos, &(rune_comb_property_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 RUNE_EQUIP_ESSENCE & data)
{
	rune_equip_array.push_back(data);
	
	unsigned int pos = rune_equip_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_RUNE_EQUIP_ESSENCE, pos, &(rune_equip_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 RUNE_SLOT_ESSENCE & data)
{
	rune_slot_array.push_back(data);
	
	unsigned int pos = rune_slot_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_RUNE_SLOT_ESSENCE, pos, &(rune_slot_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 BOOTH_FIGURE_ITEM_ESSENCE & data)
{
	booth_figure_item_array.push_back(data);
	
	unsigned int pos = booth_figure_item_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BOOTH_FIGURE_ITEM_ESSENCE, pos, &(booth_figure_item_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 FLAG_BUFF_ITEM_ESSENCE & data)
{
	flag_buff_item_array.push_back(data);
	
	unsigned int pos = flag_buff_item_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_FLAG_BUFF_ITEM_ESSENCE, pos, &(flag_buff_item_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 XINGZUO_ITEM_ESSENCE & data)
{
	xingzuo_item_essence_array.push_back(data);
	
	unsigned int pos = xingzuo_item_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_XINGZUO_ITEM_ESSENCE, pos, &(xingzuo_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 XINGZUO_ENERGY_ITEM_ESSENCE & data)
{
	xingzuo_energy_item_essence_array.push_back(data);
	
	unsigned int pos = xingzuo_energy_item_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_XINGZUO_ENERGY_ITEM_ESSENCE, pos, &(xingzuo_energy_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 CASH_MEDIC_MERGE_ITEM_ESSENCE & data)
{
	cash_medic_merge_item_essence_array.push_back(data);
	
	unsigned int pos = cash_medic_merge_item_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_CASH_MEDIC_MERGE_ITEM_ESSENCE, pos, &(cash_medic_merge_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 CHANGE_SHAPE_PROP_CONFIG & data)
{
	change_shape_prop_config_array.push_back(data);

	unsigned int pos = change_shape_prop_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CHANGE_SHAPE_PROP_CONFIG, pos, &(change_shape_prop_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 ORIGINAL_SHAPE_CONFIG & data)
{
	original_shape_config_array.push_back(data);

	unsigned int pos = original_shape_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_ORIGINAL_SHAPE_CONFIG, pos, &(original_shape_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 LIFE_SKILL_CONFIG & data)
{
	life_skill_config_array.push_back(data);

	unsigned int pos = life_skill_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_LIFE_SKILL_CONFIG, pos, &(life_skill_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 ARENA_DROPTABLE_CONFIG & data)
{
	arena_droptable_config_array.push_back(data);

	unsigned int pos = arena_droptable_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_ARENA_DROPTABLE_CONFIG, pos, &(arena_droptable_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 COMBINE_SKILL_EDIT_CONFIG & data)
{
	combine_skill_edit_config_array.push_back(data);

	unsigned int pos = combine_skill_edit_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_COMBINE_SKILL_EDIT_CONFIG, pos, &(combine_skill_edit_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 UPGRADE_EQUIP_CONFIG & data)
{
	upgrade_equip_config_array.push_back(data);

	unsigned int pos = upgrade_equip_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_UPGRADE_EQUIP_CONFIG, pos, &(upgrade_equip_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 UPGRADE_EQUIP_CONFIG_1 & data)
{
	upgrade_equip_config_1_array.push_back(data);
	
	unsigned int pos = upgrade_equip_config_1_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_UPGRADE_EQUIP_CONFIG, pos, &(upgrade_equip_config_1_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 DEITY_CHARACTER_CONFIG & data)
{
	deity_character_config_array.push_back(data);

	unsigned int pos = deity_character_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_DEITY_CHARACTER_CONFIG, pos, &(deity_character_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 ITEM_TRADE_CONFIG & data)
{
	item_trade_config_array.push_back(data);

	unsigned int pos = item_trade_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_ITEM_TRADE_CONFIG, pos, &(item_trade_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_RESETPROP_SERVICE & data)
{
	eliminate_zero_item((unsigned char*)data.prop_entry, sizeof(int)+sizeof(int)*4, 15);

	npc_resetprop_service_array.push_back(data);

	unsigned int pos = npc_resetprop_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_RESETPROP_SERVICE, pos, &(npc_resetprop_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, ESTONE_ESSENCE & data)
{
	if(data.pile_num_max <= 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	estone_essence_array.push_back(data);

	unsigned int pos = estone_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_ESTONE_ESSENCE, pos, &(estone_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PSTONE_ESSENCE & data)
{
	if(data.pile_num_max <= 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pstone_essence_array.push_back(data);

	unsigned int pos = pstone_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PSTONE_ESSENCE, pos, &(pstone_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SSTONE_ESSENCE & data)
{
	if(data.pile_num_max <= 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	sstone_essence_array.push_back(data);

	unsigned int pos = sstone_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SSTONE_ESSENCE, pos, &(sstone_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, POTENTIAL_TOME_ESSENCE & data)
{
	if(data.pile_num_max <= 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	potential_tome_essence_array.push_back(data);

	unsigned int pos = potential_tome_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_POTENTIAL_TOME_ESSENCE, pos, &(potential_tome_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,  RECIPEROLL_MAJOR_TYPE & data)
{
	reciperoll_major_type_array.push_back(data);

	unsigned int pos = reciperoll_major_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_RECIPEROLL_MAJOR_TYPE, pos, &(reciperoll_major_type_array[0]));
}

void elementdataman::add_structure(unsigned int id,  RECIPEROLL_SUB_TYPE & data)
{
	reciperoll_sub_type_array.push_back(data);

	unsigned int pos = reciperoll_sub_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_RECIPEROLL_SUB_TYPE, pos, &(reciperoll_sub_type_array[0]));
}

void elementdataman::add_structure(unsigned int id,  RECIPEROLL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	reciperoll_essence_array.push_back(data);
	
	unsigned int pos = reciperoll_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_RECIPEROLL_ESSENCE, pos, &(reciperoll_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,  SUITE_ESSENCE & data)
{
	eliminate_zero_item((unsigned char*) data.equipments, sizeof(unsigned int), 14);
	suite_essence_array.push_back(data);
	
	unsigned int pos = suite_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SUITE_ESSENCE, pos, &(suite_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, DOUBLE_EXP_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	double_exp_essence_array.push_back(data);

	unsigned int pos = double_exp_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_DOUBLE_EXP_ESSENCE, pos, &(double_exp_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SKILLMATTER_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	skillmatter_essence_array.push_back(data);
	
	unsigned int pos = skillmatter_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SKILLMATTER_ESSENCE, pos, &(skillmatter_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, VEHICLE_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	vehicle_essence_array.push_back(data);
	
	unsigned int pos = vehicle_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_VEHICLE_ESSENCE, pos, &(vehicle_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, FLY_ENERGYFOOD_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	fly_energyfood_essence_array.push_back(data);

	unsigned int pos = fly_energyfood_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_FLY_ENERGYFOOD_ESSENCE, pos, &(fly_energyfood_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, AIRCRAFT_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	aircraft_essence_array.push_back(data);
	
	unsigned int pos = aircraft_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_AIRCRAFT_ESSENCE, pos, &(aircraft_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, COUPLE_JUMPTO_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	couple_jumpto_essence_array.push_back(data);
	
	unsigned int pos = couple_jumpto_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_COUPLE_JUMPTO_ESSENCE, pos, &(couple_jumpto_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PET_BEDGE_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pet_bedge_essence_array.push_back(data);

	unsigned int pos = pet_bedge_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PET_BEDGE_ESSENCE, pos, &(pet_bedge_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PET_FOOD_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pet_food_essence_array.push_back(data);

	unsigned int pos = pet_food_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PET_FOOD_ESSENCE, pos, &(pet_food_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PET_AUTO_FOOD_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pet_auto_food_essence_array.push_back(data);
	unsigned int pos = pet_auto_food_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PET_AUTO_FOOD_ESSENCE, pos, &(pet_auto_food_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PET_REFINE_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pet_refine_essence_array.push_back(data);
	unsigned int pos = pet_refine_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PET_REFINE_ESSENCE, pos, &(pet_refine_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PET_ASSIST_REFINE_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pet_assist_refine_essence_array.push_back(data);
	unsigned int pos = pet_assist_refine_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PET_ASSIST_REFINE_ESSENCE, pos, &(pet_assist_refine_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PET_SKILL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pet_skill_essence_array.push_back(data);

	unsigned int pos = pet_skill_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PET_SKILL_ESSENCE, pos, &(pet_skill_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PET_ARMOR_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pet_armor_essence_array.push_back(data);

	unsigned int pos = pet_armor_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PET_ARMOR_ESSENCE, pos, &(pet_armor_essence_array[0]));
}

void elementdataman::add_id_index_essence(unsigned int id, DATA_TYPE type, unsigned int pos, void * start)
{
	LOCATION p;
	p.type = type;
	p.pos = pos;
	essence_id_index_map[id] = p;
	start_ptr_array[type] = start;
}


void elementdataman::add_id_index_addon(unsigned int id, DATA_TYPE type, unsigned int pos, void * start)
{
	LOCATION p;
	p.type = type;
	p.pos = pos;
	addon_id_index_map[id] = p;
	start_ptr_array[type] = start;
}

void elementdataman::add_id_index_talk(unsigned int id, DATA_TYPE type, unsigned int pos, void * start)
{
	LOCATION p;
	p.type = type;
	p.pos = pos;
	talk_id_index_map[id] = p;
	start_ptr_array[type] = start;
}

void elementdataman::add_id_index_recipe(unsigned int id, DATA_TYPE type, unsigned int pos, void * start)
{
	LOCATION p;
	p.type = type;
	p.pos = pos;
	recipe_id_index_map[id] = p;
	start_ptr_array[type] = start;
}

void elementdataman::add_id_index_config(unsigned int id, DATA_TYPE type, unsigned int pos, void * start)
{
	LOCATION p;
	p.type = type;
	p.pos = pos;
	config_id_index_map[id] = p;
	start_ptr_array[type] = start;
}

void elementdataman::add_id_index(ID_SPACE idspace, unsigned int id, DATA_TYPE type, unsigned int pos, void * start)
{
	switch(idspace)
	{
	case ID_SPACE_ESSENCE:
		add_id_index_essence(id, type, pos, start);	
		break;
	case ID_SPACE_ADDON:
		add_id_index_addon(id, type, pos, start);
		break;
	case ID_SPACE_TALK:
		add_id_index_talk(id, type, pos, start);
		break;
	case ID_SPACE_RECIPE:
		add_id_index_recipe(id, type, pos, start);
	case ID_SPACE_CONFIG:
		add_id_index_config(id, type, pos, start);			
	default:
		break;
	}
}

unsigned int elementdataman::get_data_id(ID_SPACE idspace, unsigned int index, DATA_TYPE& datatype)
{
	unsigned int id = 0;
	unsigned int i;
	IDToLOCATIONMap::iterator itr;
	switch(idspace)
	{
	case ID_SPACE_ESSENCE:
		itr = essence_id_index_map.begin();
 		for(i=0; i<index; i++)	++itr;
		id = itr->first;
		datatype = (itr->second).type;
		break;
		
	case ID_SPACE_ADDON:
		itr = addon_id_index_map.begin();
		for(i=0; i<index; i++)	++itr;
		id = itr->first;
		datatype = (itr->second).type;
		break;
		
	case ID_SPACE_TALK:
		itr = talk_id_index_map.begin();
		for(i=0; i<index; i++)	++itr;
		id = itr->first;
		datatype = (itr->second).type;
		break;

	case ID_SPACE_RECIPE:
		itr = recipe_id_index_map.begin();
		for(i=0; i<index; i++)	++itr;
		id = itr->first;
		datatype = (itr->second).type;
		break;

	case ID_SPACE_CONFIG:
		itr = config_id_index_map.begin();
		for(i=0; i<index; i++)	++itr;
		id = itr->first;
		datatype = (itr->second).type;
		break;

	default:
		id = 0;
		datatype = DT_INVALID;
		break;
	}
	return id;
}

unsigned int elementdataman::get_first_data_id(ID_SPACE idspace, DATA_TYPE& datatype)
{
	unsigned int id = 0;
	switch(idspace)
	{
	case ID_SPACE_ESSENCE:
		esssence_id_index_itr = essence_id_index_map.begin();
		if(esssence_id_index_itr == essence_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = esssence_id_index_itr->first;
			datatype = (esssence_id_index_itr->second).type;
		}
		break;
		
	case ID_SPACE_ADDON:
		addon_id_index_itr = addon_id_index_map.begin();
		if(addon_id_index_itr == addon_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = addon_id_index_itr->first;
			datatype = (addon_id_index_itr->second).type;
		}
		break;
		
	case ID_SPACE_TALK:
		talk_id_index_itr = talk_id_index_map.begin();
		if(talk_id_index_itr == talk_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = talk_id_index_itr->first;
			datatype = (talk_id_index_itr->second).type;
		}
		break;

	case ID_SPACE_RECIPE:
		recipe_id_index_itr = recipe_id_index_map.begin();
		if(recipe_id_index_itr == recipe_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = recipe_id_index_itr->first;
			datatype = (recipe_id_index_itr->second).type;
		}
		break;

	case ID_SPACE_CONFIG:
		config_id_index_itr = config_id_index_map.begin();
		if(config_id_index_itr == config_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = config_id_index_itr->first;
			datatype = (config_id_index_itr->second).type;
		}
		break;

	default:
		id = 0;
		datatype = DT_INVALID;
		break;
	}
	return id;
}

unsigned int elementdataman::get_next_data_id(ID_SPACE idspace, DATA_TYPE& datatype)
{
	unsigned int id;
	switch(idspace)
	{
	case ID_SPACE_ESSENCE:
		++esssence_id_index_itr;
		if(esssence_id_index_itr == essence_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = esssence_id_index_itr->first;
			datatype = (esssence_id_index_itr->second).type;
		}
		break;
		
	case ID_SPACE_ADDON:
		++addon_id_index_itr;
		if(addon_id_index_itr == addon_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = addon_id_index_itr->first;
			datatype = (addon_id_index_itr->second).type;
		}
		break;
		
	case ID_SPACE_TALK:
		++talk_id_index_itr;
		if(talk_id_index_itr == talk_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = talk_id_index_itr->first;
			datatype = (talk_id_index_itr->second).type;
		}
		break;

	case ID_SPACE_RECIPE:
		++recipe_id_index_itr;
		if(recipe_id_index_itr == recipe_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = recipe_id_index_itr->first;
			datatype = (recipe_id_index_itr->second).type;
		}
		break;

	case ID_SPACE_CONFIG:
		++config_id_index_itr;
		if(config_id_index_itr == config_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = config_id_index_itr->first;
			datatype = (config_id_index_itr->second).type;
		}
		break;

	default:
		datatype = DT_INVALID;
		break;
	}
	return id;	
}

unsigned int elementdataman::get_data_num(ID_SPACE idspace)
{
	switch(idspace)
	{
	case ID_SPACE_ESSENCE:
		return essence_id_index_map.size();

	case ID_SPACE_ADDON:
		return addon_id_index_map.size();

	case ID_SPACE_TALK:
		return talk_id_index_map.size();

	case ID_SPACE_RECIPE:
		return recipe_id_index_map.size();

	case ID_SPACE_CONFIG:
		return config_id_index_map.size();

	default:
		return 0;
	}
	return 0;
}

DATA_TYPE elementdataman::get_data_type(unsigned int id, ID_SPACE idspace)
{
	IDToLOCATIONMap::iterator itr;
	switch(idspace)
	{
	case ID_SPACE_ESSENCE:
		itr = essence_id_index_map.find(id);
		if(itr != essence_id_index_map.end())
		{
			return (itr->second).type;
		}
		else
			return DT_INVALID;			
		break;

	case ID_SPACE_ADDON:
		itr = addon_id_index_map.find(id);
		if(itr != addon_id_index_map.end())
		{
			return (itr->second).type;
		}
		else
			return DT_INVALID;
		break;		

	case ID_SPACE_TALK:
		itr = talk_id_index_map.find(id);
		if(itr != talk_id_index_map.end())
		{
			return (itr->second).type;
		}
		else
			return DT_INVALID;
		break;

	case ID_SPACE_RECIPE:
		itr = recipe_id_index_map.find(id);
		if(itr != recipe_id_index_map.end())
		{
			return (itr->second).type;
		}
		else
			return DT_INVALID;
		break;

	case ID_SPACE_CONFIG:
		itr = config_id_index_map.find(id);
		if(itr != config_id_index_map.end())
		{
			return (itr->second).type;
		}
		else
			return DT_INVALID;
		break;

	default:
		return DT_INVALID;
		break;
	}
}

const void * elementdataman::get_data_ptr(unsigned int id, ID_SPACE idspace, DATA_TYPE& datatype)
{
	datatype = DT_INVALID;
	int pos;
	IDToLOCATIONMap::iterator itr;
	switch(idspace)
	{
	case ID_SPACE_ESSENCE:
		itr = essence_id_index_map.find(id);
		if(itr != essence_id_index_map.end())
		{
			datatype = (itr->second).type;
			pos = (itr->second).pos;
		}
		else
			datatype = DT_INVALID;			
		break;

	case ID_SPACE_ADDON:
		itr = addon_id_index_map.find(id);
		if(itr != addon_id_index_map.end())
		{
			datatype = (itr->second).type;
			pos = (itr->second).pos;
		}
		else
			datatype = DT_INVALID;
		break;
		
	case ID_SPACE_TALK:
		itr = talk_id_index_map.find(id);
		if(itr != talk_id_index_map.end())
		{
			datatype = (itr->second).type;
			pos = (itr->second).pos;
		}
		else
			datatype = DT_INVALID;
		break;

	case ID_SPACE_RECIPE:
		itr = recipe_id_index_map.find(id);
		if(itr != recipe_id_index_map.end())
		{
			datatype = (itr->second).type;
			pos = (itr->second).pos;
		}
		else
			datatype = DT_INVALID;
		break;

	case ID_SPACE_CONFIG:
		itr = config_id_index_map.find(id);
		if(itr != config_id_index_map.end())
		{
			datatype = (itr->second).type;
			pos = (itr->second).pos;
		}
		else
			datatype = DT_INVALID;
		break;

	default:
		datatype = DT_INVALID;
		break;
	}

	if(datatype != DT_INVALID)
	{
		char * start = (char *) start_ptr_array[datatype];
		if(datatype == DT_TALK_PROC)
			return (const void*) *(int *)(start+pos*type_size_array[datatype]);
		else
			return start+pos*type_size_array[datatype];
	}
	else
		return NULL;
}

void elementdataman::setup_hash_map()
{
#define ADD_HASH_MAP(ids, typ, arr) \
p.type = typ;\
for(i=0; i<arr.size(); i++)\
{\
	start_ptr_array[typ] = &(arr[0]);\
	p.pos = i;\
	ids##_id_index_map[arr[i].id] = p;\
}

	unsigned int i;
	LOCATION p;

	ADD_HASH_MAP(addon, DT_EQUIPMENT_ADDON, equipment_addon_array)
	ADD_HASH_MAP(essence, DT_EQUIPMENT_MAJOR_TYPE, equipment_major_type_array)
	ADD_HASH_MAP(essence, DT_EQUIPMENT_SUB_TYPE, equipment_sub_type_array)
	ADD_HASH_MAP(essence, DT_EQUIPMENT_ESSENCE, equipment_essence_array)
	ADD_HASH_MAP(essence, DT_REFINE_TICKET_ESSENCE, refine_ticket_essence_array)
	ADD_HASH_MAP(essence, DT_MEDICINE_MAJOR_TYPE, medicine_major_type_array)
	ADD_HASH_MAP(essence, DT_MEDICINE_SUB_TYPE, medicine_sub_type_array)
	ADD_HASH_MAP(essence, DT_MEDICINE_ESSENCE, medicine_essence_array)
	ADD_HASH_MAP(essence, DT_MATERIAL_MAJOR_TYPE, material_major_type_array)
	ADD_HASH_MAP(essence, DT_MATERIAL_SUB_TYPE, material_sub_type_array)
	ADD_HASH_MAP(essence, DT_MATERIAL_ESSENCE, material_essence_array)
	ADD_HASH_MAP(essence, DT_SKILLTOME_SUB_TYPE, skilltome_sub_type_array)
	ADD_HASH_MAP(essence, DT_SKILLTOME_ESSENCE, skilltome_essence_array)
	ADD_HASH_MAP(essence, DT_TRANSMITROLL_ESSENCE, transmitroll_essence_array)
	ADD_HASH_MAP(essence, DT_LUCKYROLL_ESSENCE, luckyroll_essence_array)
	ADD_HASH_MAP(essence, DT_TOWNSCROLL_ESSENCE, townscroll_essence_array)
	ADD_HASH_MAP(essence, DT_REVIVESCROLL_ESSENCE, revivescroll_essence_array)
	ADD_HASH_MAP(essence, DT_TASKMATTER_ESSENCE, taskmatter_essence_array)
	ADD_HASH_MAP(essence, DT_DROPTABLE_TYPE, droptable_type_array)
	ADD_HASH_MAP(essence, DT_DROPTABLE_ESSENCE, droptable_essence_array)
	ADD_HASH_MAP(essence, DT_MONSTER_TYPE, monster_type_array)
	ADD_HASH_MAP(essence, DT_MONSTER_ESSENCE, monster_essence_array)
	ADD_HASH_MAP(essence, DT_NPC_TALK_SERVICE, npc_talk_service_array)
	ADD_HASH_MAP(essence, DT_NPC_SELL_SERVICE, npc_sell_service_array)
	ADD_HASH_MAP(essence, DT_NPC_BUY_SERVICE, npc_buy_service_array)
	ADD_HASH_MAP(essence, DT_NPC_TASK_IN_SERVICE, npc_task_in_service_array)
	ADD_HASH_MAP(essence, DT_NPC_TASK_OUT_SERVICE, npc_task_out_service_array)
	ADD_HASH_MAP(essence, DT_NPC_TASK_MATTER_SERVICE, npc_task_matter_service_array)
	ADD_HASH_MAP(essence, DT_NPC_HEAL_SERVICE, npc_heal_service_array)
	ADD_HASH_MAP(essence, DT_NPC_TRANSMIT_SERVICE, npc_transmit_service_array)
	ADD_HASH_MAP(essence, DT_NPC_PROXY_SERVICE, npc_proxy_service_array)
	ADD_HASH_MAP(essence, DT_NPC_STORAGE_SERVICE, npc_storage_service_array)
	ADD_HASH_MAP(essence, DT_NPC_TYPE, npc_type_array)
	ADD_HASH_MAP(essence, DT_NPC_ESSENCE, npc_essence_array)
	ADD_HASH_MAP(recipe, DT_RECIPE_MAJOR_TYPE, recipe_major_type_array)
	ADD_HASH_MAP(recipe, DT_RECIPE_SUB_TYPE, recipe_sub_type_array)
	ADD_HASH_MAP(recipe, DT_RECIPE_ESSENCE, recipe_essence_array)
	ADD_HASH_MAP(config, DT_ENEMY_FACTION_CONFIG, enemy_faction_config_array)
	ADD_HASH_MAP(config, DT_CHARACTER_CLASS_CONFIG, character_class_config_array)
	ADD_HASH_MAP(config, DT_PARAM_ADJUST_CONFIG, param_adjust_config_array)
	ADD_HASH_MAP(config, DT_PIE_LOVE_CONFIG, pie_love_config_array)
	ADD_HASH_MAP(essence, DT_TASKDICE_ESSENCE, taskdice_essence_array)
	ADD_HASH_MAP(essence, DT_TASKNORMALMATTER_ESSENCE, tasknormalmatter_essence_array);
	ADD_HASH_MAP(config, DT_PLAYER_LEVELEXP_CONFIG, player_levelexp_config_array)
	ADD_HASH_MAP(essence, DT_MINE_TYPE, mine_type_array)
	ADD_HASH_MAP(essence, DT_MINE_ESSENCE, mine_essence_array)
	ADD_HASH_MAP(essence, DT_GM_GENERATOR_TYPE, gm_generator_type_array);
	ADD_HASH_MAP(essence, DT_GM_GENERATOR_ESSENCE, gm_generator_essence_array);
	ADD_HASH_MAP(essence, DT_FIREWORKS_ESSENCE, fireworks_essence_array);
	ADD_HASH_MAP(essence, DT_NPC_WAR_TOWERBUILD_SERVICE, npc_war_towerbuild_service_array);
	ADD_HASH_MAP(config, DT_PLAYER_SECONDLEVEL_CONFIG, player_secondlevel_config_array);
	ADD_HASH_MAP(essence, DT_NPC_RESETPROP_SERVICE, npc_resetprop_service_array);
	ADD_HASH_MAP(essence, DT_ESTONE_ESSENCE, estone_essence_array);
	ADD_HASH_MAP(essence, DT_PSTONE_ESSENCE, pstone_essence_array);
	ADD_HASH_MAP(essence, DT_SSTONE_ESSENCE, sstone_essence_array);
	ADD_HASH_MAP(essence, DT_RECIPEROLL_MAJOR_TYPE, reciperoll_major_type_array);
	ADD_HASH_MAP(essence, DT_RECIPEROLL_SUB_TYPE, reciperoll_sub_type_array);
	ADD_HASH_MAP(essence, DT_RECIPEROLL_ESSENCE, reciperoll_essence_array);
	ADD_HASH_MAP(essence, DT_SUITE_ESSENCE, suite_essence_array);
	ADD_HASH_MAP(essence, DT_DOUBLE_EXP_ESSENCE, double_exp_essence_array);
	ADD_HASH_MAP(essence, DT_DESTROYING_ESSENCE, destroying_essence_array);
	ADD_HASH_MAP(essence, DT_NPC_EQUIPBIND_SERVICE, npc_equipbind_service_array);
	ADD_HASH_MAP(essence, DT_NPC_EQUIPDESTROY_SERVICE, npc_equipdestroy_service_array);
	ADD_HASH_MAP(essence, DT_NPC_EQUIPUNDESTROY_SERVICE, npc_equipundestroy_service_array);
	ADD_HASH_MAP(essence, DT_SKILLMATTER_ESSENCE, skillmatter_essence_array);
	ADD_HASH_MAP(essence, DT_VEHICLE_ESSENCE, vehicle_essence_array);
	ADD_HASH_MAP(essence, DT_COUPLE_JUMPTO_ESSENCE, couple_jumpto_essence_array);
	ADD_HASH_MAP(essence, DT_LOTTERY_ESSENCE, lottery_essence_array);
	ADD_HASH_MAP(essence, DT_CAMRECORDER_ESSENCE, camrecorder_essence_array);
	ADD_HASH_MAP(config, DT_TITLE_PROP_CONFIG, title_prop_config_array);
	ADD_HASH_MAP(config, DT_SPECIAL_ID_CONFIG, special_id_config_array);
	ADD_HASH_MAP(essence, DT_TEXT_FIREWORKS_ESSENCE, text_fireworks_essence_array);
	ADD_HASH_MAP(essence, DT_TALISMAN_MAINPART_ESSENCE, talisman_mainpart_essence_array);
	ADD_HASH_MAP(essence, DT_TALISMAN_EXPFOOD_ESSENCE, talisman_expfood_essence_array);
	ADD_HASH_MAP(essence, DT_TALISMAN_MERGEKATALYST_ESSENCE, talisman_mergekatalyst_essence_array);
	ADD_HASH_MAP(essence, DT_TALISMAN_ENERGYFOOD_ESSENCE, talisman_energyfood_essence_array);
	ADD_HASH_MAP(essence, DT_SPEAKER_ESSENCE, speaker_essence_array);
	ADD_HASH_MAP(config, DT_PLAYER_TALENT_CONFIG, player_talent_config_array); 
	ADD_HASH_MAP(essence, DT_POTENTIAL_TOME_ESSENCE, potential_tome_essence_array);
	ADD_HASH_MAP(essence, DT_WAR_ROLE_CONFIG, war_role_config_array);
	ADD_HASH_MAP(essence, DT_NPC_WAR_BUY_ARCHER_SERVICE, npc_war_buy_archer_service_array);
	ADD_HASH_MAP(essence, DT_SIEGE_ARTILLERY_SCROLL_ESSENCE, siege_artillery_scroll_essence_array);
	ADD_HASH_MAP(essence, DT_PET_BEDGE_ESSENCE, pet_bedge_essence_array);
	ADD_HASH_MAP(essence, DT_PET_FOOD_ESSENCE, pet_food_essence_array);
	ADD_HASH_MAP(essence, DT_PET_SKILL_ESSENCE, pet_skill_essence_array);
	ADD_HASH_MAP(essence, DT_PET_ARMOR_ESSENCE, pet_armor_essence_array);
	ADD_HASH_MAP(essence, DT_PET_AUTO_FOOD_ESSENCE, pet_auto_food_essence_array);
	ADD_HASH_MAP(essence, DT_PET_REFINE_ESSENCE, pet_refine_essence_array);
	ADD_HASH_MAP(essence, DT_PET_ASSIST_REFINE_ESSENCE, pet_assist_refine_essence_array);
	ADD_HASH_MAP(config, DT_RENASCENCE_SKILL_CONFIG, renasence_skill_config_array);
	ADD_HASH_MAP(config, DT_RENASCENCE_PROP_CONFIG, renasence_prop_config_array);
	ADD_HASH_MAP(essence, DT_AIRCRAFT_ESSENCE, aircraft_essence_array);
	ADD_HASH_MAP(essence, DT_FLY_ENERGYFOOD_ESSENCE, fly_energyfood_essence_array);
	ADD_HASH_MAP(essence, DT_ITEM_TRADE_CONFIG, item_trade_config_array);
	ADD_HASH_MAP(essence, DT_BOOK_ESSENCE, book_essence_array);
	ADD_HASH_MAP(config, DT_PLAYER_SKILL_POINT_CONFIG, player_skill_point_config_array); 
	ADD_HASH_MAP(essence, DT_OFFLINE_TRUSTEE_ESSENCE, offline_trustee_essence_array);
	ADD_HASH_MAP(essence, DT_EQUIP_SOUL_ESSENCE, equip_soul_essence_array);
	ADD_HASH_MAP(essence, DT_EQUIP_SOUL_MELD_SERVICE, equip_soul_meld_service_array);
	ADD_HASH_MAP(essence, DT_SPECIAL_NAME_ITEM_ESSENCE, special_name_item_essence_array);
	ADD_HASH_MAP(config, DT_RECYCLE_ITEM_CONFIG, recycle_item_config_array);
	ADD_HASH_MAP(config, DT_SCORE_TO_RANK_CONFIG, score_to_rank_config_array);
	ADD_HASH_MAP(config, DT_BATTLE_DROP_CONFIG, battle_drop_config_array);
	ADD_HASH_MAP(config, DT_BATTLE_DEPRIVE_CONFIG, battle_deprive_config_array);
	ADD_HASH_MAP(config, DT_BATTLE_SCORE_CONFIG, battle_score_config_array);
	ADD_HASH_MAP(essence, DT_GIFT_BAG_ESSENCE, gift_bag_essence_array);
	ADD_HASH_MAP(essence, DT_VIP_CARD_ESSENCE, vip_card_essence_array);
	ADD_HASH_MAP(config, DT_INSTANCING_BATTLE_CONFIG, instancing_battle_config_array);
	ADD_HASH_MAP(essence, DT_CHANGE_SHAPE_CARD_ESSENCE, change_shape_card_essence_array);
	ADD_HASH_MAP(essence, DT_CHANGE_SHAPE_STONE_ESSENCE, change_shape_stone_essence_array);
	ADD_HASH_MAP(config, DT_CHANGE_SHAPE_PROP_CONFIG, change_shape_prop_config_array);
	ADD_HASH_MAP(config, DT_ORIGINAL_SHAPE_CONFIG, original_shape_config_array);
	ADD_HASH_MAP(config, DT_LIFE_SKILL_CONFIG, life_skill_config_array);
	ADD_HASH_MAP(config, DT_ARENA_DROPTABLE_CONFIG, arena_droptable_config_array);
	ADD_HASH_MAP(essence, DT_MERCENARY_CREDENTIAL_ESSENCE, mercenary_credential_essence_array);
	ADD_HASH_MAP(essence, DT_TELEPORTATION_ESSENCE, teleportation_essence_array);
	ADD_HASH_MAP(essence, DT_TELEPORTATION_STONE_ESSENCE, teleportation_stone_essence_array);
	ADD_HASH_MAP(config, DT_COMBINE_SKILL_EDIT_CONFIG, combine_skill_edit_config_array);
	ADD_HASH_MAP(config, DT_UPGRADE_EQUIP_CONFIG, upgrade_equip_config_array);
	ADD_HASH_MAP(config, DT_UPGRADE_EQUIP_CONFIG_1, upgrade_equip_config_1_array);
	ADD_HASH_MAP(essence, DT_UPGRADE_EQUIP_STONE_ESSENCE, upgrade_equip_stone_essence_array);
	ADD_HASH_MAP(essence, DT_NPC_CONSIGN_SERVICE, npc_consign_service_array);
	ADD_HASH_MAP(config, DT_DEITY_CHARACTER_CONFIG, deity_character_config_array);
	ADD_HASH_MAP(essence, DT_LOTTERY2_ESSENCE, lottery2_essence_array);	
	ADD_HASH_MAP(config, DT_GEM_CONFIG, gem_config_array);//Added 2011-03-14.
	ADD_HASH_MAP(essence, DT_GEM_ESSENCE, gem_essence_array);
	ADD_HASH_MAP(essence, DT_GEM_SEAL_ESSENCE, gem_seal_essence_array);
	ADD_HASH_MAP(essence, DT_GEM_DUST_ESSENCE, gem_dust_essence_array);
	ADD_HASH_MAP(config, DT_GEM_EXTRACT_CONFIG, gem_extract_config_array);
	ADD_HASH_MAP(essence, DT_GENERAL_ARTICLE_ESSENCE, general_article_essence_array);
	ADD_HASH_MAP(essence, DT_LOTTERY3_ESSENCE, lottery3_essence_array); //Added 2011-07-01.
	ADD_HASH_MAP(config, DT_TRANSCRIPTION_CONFIG, transcription_config_array); //Added 2011-07-13.
	ADD_HASH_MAP(config, DT_ONLINE_GIFT_BAG_CONFIG, online_gift_bag_config_array); //Added 2011-12-06.
	ADD_HASH_MAP(essence, DT_SMALL_ONLINE_GIFT_BAG_ESSENCE, small_online_gift_bag_essence_array); //Added 2011-12-06.
	ADD_HASH_MAP(config, DT_SCROLL_REGION_CONFIG, scroll_region_config_array); //Added 2012-03-20.
	ADD_HASH_MAP(essence, DT_SCROLL_UNLOCK_ESSENCE, scroll_unlock_essence_array); //Added 2012-03-20.
	ADD_HASH_MAP(essence, DT_SCROLL_DIG_COUNT_INC_ESSENCE, scroll_dig_count_inc_essence_array); //Added 2012-03-22.
	ADD_HASH_MAP(config, DT_TOWER_TRANSCRIPTION_CONFIG, tower_transcription_config_array);		//Added 2012-03-31.
	ADD_HASH_MAP(config, DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG, tower_transcription_property_config_array);//Added 2012-03-31.
	ADD_HASH_MAP(config, DT_RUNE_DATA_CONFIG, rune_data_config_array);//Added 2012.04.17.
	ADD_HASH_MAP(config, DT_RUNE_COMB_PROPERTY, rune_comb_property_array);//Added 2012.04.17.
	ADD_HASH_MAP(essence, DT_RUNE_EQUIP_ESSENCE, rune_equip_array);//Added 2012.04.17.
	ADD_HASH_MAP(config, DT_LITTLE_PET_UPGRADE_CONFIG, little_pet_upgrade_config_array);	//Added 2012-04-19.
	ADD_HASH_MAP(essence, DT_RUNE_SLOT_ESSENCE, rune_slot_array);
	ADD_HASH_MAP(config, DT_DROP_INTERNAL_CONFIG, drop_interval_config_array);
	ADD_HASH_MAP(config, DT_PK2012_GUESS_CONFIG, pk2012_guess_config_array);
	ADD_HASH_MAP(config, DT_COLLISION_RAID_TRANSFIGURE_CONFIG, collision_raid_transfigure_config_array);	//Added 2012-08-02.
	ADD_HASH_MAP(essence, DT_BOOTH_FIGURE_ITEM_ESSENCE, booth_figure_item_array);
	ADD_HASH_MAP(essence, DT_FLAG_BUFF_ITEM_ESSENCE, flag_buff_item_array);
	ADD_HASH_MAP(essence, DT_NPC_REPUTATION_SHOP_SERVICE, npc_reputation_shop_service_array);
	ADD_HASH_MAP(essence, DT_NPC_UI_TRANSFER_SERVICE, npc_ui_transfer_service_array);
	ADD_HASH_MAP(config, DT_UI_TRANSFER_CONFIG, ui_transfer_config_array);
	ADD_HASH_MAP(essence, DT_XINGZUO_ITEM_ESSENCE, xingzuo_item_essence_array);
	ADD_HASH_MAP(essence, DT_XINGZUO_ENERGY_ITEM_ESSENCE, xingzuo_energy_item_essence_array);
	ADD_HASH_MAP(config, DT_COLLISION_RAID_AWARD_CONFIG, collision_raid_award_config_array);
	ADD_HASH_MAP(essence, DT_CASH_MEDIC_MERGE_ITEM_ESSENCE, cash_medic_merge_item_essence_array);
	ADD_HASH_MAP(config, DT_BUFF_AREA_CONFIG, buff_area_config_array);
	ADD_HASH_MAP(config, DT_LIVENESS_CONFIG, liveness_config_array);
	ADD_HASH_MAP(config, DT_CHALLENGE_2012_CONFIG, challenge_2012_config_array);
	ADD_HASH_MAP(essence, DT_SALE_PROMOTION_ITEM_ESSENCE, sale_promotion_item_essence_array);
	ADD_HASH_MAP(essence, DT_GIFT_BAG_LOTTERY_DELIVER_ESSENCE, gift_bag_lottery_deliver_essence_array);
	ADD_HASH_MAP(essence, DT_LOTTERY_TANGYUAN_ITEM_ESSENCE, lottery_tangyuan_item_essence_array);
	ADD_HASH_MAP(config, DT_TASK_SPECIAL_AWARD_CONFIG, task_special_award_config_array);
	ADD_HASH_MAP(essence, DT_GIFT_PACK_ITEM_ESSENCE, gift_pack_item_essence_array);
	ADD_HASH_MAP(config, DT_PROP_ADD_CONFIG, prop_add_config_array);
	ADD_HASH_MAP(essence, DT_PROP_ADD_MATERIAL_ITEM_ESSENCE, prop_add_material_item_essence_array);
	ADD_HASH_MAP(essence, DT_PROP_ADD_ITEM_ESSENCE, prop_add_item_essence_array);
	ADD_HASH_MAP(config, DT_KING_WAR_CONFIG, king_war_config_array);
	ADD_HASH_MAP(config, DT_JINFASHEN_TO_MONEY_CONFIG, jinfashen_to_money_config_array);
	ADD_HASH_MAP(config, DT_BATTLE_201304_CONFIG, battle_201304_config_array);
	ADD_HASH_MAP(essence, DT_RUNE2013_ITEM_ESSENCE, rune2013_item_essence_array);
	ADD_HASH_MAP(config, DT_RUNE2013_CONFIG, rune2013_config_array);
	ADD_HASH_MAP(config, DT_BASHE_AWARD_CONFIG, bashe_award_config_array);
	ADD_HASH_MAP(config, DT_ARENA_SEASON_TIME_CONFIG, arena_season_time_config_array);
	ADD_HASH_MAP(config, DT_PERSONAL_LEVEL_AWARD_CONFIG, personal_level_award_config_array);
	ADD_HASH_MAP(config, DT_TEAM_SEASON_AWARD_CONFIG, team_season_award_config_array);
	ADD_HASH_MAP(config, DT_WEEK_CURRNCY_AWARD_CONFIG, week_currency_award_config_array);
	ADD_HASH_MAP(essence, DT_COLORANT_ITEM_ESSENCE, colorant_item_essence_array);
	ADD_HASH_MAP(essence, DT_INTERACTION_OBJECT_ESSENCE,interaction_object_essence_array);
	ADD_HASH_MAP(config,  DT_INTERACTION_OBJECT_HOOK_CONFIG, interaction_object_hook_config_array);
	ADD_HASH_MAP(config,  DT_COLORANT_CONFIG, colorant_config_array);
	ADD_HASH_MAP(config,  DT_FACTION_TRANSFER_CONFIG, faction_transfer_config_array);
	ADD_HASH_MAP(config,  DT_BUILDING_REGION_CONFIG, building_region_config_array);
	ADD_HASH_MAP(essence, DT_BUILDING_ESSENCE, building_essence_array);
	ADD_HASH_MAP(essence, DT_REGION_BLOCK_ESSENCE, region_block_essence_array);
	ADD_HASH_MAP(config, DT_PHASE_CONFIG, phase_config_array);
	ADD_HASH_MAP(config, DT_CROSS6V6_ITEM_EXCHANGE_CONFIG, cross6v6_item_exchange_config_array);
	ADD_HASH_MAP(config, DT_TRANSCRIPT_STRATEGY_CONFIG, transcript_strategy_config_array);
	ADD_HASH_MAP(config, DT_FACTION_SHOP_CONFIG, faction_shop_config_array);
	ADD_HASH_MAP(config, DT_FACTION_AUCTION_CONFIG, faction_auction_config_array);
	ADD_HASH_MAP(config, DT_FACTION_MONEY_SHOP_CONFIG, faction_money_shop_config_array);
	ADD_HASH_MAP(essence, DT_COMPOUND_MINE_ESSENCE, compound_mine_essence_array);
	ADD_HASH_MAP(config, DT_COLORANT_DEFAULT_CONFIG, colorant_default_config_array);
#undef ADD_HASH_MAP
}

int elementdataman::save_data(const char * pathname)
{
	long md5pos[5];

	FILE * file;
	file = fopen(pathname, "wb+");
	if(file == NULL)		return -1;

	unsigned int version = ELEMENTDATA_VERSION;
	fwrite(&version, sizeof(unsigned int), 1, file);

	time_t t = time(NULL);
	fwrite(&t, sizeof(time_t), 1, file);

	equipment_addon_array.save(file);
	equipment_major_type_array.save(file);
	equipment_sub_type_array.save(file);
	equipment_essence_array.save(file);
	medicine_major_type_array.save(file);
	medicine_sub_type_array.save(file);
	medicine_essence_array.save(file);
	material_major_type_array.save(file);
	material_sub_type_array.save(file);
	material_essence_array.save(file);
	refine_ticket_essence_array.save(file);
	skilltome_sub_type_array.save(file);
	skilltome_essence_array.save(file);

	md5pos[0] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	transmitroll_essence_array.save(file);
	luckyroll_essence_array.save(file);
	townscroll_essence_array.save(file);
	revivescroll_essence_array.save(file);
	taskmatter_essence_array.save(file);
	droptable_type_array.save(file);
	droptable_essence_array.save(file);
	monster_type_array.save(file);
	monster_essence_array.save(file);
	offline_trustee_essence_array.save(file);

#ifdef _WINDOWS 
#ifdef _MD5_CHECK
	extern void MyGetComputerName(char * szBuffer, unsigned long * pLen);
	extern void MySleep();
	// Get the computer's name of the exporter
	unsigned int tag = 0x19e75edf;
	fwrite(&tag, sizeof(unsigned int), 1, file);
	char cname[256];
	unsigned long len = 256;
	MyGetComputerName(cname, &len);
	fwrite(&len, sizeof(unsigned int), 1, file);
	char c[4] = {(char)0x5f, (char)0x6d, (char)0xe8, (char)0xc9};
	for(unsigned int l=0; l<len; l++)
		cname[l] = cname[l] ^ c[l % 4];
	fwrite(cname, len, 1, file);
	MySleep();
	time_t ttt = time(NULL);
	fwrite(&ttt, sizeof(time_t), 1, file);
#endif
#endif

	npc_talk_service_array.save(file);
	npc_sell_service_array.save(file);
	npc_buy_service_array.save(file);
	npc_task_in_service_array.save(file);
	npc_task_out_service_array.save(file);
	npc_task_matter_service_array.save(file);
	npc_heal_service_array.save(file);
	npc_transmit_service_array.save(file);
	npc_proxy_service_array.save(file);
	npc_storage_service_array.save(file);
	npc_type_array.save(file);
	npc_essence_array.save(file);
	recipe_major_type_array.save(file);

	md5pos[1] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	recipe_sub_type_array.save(file);
	recipe_essence_array.save(file);
	enemy_faction_config_array.save(file);
	character_class_config_array.save(file);
	param_adjust_config_array.save(file);
	pie_love_config_array.save(file);
	taskdice_essence_array.save(file);
	tasknormalmatter_essence_array.save(file);
	mine_type_array.save(file);
	mine_essence_array.save(file);
	gm_generator_type_array.save(file);
	gm_generator_essence_array.save(file);
	fireworks_essence_array.save(file);
	player_levelexp_config_array.save(file);
	npc_war_towerbuild_service_array.save(file);
	player_secondlevel_config_array.save(file);
	npc_resetprop_service_array.save(file);
	estone_essence_array.save(file);
	pstone_essence_array.save(file);
	sstone_essence_array.save(file);

#ifdef _WINDOWS 
#ifdef _MD5_CHECK
	extern void MyGetHardInfo(unsigned char * buffer, unsigned long * len);
	unsigned int tag2 = 0xee35679f;
	fwrite(&tag2, sizeof(unsigned int), 1, file);
	unsigned char buffer[1024];
	unsigned long buffer_len = 1024;
	MyGetHardInfo(buffer, &buffer_len);
	fwrite(&buffer_len, sizeof(unsigned int), 1, file);
	fwrite(buffer, buffer_len, 1, file);
#endif
#endif

	reciperoll_major_type_array.save(file);
	reciperoll_sub_type_array.save(file);
	reciperoll_essence_array.save(file);
	suite_essence_array.save(file);
	double_exp_essence_array.save(file);
	destroying_essence_array.save(file);

	md5pos[2] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	npc_equipbind_service_array.save(file);
	npc_equipdestroy_service_array.save(file);
	npc_equipundestroy_service_array.save(file);
	skillmatter_essence_array.save(file);
	vehicle_essence_array.save(file);
	couple_jumpto_essence_array.save(file);
	lottery_essence_array.save(file);
	camrecorder_essence_array.save(file);
	title_prop_config_array.save(file);
	special_id_config_array.save(file);
	text_fireworks_essence_array.save(file);
	talisman_mainpart_essence_array.save(file);
	talisman_expfood_essence_array.save(file);
	talisman_mergekatalyst_essence_array.save(file);
	talisman_energyfood_essence_array.save(file);
	speaker_essence_array.save(file);
	player_talent_config_array.save(file);
	player_skill_point_config_array.save(file);
	potential_tome_essence_array.save(file);
	war_role_config_array.save(file);
	npc_war_buy_archer_service_array.save(file);
	siege_artillery_scroll_essence_array.save(file);
	pet_bedge_essence_array.save(file);
	pet_food_essence_array.save(file);
	pet_skill_essence_array.save(file);
	pet_armor_essence_array.save(file);
	pet_auto_food_essence_array.save(file);
	pet_refine_essence_array.save(file);
	pet_assist_refine_essence_array.save(file);
	renasence_skill_config_array.save(file);
	renasence_prop_config_array.save(file);
	aircraft_essence_array.save(file);
	fly_energyfood_essence_array.save(file);
	item_trade_config_array.save(file);
	book_essence_array.save(file);
	equip_soul_essence_array.save(file);
	equip_soul_meld_service_array.save(file);
	special_name_item_essence_array.save(file);
	recycle_item_config_array.save(file);
	score_to_rank_config_array.save(file);
	battle_drop_config_array.save(file);

	md5pos[3] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	battle_deprive_config_array.save(file);
	battle_score_config_array.save(file);
	gift_bag_essence_array.save(file);
	vip_card_essence_array.save(file);
	instancing_battle_config_array.save(file);
	change_shape_card_essence_array.save(file);
	change_shape_stone_essence_array.save(file);
	change_shape_prop_config_array.save(file);
	original_shape_config_array.save(file);
	life_skill_config_array.save(file);
	arena_droptable_config_array.save(file);
	mercenary_credential_essence_array.save(file);
	teleportation_essence_array.save(file);
	teleportation_stone_essence_array.save(file);
	combine_skill_edit_config_array.save(file);
	upgrade_equip_config_array.save(file);
	upgrade_equip_config_1_array.save(file);
	upgrade_equip_stone_essence_array.save(file);
	npc_consign_service_array.save(file);
	deity_character_config_array.save(file);
	lottery2_essence_array.save(file);
	gem_config_array.save(file);//Added 2011-03-14.
	gem_essence_array.save(file);
	gem_seal_essence_array.save(file);
	gem_dust_essence_array.save(file);
	gem_extract_config_array.save(file);
	general_article_essence_array.save(file);
	lottery3_essence_array.save(file); //Added 2011-07-01.
	transcription_config_array.save(file); //Added 2011-07-13.
	online_gift_bag_config_array.save(file); //Added 2011-12-06.
	small_online_gift_bag_essence_array.save(file); //Added 2011-12-06.
	scroll_region_config_array.save(file); //Added 2012-03-20.
	scroll_unlock_essence_array.save(file); //Added 2012-03-20.
	scroll_dig_count_inc_essence_array.save(file);	//Added 2012-03-22.
	tower_transcription_config_array.save(file);	//Added 2012-03-31.
	tower_transcription_property_config_array.save(file);//Added 2012-03-31.
	rune_data_config_array.save(file);//Added 2012.04.17
	rune_comb_property_array.save(file);//Added 2012.04.17
	rune_equip_array.save(file);//Added 2012.04.17
	little_pet_upgrade_config_array.save(file);	//Added 2012-04-19.
	rune_slot_array.save(file);//Added 2012.04.21
	drop_interval_config_array.save(file);
	pk2012_guess_config_array.save(file);
	collision_raid_transfigure_config_array.save(file);	//Added 2012-08-02.
	booth_figure_item_array.save(file);
	flag_buff_item_array.save(file);
	npc_reputation_shop_service_array.save(file);
	npc_ui_transfer_service_array.save(file);
	ui_transfer_config_array.save(file);
	xingzuo_item_essence_array.save(file);
	xingzuo_energy_item_essence_array.save(file);
	collision_raid_award_config_array.save(file);
	cash_medic_merge_item_essence_array.save(file);
	buff_area_config_array.save(file);
	liveness_config_array.save(file);
	challenge_2012_config_array.save(file);
	sale_promotion_item_essence_array.save(file);
	gift_bag_lottery_deliver_essence_array.save(file);
	lottery_tangyuan_item_essence_array.save(file);
	task_special_award_config_array.save(file);
	gift_pack_item_essence_array.save(file);	
	prop_add_config_array.save(file);
	prop_add_material_item_essence_array.save(file);
	prop_add_item_essence_array.save(file);
	king_war_config_array.save(file);
	jinfashen_to_money_config_array.save(file);
	battle_201304_config_array.save(file);
	rune2013_item_essence_array.save(file);
	rune2013_config_array.save(file);
	bashe_award_config_array.save(file);
	arena_season_time_config_array.save(file);
	personal_level_award_config_array.save(file);
	team_season_award_config_array.save(file);
	week_currency_award_config_array.save(file);
	colorant_item_essence_array.save(file);
	interaction_object_essence_array.save(file);
	interaction_object_hook_config_array.save(file);
	colorant_config_array.save(file);
	faction_transfer_config_array.save(file);
	building_region_config_array.save(file);
	building_essence_array.save(file);
	region_block_essence_array.save(file);
	phase_config_array.save(file);
	cross6v6_item_exchange_config_array.save(file);
	transcript_strategy_config_array.save(file);
	faction_shop_config_array.save(file);
	faction_auction_config_array.save(file);
	faction_money_shop_config_array.save(file);
	compound_mine_essence_array.save(file);
	colorant_default_config_array.save(file);

	size_t sz = talk_proc_array.size();

	fwrite(&sz, sizeof(size_t), 1, file);
	size_t i;
	for(i=0; i<sz; i++)
		talk_proc_array[i]->save(file);

	md5pos[4] = ftell(file);

#ifdef _WINDOWS
	long fstart = 0;
	char buf[8192];
	for(i = 0; i < 5; i++)
	{
		fseek(file, fstart, SEEK_SET);
		while(fstart < md5pos[i])
		{
			int readsize = 8192;
			if( md5pos[i] - fstart < readsize )
				readsize = md5pos[i] - fstart;
			fread(buf, 1, readsize, file);
			g_MD5Hash.Update(buf, readsize);
			fstart += readsize;
		}
		fstart += 8;
	}

	g_MD5Hash.Final();
	unsigned int size = 50;
	g_MD5Hash.GetString(buf, size);
	for(i = 0; i < 4; i++)
	{
		fseek(file, md5pos[i], SEEK_SET);
		fwrite(buf + i * 8, 1, 8, file);
	}
#endif

	fclose(file);
	return 0;
}

int elementdataman::load_data(const char * pathname)
{
	long md5pos[5];

	FILE * file;
	file = fopen(pathname, "rb");
	if(file == NULL)		return -1;

	unsigned int version = 0;
	fread(&version, sizeof(unsigned int), 1, file);
	if( version != ELEMENTDATA_VERSION )
		return -1;
					 
	time_t t;		// #define _USE_32BIT_TIME_T sizeof(time_t)=4
	fread(&t, sizeof(time_t), 1, file);

	if(equipment_addon_array.load(file) != 0) return -1;
	if(equipment_major_type_array.load(file) != 0) return -1;
	if(equipment_sub_type_array.load(file) != 0) return -1;
	if(equipment_essence_array.load(file) != 0) return -1;
	if(medicine_major_type_array.load(file) != 0) return -1;
	if(medicine_sub_type_array.load(file) != 0) return -1;
	if(medicine_essence_array.load(file) != 0) return -1;
	if(material_major_type_array.load(file) != 0) return -1;
	if(material_sub_type_array.load(file) != 0) return -1;
	if(material_essence_array.load(file) != 0) return -1;
	if(refine_ticket_essence_array.load(file) != 0) return -1;
	if(skilltome_sub_type_array.load(file) != 0) return -1;
	if(skilltome_essence_array.load(file) != 0) return -1;

	md5pos[0] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	if(transmitroll_essence_array.load(file) != 0) return -1;
	if(luckyroll_essence_array.load(file) != 0 ) return -1;
	if(townscroll_essence_array.load(file) != 0) return -1;
	if(revivescroll_essence_array.load(file) != 0) return -1;
	if(taskmatter_essence_array.load(file) != 0) return -1;
	if(droptable_type_array.load(file) != 0 ) return -1;
	if(droptable_essence_array.load(file) != 0 ) return -1;
	if(monster_type_array.load(file) != 0) return -1;
	if(monster_essence_array.load(file) != 0) return -1;
	if(offline_trustee_essence_array.load(file) != 0) return -1;

	// skip the computer's name of the exporter
	unsigned int tag;
	fread(&tag, sizeof(unsigned int), 1, file);
	char buffer[1024];
	unsigned int len;
	fread(&len, sizeof(unsigned int), 1, file);
	fread(buffer, len, 1, file);
	fread(&t, sizeof(int), 1, file);
	
	if(npc_talk_service_array.load(file) != 0) return -1;
	if(npc_sell_service_array.load(file) != 0) return -1;
	if(npc_buy_service_array.load(file) != 0) return -1;
	if(npc_task_in_service_array.load(file) != 0) return -1;
	if(npc_task_out_service_array.load(file) != 0) return -1;
	if(npc_task_matter_service_array.load(file) != 0) return -1;
	if(npc_heal_service_array.load(file) != 0) return -1;
	if(npc_transmit_service_array.load(file) != 0) return -1;
	if(npc_proxy_service_array.load(file) != 0) return -1;
	if(npc_storage_service_array.load(file) != 0) return -1;
	if(npc_type_array.load(file) != 0) return -1;
	if(npc_essence_array.load(file) != 0) return -1;
	if(recipe_major_type_array.load(file) != 0) return -1;

	md5pos[1] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	if(recipe_sub_type_array.load(file) != 0) return -1;
	if(recipe_essence_array.load(file) != 0) return -1;
	if(enemy_faction_config_array.load(file) != 0) return -1;
	if(character_class_config_array.load(file) != 0) return -1;
	if(param_adjust_config_array.load(file) != 0) return -1;
	if(pie_love_config_array.load(file) != 0) return -1;
	if(taskdice_essence_array.load(file) != 0) return -1;
	if(tasknormalmatter_essence_array.load(file) != 0) return -1;
	if(mine_type_array.load(file) != 0) return -1;
	if(mine_essence_array.load(file) != 0) return -1;
	if(gm_generator_type_array.load(file) != 0) return -1;
	if(gm_generator_essence_array.load(file) != 0) return -1;
	if(fireworks_essence_array.load(file) != 0) return -1;
	if(player_levelexp_config_array.load(file) != 0) return -1;
	if(npc_war_towerbuild_service_array.load(file) != 0) return -1;
	if(player_secondlevel_config_array.load(file) != 0) return -1;
	if(npc_resetprop_service_array.load(file) != 0) return -1;
	if(estone_essence_array.load(file) != 0) return -1;
	if(pstone_essence_array.load(file) != 0) return -1;
	if(sstone_essence_array.load(file) != 0) return -1;

	fread(&tag, sizeof(unsigned int), 1, file);
	fread(&len, sizeof(unsigned int), 1, file);
	fread(buffer, len, 1, file);
	
	if(reciperoll_major_type_array.load(file) != 0) return -1;
	if(reciperoll_sub_type_array.load(file) != 0) return -1;
	if(reciperoll_essence_array.load(file) != 0) return -1;
	if(suite_essence_array.load(file) != 0) return -1;
	if(double_exp_essence_array.load(file) != 0) return -1;
	if(destroying_essence_array.load(file) != 0) return -1;

	md5pos[2] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	if(npc_equipbind_service_array.load(file) != 0) return -1;
	if(npc_equipdestroy_service_array.load(file) != 0) return -1;
	if(npc_equipundestroy_service_array.load(file) != 0) return -1;
	if(skillmatter_essence_array.load(file) != 0) return -1;
	if(vehicle_essence_array.load(file) != 0) return -1;
	if(couple_jumpto_essence_array.load(file) != 0) return -1;
	if(lottery_essence_array.load(file) != 0) return -1;
	if(camrecorder_essence_array.load(file) != 0) return -1;
	if(title_prop_config_array.load(file) != 0) return -1;
	if(special_id_config_array.load(file) != 0) return -1;
	if(text_fireworks_essence_array.load(file) != 0) return -1;
	if(talisman_mainpart_essence_array.load(file) != 0) return -1;
	if(talisman_expfood_essence_array.load(file) != 0) return -1;
	if(talisman_mergekatalyst_essence_array.load(file) != 0) return -1;
	if(talisman_energyfood_essence_array.load(file) != 0 ) return -1;
	if(speaker_essence_array.load(file) != 0) return -1;
	if(player_talent_config_array.load(file) != 0) return -1;
	if(player_skill_point_config_array.load(file) != 0) return -1;
	if(potential_tome_essence_array.load(file) != 0) return -1;
	if(war_role_config_array.load(file) != 0) return -1;
	if(npc_war_buy_archer_service_array.load(file) != 0) return -1;
	if(siege_artillery_scroll_essence_array.load(file) != 0) return -1;
	if(pet_bedge_essence_array.load(file) != 0) return -1;
	if(pet_food_essence_array.load(file) != 0) return -1;
	if(pet_skill_essence_array.load(file) != 0) return -1;
	if(pet_armor_essence_array.load(file) != 0) return -1;
	if(pet_auto_food_essence_array.load(file) != 0) return -1;
	if(pet_refine_essence_array.load(file) != 0) return -1;
	if(pet_assist_refine_essence_array.load(file) != 0) return -1;
	if(renasence_skill_config_array.load(file) != 0) return -1;
	if(renasence_prop_config_array.load(file) != 0) return -1;
	if(aircraft_essence_array.load(file) != 0) return -1;
	if(fly_energyfood_essence_array.load(file) != 0) return -1;
	if(item_trade_config_array.load(file) != 0) return -1;
	if(book_essence_array.load(file) != 0) return -1;
	if(equip_soul_essence_array.load(file) !=0) return -1;
	if(equip_soul_meld_service_array.load(file) != 0) return -1;
	if(special_name_item_essence_array.load(file) != 0) return -1;
	if(recycle_item_config_array.load(file) != 0) return -1;
	if(score_to_rank_config_array.load(file) != 0) return -1;
	if(battle_drop_config_array.load(file) != 0) return -1;

	md5pos[3] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	if(battle_deprive_config_array.load(file) != 0) return -1;
	if(battle_score_config_array.load(file) != 0) return -1;
	if(gift_bag_essence_array.load(file) != 0) return -1;
	if(vip_card_essence_array.load(file) != 0) return -1;
	if(instancing_battle_config_array.load(file) != 0) return -1;
	if(change_shape_card_essence_array.load(file) != 0) return -1;
	if(change_shape_stone_essence_array.load(file) != 0) return -1;
	if(change_shape_prop_config_array.load(file)!= 0) return -1;
	if(original_shape_config_array.load(file)!=0) return -1;
	if(life_skill_config_array.load(file)!=0) return -1;
	if(arena_droptable_config_array.load(file)!=0) return -1;
	if(mercenary_credential_essence_array.load(file)!=0) return -1;
	if(teleportation_essence_array.load(file)!=0) return -1;
	if(teleportation_stone_essence_array.load(file)!=0) return -1;
	if(combine_skill_edit_config_array.load(file)!=0) return -1;
	if(upgrade_equip_config_array.load(file)!=0) return -1;
	if(upgrade_equip_config_1_array.load(file)!=0) return -1;
	if(upgrade_equip_stone_essence_array.load(file)!=0) return -1;
	if(npc_consign_service_array.load(file) != 0) return -1;
	if(deity_character_config_array.load(file) != 0) return -1;
	if(lottery2_essence_array.load(file) != 0) return -1;
	if(gem_config_array.load(file) != 0) return -1;//Added 2011-03-14.
	if(gem_essence_array.load(file) != 0) return -1;
	if(gem_seal_essence_array.load(file) != 0) return -1;
	if(gem_dust_essence_array.load(file) != 0) return -1;
	if(gem_extract_config_array.load(file) != 0) return -1;
	if(general_article_essence_array.load(file) != 0) return -1;
	if(lottery3_essence_array.load(file) != 0) return -1; //Added 2011-07-01.
	if(transcription_config_array.load(file) != 0) return -1; //Added 2011-07-13.
	if(online_gift_bag_config_array.load(file) != 0) return -1; //Added 2011-12-06.
	if(small_online_gift_bag_essence_array.load(file) != 0) return -1; //Added 2011-12-06.
	if(scroll_region_config_array.load(file) != 0) return -1;	//Added 2012-03-20.
	if(scroll_unlock_essence_array.load(file) != 0) return -1;	//Added 2012-03-20.
	if(scroll_dig_count_inc_essence_array.load(file) != 0) return -1;	//Added 2012-03-22.
	if(tower_transcription_config_array.load(file) != 0) return -1;		//Added 2012-03-31.
	if(tower_transcription_property_config_array.load(file) != 0) return -1;//Added 2012-03-31.
	if(rune_data_config_array.load(file) != 0) return -1;//Added 2012.04.17
	if(rune_comb_property_array.load(file) != 0) return -1;//Added 2012.04.17
	if(rune_equip_array.load(file) != 0) return -1;//Added 2012.04.17
	if(little_pet_upgrade_config_array.load(file) != 0) return -1; //Added 2012-04-19.
	if(rune_slot_array.load(file) != 0) return -1; //Added 2012-04-21.
	if(drop_interval_config_array.load(file) != 0) return -1;
	if(pk2012_guess_config_array.load(file) != 0) return -1;
	if(collision_raid_transfigure_config_array.load(file) != 0) return -1;	//Added 2012-08-02.
	if(booth_figure_item_array.load(file) != 0) return -1;	
	if(flag_buff_item_array.load(file) != 0) return -1;	
	if(npc_reputation_shop_service_array.load(file) != 0) return -1;
	if(npc_ui_transfer_service_array.load(file) != 0) return -1;
	if(ui_transfer_config_array.load(file) != 0) return -1;
	if(xingzuo_item_essence_array.load(file) != 0) return -1;
	if(xingzuo_energy_item_essence_array.load(file) != 0) return -1;
	if(collision_raid_award_config_array.load(file) != 0) return -1;
	if(cash_medic_merge_item_essence_array.load(file) != 0) return -1;
	if(buff_area_config_array.load(file) != 0) return -1;
	if(liveness_config_array.load(file) != 0) return -1;
	if(challenge_2012_config_array.load(file) != 0) return -1;
	if(sale_promotion_item_essence_array.load(file) != 0) return -1;
	if(gift_bag_lottery_deliver_essence_array.load(file) != 0) return -1;
	if(lottery_tangyuan_item_essence_array.load(file) != 0) return -1;
	if(task_special_award_config_array.load(file) != 0) return -1;
	if(gift_pack_item_essence_array.load(file) != 0) return -1;
	if(prop_add_config_array.load(file) != 0) return -1;
	if(prop_add_material_item_essence_array.load(file) != 0) return -1;
	if(prop_add_item_essence_array.load(file) != 0) return -1;
	if(king_war_config_array.load(file) != 0) return -1;
	if(jinfashen_to_money_config_array.load(file) != 0) return -1;
	if(battle_201304_config_array.load(file) != 0) return -1;
	if(rune2013_item_essence_array.load(file) != 0) return -1;
	if(rune2013_config_array.load(file) != 0) return -1;
	if(bashe_award_config_array.load(file) != 0) return -1;
	if(arena_season_time_config_array.load(file) != 0) return -1;
	if(personal_level_award_config_array.load(file) != 0) return -1;
	if(team_season_award_config_array.load(file) != 0) return -1;
	if(week_currency_award_config_array.load(file) != 0) return -1;
	if(colorant_item_essence_array.load(file) != 0) return -1;
	if(interaction_object_essence_array.load(file) != 0) return -1;
	if(interaction_object_hook_config_array.load(file) != 0) return -1;
	if(colorant_config_array.load(file) != 0) return -1;
	if(faction_transfer_config_array.load(file)  != 0) return -1;
	if(building_region_config_array.load(file) != 0) return -1;
	if(building_essence_array.load(file) != 0) return -1;
	if(region_block_essence_array.load(file) != 0) return -1;
	if(phase_config_array.load(file) != 0) return -1;
	if(cross6v6_item_exchange_config_array.load(file) != 0) return -1;
	if(transcript_strategy_config_array.load(file) != 0) return -1;
	if(faction_shop_config_array.load(file) != 0) return -1;
	if(faction_auction_config_array.load(file) != 0) return -1;
	if(faction_money_shop_config_array.load(file) != 0) return -1;
	if(compound_mine_essence_array.load(file) != 0) return -1;
	if(colorant_default_config_array.load(file) != 0) return -1;
	size_t sz = 0;
	if(fread(&sz, sizeof(size_t), 1, file) != 1)	return -1;
	size_t i;
	for(i=0; i<sz; i++)
	{
		talk_proc * tp = new talk_proc;
		tp->load(file);
		add_structure(tp->id_talk, tp);
	}

	setup_hash_map();

	md5pos[4] = ftell(file);

#ifdef _WINDOWS
	fseek(file, 0, SEEK_SET);
	long fstart = 0;
	char buf[8192];
	char md5[50];
	for(i = 0; i < 5; i++)
	{
		while(fstart < md5pos[i])
		{
			int readsize = 8192;
			if( md5pos[i] - fstart < readsize )
				readsize = md5pos[i] - fstart;
			fread(buf, 1, readsize, file);
			g_MD5Hash.Update(buf, readsize);
			fstart += readsize;
		}
		if( i < 4 )
			fread(md5 + i * 8, 1, 8, file);
		fstart += 8;
	}

	g_MD5Hash.Final();
	unsigned int size = 50;
	g_MD5Hash.GetString(buf, size);
	if( memcmp(buf, md5, 32) != 0 )
		return -1;
#endif

	fclose(file);	
	return 0;	
}
