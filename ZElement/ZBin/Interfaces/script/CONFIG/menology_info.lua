------------------------------Menology event definition-----------------------------

-- id:		本文档内容每一项独立id
-- iconpath1: 图标路径1
-- iconpath2: 图标路径2
-- backgroud1: 背景路径1(日历日期背景边框)
-- backgroud2: 背景路径2(日历日期背景底图)
-- rewardtype:	类别掩码(二进制位表示奖励类型，最后边2位修为，接下来二位金钱，然后是物品，0-没有此类奖励，1，2，3-低，中，高奖励)
-- name: 名称-显示名称
-- maskinfo:	相关配置掩码：可策划定义（是否login提示、是否特殊任务...）
-- ismendisplay: 是否日历的对应日期显示此活动
-- mendisplaylevel: 在日历界面的显示优先级1,2,3...
-- taskid:	如果不是任务，填0
-- validtime:时间段
-- levelreq: 等级要求
-- needreborn:是否要求飞升
-- permitcount:次数限制
-- cond:条件
-- volidday: 活动时间,周几
-- line: 线路
-- npc: 相关NPC
-- istaskaward: 是否读取任务id对应的任务奖励
-- awardiconpath1: 奖励内容1
-- awarddesc1: 奖励内容1文字描述
-- awardiconpath2: 奖励内容2
-- awarddesc2: 奖励内容2文字描述
-- awardiconpath3: 奖励内容3
-- awarddesc3: 奖励内容3文字描述
-- commendlevel: 推荐等级,预留

-- istasknpc: 相关NPC处是否读取taskid对应的任务NPC:0,1
-- descnpc:	如果不读取任务npc,手动填写描述
-- descmapid: 如果不读取任务npc,手动填写寻径地图id
-- descpos: 如果不读取任务npc,手动填写寻径坐标
-- istransport: 是否使用飞天传送,是则读取task的传送功能
-- desc:内容描述

-- monsterlevel: 怪物等级
-- reborncycle: 刷新周期
-- relacontroler: 关联控制器id
-- needrolenumber: 需要人数
-- ecmpath: 怪物3D形象路径 

MenologyRecommend = {}
MenologyRecommend['逐波圣灵'] = 
			{ 
				id=3, iconpath1="逐波圣灵.dds", iconpath2="逐波圣灵大.dds", iconpath3="", 
				rewardtype=203, maskinfo=0, name="逐波圣灵", ismendisplay=false, mendisplaylevel=3,
				validtime="11:50-12:00", levelreq="1-150", needreborn=false, permitcount=1, cond="^ffcb4a10金", 
				validday="1,2,3,4,5,6,7", line="15线", npc="上古奇人", istaskaward=false, taskid="14607",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff常规获得^ffcb4a20个太极金丹\r^ffffff若获胜可额外获得^ffcb4a100个太极金丹^ffffff奖励。"}, 
					{awardid=21345, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a造化珠\r\r^ffffff若获胜可获得200个造化珠^ffffff奖励。"},
				},
				commendlevel=4, istasknpc=false, descnpc="上古奇人", descmapid=1, descpos="193,0,123", istransport=true, 
				desc="^ffffff       每天11：50分在河阳^ffcb4a上古奇人^ffffff处花费^ffcb4a10金^ffffff购买神兽令牌，传送到流波海底观看神兽逐波竞速。活动结束可从夺魁神兽处获得^ffcb4a20个太极金丹^ffffff的奖励。若所购令牌刚好为夺魁神兽，还可获得神兽所赐的^ffcb4a100个太极金丹^ffffff，^ffcb4a200个造化珠^ffffff奖励。\r"
			}
MenologyRecommend['钓鱼大赛'] = 
			{ 
				id=4, iconpath1="钓鱼大赛.dds", iconpath2="钓鱼大赛大.dds", iconpath3="", 
				rewardtype=103, maskinfo=0, name="钓鱼大赛", ismendisplay=false, mendisplaylevel=4, 
				validtime="18:00-19:00", levelreq="45-150", needreborn=false, permitcount=1, cond="^ffcb4a报名券", 
				validday="1,2,3,4,5,6,7", line="1-5线", npc="云游四海的渔夫", istaskaward=false, taskid="5556",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff根据钓取的物品不同在完成相应任\r务后将获得不同金丹奖励。"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a特殊称谓\r\r^ffffff根据钓取的物品不同在完成相应任\r务后将奖励不同称谓。"},
				},
				commendlevel=3, istasknpc=false, descnpc="云游四海的渔夫", descmapid=1, descpos="121,0,145", istransport=true, 
				desc="^ffffff       在流波举行的钓鱼大赛，玩家先用报名卷在云游四海的渔夫处换取^ffcb4a鱼饵^ffffff，然后赶往流波参加大赛。"
			}
MenologyRecommend['戾气危机'] = 
			{ 
				id=5, iconpath1="戾气危机.dds", iconpath2="戾气危机大.dds",  iconpath3="",
				rewardtype=002, maskinfo=0, name="戾气危机", ismendisplay=false, mendisplaylevel=5,
				validtime="18:00-20:00", levelreq="15-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="陆雪琪", istaskaward=false, taskid="12717",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：25"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="陆雪琪", descmapid=2, descpos="403,75,-192", istransport=true, 
				desc="^ffffff       青云山方向突然间天现异象，从^ffcb4a青云 陆雪琪^ffffff处得知，青云后山祖师祠堂处设置了^ffcb4a三根阵法石柱^ffffff，用以镇压戾气，如今石柱多有损耗，阵法已经逐渐失效。请大家帮助修复三根石柱，恢复阵法的力量，可获得^ffcb4a25个太极金丹^ffffff奖励。"
			}
MenologyRecommend['雪人总动员'] = 
			{ 
				id=6, iconpath1="雪人总动员.dds", iconpath2="雪人总动员大.dds", iconpath3="icon\\menology_snowman.tga",
				rewardtype=200, maskinfo=0, name="雪人总动员", ismendisplay=true, mendisplaylevel=6,
				validtime="18:45-21:45", levelreq="60-150", needreborn=false, permitcount=1, cond=" ", 
				validday="7", line="15线", npc="昆仑仙子", istaskaward=false, taskid="10595",
				award=
				{
					{awardid=16385, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a天上天下，通存通\r取。可以在青云仓\r库老板钱多多或巡\r天右都尉处换取丰\r厚奖励"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="昆仑仙子", descmapid=10, descpos="-194,123,355", istransport=true, 
				desc="^ffffff       在^ffcb4a昆仑仙子^ffffff处接受任务,可选择任何一方雪人投注,胜利后可获得相应数量的^ffcb4a乾坤通宝^ffffff。"
			}
MenologyRecommend['新科试炼'] = 
			{ 
				id=7, iconpath1="新科试炼.dds", iconpath2="新科试炼大.dds", iconpath3="",
				rewardtype=203, maskinfo=0, name="新科试炼", ismendisplay=false, mendisplaylevel=7,
				validtime="19:00-19:15", levelreq="1-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="系统定时开启", istaskaward=false, taskid="12868",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量经验。\r个数：答题每获得10分奖励5个金丹"}, 
					{awardid=19252, awardiconpath="", awarddesc=""},
					{awardid=19253, awardiconpath="", awarddesc=""},
				},
				commendlevel=3, istasknpc=false, descnpc="系统定时开启", descmapid=-1, descpos="9999,0,9999", istransport=false, 
				desc="^ffffff       每天^ffcb4a19：00^ffffff整，玩家会接到^ffcb4a新科试炼^ffffff活动的邀请。接受邀请，参与试炼。获得经验奖励的掌握更多知识。\r新科试炼^ffcb4a前10名^ffffff都可以获得^ffcb4a轩辕通宝^ffffff的奖励。并且可以获得稀有的试炼标志称号。"
			}
MenologyRecommend['修真总动员'] = 
			{ 
				id=8, iconpath1="修真总动员.dds", iconpath2="修真总动员大.dds", iconpath3="icon\\menology_allcele.tga",
				rewardtype=200, maskinfo=0, name="修真总动员", ismendisplay=true, mendisplaylevel=8,
				validtime="19:00-22:00", levelreq="60-150", needreborn=false, permitcount=1, cond=" ", 
				validday="6", line="1-15线", npc="阿福", istaskaward=false, taskid="11339",
				award=
				{
					{awardid=16385, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a天上天下，通存通\r取。可以在青云仓\r库老板钱多多或巡\r天右都尉处换取丰\r厚奖励。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="阿福", descmapid=2, descpos="179,0,108", istransport=true, 
				desc="^ffffff       在^ffcb4a青云阿福^ffffff处领取任务，完成任务需要5个用^ffcb4a虎魄冰晶^ffffff换取的棒棒糖，成功后奖励200^ffcb4a乾坤通宝^ffffff。"
			}
MenologyRecommend['追风圣灵'] = 
			{ 
				id=9, iconpath1="追风圣灵.dds", iconpath2="追风圣灵大.dds", iconpath3="",
				rewardtype=203, maskinfo=0, name="追风圣灵", ismendisplay=false, mendisplaylevel=9,
				validtime="19:20-19:30", levelreq="1-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", line="15线", npc="上古奇人", istaskaward=false, taskid="14989",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff常规获得^ffcb4a20个太极金丹\r^ffffff若获胜可额外获得^ffcb4a100个太极金丹^ffffff奖励。"}, 
					{awardid=21345, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a造化珠\r\r^ffffff若获胜可获得200个造化珠^ffffff奖励。"},
				},
				commendlevel=3, istasknpc=false, descnpc="上古奇人", descmapid=1, descpos="193,0,123", istransport=true, 
				desc="^ffffff       每天19：20分在河阳^ffcb4a上古奇人^ffffff处花费^ffcb4a10金^ffffff购买神兽令牌，传送到焚香谷观看神兽追风竞速。活动结束可从夺魁神兽处获得^ffcb4a20个太极金丹^ffffff的奖励。若所购令牌刚好为夺魁神兽，还可获得神兽所赐的^ffcb4a100个太极金丹^ffffff，^ffcb4a200个造化珠^ffffff奖励。\r^ff0000注：神兽追风竞速过程中需全程关注，否则可能发生神兽走失事件。"
			}
MenologyRecommend['天下会武'] = 
			{ 
				id=10, iconpath1="天下会武.dds", iconpath2="天下会武大.dds", iconpath3="icon\\menology_huiwu.tga",
				rewardtype=003, maskinfo=0, name="天下会武", ismendisplay=true, mendisplaylevel=10,
				validtime="19:30-21:10", levelreq="90-134", needreborn=false, permitcount=1, cond=" ", 
				validday="2,3", line="1-15线", npc="紫微道光元帅", istaskaward=false, taskid="9620,9629,10529,9638,9647,10530",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="完成会武可以获得大量经验奖励。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="紫微道光元帅", descmapid=1, descpos="185,0,140", istransport=true, 
				desc="^ffffff       在^ffcb4a河阳紫微帝君^ffffff处领取天下会武的任务，使用^ffcb4a六道法莲^ffffff可自由选择阵营。\r       完成会武可以获得大量经验。"
			}
MenologyRecommend['金丹之术'] = 
			{ 
				id=11, iconpath1="金丹之术.dds", iconpath2="金丹之术大.dds", iconpath3="",
				rewardtype=202, maskinfo=0, name="金丹之术", ismendisplay=false, mendisplaylevel=11,
				validtime="19:30-21:30", levelreq="30-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="抱朴子", istaskaward=false, taskid="13091",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r获得后自动提升大量经验。\r个数：25"}, 
					{awardid=21510, awardiconpath="item1.dds", awarddesc=""},
				},
				commendlevel=3, istasknpc=false, descnpc="抱朴子", descmapid=2, descpos="96,64,203", istransport=true, 
				desc="^ffffff       在青云举行的大型炼丹活动，玩家在^ffcb4a青云 抱朴子^ffffff处领取前置任务。\r 完成任务后玩家可以获得^ffcb4a碎雪星芒^ffffff及^ffcb4a太极金丹^ffffff奖励。\r       50个碎雪星芒可在陆雪琪处兑换道具^ffcb4a雪之星^ffffff，可以对项链（天罪，龙啸，龙骧，神道，神罚，神藏）进行强化。"
			}
MenologyRecommend['兽妖总动员'] = 
			{ 
				id=12, iconpath1="兽妖总动员.dds", iconpath2="兽妖总动员大.dds", iconpath3="icon\\menology_monsterattack.tga",
				rewardtype=200, maskinfo=0, name="兽妖总动员", ismendisplay=true, mendisplaylevel=12,
				validtime="19:45-21:00", levelreq="60-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1", line="1-15线", npc="法觉", istaskaward=false, taskid="11262",
				award=
				{
					{awardid=16385, awardiconpath="", awarddesc=""}, 
				},
				commendlevel=3, istasknpc=false, descnpc="法觉", descmapid=3, descpos="-121,0,-123", istransport=true, 
				desc="^ffffff       防守天音千佛塔处的镇妖塔，成功抵挡三波攻击后可得到60^ffcb4a乾坤通宝^ffffff的奖励，怪物掉落的^ffcb4a血玉^ffffff也可以换取一定数量的通宝。"
			}
MenologyRecommend['星宿北斗'] = 
			{ 
				id=13, iconpath1="星宿北斗.dds", iconpath2="星宿北斗大.dds", iconpath3="icon\\menology_star.tga",
				rewardtype=003, maskinfo=0, name="星宿北斗", ismendisplay=true, mendisplaylevel=13,
				validtime="20:30-22:30", levelreq="30-150", needreborn=false, permitcount=1, cond=" ", 
				validday="4,5,7", line="1-15线", npc="星宿挑战使者", istaskaward=false, taskid="5469,5470,5471,5472,9046,9047,9048",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验\r\r^ffffff完成任务后将获得大量经验。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="星宿挑战使者", descmapid=1, descpos="113,0,62", istransport=true, 
				desc="^ffffff       在各自等级段对应的地图可找挑战使者接受任务，按要求击杀对应星宿怪，击杀七个星宿怪可得到最高奖励。"
			}
