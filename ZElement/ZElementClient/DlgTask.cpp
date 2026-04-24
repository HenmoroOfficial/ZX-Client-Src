// Filename	: DlgTask.cpp
// Creator	: Tom Zhou
// Date		: October 12, 2005

#include "AFI.h"
#include "ainifile.h"
#include "AFileImage.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\CSplit.h"
#include "DlgTask.h"
#include "DlgMiniMap.h"
#include "DlgTaskZhuXianList.h"
#include "DlgMinimizeBar.h"
#include "EC_Faction.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_Model.h"
#include "EC_World.h"
#include "EC_GameRun.h"
#include "EC_Resource.h"
#include "EC_Instance.h"
#include "EC_TaskInterface.h"
#include "EC_Global.h"
#include "TaskTemplMan.h"
#include "elementdataman.h"
#include "EC_IvtrItem.h"
#include "EC_AutoMove.h"
#include "EC_InputCtrl.h"
#include "EC_Inventory.h"
#include "EC_Configs.h"
#include "EC_UIManager.h"
#include "EC_UIData.h"
#include "A2DSprite.h"
#include "EC_GameUIMisc.h"
#include "LuaEvent.h"
#include "EC_HelpGraphGuide.h"
#include "EC_Algorithm.h"
#include "DlgPKMsg.h"
#include "DlgBroadCastInGame.h"
#include "EC_NewHelpGuide.h"


#define new A_DEBUG_NEW

extern ACString g_AutoMoveDestName;
//#define AUTO_OPEN_SEARCHTASK

AUI_BEGIN_COMMAND_MAP(CDlgTask, CDlgBase)

AUI_ON_COMMAND("focusall",		OnCommand_focusall)
AUI_ON_COMMAND("focus",			OnCommand_focus)
AUI_ON_COMMAND("abandon",		OnCommand_abandon)
AUI_ON_COMMAND("taskdaily",		OnCommand_TaskDaily)
AUI_ON_COMMAND("combotype",		OnCommand_ComboType)
AUI_ON_COMMAND("rdo*",			OnCommand_RdoType)
AUI_ON_COMMAND("Btn_Return",	OnCommand_Return)
AUI_ON_COMMAND("Btn_Accept",	OnCommand_Accept)
AUI_ON_COMMAND("Btn_Ancient",	OnCommand_Ancient)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Prev",		OnCommand_Prev)
AUI_ON_COMMAND("Btn_Next",		OnCommand_Next)
AUI_ON_COMMAND("Btn_Type2_Prev",		OnCommand_Prev2)
AUI_ON_COMMAND("Btn_Type2_Next2",		OnCommand_Next2)
AUI_ON_COMMAND("Btn_Minimize",		OnCommand_Minimize)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgTask, CDlgBase)

AUI_ON_EVENT("Tv_Quest",		WM_LBUTTONDOWN,		OnEventLButtonDown_Tv_Quest)
AUI_ON_EVENT("Txt_Content",		WM_LBUTTONDOWN,		OnEventLButtonDown_Txt_QuestItem)
AUI_ON_EVENT("Txt_Content",		WM_MOUSEMOVE,		OnEventMouseMove_Txt_QuestItem)
AUI_ON_EVENT("Txt_QuestItem",	WM_LBUTTONDOWN,		OnEventLButtonDown_Txt_QuestItem)
AUI_ON_EVENT("Txt_QuestItem",	WM_MOUSEMOVE,		OnEventMouseMove_Txt_QuestItem)

AUI_ON_EVENT("Img_ZoomIn",		WM_LBUTTONDOWN,		OnEventLButtonDown_Img_ZoomIn)
AUI_ON_EVENT("Img_ZoomIn",		WM_LBUTTONUP,		OnEventLButtonUp_Img_ZoomIn)
AUI_ON_EVENT("Img_ZoomIn",		WM_MOUSEMOVE,		OnEventMouseMove_Img_ZoomIn)
AUI_ON_EVENT("Tv_List",			WM_LBUTTONDOWN,		OnEventLButtonDown_TvTaskTrace)
AUI_ON_EVENT("Tv_List",			WM_MOUSEMOVE,		OnEventMouseMove_TvTaskTrace)

AUI_END_EVENT_MAP()

abase::vector<int> CDlgTask::m_vecTraceTaskID;
abase::hash_map<int, bool> CDlgTask::m_mapTaskMonster;
int	CDlgTask::m_idFocus = 0;
const static int s_TypeShowOrder[] = {1, 2, 3, 4,5};

#define TASK_SORT_ONTOP			4	// 置顶
#define TASK_SORT_RATIO			3	// 按比例
#define TASK_SORT_ITEM			2	// 奖励物品（金丹，通宝）
#define TASK_SORT_FIXED			1	// 固定奖励（金钱，经验）
#define TASK_SORT_DEFAULT		0	// 默认

#define TASK_REACHSITE_AUTOMOVE 999

#define TASK_TYPE_DATA_BASE		10000000	// 任务类型条目数据初始值 

const int TaskStateColor[] =
{
	0xffffd562,		//TS_FINISH
	0xffa1ef4d,		//TS_DOING
	0xffa1ef4d,		//TS_CANDELIVER
	0xffc0c0c0,		//TS_TODO
};

class CompareSortTaskShowOrder
{
public:
	bool operator ()( SORT_TASK* p1, SORT_TASK * p2) // p1应该大于p2
	{
		if(p1->nSortType > p2->nSortType)
			return true;
		else if(p1->nSortType < p2->nSortType)
			return false;
		else // p1->nSortType == p2->nSortType
		{
			if(p1->nSortValue >= p2->nSortValue)
				return true;
			else // (p1->nSortValue < p2->nSortValue)
				return false;			
		}
	}
};


#define TASK_TIME_LIMIT_MASK 0x80000000
#define TASK_PROTECTNPC_MASK 0x40000000
#define TASK_REACHSITE_MASK  0x20000000

CDlgTask::CDlgTask()
{
	m_idLastTask = -2;
	m_nAwardItemShowIndex = 0;
	m_nAwardItemShowIndex2 = 0;
	m_vecTraceTaskID.clear();
	m_mapTaskMonster.clear();
	m_bMinimized = false;
}

CDlgTask::~CDlgTask()
{
}

bool CDlgTask::OnInitDialog()
{
	if( GetDlgItem("Quest_D") )
		GetDlgItem("Quest_D")->SetAcceptMouseMessage(false);
	m_pText = (PAUILABEL)GetDlgItem("text");
	m_pTxt_QuestNO = (PAUILABEL)GetDlgItem("Txt_Limit");
	m_pTv_Quest = (PAUITREEVIEW)GetDlgItem("Tv_Quest");
	m_pTxt_Content = (PAUITEXTAREA)GetDlgItem("Txt_Content");
//	m_pTxt_QuestItem = (PAUITEXTAREA)GetDlgItem("Txt_QuestItem");
	m_pBtn_Focus = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Focus");
	m_pBtn_Abandon = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Abandon");
	m_pTxt_BaseAward = (PAUILABEL)GetDlgItem("Txt_BaseAward");
	m_pTxt_ItemAward = (PAUILABEL)GetDlgItem("Txt_ItemAward");
	m_pChk_FocusAll = (PAUICHECKBOX)GetDlgItem("Chk_FocusAll");
	m_pRdo3 = GetDlgItem("Rdo_3");
	m_pAncient = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Ancient");

	for(int i = 0; i < CDLGTASK_AWARDITEM_MAX ; i++ )
	{
		char szText[20];
		sprintf(szText, "Img_Item%d", i);
		m_pImg_Item[i] = (PAUIIMAGEPICTURE)GetDlgItem(szText);

		sprintf(szText, "Img_Type2_Item%d", i);
		m_pImg_Item2[i] = (PAUIIMAGEPICTURE)GetDlgItem(szText);
	}

	if( m_pTxt_QuestNO ) m_pTxt_QuestNO->SetText(_AL("0"));

	if( stricmp(m_szName, "Win_Quest") == 0 )
	{
		m_iCurPage = 0;
		m_pTxt_Intro		= (PAUITEXTAREA)GetDlgItem("Txt_Introduction");
		m_pTxt_Intro->Show(false);
		m_pImg_IntroPic		= (PAUIIMAGEPICTURE)GetDlgItem("Img_Bg");
		m_pImg_IntroPic->Show(false);
		m_pBtn_Accept = (AUIStillImageButton *)GetDlgItem("Btn_Accept");

		m_pBtn_Prev		= (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Prev");
		m_pBtn_Prev->Show(false);
		m_pBtn_Next		= (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Next");
		m_pBtn_Next->Show(false);
		
		m_pBtn_Prev2		= (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Type2_Prev");
		m_pBtn_Prev2->Show(false);
		m_pBtn_Next2		= (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Type2_Next2");
		m_pBtn_Next2->Show(false);

		CheckRadioButton(1,1);
		
//		m_pTxt_QuestItem->SetImageList(GetGameUIMan()->GetControlImgLst());
//		m_pTxt_QuestItem->SetItemMask(0xffffffff);
		A2DSprite* pImage = new A2DSprite;
		if (!pImage)
		{
//			return AUI_ReportError(__LINE__, __FILE__);
			return false;
		}
		
		if (!pImage->Init(m_pA3DDevice, "icon\\Transfer.tga", AUI_COLORKEY))
		{
			A3DRELEASE(pImage);
//			return AUI_ReportError(__LINE__, __FILE__);
			return false;
		}
		
		A2DSprite* pImage2 = new A2DSprite;
		if (!pImage2)
			return false;
		
		if (!pImage2->Init(m_pA3DDevice, "button\\radio.tga", AUI_COLORKEY))	// for test by ice, should be "Radio.tga" or similar
		{
			A3DRELEASE(pImage2);
			return false;
		}

		m_vecImageList.push_back(pImage);
		m_vecImageList.push_back(pImage2);
		m_iFixSpriteNum = 2;
		m_pTxt_Content->SetImageList(&m_vecImageList);
		GetGameUIMan()->AttachTaskEmotion(m_pTxt_Content);

		m_pTv_Quest->SetImageList(GetGameUIMan()->GetControlImgLst());
		m_pTv_Quest->SetItemMask(0xffffffff);
		
		AddTabDialog(this);	
		for(int i=0; i<TT_NUM; ++i)
			ms_bTaskCanDeliverTreeExpand[i] = true;
		
		GetDlgItem("Shape_1")->Show(false);
		GetDlgItem("Img_1")->Show(false);
		GetDlgItem("Txt_1")->Show(false);
	}
	else if ( stricmp(m_szName, "Win_QuestMinion") == 0 )
	{
		m_pTv_TraceContent = (PAUITREEVIEW)GetDlgItem("Tv_List");
		GetDlgItem("Edt_Back")->SetTransparent(true);
		if( GetDlgItem("Img_ZoomIn") )
			GetDlgItem("Img_ZoomIn")->SetAcceptMouseMessage(true);

		m_pTv_TraceContent->SetImageList(GetGameUIMan()->GetControlImgLst());
		m_pTv_TraceContent->SetItemMask(0xffffffff);
		
		GetDlgItem("Shape_1")->Show(false);
		GetDlgItem("Img_1")->Show(false);
		GetDlgItem("Txt_1")->Show(false);
	}

	return true;
}

bool CDlgTask::Release()
{
	if( stricmp(m_szName, "Win_Quest") == 0 )
	{
		AIniFile IniFile;
		char szIniFile[MAX_PATH];
		sprintf(szIniFile, "%s\\UserData\\Layout\\UIData%d.ini", af_GetBaseDir(), GetHostPlayer()->GetCharacterID());
		IniFile.Open(szIniFile);
		IniFile.WriteIntValue("task_interface", "type_pate", GetCheckedRadioButton(1));
		IniFile.WriteIntValue("task_interface", "zhuxian_page", m_iCurPage);

		for(int i=0; i<TT_NUM; ++i)
		{
			AString strKey;
			strKey.Format("tasktype%d", i);
			IniFile.WriteIntValue("task_candeliver_tree", strKey, ms_bTaskCanDeliverTreeExpand[i]);
		}
		
		IniFile.Save(szIniFile);
		IniFile.Close();
	}
	return CDlgTaskBase::Release();
}

void CDlgTask::OnShowDialog()
{
	if ( stricmp(m_szName, "Win_Quest") == 0 )
	{
		m_idSelTask = -1;
		m_iFirstItem = 0;
		UpdateTask();
	}

	m_bMinimized = false;
}

void CDlgTask::UpdateTask()
{
	m_iFirstItem = m_pTv_Quest->GetFirstItem();
	if (GetCheckedRadioButton(1)==1)
	{
		UpdateTaskListCur();
	}
	else if (GetCheckedRadioButton(1)==2)
	{
		UpdateTaskListCanDeliver();
	}
	else if (GetCheckedRadioButton(1)==4)
	{
		UpdateTaskListZhuxian();
	}
	GetGameUIMan()->GetDialog("Win_Quest_Main")->Show(false);
}

void CDlgTask::OnCommand_focusall(const char * szCommand)
{
	m_vecTraceTaskID.clear();
	
	EC_GAME_CLIENTDATA gcd = GetGame()->GetConfigs()->GetGameClientData();
	gcd.nTaskTrace = m_pChk_FocusAll->IsChecked();
	GetGame()->GetConfigs()->SetGameClientData(gcd);
	if( !m_pChk_FocusAll->IsChecked() )
		return;
	
	int i, idTask;
	Task_State_info tsi;
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	
	for( i = 0; i < (int)pTask->GetTaskCount(); i++ )
	{
		idTask = pTask->GetTaskId(i);
		ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);

		// 若有子任务，则跳过(会执行子任务的追踪效果)
		int nPos = pTask->GetFirstSubTaskPosition(idTask);
		int idSubTask = 0;
		bool bHasSubTask = false;
		while( nPos > -1 )
		{
			idSubTask = pTask->GetNextSub(nPos);
			if(pTask->HasTask(idSubTask))
			{
				bHasSubTask = true; //idTask = idSubTask;
				break;
			}
		}
		if(bHasSubTask)
			continue;

		pTask->GetTaskStateInfo(idTask, &tsi);
		if( tsi.m_ulTimeLimit > 0 ||
			tsi.m_ulProtectTime > 0 ||
			tsi.m_MonsterWanted[0].m_ulMonstersToKill > 0 ||
			tsi.m_InterObjWanted[0].m_ulInterObjToCollect > 0 ||
			tsi.m_ItemsWanted[0].m_ulItemId > 0 ||
			(pTemp && pTemp->GetAwardNPC() > 0) ||
			(pTemp->GetTopTask() && pTemp->GetTopTask()->GetAwardNPC() > 0) ||
			(pTemp && pTemp->GetActionNPC() > 0) ||
			(pTemp->m_ulWaitTime > 0) ||
			(pTemp->m_iFactionGrassWanted > 0) ||
			(pTemp->m_iFactionMineWanted > 0) || 
			(pTemp->m_iFactionMonsterCoreWanted > 0) ||
			(pTemp->m_iFactionMonsterFoodWanted > 0) ||
			(pTemp->m_iFactionMoneyWanted > 0) ||
			(pTemp->m_bAutoMoveForReachFixedSite && pTemp->m_enumMethod == enumTMReachSite))
		{
			m_vecTraceTaskID.push_back(idTask);
		}
		
		FocusChildren(idTask);
	}
}

void CDlgTask::OnCommand_focus(const char * szCommand)
{
	PAUITREEVIEW pTree = m_pTv_Quest;
	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
	if( !pItem || pTree->GetParentItem(pItem) == pTree->GetRootItem()) return;

	Task_State_info tsi;
	int idTask = pTree->GetItemData(pItem);
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	
	LuaEvent::FirstTraceTask();
	
	// 若有子任务，则执行子任务的追踪效果
	int nPos = pTask->GetFirstSubTaskPosition(idTask);
	int idSubTask;
	while( nPos > -1 )
	{
		idSubTask = pTask->GetNextSub(nPos);
		if(pTask->HasTask(idSubTask))
		{
			idTask = idSubTask;	
			nPos = pTask->GetFirstSubTaskPosition(idSubTask);
		}
	}

	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);

	pTask->GetTaskStateInfo(idTask, &tsi);
	if( tsi.m_ulTimeLimit <= 0 &&
		tsi.m_ulProtectTime <= 0 &&
		tsi.m_MonsterWanted[0].m_ulMonstersToKill <= 0 &&
		tsi.m_InterObjWanted[0].m_ulInterObjCollected <= 0 &&
		tsi.m_ItemsWanted[0].m_ulItemId <= 0 &&
		(pTemp && pTemp->GetAwardNPC() == 0) &&
		(pTemp && pTemp->GetActionNPC() == 0) &&
		pTemp->m_ulWaitTime == 0 &&
		(pTemp->m_iFactionGrassWanted <= 0) &&
		(pTemp->m_iFactionMineWanted <= 0) &&
		(pTemp->m_iFactionMonsterCoreWanted <= 0) &&
		(pTemp->m_iFactionMonsterFoodWanted <= 0) &&
		(pTemp->m_iFactionMoneyWanted <= 0 &&
		(!pTemp->m_bAutoMoveForReachFixedSite || pTemp->m_enumMethod != enumTMReachSite))		
		)
	{
		return;
	}

	int i;
	bool bFound = false;

	for( i = 0; i < (int)m_vecTraceTaskID.size(); i++ )
	{
		if( idTask != m_vecTraceTaskID[i] ) continue;

		m_vecTraceTaskID.erase(m_vecTraceTaskID.begin() + i);
		bFound = true;
		break;
	}
	if( !bFound ) m_vecTraceTaskID.push_back(idTask);
}

void CDlgTask::OnCommand_abandon(const char * szCommand)
{
	PAUITREEVIEW pTree = m_pTv_Quest;
	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();

	if( pItem && pTree->GetParentItem(pItem) != pTree->GetRootItem() )
	{
		PAUIDIALOG pMsgBox;

		m_pAUIManager->MessageBox("Game_AbortTask", m_pAUIManager->GetStringFromTable(236),
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(pTree->GetItemData(pItem));
	}
}

void CDlgTask::OnCommand_TaskDaily(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_DailyPromp_Today");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgTask::OnCommand_ComboType(const char * szCommand)
{
	UpdateTaskListCanDeliver();
}

void CDlgTask::OnCommand_RdoType(const char * szCommand)
{
	int index = atoi(szCommand+3);
	if (1==index)
	{
		m_idSelTask = -1;
		m_iFirstItem = 0;
		UpdateTaskListCur();
	} 
	else if(2==index)
	{
		m_idSelTask = -1;
		m_iFirstItem = 0;
		UpdateTaskListCanDeliver();
		LuaEvent::FirstSearchLatestTask();
	}
	else if(3==index)
	{
		ChangeDialog(dynamic_cast<CDlgTaskBase *>(GetGameUIMan()->GetDialog("Win_QuestKf")));
	}
	else if(4==index)
	{
		ChangeDialog(dynamic_cast<CDlgTaskBase *>(GetGameUIMan()->GetDialog("Win_Quest_Main")));
		GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_11_2_TASK_ZHUXIAN);
	}
}

void CDlgTask::OnCommand_Return(const char * szCommand)
{
	ChangeDialog(dynamic_cast<CDlgTaskBase *>(GetGameUIMan()->GetDialog("Win_Quest_Main")));
}

void CDlgTask::OnCommand_Ancient(const char * szCommand)
{
	if(GetGameUIMan()->GetDialog("Win_Quest_Wanted")->IsShow())
		GetGameUIMan()->GetDialog("Win_Quest_Wanted")->Show(false);
	else 
		GetGameUIMan()->GetDialog("Win_Quest_Wanted")->Show(true);
}

void CDlgTask::OnCommand_Accept(const char * szCommand)
{
	P_AUITREEVIEW_ITEM pItem = m_pTv_Quest->GetSelectedItem();
	if (!pItem)
		return;
	
	GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_11_5_TASK_DELIVER);
	GetGame()->GetTaskTemplateMan()->ManualTrigTask(GetHostPlayer()->GetTaskInterface(), m_pTv_Quest->GetItemData(pItem));
}

void CDlgTask::OnCommand_Prev(const char* szCommand)
{
	if(!m_pBtn_Prev || !m_pBtn_Prev->IsShow())
		return;

	m_nAwardItemShowIndex--;
	if(m_nAwardItemShowIndex < 0)
		m_nAwardItemShowIndex = 0;
}

void CDlgTask::OnCommand_Next(const char* szCommand)
{
	if(!m_pBtn_Next || !m_pBtn_Next->IsShow())
		return;

	m_nAwardItemShowIndex++;
}

void CDlgTask::OnCommand_Prev2(const char* szCommand)
{
	if(!m_pBtn_Prev2 || !m_pBtn_Prev2->IsShow())
		return;

	m_nAwardItemShowIndex2--;
	if(m_nAwardItemShowIndex2 < 0)
		m_nAwardItemShowIndex2 = 0;
}

void CDlgTask::OnCommand_Next2(const char* szCommand)
{
	if(!m_pBtn_Next2 || !m_pBtn_Next2->IsShow())
		return;

	m_nAwardItemShowIndex2++;
}

void CDlgTask::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgTask::OnEventLButtonDown_Txt_QuestItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;

	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	GetItemLinkItemOn(x, y, pObj, &Item);

	if( Item.m_pItem != NULL )
	{
		if( Item.m_pItem->GetType() == enumEICoord )
		{
			A3DVECTOR3 vec;
			int idInstance;
			a_sscanf(Item.m_pItem->GetInfo(), _AL("%f%f%f%d"), 
				&vec.x, &vec.y, &vec.z, &idInstance);
//			if( idInstance != GetWorld()->GetInstanceID() )
//			{
//				PAUIDIALOG pMsgBox;
//				m_pAUIManager->MessageBox("", GetStringFromTable(762), MB_OK,
//					A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
//				pMsgBox->SetLife(3);
//			}
//			else
			{
				GetGameUIMan()->m_pDlgMiniMap->m_dwTempTargetTime = GetTickCount();
				GetGameUIMan()->m_pDlgMiniMap->m_vecTempTarget = vec;
				GetGameUIMan()->m_pDlgMiniMap->m_idTempTargetInstance = idInstance;

				g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, idInstance);
				g_AutoMoveDestName = Item.m_pItem->GetName();

				if( idInstance == GetWorld()->GetInstanceID() && GetHostPlayer()->GetModelTempArrow() )
					GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
			}
		}
		else if( Item.m_pItem->GetType() == enumEIImage )
		{
			const ACHAR* szInfo = Item.m_pItem->GetInfo();

			if(wcsncmp(szInfo, _AL("fly"), 3) == 0)
			{
				int idTask;
				int idNpc;
				a_sscanf(Item.m_pItem->GetInfo(), _AL("fly%d %d"), &idTask, &idNpc);
				GetGameUIMan()->CheckCanTaskFly(idTask,idNpc);
			}
			else if(wcsncmp(szInfo, _AL("radio"), 5) == 0)
			{
				// popup radio dialog
				CDlgBroadCastInGame* pDlgRadioInGame = (CDlgBroadCastInGame*)GetGameUIMan()->GetDialog("Win_Radio");
				pDlgRadioInGame->Show(true);
			}
			else if(wcsncmp(szInfo, _AL("reachsite"), 9) == 0)
			{
				int idTask;
				a_sscanf(Item.m_pItem->GetInfo(), _AL("reachsite%d"), &idTask);
				GetGameUIMan()->CheckTaskReachSiteFly(idTask);
			}
		}
	}
}

