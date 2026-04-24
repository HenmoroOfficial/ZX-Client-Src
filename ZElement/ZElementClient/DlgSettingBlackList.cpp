/********************************************************************
	created:	2005/08/11
	created:	11:8:2005   17:50
	file name:	DlgSettingBlackList.cpp
	author:		yaojun
	
	purpose:	
*********************************************************************/

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_ManPlayer.h"
#include "DlgSettingBlackList.h"
#include "aui\\AUIStillImageButton.h"
#include "aui\\AUIListBox.h"
#include "aui\\AUIEditBox.h"


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSettingBlackList, CDlgSetting)

AUI_ON_COMMAND("add", OnCommandAdd)
AUI_ON_COMMAND("remove", OnCommandRemove)
AUI_ON_COMMAND("choosename", OnCommandChooseName)

AUI_END_COMMAND_MAP()

CDlgSettingBlackList::CDlgSettingBlackList()
{
	m_pEbxName = NULL;
	m_pLbxBlackList = NULL;
}

CDlgSettingBlackList::~CDlgSettingBlackList()
{

}

void CDlgSettingBlackList::DoDataExchange(bool bSave)
{
	CDlgSetting::DoDataExchange(bSave);

	DDX_Control("Txt_Name", m_pEbxName);
	DDX_Control("Lst_Blacklist", m_pLbxBlackList);
	DDX_EditBox(bSave, "Txt_Name", m_strName);
}

bool CDlgSettingBlackList::OnInitDialog()
{
	if (!CDlgSetting::OnInitDialog())
		return false;
	
	UpdateView();
	
	return true;
}

void CDlgSettingBlackList::OnShowDialog()
{
	CDlgSetting::OnShowDialog();
	
	// init control
	m_pBtnSystem->SetPushed(false);
	m_pBtnGame->SetPushed(false);
	m_pBtnVideo->SetPushed(false);
	m_pBtnBlackList->SetPushed(true);
	m_pBtnDefault->Show(false);

	// init diaplay
	UpdateView();
}

void CDlgSettingBlackList::OnCommandAdd(const char *szCommand)
{
	UpdateData(true);
	if( m_strName == _AL("") )
		return;

	int nCount = m_pLbxBlackList->GetCount();
	// blacklist length limit
	if( nCount + m_vecBlackCache.size() >= CDLGSETTINGBLACKLIST_BLACKLIST_LEN )
	{
		AUIDialog *pMsgBox = NULL;
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(262), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetLife(3);
		return;
	}

	// avoid duplication 
	bool bDuplicate = false;
	for (int i = 0; i < nCount; i++ )
	{
		AUIDialog *pMsgBox = NULL;
		ACString strItem = m_pLbxBlackList->GetText(i);
		if( 0 == m_strName.CompareNoCase(strItem) )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(260), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			pMsgBox->SetLife(3);
			return;
		}
	}

	// get player id if he is online and valid
	int idRole = GetGameRun()->GetPlayerID(m_strName);
	if( idRole > 0 )	// online and valid
	{
		m_pLbxBlackList->AddString(m_strName);
		m_setting.idPlayer[nCount] = idRole;
		g_pGame->GetConfigs()->SetBlackListSettings(m_setting);
	}
	else // offline or invalid
	{
		GetGameSession()->GetPlayerIDByName(m_strName, 0);
		BLACK_CACHE bc;
		bc.dwTime = GetTickCount();
		bc.strName = m_strName;
		m_vecBlackCache.push_back(bc);
	}
	
	// set control state
	m_strName.Empty();
	UpdateData(false);
	m_pEbxName->SetFocus(true);
}

void CDlgSettingBlackList::OnCommandRemove(const char *szCommand)
{
	int nCount = m_pLbxBlackList->GetCount();
	int nCurSel = m_pLbxBlackList->GetCurSel();
	
	if( nCount > 0 && nCurSel >= 0 && nCurSel < nCount )
	{
		m_pLbxBlackList->DeleteString(nCurSel);
		if( nCurSel + 1 < nCount )
		{
			memmove(m_setting.idPlayer + nCurSel, m_setting.idPlayer + nCurSel + 1,
				sizeof(int) * (nCount - nCurSel - 1));
		}
		m_setting.idPlayer[nCount - 1] = 0;
		g_pGame->GetConfigs()->SetBlackListSettings(m_setting);
	}
}

void CDlgSettingBlackList::OnCommandChooseName(const char *szCommand)
{
	int nCurSel = m_pLbxBlackList->GetCurSel();
	m_strName = m_pLbxBlackList->GetText(nCurSel);
	UpdateData(false);
}

void CDlgSettingBlackList::OnTick(void)
{
	CDlgSetting::OnTick();

	if( m_vecBlackCache.size() == 0 || !IsShow())
		return;
	
	CECGameRun *pGameRun = g_pGame->GetGameRun();
	DWORD dwTick = GetTickCount();
	
	for(int i = 0; i < (int)m_vecBlackCache.size(); i++ )
	{
		int idRole = pGameRun->GetPlayerID(m_vecBlackCache[i].strName);
		if( idRole > 0 )
		{
			m_pLbxBlackList->AddString(m_vecBlackCache[i].strName);
			m_setting.idPlayer[m_pLbxBlackList->GetCount()] = idRole;
			m_vecBlackCache.erase(m_vecBlackCache.begin() + i);
			i--;
		}
		else if( dwTick - m_vecBlackCache[i].dwTime >= 10 * 1000 )
		{
			ACString strMsg;
			strMsg.Format(GetGameUIMan()->GetStringFromTable(261), m_vecBlackCache[i].strName);
			GetGameUIMan()->AddChatMessage(strMsg, GP_CHAT_MISC);
			m_vecBlackCache.erase(m_vecBlackCache.begin() + i);
			i--;
		}
	}
}

void CDlgSettingBlackList::UpdateView()
{
	m_setting = GetGame()->GetConfigs()->GetBlackListSettings();
	
	m_pLbxBlackList->ResetContent();
	m_vecBlackCache.clear();
	CECGameRun *pGameRun = GetGame()->GetGameRun();
	for (int i = 0; i < CDLGSETTINGBLACKLIST_BLACKLIST_LEN && m_setting.idPlayer[i] > 0; i++)
	{
		m_pLbxBlackList->AddString(pGameRun->GetPlayerName(m_setting.idPlayer[i], true));
	}
	
	m_strName.Empty();
	UpdateData(false);
}
