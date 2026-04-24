#include "skillwrapper.h"
#include "skill.h"

namespace GNET{

	SkillWrapper SkillWrapper::instance;

	SkillWrapper::SkillWrapper():commonlevel(0)
	{
	}

	SkillWrapper::~SkillWrapper()
	{
		map.clear();
	}

	void SkillWrapper::LoadData(cmd_skill_data* pdata)
	{
		map.clear();
		PersistentData skill;
		for (int i=0; i < (int)pdata->count; i++)
		{
			const cmd_skill_data::data& item = pdata->list[i];
			skill.level = item.level;
			map[item.id] = skill;
		}
		
		char *pcdata = (char *)pdata;
		pcdata += sizeof(size_t);
		pcdata += pdata->count*sizeof(cmd_skill_data::data);
		size_t count2 = *(size_t *)pcdata;
		pcdata += sizeof(size_t);
		cmd_skill_data::data2 *pdata2 = (cmd_skill_data::data2 *)pcdata;
		profmap.clear();
		for (int j = 0; j < (int)count2; ++j)
		{
			const cmd_skill_data::data2 &item = pdata2[j];
			profmap[item.id] = item.proficiency;
		}
	}

	int  SkillWrapper::GetLevel(unsigned int id)
	{
		StorageMap::iterator it = map.find(id);
		if(it != map.end() )
			return it->second.level;
		it = extraMap.find(id);
		if(it != extraMap.end())
			return it->second.level;
		it = fashionMap.find(id);
		if(it != fashionMap.end())
			return it->second.level;
		return 0;
	}

	int SkillWrapper::GetLearnLevel(unsigned int id)
	{
		StorageMap::iterator it = map.find(id);
		if(it != map.end() )
			return it->second.level;
		return 0;
	}

	int  SkillWrapper::SetLevel(unsigned int id, int level)
	{
		map[id].level = level;
		return level;
	}

	int  SkillWrapper::SetExtraSkillLevel(unsigned int id, int level)
	{
		extraMap[id].level = level;
		return level;
	}

	int  SkillWrapper::SetFashionSkillLevel(unsigned int id, int level)
	{
		fashionMap[id].level = level;
		return level;
	}

	int  SkillWrapper::GetSum(int rank)
	{
		int sum=0;
		for(StorageMap::iterator it=map.begin(),ie=map.end();it!=ie;++it)
		{
			const SkillStub* stub = SkillStub::GetStub(it->first);
			if(!stub)
				continue;
			if(stub->occupation==rank&&stub->talent_type==0)
				sum += it->second.level;
		}
		return sum;
	}

	int SkillWrapper::GetProficiency(unsigned int id)
	{
		ProficiencyMap::iterator it = profmap.find(id);
		if (it != profmap.end())
			return it->second;
		else
			return 0;
	}

	void SkillWrapper::SetProficiency(unsigned int id, int prof)
	{
		profmap[id] = prof;
	}
	
	void SkillWrapper::LoadSkillElemsData(cmd_skill_elems_data* data)
	{
		semap.clear();
		if(data->count < 0) return;
		for(int i = 0; i < data->count; ++i)
		{
			int skill_id = data->skillelems[i].skillid;
			semap[skill_id] = data->skillelems[i].elems;
		}
	}

	unsigned short* SkillWrapper::GetUserSkillElems(int skillid, int& num)
	{
		SkillElemsMap::iterator it = semap.find(skillid);
		if(it != semap.end())
		{
			num = it->second.curr_num;
			return it->second.ids;
		}
		num = 0;
		return NULL;
	}
	
	void SkillWrapper::UpdateComboSkillElems(int skillid, int* data, int num)
	{
		SkillElemsMap::iterator it = semap.find(skillid);
		if(it != semap.end())
		{
			it->second.curr_num = num;
			for(int i = 0; i< num; ++i)
				it->second.ids[i] = data[i];
		}
		else
		{
			cmd_skill_elems_data::SkillElems elems;
			elems.curr_num = num;
			for(int i = 0; i< num; ++i)
				elems.ids[i] = data[i];
			semap[skillid] = elems;
		}

	}
}
