// Filename	: EC_NewHelpGuide.cpp
// Creator	: Wang Dongliang
// Date		: 2013/07/10
// Desc		: 指引系统
#include "EC_NewHelpGuide.h"
#include <assert.h>
#include "ScriptValue.h"
#include "LuaState.h"
#include "LuaUtil.h"
#include "LuaScript.h"
#include "LuaFuncCommon.h"
#include "aui\\CSplit.h"
#include "AUI\\AUITreeView.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Configs.h"
#include "EC_World.h"
#include "EC_HostPlayer.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_Secretary.h"
#include "EC_Global.h"
#include "EC_TaskInterface.h"
#include "TaskTemplMan.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_Shortcut.h"
#include "EC_Algorithm.h"
#include "AMSoundEngine.h"
#include "AMSoundBufferMan.h"
#include "DlgBase.h"
#include "DlgInventory.h"


#define  GUIDE_STEP_NAME  "HelpGuideStep"
#define  GUIDE_CONTENT_NAME   "HelpGuideContent"
#define  AIM_GUIDE_INFO "AimGuideData"
#define  AIM_GUIDE_INFO_2 "AimGuideData2"
#define  LUA_GUIDEDATA_FILE  "Interfaces\\script\\interfaces\\guidedata.lua"

static const char* strHintUI[] = 
{
   "Win_Help_Up2013",
   "Win_Help_Down2013",
   "Win_Help_Left2013",
   "Win_Help_Right2013"
};
static int GetTaskIdForStep(int stepid,const CSplit::VectorAWString& ids)
{
	int taskid = -1;
	int race = g_pGame->GetGameRun()->GetHostPlayer()->GetRace();
    if(stepid == ESTEP_TASK_PATHFIND || stepid == ESTEP_TASK_FIGHTWAR)
	{
		if(ids.size() >= 3)
		{
		   //taskid = ids[race].ToInt();
		   if(RACE_HUMAN == race)
		   {
		      taskid = ids[0].ToInt();
		   }
		   else if(RACE_ORC == race)
		   {
		      taskid = ids[1].ToInt();
		   }
		   else if(RACE_EVILKIND == race)
		   {
		      taskid = ids[2].ToInt();
		   }
		}
	}
	else if(stepid == ESTEP_TASK_REFINING || stepid == ESTEP_TASK_PET)
	{
	    if(ids.size() >= 1)
		{
		   taskid = ids[0].ToInt();
		}
	}
	return taskid;
}

int GetTaskIdForStep(int stepid)
{
	int taskid = -1;
	CECNewHelpGuide* pNewGuide = g_pGame->GetGameRun()->GetNewHelpGuide();
	if(pNewGuide)
	{
		CECNewHelpGuide::SGuideStep* pGuideStep =  pNewGuide->FindGuideStepById(stepid);
        if(pGuideStep)
		{
			CSplit s = pGuideStep->strControl;
			CSplit::VectorAWString vec = s.Split(L",");
			taskid = GetTaskIdForStep(stepid,vec);
		}
	}
	return taskid;
}

static int GetItemIdForStep(int stepid,const CSplit::VectorAWString& ids)
{
    int itemid = -1;
	
	if(stepid == ESTEP_ITEM_EQUIPTALISMAN_4_2)
	{
		 if(ids.size() >= 3)
		 {
			 int race = g_pGame->GetGameRun()->GetHostPlayer()->GetRace();
			 if(race == RACE_HUMAN)
			 {
				itemid = ids[0].ToInt(); 
			 }
			 else if(race == RACE_ORC)
			 {
				itemid = ids[1].ToInt();
			 }
			 else if(race == RACE_EVILKIND)
			 {
				 itemid = ids[2].ToInt();
			 }
		 }
	}
	if(stepid == ESTEP_ITEM_EQUIPWEAPON || 
	   stepid == ESTEP_ITEM_USEGIFT_2_2)
	{
		
	   if(ids.size() >= 9)
	   {
		   int profrace =  g_pGame->GetGameRun()->GetHostPlayer()->GetProfRace();

			 if(profrace == PROFRACE_SHAOXIA ||
				profrace == PROFRACE_GUIWANG ||
				profrace == PROFRACE_HEHUAN  ||
				profrace == PROFRACE_QINGYUN ||
				profrace == PROFRACE_TIANYIN ||
				profrace == PROFRACE_GUIDAO  ||
				profrace == PROFRACE_FENGXIANG)
			 {
				 itemid = ids[0].ToInt();
			 }
			 else if(profrace == PROFRACE_JIULI)
			 {
				 itemid = ids[1].ToInt();
			 }
			 else if(profrace == PROFRACE_LIESHAN)
			 {
				 itemid = ids[2].ToInt();
			 }
			 else if(profrace == PROFRACE_HUAIGUANG)
			 {
				 itemid = ids[3].ToInt();
			 }
			 else if(profrace == PROFRACE_TIANHUA)
			 {
				 itemid = ids[4].ToInt();
			 }
			 else if(profrace == PROFRACE_CHENHUANG)
			 {
				 itemid = ids[5].ToInt();
			 }
			 else if(profrace == PROFRACE_TAIHAO)
			 {
				 itemid = ids[6].ToInt();
			 }
			 else if(profrace == PROFRACE_QIANJI)
			 {
				 itemid = ids[7].ToInt();
			 }
			 else if(profrace == PROFRACE_YINGZHAO)
			 {
				 itemid = ids[8].ToInt();
			 }
	   }
	}
	else if(stepid == ESTEP_ITEM_EQUIPTALISMAN_4_3 || 
		    stepid == ESTEP_ITEM_REFINING_5_3 ||
			stepid == ESTEP_ITEM_REFINING_5_4 ||
			stepid == ESTEP_ITEM_PET_11_3 ||
			stepid == ESTEP_ITEM_PET_11_10)
	{
       if(ids.size()>=1)
		  itemid = ids[0].ToInt();
	}
	return itemid;
}
int GetItemIdForStep(int stepid)
{
	int itemid = -1;
	CECNewHelpGuide* pNewGuide = g_pGame->GetGameRun()->GetNewHelpGuide();
	if(pNewGuide)
	{
		CECNewHelpGuide::SGuideStep* pGuideStep =  pNewGuide->FindGuideStepById(stepid);

		if(pGuideStep)
		{
			CSplit s = pGuideStep->strControl;
			CSplit::VectorAWString vec = s.Split(L",");
			itemid = GetItemIdForStep(stepid,vec);
		}
	}
	return itemid;
}
static int GetNpcIdForStep(int stepid,const CSplit::VectorAWString& ids)
{
	int npcid = -1;
	int race = g_pGame->GetGameRun()->GetHostPlayer()->GetRace();
	if(stepid == ESTEP_NPC_NPCTALK)
	{
		if(ids.size() >= 4)
		{
			//npcid = ids[race+1].ToInt();
			if(RACE_HUMAN == race)
			{
				npcid = ids[1].ToInt();
			}
			else if(RACE_ORC == race)
			{
				npcid = ids[2].ToInt();
			}
			else if(RACE_EVILKIND == race)
			{
				npcid = ids[3].ToInt();
			}
		}
	}
	else if(stepid == ESTEP_NPC_NPCREFINING)
	{
	    if(ids.size() >= 2)
		{
		   npcid = ids[1].ToInt();
		}
	}
	return npcid;
}
int GetNpcIdForStep(int stepid)
{
	int npcid = -1;
	CECNewHelpGuide* pNewGuide = g_pGame->GetGameRun()->GetNewHelpGuide();
	if(pNewGuide)
	{
		CECNewHelpGuide::SGuideStep* pGuideStep =  pNewGuide->FindGuideStepById(stepid);
        
		if(pGuideStep)
		{
			CSplit s = pGuideStep->strControl;
			CSplit::VectorAWString vec = s.Split(L",");
			npcid = GetNpcIdForStep(stepid,vec);
		}
	}
	return npcid;
}

