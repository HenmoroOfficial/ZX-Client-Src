#include "policy.h"
#include <memory.h>

#define CONDITION_TREE_NULL 0
#define CONDITION_LEFT_SUB  1
#define CONDITION_RIGHT_SUB 2
#define CONDITION_LEAF      3
#define CONDITION_NODE_END  4

//----------------------------------------------------------------------------------------
//CTriggerData
//----------------------------------------------------------------------------------------

void CTriggerData::AddOperaion(unsigned int iType, void *pParam, _s_target *pTarget)
{
	_s_operation* pNew = new _s_operation;
	pNew->iType = iType;
	pNew->pParam = pParam;
	pNew->mTarget.iType = pTarget->iType;
	pNew->mTarget.pParam = pTarget->pParam;
	listOperation.push_back(pNew);
}

void CTriggerData::AddOperaion(_s_operation*pOperation)
{
	if(pOperation==0) return;
	listOperation.push_back(pOperation);
}

int	CTriggerData::GetOperaion(unsigned int idx, unsigned int &iType, void **ppData, _s_target& target)
{
	if(idx> listOperation.size()) return -1;
	iType = listOperation[idx]->iType;
	*ppData = listOperation[idx]->pParam;
	target = listOperation[idx]->mTarget;
	return iType;
}

void CTriggerData::SetOperation(unsigned int idx, unsigned int iType, void *pData, _s_target *pTarget)
{
	if(idx > listOperation.size()) return;
	listOperation[idx]->iType = iType;
	listOperation[idx]->pParam = pData;
	listOperation[idx]->mTarget = *pTarget;
}

void CTriggerData::DelOperation(unsigned int idx)
{
	if(idx > listOperation.size()) return;
	if(listOperation[idx]->pParam) free(listOperation[idx]->pParam);
	listOperation.erase(&listOperation[idx]);
}

CTriggerData::_s_tree_item *CTriggerData::GetConditonRoot()
{
	return rootConditon;
}

bool CTriggerData::SaveConditonTree(FILE *pFile, _s_tree_item *pNode)
{
	if(pNode==0) return true;
	int dat_size = 0;
	int flag = 0;
	fwrite(&pNode->mConditon.iType,sizeof(int),1,pFile);
	if(pNode->mConditon.pParam==0)
		fwrite(&dat_size,sizeof(int),1,pFile);
	else
	{
		switch(pNode->mConditon.iType) 
		{
		case c_time_come:
			dat_size = sizeof(C_TIME_COME);
			break;
		case c_hp_less:
			dat_size = sizeof(C_HP_LESS);
			break;
		case c_random:
			dat_size = sizeof(C_RANDOM);
			break;
		case c_path_end_point:
			dat_size = sizeof(C_PATH_END_POINT);
			break;
		case c_enmity_reach:
			dat_size = sizeof(C_ENMITY_REACH);
			break;
		case c_distance_reach:
			dat_size = sizeof(C_DISTANCE_REACH);
			break;
		case c_attack_by_skill:
			dat_size = sizeof(C_ATTACK_BY_SKILL);
			break;
		case c_var:
			dat_size = sizeof(C_VAR);
			break;
		case c_constant:
			dat_size = sizeof(C_CONSTANT);
			break;
		case c_rank_level:
			dat_size = sizeof(C_RANK_LEVEL);
			break;
		}
		fwrite(&dat_size,sizeof(int),1,pFile);
		fwrite(pNode->mConditon.pParam,dat_size,1,pFile);
	}

	if(pNode->pLeft == 0 && pNode->pRight == 0)
	{
		flag = CONDITION_LEAF;
		fwrite(&flag,sizeof(int),1,pFile);
		return true;
	}

	if(pNode->pLeft) 
	{
		flag = CONDITION_LEFT_SUB;
		fwrite(&flag,sizeof(int),1,pFile);
		SaveConditonTree(pFile,pNode->pLeft);
	}

	if(pNode->pRight)
	{
		flag = CONDITION_RIGHT_SUB;
		fwrite(&flag,sizeof(int),1,pFile);
		SaveConditonTree(pFile,pNode->pRight);
	}
	
	flag = CONDITION_NODE_END;
	fwrite(&flag,sizeof(int),1,pFile);

	return true;
}

