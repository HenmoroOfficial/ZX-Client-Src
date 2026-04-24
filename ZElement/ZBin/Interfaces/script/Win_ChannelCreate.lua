--local tpl = require("DlgTemplate")
--require("Widget")

local tpl = DlgTemplate
local CGameApi = GameApi
local aCheck = AUICheckBox
local aEdit  = AUIEditBox
local aButton = AUIButton

local datamember =
{
m_EnablePw = 0, 
m_Name = 0,  
m_Password = 0,
m_Create = 0
}


Win_ChannelCreate =  tpl:new(datamember)

local queryCreate = "MB_QueryCreate"

function Win_ChannelCreate:OnInitDialog()
    self.m_EnablePw =  aCheck:new({this = Win_ChannelCreate:GetDlgItem("Chk_EnablePw") })
    self.m_Name = aEdit:new({this = Win_ChannelCreate:GetDlgItem("DEFAULT_Txt_ChName") })    
    self.m_Password = aEdit:new({this = Win_ChannelCreate:GetDlgItem("Txt_ChPw") })    
    self.m_Create =  aButton:new({this =Win_ChannelCreate:GetDlgItem("Btn_Create")})
   
    Win_ChannelCreate:RegisterEvent("enablepw", self.EnablePw )
    Win_ChannelCreate:RegisterEvent("confirm", self.Confirm )  
    Win_ChannelCreate:RegisterEvent(queryCreate, self.QueryCreate )             
end

function Win_ChannelCreate:OnShowDialog()
    self.m_Name:SetText("")
    self.m_Password:Enable(false)
    self.m_Password:SetText("")
    self.m_EnablePw:Check(false)
    self.m_Create:Enable(false)
end

function Win_ChannelCreate:EnablePw()
    self.m_Password:Enable(self.m_EnablePw:IsCheck())
    self.m_Password:SetText("")
end

function Win_ChannelCreate:OnTick()
    local name = self.m_Name:GetText()
    self.m_Create:Enable(name ~= '')
end

function Win_ChannelCreate:Confirm()
    local txt = Win_ChannelCreate:GetStringFromTable(654)
    GameUIMan:MessageBox(queryCreate, txt, MB_YESNO, 255, 255, 255, 160, self)
end

function Win_ChannelCreate:QueryCreate(iRet)
    if iRet == IDYES then
        Win_ChannelCreate:CreateChatRoom()
    end
end

function Win_ChannelCreate:CreateChatRoom()
    local name = self.m_Name:GetText()
    local pass = self.m_Password:GetText()
    CGameApi.CreateChatRoom(name, pass, 30)
    Win_ChannelCreate:Show(false)
end