static int GetGuideIdOfSkillAddPoint()
{
	int profrace =  g_pGame->GetGameRun()->GetHostPlayer()->GetProfRace();
	if(profrace == PROFRACE_UNKNOWN)
		return -1;
	if(profrace < PROFRACE_JIULI)
		return GUIDE_HELP_SKILLADDPOINT_3;
	else
	{
	    return GUIDE_HELP_SKILLADDPOINT_JIULI_3 + profrace - PROFRACE_JIULI;
	}
}
static bool CheckGuideState(int guideid)
{
	assert(guideid >= 0 && guideid < GUIDE_HELP_NUM);
	CECNewHelpGuide* pNewGuide = g_pGame->GetGameRun()->GetNewHelpGuide();
	if(pNewGuide)
	{
		return pNewGuide->CheckGuideState(guideid);
	}
	return false;
}
class CompareAimGuideInfo
{
public:
	CECNewHelpGuide::SortType m_St;
	bool	m_bInc;
	CompareAimGuideInfo(CECNewHelpGuide::SortType st, bool bInc=true) : m_St(st),m_bInc(bInc){}
	bool operator ()(CECNewHelpGuide::SAimGuideInfo & info1, CECNewHelpGuide::SAimGuideInfo & info2)
	{
		bool bRet = true;
		switch (m_St)
		{
		case CECNewHelpGuide::ST_REQLEVEL:
			bRet = info1.ulMinLevel < info2.ulMinLevel;
			break;
		case CECNewHelpGuide::ST_STARTLEVEL:
			bRet = info1.GetStarLevel() < info2.GetStarLevel();
			break;
		}
		if(m_bInc)
			return bRet;
		else
			return !bRet;
	}
};

int CECNewHelpGuide::SAimGuideInfo::GetStarLevel()
{
	int iLevel = g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel;
	int index = iLevel/15;
	assert(index>=0 && index<AIMGUIDE_STARTLEVEL_MAX);
	return istarLevel[index];
}
CECNewHelpGuide::CECNewHelpGuide()
{
	m_pState = NULL;
	m_iCurGuide  = -1;
	m_iCurStep   = 0;
	m_iCurType   = -1;
}

CECNewHelpGuide::~CECNewHelpGuide()
{
	Release();
}

