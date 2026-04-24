// Filename	: DlgRuneOperate.cpp
// Creator	: Fu Chonggang
// Date		: 2012.4.21

#include "DlgRuneOperate.h"
#include "DlgRuneMain.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIProgress.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrRune.h"
#include "EC_GameSession.h"
#include "EC_Game.h"
#include "EC_IvtrTypes.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW


//////////////////////////////////////////////////////////////////////////
//  元魂洗练
//////////////////////////////////////////////////////////////////////////
AUI_BEGIN_COMMAND_MAP(CDlgRuneRefine, CDlgItemSvrBase)

AUI_ON_COMMAND("Btn_Keep1",			OnCommandKeep)
AUI_ON_COMMAND("Btn_Keep2",			OnCommandChange)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)

AUI_END_COMMAND_MAP()

const int CDlgRuneRefine::ms_InternalTime = 2200;

CDlgRuneRefine::CDlgRuneRefine()
{
	m_iIndexAssist = -1;
}

CDlgRuneRefine::~CDlgRuneRefine()
{
}

bool CDlgRuneRefine::OnInitDialog()
{
	elementdataman* pDB = GetGame()->GetElementDataMan();
	DATA_TYPE DataType = DT_INVALID;
	const void* p = pDB->get_data_ptr(CDlgRuneMain::ms_idTmpConfig, ID_SPACE_CONFIG, DataType);
	assert(DataType==DT_RUNE_DATA_CONFIG);
	const RUNE_DATA_CONFIG* pConfig = (const RUNE_DATA_CONFIG*)p;
	int i;
	for(i=0; i<5; ++i)
		m_iAssistId[i] = pConfig->id_rune_refine[i];
	
	m_iChangeDelta = 0;
	m_iLastTimeServe = 0;
	
	return CDlgItemSvrBase::OnInitDialog();
}

