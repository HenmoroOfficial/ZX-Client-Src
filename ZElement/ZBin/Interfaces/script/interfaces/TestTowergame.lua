local DlgTemplate = DlgTemplate
local DlgApi = DlgApi
local GameAI=GameAI
local string=string
--------------------------------------
--list
local TD_WaitPerLevel = TD_WaitPerLevel
local TD_PerPassData = TD_PerPassData
local TD_WeaponIndex = TD_WeaponIndex
local TD_WRangeCircle = TD_WRangeCircle
local TD_MonsterIndex = TD_MonsterIndex
local TD_MonsterBossIndex = TD_MonsterBossIndex
local TD_BulletIndex = TD_BulletIndex
local TD_AttackEffectIndex = TD_AttackEffectIndex
local TD_BeAtkEfeIndex = TD_BeAtkEfeIndex
local TD_RTEfeIndex = TD_RTEfeIndex
local TD_WImpSize = TD_WImpSize
local TD_MapPic= TD_MapPic
local TD_MapSize = TD_MapSize
local TD_MapOrigin = TD_MapOrigin
local TD_MapPath = TD_MapPath
local TD_CircleRange = TD_CircleRange
local TD_RdTypeIndex = TD_RdTypeIndex
local TD_FlyRdTypeIndex = TD_FlyRdTypeIndex
local TD_RanBossTypeIndex = TD_RanBossTypeIndex
local TD_FlyRanBossTypeIndex = TD_FlyRanBossTypeIndex
local TD_DiffCoe = TD_DiffCoe
local TD_WeaponName = TD_WeaponName
local TD_TaskIndex = TD_TaskIndex

--variable value
local TD_AllowDeadNum = TD_AllowDeadNum
local TD_MapBlockSize = TD_MapBlockSize
local TD_MapBlock = TD_MapBlock
local TD_Gold = TD_Gold
local TD_NoFireTime=TD_NoFireTime
local TD_FreezeTime=TD_FreezeTime
local TD_MNormal=TD_MNormal
local TD_MBossNormal = TD_MBossNormal
local TD_FreezeMax=TD_FreezeMax
local TD_FreezeStopMin=TD_FreezeStopMin
local TD_FreezeStopStep=TD_FreezeStopStep
local TD_NormalHurt = TD_NormalHurt
local TD_BossHurt = TD_BossHurt


---关于其他控件的名字---------------------------------
local TD_ItemSell=TD_ItemSell
local TD_ItemImprove=TD_ItemImprove
local TD_PicWDrag=TD_PicWDrag
local TD_MlifePro = TD_MlifePro
local TD_WimprovePro = TD_WimprovePro
local TD_RTEfePre = TD_RTEfePre
local TD_BeAtkEfePre = TD_BeAtkEfePre
local TD_AttackEffectPre = TD_AttackEffectPre
local TD_BulletPrefix=TD_BulletPrefix
local TD_MonsterPrefix=TD_MonsterPrefix
local TD_MonsterVariPrefix=TD_MonsterVariPrefix
local TD_WeaponRangePre=TD_WeaponRangePre
local TD_WeaponButtonPrefix=TD_WeaponButtonPrefix
local TD_WeaponPrefix=TD_WeaponPrefix
--------------------------------------
local datamember = {
this = "Win_TestTowergame",
--
curgame=0,
diff = 0,
--
mlifecoe = 1,
mscorecoe = 1,
--
AllowDeath =0,
Gold = 0,
Score = 0,
--
weaponlist={},
monsterlist={},
bulletlist={},
ateffectlist={},
beatkeflist={},
rteflist={},
mlifelist={},
wimprovelist={},

--
mapheight =0,
mapwidth =0,
maporigin = {},
mapblocksize=0,
mapblockhsize = 0,
pathnum = 0,
weaponmap={},
monstermap={},
blockmap={},	--障碍和通路的地图

mapscale=0;
--
minscreen=0,
mgroupcounter = 0,	--30s内的计数器

--
			--同一拨怪物
level=0;
mcreateflag ={},
mnumcounter ={},
mcreatecounter ={},

monsterfly={},
monsterboss={},
monstertype={},
numpergroup={},

--
weaponbutton={},
weaponselect={},
--
weaponrange={},
--
startpause=TD_WaitforState,		--1为开始游戏，0为暂停


--存储每个方格的塔
winblock={},

soundflag = true,

curmonster = {},

}

Win_TestTowergame = DlgTemplate:new(datamember)
---------------------------初始化与销毁------------------------------------------------------
--============
function Win_TestTowergame:Init()
    
	self:RegisterEvent("IDCANCEL",self.OnCancel);
	
	--暂停、开始

	self:RegisterEvent("start",self.OnStart);
	self:RegisterEvent("pause",self.OnPause);
	self:RegisterEvent("continue",self.OnContinue);
	self:RegisterEvent("help",self.OnHelp);
    self:RegisterEvent("sound",self.OnSoundControl);
	
	--创建weapon
	self:RegisterEvent("weaponset",self.OnWeapon);
	self:RegisterEvent("weaponsell",self.OnWeaponSell);
	self:RegisterEvent("weaponipv",self.OnWeaponImprove);
	self:RegisterEvent("nextwave",self.OnNextWave);
	self:RegisterEvent("winmin",self.OnWinMin);
	
	--鼠标信息
	self:RegisterEvent(WM_RBUTTONDOWN,self.OnRButtonDown);	--在游戏中右鼠标点击,取消
	self:RegisterEvent(WM_LBUTTONDOWN,self.OnLButtonDown);	--在游戏中左鼠标点击，创建
	self:RegisterEvent(WM_MOUSEMOVE, self.OnMouseMove);
end
--============

function Win_TestTowergame:OnShowDialog(curgame,diff)
	
	--初始化
	self:Clear();
	
	self.curgame = curgame;	--当前关

	self.diff = diff;
	
	self.Gold = TD_Gold[self.curgame];
	self.Score = 0;
	
	self.mlifecoe = TD_DiffCoe[diff][1];
	self.mscorecoe = TD_DiffCoe[diff][2];
	
	self:SetData();
	
	DlgApi.ShowDialog(self.this,true);

		
	self:SetItem();
	self:ResizeWindows();
end

function Win_TestTowergame:OnHelp()
	Win_TestTowerHelp:OnShowDialog();
end

function Win_TestTowergame:OnSoundControl()
    self.soundflag = not(self.soundflag);
    if self.soundflag == true then
        DlgApi.ShowItem(self.this,"Btn_SoundOn",true);
        DlgApi.ShowItem(self.this,"Btn_SoundOff",false);
    else
        DlgApi.ShowItem(self.this,"Btn_SoundOn",false);
        DlgApi.ShowItem(self.this,"Btn_SoundOff",true);
    end         
end

function Win_TestTowergame:OnWinMin()

	DlgApi.ShowDialog(self.this,false);
	Win_TestTowergamemin:OnShowDialog();
end

--============

