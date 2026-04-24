// Filename	: DlgTaskDaily.cpp
// Creator	: Xiao Zhou
// Date		: 2008/8/26

#include "AWScriptFile.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"
#include "DlgTaskDaily.h"
#include "EC_Game.h"
#include "EC_Global.h"

#include "time.h"

#include "DlgMiniMap.h"
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
#include "TaskTemplMan.h"
#include "elementdataman.h"
#include "EC_IvtrItem.h"
#include "EC_AutoMove.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgTaskDaily, CDlgBase)

AUI_ON_COMMAND("rdo*",		OnCommand_Rdo)
AUI_ON_COMMAND("time",		OnCommand_Btn_Time)
AUI_ON_COMMAND("taskname",	OnCommand_Btn_TaskName)
AUI_ON_COMMAND("level",		OnCommand_Btn_Level)
AUI_ON_COMMAND("type",		OnCommand_Btn_Type)
AUI_ON_COMMAND("number",	OnCommand_Btn_Number)
AUI_ON_COMMAND("suggest",	OnCommand_Btn_Suggest)
AUI_ON_COMMAND("chktj1",	OnCommand_Chk_tj1)
AUI_ON_COMMAND("chktj2",	OnCommand_Chk_tj2)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgTaskDaily, CDlgBase)

AUI_ON_EVENT("Lst_Task",	WM_LBUTTONDOWN,		OnEventLButtonDown_Lst_Task)
AUI_ON_EVENT("Txt_NPC",		WM_LBUTTONDOWN,		OnEventLButtonDown_Txt_NPC)
AUI_ON_EVENT("Txt_NPC",		WM_MOUSEMOVE,		OnEventMouseMove_Txt_NPC)

AUI_END_EVENT_MAP()


abase::vector<TaskDailyInfo>	CDlgTaskDaily::m_vecTaskDaily[7];
abase::vector<TaskDailyInfo>	CDlgTaskDaily::m_AllTaskDaily;
TaskDailyInfo		CDlgTaskDaily::m_CurTaskDaily;
DWORD	CDlgTaskDaily::m_dwCurDay = 1;

CDlgTaskDaily::CDlgTaskDaily()
{
}

CDlgTaskDaily::~CDlgTaskDaily()
{
	for(DWORD i=0; i<7; i++)
		m_vecTaskDaily[i].clear();
	m_AllTaskDaily.clear();
}

