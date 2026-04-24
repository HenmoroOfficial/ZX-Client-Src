// Filename	:	DlgClassmateManage.cpp
// Creator	:	Fu Chonggang
// Date		:	May 17, 2010

#include "DlgClassmateManage.h"
#include "DlgSystem4.h"
#include "DlgGuildman.h"

#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIComboBox.h"

#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_Circle.h"
#include "EC_HostPlayer.h"
#include "EC_GameSession.h"
#include "EC_GameRun.h"
#include "EC_Faction.h"
#include "EC_TaskInterface.h"
#include "A2DSprite.h"
#include "Tasktempl.h"
#include "TaskTemplMan.h"

#define new A_DEBUG_NEW

enum
{
	CIRCLE_TASK_AWARD_ID	= 0,
	CIRCLE_TASK_LV_LOW		= 1,
	CIRCLE_TASK_LV_HIGH		= 2,
	CIRCLE_TASK_POINT		= 3,
	CIRCLE_TASK_ID			= 4,
	CIRCLE_TASK_INFO_NUM,

};

static const int s_idAwardTask[][CIRCLE_TASK_INFO_NUM] =
{
//  领取奖励任务id，最低等级，最高等级，每天领取奖励所需人气，领取每日任务id
	{21070, 10, 30, 500, 21218},
	{21071, 31, 45, 450, 21073},
	{21075, 46, 60, 400, 21073},
	{21076, 61, 75, 350, 21073},
	{21077, 76, 90, 300, 21073},
	{21078, 91, 105,250, 21073},
	{21079, 106,120,200, 21073},
	{21080, 121,135,150, 21073},
	{21081, 136,150,100, 21073},
};

static const int s_idTask = 21073;

AUI_BEGIN_COMMAND_MAP(CDlgClassmateManage, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_ON_COMMAND("help",					OnCommand_help)

AUI_ON_COMMAND("SortName",				OnCommand_sortname)
AUI_ON_COMMAND("Btn_SortGender",		OnCommand_sortgender)
AUI_ON_COMMAND("sortlevel",				OnCommand_sortlevel)
AUI_ON_COMMAND("sortprof",				OnCommand_sortprof)
AUI_ON_COMMAND("Btn_Timein",			OnCommand_timein)
AUI_ON_COMMAND("Btn_Timeout",			OnCommand_timeout)
AUI_ON_COMMAND("sortol",				OnCommand_sortline)
AUI_ON_COMMAND("top",					OnCommand_top)
AUI_ON_COMMAND("prec",					OnCommand_prec)
AUI_ON_COMMAND("next",					OnCommand_next)
AUI_ON_COMMAND("bottom",				OnCommand_bottom)
AUI_ON_COMMAND("Btn_Refresh",			OnCommand_refresh)
AUI_ON_COMMAND("Btn_AwardExp",			OnCommand_GetAwardExp)
AUI_ON_COMMAND("Btn_GetAward",			OnCommand_GetAward)
AUI_ON_COMMAND("Btn_ShowFamily",		OnCommand_Family)
AUI_ON_COMMAND("Btn_ShowFaction",		OnCommand_Guild)
AUI_ON_COMMAND("Btn_GiveTask",			OnCommand_GiveTask)
AUI_ON_COMMAND("Combo_Display",			OnCommand_ShowType)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgClassmateManage, CDlgBase)

AUI_ON_EVENT("List_Member",	WM_RBUTTONUP,	OnEventRButtonUp)

AUI_END_EVENT_MAP()

CDlgClassmateManage::CDlgClassmateManage()
{
}

CDlgClassmateManage::~CDlgClassmateManage()
{
}

bool CDlgClassmateManage::Release()
{
	for (UINT i=0; i<m_vecImageList.size(); i++)
	{
		A3DRELEASE(m_vecImageList[i]);
	}
	m_vecImageList.clear();

	return CDlgBase::Release();
}

