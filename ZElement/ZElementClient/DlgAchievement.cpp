// Filename	: DlgAchievement.cpp
// Creator	: Fu Chonggang
// Date		: 2009/7/9

#include "AUI\\AUICTranslate.h"
#include "AFI.h"
#include "AFileImage.h"
#include "DlgAchievement.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_IvtrConsume.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"
#include "EC_Inventory.h"
#include "EC_Achievement.h"
#include "EC_ManPlayer.h"
#include "EC_World.h"
#include "time.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgAchievement, CDlgBase)

AUI_ON_COMMAND("Rdo_*",		OnCommandRdoType)
AUI_ON_COMMAND("IDCANCEL",	OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAchievement, CDlgBase)

AUI_ON_EVENT("Tv_List", WM_LBUTTONDOWN, OnEventLButtonDown_Tv_Lst)
AUI_END_EVENT_MAP()

const ACHAR* szFinishColor = _AL("^00ff00");
const ACHAR* szWhiteColor = _AL("^ffffff");

CDlgAchievement::CDlgAchievement()
{
	m_bSet			= false;
	m_iTarget		= 0;
	m_pLab_Point	= NULL;
	m_pTv_List		= NULL;
	m_pTxt_Desc		= NULL;
	m_pTxt_Cond		= NULL;

	for(int i=0; i<ACHIEVEMENT_REWARD_MAX; i++)
		m_pImg_Item[i]	= NULL;
	m_pLab_AwardBase	= NULL;
	m_pLab_AwardMoney	= NULL;
}

CDlgAchievement::~CDlgAchievement()
{
}

void CDlgAchievement::OnCommandRdoType(const char* szCommand)
{
	UINT index = atoi(szCommand+4);
	PAUITREEVIEW pTree = m_pTv_List;
	pTree->DeleteAllItems();
	P_AUITREEVIEW_ITEM pItem = NULL;
	CECAchievementMan* pMan = m_pPlayer->GetAchievementMan();
	abase::hash_map<unsigned int, sAchievement>& achivSet = pMan->GetAchievementSet();
	abase::hash_map<unsigned int,sAchievement>::iterator iter;
	for ( iter = achivSet.begin(); iter!=achivSet.end(); ++iter)
	{
		sAchievement& achievement = iter->second;
		if (achievement.iClass==index)
		{
			pItem = pTree->GetFirstChildItem(pTree->GetRootItem());
			while( pItem )
			{
				if( pTree->GetItemData(pItem) == achievement.iSubClass )
					break;
				pItem = pTree->GetNextSiblingItem(pItem);
			}
			if (!pItem)
			{
				const sAchievementClass* aClass = pMan->GetAchievementClass(achievement.iClass);
				if (!aClass)
				{
					return;
				}
				pItem = pTree->InsertItem((aClass->vecSubClass[achievement.iSubClass-1]).name);
				pTree->SetItemData(pItem, achievement.iSubClass);
			}
			pItem = pTree->InsertItem(achievement.name, pItem);
			if (!achievement.bFinish)
			{
				pTree->SetItemTextColor(pItem, A3DCOLORRGB(128, 128, 128));
			}
			pTree->SetItemData(pItem, achievement.id);
		}
	}
}


void CDlgAchievement::OnCommandCancel(const char* szCommand)
{
	m_iTarget = 0;
	m_pPlayer = GetHostPlayer();
	Show(false);
}

bool CDlgAchievement::OnInitDialog()
{
	char szText[64];
	DDX_Control("Txt_SuccessPoint", m_pLab_Point);
	DDX_Control("Tv_List", m_pTv_List);
	DDX_Control("Txt_Content", m_pTxt_Desc);
	DDX_Control("Txt_Qualify", m_pTxt_Cond);
	for (int i=0; i<ACHIEVEMENT_REWARD_MAX; i++)
	{
		sprintf(szText,"Img_Item%d",i+1);
		DDX_Control(szText, m_pImg_Item[i]);
	}
	DDX_Control("Txt_BaseAward", m_pLab_AwardBase);
	DDX_Control("Txt_MoneyAward", m_pLab_AwardMoney);
	CheckRadioButton(1,1);
	m_pPlayer = GetHostPlayer();

	return true;
}

