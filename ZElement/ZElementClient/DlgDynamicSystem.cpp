/********************************************************************
	created:	2012/11/5
	file name:	DlgDynamicSystem.h
	author:		zhuyan
	
	purpose:		
*********************************************************************/
#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"
#include "DlgChat.h"
#include "DlgDynamicSystem.h"
#include "DlgMiniMap.h"

#include "EC_Global.h"
#include "EC_GameRun.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_IvtrItem.h"
#include "EC_HostPlayer.h"
#include "EC_TaskInterface.h"
#include "EC_Instance.h"
#include "EC_Inventory.h"
#include "EC_GameUIMisc.h"
#include "EC_AutoMove.h"
#include "EC_World.h"
#include "EC_Model.h"
#include "EC_FixedMsg.h"

#include "elementdataman.h"
#include "TaskTemplMan.h"


#define LIVENESS_MAX 15	// 因为不知道个数 所以根据结构最大值定义
#define LIVENESS_CHILD_MAX 120	// 因为不知道个数 所以根据结构最大值定义
extern ACString g_AutoMoveDestName;
AUI_BEGIN_COMMAND_MAP(CDlgDynamicMin, CDlgBase)

AUI_ON_COMMAND("Btn_Max",		OnCommandOpen)

AUI_END_COMMAND_MAP()

bool CDlgDynamicMin::OnInitDialog()
{
	if(GetHostPlayer()->GetRebornInfo().size() >0 || GetHostPlayer()->GetBasicProps().iLevel >= 45)
	{
		GetDlgItem("Btn_Max")->Enable(true);
	}
	else if(GetHostPlayer()->GetRebornInfo().size() == 0 && GetHostPlayer()->GetBasicProps().iLevel < 45)
	{
		GetDlgItem("Btn_Max")->Enable(false);
	}
	return true;
}


void CDlgDynamicMin::OnCommandOpen(const char * szCommand)
{
	PAUIDIALOG	pDlg = GetGameUIMan()->GetDialog("Win_Activity");
	pDlg->Show(!pDlg->IsShow());
}


void CDlgDynamicMin::OnTick()
{
	if((GetHostPlayer()->GetRebornInfo().size() >0 || GetHostPlayer()->GetBasicProps().iLevel >= 45) && GetDlgItem("Btn_Max")->IsEnabled() == false)
	{
		GetDlgItem("Btn_Max")->Enable(true);	
	}
}


AUI_BEGIN_COMMAND_MAP(DlgDynamicContent, CDlgBase)


AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(DlgDynamicContent, CDlgBase)

AUI_ON_EVENT("Lst_Task",			WM_LBUTTONDOWN,	OnEventLButtonList)
AUI_ON_EVENT("Txt_BossPos",		WM_LBUTTONDOWN,		OnEventLButtonDownNpc)
AUI_ON_EVENT("Txt_TaskNpc",		WM_LBUTTONDOWN,		OnEventLButtonDownNpc)
AUI_ON_EVENT("Txt_BossPos",		WM_MOUSEMOVE,		OnEventMouseMoveNpc)
AUI_ON_EVENT("Txt_TaskNpc",		WM_MOUSEMOVE,		OnEventMouseMoveNpc)
AUI_END_EVENT_MAP()


