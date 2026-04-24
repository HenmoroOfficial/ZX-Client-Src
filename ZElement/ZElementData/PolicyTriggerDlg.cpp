// PolicyTriggerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ElementData.h"
#include "PolicyTriggerDlg.h"
#include "PolicyOperationDlg.h"
#include "OperationParam.h"
#include "ConditionDlg.h"
#include "BaseDataIDMan.h"
#include "PolicyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CString strConditonSign[];
/////////////////////////////////////////////////////////////////////////////
// CPolicyTriggerDlg dialog
extern CString strOperationList[];

CPolicyTriggerDlg::CPolicyTriggerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPolicyTriggerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPolicyTriggerDlg)
	m_strProgram = _T("");
	m_strTriggerName = _T("");
	m_bActive = FALSE;
	m_uTriggerID = 0;
	//}}AFX_DATA_INIT
	m_pTriggerData = NULL;
	m_bModifyed = false;
	m_dwTriggerID = 0;
	m_bIsChanged = false; 
	m_RunCondition = CTriggerData::run_attack_effect;
	m_pCurrentPolicy = 0;
	m_pOldTree = 0;
}


void CPolicyTriggerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPolicyTriggerDlg)
	DDX_Control(pDX, IDC_LISTBOX_OPERATION, m_listOperation);
	DDX_Text(pDX, IDC_EDIT_PROGRAM, m_strProgram);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_NAME, m_strTriggerName);
	DDX_Check(pDX, IDC_CHECK_ACTIVE, m_bActive);
	DDX_Text(pDX, IDC_EDIT_TRIGGER_ID, m_uTriggerID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPolicyTriggerDlg, CDialog)
	//{{AFX_MSG_MAP(CPolicyTriggerDlg)
	ON_BN_CLICKED(IDC_ADD_OPERATION, OnAddOperation)
	ON_BN_CLICKED(ID_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DEL_OPERATION, OnDelOperation)
	ON_BN_CLICKED(IDC_MOVE_UP, OnMoveUp)
	ON_BN_CLICKED(IDC_MOVE_DOWN, OnMoveDown)
	ON_BN_CLICKED(IDC_ADD_CONDITION, OnAddCondition)
	ON_EN_CHANGE(IDC_EDIT_TRIGGER_NAME, OnChangeEditTriggerName)
	ON_BN_CLICKED(IDC_CHECK_ACTIVE, OnCheckActive)
	ON_LBN_DBLCLK(IDC_LISTBOX_OPERATION, OnDblclkListboxOperation)
	ON_BN_CLICKED(IDC_RADIO_ATTACK_EFFECT, OnRadioAttackEffect)
	ON_BN_CLICKED(IDC_RADIO_ATTACK_NOEFFECT, OnRadioAttackNoeffect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPolicyTriggerDlg message handlers

void CPolicyTriggerDlg::OnAddOperation() 
{
	// TODO: Add your control notification handler code here
	CPolicyOperationDlg dlg;
	dlg.m_pPolicyData = m_pCurrentPolicy;
	if(IDOK==dlg.DoModal())
	{
		void *pTemp = NULL;
		COperationParam *pParam = dlg.GetNewOperation();
		switch(pParam->GetOperationType())
		{
		case CTriggerData::o_active_trigger:
			pTemp = malloc(sizeof(O_ACTIVE_TRIGGER));
			((O_ACTIVE_TRIGGER *)pTemp)->uID = ((COperationParam_Active_Trigger*)pParam)->m_Data.uID;
			break;
		case CTriggerData::o_attact:
			pTemp = malloc(sizeof(O_ATTACK_TYPE));
			((O_ATTACK_TYPE *)pTemp)->uType = ((COperationParam_Attack*)pParam)->m_Data.uType;
			break;
		case CTriggerData::o_create_timer:
			pTemp = malloc(sizeof(O_CREATE_TIMER));
			*((O_CREATE_TIMER *)pTemp) = ((COperationParam_Create_Timer*)pParam)->m_Data;
			break;
		case CTriggerData::o_summon:
			pTemp = malloc(sizeof(O_SUMMON));
			*((O_SUMMON *)pTemp) = ((COperationParam_Summon*)pParam)->m_Data;
			break;
		case CTriggerData::o_summon_mineral:
			pTemp = malloc(sizeof(O_SUMMON_MINERAL));
			*((O_SUMMON_MINERAL *)pTemp) = ((COperationParam_Summon_Mineral*)pParam)->m_Data;
			break;
		case CTriggerData::o_drop_item:
			pTemp = malloc(sizeof(O_DROP_ITEM));
			*((O_DROP_ITEM *)pTemp) = ((COperationParam_Drop_Item*)pParam)->m_Data;
			break;
		case CTriggerData::o_change_hate:
		{
			pTemp = malloc(sizeof(O_CHANGE_HATE));
			*((O_CHANGE_HATE *)pTemp) = ((COperationParam_Change_Hate*)pParam)->m_Data;
			break;
		}
		case CTriggerData::o_start_event:
		{
			pTemp = malloc(sizeof(O_START_EVENT));
			*((O_START_EVENT *)pTemp) = ((COperationParam_Start_Event*)pParam)->m_Data;
			break;
		}
		case CTriggerData::o_stop_event:
		{
			pTemp = malloc(sizeof(O_STOP_EVENT));
			*((O_STOP_EVENT *)pTemp) = ((COperationParam_Stop_Event*)pParam)->m_Data;
			break;
		}
		case CTriggerData::o_kill_timer:
			pTemp = malloc(sizeof(O_KILL_TIMER));
			((O_KILL_TIMER *)pTemp)->uID = ((COperationParam_Kill_Timer*)pParam)->m_Data.uID;
			break;
		case CTriggerData::o_active_controller:
			pTemp = malloc(sizeof(O_ACTIVE_CONTROLLER));
			((O_ACTIVE_CONTROLLER *)pTemp)->uID = ((COperationParam_Active_Controller*)pParam)->m_Data.uID;
			((O_ACTIVE_CONTROLLER *)pTemp)->bStop = ((COperationParam_Active_Controller*)pParam)->m_Data.bStop;
			break;
		case CTriggerData::o_run_trigger:
			pTemp = malloc(sizeof(O_RUN_TRIGGER));
			((O_RUN_TRIGGER *)pTemp)->uID = ((COperationParam_Run_Trigger*)pParam)->m_Data.uID;
			break;
		case CTriggerData::o_stop_trigger:
			pTemp = malloc(sizeof(O_STOP_TRIGGER));
			((O_STOP_TRIGGER *)pTemp)->uID = ((COperationParam_Stop_Trigger*)pParam)->m_Data.uID;
			break;
		case CTriggerData::o_talk:
			pTemp = malloc(sizeof(O_TALK_TEXT));
			((O_TALK_TEXT *)pTemp)->uSize = wcslen((WCHAR*)((COperationParam_Talk*)pParam)->m_Data.szData)*2 + 2;
			((O_TALK_TEXT *)pTemp)->szData = new unsigned short[((O_TALK_TEXT *)pTemp)->uSize];
			wcscpy((WCHAR*)((O_TALK_TEXT *)pTemp)->szData,(WCHAR*)((COperationParam_Talk*)pParam)->m_Data.szData);
			break;
		case CTriggerData::o_use_skill:
			pTemp = malloc(sizeof(O_USE_SKILL));
			((O_USE_SKILL *)pTemp)->uSkill = ((COperationParam_Use_Skill*)pParam)->m_Data.uSkill;
			((O_USE_SKILL *)pTemp)->uLevel = ((COperationParam_Use_Skill*)pParam)->m_Data.uLevel;
			break;
		case CTriggerData::o_use_range_skill:
			pTemp = malloc(sizeof(O_USE_RANGE_SKILL));
			((O_USE_RANGE_SKILL *)pTemp)->uSkill = ((COperationParam_Use_Range_Skill*)pParam)->m_Data.uSkill;
			((O_USE_RANGE_SKILL *)pTemp)->uLevel = ((COperationParam_Use_Range_Skill*)pParam)->m_Data.uLevel;
			((O_USE_RANGE_SKILL *)pTemp)->fRange = ((COperationParam_Use_Range_Skill*)pParam)->m_Data.fRange;
			break;
		case CTriggerData::o_trigger_task:
			pTemp = malloc(sizeof(O_TRIGGER_TASK));
			((O_TRIGGER_TASK *)pTemp)->uTaskID = ((COperationParam_Trigger_Task*)pParam)->m_Data.uTaskID;
			break;
		case CTriggerData::o_change_path:
			pTemp = malloc(sizeof(O_CHANGE_PATH));
			((O_CHANGE_PATH *)pTemp)->uPathID = ((COperationParam_Change_Path*)pParam)->m_Data.uPathID;
			((O_CHANGE_PATH *)pTemp)->iType = ((COperationParam_Change_Path*)pParam)->m_Data.iType;
			break;
		case CTriggerData::o_sneer_monster:
			pTemp = malloc(sizeof(O_SNEER_MONSTER));
			((O_SNEER_MONSTER *)pTemp)->fRange = ((COperationParam_Sneer_Monster*)pParam)->m_Data.fRange;
			break;
		case CTriggerData::o_set_global:
			pTemp = malloc(sizeof(O_SET_GLOBAL));
			*((O_SET_GLOBAL *)pTemp) = ((COperationParam_Set_Global*)pParam)->m_Data;
			break;
		case CTriggerData::o_revise_global:
			pTemp = malloc(sizeof(O_REVISE_GLOBAL));
			*((O_REVISE_GLOBAL *)pTemp) = ((COperationParam_Revise_Global*)pParam)->m_Data;
			break;
		case CTriggerData::o_assign_global:
			pTemp = malloc(sizeof(O_ASSIGN_GLOBAL));
			*((O_ASSIGN_GLOBAL *)pTemp) = ((COperationParam_Assign_Global*)pParam)->m_Data;
			break;
		}
		if(m_pTriggerData) m_pTriggerData->AddOperaion(pParam->GetOperationType(),pTemp,&pParam->mTarget);
		m_bIsChanged = true;
		FreshList();
		if(dlg.GetNewOperation()) 
		{
			delete dlg.GetNewOperation();
		}
	}

}

void CPolicyTriggerDlg::OnModify() 
{
	if(m_pTriggerData==NULL || m_pCurrentPolicy==NULL) return;
	UpdateData(true);
	
	if(m_pTriggerData->GetConditonRoot()==0)
	{
		MessageBox("触发器必须有条件表达式!");
		return;
	}
	
	if(m_pTriggerData->GetOperaionNum()<=0)
	{
		MessageBox("触发器中至少有一个操作!");
		return;
	}

	if(m_bModifyed)
	{
		if(m_strTriggerName.IsEmpty())
		{
			MessageBox("触发器名字不能为空!");
			return;
		}
		if(m_pTriggerData) m_pTriggerData->SetName(m_strTriggerName);
		if(m_pOldTree) delete m_pOldTree;
		
	}else
	{
		CString strName;
		unsigned int tempId = 0;
		strName = m_strTriggerName;
		if(m_strTriggerName.IsEmpty()) 
		{
			MessageBox("触发器名字不能为空!");
			return;
		}
		for( int i = 0; i < m_pCurrentPolicy->GetTriggerPtrNum(); ++i)
		{
			CTriggerData *pData = m_pCurrentPolicy->GetTriggerPtr(i);
			if(tempId <= pData->GetID()) tempId = pData->GetID() + 1;
		}
		m_dwTriggerID = tempId;
		m_pTriggerData->SetName(strName);
		m_pTriggerData->SetID(m_dwTriggerID);
	}
	if(m_bActive) m_pTriggerData->ActiveTrigger();
	else m_pTriggerData->ToggleTrigger();
	m_pTriggerData->SetRunCondition(m_RunCondition);
	m_bIsChanged = true;
	CDialog::OnOK();
}

void CPolicyTriggerDlg::OnCancel()
{
	if(m_bModifyed)
	{
		RestoreTrigger(m_pOldTree);
		if(m_pOldTree) delete m_pOldTree;
	}else
	{	
		if(m_pTriggerData)
		{
			m_pTriggerData->Release();
			delete m_pTriggerData;
		}
	}
	CDialog::OnCancel();
}

BOOL CPolicyTriggerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(!m_bModifyed)
	{
		CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO_ATTACK_EFFECT);
		pButton->SetCheck(true);
		m_RunCondition = CTriggerData::run_attack_effect;
		m_pTriggerData = new CTriggerData;
	}else 
	{
		CWnd *pWnd = GetDlgItem(IDC_EDIT_TRIGGER_NAME);
		m_strTriggerName = m_pTriggerData->GetName();
		//pWnd->EnableWindow(false);
		FreshList();
		m_bActive = m_pTriggerData->IsActive();
		m_RunCondition = m_pTriggerData->GetRunCondition();
		
		CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO_ATTACK_EFFECT);
		if(m_RunCondition==CTriggerData::run_attack_effect) pButton->SetCheck(true);
			else pButton->SetCheck(false);
		pButton = (CButton *)GetDlgItem(IDC_RADIO_ATTACK_NOEFFECT);
		if(m_RunCondition==CTriggerData::run_attack_noeffect) pButton->SetCheck(true);
			else pButton->SetCheck(false);
		pButton = (CButton *)GetDlgItem(IDC_RADIO_TICK);

		CTriggerData::_s_tree_item *root = m_pTriggerData->GetConditonRoot();
		m_strProgram = TraverselTree(root);
		m_uTriggerID = m_pTriggerData->GetID();
		
		//保存原来的触发数据，以后便于取消是恢复
		StartRecord();
		
		UpdateData(false);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPolicyTriggerDlg::FreshList()
{
	int count = m_listOperation.GetCount();
	int i;
	for( i = 0; i < count; i++ )
		m_listOperation.DeleteString(0);

	if(m_pTriggerData==NULL) return;
	int n = m_pTriggerData->GetOperaionNum();
	unsigned int iType;
	CString strCommand;
	CString strParam;
	CTriggerData::_s_operation* pOperation = 0;
	void *pData;
	for( i = 0; i < n; i++)
	{
		pOperation = m_pTriggerData->GetOperaion(i);
		pData = pOperation->pParam;
		iType = pOperation->iType;
		strCommand = strOperationList[iType];
		strCommand +=" ";
		strParam = "";
		switch(iType)
		{
		case CTriggerData::o_active_trigger:
			strParam.Format("[触发器(%s)]",GetTriggerName(((O_ACTIVE_TRIGGER *)pData)->uID));
			break;
		case CTriggerData::o_attact:
			switch(((O_ATTACK_TYPE *)pData)->uType)
			{
			case COperationParam_Attack::a_close_battle:
				strParam ="[物理肉搏]";
				break;
			case COperationParam_Attack::a_skill:
				strParam ="[物理弓箭]";
				break;
			case COperationParam_Attack::a_long_distance:
				strParam ="[魔法]";
				break;
			case COperationParam_Attack::a_skill_and_battle:
				strParam ="[肉搏＋远程]";
				break;
			}
			break;
		case CTriggerData::o_create_timer:
			strParam.Format("[定时器(%d)] [间隔 %d] [次数 %d]",((O_CREATE_TIMER *)pData)->uID,((O_CREATE_TIMER *)pData)->uPeriod,((O_CREATE_TIMER *)pData)->uCounter);
			break;
		case CTriggerData::o_summon:
			{
				char szText[MAX_PATH];
				WideCharToMultiByte(CP_ACP,0,(WCHAR*)((O_SUMMON *)pData)->szName,16,szText,MAX_PATH,NULL,FALSE);
				strParam.Format("[(%s)]",szText);
				break;
			}
		case CTriggerData::o_summon_mineral:
			{
				strParam.Format("[(id=%d)]",((O_SUMMON_MINERAL *)pData)->uMineralID);
				break;
			}
		case CTriggerData::o_drop_item:
			{
				strParam.Format("[(id=%d)]",((O_DROP_ITEM *)pData)->uItemID);
				break;
			}
		case CTriggerData::o_change_hate:
			{
				strParam.Format("[(改变仇恨值=%d)]",((O_CHANGE_HATE *)pData)->iHateValue);
				break;
			}
		case CTriggerData::o_start_event:
			{
				strParam.Format("[(id=%d)]",((O_START_EVENT *)pData)->iId);
				break;
			}
		case CTriggerData::o_stop_event:
			{
				strParam.Format("[(id=%d)]",((O_STOP_EVENT *)pData)->iId);
				break;
			}
		case CTriggerData::o_set_global:
			{
				strParam.Format("[(id=%d)(value=%d)]",((O_SET_GLOBAL *)pData)->iID,((O_SET_GLOBAL *)pData)->iValue);
				break;
			}
		case CTriggerData::o_revise_global:
			{
				strParam.Format("[(id=%d)(value=%d)]",((O_REVISE_GLOBAL *)pData)->iID,((O_REVISE_GLOBAL *)pData)->iValue);
				break;
			}
		case CTriggerData::o_assign_global:
			{
				strParam.Format("[(iddst=%d)(idsrc=%d)]",((O_ASSIGN_GLOBAL *)pData)->iIDDst,((O_ASSIGN_GLOBAL *)pData)->iIDSrc);
				break;
			}
		case CTriggerData::o_kill_timer:
			strParam.Format("[定时器(%d)]",((O_KILL_TIMER *)pData)->uID);
			break;
		case CTriggerData::o_active_controller:
			strParam.Format("[产生怪物控制器(%d)] [停止激活(%d)]",((O_ACTIVE_CONTROLLER *)pData)->uID,((O_ACTIVE_CONTROLLER *)pData)->bStop);
			break;
		case CTriggerData::o_run_trigger:
			strParam.Format("[触发器(%s)]",GetTriggerName(((O_RUN_TRIGGER *)pData)->uID));
			break;
		case CTriggerData::o_stop_trigger:
			strParam.Format("[触发器(%s)]",GetTriggerName(((O_STOP_TRIGGER *)pData)->uID));
			break;
		case CTriggerData::o_talk:
			//((O_TALK_TEXT *)pTemp)->szBuffer
			strParam.Format("[内容%s]","...");
			break;
		case CTriggerData::o_use_skill:
			strParam.Format("[技能%d] [等级%d]",((O_USE_SKILL *)pData)->uSkill,((O_USE_SKILL *)pData)->uLevel);
			break;
		case CTriggerData::o_use_range_skill:
			strParam.Format("[技能%d] [等级%d] [范围%g]",((O_USE_RANGE_SKILL *)pData)->uSkill,((O_USE_RANGE_SKILL *)pData)->uLevel,((O_USE_RANGE_SKILL *)pData)->fRange);
			break;
		case CTriggerData::o_trigger_task:
			strParam.Format("[任务ID(%d)]",((O_TRIGGER_TASK *)pData)->uTaskID);
			break;
		case CTriggerData::o_change_path:
			strParam.Format("[路径ID(%d)[类型ID(%d)]",((O_CHANGE_PATH *)pData)->uPathID,((O_CHANGE_PATH *)pData)->iType);
			break;
		case CTriggerData::o_sneer_monster:
			strParam.Format("[范围(%4.2f)]",((O_SNEER_MONSTER *)pData)->fRange);
			break;
		}
		strParam += GetTargetStr(&pOperation->mTarget);
		m_listOperation.AddString(strCommand + strParam);
	}
}

