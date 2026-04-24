// Filename	: CDlgTaskBase.cpp
// Creator	: Fu Chonggang
// Date		: 2011/4/02

#include "AFI.h"
#include "aui\\CSplit.h"
#include "AUI\\AUICTranslate.h"
#include "DlgTaskBase.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_HostPlayer.h"
#include "EC_Faction.h"
#include "EC_IvtrItem.h"
#include "EC_TaskInterface.h"
#include "TaskTemplMan.h"
#include "elementdataman.h"
#include "A2DSprite.h"

#define new A_DEBUG_NEW

template<> abase::vector<CDlgTaskBase*> CDlgTabContainer<CDlgTaskBase*>::ms_Pages;
template<> CDlgTaskBase* CDlgTabContainer<CDlgTaskBase*>::ms_LastShow = 0;

CDlgTaskBase::CDlgTaskBase()
{
	m_iFixSpriteNum = 0;
}

CDlgTaskBase::~CDlgTaskBase()
{
}

bool CDlgTaskBase::Release()
{
	ClearImages();
	for (UINT i=0; i<m_vecImageList.size(); ++i)
	{
		A3DRELEASE(m_vecImageList[i]);
	}
	m_vecImageList.clear();
	return CDlgBase::Release();
}

void CDlgTaskBase::Resize(A3DRECT rcOld, A3DRECT rcNew)
{
	AUIDialog::Resize(rcOld, rcNew);
	float fWindowScale = GetAUIManager()->GetWindowScale();
	abase::vector<A2DSprite*>::iterator it;
	for(it=m_vecImageList.begin(); it!=m_vecImageList.end(); ++it)
	{
		(*it)->SetScaleX(fWindowScale);
		(*it)->SetScaleY(fWindowScale);
	}
}

void CDlgTaskBase::ClearImages()
{
	while(m_vecImageList.size()>m_iFixSpriteNum)
	{
		A3DRELEASE(m_vecImageList[m_vecImageList.size()-1]);
		m_vecImageList.pop_back();
	}
}

