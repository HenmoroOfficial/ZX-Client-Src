----------------------------------------------------	about game ----------------------------------------------------
    TD_PassNum = 4;	--游戏的总关数    
    TD_AllowDeadNum = 15;	--每一关走过TD_AllowDeadNum个怪物就死亡

    
    TD_BossHurt = 3;
    TD_NormalHurt = 1;
    
    
    
    --说明：

    -- boss：按概率数值产生boss，bossnumrandom：随机产生的boss数，当nil时，表示读取写死的boss数，bossnum为写死的boss数目，

    -- randomtype为是否要随机抽取怪物种类，bosstype写死的boss种类，variance是一波每个怪产生变种的几率，monstertype为写死的普通怪种类

    -- monsternum为写死的一群怪的数目，waitperm为写死的每一波怪中怪与怪之间的间隔
    								--每关游戏内 每拨怪物出来的时间间隔

    TD_WaitPerLevel={ 
    					[1]=30000,
    					[2]=30000,
    					[3]=30000,
    					[4]=30000,
			
    				 };
    
    
	TD_PerPassData={
	
	[1]={
			[1]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=1, monsternum=6, waitperm= 500},

				},
				
			[2]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=2, monsternum=6, waitperm= 800},

				},
				
			[3]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=3, monsternum=6, waitperm= 800}, 

				},
				
			[4]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=4, monsternum=6, waitperm= 800},

				},
				
			[5]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=5, monsternum=6, waitperm= 400}, 
				},
				
			[6]={ [1]={flyrate=1,flytype=51,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=6, monsternum=6, waitperm= 800},
				},
				
			[7]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=7, monsternum=6, waitperm= 1200},
				},
				
			[8]={ [1]={flyrate=0,flytype=1,bossflytype=1,bossrate=1, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=8, monsternum=6, waitperm= 800},
				},
				
			[9]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=9, monsternum=6, waitperm= 800},
				},
				
			[10]={ [1]={flyrate=0.5,flytype=57,bossflytype=10, bossrate=0, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=1, variance=0, monstertype=10, monsternum=6, waitperm= 400},
				},
				
			[11]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=11, monsternum=6, waitperm= 800},
				},
				
			[12]={ [1]={flyrate=1,flytype=52,bossflytype=10, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=12, monsternum=6, waitperm= 1200},
				},
				
			[13]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=13, monsternum=6, waitperm= 800},
				},
				
			[14]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=14, monsternum=6, waitperm= 1800},
				},
				
			[15]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=15, monsternum=6, waitperm= 400},
				},
				
			[16]={ [1]={flyrate=0.4,flytype=1,bossflytype=10, bossrate=1, bossnrandom=3, bossnum=4, randomtype=false, bosstype=2, variance=0, monstertype=16, monsternum=6, waitperm= 800},
				},
				
			[17]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=17, monsternum=6, waitperm= 800},
				},
				
			[18]={ [1]={flyrate=1,flytype=53,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=18, monsternum=6, waitperm= 1800},
				},
				
			[19]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=19, monsternum=6, waitperm= 800},
				},
			
			[20]={ [1]={flyrate=0.5,flytype=58,bossflytype=10, bossrate=0.5, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=6, variance=0, monstertype=20, monsternum=6, waitperm= 400},
			},			
		},
	
	
	[2]={
			[1]={   [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=1, monsternum=10, waitperm= 800},
				},
				
				
			[2]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=2, monsternum=10, waitperm= 800},

				},
				
			[3]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=3, monsternum=10, waitperm= 800}, 

				},
				
			[4]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=4, monsternum=10, waitperm= 1200},

				},
				
			[5]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0.5, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=5, monsternum=10, waitperm= 400}, 
				},
				
			[6]={ [1]={flyrate=1,flytype=51,bossflytype=10, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=6, monsternum=10, waitperm= 800},
				},
				
			[7]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=7, monsternum=10, waitperm= 800},
				},
				
			[8]={ [1]={flyrate=0,flytype=1,bossflytype=1,bossrate=1, bossnrandom=nil, bossnum=2, randomtype=false, bosstype=2, variance=0, monstertype=8, monsternum=10, waitperm= 1200},
				},
				
			[9]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=9, monsternum=10, waitperm= 800},
				},
				
			[10]={ [1]={flyrate=0.5,flytype=57,bossflytype=10, bossrate=0.5, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=6, variance=0, monstertype=10, monsternum=10, waitperm= 400},
				},
				
			[11]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=11, monsternum=10, waitperm= 800},
				},
				
			[12]={ [1]={flyrate=1,flytype=52,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=12, monsternum=10, waitperm= 800},
				},
				
			[13]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=13, monsternum=10, waitperm= 1200},
				},
				
			[14]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=14, monsternum=10, waitperm= 800},
				},
				
			[15]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=15, monsternum=10, waitperm= 400},
				},
				
			[16]={ [1]={flyrate=0.4,flytype=1,bossflytype=11, bossrate=1, bossnrandom=3, bossnum=4, randomtype=true, bosstype=2, variance=0, monstertype=16, monsternum=10, waitperm= 800},
				},
				
			[17]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=17, monsternum=10, waitperm= 1600},
				},
				
			[18]={ [1]={flyrate=1,flytype=53,bossflytype=10, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=18, monsternum=10, waitperm= 800},
				},
				
			[19]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=19, monsternum=10, waitperm= 1800},
				},
				
			[20]={ [1]={flyrate=0.5,flytype=58,bossflytype=11, bossrate=0.5, bossnrandom=nil, bossnum=3, randomtype=true, bosstype=7, variance=0, monstertype=20, monsternum=10, waitperm= 400},
				},
				
			[21]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=21, monsternum=10, waitperm= 1800},
				},
				
			[22]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=22, monsternum=10, waitperm= 800},
				},
				
			[23]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=23, monsternum=10, waitperm= 800},
				},
				
			[24]={ [1]={flyrate=0.6,flytype=54,bossflytype=11, bossrate=1, bossnrandom=3, bossnum=3, randomtype=false, bosstype=3, variance=0, monstertype=24, monsternum=10, waitperm= 1600},
				},
				
			[25]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=25, monsternum=10, waitperm= 400},
				},
				
			[26]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=26, monsternum=10, waitperm= 1200},
				},
				
			[27]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=27, monsternum=10, waitperm= 800},
				},
				
			[28]={ [1]={flyrate=0.5,flytype=59,bossflytype=12, bossrate=0.3, bossnrandom=nil, bossnum=4, randomtype=false, bosstype=8, variance=0, monstertype=28, monsternum=10, waitperm= 800},
				},
				
			[29]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=29, monsternum=10, waitperm= 800},
				},
				
			[30]={ [1]={flyrate=1,flytype=55,bossflytype=12, bossrate=0.6, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=34, monsternum=10, waitperm= 400},
				},	
		},
		
	[3]={
			[1]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=1, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=1, monsternum=10, waitperm= 800},	
				},
				
				
			[2]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=2, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=2, monsternum=10, waitperm= 800},
				},
				
			[3]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=3, monsternum=10, waitperm= 1400}, 
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=3, monsternum=10, waitperm= 1400},
				},
				
			[4]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=4, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=4, monsternum=10, waitperm= 800},
				},
				
			[5]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=5, monsternum=10, waitperm= 400}, 
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=5, monsternum=10, waitperm= 400},
				},
				
			[6]={ [1]={flyrate=1,flytype=51,bossflytype=11, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=6, monsternum=10, waitperm= 800},
						[2]={flyrate=1,flytype=51,bossflytype=11, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=6, monsternum=10, waitperm= 800},
				},
				
			[7]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=7, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=7, monsternum=10, waitperm= 800},
				},
				
			[8]={ [1]={flyrate=0,flytype=1,bossflytype=1,bossrate=1, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=8, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1,bossrate=1, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=8, monsternum=10, waitperm= 800},
				},
				
			[9]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=9, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=9, monsternum=10, waitperm= 800},
				},
				
			[10]={[1]={flyrate=0.5,flytype=57,bossflytype=10, bossrate=0.5, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=6, variance=0, monstertype=10, monsternum=10, waitperm= 400},
						[2]={flyrate=0.5,flytype=57,bossflytype=10, bossrate=0.5, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=6, variance=0, monstertype=10, monsternum=10, waitperm= 400},
				},
				
			[11]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=11, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=11, monsternum=10, waitperm= 800},
				},
				
			[12]={[1]={flyrate=1,flytype=55,bossflytype=12, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=12, monsternum=10, waitperm= 800},
						[2]={flyrate=1,flytype=55,bossflytype=12, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=12, monsternum=10, waitperm= 800},
				},
				
			[13]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=13, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=13, monsternum=10, waitperm= 800},
				},
				
			[14]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=14, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=14, monsternum=10, waitperm= 800},
				},
				
			[15]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=15, monsternum=10, waitperm= 400},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=15, monsternum=10, waitperm= 400},
				},
				
			[16]={[1]={flyrate=0.4,flytype=53,bossflytype=10, bossrate=1, bossnrandom=3, bossnum=4, randomtype=false, bosstype=2, variance=0, monstertype=16, monsternum=10, waitperm= 800},
						[2]={flyrate=0.4,flytype=53,bossflytype=10, bossrate=1, bossnrandom=3, bossnum=4, randomtype=false, bosstype=2, variance=0, monstertype=16, monsternum=10, waitperm= 800},
				},
				
			[17]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=17, monsternum=10, waitperm= 300},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=17, monsternum=10, waitperm= 300},
				},
				
			[18]={[1]={flyrate=1,flytype=53,bossflytype=11, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=18, monsternum=10, waitperm= 800},
						[2]={flyrate=1,flytype=53,bossflytype=11, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=18, monsternum=10, waitperm= 800},
				},
				
			[19]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=19, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=19, monsternum=10, waitperm= 800},
				},
				
			[20]={[1]={flyrate=0.5,flytype=58,bossflytype=12, bossrate=0.5, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=7, variance=0, monstertype=20, monsternum=10, waitperm= 400},
						[2]={flyrate=0.5,flytype=58,bossflytype=12, bossrate=0.5, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=7, variance=0, monstertype=20, monsternum=10, waitperm= 400},
				},
				
			[21]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=21, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=21, monsternum=10, waitperm= 800},
				},
				
			[22]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=22, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=22, monsternum=10, waitperm= 800},
				},
				
			[23]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=23, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=23, monsternum=10, waitperm= 800},
				},
				
			[24]={[1]={flyrate=0.6,flytype=54,bossflytype=11, bossrate=1, bossnrandom=3, bossnum=3, randomtype=false, bosstype=3, variance=0, monstertype=24, monsternum=10, waitperm= 800},
						[2]={flyrate=0.6,flytype=54,bossflytype=11, bossrate=1, bossnrandom=3, bossnum=3, randomtype=false, bosstype=3, variance=0, monstertype=24, monsternum=10, waitperm= 800},
				},
				
			[25]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=25, monsternum=10, waitperm= 400},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=25, monsternum=10, waitperm= 400},
				},
				
			[26]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=26, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=26, monsternum=10, waitperm= 800},
				},
				
			[27]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=27, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=27, monsternum=10, waitperm= 800},
				},
				
			[28]={[1]={flyrate=0.5,flytype=59,bossflytype=12, bossrate=0.3, bossnrandom=nil, bossnum=4, randomtype=false, bosstype=8, variance=0, monstertype=28, monsternum=10, waitperm= 800},
						[2]={flyrate=0.5,flytype=59,bossflytype=12, bossrate=0.3, bossnrandom=nil, bossnum=4, randomtype=false, bosstype=8, variance=0, monstertype=28, monsternum=10, waitperm= 800},
				},
				
			[29]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=29, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=29, monsternum=10, waitperm= 800},
				},
				
			[30]={[1]={flyrate=1,flytype=55,bossflytype=11, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=30, monsternum=10, waitperm= 400},
						[2]={flyrate=1,flytype=55,bossflytype=11, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=30, monsternum=10, waitperm= 400},
				},
				
			[31]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=31, monsternum=10, waitperm= 700},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=31, monsternum=10, waitperm= 700},
				},
				
			[32]={[1]={flyrate=0.3,flytype=1,bossflytype=12, bossrate=1, bossnrandom=3, bossnum=2, randomtype=false, bosstype=4, variance=0, monstertype=32, monsternum=10, waitperm= 700},
						[2]={flyrate=0.3,flytype=1,bossflytype=12, bossrate=1, bossnrandom=3, bossnum=2, randomtype=false, bosstype=4, variance=0, monstertype=32, monsternum=10, waitperm= 700},
				},
				
			[33]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=33, monsternum=10, waitperm= 700},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=33, monsternum=10, waitperm= 700},
				},
				
			[34]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=34, monsternum=10, waitperm= 700},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=34, monsternum=10, waitperm= 700},
				},
				
			[35]={[1]={flyrate=0.3,flytype=60,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=1, variance=0, monstertype=35, monsternum=10, waitperm= 300},
						[2]={flyrate=0.3,flytype=60,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=1, variance=0, monstertype=35, monsternum=10, waitperm= 300},
				},
				
			[36]={[1]={flyrate=1,flytype=56,bossflytype=11, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=36, monsternum=10, waitperm= 700},
						[2]={flyrate=1,flytype=56,bossflytype=11, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=36, monsternum=10, waitperm= 700},
				},
				
			[37]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=37, monsternum=10, waitperm= 700},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=37, monsternum=10, waitperm= 700},
				},
				
			[38]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=38, monsternum=10, waitperm= 700},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=38, monsternum=10, waitperm= 700},
				},
				
			[39]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=39, monsternum=10, waitperm= 700},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=39, monsternum=10, waitperm= 700},
				},
				
			[40]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=1, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=40, monsternum=10, waitperm= 300},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=1, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=40, monsternum=10, waitperm= 300},
				},			
		},
		