bool DlgDynamicContent::OnInitDialog()
{
	m_pLst_Task = (AUIListBox *)GetDlgItem("Lst_Task");
	m_pTxt_TaskNpc = (AUITextArea *)GetDlgItem("Txt_TaskNpc");
	m_pTxt_TaskNpcMap = (AUITextArea *)GetDlgItem("Txt_Tasknpc_Pos");
	m_pLst_Task->SetAlign(AUIFRAME_ALIGN_CENTER);

	m_pImg_Task = (AUIImagePicture *)GetDlgItem("Img_Task");
	
	m_pTxt_TaskLine = (AUILabel *)GetDlgItem("Txt_TaskLine");
	m_pTxt_TaskLv = (AUILabel *)GetDlgItem("Txt_TaskLv");
	m_pTxt_TaskTime = (AUILabel *)GetDlgItem("Txt_TaskTime");
	m_pTxt_BossName = (AUILabel *)GetDlgItem("Txt_BossName");
	m_pTxt_BossLv = (AUILabel *)GetDlgItem("Txt_BossLv");
	m_pTxt_Desc = (AUITextArea *)GetDlgItem("Txt_2");
	for (int i=0; i<AWARD_MAX; i++)
	{
		AString strName;
		strName.Format("Img_Award%d", i+1);
		m_pImg_Award[i] = (AUIImagePicture *)GetDlgItem(strName);
	}
	
	m_vecImageList.clear();
	m_vecImageList.push_back(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_DAILYPROMP24));	//ICONTYPE_TASK24
	m_vecImageList.push_back(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_DAILYPROMP32));	//ICONTYPE_TASK32
	m_vecImageList.push_back(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_DAILYPROMP48)); //ICONTYPE_TASK48
	m_vecImageList.push_back(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY)); //ICONTYPE_TASK48

	
	m_pTxt_TaskNpc->SetImageList(GetGameUIMan()->GetControlImgLst());
	m_pTxt_TaskNpc->SetItemMask(0xffffffff);
	m_pLst_Task->SetImageList(&m_vecImageList);
	m_pLst_Task->SetItemMask(0xffffffff);
	m_nCurSel = 0;

	return true;
}


void DlgDynamicContent::UpDataUIInterface(int nType)
{
	ACString strText;
	ACString strIcon;
	ACString strWeek;
	GetDlgItem("Title")->SetText(GetStringFromTable(nType + 17203));
	m_pLst_Task->ResetContent();
	
	MenologyInfoVector vecInfo = CECMenology::GetInstance()->GetInfo(CECMenology::TYPE_DAILYDUPLICATE + nType);
	MenologyInfoVector::iterator it = vecInfo.begin();
	for ( ; it!=vecInfo.end(); ++it)
	{
		MenologyInfo* pData = it;
		EditBoxItemBase item(enumEIImage);
		item.SetImageIndex(1);
		item.SetImageFrame(GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_DAILYPROMP32, pData->strIcon1));
		item.SetColor(A3DCOLORRGB(255,255,255));
		strIcon = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
		
		if (it->iPermitCount==-1)
		{
			strText.Format( _AL("%s\t%s\t%s\t%d\t%s"), strIcon, 
				pData->strName, pData->strTime, pData->ulMinLevel, GetGameUIMan()->GetStringFromTable(7140) );
		}
		else	
		{
			strText.Format( _AL("%s\t%s\t%s\t%d\t%d"), strIcon, 
				pData->strName, pData->strTime, pData->ulMinLevel, pData->iPermitCount);
		}

		//		strText.Format( _AL("%s"), strIcon);
		m_pLst_Task->AddString(strText);
		pData->iCurTaskShow = GetSuitTask(pData->uiTaskID, pData->uiNumID);
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it-vecInfo.begin(), 0);	//index
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it->iCurTaskShow, 1);	//taskid
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, it->iId, 2);			//id
		m_pLst_Task->SetItemData(m_pLst_Task->GetCount()-1, CECMenology::TYPE_DAILYDUPLICATE + nType, 3);			//id
	}
	m_pLst_Task->SetCurSel(0);
	OnEventLButtonList(0, 0, 0);
}

bool DlgDynamicContent::CheckLevel(const MenologyInfo* info)
{
	unsigned long iLv = GetHostPlayer()->GetBasicProps().iLevel;
	if (!info->bReborn)
		return iLv>=info->ulMinLevel && iLv<=info->ulMaxLevel;
	else if (GetHostPlayer()->GetRebornInfo().size()>0)
		return iLv>=info->ulMinLevel && iLv<=info->ulMaxLevel;

	return false;

}

