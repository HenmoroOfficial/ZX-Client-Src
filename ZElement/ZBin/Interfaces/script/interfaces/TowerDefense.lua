local DlgTemplate = DlgTemplate
local DlgApi  = DlgApi
local GameAI=GameAI
local Format = string.format
local TD_MapInfo = TD_MapInfo
local TD_TaskIndex = TD_TaskIndex

Win_TowerDefense= DlgTemplate:new( {this = "Win_TowerDefense",missionnum=1,Score ={},diff = 1,initflag = false, } )

function Win_TowerDefense:Init()

--	self:RegisterEvent("tgllk", self.ToLLK);
------------------------------------------------
	self:RegisterEvent("help", self.OnHelp);
	self:RegisterEvent("cancel", self.OnCancel);
	
	--鼠标信息
	self:RegisterEvent(WM_LBUTTONDOWN,self.OnLButtonDown);	--在游戏中左鼠标点击，创建
	self:RegisterEvent(WM_MOUSEMOVE, self.OnMouseMove);	
	
	
	for i=1,TD_PassNum+1 do
	    if TD_MapInfo[i] then
	        DlgApi.SetItemText(self.this,"Text_Map"..i,TD_MapInfo[i]);
	    end
	end
end

function Win_TowerDefense:GetMissionPar()
	local missionnum=1;
	local score ={ };
	for i = 1, TD_PassNum do
	    score[i] = 0;
	end	
	
	--从文件中读取
	local retdata = GameAI.LoadTDFile("towerdefense");
	
    if retdata ~= nil and #retdata < TD_PassNum+2 then	
    	local retnum = #retdata;
    	for i = 1,TD_PassNum do
    	    if i< retnum and type(retdata[i])=="number" then
    	        score[i] = retdata[i];
    	    end
    	end
    	
    	if type(retdata[retnum])=="number" and math.floor(retdata[retnum])>0.9 and math.floor(retdata[retnum])<TD_PassNum+1.1 then
    	    missionnum =  math.floor(retdata[retnum]);
    	end
    	--为增加一个关口时准备
    	if missionnum > TD_PassNum then
    	    missionnum = TD_PassNum;
    	end
    		--第四关增加的单独处理
    	if missionnum == 3 and retnum == 4 and score[3]>5000 then
    	    missionnum = 4;
    	end
    	
    end

	return missionnum,score;
end

function Win_TowerDefense:SaveMissionPar(enablenum,score,succflag)
    
    local retnum = enablenum;
    if succflag == true then
        retnum = retnum+1;
    end
    
    if retnum<self.missionnum then  --retnum>TD_PassNum or
        retnum = self.missionnum;
    end
    
    local retscore = {};
    for i = 1,TD_PassNum do
        retscore[i] = self.Score[i];
    end
    if score>self.Score[enablenum] then
       retscore[enablenum] = score;
    end
    
    retscore[TD_PassNum+1] = retnum;
    
    GameAI.SaveTDFile(retscore,"towerdefense");
    
end 

function Win_TowerDefense:OnShowDialog()
	
	--读分数和可用关    
    self.missionnum,self.Score= self:GetMissionPar();
	--显示难度
    if type(self.diff)=="number" and self.diff<=3 and self.diff>=1 then
        DlgApi.CheckRadioButton(self.this,1,self.diff);
    else
        self.diff = 1;
        DlgApi.CheckRadioButton(self.this,1,self.diff);
    end
    
    for i=1,self.missionnum do
       DlgApi.SelectImageFrame(self.this,"Pic_Map"..i,2);
    end

    --是否能够enable关的选择按钮
    for i = self.missionnum+1, TD_PassNum do
        DlgApi.SelectImageFrame(self.this,"Pic_Map"..i,0);
    end
    
    local score = 0;
    for i = 1,TD_PassNum do
        DlgApi.SetItemText(self.this,"Edit_Score"..i,tostring(self.Score[i]));
        score = score+self.Score[i]
    end
    DlgApi.SetItemText(self.this,"Edit_Score",tostring(score));

    
    DlgApi.ShowDialog(self.this,true);
    	

   
	------------------
	self.Gold = TD_Gold;

end

----------------------------------------------------

function Win_TowerDefense:OnHelp()
    
	Win_TestTowerHelp:OnShowDialog();	
	
end