bool CTriggerData::ReadConditonTree(FILE *pFile, _s_tree_item *pNode)
{
	if(pNode==0) return true;
	int dat_size = 0;
	int flag = 0;
	fread(&pNode->mConditon.iType,sizeof(int),1,pFile);
	fread(&dat_size,sizeof(int),1,pFile);
	if(dat_size!=0) 
	{
		pNode->mConditon.pParam = malloc(dat_size); 
		fread(pNode->mConditon.pParam,dat_size,1,pFile);
	}else pNode->mConditon.pParam= 0;
	
	while(1)
	{
		fread(&flag,sizeof(int),1,pFile);
		if(flag==CONDITION_LEAF)
		{
			pNode->pLeft = 0;
			pNode->pRight = 0;
			break;
		}else if(flag==CONDITION_LEFT_SUB)
		{
			pNode->pLeft = new _s_tree_item;
			ReadConditonTree(pFile, pNode->pLeft);
		}else if(flag==CONDITION_RIGHT_SUB)
		{
			pNode->pRight = new _s_tree_item;
			ReadConditonTree(pFile,pNode->pRight);
		}else if(flag==CONDITION_NODE_END) break;
	}
	return true;	
}


bool CTriggerData::Save(FILE *pFile)
{
	unsigned int dwVersion = F_TRIGGER_VERSION;
	fwrite(&dwVersion,sizeof(unsigned int),1,pFile);
	fwrite(&uID,sizeof(unsigned int),1,pFile);
	fwrite(&bActive,sizeof(bool),1,pFile);
	fwrite(&bRun,sizeof(bool),1,pFile);
	fwrite(&runCondition,sizeof(char),1,pFile);
	fwrite(szName,sizeof(char)*128,1,pFile);
	
	int n;
	//Writting conditon data(tree)
	
	SaveConditonTree(pFile,rootConditon);

	//Writting operation data
	n = listOperation.size();
	fwrite(&n,sizeof(int),1,pFile);
	for(int i = 0; i < n; ++i)
	{
		_s_operation *pTemp = (_s_operation* )listOperation[i];
		fwrite(&pTemp->iType,sizeof(int),1,pFile);
		switch(pTemp->iType) 
		{
		case o_attact:
			fwrite(pTemp->pParam,sizeof(O_ATTACK_TYPE),1,pFile);
			break;
		case o_use_skill:
			fwrite(pTemp->pParam,sizeof(O_USE_SKILL),1,pFile);
			break;
		case o_use_range_skill:
			fwrite(pTemp->pParam,sizeof(O_USE_RANGE_SKILL),1,pFile);
			break;
		case o_talk:
			fwrite(&((O_TALK_TEXT*)pTemp->pParam)->uSize,sizeof(unsigned int),1,pFile);
			fwrite(((O_TALK_TEXT*)pTemp->pParam)->szData,((O_TALK_TEXT*)pTemp->pParam)->uSize,1,pFile);
			break;
		case o_reset_hate_list:
			break;
		case o_run_trigger:
			fwrite(pTemp->pParam,sizeof(O_RUN_TRIGGER),1,pFile);
			break;
		case o_stop_trigger:
			fwrite(pTemp->pParam,sizeof(O_STOP_TRIGGER),1,pFile);
			break;
		case o_active_trigger:
			fwrite(pTemp->pParam,sizeof(O_ACTIVE_TRIGGER),1,pFile);
			break;
		case o_create_timer:
			fwrite(pTemp->pParam,sizeof(O_CREATE_TIMER),1,pFile);
			break;
		case o_kill_timer:
			fwrite(pTemp->pParam,sizeof(O_KILL_TIMER),1,pFile);
			break;
		case o_active_controller:
			fwrite(pTemp->pParam,sizeof(O_ACTIVE_CONTROLLER),1,pFile);
			break;
		case o_summon:
			fwrite(pTemp->pParam,sizeof(O_SUMMON),1,pFile);
			break;
		case o_trigger_task:
			fwrite(pTemp->pParam,sizeof(O_TRIGGER_TASK),1,pFile);
			break;
		case o_change_path:
			fwrite(pTemp->pParam,sizeof(O_CHANGE_PATH),1,pFile);
			break;
		case o_sneer_monster:
			fwrite(pTemp->pParam,sizeof(O_SNEER_MONSTER),1,pFile);
			break;
		case o_set_global:
			fwrite(pTemp->pParam,sizeof(O_SET_GLOBAL),1,pFile);
			break;
		case o_revise_global:
			fwrite(pTemp->pParam,sizeof(O_REVISE_GLOBAL),1,pFile);
			break;
		case o_assign_global:
			fwrite(pTemp->pParam,sizeof(O_ASSIGN_GLOBAL),1,pFile);
			break;
		case o_summon_mineral:
			fwrite(pTemp->pParam,sizeof(O_SUMMON_MINERAL),1,pFile);
			break;
		case o_drop_item:
			fwrite(pTemp->pParam,sizeof(O_DROP_ITEM),1,pFile);
			break;
		case o_change_hate:
			fwrite(pTemp->pParam,sizeof(O_CHANGE_HATE),1,pFile);
			break;
		case o_start_event:
			fwrite(pTemp->pParam,sizeof(O_START_EVENT),1,pFile);
			break;
		case o_stop_event:
			fwrite(pTemp->pParam,sizeof(O_STOP_EVENT),1,pFile);
			break;
		}
		
		fwrite(&pTemp->mTarget.iType,sizeof(int),1,pFile);
		switch(pTemp->mTarget.iType) 
		{
		case t_occupation_list:
			fwrite(pTemp->mTarget.pParam,sizeof(T_OCCUPATION),1,pFile);
			break;
		}
		
	}

	return true;
}