function Win_TestTowergame:SetItem()
    
   
    --初始化地图

	DlgApi.SetImageFile(self.this,"Pic_Map",TD_MapPic[self.curgame],1);
	DlgApi.SelectImageFrame(self.this,"Pic_Map",0 );
	DlgApi.ShowItem(self.this,"Pic_Map",true);    
    	--初始化控件

    DlgApi.ShowItem(self.this,"Btn_Pause",false);
    DlgApi.ShowItem(self.this,"Btn_Continue",false);
    DlgApi.ShowItem(self.this,"Btn_Start",true);
    DlgApi.ShowItem(self.this,"Pic_Monster24",false);
    DlgApi.ShowItem(self.this,"Pic_Monster32",false);
    DlgApi.ShowItem(self.this,"Pic_Bullet4",false);
    DlgApi.ShowItem(self.this,"Pic_Bullet5",false);    
    DlgApi.ShowItem(self.this,"Pic_BeAttackFreeze",false);    
    DlgApi.ShowItem(self.this,"Pic_BeAttackToAD",false);
    DlgApi.ShowItem(self.this,"Pic_BeAttackToBD",false);
    DlgApi.ShowItem(self.this,"Pic_Weapon",false);
    DlgApi.ShowItem(self.this,"Pic_MShow1",false);
    DlgApi.ShowItem(self.this,"Pic_MShow2",false);
    DlgApi.ShowItem(self.this,"Pic_MShow3",false);
    DlgApi.ShowItem(self.this,"Pic_MShow4",false);

    for i = 1,#TD_RTEfeIndex do
        DlgApi.ShowItem(self.this,"Pic_RTeffect"..i,false);
    end
	DlgApi.ShowItem(self.this,"Pic_Multinum",false);    

        
	DlgApi.EnableItem(self.this,TD_ItemSell,false);
	DlgApi.EnableItem(self.this,TD_ItemImprove,false);
	
	for i=1,#TD_WRangeCircle do
	    DlgApi.ShowItem(self.this,TD_WeaponRangePre..i,false);
	end
   
   	--设置属性显示控件		
   	DlgApi.SetItemText(self.this,"Lab_MonsterPic",TD_TextCurMonsterPic);
   	DlgApi.ShowItem(self.this,"Lab_MonsterPic",false);
	DlgApi.SetItemText(self.this,"Lab_CurTower",TD_TextCurTower);
	DlgApi.ShowItem(self.this,"Lab_CurTower",false);
	DlgApi.SetItemText(self.this,"Lab_CurTowerValue",TD_TextCurTowerValue);
	DlgApi.ShowItem(self.this,"Lab_CurTowerValue",false);
	DlgApi.SetItemText(self.this,"Lab_CurTowerA",TD_TextCurTowerA);
	DlgApi.ShowItem(self.this,"Lab_CurTowerA",false);
	DlgApi.SetItemText(self.this,"Lab_CurTowerB",TD_TextCurTowerB);
	DlgApi.ShowItem(self.this,"Lab_CurTowerB",false);
	DlgApi.SetItemText(self.this,"Lab_CurTowerToA",TD_TextCurTowerToA);
	DlgApi.ShowItem(self.this,"Lab_CurTowerToA",false);	
	DlgApi.SetItemText(self.this,"Lab_CurTowerToB",TD_TextCurTowerToB);
	DlgApi.ShowItem(self.this,"Lab_CurTowerToB",false);
	DlgApi.SetItemText(self.this,"Lab_CurTowerSpeL",TD_TextCurTowerSpeL);
	DlgApi.ShowItem(self.this,"Lab_CurTowerSpeL",false);		
	DlgApi.SetItemText(self.this,"Lab_CurTowerAttFre",TD_TextCurTowerAttFre);
	DlgApi.ShowItem(self.this,"Lab_CurTowerAttFre",false); 
	DlgApi.SetItemText(self.this,"Lab_CurTowerRadius",TD_TextCurTowerRadius);
	DlgApi.ShowItem(self.this,"Lab_CurTowerRadius",false); 
	DlgApi.SetItemText(self.this,"Lab_CurTowerMulPro",TD_TextCurTowerMulPro);
	DlgApi.ShowItem(self.this,"Lab_CurTowerMulPro",false);		
	DlgApi.SetItemText(self.this,"Lab_CurTowerSellM",TD_TextCurTowerSellM);
	DlgApi.ShowItem(self.this,"Lab_CurTowerSellM",false); 
	DlgApi.SetItemText(self.this,"Lab_CurTowerImprM",TD_TextCurTowerImprM);
	DlgApi.ShowItem(self.this,"Lab_CurTowerImprM",false); 
	
	DlgApi.SetItemText(self.this,"Edit_CurTowerType",TD_TextWtypeTitle);
	DlgApi.ShowItem(self.this,"Edit_CurTowerType",false);
	DlgApi.SetItemText(self.this,"Edit_CurTowerToair",TD_TextWAttAreaBoth);
	DlgApi.ShowItem(self.this,"Edit_CurTowerToair",false);
	DlgApi.SetItemText(self.this,"Edit_CurTowerValue",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurTowerValue",false);	
	DlgApi.SetItemText(self.this,"Edit_CurTowerA",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurTowerA",false);
	DlgApi.SetItemText(self.this,"Edit_CurTowerB",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurTowerB",false);
	DlgApi.SetItemText(self.this,"Edit_CurTowerToA",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurTowerToA",false);	
	DlgApi.SetItemText(self.this,"Edit_CurTowerToB",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurTowerToB",false);
	DlgApi.SetItemText(self.this,"Edit_CurTowerSpeL",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurTowerSpeL",false);		
	DlgApi.SetItemText(self.this,"Edit_CurTowerAttFre",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurTowerAttFre",false); 
	DlgApi.SetItemText(self.this,"Edit_CurTowerRadius",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurTowerRadius",false);
	DlgApi.SetItemText(self.this,"Edit_CurTowerMulPro",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurTowerMulPro",false);	
	DlgApi.SetItemText(self.this,"Edit_CurTowerSellM",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurTowerSellM",false);
	DlgApi.SetItemText(self.this,"Edit_CurTowerImprM",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurTowerImprM",false);		
	
	DlgApi.SetItemText(self.this,"Lab_CurMonster",TD_TextCurMonster);
	DlgApi.ShowItem(self.this,"Lab_CurMonster",false); 
	DlgApi.SetItemText(self.this,"Lab_CurMonsterLife",TD_TextCurMonsterLife);
	DlgApi.ShowItem(self.this,"Lab_CurMonsterLife",false);	
	DlgApi.SetItemText(self.this,"Lab_CurMonsterSped",TD_TextCurMonsterSped);
	DlgApi.ShowItem(self.this,"Lab_CurMonsterSped",false);	
	DlgApi.SetItemText(self.this,"Lab_CurMonsterA",TD_TextCurMonsterA);
	DlgApi.ShowItem(self.this,"Lab_CurMonsterA",false);
	DlgApi.SetItemText(self.this,"Lab_CurMonsterB",TD_TextCurMonsterB);
	DlgApi.ShowItem(self.this,"Lab_CurMonsterB",false);
	DlgApi.SetItemText(self.this,"Lab_CurMonsterM",TD_TextCurMonsterM);
	DlgApi.ShowItem(self.this,"Lab_CurMonsterM",false);	


	DlgApi.SetItemText(self.this,"Edit_CurMonsterLife",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurMonsterLife",false);	 
	DlgApi.SetItemText(self.this,"Edit_CurMonsterSped",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurMonsterSped",false);	
	DlgApi.SetItemText(self.this,"Edit_CurMonsterA",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurMonsterA",false);
	DlgApi.SetItemText(self.this,"Edit_CurMonsterB",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurMonsterB",false);
	DlgApi.SetItemText(self.this,"Edit_CurMonsterM",tostring(0));
	DlgApi.ShowItem(self.this,"Edit_CurMonsterM",false);	
	
	------------
	self:SetWeaponBtn();
	DlgApi.SetItemText(self.this,"Edit_Gold",tostring(self.Gold));
	DlgApi.SetItemText(self.this,"Edit_Score",tostring(self.Score));
	DlgApi.SetProgress(self.this,"Prog_Life",self.AllowDeath/TD_AllowDeadNum);
	DlgApi.EnableItem(self.this,"Btn_NextWave",false);
	
	--
	for i = 1,#TD_WeaponIndex do
	    DlgApi.ShowItem(self.this, TD_PicWDrag..i, false);
	end
	
	for i = 2,#TD_WeaponIndex do
	    DlgApi.SelectImageFrame(self.this,"Pic_WeaponBK"..i,0);
	end
	
	--
	DlgApi.SetItemText(self.this,"Edit_Wave",tostring(#TD_PerPassData[self.curgame]));
	
	if self.soundflag == true then
	    DlgApi.ShowItem(self.this,"Btn_SoundOn",true);
	    DlgApi.ShowItem(self.this,"Btn_SoundOff",false);
	else
	    DlgApi.ShowItem(self.this,"Btn_SoundOff",true);
	    DlgApi.ShowItem(self.this,"Btn_SoundOn",false);
	end


end

function Win_TestTowergame:SetInfoItem()
   	--设置属性显示控件		
	DlgApi.ShowItem(self.this,"Lab_CurTower",false);
	DlgApi.ShowItem(self.this,"Lab_CurTowerValue",false);
	DlgApi.ShowItem(self.this,"Lab_CurTowerA",false);
	DlgApi.ShowItem(self.this,"Lab_CurTowerB",false);
	DlgApi.ShowItem(self.this,"Lab_CurTowerToA",false);	
	DlgApi.ShowItem(self.this,"Lab_CurTowerToB",false);
	DlgApi.ShowItem(self.this,"Lab_CurTowerSpeL",false);		
	DlgApi.ShowItem(self.this,"Lab_CurTowerAttFre",false);
    DlgApi.ShowItem(self.this,"Lab_CurTowerRadius",false);
    DlgApi.ShowItem(self.this,"Lab_CurTowerMulPro",false);    
    DlgApi.ShowItem(self.this,"Lab_CurTowerSellM",false);
    DlgApi.ShowItem(self.this,"Lab_CurTowerImprM",false);

	DlgApi.ShowItem(self.this,"Edit_CurTowerValue",false);	
	DlgApi.ShowItem(self.this,"Edit_CurTowerA",false);
	DlgApi.ShowItem(self.this,"Edit_CurTowerB",false);
	DlgApi.ShowItem(self.this,"Edit_CurTowerToA",false);	
	DlgApi.ShowItem(self.this,"Edit_CurTowerToB",false);
	DlgApi.ShowItem(self.this,"Edit_CurTowerSpeL",false);		
	DlgApi.ShowItem(self.this,"Edit_CurTowerAttFre",false);
	DlgApi.ShowItem(self.this,"Edit_CurTowerRadius",false);
	DlgApi.ShowItem(self.this,"Edit_CurTowerMulPro",false);	 
	DlgApi.ShowItem(self.this,"Edit_CurTowerSellM",false);
	DlgApi.ShowItem(self.this,"Edit_CurTowerImprM",false);
	DlgApi.ShowItem(self.this,"Edit_CurTowerType",false);
	DlgApi.ShowItem(self.this,"Edit_CurTowerToair",false);
	
	DlgApi.ShowItem(self.this,"Lab_CurMonster",false); 
	DlgApi.ShowItem(self.this,"Lab_CurMonsterLife",false);	
	DlgApi.ShowItem(self.this,"Lab_CurMonsterSped",false);	
	DlgApi.ShowItem(self.this,"Lab_CurMonsterA",false);
	DlgApi.ShowItem(self.this,"Lab_CurMonsterB",false);
	DlgApi.ShowItem(self.this,"Lab_CurMonsterM",false);
	
	DlgApi.ShowItem(self.this,"Edit_CurMonsterLife",false);	
	DlgApi.ShowItem(self.this,"Edit_CurMonsterSped",false);	
	DlgApi.ShowItem(self.this,"Edit_CurMonsterA",false);
	DlgApi.ShowItem(self.this,"Edit_CurMonsterB",false);
	DlgApi.ShowItem(self.this,"Edit_CurMonsterM",false);	
end
--
function Win_TestTowergame:SetWeaponInfo(w,state)
   if  state == true then
       self:WeaponPicInfo(w);
   else
       self:WeaponButtonInfo(w);
   end
end

function Win_TestTowergame:WeaponPicInfo(wname)    
	DlgApi.ShowItem(self.this,"Lab_CurTower",true);
    local w = self.weaponlist[wname];	
	
	if w.wtype~=1 then
		--A类攻击

    	local ori = TD_WeaponIndex[w.wtype][w.grade].adamage;
    	local ad = w.adamage - ori;
    	if math.abs(ad) < 1.0e-3 then
    	    DlgApi.SetItemText(self.this,"Edit_CurTowerA",string.format("%.1f",ori) );
    	else
    	    DlgApi.SetItemText(self.this,"Edit_CurTowerA",string.format("%.1f",ori).."^6cfb4b+"..string.format("%.1f",ad));
    	end
		DlgApi.SetItemText(self.this,"Lab_CurTowerA",TD_TextCurTowerA);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerA",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerA",true);
    	
    	--B类攻击

    	ori = TD_WeaponIndex[w.wtype][w.grade].bdamage;
    	ad = w.bdamage - ori;	
    	if math.abs(ad) < 1.0e-3 then
    	    DlgApi.SetItemText(self.this,"Edit_CurTowerB",string.format("%.1f",ori));
    	else
    	    DlgApi.SetItemText(self.this,"Edit_CurTowerB",string.format("%.1f",ori).."^6cfb4b+"..string.format("%.1f",ad));
    	end
    	DlgApi.SetItemText(self.this,"Lab_CurTowerB",TD_TextCurTowerB);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerB",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerB",true);
		--对A护甲的伤害    	
    	ori = TD_WeaponIndex[w.wtype][w.grade].damagetoa;
    	ad = w.damagetoa - ori;	
    	if math.abs(ad) < 1.0e-3 then
    	    DlgApi.SetItemText(self.this,"Edit_CurTowerToA",string.format("%.0f",ori*100).."%");
    	else
    	    DlgApi.SetItemText(self.this,"Edit_CurTowerToA",string.format("%.0f",ori*100).."%".."^6cfb4b+"..string.format("%.0f",ad*100).."%");
    	end
    	DlgApi.SetItemText(self.this,"Lab_CurTowerToA",TD_TextCurTowerToA);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerToA",true);	
    	DlgApi.ShowItem(self.this,"Edit_CurTowerToA",true);
    	--对B护甲的伤害

    	ori = TD_WeaponIndex[w.wtype][w.grade].damagetob;
    	ad = w.damagetob - ori;		
    	if math.abs(ad) < 1.0e-3 then
    	    DlgApi.SetItemText(self.this,"Edit_CurTowerToB",string.format("%.0f",ori*100).."%");
    	else
    	    DlgApi.SetItemText(self.this,"Edit_CurTowerToB",string.format("%.0f",ori*100).."%".."^6cfb4b+"..string.format("%.0f",ad*100).."%");
    	end
    	DlgApi.SetItemText(self.this,"Lab_CurTowerToB",TD_TextCurTowerToB);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerToB",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerToB",true);
    	--减速

    	ori = TD_WeaponIndex[w.wtype][w.grade].freeze;
    	ad = w.freeze - ori;	
    	if math.abs(ad) < 1.0e-3 then
    	    DlgApi.SetItemText(self.this,"Edit_CurTowerSpeL",string.format("%.0f",ori*100).."%");
    	else
    	    DlgApi.SetItemText(self.this,"Edit_CurTowerSpeL",string.format("%.0f",ori*100).."%".."^6cfb4b+"..string.format("%.0f",ad*100).."%");
    	end
    	DlgApi.SetItemText(self.this,"Lab_CurTowerSpeL",TD_TextCurTowerSpeL);--
    	DlgApi.ShowItem(self.this,"Lab_CurTowerSpeL",true);	
    	DlgApi.ShowItem(self.this,"Edit_CurTowerSpeL",true);
    	--倍数攻击百分比

    	ori = TD_WeaponIndex[w.wtype][w.grade].multihit;
    	ad = w.multihit - ori;	
    	if math.abs(ad) < 1.0e-3 then
    	    DlgApi.SetItemText(self.this,"Edit_CurTowerMulPro",string.format("%.0f",ori*100).."%");
    	else
    	    DlgApi.SetItemText(self.this,"Edit_CurTowerMulPro",string.format("%.0f",ori*100).."%".."^6cfb4b+"..string.format("%.0f",ad*100).."%");
    	end
    	DlgApi.SetItemText(self.this,"Lab_CurTowerMulPro",TD_TextCurTowerMulPro);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerMulPro",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerMulPro",true);	
    	--攻击的间隔

    	ori = TD_WeaponIndex[w.wtype][w.grade].cooltime;
    	DlgApi.SetItemText(self.this,"Lab_CurTowerAttFre",TD_TextCurTowerAttFre);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerAttFre",true);	
    	DlgApi.SetItemText(self.this,"Edit_CurTowerAttFre",string.format("%.1f",ori/1000)..TD_TextSecond);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerAttFre",true); 
    	--攻击半径
    	ori = TD_WeaponIndex[w.wtype][w.grade].wradius;
    	DlgApi.SetItemText(self.this,"Lab_CurTowerRadius",TD_TextCurTowerRadius);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerRadius",true);	
    	DlgApi.SetItemText(self.this,"Edit_CurTowerRadius",string.format("%.1f",(ori+1)/2)..TD_TextGrid);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerRadius",true);
    	--售出金钱
    	ori = TD_WeaponIndex[w.wtype][w.grade].sell;
    	DlgApi.SetItemText(self.this,"Edit_CurTowerSellM",string.format("%.0f",ori));
    	DlgApi.ShowItem(self.this,"Lab_CurTowerSellM",true);	
    	DlgApi.ShowItem(self.this,"Edit_CurTowerSellM",true);		
    	--升级的金钱

    	ori = TD_WeaponIndex[w.wtype][w.grade].improvecost;
    	if ori~=nil then
    		DlgApi.ShowItem(self.this,"Lab_CurTowerImprM",true);	    
    	    DlgApi.SetItemText(self.this,"Edit_CurTowerImprM",string.format("%.0f",ori));
    		DlgApi.ShowItem(self.this,"Edit_CurTowerImprM",true);
    	end
    	--等级
        DlgApi.SetItemText(self.this,"Edit_CurTowerType",tostring(TD_WeaponName[w.wtype]..w.grade..TD_TextLevel));
    	DlgApi.ShowItem(self.this,"Edit_CurTowerType",true);
    	
    	ori = " ";
    	if TD_WeaponIndex[w.wtype][w.grade].groundflag==true then
    	    ori = TD_TextWAttGround..ori;
    	end
    	if TD_WeaponIndex[w.wtype][w.grade].airflag==true then
    	    ori = ori..TD_TextWAttAir;
    	end
    	    
        DlgApi.SetItemText(self.this,"Edit_CurTowerToair",tostring(ori));
    	DlgApi.ShowItem(self.this,"Edit_CurTowerToair",true);
	else
	    DlgApi.SetItemText(self.this,"Edit_CurTowerType",tostring(TD_WeaponName[w.wtype]));
    	DlgApi.ShowItem(self.this,"Edit_CurTowerType",true);
    	
        DlgApi.SetItemText(self.this,"Edit_CurTowerToair",TD_TextAssistFunction);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerToair",true);

		--显示石头的攻击辅助

    	local ori = TD_WeaponIndex[w.wtype][w.grade].hitadd;
   	    DlgApi.SetItemText(self.this,"Edit_CurTowerA",string.format("%.0f",ori*100).."%" );
		DlgApi.SetItemText(self.this,"Lab_CurTowerA",TD_TextAssiAtt);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerA",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerA",true);    	
    	
      	--显示石头的腐蚀辅助
      	ori = TD_WeaponIndex[w.wtype][w.grade].toabadd;
   	    DlgApi.SetItemText(self.this,"Edit_CurTowerB",string.format("%.0f",ori*100).."%" );
    	DlgApi.SetItemText(self.this,"Lab_CurTowerB",TD_TextAssiEro);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerB",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerB",true);  	
    	
		--显示石头的减速辅助

		ori = TD_WeaponIndex[w.wtype][w.grade].tofreezeadd;
  	    DlgApi.SetItemText(self.this,"Edit_CurTowerToA",string.format("%.0f",ori*100).."%");
    	DlgApi.SetItemText(self.this,"Lab_CurTowerToA",TD_TextAssiSlo);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerToA",true);	
    	DlgApi.ShowItem(self.this,"Edit_CurTowerToA",true);    	

    	--显示石头对倍击率的提升
    	ori = TD_WeaponIndex[w.wtype][w.grade].multiadd;
   	    DlgApi.SetItemText(self.this,"Edit_CurTowerToB",string.format("%.0f",ori*100).."%");
    	DlgApi.SetItemText(self.this,"Lab_CurTowerToB",TD_TextAssiMulp);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerToB",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerToB",true);
    	
    	--提升的倍数，即增加的倍数
    	ori = TD_WeaponIndex[w.wtype][w.grade].multidamageadd;
   	    DlgApi.SetItemText(self.this,"Edit_CurTowerSpeL",string.format("%.0f",ori)..TD_TextTimes);
    	DlgApi.SetItemText(self.this,"Lab_CurTowerSpeL",TD_TextAssiMulh);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerSpeL",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerSpeL",true);  
    	
    	--售出的价格

    	ori = TD_WeaponIndex[w.wtype][w.grade].sell;
    	DlgApi.SetItemText(self.this,"Lab_CurTowerAttFre",TD_TextAssiSellM);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerAttFre",true);	
    	DlgApi.SetItemText(self.this,"Edit_CurTowerAttFre",string.format("%.0f",ori) );
    	DlgApi.ShowItem(self.this,"Edit_CurTowerAttFre",true);     	 	
    	
	end
	
end

function Win_TestTowergame:WeaponButtonInfo(w)

	DlgApi.ShowItem(self.this,"Lab_CurTower",true);
	w = tonumber(w);
	if w~=1 then
		--购买价格
    	local ori = TD_WeaponIndex[w][1].createcost;
   	    DlgApi.SetItemText(self.this,"Edit_CurTowerValue",string.format("%.0f",ori) );
   	    DlgApi.SetItemText(self.this,"Lab_CurTowerValue",TD_TextCurTowerValue);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerValue",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerValue",true);	    
	    
		--A类攻击

    	ori = TD_WeaponIndex[w][1].adamage;
   	    DlgApi.SetItemText(self.this,"Edit_CurTowerA",string.format("%.1f",ori) );
   	    DlgApi.SetItemText(self.this,"Lab_CurTowerA",TD_TextCurTowerA);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerA",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerA",true);
    	
    	--B类攻击

    	ori = TD_WeaponIndex[w][1].bdamage;
   	    DlgApi.SetItemText(self.this,"Edit_CurTowerB",string.format("%.1f",ori));
   	    DlgApi.SetItemText(self.this,"Lab_CurTowerB",TD_TextCurTowerB);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerB",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerB",true);
		
		--对A护甲的伤害    	
    	ori = TD_WeaponIndex[w][1].damagetoa;
    	DlgApi.SetItemText(self.this,"Edit_CurTowerToA",string.format("%.0f",ori*100).."%");
    	DlgApi.SetItemText(self.this,"Lab_CurTowerToA",TD_TextCurTowerToA);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerToA",true);	
    	DlgApi.ShowItem(self.this,"Edit_CurTowerToA",true);

    	--对B护甲的伤害

    	ori = TD_WeaponIndex[w][1].damagetob;
		DlgApi.SetItemText(self.this,"Edit_CurTowerToB",string.format("%.0f",ori*100).."%");
		DlgApi.SetItemText(self.this,"Lab_CurTowerToB",TD_TextCurTowerToB);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerToB",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerToB",true);

    	--减速

    	ori = TD_WeaponIndex[w][1].freeze;
   	    DlgApi.SetItemText(self.this,"Edit_CurTowerSpeL",string.format("%.0f",ori*100).."%");
   	    DlgApi.SetItemText(self.this,"Lab_CurTowerSpeL",TD_TextCurTowerSpeL);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerSpeL",true);	
    	DlgApi.ShowItem(self.this,"Edit_CurTowerSpeL",true);

    	--倍数攻击百分比

    	ori = TD_WeaponIndex[w][1].multihit;
   	    DlgApi.SetItemText(self.this,"Edit_CurTowerMulPro",string.format("%.0f",ori*100).."%");
   	    DlgApi.SetItemText(self.this,"Lab_CurTowerMulPro",TD_TextCurTowerMulPro);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerMulPro",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerMulPro",true);	

    	--攻击的间隔

    	ori = TD_WeaponIndex[w][1].cooltime;
    	DlgApi.SetItemText(self.this,"Lab_CurTowerAttFre",TD_TextCurTowerAttFre);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerAttFre",true);	
    	DlgApi.SetItemText(self.this,"Edit_CurTowerAttFre",string.format("%.1f",ori/1000)..TD_TextSecond);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerAttFre",true); 
    	
    	--攻击半径
    	ori = TD_WeaponIndex[w][1].wradius;
    	DlgApi.SetItemText(self.this,"Lab_CurTowerRadius",TD_TextCurTowerRadius);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerRadius",true);	
    	DlgApi.SetItemText(self.this,"Edit_CurTowerRadius",string.format("%.1f",(ori+1)/2)..TD_TextGrid);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerRadius",true);
    	
    	--售出金钱
    	ori = TD_WeaponIndex[w][1].sell;
    	DlgApi.SetItemText(self.this,"Edit_CurTowerSellM",string.format("%.0f",ori));
    	DlgApi.ShowItem(self.this,"Lab_CurTowerSellM",true);	
    	DlgApi.ShowItem(self.this,"Edit_CurTowerSellM",true);		
    	
    	--升级的金钱

    	ori = TD_WeaponIndex[w][1].improvecost;
    	if ori~=nil then
    		DlgApi.ShowItem(self.this,"Lab_CurTowerImprM",true);	    
    	    DlgApi.SetItemText(self.this,"Edit_CurTowerImprM",string.format("%.0f",ori));
    		DlgApi.ShowItem(self.this,"Edit_CurTowerImprM",true);
    	end
    	
    	--等级
        DlgApi.SetItemText(self.this,"Edit_CurTowerType",tostring(TD_WeaponName[w].."1"..TD_TextLevel));
    	DlgApi.ShowItem(self.this,"Edit_CurTowerType",true);
    	
    	ori = " ";
    	if TD_WeaponIndex[w][1].groundflag==true then
    	    ori = TD_TextWAttGround..ori;
    	end
    	if TD_WeaponIndex[w][1].airflag==true then
    	    ori = ori..TD_TextWAttAir;
    	end
    	    
        DlgApi.SetItemText(self.this,"Edit_CurTowerToair",tostring(ori));
    	DlgApi.ShowItem(self.this,"Edit_CurTowerToair",true);
	else
    
	    DlgApi.SetItemText(self.this,"Edit_CurTowerType",tostring(TD_WeaponName[w]));
    	DlgApi.ShowItem(self.this,"Edit_CurTowerType",true);
    	
        DlgApi.SetItemText(self.this,"Edit_CurTowerToair",TD_TextAssistFunction);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerToair",true);
		--购买价格
    	local ori = TD_WeaponIndex[w][1].createcost;
   	    DlgApi.SetItemText(self.this,"Edit_CurTowerValue",string.format("%.0f",ori) );
    	DlgApi.ShowItem(self.this,"Lab_CurTowerValue",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerValue",true);
    	
 		--显示石头的攻击辅助

    	local ori = TD_WeaponIndex[w][1].hitadd;
   	    DlgApi.SetItemText(self.this,"Edit_CurTowerA",string.format("%.0f",ori*100).."%" );
		DlgApi.SetItemText(self.this,"Lab_CurTowerA",TD_TextAssiAtt);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerA",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerA",true);    	
    	
      	--显示石头的腐蚀辅助
      	ori = TD_WeaponIndex[w][1].toabadd;
   	    DlgApi.SetItemText(self.this,"Edit_CurTowerB",string.format("%.0f",ori*100).."%" );
    	DlgApi.SetItemText(self.this,"Lab_CurTowerB",TD_TextAssiEro);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerB",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerB",true);  	
    	
		--显示石头的减速辅助

		ori = TD_WeaponIndex[w][1].tofreezeadd;
  	    DlgApi.SetItemText(self.this,"Edit_CurTowerToA",string.format("%.0f",ori*100).."%");
    	DlgApi.SetItemText(self.this,"Lab_CurTowerToA",TD_TextAssiSlo);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerToA",true);	
    	DlgApi.ShowItem(self.this,"Edit_CurTowerToA",true);    	

    	--显示石头对倍击率的提升
    	ori = TD_WeaponIndex[w][1].multiadd;
   	    DlgApi.SetItemText(self.this,"Edit_CurTowerToB",string.format("%.0f",ori*100).."%");
    	DlgApi.SetItemText(self.this,"Lab_CurTowerToB",TD_TextAssiMulp);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerToB",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerToB",true);
    	
    	--提升的倍数，即增加的倍数
    	ori = TD_WeaponIndex[w][1].multidamageadd;
   	    DlgApi.SetItemText(self.this,"Edit_CurTowerSpeL",string.format("%.0f",ori)..TD_TextTimes);
    	DlgApi.SetItemText(self.this,"Lab_CurTowerSpeL",TD_TextAssiMulh);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerSpeL",true);
    	DlgApi.ShowItem(self.this,"Edit_CurTowerSpeL",true);  
    	
    	--售出的价格

    	ori = TD_WeaponIndex[w][1].sell;
    	DlgApi.SetItemText(self.this,"Lab_CurTowerAttFre",TD_TextAssiSellM);
    	DlgApi.ShowItem(self.this,"Lab_CurTowerAttFre",true);	
    	DlgApi.SetItemText(self.this,"Edit_CurTowerAttFre",string.format("%.0f",ori) );
    	DlgApi.ShowItem(self.this,"Edit_CurTowerAttFre",true);      	
    	
  	
	end
end
--
function Win_TestTowergame:SetMonsterInfo(mname)
    
    DlgApi.ShowItem(self.this,"Lab_CurMonster",true); 
    DlgApi.ShowItem(self.this,"Lab_CurMonsterLife",true);
	DlgApi.ShowItem(self.this,"Lab_CurMonsterSped",true);	
	DlgApi.ShowItem(self.this,"Lab_CurMonsterA",true);
	DlgApi.ShowItem(self.this,"Lab_CurMonsterB",true);
	DlgApi.ShowItem(self.this,"Lab_CurMonsterM",true);
	

	local m = self.monsterlist[mname].mindex;
	--生命值

	DlgApi.SetItemText(self.this,"Edit_CurMonsterLife",string.format("%.0f",m.life*self.mlifecoe));
	DlgApi.ShowItem(self.this,"Edit_CurMonsterLife",true);
	--速度
	DlgApi.SetItemText(self.this,"Edit_CurMonsterSped",string.format("%.2f",m.speed*1000/32)..TD_TextGridPerSec);
	DlgApi.ShowItem(self.this,"Edit_CurMonsterSped",true);	
	--A类的抵抗
	DlgApi.SetItemText(self.this,"Edit_CurMonsterA",string.format("%.1f",m.adefence*100).."%");
	DlgApi.ShowItem(self.this,"Edit_CurMonsterA",true);
	--B类的抵抗
	DlgApi.SetItemText(self.this,"Edit_CurMonsterB",string.format("%.1f",m.bdefence*100).."%");
	DlgApi.ShowItem(self.this,"Edit_CurMonsterB",true);	
	--回报金钱
	DlgApi.SetItemText(self.this,"Edit_CurMonsterM",string.format("%.0f",m.rewardgold));
	DlgApi.ShowItem(self.this,"Edit_CurMonsterM",true);	    
end
--
function Win_TestTowergame:SetMPicInfo(mname)

    local ret=string.match(mname,"%d+");
    if math.abs(ret%2) < 1.0e-3 then
        curpath = 2;
    else
        curpath = 1;
    end

    if self.curmonster[curpath] then

        DlgApi.ShowItem(self.this,"Lab_CurMonster",true); 
        DlgApi.ShowItem(self.this,"Lab_CurMonsterLife",true);
    	DlgApi.ShowItem(self.this,"Lab_CurMonsterSped",true);	
    	DlgApi.ShowItem(self.this,"Lab_CurMonsterA",true);
    	DlgApi.ShowItem(self.this,"Lab_CurMonsterB",true);
    	DlgApi.ShowItem(self.this,"Lab_CurMonsterM",true);
    	
    
    	local m = self.curmonster[curpath];
    	--生命值

    	DlgApi.SetItemText(self.this,"Edit_CurMonsterLife",string.format("%.0f",m.life*self.mlifecoe));
    	DlgApi.ShowItem(self.this,"Edit_CurMonsterLife",true);
    	--速度
    	DlgApi.SetItemText(self.this,"Edit_CurMonsterSped",string.format("%.2f",m.speed*1000/32)..TD_TextGridPerSec);
    	DlgApi.ShowItem(self.this,"Edit_CurMonsterSped",true);	
    	--A类的抵抗
    	DlgApi.SetItemText(self.this,"Edit_CurMonsterA",string.format("%.1f",m.adefence*100).."%");
    	DlgApi.ShowItem(self.this,"Edit_CurMonsterA",true);
    	--B类的抵抗
    	DlgApi.SetItemText(self.this,"Edit_CurMonsterB",string.format("%.1f",m.bdefence*100).."%");
    	DlgApi.ShowItem(self.this,"Edit_CurMonsterB",true);	
    	--回报金钱
    	DlgApi.SetItemText(self.this,"Edit_CurMonsterM",string.format("%.0f",m.rewardgold));
    	DlgApi.ShowItem(self.this,"Edit_CurMonsterM",true);	  
    end  
end
--===============
function Win_TestTowergame:SetData()
    
    self.AllowDeath = TD_AllowDeadNum;
--    self.Gold = TD_Gold;
--    self.Score = 0;
    
    ---map
    self.maporigin.x = TD_MapOrigin[self.curgame].x;
    self.maporigin.y = TD_MapOrigin[self.curgame].y;
    self.mapheight = TD_MapSize[self.curgame].height;
    self.mapwidth = TD_MapSize[self.curgame].width;
    self.blockmap={};
    self.monstermap={};
    	------初始化block和怪物的map分布
    for i=1,self.mapheight do
       self.blockmap[i]={};
       self.weaponmap[i]={};
       self.monstermap[i]={};
       self.winblock[i]={};
       for j=1,self.mapwidth do
           self.blockmap[i][j]=0;
           self.monstermap[i][j]=0;
           self.weaponmap[i][j]=nil;
           self.winblock[i][j]={};           
       end    
    end
    
    
		------初始化block,不能走怪物和不能放武器的block
	for i,b in pairs(TD_MapBlock[self.curgame]) do
	    self.monstermap[b[2]][b[1]]=1;
	    self.blockmap[b[2]][b[1]]=1;
	end
	
		------不能在入口和出口放武器的block
	local np = #TD_MapPath[self.curgame];
	for pathn = 1,np do
    	for i,b in pairs(TD_MapPath[self.curgame][pathn].entry) do
    	    self.blockmap[b[2]][b[1]]=1;
    	end	
    	for i,b in pairs(TD_MapPath[self.curgame][pathn].exit) do
    	    self.blockmap[b[2]][b[1]]=1;
    	end	    	
	end
	for i,b in pairs(TD_ExpMapPath[self.curgame]) do
	    self.blockmap[b[2]][b[1]]=1;
	end	
	
		
	self.mapblocksize=TD_MapBlockSize;
	self.mapblockhsize = TD_MapBlockSize/2;

	-----init game
	self.startpause = TD_WaitforState;
	
	self.weaponlist={};
	self.weaponnum=0;
	self.wimprovelist={};
	self.wimprovenum=0;
	self.monsterlist={};
	self.mlifelist={};
	self.monsternum=0;
	self.bulletlist={}
	self.bulletnum=0;
	self.ateffectlist={};
	self.ateffectnum=0;
	self.beatkeflist={};
	self.beatkefnum=0;
	self.rteflist={};
	self.rtefnum=0;
	
	--self.soundflag = true;
	self.weaponbutton={ state=false, wtype=1 };

	self.weaponselect={};
	self.weaponselect.state=false;
	
	--counter and flag
	self.minscreen=0;
	self.level=0;
	self.mgroupcounter = 0;
	for i=1,#TD_MapPath[self.curgame] do
	    self.mcreateflag[i] = 0;
	    self.mnumcounter[i] = 0;
	    self.mcreatecounter[i] = 0;
	end
	
	--圆圈圈的item参数
	for i=1,#TD_WRangeCircle do
	    self.weaponrange[i]={ frame=0;framecounter=0 };
	end
	
	--初始化寻径地图

	if #TD_MapBlock[self.curgame]<1 then
	    GameAI.InitTDPath(self.mapwidth, self.mapheight,TD_MapPath[self.curgame]);
	else
	    GameAI.InitTDPath(self.mapwidth, self.mapheight,TD_MapPath[self.curgame],TD_MapBlock[self.curgame]);
	end
	
	self.curmonster = {};
end

--=============

function Win_TestTowergame:ResizeWindows()
    --读取游戏的框标尺
    self.mapscale = DlgApi.GetWindowScale();

end

--=====================
function Win_TestTowergame:OnCancel()

    self:Clear();
    DlgApi.ShowDialog(self.this,false);
	Win_TowerDefense:OnShowDialog();

end
--===============

function Win_TestTowergame:Clear()
    
 
	for mname,m in pairs(self.monsterlist) do
		DlgApi.DeleteControl(self.this,mname);
		self.monsterlist[mname]=nil;
	end
	for mpname,mp in pairs(self.mlifelist) do
	    DlgApi.DeleteControl(self.this,mpname);
		self.mlifelist[mpname]=nil;
	end
	for wname,w in pairs(self.weaponlist) do
		DlgApi.DeleteControl(self.this,wname);
		self.weaponlist[wname]=nil;
	end
	for wname,w in pairs(self.wimprovelist) do
		DlgApi.DeleteControl(self.this,wname);
		self.wimprovelist[wname]=nil;
	end	
	for bname,b in pairs(self.bulletlist) do
		DlgApi.DeleteControl(self.this,bname);
		self.bulletlist[bname]=nil;
	end	
	for aname,a in pairs(self.ateffectlist) do
		DlgApi.DeleteControl(self.this,aname);
		self.ateffectlist[aname]=nil;
	end
	for aname,a in pairs(self.beatkeflist) do
	    DlgApi.DeleteControl(self.this,aname);
	    self.beatkeflist[aname]=nil;
	end
	for aname,a in pairs(self.rteflist) do
	    DlgApi.DeleteControl(self.this,aname);
	    self.rteflist[aname]=nil;
	end

	for i=1,#TD_WRangeCircle do
	    DlgApi.ShowItem(self.this,TD_WeaponRangePre..i,false);
	end	

end
    

-----------------------------关于游戏按钮响应---------------------------------------------------------
--==========================
--暂停
function Win_TestTowergame:OnStart()
    if self.startpause==TD_WaitforState then	--初始游戏，转为游戏开始

        self.startpause=TD_PlayingState;
        self.mgroupcounter=TD_WaitPerLevel[self.curgame]+1;--开始游戏就出怪物
        DlgApi.ShowItem(self.this,"Btn_Start",false);
        DlgApi.ShowItem(self.this,"Btn_Pause",true);
        DlgApi.EnableItem(self.this,"Btn_Pause",true);

    end
end

function Win_TestTowergame:OnPause()
    if self.startpause == TD_PlayingState then		--游戏转为暂停
        self.startpause=TD_PauseState;
        DlgApi.ShowItem(self.this,"Btn_Pause",false);
        DlgApi.ShowItem(self.this,"Btn_Continue",true);
    end
	
end

function Win_TestTowergame:OnContinue()
    if self.startpause == TD_PauseState then						--暂停转为游戏中

        self.startpause= TD_PlayingState;
        self:OnRButtonDown();
        DlgApi.ShowItem(self.this,"Btn_Continue",false);
        DlgApi.ShowItem(self.this,"Btn_Pause",true);
    end
end
--==========================

function Win_TestTowergame:OnWeapon()
    
    if	self.startpause==TD_PauseState then
        return true;
	end
	self:SetInfoItem();
	local DlgName,ItemName = DlgApi.GetHitItem();
	
	if ItemName==nil then
	    return true;
	end
	
    if string.match(ItemName,TD_WeaponButtonPrefix.."%d+")==nil then
        return true;
    end
    
        local wtype=string.match(ItemName,"%d+");
        self:SetWeaponInfo(wtype,false);
        wtype=tonumber(wtype);
        
        if self.Gold>=TD_WeaponIndex[wtype][1].createcost then
              							--选择了command为"weaponset"的武器

    		self.weaponbutton.state = true;	--表示创建武器的按钮已经按下

    		self.weaponbutton.wtype = wtype;	--记录创建的武器的种类（按照类型创建不同的控件）

    		self.weaponbutton.bname = "Pic_Weapon";--TD_WeaponPrefix..wtype;
    		DlgApi.SelectImageFrame(self.this,"Pic_WeaponBK"..self.weaponbutton.wtype,1);
    		
    		self.weaponselect.state = false;
    		
    		--需要取消武器上的圆圈圈
    		for i=1,#TD_WRangeCircle do
    		    DlgApi.ShowItem(self.this,TD_WeaponRangePre..i,false);
    		end
    		
    				--disable 升级键和卖出键

    		DlgApi.EnableItem(self.this,TD_ItemSell,false);
    		DlgApi.EnableItem(self.this,TD_ItemImprove,false);
        end
        
	return true;
end

--===========
--根据钱来判断是否激活武器的按钮
function Win_TestTowergame:SetWeaponBtn()
	for i=1,#TD_WeaponIndex do
	    if self.Gold<TD_WeaponIndex[i][1].createcost then
			DlgApi.EnableItem(self.this,TD_WeaponButtonPrefix..i,false);
	    else
	        DlgApi.EnableItem(self.this,TD_WeaponButtonPrefix..i,true);
	    end
	end
	if self.weaponselect.state==true and self.weaponlist[self.weaponselect.serial]~=nil then
    	local wtype = self.weaponlist[self.weaponselect.serial].wtype;
		local wgrade = self.weaponlist[self.weaponselect.serial].grade;
	
		if(  wgrade < #TD_WeaponIndex[wtype] and self.Gold >= TD_WeaponIndex[wtype][wgrade].improvecost and self.weaponlist[self.weaponselect.serial].improveflag~=true) then
	    	--激活升级的item
	    	DlgApi.EnableItem(self.this,TD_ItemImprove,true);
	    else
	        --diable升级的item
	    	DlgApi.EnableItem(self.this,TD_ItemImprove,false);  
		end
	end
end
--=============================

function Win_TestTowergame:OnWeaponSell()
	if self.startpause==TD_PauseState then
	    return true;
	end

    self:SetInfoItem();	
	if self.weaponselect.state==false then
   		DlgApi.EnableItem(self.this,TD_ItemSell,false);
		DlgApi.EnableItem(self.this,TD_ItemImprove,false);
	    return true;
	end
	
	local sellname=self.weaponselect.serial;
	local selltype=self.weaponlist[self.weaponselect.serial].wtype;
	local sellgrade=self.weaponlist[self.weaponselect.serial].grade;
	
    local pathbx=self.weaponlist[sellname].bpos[1].bx;
    local pathby=self.weaponlist[sellname].bpos[1].by;
   	local pathflag = GameAI.SetBlockPosition(pathbx,pathby,0);
	if pathflag==false then
	    self.weaponselect.state=false;
	    DlgApi.EnableItem(self.this,TD_ItemSell,false);
	    return true;
	end
	
    DlgApi.DeleteControl(self.this,sellname);
    self.Gold=self.Gold + TD_WeaponIndex[selltype][sellgrade].sell;
    self:SetWeaponBtn();
    DlgApi.SetItemText(self.this,"Edit_Gold",tostring(self.Gold));
		
	--是否为辅助塔的处理

	self:AssistTChange(sellname,selltype,sellgrade,true);

	for i=1,4 do
	    self.blockmap[self.weaponlist[sellname].bpos[i].by][self.weaponlist[sellname].bpos[i].bx]=0;
	   	self.weaponmap[self.weaponlist[sellname].bpos[i].by][self.weaponlist[sellname].bpos[i].bx]=nil;
	end

   	--从它的攻击范围内的列表中取出
   	local ret = TD_WeaponIndex[selltype][sellgrade].wradius;
   	if ret~=nil then
       	local bpos = self.weaponlist[sellname].bpos;
       	for i=bpos[1].by-ret,bpos[4].by+ret do
       	    for j = bpos[1].bx-ret,bpos[4].bx+ret do
       			if i>=1 and i<=self.mapheight and j>=1 and j<=self.mapwidth then
       				self.winblock[i][j][sellname]=nil;
       			end
       	    end
       	end
   	end		
	--需要取消正在升级的进度条

	if self.weaponlist[sellname].improvepro~=nil then
	    DlgApi.DeleteControl(self.this,self.weaponlist[sellname].improvepro); 
		self.wimprovelist[self.weaponlist[sellname].improvepro]=nil;
	end
	
	self.weaponlist[sellname]=nil;
	self.weaponselect.state = false;		
	--需要取消圈圈

	for i=1,#TD_WRangeCircle do
	    DlgApi.ShowItem(self.this,TD_WeaponRangePre..i,false);
	end
	
	DlgApi.EnableItem(self.this,TD_ItemSell,false);
	DlgApi.EnableItem(self.this,TD_ItemImprove,false);
	
	return true;
end

--======================================
function Win_TestTowergame:OnWeaponImprove()
	if self.startpause==TD_PauseState then
	    return true;
	end

    self:SetInfoItem();	
	if self.weaponselect==false then
		DlgApi.EnableItem(self.this,TD_ItemSell,false);
		DlgApi.EnableItem(self.this,TD_ItemImprove,false);
	    return true;
	end
							--已经升到顶或者钱不够不能升级
	if	self.weaponlist[self.weaponselect.serial].grade >= #TD_WeaponIndex[self.weaponlist[self.weaponselect.serial].wtype] or 
		self.Gold < TD_WeaponIndex[self.weaponlist[self.weaponselect.serial].wtype][self.weaponlist[self.weaponselect.serial].grade].improvecost	then
		 					--这个可以省略
		DlgApi.EnableItem(self.this,TD_ItemImprove,false);
		return true;
	end
			--正在升级的武器能卖但是不能升级

	if self.weaponlist[self.weaponselect.serial].improveflag==true then
	    return true;
	end	  
	
	if self.startpause==TD_WaitforState then		--表示游戏真正开始之前，升级不需要等待，等开始之后，暂停过程不能买卖升级，进行时需要计时

		local imprname=self.weaponselect.serial;
		local imprtype=self.weaponlist[imprname].wtype;
		
		self.Gold=self.Gold-TD_WeaponIndex[imprtype][self.weaponlist[imprname].grade].improvecost;
		self:SetWeaponBtn();
		DlgApi.SetItemText(self.this,"Edit_Gold",tostring(self.Gold));
		
		self.weaponlist[imprname].grade=self.weaponlist[imprname].grade+1;
		local imprgrade=self.weaponlist[imprname].grade;
		
		--当进行升级的不是辅助塔时，按照原来加强的系数进行加强
	   	self.weaponlist[imprname].adamage=TD_WeaponIndex[imprtype][imprgrade].adamage*(1+self.weaponlist[imprname].addcoea);
	   	self.weaponlist[imprname].bdamage=TD_WeaponIndex[imprtype][imprgrade].bdamage*(1+self.weaponlist[imprname].addcoeb);
	   	self.weaponlist[imprname].damagetoa=TD_WeaponIndex[imprtype][imprgrade].damagetoa*(1+self.weaponlist[imprname].addcoetoa);
	   	self.weaponlist[imprname].damagetob=TD_WeaponIndex[imprtype][imprgrade].damagetob*(1+self.weaponlist[imprname].addcoetob);
	    self.weaponlist[imprname].freeze=TD_WeaponIndex[imprtype][imprgrade].freeze*(1+self.weaponlist[imprname].addcoefreeze);	

		self.weaponlist[imprname].multihit=TD_WeaponIndex[imprtype][imprgrade].multihit+self.weaponlist[imprname].addcoemultihit;
		self.weaponlist[imprname].multidamage=TD_WeaponIndex[imprtype][imprgrade].multidamage+self.weaponlist[imprname].addcoemultidamage;
	   	self.weaponlist[imprname].cooltime=TD_WeaponIndex[imprtype][imprgrade].cooltime*(1-self.weaponlist[imprname].addcoecool);

       self.weaponlist[imprname].airflag=TD_WeaponIndex[imprtype][imprgrade].airflag;
       self.weaponlist[imprname].groundflag=TD_WeaponIndex[imprtype][imprgrade].groundflag;
		--当升级的是辅助塔时

		self:AssistTChange(imprname,imprtype,imprgrade,false);    

	    --设置升级后的图片
	    DlgApi.SetImageFile(self.this, imprname, TD_WeaponIndex[imprtype][imprgrade].picname, TD_WeaponIndex[imprtype][imprgrade].frames);
		--不需要改变控件的方向
	    DlgApi.SelectImageFrame(self.this, imprname, self.weaponlist[imprname].directindex);
	    
	    --更换圆圈
	    DlgApi.ShowItem(self.this,TD_WeaponRangePre..TD_WeaponIndex[imprtype][imprgrade-1].rangecircle,false);
		local posx=self.weaponlist[imprname].bpos[1].bx*self.mapblocksize+self.maporigin.x-TD_WRangeCircle[TD_WeaponIndex[imprtype][imprgrade].rangecircle].w+1;
		local posy=self.weaponlist[imprname].bpos[1].by*self.mapblocksize+self.maporigin.y-TD_WRangeCircle[TD_WeaponIndex[imprtype][imprgrade].rangecircle].h+1;
		self:SetItemPos(TD_WeaponRangePre..TD_WeaponIndex[imprtype][imprgrade].rangecircle, posx, posy);	    
		DlgApi.ShowItem(self.this,TD_WeaponRangePre..TD_WeaponIndex[imprtype][imprgrade].rangecircle,true);

	   	    	--存入它的攻击范围内的列表
       	local ret = TD_WeaponIndex[imprtype][imprgrade].wradius;
       	if ret~=nil then
           	local bpos = self.weaponlist[imprname].bpos;
           	for i=bpos[1].by-ret,bpos[4].by+ret do
           	    for j = bpos[1].bx-ret,bpos[4].bx+ret do
           			if i>=1 and i<=self.mapheight and j>=1 and j<=self.mapwidth then
           				self.winblock[i][j][imprname]=true;
           			end
           	    end
           	end
       	end
       		--剔除不该在圆圈里的块
       	local retcircle = TD_WeaponIndex[imprtype][imprgrade].rangecircle;
       	if retcircle~=nil then
       	    local bpos = self.weaponlist[imprname].bpos;
           	for n=1,#TD_CircleRange[retcircle] do
           	    local i = TD_CircleRange[retcircle][n][2]+bpos[1].by;
           	    local j = TD_CircleRange[retcircle][n][1]+bpos[1].bx;
       			if i>=1 and i<=self.mapheight and j>=1 and j<=self.mapwidth then
       				self.winblock[i][j][imprname]=nil;
       			end
    		end            	
       	end      	

		self:SetWeaponInfo(imprname,true);		
			
		if	self.weaponlist[imprname].grade >= #TD_WeaponIndex[imprtype] 
		    or self.Gold < TD_WeaponIndex[imprtype][imprgrade].improvecost then
				DlgApi.EnableItem(self.this,TD_ItemImprove,false);
		end		

	elseif	self.startpause==TD_PlayingState then
	    	local imprname=self.weaponselect.serial;
	    	local imprtype=self.weaponlist[imprname].wtype;
	    	local imprgrade=self.weaponlist[imprname].grade;
    	
		    self.Gold=self.Gold-TD_WeaponIndex[imprtype][self.weaponlist[imprname].grade].improvecost;
		    self:SetWeaponBtn();
		    DlgApi.SetItemText(self.this,"Edit_Gold",tostring(self.Gold));

		    self.weaponlist[imprname].grade=self.weaponlist[imprname].grade+1;--必须先扣钱，最高级improvecost为空
		    local imprgrade=self.weaponlist[imprname].grade;

    		--当进行升级的不是辅助塔时，按照原来加强的系数进行加强
    	   	self.weaponlist[imprname].adamage=TD_WeaponIndex[imprtype][imprgrade].adamage*(1+self.weaponlist[imprname].addcoea);
    	   	self.weaponlist[imprname].bdamage=TD_WeaponIndex[imprtype][imprgrade].bdamage*(1+self.weaponlist[imprname].addcoeb);
    	   	self.weaponlist[imprname].damagetoa=TD_WeaponIndex[imprtype][imprgrade].damagetoa*(1+self.weaponlist[imprname].addcoetoa);
    	   	self.weaponlist[imprname].damagetob=TD_WeaponIndex[imprtype][imprgrade].damagetob*(1+self.weaponlist[imprname].addcoetob);
    	    self.weaponlist[imprname].freeze=TD_WeaponIndex[imprtype][imprgrade].freeze*(1+self.weaponlist[imprname].addcoefreeze);	
    
    		self.weaponlist[imprname].multihit=TD_WeaponIndex[imprtype][imprgrade].multihit+self.weaponlist[imprname].addcoemultihit;
    		self.weaponlist[imprname].multidamage=TD_WeaponIndex[imprtype][imprgrade].multidamage+self.weaponlist[imprname].addcoemultidamage;
    	   	self.weaponlist[imprname].cooltime=TD_WeaponIndex[imprtype][imprgrade].cooltime*(1-self.weaponlist[imprname].addcoecool);
    	   	
            self.weaponlist[imprname].airflag=TD_WeaponIndex[imprtype][imprgrade].airflag;
            self.weaponlist[imprname].groundflag=TD_WeaponIndex[imprtype][imprgrade].groundflag;    	   		 
    	   			    
		    self.weaponlist[imprname].improveflag=true;
		    self.weaponlist[imprname].improvecounter=0;
		    DlgApi.EnableItem(self.this,TD_ItemImprove,false);
		    
    	   	    	--存入它的攻击范围内的列表
           	local ret = TD_WeaponIndex[imprtype][imprgrade].wradius;
           	if ret ~= nil then
               	local bpos = self.weaponlist[imprname].bpos;
               	for i=bpos[1].by-ret,bpos[4].by+ret do
               	    for j = bpos[1].bx-ret,bpos[4].bx+ret do
               			if i>=1 and i<=self.mapheight and j>=1 and j<=self.mapwidth then
               				self.winblock[i][j][imprname]=true;
               			end
               	    end
               	end		
           	end    
           		--剔除不该在圆圈里的块
           	local retcircle = TD_WeaponIndex[imprtype][imprgrade].rangecircle;
           	if retcircle~=nil then
           	    local bpos = self.weaponlist[imprname].bpos;
               	for n=1,#TD_CircleRange[retcircle] do
               	    local i = TD_CircleRange[retcircle][n][2]+bpos[1].by;
               	    local j = TD_CircleRange[retcircle][n][1]+bpos[1].bx;
           			if i>=1 and i<=self.mapheight and j>=1 and j<=self.mapwidth then
           				self.winblock[i][j][imprname]=nil;
           			end
        		end            	
           	end	
           	
		    ----进度条

		    self.wimprovenum=self.wimprovenum+1;
		    local btn="WImprove"..self.wimprovenum;
		    self.wimprovelist[btn]=true;
		    self.weaponlist[imprname].improvepro=btn;
		    DlgApi.CreateControl(self.this,TD_WimprovePro,btn);
			DlgApi.SetProgress(self.this,btn ,0);
			
			--local ret=DlgApi.GetItemRect(self.this, btn);			
			local posx= math.ceil( (self.weaponlist[imprname].bpos[1].bx)*self.mapblocksize+self.maporigin.x-TD_WImpSize.w);
			local posy= math.ceil( (self.weaponlist[imprname].bpos[1].by-1)*self.mapblocksize+self.maporigin.y-TD_WImpSize.h);
			self:SetItemPos(btn,posx,posy-4);	
			DlgApi.ShowItem(self.this,btn,true);
			
		    self:SetWeaponInfo(imprname,true);

	   
	end
	return true;
end

--=================================
--送出下一波的怪物，当当前没有怪物存在时

function Win_TestTowergame:OnNextWave()
    if self.startpause~= TD_PauseState then
    	DlgApi.EnableItem(self.this,"Btn_NextWave",false);
    	self.Score = self.Score + (TD_WaitPerLevel[self.curgame]-self.mgroupcounter)/1000;
    	DlgApi.SetItemText( self.this,"Edit_Score",tostring(math.ceil(self.Score)) );
    	self.mgroupcounter=TD_WaitPerLevel[self.curgame];
    end
end

---------------------------关于辅助类武器建立销毁与升级的影响---------------------------------------
--========================================
function Win_TestTowergame:AssistTChange(wname,wtype,wgrade,sellflag)
		--需要判断拆除的塔是不是属性加强的塔


				--辅助塔

   	if TD_WeaponIndex[wtype][wgrade].addradius~=nil and (TD_WeaponIndex[wtype][wgrade].hitadd~=nil or TD_WeaponIndex[wtype][wgrade].multiadd~=nil or TD_WeaponIndex[wtype][wgrade].speedadd~=nil) then
   	    local blockradius=math.ceil(TD_WeaponIndex[wtype][wgrade].addradius/self.mapblocksize);
   	    local weaponinradius={};
   	    local rect={};
   	    rect.top={ x=self.weaponlist[wname].bpos[1].bx-blockradius, y=self.weaponlist[wname].bpos[1].by-blockradius };
   	    rect.down={ x=self.weaponlist[wname].bpos[1].bx+blockradius, y=self.weaponlist[wname].bpos[1].by+blockradius };
   	    
   	    for i=rect.top.y,rect.down.y do
   	        for j=rect.top.x,rect.down.x do

   	            if i>=1 and i<=self.mapheight and j>=1 and j<=self.mapwidth and self.weaponmap[i][j]~=nil and weaponinradius[self.weaponmap[i][j]]==nil then	
   	            --ij中有武器,并且该武器还没有添加到范围武器列表中
   	            	local detx=(j-1+0.5)*self.mapblocksize-self.weaponlist[wname].bpos[1].bx*self.mapblocksize;
   	                local dety=(i-1+0.5)*self.mapblocksize-self.weaponlist[wname].bpos[1].by*self.mapblocksize;
   	            	local r=detx^2+dety^2;
   	            	if r<=TD_WeaponIndex[wtype][wgrade].addradius^2 then
   	            	    weaponinradius[self.weaponmap[i][j]]=true;
   	            	end
   	            end
   	        end
   	    end
   	   
		if sellflag==true then
   	    	self:AssistTSell(wname,wtype,wgrade,weaponinradius);
		else
		    self:AssisttoT(wname,wtype,wgrade,weaponinradius)
		end
   
   end
	   	    	    
end

--========================================
function Win_TestTowergame:AssistTSell(sellname,selltype,sellgrade,weaponinradius)
	for name,state in pairs(weaponinradius) do	--遍历范围内的武器列表，更新数据

       	
       	if TD_WeaponIndex[selltype][sellgrade].addradius~=nil and TD_WeaponIndex[selltype][sellgrade].hitadd ~= nil then
       	    
       	    if TD_WeaponIndex[selltype][sellgrade].hitadd==self.weaponlist[name].addcoea or TD_WeaponIndex[selltype][sellgrade].hitadd==self.weaponlist[name].addcoeb
       	    	or TD_WeaponIndex[selltype][sellgrade].toabadd==self.weaponlist[name].addcoetoa or TD_WeaponIndex[selltype][sellgrade].toabadd==self.weaponlist[name].addcoetob
       	    	or TD_WeaponIndex[selltype][sellgrade].tofreezeadd==self.weaponlist[name].addcoefreeze then	--如果由这个待拆炮台加强

     	    	
       		    	self.weaponlist[name].adamage=TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].adamage;
       		    	self.weaponlist[name].addcoea=0;
       		    	
       		    	self.weaponlist[name].bdamage=TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].bdamage;
       		    	self.weaponlist[name].addcoeb=0;
       		    	
       		    	self.weaponlist[name].damagetoa=TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].damagetoa;
    				self.weaponlist[name].addcoetoa=0;
    
       		    	self.weaponlist[name].damagetob=TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].damagetob;
    				self.weaponlist[name].addcoetob=0;
    
       		    	self.weaponlist[name].freeze=TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].freeze;
    				self.weaponlist[name].addcoefreeze=0;
    
       	    	for wname,w in pairs(self.weaponlist) do   	    				    	    
    	   			if wname~=sellname and TD_WeaponIndex[w.wtype][w.grade].addradius~=nil and TD_WeaponIndex[w.wtype][w.grade].hitadd~=nil then
    	   		    	local detx=(self.weaponlist[name].bpos[1].bx-w.bpos[1].bx)*self.mapblocksize;
    	   	    		local dety=(self.weaponlist[name].bpos[1].by-w.bpos[1].by)*self.mapblocksize;
    	   	    		local r=detx^2+dety^2;
    	   	    		if r<=TD_WeaponIndex[w.wtype][w.grade].addradius^2 then		--在w的加强范围内
    	   	    		
       	   	    			if TD_WeaponIndex[w.wtype][w.grade].hitadd>self.weaponlist[name].addcoea or TD_WeaponIndex[w.wtype][w.grade].hitadd>self.weaponlist[name].addcoeb
    	   	    				or TD_WeaponIndex[w.wtype][w.grade].toabadd>self.weaponlist[name].addcoetoa or TD_WeaponIndex[w.wtype][w.grade].toabadd>self.weaponlist[name].addcoetob 
    	   	    					or TD_WeaponIndex[w.wtype][w.grade].tofreezeadd>self.weaponlist[name].addcoefreeze then	--攻击被加强

    	   	    		    	
    	   	    		    	self.weaponlist[name].addcoea=TD_WeaponIndex[w.wtype][w.grade].hitadd;
    	   	    		    	self.weaponlist[name].adamage=(1+self.weaponlist[name].addcoea)*TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].adamage;
    	   	    		    	
    	   	    		    	self.weaponlist[name].addcoeb=TD_WeaponIndex[w.wtype][w.grade].hitadd;
    	   	    		        self.weaponlist[name].bdamage=(1+self.weaponlist[name].addcoeb)*TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].bdamage;
    	   	    		        
    	   	    		        self.weaponlist[name].addcoetoa=TD_WeaponIndex[w.wtype][w.grade].toabadd;
    	   	    		        self.weaponlist[name].damagetoa=(1+self.weaponlist[name].addcoetoa)*TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].damagetoa;
    
    							self.weaponlist[name].addcoetob=TD_WeaponIndex[w.wtype][w.grade].toabadd;
    	   	    		        self.weaponlist[name].damagetob=(1+self.weaponlist[name].addcoetob)*TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].damagetob;
    
    							self.weaponlist[name].addcoefreeze=TD_WeaponIndex[w.wtype][w.grade].tofreezeadd;
    	   	    		        self.weaponlist[name].freeze=(1+self.weaponlist[name].addcoefreeze)*TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].freeze;
    
    	   	    		    end
    	   	    		end
       
    	   	    	end
    	   	    		
    	   	  	end
       	    end
       	end

       	if TD_WeaponIndex[selltype][sellgrade].addradius~=nil and TD_WeaponIndex[selltype][sellgrade].multiadd ~=nil then
       	    
       	    if TD_WeaponIndex[selltype][sellgrade].multiadd==self.weaponlist[name].addcoemultihit then	--如果由这个待拆炮台加强

       	    	
       	    	self.weaponlist[name].addcoemultihit=0;
       	    	self.weaponlist[name].addcoemultidamage=0;
       	    	self.weaponlist[name].multihit=TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].multihit;
       	    	self.weaponlist[name].multidamage=TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].multidamage;
    
       	   		for wname,w in pairs(self.weaponlist) do
    	   	    	if wname~=sellname and TD_WeaponIndex[w.wtype][w.grade]~=nil and TD_WeaponIndex[w.wtype][w.grade].multiadd~=nil then
    	   	    		local detx=(self.weaponlist[name].bpos[1].bx-w.bpos[1].bx)*self.mapblocksize;
    	   	    		local dety=(self.weaponlist[name].bpos[1].by-w.bpos[1].by)*self.mapblocksize;
    	   	    		local r=detx^2+dety^2;
    	   	    		if r<=TD_WeaponIndex[w.wtype][w.grade].addradius^2 then		--在w的加强范围内
    	   	    		
    	   	    		    if TD_WeaponIndex[w.wtype][w.grade].multiadd>self.weaponlist[name].addcoemultihit then
    	   	    		        
    	   	    		        self.weaponlist[name].addcoemultihit=TD_WeaponIndex[w.wtype][w.grade].multiadd;
    	   	    		        self.weaponlist[name].addcoemultidamage=TD_WeaponIndex[w.wtype][w.grade].multidamageadd;
    	   	    		    	self.weaponlist[name].multihit=self.weaponlist[name].addcoemultihit+TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].multihit;
    	   	    		    	self.weaponlist[name].multidamage=self.weaponlist[name].addcoemultidamage+TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].multidamage;
    	   	    		    end
    	   	    		end   	    		    
    	   	    	end
    	   	    		
    	   	    end
            	    				        
       	    end
       	end
    
       	if TD_WeaponIndex[selltype][sellgrade].addradius~=nil and TD_WeaponIndex[selltype][sellgrade].speedadd ~=nil then
    
       	    if TD_WeaponIndex[selltype][sellgrade].speedadd==self.weaponlist[name].addcoecool then	--如果由这个待拆炮台加强

       	    
       	    	self.weaponlist[name].addcoecool=0;
    			self.weaponlist[name].cooltime=TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].cooltime;
    			
     	    	for wname,w in pairs(self.weaponlist) do
       	    		if wname~=sellname and TD_WeaponIndex[w.wtype][w.grade]~=nil and TD_WeaponIndex[w.wtype][w.grade].speedadd~=nil then
       	    			local detx=(self.weaponlist[name].bpos[1].bx-w.bpos[1].bx)*self.mapblocksize;
       	    		    local dety=(self.weaponlist[name].bpos[1].by-w.bpos[1].by)*self.mapblocksize;
       	    		    local r=detx^2+dety^2;
       	    		    if r<=TD_WeaponIndex[w.wtype][w.grade].addradius^2 then		--在w的加强范围内
       	    		    
       	    		        if TD_WeaponIndex[w.wtype][w.grade].speedadd>self.weaponlist[name].addcoecool then
       	    		            
       	    		            self.weaponlist[name].addcoecool=TD_WeaponIndex[w.wtype][w.grade].speedadd;
       	    		        	self.weaponlist[name].cooltime=(1-self.weaponlist[name].addcoecool)*TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].cooltime;
       	    		        end
       	    		    end	   	    		    
       	    		 end
           		
           		end
     	    end
       	end
	end