bool CDlgClassmateManage::OnInitDialog()
{
	m_pShape12 = (AUIEditBox *)GetDlgItem("Shape12");
	m_pImg_Title = (AUIImagePicture *)GetDlgItem("Img_Title");
	m_pTxt_ClassName = (AUILabel *)GetDlgItem("Txt_ClassName");
	m_pBtn_Help = (AUIStillImageButton *)GetDlgItem("Btn_Help");
	m_pBtn_SortName = (AUIStillImageButton *)GetDlgItem("Btn_SortName");
	m_pBtn_SortLevel = (AUIStillImageButton *)GetDlgItem("Btn_SortLevel");
	m_pBtn_SortProf = (AUIStillImageButton *)GetDlgItem("Btn_SortProf");
	m_pBtn_SortOL = (AUIStillImageButton *)GetDlgItem("Btn_SortOL");
	m_pBtn_Timein = (AUIStillImageButton *)GetDlgItem("Btn_Timein");
	m_pBtn_SortJob = (AUIStillImageButton *)GetDlgItem("Btn_SortJob");
	m_pList_Member = (AUIListBox *)GetDlgItem("List_Member");
	m_pLab_PplCap = (AUILabel *)GetDlgItem("Lab_PplCap");
	m_pTxt_PplCap = (AUILabel *)GetDlgItem("Txt_CreateTime");
	m_pLab_PplOnline = (AUILabel *)GetDlgItem("Lab_PplOnline");
	m_pTxt_PplOnline = (AUILabel *)GetDlgItem("Txt_PplOnline");
	m_pTxt_PplGrad = (AUILabel *)GetDlgItem("Txt_GraduateNum");
	m_pTxt_Message = (AUITextArea *)GetDlgItem("Txt_Message");
	m_pBtn_GuildInvite = (AUIStillImageButton *)GetDlgItem("Btn_GuildInvite");
	m_pTxt_DeleteTime = (AUILabel *)GetDlgItem("Txt_DeleteTime");
	m_pBtn_Timeout = (AUIStillImageButton *)GetDlgItem("Btn_Timeout");
	m_pCombo_Display = (AUIComboBox *)GetDlgItem("Combo_Display");
	int i;
	for (i=9500; i<=9502; i++)
	{
		m_pCombo_Display->AddString(GetGameUIMan()->GetStringFromTable(i));
	}
	m_pCombo_Display->SetCurSel(0);
	m_pBtn_Top = (AUIStillImageButton *)GetDlgItem("Btn_Top");
	m_pBtn_Prec = (AUIStillImageButton *)GetDlgItem("Btn_Prec");
	m_pBtn_Next = (AUIStillImageButton *)GetDlgItem("Btn_Next");
	m_pBtn_Bottom = (AUIStillImageButton *)GetDlgItem("Btn_Bottom");
	m_pBtn_Refresh = (AUIStillImageButton *)GetDlgItem("Btn_Refresh");
	m_pBtn_Award = (AUIStillImageButton *)GetDlgItem("Btn_GetAward");
	m_pBtn_AwardExp = (AUIStillImageButton *)GetDlgItem("Btn_AwardExp");
	m_pBtn_GiveTask = (AUIStillImageButton *)GetDlgItem("Btn_GiveTask");
	m_pLab_Developments = (AUILabel *)GetDlgItem("Lab_Developments");
	m_pLab_History = (AUILabel *)GetDlgItem("Lab_History");
	m_pList_History = (AUIListBox *)GetDlgItem("List_History");
	m_pLab_Point = (AUILabel *)GetDlgItem("Txt_AwardNum");
	m_pLab_AwardExp = (AUILabel *)GetDlgItem("Lab_ExpNum");
	GetDlgItem("Btn_Vote")->Show(false);

	m_vecImageList.clear();
	A2DSprite* pImageList = new A2DSprite;
	if( !pImageList ) return AUI_ReportError(__LINE__, __FILE__);
	
	bool bval = pImageList->Init(m_pA3DDevice, "icon\\class.tga", AUI_COLORKEY);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	A3DRECT rc[3];
	int W = pImageList->GetWidth();
	int H = pImageList->GetHeight() / 3;
	for( i = 0; i < 3; i++ )
		rc[i].SetRect(0, H * i, W, H * (i + 1));
	pImageList->ResetItems(4, rc);
	m_vecImageList.push_back(pImageList);
	m_pList_Member->SetImageList(&m_vecImageList);
	m_pList_Member->SetItemMask(0xffffffff);

	m_bFirstOpen =  true;
	m_nPage = 0;
	m_nSortMode = SORT_BY_ONLINE;
	m_nSuitAwardIndex = -1;
	m_nGradNum = 0;
	return true;
}