bool CECNewHelpGuide::Init()
{
	m_pState = g_LuaStateMan.GetConfigState();
	assert(m_pState);

	m_pState->Lock();


	CLuaScript * pScript = m_pState->RegisterFile(LUA_GUIDEDATA_FILE);

	if (!pScript)
	{
		m_pState->Unlock();
		return false;
	}

	bool rt = LoadGuideData();

	assert(rt);
	
	m_pState->Unlock();
	return	rt;

}
void CECNewHelpGuide::Release()
{
    ReleaseGuideData();
	abase::vector<AString> rmTbls;

	rmTbls.push_back(GUIDE_STEP_NAME);
	rmTbls.push_back(GUIDE_CONTENT_NAME);
	rmTbls.push_back(AIM_GUIDE_INFO);
	rmTbls.push_back(AIM_GUIDE_INFO_2);

	m_pState->Lock();
	m_pState->UnRegister(LUA_GUIDEDATA_FILE, &rmTbls);
	m_pState->Unlock();

}
void CECNewHelpGuide::Reset()
{
	m_iCurGuide  = -1;
	m_iCurStep   = 0;
	m_iCurType   = -1;
}
bool CECNewHelpGuide::LoadGuideData()
{
	bool ret =  LoadGuideStep() && LoadGuideContent() && LoadAimGuideInfo() && LoadAimGuideInfo2();
	if(!ret)
		ReleaseGuideData();
	return ret;
}
void CECNewHelpGuide::ReleaseGuideData()
{
	GuideStepMap::iterator stepitr;
	for (stepitr = m_GuideStep.begin();stepitr != m_GuideStep.end();++stepitr)
	{
		SGuideStep* pGuideStep = stepitr->second;
		delete pGuideStep;
		pGuideStep = NULL;
	}
	m_GuideStep.clear();
	GuideContentMap::iterator contentitr;
	for (contentitr = m_GuideContent.begin();contentitr != m_GuideContent.end();++contentitr)
	{
		SGuideContent* pGuideContent = contentitr->second;
		delete pGuideContent;
		pGuideContent = NULL;
	}
	m_GuideContent.clear();

	m_AimGuideInfo.clear();
}
void CECNewHelpGuide::SortAimGuideInfo(SortType st,bool bInc)
{
   BubbleSort(m_AimGuideInfo.begin(), m_AimGuideInfo.end(), CompareAimGuideInfo(st,bInc));
}
bool CECNewHelpGuide::LoadAimGuideInfo()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;

	if (!m_pState->Call(AIM_GUIDE_INFO, "GetSelf", args, results))
	{
		return false;
	}

	ACString strText;
	CScriptValue& ret0 = results[0];

	for (int i = 0; i < (int)ret0.m_ArrVal.size(); i++)
	{
		SAimGuideInfo info;
		luaf::GetValue(info.iId,			ret0.m_ArrVal[i],   _AL("id"));
		luaf::GetValue(info.strName,		ret0.m_ArrVal[i],	_AL("name"));
		luaf::GetValue(info.strIcon,		ret0.m_ArrVal[i],   _AL("iconpath"));
		luaf::GetValue(info.strDay,	        ret0.m_ArrVal[i],	_AL("validday"));
		luaf::GetValue(info.strTime,		ret0.m_ArrVal[i],	_AL("validtime"));
		luaf::GetValue(info.strLevelInfo,	ret0.m_ArrVal[i],	_AL("levelreq"));
		luaf::GetValue(info.bReborn,		ret0.m_ArrVal[i],	_AL("needreborn"));
		luaf::GetValue(info.iPermitCount,	ret0.m_ArrVal[i],	_AL("permitcount"));
		luaf::GetValue(info.strCondition,	ret0.m_ArrVal[i],	_AL("cond"));
		luaf::GetValue(info.strNPC,			ret0.m_ArrVal[i],	_AL("npc"));
		luaf::GetValue(info.bTaskNpc,		ret0.m_ArrVal[i],	_AL("istasknpc"));
		luaf::GetValue(info.iNpcMapid,		ret0.m_ArrVal[i],	_AL("descmapid"));
		luaf::GetValue(info.strNpcPos,		ret0.m_ArrVal[i],	_AL("descpos"));
		luaf::GetValue(info.bIsTransport,	ret0.m_ArrVal[i],	_AL("istransport"));
		luaf::GetValue(info.strDesc,	    ret0.m_ArrVal[i],	_AL("desc"));
		
        //类型信息提取
        luaf::GetValue(strText,				ret0.m_ArrVal[i],	_AL("type"));
		CSplit sp = strText.GetBuffer(0);
		CSPLIT_STRING_VECTOR vec = sp.Split(_AL(","));
		assert(vec.size() == AIMGUIDE_TYPE_MAX);
		info.uiType = 0x0;
		for(int m=0;m<(int)vec.size();++m)
		{
		    if(a_atoi(vec[m]))
			  info.uiType |= 0x1 << m;
		}
		//任务ID信息提取
		luaf::GetValue(strText,				ret0.m_ArrVal[i],	_AL("taskid"));
		sp = strText.GetBuffer(0);
		vec = sp.Split(_AL(","));
		info.uiNumID = vec.size();

		for( unsigned int j = 0; j < info.uiNumID; ++j )
		{
		  info.uiTaskID[j] = a_atoi(vec[j]);
		}
        //星级信息提取
		luaf::GetValue(strText,				ret0.m_ArrVal[i],	_AL("starlevel"));
		sp = strText.GetBuffer(0);
		vec = sp.Split(_AL(","));
		assert(vec.size() == AIMGUIDE_STARTLEVEL_MAX);
		for (int k=0;k<(int)vec.size();++k)
		{
           info.istarLevel[k] = a_atoi(vec[k]);
		}
		//等级限制信息提取
		int len = info.strLevelInfo.GetLength();
		int loc = info.strLevelInfo.Find('-');
		info.ulMinLevel = 1;
		info.ulMaxLevel = 10000;
		if(-1==loc)
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			info.ulMaxLevel = a_atoi(info.strLevelInfo);
		}
		else if(loc+1 == len)
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
		}
		else
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			info.ulMaxLevel = a_atoi(info.strLevelInfo.Mid(loc+1));
		}

		m_AimGuideInfo.push_back(info);

	}
	return true;
	
}
bool CECNewHelpGuide::LoadAimGuideInfo2()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;

	if (!m_pState->Call(AIM_GUIDE_INFO_2, "GetSelf", args, results))
	{
		return false;
	}

	ACString strText;
	CScriptValue& ret0 = results[0];

	for (int i = 0; i < (int)ret0.m_ArrVal.size(); i++)
	{
		SAimGuideInfo info;
		luaf::GetValue(info.iId,			ret0.m_ArrVal[i],   _AL("id"));
		luaf::GetValue(info.strName,		ret0.m_ArrVal[i],	_AL("name"));
		luaf::GetValue(info.strIcon,		ret0.m_ArrVal[i],   _AL("iconpath"));
		luaf::GetValue(info.strLevelInfo,	ret0.m_ArrVal[i],	_AL("levelreq"));
		luaf::GetValue(info.bReborn,		ret0.m_ArrVal[i],	_AL("needreborn"));
		luaf::GetValue(info.strCondition,	ret0.m_ArrVal[i],	_AL("cond"));
		luaf::GetValue(info.strBtnName,	    ret0.m_ArrVal[i],	_AL("btnname"));
		luaf::GetValue(info.strDlgName,	    ret0.m_ArrVal[i],	_AL("dlgname"));
		luaf::GetValue(info.strDesc,	    ret0.m_ArrVal[i],	_AL("desc"));

		//类型信息提取
		luaf::GetValue(strText,				ret0.m_ArrVal[i],	_AL("type"));
		CSplit sp = strText.GetBuffer(0);
		CSPLIT_STRING_VECTOR vec = sp.Split(_AL(","));
		assert(vec.size() == AIMGUIDE_TYPE_MAX);
		info.uiType = 0x0;
		for(int m=0;m<(int)vec.size();++m)
		{
			if(a_atoi(vec[m]))
				info.uiType |= 0x1 << m;
		}
		//星级信息提取
		luaf::GetValue(strText,				ret0.m_ArrVal[i],	_AL("starlevel"));
		sp = strText.GetBuffer(0);
		vec = sp.Split(_AL(","));
		assert(vec.size() == AIMGUIDE_STARTLEVEL_MAX);
		for (int k=0;k<(int)vec.size();++k)
		{
			info.istarLevel[k] = a_atoi(vec[k]);
		}
		//等级限制信息提取
		int len = info.strLevelInfo.GetLength();
		int loc = info.strLevelInfo.Find('-');
		info.ulMinLevel = 1;
		info.ulMaxLevel = 10000;
		if(-1==loc)
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			info.ulMaxLevel = a_atoi(info.strLevelInfo);
		}
		else if(loc+1 == len)
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
		}
		else
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			info.ulMaxLevel = a_atoi(info.strLevelInfo.Mid(loc+1));
		}

		m_AimGuideInfo.push_back(info);
	}
	return true;
}
CECNewHelpGuide::SAimGuideInfo* CECNewHelpGuide::GetAimGuideInfo(int index)
{
   if(index<0 || index >= (int)m_AimGuideInfo.size())
	   return NULL;
   return &(m_AimGuideInfo[index]);

}
void CECNewHelpGuide::GetAimGuideIndexs(int type,abase::vector<int>& vecIndex)
{
    vecIndex.clear();
	int iLevel = g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel;
	bool bLevelSuit = false;
	for(int i=0;i<(int)m_AimGuideInfo.size();++i)
	{
		 bLevelSuit = false;

         if(!m_AimGuideInfo[i].bReborn)
			 bLevelSuit = (iLevel >= (int)m_AimGuideInfo[i].ulMinLevel);
		 else if(g_pGame->GetGameRun()->GetHostPlayer()->GetRebornCount() > 0)
			 bLevelSuit = (iLevel >= (int)m_AimGuideInfo[i].ulMinLevel);

	    if(bLevelSuit && m_AimGuideInfo[i].IsType(type))
			vecIndex.push_back(i);
	}
}
bool CECNewHelpGuide::LoadGuideStep()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;

	if (!m_pState->Call(GUIDE_STEP_NAME, "GetSelf", args, results))
	{
		return false;
	}

	CScriptValue& ret0 = results[0];

	unsigned int i, j;
	for (i = 0; i < ret0.m_ArrVal.size(); i++)
	{
		abase::vector<CScriptValue>& ret0ik = ret0.m_ArrVal[i].m_ArrKey;
		abase::vector<CScriptValue>& ret0iv = ret0.m_ArrVal[i].m_ArrVal;
		int id;
		BYTE type;
		AWString desc;
		AWString dlg;
		AWString control;

		for (j = 0; j < ret0ik.size(); j++)
		{
			CScriptValue& ret0ijk = ret0ik[j];
			if (ret0ijk.m_Type != CScriptValue::SVT_STRING)
			{
				return false;
			}
			AString key; 
			ret0ijk.RetrieveAString(key);
			if (strcmp(key, "id") == 0)
			{
				id = ret0iv[j].GetInt();
			}
			else if(strcmp(key,"type") == 0)
			{
			    type = ret0iv[j].GetInt();
			}
			else if (strcmp(key, "desc") == 0)
			{
				ret0iv[j].RetrieveAWString(desc);	
			}
			else if (strcmp(key, "dlg") == 0)
			{
				ret0iv[j].RetrieveAWString(dlg);
			}
			else if (strcmp(key, "control") == 0)
			{
				ret0iv[j].RetrieveAWString(control);
			}
			else
			{
				return false;
			}
		}

		SGuideStep* pGuideStep = new SGuideStep();
		assert(NULL != pGuideStep);
		pGuideStep->stepId = id;
		pGuideStep->type = type;
		pGuideStep->strDesc = desc;
		pGuideStep->strDlg = dlg;
		pGuideStep->strControl = control;

		m_GuideStep[id] = pGuideStep;

	}


	return true;
}
bool CECNewHelpGuide::LoadGuideContent()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;

	if (!m_pState->Call(GUIDE_CONTENT_NAME, "GetSelf", args, results))
	{
		return false;
	}

	CScriptValue& ret0 = results[0];

	unsigned int i, j;
	for (i = 0; i < ret0.m_ArrVal.size(); i++)
	{
		abase::vector<CScriptValue>& ret0ik = ret0.m_ArrVal[i].m_ArrKey;
		abase::vector<CScriptValue>& ret0iv = ret0.m_ArrVal[i].m_ArrVal;
		int id;
		int level_min = 1;
		int level_max = 1024;
		bool isrepeate = false;
		AWString steps;

		for (j = 0; j < ret0ik.size(); j++)
		{
			CScriptValue& ret0ijk = ret0ik[j];
			if (ret0ijk.m_Type != CScriptValue::SVT_STRING)
			{
				return false;
			}
			AString key; 
			ret0ijk.RetrieveAString(key);
			if (strcmp(key, "id") == 0)
			{
				id = ret0iv[j].GetInt();
			}
			else if(strcmp(key,"level_min") == 0)
			{
			    level_min = ret0iv[j].GetInt();
			}
			else if(strcmp(key,"level_max") == 0)
			{
                level_max = ret0iv[j].GetInt();
			}
			else if(strcmp(key, "steps") == 0)
			{
				ret0iv[j].RetrieveAWString(steps);
			}
			else if(strcmp(key,"isrepeate") == 0)
			{
			    isrepeate = ret0iv[j].GetBool();
			}
			else
			{
				return false;
			}
		}
        
		CSplit s = steps;
		CSplit::VectorAWString vec = s.Split(L",");
		assert(vec.size() > 0);
		SGuideContent* pGuideContent = new SGuideContent();
        assert(NULL != pGuideContent);
		pGuideContent->guideid = id;
		pGuideContent->level_min = level_min;
		pGuideContent->level_max = level_max;
		pGuideContent->isrepeate = isrepeate;
		for (int i = 0; i< (int)vec.size();++i)
		{
           pGuideContent->ids.push_back(vec[i].ToInt());
		}

		m_GuideContent[id] = pGuideContent;

	}


	return true;
}

