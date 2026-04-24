/********************************************************************
	created:	3/6/2009
	author:		liudong
	
	purpose:   information about achievement.
	Copyright (C) All Rights Reserved
*********************************************************************/
#include "EC_Achievement.h"

#include "LuaState.h"
#include "LuaUtil.h"
#include "ScriptValue.h"
#include "LuaFuncCommon.h"

#include "ExpTypes.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_IvtrItem.h"
#include "EC_HostPlayer.h"
#include "EC_Global.h"
#include "elementdataman.h"
#include "TaskTempl.h"
#include "TaskTemplMan.h"
#include "EC_GPDataType.h"
#include "EC_RTDebug.h"
#include "ElementSkill.h"
#include "EC_FixedMsg.h"
#include <ACounter.h>
#include "EC_Counter.h"
#include "EC_Utility.h"
#include "EC_Algorithm.h"
#include "EC_GameUIMan.h"
#include "EC_UIManager.h"
#include "DlgAchivItem.h"
#include "DlgAchivMain.h"
#include "DlgAchivItemMan.h"

using namespace S2C;

//////////////////////////////////////////////////////////////////////////
//macro
#define ACHIEVEMENT_TABLE_NAME "AchievementCfg"

//////////////////////////////////////////////////////////////////////////
//local class
class CompareAchSubClassOrder
{
public:
	bool operator ()(sAchievementSubClass * p1, sAchievementSubClass * p2)
	{
		return p1->id < p2->id;
	}
};



//////////////////////////////////////////////////////////////////////////
// implementation
abase::hash_map<unsigned int,sAchievementCondCommon> CECAchievementMan::s_mapCondCommon;
abase::hash_map<unsigned int,sAchievementClass> CECAchievementMan::s_mapAchievementClass;
CECAchievementMan CECAchievementMan::s_DefaultData;

sAchievementCondCommon::sAchievementCondCommon()
{
	id		= 0;
	dataSize= 0;
}

sAchievement::sAchievement()
{
	id				= 0;
	name			= _AL("");
	strDesc			= _AL("");
	strPreCondDesc	= _AL("");
	strMsg          = _AL("");
	iClass			= 0;
	iSubClass		= 0;
	awardScore		= 0;
	awardItem		= 0;
	awardMoney		= 0;
	bFinish			= false;
	bActive			= false;
	bMarkFinish		= false;
	iFinishTime		= 0;
	bGetAward		= false;
	strOwnerName	= _AL("");
}

void sAchievement::GetStrDesc(AWString& str)
{
	str = strPreCondDesc;
	for (int i=0; i<(int)vecCond.size(); ++i)
	{
		AWString strCond;
		vecCond[i].GetStrDesc(strCond);
		str += _AL("\r");
	    str += strCond;		
	}
}

void sAchievement::SetFinish(const bool finish)
{
	bFinish = finish;
	if (finish)
	{
		for (int i=0; i<(int)vecCond.size(); ++i)
		{
			vecCond[i].bFinish = true;
		}
	}
}

sAchievementClass::sAchievementClass()
{
	id	= 0;
	name= _AL("");
}

sAchievementSubClass::sAchievementSubClass()
{
	id	= 0;
	name= _AL("");
	awardNum = 0;
}

CECAchievementMan::CECAchievementMan()
{
	m_AchievePoint = 0;
	m_CurAchievePoint = 0;
};

CECAchievementMan::~CECAchievementMan()
{
    Release();
}


bool CECAchievementMan::Init()
{
	Release();

	AString strMsg;
	if (!s_DefaultData.m_mapAchievement.size())
	{
		ASSERT(0);
		return false;
	}

	//有默认数据,不必从脚本中读。
	CopyHashMap(m_mapAchievement, s_DefaultData.m_mapAchievement);
	m_AchievePoint = 0;
	m_CurAchievePoint = 0;
    return true; 
}

