#include "AFI.h"

#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"

#include "AUI\\AUICommon.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUISubDialog.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUIRadioButton.h "
#include "aui\\CSplit.h"

#include "DlgProclaim.h"
#include "DlgGuildMain.h"
#include "DlgGuildMan.h"
#include "DlgTask.h"
#include "DlgMiniMap.h"

#include "EC_Faction.h"
#include "EC_Instance.h"
#include "EC_World.h"
#include "EC_IvtrItem.h"
#include "EC_AutoMove.h"
#include "EC_HostPlayer.h"
#include "EC_TaskInterface.h"
#include "TaskProcess.h"
#include "TaskTempl.h"
#include "TaskTemplMan.h"

#include "combattop_re.hpp"
#include "siegeinfoget_re.hpp"
#include "getfactiondynamic_re.hpp"
#include "factiondynamic.h"
#include "getfacacttoptable_re.hpp"
#include "elementdataman.h"
#include "Network\\ids.hxx"
#include "getkdctoken_re.hpp"
#include "EL_Region.h"
#include "EC_TaskInterface.h"
#include "TaskProcess.h"
#include "TaskTempl.h"
#include "TaskTemplMan.h"

AUI_BEGIN_COMMAND_MAP(CDlgGuildInfoSubItem, CDlgBase)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgGuildInfoSubItem, CDlgBase)
AUI_ON_EVENT("*",					WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("NULL",				WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("*",					WM_MOUSEWHEEL,	OnEventMouseWheel)
AUI_ON_EVENT(NULL,					WM_MOUSEWHEEL,	OnEventMouseWheel)
AUI_END_EVENT_MAP()

CDlgGuildInfoSubItem::CDlgGuildInfoSubItem()
{

}
CDlgGuildInfoSubItem::~CDlgGuildInfoSubItem()
{

}
bool CDlgGuildInfoSubItem::OnInitDialog()
{
	m_pTxt_Description = (AUILabel* )GetDlgItem("Lab_Description");
	m_pDlgGuildInfoSubMan =  (CDlgGuildInfoSub*)GetGameUIMan()->GetDialog("Win_Guild_Sub");

	m_nItemID = -1;
	return true;
}

bool CDlgGuildInfoSubItem::Release()
{
	return CDlgBase::Release();
}

void CDlgGuildInfoSubItem::UpDateItem(int nItemID)
{
	m_nItemID = nItemID;
}

void CDlgGuildInfoSubItem::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	CDlgGuildInfoSubItemExpand *pDlgAchivItemExpand = (CDlgGuildInfoSubItemExpand*)GetGameUIMan()->GetDialog("Win_Guild_InfoL");
	if(pDlgAchivItemExpand)
	{
		m_pDlgGuildInfoSubMan->ExpandItem(this, pDlgAchivItemExpand != this);
	}

}

void CDlgGuildInfoSubItem::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_Guild_Info")->GetDlgItem("Sub_01");
	if(pSubD)
		pSubD->OnDlgItemMessage(WM_MOUSEWHEEL, wParam, lParam);
}

void CDlgGuildInfoSubItem::UpDateItemText(ACString szText)
{
	m_pTxt_Description->SetText(szText);
}

