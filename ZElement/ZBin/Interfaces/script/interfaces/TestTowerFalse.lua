local DlgTemplate = DlgTemplate
local DlgApi  = DlgApi

Win_TestTowerFalse= DlgTemplate:new({this = "Win_TestTowerFalse"})


function Win_TestTowerFalse:Init()
    self:RegisterEvent("mainmenu",self.OnCancel);
    self:RegisterEvent("exitgame",self.OnExitGame);
	  
end


function Win_TestTowerFalse:OnShowDialog()
    
    DlgApi.ShowDialog(self.this,true);	

end


function Win_TestTowerFalse:OnCancel()

      DlgApi.ShowDialog(self.this, false);
      Win_TowerDefense:OnShowDialog();
end
--------------------------------------------
function Win_TestTowerFalse:OnExitGame()
      DlgApi.ShowDialog(self.this, false);
end
