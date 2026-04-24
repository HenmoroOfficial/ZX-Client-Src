// Filename	:	DlgStorageTaskCross.cpp
// Creator	:	Fu Chonggang
// Date		:	Feb 25, 2011

#include "AFI.h"
#include "DlgStorageTaskCross.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUICTranslate.h"
#include "DlgTask.h"
#include "DlgMiniMap.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_TaskInterface.h"
#include "EC_IvtrItem.h"
#include "EC_Instance.h"
#include "EC_AutoMove.h"
#include "EC_World.h"
#include "EC_Model.h"
#include "EC_GameUIMisc.h"
#include "TaskTemplMan.h"
#include "elementdataman.h"

#include "A2DSprite.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgStorageTaskCross, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",					OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Accept",				OnCommand_Accept)
AUI_ON_COMMAND("taskdaily",					OnCommand_TaskDaily)
AUI_ON_COMMAND("Btn_Abandon",				OnCommand_Abandon)
AUI_ON_COMMAND("Combo_KfMap",				OnCommand_ChangeMap)
AUI_ON_COMMAND("rdo*",						OnCommand_RdoType)
AUI_ON_COMMAND("Btn_Ancient",	            OnCommand_Ancient)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgStorageTaskCross, CDlgBase)

AUI_ON_EVENT("Lst_KfQuest",		WM_LBUTTONUP,		OnEventLButtonDownStorageTask)
AUI_ON_EVENT("Txt_Content",		WM_LBUTTONDOWN,		OnEventLButtonDown_Txt_QuestItem)
AUI_ON_EVENT("Txt_Content",		WM_MOUSEMOVE,		OnEventMouseMove_Txt_QuestItem)
AUI_ON_EVENT("Txt_QuestItem",	WM_LBUTTONDOWN,		OnEventLButtonDown_Txt_QuestItem)
AUI_ON_EVENT("Txt_QuestItem",	WM_MOUSEMOVE,		OnEventMouseMove_Txt_QuestItem)

AUI_END_EVENT_MAP()

const int map2StorageTaskIndex[] = {3, 4, 5, 6, 7, 8, 1, 2};

const int CDlgStorageTaskCross::ITEM_ID = 42929;
const int CDlgStorageTaskCross::CROSS_MAP_NUM = 1;

extern ACString g_AutoMoveDestName;
CDlgStorageTaskCross::CDlgStorageTaskCross()
{
}

CDlgStorageTaskCross::~CDlgStorageTaskCross()
{
}

bool CDlgStorageTaskCross::OnInitDialog()
{
	m_pTxt_Content = (AUITextArea *)GetDlgItem("Txt_Content");
	m_pTxt_QuestItem = (AUITextArea *)GetDlgItem("Txt_QuestItem");
	m_pTxt_BaseAward = (AUILabel *)GetDlgItem("Txt_BaseAward");
	m_pTxt_ItemAward = (AUILabel *)GetDlgItem("Txt_ItemAward");
	
	int i;
	for(i = 0; i < CDLGTASK_AWARDITEM_MAX ; i++ )
	{
		char szText[20];
		sprintf(szText, "Img_Item%d", i);
		m_pImg_Item[i] = (PAUIIMAGEPICTURE)GetDlgItem(szText);
	}
	m_pBtn_Accept = (AUIStillImageButton *)GetDlgItem("Btn_Accept");
	m_pCombo_KfMap = (AUIComboBox *)GetDlgItem("Combo_KfMap");
	for (i=0; i<CROSS_MAP_NUM; ++i)
	{
		m_pCombo_KfMap->AddString(GetStringFromTable(3150 + i));
	}
	m_pCombo_KfMap->SetCurSel(0);
	m_pTxt_KfItem = (AUILabel *)GetDlgItem("Txt_KfItem");
	m_pLst_KfQuest = (AUIListBox *)GetDlgItem("Lst_KfQuest");
	m_pTv_KfQuestGot = (AUITreeView *)GetDlgItem("Tv_KfQuestGot");
	m_pBtn_Abandon = (AUIStillImageButton *)GetDlgItem("Btn_Abandon");
	
	m_pTxt_QuestItem->SetImageList(GetGameUIMan()->GetControlImgLst());
	
	m_idLastTask = -2;
	m_idSelTask = 0;
	m_pAncient = (AUIStillImageButton *)GetDlgItem("Btn_Ancient");	
	//AddTabDialog(this);
	return true;
}

