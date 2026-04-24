// Filename	: DlgReName.cpp
// Creator	: Fu Chonggang
// Date		: 2012.7.27

#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUIImagePicture.h"
#include "DlgReName.h"
#include "EC_GameUIMan.h"
#include "EC_GameRun.h"
#include "Exptypes.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrEquip.h"
#include "queryrolenamehistory.hpp"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgReName, CDlgBase)

AUI_ON_COMMAND("start",	OnCommandConfirm)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgReName, CDlgBase)

AUI_ON_EVENT("Img_Item",			WM_LBUTTONUP,		OnEventLButtonDown)

AUI_END_EVENT_MAP()


CDlgReName::CDlgReName() : m_iIndex(-1)
{
}

CDlgReName::~CDlgReName()
{
}

void CDlgReName::OnCommandConfirm(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("Edit_ChangeName");
	if (pEdt->GetText()==0)
	{
		return;
	}
	if(m_szName=="Win_Char_ChangeName")
		GetGameSession()->c2s_CmdNPCSevChangeName(m_iIndex, GetHostPlayer()->GetPack()->GetItem(m_iIndex)->GetTemplateID(), pEdt->GetText());
	else if(m_szName=="Win_Guild_ChangeName")
	{
		char cFamily = (char)GetData("is_family");
		GetGameSession()->c2s_CmdNPCSevChangeFamilyGuildName(m_iIndex, GetHostPlayer()->GetPack()->GetItem(m_iIndex)->GetTemplateID(), pEdt->GetText(), cFamily);
	}
	OnEventLButtonDown(0, 0, GetDlgItem("Img_Item"));
}

void CDlgReName::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (m_iIndex>=0)
	{
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
		pImg->ClearCover();
		pImg->SetHint(_AL(""));
		if (GetHostPlayer()->GetPack()->GetItem(m_iIndex))
			GetHostPlayer()->GetPack()->GetItem(m_iIndex)->Freeze(false);
		m_iIndex = -1;
	}
	GetDlgItem("Btn_Confirm")->Enable(false);
}

void CDlgReName::OnShowDialog()
{
	OnEventLButtonDown(0, 0, GetDlgItem("Img_Item"));
	if (m_szName=="Win_Guild_ChangeName")
	{
		char cFamily = (char)GetData("is_family");
		GetDlgItem("Title")->Show(cFamily==0);
		GetDlgItem("Title2")->Show(cFamily>0);
		
		GetDlgItem("Btn_Qshop_Rename_Clan")->Show(cFamily>0);
		GetDlgItem("Btn_Qshop_Rename_Junto")->Show(cFamily==0);
	}
}

void CDlgReName::OnHideDialog()
{
	OnEventLButtonDown(0, 0, GetDlgItem("Img_Item"));
	GetGameUIMan()->EndNPCService();
}

void CDlgReName::SetItem(int iIndex, CECIvtrItem* pItem)
{
	int i;
	for (i=0; i<3; ++i)
	{
		int testId = 0;
		if(m_szName=="Win_Char_ChangeName")
			testId = GetGameRun()->GetSpecialIDConfig().id_change_name[i];
		else if(m_szName=="Win_Guild_ChangeName")
		{
			char cFamily = (char)GetData("is_family");
			if(cFamily==0)
				testId = GetGameRun()->GetSpecialIDConfig().id_change_name_guild[i];
			else
				testId = GetGameRun()->GetSpecialIDConfig().id_change_name_family[i];
		}

		if (pItem->GetTemplateID()==testId)
		{
			if (m_iIndex>=0)
			{
				GetHostPlayer()->GetPack()->GetItem(m_iIndex)->Freeze(false);
			}
			m_iIndex = iIndex;
			pItem->Freeze(true);
			PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
			GetGameUIMan()->SetIvtrInfo(pImg, pItem);
			GetDlgItem("Btn_Confirm")->Enable(true);
			break;
		}
	}
}



AUI_BEGIN_COMMAND_MAP(CDlgNameHistory, CDlgBase)