end
--=====================================
function Win_TestTowergame:AssisttoT(imprname,imprtype,imprgrade,weaponinradius)
   	    for name,state in pairs(weaponinradius) do	--遍历范围内的武器列表，更新数据


   			if  TD_WeaponIndex[imprtype][imprgrade].addradius~=nil and TD_WeaponIndex[imprtype][imprgrade].hitadd ~= nil then
  			    
   			    if TD_WeaponIndex[imprtype][imprgrade].hitadd>self.weaponlist[name].addcoea or TD_WeaponIndex[imprtype][imprgrade].hitadd>self.weaponlist[name].addcoeb 
   			    	or TD_WeaponIndex[imprtype][imprgrade].toabadd>self.weaponlist[name].addcoetoa or TD_WeaponIndex[imprtype][imprgrade].toabadd>self.weaponlist[name].addcoetob 
   			    		or TD_WeaponIndex[imprtype][imprgrade].tofreezeadd>self.weaponlist[name].addcoefreeze then	--如果由这个升级炮台加强

   			    
   			        self.weaponlist[name].addcoea=TD_WeaponIndex[imprtype][imprgrade].hitadd;
   			        self.weaponlist[name].adamage=(1+self.weaponlist[name].addcoea)*TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].adamage;

					self.weaponlist[name].addcoeb=TD_WeaponIndex[imprtype][imprgrade].hitadd;
   			        self.weaponlist[name].bdamage=(1+self.weaponlist[name].addcoeb)*TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].bdamage;

					self.weaponlist[name].addcoetoa=TD_WeaponIndex[imprtype][imprgrade].toabadd;		
 			        self.weaponlist[name].damagetoa=(1+self.weaponlist[name].addcoetoa)*TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].damagetoa;

					self.weaponlist[name].addcoetob=TD_WeaponIndex[imprtype][imprgrade].toabadd;
					self.weaponlist[name].damagetob=(1+self.weaponlist[name].addcoetob)*TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].damagetob;

					self.weaponlist[name].addcoefreeze=TD_WeaponIndex[imprtype][imprgrade].tofreezeadd;	
					self.weaponlist[name].freeze=(1+self.weaponlist[name].addcoefreeze)*TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].freeze;

   			    end
   			end    
   			if  TD_WeaponIndex[imprtype][imprgrade].addradius~=nil and  TD_WeaponIndex[imprtype][imprgrade].multiadd ~=nil then
   			    
   			    if TD_WeaponIndex[imprtype][imprgrade].multiadd>self.weaponlist[name].addcoemultihit then	--如果由这个升级炮台加强


					self.weaponlist[name].addcoemultihit=TD_WeaponIndex[imprtype][imprgrade].multiadd;
					self.weaponlist[name].addcoemultidamage=TD_WeaponIndex[imprtype][imprgrade].multidamageadd;
   			    	self.weaponlist[name].multihit= self.weaponlist[name].addcoemultihit+TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].multihit;
   			    	self.weaponlist[name].multidamage= self.weaponlist[name].addcoemultidamage+TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].multidamage;    				         	    				        
   			    end
   			end 
   			if  TD_WeaponIndex[imprtype][imprgrade].addradius~=nil and  TD_WeaponIndex[imprtype][imprgrade].speedadd ~=nil then

   			    if TD_WeaponIndex[imprtype][imprgrade].speedadd>self.weaponlist[name].addcoecool then	--如果由这个升级炮台加强


   			        self.weaponlist[name].addcoecool=TD_WeaponIndex[imprtype][imprgrade].speedadd;
   			        self.weaponlist[name].cooltime=(1-self.weaponlist[name].addcoecool)*TD_WeaponIndex[self.weaponlist[name].wtype][self.weaponlist[name].grade].cooltime;
 			    end
   			end
   
   	    end

