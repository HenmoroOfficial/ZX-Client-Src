#include "AFI.h"
#include "EC_GameUIMan.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "EC_GameSession.h"
#include "EC_IvtrEquip.h"
#include "EC_HostPlayer.h"
#include "EC_LoginPlayer.h"
#include "EC_Model.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_IvtrEquip.h"
#include "EC_ManPlayer.h"

#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"

#include "DlgIndividualityColor.h"
#include "elementdataman.h"
AUI_BEGIN_COMMAND_MAP(DlgIndividualityColor, CDlgBase)

AUI_ON_COMMAND("Btn_Combine", OnCommand_Compose)
AUI_ON_COMMAND("Rdo_*",				OnCommand_Page)
AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(DlgIndividualityColor, CDlgBase)
AUI_ON_EVENT("Item_0*",			WM_RBUTTONUP,		OnEventRButtonDblclk)
AUI_END_EVENT_MAP()
DlgIndividualityColor::DlgIndividualityColor()
{
	m_nPos1 = -1;
	m_nPos2 = -1;
	m_nPos3 = -1;
}
DlgIndividualityColor::~DlgIndividualityColor()
{

}

bool DlgIndividualityColor::OnInitDialog()
{
	m_pPicture1 = (PAUIIMAGEPICTURE)GetDlgItem("Item_01");
	m_pPicture2 = (PAUIIMAGEPICTURE)GetDlgItem("Item_02");
	m_pPicture3 = (PAUIIMAGEPICTURE)GetDlgItem("Item_03");
	m_pPicture4 = (PAUIIMAGEPICTURE)GetDlgItem("Item_00");
	m_pPicture1->SetDataPtr(NULL);
	m_pPicture2->SetDataPtr(NULL);
	m_pPicture3->SetDataPtr(NULL);
	m_pPicture4->SetDataPtr(NULL);
	return true;
}

void DlgIndividualityColor::OnShowDialog()
{
	CheckRadioButton(1,2);
	UpdateFreezeItem();
	CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture4->GetDataPtr();
	if(pData)
	{
		pData->Freeze(false);
		m_pPicture4->SetCover(NULL, -1, 0);
		m_pPicture4->SetHint(_AL(""));
		m_pPicture4->SetData(-1);
		m_pPicture4->SetDataPtr(NULL);
		m_pPicture4->SetText(_AL(""));
	}
}

