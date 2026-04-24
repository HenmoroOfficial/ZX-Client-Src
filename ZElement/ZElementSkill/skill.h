#ifndef __SKILL_SKILL_H
#define __SKILL_SKILL_H

#pragma warning(disable:4786)
#include "gnstring.h"
#include <vector.h>
#include <hashmap.h>
#include "playerwrapper.h"
#include "skillwrapper.h"
#include "ElementSkill.h"

#pragma pack(1)

#define MAX_COMMON_VSTATE  64


namespace GNET
{

typedef GnetString wsstream;

class Skill;

int Int(int d);
int Int(float f);
int Int(double d);

int INT(int d);
int INT(float f);
int INT(double d);

int norm(float f); 
int norm(double f); 
int norm(int n);


enum
{
        OCCUPATION_TALISMAN     = 130, //法宝技能
        OCCUPATION_FAMILY       = 131, //家族技能
		OCCUPATION_REBORN       = 146, //转生技能
        OCCUPATION_SECT         = 147, //师徒技能
        OCCUPATION_TAOIST       = 148, //道心技能
        OCCUPATION_EVIL         = 149, //魔性技能
        OCCUPATION_BUDDHA       = 150, //佛缘技能
		OCCUPATION_EQUIP		= 151, //装备技能
        OCCUPATION_SPOUSE       = 152, //夫妻技能
		OCCUPATION_LIFE			= 166, //生活技能
		OCCUPATION_DEITY_TAOIST = 167, //封神技能仙
		OCCUPATION_DEITY_EVIL	= 168, //封神技能魔
		OCCUPATION_DEITY_BUDDHA	= 169, //封神技能佛
		OCCUPATION_TRIGGERED	= 180, //被触发类技能
		OCCUPATION_XPSKILL		= 187, //XP技能
};


//技能使用限制
enum
{
    SKILL_LIMIT_MONSTER     = 0x00000001,   //对怪物无效
    SKILL_LIMIT_PET         = 0x00000002,   //对宠物无效
    SKILL_LIMIT_SUMMON      = 0x00000004,   //对召唤无效
    SKILL_LIMIT_PLAYER      = 0x00000008,   //对人物无效
    SKILL_LIMIT_SELF        = 0x00000010,   //对自身无效
    SKILL_LIMIT_BATTLEFIELD = 0x00000020,   //仅战场下可用
    SKILL_LIMIT_COMBAT      = 0x00000040,   //战斗时不能使用
    SKILL_LIMIT_NONCOMBAT   = 0x00000080,   //战斗外不能使用
    SKILL_LIMIT_NOSUMMONPET = 0x00000100,   //无召唤兽不能使用
    SKILL_LIMIT_MOUNT       = 0x00000200,   //骑乘时不能使用
    SKILL_LIMIT_FLY         = 0x00000400,   //飞行是不能使用
	SKILL_LIMIT_NOSUMMON	= 0x00000800,	//无指定ID召唤兽时不能使用
	SKILL_LIMIT_BOSS		= 0x00001000,	//对BOSS无效
};

//玩家所在状态
enum
{
	PLAYER_IN_BATTLE		= 0x00000001,
	PLAYER_IS_FIGHTING		= 0x00000002,
	PLAYER_HAS_CLONE		= 0x00000004,
	PLAYER_IN_MOUNT			= 0x00000008,
	PLAYER_IN_FLYING		= 0x00000010,

};

class Rank
{       
	Rank();
	static Rank instance;
	abase::hash_map<int, int, abase::_hash_function> basemap;
public:
	static int GetBase(int rank);
	static bool SameBranch(int rank, int root);
};     

class SkillStub
{
public:
	typedef unsigned int ID;
	enum { MIN_LEVEL = 1, MAX_LEVEL = 10 };

public:
    ID              id;                   
    int             occupation;           
    int             maxlevel; 
	int             maxlearn;
    int             rank;         
    int             eventflag;    
    ID              baseid;       
    int             basesp;       
    int             spcost;       
	int             reborncount;
	int 			allowform;
	int				credittype;
	int				clearmask;
	

    char            type;         
    char            timetype;     
    char            targettype;   
    char            rangetype;    
    const char*     action[31];      
    bool            doenchant;    
    bool            dobless;      
	bool            autoattack;
	short			talent[8];       // 相关天赋
	short			talent_size;     // 天赋个数
	char			talent_type;     // 天赋个数

