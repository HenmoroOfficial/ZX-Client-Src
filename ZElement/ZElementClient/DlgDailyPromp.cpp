// Filename	:	DlgDailyPromp.cpp
// Creator	:	Fu Chonggang
// Date		:	Sep 2, 2010

#include "DlgDailyPromp.h"
#include "DlgMiniMap.h"
#include "DlgFengshenSoul.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUISubDialog.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIImagePicture.h"

#include "EC_Menology.h"
#include "EC_GameUIMisc.h"
#include "elementdataman.h"
#include "TaskTemplMan.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Instance.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"
#include "EC_TaskInterface.h"
#include "EC_AutoMove.h"
#include "EC_World.h"
#include "EC_Model.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_Configs.h"
#include "EC_ModelRender.h"
#include "EC_Utility.h"
#include "EC_Global.h"
#include "A2DSprite.h"
#include <map>

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgDailyPromp, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_ON_COMMAND("rdo1",					OnCommand_RdoTypeToday)
AUI_ON_COMMAND("rdo2",					OnCommand_RdoTypeWeek)
AUI_ON_COMMAND("rdo3",					OnCommand_RdoTypeBattle)
AUI_ON_COMMAND("rdo4",					OnCommand_RdoTypeBoss)
AUI_ON_COMMAND("Chk_15Boss",			OnCommand_Boss15Only)
AUI_ON_COMMAND("Chk_LoginHint",			OnCommand_LoginShowState)

AUI_ON_COMMAND("Btn_Time",				OnCommand_DailySortTime)
AUI_ON_COMMAND("Btn_Lv",				OnCommand_DailySortLevel)
AUI_ON_COMMAND("Btn_Status",			OnCommand_DailySortStatus)
AUI_ON_COMMAND("Btn_LvBoss",			OnCommand_DailySortLevelBoss)
AUI_ON_COMMAND("Btn_SuggestLv",			OnCommand_DailySortLvRecommandBoss)
AUI_ON_COMMAND("Btn_Line",				OnCommand_DailySortLineBoss)

AUI_ON_COMMAND("Btn_Reward",			OnCommand_DailySortReward)
AUI_ON_COMMAND("Btn_Name",				OnCommand_DailySortName)
AUI_ON_COMMAND("Btn_Times",				OnCommand_DailySortCount)
AUI_ON_COMMAND("Btn_Cond",				OnCommand_DailySortCond)
AUI_ON_COMMAND("Btn_Period",			OnCommand_DailySortPeriod)
AUI_ON_COMMAND("Btn_Line",				OnCommand_DailySortBossRefresh)

//AUI_ON_COMMAND("Chk_LoginHint",			OnCommand_DailySort)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgDailyPromp, CDlgBase)

AUI_ON_EVENT("Lst_Task",		WM_LBUTTONDOWN,		OnEventLButtonDownList)
AUI_ON_EVENT("Lst_TaskMonster",	WM_LBUTTONDOWN,		OnEventLButtonDownListMonster)
AUI_ON_EVENT("Txt_BossPos",		WM_LBUTTONDOWN,		OnEventLButtonDownNpc)
AUI_ON_EVENT("Txt_TaskNpc",		WM_LBUTTONDOWN,		OnEventLButtonDownNpc)
AUI_ON_EVENT("Txt_BossPos",		WM_MOUSEMOVE,		OnEventMouseMoveNpc)
AUI_ON_EVENT("Txt_TaskNpc",		WM_MOUSEMOVE,		OnEventMouseMoveNpc)

AUI_END_EVENT_MAP()

extern ACString g_AutoMoveDestName;

bool					CDlgDailyPromp::ms_bLoaded = false;
DWORD					CDlgDailyPromp::ms_dwActionTime = 10000;
int						CDlgDailyPromp::m_nType;
abase::vector<A2DSprite*> CDlgDailyPromp::m_vecImageList;

const int invalidPosTag = 9999;
const char*	szActName[] = 
{ 
	"站立",
	"休闲",
	"行走",
	"普通攻击1",
	"技能攻击1",
	"普通攻击2",
	"技能攻击2",
};

CDlgDailyPromp::CDlgDailyPromp()
{
	if ( m_szName=="Win_DailyPrompBoss" )
	{
		m_pModel = NULL;
	}
}

CDlgDailyPromp::~CDlgDailyPromp()
{
}

bool CDlgDailyPromp::Release()
{
	if ( m_szName=="Win_DailyPrompBoss" )
	{
		A3DRELEASE(m_pModel);
	}
	if (ms_bLoaded)
	{
		ms_bLoaded = false;
	}
	return CDlgBase::Release();
}

bool CDlgDailyPromp::OnInitDialog()
{
	if ( m_szName=="Win_DailyPrompBoss" )
	{
		m_pLst_Task = (AUIListBox *)GetDlgItem("Lst_TaskMonster");
		m_pTxt_TaskNpc = (AUITextArea *)GetDlgItem("Txt_BossPos");	
		m_pModel = new CECModel;
		m_bLoaded = false;
	}
	else
	{
		m_pLst_Task = (AUIListBox *)GetDlgItem("Lst_Task");
		m_pTxt_TaskNpc = (AUITextArea *)GetDlgItem("Txt_TaskNpc");
		m_pTxt_TaskNpcMap = (AUITextArea *)GetDlgItem("Txt_Tasknpc_Pos");
	}
	m_pLst_Task->SetAlign(AUIFRAME_ALIGN_CENTER);
	m_pTxt_Time = (AUILabel *)GetDlgItem("Txt_Time");
	m_pImg_Task = (AUIImagePicture *)GetDlgItem("Img_Task");
/*	if(GetDlgItem("Btn_xx")) GetDlgItem("Btn_xx")->SetAcceptMouseMessage(false);
	if(GetDlgItem("Btn_Reward")) GetDlgItem("Btn_Reward")->SetAcceptMouseMessage(false);
	if(GetDlgItem("Btn_Name")) GetDlgItem("Btn_Name")->SetAcceptMouseMessage(false);
	if(GetDlgItem("Btn_Cond")) GetDlgItem("Btn_Cond")->SetAcceptMouseMessage(false);
	if(GetDlgItem("Btn_Period")) GetDlgItem("Btn_Period")->SetAcceptMouseMessage(false);
	if(GetDlgItem("Btn_Times")) GetDlgItem("Btn_Times")->SetAcceptMouseMessage(false);
*/
	m_pTxt_TaskLine = (AUILabel *)GetDlgItem("Txt_TaskLine");
	m_pTxt_TaskLv = (AUILabel *)GetDlgItem("Txt_TaskLv");
	m_pTxt_TaskTime = (AUILabel *)GetDlgItem("Txt_TaskTime");
	m_pTxt_BossName = (AUILabel *)GetDlgItem("Txt_BossName");
	m_pTxt_BossLv = (AUILabel *)GetDlgItem("Txt_BossLv");
	m_pTxt_Desc = (AUITextArea *)GetDlgItem("Txt_2");
	for (int i=0; i<AWARD_MAX; i++)
	{
		AString strName;
		strName.Format("Img_Award%d", i+1);
		m_pImg_Award[i] = (AUIImagePicture *)GetDlgItem(strName);
	}
	
	if (!ms_bLoaded)
	{
		m_nType = TYPE_RECOMMAND;
		m_vecImageList.clear();
		m_vecImageList.push_back(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_DAILYPROMP24));	//ICONTYPE_TASK24
		m_vecImageList.push_back(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_DAILYPROMP32));	//ICONTYPE_TASK32
		m_vecImageList.push_back(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_DAILYPROMP48)); //ICONTYPE_TASK48
		ms_bLoaded = true;
	}

	m_pTxt_TaskNpc->SetImageList(GetGameUIMan()->GetControlImgLst());
	m_pTxt_TaskNpc->SetItemMask(0xffffffff);
	m_pLst_Task->SetImageList(&m_vecImageList);
	m_pLst_Task->SetItemMask(0xffffffff);
	m_nCurSel = 0;
	return true;
}