void CPolicyTriggerDlg::OnDelOperation() 
{
	// TODO: Add your control notification handler code here
	int sel = m_listOperation.GetCurSel();
	if(sel!=-1) 
	{
		if(IDYES ==	AfxMessageBox("你确定要删除当前选中的操作吗?",MB_YESNO|MB_ICONQUESTION))
		{
			m_pTriggerData->DelOperation(sel);
			FreshList();
			m_bIsChanged = true;
		}
	}
}

void CPolicyTriggerDlg::OnMoveUp() 
{
	// TODO: Add your control notification handler code here
	int sel = m_listOperation.GetCurSel();
	if(sel == -1 || sel == 0) return;
	unsigned int iType1,iType2;
	void *pData1, *pData2;
	CTriggerData::_s_target target1,target2;
	m_pTriggerData->GetOperaion(sel,iType1,&pData1, target1);
	m_pTriggerData->GetOperaion(sel-1,iType2,&pData2,target2);

	m_pTriggerData->SetOperation(sel,iType2,pData2,&target2);
	m_pTriggerData->SetOperation(sel-1,iType1, pData1,&target1);
	FreshList();
	m_listOperation.SetCurSel(sel-1);
	m_bIsChanged = true;

}

void CPolicyTriggerDlg::OnMoveDown() 
{
	// TODO: Add your control notification handler code here
	int sel = m_listOperation.GetCurSel();
	if(sel == -1 || sel > m_listOperation.GetCount()-2) return;
	unsigned int iType1,iType2;
	void *pData1,*pData2;
	CTriggerData::_s_target target1,target2;
	m_pTriggerData->GetOperaion(sel,iType1,&pData1, target1);
	m_pTriggerData->GetOperaion(sel+1,iType2,&pData2,target2);

	m_pTriggerData->SetOperation(sel,iType2,pData2,&target2);
	m_pTriggerData->SetOperation(sel+1,iType1, pData1,&target1);
	FreshList();
	m_listOperation.SetCurSel(sel+1);
	m_bIsChanged = true;
}


