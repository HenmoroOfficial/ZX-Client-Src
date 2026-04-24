---------Data   for TGllk game-----------------------

--//连连看方阵的规模，共包含了三个难度，height为高，width为宽。例如：9*14 实际的块排列规模为7*12，其他两行两列为留白用作连接判断，其余类同

TGllk_MapSize = { { height = 9, width = 14 },

				  { height = 10, width = 16 },
				  
				  { height = 11, width = 18 },
				};
				
--//连连看的每个物块大小，height为高度像素，width为宽度像素			
TGllk_BlockSize = { height=36, width=36 };

--//连连看的三个难度对应的 方阵原点 。 x为原点在游戏界面x轴的位置，y为原点在游戏界面y轴的位置。
TGllk_MapOrigin = {
					{ x=90, y=200 },
					{ x=60, y=185 },
					{ x=25, y=170 },
				};
				
--//连连看中可以供选择的背景图的个数（在游戏设置中使用了加黑色过渡边框的原画）				
TGllk_BkNum = 5;

--//连连看的难度数总数
TGllk_DiffStepNum = 3;

--//连连看中物块的重复个数，也就是height*width的物块数中，每TGllk_NumPerPic个物块是相同的。调整该数据时，需要同步调整TGllk_Pic
TGllk_NumPerPic = 4;

--//连连看游戏中能容忍的最大的点击连续错误数目
TGllk_ContinueFalse = 3; 

--//连连看中三个难度开局时赋予的提示数目
TGllk_HintNum = { 4, 4, 4,};
--//连连看中三个难度开局时赋予的洗牌数目
TGllk_WashNum = { 2, 4, 4 };

--//连连看中三个难度初始化给与的时间
TGllk_Time = {200000,160000,160000};

--//连连看中消除一次所返回的时间
TGllk_TimeReward = {2000,1500,1500};	

--//连连看中发生连续错误数大于TGllk_ContinueFalse扣除的时间
TGllk_TimeFalse = 5000;

--//对应连连看中三个难度，消除连续达到阈值后给与奖励。该阈值为3000，即3秒。
--//-1为最小值，当消除时，加上消除的奖励时间，当一个tick过去时，减去一个tick的时间，当该项累计时间到达TGllk_RewardThresholdTime时，则进行奖励随机计算,该数值设置不得小于一次成功消除的奖励时间TGllk_TimeReward
TGllk_RewardThresholdTime = {4000,3300,3300};

--//当累积的奖励时间达到了TGllk_RewardThresholdTime后进行获奖判断，判断后累积的奖励时间减去TGllk_RewardSucTimeReduce的值为剩余的积累时间（三个数据项对应三个难度）
TGllk_RewardSucTimeReduce = {1500,900,1000 };

--//连连看三个难度每次点击消除以后的得分增加
TGllk_ScoreReward = {10,15,20}; 

--//连连看三个难度 过关后每个提示转化的分数
TGllk_ScoreHint = {80,100,150};	

--//连连看三个难度 过关后每个洗牌转化的分数
TGllk_ScoreWash = {100,200,300};

--//连连看三个难度 每个小局结束后，多于的时间转化为分数，每1000毫秒（1秒）转化的分数
TGllk_ScoreTime = {5,10,20};	-- 剩余时间每1000毫秒的分数

--//连连看 选中的效果图
TGllk_PressedPic = "TinyGame\\TGllk\\Icon\\Chosen1.tga";

--//连连看 选中的效果图帧数
TGllk_PressedPicNum = 11;

--//连连看 提示的效果图
TGllk_HintPic = "TinyGame\\TGllk\\Icon\\Chosen1.tga";

--//连连看 提示图的效果图帧数
TGllk_HintPicNum= 11;

--//连连看 连接线的图
TGllk_LinePic = "TinyGame\\TGllk\\LinePixel.tga";

--//连连看 消失效果的图
TGllk_DisapearPic = "TinyGame\\TGllk\\Icon\\Disapear1.tga";

--//连连看 消失效果的图
TGllk_DisapearPicNum = 11;

--//连连看  半个物块的像素尺寸
TGllk_BlockHSize = 18

--//连连看  半个提示和消失块的尺寸
TGllk_HintSize = 25;