void CDlgRuneRefine::OnTick()
{
	CECIvtrRune *pRune = (CECIvtrRune*)GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	if (!pRune)
	{
		Show(false);
		return;
	}
	bool bRefresh=false;
	int i;
	for (i=0; i<8; ++i)
	{
		if (pRune->GetEssence().refine_prop[i].id!=m_iId[i] || pRune->GetEssence().refine_prop[i].value!=m_iValue[i] )
		{
			bRefresh = true;
			int j;
			for (j=0; j<8; ++j)
			{
				m_iId[j] = pRune->GetEssence().refine_prop[j].id;
				m_iValue[j] = pRune->GetEssence().refine_prop[j].value;
			}
			break;
		}
	}
	if (bRefresh)
	{
		CDlgRuneMain *pRuneMain = (CDlgRuneMain*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying");
		pRuneMain->DisplayOneProp(pRune->GetEssence().prop[0].id, pRune->GetEssence().prop[0].value, "A01",	this);
		pRuneMain->DisplayOneProp(pRune->GetEssence().prop[1].id, pRune->GetEssence().prop[1].value, "A02",	this);
		pRuneMain->DisplayOneProp(pRune->GetEssence().prop[2].id, pRune->GetEssence().prop[2].value, "A03",	this);
		pRuneMain->DisplayOneProp(pRune->GetEssence().prop[3].id, pRune->GetEssence().prop[3].value, "A04",	this);
		pRuneMain->DisplayOneProp(pRune->GetEssence().prop[4].id, pRune->GetEssence().prop[4].value, "A05",	this);
		pRuneMain->DisplayOneProp(pRune->GetEssence().prop[5].id, pRune->GetEssence().prop[5].value, "A06",	this);
		pRuneMain->DisplayOneProp(pRune->GetEssence().prop[6].id, pRune->GetEssence().prop[6].value, "A07",	this);
		pRuneMain->DisplayOneProp(pRune->GetEssence().prop[7].id, pRune->GetEssence().prop[7].value, "A08",	this);
		int i;
		for (i=0; i<8; ++i)
		{
			AString strName;
			strName.Format("Txt_C%02d", i+1);
			PAUILABEL pLabComp = (PAUILABEL)GetDlgItem(strName);
			pLabComp->SetText(_AL(""));
		}
		PAUITEXTAREA pTxtResult = (PAUITEXTAREA)GetDlgItem("Txt_Contrast");
		pTxtResult->SetText(_AL(""));
		
		if (pRune->GetEssence().refine_active)
		{
			pRuneMain->DisplayOneProp(pRune->GetEssence().refine_prop[0].id, pRune->GetEssence().refine_prop[0].value, "B01",	this);
			pRuneMain->DisplayOneProp(pRune->GetEssence().refine_prop[1].id, pRune->GetEssence().refine_prop[1].value, "B02",	this);
			pRuneMain->DisplayOneProp(pRune->GetEssence().refine_prop[2].id, pRune->GetEssence().refine_prop[2].value, "B03",	this);
			pRuneMain->DisplayOneProp(pRune->GetEssence().refine_prop[3].id, pRune->GetEssence().refine_prop[3].value, "B04",	this);
			pRuneMain->DisplayOneProp(pRune->GetEssence().refine_prop[4].id, pRune->GetEssence().refine_prop[4].value, "B05",	this);
			pRuneMain->DisplayOneProp(pRune->GetEssence().refine_prop[5].id, pRune->GetEssence().refine_prop[5].value, "B06",	this);
			pRuneMain->DisplayOneProp(pRune->GetEssence().refine_prop[6].id, pRune->GetEssence().refine_prop[6].value, "B07",	this);
			pRuneMain->DisplayOneProp(pRune->GetEssence().refine_prop[7].id, pRune->GetEssence().refine_prop[7].value, "B08",	this);
			int i;
			ACString strText;
			ACString strTemp;
			float fDeltaResult = .0f;
			int iNumProp = 0;
			for (i=0; i<8; ++i)
			{
				AString strName;
				strName.Format("Txt_C%02d", i+1);
				PAUILABEL pLabComp = (PAUILABEL)GetDlgItem(strName);
				if (pRune->GetEssence().refine_prop[i].id>0)
				{
					iNumProp++;
					int iDelta = pRune->GetEssence().refine_prop[i].value-pRune->GetEssence().prop[i].value;
					bool bNegative = iDelta<0;
					if (bNegative)
						iDelta = -iDelta;
					float fDeltaPercent = iDelta*100.0f/CDlgRuneMain::ms_iDefaultPropValue[pRune->GetEssence().refine_prop[i].id];
					if (bNegative)
						fDeltaResult -= fDeltaPercent;
					else
						fDeltaResult += fDeltaPercent;
					strTemp.Format(_AL("%.2f"), fDeltaPercent);

					strText = pRune->GetPropString(pRune->GetEssence().refine_prop[i].id, iDelta);
					if (bNegative)
					{
						strText = _AL("-")+strText;
					}
					if (pRune->GetEssence().refine_prop[i].value<pRune->GetEssence().prop[i].value)
					{
						strText = _AL("^ff8080")+strText;
						strText += _AL("(↓");
						strText += strTemp;
						strText += _AL("%)");
					}
					else if (pRune->GetEssence().refine_prop[i].value>pRune->GetEssence().prop[i].value)
					{
						strText = _AL("^80ff80")+strText;
						strText += _AL("(↑");
						strText += strTemp;
						strText += _AL("%)");
					}
					pLabComp->SetText(strText);
				}
				else
				{
					pLabComp->SetText(_AL(""));
				}
			}

			if (fDeltaResult>0)
			{
				m_iChangeDelta = 1;
				strText.Format(GetGameUIMan()->GetStringFromTable(13255), fDeltaResult/iNumProp);
			}
			else
			{
				m_iChangeDelta = -1;
				strText.Format(GetGameUIMan()->GetStringFromTable(13256), -fDeltaResult/iNumProp);
			}
			pTxtResult->SetText(strText);
		}
		else
		{
			pRuneMain->DisplayOneProp(0, pRune->GetEssence().refine_prop[0].value, "B01",	this);
			pRuneMain->DisplayOneProp(0, pRune->GetEssence().refine_prop[1].value, "B02",	this);
			pRuneMain->DisplayOneProp(0, pRune->GetEssence().refine_prop[2].value, "B03",	this);
			pRuneMain->DisplayOneProp(0, pRune->GetEssence().refine_prop[3].value, "B04",	this);
			pRuneMain->DisplayOneProp(0, pRune->GetEssence().refine_prop[4].value, "B05",	this);
			pRuneMain->DisplayOneProp(0, pRune->GetEssence().refine_prop[5].value, "B06",	this);
			pRuneMain->DisplayOneProp(0, pRune->GetEssence().refine_prop[6].value, "B07",	this);
			pRuneMain->DisplayOneProp(0, pRune->GetEssence().refine_prop[7].value, "B08",	this);
		}

		ACString strText;
		PAUILABEL	pLabAP = (PAUILABEL)GetDlgItem("Txt_AP");
		strText.Format(_AL("%d"), pRune->GetEssence().refine_remain);
		pLabAP->SetText(strText);
		pLabAP = (PAUILABEL)GetDlgItem("Txt_AP2");
		strText.Format(_AL("%d"), pRune->GetEssence().cash_refine_total);
		pLabAP->SetText(strText);
	}
	m_pBtn_Confirm->Enable(GetDlgItem("Img_Pic")->GetDataPtr("ptr_CECIvtrItem")!=NULL || pRune->GetEssence().refine_remain>0);
	GetDlgItem("Btn_Keep1")->Enable(pRune->GetEssence().refine_active);
	GetDlgItem("Btn_Keep2")->Enable(pRune->GetEssence().refine_active);
	if (m_iIndexAssist>=0 && !GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist))
	{
		OnEventLButtonDown(0, 0, GetDlgItem("Img_Pic"));
		m_iIndexAssist = -1;
		GetDlgItem("Img_Pic")->SetHint(GetGameUIMan()->GetStringFromTable(13265));
	}
	
	if (GetDlgItem("Img_Pic")->GetDataPtr("ptr_CECIvtrItem")==NULL && m_iIndexAssist>0)
	{
		m_iIndexAssist = -1;
		GetDlgItem("Img_Pic")->SetHint(GetGameUIMan()->GetStringFromTable(13265));
	}
	
	if (m_iLastTimeServe>0)
	{
		m_iLastTimeServe -= GetGame()->GetTickTime();
	}
}

