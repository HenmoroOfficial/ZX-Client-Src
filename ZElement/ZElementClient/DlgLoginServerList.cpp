// Filename	: DlgLoginServerList.cpp
// Creator	: Xiao Zhou
// Date		: 2005/11/4

#include <AFileImage.h>

#include "AFI.h"
#include "A2DSprite.h"
#include "DlgLoginServerList.h"
#include "AUI\\CSplit.h"
#include "AIniFile.h"
#include "EC_LoginUIMan.h"
#include "EC_Game.h"
#include "EC_Global.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_ThreadPing.h"
#include "gameclient.h"
#include "LuaState.h"
#include "DlgLoginServerSearch.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgLoginServerList, AUIDialog)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)
AUI_ON_COMMAND("confirm",		OnCommandConfirm)
AUI_ON_COMMAND("search",		OnCommandSearch)
AUI_ON_COMMAND("sortname",		OnCommandSortName)
AUI_ON_COMMAND("sortspeed",		OnCommandSortSpeed)
AUI_ON_COMMAND("sortstats",		OnCommandSortStats)
AUI_ON_COMMAND("sortother",		OnCommandSortOther)

AUI_ON_COMMAND("Rdo_*",			OnCommandArea)
AUI_ON_COMMAND("Combo_Serverline",	OnCommandLineType)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgLoginServerList, AUIDialog)

AUI_ON_EVENT("Lst_Server00",	WM_LBUTTONDOWN,	OnEventLBtnDown_LstServerNew)
AUI_ON_EVENT("Lst_Server00",	WM_LBUTTONUP,	OnEventLBtnDown_LstServerNew)
AUI_ON_EVENT("Lst_Server*",	WM_LBUTTONDOWN,	OnEventLBtnDown_LstServer)
AUI_ON_EVENT("Lst_Server*",	WM_LBUTTONUP,	OnEventLBtnDown_LstServer)
AUI_ON_EVENT("*",			WM_KEYDOWN,		OnEventKeyDown)
AUI_ON_EVENT(NULL,			WM_KEYDOWN,		OnEventKeyDown)

AUI_END_EVENT_MAP()

CDlgLoginServerList::CDlgLoginServerList()
{
	m_pPool = NULL;

	m_pBtn_Choose = NULL;
	m_pLst_Group = NULL;
	m_pLst_Server = NULL;
	m_pLst_ServerNew = NULL;
}

CDlgLoginServerList::~CDlgLoginServerList()
{
}

bool CDlgLoginServerList::OnInitDialog()
{
	m_pPool = new CECPool(15);
	
	DDX_Control("Btn_Choose", m_pBtn_Choose);
	DDX_Control( "Lst_Group", m_pLst_Group);
	DDX_Control( "Lst_Server", m_pLst_Server);
	DDX_Control( "Lst_Server00", m_pLst_ServerNew);

	PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Serverline");
	pCombo->AddString(m_pAUIManager->GetStringFromTable(650));
	pCombo->AddString(m_pAUIManager->GetStringFromTable(651));
	pCombo->AddString(m_pAUIManager->GetStringFromTable(652));
	pCombo->SetCurSel(0);

	ACHAR szCurGroup[MAX_PATH];
	GetPrivateProfileString(_AL("Server"), _AL("CurrentGroup"),
		_AL("UNKNOWN"), szCurGroup, MAX_PATH,  _AL("userdata\\currentserver.ini"));

	LoadGroups(szCurGroup);

	if (g_pGame->GetGameInit().szUserName[0] && g_pGame->GetGameInit().szToken[0] && g_pGame->GetGameInit().szArea[0])
	{		
		LoadServerInfo(AS2AC(g_pGame->GetGameInit().szArea));
	}
	else
		LoadServerInfo(szCurGroup);

	OnCommandArea("");
	int i;
	for (i=0; i<m_pLst_Group->GetCount(); ++i)
	{
		if (ACString(m_pLst_Group->GetText(i))==ACString(szCurGroup))
		{
			m_pLst_Group->SetCurSel(i);
			m_pLst_ServerNew->SetCurSel(-1);
			break;
		}
	}

	m_vecImageList.clear();
	A2DSprite* pSprite = new A2DSprite;
	if( !pSprite ) return AUI_ReportError(__LINE__, __FILE__);
	
	bool bval = pSprite->Init(m_pA3DDevice, "InGame\\serverstatus.tga", 0);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	
	A3DRECT rc[SERVER_STATUS_NUM];
	int W = pSprite->GetWidth();
	int H = pSprite->GetHeight() / SERVER_STATUS_NUM;
	for( i = 0; i < SERVER_STATUS_NUM; i++ )
		rc[i].SetRect(0, H * i, W, H * (i + 1));
	pSprite->ResetItems(SERVER_STATUS_NUM, rc);

	m_vecImageList.push_back(pSprite);
	m_pLst_Server->SetImageList(&m_vecImageList);
	m_pLst_Server->SetItemMask(0xffffffff);

	return true;
}