bool CECAchievementMan::PreLoadDataFromScript()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	CLuaState *pState = g_LuaStateMan.GetAIState();
	if (!pState)
	{
		ASSERT(0);
		return false;
	}

	pState->Call(ACHIEVEMENT_TABLE_NAME, "GetSelf", args, results);
	if( results.size()<1 || CScriptValue::SVT_ARRAY!=results[0].m_Type)
	{
		return false;
	} 

    const CScriptValue* pConds		= luaf::GetValue(results[0],_AL("conds"));
	const CScriptValue* pAchievement	= luaf::GetValue(results[0],_AL("achievement"));
	const CScriptValue* pClass		= luaf::GetValue(results[0],_AL("class"));
	if (!pConds || !pAchievement || !pClass || CScriptValue::SVT_ARRAY!=pConds->m_Type || CScriptValue::SVT_ARRAY!=pAchievement->m_Type || CScriptValue::SVT_ARRAY!=pClass->m_Type)
	{
		return false;
	}
	
	//common achievement condition.
	int i=0;
	if (s_mapAchievementClass.size()==0)
	{
		for (i=0; i<(int)pConds->m_ArrKey.size(); ++i)
		{
			if (CScriptValue::SVT_ARRAY!=pConds->m_ArrVal[i].m_Type)
			{
				continue;
			}
			sAchievementCondCommon condCommon;
			condCommon.id = (unsigned int)pConds->m_ArrKey[i].GetDouble();

			//todo: liudong change this, waiting for liuwenwei.  [3/10/2009] 
			//luaf::GetValue(condCommon.dataSize,pCond->m_ArrVal[i],_AL("size"));
			if (condCommon.id<1000)
			{
				condCommon.dataSize = 1;
			}else
			{
				condCommon.dataSize = 2;
			}
			s_mapCondCommon[condCommon.id] = condCommon;
		}
	}

	//achievement class information.
	if (s_mapAchievementClass.size()==0)
	{
		for (i=0; i<(int)pClass->m_ArrKey.size(); ++i)
		{
			const CScriptValue& var = pClass->m_ArrVal[i];
			if (CScriptValue::SVT_ARRAY!=var.m_Type)
			{
				continue;
			}
			sAchievementClass aClass;
			aClass.id = (unsigned int)pClass->m_ArrKey[i].GetDouble();
			luaf::GetValue(aClass.name, var, _AL("name"));

			//两层目录结构		
			abase::vector<sAchievementSubClass*> vecSubClass;  // 成就子类
			for (int j=0; j<(int)var.m_ArrKey.size(); ++j)
			{
				const CScriptValue& subVar = var.m_ArrVal[j];
				if (CScriptValue::SVT_ARRAY!=subVar.m_Type)
				{
					continue;
				}
				sAchievementSubClass* pSubClass = new sAchievementSubClass();
				pSubClass->id = (unsigned int)var.m_ArrKey[j].GetDouble();
				luaf::GetValue(pSubClass->name, subVar, _AL("name"));
				vecSubClass.push_back(pSubClass);
			}
			BubbleSort(vecSubClass.begin(),vecSubClass.end(),CompareAchSubClassOrder());
			abase::vector<sAchievementSubClass*>::iterator it;
			for (it=vecSubClass.begin();it!=vecSubClass.end();++it)
			{
				sAchievementSubClass* pSubClass = *it;
				if (pSubClass)
				{
					aClass.vecSubClass.push_back(*pSubClass);
					delete pSubClass;
				}
			}
			s_mapAchievementClass[aClass.id] = aClass;
		}
	}

	AWString msgCommon;
	if(!luaf::GetValue(msgCommon,*pAchievement,_AL("msg")))
	{
		msgCommon = _AL("error");
	}

	//achievements.
	for (i=0; i<(int)pAchievement->m_ArrKey.size(); ++i)
	{
		const CScriptValue& var = pAchievement->m_ArrVal[i];
		if (CScriptValue::SVT_NUMBER!=pAchievement->m_ArrKey[i].m_Type || CScriptValue::SVT_ARRAY!=var.m_Type)
		{
			continue;
		}

		sAchievement achievement;
		achievement.id = (unsigned int)pAchievement->m_ArrKey[i].GetDouble();
		luaf::GetValue(achievement.name,var,_AL("name"));
		luaf::GetValue(achievement.strDesc,var,_AL("desc"));
		luaf::GetValue(achievement.strPreCondDesc,var,_AL("preCondDesc"));
		luaf::GetValue(achievement.iClass,var,_AL("mainClass"));
		luaf::GetValue(achievement.iSubClass, var, _AL("subClass"));
		luaf::GetValue(achievement.awardScore,var,_AL("awardScore"));
		luaf::GetValue(achievement.awardItem, var, _AL("awardItem"));
		luaf::GetValue(achievement.awardMoney, var, _AL("awardMoney"));
		if(!luaf::GetValue(achievement.strMsg,var,_AL("msg")))
		{
			achievement.strMsg = msgCommon;
		}

		//premiss
		luaf::GetValue(achievement.vecPremissID,var,_AL("premissID"));

		//conditions
		abase::vector<const CScriptValue*> vecConds ;
		if (luaf::GetValue(vecConds,var,_AL("conds")))
		{
			int j;
			for (j=0; j<(int)vecConds.size(); ++j)
			{
				sAchievementCond cond(achievement.id,j);
				if(!luaf::GetValue(cond.id,*(vecConds[j]),_AL("id")))
				{
					break;
				}
				achievement.vecCond.push_back(cond);
			}
			if (j!=(int)vecConds.size())
			{
				continue;
			}
		}

		s_DefaultData.m_mapAchievement[achievement.id] = achievement;
	}

	return true;
}