void CDlgRuneRefine::ServerStart()
{
	if (m_iLastTimeServe>0)
	{
		return;
	}

	if(GetDlgItem("Img_Pic")->GetDataPtr("ptr_CECIvtrItem"))
		GetGame()->GetGameSession()->c2s_CmdRuneRefine(EQUIPIVTR_RUNE, m_iIndexAssist);
	else
		GetGame()->GetGameSession()->c2s_CmdRuneRefine(EQUIPIVTR_RUNE, -1);	// 不用道具
	m_iLastTimeServe = ms_InternalTime;
}

void CDlgRuneRefine::Reset()
{
	OnEventLButtonDown(0, 0, GetDlgItem("Img_Pic"));
	m_iIndexAssist = -1;
	GetDlgItem("Img_Pic")->SetHint(GetGameUIMan()->GetStringFromTable(13265));
}

void CDlgRuneRefine::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if (pImg && pImg!=GetDlgItem("Img_Pic"))
	{
		return;
	}

	int i;
	for (i=0; i<5; ++i)
	{
		if (pItem->GetTemplateID()==m_iAssistId[i])
		{
			break;
		}
	}
	if (i==5)
	{
		return;
	}
	
	if (m_iIndexAssist>=0)
	{
		if(GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist))
			GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist)->Freeze(false);
		m_iIndexAssist = -1;
	}
	
	if(!pImg)
		pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Pic");
	pItem->Freeze(true);
	GetGameUIMan()->SetIvtrInfo(pImg, pItem->GetTemplateID());
	pItem->Freeze(true);
	pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
	m_iIndexAssist = nSlot;
}



void CDlgRuneRefine::OnCommandKeep(const char* szCommand)
{
	GetGame()->GetGameSession()->c2s_CmdRefineAction(false, EQUIPIVTR_RUNE);
}

void CDlgRuneRefine::OnCommandChange(const char* szCommand)
{
	if (m_iChangeDelta<0)
	{
		GetGameUIMan()->MessageBox("Game_RuneRefineChange", GetGameUIMan()->GetStringFromTable(13259), MB_YESNO);
	}
	else
		GetGame()->GetGameSession()->c2s_CmdRefineAction(true, EQUIPIVTR_RUNE);
}

