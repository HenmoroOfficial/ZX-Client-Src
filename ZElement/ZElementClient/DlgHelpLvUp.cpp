// Filename	:	DlgHelpLvUp.cpp
// Creator	:	Fu Chonggang
// Date		:	Aug 31, 2009

#include "DlgHelpLvUp.h"
#include "DlgTaskDaily.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIEditBox.h"

#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "TaskTemplMan.h"
#include "EC_Game.h"
#include "luastate.h"
#include "LuaScript.h"
#include "LuaUtil.h"
#include "ScriptValue.h"
#include <vector.h>
#include "LuaEvent.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgHelpLvUp, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Secretary",			OnCommand_Secretary)
AUI_ON_COMMAND("Rdo_*",					OnCommand_RdoType)
AUI_ON_COMMAND("Btn_Close",				OnCommand_close)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgHelpLvUp, CDlgBase)


AUI_END_EVENT_MAP()


#define  HELP_LEVELUP_INFO    "LevelUpHelp"
const char szLevelUpHelpFile[] = "Interfaces\\script\\LevelUpHelp.lua";

CDlgHelpLvUp::CDlgHelpLvUp()
{
}

CDlgHelpLvUp::~CDlgHelpLvUp()
{
}

bool CDlgHelpLvUp::Release()
{
	if (m_pState)
	{
		m_pState->ReleaseTbl(HELP_LEVELUP_INFO);
		m_pState->UnRegister(szLevelUpHelpFile);
	}
	
	return CDlgBase::Release();
}

bool CDlgHelpLvUp::OnInitDialog()
{
	m_pTxt_LvUp= (AUITextArea *)GetDlgItem("Txt_LvUp");

	m_pState = g_LuaStateMan.GetAIState();
	if (!m_pState)
	{
		a_LogOutput(1, "CDlgHelpLvUp::InitDialog, null lua state\n");
		return false;
	}
	CLuaScript * pScript = m_pState->RegisterFile(szLevelUpHelpFile);
	if (!pScript)
	{
		return false;
	}

	return true;
}

void CDlgHelpLvUp::OnShowDialog()
{
	m_iLevel = GetHostPlayer()->GetBasicProps().iLevel;
	if (GetCheckedRadioButton(1)<=0)
	{
		CheckRadioButton(1,1);
	}
	UpdateInfo();
}

void CDlgHelpLvUp::OnTick()
{
	if (m_iLevel != GetHostPlayer()->GetBasicProps().iLevel)
	{
		UpdateInfo();
	}
}

bool CDlgHelpLvUp::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

int CDlgHelpLvUp::OnChangeUIMan(BYTE bUserChanged, BYTE bRoleChanged, BYTE bLineChanged)
{
	if (IsShow())
		Show(false);
	return 1;
}

void CDlgHelpLvUp::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgHelpLvUp::OnCommand_Secretary(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Help_Fairy");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgHelpLvUp::OnCommand_RdoType(const char * szCommand)
{
	UpdateInfo();
}

void CDlgHelpLvUp::OnCommand_close(const char * szCommand)
{
	Show(false);
}

void CDlgHelpLvUp::UpdateInfo()
{
	m_pTxt_LvUp->SetText(_AL(""));
	int iType = GetCheckedRadioButton(1);
	ACString strText;
	UINT i, j;

	abase::vector<CScriptValue> args, results;
	args.push_back((double)m_iLevel);
	switch (iType)
	{
	case 1:	// 任务
		{
			ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
			CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
			TaskTemplLst ttl;
			ATaskTempl *pTemp;
			pMan->GetAvailableTasks((TaskInterface*)pTask, ttl);
			UINT iSize = ttl.size();
			UINT iNum = 0;
			int taskIndex[5];			// 5个任务的index
			// 诛仙任务
			for( i = 0; i < iSize; i++ )
			{
				int id = ttl[i]->GetID();
				pTemp = pMan->GetTaskTemplByID(id);
				if( !pTemp ) continue;
				if( pTemp->IsKeyTask() )
				{
					strText += pTemp->GetName();
					taskIndex[iNum] = i;
					if (++iNum>=5)
						break;
					strText += _AL("\r");
				}
			}
			// 其它任务补全5个
			if (iNum>iSize)
			{
				for (;iNum<5;iNum++)
				{
					int curIndex;
					bool bUsed = false;
					do
					{
						bUsed = false;
						curIndex = rand()%iSize;
						for (j=0; j<iNum; j++)
						{
							if (taskIndex[j]==curIndex)
								bUsed = true;
						}
					}
					while(bUsed);
					
					int id = ttl[curIndex]->GetID();
					pTemp = pMan->GetTaskTemplByID(id);
					if( !pTemp ) continue;
					strText += pTemp->GetName();
					taskIndex[iNum] = curIndex;
					if (iNum>=4)
						break;
					strText += _AL("\r");
				}
			}
			break;
		}
	case 2: // 活动
		{
			abase::vector<TaskDailyInfo>* pAllTaskDaily = CDlgTaskDaily::GetAllTaskdaily();
			abase::vector<TaskDailyInfo> curLevel;
			abase::vector<TaskDailyInfo> allLevel;
			UINT iSize = pAllTaskDaily->size();
			// 适合当前级别活动
			for (i=0; i<iSize; i++)
			{
				TaskDailyInfo info = (*pAllTaskDaily)[i];
				if (info.ulMinLevel==1 && info.ulMaxLevel>=150)
					allLevel.push_back(info);
				else if (info.ulMinLevel<=(UINT)m_iLevel && info.ulMaxLevel>=(UINT)m_iLevel)
					curLevel.push_back(info);
			}
			iSize = curLevel.size();
			for (i=0; i<iSize; i++)
			{
				TaskDailyInfo info = curLevel[i];
				strText += info.strName;
				strText += _AL("\r");
			}
			// 不足5个补全(1-150)
			if (iSize<5)
			{
			}
			break;
		}
	case 3:	// 怪物
		{
			m_pState->Call(HELP_LEVELUP_INFO, "GetMonster", args, results);
			break;
		}
	case 4:	// 地图
		{
			m_pState->Call(HELP_LEVELUP_INFO, "GetMap", args, results);
			break;
		}
	case 5:	// BOSS
		{
			m_pState->Call(HELP_LEVELUP_INFO, "GetBoss", args, results);
			break;
		}
	}

	if (results.size() == 1 && results[0].m_Type == CScriptValue::SVT_STRING)
	{
		results[0].RetrieveAWString(strText);
	}
	m_pTxt_LvUp->SetText(strText);

}

#undef  HELP_LEVELUP_INFO