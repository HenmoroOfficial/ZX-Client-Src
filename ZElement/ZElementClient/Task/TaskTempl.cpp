#include "TaskTempl.h"
#include "TaskTemplMan.h"

//Added by Ford.W 2010-06-25.
#ifdef TASK_TEMPL_EDITOR
#include "BaseDataIDMan.h"
	//Added by Ford.W 2010-06-28.
bool ATaskTempl::m_bCopySubTask = false;
	//Added end.
#endif
//Added end.

/* Version Info */
unsigned long _task_templ_cur_version = 165;

static const char _format_version[]			= "MOXTVersion: %u";
static const char _format_task_count[]		= "TaskCount: %d";

/* 任务对象的属性 */
static const char _format_id[]				= "ID: %u";
static const char _format_type[]			= "Type: %u";
static const char _format_dyn[]				= "Dyn: %d";
static const char _format_has_sign[]		= "HasSign: %d";
static const char _format_time_limit[]		= "TimeLimit: %u";
static const char _format_abs_time[]		= "AbsTime: %d";
static const char _format_child_in_order[]	= "ChildInOrder: %u";
static const char _format_choose_one[]		= "ChooseOne: %u";
static const char _format_rand_one[]		= "RandOne: %u";
static const char _format_parent_fail[]		= "ParentAlsoFail: %d";
static const char _format_parent_succ[]		= "ParentAlsoSuccess: %d";
static const char _format_can_give_up[]		= "CanGiveUp: %d";
static const char _format_die_as_fail[]		= "DieAsFail: %d";
static const char _format_can_redo[]		= "CanRedo: %d";
static const char _format_redo_after_fail[]	= "CanRedoAfterFailure: %d";
static const char _format_clear_as_giveup[]	= "ClearAsGiveUp: %d";
static const char _format_need_record[]		= "NeedRecord: %d";
static const char _format_max_receiver[]	= "MaxReceiver: %u";
static const char _format_clear_rev_type[]  = "ClearReceiverType: %d";
static const char _format_clear_rev_t_i[]   = "ClearReceiverTimeInterval: %d";
static const char _format_must_in_zone[]	= "MustInZone: %u";
static const char _format_trans_to[]		= "TransTo: %u";
static const char _format_world_id[]		= "WorldId: %u";
static const char _format_zone_vert[]		= "ZoneVert: %f, %f, %f";
static const char _format_controller[]		= "Controller: %d";
static const char _format_trig_flag[]		= "TrigFlag: %d";
static const char _format_auto_deliver[]	= "AutoDeliver: %d";
static const char _format_auto_deliver_mode[]= "AutoDeliverWindowMode: %d";
static const char _format_death_trig[]		= "DeathTrig: %d";
static const char _format_manual_trig[]		= "ManualTrig: %d";
static const char _format_key_task_must_show[]= "KeyTaskMustShown: %d";
static const char _format_clear_acquired[]	= "ClearAcquired: %d";
static const char _format_suit_lev[]		= "SuitLev: %d";
static const char _format_show_prompt[]		= "ShowPrompt: %d";
static const char _format_key_task[]		= "KeyTask: %d";
static const char _format_npc[]				= "NPC: %d";
static const char _format_skill_task[]		= "SkillTask: %d";
static const char _format_seek_out[]		= "SeekOut: %d";
static const char _format_direction[]		= "Direction: %d";
static const char _format_weight[]			= "Weight: %f";
static const char _format_rank[]			= "Rank: %d";
static const char _format_avail_frequency[]	= "AvailFrequency: %d";
static const char _format_time_interval[]	= "TimeInterval: %d";
static const char _format_marriage[]		= "%d";
static const char _format_faction_task[]	= "FactionTask: %d";
static const char _format_shared_by_fam[]	= "SharedByFamily: %d";
static const char _format_rec_finish_count[]= "RecFinishCount: %d";
static const char _format_finish_count[]	= "FinishCount: %d";
static const char _format_lifeagain_reset[] = "LifeAgainReset: %d";
static const char _format_fail_logout[]     = "FailAfterLogout: %d";
static const char _format_logout_fail_time[] = "LogoutFailTime: %d";
static const char _format_prentice_task[]   = "PrenticeTask: %d";
static const char _format_hidden[]          = "Hidden: %u";
static const char _format_out_zone_fail[]   = "OutZoneFail: %u";
static const char _format_enter_zone_fail[] = "EnterZoneFail: %u";
static const char _format_cam_move[]        = "CameraMove: %d";
static const char _format_animation[]        = "Animation: %d";
static const char _format_display_type[]	= "DisplayType: %d";
static const char _format_recommend_type[]	= "RecommendType: %d";
static const char _format_tiny_game_id[]	= "TinyGameID: %d";
static const char _format_dyn_clear_time[]  = "DynFinishClearTime: %d";
static const char _format_clear_xp_cd[]		= "ClearXpCD: %d";

/* 任务的开启条件及方式 */
static const char _format_prem_level[]		= "PremiseLevel: %u";
static const char _format_talisman_value[]	= "TalismanValue: %d";
static const char _format_consume_treasure[]= "ConsumeTreasure: %d";
static const char _format_total_caseadd[]   = "TotalCaseAdd: %d";
static const char _format_prem_deposit[]	= "PremiseDeposit: %u";
static const char _format_prem_contrib[]	= "PremiseContrib: %d, %d";
static const char _format_prem_fam_contrib[]= "PremiseFamContrib: %d, %d";
static const char _format_prem_fam_max[]	= "PremFamContribMax: %d";
static const char _format_prem_reputation[]	= "PremiseReputation: %u";
static const char _format_prem_repu_depo[]	= "PremiseRepuDeposit: %d";
static const char _format_prem_task_count[]	= "PremiseTaskCount: %u";
static const char _format_prem_task[]		= "PremiseTask: %u";
static const char _format_prem_period[]		= "PremisePeriod: %u";
static const char _format_prem_npc[]		= "PremiseNPC: %u";
static const char _format_prem_is_team[]	= "PremiseIsTeam: %d";
static const char _format_prem_share[]		= "PremiseIsShare: %d";
static const char _format_prem_is_spouse[]	= "PremiseIsSpouse: %d";
static const char _format_prem_clan[]		= "PremiseClan: %u";
static const char _format_prem_gender[]		= "PremiseGender: %u";
static const char _format_prem_occupation[]	= "PremiseOccupation: %u";
static const char _format_prem_cotask[]		= "PremiseCotask: %u";
static const char _format_cotask_cond[]		= "CotaskCond: %u";
static const char _format_mutex_task_count[]= "MutexTaskCount: %u";
static const char _format_mutex_task[]		= "MutexTask: %u";
static const char _format_rcv_by_team[]		= "RcvByTeam: %d";
static const char _format_shared[]			= "SharedWithNew: %d";
static const char _format_share_acquire[]	= "ShareAcquired: %d";
static const char _format_check_tmmate[]	= "CheckTeammate: %d";
static const char _format_rcv_chck_mem[]	= "RcvChckMem: %d";
static const char _format_cnt_by_mem_pos[]	= "CntByMemPos: %d";
static const char _format_all_fail[]		= "TeamAllFail: %d";
static const char _format_cap_fail[]		= "TeamCapFail: %d";
static const char _format_cap_succ[]		= "TeamCapSucc: %d";
static const char _format_all_succ[]		= "TeamAllSucc: %d";
static const char _format_dism_self_fail[]	= "DismissAsSelfFail: %d";
static const char _format_dism_all_fail[]	= "DismissAsAllFail: %d";
static const char _format_mem_num_wanted[]	= "MemNumWanted: %d";
static const char _format_race[]			= "Race: %d";
static const char _format_occupation[]		= "Occupation: %X";
static const char _format_task[]			= "Task: %d";
static const char _format_level[]			= "Level: %d";
static const char _format_gender[]			= "Gender: %d";
static const char _format_lifeagain[]       = "LifeAgain: %d";

static const char _format_master_wugui[]    = "IsMaster: %d";
static const char _format_prentice[]        = "IsPrentice: %d";
static const char _format_master_moral[]    = "MasterMoral: %d";
static const char _format_cap_change_fail[] = "CapChangeAllFail: %d";
static const char _format_mp_task[]         = "MasterPrenticeTask :%d";
static const char _format_out_master_task[] = "OutMasterTask: %d";
static const char _format_mp_task_cnt[]     = "MPTaskCount: %d";
static const char _format_mp_lev_limit[]    = "MPTaskLevLimit: %d";
static const char _format_mp_task_id[]      = "MPTaskID: %d";

static const char _format_count[]			= "Count: %d";
static const char _format_same_family[]		= "SaveFamily: %d";
static const char _format_full_tm[]			= "Y: %d, M: %d, D: %d, H: %d, M: %d, W: %d";
static const char _format_full_tm_old[]		= "Y: %d, M: %d, D: %d, H: %d, M: %d";
static const char _format_abs_fail_time[]	= "AbsFailTime: %d";
static const char _format_dist[]			= "Dist: %f";
static const char _format_given_items_num[]	= "GivenItemsNum: %d";
static const char _format_proficiency[]		= "Proficiency: %d";
static const char _format_special_award[]	= "SpeAward: %d";
static const char _format_tm_type[]			= "TimeType: %d";
static const char _format_pk_value[]		= "PKValue: %d";
static const char _format_gm[]				= "GM: %d";
static const char _format_master[]			= "Master: %d";
static const char _format_in_family[]		= "IsFamily: %d";
static const char _format_family_header[]	= "IsFamilyHeader: %d";
static const char _format_f_skill_lev[]		= "FamilySkillLev: %d";
static const char _format_f_skill_prof[]	= "FamilySkillProf: %d";
static const char _format_f_skill_index[]	= "FamilySkillIndex: %d";
static const char _format_record_index[]	= "RecordIndex: %d";
static const char _format_record_value[]	= "RecordValue: %d";
static const char _format_f_value_index[]	= "FamilyValueIndex: %d";
static const char _format_f_value[]			= "FamilyValue: %d";
static const char _format_f_deposit_value[]	= "FamilyDepositValue: %d";
static const char _format_battle_score[]	= "BattleScore: %d";
static const char _format_deposit_battle[]	= "DepositBattle: %d";
static const char _format_prem_skill_lev[]  = "PremSkillLev: %d";

static const char _format_check_lifeagain[] = "CheckLifeAgain: %d";
static const char _format_spouse_again[]    = "SpouseAgain: %d";
static const char _format_lifeagain_cnt[]   = "LifeAgainCnt: %d";
static const char _format_lifeagain_cntcom[]= "LifeAgainCntCompare: %d";
static const char _format_lifeagain_occup[] = "LifeAgainOccup: %d";

static const char _format_prem_cult[]       = "PremCultivate: %d";

static const char _format_pet_con[]         = "PremPetCon: %d";
static const char _format_pet_civ[]         = "PremPetCiv: %d";

static const char _format_clear_illegal[]   = "ClearIllegalStates: %d";

static const char _format_change_key_size[] = "ChangeKeySize: %d";
static const char _format_change_key[]		= "ChangeKey: %d";
static const char _format_change_kv[]		= "ChangeKeyValue: %d";
static const char _format_change_typ[]		= "ChangeType: %d";
static const char _format_multi_global_key[]= "MultiGlobalKey: %d";

static const char _format_kill_monster_fail[]= "KillMonsterFail: %d";
static const char _format_kill_monster_fail_cand[]= "KillFailMonsterCand: %d";
static const char _format_kill_monster_fail_id[]= "KillFailMonsterID: %d";

static const char _format_have_item_fail[]		= "HaveItemFail: %d";
static const char _format_have_item_fail_not_take_off[]	= "HaveItemFailNotTakeOff: %d";
static const char _format_have_item_fail_cand[]	= "HaveFailItemCand: %d";
static const char _format_have_item_fail_id[]	= "HaveFailItemID: %d";

static const char _format_not_have_item_fail[]		= "NotHaveItemFail: %d";
static const char _format_not_have_item_fail_cand[]	= "NotHaveFailItemCand: %d";
static const char _format_not_have_item_fail_id[]	= "NotHaveFailItemID: %d";

static const char _format_by_deposit[]		= "ShowByDeposit: %d";
static const char _format_by_gender[]		= "ShowByGender: %d";
static const char _format_by_items[]		= "ShowByItems: %d";
static const char _format_by_lev[]			= "ShowByLev: %d";
static const char _format_by_occup[]		= "ShowByOccup: %d";
static const char _format_by_preTask[]		= "ShowByPreTask: %d";
static const char _format_by_repu[]			= "ShowByRepu: %d";
static const char _format_by_team[]			= "ShowByTeam: %d";
static const char _format_by_faction[]		= "ShowByFaction: %d";
static const char _format_by_period[]		= "ShowByPeriod: %d";
static const char _format_by_spouse[]		= "ShowBySpouse: %d";
static const char _format_by_fengshen[]		= "ShowByFengshen: %d";

static const char _format_need_comp[]       = "NeedCompare: %d";
static const char _format_exp1_andor_exp2[]	= "PremExp1AndOrExp2: %d";
static const char _format_comp_oper[]       = "CompareOper: %d";
static const char _format_left_string[]     = "LeftString: %s";
static const char _format_right_string[]    = "RightString: %s";

static const char _format_transform_id[]    = "TransformID: %d";
static const char _format_transform_lev[]   = "TransformLevel: %d";
static const char _format_transform_dur[]   = "TransformDuration: %d";
static const char _format_transform_cover[] = "TransformCover: %d";

static const char _format_monster_summon_cand[]	= "MonsterSummonCand: %u";
static const char _format_monster_summon_mode[]	= "MonsterSummonMode: %d";
static const char _format_monster_summon_is_monster[] = "IsMonster: %d";

static const char _format_nation_position_mask[] = "NationPositionMask: %u";
static const char _format_king_score_cost[] = "KingScoreCost: %d";
static const char _format_max_king_score[]  = "MaxKingScore: %d";
static const char _format_min_king_score[]  = "MinKingScore: %d";
static const char _format_has_king[]		= "HasKing: %d";


/* 任务完成的方式及条件 */
static const char _format_method[]			= "Method: %u";
static const char _format_finish_type[]		= "FinishType: %u";
static const char _format_finish_achieve[]	= "FinishAchievement: %u";
static const char _format_finish_friend_num[]= "FinishFriendNum: %u";
static const char _format_finish_lev[]		= "FinishLev: %u";
static const char _format_finish_dlg[]      = "FinishDlg: %d";
static const char _format_fixed_time_type[] = "FixedTimeType: %d";

static const char _format_monster_wanted[]	= "MonsterWanted: %u";
static const char _format_summon_monster_mode[]	= "IsSummonMonsterMode: %d";
static const char _format_monster_templ[]	= "MonsterTempl: %u";
static const char _format_monster_num[]		= "MonsterNum: %u";
static const char _format_item_drop_prob[]	= "ItemDropProb: %f";
static const char _format_bind[]			= "Bind: %d";
static const char _format_period[]			= "Period: %d";
static const char _format_time_point[]		= "TimePoint: %d";
static const char _format_tp_week[]			= "W: %d, H: %d, M: %d";
static const char _format_killer_lev[]		= "KillerLev: %d";
static const char _format_item_wanted_num[]	= "ItemWantedNum: %u";
static const char _format_item_not_take_off[]= "ItemNotTakeOff: %d";
static const char _format_item_templ[]		= "ItemTempl: %u";
static const char _format_item_is_cmn[]		= "IsCommon: %d";
static const char _format_item_num[]		= "ItemNum: %u";
static const char _format_item_refine_cond[]= "ItemRefineCondition: %d";
static const char _format_item_refine_level[]= "ItemRefineLevel: %d";
static const char _format_item_replace_templ[] = "ItemReplaceTempl: %d";
static const char _format_npc_to_protect[]	= "NPCToProtect: %u";
static const char _format_protect_tm_len[]	= "ProtectTimeLen: %u";
static const char _format_waittime[]		= "WaitTime: %u";
static const char _format_gold[]			= "GoldNum: %u";
static const char _format_friendship[]		= "Friendship: %d";
static const char _format_friendship_reset[]= "FriendshipReset: %d";
static const char _format_friendship_depo[]	= "FriendshipDepo: %d";
static const char _format_title_count[]		= "TitleCount: %d";
static const char _format_title[]			= "Title: %d";
static const char _format_accomp_cnt[]      = "AccompCnt: %d";
static const char _format_accomp_id[]       = "AccompID: %d";
static const char _format_accomp_cond[]     = "AccompCond: %d";
static const char _format_script_open[]		= "ScriptOpenTask: %d";
static const char _format_script_finish[]	= "ScriptFinishTask: %d";
static const char _format_prem_achieve_min[]= "PremAchievementMin: %d";
static const char _format_prem_achieve_max[]= "PremAchievementMax: %d";
static const char _format_prem_circle_group_min[]= "PremCircleGroupMin: %d";
static const char _format_prem_circle_group_max[]= "PremCircleGroupMax: %d";
static const char _format_prem_territory_score_min[]= "PremTerritoryScoreMin: %d";
static const char _format_prem_territory_score_max[]= "PremTerritoryScoreMax: %d";

static const char _format_prem_fengshen_type[]		= "PremFengshenType: %d";
static const char _format_prem_fengshen_lvl_min[]	= "PremFengshenLevelMin: %d";
static const char _format_prem_fengshen_lvl_max[]	= "PremFengshenLevelMax: %d";
static const char _format_prem_fengshen_exp_full[]	= "PremFengshenExpFull: %d";

static const char _format_action_npc[]		= "ActionNPC: %u";
static const char _format_action_id[]		= "ActionID: %d";

static const char _format_faction_gold_note_wanted[] = "FactionGoldNoteWanted: %d";
static const char _format_faction_grass_wanted[] = "FactionGrassWanted: %d";
static const char _format_faction_mine_wanted[] = "FactionMineWanted: %d";
static const char _format_faction_monster_core_wanted[] = "FactionMonsterCoreWanted: %d";
static const char _format_faction_mosnter_food_wanted[] = "FactionMonsterFoodWanted: %d";
static const char _format_faction_money_wanted[] = "FactionMoneyWanted: %d";

static const char _format_interobj_wanted[]	= "InterObjWanted: %d";
static const char _format_interobj_templ[]	= "InterObjTempl: %d";
static const char _format_interobj_num[]    = "InterObjNum: %u";

static const char _format_build_id_wanted[] = "BuildIdWant: %d";
static const char _format_build_level_wanted[] = "BuildLevelWanted: %d";


/* 任务结束后的奖励 */
static const char _format_award_type[]		= "AwardType: %u";
static const char _format_award_gold_num[]	= "GoldNum: %u";
static const char _format_award_exp_num[]	= "ExpNum: %I64d";
static const char _format_award_cmn_item[]	= "CommonItem: %u";
static const char _format_award_task_item[]	= "TaskItem: %u";
static const char _format_award_sp_num[]	= "SPNum: %u";
static const char _format_award_repu[]		= "Reputation: %u";
static const char _format_award_title[]		= "Title: %d";
static const char _format_award_contrib[]	= "Contrib: %d";
static const char _format_award_prosp[]		= "Prosperity: %d";
static const char _format_award_reset_pk[]	= "ResetPKVale: %d";
static const char _format_award_divorce[]	= "Divorce: %d";
static const char _format_award_period[]	= "Period: %u";
static const char _format_award_station[]	= "Station: %u";
static const char _format_award_store[]		= "StoreSize: %u";
static const char _format_award_invent[]	= "InventSize: %d";
static const char _format_award_pocket[]    = "PocketSize: %d";
static const char _format_award_pet[]		= "PetNum: %u";
static const char _format_award_mount[]		= "MountNum: %u";
static const char _format_award_fury[]		= "Fury: %u";
static const char _format_award_skill_lev[]	= "SetSkill: %u";
static const char _format_award_skill_exp[]	= "SkillExp: %u";
static const char _format_award_prof[]		= "Prof: %u";
static const char _format_award_lifeag_cnt[]= "LifeAgaincnt: %u";
static const char _format_award_lifeag_pro[]= "LifeAgainProf: %u";
static const char _format_award_new_task[]	= "NewTask: %u";
static const char _format_ratio[]			= "Ratio: %f";
static const char _format_award_cands_num[]	= "CandsNum: %d";
static const char _format_prob[]			= "Prob: %f";
static const char _format_award_buffid[]	= "BuffId: %d";
static const char _format_award_bufflev[]	= "BuffLev: %d";
static const char _format_award_msg[]		= "SendMsg: %d";
static const char _format_award_channel[]	= "Channel: %d";
static const char _format_double_exp_time[] = "DoubleExp: %d";
static const char _format_special_award_type[] = "SpecialAwardType: %d";
static const char _format_special_award_id[] = "SpecialAwardID: %d";
static const char _format_award_m_moral[]   = "AwardMasterMoral: %d";
static const char _format_award_out[]       = "OutMaster: %d";
static const char _format_award_deviate[]   = "DeviateMaster: %d";
static const char _format_award_role[]      = "AwardSpecifyRole: %d";
static const char _format_award_role_Sel[]  = "RoleSelected: %d";
static const char _format_award_set_cult[]  = "AwardSetCult: %d";
static const char _format_award_clearskill[]= "AwardClearCultSkill: %d";
static const char _format_award_o_m_exp[]   = "AwardOutMasterExp: %d";
static const char _format_award_o_m_moral[] = "AwardOutMasterMoral: %d";
static const char _format_award_exp_coef[]  = "AwardExpCoef: %f";
static const char _format_award_cl_sk_p[]   = "AwardClearSkillPoints: %d";
static const char _format_award_cl_bo_p[]   = "AwardClearBookPoints: %d";
static const char _format_award_exp_sel[]   = "ParaExpSel: %d";
static const char _format_award_exp_size[]  = "ParaExpSize: %d";
static const char _format_award_reset_sj[]	= "ResetSJBattleScore: %d";
static const char _format_award_clear_nkat[]= "ClearNoKeyActiveTask: %d";

static const char _format_award_ctrl_cnt[]  = "MonsterCtrlCnt: %d";
static const char _format_award_ctrl_id[]   = "MonsterCtrlID: %d";
static const char _format_award_ctrl_prob[] = "MonsterCtrlProb: %f";
static const char _format_award_ctrl_open[] = "MonsterCtrlOpen: %d";
static const char _format_award_ctrl_rand[] = "MonsterCtrlRan: %d";

static const char _format_award_cam_move[]  = "CameraMove: %d";
static const char _format_award_circle_group_pt[] = "CircleGroupPoint: %d";

static const char _format_award_animation[] = "AwardAnimation: %d";

static const char _format_award_ter_task_cnt[]  = "TerminateCnt: %u";
static const char _format_award_ter_task_id[]  = "TerminateID: %u";

static const char _format_award_fengshen_exp[] = "FengshenExp: %u";
static const char _format_award_fengshen_dujie[] = "FengshenDujie: %d";

//Added 2011-02-18.
static const char _format_award_bonus_num[] = "BonusNum: %d";
static const char _format_award_battle_field_reputaion[] = "BattleFieldReputation: %d";
static const char _format_award_faction_extra_cands_num[] = "ExtraFactionAwardCandsNum: %d";
//Added 2011-03-04.
static const char _format_award_extra_cands_num[] = "ExtraAwardCandsNum: %d";
//Added 2011-03-07.
static const char _format_award_extra_ctrl_cnt[]	= "ExtraMonsterCtrlCnt: %d";
static const char _format_award_extra_ctrl_id[]		= "ExtraMonsterCtrlID: %d";
static const char _format_award_extra_ctrl_prob[]	= "ExtraMonsterCtrlProb: %f";
static const char _format_award_extra_ctrl_open[]	= "ExtraMonsterCtrlOpen: %d";
static const char _format_award_extra_ctrl_rand[]	= "ExtraMonsterCtrlRan: %d";
//Added 2011-03-08.
static const char _format_award_check_fnsh_global_count[]	= "CheckGlobalFinishCount: %d";
static const char _format_award_global_fnsh_count_precondition[]	= "GlobalFinishCountPrecondition: %d";
static const char _format_award_check_global_comp_expression[]	= "CheckGlobalCompareExpression: %d";
//Added 2011-03-30
static const char _format_award_variable[]						= "VariableForExtraAward: %d";
//Added 2011-04-11
static const char _format_variables[]							= "Variables: %d";
static const char _format_create_role_time_duration[]			= "CreateRoleTimeDuration: %d";

static const char _format_building_id[] = "BuildID: %d";
static const char _format_building_level[] = "BuildLevel: %d";
static const char _format_faction_premise_gold_note[] = "FactionPremiseGoldNote: %d";
static const char _format_by_gold_note[] = "ByGoldNote: %d";
static const char _format_faction_premise_grass[] = "FactionPremiseGrass: %d";
static const char _format_faction_premise_mine[] = "FactionPremiseMine: %d";
static const char _format_faction_premise_monster_core[] = "FactionPremiseMosnterCore: %d";
static const char _format_faction_premise_mosnter_food[] = "FactionPremiseMonsterFood: %d";
static const char _format_faction_premise_money[] = "FactionPremiseMoney: %d";

static const char _format_build_level_in_construct[] = "BuildLevelCon: %d";

static const char _format_interobj_id[] = "InterObjID: %d";
static const char _format_by_interobj_id[] = "ByInterObj: %d";

//Added 2011-10-08.
static const char _format_auto_move_enable[]					= "AutoMoveEnable: %d";
static const char _format_show_wait_time_enable[]				= "ShowWaitTimeEnable: %d";
static const char _format_auto_move_for_collect_num_items_enable[] = "AutoMoveEnbaleForCollectNumItems: %d";
//Added 2012-04-18.
static const char _format_award_open_soul_equip[]				= "OpenSoulEquip: %d";

static const char _format_award_king_score[] = "AwardKingScore: %d";

static const char _format_award_Phase_cnt[]  = "PhaseCnt: %d";
static const char _format_award_Phase_id[]   = "PhaseID: %d";
static const char _format_award_Phase_open[] = "PhaseOpen: %d";
static const char _format_prop_Phase_triger[] = "PhaseTriger: %d";
static const char _format_prop_Phase_visual[] = "PhaseVisual: %d";

static const char _format_award_auction_cnt[] = "AuctionCnt: %d";
static const char _format_award_auction_id[] = "AuctionId: %d";
static const char _format_award_auction_prob[] = "AuctionProb: %f";

static const char _format_award_faction_gold_note[] = "FactionGoldNote: %d";
static const char _format_award_faction_grass[] = "FactionGrass: %d";
static const char _format_award_faction_mine[] = "FactionMine: %d";
static const char _format_award_faction_monster_core[] = "FactionMosnterCore: %d";
static const char _format_award_faction_monster_food[] = "FactionMonsterFood: %d";
static const char _format_award_faction_money[] = "FactionMoney: %d";
static const char _format_award_faction_building_progress[] = "BuildingProgress: %d";

static const char _format_award_faction_extra_gold_note[] = "FactionExtraGoldNote: %d";
static const char _format_award_faction_extra_grass[] = "FactionExtraGrass: %d";
static const char _format_award_faction_extra_mine[] = "FactionExtraMine: %d";
static const char _format_award_faction_extra_monster_core[] = "FactionExtraMosnterCore: %d";
static const char _format_award_faction_extra_monster_food[] = "FactionExtraMonsterFood: %d";
static const char _format_award_faction_extra_money[] = "FactionExtraMoney: %d";

static const char _format_award_travel_item[] = "Travel_Item: %d";
static const char _format_award_travel_time[] = "Travel_Time: %d";
static const char _format_award_travel_speed[] = "Travel_Speed: %f";
static const char _format_award_travel_path[] = "Travel_Path: %d";


/* 层次关系 */
static const char _format_parent[]			= "ParentId: %u";
static const char _format_next_sibling[]	= "NextSibling: %u";
static const char _format_prev_sibling[]	= "PrevSibling: %u";
static const char _format_first_child[]		= "FirstChild: %u";

static const char _format_descpt_len[]		= "DescriptLen: %u";
static const char _format_tribute_len[]		= "TributeLen: %u";
static const char _format_hint_len[]		= "HintLen: %u";
static const char _format_can_deliver_len[]	= "CanDeliverLen: %u";

const unsigned long _race_occ_map[8] =
{
	1,
	1,
	1,
	2,
	2,
	3,
	4,
	4
};

unsigned long _living_skill_ids[MAX_LIVING_SKILLS] =
{
	158, // 武器制造
	159, // 护具制造
	160, // 饰品制造
	161	 // 药品、符箓制造
};

storage_task_info _storage_task_info[STORAGE_TASK_TYPES] =
{
	{	0,		false	},
	{	0.01f,	true	},//挑战
	{	0.1f,	true	},//平妖
	{	1.0f,	true	},//试炼
	{	0,		false	},
	{	0,		false	},
	{	0,		false	},
	{	0,		false	},
	{	0,		false	},
	{	0,		false	},
	{	0,		false	},
	{	0.06f,	true	},//寻宝
	{	0,		false	},
	{	0,		false	},
	{	0.0f,	false	},//跨服任务-->非库任务？Modified 2011-02-23
	{	0.1f,	true	},//天界平妖
	{	0.01f,	true	},//天界挑战
	{	1.0f,	true	},//天界试炼
	{	1.0f,	true	},//上古白字
	{	0.1f,	true	},//上古驴子
	{	0.01f,	true	},//上古红字
	
};

unsigned long g_ulNewCount = 0;
unsigned long g_ulDelCount = 0;

#define VERT_ZONE(pos)		(pos).x, (pos).y, (pos).z
#define VERT_ZONE_ADDR(pos)	&(pos).x, &(pos).y, &(pos).z

#ifdef _TASK_CLIENT

inline int marshal_str(char* data, const task_char* s)
{
	char* p = data;
	size_t len = s ? wcslen(s) : 0;

	*(int*)p = len;
	p += sizeof(int);

	if (len)
	{
		len *= sizeof(task_char);
		memcpy(p, s, len);
		p += len;
	}

	return p - data;
}

#endif

inline int unmarshal_str(const char* data, task_char*& s)
{
	const char* p = data;

	size_t len = *(int*)p;
	p += sizeof(int);

	s = new task_char[len + 1];
	g_ulNewCount++;
	s[len] = 0;

	if (len)
	{
		len *= sizeof(task_char);
		memcpy(s, p, len);
		p += len;
	}

	return p - data;
}