void CDlgGuildInfoSubItem::SetSubGuildInfo(void* pData,int nType,ACString szTime)
{
	if(!pData)
	{
		return;
	}
	ACString szText;
	ACString szTextContent;
	ACString szTextTime;

	switch (nType)
	{
	case FAC_DYNAMIC::SIEGE_READY:
		{
			FAC_DYNAMIC::siege_ready *pDat = (FAC_DYNAMIC::siege_ready*)pData;
			ACString  szDefendername;
			ACString szAttackername;
			ACString szAssistname;
			szText = GetStringFromTable(20105);
			const long t = pDat->battletime;
			tm *gtime = localtime((const time_t*)&t);
			tm dTime = *gtime;
			tm tNow = GetGame()->GetServerLocalTime();
			szTextTime.Format(GetStringFromTable(20111),dTime.tm_year+ 1900,dTime.tm_mon+1,dTime.tm_mday);		// 时间
			if(pDat->self_role == FAC_DYNAMIC::siege_ready::DEFENDER)
			{
				szAttackername =  ACString((ACHAR*)pDat->attackername, pDat->attackernamesize/sizeof(ACHAR));
				szTextContent.Format(GetStringFromTable(20107),szAttackername,szTextTime);
			}
			else if(pDat->self_role == FAC_DYNAMIC::siege_ready::ATTACKER)
			{
				szDefendername=  ACString((ACHAR*)pDat->defendername, pDat->defendernamesize/sizeof(ACHAR));
				szAssistname =  ACString((ACHAR*)pDat->assistname, pDat->assistnamesize/sizeof(ACHAR));
				szTextContent.Format(GetStringFromTable(20106),szDefendername,szTextTime,szAssistname);
			}
			else if(pDat->self_role == FAC_DYNAMIC::siege_ready::ASSISTANT)
			{
				szDefendername=  ACString((ACHAR*)pDat->defendername, pDat->defendernamesize/sizeof(ACHAR));
				szAttackername =  ACString((ACHAR*)pDat->attackername, pDat->attackernamesize/sizeof(ACHAR));
				szTextContent.Format(GetStringFromTable(20108),szTextTime,szAttackername,szDefendername);
			}
		}
		break;
	case FAC_DYNAMIC::TERRI_READY:
		{
			FAC_DYNAMIC::terri_ready *pDat = (FAC_DYNAMIC::terri_ready*)pData;
			szText = GetStringFromTable(20105);
			const long t = pDat->battletime;
			tm *gtime = localtime((const time_t*)&t);
			tm dTime = *gtime;
			tm tNow = GetGame()->GetServerLocalTime();
			szTextTime.Format(GetStringFromTable(20111),dTime.tm_year + 1900,dTime.tm_mon+1,dTime.tm_mday);		// 时间

			CECInstance *pInstance = GetGameRun()->GetInstance(pDat->tid);
			if(pInstance)
			{
				szText.Format(GetStringFromTable(20104),pInstance->GetName());
				szTextContent.Format(GetStringFromTable(20108),szTextTime,pInstance->GetName());
			}
		}
		break;
	case FAC_DYNAMIC::MEMBER_CHANGE:
		{
			FAC_DYNAMIC::member_change *pDat = (FAC_DYNAMIC::member_change*)pData;
			ACString rolename =  ACString((ACHAR*)pDat->rolename, pDat->rolenamesize/sizeof(ACHAR));
			ACString familyname = ACString((ACHAR*)pDat->familyname, pDat->familynamesize/sizeof(ACHAR)) ;
			if(pDat->type == FAC_DYNAMIC::member_change::MEMBER_JOIN)
			{
				szText.Format(GetStringFromTable(20102),rolename,familyname);
			}
			else if(pDat->type == FAC_DYNAMIC::member_change::MEMBER_LEAVE)
			{
				szText.Format(GetStringFromTable(20103),rolename,familyname);
			}
			else if(pDat->type == FAC_DYNAMIC::member_change::FAMILY_JOIN)
			{
				szText.Format(GetStringFromTable(20102),rolename,familyname);
			}
			else if(pDat->type == FAC_DYNAMIC::member_change::FAMILY_LEAVE)
			{
				szText.Format(GetStringFromTable(20103),familyname,rolename);
			}
			szTextContent = szText;
		}
		break;
	case FAC_DYNAMIC::FAC_PK:
		{
			FAC_DYNAMIC::fac_pk *pDat = (FAC_DYNAMIC::fac_pk*)pData;
			
			szText = GetStringFromTable(20105);
			const long t = pDat->battletime;
			tm *gtime = localtime((const time_t*)&t);
			tm dTime = *gtime;
			tm tNow = GetGame()->GetServerLocalTime();
			szTextTime.Format(GetStringFromTable(20111),dTime.tm_year + 1900,dTime.tm_mon+1,dTime.tm_mday);		// 时间
			if(pDat->nType == 1)
			{
				szText.Format(GetStringFromTable(20100), ACString((ACHAR*)pDat->enemyname, pDat->enemynamesize/sizeof(ACHAR)));

				szTextContent.Format(GetStringFromTable(20110),ACString((ACHAR*)pDat->enemyname, pDat->enemynamesize/sizeof(ACHAR)),szTextTime);
			}
			else
			{
				szText.Format(GetStringFromTable(20131), ACString((ACHAR*)pDat->enemyname, pDat->enemynamesize/sizeof(ACHAR))); 

				szTextContent.Format(GetStringFromTable(20132),ACString((ACHAR*)pDat->enemyname, pDat->enemynamesize/sizeof(ACHAR)),szTextTime);
			}
		}
		break;
	case FAC_DYNAMIC::CHSH_DONATE:
		{
			FAC_DYNAMIC::cash_donate *pDat = (FAC_DYNAMIC::cash_donate*)pData;
			ACString rolename =  ACString((ACHAR*)pDat->rolename, pDat->rolenamesize/sizeof(ACHAR));
			szText.Format(GetStringFromTable(20117), rolename,pDat->cash);

			szTextContent.Format(GetStringFromTable(20117),rolename,pDat->cash);
		}
		break;
	case FAC_DYNAMIC::PUT_AUCTION_ITEM:
		{
			FAC_DYNAMIC::put_auction_item *pDat = (FAC_DYNAMIC::put_auction_item*)pData;
			UpDateItem(pDat->itemid);
			ACString szDayTime;
			ACString szHourTime;
			ACString szMinTime;
			ACString szSecTime;
			ACString szAllTextTime;
			int nCountTime =  pDat->last_time;
			if(nCountTime > (3600 * 24))				// 天
			{
				szDayTime.Format(GetStringFromTable(20237),nCountTime / (3600 * 24));
				szHourTime.Format(GetStringFromTable(20238),(nCountTime % (3600 * 24))/3600);
				szAllTextTime.Format(GetStringFromTable(20236),szDayTime + szHourTime);
			}
			else if(nCountTime > 3600)					// 小时
			{
				szHourTime.Format(GetStringFromTable(20238),nCountTime / 3600);
				szMinTime.Format(GetStringFromTable(20239), (nCountTime/60)%60);
				szAllTextTime.Format(GetStringFromTable(20236),szHourTime + szMinTime);	
			}
			else
			{
				szHourTime.Format(GetStringFromTable(20239),nCountTime / 60);
				szSecTime.Format(GetStringFromTable(20240),nCountTime % 60);
				szAllTextTime.Format(GetStringFromTable(20236),szHourTime + szSecTime);	
			}
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(pDat->itemid, 0,1);
			szText.Format(GetStringFromTable(20118), pItem->GetName(),szTextTime);
			ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
			ATaskTempl*pTask = pMan->GetTaskTemplByID(pDat->task_id);
			if(pTask)
			{
				ACString rolename =  ACString((ACHAR*)pDat->rolename, pDat->rolenamesize/sizeof(ACHAR));
				szTextContent.Format(GetStringFromTable(20119),szText,rolename,pTask->GetName(),pItem->GetName(),szAllTextTime);
			}
		
			delete pItem;
		}
		break;
	case  FAC_DYNAMIC::WIN_AUCTION_ITEM:
		{
			FAC_DYNAMIC::win_auction_item *pDat = (FAC_DYNAMIC::win_auction_item*)pData;
			UpDateItem(pDat->itemid);
			ACString rolename =  ACString((ACHAR*)pDat->rolename, pDat->rolenamesize/sizeof(ACHAR));
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(pDat->itemid, 0,1);
			szText.Format(GetStringFromTable(20126), rolename,pItem->GetName());
			szTextContent.Format(GetStringFromTable(20127),rolename,pDat->cost,pItem->GetName());
			delete pItem;
		}
		break;
	case  FAC_DYNAMIC::BUY_AUCTION_ITEM:
		{
			FAC_DYNAMIC::buy_auction_item *pDat = (FAC_DYNAMIC::buy_auction_item*)pData;
			UpDateItem(pDat->itemid);
			ACString rolename =  ACString((ACHAR*)pDat->rolename, pDat->rolenamesize/sizeof(ACHAR));
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(pDat->itemid, 0,1);
			szText.Format(GetStringFromTable(20124), rolename,pItem->GetName());
			ACString szDayTime;
			ACString szHourTime;
			ACString szMinTime;
			ACString szSecTime;
			ACString szAllTextTime;
			int nCountTime =  pDat->last_time;
			if(nCountTime > (3600 * 24))				// 天
			{
				szDayTime.Format(GetStringFromTable(20237),nCountTime / (3600 * 24));
				szHourTime.Format(GetStringFromTable(20238),(nCountTime % (3600 * 24))/3600);
				szAllTextTime.Format(GetStringFromTable(20236),szDayTime + szHourTime);
			}
			else if(nCountTime > 3600)					// 小时
			{
				szHourTime.Format(GetStringFromTable(20238),nCountTime / 3600);
				szMinTime.Format(GetStringFromTable(20239), (nCountTime/60)%60);
				szAllTextTime.Format(GetStringFromTable(20236),szHourTime + szMinTime);	
			}
			else
			{
				szHourTime.Format(GetStringFromTable(20239),nCountTime / 60);
				szSecTime.Format(GetStringFromTable(20240),nCountTime % 60);
				szAllTextTime.Format(GetStringFromTable(20236),szHourTime + szSecTime);	
			}
			szTextContent.Format(GetStringFromTable(20125),rolename,pItem->GetName(),szAllTextTime);
			delete pItem;
		}
		break;
	case FAC_DYNAMIC::BUILDING_UPGRADE:
		{
			DATA_TYPE DataType;
			FAC_DYNAMIC::building_upgrade *pDat = (FAC_DYNAMIC::building_upgrade*)pData;
			ACString rolename =  ACString((ACHAR*)pDat->rolename, pDat->rolenamesize/sizeof(ACHAR));
			BUILDING_ESSENCE*m_pDBEssence = (BUILDING_ESSENCE *)GetGame()->GetElementDataMan()->
				get_data_ptr(pDat->tid, ID_SPACE_ESSENCE, DataType);
			szText.Format(GetStringFromTable(20128), rolename,m_pDBEssence->name,pDat->level);
			ACString szTemp;
			if(pDat->cost_grass > 0)
			{
				szTemp.Format(GetStringFromTable(20225),pDat->cost_grass);
				szTemp += _AL("\r");
			}
			if(pDat->cost_mine > 0)
			{
				szTemp.Format(GetStringFromTable(20227),pDat->cost_mine);
				szTemp += _AL("\r");
			}
			if(pDat->cost_monster_food > 0)
			{
				szTemp.Format(GetStringFromTable(20229),pDat->cost_monster_food);
				szTemp += _AL("\r");
			}
			if(pDat->cost_monster_core > 0)
			{
				szTemp.Format(GetStringFromTable(20231),pDat->cost_monster_core);
				szTemp += _AL("\r");
			}
			szTextContent.Format(GetStringFromTable(20129), rolename,m_pDBEssence->name,pDat->level);
			szTextContent += _AL("^00ff00") + szTemp;
		}
		break;
	case FAC_DYNAMIC::BUILDING_COMPLETE:
		{
			DATA_TYPE DataType;
			FAC_DYNAMIC::building_complete *pDat = (FAC_DYNAMIC::building_complete*)pData;
			BUILDING_ESSENCE*m_pDBEssence = (BUILDING_ESSENCE *)GetGame()->GetElementDataMan()->
				get_data_ptr(pDat->tid, ID_SPACE_ESSENCE, DataType);
			szText.Format(GetStringFromTable(20130), m_pDBEssence->name,pDat->level);
			szTextContent.Format(GetStringFromTable(20130), m_pDBEssence->name,pDat->level);
		}
		break;
	//case :
	//	{
	//		UpDateItem(id);
	//	}
	//	break;
	}
	ACString szTemp = _AL("\r\n");
	szTextContent  += szTemp + szTime;
	m_szGuilInfo = szTextContent;
	UpDateItemText(szText);
}

AUI_BEGIN_COMMAND_MAP(CDlgGuildInfoSubItemExpand, CDlgBase)
AUI_ON_COMMAND("Btn_Link", OnCommmandLink)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgGuildInfoSubItemExpand, CDlgBase)
AUI_ON_EVENT("*",	WM_LBUTTONDOWN,	OnEventLButtonDownText)

AUI_ON_EVENT("NULL",	WM_MOUSEMOVE,	OnEventMouseMoveText)
AUI_END_EVENT_MAP()

CDlgGuildInfoSubItemExpand::CDlgGuildInfoSubItemExpand()
{

}
CDlgGuildInfoSubItemExpand::~CDlgGuildInfoSubItemExpand()
{

}
bool CDlgGuildInfoSubItemExpand::OnInitDialog()
{
	m_pTxt = (PAUITEXTAREA)GetDlgItem("Txt_1");
	m_Btn_Link = (PAUISTILLIMAGEBUTTON )GetDlgItem("Btn_Link");
	m_pImg_Item = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");

	return CDlgGuildInfoSubItem::OnInitDialog();
}

extern ACString g_AutoMoveDestName;

void CDlgGuildInfoSubItemExpand::OnEventLButtonDownText(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);

	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	if( stricmp(pObj->GetName(),"Img_Item") == 0)
	{
		return;
	}
	CDlgTask::GetItemLinkItemOn(x, y, pObj, &Item);

	if( Item.m_pItem != NULL )
	{
		if( Item.m_pItem->GetType() == enumEICoord )
		{
			A3DVECTOR3 vec;
			int idInstance;
			a_sscanf(Item.m_pItem->GetInfo(), _AL("%f%f%f%d"), 
				&vec.x, &vec.y, &vec.z, &idInstance);
			{
				CDlgMiniMap* pDlgMiniMap = (CDlgMiniMap*)GetGameUIMan()->GetDialog("Win_MiniMap");
				pDlgMiniMap->m_dwTempTargetTime = GetTickCount();
				pDlgMiniMap->m_vecTempTarget = vec;
				pDlgMiniMap->m_idTempTargetInstance = idInstance;

				g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, idInstance);
				g_AutoMoveDestName = Item.m_pItem->GetName();
			}
		}
	}
	else
	{
		CDlgGuildInfoSubItem::OnEventLButtonDown(wParam,lParam,pObj);
	}
}

void CDlgGuildInfoSubItemExpand::OnEventMouseMoveText(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);

	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	CDlgTask::GetItemLinkItemOn(x, y, pObj, &Item);
}

