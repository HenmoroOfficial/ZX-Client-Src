/********************************************************************
  created:	   9/5/2006  
  filename:	   LuaGameAPI.cpp
  author:      Wangkuiwu  
  description: 
  Copyright (c) , All Rights Reserved.
*********************************************************************/



#include "AFI.h"
#include "LuaGameAPI.h"
#include "LuaUtil.h"
#include "LuaBind.h"
#include "LuaAPI.h"
#include "LuaState.h"
#include "LuaDlg.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_GameSession.h"
#include "EC_Player.h"
#include "EC_ManPlayer.h"
#include "EC_RoleTypes.h"
#include "EC_HostPlayer.h"
#include "EC_Skill.h"
#include "EC_GameUIMan.h"
#include "EC_GameUIMan.h"
#include "DlgTalisman.h"
#include "EC_UIManager.h"
#include "EC_ManNPC.h"
#include "EC_NPC.h"
#include "LuaEvent.h"
#include "scriptvalue.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_HPWork.h"
#include "EC_HPWorkTrace.h"
#include "EC_HPWorkSpell.h"
#include "EC_AutoMove.h"
#include "EC_ManMatter.h"
#include "TowerDefencePF.h"
#include "AMSoundEngine.h"
#include "AMSoundBufferMan.h"
#include "AutoMove\MoveAgent.h"
#include "TaskTempl.h"
#include "EC_TaskInterface.h"
#include "EC_Secretary.h"

using namespace abase;
using namespace LuaBind;

#define LOCAL_DIR "userdata\\tinygame"

#define SetKVPaires(key_array, val_array, key, value)	\
	do {\
		CScriptValue sv1;\
		(sv1).SetVal(key);\
		(key_array).push_back(sv1);\
		CScriptValue sv2;\
		(sv2).SetVal(value);\
		(val_array).push_back(sv2);\
	} while(0);

//////////////////////////////////////////////////////////////////////////
//apis

inline CECGame * GetGame()
{
	return g_pGame;
}
inline CECGameRun * GetGameRun()
{
	return g_pGame->GetGameRun();
}
inline CECWorld * GetWorld()
{
	return g_pGame->GetGameRun()->GetWorld();
}
inline CECGameSession * GetGameSession()
{
	return g_pGame->GetGameSession();
}
inline CECHostPlayer * GetHostPlayer()
{
	return GetGameRun()->GetHostPlayer();
}

inline CECGameUIMan * GetGameUIMan()
{
	return GetGameRun()->GetUIManager()->GetInGameUIMan();
}

/*
//return a table 
void GetHostProp(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	ROLEBASICPROP &rbp =	GetHostPlayer()->GetBasicProps();
	ROLEEXTPROP   &rep =    GetHostPlayer()->GetExtendProps();

	vector<CScriptValue> keys;
	vector<CScriptValue> vals;
	
	CScriptValue  k, v;
	double rate;

	rate = rbp.iCurHP /(double)rep.max_hp;
	k.SetAString("hp");
	keys.push_back(k);
	v.SetDouble(rate);
	vals.push_back(v);
	
	rate = rbp.iCurMP /(double)rep.max_mp;
	k.SetAString("mp");
	keys.push_back(k);
	v.SetDouble(rate);
	vals.push_back(v);

	CScriptValue  a;
	a.SetArray(vals, keys);
	
	
	results.push_back(a);
}
*/

//currently return a list
void GetHostProp(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	ROLEBASICPROP &rbp =	GetHostPlayer()->GetBasicProps();
	ROLEEXTPROP   &rep =    GetHostPlayer()->GetExtendProps();

	
	CScriptValue  v;
	double rate;

	rate = rbp.iCurHP /(double)rep.max_hp;
	v.SetVal(rate);
	results.push_back(v);
	
	rate = rbp.iCurMP /(double)rep.max_mp;
	v.SetVal(rate);
	results.push_back(v);


	rate = 0.0;
	CECInventory *pPack = GetHostPlayer()->GetPack(IVTRTYPE_EQUIPPACK);
	if( pPack->GetItem(16) && pPack->GetItem(16)->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART )
	{
		CECIvtrTalismanMain *pItem = (CECIvtrTalismanMain*)pPack->GetItem(16);
		
		int nNow, nMax;
		nNow = rbp.iTalismanStamina;
		nMax = pItem->GetEssence().ess.stamina;
		rate = nNow/(double)nMax;	
	}
	v.SetVal(rate);
	results.push_back(v);

}
//IMPLEMENT_SCRIPT_API(GetHostProp);

void GetTargetProp(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	int id = GetHostPlayer()->GetSelectedTarget();
	CScriptValue  val((double)id);
	
	results.push_back(val);

	CECNPC * pNpc = NULL;
	if (ISNPCID(id))
	{
		pNpc = GetWorld()->GetNPCMan()->GetNPC(id);
	}
	//todo: refine the default
	val.SetVal(0.0);
	if (pNpc && pNpc->IsMonsterNPC())
	{
		ROLEBASICPROP &rbp =	pNpc->GetBasicProps();
		ROLEEXTPROP   &rep =    pNpc->GetExtendProps();
		
		double rate = rbp.iCurHP/(double)rep.max_hp;
		val.SetVal(rate);
	}
	results.push_back(val);
}
//IMPLEMENT_SCRIPT_API(GetTargetProp);

 
void HealHp(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	//just for test
	//GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, 0);
	CDlgTalisman *pDlg = (CDlgTalisman *)GetGameUIMan()->GetDialog("Win_Talisman");
	if (pDlg)
	{
		pDlg->UseTalismanItem(1);
	}

}
//IMPLEMENT_SCRIPT_API(HealHp);