end
--==================================
function Win_TestTowergame:OthertToAssist(weaponname,wtype,wgrade)
   	for wname,w in pairs(self.weaponlist) do
   	    if TD_WeaponIndex[w.wtype][w.grade].addradius~=nil and TD_WeaponIndex[w.wtype][w.grade].hitadd~=nil then
   	        local detx=(self.weaponlist[weaponname].bpos[1].bx-w.bpos[1].bx)*self.mapblocksize;
   	        local dety=(self.weaponlist[weaponname].bpos[1].by-w.bpos[1].by)*self.mapblocksize;
   	        local r=detx^2+dety^2;
   	        if r<=TD_WeaponIndex[w.wtype][w.grade].addradius^2 then		--在w的加强范围内
  	            
   	            if TD_WeaponIndex[w.wtype][w.grade].hitadd>self.weaponlist[weaponname].addcoea or TD_WeaponIndex[w.wtype][w.grade].hitadd>self.weaponlist[weaponname].addcoeb 
   	            	or TD_WeaponIndex[w.wtype][w.grade].toabadd>self.weaponlist[weaponname].addcoetoa or TD_WeaponIndex[w.wtype][w.grade].toabadd>self.weaponlist[weaponname].addcoetob 
   	            	or TD_WeaponIndex[w.wtype][w.grade].tofreezeadd>self.weaponlist[weaponname].addcoefreeze then	--攻击被加强

   	            	
   	            	self.weaponlist[weaponname].addcoea=TD_WeaponIndex[w.wtype][w.grade].hitadd;
   	            	self.weaponlist[weaponname].adamage=(1+self.weaponlist[weaponname].addcoea)*TD_WeaponIndex[wtype][wgrade].adamage;
   	            	
   	            	self.weaponlist[weaponname].addcoeb=TD_WeaponIndex[w.wtype][w.grade].hitadd;
   	            	self.weaponlist[weaponname].bdamage=(1+self.weaponlist[weaponname].addcoeb)*TD_WeaponIndex[wtype][wgrade].bdamage;

					self.weaponlist[weaponname].addcoetoa=TD_WeaponIndex[w.wtype][w.grade].toabadd;
   		            self.weaponlist[weaponname].damagetoa=(1+self.weaponlist[weaponname].addcoetoa)*TD_WeaponIndex[wtype][wgrade].damagetoa;

					self.weaponlist[weaponname].addcoetob=TD_WeaponIndex[w.wtype][w.grade].toabadd;
   		            self.weaponlist[weaponname].damagetob=(1+self.weaponlist[weaponname].addcoetob)*TD_WeaponIndex[wtype][wgrade].damagetob;

					self.weaponlist[weaponname].addcoefreeze=TD_WeaponIndex[w.wtype][w.grade].tofreezeadd;
   		            self.weaponlist[weaponname].freeze=(1+self.weaponlist[weaponname].addcoefreeze)*TD_WeaponIndex[wtype][wgrade].freeze;

   	            end
   	        end
   	    end
   	    if  TD_WeaponIndex[w.wtype][w.grade].addradius~=nil and TD_WeaponIndex[w.wtype][w.grade].multiadd~=nil then
   	    	local detx=(self.weaponlist[weaponname].bpos[1].bx-w.bpos[1].bx)*self.mapblocksize;
   	        local dety=(self.weaponlist[weaponname].bpos[1].by-w.bpos[1].by)*self.mapblocksize;
   	        local r=detx^2+dety^2;
   	        if r<=TD_WeaponIndex[w.wtype][w.grade].addradius^2 then		--在w的加强范围内

   	        	if TD_WeaponIndex[w.wtype][w.grade].multiadd>self.weaponlist[weaponname].addcoemultihit then
   	        	    
   	        	    self.weaponlist[weaponname].addcoemultihit=TD_WeaponIndex[w.wtype][w.grade].multiadd;
   	        	    self.weaponlist[weaponname].addcoemultidamage=TD_WeaponIndex[w.wtype][w.grade].multidamageadd;
   	        	    self.weaponlist[weaponname].multihit=self.weaponlist[weaponname].addcoemultihit+TD_WeaponIndex[wtype][wgrade].multihit;
   	        	    self.weaponlist[weaponname].multidamage=self.weaponlist[weaponname].addcoemultidamage+TD_WeaponIndex[wtype][wgrade].multidamage;
   	        	end
   	        end
   	    end
   	    if  TD_WeaponIndex[w.wtype][w.grade].addradius~=nil and TD_WeaponIndex[w.wtype][w.grade].speedadd~=nil then
   	    	local detx=(self.weaponlist[weaponname].bpos[1].bx-w.bpos[1].bx)*self.mapblocksize;
   	        local dety=(self.weaponlist[weaponname].bpos[1].by-w.bpos[1].by)*self.mapblocksize;
   	        local r=detx^2+dety^2;
   	        if r<=TD_WeaponIndex[w.wtype][w.grade].addradius^2 then		--在w的加强范围内

   	        	if TD_WeaponIndex[w.wtype][w.grade].speedadd>self.weaponlist[weaponname].addcoecool then
   	        	    
   	        	    self.weaponlist[weaponname].addcoecool=TD_WeaponIndex[w.wtype][w.grade].speedadd;
   	        	    self.weaponlist[weaponname].cooltime=(1-self.weaponlist[weaponname].addcoecool)*TD_WeaponIndex[wtype][wgrade].cooltime;
   	        	end
   	        end	   	    		    
   	    end
   	
   	end
end

---------------------------------关于windows消息的相应--------------------------------------------
--======================================
function Win_TestTowergame:OnMouseMove()
	
	if self.weaponbutton.state==true and self.startpause ~=TD_PauseState then
	    
    	local mousex,mousey = GameAI.GetCursorPosition();	--鼠标绝对位置
    	local TestgameDlg = DlgApi.GetDialogProp(self.this);	--dialog的绝对位置

    	
    	local dlgx = mousex-TestgameDlg.x;	--dialog相对位置
        local dlgy = mousey-TestgameDlg.y;
        
        local mapx = dlgx/self.mapscale-self.maporigin.x;		--地图上的相对坐标
        local mapy = dlgy/self.mapscale-self.maporigin.y;	
        
        local blocknx = math.ceil(mapx/self.mapblocksize);	--在地图上的block位置
        local blockny = math.ceil(mapy/self.mapblocksize); 
        
        local selectblock={}

        for i=1,4 do
            selectblock[i] = {};
        end 
    	
    	if( blocknx>=1 and blocknx<=self.mapwidth and blockny>=1 and blockny<=self.mapheight) then
    	    self:FoundBlock(selectblock,blocknx,blockny,mapx,mapy);
    	    
			for i=1,self.mapheight do
			    for j=1,self.mapwidth do
			        self.monstermap[i][j]=0;
			    end
			end
			
			for mname,m in pairs(self.monsterlist) do
			    if m.flyflag == false then
    			   self.monstermap[m.bpos.y][m.bpos.x]=1;
			    end 
			end
						
			--判断是否有初始障碍或武器或怪物
			local showflag=true;
			for i=1,4 do
			    if selectblock[i].nx<1 or selectblock[i].nx>self.mapwidth or selectblock[i].ny<1 or selectblock[i].ny> self.mapheight then
			       showflag=false;
			    elseif self.blockmap[ selectblock[i].ny ][ selectblock[i].nx ]==1 or self.monstermap[ selectblock[i].ny ][ selectblock[i].nx ]==1 then
				   showflag=false;
				end
			end
			
			if showflag == true then
			    DlgApi.SelectImageFrame(self.this, TD_PicWDrag..self.weaponbutton.wtype, 0);
			else
			    DlgApi.SelectImageFrame(self.this, TD_PicWDrag..self.weaponbutton.wtype, 1);
			    
			end
			DlgApi.ShowItem(self.this, TD_PicWDrag..self.weaponbutton.wtype, true);
			--显示武器周围的这个圆圈圈
			if TD_WeaponIndex[self.weaponbutton.wtype][1].rangecircle~=nil then
			    
    			--local ret= DlgApi.GetItemRect(self.this, TD_WeaponRangePre..TD_WeaponIndex[self.weaponbutton.wtype][1].rangecircle);
    			local posx=selectblock[1].nx*self.mapblocksize+self.maporigin.x-TD_WRangeCircle[TD_WeaponIndex[self.weaponbutton.wtype][1].rangecircle].w+1;
    			local posy=selectblock[1].ny*self.mapblocksize+self.maporigin.y-TD_WRangeCircle[TD_WeaponIndex[self.weaponbutton.wtype][1].rangecircle].h+1;
    			self:SetItemPos(TD_WeaponRangePre..TD_WeaponIndex[self.weaponbutton.wtype][1].rangecircle, posx, posy);
    			DlgApi.ShowItem(self.this,TD_WeaponRangePre..TD_WeaponIndex[self.weaponbutton.wtype][1].rangecircle,true);	
			end		
			------
			self:SetItemPos(TD_PicWDrag..self.weaponbutton.wtype, (selectblock[1].nx-1)*self.mapblocksize+self.maporigin.x, (selectblock[1].ny-1)*self.mapblocksize+self.maporigin.y);
		
		else
		    
    	for i=1,#TD_WRangeCircle do
    	    DlgApi.ShowItem(self.this,TD_WeaponRangePre..i,false);
    	end		    
		    DlgApi.ShowItem(self.this, TD_PicWDrag..self.weaponbutton.wtype, false);
		end
		
	else

	    DlgApi.ShowItem(self.this, TD_PicWDrag..self.weaponbutton.wtype, false);
	end 
	return true;
end

--====================
function Win_TestTowergame:OnRButtonDown()
    
    
    self:SetInfoItem();
    
	self.weaponbutton.state = false;	--取消要创建的武器
	self.weaponselect.state = false;	--取消选择的要进行卖或者升级的武器
	DlgApi.SelectImageFrame(self.this,"Pic_WeaponBK"..self.weaponbutton.wtype,0);
	--需要取消选中武器的圆圈圈
	for i=1,#TD_WRangeCircle do
	    DlgApi.ShowItem(self.this,TD_WeaponRangePre..i,false);
	end
	--disable卖出键和升级键

	DlgApi.EnableItem(self.this,TD_ItemSell,false);
	DlgApi.EnableItem(self.this,TD_ItemImprove,false);
	DlgApi.ShowItem(self.this, TD_PicWDrag..self.weaponbutton.wtype, false);

	return true;
end

