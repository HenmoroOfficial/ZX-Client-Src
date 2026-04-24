// Filename	: DlgMailRead.cpp
// Creator	: Xiao Zhou
// Date		: 2005/10/31

#include "AFI.h"
#include "DlgMailRead.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_GameRun.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_NetDef.h"
#include "AUI\\AUICTranslate.h"
#include "DlgMailWrite.h"
#include "EC_GameUIMisc.h"
#include "getmail_re.hpp"
#include "getmailattachobj_re.hpp"
#include "gconsignmailinfo"

AUI_BEGIN_COMMAND_MAP(CDlgMailRead, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",	OnCommandCancel)
AUI_ON_COMMAND("getattach",	OnCommandGetAttach)
AUI_ON_COMMAND("reply",		OnCommandReply)

AUI_END_COMMAND_MAP()

CDlgMailRead::CDlgMailRead()
{
}

CDlgMailRead::~CDlgMailRead()
{
}

bool CDlgMailRead::OnInitDialog()
{
	m_pTxt_MailFrom = (PAUILABEL)GetDlgItem("Txt_MailFrom");
	m_pTxt_Subject = (PAUILABEL)GetDlgItem("Txt_Subject");
	m_pTxt_NotSystemMail = (PAUILABEL)GetDlgItem("Txt_NotSystemMail");
	m_pTxt_Content = (PAUITEXTAREA)GetDlgItem("Txt_Content");
	m_pImg_AttachItem = (PAUIIMAGEPICTURE)GetDlgItem("Img_AttachItem");
	m_pBtn_GetAttach = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_GetAttach");
	m_pBtn_Reply = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Reply");
	m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
	m_pTxt_Money2 = (PAUILABEL)GetDlgItem("Txt_Money2");
	m_pTxt_Money3 = (PAUILABEL)GetDlgItem("Txt_Money3");
	m_pTxt_Money1->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money2->SetAlign(AUIFRAME_ALIGN_RIGHT);
	m_pTxt_Money3->SetAlign(AUIFRAME_ALIGN_RIGHT);

	return true;
}

void CDlgMailRead::OnCommandCancel(const char* szCommand)
{
	Show(false);
}

void CDlgMailRead::OnCommandReply(const char* szCommand)
{
	ACString strSubject;
	strSubject.Format(_AL("Re: %s"), m_pTxt_Subject->GetText() );
	if( strSubject.GetLength() > 20 )
		strSubject = strSubject.Left(20);
	GetGameUIMan()->m_pDlgMailWrite->CreateNewMail(m_pTxt_MailFrom->GetText(), strSubject );
	Show(false);
}

void CDlgMailRead::OnCommandGetAttach(const char* szCommand)
{
	m_pBtn_GetAttach->Enable(false);
	GetGameSession()->mail_GetAttachment(m_idMail, _MA_ATTACH_OBJ | _MA_ATTACH_MONEY);
}

void CDlgMailRead::ReadMail(void *pData)
{
	GetMail_Re *pMail = (GetMail_Re *)pData;
	if( pMail->retcode == ERR_MS_MAIL_INV )
	{
		GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(639), GP_CHAT_MISC);
		return;
	}
	else if( pMail->retcode != ERR_SUCCESS)
	{
		GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(642), GP_CHAT_MISC);
		return;
	}

	if( !IsShow() )
		Show(true);
	else
		GetGameUIMan()->BringWindowToTop(this);

	GNET::GMail mail = pMail->mail;
	GNET::GMailHeader mailHeader = mail.header; 
	GNET::GRoleInventory attachItem = mail.attach_obj;
	m_idMail = mailHeader.id;
	m_idSender = mailHeader.sender;

	ACString strSubject;
	ACString strMailFrom;
	ACString strContent;
	unsigned int aid, item, count;
	m_pBtn_Reply->Enable(mailHeader.sndr_type == _MST_PLAYER);
	switch(mailHeader.sndr_type)
	{
	case _MST_AUCTION:
	{
		ACString strCount;
		strMailFrom = GetStringFromTable(5001);
		Marshal::OctetsStream(mailHeader.title) >> aid >> item >> count;		
		if( count > 1 )
			strCount.Format(_AL("(%d)"), count);
		else
			strCount = _AL("");
		CECIvtrItem *pItem = CECIvtrItem::CreateItem(item, 0, count);
		switch(mailHeader.sender)
		{
		case _AUCTION_ITEM_SOLD:
			strSubject = GetStringFromTable(5101);
			strContent.Format(GetStringFromTable(5201),
				aid, pItem->GetName(), strCount);
			break;
		case _AUCTION_BID_WIN:
			strSubject = GetStringFromTable(5102);
			strContent.Format(GetStringFromTable(5202),
				aid, pItem->GetName(), strCount);
			break;
		case _AUCTION_BID_LOSE:
			strSubject = GetStringFromTable(5103);
			strContent.Format(GetStringFromTable(5203),
				aid, pItem->GetName(), strCount);
			break;
		case _AUCTION_BID_CANCEL:
			strSubject = GetStringFromTable(5104);
			strContent.Format(GetStringFromTable(5204),
				aid, pItem->GetName(), strCount);
			break;
		case _AUCTION_BID_TIMEOUT:
			strSubject = GetStringFromTable(5105);
			strContent.Format(GetStringFromTable(5205),
				aid, pItem->GetName(), strCount);
			break;
		}
		delete pItem;
		break;
	}
	case _MST_LOCAL_CONSIGN:
		{
		strMailFrom = GetStringFromTable(5003);
		GConsignMailInfo cmi;
		Marshal::OctetsStream(mailHeader.title) >> cmi;
		CECIvtrItem *pItem = CECIvtrItem::CreateItem(cmi.item_id, 0, cmi.item_cnt);
		ACString strName((const ACHAR*)cmi.sell_to_rolename.begin(), cmi.sell_to_rolename.size() / sizeof(ACHAR));
		int money = cmi.money/10000;
		int margin = cmi.margin/10000;
		switch(mailHeader.sender)
		{
		case _CONSIGN_BUY:
			strSubject = GetStringFromTable(10260);
			if (money==0)
				strContent.Format(GetStringFromTable(10261), pItem->GetName());
			else
				strContent.Format(GetStringFromTable(10262), money);
			break;
		case _CONSIGN_SOLD:
			strSubject = GetStringFromTable(10263);
			if (money==0)
				strContent.Format(GetStringFromTable(10264), pItem->GetName(), strName, margin);
			else
				strContent.Format(GetStringFromTable(10265), money, strName, margin);
			break;
		case _CONSIGN_CANCEL:
			strSubject = GetStringFromTable(10266);
			if (money==0)
				strContent.Format(GetStringFromTable(10267), pItem->GetName(), margin);
			else
				strContent.Format(GetStringFromTable(10268), money, margin);
			break;
		case _CONSIGN_FORCED_CANCEL:
			strSubject = GetStringFromTable(10269);
			if (money==0)
				strContent.Format(GetStringFromTable(10270), pItem->GetName(), margin);
			else
				strContent.Format(GetStringFromTable(10271), money, margin);
			break;
		case _CONSIGN_EXPIRE:
			strSubject = GetStringFromTable(10272);
			if (money==0)
				strContent.Format(GetStringFromTable(10273), pItem->GetName(), margin);
			else
				strContent.Format(GetStringFromTable(10274), money, margin);
			break;
		case _CONSIGN_FAIL:
			strSubject = GetStringFromTable(10275);
			if (money==0)
				strContent.Format(GetStringFromTable(10276), pItem->GetName(), margin);
			else
				strContent.Format(GetStringFromTable(10277), money, margin);
			break;
		}
		break;
		}
	case _MST_ROLE_CONSIGN_NOTIFY:
		{
			strMailFrom = GetStringFromTable(5003);
			strSubject = GetStringFromTable(5000);
			ACString strName = ACString((const ACHAR*)mailHeader.sender_name.begin(), mailHeader.sender_name.size() / sizeof (ACHAR));
			strContent.Format(GetStringFromTable(4999), strName);
			break;
		}
	case _MST_FACTION_PK_BONUS_NOTIFY:
		{
			strMailFrom = GetStringFromTable(5004);
			strSubject = GetStringFromTable(5110); 
			strContent = GetStringFromTable(5210);
			break;
		}
	case _MST_KINGDOM_AWARD:
		{
			strMailFrom = GetStringFromTable(5005);
			strSubject  = GetStringFromTable(5111); 
			strContent  = GetStringFromTable(5211);
		    break;
		}
	default:
		strMailFrom = ACString((const ACHAR*)mailHeader.sender_name.begin(), mailHeader.sender_name.size() / sizeof (ACHAR));
		strSubject = ACString((const ACHAR*)mailHeader.title.begin(), mailHeader.title.size() / sizeof (ACHAR));
		ConvertChatString(strSubject);
		strContent = ACString((const ACHAR*)mail.context.begin(), mail.context.size() / sizeof (ACHAR));
		ConvertChatString(strContent);
	}
	m_pTxt_NotSystemMail->Show(mailHeader.sndr_type == _MST_PLAYER);
	m_pTxt_Subject->SetText(strSubject);
	m_pTxt_MailFrom->SetText(strMailFrom);
	if(mailHeader.sndr_type == _MST_PLAYER)
		m_pTxt_Content->SetText(GetStringFromTable(982) + strContent);
	else
		m_pTxt_Content->SetText(strContent);

	ACHAR szText[20];
	if( attachItem.id != 0 && attachItem.count != 0)
	{
		CECIvtrItem *pItem = CECIvtrItem::CreateItem(attachItem.id, attachItem.expire_date, attachItem.count);
		if (pItem->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART)
		{
			((CECIvtrTalismanMain*)pItem)->SetItemFromMailInfo((unsigned char*)attachItem.data.begin(), attachItem.data.size());
		}
		else
			pItem->SetItemInfo((unsigned char*)attachItem.data.begin(), attachItem.data.size());
		pItem->UpdateInfoMD5();
		AString strFile;
		af_GetFileTitle(pItem->GetIconFile(), strFile);
		strFile.MakeLower();
		m_pImg_AttachItem->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
			GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
		const wchar_t* szDesc = pItem->GetDesc();
		AUICTranslate trans;
		if( szDesc )
			m_pImg_AttachItem->SetHint(trans.Translate(szDesc));
		else
			m_pImg_AttachItem->SetHint(_AL(""));
		delete pItem;
		a_sprintf(szText,_AL("%d"), attachItem.count);
		m_pImg_AttachItem->SetText(szText);
	}
	else
	{
		m_pImg_AttachItem->SetCover(NULL, -1);
		m_pImg_AttachItem->SetText(_AL(""));
		m_pImg_AttachItem->SetHint(_AL(""));
	}
	int nMoney = mail.attach_money;
	ACHAR szMoney[20];
	a_sprintf(szMoney, _AL("%d"), nMoney % 100);
	m_pTxt_Money3->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
	m_pTxt_Money2->SetText(szMoney);
	a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
	m_pTxt_Money1->SetText(szMoney);

	m_pBtn_GetAttach->Enable(attachItem.id != 0 && attachItem.count != 0 || mail.attach_money > 0);
}

void CDlgMailRead::GetAttachRe(void *pData)
{
	ACHAR szText[20];
	GetMailAttachObj_Re *pAttach = (GetMailAttachObj_Re *)pData;
	if( pAttach->retcode == ERR_SUCCESS )
	{
		int nMoney = pAttach->money_left;
		ACHAR szMoney[20];
		a_sprintf(szMoney, _AL("%d"), nMoney % 100);
		m_pTxt_Money3->SetText(szMoney);
		a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
		m_pTxt_Money2->SetText(szMoney);
		a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
		m_pTxt_Money1->SetText(szMoney);
		if( pAttach->item_left == 0 )
		{
			m_pImg_AttachItem->SetCover(NULL, -1);
			m_pImg_AttachItem->SetText(_AL(""));
			m_pImg_AttachItem->SetHint(_AL(""));
		}
		else
		{
			a_sprintf(szText,_AL("%d"), pAttach->item_left);
			m_pImg_AttachItem->SetText(szText);
		}
		if( pAttach->item_left > 0 || pAttach->money_left > 0 )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(704), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160));
			m_pBtn_GetAttach->Enable(true);
		}
	}
}