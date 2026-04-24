// PolicyOperationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ElementData.h"
#include "PolicyOperationDlg.h"
#include "PropertyList.h"
#include "AObject.h"
#include "Policy.h"
#include "OperationParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString strOperationList[CTriggerData::o_num] = 
{ "发起攻击","使用技能","说话","重置仇恨列表","执行一个新触发器",
"停止一个触发器","激活一个触发器","创建一个定时器","停止一个定时器",
"逃跑","调整仇恨到第一","调整仇恨度到最低",
"仇恨度减少50%",
"跳过后面的操作条目",
"激活产生怪物控制器",
"召唤",
"触发任务", 
"切换路径",
"自消失",
"嘲讽怪物",
"使用大范围技能",
"归位",
"设置全局变量",
"修正全局变量",
"全局变量赋值",
"召唤矿物",
"丢出物品",
"改变仇恨值",
"开始一个事件",
"停止一个事件",
};
CString strTargetList[CTriggerData::t_num] = { "仇恨排名第一位","仇恨排名第二位","仇恨排名第一位以后对象的随机一位","最多hp","最多mp","最少hp","职业的组合","自己"};
/////////////////////////////////////////////////////////////////////////////
// CPolicyOperationDlg dialog


CPolicyOperationDlg::CPolicyOperationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPolicyOperationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPolicyOperationDlg)
	m_bWuXia = FALSE;
	m_bYaoShou = FALSE;
	m_bYuLin = FALSE;
	m_bYuMang = FALSE;
	m_bFaShi = FALSE;
	m_bMeiLing = FALSE;
	m_bSengLv = FALSE;
	m_bYaoJing = FALSE;
	//}}AFX_DATA_INIT
	m_pList = NULL;
	m_pNewOperaion = NULL;
	m_pPolicyData = NULL;
	m_bModify = false;
}


void CPolicyOperationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPolicyOperationDlg)
	DDX_Control(pDX, IDC_COMBO_TARGET_TYPE, m_cbTargetType);
	DDX_Control(pDX, IDC_COMBO_OPERATION_TYPE, m_cbOperationType);
	DDX_Check(pDX, IDC_CHECK_WUXIA, m_bWuXia);
	DDX_Check(pDX, IDC_CHECK_YAOSHOU, m_bYaoShou);
	DDX_Check(pDX, IDC_CHECK_YULIN, m_bYuLin);
	DDX_Check(pDX, IDC_CHECK_YUMANG, m_bYuMang);
	DDX_Check(pDX, IDC_CHECK_FASHI, m_bFaShi);
	DDX_Check(pDX, IDC_CHECK_MEILING, m_bMeiLing);
	DDX_Check(pDX, IDC_CHECK_SENGLV, m_bSengLv);
	DDX_Check(pDX, IDC_CHECK_YAOJING, m_bYaoJing);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPolicyOperationDlg, CDialog)
	//{{AFX_MSG_MAP(CPolicyOperationDlg)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_OPERATION_TYPE, OnSelchangeComboOperationType)
	ON_CBN_SELCHANGE(IDC_COMBO_TARGET_TYPE, OnSelchangeComboTargetType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPolicyOperationDlg message handlers

int CPolicyOperationDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return 0;
}

BOOL CPolicyOperationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pList = new CPropertyList();
	m_pTemp = new ADynPropertyObject;
	CWnd *pWnd = GetDlgItem(IDC_LIST_OPERATION);
	CRect rc,rc1;
	pWnd->GetClientRect(&rc);

	int width = rc.Width();
	int length = rc.Height();

	pWnd->GetWindowRect(&rc);
	GetWindowRect(&rc1);
	rc.left = rc.left - rc1.left;
	rc.top = rc.top - rc1.top - 20;
	rc.right = rc.left + width;
	rc.bottom = rc.top + length;

	BOOL bResult = m_pList->Create("操作表", WS_CHILD | WS_VISIBLE, rc, this, 111);

	int i;
	//填充操作类型列表
	for( i = 0; i < CTriggerData::o_num; i++)
		m_cbOperationType.InsertString(i,strOperationList[i]);
	if(m_cbOperationType.GetCount()>0) m_cbOperationType.SetCurSel(0);

	for( i = 0; i < CTriggerData::t_num; i++)
		m_cbTargetType.InsertString(i,strTargetList[i]);
	if(m_cbTargetType.GetCount()>0) m_cbTargetType.SetCurSel(0);
	EnableCheckBox(false);
	if(m_bModify)
	{
		InitOperation();
	}
	else ChangeProperty(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPolicyOperationDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
	if(m_pList)
	{
		m_pList->CleanItems();
		m_pList->DestroyWindow();
		delete m_pList;
		if(m_pTemp) delete m_pTemp;
	}
	
}

