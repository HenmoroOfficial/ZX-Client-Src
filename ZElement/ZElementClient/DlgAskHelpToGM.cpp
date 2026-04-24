/********************************************************************
	created:	2005/08/11
	created:	11:8:2005   15:31
	file base:	DlgAskHelpToGM
	file ext:	cpp
	author:		yaojun
	
	purpose:	
*********************************************************************/

#include "DlgAskHelpToGM.h"
#include "aui\\AUIEditBox.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_NetDef.h"
#include "PredefinedGMHelpMsg.h"
#include "aui\\AUIEditBox.h"
#include "aui\\AUIComboBox.h"

#define new A_DEBUG_NEW


AUI_BEGIN_COMMAND_MAP(CDlgAskHelpToGM, CDlgBase)

AUI_ON_COMMAND("confirm", OnCommandConfirm)

AUI_END_COMMAND_MAP()

CDlgAskHelpToGM::CDlgAskHelpToGM() :
	m_pCbxType(NULL),
	m_pEbxMemo(NULL),
	m_dwSelectedType(-1)
{

}

CDlgAskHelpToGM::~CDlgAskHelpToGM()
{

}

void CDlgAskHelpToGM::DoDataExchange(bool bSave)
{
	CDlgBase::DoDataExchange(bSave);

	DDX_Control("cbxType", m_pCbxType);
//	DDX_Control("ebxMemo", m_pEbxMemo);

	DDX_ComboBoxItemData(bSave, "cbxType", m_dwSelectedType);
	DDX_EditBox(bSave, "ebxMemo", m_strMemo);
}

bool CDlgAskHelpToGM::OnInitDialog()
{
	if (!CDlgBase::OnInitDialog())
		return false;

//	m_pEbxMemo->SetIsAutoReturn(true);

	const CGMHelpMsgArray & ary = *TheGMHelpMsgArray::Instance();
	CGMHelpMsgArray::const_iterator iter = ary.begin();
	for (; ary.end() != iter; ++iter)
	{
		int nIndex = m_pCbxType->AddString(GetStringFromTable(iter->msg)) - 1;
		m_pCbxType->SetItemData(nIndex, static_cast<DWORD>(iter->type));
	}
	GetDlgItem("cbxType")->Enable(false);
	GetDlgItem("ebxMemo")->Enable(false);

	return true;
}


void CDlgAskHelpToGM::OnShowDialog()
{
	CDlgBase::OnShowDialog();

	m_dwSelectedType = GM_HELP_MSG_TYPE_BASE + 1;
	m_strMemo.Empty();
	UpdateData(false);
}

void CDlgAskHelpToGM::OnCommandConfirm(const char *szCommand)
{
	UpdateData(true);


	ACHAR type = static_cast<ACHAR>(m_dwSelectedType);
	GMHelpMsg msg;
	if (!TheGMHelpMsgArray::Instance()->FindByType(type, &msg))
	{
		ASSERT(!"invalid type for predefined gm help message");
		return;
	}
	ACString strMsg;
	if (m_strMemo.IsEmpty())
		strMsg = ACString(GM_HELP_MSG_TYPE_BASE, 1) + ACString(type, 1) +
			GetStringFromTable(msg.msg);
	else
		strMsg = ACString(GM_HELP_MSG_TYPE_BASE, 1) + ACString(type, 1) +
			GetStringFromTable(msg.msg) + _AL(",") + m_strMemo;

	strMsg += _AL(",");
	strMsg += GetGameUIMan()->GetDialog("Win_MiniMap")->GetDlgItem("Img_ServerLine")->GetHint();
	// send to gm
	GetGameSession()->SubmitAccuse((int)GetDataPtr("rold_id"));
	GetGameSession()->SubmitProblemToGM(strMsg);
	GetGameSession()->SendPrivateChatData(_AL("GM"), strMsg);	

	// display in local window
	GetGameUIMan()->AddChatMessage(
		GetGameUIMan()->GetStringFromTable(569), GP_CHAT_MISC);
	
	SetData(GetTickCount(), "tick_time");
	
	Show(false);
}


void CDlgAskHelpToGM::ShowWithHelpMessage(const ACHAR * szMsg, bool bReportPlugin)
{
	Show(true);

	m_strMemo = szMsg;
	if (bReportPlugin)
		m_dwSelectedType = GM_HELP_MSG_TYPE_BASE + 2; // note : modify this value when type list changed
	else
		m_dwSelectedType = GM_HELP_MSG_TYPE_BASE;
	UpdateData(false);

}