	bool			need_item;          //该技能是否消耗物品
    int				item1_id;           //消耗的物品1 id
    int				item1_num;          //消耗的物品1数量
    int				item2_id;           //消耗的物品2 id
    int				item2_num;          //消耗的物品2数量
    int				item_relation;      //物品1、2的关系：0：没有物品2，1：且；2：或

	int				weapon_limit;		//技能使用的武器限制
	int				skill_limit;		//技能使用限制

	bool			use_proficiency;	//是否使用熟练度
	int				inc_proficiency;	//每次使用技能熟练度增加值
	
	//Add by Houjun 2010-03-18
	int				skillelemsnum;      //组合技技能元素数量
    float			skillelemsfactor;   //组合技技能元素系数
    int				serialskill;        //连续技属性设置，0，非连续技；1，起手技；2，继承技；3，终结技；
    char			charge;             //是否带有冲锋效果，0，否；1，是；
	float			chargedist;			//冲锋或闪现距离
    int				succeedskillcolor;  //继承技颜色，0-4，与c0-c4对应
    int				succeedskillcolornum; //继承技所给的颜色数量0-5
	int				comboskill;			//组合技属性设置，0，是；1，否；
	//Add end.

	//Add by Houjun 2010-10-14
	int				preskillnum;		//前提技能数量
	//Add end.

	bool		cycle;		//是否是阵法
	int			cyclemode;
    const char* cyclegfx;   //阵法特效gfx

	int skill_class;        //所属人物职业分类

	bool guide;				//引导技能
	int  summon_id;			//无召唤兽不可用，召唤兽ID
	bool trigger;			//触发技能

	bool castInPreSkill;	//只能在前置技能释放中使用
	int preCastSkillId;		//前置释放技能ID
	char darkLightType;		//技能光暗类型

	int followskill;		//技能释放结束自动接技能
	int targetcnt;			//目标数量
	int excludemonstercnt;

    const wchar_t*  name;   
    const char*     nativename;         
    const char*     icon;               
	typedef abase::hash_map<ID, SkillStub*, abase::_hash_function> Map;
	static Map& GetMap();

protected:
	SkillStub(ID i) : id(i), use_proficiency(false), inc_proficiency(0)
	{ 
		if(!GetStub(id)) 
			GetMap().insert(Map::value_type(id, this)); 
	}
	
	~SkillStub();

public:

	static const SkillStub *GetStub(ID i)
	{
		Map::iterator it = GetMap().find(i);
		return it == GetMap().end() ? NULL : it->second;
	}

    virtual int GetExecutetime(Skill *skill) const { return 0; }
    virtual int GetCooldowntime(Skill *skill) const { return 0; }
    virtual int GetEnmity(Skill* skill) const { return 0; }
    virtual int GetRequiredLevel(int level) const { return 0; }
	virtual int GetRequiredProficiency(int level) const { return 0; }
    virtual float GetRadius(Skill *skill) const { return 0; }
    virtual float GetAttackdistance(Skill *skill) const { return 0; }
    virtual float GetAngle(Skill *skill) const { return 0; }
    virtual float GetPraydistance(Skill *skill) const { return 0; }
	virtual float GetCastdistance(Skill *skill) const { return 0; }
    virtual float GetEffectdistance(Skill *skill) const { return 0; }
    virtual float GetMpcost(Skill* skill) const { return 0; }
    virtual bool StateAttack(Skill * skill) const { return 0; }
    virtual bool TakeEffect(Skill *skill) const { return 0; }
    virtual bool BlessMe(Skill * skill) const { return 0; }	
	virtual int GetIntroduction(Skill*,wchar_t*,int,wchar_t*, const SkillStr &table) const {return 0;}
	virtual int GetDescription(Skill*,wchar_t*,int,wchar_t*, const SkillStr &table) const {return 0;}
	virtual int GetCreditCost(int level) const { return 0; }
	virtual int GetSkillElem(int index) const { return 0; }
	virtual int GetComboSkillCapacity(int level) const { return 0; }
	virtual int  GetCoverage(Skill *skill) const { return 20; }
	virtual bool CheckDarkLightValue(Skill* skill) const { return true; }