--=========================
function Win_TestTowergame:OnLButtonDown(itemName, wParam, x, y)

	if self.startpause ~= TD_PauseState then
	    self:SetInfoItem();
	end
        
    if self.weaponbutton.state ~=true and self.startpause ~= TD_PauseState then  

        if string.match(itemName,"Monster".."%d+")  then
           self:SetMonsterInfo(itemName); 
        end
        
        if string.match(itemName,"Pic_MShow".."%d+")  then
            self:SetMPicInfo(itemName);
        end     
              
        if string.match(itemName,"Weapon".."%d+") and self.weaponlist[itemName]~=nil   then
            self:SetWeaponInfo(itemName,true);
            
            if self.weaponbutton.state~=true then
                --if self.weaponlist[itemName]~=nil then            
              		self:WeaponSelect(itemName);
                --end
            end
      		return true;
        end
        
        if	itemName~=TD_ItemSell and itemName~=TD_ItemImprove then	--在鼠标点击买和卖以外的控件的时候

           
            --撤销武器边上的圆圈圈
            for i=1,#TD_WRangeCircle do
                DlgApi.ShowItem(self.this,TD_WeaponRangePre..i,false);
            end
            --撤销武器的选定
    		self.weaponselect.state=false;	
    	   	DlgApi.EnableItem(self.this,TD_ItemSell,false);
    		DlgApi.EnableItem(self.this,TD_ItemImprove,false);
        end

	end
	
	-----在map上生成武器

    if self.weaponbutton.state ==true and self.startpause ~=TD_PauseState then
        
        local mousex,mousey = GameAI.GetCursorPosition();	--鼠标绝对位置
    	local TestgameDlg = DlgApi.GetDialogProp(self.this);	--dialog的绝对位置

    	
    	local dlgx = mousex-TestgameDlg.x;	--dialog相对位置
        local dlgy = mousey-TestgameDlg.y;
        
        local mapx = dlgx/self.mapscale-self.maporigin.x;		--地图上的相对坐标
        local mapy = dlgy/self.mapscale-self.maporigin.y;	
        
        local blocknx = math.ceil(mapx/self.mapblocksize);	--在地图上的block位置
        local blockny = math.ceil(mapy/self.mapblocksize); 
    	local selectblock={}

        for i=1,4 do
            selectblock[i] = {};
        end

    	if( blocknx>=1 and blocknx<=self.mapwidth and blockny>=1 and blockny<=self.mapheight) then
    	    self:FoundBlock(selectblock,blocknx,blockny,mapx,mapy);
			
			--统计map每格中是否有monster
			for i=1,self.mapheight do
			    for j=1,self.mapwidth do
			        self.monstermap[i][j]=0;
			    end
			end
			
			local msbpos={};			
			for mname,m in pairs(self.monsterlist) do
			    if m.flyflag == false then
    			   self.monstermap[m.bpos.y][m.bpos.x]=1;
    			   local mbpos = {m.bpos.x,m.bpos.y,m.path-1};
    			   table.insert(msbpos,mbpos);
			    end 
			end
						
			--判断是否有初始障碍或武器或怪物
			for i=1,4 do
			   if selectblock[i].nx<1 or selectblock[i].nx>self.mapwidth or selectblock[i].ny<1 or selectblock[i].ny> self.mapheight then
			       return true;
				end
				if self.blockmap[ selectblock[i].ny ][ selectblock[i].nx ]==1 or self.monstermap[ selectblock[i].ny ][ selectblock[i].nx ]==1 then
				    return true;
				end
		
			end
			
    	    --判断寻径的结果是否可以设置，取了左上方的一个方格

	   	    local pathfound = GameAI.SetBlockPosition(selectblock[1].nx, selectblock[1].ny,1,msbpos);
	   	    --如果寻径表示可以，并且没有障碍和怪物，可以创建该武器了

	   	    if pathfound == true then
	   	        if self.soundflag == true then
	   	            GameAI.Play2DSound("Sfx\\TinyGame\\TD\\放置武器.wav");
	   	        end
	   	        self.weaponnum = self.weaponnum+1;
	   	        local weaponname = "Weapon"..self.weaponnum;	--控件名字，用于删除时
	   	        self.weaponlist[weaponname]={};
	   	        self.weaponlist[weaponname].wtype=self.weaponbutton.wtype;	--武器的种类

	   	        
	   	        self.weaponlist[weaponname].grade=1;		--武器的等级

	   	        
	   	        local wtype=self.weaponlist[weaponname].wtype;
	   	        local wgrade=self.weaponlist[weaponname].grade;
	   	        
	   	        self.weaponlist[weaponname].airflag=TD_WeaponIndex[wtype][wgrade].airflag;
	   	        self.weaponlist[weaponname].groundflag=TD_WeaponIndex[wtype][wgrade].groundflag;
	   	        
	   	        self.weaponlist[weaponname].adamage=TD_WeaponIndex[wtype][wgrade].adamage;
	   	        self.weaponlist[weaponname].addcoea=0;
	   	        self.weaponlist[weaponname].bdamage=TD_WeaponIndex[wtype][wgrade].bdamage;
	   	        self.weaponlist[weaponname].addcoeb=0;
	   	        self.weaponlist[weaponname].damagetoa=TD_WeaponIndex[wtype][wgrade].damagetoa;
	   	        self.weaponlist[weaponname].addcoetoa=0;
	   	        self.weaponlist[weaponname].damagetob=TD_WeaponIndex[wtype][wgrade].damagetob;
	   	        self.weaponlist[weaponname].addcoetob=0;
	   	        self.weaponlist[weaponname].freeze=TD_WeaponIndex[wtype][wgrade].freeze;
	   	        self.weaponlist[weaponname].addcoefreeze=0;
	   	        self.weaponlist[weaponname].multihit=TD_WeaponIndex[wtype][wgrade].multihit;
	   	        self.weaponlist[weaponname].addcoemultihit=0;
	   	        self.weaponlist[weaponname].multidamage=TD_WeaponIndex[wtype][wgrade].multidamage;
	   	        self.weaponlist[weaponname].addcoemultidamage=0;
	   	        self.weaponlist[weaponname].cooltime=TD_WeaponIndex[wtype][wgrade].cooltime;
	   	        self.weaponlist[weaponname].addcoecool=0;

	   	        
	   	        self.weaponlist[weaponname].bpos={
	   	        								{bx=selectblock[1].nx,by=selectblock[1].ny},
                    							{bx=selectblock[2].nx,by=selectblock[2].ny},
                    							{bx=selectblock[3].nx,by=selectblock[3].ny},
                    							{bx=selectblock[4].nx,by=selectblock[4].ny}
                    							};		--武器所在的位置
               	self.weaponlist[weaponname].coolcounter=0;	--计数来统计时间，每一帧递减，0为最小，可以射击范围内的目标     							
              	self.weaponlist[weaponname].target=nil;

              	self.weaponlist[weaponname].improveflag=false;
	   	        self.weaponlist[weaponname].improvecounter=0;
	   	       --武器的方向

              	self.weaponlist[weaponname].direction={};
				self.weaponlist[weaponname].direction.detx=math.random(3)-2;
				self.weaponlist[weaponname].direction.dety=math.random(3)-2;
				--关于武器的动画控制

				self.weaponlist[weaponname].directindex=0;
				self.weaponlist[weaponname].frameindex=0;
				self.weaponlist[weaponname].acframeindex = 0;
				self.weaponlist[weaponname].framecounter=0; 
				self.weaponlist[weaponname].attackflag = false;	        
	        
	   	        local posx = (selectblock[1].nx-1)*self.mapblocksize+self.maporigin.x;
			   	local posy = (selectblock[1].ny-1)*self.mapblocksize+self.maporigin.y;
			   	
			   	self.Gold=self.Gold-TD_WeaponIndex[wtype][wgrade].createcost;
			   	self:SetWeaponBtn();
			   	DlgApi.SetItemText(self.this,"Edit_Gold",tostring(self.Gold));--修改了金钱

		   	
		   		DlgApi.ShowItem(self.this, TD_PicWDrag..self.weaponbutton.wtype, false);	--提示方块取消
		   		
			   	DlgApi.CreateControl(self.this,self.weaponbutton.bname,weaponname);--生成控件
			   	
			    DlgApi.SetImageFile(self.this, weaponname, TD_WeaponIndex[wtype][wgrade].picname, TD_WeaponIndex[wtype][wgrade].frames);
			    	
				--设置控件的方向

				self.weaponlist[weaponname].directindex=self:WeaponDirection(self.weaponlist[weaponname]);
			    DlgApi.SelectImageFrame(self.this, weaponname, self.weaponlist[weaponname].directindex);

			   	self:SetItemPos( weaponname, posx, posy);

			   	DlgApi.ShowItem(self.this, weaponname, true);
			   	
		   		--修改blockmap
			   	for i=1,4 do
			   	   self.blockmap[selectblock[i].ny][selectblock[i].nx] =1;
			   	   self.weaponmap[selectblock[i].ny][selectblock[i].nx] =weaponname;
			   	end
			   	
			   	--修改部分monster的路径，若武器放在了monster的下一个目标点的格子上
			   			--注：表示物体已经走过了当前格的中心，走向下一个格的中心，这个时候需要调整det
			   	for mname,m in pairs(self.monsterlist) do
					if m.flyflag==false and self.blockmap[m.nextbpos.y][m.nextbpos.x]==1 then	--表示去路被堵	
			   			local minmap,nextbx,nextby=GameAI.GetNextPos(m.bpos.x, m.bpos.y, m.path-1);
			   			
                		if	minmap==true	then		--没有走出迷宫
                			m.nextbpos.x=nextbx;
                			m.nextbpos.y=nextby;
                			
                			m.nextpixcentre.x=(m.nextbpos.x-1+0.5)*self.mapblocksize;
            				m.nextpixcentre.y=(m.nextbpos.y-1+0.5)*self.mapblocksize;
                			
                  		end
			   			
			   		end
	   	    	end
	   	    	
		   	    	--存入它的攻击范围内的列表
	   	    	local ret = TD_WeaponIndex[wtype][wgrade].wradius;
	   	    	if ret~=nil then
    	   	    	for i=selectblock[1].ny-ret,selectblock[4].ny+ret do
    	   	    	    for j = selectblock[1].nx-ret,selectblock[4].nx+ret do
    	   	    			if i>=1 and i<=self.mapheight and j>=1 and j<=self.mapwidth then
    	   	    				self.winblock[i][j][weaponname]=true;
    	   	    			end
    	   	    	    end
    	   	    	end
	   	    	end
		   	    	--剔除不该在圆圈里的块
               	local retcircle = TD_WeaponIndex[wtype][wgrade].rangecircle;
               	if retcircle~=nil then
                   	for n=1,#TD_CircleRange[retcircle] do
                   	    local i = TD_CircleRange[retcircle][n][2]+selectblock[1].ny;
                   	    local j = TD_CircleRange[retcircle][n][1]+selectblock[1].nx;
               			if i>=1 and i<=self.mapheight and j>=1 and j<=self.mapwidth then
               				self.winblock[i][j][weaponname]=nil;
               			end
            		end            	
               	end	 		   	    	

	   	    	--处理塔生成时的属性加强问题-在此处需要判断 生成的塔是否是加强塔
				--自身对周围的影响
				self:AssistTChange(weaponname,wtype,wgrade,false);
	   	    	--检查周围是否有塔能给自己辅助或更强辅助wtype
				self:OthertToAssist(weaponname,wtype,wgrade)

    			self.weaponbutton.state = false;
    			DlgApi.SelectImageFrame(self.this,"Pic_WeaponBK"..self.weaponbutton.wtype,0);
    			self:WeaponSelect(weaponname);	  
    			
		        self:SetWeaponInfo(weaponname,true);  
		      	
		      	    		
	   	    end	   		   	    
	   	    --寻径成功    	
    	end
		---在范围内
    end
   ----生成武器
    return true;
end

--====================
-----------------------LButtonDown的相关函数

--选择了界面上生成的武器

