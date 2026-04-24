/********************************************************************
	created:	2006/08/25
	author:		kuiwu
	
	purpose:	use lua to config rank display
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "vector.h"
#include <AWString.h>

class CLuaState;




class CLuaRankDisp  
{
public:
	CLuaRankDisp();
	virtual ~CLuaRankDisp();

	bool Init();
	void Release();
	
	
	bool GetDisp(int id, int rank, int last_rank, const AWString& name,
				int prof, __int64 val, int val2 ,AWString& disp);


private:
	CLuaState * m_pState;

};


