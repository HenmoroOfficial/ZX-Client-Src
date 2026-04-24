----description:This script file is for user guide.
----author: wangdongliang


--指引步骤
------步骤ID  箭头类型  步骤描述   指向的界面  指向的控件
------id（步骤ID）唯一，主要用于被指引引用，可以被多个指引引用
------type（箭头类型） 0(上)、1(下)、2(左)、3(右)
------dlg（指向的界面）通常为界面的名称，control（指向的控件）通常为控件的名称。
------另外，dlg与control也可指向不确定的界面控件（当dlg="HGForTaskTrace"时，指向任务追踪界面的某个任务的第一个子树，control的值为任务ID。
------当dlg="HGForInventory"时，指向包裹界面中某一物品，control的值为物品ID。
------若任务ID或物品ID需要多个，control填充多个ID，以逗号隔开，须程序特殊处理。当dlg="HGForNpcTalk"时，指向NPC对话界面的选项列表框的某一项，control的值为列表框的索引（从0开始），及需要的NPCID，若有多个NPC，以逗号隔开）。


HelpGuideStep = {}
 
---移动教学，特殊界面处理，不填充数据
HelpGuideStep['移动教学'] = {id = 2, type = -1, desc = "", dlg = "", control = ""}
---寻径
----任务ID三个，分别为人族，兽族，妖族
HelpGuideStep['寻径'] = {id = 3, type = 2, desc = "左键点击，寻找NPC", dlg = "HGForTaskTrace", control = "29066,29192,29623"}
---NPC对话，指向第一个选项（玩家对话选项），需特殊处理
HelpGuideStep['NPC对话-1'] = {id = 4, type = 2, desc = "左键点击，接取任务", dlg = "HGForNpcTalk", control = "0,58440,58447,58448"}
---战斗，特殊界面处理
----任务ID三个，分别为人族，兽族，妖族
HelpGuideStep['战斗-1'] = {id = 5, type = -1, desc = "", dlg = "HGForFighting", control = "29093,29206,29626"}
---装备武器
---物品ID7个，分别为人族,九黎，烈山，怀光，天华，辰皇，太昊，牵机，英招
HelpGuideStep['点击包裹'] = {id = 6, type = 1, desc = "左键点击，打开包裹", dlg = "Win_Main", control = "Btn_Inventory"}
HelpGuideStep['装备武器-2'] = {id = 7, type = 3, desc = "右键点击，装备武器", dlg = "HGForInventory", control = "23863,31882,31883,37870,37871,52770,49417,60069,60070"}
HelpGuideStep['装备武器-3'] = {id = 8, type = 3, desc = "左键点击，关闭包裹", dlg = "Win_Inventory", control = "Btn_Close"}
---技能加点
HelpGuideStep['技能加点-1'] = {id = 9, type = 1, desc = "左键点击，打开技能界面，", dlg = "Win_Main", control = "Btn_Skill"}
---人族
HelpGuideStep['技能加点-2-1'] = {id = 10, type = 2, desc = "左键点击，提高技能等级", dlg = "Win_Skill00", control = "Btn_218"}


HelpGuideStep['技能加点-3'] = {id = 11, type = 2, desc = "左键点击，关闭技能界面", dlg = "Win_Skillzx2", control = "Btn_Close"}

---使用技能
HelpGuideStep['使用技能'] = {id = 12, type = 1, desc = "左键点击，使用技能攻击怪物", dlg = "Win_QuickBar8H_1", control = "Item_2"}
----使用礼包
----HelpGuideStep['使用礼包-1'] = {id = 13, type = 3, desc = "左键点击，打开包裹", dlg = "Win_Main", control = "Btn_Inventory"}
HelpGuideStep['使用礼包-2'] = {id = 14, type = 3, desc = "右键点击或双击，打开百宝箱", dlg = "HGForInventory", control = "24347,32097,31887,37883,37884,54594,49424,60137,60190"}

HelpGuideStep['使用药品'] = {id = 15, type = 1, desc = "左键点击，使用药品", dlg = "Win_QuickBar8H_1", control = "Item_3"}

----HelpGuideStep['装备法宝-1'] = {id = 16, type = 3, desc = "左键点击，打开包裹", dlg = "Win_Main", control = "Btn_Inventory"}
HelpGuideStep['装备法宝-2'] = {id = 17, type = 3, desc = "右键点击或双击，装备法宝", dlg = "HGForInventory", control = "4703,30076,58825"}
HelpGuideStep['装备法宝-3'] = {id = 18, type = 3, desc = "左键点击拖拽，选中寒月之辉", dlg = "HGForInventory", control = "22124"}
HelpGuideStep['装备法宝-4'] = {id = 19, type = 0, desc = "拖拽至此，将寒月之辉放在法宝上", dlg = "Win_Talisman", control = "Item_3"}
  
HelpGuideStep['使用天人合一'] = {id = 20, type = 1, desc = "左键点击，开启天人合一", dlg = "Win_Talisman", control = "Btn_Zoarium"}


HelpGuideStep['炼器教学-1'] = {id = 21, type = 2, desc = "左键点击，找到炼器师", dlg = "HGForTaskTrace", control = "29834"}
HelpGuideStep['炼器教学-2'] = {id = 22, type = 2, desc = "左键点击，打开装备炼器界面", dlg = "HGForNpcTalk", control = "0,1196"}
HelpGuideStep['炼器教学-3'] = {id = 23, type = 2, desc = "右键点击，将木剑放入炼器界面装备栏", dlg = "HGForInventory", control = "55"}
HelpGuideStep['炼器教学-4'] = {id = 24, type = 2, desc = "右键点击，将炼器符放入炼器界面仙符栏", dlg = "HGForInventory", control = "685"}
HelpGuideStep['炼器教学-5'] = {id = 25, type = 2, desc = "左键点击，开始炼器，一定几率会失败，失败后装备消失", dlg = "Win_Enchase", control = "Btn_Enchase"}



HelpGuideStep['宠物-1'] = {id = 26, type = 2, desc = "左键点击", dlg = "HGForTaskTrace", control = "29837"}
HelpGuideStep['宠物-2'] = {id = 27, type = 2, desc = "左键点击", dlg = "Win_Pet_Main", control = "Btn_PetTame"}
HelpGuideStep['宠物-3'] = {id = 28, type = 2, desc = "右键点击", dlg = "HGForInventory", control = "18386"}
HelpGuideStep['宠物-4'] = {id = 29, type = 2, desc = "左键点击", dlg = "Win_Pet_Tame", control = "Btn_Confirm"}
HelpGuideStep['宠物-5'] = {id = 30, type = 2, desc = "左键点击", dlg = "Win_Pet_Tame", control = "Btn_Close"}
HelpGuideStep['宠物-6'] = {id = 31, type = 2, desc = "左键点击", dlg = "Win_MainClose", control = "Btn_Main"}
HelpGuideStep['宠物-7'] = {id = 32, type = 3, desc = "左键点击", dlg = "Win_Main", control = "Btn_All"}
HelpGuideStep['宠物-8'] = {id = 33, type = 3, desc = "左键点击", dlg = "Win_WinmainAll", control = "Btn_Pet"}
---HelpGuideStep['宠物-9'] = {id = 34, type = 3, desc = "左键点击", dlg = "Win_Main", control = "Btn_Inventory"}
HelpGuideStep['宠物-10'] = {id = 35, type = 3, desc = "右键点击", dlg = "HGForInventory", control = "18386"}
HelpGuideStep['宠物-11'] = {id = 36, type = 2, desc = "左键点击", dlg = "Win_PetPackage", control = "Btn_Summon"}

---九黎
HelpGuideStep['技能加点-2-2'] = {id = 37, type = 2, desc = "左键点击，提高技能等级", dlg = "Win_Skill33", control = "Btn_1843"}
---烈山
HelpGuideStep['技能加点-2-3'] = {id = 38, type = 2, desc = "左键点击，提高技能等级", dlg = "Win_Skill39", control = "Btn_1883"}
---怀光
HelpGuideStep['技能加点-2-4'] = {id = 39, type = 2, desc = "左键点击，提高技能等级", dlg = "Win_Skill45", control = "Btn_2411"}
---天华
HelpGuideStep['技能加点-2-5'] = {id = 40, type = 2, desc = "左键点击，提高技能等级", dlg = "Win_Skill51", control = "Btn_2459"}
---辰皇
HelpGuideStep['技能加点-2-6'] = {id = 41, type = 2, desc = "左键点击，提高技能等级", dlg = "Win_Skill56", control = "Btn_3997"}
---太昊
HelpGuideStep['技能加点-2-7'] = {id = 42, type = 2, desc = "左键点击，提高技能等级", dlg = "Win_Skill96", control = "Btn_3455"}
---牵机
HelpGuideStep['技能加点-2-8'] = {id = 43, type = 2, desc = "左键点击，提高技能等级", dlg = "Win_Skill102", control = "Btn_4771"}
---英招
HelpGuideStep['技能加点-2-9'] = {id = 44, type = 2, desc = "左键点击，提高技能等级", dlg = "Win_Skill108", control = "Btn_4838"}
---NPC诛仙任务对话，指向第一个选项（玩家对话选项），需特殊处理
HelpGuideStep['NPC对话-2'] = {id = 45, type = 2, desc = "左键点击，与NPC对话", dlg = "HGForNpcTalkMain", control = "0"}

HelpGuideStep['战斗-2'] = {id = 46, type = 1, desc = "左键点击，利用普通攻击击杀怪物", dlg = "Win_QuickBar8H_1", control = "Item_1"}



  
 
function HelpGuideStep:GetSelf()
	return self;
end


--指引
--------指引ID  失败重复触发    等级下限   等级上限   指引步骤列表
--------指引ID唯一，且与c++代码中的枚举一一对应，此处没有在全局脚本中定义的枚举
--------步骤ID以逗号分隔，并且须按指引的顺序填写


HelpGuideContent = {}

