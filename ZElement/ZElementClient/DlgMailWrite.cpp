// Filename	: DlgMailWrite.cpp
// Creator	: Xiao Zhou
// Date		: 2005/10/31

#include "AFI.h"
#include "DlgMailWrite.h"
#include "DlgInputNO.h"
#include "DlgFriendList.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "playersendmail_re.hpp"


AUI_BEGIN_COMMAND_MAP(CDlgMailWrite, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)
AUI_ON_COMMAND("Btn_MailFriendlist",OnCommandFriend)
AUI_ON_COMMAND("send",				OnCommandSend)
AUI_ON_COMMAND("choosemoney",		OnCommandChoosemoney)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgMailWrite, CDlgBase)

AUI_ON_EVENT("Img_AttachItem",	WM_LBUTTONDOWN,	OnEvent_LButtonDown_AttachItem)
AUI_ON_EVENT("Img_Money",		WM_LBUTTONDOWN,	OnEvent_LButtonDown_Money)

AUI_END_EVENT_MAP()

CDlgMailWrite::CDlgMailWrite()
{
	m_idItem = 0;
	m_nItemNumber = 0;
	m_nItemPos = 0;
}

CDlgMailWrite::~CDlgMailWrite()
{
}

bool CDlgMailWrite::OnInitDialog()
{
	m_pTxt_MailTo = (PAUIEDITBOX)GetDlgItem("Txt_MailTo");
	m_pTxt_Subject = (PAUIEDITBOX)GetDlgItem("Txt_Subject");
	m_pTxt_Content = (PAUIEDITBOX)GetDlgItem("Txt_Content");
	m_pImg_AttachItem = (PAUIIMAGEPICTURE)GetDlgItem("Img_AttachItem");
	m_pBtn_Send = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Send");
	m_pLab_Money1 = (PAUILABEL)GetDlgItem("Lab_Money1");
	m_pLab_Money2 = (PAUILABEL)GetDlgItem("Lab_Money2");
	m_pLab_Money3 = (PAUILABEL)GetDlgItem("Lab_Money3");
	m_pLab_Money1->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pLab_Money2->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pLab_Money3->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
	m_pTxt_Money2 = (PAUILABEL)GetDlgItem("Txt_Money2");
	m_pTxt_Money3 = (PAUILABEL)GetDlgItem("Txt_Money3");
	m_pTxt_Money1->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money2->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money3->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Content->SetIsAutoReturn(true);

	return true;
}

void CDlgMailWrite::OnShowDialog()
{
	m_pTxt_Money1->SetData(0);
}

void CDlgMailWrite::OnCommandCancel(const char* szCommand)
{
	OnEvent_LButtonDown_AttachItem(0, 0, NULL);
	
	Show(false);
}

void CDlgMailWrite::OnCommandFriend(const char *szCommand)
{
	int battleType = GetHostPlayer()->GetBattleType();
	if ( battleType!=BATTLE_TYPE_ARENA && battleType!=BATTLE_TYPE_ARENAREBORN && battleType!=BATTLE_TYPE_CRSSVR )
	{
		CDlgFriendList* pDlg = GetGameUIMan()->m_pDlgFriendList;
		if( pDlg )
		{
			pDlg->Show(!pDlg->IsShow());
			pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
				AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
		}
	}
}

void CDlgMailWrite::OnCommandSend(const char* szCommand)
{
	int idPlayer= GetGameRun()->GetPlayerID(m_pTxt_MailTo->GetText());
	SendMail(idPlayer);
}

void CDlgMailWrite::OnCommandChoosemoney(const char * szCommand)
{
	CECHostPlayer *pHost = GetHostPlayer();
	CDlgInputNO* pShow = GetGameUIMan()->m_pDlgInputMoney;
	pShow->SetData((int)GetDataPtr("int"));
	pShow->Show(true, true);
	pShow->SetType(CDlgInputNO::INPUTNO_ATTACH_MONEY);
	PAUIOBJECT pEdit = pShow->GetDlgItem("DEFAULT_Txt_Money1");

	pEdit->SetData(GetHostPlayer()->GetMoneyAmount());
}

void CDlgMailWrite::OnEvent_LButtonDown_Money(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	OnCommandChoosemoney("");
}

void CDlgMailWrite::OnEvent_LButtonDown_AttachItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( m_idItem == 0 )
		return;

	CECInventory *pPack = GetHostPlayer()->GetPack();
	CECIvtrItem *pItem = pPack->GetItem(m_nItemPos);
	if( pItem )
		pItem->Freeze(false);
	m_idItem = 0;
	m_nItemNumber = 0;
	m_nItemPos = 0;
	m_pImg_AttachItem->SetCover(NULL, -1);
	m_pImg_AttachItem->SetText(_AL(""));
}