void CDlgGuildInfoSubItemExpand::UpdateTextArea(AUITextArea* pTxt)
{
	if (!pTxt)
		return;

	ACString strText;
	ACString strOrg = pTxt->GetText();
	while(strOrg.GetLength()>0)
	{
		if ( strOrg.GetLength()<(int)a_strlen(_AL("&location")) )
		{
			strText += strOrg;
			break;
		}
		else if ( strOrg.Left(a_strlen(_AL("&location")))==_AL("&location") )
		{
			int i;
			ACString strInfo;
			for (i=1; i<strOrg.GetLength(); ++i)
			{
				if (strOrg[i]==_AL('&'))
				{
					strInfo = strOrg.Mid(a_strlen(_AL("&location")), i-a_strlen(_AL("&location")));
					break;
				}
			}

			if (i==strOrg.GetLength())
			{
				strText += strOrg;
				break;
			}
			else
			{
				strInfo.CutLeft(1);
				strInfo.CutRight(1);
				CSplit s = strInfo;
				CSPLIT_STRING_VECTOR vec = s.Split(_AL(","));
				if (vec.size()==5)
				{
					EditBoxItemBase item(enumEICoord);
					item.SetName(vec[0]);
					ACString strPos;
					strPos = vec[1]+_AL(" ")+vec[2]+_AL(" ")+vec[3]+_AL(" ")+vec[4];
					item.SetInfo(strPos);
					item.SetColor(A3DCOLORRGB(0, 255, 0));
					strText += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
					strOrg.CutLeft(i+1);
				}
				else
				{
					strText += strOrg[0];
					strOrg.CutLeft(1);
					continue;
				}
			}
		}
		else
		{
			strText += strOrg[0];
			strOrg.CutLeft(1);
		}
	}

	pTxt->SetText(strText);
}

void CDlgGuildInfoSubItemExpand::SetSubGuildInfoContent(ACString szContent,int nItemID)
{
	m_pTxt->Show(false);
	m_Btn_Link->Show(false);
	m_pImg_Item->Show(false);
	
	UpDateContent(szContent);
	if(nItemID != -1)
	{
		UpDateItemShow(nItemID);
	}
}

void CDlgGuildInfoSubItemExpand::UpDateContent(ACString szText)
{
	m_pTxt->SetText(szText);
	UpdateTextArea(m_pTxt);
	m_pTxt->Show(true);
}

void CDlgGuildInfoSubItemExpand::UpDateItemShow(int nItemID)
{
	CECIvtrItem* pItem = CECIvtrItem::CreateItem(nItemID, 0,1);
	AString strFile;
	AUICTranslate trans;
	if (pItem )
	{
		pItem->SetNeedUpdate(false);
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		m_pImg_Item->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
			GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
		m_pImg_Item->SetCover(NULL, 0, 1);
		m_pImg_Item->SetHint(trans.Translate(pItem->GetDesc()));
		m_pImg_Item->Show(true);
		delete pItem;
	}
}

void CDlgGuildInfoSubItemExpand::OnCommmandLink(const char* szCommand)
{

}

AUI_BEGIN_COMMAND_MAP(CDlgGuildInfoSub, CDlgBase)
AUI_END_COMMAND_MAP()
CDlgGuildInfoSub::CDlgGuildInfoSub()
{

}
CDlgGuildInfoSub::~CDlgGuildInfoSub()
{
	m_vecDlgItems.clear();
	int nSize = m_vecSubDialogs.size();
	for (int i=0; i<nSize; i++)
	{
		DeleteControl(m_vecSubDialogs[i]);
	}
	m_vecSubDialogs.clear();
}
bool CDlgGuildInfoSub::OnInitDialog()
{
	m_vecDlgItems.clear();
	m_vecSubDialogs.clear();
	m_pSubItem = (PAUISUBDIALOG)GetDlgItem("Sub_1");
	m_rcWindowOld = GetGameUIMan()->GetRect();
	return true;
}
bool CDlgGuildInfoSub::Release()
{
	int i;
	int nSize = m_vecSubDialogs.size();
	for (i=0; i<nSize; i++)
	{
		DeleteControl(m_vecSubDialogs[i]);
	}
	m_vecSubDialogs.clear();
	//创建的界面uiman统一管理，不必单独destroy
	m_vecDlgItems.clear();

	return CDlgBase::Release();
}
bool CDlgGuildInfoSub::Render()
{
	return AUIDialog::Render();
}

bool CDlgGuildInfoSub::ExpandItem(CDlgGuildInfoSubItem* pDlg, bool bExpand)
{
	UINT index = 0;
	UINT i;
	UINT nSize = m_vecDlgItems.size();
	m_pDlgItemExpand = (CDlgGuildInfoSubItemExpand*)GetGameUIMan()->GetDialog("Win_Guild_InfoL");
	if (bExpand)
	{
		for (i=0; i<nSize; i++)
		{
			if (m_vecDlgItems[i] == pDlg)
			{
				index = i;
				break;
			}
		}
		m_pDlgItemExpand->SetSubGuildInfoContent(pDlg->m_szGuilInfo,pDlg->m_nItemID);
		// 这次show作用：调整这个界面大小
		m_pDlgItemExpand->Show(true);
	}
	else
	{
		for (i=0; i<nSize; i++)
		{
			if (m_vecSubDialogs[i]->GetSubDialog() == pDlg)
			{
				index = i;
				break;
			}
		}
	}
	if (index >= m_vecDlgItems.size())
	{
		return false;
	}
	bool bReset = false;
	int w = GetSize().cx;
	int h = m_pSubItem->GetSize().cy*nSize;
	int hDelta = m_pDlgItemExpand->GetSize().cy-m_pSubItem->GetSize().cy;
	int x = m_vecSubDialogs[i]->GetPos(true).x;
	int y=m_vecSubDialogs[0]->GetPos(true).y;
	if (bExpand)
	{
		for (i=0; i<nSize; i++)
		{
			if (i==index)
			{
				m_vecSubDialogs[i]->SetDialog(m_pDlgItemExpand);
				m_vecSubDialogs[i]->SetSize(m_pSubItem->GetSize().cx, m_pSubItem->GetSize().cy+hDelta);
				m_vecSubDialogs[i]->SetPos(x, y);
				y += m_pDlgItemExpand->GetSize().cy;
			}
			else
			{
				m_vecSubDialogs[i]->SetDialog(m_vecDlgItems[i]);
				m_vecSubDialogs[i]->SetSize(m_pSubItem->GetSize().cx, m_pSubItem->GetSize().cy);
				m_vecSubDialogs[i]->SetPos(x, y);
				y += m_pSubItem->GetSize().cy;
			}
		}
		// setdialog后原dialog会不显示，这里将其强制显示
		m_pDlgItemExpand->Show(true);
		h += hDelta;
	}
	else
	{
		for (i=nSize-1; i>index; i--)
		{
			int x = m_vecSubDialogs[i]->GetPos(true).x;
			int y = m_vecSubDialogs[i]->GetPos(true).y-m_pDlgItemExpand->GetSize().cy+m_pSubItem->GetSize().cy;
			m_vecSubDialogs[i]->SetPos(x, y);
		}
		if(nSize-1 == index)
		{
			y = 0;
			for (i=0; i<nSize; i++)
			{
				m_vecSubDialogs[i]->SetDialog(m_vecDlgItems[i]);
				m_vecSubDialogs[i]->SetSize(m_pSubItem->GetSize().cx, m_pSubItem->GetSize().cy);
				m_vecSubDialogs[i]->SetPos(x, y);
				y += m_pSubItem->GetSize().cy;
			}
			bReset = true;
		}
		m_vecSubDialogs[index]->SetDialog(m_vecDlgItems[index]);
		m_vecSubDialogs[index]->SetSize(m_pSubItem->GetSize().cx, m_pSubItem->GetSize().cy);
	}

	SetSize(w, h);

	PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_Guild_Info")->GetDlgItem("Sub_01");
	pSubD->SetSubDialogOrgSize(w, h, bReset);
	pSubD->UpdateViewport();
	return true;
}

void CDlgGuildInfoSub::InitItem(void* pData)
{
	if(pData == NULL)
	{
		return;
	}
	int i;
	int nSize = m_vecSubDialogs.size();
	//先删掉控件，再删对应的dialog
	for (i=0; i<nSize; i++)
	{
		DeleteControl(m_vecSubDialogs[i]);
	}
	m_vecSubDialogs.clear();
	nSize = m_vecDlgItems.size();
	for (i=0; i<nSize; i++)
	{
		GetGameUIMan()->DestroyDialog(m_vecDlgItems[i]->GetName());
	}
	m_vecDlgItems.clear();
	int y=0;
	A3DRECT	rcNew = GetGameUIMan()->GetRect();


	GNET::GetFactionDynamic_Re* p = (GNET::GetFactionDynamic_Re*)pData;

	int iSize = p->totalsize;
	for (int i=iSize-1; i>=0; i--)
	{
		GNET::Octets octets = p->dynamic[i];
		char *data = (char*)octets.begin();
		FAC_DYNAMIC::dynamic_header *pDynamicHeader = (FAC_DYNAMIC::dynamic_header*)data;
		ACString strTime;
		const long t = pDynamicHeader->time;
		tm *gtime = localtime((const time_t*)&t);
		tm dTime = *gtime;
		tm tNow = GetGame()->GetServerLocalTime();
		//if (tNow.tm_mday == dTime.tm_mday)
		//{
		//	strTime =GetStringFromTable(8606);
		//}
		//else if (tNow.tm_mday == (dTime.tm_mday+1))
		//{
		//	strTime = GetStringFromTable(8607);
		//}
		//else
		//{
		//	strTime.Format(GetStringFromTable(8608),dTime.tm_mon+1,dTime.tm_mday);
		//}//
		strTime.Format(GetStringFromTable(20111),dTime.tm_year+ 1900,dTime.tm_mon+1,dTime.tm_mday);	
		GetGameUIMan()->CreateDlg(GetGame()->GetA3DEngine(), GetGame()->GetA3DDevice(), "Guild_InfoS.xml");
		CDlgGuildInfoSubItem* pDlg = (CDlgGuildInfoSubItem*)GetGameUIMan()->GetDialog("Win_Guild_InfoS");
		AString strName;
		strName.Format("Win_Guild_InfoS%d", i);
		pDlg->SetName(strName);
		//pDlg->UpDateItem();
		pDlg->Resize(m_rcWindowOld, rcNew);
		m_vecDlgItems.push_back(pDlg);
		ACString szText;
		data = data + sizeof(FAC_DYNAMIC::dynamic_header);
		pDlg->SetSubGuildInfo(data,pDynamicHeader->action,strTime);
		PAUISUBDIALOG pSubItem = (PAUISUBDIALOG)CloneControl(m_pSubItem);
		strName.Format("Sub_Clone%d", i);
		pSubItem->SetName(strName.GetBuffer(0));
		pSubItem->SetSize(m_pSubItem->GetSize().cx, m_pSubItem->GetSize().cy);//pDlg->GetSize().cx,pDlg->GetSize().cy);
		pSubItem->SetDialog(pDlg);
		pSubItem->SetPos(0,y);
		m_vecSubDialogs.push_back(pSubItem);
		y += pSubItem->GetSize().cy;
	}
	int w = GetSize().cx;
	int h = y;
	SetSize(w, h);

	PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_Guild_Info")->GetDlgItem("Sub_01");
	pSubD->SetSubDialogOrgSize(w, h);
}