AUI_ON_COMMAND("Btn_Search",	OnCommandConfirm)
AUI_ON_COMMAND("DEFAULT_Edt_1",	OnCommandConfirm)
AUI_ON_COMMAND("Rdo_Char*",		OnCommandRdoChar)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgNameHistory, CDlgBase)

AUI_ON_EVENT("*",				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,				WM_MOUSEWHEEL,		OnEventMouseWheel)

AUI_END_EVENT_MAP()


const int CDlgNameHistory::ms_iMaxNameDisp = 8;

bool CDlgNameHistory::OnInitDialog()
{
	m_pSclName = (PAUISCROLL)GetDlgItem("Scl_List");
	m_iFirstLine = 0;
	m_pSclName->Show(false);
	UpdateDisplay();

	return true;
}

void CDlgNameHistory::OnTick()
{
	if (m_iFirstLine!=m_pSclName->GetBarLevel())
	{
		int index = m_iFirstLine + GetCheckedRadioButton(1)-1;
		m_iFirstLine = m_pSclName->GetBarLevel();
		if (index-m_iFirstLine>=0)
			CheckRadioButton(1, index-m_iFirstLine+1);
		else
			CheckRadioButton(1, 1);
		UpdateDisplay();
		OnCommandRdoChar("");
	}
}

void CDlgNameHistory::OnCommandConfirm(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Edt_1");
	if (pEdt->GetText()==0)
	{
		return;
	}
	
	QueryRolenameHistory p;
	p.name.replace(pEdt->GetText(), sizeof(ACHAR)*a_strlen(pEdt->GetText()));
	
	GetGameSession()->SendNetData(p);
}

void CDlgNameHistory::OnCommandRdoChar(const char * szCommand)
{
	int index = GetCheckedRadioButton(1)-1;
	GNET::NameHistory curHist = m_vecNameHistory[m_iFirstLine+index];
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Prof");
	pLab->SetText(GetGameUIMan()->GetStringFromTable(PROFESSION_START+curHist.occupation));
	pLab = (PAUILABEL)GetDlgItem("Txt_Lv");
	ACString strText;
	strText.Format(_AL("%d"), curHist.level);
	if (curHist.reborn_cnt>0)
	{
		strText += GetGameUIMan()->GetStringFromTable(7012);
	}
	pLab->SetText(strText);
	pLab = (PAUILABEL)GetDlgItem("Txt_Gender");
	pLab->SetText(GetGameUIMan()->GetStringFromTable(curHist.gender>0 ? 7142 : 7141));
	pLab = (PAUILABEL)GetDlgItem("Txt_Name");
	ACString strName((const ACHAR*)curHist.namelist[0].begin(), curHist.namelist[0].size()/sizeof(ACHAR));
	pLab->SetText(strName);
	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_NameList");
	unsigned int i;
	pLst->ResetContent();
	for (i=0; i<curHist.namelist.size(); ++i)
	{
		ACString strName((const ACHAR*)curHist.namelist[i].begin(), curHist.namelist[i].size()/sizeof(ACHAR));
		pLst->AddString(strName);
		if (strName==m_strNameSearch)
			pLst->SetItemTextColor(i, A3DCOLORRGBA(0, 255, 255, 160));
		else
			pLst->SetItemTextColor(i, A3DCOLORRGBA(255, 255, 255, 160));
	}
}

void CDlgNameHistory::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	int zDelta = (short)HIWORD(wParam);
	if( zDelta > 0 )
		zDelta = 1;
	else
		zDelta = -1;
	if( m_pSclName->IsShow() )
		m_pSclName->SetBarLevel(m_pSclName->GetBarLevel() - zDelta);
}