void HealMp(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	//just for test
	//GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, 1);
	CDlgTalisman *pDlg = (CDlgTalisman *)GetGameUIMan()->GetDialog("Win_Talisman");
	if (pDlg)
	{
		pDlg->UseTalismanItem(2);
	}

}
//IMPLEMENT_SCRIPT_API(HealMp);

void HealDp(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	//just for test
	//GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, 1);
	CDlgTalisman *pDlg = (CDlgTalisman *)GetGameUIMan()->GetDialog("Win_Talisman");
	if (pDlg)
	{
		pDlg->UseTalismanItem(4);
	}
	
}
//IMPLEMENT_SCRIPT_API(HealDp);

void HealStamina(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	CDlgTalisman *pDlg = (CDlgTalisman *)GetGameUIMan()->GetDialog("Win_Talisman");
	if (pDlg)
	{
		pDlg->UseTalismanItem(3);
	}

}
//IMPLEMENT_SCRIPT_API(HealStamina);

void SelectMonster(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	// 脚本触发的自动选怪，优先选怪
	GetGameRun()->PostMessage(MSG_HST_AUTOSELECT, MAN_PLAYER, 0, 1);
}
//IMPLEMENT_SCRIPT_API(SelectMonster);

void NormalAttack(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	GetHostPlayer()->CmdNormalAttack();
}
//IMPLEMENT_SCRIPT_API(NormalAttack);

void CanAttack(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	int atk = GetHostPlayer()->AttackableJudge(GetHostPlayer()->GetSelectedTarget(), false);

	CScriptValue v;
	v.SetVal((atk> 0));

	results.push_back(v);
}
//IMPLEMENT_SCRIPT_API(CanAttack);

void PickUp(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	GetHostPlayer()->CmdPickup();
}
//IMPLEMENT_SCRIPT_API(PickUp);

void IsPreparingPickingUp(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	bool ret = false;
	int work_id = GetHostPlayer()->GetHPWorkMan()->GetCurWorkID();
	if (work_id == CECHPWork::WORK_TRACEOBJECT)
	{
		CECHPWorkTrace * pWorkTrace = (CECHPWorkTrace *)GetHostPlayer()->GetHPWorkMan()->GetCurWork();
		if (pWorkTrace && pWorkTrace->GetTraceReason() == CECHPWorkTrace::TRACE_PICKUP)
		{
			ret = true;
		}
	}
	results.push_back(CScriptValue(ret));
}
//IMPLEMENT_SCRIPT_API(IsPreparingPickingUp);

void IsPickingUp(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	int work_id = GetHostPlayer()->GetHPWorkMan()->GetCurWorkID();
	bool	ret = (work_id == CECHPWork::WORK_PICKUP);
	results.push_back(CScriptValue(ret));
}
//IMPLEMENT_SCRIPT_API(IsPickingUp);


void IsSkillAvailable(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() != 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		return;
	}
	CDlgTalisman *pDlg = (CDlgTalisman *)GetGameUIMan()->GetDialog("Win_Talisman");
	int skill_id = 0;
	if (pDlg)
	{
		int index = args[0].GetInt();
		skill_id = pDlg->CheckTalismanSkill(index);
	}
	results.push_back(CScriptValue(skill_id != 0));
	results.push_back(CScriptValue(double(skill_id)));
}
//IMPLEMENT_SCRIPT_API(IsSkillAvailable);

void CheckSkillCastCondition(vector<CScriptValue>&args, vector<CScriptValue>& results)
{
	if (args.size() != 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		//ReportError(NULL, "CheckSkillCastCondition, args error");
		return;
	}

	int skill_id = args[0].GetInt();
	CECSkill * pSkill = GetHostPlayer()->GetSkill(skill_id);
	int ret = -1;
	if (pSkill)
	{
		ret =	GetHostPlayer()->CheckSkillCastCondition(pSkill);
	}

	results.push_back(CScriptValue(ret == 0));
	
}
//IMPLEMENT_SCRIPT_API(CheckSkillCastCondition);

void IsSkillCooling(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() != 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		//ReportError(NULL, "IsSkillCooling, args error");
		return;
	}

	int skill_id = args[0].GetInt();
	
	bool ret = false;
	
	CECSkill * pSkill = GetHostPlayer()->GetSkill(skill_id);

	if (pSkill)
	{
		ret = !pSkill->ReadyToCast();
	}
	
	results.push_back(CScriptValue(ret));
}
//IMPLEMENT_SCRIPT_API(IsSkillCooling);


void UseSkill(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() != 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		//ReportError(NULL, "UseSkill, args error");
		return;
	}
	CDlgTalisman *pDlg = (CDlgTalisman *)GetGameUIMan()->GetDialog("Win_Talisman");
	CScriptValue val(0.0);
	if (pDlg)
	{
		int index =  args[0].GetInt();
		int id =	pDlg->UseTalismanSkill(index);
		val.SetVal(id);
	}
	
	results.push_back(val);

}
//IMPLEMENT_SCRIPT_API(UseSkill);

void IsPreparingSkill(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() != 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		//ReportError(NULL, "IsPreparingSkill, args error");
		return;
	}

	int work_id = GetHostPlayer()->GetHPWorkMan()->GetCurWorkID();
	int skill_id = args[0].GetInt();

	bool ret = false;
	
	if (work_id == CECHPWork::WORK_TRACEOBJECT)
	{
		CECHPWorkTrace * pWorkTrace = (CECHPWorkTrace *)GetHostPlayer()->GetHPWorkMan()->GetCurWork();
		if (pWorkTrace && pWorkTrace->GetTraceReason() == CECHPWorkTrace::TRACE_SPELL)
		{
			ret = (pWorkTrace->GetPrepSkill()->GetSkillID() == skill_id);
		}
	}

// 	AString str;
// 	str.Format("%d %d\n", work_id, ret);
// 	OutputDebugStringA(str);
	
	results.push_back(CScriptValue(ret));
}
//IMPLEMENT_SCRIPT_API(IsPreparingSkill);