const int honorlevel[] =
{
	0,
	5000,
	20000,
	60000,
	120000,
	300000,
	INT_MAX
};

const int terrscorelevel[] =
{
	0,
	10,
	20,
	40,
	70,
	100,
	130,
	160,
	190,
	230,
	270,
	310,
	350,
	390,
	760,
	INT_MAX
};

AUI_BEGIN_COMMAND_MAP(CDlgGuildInfo, CDlgBase)

AUI_ON_COMMAND("Edit", OnCommand_Editor)
AUI_ON_COMMAND("Btn_GuildWar", OnCommand_GetRankAward)
AUI_ON_COMMAND("Btn_GetMoney", OnCommand_getmoney)

AUI_END_COMMAND_MAP()
CDlgGuildInfo::CDlgGuildInfo()
{

}

CDlgGuildInfo::~CDlgGuildInfo()
{

}

bool CDlgGuildInfo::OnInitDialog()
{
	m_pImg_GuildIcon= (AUIImagePicture*)GetDlgItem("Img_GuildIcon");
	m_pImg_Icon01= (AUIImagePicture*)GetDlgItem("Img_Icon01");
	m_pImg_Icon02= (AUIImagePicture*)GetDlgItem("Img_Icon02");

	m_pPrg_GuildHonor= (PAUIPROGRESS)GetDlgItem("Pro_GuildHonor");
	m_pPrg_LandOccupy= (PAUIPROGRESS)GetDlgItem("Pro_LandOccupy");

	m_pTxt_GuildLevel= (AUILabel*)GetDlgItem("Txt_GuildLevel");
	m_pTxt_GuildProsperity= (AUILabel*)GetDlgItem("Txt_GuildProsperity");
	m_pTxt_PplCap= (AUILabel*)GetDlgItem("Txt_PplCap");
	m_pTxt_PplOnline= (AUILabel*)GetDlgItem("Txt_PplOnline");
	m_pTxt_GuildHonor= (AUILabel*)GetDlgItem("Txt_GuildHonor");



	m_pTxt_LandOccupy= (AUILabel*)GetDlgItem("Txt_LandOccupy");
	m_pTxt_GetGuildwarMoney= (AUILabel*)GetDlgItem("Txt_GetGuildwarMoney");
	//m_pTxt_Gold= (AUILabel*)GetDlgItem("Txt_Gold");
	m_pTxt_Money= (AUILabel*)GetDlgItem("Txt_Money");
	//m_pTxt_GuildWarInfo= (AUILabel*)GetDlgItem("Txt_GuildWarInfo");

	//m_pTxt_GuildWarTime= (AUILabel*)GetDlgItem("Txt_GuildWarTime");
	//m_pTxt_GuildWarStart= (AUILabel*)GetDlgItem("Txt__GuildWarStart");
	m_pTxt_DefenderGuild= (AUILabel*)GetDlgItem("Lab_Name_DefenderGuild");
	m_pTxt_Defender= (AUILabel*)GetDlgItem("Lab_Name_Defender");
	m_pTxt_DefenderName = (AUILabel*)GetDlgItem("Lab_InfoB");

	m_pTxt_Message = (PAUITEXTAREA)GetDlgItem("Txt_Message");
	m_pTxt_GuildHonor->SetAlign(AUIFRAME_ALIGN_CENTER);


	m_pTxt_GuildLevel->SetText(_AL("0"));
	m_pTxt_GuildProsperity->SetText(_AL("0"));
	m_pTxt_PplCap->SetText(_AL("0"));
	m_pTxt_PplOnline->SetText(_AL("0"));
	m_pTxt_GuildHonor->SetText(_AL("0"));



	m_pTxt_LandOccupy->SetText(_AL("0"));
	m_pTxt_GetGuildwarMoney->SetText(_AL("0"));
	//m_pTxt_Gold->SetText(_AL("0"));
	m_pTxt_Money->SetText(_AL("0"));
	//m_pTxt_GuildWarInfo->SetText(_AL("0"));

	//m_pTxt_GuildWarTime->SetText(_AL("0"));
	//m_pTxt_GuildWarStart->SetText(_AL("0"));
	m_pTxt_DefenderGuild->SetText(_AL("--"));
	m_pTxt_Defender->SetText(_AL("--"));
	//m_pTxt_DefenderName->SetText(_AL("--"));
	return true;
}

void CDlgGuildInfo::OnShowDialog()
{
	int nIndex = 0;
	A2DSprite *sprite = GetGameUIMan()->GetFactionIcon(GetGame()->GetGameInit().iServerID, GetHostPlayer()->GetFactionID(), &nIndex);
	m_pImg_GuildIcon->SetCover(sprite, nIndex);

	int idFaction = GetHostPlayer()->GetFactionID();
	CECFactionMan *pFMan = GetGame()->GetFactionMan();
	if(pFMan)
	{
		Faction_Info *pInfo = pFMan->GetFaction(idFaction);
		ACString strOnlineText;
		if(pInfo)
		{	
			ACString strText;
			strText.Format(_AL("%d"), pInfo->GetLevel() + 1);
			m_pTxt_GuildLevel->SetText(strText);

			strText.Format(_AL("%d"), pInfo->GetProsperity());
			m_pTxt_GuildProsperity->SetText(strText);

			strText.Format(_AL("%d/%d"), pFMan->GetFamilyList().size(), pInfo->GetLevel() * 2 + 5);
			m_pTxt_PplCap->SetText(strText);

	
			strOnlineText.Format(_AL("%d/%d"), pInfo->GetOnlineMemNum(), pInfo->GetMemNum());
			
			int nHonor = pInfo->GetNimbus();
			int level = 0;
			while( nHonor >= honorlevel[level + 1] )
				level++;
			int nNowHonor = nHonor - honorlevel[level];
			int nMaxHonor = honorlevel[level + 1] - honorlevel[level];
			m_pPrg_GuildHonor->SetProgress(int(double(nNowHonor) / nMaxHonor * AUIPROGRESS_MAX));
			strText.Format(_AL("%d/%d"), nHonor, honorlevel[level + 1]);
			m_pPrg_GuildHonor->SetHint(strText);
			m_pTxt_GuildHonor->SetText(GetStringFromTable(1200 + level));

			int nTerr = pInfo->GetTerritoryScore();
			level = 0;
			while( nTerr >= terrscorelevel[level + 1] )
				level++;
			int nNowTerr = nTerr - terrscorelevel[level];
			int nMaxTerr = terrscorelevel[level + 1] - terrscorelevel[level];
			m_pPrg_LandOccupy->SetProgress(int(double(nNowTerr) / nMaxTerr * AUIPROGRESS_MAX));
			strText.Format(_AL("%d/%d"), nTerr, terrscorelevel[level + 1]);
			m_pPrg_LandOccupy->SetHint(strText);
			m_pTxt_LandOccupy->SetText(GetStringFromTable(1210 + level));

			strText = _AL("");
			strText.Format(GetStringFromTable(16001),pInfo->GetMoney()/10000);
			m_pTxt_GetGuildwarMoney->SetText(strText);

			m_nGuildWarPage = 0;
			GetGameSession()->combat_Top(GetHostPlayer()->GetCharacterID(),
				GetHostPlayer()->GetFactionID(), 0);
			
		}
		else
		{
				strOnlineText = _AL("");
		}
		m_pTxt_PplOnline->SetText(strOnlineText);
	}
	GetGameSession()->war_infoget(1);
	ACString szTemp;
	szTemp.Format(_AL("%d"),GetHostPlayer()->GetFactionMoney());
	m_pTxt_Money->SetText(szTemp);
}

void CDlgGuildInfo::OnTick()
{
	int idFaction = GetHostPlayer()->GetFactionID();
	CECFactionMan *pFMan = GetGame()->GetFactionMan();
	if(pFMan)
	{
		Faction_Info *pInfo = pFMan->GetFaction(idFaction);
		if( 0 != a_strcmp(pFMan->GetProclaim(), m_pTxt_Message->GetText()) )
		{
			ACString strText;
			strText = pFMan->GetProclaim();
			GetGameUIMan()->FilterBadWords(strText);
			m_pTxt_Message->SetText(strText);
		}
	}
}