	//Add by Houjun, 封神相关接口
	virtual int GetDpcost(Skill* skill) const { return 0; }
	virtual int GetHpcost(Skill* skill) const { return 0; }
	virtual int GetInkcost(Skill* skill) const { return 0; }
	virtual int GetDivinityExp(int level) const { return 0; }
	virtual int GetDivinityLevel(int level) const { return 0; }
	virtual int GetPreSkillID(int index) const { return 0; }
	virtual int GetPreSkillSP(int index) const { return 0; }
};

class Skill : public ElementSkill
{
protected:
	const SkillStub * stub;
	PlayerWrapper player;

	unsigned int baselevel;
	unsigned int reallevel;
	unsigned int addon;

	Skill(const SkillStub* s) : stub(s), baselevel(0), reallevel(0), addon(0)
	{
	}
	
	virtual ~Skill() { }
public:
	static Skill* Create(ID id, int n)
	{
		const SkillStub * stub = SkillStub::GetStub(id);
		if(!stub)
			return NULL;
		Skill* skill = new Skill(stub);
		int max = skill->GetMaxLevel();
		if(n>max)
			skill->SetLevel(max);
		else
			skill->SetLevel(n);
		
		return skill;
	}

	void Destroy() 
	{ 
		delete this; 
	}

	const SkillStub * GetStub() const {	return stub; }
	PlayerWrapper * GetPlayer()  { return &player; }

	ID GetId() const { return stub->id; }
	int SetLevel(int n) 
	{
		baselevel = n; 
		return UpdateLevel();
	}
	int SetAddonLevel(int n) 
	{
		addon = n; 
		return UpdateLevel();
	}
	int SetCommonLevel(int n) 
	{
		SkillWrapper::Instance()->SetCommonLevel(n);
		return UpdateLevel();
	}
	int UpdateLevel()
	{
		if(!baselevel)
		{
			reallevel = 0;
			return 0;
		}
		reallevel = baselevel + addon + SkillWrapper::Instance()->GetCommonLevel();
		if((int)reallevel>stub->maxlevel)
			reallevel = stub->maxlevel;
		return reallevel;
	}
	int GetLevel() const { return reallevel; }
	int GetBaseLevel() { return baselevel; }
	int GetAddonLevel() { return addon; }
	char GetType() const { return stub->type; }
	const wchar_t* GetName() { return stub->name;}

	float GetPrayRange(float range) {
		player.SetRange(range);
		if(stub->type==TYPE_ATTACK || stub->type==TYPE_CURSE)
		{
			return stub->GetCastdistance(this);
		}
		if(stub->type==TYPE_JUMP)
			return stub->GetCastdistance(this); 
		if(stub->rangetype==2 || stub->rangetype==5)
			return -1;
		else
			return stub->GetCastdistance(this); 
	}

	const char* GetIcon() { return stub->icon; }
	int GetRangeType() { return stub->rangetype; }
	int GetOccupation() const { return stub->occupation; }
	int GetCoolingTime() { return stub->GetCooldowntime(this); }
	int GetMaxLevel()  { return stub->maxlevel; }
	int GetRand() { return 100; }
	int GetMpCost() { return (int)stub->GetMpcost(this); }
	int GetAllowForm() { return stub->allowform; }
	int GetRequiredLevel() { return  stub->GetRequiredLevel(baselevel); } 
	bool IsUseProficiency() { return stub->use_proficiency; }
	int GetIncProficiency() { return stub->inc_proficiency; }
	int GetRequiredProficiency() { return stub->GetRequiredProficiency(baselevel); }
	int GetProficiency() { return SkillWrapper::Instance()->GetProficiency(GetId()); }
	void SetProficiency(int prof) { SkillWrapper::Instance()->SetProficiency(GetId(), prof); }
	int GetCreditCost() { return  stub->GetCreditCost(baselevel); } 
	int GetRequiredSkill() { return stub->baseid; }
	int GetOccupation() { return stub->occupation; }
	bool IsComboSkill() { return stub->comboskill == 0; }
	float GetRadius() { return stub->GetRadius(this); }

