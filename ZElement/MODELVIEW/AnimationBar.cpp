// AnimationBar.cpp : implementation file
//

#include "global.h"
#include "ModelView.h"
#include "AnimationBar.h"
#include "MODELVIEWDataMan.h"
#include "Player.h"
#include "..\ZElementSkill\ElementSkill.h"
#include "..\ZElementSkill\skill.h"
#include "..\ZElementClient\EC_StringTab.h"
#define new A_DEBUG_NEW

extern CECStringTab g_SkillActs;

/////////////////////////////////////////////////////////////////////////////
// CAnimationBar dialog


CAnimationBar::CAnimationBar()
{
	//{{AFX_DATA_INIT(CAnimationBar)
	m_bOccupLife = FALSE;
	m_bOccupFamily = FALSE;
	m_bOccupSect = FALSE;
	//}}AFX_DATA_INIT
	m_bPauseButtonEnabled = false;
}


void CAnimationBar::DoDataExchange(CDataExchange* pDX)
{
	CInitDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimationBar)
	DDX_Control(pDX, IDC_COMBO_SKILL, m_cbSkill);
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_SpeedCtrl);
	DDX_Control(pDX, IDC_COMBO_ACTION, m_cbAction);
	DDX_Check(pDX, IDC_CHECK_OCCUP_LIFE, m_bOccupLife);
	DDX_Check(pDX, IDC_CHECK_OCCUP_FAMILY, m_bOccupFamily);
	DDX_Check(pDX, IDC_CHECK_OCCUP_SECT, m_bOccupSect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimationBar, CInitDialogBar)
	//{{AFX_MSG_MAP(CAnimationBar)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnButtonPause)
	ON_CBN_SELCHANGE(IDC_COMBO_ACTION, OnSelchangeComboAction)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SPEED, OnReleasedcaptureSliderSpeed)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_SLIDER_SPEED, OnOutofmemorySliderSpeed)
	ON_BN_CLICKED(IDC_CHECK_LOOP, OnCheckLoop)
	ON_CBN_SELCHANGE(IDC_COMBO_SKILL, OnSelchangeComboSkill)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_PLAY, OnButtonSkillPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_CHECK_OCCUP_LIFE, OnCheckOccupLife)
	ON_BN_CLICKED(IDC_CHECK_OCCUP_FAMILY, OnCheckOccupFamily)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_PAUSE, OnUpdateButtonPause)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_SKILL_PLAY,OnUpdateButtonSkillPlay)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_STOP,OnUpdateButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_CHECK_OCCUP_SECT, OnCheckOccupSect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimationBar message handlers

void CAnimationBar::OnButtonPause() 
{
	// TODO: Add your control notification handler code here
	g_bPause = true;
	CWnd *pwnd = GetDlgItem(IDC_BUTTON_STOP);
	pwnd->SetWindowText("播放");
		
	
//	if(g_pRenderableObject)
//	{
//		if(g_bPause){
//			g_pRenderableObject->StopAction();
//		}
//		else OnSelchangeComboAction();
//	}
}

void CAnimationBar::OnUpdateButtonPause(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPauseButtonEnabled);
	//UpdateButton();

}

void CAnimationBar::UpdateStopButton(bool bStop)
{
	CWnd *pwnd = GetDlgItem(IDC_BUTTON_STOP);
	if(bStop)
	{		
		pwnd->SetWindowText("播放");
	}else
	{
		pwnd->SetWindowText("停止");
	}
}

void CAnimationBar::OnUpdateButtonStop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CAnimationBar::AddAction(CString strAction)
{
	m_cbAction.AddString(strAction);
}

void  CAnimationBar::AddSkill( CString strSkill, int id)
{
	int idx = m_cbSkill.AddString(strSkill);
	m_cbSkill.SetItemData(idx,id);
}

void CAnimationBar::ClearActions()
{
	int n = m_cbAction.GetCount();
	for( int i = 0; i < n; ++i)
		m_cbAction.DeleteString(0);
}

void  CAnimationBar::ClearSkill()
{
	int n = m_cbSkill.GetCount();
	for( int i = 0; i < n; ++i)
		m_cbSkill.DeleteString(0);
}

CString CAnimationBar::GetSelAction()
{
	CString str;
	int n = m_cbAction.GetCurSel();
	if(n==-1) 
	{
		if(m_cbAction.GetCount() > 0)
		{
			m_cbAction.GetLBText(0,str);
			return str;
		}
		return "";
	}else 
	{
		
		m_cbAction.GetLBText(n,str);
		return str;
	}
}

