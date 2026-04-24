// ConditionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ElementData.h"
#include "ConditionDlg.h"
#include "Policy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString strConditonList[CTriggerData::c_num] = {"定时器到时(只能单独使用)","血量少于百分比","战斗开始(只能单独使用)","随机","杀死玩家(只能单独使用)","非","或","与","怪物死亡","到达路径终点","仇恨表达到指定人数","偏离中心位置","加","减","乘","除","大于","小于","等于","变量","常量","人物等级排行榜","怪物出生","被指定技能攻击"};
CString strConditonSign[CTriggerData::c_num] = {"Timer(%d)","HpLess(%f)","StartAttack()","Random(%f)","KillPlayer()","!","||","&&","Died()","Path_EndPoint(%d)","Enmity_Reach(%d,%d)","Distance_Reach(%f)","+","-","*","/",">","<","==","var[%d]","%d","RankLevel(%d)","Born()","Attack_Skill[%d]"};
/////////////////////////////////////////////////////////////////////////////
// CConditionDlg dialog

CConditionDlg::CConditionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConditionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConditionDlg)
	m_fConditionParam = 0;
	m_iConditionParam = 0;
	m_strLeft = _T("");
	m_strRight = _T("");
	//}}AFX_DATA_INIT
	m_uConditionFunc = 0;
	m_pRoot = NULL;
	m_bIsRoot = false;
	bModify = false;
}


void CConditionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConditionDlg)
	DDX_Control(pDX, IDC_COMBO_FUNC, m_cbFunc);
	DDX_Text(pDX, IDC_EDIT_PARAM_FLOAT, m_fConditionParam);
	DDX_Text(pDX, IDC_EDIT_PARAM_INT, m_iConditionParam);
	DDX_Text(pDX, IDC_EDIT_SUB1, m_strLeft);
	DDX_Text(pDX, IDC_EDIT_SUB2, m_strRight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConditionDlg, CDialog)
	//{{AFX_MSG_MAP(CConditionDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNC, OnSelchangeComboFunc)
	ON_BN_CLICKED(IDC_BUTTON_SUB1, OnButtonSub1)
	ON_BN_CLICKED(IDC_BUTTON_SUB2, OnButtonSub2)
	ON_EN_CHANGE(IDC_EDIT_PARAM_FLOAT, OnChangeEditParamFloat)
	ON_EN_CHANGE(IDC_EDIT_PARAM_INT, OnChangeEditParamInt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConditionDlg message handlers

void CConditionDlg::OnSelchangeComboFunc() 
{
	// TODO: Add your control notification handler code here
	FreshInterface();
}

void CConditionDlg::FreshInterface()
{
	CWnd *pParamI = GetDlgItem(IDC_EDIT_PARAM_INT);
	CWnd *pParamF  = GetDlgItem(IDC_EDIT_PARAM_FLOAT);
	CWnd *pButton1 = GetDlgItem(IDC_BUTTON_SUB1);
	CWnd *pButton2 = GetDlgItem(IDC_BUTTON_SUB2);
	pButton1->EnableWindow(false);
	pButton2->EnableWindow(false);
	CConditionDlg dlg;
	int sel = m_cbFunc.GetCurSel();
	if(sel==-1)
	{
		pParamI->ShowWindow(SW_HIDE);
		pParamF->ShowWindow(SW_HIDE);
	}else
	{
		m_uConditionFunc = sel;
		switch(sel) 
		{
		case CTriggerData::c_plus:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_HIDE);
			pButton1->EnableWindow(true);
			pButton2->EnableWindow(true);
			break;
		case CTriggerData::c_minus:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_HIDE);
			pButton1->EnableWindow(true);
			pButton2->EnableWindow(true);
			break;
		case CTriggerData::c_multiply:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_HIDE);
			pButton1->EnableWindow(true);
			pButton2->EnableWindow(true);
			break;
		case CTriggerData::c_divide:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_HIDE);
			pButton1->EnableWindow(true);
			pButton2->EnableWindow(true);
			break;
		case CTriggerData::c_great:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_HIDE);
			pButton1->EnableWindow(true);
			pButton2->EnableWindow(true);
			break;
		case CTriggerData::c_less:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_HIDE);
			pButton1->EnableWindow(true);
			pButton2->EnableWindow(true);
			break;
		case CTriggerData::c_equ:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_HIDE);
			pButton1->EnableWindow(true);
			pButton2->EnableWindow(true);
			break;
		case CTriggerData::c_var:
			pParamI->ShowWindow(SW_SHOW);
			pParamF->ShowWindow(SW_HIDE);
			pButton1->EnableWindow(false);
			pButton2->EnableWindow(false);
			break;
		case CTriggerData::c_constant:
			pParamI->ShowWindow(SW_SHOW);
			pParamF->ShowWindow(SW_HIDE);
			pButton1->EnableWindow(false);
			pButton2->EnableWindow(false);
			break;
		case CTriggerData::c_rank_level:
			pParamI->ShowWindow(SW_SHOW);
			pParamF->ShowWindow(SW_HIDE);
			pButton1->EnableWindow(false);
			pButton2->EnableWindow(false);
			break;

		case CTriggerData::c_and:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_HIDE);
			pButton1->EnableWindow(true);
			pButton2->EnableWindow(true);
			break;
		case CTriggerData::c_not:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_HIDE);
			pButton2->EnableWindow(true);
			break;
		case CTriggerData::c_or:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_HIDE);
			pButton1->EnableWindow(true);
			pButton2->EnableWindow(true);
			break;
		case CTriggerData::c_hp_less:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_SHOW);
			if(m_pRoot->mConditon.pParam)
				m_fConditionParam = ((C_HP_LESS*)m_pRoot->mConditon.pParam)->fPercent;
			break;
		case CTriggerData::c_kill_player:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_HIDE);
			break;
		case CTriggerData::c_born:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_HIDE);
			break;
		case CTriggerData::c_died:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_HIDE);
			break;
		case CTriggerData::c_random:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_SHOW);
			if(m_pRoot->mConditon.pParam)
				m_fConditionParam = ((C_RANDOM*)m_pRoot->mConditon.pParam)->fProbability;
			break;
		case CTriggerData::c_start_attack:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_HIDE);
			break;
		case CTriggerData::c_time_come:
			pParamI->ShowWindow(SW_SHOW);
			pParamF->ShowWindow(SW_HIDE);
			if(m_pRoot->mConditon.pParam)
				m_iConditionParam = ((C_TIME_COME*)m_pRoot->mConditon.pParam)->uID;
			break;
		case CTriggerData::c_path_end_point:
			pParamI->ShowWindow(SW_SHOW);
			pParamF->ShowWindow(SW_HIDE);
			if(m_pRoot->mConditon.pParam)
				m_iConditionParam = ((C_PATH_END_POINT*)m_pRoot->mConditon.pParam)->uID;
			break;
		case CTriggerData::c_enmity_reach:
			pParamI->ShowWindow(SW_SHOW);
			pParamF->ShowWindow(SW_SHOW);
			if(m_pRoot->mConditon.pParam)
			{
				m_iConditionParam = ((C_ENMITY_REACH*)m_pRoot->mConditon.pParam)->nPlayer;
				m_fConditionParam = ((C_ENMITY_REACH*)m_pRoot->mConditon.pParam)->nPlayerLevel;
			}
			break;
		case CTriggerData::c_distance_reach:
			pParamI->ShowWindow(SW_HIDE);
			pParamF->ShowWindow(SW_SHOW);
			if(m_pRoot->mConditon.pParam)
				m_fConditionParam = ((C_DISTANCE_REACH*)m_pRoot->mConditon.pParam)->fDistance;
			break;
		case CTriggerData::c_attack_by_skill:
			pParamI->ShowWindow(SW_SHOW);
			pParamF->ShowWindow(SW_HIDE);
			if(m_pRoot->mConditon.pParam)
				m_iConditionParam = ((C_ATTACK_BY_SKILL*)m_pRoot->mConditon.pParam)->uID;
			break;

		}
		UpdateData(false);
	}	
}

