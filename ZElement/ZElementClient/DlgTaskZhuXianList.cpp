// Filename	: CDlgTaskZhuXianList.cpp
// Creator	: Fu Chonggang
// Date		: 2011/3/31

#include "AFI.h"
#include "ainifile.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUIVerticalText.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUICTranslate.h"
#include "AWScriptFile.h"

#include "DlgTaskZhuXianList.h"
#include "DlgTask.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_TaskInterface.h"
#include "EC_UIData.h"
#include "TaskTemplMan.h"
#include "LuaEvent.h"
#include "EC_HelpGraphGuide.h"
#include "EC_World.h"


#define new A_DEBUG_NEW

abase::vector<int> CDlgTaskZhuXianList::m_vecTaskSuit;

AUI_BEGIN_COMMAND_MAP(CDlgTaskZhuXianList, CDlgBase)

AUI_ON_COMMAND("Btn_Prev",		OnCommand_Left)
AUI_ON_COMMAND("Btn_Next",		OnCommand_Right)
AUI_ON_COMMAND("Rdo_Book*",		OnCommand_Page)
AUI_ON_COMMAND("taskdaily",		OnCommand_TaskDaily)
AUI_ON_COMMAND("rdo_type*",		OnCommand_RdoType)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Ancient",	OnCommand_Ancient)

AUI_END_COMMAND_MAP()

CDlgTaskZhuXianList::CDlgTaskZhuXianList()
{
}

CDlgTaskZhuXianList::~CDlgTaskZhuXianList()
{
}

bool CDlgTaskZhuXianList::OnInitDialog()
{
	AString strName;
	for (int i=0; i<TASK_PERPAGE; ++i)
	{
		strName.Format("Lab_Book%d", i+1);
		m_pVTxtName[i] = (PAUIVERTICALTEXT)GetDlgItem(strName);
		m_pVTxtName[i]->SetTransparent(true);
		m_pVTxtName[i]->SetAcceptMouseMessage(false);
		strName.Format("Rdo_Book%d", i+1);
		m_pRdoPage[i] = (PAUIRADIOBUTTON)GetDlgItem(strName);
	}
	m_pRdo3 = (PAUIRADIOBUTTON)GetDlgItem("Rdo_3");
	m_pGfxOpen = (PAUIIMAGEPICTURE)GetDlgItem("Gfx_1");

	m_iStartIndex = 0;
	m_iClickPageTime = 0;
	m_pAncient = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Ancient");	
	AddTabDialog(this);
	
	return true;
}

bool CDlgTaskZhuXianList::Release()
{
	AIniFile IniFile;
	char szIniFile[MAX_PATH];
	sprintf(szIniFile, "%s\\UserData\\Layout\\UIData%d.ini", af_GetBaseDir(), GetHostPlayer()->GetCharacterID());
	IniFile.Open(szIniFile);
	IniFile.WriteIntValue("task_interface", "start_index", m_iStartIndex);
	
	IniFile.Save(szIniFile);
	return CDlgBase::Release();
}

void CDlgTaskZhuXianList::OnShowDialog()
{
	CheckRadioButton(1, 4);
	CheckRadioButton(2, -1);
	UpdateDisplay();
}

void CDlgTaskZhuXianList::OnTick()
{
	if(NULL != m_pRdo3)
	   m_pRdo3->Enable(GetHostPlayer()->GetSrcServerID()!=0);
    if(NULL != m_pAncient)
	   m_pAncient->Enable(g_pGame->GetGameRun()->GetWorld()->GetInstanceID() == 44);
}

bool CDlgTaskZhuXianList::Render()
{
	bool bRet;
	if (m_iClickPageTime==0)
	{
		return CDlgTaskBase::Render();
	}
	else
	{
		bRet = m_pGfxOpen->Render();
		if ((int)GetTickCount()>(m_iClickPageTime+5))
		{
			Show(false);
			m_iClickPageTime = 0;
			GetGameUIMan()->CommonMessage("Win_Quest", "showpage", m_vecTaskSuit[m_iClickPage], 0);
			ChangeDialog(dynamic_cast<CDlgTask*>(GetGameUIMan()->GetDialog("Win_Quest")));
		}
	}
	return bRet;
}

int CDlgTaskZhuXianList::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="loadInfo")
	{	
		AIniFile IniFile;
		char szIniFile[MAX_PATH];
		sprintf(szIniFile, "%s\\UserData\\Layout\\UIData%d.ini", af_GetBaseDir(), GetHostPlayer()->GetCharacterID());
		if (IniFile.Open(szIniFile))
		{
			m_iStartIndex = IniFile.GetValueAsInt("task_interface", "start_index", 0);
		}
	}
	
	return true;
}

void CDlgTaskZhuXianList::OnCommand_Left(const char * szCommand)
{
	--m_iStartIndex;
	UpdateDisplay();
}

void CDlgTaskZhuXianList::OnCommand_Right(const char * szCommand)
{
	++m_iStartIndex;
	UpdateDisplay();
}