void CDlgTask::OnEventMouseMove_Txt_QuestItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	AUITEXTAREA_EDITBOX_ITEM Item;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	pObj->SetHint(_AL(""));
	GetItemLinkItemOn(x, y, pObj, &Item);
}

void CDlgTask::OnEventLButtonDown_TvTaskTrace(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X - m_pTv_TraceContent->GetPos().x;
	int y = GET_Y_LPARAM(lParam) - p->Y - m_pTv_TraceContent->GetPos().y;
	bool bClickIcon = false;
	P_AUITREEVIEW_ITEM pItem = m_pTv_TraceContent->HitTest(x, y, &bClickIcon);
	// Respown MouseClick Event When Not Click Task Text, or Not Click Icon in tree.
	//	if ((!pItem || m_pTv_TraceContent->GetParentItem(pItem) != m_pTv_TraceContent->GetRootItem())
	//		&& !bClickIcon)
	//		GetGameUIMan()->m_bClickedTaskTrace = true;
	//	else
	//		SetCapture(true);
	
	if (pItem)
	{
		if (m_pTv_TraceContent->GetParentItem(pItem) == m_pTv_TraceContent->GetRootItem())
		{
			m_idFocus = m_pTv_TraceContent->GetItemData(pItem);
//			GetGameUIMan()->GetDialog("Win_Message6")->Show(true);
		}
		else
		{
			int id = m_pTv_TraceContent->GetItemData(pItem);
			if (id>0)
			{
				A3DVECTOR3 vec;
				int idInstance;
				ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
				const NPC_INFO *info = pMan->GetTaskNPCInfo(id);
				if( info)
				{
					vec.x = (float)(info->x);
					vec.y = (float)(info->y);
					vec.z = (float)(info->z);
					idInstance = info->map_id;
					GetGameUIMan()->m_pDlgMiniMap->m_dwTempTargetTime = GetTickCount();
					GetGameUIMan()->m_pDlgMiniMap->m_vecTempTarget = vec;
					GetGameUIMan()->m_pDlgMiniMap->m_idTempTargetInstance = idInstance;
					

					int i;
					abase::vector<AUITEXTAREA_EDITBOX_ITEM> &vecItemLink = m_pTv_TraceContent->GetEditBoxItems();

					x = GET_X_LPARAM(lParam);
					y = GET_Y_LPARAM(lParam);
					for( i = 0; i < (int)vecItemLink.size(); i++ )
					{
						if( vecItemLink[i].rc.PtInRect(x, y) )
						{
							int idTask;
							int idNpc;
							if(a_strstr(vecItemLink[i].m_pItem->GetInfo(),_AL("fly")))
							{
								a_sscanf(vecItemLink[i].m_pItem->GetInfo(), _AL("fly%d %d"), &idTask, &idNpc);
								GetGameUIMan()->CheckCanTaskFly(idTask,idNpc);
							}
							else if(a_strstr(vecItemLink[i].m_pItem->GetInfo(),_AL("reachsite")))
							{
								a_sscanf(vecItemLink[i].m_pItem->GetInfo(), _AL("reachsite%d"), &idTask);
								GetGameUIMan()->CheckTaskReachSiteFly(idTask);
							}
							break;
						}						
					}

					if(i == (int)vecItemLink.size())	// auto move
					{
						{
							//寻径指引
							P_AUITREEVIEW_ITEM  pParentItem = m_pTv_TraceContent->GetParentItem(pItem);
							if(pParentItem)
							{
								int taskId = m_pTv_TraceContent->GetItemData(pParentItem);
								NewHelpGuide::TriggerClickTaskTraceNpcName(taskId);
							}
						}
						g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, idInstance);
						
						g_AutoMoveDestName = m_pTv_TraceContent->GetItemText(pItem);
						if (g_AutoMoveDestName[0] = _AL('^') && g_AutoMoveDestName.GetLength()>=7)
						{
							g_AutoMoveDestName.CutLeft(7);
						}

						elementdataman *pDataMan = GetGame()->GetElementDataMan();						
						DATA_TYPE dt;
						NPC_ESSENCE *pNPC = (NPC_ESSENCE *)pDataMan->get_data_ptr(
							m_pTv_TraceContent->GetItemData(pItem), ID_SPACE_ESSENCE, dt);
						if (dt==DT_NPC_ESSENCE)
						{
							g_AutoMoveDestName = pNPC->name;
						}
						
						if( idInstance == GetWorld()->GetInstanceID() && GetHostPlayer()->GetModelTempArrow() )
							GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
#ifdef AUTO_OPEN_SEARCHTASK
						P_AUITREEVIEW_ITEM pItemParent = m_pTv_TraceContent->GetParentItem(pItem);
						while(m_pTv_TraceContent->GetParentItem(pItemParent) != m_pTv_TraceContent->GetRootItem()) pItemParent = m_pTv_TraceContent->GetParentItem(pItemParent);
						GetGameUIMan()->CommonMessage("Win_NPC", "set_task", m_pTv_TraceContent->GetItemData(pItemParent), 0);
#endif 
					}
				}
			}
			else if(m_pTv_TraceContent->GetItemDataPtr(pItem, "ReachSite"))
			{
				int i;
				abase::vector<AUITEXTAREA_EDITBOX_ITEM> &vecItemLink = m_pTv_TraceContent->GetEditBoxItems();

				x = GET_X_LPARAM(lParam);
				y = GET_Y_LPARAM(lParam);
				for( i = 0; i < (int)vecItemLink.size(); i++ )
				{
					if( vecItemLink[i].rc.PtInRect(x, y) )
					{
						int idTask;
						if(a_strstr(vecItemLink[i].m_pItem->GetInfo(),_AL("reachsite")))
						{
							a_sscanf(vecItemLink[i].m_pItem->GetInfo(), _AL("reachsite%d"), &idTask);
							GetGameUIMan()->CheckTaskReachSiteFly(idTask);
						}
						else if(a_strstr(vecItemLink[i].m_pItem->GetInfo(),_AL("automove")))
						{
							A3DVECTOR3 vec;
							int idInstance;
							int x, y, z;
							a_sscanf(vecItemLink[i].m_pItem->GetInfo(), _AL("automove%d %d %d %d"), 
								&x, &y, &z, &idInstance);
							vec = A3DVECTOR3(x, y, z);

							GetGameUIMan()->m_pDlgMiniMap->m_dwTempTargetTime = GetTickCount();
							GetGameUIMan()->m_pDlgMiniMap->m_vecTempTarget = vec;
							GetGameUIMan()->m_pDlgMiniMap->m_idTempTargetInstance = idInstance;

							g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, idInstance);
							g_AutoMoveDestName = vecItemLink[i].m_pItem->GetName();

							if( idInstance == GetWorld()->GetInstanceID() && GetHostPlayer()->GetModelTempArrow() )
								GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
						}
						break;
					}						
				}
			}

		}
	}
	OnEventMouseMove_TvTaskTrace(wParam, lParam, pObj);
// 	else
// 		GetGameUIMan()->m_bClickedTaskTrace = true;
}

void CDlgTask::OnEventMouseMove_TvTaskTrace(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X - m_pTv_TraceContent->GetPos().x;
	int y = GET_Y_LPARAM(lParam) - p->Y - m_pTv_TraceContent->GetPos().y;
	bool bClickIcon = false;
	P_AUITREEVIEW_ITEM pItem = m_pTv_TraceContent->HitTest(x, y, &bClickIcon);
	
//	if (!pItem)
//		GetGameUIMan()->m_bClickedTaskTrace = true;

	GetGame()->ChangeCursor(RES_CUR_NORMAL);
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	if (pItem
		&& m_pTv_TraceContent->GetParentItem(pItem) != m_pTv_TraceContent->GetRootItem()
		&& (m_pTv_TraceContent->GetItemData(pItem) > 0 || m_pTv_TraceContent->GetItemDataPtr(pItem, "ReachSite")))
	{
		const NPC_INFO *info = pMan->GetTaskNPCInfo(m_pTv_TraceContent->GetItemData(pItem));
		if (info || m_pTv_TraceContent->GetItemDataPtr(pItem, "ReachSite"))
			GetGame()->ChangeCursor(RES_CUR_HAND);
	}
}


void CDlgTask::OnEventLButtonDown_Img_ZoomIn(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	GetGame()->ChangeCursor(RES_CUR_DRAG1);
	SetCaptureObject(pObj);
	m_nMouseLastX = GET_X_LPARAM(lParam);
	m_nMouseLastY = GET_Y_LPARAM(lParam);
	m_nOldSize = GetSize();
}

void CDlgTask::OnEventLButtonUp_Img_ZoomIn(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	GetGame()->ChangeCursor(RES_CUR_NORMAL);
	SetCaptureObject(NULL);
}

void CDlgTask::OnEventMouseMove_Img_ZoomIn(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	GetGame()->ChangeCursor(RES_CUR_DRAG1);
	if( GetCaptureObject() == pObj )
	{
		SIZE oldSize = GetSize();
		PAUIOBJECT pTv = GetDlgItem("Tv_List");
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_ZoomIn"); // Keep constant distance to right and bottom
		POINT p2 = pImg->GetPos(true);
		SIZE size = m_nOldSize;
		size.cx += GET_X_LPARAM(lParam) - m_nMouseLastX;
		size.cy += GET_Y_LPARAM(lParam) - m_nMouseLastY;
		SetSize(size.cx, size.cy);
		float fWindowScale = GetGameUIMan()->GetWindowScale();
		a_Clamp(m_nDefaultWidth, 229, 400);
		a_Clamp(m_nDefaultHeight, 150, 800);
		m_nWidth = int(m_nDefaultWidth * fWindowScale);
		m_nHeight = int(m_nDefaultHeight * fWindowScale);
		size = GetSize();
		pObj->SetPos(size.cx - pObj->GetSize().cx, size.cy - pObj->GetSize().cy);
		
		SIZE delta = GetSize();
		delta.cx -= oldSize.cx;
		delta.cy -= oldSize.cy;
		size = pTv->GetSize();
		pImg->SetPos(p2.x+delta.cx, p2.y+delta.cy);
		pTv->SetSize(size.cx+delta.cx, size.cy+delta.cy);
	}
}

int CDlgTask::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="showpage")
	{
		m_iCurPage = wParam;
		m_pTxt_Intro->SetText(GetUIData()->m_vecTaskZhuxian[m_iCurPage]->strIntro);
		m_pTxt_Intro->Show(true);
		AUIOBJECT_SETPROPERTY p;
		sprintf(p.fn, GetUIData()->m_vecTaskZhuxian[m_iCurPage]->strBackGround);
		m_pImg_IntroPic->SetProperty("Image File", &p);
		m_pImg_IntroPic->Show(true);
		CheckRadioButton(1, 4);
	}
	else if (strMsg=="loadInfo")
	{	
		AIniFile IniFile;
		char szIniFile[MAX_PATH];
		sprintf(szIniFile, "%s\\UserData\\Layout\\UIData%d.ini", af_GetBaseDir(), GetHostPlayer()->GetCharacterID());
		if (IniFile.Open(szIniFile))
		{
			int taskType = IniFile.GetValueAsInt("task_interface", "type_pate", 0);
			CheckRadioButton(1, taskType);
			if (GetHostPlayer()->GetSrcServerID()==0)
			{
				CheckRadioButton(1, 1);
			}
			if(taskType==4)
			{
				int curPage = IniFile.GetValueAsInt("task_interface", "zhuxian_page", 0);
				CommonMessage("showpage", curPage, 0);
			}
			for(int i=0; i<TT_NUM; ++i)
			{
				AString strKey;
				strKey.Format("tasktype%d", i);
				ms_bTaskCanDeliverTreeExpand[i] = IniFile.GetValueAsInt("task_candeliver_tree", strKey, 1)>0;
			}
		}
	}
	
	return true;
}