void CDlgGuildInfo::OnCommand_Editor(const char * szCommand)
{
	ACString strText = m_pTxt_Message->GetText();
	GetGameUIMan()->m_pDlgProclaim->Show(true);
	GetGameUIMan()->m_pDlgProclaim->m_pDEFAULT_Txt_Input->SetText(strText);
	GetGameUIMan()->m_pDlgProclaim->SetData(0);
}

void CDlgGuildInfo::OnCommand_GetRankAward(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_GuildWar");
	if( pDlg )
	{
		PAUILISTBOX pList = (PAUILISTBOX)pDlg->GetDlgItem("List_Member");
		pList->ResetContent();
		pDlg->Show(true);
		m_nGuildWarPage = 0;
		GetGameSession()->combat_Top(GetHostPlayer()->GetCharacterID(),
			GetHostPlayer()->GetFactionID(), 0);
	}
}

void CDlgGuildInfo::OnCommand_getmoney(const char* szCommand)
{
	Faction_Info *pFactionInfo= NULL;
	pFactionInfo = GetGame()->GetFactionMan()->GetFaction(GetHostPlayer()->GetFactionID());
	if (NULL != pFactionInfo && pFactionInfo->GetMoney() > 0)
	{
		GetGameSession()->factionpkraid_withdrawbonus(GetHostPlayer()->GetCharacterID());
	}
}

void CDlgGuildInfo::BuildGuildWarRankList(void *pData)
{
	CombatTop_Re *p = (CombatTop_Re*)pData;
	DWORD i;
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
	pList->ResetContent();
	for(i = 0; i < p->list.size(); i++)
	{
		ACString strName((const ACHAR*)p->list[i].rolename.begin(), p->list[i].rolename.size() / sizeof(ACHAR));
		ACString strText;

		strText.Format(_AL("%s\t%d\t%d"), strName, p->list[i].kills, p->list[i].deaths);
		pList->AddString(strText);
		if( (int)p->list[i].factionid == GetHostPlayer()->GetFactionID() )
			pList->SetItemTextColor(pList->GetCount() - 1, A3DCOLORRGB(167, 205, 255));
		else
			pList->SetItemTextColor(pList->GetCount() - 1, A3DCOLORRGB(255, 0, 0));
	}
	//m_nGuildWarPage = p->handle;
}

void CDlgGuildInfo::UpDateGuildInfo(void* pData)
{
	//m_pPro_Mana->SetProgress(int(double(nNowReputation) / nMaxReputation * AUIPROGRESS_MAX));
	CDlgGuildInfoSub*pDlg = (CDlgGuildInfoSub*)GetGameUIMan()->GetDialog("Win_Guild_Sub");
	pDlg->InitItem(pData);
}

void CDlgGuildInfo::UpdateGuildSiege(void* pData)
{
	SiegeInfoGet_Re* p = (SiegeInfoGet_Re*)pData;
	ACString strText;
	size_t i;
	int fid = 0, fid1 = 0;
	ACString str, str1;
	Faction_Info *pFInfo = NULL;
	int hostFaction = GetHostPlayer()->GetFactionID();
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Member");
	pList->ResetContent();
	for (i = 0; i < p->challengers.size(); ++i)
	{
		const GNET::GChallenger& curChall = p->challengers[i];
		// 过滤掉与本帮派无关的条目
		if (p->city.owner.fid != hostFaction &&
			curChall.challenger.fid != hostFaction &&
			curChall.assistant.fid != hostFaction)
			continue;

		time_t beginTime = curChall.begin_time;
		tm *timeTM = gmtime(&beginTime);

		str = str1 = _AL("---");
		fid = curChall.challenger.fid;
		pFInfo = GetGame()->GetFactionMan()->GetFaction(fid, true);
		if (pFInfo) str = pFInfo->GetName();
		fid1 = curChall.assistant.fid;
		pFInfo = GetGame()->GetFactionMan()->GetFaction(fid1, true);
		if (pFInfo) str1 = pFInfo->GetName();

		// 攻城日期 攻方 辅助方 
		ACString strFormat = GetStringFromTable(1612);
		strText.Format(strFormat + ACString(_AL("\t%s\t%s")),
			timeTM->tm_mon+1, timeTM->tm_mday, str, str1);
		pList->AddString(strText);
		int curIndex = pList->GetCount() - 1;
		pList->SetItemData(curIndex, curChall.begin_time, 0);
		pList->SetItemData(curIndex, fid,	1);
		pList->SetItemData(curIndex, fid1,	2);
	}

	// 今日守城方
	str = _AL("---");
	fid = p->city.owner.fid;
	// 帮派名
	pFInfo = GetGame()->GetFactionMan()->GetFaction(fid, true);
	if (pFInfo) {
		str = pFInfo->GetName();
		fid = 0;
	}
	m_pTxt_DefenderGuild->SetData(fid);
	m_pTxt_DefenderGuild->SetText(str);
	// 帮主名
	str1 = _AL("---");
	fid1 = p->city.owner.master;
	const ACHAR* pszName = GetGame()->GetGameRun()->GetPlayerName(fid1, false);
	if (!pszName && fid1 != 0)
		GetGameSession()->GetPlayerBriefInfo(1, &fid1, 2);
	else { // 以后不需更新
		if (pszName) str1 = pszName;
		fid1 = 0;
	}
	m_pTxt_Defender->SetData(fid1);
	m_pTxt_Defender->SetText(str1);
}


AUI_BEGIN_COMMAND_MAP(CDlgGuildLandCreate, CDlgBase)

AUI_ON_COMMAND("Btn_Create", OnCommand_Create)
AUI_ON_COMMAND("Btn_Prev", OnCommand_Up)
AUI_ON_COMMAND("Btn_Next", OnCommand_Down)
AUI_ON_COMMAND("Btn_Close", OnCommand_Close)
AUI_END_COMMAND_MAP()

CDlgGuildLandCreate::CDlgGuildLandCreate()
{

}
CDlgGuildLandCreate::~CDlgGuildLandCreate()
{

}
bool CDlgGuildLandCreate::OnInitDialog()
{
	m_pTxt_Page = (AUILabel*)GetDlgItem("Txt_Page");

	m_nCurPage = 1;
	m_nMaxPage = 1;
	return true;
}

void  CDlgGuildLandCreate::OnShowDialog()
{
	GetGameSession()->c2s_CmdSendGetFacActTopTable(m_nCurPage - 1);	// 请求帮派列表
}
void CDlgGuildLandCreate::OnCommand_Create(const char * szCommand)
{
	if(GetHostPlayer()->GetFRoleID() == GNET::_R_MASTER)
	{
		GetGameSession()->c2s_CmdSendCreateFacBase();
	}
}

void CDlgGuildLandCreate::UpDateList(void* pData)
{
	GNET::GetFacActTopTable_Re* p = (GNET::GetFacActTopTable_Re*)pData;	
	if(p)
	{
		m_nCurPage = p->pageid  + 1 ;
		m_nMaxPage = p->maxsize / 20;
		if( p->maxsize % 20 > 0)
		{
			m_nMaxPage += 1;
		}
		if(m_nMaxPage == 0)
		{
			m_nMaxPage =1;
		}
		//unsigned int id;
		//Octets name;
		//unsigned int occupation;
		//short oldrank;
		//int64_t value;
		PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("Lst_Guild");
		pList->ResetContent();
		for(int i = 0; i < (int)p->items.size(); i++)
		{
			ACString strName((const ACHAR*)p->items[i].name.begin(), p->items[i].name.size() / sizeof(ACHAR));
			ACString strText;

			strText.Format(_AL("%d\t%s\t%d"),i + p->pageid* 20 + 1, strName, p->items[i].value);
			pList->AddString(strText);
		}
		int nCanCreate = p->can_create_base;
		GetDlgItem("Btn_Create")->Enable((bool)nCanCreate);
	}
	ACString strText;
	strText.Format(_AL(" %d / %d"),m_nCurPage,m_nMaxPage);
	GetDlgItem("Txt_Page")->SetText(strText);
}

void CDlgGuildLandCreate::OnCommand_Up(const char * szCommand)
{
	m_nCurPage--;
	if(m_nCurPage < 1)
	{
		m_nCurPage = 1;
		return;
	}
	GetGameSession()->c2s_CmdSendGetFacActTopTable(m_nCurPage - 1);
}

void CDlgGuildLandCreate::OnCommand_Down(const char * szCommand)
{
	m_nCurPage++;
	if(m_nCurPage > m_nMaxPage)
	{
		m_nCurPage = m_nMaxPage;
		return;
	}
	GetGameSession()->c2s_CmdSendGetFacActTopTable(m_nCurPage - 1);
}

void CDlgGuildLandCreate::OnCommand_Close(const char * szCommand)
{
	Show(false);
}

AUI_BEGIN_COMMAND_MAP(CDlgGuildInputMoney, CDlgBase)
AUI_ON_COMMAND("confirm", OnCommand_OK)
AUI_END_COMMAND_MAP()

CDlgGuildInputMoney::CDlgGuildInputMoney()
{
}

CDlgGuildInputMoney::~CDlgGuildInputMoney()
{

}

bool CDlgGuildInputMoney::OnInitDialog()
{
	return true;
}

