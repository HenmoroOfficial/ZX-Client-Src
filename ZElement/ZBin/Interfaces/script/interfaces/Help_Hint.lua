local DlgTemplate = DlgTemplate
local DlgApi  = DlgApi
local SecretarySystemRemind = SecretarySystemRemind

local datamember = 
{
	this = "Win_Help_Hint",
	m_curEvent = 0,
	m_curMsgIndex = 0,
	m_counter = 0,
	m_bUsed  = false,
	
}


Win_Help_Hint= DlgTemplate:new(datamember)


function Win_Help_Hint:Init()
    
--    self:RegisterEvent("CmdHelp",self.OnCmdHelp);
--    self:RegisterEvent("CmdSearch",self.OnCmdSearch);
--    self:RegisterEvent("CmdMemo", self.OnCmdMemo);
    self:RegisterEvent("Btn_Help", self.OnCmdToMain);
	self:RegisterEvent("Btn_Close", self.OnCmdClose);
	

    self:RegisterEvent(	LEVT_FIRST_ENTER_GAME	,self.OnFirstEnterGame	);
--    self:RegisterEvent(	LEVT_PLAYER_IS_MOVING	,self.OnIsMoving	);
--    self:RegisterEvent(	LEVT_PLAYER_ADJUST_ORIENT	,self.OnIsAdjustOrient	);
    self:RegisterEvent(	LEVT_PLAYER_LOCK_VISION	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_EMOTION_ACTION	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_EMOTION_WORD	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_CAMERA_MODE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_SAFELOCK	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_BOSSKEY	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TALK_NPC	,self.OnCommandEvent );
    self:RegisterEvent(	LEVT_FIRST_TASK_MAIN	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_TRAVEL	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_RISK	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_CLASSICAL	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_CHANLLENGE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_KILL_MONSTER	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_FORGE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_HERIT	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_ASCETIC	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_LUPIN	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_TEACHER	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_GUILD	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_TREASURE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_ADVENTURE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_LOVE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_READ	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_OBJECT_PICK	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_FAIL	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_LIMIT	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_TRACK	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_MAP_AUTOTRACK	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_MAP_OPEN	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_FINISH	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_AWARD	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TASK_SEARCH_LATEST	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_BAG_OPEN	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_ITEM_EQUIP	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_MEDICINE_USE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_ITEM_SPECIAL	,self.OnCommandEventWithArg	);
    self:RegisterEvent(	LEVT_FIRST_BAG_NEATEN	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_BAG_EXPAND	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_ITEM_SPILT	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_BAG_SELL	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_EXCHANGE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_CASH_EXCHANGE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_DEPOT_OPEN	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_DEPOT_EXPAND	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_HORSE_EQUIP	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_ITEM_LOCK	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_BAG_PET	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_BAG_FASHDRESS	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_SKILL_SPECIAL	,self.OnCommandEventWithArg	);
    self:RegisterEvent(	LEVT_FIRST_SKILL_POINT_ADD	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_SKILL_EDIT	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_SKILL_CLEAR	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_ATTACK	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PK	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PK_SETTING	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PK_UP	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PK_DOWN	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_BUFF	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_HP	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_DIE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_DIE_PUNISH	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_REST	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_KILL_MONSTER	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PICKUP	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_ABANDON	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_CHAT_OPEN	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_MONSTER_SPECIAL	,self.OnCommandEventWithArg	);
    self:RegisterEvent(	LEVT_FIRST_ENTER_MAP_NEW	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_MAP_TRANSFER	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TEAM_SETUP	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TEAM_SETTING	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_BATTLE_GUILD	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_BATTLE_CITY	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_MAIN	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_VICE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_START	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_UPDATE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_MEDIUM	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_REFINE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_ENCHANT	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_CLEAN	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TALISMAN_COMBINE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_FRIEND_ADD	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_HUG	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_MARRY	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_FAMILY_CREATE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_FAMILY_JOIN	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_GUILD_CREATE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_GUILD_JOIN	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_FRIEND_DELETE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_BLACKLIST	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_ENEMY	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_STUDENT	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_STUDENT_OUT	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_TEACHER	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_STUDENT_ADD	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_FRIEND_SEEK	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_LUPIN_POINT	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_GET	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_SPECIAL	,self.OnCommandEventWithArg	);
    self:RegisterEvent(	LEVT_FIRST_PET_USE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_EQUIPMENT	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_EQUIPMENT_STRENGHTEN	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_TAME	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_COMBINE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_STAR_UP	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_CHANGE_IMAGE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_CHANGE_NAME	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_CHANGE_MODE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_FEED	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_WEAK	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_HUNGRY	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_DIE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_LV_UP	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_COLLECT	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_PRODUCE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_PET_CATCH	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_FLY_GET	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_FLY_MODE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_FLY_FIGHT_MODE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_FLY_LV_UP	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_FLY_SPEED_UP	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_ENCHASE_LV	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_ENCHASE_POINT	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_BLOOD_ENCHANSE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_EQUIP_EXCHANGE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_EQUIP_BREAK	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_EQUIP_HIDE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_EQUIP_BIND	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_EQUIP_DEPART	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_EQUIP_DESTROY	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_EQUIP_SUIT	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_MAIL	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_DESC_JOIN	,self.OnCommandEventWithArg	);
    self:RegisterEvent(	LEVT_LV_UP	,self.OnCommandEventWithArg	);
    self:RegisterEvent(	LEVT_FIRST_QSHOP_OPEN	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_QSHOP_ITEM_GET	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_RAFFLE	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_POPSHOP	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_FIRST_CREDIT_ADD	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_RANK_PERSON_DROPOUT	,self.OnCommandEvent	);
    self:RegisterEvent(	LEVT_RANK_PERSONALL_DROPOUT	,self.OnCommandEvent	);
	self:RegisterEvent(	LEVT_FIRST_HIDE_ELSEPLAYER	,self.OnCommandEvent	);