extern CECStringTab _task_err;
ACString CDlgTaskBase::GetTaskItem(int idTask)
{
	unsigned long i;
	ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
	CECTaskInterface *pTask = GetHostPlayer()->GetTaskInterface();
	ACString strText, strTemp;
	Task_State_info tsi;
	int nHour, nMin, nSec;
	MONSTER_ESSENCE *pMonster;
	INTERACTION_OBJECT_ESSENCE* pInterObj;
	NPC_ESSENCE		*pNpc;
	DATA_TYPE DataType;
	elementdataman *pDataMan = GetGame()->GetElementDataMan();

	pTask->GetTaskStateInfo(idTask, &tsi);

	if (tsi.m_ulErrCode)
	{
		const wchar_t* szMsg = _task_err.GetWideString(tsi.m_ulErrCode);

		if (szMsg)
		{
			strText = _AL("^ff0000");
			strText += szMsg;
			strText +=  GetStringFromTable(798);
			strText += _AL("\r^ffffff");
		}
	}

	if( tsi.m_ulTimeLimit > 0 )
	{
		nSec = tsi.m_ulTimeLimit;
		nHour = nSec / 3600;
		nSec -= nHour * 3600;
		nMin = nSec / 60;
		nSec -= nMin * 60;
		strTemp.Format(GetStringFromTable(245), nHour, nMin, nSec);
		strText += strTemp;

		nSec = max(0, tsi.m_ulTimeLimit - tsi.m_ulTimePassed);
		nHour = nSec / 3600;
		nSec -= nHour * 3600;
		nMin = nSec / 60;
		nSec -= nMin * 60;
		strTemp.Format(GetStringFromTable(246), nHour, nMin, nSec);
		strTemp += _AL("\r");
		strText += strTemp;
	}

	if( tsi.m_ulWaitTime > 0 )
	{
		nSec = tsi.m_ulWaitTime;
		nHour = nSec / 3600;
		nSec -= nHour * 3600;
		nMin = nSec / 60;
		nSec -= nMin * 60;
		strTemp.Format(GetStringFromTable(13001), nHour, nMin, nSec);
		strText += strTemp;
		
		nSec = max(0, tsi.m_ulWaitTime - tsi.m_ulTimePassed);
		nHour = nSec / 3600;
		nSec -= nHour * 3600;
		nMin = nSec / 60;
		nSec -= nMin * 60;
		strTemp.Format(GetStringFromTable(13002), nHour, nMin, nSec);
		strTemp += _AL("\r");
		strText += strTemp;
	}

	if( tsi.m_ulNPCToProtect > 0 )
	{
		pMonster = (MONSTER_ESSENCE *)pDataMan->get_data_ptr(
			tsi.m_ulNPCToProtect, ID_SPACE_ESSENCE, DataType);
		if( DataType == DT_MONSTER_ESSENCE )
		{
			strTemp.Format(GetStringFromTable(257), pMonster->name);
			strText += strTemp;
		}

		nSec = tsi.m_ulProtectTime;
		nHour = nSec / 3600;
		nSec -= nHour * 3600;
		nMin = nSec / 60;
		nSec -= nMin * 60;
		strTemp.Format(GetStringFromTable(258), nHour, nMin, nSec);
		strText += strTemp;

		nSec = max(0, tsi.m_ulProtectTime - tsi.m_ulTimePassed);
		nHour = nSec / 3600;
		nSec -= nHour * 3600;
		nMin = nSec / 60;
		nSec -= nMin * 60;
		strTemp.Format(GetStringFromTable(259), nHour, nMin, nSec);
		strTemp += _AL("\r");
		strText += strTemp;
	}
	
	for(i = 0; i < MAX_INTEROBJ_WANTED; i++)
	{
		if(tsi.m_InterObjWanted[i].m_ulInterObjId == 0)
			break;

		const void* vPtr = pDataMan->get_data_ptr(tsi.m_InterObjWanted[i].m_ulInterObjId, ID_SPACE_ESSENCE, DataType);

		if(vPtr && DataType == DT_INTERACTION_OBJECT_ESSENCE)
		{
			pInterObj = (INTERACTION_OBJECT_ESSENCE*)vPtr;
			ACString strName;
			strName = pInterObj->name;
			
			if(tsi.m_InterObjWanted[i].m_ulInterObjToCollect > 0)
			{
				strTemp.Format(GetStringFromTable(21550), strName, 
					tsi.m_InterObjWanted[i].m_ulInterObjCollected, tsi.m_InterObjWanted[i].m_ulInterObjToCollect);

				strTemp + _AL("\r");
				strText += strTemp;
			}
		}
	}

	for( i = 0; i < MAX_MONSTER_WANTED; i++ )
	{
		if (tsi.m_MonsterWanted[i].m_ulMonsterId == 0)
			break;

		const void *vPtr = pDataMan->get_data_ptr(
					tsi.m_MonsterWanted[i].m_ulMonsterId, ID_SPACE_ESSENCE, DataType);
		if( DataType == DT_MONSTER_ESSENCE )
		{
			pMonster = (MONSTER_ESSENCE *)vPtr;
			ACString strName;
			const NPC_INFO *info = pMan->GetTaskNPCInfo(pMonster->id);
			if( info && info->map_id!=0 )
			{
				EditBoxItemBase item(enumEICoord);
				item.SetName(pMonster->name);
				ACHAR szPos[100];
				a_sprintf(szPos, _AL("%d %d %d %d"), info->x, info->y, info->z, info->map_id);
				item.SetInfo(szPos);
				item.SetColor(A3DCOLORRGB(0, 255, 0));
				strName = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			}
			else
				strName = pMonster->name;
			if( tsi.m_MonsterWanted[i].m_ulMonstersToKill > 0 )
			{
				strTemp.Format(GetStringFromTable(247), strName,
					tsi.m_MonsterWanted[i].m_ulMonstersKilled, tsi.m_MonsterWanted[i].m_ulMonstersToKill);
			}
			else
			{
				strTemp.Format(GetStringFromTable(256), strName, tsi.m_MonsterWanted[i].m_ulMonstersKilled);
			}
			if( info && info->map_id!=0 )
			{
				strTemp += _AL(" ");
				EditBoxItemBase item(enumEIImage);
				item.SetImageIndex(0);
				item.SetImageFrame(0);
				ACHAR szPos[100];
				a_sprintf(szPos, _AL("fly%d %d"), idTask, pMonster->id);
				item.SetInfo(szPos);
				strTemp += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			}
			strTemp += _AL("\r");
			strText += strTemp;
		}
		else if( DataType == DT_NPC_ESSENCE )
		{
			pNpc = (NPC_ESSENCE *)vPtr;
			ACString strName;
			const NPC_INFO *info = pMan->GetTaskNPCInfo(pNpc->id);
			if( info && info->map_id!=0 )
			{
				EditBoxItemBase item(enumEICoord);
				item.SetName(pNpc->name);
				ACHAR szPos[100];
				a_sprintf(szPos, _AL("%d %d %d %d"), info->x, info->y, info->z, info->map_id);
				item.SetInfo(szPos);
				item.SetColor(A3DCOLORRGB(0, 255, 0));
				strName = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			}
			else
				strName = pNpc->name;
			if( tsi.m_MonsterWanted[i].m_ulMonstersToKill > 0 )
			{
				strTemp.Format(GetStringFromTable(247), strName,
					tsi.m_MonsterWanted[i].m_ulMonstersKilled, tsi.m_MonsterWanted[i].m_ulMonstersToKill);
			}
			else
			{
				strTemp.Format(GetStringFromTable(256), strName, tsi.m_MonsterWanted[i].m_ulMonstersKilled);
			}
			if( info && info->map_id!=0 )
			{
				strTemp += _AL(" ");
				EditBoxItemBase item(enumEIImage);
				item.SetImageIndex(0);
				item.SetImageFrame(0);
				ACHAR szPos[100];
				a_sprintf(szPos, _AL("fly%d %d"), idTask, pNpc->id);
				item.SetInfo(szPos);
				strTemp += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			}
			strTemp += _AL("\r");
			strText += strTemp;
		}
	}

	for( i = 0; i < MAX_ITEM_WANTED; i++ )
	{
		if( tsi.m_ItemsWanted[i].m_ulItemId <= 0 )
			break;

		// replace item name
		ACString strReplaceItem;
		if(tsi.m_ItemsWanted[i].m_ulReplaceItemId > 0)
		{
			CECIvtrItem* pReplaceItem = CECIvtrItem::CreateItem(
				tsi.m_ItemsWanted[i].m_ulReplaceItemId, 0, 1);

			if (pReplaceItem)
			{
				ACString strName = pReplaceItem->GetName();
				int idMonster = tsi.m_ItemsWanted[i].m_ulReplaceItemId;
				const NPC_INFO *info = pMan->GetTaskNPCInfo(idMonster);
				if( info && info->map_id!=0 )
				{
					EditBoxItemBase item(enumEICoord);
					item.SetName(pReplaceItem->GetName());
					ACHAR szPos[100];
					a_sprintf(szPos, _AL("%d %d %d %d"), info->x, info->y, info->z, info->map_id);
					item.SetInfo(szPos);
					item.SetColor(A3DCOLORRGB(0, 255, 0));
					strName = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
				}

				if( tsi.m_ItemsWanted[i].m_ulItemsToGet > 0 )
				{
					strReplaceItem.Format(GetStringFromTable(17111), strName);
				}

				if( info && info->map_id!=0 )
				{
					strTemp += _AL(" ");
					EditBoxItemBase item(enumEIImage);
					item.SetImageIndex(0);
					item.SetImageFrame(0);
					ACHAR szPos[100];
					a_sprintf(szPos, _AL("fly%d %d"), idTask, idMonster);
					item.SetInfo(szPos);
					strReplaceItem += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
				}
				delete pReplaceItem;
			}
		}

		CECIvtrItem* pItem = CECIvtrItem::CreateItem(
			tsi.m_ItemsWanted[i].m_ulItemId, 0, 1);

		if (pItem)
		{
			ACString strName = pItem->GetName();
			int idMonster = tsi.m_ItemsWanted[i].m_ulMonsterId;
			const NPC_INFO *info = pMan->GetTaskNPCInfo(idMonster);
			if( info && info->map_id!=0 )
			{
				EditBoxItemBase item(enumEICoord);
				item.SetName(pItem->GetName());
				ACHAR szPos[100];
				a_sprintf(szPos, _AL("%d %d %d %d"), info->x, info->y, info->z, info->map_id);
				item.SetInfo(szPos);
				item.SetColor(A3DCOLORRGB(0, 255, 0));
				strName = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			}

			if( tsi.m_ItemsWanted[i].m_ulItemsToGet > 0 )
			{
				if(tsi.m_ItemsWanted[i].m_ulReplaceItemId > 0)
				{
					strTemp.Format(GetStringFromTable(17110), strName,strReplaceItem,
						tsi.m_ItemsWanted[i].m_ulItemsGained,
						tsi.m_ItemsWanted[i].m_ulItemsToGet);
				}
				else
				{
					strTemp.Format(GetStringFromTable(248), strName,
						tsi.m_ItemsWanted[i].m_ulItemsGained,
						tsi.m_ItemsWanted[i].m_ulItemsToGet);
				}
			}
			else
			{
				strTemp.Format(GetStringFromTable(309), strName,
					tsi.m_ItemsWanted[i].m_ulItemsGained);
			}
			if( info && info->map_id!=0 )
			{
				strTemp += _AL(" ");
				EditBoxItemBase item(enumEIImage);
				item.SetImageIndex(0);
				item.SetImageFrame(0);
				ACHAR szPos[100];
				a_sprintf(szPos, _AL("fly%d %d"), idTask, idMonster);
				item.SetInfo(szPos);
				strTemp += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			}
			strTemp += _AL("\r");
			strText += strTemp;
			delete pItem;
		}
	}
	
	ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);
	if ( pTemp && pTemp->m_bAutoMoveForReachFixedSite )
	{
		EditBoxItemBase item(enumEICoord);
		item.SetName((const ACHAR*)pTemp->m_szAutoMoveDestPosName);
		ACHAR szPos[100];
		a_sprintf(szPos, _AL("%.0f %.0f %.0f %d"), pTemp->m_AutoMoveDestPos.x, pTemp->m_AutoMoveDestPos.y, 
			pTemp->m_AutoMoveDestPos.z, pTemp->m_ulReachSiteId);
		item.SetInfo(szPos);
		item.SetColor(A3DCOLORRGB(0, 255, 0));
		ACString strPlace = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();

		strTemp.Format(GetStringFromTable(13003), strPlace);
		
		if(pTemp->m_enumMethod == enumTMReachSite)
		{
			strTemp += _AL(" ");
			EditBoxItemBase itemimage(enumEIImage);
			itemimage.SetImageIndex(0);
			itemimage.SetImageFrame(0);
			ACHAR szPosImage[100];
			a_sprintf(szPosImage, _AL("reachsite%d"), idTask);
			itemimage.SetInfo(szPosImage);
			strTemp += (ACHAR)AUICOMMON_ITEM_CODE_START + itemimage.Serialize();
		}

		strTemp += _AL("\r");
		strText += strTemp;
	}

	for(i = 0; i < 5; i++)
	{
		if(tsi.m_iFactionPropWanted[i] > 0)
		{
			strTemp.Format(GetStringFromTable(21551 + i), tsi.m_iFactionPropWanted[i]);
			strTemp += _AL("\r");
			strText += strTemp;
		}
	}

	if(pTemp && pTemp->m_enumMethod == enumTMCollectNumArticle)
	{
		if(pTemp->m_iFactionGoldNoteWanted > 0)
		{
			strTemp.Format(GetStringFromTable(21556), pTemp->m_iFactionGoldNoteWanted);
			strTemp += _AL("\r");
			strText += strTemp;
		}
	}

	return strText;
}