bool CDlgLoginServerList::Release()
{
	delete m_pPool;
	abase::vector<A2DSprite*>::iterator it;
	for(it=m_vecImageList.begin(); it!=m_vecImageList.end(); ++it)
	{
		A3DRELEASE(*it);
	}
	return AUIDialog::Release();
}

void CDlgLoginServerList::OnShowDialog()
{
	CECLoginUIMan *pLoginUIMan = (CECLoginUIMan *)m_pAUIManager;
	PAUIDIALOG pDlgLogin = pLoginUIMan->GetDialog("Win_Login");
	pDlgLogin->Show(false);

	// Change color of current server
	PAUILABEL pCurrentServer = (PAUILABEL)pDlgLogin->GetDlgItem("Txt_CurrentServer");
	const ACHAR *szCurServer = pCurrentServer->GetText();
	CECGameSession *pSession = g_pGame->GetGameSession();
	int i;
	for( i = 0; i < m_pLst_Server->GetCount(); i++ )
	{
		CSplit s(m_pLst_Server->GetText(i));
		CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
		if( a_stricmp(vec[0], szCurServer) == 0 )
			m_pLst_Server->SetItemTextColor(i, 0xFFFFCB4A, 0);
		else
			m_pLst_Server->SetItemTextColor(i, 0xFF7FFF86, 0);
		m_pLst_Server->SetItemData(i, 99999999, 0, "time");
		m_pLst_Server->SetItemData(i, 99999999, 2, "load");
		ACString strText;
		strText.Format(_AL("%s\t---\t---\t "), vec[0]);
		m_pLst_Server->SetItemTextColor(i, 0xFFFFFFFF, 1);
		m_pLst_Server->SetText(i, strText);
	}
}

void CDlgLoginServerList::Resize(A3DRECT rcOld, A3DRECT rcNew)
{
	AUIDialog::Resize(rcOld, rcNew);
	float fWindowScale = GetAUIManager()->GetWindowScale();
	abase::vector<A2DSprite*>::iterator it;
	for(it=m_vecImageList.begin(); it!=m_vecImageList.end(); ++it)
	{
		(*it)->SetScaleX(fWindowScale);
		(*it)->SetScaleY(fWindowScale);
	}
}