void CDlgClassmateManage::OnShowDialog()
{
	OnCommand_refresh("");
	BuildList();
}

void CDlgClassmateManage::OnTick()
{
	if (m_nSuitAwardIndex<0)
	{
		return;
	}
	m_pBtn_Refresh->Enable(GetTickCount()-m_dwLastRefreshTime>3000);
	
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	m_pBtn_Award->Enable(GetTickCount()-m_dwLastGiveAwardTime>6000 && pTask->CanDeliverTask(s_idAwardTask[m_nSuitAwardIndex][CIRCLE_TASK_AWARD_ID]) == 0);
	m_pBtn_GiveTask->Enable(GetTickCount()-m_dwLastGiveTaskTime>6000 && pTask->CanDeliverTask(s_idAwardTask[m_nSuitAwardIndex][CIRCLE_TASK_ID]) == 0);
}

bool CDlgClassmateManage::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

int CDlgClassmateManage::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="get_exp_re")
	{
		CECCircleMan* pCircleMan = GetGame()->GetCircleMan();
		pCircleMan->SetGradBonus(0);
		m_pLab_AwardExp->SetText(_AL("0"));
	}
	return 1;
}

void CDlgClassmateManage::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgClassmateManage::OnCommand_help(const char * szCommand)
{
}

void CDlgClassmateManage::OnCommand_sortname(const char * szCommand)
{
	m_nSortMode = SORT_BY_NAME;
	BuildClassMateList();
}

void CDlgClassmateManage::OnCommand_sortgender(const char * szCommand)
{
	m_nSortMode = SORT_BY_GENDER;
	BuildClassMateList();
}

void CDlgClassmateManage::OnCommand_sortlevel(const char * szCommand)
{
	m_nSortMode = SORT_BY_LEVEL;
	BuildClassMateList();
}

void CDlgClassmateManage::OnCommand_sortprof(const char * szCommand)
{
	m_nSortMode = SORT_BY_PROFESSION;
	BuildClassMateList();
}

void CDlgClassmateManage::OnCommand_timein(const char * szCommand)
{
	m_nSortMode = SORT_BY_TIMEIN;
	BuildClassMateList();
}

void CDlgClassmateManage::OnCommand_timeout(const char * szCommand)
{
	m_nSortMode = SORT_BY_TIMEOUT;
	BuildClassMateList();
}

void CDlgClassmateManage::OnCommand_sortline(const char * szCommand)
{
	m_nSortMode = SORT_BY_ONLINE;
	BuildClassMateList();
}

void CDlgClassmateManage::OnCommand_top(const char * szCommand)
{
	m_nPage = 0;
	BuildClassMateList();
}

void CDlgClassmateManage::OnCommand_prec(const char * szCommand)
{
	m_nPage--;
	BuildClassMateList();
}

void CDlgClassmateManage::OnCommand_next(const char * szCommand)
{
	CircleMemList &cml = GetGame()->GetCircleMan()->GetMemList();
	m_nPage++;
	BuildClassMateList();
}

void CDlgClassmateManage::OnCommand_bottom(const char * szCommand)
{
	CircleMemList &cml = GetGame()->GetCircleMan()->GetMemList();
	m_nPage = (cml.size()-1)/NUM_PER_PAGE;
	BuildClassMateList();
}