#ifdef _WINDOWS

	int marshal_option(const talk_proc::option* opt, char* data)
	{
		char* p = data;

		*(int*)p = opt->id;
		p += sizeof(int);

		*(int*)p = opt->param;
		p += sizeof(int);

		size_t sz = wcslen(opt->text) * sizeof(namechar);
		*(int*)p = sz;
		p += sizeof(int);

		if (sz)
		{
			memcpy(p, opt->text, sz);
			p += sz;
		}

		return p - data;
	}

	int marshal_window(const talk_proc::window* win, char* data)
	{
		char* p = data;

		*p = (char)win->id;
		p++;

		*p = (char)win->id_parent;
		p++;

		*(int*)p = win->talk_text_len;
		p += sizeof(int);

		if (win->talk_text_len)
		{
			size_t sz = sizeof(namechar) * win->talk_text_len;
			memcpy(p, win->talk_text, sz);
			p += sz;
		}

		*p = (char)win->num_option;
		p++;

		for (char i = 0; i < (char)win->num_option; i++)
			p += marshal_option(&win->options[i], p);

		return p - data;
	}

	int marshal_talk_proc(const talk_proc* talk, char* data)
	{
		char* p = data;

		size_t sz = wcslen(talk->text) * sizeof(namechar);
		*(int*)p = sz;
		p += sizeof(int);

		if (sz)
		{
			memcpy(p, talk->text, sz);
			p += sz;
		}

		*p = (char)talk->num_window;
		p++;

		for (char i = 0; i < (char)talk->num_window; i++)
			p += marshal_window(&talk->windows[i], p);

		return p - data;
	}

#endif

int unmarshal_option(talk_proc::option* opt, const char* data)
{
	const char* p = data;

	opt->id = *(int*)p;
	p += sizeof(int);

	opt->param = *(int*)p;
	p += sizeof(int);

	size_t sz = *(int*)p;
	p += sizeof(int);

	if (sz)
	{
		memcpy(opt->text, p, sz);
		p += sz;
	}

	return p - data;
}

int unmarshal_window(talk_proc::window* win, const char* data)
{
	const char* p = data;

	win->id = *p;
	p++;

	win->id_parent = *p;
	p++;

	win->talk_text_len = *(int*)p;
	p += sizeof(int);

	if (win->talk_text_len)
	{
		win->talk_text = new namechar[win->talk_text_len];
		size_t sz = sizeof(namechar) * win->talk_text_len;
		memcpy(win->talk_text, p, sz);
		p += sz;
		g_ulNewCount++;
	}

	win->num_option = *p;
	p++;

	if (win->num_option)
	{
		win->options = new talk_proc::option[win->num_option];
		memset(win->options, 0, sizeof(talk_proc::option) * win->num_option);
		g_ulNewCount++;

		for (int i = 0; i < win->num_option; i++)
			p += unmarshal_option(&win->options[i], p);
	}

	return p - data;
}

int unmarshal_talk_proc(talk_proc* talk, const char* data)
{
	const char* p = data;

	size_t sz = *(int*)p;
	p += sizeof(int);

	if (sz)
	{
		memcpy(talk->text, p, sz);
		p += sz;
	}

	talk->num_window = *p;
	p++;

	if (talk->num_window)
	{
		talk->windows = new talk_proc::window[talk->num_window];
		memset(talk->windows, 0, sizeof(talk_proc::window) * talk->num_window);
		g_ulNewCount++;

		for (int i = 0; i < talk->num_window; i++)
			p += unmarshal_window(&talk->windows[i], p);
	}

	return p - data;
}

static bool ReadLine(FILE* fp, char* szLine)
{
	if (fgets(szLine, TASK_MAX_LINE_LEN, fp) == NULL) return false;
	szLine[strcspn(szLine, "\r\n")] = '\0';
	return true;
}

inline bool LoadAwardCand(FILE* fp, AWARD_ITEMS_CAND& ic, unsigned long ulVersion)
{
	char line[TASK_MAX_LINE_LEN];
	unsigned long i;
	int nRead;

	ic.m_bRandChoose = false;
	if (ulVersion >= 26)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_rand_one, &nRead);
		ic.m_bRandChoose = (nRead != 0);
	}

	ic.m_ulAwardItems = 0;
	ic.m_ulAwardCmnItems = 0;
	ic.m_ulAwardTskItems = 0;

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_item_wanted_num, &ic.m_ulAwardItems);

	if (ic.m_ulAwardItems)
	{
#ifdef TASK_TEMPL_EDITOR
		ic.m_AwardItems = new ITEM_WANTED[MAX_ITEM_AWARD];
		memset(ic.m_AwardItems, 0, sizeof(ITEM_WANTED) * MAX_ITEM_AWARD);
#else
		ic.m_AwardItems = new ITEM_WANTED[ic.m_ulAwardItems];
#endif
	}

	for (i = 0; i < ic.m_ulAwardItems; i++)
	{
		ITEM_WANTED& iw = ic.m_AwardItems[i];
		int nBool = 0;

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_item_templ, &iw.m_ulItemTemplId);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_item_is_cmn, &nBool);
		iw.m_bCommonItem = (nBool != 0);

		if (nBool) ic.m_ulAwardCmnItems++;
		else ic.m_ulAwardTskItems++;

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_item_num, &iw.m_ulItemNum);

		if (ulVersion >= 8)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_prob, &iw.m_fProb);
		}
		else
			iw.m_fProb = 1.0f;

		iw.m_bBind = false;

		if (ulVersion >= 61)
		{
			nBool = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_bind, &nBool);
			iw.m_bBind = (nBool != 0);
		}

		iw.m_lPeriod = 0;

		if (ulVersion >= 67)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_period, &iw.m_lPeriod);
		}

		if (ulVersion >= 74)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_time_point, &nBool);
			iw.m_bTimetable = (nBool != 0);

			int w, h, m;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_tp_week, &w, &h, &m);
			iw.m_cDayOfWeek = w;
			iw.m_cHour = h;
			iw.m_cMinute = m; 
		}
		else
		{
			iw.m_bTimetable = false;
			iw.m_cDayOfWeek = 0;
			iw.m_cHour = 0;
			iw.m_cMinute = 0; 
		}

		if(ulVersion >= 125)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_item_refine_cond, &iw.m_cRefineCond);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_item_refine_level, &iw.m_ulRefineLevel);
		}
		else
		{
			iw.m_cRefineCond = -1;
			iw.m_ulRefineLevel = 0;
		}
		



	}

	return true;
}

inline bool LoadAwardCandBin(FILE* fp, AWARD_ITEMS_CAND& ic, unsigned long ulVersion)
{
	unsigned long i;

	ic.m_ulAwardItems = 0;
	ic.m_ulAwardCmnItems = 0;
	ic.m_ulAwardTskItems = 0;

	fread(&ic.m_bRandChoose, sizeof(ic.m_bRandChoose), 1, fp);
	fread(&ic.m_ulAwardItems, sizeof(ic.m_ulAwardItems), 1, fp);

	LOG_DELETE_ARR(ic.m_AwardItems);
	ic.m_AwardItems = NULL;

	if (ic.m_ulAwardItems)
	{
#ifdef TASK_TEMPL_EDITOR
		ic.m_AwardItems = new ITEM_WANTED[MAX_ITEM_AWARD];
#else
		ic.m_AwardItems = new ITEM_WANTED[ic.m_ulAwardItems];
#endif
	}

	for (i = 0; i < ic.m_ulAwardItems; i++)
	{
		ITEM_WANTED& iw = ic.m_AwardItems[i];
		fread(&iw, sizeof(iw), 1, fp);

		if (iw.m_bCommonItem) ic.m_ulAwardCmnItems++;
		else ic.m_ulAwardTskItems++;
	}

	return true;
}

inline bool LoadAwardData(FILE* fp, AWARD_DATA& ad, unsigned long ulVersion)
{
	char line[TASK_MAX_LINE_LEN];
	unsigned long i;

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_award_gold_num, &ad.m_ulGoldNum);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_award_exp_num, &ad.m_ulExp);

	if (ulVersion < 7)
		ad.m_ulCandItems = 1;
	else
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_cands_num, &ad.m_ulCandItems);
	}

	if (ad.m_ulCandItems)
	{
#ifdef TASK_TEMPL_EDITOR
		ad.m_CandItems = new AWARD_ITEMS_CAND[MAX_AWARD_CANDIDATES];
		memset(ad.m_CandItems, 0, sizeof(AWARD_ITEMS_CAND) * MAX_AWARD_CANDIDATES);
#else
		ad.m_CandItems = new AWARD_ITEMS_CAND[ad.m_ulCandItems];
#endif
	}

	for (i = 0; i < ad.m_ulCandItems; i++)
		LoadAwardCand(
			fp,
			ad.m_CandItems[i],
			ulVersion);
		
	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_award_sp_num, &ad.m_ulSP);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_award_repu, &ad.m_ulReputation);

	if (ulVersion >= 55)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_title, &ad.m_lTitle);
	}

	if (ulVersion >= 64)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_contrib, &ad.m_lContribution);
	}

	if(ulVersion >= 162)
	{
		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_faction_gold_note, &ad.m_iFactionGoldNote);

		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_faction_grass, &ad.m_iFactionGrass);

		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_faction_mine, &ad.m_iFactionMine);

		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_faction_monster_core, &ad.m_iFactionMonsterCore);

		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_faction_monster_food, &ad.m_iFactionMonsterFood);

		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_faction_money, &ad.m_iFactionMoney);

		int nBool = 0;
		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_faction_building_progress, &nBool);
		ad.m_bBuildingProgress = (nBool != 0);

		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_faction_extra_gold_note, &ad.m_iFactionExtraGoldNote);

		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_faction_extra_grass, &ad.m_iFactionExtraGrass);
		
		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_faction_extra_mine, &ad.m_iFactionExtraMine);

		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_faction_extra_monster_core, &ad.m_iFactionExtraMonsterCore);

		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_faction_extra_monster_food, &ad.m_iFactionExtraMonsterFood);

		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_faction_extra_money, &ad.m_iFactionExtraMoney);
	}

	if(ulVersion >= 164)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_travel_item, &ad.m_iTravelItemId);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_travel_time, &ad.m_iTravelTime);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_travel_speed, &ad.m_fTravelSpeed);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_travel_path, &ad.m_iTravelPath);
	}

	if (ulVersion >= 81)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_contrib, &ad.m_nFamContrib);
	}

	if (ulVersion >= 65)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_prosp, &ad.m_ulProsperity);
	}

	if (ulVersion >= 59)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_pk_value, &ad.m_lPKValue);

		int nBool = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_reset_pk, &nBool);
		ad.m_bResetPKValue = (nBool != 0);
	}

	if (ulVersion >= 66)
	{
		int nBool = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_divorce, &nBool);
		ad.m_bDivorce = (nBool != 0);
	}

	if (ulVersion >= 53)
	{
		size_t c;

		if (ulVersion < 103)
		{
			c = 10;
		}
		else if (ulVersion < 123)
		{
			c = 16;
		}
		else
		{
			c = TASK_ZONE_FRIENDSHIP_COUNT;
		}

		for (i = 0; i < c; i++)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_friendship, &ad.m_aFriendships[i]);
		}
	}

	if (ulVersion >= 106)
	{
		if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_friendship_reset, &ad.m_ulFriendshipResetSel);
	}

	if (ulVersion >= 25)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_period, &ad.m_ulNewPeriod);
	}

	if (ulVersion >= 33)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_station, &ad.m_ulNewRelayStation);
	}

	if (ulVersion >= 36)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_store, &ad.m_ulStorehouseSize);
	}

	if (ulVersion >= 70)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_store, &ad.m_ulFactionStorehouseSize);
	}

	if (ulVersion >= 56)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_invent, &ad.m_lInventorySize);
	}

	if (ulVersion >= 119)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_pocket, &ad.m_lPocketSize);
	}

	if (ulVersion >= 45)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_pet, &ad.m_ulPetInventorySize);
	}

	if (ulVersion >= 147)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_mount, &ad.m_ulMountInventorySize);
	}

	if (ulVersion >= 37)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_fury, &ad.m_ulFuryULimit);
	}

	if (ulVersion >= 49)
	{
		int nBool;

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_skill_lev, &nBool);
		ad.m_bSetProduceSkill = (nBool != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_skill_exp, &ad.m_ulProduceSkillExp);
	}
	else
	{
		ad.m_bSetProduceSkill = false;
		ad.m_ulProduceSkillExp = 0;
	}

	if (ulVersion >= 50)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_prof, &ad.m_ulNewProfession);
	}
	else
		ad.m_ulNewProfession = 0;

	if (ulVersion >= 110)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_lifeag_cnt, &ad.m_ulLifeAgainCnt);
	}

	if (ulVersion >= 95)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_lifeag_pro, &ad.m_ulLifeAgainFaction);
	}

	if (ulVersion >= 38)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_world_id, &ad.m_ulTransWldId);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(ad.m_TransPt));
	}

	if (ulVersion >= 68)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_controller, &ad.m_lMonsCtrl);

		int nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_trig_flag, &nRead);
		ad.m_bTrigCtrl = (nRead != 0);
	}

	if (ulVersion >= 123)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_ctrl_cnt, &ad.m_ulMonCtrlCnt);

		for (i=0; i<ad.m_ulMonCtrlCnt; i++)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_ctrl_id, &ad.m_MonCtrl[i].lMonCtrlID);			

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_ctrl_prob, &ad.m_MonCtrl[i].fGetProb);

			int nRead = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_ctrl_open, &nRead);
			ad.m_MonCtrl[i].bOpenClose = (nRead != 0);
		}
		
		int nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_ctrl_rand, &nRead);
		ad.m_bRanMonCtrl = (nRead != 0);
	}	

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_award_new_task, &ad.m_ulNewTask);

	if(ulVersion >= 132)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_ter_task_cnt, &ad.m_ulTerminateTaskCnt);
		
		for (i=0; i<ad.m_ulTerminateTaskCnt; i++)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_ter_task_id, &ad.m_ulTerminateTask[i]);
		}	

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_cam_move, &ad.m_ulCameraMove);
	}

	if(ulVersion >= 152)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_animation, &ad.m_ulAnimation);
	}

	if (ulVersion >= 132)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_circle_group_pt, &ad.m_ulCircleGroupPoint);
	}

	if (ulVersion >= 75)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_buffid, &ad.m_lBuffId);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_bufflev, &ad.m_lBuffLev);
	}

	if (ulVersion >= 79)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_f_skill_prof, &ad.m_nFamilySkillProficiency);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_f_skill_lev, &ad.m_nFamilySkillLevel);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_f_skill_index, &ad.m_nFamilySkillIndex);
	}

	if (ulVersion >= 82)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_record_index, &ad.m_nFamilyMonRecordIndex);
	}

	if (ulVersion >= 84)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_f_value_index, &ad.m_nFamilyValueIndex);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_f_value, &ad.m_nFamilyValue);
	}

	if (ulVersion >= 88)
	{
		int nBool;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_msg, &nBool);
		ad.m_bSendMsg = (nBool != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_channel, &ad.m_nMsgChannel);
	}

	if (ulVersion >= 89)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_id, &ad.m_ulClearCountTask);
	}

	if (ulVersion >= 93)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_double_exp_time, &ad.m_ulDoubleExpTime);
	}

	if (ulVersion >= 153)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_special_award_type, &ad.m_ulSpecialAwardType);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_special_award_id, &ad.m_ulSpecialAwardID);
	}

	if (ulVersion >= 103)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_m_moral, &ad.m_lMasterMoral);
		
		int nBool = 0;
		if (ulVersion >= 109)
		{			
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_out, &nBool);
			ad.m_bOutMaster = (nBool != 0);
		}

		nBool=0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_deviate, &nBool);
		ad.m_bDeviateMaster = (nBool != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_role, &nBool);
		ad.m_bAwardSpecifyRole = (nBool != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_role_Sel, &ad.m_ulRoleSelected);

		if (ad.m_bAwardSpecifyRole && ad.m_ulRoleSelected)
		{
			ad.m_pAwardSpecifyRole = new AWARD_DATA;
			
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_gold_num, &ad.m_pAwardSpecifyRole->m_ulGoldNum);
			
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_exp_num, &ad.m_pAwardSpecifyRole->m_ulExp);
			
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_sp_num, &ad.m_pAwardSpecifyRole->m_ulSP);
			
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_repu, &ad.m_pAwardSpecifyRole->m_ulReputation);
			
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_m_moral, &ad.m_pAwardSpecifyRole->m_lMasterMoral);

			if (ulVersion >= 108)
			{
				int nBool = 0;
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_award_m_moral, &nBool);
				ad.m_pAwardSpecifyRole->m_bOutMasterMoral = (nBool != 0);
			}
			
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_title, &ad.m_pAwardSpecifyRole->m_lTitle);
			
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_cands_num, &ad.m_pAwardSpecifyRole->m_ulCandItems);			
			
			if (ad.m_pAwardSpecifyRole->m_ulCandItems)
			{
				ad.m_pAwardSpecifyRole->m_CandItems = new AWARD_ITEMS_CAND[MAX_AWARD_CANDIDATES];
				memset(ad.m_pAwardSpecifyRole->m_CandItems, 0, sizeof(AWARD_ITEMS_CAND) * MAX_AWARD_CANDIDATES);
			}
			
			for (i = 0; i < ad.m_pAwardSpecifyRole->m_ulCandItems; i++)
				LoadAwardCand(
				fp,
				ad.m_pAwardSpecifyRole->m_CandItems[i],
				ulVersion);
		}
	}

	if (ulVersion >= 106)
	{
		if (ulVersion >= 114)
		{			
			int nBool = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_set_cult, &nBool);
			ad.m_bSetCult = (nBool != 0);
		}

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_set_cult, &ad.m_ulSetCult);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_clearskill, &ad.m_ulClearCultSkill);
	}

	if (ulVersion >= 107)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_exp_coef, &ad.m_fExpCoef);	
		
		if (ulVersion >= 111)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_exp_coef, &ad.m_fExpCoef2);
			
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_exp_coef, &ad.m_fExpCoef3);
		}

		int nBool;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_o_m_exp, &nBool);
		ad.m_bOutMasterExp = (nBool != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_o_m_moral, &nBool);
		ad.m_bOutMasterMoral = (nBool != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_cl_sk_p, &nBool);
		ad.m_bClearSkillPoints = (nBool != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_cl_bo_p, &nBool);
		ad.m_bClearBookPoints = (nBool != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_exp_sel, &ad.m_nParaExpSel);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_exp_size, &ad.m_ulParaExpSize);
		
		if (ad.m_ulParaExpSize)
		{
			ad.m_pszParaExp = new char[MAX_TASK_EXP_LEN];
			memset(ad.m_pszParaExp, 0, MAX_TASK_EXP_LEN);
			
			fread(ad.m_pszParaExp, 1, MAX_TASK_EXP_LEN, fp);
			if (!ReadLine(fp, line)) return false;
		}
	}

	if (ulVersion >= 117)
	{		
		int nChangeKeySize = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_change_key_size, &nChangeKeySize);	

		for (int i=0; i<nChangeKeySize; i++)
		{
			long lChangeKey = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_change_key, &lChangeKey);
			ad.m_lChangeKeyArr.push_back(lChangeKey);

			long lChangeKeyValue = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_change_kv, &lChangeKeyValue);
			ad.m_lChangeKeyValueArr.push_back(lChangeKeyValue);

			int nBool = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_change_typ, &nBool);
			bool bChangeType = (nBool != 0);
			ad.m_bChangeTypeArr.push_back(bChangeType);
		}
		
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_multi_global_key, &ad.m_lMultiGlobalKey);	
	}

	if (ulVersion >= 120)
	{				
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_battle_score, &ad.m_nSJBattleScore);
		
		int nBool = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_reset_sj, &nBool);
		ad.m_bResetSJBattleScore = (nBool != 0);
	}

	if (ulVersion >= 121)
	{
		int nBool = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_clear_nkat, &nBool);
		ad.m_bClearNoKeyActiveTask = (nBool != 0);
	}

	if (ulVersion >= 124)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_transform_id, &ad.m_iTransformID);
		
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_transform_dur, &ad.m_iTransformDuration);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_transform_lev, &ad.m_iTransformLevel);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_transform_lev, &ad.m_iTransformExpLevel);
	}

	if(ulVersion >= 125)
	{
		int nBool = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_transform_cover, &nBool);
		ad.m_bTransformCover = (nBool != 0);
	}

	if(ulVersion >= 138)
	{
		int nBool = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_fengshen_exp, &ad.m_ulFengshenExp);
		
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_fengshen_dujie, &nBool);
		ad.m_bFengshenDujie = (nBool != 0);
	}

	if(ulVersion >= 145)	//Added 2012-04-18.
	{
		int nBool = 0;
		if ( !ReadLine(fp, line) )
		{
			return false;
		}
		sscanf(line, _format_award_open_soul_equip, &nBool);
		ad.m_bOpenSoulEquip = ( nBool != 0 );
	}

	if( ulVersion >= 139 )//Added 2011-02-18.
	{
		if( !ReadLine(fp, line) )
		{
			return false;
		}
		sscanf(line, _format_award_bonus_num, &ad.m_ulBonusNum);

		if( !ReadLine(fp, line) )
		{
			return false;
		}
		sscanf(line, _format_award_battle_field_reputaion, &ad.m_ulBattleScore);
	}

	//Added 2011-03-08.
	if( ulVersion >= 140 )
	{
		int nBool = 0;
		if( !ReadLine(fp, line) )
		{
			return false;
		}
		sscanf(line, _format_award_check_fnsh_global_count, &nBool);
		ad.m_bCheckGlobalFinishCount = ( nBool != 0 );
		
		if( !ReadLine(fp, line) )
		{
			return false;
		}
		sscanf(line, _format_award_global_fnsh_count_precondition, &ad.m_ulGlobalFinishCountPrecondition);	

		if( !ReadLine(fp, line) )
		{
			return false;
		}
		sscanf(line, _format_award_check_global_comp_expression, &nBool);
		ad.m_bCheckGlobalCompareExpression = ( nBool != 0 );

		if( ad.m_bCheckGlobalCompareExpression )
		{
			char szExpLeft[MAX_TASK_EXP_LEN]={0};
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_left_string, szExpLeft);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_comp_oper, &ad.m_GlobalCompareExpression.nCompOper);

			char szExpRight[MAX_TASK_EXP_LEN]={0};
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_right_string, szExpRight);
			
			int i=0;
			do 
			{
				ad.m_GlobalCompareExpression.strExpLeft.push_back(szExpLeft[i]);
				
			} while(szExpLeft[i++]);	
			
			i=0;
			do 
			{
				ad.m_GlobalCompareExpression.strExpRight.push_back(szExpRight[i]);
				
			} while(szExpRight[i++]);
		}
	}

	if(ulVersion >= 162)
	{
		if(!ReadLine(fp, line))
		{
			return false;
		}
		sscanf(line, _format_award_faction_extra_cands_num, &ad.m_ulFactionExtraCandItems);
		if(ad.m_ulFactionExtraCandItems)
		{
#ifdef TASK_TEMPL_EDITOR
			ad.m_FactionExtraCandItems = new AWARD_ITEMS_CAND[MAX_AWARD_CANDIDATES];
			memset(ad.m_FactionExtraCandItems, 0, sizeof(AWARD_ITEMS_CAND) * MAX_AWARD_CANDIDATES);
#else
			ad.m_FactionExtraCandItems = new AWARD_ITEMS_CAND[ad.m_ulFactionExtraCandItems];
#endif
		}
		
		for(i = 0; i < ad.m_ulFactionExtraCandItems; i++)
		{
			LoadAwardCand(fp, ad.m_FactionExtraCandItems[i], ulVersion);
		}
	}

	if( ulVersion >= 140 )//Added 2011-03-04.
	{
		if( !ReadLine(fp, line) )
		{
			return false;
		}
		sscanf(line, _format_award_extra_cands_num, &ad.m_ulExtraCandItems);

		if (ad.m_ulExtraCandItems)
		{
#ifdef TASK_TEMPL_EDITOR
			ad.m_ExtraCandItems = new AWARD_ITEMS_CAND[MAX_AWARD_CANDIDATES];
			memset(ad.m_ExtraCandItems, 0, sizeof(AWARD_ITEMS_CAND) * MAX_AWARD_CANDIDATES);
#else
			ad.m_ExtraCandItems = new AWARD_ITEMS_CAND[ad.m_ulExtraCandItems];
#endif
		}

		for (i = 0; i < ad.m_ulExtraCandItems; i++)
		{
			LoadAwardCand(
				fp,
				ad.m_ExtraCandItems[i],
				ulVersion);
		}
	}

	if (ulVersion >= 140)//Added 2011-03-07.
	{
		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_extra_ctrl_cnt, &ad.m_ulExtraMonCtrlCnt);

		for (i=0; i<ad.m_ulExtraMonCtrlCnt; i++)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_extra_ctrl_id, &ad.m_ExtraMonCtrl[i].lMonCtrlID);			

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_extra_ctrl_prob, &ad.m_ExtraMonCtrl[i].fGetProb);

			int nRead = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_extra_ctrl_open, &nRead);
			ad.m_ExtraMonCtrl[i].bOpenClose = (nRead != 0);
		}
		
		int nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_extra_ctrl_rand, &nRead);
		ad.m_bExtraRanMonCtrl = (nRead != 0);
	}

	if (ulVersion >= 140)//Added 2011-03-07.
	{
		int nBool;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_msg, &nBool);
		ad.m_bSendExtraMsg = (nBool != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_channel, &ad.m_nExtraMsgChannel);

		size_t len;
		char line[TASK_MAX_LINE_LEN];

		len = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_tribute_len, &len);

		ad.m_pwstrExtraTribute = new task_char[len+1];
		if( !ad.m_pwstrExtraTribute )
		{
			return false;
		}
		ad.m_pwstrExtraTribute[len] = L'\0';
		g_ulNewCount++;

		if (len)
		{
			fread(ad.m_pwstrExtraTribute, sizeof(task_char), len, fp);
			ReadLine(fp, line);
		}

		//Added 2011-03-30
		int i = 0;
		for( ; i < MAX_VARIABLE_NUM; ++i )
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_variable, &ad.m_lVariable[i]);
		}
	}

	if (ulVersion >= 155)
	{
		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_king_score, &ad.m_nKingScore);
	}

	if (ulVersion >= 158)
	{
		int nBool = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_clear_xp_cd, &nBool);
		ad.m_bClearXpCD = (nBool != 0);
	}

	if(ulVersion >= 161)
	{
		if(!ReadLine(fp,line)) return false;
		sscanf(line, _format_award_Phase_cnt, &ad.m_iPhaseCnt);

		for(i = 0; i < ad.m_iPhaseCnt; i++)
		{
			if(!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_Phase_id, &ad.m_Phase[i].iPhaseID);

			int nRead = 0;
			if(!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_Phase_open, &nRead);
			ad.m_Phase[i].bOpenClose = (nRead != 0);
		}

	}


	if(ulVersion >= 164)
	{
		if(!ReadLine(fp,line)) return false;
		sscanf(line, _format_award_auction_cnt, &ad.m_iAuctionCnt);

		for(i = 0; i < ad.m_iAuctionCnt; i++)
		{
			if(!ReadLine(fp,line)) return false;
			sscanf(line, _format_award_auction_id, &ad.m_Auction[i].m_ulAuctionItemId);

			if(!ReadLine(fp,line)) return false;
			sscanf(line, _format_award_auction_prob, &ad.m_Auction[i].m_fAuctionProp);
		}
	}
	

	return true;
}

