
local DlgTemplate = DlgTemplate
local DlgApi  = DlgApi
local GameAI = GameAI

local TGllk_MapSize  = TGllk_MapSize
local TGllk_BlockSize = TGllk_BlockSize
local TGllk_MapOrigin = TGllk_MapOrigin
local TGllk_HintNum = TGllk_HintNum
local TGllk_Pic = TGllk_Pic

local TGllk_Time = TGllk_Time
local TGllk_ScoreReward = TGllk_ScoreReward
local TGllk_ScoreHint = TGllk_ScoreHint
local TGllk_ScoreWash = TGllk_ScoreWash
local TGllk_ScoreTime = TGllk_ScoreTime
local TGllk_RewardThresholdTime = TGllk_RewardThresholdTime
local TGllk_Reward = TGllk_Reward
local TGllk_RewardArea = TGllk_RewardArea
local TGllk_RewardRate = TGllk_RewardRate
local TGllk_RewardRate = TGllk_RewardRate
local TGllk_Sound = TGllk_Sound


--
local datamember = {

this = "Win_TGllkGame",
prepress={},
level=0,
difflevel=0,
gamestate=nil,

map_h=0,
map_w=0,
timestore=0;
blocksum=nil,


hintnum=nil,
washnum=nil,

BlockMatrix = {},
BlockInMatrix={},
BlockList={},
DeleteFlag={},

mapscale=nil,

hintstore={},

textcounter =-1,

cnroute={},
btnflag = false,
soundflag = true,

linelist = {},

hintcounter = nil,
hintframe = 0,

disapearlist = {},
disapearnum = 0,
falsenum = 0,
rewardcounter = -1,
succnum = 0,

dialogflag = false,

maxdiff = 1,
maxscore = 0,

scorestore = 0,

levelendflag = false;

};

Win_TGllkGame = DlgTemplate:new(datamember);

function Win_TGllkGame:Init()
	self:RegisterEvent("close",self.OnCancel );
	self:RegisterEvent("min",self.OnToGameMin);
    self:RegisterEvent("wash",self.OnWashArrange);
    self:RegisterEvent("hint",self.OnHint);
    self:RegisterEvent("pause",self.OnPause);
    self:RegisterEvent("continue",self.OnContinue);
    self:RegisterEvent("abandon",self.OnAbandon);
    
    self:RegisterEvent(WM_LBUTTONDOWN,self.OnLButtonDown);
    self:RegisterEvent(WM_RBUTTONDOWN,self.OnRButtonDown);
    
    --代用资源
    self:RegisterEvent("next",self.OnNextLevel);
    self:RegisterEvent("sound",self.OnSoundControl);
    
end
-------------------------
function Win_TGllkGame:OnShowDialog()
	
	--任务线
	if TGllk_TaskNum then 
		for i = 1,#TGllk_TaskNum do
		    if TGllk_TaskNum[i] and type(TGllk_TaskNum[i])=="number" then
		        GameAI.TaskGiveup(TGllk_TaskNum[i]);
		    end
		end
	end  
		
	self:ResizeWindows();

	self:DataInit();
	self:ItemInit();
	DlgApi.ShowDialog(self.this,true);
	
	local props = { canmove = false,canoutofwindow = false };
	DlgApi.SetDialogProp(self.this,props);

end

function Win_TGllkGame:OnToGameMin()
	Win_TestTowergamemin:OnShowDialog("TGllk");
	DlgApi.ShowDialog(self.this,false);
end

function Win_TGllkGame:OnSoundControl()
    if self.soundflag == true then
        self.soundflag = false;
        DlgApi.ShowItem(self.this,"Btn_SoundOff",false);
        DlgApi.ShowItem(self.this,"Btn_SoundOn",true);
    else
        self.soundflag = true;
        DlgApi.ShowItem(self.this,"Btn_SoundOn",false);
        DlgApi.ShowItem(self.this,"Btn_SoundOff",true);
    end        
end
------Init-------------------
function Win_TGllkGame:DataInit()

	self.level=1;
	self.difflevel=1;
	self.gamestate=TGllk_Wait;
	
	self.BlockMatrix={};
	self.BlockInMatrix={};
	self.BlockList={};
	
	self.DeleteFlag={};

	self.prepress={};
   	self.hintnum=0;
	self.washnum=0;	
    self.timestore=0;
    
    self.linelist = {};
    for i = 1,3 do
        self.linelist[i] = {counter = -1,period = 100};
    end
    self.levelendflag = false;
    self.maxdiff = 1;
    self.maxscore = 0;
    
    self.scorestore = 0;
end

----------------------------
function Win_TGllkGame:ItemInit()
    local i = math.random(TGllk_BkNum);
    local ifile = "TinyGame\\Maps\\Scene"..i..".tga";
    DlgApi.SetImageFile(self.this,"Pic_BackMap",ifile,1);

    DlgApi.ShowItem(self.this,"Rdo_11",true);
    DlgApi.ShowItem(self.this,"Rdo_12",true);
    DlgApi.ShowItem(self.this,"Rdo_13",true);
    DlgApi.EnableItem(self.this,"Rdo_11",true);
    DlgApi.EnableItem(self.this,"Rdo_12",true);
    DlgApi.EnableItem(self.this,"Rdo_13",true);    
	DlgApi.CheckRadioButton(self.this,1,1);
	
	DlgApi.ShowItem(self.this,"Btn_Start",true);
	DlgApi.ShowItem(self.this,"Btn_ToNext",false);
	
	DlgApi.ShowItem(self.this,"Edit_BackGround",false);--背景方块
	
    DlgApi.ShowItem(self.this,"Btn_Pause",false);  
    DlgApi.ShowItem(self.this,"Btn_Continue",false);  
    DlgApi.ShowItem(self.this,"Btn_Wash",false);
    DlgApi.ShowItem(self.this,"Btn_Hint",false);
    DlgApi.ShowItem(self.this,"Btn_Abandon",false);
    DlgApi.ShowItem(self.this,"Edit_HintNum",false);
    DlgApi.ShowItem(self.this,"Edit_WashNum",false);    
    
    DlgApi.ShowItem(self.this,"Pic_Pressed",false);
    DlgApi.ShowItem(self.this,"Pic_Hint1",false);
    DlgApi.ShowItem(self.this,"Pic_Hint2",false);
    DlgApi.ShowItem(self.this,"Pic_MemberPic",false);    
    DlgApi.ShowItem(self.this,"Pic_Disapear",false);

    DlgApi.ShowItem(self.this,"Prog_Time",false);
    DlgApi.ShowItem(self.this,"Pic_TimeText",false);

	DlgApi.ShowItem(self.this,"Edit_Score",false);
	DlgApi.ShowItem(self.this,"Lab_TextScore",false);
	DlgApi.ShowItem(self.this,"Lab_LevelText",false);	
	DlgApi.ShowItem(self.this,"Edit_Level",false);

	DlgApi.ShowItem(self.this,"Btn_SoundOff",false);
	DlgApi.ShowItem(self.this,"Btn_SoundOn",false);
	        
	self.hintcounter = 0;        
  	DlgApi.CreateControl(self.this,"Pic_Pressed","Pressed");
    DlgApi.SetImageFile(self.this,"Pressed",TGllk_PressedPic,TGllk_PressedPicNum);
  	DlgApi.SelectImageFrame(self.this,"Pressed",0);
    DlgApi.ShowItem(self.this,"Pressed",false);
 
	DlgApi.CreateControl(self.this,"Pic_Hint1","Hint1");
    DlgApi.SetImageFile(self.this,"Hint1",TGllk_HintPic,TGllk_HintPicNum);
   	DlgApi.SelectImageFrame(self.this,"Hint1",0);
    DlgApi.ShowItem(self.this,"Hint1",false);

	DlgApi.CreateControl(self.this,"Pic_Hint2","Hint2");
    DlgApi.SetImageFile(self.this,"Hint2",TGllk_HintPic,TGllk_HintPicNum);
    DlgApi.SelectImageFrame(self.this,"Hint2",0);
    DlgApi.ShowItem(self.this,"Hint2",false);

    DlgApi.CreateControl(self.this,"Pic_Line","Line1");
    DlgApi.SetImageFile(self.this,"Line1",TGllk_LinePic,1);
    DlgApi.ShowItem(self.this,"Line1",false);
	
    DlgApi.CreateControl(self.this,"Pic_Line","Line2");
    DlgApi.SetImageFile(self.this,"Line2",TGllk_LinePic,1);
    DlgApi.ShowItem(self.this,"Line2",false);

    DlgApi.CreateControl(self.this,"Pic_Line","Line3");
    DlgApi.SetImageFile(self.this,"Line3",TGllk_LinePic,1);
    DlgApi.ShowItem(self.this,"Line3",false);    
        
    DlgApi.ShowItem(self.this,"Pic_Line",false);
    DlgApi.SetItemText(self.this,"Lab_TextOut"," ");

	--显示最高分
	local retdata = GameAI.LoadTDFile("tgllk");
	if retdata~= nil and #retdata == 2 then
	    self.maxscore = retdata[1];
	    if self.maxdiff < 5 then
	        self.maxdiff = retdata[2];
	    end
	    if self.maxdiff > 3 then
	        self.maxdiff = 3; 
	    end
	end
	
	for i = self.maxdiff+1,3 do
	    DlgApi.EnableItem(self.this,"Rdo_1"..i,false);
	end
	DlgApi.SetItemText(self.this,"Edit_MaxScore",tostring(self.maxscore));
	DlgApi.ShowItem(self.this,"Edit_MaxScore",true);
	DlgApi.ShowItem(self.this,"Lab_MaxScoreText",true);			    