	bool IsMonsterLimitSkill() { return (stub->skill_limit & SKILL_LIMIT_MONSTER) > 0; }
    bool IsPetLimitSkill() { return (stub->skill_limit & SKILL_LIMIT_PET) > 0; }
    bool IsSummonLimitSkill() { return (stub->skill_limit & SKILL_LIMIT_SUMMON) > 0; }
    bool IsPlayerLimitSkill() { return (stub->skill_limit & SKILL_LIMIT_PLAYER) > 0; }
    bool IsSelfLimitSkill() { return (stub->skill_limit & SKILL_LIMIT_SELF) > 0; }
    bool IsBattleLimitSkill() { return (stub->skill_limit & SKILL_LIMIT_BATTLEFIELD) > 0; }
    bool IsCombatLimitSkill() { return (stub->skill_limit & SKILL_LIMIT_COMBAT) > 0; }
    bool IsNonCombatLimitSkill() { return (stub->skill_limit & SKILL_LIMIT_NONCOMBAT) > 0; }
    bool IsNoSummonPetLimitSkill() { return (stub->skill_limit & SKILL_LIMIT_NOSUMMONPET) > 0; }
    bool IsMountLimitSkill() { return (stub->skill_limit & SKILL_LIMIT_MOUNT) > 0; }
    bool IsFlyLimitSkill() { return (stub->skill_limit & SKILL_LIMIT_FLY) > 0; }
	bool IsNoSummonLimitSkill() { return (stub->skill_limit & SKILL_LIMIT_NOSUMMON) > 0; }
	int GetSummonLimitSkillId() { return stub->summon_id; }
	bool IsBossLimitSkill() { return (stub->skill_limit & SKILL_LIMIT_BOSS) > 0; }

	bool IsXPSkill() { return stub->occupation == OCCUPATION_XPSKILL; }

	int  GetMaxTargetCnt() { return stub->targetcnt; }
	int  GetFollowSkill() { return stub->followskill; }
	bool CheckDarkLightValue() { return stub->CheckDarkLightValue(this); }

	float GetEffectdistance(){ return stub->GetEffectdistance(this); }

	//Add by Houjun 2010-03-18
	int GetSkillElemsNum()    { return stub->skillelemsnum; }
    int GetSerialSkill() const{ return stub->serialskill; }
    bool IsCharge()     { return stub->charge == 1;}
	bool IsChargeToTarget() { return stub->charge == 2;} 
	bool IsBlinkFaceDir() { return stub->charge == 3; }
	float GetChargeDist() { return stub->chargedist; }
    int GetSucceedSkillColor(){ return stub->succeedskillcolor; }
    int GetSucceedSkillColorNum() { return stub->succeedskillcolornum; }
	//Add end.

	//Add by Houjun 2010-03-29
	int GetSkillElem(int index) { return stub->GetSkillElem(index); }
	int GetComboSkillCapacity(int level) { return stub->GetComboSkillCapacity(level); }
	//Add end.

	//Add by Houjun 2010-10-14
	int GetPreskillNum() const { return stub->preskillnum; }
	int GetDpcost() { return stub->GetDpcost(this); }
	int GetHpcost() { return stub->GetHpcost(this); }
	int GetInkcost() { return stub->GetInkcost(this); }
	int GetDivinityExp(int level) const { return stub->GetDivinityExp(level); }
	int GetDivinityLevel(int level) const { return stub->GetDivinityLevel(level); }
	int GetPreSkillID(int index) const { return stub->GetPreSkillID(index); }
	int GetPreSkillSP(int index) const { return stub->GetPreSkillSP(index); }
	int GetRequiredDivinityExp() const { return GetDivinityExp(baselevel); }
	int GetRequiredDivinityLevel() const { return GetDivinityLevel(baselevel); }

	bool IsWarmup()	 { return stub->timetype==3; }
	bool IsInstant() { return stub->timetype==1; }
	bool IsDurative() { return stub->timetype==2; }
	bool IsAutoAttack() { return stub->autoattack; }
	int  GetCategory() 
	{ 
		int mask = 0;
		if(stub->occupation==130)
			mask |= 0x0001;
		else if(stub->occupation==131)
			mask |= 0x0002;
		if(stub->talent_type==1)
			mask |= 0x0004;
		else if(stub->talent_type==2)
			mask |= 0x0008;
		if(stub->occupation==147)
			mask |= 0x0010;
		else if(stub->occupation==146)
			mask |= 0x0040;
		else if(stub->occupation>=148 && stub->occupation<=150)
			mask |= 0x0020;
		else if(stub->occupation==OCCUPATION_SPOUSE)
			mask |= CATEGORY_SPOUSE;
		else if(stub->occupation==OCCUPATION_LIFE)
			mask |= CATEGORY_LIFE;
		else if(stub->occupation>=167 && stub->occupation<=169)
			mask |= CATEGORY_DEITY;
		else if(stub->occupation==OCCUPATION_TRIGGERED || stub->trigger)
			mask |= CATEGORY_TRIGGERED;
		return mask; 
	}

