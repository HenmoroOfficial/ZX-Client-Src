--local tpl = require("DlgTemplate")
--require("Widget")

local tpl = DlgTemplate
local CGameApi = GameApi
local print    = DebugPrint
local aButton  = AUIButton
local aCheck   = AUICheckBox
local aCombo   = AUIComboBox

local datamember =
{
    m_MoveLeft,
    m_MoveRight,
    m_MoveUp,
    m_MoveDown,
    m_MoveFront,
    m_MoveBack,
    m_FarFocus,
    m_NearFocus,
    m_HideUI,
    m_Scale
}


Win_Camera = tpl:new(datamember)

function Win_Camera:OnInitDialog()
     self.m_MoveLeft = aButton:new({this =Win_Camera:GetDlgItem("Btn_CameraMoveLeft")})
     self.m_MoveRight = aButton:new({this =Win_Camera:GetDlgItem("Btn_CameraMoveRight")})     
     self.m_MoveUp = aButton:new({this =Win_Camera:GetDlgItem("Btn_CameraMoveUp")})     
     self.m_MoveDown = aButton:new({this =Win_Camera:GetDlgItem("Btn_CameraMoveDown")})      
     self.m_MoveFront = aButton:new({this =Win_Camera:GetDlgItem("Btn_CameraMoveFront")})     
     self.m_MoveBack = aButton:new({this =Win_Camera:GetDlgItem("Btn_CameraMoveBack")})     
     self.m_FarFocus = aButton:new({this =Win_Camera:GetDlgItem("Btn_CameraFarFocus")})     
     self.m_NearFocus = aButton:new({this =Win_Camera:GetDlgItem("Btn_CameraNearFocus")})     
     self.m_HideUI    = aCheck:new({this = Win_Camera:GetDlgItem("Chk_HideUI")}) 
     self.m_Scale    =  aCombo:new({this = Win_Camera:GetDlgItem("Combo_Scale")})      
          
     local i;
     for i = 1, 8 do
         local temp = Win_Camera:GetStringFromTable(761)
         local txt  = string.format(temp, i)
         self.m_Scale:AddString(txt)     
     end
     self.m_HideUI:Check(true)
     Win_Camera:SetCanOutOfWindow(true)
     
     Win_Camera:RegisterEvent("printscr", self.PrintScr )
     Win_Camera:RegisterEvent("IDCANCEL", self.Cancel )
end

function Win_Camera:PrintScr()
    CGameApi.SetHideUIPrintMode(self.m_HideUI:IsCheck())
    local scale = self.m_Scale:GetCurSel()
    CGameApi.CaptureScreen(scale +1)
end

function Win_Camera:Cancel()
end

function Win_Camera:OnShowDialog()
    Win_Camera:AlignTo(AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_CENTER, 0,
                        AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_CENTER, 0)
end

function Win_Camera:OnTick()
    local scale = self.m_Scale:GetCurSel()
    if scale > 0 then
        self.m_HideUI:Check(true)        
    end
    
    if not CGameApi.IsSceneryMode() then
        Win_Camera:Show(false)
        return
    end
    local btn0, btn1, btn2, btn3, btn4, btn5, btn6
    btn0 = (self.m_MoveLeft:GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
    btn1 = (self.m_MoveRight:GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
    btn2 = (self.m_MoveFront:GetState()  == AUISTILLIMAGEBUTTON_STATE_CLICK)
            or (self.m_NearFocus:GetState()  == AUISTILLIMAGEBUTTON_STATE_CLICK)
    btn3 = (self.m_MoveBack:GetState()  == AUISTILLIMAGEBUTTON_STATE_CLICK)
            or (self.m_FarFocus:GetState()  == AUISTILLIMAGEBUTTON_STATE_CLICK)
    btn4 = (self.m_MoveUp:GetState()  == AUISTILLIMAGEBUTTON_STATE_CLICK)
    btn5 = (self.m_MoveDown:GetState()  == AUISTILLIMAGEBUTTON_STATE_CLICK)
    btn6 = (self.m_NearFocus:GetState()  == AUISTILLIMAGEBUTTON_STATE_CLICK)
            or (self.m_FarFocus:GetState()  == AUISTILLIMAGEBUTTON_STATE_CLICK)

    CGameApi.SetCameraKeyState(btn0, btn1, btn2, btn3, btn4, btn5, btn6)
end