void DlgIndividualityColor::OnEventRButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if(0 == stricmp(pObj->GetName(),m_pPicture1->GetName()))
	{
		CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture1->GetDataPtr();
		if(pData)
		{
			if(pData->GetCount() == 1)
			{
				pData->Freeze(false);
			}
			m_pPicture1->SetCover(NULL, -1, 0);
			m_pPicture1->SetHint(_AL(""));
			m_pPicture1->SetData(-1);
			m_pPicture1->SetDataPtr(NULL);
			m_pPicture1->SetText(_AL(""));
			m_nPos1 = -1;	
			if(m_nPos3 == -1)
			{
				if(m_nPos1 != m_nPos2 )
				{
					pData->Freeze(false);
				}
			}
			if(m_nPos2 == -1)
			{
				if(m_nPos1 != m_nPos3 )
				{
					pData->Freeze(false);
				}	
			}
			if(m_nPos2 != -1 && m_nPos3 != -1)
			{
				if(m_nPos1 != m_nPos2 && m_nPos1 != m_nPos3)
				{
					pData->Freeze(false);
				}
			}
		}
	}
	else if(0 == stricmp(pObj->GetName(),m_pPicture2->GetName()))
	{
		CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture2->GetDataPtr();
		if(pData)
		{
			if(pData->GetCount() == 1)
			{
				pData->Freeze(false);
			}
			m_pPicture2->SetCover(NULL, -1, 0);
			m_pPicture2->SetHint(_AL(""));
			m_pPicture2->SetData(-1);
			m_pPicture2->SetDataPtr(NULL);
			m_pPicture2->SetText(_AL(""));
			m_nPos2 = -1;
			if(m_nPos3 == -1)
			{
				if(m_nPos1 != m_nPos2 )
				{
					pData->Freeze(false);
				}
			}
			if(m_nPos1 == -1)
			{
				if(m_nPos2 != m_nPos3 )
				{
					pData->Freeze(false);
				}	
			}
		
			if(m_nPos2 != -1 && m_nPos3 != -1)
			{
				if(m_nPos1 != m_nPos2 && m_nPos1 != m_nPos3)
				{
					pData->Freeze(false);
				}
			}
		}
	}
	else if(0 == stricmp(pObj->GetName(),m_pPicture3->GetName()))
	{
		CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture3->GetDataPtr();
		if(pData)
		{
			if(pData->GetCount() == 1)
			{
				pData->Freeze(false);
			}
			m_pPicture3->SetCover(NULL, -1, 0);
			m_pPicture3->SetHint(_AL(""));
			m_pPicture3->SetData(-1);
			m_pPicture3->SetDataPtr(NULL);
			m_pPicture3->SetText(_AL(""));
			m_nPos3 = -1;
			if(m_nPos1 == -1)
			{
				if(m_nPos3 != m_nPos2 )
				{
					pData->Freeze(false);
				}
			}
			if(m_nPos2 == -1)
			{
				if(m_nPos3 != m_nPos1 )
				{
					pData->Freeze(false);
				}	
			}
			
			if(m_nPos1 != -1 && m_nPos2 != -1)
			{
				if(m_nPos1 != m_nPos2 && m_nPos1 != m_nPos3)
				{
					pData->Freeze(false);
				}
			}
	
		}
	}
	else if(0 == stricmp(pObj->GetName(),m_pPicture4->GetName()))
	{
		CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture4->GetDataPtr();
		if(pData)
		{
			pData->Freeze(false);
			m_pPicture4->SetCover(NULL, -1, 0);
			m_pPicture4->SetHint(_AL(""));
			m_pPicture4->SetData(-1);
			m_pPicture4->SetDataPtr(NULL);
			m_pPicture4->SetText(_AL(""));
		}
	}
}

void DlgIndividualityColor::OnHideDialog()
{
	UpdateFreezeItem();
}