	const wchar_t* GetShowlevel()
	{
		static wchar_t buffer[16];
		if(reallevel!=baselevel)
			_snwprintf(buffer, 64, L"%d+%d", baselevel, reallevel-baselevel);
		else
			_snwprintf(buffer, 64, L"%d", baselevel);
		return buffer;
	}
	
	const wchar_t *GetCurproficiency() 
	{ 
		static wchar_t buffer[16];
		if (baselevel >= stub->maxlearn)
			_snwprintf(buffer, 64, L"99999");
		else
			_snwprintf(buffer, 64, L"%d", GetProficiency());
		return buffer; 
	}
	const wchar_t *GetNeedproficiency() 
	{ 
		static wchar_t buffer[16];
		if (baselevel >= stub->maxlearn)
			_snwprintf(buffer, 64, L"99999");
		else
			_snwprintf(buffer, 64, L"%d", stub->GetRequiredProficiency(baselevel+1)); 
		return buffer;
	}
	const wchar_t* GetIntroduction(wchar_t* buf,int len,const SkillStr& table,bool lvnext=true)
	{
		int tmp = GetBaseLevel();
		int n = stub->GetIntroduction(this,buf,len,table.Find(stub->id*10+1), table); 
		if(n<0)
			return L"";
		buf += n; len -= n;

		if(tmp)
		{
			n = _snwprintf(buf, len, L"\r");
			buf += n; len -= n;
			n = stub->GetDescription(this,buf,len,table.Find(stub->id*10+2), table); 
			if(n<0) 
				return L"";
			buf += n; len -= n;
		}

		if((int)reallevel==stub->maxlevel || tmp>=stub->maxlearn || !lvnext)
			return buf;
		SetLevel(tmp+1);
		n = _snwprintf(buf, len, L"\r\r%s\r", table.Find(1));
		buf += n; len -= n;
		if(n<0 || len<0)
		{
			SetLevel(tmp);
			return L"";
		}
		
		n = stub->GetDescription(this,buf,len,table.Find(stub->id*10+2), table);
		buf += n; len -= n;
		if(n<0 || len<0)
		{
			SetLevel(tmp);
			return L"";
		}

		if(len<128 || stub->occupation==130 || stub->occupation==131 || stub->occupation==145 || stub->occupation == 151)
		{
			SetLevel(tmp);
			return n>0?buf:L"";
		}

		if(stub->occupation==OCCUPATION_TRIGGERED)
		{
			SetLevel(tmp);
			return n>0?buf:L"";
		}

		n = _snwprintf(buf, len, table.Find(2), GetRequiredLevel());
		if(n>0)
		{
			buf += n; len -= n;
		}
		if(stub->talent_type)
		{
			for(int i = 0; i < stub->preskillnum; i ++)
			{
				if(GetPreSkillID(i))
				{
					n = _snwprintf(buf, len, table.Find(3), table.Find(GetPreSkillID(i)*10), GetPreSkillSP(i));
					if(n>0)
					{
						buf += n; len -= n;
					}
				}
			}
			n = _snwprintf(buf, len, table.Find(6), 1);
			if(n>0)
			{
				buf += n; len -= n;
			}
		}
		else
		{
			for(int i = 0; i < stub->preskillnum; i ++)
			{
				if(GetPreSkillID(i))
				{
					n = _snwprintf(buf, len, table.Find(3), table.Find(GetPreSkillID(i)*10), GetPreSkillSP(i));
					if(n>0)
					{
						buf += n; len -= n;
					}
				}
				else if(stub->occupation!=OCCUPATION_SECT && stub->occupation!=OCCUPATION_TAOIST && stub->occupation!=OCCUPATION_EVIL 
					&& stub->occupation!=OCCUPATION_BUDDHA && stub->occupation!=OCCUPATION_SPOUSE
					&& !stub->use_proficiency)
				{
					n = _snwprintf(buf, len, table.Find(4), GetPreSkillSP(i));
					if(n>0)
					{
						buf += n; len -= n;
					}
				}
			}
			if(stub->occupation == OCCUPATION_DEITY_TAOIST || stub->occupation == OCCUPATION_DEITY_EVIL || stub->occupation == OCCUPATION_DEITY_BUDDHA)
			{
				n = _snwprintf(buf, len, table.Find(13), GetRequiredDivinityLevel());
				if(n>0)
				{
					buf += n; len -= n;
				}
				n = _snwprintf(buf, len, table.Find(14), GetRequiredDivinityExp());
				if(n>0)
				{
					buf += n; len -= n;
				}
			}
			else if(stub->credittype==0 && !stub->use_proficiency)
			{
				n = _snwprintf(buf, len, table.Find(5), 1);
				if(n>0)
				{
					buf += n; len -= n;
				}
			}
		}
		if(stub->reborncount)
		{
				n = _snwprintf(buf, len, table.Find(7), stub->reborncount);
				if(n>0)
				{
					buf += n; len -= n;
				}
		}
		if(stub->credittype)
		{
			int index = 8;
			if(stub->credittype==9)
				index = 9;
			else if(stub->credittype==10)
				index = 18;
			else if(stub->credittype==11)
				index = 19;
			else if(stub->credittype==6)
				index = 17;
			n = _snwprintf(buf, len, table.Find(index), GetCreditCost());
			if(n>0)
			{
				buf += n; len -= n;
			}
		}
		if (stub->use_proficiency)
		{
			n = _snwprintf(buf, len, table.Find(20), GetRequiredProficiency());
			if (n>0)
			{
				buf += n; len -= n;
			}
		}
		SetLevel(tmp);
		return n>0?buf:L"";
	}