void CDlgClassmateManage::OnCommand_refresh(const char * szCommand)
{
	UINT uId = GetHostPlayer()->GetCircleID();
	GetGame()->GetGameSession()->circle_get_baseinfo(1, reinterpret_cast<int*>(&uId));
	GetGame()->GetGameSession()->circle_get_list(uId);
	m_dwLastRefreshTime = GetTickCount();
}

void CDlgClassmateManage::OnCommand_GetAwardExp(const char * szCommand)
{
	GetGameSession()->circle_get_award(GetHostPlayer()->GetCircleID());
}

void CDlgClassmateManage::OnCommand_GetAward(const char * szCommand)
{
	if (m_nSuitAwardIndex>=0)
	{
		GetGame()->GetTaskTemplateMan()->ManualTrigTask(GetHostPlayer()->GetTaskInterface(), s_idAwardTask[m_nSuitAwardIndex][CIRCLE_TASK_AWARD_ID]);
		m_dwLastGiveAwardTime = GetTickCount();
	}
}

void CDlgClassmateManage::OnCommand_Family(const char * szCommand)
{
	if( GetHostPlayer()->GetFamilyID() )
	{
		Show(false);
		CDlgSystem4* pDlgSystem4 = (CDlgSystem4*)GetGameUIMan()->GetDialog("Win_Main4");
		pDlgSystem4->OnCommandFaction("wfaction");
	}
}

void CDlgClassmateManage::OnCommand_Guild(const char * szCommand)
{
	if( GetHostPlayer()->GetFactionID() )
	{
		Show(false);
		CDlgGuildMan *pDlg = (CDlgGuildMan*)GetGameUIMan()->GetDialog("Win_GuildManage");
		pDlg->Show(true);
		pDlg->SetPosEx(GetPos().x, GetPos().y);
		if( pDlg->IsFirstOpen() || (int)pDlg->GetDlgItem("List_Family")->GetData() != GetHostPlayer()->GetFactionID() )
		{
			GetGame()->GetFactionMan()->RefreshMemList(0, 0, true);
			pDlg->SetFirstOpen(false);
		}
		else
			GetGame()->GetFactionMan()->RefreshMemList(0, 0, false);
		GetGameUIMan()->GetDialog("Win_GuildManage")->Show(true);
	}
}

void CDlgClassmateManage::OnCommand_GiveTask(const char * szCommand)
{
	GetGame()->GetTaskTemplateMan()->ManualTrigTask(GetHostPlayer()->GetTaskInterface(), s_idAwardTask[m_nSuitAwardIndex][CIRCLE_TASK_ID]);
	m_dwLastGiveTaskTime = GetTickCount();
}

void CDlgClassmateManage::OnCommand_ShowType(const char * szCommand)
{
	BuildClassMateList();
}

void CDlgClassmateManage::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	POINT ptPos = pObj->GetPos();
	int nCurSel = m_pList_Member->GetCurSel();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	
	if( nCurSel >= 0 && nCurSel < m_pList_Member->GetCount() &&
		m_pList_Member->GetHitArea(x, y) == AUILISTBOX_RECT_TEXT )
	{
		int idPlayer = m_pList_Member->GetItemData(nCurSel);
		
		x = GET_X_LPARAM(lParam) - p->X;
		y = GET_Y_LPARAM(lParam) - p->Y;
		GetGameUIMan()->PopupPlayerContextMenu(idPlayer, x, y);
	}
}