void CDlgLoginServerList::OnTick()
{
	int i;

	if(m_iCurSelGroup!=m_pLst_Group->GetCurSel())
	{
		m_iCurSelGroup = m_pLst_Group->GetCurSel();
		ChangeGroup(m_pLst_Group->GetText(m_iCurSelGroup));
	}	

	for(i=0; i<(int)m_servers.size(); i++)
	{
		if( m_servers[i].addr_valid && m_servers[i].ping_stage==0 )
		{
			m_servers[i].ping_stage = 1;
			m_servers[i].lasttime = GetTickCount();
			m_servers[i].num = 1;
			g_pGame->GetGameSession()->server_Ping(m_servers[i].serverID, 
				&m_servers[i].addr, m_servers[i].info.vecPorts[0]);
			break;
		}
	}	

	CECGameSession *pSession = g_pGame->GetGameSession();
	bool bCanChoose = false;
	int nSel = m_pLst_Server->GetCurSel();
	if( nSel >= 0 && nSel < m_pLst_Server->GetCount() )
		bCanChoose = true;
	nSel = m_pLst_ServerNew->GetCurSel();
	if( nSel >= 0 && nSel < m_pLst_ServerNew->GetCount() )
		bCanChoose = true;
	for( i = 0; i < m_pLst_Server->GetCount(); i++ )
	{
		if( m_pLst_Server->GetItemData(i, 0, "time") == 99999999 )
		{
			int id = m_pLst_Server->GetItemData(i, 1, "index");
			CSplit s(m_pLst_Server->GetText(i));
			CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
			UINT64 ui64Status;
			DWORD dwTime = g_pGame->GetGameSession()->GetPingTime(id, ui64Status);
			if( dwTime != 99999999 )
			{
				m_servers[id].ping_stage = 2;
				if( dwTime < 300 )
					m_pLst_Server->SetItemTextColor(i, 0xFF00FF7F, 1);
				else if( dwTime < 1000 )
					m_pLst_Server->SetItemTextColor(i, 0xFFFFFF7F, 1);
				else
					m_pLst_Server->SetItemTextColor(i, 0xFFFF0000, 1);
				unsigned char load = ((GNET::Attr *)&ui64Status)->load;
				if( load < 100 )
				{
					vec[2] = GetStringFromTable(222);
					m_pLst_Server->SetItemTextColor(i, 0xFF00FF7F, 2);
				}
				else if( load < 180 )
				{
					vec[2] = GetStringFromTable(223);
					m_pLst_Server->SetItemTextColor(i, 0xFFFFFF7F, 2);
				}
				else
				{
					vec[2] = GetStringFromTable(224);
					m_pLst_Server->SetItemTextColor(i, 0xFFFF0000, 2);
				}

				ACString strText;
				strText.Format(_AL("%dms"), dwTime);
				vec[1] = strText;
				ACString strStatus[3];
				ACString strStatusHint[3];
				int indexStatus=0;

				if( ((GNET::Attr *)&ui64Status)->multipleExp > 0 )
				{
					EditBoxItemBase itemImage(enumEIImage);
					itemImage.SetImageIndex(0);
					itemImage.SetImageFrame(SERVER_STATUS_EXP);
					strStatus[indexStatus] = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
					strStatusHint[indexStatus].Format(GetStringFromTable(261), ((GNET::Attr *)&ui64Status)->multipleExp);
					++indexStatus;
				}
				if( ((GNET::Attr *)&ui64Status)->multipleMoney > 0 )
				{
					EditBoxItemBase itemImage(enumEIImage);
					itemImage.SetImageIndex(0);
					itemImage.SetImageFrame(SERVER_STATUS_MONEY);
					strStatus[indexStatus] = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
					strStatusHint[indexStatus] = GetStringFromTable(263);
					++indexStatus;
				}
				if( ((GNET::Attr *)&ui64Status)->doubleObject > 0 )
				{
					EditBoxItemBase itemImage(enumEIImage);
					itemImage.SetImageIndex(0);
					itemImage.SetImageFrame(SERVER_STATUS_OBJECT);
					strStatus[indexStatus] = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
					strStatusHint[indexStatus] = GetStringFromTable(262);
					++indexStatus;
				}

				strText.Format(_AL("%s\t%s\t%s\t%s\t%s\t%s"), vec[0], vec[1], vec[2], strStatus[0], strStatus[1], strStatus[2]);
				m_pLst_Server->SetItemColHint(i, 3, strStatusHint[0]);
				m_pLst_Server->SetItemColHint(i, 4, strStatusHint[1]);
				m_pLst_Server->SetItemColHint(i, 5, strStatusHint[2]);
				m_pLst_Server->SetText(i, strText);
				m_pLst_Server->SetItemData(i, dwTime, 0, "time");
				m_pLst_Server->SetItemData(i, load, 2, "load");
				m_pLst_Server->SetItemData(i, 
					(((GNET::Attr *)&ui64Status)->multipleExp)+(((GNET::Attr *)&ui64Status)->multipleMoney)+(((GNET::Attr *)&ui64Status)->doubleObject), 3, "load");
			}
		}
	}
	
	for( i = 0; i < m_pLst_ServerNew->GetCount(); i++ )
	{
		if( m_pLst_ServerNew->GetItemData(i, 0, "time") == 99999999 )
		{
			int id = m_pLst_ServerNew->GetItemData(i, 1, "index");
			CSplit s(m_pLst_ServerNew->GetText(i));
			CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
			UINT64 ui64Status;
			DWORD dwTime = g_pGame->GetGameSession()->GetPingTime(id, ui64Status);
			if( dwTime != 99999999 )
			{
				m_servers[id].ping_stage = 2;
				if( dwTime < 300 )
					m_pLst_ServerNew->SetItemTextColor(i, 0xFF00FF7F, 1);
				else if( dwTime < 1000 )
					m_pLst_ServerNew->SetItemTextColor(i, 0xFFFFFF7F, 1);
				else
					m_pLst_ServerNew->SetItemTextColor(i, 0xFFFF0000, 1);
				unsigned char load = ((GNET::Attr *)&ui64Status)->load;
				if( load < 100 )
				{
					vec[3] = GetStringFromTable(222);
					m_pLst_ServerNew->SetItemTextColor(i, 0xFF00FF7F, 2);
				}
				else if( load < 180 )
				{
					vec[3] = GetStringFromTable(223);
					m_pLst_ServerNew->SetItemTextColor(i, 0xFFFFFF7F, 2);
				}
				else
				{
					vec[3] = GetStringFromTable(224);
					m_pLst_ServerNew->SetItemTextColor(i, 0xFFFF0000, 2);
				}
				
				ACString strText;
				strText.Format(_AL("%dms"), dwTime);
				vec[2] = strText;
				ACString strStatus[3];
				ACString strStatusHint[3];
				int indexStatus=0;
				
				if( ((GNET::Attr *)&ui64Status)->multipleExp > 0 )
				{
					EditBoxItemBase itemImage(enumEIImage);
					itemImage.SetImageIndex(0);
					itemImage.SetImageFrame(SERVER_STATUS_EXP);
					strStatus[indexStatus] = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
					strStatusHint[indexStatus].Format(GetStringFromTable(261), ((GNET::Attr *)&ui64Status)->multipleExp);
					++indexStatus;
				}
				if( ((GNET::Attr *)&ui64Status)->multipleMoney > 0 )
				{
					EditBoxItemBase itemImage(enumEIImage);
					itemImage.SetImageIndex(0);
					itemImage.SetImageFrame(SERVER_STATUS_MONEY);
					strStatus[indexStatus] = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
					strStatusHint[indexStatus] = GetStringFromTable(263);
					++indexStatus;
				}
				if( ((GNET::Attr *)&ui64Status)->doubleObject > 0 )
				{
					EditBoxItemBase itemImage(enumEIImage);
					itemImage.SetImageIndex(0);
					itemImage.SetImageFrame(SERVER_STATUS_OBJECT);
					strStatus[indexStatus] = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
					strStatusHint[indexStatus] = GetStringFromTable(262);
					++indexStatus;
				}
				
				strText.Format(_AL("%s\t%s\t%s\t%s\t%s\t%s\t%s"), vec[0], vec[1], vec[2], vec[3], strStatus[0], strStatus[1], strStatus[2]);
				m_pLst_ServerNew->SetItemColHint(i, 4, strStatusHint[0]);
				m_pLst_ServerNew->SetItemColHint(i, 5, strStatusHint[1]);
				m_pLst_ServerNew->SetItemColHint(i, 6, strStatusHint[2]);
				m_pLst_ServerNew->SetText(i, strText);
				m_pLst_ServerNew->SetItemData(i, dwTime, 0, "time");
				m_pLst_ServerNew->SetItemData(i, load, 2, "load");
			}
		}
	}
	m_pBtn_Choose->Enable(bCanChoose);
	
	for(i = 0; i < (int)m_servers.size(); i++)
	{
		if( m_servers[i].ping_stage==1 && m_servers[i].num < 4 &&
			GetTickCount() - m_servers[i].lasttime > m_servers[i].num * 10000 )
		{
			m_servers[i].lasttime = GetTickCount();
			m_servers[i].num++;
			g_pGame->GetGameSession()->server_Ping(i, &m_servers[i].addr, 
				m_servers[i].info.vecPorts[0]);
		}
	}
}

