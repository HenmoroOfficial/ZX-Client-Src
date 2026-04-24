// Filename	:	DlgMarkAstroList.cpp
// Creator	:	Fu Chonggang
// Date		:	May 18, 2010

#include "DlgMarkAstroList.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIImagePicture.h"
#include "EC_IvtrConsume.h"
#include "EC_IvtrTypes.h"
#include "EC_Instance.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_World.h"
#include "EL_Precinct.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgMarkAstroList, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",					OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Transport",				OnCommand_Transport)
AUI_ON_COMMAND("Btn_Add",					OnCommand_Add)
AUI_ON_COMMAND("Btn_Delete",				OnCommand_Delete)
AUI_ON_COMMAND("Btn_Edit",					OnCommand_Edit)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgMarkAstroList, CDlgBase)

AUI_ON_EVENT("Lst_Marklist",		WM_LBUTTONDBLCLK,	OnEventLButtonDBClick_Transport)

AUI_END_EVENT_MAP()

CDlgMarkAstroList::CDlgMarkAstroList()
{
}

CDlgMarkAstroList::~CDlgMarkAstroList()
{
}

bool CDlgMarkAstroList::Release()
{
	return CDlgBase::Release();
}

bool CDlgMarkAstroList::OnInitDialog()
{
	m_pTitle = (AUILabel *)GetDlgItem("Title");
	m_pBtn_Transport = (AUIStillImageButton *)GetDlgItem("Btn_Transport");
	m_pLab_Num = (AUILabel *)GetDlgItem("Lab_Num");
	m_pTxt_Num = (AUILabel *)GetDlgItem("Txt_Num");
	m_pLst_Marklist = (AUIListBox *)GetDlgItem("Lst_Marklist");
	m_pBtn_Add = (AUIStillImageButton *)GetDlgItem("Btn_Add");
	m_pImg_Item = (PAUIIMAGEPICTURE)GetDlgItem("Img_Item");
	m_pImg_Item->Show(false);

	m_pItem = NULL;
	m_bNeedUpdate = false;
	return true;
}

void CDlgMarkAstroList::OnShowDialog()
{
	UpdateInfo();
}

void CDlgMarkAstroList::OnTick()
{
	if (m_bNeedUpdate && !m_pItem->NeedUpdate())
	{
		UpdateInfo();
		m_bNeedUpdate = false;
	}
}

bool CDlgMarkAstroList::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

void CDlgMarkAstroList::UpdateInfo()
{
	ACString strText;
	const IVTR_TELEPORTATION_ESSENCE& ite = m_pItem->GetEssence();
	const TELEPORTATION_ESSENCE*  te = m_pItem->GetDBEssence();
	m_pLst_Marklist->ResetContent();
	for (UINT i=0; i<ite.points.size(); i++)
	{
		ACString strPrcName(_AL(" "));
		if (ite.points[i].inst == GetGameRun()->GetWorld()->GetInstanceID())
		{
			CELPrecinct *pPrecinct = GetWorld()->GetPrecinctSet()->IsPointIn(ite.points[i].x, ite.points[i].z, ite.points[i].inst);
			if (pPrecinct)
			{
				strPrcName = pPrecinct->GetName();
			}
		}
		CECInstance *pInstance = GetGameRun()->GetInstance(ite.points[i].inst);
		if (pInstance)
		{
			strText.Format(_AL("%s\t%s\t%.0f,%.0f"), pInstance->GetName(), strPrcName, ite.points[i].x, ite.points[i].z);
			m_pLst_Marklist->AddString(strText);
		}
	}
	
	strText.Format(_AL("%d/%d"), ite.points.size(), te->max_tele_point);
	m_pTxt_Num->SetText(strText);
	
	char cMemoInfo[18];
	memset(cMemoInfo, 0, 18);
	memcpy(cMemoInfo, ite.memo, 16);
	PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("Edt_Input");
	pEdt->SetText((ACHAR*)ite.memo);
}

int CDlgMarkAstroList::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg == "show_mark")
	{
		m_nIndex = wParam;
		if (m_pItem)
		{
			m_pItem->Freeze(false);
		}
		m_pItem = (CECIvtrTelePortation*)lParam;
		m_pItem->Freeze(true);
		Show(true);
	}
	else if (strMsg == "transport")
	{
		struct use_arg
		{
			int op_type;
			int op_arg;
		};
		use_arg ua;
		ua.op_type = 3;
		ua.op_arg = m_pLst_Marklist->GetCurSel();
		GetGameSession()->c2s_CmdUseItemWithArg(IVTRTYPE_PACK, 1, m_nIndex,
			m_pItem->GetTemplateID(), &ua, sizeof(use_arg));

		m_pItem->SetNeedUpdate(true);
		m_bNeedUpdate = true;
	}
	else if (strMsg == "add")
	{
		struct use_arg
		{
			int op_type;
			int op_arg;
		};
		use_arg ua;
		ua.op_type = 1;
		ua.op_arg = -1;
		GetGameSession()->c2s_CmdUseItemWithArg(IVTRTYPE_PACK, 1, m_nIndex,
			m_pItem->GetTemplateID(), &ua, sizeof(use_arg));
		
		m_pItem->SetNeedUpdate(true);
		m_bNeedUpdate = true;
	}
	else if (strMsg == "delete")
	{
		struct use_arg
		{
			int op_type;
			int op_arg;
		};
		use_arg ua;
		ua.op_type = 2;
		ua.op_arg = m_pLst_Marklist->GetCurSel();
		if (ua.op_arg<0)
		{
			return 1;
		}
		GetGameSession()->c2s_CmdUseItemWithArg(IVTRTYPE_PACK, 1, m_nIndex,
			m_pItem->GetTemplateID(), &ua, sizeof(use_arg));
		
		m_pItem->SetNeedUpdate(true);
		m_bNeedUpdate = true;
	}
	else if (strMsg == "change_string")
	{
		PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("Edt_Input");
		ACString strText = pEdt->GetText();		
		GetGameUIMan()->FilterBadWords(strText);
		if (CheckNameValid(strText.GetBuffer(0)))
		{
			if (strText.GetLength()>8)
				return 0;
			
#pragma pack(1)
			struct use_arg
			{
				int op_type;
				char memo[16];
			};
			use_arg ua;
			ua.op_type = 4;
			memcpy(ua.memo, strText.GetBuffer(0), 2*strText.GetLength());
			GetGameSession()->c2s_CmdUseItemWithArg(IVTRTYPE_PACK, 1, m_nIndex,
				m_pItem->GetTemplateID(), &ua, sizeof(int)+2*strText.GetLength());
			m_pItem->SetNeedUpdate(true);
			m_bNeedUpdate = true;
			
#pragma pack()
		}
	}
	return 1;
}

