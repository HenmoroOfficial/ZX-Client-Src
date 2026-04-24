// Filename	:	DlgEquipBijou.cpp
// Creator	:	Fu Chonggang
// Date		:	Mar 15, 2011

#include "DlgEquipBijou.h"
#include "DlgInputNO.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUICheckBox.h"

#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrStone.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "ExpTypes.h"
#include "elementdataman.h"


#define new A_DEBUG_NEW


//////////////////////////////////////////////////////////////////////////
// CDlgBijouUpgrade
//////////////////////////////////////////////////////////////////////////

AUI_BEGIN_EVENT_MAP(CDlgBijouUpgrade, CDlgItemSvrBase)

AUI_ON_EVENT("Img_Item*",	WM_LBUTTONDOWN,	OnEventLButtonDownDust)
AUI_ON_EVENT("Img_Item*",	WM_RBUTTONDOWN,	OnEventLButtonDownDust)

AUI_END_EVENT_MAP()

CDlgBijouUpgrade::CDlgBijouUpgrade()
{
	m_pImgDestNormal = NULL;
	m_pImgDestGood = NULL;
	m_pLabProbNormal = NULL;
	m_pLabProbGood = NULL;
	for(int i=0; i<PROP_NUM_MAX; ++i)
	{
		m_pImgProp[i] = NULL;
	}
}

CDlgBijouUpgrade::~CDlgBijouUpgrade()
{
}

bool CDlgBijouUpgrade::OnInitDialog()
{
	DDX_Control("Img_Stone2", m_pImgDestNormal);
	DDX_Control("Img_Stone3", m_pImgDestGood);
	DDX_Control("Txt_Common",	m_pLabProbNormal);
	DDX_Control("Txt_Perfect",	m_pLabProbGood);
	AString strName;
	for(int i=0; i<PROP_NUM_MAX; ++i)
	{
		strName.Format("Img_Item%d", i+1);
		DDX_Control(strName, m_pImgProp[i]);
	}

	return CDlgItemSvrBase::OnInitDialog();
}

int CDlgBijouUpgrade::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="set_item")
	{
		int nSlot = wParam;
		int nNum = lParam;
		for (int i=0; i<PROP_NUM_MAX; ++i)
		{
			if (!m_pImgProp[i]->GetDataPtr("ptr_CECIvtrItem"))
			{
				m_pImgProp[i]->SetData(nSlot);
				m_pImgProp[i]->SetDataPtr(GetHostPlayer()->GetPack()->GetItem(nSlot), "ptr_CECIvtrItem");
				if (--nNum<=0)
				{
					break;
				}
			}
		}
		if ( nNum!=lParam )
		{
			GetHostPlayer()->GetPack()->GetItem(nSlot)->Freeze(true);
			UpdateDisplay();
		}
	}
	return 0;
}

void CDlgBijouUpgrade::OnEventLButtonDownDust(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if(!pObj || pObj->GetType()!=AUIOBJECT_TYPE_IMAGEPICTURE || !pObj->GetDataPtr("ptr_CECIvtrItem"))
		return;

	CECIvtrItem* pItem = (CECIvtrItem*)pObj->GetDataPtr("ptr_CECIvtrItem");
	int iNumPut = 0;
	for (int i=0; i<PROP_NUM_MAX; ++i)
	{
		if (m_pImgProp[i]!=pObj && m_pImgProp[i]->GetDataPtr("ptr_CECIvtrItem")==pObj->GetDataPtr("ptr_CECIvtrItem"))
		{
			iNumPut++;
		}
	}
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pObj;
	pItem->Freeze(iNumPut==pItem->GetCount());
	pImg->ClearCover();
	pImg->SetText(_AL(""));
	pImg->SetHint(_AL(""));
	pImg->SetData(0);
	pImg->SetDataPtr(0, "ptr_CECIvtrItem");
	UpdateDisplay();
}

