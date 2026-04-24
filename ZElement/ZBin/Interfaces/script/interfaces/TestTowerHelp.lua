local DlgTemplate = DlgTemplate
local DlgApi  = DlgApi

Win_TestTowerHelp= DlgTemplate:new({this = "Win_TestTowerHelp",wnum = 1})

function Win_TestTowerHelp:Init()
    
	self:RegisterEvent("IDCANCEL",self.OnCancel);
	self:RegisterEvent("right",self.OnRight);
	self:RegisterEvent("left",self.OnLeft);
  
end


function Win_TestTowerHelp:OnShowDialog(latewindow)
    
    DlgApi.SetItemText(self.this,"AText_Rule",TD_TextHelpRule);
    
    DlgApi.SetImageFile(self.this,"Pic_Weapon",TD_HelpWPic[self.wnum][1],1);
    DlgApi.SetImageFile(self.this,"Pic_WeaponName",TD_HelpWPic[self.wnum][2],1);
    
	DlgApi.SetItemText(self.this,"AText_WeaponInfo",TD_TextHelpWeapon[self.wnum]);
    
	if self.wnum==1 then
	    DlgApi.EnableItem(self.this,"Btn_ToLeft",false);
	    DlgApi.EnableItem(self.this,"Btn_ToRight",true);
	elseif self.wnum == #TD_HelpWPic then
	    DlgApi.EnableItem(self.this,"Btn_ToLeft",true);
	    DlgApi.EnableItem(self.this,"Btn_ToRight",false);
	else
	    DlgApi.EnableItem(self.this,"Btn_ToLeft",true);
	    DlgApi.EnableItem(self.this,"Btn_ToRight",true);	    
	end
	
    DlgApi.ShowDialog(self.this,true);

end

function Win_TestTowerHelp:OnRight()
    if self.wnum<#TD_HelpWPic then
        self.wnum = self.wnum+1;
    else
        self.wnum = #TD_HelpWPic;
    end
    DlgApi.SetImageFile(self.this,"Pic_Weapon",TD_HelpWPic[self.wnum][1],1);
    DlgApi.SetImageFile(self.this,"Pic_WeaponName",TD_HelpWPic[self.wnum][2],1);
    
	DlgApi.SetItemText(self.this,"AText_WeaponInfo",TD_TextHelpWeapon[self.wnum]);    
	
	if self.wnum==1 then
	    DlgApi.EnableItem(self.this,"Btn_ToLeft",false);
	    DlgApi.EnableItem(self.this,"Btn_ToRight",true);
	elseif self.wnum == #TD_HelpWPic then
	    DlgApi.EnableItem(self.this,"Btn_ToLeft",true);
	    DlgApi.EnableItem(self.this,"Btn_ToRight",false);
	else
	    DlgApi.EnableItem(self.this,"Btn_ToLeft",true);
	    DlgApi.EnableItem(self.this,"Btn_ToRight",true);	    
	end
    
end
function Win_TestTowerHelp:OnLeft()
    if self.wnum>1 then
        self.wnum = self.wnum-1;
    else
        self.wnum = 1;
    end
    DlgApi.SetImageFile(self.this,"Pic_Weapon",TD_HelpWPic[self.wnum][1],1);
    DlgApi.SetImageFile(self.this,"Pic_WeaponName",TD_HelpWPic[self.wnum][2],1);
    
	DlgApi.SetItemText(self.this,"AText_WeaponInfo",TD_TextHelpWeapon[self.wnum]); 
	
	if self.wnum==1 then
	    DlgApi.EnableItem(self.this,"Btn_ToLeft",false);
	    DlgApi.EnableItem(self.this,"Btn_ToRight",true);
	elseif self.wnum == #TD_HelpWPic then
	    DlgApi.EnableItem(self.this,"Btn_ToLeft",true);
	    DlgApi.EnableItem(self.this,"Btn_ToRight",false);
	else
	    DlgApi.EnableItem(self.this,"Btn_ToLeft",true);
	    DlgApi.EnableItem(self.this,"Btn_ToRight",true);	    
	end
end

function Win_TestTowerHelp:OnCancel()
	DlgApi.ShowDialog(self.this,false);
end