void CDlgTaskZhuXianList::OnCommand_Page(const char * szCommand)
{
	int index = atoi(szCommand+strlen("Rdo_Book"));
	m_iClickPage = m_iStartIndex+index-1;
	m_iClickPageTime = GetTickCount();
	GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_11_3_TASK_QINGYUN);
}

void CDlgTaskZhuXianList::OnCommand_TaskDaily(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_DailyPromp_Today");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgTaskZhuXianList::OnCommand_RdoType(const char * szCommand)
{
	int index = atoi(szCommand+strlen("rdo_type"));
	if(3==index)
	{
		ChangeDialog(dynamic_cast<CDlgTaskBase *>(GetGameUIMan()->GetDialog("Win_QuestKf")));
	}
	else
	{
		CDlgTask *pDlgTask = dynamic_cast<CDlgTask*>(GetGameUIMan()->GetDialog("Win_Quest"));
		pDlgTask->CheckRadioButton(1, GetCheckedRadioButton(1));
		ChangeDialog(pDlgTask);
	}
}

void CDlgTaskZhuXianList::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgTaskZhuXianList::OnCommand_Ancient(const char * szCommand)
{
	if(GetGameUIMan()->GetDialog("Win_Quest_Wanted")->IsShow())
		GetGameUIMan()->GetDialog("Win_Quest_Wanted")->Show(false);
	else 
		GetGameUIMan()->GetDialog("Win_Quest_Wanted")->Show(true);
}

void CDlgTaskZhuXianList::UpdateDisplay()
{
	UpdateTaskSuitable();
	GetDlgItem("Btn_Prev")->Show(m_iStartIndex>0);
	GetDlgItem("Btn_Next")->Show(m_vecTaskSuit.size()>UINT(m_iStartIndex+TASK_PERPAGE));
	int iLvHost = GetHostPlayer()->GetBasicProps().iLevel;
	bool bFinish = true;
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	CECTaskInterface *pTaskInterafce = GetHostPlayer()->GetTaskInterface();
	FinishedTaskList* pTaskFinished = static_cast<FinishedTaskList*>(pTaskInterafce->GetFinishedTaskList());
	UINT i;
	for (i=0; i<TASK_PERPAGE; ++i)
	{
		if (i>=m_vecTaskSuit.size())	//m_iStartIndex=0
		{
			for (; i<TASK_PERPAGE; ++i)
			{
				m_pVTxtName[i]->Show(false);
				m_pRdoPage[i]->Show(false);
			}
			break;
		}

		TaskZhuxian* pTaskZhuxian = GetUIData()->m_vecTaskZhuxian[m_vecTaskSuit[i+m_iStartIndex]];

		m_pVTxtName[i]->Show(true);
		m_pRdoPage[i]->Show(true);
		m_pVTxtName[i]->SetText(pTaskZhuxian->strName);
		m_pRdoPage[i]->Enable(iLvHost>=pTaskZhuxian->iLevel);
		m_pVTxtName[i]->SetHint(iLvHost>=pTaskZhuxian->iLevel ? _AL("") : GetGameUIMan()->GetStringFromTable(10503));
		m_pRdoPage[i]->SetHint(iLvHost>=pTaskZhuxian->iLevel ? _AL("") : GetGameUIMan()->GetStringFromTable(10503));

		abase::vector<int>& vecIds = pTaskZhuxian->vecIds;
		bFinish = true;
		for (UINT j=0; j<vecIds.size(); ++j)
		{
			if( pTaskFinished->SearchTask(vecIds[j]) != 0 )
			{
				bFinish = false;
				break;
			}
		}
		AUIOBJECT_SETPROPERTY p;
		if(bFinish)
			sprintf(p.fn, "Button\\QuestMain_bookdown.tga");
		else
			sprintf(p.fn, "Button\\questmain_bookdisable.tga");
		m_pRdoPage[i]->SetProperty("Normal Image File", &p);
	}
}

void CDlgTaskZhuXianList::UpdateTaskSuitable()
{
	m_vecTaskSuit.clear();
	int iProfRace = GetHostPlayer()->GetProfRace();
	for (UINT i=0; i<GetUIData()->m_vecTaskZhuxian.size(); ++i)
	{
		TaskZhuxian* pTaskZhuxian = GetUIData()->m_vecTaskZhuxian[i];
		if( pTaskZhuxian->iProfRace==PROFRACE_UNKNOWN	||
			pTaskZhuxian->iProfRace==iProfRace			||
			pTaskZhuxian->iProfRace==PROFRACE_HUMAN && (iProfRace<=PROFRACE_FENGXIANG) ||
			pTaskZhuxian->iProfRace==PROFRACE_DEITY && (iProfRace>=PROFRACE_JIULI && iProfRace<=PROFRACE_TAIHAO) ||
			pTaskZhuxian->iProfRace==PROFRACE_TIANMAI && (iProfRace>=PROFRACE_QIANJI) )
		{
			m_vecTaskSuit.push_back(i);
		}
	}
}
