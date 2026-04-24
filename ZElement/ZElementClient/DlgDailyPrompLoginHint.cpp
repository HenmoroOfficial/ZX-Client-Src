// Filename	:	DlgDailyPrompLoginHint.cpp
// Creator	:	Fu Chonggang
// Date		:	Sep 14, 2010

#include "DlgDailyPrompLoginHint.h"
#include "DlgDailyPromp.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUICheckBox.h"
#include "EC_Configs.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_Menology.h"
#include "EC_HostPlayer.h"
#include "EC_AutoMove.h"
#include "TaskTemplMan.h"
#include "EC_TaskInterface.h"
#include "EC_GameSession.h"
#include "EC_Secretary.h"
#include "EC_GameUIMisc.h"
#include "gt_overlay.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgDailyPrompLoginHint, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_TaskDaily",			OnCommand_taskdaily)
AUI_ON_COMMAND("Chk_NeverHint",			OnCommand_LoginShowState)
AUI_ON_COMMAND("confirm",				OnCommand_Exit)
AUI_ON_COMMAND("Btn_Close",				OnCommand_close)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgDailyPrompLoginHint, CDlgBase)

AUI_ON_EVENT("Lst_TaskDailyList",		WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Lst_TaskDailyList",		WM_MOUSEMOVE,		OnEventMouseMove)

AUI_END_EVENT_MAP()
extern ACString g_AutoMoveDestName;
extern CECGame* g_pGame;
CDlgDailyPrompLoginHint::CDlgDailyPrompLoginHint()
{
}

CDlgDailyPrompLoginHint::~CDlgDailyPrompLoginHint()
{
}

bool CDlgDailyPrompLoginHint::Release()
{
	return CDlgBase::Release();
}

bool CDlgDailyPrompLoginHint::OnInitDialog()
{
	m_pLst_TaskDailyList = (AUIListBox *)GetDlgItem("Lst_TaskDailyList");
	m_pLab_List = (AUILabel *)GetDlgItem("Lab_List");
	m_pBtn_TaskDaily = (AUIStillImageButton *)GetDlgItem("Btn_TaskDaily");
	m_pBtn_Close = (AUIStillImageButton *)GetDlgItem("Btn_Close");
	m_pTitle = (AUILabel *)GetDlgItem("Title");
	m_pChk_NeverHint = (AUICheckBox *)GetDlgItem("Chk_NeverHint");
	m_pChk_ExitOffExp = (PAUICHECKBOX)GetDlgItem("Chk_OffExp");
	m_vecImageList.push_back(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_DAILYPROMP32));
	m_pLst_TaskDailyList->SetImageList(&m_vecImageList);
	m_pLst_TaskDailyList->SetItemMask(0xffffffff);
	return true;
}
bool CDlgDailyPrompLoginHint::HasAnyDailyPrompToComplete()
{
	bool bHasAnyDailyPromp = false;
	MenologyInfoVector vecInfo = CECMenology::GetInstance()->GetInfo(CECMenology::TYPE_RECOMMAND);
	MenologyInfoVector::iterator it = vecInfo.begin();
	CECTaskInterface* pTask = g_pGame->GetGameRun()->GetHostPlayer()->GetTaskInterface();
	TaskFinishTimeList* pList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
	tm tmNow = g_pGame->GetServerLocalTime();
	int nowMinute = tmNow.tm_hour*60 + tmNow.tm_min;
	bool bCrsServer = g_pGame->GetGameRun()->GetHostPlayer()->GetSrcServerID()!=0;
	for ( ; it!=vecInfo.end(); ++it)
	{
		if (!CDlgDailyPromp::DailyShowToday(it))
			continue;

		int idTask = GetSuitTask((*it).uiTaskID, (*it).uiNumID);
		// 是否可接
		if(pTask->CanDeliverTask(idTask)!=0)
			continue;

		// 跨服只显示跨服任务
		if (bCrsServer)
		{
			const ATaskTempl* pTempl = g_pGame->GetTaskTemplateMan()->GetTaskTemplByID(idTask);
			if (!pTempl || pTempl->GetType()!=14)// 封神任务
			{
				continue;
			}
		}

		if ( nowMinute<((*it).tmStartTime.tm_hour*60+(*it).tmStartTime.tm_min) ||
			nowMinute>((*it).tmEndTime.tm_hour*60+(*it).tmEndTime.tm_min) )
			continue;
		bHasAnyDailyPromp = true;
	}
	return bHasAnyDailyPromp;
}
void CDlgDailyPrompLoginHint::OnShowDialog()
{
	ACString strText;
	MenologyInfoVector vecInfo = CECMenology::GetInstance()->GetInfo(CECMenology::TYPE_RECOMMAND);
	MenologyInfoVector::iterator it = vecInfo.begin();
	CECTaskInterface* pTask = GetHostPlayer()->GetTaskInterface();
	TaskFinishTimeList* pList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
	tm tmNow = GetGame()->GetServerLocalTime();
	int nowMinute = tmNow.tm_hour*60 + tmNow.tm_min;
	m_pLst_TaskDailyList->ResetContent();
	bool bCrsServer = GetHostPlayer()->GetSrcServerID()!=0;
	for ( ; it!=vecInfo.end(); ++it)
	{
		if (!CDlgDailyPromp::DailyShowToday(it))
			continue;
		
		int idTask = GetSuitTask((*it).uiTaskID, (*it).uiNumID);
		// 是否可接
		if(pTask->CanDeliverTask(idTask)!=0)
			continue;

		// 跨服只显示跨服任务
		if (bCrsServer)
		{
			const ATaskTempl* pTempl = GetGame()->GetTaskTemplateMan()->GetTaskTemplByID(idTask);
			if (!pTempl || pTempl->GetType()!=14)// 封神任务
			{
				continue;
			}
		}

		if ( nowMinute<((*it).tmStartTime.tm_hour*60+(*it).tmStartTime.tm_min) ||
			 nowMinute>((*it).tmEndTime.tm_hour*60+(*it).tmEndTime.tm_min) )
			 continue;

		EditBoxItemBase item(enumEIImage);
		item.SetImageIndex(0);
		item.SetImageFrame(GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_DAILYPROMP32, (*it).strIcon1));
		item.SetImageScale(0.5f);
		ACString strIcon = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
 		strText.Format(_AL("%s\t%s\t%s"), strIcon, (*it).strName, (*it).strTime);
		m_pLst_TaskDailyList->AddString(strText);
		m_pLst_TaskDailyList->SetItemData(m_pLst_TaskDailyList->GetCount()-1, (*it).iNpcMapid, 0);
		if ((*it).iNpcMapid>0)
		{
			int x, y, z;
			a_sscanf((*it).strNpcPos, _AL("%d,%d,%d"), &x, &y, &z);
			m_pLst_TaskDailyList->SetItemData(m_pLst_TaskDailyList->GetCount()-1, x, 1);
			m_pLst_TaskDailyList->SetItemData(m_pLst_TaskDailyList->GetCount()-1, z, 2);
		}
	}
	EC_GAME_CLIENTDATA gcd = GetGame()->GetConfigs()->GetGameClientData();
	if ( 0 == stricmp(GetName(), "Win_DailyPrompExit") )
	{
		m_pChk_ExitOffExp->Check(false);
		if (GetHostPlayer()->GetAgentTime()<=0)
		{
			m_pChk_ExitOffExp->Enable(false);
		}
		else
		{
			m_pChk_ExitOffExp->Enable(true);
		}
	}
	else
		m_pChk_NeverHint->Check(gcd.nHideDailyPrompLoginHint!=0);
}

