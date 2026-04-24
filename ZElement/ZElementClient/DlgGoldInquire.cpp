// File		: DlgGoldInquire.cpp
// Creator	: Xiao Zhou
// Date		: 2006/9/20

#include "DlgGoldInquire.h"
#include "DlgGoldTrade.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgGoldInquire, CDlgBase)

AUI_ON_COMMAND("giveup",		OnCommandGiveUp)
AUI_ON_COMMAND("trade",			OnCommandTrade)
AUI_ON_COMMAND("unlock",		OnCommandUnlock)
AUI_ON_COMMAND("help",			OnCommandHelp)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

CDlgGoldInquire::CDlgGoldInquire()
{
}

CDlgGoldInquire::~CDlgGoldInquire()
{
}

void CDlgGoldInquire::OnCommandHelp(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_GoldTrade_Help");
	//pDlg->Show(!pDlg->IsShow());
	AString strDlg = "Win_GoldTrade_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgGoldInquire::OnCommandGiveUp(const char * szCommand)
{
	int nSel = m_pLst_Inquire->GetCurSel();
	if( nSel >= 0 && nSel < m_pLst_Inquire->GetCount() && m_pLst_Inquire->GetItemData(nSel) != 0 
		&& m_pBtn_GiveUp->IsShow() )
	{
		ACString strText;
		strText.Format(GetStringFromTable(843), m_pLst_Inquire->GetItemData(nSel));
		PAUIDIALOG pMsgBox;
		GetGameUIMan()->MessageBox("Game_GoldGiveUp", strText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(m_pLst_Inquire->GetItemData(nSel));
		pMsgBox->SetDataPtr(m_pLst_Inquire->GetItemDataPtr(nSel));
	}
}

void CDlgGoldInquire::OnCommandTrade(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->m_pDlgGoldTrade->Show(true);
	GetGameUIMan()->m_pDlgGoldTrade->SetPosEx(GetPos().x, GetPos().y);
}

void CDlgGoldInquire::OnCommandUnlock(const char * szCommand)
{
	GetGameUIMan()->GetDialog("Win_GoldPwdInput")->Show(true);
}

void CDlgGoldInquire::OnCommandCancel(const char * szCommand)
{
	GetGameSession()->cash_Lock(1, _AL(""));
	GetGameUIMan()->EndNPCService();
	Show(false);
}

bool CDlgGoldInquire::OnInitDialog()
{
	m_pLst_Sell = (PAUILISTBOX)GetDlgItem("Lst_Sell");
	m_pLst_Buy = (PAUILISTBOX)GetDlgItem("Lst_Buy");
	m_pLst_Inquire = (PAUILISTBOX)GetDlgItem("Lst_Inquire");
	m_pBtn_GiveUp = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_GiveUp");
	
	return true;
}

void CDlgGoldInquire::OnShowDialog()
{
	GetGameSession()->stock_bill();
	m_pLst_Inquire->ResetContent();
}

void CDlgGoldInquire::OnTick()
{
	int nSel = m_pLst_Inquire->GetCurSel();
	m_pBtn_GiveUp->Enable( nSel >= 0 && nSel < m_pLst_Inquire->GetCount() &&
		m_pLst_Inquire->GetItemData(nSel) != 0 );
}