function Win_TowerDefense:OnCancel()

	DlgApi.ShowDialog(self.this,false);

	for i = 1,#TD_TaskIndex do
	    if TD_TaskIndex[i] then
	        for j = 1,#TD_TaskIndex[i] do
	            if TD_TaskIndex[i][j] then
	            	GameAI.TaskGiveup(TD_TaskIndex[i][j]);
	            end
	        end
	    end
	end
	
	--********************为下一关的显示作准备 


	--读分数和可用关    
    self.missionnum,self.Score= self:GetMissionPar();
	--显示难度

    if type(self.diff)=="number" and self.diff<3.1 and self.diff>0.9 then
        DlgApi.CheckRadioButton(self.this,1,self.diff);
    else
        self.diff = 1;
        DlgApi.CheckRadioButton(self.this,1,self.diff);
    end
    
    for i=1,self.missionnum do
       DlgApi.SelectImageFrame(self.this,"Pic_Map"..i,2);
    end

    --是否能够enable关的选择按钮
    for i = self.missionnum+1, TD_PassNum do
        DlgApi.SelectImageFrame(self.this,"Pic_Map"..i,0);
    end
    local score = 0;
    for i = 1,TD_PassNum do
        DlgApi.SetItemText(self.this,"Edit_Score"..i,tostring(self.Score[i]));
        score = score+self.Score[i]
    end
    DlgApi.SetItemText(self.this,"Edit_Score",tostring(score));
    
    DlgApi.ShowDialog("Win_GameCasual",true);

end

function Win_TowerDefense:OnStart(i)
	
	self.diff = DlgApi.GetCheckedRadioButton(self.this,1);
	if self.diff<0.9 and self.diff>3.1 then
	    self.diff = 1;
	end
	Win_TestTowergame:OnShowDialog(i,self.diff);
	DlgApi.ShowDialog(self.this,false);

end

-----------------***********************---------------------------------
function Win_TowerDefense:OnMouseMove()
--    local mousex,mousey = GameAI.GetCursorPosition();	--鼠标绝对位置
--	local TestgameDlg = DlgApi.GetDialogProp(self.this);	--dialog的绝对位置 


	local DlgName,ItemName = DlgApi.GetHitItem();
	local nchosen = 0;
	if DlgName == self.this and ItemName~=nil then
	    if string.match(ItemName,"Pic_Map".."%d+") then
	        nchosen = string.match(ItemName,"%d+");
	    end
	end
	
	for i = 1, self.missionnum do
	    if math.abs(i-nchosen)<1.0e-2 then
	        DlgApi.SelectImageFrame(self.this,"Pic_Map"..i,1);
	    else
	        DlgApi.SelectImageFrame(self.this,"Pic_Map"..i,2);
	    end
	end

	return true;
end



function Win_TowerDefense:OnLButtonDown(itemName, wParam, x, y)
	local pathnum = nil;
	if string.match(itemName,"Pic_Map".."%d+") then
	    pathnum = string.match(itemName,"%d+");
	    pathnum = tonumber(pathnum);
	    if pathnum < self.missionnum+0.1 and pathnum>0 then
	        Win_TowerDefense:OnStart(pathnum);
	    end
	end
	return true;
end


------------------------******************---------------------------------
function Win_TowerDefense:MissionSuc(enablenum,score,succflag)
	
	-- 把当前的关卡的数据存入数据文件 


	Win_TowerDefense:SaveMissionPar(enablenum,score,succflag);

   	self:OnShowDialog();

	if self.Score[enablenum]<score then
	    self.Score[enablenum] = score;
	end
	
	local pathnum = enablenum;
	if succflag==true then
	    pathnum = pathnum+1;
	end
	
	if self.missionnum<pathnum then
	    self.missionnum=pathnum;
	end
	
	if self.missionnum>TD_PassNum then
		self.missionnum=TD_PassNum;
	end
	
	for i=1,self.missionnum do
	   DlgApi.SelectImageFrame(self.this,"Pic_Map"..i,2);
	end
    local score = 0;
    for i = 1,TD_PassNum do
        DlgApi.SetItemText(self.this,"Edit_Score"..i,tostring(self.Score[i]));
        score = score+self.Score[i];
    end
    DlgApi.SetItemText(self.this,"Edit_Score",tostring(score));	

end

function Win_TowerDefense:ResizeWindows()

end


-----------------------------------------------------------------------Tick

function Win_TowerDefense:Tick(dwDeltaTime)

	if self.initflag==false then
	--初始化的一些提前预设 


	   	--读分数和可用关    
        self.missionnum,self.Score= self:GetMissionPar();
    	--显示难度
        if type(self.diff)=="number" and self.diff<=3 and self.diff>=1 then
            DlgApi.CheckRadioButton(self.this,1,self.diff);
        else
            self.diff = 1;
            DlgApi.CheckRadioButton(self.this,1,self.diff);
        end
        
        for i=1,self.missionnum do
           DlgApi.SelectImageFrame(self.this,"Pic_Map"..i,2);
        end
    
        --是否能够enable关的选择按钮
        for i = self.missionnum+1, TD_PassNum do
            DlgApi.SelectImageFrame(self.this,"Pic_Map"..i,0);
        end		
        local score = 0;
        for i = 1,TD_PassNum do
            DlgApi.SetItemText(self.this,"Edit_Score"..i,tostring(self.Score[i]));
            score = score+self.Score[i]
        end
        DlgApi.SetItemText(self.this,"Edit_Score",tostring(score));	
        self.initflag = true;
	end

end

---------------------------------------------------------------------------------
--function Win_TowerDefense:ToLLK()
--	Win_TGllkGame:OnShowDialog();
--end