ACString CDlgTaskBase::FormatTaskText(const ACHAR* szText, bool bFormatImg)
{
	if(bFormatImg)
		ClearImages();
	int len = a_strlen(szText);
	ACString keyword;
	ACString result = _AL("");
	ACHAR text[3000];
	a_strcpy(text, szText);
	text[len++] = '&';
	int i = 0;
	int j = 0;
	int k;
	while( i < len )
	{
		k = i;
		while( i < len && text[i] != '&' )
			i++;
		if( i >= len ) break;
		text[i] = '\0';
		result += text + k;
		k = ++i;
		while( i < len && text[i] != '&' )
			i++;
		if( i >= len ) break;
		text[i] = '\0';
		keyword = text + k;
		if( keyword == _AL("") )
			result += _AL("^&");
		else if( keyword == _AL("name") )
			result += GetHostPlayer()->GetName();
		else if( keyword == _AL("level") )
		{
			ACString level;
			level.Format(_AL("%d"), GetHostPlayer()->GetBasicProps().iLevel);
			result += level;
		}
		else if( keyword == _AL("prof") )
			result += GetStringFromTable(PROFESSION_START + GetHostPlayer()->GetProfession());
		else if( keyword == _AL("org") )
			result += GetStringFromTable(2101 + GetHostPlayer()->GetProfession());
		else if( keyword == _AL("sex1") )
			result += GetStringFromTable(2251 + GetHostPlayer()->GetGender());
		else if( keyword == _AL("sex2") )
			result += GetStringFromTable(2253 + GetHostPlayer()->GetGender());
		else if( keyword == _AL("sex3") )
			result += GetStringFromTable(2255 + GetHostPlayer()->GetGender());
		else if( keyword == _AL("guild") )
		{
			if( GetHostPlayer()->GetFactionID() )
			{
				Faction_Info *pInfo = GetGame()->GetFactionMan()->GetFaction(GetHostPlayer()->GetFactionID());
				if( pInfo )
					result += pInfo->GetName();
			}
		}
		else if( keyword == _AL("family") )
		{
			if( GetHostPlayer()->GetFamilyID() )
			{
				Faction_Info *pInfo = GetGame()->GetFactionMan()->GetFaction(GetHostPlayer()->GetFamilyID(), false, true);
				if( pInfo )
					result += pInfo->GetName();
			}
			else
				result += GetGameUIMan()->GetStringFromTable(9128);
		}
		else if (keyword.Find(_AL("image"))>=0)// &image(xx.tga)&
		{
			ACString strFileName = keyword;
			strFileName.CutLeft(6);
			strFileName.CutRight(1);
			A2DSprite* pImageList = new A2DSprite;
			if( !pImageList )
			{
				AUI_ReportError(__LINE__, 1,"CDlgTask::FormatTaskText, open image file %s error!", AC2AS(strFileName));
				continue;
			}
			
			bool bval = pImageList->Init(m_pA3DDevice, AC2AS(strFileName), AUI_COLORKEY);
			if( !bval )
			{
#ifndef _DEBUG
				AUI_ReportError(__LINE__, 1,"CDlgTask::FormatTaskText, open image file %s error!", AC2AS(strFileName));
#endif
				continue;
			}
			m_vecImageList.push_back(pImageList);
			EditBoxItemBase item(enumEIImage);
			item.SetImageIndex(m_vecImageList.size()-1);
			item.SetImageFrame(0);
			result += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
		}
		else if (keyword.Find(_AL("radio"))>=0)// &radio&
		{
			if(m_vecImageList.size() >= 2)		//0: transfer; 1:radio
			{
				EditBoxItemBase item(enumEIImage);
				item.SetImageIndex(1);
				item.SetImageFrame(0);
				item.SetInfo(_AL("radio"));
				result += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			}
		}
		else if (keyword.Find(_AL("anim"))>=0)
		{
			ACString strFileInfo = keyword;
			strFileInfo.CutLeft(5);
			strFileInfo.CutRight(1);
			CSplit s = strFileInfo;
			CSPLIT_STRING_VECTOR vec = s.Split(_AL(","));
			if (vec.size()==2)
			{
				int iSet = vec[0].ToInt();
				int iIndex = vec[1].ToInt();
				EditBoxItemBase item(enumEIEmotion);
				item.SetInfo(MarshalEmotionInfo(iSet, iIndex));
				result += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			}
			else
			{
#ifndef _DEBUG
				AUI_ReportError(__LINE__, 1,"CDlgTask::FormatTaskText, animation %s info error!", AC2AS(keyword));
#endif
				continue;
			}
		}
		else if (keyword.Find(_AL("location"))>=0)//&location(÷‹“ªœ…,300,250,240,1)&
		{
			ACString strFileInfo = keyword;
			strFileInfo.CutLeft(9);
			strFileInfo.CutRight(1);
			CSplit s = strFileInfo;
			CSPLIT_STRING_VECTOR vec = s.Split(_AL(","));
			if (vec.size()==5)
			{
				EditBoxItemBase item(enumEICoord);
				item.SetName(vec[0]);
				ACString strPos;
				strPos = vec[1]+_AL(" ")+vec[2]+_AL(" ")+vec[3]+_AL(" ")+vec[4];
				item.SetInfo(strPos);
				item.SetColor(A3DCOLORRGB(0, 255, 0));
				result += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			}
			else
			{
#ifndef _DEBUG
				AUI_ReportError(__LINE__, 1,"CDlgTask::FormatTaskText, location %s info error!", AC2AS(vec[0]));
#endif
				continue;
			}
		}
		i++;
	}
	AUICTranslate trans;
	return trans.Translate(result);
}