	const wchar_t* GetNextLevelDescription(wchar_t* buf,int len,const SkillStr& table,bool showtip=true)
	{
		int tmp = GetBaseLevel();
		if(tmp <= 0)
		{			
			return L"";
		}
		if((int)reallevel==stub->maxlevel || tmp>=stub->maxlearn)
		{
			return L"";
		}

		SetLevel(tmp+1);
		int n;
		if (showtip)
		{
			n = _snwprintf(buf, len, L"\r\r%s\r", table.Find(1));
			buf += n; len -= n;
			if(n<0 || len<0)
			{
				SetLevel(tmp);
				return L"";
			}
		}
		
		n = stub->GetDescription(this,buf,len,table.Find(stub->id*10+2), table);
		buf += n; len -= n;
		if(n<0 || len<0)
		{
			SetLevel(tmp);
			return L"";
		}
		SetLevel(tmp);
		return n>0?buf:L"";
	}

	const wchar_t* GetLearnConditionDescrition(wchar_t* buf,int len,const SkillStr& table)
	{
		int tmp = GetBaseLevel();
		if(tmp <= 0)
		{
			return L"";
		}

		if((int)reallevel==stub->maxlevel || tmp>=stub->maxlearn)
		{
			return L"";
		}		
		if(len<128 || stub->occupation==130 || stub->occupation==131 || stub->occupation==145 || stub->occupation == 151)
		{
			return L"";
		}

		if(stub->occupation==OCCUPATION_TRIGGERED)
		{			
			return L"";
		}

		SetLevel(tmp+1);
		int n = _snwprintf(buf, len, table.Find(2), GetRequiredLevel());
		if(n>0)
		{
			buf += n; len -= n;
		}
		if(stub->talent_type)
		{
			for(int i = 0; i < stub->preskillnum; i ++)
			{
				if(GetPreSkillID(i))
				{
					n = _snwprintf(buf, len, table.Find(3), table.Find(GetPreSkillID(i)*10), GetPreSkillSP(i));
					if(n>0)
					{
						buf += n; len -= n;
					}
				}
			}
			n = _snwprintf(buf, len, table.Find(6), 1);
			if(n>0)
			{
				buf += n; len -= n;
			}
		}
		else
		{
			for(int i = 0; i < stub->preskillnum; i ++)
			{
				if(GetPreSkillID(i))
				{
					n = _snwprintf(buf, len, table.Find(3), table.Find(GetPreSkillID(i)*10), GetPreSkillSP(i));
					if(n>0)
					{
						buf += n; len -= n;
					}
				}
				else if(stub->occupation!=OCCUPATION_SECT && stub->occupation!=OCCUPATION_TAOIST && stub->occupation!=OCCUPATION_EVIL 
					&& stub->occupation!=OCCUPATION_BUDDHA && stub->occupation!=OCCUPATION_SPOUSE
					&& !stub->use_proficiency)
				{
					n = _snwprintf(buf, len, table.Find(4), GetPreSkillSP(i));
					if(n>0)
					{
						buf += n; len -= n;
					}
				}
			}
			if(stub->occupation == OCCUPATION_DEITY_TAOIST || stub->occupation == OCCUPATION_DEITY_EVIL || stub->occupation == OCCUPATION_DEITY_BUDDHA)
			{
				n = _snwprintf(buf, len, table.Find(13), GetRequiredDivinityLevel());
				if(n>0)
				{
					buf += n; len -= n;
				}
				n = _snwprintf(buf, len, table.Find(14), GetRequiredDivinityExp());
				if(n>0)
				{
					buf += n; len -= n;
				}
			}
			else if(stub->credittype==0 && !stub->use_proficiency)
			{
				n = _snwprintf(buf, len, table.Find(5), 1);
				if(n>0)
				{
					buf += n; len -= n;
				}
			}
		}
		if(stub->reborncount)
		{
				n = _snwprintf(buf, len, table.Find(7), stub->reborncount);
				if(n>0)
				{
					buf += n; len -= n;
				}
		}
		if(stub->credittype)
		{
			int index = 8;
			if(stub->credittype==9)
				index = 9;
			else if(stub->credittype==10)
				index = 18;
			else if(stub->credittype==11)
				index = 19;
			else if(stub->credittype==6)
				index = 17;
			n = _snwprintf(buf, len, table.Find(index), GetCreditCost());
			if(n>0)
			{
				buf += n; len -= n;
			}
		}
		if (stub->use_proficiency)
		{
			n = _snwprintf(buf, len, table.Find(20), GetRequiredProficiency());
			if (n>0)
			{
				buf += n; len -= n;
			}
		}
		SetLevel(tmp);
		return n>0?buf:L"";
	}