inline bool LoadAwardDataBin(FILE* fp, AWARD_DATA& ad, unsigned long ulVersion)
{
	unsigned long i;

	LOG_DELETE_ARR(ad.m_CandItems);
	ad.m_CandItems = NULL;

	LOG_DELETE(ad.m_pAwardSpecifyRole);
	ad.m_pAwardSpecifyRole = NULL;

	LOG_DELETE_ARR(ad.m_pszParaExp);
	ad.m_pszParaExp = NULL;
	
	LOG_DELETE_ARR(ad.m_pParaExpArr);
	ad.m_pParaExpArr = NULL;

	LOG_DELETE_ARR(ad.m_FactionExtraCandItems);
	ad.m_FactionExtraCandItems = NULL;

	//Added 2011-03-04.
	LOG_DELETE_ARR(ad.m_ExtraCandItems);
	ad.m_ExtraCandItems = NULL;	

	//Added 2011-03-07.
	LOG_DELETE_ARR(ad.m_pwstrExtraTribute);
	ad.m_pwstrExtraTribute = NULL;	

	fread(&ad, sizeof(ad), 1, fp);

	if (ad.m_ulCandItems)
	{
#ifdef TASK_TEMPL_EDITOR
		ad.m_CandItems = new AWARD_ITEMS_CAND[MAX_AWARD_CANDIDATES];
		memset(ad.m_CandItems, 0, sizeof(AWARD_ITEMS_CAND) * MAX_AWARD_CANDIDATES);
#else
		ad.m_CandItems = new AWARD_ITEMS_CAND[ad.m_ulCandItems];
#endif
	}
	else
		ad.m_CandItems = NULL;

	for (i = 0; i < ad.m_ulCandItems; i++)
		LoadAwardCandBin(
			fp,
			ad.m_CandItems[i],
			ulVersion);

	if (ad.m_bAwardSpecifyRole && ad.m_ulRoleSelected)
	{
		ad.m_pAwardSpecifyRole = new AWARD_DATA;
		LoadAwardDataBin(fp, *(ad.m_pAwardSpecifyRole), ulVersion);
	}

	if (ad.m_ulParaExpSize)
	{
#ifdef TASK_TEMPL_EDITOR
		ad.m_pszParaExp = new char[MAX_TASK_EXP_LEN];
		memset(ad.m_pszParaExp, 0, MAX_TASK_EXP_LEN);
#else
		ad.m_pszParaExp = new char[ad.m_ulParaExpSize];
#endif

		fread(ad.m_pszParaExp, 1, ad.m_ulParaExpSize, fp);
		
		fread(&ad.m_ulParaExpArrLen, sizeof(unsigned long), 1, fp);
		if (ad.m_ulParaExpArrLen)
		{
			ad.m_pParaExpArr = new TASK_EXPRESSION[ad.m_ulParaExpArrLen];
			g_ulNewCount++;
			
			fread(ad.m_pParaExpArr, sizeof(TASK_EXPRESSION), ad.m_ulParaExpArrLen, fp);
		}
	}

	if (ad.m_lChangeKeyArr.size())
	{
		size_t nArrSize = ad.m_lChangeKeyArr.size();
		
		long lChangeKeyArr[TASK_AWARD_MAX_CHANGE_VALUE]={0};
		long lChangeKeyValueArr[TASK_AWARD_MAX_CHANGE_VALUE]={0};
		bool bChangeTypeArr[TASK_AWARD_MAX_CHANGE_VALUE]={0};
		fread(lChangeKeyArr, sizeof(long), nArrSize, fp);
		fread(lChangeKeyValueArr, sizeof(long), nArrSize, fp);
		fread(bChangeTypeArr, sizeof(bool), nArrSize, fp);
		
		memset(&ad.m_lChangeKeyArr, 0, sizeof(ad.m_lChangeKeyArr));
		memset(&ad.m_lChangeKeyValueArr, 0, sizeof(ad.m_lChangeKeyArr));
		memset(&ad.m_bChangeTypeArr, 0, sizeof(ad.m_lChangeKeyArr));
		for (i=0; i<nArrSize; i++)
		{
			ad.m_lChangeKeyArr.push_back(lChangeKeyArr[i]);
			ad.m_lChangeKeyValueArr.push_back(lChangeKeyValueArr[i]);
			ad.m_bChangeTypeArr.push_back(bChangeTypeArr[i]);
		}
	}
	
	//Added 2011-03-08.
	if(ad.m_bCheckGlobalCompareExpression)//需要保存全局key/value表达式
	{
		char szExpLeft[MAX_TASK_EXP_LEN] ={0};
		char szExpRight[MAX_TASK_EXP_LEN]={0};
		
		size_t nstrExpLeftLen  = ad.m_GlobalCompareExpression.strExpLeft.size();
		size_t nstrExpRightLen = ad.m_GlobalCompareExpression.strExpRight.size();
		
		fread(szExpLeft, sizeof(char), nstrExpLeftLen, fp);
		fread(szExpRight, sizeof(char), nstrExpRightLen, fp);		
		
		size_t i;
		memset(&ad.m_GlobalCompareExpression.strExpLeft, 0, sizeof(ad.m_GlobalCompareExpression.strExpLeft));
		for(i=0; i<nstrExpLeftLen+1; i++)
		{
			ad.m_GlobalCompareExpression.strExpLeft.push_back(szExpLeft[i]);
		}

		memset(&ad.m_GlobalCompareExpression.strExpRight, 0, sizeof(ad.m_GlobalCompareExpression.strExpRight));
		for (i=0; i<nstrExpRightLen+1; i++)
		{
			ad.m_GlobalCompareExpression.strExpRight.push_back(szExpRight[i]);
		}

		TaskExpAnalyser Analyser;
		
		memset(&ad.m_GlobalCompareExpression.arrExpLeft, 0, sizeof(ad.m_GlobalCompareExpression.arrExpLeft));	
		Analyser.Analyse(&ad.m_GlobalCompareExpression.strExpLeft[0], ad.m_GlobalCompareExpression.arrExpLeft);	
		
		memset(&ad.m_GlobalCompareExpression.arrExpRight, 0, sizeof(ad.m_GlobalCompareExpression.arrExpRight));					
		Analyser.Analyse(&ad.m_GlobalCompareExpression.strExpRight[0], ad.m_GlobalCompareExpression.arrExpRight);
	}

	if(ad.m_ulFactionExtraCandItems)
	{
#ifdef TASK_TEMPL_EDITOR
		ad.m_FactionExtraCandItems = new AWARD_ITEMS_CAND[MAX_AWARD_CANDIDATES];
		memset(ad.m_FactionExtraCandItems, 0, sizeof(AWARD_ITEMS_CAND) * MAX_AWARD_CANDIDATES);
#else
		ad.m_FactionExtraCandItems = new AWARD_ITEMS_CAND[ad.m_ulFactionExtraCandItems];
#endif
	}
	else
	{
		ad.m_FactionExtraCandItems = NULL;
	}

	for(i = 0; i < ad.m_ulFactionExtraCandItems; i++)
	{
		LoadAwardCandBin(fp, ad.m_FactionExtraCandItems[i], ulVersion);
	}

	//Added 2011-03-04.
	//读取额外奖励物品数据
	if (ad.m_ulExtraCandItems)
	{
#ifdef TASK_TEMPL_EDITOR
		ad.m_ExtraCandItems = new AWARD_ITEMS_CAND[MAX_AWARD_CANDIDATES];
		memset(ad.m_ExtraCandItems, 0, sizeof(AWARD_ITEMS_CAND) * MAX_AWARD_CANDIDATES);
#else
		ad.m_ExtraCandItems = new AWARD_ITEMS_CAND[ad.m_ulExtraCandItems];
#endif
	}
	else
	{
		ad.m_ExtraCandItems = NULL;
	}

	for (i = 0; i < ad.m_ulExtraCandItems; i++)
		LoadAwardCandBin(
			fp,
			ad.m_ExtraCandItems[i],
			ulVersion);

	//Added 2011-03-07.
	//读取额外奖励中的系统喊话内容
	size_t len;	
	len = 0;
	fread(&len, sizeof(len), 1, fp);
	ad.m_pwstrExtraTribute = new task_char[len+1];
#ifdef _TASK_CLIENT
	ad.m_pwstrExtraTribute[len] = L'\0';
#else
	ad.m_pwstrExtraTribute[len] = 0;//主要是供服务器使用，防止服务器将L"\0"扩展成4字节的wchar_t。
#endif
	if(len)
	{
		fread(ad.m_pwstrExtraTribute, sizeof(task_char), len, fp);
	}

	return true;
}

inline bool LoadAwardDataRatioScale(FILE* fp, AWARD_RATIO_SCALE& ad, unsigned long ulVersion)
{
	fread(&ad.m_ulScales, sizeof(ad.m_ulScales), 1, fp);
	fread(ad.m_Ratios, sizeof(ad.m_Ratios), 1, fp);

	LOG_DELETE_ARR(ad.m_Awards);
	ad.m_Awards = NULL;

	if (ad.m_ulScales)
	{
#ifdef TASK_TEMPL_EDITOR
		ad.m_Awards = new AWARD_DATA[MAX_AWARD_SCALES];
		memset(ad.m_Awards, 0, sizeof(AWARD_DATA) * MAX_AWARD_SCALES);
#else
		ad.m_Awards = new AWARD_DATA[ad.m_ulScales];
#endif
	}

	for (unsigned long i = 0; i < ad.m_ulScales; i++)
		LoadAwardDataBin(fp, ad.m_Awards[i], ulVersion);

	return true;
}

inline bool LoadAwardDataItemsScale(FILE* fp, AWARD_ITEMS_SCALE& ad, unsigned long ulVersion)
{
	fread(&ad.m_ulScales, sizeof(ad.m_ulScales), 1, fp);
	fread(&ad.m_ulItemId, sizeof(ad.m_ulItemId), 1, fp);
	fread(ad.m_Counts, sizeof(ad.m_Counts), 1, fp);

	LOG_DELETE_ARR(ad.m_Awards);
	ad.m_Awards = NULL;

	if (ad.m_ulScales)
	{
#ifdef TASK_TEMPL_EDITOR
		ad.m_Awards = new AWARD_DATA[MAX_AWARD_SCALES];
		memset(ad.m_Awards, 0, sizeof(AWARD_DATA) * MAX_AWARD_SCALES);
#else
		ad.m_Awards = new AWARD_DATA[ad.m_ulScales];
#endif
	}

	for (unsigned long i = 0; i < ad.m_ulScales; i++)
		LoadAwardDataBin(fp, ad.m_Awards[i], ulVersion);

	return true;
}

inline bool LoadAwardDataCountScale(FILE* fp, AWARD_COUNT_SCALE& ad, unsigned long ulVersion)
{
	fread(&ad.m_ulScales, sizeof(ad.m_ulScales), 1, fp);
	fread(ad.m_Counts, sizeof(ad.m_Counts), 1, fp);

	delete[] ad.m_Awards;
	ad.m_Awards = NULL;

	if (ad.m_ulScales)
	{
#ifdef TASK_TEMPL_EDITOR
		ad.m_Awards = new AWARD_DATA[MAX_AWARD_SCALES];
		memset(ad.m_Awards, 0, sizeof(AWARD_DATA) * MAX_AWARD_SCALES);
#else
		ad.m_Awards = new AWARD_DATA[ad.m_ulScales];
#endif
	}

	for (unsigned long i = 0; i < ad.m_ulScales; i++)
		LoadAwardDataBin(fp, ad.m_Awards[i], ulVersion);

	return true;
}

bool ATaskTemplFixedData::LoadFixedDataFromTextFile(FILE* fp, unsigned long ulVersion)
{
	char line[TASK_MAX_LINE_LEN];
	int nRead;
	unsigned long i;

	/* 任务对象的属性 */

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_id, &m_ID);

	fread(m_szName, sizeof(task_char), MAX_TASK_NAME_LEN, fp);
	if (!ReadLine(fp, line)) return false;

	if (ulVersion >= 43)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_dyn, &nRead);
		m_DynTaskType = nRead;
	}

	if (ulVersion >= 41)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_type, &m_ulType);
	}

	if (ulVersion >= 40)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_has_sign, &nRead);
		m_bHasSign = (nRead != 0);

		if (m_bHasSign)
		{
			if (!m_pszSignature)
			{
				m_pszSignature = new task_char[MAX_TASK_NAME_LEN];
				g_ulNewCount++;
			}

			fread(m_pszSignature, sizeof(task_char), MAX_TASK_NAME_LEN, fp);
			if (!ReadLine(fp, line)) return false;
		}
	}

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_time_limit, &m_ulTimeLimit);

	if (ulVersion >= 72)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_abs_time, &nRead);
		m_bAbsTime = (nRead != 0);
	}

	if (ulVersion >= 54)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_avail_frequency, &m_lAvailFrequency);
	}

	if (ulVersion >= 71)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_time_interval, &m_lTimeInterval);
	}

	if (ulVersion >= 2)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_count, &m_ulTimetable);

		if (m_ulTimetable)
		{
#ifdef TASK_TEMPL_EDITOR
			m_tmStart	= new task_tm[MAX_TIMETABLE_SIZE];
			m_tmEnd		= new task_tm[MAX_TIMETABLE_SIZE];
			memset(m_tmStart, 0, sizeof(task_tm) * MAX_TIMETABLE_SIZE);
			memset(m_tmEnd, 0, sizeof(task_tm) * MAX_TIMETABLE_SIZE);
#else
			m_tmStart	= new task_tm[m_ulTimetable];
			m_tmEnd		= new task_tm[m_ulTimetable];
#endif
		}

		for (i = 0; i < m_ulTimetable; i++)
		{
			task_tm& s = m_tmStart[i];
			task_tm& e = m_tmEnd[i];

			if (ulVersion >= 47)
			{
				int n;

				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_tm_type, &n);
				m_tmType[i] = (char)n;

				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_full_tm, &s.year, &s.month, &s.day, &s.hour, &s.min, &s.wday);

				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_full_tm, &e.year, &e.month, &e.day, &e.hour, &e.min, &e.wday);
			}
			else
			{
				m_tmType[i] = (char)enumTaskTimeDate;

				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_full_tm_old, &s.year, &s.month, &s.day, &s.hour, &s.min);
				s.wday = 0;

				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_full_tm_old, &e.year, &e.month, &e.day, &e.hour, &e.min);
				e.wday = 0;
			}
		}
	}

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_child_in_order, &nRead);
	m_bExeChildInOrder = (nRead != 0);

	if (ulVersion >= 10)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_choose_one, &nRead);
		m_bChooseOne = (nRead != 0);
	}

	if (ulVersion >= 17)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_rand_one, &nRead);
		m_bRandOne = (nRead != 0);
	}

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_parent_fail, &nRead);
	m_bParentAlsoFail = (nRead != 0);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_parent_succ, &nRead);
	m_bParentAlsoSucc = (nRead != 0);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_can_give_up, &nRead);
	m_bCanGiveUp = (nRead != 0);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_die_as_fail, &nRead);
	m_bFailAsPlayerDie = (nRead != 0);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_can_redo, &nRead);
	m_bCanRedo = (nRead != 0);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_redo_after_fail, &nRead);
	m_bCanRedoAfterFailure = (nRead != 0);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_clear_as_giveup, &nRead);
	m_bClearAsGiveUp = (nRead != 0);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_need_record, &nRead);
	m_bNeedRecord = (nRead != 0);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_max_receiver, &m_ulMaxReceiver);

	if (ulVersion >= 116)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_clear_rev_type, &m_nClearReceiverType);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_clear_rev_t_i, &m_lClearReceiverTimeInterval);
	}

	if (ulVersion >= 11)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_must_in_zone, &nRead);
		m_bDelvInZone = (nRead != 0);

		if (ulVersion >= 35)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_world_id, &m_ulDelvWorld);
		}

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_DelvMinVert));

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_DelvMaxVert));
	}
	
	if (ulVersion >= 38)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_trans_to, &nRead);
		m_bTransTo = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_world_id, &m_ulTransWldId);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_TransPt));
	}


	if(ulVersion >= 161)
	{
		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_Phase_cnt, &m_iPhaseCnt);
		
		for(i = 0; i < m_iPhaseCnt; i++)
		{
			if(!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_Phase_id, &m_Phase[i].iPhaseID);

			int nRead = 0;
			if(!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_Phase_open, &nRead);
			m_Phase[i].bOpenClose = (nRead != 0);

			nRead = 0;
			if(!ReadLine(fp, line)) return false;
			sscanf(line, _format_prop_Phase_triger, &nRead);
			m_Phase[i].bTrigger = (nRead != 0);

			nRead = 0;
			if(!ReadLine(fp, line)) return false;
			sscanf(line, _format_prop_Phase_visual, &nRead);
			m_Phase[i].bVisual = (nRead != 0);
		}

	}

	if (ulVersion >= 69)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_controller, &m_lMonsCtrl);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_trig_flag, &nRead);
		m_bTrigCtrl = (nRead != 0);
	}
	
	if (ulVersion >= 123)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_ctrl_cnt, &m_ulMonCtrlCnt);
		
		for (i=0; i<m_ulMonCtrlCnt; i++)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_ctrl_id, &m_MonCtrl[i].lMonCtrlID);			
			
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_ctrl_prob, &m_MonCtrl[i].fGetProb);
			
			int nRead = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_award_ctrl_open, &nRead);
			m_MonCtrl[i].bOpenClose = (nRead != 0);
		}
		
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_award_ctrl_rand, &m_bRanMonCtrl);
	}

	if (ulVersion >= 16)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_auto_deliver, &nRead);
		m_bAutoDeliver = (nRead != 0);
	}

	if (ulVersion >= 127)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_auto_deliver_mode, &nRead);
		m_bDeliverWindowMode = (nRead != 0);
	}

	if (m_bDelvInZone) m_bAutoDeliver = true;

	if (ulVersion >= 20)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_death_trig, &nRead);
		m_bDeathTrig = (nRead != 0);
	}
	
	if (ulVersion >= 136)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_manual_trig, &nRead);
		m_bManualTrig = (nRead != 0);
	}

	if (ulVersion >= 127)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_key_task_must_show, &nRead);
		m_bMustShown = (nRead != 0);
	}

	if (ulVersion >= 24)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_clear_acquired, &nRead);
		m_bClearAcquired = (nRead != 0);
	}

	if (ulVersion >= 27)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_suit_lev, &m_ulSuitableLevel);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_show_prompt, &nRead);
		m_bShowPrompt = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_key_task, &nRead);
		m_bKeyTask = (nRead != 0);
	}

	if (ulVersion >= 29)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_npc, &m_ulDelvNPC);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_npc, &m_ulAwardNPC);
	}

	if (ulVersion >= 31)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_skill_task, &nRead);
		m_bSkillTask = (nRead != 0);
	}

	if (ulVersion >= 44)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_seek_out, &nRead);
		m_bCanSeekOut = (nRead != 0);
	}
	else
		m_bCanSeekOut = true;

	if (ulVersion >= 46)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_direction, &nRead);
		m_bShowDirection = (nRead != 0);
	}
	else
		m_bShowDirection = true;

	if (ulVersion >= 48)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_weight, &m_fStorageWeight);
	}

	if (ulVersion >= 51)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_rank, &m_ulRank);
	}

	if (ulVersion >= 62)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_marriage, &nRead);
		m_bMarriage = (nRead != 0);
	}

	if(ulVersion >= 162)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_faction_task, &nRead);
		m_bFaction = (nRead != 0);
	}

	if (ulVersion >= 80)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_shared_by_fam, &nRead);
		m_bSharedByFamily = (nRead != 0);
	}

	if (ulVersion >= 87)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_rec_finish_count, &nRead);
		m_bRecFinishCount = (nRead != 0);

		if (ulVersion >= 90)
		{
			nRead = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_rec_finish_count, &nRead);
			m_bRecFinishCountGlobal = (nRead != 0);
		}

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_finish_count, &m_ulMaxFinishCount);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_full_tm, &m_FinishClearTime.year, &m_FinishClearTime.month, &m_FinishClearTime.day, &m_FinishClearTime.hour, &m_FinishClearTime.min, &m_FinishClearTime.wday);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_type, &m_nFinishTimeType);
	}

	if (ulVersion >= 97)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_lifeagain_reset, &nRead);
		m_bLifeAgainReset = (nRead != 0);
	}

	if (ulVersion >= 98)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_fail_logout, &nRead);
		m_bFailAfterLogout = (nRead != 0);
	}

	if (ulVersion >= 134)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_logout_fail_time, &m_ulLogoutFailTime);
	}

	if (ulVersion >= 131)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_abs_fail_time, &nRead);
		m_bAbsFail = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_full_tm_old, &m_tmAbsFailTime.year, &m_tmAbsFailTime.month, &m_tmAbsFailTime.day, &m_tmAbsFailTime.hour, &m_tmAbsFailTime.min);
	}

	if (ulVersion >= 101)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prentice_task, &nRead);
		m_bPrenticeTask = (nRead != 0);
	}
	
	if (ulVersion >= 104)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_hidden, &nRead);
		m_bHidden = (nRead != 0);
	}

	if (ulVersion >= 105)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_out_zone_fail, &nRead);
		m_bOutZoneFail = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_world_id, &m_ulOutZoneWorldID);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_OutZoneMinVert));

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_OutZoneMaxVert));
	}

	if (ulVersion >= 131)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_enter_zone_fail, &nRead);
		m_bEnterZoneFail = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_world_id, &m_ulEnterZoneWorldID);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_EnterZoneMinVert));

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_EnterZoneMaxVert));
	}

	if (ulVersion >= 115)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_clear_illegal, &nRead);
		m_bClearSomeIllegalStates = (nRead != 0);
	}

	if (ulVersion >= 117)
	{		
		unsigned int nChangeKeySize = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_change_key_size, &nChangeKeySize);	

		for (i=0; i<nChangeKeySize; i++)
		{
			long lChangeKey = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_change_key, &lChangeKey);
			m_lChangeKeyArr.push_back(lChangeKey);

			long lChangeKeyValue = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_change_kv, &lChangeKeyValue);
			m_lChangeKeyValueArr.push_back(lChangeKeyValue);

			nRead = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_change_typ, &nRead);
			bool bChangeType = (nRead != 0);
			m_bChangeTypeArr.push_back(bChangeType);
		}
	}

	if (ulVersion >= 131)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_kill_monster_fail, &nRead);
		m_bKillMonsterFail = (nRead != 0);

		if(m_bKillMonsterFail)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_kill_monster_fail_cand, &m_ulKillFailMonster);

			for(i=0;i<m_ulKillFailMonster; i++)
			{
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_kill_monster_fail_id, &m_KillFailMonsters[i]);
			}
		}

		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_have_item_fail, &nRead);
		m_bHaveItemFail = (nRead != 0);

		if(m_bHaveItemFail)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_have_item_fail_cand, &m_ulHaveItemFail);

			nRead = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_have_item_fail_not_take_off, &nRead);
			m_bHaveItemFailNotTakeOff = (nRead!= 0);

			for(i=0;i<m_ulHaveItemFail; i++)
			{
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_have_item_fail_id, &m_HaveFailItems[i]);
			}
		}

		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_not_have_item_fail, &nRead);
		m_bNotHaveItemFail = (nRead != 0);

		if(m_bNotHaveItemFail)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_not_have_item_fail_cand, &m_ulNotHaveItemFail);

			for(i=0;i<m_ulNotHaveItemFail; i++)
			{
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_not_have_item_fail_id, &m_NotHaveFailItems[i]);
			}
		}
	}

	if(ulVersion >= 132)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_cam_move, &m_ulCameraMove);
	}

	if(ulVersion >= 152)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_animation, &m_ulAnimation);
	}

	if ( ulVersion >= 141 )// Added 2011-04-11
	{
		for ( int i = 0; i < MAX_VARIABLE_NUM; ++i )
		{
			if ( !ReadLine(fp, line) )
			{
				return false;
			}
			sscanf(line, _format_variables, &m_lVariables[i]);
		}
	}

	if (ulVersion >= 144)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_display_type, &m_ulDisplayType);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_recommend_type, &m_ulRecommendType);
	}

	if (ulVersion >= 153)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_tiny_game_id, &m_ulTinyGameID);
	}

	if (ulVersion >= 158)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_dyn_clear_time, &m_lDynFinishClearTime);

		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_clear_xp_cd, &nRead);
		m_bClearXpCD = (nRead != 0);
	}

	/* 任务的开启条件及方式 */

	if (ulVersion >= 42)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_special_award, &m_ulSpecialAward);
	}

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_prem_level, &m_ulPremise_Lev_Min);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_prem_level, &m_ulPremise_Lev_Max);

	if (ulVersion >= 85)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_talisman_value, &m_nTalismanValueMin);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_talisman_value, &m_nTalismanValueMax);
	}

	if (ulVersion >= 113)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_consume_treasure, &m_nConsumeTreasureMin);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_consume_treasure, &m_nConsumeTreasureMax);
	}

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_item_wanted_num, &m_ulPremItems);
	
	if (ulVersion >= 125)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_item_not_take_off, &nRead);
		m_bItemNotTakeOff = (nRead != 0);
	}

	if (m_ulPremItems)
	{
#ifdef TASK_TEMPL_EDITOR
		m_PremItems = new ITEM_WANTED[MAX_ITEM_WANTED];
		memset(m_PremItems, 0, sizeof(ITEM_WANTED) * MAX_ITEM_WANTED);
#else
		m_PremItems = new ITEM_WANTED[m_ulPremItems];
#endif
	}

	for (i = 0; i < m_ulPremItems; i++)
	{
		ITEM_WANTED& iw = m_PremItems[i];
		int nBool;

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_item_templ, &iw.m_ulItemTemplId);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_item_is_cmn, &nBool);
		iw.m_bCommonItem = (nBool != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_item_num, &iw.m_ulItemNum);

		if(ulVersion >= 125)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_item_refine_cond, &iw.m_cRefineCond);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_item_refine_level, &iw.m_ulRefineLevel);
		}
	}


	if(ulVersion >= 130)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_monster_summon_cand, &m_ulPremMonsterSummoned);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_monster_summon_mode, &m_ulSummonMode);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_rand_one, &nRead);
		m_bRandSelMonster = (nRead != 0);

		if (m_ulPremMonsterSummoned)
		{
#ifdef TASK_TEMPL_EDITOR
			m_MonsterSummoned = new MONSTER_SUMMONED[MAX_MONSTER_SUMMONED_CAND];
			memset(m_MonsterSummoned, 0, sizeof(MONSTER_SUMMONED) * MAX_MONSTER_SUMMONED_CAND);
#else
			m_MonsterSummoned = new MONSTER_SUMMONED[m_ulPremMonsterSummoned];
#endif
		}

		for( i=0; i < m_ulPremMonsterSummoned; i++)
		{
			MONSTER_SUMMONED& ms = m_MonsterSummoned[i];
			int nBool;
			
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_monster_templ, &ms.m_ulMonsterTemplID);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_monster_summon_is_monster, &nBool);
			ms.m_bIsMonster = (nBool != 0);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_monster_num, &ms.m_ulMonsterNum);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_world_id, &ms.m_ulMapId);

			if (!ReadLine(fp, line)) return false;
			sscanf(line,  _format_zone_vert, &ms.m_SummonVert.x, &ms.m_SummonVert.y, &ms.m_SummonVert.z);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_period, &ms.m_lPeriod);
		}
	}

	if (ulVersion >= 58)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_count, &m_ulPremTitleCount);

		if (m_ulPremTitleCount)
			m_PremTitles = new short[m_ulPremTitleCount];

		for (i = 0; i < m_ulPremTitleCount; i++)
		{
			if (!ReadLine(fp, line)) return false;
			int nTitle;
			sscanf(line, _format_award_title, &nTitle);
			m_PremTitles[i] = (short)nTitle;
		}
	}

	if (ulVersion >= 5)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_given_items_num, &m_ulGivenItems);

		if (m_ulGivenItems)
		{
#ifdef TASK_TEMPL_EDITOR
			m_GivenItems = new ITEM_WANTED[MAX_ITEM_WANTED];
			memset(m_GivenItems, 0, sizeof(ITEM_WANTED) * MAX_ITEM_WANTED);
#else
			m_GivenItems = new ITEM_WANTED[m_ulGivenItems];
#endif
		}

		for (i = 0; i < m_ulGivenItems; i++)
		{
			ITEM_WANTED& iw = m_GivenItems[i];

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_item_templ, &iw.m_ulItemTemplId);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_item_num, &iw.m_ulItemNum);

			if (ulVersion >= 21)
			{
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_item_is_cmn, &nRead);
				iw.m_bCommonItem = (nRead != 0);
			}
			else
				iw.m_bCommonItem = false;

			if (iw.m_bCommonItem) m_ulGivenCmnCount++;
			else m_ulGivenTskCount++;
		}
	}

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_prem_deposit, &m_ulPremise_Deposit);

	if (ulVersion >= 63)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_contrib, &m_lPremise_Contribution, &nRead);
		m_bDepositContribution = (nRead != 0);
	}

	if (ulVersion >= 81)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_fam_contrib, &m_nPremise_FamContrib, &nRead);
		m_bDepositFamContrib = (nRead != 0);
	}

	if (ulVersion >= 92)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_fam_max, &m_nPremFamContribMax);
	}

	if (ulVersion >= 92)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_battle_score, &m_nPremBattleScoreMin);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_battle_score, &m_nPremBattleScoreMax);

		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_deposit_battle, &nRead);
		m_bDepositBattleScore = (nRead != 0);
	}

	if (ulVersion >= 120)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_battle_score, &m_nPremSJBattleScore);
		
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_deposit_battle, &nRead);
		m_bSJDepostiBattleScore = (nRead != 0);
	}

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_prem_reputation, &m_lPremise_Reputation);

	if (ulVersion >= 73)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_repu_depo, &nRead);
		m_bRepuDeposit = (nRead != 0);
	}

	if (ulVersion >= 53)
	{
		size_t c;

		if (ulVersion < 103)
		{
			c = 10;
		}
		else if (ulVersion < 123)
		{
			c = 16;
		}
		else
		{
			c = TASK_ZONE_FRIENDSHIP_COUNT;
		}

		for (i = 0; i < c; i++)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_friendship, &m_Premise_Friendship[i]);
		}
	}

	if (ulVersion >= 77)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_friendship_depo, &nRead);
		m_bFriendshipDeposit = (nRead != 0);
	}

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_prem_cotask, &m_ulPremise_Cotask);

	if (ulVersion >= 18)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_cotask_cond, &m_ulCoTaskCond);

		if (ulVersion >= 30)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_mutex_task_count, &m_ulMutexTaskCount);

			for (i = 0; i < m_ulMutexTaskCount; i++)
			{
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_mutex_task, &m_ulMutexTasks[i]);
			}
		}
		else
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_mutex_task, &m_ulMutexTasks[0]);
			if (m_ulMutexTasks[0]) m_ulMutexTaskCount++;
		}
	}

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_prem_task_count, &m_ulPremise_Task_Count);

	for (i = 0; i < m_ulPremise_Task_Count; i++)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_task, &m_ulPremise_Tasks[i]);

		if (ulVersion < 32) ReadLine(fp, line);
	}

	if (ulVersion >= 87)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_task_count, &m_ulPremFinishTaskCount);

		for (i = 0; i < m_ulPremFinishTaskCount; i++)
		{
			FINISH_TASK_COUNT_INFO& info = m_PremFinishTasks[i];
			int count;

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_prem_task, &info.task_id);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_finish_count, &count);
			info.count = (unsigned short)count;
		}
	}

	if (ulVersion >= 89)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_task_count, &m_ulPremGlobalCount);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_task, &m_ulPremGlobalTask);
	}

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_prem_period, &m_ulPremise_Period);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_prem_is_team, &nRead);
	m_bTeamwork = (nRead != 0);

	if ( ulVersion >= 143 ) //共享任务 Added 2012-04-09.
	{
		if (!ReadLine(fp, line))
		{
			return false;
		}
		int nShare = 0;
		sscanf(line, _format_prem_share, &nShare);
		m_bShareWork = (nShare != 0);	
	}

	if (ulVersion >= 2 && m_bTeamwork)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_rcv_by_team, &nRead);
		m_bRcvByTeam = (nRead != 0);

		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_shared, &nRead);
		m_bSharedTask = (nRead != 0);

		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_share_acquire, &nRead);
		m_bSharedAchieved = (nRead != 0);

		if (ulVersion >= 4)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_check_tmmate, &nRead);
			m_bCheckTeammate = (nRead != 0);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_dist, &m_fTeammateDist);
		}

		if (ulVersion >= 12)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_rcv_chck_mem, &nRead);
			m_bRcvChckMem = (nRead != 0);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_dist, &m_fRcvMemDist);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_cnt_by_mem_pos, &nRead);
			m_bCntByMemPos = (nRead != 0);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_dist, &m_fCntMemDist);
		}

		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_all_fail, &nRead);
		m_bAllFail = (nRead != 0);

		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_cap_fail, &nRead);
		m_bCapFail = (nRead != 0);

		if (ulVersion >= 19)
		{
			nRead = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_cap_succ, &nRead);
			m_bCapSucc = (nRead != 0);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_dist, &m_fSuccDist);
		}

		if (ulVersion >= 131)
		{
			nRead = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_all_succ, &nRead);
			m_bAllSucc = (nRead != 0);
		}

		if (ulVersion < 2) ReadLine(fp, line);

		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_dism_self_fail, &nRead);
		m_bDismAsSelfFail = (nRead != 0);

		m_ulTeamMemsWanted = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_mem_num_wanted, &m_ulTeamMemsWanted);

		if (m_ulTeamMemsWanted)
		{
#ifdef TASK_TEMPL_EDITOR
			m_TeamMemsWanted = new TEAM_MEM_WANTED[MAX_TEAM_MEM_WANTED];
			memset(m_TeamMemsWanted, 0, sizeof(TEAM_MEM_WANTED) * MAX_TEAM_MEM_WANTED);
#else
			m_TeamMemsWanted = new TEAM_MEM_WANTED[m_ulTeamMemsWanted];
#endif
		}

		for (i = 0; i < m_ulTeamMemsWanted; i++)
		{
			TEAM_MEM_WANTED& tmw = m_TeamMemsWanted[i];

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_level, &tmw.m_ulLevelMin);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_level, &tmw.m_ulLevelMax);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_occupation, &tmw.m_ulOccupation);

			if (ulVersion >= 22)
			{
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_race, &tmw.m_ulRace);
			}

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_gender, &tmw.m_ulGender);

			if (ulVersion >= 94)
			{
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_lifeagain, &tmw.m_ulLifeAgain);
			}

			if (ulVersion >= 83)
			{
				nRead = 0;
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_same_family, &nRead);
				tmw.m_bSameFamily = (nRead != 0);
			}

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_count, &tmw.m_ulMinCount);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_count, &tmw.m_ulMaxCount);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_task, &tmw.m_ulTask);
		}
	}

	if (ulVersion >= 101)
	{
		nRead = 0;

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_master_wugui, &nRead);
		m_bMaster = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prentice, &nRead);
		m_bPrentice =  (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_master_moral, &m_lMasterMoral);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_cap_change_fail, &nRead);
		m_bCapChangeAllFail = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_mp_task, &nRead);
		m_bMPTask = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_mp_task_cnt, &m_ulMPTaskCnt);

		for (i = 0; i < m_ulMPTaskCnt; i++)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_mp_lev_limit, &m_MPTask[i].m_ulLevLimit);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_mp_task_id, &m_MPTask[i].m_ulTaskID);			
		}		
	}

	if (ulVersion >= 107)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_out_master_task, &nRead);
		m_bOutMasterTask = (nRead != 0);
	}

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_prem_is_spouse, &nRead);
	m_bPremise_Spouse = (nRead != 0);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_prem_clan, &m_ulPremise_Faction);

	if (ulVersion >= 76)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_master, &nRead);
		m_bPremise_FactionMaster = (nRead != 0);
	}

	if (ulVersion < 2)
	{
		ReadLine(fp, line); // old race

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_occupation, &m_Occupations[0]);
		if (m_Occupations[0] != INVALID_VAL) m_ulOccupations++;
	}
	else
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_gender, &m_ulGender);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_count, &m_ulOccupations);

		for (i = 0; i < m_ulOccupations; i++)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_prem_occupation, &m_Occupations[i]);
		}
	}

	if (ulVersion >= 30)
	{
		for (i = 0; i < MAX_LIVING_SKILLS; i++)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_proficiency, &m_lSkillLev[i]);
		}
	}

	if (ulVersion >= 99)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_skill_lev, &m_nSkillLev);
	}

	if (ulVersion >= 100)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_pet_con, &m_nPetCon);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_pet_civ, &m_nPetCiv);
	}

	if (ulVersion >= 51)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_pk_value, &m_lPKValueMin);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_pk_value, &m_lPKValueMax);
	}

	if (ulVersion >= 52)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_gm, &nRead);
		m_bPremise_GM = (nRead != 0);
	}

	if (ulVersion >= 14)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_by_deposit, &nRead);
		m_bShowByDeposit = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_by_gender, &nRead);
		m_bShowByGender = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_by_items, &nRead);
		m_bShowByItems = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_by_lev, &nRead);
		m_bShowByLev = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_by_occup, &nRead);
		m_bShowByOccup = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_by_preTask, &nRead);
		m_bShowByPreTask = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_by_repu, &nRead);
		m_bShowByRepu = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_by_team, &nRead);
		m_bShowByTeam = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_by_faction, &nRead);
		m_bShowByFaction = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_by_period, &nRead);
		m_bShowByPeriod = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_by_spouse, &nRead);
		m_bShowBySpouse = (nRead != 0);
	}

	if (ulVersion >= 79)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_in_family, &nRead);
		m_bInFamily = (nRead != 0);

		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_family_header, &nRead);
		m_bFamilyHeader = (nRead != 0);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_f_skill_lev, &m_nFamilySkillLevelMin);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_f_skill_lev, &m_nFamilySkillLevelMax);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_f_skill_prof, &m_nFamilySkillProficiencyMin);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_f_skill_prof, &m_nFamilySkillProficiencyMax);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_f_skill_index, &m_nFamilySkillIndex);
	}

	if (ulVersion >= 82)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_record_index, &m_nFamilyMonRecordIndex);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_record_value, &m_nFamilyMonRecordMin);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_record_value, &m_nFamilyMonRecordMax);
	}

	if (ulVersion >= 84)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_f_value_index, &m_nFamilyValueIndex);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_f_value, &m_nFamilyValueMin);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_f_value, &m_nFamilyValueMax);
	}

	if (ulVersion >= 86)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_f_deposit_value, &nRead);
		m_bDepositFamilyValue = (nRead != 0);
	}

    if (ulVersion >= 96)
    {
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_check_lifeagain, &nRead);
		m_bCheckLifeAgain = (nRead != 0);

		if (m_bCheckLifeAgain)
		{		
			nRead = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_spouse_again, &nRead);
			m_bSpouseAgain = (nRead != 0);
			
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_lifeagain_cnt, &m_ulLifeAgainCnt);

			if (ulVersion >= 118)
			{
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_lifeagain_cntcom, &m_nLifeAgainCntCompare);
			}
			
			int max_occupations;
			if(ulVersion >= 165)
				max_occupations = MAX_OCCUPATIONS;
			else
				max_occupations = 32;

			for (i=0; i<max_occupations; i++)
			{
				nRead = 0;
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_lifeagain_occup, &nRead);
				m_bLifeAgainOneOccup[i] = (nRead != 0);
			}

			for (i=0; i<max_occupations; i++)
			{
				nRead = 0;
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_lifeagain_occup, &nRead);
				m_bLifeAgainTwoOccup[i] = (nRead != 0);
			}

			for (i=0; i<max_occupations; i++)
			{
				nRead = 0;
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_lifeagain_occup, &nRead);
				m_bLifeAgainThrOccup[i] = (nRead != 0);
			}
		}
    }

    if (ulVersion >= 106)
    {
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_cult, &m_ulPremCult);
    }

	if (ulVersion >= 117)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_need_comp, &nRead);
		m_bPremNeedComp = (nRead != 0);			

		if (m_bPremNeedComp)
		{
			if (ulVersion >= 131)
			{
				if (!ReadLine(fp, line)) return false;
					sscanf(line, _format_exp1_andor_exp2, &m_nPremExp1AndOrExp2);
			}

			char szExpLeft[MAX_TASK_EXP_LEN]={0};
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_left_string, szExpLeft);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_comp_oper, &m_PremCompExp1.nCompOper);
			
			char szExpRight[MAX_TASK_EXP_LEN]={0};
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_right_string, szExpRight);
			
			int i=0;
			do 
			{
				m_PremCompExp1.strExpLeft.push_back(szExpLeft[i]);

			} while(szExpLeft[i++]);	
			
			i=0;
			do 
			{
				m_PremCompExp1.strExpRight.push_back(szExpRight[i]);
				
			} while(szExpRight[i++]);

			if (ulVersion >= 131)
			{
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_left_string, szExpLeft);

				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_comp_oper, &m_PremCompExp2.nCompOper);
				
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_right_string, szExpRight);
				
				i=0;
				do 
				{
					m_PremCompExp2.strExpLeft.push_back(szExpLeft[i]);

				} while(szExpLeft[i++]);	
				
				i=0;
				do 
				{
					m_PremCompExp2.strExpRight.push_back(szExpRight[i]);
					
				} while(szExpRight[i++]);
			}
		}
	}

	if (ulVersion >= 122)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_accomp_cnt, &m_ulPremAccompCnt);
		
		for (i = 0; i < m_ulPremAccompCnt; i++)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_accomp_id, &m_aPremAccompID[i]);
		}
		
		int nBool = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_accomp_cond, &nBool);
		m_bPremAccompCond = (nBool != 0);
	}

	if (ulVersion >= 124)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_transform_id, &m_iPremTransformID);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_transform_lev, &m_iPremTransformLevel);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_transform_lev, &m_iPremTransformExpLevel);
	}
	
	if (ulVersion >= 129)
	{
		int nBool = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_script_open, &nBool);
		m_bScriptOpenTask = (nBool != 0);		
	}

	if (ulVersion >= 130)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_achieve_min, &m_ulPremAchievementMin);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_achieve_max, &m_ulPremAchievementMax);
	}

	if (ulVersion >= 132)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_circle_group_min, &m_ulPremCircleGroupMin);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_circle_group_max, &m_ulPremCircleGroupMax);
	}

	if (ulVersion >= 135)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_territory_score_min, &m_ulPremTerritoryScoreMin);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_territory_score_max, &m_ulPremTerritoryScoreMax);
	}

	if (ulVersion >= 138)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_fengshen_type, &m_nPremFengshenType);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_fengshen_lvl_min, &m_ulPremFengshenLvlMin);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_fengshen_lvl_max, &m_ulPremFengshenLvlMax);

		int nBool = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_prem_fengshen_exp_full, &nBool);
		m_bExpMustFull = (nBool != 0);
		
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_by_fengshen, &nBool);
		m_bShowByFengshenLvl = (nBool != 0);	
	}

	//Added 2011-04-11.
	if ( ulVersion >= 141 )
	{
		if ( !ReadLine(fp, line) )
		{
			return false;
		}
		sscanf(line, _format_create_role_time_duration, &m_ulCreateRoleTimeDuration);
	}


	if(ulVersion >= 162)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_building_id, &m_nBuildId);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_building_level, &m_nBuildLevel);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_faction_premise_gold_note, &m_iPremise_FactionGoldNote);
		
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_by_gold_note, &nRead);
		m_bShowByFactionGoldNote = (nRead != 0);
	
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_faction_premise_grass, &m_iPremise_FactionGrass);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_faction_premise_mine, &m_iPremise_FactionMine);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_faction_premise_monster_core, &m_iPremise_FactionMonsterCore);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_faction_premise_mosnter_food,&m_iPremise_FactionMonsterFood);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_faction_premise_money, &m_iPremise_FactionMoney);


	}

	if(ulVersion >= 164)
	{
		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_build_level_in_construct, &m_nBuildLevelInConstruct);
	}

	if(ulVersion >= 163)
	{
		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_interobj_id, &m_iInterObjId);

		nRead = 0;
		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_by_interobj_id, &nRead);
		m_bShowByInterObjId = (nRead != 0);

	}

	if (ulVersion >= 157)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_total_caseadd, &m_nPremTotalCaseAddMin);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_total_caseadd, &m_nPremTotalCaseAddMax);
	}

	/* 任务完成的方式及条件 */

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_method, &m_enumMethod);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_finish_type, &m_enumFinishType);

	if (ulVersion >= 130)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_finish_achieve, &m_ulFinishAchievement);		
	}

	if (ulVersion >= 131)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_finish_friend_num, &m_ulFriendNum);		
	}

	if (ulVersion >= 57)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_finish_lev, &m_ulFinishLev);

		if (ulVersion >= 112)
		{
			nRead = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_finish_dlg, &nRead);
			m_bDisFinDlg = (nRead != 0);
		}
	}

	if (ulVersion >= 129)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_script_finish, &nRead);
		m_bScriptFinishTask = (nRead != 0);
	}

	if (ulVersion >= 117)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_need_comp, &nRead);
		m_bFinNeedComp = (nRead != 0);			

		if (m_bFinNeedComp)
		{
			if (ulVersion >= 131)
			{
				if (!ReadLine(fp, line)) return false;
					sscanf(line, _format_exp1_andor_exp2, &m_nFinExp1AndOrExp2);
			}

			char szExpLeft[MAX_TASK_EXP_LEN]={0};
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_left_string, szExpLeft);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_comp_oper, &m_FinCompExp1.nCompOper);

			char szExpRight[MAX_TASK_EXP_LEN]={0};
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_right_string, szExpRight);
			
			int i=0;
			do 
			{
				m_FinCompExp1.strExpLeft.push_back(szExpLeft[i]);
				
			} while(szExpLeft[i++]);	
			
			i=0;
			do 
			{
				m_FinCompExp1.strExpRight.push_back(szExpRight[i]);
				
			} while(szExpRight[i++]);

			if (ulVersion >= 131)
			{
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_left_string, szExpLeft);

				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_comp_oper, &m_FinCompExp2.nCompOper);
				
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_right_string, szExpRight);
				
				i=0;
				do 
				{
					m_FinCompExp2.strExpLeft.push_back(szExpLeft[i]);

				} while(szExpLeft[i++]);	
				
				i=0;
				do 
				{
					m_FinCompExp2.strExpRight.push_back(szExpRight[i]);
					
				} while(szExpRight[i++]);
			}
		}
	}
	
	if (m_enumMethod == enumTMKillNumMonster)
	{
		if (ulVersion < 28)
			m_ulMonsterWanted = 1;
		else
		{
			m_ulMonsterWanted = 0;

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_monster_wanted, &m_ulMonsterWanted);
		}

		if (ulVersion >= 130)
		{
			int nBool = 0;
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_summon_monster_mode, &nBool);
			m_bSummonMonsterMode = (nBool != 0);
		}

		if (m_ulMonsterWanted)
		{
#ifdef TASK_TEMPL_EDITOR
			m_MonsterWanted = new MONSTER_WANTED[MAX_MONSTER_WANTED];
			memset(m_MonsterWanted, 0, sizeof(MONSTER_WANTED) * MAX_MONSTER_WANTED);
#else
			m_MonsterWanted = new MONSTER_WANTED[m_ulMonsterWanted];
#endif
		}

		for (i = 0; i < m_ulMonsterWanted; i++)
		{
			MONSTER_WANTED& Monster = m_MonsterWanted[i];

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_monster_templ, &Monster.m_ulMonsterTemplId);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_monster_num, &Monster.m_ulMonsterNum);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_item_templ, &Monster.m_ulDropItemId);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_item_wanted_num, &Monster.m_ulDropItemCount);

			if (ulVersion >= 23)
			{
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_item_is_cmn, &nRead);
				Monster.m_bDropCmnItem = (nRead != 0);
			}

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_item_drop_prob, &Monster.m_fDropProb);

			if (ulVersion >= 34)
			{
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_killer_lev, &nRead);
				Monster.m_bKillerLev = (nRead != 0);
			}
			else
				Monster.m_bKillerLev = false;
		}
	}
	else if (m_enumMethod == enumTMCollectNumArticle)
	{
		if (ulVersion >= 39)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_gold, &m_ulGoldWanted);
		}

		m_ulItemsWanted = 0;

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_item_wanted_num, &m_ulItemsWanted);

		if (m_ulItemsWanted)
		{
#ifdef TASK_TEMPL_EDITOR
			m_ItemsWanted = new ITEM_WANTED[MAX_ITEM_WANTED];
			memset(m_ItemsWanted, 0, sizeof(ITEM_WANTED) * MAX_ITEM_WANTED);
#else
			m_ItemsWanted = new ITEM_WANTED[m_ulItemsWanted];
#endif
		}

		for (i = 0; i < m_ulItemsWanted; i++)
		{
			ITEM_WANTED& iw = m_ItemsWanted[i];
			int nBool = 0;
			iw.m_fProb = 1.0f;
			iw.m_bBind = false;
			iw.m_lPeriod = 0;
			iw.m_bTimetable = false;

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_item_templ, &iw.m_ulItemTemplId);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_item_is_cmn, &nBool);
			iw.m_bCommonItem = (nBool != 0);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_item_num, &iw.m_ulItemNum);

			if(ulVersion >= 133)
			{
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_item_refine_level, &iw.m_ulRefineLevel);
			}

			if(ulVersion >= 150)
			{
				if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_item_replace_templ, &iw.m_ulReplaceItemTemplId);				
			}
		}

		//Added 2011-10-08.
		if ( ulVersion >= 142 )
		{
			int nAutoMoveForCollectNumItems = 0;
			if ( !ReadLine(fp, line) )
			{
				return false;
			}
			sscanf(line, _format_auto_move_for_collect_num_items_enable, &nAutoMoveForCollectNumItems);
			m_bAutoMoveForCollectNumItems = (nAutoMoveForCollectNumItems != 0);
		}
		//Added end.
	}
	else if (m_enumMethod == enumTMTalkToNPC)
	{
		if (ulVersion < 13) ReadLine(fp, line);
	}
	else if (m_enumMethod == enumTMProtectNPC)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_npc_to_protect, &m_ulNPCToProtect);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_protect_tm_len, &m_ulProtectTimeLen);
	}
	else if (m_enumMethod == enumTMWaitTime)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_waittime, &m_ulWaitTime);

		//Added 2011-10-08.
		if ( ulVersion >= 142 )
		{
			int nShowWaitTime = 0;
			if ( !ReadLine(fp, line) )
			{
				return false;
			}
			sscanf(line, _format_show_wait_time_enable, &nShowWaitTime);
			m_bShowWaitTime = (nShowWaitTime != 0);
		}
		//Added end.
	}
	else if (m_enumMethod == enumTMReachSite)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_ReachSiteMin));

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_ReachSiteMax));

		if (ulVersion >= 39)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_world_id, &m_ulReachSiteId);
		}

		//Added 2011-10-08.
		if ( ulVersion >= 142 )
		{
			if ( !ReadLine(fp, line) )
			{
				return false;
			}
			sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_AutoMoveDestPos));

			int nAutoMoveEnable = 0;
			if ( !ReadLine(fp, line) )
			{
				return false;
			}
			sscanf(line, _format_auto_move_enable, &nAutoMoveEnable);
			m_bAutoMoveForReachFixedSite = ( nAutoMoveEnable != 0 );

			//Added 2011-10-09.
			fread(m_szAutoMoveDestPosName, sizeof(task_char), MAX_TASK_NAME_LEN, fp);
			if (!ReadLine(fp, line)) return false;
			//Added end.
		}
		//Added end.
	}
	else if (m_enumMethod == enumTMLeaveSite)
	{
		if(ulVersion >= 131)
		{
			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_world_id, &m_ulLeaveSiteId);

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_LeaveSiteMin));

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_LeaveSiteMax));
		}
	}
	else if (m_enumMethod == enumTMTitle)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_title_count, &m_ulTitleWantedNum);

		for (i = 0; i < m_ulTitleWantedNum; i++)
		{
			long t;

			if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_title, &t);

			if (i < MAX_TITLE_WANTED)
				m_TitleWanted[i] = (short)t;
		}
	}
	else if (m_enumMethod == enumTMFixedTime)
	{
		task_tm& s = m_tmFixedTime;

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_fixed_time_type, &m_iFixedType);

		if (!ReadLine(fp, line)) return false;
			sscanf(line, _format_full_tm, &s.year, &s.month, &s.day, &s.hour, &s.min, &s.wday);		
	}
	else if (m_enumMethod == enumTMNPCAction)
	{
		if( ulVersion > 148)
		{
			if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_action_npc, &m_ulActionNPC);

			if (!ReadLine(fp, line)) return false;
				sscanf(line, _format_action_id, &m_nActionID);
		}
	}

	if (ulVersion >= 154)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_nation_position_mask, &m_ulPremNationPositionMask);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_king_score_cost, &m_nPremKingScoreCost);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_max_king_score, &m_nPremKingScoreMax);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_min_king_score, &m_nPremKingScoreMin);
	}

	if (ulVersion >= 156)
	{
		int nBool = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_has_king, &nBool);
		m_bPremHasKing = (nBool != 0);
	}

	if (ulVersion >= 157)
	{
		nRead = 0;
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_total_caseadd, &m_nTotalCaseAddMin);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_total_caseadd, &m_nTotalCaseAddMax);
	}

	if(ulVersion >= 162)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_faction_gold_note_wanted, &m_iFactionGoldNoteWanted);


		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_faction_grass_wanted, &m_iFactionGrassWanted);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_faction_mine_wanted, &m_iFactionMineWanted);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_faction_monster_core_wanted, &m_iFactionMonsterCoreWanted);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_faction_mosnter_food_wanted, &m_iFactionMonsterFoodWanted);

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_faction_money_wanted, &m_iFactionMoneyWanted);
	}
	
	if(ulVersion >= 163)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_interobj_wanted, &m_ulInterObjWanted);


		if (m_ulInterObjWanted)
		{
#ifdef TASK_TEMPL_EDITOR
			m_InterObjWanted = new INTEROBJ_WANTED[MAX_INTEROBJ_WANTED];
			memset(m_InterObjWanted, 0, sizeof(INTEROBJ_WANTED) * MAX_INTEROBJ_WANTED);
#else
			m_InterObjWanted = new INTEROBJ_WANTED[m_ulInterObjWanted];
#endif
		}

			for (i = 0; i < m_ulInterObjWanted; i++)
			{
				INTEROBJ_WANTED& wi = m_InterObjWanted[i];

				if(!ReadLine(fp, line)) return false;
				sscanf(line, _format_interobj_templ, &wi.m_ulInterObjId);

				if(!ReadLine(fp, line)) return false;
				sscanf(line, _format_interobj_num, &wi.m_ulInterObjNum);					
			}
	}

	if(ulVersion >= 163)
	{
		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_InterReachSiteMin));

		if (!ReadLine(fp, line)) return false;
		sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_InterReachSiteMax));
		
		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_world_id, &m_ulInterReachSiteId);

		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_interobj_wanted, &m_iInterReachItemId);


		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_InterLeaveSiteMin));

		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_zone_vert, VERT_ZONE_ADDR(m_InterLeaveSiteMax));

		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_world_id, &m_ulInterLeaveSiteId);

		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_interobj_wanted, &m_iInterLeaveItemId);
	}

	if(ulVersion >= 164)
	{	
		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_build_id_wanted, &m_iBuildingIdWanted);
		
		if(!ReadLine(fp, line)) return false;
		sscanf(line, _format_build_level_wanted, &m_iBuildingLevelWanted);
	}


	/* 任务结束后的奖励 */