void CDlgDailyPromp::OnShowDialog()
{
	m_nCurSel = m_pLst_Task->GetCurSel();
	PAUIDIALOG pDlgCurShow = NULL;
	if ( m_szName!="Win_DailyPromp_Today"&&GetGameUIMan()->GetDialog("Win_DailyPromp_Today")->IsShow() )
	{
		pDlgCurShow = GetGameUIMan()->GetDialog("Win_DailyPromp_Today");
	}
	else if ( m_szName!="Win_DailyPromp_Week"&&GetGameUIMan()->GetDialog("Win_DailyPromp_Week")->IsShow() )
	{
		pDlgCurShow = GetGameUIMan()->GetDialog("Win_DailyPromp_Week");
	}
	else if ( m_szName!="Win_DailyPromp_Battlefield"&&GetGameUIMan()->GetDialog("Win_DailyPromp_Battlefield")->IsShow() )
	{
		pDlgCurShow = GetGameUIMan()->GetDialog("Win_DailyPromp_Battlefield");
	}
	else if ( m_szName!="Win_DailyPrompBoss"&&GetGameUIMan()->GetDialog("Win_DailyPrompBoss")->IsShow() )
	{
		pDlgCurShow = GetGameUIMan()->GetDialog("Win_DailyPrompBoss");
	}
	if(pDlgCurShow)
		pDlgCurShow->Show(false);

	if ( m_szName=="Win_DailyPromp_Today" )
	{
		CheckRadioButton(1, 1);
		OnCommand_DailySortStatus("");
		
		EC_GAME_CLIENTDATA gcd = GetGame()->GetConfigs()->GetGameClientData();
		((PAUICHECKBOX)GetDlgItem("Chk_LoginHint"))->Check(gcd.nHideDailyPrompLoginHint==0);
		if(pDlgCurShow)
			AlignTo(pDlgCurShow, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
		 AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
		return;
	}
	else if ( m_szName=="Win_DailyPromp_Week")
	{
		CheckRadioButton(1, 2);
		OnCommand_DailySortTime("");
		if(pDlgCurShow)
			AlignTo(pDlgCurShow, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
		 AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
		return;
	}
	else if ( m_szName=="Win_DailyPromp_Battlefield")
	{
		CheckRadioButton(1, 3);
		OnCommand_DailySortTime("");
		if(pDlgCurShow)
			AlignTo(pDlgCurShow, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
		 AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
		return;
	}
	else if ( m_szName=="Win_DailyPrompBoss")
	{
		CheckRadioButton(1, 4);
		OnCommand_DailySortLevelBoss("");
		if(pDlgCurShow)
			AlignTo(pDlgCurShow, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
		 AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
		return;
	}
}

void CDlgDailyPromp::OnTick()
{
	// 当前时间
	// 注释掉部分让中间 ： 闪动
//	static DWORD dwFlash = 0;
	DWORD dwTick = GetGame()->GetTickTime();
//	dwFlash += dwTick;
	tm tmNow = GetGame()->GetServerLocalTime();
	ACString strText;
//	if (dwFlash>1000)
//		dwFlash = 0;
// 	if (dwFlash<500)
// 	{
// 		strText.Format(_AL("%d   %02d"), tmNow.tm_hour, tmNow.tm_min);
// 	}
// 	else if (dwFlash<1000)
// 	{
		strText.Format(_AL("%d : %02d"), tmNow.tm_hour, tmNow.tm_min);
//	}
		m_pTxt_Time->SetText(strText);
		
		// 处理键盘上下按钮
	if (m_nCurSel!=m_pLst_Task->GetCurSel())
	{
		if ( m_szName=="Win_DailyPrompBoss" )
			OnEventLButtonDownListMonster(0, 0, m_pLst_Task);
		else
			OnEventLButtonDownList(0, 0, m_pLst_Task);
	}

	if (m_szName=="Win_DailyPrompBoss")
	{
		if (m_bLoaded)
		{
			if (ms_dwActionTime<dwTick)
			{
				ms_dwActionTime = a_Random(8000, 20000);
				m_pModel->PlayActionByName(szActName[a_Random(3,6)], 1.0f, true);
				m_pModel->QueueAction(szActName[a_Random(0,2)]);
			}
			ms_dwActionTime -= dwTick;
			m_pModel->Tick(dwTick);
			m_pImg_Task->SetRenderCallback( DlgModelRender,	(DWORD)m_pModel, MRT_DAILYPROMP);
		}
		else
			m_pImg_Task->SetRenderCallback( NULL, 0, 0);
	}
}

bool CDlgDailyPromp::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

int CDlgDailyPromp::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	return 1;
}

void CDlgDailyPromp::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgDailyPromp::OnCommand_RdoTypeToday(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_DailyPromp_Today");
	pDlg->Show(true);
}

void CDlgDailyPromp::OnCommand_RdoTypeWeek(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_DailyPromp_Week");
	pDlg->Show(true);
}

void CDlgDailyPromp::OnCommand_RdoTypeBattle(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_DailyPromp_Battlefield");
	pDlg->Show(true);
}

void CDlgDailyPromp::OnCommand_RdoTypeBoss(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_DailyPrompBoss");
	pDlg->Show(true);
}

void CDlgDailyPromp::OnCommand_Boss15Only(const char * szCommand)
{
	UpdateMonster();
}

void CDlgDailyPromp::OnCommand_LoginShowState(const char * szCommand)
{
	EC_GAME_CLIENTDATA gcd = GetGame()->GetConfigs()->GetGameClientData();
	gcd.nHideDailyPrompLoginHint = !((PAUICHECKBOX)GetDlgItem("Chk_LoginHint"))->IsChecked();
	GetGame()->GetConfigs()->SetGameClientData(gcd);
}

void CDlgDailyPromp::OnCommand_DailySortTime(const char * szCommand)
{
	if (m_szName=="Win_DailyPromp_Today")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_RECOMMAND, CECMenology::ST_TIME, bInc);
		bInc = !bInc;
		UpdateDaily();
	}
	else if (m_szName=="Win_DailyPromp_Week")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_WEEKLY, CECMenology::ST_TIME, bInc);
		bInc = !bInc;
		UpdateWeekly();
	}
	else if (m_szName=="Win_DailyPromp_Battlefield")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_BATTLEFIELD, CECMenology::ST_TIME, bInc);
		bInc = !bInc;
		UpdateBattleInfo();
	}
}
void CDlgDailyPromp::OnCommand_DailySortLevel(const char * szCommand)
{
	if (m_szName=="Win_DailyPromp_Today")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_RECOMMAND, CECMenology::ST_LV_RCOMMAND, bInc);
		bInc = !bInc;
		UpdateDaily();
	}
	else if (m_szName=="Win_DailyPromp_Week")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_WEEKLY, CECMenology::ST_LV_RCOMMAND, bInc);
		bInc = !bInc;
		UpdateWeekly();
	}
	else if (m_szName=="Win_DailyPromp_Battlefield")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_BATTLEFIELD, CECMenology::ST_LV_RCOMMAND, bInc);
		bInc = !bInc;
		UpdateBattleInfo();
	}
}
void CDlgDailyPromp::OnCommand_DailySortStatus(const char * szCommand)
{
	static bool bInc = true;
	CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_RECOMMAND, CECMenology::ST_STATUS, bInc);
	bInc = !bInc;
	UpdateDaily();
}
void CDlgDailyPromp::OnCommand_DailySortLevelBoss(const char * szCommand)
{
	static bool bInc = true;
	CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_MONSTER, CECMenology::ST_LV_BOSS, bInc);
	bInc = !bInc;
	UpdateMonster();
}
void CDlgDailyPromp::OnCommand_DailySortLvRecommandBoss(const char * szCommand)
{
	static bool bInc = true;
	CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_MONSTER, CECMenology::ST_BOSS_RECLV, bInc);
	bInc = !bInc;
	UpdateMonster();
}
void CDlgDailyPromp::OnCommand_DailySortLineBoss(const char * szCommand)
{
	static bool bInc = true;
	CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_MONSTER, CECMenology::ST_BOSS_LINE, bInc);
	bInc = !bInc;
	UpdateMonster();
}
void CDlgDailyPromp::OnCommand_DailySortReward(const char * szCommand)
{
	if (m_szName=="Win_DailyPromp_Today")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_RECOMMAND, CECMenology::ST_REWARD, bInc);
		bInc = !bInc;
		UpdateDaily();
	}
	else if (m_szName=="Win_DailyPromp_Week")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_WEEKLY, CECMenology::ST_REWARD, bInc);
		bInc = !bInc;
		UpdateWeekly();
	}
}
void CDlgDailyPromp::OnCommand_DailySortName(const char * szCommand)
{
	if (m_szName=="Win_DailyPromp_Today")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_RECOMMAND, CECMenology::ST_NAME, bInc);
		bInc = !bInc;
		UpdateDaily();
	}
	else if (m_szName=="Win_DailyPromp_Week")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_WEEKLY, CECMenology::ST_NAME, bInc);
		bInc = !bInc;
		UpdateWeekly();
	}
	else if (m_szName=="Win_DailyPromp_Battlefield")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_BATTLEFIELD, CECMenology::ST_NAME, bInc);
		bInc = !bInc;
		UpdateBattleInfo();
	}
	else if (m_szName=="Win_DailyPrompBoss")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_MONSTER, CECMenology::ST_NAME, bInc);
		bInc = !bInc;
		UpdateMonster();
	}
}
void CDlgDailyPromp::OnCommand_DailySortCount(const char * szCommand)
{
	if (m_szName=="Win_DailyPromp_Today")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_RECOMMAND, CECMenology::ST_COUNT, bInc);
		bInc = !bInc;
		UpdateDaily();
	}
	else if (m_szName=="Win_DailyPromp_Battlefield")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_BATTLEFIELD, CECMenology::ST_COUNT, bInc);
		bInc = !bInc;
		UpdateBattleInfo();
	}
}
void CDlgDailyPromp::OnCommand_DailySortCond(const char * szCommand)
{
	if (m_szName=="Win_DailyPromp_Today")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_RECOMMAND, CECMenology::ST_CONDICTION, bInc);
		bInc = !bInc;
		UpdateDaily();
	}
	else if (m_szName=="Win_DailyPromp_Week")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_WEEKLY, CECMenology::ST_CONDICTION, bInc);
		bInc = !bInc;
		UpdateWeekly();
	}
}
void CDlgDailyPromp::OnCommand_DailySortPeriod(const char * szCommand)
{
	if (m_szName=="Win_DailyPromp_Week")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_WEEKLY, CECMenology::ST_PERIOD, bInc);
		bInc = !bInc;
		UpdateWeekly();
	}
	else if (m_szName=="Win_DailyPromp_Battlefield")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_BATTLEFIELD, CECMenology::ST_PERIOD, bInc);
		bInc = !bInc;
		UpdateBattleInfo();
	}
	else if (m_szName=="Win_DailyPrompBoss")
	{
		static bool bInc = true;
		CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_MONSTER, CECMenology::ST_BOSS_PERIOD, bInc);
		bInc = !bInc;
		UpdateMonster();
	}
}
void CDlgDailyPromp::OnCommand_DailySortBossRefresh(const char * szCommand)
{
	static bool bInc = true;
	CECMenology::GetInstance()->SortInfo(CECMenology::TYPE_MONSTER, CECMenology::ST_BOSS_PERIOD, bInc);
	bInc = !bInc;
	UpdateMonster();
}

