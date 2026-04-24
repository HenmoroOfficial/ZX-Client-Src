--if not DlgTemplate then
	--为了方便策划查错，使用包里的脚本时不能这样加载，否则客户端可能会崩
--	CreateObj = function() return {eventMap = {}} end
--	LuaInclude("dlgtemplate.lua")
--end

local DlgTemplate = DlgTemplate
local DlgApi  = DlgApi
local pageid = 0

local datamember = 
{
	this = "Win_NewerHelp2",
	m_FrameMax = 1,
	curContent = 0,
	
}

local list =
{
"移动篇","任务篇","战斗篇","法宝篇","技能篇","社交篇","角色篇","地图篇","系统篇","飞剑篇","精力篇",
}

Win_NewerHelp2 = DlgTemplate:new(datamember)

function Win_NewerHelp2:Init()
    self:RegisterEvent("Btn_Prev",self.OnCmdPrev);
    self:RegisterEvent("Btn_Next",self.OnCmdNext);
    self:RegisterEvent(WM_LBUTTONDOWN,self.OnLButtonDown);
end

function Win_NewerHelp2:ShowDialog()
    DlgApi.SetListText(self.this,"Lst_Type",list);
    
end

function Win_NewerHelp2:OnCmdPrev()
    if pageid >0 then
    pageid = pageid-1;
    DlgApi.SelectImageFrame(self.this,"Img_Pic",pageid); 
    end
end

function Win_NewerHelp2:OnCmdNext()
    if pageid<(m_FrameMax-1) then
    pageid = pageid+1;   
    DlgApi.SelectImageFrame(self.this,"Img_Pic",pageid);
    end
end 

function Win_NewerHelp2:OnLButtonDown()
		curContent = DlgApi.GetListCurLine(self.this, "Lst_Type");
		if(curContent==1) then
				DlgApi.SetImageFile(self.this,"Img_Pic","help\\NewerHelp_level_10.dds",2); 			
        m_FrameMax = 2;	
    end
		if(curContent==2) then
				DlgApi.SetImageFile(self.this,"Img_Pic","help\\NewerHelp_level_20.dds",2); 			
        m_FrameMax = 2;	
    end
end