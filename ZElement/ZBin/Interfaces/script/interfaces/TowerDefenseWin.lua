local DlgTemplate = DlgTemplate
local DlgApi  = DlgApi

Win_TowerDefenseWin= DlgTemplate:new({this = "Win_TowerDefenseWin",curgame=0,score=0,diff=0})


function Win_TowerDefenseWin:Init()
    
    self:RegisterEvent("next",self.OnNext);
    self:RegisterEvent("main",self.OnToMain);
	  
end


function Win_TowerDefenseWin:OnShowDialog(game,score,dif)
    
    self.curgame = game;
    self.score = score;
    self.diff = dif;
    
    if self.curgame > TD_PassNum-0.1 then
        DlgApi.EnableItem(self.this,"Btn_Next",false);
    else
        DlgApi.EnableItem(self.this,"Btn_Next",true);
    end

    local ret = DlgApi.GetDialogProp("Win_TestTowergame");
    DlgApi.SetDialogPosition(self.this,ret.x,ret.y);    
    DlgApi.ShowDialog(self.this,true,true,true);	

end


function Win_TowerDefenseWin:OnNext()
    
    DlgApi.ShowDialog(self.this,false);
	Win_TowerDefense:MissionSuc(self.curgame,self.score,true);
	DlgApi.ShowDialog("Win_TowerDefense",false);	
	Win_TowerDefense:OnStart(self.curgame+1);	
	
	if TD_TaskIndex and TD_TaskIndex[self.diff] and TD_TaskIndex[self.diff][self.curgame] then
	    GameAI.TaskFin(TD_TaskIndex[self.diff][self.curgame]);
	end		
end
--------------------------------------------
function Win_TowerDefenseWin:OnToMain()
    
    DlgApi.ShowDialog(self.this,false);
    DlgApi.ShowDialog("Win_TestTowergame",false);
	Win_TowerDefense:MissionSuc(self.curgame,self.score,true);

	if TD_TaskIndex and TD_TaskIndex[self.diff] and TD_TaskIndex[self.diff][self.curgame] then
	    GameAI.TaskFin(TD_TaskIndex[self.diff][self.curgame]);
	end			

end

function Win_TowerDefenseWin:Tick(dwDeltaTime)

	local ret = DlgApi.GetDialogProp(self.this);
	DlgApi.SetDialogPosition("Win_TestTowergame",ret.x,ret.y);

end