--移动教学
HelpGuideContent['移动教学'] = { id = 0, isrepeate = false, level_min = 1, level_max = 1, steps = "2"}
--寻径
HelpGuideContent['寻径'] = { id = 1, isrepeate = false, level_min = 1, level_max = 1, steps = "3"}
--NPC对话
HelpGuideContent['NPC对话'] = { id = 2, isrepeate = false, level_min = 1, level_max = 1, steps = "4,45"}
--战斗
HelpGuideContent['战斗'] = { id = 3, isrepeate = false, level_min = 2, level_max = 2, steps = "5,46"}
--装备武器
HelpGuideContent['装备武器'] = { id = 4, isrepeate = false, level_min = 3, level_max = 3, steps = "6,7,8"}
--技能加点（人族）
HelpGuideContent['技能加点-1'] = { id = 5, isrepeate = false, level_min = 4, level_max = 4, steps = "9,10,11"}
--技能加点（九黎）
HelpGuideContent['技能加点-2'] = { id = 13, isrepeate = false, level_min = 4, level_max = 4, steps = "9,37,11"}
--技能加点（烈山）
HelpGuideContent['技能加点-3'] = { id = 14, isrepeate = false, level_min = 4, level_max = 4, steps = "9,38,11"}
--技能加点（怀光）
HelpGuideContent['技能加点-4'] = { id = 15, isrepeate = false, level_min = 4, level_max = 4, steps = "9,39,11"}
--技能加点（天华）
HelpGuideContent['技能加点-5'] = { id = 16, isrepeate = false, level_min = 4, level_max = 4, steps = "9,40,11"}
--技能加点（辰皇）
HelpGuideContent['技能加点-6'] = { id = 17, isrepeate = false, level_min = 4, level_max = 4, steps = "9,41,11"}
--技能加点（太昊）
HelpGuideContent['技能加点-7'] = { id = 18, isrepeate = false, level_min = 4, level_max = 4, steps = "9,42,11"}
--技能加点（牵机）
HelpGuideContent['技能加点-8'] = { id = 19, isrepeate = false, level_min = 4, level_max = 4, steps = "9,43,11"}
--技能加点（英招）
HelpGuideContent['技能加点-9'] = { id = 20, isrepeate = false, level_min = 4, level_max = 4, steps = "9,44,11"}
--使用技能
HelpGuideContent['使用技能'] = { id = 6, isrepeate = false, level_min = 4, level_max = 4, steps = "12"}
--使用礼包
HelpGuideContent['使用礼包'] = { id = 7, isrepeate = false, level_min = 5, level_max = 5, steps = "6,14"}
--使用药品
HelpGuideContent['使用药品'] = { id = 8, isrepeate = false, level_min = 6, level_max = 20, steps = "15"}
--装备法宝
HelpGuideContent['装备法宝'] = { id = 9, isrepeate = false, level_min = 10, level_max = 10, steps = "6,17,18,19"}
--使用天人合一
HelpGuideContent['使用天人合一'] = { id = 10, isrepeate = false, level_min = 10, level_max = 15, steps = "20"}
--炼器教学一
HelpGuideContent['炼器教学'] = { id = 11, isrepeate = false, level_min = 15, level_max = 20, steps = "21,22,23,24,25"}
--宠物一
HelpGuideContent['宠物'] = { id = 12, isrepeate = false, level_min = 20, level_max = 25, steps = "26,27,28,29,30,31,32,33,6,35,36"}



function HelpGuideContent:GetSelf()
	return self;
end







-- 目的指引
-- id: ID
-- type: 类型（六种，分别是：赚钱，升级，泡妞，提升实力，不无聊，花钱）
-- name: 名称
-- iconpath: 图标路径
-- validday：活动时间日期（周--周日）
-- validtime：时间段
-- levelreq: 等级要求
-- needreborn：是否要求飞升
-- permitcount：次数限制
-- cond：条件
-- npc: 相关NPC
-- taskid:	如果不是任务，填0
-- istasknpc: 相关NPC处是否读取taskid对应的任务NPC:0,1
-- descnpc:	如果不读取任务npc,手动填写描述
-- descmapid: 如果不读取任务npc,手动填写寻径地图id
-- descpos: 如果不读取任务npc,手动填写寻径坐标
-- istransport: 是否使用飞天传送,是则读取task的传送功能
-- starlevel：星级（11个星级，分别对应1,15,30,45....150级）
-- desc:内容描述


AimGuideData = {}
AimGuideData['逐波圣灵'] = 
			{ 
				id=101, type="0,1,0,0,1,1", name="逐波圣灵", iconpath="逐波圣灵.dds", validday="周一-周日", validtime="11:50-12:00", levelreq="45-160", needreborn=false, permitcount=1, cond="^ffcb4a10金", 
				npc="上古奇人", taskid="14607", istasknpc=false, descnpc="上古奇人", descmapid=401, descpos="433,482,389", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5", 
				desc="^ffffff       每天11：50分在河阳^ffcb4a上古奇人^ffffff处花费^ffcb4a10金^ffffff购买神兽令牌，传送到流波海底观看神兽逐波竞速。活动结束可从夺魁神兽处获得^ffcb4a20个太极金丹^ffffff的奖励。若所购令牌刚好为夺魁神兽，还可获得神兽所赐的^ffcb4a100个太极金丹^ffffff，^ffcb4a200个造化珠^ffffff奖励。\r"
			}
AimGuideData['彻夜不眠'] = 
			{ 
				id=117, type="1,1,0,1,0,0", iconpath="彻夜不眠.dds", name="彻夜不眠", 
				validtime="0:00-24:00", levelreq="60-150", needreborn=false, permitcount=-1, cond=" ", 
	      validday="周一-周日", npc="60级以上各地名医", taskid="11586,11588,11590,11592,11595,11598,11501,11618,11635,11638,11647,11654,11674,11678,11681,11739,11746,11754,11783,11791,11794,11796,11798,11800",
		    istasknpc=false, descnpc="60级以上各地名医", descmapid=-1, descpos="9999,0,9999", istransport=false, starlevel="5,5,5,5,5,5,5,5,5,5,5",
				desc="^ffffff       60级之后，可以在每张地图的医师处领取对应等级的^ffcb4a彻夜不眠^ffffff任务。\r       任务为计时形式，根据杀怪数量计算获取的经验。\r       可以在时间未结束时直接提交任务获取经验，或者等待任务时间结束，自动获得相应的经验奖励。"
			}
AimGuideData['师门英雄帖'] = 
			{ 
				id=118, type="0,1,0,1,1,0", iconpath="师门英雄帖.dds", name="师门英雄帖",
				validtime="0:00-24:00", levelreq="30-160", needreborn=false, permitcount=-1, cond=" ", 
				validday="周一-周日",  npc="本职业门派使者", taskid="3162,3159,3165,3168,11682,3174,3171,3177,3180,11689,3186,3183,3189,3192,11696,3858,3855,3861,3865,11703,5628,5538,5625,5631,11710,5652,5634,5637,5655,11717,7614,7611,7617,7620,11724,7660,7657,7663,7666,11734",
				istasknpc=false, descnpc="本职业门派使者", descmapid=-1, descpos="9999,0,9999", istransport=false, starlevel="5,5,5,5,5,5,5,5,5,5,5",
				desc="^ffffff       从天音开始，当你的本门贡献度足够高时，可以在本门的门派使者那里领取不同档次的英雄帖。（每天可以领取一张）\r       完成英雄帖上的任务，可以换取不同的发光英雄帖。凭借发光英雄帖在门派使者处换取大量经验。（只要身上还有金字英雄帖，每天可重复完成此任务）"
			}			
AimGuideData['钓鱼大赛'] = 
			{ 
				id=4, type="0,0,1,0,1,0", iconpath="钓鱼大赛.dds", name="钓鱼大赛",
				validtime="18:00-19:00", levelreq="45-160", needreborn=false, permitcount=1, cond=" ", 
				validday="周一-周日",  npc="云游四海的渔夫", taskid="5556",
				istasknpc=false, descnpc="云游四海的渔夫", descmapid=401, descpos="401,0,457", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
				desc="^ffffff       在流波举行的钓鱼大赛，玩家先用报名卷在云游四海的渔夫处换取^ffcb4a鱼饵^ffffff，然后赶往流波参加大赛。"
			}
AimGuideData['戾气危机'] = 
			{ 
				id=5, type="0,1,0,1,1,0", iconpath="戾气危机.dds", name="戾气危机",
				validtime="18:00-20:00", levelreq="75-160", needreborn=false, permitcount=1, cond=" ", 
				validday="周一-周日",  npc="陆雪琪", taskid="12717",
				istasknpc=false, descnpc="陆雪琪", descmapid=2, descpos="403,75,-192", istransport=false, starlevel="5,5,5,5,5,5,5,5,5,5,5",
				desc="^ffffff       青云山方向突然间天现异象，从^ffcb4a青云 陆雪琪^ffffff处得知，青云后山祖师祠堂处设置了^ffcb4a三根阵法石柱^ffffff，用以镇压戾气，如今石柱多有损耗，阵法已经逐渐失效。请大家帮助修复三根石柱，恢复阵法的力量，可获得^ffcb4a25个太极金丹^ffffff奖励。"
			}
AimGuideData['新科试炼'] = 
			{ 
				id=105, type="0,1,0,1,1,0", iconpath="新科试炼.dds", name="新科试炼",
				validtime="19:00-19:15", levelreq="1-160", needreborn=false, permitcount=1, cond=" ", 
				validday="周一-周日",  npc="系统定时开启", taskid="12868",
				istasknpc=false, descnpc="系统定时开启", descmapid=-1, descpos="9999,0,9999", istransport=false, starlevel="5,5,5,5,5,5,5,5,5,5,5",
				desc="^ffffff       每天^ffcb4a19：00^ffffff整，玩家会接到^ffcb4a新科试炼^ffffff活动的邀请。接受邀请，参与试炼。获得经验奖励的掌握更多知识。\r新科试炼^ffcb4a前10名^ffffff都可以获得^ffcb4a轩辕通宝^ffffff的奖励。并且可以获得稀有的试炼标志称号。"
			}