void CPolicyOperationDlg::UpdateProperty(ADynPropertyObject *pProObj)
{
	if(m_pList && pProObj) m_pList->AttachDataObject(pProObj);
}

void CPolicyOperationDlg::OnSelchangeComboOperationType() 
{
	// TODO: Add your control notification handler code here
	int sel = m_cbOperationType.GetCurSel();
	if(sel!=-1)
	{
		CString szOp;
		m_cbOperationType.GetLBText(sel,szOp);
		ChangeProperty(sel);
	}
}

void CPolicyOperationDlg::ChangeProperty(int iOpType)
{
	UpdateProperty(m_pTemp);
	if(m_pNewOperaion) 
	{
		if(m_pNewOperaion->mTarget.pParam) free(m_pNewOperaion->mTarget.pParam);
		delete m_pNewOperaion;
	}
	m_pNewOperaion = NULL;
	switch(iOpType) 
		{
		case CTriggerData::o_attact:
			m_pNewOperaion = new COperationParam_Attack;
			break;
		case CTriggerData::o_use_skill:
			m_pNewOperaion = new COperationParam_Use_Skill;
			break;
		case CTriggerData::o_use_range_skill:
			m_pNewOperaion = new COperationParam_Use_Range_Skill;
			break;
		case CTriggerData::o_trigger_task:
			m_pNewOperaion = new COperationParam_Trigger_Task;
			break;
		case CTriggerData::o_change_path:
			m_pNewOperaion = new COperationParam_Change_Path;
			break;
		case CTriggerData::o_sneer_monster:
			m_pNewOperaion = new COperationParam_Sneer_Monster;
			break;
		case CTriggerData::o_talk:
			m_pNewOperaion = new COperationParam_Talk;
			break;
		case CTriggerData::o_reset_hate_list:
			m_pNewOperaion = new COperationParam_Reset_hates;
			break;
		case CTriggerData::o_run_trigger:
			m_pNewOperaion = new COperationParam_Run_Trigger;
			break;
		case CTriggerData::o_stop_trigger:
			m_pNewOperaion = new COperationParam_Stop_Trigger;
			break;
		case CTriggerData::o_active_trigger:
			m_pNewOperaion = new COperationParam_Active_Trigger;
			break;
		case CTriggerData::o_create_timer:
			m_pNewOperaion = new COperationParam_Create_Timer;
			break;
		case CTriggerData::o_summon:
			m_pNewOperaion = new COperationParam_Summon;
			break;
		case CTriggerData::o_summon_mineral:
			m_pNewOperaion = new COperationParam_Summon_Mineral;
			break;
		case CTriggerData::o_drop_item:
			m_pNewOperaion = new COperationParam_Drop_Item;
			break;
		case CTriggerData::o_change_hate:
		{
			m_pNewOperaion = new COperationParam_Change_Hate;
			break;
		}
		case CTriggerData::o_start_event:
		{
			m_pNewOperaion = new COperationParam_Start_Event;
			break;
		}
		case CTriggerData::o_stop_event:
		{
			m_pNewOperaion = new COperationParam_Stop_Event;
			break;
		}
		case CTriggerData::o_kill_timer:
			m_pNewOperaion = new COperationParam_Kill_Timer;
			break;
		case CTriggerData::o_active_controller:
			m_pNewOperaion = new COperationParam_Active_Controller;
			break;
		case CTriggerData::o_flee:
			m_pNewOperaion = new COperationParam_Flee;
			break;
		case CTriggerData::o_dispear:
			m_pNewOperaion = new COperationParam_Dispear;
			break;
		case CTriggerData::o_reset:
			m_pNewOperaion = new COperationParam_Reset;
			break;
		case CTriggerData::o_set_hate_to_first:
			m_pNewOperaion = new COperationParam_SetHateToFirst;
			break;
		case CTriggerData::o_set_hate_to_last:
			m_pNewOperaion = new COperationParam_SetHateToLast;
			break;
		case CTriggerData::o_set_hate_fifty_percent:
			m_pNewOperaion = new COperationParam_SetHateFiftyPercent;
			break;
		case CTriggerData::o_skip_operation:
			m_pNewOperaion = new COperationParam_SkipOperation;
			break;
		case CTriggerData::o_set_global:
			m_pNewOperaion = new COperationParam_Set_Global;
			break;
		case CTriggerData::o_revise_global:
			m_pNewOperaion = new COperationParam_Revise_Global;
			break;
		case CTriggerData::o_assign_global:
			m_pNewOperaion = new COperationParam_Assign_Global;
			break;
		default:
			MessageBox("不认识的操作类型!");
	}
	
	if(m_pNewOperaion) 
	{
		m_pNewOperaion->m_pPolicyData = m_pPolicyData;
		m_pNewOperaion->BuildProperty();
		UpdateProperty(m_pNewOperaion->m_pProperty);
	}
}