[4]={
			[1]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=1, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=1, monsternum=10, waitperm= 800},	
				},
				
				
			[2]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=2, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=2, monsternum=10, waitperm= 800},
				},
				
			[3]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=3, monsternum=10, waitperm= 800}, 
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=3, monsternum=10, waitperm= 800},
				},
				
			[4]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=4, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=4, monsternum=10, waitperm= 800},
				},
				
			[5]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=5, monsternum=10, waitperm= 400}, 
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=5, monsternum=10, waitperm= 400},
				},
				
			[6]={ [1]={flyrate=1,flytype=51,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=6, monsternum=10, waitperm= 800},
						[2]={flyrate=1,flytype=51,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=6, monsternum=10, waitperm= 800},
				},
				
			[7]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=7, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=7, monsternum=10, waitperm= 800},
				},
				
			[8]={ [1]={flyrate=0,flytype=1,bossflytype=1,bossrate=1, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=8, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1,bossrate=1, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=8, monsternum=10, waitperm= 800},
				},
				
			[9]={ [1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=9, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=1, variance=0, monstertype=9, monsternum=10, waitperm= 800},
				},
				
			[10]={[1]={flyrate=0.5,flytype=57,bossflytype=10, bossrate=0.5, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=6, variance=0, monstertype=10, monsternum=10, waitperm= 400},
						[2]={flyrate=0.5,flytype=57,bossflytype=10, bossrate=0.5, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=6, variance=0, monstertype=10, monsternum=10, waitperm= 400},
				},
				
			[11]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=11, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=11, monsternum=10, waitperm= 800},
				},
				
			[12]={[1]={flyrate=1,flytype=52,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=12, monsternum=10, waitperm= 800},
						[2]={flyrate=1,flytype=52,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=12, monsternum=10, waitperm= 800},
				},
				
			[13]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=13, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=13, monsternum=10, waitperm= 800},
				},
				
			[14]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=14, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=14, monsternum=10, waitperm= 800},
				},
				
			[15]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=15, monsternum=10, waitperm= 400},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=15, monsternum=10, waitperm= 400},
				},
				
			[16]={[1]={flyrate=0.4,flytype=1,bossflytype=10, bossrate=1, bossnrandom=3, bossnum=4, randomtype=false, bosstype=2, variance=0, monstertype=16, monsternum=10, waitperm= 800},
						[2]={flyrate=0.4,flytype=1,bossflytype=10, bossrate=1, bossnrandom=3, bossnum=4, randomtype=false, bosstype=2, variance=0, monstertype=16, monsternum=10, waitperm= 800},
				},
				
			[17]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=17, monsternum=10, waitperm= 300},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=17, monsternum=10, waitperm= 300},
				},
				
			[18]={[1]={flyrate=1,flytype=53,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=18, monsternum=10, waitperm= 800},
						[2]={flyrate=1,flytype=53,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=18, monsternum=10, waitperm= 800},
				},
				
			[19]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=19, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=19, monsternum=10, waitperm= 800},
				},
				
			[20]={[1]={flyrate=0.5,flytype=58,bossflytype=1, bossrate=0.5, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=7, variance=0, monstertype=20, monsternum=10, waitperm= 400},
						[2]={flyrate=0.5,flytype=58,bossflytype=1, bossrate=0.5, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=7, variance=0, monstertype=20, monsternum=10, waitperm= 400},
				},
				
			[21]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=21, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=21, monsternum=10, waitperm= 800},
				},
				
			[22]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=22, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=22, monsternum=10, waitperm= 800},
				},
				
			[23]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=23, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=23, monsternum=10, waitperm= 800},
				},
				
			[24]={[1]={flyrate=0.6,flytype=54,bossflytype=11, bossrate=1, bossnrandom=3, bossnum=3, randomtype=false, bosstype=3, variance=0, monstertype=24, monsternum=10, waitperm= 800},
						[2]={flyrate=0.6,flytype=54,bossflytype=11, bossrate=1, bossnrandom=3, bossnum=3, randomtype=false, bosstype=3, variance=0, monstertype=24, monsternum=10, waitperm= 800},
				},
				
			[25]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=25, monsternum=10, waitperm= 400},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=25, monsternum=10, waitperm= 400},
				},
				
			[26]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=26, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=26, monsternum=10, waitperm= 800},
				},
				
			[27]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=27, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=27, monsternum=10, waitperm= 800},
				},
				
			[28]={[1]={flyrate=0.5,flytype=59,bossflytype=1, bossrate=0.3, bossnrandom=nil, bossnum=4, randomtype=false, bosstype=8, variance=0, monstertype=28, monsternum=10, waitperm= 800},
						[2]={flyrate=0.5,flytype=59,bossflytype=1, bossrate=0.3, bossnrandom=nil, bossnum=4, randomtype=false, bosstype=8, variance=0, monstertype=28, monsternum=10, waitperm= 800},
				},
				
			[29]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=29, monsternum=10, waitperm= 800},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=29, monsternum=10, waitperm= 800},
				},
				
			[30]={[1]={flyrate=1,flytype=55,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=30, monsternum=10, waitperm= 400},
						[2]={flyrate=1,flytype=55,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=30, monsternum=10, waitperm= 400},
				},
				
			[31]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=31, monsternum=10, waitperm= 700},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=31, monsternum=10, waitperm= 700},
				},
				
			[32]={[1]={flyrate=0.3,flytype=1,bossflytype=12, bossrate=1, bossnrandom=3, bossnum=2, randomtype=false, bosstype=4, variance=0, monstertype=32, monsternum=10, waitperm= 700},
						[2]={flyrate=0.3,flytype=1,bossflytype=12, bossrate=1, bossnrandom=3, bossnum=2, randomtype=false, bosstype=4, variance=0, monstertype=32, monsternum=10, waitperm= 700},
				},
				
			[33]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=33, monsternum=10, waitperm= 700},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=33, monsternum=10, waitperm= 700},
				},
				
			[34]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=34, monsternum=10, waitperm= 700},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=34, monsternum=10, waitperm= 700},
				},
				
			[35]={[1]={flyrate=0.3,flytype=60,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=1, variance=0, monstertype=35, monsternum=10, waitperm= 300},
						[2]={flyrate=0.3,flytype=60,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=3, randomtype=false, bosstype=1, variance=0, monstertype=35, monsternum=10, waitperm= 300},
				},
				
			[36]={[1]={flyrate=1,flytype=56,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=36, monsternum=10, waitperm= 700},
						[2]={flyrate=1,flytype=56,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=36, monsternum=10, waitperm= 700},
				},
				
			[37]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=37, monsternum=10, waitperm= 700},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=37, monsternum=10, waitperm= 700},
				},
				
			[38]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=38, monsternum=10, waitperm= 700},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=38, monsternum=10, waitperm= 700},
				},
				
			[39]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=39, monsternum=10, waitperm= 700},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=1, variance=0, monstertype=39, monsternum=10, waitperm= 700},
				},
				
			[40]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=1, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=40, monsternum=10, waitperm= 300},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=1, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=40, monsternum=10, waitperm= 300},
				},
			
			[41]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=5, variance=0, monstertype=41, monsternum=10, waitperm= 300},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=41, monsternum=10, waitperm= 300},
				}, 
				
			[42]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=42, monsternum=10, waitperm= 300},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=42, monsternum=10, waitperm= 300},
				}, 
				
			[43]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=43, monsternum=10, waitperm= 300},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=43, monsternum=10, waitperm= 300},
				}, 
				
			[44]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=44, monsternum=10, waitperm= 300},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=44, monsternum=10, waitperm= 300},
				}, 
				
			[45]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0.5, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=45, monsternum=10, waitperm= 300},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0.5, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=45, monsternum=10, waitperm= 300},
				}, 
				
			[46]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=5, variance=0, monstertype=46, monsternum=10, waitperm= 300},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=5, variance=0, monstertype=46, monsternum=10, waitperm= 300},
				}, 
				
			[47]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=5, variance=0, monstertype=40, monsternum=10, waitperm= 300},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=5, variance=0, monstertype=40, monsternum=10, waitperm= 300},
				}, 
				
			[48]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=5, variance=0, monstertype=40, monsternum=10, waitperm= 300},
						[2]={flyrate=0,flytype=1,bossflytype=1, bossrate=0, bossnrandom=nil, bossnum=1, randomtype=true, bosstype=5, variance=0, monstertype=40, monsternum=10, waitperm= 300},
				}, 
				
			[49]={[1]={flyrate=1,flytype=1,bossflytype=13, bossrate=1, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=40, monsternum=10, waitperm= 300},
						[2]={flyrate=1,flytype=1,bossflytype=13, bossrate=1, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=5, variance=0, monstertype=40, monsternum=10, waitperm= 300},
				}, 
				
			[50]={[1]={flyrate=0,flytype=1,bossflytype=1, bossrate=1, bossnrandom=nil, bossnum=1, randomtype=false, bosstype=9, variance=0, monstertype=40, monsternum=10, waitperm= 300},
						
				},			
		},


	
	
	
	};


    