void CECAchievementMan::Release()
{
	m_mapAchievement.clear();
	m_AchievePoint = 0;
	m_CurAchievePoint = 0;
}

sAchievement* CECAchievementMan::GetAchievement(const unsigned int idAchievement)
{
	if (m_mapAchievement.find(idAchievement) == m_mapAchievement.end())
	{
		return NULL;
	}
	return &(m_mapAchievement[idAchievement]);
}

const sAchievementCondCommon* CECAchievementMan::GetCondCommon(const unsigned int idCond)
{
	if (s_mapCondCommon.find(idCond) == s_mapCondCommon.end())
	{
		return NULL;
	}
	return &(s_mapCondCommon[idCond]);
}

const sAchievementClass* CECAchievementMan::GetAchievementClass(const unsigned int idClass)
{
	if (s_mapAchievementClass.find(idClass) == s_mapAchievementClass.end())
	{
		return NULL;
	}
	return &(s_mapAchievementClass[idClass]);
}

bool CECAchievementMan::_UpdateData(unsigned int& sz, const S2C::achievement* p,const unsigned int buffSize)
{
	sz = 0;
	
	try
	{
		CECDataReader dr((void*)p, buffSize);
		
		unsigned short achieve_id = dr.Read_short();
		unsigned short premiss_mask = dr.Read_short();
		unsigned char premiss_count = dr.Read_char();

		sAchievement* pAchievement;
		if (!(pAchievement=GetAchievement(achieve_id)))
		{
			ASSERT(0);
			return false;
		}
		if ( pAchievement->vecCond.size() > (sizeof(premiss_mask)*8) )
		{
			ASSERT(0);
			return false;
		}
		unsigned int i;
		for (i=0; i<pAchievement->vecCond.size(); ++i)
		{
			pAchievement->vecCond[i].bFinish =  (premiss_mask & (1<<i)) ? true:false;
		}
		
		for (i=0; i<premiss_count; ++i)
		{
			unsigned int index = dr.Read_char();
			if (index >= pAchievement->vecCond.size())
			{
				ASSERT(0);
				return false;
			}
			sAchievementCond& cond = pAchievement->vecCond[index];
			ASSERT(!cond.bFinish);
			const sAchievementCondCommon* pCondCommon =GetCondCommon(cond.id);
			if(!pCondCommon || !pCondCommon->dataSize)
			{
				ASSERT(0);
				return false;
			}
			const char* pData = (const char*)dr.Read_Data(pCondCommon->dataSize);
			cond.SetData(pData,pCondCommon->dataSize);
		}
		pAchievement->bActive = true;
		pAchievement->bFinish = false;
		sz = dr.GetPos();
	}catch (CECException&)
	{
		ASSERT(0);
		return false;
	}
	return true;
}