void CDlgGuildInputMoney::OnCommand_OK(const char * szCommand)
{
	ACString szMoney = GetDlgItem("DEFAULT_Txt_No")->GetText();
	//if(a_atoi(szMoney) < 100)
	//{
	//	GetGameUIMan()->MessageBox("", GetStringFromTable(20241), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	//	return;
	//}
	if(GetHostPlayer()->GetCash() <  (a_atoi(szMoney) * 100))
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(8397), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	GetGameSession()->c2s_SendCmdFacbaseMoney(a_atoi(szMoney));
	Show(false);
}


AUI_BEGIN_COMMAND_MAP(CDlgGuildBuild, CDlgBase)
AUI_ON_COMMAND("Btn_EnterLand", OnCommand_EnterLand)
AUI_ON_COMMAND("Btn_Donate", OnCommand_Donate)
AUI_ON_COMMAND("Btn_ExitLand", OnCommand_ExitLand)
AUI_ON_COMMAND("Btn_Edit", OnCommand_Edit)
AUI_ON_COMMAND("Btn_GRule", OnCommand_GRule)

AUI_END_COMMAND_MAP()

CDlgGuildBuild::CDlgGuildBuild()
{
	m_bIsEnter = false;
	m_bIsSend = false;
}
CDlgGuildBuild::~CDlgGuildBuild()
{

}

void UpdateTextArea(AUITextArea* pTxt)
{
	if (!pTxt)
		return;

	ACString strText;
	ACString strOrg = pTxt->GetText();
	while(strOrg.GetLength()>0)
	{
		if ( strOrg.GetLength()<(int)a_strlen(_AL("&location")) )
		{
			strText += strOrg;
			break;
		}
		else if ( strOrg.Left(a_strlen(_AL("&location")))==_AL("&location") )
		{
			int i;
			ACString strInfo;
			for (i=1; i<strOrg.GetLength(); ++i)
			{
				if (strOrg[i]==_AL('&'))
				{
					strInfo = strOrg.Mid(a_strlen(_AL("&location")), i-a_strlen(_AL("&location")));
					break;
				}
			}

			if (i==strOrg.GetLength())
			{
				strText += strOrg;
				break;
			}
			else
			{
				strInfo.CutLeft(1);
				strInfo.CutRight(1);
				CSplit s = strInfo;
				CSPLIT_STRING_VECTOR vec = s.Split(_AL(","));
				if (vec.size()==5)
				{
					EditBoxItemBase item(enumEICoord);
					item.SetName(vec[0]);
					ACString strPos;
					strPos = vec[1]+_AL(" ")+vec[2]+_AL(" ")+vec[3]+_AL(" ")+vec[4];
					item.SetInfo(strPos);
					item.SetColor(A3DCOLORRGB(0, 255, 0));
					strText += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
					strOrg.CutLeft(i+1);
				}
				else
				{
					strText += strOrg[0];
					strOrg.CutLeft(1);
					continue;
				}
			}
		}
		else
		{
			strText += strOrg[0];
			strOrg.CutLeft(1);
		}
	}

	pTxt->SetText(strText);
}


bool CDlgGuildBuild::OnInitDialog()
{
	m_pTxt_Grass = (AUILabel*)GetDlgItem("Txt_Item01");
	m_pTxt_Mine= (AUILabel*)GetDlgItem("Txt_Item02");
	m_pTxt_MonsterFood= (AUILabel*)GetDlgItem("Txt_Item03");
	m_pTxt_MonsterCore= (AUILabel*)GetDlgItem("Txt_Item04");
	m_pTxt_Money= (AUILabel*)GetDlgItem("Txt_Item05");
	m_pTxt_GActivity= (AUILabel*)GetDlgItem("Txt_GActivity");
	m_pTxt_Task= (AUILabel*)GetDlgItem("Txt_Task");
	m_pLab_BuildStatus= (AUILabel*)GetDlgItem("Lab_BuildStatus");
	m_pLab_Prgs= (AUILabel*)GetDlgItem("Lab_Prgs");
	m_pIcon_build = (AUIImagePicture	*)GetDlgItem("Img_Icon");
	m_pTxt_Message = (PAUITEXTAREA)GetDlgItem("Txt_Msg");
	m_pImg_WorldMap = (PAUIIMAGEPICTURE)GetDlgItem("Img_Land");
	m_pTaskList = (PAUILISTBOX)GetDlgItem("Lst_Task");
	DATA_TYPE DataType;
	BUILDING_REGION_CONFIG * m_pEssence = (BUILDING_REGION_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(805, ID_SPACE_CONFIG, DataType);
	AString szTemp;
	for(int nIndex = 0; nIndex < 18; nIndex++)
	{
		szTemp.Format("Img_Y%02d", nIndex + 1);
		m_pImg_Icon[nIndex]= (AUIImagePicture	*)GetDlgItem(szTemp);
		m_pImg_Icon[nIndex]->Show(false);
		for(int nBuilIndex = 0; nBuilIndex < 30; nBuilIndex++)
		{
			if(nIndex + 1 == m_pEssence->build_region[nBuilIndex].region_index && m_pEssence->build_region[nBuilIndex].region_index != 0)
			{
				float X = float(800)/512;
				float Y = 1.3;
				m_pImg_Icon[nIndex]->SetPos(m_pEssence->build_region[nBuilIndex].centerx / X,m_pEssence->build_region[nBuilIndex].centery /Y );
				//m_pImg_Icon[nIndex]->SetSize(m_pEssence->build_region[nBuilIndex].width,m_pEssence->build_region[nBuilIndex].length);
				m_pImg_Icon[nIndex]->SetText(m_pEssence->build_region[nBuilIndex].name);
				m_pImg_Icon[nIndex]->Show(true);
				m_pImg_Icon[nIndex]->SetData(m_pEssence->build_region[nBuilIndex].region_id);
				m_pImg_Icon[nIndex]->SetDataPtr((void*)nIndex);
				m_pImg_Icon[nIndex]->SetHint(m_pEssence->build_region[nBuilIndex].name);
				m_pImg_Icon[nIndex]->SetColor(A3DCOLORRGBA(255,255,0,255));
			}
		}
	}	
	AUIOBJECT_SETPROPERTY p;
	strcpy(p.fn,"");
	m_pIcon_build->SetProperty("Image File",&p);
	m_pLab_BuildStatus->SetText(GetStringFromTable(20188));
	m_pLab_Prgs->SetText(_AL(""));
	SetEnterBuild(false);
	m_pTxt_Task->SetText(_AL("0/0"));
	m_pTxt_GActivity->SetText(_AL("0"));
	m_pTxt_Money->SetText(_AL("0"));
	m_pTxt_Grass->SetText(_AL("0"));
	m_pTxt_Mine->SetText(_AL("0"));
	m_pTxt_MonsterFood->SetText(_AL("0"));
	m_pTxt_MonsterCore->SetText(_AL("0"));


	return true;
}

void  CDlgGuildBuild::OnShowDialog()
{
	UpDataBuildInfo();
	GetDlgItem("Btn_Edit")->Enable(false);
	if (GetHostPlayer()->GetFRoleID() == GNET::_R_MASTER
		|| GetHostPlayer()->GetFRoleID() == GNET::_R_VICEMASTER
		|| GetHostPlayer()->GetFRoleID() == GNET::_R_BODYGUARD)
	{
		GetDlgItem("Btn_Edit")->Enable(true);
	}
	if(!GetGame()->GetGuildEnter()&& m_bIsSend == false)
	{
		c2s_SendCmdOueSideFacBaseInfo();
		m_bIsSend = true;
	}
	AString strFile;
	AUICTranslate trans;
	ACString szDesc;
	//AUIOBJECT_SETPROPERTY p;
	DATA_TYPE DataType ;
	A3DRECT rcMiniMap, rcMap;
	BUILDING_REGION_CONFIG *m_pEssence = (BUILDING_REGION_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(805, ID_SPACE_CONFIG, DataType);

	for(int nIndex = 0; nIndex < 18; nIndex++)
	{	
		int nTempID = 0;
		CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfo(nIndex +1);
		if(pInfo)
		{
			BUILDING_ESSENCE* pBuildEssence = (BUILDING_ESSENCE *)GetGame()->GetElementDataMan()->
				get_data_ptr(pInfo->nId, ID_SPACE_ESSENCE, DataType);
			szDesc.Format(GetStringFromTable(20201),pBuildEssence->name,pInfo->nLevel ? pInfo->nLevel : 1,GetStringFromTable(pInfo->nStatus ? 20203 : 20202),pBuildEssence->building_desc);
			m_pImg_Icon[nIndex]->SetHint(trans.Translate(szDesc));
		}
	}
	m_pTaskList->ResetContent();
	int idFaction = GetHostPlayer()->GetFactionID();
	CECFactionMan *pFMan = GetGame()->GetFactionMan();
	if(pFMan)
	{
		Faction_Info *pInfo = pFMan->GetFaction(idFaction);
		ACString strText;
		if(pInfo)
		{
			CECTaskInterface *pTaskInterafce = GetGame()->GetGameRun()->GetHostPlayer()->GetTaskInterface();
			ActiveTaskList* pLst =static_cast<ActiveTaskList*>(pTaskInterafce->GetActiveTaskList());
			ATaskTemplMan *pMan = GetGame()->GetTaskTemplateMan();
			ATaskTempl*pTask = pMan->GetTaskTemplByID(GetHostPlayer()->GetFactionTask_id());
			if(pTask)
			{	
				m_pTaskList->InsertString(m_pTaskList->GetCount(),pTask->GetName());
			}
			if(pMan)
			{
				TaskTemplMap::iterator it =  pMan->GetAllTemplMap().begin();

				for (; it !=  pMan->GetAllTemplMap().end(); ++it)
				{
					ATaskTempl* pTop = it->second;
					if(pTop && pTop->m_bFaction && !pLst->GetEntry(pTop->GetID()))
					{
						CECHostPlayer::Fac_BuildField* pInfo = GetHostPlayer()->GetBuildInfoTemp(pTop->m_nBuildId);
						if(pInfo && pInfo->nLevel == pTop->m_nBuildLevel)
						{
							m_pTaskList->InsertString(m_pTaskList->GetCount(),pTop->GetName());
						}
					}
				}
			}
		}
	}
	CECTaskInterface* pTask = GetHostPlayer()->GetTaskInterface();
	int nNum = 0;
	if(pTask)
	{
		TaskFinishTimeList* pFinishLst = static_cast<TaskFinishTimeList*>(pTask->GetFinishedTimeList());
		if(!pFinishLst)
			return ;

		unsigned short count = 0;
		for (unsigned short i = 0; i < pFinishLst->m_uCount; i++)
		{
			TaskFinishTimeEntry& entry = pFinishLst->m_aList[i];

			ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(entry.m_uTaskId);

			if(pTempl && pTempl->m_bFaction && pTempl->m_bRecFinishCount)
				nNum += entry.m_uFinishCount;			
		}
	}
	szDesc.Format(GetStringFromTable(20187),nNum,10);
	m_pTxt_Task->SetText(szDesc);
}



void CDlgGuildBuild::OnCommand_EnterLand(const char * szCommand)
{
	if(GetHostPlayer()->IsFighting())
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20189), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if(GetHostPlayer()->GetSrcServerID() != 0)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20190), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if(	GetHostPlayer()->GetRaidBattleInfo() > 0)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20007), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if( GetGameUIMan()->GetDialog("Win_CharHead")->GetDlgItem("Chk_BattleInfo")->IsShow() == true)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(18009), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if(GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_Wait")->IsShow() == true)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(18009), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	c2s_SendEndter_Faction_Base();
}