void CPolicyTriggerDlg::FreshCondition()
{
	/*
	int count = m_listCondition.GetCount();
	for( int i = 0; i < count; i++ )
		m_listCondition.DeleteString(0);

	if(m_pTriggerData==NULL) return;
	int n = m_pTriggerData->GetConditionNum();
	unsigned int iType;
	CString strCommand;
	CString strParam;
	CString strTotal;
	CString strSign;
	void *pData;
	for( i = 0; i < n; i++)
	{
		m_pTriggerData->GetCondition(i,iType,&pData);
		strCommand = strConditonList[iType];
		strParam = "";
		switch(iType)
		{
		case CTriggerData::c_hp_less:
			strParam.Format("%d",((C_HP_LESS*)pData)->uPercent);
			break;
		case CTriggerData::c_time_come:
			strParam.Format("%d",((C_TIME_COME*)pData)->uID);
			break;
		case CTriggerData::c_random:
			strParam.Format("%d",((C_RANDOM*)pData)->uProbability);
			break;
		}
		if(!strParam.IsEmpty())
		{
			strCommand +="(";
			strCommand +=strParam;
			strCommand +=")";
			m_listCondition.AddString(strCommand);
		}else m_listCondition.AddString(strCommand);
		
		strSign.Format(strConditonSign[iType],strParam);
		strTotal += strSign;
		strTotal +=" ";
	}

	m_strProgram = strTotal;
	*/
	UpdateData(false);
}