void CDlgLoginServerList::OnCommandCancel(const char* szCommand)
{
	Show(false);
	CECLoginUIMan *pLoginUIMan = (CECLoginUIMan *)m_pAUIManager;
	pLoginUIMan->GetDialog("Win_LoginServerListSearch")->Show(false);
	PAUIDIALOG pDlgLogin = pLoginUIMan->GetDialog("Win_Login");
	pDlgLogin->Show(true);
	PAUILABEL pCurrentServer = (PAUILABEL)pDlgLogin->GetDlgItem("Txt_CurrentServer");
	SetServerAddress(pCurrentServer->GetText());
	
	if ( (g_pGame->GetGameInit().szUserName[0] && g_pGame->GetGameInit().szToken[0]) ||
		(g_pGame->GetGameInit().szUserName[0] && g_pGame->GetGameInit().szPassword[0]) )
	{
		CECGameSession *pSession = g_pGame->GetGameSession();
		AString strName(g_pGame->GetGameInit().szUserName);
		strName.MakeLower();
		pSession->SetUserName(strName);
		if(g_pGame->GetGameInit().szToken[0])
			pSession->SetUserPassword(g_pGame->GetGameInit().szToken);
		else
			pSession->SetUserPassword(g_pGame->GetGameInit().szPassword);
		pSession->SetKickUserFlag(true);
		pSession->Open();
		
		g_pGame->GetGameRun()->ResetWallowInfo();
		g_pGame->GetGameRun()->SetLastLoginId(0);
		pLoginUIMan->ShowCombineDialogs("Login", false);
		pLoginUIMan->GetDialog("Win_Softkb")->Show(false);
		pLoginUIMan->GetDialog("Win_LoginWait")->Show(true);
	}
}

void CDlgLoginServerList::OnCommandConfirm(const char* szCommand)
{
	ACString strServer, strGroup;
	CECLoginUIMan *pLoginUIMan = (CECLoginUIMan *)m_pAUIManager;
	int nSels = m_pLst_Server->GetCurSel();
	int nSels1 = m_pLst_ServerNew->GetCurSel();
	if( nSels >= 0 && nSels < m_pLst_Server->GetCount() )
	{
		CSplit s(m_pLst_Server->GetText(nSels));
		CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
		g_pGame->GetGameRun()->SetCurGroup(m_pLst_Group->GetText(m_iCurSelGroup));
		strServer = vec[0];
		strGroup = m_pLst_Group->GetText(m_iCurSelGroup);
	}
	if( nSels1 >= 0 && nSels1 < m_pLst_ServerNew->GetCount() )
	{
		CSplit s(m_pLst_ServerNew->GetText(nSels1));
		CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
		g_pGame->GetGameRun()->SetCurGroup(vec[0]);
		strServer = vec[1];
		strGroup = vec[0];
	}
	if( SetServerAddress(strServer, strGroup) )
	{
		PAUIDIALOG pDlgLogin = pLoginUIMan->GetDialog("Win_Login");
		pDlgLogin->Show(true);
		Show(false);
		if ( (g_pGame->GetGameInit().szUserName[0] && g_pGame->GetGameInit().szToken[0]) ||
			(g_pGame->GetGameInit().szUserName[0] && g_pGame->GetGameInit().szPassword[0]) )
		{
			CECGameSession *pSession = g_pGame->GetGameSession();
			AString strName(g_pGame->GetGameInit().szUserName);
			strName.MakeLower();
			pSession->SetUserName(strName);
			if(g_pGame->GetGameInit().szToken[0])
				pSession->SetUserPassword(g_pGame->GetGameInit().szToken);
			else
				pSession->SetUserPassword(g_pGame->GetGameInit().szPassword);
			pSession->SetKickUserFlag(true);
			pSession->Open();
			
			g_pGame->GetGameRun()->ResetWallowInfo();
			g_pGame->GetGameRun()->SetLastLoginId(0);
			pLoginUIMan->ShowCombineDialogs("Login", false);
			pLoginUIMan->GetDialog("Win_Softkb")->Show(false);
			pLoginUIMan->GetDialog("Win_LoginWait")->Show(true);
		}
		else if (g_pGame->GetGameInit().szUserName[0] && g_pGame->GetGameInit().szPassword[0])
		{
			pLoginUIMan->OnCommand_Login("login", pLoginUIMan->GetDialog("Win_Login"));
		}
		pLoginUIMan->GetDialog("Win_LoginServerListSearch")->Show(false);
		PAUIDIALOG pDlg = pLoginUIMan->GetDialog("Win_Login");
		if (pDlg->IsShow())	// active it
		{
			pDlg->Show(true);
		}
	}
}