bool CDlgMarkAstroList::CheckNameValid(ACHAR* szText)
{
	int iLength = a_strlen(szText);
	for (int i=0; i<iLength; i++)
	{
		if (szText[i] == '^' || szText[i] == '\\' || szText[i] == '&')
		{
			GetGameUIMan()->AddChatMessage(GetStringFromTable(9180), GP_CHAT_MISC);
			return false;
		}
	}
	return true;
}

void CDlgMarkAstroList::OnCommand_CANCEL(const char * szCommand)
{
	m_pItem->Freeze(false);
	m_pItem = NULL;
	Show(false);
}

void CDlgMarkAstroList::OnCommand_Transport(const char * szCommand)
{
	if (m_pLst_Marklist->GetCurSel()<0 || m_pLst_Marklist->GetCurSel()>=m_pLst_Marklist->GetCount())
	{
		return;
	}
	ACString strText, strTextInfo;
	strTextInfo = m_pLst_Marklist->GetText(m_pLst_Marklist->GetCurSel());
	for (int i=0; i<strTextInfo.GetLength(); i++)
	{
		if (strTextInfo[i] == _AL('\t'))
		{
			strTextInfo[i] = _AL(' ');
		}
	}
	strText.Format(GetGameUIMan()->GetStringFromTable(9463), strTextInfo);
	PAUIDIALOG pDlg;
	GetGameUIMan()->MessageBox("Game_Mark_Transport", strText, MB_YESNO, A3DCOLORRGBA(255,255,255,160), &pDlg);
	pDlg->SetData(m_nIndex);
	pDlg->SetDataPtr((void*)(m_pLst_Marklist->GetCurSel()));
}

void CDlgMarkAstroList::OnCommand_Add(const char * szCommand)
{
	const IVTR_TELEPORTATION_ESSENCE& ite = m_pItem->GetEssence();
	const TELEPORTATION_ESSENCE*  te = m_pItem->GetDBEssence();

	if ((int)ite.points.size()>=te->max_tele_point)
	{
		GetGameUIMan()->MessageBox(9461);
		return;
	}
	
	ACString strText;
	PAUIDIALOG pDlg;
	int idInstance = GetWorld()->GetInstanceID();
	CECInstance *pInstance = GetGameRun()->GetInstance(idInstance);
	CELPrecinct *pPrecinct = GetWorld()->GetCurPrecinct();
	strText.Format(_AL("%s %s(%.0f,%.0f)"), pInstance->GetName(), pPrecinct ? pPrecinct->GetName() : _AL(" "), GetHostPlayer()->GetPos().x, GetHostPlayer()->GetPos().z);
	GetGameUIMan()->MessageBox("Game_Mark_Add", strText, MB_YESNO, A3DCOLORRGBA(255,255,255,160), &pDlg);
	pDlg->SetData(m_nIndex);
}

void CDlgMarkAstroList::OnCommand_Delete(const char * szCommand)
{
	if (m_pLst_Marklist->GetCurSel()<0)
	{
		return;
	}
	if (m_pLst_Marklist->GetCurSel()<0 || m_pLst_Marklist->GetCurSel()>=m_pLst_Marklist->GetCount())
	{
		return;
	}
	
	ACString strText, strTextInfo;
	PAUIDIALOG pDlg;
	strTextInfo = m_pLst_Marklist->GetText(m_pLst_Marklist->GetCurSel());
	for (int i=0; i<strTextInfo.GetLength(); i++)
	{
		if (strTextInfo[i] == _AL('\t'))
		{
			strTextInfo[i] = _AL(' ');
		}
	}
	strText.Format(GetGameUIMan()->GetStringFromTable(9465), strTextInfo);
	GetGameUIMan()->MessageBox("Game_Mark_Del", strText, MB_YESNO, A3DCOLORRGBA(255,255,255,160), &pDlg);
	pDlg->SetData(m_nIndex);
}

void CDlgMarkAstroList::OnCommand_Edit(const char * szCommand)
{
	GetGameUIMan()->MessageBox("Game_Mark_Change_String", GetGameUIMan()->GetStringFromTable(15024), MB_YESNO, A3DCOLORRGBA(255,255,255,160));
}

void CDlgMarkAstroList::OnEventLButtonDBClick_Transport(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	struct use_arg
	{
		int op_type;
		int op_arg;
	};
	use_arg ua;
	ua.op_type = 3;
	ua.op_arg = m_pLst_Marklist->GetCurSel();
	GetGameSession()->c2s_CmdUseItemWithArg(IVTRTYPE_PACK, 1, m_nIndex,
		m_pItem->GetTemplateID(), &ua, sizeof(use_arg));

	m_pItem->SetNeedUpdate(true);
	m_bNeedUpdate = true;

}