void CDlgTask::GetItemLinkItemOn(int x, int y,
	PAUIOBJECT pObj, AUITEXTAREA_EDITBOX_ITEM *pLink)
{
	POINT ptPos = pObj->GetPos();
	PAUITEXTAREA pText = (PAUITEXTAREA)pObj;

	g_pGame->ChangeCursor(RES_CUR_NORMAL);
	if( pText->GetHitArea(x - ptPos.x, y - ptPos.y) == AUITEXTAREA_RECT_FRAME )
	{
		int i;
		abase::vector<AUITEXTAREA_EDITBOX_ITEM> &vecItemLink = pText->GetEditBoxItems();

		for( i = 0; i < (int)vecItemLink.size(); i++ )
		{
			if( (vecItemLink[i].m_pItem->GetType() == enumEIIvtrlItem ||
				vecItemLink[i].m_pItem->GetType() == enumEICoord ||
				(vecItemLink[i].m_pItem->GetType() == enumEIImage&&
				(a_strstr(vecItemLink[i].m_pItem->GetInfo(),_AL("fly")) ||
				a_strstr(vecItemLink[i].m_pItem->GetInfo(),_AL("reachsite")) ||
				a_strstr(vecItemLink[i].m_pItem->GetInfo(),_AL("radio"))))
				) && vecItemLink[i].rc.PtInRect(x, y) )
			{
				g_pGame->ChangeCursor(RES_CUR_HAND);
				if (vecItemLink[i].m_pItem->GetType() == enumEIImage)
				{
					if(a_strstr(vecItemLink[i].m_pItem->GetInfo(), _AL("fly")) || a_strstr(vecItemLink[i].m_pItem->GetInfo(),_AL("reachsite")))
						pObj->SetHint(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(3302));
					else if(a_strstr(vecItemLink[i].m_pItem->GetInfo(), _AL("radio")))
						pObj->SetHint(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(10501));
				}
				*pLink = vecItemLink[i];
				break;
			}
		}
	}
}

bool CDlgTask::UpdateTaskDoing(int idTask)
{
	if( m_szName != "Win_Quest" ) return true;

	ACString strTemp;
	ACString strText;
	ACString strDesc;

	// 判断是否点击的是任务类别项
	if(idTask >= TASK_TYPE_DATA_BASE)
	{
		int nTaskDisplayType = idTask - TASK_TYPE_DATA_BASE;
		m_pTxt_Content->SetText(GetStringFromTable(13430+nTaskDisplayType-1));

		for(int i = 0; i < CDLGTASK_AWARDITEM_MAX ; i++)
		{
			m_pImg_Item[i]->Show(false);
			m_pImg_Item2[i]->Show(false);
		}

		m_pTxt_ItemAward->SetText(_AL(""));
		m_pTxt_BaseAward->SetText(_AL(""));

		GetDlgItem("Lab_Type1")->Show(false);
		GetDlgItem("Lab_Type2")->Show(false);

		return true;
	}

	m_pBtn_Prev->Show(false);
	m_pBtn_Next->Show(false);

	m_pBtn_Prev2->Show(false);
	m_pBtn_Next2->Show(false);

	unsigned long i;
	int idTopTask = idTask;	// 记录父任务id
	int idSubTask = 0;
	bool bCrsServer = GetHostPlayer()->GetSrcServerID()!=0;

	elementdataman *pDataMan = GetGame()->GetElementDataMan();
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);

	ATaskTempl* pTopTask = NULL;
	ATaskTempl* pSubTask = NULL;

	if( !pTemp )
		return false;

	int nFirstLines = m_pTxt_Content->GetFirstLine();
	m_pTxt_Content->SetText(_AL(""));
	

	pTopTask = (ATaskTempl*)pTemp->GetTopTask();
	if(!pTopTask)
		return false;

	idTopTask = pTopTask->m_ID;

	strDesc = pTopTask->GetDescription();

	strDesc += _AL("^FFFFFF");

	// 父任务限时
	strText = GetTaskItem(idTopTask);
	if(strText.GetLength() > 0)
	{
		strDesc += _AL("\r\r");
		strDesc += strText;
	}

	// 子任务描述
	if(idTask != idTopTask)
	{
		idSubTask = idTask;
	}
	else	// 找到第一个子任务，获取其描述
	{
		int nPos = pTask->GetFirstSubTaskPosition(idTopTask);
		int nSubTaskCount = 0;
		while(nPos > -1)
		{
			idSubTask = pTask->GetNextSub(nPos);
					
			pSubTask = pMan->GetTaskTemplByID(idSubTask);
			int nSiblingPos = 0;
			int idSiblingTask = 0;
			if(pSubTask)
			{
				nSiblingPos = nPos;
				idSiblingTask = pTask->GetNextSub(nSiblingPos);

				nPos = pTask->GetFirstSubTaskPosition(idSubTask);
			}			
		}

		// 若有子任务，查找子任务个数
		nPos = pTask->GetFirstSubTaskPosition(idTopTask);
		int idTemp;
		while( nPos > -1 )
		{
			idTemp = pTask->GetNextSub(nPos);
			if(pTask->HasTask(idTemp))
				nSubTaskCount++;		
		}

		if(nSubTaskCount > 1)	// 多于一个子任务，不显示子任务描述，故此处设置为0
			idSubTask = 0;
	}
	
	pSubTask = pMan->GetTaskTemplByID(idSubTask);

	if(pSubTask)	// 包含子任务
	{
		ACString strSubDesc = pSubTask->GetDescription();
		strDesc += _AL("\r^FFFFFF");
		strDesc += strSubDesc;
	}

	if(idTask != m_idLastTask)
	{
		m_nAwardItemShowIndex = 0;
		m_nAwardItemShowIndex2 = 0;
		m_idLastTask = idTask;
	}

	bool bCanGiveup = pMan->CanGiveUpTask(idTask);
	if (GetHostPlayer()->GetSrcServerID()!=0&&pTemp->HasFailAward()&&!pTemp->IsCrossServerTask())
	{
		bCanGiveup=false;
	}
	m_pBtn_Abandon->Enable(bCanGiveup);
	
	// 子任务限时、物品
	if(idSubTask != 0)
	{
		strTemp = GetTaskItem(idSubTask);
		if(strTemp.GetLength() > 0)
		{
			strText = _AL("\r");
			strText += strTemp;
			strText += _AL("\r");
		}
	}
	else
		strText = _AL("");
	
	int nNPC = 0;	// 为子任务完成npc
	if(pSubTask)
		nNPC = pSubTask->GetAwardNPC();
	else 
		nNPC = pTemp->GetAwardNPC();

	if(pTemp->m_DynTaskType == enumDTTSpecialAward && pTemp->m_ulSpecialAward != 0)
	{
		nNPC = 1188; //新手卡所有任务完成npc都是完美使者
	}

	bool bActionNPC = false;	// 默认显示“完成任务NPC”，否则显示“对目标使用动作”
	if(nNPC == 0)	// 查看是否为表情任务
	{
		int nAction = 0;
		if(pSubTask)
		{
			nNPC = pSubTask->GetActionNPC();
			nAction = pSubTask->GetActionID();
		}
		else
		{
			nNPC = pTemp->GetActionNPC();
			nAction = pTemp->GetActionID();
		}

		if(nNPC > 0)	// 显示“对目标使用动作：”
		{
			strText += _AL("^ffffff");
			strTemp.Format(GetStringFromTable(16150), GetStringFromTable(16150 + nAction));
			strText += strTemp;
			strText += _AL("\r");
		}
		bActionNPC = true;
	}

	if( nNPC != 0 )
	{
		DATA_TYPE dt;
		NPC_ESSENCE *pNPC = (NPC_ESSENCE *)pDataMan->get_data_ptr(
			nNPC, ID_SPACE_ESSENCE, dt);
		
		if( dt != DT_NPC_ESSENCE )
			a_LogOutput(1, "Invalid NPC id: %d", nNPC);
		else
		{
			int stringID = 684;
			if(bActionNPC)
				stringID = 16210;

			const NPC_INFO *info = pMan->GetTaskNPCInfo(nNPC);
			bool bCanFinishTask = pTask->CanFinishTask(idSubTask==0 ? idTopTask : idSubTask);
			if( info && info->x != 0 && info->z != 0)
			{
				ACHAR szPos[100];
				a_sprintf(szPos, _AL("%d %d %d %d"), info->x, info->y, info->z, info->map_id);

				if(bCanFinishTask)
				{
					EditBoxItemBase item(enumEICoord);
					item.SetName(pNPC->name);
					
					item.SetInfo(szPos);
					item.SetColor(A3DCOLORRGB(0, 255, 0));
					ACString strName = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
					CECInstance *pInstance = GetGameRun()->GetInstance(info->map_id);
					if( pInstance )
					{
						strTemp.Format(GetStringFromTable(stringID), pInstance->GetNameByCoord(info->x, info->z),
							strName, info->x, info->z);
					}
					else
					{
						strTemp.Format(GetStringFromTable(stringID), _AL(""),
							pNPC->name, info->x, info->z);
					}
								
					if( GetWorld()->GetInstanceID() == (int)info->map_id )
						m_AwardNPC.Set(info->x, 0, info->z);

					strText += strTemp;

					strText += _AL(" ");
					EditBoxItemBase itemImage(enumEIImage);
					itemImage.SetImageIndex(0);	// 飞天位于m_vecImageList中第一个，并且永不删除
					itemImage.SetImageFrame(0);
					a_sprintf(szPos, _AL("fly%d %d"), idSubTask==0 ? idTopTask : idSubTask, nNPC);
					itemImage.SetInfo(szPos);
					strText += (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();

				}
				else
				{
					CECInstance *pInstance = GetGameRun()->GetInstance(info->map_id);
					if( pInstance )
					{
						strTemp.Format(GetStringFromTable(stringID), pInstance->GetNameByCoord(info->x, info->z),
							pNPC->name, info->x, info->z);
					}
					else
					{
						strTemp.Format(GetStringFromTable(stringID), _AL(""),
							pNPC->name, info->x, info->z);
					}	
					
					strText += strTemp;
				}			
			}
			else
			{
				strTemp.Format(GetStringFromTable(GetCheckedRadioButton(1)==1 ? 6009 : 6010), pNPC->name);
				strText += strTemp;
			}
		}
	}
	
	if(strText.GetLength() > 0)
	{
		strDesc += _AL("\r\r^FFFFFF");
		strDesc += strText;
	}

	ACString strTaskDesc;
	strTaskDesc = FormatTaskText(strDesc);

 	m_pTxt_Content->SetText(strTaskDesc);

	for(int k = 0; k < nFirstLines; k++)
 		m_pTxt_Content->OnDlgItemMessage(WM_KEYDOWN, VK_DOWN, 0);

//	if( strText != m_pTxt_QuestItem->GetText() )
// 	{
// 		int nFirstLines = m_pTxt_QuestItem->GetFirstLine();
// 		m_pTxt_QuestItem->SetText(strText);
// 		if( idTask == (int)m_pTxt_QuestItem->GetData() )
// 		{
// 			for(int i = 0; i < nFirstLines; i++)
// 				m_pTxt_QuestItem->OnDlgItemMessage(WM_KEYDOWN, VK_DOWN, 0);
// 		}
// 	}
//	m_pTxt_QuestItem->SetData(idTask);
	
	int nRank = pTemp->GetRank();
	for( i = 0; ; i++ )
	{
		char szName[20];
		sprintf(szName, "Rank_%02d", i + 1);
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
		if( !pImage )
			break;
		pImage->Show(true);
		if( (int)i < nRank )
			pImage->FixFrame(0);
		else
			pImage->FixFrame(1);
	}
	
	Task_Award_Preview award;
	pTask->GetTaskAwardPreview(idTopTask, &award);	// 任务奖励物品，只显示父任务奖励
	strText = _AL("");
	if( award.m_ulGold > 0 )
	{
		ACHAR szGold[256];
		CECIvtrItem::GetPriceText(szGold, award.m_ulGold);
		strTemp.Format(GetStringFromTable(3201), szGold);
		strText += strTemp;
	}
	if( award.m_ulExp > 0 )
	{
		strTemp.Format(GetStringFromTable(3202), award.m_ulExp);
		strText += strTemp;
	}
	if( award.m_ulFengshenExp > 0 )
	{
		strTemp.Format(GetStringFromTable(3203), award.m_ulFengshenExp);
		strText += strTemp;
	}
	if( award.m_ulReputation > 0 )
	{
		strTemp.Format(GetStringFromTable(3205), award.m_ulReputation);
		strText += strTemp;
	}
	if( award.m_ulBonus > 0 )
	{
		strTemp.Format(GetStringFromTable(3250), GetCashText(award.m_ulBonus));
		strText += strTemp;
	}
	if( award.m_ulBattleScore > 0 )
	{
		strTemp.Format(GetStringFromTable(3251), award.m_ulBattleScore);
		strText += strTemp;
	}

	if(award.m_iFactionGrass > 0)
	{
		strTemp.Format(GetStringFromTable(21581), award.m_iFactionGrass);
		strText += strTemp;
	}

	if(award.m_iFactionMine > 0)
	{
		strTemp.Format(GetStringFromTable(21582), award.m_iFactionMine);
		strText += strTemp;
	}

	if(award.m_iFactionMonsterCore > 0)
	{
		strTemp.Format(GetStringFromTable(21583), award.m_iFactionMonsterCore);
		strText += strTemp;
	}

	if(award.m_iFactionMonsterFood > 0)
	{
		strTemp.Format(GetStringFromTable(21584), award.m_iFactionMonsterFood);
		strText += strTemp;
	}

	if(award.m_iFactionMoney > 0)
	{
		strTemp.Format(GetStringFromTable(21585), award.m_iFactionMoney);
		strText += strTemp;
	}

	if(award.m_iFactionGoldNote > 0)
	{
		strTemp.Format(GetStringFromTable(21586), award.m_iFactionGoldNote);
		strText += strTemp;
	}	

	m_pTxt_ItemAward->Show(false);
	m_pTxt_BaseAward->Show(false);
	for(i = 0; i < CDLGTASK_AWARDITEM_MAX ; i++)
	{
		m_pImg_Item[i]->Show(false);
		m_pImg_Item2[i]->Show(false);
	}
	GetDlgItem("Lab_Type1")->Show(false);
	GetDlgItem("Lab_Type2")->Show(false);
	for(i = 0; i < TASK_ZONE_FRIENDSHIP_COUNT; i++)
	{
		if( award.m_aFriendships[i] > 0 )
		{
			strTemp.Format(GetStringFromTable(3210 + i), award.m_aFriendships[i]);
			strText += strTemp;
		}
	}

		if( award.m_nFamilySkillProficiency )
		{
			strTemp.Format(GetStringFromTable(3431 + award.m_nFamilySkillIndex), award.m_nFamilySkillProficiency);
			strText += strTemp;
		}
		else if( award.m_nFamilySkillLev )
			strText += GetStringFromTable(3441 + award.m_nFamilySkillIndex);
		if( strText != _AL("") )
		{
			m_pTxt_BaseAward->SetText(strText);
			m_pTxt_BaseAward->Show(true);
		}
		if( award.m_bHasItem )
		{
// 			if( !award.m_bItemKnown )
// 			{
// 				if( m_pTxt_BaseAward->IsShow() )
// 				{
// 					m_pTxt_ItemAward->SetText(GetStringFromTable(3204));
// 					m_pTxt_ItemAward->Show(true);
// 				}
// 				else
// 				{
// 					m_pTxt_BaseAward->SetText(GetStringFromTable(3204));
// 					m_pTxt_BaseAward->Show(true);
// 				}
// 			}
// 			else
// 			{
				DWORD i;
				for(i = 0; i < CDLGTASK_AWARDITEM_MAX ; i++ )
				{
					int k = m_nAwardItemShowIndex + i;
					if( i < award.m_ulItemTypes )
					{
						CECIvtrItem *pItem = CECIvtrItem::CreateItem(award.m_ItemsId[k], 0, award.m_ItemsNum[k]);
						if( !pItem ) continue;
						
						AString strFile;
						af_GetFileTitle(pItem->GetIconFile(), strFile);
						strFile.MakeLower();
						m_pImg_Item[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
							GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
						AUICTranslate trans;
						pItem->GetDetailDataFromLocal();
						const ACHAR *pszDesc = pItem->GetDesc();
						if( pszDesc )
							m_pImg_Item[i]->SetHint(trans.Translate(pszDesc));
						else
							m_pImg_Item[i]->SetHint(pItem->GetName());
						delete pItem;
						strTemp.Format(_AL("%d"), award.m_ItemsNum[k]);
						m_pImg_Item[i]->SetText(strTemp);
						m_pImg_Item[i]->Show(true);
					}
				}

				if(award.m_ulItemTypes > CDLGTASK_AWARDITEM_MAX)
				{
					if(m_nAwardItemShowIndex != 0)
						m_pBtn_Prev->Show(true);
					else
						m_pBtn_Prev->Show(false);
					
					if((unsigned long)m_nAwardItemShowIndex + CDLGTASK_AWARDITEM_MAX  < award.m_ulItemTypes )
						m_pBtn_Next->Show(true);
					else
						m_pBtn_Next->Show(false);
				}
				else
				{
					m_pBtn_Prev->Show(false);
					m_pBtn_Next->Show(false);
				}

				if(award.m_ulItemTypes > 0)
					GetDlgItem("Lab_Type1")->Show(true);
				else
					GetDlgItem("Lab_Type1")->Show(false);
				


				for(i = 0; i < CDLGTASK_AWARDITEM_MAX ; i++ )
				{
					int k = m_nAwardItemShowIndex2 + i;
					if( i < award.m_ulItemTypes2 )
					{
						CECIvtrItem *pItem = CECIvtrItem::CreateItem(award.m_ItemsId2[k], 0, award.m_ItemsNum2[k]);
						if( !pItem ) continue;
						
						AString strFile;
						af_GetFileTitle(pItem->GetIconFile(), strFile);
						strFile.MakeLower();
						m_pImg_Item2[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
							GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
						AUICTranslate trans;
						pItem->GetDetailDataFromLocal();
						const ACHAR *pszDesc = pItem->GetDesc();
						if( pszDesc )
							m_pImg_Item2[i]->SetHint(trans.Translate(pszDesc));
						else
							m_pImg_Item2[i]->SetHint(pItem->GetName());
						delete pItem;
						strTemp.Format(_AL("%d"), award.m_ItemsNum2[k]);
						m_pImg_Item2[i]->SetText(strTemp);
						m_pImg_Item2[i]->Show(true);
					}
				}

				if(award.m_ulItemTypes2 > CDLGTASK_AWARDITEM_MAX)
				{
					if(m_nAwardItemShowIndex2 != 0)
						m_pBtn_Prev2->Show(true);
					else
						m_pBtn_Prev2->Show(false);
					
					if((unsigned long)m_nAwardItemShowIndex2 + CDLGTASK_AWARDITEM_MAX  < award.m_ulItemTypes2 )
						m_pBtn_Next2->Show(true);
					else
						m_pBtn_Next2->Show(false);
				}
				else
				{
					m_pBtn_Prev2->Show(false);
					m_pBtn_Next2->Show(false);
				}

				if(award.m_ulItemTypes2 > 0)
					GetDlgItem("Lab_Type2")->Show(true);
				else
					GetDlgItem("Lab_Type2")->Show(false);
//			}
		}


	m_pBtn_Focus->Enable(true);
	
	return true;
}

bool CDlgTask::UpdateTaskCanDeliver(int idTask)
{
	if( m_szName != "Win_Quest" ) return true;
	if( idTask<0 ) return false;
	
	if(idTask != m_idLastTask)
	{
		m_nAwardItemShowIndex = 0;
		m_nAwardItemShowIndex2 = 0;
	}

	m_idLastTask = idTask;
	elementdataman *pDataMan = GetGame()->GetElementDataMan();
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);
	if( !pTemp )
		return false;

	m_pBtn_Accept->Enable(pTemp->m_bManualTrig);

	ACString strDesc = pTemp->GetCanDeliverText();
	if ( strDesc.IsEmpty() )
		strDesc = pTemp->GetDescription();
	ACString strTaskDesc;
	strTaskDesc = FormatTaskText(strDesc);
	
//	m_pTxt_QuestItem->SetText(_AL(""));

	ACString strTemp;
	ACString strText;
	unsigned long i;
	bool bCrsServer = GetHostPlayer()->GetSrcServerID()!=0;

	int nNPC = pTemp->GetDeliverNPC();
	if( nNPC != 0 )
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
				strTemp.Format(GetStringFromTable(705), pInstance->GetNameByCoord(info->x, info->z),
					strName, info->x, info->z);
			}
			else
			{
				strTemp.Format(GetStringFromTable(705), _AL(""),
					pNPC->name, info->x, info->z);
			}
			
			strText += strTemp;
		}
		else
		{
			strTemp.Format(GetStringFromTable(GetCheckedRadioButton(1)==1 ? 6009 : 6010), pNPC->name);
			strText += strTemp;
		}
	}

	strTaskDesc += _AL("\r\r^FFFFFF");
	strTaskDesc += strText;
	m_pTxt_Content->SetText(strTaskDesc);

	int nRank = pTemp->GetRank();
	for( i = 0; ; i++ )
	{
		char szName[20];
		sprintf(szName, "Rank_%02d", i + 1);
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
		if( !pImage )
			break;
		pImage->Show(true);
		if( (int)i < nRank )
			pImage->FixFrame(0);
		else
			pImage->FixFrame(1);
	}
	for( int j = 0; j < CDLGTASK_AWARDITEM_MAX; j++ )
	{
		m_pImg_Item[j]->Show(false);
		m_pImg_Item2[j]->Show(false);
	}

	m_pTxt_ItemAward->SetText(_AL(""));
	m_pTxt_BaseAward->SetText(_AL(""));
	
	m_pBtn_Focus->Enable(false);
	m_pBtn_Abandon->Enable(false);


	// show award item
	Task_Award_Preview award;
	pTask->GetCanDeliverTaskAwardPreview(idTask, &award);	// 任务奖励物品，只显示父任务奖励
	strText = _AL("");
	if( award.m_ulGold > 0 )
	{
		ACHAR szGold[256];
		CECIvtrItem::GetPriceText(szGold, award.m_ulGold);
		strTemp.Format(GetStringFromTable(3201), szGold);
		strText += strTemp;
	}
	if( award.m_ulExp > 0 )
	{
		strTemp.Format(GetStringFromTable(3202), award.m_ulExp);
		strText += strTemp;
	}
	if( award.m_ulFengshenExp > 0 )
	{
		strTemp.Format(GetStringFromTable(3203), award.m_ulFengshenExp);
		strText += strTemp;
	}
	if( award.m_ulReputation > 0 )
	{
		strTemp.Format(GetStringFromTable(3205), award.m_ulReputation);
		strText += strTemp;
	}
	if( award.m_ulBonus > 0 )
	{
		strTemp.Format(GetStringFromTable(3232), GetCashText(award.m_ulBonus));
		strText += strTemp;
	}
	if( award.m_ulBattleScore > 0 )
	{
		strTemp.Format(GetStringFromTable(3233), award.m_ulBattleScore);
		strText += strTemp;
	}

	if(award.m_iFactionGrass > 0)
	{
		strTemp.Format(GetStringFromTable(21581), award.m_iFactionGrass);
		strText += strTemp;
	}

	if(award.m_iFactionMine > 0)
	{
		strTemp.Format(GetStringFromTable(21582), award.m_iFactionMine);
		strText += strTemp;
	}

	if(award.m_iFactionMonsterCore > 0)
	{
		strTemp.Format(GetStringFromTable(21583), award.m_iFactionMonsterCore);
		strText += strTemp;
	}

	if(award.m_iFactionMonsterFood > 0)
	{
		strTemp.Format(GetStringFromTable(21584), award.m_iFactionMonsterFood);
		strText += strTemp;
	}

	if(award.m_iFactionMoney > 0)
	{
		strTemp.Format(GetStringFromTable(21585), award.m_iFactionMoney);
		strText += strTemp;
	}

	if(award.m_iFactionGoldNote > 0)
	{
		strTemp.Format(GetStringFromTable(21586), award.m_iFactionGoldNote);
		strText += strTemp;
	}	

	m_pTxt_ItemAward->Show(false);
	m_pTxt_BaseAward->Show(false);
	for(i = 0; i < CDLGTASK_AWARDITEM_MAX ; i++)
	{
		m_pImg_Item[i]->Show(false);
		m_pImg_Item2[i]->Show(false);
	}
	GetDlgItem("Lab_Type1")->Show(false);
	GetDlgItem("Lab_Type2")->Show(false);
	for(i = 0; i < TASK_ZONE_FRIENDSHIP_COUNT; i++)
	{
		if( award.m_aFriendships[i] > 0 )
		{
			strTemp.Format(GetStringFromTable(3210 + i), award.m_aFriendships[i]);
			strText += strTemp;
		}
		if( award.m_nFamilySkillProficiency )
		{
			strTemp.Format(GetStringFromTable(3431 + award.m_nFamilySkillIndex), award.m_nFamilySkillProficiency);
			strText += strTemp;
		}
		else if( award.m_nFamilySkillLev )
			strText += GetStringFromTable(3441 + award.m_nFamilySkillIndex);
		if( strText != _AL("") )
		{
			m_pTxt_BaseAward->SetText(strText);
			m_pTxt_BaseAward->Show(true);
		}
		if( award.m_bHasItem )
		{
// 			if( !award.m_bItemKnown )
// 			{
// 				if( m_pTxt_BaseAward->IsShow() )
// 				{
// 					m_pTxt_ItemAward->SetText(GetStringFromTable(3204));
// 					m_pTxt_ItemAward->Show(true);
// 				}
// 				else
// 				{
// 					m_pTxt_BaseAward->SetText(GetStringFromTable(3204));
// 					m_pTxt_BaseAward->Show(true);
// 				}
// 			}
// 			else
// 			{
				DWORD i;
				for( i = 0; i < CDLGTASK_AWARDITEM_MAX ; i++ )
				{
					int k = m_nAwardItemShowIndex + i;
					if( i < award.m_ulItemTypes )
					{
						CECIvtrItem *pItem = CECIvtrItem::CreateItem(award.m_ItemsId[k], 0, award.m_ItemsNum[k]);
						if( !pItem ) continue;
						
						AString strFile;
						af_GetFileTitle(pItem->GetIconFile(), strFile);
						strFile.MakeLower();
						m_pImg_Item[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
							GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
						AUICTranslate trans;
						pItem->GetDetailDataFromLocal();
						const ACHAR *pszDesc = pItem->GetDesc();
						if( pszDesc )
							m_pImg_Item[i]->SetHint(trans.Translate(pszDesc));
						else
							m_pImg_Item[i]->SetHint(pItem->GetName());
						delete pItem;
						strTemp.Format(_AL("%d"), award.m_ItemsNum[k]);
						m_pImg_Item[i]->SetText(strTemp);
						m_pImg_Item[i]->Show(true);
					}
				}

				if(award.m_ulItemTypes > CDLGTASK_AWARDITEM_MAX)
				{
					if(m_nAwardItemShowIndex != 0)
						m_pBtn_Prev->Show(true);
					else
						m_pBtn_Prev->Show(false);
					
					if((unsigned long)m_nAwardItemShowIndex + CDLGTASK_AWARDITEM_MAX  < award.m_ulItemTypes )
						m_pBtn_Next->Show(true);
					else
						m_pBtn_Next->Show(false);
				}
				else
				{
					m_pBtn_Prev->Show(false);
					m_pBtn_Next->Show(false);
				}

				if(award.m_ulItemTypes > 0)
					GetDlgItem("Lab_Type1")->Show(true);
				else
					GetDlgItem("Lab_Type1")->Show(false);


				for( i = 0; i < CDLGTASK_AWARDITEM_MAX ; i++ )
				{
					int k = m_nAwardItemShowIndex2 + i;
					if( i < award.m_ulItemTypes2 )
					{
						CECIvtrItem *pItem = CECIvtrItem::CreateItem(award.m_ItemsId2[k], 0, award.m_ItemsNum2[k]);
						if( !pItem ) continue;
						
						AString strFile;
						af_GetFileTitle(pItem->GetIconFile(), strFile);
						strFile.MakeLower();
						m_pImg_Item2[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
							GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
						AUICTranslate trans;
						pItem->GetDetailDataFromLocal();
						const ACHAR *pszDesc = pItem->GetDesc();
						if( pszDesc )
							m_pImg_Item2[i]->SetHint(trans.Translate(pszDesc));
						else
							m_pImg_Item2[i]->SetHint(pItem->GetName());
						delete pItem;
						strTemp.Format(_AL("%d"), award.m_ItemsNum2[k]);
						m_pImg_Item2[i]->SetText(strTemp);
						m_pImg_Item2[i]->Show(true);
					}
				}

				if(award.m_ulItemTypes2 > CDLGTASK_AWARDITEM_MAX)
				{
					if(m_nAwardItemShowIndex2 != 0)
						m_pBtn_Prev2->Show(true);
					else
						m_pBtn_Prev2->Show(false);
					
					if((unsigned long)m_nAwardItemShowIndex2 + CDLGTASK_AWARDITEM_MAX  < award.m_ulItemTypes2 )
						m_pBtn_Next2->Show(true);
					else
						m_pBtn_Next2->Show(false);
				}
				else
				{
					m_pBtn_Prev2->Show(false);
					m_pBtn_Next2->Show(false);
				}

				if(award.m_ulItemTypes2 > 0)
					GetDlgItem("Lab_Type2")->Show(true);
				else
					GetDlgItem("Lab_Type2")->Show(false);
//			}
		}
	}

	return true;
}

bool CDlgTask::UpdateTaskFinish(int idTask)
{
	if( m_szName != "Win_Quest" ) return true;
	if( idTask<0 ) return false;
	if( idTask==m_idLastTask ) return true;

	m_idLastTask = idTask;
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);
	ACString strDesc;
	if ( pTemp )
	{
		strDesc = pTemp->GetTribute();
		if ( strDesc.IsEmpty() )
		{
			strDesc = pTemp->GetDescription();
		}
	}

	ACString strTaskDesc;
	strTaskDesc = FormatTaskText(strDesc);
	m_pTxt_Content->SetText(strTaskDesc);
//	m_pTxt_QuestItem->SetText(_AL(""));

	int nRank = pTemp->GetRank();
	for( int i = 0; ; i++ )
	{
		char szName[20];
		sprintf(szName, "Rank_%02d", i + 1);
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
		if( !pImage )
			break;
		pImage->Show(false);
	}
	for( int j = 0; j < CDLGTASK_AWARDITEM_MAX; j++ )
	{
		m_pImg_Item[j]->Show(false);
		m_pImg_Item2[j]->Show(false);
	}

	m_pTxt_ItemAward->SetText(_AL(""));
	m_pTxt_BaseAward->SetText(_AL(""));
	m_pBtn_Focus->Enable(false);
	m_pBtn_Abandon->Enable(false);
	return true;
}

