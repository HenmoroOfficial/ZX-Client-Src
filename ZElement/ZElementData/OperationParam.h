#ifndef _AI_OPERATION_PARAM_H_
#define _AI_OPERATION_PARAM_H_


#include "AObject.h"
#include "Policy.h"



struct TRIGGER_ID_FUNCS : public CUSTOM_FUNCS
{
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	AVariant	m_var;
	AString		m_strText;
	void*       m_pParent;
};

class COperationParam
{

public:
	COperationParam(){ m_pPolicyData = 0; mTarget.iType = 0; mTarget.pParam = 0; m_pProperty = new ADynPropertyObject;}
	virtual ~COperationParam()
	{
		if(m_pProperty) delete m_pProperty; 
	}
	ADynPropertyObject* m_pProperty;
	
	virtual void UpdateProperty(bool bGet)=0;
	unsigned int GetOperationType(){ return uOperationType; };
	
	CTriggerData::_s_target mTarget;
	CPolicyData *m_pPolicyData;
	virtual void BuildProperty()=0;
protected:
	unsigned int uOperationType;
	
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Attack : public COperationParam
{

public:
	COperationParam_Attack(){ uOperationType = CTriggerData::o_attact; m_Data.uType = 0;}

public:
	enum _e_attack_type
	{
		a_close_battle = 0,		//物理肉搏
		a_skill,				//物理弓箭
		a_long_distance,        //魔法
		a_skill_and_battle,     //肉搏＋远程
		a_num,
	};
	O_ATTACK_TYPE m_Data;
protected:
	ASetTemplate<int> attack_type;
	virtual void BuildProperty()
	{
		attack_type.AddElement("物理肉搏", a_close_battle);
		attack_type.AddElement("物理弓箭", a_skill);
		attack_type.AddElement("魔法", a_long_distance);
		attack_type.AddElement("肉搏＋远程", a_skill_and_battle);
				
		m_pProperty->DynAddProperty(AVariant(m_Data.uType), "攻击类型", &attack_type);
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
			m_Data.uType = m_pProperty->GetPropVal(0);	
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.uType);
		}
	};
};