void DlgIndividualityColor::OnCommand_Compose(const char * szCommand)
{
	int nQuality1,nQuality2,	nQuality3;
	nQuality1 = -1;
	nQuality2 = -1;
	nQuality3 = -1;

	if(m_pPicture1->GetDataPtr() != NULL)
	{
		CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture1->GetDataPtr();
		if(pData)
		{
			nQuality1 = pData->GetDBEssence()->quality;
		}
	}
	if(m_pPicture2->GetDataPtr() != NULL)
	{
		CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture2->GetDataPtr();
		if(pData)
		{
			nQuality2 = pData->GetDBEssence()->quality;
		}
	}
	if(m_pPicture3->GetDataPtr() != NULL)
	{
		CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture3->GetDataPtr();
		if(pData)
		{
			nQuality3 = pData->GetDBEssence()->quality;
		}
	}
	if(nQuality1 != nQuality2 && nQuality1 != -1 && nQuality2 != -1)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(20003),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	else if(nQuality1 != nQuality3&& nQuality1 != -1 && nQuality3 != -1)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(20003),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}	
	else if(nQuality2 != nQuality3&& nQuality3 != -1 && nQuality2 != -1)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(20003),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}	
	if((nQuality1 != -1 && nQuality2 != -1 && nQuality3 == -1)
		|| (nQuality1 == -1 && nQuality2 != -1 && nQuality3 != -1)
		|| (nQuality1 != -1 && nQuality2 == -1 && nQuality3 != -1))
	{
		if((nQuality1 > 0  &&  nQuality2 > 0)&& (nQuality1 == nQuality2))
		{
			GetGameUIMan()->MessageBox("",GetStringFromTable(20005),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
		else if((nQuality1 > 0 && nQuality3 > 0) && (nQuality1 == nQuality3))
		{
			GetGameUIMan()->MessageBox("",GetStringFromTable(20005),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
		else if((nQuality2 > 0 && nQuality3 > 0) && (nQuality2 == nQuality3))
		{
			GetGameUIMan()->MessageBox("",GetStringFromTable(20005),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
	}
	CECIvtrItem*pItem1 = GetHostPlayer()->GetPack()->GetItem(m_nPos1);
	CECIvtrItem*pItem2 = GetHostPlayer()->GetPack()->GetItem(m_nPos2);
	CECIvtrItem*pItem3 = GetHostPlayer()->GetPack()->GetItem(m_nPos3);
	if(m_nPos1 != -1 && pItem1 && pItem1->GetTemplateID() != m_nItemID1)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(20004),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if(m_nPos2 != -1 && pItem2 && pItem2->GetTemplateID() != m_nItemID2)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(20004),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if(m_nPos3 != -1 && pItem3 && pItem3->GetTemplateID() != m_nItemID3)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(20004),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	GetGameSession()->c2s_SendFashionColorantCombine(m_nPos1,m_nPos2,m_nPos3);
}

void DlgIndividualityColor::UpDateResult(int nPos)
{
	CECIvtrItem*pItem = GetHostPlayer()->GetPack()->GetItem(nPos);
	if(pItem)
	{
		AString strFile;
		AUICTranslate trans;

		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)pItem;
		m_pPicture4->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
			GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
		m_pPicture4->SetCover(NULL, 0, 1);
		m_pPicture4->SetHint(trans.Translate(pData->GetDesc()));
		m_pPicture4->SetDataPtr(pData);

		UpdateFreezeItem();
	}
}

void DlgIndividualityColor::SetIndividualityItem(CECIvtrItem* pIvtrSrc,int nPos)
{
	CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture4->GetDataPtr();
	if(pData)
	{
		pData->Freeze(false);
		m_pPicture4->SetCover(NULL, -1, 0);
		m_pPicture4->SetHint(_AL(""));
		m_pPicture4->SetData(-1);
		m_pPicture4->SetDataPtr(NULL);
		m_pPicture4->SetText(_AL(""));
	}
	if(m_nPos1 != -1 && m_nPos2 != -1 && m_nPos3 != -1)
	{
		return;
	}
	AString strFile;
	AUICTranslate trans;
	if (pIvtrSrc)
	{
		if(m_pPicture1->GetDataPtr() == NULL)
		{
			UpDateItemImg(m_pPicture1,pIvtrSrc);
			m_nPos1 = nPos;
			m_nItemID1 = pIvtrSrc->GetTemplateID();
		}
		else if(m_pPicture2->GetDataPtr() == NULL)
		{
			UpDateItemImg(m_pPicture2,pIvtrSrc);
			m_nPos2 = nPos;
			m_nItemID2 = pIvtrSrc->GetTemplateID();
		}
		else if(m_pPicture3->GetDataPtr() == NULL)
		{
			UpDateItemImg(m_pPicture3,pIvtrSrc);
			m_nPos3 = nPos;
			m_nItemID3 = pIvtrSrc->GetTemplateID();
		}
	}
	if(m_nPos1 == m_nPos2)
	{
		if(pIvtrSrc->GetCount() <= 2)
		{
			pIvtrSrc->Freeze(true);
		}
	}
	if(m_nPos1 == m_nPos3)
	{
		if(pIvtrSrc->GetCount() <= 2)
		{
			pIvtrSrc->Freeze(true);
		}
	}
	if(m_nPos1 == m_nPos2  && m_nPos1 == m_nPos3)
	{
		if(pIvtrSrc->GetCount() <= 3)
		{
			pIvtrSrc->Freeze(true);
		}
	}
}

void DlgIndividualityColor::UpDateItemImg(PAUIIMAGEPICTURE pDlg, CECIvtrItem* pIvtrSrc ) 
{
	pIvtrSrc->SetNeedUpdate(false);
	//pIvtrSrc->Freeze(true);
	AString strFile;
	AUICTranslate trans;
	if	(pIvtrSrc->GetCount() == 1)
	{
		pIvtrSrc->Freeze(true);
	}
	af_GetFileTitle(pIvtrSrc->GetIconFile(), strFile);
	strFile.MakeLower();
	CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)pIvtrSrc;
	pDlg->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
		GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
	pDlg->SetCover(NULL, 0, 1);
	pDlg->SetHint(trans.Translate(pData->GetDesc()));
	pDlg->SetDataPtr(pData);
}

void DlgIndividualityColor::SetNameToPosItem(const char* szName,CECIvtrItem* pIvtrSrc,int nPos)
{
	CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture4->GetDataPtr();
	if(pData)
	{
		pData->Freeze(false);
		m_pPicture4->SetCover(NULL, -1, 0);
		m_pPicture4->SetHint(_AL(""));
		m_pPicture4->SetData(-1);
		m_pPicture4->SetDataPtr(NULL);
		m_pPicture4->SetText(_AL(""));
	}
	if(pIvtrSrc->GetClassID() != CECIvtrItem::ICID_INDIVIDUALITY_ITEM)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(20000),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if(0 == stricmp(m_pPicture1->GetName(), szName))
	{
		if(m_pPicture1->GetDataPtr() != NULL)
		{
			CECIvtrIndividualityItem* pData = (	CECIvtrIndividualityItem*)m_pPicture1->GetDataPtr();
			if(pData)
			{
				pData->Freeze(false);
			}
		}
		UpDateItemImg(m_pPicture1,pIvtrSrc);
		 m_nPos1 = nPos;
		 m_nItemID1 = pIvtrSrc->GetTemplateID();
		 if(m_nPos1 == m_nPos2)
		 {
			 if(pIvtrSrc->GetCount() <= 2)
			 {
				 pIvtrSrc->Freeze(true);
			 }
		 }
		 if(m_nPos1 == m_nPos3)
		 {
			 if(pIvtrSrc->GetCount() <= 2)
			 {
				 pIvtrSrc->Freeze(true);
			 }
		 }
		 if(m_nPos1 == m_nPos2  && m_nPos1 == m_nPos3)
		 {
			 if(pIvtrSrc->GetCount() <= 3)
			 {
				 pIvtrSrc->Freeze(true);
			 }
		 }
	}
	if(0 == stricmp(m_pPicture2->GetName(), szName))
	{
		if(m_pPicture2->GetDataPtr() != NULL)
		{
			CECIvtrIndividualityItem* pData = (	CECIvtrIndividualityItem*)m_pPicture2->GetDataPtr();
			if(pData)
			{
				pData->Freeze(false);
			}
		}
		UpDateItemImg(m_pPicture2,pIvtrSrc);
		 m_nPos2 = nPos;
		 m_nItemID2 = pIvtrSrc->GetTemplateID();
		 if(m_nPos1 == m_nPos2)
		 {
			 if(pIvtrSrc->GetCount() <= 2)
			 {
				 pIvtrSrc->Freeze(true);
			 }
		 }
		 if(m_nPos2 == m_nPos3)
		 {
			 if(pIvtrSrc->GetCount() <= 2)
			 {
				 pIvtrSrc->Freeze(true);
			 }
		 }
		 if(m_nPos1 == m_nPos2  && m_nPos1 == m_nPos3)
		 {
			 if(pIvtrSrc->GetCount() <= 3)
			 {
				 pIvtrSrc->Freeze(true);
			 }
		 }
	}
	if(0 == stricmp(m_pPicture3->GetName(), szName))
	{
		if(m_pPicture3->GetDataPtr() != NULL)
		{
			CECIvtrIndividualityItem* pData = (	CECIvtrIndividualityItem*)m_pPicture3->GetDataPtr();
			if(pData)
			{
				pData->Freeze(false);
			}
		}
		UpDateItemImg(m_pPicture3,pIvtrSrc);
		 m_nPos3 = nPos;
		 m_nItemID3 = pIvtrSrc->GetTemplateID();
		 if(m_nPos1 == m_nPos2)
		 {
			 if(pIvtrSrc->GetCount() <= 2)
			 {
				 pIvtrSrc->Freeze(true);
			 }
		 }
		 if(m_nPos1 == m_nPos3)
		 {
			 if(pIvtrSrc->GetCount() <= 2)
			 {
				 pIvtrSrc->Freeze(true);
			 }
		 }
		 if(m_nPos1 == m_nPos2  && m_nPos1 == m_nPos3)
		 {
			 if(pIvtrSrc->GetCount() <= 3)
			 {
				 pIvtrSrc->Freeze(true);
			 }
		 }
	}
}


void  DlgIndividualityColor::UpdateFreezeItem() 
{
	m_nPos1 = -1;
	m_nPos2 = -1;
	m_nPos3 = -1;
	CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture1->GetDataPtr();
	if(pData)
	{
		pData->Freeze(false);
		m_pPicture1->SetCover(NULL, -1, 0);
		m_pPicture1->SetHint(_AL(""));
		m_pPicture1->SetData(-1);
		m_pPicture1->SetDataPtr(NULL);
		m_pPicture1->SetText(_AL(""));
	}
	pData = (CECIvtrIndividualityItem*)m_pPicture2->GetDataPtr();
	if(pData)
	{
		pData->Freeze(false);
		m_pPicture2->SetCover(NULL, -1, 0);
		m_pPicture2->SetHint(_AL(""));
		m_pPicture2->SetData(-1);
		m_pPicture2->SetDataPtr(NULL);
		m_pPicture2->SetText(_AL(""));
	}
	pData = (CECIvtrIndividualityItem*)m_pPicture3->GetDataPtr();
	if(pData)
	{
		pData->Freeze(false);
		m_pPicture3->SetCover(NULL, -1, 0);
		m_pPicture3->SetHint(_AL(""));
		m_pPicture3->SetData(-1);
		m_pPicture3->SetDataPtr(NULL);
		m_pPicture3->SetText(_AL(""));
	}
}


void DlgIndividualityColor::OnCommand_Page(const char * szCommand)
{
	Show(false);
	PAUIDIALOG pDlg = NULL;
	int nType = atoi(szCommand+5);
	switch(nType)
	{
	case 1:
		pDlg = GetGameUIMan()->GetDialog("Win_Dye_FashDress");
		break;
	case 2:
		pDlg = GetGameUIMan()->GetDialog("Win_Dye_Combine");
		break;
	}

	if( pDlg )
	{
		pDlg->Show(true);
		pDlg->SetPosEx(GetPos().x, GetPos().y);
	}
}

void DlgIndividualityColor::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}



AUI_BEGIN_COMMAND_MAP(DlgIndividualityReveal, CDlgBase)

AUI_ON_COMMAND("Btn_Confirm", OnCommand_OK)
AUI_ON_COMMAND("Btn_Close", OnCommand_Close)
AUI_ON_COMMAND("Btn_Max_Ceccamera", OnCommand_Max_Ceccamera)
AUI_ON_COMMAND("Btn_Min_Ceccamera", OnCommand_Min_Ceccamera)
AUI_ON_COMMAND("Rdo_*",				OnCommand_Page)
AUI_ON_COMMAND("IDCANCEL",				OnCommand_Close)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(DlgIndividualityReveal, CDlgBase)
AUI_ON_EVENT("Item_0*",			WM_RBUTTONUP,		OnEventRButtonDblclk)
AUI_END_EVENT_MAP()

DlgIndividualityReveal::DlgIndividualityReveal()
{
	m_pPlayer = NULL;
	memset(m_aEquips, 0, sizeof(m_aEquips));
	m_nPackType = 0;
	m_nPackFashionPos = 0;
	m_nPackPos = 0;
}
DlgIndividualityReveal::~DlgIndividualityReveal()
{
	A3DRELEASE(m_pPlayer);
}

void DlgIndividualityReveal::OnShowDialog()
{
	m_nAngle = 0;
	CheckRadioButton(1,1);
	if (m_pPlayer)
	{
		CECHostPlayer* pHost = GetHostPlayer();
		pHost->SyncDemonstrationPlayer(m_pPlayer, 1);
		memset(m_aEquips, 0, sizeof(m_aEquips));
		m_pPlayer->ShowEquipments(m_aEquips, false, true);
		m_pPlayer->SetFashionMode(true);
	}
	ACString szTemp;
	szTemp.Format(_AL("0/3"));
	GetDlgItem("Txt_Num")->SetText(szTemp);
}


bool DlgIndividualityReveal::OnInitDialog()
{
	m_pBtn_LeftTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Left");
	m_pBtn_RightTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Right");

	m_pPicture1 = (PAUIIMAGEPICTURE)GetDlgItem("Item_01");
	m_pPicture2 = (PAUIIMAGEPICTURE)GetDlgItem("Item_00");

	m_pImg_Char = (PAUIIMAGEPICTURE)GetDlgItem("Img_Show");

	assert(!m_pPlayer);
	ReLoadModel();
	GetDlgItem("Btn_Min_Ceccamera")->Show(false);
	GetDlgItem("Btn_Max_Ceccamera")->Show(false);
	return true;
}

void DlgIndividualityReveal::OnHideDialog()
{
	UpdateFreezeItem();
}

void DlgIndividualityReveal::OnTick()
{
	if( m_pBtn_LeftTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
		m_nAngle = (m_nAngle + 360 - GetGame()->GetTickTime() / 6 ) % 360;
	else if( m_pBtn_RightTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
		m_nAngle = (m_nAngle + GetGame()->GetTickTime() / 6) % 360;

	if (m_pPlayer)
	{
		m_pPlayer->Stand();
		m_pPlayer->Tick(GetGame()->GetTickTime());


		m_pImg_Char->SetRenderCallback(
			PlayerRenderDemonstration,
			(DWORD)m_pPlayer,
			m_nAngle);
	}
}

void DlgIndividualityReveal::ReLoadModel()
{
	A3DRELEASE(m_pPlayer);
	m_pPlayer = new CECLoginPlayer(GetGameRun()->GetWorld()->GetPlayerMan());
	const GNET::RoleInfo& info = GetGameRun()->GetSelectedRoleInfo();

	memset(m_aEquips, 0, sizeof(m_aEquips));
	if (!m_pPlayer->Load(CECPlayer::GetPhysiqueByProf(info.occupation), info.occupation, info.faceid, info.hairid, info.gender, NULL))
	{
		m_pPlayer->Release();
		delete m_pPlayer;
		m_pPlayer = NULL;
	}
	else
	{
		m_pPlayer->SetPos(A3DVECTOR3(0));
		m_pPlayer->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
		m_pPlayer->GetPlayerModel()->SetAutoUpdateFlag(false);
		m_pPlayer->ShowEquipments(m_aEquips, true, true);	
	}
	//CECHostPlayer* pHost = GetHostPlayer();
}


void DlgIndividualityReveal::OnCommand_Page(const char * szCommand)
{
	Show(false);
	PAUIDIALOG pDlg = NULL;
	int nType = atoi(szCommand+5);
	switch(nType)
	{
	case 1:
		pDlg = GetGameUIMan()->GetDialog("Win_Dye_FashDress");
		break;
	case 2:
		pDlg = GetGameUIMan()->GetDialog("Win_Dye_Combine");
		break;
	}

	if( pDlg )
	{
		pDlg->Show(true);
		pDlg->SetPosEx(GetPos().x, GetPos().y);
	}
}

void DlgIndividualityReveal::OnCommand_OK(const char * szCommand)
{	
	CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture1->GetDataPtr();
	if(pData)
	{
		int nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(pData->GetTemplateID());
		if(nNum < 3)
		{
			GetGameUIMan()->MessageBox("",GetStringFromTable(20002),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
	}
	GetGameSession()->c2s_SendChangeFashionColor(m_nPackType,m_nPackFashionPos,m_nPackPos);
}

void DlgIndividualityReveal::OnCommand_Close(const char * szCommand)
{
	Show(false);
}

void DlgIndividualityReveal::UpdateFreezeItem()
{
	if(m_pPlayer)
	{
		CECHostPlayer* pHost = GetHostPlayer();
		memset(m_aEquips, 0, sizeof(m_aEquips));
		m_pPlayer->ShowEquipments(m_aEquips, true, true);
	}
	m_nPackType = -1;
	m_nPackFashionPos = -1;
	m_nPackPos = -1;
	CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture1->GetDataPtr();
	if(pData)
	{
		pData->Freeze(false);
		m_pPicture1->SetCover(NULL, -1, 0);
		m_pPicture1->SetHint(_AL(""));
		m_pPicture1->SetData(-1);
		m_pPicture1->SetDataPtr(NULL);
		m_pPicture1->SetText(_AL(""));
		ACString szTemp;
		szTemp.Format(_AL("0/3"));
		GetDlgItem("Txt_Num")->SetText(szTemp);
	}
	CECIvtrEquip* pItem = (CECIvtrEquip*)m_pPicture2->GetDataPtr();
	if(pItem)
	{
		pItem->Freeze(false);
		m_pPicture2->SetCover(NULL, -1, 0);
		m_pPicture2->SetHint(_AL(""));
		m_pPicture2->SetData(-1);
		m_pPicture2->SetDataPtr(NULL);
		m_pPicture2->SetText(_AL(""));
	}
}

void DlgIndividualityReveal::SetNameToPosItem(const char* szName,CECIvtrItem* pIvtrSrc,int nPos,int nPackType,int nPackPos)
{
	AString strFile;
	AUICTranslate trans;
	if (pIvtrSrc)
	{
		if(0 == stricmp(m_pPicture1->GetName(), szName))
		{
			if(pIvtrSrc->GetClassID() != CECIvtrItem::ICID_INDIVIDUALITY_ITEM)
			{
				GetGameUIMan()->MessageBox("",GetStringFromTable(20000),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
				return;
			}
			if(m_pPicture1->GetDataPtr() != NULL)
			{
				CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture1->GetDataPtr();
				pData->Freeze(false);
			}
			CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)pIvtrSrc;
			pIvtrSrc->SetNeedUpdate(false);
			pIvtrSrc->Freeze(true);
			af_GetFileTitle(pIvtrSrc->GetIconFile(), strFile);
			strFile.MakeLower();
			m_pPicture1->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
				GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
			m_pPicture1->SetCover(NULL, 0, 1);
			m_pPicture1->SetHint(trans.Translate(pData->GetDesc()));
			m_pPicture1->SetDataPtr(pData);
			m_nPackPos = nPos;
			int nNum = GetHostPlayer()->GetPack()->GetItemTotalNum(pIvtrSrc->GetTemplateID());
			ACString szTemp;
			szTemp.Format(_AL("%d/3"),nNum);
			GetDlgItem("Txt_Num")->SetText(szTemp);
		}
		if(0 == stricmp(m_pPicture2->GetName(), szName))
		{
			if(pIvtrSrc->GetClassID() != CECIvtrItem::ICID_EQUIP)
			{
				GetGameUIMan()->MessageBox("",GetStringFromTable(20001),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
				return;
			}
			if(m_pPicture2->GetDataPtr() != NULL)
			{
				CECIvtrEquip* pData = (CECIvtrEquip*)m_pPicture2->GetDataPtr();
				pData->Freeze(false);
			}
			CECIvtrEquip* pData = (CECIvtrEquip*)pIvtrSrc;
			pIvtrSrc->SetNeedUpdate(false);
			pIvtrSrc->Freeze(true);
			af_GetFileTitle(pIvtrSrc->GetIconFile(), strFile);
			strFile.MakeLower();
			m_pPicture2->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
				GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
			m_pPicture2->SetCover(NULL, 0, 1);
			m_pPicture2->SetHint(trans.Translate(pData->GetDesc()));
			m_pPicture2->SetDataPtr(pData);

			m_nPackType = nPackType;
			m_nPackFashionPos = nPackPos;
	
		}
	}

	UpDateEquipColor();
}

void DlgIndividualityReveal::OnEventRButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if(0 == stricmp(pObj->GetName(),m_pPicture1->GetName()))
	{
		CECIvtrIndividualityItem* pData = (CECIvtrIndividualityItem*)m_pPicture1->GetDataPtr();
		if(pData)
		{
			pData->Freeze(false);
			m_pPicture1->SetCover(NULL, -1, 0);
			m_pPicture1->SetHint(_AL(""));
			m_pPicture1->SetData(-1);
			m_pPicture1->SetDataPtr(NULL);
			m_pPicture1->SetText(_AL(""));
			ACString szTemp;
			szTemp.Format(_AL("0/3"));
			GetDlgItem("Txt_Num")->SetText(szTemp);
		}
	}
	else if(0 == stricmp(pObj->GetName(),m_pPicture2->GetName()))
	{
		CECIvtrEquip* pData = (CECIvtrEquip*)m_pPicture2->GetDataPtr();
		if(pData)
		{
			pData->Freeze(false);
			m_pPicture2->SetCover(NULL, -1, 0);
			m_pPicture2->SetHint(_AL(""));
			m_pPicture2->SetData(-1);
			m_pPicture2->SetDataPtr(NULL);
			m_pPicture2->SetText(_AL(""));
		}
	}
	UpDateEquipColor();
}

void  DlgIndividualityReveal::OnCommand_Min_Ceccamera(const char * szCommand)
{
	//m_bSliding = true;
	//m_fDistanceDest -= gs.fCamZoomSpeed;
	//if( m_fDistanceDest < CECCAMERA_MIN_DISTANCE )
	//	m_fDistanceDest = CECCAMERA_MIN_DISTANCE;
}
void  DlgIndividualityReveal::OnCommand_Max_Ceccamera(const char * szCommand)
{
	//m_bSliding = true;
	//m_fDistanceDest += gs.fCamZoomSpeed;
	//if( m_fDistanceDest > CECCAMERA_MAX_DISTANCE )
	//	m_fDistanceDest = CECCAMERA_MAX_DISTANCE;
}

void  DlgIndividualityReveal::UpDateEquipColor() 
{
	int nIndexColor = -1;
	CECIvtrIndividualityItem* pData = NULL;
	if(m_pPicture1->GetDataPtr() != NULL)
	{
		pData = (CECIvtrIndividualityItem*)m_pPicture1->GetDataPtr();
	}
	if(m_pPicture2->GetDataPtr() != NULL)
	{
		CECIvtrEquip* pItem = (CECIvtrEquip*)m_pPicture2->GetDataPtr();
		if(pItem)
		{
			memset(m_aEquips, 0, sizeof(m_aEquips));
			//WORD num_estone =1;// pItem->GetFashionColor();
			int i, iFirstCan=-1;
			for (i=0; i < SIZE_EQUIPIVTR; i++)
			{
				if (pItem->CanEquippedTo(i))
				{
					if (iFirstCan < 0)
						iFirstCan = i;
				}
			}
			if (iFirstCan >= 0)
			{	
				int nIndex = 0;
				if(pData)
				{
					nIndex = pData->GetDBEssence()->index;
				}
				if(nIndex >= 69)
				{
					nIndex = 69;
				}
				if(nIndex == 0)
				{
					CECHostPlayer* pHost = GetHostPlayer();
					memset(m_aEquips, 0, sizeof(m_aEquips));
					m_pPlayer->ShowEquipments(m_aEquips, true, true);
					m_aEquips[iFirstCan] = pItem->GetTemplateID() ;
					int nItemIdex = pItem->GetNewEssenceForGem().reserved1;
					if(nItemIdex)
					{
						CECIvtrIndividualityItem* pItem = 	(CECIvtrIndividualityItem*)CECIvtrItem::CreateItem(nItemIdex, 0, 1);
						if(pItem)
						{
							WORD nColorId = pItem->GetDBEssence()->index;
							m_aEquips[iFirstCan] |= (nColorId << 16) & 0x7fffffff;
							delete pItem;
						}
					}
				}
				else
				{
					m_aEquips[iFirstCan] = pItem->GetTemplateID() ;
					m_aEquips[iFirstCan] |= ( nIndex<< 16) & 0x7fffffff;
				}	
			}
			m_pPlayer->ShowEquipments(m_aEquips, true, true);
		}
	}
	else
	{
		CECHostPlayer* pHost = GetHostPlayer();
		memset(m_aEquips, 0, sizeof(m_aEquips));
		m_pPlayer->ShowEquipments(m_aEquips, true, true);
	}

}