CString CPolicyTriggerDlg::TraverselTree( void *pTree)
{
	CTriggerData::_s_tree_item * temp = (CTriggerData::_s_tree_item*)pTree;
	if(temp==0) return "";
	CString str;
	CString sign;
	switch(temp->mConditon.iType) 
	{
	case CTriggerData::c_time_come:
		sign.Format(strConditonSign[temp->mConditon.iType],((C_TIME_COME*)temp->mConditon.pParam)->uID);
		break;
	case CTriggerData::c_path_end_point:
		sign.Format(strConditonSign[temp->mConditon.iType],((C_PATH_END_POINT*)temp->mConditon.pParam)->uID);
		break;
	case CTriggerData::c_hp_less:
		sign.Format(strConditonSign[temp->mConditon.iType],((C_HP_LESS*)temp->mConditon.pParam)->fPercent);
		break;
	case CTriggerData::c_random:
		sign.Format(strConditonSign[temp->mConditon.iType],((C_RANDOM*)temp->mConditon.pParam)->fProbability);
		break;
	case CTriggerData::c_enmity_reach:
		sign.Format(strConditonSign[temp->mConditon.iType],((C_ENMITY_REACH*)temp->mConditon.pParam)->nPlayer,((C_ENMITY_REACH*)temp->mConditon.pParam)->nPlayerLevel);
		break;
	case CTriggerData::c_distance_reach:
		sign.Format(strConditonSign[temp->mConditon.iType],((C_DISTANCE_REACH*)temp->mConditon.pParam)->fDistance);
		break;
	case CTriggerData::c_attack_by_skill:
		sign.Format(strConditonSign[temp->mConditon.iType],((C_ATTACK_BY_SKILL*)temp->mConditon.pParam)->uID);
		break;
	case CTriggerData::c_var:
		sign.Format(strConditonSign[temp->mConditon.iType],((C_VAR*)temp->mConditon.pParam)->iID);
		break;
	case CTriggerData::c_constant:
		sign.Format(strConditonSign[temp->mConditon.iType],((C_CONSTANT*)temp->mConditon.pParam)->iValue);
		break;
	case CTriggerData::c_rank_level:
		sign.Format(strConditonSign[temp->mConditon.iType],((C_RANK_LEVEL*)temp->mConditon.pParam)->iValue);
		break;
	default:
		sign = strConditonSign[temp->mConditon.iType];
	}
	
	str +="(";
	if(temp->pLeft) str += TraverselTree(temp->pLeft);
	str +=sign;
	if(temp->pRight) str += TraverselTree(temp->pRight);
	str +=")";
	return str;
}

