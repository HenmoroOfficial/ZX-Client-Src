#ifndef __ELEMENT_SKILL_H
#define __ELEMENT_SKILL_H

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ELEMENTSKILL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ELEMENTSKILL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef ELEMENTSKILL_EXPORTS
#define ELEMENTSKILL_API __declspec(dllexport)
#else
#define ELEMENTSKILL_API __declspec(dllimport)
#endif

namespace GNET
{
enum skill_type{
    TYPE_ATTACK  = 1,  // 主动攻击
    TYPE_BLESS   = 2,  // 主动祝福
    TYPE_CURSE   = 3,  // 主动诅咒
    TYPE_JUMP    = 4,  // 瞬移

	TYPE_PASSIVE = 11, // 被动
	TYPE_ENABLED = 12, // 武器附加
	TYPE_RUNE    = 13, // 物品技能
	TYPE_LIVE    = 14, // 生产
};

enum range_type{
	TYPE_POINT         = 0, // 点 
	TYPE_LINE          = 1, // 线 
	TYPE_SELFSPHERE    = 2, // 自身为中心的球
	TYPE_TARGETSPHERE  = 3, // 目标为中心的球
	TYPE_TAPER         = 4, // 圆锥
	TYPE_SLEF          = 5, // 自身
	TYPE_POS           = 6, // 地面位置
	TYPE_OWNER		   = 7,	// 宠物主人
	TYPE_SPOUSE		   = 8,	// 情侣, 夫妻
	TYPE_CLONE		   = 9, // 分身
	TYPE_CHAIN		   = 10,// 目标为起点的链
	TYPE_MULTILINE	   = 11,// 多条线形攻击
	TYPE_FACELINE          = 12,// 施法者面向线（无目标）
    TYPE_FACESECTOR        = 13,// 施法者面向扇形（无目标）
    TYPE_SELFBALLNOTARGET  = 14,// 自身为圆心的球形（无目标）
    TYPE_SELFCROSS         = 15,// 自身为中心的十字（无目标）
};

enum
{
	CATEGORY_TALISMAN  = 0x0001, //法宝
	CATEGORY_FAMILY    = 0x0002, //家族
	CATEGORY_TALENT    = 0x0004, //天赋
	CATEGORY_POTENTIAL = 0x0008, //潜能
	CATEGORY_SECT      = 0x0010, //师徒
	CATEGORY_MYSTERY   = 0x0020, //仙魔
	CATEGORY_REBORN    = 0x0040, //转生
	CATEGORY_SPOUSE    = 0x0080, //夫妻
	CATEGORY_LIFE	   = 0x0100, //生活
	CATEGORY_DEITY	   = 0x0200, //封神仙佛魔
	CATEGORY_TRIGGERED = 0x0400, //被触发类技能
};

enum DARK_LIGHT_TYPE
{
    DLTYPE_NONE = 0,
    DLTYPE_DARK,
    DLTYPE_LIGHT,
};

struct LearnRequirement
{
	int level;		//玩家级别
	int sp;         //技能点
	int talent;     //天赋点数
	int profession; //职业
	int reborncount;//转生次数
	int cultivation;//仙，魔，佛，情侣状态
	int deityLevel;	//封神等级
	double deityExp;//封神修为
	int* region_repu;
	size_t repu_count;
};

struct UseRequirement
{
	int mp;         //魔法
	int form;       //变身状态
	int cultivation;	//仙、魔、佛，情侣状态
	int weapon_type;	//玩家武器类型,0:空手，1.....
	int skill_limit;	//在战场中0x0001，在战斗中0x0002，有分身0x0004，在骑乘状态0x0008，在飞行中0x0010，
	int dp;			// 元力
	int curr_skill_id;	//当前释放技能
	int hp;			//当前hp
	int ink;		//当前墨水
	int iProfession;
};

struct PlayerProps
{
	int level;		// 玩家等级，暂时先加这个属性
	int occupation; // 玩家职业，新添加的属性，需要注意添加这种属性对整个程序的影响 Added 2011-04-18
	int darklight;	//光暗度
};

class ELEMENTSKILL_API SkillStr
{
public:
	virtual wchar_t* Find(int id) const {  return L""; }
};

class ELEMENTSKILL_API ElementSkill
{
public:
	typedef unsigned int ID;
	
	static ID NextSkill(ID id = 0);

