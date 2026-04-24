--local tpl = require("DlgTemplate")
--require("Widget")
local tpl = DlgTemplate
local CGameApi = GameApi
local print    = DebugPrint
local aGameUIMan = GameUIMan
local aPicture   = AUIPicture

local datamember =
{
    m_Player = 0,
    m_PlayerPic = 0,
    m_EquipImg = {},
    
}

Win_EPEquip= tpl:new(datamember)

function Win_EPEquip:OnInitDialog()
    self.m_PlayerPic = aPicture:new({this =self:GetDlgItem("Pic_Part") })
    
    local i
    for i = 0, IVTRSIZE_EQUIPPACK-1 do
       local name = string.format("Equip_%02d", i)
       local item = self:GetDlgItem(name)
       if not item then break end
       self.m_EquipImg[i] = aPicture:new({this = item}) 
    end
    
    aGameUIMan:RegisterEvent(DLG_SHOW_EPEQUIP, self)
    self:RegisterEvent(DLG_SHOW_EPEQUIP, self.ShowMe)
end

function Win_EPEquip:ShowMe(id)
    self.m_Player = CGameApi.CreateLoginPlayer(id)
    CGameApi.SyncDemonstrationPlayer(self.m_Player, id)
    local  i, v
    for i, v in ipairs(self.m_EquipImg) do
        CGameApi.SetIvtrIcon(v.this, IVTRTYPE_EPEQUIPPACK, i)    
    end
    self:Show(true)
end

function Win_EPEquip:OnTick()
   self.m_PlayerPic:RenderPlayerDemonstration(self.m_Player, 0)
end

function Win_EPEquip:OnDestroy()
    if self.m_Player and self.m_Player ~= 0 then
        CGameApi.ReleaseECObject(self.m_Player)
        self.m_Player = nil
    end
end