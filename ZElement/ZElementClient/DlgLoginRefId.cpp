// File		: DlgLoginRefId.h
// Creator	: Fu Chonggang
// Date		: 2009/04/27

#include "DlgLoginRefId.h"
#include "EC_LoginUIMan.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_Configs.h"
#include "AUI\\AUIEditBox.h"
#include "A3DGFXEx.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgLoginCommon, AUIDialog)
AUI_ON_COMMAND("confirm",	OnCommandConfirm)
AUI_ON_COMMAND("IDCANCEL",	OnCommandCancel)
AUI_END_COMMAND_MAP()

CDlgLoginCommon::CDlgLoginCommon()
{
}

CDlgLoginCommon::~CDlgLoginCommon()
{
}

void CDlgLoginCommon::OnShowDialog()
{
	if (stricmp(m_szName,"Win_PopID") == 0)
	{
		if (m_strId.GetLength()>0)
		{
			m_pEdtId->SetText(m_strId);
		}
	}
	else if (stricmp(m_szName,"Win_Login") == 0)
	{
		PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Account");
		ACString strName = pEdt->GetText();
		if (strName.GetLength()>0)
		{
			ChangeFocus(GetDlgItem("Txt_PassWord"));
		}
	}
}

bool CDlgLoginCommon::OnInitDialog()
{
	if (stricmp(m_szName,"Win_PopID") == 0)
		m_pEdtId = (PAUIEDITBOX)GetDlgItem("Edt_ID");
	else if (stricmp(m_szName,"Win_Login") == 0)
	{

	}
	return true;
}

void CDlgLoginCommon::OnCommandConfirm(const char * szCommand)
{
	m_strId = m_pEdtId->GetText();
}

void CDlgLoginCommon::OnCommandCancel(const char * szCommand)
{
	if (stricmp(m_szName, "Win_Login") == 0)
	{
		PostQuitMessage(0);
	}
	else
		Show(false);
}