void CDlgDailyPromp::OnEventLButtonDownList(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	m_strHintNpc.Empty();
	m_pTxt_TaskLine->SetText(_AL(""));
	m_pTxt_TaskLv->SetText(_AL(""));
	m_pTxt_TaskTime->SetText(_AL(""));
	m_pTxt_TaskNpc->SetText(_AL(""));
	m_pTxt_TaskNpcMap->SetText(_AL(""));
	m_pImg_Task->ClearCover();
	UINT i;
	for (i=0; i<AWARD_MAX; i++)
	{
		m_pImg_Award[i]->Show(false);
		m_pImg_Award[i]->SetText(_AL(""));
		m_pImg_Award[i]->SetHint(_AL(""));
	}
	
	m_nCurSel = m_pLst_Task->GetCurSel();
	int nSel = m_pLst_Task->GetCurSel();
	if( nSel < 0 || nSel >= m_pLst_Task->GetCount() )
		return;
	
	MenologyInfoVector vecInfo = CECMenology::GetInstance()->GetInfo(m_nType);
	int index = m_pLst_Task->GetItemData(nSel, 0);
	const MenologyInfo& mInfo = vecInfo[index];
	bool bOk = false;

	m_pTxt_TaskLine->SetText(mInfo.strLineInfo);
	int nCurLine = GetGame()->GetGameRun()->GetCurLine();
	for (i=0; i<mInfo.vecLine.size(); i++)
	{
		if (nCurLine==mInfo.vecLine[i])
			bOk = true;
	}
	m_pTxt_TaskLine->SetColor(bOk?A3DCOLORRGB(255,0xe9,0x82):A3DCOLORRGB(255,0,0));

	m_pTxt_TaskLv->SetText(mInfo.strLevelInfo);
	m_pTxt_TaskLv->SetColor(CheckLevel(&mInfo)?A3DCOLORRGB(255,0xe9,0x82):A3DCOLORRGB(255,0,0));
	m_pTxt_TaskTime->SetText(mInfo.strTime);
	bOk = false;
	tm tmNow = GetGame()->GetServerLocalTime();
	int nowMinute = tmNow.tm_hour*60 + tmNow.tm_min;
	if ( nowMinute>=(mInfo.tmStartTime.tm_hour*60+mInfo.tmStartTime.tm_min) &&
		 nowMinute<=(mInfo.tmEndTime.tm_hour*60+mInfo.tmEndTime.tm_min) )
		bOk = true;
	m_pTxt_TaskTime->SetColor(bOk?A3DCOLORRGB(255,0xe9,0x82):A3DCOLORRGB(255,0,0));
	ACString strText;
	ACString strHint;
	if (mInfo.bTaskNpc)
	{
		int idTask = m_pLst_Task->GetItemData(nSel, 1);
		elementdataman *pDataMan = GetGame()->GetElementDataMan();
		ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
		ATaskTempl* lstTask = pMan->GetTaskTemplByID(idTask);
		if (lstTask)
		{
			int nNPC = lstTask->GetDeliverNPC();
			if (nNPC>0)
			{
				DATA_TYPE dt;
				NPC_ESSENCE *pNPC = (NPC_ESSENCE *)pDataMan->get_data_ptr(
					nNPC, ID_SPACE_ESSENCE, dt);
				const NPC_INFO *info = pMan->GetTaskNPCInfo(nNPC);
				if( info && info->x != 0 && info->z != 0)
				{
					EditBoxItemBase item(enumEICoord);
					item.SetName(pNPC->name);
					ACHAR szPos[100];
					a_sprintf(szPos, _AL("%d %d %d %d"), info->x, info->y, info->z, info->map_id);
					item.SetInfo(szPos);
					item.SetColor(A3DCOLORRGB(0, 255, 0));
					ACString strName = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
					CECInstance *pInstance = GetGameRun()->GetInstance(info->map_id);
					if( pInstance )
					{
						m_pTxt_TaskNpcMap->SetText(pInstance->GetNameByCoord(info->x, info->z));
						strText = strName;
						strHint.Format(_AL("%d,%d"), info->x, info->z);
					}
					else
					{
						m_pTxt_TaskNpcMap->SetText(_AL(""));
						strText = strName;
						strHint.Format(_AL("%d,%d"), info->x, info->z);
					}
					
					if (mInfo.bIsTransport)
					{
						strText += _AL(" ");
						EditBoxItemBase itemImage(enumEIImage);
						itemImage.SetImageIndex(CECGameUIMan::ICONLIST_TRANSFER);
						itemImage.SetImageFrame(0);
						ACHAR szId[100];
						a_sprintf(szId, _AL("%d"), m_pLst_Task->GetItemData(nSel, 2));
						itemImage.SetInfo(szId);
						strText += (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
					}
				}
				else
				{
					strText.Format(GetStringFromTable(GetCheckedRadioButton(1)==1 ? 6009 : 6010), pNPC->name);
				}
			}
			m_pTxt_TaskNpc->SetText(strText);
		}
		m_pTxt_TaskNpc->SetHint(strHint);
		m_strHintNpc = strHint;
	}
	else
	{
		ACString strName;
		int x, y, z;
		a_sscanf(mInfo.strNpcPos, _AL("%d,%d,%d"), &x, &y, &z);
		if (x!=invalidPosTag&&y!=invalidPosTag)
		{
			EditBoxItemBase item(enumEICoord);
			item.SetName(mInfo.strNPC);
			y = 0;
			ACHAR szPos[100];
			a_sprintf(szPos, _AL("%d %d %d %d"), x, y, z, mInfo.iNpcMapid);
			item.SetInfo(szPos);
			item.SetColor(A3DCOLORRGB(0, 255, 0));
			strName = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			strHint.Format(_AL("%d,%d"), x, z);
		}
		else
			strName = mInfo.strNPC;
		CECInstance *pInstance = GetGameRun()->GetInstance(mInfo.iNpcMapid);
		if( pInstance )
		{
			m_pTxt_TaskNpcMap->SetText(pInstance->GetNameByCoord((float)x, (float)z));
			strText = strName;
		}
		else
		{
			m_pTxt_TaskNpcMap->SetText(_AL(""));
			strText = mInfo.strNPC;
		}
		
		if (mInfo.bIsTransport)
		{
			strText += _AL(" ");
			EditBoxItemBase itemImage(enumEIImage);
			itemImage.SetImageIndex(CECGameUIMan::ICONLIST_TRANSFER);
			itemImage.SetImageFrame(0);
			ACHAR szId[100];
			a_sprintf(szId, _AL("%d"), m_pLst_Task->GetItemData(nSel, 2));
			itemImage.SetInfo(szId);
			strText += (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
		}
		m_pTxt_TaskNpc->SetText(strText);
		m_pTxt_TaskNpc->SetHint(strHint);
		m_strHintNpc = strHint;
	}
	
	GetGameUIMan()->SetImgIconSet(m_pImg_Task, CECGameUIMan::ICONS_DAILYPROMP48, mInfo.strIcon2);
	m_pImg_Task->SetHint(mInfo.strName);
	m_pTxt_Desc->SetText(mInfo.strDesc);
	if (mInfo.bTemplateAward)
	{
		UINT index = 0;
		int idTask = GetSuitTask(mInfo.uiTaskID, mInfo.uiNumID);
		ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
		ATaskTempl* lstTask = pMan->GetTaskTemplByID(idTask);
		if(!lstTask || lstTask->m_ulAwardType_S!=enumTATNormal)
			return;
		AWARD_DATA* awardData = lstTask->m_Award_S;
		ACString strTemp;
		if( awardData->m_ulGoldNum > 0 )
		{
			GetGameUIMan()->SetImgIconSet(m_pImg_Award[index], CECGameUIMan::ICONS_DAILYPROMP32, "金钱.dds");
			ACHAR szGold[256];
			CECIvtrItem::GetPriceText(szGold, awardData->m_ulGoldNum);
			m_pImg_Award[index]->SetHint(szGold);
			index++;
		}
		if( awardData->m_ulExp > 0 )
		{
			GetGameUIMan()->SetImgIconSet(m_pImg_Award[index], CECGameUIMan::ICONS_DAILYPROMP32, "经验.dds");
			strText.Format(GetStringFromTable(3202), awardData->m_ulExp);
			m_pImg_Award[index]->SetHint(strText);
			index++;
		}
		strText.Empty();
		if( awardData->m_ulSP > 0 )
		{
			strTemp.Format(GetStringFromTable(3203), awardData->m_ulSP);
			strText += strTemp;
			strText += _AL("\r");
		}
		if( awardData->m_ulReputation > 0 )
		{
			strTemp.Format(GetStringFromTable(3205), awardData->m_ulReputation);
			strText += strTemp;
			strText += _AL("\r");
		}
		for(UINT i = 0; i < TASK_ZONE_FRIENDSHIP_COUNT; i++)
			if( awardData->m_aFriendships[i] > 0 )
			{
				strTemp.Format(GetStringFromTable(3210 + i), awardData->m_aFriendships[i]);
				strText += strTemp;
				strText += _AL("\r");
			}
		
		if( awardData->m_nFamilySkillProficiency )
		{
			strTemp.Format(GetStringFromTable(3431 + awardData->m_nFamilySkillIndex), awardData->m_nFamilySkillProficiency);
			strText += strTemp;
		}
		else if( awardData->m_nFamilySkillLevel )
			strText += GetStringFromTable(3441 + awardData->m_nFamilySkillIndex);
		if( strText != _AL("") )
		{
			GetGameUIMan()->SetImgIconSet(m_pImg_Award[index], CECGameUIMan::ICONS_DAILYPROMP32, "声望.dds");
			m_pImg_Award[index]->SetHint(strText);
			index++;
		}
		if( awardData->m_ulCandItems>0 )
		{
			const AWARD_ITEMS_CAND& ic = awardData->m_CandItems[0];
			if (ic.m_bRandChoose)
			{
				GetGameUIMan()->SetImgIconSet(m_pImg_Award[index], CECGameUIMan::ICONS_DAILYPROMP32, "未知物品.dds");
				m_pImg_Award[index]->SetHint(GetGameUIMan()->GetStringFromTable(3204));
				index++;
			}
			else
			{
				std::map<unsigned long, unsigned long> mapItemIdNum;
				for (unsigned long j = 0; j < ic.m_ulAwardItems; j++)
				{
					const ITEM_WANTED& wi = ic.m_AwardItems[j];
					
					//if (!wi.m_bCommonItem) continue;
					//else
						if (wi.m_fProb != 1.0f)
					{
						mapItemIdNum.clear();
						GetGameUIMan()->SetImgIconSet(m_pImg_Award[index], CECGameUIMan::ICONS_DAILYPROMP32, "未知物品.dds");
						m_pImg_Award[index]->SetHint(GetGameUIMan()->GetStringFromTable(3204));
						index++;
						break;
					}
					else
					{
						mapItemIdNum[wi.m_ulItemTemplId] = wi.m_ulItemNum;
					}
				}
				if (!mapItemIdNum.empty())
				{
					for (std::map<unsigned long, unsigned long>::iterator it = mapItemIdNum.begin(); it!=mapItemIdNum.end(); ++it)
					{
						m_pImg_Award[index]->Show(true);
						GetGameUIMan()->SetIvtrInfo(m_pImg_Award[index], it->first, it->second, true);
						index++;
						if (index>=6)
							break;
					}
				}

			}
		}
	}
	else
	{
		UINT i;
		for ( i=0; i<mInfo.vecAward.size(); i++ )
		{
			MenologyAward  mAward = mInfo.vecAward[i];
			if (mAward.iId>0)
			{
				GetGameUIMan()->SetIvtrInfo(m_pImg_Award[i], mAward.iId, 1, true);
			}
			else
			{
				GetGameUIMan()->SetImgIconSet(m_pImg_Award[i], CECGameUIMan::ICONS_DAILYPROMP32, mAward.strIcon);
				m_pImg_Award[i]->SetHint(mAward.strDesc);
			}
			m_pImg_Award[i]->Show(true);
			
		}
	}
}

void CDlgDailyPromp::OnEventLButtonDownListMonster(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	m_pTxt_BossName->SetText(_AL(""));
	m_pTxt_BossLv->SetText(_AL(""));
	m_pTxt_TaskNpc->SetText(_AL(""));
	m_pImg_Task->ClearCover();
	int i;
	for (i=0; i<AWARD_MAX; i++)
	{
		m_pImg_Award[i]->Show(false);
	}

	m_nCurSel = m_pLst_Task->GetCurSel();
	int nSel = m_pLst_Task->GetCurSel();
	if( nSel < 0 || nSel >= m_pLst_Task->GetCount() )
		return;
	
	MenologyInfoVector vecInfo = CECMenology::GetInstance()->GetInfo(m_nType);
	int index = m_pLst_Task->GetItemData(nSel, 0);
	MenologyInfo mInfo = vecInfo[index];
	ACString strText;
	ACString strHint;
	m_pTxt_BossName->SetText(mInfo.strName);
	strText.Format(_AL("%d"), mInfo.iBossLv);
	m_pTxt_BossLv->SetText(strText);

	int map_id = mInfo.iNpcMapid;
	strText.Empty();
	if (map_id>0)
	{
		int x, y, z;
		a_sscanf(mInfo.strNpcPos, _AL("%d,%d,%d"), &x, &y, &z);
		y = 0;
		CECInstance *pInstance = GetGameRun()->GetInstance(map_id);
		if( pInstance )
		{
			strText = pInstance->GetNameByCoord((float)x, (float)z);
		}
		else
		{
			strText.Empty();
		}
		if (x!=invalidPosTag&&z!=invalidPosTag)
		{
			EditBoxItemBase item(enumEICoord);
			item.SetName(strText);
			ACHAR szPos[100];
			a_sprintf(szPos, _AL("%d %d %d %d"), x, y, z, map_id);
			item.SetInfo(szPos);
			item.SetColor(A3DCOLORRGB(0, 255, 0));
			strText = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			strHint.Format(_AL("%d,%d"), x, z);
		}
		
		if (mInfo.bIsTransport)
		{
			strText += _AL(" ");
			EditBoxItemBase itemImage(enumEIImage);
			itemImage.SetImageIndex(CECGameUIMan::ICONLIST_TRANSFER);
			itemImage.SetImageFrame(0);
			ACHAR szId[100];
			a_sprintf(szId, _AL("%d"), m_pLst_Task->GetItemData(nSel, 2));
			itemImage.SetInfo(szId);
			strText += (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
		}
	}
	m_pTxt_TaskNpc->SetText(strText);
	m_pTxt_TaskNpc->SetHint(strHint);
	m_strHintNpc = strHint;
	
	if (m_bLoaded)
	{
		m_pModel->Release();
		m_bLoaded = false;
	}
	//if (mInfo.strIcon2.GetLength()==0)
	{
		elementdataman* pDB = GetGame()->GetElementDataMan();
		DATA_TYPE DataType;
		MONSTER_ESSENCE* pDBEssence = (MONSTER_ESSENCE*)pDB->get_data_ptr(mInfo.iId, ID_SPACE_ESSENCE, DataType);
		if(pDBEssence)
			mInfo.strIcon2 = GetGame()->GetDataPath(pDBEssence->file_model);
	}
	if ( m_pModel->Load(mInfo.strIcon2))
	{
//		if (m_pModel->GetSkinNum()==0)
		{
			A3DSkin* pSkin;
			
			char szSkin[MAX_PATH];
			strncpy(szSkin, mInfo.strIcon2, MAX_PATH);
			glb_ChangeExtension(szSkin, "ski");
			
			if (pSkin = GetGame()->LoadA3DSkin(szSkin, true))
				m_pModel->GetA3DSkinModel()->AddSkin(pSkin, true);
		}
		m_pModel->SetGfxUseLOD(false);
		m_pModel->SetPos(A3DVECTOR3(0));
		m_pModel->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
		m_pModel->PlayActionByName(szActName[0], 1.0f, true);
		m_pModel->SetAutoUpdateFlag(false);
		m_bLoaded = true;
	}
	GetGameUIMan()->SetImgIconSet(m_pImg_Task, CECGameUIMan::ICONS_DAILYPROMP48, mInfo.strIcon2);
	m_pTxt_Desc->SetText(vecInfo[index].strDesc);
	if (mInfo.vecAward.size()>0)
	{
		for ( UINT i=0; i<mInfo.vecAward.size(); i++ )
		{
			MenologyAward  mAward = mInfo.vecAward[i];
			if (mAward.iId>0)
			{
				GetGameUIMan()->SetIvtrInfo(m_pImg_Award[i], mAward.iId, 1, true);
			}
			else
			{
				GetGameUIMan()->SetImgIconSet(m_pImg_Award[i], CECGameUIMan::ICONS_DAILYPROMP32, mAward.strIcon);
				m_pImg_Award[i]->SetHint(mAward.strDesc);
			}
			m_pImg_Award[i]->Show(true);
			
		}
	}
}

void CDlgDailyPromp::OnEventLButtonDownNpc(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	
	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	GetItemLinkOnText(x, y, (PAUITEXTAREA)pObj, &Item);
	
	if( Item.m_pItem != NULL )
	{
		if( Item.m_pItem->GetType() == enumEICoord )
		{
			A3DVECTOR3 vec;
			int idInstance;
			a_sscanf(Item.m_pItem->GetInfo(), _AL("%f%f%f%d"), 
				&vec.x, &vec.y, &vec.z, &idInstance);
			((CDlgMiniMap*)GetGameUIMan()->GetDialog("Win_MiniMap"))->m_dwTempTargetTime = GetTickCount();
			((CDlgMiniMap*)GetGameUIMan()->GetDialog("Win_MiniMap"))->m_vecTempTarget = vec;
			((CDlgMiniMap*)GetGameUIMan()->GetDialog("Win_MiniMap"))->m_idTempTargetInstance = idInstance;
			
			g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, idInstance);
			g_AutoMoveDestName = Item.m_pItem->GetName();
			
			if( idInstance == GetWorld()->GetInstanceID() && GetHostPlayer()->GetModelTempArrow() )
				GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
		}
		else if( Item.m_pItem->GetType() == enumEIImage )
		{
			int id;
			a_sscanf(Item.m_pItem->GetInfo(), _AL("%d"), &id);
			GetGameUIMan()->CheckCanDailyPrompFly(id);
		}
	}
}

void CDlgDailyPromp::OnEventMouseMoveNpc(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	GetItemLinkOnText(x, y, (PAUITEXTAREA)pObj, &Item);
	pObj->SetHint(m_strHintNpc);
	if( Item.m_pItem != NULL )
	{
		if( Item.m_pItem->GetType() == enumEIImage )
		{
			pObj->SetHint(GetGameUIMan()->GetStringFromTable(3302));
		}
	}	
}

void CDlgDailyPromp::UpdateDaily()
{
	m_nType = TYPE_RECOMMAND;
	MenologyInfoVector vecInfo = CECMenology::GetInstance()->GetInfo(TYPE_RECOMMAND);
	MenologyInfoVector::iterator it = vecInfo.begin();
	ACString strText;
	ACString strIcon;
	ACString strAwdExp, strAwdMoney, strAwdItem;
	ACString strHintExp, strHintMoney, strHintItem;
	ACString strStatus;
	int iFinishCount=0;
	CECTaskInterface* pTask = GetHostPlayer()->GetTaskInterface();
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	m_pLst_Task->ResetContent();
	for ( ; it!=vecInfo.end(); ++it)
	{
		iFinishCount = 0;
		if (!DailyShowToday(it))
		{
			continue;
		}
		int iStatus = 0;
		int idTask = GetSuitTask(it->uiTaskID, it->uiNumID);
		ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);
		if (pTemp)
		{
			if (pTemp->m_ulMaxFinishCount>0)
			{
				TaskFinishTimeList* pList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
				TaskFinishTimeEntry* pEntry = pList->Search(idTask);
				if (pEntry)
					iFinishCount = pEntry->m_uFinishCount;
			}
			else if (it->iPermitCount==1&&pTemp->CheckDeliverTime(pTask, pTask->GetCurTime()))
			{
				iFinishCount = 1;
			}
		}
		tm tmNow = GetGame()->GetServerLocalTime();
		int nowMinute = tmNow.tm_hour*60 + tmNow.tm_min;
		if ( iFinishCount==it->iPermitCount )
			iStatus = 2;
		else if ( nowMinute<(it->tmStartTime.tm_hour*60+it->tmStartTime.tm_min) )
			iStatus = 0;
		else if ( nowMinute>(it->tmEndTime.tm_hour*60+it->tmEndTime.tm_min) )
			iStatus = 3;
		else
			iStatus = 1;
		EditBoxItemBase item(enumEIImage);
		item.SetImageIndex(ICONTYPE_TASK32);
		item.SetImageFrame(GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_DAILYPROMP32, it->strIcon1));
		if( (iStatus==1||iStatus==2) && CheckLevel(it) )
			item.SetColor(A3DCOLORRGB(255,255,255));
		else
			item.SetColor(A3DCOLORRGB(128,128,128));
		strIcon = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
		GetExpAward(it->iRewardType, strAwdExp, strHintExp);
		GetMoneyAward(it->iRewardType, strAwdMoney, strHintMoney);
		GetItemAward(it->iRewardType, strAwdItem, strHintItem);
		
		static const char* szName[] = {	"未完成.dds", "进行中.dds", "已完成.dds",  "已结束.dds"};
		EditBoxItemBase itemImage(enumEIImage);
		itemImage.SetImageIndex(ICONTYPE_TASK32);
		itemImage.SetImageFrame(GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_DAILYPROMP32, szName[iStatus]));
		strStatus = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
		
		if (it->iPermitCount==-1)
		{
			strText.Format( _AL("%s\t%s\t%s\t%s\t%s\t%s\t%d\t%s\t%s\t%s"), 
				strIcon, strAwdExp, strAwdItem, strAwdMoney, it->strName, it->strTime, it->ulMinLevel, 
				/*iFinishCount,*/ GetGameUIMan()->GetStringFromTable(7140), it->strCondition, strStatus );
		}
// 		else if (it->iPermitCount>1)
// 		{
// 			strText.Format( _AL("%s\t%s\t%s\t%s\t%s\t%s\t%d\t%d\t%s\t%s"), 
// 				strIcon, strAwdExp, strAwdItem, strAwdMoney, it->strName, it->strTime, it->ulMinLevel, 
// 				/*iFinishCount,*/ it->iPermitCount, it->strCondition, strStatus );
// 		}
		else
			strText.Format( _AL("%s\t%s\t%s\t%s\t%s\t%s\t%d\t%d/%d\t%s\t%s"), 
				strIcon, strAwdExp, strAwdItem, strAwdMoney, it->strName, it->strTime, it->ulMinLevel, 
				iFinishCount, it->iPermitCount, it->strCondition, strStatus );
		m_pLst_Task->AddString(strText);
		it->iCurTaskShow = GetSuitTask(it->uiTaskID, it->uiNumID);
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it-vecInfo.begin(), 0);	//index
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it->iCurTaskShow, 1);	//taskid
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it->iId, 2);			//id
		m_pLst_Task->SetItemColHint(m_pLst_Task->GetCount()-1, 1, strHintExp);
		m_pLst_Task->SetItemColHint(m_pLst_Task->GetCount()-1, 2, strHintItem);
		m_pLst_Task->SetItemColHint(m_pLst_Task->GetCount()-1, 3, strHintMoney);
		m_pLst_Task->SetItemColHint(m_pLst_Task->GetCount()-1, 9, GetGameUIMan()->GetStringFromTable(10433+iStatus));
	}
	m_pLst_Task->SetCurSel(0);
	OnEventLButtonDownList(0, 0, 0);
}

