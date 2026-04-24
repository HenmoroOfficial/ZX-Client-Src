--local require = require

--local AUIObject = require("Widget")
local AUIObject = AUIObject
local CDlgApi = DlgApi

-------------------aui player--------------------------
AUIPlayer = AUIObject:new()

function AUIPlayer:SetTemplateID(id)
   CDlgApi.UIPLSetTemplate(self.this, id)
end

function AUIPlayer:SetAngle(angle)
   CDlgApi.UIPLSetAngle(self.this, angle)
end

function AUIPlayer:UpdateEquips()
   CDlgApi.UIPLUpdateEquips(self.this)
end

--return AUIPlayer