BOOL CConditionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for( int i = 0; i < CTriggerData::c_num; i++)
	{
		m_cbFunc.InsertString(i,strConditonList[i]);
	}
	m_cbFunc.SetCurSel(m_pRoot->mConditon.iType);
	FreshInterface();
	
	if(m_pRoot)
	{	
		m_strLeft = TraverselTree(m_pRoot->pLeft);
		m_strRight = TraverselTree(m_pRoot->pRight);
	}
	
	CRect rc;
	GetWindowRect(&rc);
	rc.left +=20;
	rc.top  += 20;
	rc.right += 20;
	rc.bottom +=20;
	MoveWindow(rc);
	OnSelchangeComboFunc();

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConditionDlg::DisableConditon2(bool bDisable)
{
}

bool CConditionDlg::Check(CTriggerData::_s_tree_item* left, CTriggerData::_s_tree_item* right)
{
	if(m_uConditionFunc == CTriggerData::c_plus ||
	   m_uConditionFunc == CTriggerData::c_minus ||
	   m_uConditionFunc == CTriggerData::c_multiply ||
	   m_uConditionFunc == CTriggerData::c_divide ||
	   m_uConditionFunc == CTriggerData::c_great ||
	   m_uConditionFunc == CTriggerData::c_less ||
	   m_uConditionFunc == CTriggerData::c_equ)
	   {
			if(left->mConditon.iType == CTriggerData::c_plus || 
				left->mConditon.iType == CTriggerData::c_minus || 
				left->mConditon.iType == CTriggerData::c_multiply || 
				left->mConditon.iType == CTriggerData::c_divide ||
				left->mConditon.iType == CTriggerData::c_var || 
				left->mConditon.iType == CTriggerData::c_constant||
				left->mConditon.iType == CTriggerData::c_rank_level)
			{
				if(right->mConditon.iType == CTriggerData::c_plus || 
				right->mConditon.iType == CTriggerData::c_minus || 
				right->mConditon.iType == CTriggerData::c_multiply || 
				right->mConditon.iType == CTriggerData::c_divide ||
				right->mConditon.iType == CTriggerData::c_var || 
				right->mConditon.iType == CTriggerData::c_constant||
				right->mConditon.iType == CTriggerData::c_rank_level)
					return true;
			}
	   }else if( m_uConditionFunc == CTriggerData::c_or ||
				 m_uConditionFunc == CTriggerData::c_and)
	   {
			if(left->mConditon.iType == CTriggerData::c_plus || 
				left->mConditon.iType == CTriggerData::c_minus || 
				left->mConditon.iType == CTriggerData::c_multiply || 
				left->mConditon.iType == CTriggerData::c_divide ||
				left->mConditon.iType == CTriggerData::c_var || 
				left->mConditon.iType == CTriggerData::c_constant||
				left->mConditon.iType == CTriggerData::c_rank_level)
					return false;
			
				if(right->mConditon.iType == CTriggerData::c_plus || 
				right->mConditon.iType == CTriggerData::c_minus || 
				right->mConditon.iType == CTriggerData::c_multiply || 
				right->mConditon.iType == CTriggerData::c_divide ||
				right->mConditon.iType == CTriggerData::c_var || 
				right->mConditon.iType == CTriggerData::c_constant||
				right->mConditon.iType == CTriggerData::c_rank_level)
					return false;

				return true;
	   }else if(m_uConditionFunc == CTriggerData::c_not)
	   {
		   if(right->mConditon.iType == CTriggerData::c_plus || 
			   right->mConditon.iType == CTriggerData::c_minus || 
			   right->mConditon.iType == CTriggerData::c_multiply || 
			   right->mConditon.iType == CTriggerData::c_divide ||
			   right->mConditon.iType == CTriggerData::c_var || 
			   right->mConditon.iType == CTriggerData::c_constant||
			   right->mConditon.iType == CTriggerData::c_rank_level)
			   return false;
		   
		   return true;
	   }

	return false;
}

