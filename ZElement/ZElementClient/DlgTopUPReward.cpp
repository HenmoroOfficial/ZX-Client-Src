/********************************************************************
created:	2012/8/31
file base:	CDlgTopUPReward
file ext:	h
author:		zy

  purpose:	充值奖励界面
*********************************************************************/


#include "EC_GameUIMan.h"
#include "EC_IvtrItem.h"
#include "AFI.h"
#include "DlgTopUPReward.h"
#include "AUI\\AUICTranslate.h"


AUI_BEGIN_COMMAND_MAP(CDlgTopUPRewardMin, CDlgBase)

AUI_ON_COMMAND("Btn_Max",		OnCommandOpen)

AUI_END_COMMAND_MAP()

void CDlgTopUPRewardMin::OnCommandOpen(const char * szCommand)
{
	PAUIDIALOG	pDlg = GetGameUIMan()->GetDialog("Win_ChargeAward");
	pDlg->Show(!pDlg->IsShow());
}


AUI_BEGIN_COMMAND_MAP(CDlgTopUPReward, CDlgBase)


AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgTopUPReward, CDlgBase)

AUI_END_EVENT_MAP()

CDlgTopUPReward::CDlgTopUPReward()
{
	
}

CDlgTopUPReward::~CDlgTopUPReward()
{
}

bool CDlgTopUPReward::OnInitDialog()
{
	
	PAUIOBJECTLISTELEMENT pElement = GetFirstControl();
	CECIvtrItem* pItem = NULL;
	AString strFile;
	AUICTranslate trans;
	while( pElement )
	{
		if( strstr(pElement->pThis->GetName(), "Item_Main_") )
		{
			int id = atoi(pElement->pThis->GetName() + strlen("Item_Main_A")); // 后strlen只是获取字符串长度
			PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(pElement->pThis);
			if( id == 0 )
			{
				pElement = pElement->pNext;
				continue;
			}	
			GetGroupItem(id,pElement->pThis->GetName());
		}
		if( strstr(pElement->pThis->GetName(), "Item_") )
		{
			int id = atoi(pElement->pThis->GetName() + strlen("Item_A"));
			PAUIIMAGEPICTURE pImage = static_cast<PAUIIMAGEPICTURE>(pElement->pThis);
			if( id == 0 )
			{
				pElement = pElement->pNext;
				continue;
			}	
			GetGroupItem(id,pElement->pThis->GetName());
		}
		pElement = pElement->pNext;
	}
	
	return true;
}

void CDlgTopUPReward::OnShowDialog()
{
	//OnInitDialog();
}


PAUIIMAGEPICTURE CDlgTopUPReward::GetGroupItem(int nID,const char *pszItemName)
{
	CECIvtrItem* pItem = CECIvtrItem::CreateItem(nID, 0,1);
	AString strFile;
	AUICTranslate trans;
	PAUIIMAGEPICTURE pPicture = (PAUIIMAGEPICTURE)GetDlgItem(pszItemName);
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
	return pPicture;
}