bool CTriggerData::Load(FILE *pFile)
{
	unsigned int dwVersion;
	fread(&dwVersion,sizeof(unsigned int),1,pFile);
	if(dwVersion==0)
	{
		
		fread(&uID,sizeof(unsigned int),1,pFile);
		fread(&bActive,sizeof(bool),1,pFile);
		fread(&bRun,sizeof(bool),1,pFile);
		fread(&runCondition,sizeof(char),1,pFile);
		fread(szName,sizeof(char)*128,1,pFile);
		int n = 0;
		
		//Reading conditon data
		rootConditon = new _s_tree_item;
		ReadConditonTree(pFile,rootConditon);
		
		//Reading operation data
		fread(&n,sizeof(int),1,pFile);
		for(int i = 0; i < n; ++i)
		{
			_s_operation *pTemp = new _s_operation;
			fread(&pTemp->iType,sizeof(int),1,pFile);
			switch(pTemp->iType) 
			{
			case o_attact:
				pTemp->pParam = malloc(sizeof(O_ATTACK_TYPE));
				fread(pTemp->pParam,sizeof(O_ATTACK_TYPE),1,pFile);
				break;
			case o_use_skill:
				pTemp->pParam = malloc(sizeof(O_USE_SKILL));
				fread(pTemp->pParam,sizeof(O_USE_SKILL),1,pFile);
				break;
			case o_talk:
				pTemp->pParam = malloc(sizeof(O_TALK_TEXT));
				fread(&((O_TALK_TEXT*)pTemp->pParam)->uSize,sizeof(unsigned int),1,pFile);
				((O_TALK_TEXT*)pTemp->pParam)->szData = new unsigned short[((O_TALK_TEXT*)pTemp->pParam)->uSize/2];
				fread(((O_TALK_TEXT*)pTemp->pParam)->szData,((O_TALK_TEXT*)pTemp->pParam)->uSize,1,pFile);
				break;
			case o_run_trigger:
				pTemp->pParam = malloc(sizeof(O_RUN_TRIGGER));
				fread(pTemp->pParam,sizeof(O_RUN_TRIGGER),1,pFile);
				break;
			case o_stop_trigger:
				pTemp->pParam = malloc(sizeof(O_STOP_TRIGGER));
				fread(pTemp->pParam,sizeof(O_STOP_TRIGGER),1,pFile);
				break;
			case o_active_trigger:
				pTemp->pParam = malloc(sizeof(O_ACTIVE_TRIGGER));
				fread(pTemp->pParam,sizeof(O_ACTIVE_TRIGGER),1,pFile);
				break;
			case o_create_timer:
				pTemp->pParam = malloc(sizeof(O_CREATE_TIMER));
				fread(pTemp->pParam,sizeof(O_CREATE_TIMER),1,pFile);
				break;
			case o_kill_timer:
				pTemp->pParam = malloc(sizeof(O_KILL_TIMER));
				fread(pTemp->pParam,sizeof(O_KILL_TIMER),1,pFile);
				break;
			case o_active_controller:
				pTemp->pParam = malloc(sizeof(O_ACTIVE_CONTROLLER));
				fread(&((O_ACTIVE_CONTROLLER*)pTemp->pParam)->uID,sizeof(unsigned int),1,pFile);
				((O_ACTIVE_CONTROLLER*)pTemp->pParam)->bStop = false;
				break;
			case o_summon:
				pTemp->pParam = malloc(sizeof(O_SUMMON));
				fread(pTemp->pParam,sizeof(O_SUMMON),1,pFile);
				break;
			case o_use_range_skill:
				pTemp->pParam = malloc(sizeof(O_USE_RANGE_SKILL));
				fread(pTemp->pParam,sizeof(O_USE_RANGE_SKILL),1,pFile);
				break;
			default:
				pTemp->pParam = 0;
			}
			
			fread(&pTemp->mTarget.iType,sizeof(int),1,pFile);
			switch(pTemp->mTarget.iType) 
			{
			case t_occupation_list:
				pTemp->mTarget.pParam = malloc(sizeof(T_OCCUPATION));
				fread(pTemp->mTarget.pParam,sizeof(T_OCCUPATION),1,pFile);
				break;
			default:
				pTemp->mTarget.pParam = NULL;
			}
			
			listOperation.push_back(pTemp);
		}
	}else if(dwVersion>=1)
	{
		fread(&uID,sizeof(unsigned int),1,pFile);
		fread(&bActive,sizeof(bool),1,pFile);
		fread(&bRun,sizeof(bool),1,pFile);
		fread(&runCondition,sizeof(char),1,pFile);
		fread(szName,sizeof(char)*128,1,pFile);
		int n = 0;
		
		//Reading conditon data
		rootConditon = new _s_tree_item;
		ReadConditonTree(pFile,rootConditon);
		
		//Reading operation data
		fread(&n,sizeof(int),1,pFile);
		for(int i = 0; i < n; ++i)
		{
			_s_operation *pTemp = new _s_operation;
			fread(&pTemp->iType,sizeof(int),1,pFile);
			switch(pTemp->iType) 
			{
			case o_attact:
				pTemp->pParam = malloc(sizeof(O_ATTACK_TYPE));
				fread(pTemp->pParam,sizeof(O_ATTACK_TYPE),1,pFile);
				break;
			case o_use_skill:
				pTemp->pParam = malloc(sizeof(O_USE_SKILL));
				fread(pTemp->pParam,sizeof(O_USE_SKILL),1,pFile);
				break;
			case o_talk:
				pTemp->pParam = malloc(sizeof(O_TALK_TEXT));
				fread(&((O_TALK_TEXT*)pTemp->pParam)->uSize,sizeof(unsigned int),1,pFile);
				((O_TALK_TEXT*)pTemp->pParam)->szData = new unsigned short[((O_TALK_TEXT*)pTemp->pParam)->uSize/2];
				fread(((O_TALK_TEXT*)pTemp->pParam)->szData,((O_TALK_TEXT*)pTemp->pParam)->uSize,1,pFile);
				break;
			case o_run_trigger:
				pTemp->pParam = malloc(sizeof(O_RUN_TRIGGER));
				fread(pTemp->pParam,sizeof(O_RUN_TRIGGER),1,pFile);
				break;
			case o_stop_trigger:
				pTemp->pParam = malloc(sizeof(O_STOP_TRIGGER));
				fread(pTemp->pParam,sizeof(O_STOP_TRIGGER),1,pFile);
				break;
			case o_active_trigger:
				pTemp->pParam = malloc(sizeof(O_ACTIVE_TRIGGER));
				fread(pTemp->pParam,sizeof(O_ACTIVE_TRIGGER),1,pFile);
				break;
			case o_create_timer:
				pTemp->pParam = malloc(sizeof(O_CREATE_TIMER));
				fread(pTemp->pParam,sizeof(O_CREATE_TIMER),1,pFile);
				break;
			case o_kill_timer:
				pTemp->pParam = malloc(sizeof(O_KILL_TIMER));
				fread(pTemp->pParam,sizeof(O_KILL_TIMER),1,pFile);
				break;
			case o_active_controller:
				pTemp->pParam = malloc(sizeof(O_ACTIVE_CONTROLLER));
				fread(pTemp->pParam,sizeof(O_ACTIVE_CONTROLLER),1,pFile);
				break;
			case o_summon:
				pTemp->pParam = malloc(sizeof(O_SUMMON));
				fread(pTemp->pParam,sizeof(O_SUMMON),1,pFile);
				break;
			case o_trigger_task:
				pTemp->pParam = malloc(sizeof(O_TRIGGER_TASK));
				fread(pTemp->pParam,sizeof(O_TRIGGER_TASK),1,pFile);
				break;
			case o_change_path:
				pTemp->pParam = malloc(sizeof(O_CHANGE_PATH));
				if(dwVersion < 9) 
				{
					fread(pTemp->pParam,sizeof(O_CHANGE_PATH_VERSION8),1,pFile);
					((O_CHANGE_PATH*)(pTemp->pParam))->iType = 0;
				}
				else fread(pTemp->pParam,sizeof(O_CHANGE_PATH),1,pFile);
				break;
			case o_sneer_monster:
				pTemp->pParam = malloc(sizeof(O_SNEER_MONSTER));
				fread(pTemp->pParam,sizeof(O_SNEER_MONSTER),1,pFile);
				break;
			case o_use_range_skill:
				pTemp->pParam = malloc(sizeof(O_USE_RANGE_SKILL));
				fread(pTemp->pParam,sizeof(O_USE_RANGE_SKILL),1,pFile);
				break;
			case o_set_global:
				pTemp->pParam = malloc(sizeof(O_SET_GLOBAL));
				fread(pTemp->pParam,sizeof(O_SET_GLOBAL),1,pFile);
				break;
			case o_revise_global:
				pTemp->pParam = malloc(sizeof(O_REVISE_GLOBAL));
				fread(pTemp->pParam,sizeof(O_REVISE_GLOBAL),1,pFile);
				break;
			case o_assign_global:
				pTemp->pParam = malloc(sizeof(O_ASSIGN_GLOBAL));
				fread(pTemp->pParam,sizeof(O_ASSIGN_GLOBAL),1,pFile);
				break;
			case o_summon_mineral:
				pTemp->pParam = malloc(sizeof(O_SUMMON_MINERAL));
				fread(pTemp->pParam,sizeof(O_SUMMON_MINERAL),1,pFile);
				break;
			case o_drop_item:
				pTemp->pParam = malloc(sizeof(O_DROP_ITEM));
				fread(pTemp->pParam, sizeof(O_DROP_ITEM),1,pFile);
				break;
			case o_change_hate:
				pTemp->pParam = malloc(sizeof(O_CHANGE_HATE));
				fread(pTemp->pParam, sizeof(O_CHANGE_HATE),1,pFile);
				break;
			case o_start_event:
				pTemp->pParam = malloc(sizeof(O_START_EVENT));
				fread(pTemp->pParam, sizeof(O_START_EVENT),1,pFile);
				break;
			case o_stop_event:
				pTemp->pParam = malloc(sizeof(O_STOP_EVENT));
				fread(pTemp->pParam, sizeof(O_STOP_EVENT),1,pFile);
				break;
			default:
				pTemp->pParam = 0;
			}
			
			fread(&pTemp->mTarget.iType,sizeof(int),1,pFile);
			switch(pTemp->mTarget.iType) 
			{
			case t_occupation_list:
				pTemp->mTarget.pParam = malloc(sizeof(T_OCCUPATION));
				fread(pTemp->mTarget.pParam,sizeof(T_OCCUPATION),1,pFile);
				break;
			default:
				pTemp->mTarget.pParam = NULL;
			}
			
			listOperation.push_back(pTemp);
		}
	}
	return true;
}