void CDlgAchievement::OnTick()
{
	if (m_iTarget>0)
	{
		CECPlayerMan *pPlayerMan = GetWorld()->GetPlayerMan();
		CECPlayer *pPlayer = (CECPlayer *)pPlayerMan->GetPlayer(m_iTarget);
		if (pPlayer)
		{
			m_pPlayer = pPlayer;
		}
		else
			OnCommandCancel("");
	}
	else if (!m_pPlayer)
	{
		m_pPlayer = GetHostPlayer();
	}
	CECAchievementMan* pMan = m_pPlayer->GetAchievementMan();
	P_AUITREEVIEW_ITEM pItem, pItemChild;
	pItem = m_pTv_List->GetFirstChildItem(m_pTv_List->GetRootItem());
	int iTotal = 0;
	int iFinished = 0;
	while(pItem)
	{
		pItemChild =  m_pTv_List->GetFirstChildItem(pItem);
		ACString strText;
		while (pItemChild)
		{
			iTotal++;
			sAchievement* achiv = pMan->GetAchievement(m_pTv_List->GetItemData(pItemChild));
			if (achiv && !achiv->bFinish)
			{
				ACString strText = m_pTv_List->GetItemText(pItemChild);
				strText.CutLeft(7);
				strText = ACString(_AL("^888888")) + strText;
				m_pTv_List->SetItemText(pItemChild, strText);
			}
			else if (achiv && achiv->bFinish)
				iFinished++;
			else
				return;
			const sAchievementClass* aClass = pMan->GetAchievementClass(achiv->iClass);
			if (!aClass)
			{
				return;
			}
			strText = aClass->vecSubClass[achiv->iSubClass-1].name;
			pItemChild = m_pTv_List->GetNextSiblingItem(pItemChild);
		}
		strText.Format(GetStringFromTable(8482), strText,iFinished, iTotal);
		m_pTv_List->SetItemText(pItem, strText);
		if (iTotal==iFinished)
		{
			m_pTv_List->SetItemTextColor(pItem, A3DCOLORRGB(0, 255, 0));
		}
		iTotal = 0;
		iFinished = 0;
		pItem = m_pTv_List->GetNextSiblingItem(pItem);
	}
	pItem = m_pTv_List->GetSelectedItem();
	if( !pItem || m_pTv_List->GetParentItem(pItem) == m_pTv_List->GetRootItem())
	{
		GetDlgItem("Lab_ItemAward")->Show(false);
		m_pTxt_Desc->SetText(_AL(""));
		m_pTxt_Cond->SetText(_AL(""));
		m_pLab_AwardBase->SetText(_AL(""));
		m_pLab_AwardMoney->SetText(_AL(""));
		for (int i=0; i<ACHIEVEMENT_REWARD_MAX ; i++)
		{
			m_pImg_Item[i]->Show(false);
		}
	}
	else
		OnEventLButtonDown_Tv_Lst(0,0,NULL);
	ACHAR szText[20];
	a_sprintf(szText,_AL("%d/%d"),pMan->GetAchievementPoint(),pMan->GetAchievementPoint());
	m_pLab_Point->SetText(szText);
}

void CDlgAchievement::OnShowDialog()
{
	int index = GetCheckedRadioButton(1);
	if (index<1)
	{
		index = 1;
		CheckRadioButton(1,1);
	}
	char szText[20];
	sprintf(szText,"Rdo_%d",index);
	OnCommandRdoType(szText);
}

