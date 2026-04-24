// File		: DlgFriendHistory.cpp
// Creator	: Xiao Zhou
// Date		: 2005/8/18

#include "AFI.h"
#include "time.h"
#include "AUI\\CSplit.h"
#include "DlgFriendHistory.h"
#include "DlgFriendList.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"

#define new A_DEBUG_NEW

AUI_BEGIN_EVENT_MAP(CDlgFriendHistory, CDlgBase)

AUI_ON_EVENT("List_Message",	WM_LBUTTONUP,		OnEventLButtonUp)

AUI_END_EVENT_MAP()

CDlgFriendHistory::CDlgFriendHistory()
{
	m_pTxtContent = NULL;
	m_pListMessage = NULL;
	m_pTvFriendList = NULL;
	m_pItemSelected = NULL;
}

CDlgFriendHistory::~CDlgFriendHistory()
{
}

bool CDlgFriendHistory::OnInitDialog()
{
	GetGameUIMan()->AttachEmotion(m_pTxtContent);
	m_pListMessage->SetItemMask(0xffffffff);
	return true;
}

void CDlgFriendHistory::DoDataExchange(bool bSave)
{
	CDlgBase::DoDataExchange(bSave);

	DDX_Control("Txt_Content", m_pTxtContent);
	DDX_Control("List_Message", m_pListMessage);
	DDX_Control("Tv_FriendList", m_pTvFriendList);
}

void CDlgFriendHistory::OnTick()
{
	P_AUITREEVIEW_ITEM pItem = m_pTvFriendList->GetSelectedItem();
	if (pItem && pItem != m_pItemSelected)
	{
		SetData(m_pTvFriendList->GetItemData(pItem));
		BuildChatHistory(m_pTvFriendList->GetItemData(pItem));
		m_pItemSelected = pItem;
	}

}

void CDlgFriendHistory::OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	PAUILISTBOX pList = (PAUILISTBOX)pObj;
	int nCurSel = pList->GetCurSel();
	if( nCurSel < 0 || nCurSel >= pList->GetCount() )
	{
		m_pTxtContent->SetText(_AL(""));
	}
	else
		m_pTxtContent->SetText(m_vecMsg[nCurSel]);
}

void CDlgFriendHistory::BuildChatHistory(int idPlayer)
{
	FILE *file;
	char szFile[MAX_PATH];
	CECHostPlayer *pHost = GetHostPlayer();
	PAUILISTBOX pList = m_pListMessage;

	pList->ResetContent();
	m_pTxtContent->SetText(_AL(""));

	sprintf(szFile, "%s\\Userdata\\Msg\\%d\\%d.txt",
		af_GetBaseDir(), pHost->GetCharacterID(), idPlayer);
	file = fopen(szFile, "rb");
	if( !file ) return;

	DWORD dwTime;
	struct tm *ptm;
	int nLen, nRead;
	ACString strItem;
	ACHAR szName[40], szText[512];
	m_vecMsg.clear();

	while( true )
	{
		// Player name.
		nRead = fread(&nLen, sizeof(int), 1, file);
		if( nRead <= 0 ) break;		// Error or end of file.
		memset(szName, 0, sizeof(ACHAR) * 40);
		nRead = fread(szName, sizeof(ACHAR), nLen, file);

		// Message time.
		nRead = fread(&dwTime, sizeof(DWORD), 1, file);

		// Message content.
		nRead = fread(&nLen, sizeof(int), 1, file);
		memset(szText, 0, sizeof(ACHAR) * 512);
		nRead = fread(szText, sizeof(ACHAR), nLen, file);

		ptm = localtime((const time_t *)&dwTime);
		{

			ACString strName;
			if(CDlgFriendList::GetFriendRemark(idPlayer,strName))
			{
				memset(szName,0,sizeof(szName));
				a_strcpy(szName,strName.GetBuffer(0));
			}

		}

		strItem.Format(_AL("%s\t%04d.%02d.%02d\t%s"), szName,
			ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, szText);
		m_vecMsg.push_back(ACString(szText));
		pList->AddString(strItem);
	}

	fclose(file);

	pList->SetCurSel(0);
	OnEventLButtonUp(0, 0, pList);
}
