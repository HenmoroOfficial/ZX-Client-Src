--local require = require
--local tpl = require("DlgTemplate")
--require("Widget")
local aPlayer   = AUIPlayer
local tpl = DlgTemplate
local CGameApi = GameApi
local CDlgApi  = DlgApi
local print    = DebugPrint
local aPicture  = AUIPicture
local aButton   = AUIButton


local datamember =
{
    m_Picture = 0,
    m_EquipImg = {},
    m_PackImg = {},
    m_Angle = 0,
    m_LeftTurn,
    m_RightTurn,
}





Win_Inventory= tpl:new(datamember)

function Win_Inventory:OnInitDialog()
    self.m_Picture =  aPlayer:new({this = self:GetDlgItem("playerdemo")})
    self.m_Picture:SetProperty("Image File", "Icon\\PartFrame.tga")

    local i
    for i = 0, IVTRSIZE_EQUIPPACK-1 do
       local name = string.format("Equip_%02d", i)
       local item = self:GetDlgItem(name)
       if not item then break end
       self.m_EquipImg[i] = aPicture:new({this = item}) 
    end
    for i = 0, IVTRSIZE_PACK0-1 do
       local name = string.format("Item_%02d", i)
       local item = self:GetDlgItem(name)
       if not item then break end
       self.m_PackImg[i] = aPicture:new({this = item}) 
    end
    self.m_LeftTurn = aButton:new({this = self:GetDlgItem("Btn_LeftTurn")})
    self.m_RightTurn = aButton:new({this = self:GetDlgItem("Btn_RightTurn")})
    
    self:RegisterEvent("wear", self.Wear)
    self:RegisterEvent("normal", self.Normal)
    
end


function  Win_Inventory:OnShowDialog()
    local hostid = CGameApi.GetHostPlayerID()
    self.m_Picture:SetTemplateID(hostid)
end

function Win_Inventory:OnTick()
    local  i, v
    for i, v in ipairs(self.m_EquipImg) do
        CGameApi.SetIvtrIcon(v.this, IVTRTYPE_EQUIPPACK, i)    
    end
    for i, v in ipairs(self.m_PackImg) do
        CGameApi.SetIvtrIcon(v.this, IVTRTYPE_PACK, i)    
    end
    local ticktime = CGameApi.GetTickTime()
    
    if(self.m_LeftTurn:GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK) 
    then
        self.m_Angle = (self.m_Angle + 360 - ticktime /6 )%360
    else if(self.m_RightTurn:GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK) 
        then
            self.m_Angle = (self.m_Angle +  ticktime /6 ) % 360            
        end
    end    
    
    self.m_Picture:SetAngle(self.m_Angle)
    self.m_Picture:UpdateEquips()
end

function Win_Inventory:Wear()
    if not CGameApi.IsFashionMode() then
        CGameApi.SwitchFashion()
    end
end

function Win_Inventory:Normal()
    if CGameApi.IsFashionMode() then
        CGameApi.SwitchFashion()
    end
end