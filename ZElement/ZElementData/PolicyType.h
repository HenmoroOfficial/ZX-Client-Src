#ifndef _AI_POLICY_TYPE_H_
#define _AI_POLICY_TYPE_H_


#define OP_TEXT_LENGTH 256

//----------------------------------------------------------------------------------------
//Condition
//----------------------------------------------------------------------------------------

struct C_TIME_COME
{
	unsigned int uID;
};

struct C_PATH_END_POINT
{
	unsigned int uID;
};

struct C_HP_LESS
{
	float fPercent;
};


struct C_RANDOM
{
	float fProbability;
};

struct C_ENMITY_REACH
{
	int nPlayer;		//人数
	int nPlayerLevel;	//表示人的级别
};

struct C_DISTANCE_REACH
{
	float fDistance;   //距离上限
};

struct C_ATTACK_BY_SKILL
{
	unsigned int uID;
};

struct C_VAR
{
	int iID;
};

struct C_CONSTANT
{
	int iValue;
};

struct C_RANK_LEVEL
{
	int iValue;
};


//----------------------------------------------------------------------------------------
//Operation
//----------------------------------------------------------------------------------------

struct O_ATTACK_TYPE
{
	unsigned int uType;
};

struct O_USE_SKILL
{
	unsigned int uSkill; //技能
	unsigned int uLevel; //级别
};

struct O_USE_RANGE_SKILL
{
	unsigned int uSkill; //技能
	unsigned int uLevel; //级别
	float        fRange; //范围（1-512）
};

struct O_TRIGGER_TASK
{
	unsigned int uTaskID;
};

struct O_CHANGE_PATH_VERSION8
{
	unsigned int uPathID;
};

struct O_CHANGE_PATH
{
	unsigned int uPathID;
	int			 iType; //其值为0（终点停止）、1（原路返回）或2（首尾循环）
};

struct O_SNEER_MONSTER
{
	float fRange;
};

struct O_TALK_TEXT
{
	unsigned int uSize;
	unsigned short* szData;
};

struct O_RUN_TRIGGER
{
	unsigned int uID;
};

struct O_STOP_TRIGGER
{
	unsigned int uID;
};

struct O_ACTIVE_TRIGGER
{
	unsigned int uID;
};

struct O_CREATE_TIMER
{
	unsigned int uID;
	unsigned int uPeriod;
	unsigned int uCounter;
};

struct O_SUMMON
{
	unsigned int uMounsterID;		//怪物ID
	unsigned int uBodyMounsterID;	//怪物外形，为0时使用上面的形体
	unsigned int uMounsterNum;		//怪物数量（1-999）
	unsigned int uLife;				//怪物生存时间（0-MAX），0表示不会自动消失，(s)
	unsigned short szName[16];		//怪物名字，如果为空就现实怪物ID定义的名字
	float    fRange;				//怪物生成距离本体范围半径(m)
	bool     bFollow;				//怪物是否跟随本体，如果为否，下面随本体消失也必须为否
	bool     bDispear;				//是否跟随本体消失
};

struct O_KILL_TIMER
{
	unsigned int uID;
};

struct O_ACTIVE_CONTROLLER
{
	unsigned int uID;
	bool         bStop;//是否是停止激活
};

struct O_SET_GLOBAL
{
	int  iID;
	int  iValue;
};

struct O_REVISE_GLOBAL
{
	int  iID;
	int  iValue;
};

struct O_ASSIGN_GLOBAL
{
	int  iIDSrc;
	int  iIDDst;
};

struct O_SUMMON_MINERAL
{
	unsigned int uMineralID;		//矿物ID
	unsigned int uMineralNum;		//矿物数量（1-999）
	unsigned int uHP;				//矿物生命（0-MAX）
	float    fRange;				//矿物距本体距离(0-50m)
	bool     bBind;				    //是否绑定目标
};

struct O_DROP_ITEM
{
	unsigned int uItemID;			//丢出来的物品id
	unsigned int uItemNum;			//丢出来的物品数量
	unsigned int uExpireDate;		//丢出来的物品的有效期（当物品不可堆叠时有效，默认为0没有时间限制）
};

struct O_CHANGE_HATE
{
	int			iHateValue;			//改变的仇恨值 Added 2012-08-21.
};

struct O_START_EVENT
{
	int iId;
};

struct O_STOP_EVENT
{
	int iId;
};

//----------------------------------------------------------------------------------------
//Target Occupation
//----------------------------------------------------------------------------------------
struct T_OCCUPATION
{
	unsigned int uBit;
};

#endif
