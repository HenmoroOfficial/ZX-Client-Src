--if not DlgTemplate then
	--为了方便策划查错，使用包里的脚本时不能这样加载，否则客户端可能会崩
--	CreateObj = function() return {eventMap = {}} end
--	LuaInclude("dlgtemplate.lua")
--end

local DlgTemplate = DlgTemplate
local DlgApi  = DlgApi
local SecretarySystemRemind = SecretarySystemRemind
local pageid = 0
local m_FrameMax = 1

local datamember = 
{
	this = "Win_NewerHelp",
	m_curEvent = 0,
	m_curMsgIndex = 0,
	m_FrameMax = 1,
}


Win_NewerHelp = DlgTemplate:new(datamember)


function Win_NewerHelp:Init()
    self:RegisterEvent(LEVT_FIRST_ENTER_GAME,self.OnFirstEnterGame);
    self:RegisterEvent(	LEVT_FIRST_TALK_NPC2	,self.OnFirstTalkNpc);
    self:RegisterEvent(LEVT_FIRST_MAP_AUTOTRACK2,self.OnFirstMapAutotrack);
    self:RegisterEvent(LEVT_FIRST_ITEM_SPECIAL2,self.OnGetItemID);
    self:RegisterEvent(LEVT_LV_UP2,self.OnLvUp);
    self:RegisterEvent("Btn_Prev",self.OnCmdPrev);
    self:RegisterEvent("Btn_Next",self.OnCmdNext);
end

function Win_NewerHelp:OnFirstEnterGame()---第一次进入游戏
    if SecretarySystemRemind[LEVT_FIRST_ENTER_GAME] then
        DlgApi.ShowDialog(self.this,true);
        DlgApi.SetImageFile(self.this,"Img_Pic","help\\NewerHelp_Move_1.dds",3);
        m_FrameMax = 3;
    end      
    return false;
end

function Win_NewerHelp:OnFirstTalkNpc()---第一次与NPC对话
      DlgApi.SetImageFile(self.this,"Img_Pic","help\\NewerHelp_Task_1.dds",3);
        m_FrameMax = 3;
      DlgApi.ShowDialog(self.this,true);
end


function Win_NewerHelp:OnFirstMapAutotrack()---第一次自动寻径(OK)
       DlgApi.SetImageFile(self.this,"Img_Pic","help\\NewerHelp_Combat_1.dds",3);
        m_FrameMax = 3;
       DlgApi.ShowDialog(self.this,true);
end    
    
function Win_NewerHelp:OnGetItemID(event,id)---第一次获得法宝主件八卦石4703、五色石30076
        if SecretarySystemRemind[event] and SecretarySystemRemind[event][id] then
            DlgApi.ShowDialog(self.this,true); 
        end       
        if id == 4703 or id == 30076 then
            DlgApi.SetImageFile(self.this,"Img_Pic","help\\NewerHelp_Talisman_1.dds",2);
        m_FrameMax = 2;
        end
end

function Win_NewerHelp:OnLvUp(event, level)---升级8、10、15、16、20、45、90
    if level == 8 then
        DlgApi.SetImageFile(self.this,"Img_Pic","help\\NewerHelp_level_8.dds",1); 		
        m_FrameMax = 1;
   	 		DlgApi.SelectImageFrame(self.this,"Img_Pic",0);
        DlgApi.ShowDialog(self.this,true);         
    end
    
    if level == 10 then
        DlgApi.SetImageFile(self.this,"Img_Pic","help\\NewerHelp_level_10.dds",2); 	
        m_FrameMax = 2;	
   	 		DlgApi.SelectImageFrame(self.this,"Img_Pic",0);
        DlgApi.ShowDialog(self.this,true);         
    end
    
    if level == 15 then
        DlgApi.SetImageFile(self.this,"Img_Pic","help\\NewerHelp_level_15.dds",1); 		
        m_FrameMax = 1;
   	 		DlgApi.SelectImageFrame(self.this,"Img_Pic",0);
        DlgApi.ShowDialog(self.this,true);         
    end
    
    if level == 16 then
        DlgApi.SetImageFile(self.this,"Img_Pic","help\\NewerHelp_level_16.dds",2); 			
        m_FrameMax = 2;
   	 		DlgApi.SelectImageFrame(self.this,"Img_Pic",0);
        DlgApi.ShowDialog(self.this,true);         
    end
    
    if level == 20 then
        DlgApi.SetImageFile(self.this,"Img_Pic","help\\NewerHelp_level_20.dds",2); 			
        m_FrameMax = 2;	
   	 		DlgApi.SelectImageFrame(self.this,"Img_Pic",0);
        DlgApi.ShowDialog(self.this,true);         
    end
    
    if level == 45 then
        DlgApi.SetImageFile(self.this,"Img_Pic","help\\NewerHelp_level_45.dds",1); 			
        m_FrameMax = 1;	
   	 		DlgApi.SelectImageFrame(self.this,"Img_Pic",0);
        DlgApi.ShowDialog(self.this,true);         
    end

    if level == 90 then
        DlgApi.SetImageFile(self.this,"Img_Pic","help\\NewerHelp_level_90.dds",1); 				
        m_FrameMax = 1;
   	 		DlgApi.SelectImageFrame(self.this,"Img_Pic",0);
        DlgApi.ShowDialog(self.this,true);         
    end

end

function Win_NewerHelp:OnCmdPrev()
    if pageid >0 then
    pageid = pageid-1;
    DlgApi.SelectImageFrame(self.this,"Img_Pic",pageid); 
    end
end

function Win_NewerHelp:OnCmdNext()
    if pageid<(m_FrameMax-1) then
    pageid = pageid+1;   
    DlgApi.SelectImageFrame(self.this,"Img_Pic",pageid);
    end
end 
      
