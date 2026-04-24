--local require = require
--local tpl = require("DlgTemplate")
--require("Widget")

local tpl = DlgTemplate
local CGameApi = GameApi
local print    = DebugPrint
local aLabel = AUILabel
local aProgress = AUIProgress

Win_Target = tpl:new({m_Name, m_HP, ST_MAX = 7})

function Win_Target:OnInitDialog()
    self.m_Name = aLabel:new({this = self:GetDlgItem("Txt_Name")})
    self.m_Name:SetAlign(AUIFRAME_ALIGN_CENTER)
    self.m_HP =  aProgress:new({this =self:GetDlgItem("Prg_HP")})
    
    
    self:RegisterEvent("IDCANCEL", self.cancel)
end

function  Win_Target:cancel()
   CGameApi.RespawnMessage(self.this)
end

function  Win_Target:OnTick()
   local idSel=CGameApi.GetSelectTarget()
   local typeSel = CGameApi.GetIDType(idSel)
   if typeSel ~= IDTYPE_UNKNOWN then
        local name, namecolor, curHP, maxHP, curMP, 
              maxMP = CGameApi.GetRoleBasicProp(idSel)
        if not name then
            self:Show(false)
            return
        end     
        self.m_Name:SetText(name)
        self.m_Name:SetColor(namecolor)
        self.m_HP:SetProgress(AUIPROGRESS_MAX*curHP/maxHP)
        if self.m_MP then
            self.m_MP:SetProgress(AUIPROGRESS_MAX*curMP/maxMP)
        end
        
        local i;
        for i = 1, self.ST_MAX do
            local img = self:GetDlgItem("st_"..i)
            if not img then   break  end        
            local buffID = CGameApi.GetBuffID(idSel, i-1)
            CGameApi.SetBuffIcon(img, buffID)
        end          
   end
end

