--local require = require
--require("Win_Target")

local tpl = Win_Target
local CGameApi = GameApi
local print    = DebugPrint
local aProgress = AUIProgress

local datamember = 
{
    m_MP,
}

Win_TargetPlayer = tpl:new(datamember)

function Win_TargetPlayer:OnInitDialog()
    tpl.OnInitDialog(self)
    self.m_MP =  aProgress:new({this =self:GetDlgItem("Prg_MP")})
    self:RegisterEvent(WM_RBUTTONUP, self.OnRButtonUp)
end

function  Win_Target:OnRButtonUp(vKey, x, y, name)
   if name == "Pic_HeadFrame" then
     local idSel=CGameApi.GetSelectTarget()
     CGameApi.PopupPlayerContextMenu(idSel, x, y)
    end
end
