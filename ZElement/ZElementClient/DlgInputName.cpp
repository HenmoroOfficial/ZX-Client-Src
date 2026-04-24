// Filename	: DlgInputName.cpp
// Creator	: Tom Zhou
// Date		: October 13, 2005

#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\CSplit.h"
#include "DlgInputName.h"
#include "DlgMiniMap.h"
#include "DlgMidMap.h"
#include "DlgWorldMap.h"
#include "DlgFriendList.h"
#include "DlgFriendHistory.h"
#include "DlgReplayEdit.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_Friend.h"
#include "EC_HostPlayer.h"
#include "EC_ManPlayer.h"
#include "EC_World.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgInputName, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommand_confirm)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

CDlgInputName::CDlgInputName()
{
	m_pTxtInput = NULL;
}

CDlgInputName::~CDlgInputName()
{
}

void CDlgInputName::DoDataExchange(bool bSave)
{
	CDlgBase::DoDataExchange(bSave);

	DDX_Control("DEFAULT_Txt_Input", m_pTxtInput);
}

void CDlgInputName::OnCommand_confirm(const char * szCommand)
{
	ACString strName = m_pTxtInput->GetText();
	if( strName.GetLength() <= 0 && GetData() != INPUTNAME_FACTION_TITLE )
		return;

	PAUILISTBOX pList = (PAUILISTBOX)GetGameUIMan()->
		m_pDlgMiniMapMark->GetDlgItem("List_Choose");
	PAUILISTBOX pList1 = (PAUILISTBOX)GetGameUIMan()->
		m_pDlgMidMap->GetDlgItem("List_Choose");

	if( GetData() == INPUTNAME_ADD_GUILD )
	{
		int idPlayer = GetGameRun()->GetPlayerID(strName);

		if( ISPLAYERID(idPlayer) )
		{
			CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(idPlayer);
			if( pPlayer && (pPlayer->GetFamilyID() <= 0 || pPlayer->GetFactionID() > 0 ) )
				GetGameUIMan()->AddChatMessage(GetStringFromTable(613), GP_CHAT_MISC);
			else
			{
				GetGameSession()->faction_accept_join(idPlayer, 0);
				GetGameUIMan()->AddChatMessage(GetStringFromTable(601), GP_CHAT_MISC);
			}
		}
		else
			GetGameSession()->GetPlayerIDByName(strName, 4);
	}
	if( GetData() == INPUTNAME_ADD_FAMILY )
	{
		int idPlayer = GetGameRun()->GetPlayerID(strName);

		if( ISPLAYERID(idPlayer) )
		{
			CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(idPlayer);
			if( pPlayer && pPlayer->GetFactionID() > 0 )
				GetGameUIMan()->AddChatMessage(GetStringFromTable(862), GP_CHAT_MISC);
			else
			{
				GetGameSession()->faction_accept_join(idPlayer, 1);
				GetGameUIMan()->AddChatMessage(GetStringFromTable(858), GP_CHAT_MISC);
			}
		}
		else
			GetGameSession()->GetPlayerIDByName(strName, 2);
	}
	if( GetData() == INPUTNAME_ADD_SECT )
	{
		int idPlayer = GetGameRun()->GetPlayerID(strName);

		if( ISPLAYERID(idPlayer) )
		{
			CECPlayer *pPlayer = GetWorld()->GetPlayerMan()->GetPlayer(idPlayer);
			if( pPlayer && pPlayer->GetMaster() != 0 )
				GetGameUIMan()->AddChatMessage(GetStringFromTable(1000), GP_CHAT_MISC);
			else if( pPlayer && pPlayer->GetBasicProps().iLevel > 75 )
				GetGameUIMan()->AddChatMessage(GetStringFromTable(1008), GP_CHAT_MISC);
			else
			{
				GetGameSession()->sect_recruit(idPlayer);
				GetGameUIMan()->AddChatMessage(GetStringFromTable(999), GP_CHAT_MISC);
			}
		}
		else
			GetGameSession()->GetPlayerIDByName(strName, 5);
	}
	else if( GetData() == INPUTNAME_ADD_FRIEND )
	{
		int idPlayer = GetGameRun()->GetPlayerID(strName);
		CECFriendMan::FRIEND* pFriend = GetHostPlayer()->GetFriendMan()->GetFriendByID(idPlayer);

		//	In friend list, one player ID may have two names, so check name here !
		if( pFriend && !a_strcmp(strName, pFriend->GetName()) )
			GetGameUIMan()->AddChatMessage(GetStringFromTable(565), GP_CHAT_MISC);
		else
		{
			GetGameSession()->friend_Add(idPlayer, strName);
			GetGameUIMan()->AddChatMessage(GetStringFromTable(533), GP_CHAT_MISC);
		}
	}
	else if( GetData() == INPUTNAME_ADD_BLACKLIST )
	{
		GetGameUIMan()->m_pDlgFriendList->AddBlackList(strName);
	}
	else if( GetData() == INPUTNAME_ADD_GROUP )
	{
		int i, idMax = -1;
		CECFriendMan::GROUP *pGroup;
		CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();

		for( i = 0; i < pMan->GetGroupNum(); i++ )
		{
			pGroup = pMan->GetGroupByIndex(i);
			idMax = max(idMax, pGroup->iGroup);
		}
		GetGameSession()->friend_SetGroupName(idMax + 1, strName);
	}
	else if( GetData() == INPUTNAME_RENAME_GROUP )
	{
		int idGroup = (int)GetDataPtr("int");

		GetGameSession()->friend_SetGroupName(idGroup, strName);
	}
	else if( GetData() == INPUTNAME_FACTION_TITLE )
	{
		int idPlayer = (int)GetDataPtr("int");

		m_pTxtInput->SetMaxLength(20);
		GetGameUIMan()->FilterBadWords(strName);
		GetGameSession()->faction_rename(idPlayer, strName);
	}
	else if( GetData() == INPUTNAME_CHANNEL_INVITE )
	{
		int idRoom = (int)GetDataPtr("int");
		int idPlayer = GetGameRun()->GetPlayerID(strName);

		if( idPlayer != 0 )
			GetGameSession()->chatroom_Invite(idRoom, idPlayer);
		else
			GetGameSession()->chatroom_Invite(idRoom, strName);
		GetGameUIMan()->AddChatMessage(GetStringFromTable(633), GP_CHAT_MISC);
	}
	else if( GetData() == INPUTNAME_EDIT_RECORD)
	{
		GetGameUIMan()->m_pDlgReplayEdit->Export(strName);
	}
	else if(GetData() == INPUTNAME_CRSSVRTEAMES)
	{
		int idPlayer = GetGameRun()->GetPlayerID(strName);
		GetGameSession()->c2s_CmdSendCrssvrTeamsRecruit(strName,idPlayer);
	}
	else if(GetData() == INPUTNAME_ADD_FRIEND_REMARK)
	{
		int idPlayer = (int)GetDataPtr("int");
		GetGameUIMan()->FilterBadWords(strName);
		if(CDlgFriendList::AddFriendRemark(idPlayer,strName))
		{
			if(GetGameUIMan()->m_pDlgFriendList->IsShow())
			{
				GetGameUIMan()->BuildFriendList();
			}
			CDlgFriendHistory* pDlgHis = GetGameUIMan()->m_pDlgFriendHistory;
			if(pDlgHis->IsShow())
			{
				GetGameUIMan()->m_pDlgFriendList->BuildFriendList(pDlgHis);
				int idPlayer = pDlgHis->GetData();
				if(idPlayer > 0)
				{
				   pDlgHis->BuildChatHistory(idPlayer);
				}

			}
            
			char szName[40];
			PAUIDIALOG pDlgChat;
			for( int i = 0; i < CECGAMEUIMAN_MAX_CHATS; i++ )
			{
				sprintf(szName, "Win_FriendChat%d", i);
				pDlgChat = GetGameUIMan()->GetDialog(szName);
				if( (int)pDlgChat->GetData() == idPlayer )
				{
					if(pDlgChat->IsShow())
					{ 
						ACString strText;
						PAUIOBJECT pName = pDlgChat->GetDlgItem("Txt_ToWho");
						strText.Format(GetGameUIMan()->GetStringFromTable(548), strName);
						pName->SetText(strText);
					}
				}
			}
		}
	}
	else
	{
		if( GetData() == INPUTNAME_MAKE_MARK )
		{
			ACHAR szText[40];
			CDlgMiniMap::MARK Mark;
			
			pList->AddString(strName);
			pList1->AddString(strName);
			Mark.nNPC = -1;
			Mark.strName = strName;
			Mark.vecPos = GetGameUIMan()->m_vecTargetPos;
			Mark.idInstance = GetWorld()->GetInstanceID();
			GetGameUIMan()->m_pDlgMiniMap->m_vecMark.push_back(Mark);

			a_sprintf(szText, _AL("%d,%d"),
				int(GetGameUIMan()->m_vecTargetPos.x),
				int(GetGameUIMan()->m_vecTargetPos.z));
			pList->SetItemHint(pList->GetCount() - 1, szText);
			pList1->SetItemHint(pList1->GetCount() - 1, szText);

			GetGameUIMan()->AddChatMessage(GetStringFromTable(238), GP_CHAT_MISC);
		}
		else if( GetData() == INPUTNAME_RENAME_MARK )
		{
			AUIEditBox * pTxtCoord = (PAUIEDITBOX)GetDlgItem("Txt_NewLocation");
			ACString strText = pTxtCoord->GetText();
			if( strText.GetLength() <= 0 )
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(237), GP_CHAT_MISC);
				return;
			}
			CSplit s(strText);
			CSPLIT_STRING_VECTOR vec;
			
			for( int i = 266; i <= 269; i++ )
			{
				vec = s.Split(GetStringFromTable(i));
				if( vec.size() >= 2 ) break;
			}
			
			if( vec.size() < 2 )
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(237), GP_CHAT_MISC);
				return;
			}
			
			pList->SetText((int)GetDataPtr(), strName);
			pList->SetItemHint((int)GetDataPtr(), strText);
			pList1->SetText((int)GetDataPtr(), strName);
			pList1->SetItemHint((int)GetDataPtr(), strText);
			GetGameUIMan()->m_pDlgMiniMap->m_vecMark[pList->GetCurSel()].strName = strName;
			GetGameUIMan()->m_pDlgMiniMap->m_vecMark[pList->GetCurSel()].vecPos.x = (float)a_atoi(vec[0]);
			GetGameUIMan()->m_pDlgMiniMap->m_vecMark[pList->GetCurSel()].vecPos.z = (float)a_atoi(vec[1]);
		}
		GetGameUIMan()->m_pDlgMiniMapMark->GetDlgItem("Txt_Mark")->SetText(_AL(""));
	}

	Show(false);
	SetData(INPUTNAME_NULL);
}

void CDlgInputName::OnCommand_CANCEL(const char * szCommand)
{
	m_pTxtInput->SetMaxLength(20);
	Show(false);
	SetData(INPUTNAME_NULL);
}