int DlgDynamicContent::GetSuitTask(const unsigned int* tasks, const int num)
{
	bool suit[MENOLOGY_TASKID_MAX];
	int i, j;
	UINT suitLevelMaxLimit = 0;
	BOOL bFindSuit = false;
	UINT minDiffIndex = 0;
	int	 minDiff = INT_MAX;
	ATaskTempl* lstTask;
	ATaskTemplMan		*pMan = GetGame()->GetTaskTemplateMan();
	CECTaskInterface	*pTask = GetHostPlayer()->GetTaskInterface();
	UINT		playerLevel = pTask->GetPlayerLevel();
	UINT		ulOccup = pTask->GetPlayerOccupation();

	//筛选合适职业
	for ( i = 0; i < num; i++ )
	{
		suit[i] = true;
		lstTask = pMan->GetTaskTemplByID(tasks[i]);
		if (!lstTask)
		{
			continue;
		}
		if( 0==lstTask->m_ulOccupations )
		{
			continue;
		}
		for ( j = 0; j < (int)lstTask->m_ulOccupations; j++ )
			if ( lstTask->m_Occupations[j] == ulOccup )
			{
				break;
			}
			if ( j == (int)lstTask->m_ulOccupations )
			{
				suit[i] = false;
			}
	}
	
	//筛选合适级别
	for ( i = 0; i < num; i++)
	{
		lstTask = pMan->GetTaskTemplByID(tasks[i]);
		if (!lstTask)
		{
			continue;
		}
		unsigned long levMin = lstTask->m_ulPremise_Lev_Min;
		unsigned long levMax = lstTask->m_ulPremise_Lev_Max;
		levMax = levMax>0 ? levMax : 10000;
		
		if (suit[i] && playerLevel>=levMin && playerLevel<=levMax && suitLevelMaxLimit<levMax)
		{
			return tasks[i];
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

	return tasks[minDiffIndex];
}

void DlgDynamicContent::OnEventLButtonList(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	m_strHintNpc.Empty();
	m_pTxt_TaskLine->SetText(_AL(""));
	m_pTxt_TaskLv->SetText(_AL(""));
	m_pTxt_TaskTime->SetText(_AL(""));
	m_pTxt_TaskNpc->SetText(_AL(""));
	m_pTxt_TaskNpcMap->SetText(_AL(""));
	m_pImg_Task->ClearCover();
	UINT i;
	for (i=0; i<AWARD_MAX; i++)
	{
		m_pImg_Award[i]->Show(false);
		m_pImg_Award[i]->SetText(_AL(""));
		m_pImg_Award[i]->SetHint(_AL(""));
	}
	m_nCurSel = m_pLst_Task->GetCurSel();
	int nSel = m_pLst_Task->GetCurSel();
	if( nSel < 0 || nSel >= m_pLst_Task->GetCount() )
		return;
	ACString strText;
	int index = m_pLst_Task->GetItemData(nSel, 0);
	int nType = m_pLst_Task->GetItemData(nSel, 3);
	MenologyInfoVector vecInfo = CECMenology::GetInstance()->GetInfo(nType);
	const MenologyInfo* mInfo = &vecInfo[index];

	if(mInfo)
	{
		bool bOk = false;
		
		m_pTxt_TaskLine->SetText(mInfo->strLineInfo);
		int nCurLine = GetGame()->GetGameRun()->GetCurLine();
		for (i=0; i<mInfo->vecLine.size(); i++)
		{
			if (nCurLine==mInfo->vecLine[i])
				bOk = true;
		}
		m_pTxt_TaskLine->SetColor(bOk?A3DCOLORRGB(255,0xe9,0x82):A3DCOLORRGB(255,0,0));
		
		m_pTxt_TaskLv->SetText(mInfo->strLevelInfo);
		m_pTxt_TaskLv->SetColor(CheckLevel(mInfo)?A3DCOLORRGB(255,0xe9,0x82):A3DCOLORRGB(255,0,0));
		m_pTxt_TaskTime->SetText(mInfo->strTime);
		
		tm tmNow = GetGame()->GetServerLocalTime();
		int nowMinute = tmNow.tm_hour*60 + tmNow.tm_min;
		if ( nowMinute>=(mInfo->tmStartTime.tm_hour*60+mInfo->tmStartTime.tm_min) &&
			nowMinute<=(mInfo->tmEndTime.tm_hour*60+mInfo->tmEndTime.tm_min) )
			bOk = true;
		m_pTxt_TaskTime->SetColor(bOk?A3DCOLORRGB(255,0xe9,0x82):A3DCOLORRGB(255,0,0));
		
		ACString strText;
		ACString strHint;
		if (mInfo->bTaskNpc)
		{
			int idTask = m_pLst_Task->GetItemData(nSel, 1);
			elementdataman *pDataMan = GetGame()->GetElementDataMan();
			ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
			ATaskTempl* lstTask = pMan->GetTaskTemplByID(idTask);
			
			if (lstTask)
			{
				int nNPC = lstTask->GetDeliverNPC();
				if (nNPC>0)
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
							m_pTxt_TaskNpcMap->SetText(pInstance->GetNameByCoord(info->x, info->z));
							strText = strName;
							strHint.Format(_AL("%d,%d"), info->x, info->z);
						}
						else
						{
							m_pTxt_TaskNpcMap->SetText(_AL(""));
							strText = strName;
							strHint.Format(_AL("%d,%d"), info->x, info->z);
						}
						
						if (mInfo->bIsTransport)
						{
							strText += _AL(" ");
							EditBoxItemBase itemImage(enumEIImage);
							itemImage.SetImageIndex(CECGameUIMan::ICONLIST_TRANSFER);
							itemImage.SetImageFrame(0);
							ACHAR szId[100];
							a_sprintf(szId, _AL("%d"), m_pLst_Task->GetItemData(nSel, 2));
							itemImage.SetInfo(szId);
							strText += (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
						}
					}
					else
					{
						strText.Format(GetStringFromTable(GetCheckedRadioButton(1)==1 ? 6009 : 6010), pNPC->name);
					}
				}
				m_pTxt_TaskNpc->SetText(strText);
			}
			m_pTxt_TaskNpc->SetHint(strHint);
			m_strHintNpc = strHint;
		}
		else
		{
			ACString strName;
			int x, y, z;
			a_sscanf(mInfo->strNpcPos, _AL("%d,%d,%d"), &x, &y, &z);
			if (x!=9999&&y!=9999)
			{
				EditBoxItemBase item(enumEICoord);
				item.SetName(mInfo->strNPC);
				y = 0;
				ACHAR szPos[100];
				a_sprintf(szPos, _AL("%d %d %d %d"), x, y, z, mInfo->iNpcMapid);
				item.SetInfo(szPos);
				item.SetColor(A3DCOLORRGB(0, 255, 0));
				strName = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
				strHint.Format(_AL("%d,%d"), x, z);
			}
			else
			{
				strName = mInfo->strNPC;
			}
			CECInstance *pInstance = GetGameRun()->GetInstance(mInfo->iNpcMapid);
			if( pInstance )
			{
				m_pTxt_TaskNpcMap->SetText(pInstance->GetNameByCoord((float)x, (float)z));
				strText = strName;
			}
			else
			{
				m_pTxt_TaskNpcMap->SetText(_AL(""));
				strText = mInfo->strNPC;
			}	
			
			if (mInfo->bIsTransport)
			{
				strText += _AL(" ");
				EditBoxItemBase itemImage(enumEIImage);
				itemImage.SetImageIndex(CECGameUIMan::ICONLIST_TRANSFER);
				itemImage.SetImageFrame(0);
				ACHAR szId[100];
				a_sprintf(szId, _AL("%d"), m_pLst_Task->GetItemData(nSel, 2));
				itemImage.SetInfo(szId);
				strText += (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
			}
			m_pTxt_TaskNpc->SetText(strText);
			m_pTxt_TaskNpc->SetHint(strHint);
			m_strHintNpc = strHint;
		}
		GetGameUIMan()->SetImgIconSet(m_pImg_Task, CECGameUIMan::ICONS_DAILYPROMP48, mInfo->strIcon2);
		m_pImg_Task->SetHint(mInfo->strName);
		m_pTxt_Desc->SetText(mInfo->strDesc);

		
		if (mInfo->bTemplateAward)
		{
			UINT index = 0;
			int idTask = GetSuitTask(mInfo->uiTaskID, mInfo->uiNumID);
			ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
			ATaskTempl* lstTask = pMan->GetTaskTemplByID(idTask);
			if(!lstTask || lstTask->m_ulAwardType_S!=enumTATNormal)
				return;
			AWARD_DATA* awardData = lstTask->m_Award_S;
			ACString strTemp;


 			if( awardData->m_ulGoldNum > 0 )
 			{
 				GetGameUIMan()->SetImgIconSet(m_pImg_Award[index], CECGameUIMan::ICONS_DAILYPROMP32, "金钱.dds");
 				ACHAR szGold[256];
 				CECIvtrItem::GetPriceText(szGold, awardData->m_ulGoldNum);
 				m_pImg_Award[index]->SetHint(szGold);
				index++;
			}
			if( awardData->m_ulExp > 0 )
			{
				GetGameUIMan()->SetImgIconSet(m_pImg_Award[index], CECGameUIMan::ICONS_DAILYPROMP32, "经验.dds");
				strText.Format(GetStringFromTable(3202), awardData->m_ulExp);
				m_pImg_Award[index]->SetHint(strText);
				index++;
			}
			strText.Empty();
			if( awardData->m_ulSP > 0 )
			{
				strTemp.Format(GetStringFromTable(3203), awardData->m_ulSP);
				strText += strTemp;
				strText += _AL("\r");
			}
			if( awardData->m_ulReputation > 0 )
			{
				strTemp.Format(GetStringFromTable(3205), awardData->m_ulReputation);
				strText += strTemp;
				strText += _AL("\r");
			}
			for(UINT i = 0; i < TASK_ZONE_FRIENDSHIP_COUNT; i++)
			{
				if( awardData->m_aFriendships[i] > 0 )
				{
					strTemp.Format(GetStringFromTable(3210 + i), awardData->m_aFriendships[i]);
					strText += strTemp;
					strText += _AL("\r");
				}
				
				if( awardData->m_nFamilySkillProficiency )
				{
					strTemp.Format(GetStringFromTable(3431 + awardData->m_nFamilySkillIndex), awardData->m_nFamilySkillProficiency);
					strText += strTemp;
				}
				else if( awardData->m_nFamilySkillLevel )
				{
					strText += GetStringFromTable(3441 + awardData->m_nFamilySkillIndex);
				}
				if( strText != _AL("") )
				{
					GetGameUIMan()->SetImgIconSet(m_pImg_Award[index], CECGameUIMan::ICONS_DAILYPROMP32, "声望.dds");
					m_pImg_Award[index]->SetHint(strText);
					index++;
				}
				if( awardData->m_ulCandItems>0 )
				{
					const AWARD_ITEMS_CAND& ic = awardData->m_CandItems[0];
					if (ic.m_bRandChoose)
					{
						GetGameUIMan()->SetImgIconSet(m_pImg_Award[index], CECGameUIMan::ICONS_DAILYPROMP32, "未知物品.dds");
						m_pImg_Award[index]->SetHint(GetGameUIMan()->GetStringFromTable(3204));
						index++;
					}
					else
					{
						std::map<unsigned long, unsigned long> mapItemIdNum;
						for (unsigned long j = 0; j < ic.m_ulAwardItems; j++)
						{
							const ITEM_WANTED& wi = ic.m_AwardItems[j];
							
							//if (!wi.m_bCommonItem) continue;
							//else
							if (wi.m_fProb != 1.0f)
							{
								mapItemIdNum.clear();
								GetGameUIMan()->SetImgIconSet(m_pImg_Award[index], CECGameUIMan::ICONS_DAILYPROMP32, "未知物品.dds");
								m_pImg_Award[index]->SetHint(GetGameUIMan()->GetStringFromTable(3204));
								index++;
								break;
							}
							else
							{
								mapItemIdNum[wi.m_ulItemTemplId] = wi.m_ulItemNum;
							}
						}
						if (!mapItemIdNum.empty())
						{
							for (std::map<unsigned long, unsigned long>::iterator it = mapItemIdNum.begin(); it!=mapItemIdNum.end(); ++it)
							{
								m_pImg_Award[index]->Show(true);
								GetGameUIMan()->SetIvtrInfo(m_pImg_Award[index], it->first, it->second, true);
								index++;
								if (index>=6)
									break;
							}
						}			
					}
				}
			}
		}
		else
		{
			UINT i;
			for ( i=0; i<mInfo->vecAward.size(); i++ )
			{
				MenologyAward  mAward = mInfo->vecAward[i];
				if (mAward.iId>0)
				{
					GetGameUIMan()->SetIvtrInfo(m_pImg_Award[i], mAward.iId, 1, true);
				}
				else
				{
					GetGameUIMan()->SetImgIconSet(m_pImg_Award[i], CECGameUIMan::ICONS_DAILYPROMP32, mAward.strIcon);
					m_pImg_Award[i]->SetHint(mAward.strDesc);
				}
				m_pImg_Award[i]->Show(true);
				
			}
		}
	}
}

void DlgDynamicContent::OnEventLButtonDownNpc(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	
	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	GetItemLinkOnText(x, y, (PAUITEXTAREA)pObj, &Item);
	
	if( Item.m_pItem != NULL )
	{
		if( Item.m_pItem->GetType() == enumEICoord )
		{
			A3DVECTOR3 vec;
			int idInstance;
			a_sscanf(Item.m_pItem->GetInfo(), _AL("%f%f%f%d"), 
				&vec.x, &vec.y, &vec.z, &idInstance);
			((CDlgMiniMap*)GetGameUIMan()->GetDialog("Win_MiniMap"))->m_dwTempTargetTime = GetTickCount();
			((CDlgMiniMap*)GetGameUIMan()->GetDialog("Win_MiniMap"))->m_vecTempTarget = vec;
			((CDlgMiniMap*)GetGameUIMan()->GetDialog("Win_MiniMap"))->m_idTempTargetInstance = idInstance;
			
			g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, idInstance);
			g_AutoMoveDestName = Item.m_pItem->GetName();
			
			if( idInstance == GetWorld()->GetInstanceID() && GetHostPlayer()->GetModelTempArrow() )
				GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
		}
		else if( Item.m_pItem->GetType() == enumEIImage )
		{
			int id;
			a_sscanf(Item.m_pItem->GetInfo(), _AL("%d"), &id);
			GetGameUIMan()->CheckCanDailyPrompFly(id);
		}
	}
}

