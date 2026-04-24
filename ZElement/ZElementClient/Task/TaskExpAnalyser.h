#ifndef _TASKEXPANALYSER_H_
#define _TASKEXPANALYSER_H_

#include "vector.h"
#include "TaskInterface.h"

#pragma pack(1)

enum
{
	enumTaskCalcExp,       //玩家等级
	enumTaskCalcGlobalVal, //全局变量
};

enum
{
	enumTaskExpVar,
	enumTaskExpConst,
	enumTaskExpOprt,
};

struct TASK_EXPRESSION
{
	int type;
	float value;
};

#pragma pack()

typedef abase::vector<TASK_EXPRESSION> TaskExpressionArr;

class TaskExpAnalyser
{
public:

	TaskExpAnalyser();
	~TaskExpAnalyser();

protected:

	int pos;
	const char* sentence;
	TaskExpressionArr* exp_arr;

protected:

	bool add();
	bool multiply();
	bool bracket();

public:

	void KickBlank(char *szSen);
	bool Analyse(const char* szSentence, TaskExpressionArr& aExp);
	float Run(TaskInterface* pTask, const TaskExpressionArr& aExp, int nCalcType);
};

#endif