end

function Win_Help_Hint:OnCmdClose()
	DlgApi.ShowDialog(self.this,false);
	self.m_counter = 0;
	self.m_bUsed  = false;
end


function Win_Help_Hint:OnCmdToMain()
   DlgApi.ShowDialog(self.this, false);
   self.m_counter = 0;
   self.m_bUsed = false;
   DlgApi.ShowDialog("Win_Help_Fairy", true); 
end

function Win_Help_Hint:OnCommandEvent(event)
    if( m_bUsed) then
        if SecretarySystemRemind[event] then
   		    DlgApi.SetItemText("Win_Help_Hint2", "Txt_Hint", SecretarySystemRemind[event]);
   			DlgApi.ShowDialog("Win_Help_Hint2", true); 
        end
	else 
	    if SecretarySystemRemind[event] then
     	    DlgApi.SetItemText(self.this, "Txt_Hint", SecretarySystemRemind[event]);
   	    	DlgApi.ShowDialog(self.this, true); 
	    end
	end
	return false;
end


function Win_Help_Hint:OnFirstEnterGame(event)
	m_curEvent = LEVT_FIRST_ENTER_GAME;
	if (SecretarySystemRemind[event]) then
   		DlgApi.SetItemText(self.this, "Txt_Hint", SecretarySystemRemind[event] or "");
   		DlgApi.ShowDialog(self.this, true); 
	end
	return false;
end

function Win_Help_Hint:OnIsMoving(event)
    if(m_curEvent == LEVT_FIRST_ENTER_GAME and SecretarySystemRemind[event]) then
   		DlgApi.SetItemText(self.this, "Txt_Hint", SecretarySystemRemind[event] or "");
   		DlgApi.ShowDialog(self.this, true); 
   		m_curEvent = LEVT_PLAYER_IS_MOVING;
	end
end

--function Win_Help_Hint:OnIsJumping(event)
--    if ( m_curEvent == LEVT_PLAYER_IS_MOVING) then
--		DlgApi.SetItemText(self.this, "Txt_Hint", SecretarySystemRemind[event] or "");
--   		DlgApi.ShowDialog(self.this, true);
--   		m_curEvent =  LEVT_PLAYER_IS_JUMPING
--	end
--end

function Win_Help_Hint:OnIsAdjustOrient(event)
	if( m_curEvent == LEVT_PLAYER_IS_MOVING and SecretarySystemRemind[event]) then
   		DlgApi.SetItemText(self.this, "Txt_Hint", SecretarySystemRemind[event] or "");
   		DlgApi.ShowDialog(self.this, true);
   		m_curEvent =  0
	end
end

--function Win_Help_Hint:OnIsAdjustDistance(event)
--    if(m_curEvent == LEVT_PLAYER_ADJUST_ORIENT) then
--   		DlgApi.SetItemText(self.this, "Txt_Hint", SecretarySystemRemind[event] or "");
--   		DlgApi.ShowDialog(self.this, true);
--   		m_curEvent =  0
--    end
--end

function Win_Help_Hint:OnCommandEventWithArg(event, id)
    if( m_bUsed) then
        if (SecretarySystemRemind[event] and SecretarySystemRemind[event][id]) then
	        DlgApi.SetItemText("Win_Help_Hint2", "Txt_Hint", SecretarySystemRemind[event][id] or "");
   			DlgApi.ShowDialog("Win_Help_Hint2", true);
        end
   	else
   	    if (SecretarySystemRemind[event] and SecretarySystemRemind[event][id]) then
   			DlgApi.SetItemText(self.this, "Txt_Hint", SecretarySystemRemind[event][id] or "");
   			DlgApi.ShowDialog(self.this, true);
   	    end
	end
end

----------------------------------------------------------------------------------
----------------------------------------------------------------------------------

function Win_Help_Hint:Tick(dwDeltaTime)
    self.m_counter  = self.m_counter + dwDeltaTime;
    if( self.m_counter > 20000 ) then
        self.m_counter = 0;
        self.m_bUsed = false;
        DlgApi.ShowDialog(self.this, false);
	end
end