void CPolicyTriggerDlg::OnAddCondition() 
{
	// TODO: Add your control notification handler code here
	CTriggerData::_s_tree_item* root = m_pTriggerData->GetConditonRoot();
	CTriggerData::_s_tree_item *old = 0;
	if(root==0)
		root = new CTriggerData::_s_tree_item;
	else old = CTriggerData::CopyConditonTree(root);
	
	CConditionDlg dlg;
	dlg.m_pRoot = root;
	dlg.m_bIsRoot = true;


	if(IDOK == dlg.DoModal())
	{
		m_pTriggerData->SetConditionRoot(root);
		m_strProgram = TraverselTree(root);
		m_bIsChanged = true;
		if(old) delete old;
	}else 
	{
		delete root;
		m_pTriggerData->SetConditionRoot(old);
		m_strProgram = TraverselTree(old);
	}
	UpdateData(false);
}

void CPolicyTriggerDlg::OnDelCondition() 
{
	// TODO: Add your control notification handler code here
	/*
	int sel = m_listCondition.GetCurSel();
	if(sel!=-1) 
	{
		if(IDYES ==	AfxMessageBox("你确定要删除当前选中的条件吗?",MB_YESNO|MB_ICONQUESTION))
		{
			m_pTriggerData->DelCondition(sel);
			FreshCondition();
		}
	}*/
}