void CDlgClassmateManage::BuildList()
{
	ACString strText;
	int i, j;

	m_nPage = 0;

	BuildClassMateList();

	int iHostLevel = GetHostPlayer()->GetBasicProps().iLevel;
	m_nSuitAwardIndex = -1;
	for (i=0; i<9; i++)
	{
		if (iHostLevel>=s_idAwardTask[i][CIRCLE_TASK_LV_LOW] && iHostLevel<=s_idAwardTask[i][CIRCLE_TASK_LV_HIGH])
		{
			m_nSuitAwardIndex = i;
			break;
		}
	}
	if (m_nSuitAwardIndex == -1)
	{
		Show(false);
		return;
	}

	CECCircleMan* pCircleMan = GetGame()->GetCircleMan();

	CircleHistoryList &chl = pCircleMan->GetHisList();
	m_pList_History->ResetContent();
	ACString strOutTime;
	for ( i=0; i< (int)chl.size(); i++)
	{	
		const long joinTime = chl[i]->GetJoinTime();
		tm tmIn = *gmtime((const time_t*)&joinTime);
		const long gradTime = chl[i]->GetGradTime();
		tm tmOut = *gmtime((const time_t*)&gradTime);
		if (gradTime==0)
		{
			strOutTime = _AL(" ");
		}
		else
			strOutTime.Format(_AL("%d.%02d.%02d"), tmOut.tm_year+1900, tmOut.tm_mon+1, tmOut.tm_mday);

		strText.Format(_AL("%s\t%d.%02d.%02d-\t-%s"), chl[i]->GetName(), 
			tmIn.tm_year+1900, tmIn.tm_mon+1, tmIn.tm_mday, strOutTime);
		m_pList_History->AddString(strText);
	}

	m_pTxt_Message->SetText(_AL(""));
	CircleMemList &cml = pCircleMan->GetMemList();
	int t = GetGame()->GetServerGMTTime() - GetGame()->GetTimeZoneBias() * 60;
	//找出时间最近的10条信息
	const int numMsg = 10;
	int mesIndex[numMsg];
	for (i=0; i<numMsg; i++)
	{
		mesIndex[i] = -1;
	}
	bool bFinish = false;
	int num = 0;
	
	while(!bFinish)
	{
		int tMinTemp = 0;
		int bestIndex = -1;
		bool bOut = true;
		for (i=0; i<(int)cml.size(); i++)
		{
			for (j=0; j<numMsg; j++)
			{
				if (mesIndex[j]<0)
					break;
			}
			if (j==numMsg) //最多10条信息
			{
				break;
				bFinish = true;
			}
			// 加入时间 2*i
			for (j=0; j<numMsg; j++)
			{
				if (mesIndex[j]==i*2)
					break;
			}
			if (j==numMsg)
			{
				if (cml[i]->GetJoinTime()<t && tMinTemp<cml[i]->GetJoinTime())
				{
					tMinTemp = cml[i]->GetJoinTime();
					bestIndex = i*2;
				}
			}
			// 毕业时间 2*i+1
			if (cml[i]->GetGradTime()>0)
			{
				for (j=0; j<numMsg; j++)
				{
					if (mesIndex[j]==(i*2+1))
						break;
				}
				if (j==numMsg)
				{
					if (cml[i]->GetGradTime()<t && tMinTemp<cml[i]->GetGradTime())
					{
						tMinTemp = cml[i]->GetGradTime();
						bestIndex = i*2+1;
					}
				}
			}
		}
		if (bestIndex<0)
		{
			break;
		}
		for (j=0; j<numMsg; j++)
		{
			if (mesIndex[j]<0)
			{
				mesIndex[j] = bestIndex;
				break;
			}
		}
		if (bestIndex%2 == 0)
		{
			const long joinTime = cml[bestIndex/2]->GetJoinTime();
			tm tmIn = *(gmtime((const time_t*)&joinTime));
			strText.Format(GetGameUIMan()->GetStringFromTable(10230), tmIn.tm_mon+1, tmIn.tm_mday, tmIn.tm_hour, tmIn.tm_min, cml[bestIndex/2]->GetName());
		}
		else
		{
			const long gradTime = cml[bestIndex/2]->GetGradTime();
			tm tmOut = *(gmtime((const time_t*)&gradTime));
			strText.Format(GetGameUIMan()->GetStringFromTable(10231), tmOut.tm_mon+1, tmOut.tm_mday, tmOut.tm_hour, tmOut.tm_min, cml[bestIndex/2]->GetName());
		}
		m_pTxt_Message->AppendText(strText);
		m_pTxt_Message->AppendText(_AL("\r"));
		num++;
		if (num>numMsg)
		{
			GetGameUIMan()->MessageBox("", _AL("error"));
			break;
		}
	}

	if (pCircleMan->GetCircle(GetHostPlayer()->GetCircleID()))
		m_pTxt_ClassName->SetText(pCircleMan->GetCircle(GetHostPlayer()->GetCircleID())->GetName());

	const long createTime = pCircleMan->GetCreateTime() - GetGame()->GetTimeZoneBias() * 60;
	tm* tmCreate = gmtime((const time_t*)&createTime);
	if( tmCreate )
	{
		strText.Format(GetGameUIMan()->GetStringFromTable(8000),tmCreate->tm_year+1900,tmCreate->tm_mon+1,tmCreate->tm_mday,tmCreate->tm_hour,tmCreate->tm_min);
		m_pTxt_PplCap->SetText(strText);
	}

	strText.Format(_AL("%d/%d"), pCircleMan->GetOnlineNum(), pCircleMan->GetMemList().size());
	m_pTxt_PplOnline->SetText(strText);
	strText.Format(_AL("%d"), m_nGradNum);
	m_pTxt_PplGrad->SetText(strText);

	strText.Format(_AL("%d/%d"), pCircleMan->GetCirclePoint(), s_idAwardTask[m_nSuitAwardIndex][CIRCLE_TASK_POINT]);
	m_pLab_Point->SetText(strText);

	GetGameUIMan()->Int64ToStr(pCircleMan->GetGradBonus(), strText);
	m_pLab_AwardExp->SetText(strText);
}

