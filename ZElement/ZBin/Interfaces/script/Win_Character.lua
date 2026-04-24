--local tpl = require("DlgTemplate")
--require("Widget")

local tpl = DlgTemplate
local aLabel = AUILabel
local aPicture = AUIPicture
local print = DebugPrint

local data =
{
   m_CharName = 0,
   m_CharLevel = 0,
   m_Profession  = 0,
   m_PK  = 0,
   m_Mate = 0,
   m_Title = 0,
   m_Exp  = 0,
   m_Faction = 0,
   m_HP =0,
   m_MP =0,
   m_Damage =0,
   m_Defense = 0,
   m_Definition = 0,
   m_Evade = 0,
   m_Critical = 0,
   m_CriticalDamage = 0,
   m_StunRes = 0,
   m_SilenceRes = 0,
   m_WeaknessRes = 0,
   m_SleepRes = 0,
   m_SlowRes = 0,
   m_EnlaceRes = 0,
   m_Img = 0
}

--Win_Character = CreateDlg(tpl, data)
Win_Character  = tpl:new(data)

function Win_Character:OnInitDialog()
    self.m_CharName=  aLabel:new({this = Win_Character:GetDlgItem("Txt_CharName") })
    self.m_CharLevel=  aLabel:new({this = Win_Character:GetDlgItem("Txt_CharLevel") })
    self.m_Profession=  aLabel:new({this = Win_Character:GetDlgItem("Txt_Profession") })
    self.m_PK=  aLabel:new({this = Win_Character:GetDlgItem("Txt_PK") })
    self.m_Mate=  aLabel:new({this = Win_Character:GetDlgItem("Txt_Mate") })
    self.m_Title=  aLabel:new({this = Win_Character:GetDlgItem("Txt_Title") })
    self.m_Exp=  aLabel:new({this = Win_Character:GetDlgItem("Txt_Exp") })
    self.m_Faction=  aLabel:new({this = Win_Character:GetDlgItem("Txt_Faction") })
    self.m_HP=  aLabel:new({this = Win_Character:GetDlgItem("Txt_HP") })
    self.m_MP=  aLabel:new({this = Win_Character:GetDlgItem("Txt_MP") })
    self.m_Damage=  aLabel:new({this = Win_Character:GetDlgItem("Txt_Damage") })
    self.m_Defense=  aLabel:new({this = Win_Character:GetDlgItem("Txt_Defense") })
    self.m_Definition=  aLabel:new({this = Win_Character:GetDlgItem("Txt_Definition") })
    self.m_Evade=  aLabel:new({this = Win_Character:GetDlgItem("Txt_Evade") })
    self.m_Critical=  aLabel:new({this = Win_Character:GetDlgItem("Txt_Critical") })
    self.m_CriticalDamage=  aLabel:new({this = Win_Character:GetDlgItem("Txt_CriticalDamage") })
    self.m_StunRes=  aLabel:new({this = Win_Character:GetDlgItem("Txt_StunRes") })
    self.m_SilenceRes=  aLabel:new({this = Win_Character:GetDlgItem("Txt_SilenceRes") })
    self.m_WeaknessRes=  aLabel:new({this = Win_Character:GetDlgItem("Txt_WeaknessRes") })
    self.m_SleepRes=  aLabel:new({this = Win_Character:GetDlgItem("Txt_SleepRes") })
    self.m_SlowRes=  aLabel:new({this = Win_Character:GetDlgItem("Txt_SlowRes") })
    self.m_EnlaceRes=  aLabel:new({this = Win_Character:GetDlgItem("Txt_EnlaceRes") })

    self.m_Img=  aPicture:new({this = Win_Character:GetDlgItem("Img_Char") })

    self.m_CharName:SetAlign(AUIFRAME_ALIGN_CENTER)
end
--global table to contain player property
--the data are filled by c api
HostProp = 
{
   name = 0,
   level = 0,
   profession  = 0,
   pk  = '',
   mate = '',
   title = '',
   exp  = 0,
   faction = 0,
   hp =0,
   mp =0,
   damage =0,
   defense = 0,
   definition = 0,
   evade = 0,
   critical = 0,
   criticalDamage = 0,
   stunRes = 0,
   silenceRes = 0,
   weaknessRes = 0,
   sleepRes = 0,
   slowRes = 0,
   enlaceRes = 0
}

function Win_Character:Update()
   GameApi.GetHostProp()  --fill the HostProp
   self.m_CharName:SetText(HostProp.name)
   --print("level this "..tostring(self.m_CharLevel.this).."\n")
   self.m_CharLevel:SetText(HostProp.level)
   self.m_Profession:SetText(HostProp.profession)
   self.m_PK:SetText(HostProp.pk)
   self.m_Mate:SetText(HostProp.mate)
   self.m_Title:SetText(HostProp.title)
   self.m_Exp:SetText(HostProp.exp)
   self.m_Faction:SetText(HostProp.faction)
   self.m_HP:SetText(HostProp.hp)
   self.m_MP:SetText(HostProp.mp)
   self.m_Damage:SetText(HostProp.damage)
   self.m_Defense:SetText(HostProp.defense)
   self.m_Definition:SetText(HostProp.definition)
   self.m_Evade:SetText(HostProp.evade)
   self.m_Critical:SetText(HostProp.critical)
   self.m_CriticalDamage:SetText(HostProp.criticalDamage)
   self.m_StunRes:SetText(HostProp.stunRes)
   self.m_SilenceRes:SetText(HostProp.silenceRes)
   self.m_WeaknessRes:SetText(HostProp.weaknessRes)
   self.m_SleepRes:SetText(HostProp.sleepRes)
   self.m_SlowRes:SetText(HostProp.slowRes)
   self.m_EnlaceRes:SetText(HostProp.enlaceRes)
end
--[[
function Win_Character:Render()
    local bShow = Win_Character:IsShow()
    if bShow then  Win_Character:Update() end
end
]]--

function Win_Character:OnTick()
    Win_Character:Update()
end