void IsUsingSkill(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() != 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		//ReportError(NULL, "IsUsingSkill, args error");
		return;
	}

	int work_id = GetHostPlayer()->GetHPWorkMan()->GetCurWorkID();
	int skill_id = args[0].GetInt();

	bool ret = false;
	
	if (work_id == CECHPWork::WORK_SPELLOBJECT)
	{
		CECHPWorkSpell * pWorkSpell = (CECHPWorkSpell *)GetHostPlayer()->GetHPWorkMan()->GetCurWork();
		if (pWorkSpell && pWorkSpell->GetSkill())
		{
			ret = (pWorkSpell->GetSkill()->GetSkillID() == skill_id);
		}
	}

// 	AString str;
// 	str.Format("%d %d\n", work_id, ret);
// 	OutputDebugStringA(str);
	
	results.push_back(CScriptValue(ret));
}
//IMPLEMENT_SCRIPT_API(IsUsingSkill);


void StartAutoMove(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() != 3 || args[0].m_Type != CScriptValue::SVT_NUMBER
		|| args[1].m_Type != CScriptValue::SVT_NUMBER
		|| args[2].m_Type != CScriptValue::SVT_NUMBER)
	{
		//ReportError(NULL, "StartAutoMove, args error");
		return;
	}
	A3DVECTOR3   vStart, vEnd;
	vStart = GetHostPlayer()->GetPos();
	vEnd.x = (float)args[0].GetDouble();
	vEnd.y = (float)args[1].GetDouble();
	vEnd.z = (float)args[2].GetDouble();
	int end_id = GetWorld()->GetInstanceID();

	g_ECAutoMove.SearchPath(vStart, vEnd, end_id);
}
//IMPLEMENT_SCRIPT_API(StartAutoMove);

void IsAutoMoving(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	bool ret;
	int work_id = GetHostPlayer()->GetHPWorkMan()->GetCurWorkID();
	ret = (work_id == CECHPWork::WORK_AUTOMOVE);
	results.push_back(CScriptValue(ret));
}
//IMPLEMENT_SCRIPT_API(IsAutoMoving);

void GetHostPos(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	A3DVECTOR3 pos = GetHostPlayer()->GetPos();
	CScriptValue val;
	val.SetVal((double)pos.x);
	results.push_back(val);
	val.SetVal((double)pos.y);	
	results.push_back(val);
	val.SetVal((double)pos.z);
	results.push_back(val);
}
//IMPLEMENT_SCRIPT_API(GetHostPos);

void CheckDrops(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() != 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		return;
	}
	float range = (float)args[0].GetDouble();

	if (range > 10.0f)
	{
		range = 10.0f;
	}
	
	int count = 0;
	if (GetWorld() && GetWorld()->GetMatterMan())
	{
		count = GetWorld()->GetMatterMan()->MatterInRange(range, true);
	}
	results.push_back(CScriptValue((double)count));

}
//IMPLEMENT_SCRIPT_API(CheckDrops);


void Report2Svr(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	GetGameSession()->bot_run();
}
//IMPLEMENT_SCRIPT_API(Report2Svr);

void GetCursorPosition(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	//int x, y;
	//g_pGame->GetGameRun()->GetInputCtrl()->GetMousePos(&x, &y);

	POINT p;
	GetCursorPos(&p);
	ScreenToClient(g_pGame->GetGameInit().hWnd, &p);

	results.push_back((double)p.x);
	results.push_back((double)p.y);
}
IMPLEMENT_SCRIPT_API(GetCursorPosition);

// find path for towerDenfense, tiny game for zhuxian
void GetNextPos(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if ( args.size() != 3 || 
		 args[0].m_Type != CScriptValue::SVT_NUMBER||
		 args[1].m_Type != CScriptValue::SVT_NUMBER||
		 args[2].m_Type != CScriptValue::SVT_NUMBER)
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}
	
	APointI pos;
	pos.x = args[0].GetInt()-1;
	pos.y = args[1].GetInt()-1;
	int route = args[2].GetInt();
	APointI nextPos;
	// get next pos
	CTowerDefencePF * inst = CTowerDefencePF::GetInstance();

	bool ret = inst->GetNextEscapePos(nextPos, pos, route);

	CScriptValue val;
	val.SetVal(ret);
	results.push_back(val);
	val.SetVal(nextPos.x+1);
	results.push_back(val);
	val.SetVal(nextPos.y+1);
	results.push_back(val);
}
IMPLEMENT_SCRIPT_API(GetNextPos);

void SetBlockPosition(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if( args.size() < 3 ||
		args[0].m_Type != CScriptValue::SVT_NUMBER ||
		args[1].m_Type != CScriptValue::SVT_NUMBER ||
		args[2].m_Type != CScriptValue::SVT_NUMBER )
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}
	
	abase::vector<APointI> vec;
	APointI pos;
	pos.x = args[0].GetInt();
	pos.y = args[1].GetInt();
	pos.x -=1;
	pos.y -=1;

	vec.push_back(pos);
	pos.x += 1;
	vec.push_back(pos);
	pos.y += 1;
	vec.push_back(pos);
	pos.x -= 1;
	vec.push_back(pos);
	
	int block = args[2].GetInt();
	abase::vector<CTowerDefencePF::TDPosEx> vecPos;

	if(block == CTowerDefencePF::PosBlock && args[3].m_Type == CScriptValue::SVT_ARRAY)
	{
		CTowerDefencePF::TDPosEx tdPos;
		int i;
		for(i = 0; i< (int)args[3].m_ArrKey.size(); ++i)
		{
			if(args[3].m_ArrVal[i].m_Type == CScriptValue::SVT_ARRAY)
			{
				CScriptValue& val = args[3].m_ArrVal[i];
				tdPos.pos.x = val.m_ArrVal[0].GetInt()-1;
				tdPos.pos.y = val.m_ArrVal[1].GetInt()-1;
				tdPos.iEscape = val.m_ArrVal[2].GetInt();
			}
			vecPos.push_back(tdPos);
		}
	}
	// it is not necessary to obtain all monsters' position. 
	CTowerDefencePF * inst = CTowerDefencePF::GetInstance();
	bool ret = inst->SetPosType(vec, static_cast<CTowerDefencePF::PosType>(block), block == CTowerDefencePF::PosBlock? &vecPos:NULL);
	CScriptValue val;
	val.SetVal(ret);

	results.push_back(val);	
}
IMPLEMENT_SCRIPT_API(SetBlockPosition);