#define LOAD_AWARD_BY_RESULT(Result, editor) \
{\
	unsigned long i;\
\
	if (ulVersion >= 3)\
	{\
		if (!ReadLine(fp, line)) return false;\
		sscanf(line, _format_award_type, &m_ulAwardType##Result);\
	}\
	else\
		m_ulAwardType##Result = enumTATNormal;\
\
	switch (m_ulAwardType##Result)\
	{\
	case enumTATNormal:\
	case enumTATEach:\
		LoadAwardData(fp, *m_Award##Result, ulVersion);\
		break;\
	case enumTATRatio:\
		if (!ReadLine(fp, line)) return false;\
		sscanf(line, _format_count, &m_AwByRatio##Result->m_ulScales);\
		if (m_AwByRatio##Result->m_ulScales)\
		{\
		    if (editor)\
			{\
				m_AwByRatio##Result->m_Awards = new AWARD_DATA[MAX_AWARD_SCALES];\
				memset(m_AwByRatio##Result->m_Awards, 0, sizeof(AWARD_DATA) * MAX_AWARD_SCALES);\
			}\
			else\
				m_AwByRatio##Result->m_Awards = new AWARD_DATA[m_AwByRatio##Result->m_ulScales];\
		}\
\
		for (i = 0; i < m_AwByRatio##Result->m_ulScales; i++)\
		{\
			if (!ReadLine(fp, line)) return false;\
			sscanf(line, _format_ratio, &m_AwByRatio##Result->m_Ratios[i]);\
\
			LoadAwardData(fp, m_AwByRatio##Result->m_Awards[i], ulVersion);\
		}\
		break;\
	case enumTATItemCount:\
		if (!ReadLine(fp, line)) return false;\
		sscanf(line, _format_count, &m_AwByItems##Result->m_ulScales);\
\
		if (!ReadLine(fp, line)) return false;\
		sscanf(line, _format_item_templ, &m_AwByItems##Result->m_ulItemId);\
		if (m_AwByItems##Result->m_ulScales)\
		{\
			if (editor)\
			{\
				m_AwByItems##Result->m_Awards = new AWARD_DATA[MAX_AWARD_SCALES];\
				memset(m_AwByItems##Result->m_Awards, 0, sizeof(AWARD_DATA) * MAX_AWARD_SCALES);\
			}\
			else\
				m_AwByItems##Result->m_Awards = new AWARD_DATA[m_AwByItems##Result->m_ulScales];\
		}\
\
		for (i = 0; i < m_AwByItems##Result->m_ulScales; i++)\
		{\
			if (!ReadLine(fp, line)) return false;\
			sscanf(line, _format_item_num, &m_AwByItems##Result->m_Counts[i]);\
\
			LoadAwardData(fp, m_AwByItems##Result->m_Awards[i], ulVersion);\
		}\
		break;\
	case enumTATFinishCount:\
		if (!ReadLine(fp, line)) return false;\
		sscanf(line, _format_count, &m_AwByCount##Result->m_ulScales);\
\
		if (m_AwByCount##Result->m_ulScales)\
		{\
			if (editor)\
			{\
				m_AwByCount##Result->m_Awards = new AWARD_DATA[MAX_AWARD_SCALES];\
				memset(m_AwByCount##Result->m_Awards, 0, sizeof(AWARD_DATA) * MAX_AWARD_SCALES);\
			}\
			else\
				m_AwByCount##Result->m_Awards = new AWARD_DATA[m_AwByCount##Result->m_ulScales];\
		}\
\
		for (i = 0; i < m_AwByCount##Result->m_ulScales; i++)\
		{\
			if (!ReadLine(fp, line)) return false;\
			sscanf(line, _format_item_num, &m_AwByCount##Result->m_Counts[i]);\
\
			LoadAwardData(fp, m_AwByCount##Result->m_Awards[i], ulVersion);\
		}\
		break;\
	}\
}

#ifdef TASK_TEMPL_EDITOR
	LOAD_AWARD_BY_RESULT(_S, true)
	LOAD_AWARD_BY_RESULT(_F, true)
#else
	LOAD_AWARD_BY_RESULT(_S, false)
	LOAD_AWARD_BY_RESULT(_F, false)
#endif

	/* 层次关系 */

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_parent, &m_ulParent);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_next_sibling, &m_ulNextSibling);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_prev_sibling, &m_ulPrevSibling);

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_first_child, &m_ulFirstChild);

	return true;
}

bool ATaskTemplFixedData::LoadFixedDataFromBinFile(FILE* fp)
{
	LOG_DELETE(m_Award_S);
	LOG_DELETE(m_Award_F);
	LOG_DELETE(m_AwByRatio_S);
	LOG_DELETE(m_AwByRatio_F);
	LOG_DELETE(m_AwByItems_S);
	LOG_DELETE(m_AwByItems_F);
	LOG_DELETE(m_AwByCount_S);
	LOG_DELETE(m_AwByCount_F);
	LOG_DELETE_ARR(m_tmStart);
	LOG_DELETE_ARR(m_tmEnd);
	LOG_DELETE_ARR(m_PremItems);
	LOG_DELETE_ARR(m_MonsterSummoned);
	LOG_DELETE_ARR(m_GivenItems);
	LOG_DELETE_ARR(m_TeamMemsWanted);
	LOG_DELETE_ARR(m_ItemsWanted);
	LOG_DELETE_ARR(m_MonsterWanted);
	LOG_DELETE_ARR(m_pszSignature);


	unsigned long i;
	fread(this, sizeof(*this), 1, fp);
	assert(m_ID);

	m_Award_S		= new AWARD_DATA;
	m_Award_F		= new AWARD_DATA;
	m_AwByRatio_S	= new AWARD_RATIO_SCALE;
	m_AwByRatio_F	= new AWARD_RATIO_SCALE;
	m_AwByItems_S	= new AWARD_ITEMS_SCALE;
	m_AwByItems_F	= new AWARD_ITEMS_SCALE;
	m_AwByCount_S	= new AWARD_COUNT_SCALE;
	m_AwByCount_F	= new AWARD_COUNT_SCALE;
	m_tmStart		= NULL;
	m_tmEnd			= NULL;
	m_PremItems		= NULL;
	m_MonsterSummoned = NULL;
	m_GivenItems	= NULL;
	m_TeamMemsWanted= NULL;
	m_ItemsWanted	= NULL;
	m_MonsterWanted	= NULL;
	m_pszSignature	= NULL;

	if (m_bHasSign)
	{
		m_pszSignature = new task_char[MAX_TASK_NAME_LEN];
		g_ulNewCount++;
		fread(m_pszSignature, sizeof(task_char), MAX_TASK_NAME_LEN, fp);
	}

	if (m_ulTimetable)
	{
		m_tmStart	= new task_tm[m_ulTimetable];
		g_ulNewCount++;
		m_tmEnd		= new task_tm[m_ulTimetable];
		g_ulNewCount++;
	}

	for (i = 0; i < m_ulTimetable; i++)
	{
		fread(&m_tmStart[i], sizeof(task_tm), 1, fp);
		fread(&m_tmEnd[i], sizeof(task_tm), 1, fp);
	}

	if (m_lChangeKeyArr.size())
	{
		size_t nArrSize = m_lChangeKeyArr.size();

		long lChangeKeyArr[TASK_AWARD_MAX_CHANGE_VALUE]={0};
		long lChangeKeyValueArr[TASK_AWARD_MAX_CHANGE_VALUE]={0};
		bool bChangeTypeArr[TASK_AWARD_MAX_CHANGE_VALUE]={0};
		fread(lChangeKeyArr, sizeof(long), nArrSize, fp);
		fread(lChangeKeyValueArr, sizeof(long), nArrSize, fp);
		fread(bChangeTypeArr, sizeof(bool), nArrSize, fp);

		memset(&m_lChangeKeyArr, 0, sizeof(m_lChangeKeyArr));
		memset(&m_lChangeKeyValueArr, 0, sizeof(m_lChangeKeyValueArr));
		memset(&m_bChangeTypeArr, 0, sizeof(m_bChangeTypeArr));
		for (i=0; i<nArrSize; i++)
		{
			m_lChangeKeyArr.push_back(lChangeKeyArr[i]);
			m_lChangeKeyValueArr.push_back(lChangeKeyValueArr[i]);
			m_bChangeTypeArr.push_back(bChangeTypeArr[i]);
		}
	}

	/* 任务的开启条件及方式 */
	if (m_ulPremItems)
	{
		m_PremItems = new ITEM_WANTED[m_ulPremItems];
		g_ulNewCount++;
	}

	for (i = 0; i < m_ulPremItems; i++)
	{
		ITEM_WANTED& iw = m_PremItems[i];
		fread(&iw, sizeof(iw), 1, fp);
	}

	if (m_ulPremMonsterSummoned)
	{
		m_MonsterSummoned = new MONSTER_SUMMONED[m_ulPremMonsterSummoned];
		g_ulNewCount++;
	}

	for (i = 0; i < m_ulPremMonsterSummoned; i++)
	{
		MONSTER_SUMMONED& ms = m_MonsterSummoned[i];
		fread(&ms, sizeof(ms), 1, fp);
	}	

	if (m_ulPremTitleCount)
	{
		m_PremTitles = new short[m_ulPremTitleCount];
		fread(m_PremTitles, sizeof(short), m_ulPremTitleCount, fp);
	}
	else
		m_PremTitles = NULL;

	m_ulGivenCmnCount = 0;
	m_ulGivenTskCount = 0;

	if (m_ulGivenItems)
	{
		m_GivenItems = new ITEM_WANTED[m_ulGivenItems];
		g_ulNewCount++;
	}

	for (i = 0; i < m_ulGivenItems; i++)
	{
		ITEM_WANTED& iw = m_GivenItems[i];
		fread(&iw, sizeof(iw), 1, fp);

		if (iw.m_bCommonItem) m_ulGivenCmnCount++;
		else m_ulGivenTskCount++;
	}

	if (m_bTeamwork)
	{
		if (m_ulTeamMemsWanted)
		{
			m_TeamMemsWanted = new TEAM_MEM_WANTED[m_ulTeamMemsWanted];
			g_ulNewCount++;
		}

		for (i = 0; i < m_ulTeamMemsWanted; i++)
		{
			TEAM_MEM_WANTED& tmw = m_TeamMemsWanted[i];
			fread(&tmw, sizeof(tmw), 1, fp);
		}
	}
	
	if (m_bPremNeedComp)
	{
		size_t i=0;
		char szExpLeft[MAX_TASK_EXP_LEN] ={0};
		char szExpRight[MAX_TASK_EXP_LEN]={0};			

		size_t nstrExpLeftLen  = m_PremCompExp1.strExpLeft.size();
		size_t nstrExpRightLen = m_PremCompExp1.strExpRight.size();

		fread(szExpLeft, sizeof(char), nstrExpLeftLen, fp);
		fread(szExpRight, sizeof(char), nstrExpRightLen, fp);

		memset(&m_PremCompExp1.strExpLeft, 0, sizeof(m_PremCompExp1.strExpLeft));		
		for (i=0; i<nstrExpLeftLen+1; i++)
		{
			m_PremCompExp1.strExpLeft.push_back(szExpLeft[i]);
		}

		memset(&m_PremCompExp1.strExpRight, 0, sizeof(m_PremCompExp1.strExpRight));
		for (i=0; i<nstrExpRightLen+1; i++)
		{
			m_PremCompExp1.strExpRight.push_back(szExpRight[i]);
		}
	
		TaskExpAnalyser Analyser;

		memset(&m_PremCompExp1.arrExpLeft, 0, sizeof(m_PremCompExp1.arrExpLeft));	
		Analyser.Analyse(&m_PremCompExp1.strExpLeft[0], m_PremCompExp1.arrExpLeft);	

		memset(&m_PremCompExp1.arrExpRight, 0, sizeof(m_PremCompExp1.arrExpRight));
		Analyser.Analyse(&m_PremCompExp1.strExpRight[0], m_PremCompExp1.arrExpRight);

		
		for(i=0;i<MAX_TASK_EXP_LEN;i++)
		{
			szExpLeft[i] = 0;
			szExpRight[i] = 0;
		}

		nstrExpLeftLen  = m_PremCompExp2.strExpLeft.size();
		nstrExpRightLen = m_PremCompExp2.strExpRight.size();

		if(nstrExpLeftLen == 0)
		{
			szExpLeft[0] = '0';
			nstrExpLeftLen = 1;
		}
		else
			fread(szExpLeft, sizeof(char), nstrExpLeftLen, fp);
			
		if(nstrExpRightLen == 0)
		{
			szExpRight[0] = '0';
			nstrExpRightLen = 1;
		}
		else
			fread(szExpRight, sizeof(char), nstrExpRightLen, fp);

		memset(&m_PremCompExp2.strExpLeft, 0, sizeof(m_PremCompExp2.strExpLeft));		
		for (i=0; i<nstrExpLeftLen+1; i++)
		{
			m_PremCompExp2.strExpLeft.push_back(szExpLeft[i]);
		}

		memset(&m_PremCompExp2.strExpRight, 0, sizeof(m_PremCompExp2.strExpRight));
		for (i=0; i<nstrExpRightLen+1; i++)
		{
			m_PremCompExp2.strExpRight.push_back(szExpRight[i]);
		}
	
		memset(&m_PremCompExp2.arrExpLeft, 0, sizeof(m_PremCompExp2.arrExpLeft));	
		Analyser.Analyse(&m_PremCompExp2.strExpLeft[0], m_PremCompExp2.arrExpLeft);	

		memset(&m_PremCompExp2.arrExpRight, 0, sizeof(m_PremCompExp2.arrExpRight));
		Analyser.Analyse(&m_PremCompExp2.strExpRight[0], m_PremCompExp2.arrExpRight);
	}

	/* 任务完成的方式及条件 */

	if (m_ulMonsterWanted)
	{
		m_MonsterWanted = new MONSTER_WANTED[m_ulMonsterWanted];
		g_ulNewCount++;
	}

	for (i = 0; i < m_ulMonsterWanted; i++)
	{
		MONSTER_WANTED& mw = m_MonsterWanted[i];
		fread(&mw, sizeof(mw), 1, fp);
	}

	if (m_ulItemsWanted)
	{
		m_ItemsWanted = new ITEM_WANTED[m_ulItemsWanted];
		g_ulNewCount++;
	}

	for (i = 0; i < m_ulItemsWanted; i++)
	{
		ITEM_WANTED& iw = m_ItemsWanted[i];
		fread(&iw, sizeof(iw), 1, fp);
	}

	if(m_ulInterObjWanted)
	{
		m_InterObjWanted = new INTEROBJ_WANTED[m_ulInterObjWanted];
		g_ulNewCount++;
	}

	for(i = 0; i < m_ulInterObjWanted; i++)
	{
		INTEROBJ_WANTED& wi = m_InterObjWanted[i];
		fread(&wi, sizeof(wi), 1, fp);
	}

	if (m_bFinNeedComp)
	{
		char szExpLeft[MAX_TASK_EXP_LEN] ={0};
		char szExpRight[MAX_TASK_EXP_LEN]={0};
		
		size_t nstrExpLeftLen  = m_FinCompExp1.strExpLeft.size();
		size_t nstrExpRightLen = m_FinCompExp1.strExpRight.size();
		
		fread(szExpLeft, sizeof(char), nstrExpLeftLen, fp);
		fread(szExpRight, sizeof(char), nstrExpRightLen, fp);		
		
		memset(&m_FinCompExp1.strExpLeft, 0, sizeof(m_FinCompExp1.strExpLeft));		
		for (i=0; i<nstrExpLeftLen+1; i++)
		{
			m_FinCompExp1.strExpLeft.push_back(szExpLeft[i]);
		}
		memset(&m_FinCompExp1.strExpRight, 0, sizeof(m_FinCompExp1.strExpRight));
		for (i=0; i<nstrExpRightLen+1; i++)
		{
			m_FinCompExp1.strExpRight.push_back(szExpRight[i]);
		}

		TaskExpAnalyser Analyser;
		
		memset(&m_FinCompExp1.arrExpLeft, 0, sizeof(m_FinCompExp1.arrExpLeft));	
		Analyser.Analyse(&m_FinCompExp1.strExpLeft[0], m_FinCompExp1.arrExpLeft);	
		
		memset(&m_FinCompExp1.arrExpRight, 0, sizeof(m_FinCompExp1.arrExpRight));					
		Analyser.Analyse(&m_FinCompExp1.strExpRight[0], m_FinCompExp1.arrExpRight);


		for(i=0;i<MAX_TASK_EXP_LEN;i++)
		{
			szExpLeft[i] = 0;
			szExpRight[i] = 0;
		}

		nstrExpLeftLen  = m_FinCompExp2.strExpLeft.size();
		nstrExpRightLen = m_FinCompExp2.strExpRight.size();
		
		if(nstrExpLeftLen == 0)
		{
			szExpLeft[0] = '0';
			nstrExpLeftLen = 1;
		}
		else
			fread(szExpLeft, sizeof(char), nstrExpLeftLen, fp);
			
		if(nstrExpRightLen == 0)
		{
			szExpRight[0] = '0';
			nstrExpRightLen = 1;
		}
		else
			fread(szExpRight, sizeof(char), nstrExpRightLen, fp);	
		
		memset(&m_FinCompExp2.strExpLeft, 0, sizeof(m_FinCompExp2.strExpLeft));		
		for (i=0; i<nstrExpLeftLen+1; i++)
		{
			m_FinCompExp2.strExpLeft.push_back(szExpLeft[i]);
		}
		memset(&m_FinCompExp2.strExpRight, 0, sizeof(m_FinCompExp2.strExpRight));
		for (i=0; i<nstrExpRightLen+1; i++)
		{
			m_FinCompExp2.strExpRight.push_back(szExpRight[i]);
		}

		memset(&m_FinCompExp2.arrExpLeft, 0, sizeof(m_FinCompExp2.arrExpLeft));	
		Analyser.Analyse(&m_FinCompExp2.strExpLeft[0], m_FinCompExp2.arrExpLeft);	
		
		memset(&m_FinCompExp2.arrExpRight, 0, sizeof(m_FinCompExp2.arrExpRight));					
		Analyser.Analyse(&m_FinCompExp2.strExpRight[0], m_FinCompExp2.arrExpRight);
	}

	/* 任务奖励 */
	LoadAwardDataBin(fp, *m_Award_S, _task_templ_cur_version);
	LoadAwardDataBin(fp, *m_Award_F, _task_templ_cur_version);

	LoadAwardDataRatioScale(fp, *m_AwByRatio_S, _task_templ_cur_version);
	LoadAwardDataRatioScale(fp, *m_AwByRatio_F, _task_templ_cur_version);

	LoadAwardDataItemsScale(fp, *m_AwByItems_S, _task_templ_cur_version);
	LoadAwardDataItemsScale(fp, *m_AwByItems_F, _task_templ_cur_version);

	LoadAwardDataCountScale(fp, *m_AwByCount_S, _task_templ_cur_version);
	LoadAwardDataCountScale(fp, *m_AwByCount_F, _task_templ_cur_version);

	if (m_ulType >= STORAGE_TASK_TYPES)
		m_fStorageWeight = 1.0f;
	else
		m_fStorageWeight = _storage_task_info[m_ulType].weight;

	if (m_ulMaxReceiver || m_enumMethod == enumTMProtectNPC || m_bSharedByFamily)
		m_bAbsTime = true;

	return true;
}

ATaskTempl* ATaskTempl::CreateEmptyTaskTempl()
{
	ATaskTempl* pTempl = new ATaskTempl;
	g_ulNewCount++;
	pTempl->Init();
	return pTempl;
}

void ATaskTempl::AddSubTaskTempl(ATaskTempl* pSub)
{
	pSub->m_pParent = this;

	if (m_pFirstChild == NULL)
		m_pFirstChild = pSub;
	else
	{
		ATaskTempl* pSibling = m_pFirstChild;
		while (pSibling->m_pNextSibling) pSibling = pSibling->m_pNextSibling;
		pSibling->m_pNextSibling = pSub;
		pSub->m_pPrevSibling = pSibling;
	}
}

void ATaskTempl::InsertTaskTemplBefore(ATaskTempl* pInsert)
{
	pInsert->m_pParent = m_pParent;
	if (m_pParent && m_pParent->m_pFirstChild == this)
		m_pParent->m_pFirstChild = pInsert;
	pInsert->m_pPrevSibling = m_pPrevSibling;
	pInsert->m_pNextSibling = this;
	if (m_pPrevSibling) m_pPrevSibling->m_pNextSibling = pInsert;
	m_pPrevSibling = pInsert;
}

bool ATaskTempl::LoadFromTextFile(const char* szPath, bool bLoadDescript)
{
	unsigned long ulVersion;

	return LoadFromTextFile(szPath, bLoadDescript, ulVersion);
}

bool ATaskTempl::LoadFromTextFile(FILE* fp, bool bLoadDescript, unsigned long &ulVersion)
{
//	unsigned long ulVersion;
	char line[TASK_MAX_LINE_LEN];

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_version, &ulVersion);

	if (ulVersion > _task_templ_cur_version) return false;

	if (!Load(fp, ulVersion, true)) return false;

#ifndef _TASK_CLIENT
	// 如果有接受者上限，则记录任务的接受者id和起始时间
	if (m_ulMaxReceiver)
	{
#if DEBUG_LOG == 1
		if (!m_ulTimeLimit)
			TaskInterface::WriteLog(0, m_ID, 0, "MaxRcv Dont Have TimeLimit");
#endif
	}
#endif

	CheckDepth();
	return true;
}

bool ATaskTempl::LoadFromTextFile(const char* szPath, bool bLoadDescript, unsigned long &ulVersion)
{
	FILE* fp = fopen(szPath, "rb");
	if (fp == NULL) return false;
	if (!LoadFromTextFile(fp, bLoadDescript, ulVersion))
	{
		fclose(fp);
		return false;
	}

	fclose(fp);
	strcpy(m_szFilePath, szPath);
	return true;
}

bool ATaskTempl::LoadFromBinFile(FILE* fp)
{
	LoadBinary(fp);
	CheckDepth();
	return true;
}

int ATaskTempl::UnmarshalKillMonster(const char* pData)
{
	const char* p = pData;

	m_ulMonsterWanted = *p;
	p++;

	if (m_ulMonsterWanted)
	{
		m_MonsterWanted = new MONSTER_WANTED[m_ulMonsterWanted];
		g_ulNewCount++;
	}

	size_t sz = m_ulMonsterWanted * sizeof(MONSTER_WANTED);
	if (sz)
	{
		memcpy(m_MonsterWanted, p, sz);
		p += sz;
	}

	return p - pData;
}

int ATaskTempl::UnmarshalCollectItems(const char* pData)
{
	const char* p = pData;

	m_ulItemsWanted = *(unsigned char*)p;
	p++;

	if (m_ulItemsWanted)
	{
		m_ItemsWanted = new ITEM_WANTED[m_ulItemsWanted];
		g_ulNewCount++;

		size_t sz = m_ulItemsWanted * sizeof(ITEM_WANTED);
		memcpy(m_ItemsWanted, p, sz);
		p += sz;
	}

	m_ulGoldWanted = *(long*)p;
	p += sizeof(long);

	return p - pData;
}

int ATaskTempl::UnmarshalDynTask(const char* pData)
{
	Init();

	const char* p = pData;

	unsigned long token_mask1, token_mask2;

	token_mask1 = *(long*)p;
	p += sizeof(long);

	token_mask2 = *(long*)p;
	p += sizeof(long);

	int token_count = 0;

	// dyn type
	m_DynTaskType = *p;
	p++;

	// top task
	if (!m_pParent)
	{
		switch (m_DynTaskType)
		{
		case enumDTTSpecialAward:
			p += UnmarshalSpecialAwardData(p);
			break;
		default:
			break;
		}
	}

	// id
	m_ID = *(long*)p;
	p += sizeof(long);

	// name
	char len = *p; p++;
	len *= sizeof(task_char);
	memcpy(m_szName, p, len);
	p += len;

	// choose one
	m_bChooseOne = (*p != 0);
	p++;

	// rand one
	m_bRandOne = (*p != 0);
	p++;

	// in order
	m_bExeChildInOrder = (*p != 0);
	p++;

	// parent fail
	m_bParentAlsoFail = (*p != 0);
	p++;

	// parent succ
	m_bParentAlsoSucc = (*p != 0);
	p++;

	// give up
	m_bCanGiveUp = (*p != 0);
	p++;

	// redo
	m_bCanRedo = (*p != 0);
	p++;

	// redo after fail
	m_bCanRedoAfterFailure = (*p != 0);
	p++;

	// clear as give up
	m_bClearAsGiveUp = (*p != 0);
	p++;

	// record
	m_bNeedRecord = (*p != 0);
	p++;

	// die
	m_bFailAsPlayerDie = (*p != 0);
	p++;

	// auto deliver
	m_bAutoDeliver = (*p != 0);
	p++;

	// death trig
	m_bDeathTrig = (*p != 0);
	p++;

	// clear acquired
	m_bClearAcquired = (*p != 0);
	p++;

	// spouse
	m_bPremise_Spouse = (*p != 0);
	p++;

	// teamwork
	m_bTeamwork = (*p != 0);
	p++;

	// direction
	m_bShowDirection = (*p != 0);
	p++;

	// level
	m_ulPremise_Lev_Min = *(unsigned char*)p; p++;
	m_ulPremise_Lev_Max = *(unsigned char*)p; p++;

	// 0: time limit
	if ((1 << token_count++) & token_mask1)
	{
		m_ulTimeLimit = *(long*)p;
		p += sizeof(long);
	}

	// 1: reputation
	if ((1 << token_count++) & token_mask1)
	{
		m_lPremise_Reputation = *(long*)p;
		p += sizeof(long);
	}

	// 2: period
	if ((1 << token_count++) & token_mask1)
	{
		m_ulPremise_Period = *(unsigned short*)p;
		p += sizeof(unsigned short);
	}

	// 3: prem items
	if ((1 << token_count++) & token_mask1)
	{
		m_ulPremItems = *(unsigned char*)p;
		p++;

		m_PremItems = new ITEM_WANTED[m_ulPremItems];
		g_ulNewCount++;

		size_t sz = m_ulPremItems * sizeof(ITEM_WANTED);
		memcpy(m_PremItems, p, sz);
		p += sz;
	}

	// 4: delv in zone
	if ((1 << token_count++) & token_mask1)
	{
		m_bDelvInZone = (*p != 0);
		p++;

		m_ulDelvWorld = *(long*)p;
		p += sizeof(long);

		m_DelvMinVert = *(ZONE_VERT*)p;
		p += sizeof(ZONE_VERT);

		m_DelvMaxVert = *(ZONE_VERT*)p;
		p += sizeof(ZONE_VERT);
	}

	// 5: trans to
	if ((1 << token_count++) & token_mask1)
	{
		m_bTransTo = (*p != 0);
		p++;

		m_ulTransWldId = *(long*)p;
		p += sizeof(long);

		m_TransPt = *(ZONE_VERT*)p;
		p += sizeof(ZONE_VERT);
	}

	// 6: given items
	if ((1 << token_count++) & token_mask1)
	{
		m_ulGivenItems = *p;
		p++;

		m_ulGivenCmnCount = *p;
		p++;

		m_ulGivenTskCount = *p;
		p++;

		m_GivenItems = new ITEM_WANTED[m_ulGivenItems];
		g_ulNewCount++;

		size_t sz = m_ulGivenItems * sizeof(ITEM_WANTED);
		memcpy(m_GivenItems, p, sz);
		p += sz;
	}

	// 7: deposit
	if ((1 << token_count++) & token_mask1)
	{
		m_ulPremise_Deposit = *(long*)p;
		p += sizeof(long);
	}

	// 8: pre task
	if ((1 << token_count++) & token_mask1)
	{
		m_ulPremise_Task_Count = *p;
		p++;

		size_t sz = sizeof(long) * m_ulPremise_Task_Count;
		memcpy(m_ulPremise_Tasks, p, sz);
		p += sz;
	}

	// 9: gender
	if ((1 << token_count++) & token_mask1)
	{
		m_ulGender = *p;
		p++;
	}

	// 10: occupation
	if ((1 << token_count++) & token_mask1)
	{
		m_ulOccupations = *p;
		p++;

		size_t sz = sizeof(long) * m_ulOccupations;
		memcpy(m_Occupations, p, sz);
		p += sz;
	}

	// 11: mutex task
	if ((1 << token_count++) & token_mask1)
	{
		m_ulMutexTaskCount = *p;
		p++;

		size_t sz = sizeof(long) * m_ulMutexTaskCount;
		memcpy(m_ulMutexTasks, p, sz);
		p += sz;
	}

	// 12: time table
	if ((1 << token_count++) & token_mask1)
	{
		m_ulTimetable = *p;
		p++;

		m_tmStart = new task_tm[m_ulTimetable];
		g_ulNewCount++;

		m_tmEnd = new task_tm[m_ulTimetable];
		g_ulNewCount++;

		size_t sz;

		sz = sizeof(char) * m_ulTimetable;

		memcpy(m_tmType, p, sz);
		p += sz;

		sz = sizeof(task_tm) * m_ulTimetable;

		memcpy(m_tmStart, p, sz);
		p += sz;

		memcpy(m_tmEnd, p, sz);
		p += sz;
	}

	// method
	m_enumMethod = *(unsigned char*)p;
	p++;

	switch (m_enumMethod)
	{
	case enumTMKillNumMonster:
		p += UnmarshalKillMonster(p);
		break;
	case enumTMCollectNumArticle:
		p += UnmarshalCollectItems(p);
		break;
	case enumTMReachSite:
		m_ulReachSiteId = *(long*)p;
		p += sizeof(long);
		memcpy(&m_ReachSiteMin, p, sizeof(ZONE_VERT));
		p += sizeof(ZONE_VERT);
		memcpy(&m_ReachSiteMax, p, sizeof(ZONE_VERT));
		p += sizeof(ZONE_VERT);
		break;
	case enumTMLeaveSite:
		m_ulLeaveSiteId = *(long*)p;
		p += sizeof(long);
		memcpy(&m_LeaveSiteMin, p, sizeof(ZONE_VERT));
		p += sizeof(ZONE_VERT);
		memcpy(&m_LeaveSiteMax, p, sizeof(ZONE_VERT));
		p += sizeof(ZONE_VERT);
		break;
	case enumTMWaitTime:
		m_ulWaitTime = *(long*)p;
		p += sizeof(long);
		break;
	}

	// finish type
	m_enumFinishType = *(unsigned char*)p;
	p++;

	// award
	p += m_Award_S->UnmarshalBasicData(p);

	// talks
	p += unmarshal_str(p, m_pwstrDescript);
	p += unmarshal_str(p, m_pwstrOkText);
	p += unmarshal_str(p, m_pwstrNoText);
	p += unmarshal_talk_proc(&m_DelvTaskTalk, p);
	p += unmarshal_talk_proc(&m_UnqualifiedTalk, p);
	p += unmarshal_talk_proc(&m_DelvItemTalk, p);
	p += unmarshal_talk_proc(&m_ExeTalk, p);
	p += unmarshal_talk_proc(&m_AwardTalk, p);

#ifndef _TASK_CLIENT
	CheckMask();
#endif

	m_nSubCount = *(int*)p;
	p += sizeof(int);

	for (int i = 0; i < m_nSubCount; i++)
	{
		ATaskTempl* pSub = new ATaskTempl;
		g_ulNewCount++;
		AddSubTaskTempl(pSub);
		p += pSub->UnmarshalDynTask(p);
	}

	SynchID();
	if (!m_pParent) CheckDepth();

	return p - pData;
}

int ATaskTempl::UnmarshalSpecialAwardData(const char* pData)
{
	const char* p = pData;
	m_ulSpecialAward = *(long*)p;
	p += sizeof(long);

	return p - pData;
}

bool ATaskTempl::Load(FILE* fp, unsigned long ulVersion, bool bTextFile)
{
	char line[TASK_MAX_LINE_LEN];

	Init();

	if (!LoadFixedDataFromTextFile(fp, ulVersion)
	 || !LoadDescription(fp)) return false;

	if (ulVersion >= 9 && !LoadTribute(fp))
		return false;

	if (ulVersion >= 128 && !LoadHintText(fp))
		return false;

	if (ulVersion >= 137 && !LoadCanDeliverText(fp))
		return false;

#ifndef _TASK_CLIENT
	CheckMask();
#endif

	if (ulVersion >= 6)
	{
		if (m_DelvTaskTalk.load(fp) != 0)
			return false;

		if (ulVersion >= 15 && m_UnqualifiedTalk.load(fp) != 0)
			return false;

		if (m_DelvItemTalk.load(fp) != 0)
			return false;

		if (m_ExeTalk.load(fp) != 0)
			return false;

		if (m_AwardTalk.load(fp) != 0)
			return false;
	}


#ifdef TASK_TEMPL_EDITOR
	// calculate some default values
	if( ulVersion < 144 && HasSuccAward())
	{
		// 搜索奖励
		for(int i=0;i<m_Award_S->m_ulCandItems;i++)
		{
			if(!m_Award_S->m_CandItems)
				break;
			AWARD_ITEMS_CAND* pCand = &m_Award_S->m_CandItems[i];

			for(int j=0;j<pCand->m_ulAwardItems;j++)
			{	
				if(!pCand->m_AwardItems)
					break;

				ITEM_WANTED* pItem = &pCand->m_AwardItems[j];
				unsigned long itemID = pItem->m_ulItemTemplId;

				if(itemID == 0)
					continue;
				else if(itemID == 13425 || m_Award_S->m_ulExp)	// 经验
					m_ulRecommendType |= (1<<RECOMMEND_TYPE_EXP);
				else if(itemID == 19074 || m_Award_S->m_ulGoldNum)	// 金钱
					m_ulRecommendType |= (1<<RECOMMEND_TYPE_MONEY);
				else if(itemID == 21202 || itemID == 21345 || itemID == 22343)	// 造化
					m_ulRecommendType |= (1<<RECOMMEND_TYPE_CULTIVATION);
				else if(itemID == 41668 || itemID == 41669)	// 元神
					m_ulRecommendType |= (1<<RECOMMEND_TYPE_DEITY);
				else if(itemID == 23360 || itemID == 19252 || itemID == 19253 || itemID == 21153)	// 特殊奖励
					m_ulRecommendType |= (1<<RECOMMEND_TYPE_SPECIAL);
		

				if(m_Award_S->m_lTitle)	// 称号
					m_ulRecommendType |= (1<<RECOMMEND_TYPE_TITLE);
			}
		}
	}

#endif

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_task_count, &m_nSubCount);

	for (int i = 0; i < m_nSubCount; i++)
	{
		ATaskTempl* pSub = new ATaskTempl;
		g_ulNewCount++;
		AddSubTaskTempl(pSub);
		if (!pSub->Load(fp, ulVersion, bTextFile))
		{
			LOG_DELETE(pSub);
			return false;
		}
	}

	SynchID();
	return true;
}

bool ATaskTempl::LoadBinary(FILE* fp)
{
	LoadFixedDataFromBinFile(fp);
	LoadDescriptionBin(fp);
	LoadTributeBin(fp);
	LoadHintTextBin(fp);
	LoadCanDeliverTextBin(fp);

#ifndef _TASK_CLIENT
	CheckMask();
#endif

	m_DelvTaskTalk.load(fp);
	m_UnqualifiedTalk.load(fp);
	m_DelvItemTalk.load(fp);
	m_ExeTalk.load(fp);
	m_AwardTalk.load(fp);

	fread(&m_nSubCount, sizeof(m_nSubCount), 1, fp);

	for (int i = 0; i < m_nSubCount; i++)
	{
		ATaskTempl* pSub = new ATaskTempl;
		g_ulNewCount++;
		AddSubTaskTempl(pSub);
		pSub->LoadBinary(fp);
	}

	SynchID();
	return true;
}

bool ATaskTempl::LoadDescription(FILE* fp)
{
	size_t len = 0;
	char line[TASK_MAX_LINE_LEN];

	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_descpt_len, &len);

	m_pwstrDescript = new task_char[len+1];
	m_pwstrDescript[len] = L'\0';
	g_ulNewCount++;

	if (len)
	{
		fread(m_pwstrDescript, sizeof(task_char), len, fp);
		ReadLine(fp, line);
	}

	len = 0;
	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_descpt_len, &len);

	m_pwstrOkText = new task_char[len+1];
	m_pwstrOkText[len] = L'\0';
	g_ulNewCount++;

	if (len)
	{
		fread(m_pwstrOkText, sizeof(task_char), len, fp);
		ReadLine(fp, line);
	}

	len = 0;
	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_descpt_len, &len);

	m_pwstrNoText = new task_char[len+1];
	m_pwstrNoText[len] = L'\0';
	g_ulNewCount++;

	if (len)
	{
		fread(m_pwstrNoText, sizeof(task_char), len, fp);
		ReadLine(fp, line);
	}

	return true;
}

bool ATaskTempl::LoadDescriptionBin(FILE* fp)
{
	size_t len = 0;

	fread(&len, sizeof(len), 1, fp);
	m_pwstrDescript = new task_char[len+1];
	m_pwstrDescript[len] = L'\0';
	if (len) fread(m_pwstrDescript, sizeof(task_char), len, fp);
	g_ulNewCount++;

	len = 0;
	fread(&len, sizeof(len), 1, fp);
	m_pwstrOkText = new task_char[len+1];
	m_pwstrOkText[len] = L'\0';
	if (len) fread(m_pwstrOkText, sizeof(task_char), len, fp);
	g_ulNewCount++;

	len = 0;
	fread(&len, sizeof(len), 1, fp);
	m_pwstrNoText = new task_char[len+1];
	m_pwstrNoText[len] = L'\0';
	if (len) fread(m_pwstrNoText, sizeof(task_char), len, fp);
	g_ulNewCount++;

	return true;
}

bool ATaskTempl::LoadTribute(FILE* fp)
{
	size_t len;
	char line[TASK_MAX_LINE_LEN];

	len = 0;
	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_tribute_len, &len);

	m_pwstrTribute = new task_char[len+1];
	m_pwstrTribute[len] = L'\0';
	g_ulNewCount++;

	if (len)
	{
		fread(m_pwstrTribute, sizeof(task_char), len, fp);
		ReadLine(fp, line);
	}

	return true;
}

bool ATaskTempl::LoadTributeBin(FILE* fp)
{
	size_t len;
	
	len = 0;
	fread(&len, sizeof(len), 1, fp);
	m_pwstrTribute = new task_char[len+1];
	m_pwstrTribute[len] = L'\0';
	if (len) fread(m_pwstrTribute, sizeof(task_char), len, fp);
	g_ulNewCount++;

	return true;
}

bool ATaskTempl::LoadHintText(FILE* fp)
{
	size_t len;
	char line[TASK_MAX_LINE_LEN];

	len = 0;
	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_hint_len, &len);

	m_pwstrHintText = new task_char[len+1];
	m_pwstrHintText[len] = L'\0';
	g_ulNewCount++;

	if (len)
	{
		fread(m_pwstrHintText, sizeof(task_char), len, fp);
		ReadLine(fp, line);
	}

	return true;
}

bool ATaskTempl::LoadHintTextBin(FILE* fp)
{
	size_t len;
	
	len = 0;
	fread(&len, sizeof(len), 1, fp);
	m_pwstrHintText = new task_char[len+1];
	m_pwstrHintText[len] = L'\0';
	if (len) fread(m_pwstrHintText, sizeof(task_char), len, fp);
	g_ulNewCount++;

	return true;
}

bool ATaskTempl::LoadCanDeliverText(FILE* fp)
{
	size_t len;
	char line[TASK_MAX_LINE_LEN];

	len = 0;
	if (!ReadLine(fp, line)) return false;
	sscanf(line, _format_can_deliver_len, &len);

	m_pwstrCanDeliverText = new task_char[len+1];
	m_pwstrCanDeliverText[len] = L'\0';
	g_ulNewCount++;

	if (len)
	{
		fread(m_pwstrCanDeliverText, sizeof(task_char), len, fp);
		ReadLine(fp, line);
	}

	return true;
}

bool ATaskTempl::LoadCanDeliverTextBin(FILE* fp)
{
	size_t len;
	
	len = 0;
	fread(&len, sizeof(len), 1, fp);
	m_pwstrCanDeliverText = new task_char[len+1];
	m_pwstrCanDeliverText[len] = L'\0';
	if (len) fread(m_pwstrCanDeliverText, sizeof(task_char), len, fp);
	g_ulNewCount++;

	return true;
}

#ifdef _TASK_CLIENT

void ATaskTempl::SaveDescription(FILE* fp)
{
	size_t len;

	len = m_pwstrDescript ? wcslen(m_pwstrDescript) : 0;
	fprintf(fp, _format_descpt_len, len);
	fprintf(fp, "\r\n");

	if (len)
	{
		fwrite(m_pwstrDescript, sizeof(task_char), len, fp);
		fprintf(fp, "\r\n");
	}

	len = m_pwstrOkText ? wcslen(m_pwstrOkText) : 0;
	fprintf(fp, _format_descpt_len, len);
	fprintf(fp, "\r\n");

	if (len)
	{
		fwrite(m_pwstrOkText, sizeof(task_char), len, fp);
		fprintf(fp, "\r\n");
	}

	len = m_pwstrNoText ? wcslen(m_pwstrNoText) : 0;
	fprintf(fp, _format_descpt_len, len);
	fprintf(fp, "\r\n");

	if (len)
	{
		fwrite(m_pwstrNoText, sizeof(task_char), len, fp);
		fprintf(fp, "\r\n");
	}
}

void ATaskTempl::SaveDescriptionBin(FILE* fp)
{
	size_t len;

	len = m_pwstrDescript ? wcslen(m_pwstrDescript) : 0;
	fwrite(&len, sizeof(len), 1, fp);
	if (len) fwrite(m_pwstrDescript, sizeof(task_char), len, fp);

	len = m_pwstrOkText ? wcslen(m_pwstrOkText) : 0;
	fwrite(&len, sizeof(len), 1, fp);
	if (len) fwrite(m_pwstrOkText, sizeof(task_char), len, fp);

	len = m_pwstrNoText ? wcslen(m_pwstrNoText) : 0;
	fwrite(&len, sizeof(len), 1, fp);
	if (len) fwrite(m_pwstrNoText, sizeof(task_char), len, fp);
}

void ATaskTempl::SaveTribute(FILE* fp)
{
	size_t len;

	len = m_pwstrTribute ? wcslen(m_pwstrTribute) : 0;
	fprintf(fp, _format_tribute_len, len);
	fprintf(fp, "\r\n");

	if (len)
	{
		fwrite(m_pwstrTribute, sizeof(task_char), len, fp);
		fprintf(fp, "\r\n");
	}
}

void ATaskTempl::SaveTributeBin(FILE* fp)
{
	size_t len;

	len = m_pwstrTribute ? wcslen(m_pwstrTribute) : 0;
	fwrite(&len, sizeof(len), 1, fp);
	if (len) fwrite(m_pwstrTribute, sizeof(task_char), len, fp);
}

void ATaskTempl::SaveHintText(FILE* fp)
{
	size_t len;

	len = m_pwstrHintText ? wcslen(m_pwstrHintText) : 0;
	fprintf(fp, _format_hint_len, len);
	fprintf(fp, "\r\n");

	if (len)
	{
		fwrite(m_pwstrHintText, sizeof(task_char), len, fp);
		fprintf(fp, "\r\n");
	}
}

void ATaskTempl::SaveHintTextBin(FILE* fp)
{
	size_t len;

	len = m_pwstrHintText ? wcslen(m_pwstrHintText) : 0;
	fwrite(&len, sizeof(len), 1, fp);
	if (len) fwrite(m_pwstrHintText, sizeof(task_char), len, fp);
}

void ATaskTempl::SaveCanDeliverText(FILE* fp)
{
	size_t len;

	len = m_pwstrCanDeliverText ? wcslen(m_pwstrCanDeliverText) : 0;
	fprintf(fp, _format_can_deliver_len, len);
	fprintf(fp, "\r\n");

	if (len)
	{
		fwrite(m_pwstrCanDeliverText, sizeof(task_char), len, fp);
		fprintf(fp, "\r\n");
	}
}

void ATaskTempl::SaveCanDeliverTextBin(FILE* fp)
{
	size_t len;

	len = m_pwstrCanDeliverText ? wcslen(m_pwstrCanDeliverText) : 0;
	fwrite(&len, sizeof(len), 1, fp);
	if (len) fwrite(m_pwstrCanDeliverText, sizeof(task_char), len, fp);
}

#include "windows.h"
#define CODE_PAGE CP_ACP

class CSafeString
{
	LPSTR m_szBuf;
	LPWSTR m_wszBuf;

public:
	CSafeString(LPCSTR lpsz, int nLen) : m_wszBuf(NULL)
	{
		assert(lpsz);
		if (nLen < 0) nLen = strlen(lpsz);
		m_szBuf = new char[nLen+1];
		strncpy(m_szBuf, lpsz, nLen);
		m_szBuf[nLen] = '\0';
	}

	CSafeString(LPCSTR lpsz) : m_wszBuf(NULL)
	{
		assert(lpsz);
		int n = strlen(lpsz);
		m_szBuf = new char[n+1];
		strcpy(m_szBuf, lpsz);
	}
	
	CSafeString(LPCWSTR lpwsz, int nLen) : m_szBuf(NULL)
	{
		assert(lpwsz);
		if (nLen < 0) nLen = wcslen(lpwsz);
		m_wszBuf = new wchar_t[nLen+1];
		wcsncpy(m_wszBuf, lpwsz, nLen);
		m_wszBuf[nLen] = L'\0';
	}

	CSafeString(LPCWSTR lpwsz) : m_szBuf(NULL)
	{
		assert(lpwsz);
		int n = wcslen(lpwsz);
		m_wszBuf = new wchar_t[n+1];
		wcscpy(m_wszBuf, lpwsz);
	}

	operator LPCSTR() { return GetAnsi();}
	operator LPCWSTR() { return GetUnicode(); }
	
	CSafeString& operator= (const CSafeString& str)
	{
		delete[] m_szBuf;
		delete[] m_wszBuf;

		m_szBuf = NULL;
		m_wszBuf = NULL;

		if (str.m_szBuf)
		{
			m_szBuf = new char[strlen(str.m_szBuf)+1];
			strcpy(m_szBuf, str.m_szBuf);
		}

		if (str.m_wszBuf)
		{
			m_wszBuf = new wchar_t[wcslen(str.m_wszBuf)+1];
			wcscpy(m_wszBuf, str.m_wszBuf);
		}
		return *this;
	}

	LPCSTR GetAnsi()
	{
		if (m_szBuf) return m_szBuf;
		assert(m_wszBuf);
		
		int nCount = WideCharToMultiByte(
			CODE_PAGE,
			0,
			m_wszBuf,
			-1,
			NULL,
			0,
			NULL,
			NULL);
	
		m_szBuf = new char[nCount];

		WideCharToMultiByte(
			CODE_PAGE,
			0,
			m_wszBuf,
			-1,
			m_szBuf,
			nCount,
			NULL,
			NULL);
		return m_szBuf;
	}
	
	LPCWSTR GetUnicode()
	{
		if (m_wszBuf) return m_wszBuf;
		assert(m_szBuf);
		
		int nCount = MultiByteToWideChar(
			CODE_PAGE,
			0,
			m_szBuf,
			-1,
			NULL,
			0);

		m_wszBuf = new wchar_t[nCount];

		MultiByteToWideChar(
			CODE_PAGE,
			0,
			m_szBuf,
			-1,
			m_wszBuf,
			nCount);

		return m_wszBuf;
	}

	virtual ~CSafeString()
	{
		delete[] m_szBuf;
		delete[] m_wszBuf;
	}
};

void save_talk_text(FILE* fp, const talk_proc* pTalk)
{
	for (int i = 0; i < pTalk->num_window; i++)
	{
		talk_proc::window* win = &pTalk->windows[i];

		fprintf(fp, "【窗口提示文字】\r\n");
		fprintf(fp, CSafeString(win->talk_text));
		fprintf(fp, "\r\n");

		for (int j = 0; j < win->num_option; j++)
		{
			fprintf(fp, "【选项提示文字】\r\n");
			fprintf(fp, CSafeString(win->options[j].text));
			fprintf(fp, "\r\n");
		}
	}
}

void ATaskTempl::SaveAllText(FILE* fp)
{
	fprintf(fp, "【任务名称】\r\n");
	fprintf(fp, CSafeString(m_szName));
	fprintf(fp, "\r\n");

	fprintf(fp, "【任务描述】\r\n");
	fprintf(fp, CSafeString(m_pwstrDescript));
	fprintf(fp, "\r\n");

	fprintf(fp, "【发放对话】\r\n");
	save_talk_text(fp, &m_DelvTaskTalk);
	fprintf(fp, "\r\n");

	fprintf(fp, "【不符合条件对话】\r\n");
	save_talk_text(fp, &m_UnqualifiedTalk);
	fprintf(fp, "\r\n");

	fprintf(fp, "【未完成对话】\r\n");
	save_talk_text(fp, &m_ExeTalk);
	fprintf(fp, "\r\n");

	fprintf(fp, "【完成对话】\r\n");
	save_talk_text(fp, &m_AwardTalk);
	fprintf(fp, "\r\n");

	ATaskTempl* pChild = m_pFirstChild;
	while (pChild)
	{
		pChild->SaveAllText(fp);
		pChild = pChild->m_pNextSibling;
	}
}

inline void SaveAwardCand(const AWARD_ITEMS_CAND& ic, FILE* fp)
{
	unsigned long i;

	fprintf(fp, _format_rand_one, ic.m_bRandChoose);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_item_wanted_num, ic.m_ulAwardItems);
	fprintf(fp, "\r\n");

	for (i = 0; i < ic.m_ulAwardItems; i++)
	{
		const ITEM_WANTED& iw = ic.m_AwardItems[i];

		fprintf(fp, _format_item_templ, iw.m_ulItemTemplId);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_item_is_cmn, iw.m_bCommonItem);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_item_num, iw.m_ulItemNum);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_prob, iw.m_fProb);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_bind, iw.m_bBind);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_period, iw.m_lPeriod);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_time_point, iw.m_bTimetable);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_tp_week, iw.m_cDayOfWeek, iw.m_cHour, iw.m_cMinute);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_item_refine_cond, iw.m_cRefineCond);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_item_refine_level, iw.m_ulRefineLevel);
		fprintf(fp, "\r\n");		
	}
}

