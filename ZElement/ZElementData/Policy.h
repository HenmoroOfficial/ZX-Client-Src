#ifndef _AI_POLICY_H_
#define _AI_POLICY_H_


#include <vector.h>
#include "policytype.h"
#include <stdio.h>
#include <string.h>

#define F_POLICY_VERSION	0
#define F_TRIGGER_VERSION   11
#define F_POLICY_EXP_VERSION 0


//----------------------------------------------------------------------------------------
//CTriggerData
//----------------------------------------------------------------------------------------

class CTriggerData
{

public:	
	CTriggerData()
	{
		rootConditon = 0;
		bActive = false;
		runCondition = run_attack_effect;
	}

	//0-武侠, 1-法师, 2-僧侣, 3-妖精, 4-妖兽, 5-魅灵, 6-羽芒, 7-羽灵
	enum _e_occupation
	{
		o_wuxia			= 1,
		o_fashi			= 2,
		o_senglv		= 4,
		o_yaojing		= 8,
		o_yaoshou		= 16,
		o_meiling		= 32,
		o_yumang		= 64,
		o_yuling		= 128,
	};

	enum _e_condition
	{
		c_time_come = 0,		//指定定时器到时 	参数：定时器ID **只能单独用
		c_hp_less,				//血量少于百分比	参数：血量比例
		c_start_attack,			//战斗开始			参数：无意义   **只能单独用
		c_random,				//随机				参数：随机概率
		c_kill_player,			//杀死玩家　　　　	参数：无意义   **只能单独用
		c_not,					//一个条件的非
		c_or,					//两个条件或
		c_and,					//两个条件与
		c_died,                 //死亡时                           **只能单独用
		c_path_end_point,       //到达路径终点。    参数：有一个路径ID
		c_enmity_reach,			//仇恨列表人数判断  参数：1,人数；2,等级
		c_distance_reach,		//表示怪物触发战斗位置与当前位置的距离，当距离超过该参数时，逻辑表达式为真 参数：距离
		
		c_plus,					//加
		c_minus,                //减
		c_multiply,             //乘
		c_divide,               //除
		c_great,                //大于
		c_less,                 //小于
		c_equ,					//等于
		c_var,                  //变量，           参数：变量ID
		c_constant,             //常量，           参数：int
		c_rank_level,			//人物等级排行榜第X名是否到达Y级 参数：int 名次
		c_born,                 //出生时                           **只能单独用
		c_attack_by_skill,      //受到某ID技能攻击  参数：技能ID

		c_num,
	};
	

	enum _e_target
	{
		t_hate_first = 0,		//仇恨排名第一位
		t_hate_second,			//仇恨排名第二位
		t_hate_others,			//仇恨排名第二位及其以后所有对象中的随机一个
		t_most_hp,				//最多hp
		t_most_mp,				//最多mp
		t_least_hp,				//最少hp
		t_occupation_list,		//某些职业的组合	参数：职业组合表
		t_self,					//自己 
		t_num,
	};

	enum _e_operation
	{
		o_attact = 0,			//发起攻击　	参数：0 物理肉搏 1 物理弓箭 2 魔法类 3 肉搏＋远程
		o_use_skill,			//使用技能　	参数：指定技能和级别
		o_talk,					//说话		    参数：话的文字内容，unicode编码
		o_reset_hate_list,		//重置仇恨列表
		o_run_trigger,			//执行一个新触发器
		o_stop_trigger,			//停止一个触发器	参数：id
		o_active_trigger,		//激活一个触发器	参数：id
		o_create_timer,		    //创建一个定时器	参数：id
		o_kill_timer,			//停止一个定时器	参数：id
		
		o_flee,                 //逃跑
		o_set_hate_to_first,    //将选定目标调整到仇恨列表第一位
		o_set_hate_to_last,     //将选定目标的仇恨度调整到最低
		o_set_hate_fifty_percent,     //仇恨度列表中所有对象的仇恨度减少50%，最少减少到1
		o_skip_operation,       //跳过后面的操作条目
		o_active_controller,
		o_summon,               //新加召唤怪物
		o_trigger_task,         //触发任务事件， param 事件ID
		o_change_path,          //切换路径       param 路径ID 
		o_dispear,              //自消失
		o_sneer_monster,        //嘲讽怪物       param 范围
		o_use_range_skill,      //使用大范围技能 param 技能ID 技能等级 范围（1-512）
		o_reset,				//在"操作类型"中添加"归位"，这种操作将令怪物回到触发战斗的位置，并清除战斗状态（气血回满，buff清空）
		