void InitTDPath(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if( args.size() < 3 ||
		args[0].m_Type != CScriptValue::SVT_NUMBER ||
		args[1].m_Type != CScriptValue::SVT_NUMBER ||
		args[2].m_Type != CScriptValue::SVT_ARRAY)
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}
	
	int width = args[0].GetInt();
	int height = args[1].GetInt();
	abase::vector<CTowerDefencePF::EscapeInfo> vec; 
	APointI pos;
	int i,j,k;
	for( i = 0; i< (int)args[2].m_ArrKey.size(); ++i )
	{
		CTowerDefencePF::EscapeInfo esp; 
		CScriptValue& val = args[2].m_ArrVal[i];
		for(j=0; j< (int)val.m_ArrKey.size(); ++j)
		{
			CScriptValue& arr = val.m_ArrVal[0];
			for(k =0; k<(int)arr.m_ArrKey.size(); ++k)
			{
				CScriptValue& arr2 = arr.m_ArrVal[k];
				pos.x = arr2.m_ArrVal[0].GetInt()-1;
				pos.y = arr2.m_ArrVal[1].GetInt()-1;
				esp.entry.push_back(pos);
			}
			CScriptValue& arr1 = val.m_ArrVal[1];
			for(k=0; k<(int)arr1.m_ArrKey.size(); ++k)
			{
				CScriptValue& arr2 = arr1.m_ArrVal[k];
				pos.x = arr2.m_ArrVal[0].GetInt()-1;
				pos.y = arr2.m_ArrVal[1].GetInt()-1;
				esp.exit.push_back(pos);
			}
		}
		vec.push_back(esp);
	}
	abase::vector<APointI> block;
	if(args.size() == 4)
	{
		for( i = 0; i< (int)args[3].m_ArrKey.size(); ++i)
		{
			CScriptValue& blk = args[3].m_ArrVal[i];
			pos.x = blk.m_ArrVal[0].GetInt()-1;
			pos.y = blk.m_ArrVal[1].GetInt()-1;
			block.push_back(pos);
		}
	}

	CScriptValue res;
	bool ret1 = CTowerDefencePF::GetInstance()->Init(width, height, vec);
	bool ret2 = true;
	if(args.size() == 4)
		ret2 = CTowerDefencePF::GetInstance()->SetPosType(block,CTowerDefencePF::PosBlock,NULL);
	bool ret = ret1 && ret2;
	res.SetVal(ret);
	results.push_back(res);
}
IMPLEMENT_SCRIPT_API(InitTDPath);

void ClearTDMap(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	CTowerDefencePF * inst = CTowerDefencePF::GetInstance();
	bool ret = inst->ClearMap();
	CScriptValue res;
	res.SetVal(ret);
	results.push_back(res);
}
IMPLEMENT_SCRIPT_API(ClearTDMap);

void Play2DSound(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if( args.size() != 1 ||
		args[0].m_Type != CScriptValue::SVT_STRING)
	{
		AUI_ReportError(__LINE__,__FILE__);
		return;
	}
	AString strSound;
	args[0].RetrieveAString(strSound);
	GetGame()->GetA3DEngine()->GetAMSoundEngine()->GetAMSoundBufferMan()->Play2DAutoSound(strSound);
}
IMPLEMENT_SCRIPT_API(Play2DSound);

void CanAutoMoving(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	bool ret = g_AutoMoveAgent.IsReady();
	results.push_back(CScriptValue(ret));
}
//IMPLEMENT_SCRIPT_API(CanAutoMoving);

void LoadTDFile(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if( args.size() !=1 ||
		args[0].m_Type != CScriptValue::SVT_STRING)
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}
	AString file_name;
	AString name;
	args[0].RetrieveAString(name);
	file_name.Format("%s\\%d\\%s.txt", LOCAL_DIR, GetHostPlayer()->GetCharacterID(),name);
	
	AFile afile;
	if (!afile.Open(file_name, AFILE_OPENEXIST|AFILE_TEXT|AFILE_TEMPMEMORY))
	{
		return ;
	}
	int count = 0;

	char line[AFILE_LINEMAXLEN];
	DWORD  read_len;

	abase::vector<CScriptValue> vecvalue;
	CScriptValue ret, val;

	while (afile.ReadLine(line, AFILE_LINEMAXLEN, &read_len) && (count < 8))
	{	
		val.SetVal(atoi(line));
		vecvalue.push_back(val);
		++count;
	}

	afile.Close();
	ret.SetArray(vecvalue);
	results.push_back(ret);
}
IMPLEMENT_SCRIPT_API(LoadTDFile);

void SaveTDFile(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if( args.size() !=2 ||
		args[0].m_Type != CScriptValue::SVT_ARRAY ||
		args[1].m_Type != CScriptValue::SVT_STRING)
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}
	AString file_name;
	CreateDirectoryA(LOCAL_DIR, NULL);
	AString sub_dir;
	sub_dir.Format("%s\\%d", LOCAL_DIR, GetHostPlayer()->GetCharacterID());
	CreateDirectoryA(sub_dir, NULL);
	AString name;
	args[1].RetrieveAString(name);

	file_name.Format("%s\\%s.txt", sub_dir, name);

	AFile afile;
	if (!afile.Open(file_name, AFILE_CREATENEW|AFILE_TEXT|AFILE_TEMPMEMORY))
	{
		return ;
	}

	int count = 0;
	char val[10];
	while (count < 8 && count < (int)args[0].m_ArrKey.size())
	{
		itoa(args[0].m_ArrVal[count].GetInt(), val, 10);
		AString str(val);
		afile.WriteLine(str);
		count++;
	}
	afile.Close();
}
IMPLEMENT_SCRIPT_API(SaveTDFile);