void CPolicyTriggerDlg::OnMoveUpCondition() 
{
	// TODO: Add your control notification handler code here
	/*
	int sel = m_listCondition.GetCurSel();
	if(sel == -1 || sel == 0) return;
	unsigned int iType1,iType2;
	void *pData1, *pData2;
	m_pTriggerData->GetCondition(sel,iType1,&pData1);
	m_pTriggerData->GetCondition(sel-1,iType2,&pData2);

	m_pTriggerData->SetCondition(sel,iType2,pData2);
	m_pTriggerData->SetCondition(sel-1,iType1, pData1);
	FreshCondition();
	m_listCondition.SetCurSel(sel-1);*/

}

void CPolicyTriggerDlg::OnMoveDownCondition() 
{
	/*
	// TODO: Add your control notification handler code here
		int sel = m_listCondition.GetCurSel();
	if(sel == -1 || sel > m_listCondition.GetCount()-2) return;
	unsigned int iType1,iType2;
	void *pData1,*pData2;
	m_pTriggerData->GetCondition(sel,iType1,&pData1);
	m_pTriggerData->GetCondition(sel+1,iType2,&pData2);

	m_pTriggerData->SetCondition(sel,iType2,pData2);
	m_pTriggerData->SetCondition(sel+1,iType1, pData1);
	FreshCondition();
	m_listCondition.SetCurSel(sel+1);
	*/
	
}

void CPolicyTriggerDlg::OnCheckProgramError() 
{
	// TODO: Add your control notification handler code here
	
}


void CPolicyTriggerDlg::OnChangeEditTriggerName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CPolicyTriggerDlg::OnCheckActive() 
{
	// TODO: Add your control notification handler code here
	m_bIsChanged = true;
	UpdateData(TRUE);
}

CString CPolicyTriggerDlg::GetTriggerName( unsigned int id)
{
	if(m_pCurrentPolicy)
	{
		int n = m_pCurrentPolicy->GetTriggerPtrNum();
		for( int i = 0; i < n; ++i)
		{
			CTriggerData *pData = m_pCurrentPolicy->GetTriggerPtr(i);
			if(pData->GetID() == id) return pData->GetName();
		}
	}
	return "错误的触发器ID";
}

CString GetTargetStr(void *pTarget)
{
	if(pTarget==NULL) return "没目标";
	CString msg;
	CTriggerData::_s_target *pTempTarget = (CTriggerData::_s_target *)pTarget;
	if(pTempTarget==0) return msg;  
	msg +=" [";

	CString mTargetList[CTriggerData::t_num] = 
	{
		"仇恨排名第一位",
		"仇恨排名第二位",
		"仇恨排名第二位及其以后所有对象中的随机一个",
		"最多hp",
		"最多mp",
		"最少hp",
		"职业的组合",
		"自己"
	};
	
	msg += mTargetList[pTempTarget->iType];
	switch(pTempTarget->iType)
	{
	case CTriggerData::t_occupation_list:
		break;
	}

	msg +="]";
	return msg;
}