end

------------------------
--
function Win_TGllkGame:OnNextLevel()

    	if self.levelendflag == true then
    		self.scorestore = self.scorestore + self.timestore*TGllk_ScoreTime[self.difflevel]/1000;
    		self.levelendflag = false;
    	end		
		local i = math.random(TGllk_BkNum);
    	local ifile = "TinyGame\\Maps\\Scene"..i..".tga";
    	DlgApi.SetImageFile(self.this,"Pic_BackMap",ifile,1)
    	;
        DlgApi.ShowItem(self.this,"Pic_TimeText",false);
        DlgApi.ShowItem(self.this,"Prog_Time",false);    
    	DlgApi.ShowItem(self.this,"Btn_Next",false);
	   	DlgApi.EnableItem(self.this,"Btn_Hint",true);
    	DlgApi.EnableItem(self.this,"Btn_Wash",true);
    		
        DlgApi.SetItemText(self.this,"Edit_Level",tostring(self.level));     	
    	
    	self:GameStart();

        --为了让提示的图片覆盖在连连看图片之上
       	self.hintcounter = 0;  
        DlgApi.DeleteControl(self.this,"Pressed");
        DlgApi.CreateControl(self.this,"Pic_Pressed","Pressed");
        DlgApi.SetImageFile(self.this,"Pressed",TGllk_PressedPic,TGllk_PressedPicNum);
       	DlgApi.SelectImageFrame(self.this,"Pressed",0);
        DlgApi.ShowItem(self.this,"Pressed",false);
        
        DlgApi.DeleteControl(self.this,"Hint1");    	
        DlgApi.CreateControl(self.this,"Pic_Hint1","Hint1");
        DlgApi.SetImageFile(self.this,"Hint1",TGllk_HintPic,TGllk_HintPicNum);
        DlgApi.SelectImageFrame(self.this,"Hint1",0);
        DlgApi.ShowItem(self.this,"Hint1",false);
        
        DlgApi.DeleteControl(self.this,"Hint2"); 
        DlgApi.CreateControl(self.this,"Pic_Hint2","Hint2");
        DlgApi.SetImageFile(self.this,"Hint2",TGllk_HintPic,TGllk_HintPicNum);
        DlgApi.SelectImageFrame(self.this,"Hint2",0);
        DlgApi.ShowItem(self.this,"Hint2",false);
		-----
		DlgApi.DeleteControl(self.this,"Line1"); 
        DlgApi.CreateControl(self.this,"Pic_Line","Line1");
        DlgApi.SetImageFile(self.this,"Line1",TGllk_LinePic,1);
        DlgApi.ShowItem(self.this,"Line1",false);
		
		DlgApi.DeleteControl(self.this,"Line2"); 
        DlgApi.CreateControl(self.this,"Pic_Line","Line2");
        DlgApi.SetImageFile(self.this,"Line2",TGllk_LinePic,1);
        DlgApi.ShowItem(self.this,"Line2",false);

		DlgApi.DeleteControl(self.this,"Line3"); 
        DlgApi.CreateControl(self.this,"Pic_Line","Line3");
        DlgApi.SetImageFile(self.this,"Line3",TGllk_LinePic,1);
        DlgApi.ShowItem(self.this,"Line3",false);
    	
    	self.gamestate = TGllk_InGame;	
end

----
function Win_TGllkGame:GameStart()
    
    self.levelendflag = false;
    self.btnflag = false;
    self.falsenum = 0;
    self.rewardcounter = -1;
    self.succnum = 0;

   		--取难度

   	if self.level< 1.1 then	
       	self.difflevel=DlgApi.GetCheckedRadioButton(self.this,1);
    	if self.difflevel>3 then 
    	    self.difflevel=3 
    	end --test,只有第一/二/三难度

       	self.map_h = TGllk_MapSize[self.difflevel].height;
       	self.map_w = TGllk_MapSize[self.difflevel].width;
       	self.hintnum=TGllk_HintNum[self.difflevel];
    	self.washnum=TGllk_WashNum[self.difflevel];
   end

    self.timestore = TGllk_Time[self.difflevel];    
    DlgApi.SetProgress(self.this,"Prog_Time",(self.timestore/TGllk_Time[self.difflevel]));
    	
	DlgApi.SetItemText(self.this,"Edit_HintNum",tostring(self.hintnum));   	
	DlgApi.SetItemText(self.this,"Edit_WashNum",tostring(self.washnum));
	
	--生成图标的控件列表

	self.blocksum=(self.map_h-2)*(self.map_w-2);
	self.blockgroup = self.blocksum/4;
	
	for i=1,self.blocksum do
    	local num=math.ceil(i/4);
    	local bmodel = "Pic_MemberPic";           
        local btn = "Block"..i;
        DlgApi.CreateControl(self.this,bmodel,btn);
        DlgApi.SetImageFile(self.this,btn,TGllk_Pic[num],1);
        self.DeleteFlag[i]=true;
        self.BlockList[i]={};
        self.BlockList[i].btn = btn;
        self.BlockList[i].btype = num;
	end
	
	for i=1,self.map_h do
	    self.BlockInMatrix[i]={};
	    self.BlockMatrix[i]={};
	    for j=1,self.map_w do
	        self.BlockInMatrix[i][j]=0;
	        self.BlockMatrix[i][j]=0;
	    end
	end

	for i=2,self.map_h-1 do
	    for j=2,self.map_w-1 do
	        self.BlockInMatrix[i][j]=1;
	    end
	end
	
	self:GeneratorMatrix();
	
	self:foundhint();
	
	self.prepress.state=false;	--之前按下的键
	
	if self.washnum>0 then
	    DlgApi.ShowItem(self.this,"Btn_Wash",true);
	    DlgApi.EnableItem(self.this,"Btn_Wash",true);

	end
	if self.hintnum>0 then
	    DlgApi.ShowItem(self.this,"Btn_Hint",true);
	    DlgApi.EnableItem(self.this,"Btn_Hint",true);
	end
	DlgApi.ShowItem(self.this,"Btn_Abandon",true);

	DlgApi.ShowItem(self.this,"Btn_Start",false);
	DlgApi.ShowItem(self.this,"Btn_ToNext",false);
    
    DlgApi.ShowItem(self.this,"Rdo_11",false);
    DlgApi.ShowItem(self.this,"Rdo_12",false);
    DlgApi.ShowItem(self.this,"Rdo_13",false);   
    
    DlgApi.ShowItem(self.this,"Edit_MaxScore",false);
    DlgApi.ShowItem(self.this,"Lab_MaxScoreText",false);
    
    DlgApi.SetItemText(self.this,"Edit_Level",tostring(self.level)); 

    DlgApi.ShowItem(self.this,"Btn_Pause",true);
    DlgApi.ShowItem(self.this,"Btn_Continue",false);    
    DlgApi.ShowItem(self.this,"Edit_HintNum",true);
    DlgApi.ShowItem(self.this,"Edit_WashNum",true);
	DlgApi.ShowItem(self.this,"Edit_BackGround",true);--背景方块
    DlgApi.SetProgress(self.this,"Prog_Time",(self.timestore/TGllk_Time[self.difflevel]));
    DlgApi.ShowItem(self.this,"Prog_Time",true);
    DlgApi.ShowItem(self.this,"Pic_TimeText",true);

	DlgApi.ShowItem(self.this,"Edit_Score",true);
	DlgApi.SetItemText(self.this,"Edit_Score",tostring(math.floor(self.scorestore)));
	DlgApi.ShowItem(self.this,"Lab_TextScore",true);
	DlgApi.ShowItem(self.this,"Lab_LevelText",true);	
	DlgApi.ShowItem(self.this,"Edit_Level",true);	
	
	if self.soundflag == true then
	    DlgApi.ShowItem(self.this,"Btn_SoundOff",true);
		DlgApi.ShowItem(self.this,"Btn_SoundOn",false);
	else
	    DlgApi.ShowItem(self.this,"Btn_SoundOff",false);
		DlgApi.ShowItem(self.this,"Btn_SoundOn",true);
	end	    
    self.btnflag = false;   
     
