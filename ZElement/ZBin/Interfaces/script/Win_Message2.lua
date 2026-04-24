--local tpl = require("DlgTemplate")

--require("Widget")
local tpl = DlgTemplate
local aCheck = AUICheckBox

--Win_Message2 = CreateDlg(tpl, {m_ChkForceQuit = 0})
Win_Message2 = tpl:new({m_ForceQuit = 0})

function  Win_Message2:OnInitDialog()
    self.m_ForceQuit = aCheck:new({this = Win_Message2:GetDlgItem("Chk_Forcequit")})
    Win_Message2:RegisterEvent("confirm", self.confirm)
end

function Win_Message2:confirm()
    GameApi.Exit(self.m_ForceQuit:IsCheck())
    Win_Message2:Show(false)
end