void CDlgBijouUpgrade::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if ( pItem->GetClassID()==CECIvtrItem::ICID_GEM && (pImg==m_pImg_Item||!pImg) )
	{
		CECIvtrItem *pItemOrg = (CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		if( pItemOrg )
			pItemOrg->Freeze(false);
		
		CECIvtrGem* pGem = (CECIvtrGem*)pItem;
		if (pGem->GetDBEssence()->grade>=GetGameRun()->GetSpecialIDConfig().gem_upgrade_upper_limit)
		{
			GetGameUIMan()->MessageBox(1530);
			return;
		}
		m_pImg_Item->SetData(nSlot);
		m_pImg_Item->SetDataPtr(pItem, "ptr_CECIvtrItem");
		pItem->Freeze(true);
	}
	else if ( pItem->GetClassID()==CECIvtrItem::ICID_GEM_DUST && 
		(!pImg||(strstr(pImg->GetName(), "Img_Item")&&strlen(pImg->GetName())>strlen("Img_Item"))) )
	{
		if (!pImg)
		{
			for (int i=0; i<PROP_NUM_MAX; ++i)
			{
				if(!m_pImgProp[i]->GetDataPtr("ptr_CECIvtrItem"))
				{
					pImg = m_pImgProp[i];
					break;
				}
			}
		}
		if (!pImg)
		{
			return;
		}
		CECIvtrItem *pItemOrg = (CECIvtrItem*)pImg->GetDataPtr("ptr_CECIvtrItem");
		if( pItemOrg )
			pItemOrg->Freeze(false);

		pImg->SetData(nSlot);
		pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
		int iNumPut = 1;
		for (int i=0; i<PROP_NUM_MAX; ++i)
		{
			if (m_pImgProp[i]!=pImg && m_pImgProp[i]->GetDataPtr("ptr_CECIvtrItem")==pImg->GetDataPtr("ptr_CECIvtrItem"))
			{
				iNumPut++;
			}
		}
		pItem->Freeze(iNumPut==pItem->GetCount());
/*
		if (pItem->GetCount()==1)
		{
			if (!pImg)
			{
			}
			if(!pImg)
				return;
			CECIvtrItem *pItemOrg = (CECIvtrItem*)pImg->GetDataPtr("ptr_CECIvtrItem");
			if( pItemOrg )
				pItemOrg->Freeze(false);
			pImg->SetData(nSlot);
			pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
			pItem->Freeze(true);
		}
		else
		{
			if (pImg)
			{
				CECIvtrItem *pItemOrg = (CECIvtrItem*)pImg->GetDataPtr("ptr_CECIvtrItem");
				if( pItemOrg )
				{
					OnEventLButtonDownDust(0, 0, pImg);
				}
			}
			int iMax = pItem->GetCount();
			int iMaxSlot = 0;
			for (int i=0; i<PROP_NUM_MAX; ++i)
			{
				if((int)m_pImgProp[i]->GetData()==nSlot)
					iMax--;
				if (!m_pImgProp[i]->GetDataPtr("ptr_CECIvtrItem"))
				{
					iMaxSlot++;
				}
			}
			if (iMax>iMaxSlot)
			{
				iMax = iMaxSlot;
			}
			GetGameUIMan()->InvokeNumberDialog((AUIObject*)nSlot, NULL,
				CDlgInputNO::INPUTNO_DRAG_DIAMOND, iMax);
		}
*/	}
	else
		return;
	
	UpdateDisplay();
}