int CDlgNameHistory::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="hist_info")
	{
		GNET::QueryRolenameHistory_Re *p = (GNET::QueryRolenameHistory_Re*)wParam;

		m_strNameSearch = ACString((const ACHAR*)p->name.begin(), p->name.size()/sizeof(ACHAR));
		m_vecNameHistory.clear();
		unsigned int i;
		for (i=0; i<p->list.size(); ++i)
		{
			m_vecNameHistory.push_back(p->list[i]);
		}

		if (m_vecNameHistory.size()>ms_iMaxNameDisp)
		{
			m_pSclName->Show(true);
			m_pSclName->SetScrollRange(0, m_vecNameHistory.size()-ms_iMaxNameDisp);
			m_pSclName->SetScrollStep(1);
			m_pSclName->SetBarLevel(0);
			m_iFirstLine = 0;
		}
		else
			m_pSclName->Show(false);
		UpdateDisplay();
		if (m_vecNameHistory.size()>0)
		{
			CheckRadioButton(1, 1);
			OnCommandRdoChar("");
		}
	}

	return 0;
}

void CDlgNameHistory::UpdateDisplay()
{
	AString strName;
	int i;
	for (i=0; i<ms_iMaxNameDisp; ++i)
	{
		strName.Format("Txt_Name%d", i+1);
		PAUILABEL pLab = (PAUILABEL)GetDlgItem(strName);
		strName.Format("Rdo_Char%d", i+1);
		PAUIOBJECT pObj = GetDlgItem(strName);
		if ((m_iFirstLine+i)<(int)m_vecNameHistory.size())
		{
			ACString strText((const ACHAR*)m_vecNameHistory[m_iFirstLine+i].namelist[0].begin(), m_vecNameHistory[m_iFirstLine+i].namelist[0].size()/sizeof(ACHAR));
			pLab->SetText(strText);
			pLab->Show(true);
			pObj->Show(true);
		}
		else
		{
			pLab->Show(false);
			pObj->Show(false);
		}
	}
	
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Prof");
	pLab->SetText(_AL(""));
	pLab = (PAUILABEL)GetDlgItem("Txt_Lv");
	pLab->SetText(_AL(""));
	pLab = (PAUILABEL)GetDlgItem("Txt_Gender");
	pLab->SetText(_AL(""));
	pLab = (PAUILABEL)GetDlgItem("Txt_Name");
	pLab->SetText(_AL(""));
	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_NameList");
	pLst->ResetContent();

}


AUI_BEGIN_COMMAND_MAP(CDlgEquipReName, CDlgBase)
AUI_ON_COMMAND("Btn_Confirm",	OnCommandConfirm)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgEquipReName, CDlgBase)
AUI_ON_EVENT("Img_Equip",		WM_LBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Img_Equip",		WM_RBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Img_Item",		WM_LBUTTONDOWN,			OnEventLButtonDown)
AUI_ON_EVENT("Img_Item",		WM_RBUTTONDOWN,			OnEventLButtonDown)
AUI_END_EVENT_MAP()