void DlgDynamicContent::OnEventMouseMoveNpc(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	GetItemLinkOnText(x, y, (PAUITEXTAREA)pObj, &Item);
	pObj->SetHint(m_strHintNpc);
	if( Item.m_pItem != NULL )
	{
		if( Item.m_pItem->GetType() == enumEIImage )
		{
			pObj->SetHint(GetGameUIMan()->GetStringFromTable(3302));
		}
	}	
}


AUI_BEGIN_COMMAND_MAP(DlgDynamicSystem, CDlgBase)

AUI_ON_COMMAND("Btn_Get0*",		OnCommandType)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(DlgDynamicSystem, CDlgBase)

// AUI_ON_EVENT("Item_*",			WM_LBUTTONDOWN,		OnEventLButtonDownItem)
// AUI_ON_EVENT("Item_*",				WM_LBUTTONDBLCLK,	OnEventLButtonDownItem)

AUI_ON_EVENT("Lst_Task",			WM_LBUTTONDOWN,	OnEventLButtonList)

AUI_END_EVENT_MAP()


DlgDynamicSystem::DlgDynamicSystem()
{
}

 DlgDynamicSystem::~DlgDynamicSystem()
{
	m_mapDyanmicList.clear();
	m_mapDyanmicChild.clear();
}