MenologyRecommend['八极战魂'] = 
			{ 
				id=14, iconpath1="八极战魂.dds", iconpath2="八极战魂大.dds", iconpath3="",
				rewardtype=202, maskinfo=0, name="八极战魂", ismendisplay=false, mendisplaylevel=14,
				validtime="0:00-24:00", levelreq="1-150", needreborn=false, permitcount=-1, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="封印石", istaskaward=false, taskid="22314",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。"}, 
					{awardid=21345, awardiconpath="item1.dds", awarddesc=""},
					{awardid=21154, awardiconpath="item1.dds", awarddesc=""},
				},
				commendlevel=3, istasknpc=false, descnpc="封印石", descmapid=2, descpos="93,64,208", istransport=true, 
				desc="^ffffff       完成^ffcb4a国家宝藏^ffffff或者^ffcb4a天诛谕令^ffffff任务后，有几率获得任务道具^ffcb4a启玄印^ffffff，可以用来破除位于青云的^ffcb4a八极镇石^ffffff封印，并换得^ffcb4a八极元珠^ffffff，当封印石收集足够数量的^ffcb4a启玄印^ffffff后，封印效果将消失，可以召唤封印石内的战魂。\r       战魂被消灭后可于^ffcb4a青云 混元真君^ffffff处用^ffcb4a八极元珠^ffffff领取奖励"
			}--]]
