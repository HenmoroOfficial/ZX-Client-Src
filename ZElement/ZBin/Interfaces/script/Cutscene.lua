------------------------------------------------------------------
--  created:   2010.4.30
--  author:    SunXuewei
--
--  过场动画脚本
------------------------------------------------------------------

-- 测试用简易函数
local function CopyTable(src)
	local list = {}
 		for i,v in pairs(src) do
			list[i] = v
		end
 	return list
end

------------------------------------------------------------------
--   实际过场动画配置
------------------------------------------------------------------
Cutscene = {};

Cutscene["default"] = 
{
		idSubtitle				= 0,				--字幕id
		idCameraPath			= {0,0},				--cameraPath id列表，最多32个,依次为摄像机id和停留时间
		idDisplaySettings		= 0,				--全局显示设置id
		idBackGroundMusic		= 0,				--背景音乐id, 若禁止背景音乐，则设为-1；若播放原有背景音乐，设为0
		turnOffSound			= false,			--是否关闭音效； false:不关闭  true:关闭
		idBackGroundImage		= 0,				--背景图片id，若不使用背景图片，则设为0；
		shadowTime				= 0,				--背景图片渐变时间，单位毫秒
		
		idNPCConfig				= 0,				--刷NPC配置ID
		idPlayerConfig			= 0,				--刷Player配置ID		
}

Cutscene[1] = 
{
		idSubtitle				= 2,				
		idCameraPath			= {2925, 0},			
		idDisplaySettings		= 3,				
		idBackGroundMusic	= 0,
		turnOffSound			= false,		
		idBackGroundImage		= 0,
		shadowTime				= 1000,
		
		idNPCConfig				= 1,				--刷NPC配置ID
		idPlayerConfig			= 1,				--刷Player配置ID
}

Cutscene[2] = 
{
		idSubtitle				= 3,				--字幕id
		idCameraPath			= {3031,0,
								   3032,0,
								   1122,0,
								   },		--cameraPath id列表的索引，最多32个
		idDisplaySettings		= 33,				--全局显示设置id
		idBackGroundMusic	= -1,
		turnOffSound			= true,
		idBackGroundImage		= 0,
		shadowTime				= 1000,
		
		idNPCConfig				= 2,				--刷NPC配置ID
		idPlayerConfig			= 2,				--刷Player配置ID
}

Cutscene[4] =                        --人族
{
		idSubtitle				= 4,				
		idCameraPath			= { 3049,0,		--依次为摄像机id和停留时间
									3056,0,
									3058,0,
									3054,0,
									3062,0,
									3067,0,
									3068,0,
									3166,0, 
									3066,0,
								   },		
		idDisplaySettings		= 3,		
		idBackGroundMusic	= 2,
		turnOffSound			= true,
		idBackGroundImage		= 0,
		shadowTime				= 0,
		
		idNPCConfig				= 2,				
		idPlayerConfig			= 2,				
}

Cutscene[6] =                                --神族
{
		idSubtitle				= 7,				
		idCameraPath			= { 3187,0,		--依次为摄像机id和停留时间
									3188,0,
									3190,0,
									3191,0,
									3192,0,
									3195,0,
									3196,0,
								   },		
		idDisplaySettings		= 3,		
		idBackGroundMusic	= 3,
		turnOffSound			= true,
		idBackGroundImage		= 0,
		shadowTime				= 0,
		
		idNPCConfig				= 2,				
		idPlayerConfig			= 2,				
} 

Cutscene[8] =                                --市场演示用
{
		idSubtitle				= 8,				
		idCameraPath			= { 3250,0,		--依次为摄像机id和停留时间
									3251,0,
									3252,0,									
								   },		
		idDisplaySettings		= 3,		
		idBackGroundMusic	= -1,
		turnOffSound			= true,
		idBackGroundImage		= 0,
		shadowTime				= 0,
		
		idNPCConfig				= 2,				
		idPlayerConfig			= 2,
} 		

function Cutscene:GetCutscene(index)
	if not Cutscene["default"] then
		return
	end
	if not self[index] then
	    local cut = CopyTable(Cutscene["default"])
	    return cut
	end
	local cut = CopyTable(self[index])
	for k,v in pairs(Cutscene["default"]) do
	    if not cut[k] then
	        cut[k] = v
	    end
	end
	return cut	    
end	