extern void _notify_svr(TaskInterface* pTask, unsigned char uReason, unsigned short uTaskID);

void TaskReq(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if( args.size() !=1 ||
		args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}
	
	int idTask = args[0].GetInt();
	_notify_svr(GetHostPlayer()->GetTaskInterface(), TASK_CLT_NOTIFY_AUTO_DELV, idTask);
}
IMPLEMENT_SCRIPT_API(TaskReq);

void TaskFin(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if( args.size() !=1 ||
		args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}
	
	int idTask = args[0].GetInt();
	
	_notify_svr(GetHostPlayer()->GetTaskInterface(), TASK_CLT_NOTIFY_CHECK_FINISH,  idTask);
}
IMPLEMENT_SCRIPT_API(TaskFin);

void TaskGiveup(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if( args.size() !=1 ||
		args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}
	int idTask = args[0].GetInt();

	ActiveTaskList* pLst = (ActiveTaskList*)GetHostPlayer()->GetTaskInterface()->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(idTask);
	if (pEntry)
	{
		_notify_svr(GetHostPlayer()->GetTaskInterface(), TASK_CLT_NOTIFY_CHECK_GIVEUP,  idTask);
	}
}
IMPLEMENT_SCRIPT_API(TaskGiveup);

bool SearchPictureV(int **p, int type, int src_y, int src_x, int max_y, int &res_y)
{
	res_y = src_y;
	while( res_y > 0 && p[res_y][src_x] == 0 )
		res_y--;
	if( p[res_y][src_x] == type )
		return true;

	res_y = src_y;
	while( res_y < max_y-1 && p[res_y][src_x] == 0 )
		res_y++;

	if( p[res_y][src_x] == type )
		return true;

	return false;
}

bool SearchPictureH(int **p, int type, int src_y, int src_x, int max_x, int &res_x)
{
	res_x = src_x ;
	while( res_x > 0 && p[src_y][res_x] == 0 )
		res_x--;
	if( p[src_y][res_x] == type )
		return true;

	res_x = src_x ;
	while( res_x < max_x-1 && p[src_y][res_x] == 0 )
		res_x++;

	if( p[src_y][res_x] == type )
		return true;

	return false;
}

void PictureMatching(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if( args.size() != 3 ||
		args[0].m_Type != CScriptValue::SVT_NUMBER ||
		args[1].m_Type != CScriptValue::SVT_NUMBER ||
		args[2].m_Type != CScriptValue::SVT_ARRAY )
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}
	int n = args[0].GetInt();
	int m = args[1].GetInt();
	if( (int)args[2].m_ArrVal.size() < n )
		return;

	int i, j, k, kh, l,lh;
	int l1, l2,lh1;
	int type;
	int **p, **ph, **v, **vh, **h, **hh; //低位和高位
	p = new int*[n];
	ph = new int*[n];
	v = new int*[n];
	vh = new int*[n];
	h = new int*[n];
	hh = new int*[n];
	for(i = 0; i < n ; i++)
	{
		p[i] = new int[m];
		ph[i] = new int[m];
		v[i] = new int[m];
		vh[i] = new int[m];
		h[i] = new int[m];
		hh[i] = new int[m];
		memset(p[i], 0, sizeof(int) * m);
		memset(ph[i], 0, sizeof(int) * m);
		memset(v[i], 0, sizeof(int) * m);
		memset(vh[i], 0, sizeof(int) * m);
		memset(h[i], 0, sizeof(int) * m);
		memset(hh[i], 0, sizeof(int) * m);
	}


#define FREE_MEM_AND_RETURN \
	{\
		for(int i = 0; i < n; i++)\
		{\
			delete []p[i];\
			delete []ph[i];\
			delete []v[i];\
			delete []vh[i];\
			delete []h[i];\
			delete []hh[i];\
		}\
		delete []p;\
		delete []ph;\
		delete []v;\
		delete []vh;\
		delete []h;\
		delete []hh;\
		return;\
	}

	for(i = 0; i < n; i++)
		if( args[2].m_ArrVal[i].m_Type != CScriptValue::SVT_ARRAY ||
			(int)args[2].m_ArrVal[i].m_ArrVal.size() < m )
			FREE_MEM_AND_RETURN
		else
		{
			for(j = 0; j < m; j++)
				p[i][j] = args[2].m_ArrVal[i].m_ArrVal[j].GetInt();
		}