void CConditionDlg::OnOK()
{
	UpdateData(true);
	switch(m_uConditionFunc) 
	{
	case CTriggerData::c_plus:
	case CTriggerData::c_minus:
	case CTriggerData::c_multiply:
	case CTriggerData::c_divide:
	case CTriggerData::c_great:
	case CTriggerData::c_less:
	case CTriggerData::c_equ:
	case CTriggerData::c_and:
	case CTriggerData::c_or:
		if(m_pRoot->pLeft==0) 
		{
			MessageBox("条件1不能为空!");
			return;
		}
		if(m_pRoot->pRight==0) 
		{
			MessageBox("条件2不能为空!");
			return;
		}
		
		if(!Check(m_pRoot->pLeft,m_pRoot->pRight))
		{
			MessageBox("逻辑错误!");
			return;
		}
		
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		break;
	case CTriggerData::c_not:
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight==0) 
		{
			MessageBox("条件2不能为空!");
			return;
		}
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		break;

	case CTriggerData::c_hp_less:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		m_pRoot->mConditon.pParam = malloc(sizeof(C_HP_LESS));
		((C_HP_LESS*)m_pRoot->mConditon.pParam)->fPercent = m_fConditionParam;
		break;
	case CTriggerData::c_var:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		m_pRoot->mConditon.pParam = malloc(sizeof(C_VAR));
		((C_VAR*)m_pRoot->mConditon.pParam)->iID = m_iConditionParam;
		break;
	case CTriggerData::c_constant:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		m_pRoot->mConditon.pParam = malloc(sizeof(C_RANK_LEVEL));
		((C_RANK_LEVEL*)m_pRoot->mConditon.pParam)->iValue = m_iConditionParam;
		break;
	case CTriggerData::c_rank_level:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		m_pRoot->mConditon.pParam = malloc(sizeof(C_CONSTANT));
		((C_CONSTANT*)m_pRoot->mConditon.pParam)->iValue = m_iConditionParam;
		break;
	case CTriggerData::c_kill_player:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		break;
	case CTriggerData::c_born:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		break;
	case CTriggerData::c_died:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		break;
	case CTriggerData::c_random:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		m_pRoot->mConditon.pParam = malloc(sizeof(C_RANDOM));
		((C_RANDOM*)m_pRoot->mConditon.pParam)->fProbability = m_fConditionParam;
		break;
	case CTriggerData::c_start_attack:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		break;
	case CTriggerData::c_time_come:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		m_pRoot->mConditon.pParam = malloc(sizeof(C_TIME_COME));
		((C_TIME_COME*)m_pRoot->mConditon.pParam)->uID = m_iConditionParam;
		break;
	case CTriggerData::c_path_end_point:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		m_pRoot->mConditon.pParam = malloc(sizeof(C_PATH_END_POINT));
		((C_PATH_END_POINT*)m_pRoot->mConditon.pParam)->uID = m_iConditionParam;
		break;
	case CTriggerData::c_enmity_reach:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		m_pRoot->mConditon.pParam = malloc(sizeof(C_ENMITY_REACH));
		((C_ENMITY_REACH*)m_pRoot->mConditon.pParam)->nPlayer = m_iConditionParam;
		((C_ENMITY_REACH*)m_pRoot->mConditon.pParam)->nPlayerLevel = m_fConditionParam;
		break;
	case CTriggerData::c_distance_reach:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		m_pRoot->mConditon.pParam = malloc(sizeof(C_DISTANCE_REACH));
		((C_DISTANCE_REACH*)m_pRoot->mConditon.pParam)->fDistance = m_fConditionParam;
		break;
	case CTriggerData::c_attack_by_skill:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		m_pRoot->mConditon.pParam = malloc(sizeof(C_ATTACK_BY_SKILL));
		((C_ATTACK_BY_SKILL*)m_pRoot->mConditon.pParam)->uID = m_iConditionParam;
		break;
	default:
		if(m_pRoot->mConditon.pParam) free(m_pRoot->mConditon.pParam);
		m_pRoot->mConditon.pParam = NULL;
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = 0;
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = 0;
		break;
	}
	
	if(m_bIsRoot) 
	{
		if(m_uConditionFunc == CTriggerData::c_plus||
			m_uConditionFunc == CTriggerData::c_minus||
			m_uConditionFunc == CTriggerData::c_multiply||
			m_uConditionFunc == CTriggerData::c_divide||
			m_uConditionFunc == CTriggerData::c_var||
			m_uConditionFunc == CTriggerData::c_constant||
			m_uConditionFunc == CTriggerData::c_rank_level)
		{
			MessageBox("逻辑错误!");
			return;
		}
	}
	
	m_pRoot->mConditon.iType = m_uConditionFunc;
	
	CDialog::OnOK();
}