bool CECAchievementMan::UpdateData(const S2C::player_achievement* p,const unsigned int buffSize)
{
	try
	{
		CECDataReader dr((void*)p, buffSize);
        
		const unsigned int mapCount = dr.Read_DWORD();//zero means unfinished.
		const unsigned int MAX_ID   = mapCount*8-1;
		const char* pMap = (const char*)dr.Read_Data(mapCount);
		abase::hash_map<unsigned int,sAchievement>::iterator it;
		for (it=m_mapAchievement.begin(); it!=m_mapAchievement.end(); ++it)
		{
			sAchievement& achievement = it->second;
			bool bFinish = false;
			if (mapCount)
			{
				if (achievement.id <= MAX_ID)
				{
					// 服务器端只是发送前面成就状态有改变的掩码
					bFinish = (pMap[achievement.id/8] & (1<<achievement.id%8))?true:false;
				}	
			}
			achievement.SetFinish(bFinish);
			achievement.bActive = achievement.bFinish;
		}

		const unsigned short active_count = dr.Read_short();
		unsigned int i;
		for (i=0; i<active_count; ++i)
		{
			unsigned int sz =0;
			if(!_UpdateData(sz, (const achievement*)dr.GetCurAddr(), buffSize-dr.GetPos()))
			{
				ASSERT(0);
				throw CECException(CECException::TYPE_DATAERR);
			}
			dr.Read_Data(sz);
		}
		m_AchievePoint = dr.Read_int();
		m_CurAchievePoint = dr.Read_int();
		const unsigned int finish_count = dr.Read_int();
		for (i =0; i< finish_count; ++i)
		{
			unsigned short	id = dr.Read_short();
			int				iTime = dr.Read_int();
			if (m_mapAchievement.find(id) == m_mapAchievement.end())
			{
				ASSERT(0);
			}
			m_mapAchievement[id].bMarkFinish = true;
			m_mapAchievement[id].iFinishTime = iTime;
		}

		const unsigned int awardMapCount = dr.Read_DWORD();
		const unsigned int AWARD_MAX_ID   = awardMapCount*8-1;
		const char* pAwardMap = (const char*)dr.Read_Data(awardMapCount);
		for (it=m_mapAchievement.begin(); it!=m_mapAchievement.end(); ++it)
		{
			sAchievement& achievement = it->second;
			bool bGetAward = false;
			if (mapCount)
			{
				if (achievement.id <= AWARD_MAX_ID)
				{
					// 服务器端只是发送前面成就状态有改变的掩码
					bGetAward = (pAwardMap[achievement.id/8] & (1<<achievement.id%8))?true:false;
				}	
			}
			achievement.bGetAward = bGetAward;
			if (achievement.awardItem != 0 && !achievement.bGetAward && achievement.bFinish)
				s_mapAchievementClass[achievement.iClass].vecSubClass[achievement.iSubClass].awardNum ++;
		}
	}catch (CECException&)
	{
		ASSERT(0);
		Init();
		return false;
	}
	return true;
}

bool CECAchievementMan::UpdateData(const S2C::player_achievement_finish* p,const unsigned int buffSize)
{
	try
	{
		CECDataReader dr((void*)p, buffSize);
		unsigned int achieve_id = dr.Read_short();
		sAchievement* pAchievement=GetAchievement(achieve_id);
		if (!pAchievement)
		{
			ASSERT(0);
			return false;
		} 
		pAchievement->bActive = true;
		pAchievement->bGetAward = false;
		pAchievement->SetFinish(true);
		int point = dr.Read_int();
		m_AchievePoint += point;
		m_CurAchievePoint += point;
		int time = dr.Read_int();
		if (time)
		{
			pAchievement->bMarkFinish = true;
			pAchievement->iFinishTime = time;
		}
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GAIN_ACHIEVEMENT_POINT, point);
		CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		CDlgAchivPop* pDlgAchivPop = (CDlgAchivPop*)pGameUI->GetDialog("Win_Achievement_PopUp");
		pDlgAchivPop->SetAchiv(pAchievement);
		pDlgAchivPop->Show(true);

		if (pAchievement->awardItem != 0 && !pAchievement->bGetAward && pAchievement->bFinish)
			s_mapAchievementClass[pAchievement->iClass].vecSubClass[pAchievement->iSubClass].awardNum ++;

		CDlgAchivMain* pDlgAchivMain = (CDlgAchivMain*)pGameUI->GetDialog("Win_Achievement");
		pDlgAchivMain->SetTarget(0);
	}catch (CECException&)
	{
		ASSERT(0);
		return false;
	}

	return true;
}

bool CECAchievementMan::UpdateData(const S2C::player_achievement_active* p,const unsigned int buffSize)
{
	if (!p)
	{
		ASSERT(0);
		return false;
	}

	unsigned int sz;
	return _UpdateData(sz,&(p->achieve),buffSize);
}
	