#define FREE_MEM_AND_RETURN \
	{\
		for(int i = 0; i < n; i++)\
		{\
			delete []p[i];\
			delete []ph[i];\
			delete []v[i];\
			delete []vh[i];\
			delete []h[i];\
			delete []hh[i];\
		}\
		delete []p;\
		delete []ph;\
		delete []v;\
		delete []vh;\
		delete []h;\
		delete []hh;\
		return;\
	}

	
	for(i = 0; i < n; i++)
	{
		j = 0;
		while(j < m)
		{
			while(j < m && p[i][j] != 0)
			{
				j++;
				if(p[i][j] == p[i][j-1])
				{
					results.push_back(CScriptValue(double(i)));
					results.push_back(CScriptValue(double(j-1)));
					results.push_back(CScriptValue(double(i)));
					results.push_back(CScriptValue(double(j)));
					FREE_MEM_AND_RETURN
				}
			}

			l = j - 1;
			if( j > 0 )
				l1 = p[i][j - 1];
			else
				l1 = 0;

			while(j < m && p[i][j] == 0)
				j++;
			if( j < m )
				l2 = p[i][j];
			else
				l2 = 0;

			if( l1 == l2 && l1 != 0 )
			{
				results.push_back(CScriptValue(double(i)));
				results.push_back(CScriptValue(double(l)));
				results.push_back(CScriptValue(double(i)));
				results.push_back(CScriptValue(double(j)));
				FREE_MEM_AND_RETURN
			}
			for(k = l + 1; k < j; k++)
			{
				if ( l1<31 && l2<31 )
					h[i][k] = ((1 << l1) | (1 << l2))>>1;
				else if ( l1>30 && l2<31 )
				{
					h[i][k] = (1<<l2)>>1;
					hh[i][k] = ((1<<(l1-30))>>1);
				}
				else if ( l1<31 && l2>30 )
				{
					h[i][k] = (1<<l1)>>1;
					hh[i][k] = (1<<(l2-30))>>1;
				}
				else if ( l1>30 && l2>30 )
					hh[i][k] = ((1<<(l1-30))|(1<<(l2-30)))>>1;
			}
		}
	}
	for(i = 0; i < m; i++)
	{
		j = 0;
		while(j < n)
		{
			while(j < n && p[j][i] != 0)
			{
				j++;
				if(p[j][i] == p[j-1][i])
				{
					results.push_back(CScriptValue(double(j-1)));
					results.push_back(CScriptValue(double(i)));
					results.push_back(CScriptValue(double(j)));
					results.push_back(CScriptValue(double(i)));
					FREE_MEM_AND_RETURN
				}
			}

			l = j - 1;
			if( j > 0 )
				l1 = p[j - 1][i];
			else
				l1 = 0;

			while(j < n && p[j][i] == 0)
				j++;
			if( j < n )
				l2 = p[j][i];
			else
				l2 = 0;

			if( l1 == l2 && l1 != 0 )
			{
				results.push_back(CScriptValue(double(l)));
				results.push_back(CScriptValue(double(i)));
				results.push_back(CScriptValue(double(j)));
				results.push_back(CScriptValue(double(i)));
				FREE_MEM_AND_RETURN
			}
			for(k = l + 1; k < j ; k++)
			{
				if ( l1<31 && l2<31 )
					v[k][i] = ((1 << l1) | (1 << l2))>>1;
				else if ( l1>30 && l2<31 )
				{
					v[k][i] = (1<<l2)>>1;
					vh[k][i] = (1<<(l1-30))>>1;
				}
				else if ( l1<31 && l2>30 )
				{
					v[k][i] = (1<<l1)>>1;
					vh[k][i] = (1<<(l2-30))>>1;
				}
				else if ( l1>30 && l2>30 )
					vh[k][i] = ((1<<(l1-30))|(1<<(l2-30)))>>1;

			}
		
		}
	}

	for(i = 0; i < n; i++)
		for(j = 0; j < m; j++)
			if( p[i][j] == 0 && ((v[i][j] & h[i][j])|(vh[i][j] & hh[i][j]) ) )
			{
				l = v[i][j] & h[i][j];
				lh = vh[i][j] & hh[i][j];
				
				if (l>0 && lh==0)
					type = 0;
				else if (lh>0 && l==0)
					type = 30;

				while(l > 0)
				{
					type++;
					l = l >> 1;
				}
				while(lh > 0)
				{
					type++;
					lh = lh >> 1;
				}
				SearchPictureV(p, type, i, j, n, l);
				results.push_back(CScriptValue(double(l)));
				results.push_back(CScriptValue(double(j)));
				results.push_back(CScriptValue(double(i)));
				results.push_back(CScriptValue(double(j)));
				SearchPictureH(p, type, i, j, m, l);
				results.push_back(CScriptValue(double(i)));
				results.push_back(CScriptValue(double(l)));
				FREE_MEM_AND_RETURN
			}
			
	for(i = 0; i < n; i++)
	{
		j = 0;
		while(j < m)
		{
			while(j < m && p[i][j] != 0)
				j++;
			l1 = 0;
			lh1 = 0;

			while(j < m && p[i][j] == 0)
			{
				if( (l1 & v[i][j])|(lh1 & vh[i][j]) )
				{
					l1 &= v[i][j];
					lh1 &= vh[i][j];

					if (l1 > 0 && lh1==0)
						type = 0;
					else if (lh1 > 0 && l1==0)
						type = 30;

					while(l1 > 0)
					{
						type++;
						l1 = l1 >> 1;
					}
					while(lh1 > 0)
					{
						type++;
						lh1 = lh1 >> 1;
					}
					SearchPictureV(p, type, i, j, n, l);
					results.push_back(CScriptValue(double(l)));
					results.push_back(CScriptValue(double(j)));
					results.push_back(CScriptValue(double(i)));
					results.push_back(CScriptValue(double(j)));

					k = 0;
					kh = 0;
					if ( type<31 )
						k = 1 << (type-1);
					else if ( type>30 )
						kh = 1<< (type-31);
					j--;

					while( j > 0 && (v[i][j] & k) == 0 && (vh[i][j] & kh) == 0 )
						j--;
					SearchPictureV(p, type, i, j, n, l);
					results.push_back(CScriptValue(double(i)));
					results.push_back(CScriptValue(double(j)));
					results.push_back(CScriptValue(double(l)));
					results.push_back(CScriptValue(double(j)));

					FREE_MEM_AND_RETURN
				}
				else
				{	
					l1 |= v[i][j];
					lh1 |= vh[i][j];
				}
				j++;
			}
		}
	}
	for(i = 0; i < m; i++)
	{
		j = 0;
		while(j < n)
		{
			while(j < n && p[j][i] != 0)
				j++;
			l1 = 0;
			lh1 = 0;

			while(j < n && p[j][i] == 0)
			{
				if( (l1 & h[j][i])|(lh1 & hh[j][i]) )
				{
					l1 &= h[j][i];
					lh1 &= hh[j][i];

					if (l1>0 && lh1==0)
						type = 0;
					else if (lh1>0 && l1==0)
						type = 30;

					while(l1 > 0)
					{
						type++;
						l1 = l1 >> 1;
					}
					while(lh1 > 0)
					{
						type++;
						lh1 = lh1 >> 1;
					}

					SearchPictureH(p, type, j, i, m, l);
					results.push_back(CScriptValue(double(j)));
					results.push_back(CScriptValue(double(l)));
					results.push_back(CScriptValue(double(j)));
					results.push_back(CScriptValue(double(i)));

					k = 0;
					kh = 0;
					if ( type<31 )
						k = 1 << (type-1);
					else if ( type>30 )
						kh = 1<< (type-31);
						j--;
					
					while( j > 0 && (h[j][i] & k) == 0 && (hh[j][i] & kh) == 0 )
						j--;
					SearchPictureH(p, type, j, i, m, l);
					results.push_back(CScriptValue(double(j)));
					results.push_back(CScriptValue(double(i)));
					results.push_back(CScriptValue(double(j)));
					results.push_back(CScriptValue(double(l)));

					FREE_MEM_AND_RETURN
				}
				else
				{
					l1 |= h[j][i];
					lh1 |= hh[j][i];
				}
				j++;
			}
		}
	}
}
IMPLEMENT_SCRIPT_API(PictureMatching);