void CAnimationBar::SetDefaultSel()
{
	if(m_cbAction.GetCount() > 0)
		m_cbAction.SetCurSel(0);
}

void CAnimationBar::SetSkillDefaultSel()
{
	if(m_cbSkill.GetCount() > 0)
		m_cbSkill.SetCurSel(0);
}

void CAnimationBar::OnSelchangeComboAction() 
{
	// TODO: Add your control notification handler code here
	if(g_pRenderableObject)
	{
		//g_pRenderableObject->StopAction();
		g_pRenderableObject->PlayAction(AString(GetSelAction()),g_bLoop);
		g_bStop = false;
		g_bPause = false;
	}
}

//void CAnimationBar::UpdateButton(bool isPlaying)
//{
//	CWnd *pwnd = GetDlgItem(IDC_BUTTON_PAUSE);
//	if(g_bPause) pwnd->SetWindowText("播放");
//	else pwnd->SetWindowText("停止");
//}

void CAnimationBar::EnablePauseButton()
{
	m_bPauseButtonEnabled = true;
	//CWnd *pwnd = GetDlgItem(IDC_BUTTON_PAUSE);
	//pwnd->EnableWindow(true);
//	if(g_bPause) pwnd->SetWindowText("播放");
//	else pwnd->SetWindowText("停止");
}
void CAnimationBar::DisablePauseButton()
{
	m_bPauseButtonEnabled = false;
	//CWnd *pwnd = GetDlgItem(IDC_BUTTON_PAUSE);
	//pwnd->EnableWindow(false);
}
bool CAnimationBar::IsPauseButtonEnabled()
{
	return m_bPauseButtonEnabled;
}

void CAnimationBar::OnReleasedcaptureSliderSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int pos = m_SpeedCtrl.GetPos();
	g_fPlaySpeed = (pos + 1)/10.0f;
	
	CWnd *pTxt = GetDlgItem(IDC_STATIC_SPEED);
	CString str;
	str.Format("Speed = %4.2f X",g_fPlaySpeed);
	pTxt->SetWindowText(str);
	*pResult = 0;
}

void CAnimationBar::OnOutofmemorySliderSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CAnimationBar::OnCheckLoop() 
{
	// TODO: Add your control notification handler code here
	g_bLoop = !g_bLoop;
	g_bPause = false;
	if(g_pRenderableObject)
		g_pRenderableObject->PlayAction(AString(GetSelAction()),g_bLoop);
	g_bStop = false;
	CWnd *pwnd = GetDlgItem(IDC_BUTTON_STOP);
	pwnd->SetWindowText("停止");
	
}

int CAnimationBar::GetSelSkill()
{
	int sel = m_cbSkill.GetCurSel();
	if(sel==-1) return -1;
	else return m_cbSkill.GetItemData(sel);
}

void CAnimationBar::OnSelchangeComboSkill() 
{
	// TODO: Add your control notification handler code here
	if(g_pRenderableObject)
	{
		CString actName;
		int id = GetSelSkill();
		if(-1 == id ) return;
		switch(g_pRenderableObject->GetObjectType()) {
		case RENDERABLE_OBJECT_PLAYER:
			actName = GetSillActionName(id,g_pRenderableObject->GetWeaponID());
			if(actName=="0")SetActionCurSel("无动作"); 
			else SetActionCurSel(actName);
			g_bStop = !g_pRenderableObject->PlayAttackAction(actName,id);
			//g_pRenderableObject->PlayAction(AString(actName),g_bLoop);
			
			//CWnd *pwnd = GetDlgItem(IDC_BUTTON_SKILL_PLAY);
			//pwnd->SetWindowText("停止");
			break;
		default:
			break;
		}

	}
}

CString CAnimationBar::GetSillActionName(int _id_sill, int _id_weapon)
{
	DATA_TYPE type;
	EQUIPMENT_ESSENCE* pData = (EQUIPMENT_ESSENCE*)g_DataMan.get_data_ptr(_id_weapon,ID_SPACE_ESSENCE,type);
	char szAct[256] = "";
	int nActType;
	if(pData)
		nActType = pData->action_type;
	else nActType = 0;

	if( nActType < 0 ) nActType = 0;
	if( nActType > 12 ) nActType = 12;
	CPlayer* pPlayer = (CPlayer*) g_pRenderableObject;
	sprintf(szAct, "%s", GNET::ElementSkill::GetAction(_id_sill, pPlayer->GetRide()?nActType+NUM_WEAPON:nActType));
	if(szAct[0])
		int  U = 1; 
	return szAct;
}