void CDlgRuneRefine::SetRune()
{
	Show(true);
	CECIvtrItem *pItem = GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	if (!pItem || pItem->GetClassID()!=CECIvtrItem::ICID_RUNE)
	{
		Show(false);
		return;
	}
	
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
	GetGameUIMan()->SetIvtrInfo(pImg, pItem);
	m_pImg_Item->SetDataPtr(pItem, "ptr_CECIvtrItem");
	for (int i=0; i<8; ++i)
	{
		m_iId[i] = -1;
		m_iValue[i] = -1;
	}

	CECIvtrRune *pRune = (CECIvtrRune*)pItem;
	if (m_iIndexAssist<0)
	{
		int i;
		int iIndex;
		for (i=0; i<5; ++i)
		{
			if ( (iIndex=GetHostPlayer()->GetPack()->FindItem(m_iAssistId[i]))>=0 )
			{
				SetItem(GetHostPlayer()->GetPack()->GetItem(iIndex), iIndex);
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//  元魂升级
//////////////////////////////////////////////////////////////////////////
CDlgRuneLevelUp::CDlgRuneLevelUp()
{
	m_iIndexAssist = -1;
}

CDlgRuneLevelUp::~CDlgRuneLevelUp()
{
}

void CDlgRuneLevelUp::ServerStart()
{
	CECIvtrItem * pItem_d = (CECIvtrItem *) GetDlgItem("Item_d")->GetDataPtr("ptr_CECIvtrItem");
	if ( pItem_d )
	{
		const RUNE_ENHANCED_ESSENCE essence = ( (CECIvtrRune*) pItem_d )->GetEssence( );
		int propcount = 0;
		for ( int i = 0; i < 8; ++ i )
		{
			if ( essence.prop[i].id != 0 )
				++ propcount;
		}

		if ( ( essence.quality >= 3 || propcount >= 6 ) )
		{
			GetGameUIMan()->MessageBox( "Game_RuneLevelUp", GetGameUIMan()->GetStringFromTable( 18800 ),  MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160) );
		}
		else
		{
			GetGame()->GetGameSession()->c2s_CmdRuneCombine(EQUIPIVTR_RUNE, m_iIndexAssist);
			OnEventLButtonDown(0, 0, GetDlgItem("Item_d"));
		}
	}
}

void CDlgRuneLevelUp::Reset()
{
	OnEventLButtonDown(0, 0, GetDlgItem("Item_d"));
	m_iIndexAssist = -1;
}

void CDlgRuneLevelUp::ServerStartWithoutCheck()
{
	CECIvtrItem * pItem_d = (CECIvtrItem *) GetDlgItem("Item_d")->GetDataPtr("ptr_CECIvtrItem");
	if ( pItem_d )
	{
		GetGame()->GetGameSession()->c2s_CmdRuneCombine(EQUIPIVTR_RUNE, m_iIndexAssist);
		OnEventLButtonDown(0, 0, GetDlgItem("Item_d"));
	}
}

void CDlgRuneLevelUp::SetRune()
{
	Show(true);
	CECIvtrItem *pItem = GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	if (!pItem || pItem->GetClassID()!=CECIvtrItem::ICID_RUNE)
	{
		Show(false);
		return;
	}
	
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
	GetGameUIMan()->SetIvtrInfo(pImg, pItem);
	m_pImg_Item->SetDataPtr(pItem, "ptr_CECIvtrItem");
}

void CDlgRuneLevelUp::OnTick() 
{
	CECIvtrRune *pRune = (CECIvtrRune*)GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	if (!pRune)
	{
		Show(false);
		return;
	}
}

void CDlgRuneLevelUp::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if (pImg && pImg!=GetDlgItem("Item_d"))
	{
		return;
	}

	if (pItem->GetClassID()!=CECIvtrItem::ICID_RUNE)
	{
		return;
	}

	CECIvtrRune *pRune = (CECIvtrRune*)pItem;
	if (pRune->GetEssence().level<=0)
	{
		GetGameUIMan()->MessageBox(13251);
		return;
	}
	
	if (m_iIndexAssist>=0)
	{
		if(GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist))
			GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist)->Freeze(false);
		m_iIndexAssist = -1;
	}
	if(!pImg)
		pImg = (PAUIIMAGEPICTURE)GetDlgItem("Item_d");
	GetGameUIMan()->SetIvtrInfo(pImg, pItem);
	pItem->Freeze(true);
	pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
	m_iIndexAssist = nSlot;
}





//////////////////////////////////////////////////////////////////////////
//  元魂归元
//////////////////////////////////////////////////////////////////////////

const int CDlgRuneReset::ms_InternalTime = 2200;

CDlgRuneReset::CDlgRuneReset()
{
	m_iIndexAssist = -1;
}

CDlgRuneReset::~CDlgRuneReset()
{
}

bool CDlgRuneReset::OnInitDialog()
{
	elementdataman* pDB = GetGame()->GetElementDataMan();
	DATA_TYPE DataType = DT_INVALID;
	const void* p = pDB->get_data_ptr(CDlgRuneMain::ms_idTmpConfig, ID_SPACE_CONFIG, DataType);
	assert(DataType==DT_RUNE_DATA_CONFIG);
	const RUNE_DATA_CONFIG* pConfig = (const RUNE_DATA_CONFIG*)p;
	int i;
	for (i=0; i<5; ++i)
	{
		m_iAssistId[i] = pConfig->id_rune_reset[i];
	}

	m_iLastTimeServe = 0;

	return CDlgItemSvrBase::OnInitDialog();
}

void CDlgRuneReset::OnTick() 
{
	CECIvtrRune *pRune = (CECIvtrRune*)GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	if (!pRune)
	{
		Show(false);
		return;
	}
	if (m_iIndexAssist>=0 && !GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist))
	{
		OnEventLButtonDown(0, 0, GetDlgItem("Item_d"));
	}

	PAUIDIALOG pDlgMouseOn;
	PAUIOBJECT pObjMouseOn;
	GetGameUIMan()->GetMouseOn(&pDlgMouseOn, &pObjMouseOn);
	if (pObjMouseOn && pObjMouseOn == GetDlgItem("Img_Item"))
	{
		GetGameUIMan()->SetIvtrInfo((PAUIIMAGEPICTURE)GetDlgItem("Img_Item"), pRune);
	}
	if (m_iLastTimeServe>0)
	{
		m_iLastTimeServe -= GetGame()->GetTickTime();
	}
	
	if (GetDlgItem("Item_d")->GetDataPtr("ptr_CECIvtrItem")==NULL && m_iIndexAssist>0)
	{
		m_iIndexAssist = -1;
		GetDlgItem("Item_d")->SetHint(GetGameUIMan()->GetStringFromTable(13264));
	}
}

