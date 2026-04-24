// Filename	: DlgAwardOnline.cpp
// Creator	: Fu Chonggang
// Date		: 2012/3/16

#include "DlgAwardOnline.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_OnlineAward.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrStone.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagepicture.h"
#include "afi.h"
#include "EC_Game.h"
#include "EC_GameUIMisc.h"
#include "time.h"

#include "A2DSprite.h"
#include "ExpTypes.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW


AUI_BEGIN_COMMAND_MAP(CDlgAwardOnline, CDlgBase)

AUI_ON_COMMAND("Btn_Get",		OnCommand_confirm)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAwardOnline, CDlgBase)

AUI_ON_EVENT("Item_MainType*",		WM_LBUTTONDOWN,		OnEventLButtonDownImage)

AUI_END_EVENT_MAP()

CDlgAwardOnline::CDlgAwardOnline()
{
}

CDlgAwardOnline::~CDlgAwardOnline()
{
}

bool CDlgAwardOnline::OnInitDialog()
{
	int i;
	AString strName;
	for (i=0; i<AWARD_ONLINE_NUM_MAIN; ++i)
	{
		strName.Format("Item_MainType%d", i+1);
		m_pImgMain[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Img_MainGetBg%d", i+1);
		m_pImgMainSel[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Txt_MainTypeTime%d", i+1);
		m_pLabTime[i] = (PAUILABEL)GetDlgItem(strName);

		m_iAwardIds[i] = -1;
	}
	for (i=0; i<AWARD_ONLINE_NUM_SUB; ++i)
	{
		strName.Format("Item_SubType%d", i+1);
		m_pImgSub[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
	}
	m_pLabTimeCur = (PAUILABEL)GetDlgItem("Txt_Time");

	m_iCurSel = -1;
	m_iCurSmallBagIndex	= -1;

	return true;
}

bool CDlgAwardOnline::Release()
{
	return CDlgBase::Release();
}

void CDlgAwardOnline::OnTick()
{
	OnlineAwardManager* pOnlineMan = GetHostPlayer()->GetOnlineAwardManager();
	OnlineAwardInfoVector awardVector;
	pOnlineMan->GetOnlineAwardInfos(awardVector);

	ACString strText;
	ACString strHint;
	int i;
	for (i=0; i<(int)awardVector.size(); ++i)
	{
		const OnlineAwardInfoItem& item = awardVector[i];
		strText = GetTimeText(item.m_nRemainTime);
		m_pLabTime[i]->SetText(strText);
		if (m_iAwardIds[i]==item.m_nAwardID)
			continue;
		
		m_iCurSmallBagIndex = -1;
		m_iAwardIds[i] = item.m_nAwardID;
		ONLINE_GIFT_BAG_CONFIG*	pConfig = GetGiftConfig(item.m_nAwardID);
		if (pConfig)
		{
			AString strFile;
			af_GetFileTitle(GetGame()->GetDataPath(pConfig->file_icon), strFile);
			m_pImgMain[i]->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
			GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY, strFile));

			//hint
			strBuildHint(strHint, pConfig);
			m_pImgMain[i]->SetHint(strHint);
		}
	}
	for (; i<AWARD_ONLINE_NUM_MAIN; ++i)
	{
		m_iAwardIds[i] = -1;
		m_pImgMain[i]->ClearCover();
		m_pImgMain[i]->SetHint(_AL(""));
		m_pLabTime[i]->SetText(_AL(""));
	}

	if (awardVector.size()>0)
	{
		if (m_iCurSel>=0 && m_iCurSel<(int)awardVector.size())
		{
			GetDlgItem("Btn_Get")->Enable(awardVector[m_iCurSel].m_nRemainTime<=0);
			m_pLabTimeCur->SetText(m_pLabTime[m_iCurSel]->GetText());
			if(m_iCurSmallBagIndex!=awardVector[m_iCurSel].m_nSmallBagIndex)
				UpdateDisplaySub();
		}
		else
			OnEventLButtonDownImage(0, 0, GetDlgItem("Item_MainType1"));
	}
	else
	{
		GetDlgItem("Btn_Get")->Enable(false);
		m_pLabTimeCur->SetText(_AL(""));
		
		for (i=0; i<AWARD_ONLINE_NUM_MAIN; ++i)
		{
			m_pImgMainSel[i]->Show(false);
		}
		for (i=0; i<AWARD_ONLINE_NUM_SUB; ++i)
		{
			m_pImgSub[i]->ClearCover();
			m_pImgSub[i]->SetText(_AL(""));
			m_pImgSub[i]->SetHint(_AL(""));
		}
		return;
	}
}

void CDlgAwardOnline::OnCommand_confirm(const char * szCommand)
{
	OnlineAwardManager* pOnlineMan = GetHostPlayer()->GetOnlineAwardManager();
	OnlineAwardInfoVector awardVector;
	pOnlineMan->GetOnlineAwardInfos(awardVector);
	if (m_iCurSel<0 || m_iCurSel>=(int)awardVector.size())
	{
		GetGameUIMan()->MessageBox(13301);
		return;
	}
	const OnlineAwardInfoItem& item = awardVector[m_iCurSel];
	ONLINE_GIFT_BAG_CONFIG*	pConfig = GetGiftConfig(item.m_nAwardID);
	if(pConfig)
	{
		GetGameSession()->c2s_CmdObtainOnlineAward(item.m_nAwardID, item.m_nSmallBagIndex, 
			pConfig->small_gift_bags[item.m_nSmallBagIndex].gift_bag_id, false);
	}
}

void CDlgAwardOnline::OnEventLButtonDownImage(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	AString strName = pObj->GetName();
	int iCurSel = strName.Right(1).ToInt();
	iCurSel--;
	OnlineAwardManager* pOnlineMan = GetHostPlayer()->GetOnlineAwardManager();
	OnlineAwardInfoVector awardVector;
	pOnlineMan->GetOnlineAwardInfos(awardVector);
	if (iCurSel>=(int)awardVector.size())
	{
		return;
	}
	m_iCurSel = iCurSel;
	UpdateDisplaySub();
}

void CDlgAwardOnline::UpdateDisplaySub()
{
	OnlineAwardManager* pOnlineMan = GetHostPlayer()->GetOnlineAwardManager();
	OnlineAwardInfoVector awardVector;
	pOnlineMan->GetOnlineAwardInfos(awardVector);
	int i;
	for (i=0; i<AWARD_ONLINE_NUM_MAIN; ++i)
	{
		m_pImgMainSel[i]->Show(false);
		m_pImgMain[i]->SetColor(0x808080);
	}
	for (i=0; i<AWARD_ONLINE_NUM_SUB; ++i)
	{
		m_pImgSub[i]->ClearCover();
		m_pImgSub[i]->SetText(_AL(""));
		m_pImgSub[i]->SetHint(_AL(""));
	}
	if (m_iCurSel>=(int)awardVector.size())
	{
		// 这样在OnTick中会重新选择第一个礼包
		m_iCurSel = -1;
		m_iCurSmallBagIndex = -1;
		return;
	}
	m_pImgMainSel[m_iCurSel]->Show(true);
	m_pImgMain[m_iCurSel]->SetColor(0xFFFFFF);
	const OnlineAwardInfoItem& item = awardVector[m_iCurSel];
	ONLINE_GIFT_BAG_CONFIG*	pConfig = GetGiftConfig(item.m_nAwardID);
	CECIvtrSmallGiftBag	*pItem = dynamic_cast<CECIvtrSmallGiftBag*>(CECIvtrItem::CreateItem(
		pConfig->small_gift_bags[item.m_nSmallBagIndex].gift_bag_id, 0, 1));
	m_iCurSmallBagIndex = item.m_nSmallBagIndex;

	if (pItem)
	{
		const SMALL_ONLINE_GIFT_BAG_ESSENCE* pEss = pItem->GetDBEssence();
		for (i=0; i<AWARD_ONLINE_NUM_SUB; ++i)
		{
			if (pEss->item_configs[i].item_count==0)
			{
				break;
			}
			
			GetGameUIMan()->SetIvtrInfo(m_pImgSub[i], pEss->item_configs[i].item_id, pEss->item_configs[i].item_count, true);
		}
		delete pItem;
	}
}

void CDlgAwardOnline::strBuildHint(ACString& strHint, ONLINE_GIFT_BAG_CONFIG* pConfig)
{
	int i;
	if (pConfig->award_type==1)
	{
		strHint.Format(GetGameUIMan()->GetStringFromTable(10504), pConfig->name);
	}
	else
	{
		const long tExpire = pConfig->expire_time;
		tm* t = gmtime((const time_t*)&tExpire);
		strHint.Format(GetGameUIMan()->GetStringFromTable(10505), pConfig->name, 1900+t->tm_year, 1+t->tm_mon, t->tm_mday);
	}

	bool bFirstItem = true;
	for (i=0; i<16; ++i)
	{
		if (pConfig->small_gift_bags[i].gift_bag_id==0)
		{
			continue;
		}
		
		CECIvtrSmallGiftBag	*pItem = dynamic_cast<CECIvtrSmallGiftBag*>(CECIvtrItem::CreateItem(
			pConfig->small_gift_bags[i].gift_bag_id, 0, 1));

		ACString strItem;
		if (pItem)
		{
			const SMALL_ONLINE_GIFT_BAG_ESSENCE* pEss = pItem->GetDBEssence();
			int k;
			for (k=0; k<6; ++k)
			{
				if (pEss->item_configs[k].item_count==0)
					break;

				ACString strTemp;
				CECIvtrItem *pItemDeliver = CECIvtrItem::CreateItem(pEss->item_configs[k].item_id, 0, 1);
				if (pItemDeliver)
				{
					strTemp.Format(_AL("%sx%d "), pItemDeliver->GetName(), pEss->item_configs[k].item_count);
					strItem += strTemp;
					delete pItemDeliver;
				}
			}

			delete pItem;
		}

		ACString strTime = GetTimeTextNumber(pConfig->small_gift_bags[i].deliver_interval);
		ACString strBagItems;
		strBagItems.Format(GetGameUIMan()->GetStringFromTable(bFirstItem ? 10506:10507), i+1, strItem, strTime);
		bFirstItem = false;
		strHint += strBagItems;
		strHint += _AL("\r");
	}
//	FormatTextFixedWidth(strHint, 500);
}

ONLINE_GIFT_BAG_CONFIG* CDlgAwardOnline::GetGiftConfig(int tid)
{
	elementdataman* pDB = GetGame()->GetElementDataMan();
	DATA_TYPE DataType = DT_INVALID;
	const void* pData = pDB->get_data_ptr(tid, ID_SPACE_CONFIG, DataType);
	if (DataType==DT_ONLINE_GIFT_BAG_CONFIG)
	{
		ONLINE_GIFT_BAG_CONFIG*	pConfig = (ONLINE_GIFT_BAG_CONFIG*)pData;
		return pConfig;
	}

	return 0;
}