void CDlgDailyPromp::UpdateWeekly()
{
	m_nType = TYPE_WEEKLY;
	MenologyInfoVector vecInfo = CECMenology::GetInstance()->GetInfo(TYPE_WEEKLY);
	MenologyInfoVector::iterator it = vecInfo.begin();
	ACString strText;
	ACString strIcon;
	ACString strAwdExp, strAwdMoney, strAwdItem;
	ACString strHintExp, strHintMoney, strHintItem;
	DWORD dwCurDay = GetGame()->GetServerLocalTime().tm_wday;
	m_pLst_Task->ResetContent();
	for ( ; it!=vecInfo.end(); ++it)
	{
		EditBoxItemBase item(enumEIImage);
		item.SetImageIndex(ICONTYPE_TASK32);
		item.SetImageFrame(GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_DAILYPROMP32, it->strIcon1));
		if( CheckLevel(it) )
			item.SetColor(A3DCOLORRGB(255,255,255));
		else
			item.SetColor(A3DCOLORRGB(128,128,128));
		strIcon = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
		GetExpAward(it->iRewardType, strAwdExp, strHintExp);
		GetMoneyAward(it->iRewardType, strAwdMoney, strHintMoney);
		GetItemAward(it->iRewardType, strAwdItem, strHintItem);
		
		strText.Format( _AL("%s\t%s\t%s\t%s\t%s\t%s\t%d\t%s\t%s"), strIcon, strAwdExp, strAwdItem, strAwdMoney,
			it->strName, it->strTime, it->ulMinLevel, it->strDisplayDays, it->strCondition );
		m_pLst_Task->AddString(strText);
		it->iCurTaskShow = GetSuitTask(it->uiTaskID, it->uiNumID);
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it-vecInfo.begin(), 0);
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it->iCurTaskShow, 1);
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it->iId, 2);			//id
		m_pLst_Task->SetItemColHint(m_pLst_Task->GetCount()-1, 1, strHintExp);
		m_pLst_Task->SetItemColHint(m_pLst_Task->GetCount()-1, 2, strHintItem);
		m_pLst_Task->SetItemColHint(m_pLst_Task->GetCount()-1, 3, strHintMoney);
	}
	m_pLst_Task->SetCurSel(0);
	OnEventLButtonDownList(0, 0, 0);
}