void CDlgRuneReset::ServerStart()
{
	if (m_iLastTimeServe>0)
	{
		return;
	}

	if (GetDlgItem("Item_d")->GetDataPtr("ptr_CECIvtrItem"))
	{
		CECIvtrRune *pRune = (CECIvtrRune*)GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
		int i;
		for (i=0; i<8; ++i)
		{
			if (pRune->GetEssence().prop[i].id==0)
			{
				break;
			}
		}
		// 5个属性以下不提示
		if (i<=5)
		{
			DoServerStart();
		}
		else
		{
			ACString strText;
			PAUIDIALOG pDlg;
			strText.Format(GetGameUIMan()->GetStringFromTable(13260), i);
			GetGameUIMan()->MessageBox("Game_RuneReset", strText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pDlg);
			pDlg->SetData(m_iIndexAssist);
		}
	}
}

void CDlgRuneReset::Reset()
{
	OnEventLButtonDown(0, 0, GetDlgItem("Item_d"));
	m_iIndexAssist = -1;
	GetDlgItem("Item_d")->SetHint(GetGameUIMan()->GetStringFromTable(13264));
}

void CDlgRuneReset::SetRune()
{
	Show(true);
	CECIvtrItem *pItem = GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	if (!pItem || pItem->GetClassID()!=CECIvtrItem::ICID_RUNE)
	{
		Show(false);
		return;
	}
	
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
	GetGameUIMan()->SetIvtrInfo(pImg, pItem);
	m_pImg_Item->SetDataPtr(pItem, "ptr_CECIvtrItem");
	if (m_iIndexAssist<0)
	{
		int i;
		int iIndex;
		for (i=0; i<5; ++i)
		{
			if ( (iIndex=GetHostPlayer()->GetPack()->FindItem(m_iAssistId[i]))>=0 )
			{
				SetItem(GetHostPlayer()->GetPack()->GetItem(iIndex), iIndex);
			}
		}
	}
}

void CDlgRuneReset::DoServerStart()
{
	GetGame()->GetGameSession()->c2s_CmdRuneReset(EQUIPIVTR_RUNE, m_iIndexAssist);
	m_iLastTimeServe = ms_InternalTime;
}

void CDlgRuneReset::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if (pImg && pImg!=GetDlgItem("Item_d"))
	{
		return;
	}
	
	int i;
	for (i=0; i<5; ++i)
	{
		if (pItem->GetTemplateID()==m_iAssistId[i])
		{
			break;
		}
	}
	if (i==5)
	{
		return;
	}
	
	if (m_iIndexAssist>=0)
	{
		if(GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist))
			GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist)->Freeze(false);
		m_iIndexAssist = -1;
	}
	
	if (!pImg)
	{
		pImg = (PAUIIMAGEPICTURE)GetDlgItem("Item_d");
	}
	GetGameUIMan()->SetIvtrInfo(pImg, pItem->GetTemplateID());
	pItem->Freeze(true);
	pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
	m_iIndexAssist = nSlot;
}




//////////////////////////////////////////////////////////////////////////
//  孔位擦写
//////////////////////////////////////////////////////////////////////////
CDlgSlotErase::CDlgSlotErase()
{
}

CDlgSlotErase::~CDlgSlotErase()
{
}

bool CDlgSlotErase::OnInitDialog()
{
	elementdataman* pDB = GetGame()->GetElementDataMan();
	DATA_TYPE DataType = DT_INVALID;
	const void* p = pDB->get_data_ptr(CDlgRuneMain::ms_idTmpConfig, ID_SPACE_CONFIG, DataType);
	assert(DataType==DT_RUNE_DATA_CONFIG);
	const RUNE_DATA_CONFIG* pConfig = (const RUNE_DATA_CONFIG*)p;
	int i;
	for (i=0; i<5; ++i)
	{
		m_iAssistId[i] = pConfig->id_rune_change_slot[i];
	}
	
	AString strName;
	for (i=0; i<5; ++i)
	{
		strName.Format("Img_RuneShape%d", i+1);
		m_pImgShape[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
	}

	return CDlgItemSvrBase::OnInitDialog();
}

void CDlgSlotErase::OnTick() 
{
	CECIvtrRune *pRune = (CECIvtrRune*)GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	if (!pRune)
	{
		Show(false);
		return;
	}
	int i;
	for (i=0; i<5; ++i)
	{
		m_pImgShape[i]->Show(pRune->GetEssence().hole>i);
	}

	if (m_iIndexAssist>=0 && !GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist))
	{
		OnEventLButtonDown(0, 0, GetDlgItem("Item_a"));
		m_iIndexAssist = -1;
		GetDlgItem("Item_a")->SetHint(GetGameUIMan()->GetStringFromTable(13267));
	}
	
	if (GetDlgItem("Item_a")->GetDataPtr("ptr_CECIvtrItem")==NULL && m_iIndexAssist>0)
	{
		m_iIndexAssist = -1;
		GetDlgItem("Item_a")->SetHint(GetGameUIMan()->GetStringFromTable(13267));
	}
}

