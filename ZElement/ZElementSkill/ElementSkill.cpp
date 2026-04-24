// ElementSkill.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ElementSkill.h"
#include "playerwrapper.h"
#include "skillwrapper.h"
#include "skill.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

namespace GNET{

	
ELEMENTSKILL_API void SkillTimer()
{
}

ElementSkill* ElementSkill::Create(ID id, int n)
{
	return Skill::Create(id, n);
}

void ElementSkill::Destroy() 
{ 
	delete this; 
}

int ElementSkill::LearnCondition(ID id, LearnRequirement& info, int ilevel )
{
	Skill* s =  Skill::Create(id, ilevel);
	if(!s)
		return 5;
	const SkillStub * stub = s->GetStub();
	int ret = 0;
	if(ilevel<1 || ilevel> stub->maxlearn)
		ret = 4;
	else if(stub->occupation==OCCUPATION_TAOIST)
	{
	   	if((info.cultivation&0x01)==0)
			ret = 10;
	}
	else if(stub->occupation==OCCUPATION_EVIL)
	{
	   	if((info.cultivation&0x02)==0)
			ret = 10;
	}
	else if(stub->occupation==OCCUPATION_BUDDHA)
	{
	   	if((info.cultivation&0x04)==0)
			ret = 10;
	}
	else if(stub->occupation==OCCUPATION_SPOUSE)
	{
	   	if((info.cultivation&0x08)==0)
			ret = 10;
	}
	else if(stub->occupation==OCCUPATION_DEITY_TAOIST)
	{
		if((info.cultivation&0x01)==0 || info.deityLevel <=0 || info.deityLevel < stub->GetDivinityLevel(ilevel) || info.deityExp < stub->GetDivinityExp(ilevel))
			ret = 12;
	}
	else if(stub->occupation==OCCUPATION_DEITY_EVIL)
	{
		if((info.cultivation&0x02)==0 || info.deityLevel <=0 || info.deityLevel < stub->GetDivinityLevel(ilevel) || info.deityExp < stub->GetDivinityExp(ilevel))
			ret = 12;
	}
	else if(stub->occupation==OCCUPATION_DEITY_BUDDHA)
	{
		if((info.cultivation&0x04)==0 || info.deityLevel <=0 || info.deityLevel < stub->GetDivinityLevel(ilevel) || info.deityExp < stub->GetDivinityExp(ilevel))
			ret = 12;
	}
	else if(stub->occupation!=255 && stub->occupation!=OCCUPATION_LIFE && stub->occupation!=OCCUPATION_SECT && !Rank::SameBranch(info.profession,stub->occupation))
		ret = 6;
	if(info.level<stub->GetRequiredLevel(ilevel))
		ret = 2;
	for(int i = 0; i < stub->preskillnum; i ++)
	{
		if(stub->GetPreSkillID(i))
		{
			if(SkillWrapper::Instance()->GetLearnLevel(stub->GetPreSkillID(i))<stub->GetPreSkillSP(i))
				ret = 3;
		}
		else
		{
			if(stub->GetPreSkillSP(i) && SkillWrapper::Instance()->GetSum((Rank::GetBase(stub->occupation)))<stub->GetPreSkillSP(i))
				ret = 3; 
		}
	}
	
	if(stub->talent_type)
	{
		if(info.talent<1)
			ret = 7;
	}
	else if (stub->use_proficiency)
	{
		if (SkillWrapper::Instance()->GetProficiency(id) < stub->GetRequiredProficiency(ilevel))
			ret = 11;
	}
	else if(stub->occupation!=OCCUPATION_DEITY_TAOIST && stub->occupation!=OCCUPATION_DEITY_EVIL && stub->occupation!=OCCUPATION_DEITY_BUDDHA && stub->credittype==0)
	{
		if(info.sp<1)
			ret = 1;
	}
	if(stub->reborncount && stub->reborncount>info.reborncount)
		ret = 8;
	if(stub->credittype>0)
	{
		if(stub->credittype>=info.repu_count)
			ret = 9;
		else if(info.region_repu[stub->credittype]<stub->GetCreditCost(ilevel))
			ret = 9;
	}
	s->Destroy();
	return ret;
}

const char* ElementSkill::GetIcon(ID id)
{
	const SkillStub * s = SkillStub::GetStub(id);
	if(s)
		return s->icon;
	return "";
}

const char* ElementSkill::GetNativeName(ID id)
{
	const SkillStub * s = SkillStub::GetStub(id);
	if(s)
		return s->nativename;
	return "";
}

const wchar_t* ElementSkill::GetName(ID id)
{
	const SkillStub * s = SkillStub::GetStub(id);
	if(s)
		return s->name;
	return L"";
}

char ElementSkill::GetType(ID id)
{
	const SkillStub * s = SkillStub::GetStub(id);
	if(s)
		return s->type;
	return 0;
}

ElementSkill::ID ElementSkill::NextSkill(ID start)
{
	SkillStub::Map& m = SkillStub::GetMap();
	SkillStub::Map::iterator it; 
	if(start == 0 )
		it = m.begin();
	else
	{
		it = m.find(start);
		++it;
	}
	if(it==m.end())
		return 0;
	else
		return it->second->id;
}

const char* ElementSkill::GetAction(ID id, unsigned char type) 
{ 
	const SkillStub * stub = SkillStub::GetStub(id);
	return (stub&&type<sizeof(stub->action)/sizeof(stub->action[0])) ? stub->action[type] : "";
}

const VisibleState* VisibleState::Query(int profession, int id)
{
	return VisibleStateImp::Query(profession, id);
}

const TeamState* TeamState::Query(int id)
{
	return TeamStateImp::Query(id);
}

int ElementSkill::GetVersion()
{
	return 6;
}


void ElementSkill::LoadSkillData(void * data)
{
	SkillWrapper::Instance()->LoadData((cmd_skill_data*)data);
}

void ElementSkill::LoadSkillElemsData(void * data)
{
	SkillWrapper::Instance()->LoadSkillElemsData((cmd_skill_elems_data*)data);
}

int ElementSkill::SetLevel(ID id, int level)
{
	return SkillWrapper::Instance()->SetLevel(id, level);
}

int ElementSkill::SetExtraSkillLevel(ID id, int level)
{
	return SkillWrapper::Instance()->SetExtraSkillLevel(id, level);
}

int ElementSkill::SetFashionSkillLevel(ID id, int level)
{
	return SkillWrapper::Instance()->SetFashionSkillLevel(id, level);
}

int ElementSkill::GetExecuteTime(ID id, int level)
{
	Skill* s =  Skill::Create(id, level);
	if(!s)
		return 0;
	
	int ret = s->GetExecuteTime(); 
	s->Destroy();
	return ret;
}

unsigned short* ElementSkill::GetComboSkillElems(int skillid, int& num)
{
	return SkillWrapper::Instance()->GetUserSkillElems(skillid, num);
}

void ElementSkill::UpdateComboSkillElems(int skillid, int* data, int num)
{
	SkillWrapper::Instance()->UpdateComboSkillElems(skillid, data, num);
}
}
