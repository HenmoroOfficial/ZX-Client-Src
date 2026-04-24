local DlgTemplate = DlgTemplate
local DlgApi  = DlgApi
local GameAI = GameAI

Win_TGllkFail = DlgTemplate:new({this = "Win_TGllkFail"});

function Win_TGllkFail:Init()

	self:RegisterEvent("return",self.ReturnMain );

end

function Win_TGllkFail:OnShowDialog()
    DlgApi.ShowDialog(self.this,true);
    
    local pos = DlgApi.GetDialogProp("Win_TGllkGame");
	local props = { canmove = false,canoutofwindow = false };
	DlgApi.SetDialogProp(self.this,props);
	DlgApi.SetDialogPosition(self.this,(pos.x+pos.width*0.28),(pos.y+pos.height*0.42));
end

function Win_TGllkFail:ReturnMain()
	DlgApi.ShowDialog(self.this,false);

end