void CDlgSlotErase::ServerStart()
{
	if (GetDlgItem("Item_a")->GetDataPtr("ptr_CECIvtrItem"))
	{
		GetGame()->GetGameSession()->c2s_CmdRuneChangeSlot(EQUIPIVTR_RUNE, m_iIndexAssist);
	}
}

void CDlgSlotErase::Reset()
{
	OnEventLButtonDown(0, 0, GetDlgItem("Item_a"));
	m_iIndexAssist = -1;
	GetDlgItem("Item_a")->SetHint(GetGameUIMan()->GetStringFromTable(13267));
}

void CDlgSlotErase::SetRune()
{
	Show(true);
	CECIvtrItem *pItem = GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	if (!pItem || pItem->GetClassID()!=CECIvtrItem::ICID_RUNE)
	{
		Show(false);
		return;
	}

	if (m_iIndexAssist<0)
	{
		int i;
		int iIndex;
		for (i=0; i<5; ++i)
		{
			if ( (iIndex=GetHostPlayer()->GetPack()->FindItem(m_iAssistId[i]))>=0 )
			{
				SetItem(GetHostPlayer()->GetPack()->GetItem(iIndex), iIndex);
			}
		}
	}
}

void CDlgSlotErase::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if (pImg && pImg!=GetDlgItem("Img_Pic"))
	{
		return;
	}	
	
	int i;
	for (i=0; i<5; ++i)
	{
		if (pItem->GetTemplateID()==m_iAssistId[i])
		{
			break;
		}
	}
	if (i==5)
	{
		return;
	}
	
	if (m_iIndexAssist>=0)
	{
		if(GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist))
			GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist)->Freeze(false);
		m_iIndexAssist = -1;
	}
	
	if(!pImg)
		pImg = (PAUIIMAGEPICTURE)GetDlgItem("Item_a");
	GetGameUIMan()->SetIvtrInfo(pImg, pItem->GetTemplateID());
	pItem->Freeze(true);
	pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
	m_iIndexAssist = nSlot;
}





//////////////////////////////////////////////////////////////////////////
//  元魂鉴定
//////////////////////////////////////////////////////////////////////////

CDlgRuneIdentify::CDlgRuneIdentify()
{
	m_iIndexRune = -1;
	m_iIndexAssist = -1;
}

CDlgRuneIdentify::~CDlgRuneIdentify()
{
}

bool CDlgRuneIdentify::OnInitDialog()
{
	elementdataman* pDB = GetGame()->GetElementDataMan();
	DATA_TYPE DataType = DT_INVALID;
	const void* p = pDB->get_data_ptr(CDlgRuneMain::ms_idTmpConfig, ID_SPACE_CONFIG, DataType);
	assert(DataType==DT_RUNE_DATA_CONFIG);
	const RUNE_DATA_CONFIG* pConfig = (const RUNE_DATA_CONFIG*)p;
	int i;
	for (i=0; i<5; ++i)
	{
		m_iAssistId[i] = pConfig->id_rune_identify[i];
	}
	return CDlgItemSvrBase::OnInitDialog();
}

void CDlgRuneIdentify::OnTick() 
{
	if (m_iIndexRune<0)
	{
		Show(false);
		return;
	}

	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(m_iIndexRune);
	if(!pItem)
	{
		Show(false);
		return;
	}
	CECIvtrRune *pRune = (CECIvtrRune*)pItem;
	if (pRune->GetEssence().level>0)
	{
		pItem->Freeze(false);
		Show(false);
		if (m_iIndexAssist>=0 && GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist))
		{
			GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist)->Freeze(false);
		}
	}
	
	if (GetDlgItem("Item_d")->GetDataPtr("ptr_CECIvtrItem")==NULL && m_iIndexAssist>0)
	{
		m_iIndexAssist = -1;
		GetDlgItem("Item_d")->SetHint(GetGameUIMan()->GetStringFromTable(13261));
	}
}

void CDlgRuneIdentify::ServerStart()
{
	if (GetDlgItem("Item_d")->GetDataPtr("ptr_CECIvtrItem"))
	{
		GetGame()->GetGameSession()->c2s_CmdCmdRuneIdentify(m_iIndexRune, m_iIndexAssist);
	}
}

void CDlgRuneIdentify::Reset()
{
	OnEventLButtonDown(0, 0, GetDlgItem("Item_d"));
	m_iIndexAssist = -1;
	GetDlgItem("Item_d")->SetHint(GetGameUIMan()->GetStringFromTable(13261));
}