void CPolicyTriggerDlg::OnDblclkListboxOperation() 
{
	// TODO: Add your control notification handler code here
	int sel = m_listOperation.GetCurSel();
	if(sel == -1) return;

	CPolicyOperationDlg dlg;
	dlg.m_pPolicyData = m_pCurrentPolicy;
	dlg.m_bModify = true;
	dlg.m_pOperationData = m_pTriggerData->GetOperaion(sel);
	if(IDOK==dlg.DoModal())
	{
		void *pTemp = NULL;
		COperationParam *pParam = dlg.GetNewOperation();
		switch(pParam->GetOperationType())
		{
		case CTriggerData::o_active_trigger:
			pTemp = malloc(sizeof(O_ACTIVE_TRIGGER));
			((O_ACTIVE_TRIGGER *)pTemp)->uID = ((COperationParam_Active_Trigger*)pParam)->m_Data.uID;
			break;
		case CTriggerData::o_attact:
			pTemp = malloc(sizeof(O_ATTACK_TYPE));
			((O_ATTACK_TYPE *)pTemp)->uType = ((COperationParam_Attack*)pParam)->m_Data.uType;
			break;
		case CTriggerData::o_create_timer:
			pTemp = malloc(sizeof(O_CREATE_TIMER));
			*((O_CREATE_TIMER *)pTemp) = ((COperationParam_Create_Timer*)pParam)->m_Data;
			break;
		case CTriggerData::o_set_global:
			pTemp = malloc(sizeof(O_SET_GLOBAL));
			*((O_SET_GLOBAL *)pTemp) = ((COperationParam_Set_Global*)pParam)->m_Data;
			break;
		case CTriggerData::o_revise_global:
			pTemp = malloc(sizeof(O_REVISE_GLOBAL));
			*((O_REVISE_GLOBAL *)pTemp) = ((COperationParam_Revise_Global*)pParam)->m_Data;
			break;
		case CTriggerData::o_assign_global:
			pTemp = malloc(sizeof(O_ASSIGN_GLOBAL));
			*((O_ASSIGN_GLOBAL *)pTemp) = ((COperationParam_Assign_Global*)pParam)->m_Data;
			break;
		case CTriggerData::o_summon:
			pTemp = malloc(sizeof(O_SUMMON));
			*((O_SUMMON *)pTemp) = ((COperationParam_Summon*)pParam)->m_Data;
			break;
		case CTriggerData::o_summon_mineral:
			pTemp = malloc(sizeof(O_SUMMON_MINERAL));
			*((O_SUMMON_MINERAL *)pTemp) = ((COperationParam_Summon_Mineral*)pParam)->m_Data;
			break;
		case CTriggerData::o_drop_item:
			pTemp = malloc(sizeof(O_DROP_ITEM));
			*((O_DROP_ITEM *)pTemp) = ((COperationParam_Drop_Item*)pParam)->m_Data;
			break;
		case CTriggerData::o_change_hate:	//改变仇恨值
		{
			pTemp = malloc(sizeof(O_CHANGE_HATE));
			*((O_CHANGE_HATE*)pTemp) = ((COperationParam_Change_Hate*)pParam)->m_Data;
			break;
		}
		case CTriggerData::o_kill_timer:
			pTemp = malloc(sizeof(O_KILL_TIMER));
			((O_KILL_TIMER *)pTemp)->uID = ((COperationParam_Kill_Timer*)pParam)->m_Data.uID;
			break;
		case CTriggerData::o_active_controller:
			pTemp = malloc(sizeof(O_ACTIVE_CONTROLLER));
			((O_ACTIVE_CONTROLLER *)pTemp)->uID = ((COperationParam_Active_Controller*)pParam)->m_Data.uID;
			((O_ACTIVE_CONTROLLER *)pTemp)->bStop = ((COperationParam_Active_Controller*)pParam)->m_Data.bStop;
			break;
		case CTriggerData::o_run_trigger:
			pTemp = malloc(sizeof(O_RUN_TRIGGER));
			((O_RUN_TRIGGER *)pTemp)->uID = ((COperationParam_Run_Trigger*)pParam)->m_Data.uID;
			break;
		case CTriggerData::o_stop_trigger:
			pTemp = malloc(sizeof(O_STOP_TRIGGER));
			((O_STOP_TRIGGER *)pTemp)->uID = ((COperationParam_Stop_Trigger*)pParam)->m_Data.uID;
			break;
		case CTriggerData::o_talk:
			pTemp = malloc(sizeof(O_TALK_TEXT));
			((O_TALK_TEXT *)pTemp)->szData = 0;
			((O_TALK_TEXT *)pTemp)->uSize = wcslen((WCHAR*)((COperationParam_Talk*)pParam)->m_Data.szData)*2 + 2;
			((O_TALK_TEXT *)pTemp)->szData = new unsigned short[((O_TALK_TEXT *)pTemp)->uSize];
			wcscpy((WCHAR*)((O_TALK_TEXT *)pTemp)->szData,(WCHAR*)((COperationParam_Talk*)pParam)->m_Data.szData);
			break;
		case CTriggerData::o_use_skill:
			pTemp = malloc(sizeof(O_USE_SKILL));
			((O_USE_SKILL *)pTemp)->uSkill = ((COperationParam_Use_Skill*)pParam)->m_Data.uSkill;
			((O_USE_SKILL *)pTemp)->uLevel = ((COperationParam_Use_Skill*)pParam)->m_Data.uLevel;
			break;
		case CTriggerData::o_use_range_skill:
			pTemp = malloc(sizeof(O_USE_RANGE_SKILL));
			((O_USE_RANGE_SKILL *)pTemp)->uSkill = ((COperationParam_Use_Range_Skill*)pParam)->m_Data.uSkill;
			((O_USE_RANGE_SKILL *)pTemp)->uLevel = ((COperationParam_Use_Range_Skill*)pParam)->m_Data.uLevel;
			((O_USE_RANGE_SKILL *)pTemp)->fRange = ((COperationParam_Use_Range_Skill*)pParam)->m_Data.fRange;
			break;
		case CTriggerData::o_trigger_task:
			pTemp = malloc(sizeof(O_TRIGGER_TASK));
			((O_TRIGGER_TASK *)pTemp)->uTaskID = ((COperationParam_Trigger_Task*)pParam)->m_Data.uTaskID;
			break;

		case CTriggerData::o_change_path:
			pTemp = malloc(sizeof(O_CHANGE_PATH));
			((O_CHANGE_PATH *)pTemp)->uPathID = ((COperationParam_Change_Path*)pParam)->m_Data.uPathID;
			((O_CHANGE_PATH *)pTemp)->iType = ((COperationParam_Change_Path*)pParam)->m_Data.iType;
			break;
		case CTriggerData::o_sneer_monster:
			pTemp = malloc(sizeof(O_SNEER_MONSTER));
			((O_SNEER_MONSTER *)pTemp)->fRange = ((COperationParam_Sneer_Monster*)pParam)->m_Data.fRange;
			break;
		case CTriggerData::o_start_event:
			pTemp = malloc(sizeof(O_START_EVENT));
			((O_START_EVENT *)pTemp)->iId = ((COperationParam_Start_Event*)pParam)->m_Data.iId;
		case CTriggerData::o_stop_event:
			pTemp = malloc(sizeof(O_STOP_EVENT));
			((O_STOP_EVENT *)pTemp)->iId = ((COperationParam_Stop_Event*)pParam)->m_Data.iId;
		}
		
		//Release old operation data
		if(dlg.m_pOperationData->mTarget.pParam) 
		{
			free(dlg.m_pOperationData->mTarget.pParam);
			dlg.m_pOperationData->mTarget.pParam = 0;
		}
		if(dlg.m_pOperationData->pParam)
		{
			if(dlg.m_pOperationData->iType==CTriggerData::o_talk)
				if(((O_TALK_TEXT*)dlg.m_pOperationData->pParam)->szData) delete ((O_TALK_TEXT*)dlg.m_pOperationData->pParam)->szData;
			free(dlg.m_pOperationData->pParam);
			dlg.m_pOperationData->pParam = 0;
		}
		//Set to new operation data
		if(m_pTriggerData) m_pTriggerData->SetOperation(sel,pParam->GetOperationType(),pTemp,&pParam->mTarget);
		m_bIsChanged = true;
		FreshList();
		if(dlg.GetNewOperation()) delete dlg.GetNewOperation();
	}else
	{
	}
	
}