void CDlgMailWrite::OnTick()
{
	if( m_idItem )
	{
		CECInventory *pPack = GetHostPlayer()->GetPack();
		CECIvtrItem *pItem = pPack->GetItem(m_nItemPos);
		if( !pItem )
		{
			m_idItem = 0;
			m_nItemNumber = 0;
			m_nItemPos = 0;
			m_pImg_AttachItem->SetCover(NULL, -1);
			m_pImg_AttachItem->SetText(_AL(""));
		}
	}
	m_pBtn_Send->Enable(m_pTxt_MailTo->GetText()[0] != '\0' && 
						m_pTxt_Subject->GetText()[0] != '\0' &&
						m_pTxt_Content->GetText()[0] != '\0' );
	int nMoney = m_pTxt_Money1->GetData();
	ACHAR szMoney[20];
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
	m_pTxt_Money3->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pTxt_Money2->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pTxt_Money1->SetText(szMoney);
	nMoney = GetHostPlayer()->GetMoneyAmount();
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
	m_pLab_Money3->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pLab_Money2->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pLab_Money1->SetText(szMoney);
}

void CDlgMailWrite::CreateNewMail(const ACHAR *strMailto, ACString szSubject)
{
	m_pTxt_Content->SetText(_AL(""));
	m_pImg_AttachItem->SetCover(NULL, -1);
	m_pImg_AttachItem->SetText(_AL(""));
	OnEvent_LButtonDown_AttachItem(0, 0, NULL);

	if( strMailto == NULL )
	{
		m_pTxt_MailTo->SetText(_AL(""));
		m_pTxt_Subject->SetText(_AL(""));
		ChangeFocus(m_pTxt_MailTo);
	}
	else
	{
		m_pTxt_MailTo->SetText(strMailto);
		m_pTxt_Subject->SetText(szSubject);
		ChangeFocus(m_pTxt_Content);
	}

	Show(true);
}

void CDlgMailWrite::SetAttachItem(CECIvtrItem *pItem, int nItemNumber, int nItemPos)
{
	pItem->Freeze(true);
	ACHAR szText[20];
	a_sprintf(szText, _AL("%d"), nItemNumber);
	m_pImg_AttachItem->SetText(szText);
	AString strFile;
	af_GetFileTitle(pItem->GetIconFile(), strFile);
	strFile.MakeLower();
	m_pImg_AttachItem->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
		GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
	m_idItem = pItem->GetTemplateID();
	m_nItemNumber = nItemNumber;
	m_nItemPos = nItemPos;
}

void CDlgMailWrite::SendMail(int idPlayer)
{
	if( idPlayer == -1 )
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(643), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if( !IsShow() )
		return;

	if( m_pTxt_MailTo->GetText()[0] != '\0' && 
		m_pTxt_Subject->GetText()[0] != '\0' &&
		m_pTxt_Content->GetText()[0] != '\0' )
	{
		if( idPlayer == GetHostPlayer()->GetCharacterID() )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(641), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}

		int nAttachGold = m_pTxt_Money1->GetData();
		int nPackGold = GetHostPlayer()->GetMoneyAmount();
		if( nAttachGold >= 0 && nPackGold < 500 )
		{
			PAUIDIALOG pMsgBox;
			m_pAUIManager->MessageBox("", GetStringFromTable(635), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			pMsgBox->SetLife(3);
			return;
		}

		if( nAttachGold < 0 || nAttachGold + 500 > nPackGold )
		{
			PAUIDIALOG pMsgBox;
			m_pAUIManager->MessageBox("", GetStringFromTable(636), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			pMsgBox->SetLife(3);
			return;
		}

		if( idPlayer != 0 )
			GetGameSession()->mail_Send(
				idPlayer ,
				m_pTxt_Subject->GetText(), m_pTxt_Content->GetText(), 
				m_idItem, m_nItemNumber, m_nItemPos, nAttachGold);
		else
			GetGameSession()->GetPlayerIDByName(m_pTxt_MailTo->GetText(), 3);
	}
}

void CDlgMailWrite::SendMailRe(void *pData)
{
	PlayerSendMail_Re *pPreserveMail = (PlayerSendMail_Re *)pData;
	if( pPreserveMail->retcode == ERR_SUCCESS)
	{
		Show(false);
		int nAttachGold = m_pTxt_Money1->GetData();
		if( m_idItem == 0 )
			GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(637), GP_CHAT_MISC);
		else
			GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(649), GP_CHAT_MISC);
		m_idItem = 0;
		m_nItemNumber = 0;
		m_nItemPos = 0;
		m_pImg_AttachItem->SetCover(NULL, -1);
		m_pImg_AttachItem->SetText(_AL(""));
	}
	else if( pPreserveMail->retcode == ERR_MS_BOXFULL)
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(650), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160));
	else if( pPreserveMail->retcode == ERR_MS_ACCOUNTFROZEN)
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(651), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160));
	else if( pPreserveMail->retcode == ERR_MS_ATTACH_INV)
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(640), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160));
	else if( pPreserveMail->retcode == ERR_MS_SEND_SELF)
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(641), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160));
	else
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(638), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160));
}