-------------------------------------------------	about gamer ----------------------------------------------------


    TD_Gold = {400,200,200,200,};		--初始的金钱值_按照关数设置
    
    TD_DiffCoe = { {0.8,0.8},{1,1},{1.2,1.5},};    --难度的系数  {怪物生命加权系数，怪物回报金钱的难度系数}

	TD_TaskIndex = { 	[1]={nil, 14196, 14196},	--简单的三关任务线，如果不返回任务，写nil
						[2]={nil, 14196, 14197},	--中等的三关任务线
						[3]={14196, 14197, 14198},	--困难的三关任务线
						[4]={14196, 14197, 14198},
					 };	--每一关的 起始和终止 任务号

	
-------------------------------------------------	weapon index -----------------------------------------------------
	TD_WeaponName = { "仙石", "鬼王宗", "青云门", "合欢派", "天音寺", "鬼道", };
	--武器的属性列表



	TD_NoFireTime=5000;
	
			--需要添加图片，这个是怎么放置的

	TD_FreezeTime=3000;	--所有的减速弹作用时间相同
						--攻击间隔的缩短为原来的百分比，不要写直接减少的数值

						--要求在数据设置时，辅助塔的辅助范围只能递增，辅助效果只能递增
						--暴击的数值为增加的倍数，若填写n，可以n+1倍的攻击，multihit为几率，0为概率为0,增强时multiadd和multidamageadd都只能增加

						--子弹编号为0，表示没有子弹显示，此时可以对应快速的攻击
						
	TD_WeaponIndex = {
						[1] = {

								{ 	createcost=200,sell=150,cooltime=TD_NoFireTime,wradius=nil,bullettype=nil,speed=nil,bradius=nil,adamage=0,bdamage=0,damagetoa=0,damagetob=0,groundflag=false,airflag=false,freeze=0,improvecost=nil,improvetime=nil,improvepic=nil,
									multihit=0,multidamage=0,multibullet=nil,hitadd=0.2,toabadd=0.1,tofreezeadd=0.1,multiadd=0.1,multidamageadd=1,speedadd=nil,addradius=80,
									picname="TOWERDEFENSE\\weaponpic\\仙石.tga",frames=3,direnum=1,franum=3,acfranum=0,timeperfra=200,attperfra=100,rangecircle=3,attackeffect=nil
								}
							},
							
						[2] = {
								{ 	createcost=10,sell=7,cooltime=1000,wradius=3,bullettype=1,speed=0.150,bradius=nil,adamage=0,bdamage=5,damagetoa=0,damagetob=0,groundflag=true,airflag=false,freeze=0,improvecost=20,improvetime=2000,improvepic=1,
									multihit=0.1,multidamage=1,multibullet=1,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\鬼王宗1级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=2,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=22,cooltime=1000,wradius=3,bullettype=1,speed=0.150,bradius=nil,adamage=0,bdamage=11,damagetoa=0,damagetob=0,groundflag=true,airflag=true,freeze=0,improvecost=20,improvetime=5000,improvepic=1,
									multihit=0.1,multidamage=1,multibullet=1,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\鬼王宗2级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=2,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=37,cooltime=1000,wradius=4,bullettype=1,speed=0.150,bradius=nil,adamage=0,bdamage=20,damagetoa=0,damagetob=0,groundflag=true,airflag=true,freeze=0,improvecost=40,improvetime=8000,improvepic=1,
									multihit=0.2,multidamage=1,multibullet=1,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\鬼王宗3级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=3,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=67,cooltime=1000,wradius=4,bullettype=1,speed=0.150,bradius=nil,adamage=0,bdamage=38,damagetoa=0,damagetob=0,groundflag=true,airflag=true,freeze=0,improvecost=210,improvetime=11000,improvepic=1,
									multihit=0.2,multidamage=1,multibullet=1,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\鬼王宗4级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=3,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=225,cooltime=1000,wradius=4,bullettype=1,speed=0.150,bradius=nil,adamage=0,bdamage=140,damagetoa=0,damagetob=0,groundflag=true,airflag=true,freeze=0,improvecost=nil,improvetime=15000,improvepic=1,
									multihit=0.3,multidamage=1,multibullet=1,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\鬼王宗5级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=4,attackeffect=nil
								},
							},
							
						[3] = {

								{ 	createcost=15,sell=11,cooltime=3000,wradius=4,bullettype=2,speed=0.100,bradius=28,adamage=6,bdamage=2,damagetoa=0,damagetob=0,groundflag=true,airflag=false,freeze=0,improvecost=20,improvetime=2000,improvepic=1,
									multihit=0.1,multidamage=1,multibullet=2,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\青云1级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=3,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=26,cooltime=3000,wradius=4,bullettype=2,speed=0.100,bradius=32,adamage=12,bdamage=4,damagetoa=0,damagetob=0,groundflag=true,airflag=false,freeze=0,improvecost=35,improvetime=5000,improvepic=1,
									multihit=0.1,multidamage=1,multibullet=2,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\青云2级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=3,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=52,cooltime=3000,wradius=5,bullettype=2,speed=0.100,bradius=36,adamage=24,bdamage=8,damagetoa=0,damagetob=0,groundflag=true,airflag=true,freeze=0,improvecost=60,improvetime=8000,improvepic=1,
									multihit=0.1,multidamage=1,multibullet=2,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\青云3级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=4,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=97,cooltime=2500,wradius=6,bullettype=2,speed=0.100,bradius=40,adamage=48,bdamage=16,damagetoa=0,damagetob=0,groundflag=true,airflag=true,freeze=0,improvecost=150,improvetime=11000,improvepic=1,
									multihit=0.1,multidamage=1,multibullet=2,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\青云4级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=5,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=180,cooltime=2500,wradius=7,bullettype=2,speed=0.100,bradius=48,adamage=96,bdamage=72,damagetoa=0,damagetob=0,groundflag=true,airflag=true,freeze=0,improvecost=nil,improvetime=15000,improvepic=1,
									multihit=0.1,multidamage=1,multibullet=2,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\青云5级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=6,attackeffect=nil
								},

							},

							  
							
						[4] = {
								{ 	createcost=15,sell=14,cooltime=1500,wradius=3,bullettype=3,speed=0.125,bradius=nil,adamage=2,bdamage=8,damagetoa=0,damagetob=0,groundflag=true,airflag=false,freeze=0,improvecost=30,improvetime=2000,improvepic=1,
									multihit=0.2,multidamage=1,multibullet=3,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\合欢1级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=2,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=32,cooltime=1500,wradius=3,bullettype=3,speed=0.125,bradius=nil,adamage=4,bdamage=16,damagetoa=0,damagetob=0,groundflag=true,airflag=false,freeze=0,improvecost=50,improvetime=5000,improvepic=1,
									multihit=0.2,multidamage=2,multibullet=3,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\合欢2级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=2,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=68,cooltime=1200,wradius=3,bullettype=3,speed=0.125,bradius=nil,adamage=8,bdamage=32,damagetoa=0,damagetob=0,groundflag=true,airflag=false,freeze=0,improvecost=100,improvetime=8000,improvepic=1,
									multihit=0.2,multidamage=3,multibullet=3,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\合欢3级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=2,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=145,cooltime=1200,wradius=3,bullettype=3,speed=0.125,bradius=nil,adamage=16,bdamage=64,damagetoa=0,damagetob=0,groundflag=true,airflag=false,freeze=0,improvecost=185,improvetime=11000,improvepic=1,
									multihit=0.3,multidamage=3,multibullet=3,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\合欢4级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=2,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=280,cooltime=1200,wradius=5,bullettype=3,speed=0.125,bradius=nil,adamage=32,bdamage=128,damagetoa=0,damagetob=0,groundflag=true,airflag=false,freeze=0,improvecost=nil,improvetime=15000,improvepic=1,
									multihit=0.5,multidamage=3,multibullet=3,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\合欢5级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=2,attackeffect=nil
								},

							},
							
						[5] = {
								{ 	createcost=30,sell=22,cooltime=2500,wradius=2,bullettype=4,speed=0.100,bradius=24,adamage=0,bdamage=12,damagetoa=0,damagetob=0,groundflag=true,airflag=true,freeze=0.3,improvecost=30,improvetime=2000,improvepic=1,
									multihit=0,multidamage=1,multibullet=4,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\天音1级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=1,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=45,cooltime=2000,wradius=2,bullettype=4,speed=0.100,bradius=24,adamage=0,bdamage=24,damagetoa=0,damagetob=0,groundflag=true,airflag=true,freeze=0.3,improvecost=40,improvetime=5000,improvepic=1,
									multihit=0,multidamage=1,multibullet=4,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\天音2级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=1,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=75,cooltime=2000,wradius=3,bullettype=4,speed=0.100,bradius=24,adamage=0,bdamage=36,damagetoa=0,damagetob=0,groundflag=true,airflag=true,freeze=0.4,improvecost=60,improvetime=8000,improvepic=1,
									multihit=0,multidamage=1,multibullet=4,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\天音3级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=2,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=120,cooltime=2000,wradius=3,bullettype=4,speed=0.100,bradius=28,adamage=0,bdamage=48,damagetoa=0,damagetob=0,groundflag=true,airflag=true,freeze=0.4,improvecost=80,improvetime=11000,improvepic=1,
									multihit=0,multidamage=1,multibullet=4,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\天音4级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=2,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=180,cooltime=1800,wradius=4,bullettype=4,speed=0.100,bradius=28,adamage=0,bdamage=64,damagetoa=0,damagetob=0,groundflag=true,airflag=true,freeze=0.5,improvecost=nil,improvetime=15000,improvepic=1,
									multihit=0,multidamage=1,multibullet=4,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\天音5级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=3,attackeffect=nil
								},

							},
							
						[6] = {
								{ 	createcost=20,sell=15,cooltime=3000,wradius=4,bullettype=5,speed=0.090,bradius=nil,adamage=3,bdamage=3,damagetoa=0.2,damagetob=0.2,groundflag=true,airflag=false,freeze=0,improvecost=15,improvetime=2000,improvepic=1,
									multihit=0.1,multidamage=1,multibullet=5,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\鬼道1级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=3,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=26,cooltime=3000,wradius=5,bullettype=5,speed=0.090,bradius=nil,adamage=7,bdamage=7,damagetoa=0.3,damagetob=0.3,groundflag=true,airflag=false,freeze=0,improvecost=25,improvetime=5000,improvepic=1,
									multihit=0.1,multidamage=1,multibullet=5,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\鬼道2级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=4,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=42,cooltime=3000,wradius=5,bullettype=5,speed=0.090,bradius=nil,adamage=14,bdamage=14,damagetoa=0.4,damagetob=0.4,groundflag=true,airflag=true,freeze=0,improvecost=80,improvetime=8000,improvepic=1,
									multihit=0.1,multidamage=1,multibullet=5,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\鬼道3级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=4,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=105,cooltime=2500,wradius=6,bullettype=5,speed=0.090,bradius=nil,adamage=30,bdamage=30,damagetoa=0.5,damagetob=0.5,groundflag=true,airflag=true,freeze=0,improvecost=120,improvetime=11000,improvepic=1,
									multihit=0.1,multidamage=1,multibullet=5,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\鬼道4级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=5,attackeffect=nil
								},
								
								{ 	createcost=nil,sell=195,cooltime=2500,wradius=7,bullettype=5,speed=0.090,bradius=24,adamage=80,bdamage=80,damagetoa=0.6,damagetob=0.6,groundflag=true,airflag=true,freeze=0,improvecost=nil,improvetime=15000,improvepic=1,
									multihit=0.1,multidamage=1,multibullet=5,hitadd=nil,toabadd=nil,tofreezeadd=nil,multiadd=nil,multidamageadd=nil,speedadd=nil,addradius=nil,
									picname="TOWERDEFENSE\\weaponpic\\鬼道5级兵种.tga",frames=48,direnum=8,franum=2,acfranum=4,timeperfra=300,attperfra=100,rangecircle=6,attackeffect=nil
								},

							},
							
					};						
					
					
		
				