void CDlgTaskDaily::OnCommand_Rdo(const char * szCommand)
{
	m_dwCurDay = atoi(szCommand + strlen("rdo"));
	int index = 0;
	UINT idInVector = 0;	//保存当前列中任务在数组中的位置，不显示红/蓝任务时候用
	UINT i, j;
	m_iCurSel = 0;

	m_pLst_Task->ResetContent();

	ATaskTemplMan		*pMan = GetGame()->GetTaskTemplateMan();
	CECTaskInterface	*pTask = GetHostPlayer()->GetTaskInterface();
	struct tm			serverLocal = g_pGame->GetServerLocalTime();
	
	int		playerLevel = pTask->GetPlayerLevel();
	UINT		ulOccup = pTask->GetPlayerOccupation();
	ATaskTempl* lstTask;

	abase::vector<TaskDailyInfo>::iterator iterW = m_vecTaskDaily[m_dwCurDay-1].begin();
	for( ;iterW != m_vecTaskDaily[m_dwCurDay-1].end(); ++iterW)
	{
		iterW->taskType = TASKDAILY_TYPE_MIN;
		iterW->iCurTaskShow = 0;
		//选择合适的任务ID，保存到结构体成员iCurTaskShow，判断当前任务是否条件满足
		//if(iterW->uiNumID>1)
		if(0!=iterW->iNumMax)
		{
			BOOL bFindSuit = false;
			BOOL bOccupLimitTask = false;
			int	 minDiff = INT_MAX;
			UINT minDiffIndex = 0;
			UINT suitLevelMaxLimit = 0;

			lstTask = pMan->GetTaskTemplByID(iterW->uiTaskID[0]);
			//筛选合适职业
			for ( i = 0; i < iterW->uiNumID; i++ )
			{
				iterW->bSuit[i] = true;
				lstTask = pMan->GetTaskTemplByID(iterW->uiTaskID[i]);
				if (!lstTask)
				{
					continue;
				}
				if( 0==lstTask->m_ulOccupations )
				{
					bOccupLimitTask = true;
					continue;
				}
				for ( j = 0; j < lstTask->m_ulOccupations; j++ )
					if ( lstTask->m_Occupations[j] == ulOccup )
					{
						break;
					}
				if ( j == lstTask->m_ulOccupations )
				{
					iterW->bSuit[i] = false;
				}
			}
			if ( 0==ulOccup && !bOccupLimitTask)
			{
				iterW->taskType = TASKDAILY_TYPE_COND_LIMIT;
			}

			//筛选合适级别
			for ( i = 0; i < iterW->uiNumID; i++)
			{
				lstTask = pMan->GetTaskTemplByID(iterW->uiTaskID[i]);
				int levMin = iterW->ulMinLevel;//lstTask->m_ulPremise_Lev_Min;
				int levMax = iterW->ulMaxLevel;//lstTask->m_ulPremise_Lev_Max;
				levMax = levMax>0 ? levMax : 10000;
				
				if (iterW->bSuit[i] && playerLevel>=levMin && playerLevel<=levMax && suitLevelMaxLimit<levMax)
				{
					iterW->iCurTaskShow = i;
					bFindSuit = true;
					suitLevelMaxLimit = levMax;
					// 如果任务已经确定不满足条件，则显示满足条件的第一个任务
					// 否则显示所有满足等级条件最高等级的任务
					if(iterW->taskType!=TASKDAILY_TYPE_MIN)
						break;
					else
						continue;
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
			//挑选最接近任务等级任务ID
			if (!bFindSuit)
			{
				iterW->taskType = TASKDAILY_TYPE_COND_LIMIT;
				iterW->iCurTaskShow = minDiffIndex;
			}
		}
		else
		{
			if ( playerLevel<iterW->ulMinLevel || playerLevel>iterW->ulMaxLevel )
				iterW->taskType = TASKDAILY_TYPE_COND_LIMIT;
		}

		//判断任务类别
		lstTask = pMan->GetTaskTemplByID(iterW->uiTaskID[iterW->iCurTaskShow]);
		int curminute = serverLocal.tm_hour * 60 + serverLocal.tm_min;
		int minminute = iterW->tmStartTime.tm_hour * 60 + iterW->tmStartTime.tm_min;
		int maxminute = iterW->tmEncTime.tm_hour * 60 + iterW->tmEncTime.tm_min;
		if( 0==minminute && ((60*24)==maxminute) )
			iterW->bNoTimeLimit = true;
		else
			iterW->bNoTimeLimit = false;
		if (curminute>maxminute)
			iterW->taskType = TASKDAILY_TYPE_TIME_LIMIT_MISS;
		else if(TASKDAILY_TYPE_COND_LIMIT != iterW->taskType)
		{
			if( curminute<minminute )
				iterW->taskType = TASKDAILY_TYPE_TIME_LIMIT_WAIT;
			else 
				iterW->taskType = TASKDAILY_TYPE_CANDO;
		}

		//显示合适内容
		if( (TASKDAILY_TYPE_TIME_LIMIT_MISS != iterW->taskType && TASKDAILY_TYPE_COND_LIMIT	!= iterW->taskType)||
		(TASKDAILY_TYPE_TIME_LIMIT_MISS	== iterW->taskType && m_bShowBlue )	||
		(TASKDAILY_TYPE_COND_LIMIT		== iterW->taskType && m_bShowRed  )	 )
		{
			ACString strText;
			if(iterW->bNoCountLimit)
			{
				if(iterW->bNoTimeLimit)
				{
					strText.Format(_AL("%s\t%s\t%s\t%s\t%s\t%s\t"), _AL(" "), GetStringFromTable(1060),
						iterW->strName, iterW->strLevelInfo, iterW->strType, GetStringFromTable(1061));
				}
				else
				{
					if( TASKDAILY_TYPE_CANDO == iterW->taskType )
						strText.Format(_AL("%s\t%s\t%s\t%s\t%s\t%s\t"), _AL("◆"), iterW->strTime,
						iterW->strName, iterW->strLevelInfo, iterW->strType, GetStringFromTable(1061));
					else
						strText.Format(_AL("%s\t%s\t%s\t%s\t%s\t%s\t"), _AL(" "), iterW->strTime, 
						iterW->strName, iterW->strLevelInfo, iterW->strType, GetStringFromTable(1061));
				}
			}
			else
			{
				if(iterW->bNoTimeLimit)
				{
					strText.Format(_AL("%s\t%s\t%s\t%s\t%s\t%d\t"), _AL(" "),  GetStringFromTable(1060),
						iterW->strName, iterW->strLevelInfo, iterW->strType, iterW->iNumMax);
				}
				else
				{
					if( TASKDAILY_TYPE_CANDO == iterW->taskType )
						strText.Format(_AL("%s\t%s\t%s\t%s\t%s\t%d\t"), _AL("◆"), iterW->strTime, iterW->strName, 
						iterW->strLevelInfo, iterW->strType, iterW->iNumMax);
					else
						strText.Format(_AL("%s\t%s\t%s\t%s\t%s\t%d\t"), _AL(" "), iterW->strTime, iterW->strName, 
						iterW->strLevelInfo, iterW->strType, iterW->iNumMax);
				}
			}

			for(DWORD i=0;i<iterW->uiCommend;i++)
				strText+=_AL("★");
			m_pLst_Task->AddString(strText);
			m_pLst_Task->SetItemData(index, iterW->tmStartTime.tm_hour*60+iterW->tmStartTime.tm_min ,1,"");
			m_pLst_Task->SetItemData(index,iterW->ulMinLevel,3,"");
			m_pLst_Task->SetItemData(index,idInVector,4,"");
			m_pLst_Task->SetItemData(index,0==iterW->iNumMax ? INT_MAX:iterW->iNumMax,5,"");
			m_pLst_Task->SetItemData(index,iterW->uiCommend,6,"");
			if( TASKDAILY_TYPE_COND_LIMIT == iterW->taskType )
			{
				m_pLst_Task->SetItemTextColor(index,A3DCOLORRGB(255, 0, 0));
			}
			else if( TASKDAILY_TYPE_TIME_LIMIT_MISS == iterW->taskType )
			{
				m_pLst_Task->SetItemTextColor(index,A3DCOLORRGB(0, 0, 255));
			}
			else if( iterW->bSpecial )
			{
				m_pLst_Task->SetItemTextColor(index,A3DCOLORRGB(255, 220, 138));
			}
			index++;
		}
		idInVector++;
	}

	OnEventLButtonDown_Lst_Task(0,0,0);
}

void CDlgTaskDaily::OnEventLButtonDown_Lst_Task(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();

	int nSel = m_pLst_Task->GetCurSel();
	if( nSel >= 0 && nSel < m_pLst_Task->GetCount() )
	{
		m_iCurSel = nSel;
		unsigned long curIndex = m_pLst_Task->GetItemData(nSel,4);
		for (UINT index=0; index<m_vecTaskDaily[m_dwCurDay-1].size(); index++)
		{
			if ( index == curIndex )
			{
				m_CurTaskDaily = m_vecTaskDaily[m_dwCurDay-1][index];
				break;
			}
		}
		
		DATA_TYPE dt;
		ACString strMsg;

		elementdataman *pDataMan = GetGame()->GetElementDataMan();
		const NPC_INFO *info;
		NPC_ESSENCE *pNPC;

		if(m_CurTaskDaily.bNoCountLimit)
		{
			pNPC = (NPC_ESSENCE *)pDataMan->get_data_ptr(
				m_CurTaskDaily.uiTaskID[m_CurTaskDaily.iCurTaskShow], ID_SPACE_ESSENCE, dt);
			info = pMan->GetTaskNPCInfo(m_CurTaskDaily.uiTaskID[m_CurTaskDaily.iCurTaskShow]);
		}
		else
		{
			ATaskTempl	*pTaskTempl = pMan->GetTaskTemplByID(m_CurTaskDaily.uiTaskID[m_CurTaskDaily.iCurTaskShow]);
			if (!pTaskTempl)
			{
				return;
			}
			pNPC = (NPC_ESSENCE *)pDataMan->get_data_ptr(
				pTaskTempl->GetDeliverNPC(), ID_SPACE_ESSENCE, dt);
			info = pMan->GetTaskNPCInfo(pTaskTempl->GetDeliverNPC());
		}
		
		if( info && info->x != 0 && info->z != 0 )
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
				strMsg.Format(_AL("%s %s(%d,%d)"), pInstance->GetNameByCoord(info->x, info->z),
					strName, info->x, info->z);
				wchar_t test[100];
				wcscpy(test,strName);
			}
			else
			{
				strMsg.Format(_AL("%s(%d,%d)"), _AL(""),
					pNPC->name, info->x, info->z);
			}
		}
		else
			strMsg.Format(_AL("%s"), pNPC->name);
		m_pTxt_TaskName->SetText(m_CurTaskDaily.strName);
		m_pTxt_NPC->SetText(strMsg);
		m_pTxt_Line->SetText( m_CurTaskDaily.strLineInfo);
		AUICTranslate trans;
		m_pTxt_Desp->SetText(trans.Translate(m_CurTaskDaily.strDescription));
	}
}