bool CECAchievementMan::UpdateData(const S2C::player_premiss_data* p,const unsigned int buffSize)
{
	try
	{
		CECDataReader dr((void*)p, buffSize);
		unsigned int achieve_id = dr.Read_short();
		unsigned int premiss_id = dr.Read_char();
		sAchievement* pAchievement=GetAchievement(achieve_id);
		if (!pAchievement || premiss_id>=pAchievement->vecCond.size())
		{
			ASSERT(0);
			return false;
		} 
		sAchievementCond& cond = pAchievement->vecCond[premiss_id];
		const sAchievementCondCommon* pCondCommon =GetCondCommon(cond.id);
		if(!pCondCommon || !pCondCommon->dataSize)
		{
			ASSERT(0);
			return false;
		}
		const char* pData = (const char*)dr.Read_Data(pCondCommon->dataSize);
		cond.SetData(pData,pCondCommon->dataSize);

	}catch (CECException&)
	{
		ASSERT(0);
		return false;
	}

	return true;
}
	
bool CECAchievementMan::UpdateData(const S2C::player_premiss_finish* p,const unsigned int buffSize)
{
	try
	{
		CECDataReader dr((void*)p, buffSize);
		unsigned int achieve_id = dr.Read_short();
		unsigned int premiss_id = dr.Read_char();
		sAchievement* pAchievement  =GetAchievement(achieve_id);
		if (!pAchievement || premiss_id>=pAchievement->vecCond.size())
		{
			ASSERT(0);
			return false;
		} 
		sAchievementCond& cond = pAchievement->vecCond[premiss_id];
		cond.bFinish = true;

		CECGameUIMan *pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		CDlgAchivItemMan *pDlgAchivMain = (CDlgAchivItemMan*)pGameUIMan->GetDialog("Win_Achievement_Sub");
		CDlgAchivItemExpand *expand = pDlgAchivMain->GetAchivItemExpand();
		if (expand && expand->GetAchiv() && expand->GetAchiv()->id == achieve_id)
			expand->SetAchiv(pAchievement);
			
	}catch (CECException&)
	{
		ASSERT(0);
		return false;
	}

	return true;
}

bool CECAchievementMan::UpdateData(const S2C::player_achievement_map* p,const unsigned int buffSize)
{
	Init();
	try
	{
		CECDataReader dr((void*)p, buffSize);
		int target				= dr.Read_int();
		m_AchievePoint			= dr.Read_int();
		const unsigned int mapCount = dr.Read_DWORD();//zero means unfinished.
		const unsigned int MAX_ID   = mapCount*8-1;
		const char* pMap = (const char*)dr.Read_Data(mapCount);
		abase::hash_map<unsigned int,sAchievement>::iterator it;
		for (it=m_mapAchievement.begin(); it!=m_mapAchievement.end(); ++it)
		{
			sAchievement& achievement = it->second;
			bool bFinish = false;
			if (mapCount)
			{
				if (achievement.id <= MAX_ID)
				{
					// 服务器端只是发送前面成就状态有改变的掩码
					bFinish = (pMap[achievement.id/8] & (1<<achievement.id%8))?true:false;
				}	
			}
			achievement.SetFinish(bFinish);
			achievement.bActive = achievement.bFinish;
		}
	}catch (CECException&)
	{
		ASSERT(0);
		Init();
		return false;
	}

	return true;
}