void CDlgRuneIdentify::SetRune(int index)
{
	Show(true);
	CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(index);
	if (pItem->GetClassID()!=CECIvtrItem::ICID_RUNE)
	{
		return;
	}

	if (m_iIndexRune>=0)
	{
		if(GetHostPlayer()->GetPack()->GetItem(m_iIndexRune))
			GetHostPlayer()->GetPack()->GetItem(m_iIndexRune)->Freeze(false);
	}
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
	GetGameUIMan()->SetIvtrInfo(pImg, pItem);
	pItem->Freeze(true);
	m_pImg_Item->SetDataPtr(pItem, "ptr_CECIvtrItem");
	m_iIndexRune = index;
	if (m_iIndexAssist<0)
	{
		int i;
		int iIndex;
		for (i=0; i<5; ++i)
		{
			if ( (iIndex=GetHostPlayer()->GetPack()->FindItem(m_iAssistId[i]))>=0 )
			{
				SetItem(GetHostPlayer()->GetPack()->GetItem(iIndex), iIndex);
			}
		}
	}
}

void CDlgRuneIdentify::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	if (pImg && pImg!=GetDlgItem("Img_Pic"))
	{
		return;
	}
	
	int i;
	for (i=0; i<5; ++i)
	{
		if (pItem->GetTemplateID()==m_iAssistId[i])
		{
			break;
		}
	}
	if (i==5)
	{
		return;
	}
	
	if (m_iIndexAssist>=0)
	{
		if(GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist))
			GetHostPlayer()->GetPack()->GetItem(m_iIndexAssist)->Freeze(false);
		m_iIndexAssist = -1;
	}
	
	if(!pImg)
		pImg = (PAUIIMAGEPICTURE)GetDlgItem("Item_d");
	GetGameUIMan()->SetIvtrInfo(pImg, pItem->GetTemplateID());
	pItem->Freeze(true);
	pImg->SetDataPtr(pItem, "ptr_CECIvtrItem");
	m_iIndexAssist = nSlot;
}




//////////////////////////////////////////////////////////////////////////
//  符语镶嵌
//////////////////////////////////////////////////////////////////////////
AUI_BEGIN_COMMAND_MAP(CDlgRuneFillSlot, CDlgBase)

AUI_ON_COMMAND("Btn_Confirm",			OnCommandStart)
AUI_ON_COMMAND("IDCANCEL",				OnCommandCancel)

AUI_END_COMMAND_MAP()



AUI_BEGIN_EVENT_MAP(CDlgRuneFillSlot, CDlgBase)

