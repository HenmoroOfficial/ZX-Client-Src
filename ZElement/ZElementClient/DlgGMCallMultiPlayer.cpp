#include "AFI.h"
#include "DlgGMCallMultiPlayer.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\CSplit.h"
#include "AWScriptFile.h"
#include "EC_GameSession.h"

AUI_BEGIN_COMMAND_MAP(CDlgGMCallMultiPlayer, CDlgBase)

AUI_ON_COMMAND("Btn_Refresh",		OnCommandChangeRefresh)
AUI_ON_COMMAND("Combo_Group",		OnCommandChangeGroup)
AUI_ON_COMMAND("Btn_Individual",	OnCommandCallSingle)
AUI_ON_COMMAND("Btn_Group",			OnCommandCallMulti)

AUI_END_COMMAND_MAP()

const AString CDlgGMCallMultiPlayer::CONFIG_FILENAME = "gmcallmultplayer.txt";

CDlgGMCallMultiPlayer::CDlgGMCallMultiPlayer()
{

}

CDlgGMCallMultiPlayer::~CDlgGMCallMultiPlayer()
{
	
}

bool CDlgGMCallMultiPlayer::OnInitDialog()
{
	m_pComboGroup = (PAUICOMBOBOX)GetDlgItem("Combo_Group");
	m_pLstNames = (PAUILISTBOX)GetDlgItem("Lst_NameList");

	return true;
}

void CDlgGMCallMultiPlayer::OnShowDialog()
{
	UpdateFromFile();
}

void CDlgGMCallMultiPlayer::OnTick()
{

}

void CDlgGMCallMultiPlayer::OnCommandChangeRefresh(const char * szCommand)
{
	UpdateFromFile();
}

void CDlgGMCallMultiPlayer::OnCommandChangeGroup(const char * szCommand)
{
	UpdateDisplay();
}

void CDlgGMCallMultiPlayer::OnCommandCallSingle(const char * szCommand)
{
	int iCurSel = m_pLstNames->GetCurSel();
	if (iCurSel>=0 && iCurSel<m_pLstNames->GetCount())
	{
		GetGameSession()->gm_CallInPlayer(m_pLstNames->GetItemData(iCurSel, 0));
	}
}

void CDlgGMCallMultiPlayer::OnCommandCallMulti(const char * szCommand)
{
	for (int i=0; i<m_pLstNames->GetCount(); ++i)
	{
		GetGameSession()->gm_CallInPlayer(m_pLstNames->GetItemData(i, 0));
	}
}

void CDlgGMCallMultiPlayer::UpdateFromFile()
{
	m_pComboGroup->ResetContent();
	m_pLstNames->ResetContent();
	m_pGroupInfo.clear();
	
	ACString strText;
	int iState = 0; // 简单状态机 0-新的分组 1-读取分组名字
	AFile afile;
	if ( afile.Open(CONFIG_FILENAME, AFILE_OPENEXIST|AFILE_TEXT|AFILE_NOHEAD|AFILE_TEMPMEMORY))
	{
		char line[AFILE_LINEMAXLEN];
		DWORD  read_len;
		PlayerInfo playerInfo;
		ACString strGroupName;
		while (afile.ReadLine(line, AFILE_LINEMAXLEN, &read_len))
		{
			if (read_len>2 && line[0]=='/' && line[1]=='/')
			{
				continue;
			}
			if (read_len<=1)	// only return
			{
				m_pGroupInfo[strGroupName] = playerInfo;
				playerInfo.clear();
				iState = 0;
				continue;
			}
			CSplit s(AS2WC(line));
			CSplit::VectorAWString vec = s.Split(_AL(" "));
			if (vec.size()!=2)
				continue;

			if (iState==0)
			{
				strGroupName = vec[1];
				m_pComboGroup->AddString(vec[1]);
				iState = 1;
			}
			else if( iState==1 )
			{
				playerInfo.push_back(abase::pair<ACString, int>(vec[0], vec[1].ToInt()));
				strText.Format(_AL("%s\t%s"), vec[0], vec[1]);
				m_pLstNames->AddString(strText);
				m_pLstNames->SetItemData(m_pLstNames->GetCount()-1, vec[1].ToInt(), 0);
			}
		}
		if (playerInfo.size()>0)
		{
			m_pGroupInfo[strGroupName] = playerInfo;
		}
		afile.Close();
	}

	m_pComboGroup->SetCurSel(0);
	UpdateDisplay();
}

void CDlgGMCallMultiPlayer::UpdateDisplay()
{
	m_pLstNames->ResetContent();
	ACString strGroupName = m_pComboGroup->GetText(m_pComboGroup->GetCurSel());
	GroupInfo::iterator it = m_pGroupInfo.find(strGroupName);
	if ( it == m_pGroupInfo.end())
		return;
	
	ACString strText;
	PlayerInfo pInfo = it->second;
	for (PlayerInfo::iterator itPlayer=pInfo.begin(); itPlayer!=pInfo.end(); ++itPlayer)
	{
		strText.Format(_AL("%s\t%d"), (*itPlayer).first, (*itPlayer).second);
		m_pLstNames->AddString(strText);
		m_pLstNames->SetItemData(m_pLstNames->GetCount()-1, (*itPlayer).second);
	}
}