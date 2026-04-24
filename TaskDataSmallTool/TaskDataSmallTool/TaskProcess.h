//
//Just for compile usage!!!!
//
#ifndef __TASK_PROCESS__
#define __TASK_PROCESS__

#include "stdafx.h"



///////////////////////////////////////////////////////////////////////////////
#ifndef SIZE_OF_ARRAY
#define SIZE_OF_ARRAY(x) (sizeof(x) / sizeof((x)[0]))
#endif

#define LOG_DELETE(p) \
if (p)\
{\
	delete p;\
	p = NULL;\
	g_ulDelCount++;\
}

#define LOG_DELETE_ARR(p) \
if (p)\
{\
	delete[] p;\
	p = NULL;\
	g_ulDelCount++;\
}

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

extern unsigned long g_ulNewCount;
extern unsigned long g_ulDelCount;


//============================================================================
struct ActiveTaskEntry
{
};

struct ActiveTaskList
{
};

union TaskGlobalData
{
};

#ifndef _TASK_CLIENT
#define _TASK_CLIENT
#endif

//============================================================================

///////////////////////////////////////////////////////////////////////////////


#endif //__TASK_PROCESS__