void CDlgTaskDaily::OnEventLButtonDown_Txt_NPC(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
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

				if( idInstance == GetWorld()->GetInstanceID() && GetHostPlayer()->GetModelTempArrow() )
					GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
			}
		}
	}
}

void CDlgTaskDaily::OnEventMouseMove_Txt_NPC (WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	AUITEXTAREA_EDITBOX_ITEM Item;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	GetItemLinkItemOn(x, y, pObj, &Item);
}

void CDlgTaskDaily::GetItemLinkItemOn(int x, int y,
	PAUIOBJECT pObj, AUITEXTAREA_EDITBOX_ITEM *pLink)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT ptPos = pObj->GetPos();
	PAUITEXTAREA pText = (PAUITEXTAREA)pObj;

	GetGame()->ChangeCursor(RES_CUR_NORMAL);
	if( pText->GetHitArea(x - ptPos.x, y - ptPos.y) == AUITEXTAREA_RECT_FRAME )
	{
		int i;
		abase::vector<AUITEXTAREA_EDITBOX_ITEM> &vecItemLink = pText->GetEditBoxItems();

		x += p->X;
		y += p->Y;
		for( i = 0; i < (int)vecItemLink.size(); i++ )
		{
			if( (vecItemLink[i].m_pItem->GetType() == enumEIIvtrlItem ||
				vecItemLink[i].m_pItem->GetType() == enumEICoord) &&
				vecItemLink[i].rc.PtInRect(x, y) )
			{
				GetGame()->ChangeCursor(RES_CUR_HAND);
				*pLink = vecItemLink[i];
				break;
			}
		}
	}
}

