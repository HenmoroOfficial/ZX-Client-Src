--local tpl = require("DlgTemplate")
--require("Widget")
--require("GameUIMan")

local tpl = DlgTemplate
local CGameApi = GameApi
local print    = DebugPrint
local aLabel = AUILabel
local aList  = AUIListBox
local aEdit  = AUIEditBox
local aUIMan = GameUIMan

local datamember =
{
    m_RoomTotal,
    m_AllChannel,
    m_CreatorName,
    m_Number,
    m_RoomIDStart,
}

Win_ChannelJoin = tpl:new(datamember)

function Win_ChannelJoin:OnInitDialog()
 	self.m_RoomTotal = aLabel:new({this = self:GetDlgItem("Txt_RoomTotal")})
 	self.m_AllChannel = aList:new({this = self:GetDlgItem("Lst_AllChannel")})
 	self.m_CreatorName = aEdit:new({this = self:GetDlgItem("Default_Txt_CreatorName")})
 	self.m_Number = aEdit:new({this = self:GetDlgItem("Txt_Number")})
 	self.m_RoomIDStart = aEdit:new({this = self:GetDlgItem("Txt_RoomIDStart")})
 	self.m_Number:SetNumberOnly(true)
 	self.m_RoomIDStart:SetNumberOnly(true)
 	
 	aUIMan:RegisterEvent(CHATROOM_LIST_COMING, self)
 	self:RegisterEvent(CHATROOM_LIST_COMING, self.OnListComing)
 	aUIMan:RegisterEvent(CHATROOM_INFO, self)
 	self:RegisterEvent(CHATROOM_INFO, self.OnAddRoom)
 	
end

function Win_ChannelJoin:OnShowDialog()
    self.m_CreatorName:SetText("")
    self.m_RoomIDStart:SetText("1")
    self.m_RoomTotal:SetText("0")
    CGameApi.OnEvent(CHATROOM_GET_LIST, 0, false)
end

function Win_ChannelJoin:OnListComing(count)
   self.m_AllChannel:ResetContent()
   self.m_RoomTotal:SetText(tostring(count))
   --print("ready for new channels\n")
end

function Win_ChannelJoin:OnAddRoom(id, bLock, subject, owner, number, capacity)
   --print("ready for new channels\n")

   local lck = bLock and "ÓÐËø" or "ÆÕÍ¨"
   local txt = string.format("%d\t%s\t%s\t%s\t%d/%d", id, lck, subject, owner, number, capacity)
   local i =  self.m_AllChannel:AddString(txt)
   self.m_AllChannel:SetItemData(i-1, id, 0)
   self.m_AllChannel:SetItemData(i-1, bLock and 1 or 0, 1)
end