CECNewHelpGuide::SGuideStep* CECNewHelpGuide::FindGuideStepById(int iStep)
{
	GuideStepMap::iterator itr = m_GuideStep.find(iStep);
	if(itr != m_GuideStep.end())
		return itr->second;
    return NULL;
}
CECNewHelpGuide::SGuideContent* CECNewHelpGuide::FindGuideContentById(int iGuide)
{
	GuideContentMap::iterator itr= m_GuideContent.find(iGuide);
	if(itr != m_GuideContent.end())
		return itr->second;
	return NULL;
}
void CECNewHelpGuide::SetGuideState(int iGuide)
{
   CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
   if(pSecretary)
	   pSecretary->SetGuideState(iGuide);
}
void CECNewHelpGuide::ReSetGuideState(int iGuide)
{
	CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
	if(pSecretary)
		pSecretary->ResetGuideState(iGuide);
}
bool CECNewHelpGuide::CheckGuideState(int iGuide)
{
	CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
	if(pSecretary)
		return pSecretary->CheckGuideState(iGuide);
	return false;
}
/*void CECNewHelpGuide::Enable(bool isEnable)
{
   if(isEnable)
	   ReSetGuideState(GUIDE_HELP_ENABLE);
   else
   {
	   SetGuideState(GUIDE_HELP_ENABLE);
	   StopGuide();
   }

}*/
bool CECNewHelpGuide::IsEnable()
{
	//return CheckGuideState(GUIDE_HELP_ENABLE);
	CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
	if(pSecretary)
		return pSecretary->IsHelpGuideEnable();
	return false;
}
bool CECNewHelpGuide::StartStep(int iStep)
{
	SGuideStep* pGuideStep = FindGuideStepById(iStep);
	assert(pGuideStep);
	if(pGuideStep->strDlg != L"" && pGuideStep->strControl != L"")
	{
		//assert( pGuideStep->type >=0 && pGuideStep->type <= 3);
		AString strDlg;
		AString strControl;
		PAUIDIALOG pDlg = NULL;
		PAUIOBJECT pControl = NULL;
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();


		int id = -1;
		//任务追踪界面
		if(pGuideStep->strDlg == L"HGForTaskTrace" && pGuideStep->strControl != L"")
		{

			CSplit s = pGuideStep->strControl;
			CSplit::VectorAWString vec = s.Split(L",");

			int taskId = -1;
		    taskId = GetTaskIdForStep(pGuideStep->stepId,vec);
			if(taskId == -1)
				return false;
			if( NULL == GetTaskTraceItemByTaskId(taskId))
				return false;
			strDlg = "Win_QuestMinion";
			strControl = "Tv_List";

			//此处是否需要强制显示任务追踪界面
			PAUIDIALOG pDlgTaskTrace = pGameUI->GetDialog(strDlg);
			if(pDlgTaskTrace && !pDlgTaskTrace->IsShow())
			    pDlgTaskTrace->Show(true);

			id = taskId;
		}
		//包裹界面
		else if(pGuideStep->strDlg == L"HGForInventory" && pGuideStep->strControl != L"")
		{
			int itemid = -1;
			CSplit s = pGuideStep->strControl;
			CSplit::VectorAWString vec = s.Split(L",");
			itemid = GetItemIdForStep(pGuideStep->stepId,vec);
			if(itemid == -1)
				return false;
			strDlg = "Win_Inventory";
			strControl = "Item_00";
			id = itemid;
		}
		//NPC对话界面
		else if(pGuideStep->strDlg == L"HGForNpcTalk" && pGuideStep->strControl != L"")
		{
			int npcindex = -1;
			CSplit s = pGuideStep->strControl;
			CSplit::VectorAWString vec = s.Split(L",");
			assert(vec.size()>=2);
			npcindex = vec[0].ToInt();
			strDlg = "Win_NPC";
			strControl = "Lst_Main";
			id = npcindex;
		}
		//NPC主线对话界面
		else if(pGuideStep->strDlg == L"HGForNpcTalkMain" && pGuideStep->strControl != L"")
		{
			int npcindex = -1;
			CSplit s = pGuideStep->strControl;
			CSplit::VectorAWString vec = s.Split(L",");
			assert(vec.size()>=1);
			npcindex = vec[0].ToInt();
			strDlg = "Win_NPC_Main";
			strControl = "Lst_Main";
			id = npcindex;
		}
		//战斗指引特殊处理
	    else if(pGuideStep->strDlg == L"HGForFighting" && pGuideStep->strControl != L"")
		{
		    //打开战斗指引界面
			strDlg = "Win_Help_Tab";
			strControl = "IDCANCEL";

			//此处强制显示战斗指引界面
			PAUIDIALOG pDlgFight = pGameUI->GetDialog(strDlg);
			if(pDlgFight && !pDlgFight->IsShow())
			{
				pDlgFight->Show(true);
				return true;
			}
			return false;
		}
		//脚本配置界面
		else
		{
			strDlg = WC2AS(pGuideStep->strDlg);
			strControl = WC2AS(pGuideStep->strControl);
		}
		pDlg = pGameUI->GetDialog(strDlg);
		pControl = pDlg->GetDlgItem(strControl);
		assert(NULL != pDlg);
		assert(NULL != pControl);
		//当前索引的界面或控件没有显示
		assert(pDlg->IsShow() && pControl->IsShow());
		if(!pDlg->IsShow() || !pControl->IsShow())
			return false;
		m_iCurType = pGuideStep->type;
	    if(m_iCurType != -1)
		{
	        pGameUI->CommonMessage(strHintUI[m_iCurType],"open",(int)pGuideStep,(int)id);
		    g_pGame->GetA3DEngine()->GetAMSoundEngine()->GetAMSoundBufferMan()->Play2DAutoSound("SFX\\Interface\\HelpClick.wav");
		}
	}
	
       
    return true;
}
void CECNewHelpGuide::StopStep()
{
	assert(m_iCurGuide != -1 && m_iCurStep != 0);
	if(m_iCurType != -1)
	{
        g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->CommonMessage(strHintUI[m_iCurType],"close",0,0);
    	m_iCurType = -1;
	}
}
bool CECNewHelpGuide::StartGuide(int iGuide)
{
	if(!IsEnable())
	  return false;
	//检查指引状态
    if(!CheckGuideState(iGuide))
	    return false;
	//当前有指引
	if(m_iCurGuide != -1)
		return false;
	SGuideContent* pGuideContent = FindGuideContentById(iGuide);
	assert(pGuideContent && pGuideContent->ids.size() > 0);
	//检查等级要求
	int level = g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel;
	if(level < pGuideContent->level_min || level > pGuideContent->level_max)
		return false;

	m_iCurGuide = iGuide;
	m_iCurStep  = 1;
	if(!StartStep(pGuideContent->ids[0]))
	{
		m_iCurGuide = -1;
		m_iCurStep  = 0;
		return false;
	}

	//设置指引状态
	if(!pGuideContent->isrepeate)
		SetGuideState(iGuide);

	return true;

}
bool CECNewHelpGuide::NextGuide(int iGuide,int iStep)
{
	assert(iStep >= 2);
	if(m_iCurGuide == -1 || iGuide != m_iCurGuide || iStep != m_iCurStep+1)
		return false;
	StopStep();
	SGuideContent* pGuideContent = FindGuideContentById(iGuide);
	assert(pGuideContent && (int)pGuideContent->ids.size() >= iStep);
	if(!IsEnable())
	{
		 m_iCurGuide = -1;
		 m_iCurStep  = 0;
		 return false;
	}
	m_iCurStep = iStep;
	if(!StartStep(pGuideContent->ids[iStep-1]))
	{
		m_iCurGuide = -1;
		m_iCurStep  = 0;
		return false;
	}
	return true;

}
bool CECNewHelpGuide::PauseGuide(int iGuide,int iStep)
{
    if(m_iCurGuide == -1 || m_iCurGuide != iGuide || m_iCurStep != iStep)
		return false;
	StopStep();
	return true;
}
bool CECNewHelpGuide::FinishGuide(int iGuide)
{
	if(m_iCurGuide == -1 || iGuide != m_iCurGuide)
		return false;
	SGuideContent* pGuideContent = FindGuideContentById(iGuide);
	assert(pGuideContent);
	if(pGuideContent->ids.size() != m_iCurStep)
		return false;
	StopStep();
	m_iCurGuide = -1;
	m_iCurStep  = 0;
	//设置指引状态
	if(pGuideContent->isrepeate)
		SetGuideState(iGuide);
	g_pGame->GetA3DEngine()->GetAMSoundEngine()->GetAMSoundBufferMan()->Play2DAutoSound("SFX\\Interface\\HelpFinish.wav");
	return true;
		
}

