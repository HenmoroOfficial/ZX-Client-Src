/********************************************************************
	created:	2012/10/15
	file name:	CDlgAgentCombined.h
	author:		zhuyan
	
	purpose:		
*********************************************************************/
#include "AFI.h"
#include "DlgRemedyMetempsychosis.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"


AUI_BEGIN_COMMAND_MAP(DlgRemedyMetempsychosis, CDlgBase)

AUI_ON_COMMAND("Btn_Confirm", OnCommand_Ok)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(DlgRemedyMetempsychosis, CDlgBase)

AUI_ON_EVENT("Img_Item",			WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()


DlgRemedyMetempsychosis::DlgRemedyMetempsychosis()
{
	m_pItem = NULL;
	m_nItemIndex = -1;
	m_mapLevel.clear();

	m_mapLevel[135] = 27;
	m_mapLevel[136] = 26;
	m_mapLevel[137] = 25;
	m_mapLevel[138] = 24;
	m_mapLevel[139] = 23;
	m_mapLevel[140] = 22;

	m_mapLevel[141] = 20;
	m_mapLevel[142] = 18;
	m_mapLevel[143] = 16;


	m_mapLevel[144] = 14;
	m_mapLevel[145] = 12;
	m_mapLevel[146] = 10;
	m_mapLevel[147] = 8;
	m_mapLevel[148] = 6;
	m_mapLevel[149] = 3;
}

 DlgRemedyMetempsychosis::~DlgRemedyMetempsychosis()
{
	m_mapLevel.clear();	
}


bool DlgRemedyMetempsychosis::OnInitDialog()
{
	m_pTxt_Now = (PAUILABEL)GetDlgItem("Txt_Num1");
	m_pTxt_Existing = (PAUILABEL)GetDlgItem("Txt_Num2");
	
	const SPECIAL_ID_CONFIG& ids = GetGame()->GetGameRun()->GetSpecialIDConfig();
	CECIvtrItem* pItem = CECIvtrItem::CreateItem(ids.id_fix_prop_lose, 0,1);
	AUICTranslate trans;
	AString strFile;
	PAUIIMAGEPICTURE pPicture = (PAUIIMAGEPICTURE)GetDlgItem("Img_Num");
	if (pItem )
	{

		pItem->SetNeedUpdate(false);
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		pPicture->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
			GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
		pPicture->SetCover(NULL, 0, 1);
		pPicture->SetHint(trans.Translate(pItem->GetDesc()));
		delete pItem;
	}
	
	return true;
}


void DlgRemedyMetempsychosis::OnHideDialog()
{
	if(m_pItem)
	{
		m_pItem->Freeze(false);
	}
}


void DlgRemedyMetempsychosis::OnShowDialog()
{
	// 显示界面
	UpdataRemedyMetempsychosisItem();
	if (GetHostPlayer()->GetRebornInfo().size() > 0)
	{
		SetLevel((GetHostPlayer()->GetRebornInfo())[0].level);
		SetClass((GetHostPlayer()->GetRebornInfo())[0].prof);
	}
}		

void DlgRemedyMetempsychosis::UpdataRemedyMetempsychosisItem()
{
	OnEventLButtonDown(0,0,NULL);
	if (GetHostPlayer()->GetRebornInfo().size() == 0)
	{
		return;
	}
	LevelMap::iterator it = m_mapLevel.find((GetHostPlayer()->GetRebornInfo())[0].level);
	if(it != m_mapLevel.end())
	{
		ACString strText;
		strText.Format(_AL("%d"),it->second);
		m_pTxt_Now->SetText(strText);
		
	}
	CECInventory *pPack = GetHostPlayer()->GetPack();
	const SPECIAL_ID_CONFIG& ids = GetGame()->GetGameRun()->GetSpecialIDConfig();
	if(pPack)
	{
		ACString strText;
		strText.Format(_AL("%d"),pPack->GetItemTotalNum(ids.id_fix_prop_lose));
		m_pTxt_Existing->SetText(strText);
	}
}


void DlgRemedyMetempsychosis::SetRemedyMetempsychosisItem(CECIvtrItem* pIvtrSrc,int nIndex)
{
	AString strFile;
	ACHAR szText[20];
	AUICTranslate trans;
	PAUIIMAGEPICTURE pPicture = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
	if (pIvtrSrc )
	{
		m_pItem = pIvtrSrc;
		m_nItemIndex = nIndex;
		pIvtrSrc->SetNeedUpdate(false);
		af_GetFileTitle(pIvtrSrc->GetIconFile(), strFile);
		strFile.MakeLower();
		pPicture->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
			GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
		pPicture->SetCover(NULL, 0, 1);
		pPicture->SetHint(trans.Translate(pIvtrSrc->GetDesc()));
		a_sprintf(szText, _AL("%d"), pIvtrSrc->GetCount());
		pPicture->SetText(szText);
		// pPicture->SetData(nIndex);
	}
}


void DlgRemedyMetempsychosis::OnCommand_Ok(const char * szCommand)
{
	// 发送消息给服务器
	if(m_pItem)
	{
		int iError = CheckRemedyMetempsychosisItem(m_pItem->GetTemplateID(),m_pItem->GetCount());
		if( iError ==1)
		{
			GetGameSession()->c2s_CmdRemedyMetempsyChosisLevel(m_nItemIndex);
		}
		else if(iError == 2)
		{
			GetGameUIMan()->MessageBox(17005);
		}
		else if(iError == 3)
		{
			GetGameUIMan()->MessageBox(17003);
		}
	}
// 	const ECMSG Msg;
// 	GetHostPlayer()->OnMsgHstRemedy_MetempsyChosisLevel(Msg);
}

void DlgRemedyMetempsychosis::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)	// 点击配方
{
	PAUIIMAGEPICTURE pPicture = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
	if (pPicture )
	{
		GetDlgItem("Img_Item")->SetText(_AL(""));
		GetDlgItem("Img_Item")->SetDataPtr(NULL);
		pPicture->SetCover(NULL, -1);
		GetDlgItem("Img_Item")->SetColor(A3DCOLORRGB(255, 255, 255));
		pPicture->SetHint(_AL(""));
		if(m_pItem)
		{
			m_pItem->Freeze(false);
		}
		m_pItem = NULL;
	}
}

int DlgRemedyMetempsychosis::CheckRemedyMetempsychosisItem(int nItemID,int nNum)
{
	const SPECIAL_ID_CONFIG& ids = GetGame()->GetGameRun()->GetSpecialIDConfig();
	if (GetHostPlayer()->GetRebornInfo().size() == 0)
	{
		return 0;
	}
	if(ids.id_fix_prop_lose/*42402*/  == nItemID)
	{
		LevelMap::iterator it = m_mapLevel.find((GetHostPlayer()->GetRebornInfo())[0].level);
		if(it != m_mapLevel.end())
		{
			if(it->second <= nNum)
			{
				return 1;
			}
			return 3;
		}
	}
	return 2;

}

void DlgRemedyMetempsychosis::SetLevel(int i)
{
	m_nLevel = i;
}

int DlgRemedyMetempsychosis::GetLevel()
{
	return m_nLevel;
}


void DlgRemedyMetempsychosis::SetClass(int i)
{
	m_nClass = i;
}

int DlgRemedyMetempsychosis::GetClass()
{
	return m_nClass;
}
