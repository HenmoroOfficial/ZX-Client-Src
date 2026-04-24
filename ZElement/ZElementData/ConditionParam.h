#ifndef _AI_CONDITION_PARAM_H_
#define _AI_CONDITION_PARAM_H_

#include "PolicyType.h"

class CConditionParam
{

public:
	CConditionParam(){}
	~CConditionParam(){}
	
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class CConditionParam_Time_Come :public CConditionParam
{

public:
	C_TIME_COME m_Data;
	
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class CConditionParam_Path_End_Point :public CConditionParam
{

public:
	C_PATH_END_POINT m_Data;
	
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class CConditionParam_Attack_By_Skill :public CConditionParam
{

public:
	C_ATTACK_BY_SKILL m_Data;
	
};



//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class CConditionParam_HP_Less :public CConditionParam
{

public:
	C_HP_LESS m_Data;
	
};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class CConditionParam_Random :public CConditionParam
{

public:
	C_RANDOM m_Data;

};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class CConditionParam_Start_Attack :public CConditionParam
{

public:
	//Not ready
	
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
class CConditionParam_Var :public CConditionParam
{

public:
	C_VAR m_Data;
	
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
class CConditionParam_Constant :public CConditionParam
{

public:
	C_CONSTANT m_Data;
	
};


#endif