//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Use_Skill : public COperationParam
{
public:
	COperationParam_Use_Skill(){ uOperationType = CTriggerData::o_use_skill; m_Data.uSkill = 0; m_Data.uLevel = 0;}

public:
	O_USE_SKILL m_Data;

protected:
	
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.uSkill), "技能");
		m_pProperty->DynAddProperty(AVariant(m_Data.uLevel), "级别");
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
			m_Data.uSkill = m_pProperty->GetPropVal(0);
			m_Data.uLevel = m_pProperty->GetPropVal(1);	
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.uSkill);
			m_pProperty->SetPropVal(1,m_Data.uLevel);
		}
	};
};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Use_Range_Skill : public COperationParam
{
public:
	COperationParam_Use_Range_Skill(){ uOperationType = CTriggerData::o_use_range_skill; m_Data.uSkill = 0; m_Data.uLevel = 0; m_Data.fRange = 1; }

public:
	O_USE_RANGE_SKILL m_Data;

protected:
	
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.uSkill), "技能");
		m_pProperty->DynAddProperty(AVariant(m_Data.uLevel), "级别");
		m_pProperty->DynAddProperty(AVariant(m_Data.fRange), "范围(1-512)");
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
			m_Data.uSkill = m_pProperty->GetPropVal(0);
			m_Data.uLevel = m_pProperty->GetPropVal(1);	
			m_Data.fRange = m_pProperty->GetPropVal(2);	
			if(m_Data.fRange < 1.0f) m_Data.fRange = 1.0f;
			if(m_Data.fRange > 512.0f) m_Data.fRange = 512.0f;
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.uSkill);
			m_pProperty->SetPropVal(1,m_Data.uLevel);
			m_pProperty->SetPropVal(2,m_Data.fRange);
		}
	};
};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Trigger_Task : public COperationParam
{
public:
	COperationParam_Trigger_Task(){ uOperationType = CTriggerData::o_trigger_task; m_Data.uTaskID = 0; }

public:
	O_TRIGGER_TASK m_Data;

protected:
	
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.uTaskID), "任务ID");
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
			m_Data.uTaskID = m_pProperty->GetPropVal(0);
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.uTaskID);
		}
	};
};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Change_Path : public COperationParam
{
public:
	COperationParam_Change_Path(){ uOperationType = CTriggerData::o_change_path; m_Data.uPathID = 0; m_Data.iType = 0;}

public:
	O_CHANGE_PATH m_Data;

protected:
	ASetTemplate<int> path_type;
	virtual void BuildProperty()
	{
		path_type.AddElement("终点停止",0);
		path_type.AddElement("原路返回",1);
		path_type.AddElement("首尾循环",2);

		m_pProperty->DynAddProperty(AVariant(m_Data.uPathID), "路径ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.iType), "类型",&path_type);
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
			m_Data.uPathID = m_pProperty->GetPropVal(0);
			m_Data.iType = m_pProperty->GetPropVal(1);
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.uPathID);
			m_pProperty->SetPropVal(1,m_Data.iType);
		}
	};
};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Sneer_Monster : public COperationParam
{
public:
	COperationParam_Sneer_Monster(){ uOperationType = CTriggerData::o_sneer_monster; m_Data.fRange = 5.0f; }

public:
	O_SNEER_MONSTER m_Data;

protected:
	
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.fRange), "范围(m)");
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
			m_Data.fRange = m_pProperty->GetPropVal(0);
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.fRange);
		}
	};
};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Talk : public COperationParam
{
public:
	COperationParam_Talk()
	{ 
		uOperationType = CTriggerData::o_talk;
		char szText[MAX_PATH];
		strcpy(szText,"\0");
		m_Data.uSize = OP_TEXT_LENGTH*2;
		m_Data.szData = new unsigned short[m_Data.uSize/2];
		MultiByteToWideChar(CP_ACP,0,szText,MAX_PATH,(LPWSTR)m_Data.szData,OP_TEXT_LENGTH);
	}
	virtual ~COperationParam_Talk()
	{
		if(m_Data.szData) delete m_Data.szData;
	}

public:
	O_TALK_TEXT m_Data;

protected:
	virtual void BuildProperty()
	{
		char szText[1024];
		WideCharToMultiByte(CP_ACP,0,(LPWSTR)m_Data.szData,m_Data.uSize/2,szText,1024,NULL,FALSE);
		m_pProperty->DynAddProperty(AVariant(AString(szText)), "文字内容");
	}
	virtual void UpdateProperty(bool bGet)
	{
		char szText[1024];
		strcpy(szText,"\0");
		if(bGet)
		{
			AString temp = AString(m_pProperty->GetPropVal(0));	
			if(!temp.IsEmpty())
			{
				strcpy(szText,temp.GetBuffer(0));
				temp.ReleaseBuffer();
			}
			MultiByteToWideChar(CP_ACP,0,szText,1024,(LPWSTR)m_Data.szData,OP_TEXT_LENGTH);
		}else
		{
			WideCharToMultiByte(CP_ACP,0,(LPWSTR)m_Data.szData,OP_TEXT_LENGTH,szText,1024,NULL,FALSE);
			m_pProperty->SetPropVal(0,AString(szText));
		}
	};
};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Run_Trigger : public COperationParam
{

public:
	COperationParam_Run_Trigger(){ uOperationType = CTriggerData::o_run_trigger;m_Data.uID = 0;}
	virtual ~COperationParam_Run_Trigger(){ if(m_pTriggerFunc) delete m_pTriggerFunc;}
public:
	O_RUN_TRIGGER m_Data;
	
protected:
	TRIGGER_ID_FUNCS *m_pTriggerFunc;
	virtual void BuildProperty()
	{
		AString temp;
		m_pTriggerFunc = new TRIGGER_ID_FUNCS;
		m_pTriggerFunc->m_pParent = this;
		m_pProperty->DynAddProperty(AVariant(temp), "触发器", (ASet*)m_pTriggerFunc, NULL, WAY_CUSTOM);
		UpdateProperty(false);
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
		}else
		{
			m_pTriggerFunc->OnSetValue(AVariant());
		}
	};
};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Stop_Trigger : public COperationParam
{

public:
	COperationParam_Stop_Trigger(){ uOperationType = CTriggerData::o_stop_trigger;m_Data.uID = 0;}
	
public:
	O_STOP_TRIGGER m_Data;

protected:
	ASetTemplate<int> trigger_list;
	virtual void BuildProperty()
	{
		if(m_pPolicyData)
		{
			int n = m_pPolicyData->GetTriggerPtrNum();
			for( int i = 0; i < n; ++i)
			{
				CTriggerData *pData = m_pPolicyData->GetTriggerPtr(i);
				if(!pData->IsRun()) trigger_list.AddElement(pData->GetName(),pData->GetID());
			}
		}
		m_pProperty->DynAddProperty(AVariant(m_Data.uID), "触发器",&trigger_list);
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
			m_Data.uID = m_pProperty->GetPropVal(0);
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.uID);
		}
	};
};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Active_Trigger : public COperationParam
{

public:
	COperationParam_Active_Trigger(){ uOperationType = CTriggerData::o_active_trigger;m_Data.uID = 0;}

public:
	O_ACTIVE_TRIGGER m_Data;

protected:
	ASetTemplate<int> trigger_list;
	virtual void BuildProperty()
	{
		if(m_pPolicyData)
		{
			int n = m_pPolicyData->GetTriggerPtrNum();
			for( int i = 0; i < n; ++i)
			{
				CTriggerData *pData = m_pPolicyData->GetTriggerPtr(i);
				if(!pData->IsRun()) trigger_list.AddElement(pData->GetName(),pData->GetID());
			}
		}
		m_pProperty->DynAddProperty(AVariant(m_Data.uID), "触发器",&trigger_list);
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
			m_Data.uID = m_pProperty->GetPropVal(0);
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.uID);
		}
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Summon : public COperationParam
{

public:
	COperationParam_Summon(){ uOperationType = CTriggerData::o_summon;
	m_Data.uMounsterID = 0;m_Data.uBodyMounsterID = 0;m_Data.uMounsterNum = 1; 
	m_Data.uLife = 0; memset(m_Data.szName,0,sizeof(WORD)*16);
	m_Data.fRange = 2; m_Data.bFollow = true; m_Data.bDispear = true;}

public:
	O_SUMMON m_Data;

protected:
	virtual void BuildProperty()
	{
		char szText[MAX_PATH];
		memset(szText,0,MAX_PATH);
		WideCharToMultiByte(CP_ACP,0,(LPWSTR)m_Data.szName,16,szText,MAX_PATH,NULL,FALSE);
		m_pProperty->DynAddProperty(AVariant(AString(szText)), "怪物名字(9字符)");
		
		m_pProperty->DynAddProperty(AVariant(m_Data.uMounsterID), "怪物ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.uBodyMounsterID), "怪物外形");
		m_pProperty->DynAddProperty(AVariant(m_Data.uMounsterNum), "怪物数量");
		m_pProperty->DynAddProperty(AVariant(m_Data.uLife), "怪物生存时间(秒)");
		m_pProperty->DynAddProperty(AVariant(m_Data.fRange), "距离本体的距离");
		m_pProperty->DynAddProperty(AVariant(m_Data.bFollow), "是否跟随本体");
		m_pProperty->DynAddProperty(AVariant(m_Data.bDispear), "是否跟随本题消失");
		
	}
	virtual void UpdateProperty(bool bGet)
	{
		char szText[MAX_PATH];
		memset(szText,0,MAX_PATH);
		if(bGet)
		{
			
			AString temp = AString(m_pProperty->GetPropVal(0));	
			if(!temp.IsEmpty())
			{
				strcpy(szText,temp.GetBuffer(0));
				temp.ReleaseBuffer();
			}
			
			MultiByteToWideChar(CP_ACP,0,szText,MAX_PATH,(LPWSTR)m_Data.szName,16);
			szText[15] = 0;
			m_Data.uMounsterID = m_pProperty->GetPropVal(1);
			m_Data.uBodyMounsterID = m_pProperty->GetPropVal(2);
			m_Data.uMounsterNum = m_pProperty->GetPropVal(3);
			m_Data.uLife = m_pProperty->GetPropVal(4);
			m_Data.fRange = m_pProperty->GetPropVal(5);
			m_Data.bFollow = m_pProperty->GetPropVal(6);
			m_Data.bDispear = m_pProperty->GetPropVal(7);

			if(!m_Data.bFollow) m_Data.bDispear = false;
		
		}else
		{
			WideCharToMultiByte(CP_ACP,0,(LPWSTR)m_Data.szName,OP_TEXT_LENGTH,szText,MAX_PATH,NULL,FALSE);
			m_pProperty->SetPropVal(0,AString(szText));
			
			m_pProperty->SetPropVal(1,m_Data.uMounsterID);
			m_pProperty->SetPropVal(2,m_Data.uBodyMounsterID);
			m_pProperty->SetPropVal(3,m_Data.uMounsterNum);
			m_pProperty->SetPropVal(4,m_Data.uLife);
			m_pProperty->SetPropVal(5,m_Data.fRange);
			m_pProperty->SetPropVal(6,m_Data.bFollow);
			m_pProperty->SetPropVal(7,m_Data.bDispear);
		}
	};
};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Create_Timer : public COperationParam
{

public:
	COperationParam_Create_Timer(){ uOperationType = CTriggerData::o_create_timer;m_Data.uID = 0;m_Data.uCounter = 1;m_Data.uPeriod = 10;}

public:
	O_CREATE_TIMER m_Data;

protected:
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.uID), "定时器ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.uPeriod), "时间间隔");
		m_pProperty->DynAddProperty(AVariant(m_Data.uCounter), "次数");
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
			m_Data.uID = m_pProperty->GetPropVal(0);
			m_Data.uPeriod = m_pProperty->GetPropVal(1);
			m_Data.uCounter = m_pProperty->GetPropVal(2);
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.uID);
			m_pProperty->SetPropVal(1,m_Data.uPeriod);
			m_pProperty->SetPropVal(2,m_Data.uCounter);
		}
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Kill_Timer : public COperationParam
{

public:
	COperationParam_Kill_Timer(){ uOperationType = CTriggerData::o_kill_timer;m_Data.uID = 0;}

public:
	O_KILL_TIMER m_Data;

protected:
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.uID), "定时器ID");
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
			m_Data.uID = m_pProperty->GetPropVal(0);
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.uID);
		}
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Reset_hates : public COperationParam
{

public:
	COperationParam_Reset_hates(){ uOperationType = CTriggerData::o_reset_hate_list;}

public:
	

protected:
	virtual void BuildProperty()
	{
	}
	virtual void UpdateProperty(bool bGet)
	{
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Flee : public COperationParam
{

public:
	COperationParam_Flee(){ uOperationType = CTriggerData::o_flee;}

public:
	

protected:
	virtual void BuildProperty()
	{
	}
	virtual void UpdateProperty(bool bGet)
	{
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Dispear : public COperationParam
{

public:
	COperationParam_Dispear(){ uOperationType = CTriggerData::o_dispear;}

public:
	

protected:
	virtual void BuildProperty()
	{
	}
	virtual void UpdateProperty(bool bGet)
	{
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Reset : public COperationParam
{

public:
	COperationParam_Reset(){ uOperationType = CTriggerData::o_reset;}

public:
	

protected:
	virtual void BuildProperty()
	{
	}
	virtual void UpdateProperty(bool bGet)
	{
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_SetHateToFirst : public COperationParam
{

public:
	COperationParam_SetHateToFirst(){ uOperationType = CTriggerData::o_set_hate_to_first;}

public:
	

protected:
	virtual void BuildProperty()
	{
	}
	virtual void UpdateProperty(bool bGet)
	{
	};
};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_SetHateToLast : public COperationParam
{

public:
	COperationParam_SetHateToLast(){ uOperationType = CTriggerData::o_set_hate_to_last;}

public:
	

protected:
	virtual void BuildProperty()
	{
	}
	virtual void UpdateProperty(bool bGet)
	{
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_SetHateFiftyPercent : public COperationParam
{

public:
	COperationParam_SetHateFiftyPercent(){ uOperationType = CTriggerData::o_set_hate_fifty_percent;}

public:
	

protected:
	virtual void BuildProperty()
	{
	}
	virtual void UpdateProperty(bool bGet)
	{
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_SkipOperation : public COperationParam
{

public:
	COperationParam_SkipOperation(){ uOperationType = CTriggerData::o_skip_operation;}

public:
	

protected:
	virtual void BuildProperty()
	{
	}
	virtual void UpdateProperty(bool bGet)
	{
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Active_Controller : public COperationParam
{

public:
	COperationParam_Active_Controller(){ uOperationType = CTriggerData::o_active_controller;m_Data.uID = 0; m_Data.bStop = 0; }

public:
	O_ACTIVE_CONTROLLER m_Data;

protected:
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.uID), "产生怪物控制器ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.bStop), "停止激活");
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
			m_Data.uID = m_pProperty->GetPropVal(0);
			m_Data.bStop = m_pProperty->GetPropVal(1);
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.uID);
			m_pProperty->SetPropVal(1,m_Data.bStop);
		}
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Set_Global : public COperationParam
{

public:
	COperationParam_Set_Global(){ uOperationType = CTriggerData::o_set_global;m_Data.iID = 0; m_Data.iValue = 0; }

public:
	O_SET_GLOBAL m_Data;

protected:
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.iID), "变量ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.iValue), "数值");
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
			m_Data.iID = m_pProperty->GetPropVal(0);
			m_Data.iValue = m_pProperty->GetPropVal(1);
		
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.iID);
			m_pProperty->SetPropVal(1,m_Data.iValue);
		}
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Revise_Global : public COperationParam
{

public:
	COperationParam_Revise_Global(){ uOperationType = CTriggerData::o_revise_global;m_Data.iID = 0; m_Data.iValue = 0; }

public:
	O_REVISE_GLOBAL m_Data;

protected:
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.iID), "变量ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.iValue), "数值");
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
			m_Data.iID = m_pProperty->GetPropVal(0);
			m_Data.iValue = m_pProperty->GetPropVal(1);
		
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.iID);
			m_pProperty->SetPropVal(1,m_Data.iValue);
		}
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Assign_Global : public COperationParam
{

public:
	COperationParam_Assign_Global(){ uOperationType = CTriggerData::o_assign_global;m_Data.iIDSrc = 0; m_Data.iIDDst = 0; }

public:
	O_ASSIGN_GLOBAL m_Data;

protected:
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.iIDDst), "被赋值全局变量ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.iIDSrc), "赋值全局变量ID");
	}
	virtual void UpdateProperty(bool bGet)
	{
		if(bGet)
		{
			m_Data.iIDSrc = m_pProperty->GetPropVal(0);
			m_Data.iIDDst = m_pProperty->GetPropVal(1);
		
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.iIDSrc);
			m_pProperty->SetPropVal(1,m_Data.iIDDst);
		}
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Summon_Mineral : public COperationParam
{

public:
	COperationParam_Summon_Mineral(){ uOperationType = CTriggerData::o_summon_mineral;
	m_Data.uMineralID= 0;m_Data.uMineralNum= 1; 
	m_Data.uHP= 0; m_Data.fRange = 10; m_Data.bBind= false;}

public:
	O_SUMMON_MINERAL m_Data;

protected:
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.uMineralID), "矿物ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.uMineralNum), "矿物数量");
		m_pProperty->DynAddProperty(AVariant(m_Data.uHP), "矿物生命");
		m_pProperty->DynAddProperty(AVariant(m_Data.fRange), "距本体距离");
		m_pProperty->DynAddProperty(AVariant(m_Data.bBind), "是否绑定目标");
		
	}
	virtual void UpdateProperty(bool bGet)
	{
		char szText[MAX_PATH];
		memset(szText,0,MAX_PATH);
		if(bGet)
		{
			m_Data.uMineralID = m_pProperty->GetPropVal(0);
			m_Data.uMineralNum = m_pProperty->GetPropVal(1);
			m_Data.uHP = m_pProperty->GetPropVal(2);
			m_Data.fRange = m_pProperty->GetPropVal(3);
			m_Data.bBind = m_pProperty->GetPropVal(4);
	
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.uMineralID);
			m_pProperty->SetPropVal(1,m_Data.uMineralNum);
			m_pProperty->SetPropVal(2,m_Data.uHP);
			m_pProperty->SetPropVal(3,m_Data.fRange);
			m_pProperty->SetPropVal(4,m_Data.bBind);
		}
	};
};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class COperationParam_Drop_Item : public COperationParam
{

public:
	COperationParam_Drop_Item(){ uOperationType = CTriggerData::o_drop_item;
	m_Data.uItemID= 0;m_Data.uItemNum= 1; 
	m_Data.uExpireDate= 0;}

public:
	O_DROP_ITEM m_Data;

protected:
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.uItemID), "物品ID");
		m_pProperty->DynAddProperty(AVariant(m_Data.uItemNum), "物品数量");
		m_pProperty->DynAddProperty(AVariant(m_Data.uExpireDate), "物品生命");		
	}
	virtual void UpdateProperty(bool bGet)
	{
		char szText[MAX_PATH];
		memset(szText,0,MAX_PATH);
		if(bGet)
		{
			m_Data.uItemID = m_pProperty->GetPropVal(0);
			m_Data.uItemNum = m_pProperty->GetPropVal(1);
			m_Data.uExpireDate = m_pProperty->GetPropVal(2);
	
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.uItemID);
			m_pProperty->SetPropVal(1,m_Data.uItemNum);
			m_pProperty->SetPropVal(2,m_Data.uExpireDate);
		}
	};
};