bool CECAchievementMan::UpdateData(const S2C::player_obtain_achieve_award* p, const unsigned int buffSize)
{
	try
	{
		CECDataReader dr((void*)p, buffSize);
		unsigned int achieve_id = dr.Read_short();
		sAchievement* pAchievement=GetAchievement(achieve_id);
		if (!pAchievement)
		{
			ASSERT(0);
			return false;
		} 
		pAchievement->bGetAward = true;

		CECGameUIMan *pGameUIMan = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

		/*
		CDlgAchivPop *pDlgAchivPop = (CDlgAchivPop*) pGameUIMan->GetDialog("Win_Achievement_PopUp");
		pDlgAchivPop->SetAchiv(pAchievement);
		pDlgAchivPop->Show(true);
		*/

		if (s_mapAchievementClass[pAchievement->iClass].vecSubClass[pAchievement->iSubClass].awardNum>0)
			s_mapAchievementClass[pAchievement->iClass].vecSubClass[pAchievement->iSubClass].awardNum --;

		CDlgAchivMain *pDlgAchivMain = (CDlgAchivMain*) pGameUIMan->GetDialog("Win_Achievement");
		pDlgAchivMain->SetTarget(0);

	}catch (CECException&)
	{
		ASSERT(0);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// achievement condition
//////////////////////////////////////////////////////////////////////////
sAchievementCond::sAchievementCond(const unsigned int _idAchievement,const int _index)
{
	id				= -1;
	bFinish			= false;
	index			= _index;
	pData			= NULL;
	idAchievement	= _idAchievement;
}

sAchievementCond::~sAchievementCond()
{
	if (pData)
	{
		delete pData;
		pData = NULL;
	}
}

sAchievementCond::sAchievementCond(const sAchievementCond& src)
{
	*this = src;
}

sAchievementCond& sAchievementCond::operator=(const sAchievementCond& rhs)
{
	if (this==&rhs)
	{
		return *this;
	}
	id				= rhs.id;
	bFinish			= rhs.bFinish;
	index			= rhs.index;
	idAchievement	= rhs.idAchievement;
    pData			= NULL;
	if (rhs.pData)
	{
		const sAchievementCondCommon* pCondCommon = CECAchievementMan::GetCondCommon(id);
		if (pCondCommon && pCondCommon->dataSize>0)
		{
			pData = new char[pCondCommon->dataSize];
			memcpy(pData,rhs.pData,pCondCommon->dataSize);
		}
	}
	return *this;
}

bool sAchievementCond::GetDataInt(int& data) const
{
	data =0 ;
	//when finish achievement, this data is no use.
	if (bFinish || !pData || !CECAchievementMan::GetCondCommon(id))
	{
		return false;
	}

	unsigned int dataSize =CECAchievementMan::GetCondCommon(id)->dataSize;
	if(1==dataSize)
	{
		data = *pData;
	}else if (2==dataSize)
	{
		data = *(short*)pData;
	}else if (4==dataSize)
	{
		data = *(int*)pData;
	}else
	{
		return false;
	}
	return true;
}

void sAchievementCond::SetData(const char* data, const unsigned int size)
{
	if (pData)
	{
		delete[] pData;
		pData = NULL;
	}
	if (data && size)
	{
		pData = new char[size];
		memcpy(pData,data,size);
	}
}

void sAchievementCond::GetStrDesc(AWString& str)
{
	str = _AL("error");
	abase::vector<CScriptValue> args,results;
	CLuaState* pState = g_LuaStateMan.GetAIState();
	if (!pState)
	{
		ASSERT(0);
		return ;
	}
	args.push_back((double)idAchievement);
	args.push_back((double)(index+1));//C is zero based, lua is 1 based.
	args.push_back(bFinish);
	args.push_back((void*)this);

	pState->Call(ACHIEVEMENT_TABLE_NAME, "GetCondDesc", args, results);
	if( results.size()>=1 && CScriptValue::SVT_STRING==results[0].m_Type)
	{
		results[0].RetrieveAWString(str);
 	}
}

void sAchievementCond::GetStrInfo(AWString& str)
{
	str = _AL("error");
	abase::vector<CScriptValue> args,results;
	CLuaState* pState = g_LuaStateMan.GetAIState();
	if (!pState)
	{
		ASSERT(0);
		return ;
	}
	args.push_back((double)idAchievement);
	args.push_back((double)(index+1));//C is zero based, lua is 1 based.
	args.push_back(bFinish);
	args.push_back((void*)this);

	pState->Call(ACHIEVEMENT_TABLE_NAME, "GetCondInfo", args, results);
	if( results.size()>=1 && CScriptValue::SVT_STRING==results[0].m_Type)
	{
		results[0].RetrieveAWString(str);
 	}
}

bool sAchievementCond::GetProgress(int &current, int &max)
{
	current = 0;
	max = 0;

	abase::vector<CScriptValue> args,results;
	CLuaState* pState = g_LuaStateMan.GetAIState();
	if (!pState)
	{
		ASSERT(0);
		return false;
	}

	args.push_back((double)idAchievement);
	args.push_back((double)(index+1));//C is zero based, lua is 1 based.
	args.push_back(bFinish);
	args.push_back((void*)this);

	pState->Call("AchievementCfg", "GetCondProg", args, results);
	if( results.size()>=2
		&& CScriptValue::SVT_NUMBER==results[0].m_Type && CScriptValue::SVT_NUMBER==results[1].m_Type)
	{
		current = results[0].GetInt();
		max = results[1].GetInt();

		return true;
	}

	return false;
}