void CDlgLoginServerList::OnCommandSearch(const char* szCommand)
{
	CECLoginUIMan *pLoginUIMan = (CECLoginUIMan *)m_pAUIManager;
	PAUIDIALOG pDlg = pLoginUIMan->GetDialog("Win_LoginServerListSearch");
	pDlg->Show(!pDlg->IsShow());

	pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
}

void CDlgLoginServerList::OnCommandSortName(const char* szCommand)
{
	m_bSortName = !m_bSortName;
	if( m_bSortName )
		m_pLst_Server->SortItems(AUILISTBOX_SORT_ASCENT, AUILISTBOX_SORTTYPE_STRING, 0);
	else
		m_pLst_Server->SortItems(AUILISTBOX_SORT_DESCENT, AUILISTBOX_SORTTYPE_STRING, 0);
}

void CDlgLoginServerList::OnCommandSortSpeed(const char* szCommand)
{
	m_bSortSpeed = !m_bSortSpeed;
	if( m_bSortSpeed )
		m_pLst_Server->SortItemsByData(AUILISTBOX_SORT_ASCENT, AUILISTBOX_DATASORTTYPE_DWORD, 0);
	else
		m_pLst_Server->SortItemsByData(AUILISTBOX_SORT_DESCENT, AUILISTBOX_DATASORTTYPE_DWORD, 0);
}

void CDlgLoginServerList::OnCommandSortStats(const char* szCommand)
{
	m_bSortStats = !m_bSortStats;
	if( m_bSortStats )
		m_pLst_Server->SortItemsByData(AUILISTBOX_SORT_ASCENT, AUILISTBOX_DATASORTTYPE_DWORD, 2);
	else
		m_pLst_Server->SortItemsByData(AUILISTBOX_SORT_DESCENT, AUILISTBOX_DATASORTTYPE_DWORD, 2);
}

void CDlgLoginServerList::OnCommandSortOther(const char* szCommand)
{
	m_bSortOther = !m_bSortOther;
	if( m_bSortOther )
		m_pLst_Server->SortItems(AUILISTBOX_SORT_ASCENT, AUILISTBOX_SORTTYPE_STRING, 3);
	else
		m_pLst_Server->SortItems(AUILISTBOX_SORT_DESCENT, AUILISTBOX_SORTTYPE_STRING, 3);
}

void CDlgLoginServerList::OnCommandArea(const char* szCommand)
{
	m_pLst_Group->ResetContent();
	int index = GetCheckedRadioButton(1);
	if (index<0)
	{
		CheckRadioButton(1, 1);
		index = 1;
	}
	unsigned int i;
	if (index==1)
	{
		unsigned int j;
		for (i=0; i<3; ++i)
		{
			for (j=0; j<m_vecGroups[i].size(); ++j)
			{
				m_pLst_Group->AddString(m_vecGroups[i][j]);
			}
		}
	}
	else
	{
		for (i=0; i<m_vecGroups[index-2].size(); ++i)
		{
			m_pLst_Group->AddString(m_vecGroups[index-2][i]);
		}
	}
	m_iCurSelGroup = -1;
	m_pLst_Group->SetCurSel(0);
}