------------------------------------------------------------------
--   全局显示设置配置
------------------------------------------------------------------
DisplaySettings = {};

--默认
DisplaySettings["default"] =
{
    onlyShowOnce      = false, 			--是否只显示一次，只显示一次:true  , 显示无限多次:false
    canEscClose       = true, 			--是否允许按ESC退出，允许ESC退出:true  , 不允许ESC退出:false
    disableOtherKey   = true,			--是否允许按其他按键，不允许按其他按键:true  , 允许按其他按键:false
    showUI            = false, 			--是否显示界面，显示界面:true  , 不显示界面:false
	showSubtitle	  = true,			--是否显示字幕界面，显示：true， 不显示：false
    showElsePlayers   = true, 			--是否显示其他玩家与宠物，显示:true, 不显示:false
    showSelf		  = true,			--是否显示自己，显示:true, 不显示:false
    showNPC           = true, 			--是否显示NPC，显示NPC:true, 不显示NPC:false
    showMonster       = true, 			--是否显示怪物，显示怪物:true, 不显示怪物:false
    showModelName     = false, 			--是否显示名称，显示名称:true, 不显示名称:false
    cancelAutoPath    = true, 			--是否中断自动寻径，中断自动寻径:true, 不中断自动寻径:false
	showBgImage  	  = false,			--是否一直显示背景图片，是：true； 否：false
}

function DisplaySettings:GetDisplaySettings(index)
	if not DisplaySettings["default"] then
		return
	end
	if not self[index] then
	    local display = CopyTable(DisplaySettings["default"])
	    return display
	end
	local display = CopyTable(self[index])
	for k,v in pairs(DisplaySettings["default"]) do
	    if "id"~=k and not display[k] then
	        display[k] = v
	    end
	end
	return display	    
end	
	
----------------基类----------------------------------------------------------
--城市观光类
DisplaySettings["OverlookCity"] =
{
    onlyShowOnce      	= true,
    canEscClose       	= true,
    showElsePlayers		= true,
    showSelf			= false,
    showNPC           	= true,
    showMonster       	= true,
    cancelAutoPath    	= true,
}

--战场观光类
DisplaySettings["OverlookBattlefield"] =
{
    canEscClose       	= true,
    disableOtherKey   	= false,
    showElsePlayers		= true,
    showSelf			= false,
    showNPC           	= true,
    showMonster       	= true,
    cancelAutoPath    	= true,
    showOnceInCurWorld	= false,
}

--任务类
DisplaySettings["OverlookTask"] =
{
	disableOtherKey   	= true,
	showElsePlayers		= true,
    showSelf			= false,
	cancelAutoPath     	= true,
}

DisplaySettings["OverlookTask_NPC"] =
{
	disableOtherKey   	= true,
	showElsePlayers		= false,
	cancelAutoPath     	= true,
	showNPC           	= true,
	showMonster       	= true,
}
----------------实际显示设置-------------------------------------
DisplaySettings[2925] = CopyTable(DisplaySettings["default"])
DisplaySettings[3031] = CopyTable(DisplaySettings["OverlookCity"])
DisplaySettings[3] = CopyTable(DisplaySettings["default"])