	// 技能名称
	virtual const wchar_t* GetName() { return NULL;}
	virtual const char* GetNativeName() { return NULL;}
	// 技能类别 0.主动攻击, 1.主动祝福, 2.主动诅咒, 3.召唤, 4.被动, 5.激活, 6.变身, 7.附体
	virtual char GetType() const { return 1; }        
	// 技能图标
	virtual const char* GetIcon() { return NULL; }
	// 技能冷却时间，单位毫秒
	virtual int GetCoolingTime() { return 5000; }
	// 技能执行时间，单位毫秒
	virtual int GetExecuteTime() { return 1000; }
	// 目标类型限制, 0 无需目标， 1 需要目标，  2 目标仅限于尸体
	virtual int GetTargetType(){return 0;}
	// 技能有效释放距离: <-0.001无需目标 -0.001-0.001 默认攻击距离，>0.001 释放距离
	virtual float GetPrayRange(float range) { return 0; }  
	// 技能说明
	virtual const wchar_t* GetIntroduction(wchar_t* buf,int len,const SkillStr& table,bool lvnext=true) { return L""; }
	virtual const wchar_t* GetNextLevelDescription(wchar_t* buf,int len,const SkillStr& table,bool showtip=true) { return L""; }
	virtual const wchar_t* GetLearnConditionDescrition(wchar_t* buf,int len,const SkillStr& table) { return L""; }
	// 学习n级技能要求的玩家级别
	virtual int GetRequiredLevel() { return 0;}

	// 是否使用技能熟练度
	virtual bool IsUseProficiency() { return false; }
	// 每次使用后技能熟练度增加值
	virtual int GetIncProficiency() { return 0; }
	// 学习n级技能需要的熟练度
	virtual int GetRequiredProficiency() { return 0; }
	// 得到该技能当前的熟练度
	virtual int GetProficiency() { return 0; }

	virtual void SetProficiency(int prof) {}

	// 前提技能
	virtual int GetRequiredSkill() { return 0;}

	virtual int SetLevel(int) { return 0; } 
	virtual int SetAddonLevel(int addon) { return 0; } 
	virtual int SetCommonLevel(int level) { return 0; } 
	virtual int GetLevel() const { return 0; }
	virtual int GetBaseLevel() { return 0; } 
	virtual int GetAddonLevel() { return 0; } 
	virtual int GetMaxLevel() { return 0; }
	virtual const short* GetTalent() { return NULL; } 
	virtual short GetTalentSize() { return 0; } 

	// 是否蓄力技能
	virtual bool IsWarmup() { return false; }
	// 使用后是否自动攻击
	virtual bool IsAutoAttack(){return false;}
	// 瞬发技能
	virtual bool IsInstant(){return false;}
	// 是否持续技能
	virtual bool IsDurative() { return false; }
	// 技能所属类别
	virtual int  GetCategory() { return 0; }
	// 杀伤范围的类型
	virtual int  GetRangeType() { return 0; }
	// 动作类型: type(0-12)
	virtual const char* GetAction(unsigned char type) { return 0;}

	// 使用需要MP
	virtual int GetMpCost(){return 1;}
	virtual int GetAllowForm() { return 0; }

	// 0:成功               1:武器不匹配        2:mp不足
	// 5:错误ID             6:未选择目标		3:仙魔佛阵营不满足
	virtual int Condition(UseRequirement& info) = 0;

	virtual bool IsNeedItem() { return false; }
	virtual int GetItem1Id() { return 0; }
	virtual int GetItem1Num() { return 0; }
	virtual int GetItem2Id()	{ return 0; }
	virtual int GetItem2Num() { return 0; }
	virtual int GetItemRelation() { return 0; }
	// 获取技能种类
	virtual int GetOccupation() { return 0; }

	virtual bool IsCharge() { return false; }
	virtual bool IsChargeToTarget() { return false; }
	virtual bool IsBlinkFaceDir() { return false; }
	virtual float GetChargeDist() { return 0; }
	virtual bool IsComboSkill() { return false; }
	virtual int GetSkillElem(int index) { return 0; }
	virtual int GetSkillElemsNum()    { return 0; }
	virtual int GetComboSkillCapacity(int level) { return 0; }
	
	//Add by Houjun 2010-10-14, 封神技能接口
	virtual int GetPreskillNum() const { return 0;}
	virtual int GetDpcost() const { return 0; }
	virtual int GetDivinityExp(int level) const { return 0; }
	virtual int GetDivinityLevel(int level) const { return 0; }
	virtual int GetPreSkillID(int index) const { return 0; }
	virtual int GetPreSkillSP(int index) const { return 0; }

