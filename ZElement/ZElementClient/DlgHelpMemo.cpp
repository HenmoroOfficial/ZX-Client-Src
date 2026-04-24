// Filename	:	DlgHelpMemo.cpp
// Creator	:	Fu Chonggang
// Date		:	Aug 17, 2009

#include "AFI.h"
#include "AUI\\CSplit.h"
#include "DlgHelpMemo.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIDef.h"
#include "DlgTaskDaily.h"
#include "DlgHelpFariy.h"

#include "time.h"

#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Global.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgHelpMemo, CDlgBase)

AUI_ON_COMMAND("Btn_Apply",				OnCommand_apply)
AUI_ON_COMMAND("Btn_Cancel",			OnCommand_cancel)
AUI_ON_COMMAND("Btn_Close",				OnCommand_close)
AUI_ON_COMMAND("Btn_Confirm",			OnCommand_confirm)
AUI_ON_COMMAND("Btn_ConfirmType",		OnCommand_confirmType)
AUI_ON_COMMAND("Chk_*",					OnCommand_ChkRemind)
AUI_ON_COMMAND("RdoWeek_*",				OnCommand_RdoWeek)
AUI_ON_COMMAND("RdoType_*",				OnCommand_RdoType)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgHelpMemo, CDlgBase)

AUI_ON_EVENT("*",				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,				WM_MOUSEWHEEL,		OnEventMouseWheel)

AUI_END_EVENT_MAP()

AUILabel *CDlgHelpMemo::m_pLab_Time[DLGHELPMEMO_NUMPERPAGE];
AUILabel *CDlgHelpMemo::m_pLab_Task[DLGHELPMEMO_NUMPERPAGE];
int CDlgHelpMemo::m_iType = 0;
int CDlgHelpMemo::m_iWeek = 0;
int CDlgHelpMemo::m_iCurIndex = 0;
DWORD CDlgHelpMemo::m_dwNextMemoTime = 0;
CDlgHelpMemo::TaskRemind CDlgHelpMemo::m_nextRemind;
CDlgHelpMemo::TaskRemindMap CDlgHelpMemo::m_mapRemind;
CDlgHelpMemo::TaskRemind CDlgHelpMemo::m_vecCustome[DLGHELPMEMO_NUMCUSTOMIZE];
abase::vector<CDlgHelpMemo::TaskRemind*> CDlgHelpMemo::m_vecReminding;

const static int numTaskDailyPerPage = 6;


CDlgHelpMemo::CDlgHelpMemo()
{
}

CDlgHelpMemo::~CDlgHelpMemo()
{
}

bool CDlgHelpMemo::Release()
{
	ClearRemindings();
	return CDlgBase::Release();
}

bool CDlgHelpMemo::OnInitDialog()
{
	AString strName;
	int i;
	if (stricmp(m_szName, "Win_Help_Memo")==0)
	{
		m_pTitle= (AUILabel *)GetDlgItem("Title");
		m_pBtn_Apply= (AUIStillImageButton *)GetDlgItem("Btn_Apply");
		m_pBtn_Cancel= (AUIStillImageButton *)GetDlgItem("Btn_Cancel");
		m_pBtn_Close= (AUIStillImageButton *)GetDlgItem("Btn_Close");
		m_pBtn_Confirm= (AUIStillImageButton *)GetDlgItem("Btn_Confirm");
		m_pRdo_Today= (AUIRadioButton *)GetDlgItem("Rdo_Today");
		m_pRdo_System= (AUIRadioButton *)GetDlgItem("Rdo_System");
		m_pRdo_Custom= (AUIRadioButton *)GetDlgItem("Rdo_Custom");
		m_pTxt_LvUp= (AUITextArea *)GetDlgItem("Txt_LvUp");
		m_pScl_Memo= (AUIScroll *)GetDlgItem("Scl_Memo");
		m_pTxt_LvUp->SetAcceptMouseMessage(false);
		m_nFirstLine = -1;
		for(i=0; i<DLGHELPMEMO_NUMPERPAGE; i++)
		{
			strName.Format("Chk_%d", i+1);
			m_pChk_Index[i]= (AUICheckBox *)GetDlgItem(strName);
			strName.Format("Lab_Time%d", i+1);
			m_pLab_Time[i] = (AUILabel *)GetDlgItem(strName);
			strName.Format("Lab_Task%d", i+1);
			m_pLab_Task[i] = (AUILabel *)GetDlgItem(strName);
		}
		m_dwNextMemoTime = 0;
		m_nextRemind.strName.Empty();
		m_nextRemind.strTime.Empty();
		ClearRemindings();
	}
	else if ( stricmp(m_szName, "Win_Help_MemoCustom") == 0 )
	{
		for(i=0; i<DLGHELPMEMO_NUMCUSTOMIZE; i++)
		{
			strName.Format("Chk_%d", i+1);
			m_pChk_Index[i]= (AUICheckBox *)GetDlgItem(strName);
			m_vecCustome[i].iRemind = 1;
		}
		GetDlgItem("Txt_LvUp")->SetAcceptMouseMessage(false);
	}
	return true;
}

