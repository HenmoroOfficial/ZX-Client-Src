// Filename	: DlgTaskList.cpp
// Creator	: Xiao Zhou
// Date		: 2006/6/1

#include "AFI.h"
#include "AUI\\AUICTranslate.h"
#include "DlgTaskList.h"
#include "DlgTask.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_Global.h"
#include "EC_GameUIMan.h"
#include "EC_TaskInterface.h"
#include "TaskTemplMan.h"
#include "elementdataman.h"
#include "EC_IvtrItem.h"
#include "EC_GameSession.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgTaskList, CDlgBase)

AUI_ON_COMMAND("accept",	OnCommand_Accept)
AUI_ON_COMMAND("IDCANCEL",	OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

CDlgTaskList::CDlgTaskList()
{
}

CDlgTaskList::~CDlgTaskList()
{
}

bool CDlgTaskList::OnInitDialog()
{
	GetDlgItem("Quest_D")->SetAcceptMouseMessage(false);
	m_pTxt_Content = (PAUITEXTAREA)GetDlgItem("Txt_Content");
	m_pTxt_BaseAward = (PAUILABEL)GetDlgItem("Txt_BaseAward");
	m_pTxt_ItemAward = (PAUILABEL)GetDlgItem("Txt_ItemAward");
	m_pLst_Quest = (PAUILISTBOX)GetDlgItem("Lst_Quest");
	for(int i = 0; i < CDlgTaskBase::CDLGTASK_AWARDITEM_MAX ; i++ )
	{
		char szText[20];
		sprintf(szText, "Img_Item%d", i);
		m_pImg_Item[i] = (PAUIIMAGEPICTURE)GetDlgItem(szText);
	}

	m_idSelTask = -1;

	return true;
}

void CDlgTaskList::OnCommand_Accept(const char * szCommand)
{
	int nSel = m_pLst_Quest->GetCurSel();
	if( nSel < m_pLst_Quest->GetCount() )
	{
		NPC_ESSENCE* pEssence = GetGameUIMan()->m_pCurNPCEssence;
		if (pEssence)
		{
			DATA_TYPE DataType;
			const void* pSvr = g_pGame->GetElementDataMan()->get_data_ptr(pEssence->id_task_out_service, ID_SPACE_ESSENCE, DataType);

			if (pSvr && DataType == DT_NPC_TASK_OUT_SERVICE)
			{
				GetGameSession()->c2s_CmdNPCSevAcceptTask(m_pLst_Quest->GetItemData(nSel), static_cast<const NPC_TASK_OUT_SERVICE*>(pSvr)->id_task_set);
			}
		}
	}
}

void CDlgTaskList::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->EndNPCService();
}

void CDlgTaskList::OnShowDialog()
{
	NPC_ESSENCE *pCurNPCEssence = GetGameUIMan()->m_pCurNPCEssence;
	NPC_ESSENCE *pEssence = pCurNPCEssence;
	if( !pEssence ) return;

	int j, idTask;
	const void *pData;
	DATA_TYPE DataType;
	unsigned int a_idTask[512];
	int task_count;
	int idLastTask = 0, nLastTaskTime = 0, nFinishTime;
	elementdataman *pDataMan = GetGame()->GetElementDataMan();
	abase::hash_map<int, int> mapOut;
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();

	pData = pDataMan->get_data_ptr(pEssence->id_task_out_service, ID_SPACE_ESSENCE, DataType);

	if( DataType == DT_NPC_TASK_OUT_SERVICE )
	{
		NPC_TASK_OUT_SERVICE *pService = (NPC_TASK_OUT_SERVICE *)pData;

		if ( pService->id_task_set > 0 )
		{
			const unsigned short* pStorage = pTask->GetStorageTasks(pService->id_task_set);
			task_count = 0;

			if (pStorage)
			{
				for (int i = 0; i < SIZE_OF_ARRAY(pService->id_tasks); i++)
				{
					int id = pService->id_tasks[i];

					if (id == 0)
						continue;

					for (int j = 0; j < TASK_STORAGE_LEN; j++)
					{
						if (pStorage[j] == id)
						{
							a_idTask[task_count] = id;
							task_count++;
						}
					}
				}
			}
		}

		for( j = 0; j < task_count; j++ )
		{
			idTask = a_idTask[j];
			if( idTask <= 0 ) continue;

			mapOut[idTask] = pEssence->id_task_out_service;

			nFinishTime = (int)pTask->GetTaskFinishedTime(idTask);
			if( nFinishTime > nLastTaskTime )
			{
				idLastTask = idTask;
				nLastTaskTime = nFinishTime;
			}
		}
	}

	A3DCOLOR color;
	ACString strText;
	ATaskTempl *pTemp, *pTempRoot;
	int nIndex, nNumTasks = 0;
	abase::hash_map<int, int>::iterator it;
	int nHostLevel = GetHostPlayer()->GetBasicProps().iLevel;
	ATaskTemplMan *pTempMan = GetGame()->GetTaskTemplateMan();

	m_pLst_Quest->ResetContent();
	for( it = mapOut.begin(); it != mapOut.end(); ++it )
	{
		idTask = it->first;

		color = 0;
		pTemp = NULL;
		color = m_pLst_Quest->GetColor();
		if( !pTask->HasTask(idTask) && pTask->CanShowTask(idTask) )
		{
			pTemp = pTempMan->GetTaskTemplByID(idTask);

			if( 0 != pTask->CanDeliverTask(idTask) )
				color = A3DCOLORRGB(128, 128, 128);
		}
		
		if( pTemp )
		{
			strText = GetStringFromTable(249);
			pTempRoot = (ATaskTempl *)pTemp->GetTopTask();
			if( pTemp != pTempRoot )
			{
				strText += pTempRoot->GetName();
				strText += _AL(" - ");
			}
			strText += pTemp->GetName();
			m_pLst_Quest->AddString(strText);
			nIndex = m_pLst_Quest->GetCount() - 1;
			m_pLst_Quest->SetItemData(nIndex, idTask);		// Task ID.
			
			int nTaskDisplayType = pTemp->GetDisplayType();
			m_pLst_Quest->SetItemTextColor(nIndex, TaskNewColor[nTaskDisplayType]);

			if (m_idSelTask==idTask)
				m_pLst_Quest->SetCurSel(m_pLst_Quest->GetCount()-1);

			nNumTasks++;
		}
	}
}