end
------------------------------------------    

function Win_TGllkGame:OnCancel()
    self:Clear();
	DlgApi.ShowDialog(self.this,false);
	self.dialogflag = false;
	DlgApi.ShowDialog("Win_GameCasual",true);
end
-------------------------	

function Win_TGllkGame:OnPause()

    if self.gamestate == TGllk_InGame then
        --暂停
        self.gamestate = TGllk_Pause;
        DlgApi.ShowItem(self.this,"Btn_Pause",false);
        DlgApi.ShowItem(self.this,"Btn_Continue",true);
        
        --把牌面隐藏

        for bname,b in pairs(self.BlockList) do
            DlgApi.ShowItem(self.this,b.btn,false);
        end
    end
    
end
function Win_TGllkGame:OnContinue()
	if self.gamestate == TGllk_Pause then
        --继续游戏
        self.gamestate = TGllk_InGame;
        DlgApi.ShowItem(self.this,"Btn_Pause",true);
        DlgApi.ShowItem(self.this,"Btn_Continue",false);
        
        --把牌面显示

        for bname,b in pairs(self.BlockList) do
            DlgApi.ShowItem(self.this,b.btn,true);
        end   
    end
end
-------------------	

function Win_TGllkGame:ResizeWindows()
    self.mapscale=DlgApi.GetWindowScale();
end
-------------------=============================---------------
-------------------=============================-------------------
--洗牌按钮被按下

function Win_TGllkGame:OnWashArrange()
	
	if self.gamestate~= TGllk_InGame then
	    return true;
	end
	
    DlgApi.ShowItem(self.this,"Hint1",false);
    DlgApi.ShowItem(self.this,"Hint2",false);    	
    DlgApi.ShowItem(self.this,"Pressed",false);
    self.prepress.state = false;			
	
	self:wash();
	
	self:foundhint();
	
	return true;
end

function Win_TGllkGame:wash()
    if self.soundflag == true then
        GameAI.Play2DSound(TGllk_Sound.Wash);
    end

	for i=#self.BlockList,1,-1 do
	    if self.DeleteFlag[i]==false then
	        table.remove(self.BlockList,i);
	        table.remove(self.DeleteFlag,i);
	    end
	end
	
	self:GeneratorMatrix();
	
	if self.gamestate == TGllk_InGame then
    	self.washnum = self.washnum-1;
    	DlgApi.SetItemText(self.this,"Edit_WashNum",tostring(self.washnum));
	end
	
	if self.washnum <=0 then
	    DlgApi.EnableItem(self.this,"Btn_Wash",false);
	end