-- ---------------------------------------------moster index  ---------------需要存图片的大小，显示的时候顶点与中心点有错位-------------
	--其中picw和pich为控件图像的宽和高，显示时需要挪动中心点到合适位置

	--需要添加图片的地址和大小,speed为每毫秒多少像素

	TD_MNormal=50;		--不飞的怪物种类数目
	TD_MonsterIndex = {
						[1]	= { speed=0.012,life=20,rewardgold=2,adefence=0,bdefence=0.35,speeddefence=0,rewardscore=1,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小鸡24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[2] = { speed=0.012,life=26,rewardgold=2,adefence=0.35,bdefence=0,speeddefence=0,rewardscore=1,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小猫24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[3]	= { speed=0.012,life=33,rewardgold=2,adefence=0.35,bdefence=0.05,speeddefence=0,rewardscore=1,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙狗狗24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[4] = { speed=0.02,life=30,rewardgold=2,adefence=0,bdefence=0.35,speeddefence=0,rewardscore=2,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙兔子24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
						
						[5]	= { speed=0.012,life=40,rewardgold=3,adefence=0.25,bdefence=0.2,speeddefence=0,rewardscore=2,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小鸡24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[6] = { speed=0.016,life=52,rewardgold=3,adefence=0.1,bdefence=0.35,speeddefence=0,rewardscore=2,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小猫24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[7]	= { speed=0.018,life=58,rewardgold=3,adefence=0,bdefence=0.40,speeddefence=0,rewardscore=3,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙狗狗24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[8] = { speed=0.024,life=66,rewardgold=3,adefence=0.4,bdefence=0.1,speeddefence=0,rewardscore=3,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙兔子24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								}, 
								
						[9]	= { speed=0.018,life=73,rewardgold=3,adefence=0.6,bdefence=0.15,speeddefence=0,rewardscore=3,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙暗鸡24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[10] = { speed=0.018,life=79,rewardgold=3,adefence=0.15,bdefence=0.6,speeddefence=0,rewardscore=4,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙黑猫24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[11]	= { speed=0.036,life=84,rewardgold=4,adefence=0,bdefence=0.45,speeddefence=0,rewardscore=4,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙紫狗24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[12] = { speed=0.054,life=88,rewardgold=4,adefence=0.45,bdefence=0,speeddefence=0,rewardscore=4,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙暗兔24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[13]	= { speed=0.036,life=96,rewardgold=4,adefence=0.75,bdefence=0.15,speeddefence=0,rewardscore=5,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小鸡24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[14] = { speed=0.036,life=110,rewardgold=4,adefence=0.6,bdefence=0.15,speeddefence=0,rewardscore=5,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小猫24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
						
						[15]	= { speed=0.036,life=124,rewardgold=4,adefence=0.15,bdefence=0.6,speeddefence=0,rewardscore=5,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙狗狗24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[16] = { speed=0.048,life=138,rewardgold=5,adefence=0.5,bdefence=0,speeddefence=0,rewardscore=6,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙兔子24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[17]	= { speed=0.036,life=145,rewardgold=5,adefence=0,bdefence=0.4,speeddefence=0,rewardscore=6,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小鸡24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[18] = { speed=0.036,life=160,rewardgold=5,adefence=0.2,bdefence=0.4,speeddefence=0,rewardscore=6,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小猫24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								}, 
								
						[19]	= { speed=0.036,life=175,rewardgold=5,adefence=0.2,bdefence=0.7,speeddefence=0,rewardscore=7,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙狗狗24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[20] = { speed=0.036,life=191,rewardgold=5,adefence=0.7,bdefence=0.2,speeddefence=0,rewardscore=7,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙兔子24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[21]	= { speed=0.040,life=212,rewardgold=5,adefence=0,bdefence=0.55,speeddefence=0,rewardscore=7,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙暗鸡24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[22] = { speed=0.036,life=232,rewardgold=5,adefence=0.55,bdefence=0,speeddefence=0,rewardscore=8,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙黑猫24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[23]	= { speed=0.040,life=252,rewardgold=5,adefence=0.55,bdefence=0.25,speeddefence=0,rewardscore=8,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙紫狗24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[24] = { speed=0.048,life=272,rewardgold=5,adefence=0.2,bdefence=0.8,speeddefence=0,rewardscore=8,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙暗兔24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
						
						[25]	= { speed=0.036,life=300,rewardgold=5,adefence=0.8,bdefence=0.2,speeddefence=0,rewardscore=9,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小鸡24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[26] = { speed=0.036,life=330,rewardgold=5,adefence=0.7,bdefence=0,speeddefence=0,rewardscore=9,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小猫24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[27]	= { speed=0.036,life=370,rewardgold=5,adefence=0,bdefence=0.7,speeddefence=0,rewardscore=9,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙狗狗24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[28] = { speed=0.048,life=412,rewardgold=5,adefence=0.7,bdefence=0.3,speeddefence=0,rewardscore=10,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙兔子24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								}, 
								
						[29]	= { speed=0.036,life=460,rewardgold=6,adefence=0.3,bdefence=0.8,speeddefence=0,rewardscore=10,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小鸡24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[30] = { speed=0.036,life=512,rewardgold=6,adefence=0.8,bdefence=0.3,speeddefence=0,rewardscore=10,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小猫24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[31]	= { speed=0.036,life=584,rewardgold=6,adefence=0.90,bdefence=0,speeddefence=0,rewardscore=11,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙狗狗24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[32] = { speed=0.054,life=660,rewardgold=6,adefence=0,bdefence=0.90,speeddefence=0,rewardscore=11,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙兔子24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[33]	= { speed=0.036,life=760,rewardgold=7,adefence=0.35,bdefence=.35,speeddefence=0,rewardscore=11,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙暗鸡24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[34] = { speed=0.036,life=880,rewardgold=7,adefence=0.9,bdefence=0.35,speeddefence=0,rewardscore=12,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙黑猫24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
						
						[35]	= { speed=0.036,life=1000,rewardgold=7,adefence=0.35,bdefence=0.9,speeddefence=0,rewardscore=12,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙紫狗24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[36] = { speed=0.054,life=1160,rewardgold=7,adefence=0.4,bdefence=0,speeddefence=0,rewardscore=12,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙暗兔24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[37]	= { speed=0.036,life=1340,rewardgold=8,adefence=0.5,bdefence=0.4,speeddefence=0,rewardscore=13,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙暗鸡24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[38] = { speed=0.036,life=1560,rewardgold=8,adefence=0.4,bdefence=0.4,speeddefence=0,rewardscore=13,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙黑猫24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								}, 
								
						[39]	= { speed=0.044,life=1800,rewardgold=8,adefence=0.4,bdefence=1,speeddefence=0,rewardscore=13,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙紫狗24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[40] = { speed=0.054,life=2100,rewardgold=8,adefence=1,bdefence=0.4,speeddefence=0,rewardscore=15,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙暗兔24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						
						[41] = { speed=0.044,life=2300,rewardgold=8,adefence=0.6,bdefence=0.4,speeddefence=0,rewardscore=15,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小鸡24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},  
								
						[42] = { speed=0.044,life=2500,rewardgold=8,adefence=0.6,bdefence=0.4,speeddefence=0,rewardscore=15,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小猫24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								}, 
								
						[43] = { speed=0.044,life=2700,rewardgold=9,adefence=0.6,bdefence=0.4,speeddefence=0,rewardscore=15,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙狗狗24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								}, 
								
						[44] = { speed=0.060,life=2900,rewardgold=9,adefence=0.6,bdefence=0.4,speeddefence=0,rewardscore=15,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙兔子24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								}, 
								
						[45] = { speed=0.044,life=3100,rewardgold=9,adefence=0.6,bdefence=0.8,speeddefence=0,rewardscore=15,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小鸡24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								}, 
								
						[46] = { speed=0.044,life=3300,rewardgold=9,adefence=0.4,bdefence=0.8,speeddefence=0,rewardscore=15,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小猫24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								}, 
								
						[47] = { speed=0.044,life=3500,rewardgold=9,adefence=0.8,bdefence=0.6,speeddefence=0,rewardscore=15,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙狗狗24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[48] = { speed=0.060,life=3700,rewardgold=11,adefence=0.8,bdefence=0.6,speeddefence=0,rewardscore=15,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙兔子24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								}, 
								
						[49] = { speed=0.050,life=3900,rewardgold=11,adefence=0.8,bdefence=0.6,speeddefence=0,rewardscore=15,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙紫狗24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},
								
						[50] = { speed=0.072,life=4200,rewardgold=13,adefence=0.8,bdefence=0.6,speeddefence=0,rewardscore=15,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙暗兔24.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 24,hsize=12,
								},		
						
								

						
						
						--flytype
						[51]	= { speed=0.020,life=45,rewardgold=4,adefence=0.4,bdefence=0,speeddefence=0,rewardscore=2,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小鸟24.tga",frames=24,direnum=8,franum=3,timeperfra=200,size = 24,hsize=12,

								},
								
						[52]	= { speed=0.020,life=88,rewardgold=4,adefence=0,bdefence=0.5,speeddefence=0,rewardscore=3,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙青鸟24.tga",frames=24,direnum=8,franum=3,timeperfra=200,size = 24,hsize=12,

								},
								
						[53]	= { speed=0.020,life=160,rewardgold=5,adefence=0.6,bdefence=0,speeddefence=0,rewardscore=4,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙阴灵32.tga",frames=24,direnum=8,franum=3,timeperfra=200,size = 32,hsize=16,

								},
								
						[54]	= { speed=0.020,life=272,rewardgold=5,adefence=0.1,bdefence=0.7,speeddefence=0,rewardscore=5,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小鸟24.tga",frames=24,direnum=8,franum=3,timeperfra=200,size = 24,hsize=12,

								},
								
						[55]	= { speed=0.025,life=502,rewardgold=5,adefence=0.4,bdefence=0.4,speeddefence=0,rewardscore=6,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙青鸟24.tga",frames=24,direnum=8,franum=3,timeperfra=200,size = 24,hsize=12,

								}, 
								
						[56]	= { speed=0.025,life=1124,rewardgold=6,adefence=0.4,bdefence=0.5,speeddefence=0,rewardscore=7,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙阴灵32.tga",frames=24,direnum=8,franum=3,timeperfra=200,size = 32,hsize=16,

								},
								
						[57]	= { speed=0.025,life=60,rewardgold=6,adefence=0,bdefence=0,speeddefence=0,rewardscore=3,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙青鸟24.tga",frames=24,direnum=8,franum=3,timeperfra=200,size = 24,hsize=12,

								},
								
						[58]	= { speed=0.025,life=180,rewardgold=6,adefence=0.6,bdefence=0.2,speeddefence=0,rewardscore=4,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙小鸟24.tga",frames=24,direnum=8,franum=3,timeperfra=200,size = 24,hsize=12,

								}, 
								
						[59]	= { speed=0.025,life=502,rewardgold=8,adefence=0.8,bdefence=0,speeddefence=0,rewardscore=5,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙青鸟24.tga",frames=24,direnum=8,franum=3,timeperfra=200,size = 24,hsize=12,

								},
						
						[60]	= { speed=0.030,life=1124,rewardgold=11,adefence=0.6,bdefence=0.2,speeddefence=0,rewardscore=6,deathtype=2,
								picname="TOWERDEFENSE\\monsterpics\\诛仙阴灵32.tga",frames=24,direnum=8,franum=3,timeperfra=200,size = 32,hsize=16,

								},						
					};
	
	--普通怪物不飞行随机对应表
	TD_RdTypeIndex = 
	{
			--第一关的怪物种类随机范围，如果会出现变化与平均水平的，请设定怪物的种类，不要使用random
		[1] = {
					--level的范围限制--如果level=7，共40个level，而限制共有5段，则取1+7/(40/5)=1，第一段的范围，在第一段范围内随机
					{1,3},{4,9},{10,14},{14,16},{17,20},
			},

		[2] = {
					
					{1,8},{9,16},{17,24},{25,32},{32,40},
			},
		[3] = {
					
					{1,8},{9,16},{17,24},{25,32},{32,40},
			},
		[4] = {
					
					{1,9},{10,18},{19,30},{31,40},{41,50},
			},		
							
			
						
	};

	--普通怪物飞行随机对应表

	TD_FlyRdTypeIndex = 
	{
			--第一关的怪物种类随机范围，如果会出现变化与平均水平的，请设定怪物的种类，不要使用random
		[1] = {
					--level的范围限制--如果level=7，共40个level，而限制共有5段，则 向上取整(7*(40/5))=1，第一段的范围，在第一段范围内随机
					{51,54},{52,53},{52,55},{54,56},{54,57},
			},
		[2] = {
					
					{57,57},{52,54},{54,56},{54,57},{57,59},
			},
		[3] = {
					
					{51,53},{53,54},{54,55},{55,55},{59,60},
			},
		[4] = {
					
					{51,54},{54,57},{57,60},
			},		
							
						
	};			
			
	TD_MBossNormal=9;		--不飞的怪物种类数目				
	TD_MonsterBossIndex = {
							[1] = {speed=0.020,life=1220,rewardgold=40,adefence=0.2,bdefence=0,speeddefence=0,rewardscore=30,deathtype=2,
									picname="TOWERDEFENSE\\monsterpics\\诛仙野猪32.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 32,hsize=16,				
									},
							
							[2] = {speed=0.024,life=1920,rewardgold=60,adefence=0,bdefence=0.2,speeddefence=0,rewardscore=40,deathtype=2,
									picname="TOWERDEFENSE\\monsterpics\\诛仙绿野猪32.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 32,hsize=16,
									},
									
							[3] = {speed=0.025,life=5400,rewardgold=140,adefence=0.2,bdefence=0.2,speeddefence=0,rewardscore=50,deathtype=2,
									picname="TOWERDEFENSE\\monsterpics\\诛仙野猪32.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 32,hsize=16,					
									},
									
							[4] = {speed=0.050,life=9600,rewardgold=120,adefence=0.5,bdefence=0,speeddefence=0,rewardscore=60,deathtype=2,
									picname="TOWERDEFENSE\\monsterpics\\诛仙小红32.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 32,hsize=16,						
									},
							
							[5] = {speed=0.040,life=36000,rewardgold=200,adefence=0,bdefence=0.5,speeddefence=0,rewardscore=70,deathtype=2,
									picname="TOWERDEFENSE\\monsterpics\\诛仙小白32.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 32,hsize=16,
									},
									
							[6] = {speed=0.060,life=1600,rewardgold=30,adefence=0.5,bdefence=0,speeddefence=0,rewardscore=20,deathtype=2,
									picname="TOWERDEFENSE\\monsterpics\\诛仙紫狸32.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 32,hsize=16,					
									}, 
									
							[7] = {speed=0.050,life=3320,rewardgold=40,adefence=0.5,bdefence=0,speeddefence=0,rewardscore=30,deathtype=2,
									picname="TOWERDEFENSE\\monsterpics\\诛仙红狐狸32.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 32,hsize=16,						
									},
							
							[8] = {speed=0.030,life=5800,rewardgold=50,adefence=0,bdefence=1,speeddefence=0,rewardscore=40,deathtype=2,
									picname="TOWERDEFENSE\\monsterpics\\诛仙绿野猪32.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 32,hsize=16,
									},
									
							[9] = {speed=0.070,life=150000,rewardgold=500,adefence=0.3,bdefence=0.3,speeddefence=0,rewardscore=400,deathtype=2,
									picname="TOWERDEFENSE\\monsterpics\\诛仙小白32.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 32,hsize=16,
									},
							
							--flytype
							[10] = {speed=0.020,life=1280,rewardgold=100,adefence=0,bdefence=0,speeddefence=0,rewardscore=45,deathtype=2,
									picname="TOWERDEFENSE\\monsterpics\\诛仙红蝴蝶32.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 32,hsize=16,
									},
									
							[11] = {speed=0.020,life=4228,rewardgold=100,adefence=0,bdefence=0,speeddefence=0,rewardscore=60,deathtype=2,
									picname="TOWERDEFENSE\\monsterpics\\诛仙绿蝴蝶32.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 32,hsize=16,
									}, 
									
							[12] = {speed=0.024,life=8560,rewardgold=100,adefence=0,bdefence=0,speeddefence=0,rewardscore=75,deathtype=2,
									picname="TOWERDEFENSE\\monsterpics\\诛仙炎魔32.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 32,hsize=16,
									},
									
							[13] = {speed=0.024,life=12300,rewardgold=200,adefence=0,bdefence=0,speeddefence=0,rewardscore=300,deathtype=2,
									picname="TOWERDEFENSE\\monsterpics\\诛仙炎魔32.tga",frames=24,direnum=8,franum=3,timeperfra=150,size = 32,hsize=16,
									},
	
						};						
					
	--Boss不飞怪物随机对应表

	TD_RdBossTypeIndex = 
	{
			--第一关的怪物种类随机范围，如果会出现变化与平均水平的，请设定怪物的种类，不要使用random
		[1] = {
					{1,2},{2,2},{2,3},{3,4},{4,4},
			},

		[2] = {
					{1,2},{2,3},{3,4},{6,8},{4,4},
			},	
			
		[3] = {
					{1,2},{2,3},{3,4},{7,8},{4,5},
			},
			
		[4] = {
					{1,3},{2,4},{3,6},{4,7},{5,9},
			},	

	
	};
	--飞行Boss怪物随机对应表

	TD_FlyRdBossTypeIndex = 
	{
			--第一关的怪物种类随机范围，如果会出现变化与平均水平的，请设定怪物的种类，不要使用random
		[1] = {
					{10,10},{10,11},{10,11},{11,11},{11,12},
			},
		[2] = {
					{10,10},{10,11},{11,11},{11,12},{12,12},
			},
			
		[3] = {
					{10,10},{10,11},{11,11},{11,12},{12,13},
			},
			
		[4] = {
					{10,10},{11,11},{12,13},
			},

	};


	TD_MonsterVariIndex = {


						};
						
						
---------------------------------------------------------- about map ------------------------------------------------------
	--一些约定

	TD_Path = 0;	--通路
	TD_Block = 1;	--阻碍
	TD_Entry = 2;	--入口
	TD_Exit = 4;	--出口
	
	--每一关dialog的底图

    TD_MapPic= {
    				[1]="TOWERDEFENSE\\window\\TD_Map1.tga",
    				[2]="TOWERDEFENSE\\window\\TD_Map2.tga",
    				[3]="TOWERDEFENSE\\window\\TD_Map3.tga",
    				[4]="TOWERDEFENSE\\window\\TD_Map4.tga",
    		};
	
	
	--每一关map的规模，高和宽

    TD_MapSize = {
    				[1]={	height = 14, width = 30	},
    				[2]={	height = 27, width = 30	},
    				[3]={	height = 30, width = 30	},
    				[4]={	height = 30, width = 30	},  
    			 };

    --每一关map左上角起始点在每个界面上的相对位置,当实际可以走的位置比较少的时候，为了让下面TD_MapBlock更简单些，可以把起点设低一点，比如32*32的第一行和最后一样不能用，是
    --树丛或者其他时，可以在y上面加上32，TD_MapSize的height写成30-4=26
    TD_MapOrigin = {
    					[1]={ x = 16, y = 76+128 },
    					[2]={ x = 16, y = 76+32 },
    					[3]={ x = 16, y = 76 },
    					[4]={ x = 16, y = 76 },
    				};
    				

    --每个map方格的像素级别大小

    TD_MapBlockSize = 16;		
    
    --每一关的map初始化的障碍,即不能放武器，怪物也不能走的块
    TD_MapBlock = {
    				[1]={	--周围一圈怪不能走的路,得去掉TD_MapPath中入口和出口（如果入口为(1,11)而1和2列都不能放武器不能走怪物，那么(1,11)和(2,11)都应该从表中去掉）

							{1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{7,1},{9,1},{10,1},{11,1},{12,1},{13,1},{19,1},{20,1},{21,1},{22,1},{24,1},{25,1},{26,1},{28,1},{29,1},{30,1},
							{1,2},{2,2},{25,2},{26,2},{30,2},
							{1,13},{2,13},{3,13},{7,13},{8,13},{9,13},{10,13},{11,13},{12,13},{13,13},{14,13},{17,13},{19,13},{21,13},{22,13},{23,13},{24,13},{25,13},{26,13},{27,13},{28,13},{29,13},{30,13},				
    				  		{1,14},{2,14},{3,14},{4,14},{5,14},{6,14},{7,14},{8,14},{9,14},{10,14},{11,14},{14,14},{15,14},{16,14},{17,14},{18,14},{19,14},{20,14},{21,14},{22,14},{23,14},{24,14},{25,14},{26,14},{27,14},{28,14},{29,14},{30,14}, 
    				  		--地图内除周围一圈外内部放置的障碍物
    				  		{1,3},{2,3},{1,4},{2,4}, {30,3},{29,3},{30,4},{15,5},{16,5},{15,6},{16,6},{17,5},{18,5},{17,6},{18,6}, {27,5},{28,5},{27,6},{28,6},  {29,5},{30,5},{29,6},{30,6},	
							{29,9},{30,9},{29,10},{30,10},  {1,11},{2,11},{1,12},{2,12}, {8,12},{9,12},{10,12}, {29,11},{30,11},{29,12},{30,12}, 
    					},

    				[2]={	
							{1,1},{2,1},{27,1},{28,1},{29,1},{30,1},  {27,2},{28,2},{29,2},{30,2},  {1,3},{29,3},{30,3},  {1,4},{2,4},{30,4},  
							{1,5},{2,5},{30,5},  {1,6},{2,6},{18,6},{19,6},{20,6},{29,6},{30,6},  {1,7},{2,7},{18,7},{19,7},{20,7},{22,7},{30,7},							
    						{1,8},{2,8},{22,8},{29,8},{30,8},  {1,9},{2,9},{9,9},{10,9},  {1,10},{2,10},{9,10},{10,10},  {13,11},{16,11},{17,11},{29,11},{30,11},
    						{11,12},{13,12},{16,12},{17,12},{28,12},{29,12},{30,12},  {10,13},{11,13},{15,13},{28,13},{29,13},{30,13},
    						{15,14},{16,14},{17,14},{28,14},{29,14},{30,14},  {28,15},{29,15},{30,15},{28,16},{29,16},{30,16},  {1,17},{2,17},{19,17},{20,17},{21,17},{22,17},{1,18},{2,18},{19,18},{20,18},{21,18},{22,18},
    						{1,19},{2,19},{11,19},{14,19},{30,19},   {1,20},{2,20},{3,20},{4,20},{5,20},{10,20},{11,20},{13,20},{14,20},{30,20},
    						{1,21},{2,21},{3,21},{4,21},{5,21},{6,21},{30,21},  {1,22},{2,22},{3,22},{4,22},{5,22},{6,22},{26,22},{27,22},{28,22},{30,22}, 
    						{1,23},{2,23},{3,23},{4,23},{5,23},{6,23},{25,23},{26,23},{27,23},{28,23},  {1,24},{2,24},{3,24},{4,24},{5,24},{24,24},{25,24},{26,24},{27,24},{28,24}, 
    						{1,25},{2,25},{19,25},{20,25},{25,25},{26,25},{27,25},{28,25},   {1,26},{2,26},{19,26},{20,26},{26,26},{30,26},
    						{1,27},{2,27},{3,27},{10,27},{11,27},{30,27},
    						
    					},

    				[3]={
        					{1,1},{1,2},{1,3},  {1,7},{1,8},{1,9},{1,10},   {1,18},{1,19},{1,20},{1,21},    {1,25},{1,26},{1,27},{1,28},{1,29},{1,30},
        					{2,1},{2,2},{2,3},  {2,7},{2,8},{2,9},{2,10},   {2,18},{2,19},{2,20},{2,21},    {2,25},{2,26},{2,27},{2,28},{2,29},{2,30},
        					
        					{3,1},{3,2},{3,3},  {3,24},{3,25},{3,26},{3,27},{3,28},{3,29},{3,30},
							{4,1},{4,2},{4,3},  {4,23},{4,24},{4,25},{4,26},{4,27},{4,28},{4,29},{4,30}, 
							
							{5,1},{5,2},{5,3},  {5,7},{5,8},  {5,23},{5,24},{5,25},{5,26},{5,27},{5,28},{5,29},{5,30},  					    					
        					{6,1},{6,2},        {6,7},{6,8},  {6,23},{6,24},{6,25},{6,26},{6,27},{6,28},{6,29},{6,30},
        					
        					{7,1},  {7,5},{7,6},{7,7},{7,8},  {7,24},{7,25},{7,26},{7,27},{7,28},{7,29},{7,30},
        					{8,1},  {8,5},{8,6},{8,7},{8,8},  {8,24},{8,25},{8,26},{8,27},{8,28},{8,29},{8,30},

        					
        					{9,1},  {9,5},{9,6},{9,7},{9,8},  {9,23},{9,24},{9,25},{9,26},{9,27},{9,28},
        					{10,7},{10,8},  {10,23},{10,24},    {10,27},{10,28},{10,30},
        					
        					{11,1},{11,2},{11,3},{11,4},{11,5},{11,6},{11,7},{11,8},{11,9},{11,10},    {11,30}, 
        					{12,1},{12,2},{12,3},{12,4},{12,5},{12,6},{12,7},{12,8},{12,9},{12,10},    {12,30},
       					
        					{13,30},{14,29},{14,30},
        					{15,27},{15,28},
        					
							{18,27},{18,28},{18,29},{18,30},
							
        					{19,1},{19,2},{19,3},{19,4},  {19,29},{19,30},
        					{20,1},{20,2},{20,3},{20,4},  {20,30},
        					
        					{21,8},
        					{22,1},  {22,3},{22,4},{22,5},{22,6},{22,7},{22,8},  {22,20},
        					
        					{23,1},{23,2},{23,3},{23,4},{23,5},{23,7},{23,8},   {23,18},{23,19},{23,20},
        					{24,1},{24,2},{24,3},{24,4},{24,5},{24,7},{24,8},   {24,21},{24,22},    {24,27},{24,30},
        					
          					{25,1},{25,2},{25,3},{25,4},{25,5},  {25,7},{25,8},  {25,19},{25,20},{25,21},{25,22},   {25,26},{25,27},{25,28},{25,29},{25,30},
        					{26,1},{26,2},{26,3},{26,4},{26,5},  {26,19},{26,20},{26,21},{26,22},   {26,26},{26,27},{26,28},{26,29},{26,30},
        					
        					{27,1},{27,2},{27,3},{27,4},  {27,11},   {27,25},{27,26},{27,27},{27,28},{27,29},{27,30},
        					{28,1},{28,2},{28,3},{28,4},  {28,11},   {28,14},{28,17},   {28,22},{28,23},{28,24},{28,25},{28,26},{28,27},{28,28},{27,29},{28,30},
        					
        					{29,1},{29,2},{29,3},{29,4},{29,5},{29,6},{29,9},   {29,11},  {29,18},{29,19},{29,21},{29,22},{29,23},{29,24},{29,25},{29,26},{29,27},{29,28},{29,29},{29,30},
        					{30,1},{30,2},{30,3},{30,4},{30,5},{30,6},{30,7},{30,8},{30,9},  {30,13},{30,14},  {30,18},{30,19},  {30,21},{30,22},{30,23},{30,24},{30,25},{30,26},{30,27},{30,28},{30,29},{30,30},							
    					},
    					
    				[4]={
                            {1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{7,1},{8,1},{9,1},{10,1},{11,1},{12,1},    {19,1},{20,1},{21,1},{22,1},{23,1},{24,1},{25,1},{26,1},{27,1},{28,1},{29,1},{30,1},
                            {1,2},{2,2},{3,2},{4,2},{5,2},{6,2},{7,2},{8,2},{9,2},{10,2},{11,2},{12,2},    {19,2},{20,2},{21,2},{22,2},{23,2},{24,2},{25,2},{26,2},{27,2},{28,2},{29,2},{30,2},
                            
                            {1,3},{2,3},{3,3},{4,3},{5,3},{6,3},{7,3},{8,3},{9,3},{10,3},{11,3},{12,3},    {19,3},{20,3},{21,3},{22,3},{23,3},{24,3},{25,3},{26,3},{27,3},{28,3},{29,3},{30,3},
                            {1,4},{2,4},{3,4},{4,4},   {8,4},{9,4},{10,4},{11,4},{12,4},    {19,4},{20,4},{21,4},{22,4},{23,4},{24,4},{25,4},{26,4},{27,4},{28,4},{29,4},{30,4},
                            
                            {1,5},{2,5},{3,5},{4,5},{5,5},    {22,5},{23,5},  {27,5},{28,5},{29,5},{30,5},
                            {1,6},{2,6},{3,6},{4,6},{5,6},{6,6},  {22,6},{23,6},  {27,6},{28,6},{29,6},{30,6},
    
                            {1,7},{2,7},  {23,7},{24,7},   {28,7},{29,7},{30,7},
                            {1,8},{2,8},{3,8},   {23,8},{24,8},   {28,8},{29,8},{30,8},
                            
                            {1,9},  {15,9},{16,9},   {22,9},{23,9},   {28,9},{29,9},{30,9},
                            {1,10}, {15,10},{16,10},{17,10},   {27,10},{28,10},{29,10},{30,10},
                            
                            {1,11},{15,11},{16,11},  {1,12},{15,12},{16,12},
                            
                            {1,13},{2,13},{3,13},{4,13},  {11,13},{12,13},{13,13},{14,13},{15,13},{16,13},{17,13},{18,13},{19,13},{20,13},
                            {1,14},{2,14},{3,14},{4,14},  {11,14},{12,14},{13,14},{14,14},{15,14},{16,14},{17,14},{18,14},{19,14},{20,14},
                            
                            {14,15},{15,15},{16,15},{17,15},{18,15},  {17,16},{18,16},
                            
                            {1,17},{2,17},{3,17},{27,17},{28,17},{29,17},{30,17},  {1,18},{2,18},  {23,18},  {28,18},{29,18},{30,18},
                            
                            {1,19},  {22,19},{23,19},{24,19},  {28,19},{29,19},{30,19},  
                            {1,20},  {22,20},{23,20},   {28,20},{29,20},{30,20},
                            
                            {1,21},{2,21},  {22,21},{23,21},  {28,21},{29,21},{30,21},
                            {1,22},{2,22},  {23,22},    {28,22},{29,22},{30,22},
                            
                            {1,23},{2,23},  {22,23},{23,23},  {27,23},{28,23},{29,23},{30,23},
                            {1,24},{2,24},  {27,24},{28,24},{29,24},{30,24},
                            
                            {1,25},{2,25},  {29,25},{30,25},  {1,26},{2,26},{3,26},{4,26},  {25,26},{26,26},{27,26},{28,26},{29,26},{30,26},
                            
                            {1,27},{2,27},{3,27},{4,27},{5,27},{6,27},{7,27},  {14,27},{18,27},  {24,27},{25,27},{26,27},{27,27},{28,27},{29,27},{30,27},                            
                            {1,28},{2,28},{3,28},{4,28},{5,28},{6,28},{7,28},{8,28},{9,28},{10,28},{11,28},{12,28},  {14,28},{18,28},  {20,28},{21,28},{22,28},{23,28},{24,28},{25,28},{26,28},{27,28},{28,28},{29,28},{30,28},
                            
                            {1,29},{2,29},{3,29},{4,29},{5,29},{6,29},{7,29},{8,29},{9,29},{10,29},{11,29},{12,29},{13,29},{14,29},  {18,29},{19,29},{20,29},{21,29},{22,29},{23,29},{24,29},{25,29},{26,29},{27,29},{28,29},{29,29},{30,29},
                            {1,30},{2,30},{3,30},{4,30},{5,30},{6,30},{7,30},{8,30},{9,30},{10,30},{11,30},{12,30},{13,30},{14,30},  {18,30},{19,30},{20,30},{21,30},{22,30},{23,30},{24,30},{25,30},{26,30},{27,30},{28,30},{29,30},{30,30},

    					},



    			  };


   	--每关游戏的入口和出口的位置以及宽度；不能放武器，怪物能走
    TD_MapPath = {
    				[1]={ 			--第一关

    						 					--第一组出口和入口
    						[1]={
    								entry = { { 1,5 },{ 1,6 },{ 1,7 },{ 1,8 },{ 1,9 },{1,10}, },
    								exit = { { 30,7 },{ 30,8 }, },
    							},

    					},
    				[2]={ 			--第一关

    						 					--第一组出口和入口
    						[1]={
    								entry = { { 1,11 },{ 1,12 },{ 1,13 },{ 1,14 },{ 1,15 },{1,16},},--
    								exit = { { 30,9 },{ 30,10 },{ 30,17 },{ 30,18 }, },--
    							},
  							
  						}, 

    				[3]={ 			--第一关

    						 					--第一组出口和入口
    						[1]={
    								entry = { { 1,11 },{ 1,12 },{ 1,13 },{ 1,14 },{ 1,15 },{1,16},{1,17}, },--
    								exit = { { 30,15 },{ 30,16 }, },--
    							},
     						[2]={
    								entry = { { 13,1 },{ 14,1 },{ 15,1 },{ 16,1 },{ 17,1 },{18,1},},--
    								exit = { { 16,30 },{ 17,30 }, },--
    							}, 							
  						}, 
  						
    				[4]={ 			--第一关
    						 					--第一组出口和入口
    						[1]={
    								entry = { { 30,11 },{ 30,12 },{ 30,13 },{ 30,14 },{ 30,15 },{ 30,16},},--
    								exit = { { 1,15 },{ 1,16 }, },--
    							},
     						[2]={
    								entry = { { 13,1 },{ 14,1 },{ 15,1 },{ 16,1 },{ 17,1 },{ 18,1},},--
    								exit = { { 16,30 },{ 17,30 }, },--
    							},   							
  						},   
 						  						
    					    					
    			};
	--出口如果设为32*32的规模，则需要在出口向地图内部推进一格，这格里不能放武器，怪物能走
    TD_ExpMapPath = {
    					[1]={
    							{ 2,5 },{ 2,6 },{ 2,7 },{ 2,8 },{ 2,9 },{2,10},{ 29,7 },{ 29,8 },
    						},
    				
    					[2]={ 
    							{ 2,11 },{ 2,12 },{ 2,13 },{ 2,14 },{ 2,15 },{2,16},{ 29,9 },{ 29,10 },{ 29,17 },{ 29,18 },
		   					},    					

    					[3]={
    							{ 2,11 },{ 2,12 },{ 2,13 },{ 2,14 },{ 2,15 },{2,16},{2,17},{ 29,15 },{ 29,16 },{ 29,17},
    							{ 13,2 },{ 14,2 },{ 15,2 },{ 16,2 },{ 17,2 },{18,2},{ 16,29 },{ 17,29 },
    						},

    					[4]={ 
    							{ 29,11 },{ 29,12 },{ 29,13 },{ 29,14 },{ 29,15 },{30,16},{ 2,15 },{ 2,16 },
    							{ 13,2 },{ 14,2 },{ 15,2 },{ 16,2 },{ 17,2 },{ 18,2},{ 16,29 },{ 17,29 },
    							
		   					}, 
    					
    			};

						
	

	--攻击范围的圆圈

	TD_WRangeCircle={	[1]={timeperfra=100, franum=3, h=48, w=48 },--48/16=3，16为单元格的大小 结果3对应 wradius=2
                    	[2]={timeperfra=100, franum=3, h=64, w=64 },--64/16=4
                    	[3]={timeperfra=100, franum=3, h=80, w=80 },--80/16=5
                       	[4]={timeperfra=100, franum=3, h=96, w=96 },--96/16=6
                    	[5]={timeperfra=100, franum=3, h=112, w=112 },--112/16=7
                    	[6]={timeperfra=100, franum=3, h=130, w=130 },--130/16=8
	
					 };		
	--wradius的方形对应圆圈后，不应该计入的块
	TD_CircleRange = {
						[1] = { 
										--以bpos[1]为零点,即武器中心左上方块为{0,0} 其中{-1,-1}表示对{0,0}的偏移量
									{-2,-2},{-2,3},{3,-2},{3,3},
							},


						[2] = { 
									{-3,-3},{-3,-2},{-3,3},{-3,4},
									{-2,-3},{-2,4},
									{3,-3},{3,4},
									{4,-3},{4,-2},{4,3},{4,4},
--									[-3] = { [-3]=true, [-2]=true, [3]=true, [4]=true, },[-2] = { [-3]=true, [4] =true, },[3]  = { [-3]=true, [4] =true, },[4]  = { [-3]=true, [-2]=true, [3]=true, [4]=true, },
		
							 },

						[3] = { 
									{-4,-4},{-4,-3},{-4,4},{-4,5},
									{-3,-4},{-3,5},
									{4,-4},{4,5},
									{5,-4},{5,-3},{5,4},{5,5},
									
--									[-4] = { [-4]=true, [-3]=true, [4]=true, [5]=true, },[-3] = { [-4]=true, [5] =true, },[4]  = { [-4]=true, [5] =true },[5]  = { [-4]=true, [-3]=true, [4]=true, [5]=true, },
							 },

						[4] = { 
								{-5,-5},{-5,-4},{-5,-3},{-5,4},{-5,5},{-5,6},
								{-4,-5},{-4,-4},{-4,5},{-4,6},
								{-3,-5},{-3,6},
								{6,-5},{6,-4},{6,-3},{6,4},{6,5},{6,6},
								{5,-5},{5,-4},{5,5},{5,6},
								{4,-5},{4,6},								
							
--								[-5] = { [-5]=true, [-4]=true, [-3]=true, [4]=true, [5]=true, [6]=true, },[-4] = { [-5]=true, [-4]=true, [5] =true, [6]=true, },[-3] = { [-5]=true, [6] =true, },
--								[4]  = { [-5]=true, [6] =true, },[5]  = { [-5]=true, [-4]=true, [5] =true, [6]=true, },[6]  = { [-5]=true, [-4]=true, [-3]=true, [4]=true, [5]=true, [6]=true, },
							 },

						[5] = { 
								{-6,-6},{-6,-5},{-6,-4},{-6,-3},{-6,4},{-6,5},{-6,6},{-6,7},
								{-5,-6},{-5,-5},{-5,6},{-5,7},
								{-4,-6},{-4,7},
								{-3,-6},{-3,7},
								{7,-6},{7,-5},{7,-4},{7,-3},{7,4},{7,5},{7,6},{7,7},
								{6,-6},{6,-5},{6,6},{6,7},
								{5,-6},{5,7},
								{4,-6},{4,7},								
--								[-6] = { [-6]=true, [-5]=true, [-4]=true, [-3]=true, [4]=true, [5]=true, [6]=true, [7]=true, },7[-5] = { [-6]=true, [-5]=true, [6] =true, [7] = true, },7[-4] = { [-6]=true, [7] =true, },7[-3] = { [-6]=true, [7] =true, },
--								[4]  = { [-6]=true, [7] =true, },7[5]  = { [-6]=true, [7] =true, },7[6]  = { [-6]=true, [-5]=true, [6] =true, [7] = true, },7[7]  = { [-6]=true, [-5]=true, [-4]=true, [-3]=true, [4]=true, [5]=true, [6]=true, [7]=true, },
						
							 },
						[6] = {
								{-7,-7},{-7,-6},{-7,-5},{-7,-4},{-7,-3},{-6,-7},{-5,-7},{-4,-7},{-3,-7},{-6,-6},{-6,-5},{-5,-6},
								{-7,8},{-7,7},{-7,6},{-7,5},{-7,4},{-6,8},{-5,8},{-4,8},{-3,8},{-6,7},{-6,6},{-5,7},
								{8,-7},{8,-6},{8,-5},{8,-4},{8,-3},{7,-7},{6,-7},{5,-7},{4,-7},{5,-6},{7,-5},{6,-6},
								{8,8},{8,7},{8,6},{8,5},{8,4},{7,8},{6,8},{5,8},{4,8},{7,7},{7,6},{6,7},
							 },
					};							
						

-- bullet index -------------------------------------------------------------------------------------------------------------------------

				--子弹的控件,TD_BulletIndex[5]对应了界面上"Pic_Bullet5"的子弹，注意名称要对上，如果子弹picname为nil，则不需要建立对应的控件
				
	TD_BulletIndex = {
						[1] = {picname=nil,franum=nil,timeperfra=nil,explodetype=3},
						[2] = {picname=nil,franum=nil,timeperfra=nil,explodetype=4},
						[3] = {picname=nil,franum=nil,timeperfra=nil,explodetype=5},							
						[4] = {picname="TOWERDEFENSE\\bulletpic\\天音子弹效果.tga",franum=3,timeperfra=100,explodetype=6,w=6,h=6},
						[5] = {picname="TOWERDEFENSE\\bulletpic\\鬼道子弹效果.tga",franum=3,timeperfra=100,explodetype=7,w=6,h=5},

					};

	
  
--					
------攻击效果的特效-----------------------------------------

	TD_AttackEffectIndex = {
								[1]={picname="TOWERDEFENSE\\attack.tga",frames=8,direnum=8,franum=1,timeperfra=100}	
	
							};
--
	--被攻击的效果

	TD_BeAtkEfeIndex = {
							Freeze={ picname="TOWERDEFENSE\\bulletpic\\冰系攻击附加特效2.tga", franum=3,timeperfra=200,w=12,h=12 },
							AttackToAD={ picname="TOWERDEFENSE\\bulletpic\\毒雾腐蚀特效.tga", franum=3,timeperfra=200,w=12,h=12 },
							AttackToBD={ picname="TOWERDEFENSE\\bulletpic\\毒雾腐蚀特效.tga", franum=3,timeperfra=200,w=12,h=12 }
						};
--
	--爆炸（包含死亡)以及升级等原地播放效果

	TD_RTEfeIndex={
						[1] = {picname = "TOWERDEFENSE\\weaponpic\\升级光效.tga",franum=4, timeperfra=150,w=16,h=26 },
						[2]={ picname = "TOWERDEFENSE\\monsterpics\\死亡爆炸.tga", franum=5,timeperfra=150,w=16, h=16 },
						
						[3] = {picname = "TOWERDEFENSE\\bulletpic\\鬼王击中效果.tga",franum=3,timeperfra=150,w=14,h=10,sound="Sfx\\TinyGame\\TD\\鬼王宗击中.wav" },
						[4] = {picname = "TOWERDEFENSE\\bulletpic\\青云击中效果.tga",franum=3,timeperfra=150,w=15,h=20,sound="Sfx\\TinyGame\\TD\\青云门击中.wav" },
						[5] = {picname = "TOWERDEFENSE\\bulletpic\\合欢击中效果.tga",franum=3,timeperfra=150,w=21,h=3,sound="Sfx\\TinyGame\\TD\\合欢派击中.wav" },
						[6] = {picname = "TOWERDEFENSE\\bulletpic\\天音击中效果.tga",franum=3,timeperfra=150,w=15,h=14,sound="Sfx\\TinyGame\\TD\\天音寺击中.wav" },
						[7] = {picname = "TOWERDEFENSE\\bulletpic\\鬼道击中效果.tga",franum=3,timeperfra=150,w=17,h=16,sound="Sfx\\TinyGame\\TD\\鬼道击中.wav" },

						
						Mult2 = {picname="TOWERDEFENSE\\bulletpic\\暴击数字2.tga",franum=3,timeperfra=200,h=23},
						Mult3 = {picname="TOWERDEFENSE\\bulletpic\\暴击数字3.tga",franum=3,timeperfra=200,h=23},
						Mult4 = {picname="TOWERDEFENSE\\bulletpic\\暴击数字4.tga",franum=3,timeperfra=200,h=23},
						Mult5 = {picname="TOWERDEFENSE\\bulletpic\\暴击数字5.tga",franum=3,timeperfra=200,h=23},
						Mult6 = {picname="TOWERDEFENSE\\bulletpic\\暴击数字6.tga",franum=3,timeperfra=200,h=23},
				
					};
					
	


	---------其他变量
	TD_FreezeMax=0.5;	--减速的最大幅度（比例）

	TD_FreezeStopMin=0.1;	--产生冰冻停住的最低概率（2倍攻击）
	TD_FreezeStopStep=0.06;
	
	-----------------控件的大小升级进度条，w为宽度1/2，h为高度

	TD_WImpSize = { h=4; w=16 };


	TD_HelpWPic = {
						{"TOWERDEFENSE\\extra\\TD_鬼王1.tga","TOWERDEFENSE\\TextPic\\鬼王宗.tga"},
						{"TOWERDEFENSE\\extra\\TD_青云1.tga","TOWERDEFENSE\\TextPic\\青云门.tga"},
						{"TOWERDEFENSE\\extra\\TD_合欢1.tga","TOWERDEFENSE\\TextPic\\合欢派.tga"},
						{"TOWERDEFENSE\\extra\\TD_天音1.tga","TOWERDEFENSE\\TextPic\\天音寺.tga"},
						{"TOWERDEFENSE\\extra\\TD_鬼道1.tga","TOWERDEFENSE\\TextPic\\鬼道.tga"},
						{"TOWERDEFENSE\\extra\\TD_仙石1.tga","TOWERDEFENSE\\TextPic\\仙石.tga"},
						
					};


--============================================================================================================
---关于其他控件的名字---------------------------------
	TD_ItemSell="Btn_Wsell";--卖按钮

	TD_ItemImprove="Btn_Wimprove";--升级按钮
	TD_PicWDrag="Pic_WeaponDrag";--武器放置位置的阴影

	
	TD_MlifePro = "Pro_Mlife";	
	TD_WimprovePro = "Pro_Wimprove";
		
		--死亡效果
	TD_RTEfePre = "Pic_RTeffect";

	--被攻击的效果
	TD_BeAtkEfePre = "Pic_BeAttack";

	--攻击效果的特效

	TD_AttackEffectPre = "Pic_Attack";

				--子弹的控件

	TD_BulletPrefix="Pic_Bullet";

	---怪物
	TD_MonsterPrefix="Pic_Monster";
	
	--攻击范围
	TD_WeaponRangePre="Pic_WRange";	


	--武器的属性列表

	TD_WeaponButtonPrefix="Btn_Weapon";
	TD_WeaponPrefix="Pic_Weapon";


-----------------------------------------------------
-----------------------------------------------------
--约定常量
TD_PauseState = 0;
TD_PlayingState = 1;
TD_WaitforState=2;

-----------------------------------------------------
-----------------------------------------------------
--Text

TD_TextCurTower = "武器当前属性";
TD_TextCurTowerValue = "购买价格";
TD_TextCurTowerA = "仙攻";
TD_TextCurTowerB = "魔攻";
TD_TextCurTowerToA = "仙甲虚弱";
TD_TextCurTowerToB = "魔甲虚弱";
TD_TextCurTowerSpeL = "减速";
TD_TextCurTowerAttFre= "攻击频率";
TD_TextCurTowerRadius = "攻击半径";
TD_TextCurTowerMulPro = "暴击概率";
TD_TextCurTowerSellM = "售出金钱";
TD_TextCurTowerImprM = "升级金钱";

TD_TextCurMonsterPic = "当前怪物"
TD_TextCurMonster = "怪物属性";
TD_TextCurMonsterLife = "生命";
TD_TextCurMonsterSped= "速度";
TD_TextCurMonsterA = "仙甲";
TD_TextCurMonsterB = "魔甲";
TD_TextCurMonsterM = "携带金钱";

TD_TextAssiAtt = "攻击辅助";
TD_TextAssiEro = "腐蚀辅助";
TD_TextAssiSlo = "减速辅助";
TD_TextAssiMulp = "倍击率提升";
TD_TextAssiMulh = "倍击提升";
TD_TextAssiSellM = "售出金钱";

TD_TextWtypeTitle = "武器类别";
TD_TextWAttAreaBoth = "对地 对空";
TD_TextSecond = "秒";
TD_TextGrid = "格";
TD_TextLevel = "重";
TD_TextWAttGround = "对地";
TD_TextWAttAir = "对空";
TD_TextAssistFunction = "辅助功能";
TD_TextTimes = "倍";
TD_TextGridPerSec = "格/秒";


TD_TextHelpRule = "        ^fff899诛仙防守游戏是一个简单有趣的防守游戏，在游戏中，玩家需要通过合理利用五大门派的弟子，充分利用地形，抵御魔化怪物的到达终点，守住一定的波数就是胜利进入下一关的挑战。（选择普通难度以上游戏还能得到丰厚的奖励哦。）\r\r玩法：\r        在游戏开始的时候玩家会得到一笔初始金钱，用来雇用不同门派的弟子。在游戏中每隔一段时间会在各个入口跑出大批的怪物，玩家需要利用地形，计算生产不同门派弟子的费用去消灭跑向出口的怪物，每个逃跑的怪物会影响玩家的生命值，成功守住一波进攻再过一段时间下一波就会进攻，你需要考虑用杀死怪物得到的金钱升级目前已有单位或者生产新单位，从而加强阻止怪物通过的能力。\r        每个门派弟子都可以升级到五重，不同的门派弟子之间的作用也不大相同，通过合理利用不同门派的特性成功防御怪物的进攻。\r";
TD_TextHelpWeapon = {
						"^fff899鬼王宗说明:\r        单体攻击职业，升级可以攻击空中和地面的怪物，有最快的攻击速度和不错的暴击，升级到5重能力会有飞跃。",
						"^fff899青云门说明:\r        群体攻击职业，升级到3级可以攻击空中和地面的怪物，攻击力不是很出众，适合对付大群敌人。",
						"^fff899合欢派说明:\r        单体攻击职业，攻击地面怪物，不能对空，有着最高的暴击率和暴击伤害。",
						"^fff899天音寺说明:\r        群体攻击职业，升级到3级可以攻击空中和地面的怪物，攻击并不出色，但是攻击附带群体减速。",
						"^fff899鬼道说明:\r        单体攻击职业，可以攻击空中和地面的怪物，有较远的射程，攻击附带腐蚀护甲的能力。5重可以群体腐蚀护甲。",
						"^fff899仙石说明:\r        落入凡间的奇异石块，可以提升周围各门派人物能力。",

					}

TD_MapInfo = {
				"^fff899第一关:\r        青云试练：20波怪物，1个出口，起始金钱较多，比较简单。",	--第一关地图缩略图说明
				"^fff899第二关:\r        流波岛：30波怪物，1个出口，中等难度。",
				"^fff899第三关:\r        蛮荒之地：40波怪物，2个出口，比较困难。",	--第一关地图缩略图说明
				"^fff899第四关:\r        南疆之地：50波怪物，2个出口，困难。",				
			};
---------------------------------------------------------------------------------------------------------------


