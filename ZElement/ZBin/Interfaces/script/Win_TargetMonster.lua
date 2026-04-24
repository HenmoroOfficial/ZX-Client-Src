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

Win_TargetMonster = tpl:new(datamember)

function Win_TargetMonster:OnInitDialog()
    tpl.OnInitDialog(self)
    self.m_MP =  aProgress:new({this =self:GetDlgItem("Prg_MP")})
end