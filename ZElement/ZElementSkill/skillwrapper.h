#ifndef __SKILL_SKILLWRAPPER_H
#define __SKILL_SKILLWRAPPER_H

#pragma warning(disable:4786)
#include <hashmap.h>

namespace GNET
{

#pragma pack(1)

struct PersistentData
{
	int level;      // »ù´¡¼¶±ð

	PersistentData(int _t = 0, int _l = 1) : level(_l){ }
};

typedef abase::hash_map<unsigned int, PersistentData, abase::_hash_function> StorageMap;
typedef abase::hash_map<unsigned int, int, abase::_hash_function> ProficiencyMap;

struct cmd_skill_data
{
	size_t count;
	struct data
	{
		short id;
		char  level;
	}list[1];
	size_t count2;
	struct data2
	{
		short id;
		int	proficiency; 
	}list2[1];
};

#define MAX_SKILL_ELEM_NUM 10

struct cmd_skill_elems_data
{
	int count;
	struct SkillElems
	{
		int curr_num; 
		unsigned short ids[MAX_SKILL_ELEM_NUM];
		SkillElems() : curr_num(0) { memset(ids, 0, sizeof(ids)); }
		SkillElems(const SkillElems& elems)
		{
			curr_num = elems.curr_num;
			memcpy(ids, elems.ids, sizeof(ids));
		}

		SkillElems& operator=(const SkillElems& elems)
		{
			curr_num = elems.curr_num;
			memcpy(ids, elems.ids, sizeof(ids));
			return *this;
		}
	};
	struct ComboSkill
	{
		int skillid;
		SkillElems elems;
	}skillelems[1];
	
};

typedef abase::hash_map<unsigned int, cmd_skill_elems_data::SkillElems, abase::_hash_function> SkillElemsMap;

#pragma pack()

class SkillWrapper
{
	StorageMap map;
	StorageMap extraMap;
	StorageMap fashionMap;
	ProficiencyMap profmap;
	SkillWrapper();
	static SkillWrapper instance;
	int commonlevel;
	SkillElemsMap   semap;

public:
	static SkillWrapper* Instance() { return &instance; }
	~SkillWrapper();
	void LoadData(cmd_skill_data* data);
	int  GetLevel(unsigned int id);
	int	 GetLearnLevel(unsigned int id);
	int  SetLevel(unsigned int id, int level);	
	int  SetExtraSkillLevel(unsigned int id, int level);
	int  SetFashionSkillLevel(unsigned int id, int level);
	int  GetSum(int rank);
	void SetCommonLevel(int n) { commonlevel = n; }
	int  GetCommonLevel() { return commonlevel; }
	int	 GetProficiency(unsigned int id);
	void SetProficiency(unsigned int id, int prof);

	void LoadSkillElemsData(cmd_skill_elems_data* data);
	unsigned short* GetUserSkillElems(int skillid, int& num);
	void UpdateComboSkillElems(int skillid, int* data, int num);
};

}

#endif