void CDlgAchievement::OnEventLButtonDown_Tv_Lst(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	P_AUITREEVIEW_ITEM pItem = m_pTv_List->GetSelectedItem();
	if (pItem && m_pTv_List->GetParentItem(pItem) != m_pTv_List->GetRootItem())
	{
		CECAchievementMan* pMan = m_pPlayer->GetAchievementMan();
		sAchievement* achiv = pMan->GetAchievement(m_pTv_List->GetItemData(pItem));
		ACString strText;
		if (achiv->bMarkFinish && achiv->iFinishTime>0)
		{
			long t = achiv->iFinishTime;
			if (t>0)
			{
				t -= g_pGame->GetTimeZoneBias() * 60;
				tm *gtime = gmtime((const time_t*)&t);
				strText.Format(GetGameUIMan()->GetStringFromTable(8000),gtime->tm_year+1900,gtime->tm_mon+1,gtime->tm_mday,gtime->tm_hour,gtime->tm_min);
			}
			strText = achiv->strDesc + _AL("\r") + strText;
			m_pTxt_Desc->SetText(strText);
		}
		else
			m_pTxt_Desc->SetText(achiv->strDesc);
		ACString strCond, strTemp, strDesc;
		UINT iSize = achiv->vecPremissID.size();
		if (iSize>0)
		{
			strCond = GetStringFromTable(8474);
			for (UINT i=0; i<iSize; i++)
			{
				sAchievement* achivC = pMan->GetAchievement(achiv->vecPremissID[i]);
				if (achivC->bFinish)
				{
					strCond += szFinishColor;
				}
				strCond += achivC->name;
				if (i<iSize-1)
				{
					strCond += _AL(",");
				}
			}
			strCond += _AL("^ffffff\r");
		}
		iSize = achiv->vecCond.size();
		for (UINT i=0; i<iSize; i++)
		{
			achiv->vecCond[i].GetStrDesc(strDesc);
			if (achiv->vecCond[i].bFinish)
				strTemp.Format(GetStringFromTable(8483), i+1, strDesc);
			else
				strTemp.Format(GetStringFromTable(8473), i+1, strDesc);
			strCond += strTemp;
		}
		ACString strOrig = m_pTxt_Cond->GetText();
		if (strOrig != strCond)
		{
			m_pTxt_Cond->SetText(strCond);
		}
		strCond.Format(GetStringFromTable(8475),achiv->awardScore);
		m_pLab_AwardBase->SetText(strCond);
		if (achiv->awardMoney>0)
			strCond.Format(GetStringFromTable(8476),achiv->awardMoney);
		else
			strCond.Empty();
		m_pLab_AwardMoney->SetText(strCond);
		if (achiv->awardItem >0)
		{
			GetDlgItem("Lab_ItemAward")->Show(false);
			CECIvtrItem *pItem = CECIvtrItem::CreateItem(achiv->awardItem, 0, 1);
			m_pImg_Item[0]->Show(true);
			if (pItem)
			{
				pItem->GetDetailDataFromLocal();
				AUICTranslate trans;
				AString strFile;
				m_pImg_Item[0]->SetHint(trans.Translate(pItem->GetDesc()));
				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				int ItemIndex;
				A2DSprite* pSprite = GetGameUIMan()->GetSpriteIcon(CECGameUIMan::ICONS_INVENTORY, strFile.GetBuffer(0), &ItemIndex);
				m_pImg_Item[0]->SetCover( pSprite, ItemIndex);
				m_pImg_Item[0]->SetColor(GetGameUIMan()->GetItemColor(pItem));
				delete pItem;
			}
			else
			{
				GetDlgItem("Lab_ItemAward")->Show(false);
				for (int i=0; i<ACHIEVEMENT_REWARD_MAX ; i++)
				{
					m_pImg_Item[i]->Show(false);
				}
			}
		}
		else
		{
			GetDlgItem("Lab_ItemAward")->Show(false);
			for (int i=0; i<ACHIEVEMENT_REWARD_MAX ; i++)
			{
				m_pImg_Item[i]->Show(false);
			}
		}
	}
}