AUI_ON_EVENT("Img_Rune*",	WM_LBUTTONDOWN,	OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgRuneFillSlot::CDlgRuneFillSlot()
{
}

CDlgRuneFillSlot::~CDlgRuneFillSlot()
{
}

bool CDlgRuneFillSlot::OnInitDialog()
{
	AString strName;
	int i=0;
	for (i=0; i<5; ++i)
	{
		strName.Format("Img_Rune%d", i+1);
		m_pImgStone[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Img_RuneShape%d", i+1);
		m_pImgShape[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		m_iIndex[i] = -1;
	}

	return true;
}

void CDlgRuneFillSlot::OnTick() 
{
	CECIvtrRune *pRune = (CECIvtrRune*)GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	if (!pRune)
	{
		Show(false);
		return;
	}
	int i;
	AString strName;
	for (i=0; i<5; ++i)
	{
		m_pImgStone[i]->Show(pRune->GetEssence().hole>i);
		m_pImgShape[i]->Show(pRune->GetEssence().hole>i);
		if(pRune->GetEssence().hole>i)
		{
			if(pRune->GetEssence().rune_stone[i]>0)
				GetGameUIMan()->SetIvtrInfo(m_pImgStone[i], pRune->GetEssence().rune_stone[i]);
			else if (m_iIndex[i]<0)
			{
				GetGameUIMan()->SetIvtrInfo(m_pImgStone[i], 0);
				m_pImgStone[i]->SetHint(GetGameUIMan()->GetStringFromTable(13266));
			}
		}
	}
}

void CDlgRuneFillSlot::OnCommandStart(const char* szCommand)
{
	CECIvtrRune *pRune = (CECIvtrRune*)GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	int i;
	for (i=0; i<5; ++i)
	{
		if (m_iIndex[i]>=0)
		{
			GetGame()->GetGameSession()->c2s_CmdInstallSlot(EQUIPIVTR_RUNE, i, m_iIndex[i],
				GetHostPlayer()->GetPack()->GetItem(m_iIndex[i])->GetTemplateID());
			OnEventLButtonDown(0, 0, m_pImgStone[i]);
		}
	}
}

void CDlgRuneFillSlot::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if (strstr(pObj->GetName(), "Img_RuneShape"))
	{
		return;
	}
	int iIndex = atoi(pObj->GetName()+strlen("Img_Rune"))-1;
	if (m_iIndex[iIndex]>=0)
	{
		CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(m_iIndex[iIndex]);
		if(pItem)
			pItem->Freeze(false);
		m_iIndex[iIndex] = -1;
		GetGameUIMan()->SetIvtrInfo(m_pImgStone[iIndex], 0);
	}
}

void CDlgRuneFillSlot::SetRune()
{
	Show(true);
	CECIvtrItem *pItem = GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	if (!pItem || pItem->GetClassID()!=CECIvtrItem::ICID_RUNE)
	{
		Show(false);
		return;
	}
	
	int i;
	for (i=0; i<5; ++i)
	{
		m_iIndex[i] = -1;
	}
}

void CDlgRuneFillSlot::SetItem(CECIvtrItem *pItem, int nSlot, AUIImagePicture* pImg)
{
	// 必须指定位置
	if (!pImg ||
		strstr(pImg->GetName(), "Img_Rune")==NULL ||
		strlen(pImg->GetName())>strlen("Img_Rune1"))
	{
		return;
	}
	if (pItem->GetClassID()!=CECIvtrItem::ICID_RUNE_SLOT)
	{
		return;
	}
	
	CECIvtrRune *pRune = (CECIvtrRune*)GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	int iIndex = atoi(pImg->GetName()+strlen("Img_Rune"))-1;
	if (pRune->GetEssence().rune_stone[iIndex]>0)	// 已经有符文
	{
		return;
	}

	if (m_iIndex[iIndex]>=0&&GetHostPlayer()->GetPack()->GetItem(m_iIndex[iIndex]))
	{
		GetHostPlayer()->GetPack()->GetItem(m_iIndex[iIndex])->Freeze(false);
	}
	m_iIndex[iIndex] = nSlot;
	pItem->Freeze(true);
	GetGameUIMan()->SetIvtrInfo(pImg, pItem->GetTemplateID());
}

void CDlgRuneFillSlot::OnCommandCancel(const char* szCommand)
{
	OnEventLButtonDown(0, 0, GetDlgItem("Img_Rune1"));
	OnEventLButtonDown(0, 0, GetDlgItem("Img_Rune2"));
	OnEventLButtonDown(0, 0, GetDlgItem("Img_Rune3"));
	OnEventLButtonDown(0, 0, GetDlgItem("Img_Rune4"));
	OnEventLButtonDown(0, 0, GetDlgItem("Img_Rune5"));
	Show(false);
}


//////////////////////////////////////////////////////////////////////////
//  符语擦写
//////////////////////////////////////////////////////////////////////////
AUI_BEGIN_COMMAND_MAP(CDlgRuneErase, CDlgBase)

AUI_ON_COMMAND("Btn_Confirm",			OnCommandStart)
AUI_ON_COMMAND("Btn_Cancel",			OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",				OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgRuneErase, CDlgBase)

AUI_ON_EVENT("Img_Rune*",	WM_LBUTTONDOWN,	OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgRuneErase::CDlgRuneErase()
{
	m_iStoneIndex = -1;
}

CDlgRuneErase::~CDlgRuneErase()
{
}

bool CDlgRuneErase::OnInitDialog()
{
	AString strName;
	int i=0;
	for (i=0; i<5; ++i)
	{
		strName.Format("Img_Rune%d", i+1);
		m_pImgStone[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Img_RuneShape%d", i+1);
		m_pImgShape[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		strName.Format("Img_Check%d", i+1);
		m_pImgGfx[i] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
	}

	return true;
}

void CDlgRuneErase::OnTick()
{
	CECIvtrRune *pRune = (CECIvtrRune*)GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	if (!pRune)
	{
		Show(false);
		return;
	}
	int i;
	for (i=0; i<5; ++i)
	{
		GetGameUIMan()->SetIvtrInfo(m_pImgStone[i], pRune->GetEssence().rune_stone[i]);
		m_pImgStone[i]->Show(i<pRune->GetEssence().hole);
		m_pImgShape[i]->Show(i<pRune->GetEssence().hole);
		m_pImgGfx[i]->Show(m_iStoneIndex==i);
	}
}

void CDlgRuneErase::SetRune()
{
	Show(true);
	CECIvtrItem *pItem = GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	if (!pItem || pItem->GetClassID()!=CECIvtrItem::ICID_RUNE)
	{
		Show(false);
		return;
	}

	m_iStoneIndex = -1;
}

void CDlgRuneErase::OnCommandStart(const char* szCommand)
{
	if (m_iStoneIndex>=0)
	{
		GetGame()->GetGameSession()->c2s_CmdRuneEraseSlot(EQUIPIVTR_RUNE, m_iStoneIndex);
		m_iStoneIndex = -1;
	}
}

void CDlgRuneErase::OnCommandCancel(const char* szCommand)
{
	Show(false);
}

void CDlgRuneErase::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if (strstr(pObj->GetName(), "Img_RuneShape"))
	{
		return;
	}
	CECIvtrRune *pRune = (CECIvtrRune*)GetHostPlayer()->GetEquipment()->GetItem(EQUIPIVTR_RUNE);
	int index = atoi(pObj->GetName()+strlen("Img_Rune"))-1;
	if(pRune->GetEssence().rune_stone[index]>0)
		m_iStoneIndex = index;
}