bool DlgDynamicSystem::OnInitDialog()
{
	DATA_TYPE dt;
	elementdataman* pDB = GetGame()->GetElementDataMan();
	LIVENESS_CONFIG* pLiveNess = (LIVENESS_CONFIG*)pDB->get_data_ptr(724, ID_SPACE_CONFIG, dt);

	ACString szName ;
	for (int nIndex = 0; nIndex < LIVENESS_MAX; nIndex++)
	{
		szName = GetStringFromTable(nIndex + 17203);
		if(szName.IsEmpty())
		{
			continue;
		}
		DYNAMIC_INFO pData;
		pData.id = nIndex;
		pData.nGain = pLiveNess->scores[nIndex];
		pData.szTypeName = GetStringFromTable(nIndex + 17203);
		m_mapDyanmicList[nIndex] = pData;		
	}
	for(int nChild = 0; nChild < LIVENESS_CHILD_MAX; nChild++)
	{
		DYNAMIC_CHILD_INFO pChild;
		szName = pLiveNess->element_list[nChild].name;
		
		if(szName.IsEmpty())
		{
			continue;
		}
		pChild.nIndex = nChild;
		pChild.nTypeid = pLiveNess->element_list[nChild].type_activity;
		pChild.name = pLiveNess->element_list[nChild].name;
		pChild.nRequire_level = pLiveNess->element_list[nChild].require_level;
		pChild.nRenascence_count = pLiveNess->element_list[nChild].renascence_count;
		m_mapDyanmicChild[nChild] = pChild;
		
	}

	m_pPrg_Point = (PAUIPROGRESS)GetDlgItem("Prgs_Score");
	
	m_pTvTaskList = (PAUILISTBOX)GetDlgItem("Lst_Task");

	/// 初始化奖励物品
	AString strName;
	int i;
	for (i=0; i<4; ++i)
	{
		strName.Format("Item_%d", i + 55115);

		m_pImgSelected[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		m_pImgSelected[i]->SetData(i);
		m_pImgSelected[i]->SetColor(A3DCOLORRGB(255, 255, 255));
		int id =  pLiveNess->gifts[i];
		GetGroupItem(id,strName);

		strName.Format("Btn_Get0%d", i + 1);
		GetDlgItem(strName)->Enable(false);
	}
	UpdataDysnamicItem();
 	UpdataDysnamic();
	return true;
}


void DlgDynamicSystem::OnHideDialog()
{
}


void DlgDynamicSystem::OnShowDialog()
{
	UpdataDysnamicItem();
	UpdataDysnamic();
}		

void DlgDynamicSystem::OnCommandType(const char* szCommand)
{
	CECInventory* pPack = GetHostPlayer()->GetPack();
	// size < 2: No need to arrange
	if (pPack)
	{
		if( 0 == pPack->GetEmptySlotNum())
		{
			GetGameUIMan()->MessageBox(17218);
		}
	}
	UINT index = atoi(szCommand+7) - 1;
	if(index >= 0 || index > 3)
	{
		GetGameSession()->c2s_CmdCmdLiveness(index);
	}
}

// void DlgDynamicSystem::OnEventLButtonDownItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
// {
// 	int nIndex = pObj->GetData();
// 	if(pObj->IsEnabled())
// 	{
// 		GetGameSession()->c2s_CmdCmdLiveness(pObj->GetData());
// 	}
// }

void DlgDynamicSystem::OnEventLButtonList(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int nSel = m_pTvTaskList->GetCurSel();
	if( nSel >= 0 && nSel < m_pTvTaskList->GetCount() )
	{
		int nType = m_pTvTaskList->GetItemData(nSel);
		DlgDynamicContent*	pDlg = (DlgDynamicContent*)GetGameUIMan()->GetDialog("Win_Activity_Daily");
			pDlg->UpDataUIInterface(nSel);
			pDlg->Show(!pDlg->IsShow());
	}
}


void DlgDynamicSystem::UpdataDysnamic()
{
	m_pTvTaskList->ResetContent();
	ACString strText;
	int nSelect = 0;
	int iColor;
	bool nIsDulfill = false;
	abase::hash_map<int,DYNAMIC_INFO>::iterator it = m_mapDyanmicList.begin();
	for(; it != m_mapDyanmicList.end(); ++it )
	{
		nSelect = 0;
		nIsDulfill = false;
		DYNAMIC_INFO* pData = &it->second;
		abase::hash_map<int, DYNAMIC_CHILD_INFO>::iterator pChild = m_mapDyanmicChild.begin();
		for(; pChild != m_mapDyanmicChild.end(); ++pChild)
		{
			DYNAMIC_CHILD_INFO* pChildData = &pChild->second;
			if(pChildData->nTypeid == pData->id)
			{
				if(GetHostPlayer()->GetBasicProps().iLevel >= pChildData->nRequire_level
					&& GetHostPlayer()->GetRebornCount() >= pChildData->nRenascence_count)
				{
					nSelect++;
				}
				if(GetHostPlayer()->HasLiveness(pChildData->nIndex) && nIsDulfill == false)
				{
					nIsDulfill = true;
				}
			}
		}
	
		strText = _AL("");
		iColor = nIsDulfill ? ITEMDESC_COL_GREEN : ITEMDESC_COL_WHITE;
		strText.Format(_AL("%s\t%d\t%d\t%s%s"),pData->szTypeName, nSelect, pData->nGain, g_pGame->GetItemDesc()->GetWideString(iColor), nIsDulfill != true ? GetStringFromTable(15009) : GetStringFromTable(15008));
		m_pTvTaskList->AddString(strText);
		m_pTvTaskList->SetItemData(m_pTvTaskList->GetCount() - 1, pData->id);
		m_pTvTaskList->SetItemHint(m_pTvTaskList->GetCount() - 1, GetStringFromTable(17100));
	}	

}

void DlgDynamicSystem::UpdataDysnamicItem()
{
	if(GetHostPlayer()->getLivenessCurPoint() != 0)
	{
		m_pPrg_Point->SetProgress(AUIPROGRESS_MAX * GetHostPlayer()->getLivenessCurPoint() / 100);
	}
	ACString strName;
	strName.Format(_AL("%d"),GetHostPlayer()->getLivenessCurPoint());
	GetDlgItem("Txt_Score")->SetText(strName);
	
	
	int nIndex = 0;
	int nPoint = 25;
	AString strBtn;
	for (int i=0; i<4; ++i)
	{
		strBtn.Format("Btn_Get0%d", i + 1);
		GetDlgItem(strBtn)->Enable(false);
		if(GetHostPlayer()->getLivenessCurPoint() >= nPoint && GetHostPlayer()->CheckLivenessTacken(i) == false)
		{
			GetDlgItem(strBtn)->Enable(true);
		}
		GetDlgItem(strBtn)->SetText(GetHostPlayer()->CheckLivenessTacken(i) ? GetStringFromTable(17221) : GetStringFromTable(17220));
		nPoint += 25;
	}
}

void DlgDynamicSystem::GetGroupItem(int nID,const char *pszItemName)
{
	CECIvtrItem* pItem = CECIvtrItem::CreateItem(nID, 0,1);
	AString strFile;
	AUICTranslate trans;
	PAUIIMAGEPICTURE pPicture = (PAUIIMAGEPICTURE)GetDlgItem(pszItemName);
	if (pItem )
	{
		pItem->SetNeedUpdate(false);
// 		af_GetFileTitle(pItem->GetIconFile(), strFile);
// 		strFile.MakeLower();
// 		pPicture->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
// 			GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
// 		pPicture->SetCover(NULL, 0, 1);
		pPicture->SetHint(trans.Translate(pItem->GetDesc()));
		delete pItem;
	}
}