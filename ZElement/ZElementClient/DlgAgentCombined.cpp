/********************************************************************
	created:	2012/10/15
	file name:	CDlgAgentCombined.h
	author:		zhuyan
	
	purpose:		
*********************************************************************/
#include "AFI.h"
#include "DlgAgentCombined.h"
#include "EC_GameUIMan.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"


#define AGENTCOMBINED_MAX 2000000000


AUI_BEGIN_COMMAND_MAP(CDlgAgentCombined, CDlgBase)

AUI_ON_COMMAND("Btn_Enchase", OnCommand_Perfusion)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAgentCombined, CDlgBase)

AUI_ON_EVENT("Item_2",			WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()



CDlgAgentCombined::CDlgAgentCombined()
{
	m_pAgentItem = NULL;
	m_pEffective = NULL;
	m_nSrcIndex = -1;
	m_nEffectiveIndex = -1;
}

 CDlgAgentCombined::~CDlgAgentCombined()
{
	
}


bool CDlgAgentCombined::OnInitDialog()
{
	return true;
}


void CDlgAgentCombined::OnShowDialog()
{
	// 显示界面
	OnEventLButtonDown(0,0,NULL);
}

void CDlgAgentCombined::OnHideDialog()
{
	if(m_pAgentItem)
	{
		m_pAgentItem->Freeze(false);
	}
}


void CDlgAgentCombined::SetAgentCombiendType(CECIvtrItem* pIvtrSrc,int nIndex)
{
	AString strFile;
	AUICTranslate trans;
	PAUIIMAGEPICTURE pPicture = (PAUIIMAGEPICTURE)GetDlgItem("Item_1");
	if (pIvtrSrc )
	{
		m_pAgentItem = pIvtrSrc;
		CECIvtrAgentCombinedItem* pData = (CECIvtrAgentCombinedItem*)m_pAgentItem;
		m_nSrcIndex = nIndex;
		pIvtrSrc->SetNeedUpdate(false);
		af_GetFileTitle(pIvtrSrc->GetIconFile(), strFile);
		strFile.MakeLower();
		pPicture->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
			GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
		pPicture->SetCover(NULL, 0, 1);
		pPicture->SetHint(trans.Translate(pData->GetDesc()));
	 	pPicture->SetData(nIndex);
	}
}


void CDlgAgentCombined::OnCommand_Perfusion(const char * szCommand)	// 灌注
{
	// 打开提示界面

	if(m_pAgentItem == NULL || m_pEffective == NULL)
	{
		GetGameUIMan()->MessageBox(17002);
		return;
	}
	if(m_pEffective)
	{
		GetGameUIMan()->MessageBox("Game_Perfusion", GetStringFromTable(17001), 
						MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
	}

}

void CDlgAgentCombined::CheckAgentCombiend()
{
	/// 检测一下 灌注容量 AGENTCOMBINED_MAX
	CECIvtrAgentCombinedItem *pItem = (CECIvtrAgentCombinedItem *)m_pAgentItem;
	int nCcapacity = pItem->GetEssence().capacity;
	CECIvtrMedicine* pData = (CECIvtrMedicine*)m_pEffective;
	if(pItem->GetDBEssence()->type == 0 || pItem->GetDBEssence()->type == 3)
	{
		nCcapacity += pData->GetDBEssence()->hp[0];
	}
	else if(pItem->GetDBEssence()->type == 1 || pItem->GetDBEssence()->type == 4)
	{
		nCcapacity += pData->GetDBEssence()->mp[0];
	}
	else if(pItem->GetDBEssence()->type == 2 || pItem->GetDBEssence()->type == 5)
	{

	}


	if(nCcapacity > AGENTCOMBINED_MAX)
	{
		GetGameUIMan()->MessageBox(17004);
		return;
	}
	GetGameSession()->c2s_CmdMergeMedicine(m_pAgentItem->GetTemplateID(),m_nSrcIndex,
		m_pEffective->GetTemplateID(),m_nEffectiveIndex);
	OnEventLButtonDown(0,0,NULL);
}



void CDlgAgentCombined::IsAgentEffective(CECIvtrItem* pEffective,int nIndex)	// 判断是否是有效ID
{
	if(m_pAgentItem && pEffective)
	{
		if(m_pEffective)
		{
			m_pEffective->Freeze(false);
		}
		m_pEffective = pEffective;
		AString strFile;
		AUICTranslate trans;
		PAUIIMAGEPICTURE pPicture = (PAUIIMAGEPICTURE)GetDlgItem("Item_2");
		if (pEffective )
		{
			m_nEffectiveIndex = nIndex;
			pEffective->SetNeedUpdate(false);
			af_GetFileTitle(pEffective->GetIconFile(), strFile);
			strFile.MakeLower();
			pPicture->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
				GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
			pPicture->SetCover(NULL, 0, 1);
			pPicture->SetHint(trans.Translate(pEffective->GetDesc()));
		}
	}
}

void CDlgAgentCombined::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)	// 点击配方
{
	PAUIIMAGEPICTURE pPicture = (PAUIIMAGEPICTURE)GetDlgItem("Item_2");
	if (pPicture )
	{
		GetDlgItem("Item_2")->SetText(_AL(""));
		GetDlgItem("Item_2")->SetDataPtr(NULL);
		pPicture->SetCover(NULL, -1);
		GetDlgItem("Item_2")->SetColor(A3DCOLORRGB(255, 255, 255));
		pPicture->SetHint(_AL(""));
		if(m_pEffective)
		{
			m_pEffective->Freeze(false);
		}
		m_pEffective = NULL;
	}
}

bool CDlgAgentCombined::IsItemIDEffective(int nItemId)
{
	if(m_pAgentItem)
	{
		CECIvtrAgentCombinedItem *pItem = (CECIvtrAgentCombinedItem *)m_pAgentItem;
		for(int nIndex = 0; nIndex < 20; nIndex++)
		{
			if(pItem->GetDBEssence()->item_ids[nIndex] == nItemId )
			{
				return true;
			}
		}
	}
	return false;
}