AimGuideData['修真总动员'] = 
			{ 
				id=106, type="0,0,0,0,1,0", iconpath="修真总动员.dds", name="修真总动员",
				validtime="19:00-22:00", levelreq="80-160", needreborn=false, permitcount=1, cond=" ", 
				validday="周五",  npc="阿福", taskid="11339",
				istasknpc=false, descnpc="阿福", descmapid=2, descpos="179,0,108", istransport=false, starlevel="5,5,5,5,5,5,5,5,5,5,5",
				desc="^ffffff       在^ffcb4a青云阿福^ffffff处领取任务，完成任务需要5个用^ffcb4a虎魄冰晶^ffffff换取的棒棒糖，成功后奖励200^ffcb4a乾坤通宝^ffffff。"
			}
AimGuideData['追风圣灵'] = 
			{ 
				id=107, type="0,0,0,0,1,0", iconpath="追风圣灵.dds", name="追风圣灵",
				validtime="19:20-19:30", levelreq="90-160", needreborn=false, permitcount=1, cond=" ", 
				validday="周一-周日",  npc="上古奇人", taskid="14989",
				istasknpc=false, descnpc="上古奇人", descmapid=401, descpos="433,482,389", istransport=false, starlevel="5,5,5,5,5,5,5,5,5,5,5",
				desc="^ffffff       每天19：20分在河阳^ffcb4a上古奇人^ffffff处花费^ffcb4a10金^ffffff购买神兽令牌，传送到焚香谷观看神兽追风竞速。活动结束可从夺魁神兽处获得^ffcb4a20个太极金丹^ffffff的奖励。若所购令牌刚好为夺魁神兽，还可获得神兽所赐的^ffcb4a100个太极金丹^ffffff，^ffcb4a200个造化珠^ffffff奖励。\r^ff0000注：神兽追风竞速过程中需全程关注，否则可能发生神兽走失事件。"
			}
AimGuideData['天下会武'] = 
			{ 
				id=108, type="0,1,1,1,1,0", iconpath="天下会武.dds", name="天下会武",
				validtime="19:30-21:10", levelreq="90-134", needreborn=false, permitcount=1, cond=" ", 
				validday="周一、二",  npc="紫微道光元帅", taskid="9620,9629,10529,9638,9647,10530",
				istasknpc=false, descnpc="紫微道光元帅", descmapid=401, descpos="514,0,349", istransport=false, starlevel="5,5,5,5,5,5,5,5,5,5,5",
				desc="^ffffff       在^ffcb4a河阳紫微帝君^ffffff处领取天下会武的任务，使用^ffcb4a六道法莲^ffffff可自由选择阵营。\r       完成会武可以获得大量经验。"
			}
AimGuideData['金丹之术'] = 
			{ 
				id=109, type="0,0,0,1,1,1", iconpath="金丹之术.dds", name="金丹之术",
				validtime="19:30-21:30", levelreq="80-160", needreborn=false, permitcount=1, cond=" ", 
				validday="周一-周日",  npc="抱朴子", taskid="13091",
				istasknpc=false, descnpc="抱朴子", descmapid=2, descpos="96,64,203", istransport=false, starlevel="5,5,5,5,5,5,5,5,5,5,5",
				desc="^ffffff       在青云举行的大型炼丹活动，玩家在^ffcb4a青云 抱朴子^ffffff处领取前置任务。\r 完成任务后玩家可以获得^ffcb4a碎雪星芒^ffffff及^ffcb4a太极金丹^ffffff奖励。\r       50个碎雪星芒可在陆雪琪处兑换道具^ffcb4a雪之星^ffffff，可以对项链（天罪，龙啸，龙骧，神道，神罚，神藏）进行强化。"
			}
AimGuideData['星宿北斗'] = 
			{ 
				id=111, type="0,1,0,1,1,0", iconpath="星宿北斗.dds", name="星宿北斗",
				validtime="20:30-22:30", levelreq="30-160", needreborn=false, permitcount=1, cond=" ", 
				validday="周三四六",  npc="星宿挑战使者", taskid="5469,5470,5471,5472,9046,9047,9048",
				istasknpc=false, descnpc="星宿挑战使者", descmapid=401, descpos="500,0,382", istransport=false, starlevel="5,5,5,5,5,5,5,5,5,5,5",
				desc="^ffffff       在各自等级段对应的地图可找挑战使者接受任务，按要求击杀对应星宿怪，击杀七个星宿怪可得到最高奖励。"
			}