void CECNewHelpGuide::StopGuide()
{
	if(m_iCurGuide == -1)
		return;
	StopStep();
	m_iCurGuide = -1;
	m_iCurStep  = 0;
}
void CECNewHelpGuide::Tick(unsigned long dwDeltaTime)
{
	//是否开启指引
	if(!IsEnable()) 
		return;
	if(m_iCurGuide != -1)
		return;

	static DWORD lastTime = GetTickCount();
	DWORD curTime  = GetTickCount();

	if(curTime - lastTime > 1000)
	{
         lastTime  =  curTime; 

		if(TickHelpGuideForGettingTask())
			return;
		else if(TickHelpGuideForGettingItem())
			return;
		else if(TickHelpGuideForHostLevelUp())
			return;
	
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

		//伤血百分之三十
		if(CheckGuideState(GUIDE_HELP_USEMEDICINE_1))
		{
			ROLEBASICPROP &rbp =	pHost->GetBasicProps();
			ROLEEXTPROP   &rep =    pHost->GetExtendProps();
			double rate = rbp.iCurHP/(double)rep.max_hp;
			if(rate <= 0.7)
			{
				CECInventory* pInventory = g_pGame->GetGameRun()->GetHostPlayer()->GetPack();
				if(-1 != pInventory->FindItem(3999))
				{
					//快捷栏检查
					PAUIDIALOG pDlgQuickBar = pGameUI->GetDialog("Win_QuickBar8H_1");
                    PAUIOBJECT pControl = pDlgQuickBar->GetDlgItem("Item_3");
					if(NULL != pControl->GetDataPtr("ptr_CECShortcut"))
					{
						CECShortcut* pShortCut = (CECShortcut*)(pControl->GetDataPtr("ptr_CECShortcut"));
						if(pShortCut->GetType() == CECShortcut::SCT_ITEM)
						{
							CECSCItem* pSCItem = (CECSCItem*)pShortCut;
							if(pSCItem && pSCItem->GetItemTID() == 3999)
							{
								if(NewHelpGuide::StartGuide_Step_1(GUIDE_HELP_USEMEDICINE_1))
									return;
							}
						}

					}
				}
			}
		}

        //天人合一
		if(CheckGuideState(GUIDE_HELP_USEGAMEAUTOMATIC_2))
		{
			PAUIDIALOG pDlg = pGameUI->GetDialog("Win_Talisman");
			if(pDlg && pDlg->IsShow())
			{
               if(g_pGame->GetGameRun()->GetWorld()->GetInstanceID() == 401)
               {                    
					//验证任务
					CECTaskInterface *pTaskInterafce = g_pGame->GetGameRun()->GetHostPlayer()->GetTaskInterface();
					ActiveTaskList* pLst =static_cast<ActiveTaskList*>(pTaskInterafce->GetActiveTaskList());
					int idTask = 29652;
					if(pLst->GetEntry(idTask))
					{
						ATaskTemplMan *pMan = g_pGame->GetTaskTemplateMan();
						ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);
						if(pTemp)
						{
							   Task_State_info tsi;
							   pTaskInterafce->GetTaskStateInfo(idTask, &tsi);
							   int idMonster = tsi.m_MonsterWanted[0].m_ulMonsterId;
							   if (idMonster != 0  && tsi.m_MonsterWanted[0].m_ulMonstersKilled != tsi.m_MonsterWanted[0].m_ulMonstersToKill)
							   {
								   const NPC_INFO *info = pMan->GetTaskNPCInfo(idMonster);
								   if( info)
								   {
									   A3DVECTOR3 vDest;
									   vDest.x = (float)(info->x);
									   vDest.z = (float)(info->z);

								      A3DVECTOR3 vPos = pHost->GetPos();
								      if((vPos.x>=vDest.x-50  && vPos.x<=vDest.x+50) &&
									     (vPos.z>=vDest.z-50  && vPos.z<=vDest.z+50))
									 {
										if(NewHelpGuide::StartGuide_Step_1(GUIDE_HELP_USEGAMEAUTOMATIC_2))
											return;
									 }
								   }

							   }

						}

					}

                }
			}
		}

		
	}
}