void CPolicyOperationDlg::InitOperation()
{
	if(m_pOperationData==NULL) return; 
	if(m_pNewOperaion) 
	{
		if(m_pNewOperaion->mTarget.pParam) free(m_pNewOperaion->mTarget.pParam);
		delete m_pNewOperaion;
	}
	
	m_cbOperationType.SetCurSel(m_pOperationData->iType);
	m_cbTargetType.SetCurSel(m_pOperationData->mTarget.iType);
	m_pNewOperaion = NULL;
	switch(m_pOperationData->iType) 
		{
		case CTriggerData::o_attact:
			m_pNewOperaion = new COperationParam_Attack;
			((COperationParam_Attack*)m_pNewOperaion)->m_Data = *((O_ATTACK_TYPE*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_use_skill:
			m_pNewOperaion = new COperationParam_Use_Skill;
			((COperationParam_Use_Skill*)m_pNewOperaion)->m_Data = *((O_USE_SKILL*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_use_range_skill:
			m_pNewOperaion = new COperationParam_Use_Range_Skill;
			((COperationParam_Use_Range_Skill*)m_pNewOperaion)->m_Data = *((O_USE_RANGE_SKILL*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_trigger_task:
			m_pNewOperaion = new COperationParam_Trigger_Task;
			((COperationParam_Trigger_Task*)m_pNewOperaion)->m_Data = *((O_TRIGGER_TASK*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_change_path:
			m_pNewOperaion = new COperationParam_Change_Path;
			((COperationParam_Change_Path*)m_pNewOperaion)->m_Data = *((O_CHANGE_PATH*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_sneer_monster:
			m_pNewOperaion = new COperationParam_Sneer_Monster;
			((COperationParam_Sneer_Monster*)m_pNewOperaion)->m_Data = *((O_SNEER_MONSTER*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_talk:
			m_pNewOperaion = new COperationParam_Talk;
			//memcpy(((COperationParam_Talk*)m_pNewOperaion)->m_Data.szData,((O_TALK_TEXT*)m_pOperationData->pParam)->szData,sizeof(unsigned short)*((O_TALK_TEXT*)m_pOperationData->pParam)->uSize);
			wcscpy((WCHAR*)((COperationParam_Talk*)m_pNewOperaion)->m_Data.szData,(WCHAR*)((O_TALK_TEXT*)m_pOperationData->pParam)->szData);
			break;
		case CTriggerData::o_reset_hate_list:
			m_pNewOperaion = new COperationParam_Reset_hates;
			break;
		case CTriggerData::o_run_trigger:
			m_pNewOperaion = new COperationParam_Run_Trigger;
			((COperationParam_Run_Trigger*)m_pNewOperaion)->m_Data = *((O_RUN_TRIGGER*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_stop_trigger:
			m_pNewOperaion = new COperationParam_Stop_Trigger;
			((COperationParam_Stop_Trigger*)m_pNewOperaion)->m_Data = *((O_STOP_TRIGGER*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_active_trigger:
			m_pNewOperaion = new COperationParam_Active_Trigger;
			((COperationParam_Active_Trigger*)m_pNewOperaion)->m_Data = *((O_ACTIVE_TRIGGER*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_create_timer:
			m_pNewOperaion = new COperationParam_Create_Timer;
			((COperationParam_Create_Timer*)m_pNewOperaion)->m_Data = *((O_CREATE_TIMER*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_summon:
			m_pNewOperaion = new COperationParam_Summon;
			((COperationParam_Summon*)m_pNewOperaion)->m_Data = *((O_SUMMON*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_summon_mineral:
			m_pNewOperaion = new COperationParam_Summon_Mineral;
			((COperationParam_Summon_Mineral*)m_pNewOperaion)->m_Data = *((O_SUMMON_MINERAL*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_drop_item:
			m_pNewOperaion = new COperationParam_Drop_Item;
			((COperationParam_Drop_Item*)m_pNewOperaion)->m_Data = *((O_DROP_ITEM*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_change_hate:
		{
			m_pNewOperaion = new COperationParam_Change_Hate;
			((COperationParam_Change_Hate*)m_pNewOperaion)->m_Data = *((O_CHANGE_HATE*)m_pOperationData->pParam);
			break;
		}
		case CTriggerData::o_start_event:
		{
			m_pNewOperaion = new COperationParam_Start_Event;
			((COperationParam_Start_Event*)m_pNewOperaion)->m_Data = *((O_START_EVENT*)m_pOperationData->pParam);
			break;
		}
		case CTriggerData::o_stop_event:
		{
			m_pNewOperaion = new COperationParam_Stop_Event;
			((COperationParam_Stop_Event*)m_pNewOperaion)->m_Data = *((O_STOP_EVENT*)m_pOperationData->pParam);
			break;
		}
		case CTriggerData::o_kill_timer:
			m_pNewOperaion = new COperationParam_Kill_Timer;
				((COperationParam_Kill_Timer*)m_pNewOperaion)->m_Data = *((O_KILL_TIMER*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_active_controller:
			m_pNewOperaion = new COperationParam_Active_Controller;
				((COperationParam_Active_Controller*)m_pNewOperaion)->m_Data = *((O_ACTIVE_CONTROLLER*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_flee:
			m_pNewOperaion = new COperationParam_Flee;
			break;
		case CTriggerData::o_dispear:
			m_pNewOperaion = new COperationParam_Dispear;
			break;
		case CTriggerData::o_reset:
			m_pNewOperaion = new COperationParam_Reset;
			break;
		case CTriggerData::o_set_hate_to_first:
			m_pNewOperaion = new COperationParam_SetHateToFirst;
			break;
		case CTriggerData::o_set_hate_to_last:
			m_pNewOperaion = new COperationParam_SetHateToLast;
			break;
		case CTriggerData::o_set_hate_fifty_percent:
			m_pNewOperaion = new COperationParam_SetHateFiftyPercent;
			break;
		case CTriggerData::o_skip_operation:
			m_pNewOperaion = new COperationParam_SkipOperation;
			break;
		case CTriggerData::o_set_global:
			m_pNewOperaion = new COperationParam_Set_Global;
			((COperationParam_Set_Global*)m_pNewOperaion)->m_Data = *((O_SET_GLOBAL*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_revise_global:
			m_pNewOperaion = new COperationParam_Revise_Global;
			((COperationParam_Revise_Global*)m_pNewOperaion)->m_Data = *((O_REVISE_GLOBAL*)m_pOperationData->pParam);
			break;
		case CTriggerData::o_assign_global:
			m_pNewOperaion = new COperationParam_Assign_Global;
			((COperationParam_Assign_Global*)m_pNewOperaion)->m_Data = *((O_ASSIGN_GLOBAL*)m_pOperationData->pParam);
			break;
		default:
			MessageBox("不认识的操作类型!");
	}
	
	if(m_pNewOperaion) 
	{
		m_pNewOperaion->mTarget.iType = m_pOperationData->mTarget.iType;
		m_pNewOperaion->m_pPolicyData = m_pPolicyData;
		m_pNewOperaion->BuildProperty();
		UpdateProperty(m_pNewOperaion->m_pProperty);
	}

	if(m_pOperationData->mTarget.iType == CTriggerData::t_occupation_list)
	{
		unsigned int uBit = ((T_OCCUPATION*)m_pOperationData->mTarget.pParam)->uBit;
		
		if(uBit & CTriggerData::o_wuxia) m_bWuXia = TRUE;
		if(uBit & CTriggerData::o_fashi) m_bFaShi = TRUE;
		if(uBit & CTriggerData::o_senglv) m_bSengLv = TRUE;
		if(uBit & CTriggerData::o_yaojing) m_bYaoJing = TRUE;
		if(uBit & CTriggerData::o_yaoshou) m_bYaoShou = TRUE;
		if(uBit & CTriggerData::o_meiling) m_bMeiLing = TRUE;
		if(uBit & CTriggerData::o_yumang) m_bYuMang = TRUE;
		if(uBit & CTriggerData::o_yuling) m_bYuLin = TRUE;
		UpdateData(false);
	}

	OnSelchangeComboTargetType();
}

void CPolicyOperationDlg::OnOK()
{
	
	int sel = m_cbTargetType.GetCurSel();
	if(sel==-1) return;
	UpdateData(true);
	if(m_pNewOperaion==NULL) return;
	
	m_pNewOperaion->mTarget.iType = sel;
	if(m_pNewOperaion->mTarget.pParam) free(m_pNewOperaion->mTarget.pParam);
	
	if(sel==CTriggerData::t_occupation_list)
	{
		T_OCCUPATION *pNewData = (T_OCCUPATION*)malloc(sizeof(T_OCCUPATION));
		pNewData->uBit = 0;
		if(m_bWuXia) pNewData->uBit |=CTriggerData::o_wuxia;
		if(m_bFaShi) pNewData->uBit |=CTriggerData::o_fashi;
		if(m_bYaoShou) pNewData->uBit |=CTriggerData::o_yaoshou;
		if(m_bYuLin) pNewData->uBit |=CTriggerData::o_yuling;
		if(m_bYuMang) pNewData->uBit |=CTriggerData::o_yumang;
		if(m_bYaoJing) pNewData->uBit |=CTriggerData::o_yaojing;
		if(m_bMeiLing) pNewData->uBit |=CTriggerData::o_meiling;
		if(m_bSengLv) pNewData->uBit |=CTriggerData::o_senglv;
		m_pNewOperaion->mTarget.pParam = pNewData;
	}else m_pNewOperaion->mTarget.pParam = 0;
	
	if(m_pList) m_pList->UpdateData(true);
	if(m_pNewOperaion) m_pNewOperaion->UpdateProperty(true);
	CDialog::OnOK();
}

void CPolicyOperationDlg::OnCancel()
{
	if(m_pList)
	{
		m_pList->CleanItems();
		m_pList->DestroyWindow();
		delete m_pList;
		if(m_pTemp) delete m_pTemp;
		m_pList = 0;
		m_pTemp = 0;
	}
	
	if(m_pNewOperaion) 
	{
		if(m_pNewOperaion->mTarget.pParam) free(m_pNewOperaion->mTarget.pParam);
		delete m_pNewOperaion;
	}
	CDialog::OnCancel();
}

void CPolicyOperationDlg::OnSelchangeComboTargetType() 
{
	// TODO: Add your control notification handler code here
	int sel = m_cbTargetType.GetCurSel();
	if(sel!=-1)
	{
		if(sel==CTriggerData::t_occupation_list)
		{
			EnableCheckBox(true);
			return;
		}
	}
	EnableCheckBox(false);
}

void CPolicyOperationDlg::OnButtonAddTarget() 
{
	// TODO: Add your control notification handler code here
}

void CPolicyOperationDlg::OnButtonDelTarget() 
{

}

void CPolicyOperationDlg::FreshTargetList()
{
}

void CPolicyOperationDlg::EnableCheckBox(bool bEnable)
{
	CWnd *pWnd = GetDlgItem(IDC_CHECK_WUXIA);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_CHECK_FASHI);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_CHECK_YAOSHOU);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_CHECK_YULIN);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_CHECK_YUMANG);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_CHECK_SENGLV);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_CHECK_YAOJING);
	pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_CHECK_MEILING);
	pWnd->EnableWindow(bEnable);
}