--//连连看 物块的图片
TGllk_Pic = {	
--1
				"TinyGame\\TGllk\\Icon\\1.tga",
				"TinyGame\\TGllk\\Icon\\5.tga",
				"TinyGame\\TGllk\\Icon\\9.tga",
				"TinyGame\\TGllk\\Icon\\13.tga",
				"TinyGame\\TGllk\\Icon\\17.tga",
				"TinyGame\\TGllk\\Icon\\21.tga",
				"TinyGame\\TGllk\\Icon\\25.tga",				
				"TinyGame\\TGllk\\Icon\\29.tga",
				"TinyGame\\TGllk\\Icon\\33.tga",
				"TinyGame\\TGllk\\Icon\\2.tga",
				"TinyGame\\TGllk\\Icon\\6.tga",
				"TinyGame\\TGllk\\Icon\\10.tga",
				"TinyGame\\TGllk\\Icon\\14.tga",
				"TinyGame\\TGllk\\Icon\\18.tga",
				"TinyGame\\TGllk\\Icon\\22.tga",
				"TinyGame\\TGllk\\Icon\\26.tga",
				"TinyGame\\TGllk\\Icon\\30.tga",	
				"TinyGame\\TGllk\\Icon\\34.tga",
				"TinyGame\\TGllk\\Icon\\3.tga",
				"TinyGame\\TGllk\\Icon\\7.tga",
				"TinyGame\\TGllk\\Icon\\11.tga",
--21
				"TinyGame\\TGllk\\Icon\\15.tga",
				"TinyGame\\TGllk\\Icon\\19.tga",
				"TinyGame\\TGllk\\Icon\\23.tga",
				"TinyGame\\TGllk\\Icon\\27.tga",				
				"TinyGame\\TGllk\\Icon\\31.tga",
				"TinyGame\\TGllk\\Icon\\35.tga",
				"TinyGame\\TGllk\\Icon\\4.tga",
--28
				"TinyGame\\TGllk\\Icon\\8.tga",
				"TinyGame\\TGllk\\Icon\\12.tga",
				"TinyGame\\TGllk\\Icon\\16.tga",
				"TinyGame\\TGllk\\Icon\\20.tga",
				"TinyGame\\TGllk\\Icon\\24.tga",
				"TinyGame\\TGllk\\Icon\\28.tga",
				"TinyGame\\TGllk\\Icon\\32.tga",
				"TinyGame\\TGllk\\Icon\\36.tga",
--36

				
			};
			
--连连看声效文件
TGllk_Sound = {
					ConnectSucc = "SFX\\TinyGame\\TGllk\\成功.wav",
					ConnectFail = "SFX\\TinyGame\\TGllk\\失败.wav",
					SuccReward = "SFX\\TinyGame\\TGllk\\连续成功的奖励.wav",
					MissionSuc = "SFX\\TinyGame\\TGllk\\任务成功.wav",
					MissionFai = "SFX\\TinyGame\\TGllk\\任务失败.wav",
					Wash = "SFX\\TinyGame\\TGllk\\洗牌.wav",
					Hint = "SFX\\TinyGame\\TGllk\\成功.wav"
			};			

--设定present

--//三个等级的奖励，每个等级里面有n种同等级的奖励。{a,b,c,d,e}参数分别为：a-奖励的生命；b-奖励的hint；c-奖励的分数；d-奖励时间；e-奖励的提示
TGllk_Reward = {		
				[1] = { {2,0,0,0,"连续消除，奖励洗牌2次!" },{0,4,0,0,"连续消除，奖励提示4次!"},{0,0,500,0,"奖励分数500"},{0,0,0,12000,"连续消除，奖励时间12秒!"}, },
				[2] = { {1,0,0,0,"连续消除，奖励洗牌1次!"},{0,3,0,0,"连续消除，奖励提示3次!"},{0,0,400,0,"奖励分数400"} ,{0,0,0,10000,"连续消除，奖励时间10秒!"},},
				[3] = { {0,2,0,0,"连续消除，奖励提示2次!"},{0,0,300,0,"奖励分数300"}, {0,0,0,8000,"连续消除，奖励时间8秒!"},},
				[4] = { {0,0,200,0,"奖励分数200"}, {0,0,0,6000,"连续消除，奖励时间6秒!"},},
			};
			
--//三个难度的奖励的时间范围 	其中每个难度登记包含了若干个可以得奖的时间段（已经花费时间占总时间的比例来计算），其中每个时间段对应的得奖概率不同
TGllk_RewardArea = { 	{ {1,0.95},{0.95,0.90},{0.90,0.75},},	
						{ {1,0.95},{0.95,0.80},{0.80,0.50},},
						{ {1,0.95},{0.95,0.75},{0.75,0.20},},
 					};
--//三个难度的对应时间段的获得奖励的几率，与TGllk_RewardArea相对应
TGllk_RewardTimeRate = { {0.4,0.4,0.4},	--第一个难度在三个时间范围内的得奖率
					 	 {0.3,0.4,0.3},	--第二个难度在三个时间范围内的得奖率
					     {0.3,0.4,0.3},	--第三个难度在三个时间范围内的得奖率
					 };

--//一但确定能获得奖励后，三个时间段对应的三种奖励的分布情况，如：{0.2,0.5,1}，0~0.2为第一等奖励，0.2~0.5为第二等奖励，0.5~1.0为第三等奖励
TGllk_RewardRate = { {0.1,0.3,0.8,1},{0.1,0.25,0.7,1},{0.1,0.25,0.7,1}, }; --三种奖励的分布

--//三个难度过关后提交的任务线
TGllk_TaskNum = { 14548, 14549, 14550 };


--Text-------------------------------------

--//补充说明：界面按钮上的文字已经在

TGllk_TextWait = "开始"
TGllk_TextInGame = "暂停"
TGllk_TextPause = "继续"

TGllk_TextBlock = "连接失败！"
TGllk_TextContiFalse = "连续错误，扣除时间5秒"  --时间与TGllk_TimeFalse的值相统一

			
--constant---------------------------------一些常量（不要修改）
TGllk_EASY = 1;
TGllk_Normal = 2;
TGllk_HARD = 3;

TGllk_Wait = 2;
TGllk_InGame = 1;
TGllk_Pause = 0;

			

			

			

			

			

			

			
--








































			