bool CECNewHelpGuide::TickHelpGuideForGettingTask()
{
   if(CheckGetTaskGuideState())
   {
	   return NewHelpGuide::TriggerGetTask();
   }
   return false;
}
bool CECNewHelpGuide::TickHelpGuideForGettingItem()
{
   if(CheckGetGetItemGuideState())
   {
	   return NewHelpGuide::TriggerGetItem();
   }
   return false;
}
bool CECNewHelpGuide::TickHelpGuideForHostLevelUp()
{
   if(CheckHostLevelUpGuideState())
   {
	   return NewHelpGuide::TriggerHostLevelUp();
   }
   return false;
}
bool CECNewHelpGuide::CheckGetTaskGuideState()
{
	if(CheckGuideState(GUIDE_HELP_PATHFIND_1) || 
		CheckGuideState(GUIDE_HELP_FIGHT_2) ||
	   CheckGuideState(GUIDE_HELP_REFINING_5) ||
	   CheckGuideState(GUIDE_HELP_PET_11))
	  return true;
    return false;
}
bool CECNewHelpGuide::CheckGetGetItemGuideState()
{
	if(CheckGuideState(GUIDE_HELP_EQUIPWEAPON_3) || 
	   CheckGuideState(GUIDE_HELP_USEGIFT_2)     ||
	   CheckGuideState(GUIDE_HELP_EQUIPTALISMAN_4))
		return true;
    return false;
}
bool CECNewHelpGuide::CheckHostLevelUpGuideState()
{
	//技能加点
	int guideId = GetGuideIdOfSkillAddPoint();
	if(guideId != -1)
	{
		if(CheckGuideState(guideId))
			return true;
	}

    return false;
}

