#include "AFI.h"
#include "DlgWish.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "EC_TaskInterface.h"
#include "TaskTemplMan.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgWish, CDlgBase)

AUI_ON_COMMAND("Btn_Wish",					OnCommand_wish)

AUI_END_COMMAND_MAP()

CDlgWish::CDlgWish()
{
}

CDlgWish::~CDlgWish()
{
	m_vecSearchWord.clear();
}

void CDlgWish::OnCommand_wish(const char * szCommand)
{
	ACString strInfo = m_pEdt->GetText();

	for(int i = 0; i < m_vecSearchWord.size(); i++)
	{
		if(strInfo.Find(m_vecSearchWord[i]) >= 0)
		{
			if(i >= 0 && i <= 4)
			{
				GetGame()->GetTaskTemplateMan()->ManualTrigTask(GetHostPlayer()->GetTaskInterface(), 29593);
				Show(false);
				return;
			}
			else if(i >= 5 && i <= 9)
			{
				GetGame()->GetTaskTemplateMan()->ManualTrigTask(GetHostPlayer()->GetTaskInterface(), 29594);
				Show(false);
				return;
			}
			else if(i >= 10 && i <= 15)
			{
				GetGame()->GetTaskTemplateMan()->ManualTrigTask(GetHostPlayer()->GetTaskInterface(), 29595);
				Show(false);
				return;
			}
			else if(i >=16 && i <= 23)
			{
				GetGame()->GetTaskTemplateMan()->ManualTrigTask(GetHostPlayer()->GetTaskInterface(), 29596);
				Show(false);
				return;
			}
		}
	}

	GetGame()->GetTaskTemplateMan()->ManualTrigTask(GetHostPlayer()->GetTaskInterface(), 29597);
	Show(false);
}

bool CDlgWish::OnInitDialog()
{
	m_pEdt = (PAUIEDITBOX)GetDlgItem("Edt_Word");
	m_vecSearchWord.clear();
	
	int nCount = 0;
	ACString strText;
	CECStringTab KeyWordTab;
	if( KeyWordTab.Init("configs\\wish_word.txt", true) )
	{
		while(KeyWordTab.GetWideString(nCount))
		{
			strText = KeyWordTab.GetWideString(nCount); 
			m_vecSearchWord.push_back(strText);
			nCount++;
		}
	}

	return true;
}

void CDlgWish::OnShowDialog()
{
	m_pEdt->SetText(_AL(""));
}