void CDlgHelpMemo::OnShowDialog()
{
	if ( stricmp(m_szName, "Win_Help_Memo") == 0 )
	{
		if (GetCheckedRadioButton(1) == -1)
		{
			tm tmTime = GetGame()->GetServerLocalTime();
			m_iWeek = tmTime.tm_wday+1;
			m_iType = HELPMEMOTYPE_TODAY;
			OnCommand_RdoType("RdoType_Today");
		}
		if (m_iType>HELPMEMOTYPE_SYSTEM)
		{
			m_iType = HELPMEMOTYPE_TODAY;
		}
		CheckRadioButton(1,m_iWeek);
		CheckRadioButton(2,m_iType);
	}
	else if ( stricmp(m_szName, "Win_Help_MemoCustom") == 0 )
	{
		AString strName;
		ACString strText;
		for(int i=0; i<DLGHELPMEMO_NUMCUSTOMIZE; i++)
		{
			m_pChk_Index[i]->Check(m_vecCustome[i].iRemind > 0);
			strName.Format("Edt_Task%d",i+1);
			PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem(strName);
			pEdt->SetText(m_vecCustome[i].strName);
			strName.Format("Edt_Hour%d",i+1);
			PAUIEDITBOX pEdtHour = (PAUIEDITBOX)GetDlgItem(strName);
			strText.Format(_AL("%d"),m_vecCustome[i].tmStartTime.tm_hour);
			pEdtHour->SetText(strText);
			strName.Format("Edt_Min%d",i+1);
			PAUIEDITBOX pEdtMin = (PAUIEDITBOX)GetDlgItem(strName);
			strText.Format(_AL("%d"),m_vecCustome[i].tmStartTime.tm_min);
			pEdtMin->SetText(strText);
		}
		CheckRadioButton(2,3);
		tm tmTime = GetGame()->GetServerLocalTime();
		m_iWeek = tmTime.tm_wday+1;
		CheckRadioButton(1,m_iWeek);
	}
}

void CDlgHelpMemo::OnTick()
{
	if ( stricmp(m_szName, "Win_Help_Memo") ==0 )
	{
		if (m_nFirstLine!= m_pScl_Memo->GetBarLevel())
			SetInfo();
	}
	else if ( stricmp(m_szName, "Win_Help_MemoCustom") ==0 )
	{

	}
}

bool CDlgHelpMemo::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

void CDlgHelpMemo::OnCommand_apply(const char * szCommand)
{
	if ( stricmp(m_szName, "Win_Help_MemoCustom") ==0 )
	{
		AString strName;
		ACString strText;
		for (int i=0; i<DLGHELPMEMO_NUMCUSTOMIZE; i++)
		{
			strName.Format("Edt_Task%d",i+1);
			PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem(strName);
			m_vecCustome[i].strName = pEdt->GetText();
			strName.Format("Edt_Hour%d",i+1);
			PAUIEDITBOX pEdtHour = (PAUIEDITBOX)GetDlgItem(strName);
			strName.Format("Edt_Min%d",i+1);
			PAUIEDITBOX pEdtMin = (PAUIEDITBOX)GetDlgItem(strName);
			strText.Format(_AL("%s:%s"),pEdtHour->GetText(),pEdtMin->GetText());
			swscanf(strText,_AL("%d:%d-%d:%d"),&m_vecCustome[i].tmStartTime.tm_hour, 
				&m_vecCustome[i].tmStartTime.tm_min, &m_vecCustome[i].tmEncTime.tm_hour, &m_vecCustome[i].tmEncTime.tm_min);
			m_vecCustome[i].strTime = strText;
		}
	}
	SaveRemind();
}