	int GetTargetType()
	{
		if(stub->targettype)
			return 2;
		if (stub->rangetype == 6)
		{
			return 0;
		}
		if(stub->type==TYPE_ATTACK || stub->type==TYPE_CURSE)
			return 1;
		if(stub->rangetype==2 || stub->rangetype==5/* || stub->rangetype == 6*/)
			return 0;
		return 1;
	}

	const short* GetTalent() 
	{ 
		return stub->talent; 
	} 

	short GetTalentSize() 
	{ 
		return stub->talent_size; 
	} 

	int GetExecuteTime()
	{
		return stub->GetExecutetime(this);
	}

	const char* GetAction(unsigned char type) 
	{ 
		return type>=sizeof(stub->action)/sizeof(stub->action[0]) ? "" : stub->action[type];
	}
	const char* GetNativeName() { return stub->nativename;}
	char  GetT0()          
	{ 
		short n = GetTalentSize();
		if (n > 0)
			return SkillWrapper::Instance()->GetLevel(stub->talent[0]);
		else
			return 0;
	}
	char  GetT1()
	{ 
		short n = GetTalentSize();
		if (n > 1)
			return SkillWrapper::Instance()->GetLevel(stub->talent[1]);
		else
			return 0;
	}
	char  GetT2()
	{ 
		short n = GetTalentSize();
		if (n > 2)
			return SkillWrapper::Instance()->GetLevel(stub->talent[2]);
		else
			return 0;
	}
	char  GetT3()
	{ 
		short n = GetTalentSize();
		if (n > 3)
			return SkillWrapper::Instance()->GetLevel(stub->talent[3]);
		else
			return 0;
	}
	char  GetT4()
	{ 
		short n = GetTalentSize();
		if (n > 4)
			return SkillWrapper::Instance()->GetLevel(stub->talent[4]);
		else
			return 0;
	}
	char  GetT5()
	{ 
		short n = GetTalentSize();
		if (n > 5)
			return SkillWrapper::Instance()->GetLevel(stub->talent[5]);
		else
			return 0;
	}
	char  GetT6()
	{ 
		return 0;
	}
	char  GetT7()
	{ 
		return 0;
	}