void CDlgTaskList::OnTick()
{
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	int nSel = m_pLst_Quest->GetCurSel();
	if( nSel >= m_pLst_Quest->GetCount() )
	{
		m_pTxt_BaseAward->Show(false);
		m_pTxt_ItemAward->Show(false);
		for( int j = 0; j < CDlgTaskBase::CDLGTASK_AWARDITEM_MAX; j++ )
			m_pImg_Item[j]->Show(false);
		m_pTxt_Content->SetText(_AL(""));
		for( int i = 0; ; i++ )
		{
			char szName[20];
			sprintf(szName, "Rank_%02d", i + 1);
			PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			if( !pImage )
				break;
			pImage->Show(false);
		}
		return;
	}

	int idTask = m_pLst_Quest->GetItemData(nSel);
	m_idSelTask = idTask;
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);
	if( pTemp )
	{
		m_pTxt_Content->SetText(GetGameUIMan()->m_pDlgTask->FormatTaskText(pTemp->GetDescription()));

		int nRank = pTemp->GetRank();
		for( int i = 0; ; i++ )
		{
			char szName[20];
			sprintf(szName, "Rank_%02d", i + 1);
			PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
			if( !pImage )
				break;
			pImage->Show(true);
			if( i < nRank )
				pImage->FixFrame(0);
			else
				pImage->FixFrame(1);
		}
	}

	m_pTxt_BaseAward->Show(false);
	m_pTxt_ItemAward->Show(false);
	for( int i = 0; i < CDlgTaskBase::CDLGTASK_AWARDITEM_MAX; i++ )
		m_pImg_Item[i]->Show(false);

	Task_Award_Preview award;
	pTask->GetTaskAwardPreview(idTask, &award);
	ACString strText, strTemp;
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
	if( award.m_ulSP > 0 )
	{
		strTemp.Format(GetStringFromTable(3203), award.m_ulSP);
		strText += strTemp;
	}
	if( strText != _AL("") )
	{
		m_pTxt_BaseAward->SetText(strText);
		m_pTxt_BaseAward->Show(true);
	}
	if( award.m_bHasItem )
	{
		if( !award.m_bItemKnown )
		{
			if( m_pTxt_BaseAward->IsShow() )
			{
				m_pTxt_ItemAward->SetText(GetStringFromTable(3204));
				m_pTxt_ItemAward->Show(true);
			}
			else
			{
				m_pTxt_BaseAward->SetText(GetStringFromTable(3204));
				m_pTxt_BaseAward->Show(true);
			}
		}
		else
		{
			for( DWORD i = 0; i < CDlgTaskBase::CDLGTASK_AWARDITEM_MAX ; i++ )
			{
				if( i < award.m_ulItemTypes )
				{
					CECIvtrItem *pItem = CECIvtrItem::CreateItem(award.m_ItemsId[i], 0, award.m_ItemsNum[i]);
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
					strText.Format(_AL("%d"), award.m_ItemsNum[i]);
					m_pImg_Item[i]->SetText(strText);
					m_pImg_Item[i]->Show(true);
				}
			}
		}
	}
}