void CDlgHelpMemo::OnCommand_cancel(const char * szCommand)
{
	Show(false);
}

void CDlgHelpMemo::OnCommand_close(const char * szCommand)
{
	Show(false);
}

void CDlgHelpMemo::OnCommand_confirm(const char * szCommand)
{
	OnCommand_apply("");
	Show(false);
}

void CDlgHelpMemo::OnCommand_confirmType(const char * szCommand)
{
	Show(false);
	if (GetGameUIMan()->GetDialog("Win_Help_Memo")->IsShow())
	{
		ACString strName = m_pLab_Task[m_iCurIndex]->GetText();
		TaskRemindMap::iterator it = m_mapRemind.find(strName);
		if (it!= m_mapRemind.end())
		{
			TaskRemind taskItem = it->second;
			taskItem.iRemind = GetCheckedRadioButton(1);
			m_mapRemind[strName] = taskItem;
		}
		else
			assert(0);
	}
	else if (GetGameUIMan()->GetDialog("Win_Help_MemoCustom")->IsShow())
	{
		m_vecCustome[m_iCurIndex].iRemind = GetCheckedRadioButton(1);
	}
}

void CDlgHelpMemo::OnCommand_ChkRemind(const char * szCommand)
{
	int iIndex = atoi(szCommand+strlen("Chk_")) - 1;
	m_iCurIndex = iIndex;
	if ( stricmp(m_szName, "Win_Help_Memo") ==0 )
	{
		ACString strName = m_pLab_Task[iIndex]->GetText();
		TaskRemindMap::iterator it = m_mapRemind.find(strName);
		if (it!= m_mapRemind.end())
		{
			TaskRemind taskItem = it->second;
			taskItem.iRemind = m_pChk_Index[iIndex]->IsChecked();
			m_mapRemind[taskItem.strName] = taskItem;
			if (m_pChk_Index[iIndex]->IsChecked())
			{
				GetGameUIMan()->GetDialog("Win_HelpMemoWay")->Show(true);
			}
		}
	}
	else if ( stricmp(m_szName, "Win_Help_MemoCustom") ==0 )
	{
		m_vecCustome[m_iCurIndex].iRemind = m_pChk_Index[iIndex]->IsChecked();
		if (m_pChk_Index[iIndex]->IsChecked())
			GetGameUIMan()->GetDialog("Win_HelpMemoWay")->Show(true);
	}
}

void CDlgHelpMemo::OnCommand_RdoWeek(const char * szCommand)
{
	int iWeek = atoi(szCommand+strlen("RdoWeek_"));
	tm tmTime = GetGame()->GetServerLocalTime();
	if (m_iType == HELPMEMOTYPE_TODAY && tmTime.tm_wday!=(iWeek-1))
	{
		return;
	}
	m_iWeek = iWeek;
	SetInfo(true);
}