void CTriggerData::ReleaseConditionTree()
{
	if(rootConditon)
		delete rootConditon;
	rootConditon = 0;
}

void CTriggerData::Release()
{
	ReleaseConditionTree();

	int n = listOperation.size();
	for(int i = 0; i < n; ++i )
	{
		if(listOperation[i]->pParam) 
		{
			if(listOperation[i]->iType == CTriggerData::o_talk)
			{
				if(((O_TALK_TEXT *)listOperation[i]->pParam)->szData)
				delete ((O_TALK_TEXT *)listOperation[i]->pParam)->szData;
			}
			free(listOperation[i]->pParam);
		}
		if(listOperation[i]->mTarget.pParam) free(listOperation[i]->mTarget.pParam);	
		
		delete listOperation[i];
	}
}

CTriggerData *CTriggerData::CopyObject()
{
	CTriggerData *pNewObject = new CTriggerData;
	if(pNewObject==0) return 0;

	strcpy(pNewObject->szName,szName); 
	pNewObject->bActive = bActive;
	pNewObject->bRun = bRun;
	pNewObject->runCondition = runCondition;
	pNewObject->uID = uID;
	pNewObject->rootConditon = CopyConditonTree(rootConditon);

	for( int i = 0; i < listOperation.size(); ++i)
	{
		_s_operation* pNewOperation = CopyOperation(listOperation[i]);
		pNewObject->AddOperaion(pNewOperation);
	}

	return pNewObject;
}

