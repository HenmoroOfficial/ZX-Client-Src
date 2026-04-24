// Filename	: DlgDungeonStep.cpp
// Creator	: Li Bin
// Date		: 2013/08/14

#include "AArray.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUIImagePicture.h"
#include "AFI.h"
#include "AFileImage.h"
#include "DlgDungeonStep.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"
#include "EC_ManPlayer.h"
#include "EC_World.h"
#include "EC_BattleDungeon.h"
//#include "time.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgDungeonStep, CDlgBase)
AUI_ON_COMMAND("Btn_Maximize",	OnCommandMaximize)
AUI_ON_COMMAND("Btn_Minimize",	OnCommandMinimize)
AUI_ON_COMMAND("Btn_Detail",	OnCommandDetail)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgDungeonStep, CDlgBase)
AUI_ON_EVENT(NULL,	WM_KEYDOWN, OnEventKeyDown)
AUI_END_EVENT_MAP()

CDlgDungeonStep::CDlgDungeonStep()
{
	mMinLabStep = NULL;
	memset(&mMaxLabSteps, 0, sizeof(PAUILABEL) * MAX_STEP_NUM);
}

CDlgDungeonStep::~CDlgDungeonStep()
{
}

void CDlgDungeonStep::OnCommandMaximize(const char* szCommand)
{
	PAUIDIALOG maxDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Step_Max");
	PAUIDIALOG minDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Step_Min");

	maxDlg->Show(true);
	maxDlg->AlignTo(minDlg, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);

	minDlg->Show(false);
}

void CDlgDungeonStep::OnCommandMinimize(const char* szCommand)
{
	PAUIDIALOG maxDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Step_Max");
	PAUIDIALOG minDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Step_Min");

	minDlg->Show(true);
	minDlg->AlignTo(maxDlg, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);

	maxDlg->Show(false);
}

void CDlgDungeonStep::OnCommandDetail(const char* szCommand)
{
	CECBattleDungeon* dungeon = CECBattleDungeonMan::getSingletonPtr()->GetCurBattle();
	if (dungeon == NULL)
		return;

	const AArray<CECGame::DUNGEONSTEPINFO>* stepInfo = g_pGame->GetDungeonStepInfo();
	
	PAUIDIALOG pDlgDetail = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Step_Detail");
	PAUIDIALOG pBtnDetailMax = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Step_Max");
	PAUIDIALOG pBtnDetailMin = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_Step_Min");

	// Hide the hint in two dialogs.
	pBtnDetailMax->GetDlgItem("Txt_01")->Show(false);
	pBtnDetailMax->GetDlgItem("Img_01")->Show(false);
	pBtnDetailMax->GetDlgItem("Shape01")->Show(false);

	pBtnDetailMin->GetDlgItem("Txt_01")->Show(false);
	pBtnDetailMin->GetDlgItem("Img_01")->Show(false);
	pBtnDetailMin->GetDlgItem("Shape01")->Show(false);

	pDlgDetail->Show(true);

	for (int i = 0; i < stepInfo->GetSize(); ++ i )
	{
		PAUIOBJECT subDlg = pDlgDetail->GetDlgItem((*stepInfo)[i].szSubWndName);
		if (subDlg == NULL)
			continue;

		subDlg->Show((*stepInfo)[i].iDungeonID == dungeon->m_iId);
	}
}

void CDlgDungeonStep::OnEventKeyDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if(int(wParam) != VK_ESCAPE)
		return;
	
	if(0 == strcmpi(GetName(), "Win_Battle_Dungeon_Step_Max") )
		OnCommandMinimize("");
}

void CDlgDungeonStep::UpdateDungeonProgress(S2C::dungeon_progress::step_type steps[], DWORD count)
{
	CECBattleDungeon* dungeon = CECBattleDungeonMan::getSingletonPtr()->GetCurBattle();
	if (dungeon == NULL)
		return;

	const CECGame::DUNGEONSTEPINFO* stepInfo = NULL;
	AArray<CECGame::DUNGEONSTEPINFO>* stepInfos = g_pGame->GetDungeonStepInfo();
	for (int i = 0; i < stepInfos->GetSize(); ++ i)
	{
		if( (*stepInfos)[i].iDungeonID == dungeon->m_iId)
		{
			stepInfo = (*stepInfos).ElementAt(i);
			break;
		}
	}

	if ( stepInfo == NULL )
		return;

	ASSERT(count == stepInfo->iStepNum);

	if (strcmp(GetName(), "Win_Battle_Dungeon_Step_Min") == 0)
	{
		DWORD curStringID = stepInfo->iStartMsgID;
		for (DWORD i = 0; i < count && i < (DWORD) stepInfo->iStepNum; ++ i)
		{
			if (i != stepInfo->iStepNum-1 && steps[i].is_pass)
			{
				if (steps[i].finish_challenge == 0)
					curStringID += 2;
				else
					curStringID += 4;

				continue;
			}

			DWORD stringID;
			if (steps[i].finish_challenge == 0)
			{
				stringID = steps[i].is_pass ? curStringID + 1 : curStringID;
			}
			else
			{
				stringID = steps[i].is_pass ? curStringID + 2 : curStringID;
				if (steps[i].finish_challenge == 2)
					stringID ++;
			}

			ACString txt;
			txt.Format(GetStringFromTable(stringID), steps[i].cur_count, steps[i].max_count);

			mMinLabStep->SetText(txt);
			break;
		}

		return;
	}

	DWORD i, curStringID = stepInfo->iStartMsgID;
	for (i = 0; i < (DWORD) stepInfo->iStepNum; ++ i)
	{
		DWORD stringID;
		if (steps[i].finish_challenge == 0)
		{
			stringID = steps[i].is_pass ? curStringID + 1 : curStringID;
			curStringID += 2;
		}
		else
		{
			stringID = steps[i].is_pass ? curStringID + 2 : curStringID;
			if (steps[i].finish_challenge == 2)
				stringID ++;

			curStringID += 4;
		}

		ACString txt;
		txt.Format(GetStringFromTable(stringID), steps[i].cur_count, steps[i].max_count);

		mMaxLabSteps[i]->SetText(txt);
		mMaxLabSteps[i]->Show(true);
	}

	for (;i < MAX_STEP_NUM; ++ i)
		mMaxLabSteps[i]->Show(false);
}

bool CDlgDungeonStep::OnInitDialog()
{
	if (strcmp(GetName(), "Win_Battle_Dungeon_Step_Max") == 0)
	{
		for (int i = 0; i < MAX_STEP_NUM; ++ i)
		{
			AString ctrlName;
			ctrlName.Format("Lab_Step%02d", i + 1);
			DDX_Control(ctrlName, mMaxLabSteps[i]);
		}
	}
	else if (strcmp(GetName(), "Win_Battle_Dungeon_Step_Min") == 0)
	{
		DDX_Control("Lab_Step", mMinLabStep);
	}

	m_dwDungeonProg = -1;
	SetData(0);

	return true;
}