void CDlgHelpMemo::OnCommand_RdoType(const char * szCommand)
{
	const char *szType = szCommand + strlen("RdoType_");
	if (stricmp(szType, "Today")==0)
	{
		if (m_iType == HELPMEMOTYPE_CUSTOM)
		{
			Show(false);
			m_iType = HELPMEMOTYPE_TODAY;
			GetGameUIMan()->GetDialog("Win_Help_Memo")->Show(true);
		}
		m_iType = HELPMEMOTYPE_TODAY;
	}
	else if (stricmp(szType, "System")==0)
	{
		if (m_iType == HELPMEMOTYPE_CUSTOM)
		{
			Show(false);
			m_iType = HELPMEMOTYPE_SYSTEM;
			GetGameUIMan()->GetDialog("Win_Help_Memo")->Show(true);
		}
		m_iType = HELPMEMOTYPE_SYSTEM;
	}
	else if (stricmp(szType, "Custom")==0)
	{
		Show(false);
		m_iType = HELPMEMOTYPE_CUSTOM;
		GetGameUIMan()->GetDialog("Win_Help_MemoCustom")->Show(true);
	}
	tm tmTime = GetGame()->GetServerLocalTime();
	char szText[20];
	sprintf(szText, "RdoWeek_%d", tmTime.tm_wday == 0 ? 7 : tmTime.tm_wday);
	m_iWeek = tmTime.tm_wday+1;
	CheckRadioButton(1,m_iWeek);
	OnCommand_RdoWeek(szText);
}

void CDlgHelpMemo::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (stricmp(m_szName, "Win_Help_Memo") ==0)
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		int zDelta = (short)HIWORD(wParam);
		if( zDelta > 0 )
			zDelta = 1;
		else
			zDelta = -1;
		if( m_pScl_Memo->IsShow() )
		{
			int iBarLevel = m_pScl_Memo->GetBarLevel();
			m_pScl_Memo->SetBarLevel(m_pScl_Memo->GetBarLevel() - zDelta);
			if (iBarLevel != m_pScl_Memo->GetBarLevel())
			{
				SetInfo();
			}
		}
	}
}

void CDlgHelpMemo::SetInfo(bool bInit /*= false*/)
{
	if ( stricmp(m_szName, "Win_Help_Memo") ==0 )
	{
		abase::vector<TaskDailyInfo>* dailyInfo = CDlgTaskDaily::GetTaskDailyInfo(m_iWeek);
		abase::vector<TaskDailyInfo>::iterator iterW = dailyInfo->begin();
		if (m_nFirstLine<0 || bInit)
		{
			m_nFirstLine = 0;
			if (dailyInfo->size()>numTaskDailyPerPage)
			{
				m_pScl_Memo->Show(true);
				m_pScl_Memo->SetScrollRange(0, dailyInfo->size()-numTaskDailyPerPage);
				m_pScl_Memo->SetBarLength(-1);
				m_pScl_Memo->SetScrollStep(1);
				m_pScl_Memo->SetBarLevel(0);
			}
			else
				m_pScl_Memo->Show(false);
		}
		int iBegin = m_pScl_Memo->GetBarLevel();
		m_nFirstLine = iBegin;
		int index = 0;
		for( ;iterW != dailyInfo->end(); ++iterW)
		{
			if (iBegin-->0)
			{
				continue;
			}
			if (index>=DLGHELPMEMO_NUMPERPAGE)
			{
				break;
			}
			m_pLab_Time[index]->SetText(iterW->strTime);
			m_pLab_Task[index]->SetText(iterW->strName);
			TaskRemindMap::iterator it = m_mapRemind.find(iterW->strName);
			if (it!= m_mapRemind.end())
			{
				TaskRemind taskItem = it->second;
				m_pChk_Index[index]->Check(taskItem.iRemind>0);
			}
			else
			{
				TaskRemind taskItem;
				taskItem.strName = iterW->strName;
				taskItem.strTime = iterW->strTime;
				swscanf(iterW->strTime,_AL("%d:%d-%d:%d"),&taskItem.tmStartTime.tm_hour, 
				&taskItem.tmStartTime.tm_min, &taskItem.tmEncTime.tm_hour, &taskItem.tmEncTime.tm_min);
				taskItem.iRemind = 1;
				m_mapRemind[iterW->strName] = taskItem;
				m_pChk_Index[index]->Check(true);
			}
			index++;
		}
	}
	else if ( stricmp(m_szName, "Win_Help_MemoCustom") ==0 )
	{

	}
}