bool CDlgTask::UpdateTaskToDo(int idTask)
{
	if( m_szName != "Win_Quest" ) return true;
	if( idTask<0 ) return false;
	if( idTask==m_idLastTask ) return true;
	
	m_idLastTask = idTask;
	ACString strDesc = GetGameUIMan()->GetStringFromTable(10502);
	ACString strTaskDesc;
	strTaskDesc = FormatTaskText(strDesc);
	m_pTxt_Content->SetText(strTaskDesc);
//	m_pTxt_QuestItem->SetText(_AL(""));
	
	for( int i = 0; ; i++ )
	{
		char szName[20];
		sprintf(szName, "Rank_%02d", i + 1);
		PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
		if( !pImage )
			break;
		pImage->Show(false);
	}
	for( int j = 0; j < CDLGTASK_AWARDITEM_MAX; j++ )
	{
		m_pImg_Item[j]->Show(false);
		m_pImg_Item2[j]->Show(false);
	}
	
	m_pTxt_ItemAward->SetText(_AL(""));
	m_pTxt_BaseAward->SetText(_AL(""));
	m_pBtn_Focus->Enable(false);
	m_pBtn_Abandon->Enable(false);
	return true;
}

void CDlgTask::UpdateTaskListCur()
{
	CheckRadioButton(1, 1);
	P_AUITREEVIEW_ITEM pItem = m_pTv_Quest->GetSelectedItem();
	if( pItem )
		m_idSelTask = m_pTv_Quest->GetItemData(pItem);
	
	m_idLastTask = -2;
	GetDlgItem("Btn_Return")->Show(false);
	GetDlgItem("Img_Mark")->Show(false);
	m_pTxt_Intro->Show(false);
	m_pImg_IntroPic->Show(false);
	m_pTxt_Content->Show(true);
//	m_pTxt_QuestItem->Show(true);
	m_pBtn_Abandon->Show(true);
	m_pChk_FocusAll->Show(true);
	m_pBtn_Accept->Show(false);
	m_pTxt_Content->SetText(_AL(""));
	m_pTxt_BaseAward->Show(false);
	m_pTxt_ItemAward->Show(false);
	m_pTv_Quest->DeleteAllItems();
	m_pBtn_Abandon->Enable(false);
	m_pBtn_Prev->Show(false);
	m_pBtn_Next->Show(false);
	for( int j = 0; j < CDLGTASK_AWARDITEM_MAX; j++ )
	{
		m_pImg_Item[j]->Show(false);
		m_pImg_Item2[j]->Show(false);
	}

	ACString strText;
	ATaskTempl *pTemp;
	unsigned long i, id;
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();	
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	bool bCrsServer = GetHostPlayer()->GetSrcServerID()!=0;
	
	UINT iSize = pTask->GetTaskCount();		// 已接任务
	for(int typeIndex=0; typeIndex<sizeof(s_TypeShowOrder)/sizeof(int); typeIndex++)
	{
		for( i = 0; i < iSize; i++ )
		{
			id = pTask->GetTaskId(i);
			pTemp = pMan->GetTaskTemplByID(id);
			if( !pTemp ) 
				continue;

			const ATaskTempl* pTopTask = pTemp->GetTopTask();
			DWORD nTaskDisplayType = pTopTask->GetDisplayType();
			if((int)nTaskDisplayType!=s_TypeShowOrder[typeIndex])	// 任务类型排序
				continue;

			// 插入类型名称
			pItem = m_pTv_Quest->GetFirstChildItem(m_pTv_Quest->GetRootItem());
			while( pItem )
			{
				if( m_pTv_Quest->GetItemData(pItem) == TASK_TYPE_DATA_BASE + nTaskDisplayType )
					break;
				pItem = m_pTv_Quest->GetNextSiblingItem(pItem);
			}
			if( !pItem )
			{
				pItem = m_pTv_Quest->InsertItem( GetStringFromTable(13410 + nTaskDisplayType - 1) );
				m_pTv_Quest->Expand(pItem, AUITREEVIEW_EXPAND_EXPAND);
				m_pTv_Quest->SetItemTextColor(pItem, TaskNewColor[nTaskDisplayType]);
				if ( bCrsServer && !pTemp->IsCrossServerTask() )
					m_pTv_Quest->SetItemTextColor(pItem, DisableColor);
				m_pTv_Quest->SetItemData(pItem, TASK_TYPE_DATA_BASE + nTaskDisplayType);
			}
			
			// 插入任务名字
			ACString strTaskTimes;
			if (pTemp->m_ulMaxFinishCount>0)
			{
				strTaskTimes.Format(_AL("(%d/%d)"), pTemp->GetFinishCount(pTask), pTemp->m_ulMaxFinishCount);
			}
			strTaskTimes = pTemp->GetName() + strTaskTimes;
			if ( bCrsServer && !pTemp->IsCrossServerTask() )
				FilterTextColor(strTaskTimes);
			pItem = m_pTv_Quest->InsertItem(strTaskTimes, pItem);
			m_pTv_Quest->SetItemTextColor(pItem, TaskNewColor[nTaskDisplayType]);
			if ( bCrsServer && !pTemp->IsCrossServerTask() )
				m_pTv_Quest->SetItemTextColor(pItem, DisableColor);
			m_pTv_Quest->SetItemHint(pItem, (const ACHAR*)pTemp->GetSignature());
			m_pTv_Quest->SetItemData(pItem, id);
			InsertTaskChildren(pItem, id, true, pTemp->IsKeyTask());
			
			// 选中m_idSelTask任务
			if( (int)id == m_idSelTask )
			{
				m_pTv_Quest->SelectItem(pItem);
				UpdateTaskDoing(id);
			}
		}
	}

	strText.Format(_AL("%d/50"), pTask->GetTaskCount());
	if( m_pTxt_QuestNO )
		m_pTxt_QuestNO->SetText(strText);
	m_pTv_Quest->SetFirstItem(m_iFirstItem);
}

