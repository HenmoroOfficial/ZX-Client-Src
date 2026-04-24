
local DlgTemplate = DlgTemplate
local DlgApi  = DlgApi
local GameAI = GameAI

Win_TGllkWin = DlgTemplate:new({this = "Win_TGllkWin"});

function Win_TGllkWin:Init()
    
    self:RegisterEvent("return",self.ReturnMain );

end


function Win_TGllkWin:OnShowDialog()
    DlgApi.ShowDialog(self.this,true);
    
    local pos = DlgApi.GetDialogProp("Win_TGllkGame");
	local props = { canmove = false,canoutofwindow = false };
	DlgApi.SetDialogProp(self.this,props);
	DlgApi.SetDialogPosition(self.this,(pos.x+pos.width*0.28),(pos.y+pos.height*0.42));
end

function Win_TGllkWin:ReturnMain()
    
	DlgApi.ShowDialog(self.this,false);

end