void CDlgBijouUpgrade::ServerStart()
{
	if(!m_bPutProp)
	{
		GetGameUIMan()->MessageBox(1537);
		return;
	}
	else if (!UpdateMoney())
	{
		GetGameUIMan()->MessageBox(7191);
		return;
	}
	else if (GetHostPlayer()->GetPack()->SearchEmpty()<0)
	{
		GetGameUIMan()->MessageBox(7076);
		return;
	}

	CECIvtrGem* pGem = (CECIvtrGem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
	if (pGem->IsLocked())
	{
		GetGameUIMan()->MessageBox(7035);
		return;
	}
	GetGameSession()->c2s_CmdNPCSevUpgradeGems(m_pImg_Item->GetData(), pGem->GetTemplateID(), m_iArticleSlots, m_iArticleTemplId);
	OnEventLButtonDown(0, 0, m_pImg_Item);
	for (int i=0; i<PROP_NUM_MAX; ++i)
	{
		OnEventLButtonDown(0, 0, m_pImgProp[i]);
	}
}

void CDlgBijouUpgrade::UpdateDisplay()
{
	bool bEnable = true;
	m_iMoney = 0;
	int iCurLevel;
	int iQuality = 0;
	if (m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))
	{
		CECIvtrGem* pGem = (CECIvtrGem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		GetGameUIMan()->SetIvtrInfo(m_pImg_Item, pGem);
		m_iMoney = pGem->GetDBEssence()->fee_upgrade;
		iCurLevel = pGem->GetDBEssence()->grade;
		iQuality = pGem->GetDBEssence()->quality;

		elementdataman* pElementDataMan = GetGame()->GetElementDataMan();
		DATA_TYPE dt;
		GEM_CONFIG* pConfig = (GEM_CONFIG*)pElementDataMan->get_data_ptr(
			pGem->GetDBEssence()->gem_config_id, ID_SPACE_CONFIG,dt); 
		if (pConfig && dt == DT_GEM_CONFIG )
		{
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(pConfig->gem_id[pGem->GetDBEssence()->grade].ordinary_gem_id, 0, 1);
			if(pItem)
			{
				GetGameUIMan()->SetIvtrInfo(m_pImgDestNormal, pItem);
				delete pItem;
			}
			pItem = CECIvtrItem::CreateItem(pConfig->gem_id[pGem->GetDBEssence()->grade].perfect_gem_id, 0, 1);
			if(pItem)
			{
				GetGameUIMan()->SetIvtrInfo(m_pImgDestGood, pItem);
				delete pItem;
			}
		}
	}
	else
	{
		m_pImg_Item->ClearCover();
		m_pImg_Item->SetHint(_AL(""));
		m_pImgDestNormal->ClearCover();
		m_pImgDestNormal->SetHint(_AL(""));
		m_pImgDestGood->ClearCover();
		m_pImgDestGood->SetHint(_AL(""));
		bEnable = false;
	}

	float prob = .0f;

	m_bPutProp = false;
	for (int i=0; i<PROP_NUM_MAX; ++i)
	{
		if (m_pImgProp[i]->GetDataPtr("ptr_CECIvtrItem"))
		{
			GetGameUIMan()->SetIvtrInfo(m_pImgProp[i], (CECIvtrItem*)m_pImgProp[i]->GetDataPtr("ptr_CECIvtrItem"));
			m_bPutProp = true;
		}
		else
		{
			m_pImgProp[i]->ClearCover();
			m_pImgProp[i]->SetHint(_AL(""));
		}
	}
	if (bEnable)
	{
		for (int i=0; i<PROP_NUM_MAX; ++i)
		{
			if (m_pImgProp[i]->GetDataPtr("ptr_CECIvtrItem"))
			{
				CECIvtrGemDust* pDust = (CECIvtrGemDust*)m_pImgProp[i]->GetDataPtr("ptr_CECIvtrItem");
				if (iQuality==0)
					prob += pDust->GetDBEssence()->upgrade_prob[iCurLevel-1].ordinary_upgrade_prob;
				else
					prob += pDust->GetDBEssence()->upgrade_prob[iCurLevel-1].perfect_upgrade_prob;
				m_iArticleSlots[i] = m_pImgProp[i]->GetData();
				m_iArticleTemplId[i] = pDust->GetTemplateID();
			}
			else
			{
				m_iArticleSlots[i] = -1;
				m_iArticleTemplId[i] = 0;
			}
		}
	}
	ACString strText;
	if (int(prob*100)>=100)
		strText = _AL("100%");
	else
	{
		bEnable = false;
		strText.Format(_AL("%d%%"), int(prob*100));
	}
	m_pLabProbNormal->SetText(strText);

	static const float scalePerfect[] = {.9f, .85f, .8f, .75f};
	if (int(prob*100)>=100)
	{
		int probPerfect = int((prob-1)*scalePerfect[iCurLevel-1]*100);
		if(probPerfect>=100)
			strText = _AL("100%");
		else
			strText.Format(_AL("%d%%"), probPerfect);
	}
	else
		strText = _AL("0%");
	m_pLabProbGood->SetText(strText);
	
	if(!UpdateMoney())
		bEnable = false;

	m_pBtn_Confirm->Enable(bEnable);
}

void CDlgBijouUpgrade::Reset()
{
	for (int i=0; i<PROP_NUM_MAX; ++i)
	{
		OnEventLButtonDownDust(0, 0, m_pImgProp[i]);
	}
	UpdateDisplay();
}


//////////////////////////////////////////////////////////////////////////
// CDlgBijouRefine
//////////////////////////////////////////////////////////////////////////

CDlgBijouRefine::CDlgBijouRefine()
{
	m_pImgProp = NULL;
	m_pImgDest = NULL;
}

CDlgBijouRefine::~CDlgBijouRefine()
{
}

bool CDlgBijouRefine::OnInitDialog()
{
	DDX_Control("Img_Prop", m_pImgProp);
	DDX_Control("Img_Dest", m_pImgDest);
	return CDlgItemSvrBase::OnInitDialog();
}

void CDlgBijouRefine::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if ( pItem->GetClassID()==CECIvtrItem::ICID_GEM && (pImg==m_pImg_Item||!pImg) )
	{
		CECIvtrGem* pGem = (CECIvtrGem*)pItem;
		if (pGem->GetDBEssence()->quality==1)
		{
			GetGameUIMan()->MessageBox(1538);
			return;
		}

		CECIvtrItem *pItemOrg = (CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		if( pItemOrg )
			pItemOrg->Freeze(false);
		m_pImg_Item->SetData(nSlot);
		m_pImg_Item->SetDataPtr(pItem, "ptr_CECIvtrItem");
		pItem->Freeze(true);
	}
	else if ( ( pItem->GetTemplateID()==GetGameRun()->GetSpecialIDConfig().id_gem_refine_article ||
		pItem->GetTemplateID()==GetGameRun()->GetSpecialIDConfig().id_gem_refine_article_1 ||
		pItem->GetTemplateID()==GetGameRun()->GetSpecialIDConfig().id_gem_refine_article_2) && (pImg==m_pImgProp||!pImg) )
	{
		CECIvtrItem *pItemOrg = (CECIvtrItem*)m_pImgProp->GetDataPtr("ptr_CECIvtrItem");
		if( pItemOrg )
			pItemOrg->Freeze(false);
		m_pImgProp->SetData(nSlot);
		m_pImgProp->SetDataPtr(pItem, "ptr_CECIvtrItem");
		pItem->Freeze(true);
	}
	else
		return;

	UpdateDisplay();
}

void CDlgBijouRefine::ServerStart()
{
	if (!UpdateMoney())
	{
		GetGameUIMan()->MessageBox(7191);
		return;
	}
	
	CECIvtrItem* pItem = (CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
	if (pItem->IsLocked())
	{
		GetGameUIMan()->MessageBox(7035);
		return;
	}
	pItem = (CECIvtrItem*)m_pImgProp->GetDataPtr("ptr_CECIvtrItem");
	if (pItem->IsLocked())
	{
		GetGameUIMan()->MessageBox(7035);
		return;
	}
	int iGemTemplId = ((CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))->GetTemplateID();
	int iPropTemplId = ((CECIvtrItem*)m_pImgProp->GetDataPtr("ptr_CECIvtrItem"))->GetTemplateID();
	GetGameSession()->c2s_CmdNPCSevRefineGems(m_pImg_Item->GetData(), iGemTemplId, m_pImgProp->GetData(), iPropTemplId);
	OnEventLButtonDown(0, 0, m_pImg_Item);
	OnEventLButtonDown(0, 0, m_pImgProp);
}

void CDlgBijouRefine::UpdateDisplay()
{
	bool bEnable = true;
	m_iMoney = 0;
	if (m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))
	{
		CECIvtrGem* pGem = (CECIvtrGem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		GetGameUIMan()->SetIvtrInfo(m_pImg_Item, pGem);
				
		elementdataman* pElementDataMan = GetGame()->GetElementDataMan();
		DATA_TYPE dt;
		GEM_CONFIG* pConfig = (GEM_CONFIG*)pElementDataMan->get_data_ptr(
			pGem->GetDBEssence()->gem_config_id, ID_SPACE_CONFIG,dt); 
		if (pConfig && dt == DT_GEM_CONFIG )
		{
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(pConfig->gem_id[pGem->GetDBEssence()->grade-1].perfect_gem_id, 0, 1);
			if(pItem)
			{
				GetGameUIMan()->SetIvtrInfo(m_pImgDest, pItem);
				delete pItem;
			}
		}

		m_iMoney = GetGameRun()->GetSpecialIDConfig().fee_gem_refine;
	}
	else
	{
		m_pImg_Item->ClearCover();
		m_pImg_Item->SetHint(_AL(""));
		m_pImgDest->ClearCover();
		m_pImgDest->SetHint(_AL(""));
		bEnable = false;
	}

	if (m_pImgProp->GetDataPtr("ptr_CECIvtrItem"))
	{
		GetGameUIMan()->SetIvtrInfo(m_pImgProp, (CECIvtrItem*)m_pImgProp->GetDataPtr("ptr_CECIvtrItem"));
	}
	else
	{
		m_pImgProp->ClearCover();
		m_pImgProp->SetHint(_AL(""));
		bEnable = false;
	}
	if(!UpdateMoney())
		bEnable = false;
	
	m_pBtn_Confirm->Enable(bEnable);
}

void CDlgBijouRefine::Reset()
{
	OnEventLButtonDown(0, 0, m_pImgProp);
}


//////////////////////////////////////////////////////////////////////////
// CDlgBijouExtract
//////////////////////////////////////////////////////////////////////////

CDlgBijouExtract::CDlgBijouExtract()
{
/*	for (int i=0; i<POWDER_NUM; ++i)
	{
		m_pImgPowder[i] = NULL;
		m_pLabProb[i] = NULL;
	}
*/}

CDlgBijouExtract::~CDlgBijouExtract()
{
}

bool CDlgBijouExtract::OnInitDialog()
{
/*	DDX_Control("Img_Ash1", m_pImgPowder[0]);
	DDX_Control("Img_Ash2", m_pImgPowder[1]);
	DDX_Control("Img_Ash3",	m_pImgPowder[2]);
	DDX_Control("Lab_Percent1", m_pLabProb[0]);
	DDX_Control("Lab_Percent2", m_pLabProb[1]);
	DDX_Control("Lab_Percent3", m_pLabProb[2]);
*/	return CDlgItemSvrBase::OnInitDialog();
}

void CDlgBijouExtract::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if ( pItem->GetClassID()==CECIvtrItem::ICID_GEM && (pImg==m_pImg_Item||!pImg) )
	{
		CECIvtrItem *pItemOrg = (CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		if( pItemOrg )
			pItemOrg->Freeze(false);
		m_pImg_Item->SetData(nSlot);
		m_pImg_Item->SetDataPtr(pItem, "ptr_CECIvtrItem");
		pItem->Freeze(true);
	}
	else
		return;
	
	UpdateDisplay();
}

void CDlgBijouExtract::ServerStart()
{
	if (!UpdateMoney())
	{
		GetGameUIMan()->MessageBox(7191);
		return;
	}
	
	CECIvtrItem* pItem = (CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
	if (pItem->IsLocked())
	{
		GetGameUIMan()->MessageBox(7035);
		return;
	}
	GetGameUIMan()->MessageBox("Game_Extract_Bijou", GetGameUIMan()->GetStringFromTable(1546), MB_YESNO);
}

void CDlgBijouExtract::UpdateDisplay()
{
	bool bEnable = true;
	m_iMoney = 0;
	if (m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))
	{
		CECIvtrGem* pGem = (CECIvtrGem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		GetGameUIMan()->SetIvtrInfo(m_pImg_Item, pGem);
		m_iMoney = GetGameRun()->GetSpecialIDConfig().fee_gem_refine;
/*		elementdataman* pElementDataMan = GetGame()->GetElementDataMan();
		DATA_TYPE dt;
		GEM_EXTRACT_CONFIG* pConfig = (GEM_EXTRACT_CONFIG*)pElementDataMan->get_data_ptr(
			pGem->GetDBEssence()->gem_extract_config_id, ID_SPACE_CONFIG,dt); 
		if (pConfig && dt == DT_GEM_EXTRACT_CONFIG )
		{
			for(int i=0; i<POWDER_NUM; ++i)
			{
				CECIvtrItem* pItem = CECIvtrItem::CreateItem(pConfig->gem_dust_config[i].gem_dust_id, 0, pConfig->gem_dust_config[i].gem_dust_num);
				if (pItem)
				{
					GetGameUIMan()->SetIvtrInfo(m_pImgPowder[i], pItem, pConfig->gem_dust_config[i].gem_dust_num);
					delete pItem;
				}
			}
		}
*/	}
	else
	{
		m_pImg_Item->ClearCover();
		m_pImg_Item->SetHint(_AL(""));
/*		for(int i=0; i<POWDER_NUM; ++i)
		{
			m_pImgPowder[i]->ClearCover();
			m_pImgPowder[i]->SetHint(_AL(""));
		}
*/		bEnable = false;
	}

	if(!UpdateMoney())
		bEnable = false;
	
	m_pBtn_Confirm->Enable(bEnable);
}

int CDlgBijouExtract::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="extract_confirm")
	{
		int iGemTemplId = ((CECIvtrItem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))->GetTemplateID();
		GetGameSession()->c2s_CmdNPCSevExtractGems(m_pImg_Item->GetData(), iGemTemplId);
		OnEventLButtonDown(0, 0, m_pImg_Item);
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////
// CDlgBijouTransfer
//////////////////////////////////////////////////////////////////////////

CDlgBijouTransfer::CDlgBijouTransfer()
{
	m_pImgProp = NULL;
	m_pImgDest = NULL;
}

CDlgBijouTransfer::~CDlgBijouTransfer()
{
}

bool CDlgBijouTransfer::OnInitDialog()
{
	DDX_Control("Img_Prop", m_pImgProp);
	DDX_Control("Img_Dest", m_pImgDest);
	return CDlgItemSvrBase::OnInitDialog();
}

void CDlgBijouTransfer::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if ( ( pItem->GetTemplateID()==GetGameRun()->GetSpecialIDConfig().id_gem_smelt_article ||
		pItem->GetTemplateID()==GetGameRun()->GetSpecialIDConfig().id_gem_smelt_article_1 ||
		 pItem->GetTemplateID()==GetGameRun()->GetSpecialIDConfig().id_gem_smelt_article_2)
		&& (pImg==m_pImgProp||!pImg) )
	{
		CECIvtrItem *pItemOrg = (CECIvtrItem*)m_pImgProp->GetDataPtr("ptr_CECIvtrItem");
		if( pItemOrg )
			pItemOrg->Freeze(false);
		m_pImgProp->SetData(nSlot);
		m_pImgProp->SetDataPtr(pItem, "ptr_CECIvtrItem");
		pItem->Freeze(true);
	}
	else if ( pItem->GetClassID()==CECIvtrItem::ICID_GEM && (!pImg||pImg==m_pImgDest||pImg==m_pImg_Item) )
	{
		if (!pImg)
		{
			if (!m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))
				pImg = m_pImg_Item;
			else if (!m_pImgDest->GetDataPtr("ptr_CECIvtrItem"))
				pImg = m_pImgDest;
			else
				return;
		}

		CECIvtrItem *pItemOrg = (CECIvtrItem*)pImg->GetDataPtr("ptr_CECIvtrItem");
		if( pItemOrg )
			pItemOrg->Freeze(false);
		pImg->SetData(nSlot);
		pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
		pItem->Freeze(true);
	}
	else
		return;
	
	UpdateDisplay();
}

void CDlgBijouTransfer::ServerStart()
{
	if (!UpdateMoney())
	{
		GetGameUIMan()->MessageBox(7191);
		return;
	}

	CECIvtrGem* srcGem = (CECIvtrGem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
	CECIvtrGem* dstGem = (CECIvtrGem*)m_pImgDest->GetDataPtr("ptr_CECIvtrItem");
	
	if (srcGem->IsLocked())
	{
		GetGameUIMan()->MessageBox(7035);
		return;
	}
	if (dstGem->IsLocked())
	{
		GetGameUIMan()->MessageBox(7035);
		return;
	}
	
	if (srcGem->GetDBEssence()->grade<=dstGem->GetDBEssence()->grade)
	{
		GetGameUIMan()->MessageBox(1539);
		return;
	}
	else if (srcGem->GetDBEssence()->quality!=dstGem->GetDBEssence()->quality)
	{
		GetGameUIMan()->MessageBox(2002);
		return;
	}

	int iGemTemplId = ((CECIvtrGem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))->GetTemplateID();
	int iDstTemplId = ((CECIvtrGem*)m_pImgDest->GetDataPtr("ptr_CECIvtrItem"))->GetTemplateID();
	int iPropTemplId = ((CECIvtrItem*)m_pImgProp->GetDataPtr("ptr_CECIvtrItem"))->GetTemplateID();
	GetGameSession()->c2s_CmdNPCSevSmeltGems(m_pImg_Item->GetData(), iGemTemplId, m_pImgDest->GetData(), iDstTemplId, m_pImgProp->GetData(), iPropTemplId);
	OnEventLButtonDown(0, 0, m_pImg_Item);
	OnEventLButtonDown(0, 0, m_pImgProp);
	OnEventLButtonDown(0, 0, m_pImgDest);
}

void CDlgBijouTransfer::UpdateDisplay()
{
	bool bEnable = true;
	m_iMoney = 0;
	int srcGrade = -1;
	int dstGrade = -1;
	if (m_pImg_Item->GetDataPtr("ptr_CECIvtrItem"))
	{
		CECIvtrGem* pGem = (CECIvtrGem*)m_pImg_Item->GetDataPtr("ptr_CECIvtrItem");
		GetGameUIMan()->SetIvtrInfo(m_pImg_Item, pGem);
		srcGrade = pGem->GetDBEssence()->grade;
		m_iMoney = GetGameRun()->GetSpecialIDConfig().fee_gem_smelt;
	}
	else
	{
		m_pImg_Item->ClearCover();
		m_pImg_Item->SetHint(_AL(""));
		bEnable = false;
	}

	if (m_pImgDest->GetDataPtr("ptr_CECIvtrItem"))
	{
		CECIvtrGem* pGem = (CECIvtrGem*)m_pImgDest->GetDataPtr("ptr_CECIvtrItem");
		GetGameUIMan()->SetIvtrInfo(m_pImgDest, pGem);
		dstGrade = pGem->GetDBEssence()->grade;
	}
	else
	{
		m_pImgDest->ClearCover();
		m_pImgDest->SetHint(_AL(""));
		bEnable = false;
	}
	
	if (m_pImgProp->GetDataPtr("ptr_CECIvtrItem"))
	{
		GetGameUIMan()->SetIvtrInfo(m_pImgProp, (CECIvtrItem*)m_pImgProp->GetDataPtr("ptr_CECIvtrItem"));
	}
	else
	{
		m_pImgProp->ClearCover();
		m_pImgProp->SetHint(_AL(""));
		bEnable = false;
	}
	if(!UpdateMoney())
		bEnable = false;
	// ÅÐ¶ÏÆ·¼¶
	if (bEnable)
		bEnable = srcGrade>dstGrade;
	
	m_pBtn_Confirm->Enable(bEnable);
}

void CDlgBijouTransfer::Reset()
{
	OnEventLButtonDown(0, 0, m_pImgProp);
	OnEventLButtonDown(0, 0, m_pImgDest);
}
