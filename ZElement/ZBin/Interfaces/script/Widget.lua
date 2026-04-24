--[[
  description:   the template for all ui widgets
  author:      Wang Kuiwu (wangkuiwu@world2.cn)
  Copyright (c) , All Rights Reserved.
  warning:   Please do NOT modify this file.  Contact me if any question.
  警告   ：  请不要修改本文件。如有问题请与我联系。
]]--

local CDlgApi = DlgApi      --from c
local print   = DebugPrint
local k, v
local pairs, setmetatable, type = pairs, setmetatable, type


-------------------create instance--------------------------
local function CopyTable(src)
  local inst = {}
  for  k,  v in pairs(src) do
     if type(v) == "table"
     then
        inst[k] = CopyTable(v)
     else
        inst[k] = v
     end
  end
  local mt = getmetatable(src)
  setmetatable(inst, src)

  return inst
end
function CreateObj(template,  obj)
  local inst = obj or {}
  for  k,  v in pairs(template) do
     if (not inst[k]) and type(v) ~= "function"  then
        if type(v) == "table"  and v ~= template
        then
            inst[k] = CopyTable(v)
        else
            inst[k] = v
        end
      end
  end

  setmetatable(inst, template)
  --template.__index = template
  return inst
end
-------------------aui object--------------------------
AUIObject =
{
    this = 0,
    --type = "AUIObject"
}


function AUIObject:new(datamember)
  self.__index = self
  return CreateObj(self, datamember)
end

function AUIObject:SetText(txt)
  --print("AUIObject this "..tostring(self.this).."\n")
  CDlgApi.SetDlgItemText(self.this, txt)
end

function AUIObject:GetText()
  return CDlgApi.GetDlgItemText(self.this)
end


function AUIObject:SetAlign(align)
  --print("AUIObject this "..tostring(self.this).."\n")
  CDlgApi.SetDlgItemAlign(self.this, align)
end

function AUIObject:SetColor(color)
  CDlgApi.SetDlgItemColor(self.this, color)
end

function AUIObject:Enable(state)
  CDlgApi.EnableDlgItem(self.this, state)
end

function AUIObject:GetState()
  return CDlgApi.GetDlgItemState(self.this)
end

function AUIObject:SetProperty(name, value)
    CDlgApi.SetItemProperty(self.this, name, value)
end


-------------------aui label--------------------------
AUILabel = AUIObject:new()

-------------------aui picture--------------------------
AUIPicture = AUIObject:new()

function AUIPicture:RenderPlayerDemonstration(player, angle)
   CDlgApi.PicRenderPlayerDemonstration(self.this, player, angle)
end
-------------------aui checkbox--------------------------
AUICheckBox = AUIObject:new()

function AUICheckBox:Check(bCheck)
   CDlgApi.SetDlgItemState(self.this, bCheck)
end

function AUICheckBox:IsCheck()
   return CDlgApi.GetDlgItemState(self.this)
end

-------------------aui progress--------------------------
AUIProgress = AUIObject:new()

function AUIProgress:SetProgress(val)
   CDlgApi.SetItemProgress(self.this, val)
end

-------------------aui editbox--------------------------
AUIEditBox = AUIObject:new()

function AUIEditBox:SetNumberOnly( bNumber)
    CDlgApi.EdtSetNumberOnly(self.this, bNumber)
end
-------------------aui button--------------------------
AUIButton = AUIObject:new()

-------------------aui combobox--------------------------
AUIComboBox = AUIObject:new()

function AUIComboBox:AddString(txt)
   CDlgApi.ComboAddString(self.this, txt)
end

function AUIComboBox:GetCurSel()
   return CDlgApi.ComboGetCurSel(self.this)
end
-------------------aui listbox--------------------------
AUIListBox = AUIObject:new()

function AUIListBox:ResetContent()
   CDlgApi.LstResetContent(self.this)
end

function AUIListBox:AddString(txt)
   return CDlgApi.LstAddString(self.this, txt)
end

function AUIListBox:SetItemData(index, data, subindex, name)
   CDlgApi.LstSetItemData(self.this, index, data, subindex, name)
end

function AUIListBox:GetItemData(index, subindex, name)
   return CDlgApi.LstGetItemData(self.this, index, subindex, name)
end



--return AUIObject