void CDlgLoginServerList::OnCommandLineType(const char* szCommand)
{
	PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Serverline");
	int iLineType = pCombo->GetCurSel();
	ACString strText;
	int i;
	UINT j;
	if( GetCheckedRadioButton(1)==1 || GetCheckedRadioButton(1)==2)
	{
		int iCurSel = m_pLst_Server->GetCurSel();
		
		for (i=0; i<m_pLst_Server->GetCount(); ++i)
		{
			SERVERENTRY& srvEntryCur = m_servers[m_pLst_Server->GetItemData(i, 1, "index")];
			if (srvEntryCur.info.iType<0)
				continue;

			for (j=0; j<m_servers.size(); ++j)
			{
				SERVERENTRY& srvEntry = m_servers[j];
				if(srvEntryCur.info.strGroup!=srvEntry.info.strGroup || srvEntryCur.info.strName!=srvEntry.info.strName)
					continue;
				
				if (iLineType!=srvEntry.info.iType)
					continue;
				
				strText.Format(_AL("%s\t---\t---\t "), srvEntry.info.strName);
				m_pLst_Server->SetText(i, strText);
				m_pLst_Server->SetItemData(i, 99999999, 0, "time");
				m_pLst_Server->SetItemData(i, j, 1, "index");
				
				if(srvEntry.ping_stage==-1)
				{
					srvEntry.ping_stage = 0;
					m_pPool->AddTask(&m_servers[j]);
				}
				break;
			}
		}

		m_pLst_Server->SetCurSel(iCurSel);
	}
	
	int iCurSel = m_pLst_ServerNew->GetCurSel();
	for (i=0; i<m_pLst_ServerNew->GetCount(); ++i)
	{
		SERVERENTRY& srvEntryCur = m_servers[m_pLst_ServerNew->GetItemData(i, 1, "index")];
		if (srvEntryCur.info.iType<0)
			continue;
		
		for (j=0; j<m_servers.size(); ++j)
		{
			SERVERENTRY& srvEntry = m_servers[j];
			if(srvEntryCur.info.strGroup!=srvEntry.info.strGroup || srvEntryCur.info.strName!=srvEntry.info.strName)
				continue;
			
			if (iLineType!=srvEntry.info.iType)
				continue;
			
			strText.Format(_AL("%s\t%s\t---\t---\t "), srvEntry.info.strGroup, srvEntry.info.strName);
			m_pLst_ServerNew->SetText(i, strText);
			m_pLst_ServerNew->SetItemData(i, 99999999, 0, "time");
			m_pLst_ServerNew->SetItemData(i, j, 1, "index");
			
			if(srvEntry.ping_stage==-1)
			{
				srvEntry.ping_stage = 0;
				m_pPool->AddTask(&m_servers[j]);
			}
			break;
		}
	}
	m_pLst_ServerNew->SetCurSel(iCurSel);
}

void CDlgLoginServerList::OnEventLBtnDown_LstServerNew(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	m_pLst_Server->SetCurSel(-1);
}

void CDlgLoginServerList::OnEventSelect_LstServerNew(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	m_pLst_Server->SetCurSel(-1);
}

void CDlgLoginServerList::OnEventLBtnDown_LstServer(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	m_pLst_ServerNew->SetCurSel(-1);
}

void CDlgLoginServerList::OnEventSelect_LstServer(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	m_pLst_ServerNew->SetCurSel(-1);
}

void CDlgLoginServerList::OnEventKeyDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( wParam == VK_RETURN )
		OnCommandConfirm("");
}

bool CDlgLoginServerList::SetServerAddress(ACString strName, ACString strGroup)
{
	if ( strName==_AL("") )
		return false;
	
	bool bFind = false;
	int i;
	if (strGroup.GetLength()==0)
		strGroup = m_pLst_Group->GetText(m_iCurSelGroup);
	ACString strAddr;
	int iPort;
	PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Serverline");
	for (i=0; i<(int)m_servers.size(); ++i)
	{
		if (m_servers[i].info.strName==strName && m_servers[i].info.strGroup==strGroup)
		{
			if (m_servers[i].info.iType<0)
			{
				strAddr = AS2AC(m_servers[i].info.vecAddress[0]);
				iPort = m_servers[i].info.vecPorts[0];
				bFind = true;
				break;
			}
			else
			{
				if (m_servers[i].info.iType==pCombo->GetCurSel())
				{
					strAddr = AS2AC(m_servers[i].info.vecAddress[0]);
					iPort = m_servers[i].info.vecPorts[0];
					bFind = true;
					break;
				}
			}
		}
	}

	if ( !bFind )
		return false;

	CECLoginUIMan *pLoginUIMan = (CECLoginUIMan *)m_pAUIManager;
	pLoginUIMan->SaveServer(strName, strAddr);

	m_pLst_Server->SetCurSel(m_pLst_Server->GetCount());
	m_pLst_Server->EnsureVisible(0);
	for( i = 0; i < m_pLst_Server->GetCount(); i++ )
	{
		CSplit s(m_pLst_Server->GetText(i));
		CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
		if( strName == vec[0] )
		{
			m_pLst_Server->BringItemToTop(i);
			m_pLst_Server->SetCurSel(0);
			break;
		}
	}

	CSplit ss(strAddr);
	CSPLIT_STRING_VECTOR vecs = ss.Split(_AL(":"));
	CECGame::GAMEINIT &gi = g_pGame->GetGameInit();
	strcpy(gi.szIP, AC2AS(vecs[0]));
	strcpy(gi.szServerName, AC2AS(strName));
	gi.iPort = iPort;
	gi.iType = pCombo->GetCurSel();
	gi.iServerID = 	m_mapServerRealID[strName];
	PAUIDIALOG pDlgLogin = pLoginUIMan->GetDialog("Win_Login");
	PAUILABEL pCurrentServer = (PAUILABEL)pDlgLogin->GetDlgItem("Txt_CurrentServer");
	pCurrentServer->SetAlign(AUIFRAME_ALIGN_CENTER);
	pCurrentServer->SetText(strName);
	return true;
}