void CDlgDailyPromp::UpdateBattleInfo()
{
	m_nType = TYPE_BATTLEFIELD;
	MenologyInfoVector vecInfo = CECMenology::GetInstance()->GetInfo(TYPE_BATTLEFIELD);
	MenologyInfoVector::iterator it = vecInfo.begin();
	ACString strText;
	ACString strIcon;
	ACString strWeek;
	DWORD dwCurDay = GetGame()->GetServerLocalTime().tm_wday;
	m_pLst_Task->ResetContent();
	for ( ; it!=vecInfo.end(); ++it)
	{
		EditBoxItemBase item(enumEIImage);
		item.SetImageIndex(ICONTYPE_TASK32);
		item.SetImageFrame(GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_DAILYPROMP32, it->strIcon1));
		if( CheckLevel(it) )
			item.SetColor(A3DCOLORRGB(255,255,255));
		else
			item.SetColor(A3DCOLORRGB(128,128,128));
		strIcon = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();

		if (it->iPermitCount==-1)
			strText.Format( _AL("%s\t%s\t%s\t%s\t%d\t%s"), strIcon, 
				it->strName, it->strDisplayDays, it->strTime, it->ulMinLevel, GetGameUIMan()->GetStringFromTable(7140) );
		else
			strText.Format( _AL("%s\t%s\t%s\t%s\t%d\t%d"), strIcon, 
				it->strName, it->strDisplayDays, it->strTime, it->ulMinLevel, it->iPermitCount );
		m_pLst_Task->AddString(strText);
		it->iCurTaskShow = GetSuitTask(it->uiTaskID, it->uiNumID);
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it-vecInfo.begin(), 0);
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it->iCurTaskShow, 1);
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it->iId, 2);			//id
	}
	m_pLst_Task->SetCurSel(0);
	OnEventLButtonDownList(0, 0, 0);
}