	bool IsNeedItem() { return stub->need_item; }
	int GetItem1Id() { return stub->item1_id; }
	int GetItem1Num() { return stub->item1_num; }
	int GetItem2Id()	{ return stub->item2_id; }
	int GetItem2Num() { return stub->item2_num; }
	int GetItemRelation() { return stub->item_relation; }
	int Condition(UseRequirement& info);

	virtual bool IsCycle() { return stub->cycle;}
	virtual const char* GetCycleGfx() { return stub->cyclegfx;}		

	virtual bool IsGuide() { return stub->guide; }
	virtual float GetDarkLightType() { return stub->darkLightType; }
	virtual bool IsCastInPreSkill() { return stub->castInPreSkill; }
	virtual int GetPreCastSkillId() { return stub->preCastSkillId; }
	
	//Add by Houjun 2010-06-17, 设置和玩家相关的属性
	void SetPlayerMinatk(int v) { player.SetMinatk(v); }
	void SetPlayerMaxatk(int v) { player.SetMaxatk(v); }
	void SetPlayerDef(int v) { player.SetDef(v); }
	void SetPlayerAtkrate(int v) { player.SetAtkrate(v); }
	void SetPlayerDodge(int v) { player.SetDodge(v); }
	void SetPlayerRes1(int v) { player.SetRes1(v); }
	void SetPlayerRes2(int v) { player.SetRes2(v); }
	void SetPlayerRes3(int v) { player.SetRes3(v); }
	void SetPlayerRes4(int v) { player.SetRes4(v); }
	void SetPlayerRes5(int v) { player.SetRes5(v); }
	void SetPlayerRes6(int v) { player.SetRes6(v); }
	void SetPlayerCrit(int v) { player.SetCrit(v); }
	void SetPlayerCrithurt(float v) { player.SetCrithurt(v); }
	void SetPlayerSkillrate(int v) { player.SetSkillrate(v); }
	void SetPlayerSkilldodge(int v) { player.SetSkilldodge(v); }
	void SetPlayerGender(int v) { player.SetGender(v); }
	void SetPlayerReborncount(int v) { player.SetReborncount(v); }
	void SetPlayerCultivation(int v) { player.SetCultivation(v); }
	void SetPlayerOccupation(int v) { player.SetOccupation(v); }
	void SetPlayerType(int v) { player.SetType(v); }
	void SetPlayerRace(int v) { player.SetRace(v); }
	void SetPlayerSpeed(float v) { player.SetSpeed(v); }
	void SetPlayerLevel(int v) { player.SetLevel(v); }
	void SetPlayerDarklight(int v) { player.SetDarklight(v); }
	void SetPlayerInk(int v) { player.SetInk(v); }
	void SetPlayerRage(int v) { player.SetRage(v); }
	//Add end. 

};

class VisibleStateImp : public VisibleState
{
	int id;
	const wchar_t*	name;
	const char* effect;
	const char* hh;
	typedef abase::hash_map<int, const VisibleState*, abase::_hash_function> Map;
	static Map stubmap;
public:
	VisibleStateImp(int i, const wchar_t* n, const char* e, const char* h):id(i), name(n), effect(e), hh(h)
	{
		stubmap.insert(Map::value_type(id, this));  
	}
	static const VisibleState* Query(int prof, int id);
	const int GetID() const { return id; }
	const wchar_t* GetName() const {  return name; }
	const char* GetHH() const { return hh; }
	const char* GetEffect() const {  return effect; }
};

class TeamStateImp : public TeamState
{
	int id;
	const wchar_t* name;
	const char*   icon;
	typedef abase::hash_map<int, const TeamState*, abase::_hash_function> Map;
	static Map stubmap;
public:
	TeamStateImp(int i, const wchar_t* n, const char* c):id(i), name(n), icon(c)
	{
		stubmap.insert(Map::value_type(id, this)); 
	}
	static const TeamState* Query(int id);
	const int GetID() const { return id; }
	const wchar_t* GetName() const {  return name; }
	const char* GetIcon() const { return icon; }
};

};
#pragma pack()

#endif

