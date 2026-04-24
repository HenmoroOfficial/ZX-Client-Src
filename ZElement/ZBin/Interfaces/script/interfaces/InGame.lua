local UIManTemplate = UIManTemplate
local DlgApi  = DlgApi
local GameAi = GameAi
local Format = string.format

InGame = UIManTemplate:new({this = "InGame"});

function InGame:Init()	
    self:RegisterEvent(	LEVT_FIRST_ENTER_GAME	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_PLAYER_IS_MOVING	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_PLAYER_ADJUST_ORIENT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_PLAYER_LOCK_VISION	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_EMOTION_ACTION	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_EMOTION_WORD	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_CAMERA_MODE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_SAFELOCK	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_BOSSKEY	, Win_Help_Hint);
    --self:RegisterEvent(	LEVT_FIRST_TALK_NPC	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_MAIN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_TRAVEL	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_RISK	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_CLASSICAL	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_CHANLLENGE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_KILL_MONSTER	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_FORGE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_HERIT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_ASCETIC	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_LUPIN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_TEACHER	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_GUILD	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_TREASURE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_ADVENTURE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_LOVE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_READ	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_OBJECT_PICK	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_FAIL	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_LIMIT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_TRACK	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_MAP_AUTOTRACK	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_MAP_OPEN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_FINISH	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_AWARD	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TASK_SEARCH_LATEST	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_BAG_OPEN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_ITEM_EQUIP	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_MEDICINE_USE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_ITEM_SPECIAL	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_BAG_NEATEN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_BAG_EXPAND	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_ITEM_SPILT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_BAG_SELL	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_EXCHANGE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_CASH_EXCHANGE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_DEPOT_OPEN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_DEPOT_EXPAND	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_HORSE_EQUIP	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_ITEM_LOCK	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_BAG_PET	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_BAG_FASHDRESS	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_SKILL_SPECIAL	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_SKILL_POINT_ADD	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_SKILL_EDIT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_SKILL_CLEAR	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_ATTACK	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PK	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PK_SETTING	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PK_UP	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PK_DOWN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_BUFF	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_HP	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_DIE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_DIE_PUNISH	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_REST	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_KILL_MONSTER	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PICKUP	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_ABANDON	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_CHAT_OPEN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_MONSTER_SPECIAL	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_ENTER_MAP_NEW	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_MAP_TRANSFER	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TEAM_SETUP	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TEAM_SETTING	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_BATTLE_GUILD	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_BATTLE_CITY	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_MAIN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_VICE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_START	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_UPDATE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_MEDIUM	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_REFINE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_ENCHANT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_CLEAN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_COMBINE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_FRIEND_ADD	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_HUG	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_MARRY	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_FAMILY_CREATE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_FAMILY_JOIN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_GUILD_CREATE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_GUILD_JOIN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_FRIEND_DELETE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_BLACKLIST	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_ENEMY	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_STUDENT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_STUDENT_OUT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_TEACHER	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_STUDENT_ADD	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_FRIEND_SEEK	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_LUPIN_POINT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_GET	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_SPECIAL	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_USE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_EQUIPMENT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_EQUIPMENT_STRENGHTEN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_TAME	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_COMBINE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_STAR_UP	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_CHANGE_IMAGE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_CHANGE_NAME	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_CHANGE_MODE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_FEED	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_WEAK	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_HUNGRY	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_DIE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_LV_UP	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_COLLECT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_PRODUCE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_PET_CATCH	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_FLY_GET	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_FLY_MODE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_FLY_FIGHT_MODE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_FLY_LV_UP	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_FLY_SPEED_UP	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_ENCHASE_LV	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_ENCHASE_POINT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_BLOOD_ENCHANSE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_EQUIP_EXCHANGE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_EQUIP_BREAK	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_EQUIP_HIDE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_EQUIP_BIND	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_EQUIP_DEPART	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_EQUIP_DESTROY	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_EQUIP_SUIT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_MAIL	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_DESC_JOIN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_LV_UP	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_QSHOP_OPEN	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_QSHOP_ITEM_GET	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_RAFFLE	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_POPSHOP	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_CREDIT_ADD	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_RANK_PERSON_DROPOUT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_RANK_PERSONALL_DROPOUT	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_HIDE_ELSEPLAYER	, Win_Help_Hint);
    self:RegisterEvent(	LEVT_FIRST_ENTER_GAME	, Win_NewerHelp);
    self:RegisterEvent(	LEVT_FIRST_TALK_NPC2	, Win_NewerHelp);
    self:RegisterEvent(	LEVT_FIRST_MAP_AUTOTRACK2	, Win_NewerHelp);
    self:RegisterEvent(	LEVT_FIRST_ITEM_SPECIAL2	, Win_NewerHelp);
    self:RegisterEvent(	LEVT_LV_UP2	, Win_NewerHelp);
end

function InGame:Release()

end

function InGame:Tick()
	
end

function InGame:ProcessEvent()
	
end
----------------------------------
function InGame:ResizeWindows()
	if Win_TestTowergame then
		Win_TestTowergame:ResizeWindows();
	end
	
	if Win_TGllkGame then
		Win_TGllkGame:ResizeWindows();
	end
	
--	if Win_TGadventure then
--		Win_TGadventure:ResizeWindows();
--	end
end