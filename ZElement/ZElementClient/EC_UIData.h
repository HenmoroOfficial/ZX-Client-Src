// Filename	: EC_UIData.h
// Creator	: Fu Chonggang
// Date		: 2011/04/28
// 多个界面公用的数据可以放到这里

#pragma once

#include "AAssist.h"
#include "vector.h"

struct TaskZhuxian
{
	int			iProfRace;	// ProfFilter
	ACString	strName;
	int			iLevel;
	AString		strBackGround;
	ACString	strIntro;
	abase::vector<int> vecIds;
};

typedef abase::vector<TaskZhuxian*> TaskZhuxianVector;

class CECUIData
{

public:
	CECUIData() {}
	~CECUIData() {}

	bool Init();
	void Release();

public:		
	TaskZhuxianVector m_vecTaskZhuxian;


protected:

	bool LoadAllData();
	bool LoadTaskZhuxian();
};