void CDlgDailyPromp::UpdateMonster()
{
	m_nType = TYPE_MONSTER;
	MenologyInfoVector vecInfo = CECMenology::GetInstance()->GetInfo(TYPE_MONSTER);
	MenologyInfoVector::iterator it = vecInfo.begin();
	ACString strText;
	ACString strIcon;
	ACString strLv;
	DWORD dwCurDay = GetGame()->GetServerLocalTime().tm_wday;
	m_pLst_Task->ResetContent();
	bool b15Only = ((PAUICHECKBOX)GetDlgItem("Chk_15Boss"))->IsChecked();
	int hostLv = GetHostPlayer()->GetBasicProps().iLevel;
	for ( ; it!=vecInfo.end(); ++it)
	{
		if (b15Only && (hostLv>(it->iBossLv+15)||(hostLv+15)<it->iBossLv))
			continue;

		EditBoxItemBase item(enumEIImage);
		item.SetImageIndex(ICONTYPE_TASK32);
		item.SetImageFrame(GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_DAILYPROMP32, it->strIcon1));
		strIcon = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
		strLv.Format(_AL("%d"), it->iBossLv);
		strText.Format( _AL("%s\t%s\t%s\t%s\t%s\t%s"), strIcon, it->strName, strLv,
			it->strBossPeriod, it->strBossRecommendLv, it->strLineInfo );
		it->iCurTaskShow = GetSuitTask(it->uiTaskID, it->uiNumID);
		m_pLst_Task->AddString(strText);
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it-vecInfo.begin(), 0);
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it->iCurTaskShow, 1);
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it->iId, 2);			//id
	}
	m_pLst_Task->SetCurSel(0);
	OnEventLButtonDownListMonster(0, 0, 0);
}