void CDlgStorageTaskCross::OnShowDialog()
{
	CheckRadioButton(1, 3);
	UpdateTask();
	OnCommand_ChangeMap("");
	GetGameUIMan()->GetDialog("Win_Quest")->Show(false);
}

void CDlgStorageTaskCross::OnTick()
{
	P_AUITREEVIEW_ITEM pItem = m_pTv_KfQuestGot->GetSelectedItem();
	bool bAccept = m_pLst_KfQuest->GetCurSel()>=0 && m_pLst_KfQuest->GetCurSel()<m_pLst_KfQuest->GetCount();
	m_pBtn_Accept->Enable(bAccept);
	if( pItem )
	{
		int idTask = m_pTv_KfQuestGot->GetItemData(pItem);
		UpdateTask(idTask);
	}
	m_pBtn_Abandon->Enable(pItem!=NULL);

	ACString strText;
	int numItem = GetHostPlayer()->GetPack()->GetItemTotalNum(ITEM_ID);
	strText.Format(_AL("%d"), numItem);
	m_pTxt_KfItem->SetText(strText);
	if(NULL != m_pAncient)
	   m_pAncient->Enable(g_pGame->GetGameRun()->GetWorld()->GetInstanceID() == 44);
}

bool CDlgStorageTaskCross::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

int CDlgStorageTaskCross::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	return 1;
}

void CDlgStorageTaskCross::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgStorageTaskCross::OnCommand_Accept(const char * szCommand)
{
	int iCurSel = m_pLst_KfQuest->GetCurSel();
	if (iCurSel<0||iCurSel>=m_pLst_KfQuest->GetCount())
		return;

	int numItem = GetHostPlayer()->GetPack()->GetItemTotalNum(ITEM_ID);
	int idTask = m_pLst_KfQuest->GetItemData(iCurSel);
	elementdataman *pDataMan = GetGame()->GetElementDataMan();
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);
	if (pTemp && pTemp->m_ulPremItems>0 && numItem<(int)pTemp->m_PremItems[0].m_ulItemNum)
	{
		ACString strText;
		strText.Format(GetGameUIMan()->GetStringFromTable(5314), pTemp->m_PremItems[0].m_ulItemNum-numItem);
		GetGameUIMan()->MessageBox("", strText);
		return;
	}

	GetGame()->GetTaskTemplateMan()->ManualTrigStorageTask(GetHostPlayer()->GetTaskInterface(), m_pLst_KfQuest->GetItemData(iCurSel), map2StorageTaskIndex[m_pCombo_KfMap->GetCurSel()]);
}