bool CDlgTaskDaily::OnInitDialog()
{
	m_bSortTime		= false;
	m_bSortTaskName	= false;
	m_bSortLevel	= false;
	m_bSortType		= false;
	m_bSortNumber	= false;
	m_bSortSuggest	= false;
	m_bShowRed		= true;
	m_bShowBlue		= true;
	m_bShowGreen	= true;
	m_iCurSel		= 0;

	m_pLst_Task		= (PAUILISTBOX)GetDlgItem("Lst_Task");
	m_pTxt_TaskName = (PAUITEXTAREA)GetDlgItem("Txt_TaskName");
	m_pTxt_NPC		= (PAUITEXTAREA)GetDlgItem("Txt_NPC");
	m_pTxt_Line		= (PAUITEXTAREA)GetDlgItem("Txt_Line");
	m_pTxt_Desp		= (PAUITEXTAREA)GetDlgItem("Txt_Desc");
	m_pTxt_HotTask	= (PAUITEXTAREA)GetDlgItem("Txt_HotTask");
	m_pTxt_Explain	= (PAUITEXTAREA)GetDlgItem("Txa_Tj");
	m_pTxt_Explain->SetTransparent(true);

	m_dwCurDay		= 1;

	AWScriptFile s;
	int		index = 0;

	if( s.Open("surfaces\\ingame\\taskpopular.txt") )
	{
		if( s.GetNextToken(true) )
				m_strTaskPopular = s.m_szToken;
		s.Close();
	}
	if( s.Open("surfaces\\ingame\\taskdaily.txt") )
	{
		while( !s.IsEnd() )
		{
			if( !s.GetNextToken(true) )
				break;

			TaskDailyInfo TDInfo;
			DWORD i;
			for( i = 0; i<7; i++)
				TDInfo.bDays[i] = false;
			for( i = 0; i<15; i++)
				TDInfo.bLines[i] = false;
			TDInfo.taskType = TASKDAILY_TYPE_MIN;

			BEGIN_FAKE_WHILE2
			CSplit sp = s.m_szToken;
			CSPLIT_STRING_VECTOR vec = sp.Split(_AL(","));
			DWORD i;
			for( i = 0; i < vec.size(); i++ )
			{
				TDInfo.bDays[a_atoi(vec[i])-1] = true;
			}
			
			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.strTime = s.m_szToken;
			swscanf(s.m_szToken,_AL("%d:%d-%d:%d"),&TDInfo.tmStartTime.tm_hour, 
				&TDInfo.tmStartTime.tm_min, &TDInfo.tmEncTime.tm_hour, &TDInfo.tmEncTime.tm_min);
			TDInfo.bNoTimeLimit = true;

			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.strName = s.m_szToken;
			
			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.strLevelInfo = s.m_szToken;
			int len = TDInfo.strLevelInfo.GetLength();
			int loc = TDInfo.strLevelInfo.Find('-');
			TDInfo.ulMinLevel = 1;
			TDInfo.ulMaxLevel = 10000;
			if(-1==loc)
			{
				TDInfo.ulMinLevel = a_atoi(TDInfo.strLevelInfo);
				TDInfo.ulMaxLevel = a_atoi(TDInfo.strLevelInfo);
				TDInfo.strLevelInfo.Format(GetStringFromTable(1062),TDInfo.ulMaxLevel);
			}
			else if(loc+1 == len)
			{
				TDInfo.ulMinLevel = a_atoi(TDInfo.strLevelInfo);
				TDInfo.strLevelInfo.Format(GetStringFromTable(1063),TDInfo.ulMinLevel);
			}
			else
			{
				TDInfo.ulMinLevel = a_atoi(TDInfo.strLevelInfo);
				TDInfo.strLevelInfo = TDInfo.strLevelInfo.Mid(loc+1);
				TDInfo.ulMaxLevel = a_atoi(TDInfo.strLevelInfo);
				TDInfo.strLevelInfo.Format(GetStringFromTable(1064),TDInfo.ulMinLevel,TDInfo.ulMaxLevel);
			}

			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.bLoginShow = a_atoi(s.m_szToken)>0? true:false;

			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.bSpecial = a_atoi(s.m_szToken)>0? true:false;

			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.strNPC = s.m_szToken;

			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.strType = s.m_szToken;

			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.iNumMax = a_atoi(s.m_szToken);
			if( TDInfo.iNumMax<=0 )
				TDInfo.bNoCountLimit = true;
			else
				TDInfo.bNoCountLimit = false;

			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.uiCommend = a_atoi(s.m_szToken);

			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.strLineInfo = s.m_szToken;
			
			CHECK_BREAK2(s.GetNextToken(true));
			sp = s.m_szToken;
			vec = sp.Split(_AL(","));
			TDInfo.uiNumID = vec.size();
			TDInfo.iCurTaskShow = 0;
			for( i = 0; i < TDInfo.uiNumID; i++ )
			{
				TDInfo.uiTaskID[i] = a_atoi(vec[i]);
			}

			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.strDescription = s.m_szToken;
			
			END_FAKE_WHILE2;

			for( i = 0; i < 7; i++ )
			{
				if(TDInfo.bDays[i])
					m_vecTaskDaily[i].push_back(TDInfo);
			}
			m_AllTaskDaily.push_back(TDInfo);
		}
		s.Close();
	}
	else
		AUI_ReportError(__LINE__, 1,"CDlgTaskDaily::OnInitDialog(), task daily file error!");

	return true;
}