void CDlgClassmateManage::BuildClassMateList()
{
	if (GetHostPlayer()->GetCircleID()==0)
	{
		Show(false);
		return;
	}
	ACString strText;
	int i;
	CircleMemList &cml = GetGame()->GetCircleMan()->GetMemList();
	//已毕业的放最后
	bool bExchanged;
	do
	{
		bExchanged = false;
		Circle_Mem_Info *pTemp;
		for( i = 0; i < (int)cml.size() - 1; i++ )
		{
			if(cml[i]->GetGradTime()!=0 && cml[i + 1]->GetGradTime()==0 )
			{
				pTemp = cml[i];
				cml[i] = cml[i + 1];
				cml[i + 1] = pTemp;
				bExchanged = true;
			}
		}
	}
	while( bExchanged );
	m_nGradNum = 0;
	for( i = 0; i < (int)cml.size(); i++ )
	{
		if(cml[i]->GetGradTime()!=0 )
		{
			m_nGradNum++;
		}
	}
	
	if( m_nSortMode != SORT_BY_ORDER )
	{
		bool bExchanged;
		Circle_Mem_Info *pTemp;
		
		do
		{
			bExchanged = false;
			for( i = 0; i < (int)cml.size() - 1; i++ )
			{
				if ((cml[i]->GetGradTime()==0 && cml[i + 1]->GetGradTime()==0) ||(cml[i]->GetGradTime()!=0 && cml[i + 1]->GetGradTime()!=0))
				{
					if( (m_nSortMode == SORT_BY_NAME &&
						a_stricmp(cml[i]->GetName(), cml[i + 1]->GetName()) > 0) ||
						(m_nSortMode == SORT_BY_LEVEL &&
						cml[i]->GetLev() < cml[i + 1]->GetLev()) ||
						(m_nSortMode == SORT_BY_GENDER &&
						cml[i]->GetGender() < cml[i + 1]->GetGender()) ||
						(m_nSortMode == SORT_BY_PROFESSION &&
						cml[i]->GetProf() > cml[i + 1]->GetProf()) ||
						(m_nSortMode == SORT_BY_TIMEIN &&
						cml[i]->GetJoinTime() > cml[i + 1]->GetJoinTime()) ||
						(m_nSortMode == SORT_BY_TIMEOUT &&
						cml[i]->GetGradTime() > cml[i + 1]->GetGradTime()) ||
						(m_nSortMode == SORT_BY_ONLINE &&
						cml[i]->GetLineId() < cml[i + 1]->GetLineId()) )
					{
						pTemp = cml[i];
						cml[i] = cml[i + 1];
						cml[i + 1] = pTemp;
						bExchanged = true;
					}
				}
			}
		}
		while( bExchanged );
	}

	m_pList_Member->ResetContent();
	int unGradIndex = 0;
	ACString strLine;
	ACString strTime;
	for( i = m_nPage*NUM_PER_PAGE; i < (int)cml.size(); i++ )
	{
		strLine = GetGameUIMan()->GetLine(cml[i]->GetLineId());
		const long joinTime = cml[i]->GetJoinTime();
		tm tmIn = *(gmtime((const time_t*)&joinTime));
		const long gradTime = cml[i]->GetGradTime();
		tm tmOut = *(gmtime((const time_t*)&gradTime));
		if (gradTime==0)
		{
			strTime = _AL("---");
			if (m_pCombo_Display->GetCurSel()==2)
				continue;
		}
		else
		{
			strTime.Format(_AL("%d.%d.%d"), tmOut.tm_year+1900, tmOut.tm_mon+1, tmOut.tm_mday);
			if (m_pCombo_Display->GetCurSel()==1)
				continue;
		}
		
		Circle_Info *cInfo = GetGame()->GetCircleMan()->GetCircle(GetHostPlayer()->GetCircleID(), true);
		bool bMaster = (cInfo && (cInfo->GetMasterID() == cml[i]->GetID()));
		EditBoxItemBase item(enumEIImage);
		item.SetImageIndex(0);
		item.SetImageFrame(0);
	
		if (gradTime==0)
		{
			strText.Format(_AL("%s\t%s\t%d\t%s\t%d.%02d.%02d\t%s\t%s"), (bMaster ? (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize() : _AL("    ")) +cml[i]->GetName(), 
				GetGameUIMan()->GetStringFromTable(7141+cml[i]->GetGender()), cml[i]->GetLev(), GetStringFromTable(2101 + cml[i]->GetProf()),
				tmIn.tm_year+1900, tmIn.tm_mon+1, tmIn.tm_mday, strTime, strLine);
			m_pList_Member->InsertString(unGradIndex, strText);
			m_pList_Member->SetItemData(unGradIndex, cml[i]->GetID());
			if (cml[i]->GetLineId()>=0)
			{
				m_pList_Member->SetItemTextColor(unGradIndex, A3DCOLORRGB(255, 255, 255));
			}
			else
				m_pList_Member->SetItemTextColor(unGradIndex, A3DCOLORRGB(128, 128, 128));
			unGradIndex++;
		}
		else
		{
			strText.Format(_AL("%s\t%s\t---\t---\t%d.%02d.%02d\t%s\t---"), (bMaster ? (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize() : _AL("    ")) +cml[i]->GetName(), 
				GetGameUIMan()->GetStringFromTable(7141+cml[i]->GetGender()), 
				tmIn.tm_year+1900, tmIn.tm_mon+1, tmIn.tm_mday, strTime);
			m_pList_Member->AddString(strText);
			m_pList_Member->SetItemData(m_pList_Member->GetCount()-1, cml[i]->GetID());
			m_pList_Member->SetItemTextColor(m_pList_Member->GetCount()-1, A3DCOLORRGB(255, 255, 0));
		}
		
		if (m_pList_Member->GetCount()>=NUM_PER_PAGE)
		{
			break;
		}
	}

	m_pBtn_Top->Enable(m_nPage>0);
	m_pBtn_Prec->Enable(m_nPage>0);
	m_pBtn_Next->Enable(cml.size()>0 && m_nPage<((int)cml.size()-1)/NUM_PER_PAGE);
	m_pBtn_Bottom->Enable(cml.size() && m_nPage<((int)cml.size()-1)/NUM_PER_PAGE);
}

void CDlgClassmateManage::SetLastRefreshTime(DWORD dwTime)
{
	m_dwLastRefreshTime = dwTime;
}