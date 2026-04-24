/*
 * FILE: MODELVIEWSKill.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Ma Yuanzheng 2010.9.26
 *
 * HISTORY:
 *
 * Copyright (c) 2010 ZhuXian Studio, All Rights Reserved.
 */

//#include "stdafx.h"
#include "Global.h"
#include "MODELVIEWSkill.h"
#include "..\ZElementSkill\skillwrapper.h"
#include "..\ZElementSkill\skill.h"


///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


namespace GNET
{


MODELVIEWSkill::~MODELVIEWSkill(){
}


	// 0:成功               1:SP不足            2:玩家级别不够        
	// 3:前提技能等级不足   4:达到学习上限　　　5:错误ID
	// 6:职业不满足         7:天赋点不足        8:转生次数不足
	// 9:声望不足           10:仙魔条件不满足   11:没有选择显示这类技能
int MODELVIEWSkill::CanLearnSkillInSomeCondition(ID id, LearnRequirement& info, int ilevel,int iCategory)
{
	Skill* s =  (Skill*) ElementSkill::Create(id, ilevel);
	if(!s)
		return 5;
	const SkillStub * stub = s->GetStub();
	int ret = 0;
	switch(stub->occupation) {
	case OCCUPATION_TAOIST:
	   	if((info.cultivation&0x01)==0)
			ret = 10;
		break;
	case OCCUPATION_EVIL:
	   	if((info.cultivation&0x02)==0)
			ret = 10;
		break;
	case OCCUPATION_BUDDHA:
		if((info.cultivation&0x04)==0)
			ret = 10;
		break;
	case OCCUPATION_SPOUSE:
	   	if((info.cultivation&0x08)==0)
			ret = 10;
		break;
	case 255:
		break;
	case OCCUPATION_LIFE:
		if(!(iCategory&CATEGORY_LIFE))
			ret = 11;
		break;
	case OCCUPATION_SECT:
		if(!(iCategory&CATEGORY_SECT))
			ret = 11;
		break;
	case OCCUPATION_FAMILY:
		if(!(iCategory&CATEGORY_FAMILY))
			ret = 11;
		break;
	default:
		if(info.profession==0){//人族 全部职业的技能都显示，所以认为他是每个职业的最高重
			if(!(Rank::SameBranch(14,stub->occupation)
				||Rank::SameBranch(17,stub->occupation)
				||Rank::SameBranch(20,stub->occupation)
				||Rank::SameBranch(23,stub->occupation)
				||Rank::SameBranch(29,stub->occupation)))
				ret = 6;
		}
		else if(!Rank::SameBranch(info.profession,stub->occupation))
			ret = 6;
		break;
	}

	if(ret ==0){
		g_Log.Log("%d",stub->id);
		g_Log.Log("%d",stub->occupation);
		g_Log.Log((CString)stub->name);
		g_Log.Log("%s",stub->action);
	}

	s->Destroy();
	return ret;
}


}