void CDlgGuildBuild::OnCommand_Donate(const char * szCommand)
{
	CDlgGuildInputMoney*pDlg = (CDlgGuildInputMoney*)GetGameUIMan()->GetDialog("Win_InputGuildDonate");
	pDlg->Show(!pDlg->IsShow());
}
void  CDlgGuildBuild::OnCommand_ExitLand(const char * szCommand)
{
	c2s_SendCmdLevelFaction();
}

void CDlgGuildBuild::OnCommand_Edit(const char * szCommand)
{
	if (GetHostPlayer()->GetFRoleID() == GNET::_R_MASTER
		|| GetHostPlayer()->GetFRoleID() == GNET::_R_VICEMASTER
		|| GetHostPlayer()->GetFRoleID() == GNET::_R_BODYGUARD)
	{
		GetGameUIMan()->m_pDlgProclaim->Show(true);
		GetGameUIMan()->m_pDlgProclaim->m_pDEFAULT_Txt_Input->SetText(_AL(""));
		GetGameUIMan()->m_pDlgProclaim->SetData(2);
	}
}

void CDlgGuildBuild::OnCommand_GRule(const char * szCommand)
{
	//AString strDlg = "Win_Guild_Help";
	//GetGameUIMan()->CommonMessage("Win_Guild_Help","open",(WPARAM)(&strDlg),0);
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Guild_Help");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgGuildBuild::UpDataMsg(void* pData)
{
	using namespace S2C;
	facbase_msg_update* pInfo = (facbase_msg_update*)pData;
	ACString msg =  ACString((ACHAR*)pInfo->msg, pInfo->msg_size/sizeof(ACHAR));
	m_pTxt_Message->SetText(msg);
}

void CDlgGuildBuild::UpDataEdit(ACString szMsg)
{
	m_pTxt_Message->SetText(szMsg);
}

void CDlgGuildBuild::UpDateFacBaseInfo() 
{
	ACString szTemp;
	szTemp.Format(_AL("%d"),GetHostPlayer()->GetFactionGrass());
	m_pTxt_Grass->SetText(szTemp);
	szTemp.Format(_AL("%d"),GetHostPlayer()->GetFactionMine());
	m_pTxt_Mine->SetText(szTemp);
	szTemp.Format(_AL("%d"),GetHostPlayer()->GetFactionMonsterFood());
	m_pTxt_MonsterFood->SetText(szTemp);
	szTemp.Format(_AL("%d"),GetHostPlayer()->GetFactionMonsterCore());
	m_pTxt_MonsterCore->SetText(szTemp);
	szTemp.Format(_AL("%d"),GetHostPlayer()->GetFactionMoney());
	m_pTxt_Money->SetText(szTemp);
	CECTaskInterface* pTask = GetHostPlayer()->GetTaskInterface();
	int nNum = 0;
	if(pTask)
	{
		TaskFinishTimeList* pFinishLst = static_cast<TaskFinishTimeList*>(pTask->GetFinishedTimeList());
		if(!pFinishLst)
			return ;

		unsigned short count = 0;
		for (unsigned short i = 0; i < pFinishLst->m_uCount; i++)
		{
			TaskFinishTimeEntry& entry = pFinishLst->m_aList[i];

			ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(entry.m_uTaskId);

			if(pTempl && pTempl->m_bFaction && pTempl->m_bRecFinishCount)
				nNum += entry.m_uFinishCount;			
		}
	}
	szTemp.Format(GetStringFromTable(20187),nNum,10);
	m_pTxt_Task->SetText(szTemp);
}

void CDlgGuildBuild::UpDateList()
{
	UpDateFacBaseInfo();
	UpDataBuildInfo();
}

void  CDlgGuildBuild::UpDataBuildInfo()
{	
	DATA_TYPE DataType;
	AUIOBJECT_SETPROPERTY p;
	CECHostPlayer::FacBuildField& MapInfo =  GetHostPlayer()->GetMapBuildInfo();

	ACString szTemp;
	strcpy(p.fn,"");
	m_pIcon_build->SetProperty("Image File",&p);
	m_pLab_BuildStatus->SetText(GetStringFromTable(20188));
	m_pLab_Prgs->SetText(_AL(""));
	AString strFile;
	for( int nIndex = 0; nIndex < 18; nIndex++)
	{
		CECHostPlayer::FacBuildField::iterator pIter = MapInfo.find(nIndex + 1);
		if(pIter != MapInfo.end())
		{
			CECHostPlayer::Fac_BuildField* pInfo = &pIter->second ;

			BUILDING_ESSENCE * m_pEssence = (BUILDING_ESSENCE *)GetGame()->GetElementDataMan()->
				get_data_ptr(pInfo->nId, ID_SPACE_ESSENCE, DataType);
			if(m_pEssence)
			{
				af_GetRelativePathNoBase(GetGame()->GetDataPath(m_pEssence->up_building[pInfo->nLevel].ordinary), "surfaces\\", strFile);
				if(pInfo->nStatus == 0)
				{
					szTemp.Format(GetStringFromTable(20185),m_pEssence->name,pInfo->nLevel + 1);
					m_pLab_BuildStatus->SetText(szTemp);
					strcpy(p.fn,strFile);
					m_pIcon_build->SetProperty("Image File",&p);
					if(GetHostPlayer()->GetFactionTask_count() == 0)
					{
						szTemp.Format(GetStringFromTable(20186), 0 );
					}
					else
					{
						szTemp.Format(GetStringFromTable(20186), GetHostPlayer()->GetFactionTask_count()/ GetHostPlayer()->GetFactionTask_need() );
					}
					m_pLab_Prgs->SetText(szTemp);
				}
				//m_pImg_Icon[nIndex]->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
				//	GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
				//m_pImg_Icon[nIndex]->SetCover(NULL, 0, 1);
			}
		}
		else
		{
			//strcpy(p.fn,"");
			//m_pImg_Icon[nIndex]->SetProperty("Image File",&p);
		}
	}
}
void CDlgGuildBuild::OnTick()
{
	int idFaction = GetHostPlayer()->GetFactionID();
	CECFactionMan *pFMan = GetGame()->GetFactionMan();
	GetDlgItem("Btn_ExitLand")->Show(false);
	if(pFMan)
	{
		Faction_Info *pInfo = pFMan->GetFaction(idFaction);
		ACString strText;
		if(pInfo)
		{
			strText.Format(_AL("%d"),pInfo->GetActivity());
			GetDlgItem("Txt_GActivity")->SetText(strText);
	
			GetDlgItem("Txt_Frozen")->Show(!pInfo->IsSelfEnter());
			SetEnterBuild(pInfo->IsSelfEnter());
			if(pInfo->IsSelfCreate() != true)
			{
				GetDlgItem("Txt_Frozen")->Show(false);
			}
			if(GetGame()->GetGuildEnter())
			{
				GetDlgItem("Btn_ExitLand")->Show(true);//GetGameSession()->c2s_SendCmdFacBaseInfo();
				SetEnterBuild(false);
				GetDlgItem("Txt_Frozen")->Show(false);
			}
			if(	GetHostPlayer()->GetRaidBattleInfo() > 0 
				|| GetHostPlayer()->GetBattleType() != -1
				|| GetHostPlayer()->GetSrcServerID() != 0)
			{
				SetEnterBuild(false);
			}
		}
	}
}

void CDlgGuildBuild::SetEnterBuild(bool val) 
{
	m_bIsEnter = val;
	if(m_bIsEnter)
	{
		GetDlgItem("Btn_EnterLand")->Show(true);
	}
	else
	{
		GetDlgItem("Btn_EnterLand")->Show(false);
	}
}


const int CDlgGuildBBS::ms_TimeCircle = 1800000;

bool CDlgGuildBBS::OnInitDialog()
{
	bool bRet = CDlgBaseExplorer::OnInitDialog();
	m_iTime = ms_TimeCircle;
	return bRet;
}

void CDlgGuildBBS::OnShowDialog()
{
	CDlgBaseExplorer::OnShowDialog();
	m_iTime = ms_TimeCircle+1;
}

void CDlgGuildBBS::OnTick()
{
	CDlgBaseExplorer::OnTick();
	if (m_iTime>ms_TimeCircle)
	{
		m_iTime = 0;
		GetGame()->GetGameSession()->get_kdctoken();
		//ChangeFocus(GetDlgItem("WPic"));
	}

	m_iTime += GetGame()->GetTickTime();
}

int CDlgGuildBBS::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="set_token")
	{
		GNET::GetKDCToken_Re* p = (GNET::GetKDCToken_Re*)wParam;
		if (p->retcode==0)
		{
			ACString szTemp;
			m_strToken =  AString((const char*)p->token.begin(), p->token.size() / sizeof (char));
			szTemp.Format(_AL("&time=%d"),GetGame()->GetServerGMTTime());
			NavigateUrl(ACString(GetStringFromTable(20195))+AS2AC(m_strToken)+szTemp);
		}
		else
		{
			ACHAR workPath[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, workPath);
			NavigateUrl(ACString(workPath)+GetStringFromTable(20197));
		}
	}
	return 0;
}

