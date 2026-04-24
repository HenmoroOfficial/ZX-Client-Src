local DlgTemplate = DlgTemplate
local DlgApi  = DlgApi

Win_TowerDefenseFail= DlgTemplate:new({this = "Win_TowerDefenseFail",curgame = 0,Score = 0})


function Win_TowerDefenseFail:Init()
    
    self:RegisterEvent("again",self.OnAgain);
    self:RegisterEvent("main",self.OnToMain);
	  
end


function Win_TowerDefenseFail:OnShowDialog(game,score)
    
    local ret = DlgApi.GetDialogProp("Win_TestTowergame");
    DlgApi.SetDialogPosition(self.this,ret.x,ret.y);
    DlgApi.ShowDialog(self.this,true,true,true);	
    self.curgame = game;
    self.Score = score;
    	
end

function Win_TowerDefenseFail:OnAgain()
	
    DlgApi.ShowDialog(self.this,false);
	Win_TowerDefense:MissionSuc(self.curgame,self.Score,false);
	DlgApi.ShowDialog("Win_TowerDefense",false);	
	Win_TowerDefense:OnStart(self.curgame);
	--DlgApi.ShowDialog(self.this,false);
	
end
--------------------------------------------
function Win_TowerDefenseFail:OnToMain()

    DlgApi.ShowDialog(self.this,false);	
    DlgApi.ShowDialog("Win_TestTowergame",false);
	Win_TowerDefense:MissionSuc(self.curgame,self.Score,false);
	--DlgApi.ShowDialog(self.this,false);
	
end

function Win_TowerDefenseFail:Tick(dwDeltaTime)

	local ret = DlgApi.GetDialogProp(self.this);
	DlgApi.SetDialogPosition("Win_TestTowergame",ret.x,ret.y);

end