void CDlgStorageTaskCross::OnCommand_TaskDaily(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_DailyPromp_Today");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgStorageTaskCross::OnCommand_Abandon(const char * szCommand)
{
	P_AUITREEVIEW_ITEM pItem = m_pTv_KfQuestGot->GetSelectedItem();
	
	if(pItem)
	{
		PAUIDIALOG pMsgBox;
		
		m_pAUIManager->MessageBox("Game_AbortTask", m_pAUIManager->GetStringFromTable(236),
			MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(m_pTv_KfQuestGot->GetItemData(pItem));
	}
}

void CDlgStorageTaskCross::OnCommand_ChangeMap(const char * szCommand)
{
	int iCurSel = m_pCombo_KfMap->GetCurSel();
	if (iCurSel<0||iCurSel>=m_pCombo_KfMap->GetCount())
		return;

	int iSTIndex = map2StorageTaskIndex[iCurSel];
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	const unsigned short* pStorage = pTask->GetStorageTasks(iSTIndex);
	int j, idTask;
	int nIndex;
	ACString strText;
	ATaskTempl *pTemp, *pTempRoot;
	ATaskTemplMan *pTempMan = GetGame()->GetTaskTemplateMan();
	m_pLst_KfQuest->ResetContent();
	for (j = 0; j < TASK_STORAGE_LEN; j++)
	{
		if (pStorage[j]>0 )
		{
			idTask = pStorage[j];
			
			int color = 0;
			pTemp = NULL;
			color = m_pLst_KfQuest->GetColor();
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
				m_pLst_KfQuest->AddString(strText);
				nIndex = m_pLst_KfQuest->GetCount() - 1;
				m_pLst_KfQuest->SetItemData(nIndex, idTask);		// Task ID.
				int nType = 0;
				if( !pTemp->IsKeyTask() )
					nType = pTemp->GetType() + 1;
				m_pLst_KfQuest->SetItemTextColor(nIndex, TaskColor[nType]);
			}

		}
	}
	if (m_pLst_KfQuest->GetCount()>0)
	{
		m_pLst_KfQuest->SetCurSel(0);
		OnEventLButtonDownStorageTask(0, 0, m_pLst_KfQuest);
	}
	else
		m_pTxt_Content->SetText(GetGameUIMan()->GetStringFromTable(8601));
}

void CDlgStorageTaskCross::OnEventLButtonDownStorageTask(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	m_pTv_KfQuestGot->SelectItem(NULL);
	m_idSelTask = 0;
	m_idLastTask = 0;

	m_pTxt_BaseAward->SetText(_AL(""));
	m_pTxt_ItemAward->SetText(_AL(""));
	for( int j = 0; j < CDLGTASK_AWARDITEM_MAX; j++ )
		m_pImg_Item[j]->Show(false);
	m_pTxt_Content->SetText(_AL(""));
	m_pTxt_QuestItem->SetText(_AL(""));

	if (m_pLst_KfQuest->GetCurSel()<0||m_pLst_KfQuest->GetCurSel()>=m_pLst_KfQuest->GetCount())
	{
		return;
	}
	
	int idTask = m_pLst_KfQuest->GetItemData(m_pLst_KfQuest->GetCurSel());
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);
	if( pTemp )
	{
		m_pTxt_Content->SetText(FormatTaskText(pTemp->GetDescription()));
	}
}

void CDlgStorageTaskCross::OnEventLButtonDown_Txt_QuestItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	
	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	CDlgTask::GetItemLinkItemOn(x, y, pObj, &Item);
	
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
				CDlgMiniMap* pDlgMiniMap = (CDlgMiniMap*)GetGameUIMan()->GetDialog("Win_MiniMap");
				pDlgMiniMap->m_dwTempTargetTime = GetTickCount();
				pDlgMiniMap->m_vecTempTarget = vec;
				pDlgMiniMap->m_idTempTargetInstance = idInstance;
				
				g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, idInstance);
				g_AutoMoveDestName = Item.m_pItem->GetName();
				
				if( idInstance == GetWorld()->GetInstanceID() && GetHostPlayer()->GetModelTempArrow() )
					GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
			}
		}
		else if( Item.m_pItem->GetType() == enumEIImage )
		{
			int idTask;
			int idNpc;
			if(a_strstr(Item.m_pItem->GetInfo(), _AL("fly")))
			{
				a_sscanf(Item.m_pItem->GetInfo(), _AL("fly%d %d"), &idTask, &idNpc);
				GetGameUIMan()->CheckCanTaskFly(idTask,idNpc);
			}
			else if(a_strstr(Item.m_pItem->GetInfo(), _AL("reachsite")))
			{
				a_sscanf(Item.m_pItem->GetInfo(), _AL("reachsite%d"), &idTask);
				GetGameUIMan()->CheckTaskReachSiteFly(idTask);
			}
		}
	}
}

void CDlgStorageTaskCross::OnEventMouseMove_Txt_QuestItem(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	AUITEXTAREA_EDITBOX_ITEM Item;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	pObj->SetHint(_AL(""));
	CDlgTask::GetItemLinkItemOn(x, y, pObj, &Item);
}

