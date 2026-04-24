#ifndef __SKILL_PLAYERWRAPPER_H
#define __SKILL_PLAYERWRAPPER_H

#include "skillwrapper.h"

namespace GNET
{
struct PlayerInfo;

class PlayerWrapper
{
public:
	typedef unsigned int	SID;
	typedef unsigned int	PID;

	float range;
private:
	PlayerInfo * object;
	
	int minatk;
	int maxatk;
	int def;
	int atkrate;
	int dodge;
	int res1;
	int res2;
	int res3;
	int res4;
	int res5;
	int res6;
	int crit;
	float crithurt;
	int skillrate;
	int skilldodge;
	int gender;
	int reborncount;
	int cultivaltion;
	int occupation;
	int type;
	int race;
	float speed;
	int level;
	int darklight;
	int ink;
	int rage;

public:
	PlayerWrapper() 
		: range(0), minatk(0), maxatk(0), def(0), atkrate(0), dodge(0), res1(0), res2(0), res3(0), res4(0), res5(0), res6(0), 
		  crithurt(0.0f), crit(0), skillrate(0), skilldodge(0), gender(0), reborncount(0), cultivaltion(0), occupation(0), type(0),
		  race(0), speed(0), level(0),darklight(0),ink(0),rage(0)
	{
	}

	PlayerWrapper(PlayerInfo * o) : range(0), object(o) { }

	void SetRange(float r) { range = r; }

	int GetMP()
	{
		return 10000;
	}

	int GetSp( )
	{
		return 10000;
	}

	bool IsRiding()
	{
		return false;
	}

	bool IsUsingWeapon(int weapon)
	{
		return true;
	}

	bool HasSkillBook( SID id )
	{
		return true;
	}

	bool SetDecsp(int sp)
	{
		return true;
	}

	bool SetDistance(float d)
	{
		return true;
	}

	float GetWeapondistance()
	{
		return -1;
	}

	bool SetDecmp(int m)
	{
		return true;
	}

	bool SetPray(bool)
	{
		return true;
	}

	bool SetInform(bool)
	{
		return true;
	}

	
	bool SetPerform(bool)
	{
		return true;
	}

	bool SetDamage(int)
	{
		return true;
	}
	int GetPerform()
	{
		return 1;
	}

	int GetDamage()
	{
		return 1;
	}
	float GetRange()
	{
		return range;
	}
	bool SetCheckbook(int i) { return true;}
    int GetCheckbook() { return 0;}
    bool SetCheckmoney(int m) { return true; }
    int GetCheckmoney() { return 0;}
    bool SetUsebook(int i) {return true; }
    int GetUsebook() { return 0;}
    bool SetUsemoney(int m) { return true; }
    int GetUsemoney() { return 0;}

	void	SetMinatk(int atk) { minatk = atk; }
	int		GetMinatk() { return minatk; }
	void	SetMaxatk(int atk) { maxatk = atk; }
	int		GetMaxatk() {return maxatk; }
	void	SetDef(int d) { def = d; }
	int		GetDef() { return def; }
	void	SetAtkrate(int v) { atkrate = v; } 
	int		GetAtkrate() { return atkrate; }
	void	SetDodge(int v) { dodge = v; }
	int		GetDodge() { return dodge; }
	void	SetRes1(int v) { res1 = v; }
	int		GetRes1() { return res1; }
	void	SetRes2(int v) { res2 = v; }
	int		GetRes2() { return res2; }
	void	SetRes3(int v) { res3 = v; }
	int		GetRes3() { return res3; }
	void	SetRes4(int v) { res4 = v; }
	int		GetRes4() { return res4; }
	void	SetRes5(int v) { res5 = v; }
	int		GetRes5() { return res5; }
	void	SetRes6(int v) { res6 = v; }
	int		GetRes6() { return res6; }
	void	SetCrit(int v) { crit = v; }
	int		GetCrit() { return crit; }
	void	SetCrithurt(float v) { crithurt = v; }
	float	GetCrithurt() { return crithurt; }
	void	SetSkillrate(int v) { skillrate = v; }
	int		GetSkillrate() { return skillrate; }
	void	SetSkilldodge(int v) { skilldodge = v; }
	int		GetSkilldodge() { return skilldodge; }
	void	SetGender(int v) { gender = v; }
	int		GetGender() { return gender; }
	void	SetReborncount(int v) { reborncount = v; }
	int		GetReborncount() { return reborncount; }
	void	SetCultivation(int v) { cultivaltion = v; }
	int		GetCultivation() { return cultivaltion; }
	void	SetOccupation(int v) { occupation = v; }
	int		GetOccupation() { return occupation; }
	void	SetType(int v) { type = v; } 
	int		GetType() { return type; }
	void	SetRace(int v) { race = v; }
	int		GetRace() { return race; }
	void	SetSpeed(float v) { speed = v; }
	float	GetSpeed() { return speed; }
	void	SetLevel(int v) { level = v; }
	int		GetLevel() { return level; }

	void SetDarklight(int v) { darklight = v; }
	int GetDarklight() { return darklight; }

	void SetInk(int v) { ink = v; }
	int GetInk() { return ink; }

	void SetRage(int v) { rage = v; }
	int GetRage() { return rage; }

	int GetSkilllevel(int id)
	{
		return SkillWrapper::Instance()->GetLevel(id);
	}
	
};

class TargetWrapper
{
public:
	int id;
	TargetWrapper(int i) : id(i) {}

	bool IsValid() { return -1 != id; }
};


}

#endif