#define  NEWHELPGUIDESTART(iGuide)\
  g_pGame->GetGameRun()->GetNewHelpGuide()->StartGuide(iGuide);
#define  NEWHELPGUIDENEXT(iGuide,iStep)\
	g_pGame->GetGameRun()->GetNewHelpGuide()->NextGuide(iGuide,iStep);
#define  NEWHELPGUIDEFINISH(iGuide)\
	g_pGame->GetGameRun()->GetNewHelpGuide()->FinishGuide(iGuide);
#define  NEWHELPGUIDEPAUSE(iGuide,iStep)\
	g_pGame->GetGameRun()->GetNewHelpGuide()->PauseGuide(iGuide,iStep);
#define  NEWHELPGUIDESTOP()\
	g_pGame->GetGameRun()->GetNewHelpGuide()->StopGuide();


namespace NewHelpGuide
{

	bool CheckTaskTraceId(int taskid,int stepid)
	{
	    return  taskid == GetTaskIdForStep(stepid);
	}
	bool CheckInventoryItemId(int itemid,int stepid)
	{
	    return itemid == GetItemIdForStep(stepid);
	}
	bool CheckNpcTalkId(int npcid,int stepid)
	{
		return npcid == GetNpcIdForStep(stepid);
	}
	void StartGuide_Welcome()
	{
	   //打开指引选择界面
		//if(StartGuide_Step_1(GUIDE_HELP_WELCOM_1))
		{
		   CECGameUIMan* pUIGame = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		   pUIGame->GetDialog("Win_Help_Welcome")->Show(true);
		   g_pGame->GetA3DEngine()->GetAMSoundEngine()->GetAMSoundBufferMan()->Play2DAutoSound("SFX\\Interface\\HelpWelcom.wav");
		}
	}
	void FinishGuide_Welcome()
	{
	    //FinishGuide(GUIDE_HELP_WELCOM_1);
	}
    
	void StartGuide_Move()
	{
        //打开移动指引界面
		if(StartGuide_Step_1(GUIDE_HELP_MOVE_1))
		{
			CECGameUIMan* pUIGame = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pUIGame->GetDialog("Win_Help_Wasd")->Show(true);

		}
	}
	void FinishGuide_Move()
	{
        FinishGuide(GUIDE_HELP_MOVE_1);
	}
	//技能加点
	bool StartGuide_SkillAddPoint_1()
	{
		int guideId = GetGuideIdOfSkillAddPoint();
		if(guideId !=-1)
		{
			 if(StartGuide_Step_1(guideId))
				 return true;
		}
		return false;
	}
	bool StartGuide_SkillAddPoint_2()
	{
		int guideId = GetGuideIdOfSkillAddPoint();
		if(guideId !=-1)
		{
			if(StartGuide_Step_2(guideId))
				return true;
		}
		return false;
	}
	bool StartGuide_SkillAddPoint_3()
	{
		int guideId = GetGuideIdOfSkillAddPoint();
		if(guideId !=-1)
		{
			if(StartGuide_Step_3(guideId))
				return true;
		}
		return false;
	}
	bool FinishGuide_SkillAddPoint()
	{
		int guideId = GetGuideIdOfSkillAddPoint();
		if(guideId !=-1)
		{
			if(FinishGuide(guideId))
				return true;
		}
		return false;
	}
	bool StartGuide_Step_1(int iGuide)
	{
	    return NEWHELPGUIDESTART(iGuide);
	}
	bool StartGuide_Step_2(int iGuide)
	{
	  return NEWHELPGUIDENEXT(iGuide,2)
	}
	bool StartGuide_Step_3(int iGuide)
	{
	  return NEWHELPGUIDENEXT(iGuide,3)
	}
	bool StartGuide_Step_4(int iGuide)
	{
	  return NEWHELPGUIDENEXT(iGuide,4)
	}
	bool StartGuide_Step_5(int iGuide)
	{
	  return NEWHELPGUIDENEXT(iGuide,5)
	}
	bool StartGuide_Step_6(int iGuide)
	{
	  return NEWHELPGUIDENEXT(iGuide,6)
	}
	bool StartGuide_Step_7(int iGuide)
	{
	  return NEWHELPGUIDENEXT(iGuide,7)
	}
	bool StartGuide_Step_8(int iGuide)
	{
	  return NEWHELPGUIDENEXT(iGuide,8)
	}
	bool StartGuide_Step_9(int iGuide)
	{
	  return NEWHELPGUIDENEXT(iGuide,9)
	}
	bool StartGuide_Step_10(int iGuide)
	{
	  return NEWHELPGUIDENEXT(iGuide,10)
	}
	bool StartGuide_Step_11(int iGuide)
	{
		return NEWHELPGUIDENEXT(iGuide,11)
	}
	bool PauseGuide_Step_1(int iGuide)
	{
	  return NEWHELPGUIDEPAUSE(iGuide,1);
	}
	bool PauseGuide_Step_2(int iGuide)
	{
       return NEWHELPGUIDEPAUSE(iGuide,2);
	}
	bool PauseGuide_Step_3(int iGuide)
	{
	   return NEWHELPGUIDEPAUSE(iGuide,3);
	}
	bool PauseGuide_Step_4(int iGuide)
	{
	   return NEWHELPGUIDEPAUSE(iGuide,4);
	}
	bool PauseGuide_Step_5(int iGuide)
	{
	   return NEWHELPGUIDEPAUSE(iGuide,5);
	}
	bool PauseGuide_Step_6(int iGuide)
	{
	   return NEWHELPGUIDEPAUSE(iGuide,6);
	}
	bool PauseGuide_Step_7(int iGuide)
	{
	   return NEWHELPGUIDEPAUSE(iGuide,7);
	}
	bool PauseGuide_Step_8(int iGuide)
	{
	   return NEWHELPGUIDEPAUSE(iGuide,8);
	}
	bool PauseGuide_Step_9(int iGuide)
	{
	   return NEWHELPGUIDEPAUSE(iGuide,9);
	}
	bool PauseGuide_Step_10(int iGuide)
	{
	   return NEWHELPGUIDEPAUSE(iGuide,10);
	}
	bool PauseGuide_Step_11(int iGuide)
	{
		return NEWHELPGUIDEPAUSE(iGuide,11);
	}
	bool FinishGuide(int iGuide)
	{
	    return NEWHELPGUIDEFINISH(iGuide);
	}
    void StopGuide()
	{
	    return NEWHELPGUIDESTOP();
	}