void CDlgHelpMemo::TickMemo()
{
	CDlgHelpFariy* pDlg = (CDlgHelpFariy*)GetGameUIMan()->GetDialog("Win_Help_Fairy");
	DWORD dwNow = GetTickCount();
	if (0==m_dwNextMemoTime)
	{
		CalRecentMemo();
	}
	else if (dwNow>m_dwNextMemoTime && !pDlg->IsShow())
	{
		pDlg->Show(true);
		CalRecentMemo();
	}

}

bool CDlgHelpMemo::LoadRemind()
{
	char szPath[MAX_PATH];
	sprintf(szPath, "%s\\UserData\\HelpMemo\\%d.txt", af_GetBaseDir(), g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID());
	AFile afile;
	int index=0;
	m_mapRemind.clear();
	if (afile.Open(szPath, AFILE_OPENEXIST|AFILE_TEXT|AFILE_NOHEAD|AFILE_TEMPMEMORY))
	{
		char line[AFILE_LINEMAXLEN];
		DWORD  read_len;
		while (afile.ReadLine(line, AFILE_LINEMAXLEN, &read_len))
		{
			CSplit s(AS2WC(line));
			CSplit::VectorAWString vec = s.Split(_AL("\t"));
			if( vec.size() == 3 )
			{
				TaskRemind taskItem;
				taskItem.strName = vec[0];
				taskItem.strTime = vec[1];
				swscanf(taskItem.strTime,_AL("%d:%d-%d:%d"),&taskItem.tmStartTime.tm_hour, 
				&taskItem.tmStartTime.tm_min, &taskItem.tmEncTime.tm_hour, &taskItem.tmEncTime.tm_min);
				taskItem.iRemind = a_atoi(vec[2]);
				if ( ++index > DLGHELPMEMO_NUMCUSTOMIZE )
					m_mapRemind[taskItem.strName] = taskItem;
				else
					m_vecCustome[index-1] = taskItem;
			}
			else if (index < DLGHELPMEMO_NUMCUSTOMIZE)
			{
				index++;
			}
			else
				assert(0);
		}
		afile.Close();
	}
	
	return true;
}

bool CDlgHelpMemo::SaveRemind()
{
	char szPath[MAX_PATH];
	sprintf(szPath, "%s\\UserData\\HelpMemo", af_GetBaseDir());
	CreateDirectoryA(szPath, NULL);
	sprintf(szPath, "%s\\UserData\\HelpMemo\\%d.txt", af_GetBaseDir(), g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID());
	AFile afile;
	if (!afile.Open(szPath, AFILE_CREATENEW|AFILE_TEXT|AFILE_NOHEAD|AFILE_TEMPMEMORY))
	{
		return false;
	}
	for (UINT i=0; i<DLGHELPMEMO_NUMCUSTOMIZE; i++)
	{
		TaskRemind taskItem = m_vecCustome[i];
		ACString strText;
		if (taskItem.strName.GetLength()>0)
		{
			strText.Format(_AL("%s\t%s\t%d"), taskItem.strName, taskItem.strTime, taskItem.iRemind);
		}
		else
			strText.Format(_AL(" \t%s\t%d"), taskItem.strTime, taskItem.iRemind);
		AString str(WC2AS(strText));
		afile.WriteLine(str);
	}
	TaskRemindMap::iterator it = m_mapRemind.begin();
	for (; it!=m_mapRemind.end(); ++it)
	{
		TaskRemind taskItem = it->second;
		ACString strText;
		strText.Format(_AL("%s\t%s\t%d"), taskItem.strName, taskItem.strTime, taskItem.iRemind);
		AString str(WC2AS(strText));
		afile.WriteLine(str);
	}
	afile.Close();
	m_nextRemind.strName.Empty();
	m_nextRemind.iRemind = 0;
	CalRecentMemo();
	return true;
}

