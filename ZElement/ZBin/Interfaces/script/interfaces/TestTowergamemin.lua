local DlgTemplate = DlgTemplate
local DlgApi  = DlgApi

Win_TestTowergamemin= DlgTemplate:new({this = "Win_TestTowergamemin",gametype = nil})


function Win_TestTowergamemin:Init()
    
    self:RegisterEvent("return",self.OnReturn);
	  
end


function Win_TestTowergamemin:OnShowDialog(rettype)
    
    self.gametype = rettype;
    
    DlgApi.SetDialogProp(self.this,{ canmove = false, canoutofwindow = false});
    DlgApi.ShowDialog(self.this,true);	
    
    	
end

function Win_TestTowergamemin:OnReturn()

	if self.gametype == nil then
	    DlgApi.ShowDialog(self.this,false);
		DlgApi.ShowDialog("Win_TestTowergame",true);
	elseif self.gametype == "TGllk" then
	    DlgApi.ShowDialog(self.this,false);
		DlgApi.ShowDialog("Win_TGllkGame",true);	    
	end
end
--------------------------------------------