void CAnimationBar::OnButtonSkillPlay() 
{
	// TODO: Add your control notification handler code here
	if(g_pRenderableObject)
	{
		int id = GetSelSkill();
		if(-1 == id ) return;
		CString actName = GetSillActionName(id,g_pRenderableObject->GetWeaponID());
		if(actName=="0")SetActionCurSel("无动作"); 
		else SetActionCurSel(actName);
		g_bStop = !g_pRenderableObject->PlayAttackAction(actName,id);
		//CWnd *pwnd = GetDlgItem(IDC_BUTTON_SKILL_PLAY);
		//pwnd->SetWindowText("停止");
	}
}

void CAnimationBar::OnUpdateButtonSkillPlay(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CAnimationBar::SetActionCurSel(CString strAction)
{

	int sel = -1;	
	for( int i = 0; i < m_cbAction.GetCount(); ++i)
	{
		CString str;
		m_cbAction.GetLBText(i,str);
		if(str==strAction) 
		{
			sel = i;
			break;
		}
	}
	if(sel!=-1)
		m_cbAction.SetCurSel(sel);
}

void CAnimationBar::OnButtonStop() 
{
	// TODO: Add your control notification handler code here

	if(g_pRenderableObject)
	{
		if(g_bStop)//停止时
		{
				g_pRenderableObject->StopAction();//如果不加这一行，如时装模式下情人节女的动作播完后不能接着再播
				g_pRenderableObject->PlayAction(AString(GetSelAction()),g_bLoop);
				g_bStop = false;
				g_bPause = false;
			CWnd *pwnd = GetDlgItem(IDC_BUTTON_STOP);
			pwnd->SetWindowText("停止");
			EnablePauseButton();
		}else
		{
			if (g_bPause) {//暂停时
				g_bPause = false;
				CWnd *pwnd = GetDlgItem(IDC_BUTTON_STOP);
				pwnd->SetWindowText("停止");
				EnablePauseButton();
			}else{//播放时
				g_pRenderableObject->StopAction();
				g_bStop = true;
				CWnd *pwnd = GetDlgItem(IDC_BUTTON_STOP);
				pwnd->SetWindowText("播放");
				DisablePauseButton();
			}
		}
	}
}

int CAnimationBar::GetSkillCategory(){
	int cate = 0;
	if(m_bOccupLife)
		cate |= GNET::CATEGORY_LIFE;
	if (m_bOccupFamily) 
		cate |= GNET::CATEGORY_FAMILY;
	if(m_bOccupSect)
		cate |= GNET::CATEGORY_SECT;
	return cate;
}

void CAnimationBar::OnCheckOccupLife() 
{
	// TODO: Add your control notification handler code here
	m_bOccupLife = !m_bOccupLife;
	CPlayer* pPlayer;
	if(g_pRenderableObject){
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) {
		case RENDERABLE_OBJECT_PLAYER:
			pPlayer = (CPlayer*)g_pRenderableObject;
			pPlayer->RecreateSkillList();
			break;
		default:
			break;
		}
	}
}

void CAnimationBar::OnCheckOccupFamily() 
{
	// TODO: Add your control notification handler code here
	m_bOccupFamily = !m_bOccupFamily;
	CPlayer* pPlayer;
	if(g_pRenderableObject){
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) {
		case RENDERABLE_OBJECT_PLAYER:
			pPlayer = (CPlayer*)g_pRenderableObject;
			pPlayer->RecreateSkillList();
			break;
		default:
			break;
		}
	}
	
}

void CAnimationBar::OnCheckOccupSect() 
{
	// TODO: Add your control notification handler code here
	m_bOccupSect = !m_bOccupSect;
	CPlayer* pPlayer;
	if(g_pRenderableObject){
		int nType = g_pRenderableObject->GetObjectType();
		switch(nType) {
		case RENDERABLE_OBJECT_PLAYER:
			pPlayer = (CPlayer*)g_pRenderableObject;
			pPlayer->RecreateSkillList();
			break;
		default:
			break;
		}
	}	
}