int CDlgDailyPromp::GetSuitTask(const unsigned int* tasks, const int num)
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

bool CDlgDailyPromp::GetExpAward(const int reward, ACString& strAward, ACString& strHint)
{
	static const char* szName[] = 
	{
		"修为1.dds", "修为2.dds", "修为3.dds"
	};
	strAward = _AL(" ");
	strHint.Empty();
	ACString strText;
	int index = reward%10;
	if ( index>0&&index<4 )
	{
		EditBoxItemBase itemImage(enumEIImage);
		itemImage.SetImageIndex(ICONTYPE_TASK24);
		itemImage.SetImageFrame(GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_DAILYPROMP24, szName[index-1]));
		strAward = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
		strHint = GetGameUIMan()->GetStringFromTable(10420+index);
		return true;
	}
	return false;
}

bool CDlgDailyPromp::GetMoneyAward(const int reward, ACString& strAward, ACString& strHint)
{
	static const char* szName[] = 
	{
		"金钱1.dds", "金钱2.dds", "金钱3.dds"
	};
	strAward = _AL(" ");
	strHint.Empty();
	ACString strText;
	int index = (reward%100)/10;
	if ( index>0&&index<4 )
	{
		EditBoxItemBase itemImage(enumEIImage);
		itemImage.SetImageIndex(ICONTYPE_TASK24);
		itemImage.SetImageFrame(GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_DAILYPROMP24, szName[index-1]));
		strAward = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
		strHint = GetGameUIMan()->GetStringFromTable(10423+index);
		return true;
	}
	return false;
}