void CDlgTask::UpdateTaskListCanDeliver()
{
	CheckRadioButton(1, 2);
	P_AUITREEVIEW_ITEM pItem = m_pTv_Quest->GetSelectedItem();
	if( pItem )
		m_idSelTask = m_pTv_Quest->GetItemData(pItem);
	
	m_idLastTask = -2;
	GetDlgItem("Btn_Return")->Show(false);
	GetDlgItem("Img_Mark")->Show(false);
	m_pTxt_Intro->Show(false);
	m_pImg_IntroPic->Show(false);
	m_pTxt_Content->Show(true);
//	m_pTxt_QuestItem->Show(true);
	m_pChk_FocusAll->Show(false);
	m_pBtn_Accept->Show(false);
	m_pTxt_Content->SetText(_AL(""));
	m_pTxt_BaseAward->Show(false);
	m_pTxt_ItemAward->Show(false);
	m_pTv_Quest->DeleteAllItems();
	m_pBtn_Abandon->Show(false);
	m_pBtn_Prev->Show(false);
	m_pBtn_Next->Show(false);
	m_pBtn_Prev2->Show(false);
	m_pBtn_Next2->Show(false);
	for( int j = 0; j < CDLGTASK_AWARDITEM_MAX; j++ )
	{
		m_pImg_Item[j]->Show(false);
		m_pImg_Item2[j]->Show(false);
	}

	bool bCrsServer = GetHostPlayer()->GetSrcServerID()!=0;
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();	
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	TaskTemplLst ttl;
	pMan->GetAvailableTasks(pTask, ttl);
	UINT iSize = ttl.size();

	ATaskTempl *pTemp;	// 推荐任务
	const int _recommend_types_bits[] = 
	{
 		RECOMMEND_TYPE_EXP,			// 经验 
 		RECOMMEND_TYPE_MONEY,		// 金钱 
 		RECOMMEND_TYPE_CULTIVATION,	// 造化 
 		RECOMMEND_TYPE_DEITY,		// 元神 
		RECOMMEND_TYPE_EQUIP,		// 装备 
		RECOMMEND_TYPE_SPECIAL,		// 特殊物品 
 		RECOMMEND_TYPE_TITLE,		// 称号 
	
	};
//		RECOMMEND_TYPE_ONTOP,		// 置顶 1

//依次显示：
// 经验奖励任务 0
// 金钱奖励任务 1
// 造化奖励任务 2
// 元神奖励任务 3
// 装备奖励任务 4
// 特殊物品奖励任务 5
// 称号任务 6
// 全部可接任务 7
	for(int typeIndex=0; typeIndex<8; typeIndex++)
	{
		SortTaskVector vecTasks;	// 任务排序表
		UINT i;
		for( i = 0; i < iSize; i++ )
		{
			int id = ttl[i]->GetID();
			pTemp = pMan->GetTaskTemplByID(id);
			if( !pTemp ) continue;
			DWORD nTaskRecommendType = pTemp->m_ulRecommendType;

			if(typeIndex != 7)	//若为7，则直接加入全部可接任务
			{
				if((nTaskRecommendType & (1<<_recommend_types_bits[typeIndex])) == 0)	// 非此类奖励任务
					continue;
			}

			// 设置任务的排序权重
			SORT_TASK* sortTask = new SORT_TASK;
			sortTask->nSortType = TASK_SORT_DEFAULT;
			sortTask->nSortValue = 0;
			sortTask->ulTemplID = pTemp->m_ID;

			if(_recommend_types_bits[typeIndex] == RECOMMEND_TYPE_EXP)	// 经验
			{
				DWORD nTaskRecommendType = pTemp->m_ulRecommendType;
				if((nTaskRecommendType & (1<<RECOMMEND_TYPE_ONTOP)) != 0)	// 是否标记了置顶
					sortTask->nSortType = TASK_SORT_ONTOP;
				else
				{
					if(pTemp->m_ulAwardType_S == enumTATRatio || 
						pTemp->m_ulAwardType_S == enumTATEach ||
						pTemp->m_ulAwardType_S == enumTATItemCount )
						sortTask->nSortType = TASK_SORT_RATIO;
					else
					{
						// 寻找是否有金丹物品
						unsigned long itemCnt = pTemp->GetAwardItemCnt(13425);
						if( itemCnt > 0)
						{
							sortTask->nSortType = TASK_SORT_ITEM;
							sortTask->nSortValue = itemCnt;
						}
						else
						{
							sortTask->nSortType = TASK_SORT_FIXED;
							sortTask->nSortValue = pTemp->m_Award_S->m_ulExp;
						}
					}						
				}
			}
			else if(_recommend_types_bits[typeIndex] == RECOMMEND_TYPE_MONEY)	// 金钱
			{
				DWORD nTaskRecommendType = pTemp->m_ulRecommendType;
				if((nTaskRecommendType & (1<<RECOMMEND_TYPE_ONTOP)) != 0)	// 是否标记了置顶
					sortTask->nSortType = TASK_SORT_ONTOP;
				else
				{
					if(pTemp->m_ulAwardType_S == enumTATRatio || 
						pTemp->m_ulAwardType_S == enumTATEach ||
						pTemp->m_ulAwardType_S == enumTATItemCount )
						sortTask->nSortType = TASK_SORT_RATIO;
					else
					{
						// 寻找是否有通宝物品
						unsigned long itemCnt = pTemp->GetAwardItemCnt(19074);
						if(itemCnt > 0)
						{
							sortTask->nSortType = TASK_SORT_ITEM;
							sortTask->nSortValue = itemCnt;
						}
						else
						{
							sortTask->nSortType = TASK_SORT_FIXED;
							sortTask->nSortValue = pTemp->m_Award_S->m_ulExp;
						}
					}						
				}
			}
			else if(_recommend_types_bits[typeIndex] == RECOMMEND_TYPE_CULTIVATION)	// 造化
			{
				sortTask->nSortValue = pTemp->GetAwardItemCnt(21202) + pTemp->GetAwardItemCnt(21345) + pTemp->GetAwardItemCnt(22343);
			}
			else if(_recommend_types_bits[typeIndex] == RECOMMEND_TYPE_DEITY)	// 元神
			{
				sortTask->nSortValue = pTemp->GetAwardItemCnt(41668) + pTemp->GetAwardItemCnt(41669);
			}

			vecTasks.push_back(sortTask);
		}

		// 排序
		BubbleSort(vecTasks.begin(), vecTasks.end(), CompareSortTaskShowOrder());

		for(i = 0; i < vecTasks.size(); i++ )
		{
			// 插入类型名称
			pItem = m_pTv_Quest->GetFirstChildItem(m_pTv_Quest->GetRootItem());
			while( pItem )
			{
				if( m_pTv_Quest->GetItemData(pItem) == (DWORD)typeIndex )
					break;
				pItem = m_pTv_Quest->GetNextSiblingItem(pItem);
			}
			if( !pItem )
			{
				pItem = m_pTv_Quest->InsertItem( GetStringFromTable(13420 + typeIndex) );
				if(ms_bTaskCanDeliverTreeExpand[typeIndex])
					m_pTv_Quest->Expand(pItem, AUITREEVIEW_EXPAND_EXPAND);

				// 推荐任务，类型名称均为白色
				m_pTv_Quest->SetItemTextColor(pItem, TaskNewColor[0]);
				if ( bCrsServer && !pTemp->IsCrossServerTask() )
					m_pTv_Quest->SetItemTextColor(pItem, DisableColor);
				m_pTv_Quest->SetItemData(pItem, typeIndex);
			}

			pTemp = pMan->GetTaskTemplByID(vecTasks[i]->ulTemplID);
			int id = pTemp->m_ID;
			// 插入任务名字
			ACString strTaskTimes;
			if (pTemp->m_ulMaxFinishCount>0)
			{
				strTaskTimes.Format(_AL("(%d/%d)"), pTemp->GetFinishCount(pTask), pTemp->m_ulMaxFinishCount);
			}
			strTaskTimes = pTemp->GetName() + strTaskTimes;

			// 加入“置顶”标记
			ACString strOnTop;
			EditBoxItemBase itemImage(enumEIImage);
			DWORD nTaskRecommendType = pTemp->m_ulRecommendType;
			if((nTaskRecommendType& (1<<RECOMMEND_TYPE_ONTOP)) != 0)	// 是否标记了置顶
			{
				itemImage.SetImageIndex(CECGameUIMan::ICONLIST_RECOMMEND);
				itemImage.SetImageFrame(0);
				strOnTop = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
				strTaskTimes = strOnTop + _AL(" ") + strTaskTimes;
			}
// 			else
// 				itemImage.SetImageIndex(CECGameUIMan::ICONLIST_RECOMMEND_BLANK);

// 			itemImage.SetImageFrame(0);
// 			strOnTop = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
// 			strTaskTimes = strOnTop + _AL(" ") + strTaskTimes;

			if ( bCrsServer && !pTemp->IsCrossServerTask() )
				FilterTextColor(strTaskTimes);
			pItem = m_pTv_Quest->InsertItem(strTaskTimes, pItem);
			int nTaskDisplayType = pTemp->GetDisplayType();
			m_pTv_Quest->SetItemTextColor(pItem, TaskNewColor[nTaskDisplayType]);
			if ( bCrsServer && !pTemp->IsCrossServerTask() )
				m_pTv_Quest->SetItemTextColor(pItem, DisableColor);
			m_pTv_Quest->SetItemHint(pItem, (const ACHAR*)pTemp->GetSignature());
			m_pTv_Quest->SetItemData(pItem, id);
			
			// 选中m_idSelTask任务
			if( (int)id == m_idSelTask )
			{
				m_pTv_Quest->SelectItem(pItem);
				UpdateTaskCanDeliver(id);
			}
		}

		// clear
		for(SortTaskVector::iterator iter = vecTasks.begin();iter!=vecTasks.end();++iter)
		{
			SORT_TASK* temp = *iter;
			delete temp;
			temp = NULL;
		}
		vecTasks.clear();
	}

	ACString strText;
	strText.Format(_AL("%d/50"), pTask->GetTaskCount());
	if( m_pTxt_QuestNO )
		m_pTxt_QuestNO->SetText(strText);
	m_pTv_Quest->SetFirstItem(m_iFirstItem);
}

void CDlgTask::UpdateTaskListZhuxian()
{
	CheckRadioButton(1, 4);	
	P_AUITREEVIEW_ITEM pItem = m_pTv_Quest->GetSelectedItem();
	m_idSelTask = -1;
	if( pItem )
		m_idSelTask = m_pTv_Quest->GetItemData(pItem);
	
	m_pChk_FocusAll->Show(false);
	m_pBtn_Accept->Show(true);
	m_pBtn_Accept->Enable(false);
	m_pTxt_Content->SetText(_AL(""));
	m_pTxt_BaseAward->Show(false);
	m_pTxt_ItemAward->Show(false);
	m_pTv_Quest->DeleteAllItems();
	m_pBtn_Abandon->Show(true);
	GetDlgItem("Btn_Return")->Show(true);
	for( int j = 0; j < CDLGTASK_AWARDITEM_MAX; j++ )
	{
		m_pImg_Item[j]->Show(false);
		m_pImg_Item2[j]->Show(false);
	}

	bool bCrsServer = GetHostPlayer()->GetSrcServerID()!=0;
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();	
	CECTaskInterface *pTaskInterafce = GetHostPlayer()->GetTaskInterface();
	P_AUITREEVIEW_ITEM pItemParent = m_pTv_Quest->InsertItem(GetUIData()->m_vecTaskZhuxian[m_iCurPage]->strName);
	m_pTv_Quest->SelectItem(pItemParent);
	m_pTv_Quest->Expand(pItemParent, AUITREEVIEW_EXPAND_EXPAND);
	m_pTv_Quest->SetItemTextColor(pItemParent, TaskColor[0]);
	abase::vector<int>& vecIds = GetUIData()->m_vecTaskZhuxian[m_iCurPage]->vecIds;

	FinishedTaskList* pTaskFinished = static_cast<FinishedTaskList*>(pTaskInterafce->GetFinishedTaskList());
	ActiveTaskList* pTaskActive = static_cast<ActiveTaskList*>(pTaskInterafce->GetActiveTaskList());

	bool allFinish = true;
	for (UINT i=0; i<vecIds.size(); ++i)
	{
		int idTask = vecIds[i];
		ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);
		if( !pTemp )
			return;
		
		if (pTaskFinished->SearchTask(idTask) == 0)	// 已经完成
		{
			pItem = m_pTv_Quest->InsertItem(pTemp->GetName(), pItemParent);
			m_pTv_Quest->SetItemTextColor(pItem, TaskStateColor[TS_FINISH]);
			m_pTv_Quest->SetItemHint(pItem, (const ACHAR*)pTemp->GetSignature());
			m_pTv_Quest->SetItemData(pItem, idTask);
			m_pTv_Quest->SetItemDataPtr(pItem, (void*)TS_FINISH, "TaskState");
		}
		else if (pTaskActive->GetEntry(idTask))	// 正在进行
		{
			ACString strTaskTimes;
			if (pTemp->m_ulMaxFinishCount>0)
			{
				strTaskTimes.Format(_AL("(%d/%d)"), pTemp->GetFinishCount(pTaskInterafce), pTemp->m_ulMaxFinishCount);
			}
			strTaskTimes = pTemp->GetName() + strTaskTimes;
			if ( bCrsServer && !pTemp->IsCrossServerTask() )
				FilterTextColor(strTaskTimes);
			pItem = m_pTv_Quest->InsertItem(strTaskTimes, pItemParent);
			m_pTv_Quest->SetItemTextColor(pItem, TaskStateColor[TS_DOING]);
			if ( bCrsServer && !pTemp->IsCrossServerTask() )
				m_pTv_Quest->SetItemTextColor(pItem, DisableColor);
			m_pTv_Quest->SetItemDataPtr(pItem, (void*)TS_DOING, "TaskState");
			m_pTv_Quest->SetItemHint(pItem, (const ACHAR*)pTemp->GetSignature());
			m_pTv_Quest->SetItemData(pItem, idTask);
			InsertTaskChildren(pItem, idTask, true, true);
			allFinish = false;
		}
		else if (i==0 || pTaskInterafce->CanDeliverTask(idTask)==0)		// 可接
		{
			pItem = m_pTv_Quest->InsertItem(pTemp->GetName(), pItemParent);
			m_pTv_Quest->SetItemTextColor(pItem, TaskStateColor[TS_CANDELIVER]);
			m_pTv_Quest->SetItemHint(pItem, (const ACHAR*)pTemp->GetSignature());
			m_pTv_Quest->SetItemData(pItem, idTask);
			m_pTv_Quest->SetItemDataPtr(pItem, (void*)TS_CANDELIVER, "TaskState");
			allFinish = false;
		}
		else	// 不可接
		{
			pItem = m_pTv_Quest->InsertItem(pTemp->GetName(), pItemParent);
			m_pTv_Quest->SetItemTextColor(pItem, TaskStateColor[TS_TODO]);	
			m_pTv_Quest->SetItemHint(pItem, (const ACHAR*)pTemp->GetSignature());
			m_pTv_Quest->SetItemData(pItem, idTask);
			m_pTv_Quest->SetItemDataPtr(pItem, (void*)TS_TODO, "TaskState");
			allFinish = false;
		}
		
		if( idTask == m_idSelTask )
		{
			m_pTv_Quest->SelectItem(pItem);
			m_idLastTask = -1;
		}
	}
	GetDlgItem("Img_Mark")->Show(allFinish);
	
	ACString strText;
	strText.Format(_AL("%d/50"), pTaskInterafce->GetTaskCount());
	if( m_pTxt_QuestNO )
		m_pTxt_QuestNO->SetText(strText);
}