inline void SaveAwardCandBin(const AWARD_ITEMS_CAND& ic, FILE* fp)
{
	unsigned long i;

	fwrite(&ic.m_bRandChoose, sizeof(ic.m_bRandChoose), 1, fp);
	fwrite(&ic.m_ulAwardItems, sizeof(ic.m_AwardItems), 1, fp);

	for (i = 0; i < ic.m_ulAwardItems; i++)
	{
		const ITEM_WANTED& iw = ic.m_AwardItems[i];
		fwrite(&iw, sizeof(iw), 1, fp);
	}
}

inline void SaveAwardData(AWARD_DATA& ad, FILE* fp)
{
	unsigned long i;

	fprintf(fp, _format_award_gold_num, ad.m_ulGoldNum);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_exp_num, ad.m_ulExp);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_cands_num, ad.m_ulCandItems);
	fprintf(fp, "\r\n");

	for (i = 0; i < ad.m_ulCandItems; i++)
		SaveAwardCand(
			ad.m_CandItems[i],
			fp
		);

	fprintf(fp, _format_award_sp_num, ad.m_ulSP);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_repu, ad.m_ulReputation);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_title, ad.m_lTitle);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_contrib, ad.m_lContribution);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_faction_gold_note, ad.m_iFactionGoldNote);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_faction_grass, ad.m_iFactionGrass);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_faction_mine, ad.m_iFactionMine);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_faction_monster_core, ad.m_iFactionMonsterCore);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_faction_monster_food, ad.m_iFactionMonsterFood);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_faction_money, ad.m_iFactionMoney);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_faction_building_progress, ad.m_bBuildingProgress);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_faction_extra_gold_note, ad.m_iFactionExtraGoldNote);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_faction_extra_grass, ad.m_iFactionExtraGrass);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_faction_extra_mine, ad.m_iFactionExtraMine);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_faction_extra_monster_core, ad.m_iFactionExtraMonsterCore);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_faction_extra_monster_food, ad.m_iFactionExtraMonsterFood);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_faction_extra_money, ad.m_iFactionExtraMoney);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_travel_item, ad.m_iTravelItemId);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_travel_time, ad.m_iTravelTime);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_travel_speed, ad.m_fTravelSpeed);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_travel_path, ad.m_iTravelPath);
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_award_contrib, ad.m_nFamContrib);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_prosp, ad.m_ulProsperity);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_pk_value, ad.m_lPKValue);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_reset_pk, ad.m_bResetPKValue);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_divorce, ad.m_bDivorce);
	fprintf(fp, "\r\n");

	for (i = 0; i < SIZE_OF_ARRAY(ad.m_aFriendships); i++)
	{
		fprintf(fp, _format_friendship, ad.m_aFriendships[i]);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_friendship_reset, ad.m_ulFriendshipResetSel);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_period, ad.m_ulNewPeriod);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_station, ad.m_ulNewRelayStation);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_store, ad.m_ulStorehouseSize);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_store, ad.m_ulFactionStorehouseSize);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_invent, ad.m_lInventorySize);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_pocket, ad.m_lPocketSize);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_pet, ad.m_ulPetInventorySize);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_mount, ad.m_ulMountInventorySize);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_fury, ad.m_ulFuryULimit);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_skill_lev, ad.m_bSetProduceSkill);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_skill_exp, ad.m_ulProduceSkillExp);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_prof, ad.m_ulNewProfession);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_lifeag_cnt, ad.m_ulLifeAgainCnt);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_lifeag_pro, ad.m_ulLifeAgainFaction);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_world_id, ad.m_ulTransWldId);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_zone_vert, VERT_ZONE(ad.m_TransPt));
	fprintf(fp, "\r\n");

	fprintf(fp, _format_controller, ad.m_lMonsCtrl);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_trig_flag, ad.m_bTrigCtrl);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_ctrl_cnt, ad.m_ulMonCtrlCnt);
	fprintf(fp, "\r\n");
	
	for (i = 0; i <ad.m_ulMonCtrlCnt; i++)
	{
		fprintf(fp, _format_award_ctrl_id, ad.m_MonCtrl[i].lMonCtrlID);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_award_ctrl_prob, ad.m_MonCtrl[i].fGetProb);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_award_ctrl_open, ad.m_MonCtrl[i].bOpenClose);
		fprintf(fp, "\r\n");
	}
	
	fprintf(fp, _format_award_ctrl_rand, ad.m_bRanMonCtrl);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_new_task, ad.m_ulNewTask);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_ter_task_cnt, ad.m_ulTerminateTaskCnt);
	fprintf(fp, "\r\n");

	for(i=0; i<ad.m_ulTerminateTaskCnt; i++)
	{
		fprintf(fp, _format_award_ter_task_id, ad.m_ulTerminateTask[i]);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_award_cam_move, ad.m_ulCameraMove);
	fprintf(fp, "\r\n");


	fprintf(fp, _format_award_animation, ad.m_ulAnimation);
	fprintf(fp, "\r\n");


	fprintf(fp, _format_award_circle_group_pt, ad.m_ulCircleGroupPoint);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_buffid, ad.m_lBuffId);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_bufflev, ad.m_lBuffLev);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_f_skill_prof, ad.m_nFamilySkillProficiency);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_f_skill_lev, ad.m_nFamilySkillLevel);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_f_skill_index, ad.m_nFamilySkillIndex);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_record_index, ad.m_nFamilyMonRecordIndex);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_f_value_index, ad.m_nFamilyValueIndex);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_f_value, ad.m_nFamilyValue);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_msg, ad.m_bSendMsg);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_channel, ad.m_nMsgChannel);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_id, ad.m_ulClearCountTask);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_double_exp_time, ad.m_ulDoubleExpTime);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_special_award_type, ad.m_ulSpecialAwardType);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_special_award_id, ad.m_ulSpecialAwardID);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_m_moral, ad.m_lMasterMoral);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_out, ad.m_bOutMaster);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_deviate, ad.m_bDeviateMaster);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_role, ad.m_bAwardSpecifyRole);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_role_Sel, ad.m_ulRoleSelected);
	fprintf(fp, "\r\n");

	if (ad.m_bAwardSpecifyRole && ad.m_ulRoleSelected)
	{	
		if (ad.m_pAwardSpecifyRole == NULL)
		{
			ad.m_pAwardSpecifyRole = new AWARD_DATA;
		}

		fprintf(fp, _format_award_gold_num, ad.m_pAwardSpecifyRole->m_ulGoldNum);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_award_exp_num, ad.m_pAwardSpecifyRole->m_ulExp);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_award_sp_num, ad.m_pAwardSpecifyRole->m_ulSP);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_award_repu, ad.m_pAwardSpecifyRole->m_ulReputation);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_award_m_moral, ad.m_pAwardSpecifyRole->m_lMasterMoral);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_award_m_moral, ad.m_pAwardSpecifyRole->m_bOutMasterMoral);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_award_title, ad.m_pAwardSpecifyRole->m_lTitle);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_award_cands_num, ad.m_pAwardSpecifyRole->m_ulCandItems);
		fprintf(fp, "\r\n");
		
		for (i = 0; i < ad.m_pAwardSpecifyRole->m_ulCandItems; i++)
			SaveAwardCand(
			ad.m_pAwardSpecifyRole->m_CandItems[i],
			fp
			);
	}

	fprintf(fp, _format_award_set_cult, ad.m_bSetCult);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_set_cult, ad.m_ulSetCult);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_clearskill, ad.m_ulClearCultSkill);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_exp_coef, ad.m_fExpCoef);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_exp_coef, ad.m_fExpCoef2);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_exp_coef, ad.m_fExpCoef3);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_o_m_exp, ad.m_bOutMasterExp);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_o_m_moral, ad.m_bOutMasterMoral);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_cl_sk_p, ad.m_bClearSkillPoints);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_cl_bo_p, ad.m_bClearBookPoints);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_exp_sel, ad.m_nParaExpSel);
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_award_exp_size, ad.m_ulParaExpSize);
	fprintf(fp, "\r\n");
	
	if (ad.m_ulParaExpSize)
	{
		fwrite(ad.m_pszParaExp, 1, MAX_TASK_EXP_LEN, fp);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_change_key_size, ad.m_lChangeKeyArr.size());
	fprintf(fp, "\r\n");
	
	for (i=0; i<ad.m_lChangeKeyArr.size(); i++)
	{
		fprintf(fp, _format_change_key, ad.m_lChangeKeyArr[i]);
		fprintf(fp, "\r\n");		
		
		fprintf(fp, _format_change_kv, ad.m_lChangeKeyValueArr[i]);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_change_typ, ad.m_bChangeTypeArr[i]);
		fprintf(fp, "\r\n");		
	}

	fprintf(fp, _format_multi_global_key, ad.m_lMultiGlobalKey);
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_battle_score, ad.m_nSJBattleScore);
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_award_reset_sj, ad.m_bResetSJBattleScore);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_clear_nkat, ad.m_bClearNoKeyActiveTask);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_transform_id, ad.m_iTransformID);
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_transform_dur, ad.m_iTransformDuration);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_transform_lev, ad.m_iTransformLevel);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_transform_lev, ad.m_iTransformExpLevel);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_transform_cover, ad.m_bTransformCover);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_fengshen_exp, ad.m_ulFengshenExp);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_fengshen_dujie, ad.m_bFengshenDujie);
	fprintf(fp, "\r\n");

	//Added 2012-04-18.
	fprintf(fp, _format_award_open_soul_equip, ad.m_bOpenSoulEquip);
	fprintf(fp, "\r\n");

	//Added 2011-02-18.
	fprintf(fp, _format_award_bonus_num, ad.m_ulBonusNum);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_battle_field_reputaion, ad.m_ulBattleScore);
	fprintf(fp, "\r\n");

	//Added 2011-03-08.
	fprintf(fp, _format_award_check_fnsh_global_count, ad.m_bCheckGlobalFinishCount);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_global_fnsh_count_precondition, ad.m_ulGlobalFinishCountPrecondition);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_check_global_comp_expression, ad.m_bCheckGlobalCompareExpression);
	fprintf(fp, "\r\n");
	
	if( ad.m_bCheckGlobalCompareExpression )
	{
		fprintf(fp, _format_left_string, &ad.m_GlobalCompareExpression.strExpLeft[0]);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_comp_oper, ad.m_GlobalCompareExpression.nCompOper);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_right_string, &ad.m_GlobalCompareExpression.strExpRight[0]);
		fprintf(fp, "\r\n");
	}


	fprintf(fp, _format_award_faction_extra_cands_num, ad.m_ulFactionExtraCandItems);
	fprintf(fp, "\r\n");

	for(i = 0; i < ad.m_ulFactionExtraCandItems; i++)
	{
		SaveAwardCand(ad.m_FactionExtraCandItems[i], fp);
	}

	//Added 2011-03-04.
	//保存额外奖励的物品信息
	fprintf(fp, _format_award_extra_cands_num, ad.m_ulExtraCandItems);
	fprintf(fp, "\r\n");

	for (i = 0; i < ad.m_ulExtraCandItems; i++)
	{
		SaveAwardCand(
			ad.m_ExtraCandItems[i],
			fp
		);
	}

	//Added 2011-03-07.
	fprintf(fp, _format_award_extra_ctrl_cnt, ad.m_ulExtraMonCtrlCnt);
	fprintf(fp, "\r\n");
	
	for (i = 0; i <ad.m_ulExtraMonCtrlCnt; i++)
	{
		fprintf(fp, _format_award_extra_ctrl_id, ad.m_ExtraMonCtrl[i].lMonCtrlID);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_award_extra_ctrl_prob, ad.m_ExtraMonCtrl[i].fGetProb);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_award_extra_ctrl_open, ad.m_ExtraMonCtrl[i].bOpenClose);
		fprintf(fp, "\r\n");
	}
	
	fprintf(fp, _format_award_extra_ctrl_rand, ad.m_bExtraRanMonCtrl);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_msg, ad.m_bSendExtraMsg);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_channel, ad.m_nExtraMsgChannel);
	fprintf(fp, "\r\n");

	//保存额外奖励的系统喊话内容
	size_t len;
	len = ad.m_pwstrExtraTribute ? wcslen(ad.m_pwstrExtraTribute) : 0;
	fprintf(fp, _format_tribute_len, len);
	fprintf(fp, "\r\n");

	if (len)
	{
		fwrite(ad.m_pwstrExtraTribute, sizeof(task_char), len, fp);
		fprintf(fp, "\r\n");
	}

	//Added 2011-03-30
	int nCurIndex = 0;
	for( ; nCurIndex < MAX_VARIABLE_NUM; ++nCurIndex )
	{
		fprintf(fp, _format_award_variable, ad.m_lVariable[nCurIndex]);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_award_king_score, ad.m_nKingScore);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_clear_xp_cd, ad.m_bClearXpCD);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_Phase_cnt, ad.m_iPhaseCnt);
	fprintf(fp, "\r\n");

	for(i = 0; i < ad.m_iPhaseCnt; i++)
	{
		fprintf(fp, _format_award_Phase_id, ad.m_Phase[i].iPhaseID);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_award_Phase_open, ad.m_Phase[i].bOpenClose);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_award_auction_cnt, ad.m_iAuctionCnt);
	fprintf(fp, "\r\n");

		for(i = 0; i < ad.m_iAuctionCnt; i++)
	{
		fprintf(fp, _format_award_auction_id, ad.m_Auction[i].m_ulAuctionItemId);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_award_auction_prob, ad.m_Auction[i].m_fAuctionProp);
		fprintf(fp, "\r\n");

	}

}