		o_set_global,           //设置全局变量   param 变量ID 数值
		o_revise_global,        //修正全局变量   param 变量ID 数值
		o_assign_global,        //全局变量赋值   param 被赋值变量ID 赋值变量ID
		o_summon_mineral,       //召唤矿物
		o_drop_item,			//丢出物品
		o_change_hate,			//改变仇恨值 范围可以为正负数 Added 2012-08-21.
		o_start_event,			//开始一个事件	param id 2012-12-26
		o_stop_event,			//停止一个事件	param id
		o_num,
	};
	
	enum _run_condition
	{
		run_attack_noeffect = 0,
		run_attack_effect,
	};
	
    
public:
	
	struct _s_condition
	{
		int iType;
		void *pParam;
	};

	struct _s_target
	{
		int iType;
		void *pParam;
	};

	struct _s_operation
	{
		int iType;
		void *pParam;
		_s_target mTarget;
	};

	struct _s_tree_item
	{
		_s_tree_item(){ mConditon.iType = 0; mConditon.pParam = 0; pLeft = 0; pRight = 0; }
		~_s_tree_item()
		{
			if(mConditon.pParam) free(mConditon.pParam);
			if(pLeft) delete pLeft;
			if(pRight) delete pRight;
		}
		_s_condition mConditon;
		_s_tree_item *pLeft;
		_s_tree_item *pRight;
	};
public:
	
	void			AddOperaion(unsigned int iType, void *pParam, _s_target *pTarget);
	void            AddOperaion(_s_operation*pOperation);
	int				GetOperaionNum(){ return listOperation.size(); }
	int				GetOperaion(unsigned int idx, unsigned int &iType, void **ppData, _s_target& target);
	_s_operation *	GetOperaion( unsigned int idx){ return listOperation[idx]; };
	void			SetOperation(unsigned int idx, unsigned int iType, void *pData, _s_target *pTarget);
	void			DelOperation(unsigned int idx);

	_s_tree_item *	GetConditonRoot();
	void            ReleaseConditionTree();
	void            SetConditionRoot( _s_tree_item *pRoot){ rootConditon = pRoot; }
	unsigned int GetID(){ return uID; }
	void         SetID( unsigned int id){ uID = id; }
	char*        GetName(){ return szName; };
	void         SetName(const char *name){ strcpy(szName,name); }

	
	
	bool        Save(FILE *pFile);
	bool        Load(FILE *pFile);

	
	void Release();
	bool IsActive(){ return bActive; };
	void ActiveTrigger(){ bActive = true; }
	void ToggleTrigger(){ bActive = false; }
	bool IsRun(){ return bRun; }
	void SetRunStatus(bool br){ bRun = br; }
	_run_condition GetRunCondition(){ return (_run_condition)runCondition; }
	void SetRunCondition( _run_condition condition ){  runCondition = condition; };
	
	CTriggerData*        CopyObject();
	static _s_tree_item* CopyConditonTree(_s_tree_item* pRoot);
	static _s_operation* CopyOperation(_s_operation *pOperation);

protected:
	bool				SaveConditonTree(FILE *pFile, _s_tree_item *pNode);
	bool				ReadConditonTree(FILE *pFile, _s_tree_item *pNode);
	
private:

	char         szName[128];
	bool         bActive;
	bool         bRun;
	char         runCondition;
	unsigned int uID;
	_s_tree_item* rootConditon;
	abase::vector<_s_operation *>	listOperation;

};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


class CPolicyData
{

public:

	inline unsigned int GetID(){ return uID; }
	inline void SetID( unsigned int id){ uID = id; }
	
	inline int GetTriggerPtrNum(){ return listTriggerPtr.size(); }
	inline CTriggerData *GetTriggerPtr( int idx ){ return listTriggerPtr[idx]; }
	int			GetIndex( unsigned int id);//如果查找失败返回-1
	inline void SetTriggerPtr( int idx, CTriggerData *pTrigger){ listTriggerPtr[idx] = pTrigger; }
	inline void AddTriggerPtr( CTriggerData *pTrigger){ listTriggerPtr.push_back(pTrigger); }
	void        DelTriggerPtr( int idx);
	bool Save(const char* szPath);
	bool Load(const char* szPath);
	bool Save(FILE *pFile);
	bool Load(FILE *pFile);

	void Release();

private:

	unsigned int uID;
	abase::vector<CTriggerData*> listTriggerPtr;
};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


class CPolicyDataManager
{

public:
	
	inline int GetPolicyNum(){ return listPolicy.size(); }
	inline CPolicyData *GetPolicy( int idx) 
	{
		if( idx >= 0 && idx < (int)listPolicy.size())
			return listPolicy[idx];
		else return 0;
	}
	
	bool Load(const char* szPath);
	bool Save(const char* szPath);
	void Release();

private:
	
	abase::vector<CPolicyData *> listPolicy;
};

#endif