void CDlgTask::FocusChildren(int idTask)
{
	int nPos;
	unsigned long id;
	Task_State_info tsi;
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	
	nPos = pTask->GetFirstSubTaskPosition(idTask);
	while( nPos > -1 )
	{
		id = pTask->GetNextSub(nPos);
		ATaskTempl *pTemp = pMan->GetTaskTemplByID(id);
		pTask->GetTaskStateInfo(id, &tsi);
		if( tsi.m_ulTimeLimit > 0 ||
			tsi.m_ulProtectTime > 0 ||
			tsi.m_MonsterWanted[0].m_ulMonsterId > 0 ||
			tsi.m_InterObjWanted[0].m_ulInterObjId > 0 ||
			tsi.m_ItemsWanted[0].m_ulItemId > 0 ||
			(pTemp && pTemp->GetAwardNPC() > 0) ||
			(pTemp && pTemp->GetActionNPC() > 0) ||
			(pTemp->m_iFactionGrassWanted > 0) ||
			(pTemp->m_iFactionMineWanted > 0) || 
			(pTemp->m_iFactionMonsterCoreWanted > 0) ||
			(pTemp->m_iFactionMonsterFoodWanted > 0) ||
			(pTemp->m_iFactionMoneyWanted > 0) ||
			(pTemp->m_bAutoMoveForReachFixedSite && pTemp->m_enumMethod == enumTMReachSite)
			)
		{
			UINT j;
			for (j=0; j<m_vecTraceTaskID.size(); j++)
			{
				if (m_vecTraceTaskID[j] == (int)id)
				{
					break;
				}
			}
			if (j == m_vecTraceTaskID.size())
			{
				m_vecTraceTaskID.push_back(id);
			}
		}
		
		FocusChildren(id);
	}
}

void CDlgTask::InsertTaskChildren(P_AUITREEVIEW_ITEM pRoot,
	unsigned long idTask, bool bExpand, bool bKey)
{
	int nPos;
	unsigned long id;
	ATaskTempl *pTemp;
	const ATaskTempl* pTopTask;
	P_AUITREEVIEW_ITEM pItem;
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();

	nPos = pTask->GetFirstSubTaskPosition(idTask);
	bool bCrsServer = GetHostPlayer()->GetSrcServerID()!=0;
	while( nPos > -1 )
	{
		id = pTask->GetNextSub(nPos);
		pTemp = pMan->GetTaskTemplByID(id);
		if( !pTemp ) continue;
		pTopTask = pTemp->GetTopTask();
		if(!pTopTask) continue;

		ACString strName = pTemp->GetName();
		if ( bCrsServer && !pTemp->IsCrossServerTask() )
			FilterTextColor(strName);
		pItem = m_pTv_Quest->InsertItem(strName, pRoot);
		if(GetCheckedRadioButton(1)==4)
			m_pTv_Quest->SetItemDataPtr(pItem, (void*)TS_DOING, "TaskState");
		DWORD nTaskDisplayType = pTopTask->GetDisplayType();
		if(GetCheckedRadioButton(1)==1)
			m_pTv_Quest->SetItemTextColor(pItem, TaskNewColor[nTaskDisplayType]);
		else
			m_pTv_Quest->SetItemTextColor(pItem, TaskStateColor[TS_DOING]);
		if ( bCrsServer && !pTemp->IsCrossServerTask() )
			m_pTv_Quest->SetItemTextColor(pItem, DisableColor);
		if( bExpand ) m_pTv_Quest->Expand(pItem, AUITREEVIEW_EXPAND_EXPAND);
		m_pTv_Quest->SetItemData(pItem, id);
		if( (int)id == m_idSelTask )
		{
			m_pTv_Quest->SelectItem(pItem);
			m_pBtn_Abandon->Enable(true);
			m_idLastTask = -1;
			UpdateTaskDoing(id);
		}
		InsertTaskChildren(pItem, id, bExpand, bKey);
	}
}

bool CDlgTask::Tick(void)
{
	if( m_szName == "Win_Quest" )
	{
		PAUITREEVIEW pTree = m_pTv_Quest;
		P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();

		if( pItem )
		{
			if(1==GetCheckedRadioButton(1)) //( pTree->GetParentItem(pItem) != pTree->GetRootItem() && (1==GetCheckedRadioButton(1)) )
			{
				UpdateTaskDoing(pTree->GetItemData(pItem));
				if (pTree->GetItemData(pItem)==23357)
					GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_3_5_TASK_LIST);

			}
			else if ( pTree->GetParentItem(pItem) != pTree->GetRootItem() && 2==GetCheckedRadioButton(1) )
			{
				UpdateTaskCanDeliver(pTree->GetItemData(pItem));
			}
			else if (4==GetCheckedRadioButton(1))
			{
				if (pTree->GetParentItem(pItem) == pTree->GetRootItem())
				{
					m_pTxt_Intro->Show(true);
					m_pImg_IntroPic->Show(true);
					m_pTxt_Content->Show(false);
//					m_pTxt_QuestItem->Show(false);
					UpdateTaskToDo(0);	// 不显示各种信息
					m_idLastTask = -1;
				}
				else
				{
					m_pTxt_Intro->Show(false);
					m_pImg_IntroPic->Show(false);
					m_pTxt_Content->Show(true);
//					m_pTxt_QuestItem->Show(true);
					int iState = (int)pTree->GetItemDataPtr(pItem, "TaskState");
					if (iState==TS_FINISH)
					{
						m_pBtn_Accept->Enable(false);
						UpdateTaskFinish(pTree->GetItemData(pItem));
					}
					else if (iState==TS_DOING)
					{
						m_pBtn_Accept->Enable(false);
						UpdateTaskDoing(pTree->GetItemData(pItem));
					}
					else if (iState==TS_CANDELIVER)
					{
						UpdateTaskCanDeliver(pTree->GetItemData(pItem));
					}
					else if (iState==TS_TODO)
					{
						m_pBtn_Accept->Enable(false);
						UpdateTaskToDo(pTree->GetItemData(pItem));
					}
				}
			}
			else
			{
				m_idLastTask = -2;
				m_pTxt_Content->SetText(_AL(""));
//				m_pTxt_QuestItem->SetText(_AL(""));
				m_pBtn_Abandon->Enable(false);
				m_pTxt_ItemAward->Show(false);
				m_pTxt_BaseAward->Show(false);
				for(int i = 0; i < CDLGTASK_AWARDITEM_MAX ; i++)
				{
					m_pImg_Item[i]->Show(false);
					m_pImg_Item2[i]->Show(false);
				}

				GetDlgItem("Lab_Type1")->Show(false);
				GetDlgItem("Lab_Type2")->Show(false);
			}
		}
		else
		{
			m_idLastTask = -2;
			m_pTxt_Content->SetText(_AL(""));
//			m_pTxt_QuestItem->SetText(_AL(""));
			m_pTxt_BaseAward->Show(false);
			m_pTxt_ItemAward->Show(false);
			m_pTxt_Intro->Show(false);
			m_pImg_IntroPic->Show(false);
			int i;
			for( i = 0; i < CDLGTASK_AWARDITEM_MAX; i++ )
			{
				m_pImg_Item[i]->Show(false);
				m_pImg_Item2[i]->Show(false);
			}
			GetDlgItem("Lab_Type1")->Show(false);
			GetDlgItem("Lab_Type2")->Show(false);
			for( i = 0; ; i++ )
			{
				char szName[20];
				sprintf(szName, "Rank_%02d", i + 1);
				PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
				if( !pImage )
					break;
				pImage->Show(false);
			}

			m_pBtn_Focus->Enable(false);
			m_pBtn_Abandon->Enable(false);
		}
		if(NULL != m_pRdo3)
		   m_pRdo3->Enable(GetHostPlayer()->GetSrcServerID()!=0);
		if(NULL != m_pAncient)
		   m_pAncient->Enable(g_pGame->GetGameRun()->GetWorld()->GetInstanceID() == 44);
	}

	return CDlgBase::Tick();
}

void CDlgTask::UpdateTaskMonster(int idTask)
{
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	Task_State_info tsi;
	int j;
	
	pTask->GetTaskStateInfo(idTask, &tsi);
	for (j = 0; j < MAX_MONSTER_WANTED; j++)
		if( tsi.m_MonsterWanted[j].m_ulMonsterId != 0 && 
			(tsi.m_MonsterWanted[j].m_ulMonstersToKill == 0 || tsi.m_MonsterWanted[j].m_ulMonstersToKill > tsi.m_MonsterWanted[j].m_ulMonstersKilled) )
			m_mapTaskMonster[tsi.m_MonsterWanted[j].m_ulMonsterId] = true;
	for (j = 0; j < MAX_ITEM_WANTED; j++)
		if( tsi.m_ItemsWanted[j].m_ulMonsterId != 0 && 
			(tsi.m_ItemsWanted[j].m_ulItemsToGet == 0 || tsi.m_ItemsWanted[j].m_ulItemsToGet > tsi.m_ItemsWanted[j].m_ulItemsGained) )
			m_mapTaskMonster[tsi.m_ItemsWanted[j].m_ulMonsterId] = true;

	int nPos = pTask->GetFirstSubTaskPosition(idTask);
	while( nPos > -1 )
	{
		int id = pTask->GetNextSub(nPos);
		UpdateTaskMonster(id);
	}
}

void CDlgTask::UpdateFocus()
{
	int i, idTask;
	Task_State_info tsi;
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	bool bFocusAll = ((PAUICHECKBOX)GetGameUIMan()->GetDialog("Win_Quest")->GetDlgItem("Chk_FocusAll"))->IsChecked();
	for( i = 0; i < (int)pTask->GetTaskCount(); i++ )
	{
		idTask = pTask->GetTaskId(i);
		ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);

		pTask->GetTaskStateInfo(idTask, &tsi);
		if( pTemp->m_bMustShown || bFocusAll )
		{
			if (tsi.m_ulTimeLimit > 0 ||
				tsi.m_ulProtectTime > 0 ||
				tsi.m_MonsterWanted[0].m_ulMonstersToKill > 0 ||
				tsi.m_InterObjWanted[0].m_ulInterObjToCollect > 0 ||
				tsi.m_ItemsWanted[0].m_ulItemId > 0 ||
				(pTemp && pTemp->GetAwardNPC() > 0) ||
				(pTemp && pTemp->GetActionNPC() > 0) ||			
				(pTemp && pTemp->m_iFactionGrassWanted > 0) ||
				(pTemp && pTemp->m_iFactionMineWanted > 0) || 
				(pTemp && pTemp->m_iFactionMonsterCoreWanted > 0) ||
				(pTemp && pTemp->m_iFactionMonsterFoodWanted > 0) ||
				(pTemp && pTemp->m_iFactionMoneyWanted > 0)	 ||
				(pTemp && pTemp->m_bAutoMoveForReachFixedSite && pTemp->m_enumMethod == enumTMReachSite)
				)
			{
				// 找到最底层的子任务
				int nPos = pTask->GetFirstSubTaskPosition(idTask);
				int idSubTask = 0;
				bool bHasSubTask = false;
				while( nPos > -1 )
				{
					idSubTask = pTask->GetNextSub(nPos);
					if(pTask->HasTask(idSubTask))
					{
						bHasSubTask = true; 
						nPos = pTask->GetFirstSubTaskPosition(idSubTask);
					}
				}

				UINT j;
				for (j=0; j<m_vecTraceTaskID.size(); j++)
				{
					if(bHasSubTask)
					{
						if(m_vecTraceTaskID[j] == idSubTask)
							break;
					}
					else if (m_vecTraceTaskID[j] == idTask)
						break;					
				}

				// 把子任务id加入trace列表
				if (j == m_vecTraceTaskID.size())
				{
					
					if(bHasSubTask)
						m_vecTraceTaskID.push_back(idSubTask);
					else
						m_vecTraceTaskID.push_back(idTask);
				}
			}
			FocusChildren(idTask);
		}
	}
}