CTriggerData::_s_tree_item* CTriggerData::CopyConditonTree( CTriggerData::_s_tree_item* pRoot)
{
	if(pRoot==0) return 0;
	CTriggerData::_s_tree_item *pNew = new CTriggerData::_s_tree_item;
	if(pNew==0) return 0;
	
	pNew->mConditon.iType = pRoot->mConditon.iType;
	pNew->mConditon.pParam = 0;
	pNew->pLeft = 0;
	pNew->pRight = 0;
	if(pRoot->mConditon.pParam!=0)
	{
		switch(pRoot->mConditon.iType) 
		{
		case CTriggerData::c_random:
			pNew->mConditon.pParam = malloc(sizeof(C_RANDOM));
			memcpy(pNew->mConditon.pParam,pRoot->mConditon.pParam,sizeof(C_RANDOM));
			break;
		case CTriggerData::c_hp_less:
			pNew->mConditon.pParam = malloc(sizeof(C_HP_LESS));
			memcpy(pNew->mConditon.pParam,pRoot->mConditon.pParam,sizeof(C_HP_LESS));
			break;
		case CTriggerData::c_time_come:
			pNew->mConditon.pParam = malloc(sizeof(C_TIME_COME));
			memcpy(pNew->mConditon.pParam,pRoot->mConditon.pParam,sizeof(C_TIME_COME));
			break;
		case CTriggerData::c_path_end_point:
			pNew->mConditon.pParam = malloc(sizeof(C_PATH_END_POINT));
			memcpy(pNew->mConditon.pParam,pRoot->mConditon.pParam,sizeof(C_PATH_END_POINT));
			break;
		case CTriggerData::c_enmity_reach:
			pNew->mConditon.pParam = malloc(sizeof(C_ENMITY_REACH));
			memcpy(pNew->mConditon.pParam,pRoot->mConditon.pParam,sizeof(C_ENMITY_REACH));
			break;
		case CTriggerData::c_distance_reach:
			pNew->mConditon.pParam = malloc(sizeof(C_DISTANCE_REACH));
			memcpy(pNew->mConditon.pParam,pRoot->mConditon.pParam,sizeof(C_DISTANCE_REACH));
			break;
		case CTriggerData::c_attack_by_skill:
			pNew->mConditon.pParam = malloc(sizeof(C_ATTACK_BY_SKILL));
			memcpy(pNew->mConditon.pParam,pRoot->mConditon.pParam,sizeof(C_ATTACK_BY_SKILL));
			break;
		case CTriggerData::c_var:
			pNew->mConditon.pParam = malloc(sizeof(C_VAR));
			memcpy(pNew->mConditon.pParam,pRoot->mConditon.pParam,sizeof(C_VAR));
			break;
		case CTriggerData::c_constant:
			pNew->mConditon.pParam = malloc(sizeof(C_CONSTANT));
			memcpy(pNew->mConditon.pParam,pRoot->mConditon.pParam,sizeof(C_CONSTANT));
			break;
		case CTriggerData::c_rank_level:
			pNew->mConditon.pParam = malloc(sizeof(C_RANK_LEVEL));
			memcpy(pNew->mConditon.pParam,pRoot->mConditon.pParam,sizeof(C_RANK_LEVEL));
			break;
		}
	}

	if(pRoot->pLeft) pNew->pLeft = CTriggerData::CopyConditonTree(pRoot->pLeft);
	if(pRoot->pRight) pNew->pRight = CTriggerData::CopyConditonTree(pRoot->pRight);
	return pNew;
}