inline void SaveAwardDataBin(AWARD_DATA& ad, FILE* fp)
{
	unsigned long i;

	AWARD_DATA* p = (AWARD_DATA*)&ad;
	AWARD_ITEMS_CAND* tmp = p->m_CandItems;
	p->m_CandItems = NULL;

	char *pszExp = p->m_pszParaExp;
	TASK_EXPRESSION *pExpArr = p->m_pParaExpArr;
	p->m_pszParaExp = NULL;
	p->m_pParaExpArr = NULL;

	AWARD_DATA *tmp2 = p->m_pAwardSpecifyRole;
	p->m_pAwardSpecifyRole = NULL;

	AWARD_ITEMS_CAND* tmpFactionExtraCandItems = p->m_FactionExtraCandItems;
	p->m_FactionExtraCandItems = NULL;

	//Added 2011-03-04.
	AWARD_ITEMS_CAND* tmpExtraCandItems = p->m_ExtraCandItems;
	p->m_ExtraCandItems = NULL;

	//Added 2011-03-07.
	task_char* pwstrExtraTribute = p->m_pwstrExtraTribute;
	p->m_pwstrExtraTribute = NULL;

	fwrite(&ad, sizeof(ad), 1, fp);
	p->m_CandItems = tmp;
	p->m_pAwardSpecifyRole = tmp2;

	p->m_pszParaExp = pszExp;
	p->m_pParaExpArr = pExpArr;	

	for (i = 0; i < ad.m_ulCandItems; i++)
		SaveAwardCandBin(
			ad.m_CandItems[i],
			fp
		);

	if (p->m_pAwardSpecifyRole)
		SaveAwardDataBin(*(p->m_pAwardSpecifyRole), fp);
	
	if (ad.m_ulParaExpSize)
	{
		fwrite(ad.m_pszParaExp, 1, ad.m_ulParaExpSize, fp);
		
		//存vector				
		TaskExpAnalyser Analyser;
		TaskExpressionArr tmpArr;
		
		Analyser.Analyse(ad.m_pszParaExp, tmpArr);
		ad.m_pParaExpArr = new TASK_EXPRESSION[tmpArr.size()];
		g_ulNewCount++;
		
		for (i = 0; i < tmpArr.size(); i++)
		{
			ad.m_pParaExpArr[i].type  = tmpArr[i].type;
			ad.m_pParaExpArr[i].value = tmpArr[i].value;
		}
		ad.m_ulParaExpArrLen = tmpArr.size();
		
		fwrite(&ad.m_ulParaExpArrLen, sizeof(unsigned long), 1, fp);				
		fwrite(ad.m_pParaExpArr, sizeof(TASK_EXPRESSION), tmpArr.size(), fp);
	}	

	if (ad.m_lChangeKeyArr.size())
	{
		fwrite(&ad.m_lChangeKeyArr[0], sizeof(long), ad.m_lChangeKeyArr.size(), fp);
		fwrite(&ad.m_lChangeKeyValueArr[0], sizeof(long), ad.m_lChangeKeyValueArr.size(), fp);
		fwrite(&ad.m_bChangeTypeArr[0], sizeof(bool), ad.m_bChangeTypeArr.size(), fp);
	}

	//保存全局key/value表达式Added 2011-03-08.
	if( ad.m_bCheckGlobalCompareExpression )
	{
		fwrite(&ad.m_GlobalCompareExpression.strExpLeft[0], sizeof(char), ad.m_GlobalCompareExpression.strExpLeft.size(), fp);
		fwrite(&ad.m_GlobalCompareExpression.strExpRight[0], sizeof(char), ad.m_GlobalCompareExpression.strExpRight.size(), fp);
	}

	p->m_FactionExtraCandItems = tmpFactionExtraCandItems;
	for(i = 0; i < ad.m_ulFactionExtraCandItems; i++)
	{
		SaveAwardCandBin(ad.m_FactionExtraCandItems[i], fp);
	}

	//Added 2011-03-04.
	p->m_ExtraCandItems = tmpExtraCandItems;
	for (i = 0; i < ad.m_ulExtraCandItems; i++)
	{
		SaveAwardCandBin(
			ad.m_ExtraCandItems[i],
			fp
		);
	}

	//Added 2011-03-07.
	p->m_pwstrExtraTribute = pwstrExtraTribute;
	size_t len;
	len = ad.m_pwstrExtraTribute ? wcslen(ad.m_pwstrExtraTribute) : 0;
	fwrite(&len, sizeof(len), 1, fp);
	if(len)
	{
		fwrite(ad.m_pwstrExtraTribute, sizeof(task_char), len, fp);
	}

}

inline void SaveAwardDataByRatio(const AWARD_RATIO_SCALE& ad, FILE* fp)
{
	fwrite(&ad.m_ulScales, sizeof(ad.m_ulScales), 1, fp);
	fwrite(ad.m_Ratios, sizeof(ad.m_Ratios), 1, fp);

	for (unsigned long i = 0; i < ad.m_ulScales; i++)
		SaveAwardDataBin(ad.m_Awards[i], fp);
}

inline void SaveAwardDataByItem(const AWARD_ITEMS_SCALE& ad, FILE* fp)
{
	fwrite(&ad.m_ulScales, sizeof(ad.m_ulScales), 1, fp);
	fwrite(&ad.m_ulItemId, sizeof(ad.m_ulItemId), 1, fp);
	fwrite(ad.m_Counts, sizeof(ad.m_Counts), 1, fp);

	for (unsigned long i = 0; i < ad.m_ulScales; i++)
		SaveAwardDataBin(ad.m_Awards[i], fp);
}

inline void SaveAwardDataByCount(const AWARD_COUNT_SCALE& ad, FILE* fp)
{
	fwrite(&ad.m_ulScales, sizeof(ad.m_ulScales), 1, fp);
	fwrite(ad.m_Counts, sizeof(ad.m_Counts), 1, fp);

	for (unsigned long i = 0; i < ad.m_ulScales; i++)
		SaveAwardDataBin(ad.m_Awards[i], fp);
}