//-----------------------------------------------------------------------------
class COperationParam_Change_Hate : public COperationParam
{
public:
	COperationParam_Change_Hate()
	{
		uOperationType		=	CTriggerData::o_change_hate;
		m_Data.iHateValue	=	0;
	}

public:
	O_CHANGE_HATE	m_Data;

protected:
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.iHateValue), "仇恨值");	
	}
	virtual void UpdateProperty(bool bGet)
	{
		char szText[MAX_PATH];
		memset(szText,0,MAX_PATH);
		if(bGet)
		{
			m_Data.iHateValue = m_pProperty->GetPropVal(0);
	
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.iHateValue);
		}
	}
};

//-----------------------------------------------------------------------------
class COperationParam_Start_Event : public COperationParam
{
public:
	COperationParam_Start_Event()
	{
		uOperationType	=	CTriggerData::o_start_event;
		m_Data.iId	=	0;
	}
	
public:
	O_START_EVENT	m_Data;
	
protected:
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.iId), "事件id");	
	}
	virtual void UpdateProperty(bool bGet)
	{
		char szText[MAX_PATH];
		memset(szText,0,MAX_PATH);
		if(bGet)
		{
			m_Data.iId = m_pProperty->GetPropVal(0);
			
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.iId);
		}
	}
};

//-----------------------------------------------------------------------------
class COperationParam_Stop_Event : public COperationParam
{
public:
	COperationParam_Stop_Event()
	{
		uOperationType	=	CTriggerData::o_stop_event;
		m_Data.iId	=	0;
	}
	
public:
	O_STOP_EVENT	m_Data;
	
protected:
	virtual void BuildProperty()
	{
		m_pProperty->DynAddProperty(AVariant(m_Data.iId), "事件id");	
	}
	virtual void UpdateProperty(bool bGet)
	{
		char szText[MAX_PATH];
		memset(szText,0,MAX_PATH);
		if(bGet)
		{
			m_Data.iId = m_pProperty->GetPropVal(0);
			
		}else
		{
			m_pProperty->SetPropVal(0,m_Data.iId);
		}
	}
};

#endif