void CPolicyTriggerDlg::OnRadioAttackEffect() 
{
	// TODO: Add your control notification handler code here
	m_RunCondition = CTriggerData::run_attack_effect;
}

void CPolicyTriggerDlg::OnRadioAttackNoeffect() 
{
	// TODO: Add your control notification handler code here
	m_RunCondition = CTriggerData::run_attack_noeffect;
}

void CPolicyTriggerDlg::SaveOldTree(TRIGGER_TREE *pTreeRoot,CTriggerData *pTrigger)
{
	if(pTreeRoot==0 || pTrigger==0) return;
	
	pTreeRoot->pTrigger = pTrigger;
	int n = pTrigger->GetOperaionNum();
	for( int i = 0; i < n; ++i)
	{
		CTriggerData::_s_operation *pOperation = pTrigger->GetOperaion(i);
		if(pOperation->iType==CTriggerData::o_run_trigger)
		{
			int index = m_pCurrentPolicy->GetIndex(((O_RUN_TRIGGER*)pOperation->pParam)->uID);
			if(index!=-1)
			{
				CTriggerData *pData = m_pCurrentPolicy->GetTriggerPtr(index);
				CTriggerData *pNewData = pData->CopyObject();
				ASSERT(pNewData);
				TRIGGER_TREE *pNewTree = new TRIGGER_TREE;
				ASSERT(pNewTree);
				pTreeRoot->listChild.push_back(pNewTree);
				SaveOldTree(pNewTree,pNewData);
			}else pTreeRoot->listChild.push_back(0);
		}
	}
}

void CPolicyTriggerDlg::StartRecord()
{
	m_pOldTree = new TRIGGER_TREE;
	if(m_pOldTree==0) return;
	CTriggerData *pNewData = m_pTriggerData->CopyObject();
	ASSERT(pNewData);
	SaveOldTree(m_pOldTree,pNewData);
}

void CPolicyTriggerDlg::RestoreTrigger(TRIGGER_TREE *pTreeRoot)
{
	if(pTreeRoot==0) return;
	
	int index = m_pCurrentPolicy->GetIndex(pTreeRoot->pTrigger->GetID());
	if(index!=-1)
	{
		CTriggerData *pData = m_pCurrentPolicy->GetTriggerPtr(index);
		if(pData) 
		{
			pData->Release();
			delete pData;
		}
		m_pCurrentPolicy->SetTriggerPtr(index,pTreeRoot->pTrigger->CopyObject());
	}
	
	int n = pTreeRoot->pTrigger->GetOperaionNum();
	int k = 0;
	for( int i = 0; i < n; ++i)
	{
		CTriggerData::_s_operation *pOperation = pTreeRoot->pTrigger->GetOperaion(i);
		if(pOperation->iType==CTriggerData::o_run_trigger)
			RestoreTrigger(pTreeRoot->listChild[k++]);
	}
}
