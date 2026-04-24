// Filename	: EC_UIData.h
// Creator	: Fu Chonggang
// Date		: 2011/04/28

#include "EC_UIData.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUICommon.h"
#include "awscriptfile.h"

bool CECUIData::Init()
{
	return LoadAllData();
}

void CECUIData::Release()
{
	if (m_vecTaskZhuxian.size()>0)
	{
		for (unsigned int i=0; i<m_vecTaskZhuxian.size(); ++i)
		{
			delete m_vecTaskZhuxian[i];
		}
		m_vecTaskZhuxian.clear();
	}
}

#define LOADONETYPEDATA(info) \
{\
bool bRet = info;\
if(!bRet)\
return false; \
}

bool CECUIData::LoadAllData()
{
	LOADONETYPEDATA(LoadTaskZhuxian());
	return true;
}

bool CECUIData::LoadTaskZhuxian()
{
	AWScriptFile ScriptFile;
	if (ScriptFile.Open("configs\\zhuxiantask.txt"))
	{
		int index = 0;
		while(ScriptFile.GetNextToken(true))
		{
			if (ScriptFile.m_szToken[0]==_AL('/') && ScriptFile.m_szToken[1] == _AL('/'))
			{
				ScriptFile.SkipLine();
				continue;
			}
			TaskZhuxian* taskPage = new TaskZhuxian;
			taskPage->iProfRace = a_atoi(ScriptFile.m_szToken);
			
			if(!ScriptFile.GetNextToken(false))
				continue;			
			taskPage->strName = ScriptFile.m_szToken;
			
			if(!ScriptFile.GetNextToken(false))
				continue;
			taskPage->iLevel = a_atoi(ScriptFile.m_szToken);
			
			if(!ScriptFile.GetNextToken(false))
				continue;
			taskPage->strBackGround = AC2AS(ScriptFile.m_szToken);
			
			if(!ScriptFile.GetNextToken(false))
				continue;			
			CSplit sp(ScriptFile.m_szToken);
			CSPLIT_STRING_VECTOR vec = sp.Split(_AL(","));
			for( unsigned int ii = 0; ii < vec.size(); ii++ )
			{
				taskPage->vecIds.push_back(a_atoi(vec[ii]));
			}
			
			if(!ScriptFile.GetNextToken(false))
				continue;
			AUICTranslate trans;
			taskPage->strIntro = trans.Translate(ScriptFile.m_szToken);
			m_vecTaskZhuxian.push_back(taskPage);
		}
		
		ScriptFile.Close();
	}
	else
	{
		AUI_ReportError(__LINE__, "configs\\zhuxiantask.txt open error");
		return false;
	}
	return true;
}