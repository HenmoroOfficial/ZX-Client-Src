--[[
  description:   the template for all dialogs.
  author:      Wang Kuiwu (wangkuiwu@world2.cn)
  Copyright (c) , All Rights Reserved.
  warning:   Please do NOT modify this file.  Contact me if any question.
  警告   ：  请不要修改本文件。如有问题请与我联系。 
]]--

--require("Widget")


-----------------global func--------------
-- can make cpp define the DlgTemplate method  directly, not in another table,
-- Maybe better? NOT sure.

local CDlgApi = DlgApi      --from c
local print = DebugPrint




-----------------template implementation--------------
DlgTemplate =
{
this = 0,
eventMap = {}
}


-------------------create instance--------------------------
function  DlgTemplate:new(datamember)
  self.__index = self
  local inst = CreateObj(self, datamember)
  return inst
end

-------------------standard method--------------------------
--[[
function DlgTemplate:new(sName)
    inst =
    {  -- here define the data member
    this = 0,
    name = "DlgTemplate"
    }
    
    if type(sName) == 'string' 
    then  inst.name = sName end

    setmetatable(inst, self)
    self.__index = self
    return inst
end
]]--

--[[
function DlgTemplate:SetDlgItemText(item, text)
   CDlgApi.SetDlgItemText(item, text)
end
function DlgTemplate:GetDlgItemText(item)
   return CDlgApi.GetDlgItemText(item)
end
function DlgTemplate:SetDlgItemColor(item, color)
   CDlgApi.SetDlgItemColor(item, color)
end

function DlgTemplate:SetDlgItemAlign(item, align)
   CDlgApi.SetDlgItemAlign(item, align)
end

function DlgTemplate:SetItemProgress(item, prog)
   CDlgApi.SetItemProgress(item, prog)
end

function DlgTemplate:GetDlgItemState(item)
   return  CDlgApi.GetDlgItemState(item)
end

function DlgTemplate:SetDlgItemState(item, state)
   CDlgApi.SetDlgItemState(item, state)
end

function DlgTemplate:EnableDlgItem(item, bEnable)
    return CDlgApi.EnableDlgItem(item, bEnable)
end

function DlgTemplate:TVInsertItem(tv, itemText, parent, after)
    return CDlgApi.TVInsertItem(tv, itemText, parent, after)
end


function DlgTemplate:TVGetSelItem(tv)
   return CDlgApi.TVGetSelItem(tv) 
end 

function DlgTemplate:TVGetItemText(tv, item)
   return CDlgApi.TVGetItemText(tv, item)
end

function DlgTemplate:LstResetContent(lst)
   CDlgApi.LstResetContent(lst)
end

function DlgTemplate:LstAddString(lst, text)
   return CDlgApi.LstAddString(lst, text)
end

function DlgTemplate:LstSetItemData(lst, index, data, subindex,name)
   CDlgApi.LstSetItemData(lst, index, data, subindex, name)
end

function DlgTemplate:LstGetItemData(lst, index, subindex, name)
    return CDlgApi.LstGetItemData(lst, index, subindex, name)
end

function DlgTemplate:LstGetCurSel(lst)
    return CDlgApi.LstGetCurSel(lst)
end

function DlgTemplate:LstSetCurSel(lst, index)
    CDlgApi.LstSetCurSel(lst, index)
end

function DlgTemplate:ComboAddString(combo, text)
    return CDlgApi.ComboAddString(combo, text)
end

function DlgTemplate:ComboGetCurSel(combo)
    return CDlgApi.ComboGetCurSel(combo)
end

]]--

function DlgTemplate:Show(bShow)
  CDlgApi.ShowDlg(self.this, bShow)
end

function DlgTemplate:IsShow()
   return CDlgApi.IsDlgShow(self.this)
end


function DlgTemplate:GetDlgItem(itemName)
    return CDlgApi.GetDlgItem(self.this, itemName)
end

function DlgTemplate:AlignTo(insideX, alignX, offsetX, insideY, alignY, offsetY, baseDlg)
    CDlgApi.AlignTo(self.this, insideX,alignX, offsetX, insideY, alignY, offsetY, baseDlg)
end


function DlgTemplate:GetStringFromTable(id)
    return CDlgApi.GetStringFromTable(self.this, id)
end

function DlgTemplate:SetCanOutOfWindow(bVal)
    CDlgApi.SetCanOutOfWindow(self.this, bVal)
end

function DlgTemplate:RegisterEvent(event, handler)
    if  event and handler then
        self.eventMap[event] = handler
    else  -- need report ???
        print("lua register event:"..tostring(event).." handler:"..tostring(handler).." fail\n")
    end
end 

function DlgTemplate:UnRegisterEvent(event)
    self.eventMap[event] = nil    
end 


function DlgTemplate:OnEventMap( event, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
    local handler = self.eventMap[event]
    if handler   then  handler(self, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)  end
end 

function DlgTemplate:OnCancel()
      self.Show(self, false)
end
-----------------overload method----------------------
function DlgTemplate:OnInitDialog()
end

--[[
function DlgTemplate:OnTick()
end 
]]--

function DlgTemplate:OnShowDialog()
end

function DlgTemplate:OnDestroy()
end


--[[
---let lua dispatch the event,
--- or  let the c dipatch the event ???
function DlgTemplate:OnMouseWheel(fwKeys, zDelta, x, y, item)
end

function DlgTemplate:OnLButtonUp(vKey, x, y, item)
end

function DlgTemplate:OnLButtonDown(vKey, x, y, item)
end

function DlgTemplate:OnRButtonUp(vKey, x, y, item)
end

function DlgTemplate:OnRButtonDown(vKey, x, y, item)
end

function DlgTemplate:OnLButtonDblClk(vKey, x, y, item)
end

function DlgTemplate:OnMouseWheel(vKey, zDelta, x, y, item)
end

function DlgTemplate:OnMouseMove(vKey, x, y, item)
end
--]]--


--return DlgTemplate