end
------------------	
function Win_TGllkGame:OnHint()
	if self.gamestate~= TGllk_InGame then
	    return true;
	end
	
	self.hintnum = self.hintnum -1;
	DlgApi.SetItemText(self.this,"Edit_HintNum",tostring(self.hintnum));
	if self.hintnum <= 0 then
	    DlgApi.EnableItem(self.this,"Btn_Hint",false);
	end

	for i=1,2 do
	    local posi=self.BlockList[ self.hintstore[i] ].i;
	    local posj=self.BlockList[ self.hintstore[i] ].j;
        local x=(posj-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x-(TGllk_HintSize-TGllk_BlockHSize);
        local y=(posi-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y-(TGllk_HintSize-TGllk_BlockHSize);
        self:SetItemPos("Hint"..i,x,y);
        DlgApi.ShowItem(self.this,"Hint"..i,true);
	end
	
	if self.soundflag == true then
	    GameAI.Play2DSound(TGllk_Sound.Hint);
	end
	
	return true;
end

--------------------
function Win_TGllkGame:OnAbandon()
    --放弃游戏，进行清除

    self:Clear();
    self:DataInit();
    self:ItemInit();
end

    -----------------------
function Win_TGllkGame:OnLButtonDown(itemName, wParam, x, y)
    
    if self.gamestate ~= TGllk_InGame then
        return true;
    end
    
    
    local posx,posy=GameAI.GetCursorPosition();
    local tempdlg = DlgApi.GetDialogProp(self.this);
    local mapx = (posx-tempdlg.x)/self.mapscale - TGllk_MapOrigin[self.difflevel].x;
    local mapy = (posy-tempdlg.y)/self.mapscale - TGllk_MapOrigin[self.difflevel].y;
    
    local mapi = math.ceil(mapy/TGllk_BlockSize.height);
    local mapj = math.ceil(mapx/TGllk_BlockSize.width);

    if mapi<1 or mapi>TGllk_MapSize[self.difflevel].height or mapj<1 or mapj>TGllk_MapSize[self.difflevel].width or self.BlockInMatrix[mapi][mapj]<0.5 then
        return true;
    end
    
    --游戏中

    if self.prepress.state == false then	--游戏中没有预先存放的键值

    
        self.prepress.block = self.BlockMatrix[mapi][mapj];

         DlgApi.ShowItem(self.this,"Hint1",false);
         DlgApi.ShowItem(self.this,"Hint2",false);            
  
        self.prepress.state=true;
        
        local x=(mapj-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x-(TGllk_HintSize-TGllk_BlockHSize);
        local y=(mapi-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y-(TGllk_HintSize-TGllk_BlockHSize);
        
        self:SetItemPos("Pressed",x,y);
        DlgApi.ShowItem(self.this,"Pressed",true); 
        --连接路径清空
        self.cnroute = {};
    
    elseif self.prepress.state == true then	--有预存的键值

    	
        self.prepress.state = false;
        DlgApi.ShowItem(self.this,"Hint1",false);
        DlgApi.ShowItem(self.this,"Hint2",false);    	
        DlgApi.ShowItem(self.this,"Pressed",false);
    	
			--图像的种类直接配对失败

		if self.BlockList[ self.prepress.block ].btype ~= self.BlockList[ self.BlockMatrix[mapi][mapj] ].btype then
		    --添加失败的提示

		    if self.soundflag == true then
		        GameAI.Play2DSound(TGllk_Sound.ConnectFail);
		    end
		    
    	    DlgApi.SetItemText(self.this,"Lab_TextOut",TGllk_TextBlock); 
    	    self.falsenum = self.falsenum+1;
    	    if self.falsenum > TGllk_ContinueFalse then
    	        DlgApi.SetItemText(self.this,"Lab_TextOut",TGllk_TextContiFalse);
    	        self.timestore = self.timestore - TGllk_TimeFalse;
    	        self.falsenum = 0;
    	        self.rewardcounter = -1;
    	    end
    	    self.textcounter = 1;
    	        	    		    
		    return true;
		end
			--点击到了同一幅图片

		if self.BlockList[ self.prepress.block ].i==mapi and self.BlockList[ self.prepress.block ].j==mapj then
		    --添加失败的提示

		    if self.soundflag == true then
		        GameAI.Play2DSound(TGllk_Sound.ConnectFail);
		    end
		    
    	    DlgApi.SetItemText(self.this,"Lab_TextOut",TGllk_TextBlock); 
    	    self.falsenum = self.falsenum+1;
    	    if self.falsenum > TGllk_ContinueFalse then
    	        DlgApi.SetItemText(self.this,"Lab_TextOut",TGllk_TextContiFalse);
    	        self.timestore = self.timestore - TGllk_TimeFalse;
    	        self.falsenum = 0;
    	        self.rewardcounter = -1;
    	    end    	    
    	    self.textcounter = 1;		    
		    return true;
		end
		
		
		--相同图像通过位置计算配对
   	    local bp=self.BlockList[self.prepress.block];
		local bn=self.BlockList[self.BlockMatrix[mapi][mapj]];		

    	local temp = self:Connect(bp,bn);
    	if temp ==true then
    	    
    	    local sucsoundflag = false;
			self.falsenum = 0;
			
			self.disapearnum = self.disapearnum+1 ;
			self.disapearlist["Disapear"..self.disapearnum] = { counter = 0, frame = 0, } ;
			DlgApi.CreateControl(self.this,"Pic_Disapear","Disapear"..self.disapearnum);
			DlgApi.SelectImageFrame(self.this,"Disapear"..self.disapearnum,0);
			local x = (bp.j-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x-(TGllk_HintSize-TGllk_BlockHSize);
			local y = (bp.i-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y-(TGllk_HintSize-TGllk_BlockHSize);
			self:SetItemPos("Disapear"..self.disapearnum,x,y);
			DlgApi.ShowItem(self.this,"Disapear"..self.disapearnum,true);
			
			self.disapearnum = self.disapearnum+1 ;
			self.disapearlist["Disapear"..self.disapearnum] = { counter = 0, frame = 0, };
			DlgApi.CreateControl(self.this,"Pic_Disapear","Disapear"..self.disapearnum);
			DlgApi.SelectImageFrame(self.this,"Disapear"..self.disapearnum,0);
			local x = (bn.j-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x-(TGllk_HintSize-TGllk_BlockHSize);
			local y = (bn.i-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y-(TGllk_HintSize-TGllk_BlockHSize);			
			self:SetItemPos("Disapear"..self.disapearnum,x,y);
			DlgApi.ShowItem(self.this,"Disapear"..self.disapearnum,true);

			--消除画面的相连接控件
			DlgApi.DeleteControl(self.this,bp.btn);
			DlgApi.DeleteControl(self.this,bn.btn);    	    
			--找到相关的路径


			--显示出路径

			for i=2,#self.cnroute do
			    local num = i-1;
			    local posx = nil;
			    local posy = nil;
			    
			    local px = self.cnroute[i][1];
			    local py = self.cnroute[i][2];
			    local bx = self.cnroute[i-1][1];
			    local by = self.cnroute[i-1][2];
			    
			    local dx = bx-px;
			    local dy = by-py;
			    
			    if math.abs(dx)<0.5 and math.abs(dy)>0.5 then		--竖向
			        DlgApi.SetItemSize(self.this,"Line"..num,3,math.abs(dy)*TGllk_BlockSize.height*self.mapscale);
			        if dy > 0.5 then
			            self:SetItemPos( "Line"..num,(px-0.5)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x,(py-0.5)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y );
			        else
			            self:SetItemPos( "Line"..num,(bx-0.5)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x,(by-0.5)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y );
			        end
			        DlgApi.ShowItem(self.this,"Line"..num,true);
			        self.linelist[num].counter = 1;
			    elseif math.abs(dx)>0.5 and math.abs(dy)<0.5 then		--横向
			        DlgApi.SetItemSize(self.this,"Line"..num,math.abs(dx)*TGllk_BlockSize.width*self.mapscale,3);
			        if dx > 0.5 then
			            self:SetItemPos( "Line"..num,(px-0.5)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x,(py-0.5)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y );
			        else
			            self:SetItemPos( "Line"..num,(bx-0.5)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x,(by-0.5)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y );
			        end
			        DlgApi.ShowItem(self.this,"Line"..num,true);			        
			        self.linelist[num].counter = 1;
			    end
			end

            self.timestore=self.timestore+TGllk_TimeReward[self.difflevel];            
            if self.timestore>=TGllk_Time[self.difflevel] then
                self.timestore = TGllk_Time[self.difflevel];
            	DlgApi.SetProgress(self.this,"Prog_Time",1);
            else
                DlgApi.SetProgress(self.this,"Prog_Time",(self.timestore/TGllk_Time[self.difflevel]));   	   
            end

			self.scorestore = self.scorestore + TGllk_ScoreReward[self.difflevel];
			DlgApi.SetItemText(self.this,"Edit_Score",tostring(math.floor(self.scorestore)));
            
			self.DeleteFlag[self.BlockMatrix[bp.i][bp.j]]=false;
			self.DeleteFlag[self.BlockMatrix[bn.i][bn.j]]=false;
			self.blocksum = self.blocksum-2;

            --奖励机制
            self.rewardcounter = self.rewardcounter + TGllk_TimeReward[self.difflevel];
            if self.rewardcounter > TGllk_RewardThresholdTime[self.difflevel] then
                self.rewardcounter = self.rewardcounter - TGllk_RewardSucTimeReduce[self.difflevel];
                if self.rewardcounter < 0 then
                    self.rewardcounter = -1;
                end
				sucsoundflag = Win_TGllkGame:Reward();
            end
            self.textcounter = 1;
            
            if sucsoundflag == false and self.soundflag == true then
                GameAI.Play2DSound(TGllk_Sound.ConnectSucc);
            end            			
						
			--需要判断是否已经消除干净
			if self.blocksum ==0 then
			    --已经消除干净，需要进入下一关

			    self:EndLevel();
			else
			    --需要根据每关的重排规则进行重新排列
			    local nod={};
			    nod[1]=bp;
			    nod[2]=bn;
			    self:realign(nod);
			    --找到下一个hint
			    self:foundhint();
			end
	

    	else
			if self.soundflag == true then
			    GameAI.Play2DSound(TGllk_Sound.ConnectFail);	
			end

    	  	DlgApi.SetItemText(self.this,"Lab_TextOut",TGllk_TextBlock); 
    	  	self.falsenum = self.falsenum+1;
    	    if self.falsenum > TGllk_ContinueFalse then
    	        DlgApi.SetItemText(self.this,"Lab_TextOut",TGllk_TextContiFalse);
    	        self.timestore = self.timestore - TGllk_TimeFalse;
    	        self.falsenum = 0;
    	        self.rewardcounter = -1;
    	    end
    	   self.textcounter = 1;
    	end
    end

    return true;
end
--
function Win_TGllkGame:Reward()

	local reti = 0;
	local rettype = 0;
    local rewardrate = TGllk_RewardTimeRate[self.difflevel];
    local rewardarea = TGllk_RewardArea[self.difflevel];
    local reward = nil;
    
    local flag = false;

    --编写奖励
    local rettime = self.timestore/TGllk_Time[self.difflevel];

	local ret = math.random();
	for retnum = 1,#rewardarea do
        if rettime>=rewardarea[retnum][2] and rettime<= rewardarea[retnum][1] then
            local i = retnum;
            if ret<rewardrate[i] then
                reti = retnum;
                break;
            end        
        end       
	end       
    
    if reti>0 then
        local ret = math.random();
        for retnum = 1,#TGllk_RewardRate[reti] do
            if ret <= TGllk_RewardRate[reti][retnum] then
                rettype = retnum;
                break;
            end
        end
    end
    
    if rettype > 0 then
        local ret = math.random(#TGllk_Reward[rettype]);
        reward = TGllk_Reward[rettype][ret];
    end      
                    
    if reward ~= nil then
        --播放奖励的声音

        if self.soundflag == true then
            GameAI.Play2DSound(TGllk_Sound.SuccReward);
        end
        flag = true;
                
        self.washnum = self.washnum+reward[1];
        self.hintnum = self.hintnum+reward[2];
   		if self.hintnum > 99 then
   		    self.hintnum = 99;
   		end
   		if self.hintnum > 0 then
   		   DlgApi.EnableItem(self.this,"Btn_Hint",true);
   		end
   		
   		if self.washnum > 99 then
   		    self.washnum = 99;
   		end
   		if self.washnum > 0 then
   		     DlgApi.EnableItem(self.this,"Btn_Wash",true);
   		end
   		
        self.scorestore = self.scorestore + reward[3];
        self.timestore = self.timestore + reward[4];
        DlgApi.SetItemText(self.this,"Lab_TextOut",reward[5]);
    end

	DlgApi.SetItemText(self.this,"Edit_HintNum",tostring(self.hintnum));   	
	DlgApi.SetItemText(self.this,"Edit_WashNum",tostring(self.washnum));
    DlgApi.SetItemText(self.this,"Edit_Score",tostring(math.floor(self.scorestore)));
    
    if self.timestore>TGllk_Time[self.difflevel] then
        DlgApi.SetProgress(self.this,"Prog_Time",1);
        self.timestore = TGllk_Time[self.difflevel];
    else
        DlgApi.SetProgress(self.this,"Prog_Time",(self.timestore/TGllk_Time[self.difflevel]));
    end
	
	if flag == nil then
	    flag = true;
	end

	return flag;
end

function Win_TGllkGame:EndLevel()
    if self.level == 11 then	--已经通关
    	
    	if TGllk_TaskNum and TGllk_TaskNum[self.difflevel] and type(TGllk_TaskNum[self.difflevel]) =="number" then
    	    GameAI.TaskFin(TGllk_TaskNum[self.difflevel]);
    	end	    	
    	
    	self.scorestore = self.scorestore + self.hintnum*TGllk_ScoreHint[self.difflevel] + self.washnum*TGllk_ScoreWash[self.difflevel] + self.timestore*TGllk_ScoreTime[self.difflevel]/1000;
    	--存储分数和最大关数

    	local ret = {};
    	if self.scorestore>self.maxscore then
    	    ret[1] = self.scorestore;
    	else
    	    ret[1] = self.maxscore;
    	end
    	local retdiff = self.difflevel +1;
    	if retdiff > self.maxdiff then
    	    ret[2] = retdiff;
    	else
    	    ret[2] = self.maxdiff;
    	end
    	
    	GameAI.SaveTDFile(ret,"tgllk");
    	
    	
    	self:OnAbandon();
    	Win_TGllkWin:OnShowDialog(); 
    	if self.soundflag == true then
    	    GameAI.Play2DSound(TGllk_Sound.MissionSuc);  	
    	end
    else
		if self.level == 9 and TGllk_TaskNum and TGllk_TaskNum[self.difflevel] and type(TGllk_TaskNum[self.difflevel])=="number" then
    	    GameAI.TaskGiveup(TGllk_TaskNum[self.difflevel]);
		end
		
		if self.level == 10 and TGllk_TaskNum and TGllk_TaskNum[self.difflevel] and type(TGllk_TaskNum[self.difflevel])=="number" then
    	    GameAI.TaskReq(TGllk_TaskNum[self.difflevel]);
		end
		
        self.gamestate = TGllk_TextWait;
        self.levelendflag = true;
   		self.level = self.level+1;
   		self.hintnum = self.hintnum + 2;
   		self.washnum = self.washnum + 1;
   		if self.hintnum > 99 then
   		    self.hintnum = 99;
   		end
   		if self.washnum>99 then
   		    self.washnum = 99;
   		end
                
    	DlgApi.ShowItem(self.this,"Btn_Abandon",true);
    	DlgApi.ShowItem(self.this,"Btn_Pause",false);
        DlgApi.ShowItem(self.this,"Btn_Continue",false);
		DlgApi.ShowItem(self.this,"Btn_SoundOff",false);
		DlgApi.ShowItem(self.this,"Btn_SoundOn",false);
	   	DlgApi.EnableItem(self.this,"Btn_Hint",false);
    	DlgApi.EnableItem(self.this,"Btn_Wash",false);
    	DlgApi.ShowItem(self.this,"Btn_ToNext",true);
    	DlgApi.ShowItem(self.this,"Btn_Next",true);
    	
    	if self.soundflag == true then
    	    GameAI.Play2DSound(TGllk_Sound.MissionSuc); 
    	end
   	
    end
        
end
--
function Win_TGllkGame:realign(nod)
	if self.level ==1 then
	    self:levelone(nod);
	    return true;
	elseif self.level ==2 then
		self:leveltwo(nod);
		return true;
	elseif self.level ==3 then
	    self:levelthree(nod);
	    return true;
	elseif self.level ==4 then
	    self:levelfour(nod);
	    return true;
	elseif self.level ==5 then
	    self:levelfive(nod);
	    return true;
	elseif self.level ==6 then
	    self:levelsix(nod);
	    return true;
	elseif self.level ==7 then
	    self:levelseven(nod);
	    return true;
	elseif self.level ==8 then
	    self:leveleight(nod);
	    return true;
	elseif self.level ==9 then
	    self:levelnine(nod);
	    return true;
	elseif self.level ==10 then
	    self:levelten(nod);
	    return true;
	elseif self.level ==11 then
	    self:leveleleven(nod);
	    return true;	    
	end
end
--level_1 ---保持原地
function Win_TGllkGame:levelone(nod)
	for id = 1,2 do
	    p=nod[id];
	    self.BlockInMatrix[p.i][p.j] = 0;
	end
end

--level_2----向下边界落去
function Win_TGllkGame:leveltwo(nod)
	for id=1,2 do
	    local p=nod[id];
	    self.BlockInMatrix[p.i][p.j] = 0;
	    for i = p.i-1,1,-1 do
	        if self.BlockInMatrix[i][p.j] >0.5 then
	            
	            self.BlockMatrix[i+1][p.j] = self.BlockMatrix[i][p.j];
	            self.BlockList[self.BlockMatrix[i][p.j]].i=i+1;
	            self.BlockList[self.BlockMatrix[i][p.j]].j=p.j;
	            self.BlockInMatrix[i+1][p.j] = self.BlockList[self.BlockMatrix[i][p.j]].btype;
	            
               local ret=DlgApi.GetItemRect(self.this,self.BlockList[ self.BlockMatrix[i+1][p.j] ].btn);
               local posx=(p.j-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x;
               local posy=(i+1-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y;
               self:SetItemPos(self.BlockList[ self.BlockMatrix[i+1][p.j] ].btn,posx,posy);
               DlgApi.ShowItem(self.this,self.BlockList[ self.BlockMatrix[i+1][p.j] ].btn,true);
	            
	            self.BlockInMatrix[i][p.j]=0;
	            
	        elseif self.BlockInMatrix[i][p.j] <0.5 then
	            self.BlockInMatrix[i+1][p.j] = 0;
	            break;
	        end
	        
	    end
	end
end
--level_3----向左边界落去
function Win_TGllkGame:levelthree(nod)
	for id=1,2 do
	    local p=nod[id];
	    self.BlockInMatrix[p.i][p.j] = 0;
	    for j = p.j+1,self.map_w do
	        if self.BlockInMatrix[p.i][j] >0.5 then
	            
	            self.BlockMatrix[p.i][j-1] = self.BlockMatrix[p.i][j];
	            self.BlockList[self.BlockMatrix[p.i][j]].i=p.i;
	            self.BlockList[self.BlockMatrix[p.i][j]].j=j-1;
	            self.BlockInMatrix[p.i][j-1] = self.BlockList[self.BlockMatrix[p.i][j]].btype;
	            
               local ret=DlgApi.GetItemRect(self.this,self.BlockList[ self.BlockMatrix[p.i][j-1] ].btn);
               local posx=(j-1-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x;
               local posy=(p.i-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y;
               self:SetItemPos(self.BlockList[ self.BlockMatrix[p.i][j-1] ].btn,posx,posy);
               DlgApi.ShowItem(self.this,self.BlockList[ self.BlockMatrix[p.i][j-1] ].btn,true);
               	            
	            self.BlockInMatrix[p.i][j]=0;
	            
	        elseif self.BlockInMatrix[p.i][j] <0.5 then
	            self.BlockInMatrix[p.i][j-1] = 0;
	            break;
	        end
	        
	    end
	end
end
--level_4-------向上下两边从中间分开
function Win_TGllkGame:levelfour(nod)
	for id=1,2 do
	    local p=nod[id];
	    local mi = math.floor(self.map_h/2);
	    if p.i<=mi then
	        va1 = p.i+1;
	        va2 = mi;
	        dva = 1;
	        self.BlockInMatrix[p.i][p.j] = 0;
	    else
	        va1 = p.i-1;
	        va2 = self.map_h-math.ceil(self.map_h/2)+1;
	        dva = -1;
	        self.BlockInMatrix[p.i][p.j] = 0;
	    end
	    for  i=va1,va2,dva do
	        if self.BlockInMatrix[i][p.j] >0.5 then
	            
	            self.BlockMatrix[i-dva][p.j] = self.BlockMatrix[i][p.j];
	            self.BlockList[self.BlockMatrix[i][p.j]].i=i-dva;
	            self.BlockList[self.BlockMatrix[i][p.j]].j=p.j;
	            self.BlockInMatrix[i-dva][p.j] = self.BlockList[self.BlockMatrix[i][p.j]].btype;
	            
               local ret=DlgApi.GetItemRect(self.this,self.BlockList[ self.BlockMatrix[i-dva][p.j] ].btn);
               local posx=(p.j-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x;
               local posy=(i-dva-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y;
               self:SetItemPos(self.BlockList[ self.BlockMatrix[i-dva][p.j] ].btn,posx,posy);
               DlgApi.ShowItem(self.this,self.BlockList[ self.BlockMatrix[i-dva][p.j] ].btn,true);
               	            
	            self.BlockInMatrix[i][p.j]=0;
	            
	        elseif self.BlockInMatrix[i][p.j] <0.5 then
	            self.BlockInMatrix[i-dva][p.j] = 0;	            
	            break;
	        end
	        
	    end
	end
end
---level_5 ---------向左右两边分开
function Win_TGllkGame:levelfive(nod)
	for id=1,2 do
	    local p=nod[id];
	    local mj = math.floor(self.map_w/2);
	    if p.j<=mj then
	        va1 = p.j+1;
	        va2 = mj;
	        dva = 1;
	        self.BlockInMatrix[p.i][p.j] = 0;
	    else
	        va1 = p.j-1;
	        va2 = self.map_w-math.ceil(self.map_w/2)+1;
	        dva = -1;
	        self.BlockInMatrix[p.i][p.j] = 0;
	    end
	    for  j=va1,va2,dva do
	        if self.BlockInMatrix[p.i][j] >0.5 then
	            
	            self.BlockMatrix[p.i][j-dva] = self.BlockMatrix[p.i][j];
	            self.BlockList[self.BlockMatrix[p.i][j]].i=p.i;
	            self.BlockList[self.BlockMatrix[p.i][j]].j=j-dva;
	            self.BlockInMatrix[p.i][j-dva] = self.BlockList[self.BlockMatrix[p.i][j]].btype;
	            
               local ret=DlgApi.GetItemRect(self.this,self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn);
               local posx=(j-dva-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x;
               local posy=(p.i-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y;
               self:SetItemPos(self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn,posx,posy);
               DlgApi.ShowItem(self.this,self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn,true);
	            
	            self.BlockInMatrix[p.i][j]=0;
	            
	        elseif self.BlockInMatrix[p.i][j] <0.5 then
	            self.BlockInMatrix[p.i][j-dva] = 0;	            
	            break;
	        end
	        
	    end
	end
end
---level6 ---------------向中间行收缩

function Win_TGllkGame:levelsix(nod)
	for id=1,2 do
	    local p=nod[id];
	    local mi = math.floor(self.map_h/2);
	    if p.i<=mi then
	        va1 = p.i-1;
	        va2 = 1;
	        dva = -1;
	        self.BlockInMatrix[p.i][p.j] = 0;
	    else
	        va1 = p.i+1;
	        va2 = self.map_h;
	        dva = 1;
	        self.BlockInMatrix[p.i][p.j] = 0;
	    end
	    for  i=va1,va2,dva do
	        if self.BlockInMatrix[i][p.j] >0.5 then
	            
	            self.BlockMatrix[i-dva][p.j] = self.BlockMatrix[i][p.j];
	            self.BlockList[self.BlockMatrix[i][p.j]].i=i-dva;
	            self.BlockList[self.BlockMatrix[i][p.j]].j=p.j;
	            self.BlockInMatrix[i-dva][p.j] = self.BlockList[self.BlockMatrix[i][p.j]].btype;
	            
               local ret=DlgApi.GetItemRect(self.this,self.BlockList[ self.BlockMatrix[i-dva][p.j] ].btn);
               local posx=(p.j-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x;
               local posy=(i-dva-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y;
               self:SetItemPos(self.BlockList[ self.BlockMatrix[i-dva][p.j] ].btn,posx,posy);
               DlgApi.ShowItem(self.this,self.BlockList[ self.BlockMatrix[i-dva][p.j] ].btn,true);	           
	            
	            self.BlockInMatrix[i][p.j]=0;
	            
	        elseif self.BlockInMatrix[i][p.j] <0.5 then
	            self.BlockInMatrix[i-dva][p.j] = 0;	            
	            break;
	        end
	        
	    end
	end
end

--level7 ----------------列向中间聚拢
function Win_TGllkGame:levelseven(nod)
	for id=1,2 do
	    local p=nod[id];
	    local mj = math.floor(self.map_w/2);
	    if p.j<=mj then
	        va1 = p.j-1;
	        va2 = 1;
	        dva = -1;
	        self.BlockInMatrix[p.i][p.j] = 0;
	    else
	        va1 = p.j+1;
	        va2 = self.map_w;
	        dva = 1;
	        self.BlockInMatrix[p.i][p.j] = 0;
	    end
	    for  j=va1,va2,dva do
	        if self.BlockInMatrix[p.i][j] >0.5 then
	            
	            self.BlockMatrix[p.i][j-dva] = self.BlockMatrix[p.i][j];
	            self.BlockList[self.BlockMatrix[p.i][j]].i=p.i;
	            self.BlockList[self.BlockMatrix[p.i][j]].j=j-dva;
	            self.BlockInMatrix[p.i][j-dva] = self.BlockList[self.BlockMatrix[p.i][j]].btype;
	            
               local ret=DlgApi.GetItemRect(self.this,self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn);
               local posx=(j-dva-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x;
               local posy=(p.i-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y;
               self:SetItemPos(self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn,posx,posy);
               DlgApi.ShowItem(self.this,self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn,true);	            
	            
	            self.BlockInMatrix[p.i][j]=0;
	            
	        elseif self.BlockInMatrix[p.i][j] <0.5 then
	            self.BlockInMatrix[p.i][j-dva] = 0;	            
	            break;
	        end
	        
	    end
	end
end
---

---level8 ---------------一半一半，行向左右两边移动
function Win_TGllkGame:leveleight(nod)
	for id=1,2 do
	    local p=nod[id];
	    local mi = math.floor(self.map_h/2);
	    if p.i<=mi then
	        va1 = p.j+1;
	        va2 = self.map_w;
	        dva = 1;
	        self.BlockInMatrix[p.i][p.j] = 0;
	    else
	        va1 = p.j-1;
	        va2 = 1;
	        dva = -1;
	        self.BlockInMatrix[p.i][p.j] = 0;
	    end
	    for  j=va1,va2,dva do
	        if self.BlockInMatrix[p.i][j] >0.5 then
	            
	            self.BlockMatrix[p.i][j-dva] = self.BlockMatrix[p.i][j];
	            self.BlockList[self.BlockMatrix[p.i][j]].i=p.i;
	            self.BlockList[self.BlockMatrix[p.i][j]].j=j-dva;
	            self.BlockInMatrix[p.i][j-dva] = self.BlockList[self.BlockMatrix[p.i][j]].btype;
	            
               local ret=DlgApi.GetItemRect(self.this,self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn);
               local posx=(j-dva-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x;
               local posy=(p.i-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y;
               self:SetItemPos(self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn,posx,posy);
               DlgApi.ShowItem(self.this,self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn,true);	            
	                        
	            self.BlockInMatrix[p.i][j]=0;
	            
	        elseif self.BlockInMatrix[p.i][j] <0.5 then
	            self.BlockInMatrix[p.i][j-dva] = 0;	            
	            break;
	        end
	        
	    end
	end
end
--level9---------------------一半一半，列向上下两边移动
function Win_TGllkGame:levelnine(nod)
	for id=1,2 do
	    local p=nod[id];
	    local mj = math.floor(self.map_w/2);
	    if p.j<=mj then
	        va1 = p.i-1;
	        va2 = 1;
	        dva = -1;
	        self.BlockInMatrix[p.i][p.j] = 0;
	    else
	        va1 = p.i+1;
	        va2 = self.map_h;
	        dva = 1;
	        self.BlockInMatrix[p.i][p.j] = 0;
	    end
	    for  i=va1,va2,dva do
	        if self.BlockInMatrix[i][p.j] >0.5 then
	            
	            self.BlockMatrix[i-dva][p.j] = self.BlockMatrix[i][p.j];
	            self.BlockList[self.BlockMatrix[i][p.j]].i=i-dva;
	            self.BlockList[self.BlockMatrix[i][p.j]].j=p.j;
	            self.BlockInMatrix[i-dva][p.j] = self.BlockList[self.BlockMatrix[i][p.j]].btype;
	            
               local ret=DlgApi.GetItemRect(self.this,self.BlockList[ self.BlockMatrix[i-dva][p.j] ].btn);
               local posx=(p.j-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x;
               local posy=(i-dva-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y;
               self:SetItemPos(self.BlockList[ self.BlockMatrix[i-dva][p.j] ].btn,posx,posy);
               DlgApi.ShowItem(self.this,self.BlockList[ self.BlockMatrix[i-dva][p.j] ].btn,true);	            
	            
	            self.BlockInMatrix[i][p.j]=0;
	            
	        elseif self.BlockInMatrix[i][p.j] <0.5 then
	            self.BlockInMatrix[i-dva][p.j] = 0;	            
	            break;
	        end
	        
	    end
	end
end
--level 10	--------------向外扩散，左右优先

function Win_TGllkGame:levelten(nod)
    local blank = {};
	for id=1,2 do
	    local p=nod[id];
	    blank[id]={};
        blank[id].j = p.j;
        blank[id].i = p.i;
		self.BlockInMatrix[p.i][p.j] = 0;

	    local mj = math.floor(self.map_w/2);
	    if p.j<=mj then
	        va1 = p.j+1;
	        va2 = mj;
	        dva = 1;
	    else
	        va1 = p.j-1;
	        va2 = self.map_w-math.ceil(self.map_w/2)+1;
	        dva = -1;

	    end
  
	    for  j=va1,va2,dva do
	        if self.BlockInMatrix[p.i][j] >0.5 then
	            
	            self.BlockMatrix[p.i][j-dva] = self.BlockMatrix[p.i][j];
	            self.BlockList[self.BlockMatrix[p.i][j]].i=p.i;
	            self.BlockList[self.BlockMatrix[p.i][j]].j=j-dva;
	            self.BlockInMatrix[p.i][j-dva] = self.BlockList[self.BlockMatrix[p.i][j]].btype;
	            
               local ret=DlgApi.GetItemRect(self.this,self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn);
               local posx=(j-dva-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x;
               local posy=(p.i-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y;
               self:SetItemPos(self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn,posx,posy);
               DlgApi.ShowItem(self.this,self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn,true);
	            
	            self.BlockInMatrix[p.i][j]=0;
	            blank[id].i = p.i;
	            blank[id].j = j;
	            
	        elseif self.BlockInMatrix[p.i][j] <0.5 then
	            blank[id].i = p.i;
	            blank[id].j = j-dva;
	            
 	            self.BlockInMatrix[p.i][j-dva] = 0;	            
	            break;
	        end
	    end
	end

	--根据空缺，向上下两边分开
	self:levelfour(blank);
end

--level 11 ---------------向内聚拢，左右优先

function Win_TGllkGame:leveleleven(nod)
    local blank={};
	for id=1,2 do
	    local p=nod[id];
		blank[id]={ i=p.i,j=p.j };
        self.BlockInMatrix[p.i][p.j] = 0;
	    local mj = math.floor(self.map_w/2);
	    if p.j<=mj then
	        va1 = p.j-1;
	        va2 = 1;
	        dva = -1;
	    else
	        va1 = p.j+1;
	        va2 = self.map_w;
	        dva = 1;
	    end

	    for  j=va1,va2,dva do
	        if self.BlockInMatrix[p.i][j] >0.5 then
	            
	            self.BlockMatrix[p.i][j-dva] = self.BlockMatrix[p.i][j];
	            self.BlockList[self.BlockMatrix[p.i][j]].i=p.i;
	            self.BlockList[self.BlockMatrix[p.i][j]].j=j-dva;
	            self.BlockInMatrix[p.i][j-dva] = self.BlockList[self.BlockMatrix[p.i][j]].btype;
	            
               local ret=DlgApi.GetItemRect(self.this,self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn);
               local posx=(j-dva-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x;
               local posy=(p.i-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y;
               self:SetItemPos(self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn,posx,posy);
               DlgApi.ShowItem(self.this,self.BlockList[ self.BlockMatrix[p.i][j-dva] ].btn,true);	            
	            
	            self.BlockInMatrix[p.i][j]=0;
	            blank[id].i = p.i;
	            blank[id].j = j;
	            
	        elseif self.BlockInMatrix[p.i][j] <0.5 then
	            blank[id].i = p.i;
	            blank[id].j = j-dva;
	            self.BlockInMatrix[p.i][j-dva] = 0;	            
	            break;
	        end
	        
	    end
	end
	
	self:levelsix(blank);
end



---------------------------------------
function Win_TGllkGame:foundhint()
    
    self.hintstore={};
    local sucflag = {};
    sucflag[1],sucflag[2],sucflag[3],sucflag[4],sucflag[5],sucflag[6],sucflag[7],sucflag[8] = GameAI.PictureMatching(self.map_h,self.map_w,self.BlockInMatrix);--self:gethint();

    while sucflag[1] ==nil do
        if self.gamestate == TGllk_InGame and self.washnum <=0 then
    		if TGllk_TaskNum and TGllk_TaskNum[self.difflevel] and type(TGllk_TaskNum[self.difflevel])=="number" then
        	    GameAI.TaskGiveup(TGllk_TaskNum[self.difflevel]);
    		end             
            self:OnAbandon();
            --给出失败界面 GameOver
            Win_TGllkFail:OnShowDialog();
            if self.soundflag == true then
                GameAI.Play2DSound(TGllk_Sound.MissionFai); 
            end
			return false;
        else
            self:wash();
        	sucflag[1],sucflag[2],sucflag[3],sucflag[4],sucflag[5],sucflag[6],sucflag[7],sucflag[8] = GameAI.PictureMatching(self.map_h,self.map_w,self.BlockInMatrix);--self:gethint();
        end
    end

	self.hintstore[1]=self.BlockMatrix[sucflag[1]+1][sucflag[2]+1];
	self.hintstore[2]=self.BlockMatrix[sucflag[#sucflag-1]+1][sucflag[#sucflag]+1];
    
    return true;    
end

--------------------------------------------------------------------
function Win_TGllkGame:OnRButtonDown()
    if self.gamestate ~= TGllk_InGame then
        return true;
    end

    DlgApi.ShowItem(self.this,"Hint1",false);
    DlgApi.ShowItem(self.this,"Hint2",false);    	    
    if self.prepress.state == true then
       self.prepress.state = false;
		--处理被按下去的状态，使恢复

       DlgApi.ShowItem(self.this,"Pressed",false); 
    end
    return true;
    
end
------------------------
function Win_TGllkGame:Tick(dwDeltaTime)
    
    if self.dialogflag == false then
        self:OnShowDialog();
        self.dialogflag = true;
    end

	if self.levelendflag == true then

	   if self.timestore >0  then
	        if self.timestore>1000 then
	            self.scorestore = self.scorestore + TGllk_ScoreTime[self.difflevel];
	            self.timestore = self.timestore - 1000;
        
	        else
	            self.scorestore = self.scorestore + TGllk_ScoreTime[self.difflevel]*self.timestore/1000;
	            self.timestore = self.timestore - self.timestore;
	        end
	        DlgApi.SetItemText(self.this,"Edit_Score",tostring(math.floor(self.scorestore)));
	        DlgApi.SetProgress(self.this,"Prog_Time",(self.timestore/TGllk_Time[self.difflevel]));	        
	    else
	        self.levelendflag = false;
	    end
	end
	    
    if self.textcounter > 0 then
        self.textcounter = self.textcounter + dwDeltaTime;
        if self.textcounter >  2000 then
    		self.textcounter = -1;
            DlgApi.SetItemText(self.this,"Lab_TextOut","");
        end
    end
        
    self:LineAction(dwDeltaTime);
        
    for name,d in pairs(self.disapearlist) do
       d.counter = d.counter + dwDeltaTime;
       if d.counter> 30 then
           d.counter = 0;
           d.frame = d.frame + 1;
           if d.frame < 12 then
               DlgApi.SelectImageFrame(self.this,name,d.frame);
           else
               self.disapearlist[name] = nil;
               DlgApi.DeleteControl(self.this,name);
           end 
       end
    end

	-------------------------------------------------------------
    if self.gamestate ~= TGllk_InGame then
        return true;
    end

    if self.hintcounter then
       self.hintcounter = self.hintcounter + dwDeltaTime;    
       if self.hintcounter > 150 then
          self.hintcounter = 0;
          
          self.hintframe = self.hintframe + 1;
          if self.hintframe > TGllk_DisapearPicNum-1 then
              self.hintframe = 0;
          end
          
          DlgApi.SelectImageFrame(self.this,"Pressed",self.hintframe);
          DlgApi.SelectImageFrame(self.this,"Hint1",self.hintframe);
          DlgApi.SelectImageFrame(self.this,"Hint2",self.hintframe);
          
       end 
    end
   

	if self.rewardcounter<0 then
	    self.rewardcounter = -1;
	else
	    self.rewardcounter = self.rewardcounter - dwDeltaTime;
	end
    
    self.timestore=self.timestore-dwDeltaTime;
    if self.timestore<=0 then
		if TGllk_TaskNum and TGllk_TaskNum[self.difflevel] and type(TGllk_TaskNum[self.difflevel])=="number" then
    	    GameAI.TaskGiveup(TGllk_TaskNum[self.difflevel]);
		end 
    	--显示分数,GameOver
	   	self:OnAbandon();
        --给出失败界面 GameOver
        Win_TGllkFail:OnShowDialog();
 
        if self.soundflag == true then
            GameAI.Play2DSound(TGllk_Sound.MissionFai);
        end
    elseif self.timestore>TGllk_Time[self.difflevel] then
        DlgApi.SetProgress(self.this,"Prog_Time",1);
    else
        DlgApi.SetProgress(self.this,"Prog_Time",(self.timestore/TGllk_Time[self.difflevel]));
    end
    
        
end

function Win_TGllkGame:LineAction(dwDeltaTime)
	for i,line in ipairs(self.linelist) do
		if line.counter>line.period then
		    line.counter = -1;
		    DlgApi.ShowItem(self.this,"Line"..i,false);
		end
		if line.counter>0 then
		    line.counter = line.counter+dwDeltaTime;
		end		    
	end
end
	------------------------



--重排方块
function Win_TGllkGame:GeneratorMatrix()

   local gtnum =self.blocksum;
   local gtarray = {};
   for i=1,gtnum do
      gtarray[i]=i; 
   end
   
   for i=1,self.map_h do
       for j=1,self.map_w do
           
           if self.BlockInMatrix[i][j]>0.5 then
               local apos=math.random(gtnum);
               self.BlockMatrix[i][j]=gtarray[apos];
               
               self.BlockList[ self.BlockMatrix[i][j] ].i = i;
               self.BlockList[ self.BlockMatrix[i][j] ].j = j;
               self.BlockInMatrix[i][j]=self.BlockList[ self.BlockMatrix[i][j] ].btype;

               local ret=DlgApi.GetItemRect(self.this,self.BlockList[ self.BlockMatrix[i][j] ].btn);
               local posx=(j-1)*TGllk_BlockSize.width+TGllk_MapOrigin[self.difflevel].x;
               local posy=(i-1)*TGllk_BlockSize.height+TGllk_MapOrigin[self.difflevel].y;
               self:SetItemPos(self.BlockList[ self.BlockMatrix[i][j] ].btn,posx,posy);
               DlgApi.ShowItem(self.this,self.BlockList[ self.BlockMatrix[i][j] ].btn,true);
               
               table.remove(gtarray,apos);         
               gtnum=gtnum-1;
           end

        end
   end
    
end
----------------    
function Win_TGllkGame:Clear()

	if TGllk_TaskNum then 
		for i = 1,#TGllk_TaskNum do
		    if TGllk_TaskNum[i] and type(TGllk_TaskNum[i])=="number" then
		        GameAI.TaskGiveup(TGllk_TaskNum[i]);
		    end
		end
	end

    DlgApi.DeleteControl(self.this,"Pressed");
    DlgApi.DeleteControl(self.this,"Hint1"); 
    DlgApi.DeleteControl(self.this,"Hint2");
	DlgApi.DeleteControl(self.this,"Line1"); 
	DlgApi.DeleteControl(self.this,"Line2"); 
	DlgApi.DeleteControl(self.this,"Line3"); 
	self.hintcounter = nil;  
        
  	for i=1,self.map_h do
	    
	    if self.BlockInMatrix[i]~=nil then
    	    for j=1,self.map_w do
    	       
    	       if self.BlockInMatrix[i][j]>0.5 then
    				DlgApi.DeleteControl(self.this,self.BlockList[ self.BlockMatrix[i][j] ].btn);
    				self.BlockInMatrix[i][j]=0;
    	       end 
    	    end
	    end
	end
	
    for name,d in pairs(self.disapearlist) do
        self.disapearlist[name] = nil;
        DlgApi.DeleteControl(self.this,name);
    end	
	
end


---------------------------
function Win_TGllkGame:Connect(bp,bn)
	
	local retvalue = false;

	if retvalue == false then
	   
	   self.cnroute = {};
	   retvalue = self:ConnectTwo(bp,bn); 
	end
	    
	return retvalue;
end
--------------------
function Win_TGllkGame:ConnectDire(bpi,bpj,bni,bnj)
	local found = true;
	local vstart = 0;
	local vend = 0;
		
	if bpi==bni then
	    if bpj>bnj then
	        table.insert(self.cnroute,{bpi,bnj});
	        table.insert(self.cnroute,{bpi,bpj});
	        vstart=bnj+1;
	        vend=bpj-1;
	    else
	        table.insert(self.cnroute,{bpi,bpj});
	        table.insert(self.cnroute,{bpi,bnj});
	        vstart=bpj+1;
	        vend=bnj-1;
	    end
	    for j=vstart,vend do
	        table.insert(self.cnroute,#self.cnroute,{ bpi,j });
	        
	        if self.BlockInMatrix[bpi][j] >0.5 then   --有阻碍

	        	found=false;
	        	self.cnroute = {};
	        	return found;
	        end
		end
		
	elseif bpj==bnj then
	    if bpi>bni then
	        table.insert(self.cnroute,{bni,bpj});
	        table.insert(self.cnroute,{bpi,bpj});	        
	        vstart=bni+1;
	        vend=bpi-1;
	    else
	        table.insert(self.cnroute,{bpi,bpj});
	        table.insert(self.cnroute,{bni,bpj});	        
	        vstart=bpi+1;
	        vend=bni-1;
	    end
	    for i=vstart,vend do
	       table.insert(self.cnroute,#self.cnroute,{i,bpj});
	       
	       if self.BlockInMatrix[i][bpj] >0.5 then
	           found=false;
	           self.cnroute = {};
	           return found;
	       end 
	        
	    end
	    
	end
	
	return found;
end

----
function Win_TGllkGame:ConnectOne(bp,bn)
    local found=false;
    
    local tempi=bp.i;
    local tempj=bn.j;
    if self.BlockInMatrix[tempi][tempj] <0.5 then
       found = self:ConnectDire(bp.i,bp.j,tempi,tempj);
       if found == true then
	       found =self:ConnectDire(bn.i,bn.j,tempi,tempj);
       end
       if found == true then
           return found;
       end
    end
    
    tempi=bn.i;
    tempj=bp.j;
    if self.BlockInMatrix[tempi][tempj] <0.5 then
       found = self:ConnectDire(bp.i,bp.j,tempi,tempj);
       if found == true then
           found =self:ConnectDire(bn.i,bn.j,tempi,tempj);
       end
       if found == true then
           return found;
       end
    end
    
    return found;
end

function Win_TGllkGame:ConnectTwo(bp,bn)

    local dire={ {x=-1,y=0},{x=1,y=0},{x=0,y=-1},{x=0,y=1} };
    local bplist={};
    local bnlist={};
    --统计两块牌的可以延伸的中间点
	self:nodlist(bp,dire,bplist);
	self:nodlist(bn,dire,bnlist);

	local found=false;
	for i=1,#bplist do
        for j=1,#bnlist do
	        
	        if bplist[i].x==bnlist[j].x or bplist[i].y==bnlist[j].y and (not(bplist[i].x==bnlist[j].x and bplist[i].y==bnlist[j].y))  then
	           
	           found = self:ConnectDire(bplist[i].y,bplist[i].x,bnlist[j].y,bnlist[j].x);
	           if found == true then
					self.cnroute = {{bp.j,bp.i},{bplist[i].x,bplist[i].y},{bnlist[j].x,bnlist[j].y},{bn.j,bn.i},};
                  return found;
	           end 
	        end
	        
	    end
	end
	return found;
end

function Win_TGllkGame:nodlist(b,dire,blist)

    table.insert(blist,{x=b.j,y=b.i});
    for dir=1,#dire do
       local nodi = b.i;
       local nodj = b.j;    
       while nodi+dire[dir].y>=1 and nodi+dire[dir].y<=TGllk_MapSize[self.difflevel].height and nodj+dire[dir].x>=1 and nodj+dire[dir].x<=TGllk_MapSize[self.difflevel].width 
       		and self.BlockInMatrix[nodi+dire[dir].y][nodj+dire[dir].x]<0.5  do

           table.insert(blist,{x=nodj+dire[dir].x,y=nodi+dire[dir].y});
           nodi=nodi+dire[dir].y;
           nodj=nodj+dire[dir].x;
       end        
    end
end

---------------    




function Win_TGllkGame:SetItemPos(Item,posx,posy)
   DlgApi.SetItemPos(self.this,Item, posx*self.mapscale, posy*self.mapscale); 
end
    

    
---------------------------------------------
---------------------------------------------