bool CDlgEquipReName::OnInitDialog()
{
	m_pImgItem1 = (AUIImagePicture *)GetDlgItem("Img_Equip");
	m_pImgItem2 = (AUIImagePicture *)GetDlgItem("Img_Item");
	m_pImgItem1->SetDataPtr(NULL);
	m_pImgItem1->SetData(-1);
	m_pImgItem2->SetDataPtr(NULL);
	m_pImgItem2->SetData(-1);
   return true;
}
void CDlgEquipReName::OnShowDialog()
{
	if (m_pImgItem1->GetDataPtr())
	{
		((CECIvtrItem*)m_pImgItem1->GetDataPtr())->Freeze(true);
	}
	if (m_pImgItem2->GetDataPtr())
	{
		((CECIvtrItem*)m_pImgItem2->GetDataPtr())->Freeze(true);
	}
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("Edit_ChangeName");
	pEdt->SetText(_AL(""));
}
int CDlgEquipReName::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg == "set_equip")
	{
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam);
		if(pItem && pItem->GetClassID() == CECIvtrItem::ICID_EQUIP)
		{
		    CECIvtrEquip* pEquip = (CECIvtrEquip*)pItem;
			if(pEquip->GetLevelRequirement() >= 151 && 
			  pEquip->GetNewEssenceForAstrology().old_data.num_estone >= 12 && 
			  (pEquip->GetStateFlag() & ITEM_STATE_BINDED))
			{
                OnEventLButtonDown(0, 0, m_pImgItem1);
				GetGameUIMan()->SetIvtrInfo(m_pImgItem1, pItem);
				m_pImgItem1->SetData(wParam);
				if (m_pImgItem1->GetDataPtr())
				{
					((CECIvtrItem*)m_pImgItem1->GetDataPtr())->Freeze(false);
				}
				m_pImgItem1->SetDataPtr(pItem);
				pItem->Freeze(true);
				return 0;
			}

		}

		return 1;
	}
	else if(strMsg == "set_props")
	{

		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam);

        if(pItem && (pItem->GetTemplateID() == GetGameRun()->GetSpecialIDConfig().id_rename_equip_Props1 || 
			         pItem->GetTemplateID() == GetGameRun()->GetSpecialIDConfig().id_rename_equip_Props2)
		  )
		{
			OnEventLButtonDown(0, 0, m_pImgItem2);
			GetGameUIMan()->SetIvtrInfo(m_pImgItem2, pItem);
			m_pImgItem2->SetData(wParam);
			if (m_pImgItem2->GetDataPtr())
			{
				((CECIvtrItem*)m_pImgItem2->GetDataPtr())->Freeze(false);
			}
			m_pImgItem2->SetDataPtr(pItem);
			pItem->Freeze(true);
			return 0;
		  
		}
		return 1;
	}
    return 0;
}
void CDlgEquipReName::OnTick()
{
    GetDlgItem("Btn_Confirm")->Enable(m_pImgItem1->GetDataPtr()&&m_pImgItem2->GetDataPtr());
}
void CDlgEquipReName::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pObj;
	if (pObj->GetDataPtr())
	{
		((CECIvtrItem*)pObj->GetDataPtr())->Freeze(false);
	}
	pImg->SetCover(NULL, -1, 0);
	pImg->SetHint(_AL(""));
	pImg->SetData(-1);
	pImg->SetDataPtr(NULL);
}
void CDlgEquipReName::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(false);
	GetGameUIMan()->EndNPCService();
	OnEventLButtonDown(0, 0, m_pImgItem1);
	OnEventLButtonDown(0, 0, m_pImgItem2);
	GetGameUIMan()->UnfreezeAllItems();
}
void CDlgEquipReName::OnCommandConfirm(const char * szCommand)
{
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("Edit_ChangeName");
	if (pEdt->GetText()==0)
	{
		return;
	}
	ACString strName = pEdt->GetText();
	if (CheckNameValid(strName.GetBuffer(0)))
	{
		GetGameUIMan()->FilterBadWords(strName);
		//提示框
		if(m_pImgItem1->GetDataPtr()&&m_pImgItem2->GetDataPtr())
		{
		   ACString strMsg;
		   strMsg.Format(GetGameUIMan()->GetStringFromTable(18201),
			             ((CECIvtrItem*)m_pImgItem2->GetDataPtr())->GetName(),
						 ((CECIvtrItem*)m_pImgItem1->GetDataPtr())->GetName(),
						 strName);
		   GetGameUIMan()->MessageBox("EquipReName",strMsg,MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
		}
	}
	else
	{
	   GetGameUIMan()->MessageBox(18200);
	}
}
void CDlgEquipReName::ProcessEquipReName(bool isAgree)
{
   if(isAgree)
   {
      PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("Edit_ChangeName");
	  ACString strName = pEdt->GetText();
	  GetGameUIMan()->FilterBadWords(strName);
	  //发送改名请求
	  //......
	  OnEventLButtonDown(0, 0, m_pImgItem1);
	  OnEventLButtonDown(0, 0, m_pImgItem2);
	  pEdt->SetText(_AL(""));

   }
}
bool CDlgEquipReName::CheckNameValid(ACHAR* szText)
{
	int iLength = a_strlen(szText);
	if(iLength <2 || iLength >8)
	{
	   return false;
	}
	for (int i=0; i<iLength; i++)
	{
		if (szText[i] == '^' || szText[i] == '\\' || szText[i] == '&')
		{
			return false;
		}
	}
	return true;
}