void GetQPCount(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	LARGE_INTEGER Count;
	BOOL ret;
	ret = QueryPerformanceCounter( &Count );
	if(ret==TRUE)
		results.push_back(CScriptValue(double(Count.QuadPart)));

}
IMPLEMENT_SCRIPT_API(GetQPCount);

void GetQPFrequency(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	LARGE_INTEGER Frequency;
	BOOL ret;
	ret = QueryPerformanceFrequency( &Frequency );
	if (ret == TRUE)
		results.push_back(CScriptValue(double(Frequency.QuadPart)));

}
IMPLEMENT_SCRIPT_API(GetQPFrequency);


void InitAStarMap(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if( args.size() < 3 ||
		args[0].m_Type != CScriptValue::SVT_NUMBER ||
		args[1].m_Type != CScriptValue::SVT_NUMBER ||
		args[2].m_Type != CScriptValue::SVT_ARRAY)
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}
	
	int width = args[0].GetInt();
	int height = args[1].GetInt();

	abase::vector<APointI> block;
	APointI pos;
	for( int i = 0; i< (int)args[2].m_ArrKey.size(); ++i)
	{
		CScriptValue& blk = args[2].m_ArrVal[i];
		pos.x = blk.m_ArrVal[0].GetInt()-1;
		pos.y = blk.m_ArrVal[1].GetInt()-1;
		block.push_back(pos);
	}
	
	CScriptValue res;
	bool ret = CAStarPF::GetInstance()->Init(width, height, block);
	res.SetVal(ret);
	results.push_back(res);
}
IMPLEMENT_SCRIPT_API(InitAStarMap);

void ClearAStarMap(vector<CScriptValue>& args, vector<CScriptValue>& results)
{	
	CScriptValue res;
	bool ret = CAStarPF::GetInstance()->ClearMap();
	res.SetVal(ret);
	results.push_back(res);
}
IMPLEMENT_SCRIPT_API(ClearAStarMap);

void SetAStarBlockPos(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if( args.size() < 1 ||
		args[0].m_Type != CScriptValue::SVT_ARRAY )
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}

	abase::vector<APointI> block;
	APointI pos;
	for( int i = 0; i< (int)args[0].m_ArrKey.size(); ++i)
	{
		CScriptValue& blk = args[0].m_ArrVal[i];
		pos.x = blk.m_ArrVal[0].GetInt()-1;
		pos.y = blk.m_ArrVal[1].GetInt()-1;
		block.push_back(pos);
	}
	
	CScriptValue res;
	bool ret = CAStarPF::GetInstance()->SetBlockPos(block);
	res.SetVal(ret);
	results.push_back(res);
}
IMPLEMENT_SCRIPT_API(SetAStarBlockPos);


void SetAStarClearPos(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if( args.size() < 1 ||
		args[0].m_Type != CScriptValue::SVT_ARRAY )
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}

	abase::vector<APointI> block;
	APointI pos;
	for( int i = 0; i< (int)args[0].m_ArrKey.size(); ++i)
	{
		CScriptValue& blk = args[0].m_ArrVal[i];
		pos.x = blk.m_ArrVal[0].GetInt()-1;
		pos.y = blk.m_ArrVal[1].GetInt()-1;
		block.push_back(pos);
	}
	
	CScriptValue res;
	bool ret = CAStarPF::GetInstance()->SetClearPos(block);
	res.SetVal(ret);
	results.push_back(res);
}
IMPLEMENT_SCRIPT_API(SetAStarClearPos);

void AStarFindPath(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if( args.size() < 4 ||
		args[0].m_Type != CScriptValue::SVT_NUMBER ||
		args[1].m_Type != CScriptValue::SVT_NUMBER ||
		args[2].m_Type != CScriptValue::SVT_NUMBER ||
		args[3].m_Type != CScriptValue::SVT_NUMBER  )
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}

	APointI start, end;
	start.x = args[0].GetInt()-1;
	start.y = args[1].GetInt()-1;

	end.x = args[2].GetInt()-1;
	end.y = args[3].GetInt()-1;

	abase::vector<APointI> vecPath;

	abase::vector<CScriptValue> path_items;
	bool ret = CAStarPF::GetInstance()->FindPath(start, end, vecPath);
	if(ret)
	{
		int i = vecPath.size()-1;
		for(; i>=0; --i)
		{
			abase::vector<CScriptValue> keysItem, valsItem;
			SetKVPaires(keysItem, valsItem, "x", vecPath[i].x+1);
			SetKVPaires(keysItem, valsItem, "y", vecPath[i].y+1);
			CScriptValue item;
			item.SetArray(valsItem, keysItem);
			path_items.push_back(item);
		}
	}
	CScriptValue res;
	res.SetArray(path_items);
	results.push_back(res);
}
IMPLEMENT_SCRIPT_API(AStarFindPath);