void CConditionDlg::OnCancel()
{
	CDialog::OnCancel();
}




void CConditionDlg::OnButtonSub1() 
{
	// TODO: Add your control notification handler code here
	CConditionDlg dlg;
	CTriggerData::_s_tree_item *pOld = CTriggerData::CopyConditonTree(m_pRoot->pLeft);
	if(m_pRoot->pLeft==0) m_pRoot->pLeft = new CTriggerData::_s_tree_item;
	dlg.m_pRoot = m_pRoot->pLeft;
	if(IDOK==dlg.DoModal())
	{
		if(pOld) delete pOld;
	}else
	{
		if(m_pRoot->pLeft) delete m_pRoot->pLeft;
		m_pRoot->pLeft = pOld; 
	}
	m_strLeft = TraverselTree(m_pRoot->pLeft);
	m_strRight = TraverselTree(m_pRoot->pRight);
	UpdateData(false);
}

void CConditionDlg::OnButtonSub2() 
{
	CConditionDlg dlg;
	// TODO: Add your control notification handler code here
	CTriggerData::_s_tree_item *pOld = CTriggerData::CopyConditonTree(m_pRoot->pRight);
	if(m_pRoot->pRight==0) m_pRoot->pRight = new CTriggerData::_s_tree_item;
	dlg.m_pRoot = m_pRoot->pRight;
	
	dlg.bModify = bModify;
	if(IDOK==dlg.DoModal())
	{
		if(pOld) delete pOld;
	}else
	{
		if(m_pRoot->pRight) delete m_pRoot->pRight;
		m_pRoot->pRight = pOld; 
	}
	m_strLeft = TraverselTree(m_pRoot->pLeft);
	m_strRight = TraverselTree(m_pRoot->pRight);
	UpdateData(false);
}

CString CConditionDlg::TraverselTree( void *pTree)
{
	if(pTree==0) return "";
	CTriggerData::_s_tree_item * temp = (CTriggerData::_s_tree_item*)pTree;
	CString str;
	CString sign;
	switch(temp->mConditon.iType) 
	{
	case CTriggerData::c_time_come:
		sign.Format(strConditonSign[temp->mConditon.iType],((C_TIME_COME*)temp->mConditon.pParam)->uID);
		break;
	case CTriggerData::c_hp_less:
		sign.Format(strConditonSign[temp->mConditon.iType],((C_HP_LESS*)temp->mConditon.pParam)->fPercent);
		break;
	case CTriggerData::c_random:
		sign.Format(strConditonSign[temp->mConditon.iType],((C_RANDOM*)temp->mConditon.pParam)->fProbability);
		break;
	case CTriggerData::c_path_end_point:
		sign.Format(strConditonSign[temp->mConditon.iType],((C_PATH_END_POINT*)temp->mConditon.pParam)->uID);
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

void CConditionDlg::OnChangeEditParamFloat() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CConditionDlg::OnChangeEditParamInt() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(true);
}