void CDlgTask::RefreshTaskTrace()
{
	int i, j, idTask;
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	Task_State_info tsi;
	
	m_mapTaskMonster.clear();
	for( i = 0; i < (int)pTask->GetTaskCount(); i++ )
		UpdateTaskMonster(pTask->GetTaskId(i));

	UpdateFocus();

	if( m_vecTraceTaskID.size() <= 0 )
	{
		if( IsShow() ) 
		{
			Show(false);
		}

		if(m_bMinimized)
		{
			m_bMinimized = false;
			PAUIDIALOG  pMiniDlg = GetGameUIMan()->GetDialog("Win_QuestMinion_Min");
			if(NULL != pMiniDlg)
			{
			   pMiniDlg->Show(false);
			}
		}
		return;
	}

	ACString strText, strTemp, strHint;
	ATaskTempl *pTemp;
	DATA_TYPE DataType;
	int nHour, nMin, nSec;
	MONSTER_ESSENCE *pMonster;
	INTERACTION_OBJECT_ESSENCE* pInterObj;
	NPC_ESSENCE		*pNPC;
	elementdataman *pDataMan = GetGame()->GetElementDataMan();

	P_AUITREEVIEW_ITEM pRootItem = m_pTv_TraceContent->GetRootItem();

	// 1. Remove Tasks Not Found in IDVec from TreeView
	P_AUITREEVIEW_ITEM pMajorItem = m_pTv_TraceContent->GetFirstChildItem(pRootItem);
	while (pMajorItem != NULL)
	{
		int idTask = m_pTv_TraceContent->GetItemData(pMajorItem);
		for( i = 0; i < (int)m_vecTraceTaskID.size(); i++ )
			if (idTask == m_vecTraceTaskID[i])
				break;

		// Task Not Found, Remove it
		if (i == (int)m_vecTraceTaskID.size())
		{
			// 1. Remove SubStrings belong to this Task
			P_AUITREEVIEW_ITEM pItem = m_pTv_TraceContent->GetFirstChildItem(pMajorItem);
			while (pItem != NULL) {
				P_AUITREEVIEW_ITEM pItemNext = m_pTv_TraceContent->GetNextSiblingItem(pItem);
				m_pTv_TraceContent->DeleteItem(pItem);
				pItem = pItemNext;
			}
			// 2. Remove this Task
			P_AUITREEVIEW_ITEM pOldMajorItem = pMajorItem;
			pMajorItem = m_pTv_TraceContent->GetNextSiblingItem(pMajorItem);
			m_pTv_TraceContent->DeleteItem(pOldMajorItem);
		}
		else
			pMajorItem = m_pTv_TraceContent->GetNextSiblingItem(pMajorItem);
	}

	// 2. Insert IDVec to TreeView
	for( i = 0; i < (int)m_vecTraceTaskID.size(); i++ )
	{
		idTask = m_vecTraceTaskID[i];
		// Find This Task
		P_AUITREEVIEW_ITEM pMajorItem = m_pTv_TraceContent->GetFirstChildItem(pRootItem);
		while (pMajorItem != NULL && (int)m_pTv_TraceContent->GetItemData(pMajorItem) != idTask)
			pMajorItem = m_pTv_TraceContent->GetNextSiblingItem(pMajorItem);

		if( pTask->HasTask(idTask) )
		{
			pTemp = pMan->GetTaskTemplByID(idTask);
			if( !pTemp ) continue;
			const ATaskTempl* pTopTask = pTemp->GetTopTask();
			if( !pTopTask) continue;

			pTask->GetTaskStateInfo(pTopTask->m_ID, &tsi);

			// Task Title
			if (tsi.m_ulErrCode)
				strTemp.Format(_AL("^ff0000%s"), pTopTask->GetName());
			else
				strTemp.Format(_AL("^ffcb4a%s"), pTopTask->GetName());
			strText = strTemp;

			// 主任务限时信息
			// Task Time Limit
			if( tsi.m_ulTimeLimit > 0 )
			{
				strText += _AL("\r");

				nSec = tsi.m_ulTimeLimit - tsi.m_ulTimePassed;
				
				if( nSec < (int)tsi.m_ulTimeLimit / 10 )
					strText += _AL("^ff4a4a");
				else if( nSec < (int)tsi.m_ulTimeLimit / 3 )
					strText += _AL("^ffbe7d");
				else
					strText += _AL("^00e83a");

				nHour = nSec / 3600;
				nSec -= nHour * 3600;
				nMin = nSec / 60;
				nSec -= nMin * 60;
				strTemp.Format(GetStringFromTable(246), nHour, nMin, nSec);
				strText += strTemp;
			}

			pTask->GetTaskStateInfo(idTask, &tsi);
			if(pTemp != pTopTask)	// 有子任务,当前选中的是子任务
			{
				if (tsi.m_ulErrCode)
					strTemp.Format(_AL("^ff0000%s"), pTemp->GetName());
				else
					strTemp.Format(_AL("^ffcb4a%s"), pTemp->GetName());
				strText += _AL("\r");
				strText += strTemp;
			}

			// Get This Task Title
			bool bTaskNew = false;
			if (!pMajorItem || (int)m_pTv_TraceContent->GetItemData(pMajorItem) != idTask) {
				// Not Found. Insert One Task Title
				pMajorItem = m_pTv_TraceContent->InsertItem(strText, pRootItem);
				m_pTv_TraceContent->SetItemData(pMajorItem, idTask);
				m_pTv_TraceContent->SetItemDataPtr(pMajorItem, NULL);
				bTaskNew = true;
			}
			else {
				// Found. Update Task Title.
				m_pTv_TraceContent->SetItemText(pMajorItem, strText);
			}
			ACString strTemp = pTopTask->GetHintText();
			if (strTemp.IsEmpty())
			{
				strTemp = pTopTask->GetDescription();
			}

			if(pTopTask != pTemp)
			{
				strTemp += _AL("\r");
				ACString strTemp2 = pTemp->GetHintText();
				if (strTemp2.IsEmpty())
				{
					strTemp2 = pTemp->GetDescription();
				}

				strTemp += strTemp2;
			}


			strHint.Empty();

			while(strTemp.GetLength()>25)
			{
				//判断换行
				int nFind = strTemp.Find(_AL('\r'));
				if (nFind>0&&nFind<=25)
				{
					strHint += strTemp.Left(nFind+1);
					strTemp.CutLeft(nFind+1);
					continue;
				}
				nFind = strTemp.Find(_AL('\n'));
				if (nFind>0&&nFind<=25)
				{
					strHint += strTemp.Left(nFind+1);
					strTemp.CutLeft(nFind+1);
					continue;
				}
				//颜色码
				int nCurLen = 25;
				int nStart = 0;
				while( (nFind = strTemp.Find(_AL('^'), nStart)), (nFind>=0 && nFind<nCurLen))
				{
					nStart = nFind+1;
					nCurLen += 7;
					if (nCurLen>50)
						break;
				}
				
				strHint += strTemp.Left(nCurLen);
				strHint += _AL("\r");
				strTemp.CutLeft(nCurLen);
			}
			strHint += strTemp;
			while (strHint.Right(1) == _AL("\r") || strHint.Right(1) == _AL("\n"))
			{
				strHint.CutRight(1);
			}

			strHint = FormatTaskHint(strHint);
			m_pTv_TraceContent->SetItemHint(pMajorItem, strHint);

			// Task Items
			for( j = 0; j < MAX_ITEM_WANTED; j++ )
			{
				int idItem = tsi.m_ItemsWanted[j].m_ulItemId;
				if (idItem <= 0)
					break;

				CECIvtrItem* pItem = CECIvtrItem::CreateItem(idItem, 0, 1);

				if (pItem)
				{
					int idMonster = tsi.m_ItemsWanted[j].m_ulMonsterId;
					if( tsi.m_ItemsWanted[j].m_ulItemsToGet > 0 )
					{
						if (tsi.m_ItemsWanted[j].m_ulItemsGained == tsi.m_ItemsWanted[j].m_ulItemsToGet)
						{
							strTemp = _AL("");
//							continue;
// 							strTemp.Format(_AL("^00ff00%s(%d/%d)"), pItem->GetName(),
// 								tsi.m_ItemsWanted[j].m_ulItemsGained,
// 								tsi.m_ItemsWanted[j].m_ulItemsToGet);
						}
						else
							strTemp.Format(_AL("%s(%d/%d)"), pItem->GetName(),
								tsi.m_ItemsWanted[j].m_ulItemsGained,
								tsi.m_ItemsWanted[j].m_ulItemsToGet);
					}
					else
					{
						strTemp.Format(_AL("%s(%d)"), pItem->GetName(),
							tsi.m_ItemsWanted[j].m_ulItemsGained);
					}

					if( tsi.m_ItemsWanted[j].m_ulItemsGained < tsi.m_ItemsWanted[j].m_ulItemsToGet )
						strText = _AL("^ffffff");
					else
						strText = _AL("");
					strText += strTemp;
					

					// 飞天神符 - 物品
					if(strText.GetLength() != 0)
					{
						if(tsi.m_ItemsWanted[j].m_ulMonsterId != 0)
						{
							// 加入“飞天神符”标记 -- NPC
							EditBoxItemBase itemImage(enumEIImage);
							itemImage.SetImageIndex(CECGameUIMan::ICONLIST_TRANSFER);
							itemImage.SetImageFrame(0);
							ACHAR szPos[100];
							a_sprintf(szPos, _AL("fly%d %d"), idTask, tsi.m_ItemsWanted[j].m_ulMonsterId);
							itemImage.SetInfo(szPos);
							strText += (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
						}
					}

					P_AUITREEVIEW_ITEM pSubItem = m_pTv_TraceContent->GetFirstChildItem(pMajorItem);
					// Get this SubItem
					while (pSubItem != NULL && m_pTv_TraceContent->GetItemDataPtr(pSubItem))
					{
						int itemIdTemp = (int)m_pTv_TraceContent->GetItemDataPtr(pSubItem);
						if (itemIdTemp != idItem)
							pSubItem = m_pTv_TraceContent->GetNextSiblingItem(pSubItem);
						else
							break;
					}
					if (!pSubItem) 
					{
						// Not found. Insert One Item of this task
						if(strText.GetLength() == 0)
							continue;

						pSubItem = m_pTv_TraceContent->InsertItem(strText, pMajorItem);
						m_pTv_TraceContent->SetItemData(pSubItem, idMonster);
						m_pTv_TraceContent->SetItemDataPtr(pSubItem,(void*)idItem);
					}
					else
					{
						// Found. Update this Item Text
						if(strText.GetLength() == 0)
						{
							// delete this item
							P_AUITREEVIEW_ITEM pItemNext  = m_pTv_TraceContent->GetNextSiblingItem(pSubItem);

							int itemIdTemp = (int)m_pTv_TraceContent->GetItemDataPtr(pSubItem);
							if (itemIdTemp == idItem)
							{
								m_pTv_TraceContent->DeleteItem(pSubItem);
								pSubItem = pItemNext;
							}
						}
						else
							m_pTv_TraceContent->SetItemText(pSubItem, strText);
					}
					if(pSubItem)
					{
						strHint = FormatTaskHint(strHint);
						m_pTv_TraceContent->SetItemHint(pSubItem, strHint);
					}
					delete pItem;
				}
			}


			//Task InterObj
			for(j = 0; j < MAX_INTEROBJ_WANTED; j++)
			{
				int idInterObj = tsi.m_InterObjWanted[j].m_ulInterObjId;
				if(idInterObj == 0)
					break;

				if(tsi.m_InterObjWanted[j].m_ulInterObjCollected == tsi.m_InterObjWanted[j].m_ulInterObjToCollect)
				{
					P_AUITREEVIEW_ITEM pItem = m_pTv_TraceContent->GetFirstChildItem(pMajorItem);
					while(pItem != NULL)
					{
						P_AUITREEVIEW_ITEM pItemNext = m_pTv_TraceContent->GetNextSiblingItem(pItem);

						unsigned long idInterObj111 = (int)m_pTv_TraceContent->GetItemData(pItem);
						
						if((int)m_pTv_TraceContent->GetItemData(pItem) == tsi.m_InterObjWanted[j].m_ulInterObjId)
						{
							m_pTv_TraceContent->DeleteItem(pItem);
							pItem = pItemNext;
							break;
						}
						pItem = pItemNext;
					}
					continue;
				}

				DATA_TYPE DataType = g_pGame->GetElementDataMan()->get_data_type(idInterObj, ID_SPACE_ESSENCE);
				if(DataType == DT_INTERACTION_OBJECT_ESSENCE)
				{
					pInterObj = (INTERACTION_OBJECT_ESSENCE*)pDataMan->get_data_ptr(idInterObj, ID_SPACE_ESSENCE, DataType);
					if(pInterObj)
					{
						if(tsi.m_InterObjWanted[j].m_ulInterObjToCollect >= 0)
						{
							strTemp.Format(GetStringFromTable(21550), pInterObj->name,
								tsi.m_InterObjWanted[j].m_ulInterObjCollected, tsi.m_InterObjWanted[j].m_ulInterObjToCollect);
						}

						if(tsi.m_InterObjWanted[j].m_ulInterObjCollected < tsi.m_InterObjWanted[j].m_ulInterObjToCollect)
							strText = _AL("^ffffff");
						else
							strText = _AL("^00e83a");
						strText += strTemp;

						P_AUITREEVIEW_ITEM pSubItem = m_pTv_TraceContent->GetFirstChildItem(pMajorItem);
	
						while (pSubItem != NULL && (int)m_pTv_TraceContent->GetItemData(pSubItem) != idInterObj)
							pSubItem = m_pTv_TraceContent->GetNextSiblingItem(pSubItem);
						if (!pSubItem || (int)m_pTv_TraceContent->GetItemData(pSubItem) != idInterObj) 
						{
							pSubItem = m_pTv_TraceContent->InsertItem(strText, pMajorItem);
							m_pTv_TraceContent->SetItemData(pSubItem, idInterObj);
							m_pTv_TraceContent->SetItemDataPtr(pSubItem, NULL);
						}
						else 
						{
							// Found. Update this Item Text
							if(strText.GetLength() == 0)
							{
								// delete this item
								P_AUITREEVIEW_ITEM pItemNext  = m_pTv_TraceContent->GetNextSiblingItem(pSubItem);;
								m_pTv_TraceContent->DeleteItem(pSubItem);
								pSubItem = pItemNext;
							}
							else
								m_pTv_TraceContent->SetItemText(pSubItem, strText);
						}
						
						if(pSubItem)
						{
							strHint = FormatTaskHint(strHint);
							m_pTv_TraceContent->SetItemHint(pSubItem, strHint);
						}
					}
				}
			}

			// Task Monsters
			for (j = 0; j < MAX_MONSTER_WANTED; j++)
			{
				int idMonster = tsi.m_MonsterWanted[j].m_ulMonsterId;
				if (idMonster == 0)
					break;
				
				if( tsi.m_MonsterWanted[j].m_ulMonstersKilled == tsi.m_MonsterWanted[j].m_ulMonstersToKill )
				{
					// Remove monster info belong to this Task
					P_AUITREEVIEW_ITEM pItem = m_pTv_TraceContent->GetFirstChildItem(pMajorItem);
					while (pItem != NULL) 
					{
						P_AUITREEVIEW_ITEM pItemNext = m_pTv_TraceContent->GetNextSiblingItem(pItem);

						unsigned long idMonster111 = (int)m_pTv_TraceContent->GetItemData(pItem);
						
						if((int)m_pTv_TraceContent->GetItemData(pItem) == tsi.m_MonsterWanted[j].m_ulMonsterId)
						{
							// delete this item
							m_pTv_TraceContent->DeleteItem(pItem);
							pItem = pItemNext;

							break;
						}
						pItem = pItemNext;
					}
					continue;
				}

				DATA_TYPE DataType = g_pGame->GetElementDataMan()->get_data_type(idMonster, ID_SPACE_ESSENCE);
				if (DataType == DT_MONSTER_ESSENCE)
				{
					pMonster = (MONSTER_ESSENCE *)pDataMan->get_data_ptr(idMonster, ID_SPACE_ESSENCE, DataType);

					if(pMonster)
					{
						if( tsi.m_MonsterWanted[j].m_ulMonstersToKill > 0 )
						{
							strTemp.Format(GetStringFromTable(247), pMonster->name,
								tsi.m_MonsterWanted[j].m_ulMonstersKilled, tsi.m_MonsterWanted[j].m_ulMonstersToKill);
						}
						else
						{
							strTemp.Format(GetStringFromTable(256), pMonster->name, tsi.m_MonsterWanted[j].m_ulMonstersKilled);
						}
						
						const NPC_INFO *info = pMan->GetTaskNPCInfo(idMonster);
				
						if ( info && (info->x != 0 || info->z != 0)) 
						{
							// 加入“飞天神符”标记 -- 杀怪
							EditBoxItemBase itemImage(enumEIImage);
							itemImage.SetImageIndex(CECGameUIMan::ICONLIST_TRANSFER);
							itemImage.SetImageFrame(0);
							ACHAR szPos[100];
							a_sprintf(szPos, _AL("fly%d %d"), idTask, idMonster);
							itemImage.SetInfo(szPos);
							strTemp += (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
						}

						if( tsi.m_MonsterWanted[j].m_ulMonstersKilled < tsi.m_MonsterWanted[j].m_ulMonstersToKill )
							strText = _AL("^ffffff");
						else
							strText = _AL("^00e83a");
						strText += strTemp;
						
						P_AUITREEVIEW_ITEM pSubItem = m_pTv_TraceContent->GetFirstChildItem(pMajorItem);
						// Get this SubItem
						while (pSubItem != NULL && (int)m_pTv_TraceContent->GetItemData(pSubItem) != idMonster)
							pSubItem = m_pTv_TraceContent->GetNextSiblingItem(pSubItem);
						if (!pSubItem || (int)m_pTv_TraceContent->GetItemData(pSubItem) != idMonster) {
							// Not found. Insert One Item of this task
							pSubItem = m_pTv_TraceContent->InsertItem(strText, pMajorItem);
							m_pTv_TraceContent->SetItemData(pSubItem, idMonster);
							m_pTv_TraceContent->SetItemDataPtr(pSubItem, NULL);
						}
						else {
							// Found. Update this Item Text
							if(strText.GetLength() == 0)
							{
								// delete this item
								P_AUITREEVIEW_ITEM pItemNext  = m_pTv_TraceContent->GetNextSiblingItem(pSubItem);;
								m_pTv_TraceContent->DeleteItem(pSubItem);
								pSubItem = pItemNext;
							}
							else
								m_pTv_TraceContent->SetItemText(pSubItem, strText);
						}
						strHint = FormatTaskHint(strHint);
						m_pTv_TraceContent->SetItemHint(pSubItem, strHint);
					}
				}
				else if (DataType == DT_NPC_ESSENCE)
				{
					pNPC = (NPC_ESSENCE *)pDataMan->get_data_ptr(idMonster, ID_SPACE_ESSENCE, DataType);
					if(pNPC)
					{
						if( tsi.m_MonsterWanted[j].m_ulMonstersToKill > 0 )
						{
							strTemp.Format(GetStringFromTable(247), pNPC->name,
								tsi.m_MonsterWanted[j].m_ulMonstersKilled, tsi.m_MonsterWanted[j].m_ulMonstersToKill);
						}
						else
						{
							strTemp.Format(GetStringFromTable(256), pNPC->name, tsi.m_MonsterWanted[j].m_ulMonstersKilled);
						}
						
						if( tsi.m_MonsterWanted[j].m_ulMonstersKilled < tsi.m_MonsterWanted[j].m_ulMonstersToKill )
							strText = _AL("^ffffff");
						else
							strText = _AL("^00e83a");
						strText += strTemp;
						
						P_AUITREEVIEW_ITEM pSubItem = m_pTv_TraceContent->GetFirstChildItem(pMajorItem);
						// Get this SubItem
						while (pSubItem != NULL && (int)m_pTv_TraceContent->GetItemData(pSubItem) != idMonster)
							pSubItem = m_pTv_TraceContent->GetNextSiblingItem(pSubItem);
						if (!pSubItem || (int)m_pTv_TraceContent->GetItemData(pSubItem) != idMonster) {
							// Not found. Insert One Item of this task
							pSubItem = m_pTv_TraceContent->InsertItem(strText, pMajorItem);
							m_pTv_TraceContent->SetItemData(pSubItem, idMonster);
							m_pTv_TraceContent->SetItemDataPtr(pSubItem, NULL);
						}
						else {
							// Found. Update this Item Text
							m_pTv_TraceContent->SetItemText(pSubItem, strText);
						}
						strHint = FormatTaskHint(strHint);
						m_pTv_TraceContent->SetItemHint(pSubItem, strHint);
					}
				}
			}

			for(j = 0; j < 5; j++)
			{
				ACString strItemType = GetStringFromTable(21571 + j);
				
				if(tsi.m_iFactionPropWanted[j] > 0)
				{																															
					strText = _AL("^ffffff");
					strTemp.Format(GetStringFromTable(21551 + j), tsi.m_iFactionPropWanted[i]);
					strText += strTemp;

					P_AUITREEVIEW_ITEM pSubItem = m_pTv_TraceContent->GetFirstChildItem(pMajorItem);

					while(pSubItem != NULL && (a_strstr(m_pTv_TraceContent->GetItemText(pSubItem), strItemType) == 0))
						pSubItem = m_pTv_TraceContent->GetNextSiblingItem(pSubItem);

					if(!pSubItem)
					{
						// Not found. Insert One Item of this task
						pSubItem = m_pTv_TraceContent->InsertItem(strText, pMajorItem);	
						m_pTv_TraceContent->SetItemDataPtr(pSubItem,NULL);
					}
					else
					{
						m_pTv_TraceContent->SetItemText(pSubItem, strText);
					}

					if(pSubItem)
					{
						strHint = FormatTaskHint(strHint);
						m_pTv_TraceContent->SetItemHint(pSubItem, strHint);
					}
				}
			}

			int nNPC = pTemp->GetAwardNPC();
			if(pTemp->m_DynTaskType == enumDTTSpecialAward && pTemp->m_ulSpecialAward != 0)
			{
				nNPC = 1188; //新手卡所有任务完成npc都是完美使者
			}

			if(pTemp->GetActionNPC() > 0)
			{
				nNPC = pTemp->GetActionNPC();
			}

			if( nNPC != 0 )
			{
				DATA_TYPE dt;
				NPC_ESSENCE *pNPC = (NPC_ESSENCE *)pDataMan->get_data_ptr(
					nNPC, ID_SPACE_ESSENCE, dt);
				const NPC_INFO *info = pMan->GetTaskNPCInfo(nNPC);
				
				int idSubTask = pTemp->m_ID;
				int idTopTask = pTemp->GetTopTask()->m_ID;
				bool bCanFinishTask = pTask->CanFinishTask(idSubTask==0 ? idTopTask : idSubTask);

				strText.Empty();
				
				if(pTemp->GetActionNPC() > 0)	// 显示“对目标使用动作：”
				{
					int nAction = pTemp->GetActionID();
					strText = _AL("^ffffff");
					strTemp.Format(GetStringFromTable(16150), GetStringFromTable(16150 + nAction));
					strText += strTemp;
					strText += _AL("\r");
				}

				if ( info && info->x != 0 && info->z != 0) 
				{
					if(bCanFinishTask)
						strText += _AL("^00ff00");
					else
						strText += _AL("^ffffff");
				}

				strText += pNPC->name;

				if( info && (info->x != 0 || info->z != 0))
				{
					// 加入“飞天神符”标记 -- NPC
					EditBoxItemBase itemImage(enumEIImage);
					itemImage.SetImageIndex(CECGameUIMan::ICONLIST_TRANSFER);
					itemImage.SetImageFrame(0);
					ACHAR szPos[100];
					a_sprintf(szPos, _AL("fly%d %d"), idSubTask==0 ? idTopTask : idSubTask, nNPC);
					itemImage.SetInfo(szPos);
					strText += (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
				}

				P_AUITREEVIEW_ITEM pSubItem = m_pTv_TraceContent->GetFirstChildItem(pMajorItem);
				
				while(pSubItem != NULL && (a_strstr(m_pTv_TraceContent->GetItemText(pSubItem), pNPC->name) == 0))
					pSubItem = m_pTv_TraceContent->GetNextSiblingItem(pSubItem);

				if(!pSubItem)
				{
					// Not found. Insert One Item of this task
					pSubItem = m_pTv_TraceContent->InsertItem(strText, pMajorItem);	
					m_pTv_TraceContent->SetItemDataPtr(pSubItem,NULL);
				}
				else
				{
					m_pTv_TraceContent->SetItemText(pSubItem, strText);
				}		

				if(bCanFinishTask)
					m_pTv_TraceContent->SetItemData(pSubItem, nNPC);
				else
					m_pTv_TraceContent->SetItemData(pSubItem, 0);

				strHint = FormatTaskHint(strHint);
				m_pTv_TraceContent->SetItemHint(pSubItem, strHint);
			}

			if(pTemp && pTemp->m_bAutoMoveForReachFixedSite && pTemp->m_enumMethod == enumTMReachSite)
			{
				strText = _AL("^ffffff");	
				EditBoxItemBase item(enumEICoord);
				item.SetName((const ACHAR*)pTemp->m_szAutoMoveDestPosName);
				ACHAR szPos[100];
				int x, y, z;
				x = pTemp->m_AutoMoveDestPos.x;
				y = pTemp->m_AutoMoveDestPos.y;
				z = pTemp->m_AutoMoveDestPos.z;
				a_sprintf(szPos, _AL("automove%d %d %d %d"), x, y, 
				z, pTemp->m_ulReachSiteId);
				item.SetInfo(szPos);
				item.SetColor(A3DCOLORRGB(0, 255, 0));
				strTemp = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
				

				strTemp += _AL(" ");
				EditBoxItemBase itemimage(enumEIImage);
				itemimage.SetImageIndex(0);
				itemimage.SetImageFrame(0);
				ACHAR szPosImage[100];
				a_sprintf(szPosImage, _AL("reachsite%d"), idTask);
				itemimage.SetInfo(szPosImage);
				strTemp += (ACHAR)AUICOMMON_ITEM_CODE_START + itemimage.Serialize();
				strText += strTemp;
				
				int idReachSiteLimit = (TASK_REACHSITE_MASK | idTask) * (-1);
				P_AUITREEVIEW_ITEM pSubItem = m_pTv_TraceContent->GetFirstChildItem(pMajorItem);

				ACString PosName = (const ACHAR*)pTemp->m_szAutoMoveDestPosName;
				while(pSubItem != NULL && (int)m_pTv_TraceContent->GetItemData(pSubItem) != idReachSiteLimit)
				{
					pSubItem = m_pTv_TraceContent->GetNextSiblingItem(pSubItem);
				}
				
				if(!pSubItem || (int)m_pTv_TraceContent->GetItemData(pSubItem) != idReachSiteLimit)
				{
					// Not found. Insert One Item of this task
					pSubItem = m_pTv_TraceContent->InsertItem(strText, pMajorItem);	
					m_pTv_TraceContent->SetItemData(pSubItem, idReachSiteLimit);
					m_pTv_TraceContent->SetItemDataPtr(pSubItem, (void*)idTask, "ReachSite");
				}
				else
				{
					m_pTv_TraceContent->SetItemText(pSubItem, strText);
				}

				strHint = FormatTaskHint(strHint);
				m_pTv_TraceContent->SetItemHint(pSubItem, strHint);
			}

			// 子任务限时
			// Task Time Limit
			if( tsi.m_ulTimeLimit > 0 && idTask != (int)pTopTask->m_ID)
			{
				nSec = tsi.m_ulTimeLimit - tsi.m_ulTimePassed;
				
				if( nSec < (int)tsi.m_ulTimeLimit / 10 )
					strText = _AL("^ff4a4a");
				else if( nSec < (int)tsi.m_ulTimeLimit / 3 )
					strText = _AL("^ffbe7d");
				else
					strText = _AL("^00e83a");

				nHour = nSec / 3600;
				nSec -= nHour * 3600;
				nMin = nSec / 60;
				nSec -= nMin * 60;
				strTemp.Format(GetStringFromTable(246), nHour, nMin, nSec);
				strText += strTemp;

				int idTimeLimit = (TASK_TIME_LIMIT_MASK | idTask);
				P_AUITREEVIEW_ITEM pSubItem = m_pTv_TraceContent->GetFirstChildItem(pMajorItem);
				// Get this SubItem
				while (pSubItem != NULL && (int)m_pTv_TraceContent->GetItemData(pSubItem) != idTimeLimit)
					pSubItem = m_pTv_TraceContent->GetNextSiblingItem(pSubItem);
				if (!pSubItem || (int)m_pTv_TraceContent->GetItemData(pSubItem) != idTimeLimit) {
					// Not found. Insert One Item of this task
					pSubItem = m_pTv_TraceContent->InsertItem(strText, pMajorItem);
					m_pTv_TraceContent->SetItemData(pSubItem, idTimeLimit);
					m_pTv_TraceContent->SetItemDataPtr(pSubItem, NULL);
				}
				else {
					// Found. Update this Item Text
					m_pTv_TraceContent->SetItemText(pSubItem, strText);
				}
				strHint = FormatTaskHint(strHint);
				m_pTv_TraceContent->SetItemHint(pSubItem, strHint);
			}

			if( tsi.m_ulWaitTime > 0 && pTemp->m_bShowWaitTime)
			{
				nSec = tsi.m_ulWaitTime - tsi.m_ulTimePassed;
				
				strText = _AL("^00e83a");
				
				nHour = nSec / 3600;
				nSec -= nHour * 3600;
				nMin = nSec / 60;
				nSec -= nMin * 60;
				strTemp.Format(GetStringFromTable(13002), nHour, nMin, nSec);
				strText += strTemp;
				
				int idTimeLimit = (TASK_TIME_LIMIT_MASK | idTask);
				P_AUITREEVIEW_ITEM pSubItem = m_pTv_TraceContent->GetFirstChildItem(pMajorItem);
				// Get this SubItem
				while (pSubItem != NULL && (int)m_pTv_TraceContent->GetItemData(pSubItem) != idTimeLimit)
					pSubItem = m_pTv_TraceContent->GetNextSiblingItem(pSubItem);
				if (!pSubItem || (int)m_pTv_TraceContent->GetItemData(pSubItem) != idTimeLimit) {
					// Not found. Insert One Item of this task
					pSubItem = m_pTv_TraceContent->InsertItem(strText, pMajorItem);
					m_pTv_TraceContent->SetItemData(pSubItem, idTimeLimit);
					m_pTv_TraceContent->SetItemDataPtr(pSubItem, NULL);
				}
				else {
					// Found. Update this Item Text
					m_pTv_TraceContent->SetItemText(pSubItem, strText);
				}
				strHint = FormatTaskHint(strHint);
				m_pTv_TraceContent->SetItemHint(pSubItem, strHint);
			}

			// Task Protect NPC
			if( tsi.m_ulNPCToProtect > 0 )
			{
				strText.Empty();
				pMonster = (MONSTER_ESSENCE *)pDataMan->get_data_ptr(
					tsi.m_ulNPCToProtect, ID_SPACE_ESSENCE, DataType);
				if(pMonster && DataType == DT_MONSTER_ESSENCE )
				{
					strTemp.Format(GetStringFromTable(257), pMonster->name);
					strText += strTemp;
				}

				nSec = tsi.m_ulProtectTime;
				nHour = nSec / 3600;
				nSec -= nHour * 3600;
				nMin = nSec / 60;
				nSec -= nMin * 60;
				strTemp.Format(GetStringFromTable(258), nHour, nMin, nSec);
				strText += strTemp;
				
				nSec = max(0, tsi.m_ulProtectTime - tsi.m_ulTimePassed);
				if( nSec < (int)tsi.m_ulTimeLimit / 10 )
					strText += _AL("^ff4a4a");
				else if( nSec < (int)tsi.m_ulTimeLimit / 3 )
					strText += _AL("^ffbe7d");
				else
					strText += _AL("^00e83a");

				nHour = nSec / 3600;
				nSec -= nHour * 3600;
				nMin = nSec / 60;
				nSec -= nMin * 60;
				strTemp.Format(GetStringFromTable(259), nHour, nMin, nSec);
				strText += strTemp;

				int idProtectNPC = (TASK_PROTECTNPC_MASK | idTask);
				P_AUITREEVIEW_ITEM pSubItem = m_pTv_TraceContent->GetFirstChildItem(pMajorItem);
				// Get this SubItem
				while (pSubItem != NULL && (int)m_pTv_TraceContent->GetItemData(pSubItem) != idProtectNPC)
					pSubItem = m_pTv_TraceContent->GetNextSiblingItem(pSubItem);
				if (!pSubItem || (int)m_pTv_TraceContent->GetItemData(pSubItem) != idProtectNPC) {
					// Not found. Insert One Item of this task
					pSubItem = m_pTv_TraceContent->InsertItem(strText, pMajorItem);
					m_pTv_TraceContent->SetItemData(pSubItem, idProtectNPC);
					m_pTv_TraceContent->SetItemDataPtr(pSubItem, NULL);
				}
				else {
					// Found. Update this Item Text
					m_pTv_TraceContent->SetItemText(pSubItem, strText);
				}
				strHint = FormatTaskHint(strHint);
				m_pTv_TraceContent->SetItemHint(pSubItem, strHint);
			}

			if (bTaskNew)
				m_pTv_TraceContent->Expand(pMajorItem, AUITREEVIEW_EXPAND_EXPAND);
		}
		else
		{
			// Erase in TreeView
			if (!pMajorItem || (int)m_pTv_TraceContent->GetItemData(pMajorItem) != idTask) {
				// Not Found..
			}
			else {
				// 1. Remove SubStrings belong to this Task
				P_AUITREEVIEW_ITEM pItem = m_pTv_TraceContent->GetFirstChildItem(pMajorItem);
				pItem = m_pTv_TraceContent->GetNextSiblingItem(pItem);
				while (pItem != NULL) {
					P_AUITREEVIEW_ITEM pItemNext = m_pTv_TraceContent->GetNextSiblingItem(pItem);
					m_pTv_TraceContent->DeleteItem(pItem);
					pItem = pItemNext;
				}
				// 2. Remove this Task
				m_pTv_TraceContent->DeleteItem(pMajorItem);
			}

			// Erase in IDvec
			m_vecTraceTaskID.erase(m_vecTraceTaskID.begin() + i);
			i--;
		}
	}
// 	const int minHeightTv = 300;
// 	const int maxHeightTv = 600;
// 	int height = m_pTv_TraceContent->GetRequiredHeight();
// 	height = height>minHeightTv ? height : minHeightTv;
// 	height = height<maxHeightTv ? height : maxHeightTv;
// 	SetSize(GetSize().cx, 30+height);
// 	m_pTv_TraceContent->SetSize(m_pTv_TraceContent->GetSize().cx, height);
	A3DRECT rcWindow = GetGameUIMan()->GetRect();
	if (GetPos().y<0)
	{
		SetPosEx(GetPos().x, 0);
	}
	if (GetPos().y>(rcWindow.Height()-20))
	{
		SetPosEx(GetPos().x, (rcWindow.Height()-20));
	}
	if ((GetPos().x+GetSize().cx/2)<0)
	{
		SetPosEx(-GetSize().cx/2, GetPos().y);
	}

	if ((GetPos().x+GetSize().cx/2)>rcWindow.Width())
	{
		SetPosEx(rcWindow.Width()-GetSize().cx/2, GetPos().y);
	}

	if (GetDlgItem("Img_1")->IsShow())
	{
		for( i = 0; i < (int)m_vecTraceTaskID.size(); i++ )
		{
			if(m_vecTraceTaskID[i]==23477||m_vecTraceTaskID[i]==23531||m_vecTraceTaskID[i]==23547)
			{
				break;
			}
		}
	}
	

	if( !IsShow() && !m_bMinimized) Show(true, false, false);
}

void CDlgTask::OnEventLButtonDown_Tv_Quest(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( AUI_PRESS(VK_SHIFT) )
	{
		POINT ptPos = pObj->GetPos();
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
		int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
		PAUITREEVIEW pTree = (PAUITREEVIEW)pObj;
		P_AUITREEVIEW_ITEM pItem = pTree->HitTest(x, y);

		if( pItem ) OnCommand_focus("focus");
	}
	if (GetCheckedRadioButton(1)==2)
	{
		for(int i=0; i<TT_NUM; ++i)
			ms_bTaskCanDeliverTreeExpand[i] = true;
		PAUITREEVIEW pTree = (PAUITREEVIEW)pObj;
		P_AUITREEVIEW_ITEM pItem = pTree->GetFirstChildItem(pTree->GetRootItem());
		while (pItem)
		{
			int iType = pTree->GetItemData(pItem);
			if ((pTree->GetItemState(pItem)&AUITREEVIEW_IS_EXPANDED)==0)
				ms_bTaskCanDeliverTreeExpand[iType] = false;

			pItem = pTree->GetNextSiblingItem(pItem);
		}
	}

	if (GetCheckedRadioButton(1)==4)
	{
		GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_11_4_TASK_QINGYUN_FIRST_TASK);
	}
}

ACString CDlgTask::FormatTaskHint(const ACHAR* szText)
{
	int len = a_strlen(szText);
	ACString keyword;
	ACString result = _AL("");
	ACHAR text[3000];
	a_strcpy(text, szText);
	text[len++] = '&';
	int i = 0;
	int j = 0;
	int k;
	while( i < len )
	{
		k = i;
		while( i < len && text[i] != '&' )
			i++;
		if( i >= len ) break;
		text[i] = '\0';
		result += text + k;
		k = ++i;
		while( i < len && text[i] != '&' )
			i++;
		if( i >= len ) break;
		text[i] = '\0';
		keyword = text + k;
		if( keyword == _AL("") )
			result += _AL("^&");
		else if( keyword == _AL("name") )
			result += GetHostPlayer()->GetName();
		else if( keyword == _AL("level") )
		{
			ACString level;
			level.Format(_AL("%d"), GetHostPlayer()->GetBasicProps().iLevel);
			result += level;
		}
		else if( keyword == _AL("prof") )
			result += GetStringFromTable(PROFESSION_START + GetHostPlayer()->GetProfession());
		else if( keyword == _AL("org") )
			result += GetStringFromTable(2101 + GetHostPlayer()->GetProfession());
		else if( keyword == _AL("sex1") )
			result += GetStringFromTable(2251 + GetHostPlayer()->GetGender());
		else if( keyword == _AL("sex2") )
			result += GetStringFromTable(2253 + GetHostPlayer()->GetGender());
		else if( keyword == _AL("sex3") )
			result += GetStringFromTable(2255 + GetHostPlayer()->GetGender());
		else if( keyword == _AL("guild") )
		{
			if( GetHostPlayer()->GetFactionID() )
			{
				Faction_Info *pInfo = GetGame()->GetFactionMan()->GetFaction(GetHostPlayer()->GetFactionID());
				if( pInfo )
					result += pInfo->GetName();
			}
		}
		else if( keyword == _AL("family") )
		{
			if( GetHostPlayer()->GetFamilyID() )
			{
				Faction_Info *pInfo = GetGame()->GetFactionMan()->GetFaction(GetHostPlayer()->GetFamilyID(), false, true);
				if( pInfo )
					result += pInfo->GetName();
			}
			else
				result += GetGameUIMan()->GetStringFromTable(9128);
		}
		else if (keyword.Find(_AL("location"))>=0)//&location(周一仙,300,250,240,1)&
		{
			ACString strFileInfo = keyword;
			strFileInfo.CutLeft(9);
			strFileInfo.CutRight(1);
			CSplit s = strFileInfo;
			CSPLIT_STRING_VECTOR vec = s.Split(_AL(","));
			result += vec[0];
		}
		i++;
	}
	return result;
}

void CDlgTask::OnCommand_Minimize(const char* szCommand)
{
    if(m_szName == "Win_QuestMinion")
	{
	   m_bMinimized = true;
	   PAUIDIALOG  pMiniDlg = GetGameUIMan()->GetDialog("Win_QuestMinion_Min");
	   if(pMiniDlg)
	   { 
		   POINT pos = GetPos();
	       pMiniDlg->SetPosEx(pos.x,pos.y);
		   Show(false);
		   pMiniDlg->Show(true);
	   }
	}
	else if(m_szName == "Win_QuestMinion_Min")
	{
		PAUIDIALOG  pDlg = GetGameUIMan()->GetDialog("Win_QuestMinion");
		if(pDlg)
		{
			POINT pos = GetPos();
			pDlg->SetPosEx(pos.x,pos.y);
			Show(false);
			pDlg->Show(true);
		}
	}
}