void CDlgLoginServerList::ChangeGroup(ACString strGroup, ACString strSvr)
{
	ACString strCurGroup = strGroup;
	ACString strText;
	
	if (strCurGroup != m_pLst_Group->GetText(m_pLst_Group->GetCurSel()))
	{
		for (int i=0; i<m_pLst_Group->GetCount(); ++i)
		{
			if ( strCurGroup==m_pLst_Group->GetText(i) )
			{
				m_pLst_Group->SetCurSel(i);
				m_iCurSelGroup = i;
				break;
			}
		}
	}
	m_pLst_Server->ResetContent();
	
	PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Serverline");
	int iLineType = pCombo->GetCurSel();
	int iStart = m_servers.size();
	m_mapServerRealID.clear();
	m_nServerSum = 0;
	UINT j=0;
	for (j=0; j<m_servers.size(); ++j)
	{
		SERVERENTRY& srvEntry = m_servers[j];
		if(strCurGroup!=srvEntry.info.strGroup)
			continue;

		if (srvEntry.info.iType!=-1 && iLineType!=srvEntry.info.iType)
			continue;
		
		strText.Format(_AL("%s\t---\t---\t "), srvEntry.info.strName);
		int n = rand() % (m_pLst_Server->GetCount() + 1);
		m_pLst_Server->InsertString(n, strText);
		m_mapServerRealID[srvEntry.info.strName] = srvEntry.info.iId;
		m_pLst_Server->SetItemData(n, j, 1, "index");
		
		if(srvEntry.ping_stage==-1)
		{
			srvEntry.ping_stage = 0;
			m_pPool->AddTask(&m_servers[j]);
		}
	}

	for (int i=0; i<m_pLst_Server->GetCount(); i++)
	{
		ACString strText = m_pLst_Server->GetText(i);
		strText = strText.Left(strText.Find(_AL("\t")));
		if (strText == strSvr)
		{
			m_pLst_Server->SetCurSel(i);
			OnCommandConfirm("");
		}
		m_pLst_Server->SetItemData(i, 99999999, 0, "time");
		m_pLst_Server->SetItemData(i, 99999999, 2, "load");

		if (strText == g_pGame->GetGameRun()->GetServerName())
		{
			m_pLst_Server->BringItemToTop(i);
			m_pLst_Server->SetItemTextColor(0, 0xFFFFCB4A, 0);
		}
	}
	if (m_pLst_ServerNew->GetCurSel()>=0)
	{
		m_pLst_Server->SetCurSel(-1);
	}
}

void CDlgLoginServerList::LoadServerInfo(ACString strCurGroup)
{
	m_iCurSelGroup = -1;
	m_pLst_Group->ResetContent();
	m_servers.clear();
	m_mapServerRealID.clear();

	FILE *fStream;
	if( fStream = fopen("userdata\\server\\newserver\\serverlist.txt", "rb") )
	{
		ACHAR szLine[256]; 
		ACString strText;
		a_fgetc(fStream);
		ACString strGroup;
		do
		{
			if( !a_fgets(szLine, 512, fStream) )
				break;
			if( a_strlen(szLine) > 0)
			{
				int n = a_strlen(szLine) - 1;
				while( n > 0 && (szLine[n] == '\r' || szLine[n] == '\n') )
					n--;
				szLine[n + 1] = '\0';
				CSplit s(szLine);
				CSplit::VectorAWString vec = s.Split(_AL("\t"));
				if( vec.size() >= 2 )
				{
					CSplit ss(vec[1]);
					CSplit::VectorAWString vecs = ss.Split(_AL(":"));
					if( vecs.size() >= 2 )
					{
						SERVERINFO svrInfo;
						svrInfo.strGroup = strGroup;
						svrInfo.strName = vec[0];
						svrInfo.iType = vecs.size()>2 ? vecs[2].ToInt() : -1;
						if( vec.size() > 2 )
							m_mapServerRealID[vec[0]] = AWString(vec[2]).ToInt();
						else
							m_mapServerRealID[vec[0]] = 0;
						
						CSplit sAddr(AC2AS(vecs[1]));
						CSplit::VectorAString vecAddrs = sAddr.Split("*");
						unsigned int k;
						for (k=0; k<vecAddrs.size(); ++k)
						{
							svrInfo.vecAddress.push_back(vecAddrs[k]);
						}
						
						CSplit s1(vecs[0]);
						CSplit::VectorAWString vecPorts = s1.Split(_AL("*"));
						k;
						for (k=0; k<vecPorts.size(); ++k)
						{
							svrInfo.vecPorts.push_back(vecPorts[k].ToInt());
						}
						svrInfo.iId = vec.size()>2 ? vec[2].ToInt() : 0;
						SERVERENTRY srvEntry;
						srvEntry.info = svrInfo;
						srvEntry.serverID = m_servers.size();
						m_servers.push_back(srvEntry);
					}
				}
				else if(szLine[0] != '\r' && szLine[0] != '\n')
				{
					strGroup = szLine;
				}
			}
		} while(true);
		fclose(fStream);
	}
	else
		AUI_ReportError(__LINE__, 1,"CDlgLoginServerList::LoadServerInfo(), server list file error!");

	m_pLst_ServerNew->ResetContent();
	PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Serverline");
	int iLineType = pCombo->GetCurSel();
	UINT j=0;
	ACString strText;
	for (j=0; j<m_servers.size(); ++j)
	{
		SERVERENTRY& srvEntry = m_servers[j];
		UINT i;
		for (i=0; i<m_strServersRecommend.size(); ++i)
		{
			if(m_strServersRecommend[i]==srvEntry.info.strName)
				break;
		}
		if(i==m_strServersRecommend.size())
			continue;
		
		if (srvEntry.info.iType!=-1 && iLineType!=srvEntry.info.iType)
			continue;

		strText.Format(_AL("%s\t%s\t---\t---\t "), srvEntry.info.strGroup, srvEntry.info.strName);
		//int n = m_pLst_ServerNew->GetCount();
		int n = rand() % (m_pLst_ServerNew->GetCount() + 1);
		m_pLst_ServerNew->InsertString(n, strText);
		m_mapServerRealID[srvEntry.info.strName] = srvEntry.info.iId;
		m_pLst_ServerNew->SetItemData(n, 99999999, 0, "time");
		m_pLst_ServerNew->SetItemData(n, j, 1, "index");
		
		if(srvEntry.ping_stage==-1)
		{
			srvEntry.ping_stage = 0;
			m_pPool->AddTask(&m_servers[j]);
		}
	}
}