void CDlgStorageTaskCross::OnCommand_RdoType(const char * szCommand)
{
	int index = atoi(szCommand+3);
	if (index==1||index==2)
	{
		ChangeDialog(GetGameUIMan()->m_pDlgTask);
		GetGameUIMan()->m_pDlgTask->CheckRadioButton(1, GetCheckedRadioButton(1));
		GetGameUIMan()->m_pDlgTask->OnCommand_RdoType(szCommand);
	}
	else if (index==4)
	{
		ChangeDialog(dynamic_cast<CDlgTaskBase *>(GetGameUIMan()->GetDialog("Win_Quest_Main")));
	}
}

void CDlgStorageTaskCross::OnCommand_Ancient(const char * szCommand)
{
	if(GetGameUIMan()->GetDialog("Win_Quest_Wanted")->IsShow())
		GetGameUIMan()->GetDialog("Win_Quest_Wanted")->Show(false);
	else 
		GetGameUIMan()->GetDialog("Win_Quest_Wanted")->Show(true);
}

bool CDlgStorageTaskCross::UpdateTask(int idTask)
{
	ACString strTemp;
	ATaskTempl *pTemp;
	unsigned long i, id;
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();

	PAUITREEVIEW pTreeTask = m_pTv_KfQuestGot;
	PAUITEXTAREA pTextDesc = m_pTxt_Content;
	PAUITEXTAREA pTextItem = m_pTxt_QuestItem;

	ACString strNewTextItem = _AL("");
	bool bCrsServer = GetHostPlayer()->GetSrcServerID()!=0;

	if( idTask >= 0 )
	{
		ACString strText;
		elementdataman *pDataMan = GetGame()->GetElementDataMan();
		pTemp = pMan->GetTaskTemplByID(idTask);
		if( pTemp )
		{
			if( idTask != m_idLastTask )
			{
				ACString strDesc;
				strDesc = pTemp->GetDescription();
				ACString strTaskDesc;
				strTaskDesc = FormatTaskText(strDesc);
				pTextDesc->SetImageList(&m_vecImageList);
				pTextDesc->SetText(strTaskDesc);
				m_idLastTask = idTask;
			}

			strText = GetTaskItem(idTask);
			int nNPC = pTemp->GetAwardNPC();
			int nString = 684;
			if(pTemp->m_DynTaskType == enumDTTSpecialAward && pTemp->m_ulSpecialAward != 0)
			{
				nNPC = 1188; //新手卡所有任务完成npc都是完美使者
			}
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
						strTemp.Format(GetStringFromTable(nString), pInstance->GetNameByCoord(info->x, info->z),
							strName, info->x, info->z);
					}
					else
					{
						strTemp.Format(GetStringFromTable(nString), _AL(""),
							pNPC->name, info->x, info->z);
					}

					strText += strTemp;
					strText += _AL(" ");
					EditBoxItemBase itemImage(enumEIImage);
					itemImage.SetImageIndex(0);
					itemImage.SetImageFrame(0);
					a_sprintf(szPos, _AL("fly%d %d"), idTask, nNPC);
					itemImage.SetInfo(szPos);
					strText += (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
				}
				else
				{
					strTemp.Format(GetStringFromTable(GetCheckedRadioButton(1)==1 ? 6009 : 6010), pNPC->name);
					strText += strTemp;
				}
			}
			strNewTextItem = strText;
			
			int nRank = pTemp->GetRank();
			int i;
			for( i = 0; ; i++ )
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

			Task_Award_Preview award;
			pTask->GetTaskAwardPreview(idTask, &award);
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
				strTemp.Format(GetStringFromTable(3203), award.m_ulSP);
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

			for(i = 0; i < TASK_ZONE_FRIENDSHIP_COUNT; i++)
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
					for( DWORD i = 0; i < CDLGTASK_AWARDITEM_MAX ; i++ )
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
							strTemp.Format(_AL("%d"), award.m_ItemsNum[i]);
							m_pImg_Item[i]->SetText(strTemp);
							m_pImg_Item[i]->Show(true);
						}
					}
				}
			}
		}
	}
	else
	{
		P_AUITREEVIEW_ITEM pItem = m_pTv_KfQuestGot->GetSelectedItem();
		m_idSelTask = -1;
		if( pItem )
			m_idSelTask = m_pTv_KfQuestGot->GetItemData(pItem);

		m_idLastTask = -2;
		pTextDesc->SetText(_AL(""));
		m_pTxt_BaseAward->Show(false);
		m_pTxt_ItemAward->Show(false);
		for( int j = 0; j < CDLGTASK_AWARDITEM_MAX; j++ )
			m_pImg_Item[j]->Show(false);
		pTreeTask->DeleteAllItems();
		m_pBtn_Abandon->Enable(false);
		
		UINT iSize = pTask->GetTaskCount();
		
		for( i = 0; i < iSize; i++ )
		{
			id = pTask->GetTaskId(i);
			pTemp = pMan->GetTaskTemplByID(id);
			if( !pTemp ) continue;
			DWORD nTaskType = 0;
			if( !pTemp->IsKeyTask() )
				nTaskType = pTemp->GetType() + 1;
			if( nTaskType!=TT_CROSS_MONSTER && nTaskType!=TT_CROSS_CHALLENGE && nTaskType!=TT_CROSS_EXAM )
			//if( nTaskType!=TT_MONSTER && nTaskType!=TT_CHALLENGE && nTaskType!=TT_EXAM )
				continue;
			
			ACString strTaskTimes;
			if (pTemp->m_ulMaxFinishCount>0)
			{
				strTaskTimes.Format(_AL("(%d/%d)"), pTemp->GetFinishCount(pTask), pTemp->m_ulMaxFinishCount);
			}
			strTaskTimes = pTemp->GetName() + strTaskTimes;
			if ( bCrsServer && !pTemp->IsCrossServerTask() )
				FilterTextColor(strTaskTimes);
			pItem = pTreeTask->InsertItem(strTaskTimes);
			pTreeTask->SetItemTextColor(pItem, TaskColor[nTaskType]);
			if ( bCrsServer && !pTemp->IsCrossServerTask() )
				pTreeTask->SetItemTextColor(pItem, DisableColor);
			pTreeTask->SetItemHint(pItem, (const ACHAR*)pTemp->GetSignature());
			pTreeTask->SetItemData(pItem, id);
			InsertTaskChildren(pItem, id, true, pTemp->IsKeyTask());
			
			if( (int)id == m_idSelTask )
			{
				pTreeTask->SelectItem(pItem);
				m_pBtn_Abandon->Enable(true);
				UpdateTask(id);
			}
		}
	}
		
	if( strNewTextItem != pTextItem->GetText() )
	{
		int nFirstLines = pTextItem->GetFirstLine();
		pTextItem->SetText(strNewTextItem);
		if( idTask == (int)pTextItem->GetData() )
		{
			for(int i = 0; i < nFirstLines; i++)
				pTextItem->OnDlgItemMessage(WM_KEYDOWN, VK_DOWN, 0);
		}
	}
	pTextItem->SetData(idTask);

	return true;
}