CTriggerData::_s_operation* CTriggerData::CopyOperation(CTriggerData::_s_operation *pOperation)
{
	_s_operation* pNewObject = new _s_operation;
	if(pNewObject==0) return 0;

	pNewObject->iType = pOperation->iType;
	pNewObject->mTarget.iType = pOperation->mTarget.iType;
	
	switch(pNewObject->iType) 
	{
	case o_attact:
		pNewObject->pParam = malloc(sizeof(O_ATTACK_TYPE));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_ATTACK_TYPE));
		break;
	case o_use_skill:
		pNewObject->pParam = malloc(sizeof(O_USE_SKILL));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_USE_SKILL));
		break;
	case o_use_range_skill:
		pNewObject->pParam = malloc(sizeof(O_USE_RANGE_SKILL));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_USE_RANGE_SKILL));
		break;
	case o_trigger_task:
		pNewObject->pParam = malloc(sizeof(O_TRIGGER_TASK));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_TRIGGER_TASK));
		break;
    case o_change_path:
		pNewObject->pParam = malloc(sizeof(O_CHANGE_PATH));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_CHANGE_PATH));
		break;
	case o_sneer_monster:
		pNewObject->pParam = malloc(sizeof(O_SNEER_MONSTER));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_SNEER_MONSTER));
		break;
	case o_set_global:
		pNewObject->pParam = malloc(sizeof(O_SET_GLOBAL));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_SET_GLOBAL));
		break;
	case o_revise_global:
		pNewObject->pParam = malloc(sizeof(O_REVISE_GLOBAL));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_REVISE_GLOBAL));
		break;
	case o_assign_global:
		pNewObject->pParam = malloc(sizeof(O_ASSIGN_GLOBAL));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_ASSIGN_GLOBAL));
		break;

	case o_talk:
		{
			pNewObject->pParam = malloc(sizeof(O_TALK_TEXT));
			unsigned int len = ((O_TALK_TEXT*)pOperation->pParam)->uSize;
			((O_TALK_TEXT*)pNewObject->pParam)->uSize = len;
			((O_TALK_TEXT*)pNewObject->pParam)->szData = new unsigned short[len/2]; 
			memcpy(((O_TALK_TEXT*)pNewObject->pParam)->szData,((O_TALK_TEXT*)pOperation->pParam)->szData,len);
		}
		break;
	case o_reset_hate_list:
		pNewObject->pParam = 0;
		break;
	case o_run_trigger:
		pNewObject->pParam = malloc(sizeof(O_RUN_TRIGGER));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_RUN_TRIGGER));
		break;
	case o_stop_trigger:
		pNewObject->pParam = malloc(sizeof(O_STOP_TRIGGER));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_STOP_TRIGGER));
		break;
	case o_active_trigger:
		pNewObject->pParam = malloc(sizeof(O_ACTIVE_TRIGGER));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_ACTIVE_TRIGGER));
		break;
	case o_create_timer:
		pNewObject->pParam = malloc(sizeof(O_CREATE_TIMER));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_CREATE_TIMER));
		break;
	case o_summon:
		pNewObject->pParam = malloc(sizeof(O_SUMMON));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_SUMMON));
		break;
	case o_summon_mineral:
		pNewObject->pParam = malloc(sizeof(O_SUMMON_MINERAL));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_SUMMON_MINERAL));
		break;
	case o_kill_timer:
		pNewObject->pParam = malloc(sizeof(O_KILL_TIMER));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_KILL_TIMER));
		break;
	case o_drop_item:
		pNewObject->pParam = malloc(sizeof(O_DROP_ITEM));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_DROP_ITEM));
		break;
	case o_change_hate:
		pNewObject->pParam = malloc(sizeof(O_CHANGE_HATE));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_CHANGE_HATE));
		break;
	case o_start_event:
		pNewObject->pParam = malloc(sizeof(O_START_EVENT));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_START_EVENT));
		break;
	case o_stop_event:
		pNewObject->pParam = malloc(sizeof(O_STOP_EVENT));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_STOP_EVENT));
		break;
	case o_active_controller:
		pNewObject->pParam = malloc(sizeof(O_ACTIVE_CONTROLLER));
		memcpy(pNewObject->pParam,pOperation->pParam,sizeof(O_ACTIVE_CONTROLLER));
		break;
	case o_flee:
		pNewObject->pParam = 0;
		break;
	case o_dispear:
		pNewObject->pParam = 0;
		break;
	case o_reset:
		pNewObject->pParam = 0;
		break;
	case o_set_hate_to_first:
		pNewObject->pParam = 0;
		break;
	case o_set_hate_to_last:
		pNewObject->pParam = 0;
		break;
	case o_set_hate_fifty_percent:
		pNewObject->pParam = 0;
		break;
	case o_skip_operation:
		pNewObject->pParam = 0;
		break;
	default:
		pNewObject->pParam = 0;
		pNewObject->iType = 0;
		break;
	}

	if(pNewObject->mTarget.iType==t_occupation_list)
	{
		pNewObject->mTarget.pParam = new T_OCCUPATION;
		memcpy(pNewObject->mTarget.pParam,pOperation->mTarget.pParam,sizeof(T_OCCUPATION));
	}else pNewObject->mTarget.pParam = 0;

	return pNewObject;
}