AimGuideData['八极战魂'] = 
   { 
    id=112, type="0,1,1,1,1,1", iconpath="八极战魂.dds", name="八极战魂", 
    validtime="0:00-24:00", levelreq="1-160", needreborn=false, permitcount=1, cond=" ", 
    validday="周一-周日", npc="封印石", taskid="22314",
    istasknpc=false, descnpc="封印石", descmapid=2, descpos="93,64,208", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       完成^ffcb4a国家宝藏^ffffff或者^ffcb4a天诛谕令^ffffff任务后，有几率获得任务道具^ffcb4a启玄印^ffffff，可以用来破除位于青云的^ffcb4a八极镇石^ffffff封印，并换得^ffcb4a八极元珠^ffffff，当封印石收集足够数量的^ffcb4a启玄印^ffffff后，封印效果将消失，可以召唤封印石内的战魂。\r       战魂被消灭后可于^ffcb4a青云 混元真君^ffffff处用^ffcb4a八极元珠^ffffff领取奖励"}
AimGuideData['天行者'] = 
   { 
    id=113, type="0,1,0,1,0,0", iconpath="天行者.dds", name="天行者", 
    validtime="0:00-24:00", levelreq="45-160", needreborn=false, permitcount=1, cond=" ", 
    validday="周一-周日", npc="天行者", taskid="12193",
    istasknpc=false, descnpc="天行者", descmapid=401, descpos="430,511,402", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       河阳的^ffcb4a天行者^ffffff提供获得海量经验的绝佳机会。\r 驾驭^ffcb4a飞剑^ffffff，与天行者相会在蔚蓝天空。"}
AimGuideData['行天'] = 
   { 
    id=114, type="0,1,0,1,0,1", iconpath="行天.dds", name="行天", 
    validtime="0:00-24:00", levelreq="45-160", needreborn=false, permitcount=1, cond=" ", 
    validday="周一-周日", npc="天行者", taskid="12844",
    istasknpc=false, descnpc="天行者", descmapid=401, descpos="430,511,402", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       每天在完成天行者的任务后，会获得道具^ffcb4a“行天之证”^ffffff，凭借“行天之证”并交纳^ffcb4a25金币^ffffff可以在天行者处领取后续高经验奖励任务任务^ffcb4a“行天”^ffffff。\r       完成任务可以获得^ffcb4a50个太极金丹^ffffff的经验奖励。"}
AimGuideData['忠烈碑'] = 
   { 
    id=115, type="0,1,0,1,0,0", iconpath="忠烈碑.dds", name="忠烈碑", 
    validtime="0:00-24:00", levelreq="50-160", needreborn=false, permitcount=1, cond=" ", 
    validday="周一-周日", npc="忠烈碑", taskid="2237",
    istasknpc=false, descnpc="忠烈碑", descmapid=4, descpos="154,0,369", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       每天可以在流波的^ffcb4a忠烈碑^ffffff进行祭奠。\r       每次需要2组奇珍，完成祭奠任务后获得大量经验"}
AimGuideData['景仰先贤'] = 
   { 
    id=116, type="0,1,0,1,0,1", iconpath="景仰先贤.dds", name="景仰先贤", 
    validtime="0:00-24:00", levelreq="120-160", needreborn=false, permitcount=1, cond=" ", 
    validday="周一-周日", npc="沈墨儒", taskid="12847",
    istasknpc=false, descnpc="沈墨儒", descmapid=401, descpos="389,0,353", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       每天可以在^ffcb4a河阳沈墨儒^ffffff处交纳^ffcb4a15个金币^ffffff后领取^ffcb4a“景仰先贤”^ffffff的高经验奖励任务。\r       完成任务可以获得^ffcb4a25个太极金丹^ffffff的经验奖励。"}
AimGuideData['破凶咒'] = 
   { 
    id=119, type="0,1,0,1,1,0", iconpath="破凶咒.dds", name="破凶咒", 
    validtime="0:00-24:00", levelreq="30-160", needreborn=false, permitcount=1, cond=" ", 
    validday="周一-周日", npc="传功使者", taskid="8667",
    istasknpc=false, descnpc="传功使者", descmapid=401, descpos="500,0,367", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       传功使者发布的破除凶咒任务。\r       杀死分布在地图上的^ffcb4a凶咒怪^ffffff，即可获得大量经验。"}
AimGuideData['建功立事'] = 
   { 
    id=120, type="0,1,0,1,1,1", iconpath="建功立事.dds", name="建功立事", 
    validtime="0:00-24:00", levelreq="50-160", needreborn=false, permitcount=1, cond=" ", 
    validday="周一-周日", npc="帮会管理员", taskid="12856",
    istasknpc=false, descnpc="帮会管理员", descmapid=401, descpos="496,0,342", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       已经加入帮派的成员每天可以在河阳的^ffcb4a帮派管理员方海平^ffffff处花费^ffcb4a20金^ffffff领取任务。\r       完成一系列委托。最终可以获得^ffcb4a40个太极金丹^ffffff的经验奖励。"}
AimGuideData['宝物'] = 
   { 
    id=121, type="0,1,0,1,0,1", iconpath="宝物.dds", name="宝物", 
    validtime="0:00-24:00", levelreq="110-160", needreborn=false, permitcount=1, cond=" ", 
    validday="周一-周日", npc="吴民财", taskid="12862",
    istasknpc=false, descnpc="吴民财", descmapid=401, descpos="382,0,316", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       在河阳吴民财处领取委托任务，需要支付^ffcb4a15金^ffffff。\r       完成任务后获得^ffcb4a30个太极金丹^ffffff的经验奖励。"}
AimGuideData['国家宝藏'] = 
   { 
    id=123, type="1,0,0,0,0,0", iconpath="国家宝藏.dds", name="国家宝藏", 
    validtime="0:00-24:00", levelreq="60-160", needreborn=false, permitcount=1, cond=" ", 
    validday="周一-周日", npc="天行者", taskid="12310",
    istasknpc=false, descnpc="天行者", descmapid=401, descpos="430,511,402", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       按照天行者的要求杀死各地的云游天将。\r       回报天行者获取大量金钱。"}
AimGuideData['造化之悟'] = 
   { 
    id=124, type="0,0,1,1,1,0", iconpath="造化之悟.dds", name="造化之悟", 
    validtime="0:00-24:00", levelreq="135-160", needreborn=false, permitcount=-1, cond=" ", 
    validday="周一-周日", npc="轩辕祖师", taskid="19260",
    istasknpc=false, descnpc="轩辕祖师", descmapid=2, descpos="41,45,-433", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       九里山垓下古战场，你是辅佐汉王刘邦，一匡天下，开创百年基业，还是救援楚王项羽，力挽狂澜，成就世间霸王？问鼎乾坤，等待你的抉择……（要求飞升角色）"}
AimGuideData['楚汉军魂'] = 
   { 
    id=202, type="0,1,1,1,1,0", iconpath="楚汉军魂.dds", name="楚汉军魂", 
    validtime="13:00-14:30", levelreq="15-160", needreborn=false, permitcount=10, cond=" ", 
    validday="周六、日", npc="太虚幻境仙官", taskid="1",
    istasknpc=false, descnpc="太虚幻境仙官", descmapid=401, descpos="488,0,326", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       九里山垓下古战场，你是辅佐汉王刘邦，一匡天下，开创百年基业，还是救援楚王项羽，力挽狂澜，成就世间霸王？问鼎乾坤，等待你的抉择……（要求飞升角色）"}
AimGuideData['炎狱'] = 
   { 
    id=205, type="0,0,1,1,1,0", iconpath="炎狱.dds", name="炎狱", 
    validtime="0:00-24:00", levelreq="90-104", needreborn=false, permitcount=10, cond=" ", 
    validday="每天", npc="诛仙战神", taskid="1",
    istasknpc=false, descnpc="诛仙战神", descmapid=401, descpos="519,0,344", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，此地烈火熊熊，进入则身受热浪蒸腾之苦，在其中历练可大幅度提升自身修为。（要求未飞升角色）"}
AimGuideData['武围'] = 
   { 
    id=206, type="0,0,0,1,1,0", iconpath="武围.dds", name="武围", 
    validtime="0:00-24:00", levelreq="105-119", needreborn=false, permitcount=10, cond=" ", 
    validday="每天", npc="诛仙战神", taskid="1",
    istasknpc=false, descnpc="诛仙战神", descmapid=401, descpos="519,0,344", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，此处机关遍地，危机重重，在其中历练可大幅度提升自身修为。（要求未飞升角色）"}
AimGuideData['界河'] = 
   { 
    id=207, type="0,0,0,1,1,0", iconpath="界河.dds", name="界河", 
    validtime="0:00-24:00", levelreq="120-134", needreborn=false, permitcount=10, cond=" ", 
    validday="每天", npc="诛仙战神", taskid="1",
    istasknpc=false, descnpc="诛仙战神", descmapid=401, descpos="519,0,344", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，双方以河为界，斗智斗勇，在其中历练可大幅度提升自身修为。（要求未飞升角色）"}
AimGuideData['天弈'] = 
   { 
    id=208, type="0,0,0,1,1,0", iconpath="天弈.dds", name="天弈", 
    validtime="0:00-24:00", levelreq="135-150", needreborn=false, permitcount=10, cond=" ", 
    validday="每天", npc="诛仙战神", taskid="1",
    istasknpc=false, descnpc="诛仙战神", descmapid=401, descpos="519,0,344", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，内呈棋盘之势，步步惊心，在其中历练可大幅度提升自身修为。（要求未飞升角色）"}
AimGuideData['幻世炎狱'] = 
   { 
    id=209, type="0,0,0,1,1,0", iconpath="幻世炎狱.dds", name="幻世炎狱", 
    validtime="12:00-24:00", levelreq="90-104", needreborn=false, permitcount=10, cond=" ", 
    validday="每天", npc="诛仙战神", taskid="1",
    istasknpc=false, descnpc="诛仙战神", descmapid=401, descpos="519,0,344", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，此地烈火熊熊，进入则身受热浪蒸腾之苦，在其中历练可大幅度提升自身修为。（要求飞升角色）"}
AimGuideData['幻世武围'] = 
   { 
    id=210, type="0,0,0,1,1,0", iconpath="幻世武围.dds", name="幻世武围", 
    validtime="12:00-24:00", levelreq="105-119", needreborn=false, permitcount=10, cond=" ", 
    validday="每天", npc="诛仙战神", taskid="1",
    istasknpc=false, descnpc="诛仙战神", descmapid=401, descpos="519,0,344", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，此处机关遍地，危机重重，在其中历练可大幅度提升自身修为。（要求飞升角色）"}
AimGuideData['幻世界河'] = 
   { 
    id=211, type="0,0,0,1,1,0", iconpath="幻世界河.dds", name="幻世界河", 
    validtime="12:00-24:00", levelreq="120-134", needreborn=false, permitcount=10, cond=" ", 
    validday="每天", npc="诛仙战神", taskid="1",
    istasknpc=false, descnpc="诛仙战神", descmapid=401, descpos="519,0,344", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，双方以河为界，斗智斗勇，在其中历练可大幅度提升自身修为。（要求飞升角色）"}
AimGuideData['幻世天弈'] = 
   { 
    id=212, type="0,0,0,1,1,0", iconpath="幻世天弈.dds", name="幻世天弈", 
    validtime="12:00-24:00", levelreq="135-160", needreborn=false, permitcount=10, cond=" ", 
    validday="每天", npc="诛仙战神", taskid="1",
    istasknpc=false, descnpc="诛仙战神", descmapid=401, descpos="519,0,344", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，内呈棋盘之势，步步惊心，在其中历练可大幅度提升自身修为。（要求飞升角色）"}
AimGuideData['草庙村'] = 
   { 
    id=213, type="0,0,0,1,1,0", iconpath="草庙村.dds", name="草庙村", 
    validtime="0:00-24:00", levelreq="30-89", needreborn=false, permitcount=3, cond=" ", 
    validday="每天", npc="时空幻境管理员", taskid="1",
    istasknpc=false, descnpc="时空幻境管理员", descmapid=401, descpos="491,0,329", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       宁静的草庙村风云诡谲，所有的传说都将从这里开始。你回到梦开始的地方，重拾曾经的感动，一切一切须在此开始，也终须在此了结。"}
AimGuideData['真武剑阁'] = 
   { 
    id=214, type="1,0,1,1,1,0", iconpath="真武剑阁.dds", name="真武剑阁", 
    validtime="0:00-24:00", levelreq="75-119", needreborn=false, permitcount=3, cond=" ", 
    validday="每天", npc="时空幻境管理员", taskid="1",
    istasknpc=false, descnpc="时空幻境管理员", descmapid=401, descpos="491,0,329", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       真武大帝藏剑的宫殿，相传遗留下诸多的真武心解，更有诸多上古神兵遗留于此，是三界修士历练寻珍的不二宝地。"}
AimGuideData['天帝宝库'] = 
   { 
    id=215, type="0,0,1,1,1,0", iconpath="天地宝库.dds", name="天帝宝库", 
    validtime="0:00-24:00", levelreq="120-160", needreborn=false, permitcount=3, cond=" ", 
    validday="每天", npc="时空幻境管理员", taskid="1",
    istasknpc=false, descnpc="时空幻境管理员", descmapid=401, descpos="491,0,329", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       天帝宝库藏有太初神物，但其间禁制重重，稍有不慎便万劫难复。如今又到了神物出世时节，谁人能够获得太初神宝，谁又是宝库的应缘之人?（要求飞升角色）"}
AimGuideData['家族副本'] = 
   { 
    id=216, type="0,1,1,0,1,0", iconpath="建功立事.dds", name="家族副本", 
    validtime="0:00-24:00", levelreq="20-160", needreborn=false, permitcount=3, cond=" ", 
    validday="每天", npc="时空幻境管理员", taskid="1",
    istasknpc=false, descnpc="时空幻境管理员", descmapid=401, descpos="491,0,329", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       幻月洞府乃青云门禁地，为重振青云威望，门中弟子陆雪琪勇闯幻月洞府，谁能协她一臂之力？（需级别为20级以上的家族成员）"}
AimGuideData['鸿蒙试炼'] = 
   { 
    id=217, type="0,0,1,0,1,0", iconpath="天行者.dds", name="鸿蒙试炼", 
    validtime="0:00-24:00", levelreq="120-160", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="武德星君", taskid="1",
    istasknpc=false, descnpc="武德星君", descmapid=401, descpos="474,0,302", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       鸿蒙之地，神秘难测，内有各种珍奇异宝，也有无数恶灵宵小，带上你的队友，挑战一下自己的实力吧！（需级别为120级以上飞升角色，建议4人以上）"}
AimGuideData['焚香谷副本'] = 
   { 
    id=218, type="1,1,1,1,1,0", iconpath="演武阵.dds", name="焚香谷副本", 
    validtime="0:00-24:00", levelreq="60-160", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="八荒来使", taskid="1",
    istasknpc=false, descnpc="八荒来使", descmapid=401, descpos="487,489,290", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       隐匿数年的焚香、太昊重现世间，一时之间流言纷纷。传说谁能破解八荒玄火阵之谜，谁就能称霸世间！修真者们聚集起来：前往焚香谷的中心玄火坛，寻找八荒玄火阵！（需级别为60级以上角色，建议10人以上）"}
AimGuideData['梦境河阳城'] = 
   { 
    id=219, type="0,1,0,1,0,0", iconpath="彻夜不眠.dds", name="梦境河阳城", 
    validtime="0:00-24:00", levelreq="30-160", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="诛仙古卷", taskid="1",
    istasknpc=false, descnpc="诛仙古卷", descmapid=401, descpos="433,0,425", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       梦境河阳城乃平行现今世界的另一个时空之物，通过诛仙古卷可前往探索。完成其居民心愿可获得大量奖励。（需要级别为30级以上角色，建议3人以上）"}
AimGuideData['神武镇元塔'] = 
   { 
    id=220, type="0,1,0,1,0,0", iconpath="忠烈碑.dds", name="神武镇元塔", 
    validtime="0:00-24:00", levelreq="30-160", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="三官大帝", taskid="1",
    istasknpc=false, descnpc="三官大帝", descmapid=401, descpos="476,0,300", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       30级以上玩家均可参与的单人闯关副本，副本内全程禁食，仅有3次复活机会，宠物被自动召唤。首次闯关成功将会获得大量奖励，每日根据闯关数量也将给予丰厚的累积奖励。"}
AimGuideData['蛙王'] = 
   { 
    id=632, type="0,0,0,1,0,0", iconpath="蛙王.dds", name="蛙王", 
    validtime="0:00-24:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="蛙王", taskid="1",
    istasknpc=false, descnpc="蛙王", descmapid=401, descpos="696,0,308", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       受兽妖妖气影响，变成了半妖的蛙类，通体红色，形态凶恶，以妖气将其附近的青蛙都制住，且不时出来伤人。"}
AimGuideData['悍匪'] = 
   { 
    id=633, type="0,0,0,1,0,0", iconpath="悍匪.dds", name="悍匪", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="悍匪", taskid="1",
    istasknpc=false, descnpc="悍匪", descmapid=401, descpos="131,0,634", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       土匪小头目，原为名门子弟，因品行不端被逐出，从此加入土匪一列，其功力高于一般匪众，受命于匪首，时常前往恶虎岗巡视。"}
AimGuideData['神火教长老'] = 
   { 
    id=634, type="0,0,0,1,0,0", iconpath="神火教长老.dds", name="神火教长老", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="神火教长老", taskid="1",
    istasknpc=false, descnpc="神火教长老", descmapid=401, descpos="740,0,165", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       神火教长老，其教义推崇火神，长老功力高强，在神火教中颇有威望，仅次于其教教主。护教之心甚笃，绝不放过任何闯入神火教坛的人。"}
AimGuideData['金刚'] = 
   { 
    id=635, type="0,0,0,1,0,0", iconpath="金刚.dds", name="金刚", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="金刚", taskid="1",
    istasknpc=false, descnpc="金刚", descmapid=2, descpos="-63,0,34", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       一只神奇的棕毛大猩猩，体高力大，长年长于山野，奔跑迅速，有着不为人知的往事。"}
AimGuideData['神火魔祖'] = 
   { 
    id=636, type="0,0,0,1,0,0", iconpath="神火魔祖.dds", name="神火魔祖", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="神火魔祖", taskid="1",
    istasknpc=false, descnpc="神火魔祖", descmapid=2, descpos="-445,0,-363", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       神火教教主，其教义推崇火神，教主功力非同一般，世人少有匹敌者，若有人闯入神火教圣坛被其发现，通常凶多吉少。"}
AimGuideData['巨熊妖'] = 
   { 
    id=637, type="0,0,0,1,0,0", iconpath="巨熊妖.dds", name="巨熊妖", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="巨熊妖", taskid="1",
    istasknpc=false, descnpc="巨熊妖", descmapid=3, descpos="-329,0,-24", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       棕色的熊妖，力大无穷，奔跑迅速，生性既猛且蠢，遇到人便穷追猛扑，一掌可将人畜拍成肉饼。"}
AimGuideData['噬血恶僧'] = 
   { 
    id=638, type="0,0,0,1,0,0", iconpath="噬血恶僧.dds", name="噬血恶僧", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="噬血恶僧", taskid="1",
    istasknpc=false, descnpc="噬血恶僧", descmapid=3, descpos="-293,312,304", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       遥远国度而来的邪教僧人，坚信只有食用人肉才能长生不老，平日躲藏在众僧人之间，唯有寻找活人食用时方现出真身，已有多名修真者欲将其除去反为其所害。"}
AimGuideData['骷髅将军'] = 
   { 
    id=639, type="0,0,0,1,0,0", iconpath="骷髅将军.dds", name="骷髅将军", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="骷髅将军", taskid="1",
    istasknpc=false, descnpc="骷髅将军", descmapid=3, descpos="125,0,346", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       此物生前本为修道之人，因死后葬身之处为妖气所腐蚀，并赋予其强烈的杀意，从此变为恶灵，统领众骷髅魔兵，祸害人间。"}
AimGuideData['渔人蛛'] = 
   { 
    id=640, type="0,0,0,1,0,0", iconpath="渔人蛛.dds", name="渔人蛛", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="渔人蛛", taskid="1",
    istasknpc=false, descnpc="渔人蛛", descmapid=4, descpos="-159,0,394", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       凶残的水生蜘蛛，毒性巨大，爬行速度极快，擅长猎杀其他生物，即使不用毒液也能轻松捕获猎物。"}
AimGuideData['海盗船长'] = 
   { 
    id=641, type="0,0,0,1,0,0", iconpath="海盗船长.dds", name="海盗船长", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="海盗船长", taskid="1",
    istasknpc=false, descnpc="海盗船长", descmapid=4, descpos="10,65,-229", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       长年漂泊在海上的海盗头目，疯狂的海上掠夺者，擅长水陆两栖作战，对财宝十分贪婪，偶尔会回到流波海域补给船只。"}
AimGuideData['犀甲之王'] = 
   { 
    id=642, type="0,0,0,1,0,0", iconpath="犀甲之王.dds", name="犀甲之王", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="犀甲之王", taskid="1",
    istasknpc=false, descnpc="犀甲之王", descmapid=4, descpos="-418,0,-411", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       擅长战斗的犀甲人首领，身材高大威猛，能以一敌百，身上的犀甲可同时发挥进攻和防守的作用，穿在身上如虎添翼。"}
AimGuideData['死泽鱼妖'] = 
   { 
    id=643, type="0,0,0,1,0,0", iconpath="死泽鱼妖.dds", name="死泽鱼妖", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="死泽鱼妖", taskid="1",
    istasknpc=false, descnpc="死泽鱼妖", descmapid=5, descpos="267,0,306", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       鱼人族中的精锐，武功高强，能说人语，还可以使用异族法术。常出现在阴冷幽暗的沼泽深处。"}
AimGuideData['万虫之母'] = 
   { 
    id=644, type="0,0,0,1,0,0", iconpath="万虫之母.dds", name="万虫之母", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="万虫之母", taskid="1",
    istasknpc=false, descnpc="万虫之母", descmapid=5, descpos="371,0,55", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       沼泽中的毒虫之首，能大量繁殖毒虫，吞噬方圆十里之内的生物，人畜遇上无一幸免。"}
AimGuideData['死泽厉鬼'] = 
   { 
    id=645, type="0,0,0,1,0,0", iconpath="死泽厉鬼.dds", name="死泽厉鬼", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="死泽厉鬼", taskid="1",
    istasknpc=false, descnpc="死泽厉鬼", descmapid=5, descpos="-148,0,144", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       误入死亡沼泽的修道中人，困死在幽暗的沼泽里，临死前的怨念与不甘成为戾气，使之难入轮回，化为厉鬼存于沼泽中。"}
AimGuideData['金瞳妖蝠'] = 
   { 
    id=673, type="0,0,0,1,0,0", iconpath="金瞳妖蝠.dds", name="金瞳妖蝠", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="金瞳妖蝠", taskid="1",
    istasknpc=false, descnpc="金瞳妖蝠", descmapid=9, descpos="302,0,-416", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       蝙蝠之王，已修炼千年，双目呈金色，锐利的眼神夺人心智，双翼展开可达三米，阻挡一切敌人的退路。"}
AimGuideData['炼血死士'] = 
   { 
    id=674, type="0,0,0,1,0,0", iconpath="炼血死士.dds", name="炼血死士", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="炼血死士", taskid="1",
    istasknpc=false, descnpc="炼血死士", descmapid=9, descpos="319,0,-262", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       练血堂的中坚力量，不惧生死，僵尸一般的存在，需吸食人畜之血维持生机。"}
AimGuideData['黑水玄蛇'] = 
   { 
    id=675, type="0,0,0,1,0,0", iconpath="黑水玄蛇.dds", name="黑水玄蛇", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="黑水玄蛇", taskid="1",
    istasknpc=false, descnpc="黑水玄蛇", descmapid=9, descpos="-335,0,-431", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       上古魔兽，凶悍无比。蛇身粗逾四丈，长逾百丈。食神仙药而不死，寿过万年，非之天敌黄鸟不能除之。"}
AimGuideData['大黑蛭'] = 
   { 
    id=676, type="0,0,0,1,0,0", iconpath="大黑蛭.dds", name="大黑蛭", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="大黑蛭", taskid="1",
    istasknpc=false, descnpc="大黑蛭", descmapid=12, descpos="-158,0,154", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       体型巨大的怪物，长年生活于地下，不见阳光，浑身长满触角，以触角吸引一切活物做食物，近其身绝不可活。出现在黑石洞地宫。"}
AimGuideData['焰魔'] = 
   { 
    id=677, type="0,0,0,1,0,0", iconpath="焰魔.dds", name="焰魔", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="焰魔", taskid="1",
    istasknpc=false, descnpc="焰魔", descmapid=12, descpos="-339,0,264", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       火之守护兽，守护着万火之精，寻常水源无法扑灭，可焚烧天地万物。出现在黑石洞地宫。"}
AimGuideData['洪荒兽王'] = 
   { 
    id=681, type="0,0,0,1,0,0", iconpath="洪荒兽王.dds", name="洪荒兽王", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="洪荒兽王", taskid="1",
    istasknpc=false, descnpc="洪荒兽王", descmapid=6, descpos="-306,0,-227", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       蛮荒之地特有的种族，身体高大，凶恶丑陋，但力大无穷，铜身铁臂，具有刀枪不入的体格。"}
AimGuideData['血狮'] = 
   { 
    id=647, type="0,0,0,1,0,0", iconpath="血狮.dds", name="血狮", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="血狮", taskid="1",
    istasknpc=false, descnpc="血狮", descmapid=6, descpos="-336,0,-236", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       十三妖王之一的狮王，全身通红，似以鲜血浸染，凶残狡猾，并具备高强的妖法。"}
AimGuideData['玄冥幽鬼'] = 
   { 
    id=648, type="0,0,0,1,0,0", iconpath="玄冥幽鬼.dds", name="玄冥幽鬼", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="玄冥幽鬼", taskid="1",
    istasknpc=false, descnpc="玄冥幽鬼", descmapid=6, descpos="-405,0,128", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       游荡并横行玄冥之界的鬼魂，怨灵的一种，不惧生死，不入轮回，对活人有着强烈的敌意。"}
AimGuideData['熔岩之王'] = 
   { 
    id=679, type="0,0,0,1,0,0", iconpath="熔岩之王.dds", name="熔岩之王", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="熔岩之王", taskid="1",
    istasknpc=false, descnpc="熔岩之王", descmapid=12, descpos="243,0,-234", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       生活在熔岩之中的恐怖魔兽，擅长用熔岩的力量攻击其他一切生物，若离开熔岩，便失去所有力量。"}
AimGuideData['咆哮之王'] = 
   { 
    id=680, type="0,0,0,1,0,0", iconpath="咆哮之王.dds", name="咆哮之王", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="咆哮之王", taskid="1",
    istasknpc=false, descnpc="咆哮之王", descmapid=12, descpos="-89,0,-212", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       性格残暴，声如厉雷的猛兽，生性既猛且蠢，空具一身蛮力，可智取不可强夺。"}
AimGuideData['黄鸟'] = 
   { 
    id=5592, type="0,0,0,1,0,0", iconpath="黄鸟.dds", name="黄鸟", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="黄鸟", taskid="1",
    istasknpc=false, descnpc="黄鸟", descmapid=12, descpos="156,0,-265", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       四神兽之一的黄鸟，九天灵鸟，黑水玄蛇的天敌。"}
AimGuideData['烛龙'] = 
   { 
    id=5593, type="0,0,0,1,0,0", iconpath="烛龙.dds", name="烛龙", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="烛龙", taskid="1",
    istasknpc=false, descnpc="烛龙", descmapid=12, descpos="460,0,-188", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       四神兽之一的烛龙，上古神兽，身长千里，神力烛照九泉。"}
AimGuideData['夔牛'] = 
   { 
    id=5594, type="0,0,0,1,0,0", iconpath="夔牛.dds", name="夔牛", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="夔牛", taskid="1",
    istasknpc=false, descnpc="夔牛", descmapid=12, descpos="261,0,-96", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       四神兽之一的夔牛，入水即风雨，目光如日月，一足能走，其声如雷。"}
AimGuideData['饕餮'] = 
   { 
    id=5595, type="0,0,0,1,0,0", iconpath="饕餮.dds", name="饕餮", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="饕餮", taskid="1",
    istasknpc=false, descnpc="饕餮", descmapid=12, descpos="115,0,-167", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       四神兽之一的饕餮，凶残好吃，见到什么吃什么，是兽妖的守护兽。"}
AimGuideData['九命猫王'] = 
   { 
    id=678, type="0,0,0,1,0,0", iconpath="九命猫王.dds", name="九命猫王", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="九命猫王", taskid="1",
    istasknpc=false, descnpc="九命猫王", descmapid=12, descpos="-223,0,-268", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       猫族的统治者，受妖气侵蚀，可使用妖法，具有不死之身。"}
AimGuideData['鱼人王'] = 
   { 
    id=5596, type="0,0,0,1,0,0", iconpath="鱼人王.dds", name="鱼人王", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="鱼人王", taskid="1",
    istasknpc=false, descnpc="鱼人王", descmapid=7, descpos="357,113,355", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       鱼人族的统治者，武功高强，能说人语，会多种法术，身旁鱼人护卫众多，难以接近。"}
AimGuideData['不死天尸'] = 
   { 
    id=10491, type="0,0,0,1,0,0", iconpath="不死天尸.dds", name="不死天尸", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="不死天尸", taskid="1",
    istasknpc=false, descnpc="不死天尸", descmapid=8, descpos="-378,0,289", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       传说中的千年古尸，一直出没于南疆万蛇殿中，汲取天地灵气补充法力。"}
AimGuideData['八荒火龙'] = 
   { 
    id=10492, type="0,0,0,1,0,0", iconpath="八荒火龙.dds", name="八荒火龙", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="八荒火龙", taskid="1",
    istasknpc=false, descnpc="八荒火龙", descmapid=14, descpos="-391,0,-101", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       “八荒玄火阵”的终极召唤灵物，南疆古老的巫族传说中，毁灭世间万物的可怕凶兽。"}
AimGuideData['兽神'] = 
   { 
    id=10493, type="0,0,0,1,0,0", iconpath="兽神.dds", name="兽神", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="兽神", taskid="1",
    istasknpc=false, descnpc="兽神", descmapid=14, descpos="-358,0,356", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       冷漠淡然的妖艳少年，为情而生，为爱而亡，与南疆巫女玲珑演绎了一场凄美的爱情神话。"}
AimGuideData['玄冥鬼手'] = 
   { 
    id=10487, type="0,0,0,1,0,0", iconpath="玄冥鬼手.dds", name="玄冥鬼手", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="玄冥鬼手", taskid="1",
    istasknpc=false, descnpc="玄冥鬼手", descmapid=14, descpos="-386,0,235", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       天地浩劫产生的异物，戾气凝聚其上，具强大的杀气，被上古高人囚禁于镇魔古洞中。"}
AimGuideData['昆仑奴'] = 
   { 
    id=10669, type="0,0,0,1,0,0", iconpath="昆仑奴.dds", name="昆仑奴", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="昆仑奴", taskid="1",
    istasknpc=false, descnpc="昆仑奴", descmapid=10, descpos="-333,0,121", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       异族进贡的奴隶，本性情温良、踏实耿直，途中被妖兽所袭，沾染妖气，后于昆仑山胡乱修炼，汲取仙气，变成半妖半仙。"}
AimGuideData['炼狱魔尊'] = 
   { 
    id=15702, type="0,0,0,1,0,0", iconpath="炼狱魔尊.dds", name="炼狱魔尊", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="炼狱魔尊", taskid="1",
    istasknpc=false, descnpc="炼狱魔尊", descmapid=17, descpos="-370,0,305", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       炼狱众魔之首，统率炼狱之中百鬼千魔，法力高强，凶猛残暴，可发毁天灭地之威。"}
AimGuideData['妙翅鸟'] = 
   { 
    id=15703, type="0,0,0,1,0,0", iconpath="黄鸟.dds", name="妙翅鸟", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="妙翅鸟", taskid="1",
    istasknpc=false, descnpc="妙翅鸟", descmapid=17, descpos="-20,0,22", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       炼狱妙翅鸟蛋中孕育的神鸟，每当蛋壳破碎便可重生一次，为上古灵鸟之一。"}
AimGuideData['黑心老人'] = 
   { 
    id=13576, type="0,0,0,1,0,0", iconpath="黑心老人.dds", name="黑心老人", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="黑心老人", taskid="1",
    istasknpc=false, descnpc="黑心老人", descmapid=16, descpos="-323,0,-52", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       八百年前一统邪教的伟大人物，练成噬血珠，给人间带来极大祸害。"}
AimGuideData['金铃夫人'] = 
   { 
    id=13575, type="0,0,0,1,0,0", iconpath="金铃夫人.dds", name="金铃夫人", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="金铃夫人", taskid="1",
    istasknpc=false, descnpc="金铃夫人", descmapid=16, descpos="-405,0,-221", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       聪明绝顶，道行精深，对魔教经典天书有大悟于心，于三百年前独立创立“合欢派”，成为魔教中四大鼎盛门派之一。"}
AimGuideData['普智'] = 
   { 
    id=13579, type="0,0,0,1,0,0", iconpath="普智.dds", name="普智", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="普智", taskid="1",
    istasknpc=false, descnpc="普智", descmapid=16, descpos="-13,0,-217", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       天音寺四大神僧之一，心怀大智，欲参破长生之谜，在联合佛道二家的努力失败后，被噬血珠侵蚀心志，遁入魔道。"}
AimGuideData['万剑一'] = 
   { 
    id=13578, type="0,0,0,1,0,0", iconpath="万剑一.dds", name="万剑一", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="万剑一", taskid="1",
    istasknpc=false, descnpc="万剑一", descmapid=16, descpos="-105,0,-76", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       道行高深的青云弟子，性格潇洒豪放，资质惊人，于幻月洞中点化道玄，并将诛仙剑交予张小凡斩杀魔教。"}
AimGuideData['法莲王'] = 
   { 
    id=10670, type="0,0,0,1,0,0", iconpath="法莲王.dds", name="法莲王", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="法莲王", taskid="1",
    istasknpc=false, descnpc="法莲王", descmapid=11, descpos="358,185,-380", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       修罗七王之一，偶尔出现在修罗法莲寺中，汲取寺内生灵之气，提升法力。"}
AimGuideData['归蝶王'] = 
   { 
    id=10671, type="0,0,0,1,0,0", iconpath="归蝶王.dds", name="归蝶王", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="归蝶王", taskid="1",
    istasknpc=false, descnpc="归蝶王", descmapid=11, descpos="-156,141,102", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       修罗七王之一，偶尔出现在修罗界归蝶塔中，妖艳貌美，体态婀娜，法力高强。"}
AimGuideData['天忍王'] = 
   { 
    id=10672, type="0,0,0,1,0,0", iconpath="天忍王.dds", name="天忍王", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="天忍王", taskid="1",
    istasknpc=false, descnpc="天忍王", descmapid=11, descpos="133,141,102", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       修罗七王之一，偶尔出现在修罗界天忍塔内，全身披满钢甲，且可召唤坐骑魔獒，攻击敌人。"}
AimGuideData['魔獒'] = 
   { 
    id=10673, type="0,0,0,1,0,0", iconpath="魔獒.dds", name="魔獒", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="魔獒", taskid="1",
    istasknpc=false, descnpc="魔獒", descmapid=11, descpos="133,141,102", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       修罗七王之天忍王的坐骑，性烈凶猛，忠实护主，若主人被攻击则不顾一切冲往敌人。"}
AimGuideData['樱木王'] = 
   { 
    id=10674, type="0,0,0,1,0,0", iconpath="樱木王.dds", name="樱木王", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="樱木王", taskid="1",
    istasknpc=false, descnpc="樱木王", descmapid=11, descpos="-384,138,71", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       修罗七王之一，同时拥有人与植物双重身体，长期的变异造成其性格的扭曲，对人怨念极深，死后魂灵也让人不得消停。"}
AimGuideData['阴阳王'] = 
   { 
    id=10675, type="0,0,0,1,0,0", iconpath="阴阳王.dds", name="阴阳王", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="阴阳王", taskid="1",
    istasknpc=false, descnpc="阴阳王", descmapid=11, descpos="-158,140,442", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       修罗七王之一，被枷锁束缚于其阴阳难分的巢穴之中，日日挣扎却始终无法解脱，倘若有人靠近，极有可能成为其深深怨气的发泄目标。"}
AimGuideData['风火王'] = 
   { 
    id=10676, type="0,0,0,1,0,0", iconpath="风火王.dds", name="风火王", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="风火王", taskid="1",
    istasknpc=false, descnpc="风火王", descmapid=11, descpos="407,122,309", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       修罗七王之一，居于修罗血池之中，双手可分别召唤风火之力，对入侵者绝不手软。"}
AimGuideData['安土王'] = 
   { 
    id=10677, type="0,0,0,1,0,0", iconpath="安土王.dds", name="安土王", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="安土王", taskid="1",
    istasknpc=false, descnpc="安土王", descmapid=11, descpos="-11,148,264", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       修罗七王之一，居于修罗大殿，身具大地之力，有震慑天地之威，多条手臂更为其增加了无穷的力量。"}
AimGuideData['玄天上将'] = 
   { 
    id=10678, type="0,0,0,1,0,0", iconpath="玄天上将.dds", name="玄天上将", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="玄天上将", taskid="1",
    istasknpc=false, descnpc="玄天上将", descmapid=15, descpos="-444,0,-237", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       相传为真武大帝座下侍将，真武大帝封剑隐去之后，将其留在昆仑仙境驻守。"}
AimGuideData['开明兽'] = 
   { 
    id=10680, type="0,0,0,1,0,0", iconpath="开明兽.dds", name="开明兽", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="开明兽", taskid="1",
    istasknpc=false, descnpc="开明兽", descmapid=15, descpos="-16,0,85", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       昆仑山守护神之一，终日瞪视昆仑山，威震百灵，使得任何异常事物都无法进入昆仑，维护了昆仑仙境的平和安宁。"}
AimGuideData['冰姹玄女'] = 
   { 
    id=11563, type="0,0,0,1,0,0", iconpath="冰姹玄女.dds", name="冰姹玄女", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="冰姹玄女", taskid="1",
    istasknpc=false, descnpc="冰姹玄女", descmapid=15, descpos="-50,0,186", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       昆仑之巅的守护神，召唤玄冰之力，护住昆仑山，使凡人异物难以闯入。"}
AimGuideData['混沌'] = 
   { 
    id=10679, type="0,0,0,1,0,0", iconpath="混沌.dds", name="混沌", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="混沌", taskid="1",
    istasknpc=false, descnpc="混沌", descmapid=15, descpos="391,0,400", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       镇压在昆仑之西的神兽，有目而不见，有耳而不闻，有腹无五脏，有肠直而不旋，难辨是非善恶不明之兽。"}
AimGuideData['邪恶先锋·凯'] = 
   { 
    id=22107, type="0,0,0,1,0,0", iconpath="战神·龙戊.dds", name="邪恶先锋·凯", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="邪恶先锋·凯", taskid="1",
    istasknpc=false, descnpc="邪恶先锋·凯", descmapid=10, descpos="-347,121,-117", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       八级战魂之一，被封印在八卦镇石之中，一旦封印开启，其便引领邪恶之力而来，需及时将其除去，否则天地剧变，苍生遭劫。"}
AimGuideData['邪恶先锋·涛'] = 
   { 
    id=22106, type="0,0,0,1,0,0", iconpath="黑心老人.dds", name="邪恶先锋·涛", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="邪恶先锋·涛", taskid="1",
    istasknpc=false, descnpc="邪恶先锋·涛", descmapid=10, descpos="-364,121,-113", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       八级战魂之一，被封印在八卦镇石之中，一旦封印开启，其便引领邪恶之力而来，需及时将其除去，否则天地剧变，苍生遭劫。"}
AimGuideData['背叛者·娟'] = 
   { 
    id=22252, type="0,0,0,1,0,0", iconpath="金铃夫人.dds", name="背叛者·娟", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="背叛者·娟", taskid="1",
    istasknpc=false, descnpc="背叛者·娟", descmapid=8, descpos="-169,0,-83", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       八级战魂之一，被封印在八卦镇石之中，一旦封印开启，其便引领邪恶之力而来，需及时将其除去，否则天地剧变，苍生遭劫。"}
AimGuideData['背叛者·瞳'] = 
   { 
    id=22251, type="0,0,0,1,0,0", iconpath="背叛者·瞳.dds", name="背叛者·瞳", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="背叛者·瞳", taskid="1",
    istasknpc=false, descnpc="背叛者·瞳", descmapid=8, descpos="-165,0,-98", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       八级战魂之一，被封印在八卦镇石之中，一旦封印开启，其便引领邪恶之力而来，需及时将其除去，否则天地剧变，苍生遭劫。"}
AimGuideData['背叛者·宇'] = 
   { 
    id=22253, type="0,0,0,1,0,0", iconpath="玄天上将.dds", name="背叛者·宇", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="背叛者·宇", taskid="1",
    istasknpc=false, descnpc="背叛者·宇", descmapid=8, descpos="-147,0,-99", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       八级战魂之一，被封印在八卦镇石之中，一旦封印开启，其便引领邪恶之力而来，需及时将其除去，否则天地剧变，苍生遭劫。"}
AimGuideData['密探·永'] = 
   { 
    id=23165, type="0,0,0,1,0,0", iconpath="黑心老人.dds", name="密探·永", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="密探·永", taskid="1",
    istasknpc=false, descnpc="密探·永", descmapid=7, descpos="242,0,19", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       八级战魂之一，被封印在八卦镇石之中，一旦封印开启，其便引领邪恶之力而来，需及时将其除去，否则天地剧变，苍生遭劫。"}
AimGuideData['冥界·古猿'] = 
   { 
    id=35239, type="0,0,0,1,0,0", iconpath="冥界·古猿.dds", name="冥界·古猿", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="冥界·古猿", taskid="1",
    istasknpc=false, descnpc="冥界·古猿", descmapid=10, descpos="267,0,350", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       冥界七王之一，利用人间的混乱冲破时空界点来到人间，从冥间带来的强大力量使其具有睥睨天下苍生之气魄，他们的目的只有一个：建造一个属于自己的全新的世界。"}
AimGuideData['冥界·玄鸟'] = 
   { 
    id=35240, type="0,0,0,1,0,0", iconpath="黄鸟.dds", name="冥界·玄鸟", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="冥界·玄鸟", taskid="1",
    istasknpc=false, descnpc="冥界·玄鸟", descmapid=5, descpos="-155,0,-394", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff冥界七王之一，利用人间的混乱冲破时空界点来到人间，从冥间带来的强大力量使其具有睥睨天下苍生之气魄，他们的目的只有一个：建造一个属于自己的全新的世界。"}
AimGuideData['冥界·饕餮'] = 
   { 
    id=35242, type="0,0,0,1,0,0", iconpath="饕餮.dds", name="冥界·饕餮", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="冥界·饕餮", taskid="1",
    istasknpc=false, descnpc="冥界·饕餮", descmapid=4, descpos="-382,0,-361", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff冥界七王之一，利用人间的混乱冲破时空界点来到人间，从冥间带来的强大力量使其具有睥睨天下苍生之气魄，他们的目的只有一个：建造一个属于自己的全新的世界。"}
AimGuideData['冥界·圣使'] = 
   { 
    id=35241, type="0,0,0,1,0,0", iconpath="冥界·圣使.dds", name="冥界·圣使", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="冥界·圣使", taskid="1",
    istasknpc=false, descnpc="冥界·圣使", descmapid=7, descpos="-371,0,17", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       冥界七王之一，利用人间的混乱冲破时空界点来到人间，从冥间带来的强大力量使其具有睥睨天下苍生之气魄，他们的目的只有一个：建造一个属于自己的全新的世界。"}
AimGuideData['冥界·天狼'] = 
   { 
    id=35236, type="0,0,0,1,0,0", iconpath="冥界·天狼.dds", name="冥界·天狼", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="冥界·天狼", taskid="1",
    istasknpc=false, descnpc="冥界·天狼", descmapid=6, descpos="365,0,-382", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       冥界七王之一，利用人间的混乱冲破时空界点来到人间，从冥间带来的强大力量使其具有睥睨天下苍生之气魄，他们的目的只有一个：建造一个属于自己的全新的世界。"}
AimGuideData['冥界·血狮'] = 
   { 
    id=35238, type="0,0,0,1,0,0", iconpath="冥界·血狮.dds", name="冥界·血狮", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="冥界·血狮", taskid="1",
    istasknpc=false, descnpc="冥界·血狮", descmapid=8, descpos="344,0,362", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff冥界七王之一，利用人间的混乱冲破时空界点来到人间，从冥间带来的强大力量使其具有睥睨天下苍生之气魄，他们的目的只有一个：建造一个属于自己的全新的世界。"}
AimGuideData['冥界·兽神'] = 
   { 
    id=35243, type="0,0,0,1,0,0", iconpath="冥界·兽神.dds", name="冥界·兽神", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="冥界·兽神", taskid="1",
    istasknpc=false, descnpc="冥界·兽神", descmapid=3, descpos="-213,0,-21", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       冥界七王之一，利用人间的混乱冲破时空界点来到人间，从冥间带来的强大力量使其具有睥睨天下苍生之气魄，他们的目的只有一个：建造一个属于自己的全新的世界。"}AimGuideData['瘟皇·秦广'] = 
   { 
    id=25949, type="0,0,0,1,0,0", iconpath="瘟皇·秦广.dds", name="瘟皇·秦广", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="瘟皇·秦广", taskid="1",
    istasknpc=false, descnpc="瘟皇·秦广", descmapid=5, descpos="-60,0,205", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff专司人间寿夭生死册集，接引超生，统管幽冥吉凶，若能将其战胜，便可超渡人间一段怨魂，"}
AimGuideData['战神·龙戊'] = 
   { 
    id=25950, type="0,0,0,1,0,0", iconpath="战神·龙戊.dds", name="战神·龙戊", 
    validtime="9:00-12:00", levelreq="1", needreborn=false, permitcount=1, cond=" ", 
    validday="每天", npc="战神·龙戊", taskid="1",
    istasknpc=false, descnpc="战神·龙戊", descmapid=10, descpos="7,183,-170", istransport=true, starlevel="5,5,5,5,5,5,5,5,5,5,5",
    desc="^ffffff       八级战魂之一，被封印在八卦镇石之中，一旦封印开启，其便引领邪恶之力而来，需及时将其除去，否则天地剧变，苍生遭劫。"}
			
function AimGuideData:GetSelf()
	return self;
end


-- 目的指引2(界面入口指引)
-- id: ID
-- type: 类型（六种，分别是：赚钱，升级，泡妞，提升实力，不无聊，花钱）
-- name: 名称
-- iconpath: 图标路径
-- levelreq: 等级要求
-- needreborn：是否要求飞升
-- cond：条件
-- starlevel：星级（11个星级，分别对应1,15,30,45....150级）
-- desc:内容描述


AimGuideData2 = {}

    AimGuideData2['征婚'] = 
			{ 
				id=-1, type="0,0,1,0,0,0", name="征婚", iconpath="彻夜不眠.dds", levelreq="10-160", needreborn=false, cond="", starlevel="5,5,5,5,5,5,5,5,5,5,5", btnname="征婚", dlgname="Win_FriendSeeking",
				desc="^ffffff       每每想起要一个人孤零零的在游戏中拼搏着就难以入眠，现在不怕了，打开上面的征婚按钮（也可以在游戏右下角的<全部>选项中选择征友查看），你可以选择<发布消息>然后选择门派、帮派、等级并发布留言，等待你的另一半出现吧！\r"
			}
			
		 AimGuideData2['征友'] = 
			{ 
				id=-1, type="0,0,1,0,0,0", name="征友", iconpath="彻夜不眠.dds", levelreq="1-160", needreborn=false, cond="", starlevel="5,5,5,5,5,5,5,5,5,5,5", btnname="征友", dlgname="Win_FriendSeeking",
				desc="^ffffff       身边没有几个兄弟姐妹一起奋斗怎么能行！打开上面的征友按钮（也可以在游戏右下角的<全部>选项中选择征友查看），你可以选择<发布消息>然后选择门派、性别、等级、帮派并发布留言，等待你志同道合的朋友出现吧！\r"
			}
		 AimGuideData2['师门'] = 
			{ 
				id=-1, type="0,0,1,1,0,0", name="师门", iconpath="彻夜不眠.dds", levelreq="1-160", needreborn=false, cond="", starlevel="5,5,5,5,5,5,5,5,5,5,5", btnname="师门", dlgname="Win_FriendSeeking",
				desc="^ffffff       升级太难，兜里没钱。想要找一个依靠吗？打开上面的师门界面（也可以在游戏右下角的<全部>选项中选择征友查看），可以找到给全力培养你的师傅！"
			}
		 AimGuideData2['入家族'] = 
			{ 
				id=-1, type="0,0,1,1,0,0", name="入家族", iconpath="彻夜不眠.dds", levelreq="20-160", needreborn=false, cond="", starlevel="5,5,5,5,5,5,5,5,5,5,5", btnname="入家族", dlgname="Win_FriendSeeking",
				desc="^ffffff       被杀了没有人帮你，好希望一呼百应，打开上面的师门界面（也可以在游戏右下角的<全部>选项中选择征友查看），可以选择你想进入的家族，以后就不怕被杀没人报仇了！"
			}
		 AimGuideData2['购买时装'] = 
			{ 
				id=-1, type="0,0,0,0,1,1", name="购买时装", iconpath="彻夜不眠.dds", levelreq="1-160", needreborn=false, cond="", starlevel="5,5,5,5,5,5,5,5,5,5,5", btnname="购买时装", dlgname="Win_QShop",
				desc="^ffffff       不喜欢装备的样子，喜欢酷炫的外貌，希望帅哥美女用羡慕的眼光看着你吗？打开天帝宝库界面，选中里面的时装标签，你可以购买各式各样酷炫靓丽的时装！"
			}
		 AimGuideData2['炼器道具'] = 
			{ 
				id=-1, type="0,0,0,1,0,1", name="炼器道具", iconpath="彻夜不眠.dds", levelreq="1-160", needreborn=false, cond="", starlevel="5,5,5,5,5,5,5,5,5,5,5", btnname="炼器道具", dlgname="Win_QShop",
				desc="^ffffff       炼器总是失败，是不炼器大师不要紧，只要打开商城界面，找到法器-炼器道具，就可以购买辅助炼器的道具了，装备再也不怕消失了！"
			}
		 AimGuideData2['排行榜'] = 
			{ 
				id=-1, type="0,0,0,0,1,0", name="排行榜", iconpath="彻夜不眠.dds", levelreq="1-160", needreborn=false, cond="", starlevel="5,5,5,5,5,5,5,5,5,5,5", btnname="排行榜", dlgname="Win_RankList",
				desc="^ffffff       想知道最顶尖的玩家都是那些人他们都在哪些方面比我强呢，打开排行榜界面，这些疑问就有了解答，你可以通过参照排行榜来提升自己的实力，很快排行榜上就会有你的名字了！"
			}
		 AimGuideData2['诛仙剧情'] = 
			{ 
				id=-1, type="0,1,0,1,0,0", name="诛仙剧情", iconpath="彻夜不眠.dds", levelreq="1-160", needreborn=false, cond="", starlevel="5,5,5,5,5,5,5,5,5,5,5", btnname="诛仙剧情", dlgname="Win_Quest",
				desc="^ffffff       任务做完了好无聊，希望了解更多的关于诛仙的情节的事，打开任务界面里的诛仙剧情，诛仙的故事情节历历在目。"
			}
		 AimGuideData2['小游戏'] = 
			{ 
				id=-1, type="0,0,0,0,1,0", name="小游戏", iconpath="彻夜不眠.dds", levelreq="1-160", needreborn=false, cond="", starlevel="5,5,5,5,5,5,5,5,5,5,5", btnname="小游戏", dlgname="Win_GameCasual",
				desc="^ffffff       好无聊，天人合一了我还能干点什么呢，可以玩玩小游戏啊，还有奖励等你拿哦！"
			}
		 AimGuideData2['玩家社区'] = 
			{ 
				id=-1, type="0,0,1,0,1,0", name="玩家社区", iconpath="彻夜不眠.dds", levelreq="1-160", needreborn=false, cond="", starlevel="5,5,5,5,5,5,5,5,5,5,5", btnname="玩家社区", dlgname="ThemeLink",
				desc="^ffffff       想了解诛仙的第一手信息，想与各个服务器的诛仙好友交流经验，打开诛仙主题站就可以在玩家社区与他人交流了！"
			}
		 AimGuideData2['新品上架'] = 
			{ 
				id=-1, type="0,0,0,1,0,1", name="新品上架", iconpath="彻夜不眠.dds", levelreq="1-160", needreborn=false, cond="", starlevel="5,5,5,5,5,5,5,5,5,5,5", btnname="新品上架", dlgname="Win_QShop",
				desc="^ffffff       时刻了解天帝宝库又有什么神奇的道具可以提升实力了，关注天气宝库的新品上架界面吧！"
			}
		 AimGuideData2['活跃度'] = 
			{ 
				id=-1, type="0,0,0,1,1,0", name="活跃度", iconpath="彻夜不眠.dds", levelreq="1-160", needreborn=false, cond="", starlevel="5,5,5,5,5,5,5,5,5,5,5", btnname="活跃度", dlgname="Win_Activity",
				desc="^ffffff       每天不知道做什么，那就按照提升活跃度的内容去玩吧，很多奖励等着你！"
			}
		 AimGuideData2['VIP奖励'] = 
			{ 
				id=-1, type="0,0,0,1,0,0", name="VIP奖励", iconpath="彻夜不眠.dds", levelreq="1-160", needreborn=false, cond="", starlevel="5,5,5,5,5,5,5,5,5,5,5", btnname="VIP奖励", dlgname="Win_QShop_vip",
				desc="^ffffff       每天上线就可以领取VIP奖励？还有这种好事？等级越高奖励越好？一定要每天打开VIP奖励界面，否则就亏大了！"
			}
		 AimGuideData2['三界商行'] = 
			{ 
				id=-1, type="0,0,0,0,0,1", name="三界商行", iconpath="彻夜不眠.dds", levelreq="1-160", needreborn=false, cond="", starlevel="5,5,5,5,5,5,5,5,5,5,5", btnname="三界商行", dlgname="Win_Auction",
				desc="^ffffff       想低价买到神奇的道具？这里是你的天堂，只要是你的价格够独特那这个物品就是你的！看看截面上的内容介绍吧，很简单的操作，拼的就是运气！"
			}

function AimGuideData2:GetSelf()
	return self;
end