------------------------------------------------------------------
--   字幕配置
------------------------------------------------------------------
Subtitle = 
{
	[1] 	= "$5000$hello world诛仙2摄像机测试\n$10000$这个string显示10s时间...\n$5000$\n$10000$camera漫游到此结束",
	[2] 	= "$10000$显示10秒测试\n$20000$显示20s测试...",
	[3]		= "$5000$我要绕河阳城一圈~~~~~~\n$10000$今天天气好晴朗，处处好风光~~~",
	[4]="$3000$ \n$3000$神州浩土\n$3000$天地不仁\n$3000$以万物为刍狗\n$3000$自太古以来\n$3000$人类眼见周遭世界\n$3000$诸般奇异之事\n$2000$电闪雷鸣\n$2000$狂风暴雨\n$3000$又有天灾人祸\n$2000$伤亡无数\n$2000$哀鸿遍野\n$3000$遂以为九天之上\n$3000$有诸般神灵\n$3000$九幽之下\n$3000$亦是阴魂归处\n$3000$阎罗殿堂\n$3000$神仙之说\n$3000$流传于世\n$3000$时至今日\n$3000$人间修真炼道之人\n$2000$数不胜数\n$3000$修炼之法道林林总总\n$2000$俱不相同\n$3000$由之而起的门户之见\n$2000$正邪之别\n$3000$以致勾心斗角\n$2000$争伐杀戮……\n$4000$一场感天动地的旷世传说\n$7000$也由此开启了……",
	[7]="$2000$ \n$3000$混沌初开\n$2000$天地伊始\n$3000$有伏羲女娲\n$2000$共治天地\n$2000$化生万物\n$2000$正姓婚姻\n$3000$世间始得繁盛\n$2500$及至二圣飞升\n$2000$先神子民\n$2000$杂居人间\n$3000$世间人族势众\n$3000$而神裔渐微\n$3500$人神二族纷争渐起\n$4000$黄帝斩蚩尤于逐鹿\n$3000$自此神裔凋零\n$2000$一蹶不振\n$3000$后有神裔大贤\n$2000$另辟神域\n$2000$众族举迁\n$2000$封印圣城\n$2000$远避人间\n$4000$至轮回密印复开\n$4000$人神二族再次相遇\n$5000$种种纠葛亦因此展开\n$5000$身为神裔血族中坚力量\n$5000$流淌于勇士血液之中的强大战意\n$5000$亦随之升腾！",
	[8] 	= "$10000$显示10秒测试\n$20000$显示20s测试...",
	}

function Subtitle:GetSubtitle(index)
    if not self[index] then
        return Subtitle[1]
    end    
	return self[index]
end

------------------------------------------------------------------
--   背景图片配置
------------------------------------------------------------------
--背景图片文件保存在element\surfaces.pck的ingame目录下

BackGroundImage = 
{
	[1] = "black.png",
	[2] = "white.png",
}

function BackGroundImage:GetImage(index)
    if not self[index] then
        return BackGroundImage[1]
    end    
	return self[index]
end
------------------------------------------------------------------
--   背景音乐文件配置
------------------------------------------------------------------
--音乐文件保存在element\music目录下

BackGroundMusic = 
{
	[1] 	= "Music\\郭德纲相声.mp3",
	[2]		= "Music\\人族过场.mp3",
	[3]		= "Music\\神裔过场.mp3",
}

function BackGroundMusic:GetMusic(index)
    if not self[index] then
        return BackGroundMusic[1]
    end    
	return self[index]
end

------------------------------------------------------------------
--   刷NPC配置
------------------------------------------------------------------
NPCConfig = {}

--默认
NPCConfig["default"] = 
{
	npcID				= 11,		--NPC模板ID
	npcShowUpTime		= 22,		--NPC出现时间(毫秒)
	npcDisappearTime	= 33, 		--NPC消失时间(毫秒)，为0则一直存在
	npcPathID			= 44,		--NPC路径ID。NPC初始位置，朝向，均保存在此path中
	npcActionGroup		= 			--NPC动作组合
	{
		{
			timeStart	= 0,		--起始时间
			timeEnd		= 1000,		--终止时间
			isMoving	= false,	--是否移动
			isLoopPlay	= false,	--是否循环播放
			isRandPlay	= false,	--是否随机播放
			actionList 	= {11, 22, 33},			--动作ID列表
		},
		{
			timeStart	= 1000,
			timeEnd		= 9000,		
			isMoving	= false,
			isLoopPlay	= false,
			isRandPlay	= false,
			actionList 	= {44, 55, 66},			
		},
	
	},			
}

function NPCConfig:GetNPCConfig(index)
    if not self[index] then
        return NPCConfig["default"]
    end
	return self[index]
end

function NPCConfig:GetNPCActionGroup(index)
      if not self.npcActionGroup[index] then
        return self.npcActionGroup[0]
    end 
	return self.npcActionGroup[index]
end
----------------刷NPC实际配置-------------------------------------
NPCConfig[1] = CopyTable(NPCConfig["default"])


------------------------------------------------------------------
--   仅播放一次的过场动画id配置
------------------------------------------------------------------
--注：只能顺序添加，已填加的顺序不能更改，否则会出现错误
OnlyShowOnceCutscenes = 
{
	[1] = 3,
	[2] = 4,
	[3] = 5,
	[4] = 6,
	[5] = 7,
	[6] = 8,
}

function OnlyShowOnceCutscenes:GetOnlyShowOnceCutscenes(index)
    if not self[index] then
        return 0
    end    
	return self[index]
end

