//----------------------------------------------------------------------------------------
//CPolicyData
//----------------------------------------------------------------------------------------


bool CPolicyData::Save(const char *szPath)
{
	FILE *pFile = fopen(szPath,"wb");
	if(pFile==NULL) return false;
	unsigned int nVersion = F_POLICY_VERSION;
	fwrite(&nVersion,sizeof(unsigned int),1,pFile);
	fwrite(&uID,sizeof(unsigned int),1,pFile);
	int n = listTriggerPtr.size();
	fwrite(&n,sizeof(int),1,pFile);
	for( int i = 0; i < n; i++)
		listTriggerPtr[i]->Save(pFile);
	fclose(pFile);
	return true;
}

bool CPolicyData::Save(FILE *pFile)
{
	unsigned int nVersion = F_POLICY_VERSION;
	fwrite(&nVersion,sizeof(unsigned int),1,pFile);
	fwrite(&uID,sizeof(unsigned int),1,pFile);
	int n = listTriggerPtr.size();
	fwrite(&n,sizeof(int),1,pFile);
	for( int i = 0; i < n; i++)
		listTriggerPtr[i]->Save(pFile);
	return true;
}

bool CPolicyData::Load(const char *szPath)
{
	
	FILE *pFile = fopen(szPath,"rb");
	if(pFile==NULL) return false;
	unsigned int nVersion;
	fread(&nVersion,sizeof(unsigned int),1,pFile);
	if(nVersion==0)
	{
		int n = 0;
		fread(&uID,sizeof(unsigned int),1,pFile);
		fread(&n,sizeof(int),1,pFile);
		for( int i = 0; i < n; i++)
		{
			CTriggerData *pNew = new CTriggerData;
			pNew->Load(pFile);
			listTriggerPtr.push_back(pNew);
		}
	}
	fclose(pFile);
	return true;
}