	virtual bool IsMonsterLimitSkill() { return false; }
    virtual bool IsPetLimitSkill() { return false; }
    virtual bool IsSummonLimitSkill() { return false; }
    virtual bool IsPlayerLimitSkill() { return false; }
    virtual bool IsSelfLimitSkill()	{ return false; }
    virtual bool IsBattleLimitSkill() { return false; }
    virtual bool IsCombatLimitSkill() { return false; }
    virtual bool IsNonCombatLimitSkill() { return false; }
    virtual bool IsNoSummonPetLimitSkill() { return false; }
    virtual bool IsMountLimitSkill() { return false; }
    virtual bool IsFlyLimitSkill() { return false; }
	virtual bool IsNoSummonLimitSkill() { return false; }
	virtual int GetSummonLimitSkillId() { return -1; }
	virtual bool IsBossLimitSkill() { return false; }
	virtual bool IsXPSkill() { return false; }

	virtual int  GetMaxTargetCnt() { return 0; }
	virtual int  GetFollowSkill() { return -1; }
	
	//是否技能带有阵法效果
	virtual bool IsCycle() { return false; }
	virtual const char* GetCycleGfx() { return NULL;}

	virtual float GetRadius() { return 0; }

	virtual float GetEffectdistance(){ return 0; }

	virtual bool IsGuide() { return false; }
	virtual float GetDarkLightType() { return 0; }
	virtual bool IsCastInPreSkill() { return false; }
	virtual int GetPreCastSkillId() { return -11; }
	virtual bool CheckDarkLightValue() { return true; }

	// 0:成功               1:SP不足            2:玩家级别不够        
	// 3:前提技能等级不足   4:达到学习上限　　　5:错误ID
	// 6:职业不满足         7:天赋点不足        8:转生次数不足
	// 9:声望不足           10:仙魔条件不满足
	static int LearnCondition(ID id, LearnRequirement& info, int baselevel); 

	static char GetType(ID id);  
	static const char* GetIcon(ID id);
	static const wchar_t* GetName(ID id);
	static const char* GetNativeName(ID id);
	static const char* GetEffect(ID id);
	static void LoadSkillData(void * data);
	static const char* GetAction(ID id,unsigned char type);
	static int GetExecuteTime(ID id, int level);

	static int SetLevel(ID id, int baselevel);
	static int SetExtraSkillLevel(ID id, int baseLevel);
	static int SetFashionSkillLevel(ID id, int baseLevel);
	static ElementSkill* Create(ID id, int baselevel);
	
	void Destroy();
	static int GetVersion();

	static void LoadSkillElemsData(void * data);
	static unsigned short* GetComboSkillElems(int skillid, int& num);
	static void UpdateComboSkillElems(int skillid, int* data, int num);
	
	virtual void SetPlayerMinatk(int v) {}
	virtual void SetPlayerMaxatk(int v) {}
	virtual void SetPlayerDef(int v) {}
	virtual void SetPlayerAtkrate(int v) {}
	virtual void SetPlayerDodge(int v) {}
	virtual void SetPlayerRes1(int v) {}
	virtual void SetPlayerRes2(int v) {}
	virtual void SetPlayerRes3(int v) {}
	virtual void SetPlayerRes4(int v) {}
	virtual void SetPlayerRes5(int v) {}
	virtual void SetPlayerRes6(int v) {}
	virtual void SetPlayerCrit(int v) {}
	virtual void SetPlayerCrithurt(float v) {}
	virtual void SetPlayerSkillrate(int v) {}
	virtual void SetPlayerSkilldodge(int v) {}
	virtual void SetPlayerGender(int v) {}
	virtual void SetPlayerReborncount(int v) {}
	virtual void SetPlayerCultivation(int v) {}
	virtual void SetPlayerOccupation(int v) {}
	virtual void SetPlayerType(int v) {}
	virtual void SetPlayerRace(int v) {}
	virtual void SetPlayerSpeed(float v) {}
	virtual void SetPlayerLevel(int v) {}
	virtual void SetPlayerDarklight(int v){}
};

class ELEMENTSKILL_API VisibleState
{
public:
	virtual const int GetID() const { return 0; }
	virtual const wchar_t* GetName() const {  return NULL; }
	virtual const char* GetHH() const { return NULL; }
	virtual const char* GetEffect() const {  return NULL; }
	static const VisibleState* Query(int profession, int id);
};

class ELEMENTSKILL_API TeamState
{
public:
	virtual const int GetID() const { return 0; }
	virtual const wchar_t* GetName() const {  return NULL; }
	virtual const char* GetIcon() const { return NULL; }
	static const TeamState* Query(int id);
};


}

#endif