AUI_BEGIN_COMMAND_MAP(CDlgGuildMain, CDlgBase)

AUI_ON_COMMAND("Rdo_*", OnCommand_Rdo)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgGuildMain, CDlgBase)
AUI_ON_EVENT("Txt_Hint",	WM_LBUTTONDOWN,	OnEventLButtonDownText)

AUI_ON_EVENT("Txt_Hint",	WM_MOUSEMOVE,	OnEventMouseMoveText)

AUI_END_EVENT_MAP()
CDlgGuildMain::CDlgGuildMain()
{

}

CDlgGuildMain::~CDlgGuildMain()
{

}

bool CDlgGuildMain::OnInitDialog()
{
	m_pTxt_SelfContribution= (AUILabel*)GetDlgItem("Txt_SelfContribution");
	m_pLab_SelfContribution= (AUILabel*)GetDlgItem("Lab_SelfContribution");
	AString strName;
	for (int i = 0; i < eGuildWindowsType_Count; ++ i)
	{
		strName.Format("Sub_0%d", i+1);
		m_pSubItems[i] = (AUISubDialog*)GetDlgItem(strName);
		m_pSubItems[i]->Show(false);
	}
	UpDataFactionCoupon(0);
	UpdateTextArea((PAUITEXTAREA)GetDlgItem("Txt_Hint"));
	return true;
}

void CDlgGuildMain::OnShowDialog()
{
	for(int i = 1; ; i++)
	{
		char szName[20];
		sprintf(szName, "Rdo_%d", i);
		PAUIRADIOBUTTON pRadio = (PAUIRADIOBUTTON)GetDlgItem(szName);
		if( !pRadio )
			break;

		pRadio->Check(false);
	}
	PAUIRADIOBUTTON pItem = (PAUIRADIOBUTTON)GetDlgItem("Rdo_1");
	pItem->Check(true);
	OnCommand_Rdo("Rdo_1");
	//GetGame()->GetFactionMan()->RefreshMemList(0, 0, false);
	//if(GetGame()->GetGuildEnter())
	//{
	//	GetGameSession()->c2s_SendCmdFacBaseInfo();
	//}
}


void CDlgGuildMain::OnEventLButtonDownText(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);

	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	if( stricmp(pObj->GetName(),"Txt_Hint") != 0)
	{
		return;
	}
	CDlgTask::GetItemLinkItemOn(x, y, pObj, &Item);

	if( Item.m_pItem != NULL )
	{
		if( Item.m_pItem->GetType() == enumEICoord )
		{
			A3DVECTOR3 vec;
			int idInstance;
			a_sscanf(Item.m_pItem->GetInfo(), _AL("%f%f%f%d"), 
				&vec.x, &vec.y, &vec.z, &idInstance);
			{
				CDlgMiniMap* pDlgMiniMap = (CDlgMiniMap*)GetGameUIMan()->GetDialog("Win_MiniMap");
				pDlgMiniMap->m_dwTempTargetTime = GetTickCount();
				pDlgMiniMap->m_vecTempTarget = vec;
				pDlgMiniMap->m_idTempTargetInstance = idInstance;

				g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, idInstance);
				g_AutoMoveDestName = Item.m_pItem->GetName();
			}
		}
	}
}

void CDlgGuildMain::OnEventMouseMoveText(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);

	AUITEXTAREA_EDITBOX_ITEM Item;
	Item.m_pItem = NULL;
	CDlgTask::GetItemLinkItemOn(x, y, pObj, &Item);
}


void CDlgGuildMain::OnTick()
{
	if(GetHostPlayer()->GetFactionID() <= 0  && GetHostPlayer()->GetFamilyID() <= 0)
	{
		Show(false);
		return;
	}
	bool isCreate = false;
	bool isGuildCreate = false;
	int idFaction = GetHostPlayer()->GetFactionID();
	CECFactionMan *pFMan = GetGame()->GetFactionMan();
	GetDlgItem("Txt_Hint")->Show(false);
	if(pFMan)
	{
		Faction_Info *pInfo = pFMan->GetFaction(idFaction);
		AString strText;
		if(pInfo)
		{
			isCreate = true;
			if(pInfo->IsSelfEnter() == true)
			{
				isGuildCreate = true;
			}
			if(pInfo->IsSelfCreate() != true )
			{
				GetDlgItem("Txt_Hint")->Show(true);
			}
			else
			{
				GetDlgItem("Txt_Hint")->Show(false);
			}
		}
	}
	AString strText;
	for(int nIndex = 2; nIndex < 4; nIndex++)
	{
		strText.Format("Rdo_%d",nIndex);
		PAUIRADIOBUTTON pItem = (PAUIRADIOBUTTON)GetDlgItem(strText);
		pItem->Enable(isCreate);
	}
	PAUIRADIOBUTTON pItem = (PAUIRADIOBUTTON)GetDlgItem("Rdo_6");
	pItem->Enable(isCreate);
	pItem = (PAUIRADIOBUTTON)GetDlgItem("Rdo_4");
	pItem->Enable(isGuildCreate);
	pItem = (PAUIRADIOBUTTON)GetDlgItem("Rdo_5");
	pItem->Enable(isGuildCreate);
}

void CDlgGuildMain::OnCommand_Rdo(const char * szCommand)
{
	UINT index = atoi(szCommand+4);
	UpDateRdo(index - 1);
	
}

void CDlgGuildMain::UpDateRdo(int nIndex)
{
	if(nIndex >= eGuildWindowsType_Count)
	{
		return;
	}
	for (int i = 0; i < eGuildWindowsType_Count; ++ i)
	{
		m_pSubItems[i]->Show(false);
	}
	m_pTxt_SelfContribution->Show(true);
	m_pLab_SelfContribution->Show(true);
	GetDlgItem("Img_GuildCon")->Show(true);
	GetGame()->GetFactionMan()->RefreshMemList(0, 0, false);
	if(nIndex == eGuildWindowsType_Family)
	{		
		m_pTxt_SelfContribution->Show(false);
		m_pLab_SelfContribution->Show(false);
		GetDlgItem("Img_GuildCon")->Show(false);
		CDlgGuildMan *pDlg = GetGameUIMan()->m_pDlgFamilyMan;
		if( GetHostPlayer()->GetFamilyID() )
		{
				GetGame()->GetFactionMan()->RefreshMemList(0, GetHostPlayer()->GetFamilyID(), false);
		}
		else
			GetGameUIMan()->MessageBox("NoFamily", GetStringFromTable(870), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if(nIndex == eGuildWindowsType_Guild)
	{
		CDlgGuildMan *pDlg = GetGameUIMan()->m_pDlgGuildMan;
		pDlg->SetPosEx(GetPos().x, GetPos().y);
		if( pDlg->IsFirstOpen() || (int)pDlg->GetDlgItem("List_Family")->GetData() != GetHostPlayer()->GetFactionID() )
		{
			GetGame()->GetFactionMan()->RefreshMemList(0, 0, true);
			pDlg->SetFirstOpen(false);
		}
		else
			GetGame()->GetFactionMan()->RefreshMemList(0, 0, false);
		pDlg->m_idCurFamily = 0;
		pDlg->m_pList_Family->SetCurSel(0);
	}
	else if(nIndex == eGuildWindowsType_Guild_Info)
	{
			GetGame()->GetFactionMan()->RefreshMemList(0, 0, false);
			GetGameSession()->c2s_CmdSendGetFactiondyNamic(0);
	}
	else
	{
		GetGame()->GetFactionMan()->RefreshMemList(0, 0, false);
	}
	m_pSubItems[nIndex]->Show(true);
	if(nIndex == eGuildWindowsType_Guild_BBS)
	{
		GetGameUIMan()->BringWindowToTop( GetGameUIMan()->GetDialog("Win_Guild_Bbs"));
	}
}

void CDlgGuildMain::UpDataFactionCoupon(int nNum)
{
	ACString szTemp;
	szTemp.Format(_AL("%d"),nNum);
	m_pTxt_SelfContribution->SetText(szTemp);
}