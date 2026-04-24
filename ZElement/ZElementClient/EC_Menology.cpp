/********************************************************************
	created:	2006/08/16
	author:		kuiwu
	
	purpose:	get config info from lua script
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/
#pragma warning ( disable : 4284 )


#include <assert.h>
#include "LuaState.h"
#include "LuaUtil.h"
#include "LuaScript.h"
#include "LuaFuncCommon.h"
#include "EC_Algorithm.h"
#include "achar.h"
#include "TaskTemplMan.h"
#include "EC_TaskInterface.h"
#include "EC_GameRun.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_HostPlayer.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUICommon.h"
#include <locale.h>

#include "EC_Menology.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CECMenology CECMenology::m_Menology;
static const char* LUA_PROMP_FILE = "Interfaces\\script\\config\\menology_info.lua";

static const char* menology_def_name[] = 
{
	"MenologyRecommend",
	"MenologyWeekly",
	"MenologyBattleField",
	"MenologyMonster",
	"",
	"MenologyDailyMissionOne",
	"MenologyDailyMissionTwo",
	"MenologyDailyActivity",
	"MenologyDailyDuplicate",
	"MenologyDailyBattle",
	"MenologySystemFunction",
	"MenologyUseItems",
	"MenologyGameActionOne",
	"MenologyGameActionTwo",
};
static const char* festival_def_name = "SpecialTask";

CECMenology::CECMenology()
{
	m_pState = NULL;
}

CECMenology::~CECMenology()
{
}

bool CECMenology::Init()
{
	m_pState = g_LuaStateMan.GetConfigState();
	assert(m_pState);
	
	m_pState->Lock();

	
	CLuaScript * pScript = m_pState->RegisterFile(LUA_PROMP_FILE);
	
	if (!pScript)
	{
		m_pState->Unlock();
		AUI_ReportError(__LINE__, 1, "Failed to open Interfaces\\script\\config\\menology_info.lua");
		return false;
	}

	bool rt = _LoadRecommandInfo();
	assert(rt);
	rt = _LoadWeeklyInfo();
	assert(rt);
	rt = _LoadBattleFieldInfo();
	assert(rt);
	rt = _LoadMonsterInfo();
	assert(rt);
	rt = _LoadFestivalInfo();
	assert(rt);
	rt = _LoadDynamicElement();
	assert(rt);

	m_pState->Unlock();

	return	rt;

}

void CECMenology::Release()
{
	UINT i;
	for (i=0; i<TYPE_NUM; i++)
	{
		m_vecInfo[i].clear();
	}
 	abase::vector<AString> rmTbls;

	for (i=0; i<sizeof(menology_def_name)/sizeof(char*); i++)
	{
		rmTbls.push_back(menology_def_name[i]);
	}
 
 	m_pState->Lock();
 	m_pState->UnRegister(LUA_PROMP_FILE, &rmTbls);
 	m_pState->Unlock();
}

int GetSuitTask(const unsigned int* tasks, const int num)
{
	bool suit[MENOLOGY_TASKID_MAX];
	int i, j;
	UINT suitLevelMaxLimit = 0;
	BOOL bFindSuit = false;
	UINT minDiffIndex = 0;
	int	 minDiff = INT_MAX;
	ATaskTempl* lstTask;
	ATaskTemplMan		*pMan = g_pGame->GetTaskTemplateMan();
	CECTaskInterface	*pTask = g_pGame->GetGameRun()->GetHostPlayer()->GetTaskInterface();
	UINT		playerLevel = pTask->GetPlayerLevel();
	UINT		ulOccup = pTask->GetPlayerOccupation();
	
	//筛选合适职业
	for ( i = 0; i < num; i++ )
	{
		suit[i] = true;
		lstTask = pMan->GetTaskTemplByID(tasks[i]);
		if (!lstTask)
		{
			continue;
		}
		if( 0==lstTask->m_ulOccupations )
		{
			continue;
		}
		for ( j = 0; j < (int)lstTask->m_ulOccupations; j++ )
			if ( lstTask->m_Occupations[j] == ulOccup )
			{
				break;
			}
			if ( j == (int)lstTask->m_ulOccupations )
			{
				suit[i] = false;
			}
	}
	
	//筛选合适级别
	for ( i = 0; i < num; i++)
	{
		lstTask = pMan->GetTaskTemplByID(tasks[i]);
		if (!lstTask)
		{
			continue;
		}
		unsigned long levMin = lstTask->m_ulPremise_Lev_Min;
		unsigned long levMax = lstTask->m_ulPremise_Lev_Max;
		levMax = levMax>0 ? levMax : 10000;
		
		if (suit[i] && playerLevel>=levMin && playerLevel<=levMax && suitLevelMaxLimit<levMax)
		{
			return tasks[i];
		}
		else
		{
			int toMin = abs((int)playerLevel-(int)levMin);
			int toMax = abs((int)playerLevel-(int)levMax);
			int temp = a_Min(toMin, toMax);
			if (temp < minDiff)
			{
				minDiff = temp;
				minDiffIndex = i;
			}
		}
	}
	
	return tasks[minDiffIndex];
}

class CompareInfoMenology
{
public:
	CECMenology::SortType m_St;
	bool	m_bInc;
	CompareInfoMenology(CECMenology::SortType st, bool bInc=true) : m_St(st),m_bInc(bInc){}
	bool operator ()(MenologyInfo & p1, MenologyInfo & p2)
	{
		bool bRet = true;
		AString strLocalOrg = setlocale(LC_ALL, NULL);
		setlocale(LC_ALL, "");
		switch (m_St)
		{
		case CECMenology::ST_MENOLOGY:
			bRet=p1.iMenologyLevel < p2.iMenologyLevel;
			break;
		case CECMenology::ST_NAME:
			bRet= wcscoll(p1.strName, p2.strName)>=0;
			break;
		case CECMenology::ST_TIME:
			bRet=(p1.tmStartTime.tm_hour<p2.tmStartTime.tm_hour||(p1.tmStartTime.tm_hour==p2.tmStartTime.tm_hour&&p1.tmStartTime.tm_min<p2.tmStartTime.tm_min));
			break;
		case CECMenology::ST_LV_RCOMMAND:
			bRet=p1.ulMinLevel < p2.ulMinLevel;
			break;
		case CECMenology::ST_LV_BOSS:
			bRet=p1.iBossLv < p2.iBossLv;
			break;
		case CECMenology::ST_COUNT:
			bRet=p1.iPermitCount < p2.iPermitCount;
			break;
		case CECMenology::ST_CONDICTION:
			bRet= wcscoll(p1.strCondition, p2.strCondition)>=0;
			break;
		case CECMenology::ST_PERIOD:
			bRet= wcscoll(p1.strDisplayDays, p2.strDisplayDays)>=0;
			break;
		case CECMenology::ST_REWARD:
			bRet=(p1.iRewardType/100+(p1.iRewardType%100/10)+p1.iRewardType%10)<(p2.iRewardType/100+(p2.iRewardType%100/10)+p2.iRewardType%10);
			break;
		case CECMenology::ST_BOSS_RECLV:
			bRet=p1.strBossRecommendLv.ToInt() < p2.strBossRecommendLv.ToInt();
			break;
		case CECMenology::ST_BOSS_LINE:
			bRet=p1.strLineInfo.ToInt() < p2.strLineInfo.ToInt();
			break;
		case CECMenology::ST_BOSS_PERIOD:
			bRet= wcscoll(p1.strBossPeriod, p2.strBossPeriod)>=0;
			break;
		case CECMenology::ST_STATUS:
			
			int iStat1=0;
			int iStat2=0;
			tm tmNow = g_pGame->GetServerLocalTime();
			int nowMinute = tmNow.tm_hour*60 + tmNow.tm_min;
			
			CECTaskInterface* pTask = g_pGame->GetGameRun()->GetHostPlayer()->GetTaskInterface();
			ATaskTemplMan *pMan = g_pGame->GetTaskTemplateMan();
			int idTask = GetSuitTask(p1.uiTaskID, p1.uiNumID);
			if (p1.iPermitCount==1)
			{
				ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);
				if(pTemp&&pTemp->CheckDeliverTime(pTask, pTask->GetCurTime()))
					iStat1 = 2;
			}
			if ( nowMinute<(p1.tmStartTime.tm_hour*60+p1.tmStartTime.tm_min) )
				iStat1 = 1;
			else if ( nowMinute>(p1.tmEndTime.tm_hour*60+p1.tmEndTime.tm_min) )
				iStat1 = 3;
			else if(iStat1!=2)
				iStat1 = 0;
			idTask = GetSuitTask(p2.uiTaskID, p2.uiNumID);
			if (p2.iPermitCount==1)
			{
				ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);
				if(pTemp&&pTemp->CheckDeliverTime(pTask, pTask->GetCurTime()))
					iStat2 = 2;
			}
			if ( nowMinute<(p2.tmStartTime.tm_hour*60+p2.tmStartTime.tm_min) )
				iStat2 = 1;
			else if ( nowMinute>(p2.tmEndTime.tm_hour*60+p2.tmEndTime.tm_min) )
				iStat2 = 3;
			else if(iStat2!=2)
				iStat2 = 0;
			bRet=iStat1<iStat2;
			break;
		}
		setlocale(LC_ALL, strLocalOrg);
		if(m_bInc)
			return bRet;
		else
			return !bRet;
	}
};

bool CECMenology::_LoadRecommandInfo()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	
	if (!m_pState->Call(menology_def_name[TYPE_RECOMMAND], "GetSelf", args, results))
	{
		return false;
	}
	
	ACString strText;
	CScriptValue& ret0 = results[0];
	
	unsigned int i;
	for (i = 0; i < ret0.m_ArrVal.size(); i++)
	{
		MenologyInfo info;
		info.iType = TYPE_RECOMMAND;
		DWORD ii;
		for( ii = 0; ii<7; ii++)
			info.bDays[ii] = false;
		//循环两次，考虑效率的情况下不可用下面函数
		luaf::GetValue(info.iId,			ret0.m_ArrVal[i],   _AL("id"));
		luaf::GetValue(info.strIcon1,		ret0.m_ArrVal[i],   _AL("iconpath1"));
		luaf::GetValue(info.strIcon2,		ret0.m_ArrVal[i],   _AL("iconpath2"));
		luaf::GetValue(info.strIcon3,		ret0.m_ArrVal[i],   _AL("iconpath3"));
		luaf::GetValue(info.iRewardType,	ret0.m_ArrVal[i],   _AL("rewardtype"));
		luaf::GetValue(info.iMask,			ret0.m_ArrVal[i],	_AL("maskinfo"));
		luaf::GetValue(info.strName,		ret0.m_ArrVal[i],	_AL("name"));
		luaf::GetValue(info.bMenology,		ret0.m_ArrVal[i],	_AL("ismendisplay"));
		luaf::GetValue(info.iMenologyLevel,	ret0.m_ArrVal[i],	_AL("mendisplaylevel"));
		luaf::GetValue(info.strTime,		ret0.m_ArrVal[i],	_AL("validtime"));
		swscanf(info.strTime,_AL("%d:%d-%d:%d"),&info.tmStartTime.tm_hour, 
			&info.tmStartTime.tm_min, &info.tmEndTime.tm_hour, &info.tmEndTime.tm_min);
		info.bNoTimeLimit = info.tmStartTime.tm_hour==0 && info.tmStartTime.tm_min==0 && info.tmEndTime.tm_hour==24&&info.tmEndTime.tm_min==0;

		luaf::GetValue(info.strLevelInfo,	ret0.m_ArrVal[i],	_AL("levelreq"));
		int len = info.strLevelInfo.GetLength();
		int loc = info.strLevelInfo.Find('-');
		info.ulMinLevel = 1;
		info.ulMaxLevel = 10000;
		if(-1==loc)
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			info.ulMaxLevel = a_atoi(info.strLevelInfo);
			//info.strLevelInfo.Format(GetStringFromTable(1062),info.ulMaxLevel);
		}
		else if(loc+1 == len)
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			//info.strLevelInfo.Format(GetStringFromTable(1063),info.ulMinLevel);
		}
		else
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			info.ulMaxLevel = a_atoi(info.strLevelInfo.Mid(loc+1));
			//info.strLevelInfo.Format(GetStringFromTable(1064),info.ulMinLevel,info.ulMaxLevel);
		}
		
		luaf::GetValue(info.bReborn,		ret0.m_ArrVal[i],	_AL("needreborn"));
		luaf::GetValue(info.iPermitCount,	ret0.m_ArrVal[i],	_AL("permitcount"));
		luaf::GetValue(info.strCondition,	ret0.m_ArrVal[i],	_AL("cond"));
		luaf::GetValue(info.strWeekDays,	ret0.m_ArrVal[i],	_AL("validday"));
		luaf::GetValue(info.strDisplayDays,	ret0.m_ArrVal[i],	_AL("displayday"));
		CSplit sp = info.strWeekDays;
		CSPLIT_STRING_VECTOR vec = sp.Split(_AL(","));
		for( ii = 0; ii < vec.size(); ii++ )
		{
			info.bDays[a_atoi(vec[ii])-1] = true;
		}

		luaf::GetValue(info.strLineInfo,	ret0.m_ArrVal[i],	_AL("line"));
		info.vecLine.clear();
		DWORD downLine = 1;
		DWORD upLine = 15;
		if (info.strLineInfo != _AL("所有线路"))
		{
			downLine = info.strLineInfo.ToInt();
			ACString strTemp = info.strLineInfo;
			if (int index = strTemp.Find(_AL("-"))>0)
			{
				strTemp.CutLeft(index+1);
				upLine = strTemp.ToInt();
			}
		}
		for (ii=downLine; ii<=upLine; ii++)
		{
			info.vecLine.push_back(ii);
		}

		luaf::GetValue(info.strNPC,			ret0.m_ArrVal[i],	_AL("npc"));
		luaf::GetValue(info.bTemplateAward,	ret0.m_ArrVal[i],	_AL("istaskaward"));
		//if (info.bTemplateAward)
		{
			abase::vector<const CScriptValue*> vecAward ;
			if (luaf::GetValue(vecAward, ret0.m_ArrVal[i], _AL("award")))
			{
				int j;
				for (j=0; j<(int)vecAward.size(); ++j)
				{
					MenologyAward ma;
					luaf::GetValue(ma.iId,*(vecAward[j]),_AL("awardid"));
					if(!luaf::GetValue(strText,*(vecAward[j]),_AL("awardiconpath")))
					{
						break;
					}
					ma.strIcon = AC2AS(strText);
					if(!luaf::GetValue(ma.strDesc,*(vecAward[j]),_AL("awarddesc")))
					{
						break;
					}
					info.vecAward.push_back(ma);
				}
				if (j!=(int)vecAward.size())
				{
					continue;
				}
			}
		}

		luaf::GetValue(info.uiCommend,		ret0.m_ArrVal[i],	_AL("commendlevel"));
		luaf::GetValue(info.bTaskNpc,		ret0.m_ArrVal[i],	_AL("istasknpc"));
		luaf::GetValue(info.strNpcDesc,		ret0.m_ArrVal[i],	_AL("descnp"));
		luaf::GetValue(info.iNpcMapid,		ret0.m_ArrVal[i],	_AL("descmapid"));
		luaf::GetValue(info.strNpcPos,		ret0.m_ArrVal[i],	_AL("descpos"));
		luaf::GetValue(info.bTaskNpc,		ret0.m_ArrVal[i],	_AL("istasknpc"));
		luaf::GetValue(info.bIsTransport,	ret0.m_ArrVal[i],	_AL("istransport"));

		luaf::GetValue(info.strDesc,		ret0.m_ArrVal[i],	_AL("desc"));
		luaf::GetValue(strText,				ret0.m_ArrVal[i],	_AL("taskid"));
		sp = strText.GetBuffer(0);
		vec = sp.Split(_AL(","));
		info.uiNumID = vec.size();
		for( ii = 0; ii < info.uiNumID; ii++ )
		{
			info.uiTaskID[ii] = a_atoi(vec[ii]);
		}

		info.vecTimes.clear();
		abase::vector<const CScriptValue*> vecAward;
		CECCalendar tmStart, tmEnd;
		if (luaf::GetValue(vecAward, ret0.m_ArrVal[i], _AL("periods")))
		{
			int j;
			for (j=0; j<(int)vecAward.size(); ++j)
			{
				FestivalTime tmInfo;
				if(!luaf::GetValue(strText,*(vecAward[j]),_AL("starttime")))
				{
					break;
				}
				swscanf(strText,_AL("%d-%d-%d"), &tmStart.m_iYear, &tmStart.m_iMonth, &tmStart.m_iDay);
				tmStart.m_iYear -= 1900;
				tmStart.m_iMonth  -= 1;
				tmInfo.tmStart = tmStart;
				if(!luaf::GetValue(strText,*(vecAward[j]),_AL("endtime")))
				{
					break;
				}
				swscanf(strText,_AL("%d-%d-%d"), &tmEnd.m_iYear, &tmEnd.m_iMonth, &tmEnd.m_iDay);
				tmEnd.m_iYear -= 1900;
				tmEnd.m_iMonth  -= 1;
				tmInfo.tmEnd = tmEnd;
				info.vecTimes.push_back(tmInfo);
			}
			if (j!=(int)vecAward.size())
			{
				continue;
			}
		}

		m_vecInfo[TYPE_RECOMMAND].push_back(info);
		
	}
	
	BubbleSort(m_vecInfo[TYPE_RECOMMAND].begin(), m_vecInfo[TYPE_RECOMMAND].end(), 
			CompareInfoMenology(ST_MENOLOGY));
	
	return true;
}

bool CECMenology::_LoadWeeklyInfo()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	
	if (!m_pState->Call(menology_def_name[TYPE_WEEKLY], "GetSelf", args, results))
	{
		return false;
	}
	
	ACString strText;
	CScriptValue& ret0 = results[0];
	
	unsigned int i;
	for (i = 0; i < ret0.m_ArrVal.size(); i++)
	{
		MenologyInfo info;
		info.iType = TYPE_WEEKLY;
		DWORD ii;
		for( ii = 0; ii<7; ii++)
			info.bDays[ii] = false;
		//循环两次，考虑效率的情况下不可用下面函数
		luaf::GetValue(info.iId,			ret0.m_ArrVal[i],   _AL("id"));
		luaf::GetValue(info.strIcon1,		ret0.m_ArrVal[i],   _AL("iconpath1"));
		luaf::GetValue(info.strIcon2,		ret0.m_ArrVal[i],   _AL("iconpath2"));
		luaf::GetValue(info.strIcon3,		ret0.m_ArrVal[i],   _AL("iconpath3"));
		luaf::GetValue(info.iRewardType,	ret0.m_ArrVal[i],   _AL("rewardtype"));
		luaf::GetValue(info.iMask,			ret0.m_ArrVal[i],	_AL("maskinfo"));
		luaf::GetValue(info.strName,		ret0.m_ArrVal[i],	_AL("name"));
		luaf::GetValue(info.bMenology,		ret0.m_ArrVal[i],	_AL("ismendisplay"));
		luaf::GetValue(info.iMenologyLevel,	ret0.m_ArrVal[i],	_AL("mendisplaylevel"));
		luaf::GetValue(info.strTime,		ret0.m_ArrVal[i],	_AL("validtime"));
		swscanf(info.strTime,_AL("%d:%d-%d:%d"),&info.tmStartTime.tm_hour, 
			&info.tmStartTime.tm_min, &info.tmEndTime.tm_hour, &info.tmEndTime.tm_min);
		info.bNoTimeLimit = info.tmStartTime.tm_hour==0 && info.tmStartTime.tm_min==0 && info.tmEndTime.tm_hour==24&&info.tmEndTime.tm_min==0;

		luaf::GetValue(info.strLevelInfo,	ret0.m_ArrVal[i],	_AL("levelreq"));
		
		int len = info.strLevelInfo.GetLength();
		int loc = info.strLevelInfo.Find('-');
		info.ulMinLevel = 1;
		info.ulMaxLevel = 10000;
		if(-1==loc)
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			info.ulMaxLevel = a_atoi(info.strLevelInfo);
			//info.strLevelInfo.Format(GetStringFromTable(1062),info.ulMaxLevel);
		}
		else if(loc+1 == len)
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			//info.strLevelInfo.Format(GetStringFromTable(1063),info.ulMinLevel);
		}
		else
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			info.ulMaxLevel = a_atoi(info.strLevelInfo.Mid(loc+1));
			//info.strLevelInfo.Format(GetStringFromTable(1064),info.ulMinLevel,info.ulMaxLevel);
		}
		
		luaf::GetValue(info.bReborn,		ret0.m_ArrVal[i],	_AL("needreborn"));
		luaf::GetValue(info.iPermitCount,	ret0.m_ArrVal[i],	_AL("permitcount"));
		luaf::GetValue(info.strCondition,	ret0.m_ArrVal[i],	_AL("cond"));
		luaf::GetValue(info.strWeekDays,	ret0.m_ArrVal[i],	_AL("validday"));
		luaf::GetValue(info.strDisplayDays,	ret0.m_ArrVal[i],	_AL("displayday"));
		
		CSplit sp = info.strWeekDays;
		CSPLIT_STRING_VECTOR vec = sp.Split(_AL(","));
		for( ii = 0; ii < vec.size(); ii++ )
		{
			info.bDays[a_atoi(vec[ii])-1] = true;
		}
		luaf::GetValue(info.strLineInfo,	ret0.m_ArrVal[i],	_AL("line"));
		info.vecLine.clear();
		DWORD downLine = 1;
		DWORD upLine = 15;
		if (info.strLineInfo != _AL("所有线路"))
		{
			downLine = info.strLineInfo.ToInt();
			ACString strTemp = info.strLineInfo;
			if (int index = strTemp.Find(_AL("-"))>0)
			{
				strTemp.CutLeft(index+1);
				upLine = strTemp.ToInt();
			}
		}
		for (ii=downLine; ii<=upLine; ii++)
		{
			info.vecLine.push_back(ii);
		}
		luaf::GetValue(info.strNPC,			ret0.m_ArrVal[i],	_AL("npc"));
		luaf::GetValue(info.bTemplateAward,	ret0.m_ArrVal[i],	_AL("istaskaward"));
		//if (info.bTemplateAward)
		{
			abase::vector<const CScriptValue*> vecAward ;
			if (luaf::GetValue(vecAward, ret0.m_ArrVal[i], _AL("award")))
			{
				int j;
				for (j=0; j<(int)vecAward.size(); ++j)
				{
					MenologyAward ma;
					luaf::GetValue(ma.iId,*(vecAward[j]),_AL("awardid"));
					if(!luaf::GetValue(strText,*(vecAward[j]),_AL("awardiconpath")))
					{
						break;
					}
					ma.strIcon = AC2AS(strText);
					if(!luaf::GetValue(ma.strDesc,*(vecAward[j]),_AL("awarddesc")))
					{
						break;
					}
					info.vecAward.push_back(ma);
				}
				if (j!=(int)vecAward.size())
				{
					continue;
				}
			}
		}		

		luaf::GetValue(info.uiCommend,		ret0.m_ArrVal[i],	_AL("commendlevel"));
		luaf::GetValue(info.bTaskNpc,		ret0.m_ArrVal[i],	_AL("istasknpc"));
		luaf::GetValue(info.strNpcDesc,		ret0.m_ArrVal[i],	_AL("descnp"));
		luaf::GetValue(info.iNpcMapid,		ret0.m_ArrVal[i],	_AL("descmapid"));
		luaf::GetValue(info.strNpcPos,		ret0.m_ArrVal[i],	_AL("descpos"));
		luaf::GetValue(info.bTaskNpc,		ret0.m_ArrVal[i],	_AL("istasknpc"));
		luaf::GetValue(info.bIsTransport,	ret0.m_ArrVal[i],	_AL("istransport"));

		luaf::GetValue(info.strDesc,	ret0.m_ArrVal[i],	_AL("desc"));
		luaf::GetValue(strText,				ret0.m_ArrVal[i],	_AL("taskid"));
		sp = strText.GetBuffer(0);
		vec = sp.Split(_AL(","));
		info.uiNumID = vec.size();
		for( ii = 0; ii < info.uiNumID; ii++ )
		{
			info.uiTaskID[ii] = a_atoi(vec[ii]);
		}

		m_vecInfo[TYPE_WEEKLY].push_back(info);
		
	}
	BubbleSort(m_vecInfo[TYPE_WEEKLY].begin(), m_vecInfo[TYPE_WEEKLY].end(), 
			CompareInfoMenology(ST_MENOLOGY));
	
	return true;
}

bool CECMenology::_LoadBattleFieldInfo()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	
	if (!m_pState->Call(menology_def_name[TYPE_BATTLEFIELD], "GetSelf", args, results))
	{
		return false;
	}
	
	ACString strText;
	CScriptValue& ret0 = results[0];
	
	unsigned int i;
	for (i = 0; i < ret0.m_ArrVal.size(); i++)
	{
		MenologyInfo info;
		info.iType = TYPE_BATTLEFIELD;
		DWORD ii;
		for( ii = 0; ii<7; ii++)
			info.bDays[ii] = false;
		//循环两次，考虑效率的情况下不可用下面函数
		luaf::GetValue(info.iId,			ret0.m_ArrVal[i],   _AL("id"));
		luaf::GetValue(info.strIcon1,		ret0.m_ArrVal[i],   _AL("iconpath1"));
		luaf::GetValue(info.strIcon2,		ret0.m_ArrVal[i],   _AL("iconpath2"));
		luaf::GetValue(info.strIcon3,		ret0.m_ArrVal[i],   _AL("iconpath3"));
		luaf::GetValue(info.iRewardType,	ret0.m_ArrVal[i],   _AL("rewardtype"));
		luaf::GetValue(info.iMask,			ret0.m_ArrVal[i],	_AL("maskinfo"));
		luaf::GetValue(info.strName,		ret0.m_ArrVal[i],	_AL("name"));
		luaf::GetValue(info.bMenology,		ret0.m_ArrVal[i],	_AL("ismendisplay"));
		luaf::GetValue(info.iMenologyLevel,	ret0.m_ArrVal[i],	_AL("mendisplaylevel"));
		luaf::GetValue(info.strTime,		ret0.m_ArrVal[i],	_AL("validtime"));
		swscanf(info.strTime,_AL("%d:%d-%d:%d"),&info.tmStartTime.tm_hour, 
			&info.tmStartTime.tm_min, &info.tmEndTime.tm_hour, &info.tmEndTime.tm_min);
		info.bNoTimeLimit = info.tmStartTime.tm_hour==0 && info.tmStartTime.tm_min==0 && info.tmEndTime.tm_hour==24&&info.tmEndTime.tm_min==0;

		luaf::GetValue(info.strLevelInfo,	ret0.m_ArrVal[i],	_AL("levelreq"));
		
		int len = info.strLevelInfo.GetLength();
		int loc = info.strLevelInfo.Find('-');
		info.ulMinLevel = 1;
		info.ulMaxLevel = 10000;
		if(-1==loc)
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			info.ulMaxLevel = a_atoi(info.strLevelInfo);
			//info.strLevelInfo.Format(GetStringFromTable(1062),info.ulMaxLevel);
		}
		else if(loc+1 == len)
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			//info.strLevelInfo.Format(GetStringFromTable(1063),info.ulMinLevel);
		}
		else
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			info.ulMaxLevel = a_atoi(info.strLevelInfo.Mid(loc+1));
			//info.strLevelInfo.Format(GetStringFromTable(1064),info.ulMinLevel,info.ulMaxLevel);
		}

		
		luaf::GetValue(info.bReborn,		ret0.m_ArrVal[i],	_AL("needreborn"));
		luaf::GetValue(info.iPermitCount,	ret0.m_ArrVal[i],	_AL("permitcount"));
		luaf::GetValue(info.strCondition,	ret0.m_ArrVal[i],	_AL("cond"));
		luaf::GetValue(info.strWeekDays,	ret0.m_ArrVal[i],	_AL("validday"));
		luaf::GetValue(info.strDisplayDays,	ret0.m_ArrVal[i],	_AL("displayday"));
		
		CSplit sp = info.strWeekDays;
		CSPLIT_STRING_VECTOR vec = sp.Split(_AL(","));
		for( ii = 0; ii < vec.size(); ii++ )
		{
			info.bDays[a_atoi(vec[ii])-1] = true;
		}
		luaf::GetValue(info.strLineInfo,	ret0.m_ArrVal[i],	_AL("line"));
		info.vecLine.clear();
		DWORD downLine = 1;
		DWORD upLine = 15;
		if (info.strLineInfo != _AL("所有线路"))
		{
			downLine = info.strLineInfo.ToInt();
			ACString strTemp = info.strLineInfo;
			if (int index = strTemp.Find(_AL("-"))>0)
			{
				strTemp.CutLeft(index+1);
				upLine = strTemp.ToInt();
			}
		}
		for (ii=downLine; ii<=upLine; ii++)
		{
			info.vecLine.push_back(ii);
		}
		luaf::GetValue(info.strNPC,			ret0.m_ArrVal[i],	_AL("npc"));
		luaf::GetValue(info.bTemplateAward,	ret0.m_ArrVal[i],	_AL("istaskaward"));
		//if (info.bTemplateAward)
		{
			abase::vector<const CScriptValue*> vecAward ;
			if (luaf::GetValue(vecAward, ret0.m_ArrVal[i], _AL("award")))
			{
				int j;
				for (j=0; j<(int)vecAward.size(); ++j)
				{
					MenologyAward ma;
					luaf::GetValue(ma.iId,*(vecAward[j]),_AL("awardid"));
					if(!luaf::GetValue(strText,*(vecAward[j]),_AL("awardiconpath")))
					{
						break;
					}
					ma.strIcon = AC2AS(strText);
					if(!luaf::GetValue(ma.strDesc,*(vecAward[j]),_AL("awarddesc")))
					{
						break;
					}
					info.vecAward.push_back(ma);
				}
				if (j!=(int)vecAward.size())
				{
					continue;
				}
			}
		}		

		luaf::GetValue(info.uiCommend,		ret0.m_ArrVal[i],	_AL("commendlevel"));
		luaf::GetValue(info.bTaskNpc,		ret0.m_ArrVal[i],	_AL("istasknpc"));
		luaf::GetValue(info.strNpcDesc,		ret0.m_ArrVal[i],	_AL("descnp"));
		luaf::GetValue(info.iNpcMapid,		ret0.m_ArrVal[i],	_AL("descmapid"));
		luaf::GetValue(info.strNpcPos,		ret0.m_ArrVal[i],	_AL("descpos"));
		luaf::GetValue(info.bTaskNpc,		ret0.m_ArrVal[i],	_AL("istasknpc"));
		luaf::GetValue(info.bIsTransport,	ret0.m_ArrVal[i],	_AL("istransport"));

		luaf::GetValue(info.strDesc,		ret0.m_ArrVal[i],	_AL("desc"));
		luaf::GetValue(strText,				ret0.m_ArrVal[i],	_AL("taskid"));
		sp = strText.GetBuffer(0);
		vec = sp.Split(_AL(","));
		info.uiNumID = vec.size();
		for( ii = 0; ii < info.uiNumID; ii++ )
		{
			info.uiTaskID[ii] = a_atoi(vec[ii]);
		}

		m_vecInfo[TYPE_BATTLEFIELD].push_back(info);
		
	}
	BubbleSort(m_vecInfo[TYPE_BATTLEFIELD].begin(), m_vecInfo[TYPE_BATTLEFIELD].end(), 
			CompareInfoMenology(ST_MENOLOGY));
	
	return true;
}

bool CECMenology::_LoadMonsterInfo()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	
	if (!m_pState->Call(menology_def_name[TYPE_MONSTER], "GetSelf", args, results))
	{
		return false;
	}
	
	ACString strText;
	CScriptValue& ret0 = results[0];
	
	unsigned int i;
	for (i = 0; i < ret0.m_ArrVal.size(); i++)
	{
		MenologyInfo info;
		info.iType = TYPE_MONSTER;
		DWORD ii;
		for( ii = 0; ii<7; ii++)
			info.bDays[ii] = false;
		//循环两次，考虑效率的情况下不可用下面函数
		luaf::GetValue(info.iId,			ret0.m_ArrVal[i],   _AL("id"));
		luaf::GetValue(info.strIcon1,		ret0.m_ArrVal[i],   _AL("iconpath1"));
		luaf::GetValue(info.strIcon2,		ret0.m_ArrVal[i],   _AL("iconpath2"));
		luaf::GetValue(info.strIcon3,		ret0.m_ArrVal[i],   _AL("iconpath3"));
		luaf::GetValue(info.iRewardType,	ret0.m_ArrVal[i],   _AL("rewardtype"));
		luaf::GetValue(info.iMask,			ret0.m_ArrVal[i],	_AL("maskinfo"));
		luaf::GetValue(info.strName,		ret0.m_ArrVal[i],	_AL("name"));
		luaf::GetValue(info.bMenology,		ret0.m_ArrVal[i],	_AL("ismendisplay"));
		luaf::GetValue(info.iMenologyLevel,	ret0.m_ArrVal[i],	_AL("mendisplaylevel"));
		luaf::GetValue(info.strTime,		ret0.m_ArrVal[i],	_AL("validtime"));
		swscanf(info.strTime,_AL("%d:%d-%d:%d"),&info.tmStartTime.tm_hour, 
			&info.tmStartTime.tm_min, &info.tmEndTime.tm_hour, &info.tmEndTime.tm_min);
		info.bNoTimeLimit = info.tmStartTime.tm_hour==0 && info.tmStartTime.tm_min==0 && info.tmEndTime.tm_hour==24&&info.tmEndTime.tm_min==0;

		luaf::GetValue(info.strLevelInfo,	ret0.m_ArrVal[i],	_AL("levelreq"));
		
		int len = info.strLevelInfo.GetLength();
		int loc = info.strLevelInfo.Find('-');
		info.ulMinLevel = 1;
		info.ulMaxLevel = 10000;
		if(-1==loc)
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			info.ulMaxLevel = a_atoi(info.strLevelInfo);
			//info.strLevelInfo.Format(GetStringFromTable(1062),info.ulMaxLevel);
		}
		else if(loc+1 == len)
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			//info.strLevelInfo.Format(GetStringFromTable(1063),info.ulMinLevel);
		}
		else
		{
			info.ulMinLevel = a_atoi(info.strLevelInfo);
			info.ulMaxLevel = a_atoi(info.strLevelInfo.Mid(loc+1));
			//info.strLevelInfo.Format(GetStringFromTable(1064),info.ulMinLevel,info.ulMaxLevel);
		}

		
		luaf::GetValue(info.bReborn,		ret0.m_ArrVal[i],	_AL("needreborn"));
		luaf::GetValue(info.iPermitCount,	ret0.m_ArrVal[i],	_AL("permitcount"));
		luaf::GetValue(info.strCondition,	ret0.m_ArrVal[i],	_AL("cond"));
		luaf::GetValue(info.strWeekDays,	ret0.m_ArrVal[i],	_AL("validday"));
		luaf::GetValue(info.strDisplayDays,	ret0.m_ArrVal[i],	_AL("displayday"));
		
		CSplit sp = info.strWeekDays;
		CSPLIT_STRING_VECTOR vec = sp.Split(_AL(","));
		for( ii = 0; ii < vec.size(); ii++ )
		{
			info.bDays[a_atoi(vec[ii])-1] = true;
		}
		luaf::GetValue(info.strLineInfo,	ret0.m_ArrVal[i],	_AL("line"));
		info.vecLine.clear();
		DWORD downLine = 1;
		DWORD upLine = 15;
		if (info.strLineInfo != _AL("所有线路"))
		{
			downLine = info.strLineInfo.ToInt();
			ACString strTemp = info.strLineInfo;
			if (int index = strTemp.Find(_AL("-"))>0)
			{
				strTemp.CutLeft(index+1);
				upLine = strTemp.ToInt();
			}
		}
		for (ii=downLine; ii<=upLine; ii++)
		{
			info.vecLine.push_back(ii);
		}
		luaf::GetValue(info.strNPC,			ret0.m_ArrVal[i],	_AL("npc"));
		luaf::GetValue(info.bTemplateAward,	ret0.m_ArrVal[i],	_AL("istaskaward"));
		//if (info.bTemplateAward)
		{
			abase::vector<const CScriptValue*> vecAward ;
			if (luaf::GetValue(vecAward, ret0.m_ArrVal[i], _AL("award")))
			{
				int j;
				for (j=0; j<(int)vecAward.size(); ++j)
				{
					MenologyAward ma;
					luaf::GetValue(ma.iId,*(vecAward[j]),_AL("awardid"));
					if(!luaf::GetValue(strText,*(vecAward[j]),_AL("awardiconpath")))
					{
						break;
					}
					ma.strIcon = AC2AS(strText);
					if(!luaf::GetValue(ma.strDesc,*(vecAward[j]),_AL("awarddesc")))
					{
						break;
					}
					info.vecAward.push_back(ma);
				}
				if (j!=(int)vecAward.size())
				{
					continue;
				}
			}
		}		

		luaf::GetValue(info.uiCommend,		ret0.m_ArrVal[i],	_AL("commendlevel"));
		luaf::GetValue(info.bTaskNpc,		ret0.m_ArrVal[i],	_AL("istasknpc"));
		luaf::GetValue(info.strNpcDesc,		ret0.m_ArrVal[i],	_AL("descnp"));
		luaf::GetValue(info.iNpcMapid,		ret0.m_ArrVal[i],	_AL("descmapid"));
		luaf::GetValue(info.strNpcPos,		ret0.m_ArrVal[i],	_AL("descpos"));
		luaf::GetValue(info.bTaskNpc,		ret0.m_ArrVal[i],	_AL("istasknpc"));
		luaf::GetValue(info.bIsTransport,	ret0.m_ArrVal[i],	_AL("istransport"));
		luaf::GetValue(info.iBossLv,		ret0.m_ArrVal[i],	_AL("npclv"));
		luaf::GetValue(info.strBossPeriod,		ret0.m_ArrVal[i],	_AL("refreshperiod"));
		luaf::GetValue(info.strBossRecommendLv,		ret0.m_ArrVal[i],	_AL("recommendlv"));
		luaf::GetValue(info.strBossRecommendPlayerNum,		ret0.m_ArrVal[i],	_AL("recommendplayernum"));

		luaf::GetValue(info.strDesc,		ret0.m_ArrVal[i],	_AL("desc"));
		luaf::GetValue(strText,				ret0.m_ArrVal[i],	_AL("taskid"));
		sp = strText.GetBuffer(0);
		vec = sp.Split(_AL(","));
		info.uiNumID = vec.size();
		for( ii = 0; ii < info.uiNumID; ii++ )
		{
			info.uiTaskID[ii] = a_atoi(vec[ii]);
		}

		m_vecInfo[TYPE_MONSTER].push_back(info);
		
	}	
	BubbleSort(m_vecInfo[TYPE_MONSTER].begin(), m_vecInfo[TYPE_MONSTER].end(), 
			CompareInfoMenology(ST_MENOLOGY));
	
	return true;
}

bool CECMenology::_LoadFestivalInfo()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	
	if (!m_pState->Call(festival_def_name, "GetSelf", args, results))
	{
		return false;
	}
	
	ACString strText;
	CScriptValue& ret0 = results[0];
	
	unsigned int i;
	for (i = 0; i < ret0.m_ArrVal.size(); i++)
	{
		MenologyInfo info;
		info.iType = TYPE_FESTIVAL;
		DWORD ii;
		for( ii = 0; ii<7; ii++)
			info.bDays[ii] = false;
		//循环两次，考虑效率的情况下不可用下面函数
		ret0.m_ArrKey[i].RetrieveAWString(info.strName);
		luaf::GetValue(info.iId,			ret0.m_ArrVal[i],   _AL("id"));
		luaf::GetValue(info.strIcon2,		ret0.m_ArrVal[i],   _AL("backgroud1"));
		luaf::GetValue(info.strIcon3,		ret0.m_ArrVal[i],   _AL("backgroud2"));
		luaf::GetValue(info.strDesc,		ret0.m_ArrVal[i],	_AL("desc"));
		luaf::GetValue(info.iMenologyLevel,	ret0.m_ArrVal[i],	_AL("mendisplaylevel"));
		luaf::GetValue(info.strTime,		ret0.m_ArrVal[i],	_AL("validtime"));
		swscanf(info.strTime,_AL("%d:%d-%d:%d"),&info.tmStartTime.tm_hour, 
			&info.tmStartTime.tm_min, &info.tmEndTime.tm_hour, &info.tmEndTime.tm_min);
		info.bNoTimeLimit = info.tmStartTime.tm_hour==0 && info.tmStartTime.tm_min==0 && info.tmEndTime.tm_hour==24&&info.tmEndTime.tm_min==0;

		luaf::GetValue(info.bWeekly,		ret0.m_ArrVal[i],	_AL("weekly"));
		luaf::GetValue(info.strWeekDays,	ret0.m_ArrVal[i],	_AL("validday"));
		luaf::GetValue(info.strDisplayDays,	ret0.m_ArrVal[i],	_AL("displayday"));
		CSplit sp = info.strWeekDays;
		CSPLIT_STRING_VECTOR vec = sp.Split(_AL(","));
		for( ii = 0; ii < vec.size(); ii++ )
		{
			info.bDays[a_atoi(vec[ii])-1] = true;
		}

		info.vecTimes.clear();
		abase::vector<const CScriptValue*> vecAward;
		CECCalendar tmStart, tmEnd;
		if (luaf::GetValue(vecAward, ret0.m_ArrVal[i], _AL("periods")))
		{
			int j;
			for (j=0; j<(int)vecAward.size(); ++j)
			{
				FestivalTime tmInfo;
				if(!luaf::GetValue(strText,*(vecAward[j]),_AL("starttime")))
				{
					break;
				}
				swscanf(strText,_AL("%d-%d-%d"), &tmStart.m_iYear, &tmStart.m_iMonth, &tmStart.m_iDay);
				tmStart.m_iYear -= 1900;
				tmStart.m_iMonth  -= 1;
				tmInfo.tmStart = tmStart;
				if(!luaf::GetValue(strText,*(vecAward[j]),_AL("endtime")))
				{
					break;
				}
				swscanf(strText,_AL("%d-%d-%d"), &tmEnd.m_iYear, &tmEnd.m_iMonth, &tmEnd.m_iDay);
				tmEnd.m_iYear -= 1900;
				tmEnd.m_iMonth  -= 1;
				tmInfo.tmEnd = tmEnd;
				info.vecTimes.push_back(tmInfo);
			}
			if (j!=(int)vecAward.size())
			{
				continue;
			}
		}

		m_vecInfo[TYPE_FESTIVAL].push_back(info);
		
	}	
	
	return true;
}


bool CECMenology::_LoadDynamicElement()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	for(int nIndex = TYPE_DAILYDUPLICATE; nIndex <= TYPE_DAILYDUPLICATE9; nIndex++)
	{
		if (!m_pState->Call(menology_def_name[nIndex], "GetSelf", args, results))
		{
			continue;
		}
		
		ACString strText;
		CScriptValue& ret0 = results[0];
		
		unsigned int i;
		for (i = 0; i < ret0.m_ArrVal.size(); i++)
		{
			MenologyInfo info;
			info.iType = TYPE_RECOMMAND;
			DWORD ii;
			for( ii = 0; ii<7; ii++)
				info.bDays[ii] = false;
			//循环两次，考虑效率的情况下不可用下面函数
			luaf::GetValue(info.iId,			ret0.m_ArrVal[i],   _AL("id"));
			luaf::GetValue(info.strIcon1,		ret0.m_ArrVal[i],   _AL("iconpath1"));
			luaf::GetValue(info.strIcon2,		ret0.m_ArrVal[i],   _AL("iconpath2"));
			luaf::GetValue(info.strName,		ret0.m_ArrVal[i],	_AL("name"));
			luaf::GetValue(info.strTime,		ret0.m_ArrVal[i],	_AL("validtime"));
			swscanf(info.strTime,_AL("%d:%d-%d:%d"),&info.tmStartTime.tm_hour, 
				&info.tmStartTime.tm_min, &info.tmEndTime.tm_hour, &info.tmEndTime.tm_min);
			info.bNoTimeLimit = info.tmStartTime.tm_hour==0 && info.tmStartTime.tm_min==0 && info.tmEndTime.tm_hour==24&&info.tmEndTime.tm_min==0;
			
			luaf::GetValue(info.strLevelInfo,	ret0.m_ArrVal[i],	_AL("levelreq"));
			int len = info.strLevelInfo.GetLength();
			int loc = info.strLevelInfo.Find('-');
			info.ulMinLevel = 1;
			info.ulMaxLevel = 10000;
			if(-1==loc)
			{
				info.ulMinLevel = a_atoi(info.strLevelInfo);
				info.ulMaxLevel = a_atoi(info.strLevelInfo);
				//info.strLevelInfo.Format(GetStringFromTable(1062),info.ulMaxLevel);
			}
			else if(loc+1 == len)
			{
				info.ulMinLevel = a_atoi(info.strLevelInfo);
				//info.strLevelInfo.Format(GetStringFromTable(1063),info.ulMinLevel);
			}
			else
			{
				info.ulMinLevel = a_atoi(info.strLevelInfo);
				info.ulMaxLevel = a_atoi(info.strLevelInfo.Mid(loc+1));
				//info.strLevelInfo.Format(GetStringFromTable(1064),info.ulMinLevel,info.ulMaxLevel);
			}
			
			luaf::GetValue(info.bReborn,		ret0.m_ArrVal[i],	_AL("needreborn"));
			luaf::GetValue(info.iPermitCount,	ret0.m_ArrVal[i],	_AL("permitcount"));
			luaf::GetValue(info.strCondition,	ret0.m_ArrVal[i],	_AL("cond"));
			luaf::GetValue(info.strWeekDays,	ret0.m_ArrVal[i],	_AL("validday"));
			CSplit sp = info.strWeekDays;
			CSPLIT_STRING_VECTOR vec = sp.Split(_AL(","));
			for( ii = 0; ii < vec.size(); ii++ )
			{
				info.bDays[a_atoi(vec[ii])-1] = true;
			}
			
			luaf::GetValue(info.strLineInfo,	ret0.m_ArrVal[i],	_AL("line"));
			info.vecLine.clear();
			DWORD downLine = 1;
			DWORD upLine = 15;
			if (info.strLineInfo != _AL("所有线路"))
			{
				downLine = info.strLineInfo.ToInt();
				ACString strTemp = info.strLineInfo;
				if (int index = strTemp.Find(_AL("-"))>0)
				{
					strTemp.CutLeft(index+1);
					upLine = strTemp.ToInt();
				}
			}
			for (ii=downLine; ii<=upLine; ii++)
			{
				info.vecLine.push_back(ii);
			}
			
			luaf::GetValue(info.strNPC,			ret0.m_ArrVal[i],	_AL("npc"));
			luaf::GetValue(info.bTemplateAward,	ret0.m_ArrVal[i],	_AL("istaskaward"));
			//if (info.bTemplateAward)
			{
				abase::vector<const CScriptValue*> vecAward ;
				if (luaf::GetValue(vecAward, ret0.m_ArrVal[i], _AL("award")))
				{
					int j;
					for (j=0; j<(int)vecAward.size(); ++j)
					{
						MenologyAward ma;
						luaf::GetValue(ma.iId,*(vecAward[j]),_AL("awardid"));
						if(!luaf::GetValue(strText,*(vecAward[j]),_AL("awardiconpath")))
						{
							break;
						}
						ma.strIcon = AC2AS(strText);
						if(!luaf::GetValue(ma.strDesc,*(vecAward[j]),_AL("awarddesc")))
						{
							break;
						}
						info.vecAward.push_back(ma);
					}
					if (j!=(int)vecAward.size())
					{
						continue;
					}
				}
			}
			luaf::GetValue(info.bTaskNpc,		ret0.m_ArrVal[i],	_AL("istasknpc"));
			luaf::GetValue(info.strNpcDesc,		ret0.m_ArrVal[i],	_AL("descnp"));
			luaf::GetValue(info.iNpcMapid,		ret0.m_ArrVal[i],	_AL("descmapid"));
			luaf::GetValue(info.strNpcPos,		ret0.m_ArrVal[i],	_AL("descpos"));
			luaf::GetValue(info.bIsTransport,	ret0.m_ArrVal[i],	_AL("istransport"));
			
			luaf::GetValue(info.strDesc,		ret0.m_ArrVal[i],	_AL("desc"));
			luaf::GetValue(strText,				ret0.m_ArrVal[i],	_AL("taskid"));
			sp = strText.GetBuffer(0);
			vec = sp.Split(_AL(","));
			info.uiNumID = vec.size();
			for( ii = 0; ii < info.uiNumID; ii++ )
			{
				info.uiTaskID[ii] = a_atoi(vec[ii]);
			}
			m_vecInfo[nIndex].push_back(info);	
		}
		BubbleSort(m_vecInfo[nIndex].begin(), m_vecInfo[nIndex].end(), 
			CompareInfoMenology(ST_LV_RCOMMAND));
	}
	return true;
}


void CECMenology::SortInfo(InfoType it, SortType st, bool bInc)
{
	BubbleSort(m_vecInfo[it].begin(), m_vecInfo[it].end(), 
			CompareInfoMenology(st, bInc));
}