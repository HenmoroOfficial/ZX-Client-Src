---------------------------------------------------------------
--  created:   2010.2.23
--  author:    zt
--
--  成就系统配置文件。
---------------------------------------------------------------
local STRING_ERROR  = "error"
local Format        = string.format
local GameAI       = GameAI
-- GameAI.GetMonsterName(tid)        --获取怪物名字。
-- GameAI.GetItemName(tid)           --获取物品名字（通过背包）。
-- GameAI.GetTaskName(id)            --获取任务名字。
-- GameAI.GetBattleName(tid)         --获取战场名字。
-- GameAI.GetReputationName(id)      --获取声望名字。
-- GameAI.GetSkillName(id)           --获取技能名字。
-- GameAI.GetProduceName(tid,index)  --获取生产物品名字，传入参数：配方id，第几个物品,index:zerobased。
-- GameAI.GetMoneyBagTypeName(id)    --获取跑商交易币名字。
-- GameAI.GetProfName(id)            --获取职业名字。
-- GameAI.GetAchievementName(id)     --获取成就名字。
-- GameAI.GetAchCondDataInt(p)       --获取成就中某个条件的整数数据，参数p: 条件数据(userdata)。

AchievementCfg              = {}

function AchievementCfg:GetSelf()
    return self
end

--获取成就某个条件的描述文本
--idAchivement  :成就id;
--iCondition    :成就条件的索引（第几个条件，1 based）
--bFinish       :是否已经完成，
--p             :条件数据(userdata)
function AchievementCfg:GetCondDesc(idAchievement,iCondition,bFinish,p)
    local condition =   idAchievement and
                        iCondition and
                        self.achievement and
                        self.achievement[idAchievement] and
                        self.achievement[idAchievement].conds and
                        self.achievement[idAchievement].conds[iCondition]
    if not condition then
        return STRING_ERROR
    end
    local condsCommon = condition.id and self.conds and self.conds[condition.id]
    if not condsCommon then
        return STRING_ERROR
    end
    local descFormat =  condition.descFormat or condsCommon.descFormat
    if not descFormat then
        return STRING_ERROR
    end
    local funcDesc = condition.funcDesc or condsCommon.funcDesc
    if not funcDesc then
        return STRING_ERROR
    end
    return funcDesc(descFormat, condition.params or {}, bFinish, p) or STRING_ERROR
end

--获取成就某个条件的详细信息
--idAchivement  :成就id;
--iCondition    :成就条件的索引（第几个条件，1 based）
--bFinish       :是否已经完成，
--p             :条件数据(userdata)
function AchievementCfg:GetCondInfo(idAchievement,iCondition,bFinish,p)
    local condition =   idAchievement and
                        iCondition and
                        self.achievement and
                        self.achievement[idAchievement] and
                        self.achievement[idAchievement].conds and
                        self.achievement[idAchievement].conds[iCondition]
    if not condition then
        return STRING_ERROR
    end
    local condsCommon = condition.id and self.conds and self.conds[condition.id]
    if not condsCommon then
        return STRING_ERROR
    end
    local infoFormat =  condition.infoFormat or condsCommon.infoFormat
    if not infoFormat then
        return STRING_ERROR
    end
    local funcInfo = condition.funcInfo or condsCommon.funcInfo
    if not funcInfo then
        return STRING_ERROR
    end
    return funcInfo(infoFormat, condition.params or {}, bFinish, p) or STRING_ERROR
end

--获取成就进度
--idAchivement  :成就id;
--iCondition    :成就条件的索引（第几个条件，1 based）
--bFinish       :是否已经完成，
--p             :条件数据(userdata)
function AchievementCfg:GetCondProg(idAchievement,iCondition,bFinish,p)
    local condition =   idAchievement and
                        iCondition and
                        self.achievement and
                        self.achievement[idAchievement] and
                        self.achievement[idAchievement].conds and
                        self.achievement[idAchievement].conds[iCondition]
    if not condition then
        return nil
    end
    local condsCommon = condition.id and self.conds and self.conds[condition.id]
    if not condsCommon then
        return nil
    end

    local funcProg = condition.funcProg or condsCommon.funcProg
    if not funcProg then
        return nil
    end
    return funcProg(condition.params or {}, bFinish, p)
end


--获取飞升状态（0-不显示、1-显示为飞升）
function AchievementCfg:GetRebornDesc(id)
	if id == 0 then
	return ""
	elseif id ==1 then
	return "飞升"
	end
end

--获取炼器结果（成功、失败）
function AchievementCfg:GetRefineDesc(id)
	if id == 1 then
	return "成功"
	else
	return "失败"
	end
end

--成就分类
AchievementCfg.class =
{
    [1] = 
    {
        name= "成长",
        
         [1] =
         {
         	name ="等级",
         },
         [2] =
         {
         	name ="财富",
         },
         [3] =
         {
         	name ="声望",
         },
         [4] =
         {
         	name ="门贡",
         },
      },

    [2] = 
    {
        name= "战斗",
         [1] = 
         {
         	name ="PVE",
         },
         [2] =
         {
         	name ="PVP",
         },
         [3] = 
         {
         	name ="战场",
         },
         [4] =
         {
         	name ="世界Boss",
         },
         [5] =
         {
         	name ="混沌大战",
         },
         [6] =
         {
         	name ="天空之城",
         },
      },

    [3] = 
    {
        name= "游历",
         [1] =
         {
         	name ="普通任务",
         },
         [2] =
         {
         	name ="人族声望",
         },
         [3] =
         {
         	name ="神裔声望",
         },
         [4] =
         {
         	name ="活跃度",
         },
      },

    [4] = 
    {
        name= "物品",
         [1] =
         {
         	name ="时装",
         },
         [2] =
         {
         	name ="饰品",
         },
         [3] =
         {
         	name ="锦囊",
         },
         [4] =
         {
         	name ="罡气",
         },
         [5] =
         {
         	name ="元魂",
         },
         [6] =
         {
         	name ="幻灵石",
         },
      },

    [5] = 
    {
        name= "生产",
         [1] =
         {
         	name ="装备炼器",
         },
         [2] =
         {
         	name ="装备血祭",
         },
         [3] =
         {
         	name ="个人生产",
         },
         [4] =
         {
         	name ="宠物生产",
         },
      },

    [6] = 
    {
        name= "特殊",

    },
}