bool CPolicyData::Load(FILE *pFile)
{
	unsigned int nVersion;
	fread(&nVersion,sizeof(unsigned int),1,pFile);
	if(nVersion==0)
	{
		int n = 0;
		fread(&uID,sizeof(unsigned int),1,pFile);
		fread(&n,sizeof(int),1,pFile);
		for( int i = 0; i < n; i++)
		{
			CTriggerData *pNew = new CTriggerData;
			pNew->Load(pFile);
			listTriggerPtr.push_back(pNew);
		}
	}else return false;
	return true;
}

void CPolicyData::Release()
{
	int n = listTriggerPtr.size();
	for( int i = 0; i < n; ++i)
	{
		listTriggerPtr[i]->Release();
		delete listTriggerPtr[i];
	}
}

int CPolicyData::GetIndex( unsigned int id)
{
	int n = listTriggerPtr.size();
	for( int i = 0; i < n; ++i)
	{
		if(listTriggerPtr[i]->GetID() == id)
			return i;
	}
	return -1;
}

void CPolicyData::DelTriggerPtr(int idx)
{
	if((size_t)idx > listTriggerPtr.size()) return;
	if(listTriggerPtr[idx]) 
	{
		listTriggerPtr[idx]->Release();
		CTriggerData **temp = &listTriggerPtr[idx];
		delete listTriggerPtr[idx];
		listTriggerPtr.erase(temp);
	}
}


//----------------------------------------------------------------------------------------
//CPolicyDataMannager
//----------------------------------------------------------------------------------------

bool CPolicyDataManager::Load(const char* szPath)
{
	FILE *pFile = fopen(szPath,"rb");
	if(pFile==0) return false;
	
	unsigned int uVersion;
	int num;
	fread(&uVersion,sizeof(unsigned int),1,pFile);
	if(uVersion!=F_POLICY_EXP_VERSION) 
	{
		fclose(pFile);
		return false;
	}
	fread(&num,sizeof(int),1,pFile);
	for (int i = 0; i < num; ++i)
	{
		CPolicyData *pNew = new CPolicyData;
		if(pNew==0) goto fail;
		if(!pNew->Load(pFile)) goto fail;
		listPolicy.push_back(pNew);
	}
	fclose(pFile);
	return true;

fail:
	fclose(pFile);
	num = listPolicy.size();
	for( int k = 0; k < num; ++k)
	{
		listPolicy[k]->Release();
		delete listPolicy[k];
	}
	listPolicy.clear();
	return false;
}

bool CPolicyDataManager::Save(const char* szPath)
{
	FILE *pFile = fopen(szPath,"wb");
	if(pFile==0) return false;
	
	unsigned int uVersion = F_POLICY_EXP_VERSION;
	int num = listPolicy.size();
	fwrite(&uVersion,sizeof(unsigned int),1,pFile);
	fwrite(&num,sizeof(int),1,pFile);
	for (int i = 0; i < num; ++i)
	{
		CPolicyData *pPolicy = listPolicy[i];
		if(!pPolicy->Save(pFile)) 
		{
			fclose(pFile);
			return false;
		}
	}
	fclose(pFile);
	return true;
}

void CPolicyDataManager::Release()
{
	int n = listPolicy.size();
	for( int i = 0; i < n; ++i)
	{
		listPolicy[i]->Release();
		delete listPolicy[i];
	}
}