void SecretarySetURL(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if( args.size() < 1 ||
		args[0].m_Type != CScriptValue::SVT_STRING )
	{
		AUI_ReportError(__LINE__, __FILE__);
		return;
	}
	AString url;
	AString strFullPath;
	args[0].RetrieveAString(url);
	af_GetFullPath(strFullPath, url);
	CECSecretary* pSecretary = g_pGame->GetGameRun()->GetSecretary();
	pSecretary->SetURL(strFullPath, GetGameUIMan()->GetStringFromTable(9124));

}
IMPLEMENT_SCRIPT_API(SecretarySetURL);

void LuaGetTickCount(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	CScriptValue res;
	DWORD dwTime = GetTickCount();
	res.SetVal(double(dwTime));
	results.push_back(res);
}
IMPLEMENT_SCRIPT_API(LuaGetTickCount);

void UseItem(vector<CScriptValue>& args, vector<CScriptValue>& results)
{
	if (args.size() != 1 || args[0].m_Type != CScriptValue::SVT_NUMBER)
	{
		//ReportError(NULL, "UseSkill, args error");
		return;
	}
	CDlgTalisman *pDlg = (CDlgTalisman *)GetGameUIMan()->GetDialog("Win_Talisman");
	if (pDlg)
	{
		int index =  args[0].GetInt();
		pDlg->UseTalismanItem(index + 9);		// 界面需要的序号10-12
	}
}
//IMPLEMENT_SCRIPT_API(UseItem);

void InitAIApi(CLuaState * pState)
{
// 	REGISTER_SCRIPT_API(GameAI, GetHostProp);
// 	REGISTER_SCRIPT_API(GameAI, GetTargetProp);
// 
// 	REGISTER_SCRIPT_API(GameAI, HealHp);
// 	REGISTER_SCRIPT_API(GameAI, HealMp);
// 	REGISTER_SCRIPT_API(GameAI, HealStamina);
// 
// 	REGISTER_SCRIPT_API(GameAI, SelectMonster);
// 	REGISTER_SCRIPT_API(GameAI, NormalAttack);
// 	REGISTER_SCRIPT_API(GameAI, CanAttack);
// 
// 	//pickup
// 	REGISTER_SCRIPT_API(GameAI, PickUp);
// 	REGISTER_SCRIPT_API(GameAI, IsPreparingPickingUp);
// 	REGISTER_SCRIPT_API(GameAI, IsPickingUp);
// 	REGISTER_SCRIPT_API(GameAI, CheckDrops);
// 	//skill
// 	REGISTER_SCRIPT_API(GameAI, IsSkillAvailable);
// 	REGISTER_SCRIPT_API(GameAI, CheckSkillCastCondition);
// 	REGISTER_SCRIPT_API(GameAI, UseSkill);
// 	REGISTER_SCRIPT_API(GameAI, IsPreparingSkill);
// 	REGISTER_SCRIPT_API(GameAI, IsUsingSkill);
// 	REGISTER_SCRIPT_API(GameAI, IsSkillCooling);
// 
// 	//automove
// 	REGISTER_SCRIPT_API(GameAI, StartAutoMove);
// 	REGISTER_SCRIPT_API(GameAI, IsAutoMoving);
// 	REGISTER_SCRIPT_API(GameAI, GetHostPos);
// 	REGISTER_SCRIPT_API(GameAI, CanAutoMoving);
// 	
// 	REGISTER_SCRIPT_API(GameAI, Report2Svr);
	REGISTER_SCRIPT_API(GameAI, GetCursorPosition);
	
	// tower defense path searching
	REGISTER_SCRIPT_API(GameAI, GetNextPos);
	REGISTER_SCRIPT_API(GameAI, SetBlockPosition);
	REGISTER_SCRIPT_API(GameAI, InitTDPath);
	REGISTER_SCRIPT_API(GameAI, ClearTDMap);

	REGISTER_SCRIPT_API(GameAI, Play2DSound);
	REGISTER_SCRIPT_API(GameAI, SaveTDFile);
	REGISTER_SCRIPT_API(GameAI, LoadTDFile);

	REGISTER_SCRIPT_API(GameAI, PictureMatching);


	REGISTER_SCRIPT_API(GameAI, TaskReq);
	REGISTER_SCRIPT_API(GameAI, TaskFin);
	REGISTER_SCRIPT_API(GameAI, TaskGiveup);

	REGISTER_SCRIPT_API(GameAI, GetQPCount);
	REGISTER_SCRIPT_API(GameAI, GetQPFrequency);

	// a star find path, for tiny game

	REGISTER_SCRIPT_API(GameAI, InitAStarMap);
	REGISTER_SCRIPT_API(GameAI, ClearAStarMap);
	REGISTER_SCRIPT_API(GameAI, SetAStarBlockPos);
	REGISTER_SCRIPT_API(GameAI, SetAStarClearPos);
	REGISTER_SCRIPT_API(GameAI, AStarFindPath);

	// secretary system
	REGISTER_SCRIPT_API(GameAI, SecretarySetURL);
	REGISTER_SCRIPT_API(GameAI, LuaGetTickCount);

//	REGISTER_SCRIPT_API(GameAI, UseItem);

	InitLuaAchievementApi();
	
	pState->RegisterLibApi("GameAI");
}