MenologyRecommend['天行者'] = 
			{ 
				id=15, iconpath1="天行者.dds", iconpath2="天行者大.dds", iconpath3="",
				rewardtype=003, maskinfo=0, name="天行者", ismendisplay=false, mendisplaylevel=15,
				validtime="0:00-24:00", levelreq="45-150", needreborn=false, permitcount=1, cond="^ffcb4a飞剑", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="天行者", istaskaward=false, taskid="12193",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
					{awardid=20102, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="天行者", descmapid=1, descpos="159,0,103", istransport=true, 
				desc="^ffffff       河阳的^ffcb4a天行者^ffffff提供获得海量经验的绝佳机会。\r 驾驭^ffcb4a飞剑^ffffff，与天行者相会在蔚蓝天空。"
			}
MenologyRecommend['行天'] = 
			{ 
				id=16, iconpath1="行天.dds", iconpath2="行天大.dds", iconpath3="",
				rewardtype=003, maskinfo=0, name="行天", ismendisplay=false, mendisplaylevel=16,
				validtime="0:00-24:00", levelreq="45-150", needreborn=false, permitcount=1, cond="^ffcb4a行天之证", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="天行者", istaskaward=false, taskid="12844",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="天行者", descmapid=1, descpos="159,0,103", istransport=true, 
				desc="^ffffff       每天在完成天行者的任务后，会获得道具^ffcb4a“行天之证”^ffffff，凭借“行天之证”并交纳^ffcb4a25金币^ffffff可以在天行者处领取后续高经验奖励任务任务^ffcb4a“行天”^ffffff。\r       完成任务可以获得^ffcb4a50个太极金丹^ffffff的经验奖励。"
			}
MenologyRecommend['忠烈碑'] = 
			{ 
				id=17, iconpath1="忠烈碑.dds", iconpath2="忠烈碑大.dds", iconpath3="",
				rewardtype=002, maskinfo=0, name="忠烈碑", ismendisplay=false, mendisplaylevel=17,
				validtime="0:00-24:00", levelreq="50-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="忠烈碑", istaskaward=false, taskid="2237,2276,2277,6204,6205,7464,7465",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验\r\r^ffffff完成任务后将获得大量经验。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="忠烈碑", descmapid=4, descpos="154,0,369", istransport=true, 
				desc="^ffffff       每天可以在流波的^ffcb4a忠烈碑^ffffff进行祭奠。\r       每次需要2组奇珍，完成祭奠任务后获得大量经验"
			}
MenologyRecommend['景仰先贤'] = 
			{ 
				id=18, iconpath1="景仰先贤.dds", iconpath2="景仰先贤大.dds", iconpath3="",
				rewardtype=002, maskinfo=0, name="景仰先贤", ismendisplay=false, mendisplaylevel=18,
				validtime="0:00-24:00", levelreq="1-150", needreborn=false, permitcount=1, cond="^ffcb4a15金", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="沈墨儒", istaskaward=false, taskid="12847",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：25"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="沈墨儒", descmapid=1, descpos="137,0,85", istransport=true, 
				desc="^ffffff       每天可以在^ffcb4a河阳沈墨儒^ffffff处交纳^ffcb4a15个金币^ffffff后领取^ffcb4a“景仰先贤”^ffffff的高经验奖励任务。\r       完成任务可以获得^ffcb4a25个太极金丹^ffffff的经验奖励。"
			}
MenologyRecommend['彻夜不眠'] = 
			{ 
				id=19, iconpath1="彻夜不眠.dds", iconpath2="彻夜不眠大.dds", iconpath3="",
				rewardtype=003, maskinfo=0, name="彻夜不眠", ismendisplay=false, mendisplaylevel=19,
				validtime="0:00-24:00", levelreq="60-150", needreborn=false, permitcount=-1, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="60级以上各地名医", istaskaward=false, taskid="11586,11588,11590,11592,11595,11598,11601,11618,11635,11638,11647,11654,11674,11678,11681,11739,11746,11754,11783,11791,11794,11796,11798,11800",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验\r\r^ffffff根据杀怪数目不同将获得大量经验。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="60级以上各地名医", descmapid=-1, descpos="9999,0,9999", istransport=false, 
				desc="^ffffff       60级之后，可以在每张地图的医师处领取对应等级的^ffcb4a彻夜不眠^ffffff任务。\r       任务为计时形式，根据杀怪数量计算获取的经验。\r       可以在时间未结束时直接提交任务获取经验，或者等待任务时间结束，自动获得相应的经验奖励。"
			}
MenologyRecommend['师门英雄帖'] = 
			{ 
				id=20, iconpath1="师门英雄帖.dds", iconpath2="师门英雄帖大.dds", iconpath3="",
				rewardtype=002, maskinfo=0, name="师门英雄帖", ismendisplay=false, mendisplaylevel=20,
				validtime="0:00-24:00", levelreq="30-150", needreborn=false, permitcount=-1, cond="^ffcb4a英雄帖", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="本职业门派使者", istaskaward=false, taskid="3162,3159,3165,3168,11682,3174,3171,3177,3180,11689,3186,3183,3189,3192,11696,3858,3855,3861,3865,11703,5628,5538,5625,5631,11710,5652,5634,5637,5655,11717,7614,7611,7617,7620,11724,7660,7657,7663,7666,11734",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量经验。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="本职业门派使者", descmapid=-1, descpos="9999,0,9999", istransport=false, 
				desc="^ffffff       从天音开始，当你的本门贡献度足够高时，可以在本门的门派使者那里领取不同档次的英雄帖。（每天可以领取一张）\r       完成英雄帖上的任务，可以换取不同的发光英雄帖。凭借发光英雄帖在门派使者处换取大量经验。（只要身上还有金字英雄帖，每天可重复完成此任务）"
			}
MenologyRecommend['破凶咒'] = 
			{ 
				id=21, iconpath1="破凶咒.dds", iconpath2="破凶咒大.dds", iconpath3="",
				rewardtype=003, maskinfo=0, name="破凶咒", ismendisplay=false, mendisplaylevel=21,
				validtime="0:00-24:00", levelreq="30-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="传功使者", istaskaward=false, taskid="8667,8669,5182,5184,8668,5183,5181,8670",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a完成后获得大量经验奖励。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="传功使者", descmapid=1, descpos="214,0,161", istransport=true, 
				desc="^ffffff       传功使者发布的破除凶咒任务。\r       杀死分布在地图上的^ffcb4a凶咒怪^ffffff，即可获得大量经验。"
			}
MenologyRecommend['建功立事'] = 
			{ 
				id=22, iconpath1="建功立事.dds", iconpath2="建功立事大.dds", iconpath3="",
				rewardtype=002, maskinfo=0, name="建功立事", ismendisplay=false, mendisplaylevel=22,
				validtime="0:00-24:00", levelreq="1-150", needreborn=false, permitcount=1, cond="^ffcb4a20金", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="帮会管理员", istaskaward=false, taskid="12856",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：40"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="帮会管理员", descmapid=1, descpos="209,0,169", istransport=true, 
				desc="^ffffff       已经加入帮派的成员每天可以在河阳的^ffcb4a帮派管理员方海平^ffffff处花费^ffcb4a20金^ffffff领取任务。\r       完成一系列委托。最终可以获得^ffcb4a40个太极金丹^ffffff的经验奖励。"
			}
MenologyRecommend['宝物'] = 
			{ 
				id=23, iconpath1="宝物.dds", iconpath2="宝物大.dds", iconpath3="",
				rewardtype=002, maskinfo=0, name="宝物", ismendisplay=false, mendisplaylevel=23,
				validtime="0:00-24:00", levelreq="1-150", needreborn=false, permitcount=1, cond="^ffcb4a15金", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="吴民财", istaskaward=false, taskid="12862",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：30"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="吴民财", descmapid=1, descpos="195,0,92", istransport=true, 
				desc="^ffffff       在河阳吴民财处领取委托任务，需要支付^ffcb4a15金^ffffff。\r       完成任务后获得^ffcb4a30个太极金丹^ffffff的经验奖励。"
			}
MenologyRecommend['金丹有价'] = 
			{ 
				id=24, iconpath1="金丹有价.dds", iconpath2="金丹有价大.dds", iconpath3="",
				rewardtype=002, maskinfo=0, name="金丹有价", ismendisplay=false, mendisplaylevel=24,
				validtime="0:00-24:00", levelreq="1-150", needreborn=false, permitcount=1, cond="^ffcb4a20金", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="何隆尧", istaskaward=false, taskid="12861",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：40"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="何隆尧", descmapid=1, descpos="184,0,77", istransport=true, 
				desc="^ffffff       在河阳的何隆尧处领取委托任务，需要支付^ffcb4a20金^ffffff。\r       完成任务后获得^ffcb4a40个太极金丹^ffffff的经验奖励。"
			}
MenologyRecommend['国家宝藏'] = 
			{ 
				id=25, iconpath1="国家宝藏.dds", iconpath2="国家宝藏大.dds", iconpath3="",
				rewardtype=030, maskinfo=0, name="国家宝藏", ismendisplay=false, mendisplaylevel=25,
				validtime="0:00-24:00", levelreq="60-150", needreborn=false, permitcount=3, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="天行者", istaskaward=false, taskid="12310,12311,12312,12313,12314,12315",
				award=
				{
					{awardid=0, awardiconpath="金钱类奖励.dds", awarddesc="^ffcb4a金币\r\r完成任务并回报天行者\r获取大量金钱。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="天行者", descmapid=1, descpos="159,0,103", istransport=true, 
				desc="^ffffff       按照天行者的要求杀死各地的云游天将。\r       回报天行者获取大量金钱。"
			}
MenologyRecommend['造化之悟'] = 
			{ 
				id=26, iconpath1="造化之悟.dds", iconpath2="造化之悟大.dds", iconpath3="",
				rewardtype=003, maskinfo=0, name="造化之悟", ismendisplay=false, mendisplaylevel=26,
				validtime="0:00-24:00", levelreq="135-150", needreborn=false, permitcount=-1, cond="^ffcb4a50金", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="轩辕祖师", istaskaward=false, taskid="19260",
				award=
				{
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a造化值\r\r^ffffff用来修炼仙魔佛造化技能之用。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="轩辕祖师", descmapid=2, descpos="41,45,-433", istransport=true, 
				desc="^ffffff       在青云^ffcb4a轩辕祖师^ffffff处可进行^ffcb4a道心^ffffff、^ffcb4a佛缘^ffffff、^ffcb4a魔性^ffffff的修炼，需要消耗50金，以击杀怪物数量得到造化值奖励，该任务限时24小时。"
			}
MenologyRecommend['瑞雪佳年'] = 
			{ 
				id=27, iconpath1="雪人总动员.dds", iconpath2="雪人总动员大.dds", iconpath3="",
				rewardtype=203, maskinfo=0, name="瑞雪佳年", ismendisplay=false, mendisplaylevel=27,
				validtime="0:00-24:00", levelreq="60-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="雪国使者", istaskaward=false, taskid="22494",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
					{awardid=21345, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
					{awardid=32934, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
				},
				periods=
				{
					{starttime="2010-12-18", endtime="2010-12-25"},
				}, 
				commendlevel=3, istasknpc=false, descnpc="雪国使者", descmapid=1, descpos="191,164,88", istransport=true, 
				desc="^ffffff       白雪飘飘，深冬严严，雪国使者在这个银装素裹的时节出现在河阳城里，原来他是来接去年送给碧瑶姑娘的小兔子们回雪国的，可是粗心大意的雪国使者不小心让小兔子们都跑掉了，河阳城的侠士们快来帮帮可怜的雪国使者，把小兔子捉回来吧。"
			}
MenologyRecommend['天池玄冰'] = 
			{ 
				id=28, iconpath1="雪人总动员.dds", iconpath2="雪人总动员大.dds", iconpath3="",
				rewardtype=203, maskinfo=0, name="天池玄冰", ismendisplay=false, mendisplaylevel=27,
				validtime="0:00-24:00", levelreq="60-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="雪国使者", istaskaward=false, taskid="22490",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
					{awardid=21345, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
				},
				periods=
				{
					{starttime="2010-12-18", endtime="2010-12-25"},
				}, 
				commendlevel=3, istasknpc=false, descnpc="雪国使者", descmapid=1, descpos="191,164,88", istransport=true, 
				desc="^ffffff       河阳城对于来自冰雪世界的使者来说，实在是太太太热了，可怜的雪国使者都快要融化了，快快帮雪国使者从昆仑的天池中采些昆仑玄冰来，让他凉爽一下吧。 \r^ffcb4a       奖励：得到1个限时1天的雪娃娃和50个太极金丹，150个造化珠。（一定概率获得永久的雪娃娃）"
			}
MenologyRecommend['雪人的圣诞心愿'] = 
			{ 
				id=29, iconpath1="雪人总动员.dds", iconpath2="雪人总动员大.dds", iconpath3="",
				rewardtype=300, maskinfo=0, name="雪人的圣诞心愿", ismendisplay=false, mendisplaylevel=27,
				validtime="0:00-24:00", levelreq="60-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="雪国使者", istaskaward=false, taskid="22489",
				award=
				{
					{awardid=22299, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
				},
				periods=
				{
					{starttime="2010-12-17", endtime="2010-12-24"},
				}, 
				commendlevel=3, istasknpc=false, descnpc="雪国使者", descmapid=1, descpos="191,164,88", istransport=true, 
				desc="^ffffff       雪国使者很担心跑掉的小兔子会被城外的野狼吃掉呢，快去帮帮他吧。"
			}
MenologyRecommend['圣诞许愿'] = 
			{ 
				id=30, iconpath1="雪人总动员.dds", iconpath2="雪人总动员大.dds", iconpath3="",
				rewardtype=300, maskinfo=0, name="圣诞许愿", ismendisplay=false, mendisplaylevel=27,
				validtime="0:00-24:00", levelreq="60-150", needreborn=false, permitcount=1, cond="^ffcb4a圣诞袜子", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="雪国使者", istaskaward=false, taskid="22488",
				award=
				{
					{awardid=22307, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
				},
				periods=
				{
					{starttime="2010-12-17", endtime="2010-12-24"},
				}, 
				commendlevel=3, istasknpc=false, descnpc="雪国使者", descmapid=1, descpos="191,164,88", istransport=true, 
				desc="^ffffff       快快将手中的圣诞袜子挂到河阳城中那些可爱的圣诞树上，到了平安夜的夜晚，圣诞老人会悄悄的在袜子里放上一份神秘的礼物喔，里面装了什么好东西呢？到了圣诞节的当天我们就会知道啦。"
			}
MenologyRecommend['圣诞老人的礼物'] = 
			{ 
				id=31, iconpath1="雪人总动员.dds", iconpath2="雪人总动员大.dds", iconpath3="",
				rewardtype=303, maskinfo=0, name="圣诞老人的礼物", ismendisplay=false, mendisplaylevel=27,
				validtime="0:00-24:00", levelreq="60-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="雪国使者", istaskaward=false, taskid="13658",
				award=
				{
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a秘密"}, 
				},
				periods=
				{
					{starttime="2010-12-25", endtime="2010-12-25"},
				}, 
				commendlevel=3, istasknpc=false, descnpc="雪国使者", descmapid=1, descpos="191,164,88", istransport=true, 
				desc="^ffffff       圣诞快乐！好期待啊！圣诞老人究竟送了我什么礼物呢？快从圣诞树上摘下来看一看吧。"
			}
MenologyRecommend['非诚勿扰'] = 
			{ 
				id=33, iconpath1="宠物使者的委托.dds", iconpath2="宠物使者的委托大.dds", iconpath3="",
				rewardtype=303, maskinfo=0, name="非诚勿扰", ismendisplay=false, mendisplaylevel=29,
				validtime="0:00-24:00", levelreq="30-150", needreborn=false, permitcount=3, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="河阳月老", istaskaward=false, taskid="23304",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
					{awardid=21345, awardiconpath="特殊物品奖励.dds", awarddesc=""},
					{awardid=42809, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a秘密"}, 
				},
				periods=
				{
					{starttime="2011-3-8", endtime="2011-3-21"},
				}, 
				commendlevel=3, istasknpc=false, descnpc="河阳月老", descmapid=1, descpos="180,164,104", istransport=true, 
				desc="^ffffff       30级以上的玩家在活动期间可找月老接取“非诚勿扰”任务，获得月老赠与的“鸳鸯兽”，“鸳鸯兽”会自动寻找与你有缘之人，找到有缘之人后，从“鸳鸯兽”吐落的“姻缘珠”中获得月老所需的物品，交给月老，完成任务。"
			}
MenologyRecommend['灵韵仙树'] = 
			{ 
				id=34, iconpath1="天地宝库.dds", iconpath2="天地宝库大.dds", iconpath3="",
				rewardtype=333, maskinfo=0, name="灵韵仙树", ismendisplay=false, mendisplaylevel=29,
				validtime="0:00-24:00", levelreq="30-150", needreborn=false, permitcount=3, cond=" ", 
				validday="1,2,3,4,5,6,7", line="15线", npc="司灵韵", istaskaward=false, taskid="20400",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
					{awardid=43345, awardiconpath="特殊物品奖励.dds", awarddesc=""},
					{awardid=21345, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a秘密"}, 
				},
				periods=
				{
					{starttime="2011-4-26", endtime="2011-5-14"},
				}, 
				commendlevel=3, istasknpc=false, descnpc="司灵韵", descmapid=1, descpos="119,164,-314", istransport=true, 
				desc="^ffffff       劳动节快乐！司灵韵（河阳相思木附近）得到一颗天界树苗，希望你能帮她培育长大。（活动开放时间：4月26日12：02-5月14日23：55）"
			}
MenologyRecommend['圣诞活动'] = 
			{ 
				id=35, iconpath1="雪人总动员.dds", iconpath2="雪人总动员大.dds", iconpath3="",
				rewardtype=303, maskinfo=0, name="圣诞活动", ismendisplay=false, mendisplaylevel=30,
				validtime="0:00-24:00", levelreq="30-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="圣诞老人", istaskaward=false, taskid="25349,25297",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。"}, 
					{awardid=50913, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
					{awardid=50914, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
					{awardid=50912, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
					{awardid=50911, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
					{awardid=41171, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
				},
				periods=
				{
					{starttime="2011-12-18", endtime="2011-12-25"},
				}, 
				commendlevel=3, istasknpc=false, descnpc="圣诞老人", descmapid=1, descpos="180,164,125", istransport=true, 
				desc="^ffffff       “祝福送给你”任务：根据任务描述中的提示，将圣诞老人的祝福送到指定的人手上。“顽皮的孩子”任务：在河阳城外找到四处乱跑的顽皮孩子，击败带回圣诞老人处。"
			}
MenologyRecommend['元旦活动'] = 
			{ 
				id=36, iconpath1="宝物.dds", iconpath2="宝物大.dds", iconpath3="",
				rewardtype=302, maskinfo=0, name="元旦活动", ismendisplay=false, mendisplaylevel=31,
				validtime="0:00-24:00", levelreq="30-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", line="1-15线", npc="新年使者", istaskaward=false, taskid="25291,25292",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。"}, 
					{awardid=44463, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
					{awardid=22343, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
				},
				periods=
				{
					{starttime="2012-1-1", endtime="2012-1-7"},
				}, 
				commendlevel=3, istasknpc=false, descnpc="新年使者", descmapid=1, descpos="180,164,125", istransport=true, 
				desc="^ffffff       在新年使者处选择“参加元旦活动”，进入元旦活动场地，完成元旦任务便可获得奖励。"
			}

function MenologyRecommend:GetSelf()
	return self;
end

MenologyWeekly = {}
MenologyWeekly['逐波圣灵'] = 
			{ 
				id=101, iconpath1="逐波圣灵.dds", iconpath2="逐波圣灵大.dds",
				rewardtype=203, maskinfo=0, name="逐波圣灵", ismendisplay=false, 
				validtime="11:50-12:00", levelreq="1-150", needreborn=false, permitcount=1, cond="^ffcb4a10金", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="15线", npc="上古奇人", istaskaward=false, taskid="14607",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff常规获得^ffcb4a20个太极金丹\r^ffffff若获胜可额外获得^ffcb4a100个太极金丹^ffffff奖励。"}, 
					{awardid=21345, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a造化珠\r\r^ffffff若获胜可获得200个造化珠^ffffff奖励。"},
				},
				commendlevel=4, istasknpc=false, descnpc="上古奇人", descmapid=1, descpos="193,0,123", istransport=true, 
				desc="^ffffff       每天11：50分在河阳^ffcb4a上古奇人^ffffff处花费^ffcb4a10金^ffffff购买神兽令牌，传送到流波海底观看神兽逐波竞速。活动结束可从夺魁神兽处获得^ffcb4a20个太极金丹^ffffff的奖励。若所购令牌刚好为夺魁神兽，还可获得神兽所赐的^ffcb4a100个太极金丹^ffffff，^ffcb4a200个造化珠^ffffff奖励。\r"
			}

MenologyWeekly['钓鱼大赛'] = 
			{ 
				id=102, iconpath1="钓鱼大赛.dds", iconpath2="钓鱼大赛大.dds", 
				rewardtype=103, maskinfo=0, name="钓鱼大赛", ismendisplay=false,
				validtime="18:00-19:00", levelreq="45-150", needreborn=false, permitcount=1, cond="^ffcb4a报名券", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-5线", npc="云游四海的渔夫", istaskaward=false, taskid="1",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff根据钓取的物品不同在完成相应任\r务后将获得不同金丹奖励。"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a特殊称谓\r\r^ffffff根据钓取的物品不同在完成相应任\r务后将奖励不同称谓。"},
				},
				commendlevel=3, istasknpc=false, descnpc="云游四海的渔夫", descmapid=1, descpos="121,0,145", istransport=true, 
				desc="^ffffff       在流波举行的钓鱼大赛，玩家先用报名卷在云游四海的渔夫处换取^ffcb4a鱼饵^ffffff，然后赶往流波参加大赛。"
			}
MenologyWeekly['戾气危机'] = 
			{ 
				id=103, iconpath1="戾气危机.dds", iconpath2="戾气危机大.dds", 
				rewardtype=002, maskinfo=0, name="戾气危机", ismendisplay=false,
				validtime="18:00-20:00", levelreq="15-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="陆雪琪", istaskaward=false, taskid="12717",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：25"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="陆雪琪", descmapid=2, descpos="403,75,-192", istransport=true, 
				desc="^ffffff       青云山方向突然间天现异象，从^ffcb4a青云 陆雪琪^ffffff处得知，青云后山祖师祠堂处设置了^ffcb4a三根阵法石柱^ffffff，用以镇压戾气，如今石柱多有损耗，阵法已经逐渐失效。请大家帮助修复三根石柱，恢复阵法的力量，可获得^ffcb4a25个太极金丹^ffffff奖励。"
			}
MenologyWeekly['雪人总动员'] = 
			{ 
				id=104, iconpath1="雪人总动员.dds", iconpath2="雪人总动员大.dds",
				rewardtype=200, maskinfo=0, name="雪人总动员", ismendisplay=false,
				validtime="18:45-21:45", levelreq="60-150", needreborn=false, permitcount=1, cond=" ", 
				validday="7", displayday="周六", line="15线", npc="昆仑仙子", istaskaward=false, taskid="10595",
				award=
				{
					{awardid=16385, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a天上天下，通存通\r取。可以在青云仓\r库老板钱多多或巡\r天右都尉处换取丰\r厚奖励"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="昆仑仙子", descmapid=10, descpos="-194,123,355", istransport=true, 
				desc="^ffffff       在^ffcb4a昆仑仙子^ffffff处接受任务,可选择任何一方雪人投注,胜利后可获得相应数量的^ffcb4a乾坤通宝^ffffff。"
			}
MenologyWeekly['新科试炼'] = 
			{ 
				id=105, iconpath1="新科试炼.dds", iconpath2="新科试炼大.dds", 
				rewardtype=203, maskinfo=0, name="新科试炼", ismendisplay=false,
				validtime="19:00-19:15", levelreq="1-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="系统定时开启", istaskaward=false, taskid="12868",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量经验。\r个数：答题每获得10分奖励5个金丹"}, 
					{awardid=19252, awardiconpath="", awarddesc=""},
					{awardid=19253, awardiconpath="", awarddesc=""},
				},
				commendlevel=3, istasknpc=false, descnpc="系统定时开启", descmapid=-1, descpos="9999,0,9999", istransport=false, 
				desc="^ffffff       每天^ffcb4a19：00^ffffff整，玩家会接到^ffcb4a新科试炼^ffffff活动的邀请。接受邀请，参与试炼。获得经验奖励的掌握更多知识。\r新科试炼^ffcb4a前10名^ffffff都可以获得^ffcb4a轩辕通宝^ffffff的奖励。并且可以获得稀有的试炼标志称号。"
			}
MenologyWeekly['修真总动员'] = 
			{ 
				id=106, iconpath1="修真总动员.dds", iconpath2="修真总动员大.dds", 
				rewardtype=200, maskinfo=0, name="修真总动员", ismendisplay=false,
				validtime="19:00-22:00", levelreq="60-150", needreborn=false, permitcount=1, cond=" ", 
				validday="6", displayday="周五", line="1-15线", npc="阿福", istaskaward=false, taskid="11339",
				award=
				{
					{awardid=16385, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a天上天下，通存通\r取。可以在青云仓\r库老板钱多多或巡\r天右都尉处换取丰\r厚奖励。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="阿福", descmapid=2, descpos="179,0,108", istransport=true, 
				desc="^ffffff       在^ffcb4a青云阿福^ffffff处领取任务，完成任务需要5个用^ffcb4a虎魄冰晶^ffffff换取的棒棒糖，成功后奖励200^ffcb4a乾坤通宝^ffffff。"
			}
MenologyWeekly['追风圣灵'] = 
			{ 
				id=107, iconpath1="追风圣灵.dds", iconpath2="追风圣灵大.dds", 
				rewardtype=203, maskinfo=0, name="追风圣灵", ismendisplay=false, 
				validtime="19:20-19:30", levelreq="1-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="15线", npc="上古奇人", istaskaward=false, taskid="14989",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff常规获得^ffcb4a20个太极金丹\r^ffffff若获胜可额外获得^ffcb4a100个太极金丹^ffffff奖励。"}, 
					{awardid=21345, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a造化珠\r\r^ffffff若获胜可获得200个造化珠^ffffff奖励。"},
				},
				commendlevel=3, istasknpc=false, descnpc="上古奇人", descmapid=1, descpos="193,0,123", istransport=true, 
				desc="^ffffff       每天19：20分在河阳^ffcb4a上古奇人^ffffff处花费^ffcb4a10金^ffffff购买神兽令牌，传送到焚香谷观看神兽追风竞速。活动结束可从夺魁神兽处获得^ffcb4a20个太极金丹^ffffff的奖励。若所购令牌刚好为夺魁神兽，还可获得神兽所赐的^ffcb4a100个太极金丹^ffffff，^ffcb4a200个造化珠^ffffff奖励。\r^ff0000注：神兽追风竞速过程中需全程关注，否则可能发生神兽走失事件。"
			}
MenologyWeekly['天下会武'] = 
			{ 
				id=108, iconpath1="天下会武.dds", iconpath2="天下会武大.dds", 
				rewardtype=003, maskinfo=0, name="天下会武", ismendisplay=true,
				validtime="19:30-21:10", levelreq="90-134", needreborn=false, permitcount=1, cond=" ", 
				validday="2,3", displayday="周一、二", line="1-15线", npc="紫微道光元帅", istaskaward=false, taskid="9620,9629,10529,9638,9647,10530",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="完成会武可以获得大量经验奖励。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="紫微道光元帅", descmapid=1, descpos="185,0,140", istransport=true, 
				desc="^ffffff       在^ffcb4a河阳紫微帝君^ffffff处领取天下会武的任务，使用^ffcb4a六道法莲^ffffff可自由选择阵营。\r       完成会武可以获得大量经验。"
			}
MenologyWeekly['金丹之术'] = 
			{ 
				id=109, iconpath1="金丹之术.dds", iconpath2="金丹之术大.dds",
				rewardtype=202, maskinfo=0, name="金丹之术", ismendisplay=false,
				validtime="19:30-21:30", levelreq="30-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="抱朴子", istaskaward=false, taskid="13091",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量经验。\r个数：25"}, 
					{awardid=21510, awardiconpath="item1.dds", awarddesc=""},
				},
				commendlevel=3, istasknpc=false, descnpc="抱朴子", descmapid=2, descpos="96,64,203", istransport=true, 
				desc="^ffffff       在青云举行的大型炼丹活动，玩家在^ffcb4a青云 抱朴子^ffffff处领取前置任务。\r 完成任务后玩家可以获得^ffcb4a碎雪星芒^ffffff及^ffcb4a太极金丹^ffffff奖励。\r       50个碎雪星芒可在陆雪琪处兑换道具^ffcb4a雪之星^ffffff，可以对项链（天罪，龙啸，龙骧，神道，神罚，神藏）进行强化。"
			}
MenologyWeekly['兽妖总动员'] = 
			{ 
				id=110, iconpath1="兽妖总动员.dds", iconpath2="兽妖总动员大.dds",
				rewardtype=200, maskinfo=0, name="兽妖总动员", ismendisplay=false,
				validtime="19:45-21:00", levelreq="60-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1", displayday="周日", line="1-15线", npc="法觉", istaskaward=false, taskid="11262",
				award=
				{
					{awardid=16385, awardiconpath="", awarddesc=""}, 
				},
				commendlevel=3, istasknpc=false, descnpc="法觉", descmapid=3, descpos="-121,0,-123", istransport=true, 
				desc="^ffffff       防守天音千佛塔处的镇妖塔，成功抵挡三波攻击后可得到60^ffcb4a乾坤通宝^ffffff的奖励，怪物掉落的^ffcb4a血玉^ffffff也可以换取一定数量的通宝。"
			}
MenologyWeekly['星宿北斗'] = 
			{ 
				id=111, iconpath1="星宿北斗.dds", iconpath2="星宿北斗大.dds", 
				rewardtype=003, maskinfo=0, name="星宿北斗", ismendisplay=false,
				validtime="20:30-22:30", levelreq="30-150", needreborn=false, permitcount=1, cond=" ", 
				validday="4,5,7", displayday="周三四六", line="1-15线", npc="星宿挑战使者", istaskaward=false, taskid="5469,5470,5471,5472,9046,9047,9048",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验\r\r^ffffff完成任务后将获得大量经验。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="星宿挑战使者", descmapid=1, descpos="113,0,62", istransport=true, 
				desc="^ffffff       在各自等级段对应的地图可找挑战使者接受任务，按要求击杀对应星宿怪，击杀七个星宿怪可得到最高奖励。"
			}
MenologyWeekly['八极战魂'] = 
			{ 
				id=112, iconpath1="八极战魂.dds", iconpath2="八极战魂大.dds",
				rewardtype=202, maskinfo=0, name="八极战魂", ismendisplay=false,
				validtime="0:00-24:00", levelreq="1-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="封印石", istaskaward=false, taskid="22314",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。"}, 
					{awardid=21345, awardiconpath="item1.dds", awarddesc=""},
					{awardid=21154, awardiconpath="item1.dds", awarddesc=""},
				},
				commendlevel=3, istasknpc=false, descnpc="封印石", descmapid=2, descpos="93,64,208", istransport=true, 
				desc="^ffffff       完成^ffcb4a国家宝藏^ffffff或者^ffcb4a天诛谕令^ffffff任务后，有几率获得任务道具^ffcb4a启玄印^ffffff，可以用来破除位于青云的^ffcb4a八极镇石^ffffff封印，并换得^ffcb4a八极元珠^ffffff，当封印石收集足够数量的^ffcb4a启玄印^ffffff后，封印效果将消失，可以召唤封印石内的战魂。\r       战魂被消灭后可于^ffcb4a青云 混元真君^ffffff处用^ffcb4a八极元珠^ffffff领取奖励"
			}
MenologyWeekly['天行者'] = 
			{ 
				id=113, iconpath1="天行者.dds", iconpath2="天行者大.dds",
				rewardtype=003, maskinfo=0, name="天行者", ismendisplay=false,
				validtime="0:00-24:00", levelreq="45-150", needreborn=false, permitcount=1, cond="^ffcb4a飞剑", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="天行者", istaskaward=false, taskid="12193",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
					{awardid=20102, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="天行者", descmapid=1, descpos="159,0,103", istransport=true, 
				desc="^ffffff       河阳的^ffcb4a天行者^ffffff提供获得海量经验的绝佳机会。\r 驾驭^ffcb4a飞剑^ffffff，与天行者相会在蔚蓝天空。"
			}
MenologyWeekly['行天'] = 
			{ 
				id=114, iconpath1="行天.dds", iconpath2="行天大.dds",
				rewardtype=003, maskinfo=0, name="行天", ismendisplay=false,
				validtime="0:00-24:00", levelreq="45-150", needreborn=false, permitcount=1, cond="^ffcb4a行天之证", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="天行者", istaskaward=false, taskid="12844",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：50"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="天行者", descmapid=1, descpos="159,0,103", istransport=true, 
				desc="^ffffff       每天在完成天行者的任务后，会获得道具^ffcb4a“行天之证”^ffffff，凭借“行天之证”并交纳^ffcb4a25金币^ffffff可以在天行者处领取后续高经验奖励任务任务^ffcb4a“行天”^ffffff。\r       完成任务可以获得^ffcb4a50个太极金丹^ffffff的经验奖励。"
			}
MenologyWeekly['忠烈碑'] = 
			{ 
				id=115, iconpath1="忠烈碑.dds", iconpath2="忠烈碑大.dds",
				rewardtype=002, maskinfo=0, name="忠烈碑", ismendisplay=false,
				validtime="0:00-24:00", levelreq="50-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="忠烈碑", istaskaward=false, taskid="2237,2276,2277,6204,6205,7464,7465",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验\r\r^ffffff完成任务后将获得大量经验。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="忠烈碑", descmapid=4, descpos="154,0,369", istransport=true, 
				desc="^ffffff       每天可以在流波的^ffcb4a忠烈碑^ffffff进行祭奠。\r       每次需要2组奇珍，完成祭奠任务后获得大量经验"
			}
MenologyWeekly['景仰先贤'] = 
			{ 
				id=116, iconpath1="景仰先贤.dds", iconpath2="景仰先贤大.dds",
				rewardtype=002, maskinfo=0, name="景仰先贤", ismendisplay=false,
				validtime="0:00-24:00", levelreq="1-150", needreborn=false, permitcount=1, cond="^ffcb4a15金", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="沈墨儒", istaskaward=false, taskid="12847",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：25"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="沈墨儒", descmapid=1, descpos="137,0,85", istransport=true, 
				desc="^ffffff       每天可以在^ffcb4a河阳沈墨儒^ffffff处交纳^ffcb4a15个金币^ffffff后领取^ffcb4a“景仰先贤”^ffffff的高经验奖励任务。\r       完成任务可以获得^ffcb4a25个太极金丹^ffffff的经验奖励。"
			}
MenologyWeekly['彻夜不眠'] = 
			{ 
				id=117, iconpath1="彻夜不眠.dds", iconpath2="彻夜不眠大.dds",
				rewardtype=003, maskinfo=0, name="彻夜不眠", ismendisplay=false,
				validtime="0:00-24:00", levelreq="60-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="60级以上各地名医", istaskaward=false, taskid="11586,11588,11590,11592,11595,11598,11601,11618,11635,11638,11647,11654,11674,11678,11681,11739,11746,11754,11783,11791,11794,11796,11798,11800",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验\r\r^ffffff根据杀怪数目不同将获得大量经验。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="60级以上各地名医", descmapid=-1, descpos="9999,0,9999", istransport=false, 
				desc="^ffffff       60级之后，可以在每张地图的医师处领取对应等级的^ffcb4a彻夜不眠^ffffff任务。\r       任务为计时形式，根据杀怪数量计算获取的经验。\r       可以在时间未结束时直接提交任务获取经验，或者等待任务时间结束，自动获得相应的经验奖励。"
			}
MenologyWeekly['师门英雄帖'] = 
			{ 
				id=118, iconpath1="师门英雄帖.dds", iconpath2="师门英雄帖大.dds",
				rewardtype=002, maskinfo=0, name="师门英雄帖", ismendisplay=false,
				validtime="0:00-24:00", levelreq="30-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="本职业门派使者", istaskaward=false, taskid="3162,3159,3165,3168,11682,3174,3171,3177,3180,11689,3186,3183,3189,3192,11696,3858,3855,3861,3865,11703,5628,5538,5625,5631,11710,5652,5634,5637,5655,11717,7614,7611,7617,7620,11724,7660,7657,7663,7666,11734",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量经验。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="本职业门派使者", descmapid=-1, descpos="9999,0,9999", istransport=false, 
				desc="^ffffff       从天音开始，当你的本门贡献度足够高时，可以在本门的门派使者那里领取不同档次的英雄帖。（每天可以领取一张）\r       完成英雄帖上的任务，可以换取不同的发光英雄帖。凭借发光英雄帖在门派使者处换取大量经验。（只要身上还有金字英雄帖，每天可重复完成此任务）"
			}
MenologyWeekly['破凶咒'] = 
			{ 
				id=119, iconpath1="破凶咒.dds", iconpath2="破凶咒大.dds",
				rewardtype=003, maskinfo=0, name="破凶咒", ismendisplay=false,
				validtime="0:00-24:00", levelreq="30-150", needreborn=false, permitcount=1, cond=" ", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="传功使者", istaskaward=false, taskid="8667,8669,5182,5184,8668,5183,5181,8670",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a完成后获得大量经验奖励。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="传功使者", descmapid=1, descpos="214,0,161", istransport=true, 
				desc="^ffffff       传功使者发布的破除凶咒任务。\r       杀死分布在地图上的^ffcb4a凶咒怪^ffffff，即可获得大量经验。"
			}
MenologyWeekly['建功立事'] = 
			{ 
				id=120, iconpath1="建功立事.dds", iconpath2="建功立事大.dds",
				rewardtype=002, maskinfo=0, name="建功立事", ismendisplay=false,
				validtime="0:00-24:00", levelreq="1-150", needreborn=false, permitcount=1, cond="^ffcb4a20金", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="帮会管理员", istaskaward=false, taskid="12856",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：40"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="帮会管理员", descmapid=1, descpos="209,0,169", istransport=true, 
				desc="^ffffff       已经加入帮派的成员每天可以在河阳的^ffcb4a帮派管理员方海平^ffffff处花费^ffcb4a20金^ffffff领取任务。\r       完成一系列委托。最终可以获得^ffcb4a40个太极金丹^ffffff的经验奖励。"
			}
MenologyWeekly['宝物'] = 
			{ 
				id=121, iconpath1="宝物.dds", iconpath2="宝物大.dds", 
				rewardtype=002, maskinfo=0, name="宝物", ismendisplay=false,
				validtime="0:00-24:00", levelreq="1-150", needreborn=false, permitcount=1, cond="^ffcb4a15金", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="吴民财", istaskaward=false, taskid="12862",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：30"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="吴民财", descmapid=1, descpos="195,0,92", istransport=true, 
				desc="^ffffff       在河阳吴民财处领取委托任务，需要支付^ffcb4a15金^ffffff。\r       完成任务后获得^ffcb4a30个太极金丹^ffffff的经验奖励。"
			}
MenologyWeekly['金丹有价'] = 
			{ 
				id=122, iconpath1="金丹有价.dds", iconpath2="金丹有价大.dds", 
				rewardtype=002, maskinfo=0, name="金丹有价", ismendisplay=false,
				validtime="0:00-24:00", levelreq="1-150", needreborn=false, permitcount=1, cond="^ffcb4a20金", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="何隆尧", istaskaward=false, taskid="12861",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。\r个数：40"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="何隆尧", descmapid=1, descpos="184,0,77", istransport=true, 
				desc="^ffffff       在河阳的何隆尧处领取委托任务，需要支付^ffcb4a20金^ffffff。\r       完成任务后获得^ffcb4a40个太极金丹^ffffff的经验奖励。"
			}
MenologyWeekly['国家宝藏'] = 
			{ 
				id=123, iconpath1="国家宝藏.dds", iconpath2="国家宝藏大.dds",
				rewardtype=030, maskinfo=0, name="国家宝藏", ismendisplay=false,
				validtime="0:00-24:00", levelreq="60-150", needreborn=false, permitcount=3, cond=" ", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="天行者", istaskaward=false, taskid="12310,12311,12312,12313,12314,12315",
				award=
				{
					{awardid=0, awardiconpath="金钱类奖励.dds", awarddesc="^ffcb4a金币\r\r完成任务并回报天行者\r获取大量金钱。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="天行者", descmapid=1, descpos="159,0,103", istransport=true, 
				desc="^ffffff       按照天行者的要求杀死各地的云游天将。\r       回报天行者获取大量金钱。"
			}
MenologyWeekly['造化之悟'] = 
			{ 
				id=124, iconpath1="造化之悟.dds", iconpath2="造化之悟大.dds",
				rewardtype=003, maskinfo=0, name="造化之悟", ismendisplay=false,
				validtime="0:00-24:00", levelreq="135-150", needreborn=false, permitcount=1, cond="^ffcb4a50金", 
				validday="1,2,3,4,5,6,7", displayday="周一-周日", line="1-15线", npc="轩辕祖师", istaskaward=false, taskid="19260",
				award=
				{
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a造化值\r\r^ffffff用来修炼仙魔佛造化技能之用。"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="轩辕祖师", descmapid=2, descpos="41,45,-433", istransport=true, 
				desc="^ffffff       在青云^ffcb4a轩辕祖师^ffffff处可进行^ffcb4a道心^ffffff、^ffcb4a佛缘^ffffff、^ffcb4a魔性^ffffff的修炼，需要消耗50金，以击杀怪物数量得到造化值奖励，该任务限时24小时。"
			}
			
function MenologyWeekly:GetSelf()
	return self;
end

MenologyBattleField = {}
MenologyBattleField['楚汉军魂'] 	= 
			{ 
				id=202, iconpath1="楚汉军魂.dds", iconpath2="楚汉军魂大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="楚汉军魂", ismendisplay=false, mendisplaylevel=150,
				validtime="13:00-14:30", levelreq="15-150", needreborn=true, permitcount=1, cond="", 
				validday="1,7", displayday="周六、日", line="10线", npc="太虚幻境仙官", istaskaward=false, taskid="1",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r^ffffff获得后自动提升大量经验。"}, 
					{awardid=24708, awardiconpath="item1.dds", awarddesc="^ffcb4a楚汉青钱\r\r用来换取离系列装备的道具。"},
				},
				commendlevel=2, istasknpc=false, descnpc="", descmapid=1, descpos="170,0,170", istransport=true, 
				desc="^ffffff       九里山垓下古战场，你是辅佐汉王刘邦，一匡天下，开创百年基业，还是救援楚王项羽，力挽狂澜，成就世间霸王？问鼎乾坤，等待你的抉择……（要求飞升角色）"
			}
---MenologyBattleField['演武阵'] = 
---			{ 
---				id=203, iconpath1="演武阵.dds", iconpath2="演武阵大.dds", iconpath3="icon\\Menology_huiwu.tga", 
---				rewardtype=0, maskinfo=0, name="演武阵" , ismendisplay=false, mendisplaylevel=149, 
---				validtime="19:00-22:00", levelreq="90-150", needreborn=false, permitcount=1, cond="", 
---				validday="1", displayday="周日", line="14线", npc="武尊神 刘岳", istaskaward=false, taskid="1",
---				award=
---				{
---					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r^ffffff获得后自动提升大量经验。"}, 
---					{awardid=34183, awardiconpath="item1.dds", awarddesc="12345"},
---				},
---				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="170,0,176", istransport=true, 
---				desc="^ffffff       演武大阵，数名高手共同竞技，不仅需要有以一敌百的实力，更考验在危机中的生存能力，最终决出唯一的胜利者，体验唯我独尊的畅快与喜悦。（要求未飞升角色）"
---			}
---MenologyBattleField['幻世演武阵'] = 
---			{ 
---				id=204, iconpath1="幻世演武阵.dds", iconpath2="幻世演武阵大.dds", iconpath3="icon\\Menology_huiwu.tga", 
---				rewardtype=0, maskinfo=0, name="幻世演武阵" , ismendisplay=false, mendisplaylevel=148, 
---				validtime="19:00-22:00", levelreq="90-150", needreborn=true, permitcount=1, cond="", 
---				validday="1", displayday="周日", line="15线", npc="武尊神 刘岳", istaskaward=false, taskid="1",
---				award=
---				{
---					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r^ffffff获得后自动提升大量经验。"}, 
---					{awardid=31282, awardiconpath="", awarddesc=""},
---				},
---				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="170,0,176", istransport=true, 
---				desc="^ffffff       演武大阵，数名高手共同竞技，不仅需要有以一敌百的实力，更考验在危机中的生存能力，最终决出唯一的胜利者，体验唯我独尊的畅快与喜悦。（要求飞升角色）"
---			}
MenologyBattleField['炎狱'] = 
			{ 
				id=205, iconpath1="炎狱.dds", iconpath2="炎狱大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="炎狱" , ismendisplay=false, mendisplaylevel=147, 
				validtime="0:00-24:00", levelreq="90-104", needreborn=false, permitcount=10, cond="", 
				validday="1,2,3,4,5,6,7", displayday="每天", line="1-15线", npc="诛仙战神", istaskaward=false, taskid="1",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验"},
					{awardid=10537, awardiconpath="", awarddesc=""},
				},
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="193,0,139", istransport=true, 
				desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，此地烈火熊熊，进入则身受热浪蒸腾之苦，在其中历练可大幅度提升自身修为。（要求未飞升角色）"
			}
MenologyBattleField['武围'] = 
			{ 
				id=206, iconpath1="武围.dds", iconpath2="武围大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="武围" , ismendisplay=false, mendisplaylevel=146, 
				validtime="0:00-24:00", levelreq="105-119", needreborn=false, permitcount=10, cond="", 
				validday="1,2,3,4,5,6,7", displayday="每天", line="1-15线", npc="诛仙战神", istaskaward=false, taskid="1",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验"},
					{awardid=10537, awardiconpath="", awarddesc=""},
				},
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="193,0,139", istransport=true, 
				desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，此处机关遍地，危机重重，在其中历练可大幅度提升自身修为。（要求未飞升角色）"
			}
MenologyBattleField['界河'] = 
			{ 
				id=207, iconpath1="界河.dds", iconpath2="界河大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="界河" , ismendisplay=false, mendisplaylevel=145, 
				validtime="0:00-24:00", levelreq="120-134", needreborn=false, permitcount=10, cond="", 
				validday="1,2,3,4,5,6,7", displayday="每天", line="1-15线", npc="诛仙战神", istaskaward=false, taskid="1",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验"},
					{awardid=10537, awardiconpath="", awarddesc=""},
				},
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="193,0,139", istransport=true, 
				desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，双方以河为界，斗智斗勇，在其中历练可大幅度提升自身修为。（要求未飞升角色）"
			}
MenologyBattleField['天弈'] = 
			{ 
				id=208, iconpath1="天弈.dds", iconpath2="天弈大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="天弈" , ismendisplay=false, mendisplaylevel=144, 
				validtime="0:00-24:00", levelreq="135-150", needreborn=false, permitcount=10, cond="", 
				validday="1,2,3,4,5,6,7", displayday="每天", line="1-15线", npc="诛仙战神", istaskaward=false, taskid="1",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验"},
					{awardid=10537, awardiconpath="", awarddesc=""},
				},
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="193,0,139", istransport=true, 
				desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，内呈棋盘之势，步步惊心，在其中历练可大幅度提升自身修为。（要求未飞升角色）"
			}
MenologyBattleField['幻世炎狱'] = 
			{ 
				id=209, iconpath1="幻世炎狱.dds", iconpath2="幻世炎狱大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="幻世炎狱" , ismendisplay=false, mendisplaylevel=143, 
				validtime="12:00-24:00", levelreq="90-104", needreborn=true, permitcount=10, cond="", 
				validday="1,2,3,4,5,6,7", displayday="每天", line="1-15线", npc="诛仙战神", istaskaward=false, taskid="1",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验"},
					{awardid=23858, awardiconpath="", awarddesc=""},
				},
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="193,0,139", istransport=true, 
				desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，此地烈火熊熊，进入则身受热浪蒸腾之苦，在其中历练可大幅度提升自身修为。（要求飞升角色）"
			}
MenologyBattleField['幻世武围'] = 
			{ 
				id=210, iconpath1="幻世武围.dds", iconpath2="幻世武围大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="幻世武围" , ismendisplay=false, mendisplaylevel=142, 
				validtime="12:00-24:00", levelreq="105-119", needreborn=true, permitcount=10, cond="", 
				validday="1,2,3,4,5,6,7", displayday="每天", line="1-15线", npc="诛仙战神", istaskaward=false, taskid="1",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验"},
					{awardid=23858, awardiconpath="", awarddesc=""},
				},
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="193,0,139", istransport=true, 
				desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，此处机关遍地，危机重重，在其中历练可大幅度提升自身修为。（要求飞升角色）"
			}
MenologyBattleField['幻世界河'] = 
			{ 
				id=211, iconpath1="幻世界河.dds", iconpath2="幻世界河大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="幻世界河" , ismendisplay=false, mendisplaylevel=141, 
				validtime="12:00-24:00", levelreq="120-134", needreborn=true, permitcount=10, cond="", 
				validday="1,2,3,4,5,6,7", displayday="每天", line="1-15线", npc="诛仙战神", istaskaward=false, taskid="1",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验"},
					{awardid=23858, awardiconpath="", awarddesc=""},
				},
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="193,0,139", istransport=true, 
				desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，双方以河为界，斗智斗勇，在其中历练可大幅度提升自身修为。（要求飞升角色）"
			}
MenologyBattleField['幻世天弈'] = 
			{  
				id=212, iconpath1="幻世天弈.dds", iconpath2="幻世天弈大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="幻世天弈" , ismendisplay=false, mendisplaylevel=140, 
				validtime="12:00-24:00", levelreq="135-150", needreborn=true, permitcount=10, cond="", 
				validday="1,2,3,4,5,6,7", displayday="每天", line="1-15线", npc="诛仙战神", istaskaward=false, taskid="1",
				award=
				{
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验"},
					{awardid=23858, awardiconpath="", awarddesc=""},
				},
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="193,0,139", istransport=true, 
				desc="^ffffff       隐仙阁与逸龙轩两派，定期举行天下会武之地，内呈棋盘之势，步步惊心，在其中历练可大幅度提升自身修为。（要求飞升角色）"
			}
MenologyBattleField['草庙村'] = 
			{ 
				id=213, iconpath1="草庙村.dds", iconpath2="草庙村大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="草庙村" , ismendisplay=false, mendisplaylevel=139, 
				validtime="0:00-24:00", levelreq="30-89", needreborn=false, permitcount=3, cond="", 
				validday="1,2,3,4,5,6,7", displayday="每天", line="1-15线", npc="时空幻境管理员", istaskaward=false, taskid="1",
				award=
				{
					{awardid=31757, awardiconpath="item1.dds", awarddesc="12345"},
					{awardid=4180, awardiconpath="item1.dds", awarddesc="12345"},
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。"},
				},
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="173,0,167", istransport=true, 
				desc="^ffffff       宁静的草庙村风云诡谲，所有的传说都将从这里开始。你回到梦开始的地方，重拾曾经的感动，一切一切须在此开始，也终须在此了结。"
			}
MenologyBattleField['真武剑阁'] = 
			{ 
				id=214, iconpath1="真武剑阁.dds", iconpath2="真武剑阁大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="真武剑阁" , ismendisplay=false, mendisplaylevel=138, 
				validtime="0:00-24:00", levelreq="75-119", needreborn=false, permitcount=3, cond="", 
				validday="1,2,3,4,5,6,7", displayday="每天", line="1-15线", npc="时空幻境管理员",	istaskaward=false, taskid="1",
				award=
				{
					{awardid=31758, awardiconpath="item1.dds", awarddesc="12345"},
					{awardid=4180, awardiconpath="item1.dds", awarddesc="12345"},
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。"},
				},
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="173,0,167", istransport=true, 
				desc="^ffffff       真武大帝藏剑的宫殿，相传遗留下诸多的真武心解，更有诸多上古神兵遗留于此，是三界修士历练寻珍的不二宝地。"
			}
MenologyBattleField['天帝宝库'] = 
			{ 
				id=215, iconpath1="天地宝库.dds", iconpath2="天地宝库大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="天帝宝库" , ismendisplay=false, mendisplaylevel=137, 
				validtime="0:00-24:00", levelreq="120-150", needreborn=true, permitcount=3, cond="", 
				validday="1,2,3,4,5,6,7", displayday="每天", line="1-15线", npc="时空幻境管理员", istaskaward=false, taskid="1",
				award=
				{
					{awardid=34229, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验"},
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。"},
				},
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="173,0,167", istransport=true, 
				desc="^ffffff       天帝宝库藏有太初神物，但其间禁制重重，稍有不慎便万劫难复。如今又到了神物出世时节，谁人能够获得太初神宝，谁又是宝库的应缘之人?（要求飞升角色）"
			}
MenologyBattleField['家族副本'] = 
			{ 
				id=216, iconpath1="建功立事.dds", iconpath2="建功立事大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="家族副本" , ismendisplay=false, mendisplaylevel=136, 
				validtime="0:00-24:00", levelreq="20-150", needreborn=false, permitcount=3, cond="", 
				validday="1,2,3,4,5,6,7", displayday="每天", line="1-15线", npc="时空幻境管理员", istaskaward=false, taskid="1",
				award=
				{
					{awardid=21345, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验"},
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。"},
				},
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="173,0,167", istransport=true, 
				desc="^ffffff       幻月洞府乃青云门禁地，为重振青云威望，门中弟子陆雪琪勇闯幻月洞府，谁能协她一臂之力？（需级别为20级以上的家族成员）"
			}
MenologyBattleField['鸿蒙试炼'] = 
			{ 
				id=217, iconpath1="天行者.dds", iconpath2="天行者大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="鸿蒙试炼" , ismendisplay=false, mendisplaylevel=135, 
				validtime="0:00-24:00", levelreq="120-150", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,4,5,6,7", displayday="每天", line="1-15线", npc="武德星君", istaskaward=false, taskid="1",
				award=
				{
					{awardid=21345, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验"},
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。"},
					{awardid=41668, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。"},
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a震系列装备\r^ffffff适合等级：飞升140级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a特殊属性称谓\r^ffffff减免他方暴击率及暴击伤害"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="178,0,166", istransport=true, 
				desc="^ffffff       鸿蒙之地，神秘难测，内有各种珍奇异宝，也有无数恶灵宵小，带上你的队友，挑战一下自己的实力吧！（需级别为120级以上飞升角色，建议4人以上）"
			}
MenologyBattleField['焚香谷副本'] = 
			{ 
				id=218, iconpath1="演武阵.dds", iconpath2="演武阵大.dds", iconpath3="icon\\Menology_huiwu.tga", 
				rewardtype=0, maskinfo=0, name="焚香谷副本" , ismendisplay=false, mendisplaylevel=134, 
				validtime="0:00-24:00", levelreq="60-150", needreborn=false, permitcount=1, cond="", 
				validday="1,2,3,4,5,6,7", displayday="每天", line="1-15线", npc="八荒来使", istaskaward=false, taskid="1",
				award=
				{
					{awardid=21345, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a大量经验"},
					{awardid=0, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。"},
					{awardid=41668, awardiconpath="经验类奖励.dds", awarddesc="^ffcb4a太极金丹\r\r^ffffff获得后自动提升大量修为。"},
					{awardid=49370, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a特殊属性称谓\r^ffffff减免他方暴击率及暴击伤害"}, 
				},
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="171,0,180", istransport=true, 
				desc="^ffffff       隐匿数年的焚香、太昊重现世间，一时之间流言纷纷。传说谁能破解八荒玄火阵之谜，谁就能称霸世间！修真者们聚集起来：前往焚香谷的中心玄火坛，寻找八荒玄火阵！（需级别为60级以上角色，建议10人以上）"
			}

function MenologyBattleField:GetSelf()
	return self;
end

MenologyMonster = {}
MenologyMonster['蛙王'] = 
			{ 
				id=632, iconpath1="蛙王.dds", iconpath2="Models\\NPCs\\怪物\\爬虫\\青蛙\\蛙王.ecm", 
				rewardtype=110101, maskinfo=0, name="蛙王", ismendisplay=false, mendisplaylevel=5,
				validtime="0:00-24:00", levelreq="1", needreborn=false, permitcount=1, cond="", 
				validday="1,2,3,4,5,6,7", line="2线", npc="蛙王", istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：10级"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：10级"},
				},
				commendlevel=4, istasknpc=false, descnpc="", descmapid=1, descpos="102,0,-174", istransport=true, 
				npclv=11, refreshperiod="8小时", recommendlv="15", recommendplayernum="1", 
				desc="^ffffff       受兽妖妖气影响，变成了半妖的蛙类，通体红色，形态凶恶，以妖气将其附近的青蛙都制住，且不时出来伤人。"
			}
MenologyMonster['悍匪'] = 
			{ 
				id=633, iconpath1="悍匪.dds", iconpath2="Models\\NPCs\\怪物\\BOSS\\八荒火龙\\八荒火龙.ecm", 
				rewardtype=0, maskinfo=0, name="悍匪", ismendisplay=false, mendisplaylevel=6, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="7线", npc="悍匪", istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：10级"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：10级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="-41,0,327", istransport=true, 
				npclv=15, refreshperiod="8小时", recommendlv="15", recommendplayernum="1", 
				desc="^ffffff       土匪小头目，原为名门子弟，因品行不端被逐出，从此加入土匪一列，其功力高于一般匪众，受命于匪首，时常前往恶虎岗巡视。"
			}
MenologyMonster['神火教长老'] = 
			{ 
				id=634, iconpath1="神火教长老.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="神火教长老", ismendisplay=false, mendisplaylevel=7, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="4线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：10级"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：10级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=1, descpos="409,171,400", istransport=true, 
				npclv=18, refreshperiod="8小时", recommendlv="25", recommendplayernum="1", 
				desc="^ffffff       神火教长老，其教义推崇火神，长老功力高强，在神火教中颇有威望，仅次于其教教主。护教之心甚笃，绝不放过任何闯入神火教坛的人。"
			}
MenologyMonster['金刚'] = 
			{ 
				id=635, iconpath1="金刚.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="金刚", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="8线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：25级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a散仙套装及武器\r^ffffff适合等级：30级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=2, descpos="-63,0,34", istransport=true, 
				npclv=23, refreshperiod="8小时", recommendlv="25", recommendplayernum="1", 
				desc="^ffffff       一只神奇的棕毛大猩猩，体高力大，长年长于山野，奔跑迅速，有着不为人知的往事。"
			}
MenologyMonster['神火魔祖'] = 
			{ 
				id=636, iconpath1="神火魔祖.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="神火魔祖", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="6线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：25级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a散仙套装及武器\r^ffffff适合等级：30级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=2, descpos="-445,0,-363", istransport=true, 
				npclv=34, refreshperiod="8小时", recommendlv="35", recommendplayernum="1", 
				desc="^ffffff       神火教教主，其教义推崇火神，教主功力非同一般，世人少有匹敌者，若有人闯入神火教圣坛被其发现，通常凶多吉少。"
			}
MenologyMonster['巨熊妖'] = 
			{ 
				id=637, iconpath1="巨熊妖.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="巨熊妖", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="8线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：40级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a散仙套装及武器\r^ffffff适合等级：45级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=3, descpos="-329,0,-24", istransport=true, 
				npclv=50, refreshperiod="8小时", recommendlv="50", recommendplayernum="1", 
				desc="^ffffff       棕色的熊妖，力大无穷，奔跑迅速，生性既猛且蠢，遇到人便穷追猛扑，一掌可将人畜拍成肉饼。"
			}
MenologyMonster['噬血恶僧'] = 
			{ 
				id=638, iconpath1="噬血恶僧.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="噬血恶僧", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="5线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：40级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a散仙套装及武器\r^ffffff适合等级：45级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=3, descpos="-293,312,304", istransport=true, 
				npclv=50, refreshperiod="8小时", recommendlv="50", recommendplayernum="1", 
				desc="^ffffff       遥远国度而来的邪教僧人，坚信只有食用人肉才能长生不老，平日躲藏在众僧人之间，唯有寻找活人食用时方现出真身，已有多名修真者欲将其除去反为其所害。"
			}
MenologyMonster['骷髅将军'] = 
			{ 
				id=639, iconpath1="骷髅将军.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="骷髅将军", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="10线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：40级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a散仙套装及武器\r^ffffff适合等级：45级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=3, descpos="125,0,346", istransport=true, 
				npclv=50, refreshperiod="8小时", recommendlv="50", recommendplayernum="1", 
				desc="^ffffff       此物生前本为修道之人，因死后葬身之处为妖气所腐蚀，并赋予其强烈的杀意，从此变为恶灵，统领众骷髅魔兵，祸害人间。"
			}
MenologyMonster['渔人蛛'] = 
			{ 
				id=640, iconpath1="渔人蛛.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="渔人蛛", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="6线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：60级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：55级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：60级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=4, descpos="-159,0,394", istransport=true, 
				npclv=65, refreshperiod="8小时", recommendlv="65", recommendplayernum="1", 
				desc="^ffffff       凶残的水生蜘蛛，毒性巨大，爬行速度极快，擅长猎杀其他生物，即使不用毒液也能轻松捕获猎物。"
			}
MenologyMonster['海盗船长'] = 
			{ 
				id=641, iconpath1="海盗船长.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="海盗船长", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="2线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：60级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：55级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：60级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=4, descpos="10,65,-229", istransport=true, 
				npclv=65, refreshperiod="8小时", recommendlv="65", recommendplayernum="1", 
				desc="^ffffff       长年漂泊在海上的海盗头目，疯狂的海上掠夺者，擅长水陆两栖作战，对财宝十分贪婪，偶尔会回到流波海域补给船只。"
			}
MenologyMonster['犀甲之王'] = 
			{ 
				id=642, iconpath1="犀甲之王.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="犀甲之王", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="7线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：60级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：55级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：60级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=4, descpos="-418,0,-411", istransport=true, 
				npclv=65, refreshperiod="8小时", recommendlv="65", recommendplayernum="1", 
				desc="^ffffff       擅长战斗的犀甲人首领，身材高大威猛，能以一敌百，身上的犀甲可同时发挥进攻和防守的作用，穿在身上如虎添翼。"
			}
MenologyMonster['死泽鱼妖'] = 
			{ 
				id=643, iconpath1="死泽鱼妖.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="死泽鱼妖", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="3线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：70级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：75级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=5, descpos="267,0,306", istransport=true, 
				npclv=80, refreshperiod="8小时", recommendlv="80", recommendplayernum="1", 
				desc="^ffffff       鱼人族中的精锐，武功高强，能说人语，还可以使用异族法术。常出现在阴冷幽暗的沼泽深处。"
			}
MenologyMonster['万虫之母'] = 
			{ 
				id=644, iconpath1="万虫之母.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="万虫之母", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="8线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：70级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：75级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=5, descpos="371,0,55", istransport=true, 
				npclv=80, refreshperiod="8小时", recommendlv="80", recommendplayernum="1", 
				desc="^ffffff       沼泽中的毒虫之首，能大量繁殖毒虫，吞噬方圆十里之内的生物，人畜遇上无一幸免。"
			}
MenologyMonster['死泽厉鬼'] = 
			{ 
				id=645, iconpath1="死泽厉鬼.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="死泽厉鬼", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="4线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：70级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：75级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=5, descpos="-148,0,144", istransport=true, 
				npclv=80, refreshperiod="8小时", recommendlv="80", recommendplayernum="1", 
				desc="^ffffff       误入死亡沼泽的修道中人，困死在幽暗的沼泽里，临死前的怨念与不甘成为戾气，使之难入轮回，化为厉鬼存于沼泽中。"
			}
MenologyMonster['金瞳妖蝠'] = 
			{ 
				id=673, iconpath1="金瞳妖蝠.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="金瞳妖蝠", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="6线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：70级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：75级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=9, descpos="302,0,-416", istransport=false, 
				npclv=81, refreshperiod="8小时", recommendlv="85", recommendplayernum="1", 
				desc="^ffffff       蝙蝠之王，已修炼千年，双目呈金色，锐利的眼神夺人心智，双翼展开可达三米，阻挡一切敌人的退路。"
			}
MenologyMonster['炼血死士'] = 
			{ 
				id=674, iconpath1="炼血死士.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="炼血死士", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="2线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：70级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：75级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=9, descpos="319,0,-262", istransport=false, 
				npclv=81, refreshperiod="8小时", recommendlv="85", recommendplayernum="1", 
				desc="^ffffff       练血堂的中坚力量，不惧生死，僵尸一般的存在，需吸食人畜之血维持生机。"
			}
MenologyMonster['黑水玄蛇'] = 
			{ 
				id=675, iconpath1="黑水玄蛇.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="黑水玄蛇", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="7线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：70级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：75级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=9, descpos="-335,0,-431", istransport=false, 
				npclv=81, refreshperiod="8小时", recommendlv="85", recommendplayernum="1", 
				desc="^ffffff       上古魔兽，凶悍无比。蛇身粗逾四丈，长逾百丈。食神仙药而不死，寿过万年，非之天敌黄鸟不能除之。"
			}
MenologyMonster['大黑蛭'] = 
			{ 
				id=676, iconpath1="大黑蛭.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="大黑蛭", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="3线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：85级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：90级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=12, descpos="-158,0,154", istransport=false, 
				npclv=96, refreshperiod="24小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       体型巨大的怪物，长年生活于地下，不见阳光，浑身长满触角，以触角吸引一切活物做食物，近其身绝不可活。出现在黑石洞地宫。"
			}
MenologyMonster['焰魔'] = 
			{ 
				id=677, iconpath1="焰魔.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="焰魔", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="2线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：85级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：90级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=12, descpos="-339,0,264", istransport=false, 
				npclv=96, refreshperiod="8小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       火之守护兽，守护着万火之精，寻常水源无法扑灭，可焚烧天地万物。出现在黑石洞地宫。"
			}
MenologyMonster['洪荒兽王'] = 
			{ 
				id=681, iconpath1="洪荒兽王.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="洪荒兽王", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="9线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：85级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：90级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=6, descpos="-306,0,-227", istransport=true, 
				npclv=95, refreshperiod="8小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       蛮荒之地特有的种族，身体高大，凶恶丑陋，但力大无穷，铜身铁臂，具有刀枪不入的体格。"
			}
MenologyMonster['血狮'] = 
			{ 
				id=647, iconpath1="血狮.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="血狮", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="5线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：85级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：90级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=6, descpos="-336,0,-236", istransport=true, 
				npclv=95, refreshperiod="8小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       十三妖王之一的狮王，全身通红，似以鲜血浸染，凶残狡猾，并具备高强的妖法。"
			}
MenologyMonster['玄冥幽鬼'] = 
			{ 
				id=648, iconpath1="玄冥幽鬼.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="玄冥幽鬼", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="10线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：85级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：90级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=6, descpos="-405,0,128", istransport=true, 
				npclv=95, refreshperiod="8小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       游荡并横行玄冥之界的鬼魂，怨灵的一种，不惧生死，不入轮回，对活人有着强烈的敌意。"
			}
MenologyMonster['熔岩之王'] = 
			{ 
				id=679, iconpath1="熔岩之王.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="熔岩之王", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="4线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：85级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：90级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=12, descpos="243,0,-234", istransport=false, 
				npclv=96, refreshperiod="8小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       生活在熔岩之中的恐怖魔兽，擅长用熔岩的力量攻击其他一切生物，若离开熔岩，便失去所有力量。"
			}
MenologyMonster['咆哮之王'] = 
			{ 
				id=680, iconpath1="咆哮之王.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="咆哮之王", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="9线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：85级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：90级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=12, descpos="-89,0,-212", istransport=false, 
				npclv=96, refreshperiod="8小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       性格残暴，声如厉雷的猛兽，生性既猛且蠢，空具一身蛮力，可智取不可强夺。"
			}
MenologyMonster['黄鸟'] = 
			{ 
				id=5592, iconpath1="黄鸟.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="黄鸟", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="10线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a兽神套装及武器\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：85级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=12, descpos="156,0,-265", istransport=false, 
				npclv=96, refreshperiod="24小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       四神兽之一的黄鸟，九天灵鸟，黑水玄蛇的天敌。"
			}
MenologyMonster['烛龙'] = 
			{ 
				id=5593, iconpath1="烛龙.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="烛龙", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="6线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a兽神套装及武器\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：85级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=12, descpos="460,0,-188", istransport=false, 
				npclv=96, refreshperiod="24小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       四神兽之一的烛龙，上古神兽，身长千里，神力烛照九泉。"
			}
MenologyMonster['夔牛'] = 
			{ 
				id=5594, iconpath1="夔牛.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="夔牛", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="2线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a兽神套装及武器\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：85级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=12, descpos="261,0,-96", istransport=false, 
				npclv=96, refreshperiod="24小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       四神兽之一的夔牛，入水即风雨，目光如日月，一足能走，其声如雷。"
			}
MenologyMonster['饕餮'] = 
			{ 
				id=5595, iconpath1="饕餮.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="饕餮", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="7线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a兽神套装及武器\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：85级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=12, descpos="115,0,-167", istransport=false, 
				npclv=96, refreshperiod="24小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       四神兽之一的饕餮，凶残好吃，见到什么吃什么，是兽妖的守护兽。"
			}
MenologyMonster['九命猫王'] = 
			{ 
				id=678, iconpath1="九命猫王.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="九命猫王", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="5线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：90级"}, 
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：85级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：90级"},
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=12, descpos="-223,0,-268", istransport=false, 
				npclv=96, refreshperiod="8小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       猫族的统治者，受妖气侵蚀，可使用妖法，具有不死之身。"
			}
MenologyMonster['鱼人王'] = 
			{ 
				id=5596, iconpath1="鱼人王.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="鱼人王", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="6线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff王道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：85级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：105级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：105级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：75级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=7, descpos="357,113,355", istransport=true, 
				npclv=95, refreshperiod="8小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       鱼人族的统治者，武功高强，能说人语，会多种法术，身旁鱼人护卫众多，难以接近。"
			}
MenologyMonster['不死天尸'] = 
			{ 
				id=10491, iconpath1="不死天尸.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="不死天尸", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="2线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff龙啸"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：115级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：120级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：120级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：105级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=8, descpos="-378,0,289", istransport=true, 
				npclv=125, refreshperiod="8小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       传说中的千年古尸，一直出没于南疆万蛇殿中，汲取天地灵气补充法力。"
			}
MenologyMonster['八荒火龙'] = 
			{ 
				id=10492, iconpath1="八荒火龙.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="八荒火龙", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="8线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：115级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：120级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：120级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：105级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=14, descpos="-391,0,-101", istransport=false, 
				npclv=126, refreshperiod="8小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       “八荒玄火阵”的终极召唤灵物，南疆古老的巫族传说中，毁灭世间万物的可怕凶兽。"
			}
MenologyMonster['兽神'] = 
			{ 
				id=10493, iconpath1="兽神.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="兽神", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="不定", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：115级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a兽神套装及武器\r^ffffff适合等级：120级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：120级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：105级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=14, descpos="-358,0,356", istransport=false, 
				npclv=126, refreshperiod="不定", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       冷漠淡然的妖艳少年，为情而生，为爱而亡，与南疆巫女玲珑演绎了一场凄美的爱情神话。"
			}
MenologyMonster['玄冥鬼手'] = 
			{ 
				id=10487, iconpath1="玄冥鬼手.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="玄冥鬼手", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="4线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：115级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：120级"},
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合等级：120级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：105级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=14, descpos="-386,0,235", istransport=false, 
				npclv=126, refreshperiod="8小时", recommendlv="100", recommendplayernum="1", 
				desc="^ffffff       天地浩劫产生的异物，戾气凝聚其上，具强大的杀气，被上古高人囚禁于镇魔古洞中。"
			}
MenologyMonster['昆仑奴'] = 
			{ 
				id=10669, iconpath1="昆仑奴.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="昆仑奴", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="7线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a130级首饰"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：135级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a135级配方"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：105级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=10, descpos="-333,0,121", istransport=true, 
				npclv=135, refreshperiod="8.5小时", recommendlv="135", recommendplayernum="1", 
				desc="^ffffff       异族进贡的奴隶，本性情温良、踏实耿直，途中被妖兽所袭，沾染妖气，后于昆仑山胡乱修炼，汲取仙气，变成半妖半仙。"
			}
MenologyMonster['炼狱魔尊'] = 
			{ 
				id=15702, iconpath1="炼狱魔尊.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="炼狱魔尊", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="8线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：148级"},
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=17, descpos="-370,0,305", istransport=false, 
				npclv=155, refreshperiod="8小时", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       炼狱众魔之首，统率炼狱之中百鬼千魔，法力高强，凶猛残暴，可发毁天灭地之威。"
			}
MenologyMonster['妙翅鸟'] = 
			{ 
				id=15703, iconpath1="黄鸟.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="妙翅鸟", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="8线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：148级"},
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=17, descpos="-20,0,22", istransport=false, 
				npclv=155, refreshperiod="8小时", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       炼狱妙翅鸟蛋中孕育的神鸟，每当蛋壳破碎便可重生一次，为上古灵鸟之一。"
			}
MenologyMonster['黑心老人'] = 
			{ 
				id=13576, iconpath1="黑心老人.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="黑心老人", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="14线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：148级"},
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=16, descpos="-323,0,-52", istransport=false, 
				npclv=155, refreshperiod="不定", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       八百年前一统邪教的伟大人物，练成噬血珠，给人间带来极大祸害。"
			}
MenologyMonster['金铃夫人'] = 
			{ 
				id=13575, iconpath1="金铃夫人.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="金铃夫人", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="14线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：148级"},
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=16, descpos="-405,0,-221", istransport=false, 
				npclv=155, refreshperiod="不定", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       聪明绝顶，道行精深，对魔教经典天书有大悟于心，于三百年前独立创立“合欢派”，成为魔教中四大鼎盛门派之一。"
			}
MenologyMonster['普智'] = 
			{ 
				id=13579, iconpath1="普智.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="普智", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="14线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：148级"},
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=16, descpos="-13,0,-217", istransport=false, 
				npclv=155, refreshperiod="不定", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       天音寺四大神僧之一，心怀大智，欲参破长生之谜，在联合佛道二家的努力失败后，被噬血珠侵蚀心志，遁入魔道。"
			}
MenologyMonster['万剑一'] = 
			{ 
				id=13578, iconpath1="万剑一.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="万剑一", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="14线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：148级"},
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=16, descpos="-105,0,-76", istransport=false, 
				npclv=155, refreshperiod="不定", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       道行高深的青云弟子，性格潇洒豪放，资质惊人，于幻月洞中点化道玄，并将诛仙剑交予张小凡斩杀魔教。"
			}
MenologyMonster['法莲王'] = 
			{ 
				id=10670, iconpath1="法莲王.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="法莲王", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="3线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：150级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=11, descpos="358,185,-380", istransport=true, 
				npclv=155, refreshperiod="9小时", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       修罗七王之一，偶尔出现在修罗法莲寺中，汲取寺内生灵之气，提升法力。"
			}
MenologyMonster['归蝶王'] = 
			{ 
				id=10671, iconpath1="归蝶王.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="归蝶王", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="8线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：150级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=11, descpos="-156,141,102", istransport=true, 
				npclv=155, refreshperiod="9.5小时", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       修罗七王之一，偶尔出现在修罗界归蝶塔中，妖艳貌美，体态婀娜，法力高强。"
			}
MenologyMonster['天忍王'] = 
			{ 
				id=10672, iconpath1="天忍王.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="天忍王", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="4线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：150级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=11, descpos="133,141,102", istransport=true, 
				npclv=155, refreshperiod="10小时", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       修罗七王之一，偶尔出现在修罗界天忍塔内，全身披满钢甲，且可召唤坐骑魔獒，攻击敌人。"
			}
MenologyMonster['魔獒'] = 
			{ 
				id=10673, iconpath1="魔獒.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="魔獒", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="4线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：148级"},
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=11, descpos="133,141,102", istransport=true, 
				npclv=155, refreshperiod="10小时", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       修罗七王之天忍王的坐骑，性烈凶猛，忠实护主，若主人被攻击则不顾一切冲往敌人。"
			}
MenologyMonster['樱木王'] = 
			{ 
				id=10674, iconpath1="樱木王.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="樱木王", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="5线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：150级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=11, descpos="-384,138,71", istransport=true, 
				npclv=155, refreshperiod="11小时", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       修罗七王之一，同时拥有人与植物双重身体，长期的变异造成其性格的扭曲，对人怨念极深，死后魂灵也让人不得消停。"
			}
MenologyMonster['阴阳王'] = 
			{ 
				id=10675, iconpath1="阴阳王.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="阴阳王", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="10线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：150级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=11, descpos="-158,140,442", istransport=true, 
				npclv=155, refreshperiod="11.5小时", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       修罗七王之一，被枷锁束缚于其阴阳难分的巢穴之中，日日挣扎却始终无法解脱，倘若有人靠近，极有可能成为其深深怨气的发泄目标。"
			}
MenologyMonster['风火王'] = 
			{ 
				id=10676, iconpath1="风火王.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="风火王", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="6线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：150级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=11, descpos="407,122,309", istransport=true, 
				npclv=155, refreshperiod="12小时", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       修罗七王之一，居于修罗血池之中，双手可分别召唤风火之力，对入侵者绝不手软。"
			}
MenologyMonster['安土王'] = 
			{ 
				id=10677, iconpath1="安土王.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="安土王", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="2线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff神道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：150级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=11, descpos="-11,148,264", istransport=true, 
				npclv=155, refreshperiod="12.5小时", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       修罗七王之一，居于修罗大殿，身具大地之力，有震慑天地之威，多条手臂更为其增加了无穷的力量。"
			}
MenologyMonster['玄天上将'] = 
			{ 
				id=10678, iconpath1="玄天上将.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="玄天上将", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="9线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff天道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：150级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：105级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=15, descpos="-444,0,-237", istransport=false, 
				npclv=155, refreshperiod="24小时", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       相传为真武大帝座下侍将，真武大帝封剑隐去之后，将其留在昆仑仙境驻守。"
			}
MenologyMonster['开明兽'] = 
			{ 
				id=10680, iconpath1="开明兽.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="开明兽", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="2线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff天道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：150级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：105级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=15, descpos="-16,0,85", istransport=false, 
				npclv=155, refreshperiod="8小时", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       昆仑山守护神之一，终日瞪视昆仑山，威震百灵，使得任何异常事物都无法进入昆仑，维护了昆仑仙境的平和安宁。"
			}
MenologyMonster['冰姹玄女'] = 
			{ 
				id=11563, iconpath1="冰姹玄女.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="冰姹玄女", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="7线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff天道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：150级"}, 
					{awardid=0, awardiconpath="特殊物品奖励.dds", awarddesc="^ffcb4a装备配方\r^ffffff适合生产等级：10级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：105级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=15, descpos="-50,0,186", istransport=false, 
				npclv=155, refreshperiod="8小时", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       昆仑之巅的守护神，召唤玄冰之力，护住昆仑山，使凡人异物难以闯入。"
			}
MenologyMonster['混沌'] = 
			{ 
				id=10679, iconpath1="混沌.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="混沌", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="5线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff天道"}, 
					{awardid=0, awardiconpath="戒指类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙套装及武器\r^ffffff适合等级：150级"}, 
					{awardid=0, awardiconpath="宝物.dds", awarddesc="^ffcb4a法宝相关\r^ffffff适合等级：105级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=15, descpos="391,0,400", istransport=false, 
				npclv=155, refreshperiod="8小时", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       镇压在昆仑之西的神兽，有目而不见，有耳而不闻，有腹无五脏，有肠直而不旋，难辨是非善恶不明之兽。"
			}
MenologyMonster['邪恶先锋·凯'] = 
			{ 
				id=22107, iconpath1="战神·龙戊.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="邪恶先锋·凯", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="不定", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="鞋子类.dds", awarddesc="^ffcb4a真仙鞋\r^ffffff适合等级：144级"}, 
					{awardid=0, awardiconpath="鞋子类.dds", awarddesc="^ffcb4a坤系列鞋\r^ffffff适合等级：飞升135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=10, descpos="-347,121,-117", istransport=true, 
				npclv=150, refreshperiod="不定", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       八级战魂之一，被封印在八卦镇石之中，一旦封印开启，其便引领邪恶之力而来，需及时将其除去，否则天地剧变，苍生遭劫。"
			}
MenologyMonster['邪恶先锋·涛'] = 
			{ 
				id=22106, iconpath1="黑心老人.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="邪恶先锋·涛", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="不定", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="鞋子类.dds", awarddesc="^ffcb4a真仙鞋\r^ffffff适合等级：144级"}, 
					{awardid=0, awardiconpath="鞋子类.dds", awarddesc="^ffcb4a坤系列鞋\r^ffffff适合等级：飞升135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=10, descpos="-364,121,-113", istransport=true, 
				npclv=150, refreshperiod="不定", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       八级战魂之一，被封印在八卦镇石之中，一旦封印开启，其便引领邪恶之力而来，需及时将其除去，否则天地剧变，苍生遭劫。"
			}
MenologyMonster['背叛者·娟'] = 
			{ 
				id=22252, iconpath1="金铃夫人.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="背叛者·娟", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="不定", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙衣\r^ffffff适合等级：146级"}, 
					{awardid=0, awardiconpath="头盔类.dds", awarddesc="^ffcb4a坤系列帽子\r^ffffff适合等级：飞升135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=8, descpos="-169,0,-83", istransport=true, 
				npclv=150, refreshperiod="不定", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       八级战魂之一，被封印在八卦镇石之中，一旦封印开启，其便引领邪恶之力而来，需及时将其除去，否则天地剧变，苍生遭劫。"
			}
MenologyMonster['背叛者·瞳'] = 
			{ 
				id=22251, iconpath1="背叛者·瞳.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="背叛者·瞳", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="不定", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙衣\r^ffffff适合等级：146级"}, 
					{awardid=0, awardiconpath="头盔类.dds", awarddesc="^ffcb4a坤系列帽子\r^ffffff适合等级：飞升135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=8, descpos="-165,0,-98", istransport=true, 
				npclv=150, refreshperiod="不定", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       八级战魂之一，被封印在八卦镇石之中，一旦封印开启，其便引领邪恶之力而来，需及时将其除去，否则天地剧变，苍生遭劫。"
			}
MenologyMonster['背叛者·宇'] = 
			{ 
				id=22253, iconpath1="玄天上将.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="背叛者·宇", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="不定", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙衣\r^ffffff适合等级：146级"}, 
					{awardid=0, awardiconpath="头盔类.dds", awarddesc="^ffcb4a坤系列帽子\r^ffffff适合等级：飞升135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=8, descpos="-147,0,-99", istransport=true, 
				npclv=150, refreshperiod="不定", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       八级战魂之一，被封印在八卦镇石之中，一旦封印开启，其便引领邪恶之力而来，需及时将其除去，否则天地剧变，苍生遭劫。"
			}
MenologyMonster['密探·永'] = 
			{ 
				id=23165, iconpath1="黑心老人.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="密探·永", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="不定", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a真仙衣\r^ffffff适合等级：146级"}, 
					{awardid=0, awardiconpath="头盔类.dds", awarddesc="^ffcb4a坤系列帽子\r^ffffff适合等级：飞升135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=7, descpos="242,0,19", istransport=true, 
				npclv=150, refreshperiod="不定", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       八级战魂之一，被封印在八卦镇石之中，一旦封印开启，其便引领邪恶之力而来，需及时将其除去，否则天地剧变，苍生遭劫。"
			}
MenologyMonster['冥界·古猿'] = 
			{ 
				id=35239, iconpath1="冥界·古猿.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="冥界·古猿", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="10线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a震系列装备\r^ffffff适合等级：飞升135级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：150级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=10, descpos="267,0,350", istransport=true, 
				npclv=150, refreshperiod="周一", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       冥界七王之一，利用人间的混乱冲破时空界点来到人间，从冥间带来的强大力量使其具有睥睨天下苍生之气魄，他们的目的只有一个：建造一个属于自己的全新的世界。"
			}
MenologyMonster['冥界·玄鸟'] = 
			{ 
				id=35240, iconpath1="黄鸟.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="冥界·玄鸟", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="10线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a震系列装备\r^ffffff适合等级：飞升135级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：150级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=5, descpos="-155,0,-394", istransport=true, 
				npclv=150, refreshperiod="周五", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff冥界七王之一，利用人间的混乱冲破时空界点来到人间，从冥间带来的强大力量使其具有睥睨天下苍生之气魄，他们的目的只有一个：建造一个属于自己的全新的世界。"
			}
MenologyMonster['冥界·饕餮'] = 
			{ 
				id=35242, iconpath1="饕餮.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="冥界·饕餮", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="10线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a震系列装备\r^ffffff适合等级：飞升135级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：150级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=4, descpos="-382,0,-361", istransport=true, 
				npclv=150, refreshperiod="周六", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff冥界七王之一，利用人间的混乱冲破时空界点来到人间，从冥间带来的强大力量使其具有睥睨天下苍生之气魄，他们的目的只有一个：建造一个属于自己的全新的世界。"
			}
MenologyMonster['冥界·圣使'] = 
			{ 
				id=35241, iconpath1="冥界·圣使.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="冥界·圣使", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="10线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a震系列装备\r^ffffff适合等级：飞升135级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：150级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=7, descpos="-371,0,17", istransport=true, 
				npclv=150, refreshperiod="周三", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       冥界七王之一，利用人间的混乱冲破时空界点来到人间，从冥间带来的强大力量使其具有睥睨天下苍生之气魄，他们的目的只有一个：建造一个属于自己的全新的世界。"
			}
MenologyMonster['冥界·天狼'] = 
			{ 
				id=35236, iconpath1="冥界·天狼.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="冥界·天狼", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="10线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a震系列装备\r^ffffff适合等级：飞升135级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：150级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=6, descpos="365,0,-382", istransport=true, 
				npclv=150, refreshperiod="周四", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       冥界七王之一，利用人间的混乱冲破时空界点来到人间，从冥间带来的强大力量使其具有睥睨天下苍生之气魄，他们的目的只有一个：建造一个属于自己的全新的世界。"
			}
MenologyMonster['冥界·血狮'] = 
			{ 
				id=35238, iconpath1="冥界·血狮.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="冥界·血狮", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="10线", npc="神火教长老", istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a震系列装备\r^ffffff适合等级：飞升135级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：150级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=8, descpos="344,0,362", istransport=true, 
				npclv=150, refreshperiod="周二", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff冥界七王之一，利用人间的混乱冲破时空界点来到人间，从冥间带来的强大力量使其具有睥睨天下苍生之气魄，他们的目的只有一个：建造一个属于自己的全新的世界。"
			}
MenologyMonster['冥界·兽神'] = 
			{ 
				id=35243, iconpath1="冥界·兽神.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="冥界·兽神", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="10线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=35407, awardiconpath="项链类.dds", awarddesc="玲珑珠泪"}, 
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a震系列装备\r^ffffff适合等级：飞升135级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a真仙武器\r^ffffff适合等级：150级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=3, descpos="-213,0,-21", istransport=true, 
				npclv=150, refreshperiod="周日", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff       冥界七王之一，利用人间的混乱冲破时空界点来到人间，从冥间带来的强大力量使其具有睥睨天下苍生之气魄，他们的目的只有一个：建造一个属于自己的全新的世界。"
			}
MenologyMonster['瘟皇·秦广'] = 
			{ 
				id=25949, iconpath1="瘟皇·秦广.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="瘟皇·秦广", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="13线", npc="神火教长老", istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a乾系列装备\r^ffffff适合等级：飞升135级"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a坤系列武器\r^ffffff适合等级：飞升135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=5, descpos="-60,0,205", istransport=true, 
				npclv=150, refreshperiod="13:00", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff专司人间寿夭生死册集，接引超生，统管幽冥吉凶，若能将其战胜，便可超渡人间一段怨魂，"
			}
MenologyMonster['战神·龙戊'] = 
			{ 
				id=25950, iconpath1="战神·龙戊.dds", iconpath2="Models\\NPCs\\怪物\\人形\\暴雪之王\\暴雪之王.ecm", 
				rewardtype=0, maskinfo=0, name="战神·龙戊", ismendisplay=false, mendisplaylevel=8, 
				validtime="9:00-12:00", levelreq="1", needreborn=true, permitcount=1, cond="", 
				validday="1,2,3,5,6,7", line="13线", npc="神火教长老",istaskaward=false,  taskid="1", relacontroler="", 
				award=
				{
					{awardid=0, awardiconpath="项链类.dds", awarddesc="^ffcb4a首饰\r^ffffff适合等级：140级"}, 
					{awardid=23360, awardiconpath="", awarddesc="^ffcb4a千华龙筋"},
					{awardid=0, awardiconpath="衣服类.dds", awarddesc="^ffcb4a坤系列装备\r^ffffff适合等级：飞升135级服"}, 
					{awardid=0, awardiconpath="武器类.dds", awarddesc="^ffcb4a乾系列武器\r^ffffff适合等级：飞升135级"}, 
				},				
				commendlevel=3, istasknpc=false, descnpc="", descmapid=10, descpos="7,183,-170", istransport=true, 
				npclv=150, refreshperiod="19:00", recommendlv="145", recommendplayernum="6", 
				desc="^ffffff      好斗与屠杀的战神，司职战争，形象英俊，性格强暴好斗，勇猛顽强，是权力与力量的象征，传说具有不死之身。"
			}

function MenologyMonster:GetSelf()
	return self;
end

SpecialTask = {}
SpecialTask['诛仙为爱成神版本上线'] = 
			{
				id = 401, backgroud1="icon\\lace_battle.tga", backgroud2="icon\\menology_aniversary.tga", 
				mendisplaylevel=112, weekly=false, validday="1,2,3,4,5,6,7", 
				periods=
				{
					{starttime="2010-11-23", endtime="2010-11-23"},
				}, 
				validtime="0:00-24:00", 
				desc="^ffffff诛仙为爱成神版本上线"
			}
SpecialTask['国庆活动'] = 
			{
				id = 402, backgroud1="icon\\lace_specday.tga", backgroud2="icon\\menology_nationalday.tga", 
				mendisplaylevel=111, weekly=false, validday="1,2,3,5,7", 
				periods=
				{
					{starttime="2010-10-01", endtime="2010-10-10"},
				}, 
				validtime="9:00-12:00", 
				desc="^ffffff国庆活动周"
			}
SpecialTask['圣诞节活动'] = 
			{
				id = 403, backgroud1="icon\\lace_commonactivity.tga", backgroud2="icon\\menology_snowman.tga", 
				mendisplaylevel=113, weekly=false, validday="1,2,3,5,7", 
				periods=
				{
					{starttime="2010-12-17", endtime="2010-12-25"},
				}, 
				validtime="9:00-12:00", 
				desc="^ffffff      白雪飘飘，深冬严严，圣诞节新增诸多精彩活动：\r^11ff11瑞雪佳年\r^11ff11把我变成雪人吧\r^11ff11天池玄冰\r^11ff11雪人的圣诞心愿\r^11ff11圣诞许愿\r^11ff11圣诞老人的礼物"
			}
SpecialTask['元旦活动'] = 
			{
				id = 404, backgroud1="icon\\lace_festival.tga", backgroud2="icon\\menology_chongyang.tga", 
				mendisplaylevel=114, weekly=false, validday="1,2,3,5,7", 
				periods=
				{
					{starttime="2011-1-1", endtime="2011-1-7"},
				}, 
				validtime="9:00-12:00", 
				desc="^ffffff      元旦活动周，精彩活动多多：\r^11ff11祈福钟声\r^11ff11幸运金蛋"
			}
SpecialTask['元宵活动'] = 
			{
				id = 405, backgroud1="icon\\lace_festival.tga", backgroud2="icon\\menology_nationalday.tga", 
				mendisplaylevel=115, weekly=false, validday="1,2,3,4,5,6,7", 
				periods=
				{
					{starttime="2011-2-15", endtime="2011-2-22"},
				}, 
				validtime="20:00-23:00", 
				desc="^ffffff      一年一度的闹元宵，猜灯谜活动又来啦！今年由宝石猫陪大家一起过元宵节。如果准备好了，就可以去那边翻开花灯猜灯谜了！"
			}

SpecialTask['白色情人节'] = 
			{
				id = 406, backgroud1="icon\\lace_battle.tga", backgroud2="icon\\menology_aniversary.tga", 
				mendisplaylevel=116, weekly=false, validday="1,2,3,4,5,6,7", 
				periods=
				{
					{starttime="2011-3-8", endtime="2011-3-21"},
				}, 
				validtime="0:00-24:00", 
				desc="^ffffff      一年一度的白色情人节又来临了，30级以上的玩家在这一期间可找^11ff11河阳月老^ffffff接取“非诚勿扰”任务，获得月老赠与的“鸳鸯兽”，“鸳鸯兽”会自动寻找与你有缘之人，找到有缘之人后，从“鸳鸯兽”吐落的“姻缘珠”中获得月老所需的物品，交给月老并获得丰厚奖励。"
			}
			
SpecialTask['五一劳动节'] = 
			{
				id = 407, backgroud1="icon\\lace_festival.tga", backgroud2="icon\\menology_nationalday.tga", 
				mendisplaylevel=117, weekly=false, validday="1,2,3,4,5,6,7", 
				periods=
				{
					{starttime="2011-4-26", endtime="2011-5-14"},
				}, 
				validtime="0:00-24:00", 
				desc="^ffffff      一年一度的劳动节又到了，30级以上的玩家可以到^11ff11司灵韵^ffffff那里领取劳动节活动任务，完成后将获得大量奖励，快去^11ff11河阳相思木^ffffff那里找他吧。（活动开放时间：4月26日12：02-5月14日23：55）"
			}

SpecialTask['诛仙周年庆'] = 
			{
				id = 408, backgroud1="icon\\lace_denglong.tga", backgroud2="icon\\menology_yuandan.tga", 
				mendisplaylevel=118, weekly=false, validday="1,2,3,4,5,6,7", 
				periods=
				{
					{starttime="2011-5-24", endtime="2011-5-24"},
				}, 
				validtime="0:00-24:00", 
				desc="^ffffff      佳节传喜庆，\r      今岁复相重，\r      欢歌颂盛世，\r      载舞祝平生。\r      这是全体诛仙人民的节日，让我们共同为这焕然一新的诛仙世界祈福！（^11ff11周年庆活动期间，在线玩家将会获得大量精彩纷呈好礼。^ffffff）"
			}
SpecialTask['诛仙·前传'] = 
			{
				id = 409, backgroud1="icon\\lace_festival.tga", backgroud2="icon\\menology_yuandan.tga", 
				mendisplaylevel=118, weekly=false, validday="1,2,3,4,5,6,7", 
				periods=
				{
					{starttime="2011-9-22", endtime="2011-10-11"},
				}, 
				validtime="0:00-24:00", 
				desc="^ffffff      佳节传喜庆，\r      今岁复相重，\r      欢歌颂盛世，\r      载舞祝平生。\r      诛仙新版《诛仙·前传》正式公测，让我们共同为这焕然一新的诛仙世界祈福！（^11ff11活动期间，在线玩家将会获得大量精彩纷呈好礼。^ffffff）"
			}
SpecialTask['国庆活动'] = 
			{
				id = 410, backgroud1="icon\\lace_festival.tga", backgroud2="icon\\menology_nationalday.tga", 
				mendisplaylevel=119, weekly=false, validday="1,2,3,5,7", 
				periods=
				{
					{starttime="2011-10-01", endtime="2011-10-11"},
				}, 
				validtime="9:00-12:00", 
				desc="^ffffff      国庆期间，所有在线玩家通过参与活动将会获得大量奖励。"
			}
SpecialTask['圣诞活动'] = 
			{
				id = 411, backgroud1="icon\\lace_commonactivity.tga", backgroud2="icon\\menology_snowman.tga", 
				mendisplaylevel=120, weekly=false, validday="1,2,3,5,7", 
				periods=
				{
					{starttime="2011-12-18", endtime="2011-12-25"},
				}, 
				validtime="0:00-24:00", 
				desc="^ffffff      白雪飘飘，深冬严严，圣诞节新增诸多精彩活动：\r^11ff11      祝福送给你\r^11ff11      顽皮的孩子"
			}
SpecialTask['元旦活动'] = 
			{
				id = 412, backgroud1="icon\\lace_festival.tga", backgroud2="icon\\menology_chongyang.tga", 
				mendisplaylevel=121, weekly=false, validday="1,2,3,5,7", 
				periods=
				{
					{starttime="2012-1-1", endtime="2012-1-7"},
				}, 
				validtime="0:00-24:00", 
				desc="^ffffff      元旦期间，在新年使者处选择“^11ff11参加元旦活动^ffffff”，进入元旦活动场地，完成元旦任务便可获得奖励。"
			}

function SpecialTask:GetSelf()
	return self;
end