void ATaskTemplFixedData::SaveFixedDataToTextFile(FILE* fp)
{
	unsigned long i;

	fprintf(fp, _format_id, m_ID);
	fprintf(fp, "\r\n");

	fwrite(m_szName, sizeof(task_char), MAX_TASK_NAME_LEN, fp);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_dyn, m_DynTaskType);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_type, m_ulType);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_has_sign, m_bHasSign);
	fprintf(fp, "\r\n");

	if (m_bHasSign)
	{
		fwrite(m_pszSignature, sizeof(task_char), MAX_TASK_NAME_LEN, fp);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_time_limit, m_ulTimeLimit);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_abs_time, (int)m_bAbsTime);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_avail_frequency, m_lAvailFrequency);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_time_interval, m_lTimeInterval);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_count, m_ulTimetable);
	fprintf(fp, "\r\n");

	for (i = 0; i < m_ulTimetable; i++)
	{
		fprintf(fp, _format_tm_type, m_tmType[i]);
		fprintf(fp, "\r\n");

		task_tm& s = m_tmStart[i];
		fprintf(fp, _format_full_tm, s.year, s.month, s.day, s.hour, s.min, s.wday);
		fprintf(fp, "\r\n");

		task_tm& e = m_tmEnd[i];
		fprintf(fp, _format_full_tm, e.year, e.month, e.day, e.hour, e.min, e.wday);
		fprintf(fp, "\r\n");
	}

	// 如果选择某个子任务，则不能顺序执行
	if (m_bChooseOne || m_bRandOne)
		m_bExeChildInOrder = false;

	fprintf(fp, _format_child_in_order, m_bExeChildInOrder);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_choose_one, m_bChooseOne);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_rand_one, m_bRandOne);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_parent_fail, m_bParentAlsoFail);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_parent_succ, m_bParentAlsoSucc);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_can_give_up, m_bCanGiveUp);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_die_as_fail, m_bFailAsPlayerDie);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_can_redo, m_bCanRedo);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_redo_after_fail, m_bCanRedoAfterFailure);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_clear_as_giveup, m_bClearAsGiveUp);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_need_record, m_bNeedRecord);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_max_receiver, m_ulMaxReceiver);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_clear_rev_type, m_nClearReceiverType);
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_clear_rev_t_i, m_lClearReceiverTimeInterval);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_must_in_zone, m_bDelvInZone);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_world_id, m_ulDelvWorld);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_zone_vert, VERT_ZONE(m_DelvMinVert));
	fprintf(fp, "\r\n");

	fprintf(fp, _format_zone_vert, VERT_ZONE(m_DelvMaxVert));
	fprintf(fp, "\r\n");

	fprintf(fp, _format_trans_to, m_bTransTo);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_world_id, m_ulTransWldId);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_zone_vert, VERT_ZONE(m_TransPt));
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_Phase_cnt, m_iPhaseCnt);
	fprintf(fp, "\r\n");

	for(i = 0; i < m_iPhaseCnt; i++)
	{
		fprintf(fp, _format_award_Phase_id, m_Phase[i].iPhaseID);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_award_Phase_open, m_Phase[i].bOpenClose);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_prop_Phase_triger, m_Phase[i].bTrigger);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_prop_Phase_visual, m_Phase[i].bVisual);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_controller, m_lMonsCtrl);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_trig_flag, m_bTrigCtrl);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_award_ctrl_cnt, m_ulMonCtrlCnt);
	fprintf(fp, "\r\n");

	for (i = 0; i <m_ulMonCtrlCnt; i++)
	{
		fprintf(fp, _format_award_ctrl_id, m_MonCtrl[i].lMonCtrlID);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_award_ctrl_prob, m_MonCtrl[i].fGetProb);
		fprintf(fp, "\r\n");
	
		fprintf(fp, _format_award_ctrl_open, m_MonCtrl[i].bOpenClose);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_award_ctrl_rand, m_bRanMonCtrl);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_auto_deliver, m_bAutoDeliver);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_auto_deliver_mode, m_bDeliverWindowMode);
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_death_trig, m_bDeathTrig);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_manual_trig, m_bManualTrig);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_key_task_must_show, m_bMustShown);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_clear_acquired, m_bClearAcquired);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_suit_lev, m_ulSuitableLevel);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_show_prompt, m_bShowPrompt);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_key_task, m_bKeyTask);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_npc, m_ulDelvNPC);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_npc, m_ulAwardNPC);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_skill_task, m_bSkillTask);
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_seek_out, m_bCanSeekOut);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_direction, m_bShowDirection);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_weight, m_fStorageWeight);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_rank, m_ulRank);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_marriage, m_bMarriage);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_faction_task, m_bFaction);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_shared_by_fam, m_bSharedByFamily);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_rec_finish_count, m_bRecFinishCount);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_rec_finish_count, m_bRecFinishCountGlobal);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_finish_count, m_ulMaxFinishCount);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_full_tm, m_FinishClearTime.year, m_FinishClearTime.month, m_FinishClearTime.day, m_FinishClearTime.hour, m_FinishClearTime.min, m_FinishClearTime.wday);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_type, m_nFinishTimeType);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_lifeagain_reset, m_bLifeAgainReset);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_fail_logout, m_bFailAfterLogout);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_logout_fail_time, m_ulLogoutFailTime);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_abs_fail_time, m_bAbsFail);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_full_tm_old, m_tmAbsFailTime.year, m_tmAbsFailTime.month, m_tmAbsFailTime.day, m_tmAbsFailTime.hour, m_tmAbsFailTime.min);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prentice_task, m_bPrenticeTask);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_hidden, m_bHidden);
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_out_zone_fail, m_bOutZoneFail);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_world_id, m_ulOutZoneWorldID);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_zone_vert, VERT_ZONE(m_OutZoneMinVert));
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_zone_vert, VERT_ZONE(m_OutZoneMaxVert));
	fprintf(fp, "\r\n");

	fprintf(fp, _format_enter_zone_fail, m_bEnterZoneFail);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_world_id, m_ulEnterZoneWorldID);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_zone_vert, VERT_ZONE(m_EnterZoneMinVert));
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_zone_vert, VERT_ZONE(m_EnterZoneMaxVert));
	fprintf(fp, "\r\n");

	fprintf(fp, _format_clear_illegal, m_bClearSomeIllegalStates);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_change_key_size, m_lChangeKeyArr.size());
	fprintf(fp, "\r\n");

	for (i=0; i<m_lChangeKeyArr.size(); i++)
	{
		fprintf(fp, _format_change_key, m_lChangeKeyArr[i]);
		fprintf(fp, "\r\n");		
		
		fprintf(fp, _format_change_kv, m_lChangeKeyValueArr[i]);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_change_typ, m_bChangeTypeArr[i]);
		fprintf(fp, "\r\n");		
	}

	fprintf(fp, _format_kill_monster_fail, m_bKillMonsterFail);
	fprintf(fp, "\r\n");

	if(m_bKillMonsterFail)
	{
		fprintf(fp, _format_kill_monster_fail_cand, m_ulKillFailMonster);
		fprintf(fp, "\r\n");

		for(i=0;i<m_ulKillFailMonster; i++)
		{
			fprintf(fp, _format_kill_monster_fail_id, m_KillFailMonsters[i]);
			fprintf(fp, "\r\n");
		}
	}

	fprintf(fp, _format_have_item_fail, m_bHaveItemFail);
	fprintf(fp, "\r\n");

	if(m_bHaveItemFail)
	{
		fprintf(fp, _format_have_item_fail_cand, m_ulHaveItemFail);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_have_item_fail_not_take_off, m_bHaveItemFailNotTakeOff);
		fprintf(fp, "\r\n");

		for(i=0;i<m_ulHaveItemFail; i++)
		{
			fprintf(fp, _format_have_item_fail_id, m_HaveFailItems[i]);
			fprintf(fp, "\r\n");
		}
	}

	fprintf(fp, _format_not_have_item_fail, m_bNotHaveItemFail);
	fprintf(fp, "\r\n");

	if(m_bNotHaveItemFail)
	{
		fprintf(fp, _format_not_have_item_fail_cand, m_ulNotHaveItemFail);
		fprintf(fp, "\r\n");

		for(i=0;i<m_ulNotHaveItemFail; i++)
		{
			fprintf(fp, _format_not_have_item_fail_id, m_NotHaveFailItems[i]);
			fprintf(fp, "\r\n");
		}
	}

	fprintf(fp, _format_cam_move, m_ulCameraMove);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_animation, m_ulAnimation);
	fprintf(fp, "\r\n");

	//Added 2011-04-11
	for ( int nIndex = 0; nIndex < MAX_VARIABLE_NUM; ++nIndex )
	{
		fprintf( fp, _format_variables, m_lVariables[nIndex] );
		fprintf( fp, "\r\n" );
	}

	fprintf( fp, _format_display_type, m_ulDisplayType);
	fprintf(fp, "\r\n");
	fprintf( fp, _format_recommend_type, m_ulRecommendType);
	fprintf(fp, "\r\n");

	fprintf( fp, _format_tiny_game_id, m_ulTinyGameID);
	fprintf(fp, "\r\n");

	fprintf( fp, _format_dyn_clear_time, m_lDynFinishClearTime);
	fprintf(fp, "\r\n");

	fprintf( fp, _format_clear_xp_cd, m_bClearXpCD);
	fprintf(fp, "\r\n");

	/* 任务的开启条件及方式 */

	fprintf(fp, _format_special_award, m_ulSpecialAward);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_level, m_ulPremise_Lev_Min);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_level, m_ulPremise_Lev_Max);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_talisman_value, m_nTalismanValueMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_talisman_value, m_nTalismanValueMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_consume_treasure, m_nConsumeTreasureMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_consume_treasure, m_nConsumeTreasureMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_item_wanted_num, m_ulPremItems);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_item_not_take_off, m_bItemNotTakeOff);
	fprintf(fp, "\r\n");

	for (i = 0; i < m_ulPremItems; i++)
	{
		ITEM_WANTED& iw = m_PremItems[i];

		fprintf(fp, _format_item_templ, iw.m_ulItemTemplId);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_item_is_cmn, iw.m_bCommonItem);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_item_num, iw.m_ulItemNum);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_item_refine_cond, iw.m_cRefineCond);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_item_refine_level, iw.m_ulRefineLevel);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_monster_summon_cand, m_ulPremMonsterSummoned);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_monster_summon_mode, m_ulSummonMode);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_rand_one, m_bRandSelMonster);
	fprintf(fp, "\r\n");

	for(i = 0; i < m_ulPremMonsterSummoned; i++)
	{
		MONSTER_SUMMONED& ms = m_MonsterSummoned[i];

		fprintf(fp, _format_monster_templ, ms.m_ulMonsterTemplID);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_monster_summon_is_monster, ms.m_bIsMonster);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_monster_num, ms.m_ulMonsterNum);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_world_id, ms.m_ulMapId);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_zone_vert, ms.m_SummonVert.x, ms.m_SummonVert.y, ms.m_SummonVert.z);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_period, ms.m_lPeriod);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_count, m_ulPremTitleCount);
	fprintf(fp, "\r\n");

	for (i = 0; i < m_ulPremTitleCount; i++)
	{
		fprintf(fp, _format_award_title, m_PremTitles[i]);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_given_items_num, m_ulGivenItems);
	fprintf(fp, "\r\n");

	for (i = 0; i < m_ulGivenItems; i++)
	{
		ITEM_WANTED& iw = m_GivenItems[i];

		fprintf(fp, _format_item_templ, iw.m_ulItemTemplId);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_item_num, iw.m_ulItemNum);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_item_is_cmn, iw.m_bCommonItem);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_prem_deposit, m_ulPremise_Deposit);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_contrib, m_lPremise_Contribution, m_bDepositContribution);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_fam_contrib, m_nPremise_FamContrib, m_bDepositFamContrib);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_fam_max, m_nPremFamContribMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_battle_score, m_nPremBattleScoreMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_battle_score, m_nPremBattleScoreMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_deposit_battle, m_bDepositBattleScore);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_battle_score, m_nPremSJBattleScore);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_deposit_battle, m_bSJDepostiBattleScore);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_reputation, m_lPremise_Reputation);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_repu_depo, m_bRepuDeposit);
	fprintf(fp, "\r\n");

	for (i = 0; i < SIZE_OF_ARRAY(m_Premise_Friendship); i++)
	{
		fprintf(fp, _format_friendship, m_Premise_Friendship[i]);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_friendship_depo, m_bFriendshipDeposit);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_cotask, m_ulPremise_Cotask);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_cotask_cond, m_ulCoTaskCond);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_mutex_task_count, m_ulMutexTaskCount);
	fprintf(fp, "\r\n");

	for (i = 0; i < m_ulMutexTaskCount; i++)
	{
		fprintf(fp, _format_mutex_task, m_ulMutexTasks[i]);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_prem_task_count, m_ulPremise_Task_Count);
	fprintf(fp, "\r\n");

	for (i = 0; i < m_ulPremise_Task_Count; i++)
	{
		fprintf(fp, _format_prem_task, m_ulPremise_Tasks[i]);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_prem_task_count, m_ulPremFinishTaskCount);
	fprintf(fp, "\r\n");

	for (i = 0; i < m_ulPremFinishTaskCount; i++)
	{
		FINISH_TASK_COUNT_INFO& info = m_PremFinishTasks[i];

		fprintf(fp, _format_prem_task, info.task_id);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_finish_count, info.count);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_prem_task_count, m_ulPremGlobalCount);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_task, m_ulPremGlobalTask);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_period, m_ulPremise_Period);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_is_team, m_bTeamwork);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_share, m_bShareWork);	//共享任务 Added 2012-04-09.
	fprintf(fp, "\r\n");

	if (m_bTeamwork)
	{
		fprintf(fp, _format_rcv_by_team, m_bRcvByTeam);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_shared, m_bSharedTask);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_share_acquire, m_bSharedAchieved);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_check_tmmate, m_bCheckTeammate);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_dist, m_fTeammateDist);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_rcv_chck_mem, m_bRcvChckMem);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_dist, m_fRcvMemDist);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_cnt_by_mem_pos, m_bCntByMemPos);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_dist, m_fCntMemDist);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_all_fail, m_bAllFail);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_cap_fail, m_bCapFail);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_cap_succ, m_bCapSucc);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_dist, m_fSuccDist);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_all_succ, m_bAllSucc);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_dism_self_fail, m_bDismAsSelfFail);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_mem_num_wanted, m_ulTeamMemsWanted);
		fprintf(fp, "\r\n");

		for (i = 0; i < m_ulTeamMemsWanted; i++)
		{
			const TEAM_MEM_WANTED& tmw = m_TeamMemsWanted[i];

			fprintf(fp, _format_level, tmw.m_ulLevelMin);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_level, tmw.m_ulLevelMax);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_occupation, tmw.m_ulOccupation);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_race, tmw.m_ulRace);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_gender, tmw.m_ulGender);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_lifeagain, tmw.m_ulLifeAgain);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_same_family, tmw.m_bSameFamily);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_count, tmw.m_ulMinCount);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_count, tmw.m_ulMaxCount);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_task, tmw.m_ulTask);
			fprintf(fp, "\r\n");
		}
	}

	fprintf(fp, _format_master_wugui, m_bMaster);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prentice, m_bPrentice);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_master_moral, m_lMasterMoral);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_cap_change_fail, m_bCapChangeAllFail);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_mp_task, m_bMPTask);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_mp_task_cnt, m_ulMPTaskCnt);
	fprintf(fp, "\r\n");
	
	for (i = 0; i < m_ulMPTaskCnt; i++)
	{	
		fprintf(fp, _format_mp_lev_limit, m_MPTask[i].m_ulLevLimit);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_mp_task_id, m_MPTask[i].m_ulTaskID);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_out_master_task, m_bOutMasterTask);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_is_spouse, m_bPremise_Spouse);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_clan, m_ulPremise_Faction);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_master, m_bPremise_FactionMaster);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_gender, m_ulGender);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_count, m_ulOccupations);
	fprintf(fp, "\r\n");

	for (i = 0; i < m_ulOccupations; i++)
	{
		fprintf(fp, _format_prem_occupation, m_Occupations[i]);
		fprintf(fp, "\r\n");
	}

	for (i = 0; i < MAX_LIVING_SKILLS; i++)
	{
		fprintf(fp, _format_proficiency, m_lSkillLev[i]);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_prem_skill_lev, m_nSkillLev);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_pet_con, m_nPetCon);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_pet_civ, m_nPetCiv);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_pk_value, m_lPKValueMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_pk_value, m_lPKValueMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_gm, m_bPremise_GM);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_by_deposit, m_bShowByDeposit);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_by_gender, m_bShowByGender);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_by_items, m_bShowByItems);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_by_lev, m_bShowByLev);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_by_occup, m_bShowByOccup);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_by_preTask, m_bShowByPreTask);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_by_repu, m_bShowByRepu);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_by_team, m_bShowByTeam);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_by_faction, m_bShowByFaction);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_by_period, m_bShowByPeriod);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_by_spouse, m_bShowBySpouse);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_in_family, m_bInFamily);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_family_header, m_bFamilyHeader);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_f_skill_lev, m_nFamilySkillLevelMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_f_skill_lev, m_nFamilySkillLevelMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_f_skill_prof, m_nFamilySkillProficiencyMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_f_skill_prof, m_nFamilySkillProficiencyMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_f_skill_index, m_nFamilySkillIndex);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_record_index, m_nFamilyMonRecordIndex);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_record_value, m_nFamilyMonRecordMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_record_value, m_nFamilyMonRecordMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_f_value_index, m_nFamilyValueIndex);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_f_value, m_nFamilyValueMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_f_value, m_nFamilyValueMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_f_deposit_value, m_bDepositFamilyValue);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_check_lifeagain, m_bCheckLifeAgain);
	fprintf(fp, "\r\n");

    if (m_bCheckLifeAgain)
    {
		fprintf(fp, _format_spouse_again, m_bSpouseAgain);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_lifeagain_cnt, m_ulLifeAgainCnt);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_lifeagain_cntcom, m_nLifeAgainCntCompare);
		fprintf(fp, "\r\n");

		for (i=0; i<MAX_OCCUPATIONS; i++)
		{
			fprintf(fp, _format_lifeagain_occup, m_bLifeAgainOneOccup[i]);
			fprintf(fp, "\r\n");
		}
		
		for (i=0; i<MAX_OCCUPATIONS; i++)
		{
			fprintf(fp, _format_lifeagain_occup, m_bLifeAgainTwoOccup[i]);
			fprintf(fp, "\r\n");
		}
		
		for (i=0; i<MAX_OCCUPATIONS; i++)
		{
			fprintf(fp, _format_lifeagain_occup, m_bLifeAgainThrOccup[i]);
			fprintf(fp, "\r\n");
		}
    }

	fprintf(fp, _format_prem_cult, m_ulPremCult);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_need_comp, m_bPremNeedComp);
	fprintf(fp, "\r\n");

	if (m_bPremNeedComp)
	{
		fprintf(fp, _format_exp1_andor_exp2, m_nPremExp1AndOrExp2);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_left_string, &m_PremCompExp1.strExpLeft[0]);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_comp_oper, m_PremCompExp1.nCompOper);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_right_string, &m_PremCompExp1.strExpRight[0]);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_left_string, &m_PremCompExp2.strExpLeft[0]);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_comp_oper, m_PremCompExp2.nCompOper);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_right_string, &m_PremCompExp2.strExpRight[0]);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_accomp_cnt, m_ulPremAccompCnt);
	fprintf(fp, "\r\n");
	
	for (i = 0; i < m_ulPremAccompCnt; i++)
	{
		fprintf(fp, _format_accomp_id, m_aPremAccompID[i]);
		fprintf(fp, "\r\n");
	}
	
	fprintf(fp, _format_accomp_cond, m_bPremAccompCond);
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_transform_id, m_iPremTransformID);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_transform_lev, m_iPremTransformLevel);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_transform_lev, m_iPremTransformExpLevel);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_script_open, m_bScriptOpenTask);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_achieve_min, m_ulPremAchievementMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_achieve_max, m_ulPremAchievementMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_circle_group_min, m_ulPremCircleGroupMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_circle_group_max, m_ulPremCircleGroupMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_territory_score_min, m_ulPremTerritoryScoreMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_territory_score_max, m_ulPremTerritoryScoreMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_fengshen_type, m_nPremFengshenType);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_fengshen_lvl_min, m_ulPremFengshenLvlMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_fengshen_lvl_max, m_ulPremFengshenLvlMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prem_fengshen_exp_full, m_bExpMustFull);
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_by_fengshen, m_bShowByFengshenLvl);
	fprintf(fp, "\r\n");

	//Added 2011-04-11.
	fprintf(fp, _format_create_role_time_duration, m_ulCreateRoleTimeDuration);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_building_id, m_nBuildId);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_building_level, m_nBuildLevel);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_faction_premise_gold_note, m_iPremise_FactionGoldNote);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_by_gold_note, m_bShowByFactionGoldNote);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_faction_premise_grass, m_iPremise_FactionGrass);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_faction_premise_mine, m_iPremise_FactionMine);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_faction_premise_monster_core, m_iPremise_FactionMonsterCore);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_faction_premise_mosnter_food, m_iPremise_FactionMonsterFood);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_faction_premise_money, m_iPremise_FactionMoney);
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_build_level_in_construct, m_nBuildLevelInConstruct);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_interobj_id, m_iInterObjId);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_by_interobj_id, m_bShowByInterObjId);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_total_caseadd, m_nPremTotalCaseAddMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_total_caseadd, m_nPremTotalCaseAddMax);
	fprintf(fp, "\r\n");

	/* 任务完成的方式及条件 */

	if (m_ulFirstChild)
		fprintf(fp, _format_method, enumTMNone);
	else
		fprintf(fp, _format_method, m_enumMethod);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_finish_type, m_enumFinishType);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_finish_achieve, m_ulFinishAchievement);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_finish_friend_num, m_ulFriendNum);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_finish_lev, m_ulFinishLev);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_finish_dlg, m_bDisFinDlg);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_script_finish, m_bScriptFinishTask);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_need_comp, m_bFinNeedComp);
	fprintf(fp, "\r\n");

	if (m_bFinNeedComp)
	{
		fprintf(fp, _format_exp1_andor_exp2, m_nFinExp1AndOrExp2);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_left_string, &m_FinCompExp1.strExpLeft[0]);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_comp_oper, m_FinCompExp1.nCompOper);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_right_string, &m_FinCompExp1.strExpRight[0]);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_left_string, &m_FinCompExp2.strExpLeft[0]);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_comp_oper, m_FinCompExp2.nCompOper);
		fprintf(fp, "\r\n");
		
		fprintf(fp, _format_right_string, &m_FinCompExp2.strExpRight[0]);
		fprintf(fp, "\r\n");
	}

	if (m_enumMethod == enumTMKillNumMonster)
	{
		fprintf(fp, _format_monster_wanted, m_ulMonsterWanted);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_summon_monster_mode, m_bSummonMonsterMode);
		fprintf(fp, "\r\n");

		for (i = 0; i < m_ulMonsterWanted; i++)
		{
			MONSTER_WANTED& Monster = m_MonsterWanted[i];

			fprintf(fp, _format_monster_templ, Monster.m_ulMonsterTemplId);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_monster_num, Monster.m_ulMonsterNum);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_item_templ, Monster.m_ulDropItemId);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_item_wanted_num, Monster.m_ulDropItemCount);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_item_is_cmn, Monster.m_bDropCmnItem);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_item_drop_prob, Monster.m_fDropProb);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_killer_lev, Monster.m_bKillerLev);
			fprintf(fp, "\r\n");
		}
	}
	else if (m_enumMethod == enumTMCollectNumArticle)
	{
		fprintf(fp, _format_gold, m_ulGoldWanted);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_item_wanted_num, m_ulItemsWanted);
		fprintf(fp, "\r\n");

		for (i = 0; i < m_ulItemsWanted; i++)
		{
			ITEM_WANTED& iw = m_ItemsWanted[i];

			fprintf(fp, _format_item_templ, iw.m_ulItemTemplId);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_item_is_cmn, iw.m_bCommonItem);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_item_num, iw.m_ulItemNum);
			fprintf(fp, "\r\n");

			fprintf(fp, _format_item_refine_level, iw.m_ulRefineLevel);
			fprintf(fp, "\r\n");
			
			fprintf(fp, _format_item_replace_templ, iw.m_ulReplaceItemTemplId);
			fprintf(fp, "\r\n");
		}

		//Added 2011-10-08.
		fprintf(fp, _format_auto_move_for_collect_num_items_enable, m_bAutoMoveForCollectNumItems);
		fprintf(fp, "\r\n");
		//Added end.
	}
	else if (m_enumMethod == enumTMProtectNPC)
	{
		fprintf(fp, _format_npc_to_protect, m_ulNPCToProtect);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_protect_tm_len, m_ulProtectTimeLen);
		fprintf(fp, "\r\n");
	}
	else if (m_enumMethod == enumTMWaitTime)
	{
		fprintf(fp, _format_waittime, m_ulWaitTime);
		fprintf(fp, "\r\n");

		//Added 2011-10-08.
		fprintf(fp, _format_show_wait_time_enable, m_bShowWaitTime);
		fprintf(fp, "\r\n");		
		//Added end.
	}
	else if (m_enumMethod == enumTMReachSite)
	{
		fprintf(fp, _format_zone_vert, VERT_ZONE(m_ReachSiteMin));
		fprintf(fp, "\r\n");

		fprintf(fp, _format_zone_vert, VERT_ZONE(m_ReachSiteMax));
		fprintf(fp, "\r\n");

		fprintf(fp, _format_world_id, m_ulReachSiteId);
		fprintf(fp, "\r\n");
		
		//Added 2011-10-08.
		fprintf(fp, _format_zone_vert, VERT_ZONE(m_AutoMoveDestPos));
		fprintf(fp, "\r\n");

		fprintf(fp, _format_auto_move_enable, m_bAutoMoveForReachFixedSite);
		fprintf(fp, "\r\n");
		//Added end.
		//Added 2011-10-09.
		fwrite(m_szAutoMoveDestPosName, sizeof(task_char), MAX_TASK_NAME_LEN, fp);
		fprintf(fp, "\r\n");
		//Added end.
	}
	else if (m_enumMethod == enumTMLeaveSite)
	{	
		fprintf(fp, _format_world_id, m_ulLeaveSiteId);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_zone_vert, VERT_ZONE(m_LeaveSiteMin));
		fprintf(fp, "\r\n");

		fprintf(fp, _format_zone_vert, VERT_ZONE(m_LeaveSiteMax));
		fprintf(fp, "\r\n");
	}
	else if (m_enumMethod == enumTMTitle)
	{
		fprintf(fp, _format_title_count, m_ulTitleWantedNum);
		fprintf(fp, "\r\n");

		for (i = 0; i < m_ulTitleWantedNum; i++)
		{
			fprintf(fp, _format_title, m_TitleWanted[i]);
			fprintf(fp, "\r\n");
		}
	}
	else if (m_enumMethod == enumTMFixedTime)
	{
		task_tm& s = m_tmFixedTime;
		
		fprintf(fp, _format_fixed_time_type, m_iFixedType);
		fprintf(fp, "\r\n");	

		fprintf(fp, _format_full_tm, s.year, s.month, s.day, s.hour, s.min, s.wday);
		fprintf(fp, "\r\n");	
	}
	else if (m_enumMethod == enumTMNPCAction)
	{
		fprintf(fp, _format_action_npc, m_ulActionNPC);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_action_id, m_nActionID);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_nation_position_mask, m_ulPremNationPositionMask);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_king_score_cost, m_nPremKingScoreCost);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_max_king_score, m_nPremKingScoreMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_min_king_score, m_nPremKingScoreMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_has_king, m_bPremHasKing);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_total_caseadd, m_nTotalCaseAddMin);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_total_caseadd, m_nTotalCaseAddMax);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_faction_gold_note_wanted, m_iFactionGoldNoteWanted);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_faction_grass_wanted, m_iFactionGrassWanted);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_faction_mine_wanted, m_iFactionMineWanted);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_faction_monster_core_wanted, m_iFactionMonsterCoreWanted);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_faction_mosnter_food_wanted, m_iFactionMonsterFoodWanted);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_faction_money_wanted, m_iFactionMoneyWanted);
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_interobj_wanted, m_ulInterObjWanted);
	fprintf(fp, "\r\n");

	for(i = 0; i < m_ulInterObjWanted; i++)
	{
		INTEROBJ_WANTED& wi = m_InterObjWanted[i];

		fprintf(fp, _format_interobj_templ, wi.m_ulInterObjId);
		fprintf(fp, "\r\n");

		fprintf(fp, _format_interobj_num, wi.m_ulInterObjNum);
		fprintf(fp, "\r\n");
	}

	fprintf(fp, _format_zone_vert, VERT_ZONE(m_InterReachSiteMin));
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_zone_vert, VERT_ZONE(m_InterReachSiteMax));
	fprintf(fp, "\r\n");

	fprintf(fp, _format_world_id, m_ulInterReachSiteId);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_interobj_wanted, m_iInterReachItemId);
	fprintf(fp, "\r\n");


	fprintf(fp, _format_zone_vert, VERT_ZONE(m_InterLeaveSiteMin));
	fprintf(fp, "\r\n");

	fprintf(fp, _format_zone_vert, VERT_ZONE(m_InterLeaveSiteMax));
	fprintf(fp, "\r\n");

	fprintf(fp, _format_world_id, m_ulInterLeaveSiteId);
	fprintf(fp, "\r\n");
	
	fprintf(fp, _format_interobj_wanted, m_iInterLeaveItemId);
	fprintf(fp, "\r\n");


	fprintf(fp, _format_build_id_wanted, m_iBuildingIdWanted);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_build_level_wanted, m_iBuildingLevelWanted);
	fprintf(fp, "\r\n");

	/* 任务结束后的奖励 */