//返回下一个提醒是否在10分钟提醒时间内
void CDlgHelpMemo::CalRecentMemo()
{
	ClearRemindings();
	tm tmTime = GetGame()->GetServerLocalTime();
	int iMin = 99999;
	TaskRemind lastItem = m_nextRemind;
	int iMinTemp = 0;
	//计算下一个提醒
	int i;
	for (i=0; i<DLGHELPMEMO_NUMCUSTOMIZE; i++)
	{
		//不提醒
		if (m_vecCustome[i].iRemind==0)
			continue;
		TaskRemind taskItem = m_vecCustome[i];
		iMinTemp = 60*taskItem.tmStartTime.tm_hour+taskItem.tmStartTime.tm_min - 60*tmTime.tm_hour-tmTime.tm_min;
		if (iMinTemp<0)
		{
			iMinTemp += 60 * 24;
		}
		if (lastItem.strName == taskItem.strName)
		{
			continue;
		}
		if (iMinTemp<=10)
		{
			TaskRemind *pRemind = new TaskRemind;
			pRemind->strName = taskItem.strName;
			pRemind->strTime = taskItem.strTime;
			m_vecReminding.push_back(pRemind);
		}
		else if (iMinTemp<iMin)
		{
			iMin = iMinTemp;
			m_nextRemind = taskItem;
			m_nextRemind.iRemind = (iMin<10);
		}
	}

	abase::vector<TaskDailyInfo>* dailyInfo = CDlgTaskDaily::GetTaskDailyInfo(m_iWeek);
	abase::vector<TaskDailyInfo>::iterator iterW = dailyInfo->begin();
	while(iterW != dailyInfo->end())
	{
		TaskRemind taskItem;
		taskItem.strName = iterW->strName;
		taskItem.strTime = iterW->strTime;
		swscanf(taskItem.strTime,_AL("%d:%d-%d:%d"),&taskItem.tmStartTime.tm_hour, 
			&taskItem.tmStartTime.tm_min, &taskItem.tmEncTime.tm_hour, &taskItem.tmEncTime.tm_min);
		TaskRemindMap::iterator it = m_mapRemind.find(taskItem.strName);
		if (it!= m_mapRemind.end())
		{
			TaskRemind taskItem = it->second;
			if (taskItem.iRemind==0)
			{
				//不提醒
				++iterW;
				++i;
				continue;
			}
		}
		
		iMinTemp = 60*iterW->tmStartTime.tm_hour+iterW->tmStartTime.tm_min - 60*tmTime.tm_hour-tmTime.tm_min;
		if (iMinTemp<0)
		{
			iMinTemp += 60 * 24;
		}
		if (lastItem.strName == taskItem.strName)
		{
			++iterW;
			continue;
		}
		if (iMinTemp<=10)
		{
			TaskRemind *pRemind = new TaskRemind;
			pRemind->strName = taskItem.strName;
			pRemind->strTime = taskItem.strTime;
			m_vecReminding.push_back(pRemind);
		}
		else if (iMinTemp<iMin)
		{
			iMin = iMinTemp;
			m_nextRemind = taskItem;
			m_nextRemind.iRemind = (iMin<10);
		}
		++iterW;
		++i;
	}
	m_dwNextMemoTime =  GetTickCount() + iMin*60000 - 10*60000;
	if (m_vecReminding.size()>0 && !GetGameUIMan()->GetDialog("Win_Help_Fairy")->IsShow())
	{
		CloseOneReminding();
	}
}

bool CDlgHelpMemo::CloseOneReminding()
{
	if (m_vecReminding.size()>0)
	{
		TaskRemind* backRemind = m_vecReminding.back();
		ACString strText;
		strText.Format(_AL("%s\r%s"), backRemind->strTime, backRemind->strName);
		CDlgHelpFariy* pDlg = (CDlgHelpFariy*)GetGameUIMan()->GetDialog("Win_Help_Fairy");
		if (!pDlg->IsShow())
		{
			pDlg->Show(true);
		}
		PAUILABEL pLab = (PAUILABEL)pDlg->GetDlgItem("Txt_Hint");
		pLab->SetText(strText);

		delete backRemind;
		m_vecReminding.pop_back();
		return true;
	}
	else
		return false;
}

void CDlgHelpMemo::ClearRemindings()
{
	for (UINT i=0; i<m_vecReminding.size(); i++)
	{
		TaskRemind *pRemind = m_vecReminding[i];
		delete pRemind;
	}
	m_vecReminding.clear();
}