void CDlgLoginServerList::LoadGroups(ACString strCurGroup)
{
	FILE *fStream;

	int index = 0;
	if( fStream = fopen("userdata\\server\\newserver\\servergroup_new.txt", "rb") )
	{
		ACHAR szLine[256]; 
		ACString strText;
		a_fgetc(fStream);
		ACString strGroup;
		do
		{
			if( !a_fgets(szLine, 512, fStream) )
				break;
			if( a_strlen(szLine) > 0)
			{
				if (szLine[0] == '/' && szLine[1] == '/')
				{
					continue;
				}
				int n = a_strlen(szLine) - 1;
				while( n > 0 && (szLine[n] == '\r' || szLine[n] == '\n') )
					n--;
				szLine[n + 1] = '\0';

				CSplit s(szLine);
				CSplit::VectorAWString vec = s.Split(_AL(","));
				unsigned int i;
				if (index>GROUP_TYPE_WANGTONG)
				{
					for (i=0; i<vec.size(); ++i)
					{
						m_strServersRecommend.push_back(vec[i]);
					}
				}
				else
				{
					m_vecGroups[index].clear();
					for (i=0; i<vec.size(); ++i)
					{
						m_vecGroups[index].push_back(vec[i]);
						if (strCurGroup==vec[i])
						{
							CheckRadioButton(1, index+2);
						}
					}
					index++;
				}
			}
		} while(true);
		fclose(fStream);
	}
	else
		AUI_ReportError(__LINE__, 1,"CDlgLoginServerList::LoadGroups(), server group file error!");
}

void CDlgLoginServerList::ResetPort()
{
	CECGame::GAMEINIT &gi = g_pGame->GetGameInit();
	ACString strGroup = m_pLst_Group->GetText(m_iCurSelGroup);
	ACString strName = AS2AC(gi.szServerName);
	AString strAddr(gi.szIP);

	int i;
	for (i=0; i<(int)m_servers.size(); ++i)
	{
		if (m_servers[i].info.strName==strName && m_servers[i].info.strGroup==strGroup && m_servers[i].info.iType==gi.iType)
		{
			SERVERENTRY& entry = m_servers[i];
			if (entry.info.vecPorts.size()==1 && entry.info.vecAddress.size()==1)
			{
				strncpy(gi.szIP, entry.info.vecAddress[0].GetBuffer(0), entry.info.vecAddress[0].GetLength());
				gi.szIP[entry.info.vecAddress[0].GetLength()] = 0;
				gi.iPort = entry.info.vecPorts[0];
			}
			else
			{
				int iTryMax = 20;
				while(iTryMax>0)
				{
					int index = a_Random(0, entry.info.vecPorts.size()*entry.info.vecAddress.size()-1);
					int indexAddr = index/entry.info.vecPorts.size();
					int indexPort = index%entry.info.vecPorts.size();
					if (gi.iPort!=entry.info.vecPorts[indexPort] || AString(gi.szIP)!=entry.info.vecAddress[indexAddr])
					{
						strncpy(gi.szIP, entry.info.vecAddress[indexAddr].GetBuffer(0), entry.info.vecAddress[indexAddr].GetLength());
						gi.szIP[entry.info.vecAddress[indexAddr].GetLength()] = 0;
						gi.iPort = entry.info.vecPorts[indexPort];
						break;
					}
					iTryMax--;
				}
			}
			break;
		}
	}


}