void CDlgTaskDaily::OnShowDialog()
{
	tm tmTime = g_pGame->GetServerLocalTime();
	char szText[20];
	sprintf(szText, "rdo%d", tmTime.tm_wday == 0 ? 7 : tmTime.tm_wday);
	OnCommand_Rdo(szText);
	CheckRadioButton(1, tmTime.tm_wday == 0 ? 7 : tmTime.tm_wday);
	m_pTxt_HotTask->SetText(m_strTaskPopular);
}

void CDlgTaskDaily::OnTick(void)
{
	int nSel = m_pLst_Task->GetCurSel();
	if (m_iCurSel != nSel)
	{
		OnEventLButtonDown_Lst_Task(0,0,0);
	}
}

void CDlgTaskDaily::OnCommand_Btn_Time(const char * szCommand)
{
	m_bSortTime = !m_bSortTime;
	if( m_bSortTime )
		m_pLst_Task->SortItemsByData(AUILISTBOX_SORT_ASCENT, AUILISTBOX_DATASORTTYPE_INT, 1);
	else
		m_pLst_Task->SortItemsByData(AUILISTBOX_SORT_DESCENT, AUILISTBOX_DATASORTTYPE_INT, 1);
}

void CDlgTaskDaily::OnCommand_Btn_TaskName(const char * szCommand)
{
	m_bSortTaskName	= !m_bSortTaskName;
	if( m_bSortTaskName )
		m_pLst_Task->SortItems(AUILISTBOX_SORT_ASCENT, AUILISTBOX_SORTTYPE_UNISTRING, 2);
	else
		m_pLst_Task->SortItems(AUILISTBOX_SORT_DESCENT, AUILISTBOX_SORTTYPE_UNISTRING, 2);
}