	 bool TriggerGetTask()
	 {
		 CECTaskInterface *pTaskInterafce = g_pGame->GetGameRun()->GetHostPlayer()->GetTaskInterface();
		 ActiveTaskList* pLst =static_cast<ActiveTaskList*>(pTaskInterafce->GetActiveTaskList());


		 //目前逻辑是一个ID只能触发一个指引
		 for (unsigned char i = 0; i < pLst->m_uTaskCount; i++)
		 {
			 ActiveTaskEntry& CurEntry = pLst->m_TaskEntries[i];

			 if(CurEntry.m_ID == GetTaskIdForStep(ESTEP_TASK_PATHFIND))
			 {
			      if(NewHelpGuide::StartGuide_Step_1(GUIDE_HELP_PATHFIND_1))
					  return true;
				  else
					  continue;
			 }
			 else if(CurEntry.m_ID == GetTaskIdForStep(ESTEP_TASK_FIGHTWAR))
			 {
				 if(NewHelpGuide::StartGuide_Step_1(GUIDE_HELP_FIGHT_2))
					 return true;
				 else
					 continue;
			 }
			 else if(CurEntry.m_ID == GetTaskIdForStep(ESTEP_TASK_REFINING))
			 {
				 if(NewHelpGuide::StartGuide_Step_1(GUIDE_HELP_REFINING_5))
					 return true;
				 else
					 continue;
			 }
			 //暂时注掉
			 //else if(CurEntry.m_ID == GetTaskIdForStep(ESTEP_TASK_PET))
			 //{
			//	 if(NewHelpGuide::StartGuide_Step_1(GUIDE_HELP_PET_11))
			//		 return true;
			//	 else
			//		 continue;
			// }

		 }
		 return false;

	 }

	 bool TriggerGetItem()
	 {
		 CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		 CECInventory* pInventory = g_pGame->GetGameRun()->GetHostPlayer()->GetPack();
		 if(pInventory)
		 {
			 abase::vector<int> vecIds;
			 for (int i =0;i<pInventory->GetSize();++i)
			 {
				 CECIvtrItem* pItem = pInventory->GetItem(i);
				 if(pItem)
					 vecIds.push_back(pItem->GetTemplateID());
			 }

			 //目前逻辑是一个ID只能触发一个指引
			 for (int i=0;i<(int)vecIds.size();++i)
			 {
				 //装备武器
				 if(vecIds[i] == GetItemIdForStep(ESTEP_ITEM_EQUIPWEAPON))
				 {
					 if(StartGuide_Step_1(GUIDE_HELP_EQUIPWEAPON_3))
						 return true;
					 else
						 continue;
				 }
				 //使用礼包
				 else if(vecIds[i] == GetItemIdForStep(ESTEP_ITEM_USEGIFT_2_2))
				 {
					 if(StartGuide_Step_1(GUIDE_HELP_USEGIFT_2))
						 return true;
					 else
						 continue;
				 }
				 //装备法宝
				 else if(vecIds[i] == GetItemIdForStep(ESTEP_ITEM_EQUIPTALISMAN_4_2))
				 {
					 if(StartGuide_Step_1(GUIDE_HELP_EQUIPTALISMAN_4))
						 return true;
					 else
						 continue;
				 }
			 }
		 }


		 return false;
	 }

	 //升级
	 bool TriggerHostLevelUp()
	 {
		 int  iLevel = g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel;
		 CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
      		
		 if(iLevel == 4)
		 {
             //技能加点
			  if(StartGuide_SkillAddPoint_1())
				return true;

		 }

		 return false;
	 }

	 //双击或右键点击包裹界面物品图标
	 bool TriggerClickInventoryItem(int itemid)
	 {
		 if(CheckInventoryItemId(itemid,ESTEP_ITEM_EQUIPWEAPON))
			 return StartGuide_Step_3(GUIDE_HELP_EQUIPWEAPON_3);
		 else if(CheckInventoryItemId(itemid,ESTEP_ITEM_USEGIFT_2_2))
		     return FinishGuide(GUIDE_HELP_USEGIFT_2);
		 else if(CheckInventoryItemId(itemid,ESTEP_ITEM_REFINING_5_3))
			 return StartGuide_Step_4(GUIDE_HELP_REFINING_5);
		 else if(CheckInventoryItemId(itemid,ESTEP_ITEM_REFINING_5_4))
		     return StartGuide_Step_5(GUIDE_HELP_REFINING_5);
		 else if(CheckInventoryItemId(itemid,ESTEP_ITEM_PET_11_3))
		     return StartGuide_Step_4(GUIDE_HELP_PET_11);
		 else if(CheckInventoryItemId(itemid,ESTEP_ITEM_PET_11_10))
		     return StartGuide_Step_11(GUIDE_HELP_PET_11);
		 
		 return false;
	 }

	 //单击任务追踪面板上的NPC名字
	 bool TriggerClickTaskTraceNpcName(int taskid)
	 {
		 if(CheckTaskTraceId(taskid,ESTEP_TASK_PATHFIND))
			  return FinishGuide(GUIDE_HELP_PATHFIND_1);
		 else if(CheckTaskTraceId(taskid,ESTEP_TASK_REFINING))
			  return PauseGuide_Step_1(GUIDE_HELP_REFINING_5);
		 else if(CheckTaskTraceId(taskid,ESTEP_TASK_PET))
			 return PauseGuide_Step_1(GUIDE_HELP_PET_11);

		 return false;
	 }
	 //NPC对话界面打开
	 bool TriggerOpeningNpcTalk(int npcid)
	 {
		 if(CheckNpcTalkId(npcid,ESTEP_NPC_NPCTALK))
			return StartGuide_Step_1(GUIDE_HELP_NPCCHAT_2);
		 else if(CheckNpcTalkId(npcid,ESTEP_NPC_NPCREFINING))
		    return StartGuide_Step_2(GUIDE_HELP_REFINING_5);

		 return false;
	 }
	 //点击NPC列表项
	 bool TriggerClickNpcListItem(int index)
	 {
	    //此处暂时写死
	    if(index == 0)
		{
			if(StartGuide_Step_2(GUIDE_HELP_NPCCHAT_2))
				return true;
			if(StartGuide_Step_3(GUIDE_HELP_REFINING_5))
				return true;
		}

		return false;
	 }

}