void CDlgStorageTaskCross::InsertTaskChildren(P_AUITREEVIEW_ITEM pRoot,
								  unsigned long idTask, bool bExpand, bool bKey)
{
	int nPos;
	unsigned long id;
	ATaskTempl *pTemp;
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
		ACString strName = pTemp->GetName();
		if ( bCrsServer && !pTemp->IsCrossServerTask() )
			FilterTextColor(strName);
		pItem = m_pTv_KfQuestGot->InsertItem(strName, pRoot);
		DWORD nTaskType = 0;
		if( !pTemp->IsKeyTask() )
			nTaskType = pTemp->GetType() + 1;
		m_pTv_KfQuestGot->SetItemTextColor(pItem, TaskColor[nTaskType]);
		if ( bCrsServer && !pTemp->IsCrossServerTask() )
			m_pTv_KfQuestGot->SetItemTextColor(pItem, DisableColor);
		if( bExpand ) m_pTv_KfQuestGot->Expand(pItem, AUITREEVIEW_EXPAND_EXPAND);
		m_pTv_KfQuestGot->SetItemData(pItem, id);
		if( (int)id == m_idSelTask )
		{
			m_pTv_KfQuestGot->SelectItem(pItem);
			m_pBtn_Abandon->Enable(true);
			m_idLastTask = -1;
			UpdateTask(id);
		}
		InsertTaskChildren(pItem, id, bExpand, bKey);
	}
}