void CDlgTaskDaily::OnCommand_Btn_Level(const char * szCommand)
{
	m_bSortLevel = !m_bSortLevel;
	if( m_bSortLevel )
		m_pLst_Task->SortItemsByData(AUILISTBOX_SORT_ASCENT, AUILISTBOX_DATASORTTYPE_INT, 3);
	else
		m_pLst_Task->SortItemsByData(AUILISTBOX_SORT_DESCENT, AUILISTBOX_DATASORTTYPE_INT, 3);

}

void CDlgTaskDaily::OnCommand_Btn_Type(const char * szCommand)
{
	m_bSortType = !m_bSortType;
	if( m_bSortType )
		m_pLst_Task->SortItems(AUILISTBOX_SORT_ASCENT, AUILISTBOX_SORTTYPE_UNISTRING, 4);
	else
		m_pLst_Task->SortItems(AUILISTBOX_SORT_DESCENT, AUILISTBOX_SORTTYPE_UNISTRING, 4);

}

void CDlgTaskDaily::OnCommand_Btn_Number(const char * szCommand)
{
	m_bSortNumber = !m_bSortNumber;
	if( m_bSortNumber )
		m_pLst_Task->SortItemsByData(AUILISTBOX_SORT_ASCENT, AUILISTBOX_DATASORTTYPE_INT, 5);
	else
		m_pLst_Task->SortItemsByData(AUILISTBOX_SORT_DESCENT, AUILISTBOX_DATASORTTYPE_INT, 5);
}

void CDlgTaskDaily::OnCommand_Btn_Suggest(const char * szCommand)
{
	m_bSortSuggest = !m_bSortSuggest;
	if( m_bSortSuggest )
		m_pLst_Task->SortItems(AUILISTBOX_SORT_ASCENT, AUILISTBOX_SORTTYPE_STRING, 6);
	else
		m_pLst_Task->SortItems(AUILISTBOX_SORT_DESCENT, AUILISTBOX_SORTTYPE_STRING, 6);
}

void CDlgTaskDaily::OnCommand_Chk_tj1(const char * szCommand)
{
	m_bShowRed = !m_bShowRed;
	char szText[20];
	sprintf(szText, "rdo%d", m_dwCurDay);
	OnCommand_Rdo(szText);
}

void CDlgTaskDaily::OnCommand_Chk_tj2(const char * szCommand)
{
	m_bShowBlue = !m_bShowBlue;
	char szText[20];
	sprintf(szText, "rdo%d", m_dwCurDay);
	OnCommand_Rdo(szText);
}