bool CDlgDailyPromp::GetItemAward(const int reward, ACString& strAward, ACString& strHint)
{
	static const char* szName[] = 
	{
		"物品1.dds", "物品2.dds", "物品3.dds"
	};
	strAward = _AL(" ");
	strHint.Empty();
	ACString strText;
	int index = reward/100;
	if ( index>0&&index<4 )
	{
		EditBoxItemBase itemImage(enumEIImage);
		itemImage.SetImageIndex(ICONTYPE_TASK24);
		itemImage.SetImageFrame(GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_DAILYPROMP24, szName[index-1]));
		strAward = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
		strHint = GetGameUIMan()->GetStringFromTable(10426+index);
		return true;
	}
	return false;
}

bool CDlgDailyPromp::CheckLevel(const MenologyInfo* info)
{
	unsigned long iLv = GetHostPlayer()->GetBasicProps().iLevel;
	if (!info->bReborn)
		return iLv>=info->ulMinLevel && iLv<=info->ulMaxLevel;
	else if (GetHostPlayer()->GetRebornInfo().size()>0)
		return iLv>=info->ulMinLevel && iLv<=info->ulMaxLevel;

	return false;

}

bool CDlgDailyPromp::DailyShowToday(const MenologyInfo* info)
{
	if (info->vecTimes.size()==0)
	{
		return info->bDays[g_pGame->GetServerLocalTime().tm_wday];
	}
	CECCalendar tmToday = g_pGame->GetServerLocalTime();
	UINT nSizeTime = info->vecTimes.size();
	for (UINT i=0; i<nSizeTime; i++)
	{
		if(info->vecTimes[i].tmStart<=tmToday && info->vecTimes[i].tmEnd>=tmToday)
			return true;
	}
	return false;
}