--成就的条件
AchievementCfg.conds=
{
    --人物等级条件：职业，级别
    [1]   =
    {
        descFormat = "%s等级达到 %d (%d/1)", 
        infoFormat = "%s等级达到 %d",
        funcDesc = function(strFormat,params,bFinish,p)
            local prof  = params[1] and AchievementCfg:GetRebornDesc(params[1]) or STRING_ERROR
            local level = params[2] or -1
            local iFinish = bFinish and 1 or 0
            return Format(strFormat,prof,level,iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local prof  = params[1] and AchievementCfg:GetRebornDesc(params[1]) or STRING_ERROR
            local level = params[2] or -1
            return Format(strFormat,prof,level)
        end,
		funcProg = function(params,bFinish,p)
			local iFinish = bFinish and 1 or 0
            return iFinish,1
		end,
    },

    --钱数达到：钱的数量
    [2]   =
    {
        descFormat = "拥有金钱 %d (%d/1)",
        infoFormat = "拥有金钱 %d",
        funcDesc = function(strFormat,params,bFinish,p)
            local iFinish = bFinish and 1 or 0
            local par = params[1] * 0.0001
            return Format(strFormat, par or -1, iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local par = params[1] * 0.0001
            return Format(strFormat, par or -1)
        end,
		funcProg = function(params,bFinish,p)
			local iFinish = bFinish and 1 or 0
            return iFinish,1
		end,
    },
    
    --声望达到：声望id，声望值
    [3]   =
    {
        descFormat = "%s 达到 %d (%d/1)",
        infoFormat = "%s 达到 %d (%d/1)",
        funcDesc = function(strFormat,params,bFinish,p)
            local name  = params[1] and GameAI.GetReputationName(params[1]) or STRING_ERROR
            local num   = params[2] or -1
            local iFinish = bFinish and 1 or 0
            return Format(strFormat, name, num, iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local name  = params[1] and GameAI.GetReputationName(params[1]) or STRING_ERROR
            local num   = params[2] or -1
            return Format(strFormat, name, num)
        end,
		funcProg = function(params,bFinish,p)
			local iFinish = bFinish and 1 or 0
            return iFinish,1
		end,
    },
    
    --杀怪：怪物id
    [4]   =
    {
        descFormat = "击杀 %s (%d/1)",
        infoFormat = "击杀 %s",
        funcDesc = function(strFormat,params,bFinish,p)
            local name  = params[1] and GameAI.GetMonsterName(params[1]) or STRING_ERROR
            local iFinish = bFinish and 1 or 0
            return Format(strFormat,name,iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local name  = params[1] and GameAI.GetMonsterName(params[1]) or STRING_ERROR
            return Format(strFormat,name)
        end,
		funcProg = function(params,bFinish,p)
			local iFinish = bFinish and 1 or 0
            return iFinish,1
		end,
    },
    
    --善恶值达到：善恶值
    [5]   =
    {
        descFormat = "善恶值达到 %d (%d/1)",
        infoFormat = "善恶值达到 %d",
        funcDesc = function(strFormat,params,bFinish,p)
            local iFinish = bFinish and 1 or 0
            return Format(strFormat, params[1] or -1, iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            return Format(strFormat, params[1] or -1)
        end,
		funcProg = function(params,bFinish,p)
			local iFinish = bFinish and 1 or 0
            return iFinish,1
		end,
    },
    
    --完成任务：任务id
    [6]   =
    {
        descFormat = "完成 %s (%d/1)",
        infoFormat = "完成 %s",
        funcDesc = function(strFormat,params,bFinish,p)
            local name  = params[1] and GameAI.GetTaskName(params[1]) or STRING_ERROR
            local iFinish = bFinish and 1 or 0
            return Format(strFormat, name, iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local name  = params[1] and GameAI.GetTaskName(params[1]) or STRING_ERROR
            return Format(strFormat, name)
        end,
		funcProg = function(params,bFinish,p)
			local iFinish = bFinish and 1 or 0
            return iFinish,1
		end,
    },
    
      --精炼：装备级别，精炼级别
    [7]   =
    {
        descFormat = "炼器+%d %s (%d/1)",
        infoFormat = "炼器+%d %s ",
        funcDesc = function(strFormat,params,bFinish,p)
            local par1 = params[2] - params[1]
            local par2 = params[1] + 1
            local prof  = par1 and AchievementCfg:GetRefineDesc(par1) or STRING_ERROR
            local iFinish = bFinish and 1 or 0
            return Format(strFormat, par2, prof, iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local par1 = params[2] - params[1]
            local par2 = params[1] + 1            
            local prof  = par1 and AchievementCfg:GetRefineDesc(par1) or STRING_ERROR
            local iFinish = bFinish and 1 or 0
            return Format(strFormat, par2, prof)
        end,
		funcProg = function(params,bFinish,p)
			local iFinish = bFinish and 1 or 0
            return iFinish,1
		end,
    },
    
        ---宠物星等达到
    [8]   =
    {
        descFormat = "宠物星等 %d (%d/1)",
        infoFormat = "宠物星等 %d",
        funcDesc = function(strFormat,params,bFinish,p)
            local iFinish = bFinish and 1 or 0
            return Format(strFormat, params[1] or -1, iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            return Format(strFormat, params[1] or -1)
        end,
		funcProg = function(params,bFinish,p)
			local iFinish = bFinish and 1 or 0
            return iFinish,1
		end,
    },
    
    ---法宝血炼值达到
    [9]   =
    {
        descFormat = "血炼值 %d (%d/1)",
        infoFormat = "血炼值 %d",
        funcDesc = function(strFormat,params,bFinish,p)
            local iFinish = bFinish and 1 or 0
            return Format(strFormat, params[1] or -1, iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            return Format(strFormat, params[1] or -1)
        end,
		funcProg = function(params,bFinish,p)
			local iFinish = bFinish and 1 or 0
            return iFinish,1
		end,
    },
    
    --生产：配方id，第几个物品
    [10]   =
    {
        descFormat = "生产 %s (%d/1)",
        infoFormat = "生产 %s",
        funcDesc = function(strFormat,params,bFinish,p)
            local name  = params[2] and GameAI.GetItemName(params[2]) or STRING_ERROR
            local iFinish = bFinish and 1 or 0
            return Format(strFormat,name,iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local name  = params[2] and GameAI.GetItemName(params[2]) or STRING_ERROR
            return Format(strFormat,name)
		end,
		funcProg = function(params,bFinish,p)
			local iFinish = bFinish and 1 or 0
            return iFinish,1
		end,
    },
    
     --装备物品：物品id
    [11]   =
    {
        descFormat = " %s (%d/1)",
        infoFormat = " %s",
        funcDesc = function(strFormat,params,bFinish,p)
            local name  = params[1] and GameAI.GetItemName(params[1]) or STRING_ERROR
            local iFinish = bFinish and 1 or 0
            return Format(strFormat,name,iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local name  = params[1] and GameAI.GetItemName(params[1]) or STRING_ERROR
            return Format(strFormat,name)
        end,
		funcProg = function(params,bFinish,p)
			local iFinish = bFinish and 1 or 0
            return iFinish,1
		end,
    },
    
     --完成成就：成就id
    [12]   =
    {
        descFormat = "达成 %s (%d/1)",
        infoFormat = "达成 %s",
        funcDesc = function(strFormat,params,bFinish,p)
            local name  = params[1] and GameAI.GetAchievementName(params[1]) or STRING_ERROR
            local iFinish = bFinish and 1 or 0
            return Format(strFormat, name, iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local name  = params[1] and GameAI.GetAchievementName(params[1]) or STRING_ERROR
            return Format(strFormat, name)
        end,
		funcProg = function(params,bFinish,p)
			local iFinish = bFinish and 1 or 0
            return iFinish,1
		end,
    },

    --完成任务次数：任务id，次数
    [1000]   =
    {
        descFormat = "完成 %s (%d/%d)次",
        infoFormat = "完成 %s (%d/%d)次",
        size=2,
        funcDesc = function(strFormat,params,bFinish,p)
            local name    = params[1] and GameAI.GetTaskName(params[1]) or STRING_ERROR
            local nFinish = bFinish and params[2] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[2] or -1
            return Format(strFormat,name,nFinish,num)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local name    = params[1] and GameAI.GetTaskName(params[1]) or STRING_ERROR
            local nFinish = bFinish and params[2] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[2] or -1
            return Format(strFormat,name,nFinish,num)
        end,
		funcProg = function(params,bFinish,p)
			local nFinish = bFinish and params[2] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[2] or -1
            return nFinish,num
		end,
    },
    
    --杀怪次数：怪物id，次数
    [1001]   =
    {
        descFormat = "击杀 %s (%d/%d)次",
        infoFormat = "击杀 %s (%d/%d)次",
        size=2,
        funcDesc = function(strFormat,params,bFinish,p)
            local name    = params[1] and GameAI.GetMonsterName(params[1]) or STRING_ERROR
            local nFinish = bFinish and params[2] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[2] or -1
            return Format(strFormat,name,nFinish,num)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local name    = params[1] and GameAI.GetMonsterName(params[1]) or STRING_ERROR
            local nFinish = bFinish and params[2] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[2] or -1
            return Format(strFormat,name,nFinish,num)
        end,
		funcProg = function(params,bFinish,p)
			local nFinish = bFinish and params[2] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[2] or -1
            return nFinish,num
		end,
    },

    --生产配方次数：配方id，第几个物品，次数
    [1002]   =
    {
        descFormat = "生产 %s (%d/%d)",
        infoFormat = "生产 %s (%d/%d)",
        size=2,
        funcDesc = function(strFormat,params,bFinish,p)
            local name    = params[2] and GameAI.GetItemName(params[2]) or STRING_ERROR
            local nFinish = bFinish and params[3] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[3] or -1
            return Format(strFormat,name,nFinish,num)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local name    = params[2] and GameAI.GetItemName(params[2]) or STRING_ERROR
            local nFinish = bFinish and params[3] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[3] or -1
            return Format(strFormat,name,nFinish,num)
        end,
		funcProg = function(params,bFinish,p)
			local nFinish = bFinish and params[2] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[2] or -1
            return nFinish,num
		end,
    },

    --参加战场次数：战场id，次数
    [1003]   =
    {
        descFormat = "参加 %s (%d/%d)",
        infoFormat = "参加 %s (%d/%d)",
        size=2,
        funcDesc = function(strFormat,params,bFinish,p)
            local name    = params[1] and GameAI.GetBattleName(params[1]) or STRING_ERROR
            local nFinish = bFinish and params[2] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[2] or -1
            return Format(strFormat,name,nFinish,num)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local name    = params[1] and GameAI.GetBattleName(params[1]) or STRING_ERROR
            local nFinish = bFinish and params[2] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[2] or -1
            return Format(strFormat,name,nFinish,num)
        end,
		funcProg = function(params,bFinish,p)
			local nFinish = bFinish and params[2] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[2] or -1
            return nFinish,num
		end,
    },
  
    --死亡次数：次数
    [1004]   =
    {
        descFormat = "死亡 (%d/%d)",
        infoFormat = "死亡 (%d/%d)",
        size=2,
        funcDesc = function(strFormat,params,bFinish,p)
            local nFinish = bFinish and params[1] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[1] or -1
            return Format(strFormat,nFinish,num)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local nFinish = bFinish and params[1] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[1] or -1
            return Format(strFormat,nFinish,num)
        end,
		funcProg = function(params,bFinish,p)
			local nFinish = bFinish and params[2] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[2] or -1
            return nFinish,num
		end,
    },
    
    [1005]   =
    {
        descFormat = "炼器+%d 成功 (%d/%d)次",
        infoFormat = "炼器+%d 成功 (%d/%d)次",
        size=2,
        funcDesc = function(strFormat,params,bFinish,p)
       		  local par1 = params[2]
            local nFinish = bFinish and params[3] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[3] or -1
						return Format(strFormat, par1, nFinish, num)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
       		  local par1 = params[2]
            local nFinish = bFinish and params[3] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[3] or -1
						return Format(strFormat, par1, nFinish, num)
        end,
		funcProg = function(params,bFinish,p)
			local nFinish = bFinish and params[2] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[2] or -1
            return nFinish,num
		end,
    },
    --活跃度达到100
    [16]   =
    {
        descFormat = "活跃度达到100 (%d/1)",
        infoFormat = "活跃度达到100 ",
		funcDesc = function(strFormat,params,bFinish,p)
            local iFinish = bFinish and 1 or 0
            return Format(strFormat, iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            return Format(strFormat)
        end,
		funcProg = function(params,bFinish,p)
			local iFinish = bFinish and 1 or 0
            return iFinish,1
		end,
    },
    
    --连续七天活跃度达到100次数：次数
    [1006]   =
    {
        descFormat = "连续七天活跃度达到100 (%d/%d)",
        infoFormat = "连续七天活跃度达到100 (%d/%d)",
        funcDesc = function(strFormat,params,bFinish,p)
            local nFinish = bFinish and params[1] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[1] or -1
            return Format(strFormat,nFinish,num)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
            local nFinish = bFinish and params[1] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[1] or -1
            return Format(strFormat,nFinish,num)
        end,
		funcProg = function(params,bFinish,p)
			local nFinish = bFinish and params[2] or GameAI.GetAchCondDataInt(p) or -1
            local num     = params[2] or -1
            return nFinish,num
		end,
    },
    
        -- 流水席战场击杀玩家 int(1:连杀,2:多杀) int(killcnt)
    [17]   =
    {
        descFormat = "%s%d人 (%d/1)",
        infoFormat = "%s%d人 ",
	      funcDesc = function(strFormat,params,bFinish,p)
	      		local iType
	      		if params[1]==2 then
	      			iType = "短时间内连续斩杀"
	      		else
	      			iType = "连杀"
	      		end
	      		local num = params[2] or -1
            local iFinish = bFinish and 1 or 0
            return Format(strFormat, iType, num, iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
	      		local iType
	      		if params[1]==2 then
	      			iType = "短时间内连续斩杀"
	      		else
	      			iType = "连杀"
	      		end
	      		local num = params[2] or -1
            return Format(strFormat,iType,num)
        end,
    },

    -- 流水席战场完成 int(1:只杀人,2:只杀怪,3:一次不死)
    [18]   =
    {
        descFormat = "%s达成目标 (%d/1)",
        infoFormat = "%s达成目标 ",
	      funcDesc = function(strFormat,params,bFinish,p)
	      		local iType
	      		if params[1]==1 then
	      			iType = "只通过杀玩家"
	      		elseif params[1]==2 then
	      			iType = "只通过杀怪物"
	      		elseif params[1]==3 then
	      			iType = "一次不死"
	      		end
            local iFinish = bFinish and 1 or 0
            return Format(strFormat, iType, iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
	      		local iType
	      		if params[1]==1 then
	      			iType = "只通过杀玩家"
	      		elseif params[1]==2 then
	      			iType = "只通过杀怪物"
	      		elseif params[1]==3 then
	      			iType = "一次不死"
	      		end
            return Format(strFormat, iType)
        end,
    },
    -- 流水席战场终结多杀 int(人数)
    [19]   =
    {
        descFormat = "终结一个连杀数在%d以上的玩家 (%d/1)",
        infoFormat = "终结一个连杀数在%d以上的玩家 ",
	      funcDesc = function(strFormat,params,bFinish,p)
	      		local num = params[1] or -1
            local iFinish = bFinish and 1 or 0
            return Format(strFormat, num, iFinish)
        end,
        funcInfo = function(strFormat,params,bFinish,p)
	      		local num = params[1] or -1
            return Format(strFormat, num)
        end,
    },
}

--成就列表
AchievementCfg.achievement=
{
    msg = "%s 完成了成就 %s",

    [1]=
    {
        name        = "^00ff00懵懂稚子^00ff00",
        desc        = "        未飞升 人物等级达到10级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00懵懂稚子^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,10,},
--            descFormat="0,10,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [2]=
    {
        name        = "^00ff00一知半解^00ff00",
        desc        = "        未飞升 人物等级达到20级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00一知半解^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,20,},
--            descFormat="0,20,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [3]=
    {
        name        = "^00ff00略窥门径^00ff00",
        desc        = "        未飞升 人物等级达到30级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00略窥门径^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,30,},
--            descFormat="0,30,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [4]=
    {
        name        = "^00ff00初出茅庐^00ff00",
        desc        = "        未飞升 人物等级达到40级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00初出茅庐^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,40,},
--            descFormat="0,40,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [5]=
    {
        name        = "^00ff00驾轻就熟^00ff00",
        desc        = "        未飞升 人物等级达到50级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00驾轻就熟^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,50,},
--            descFormat="0,50,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [6]=
    {
        name        = "^00ff00游刃有余^00ff00",
        desc        = "        未飞升 人物等级达到60级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00游刃有余^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,60,},
--            descFormat="0,60,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [7]=
    {
        name        = "^00ff00小有所成^00ff00",
        desc        = "        未飞升 人物等级达到70级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00小有所成^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,70,},
--            descFormat="0,70,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [8]=
    {
        name        = "^00ff00登堂入室^00ff00",
        desc        = "        未飞升 人物等级达到80级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00登堂入室^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,80,},
--            descFormat="0,80,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [9]=
    {
        name        = "^00ff00炉火纯青^00ff00",
        desc        = "        未飞升 人物等级达到90级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00炉火纯青^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,90,},
--            descFormat="0,90,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [10]=
    {
        name        = "^00ff00通天彻地^00ff00",
        desc        = "        未飞升 人物等级达到100级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00通天彻地^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,100,},
--            descFormat="0,100,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [11]=
    {
        name        = "^00ff00震古烁今^00ff00",
        desc        = "        未飞升 人物等级达到110级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00震古烁今^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,110,},
--            descFormat="0,110,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [12]=
    {
        name        = "^00ff00空前绝后^00ff00",
        desc        = "        未飞升 人物等级达到120级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00空前绝后^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,120,},
--            descFormat="0,120,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [13]=
    {
        name        = "^00ff00返璞归真^00ff00",
        desc        = "        未飞升 人物等级达到130级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00返璞归真^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,130,},
--            descFormat="0,130,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [14]=
    {
        name        = "^00ff00超凡入圣^00ff00",
        desc        = "        未飞升 人物等级达到140级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00超凡入圣^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,140,},
--            descFormat="0,140,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [15]=
    {
        name        = "^00ff00无上至尊^00ff00",
        desc        = "        未飞升 人物等级达到150级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00无上至尊^00ff00>",
        conds=
        {
            {
              id=1,
              params={0,150,},
--            descFormat="0,150,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [16]=
    {
        name        = "^00ff00出神入化^00ff00",
        desc        = "        飞升 人物等级达到120级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00出神入化^00ff00>",
        conds=
        {
            {
              id=1,
              params={1,120,},
--            descFormat="1,120,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [17]=
    {
        name        = "^00ff00登峰造极^00ff00",
        desc        = "        飞升 人物等级达到135级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00登峰造极^00ff00>",
        conds=
        {
            {
              id=1,
              params={1,135,},
--            descFormat="1,135,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [18]=
    {
        name        = "^00ff00破碎虚空^00ff00",
        desc        = "        飞升 人物等级达到150级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00破碎虚空^00ff00>",
        conds=
        {
            {
              id=1,
              params={1,150,},
--            descFormat="1,150,", 
              infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [19]=
    {
        name        = "^00ff00富可敌国^00ff00",
        desc        = "        拥有金钱达到20万金\r        (拥有的金钱必须存放在背包中)",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00富可敌国^00ff00>",
        conds=
        {
            {
              id=2,
              params={2000000000,},
--            descFormat="2000000000,", 
              infoFormat = "拥有金钱数达到 %d (%d/1)",
            },
        }, 
    },

    [20]=
    {
        name        = "^00ff00无量寿佛^00ff00",
        desc        = "        佛缘值达到50万",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00无量寿佛^00ff00>",
        conds=
        {
            {
              id=3,
              params={11,500000,},
--            descFormat="11,500000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [21]=
    {
        name        = "^00ff00不灭明尊^00ff00",
        desc        = "        魔性值达到50万",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00不灭明尊^00ff00>",
        conds=
        {
            {
              id=3,
              params={10,500000,},
--            descFormat="10,500000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [22]=
    {
        name        = "^00ff00三清圣祖^00ff00",
        desc        = "        道心值达到50万",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00三清圣祖^00ff00>",
        conds=
        {
            {
              id=3,
              params={9,500000,},
--            descFormat="9,500000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [23]=
    {
        name        = "^00ff00鬼王宗名宿^00ff00",
        desc        = "        鬼王贡献度达到2亿",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00鬼王宗名宿^00ff00>",
        conds=
        {
            {
              id=3,
              params={3,200000000,},
--            descFormat="3,200000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [24]=
    {
        name        = "^00ff00青云门名宿^00ff00",
        desc        = "        青云贡献度达到2亿",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00青云门名宿^00ff00>",
        conds=
        {
            {
              id=3,
              params={1,200000000,},
--            descFormat="1,200000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [25]=
    {
        name        = "^00ff00合欢派名宿^00ff00",
        desc        = "        合欢贡献度达到2亿",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00合欢派名宿^00ff00>",
        conds=
        {
            {
              id=3,
              params={4,200000000,},
--            descFormat="4,200000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [26]=
    {
        name        = "^00ff00天音寺名宿^00ff00",
        desc        = "        天音贡献度达到2亿",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天音寺名宿^00ff00>",
        conds=
        {
            {
              id=3,
              params={2,200000000,},
--            descFormat="2,200000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [27]=
    {
        name        = "^00ff00鬼道名宿^00ff00",
        desc        = "        鬼道贡献度达到2亿",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00鬼道名宿^00ff00>",
        conds=
        {
            {
              id=3,
              params={0,200000000,},
--            descFormat="0,200000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [28]=
    {
        name        = "^00ff00忠义无双^00ff00",
        desc        = "        忠义值达到2000万",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00忠义无双^00ff00>",
        conds=
        {
            {
              id=3,
              params={5,20000000,},
--            descFormat="5,20000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [29]=
    {
        name        = "^00ff00多情种子^00ff00",
        desc        = "        情缘值达到20万",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00多情种子^00ff00>",
        conds=
        {
            {
              id=3,
              params={6,200000,},
--            descFormat="6,200000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [30]=
    {
        name        = "^00ff00为人师表^00ff00",
        desc        = "        师德值达到5万",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00为人师表^00ff00>",
        conds=
        {
            {
              id=3,
              params={8,50000,},
--            descFormat="8,50000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [31]=
    {
        name        = "^00ff00才高八斗^00ff00",
        desc        = "        文采值达到100万",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00才高八斗^00ff00>",
        conds=
        {
            {
              id=3,
              params={7,1000000,},
--            descFormat="7,1000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [32]=
    {
        name        = "^00ff00卡~卡~^00ff00",
        desc        = "        在诛仙世界中死亡次数达到200次 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00卡~卡~^00ff00>",
        conds=
        {
            {
              id=1004,
              params={200,},
--            descFormat="200,", 
              infoFormat = "死亡次数 %d (%d/%d)",
            },
        }, 
    },

    [33]=
    {
        name        = "^00ff00穷凶极恶^00ff00",
        desc        = "        善恶值达到50点",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 2,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00穷凶极恶^00ff00>",
        conds=
        {
            {
              id=5,
              params={50,},
--            descFormat="50,", 
              infoFormat = "善恶值达到 %d (%d/1)",
            },
        }, 
    },

    [34]=
    {
        name        = "^00ff00凶神恶煞^00ff00",
        desc        = "        善恶值达到100点",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 2,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00凶神恶煞^00ff00>",
        conds=
        {
            {
              id=5,
              params={100,},
--            descFormat="100,", 
              infoFormat = "善恶值达到 %d (%d/1)",
            },
        }, 
    },

    [35]=
    {
        name        = "^00ff00无恶不作^00ff00",
        desc        = "        善恶值达到500点",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00无恶不作^00ff00>",
        conds=
        {
            {
              id=5,
              params={500,},
--            descFormat="500,", 
              infoFormat = "善恶值达到 %d (%d/1)",
            },
        }, 
    },

    [36]=
    {
        name        = "^00ff00恶贯满盈^00ff00",
        desc        = "        善恶值达到999点",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00恶贯满盈^00ff00>",
        conds=
        {
            {
              id=5,
              params={999,},
--            descFormat="999,", 
              infoFormat = "善恶值达到 %d (%d/1)",
            },
        }, 
    },

    [37]=
    {
        name        = "^00ff00大四喜^00ff00",
        desc        = "        击杀狐岐山地宫中的上古四大神兽 \r        夔牛、黄鸟、饕餮、烛龙",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 4,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00大四喜^00ff00>",
        conds=
        {
            {
              id=4,
              params={5594,},
--            descFormat="5594,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={5593,},
--            descFormat="5593,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={5595,},
--            descFormat="5595,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={5592,},
--            descFormat="5592,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [38]=
    {
        name        = "^00ff00修罗王者^00ff00",
        desc        = "        击杀“修罗七王” 安土王、法莲王\r        风火王、归蝶王、天忍王、阴阳王、樱木王 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 4,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00修罗王者^00ff00>",
        conds=
        {
            {
              id=4,
              params={10677,},
--            descFormat="10677,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={10670,},
--            descFormat="10670,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={10676,},
--            descFormat="10676,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={10671,},
--            descFormat="10671,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={10672,},
--            descFormat="10672,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={10675,},
--            descFormat="10675,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={10674,},
--            descFormat="10674,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [39]=
    {
        name        = "^00ff00啸傲凌霄^00ff00",
        desc        = "        击杀凌霄城五大门派首领\r        道玄、普智、金铃夫人、黑心老人、鬼先生 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 4,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00啸傲凌霄^00ff00>",
        conds=
        {
            {
              id=4,
              params={22357,},
--            descFormat="22357,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={22354,},
--            descFormat="22354,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={22356,},
--            descFormat="22356,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={22355,},
--            descFormat="22355,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={22358,},
--            descFormat="22358,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [40]=
    {
        name        = "^00ff00炎狱之王^00ff00",
        desc        = "        在炎狱战场中\r        击杀隐仙阁和逸龙轩的中心建筑各19座 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00炎狱之王^00ff00>",
        conds=
        {
            {
              id=1001,
              params={13445,19,},
--            descFormat="13445,19,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
            {
              id=1001,
              params={13439,19,},
--            descFormat="13439,19,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [41]=
    {
        name        = "^00ff00武围之王^00ff00",
        desc        = "        在武围战场中\r        击杀隐仙阁和逸龙轩的中心建筑各29座 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00武围之王^00ff00>",
        conds=
        {
            {
              id=1001,
              params={10846,29,},
--            descFormat="10846,29,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
            {
              id=1001,
              params={10850,29,},
--            descFormat="10850,29,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [42]=
    {
        name        = "^00ff00界河之王^00ff00",
        desc        = "        在界河战场中\r        击杀隐仙阁和逸龙轩的中心建筑各39座 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00界河之王^00ff00>",
        conds=
        {
            {
              id=1001,
              params={10854,39,},
--            descFormat="10854,39,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
            {
              id=1001,
              params={10858,39,},
--            descFormat="10858,39,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [43]=
    {
        name        = "^00ff00天弈之王^00ff00",
        desc        = "        在天弈战场中\r        击杀隐仙阁 将和逸龙轩 将各49次 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天弈之王^00ff00>",
        conds=
        {
            {
              id=1001,
              params={13693,49,},
--            descFormat="13693,49,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
            {
              id=1001,
              params={13698,49,},
--            descFormat="13698,49,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [44]=
    {
        name        = "^00ff00炎狱主宰^00ff00",
        desc        = "        在幻世炎狱战场中\r        击杀隐仙阁和逸龙轩的中心建筑各19座 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00炎狱主宰^00ff00>",
        conds=
        {
            {
              id=1001,
              params={23369,19,},
--            descFormat="23369,19,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
            {
              id=1001,
              params={23368,19,},
--            descFormat="23368,19,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [45]=
    {
        name        = "^00ff00武围主宰^00ff00",
        desc        = "        在幻世武围战场中\r        击杀隐仙阁和逸龙轩的中心建筑各29座 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00武围主宰^00ff00>",
        conds=
        {
            {
              id=1001,
              params={23372,29,},
--            descFormat="23372,29,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
            {
              id=1001,
              params={23373,29,},
--            descFormat="23373,29,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [46]=
    {
        name        = "^00ff00界河主宰^00ff00",
        desc        = "        在幻世界河战场中\r        击杀隐仙阁和逸龙轩的中心建筑各39座 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00界河主宰^00ff00>",
        conds=
        {
            {
              id=1001,
              params={23376,39,},
--            descFormat="23376,39,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
            {
              id=1001,
              params={23377,39,},
--            descFormat="23377,39,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [47]=
    {
        name        = "^00ff00天弈主宰^00ff00",
        desc        = "        在幻世天弈战场中\r        击杀隐仙阁 将和逸龙轩 将各49次 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天弈主宰^00ff00>",
        conds=
        {
            {
              id=1001,
              params={23379,49,},
--            descFormat="23379,49,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
            {
              id=1001,
              params={23378,49,},
--            descFormat="23378,49,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [48]=
    {
        name        = "^00ff00死泽散人^00ff00",
        desc        = "        击杀死泽 云游天将999次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00死泽散人^00ff00>",
        conds=
        {
            {
              id=1001,
              params={18828,999,},
--            descFormat="18828,999,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [49]=
    {
        name        = "^00ff00蛮荒散人^00ff00",
        desc        = "        击杀蛮荒 云游天将999次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00蛮荒散人^00ff00>",
        conds=
        {
            {
              id=1001,
              params={18829,999,},
--            descFormat="18829,999,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [50]=
    {
        name        = "^00ff00焚香散人^00ff00",
        desc        = "        击杀焚香 云游天将999次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00焚香散人^00ff00>",
        conds=
        {
            {
              id=1001,
              params={18830,999,},
--            descFormat="18830,999,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [51]=
    {
        name        = "^00ff00南疆散人^00ff00",
        desc        = "        击杀南疆 云游天将999次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 1,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00南疆散人^00ff00>",
        conds=
        {
            {
              id=1001,
              params={18831,999,},
--            descFormat="18831,999,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [52]=
    {
        name        = "^00ff00昆仑散人^00ff00",
        desc        = "        击杀昆仑 云游天将999次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 1,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00昆仑散人^00ff00>",
        conds=
        {
            {
              id=1001,
              params={18832,999,},
--            descFormat="18832,999,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [53]=
    {
        name        = "^00ff00修罗散人^00ff00",
        desc        = "        击杀修罗 云游天将999次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 1,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00修罗散人^00ff00>",
        conds=
        {
            {
              id=1001,
              params={18833,999,},
--            descFormat="18833,999,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [54]=
    {
        name        = "^00ff00死泽雪猎人^00ff00",
        desc        = "        击杀死泽 雪国特使99次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00死泽雪猎人^00ff00>",
        conds=
        {
            {
              id=1001,
              params={22285,99,},
--            descFormat="22285,99,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [55]=
    {
        name        = "^00ff00蛮荒雪猎人^00ff00",
        desc        = "        击杀蛮荒 雪国特使99次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00蛮荒雪猎人^00ff00>",
        conds=
        {
            {
              id=1001,
              params={22198,99,},
--            descFormat="22198,99,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [56]=
    {
        name        = "^00ff00焚香雪猎人^00ff00",
        desc        = "        击杀焚香 雪国特使99次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00焚香雪猎人^00ff00>",
        conds=
        {
            {
              id=1001,
              params={22207,99,},
--            descFormat="22207,99,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [57]=
    {
        name        = "^00ff00南疆雪猎人^00ff00",
        desc        = "        击杀南疆 雪国特使99次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 1,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00南疆雪猎人^00ff00>",
        conds=
        {
            {
              id=1001,
              params={22216,99,},
--            descFormat="22216,99,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [58]=
    {
        name        = "^00ff00昆仑雪猎人^00ff00",
        desc        = "        击杀昆仑 雪国特使99次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 1,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00昆仑雪猎人^00ff00>",
        conds=
        {
            {
              id=1001,
              params={22225,99,},
--            descFormat="22225,99,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [59]=
    {
        name        = "^00ff00修罗雪猎人^00ff00",
        desc        = "        击杀修罗 雪国特使99次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 1,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00修罗雪猎人^00ff00>",
        conds=
        {
            {
              id=1001,
              params={22234,99,},
--            descFormat="22234,99,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [60]=
    {
        name        = "^00ff00你完了^00ff00",
        desc        = "        击杀九人团Boss  密探·永\r        破除艮元镇石 密探将出现在焚香",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 4,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00你完了^00ff00>",
        conds=
        {
            {
              id=4,
              params={23165,},
--            descFormat="23165,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [61]=
    {
        name        = "^00ff00邪恶先锋^00ff00",
        desc        = "        击杀九人团Boss  邪恶先锋\r        破除乾元镇石 邪恶先锋将出现在昆仑",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 4,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00邪恶先锋^00ff00>",
        conds=
        {
            {
              id=4,
              params={22106,},
--            descFormat="22106,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={22107,},
--            descFormat="22107,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [62]=
    {
        name        = "^00ff00背叛者^00ff00",
        desc        = "        击杀九人团Boss  背叛者\r        破除坎元镇石 背叛者将出现在南疆 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 4,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00背叛者^00ff00>",
        conds=
        {
            {
              id=4,
              params={22253,},
--            descFormat="22253,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={22251,},
--            descFormat="22251,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={22252,},
--            descFormat="22252,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [63]=
    {
        name        = "^00ff00小霸主^00ff00",
        desc        = "        击杀三种不同形态的 西楚霸王·项羽\r        需进入楚汉军魂战场",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00小霸主^00ff00>",
        conds=
        {
            {
              id=4,
              params={24739,},
--            descFormat="24739,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={24738,},
--            descFormat="24738,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={24737,},
--            descFormat="24737,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [64]=
    {
        name        = "^00ff00流氓克星^00ff00",
        desc        = "        击杀三种不同形态的 汉王·刘邦\r        需进入楚汉军魂战场",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00流氓克星^00ff00>",
        conds=
        {
            {
              id=4,
              params={24740,},
--            descFormat="24740,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={24741,},
--            descFormat="24741,", 
              infoFormat = "击杀 %s (%d/1)",
            },
            {
              id=4,
              params={24742,},
--            descFormat="24742,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [65]=
    {
        name        = "^00ff00汉军·推倒^00ff00",
        desc        = "        在楚汉军魂战场中摧毁 汉军兵营\r        需进入楚汉军魂战场",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00汉军·推倒^00ff00>",
        conds=
        {
            {
              id=4,
              params={25173,},
--            descFormat="25173,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [66]=
    {
        name        = "^00ff00楚军·推倒^00ff00",
        desc        = "        在楚汉军魂战场中摧毁 楚军兵营\r        需进入楚汉军魂战场",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00楚军·推倒^00ff00>",
        conds=
        {
            {
              id=4,
              params={25171,},
--            descFormat="25171,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [67]=
    {
        name        = "^00ff00仙界杀星^00ff00",
        desc        = "        在混沌中击杀仙之守护99次 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 5,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00仙界杀星^00ff00>",
        conds=
        {
            {
              id=1001,
              params={22018,99,},
--            descFormat="22018,99,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [68]=
    {
        name        = "^00ff00魔域杀星^00ff00",
        desc        = "        在混沌中击杀魔之守护99次 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 5,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00魔域杀星^00ff00>",
        conds=
        {
            {
              id=1001,
              params={22019,99,},
--            descFormat="22019,99,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [69]=
    {
        name        = "^00ff00佛国杀星^00ff00",
        desc        = "        在混沌中击杀佛之守护99次 ",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 5,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00佛国杀星^00ff00>",
        conds=
        {
            {
              id=1001,
              params={22020,99,},
--            descFormat="22020,99,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [70]=
    {
        name        = "^00ff00韦陀·破灭^00ff00",
        desc        = "        在混沌中击杀韦陀·荼罗",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 5,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00韦陀·破灭^00ff00>",
        conds=
        {
            {
              id=4,
              params={23265,},
--            descFormat="23265,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [71]=
    {
        name        = "^00ff00魔君·破灭^00ff00",
        desc        = "        在混沌中击杀魔君·郁垒",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 5,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00魔君·破灭^00ff00>",
        conds=
        {
            {
              id=4,
              params={23267,},
--            descFormat="23267,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [72]=
    {
        name        = "^00ff00神将·破灭^00ff00",
        desc        = "        在混沌中击杀神将·扈戎",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 5,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00神将·破灭^00ff00>",
        conds=
        {
            {
              id=4,
              params={23262,},
--            descFormat="23262,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [73]=
    {
        name        = "^00ff00后勤干事^00ff00",
        desc        = "        完成建功立事任务99次 ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00后勤干事^00ff00>",
        conds=
        {
            {
              id=1000,
              params={12856,99,},
--            descFormat="12856,99,", 
              infoFormat = "完成 %s (%d/%d)次",
            },
        }, 
    },

    [74]=
    {
        name        = "^00ff00批发宝图^00ff00",
        desc        = "        完成国家宝藏任务99次 \r        只有修罗的国家宝藏任务有效 ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00批发宝图^00ff00>",
        conds=
        {
            {
              id=1000,
              params={12315,99,},
--            descFormat="12315,99,", 
              infoFormat = "完成 %s (%d/%d)次",
            },
        }, 
    },

    [75]=
    {
        name        = "^00ff00天空行者^00ff00",
        desc        = "        完成天行者任务99次 ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天空行者^00ff00>",
        conds=
        {
            {
              id=1000,
              params={12193,99,},
--            descFormat="12193,99,", 
              infoFormat = "完成 %s (%d/%d)次",
            },
        }, 
    },

    [76]=
    {
        name        = "^00ff00开卷有益^00ff00",
        desc        = "        参加新科试炼99次，且未能进入榜单 ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00开卷有益^00ff00>",
        conds=
        {
            {
              id=1000,
              params={12871,99,},
--            descFormat="12871,99,", 
              infoFormat = "完成 %s (%d/%d)次",
            },
        }, 
    },

    [77]=
    {
        name        = "^00ff00鬼厉装收集^00ff00",
        desc        = "        收集一套鬼厉装\r        （穿戴时装达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00鬼厉装收集^00ff00>",
        conds=
        {
            {
              id=11,
              params={14242,},
--            descFormat="14242,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={14243,},
--            descFormat="14243,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={14244,},
--            descFormat="14244,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [78]=
    {
        name        = "^00ff00碧瑶装收集^00ff00",
        desc        = "        收集一套碧瑶装\r        （穿戴时装达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00碧瑶装收集^00ff00>",
        conds=
        {
            {
              id=11,
              params={12023,},
--            descFormat="12023,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={12024,},
--            descFormat="12024,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={12025,},
--            descFormat="12025,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [79]=
    {
        name        = "^00ff00雪琪装收集^00ff00",
        desc        = "        收集一套雪琪装\r        （穿戴时装达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00雪琪装收集^00ff00>",
        conds=
        {
            {
              id=11,
              params={17077,},
--            descFormat="17077,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={17075,},
--            descFormat="17075,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={17076,},
--            descFormat="17076,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [80]=
    {
        name        = "^00ff00腾云龙装收集^00ff00",
        desc        = "        收集一套腾龙装和云龙装\r        （穿戴时装达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00腾云龙装收集^00ff00>",
        conds=
        {
            {
              id=11,
              params={19436,},
--            descFormat="19436,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={19437,},
--            descFormat="19437,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={19438,},
--            descFormat="19438,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={18884,},
--            descFormat="18884,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={18888,},
--            descFormat="18888,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={18886,},
--            descFormat="18886,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [81]=
    {
        name        = "^00ff00青烛龙装收集^00ff00",
        desc        = "        收集一套青龙装和烛龙装\r        （穿戴时装达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00青烛龙装收集^00ff00>",
        conds=
        {
            {
              id=11,
              params={23428,},
--            descFormat="23428,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={23429,},
--            descFormat="23429,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={23430,},
--            descFormat="23430,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={11955,},
--            descFormat="11955,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={11958,},
--            descFormat="11958,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={11960,},
--            descFormat="11960,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [82]=
    {
        name        = "^00ff00四龙装全收集^00ff00",
        desc        = "        达成四龙装全收集\r        青龙装、腾龙装、云龙装、烛龙装",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00四龙装全收集^00ff00>",
        conds=
        {
            {
              id=12,
              params={80,},
--            descFormat="80,", 
              infoFormat = "达成成就 %s (%d/1)",
            },
            {
              id=12,
              params={81,},
--            descFormat="81,", 
              infoFormat = "达成成就 %s (%d/1)",
            },
        }, 
    },

    [83]=
    {
        name        = "^00ff00女版圣诞装收集^00ff00",
        desc        = "        收集两套圣诞装\r        （穿戴时装达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00女版圣诞装收集^00ff00>",
        conds=
        {
            {
              id=11,
              params={13613,},
--            descFormat="13613,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={13615,},
--            descFormat="13615,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={13617,},
--            descFormat="13617,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={22001,},
--            descFormat="22001,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={22003,},
--            descFormat="22003,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={22005,},
--            descFormat="22005,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [84]=
    {
        name        = "^00ff00男版圣诞装收集^00ff00",
        desc        = "        收集两套圣诞装\r        （穿戴时装达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00男版圣诞装收集^00ff00>",
        conds=
        {
            {
              id=11,
              params={13612,},
--            descFormat="13612,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={13614,},
--            descFormat="13614,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={13616,},
--            descFormat="13616,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={22000,},
--            descFormat="22000,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={22002,},
--            descFormat="22002,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={22004,},
--            descFormat="22004,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [85]=
    {
        name        = "^00ff00夏季女装收集^00ff00",
        desc        = "        收集女款夏季时装：弥云凉装和清凉装\r        （穿戴时装达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00夏季女装收集^00ff00>",
        conds=
        {
            {
              id=11,
              params={10631,},
--            descFormat="10631,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={10635,},
--            descFormat="10635,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={10633,},
--            descFormat="10633,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={7807,},
--            descFormat="7807,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={7811,},
--            descFormat="7811,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={7816,},
--            descFormat="7816,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [86]=
    {
        name        = "^00ff00夏季男装收集^00ff00",
        desc        = "        收集男款夏季时装：弥云凉装和刺青装\r        （穿戴时装达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00夏季男装收集^00ff00>",
        conds=
        {
            {
              id=11,
              params={10630,},
--            descFormat="10630,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={10634,},
--            descFormat="10634,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={10632,},
--            descFormat="10632,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={7806,},
--            descFormat="7806,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={7810,},
--            descFormat="7810,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={7815,},
--            descFormat="7815,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [87]=
    {
        name        = "^00ff00女性双花装收集^00ff00",
        desc        = "        收集一套兰花装和鲜花装\r        （穿戴时装达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00女性双花装收集^00ff00>",
        conds=
        {
            {
              id=11,
              params={6375,},
--            descFormat="6375,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={6383,},
--            descFormat="6383,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={6388,},
--            descFormat="6388,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={6376,},
--            descFormat="6376,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={6381,},
--            descFormat="6381,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={6387,},
--            descFormat="6387,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [88]=
    {
        name        = "^00ff00造化之天道^00ff00",
        desc        = "        获取造化之天道\r        （装备饰品达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00造化之天道^00ff00>",
        conds=
        {
            {
              id=11,
              params={22086,},
--            descFormat="22086,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [89]=
    {
        name        = "^00ff00造化之天循^00ff00",
        desc        = "        获取造化之天循\r        （装备饰品达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00造化之天循^00ff00>",
        conds=
        {
            {
              id=11,
              params={22081,},
--            descFormat="22081,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [90]=
    {
        name        = "^00ff00造化之玉虚^00ff00",
        desc        = "        获取造化之玉虚\r        （装备饰品达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00造化之玉虚^00ff00>",
        conds=
        {
            {
              id=11,
              params={22085,},
--            descFormat="22085,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [91]=
    {
        name        = "^00ff00造化之天眷^00ff00",
        desc        = "        获取造化之天眷\r        （装备饰品达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00造化之天眷^00ff00>",
        conds=
        {
            {
              id=11,
              params={22084,},
--            descFormat="22084,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [92]=
    {
        name        = "^00ff00造化之天弈^00ff00",
        desc        = "        获取造化之天弈\r        （装备饰品达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00造化之天弈^00ff00>",
        conds=
        {
            {
              id=11,
              params={22083,},
--            descFormat="22083,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [93]=
    {
        name        = "^00ff00造化之天戍^00ff00",
        desc        = "        获取造化之天戍\r        （装备饰品达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00造化之天戍^00ff00>",
        conds=
        {
            {
              id=11,
              params={22082,},
--            descFormat="22082,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [94]=
    {
        name        = "^00ff00太虚锦囊·九计^00ff00",
        desc        = "        获取太虚锦囊·九计\r        （装备锦囊达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00太虚锦囊·九计^00ff00>",
        conds=
        {
            {
              id=11,
              params={21854,},
--            descFormat="21854,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [95]=
    {
        name        = "^00ff00神行锦囊·九计^00ff00",
        desc        = "        获取神行锦囊·九计\r        （装备锦囊达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00神行锦囊·九计^00ff00>",
        conds=
        {
            {
              id=11,
              params={21819,},
--            descFormat="21819,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [96]=
    {
        name        = "^00ff00逍遥锦囊·九计^00ff00",
        desc        = "        获取逍遥锦囊·九计\r        （装备锦囊达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00逍遥锦囊·九计^00ff00>",
        conds=
        {
            {
              id=11,
              params={21783,},
--            descFormat="21783,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [97]=
    {
        name        = "^00ff00初元罡气^00ff00",
        desc        = "        获取初元罡气\r        （装备罡气达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00初元罡气^00ff00>",
        conds=
        {
            {
              id=11,
              params={22115,},
--            descFormat="22115,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [98]=
    {
        name        = "^00ff00固本罡气^00ff00",
        desc        = "        获取固本罡气\r        （装备罡气达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00固本罡气^00ff00>",
        conds=
        {
            {
              id=11,
              params={22116,},
--            descFormat="22116,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [99]=
    {
        name        = "^00ff00培源罡气^00ff00",
        desc        = "        获取培源罡气\r        （装备罡气达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00培源罡气^00ff00>",
        conds=
        {
            {
              id=11,
              params={22117,},
--            descFormat="22117,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [100]=
    {
        name        = "^00ff00含章罡气^00ff00",
        desc        = "        获取含章罡气\r        （装备罡气达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00含章罡气^00ff00>",
        conds=
        {
            {
              id=11,
              params={22118,},
--            descFormat="22118,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [101]=
    {
        name        = "^00ff00冲虚罡气^00ff00",
        desc        = "        获取冲虚罡气\r        （装备罡气达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00冲虚罡气^00ff00>",
        conds=
        {
            {
              id=11,
              params={22119,},
--            descFormat="22119,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [102]=
    {
        name        = "^00ff00金身·七星转魂^00ff00",
        desc        = "        获取金身·七星转魂\r        （装备金身达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 5,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00金身·七星转魂^00ff00>",
        conds=
        {
            {
              id=11,
              params={20157,},
--            descFormat="20157,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [103]=
    {
        name        = "^00ff00金身·八法运通^00ff00",
        desc        = "        获取金身·八法运通\r        （装备金身达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 5,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00金身·八法运通^00ff00>",
        conds=
        {
            {
              id=11,
              params={20158,},
--            descFormat="20158,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [104]=
    {
        name        = "^00ff00法身·七星转魂^00ff00",
        desc        = "        获取法身·七星转魂\r        （装备法身达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 5,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00法身·七星转魂^00ff00>",
        conds=
        {
            {
              id=11,
              params={20144,},
--            descFormat="20144,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [105]=
    {
        name        = "^00ff00法身·八法运通^00ff00",
        desc        = "        获取法身·八法运通\r        （装备法身达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 5,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00法身·八法运通^00ff00>",
        conds=
        {
            {
              id=11,
              params={20145,},
--            descFormat="20145,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [106]=
    {
        name        = "^00ff00吉星高照^00ff00",
        desc        = "        精炼任意一件装备（+8）成功 ",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00吉星高照^00ff00>",
        conds=
        {
            {
              id=7,
              params={7,8,},
--            descFormat="7,8,", 
              infoFormat = "炼器+ %d %s (%d/1)",
            },
        }, 
    },

    [107]=
    {
        name        = "^00ff00功亏一篑^00ff00",
        desc        = "        精炼任意一件装备（+8）失败 \r        （未使用完璧符，装备消失）",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 1,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00功亏一篑^00ff00>",
        conds=
        {
            {
              id=7,
              params={7,},
--            descFormat="7,", 
              infoFormat = "炼器+ %d %s (%d/1)",
            },
        }, 
    },

    [108]=
    {
        name        = "^00ff00鸿运当头^00ff00",
        desc        = "        精炼任意一件装备（+10）成功 ",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00鸿运当头^00ff00>",
        conds=
        {
            {
              id=7,
              params={9,10,},
--            descFormat="9,10,", 
              infoFormat = "炼器+ %d %s (%d/1)",
            },
        }, 
    },

    [109]=
    {
        name        = "^00ff00前功尽弃^00ff00",
        desc        = "        精炼任意一件装备（+10）失败 \r        （未使用完璧符，装备消失）",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00前功尽弃^00ff00>",
        conds=
        {
            {
              id=7,
              params={9,},
--            descFormat="9,", 
              infoFormat = "炼器+ %d %s (%d/1)",
            },
        }, 
    },

    [110]=
    {
        name        = "^00ff00福星降临^00ff00",
        desc        = "        精炼任意一件装备（+12）成功 ",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 1,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00福星降临^00ff00>",
        conds=
        {
            {
              id=7,
              params={11,12,},
--            descFormat="11,12,", 
              infoFormat = "炼器+ %d %s (%d/1)",
            },
        }, 
    },

    [111]=
    {
        name        = "^00ff00衰神附体^00ff00",
        desc        = "        精炼任意一件装备（+12）失败 \r        （未使用完璧符，装备消失）",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 1,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00衰神附体^00ff00>",
        conds=
        {
            {
              id=7,
              params={11,},
--            descFormat="11,", 
              infoFormat = "炼器+ %d %s (%d/1)",
            },
        }, 
    },

    [112]=
    {
        name        = "^00ff00炼器大师^00ff00",
        desc        = "        精炼装备（+8）成功49次 ",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 1,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00炼器大师^00ff00>",
        conds=
        {
            {
              id=1005,
              params={0,8,49,},
--            descFormat="0,8,49,", 
              infoFormat = "炼器+ %d %s (%d/%d)次",
            },
        }, 
    },

    [113]=
    {
        name        = "^00ff00炼器宗师^00ff00",
        desc        = "        精炼装备（+9）成功49次 ",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 1,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00炼器宗师^00ff00>",
        conds=
        {
            {
              id=1005,
              params={0,9,49,},
--            descFormat="0,9,49,", 
              infoFormat = "炼器+ %d %s (%d/%d)次",
            },
        }, 
    },

    [114]=
    {
        name        = "^00ff00至阳之五行刃石^00ff00",
        desc        = "        在血祭大师处兑换全属性至阳之五行刃石",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00至阳之五行刃石^00ff00>",
        conds=
        {
            {
              id=10,
              params={3373,22614,},
--            descFormat="3373,22614,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3367,22534,},
--            descFormat="3367,22534,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3369,22777,},
--            descFormat="3369,22777,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3371,22857,},
--            descFormat="3371,22857,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3375,22694,},
--            descFormat="3375,22694,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [115]=
    {
        name        = "^00ff00至阳之五行魁石^00ff00",
        desc        = "        在血祭大师处兑换全属性至阳之五行魁石",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00至阳之五行魁石^00ff00>",
        conds=
        {
            {
              id=10,
              params={3363,22615,},
--            descFormat="3363,22615,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3357,22535,},
--            descFormat="3357,22535,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3359,22778,},
--            descFormat="3359,22778,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3361,22858,},
--            descFormat="3361,22858,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3365,22695,},
--            descFormat="3365,22695,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [116]=
    {
        name        = "^00ff00至阳之五行罡石^00ff00",
        desc        = "        在血祭大师处兑换全属性至阳之五行罡石",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00至阳之五行罡石^00ff00>",
        conds=
        {
            {
              id=10,
              params={3353,22616,},
--            descFormat="3353,22616,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3347,22536,},
--            descFormat="3347,22536,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3349,22779,},
--            descFormat="3349,22779,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3351,22859,},
--            descFormat="3351,22859,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3355,22696,},
--            descFormat="3355,22696,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [117]=
    {
        name        = "^00ff00至阳之五行遁石^00ff00",
        desc        = "        在血祭大师处兑换全属性至阳之五行遁石",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00至阳之五行遁石^00ff00>",
        conds=
        {
            {
              id=10,
              params={3343,22617,},
--            descFormat="3343,22617,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3337,22537,},
--            descFormat="3337,22537,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3339,22780,},
--            descFormat="3339,22780,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3341,22860,},
--            descFormat="3341,22860,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3345,22697,},
--            descFormat="3345,22697,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [118]=
    {
        name        = "^00ff00玄阴之五行刃石^00ff00",
        desc        = "        在血祭大师处兑换全属性玄阴之五行刃石",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄阴之五行刃石^00ff00>",
        conds=
        {
            {
              id=10,
              params={3374,22618,},
--            descFormat="3374,22618,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3368,22538,},
--            descFormat="3368,22538,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3370,22781,},
--            descFormat="3370,22781,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3372,22861,},
--            descFormat="3372,22861,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3376,22698,},
--            descFormat="3376,22698,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [119]=
    {
        name        = "^00ff00玄阴之五行魁石^00ff00",
        desc        = "        在血祭大师处兑换全属性玄阴之五行魁石",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄阴之五行魁石^00ff00>",
        conds=
        {
            {
              id=10,
              params={3364,22619,},
--            descFormat="3364,22619,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3358,22539,},
--            descFormat="3358,22539,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3360,22782,},
--            descFormat="3360,22782,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3362,22862,},
--            descFormat="3362,22862,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3366,22699,},
--            descFormat="3366,22699,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [120]=
    {
        name        = "^00ff00玄阴之五行罡石^00ff00",
        desc        = "        在血祭大师处兑换全属性玄阴之五行罡石",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄阴之五行罡石^00ff00>",
        conds=
        {
            {
              id=10,
              params={3354,22620,},
--            descFormat="3354,22620,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3348,22540,},
--            descFormat="3348,22540,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3350,22783,},
--            descFormat="3350,22783,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3352,22863,},
--            descFormat="3352,22863,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3356,22700,},
--            descFormat="3356,22700,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [121]=
    {
        name        = "^00ff00玄阴之五行遁石^00ff00",
        desc        = "        在血祭大师处兑换全属性玄阴之五行遁石",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄阴之五行遁石^00ff00>",
        conds=
        {
            {
              id=10,
              params={3344,22621,},
--            descFormat="3344,22621,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3338,22541,},
--            descFormat="3338,22541,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3340,22784,},
--            descFormat="3340,22784,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3342,22864,},
--            descFormat="3342,22864,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={3346,22701,},
--            descFormat="3346,22701,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [122]=
    {
        name        = "^00ff00金身·十方乾坤^00ff00",
        desc        = "        成功制造 金身·十方乾坤",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00金身·十方乾坤^00ff00>",
        conds=
        {
            {
              id=10,
              params={2987,20160,},
--            descFormat="2987,20160,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [123]=
    {
        name        = "^00ff00法身·十方乾坤^00ff00",
        desc        = "        成功制造 法身·十方乾坤",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00法身·十方乾坤^00ff00>",
        conds=
        {
            {
              id=10,
              params={2975,20147,},
--            descFormat="2975,20147,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [124]=
    {
        name        = "^00ff00金身·百劫洞冥^00ff00",
        desc        = "        成功制造 金身·百劫洞冥",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 3,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00金身·百劫洞冥^00ff00>",
        conds=
        {
            {
              id=10,
              params={2988,20161,},
--            descFormat="2988,20161,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [125]=
    {
        name        = "^00ff00法身·百劫洞冥^00ff00",
        desc        = "        成功制造 法身·百劫洞冥",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 3,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00法身·百劫洞冥^00ff00>",
        conds=
        {
            {
              id=10,
              params={2976,20148,},
--            descFormat="2976,20148,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [126]=
    {
        name        = "^00ff00金身·千界婆娑^00ff00",
        desc        = "        成功制造 金身·千界婆娑",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00金身·千界婆娑^00ff00>",
        conds=
        {
            {
              id=10,
              params={2989,20162,},
--            descFormat="2989,20162,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [127]=
    {
        name        = "^00ff00法身·千界婆娑^00ff00",
        desc        = "        成功制造 法身·千界婆娑",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00法身·千界婆娑^00ff00>",
        conds=
        {
            {
              id=10,
              params={2977,20149,},
--            descFormat="2977,20149,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [128]=
    {
        name        = "^00ff00金身·万流至尊^00ff00",
        desc        = "        成功制造 金身·万流至尊",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 3,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00金身·万流至尊^00ff00>",
        conds=
        {
            {
              id=10,
              params={2990,20163,},
--            descFormat="2990,20163,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [129]=
    {
        name        = "^00ff00法身·万流至尊^00ff00",
        desc        = "        成功制造 法身·万流至尊",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 3,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00法身·万流至尊^00ff00>",
        conds=
        {
            {
              id=10,
              params={2978,20150,},
--            descFormat="2978,20150,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [130]=
    {
        name        = "^00ff00钻石迷情^00ff00",
        desc        = "        累积制造99颗钻石\r        （需人物生产达到十级）",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 3,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00钻石迷情^00ff00>",
        conds=
        {
            {
              id=1002,
              params={2912,14065,99,},
--            descFormat="2912,14065,99,", 
              infoFormat = "成功生产 %s (%d/%d)",
            },
        }, 
    },

    [131]=
    {
        name        = "^00ff00轩辕制造^00ff00",
        desc        = "        累积制造99个轩辕通宝\r        （需人物生产达到十级）",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00轩辕制造^00ff00>",
        conds=
        {
            {
              id=1002,
              params={2917,19252,99,},
--            descFormat="2917,19252,99,", 
              infoFormat = "成功生产 %s (%d/%d)",
            },
        }, 
    },

    [132]=
    {
        name        = "^00ff00宠物装学徒^00ff00",
        desc        = "        成功制造8件★★紫色宠物装备\r        （使用1阶宠物材料）",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00宠物装学徒^00ff00>",
        conds=
        {
            {
              id=10,
              params={2407,17354,},
--            descFormat="2407,17354,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2359,17350,},
--            descFormat="2359,17350,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2395,17353,},
--            descFormat="2395,17353,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2437,17356,},
--            descFormat="2437,17356,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2371,17351,},
--            descFormat="2371,17351,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2383,17352,},
--            descFormat="2383,17352,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2425,17355,},
--            descFormat="2425,17355,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2449,17357,},
--            descFormat="2449,17357,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [133]=
    {
        name        = "^00ff00宠物装技师^00ff00",
        desc        = "        成功制造8件★★紫色宠物装备\r        （使用2阶宠物材料）",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00宠物装技师^00ff00>",
        conds=
        {
            {
              id=10,
              params={2408,17394,},
--            descFormat="2408,17394,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2360,17390,},
--            descFormat="2360,17390,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2396,17393,},
--            descFormat="2396,17393,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2438,17396,},
--            descFormat="2438,17396,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2372,17391,},
--            descFormat="2372,17391,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2384,17392,},
--            descFormat="2384,17392,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2426,17395,},
--            descFormat="2426,17395,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2450,17397,},
--            descFormat="2450,17397,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [134]=
    {
        name        = "^00ff00宠物装大师^00ff00",
        desc        = "        成功制造8件★★紫色宠物装备\r        （使用3阶宠物材料）",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 4,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00宠物装大师^00ff00>",
        conds=
        {
            {
              id=10,
              params={2409,17434,},
--            descFormat="2409,17434,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2361,17430,},
--            descFormat="2361,17430,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2397,17433,},
--            descFormat="2397,17433,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2439,17436,},
--            descFormat="2439,17436,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2373,17431,},
--            descFormat="2373,17431,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2385,17432,},
--            descFormat="2385,17432,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2427,17435,},
--            descFormat="2427,17435,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2451,17437,},
--            descFormat="2451,17437,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [135]=
    {
        name        = "^00ff00宠物装导师^00ff00",
        desc        = "        成功制造8件★★紫色宠物装备\r        （使用4阶宠物材料）",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 4,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00宠物装导师^00ff00>",
        conds=
        {
            {
              id=10,
              params={2410,17474,},
--            descFormat="2410,17474,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2362,17470,},
--            descFormat="2362,17470,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2398,17473,},
--            descFormat="2398,17473,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2440,17476,},
--            descFormat="2440,17476,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2374,17471,},
--            descFormat="2374,17471,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2386,17472,},
--            descFormat="2386,17472,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2428,17475,},
--            descFormat="2428,17475,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2452,17477,},
--            descFormat="2452,17477,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [136]=
    {
        name        = "^00ff00宠物装宗师^00ff00",
        desc        = "        成功制造8件★★紫色宠物装备\r        （使用5阶宠物材料）",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 4,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00宠物装宗师^00ff00>",
        conds=
        {
            {
              id=10,
              params={2411,17514,},
--            descFormat="2411,17514,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2363,17510,},
--            descFormat="2363,17510,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2399,17513,},
--            descFormat="2399,17513,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2441,17516,},
--            descFormat="2441,17516,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2375,17511,},
--            descFormat="2375,17511,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2387,17512,},
--            descFormat="2387,17512,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2429,17515,},
--            descFormat="2429,17515,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2453,17517,},
--            descFormat="2453,17517,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [137]=
    {
        name        = "^00ff00宠物装神匠^00ff00",
        desc        = "        成功制造8件★★紫色宠物装备\r        （使用6阶宠物材料）",
        preCondDesc = "空",
        mainClass   = 5,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00宠物装神匠^00ff00>",
        conds=
        {
            {
              id=10,
              params={2412,17554,},
--            descFormat="2412,17554,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2364,17550,},
--            descFormat="2364,17550,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2400,17553,},
--            descFormat="2400,17553,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2442,17556,},
--            descFormat="2442,17556,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2376,17551,},
--            descFormat="2376,17551,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2388,17552,},
--            descFormat="2388,17552,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2430,17555,},
--            descFormat="2430,17555,", 
              infoFormat = "生产 %s (%d/1)",
            },
            {
              id=10,
              params={2454,17557,},
--            descFormat="2454,17557,", 
              infoFormat = "生产 %s (%d/1)",
            },
        }, 
    },

    [138]=
    {
        name        = "^00ff00玄天玉薄（一级）^00ff00",
        desc        = "        青云贡献度达到500万领取炙鹰令\r         河阳 白云道长处（仅青云职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄天玉薄（一级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={1,5000000,},
--            descFormat="1,5000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [139]=
    {
        name        = "^00ff00中天灯蕊（二级）^00ff00",
        desc        = "        青云贡献度达到1000万领取炙鹰令\r         河阳 白云道长处（仅青云职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00中天灯蕊（二级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={1,10000000,},
--            descFormat="1,10000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [140]=
    {
        name        = "^00ff00羡天宝烛（三级）^00ff00",
        desc        = "        青云贡献度达到2000万领取炙鹰令\r         河阳 白云道长处（仅青云职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00羡天宝烛（三级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={1,20000000,},
--            descFormat="1,20000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [141]=
    {
        name        = "^00ff00从天纂录（四级）^00ff00",
        desc        = "        青云贡献度达到3000万领取炙鹰令\r         河阳 白云道长处（仅青云职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00从天纂录（四级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={1,30000000,},
--            descFormat="1,30000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [142]=
    {
        name        = "^00ff00更天金钵（五级）^00ff00",
        desc        = "        青云贡献度达到4000万领取炙鹰令\r         河阳 白云道长处（仅青云职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00更天金钵（五级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={1,40000000,},
--            descFormat="1,40000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [143]=
    {
        name        = "^00ff00翠天善果（六级）^00ff00",
        desc        = "        青云贡献度达到5000万领取炙鹰令\r         河阳 白云道长处（仅青云职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00翠天善果（六级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={1,50000000,},
--            descFormat="1,50000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [144]=
    {
        name        = "^00ff00廓天柒木（七级）^00ff00",
        desc        = "        青云贡献度达到6000万领取炙鹰令\r         河阳 白云道长处（仅青云职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00廓天柒木（七级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={1,60000000,},
--            descFormat="1,60000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [145]=
    {
        name        = "^00ff00减天罪竹（八级）^00ff00",
        desc        = "        青云贡献度达到7000万领取炙鹰令\r         河阳 白云道长处（仅青云职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00减天罪竹（八级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={1,70000000,},
--            descFormat="1,70000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [146]=
    {
        name        = "^00ff00沈天泣佞（九级）^00ff00",
        desc        = "        青云贡献度达到8000万领取炙鹰令\r         河阳 白云道长处（仅青云职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00沈天泣佞（九级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={1,80000000,},
--            descFormat="1,80000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [147]=
    {
        name        = "^00ff00成天正果（十级）^00ff00",
        desc        = "        青云贡献度达到1亿领取炙鹰令\r        河阳 白云道长处（仅青云职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00成天正果（十级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={1,100000000,},
--            descFormat="1,100000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [148]=
    {
        name        = "^00ff00玄天玉薄（一级）^00ff00",
        desc        = "        鬼王贡献度达到500万领取炙鹰令\r        河阳 幽姬处（仅鬼王职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄天玉薄（一级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={3,5000000,},
--            descFormat="3,5000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [149]=
    {
        name        = "^00ff00中天灯蕊（二级）^00ff00",
        desc        = "        鬼王贡献度达到1000万领取炙鹰令\r        河阳 幽姬处（仅鬼王职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00中天灯蕊（二级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={3,10000000,},
--            descFormat="3,10000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [150]=
    {
        name        = "^00ff00羡天宝烛（三级）^00ff00",
        desc        = "        鬼王贡献度达到2000万领取炙鹰令\r        河阳 幽姬处（仅鬼王职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00羡天宝烛（三级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={3,20000000,},
--            descFormat="3,20000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [151]=
    {
        name        = "^00ff00从天纂录（四级）^00ff00",
        desc        = "        鬼王贡献度达到3000万领取炙鹰令\r        河阳 幽姬处（仅鬼王职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00从天纂录（四级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={3,30000000,},
--            descFormat="3,30000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [152]=
    {
        name        = "^00ff00更天金钵（五级）^00ff00",
        desc        = "        鬼王贡献度达到4000万领取炙鹰令\r        河阳 幽姬处（仅鬼王职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00更天金钵（五级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={3,40000000,},
--            descFormat="3,40000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [153]=
    {
        name        = "^00ff00翠天善果（六级）^00ff00",
        desc        = "        鬼王贡献度达到5000万领取炙鹰令\r        河阳 幽姬处（仅鬼王职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00翠天善果（六级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={3,50000000,},
--            descFormat="3,50000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [154]=
    {
        name        = "^00ff00廓天柒木（七级）^00ff00",
        desc        = "        鬼王贡献度达到6000万领取炙鹰令\r        河阳 幽姬处（仅鬼王职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00廓天柒木（七级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={3,60000000,},
--            descFormat="3,60000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [155]=
    {
        name        = "^00ff00减天罪竹（八级）^00ff00",
        desc        = "        鬼王贡献度达到7000万领取炙鹰令\r        河阳 幽姬处（仅鬼王职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00减天罪竹（八级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={3,70000000,},
--            descFormat="3,70000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [156]=
    {
        name        = "^00ff00沈天泣佞（九级）^00ff00",
        desc        = "        鬼王贡献度达到8000万领取炙鹰令\r        河阳 幽姬处（仅鬼王职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00沈天泣佞（九级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={3,80000000,},
--            descFormat="3,80000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [157]=
    {
        name        = "^00ff00成天正果（十级）^00ff00",
        desc        = "        鬼王贡献度达到1亿领取炙鹰令\r        河阳 幽姬处（仅鬼王职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00成天正果（十级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={3,100000000,},
--            descFormat="3,100000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [158]=
    {
        name        = "^00ff00玄天玉薄（一级）^00ff00",
        desc        = "        天音贡献度达到500万领取炙鹰令\r        河阳 法济处（仅天音职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄天玉薄（一级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={2,5000000,},
--            descFormat="2,5000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [159]=
    {
        name        = "^00ff00中天灯蕊（二级）^00ff00",
        desc        = "        天音贡献度达到1000万领取炙鹰令\r        河阳 法济处（仅天音职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00中天灯蕊（二级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={2,10000000,},
--            descFormat="2,10000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [160]=
    {
        name        = "^00ff00羡天宝烛（三级）^00ff00",
        desc        = "        天音贡献度达到2000万领取炙鹰令\r        河阳 法济处（仅天音职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00羡天宝烛（三级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={2,20000000,},
--            descFormat="2,20000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [161]=
    {
        name        = "^00ff00从天纂录（四级）^00ff00",
        desc        = "        天音贡献度达到3000万领取炙鹰令\r        河阳 法济处（仅天音职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00从天纂录（四级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={2,30000000,},
--            descFormat="2,30000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [162]=
    {
        name        = "^00ff00更天金钵（五级）^00ff00",
        desc        = "        天音贡献度达到4000万领取炙鹰令\r        河阳 法济处（仅天音职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00更天金钵（五级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={2,40000000,},
--            descFormat="2,40000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [163]=
    {
        name        = "^00ff00翠天善果（六级）^00ff00",
        desc        = "        天音贡献度达到5000万领取炙鹰令\r        河阳 法济处（仅天音职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00翠天善果（六级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={2,50000000,},
--            descFormat="2,50000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [164]=
    {
        name        = "^00ff00廓天柒木（七级）^00ff00",
        desc        = "        天音贡献度达到6000万领取炙鹰令\r        河阳 法济处（仅天音职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00廓天柒木（七级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={2,60000000,},
--            descFormat="2,60000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [165]=
    {
        name        = "^00ff00减天罪竹（八级）^00ff00",
        desc        = "        天音贡献度达到7000万领取炙鹰令\r        河阳 法济处（仅天音职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00减天罪竹（八级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={2,70000000,},
--            descFormat="2,70000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [166]=
    {
        name        = "^00ff00沈天泣佞（九级）^00ff00",
        desc        = "        天音贡献度达到8000万领取炙鹰令\r        河阳 法济处（仅天音职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00沈天泣佞（九级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={2,80000000,},
--            descFormat="2,80000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [167]=
    {
        name        = "^00ff00成天正果（十级）^00ff00",
        desc        = "        天音贡献度达到1亿领取炙鹰令\r        河阳 法济处（仅天音职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00成天正果（十级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={2,100000000,},
--            descFormat="2,100000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [168]=
    {
        name        = "^00ff00玄天玉薄（一级）^00ff00",
        desc        = "        合欢贡献度达到500万领取炙鹰令\r        河阳 唐婉儿处（仅合欢职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄天玉薄（一级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={4,5000000,},
--            descFormat="4,5000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [169]=
    {
        name        = "^00ff00中天灯蕊（二级）^00ff00",
        desc        = "        合欢贡献度达到1000万领取炙鹰令\r        河阳 唐婉儿处（仅合欢职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00中天灯蕊（二级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={4,10000000,},
--            descFormat="4,10000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [170]=
    {
        name        = "^00ff00羡天宝烛（三级）^00ff00",
        desc        = "        合欢贡献度达到2000万领取炙鹰令\r        河阳 唐婉儿处（仅合欢职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00羡天宝烛（三级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={4,20000000,},
--            descFormat="4,20000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [171]=
    {
        name        = "^00ff00从天纂录（四级）^00ff00",
        desc        = "        合欢贡献度达到3000万领取炙鹰令\r        河阳 唐婉儿处（仅合欢职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00从天纂录（四级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={4,30000000,},
--            descFormat="4,30000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [172]=
    {
        name        = "^00ff00更天金钵（五级）^00ff00",
        desc        = "        合欢贡献度达到4000万领取炙鹰令\r        河阳 唐婉儿处（仅合欢职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00更天金钵（五级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={4,40000000,},
--            descFormat="4,40000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [173]=
    {
        name        = "^00ff00翠天善果（六级）^00ff00",
        desc        = "        合欢贡献度达到5000万领取炙鹰令\r        河阳 唐婉儿处（仅合欢职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00翠天善果（六级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={4,50000000,},
--            descFormat="4,50000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [174]=
    {
        name        = "^00ff00廓天柒木（七级）^00ff00",
        desc        = "        合欢贡献度达到6000万领取炙鹰令\r        河阳 唐婉儿处（仅合欢职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00廓天柒木（七级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={4,60000000,},
--            descFormat="4,60000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [175]=
    {
        name        = "^00ff00减天罪竹（八级）^00ff00",
        desc        = "        合欢贡献度达到7000万领取炙鹰令\r        河阳 唐婉儿处（仅合欢职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00减天罪竹（八级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={4,70000000,},
--            descFormat="4,70000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [176]=
    {
        name        = "^00ff00沈天泣佞（九级）^00ff00",
        desc        = "        合欢贡献度达到8000万领取炙鹰令\r        河阳 唐婉儿处（仅合欢职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00沈天泣佞（九级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={4,80000000,},
--            descFormat="4,80000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [177]=
    {
        name        = "^00ff00成天正果（十级）^00ff00",
        desc        = "        合欢贡献度达到1亿领取炙鹰令\r        河阳 唐婉儿处 （仅合欢职业）",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00成天正果（十级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={4,100000000,},
--            descFormat="4,100000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [178]=
    {
        name        = "^00ff00玄天玉薄（一级）^00ff00",
        desc        = "        鬼道贡献度达到500万领取炙鹰令\r        河阳 常在处（仅鬼道职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄天玉薄（一级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={0,5000000,},
--            descFormat="0,5000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [179]=
    {
        name        = "^00ff00中天灯蕊（二级）^00ff00",
        desc        = "        鬼道贡献度达到1000万领取炙鹰令\r        河阳 常在处（仅鬼道职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00中天灯蕊（二级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={0,10000000,},
--            descFormat="0,10000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [180]=
    {
        name        = "^00ff00羡天宝烛（三级）^00ff00",
        desc        = "        鬼道贡献度达到2000万领取炙鹰令\r        河阳 常在处（仅鬼道职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00羡天宝烛（三级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={0,20000000,},
--            descFormat="0,20000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [181]=
    {
        name        = "^00ff00从天纂录（四级）^00ff00",
        desc        = "        鬼道贡献度达到3000万领取炙鹰令\r        河阳 常在处（仅鬼道职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00从天纂录（四级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={0,30000000,},
--            descFormat="0,30000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [182]=
    {
        name        = "^00ff00更天金钵（五级）^00ff00",
        desc        = "        鬼道贡献度达到4000万领取炙鹰令\r        河阳 常在处（仅鬼道职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00更天金钵（五级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={0,40000000,},
--            descFormat="0,40000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [183]=
    {
        name        = "^00ff00翠天善果（六级）^00ff00",
        desc        = "        鬼道贡献度达到5000万领取炙鹰令\r        河阳 常在处（仅鬼道职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00翠天善果（六级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={0,50000000,},
--            descFormat="0,50000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [184]=
    {
        name        = "^00ff00廓天柒木（七级）^00ff00",
        desc        = "        鬼道贡献度达到6000万领取炙鹰令\r        河阳 常在处（仅鬼道职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00廓天柒木（七级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={0,60000000,},
--            descFormat="0,60000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [185]=
    {
        name        = "^00ff00减天罪竹（八级）^00ff00",
        desc        = "        鬼道贡献度达到7000万领取炙鹰令\r        河阳 常在处（仅鬼道职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00减天罪竹（八级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={0,70000000,},
--            descFormat="0,70000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [186]=
    {
        name        = "^00ff00沈天泣佞（九级）^00ff00",
        desc        = "        鬼道贡献度达到8000万领取炙鹰令\r        河阳 常在处（仅鬼道职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00沈天泣佞（九级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={0,80000000,},
--            descFormat="0,80000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [187]=
    {
        name        = "^00ff00成天正果（十级）^00ff00",
        desc        = "        鬼道贡献度达到1亿领取炙鹰令\r        河阳 常在处（仅鬼道职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00成天正果（十级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={0,100000000,},
--            descFormat="0,100000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [188]=
    {
        name        = "^00ff00玄天玉薄（一级）^00ff00",
        desc        = "        九黎贡献度达到500万领取炙鹰令\r        神域 姜虞处（仅九黎职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄天玉薄（一级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={13,5000000,},
--            descFormat="13,5000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [189]=
    {
        name        = "^00ff00中天灯蕊（二级）^00ff00",
        desc        = "        九黎贡献度达到1000万领取炙鹰令\r        神域 姜虞处（仅九黎职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00中天灯蕊（二级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={13,10000000,},
--            descFormat="13,10000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [190]=
    {
        name        = "^00ff00羡天宝烛（三级）^00ff00",
        desc        = "        九黎贡献度达到2000万领取炙鹰令\r        神域 姜虞处（仅九黎职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00羡天宝烛（三级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={13,20000000,},
--            descFormat="13,20000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [191]=
    {
        name        = "^00ff00从天纂录（四级）^00ff00",
        desc        = "        九黎贡献度达到3000万领取炙鹰令\r        神域 姜虞处（仅九黎职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00从天纂录（四级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={13,30000000,},
--            descFormat="13,30000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [192]=
    {
        name        = "^00ff00更天金钵（五级）^00ff00",
        desc        = "        九黎贡献度达到4000万领取炙鹰令\r        神域 姜虞处（仅九黎职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00更天金钵（五级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={13,40000000,},
--            descFormat="13,40000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [193]=
    {
        name        = "^00ff00翠天善果（六级）^00ff00",
        desc        = "        九黎贡献度达到5000万领取炙鹰令\r        神域 姜虞处（仅九黎职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00翠天善果（六级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={13,50000000,},
--            descFormat="13,50000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [194]=
    {
        name        = "^00ff00廓天柒木（七级）^00ff00",
        desc        = "        九黎贡献度达到6000万领取炙鹰令\r        神域 姜虞处（仅九黎职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00廓天柒木（七级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={13,60000000,},
--            descFormat="13,60000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [195]=
    {
        name        = "^00ff00减天罪竹（八级）^00ff00",
        desc        = "        九黎贡献度达到7000万领取炙鹰令\r        神域 姜虞处（仅九黎职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00减天罪竹（八级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={13,70000000,},
--            descFormat="13,70000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [196]=
    {
        name        = "^00ff00沈天泣佞（九级）^00ff00",
        desc        = "        九黎贡献度达到8000万领取炙鹰令\r        神域 姜虞处（仅九黎职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00沈天泣佞（九级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={13,80000000,},
--            descFormat="13,80000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [197]=
    {
        name        = "^00ff00成天正果（十级）^00ff00",
        desc        = "        九黎贡献度达到1亿领取炙鹰令\r        神域 姜虞处（仅九黎职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00成天正果（十级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={13,100000000,},
--            descFormat="13,100000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [198]=
    {
        name        = "^00ff00玄天玉薄（一级）^00ff00",
        desc        = "        烈山贡献度达到500万领取炙鹰令\r        神域 黎恂处（仅烈山职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄天玉薄（一级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={14,5000000,},
--            descFormat="14,5000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [199]=
    {
        name        = "^00ff00中天灯蕊（二级）^00ff00",
        desc        = "        烈山贡献度达到1000万领取炙鹰令\r        神域 黎恂处（仅烈山职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00中天灯蕊（二级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={14,10000000,},
--            descFormat="14,10000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [200]=
    {
        name        = "^00ff00羡天宝烛（三级）^00ff00",
        desc        = "        烈山贡献度达到2000万领取炙鹰令\r        神域 黎恂处（仅烈山职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00羡天宝烛（三级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={14,20000000,},
--            descFormat="14,20000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [201]=
    {
        name        = "^00ff00从天纂录（四级）^00ff00",
        desc        = "        烈山贡献度达到3000万领取炙鹰令\r        神域 黎恂处（仅烈山职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00从天纂录（四级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={14,30000000,},
--            descFormat="14,30000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [202]=
    {
        name        = "^00ff00更天金钵（五级）^00ff00",
        desc        = "        烈山贡献度达到4000万领取炙鹰令\r        神域 黎恂处（仅烈山职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00更天金钵（五级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={14,40000000,},
--            descFormat="14,40000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [203]=
    {
        name        = "^00ff00翠天善果（六级）^00ff00",
        desc        = "        烈山贡献度达到5000万领取炙鹰令\r        神域 黎恂处（仅烈山职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00翠天善果（六级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={14,50000000,},
--            descFormat="14,50000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [204]=
    {
        name        = "^00ff00廓天柒木（七级）^00ff00",
        desc        = "        烈山贡献度达到6000万领取炙鹰令\r        神域 黎恂处（仅烈山职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00廓天柒木（七级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={14,60000000,},
--            descFormat="14,60000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [205]=
    {
        name        = "^00ff00减天罪竹（八级）^00ff00",
        desc        = "        烈山贡献度达到7000万领取炙鹰令\r        神域 黎恂处（仅烈山职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00减天罪竹（八级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={14,70000000,},
--            descFormat="14,70000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [206]=
    {
        name        = "^00ff00沈天泣佞（九级）^00ff00",
        desc        = "        烈山贡献度达到8000万领取炙鹰令\r        神域 黎恂处（仅烈山职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00沈天泣佞（九级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={14,80000000,},
--            descFormat="14,80000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [207]=
    {
        name        = "^00ff00成天正果（十级）^00ff00",
        desc        = "        烈山贡献度达到1亿领取炙鹰令\r        神域 黎恂处（仅烈山职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00成天正果（十级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={14,100000000,},
--            descFormat="14,100000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [208]=
    {
        name        = "^00ff00九黎氏供奉^00ff00",
        desc        = "        九黎贡献度达到2亿",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00九黎氏供奉^00ff00>",
        conds=
        {
            {
              id=3,
              params={13,200000000,},
--            descFormat="13,200000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [209]=
    {
        name        = "^00ff00烈山氏供奉^00ff00",
        desc        = "        烈山贡献度达到2亿",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00烈山氏供奉^00ff00>",
        conds=
        {
            {
              id=3,
              params={14,200000000,},
--            descFormat="14,200000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [210]=
    {
        name        = "^00ff00挑战·混沌力王^00ff00",
        desc        = "        在天空之城中击杀混沌力王1000次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 6,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00挑战·混沌力王^00ff00>",
        conds=
        {
            {
              id=1001,
              params={31319,1000,},
--            descFormat="31319,1000,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [211]=
    {
        name        = "^00ff00挑战·鸿蒙大圣^00ff00",
        desc        = "        在天空之城中击杀鸿蒙大圣1000次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 6,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00挑战·鸿蒙大圣^00ff00>",
        conds=
        {
            {
              id=1001,
              params={31320,1000,},
--            descFormat="31320,1000,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [212]=
    {
        name        = "^00ff00挑战·焰金兽^00ff00",
        desc        = "        在天空之城中击杀焰金兽1000次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 6,
        awardScore  = 80,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00挑战·焰金兽^00ff00>",
        conds=
        {
            {
              id=1001,
              params={31321,1000,},
--            descFormat="31321,1000,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [213]=
    {
        name        = "^00ff00挑战·化蛇^00ff00",
        desc        = "        在天空之城中击杀化蛇1000次",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 6,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00挑战·化蛇^00ff00>",
        conds=
        {
            {
              id=1001,
              params={31322,1000,},
--            descFormat="31322,1000,", 
              infoFormat = "击杀 %s (%d/%d)次",
            },
        }, 
    },

    [214]=
    {
        name        = "^00ff00造化之太乙^00ff00",
        desc        = "        获取造化之太乙\r        （装备饰品达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00造化之太乙^00ff00>",
        conds=
        {
            {
              id=11,
              params={25727,},
--            descFormat="25727,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [215]=
    {
        name        = "^00ff00造化之太昊^00ff00",
        desc        = "        获取造化之太昊\r        （装备饰品达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00造化之太昊^00ff00>",
        conds=
        {
            {
              id=11,
              params={25724,},
--            descFormat="25724,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [216]=
    {
        name        = "^00ff00火神·祝融真炎^00ff00",
        desc        = "        获取神级·祝融套装饰品\r        （装备项链及戒指达成条件）   ",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 2,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00火神·祝融真炎^00ff00>",
        conds=
        {
            {
              id=11,
              params={32990,},
--            descFormat="32990,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={32979,},
--            descFormat="32979,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={32980,},
--            descFormat="32980,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [217]=
    {
        name        = "^00ff00水神·共工狂澜^00ff00",
        desc        = "        获取神级·共工套装饰品\r        （装备项链及戒指达成条件）   ",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 2,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00水神·共工狂澜^00ff00>",
        conds=
        {
            {
              id=11,
              params={32988,},
--            descFormat="32988,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={32983,},
--            descFormat="32983,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={32984,},
--            descFormat="32984,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [218]=
    {
        name        = "^00ff00风神·风伯天飚^00ff00",
        desc        = "        获取神级·风伯套装饰品\r        （装备项链及戒指达成条件）   ",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 2,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00风神·风伯天飚^00ff00>",
        conds=
        {
            {
              id=11,
              params={32989,},
--            descFormat="32989,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={32975,},
--            descFormat="32975,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={32976,},
--            descFormat="32976,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [219]=
    {
        name        = "^00ff00雨神·雨师煌雷^00ff00",
        desc        = "        获取神级·雨师套装饰品\r        （装备项链及戒指达成条件）   ",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 2,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00雨神·雨师煌雷^00ff00>",
        conds=
        {
            {
              id=11,
              params={32987,},
--            descFormat="32987,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={32977,},
--            descFormat="32977,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={32978,},
--            descFormat="32978,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [220]=
    {
        name        = "^00ff00战神·刑天震怒^00ff00",
        desc        = "        获取神级·刑天套装饰品\r        （装备项链及戒指达成条件）   ",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 2,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00战神·刑天震怒^00ff00>",
        conds=
        {
            {
              id=11,
              params={32991,},
--            descFormat="32991,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={32981,},
--            descFormat="32981,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={32982,},
--            descFormat="32982,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [221]=
    {
        name        = "^00ff00琴帝·帝俊情心^00ff00",
        desc        = "        获取神级·帝俊套装饰品\r        （装备项链及戒指达成条件）   ",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 2,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00琴帝·帝俊情心^00ff00>",
        conds=
        {
            {
              id=11,
              params={32992,},
--            descFormat="32992,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={32985,},
--            descFormat="32985,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={32986,},
--            descFormat="32986,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [222]=
    {
        name        = "^00ff00筑基战罡^00ff00",
        desc        = "        获取筑基战罡\r        （装备罡气达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00筑基战罡^00ff00>",
        conds=
        {
            {
              id=11,
              params={33025,},
--            descFormat="33025,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [223]=
    {
        name        = "^00ff00和合战罡^00ff00",
        desc        = "        获取和合战罡\r        （装备罡气达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00和合战罡^00ff00>",
        conds=
        {
            {
              id=11,
              params={33026,},
--            descFormat="33026,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [224]=
    {
        name        = "^00ff00辟谷战罡^00ff00",
        desc        = "        获取辟谷战罡\r        （装备罡气达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 4,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00辟谷战罡^00ff00>",
        conds=
        {
            {
              id=11,
              params={33027,},
--            descFormat="33027,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [225]=
    {
        name        = "^00ff00元婴战罡^00ff00",
        desc        = "        获取元婴战罡\r        （装备罡气达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 4,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00元婴战罡^00ff00>",
        conds=
        {
            {
              id=11,
              params={33028,},
--            descFormat="33028,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [226]=
    {
        name        = "^00ff00混沌主宰^00ff00",
        desc        = "        获得幻灵石·混沌主宰\r        （装备幻灵石达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 6,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00混沌主宰^00ff00>",
        conds=
        {
            {
              id=11,
              params={32954,},
--            descFormat="32954,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [227]=
    {
        name        = "^00ff00空冥战罡^00ff00",
        desc        = "        获取空冥战罡\r        （装备罡气达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 4,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00空冥战罡^00ff00>",
        conds=
        {
            {
              id=11,
              params={33029,},
--            descFormat="33029,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [228]=
    {
        name        = "^00ff00渡劫战罡^00ff00",
        desc        = "        获取渡劫战罡\r        （装备罡气达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 4,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00渡劫战罡^00ff00>",
        conds=
        {
            {
              id=11,
              params={33030,},
--            descFormat="33030,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [229]=
    {
        name        = "^00ff00寂灭战罡^00ff00",
        desc        = "        获取寂灭战罡\r        （装备罡气达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00寂灭战罡^00ff00>",
        conds=
        {
            {
              id=11,
              params={33031,},
--            descFormat="33031,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [230]=
    {
        name        = "^00ff00大乘战罡^00ff00",
        desc        = "        获取大乘战罡\r        （装备罡气达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00大乘战罡^00ff00>",
        conds=
        {
            {
              id=11,
              params={33032,},
--            descFormat="33032,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [231]=
    {
        name        = "^00ff00昆仑守护者^00ff00",
        desc        = "        击杀出现在昆仑的Boss <冥界·古猿>",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00昆仑守护者^00ff00>",
        conds=
        {
            {
              id=4,
              params={35239,},
--            descFormat="35239,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [232]=
    {
        name        = "^00ff00南疆守护者^00ff00",
        desc        = "        击杀出现在南疆的Boss <冥界·血狮>",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00南疆守护者^00ff00>",
        conds=
        {
            {
              id=4,
              params={35238,},
--            descFormat="35238,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [233]=
    {
        name        = "^00ff00焚香守护者^00ff00",
        desc        = "        击杀出现在焚香的Boss <冥界·圣使>",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00焚香守护者^00ff00>",
        conds=
        {
            {
              id=4,
              params={35241,},
--            descFormat="35241,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [234]=
    {
        name        = "^00ff00蛮荒守护者^00ff00",
        desc        = "        击杀出现在蛮荒的Boss <冥界·天狼>",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00蛮荒守护者^00ff00>",
        conds=
        {
            {
              id=4,
              params={35236,},
--            descFormat="35236,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [235]=
    {
        name        = "^00ff00死泽守护者^00ff00",
        desc        = "        击杀出现在死泽的Boss <冥界·玄鸟>",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00死泽守护者^00ff00>",
        conds=
        {
            {
              id=4,
              params={35240,},
--            descFormat="35240,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [236]=
    {
        name        = "^00ff00流波守护者^00ff00",
        desc        = "        击杀出现在流波的Boss <冥界·饕餮>",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00流波守护者^00ff00>",
        conds=
        {
            {
              id=4,
              params={35242,},
--            descFormat="35242,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [237]=
    {
        name        = "^00ff00天音守护者^00ff00",
        desc        = "        击杀出现在天音的Boss <冥界·兽神>",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 4,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天音守护者^00ff00>",
        conds=
        {
            {
              id=4,
              params={35243,},
--            descFormat="35243,", 
              infoFormat = "击杀 %s (%d/1)",
            },
        }, 
    },

    [238]=
    {
        name        = "^00ff00玄天玉薄（一级）^00ff00",
        desc        = "        怀光贡献度达到500万领取炙鹰令\r        神域 怀瞳处（仅怀光职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄天玉薄（一级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={17,5000000,},
--            descFormat="17,5000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [239]=
    {
        name        = "^00ff00中天灯蕊（二级）^00ff00",
        desc        = "        怀光贡献度达到1000万领取炙鹰令\r        神域 怀瞳处（仅怀光职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00中天灯蕊（二级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={17,10000000,},
--            descFormat="17,10000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [240]=
    {
        name        = "^00ff00羡天宝烛（三级）^00ff00",
        desc        = "        怀光贡献度达到2000万领取炙鹰令\r        神域 怀瞳处（仅怀光职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00羡天宝烛（三级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={17,20000000,},
--            descFormat="17,20000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [241]=
    {
        name        = "^00ff00从天纂录（四级）^00ff00",
        desc        = "        怀光贡献度达到3000万领取炙鹰令\r        神域 怀瞳处（仅怀光职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00从天纂录（四级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={17,30000000,},
--            descFormat="17,30000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [242]=
    {
        name        = "^00ff00更天金钵（五级）^00ff00",
        desc        = "        怀光贡献度达到4000万领取炙鹰令\r        神域 怀瞳处（仅怀光职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00更天金钵（五级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={17,40000000,},
--            descFormat="17,40000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [243]=
    {
        name        = "^00ff00翠天善果（六级）^00ff00",
        desc        = "        怀光贡献度达到5000万领取炙鹰令\r        神域 怀瞳处（仅怀光职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00翠天善果（六级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={17,50000000,},
--            descFormat="17,50000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [244]=
    {
        name        = "^00ff00廓天柒木（七级）^00ff00",
        desc        = "        怀光贡献度达到6000万领取炙鹰令\r        神域 怀瞳处（仅怀光职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00廓天柒木（七级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={17,60000000,},
--            descFormat="17,60000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [245]=
    {
        name        = "^00ff00减天罪竹（八级）^00ff00",
        desc        = "        怀光贡献度达到7000万领取炙鹰令\r        神域 怀瞳处（仅怀光职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00减天罪竹（八级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={17,70000000,},
--            descFormat="17,70000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [246]=
    {
        name        = "^00ff00沈天泣佞（九级）^00ff00",
        desc        = "        怀光贡献度达到8000万领取炙鹰令\r        神域 怀瞳处（仅怀光职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00沈天泣佞（九级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={17,80000000,},
--            descFormat="17,80000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [247]=
    {
        name        = "^00ff00成天正果（十级）^00ff00",
        desc        = "        怀光贡献度达到1亿领取炙鹰令\r        神域 怀瞳处（仅怀光职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00成天正果（十级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={17,100000000,},
--            descFormat="17,100000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [248]=
    {
        name        = "^00ff00玄天玉薄（一级）^00ff00",
        desc        = "        天华贡献度达到500万领取炙鹰令\r        神域 兰羽儿处（仅天华职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄天玉薄（一级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={18,5000000,},
--            descFormat="18,5000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [249]=
    {
        name        = "^00ff00中天灯蕊（二级）^00ff00",
        desc        = "        天华贡献度达到1000万领取炙鹰令\r        神域 兰羽儿处（仅天华职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00中天灯蕊（二级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={18,10000000,},
--            descFormat="18,10000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [250]=
    {
        name        = "^00ff00羡天宝烛（三级）^00ff00",
        desc        = "        天华贡献度达到2000万领取炙鹰令\r        神域 兰羽儿处（仅天华职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00羡天宝烛（三级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={18,20000000,},
--            descFormat="18,20000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [251]=
    {
        name        = "^00ff00从天纂录（四级）^00ff00",
        desc        = "        天华贡献度达到3000万领取炙鹰令\r        神域 兰羽儿处（仅天华职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00从天纂录（四级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={18,30000000,},
--            descFormat="18,30000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [252]=
    {
        name        = "^00ff00更天金钵（五级）^00ff00",
        desc        = "        天华贡献度达到4000万领取炙鹰令\r        神域 兰羽儿处（仅天华职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00更天金钵（五级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={18,40000000,},
--            descFormat="18,40000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [253]=
    {
        name        = "^00ff00翠天善果（六级）^00ff00",
        desc        = "        天华贡献度达到5000万领取炙鹰令\r        神域 兰羽儿处（仅天华职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00翠天善果（六级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={18,50000000,},
--            descFormat="18,50000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [254]=
    {
        name        = "^00ff00廓天柒木（七级）^00ff00",
        desc        = "        天华贡献度达到6000万领取炙鹰令\r        神域 兰羽儿处（仅天华职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00廓天柒木（七级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={18,60000000,},
--            descFormat="18,60000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [255]=
    {
        name        = "^00ff00减天罪竹（八级）^00ff00",
        desc        = "        天华贡献度达到7000万领取炙鹰令\r        神域 兰羽儿处（仅天华职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00减天罪竹（八级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={18,70000000,},
--            descFormat="18,70000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [256]=
    {
        name        = "^00ff00沈天泣佞（九级）^00ff00",
        desc        = "        天华贡献度达到8000万领取炙鹰令\r        神域 兰羽儿处（仅天华职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00沈天泣佞（九级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={18,80000000,},
--            descFormat="18,80000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [257]=
    {
        name        = "^00ff00成天正果（十级）^00ff00",
        desc        = "        天华贡献度达到1亿领取炙鹰令\r        神域 兰羽儿处（仅天华职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00成天正果（十级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={18,100000000,},
--            descFormat="18,100000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [258]=
    {
        name        = "^00ff00天华氏供奉^00ff00",
        desc        = "        天华贡献度达到2亿",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天华氏供奉^00ff00>",
        conds=
        {
            {
              id=3,
              params={18,200000000,},
--            descFormat="18,200000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [259]=
    {
        name        = "^00ff00怀光氏供奉^00ff00",
        desc        = "        怀光贡献度达到2亿",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00怀光氏供奉^00ff00>",
        conds=
        {
            {
              id=3,
              params={17,200000000,},
--            descFormat="17,200000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [260]=
    {
        name        = "^00ff00鬼厉装（荣耀版）收集^00ff00",
        desc        = "        收集一套鬼厉装（荣耀版）\r        （穿戴时装达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00鬼厉装（荣耀版）收集^00ff00>",
        conds=
        {
            {
              id=11,
              params={44149,},
--            descFormat="44149,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={44150,},
--            descFormat="44150,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={44151,},
--            descFormat="44151,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [261]=
    {
        name        = "^00ff00碧瑶装（痴情版）收集^00ff00",
        desc        = "        收集一套碧瑶装（痴情版）\r        （穿戴时装达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00碧瑶装（痴情版）收集^00ff00>",
        conds=
        {
            {
              id=11,
              params={42881,},
--            descFormat="42881,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={42884,},
--            descFormat="42884,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={42886,},
--            descFormat="42886,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [262]=
    {
        name        = "^00ff00雪琪装（真情版）收集^00ff00",
        desc        = "        收集一套雪琪装（真情版）\r        （穿戴时装达成条件）",
        preCondDesc = "空",
        mainClass   = 4,
        subClass    = 1,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00雪琪装（真情版）收集^00ff00>",
        conds=
        {
            {
              id=11,
              params={42771,},
--            descFormat="42771,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={42772,},
--            descFormat="42772,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
            {
              id=11,
              params={42773,},
--            descFormat="42773,", 
              infoFormat = "获取并装备 %s (%d/1)",
            },
        }, 
    },

    [263]=
    {
        name        = "^00ff00玄天玉薄（一级）^00ff00",
        desc        = "        焚香贡献度达到500万领取炙鹰令\r        河阳 云泰来处（仅焚香职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄天玉薄（一级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={19,5000000,},
--            descFormat="19,5000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [264]=
    {
        name        = "^00ff00中天灯蕊（二级）^00ff00",
        desc        = "        焚香贡献度达到1000万领取炙鹰令\r        河阳 云泰来处（仅焚香职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00中天灯蕊（二级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={19,10000000,},
--            descFormat="19,10000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [265]=
    {
        name        = "^00ff00羡天宝烛（三级）^00ff00",
        desc        = "        焚香贡献度达到2000万领取炙鹰令\r        河阳 云泰来处（仅焚香职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00羡天宝烛（三级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={19,20000000,},
--            descFormat="19,20000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [266]=
    {
        name        = "^00ff00从天纂录（四级）^00ff00",
        desc        = "        焚香贡献度达到3000万领取炙鹰令\r        河阳 云泰来处（仅焚香职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00从天纂录（四级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={19,30000000,},
--            descFormat="19,30000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [267]=
    {
        name        = "^00ff00更天金钵（五级）^00ff00",
        desc        = "        焚香贡献度达到4000万领取炙鹰令\r        河阳 云泰来处（仅焚香职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00更天金钵（五级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={19,40000000,},
--            descFormat="19,40000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [268]=
    {
        name        = "^00ff00翠天善果（六级）^00ff00",
        desc        = "        焚香贡献度达到5000万领取炙鹰令\r        河阳 云泰来处（仅焚香职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00翠天善果（六级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={19,50000000,},
--            descFormat="19,50000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [269]=
    {
        name        = "^00ff00廓天柒木（七级）^00ff00",
        desc        = "        焚香贡献度达到6000万领取炙鹰令\r        河阳 云泰来处（仅焚香职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00廓天柒木（七级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={19,60000000,},
--            descFormat="19,60000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [270]=
    {
        name        = "^00ff00减天罪竹（八级）^00ff00",
        desc        = "        焚香贡献度达到7000万领取炙鹰令\r        河阳 云泰来处（仅焚香职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00减天罪竹（八级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={19,70000000,},
--            descFormat="19,70000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [271]=
    {
        name        = "^00ff00沈天泣佞（九级）^00ff00",
        desc        = "        焚香贡献度达到8000万领取炙鹰令\r        河阳 云泰来处（仅焚香职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00沈天泣佞（九级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={19,80000000,},
--            descFormat="19,80000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [272]=
    {
        name        = "^00ff00成天正果（十级）^00ff00",
        desc        = "        焚香贡献度达到1亿领取炙鹰令\r        河阳 云泰来处（仅焚香职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 2,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00成天正果（十级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={19,100000000,},
--            descFormat="19,100000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [273]=
    {
        name        = "^00ff00玄天玉薄（一级）^00ff00",
        desc        = "        太昊贡献度达到500万领取炙鹰令\r        神域 百里尉迟处（仅太昊职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄天玉薄（一级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={16,5000000,},
--            descFormat="16,5000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [274]=
    {
        name        = "^00ff00中天灯蕊（二级）^00ff00",
        desc        = "        太昊贡献度达到1000万领取炙鹰令\r        神域 百里尉迟处（仅太昊职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00中天灯蕊（二级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={16,10000000,},
--            descFormat="16,10000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [275]=
    {
        name        = "^00ff00羡天宝烛（三级）^00ff00",
        desc        = "        太昊贡献度达到2000万领取炙鹰令\r        神域 百里尉迟处（仅太昊职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00羡天宝烛（三级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={16,20000000,},
--            descFormat="16,20000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [276]=
    {
        name        = "^00ff00从天纂录（四级）^00ff00",
        desc        = "        太昊贡献度达到3000万领取炙鹰令\r        神域 百里尉迟处（仅太昊职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00从天纂录（四级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={16,30000000,},
--            descFormat="16,30000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [277]=
    {
        name        = "^00ff00更天金钵（五级）^00ff00",
        desc        = "        太昊贡献度达到4000万领取炙鹰令\r        神域 百里尉迟处（仅太昊职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00更天金钵（五级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={16,40000000,},
--            descFormat="16,40000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [278]=
    {
        name        = "^00ff00翠天善果（六级）^00ff00",
        desc        = "        太昊贡献度达到5000万领取炙鹰令\r        神域 百里尉迟处（仅太昊职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00翠天善果（六级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={16,50000000,},
--            descFormat="16,50000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [279]=
    {
        name        = "^00ff00廓天柒木（七级）^00ff00",
        desc        = "        太昊贡献度达到6000万领取炙鹰令\r        神域 百里尉迟处（仅太昊职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00廓天柒木（七级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={16,60000000,},
--            descFormat="16,60000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [280]=
    {
        name        = "^00ff00减天罪竹（八级）^00ff00",
        desc        = "        太昊贡献度达到7000万领取炙鹰令\r        神域 百里尉迟处（仅太昊职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00减天罪竹（八级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={16,70000000,},
--            descFormat="16,70000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [281]=
    {
        name        = "^00ff00沈天泣佞（九级）^00ff00",
        desc        = "        太昊贡献度达到8000万领取炙鹰令\r        神域 百里尉迟处（仅太昊职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00沈天泣佞（九级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={16,80000000,},
--            descFormat="16,80000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [282]=
    {
        name        = "^00ff00成天正果（十级）^00ff00",
        desc        = "        太昊贡献度达到1亿领取炙鹰令\r        神域 百里尉迟处（仅太昊职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00成天正果（十级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={16,100000000,},
--            descFormat="16,100000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [283]=
    {
        name        = "^00ff00焚香谷名宿^00ff00",
        desc        = "        焚香贡献度达到2亿",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00焚香谷名宿^00ff00>",
        conds=
        {
            {
              id=3,
              params={19,200000000,},
--            descFormat="19,200000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [284]=
    {
        name        = "^00ff00太昊氏供奉^00ff00",
        desc        = "        太昊贡献度达到2亿",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00太昊氏供奉^00ff00>",
        conds=
        {
            {
              id=3,
              params={16,200000000,},
--            descFormat="16,200000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [285]=
    {
        name        = "^00ff00玄天玉薄（一级）^00ff00",
        desc        = "        辰皇贡献度达到500万领取炙鹰令\r        神域 乐正先处（仅辰皇职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00玄天玉薄（一级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={15,5000000,},
--            descFormat="15,5000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [286]=
    {
        name        = "^00ff00中天灯蕊（二级）^00ff00",
        desc        = "        辰皇贡献度达到1000万领取炙鹰令\r        神域 乐正先处（仅辰皇职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00中天灯蕊（二级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={15,10000000,},
--            descFormat="15,10000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [287]=
    {
        name        = "^00ff00羡天宝烛（三级）^00ff00",
        desc        = "        辰皇贡献度达到2000万领取炙鹰令\r        神域 乐正先处（仅辰皇职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00羡天宝烛（三级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={15,20000000,},
--            descFormat="15,20000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [288]=
    {
        name        = "^00ff00从天纂录（四级）^00ff00",
        desc        = "        辰皇贡献度达到3000万领取炙鹰令\r        神域 乐正先处（仅辰皇职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00从天纂录（四级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={15,30000000,},
--            descFormat="15,30000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [289]=
    {
        name        = "^00ff00更天金钵（五级）^00ff00",
        desc        = "        辰皇贡献度达到4000万领取炙鹰令\r        神域 乐正先处（仅辰皇职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00更天金钵（五级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={15,40000000,},
--            descFormat="15,40000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [290]=
    {
        name        = "^00ff00翠天善果（六级）^00ff00",
        desc        = "        辰皇贡献度达到5000万领取炙鹰令\r        神域 乐正先处（仅辰皇职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00翠天善果（六级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={15,50000000,},
--            descFormat="15,50000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [291]=
    {
        name        = "^00ff00廓天柒木（七级）^00ff00",
        desc        = "        辰皇贡献度达到6000万领取炙鹰令\r        神域 乐正先处（仅辰皇职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00廓天柒木（七级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={15,60000000,},
--            descFormat="15,60000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [292]=
    {
        name        = "^00ff00减天罪竹（八级）^00ff00",
        desc        = "        辰皇贡献度达到7000万领取炙鹰令\r        神域 乐正先处（仅辰皇职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00减天罪竹（八级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={15,70000000,},
--            descFormat="15,70000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [293]=
    {
        name        = "^00ff00沈天泣佞（九级）^00ff00",
        desc        = "        辰皇贡献度达到8000万领取炙鹰令\r        神域 乐正先处（仅辰皇职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00沈天泣佞（九级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={15,80000000,},
--            descFormat="15,80000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [294]=
    {
        name        = "^00ff00成天正果（十级）^00ff00",
        desc        = "        辰皇贡献度达到1亿领取炙鹰令\r        神域 乐正先处（仅辰皇职业）  ",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 3,
        awardScore  = 50,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00成天正果（十级）^00ff00>",
        conds=
        {
            {
              id=3,
              params={15,100000000,},
--            descFormat="15,100000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [295]=
    {
        name        = "^00ff00辰皇氏供奉^00ff00",
        desc        = "        辰皇贡献度达到2亿",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00辰皇氏供奉^00ff00>",
        conds=
        {
            {
              id=3,
              params={15,200000000,},
--            descFormat="15,200000000,", 
              infoFormat = "声望 %s 达到 %d (%d/1)",
            },
        }, 
    },

    [296]=
    {
        name        = "^00ff00雄姿英发^00ff00",
        desc        = "        一天内活跃度达到100分",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 4,
        awardScore  = 5,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00雄姿英发^00ff00>",
        conds=
        {
            {
              id=16,
              params={0},
--            descFormat="0", 
              infoFormat = "一天内活跃度达到100分",
            },
        }, 
    },

    [297]=
    {
        name        = "^00ff00龙马精神^00ff00",
        desc        = "        连续七天活跃度达到100分",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 4,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00龙马精神^00ff00>",
        conds=
        {
            {
              id=1006,
              params={1,},
--            descFormat="1,", 
              infoFormat = "连续七天活跃度达到100分 (%d/%d)次",
            },
        }, 
    },

    [298]=
    {
        name        = "^00ff00盖世太保^00ff00",
        desc        = "        连续七天活跃度达到100分\r        达成50次",
        preCondDesc = "空",
        mainClass   = 3,
        subClass    = 4,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00盖世太保^00ff00>",
        conds=
        {
            {
              id=1006,
              params={50,},
--            descFormat="50,", 
              infoFormat = "连续七天活跃度达到100分 (%d/%d)次",
            },
        }, 
    },
    [299]=
    {
        name        = "^00ff00法天象地^00ff00",
        desc        = "        飞升 人物等级达到160级",
        preCondDesc = "空",
        mainClass   = 1,
        subClass    = 1,
        awardScore  = 100,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00法天象地^00ff00>",
        conds=
        {
            {
              id=1,
              params={1,160,},
--            descFormat="1,160,", 
--            infoFormat = "您的%s等级达到 %d (%d/1)",
            },
        }, 
    },

    [300]=
    {
        name        = "^00ff00天界演武-大杀特杀^00ff00",
        desc        = "        在天界演武中连杀超过15。\r        （自身死亡连杀计数结束）",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-大杀特杀^00ff00>",
        conds=
        {
            {
              id=17,
              params={1,15,},
--            descFormat="1,15,", 
--            infoFormat = "流水席战场击杀玩家 int int",
            },
        }, 
    },

    [301]=
    {
        name        = "^00ff00天界演武-杀人如麻^00ff00",
        desc        = "        在天界演武中连杀超过30。\r        （自身死亡连杀计数结束）",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-杀人如麻^00ff00>",
        conds=
        {
            {
              id=17,
              params={1,30,},
--            descFormat="1,30,", 
--            infoFormat = "流水席战场击杀玩家 int int",
            },
        }, 
    },

    [302]=
    {
        name        = "^00ff00天界演武-无人能挡^00ff00",
        desc        = "        在天界演武中连杀超过50。\r        （自身死亡连杀计数结束）",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-无人能挡^00ff00>",
        conds=
        {
            {
              id=17,
              params={1,50,},
--            descFormat="1,50,", 
--            infoFormat = "流水席战场击杀玩家 int int",
            },
        }, 
    },

    [303]=
    {
        name        = "^00ff00天界演武-超神^00ff00",
        desc        = "        在天界演武中连杀超过100。\r        （自身死亡连杀计数结束）",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 20,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-超神^00ff00>",
        conds=
        {
            {
              id=17,
              params={1,100,},
--            descFormat="1,100,", 
--            infoFormat = "流水席战场击杀玩家 int int",
            },
        }, 
    },

    [304]=
    {
        name        = "^00ff00天界演武-为民除害^00ff00",
        desc        = "        终结一个连杀数在15以上的玩家",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-为民除害^00ff00>",
        conds=
        {
            {
              id=19,
              params={15,},
--            descFormat="15,", 
--            infoFormat = "流水席战场终结多杀 int",
            },
        }, 
    },

    [305]=
    {
        name        = "^00ff00天界演武-维护正义^00ff00",
        desc        = "        终结一个连杀数在30以上的玩家",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-维护正义^00ff00>",
        conds=
        {
            {
              id=19,
              params={30,},
--            descFormat="30,", 
--            infoFormat = "流水席战场终结多杀 int",
            },
        }, 
    },

    [306]=
    {
        name        = "^00ff00天界演武-替天行道^00ff00",
        desc        = "        终结一个连杀数在50以上的玩家",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-替天行道^00ff00>",
        conds=
        {
            {
              id=19,
              params={50,},
--            descFormat="50,", 
--            infoFormat = "流水席战场终结多杀 int",
            },
        }, 
    },

    [307]=
    {
        name        = "^00ff00天界演武-弑神^00ff00",
        desc        = "        终结一个连杀数在100以上的玩家",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 20,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-弑神^00ff00>",
        conds=
        {
            {
              id=19,
              params={100,},
--            descFormat="100,", 
--            infoFormat = "流水席战场终结多杀 int",
            },
        }, 
    },

    [308]=
    {
        name        = "^00ff00天界演武-五杀^00ff00",
        desc        = "        在天界演武中短时间内连续斩杀5名敌人。\r        （10秒后连续计数结束）",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-五杀^00ff00>",
        conds=
        {
            {
              id=17,
              params={2,5,},
--            descFormat="2,5,", 
--            infoFormat = "流水席战场击杀玩家 int int",
            },
        }, 
    },

    [309]=
    {
        name        = "^00ff00天界演武-十杀^00ff00",
        desc        = "        在天界演武中短时间内连续斩杀10名敌人。\r        （10秒后连续计数结束）",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-十杀^00ff00>",
        conds=
        {
            {
              id=17,
              params={2,10,},
--            descFormat="2,10,", 
--            infoFormat = "流水席战场击杀玩家 int int",
            },
        }, 
    },

    [310]=
    {
        name        = "^00ff00天界演武-廿杀^00ff00",
        desc        = "        在天界演武中短时间内连续斩杀20名敌人。\r        （10秒后连续计数结束）",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-廿杀^00ff00>",
        conds=
        {
            {
              id=17,
              params={2,20,},
--            descFormat="2,20,", 
--            infoFormat = "流水席战场击杀玩家 int int",
            },
        }, 
    },

    [311]=
    {
        name        = "^00ff00天界演武-卅杀^00ff00",
        desc        = "        在天界演武中短时间内连续斩杀30名敌人。\r        （10秒后连续计数结束）",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 10,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-卅杀^00ff00>",
        conds=
        {
            {
              id=17,
              params={2,30,},
--            descFormat="2,30,", 
--            infoFormat = "流水席战场击杀玩家 int int",
            },
        }, 
    },

    [312]=
    {
        name        = "^00ff00天界演武-和平主义者^00ff00",
        desc        = "        在一次天界演武中只通过杀怪达成目标",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 20,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-和平主义者^00ff00>",
        conds=
        {
            {
              id=18,
              params={2,},
--            descFormat="2,", 
--            infoFormat = "流水席战场完成 int",
            },
        }, 
    },

    [313]=
    {
        name        = "^00ff00天界演武-杀人狂魔^00ff00",
        desc        = "        在一次天界演武中只通过杀人达成目标",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 20,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-杀人狂魔^00ff00>",
        conds=
        {
            {
              id=18,
              params={1,},
--            descFormat="1,", 
--            infoFormat = "流水席战场完成 int",
            },
        }, 
    },

    [314]=
    {
        name        = "^00ff00天界演武-完美一战^00ff00",
        desc        = "        在一次天界演武中一次不死达成目标",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 20,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-完美一战^00ff00>",
        conds=
        {
            {
              id=18,
              params={3,},
--            descFormat="3,", 
--            infoFormat = "流水席战场完成 int",
            },
        }, 
    },

    [315]=
    {
        name        = "^00ff00天界演武-至尊^00ff00",
        desc        = "        完成以下天界演武成就：",
        preCondDesc = "空",
        mainClass   = 2,
        subClass    = 7,
        awardScore  = 30,
        awardItem   = 0,
        awardMoney  = 0,
        msg = "^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 <^00ff00天界演武-至尊^00ff00>",
        conds=
        {
            {
              id=12,
              params={303,},
--            descFormat="303,", 
--            infoFormat = "达成成就 %s (%d/1)",
            },
            {
              id=12,
              params={307,},
--            descFormat="307,", 
--            infoFormat = "达成成就 %s (%d/1)",
            },
            {
              id=12,
              params={311,},
--            descFormat="311,", 
--            infoFormat = "达成成就 %s (%d/1)",
            },
            {
              id=12,
              params={312,},
--            descFormat="312,", 
--            infoFormat = "达成成就 %s (%d/1)",
            },
            {
              id=12,
              params={313,},
--            descFormat="313,", 
--            infoFormat = "达成成就 %s (%d/1)",
            },
            {
              id=12,
              params={314,},
--            descFormat="314,", 
--            infoFormat = "达成成就 %s (%d/1)",
            },
        }, 
    },

}