#define SAVE_AWARD_BY_RESULT(Result) \
{\
	unsigned long i;\
\
	fprintf(fp, _format_award_type, m_ulAwardType##Result);\
	fprintf(fp, "\r\n");\
\
	switch (m_ulAwardType##Result)\
	{\
	case enumTATNormal:\
	case enumTATEach:\
		SaveAwardData(*m_Award##Result, fp);\
		break;\
	case enumTATRatio:\
		fprintf(fp, _format_count, m_AwByRatio##Result->m_ulScales);\
		fprintf(fp, "\r\n");\
\
		for (i = 0; i < m_AwByRatio##Result->m_ulScales; i++)\
		{\
			fprintf(fp, _format_ratio, m_AwByRatio##Result->m_Ratios[i]);\
			fprintf(fp, "\r\n");\
\
			SaveAwardData(m_AwByRatio##Result->m_Awards[i], fp);\
		}\
		break;\
	case enumTATItemCount:\
		fprintf(fp, _format_count, m_AwByItems##Result->m_ulScales);\
		fprintf(fp, "\r\n");\
\
		fprintf(fp, _format_item_templ, m_AwByItems##Result->m_ulItemId);\
		fprintf(fp, "\r\n");\
\
		for (i = 0; i < m_AwByItems##Result->m_ulScales; i++)\
		{\
			fprintf(fp, _format_item_num, m_AwByItems##Result->m_Counts[i]);\
			fprintf(fp, "\r\n");\
\
			SaveAwardData(m_AwByItems##Result->m_Awards[i], fp);\
		}\
		break;\
	case enumTATFinishCount:\
		fprintf(fp, _format_count, m_AwByCount##Result->m_ulScales);\
		fprintf(fp, "\r\n");\
\
		for (i = 0; i < m_AwByCount##Result->m_ulScales; i++)\
		{\
			fprintf(fp, _format_item_num, m_AwByCount##Result->m_Counts[i]);\
			fprintf(fp, "\r\n");\
\
			SaveAwardData(m_AwByCount##Result->m_Awards[i], fp);\
		}\
		break;\
	}\
}

	SAVE_AWARD_BY_RESULT(_S)

	SAVE_AWARD_BY_RESULT(_F)

	/* 层次关系 */

	fprintf(fp, _format_parent, m_ulParent);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_next_sibling, m_ulNextSibling);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_prev_sibling, m_ulPrevSibling);
	fprintf(fp, "\r\n");

	fprintf(fp, _format_first_child, m_ulFirstChild);
	fprintf(fp, "\r\n");
}

void ATaskTemplFixedData::SaveFixedDataToBinFile(FILE* fp)
{
	unsigned long i;

	// 如果选择某个子任务，则不能顺序执行
	if (m_bChooseOne || m_bRandOne)
		m_bExeChildInOrder = false;

	unsigned long ulDefaultDisplayType = m_ulDisplayType;
	// 对任务数据进行预处理，改变老版本数据的默认属性
	if(m_ulDisplayType == 0)	// 任务显示类型，默认为0。  1：日常， 2：活动， 3：诛仙， 4：支线。
	{
/*
1. 原有编辑器中勾选了"关键任务"的任务，在编辑器中自动归为"默认"，且任务具有原主线任务属性，即不可放弃，且强制关注，在游戏中显示为"诛仙"任务。
2. 原有编辑器中任务类型为经典、修真、侠义、寻宝、奇遇、情缘、探险的任务，在编辑器中自动归为默认任务，在游戏中显示为"支线"任务。
3. 原有编辑器中任务类型为师门、平妖、试炼、挑战、天界平妖、天界试炼、天界挑战、帮会、跨服、传功、游历的任务，在编辑器中自动归为默认任务，在游戏中显示为"日常"任务。
4. 上古任务，全部归类为日常。

游历0 挑战1 平妖2 试炼3 探险4 传功5 经典6 修真7 侠义8 师门9 帮会10 寻宝11 奇遇12 情缘13 跨服14 天界平妖15 天界挑战16 天界试炼17 上古白绿红18,19,20
*/
		if(m_bKeyTask)
			m_ulDisplayType = 3;
		else if(m_ulType == 6 ||
				m_ulType == 7 ||
				m_ulType == 8 ||
				m_ulType == 11 ||
				m_ulType == 12 ||
				m_ulType == 13 ||
				m_ulType == 4)
			m_ulDisplayType = 4;
		else if(m_ulType == 9 ||
				m_ulType == 2 ||
				m_ulType == 3 ||
				m_ulType == 1 ||
				m_ulType == 15 ||
				m_ulType == 16 ||
				m_ulType == 17 ||
				m_ulType == 10 ||
				m_ulType == 14 ||
				m_ulType == 0 ||
				m_ulType == 5 ||
				m_ulType == 18 ||
				m_ulType == 19||
				m_ulType == 20)
			m_ulDisplayType = 1;
	}

	ATaskTemplFixedData tmp;
	PushPtr(tmp);
	fwrite(this, sizeof(*this), 1, fp);
	PopPtr(tmp);

	if (m_bHasSign) fwrite(m_pszSignature, sizeof(task_char), MAX_TASK_NAME_LEN, fp);

	for (i = 0; i < m_ulTimetable; i++)
	{
		fwrite(&m_tmStart[i], sizeof(m_tmStart[i]), 1, fp);
		fwrite(&m_tmEnd[i], sizeof(m_tmEnd[i]), 1, fp);
	}

	if (m_lChangeKeyArr.size())
	{
		fwrite(&m_lChangeKeyArr[0], sizeof(long), m_lChangeKeyArr.size(), fp);
		fwrite(&m_lChangeKeyValueArr[0], sizeof(long), m_lChangeKeyValueArr.size(), fp);
		fwrite(&m_bChangeTypeArr[0], sizeof(bool), m_bChangeTypeArr.size(), fp);
	}

	/* 任务的开启条件及方式 */
	for (i = 0; i < m_ulPremItems; i++)
		fwrite(&m_PremItems[i], sizeof(m_PremItems[i]), 1, fp);

	for (i = 0; i < m_ulPremMonsterSummoned; i++)
		fwrite(&m_MonsterSummoned[i], sizeof(m_MonsterSummoned[i]), 1, fp);

	if (m_ulPremTitleCount)
		fwrite(m_PremTitles, sizeof(short), m_ulPremTitleCount, fp);

	for (i = 0; i < m_ulGivenItems; i++)
	{
		ITEM_WANTED& iw = m_GivenItems[i];
		fwrite(&iw, sizeof(iw), 1, fp);
	}

	if (m_bTeamwork)
	{
		for (i = 0; i < m_ulTeamMemsWanted; i++)
		{
			const TEAM_MEM_WANTED& tmw = m_TeamMemsWanted[i];
			fwrite(&tmw, sizeof(tmw), 1, fp);
		}
	}

	if (m_bPremNeedComp)
	{
		fwrite(&m_PremCompExp1.strExpLeft[0], sizeof(char), m_PremCompExp1.strExpLeft.size(), fp);
		fwrite(&m_PremCompExp1.strExpRight[0], sizeof(char), m_PremCompExp1.strExpRight.size(), fp);

		fwrite(&m_PremCompExp2.strExpLeft[0], sizeof(char), m_PremCompExp2.strExpLeft.size(), fp);
		fwrite(&m_PremCompExp2.strExpRight[0], sizeof(char), m_PremCompExp2.strExpRight.size(), fp);
	}

	/* 任务完成的方式及条件 */

	for (i = 0; i < m_ulMonsterWanted; i++)
	{
		MONSTER_WANTED& mw = m_MonsterWanted[i];
		fwrite(&mw, sizeof(mw), 1, fp);
	}

	for (i = 0; i < m_ulItemsWanted; i++)
	{
		ITEM_WANTED& iw = m_ItemsWanted[i];
		fwrite(&iw, sizeof(iw), 1, fp);
	}

	for(i = 0; i < m_ulInterObjWanted; i++)
	{
		INTEROBJ_WANTED& wi = m_InterObjWanted[i];
		fwrite(&wi, sizeof(wi), 1, fp);
	}

	if (m_bFinNeedComp)
	{
		fwrite(&m_FinCompExp1.strExpLeft[0], sizeof(char), m_FinCompExp1.strExpLeft.size(), fp);
		fwrite(&m_FinCompExp1.strExpRight[0], sizeof(char), m_FinCompExp1.strExpRight.size(), fp);	
		
		fwrite(&m_FinCompExp2.strExpLeft[0], sizeof(char), m_FinCompExp2.strExpLeft.size(), fp);
		fwrite(&m_FinCompExp2.strExpRight[0], sizeof(char), m_FinCompExp2.strExpRight.size(), fp);	
	}

	/* 任务结束后的奖励 */

	SaveAwardDataBin(*m_Award_S, fp);
	SaveAwardDataBin(*m_Award_F, fp);

	SaveAwardDataByRatio(*m_AwByRatio_S, fp);
	SaveAwardDataByRatio(*m_AwByRatio_F, fp);

	SaveAwardDataByItem(*m_AwByItems_S, fp);
	SaveAwardDataByItem(*m_AwByItems_F, fp);

	SaveAwardDataByCount(*m_AwByCount_S, fp);
	SaveAwardDataByCount(*m_AwByCount_F, fp);

	// reset
	m_ulDisplayType = ulDefaultDisplayType;
}

int ATaskTempl::MarshalKillMonster(char* pData)
{
	char* p = pData;

	char cnt = (char)m_ulMonsterWanted;
	*p = cnt;
	p++;

	if (cnt)
	{
		size_t sz = cnt * sizeof(MONSTER_WANTED);
		memcpy(p, m_MonsterWanted, sz);
		p += sz;
	}

	return p - pData;
}

int ATaskTempl::MarshalCollectItems(char* pData)
{
	char* p = pData;

	*p = (char)m_ulItemsWanted;
	p++;

	size_t sz = m_ulItemsWanted * sizeof(ITEM_WANTED);
	if (sz)
	{
		memcpy(p, m_ItemsWanted, sz);
		p += sz;
	}

	*(long*)p = m_ulGoldWanted;
	p += sizeof(long);

	return p - pData;
}

int ATaskTempl::MarshalDynTask(char* pData)
{
	char* p = pData;

	unsigned long* token_mask1 = (unsigned long*)p;
	*token_mask1 = 0;
	p += sizeof(long);

	unsigned long* token_mask2 = (unsigned long*)p;
	*token_mask2 = 0;
	p += sizeof(long);

	int token_count = 0;

	// dyn type
	*p = m_DynTaskType;
	p++;

	// top task
	if (!m_pParent)
	{
		int sz = 0;

		switch (m_DynTaskType)
		{
		case enumDTTSpecialAward:
			sz = MarshalSpecialAwardData(p);
			if (sz == 0) return 0;
			break;
		default:
			break;
		}

		p += sz;
	}

	// id
	*(long*)p = m_ID;
	p += sizeof(long);

	// name
	char len = (char)wcslen(m_szName);
	*p = len; p++;
	len *= sizeof(task_char);
	memcpy(p, m_szName, len);
	p += len;

	// choose one
	*p = m_bChooseOne;
	p++;

	// rand one
	*p = m_bRandOne;
	p++;

	// in order
	*p = m_bExeChildInOrder;
	p++;

	// parent fail
	*p = m_bParentAlsoFail;
	p++;

	// parent succ
	*p = m_bParentAlsoSucc;
	p++;

	// give up
	*p = m_bCanGiveUp;
	p++;

	// redo
	*p = m_bCanRedo;
	p++;

	// redo after fail
	*p = m_bCanRedoAfterFailure;
	p++;

	// clear as give up
	*p = m_bClearAsGiveUp;
	p++;

	// record
	*p = m_bNeedRecord;
	p++;

	// die
	*p = m_bFailAsPlayerDie;
	p++;

	// auto deliver
	*p = m_bAutoDeliver;
	p++;

	// death trig
	*p = m_bDeathTrig;
	p++;

	// clear acquired
	*p = m_bClearAcquired;
	p++;

	// spouse
	*p = m_bPremise_Spouse;
	p++;

	// teamwork
	*p = m_bTeamwork;
	p++;

	// direction
	*p = m_bShowDirection;
	p++;

	// lev
	*p = (char)m_ulPremise_Lev_Min; p++;
	*p = (char)m_ulPremise_Lev_Max; p++;

	// 0: time limit
	if (m_ulTimeLimit)
	{
		*(long*)p = m_ulTimeLimit;
		p += sizeof(long);

		*token_mask1 |= (1 << token_count);
	}

	token_count++;

	// 1: reputation
	if (m_lPremise_Reputation)
	{
		*(long*)p = m_lPremise_Reputation;
		p += sizeof(long);

		*token_mask1 |= (1 << token_count);
	}

	token_count++;

	// 2: period
	if (m_ulPremise_Period)
	{
		*(short*)p = (short)m_ulPremise_Period;
		p += sizeof(short);

		*token_mask1 |= (1 << token_count);
	}

	token_count++;

	// 3: prem items
	if (m_ulPremItems)
	{
		*p = (char)m_ulPremItems;
		p++;

		size_t sz = m_ulPremItems * sizeof(ITEM_WANTED);
		memcpy(p, m_PremItems, sz);
		p += sz;

		*token_mask1 |= (1 << token_count);
	}

	token_count++;

	// 4: delv in zone
	if (m_bDelvInZone)
	{
		*p = m_bDelvInZone;
		p++;

		*(long*)p = m_ulDelvWorld;
		p += sizeof(long);

		*(ZONE_VERT*)p = m_DelvMinVert;
		p += sizeof(ZONE_VERT);

		*(ZONE_VERT*)p = m_DelvMaxVert;
		p += sizeof(ZONE_VERT);

		*token_mask1 |= (1 << token_count);
	}

	token_count++;

	// 5: trans to
	if (m_bTransTo)
	{
		*p = m_bTransTo;
		p++;

		*(long*)p = m_ulTransWldId;
		p += sizeof(long);

		*(ZONE_VERT*)p = m_TransPt;
		p += sizeof(ZONE_VERT);

		*token_mask1 |= (1 << token_count);
	}

	token_count++;

	// 6: given items
	if (m_ulGivenItems)
	{
		*p = (char)m_ulGivenItems;
		p++;

		*p = (char)m_ulGivenCmnCount;
		p++;

		*p = (char)m_ulGivenTskCount;
		p++;

		size_t sz = m_ulGivenItems * sizeof(ITEM_WANTED);
		memcpy(p, m_GivenItems, sz);
		p += sz;

		*token_mask1 |= (1 << token_count);
	}

	token_count++;

	// 7: deposit
	if (m_ulPremise_Deposit)
	{
		m_ulPremise_Deposit = *(long*)p;
		p += sizeof(long);

		*token_mask1 |= (1 << token_count);
	}

	token_count++;

	// 8: pre task
	if (m_ulPremise_Task_Count)
	{
		*p = (char)m_ulPremise_Task_Count;
		p++;

		size_t sz = sizeof(long) * m_ulPremise_Task_Count;
		memcpy(p, m_ulPremise_Tasks, sz);
		p += sz;

		*token_mask1 |= (1 << token_count);
	}

	token_count++;

	// 9: gender
	if (m_ulGender)
	{
		*p = (char)m_ulGender;
		p++;

		*token_mask1 |= (1 << token_count);
	}

	token_count++;

	// 10: occupation
	if (m_ulOccupations)
	{
		*p = (char)m_ulOccupations;
		p++;

		size_t sz = sizeof(long) * m_ulOccupations;
		memcpy(p, m_Occupations, sz);
		p += sz;

		*token_mask1 |= (1 << token_count);
	}

	token_count++;

	// 11: mutex task
	if (m_ulMutexTaskCount)
	{
		*p = (char)m_ulMutexTaskCount;
		p++;

		size_t sz = sizeof(long) * m_ulMutexTaskCount;
		memcpy(p, m_ulMutexTasks, sz);
		p += sz;

		*token_mask1 |= (1 << token_count);
	}

	token_count++;

	// 12: time table
	if (m_ulTimetable)
	{
		*p = (char)m_ulTimetable;
		p++;

		size_t sz;

		sz = sizeof(char) * m_ulTimetable;
		memcpy(p, m_tmType, sz);
		p += sz;

		sz = sizeof(task_tm) * m_ulTimetable;
		memcpy(p, m_tmStart, sz);
		p += sz;

		memcpy(p, m_tmEnd, sz);
		p += sz;

		*token_mask1 |= (1 << token_count);
	}

	token_count++;

	// method
	*p = (char)m_enumMethod;
	p++;

	switch (m_enumMethod)
	{
	case enumTMKillNumMonster:
		p += MarshalKillMonster(p);
		break;
	case enumTMCollectNumArticle:
		p += MarshalCollectItems(p);
		break;
	case enumTMReachSite:
		*(long*)p = m_ulReachSiteId;
		p += sizeof(long);
		memcpy(p, &m_ReachSiteMin, sizeof(ZONE_VERT));
		p += sizeof(ZONE_VERT);
		memcpy(p, &m_ReachSiteMax, sizeof(ZONE_VERT));
		p += sizeof(ZONE_VERT);
		break;
	case enumTMLeaveSite:
		*(long*)p = m_ulLeaveSiteId;
		p += sizeof(long);
		memcpy(p, &m_LeaveSiteMin, sizeof(ZONE_VERT));
		p += sizeof(ZONE_VERT);
		memcpy(p, &m_LeaveSiteMax, sizeof(ZONE_VERT));
		p += sizeof(ZONE_VERT);
		break;
	case enumTMWaitTime:
		*(long*)p = m_ulWaitTime;
		p += sizeof(long);
		break;
	}

	// finish type
	*p = (char)m_enumFinishType;
	p++;

	// award
	p += m_Award_S->MarshalBasicData(p);

	// talks
	p += marshal_str(p, m_pwstrDescript);
	p += marshal_str(p, m_pwstrOkText);
	p += marshal_str(p, m_pwstrNoText);
	p += marshal_talk_proc(&m_DelvTaskTalk, p);
	p += marshal_talk_proc(&m_UnqualifiedTalk, p);
	p += marshal_talk_proc(&m_DelvItemTalk, p);
	p += marshal_talk_proc(&m_ExeTalk, p);
	p += marshal_talk_proc(&m_AwardTalk, p);

	// sub tasks
	*(int*)p = GetSubCount();
	p += sizeof(int);

	ATaskTempl* pChild = m_pFirstChild;
	while (pChild)
	{
		p += pChild->MarshalDynTask(p);
		pChild = pChild->m_pNextSibling;
	}

	return p - pData;
}

int ATaskTempl::MarshalSpecialAwardData(char* pData)
{
	if (!m_ulSpecialAward)
		return 0;

	char* p = pData;
	*(long*)p = m_ulSpecialAward;
	p += sizeof(long);

	return p - pData;
}

bool ATaskTempl::CheckValid(FILE* fpLog)
{
	bool ret = true;

	if (m_nFinishTimeType == enumTAFEachWeek)
	{
		if ( m_lDynFinishClearTime == 0 && (m_FinishClearTime.wday < 1 || m_FinishClearTime.wday > 7) )
		{
			fprintf(fpLog, "task = %d, name = %s, value = %d, error = %s\r\n", m_ID, CSafeString(m_szName).GetAnsi(), m_FinishClearTime.day, "按周清空必须是星期1到星期7");
			ret = false;
		}
	}

	return ret;
}

void ATaskTempl::SaveToTextFile(FILE* fp)
{
	fprintf(fp, _format_version, _task_templ_cur_version);
	fprintf(fp, "\r\n");
	Save(fp, true);
}

bool ATaskTempl::SaveToTextFile(const char* szPath)
{
	FILE* fp = fopen(szPath, "w+b");
	if (fp == NULL) return false;
	SaveToTextFile(fp);
	fclose(fp);
	return true;
}

void ATaskTempl::Save(FILE* fp, bool bTextFile)
{
	SynchID();
	ATaskTemplFixedData::SaveFixedDataToTextFile(fp);
	SaveDescription(fp);
	SaveTribute(fp);
	SaveHintText(fp);
	SaveCanDeliverText(fp);
	m_DelvTaskTalk.save(fp);
	m_UnqualifiedTalk.save(fp);
	m_DelvItemTalk.save(fp);
	m_ExeTalk.save(fp);
	m_AwardTalk.save(fp);
	fprintf(fp, _format_task_count, GetSubCount());
	fprintf(fp, "\r\n");

	ATaskTempl* pChild = m_pFirstChild;
	while (pChild)
	{
		pChild->Save(fp, bTextFile);
		pChild = pChild->m_pNextSibling;
	}
}

void ATaskTempl::SaveBinary(FILE* fp)
{
	SynchID();
	ATaskTemplFixedData::SaveFixedDataToBinFile(fp);
	SaveDescriptionBin(fp);
	SaveTributeBin(fp);
	SaveHintTextBin(fp);
	SaveCanDeliverTextBin(fp);

	m_DelvTaskTalk.save(fp);
	m_UnqualifiedTalk.save(fp);
	m_DelvItemTalk.save(fp);
	m_ExeTalk.save(fp);
	m_AwardTalk.save(fp);

	int nSubCount = GetSubCount();
	fwrite(&nSubCount, sizeof(nSubCount), 1, fp);

	ATaskTempl* pChild = m_pFirstChild;
	while (pChild)
	{
		pChild->SaveBinary(fp);
		pChild = pChild->m_pNextSibling;
	}
}

ATaskTempl& ATaskTempl::operator= (const ATaskTempl& src)
{
	int i;

	*m_Award_S = *src.m_Award_S;
	*m_Award_F = *src.m_Award_F;
	AWARD_DATA* _Award_S = m_Award_S;
	AWARD_DATA* _Award_F = m_Award_F;

	*m_AwByRatio_S = *src.m_AwByRatio_S;
	*m_AwByRatio_F = *src.m_AwByRatio_F;
	AWARD_RATIO_SCALE* _AwByRatio_S = m_AwByRatio_S;
	AWARD_RATIO_SCALE* _AwByRatio_F = m_AwByRatio_F;

	*m_AwByItems_S = *src.m_AwByItems_S;
	*m_AwByItems_F = *src.m_AwByItems_F;
	AWARD_ITEMS_SCALE* _AwByItems_S = m_AwByItems_S;
	AWARD_ITEMS_SCALE* _AwByItems_F = m_AwByItems_F;

	*m_AwByCount_S = *src.m_AwByCount_S;
	*m_AwByCount_F = *src.m_AwByCount_F;
	AWARD_COUNT_SCALE* _AwByCount_S = m_AwByCount_S;
	AWARD_COUNT_SCALE* _AwByCount_F = m_AwByCount_F;

	m_tmStart = new task_tm[MAX_TIMETABLE_SIZE];
	m_tmEnd = new task_tm[MAX_TIMETABLE_SIZE];

	for (i = 0; i < MAX_TIMETABLE_SIZE; i++)
	{
		if (src.m_tmStart )
			m_tmStart[i] = src.m_tmStart[i];

		if (src.m_tmEnd)
			m_tmEnd[i] = src.m_tmEnd[i];
	}

	task_tm* _tmStart = m_tmStart;
	task_tm* _tmEnd = m_tmEnd;

	m_PremItems = new ITEM_WANTED[MAX_ITEM_WANTED];
	m_GivenItems = new ITEM_WANTED[MAX_ITEM_WANTED];
	m_ItemsWanted = new ITEM_WANTED[MAX_ITEM_WANTED];

	for (i = 0; i < MAX_ITEM_WANTED; i++)
	{
		if (src.m_PremItems)
			m_PremItems[i] = src.m_PremItems[i];

		if (src.m_GivenItems)
			m_GivenItems[i] = src.m_GivenItems[i];

		if (src.m_ItemsWanted)
			m_ItemsWanted[i] = src.m_ItemsWanted[i];
	}
	
	ITEM_WANTED* _PremItems = m_PremItems;
	ITEM_WANTED* _GivenItems = m_GivenItems;
	ITEM_WANTED* _ItemsWanted = m_ItemsWanted;

	m_MonsterSummoned = new MONSTER_SUMMONED[MAX_MONSTER_SUMMONED_CAND];

	if (src.m_MonsterSummoned)
	{
		for(i = 0; i < MAX_MONSTER_SUMMONED_CAND; i++)
			m_MonsterSummoned[i] = src.m_MonsterSummoned[i];
	}
	
	MONSTER_SUMMONED* _MonsterSummoned = m_MonsterSummoned;

	m_TeamMemsWanted = new TEAM_MEM_WANTED[MAX_TEAM_MEM_WANTED];

	if (src.m_TeamMemsWanted)
	{
		for (i = 0; i < MAX_TEAM_MEM_WANTED; i++)
			m_TeamMemsWanted[i] = src.m_TeamMemsWanted[i];
	}

	TEAM_MEM_WANTED* _TeamMemsWanted = m_TeamMemsWanted;

	m_MonsterWanted = new MONSTER_WANTED[MAX_MONSTER_WANTED];

	if (src.m_MonsterWanted)
	{
		for (i = 0; i < MAX_MONSTER_WANTED; i++)
			m_MonsterWanted[i] = src.m_MonsterWanted[i];
	}

	MONSTER_WANTED* _MonsterWanted = m_MonsterWanted;

	*(ATaskTemplFixedData*)this = *(const ATaskTemplFixedData*)&src;

	m_pszSignature = new task_char[MAX_TASK_NAME_LEN];
	memset(m_pszSignature, 0, sizeof(task_char)*MAX_TASK_NAME_LEN);

	if (src.m_pszSignature)
		memcpy(m_pszSignature, src.m_pszSignature, sizeof(task_char)*MAX_TASK_NAME_LEN);

	m_Award_S = _Award_S;
	m_Award_F = _Award_F;

	m_AwByRatio_S = _AwByRatio_S;
	m_AwByRatio_F = _AwByRatio_F;

	m_AwByItems_S = _AwByItems_S;
	m_AwByItems_F = _AwByItems_F;

	m_AwByCount_S = _AwByCount_S;
	m_AwByCount_F = _AwByCount_F;

	m_tmStart = _tmStart;
	m_tmEnd = _tmEnd;

	m_PremItems = _PremItems;
	m_GivenItems = _GivenItems;
	m_ItemsWanted = _ItemsWanted;

	m_MonsterSummoned = _MonsterSummoned;

	m_TeamMemsWanted = _TeamMemsWanted;
	m_MonsterWanted = _MonsterWanted;

	if (src.m_pwstrDescript)
	{
		m_pwstrDescript = new task_char[wcslen(src.m_pwstrDescript)+1];
		wcscpy(m_pwstrDescript, src.m_pwstrDescript);
		g_ulNewCount++;
	}

	if (src.m_pwstrOkText)
	{
		m_pwstrOkText = new task_char[wcslen(src.m_pwstrOkText)+1];
		wcscpy(m_pwstrOkText, src.m_pwstrOkText);
		g_ulNewCount++;
	}

	if (src.m_pwstrNoText)
	{
		m_pwstrNoText = new task_char[wcslen(src.m_pwstrNoText)+1];
		wcscpy(m_pwstrNoText, src.m_pwstrNoText);
		g_ulNewCount++;
	}

	if (src.m_pwstrTribute)
	{
		m_pwstrTribute = new task_char[wcslen(src.m_pwstrTribute)+1];
		wcscpy(m_pwstrTribute, src.m_pwstrTribute);
		g_ulNewCount++;
	}

	if (src.m_pwstrHintText)
	{
		m_pwstrHintText = new task_char[wcslen(src.m_pwstrHintText)+1];
		wcscpy(m_pwstrHintText, src.m_pwstrHintText);
		g_ulNewCount++;
	}

	if (src.m_pwstrCanDeliverText)
	{
		m_pwstrCanDeliverText = new task_char[wcslen(src.m_pwstrCanDeliverText)+1];
		wcscpy(m_pwstrCanDeliverText, src.m_pwstrCanDeliverText);
		g_ulNewCount++;
	}

	if (src.m_ulPremTitleCount)
	{
		m_PremTitles = new short[src.m_ulPremTitleCount];
		memcpy(m_PremTitles, src.m_PremTitles, src.m_ulPremTitleCount * sizeof(short));
		m_ulPremTitleCount = src.m_ulPremTitleCount;
	}
	else
	{
		m_PremTitles = NULL;
		m_ulPremTitleCount = 0;
	}

	//Added by Ford.W 2010-06-03.
	memcpy( m_szFilePath, src.m_szFilePath, TASK_MAX_PATH );

	//======Canceled by Ford.W 2010-06-25.
	//copy the parent task template.
/*	if( src.m_pParent )
	{
		m_pParent = new ATaskTempl;
		if( !m_pParent )
		{
			return *this;
		}
		*m_pParent = *(src.m_pParent);
	}
	else
	{
		m_pParent = NULL;
	}
	
	//copy the previous sibling task template.
	if( src.m_pPrevSibling )
	{
		m_pPrevSibling = new ATaskTempl;
		if( !m_pPrevSibling )
		{
			return *this;
		}
		*m_pPrevSibling = *(src.m_pPrevSibling);
	}
	else
	{
		m_pPrevSibling = NULL;
	}

	//copy the next sibling task template.
	if( src.m_pNextSibling )
	{
		m_pNextSibling = new ATaskTempl;
		if( !m_pPrevSibling )
		{
			return *this;
		}
		*m_pNextSibling = *(src.m_pNextSibling);
	}
	else
	{
		m_pNextSibling = NULL;
	}

	//copy the first child task template.
	if( src.m_pFirstChild )
	{
		m_pFirstChild = new ATaskTempl;
		if( !m_pFirstChild )
		{
			return *this;
		}
		*m_pFirstChild = *(src.m_pFirstChild);
	}
	else
	{
		m_pFirstChild = NULL;
	}
*/
	//======Canceled end.
	
	//Added by Ford.W 2010-06-25.
#ifdef TASK_TEMPL_EDITOR //Added by Ford.W 2010-06-28.
	if( m_bCopySubTask )
	{
		CopyAllSubTask( src );
	}
#endif //Added end of TASK_TEMPL_EDITOR. 2010-06-28.
	//Added end.

	//
	m_uDepth = src.m_uDepth;
	m_ulMask = src.m_ulMask;
	m_nSubCount = src.m_nSubCount;
	m_uValidCount = src.m_uValidCount;
	
	// set the talk_procs.
	m_DelvTaskTalk = src.m_DelvTaskTalk;
	m_UnqualifiedTalk = src.m_UnqualifiedTalk;
	m_DelvItemTalk = src.m_DelvItemTalk;
	m_ExeTalk = src.m_ExeTalk;
	m_AwardTalk = src.m_AwardTalk;

	//Add end.

	return *this;
}

#endif

//Modified by Ford.W 2010-06-28.
#ifdef TASK_TEMPL_EDITOR

//Added by Ford.W 2010-06-25.
ATaskTempl* ATaskTempl::CreateNewSubTask( const ATaskTempl& srcTaskTempl  )
{
	//get the source task template name.
	const wchar_t* srcName = srcTaskTempl.GetName();
	wchar_t destName[1024];

	wchar_t strID[100];
//	_itow( srcTaskTempl.GetID(), strID, 10 );//Canceled by Ford.W 2010-06-28.
	//Added by Ford.W 2010-06-28.Get the current Max ID, and increase the value by 1.
	_itow( g_TaskIDMan.GetMaxID()+1, strID, 10 );

	wcscpy( destName, srcName );
	wcsncat( destName, strID, 100 );
//	wcsncat( destName, (const wchar_t*)("_复制"), 100 );
	
	size_t len = wcslen(destName) + 1;
	char strNameMultiBytes[2048];
	WideCharToMultiByte( CP_ACP, 0, destName, len, strNameMultiBytes, 2*len, NULL, NULL );

	unsigned long ulID = g_TaskIDMan.CreateID(AString(strNameMultiBytes));

	if (ulID == 0)
	{
		//AfxMessageBox(_T("创建新任务ID失败！请检查任务名是否重复"), MB_ICONSTOP);
		return NULL;
	}

	ATaskTempl* pTask = new ATaskTempl;
	g_ulNewCount++;
	pTask->Init();	

	//copy the main value.
	*pTask = srcTaskTempl;

	//reset the unique properties.
	pTask->SetName( destName );
	pTask->m_ID = ulID;

	//return the new created instance.
	return pTask;
}

void ATaskTempl::CopyAllSubTask( const ATaskTempl& srcTaskTempl )
{
	//get the first child of the source task template.
	const ATaskTempl* pChild = srcTaskTempl.m_pFirstChild;

	//deal with all the children.
	while (pChild)
	{
		//copy the value of the child to the current task template.
		ATaskTempl* currentSubTaskTempl = CreateNewSubTask( *pChild );
		if( currentSubTaskTempl )
		{

			AddSubTaskTempl( currentSubTaskTempl );
		}
		//get the next child of the srcTaskTempl.
		pChild = pChild->m_pNextSibling;
	}	
}
//Added end.
#endif //TASK_TEMPL_EDITOR