void CDlgDailyPrompLoginHint::OnTick()
{
}

bool CDlgDailyPrompLoginHint::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

int CDlgDailyPrompLoginHint::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	return 1;
}

void CDlgDailyPrompLoginHint::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgDailyPrompLoginHint::OnCommand_taskdaily(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_DailyPromp_Today");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgDailyPrompLoginHint::OnCommand_LoginShowState(const char * szCommand)
{
	EC_GAME_CLIENTDATA gcd = GetGame()->GetConfigs()->GetGameClientData();
	gcd.nHideDailyPrompLoginHint = m_pChk_NeverHint->IsChecked();
	GetGame()->GetConfigs()->SetGameClientData(gcd);
}

void CDlgDailyPrompLoginHint::OnCommand_Exit(const char * szCommand)
{
	PAUICHECKBOX pChk_Forcequit = (PAUICHECKBOX)GetDlgItem("Chk_Forcequit");
	if( pChk_Forcequit->IsChecked() )
		GetGameRun()->PostMessage(MSG_EXITGAME, -1, 0);
	else if( GetGameSession()->IsConnected() )
	{
		GetGameRun()->SaveConfigsToServer();
		if( m_pChk_ExitOffExp->IsChecked() )
		{
			GetGameSession()->c2s_CmdLogout(_PLAYER_LOGOUT_FULL, 1);
		}
		else
		{
			GetGameSession()->c2s_CmdLogout(_PLAYER_LOGOUT_FULL);
		}
	}
	else
		GetGameRun()->SetLogoutFlag(0);

	overlay::GTOverlay::Instance().Logout();
	
	Show(false);
}

void CDlgDailyPrompLoginHint::OnCommand_close(const char * szCommand)
{
	Show(false);
}

void CDlgDailyPrompLoginHint::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	if (GetItemLinkOnList(x, y, m_pLst_TaskDailyList))
	{
		int index = m_pLst_TaskDailyList->GetCurSel();
		int idInstance = m_pLst_TaskDailyList->GetItemData(index, 0);
		if (idInstance>0)
		{
			A3DVECTOR3 vec((float)m_pLst_TaskDailyList->GetItemData(index, 1),
				0,
				(float)m_pLst_TaskDailyList->GetItemData(index, 2));
			g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, idInstance);
		}
	}
}

void CDlgDailyPrompLoginHint::OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	GetItemLinkOnList(x, y, m_pLst_TaskDailyList);
}


int CDlgDailyPrompLoginHint::GetSuitTask(const unsigned int* tasks, const int num)
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