function Win_TestTowergame:WeaponSelect(ItemName)
	if(self.weaponbutton.state==true) then
	    
	    --self.weaponbutton.state = 0;
	    self.weaponselect.state=false;
	    DlgApi.SelectImageFrame(self.this,"Pic_WeaponBK"..self.weaponbutton.wtype,0);
	    
	    		--disable 升级和卖出键
    	DlgApi.EnableItem(self.this,TD_ItemSell,false);
		DlgApi.EnableItem(self.this,TD_ItemImprove,false);
	else
		--取消原来的圈圈

    	for i=1,#TD_WRangeCircle do
    	    DlgApi.ShowItem(self.this,TD_WeaponRangePre..i,false);
    	end		
		
	    self.weaponbutton.state = false;
	    DlgApi.SelectImageFrame(self.this,"Pic_WeaponBK"..self.weaponbutton.wtype,0);
		self.weaponselect.state = true;
		self.weaponselect.serial=ItemName;
		
		--需要在这个武器边上来设置一个圆圈圈
		if TD_WeaponIndex[self.weaponlist[ItemName].wtype][self.weaponlist[ItemName].grade].rangecircle~=nil then
		    
			--local ret= DlgApi.GetItemRect(self.this, TD_WeaponRangePre..TD_WeaponIndex[self.weaponlist[ItemName].wtype][self.weaponlist[ItemName].grade].rangecircle);
			local posx=self.weaponlist[ItemName].bpos[1].bx*self.mapblocksize+self.maporigin.x-TD_WRangeCircle[TD_WeaponIndex[self.weaponlist[ItemName].wtype][self.weaponlist[ItemName].grade].rangecircle].w+1;
			local posy=self.weaponlist[ItemName].bpos[1].by*self.mapblocksize+self.maporigin.y-TD_WRangeCircle[TD_WeaponIndex[self.weaponlist[ItemName].wtype][self.weaponlist[ItemName].grade].rangecircle].h+1;
			self:SetItemPos( TD_WeaponRangePre..TD_WeaponIndex[self.weaponlist[ItemName].wtype][self.weaponlist[ItemName].grade].rangecircle, posx, posy);
			DlgApi.ShowItem(self.this,TD_WeaponRangePre..TD_WeaponIndex[self.weaponlist[ItemName].wtype][self.weaponlist[ItemName].grade].rangecircle,true);	
		end	
						
				--激活 卖出控件
		DlgApi.EnableItem(self.this,TD_ItemSell,true);

		local wtype = self.weaponlist[ItemName].wtype;
		local wgrade = self.weaponlist[ItemName].grade;
	
		if(  wgrade < #TD_WeaponIndex[wtype] and self.Gold >= TD_WeaponIndex[wtype][wgrade].improvecost and self.weaponlist[ItemName].improveflag~=true) then
	    	--激活升级的item
	    	DlgApi.EnableItem(self.this,TD_ItemImprove,true);
	    else
	        --diable升级的item
	    	DlgApi.EnableItem(self.this,TD_ItemImprove,false);  
		end
	end
end
---------------------------------------------------------------------------


--由鼠标的位置来搜索相应的方格
function Win_TestTowergame:FoundBlock(selectblock,blocknx,blockny,mapx,mapy)
    
    	        	   --block的中心坐标,相对地图
    		local blockcenterx = (blocknx-1)*self.mapblocksize+self.mapblockhsize;
    		local blockcentery = (blockny-1)*self.mapblocksize+self.mapblockhsize;
    		
    								--确定选择哪四个格子作为武器

    		if(mapx<blockcenterx and mapy<blockcentery) then
				selectblock[1].nx = blocknx-1;selectblock[1].ny = blockny-1;
				selectblock[2].nx =blocknx ;selectblock[2].ny = blockny-1;
				selectblock[3].nx = blocknx-1;selectblock[3].ny =blockny ;
				selectblock[4].nx = blocknx;selectblock[4].ny = blockny;
			
			elseif(mapx>=blockcenterx and mapy<blockcentery) then
			    selectblock[1].nx = blocknx;selectblock[1].ny = blockny-1;
				selectblock[2].nx =blocknx+1 ;selectblock[2].ny = blockny-1;
				selectblock[3].nx = blocknx;selectblock[3].ny =blockny ;
				selectblock[4].nx = blocknx+1;selectblock[4].ny = blockny;
			
			elseif(mapx<blockcenterx and mapy>=blockcentery) then
			    selectblock[1].nx = blocknx-1;selectblock[1].ny = blockny;
				selectblock[2].nx =blocknx ;selectblock[2].ny = blockny;
				selectblock[3].nx = blocknx-1;selectblock[3].ny =blockny+1 ;
				selectblock[4].nx = blocknx;selectblock[4].ny = blockny+1;
			
			elseif(mapx>=blockcenterx and mapy>=blockcentery) then
			    selectblock[1].nx = blocknx;selectblock[1].ny = blockny;
				selectblock[2].nx =blocknx+1 ;selectblock[2].ny = blockny;
				selectblock[3].nx = blocknx;selectblock[3].ny =blockny+1 ;
				selectblock[4].nx = blocknx+1;selectblock[4].ny = blockny+1;
			end
		
end
------------


------------------------------每一帧的动画播放-----------------------------------------------
function Win_TestTowergame:Tick(dwDeltaTime)

	--LogPrint(tostring(dwDeltaTime).."\n");

	if(self.startpause~=TD_PlayingState) then
	    return true;
	end

	----=================================================
	--关于怪物的移动,前一帧算位移，后一帧挪动

	self:UpdateMonster(dwDeltaTime);

	
	------关于子弹------------
	self:UpdateBullet(dwDeltaTime)
	----关于武器-----
	self:UpdateWeapon(dwDeltaTime);
	
	--生成怪物--------
	self:CreateMonster(dwDeltaTime);
	------------------

	-----播放动画效果---------
	
	self:UpdateAllAction(dwDeltaTime);
	
	return true;
end


----=============
function Win_TestTowergame:UpdateBullet(dwDeltaTime)
	--bullet（在武器中生成时暂不显示，有初始化运动后显示）

	for bname,b in pairs(self.bulletlist) do
	    local targetx=0;
	    local targety=0;
	    if b.target==nil then	--这种方法把失去目标的子弹给消除掉了，而另一种方法则是把目标消失点的位置座位消失位置
	        --把目标消失点作为目标的坐标

	        targetx=b.targetpos.x;
	        targety=b.targetpos.y;
	    else
	        targetx=self.monsterlist[b.target].pixpos.x;
			targety=self.monsterlist[b.target].pixpos.y;
	    end	        
	    
	    local bmdistance=math.sqrt((b.pos.x-targetx)^2+(b.pos.y-targety)^2);
	    local detx=targetx-b.pos.x;
	    local dety=targety-b.pos.y;
	    
	    local cosa=detx/bmdistance;
	    local sina=dety/bmdistance;
	    local newposx=b.pos.x+b.speed*cosa*dwDeltaTime;
	    local newposy=b.pos.y+b.speed*sina*dwDeltaTime;
	    
	    local tempx=detx*(targetx-newposx);
	    local tempy=dety*(targety-newposy);
	    
	    if (tempx>0 and tempy>0)or(tempx>0 and tempy==0)or(tempx==0 or tempy>0) then
	        b.pos.x=newposx;
	        b.pos.y=newposy;
			
			if TD_BulletIndex[b.btype].picname~=nil then
    			--local ret=  DlgApi.GetItemRect(self.this, bname);	
    	        local posx=math.ceil(b.pos.x+self.maporigin.x-TD_BulletIndex[b.btype].w);
    	        local posy=math.ceil(b.pos.y+self.maporigin.y-TD_BulletIndex[b.btype].h);
    	        self:SetItemPos(bname,posx,posy);		--注意是否要通过图标大小调整显示位置
			end
	    
	    else			--表示子弹已经到达进攻目标,判断是否要群攻

	        local targetlist={};
	        
	        if type(b.bradius)=="number" then
	            
	            if b.target~=nil then
	                targetlist[b.target]=true;
	            end
	            
	            for mname,m in pairs(self.monsterlist) do
	                local detx=m.pixpos.x-b.pos.x;
	                local dety=m.pixpos.y-b.pos.y;
	                if detx^2+dety^2 < b.bradius^2 then		--m在范围内
	                    targetlist[mname]=true;
	                end
	            end 
	        
	        else
	            --单独攻击
	            if b.target~=nil then
	                targetlist[b.target]=true;
	            end	        
	        end
	        
	        --进行动作特效的播放--即子弹爆炸的效果
	        self:BulletExplode(b,targetx,targety,dwDeltaTime);
	        
	        for mname,state in pairs(targetlist) do		
        
       			--速度减慢判断
       			if b.freeze~=0 then			--有减速作用


					Win_TestTowergame:BulletFreeze(mname,b,dwDeltaTime);
       			end
       			
       			--护甲受到了攻击

       			if b.damagetoa>0 and self.monsterlist[mname].adefence<1 then
       			    Win_TestTowergame:BulletToAD(mname,b,dwDeltaTime);
       			end
       			if b.damagetob>0 and self.monsterlist[mname].bdefence<1 then
       				Win_TestTowergame:BulletToBD(mname,b,dwDeltaTime);
       			end
       			
		        --生命伤害值

    	        self.monsterlist[mname].life=self.monsterlist[mname].life-(1-self.monsterlist[mname].adefence)*b.adamage-(1-self.monsterlist[mname].bdefence)*b.bdamage;
    	        
    	        --是否杀死怪物       			
       			if self.monsterlist[mname].life<=0 then	--销毁怪物
       				self:MonsterDeath(mname);
    	            	 	            
    	        else
    	            DlgApi.SetProgress(self.this,self.monsterlist[mname].lifeprog ,self.monsterlist[mname].life/self.monsterlist[mname].mindex.life);
       			end
   	        
	        end
	        
	        --消除子弹
	        if TD_BulletIndex[b.btype].picname~=nil then
	            DlgApi.DeleteControl(self.this,bname);
	        end	
	        
	        self.bulletlist[bname]=nil;
	    end
	    
	end
end

--======================
function Win_TestTowergame:BulletFreeze(mname,b,dwDeltaTime)
   if self.monsterlist[mname].freezeflag==false then	--怪物未受减速影响

       self.monsterlist[mname].freezeflag=true;
       self.monsterlist[mname].freezecounter=0;
       
       --冰冻的规则：
       if b.freeze<TD_FreezeMax then
           self.monsterlist[mname].speed=self.monsterlist[mname].speed*(1-b.freeze);
       elseif b.freeze>1 and b.multiadd>0 then
          	 local rate=math.random();
          	 if rate< ( b.multiadd*TD_FreezeStopStep+TD_FreezeStopMin ) then		--b.multiadd,暴击增加的倍数。转换为冷冻概率
			 self.monsterlist[mname].speed=0;
          	 else
          	     self.monsterlist[mname].speed=self.monsterlist[mname].speed*(1-TD_FreezeMax);
          	 end
       else
           self.monsterlist[mname].speed=self.monsterlist[mname].speed*(1-TD_FreezeMax);
       end
       
       --增加减速的特效效果
       if TD_BeAtkEfeIndex.Freeze~=nil then
            self.beatkefnum=self.beatkefnum+1;
            local btn = "BeAtkEfe"..self.beatkefnum;
            self.monsterlist[mname].freezeeffect=btn;
            DlgApi.CreateControl(self.this,TD_BeAtkEfePre.."Freeze",btn);
            self.beatkeflist[btn]={};
            self.beatkeflist[btn].btype="Freeze";
            self.beatkeflist[btn].frameindex=0;
            self.beatkeflist[btn].framecounter=0-dwDeltaTime;

			DlgApi.SetImageFile(self.this, btn, TD_BeAtkEfeIndex[self.beatkeflist[btn].btype].picname, TD_BeAtkEfeIndex[self.beatkeflist[btn].btype].franum);
        	DlgApi.SelectImageFrame(self.this, btn,self.beatkeflist[btn].frameindex );
    
    		--local ret= DlgApi.GetItemRect(self.this, mname);
    		local posx=math.ceil(self.monsterlist[mname].pixpos.x+self.maporigin.x-TD_BeAtkEfeIndex.Freeze.w);
    		local posy=math.ceil(self.monsterlist[mname].pixpos.y+self.maporigin.y-TD_BeAtkEfeIndex.Freeze.h);                	    	
           	self:SetItemPos(btn,posx,posy);
           	DlgApi.ShowItem(self.this,btn,true);
       end
       ---减速

       
   else	--怪物已经受减速影响

       local newspeed = nil; 	--更强减速

       --冰冻的规则：
       if b.freeze<TD_FreezeMax then
           newspeed=self.monsterlist[mname].mindex.speed*(1-b.freeze);
       elseif b.freeze>1 and b.multiadd>0 then
          	 local rate=math.random();
          	 if rate< ( b.multiadd*TD_FreezeStopStep+TD_FreezeStopMin ) then		--b.multiadd,暴击增加的倍数。转换为冷冻概率
			 newspeed=0;
          	 else
          	     newspeed=self.monsterlist[mname].mindex.speed*(1-TD_FreezeMax);
          	 end
       else
           newspeed=self.monsterlist[mname].mindex.speed*(1-TD_FreezeMax);
       end       			        
       
       if self.monsterlist[mname].speed>=newspeed then
           self.monsterlist[mname].speed=newspeed;
           self.monsterlist[mname].freezecounter=0;
       end
   end
end

function Win_TestTowergame:BulletToAD(mname,b,dwDeltaTime)
   if self.monsterlist[mname].toaflag==false then	--怪物未受减速影响

       self.monsterlist[mname].toaflag=true;
       self.monsterlist[mname].toacounter=0;
       
       --a类减护甲规则：

       local adefence = self.monsterlist[mname].adefence-b.damagetoa;
       if adefence>-1 then
           self.monsterlist[mname].adefence = adefence;
       else
           self.monsterlist[mname].adefence = -1;
       end
      
       --增加减a护甲的特效效果

       if TD_BeAtkEfeIndex.AttackToAD~=nil then
            self.beatkefnum=self.beatkefnum+1;
            local btn = "BeAtkEfe"..self.beatkefnum;
            self.monsterlist[mname].toaeffect=btn;
            DlgApi.CreateControl(self.this,TD_BeAtkEfePre.."ToAD",btn);
            self.beatkeflist[btn]={};
            self.beatkeflist[btn].btype="AttackToAD";
            self.beatkeflist[btn].frameindex=0;
            self.beatkeflist[btn].framecounter=0-dwDeltaTime;

			DlgApi.SetImageFile(self.this, btn, TD_BeAtkEfeIndex[self.beatkeflist[btn].btype].picname, TD_BeAtkEfeIndex[self.beatkeflist[btn].btype].franum);
        	DlgApi.SelectImageFrame(self.this, btn,self.beatkeflist[btn].frameindex );
    
    		--local ret= DlgApi.GetItemRect(self.this, mname);
    		local posx=math.ceil(self.monsterlist[mname].pixpos.x+self.maporigin.x-TD_BeAtkEfeIndex.AttackToAD.w);
    		local posy=math.ceil(self.monsterlist[mname].pixpos.y+self.maporigin.y-TD_BeAtkEfeIndex.AttackToAD.h);                	    	
           	self:SetItemPos(btn,posx,posy);
           	DlgApi.ShowItem(self.this,btn,true);
       end
       ---减速

       
   else	--怪物已经受减a护甲影响
       
       --减a护甲的规则：
       local adefence = self.monsterlist[mname].mindex.adefence-b.damagetoa;
             
       if adefence<=self.monsterlist[mname].adefence then
           if adefence<=-1 then
               self.monsterlist[mname].adefence=-1;
           else
               self.monsterlist[mname].adefence=adefence;
           end
           
           self.monsterlist[mname].toacounter=0;
            
       end
   end
end

function Win_TestTowergame:BulletToBD(mname,b,dwDeltaTime)
   if self.monsterlist[mname].tobflag==false then	--怪物未受减速影响

       self.monsterlist[mname].tobflag=true;
       self.monsterlist[mname].tobcounter=0;
       
       --b类减护甲规则：

       local bdefence = self.monsterlist[mname].bdefence-b.damagetob;
       if bdefence>-1 then
           self.monsterlist[mname].bdefence = bdefence;
       else
           self.monsterlist[mname].bdefence = -1;
       end
      
       --增加减b护甲的特效效果

       if TD_BeAtkEfeIndex.AttackToBD~=nil then
            self.beatkefnum=self.beatkefnum+1;
            local btn = "BeAtkEfe"..self.beatkefnum;
            self.monsterlist[mname].tobeffect=btn;
            DlgApi.CreateControl(self.this,TD_BeAtkEfePre.."ToBD",btn);
            self.beatkeflist[btn]={};
            self.beatkeflist[btn].btype="AttackToBD";
            self.beatkeflist[btn].frameindex=0;
            self.beatkeflist[btn].framecounter=0-dwDeltaTime;

			DlgApi.SetImageFile(self.this, btn, TD_BeAtkEfeIndex[self.beatkeflist[btn].btype].picname, TD_BeAtkEfeIndex[self.beatkeflist[btn].btype].franum);
        	DlgApi.SelectImageFrame(self.this, btn,self.beatkeflist[btn].frameindex );
    
    		--local ret= DlgApi.GetItemRect(self.this, mname);
    		local posx=math.ceil(self.monsterlist[mname].pixpos.x+self.maporigin.x-TD_BeAtkEfeIndex.AttackToBD.w);
    		local posy=math.ceil(self.monsterlist[mname].pixpos.y+self.maporigin.y-TD_BeAtkEfeIndex.AttackToBD.h);                	    	
           	self:SetItemPos(btn,posx,posy);
           	DlgApi.ShowItem(self.this,btn,true);
       end
       ---减速

       
   else	--怪物已经受减a护甲影响
       
       --减a护甲的规则：
       local bdefence = self.monsterlist[mname].mindex.bdefence-b.damagetob;
             
       if bdefence<=self.monsterlist[mname].bdefence then
           if bdefence<=-1 then
               self.monsterlist[mname].bdefence=-1;
           else
               self.monsterlist[mname].bdefence=bdefence;
           end
           
           self.monsterlist[mname].tobcounter=0;
            
       end
   end
end

--========================
function Win_TestTowergame:BulletExplode(b,targetx,targety,dwDeltaTime)
	--增加子弹爆炸的特效


	if type(TD_BulletIndex[b.btype].explodetype)=="number" and TD_BulletIndex[b.btype].explodetype <= #TD_RTEfeIndex  then
        self.rtefnum=self.rtefnum+1;
        local btn="RTeffect"..self.rtefnum;	    

        local btnmodel=TD_RTEfePre..TD_BulletIndex[b.btype].explodetype;
        DlgApi.CreateControl(self.this,btnmodel,btn);
        self.rteflist[btn]={};
        self.rteflist[btn].dtype=TD_BulletIndex[b.btype].explodetype;
        self.rteflist[btn].frameindex=0;
        self.rteflist[btn].framecounter=0;
    	DlgApi.SetImageFile(self.this,btn,TD_RTEfeIndex[self.rteflist[btn].dtype].picname, TD_RTEfeIndex[self.rteflist[btn].dtype].franum);
    	DlgApi.SelectImageFrame(self.this, btn,self.rteflist[btn].frameindex );

		--local ret= DlgApi.GetItemRect(self.this,btn);
		local posx=math.ceil(targetx+self.maporigin.x-TD_RTEfeIndex[TD_BulletIndex[b.btype].explodetype].w);
		local posy=math.ceil(targety+self.maporigin.y-TD_RTEfeIndex[TD_BulletIndex[b.btype].explodetype].h);                	    	
       	self:SetItemPos(btn,posx,posy);
       	DlgApi.ShowItem(self.this,btn,true);
       	
		if self.soundflag==true and TD_RTEfeIndex[TD_BulletIndex[b.btype].explodetype].sound then
		   
		    GameAI.Play2DSound(TD_RTEfeIndex[TD_BulletIndex[b.btype].explodetype].sound);
		end	    
	end

end
--=======================
function Win_TestTowergame:MonsterDeath(mname)
    
    	self.Gold=self.Gold+self.monsterlist[mname].rewardgold;
    	self:SetWeaponBtn();
    	self.Score=self.Score+self.monsterlist[mname].rewardscore*self.mscorecoe;
    	
    	DlgApi.SetItemText( self.this,"Edit_Score",tostring(math.ceil(self.Score)) );
    	DlgApi.SetItemText(self.this,"Edit_Gold",tostring(self.Gold));

        DlgApi.DeleteControl(self.this,mname);
			--删除血条

		self.mlifelist[self.monsterlist[mname].lifeprog]=nil;
		DlgApi.DeleteControl(self.this,self.monsterlist[mname].lifeprog);   
			--删除跟随的受攻击特效
        if self.monsterlist[mname].freezeeffect~=nil then
            DlgApi.DeleteControl(self.this,self.monsterlist[mname].freezeeffect);
        	self.beatkeflist[self.monsterlist[mname].freezeeffect]=nil;
        	self.monsterlist[mname].freezeeffect=nil;
        end
        if self.monsterlist[mname].toaeffect~=nil then
            DlgApi.DeleteControl(self.this,self.monsterlist[mname].toaeffect);
        	self.beatkeflist[self.monsterlist[mname].toaeffect]=nil;
        	self.monsterlist[mname].toaeffect=nil;
        end        
        if self.monsterlist[mname].tobeffect~=nil then
            DlgApi.DeleteControl(self.this,self.monsterlist[mname].tobeffect);
        	self.beatkeflist[self.monsterlist[mname].tobeffect]=nil;
        	self.monsterlist[mname].tobeffect=nil;
        end

        	--增加死亡特效
        self.rtefnum=self.rtefnum+1;
        local btn="RTeffect"..self.rtefnum;

        local btnmodel=TD_RTEfePre..self.monsterlist[mname].mindex.deathtype;
        DlgApi.CreateControl(self.this,btnmodel,btn);
        self.rteflist[btn]={};
        self.rteflist[btn].dtype=self.monsterlist[mname].mindex.deathtype;
        self.rteflist[btn].frameindex=0;
        self.rteflist[btn].framecounter=0;
    	DlgApi.SetImageFile(self.this,btn,TD_RTEfeIndex[self.rteflist[btn].dtype].picname, TD_RTEfeIndex[self.rteflist[btn].dtype].franum);
    	DlgApi.SelectImageFrame(self.this, btn,self.rteflist[btn].frameindex );

		--local ret= DlgApi.GetItemRect(self.this,btn);
		local posx=math.ceil(self.monsterlist[mname].pixpos.x+self.maporigin.x-TD_RTEfeIndex[self.monsterlist[mname].mindex.deathtype].w);
		local posy=math.ceil(self.monsterlist[mname].pixpos.y+self.maporigin.y-TD_RTEfeIndex[self.monsterlist[mname].mindex.deathtype].h);                	    	
       	self:SetItemPos(btn,posx,posy);
       	DlgApi.ShowItem(self.this,btn,true);
       	
       	if self.soundflag == true then
	       	GameAI.Play2DSound("Sfx\\TinyGame\\TD\\死亡声音.wav");
       	end	 	            
        --此时需要处理target为消失的怪物的子弹,搜索每个目标为此目标的子弹重新设置目标位置  
--  		for b2name,b2 in pairs(self.bulletlist) do	--子弹继续跟踪
--  		    if  b2.target==mname then
--  		        b2.targetpos.x=self.monsterlist[mname].pixpos.x;
--  		        b2.targetpos.y=self.monsterlist[mname].pixpos.y;
--  		        b2.target=nil;
--  		    end      			        
--  		end
  		for b2name,b2 in pairs(self.bulletlist) do	--子弹中途消失掉
  		    if  b2.target==mname then
				DlgApi.DeleteControl(self.this,b2name);
				self.bulletlist[b2name] = nil;
  		    end      			        
  		end  		
  		
        self.monsterlist[mname]=nil; 
        self.minscreen=self.minscreen-1;   
        
		--是否可以出下一拨怪, 桌面怪物清零，该拨怪物全部走出
    	local flag=true;
    	for curpath=1,#TD_MapPath[self.curgame] do   		
       		if self.mnumcounter[curpath]~=self.numpergroup[curpath] then
       		    flag=false;
       		end
    	end

		if self.minscreen==0  and flag==true then
		    if self.level<#TD_PerPassData[self.curgame] then
                DlgApi.EnableItem(self.this,"Btn_NextWave",true);
                DlgApi.ShowItem(self.this,"Lab_MonsterPic",false);
                self.curmonster={};
                for i=1,4 do
                    DlgApi.ShowItem(self.this,"Pic_MShow"..i,false);
                end
		    else
		        --游戏顺利过关
		        
            	--设置任务开始
            	if TD_TaskIndex and TD_TaskIndex[self.diff] and TD_TaskIndex[self.diff][self.curgame] then
            	    GameAI.TaskGiveup(TD_TaskIndex[self.diff][self.curgame]);
            	    GameAI.TaskReq(TD_TaskIndex[self.diff][self.curgame]);
            	end 		        
		        
		        self.Score = math.ceil(self.Score);
		        Win_TowerDefenseWin:OnShowDialog(self.curgame,self.Score,self.diff);
		        
		       self.startpause=TD_PauseState;
		        
		        
         	   --清除游戏的状态

         	   self:Clear();
		    end
        end        
        
end
--======================
function Win_TestTowergame:UpdateWeapon(dwDeltaTime)
	--武器控制--计数：攻击蓄劲/升级等待、搜寻目标，改变方向，发出子弹（或生成特效）
	for wname,w in pairs(self.weaponlist) do
	    if w.improveflag==true then
	    	w.improvecounter=w.improvecounter+dwDeltaTime;
	    	--判断是否到了升级的时间

	    	if w.improvecounter>TD_WeaponIndex[w.wtype][w.grade-1].improvetime then		--从1到2的耗费时间存在了1里

	    	
	    		--增加升级完毕的特效

        	    self.rtefnum=self.rtefnum+1;
                local btn="RTeffect"..self.rtefnum;	    
                local btnmodel=TD_RTEfePre..TD_WeaponIndex[w.wtype][w.grade-1].improvepic;
                DlgApi.CreateControl(self.this,btnmodel,btn);
                self.rteflist[btn]={};
                self.rteflist[btn].dtype=TD_WeaponIndex[w.wtype][w.grade-1].improvepic;
                self.rteflist[btn].frameindex=0;
                self.rteflist[btn].framecounter=0;
            	DlgApi.SetImageFile(self.this,btn,TD_RTEfeIndex[self.rteflist[btn].dtype].picname, TD_RTEfeIndex[self.rteflist[btn].dtype].franum);
            	DlgApi.SelectImageFrame(self.this, btn,self.rteflist[btn].frameindex );
        
        		--local ret= DlgApi.GetItemRect(self.this,btn);
        		--local wret = DlgApi.GetItemRect(self.this,wname);
        		local posx=math.ceil((w.bpos[1].bx-1)*TD_MapBlockSize+self.maporigin.x);
        		local posy=math.ceil((w.bpos[1].by+1)*TD_MapBlockSize-TD_RTEfeIndex[TD_WeaponIndex[w.wtype][w.grade-1].improvepic].h-TD_RTEfeIndex[TD_WeaponIndex[w.wtype][w.grade-1].improvepic].h+self.maporigin.y);                	    	
               	self:SetItemPos(btn,posx,posy);
               	DlgApi.ShowItem(self.this,btn,true);
               	
               	if self.soundflag == true then
	               	GameAI.Play2DSound("Sfx\\TinyGame\\TD\\人物升级.wav");
               	end
               	           	
				--换武器可用图片--设置升级后的图片
    	    	DlgApi.SetImageFile(self.this, wname, TD_WeaponIndex[w.wtype][w.grade].picname, TD_WeaponIndex[w.wtype][w.grade].frames);
    			--不需要改变控件的方向
    	    	DlgApi.SelectImageFrame(self.this, wname, w.directindex);
				--DlgApi.EnableItem(self.this,wname,true);
					    	    
	    	    --更改更新进度条

	    	    DlgApi.DeleteControl(self.this,w.improvepro);
	    	    self.wimprovelist[w.improvepro]=nil;
	    	    w.improvepro=nil;
				--更换圈圈
				if self.weaponselect.state==true and wname==self.weaponselect.serial then
            	    DlgApi.ShowItem(self.this,TD_WeaponRangePre..TD_WeaponIndex[w.wtype][w.grade-1].rangecircle,false);
            		local posx=w.bpos[1].bx*self.mapblocksize+self.maporigin.x-TD_WRangeCircle[TD_WeaponIndex[w.wtype][w.grade].rangecircle].w+1;
            		local posy=w.bpos[1].by*self.mapblocksize+self.maporigin.y-TD_WRangeCircle[TD_WeaponIndex[w.wtype][w.grade].rangecircle].h+1;
            		self:SetItemPos(TD_WeaponRangePre..TD_WeaponIndex[w.wtype][w.grade].rangecircle, posx, posy);        	    
            	    DlgApi.ShowItem(self.this,TD_WeaponRangePre..TD_WeaponIndex[w.wtype][w.grade].rangecircle,true);	    	    
				end
	    	    --w.coolcounter= w.cooltime;	--完成升级后能在下一个Tick立刻攻击，考虑cooltime的修改，升级后快了没事儿
	    	    w.improveflag=false;
	    	    w.improvecounter=0;
	    	    if wname==self.weaponselect.serial and w.grade<#TD_WeaponIndex[w.wtype] then
	    	    	DlgApi.EnableItem(self.this,"Btn_Wimprove",true);
	    	    end	    	    

				--需要判断升级的塔是不是属性加强塔
				self:AssistTChange(wname,w.wtype,w.grade,false);
	    	    
	    	    w.coolcounter= w.cooltime-TD_RTEfeIndex[TD_WeaponIndex[w.wtype][w.grade-1].improvepic].franum*TD_RTEfeIndex[TD_WeaponIndex[w.wtype][w.grade-1].improvepic].timeperfra;	--完成升级后能在立刻攻击，空出播放特效的时间

	    	else
	    	    	    	--设置更新进度条

    			DlgApi.SetProgress(self.this,w.improvepro, w.improvecounter/TD_WeaponIndex[w.wtype][w.grade-1].improvetime);    	    	    
	    	end
	    else				--没有生级
	    	local wposx=w.bpos[1].bx*self.mapblocksize;
	    	local wposy=w.bpos[1].by*self.mapblocksize;
	    	
	    	if( TD_WeaponIndex[w.wtype][w.grade].bullettype~=nil and w.attackflag == false) then 			--该武器可以开火发子弹
	            w.coolcounter=w.coolcounter+dwDeltaTime;
      	        if w.coolcounter>=w.cooltime then
       	            w.coolcounter=w.cooltime; 

    	       	    --搜索自己的目标，并且在搜索到目标后更新方向

    	       	    if w.target==nil then
    	       	        w.fireflag = true;
    	       	    else	
    	       	        					--已经确定该目标的位置
    	       	    	if self.monsterlist[w.target]==nil then		--此时怪物已经消失了

    	       	    	    w.target=nil;
    	       	    	    w.fireflag = true;	       	    	
    	       	    	else
    	       	    	    --目标所在的block中存了该武器，就表示还在范围内

    	       	    	    local bpos=self.monsterlist[w.target].bpos
    	           			if self.winblock[bpos.y][bpos.x][wname]~=true then--TD_WeaponIndex[w.wtype][w.grade].wradius^2 then
    	           			    w.target=nil;
    	           			    w.fireflag = true;
    	           			end
    	       	    	end
    	       	        
    	       	    end
       	                       

    	       	    
--    	       	    if w.target==nil then		--如果没有目标，需要搜索目标，确定对飞行与对地
--    	       	    	
--	 	       	    	local wmdistance = TD_WeaponIndex[w.wtype][w.grade].wradius^2;
--    	       	    	for mname,m in pairs(self.monsterlist) do
--   
--     	    				if (m.flyflag==true and TD_WeaponIndex[w.wtype][w.grade].airflag==true) or (m.flyflag==false and TD_WeaponIndex[w.wtype][w.grade].groundflag==true) then	    				
--        	       	    	    local distance=(wposx-m.pixpos.x)^2+(wposy-m.pixpos.y)^2;
--	     	       	    	    if distance<wmdistance  then	--monster更近
--      	       	    	            w.target=mname;
--      	       	    	            break;
--      	       	    	    	end
--     	    				end
--    	    				
--    	       	    	end
--    	       	    end
    	       	    
    	       	    if w.target~=nil then
    	       	        --添加确定武器的瞄准方向，旋转武器的角度

                      	w.direction={};
                      	
        				w.direction.detx=self.monsterlist[w.target].pixpos.x-wposx;
        				w.direction.dety=self.monsterlist[w.target].pixpos.y-wposy;	       	        
    	       	        
    					--设置控件的方向

    					w.directindex=self:WeaponDirection(w);
    				    --DlgApi.SelectImageFrame(self.this, wname, w.directindex);
    				    
    	       	        ----------------------------------------
    	       	            w.coolcounter=0;
    	       	            w.fireflag = nil;
    	       	            --产生进攻
    	       	            self:GenerateBullet(wname,w,dwDeltaTime);
    	       	        
    	       	    end       	    
	            end--cooltime
	    	end		--w.fireflag==true
    
	    end	--w.improveflag
	end		--for w
end
--================================
function Win_TestTowergame:GenerateBullet(wname,w,dwDeltaTime)
    	local wposx=w.bpos[1].bx*self.mapblocksize;
    	local wposy=w.bpos[1].by*self.mapblocksize;       	
       	self.bulletnum=self.bulletnum+1;
       
       	local bulletname="Bullet"..self.bulletnum;
       	self.bulletlist[bulletname]={};
       		       	        	
       	local ret=math.random();
       	local bmodel=TD_BulletPrefix;
       	local hitadd=0;
       	if ret < w.multihit then		--产生暴击
       		hitadd = w.multidamage;
       		--hitadd = math.random(TD_WeaponIndex[w.wtype][w.grade].multidamage);
       		bmodel=bmodel..TD_WeaponIndex[w.wtype][w.grade].multibullet;
       		self.bulletlist[bulletname].btype=TD_WeaponIndex[w.wtype][w.grade].multibullet;	
       		--暴击倍数数字播放
       		if hitadd<=5 and hitadd>=1 then
       		        self.rtefnum=self.rtefnum+1;
                    local btn="RTeffect"..self.rtefnum;	    
	                local btnmodel="Pic_Multinum";
                    DlgApi.CreateControl(self.this,btnmodel,btn);
                    self.rteflist[btn]={};
                    self.rteflist[btn].dtype="Mult"..(hitadd+1);
                    self.rteflist[btn].frameindex=0;
                    self.rteflist[btn].framecounter=0;
                	DlgApi.SetImageFile(self.this,btn,TD_RTEfeIndex[self.rteflist[btn].dtype].picname, TD_RTEfeIndex[self.rteflist[btn].dtype].franum);
                	DlgApi.SelectImageFrame(self.this, btn,self.rteflist[btn].frameindex );

            		local posx=math.ceil((w.bpos[1].bx-1)*self.mapblocksize+self.maporigin.x);
            		local posy=math.ceil((w.bpos[1].by-1)*self.mapblocksize+self.maporigin.y-TD_RTEfeIndex[self.rteflist[btn].dtype].h);                	    	
                   	self:SetItemPos(btn,posx,posy);
                   	DlgApi.ShowItem(self.this,btn,true);
       		end       	        		
       	else
       	    bmodel=bmodel..TD_WeaponIndex[w.wtype][w.grade].bullettype;	
       		self.bulletlist[bulletname].btype=TD_WeaponIndex[w.wtype][w.grade].bullettype;	       	        	    
       	end
       	
       	--关于动画的参数

       	if TD_BulletIndex[self.bulletlist[bulletname].btype].picname~=nil then
           	self.bulletlist[bulletname].frameindex=0;
           	self.bulletlist[bulletname].framecounter=0-dwDeltaTime;
           	DlgApi.CreateControl(self.this,bmodel,bulletname);
			DlgApi.SetImageFile(self.this, bulletname, TD_BulletIndex[self.bulletlist[bulletname].btype].picname, TD_BulletIndex[self.bulletlist[bulletname].btype].franum);
			DlgApi.SelectImageFrame(self.this, bulletname,self.bulletlist[bulletname].frameindex);
       	end
       	
		if self.soundflag == true then
			GameAI.Play2DSound("Sfx\\TinyGame\\TD\\攻击声效.wav");
		end
       	--爆炸的子弹应该根据武器旋转的位置来适当选取
       	self.bulletlist[bulletname].pos={};
       	
       	self.bulletlist[bulletname].pos.x=wposx;
       	self.bulletlist[bulletname].pos.y=wposy;
       	local direction=math.floor(w.directindex/(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum));
       	if TD_WeaponIndex[w.wtype][w.grade].direnum==8 then
				if direction==1 or direction==2 or direction==3 then    --设置效果的偏移量中心点

					self.bulletlist[bulletname].pos.x=self.bulletlist[bulletname].pos.x+self.mapblocksize;
				end
				if direction==3 or direction==4 or direction==5 then
				    self.bulletlist[bulletname].pos.y=self.bulletlist[bulletname].pos.y-self.mapblocksize;
				end
				if direction==5 or direction==6 or direction==7 then
				    self.bulletlist[bulletname].pos.x=self.bulletlist[bulletname].pos.x-self.mapblocksize;
				end
				if direction==7 or direction==0 or direction==1 then
				    self.bulletlist[bulletname].pos.y=self.bulletlist[bulletname].pos.y+self.mapblocksize;
				end	       	        	    
       	end

       	self.bulletlist[bulletname].target=w.target;
       	self.bulletlist[bulletname].speed=TD_WeaponIndex[w.wtype][w.grade].speed;
       	self.bulletlist[bulletname].targetpos={};	--当目标消失时，记录消失点的位置

       	self.bulletlist[bulletname].bradius=TD_WeaponIndex[w.wtype][w.grade].bradius;
       		       	        		       	        	
       	self.bulletlist[bulletname].adamage=w.adamage*(1+hitadd);
       	self.bulletlist[bulletname].bdamage=w.bdamage*(1+hitadd);
       	self.bulletlist[bulletname].damagetoa=w.damagetoa*(1+hitadd);
       	self.bulletlist[bulletname].damagetob=w.damagetob*(1+hitadd);
       	
       	self.bulletlist[bulletname].freeze=w.freeze;
       	self.bulletlist[bulletname].multiadd=hitadd;
       	
       	if TD_BulletIndex[self.bulletlist[bulletname].btype].picname~=nil then
           	--local ret = DlgApi.GetItemRect(self.this, bulletname);	
    		local posx=math.ceil(self.bulletlist[bulletname].pos.x+self.maporigin.x-TD_BulletIndex[self.bulletlist[bulletname].btype].w);
    		local posy=math.ceil(self.bulletlist[bulletname].pos.y+self.maporigin.y-TD_BulletIndex[self.bulletlist[bulletname].btype].h);
    		self:SetItemPos(bulletname,posx,posy);
    		DlgApi.ShowItem(self.this,bulletname,true);	
       	end
		
		----------------增加武器攻击的特效

		--当处在发招的时刻
        w.attackflag = true;
        w.acframeindex =0
        w.framecounter = 0-dwDeltaTime;
	    local direct=math.floor(w.directindex/(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum));
	    w.directindex = direct * (TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+TD_WeaponIndex[w.wtype][w.grade].franum;
	    DlgApi.SelectImageFrame(self.this, wname, w.directindex);
       
       
		
		----此后的部分是叠加图片用的，暂时attackeffect ==nil 
		if type(TD_WeaponIndex[w.wtype][w.grade].attackeffect)=="number" and TD_WeaponIndex[w.wtype][w.grade].attackeffect<=#TD_AttackEffectIndex then
			
			self.ateffectnum=self.ateffectnum+1;
			local aname="AtEffect"..self.ateffectnum;
       		self.ateffectlist[aname]={};
       		self.ateffectlist[aname].atype = TD_WeaponIndex[w.wtype][w.grade].attackeffect;
           	--关于动画的参数

           	self.ateffectlist[aname].frameindex=0;
           	self.ateffectlist[aname].framecounter=0-dwDeltaTime;
           	
           	local amodel=TD_AttackEffectPre..self.ateffectlist[aname].atype;    	       	        	
           	DlgApi.CreateControl(self.this,amodel,aname);
			DlgApi.SetImageFile(self.this, aname, TD_AttackEffectIndex[self.ateffectlist[aname].atype].picname, TD_AttackEffectIndex[self.ateffectlist[aname].atype].frames);

			local direction=math.floor(w.directindex/(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum));
			local effdistowp={ x=w.bpos[1].bx*self.mapblocksize, y=w.bpos[1].by*self.mapblocksize };
			
			local ret = DlgApi.GetItemRect(self.this, aname);	--效果控件的大小

			if TD_AttackEffectIndex[self.ateffectlist[aname].atype].direnum ==8 and TD_AttackEffectIndex[self.ateffectlist[aname].atype].direnum==TD_WeaponIndex[w.wtype][w.grade].direnum then
				self.ateffectlist[aname].directindex=direction*TD_AttackEffectIndex[self.ateffectlist[aname].atype].franum+self.ateffectlist[aname].frameindex;
				
				if direction==1 or direction==2 or direction==3 then    --设置效果的偏移量中心点

					effdistowp.x=effdistowp.x+self.mapblocksize;
				end
				if direction==3 or direction==4 or direction==5 then
				    effdistowp.y=effdistowp.y-self.mapblocksize;
				end
				if direction==5 or direction==6 or direction==7 then
				    effdistowp.x=effdistowp.x-self.mapblocksize;
				end
				if direction==7 or direction==0 or direction==1 then
				    effdistowp.y=effdistowp.y+self.mapblocksize;
				end
				    						
			elseif TD_AttackEffectIndex[self.ateffectlist[aname].atype].direnum==1 then
			    self.ateffectlist[aname].directindex=0;
			else
			    self.ateffectlist[aname].directindex=0;
			end
			    
   			DlgApi.SelectImageFrame(self.this, aname,self.ateffectlist[aname].directindex);	
				
    		local posx=math.ceil(effdistowp.x+self.maporigin.x-ret.width/2);
    		local posy=math.ceil(effdistowp.y+self.maporigin.y-ret.height/2);
    		self:SetItemPos(aname,posx,posy);    						       	        		
    		DlgApi.ShowItem(self.this,aname,true);	
		       	        	
		end
end

--=========================
function Win_TestTowergame:UpdateMonster(dwDeltaTime)
	for mname,m in pairs(self.monsterlist) do
	    
	        --处理减速判断

	    if m.freezeflag == true then
	        m.freezecounter=m.freezecounter+dwDeltaTime;
	        if m.freezecounter>TD_FreezeTime then
	            m.freezeflag=false;
	            m.freezecounter=0;
	            m.speed=m.mindex.speed;	--去固定的数据列表读取原速度
	            
	            if m.freezeeffect~=nil then
	                DlgApi.DeleteControl(self.this,m.freezeeffect);
	            	self.beatkeflist[m.freezeeffect]=nil;
	            	m.freezeeffect=nil;
	            end
	       end
	    end
	        --处理减a护甲判断
	    if m.toaflag == true then
	        m.toacounter=m.toacounter+dwDeltaTime;
	        if m.toacounter>TD_FreezeTime then
	            m.toaflag=false;
	            m.toacounter=0;
	            m.adefance=m.mindex.adefence;	--去固定的数据列表读取原速度
	            
	            if m.toaeffect~=nil then
	                DlgApi.DeleteControl(self.this,m.toaeffect);
	            	self.beatkeflist[m.toaeffect]=nil;
	            	m.toaeffect=nil;
	            end
	       end
	    end
	        --处理b护甲判断
	    if m.tobflag == true then
	        m.tobcounter=m.tobcounter+dwDeltaTime;
	        if m.tobcounter>TD_FreezeTime then
	            m.tobflag=false;
	            m.tobcounter=0;
	            m.bdefence=m.mindex.bdefence;	--去固定的数据列表读取原速度
	            
	            if m.tobeffect~=nil then
	                DlgApi.DeleteControl(self.this,m.tobeffect);
	            	self.beatkeflist[m.tobeffect]=nil;
	            	m.tobeffect=nil;
	            end
	       end
	    end	    	    	    
	    
    	if m.flyflag==false then	--不飞
    		local distancex=m.nextpixcentre.x-m.pixpos.x;
    		local distancey=m.nextpixcentre.y-m.pixpos.y;
    		
    		local r=math.sqrt(distancex^2+distancey^2);
    		local sina=distancey/r;
    		local cosa=distancex/r;
    		
    		local newmdetx=m.speed*cosa*dwDeltaTime;--dwDeltaTime
    		local newmdety=m.speed*sina*dwDeltaTime;
    		
    	    
    	    
    		local newx=m.pixpos.x+newmdetx;
    		local newy=m.pixpos.y+newmdety;
    		
    		local tempx=(m.nextpixcentre.x-newx)*(m.nextpixcentre.x-m.pixpos.x);
    		local tempy=(m.nextpixcentre.y-newy)*(m.nextpixcentre.y-m.pixpos.y);
    				
    		if (tempx>0 and tempy==0) or (tempx==0 and tempy>0 ) or( tempx>0 and tempy>0 )	then	--若改变了位置还在目标中心点的同侧
    			m.pixpos.x=newx;
    			m.pixpos.y=newy;
        			
        		m.bpos.x=math.ceil(m.pixpos.x/self.mapblocksize);
        		m.bpos.y=math.ceil(m.pixpos.y/self.mapblocksize);
        		
        		--插入寻找可用的武器

        		local wlist = self.winblock[m.bpos.y][m.bpos.x];
        		for wname,state in pairs(wlist) do
        		    if self.weaponlist[wname].fireflag == true and self.weaponlist[wname].groundflag==true then
        		        self.weaponlist[wname].target = mname;
        		        self.weaponlist[wname].fireflag = false;
        		    end
        		end
    
    			--local ret= DlgApi.GetItemRect(self.this, mname);
    			local posx=math.ceil(m.pixpos.x+self.maporigin.x-m.w);
    			local posy=math.ceil(m.pixpos.y+self.maporigin.y-m.h);
    			
    			--图片不需要改方向
    			m.det.x=newmdetx;
    			m.det.y=newmdety;
    			self:SetItemPos(mname,posx,posy);
    			--设置血条

    			self:SetItemPos(m.lifeprog,posx,posy-4);  
    			--设置跟随的攻击效果

    			if m.freezeeffect~=nil then
    			    self:SetItemPos(m.freezeeffect,posx,posy);
    			end
    			if m.toaeffect~=nil then
    			    self:SetItemPos(m.toaeffect,posx,posy);
    			end
    			if m.tobeffect~=nil then
    			    self:SetItemPos(m.tobeffect,posx,posy);
    			end    			    			   		
    			
    		else
    					--寻径算法，输入 m.pos，获得下一个点的位置	
    			local minmap,nextbx,nextby=GameAI.GetNextPos(m.bpos.x, m.bpos.y, m.path-1);
        		if	minmap==true	then		--没有走出迷宫
        			m.pixpos.x=m.nextpixcentre.x;
        			m.pixpos.y=m.nextpixcentre.y;
    	    		m.bpos.x=math.ceil(m.pixpos.x/self.mapblocksize);
        			m.bpos.y=math.ceil(m.pixpos.y/self.mapblocksize);  
        			
        			--插入寻找可用的武器

	        		local wlist = self.winblock[m.bpos.y][m.bpos.x];
            		for wname,state in pairs(wlist) do
            		    if self.weaponlist[wname].fireflag == true and self.weaponlist[wname].groundflag==true then
            		        self.weaponlist[wname].target = mname;
            		        self.weaponlist[wname].fireflag = false;
            		    end
            		end        			  			
            			
        			m.nextbpos.x=nextbx;
        			m.nextbpos.y=nextby;
        			
        			m.nextpixcentre.x=(m.nextbpos.x-1)*self.mapblocksize+self.mapblockhsize;
    				m.nextpixcentre.y=(m.nextbpos.y-1)*self.mapblocksize+self.mapblockhsize;
        			
        			local distancex=m.nextpixcentre.x-m.pixpos.x;
        			local distancey=m.nextpixcentre.y-m.pixpos.y;
        			local r=math.sqrt(distancex^2+distancey^2);
        			local sina=distancey/r;
        			local cosa=distancex/r;
        			
	       			m.det.x=m.speed*cosa*dwDeltaTime;	--此处直接cos和sin也可以

        			m.det.y=m.speed*sina*dwDeltaTime;
        			--重新设置图片的方向    		
					m.directindex=self:UpdateMonsterDirection(m);
					DlgApi.SelectImageFrame(self.this, mname, m.directindex);	        			
        			
        			--local ret= DlgApi.GetItemRect(self.this, mname);	
    				local posx=math.ceil(m.pixpos.x+self.maporigin.x-m.w);
    				local posy=math.ceil(m.pixpos.y+self.maporigin.y-m.h);
    				
	  				self:SetItemPos(mname,posx,posy);
	    			--设置血条

    				self:SetItemPos(m.lifeprog,posx,posy-4);
        			--设置跟随的攻击效果

        			if m.freezeeffect~=nil then
        			    self:SetItemPos(m.freezeeffect,posx,posy);
        			end 
        			if m.toaeffect~=nil then
        			    self:SetItemPos(m.toaeffect,posx,posy);
        			end
        			if m.tobeffect~=nil then
        			    self:SetItemPos(m.tobeffect,posx,posy);
        			end        			        			    					  				    						
       			
        		else				--走出了迷宫

        			self:MonsterOutMap(m,mname);
          		end
    		end
    	else
    	    local newmdetx=m.speed*m.det.x*dwDeltaTime;--dwDeltaTime
    		local newmdety=m.speed*m.det.y*dwDeltaTime;
    		local newx=m.pixpos.x+newmdetx;
    		local newy=m.pixpos.y+newmdety;
    		local tempx=(m.exitpos.x-newx)*(m.exitpos.x-m.pixpos.x);
    		local tempy=(m.exitpos.y-newy)*(m.exitpos.y-m.pixpos.y);
    		if (tempx>0 and tempy==0) or (tempx==0 and tempy>0 ) or( tempx>0 and tempy>0 )	then	--若改变了位置还在目标中心点的同侧
    			m.pixpos.x=newx;
    			m.pixpos.y=newy;
	    		m.bpos.x=math.ceil(m.pixpos.x/self.mapblocksize);
    			m.bpos.y=math.ceil(m.pixpos.y/self.mapblocksize);    			
        		--插入寻找可用的武器

        		local wlist = self.winblock[m.bpos.y][m.bpos.x];
        		for wname,state in pairs(wlist) do
        		    if self.weaponlist[wname].fireflag == true and self.weaponlist[wname].airflag==true then
        		        self.weaponlist[wname].target = mname;
        		        self.weaponlist[wname].fireflag = false;
        		    end
        		end

        			
    			--local ret= DlgApi.GetItemRect(self.this, mname);
    			local posx=math.ceil(m.pixpos.x+self.maporigin.x-m.w);
    			local posy=math.ceil(m.pixpos.y+self.maporigin.y-m.h);
    			--图片不需要改方向

    			self:SetItemPos(mname,posx,posy); 
   				self:SetItemPos(m.lifeprog,posx,posy-4);
    			--设置跟随的攻击效果

    			if m.freezeeffect~=nil then
    			    self:SetItemPos(m.freezeeffect,posx,posy);
    			end
    			if m.toaeffect~=nil then
    			    self:SetItemPos(m.toaeffect,posx,posy);
    			end
    			if m.tobeffect~=nil then
    			    self:SetItemPos(m.tobeffect,posx,posy);
    			end    			    			    				    			    		
    		else
 				self:MonsterOutMap(m,mname);
      			
    		end    		    
		
		end
		
	end
end
--=========================
function Win_TestTowergame:MonsterOutMap(m,mname)
				--销毁控件，删除，减玩家的生命值

		if m.bossflag == true then
		    self.AllowDeath = self.AllowDeath -TD_BossHurt;
		else
		    self.AllowDeath = self.AllowDeath -TD_NormalHurt;
		end
		
		DlgApi.SetProgress(self.this,"Prog_Life",self.AllowDeath/TD_AllowDeadNum);
		
		DlgApi.DeleteControl(self.this,mname);
			--删除血条

		self.mlifelist[m.lifeprog]=nil;
		DlgApi.DeleteControl(self.this,m.lifeprog);
			--删除跟随的特效

        if m.freezeeffect~=nil then
            DlgApi.DeleteControl(self.this,m.freezeeffect);
        	self.beatkeflist[m.freezeeffect]=nil;
        	m.freezeeffect=nil;
        end
        if m.toaeffect~=nil then
            DlgApi.DeleteControl(self.this,m.toaeffect);
        	self.beatkeflist[m.toaeffect]=nil;
        	m.toaeffect=nil;
        end
        if m.tobeffect~=nil then
            DlgApi.DeleteControl(self.this,m.tobeffect);
        	self.beatkeflist[m.tobeffect]=nil;
        	m.tobeffect=nil;
        end                        			

  		
  		--考虑何处进行武器和子弹的处理，目标已经消失(在子弹每帧和武器目标选定每帧做判断)
--  		for bname,b in pairs(self.bulletlist) do	--子弹继续跑

--  		    if b.target==mname then
--  		        b.targetpos.x=m.pixpos.x;
--  		        b.targetpos.y=m.pixpos.y;
--  		        b.target=nil;
--  		    end      			        
--  		end
  		for bname,b in pairs(self.bulletlist) do	--子弹中途消失

  		    if b.target==mname then
				DlgApi.DeleteControl(self.this,bname);
				self.bulletlist[bname] = nil;
  		    end      			        
  		end
  		
  		self.monsterlist[mname]=nil;
  		self.minscreen=self.minscreen-1;
  		
  		--判断游戏是否结束
     	if self.AllowDeath <= 0 then
     	   --给出失败界面，
		   self.Score = math.ceil(self.Score);
     	   Win_TowerDefenseFail:OnShowDialog(self.curgame,self.Score);
     	   
     	   self.startpause=TD_PauseState;
     	   --清除游戏的状态

     	   self:Clear();

     	else          			

    		        --是否可以出下一拨怪, 是否可以游戏成功，桌面怪物清零，该拨怪物全部走出
        	local flag=true;
        	for curpath=1,#TD_MapPath[self.curgame] do   		
           		if self.mnumcounter[curpath]~=self.numpergroup[curpath] then
           		    flag=false;
           		end
        	end
    		if self.minscreen==0  and flag==true then
    		    if self.level<#TD_PerPassData[self.curgame] then
                    DlgApi.EnableItem(self.this,"Btn_NextWave",true);
                    DlgApi.ShowItem(self.this,"Lab_MonsterPic",false);
                    self.curmonster={};
                    for i=1,4 do
                        DlgApi.ShowItem(self.this,"Pic_MShow"..i,false);
                    end                
    		    else
    		        --游戏顺利过关
    		        
                	--设置任务开始
                	if TD_TaskIndex and TD_TaskIndex[self.diff] and TD_TaskIndex[self.diff][self.curgame] then
                	    GameAI.TaskGiveup(TD_TaskIndex[self.diff][self.curgame]);
                	    GameAI.TaskReq(TD_TaskIndex[self.diff][self.curgame]);
                	end    	
                		        
    		        self.Score = math.ceil(self.Score);
        	        
    		        Win_TowerDefenseWin:OnShowDialog(self.curgame,self.Score,self.diff);
    		        
    		        self.startpause=TD_PauseState;
             	   --清除游戏的状态
    
             	   self:Clear();
    		    end
            end
     	end
end


--==========================
function Win_TestTowergame:CreateMonster(dwDeltaTime)
								    --对30s的等待时间进行记数

					    
  	self.mgroupcounter = self.mgroupcounter+dwDeltaTime;
   	--把时间传递给倒计时的现实框

   	local timer = math.floor(self.mgroupcounter/1000);
   	
	--当到达第n波怪物的生成时间时	
	local pathnum = #TD_MapPath[self.curgame];	
	if self.mgroupcounter>TD_WaitPerLevel[self.curgame] then
	    self.mgroupcounter=TD_WaitPerLevel[self.curgame]+1;
	end

   	if self.mgroupcounter>TD_WaitPerLevel[self.curgame] and self.level<#TD_PerPassData[self.curgame] then
   	   	self.mgroupcounter=0;
   	    DlgApi.EnableItem(self.this,"Btn_NextWave",false);
   		self.level=self.level+1;	--初始化为0，从1开始

   		DlgApi.SetItemText(self.this,"Edit_Wave",tostring(#TD_PerPassData[self.curgame]-self.level));

   		-----
    	for curpath=1,pathnum do   		
       		self.mcreateflag[curpath]=1;    
       		self.mcreatecounter[curpath]=0;
       		self.mnumcounter[curpath]=0;
       		--确定这个时间段生成的怪物的种类

       			--飞否
            if TD_PerPassData[self.curgame][self.level][curpath] == nil then
            	self.numpergroup[curpath] = 0;
            else          		
               		local retmpic = nil;
               		
               		local rt=math.random();	
               		if rt<TD_PerPassData[self.curgame][self.level][curpath].flyrate then
               		    self.monsterfly[curpath]=true;
               		else
               		    self.monsterfly[curpath]=false;
               		end
        
               		    --boss否

               		rt=math.random();			
               		if rt<TD_PerPassData[self.curgame][self.level][curpath].bossrate then
               		    self.monsterboss[curpath]=true;
               		else
               		    self.monsterboss[curpath]=false;
               		end
               			--种类,数目
               		if self.monsterboss[curpath]==true then
               		    if self.monsterfly[curpath]==true then
               		        --boss,会飞
               		        if TD_PerPassData[self.curgame][self.level][curpath].bossnrandom==nil then
               		            self.numpergroup[curpath]=TD_PerPassData[self.curgame][self.level][curpath].bossnum;
               		        else
               		            self.numpergroup[curpath]=math.random(TD_PerPassData[self.curgame][self.level][curpath].bossnrandom);
               		        end		--选一关数目

               		        
               		        if TD_PerPassData[self.curgame][self.level][curpath].randomtype==false then
               		            self.monstertype[curpath]=TD_PerPassData[self.curgame][self.level][curpath].bossflytype;
               		        else
               		            --self.monstertype[curpath]=math.random(TD_MBossNormal+1,#TD_MonsterBossIndex);
               		            local nindex = math.ceil(self.level/(#TD_PerPassData[self.curgame]/#TD_FlyRdBossTypeIndex[self.curgame]));
               		            if nindex>#TD_FlyRdBossTypeIndex[self.curgame] then
               		                nindex = #TD_FlyRdBossTypeIndex[self.curgame];
               		            end
               		            self.monstertype[curpath]=math.random(TD_FlyRdBossTypeIndex[self.curgame][nindex][1],TD_FlyRdBossTypeIndex[self.curgame][nindex][2]);
               		            
               		        end		--选一关的种类
               		        self.curmonster[curpath] = TD_MonsterBossIndex[self.monstertype[curpath]];
               		        DlgApi.SetImageFile(self.this,"Pic_MShow"..curpath+2,self.curmonster[curpath].picname,self.curmonster[curpath].frames);
               		        DlgApi.ShowItem(self.this,"Pic_MShow"..curpath+2,true);               		        
               		    
               		    else		--boss,不会飞

               		        if TD_PerPassData[self.curgame][self.level][curpath].bossnrandom==nil then
               		            self.numpergroup[curpath]=TD_PerPassData[self.curgame][self.level][curpath].bossnum;
               		        else
               		            self.numpergroup[curpath]=math.random(TD_PerPassData[self.curgame][self.level][curpath].bossnrandom);
               		        end		--选一关数目

               		        
               		        if TD_PerPassData[self.curgame][self.level][curpath].randomtype==false then
               		            self.monstertype[curpath]=TD_PerPassData[self.curgame][self.level][curpath].bosstype;
               		        else
               		            --self.monstertype[curpath]=math.random(TD_MBossNormal);
               		            local nindex = math.ceil(self.level/(#TD_PerPassData[self.curgame]/#TD_RdBossTypeIndex[self.curgame]));
               		            if nindex>#TD_RdBossTypeIndex[self.curgame] then
               		                nindex = #TD_RdBossTypeIndex[self.curgame];
               		            end
               		            self.monstertype[curpath]=math.random(TD_RdBossTypeIndex[self.curgame][nindex][1],TD_RdBossTypeIndex[self.curgame][nindex][2]);
               		                   		            
               		        end		--选一关的种类 
               		        self.curmonster[curpath] = TD_MonsterBossIndex[self.monstertype[curpath]];
               		        DlgApi.SetImageFile(self.this,"Pic_MShow"..curpath+2,self.curmonster[curpath].picname,self.curmonster[curpath].frames);	
               		        DlgApi.ShowItem(self.this,"Pic_MShow"..curpath+2,true);
               		        
               		    end
               		           
               		else
               		    if self.monsterfly[curpath]==true then
               		        --普通,会飞
            	            self.numpergroup[curpath]=TD_PerPassData[self.curgame][self.level][curpath].monsternum;
              		        
               		        if TD_PerPassData[self.curgame][self.level][curpath].randomtype==false then
               		            self.monstertype[curpath]=TD_PerPassData[self.curgame][self.level][curpath].flytype;
               		        else
               		            --self.monstertype[curpath]=math.random(TD_MNormal+1,#TD_MonsterIndex);
               		            local nindex = math.ceil(self.level/(#TD_PerPassData[self.curgame]/#TD_FlyRdTypeIndex[self.curgame]));
               		            if nindex>#TD_FlyRdTypeIndex[self.curgame] then
               		                nindex = #TD_FlyRdTypeIndex[self.curgame];
               		            end
               		            self.monstertype[curpath]=math.random(TD_FlyRdTypeIndex[self.curgame][nindex][1],TD_FlyRdTypeIndex[self.curgame][nindex][2]);
               		           		            
               		        end		--选一关的种类
               		        self.curmonster[curpath] = TD_MonsterIndex[self.monstertype[curpath]];
               		        DlgApi.SetImageFile(self.this,"Pic_MShow"..curpath,self.curmonster[curpath].picname,self.curmonster[curpath].frames);
               		        DlgApi.ShowItem(self.this,"Pic_MShow"..curpath,true);
               		    
               		    else		--普通,不会飞

               		        self.numpergroup[curpath]=TD_PerPassData[self.curgame][self.level][curpath].monsternum;
               		        
               		        if TD_PerPassData[self.curgame][self.level][curpath].randomtype==false then
               		            self.monstertype[curpath]=TD_PerPassData[self.curgame][self.level][curpath].monstertype;
               		        else
               		            --self.monstertype[curpath]=math.random(TD_MNormal);
               		            local nindex = math.ceil(self.level/(#TD_PerPassData[self.curgame]/#TD_RdTypeIndex[self.curgame]));
               		            if nindex>#TD_RdTypeIndex[self.curgame] then
               		                nindex = #TD_RdTypeIndex[self.curgame];
               		            end
               		            self.monstertype[curpath]=math.random(TD_RdTypeIndex[self.curgame][nindex][1],TD_RdTypeIndex[self.curgame][nindex][2]);
               		          		            
               		        end		--选一关的种类 	
               		        self.curmonster[curpath] = TD_MonsterIndex[self.monstertype[curpath]];
               		        DlgApi.SetImageFile(self.this,"Pic_MShow"..curpath,self.curmonster[curpath].picname,self.curmonster[curpath].frames);               		        
               		        DlgApi.ShowItem(self.this,"Pic_MShow"..curpath,true);
               		    end   		    
               		    
               		end
               		--显示当前的怪物在屏幕上
               		DlgApi.ShowItem(self.this,"Lab_MonsterPic",true);

       		    end
       	end

	end  
	
    	    						--当到了怪物出现的时间，生成怪物
    for curpath=1,pathnum do
           		
		if( self.mnumcounter[curpath] == self.numpergroup[curpath] ) then
		    self.mcreateflag[curpath] =0;
		end

    	if(self.mcreateflag[curpath]==1) then
    		self.mcreatecounter[curpath] = self.mcreatecounter[curpath]+dwDeltaTime;
    		
    		if( self.mcreatecounter[curpath]>TD_PerPassData[self.curgame][self.level][curpath].waitperm ) then
    			self.mcreatecounter[curpath]=0;
    			---生成怪物
    			--选择好怪物的种类

    			self:CreateEachMonster(dwDeltaTime,curpath);	
    		end
   	
    	end
    end	
end
--================================
function Win_TestTowergame:CreateEachMonster(dwDeltaTime,curpath)
		
		self.mnumcounter[curpath]=self.mnumcounter[curpath] +1;
		--local pathnum = #TD_MapPath[self.curgame];	
		--for curpath=1,pathnum do
		--local pos=math.random(ExitWidth);同时pixpos的位置还可以在格子中随机调整
			--在Path的入口中选择 pos位置的坐标生成该控件，加入控件的列表 mosternum
		local path=curpath;
		local mtype=self.monstertype[path];
		local bossflag=self.monsterboss[path];
		local flyflag=self.monsterfly[path];
		local varflag=false;
		
		self.monsternum=self.monsternum+1;		
		local monstername = "Monster"..self.monsternum;
		self.minscreen=self.minscreen+1;
		
		--生成怪物控件
		if bossflag==true then		--是boss
			local btn = TD_MonsterPrefix..TD_MonsterBossIndex[mtype].size;
			self.monsterlist[monstername]=CreateObj(TD_MonsterBossIndex[mtype]);
			self.monsterlist[monstername].w = TD_MonsterBossIndex[mtype].hsize;
			self.monsterlist[monstername].h = TD_MonsterBossIndex[mtype].hsize;

			self.monsterlist[monstername].mindex=TD_MonsterBossIndex[mtype];
			DlgApi.CreateControl(self.this,btn,monstername);
			DlgApi.SetImageFile(self.this, monstername, TD_MonsterBossIndex[mtype].picname, TD_MonsterBossIndex[mtype].frames);
			

		else						--不是boss
--			local ret=math.random();
--			if ret<TD_PerPassData[self.curgame][self.level][path].variance then
--			    varflag=true;		--变种
--			    local btn = TD_MonsterVariPrefix;--..mtype;
--			    self.monsterlist[monstername]=CreateObj(TD_MonsterVariIndex[mtype]);
--				self.monsterlist[monstername].w = 32;
--				self.monsterlist[monstername].h = 32;
--			    
--			    self.monsterlist[monstername].mindex=TD_MonsterVariIndex[mtype];
--			    DlgApi.CreateControl(self.this,btn,monstername);
--			    DlgApi.SetImageFile(self.this, monstername, TD_MonsterVariIndex[mtype].picname, TD_MonsterVariIndex[mtype].frames);
--			    
--
--			else
			    varflag=false;		--正常
			    local btn = TD_MonsterPrefix..TD_MonsterIndex[mtype].size;
			    self.monsterlist[monstername]=CreateObj(TD_MonsterIndex[mtype]);
			    self.monsterlist[monstername].w = TD_MonsterIndex[mtype].hsize;
			    self.monsterlist[monstername].h = TD_MonsterIndex[mtype].hsize;
			    
			    self.monsterlist[monstername].mindex=TD_MonsterIndex[mtype];
			    DlgApi.CreateControl(self.this,btn,monstername);
			    DlgApi.SetImageFile(self.this, monstername, TD_MonsterIndex[mtype].picname, TD_MonsterIndex[mtype].frames);
--			end
		end
			
		self.monsterlist[monstername].life = self.mlifecoe*self.monsterlist[monstername].life ;
		self.monsterlist[monstername].mtype=mtype;
		
		self.monsterlist[monstername].path=path;
	
		self.monsterlist[monstername].bossflag=bossflag;
		self.monsterlist[monstername].flyflag=flyflag;
		self.monsterlist[monstername].varflag=varflag;
		
		local entrynum=math.random(#TD_MapPath[self.curgame][path].entry);
		self.monsterlist[monstername].bpos={};
		self.monsterlist[monstername].bpos.x=TD_MapPath[self.curgame][path].entry[entrynum][1];
		self.monsterlist[monstername].bpos.y=TD_MapPath[self.curgame][path].entry[entrynum][2];
		
		self.monsterlist[monstername].pixpos ={} ;
		self.monsterlist[monstername].pixpos.x=(self.monsterlist[monstername].bpos.x-1)*self.mapblocksize+self.mapblockhsize;
		self.monsterlist[monstername].pixpos.y=(self.monsterlist[monstername].bpos.y-1)*self.mapblocksize+self.mapblockhsize;
    		--中了减速弹效果的统计

		self.monsterlist[monstername].freezeflag=false;
		self.monsterlist[monstername].freezecounter=0;
			--中了a类护甲的攻击
		self.monsterlist[monstername].toaflag=false;
		self.monsterlist[monstername].toacounter=0;
			--中了b类护甲的攻击
		self.monsterlist[monstername].tobflag=false;
		self.monsterlist[monstername].tobcounter=0;	

		if self.monsterlist[monstername].flyflag == true then		--会飞的话，设置一下最远的
    		local exitnum=math.random(#TD_MapPath[self.curgame][path].exit);
    		self.monsterlist[monstername].exitpos={};
    		self.monsterlist[monstername].exitpos.x=(TD_MapPath[self.curgame][path].exit[exitnum][1]-1)*self.mapblocksize+self.mapblockhsize;
    		self.monsterlist[monstername].exitpos.y=(TD_MapPath[self.curgame][path].exit[exitnum][2]-1)*self.mapblocksize+self.mapblockhsize;
			--方向
    		local distancex=self.monsterlist[monstername].exitpos.x-self.monsterlist[monstername].pixpos.x;
    		local distancey=self.monsterlist[monstername].exitpos.y-self.monsterlist[monstername].pixpos.y;
    		
    		local r=math.sqrt(distancex^2+distancey^2);
    		local sina=distancey/r;
    		local cosa=distancex/r;
			
    		self.monsterlist[monstername].det={};
    		
    		
    		
    		self.monsterlist[monstername].det.x=cosa;--方向
    		self.monsterlist[monstername].det.y=sina;
    		self.monsterlist[monstername].frameindex=0;
    		self.monsterlist[monstername].framecounter=0-dwDeltaTime;
    		--self.monsterlist[monstername].directindex=0;
    		--设置飞行怪物的方向

    		local a1=math.cos(math.pi/8);
    		local a2=math.cos(math.pi*3/8);
    		if cosa==0 then
    		    if sina>0 then self.monsterlist[monstername].directindex=0*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;
    		    else self.monsterlist[monstername].directindex=4*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;end
    		elseif sina==0 then
    		    if cosa>0 then self.monsterlist[monstername].directindex=2*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;
    		    else self.monsterlist[monstername].directindex=6*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;end
    		elseif cosa>0 and sina>0 then
    		    if cosa>a1 then self.monsterlist[monstername].directindex=2*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;
    		    elseif cosa<a2 then self.monsterlist[monstername].directindex=0*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;
    		    else self.monsterlist[monstername].directindex=1*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;end
    		elseif cosa>0 and sina<0 then
    		    if cosa>a1 then self.monsterlist[monstername].directindex=2*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;
    		    elseif cosa<a2 then self.monsterlist[monstername].directindex=4*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;
    		    else self.monsterlist[monstername].directindex=3*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;end
    		elseif cosa<0 and sina>0 then
    		    if cosa<0-a1 then self.monsterlist[monstername].directindex=6*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;
    		    elseif cosa>0-a2 then self.monsterlist[monstername].directindex=0*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;
    		    else self.monsterlist[monstername].directindex=7*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;end
    		elseif cosa<0 and sina<0 then
    		    if cosa<0-a1 then self.monsterlist[monstername].directindex=6*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;   
    		    elseif cosa>0-a2 then self.monsterlist[monstername].directindex=4*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;
    		    else self.monsterlist[monstername].directindex=5*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;end
    		else
    		    self.monsterlist[monstername].directindex=0*self.monsterlist[monstername].mindex.franum+self.monsterlist[monstername].frameindex;
    		end
			-----
		else
    		self.monsterlist[monstername].nextbpos={};
    		self.monsterlist[monstername].nextbpos.x = self.monsterlist[monstername].bpos.x;
    		self.monsterlist[monstername].nextbpos.y = self.monsterlist[monstername].bpos.y;
    		
    		self.monsterlist[monstername].nextpixcentre={};
    		self.monsterlist[monstername].nextpixcentre.x=(self.monsterlist[monstername].bpos.x-1)*self.mapblocksize+self.mapblockhsize;
    		self.monsterlist[monstername].nextpixcentre.y=(self.monsterlist[monstername].bpos.y-1)*self.mapblocksize+self.mapblockhsize;
  		
    		--方向
    		self.monsterlist[monstername].det={};
    		self.monsterlist[monstername].det.x=self.monsterlist[monstername].nextpixcentre.x-self.monsterlist[monstername].pixpos.x;--方向
    		self.monsterlist[monstername].det.y=self.monsterlist[monstername].nextpixcentre.y-self.monsterlist[monstername].pixpos.y;
    		self.monsterlist[monstername].frameindex=0;
    		self.monsterlist[monstername].framecounter=0-dwDeltaTime;
    		--设置图片的方向

    		self.monsterlist[monstername].directindex=self:UpdateMonsterDirection(self.monsterlist[monstername]);    		
    		
		end

			--选位置的图片
		DlgApi.SelectImageFrame(self.this, monstername, self.monsterlist[monstername].directindex);
			
		--local ret = DlgApi.GetItemRect(self.this, monstername);		
		local posx = math.ceil(self.monsterlist[monstername].pixpos.x+self.maporigin.x-self.monsterlist[monstername].w);
		local posy = math.ceil(self.monsterlist[monstername].pixpos.y+self.maporigin.y-self.monsterlist[monstername].h);
		self:SetItemPos(monstername,posx,posy);
		DlgApi.ShowItem(self.this,monstername,true);

		--生成血条

		local mprogname="Mlife"..self.monsternum;
		self.mlifelist[mprogname]=true;
		self.monsterlist[monstername].lifeprog=mprogname;
		DlgApi.CreateControl(self.this,TD_MlifePro,mprogname);

		self:SetItemPos(mprogname,posx,posy-4);
		DlgApi.SetProgress(self.this,mprogname,1);
		DlgApi.ShowItem(self.this,mprogname,true);

	
end

--==========================
function Win_TestTowergame:WeaponDirection(w)
    local direct=0;
    local thres1 = math.pi/8;
	local thres2 = math.pi * 3/8;

 	if w.direction.detx==0 then
 		if w.direction.dety>0 then
 		    
  	       if TD_WeaponIndex[w.wtype][w.grade].direnum==8 then
 	               direct=0*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;
 	       
 	       elseif TD_WeaponIndex[w.wtype][w.grade].direnum==1 then
 	           direct=0;
 	       end
 	       		    
 		else
 		    
 	       if TD_WeaponIndex[w.wtype][w.grade].direnum==8 then
 	               direct=4*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;
 	           
 	       elseif TD_WeaponIndex[w.wtype][w.grade].direnum==1 then
 	           direct=0;
 	       end
 	        		    
 		end
 	elseif w.direction.dety==0 then
 		if w.direction.detx>0 then
 		    
  	       if TD_WeaponIndex[w.wtype][w.grade].direnum==8 then
 	               direct=2*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;

 	       elseif TD_WeaponIndex[w.wtype][w.grade].direnum==1 then
 	           direct=0;
 	       end 		
 	           
 		else
 		    
  	       if TD_WeaponIndex[w.wtype][w.grade].direnum==8 then

 	               direct=6*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;
  	           
 	       elseif TD_WeaponIndex[w.wtype][w.grade].direnum==1 then
 	           direct=0;
 	       end 	
 	       	    
 		end 	
 	
 	elseif w.direction.detx >0 then
    	local angle= math.atan(math.abs(w.direction.dety)/math.abs(w.direction.detx));
    	
 	    if w.direction.dety > 0 then --第1象限
 	       if TD_WeaponIndex[w.wtype][w.grade].direnum==8 then
 	           
 	           if angle<thres1 then
 	               		direct=2*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;
 	               
 	           elseif angle>thres2 then
 	                   direct=0*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;

 	           else
 	                   direct=1*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;

 	           end
       
 	       elseif TD_WeaponIndex[w.wtype][w.grade].direnum==1 then
 	           direct=0;
 	       end
 	       
	   elseif w.direction.dety<0 then --第2象限

  	       if TD_WeaponIndex[w.wtype][w.grade].direnum==8 then
  	           
  	           if angle<thres1 then
  	               direct=2*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;
  	           elseif angle>thres2 then
  	               direct=4*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;
  	           else
  	               direct=3*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;
  	           end
  	           
	       elseif TD_WeaponIndex[w.wtype][w.grade].direnum==1 then
 	           direct=0;
 	       end

 	    end
 	    
	elseif w.direction.detx <0 then
		local angle= math.atan(math.abs(w.direction.dety)/math.abs(w.direction.detx));	    
 	    
 	    if w.direction.dety >0 then --第三象限
  	       if TD_WeaponIndex[w.wtype][w.grade].direnum==8 then
  	           
  	           if angle<thres1 then
  	               direct=6*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;
  	           elseif angle>thres2 then
  	               direct=0*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;
  	           else
  	               direct=7*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;
  	           end  	           
 
 	       elseif TD_WeaponIndex[w.wtype][w.grade].direnum==1 then
 	           direct=0;
 	       end

 	    elseif w.direction.dety <0 then --第四象限
  	       if TD_WeaponIndex[w.wtype][w.grade].direnum==8 then
  	           
  	           if angle<thres1 then
  	               direct=6*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;
  	           elseif angle>thres2 then
  	               direct=4*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;
  	           else
  	               direct=5*(TD_WeaponIndex[w.wtype][w.grade].franum+TD_WeaponIndex[w.wtype][w.grade].acfranum)+w.frameindex;
  	           end  	           
         
 	       elseif TD_WeaponIndex[w.wtype][w.grade].direnum==1 then
 	           direct=0;
 	       end

 	    end
 	end
	
 	return direct;        
	
end
--====================
function Win_TestTowergame:UpdateMonsterDirection(m)

    local direct=0;
 	if m.det.x >0 then
    
 	    if m.det.y > 0 then 
 	       if m.mindex.direnum==8 then
 	           direct=1*m.mindex.franum+m.frameindex;
 	       elseif m.mindex.direnum==1 then
 	           direct=0;
 	       end
 	       
 	    elseif m.det.y == 0 then

  	       if m.mindex.direnum==8 then
  	           direct=2*m.mindex.franum+m.frameindex;	           
 	       elseif m.mindex.direnum==1 then
 	           direct=0;
 	       end
 	         	 		    	 
 	    elseif m.det.y<0 then

  	       if m.mindex.direnum==8 then
  	           direct=3*m.mindex.franum+m.frameindex;	           
 	       elseif m.mindex.direnum==1 then
 	           direct=0;
 	       end

 	    end
 	    
 	elseif m.det.x == 0 then
    
 	    if m.det.y >0 then
  	       if m.mindex.direnum==8 then
  	           direct=0*m.mindex.franum+m.frameindex;	           
 	       elseif m.mindex.direnum==1 then
 	           direct=0;
 	       end	       
 	    
 	    elseif m.det.y <0 then

  	       if m.mindex.direnum==8 then
  	           direct=7*m.mindex.franum+m.frameindex;	           
 	       elseif m.mindex.direnum==1 then
 	           direct=0;
 	       end


 	    end
 	    
 	elseif m.det.x <0 then
 	    
 	    if m.det.y >0 then
  	       if m.mindex.direnum==8 then
  	           direct=4*m.mindex.franum+m.frameindex;	           
 	       elseif m.mindex.direnum==1 then
 	           direct=0;
 	       end

 	    elseif m.det.y == 0 then
  	       if m.mindex.direnum==8 then
  	           direct=5*m.mindex.franum+m.frameindex;	           
 	       elseif m.mindex.direnum==1 then
 	           direct=0;
 	       end

 	    elseif m.det.y <0 then
  	       if m.mindex.direnum==8 then
  	           direct=6*m.mindex.franum+m.frameindex;	           
 	       elseif m.mindex.direnum==1 then
 	           direct=0;
 	       end

 	    end
 	end
 	return direct;        
end



--===============
function Win_TestTowergame:UpdateAllAction(dwDeltaTime)
    self:WRangeAction(dwDeltaTime);
    
    self:WeaponAction(dwDeltaTime);
    self:AttackAction(dwDeltaTime);
    
    self:MonsterAction(dwDeltaTime);
    self:BeAttackAction(dwDeltaTime);
    self:DeathAction(dwDeltaTime);
    
    self:BulletAction(dwDeltaTime);
end
--------
function Win_TestTowergame:WRangeAction(dwDeltaTime)
    for i=1,#TD_WRangeCircle do
        self.weaponrange[i].framecounter=self.weaponrange[i].framecounter+dwDeltaTime;
        if self.weaponrange[i].framecounter>TD_WRangeCircle[i].timeperfra then
            self.weaponrange[i].framecounter=0;
            
            if self.weaponrange[i].frame<TD_WRangeCircle[i].franum-1 then
                self.weaponrange[i].frame=self.weaponrange[i].frame+1;
            else
                self.weaponrange[i].frame=0;
            end        
            DlgApi.SelectImageFrame(self.this, TD_WeaponRangePre..i, self.weaponrange[i].frame);
        end
    end
end
---------
function Win_TestTowergame:WeaponAction(dwDeltaTime)
   for wname,w in pairs(self.weaponlist) do
       	w.framecounter=w.framecounter+dwDeltaTime;
       	
    	if w.attackflag == false and w.framecounter>TD_WeaponIndex[w.wtype][w.grade].timeperfra then
    	    w.framecounter=0;
        	w.directindex,w.frameindex=self:UpdateWeaponMotion(w);
        	DlgApi.SelectImageFrame(self.this, wname, w.directindex);	
    	elseif w.attackflag == true and w.framecounter>TD_WeaponIndex[w.wtype][w.grade].attperfra then
        	w.framecounter=0;
        	w.directindex,w.acframeindex=self:UpdateWeaponMotion(w);
	       	DlgApi.SelectImageFrame(self.this, wname, w.directindex);
    	end
    end 
	
end

function Win_TestTowergame:UpdateWeaponMotion(w)
    
    local direct=w.directindex;
    local frame = 0;
    
    if w.attackflag == false then
        frame=w.frameindex;
        if frame<TD_WeaponIndex[w.wtype][w.grade].franum-1 then
            frame=frame+1;
        	direct=direct+1;
        else
            direct=direct-frame;
            frame=0;
        end
    
    elseif w.attackflag == true then
        frame=w.acframeindex;
        if frame<TD_WeaponIndex[w.wtype][w.grade].acfranum-1 then
            frame=frame+1;
        	direct=direct+1;
        else
            direct=direct-frame-TD_WeaponIndex[w.wtype][w.grade].franum;
            w.frameindex = 0;
            w.attackflag = false;
            frame=0;
        end        
    end
        
    return direct,frame;
end
-----------
function Win_TestTowergame:MonsterAction(dwDeltaTime)    
   for mname,m in pairs(self.monsterlist) do
       	--进行动画的播放，不能放到最后处理，最后走出去的怪物已经销毁了控件
    	m.framecounter=m.framecounter+dwDeltaTime;
    	if m.framecounter>m.mindex.timeperfra then
    	    m.framecounter=0;
    	    m.directindex,m.frameindex=self:UpdateMonsterMotion(m);
    		DlgApi.SelectImageFrame(self.this, mname, m.directindex);		    
    	end
	end 
end

function Win_TestTowergame:UpdateMonsterMotion(m)
    local frame=m.frameindex;
    local direct=m.directindex;
    
    if frame<m.mindex.franum-1 then
        frame=frame+1;
    	direct=direct+1;
    else
        direct=direct-frame;
        frame=0;
    end
    return direct,frame;
end
-------------------
function Win_TestTowergame:AttackAction(dwDeltaTime)
	for aname,a in pairs(self.ateffectlist) do
	    a.framecounter=a.framecounter+dwDeltaTime;
	    if a.framecounter>TD_AttackEffectIndex[a.atype].timeperfra then
	        a.framecounter=0;
	        
            if a.frameindex<TD_AttackEffectIndex[a.atype].franum-1 then
                a.frameindex=a.frameindex+1;
                a.directindex=a.directindex+1;
                DlgApi.SelectImageFrame(self.this, aname, a.directindex);
            else
                self.ateffectlist[aname]=nil;
                DlgApi.DeleteControl(self.this,aname);
            end        
            
	    end
	end
end
--------------------
function Win_TestTowergame:BeAttackAction(dwDeltaTime)
	for bname,b in pairs(self.beatkeflist) do
	    b.framecounter=b.framecounter+dwDeltaTime;
	    if b.framecounter>TD_BeAtkEfeIndex[b.btype].timeperfra then
	        b.framecounter=0;
	        
            if b.frameindex<TD_BeAtkEfeIndex[b.btype].franum-1 then
                b.frameindex=b.frameindex+1;
            else
                b.frameindex=0;
            end        
	        DlgApi.SelectImageFrame(self.this, bname, b.frameindex);            
	    end
	end
end
--------------------

function Win_TestTowergame:DeathAction(dwDeltaTime)
	for dname,d in pairs(self.rteflist) do
	    d.framecounter=d.framecounter+dwDeltaTime;
	    if d.framecounter>TD_RTEfeIndex[d.dtype].timeperfra then
	        d.framecounter=0;
	        
            if d.frameindex<TD_RTEfeIndex[d.dtype].franum-1 then
                d.frameindex=d.frameindex+1;
                DlgApi.SelectImageFrame(self.this, dname, d.frameindex);
            else
                self.rteflist[dname]=nil;
                DlgApi.DeleteControl(self.this,dname);
            end        
            
	    end
	end
end
--------------------
function Win_TestTowergame:BulletAction(dwDeltaTime)
	for bname,b in pairs(self.bulletlist) do
	    if TD_BulletIndex[b.btype].picname~=nil then
    	    b.framecounter=b.framecounter+dwDeltaTime;
    	    if b.framecounter>TD_BulletIndex[b.btype].timeperfra then
    	        b.framecounter=0;
    	        b.frameindex=self:UpdateBulletMotion(b);
    	        DlgApi.SelectImageFrame(self.this, bname, b.frameindex);
    	    end
	    end
	end
end
function Win_TestTowergame:UpdateBulletMotion(b)
    local frame=b.frameindex;
    
    if frame<TD_BulletIndex[b.btype].franum-1 then
        frame=frame+1;
    else
        frame=0;
    end
    return frame;  
end

-------------------------一些其它------------------------------------


--===============
function Win_TestTowergame:SetItemPos(Item,posx,posy)
    DlgApi.SetItemPos(self.this,Item, posx*self.mapscale, posy*self.mapscale);
    
end


--===============================================================
