#include "AFI.h"
#include "EC_Global.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_Player.h"
#include "EC_ManPlayer.h"
#include "EC_LoginPlayer.h"
#include "EC_Model.h"
#include "EC_PortraitRender.h"
#include "EC_Algorithm.h"
#include "EC_Team.h"

#include "AUI\\AUICommon.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUISubDialog.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUIImagePicture.h"


#include "DlgCrssvrTeams.h"
#include "DlgInputName.h"
#include "crssvrteamsrolelist_re.hpp"
#include "crssvrteamsteambase_re.hpp"
#include "DlgCrssvrTeamsSub.h"
#include "DlgChat.h"
#include "teamraidmappingsuccess.hpp"
#include "gettoptable_re.hpp"
#include "A3DEnvironment.h"
#include "A3DEngine.h"
AUI_BEGIN_COMMAND_MAP(DlgCrssvrTeamsMin, CDlgBase)

AUI_ON_COMMAND("Btn_Max", OnCommand_Click)
AUI_ON_COMMAND("Btn_Group", OnCommand_6V6)
AUI_ON_COMMAND("Btn_6V6", OnCommand_Group)

AUI_END_COMMAND_MAP()



DlgCrssvrTeamsMin::DlgCrssvrTeamsMin()
{

}
DlgCrssvrTeamsMin::~DlgCrssvrTeamsMin()
{

}

bool DlgCrssvrTeamsMin::OnInitDialog()
{
	GetDlgItem("Btn_Group")->Show(false);
	GetDlgItem("Btn_6V6")->Show(false);
	return true;
}


void DlgCrssvrTeamsMin::OnCommand_Click(const char * szCommand)
{
	GetDlgItem("Btn_Group")->Show(!GetDlgItem("Btn_Group")->IsShow());
	GetDlgItem("Btn_6V6")->Show(!GetDlgItem("Btn_6V6")->IsShow());
}

void DlgCrssvrTeamsMin::OnCommand_Group(const char * szCommand)
{
	PAUIDIALOG	pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_MapSelect");
	pDlg->Show(!pDlg->IsShow());
}


void DlgCrssvrTeamsMin::OnCommand_6V6(const char * szCommand)
{
	DlgCrssvrTeamsMain*	pDlg = (DlgCrssvrTeamsMain*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupMain");
	GetGameSession()->c2s_CmdSendCrssvrTeamsBase();
	GetGameSession()->c2s_CmdSendCrssvrTeamsGetRoleList();
	pDlg->SetWindowsType(2);
	pDlg->Show(true);

}



AUI_BEGIN_COMMAND_MAP(DlgCrssvrTeamsCreateInput, CDlgBase)

AUI_ON_COMMAND("Btn_Confirm", OnCommand_OK)

AUI_END_COMMAND_MAP()



DlgCrssvrTeamsCreateInput::DlgCrssvrTeamsCreateInput()
{

}
DlgCrssvrTeamsCreateInput::~DlgCrssvrTeamsCreateInput()
{

}

bool DlgCrssvrTeamsCreateInput::OnInitDialog()
{
	m_pEdit = (PAUIEDITBOX*)GetDlgItem("DEFAULT_Txt_Input");
	return true;
}

void DlgCrssvrTeamsCreateInput::OnShowDialog()
{
	GetDlgItem("DEFAULT_Txt_Input")->SetText(_AL(""));
}

void DlgCrssvrTeamsCreateInput::OnCommand_OK(const char * szCommand)
{
	ACString szTemp = GetDlgItem("DEFAULT_Txt_Input")->GetText();
 	if(szTemp.IsEmpty())
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(20040), MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if(GetData() == 0)
	{
		GetGameSession()->c2s_CmdSendCrssvrTeamsRename(GetDlgItem("DEFAULT_Txt_Input")->GetText());
	}
	else if(GetData() == 1)
	{
		GetGameSession()->c2s_SendCreateCrossvrTeam(GetDlgItem("DEFAULT_Txt_Input")->GetText());
	}

	Show(false);
}


AUI_BEGIN_COMMAND_MAP(CDlgCrssvrTeamsOptionName, CDlgBase)

AUI_ON_COMMAND("Btn_Group", OnCommand_Group)
AUI_ON_COMMAND("Btn_Transfer", OnCommand_Transfer)
AUI_ON_COMMAND("Btn_Whisper", OnCommand_Whisper)
AUI_ON_COMMAND("Btn_Kickout", OnCommand_Kickout)

AUI_END_COMMAND_MAP()

CDlgCrssvrTeamsOptionName::CDlgCrssvrTeamsOptionName()
{

}
CDlgCrssvrTeamsOptionName::~CDlgCrssvrTeamsOptionName()
{

}

void CDlgCrssvrTeamsOptionName::OnCommand_Group(const char *szCommand)
{
	DlgCrssvrTeamsGroupInfo* pGroupInfo = (DlgCrssvrTeamsGroupInfo*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupInfo");
	PAUILISTBOX pGroupInfoList = pGroupInfo->m_pGroupInfoList;
	int nCurSel = pGroupInfoList->GetCurSel();
	TeamsGroupInfo* pData  = pGroupInfo->GetTeamsInfo(nCurSel);
	if(pData&& pData->roleid != GetHostPlayer()->GetCharacterID())
	{
		ACString strName = pData->name;
		if (strName[1]==_AL(' '))	// GT图标后面有一个空格
			strName.CutLeft(2);
		CECGameSession *pSession = GetGameSession();

		pSession->c2s_CmdTeamInvite(pData->roleid);
		GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(203), GP_CHAT_MISC);
	}
	Show(false);
}
void CDlgCrssvrTeamsOptionName::OnCommand_Transfer(const char *szCommand)
{
	DlgCrssvrTeamsGroupInfo* pGroupInfo = (DlgCrssvrTeamsGroupInfo*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupInfo");
	PAUILISTBOX pGroupInfoList = pGroupInfo->m_pGroupInfoList;
	int nCurSel = pGroupInfoList->GetCurSel();
	TeamsGroupInfo* pData  = pGroupInfo->GetTeamsInfo(nCurSel);
	if(pData)
	{
		PAUIDIALOG pDlg;
		GetGameUIMan()->MessageBox("CrssvrChangeCaptain",  GetStringFromTable(20112), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pDlg);
		pDlg->SetData(pData->roleid);
	}
}
void CDlgCrssvrTeamsOptionName::OnCommand_Whisper(const char *szCommand)
{
	DlgCrssvrTeamsGroupInfo* pGroupInfo = (DlgCrssvrTeamsGroupInfo*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupInfo");
	PAUILISTBOX pGroupInfoList = pGroupInfo->m_pGroupInfoList;
	int nCurSel = pGroupInfoList->GetCurSel();

	TeamsGroupInfo* pData  = pGroupInfo->GetTeamsInfo(nCurSel);
	if(pData)
	{
		ACString strName = pData->name;
		if (strName[1]==_AL(' '))	// GT图标后面有一个空格
			strName.CutLeft(2);

	GetGameUIMan()->SwitchToPrivateChat(strName);
	}
		Show(false);
}
void CDlgCrssvrTeamsOptionName::OnCommand_Kickout(const char *szCommand)
{
	DlgCrssvrTeamsGroupInfo* pGroupInfo = (DlgCrssvrTeamsGroupInfo*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupInfo");
	PAUILISTBOX pGroupInfoList = pGroupInfo->m_pGroupInfoList;
	int nCurSel = pGroupInfoList->GetCurSel();
	TeamsGroupInfo* pData  = pGroupInfo->GetTeamsInfo(nCurSel);
	if(pData && pData->roleid != GetHostPlayer()->GetCharacterID())
	{
		PAUIDIALOG pDlg;
		GetGameUIMan()->MessageBox("CrssvrKickTeamMenmber",  GetStringFromTable(20113), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pDlg);
		pDlg->SetData(pData->roleid);
	//	GetGameSession()->c2s_CmdSendCrssvrTeamsKick(pData->roleid);
	}
	Show(false);
}


AUI_BEGIN_COMMAND_MAP(DlgCrssvrTeamsGroupInfo, CDlgBase)

AUI_ON_COMMAND("Btn_Rank", OnCommand_Rank)
AUI_ON_COMMAND("Btn_Exit", OnCommand_Exit)
AUI_ON_COMMAND("Btn_ChangeGroupName", OnCommand_ChangeGroupName)
AUI_ON_COMMAND("Btn_Refresh", OnCommand_Refresh)
AUI_ON_COMMAND("Btn_Dismiss", OnCommand_Dismiss)
AUI_ON_COMMAND("Btn_Recruit", OnCommand_Recruit)
AUI_ON_COMMAND("Btn_CreateGroup", OnCommand_CreateGroup)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(DlgCrssvrTeamsGroupInfo, CDlgBase)

AUI_ON_EVENT("Lst_Info",	WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("Lst_Info",	WM_RBUTTONUP,		OnEventRButtonUp)

AUI_END_EVENT_MAP()

DlgCrssvrTeamsGroupInfo::DlgCrssvrTeamsGroupInfo()
{
	m_pImageStar  = NULL;
	m_nIconIndex = 0;
}
DlgCrssvrTeamsGroupInfo::~DlgCrssvrTeamsGroupInfo()
{
	A3DRELEASE(m_pImageStar)
}
bool DlgCrssvrTeamsGroupInfo::OnInitDialog()
{
	m_pTxt_GroupName = (AUILabel*)GetDlgItem("Lab_GroupName");
	m_pTxt_GroupScore= (AUILabel*)GetDlgItem("Txt_GroupScore");
	m_pLab_GroupScore = (AUILabel*)GetDlgItem("Lab_GroupScore");
	m_pTxt_FightTime= (AUILabel*)GetDlgItem("Txt_FightTime");
	m_pLab_FightTime= (AUILabel*)GetDlgItem("Lab_FightTime");
	m_pTxt_FightWin = (AUILabel*)GetDlgItem("Txt_FightWin");
	m_pLab_FightWin = (AUILabel*)GetDlgItem("Lab_FightWin");
	m_pTxt_FightLose = (AUILabel*)GetDlgItem("Txt_FightLose");
	m_pLab_FightLose = (AUILabel*)GetDlgItem("Lab_FightLose");
	m_pTxt_FightRate = (AUILabel*)GetDlgItem("Txt_FightRate");
	m_pLab_FightRate = (AUILabel*)GetDlgItem("Lab_FightRate");



	m_Btn_CreateGroup = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_CreateGroup");
	m_Btn_ChangeName = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_ChangeGroupName");
	m_Btn_Exit = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Exit");
	m_Btn_Recruit = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Recruit");
	m_Btn_Dismiss =  (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Dismiss");
	m_Btn_Refresh =  (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Refresh");

	
	m_pGroupInfoList = (PAUILISTBOX)GetDlgItem("Lst_Info");

	m_pTxt_GroupName->SetText(_AL(""));
	m_pTxt_GroupScore->SetText(_AL("0"));
	m_pTxt_FightTime->SetText(_AL("0"));
	m_pTxt_FightWin->SetText(_AL("0"));
	m_pTxt_FightLose->SetText(_AL("0"));
	m_pTxt_FightRate->SetText(_AL("0"));
	GetDlgItem("Txt_Num")->SetText(_AL("0/0"));
	m_vecImageList.clear();
	m_pImageStar = new A2DSprite;
	if( m_pImageStar )
	{
		bool bval = m_pImageStar->Init(m_pA3DDevice, "window\\leadermark.tga", 0);
		if( bval ) 
		{
			//GetGameUIMan()->AddImageListItem(m_pImageStar, m_nIconIndex);
			m_vecImageList.push_back(m_pImageStar);
		}

	}
	m_pGroupInfoList->SetImageList(&m_vecImageList);
	m_pGroupInfoList->SetItemMask(0xffffffff);
	//m_pGroupInfoList->SetImageList(GetGameUIMan()->GetControlImgLst());
	//m_pGroupInfoList->SetItemMask(0xffffffff);
	return true;
}

void DlgCrssvrTeamsGroupInfo::OnShowDialog()
{
	GetGameSession()->c2s_CmdSendCrssvrTeamsBase();
	GetGameSession()->c2s_CmdSendCrssvrTeamsGetRoleList();
	UpDateButtonShow();
}

void DlgCrssvrTeamsGroupInfo::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int nSel = m_pGroupInfoList->GetCurSel();
	if( nSel >= 0 && nSel < m_pGroupInfoList->GetCount() )
	{
		TeamsGroupInfo* pInfo = GetTeamsInfo(nSel);
		AUIOBJECT_SETPROPERTY p;
		if(pInfo && pInfo->roleid == GetHostPlayer()->GetCharacterID())
		{
			strcpy(p.fn,"frame\\6v6waitframe.tga");
			m_pGroupInfoList->SetProperty("Hilight Image",&p);
		}
		else
		{
			strcpy(p.fn,"Icon\\WordHover.tga");
			m_pGroupInfoList->SetProperty("Hilight Image",&p);
		}
	}
}

void DlgCrssvrTeamsGroupInfo::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	POINT ptPos = pObj->GetPos();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;

	CDlgCrssvrTeamsOptionName* pMenu  = (CDlgCrssvrTeamsOptionName*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_Option");
	x = GET_X_LPARAM(lParam) - p->X;
	y = GET_Y_LPARAM(lParam) - p->Y;
	int nRoleID = m_pGroupInfoList->GetItemData(m_pGroupInfoList->GetCurSel());
	if(nRoleID != -1)
	{
		if(nRoleID != GetHostPlayer()->GetCharacterID())
		{
			if(GetHostPlayer()->GetCharacterID() != GetHostPlayer()->GetCrssvrTeamsMaster())
			{
				EnableObject(pMenu, "Btn_Kickout", false);
				EnableObject(pMenu, "Btn_Transfer", false);
			}
			else
			{
				EnableObject(pMenu, "Btn_Kickout", true);
				EnableObject(pMenu, "Btn_Transfer", true);
			}
			// old : pMenu->SetPos(x, y);
			pMenu->SetPosEx(x, y);

			pMenu->Show(true);
		}
	}
}

void DlgCrssvrTeamsGroupInfo::EnableObject(PAUIDIALOG pDlg, const char *pszCommand, bool bEnable)
{
	PAUIOBJECT pObj = pDlg->GetDlgItem(pszCommand);
	if (pObj)
		pObj->Enable(bEnable);
	else
		assert(0);
}

void DlgCrssvrTeamsGroupInfo::OnCommand_Rank(const char * szCommand)
{
	DlgCrssvrTeamsMain*pDlg = (DlgCrssvrTeamsMain*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupMain");
	pDlg->SetWindowsType(0);
	GetGameSession()->toptable_get(96, 0, true);
}
void DlgCrssvrTeamsGroupInfo::OnCommand_Exit(const char * szCommand)
{
	PAUIDIALOG pDlg;
	GetGameUIMan()->MessageBox("CrssvrExitTeams",  GetStringFromTable(20115), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pDlg);
//	GetGameSession()->c2s_CmdSendCrssvrTeamsLeave();
}
void DlgCrssvrTeamsGroupInfo::OnCommand_ChangeGroupName(const char * szCommand)
{
	PAUIDIALOG	pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_InputGroupName");
	pDlg->SetData(0);
	pDlg->Show(true);
	//GetGameSession()->c2s_CmdSendCrssvrTeamsRename(GetHostPlayer()->GetCrssvrTeamsId())
}
void DlgCrssvrTeamsGroupInfo::OnCommand_Refresh(const char * szCommand)
{
	GetGameSession()->c2s_CmdSendCrssvrTeamsBase();
	GetGameSession()->c2s_CmdSendCrssvrTeamsGetRoleList();
}

void DlgCrssvrTeamsGroupInfo::OnCommand_Dismiss(const char * szCommand)
{
	GetGameUIMan()->MessageBox("CrssvrDismissTeams", GetStringFromTable(20114), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
	//GetGameSession()->c2s_CmdSendCrssvrTeamsDismiss();
}

void DlgCrssvrTeamsGroupInfo::OnCommand_Recruit(const char * szCommand)
{
	CDlgInputName* pDlg = GetGameUIMan()->m_pDlgInputName;
	pDlg->m_pTxtInput->SetText(_AL(""));
	pDlg->SetData(CDlgInputName::INPUTNAME_CRSSVRTEAMES);
	pDlg->Show(true);
}

void DlgCrssvrTeamsGroupInfo::OnCommand_CreateGroup(const char * szCommand)
{
	if(	GetHostPlayer()->GetBasicProps().iLevel < 135 && GetHostPlayer()->GetRebornCount() <=  1)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20116), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	PAUIDIALOG	pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_InputGroupName");
	pDlg->SetData(1);
	pDlg->Show(true);
}

void DlgCrssvrTeamsGroupInfo::AddTeamsGroupInfo( void * pInfo ) 
{
	GCrssvrTeamsRole* pData = (GCrssvrTeamsRole*)pInfo;

	//int roleid;
	//Octets name;
	//unsigned short level;
	//unsigned char occupation;
	//int score;
	//char camp;
	//char isonline;
	//int reserved1;
	//int reserved2;
	//int reserved3;
	//int reserved4;
	//int reserved5;
	ACString strName = ACString((const ACHAR*)pData->name.begin(), pData->name.size() / sizeof (ACHAR));
	TeamsGroupInfo pElement;
	pElement.roleid = pData->roleid;
	pElement.name = strName;
	pElement.level = pData->level;
	pElement.occupation = pData->occupation;
	pElement.score = pData->score;
	pElement.camp = pData->camp;
	pElement.isonline = pData->isonline;
	pElement.reserved1 = pData->reserved1;
	pElement.reserved2 = pData->reserved2;
	pElement.reserved3 = pData->reserved3;
	pElement.reserved4 = pData->reserved4;
	pElement.reserved5 = pData->reserved5;
	m_vecTeamsInfo.push_back(pElement);
	if(pElement.roleid  == GetHostPlayer()->GetCharacterID())
	{
		DlgCrssvrTeamsMain*	pDlg = (DlgCrssvrTeamsMain*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupMain");
		pDlg->UpDataSlefScore(pElement.score);
	}
}


void DlgCrssvrTeamsGroupInfo::UpDateGroupInfoList(void* pData)
{
	m_vecTeamsInfo.clear();
	m_pGroupInfoList->ResetContent();
	GNET::CrssvrTeamsRoleList_Re* p = (GNET::CrssvrTeamsRoleList_Re*)pData;
	int nSize = p->members.size();
	for(int nIndex = 0; nIndex < nSize; nIndex++)
	{
		GCrssvrTeamsRole* pInfo = &p->members[nIndex];
		if(pInfo->roleid == GetHostPlayer()->GetCrssvrTeamsMaster())
		{
			AddTeamsGroupInfo(pInfo);
		}
	}
	for(int nIndex = 0; nIndex < nSize; nIndex++)
	{
		GCrssvrTeamsRole* pInfo = &p->members[nIndex];
		if(pInfo->roleid != GetHostPlayer()->GetCrssvrTeamsMaster())
		{
			AddTeamsGroupInfo(pInfo);
		}
	}
	BubbleSort(m_vecTeamsInfo.begin(), m_vecTeamsInfo.end(), SortCrssvrTeamsInfo(GetHostPlayer()->GetCrssvrTeamsMaster()));
	//bool bExchanged;
	//TeamsGroupInfo *pTemp;

	//do
	//{
	//	bExchanged = false;
	//	for(int  i = 1; i < (int)m_vecTeamsInfo.size() - 1; i++ )
	//	{
	//		if(m_vecTeamsInfo[i].level> m_vecTeamsInfo[i+1].level && m_vecTeamsInfo[i].isonline == 0)
	//		{
	//			pTemp = &m_vecTeamsInfo[i];
	//			m_vecTeamsInfo[i] = m_vecTeamsInfo[i + 1];
	//			m_vecTeamsInfo[i + 1] = *pTemp;
	//			bExchanged = true;
	//		}
	//		else if(m_vecTeamsInfo[i].level == m_vecTeamsInfo[i+1].level)
	//		{
	//			if(m_vecTeamsInfo[i].score > m_vecTeamsInfo[i+1].score)
	//			{
	//				pTemp = &m_vecTeamsInfo[i];
	//				m_vecTeamsInfo[i] = m_vecTeamsInfo[i + 1];
	//				m_vecTeamsInfo[i + 1] = *pTemp;
	//				bExchanged = true;
	//			}
	//			//else if(m_vecTeamsInfo[i].score == m_vecTeamsInfo[i+1].score)
	//			//{
	//			//	if(m_vecTeamsInfo[i].isonline == 1)
	//			//	{
	//			//		pTemp = &m_vecTeamsInfo[i];
	//			//		m_vecTeamsInfo[i] = m_vecTeamsInfo[i + 1];
	//			//		m_vecTeamsInfo[i + 1] = *pTemp;
	//			//		bExchanged = true;
	//			//	}
	//			//}
	//		}
	//	}
	//}
	//while( bExchanged );
	ACString strIcon;
	nSize = m_vecTeamsInfo.size();
	int nOnline = 0;
	for(int nIndex = 0; nIndex < nSize; nIndex++)
	{
		//int roleid;
		//Octets name;
		//unsigned short level;
		//unsigned char occupation;
		//int score;
		//char camp;
		//char isonline;
		//int reserved1;
		//int reserved2;
		//int reserved3;
		//int reserved4;
		//int reserved5;
		TeamsGroupInfo* pInfo = &m_vecTeamsInfo[nIndex];

		ACString strText;
		ACString szOnLine,szCamp;
		if(pInfo->isonline == 1) // 1原服 2跨服 0 不在线
		{
			szOnLine = GetStringFromTable(20041);
			nOnline++;
		}
		else if(pInfo->isonline == 2) // 1原服 2跨服 0 不在线
		{
			szOnLine = GetStringFromTable(20042);
			nOnline++;
		}
		else if(pInfo->isonline == 0)
		{
			szOnLine = GetStringFromTable(20043);
		}
		//else 
		//{
		//	szOnLine = GetStringFromTable(20030);
		//}
		if( pInfo->roleid == GetHostPlayer()->GetCrssvrTeamsMaster())
		{
			EditBoxItemBase itemImage(enumEIImage);
			itemImage.SetImageIndex(0);
			itemImage.SetImageFrame(0);
			strIcon = (ACHAR)AUICOMMON_ITEM_CODE_START + itemImage.Serialize();
		}
		else
		{
			strIcon = _AL(" ");
		}
		//队长图标 名字 等级 门派 个人积分 在线状态 阵营
		strText.Format(_AL("%s\t%s\t%d\t%s\t%d\t%s\t%s"), strIcon,pInfo->name,
			pInfo->level, GetStringFromTable(PROFESSION_START + pInfo->occupation), pInfo->score,szOnLine,GetStringFromTable(7000+pInfo->camp));
		m_pGroupInfoList->AddString(strText);
		m_pGroupInfoList->SetItemData(m_pGroupInfoList->GetCount() - 1,  pInfo->roleid);
	}
	strIcon.Format(_AL("%d/%d"),nOnline,nSize);
	GetDlgItem("Txt_Num")->SetText(strIcon);
	UpDateButtonShow();
}

void DlgCrssvrTeamsGroupInfo::UpDateGroupInfoBase(void* pData)
{
	//int retcode;
	//int teamid;
	//int master;
	//Octets name;
	//int score;
	//int battle_losses ;
	//int battle_draws ;
	//int battle_wins ;

	GNET::CrssvrTeamsTeamBase_Re* p = (GNET::CrssvrTeamsTeamBase_Re*)pData;
	ACString strName((const ACHAR*)p->name.begin(), p->name.size()/sizeof(ACHAR));
	m_pTxt_GroupName->SetText(strName);
	GetHostPlayer()->SetCrssvrTeamsMaster(p->master);
	GetHostPlayer()->SetCrssvrTeamsId(p->teamid);
	ACString szTemp;
	szTemp.Format(_AL("%d"),p->score);
	m_pTxt_GroupScore->SetText(szTemp);

	szTemp.Format(_AL("%d"),p->battle_wins);
	m_pTxt_FightWin->SetText(szTemp);

	szTemp.Format(_AL("%d"),p->battle_losses);
	m_pTxt_FightLose->SetText(szTemp);

	int nTemp  = 0;
	if(p->battle_wins != 0)
	{
		nTemp = int(((float)p->battle_wins / (float)(p->battle_wins + p->battle_losses + p->battle_draws))* 100);
		szTemp.Format(_AL("%d"),nTemp);
		szTemp +=_AL("%");
	}
	else
	{
		szTemp = _AL("0%");
	}
	m_pTxt_FightRate->SetText(szTemp);

	UpDateButtonShow();
}


void DlgCrssvrTeamsGroupInfo::UpDateButtonShow() 
{
	m_Btn_Exit->Show(false);
	m_Btn_Recruit->Show(false);
	m_Btn_CreateGroup->Show(false);
	m_Btn_ChangeName->Show(false);
	m_Btn_Dismiss->Show(false);
	m_Btn_ChangeName->Enable(true);
	if(GetHostPlayer()->GetCrssvrTeamsId() != -1)	// 判断自己是否有队伍. 这个判断暂时不对
	{
		if(GetHostPlayer()->GetCrssvrTeamsMaster() == GetHostPlayer()->GetCharacterID())
		{
			m_Btn_ChangeName->Show(true);
			m_Btn_Dismiss->Show(true);
			m_Btn_Recruit->Show(true);
			m_Btn_Recruit->Show(true);
		}
		else
		{
			m_Btn_ChangeName->Show(true);
			m_Btn_ChangeName->Enable(false);
			m_Btn_Exit->Show(true);
		}
		m_Btn_Refresh->Show(true);
	}
	else
	{
		m_Btn_CreateGroup->Show(true);
	}
	if(GetHostPlayer()->GetSrcServerID() != 0)
	{	
		m_Btn_Exit->Enable(false);
		m_Btn_Recruit->Enable(false);
		m_Btn_CreateGroup->Enable(false);
		m_Btn_ChangeName->Enable(false);
		m_Btn_Dismiss->Enable(false);
		m_Btn_ChangeName->Enable(false);
		m_Btn_Refresh->Enable(true);
	}
}

void DlgCrssvrTeamsGroupInfo::ClearData()
{
	m_pTxt_GroupName->SetText(_AL(""));
	m_pTxt_GroupScore->SetText(_AL("0"));
	m_pTxt_FightTime->SetText(_AL("0"));
	m_pTxt_FightWin->SetText(_AL("0"));
	m_pTxt_FightLose->SetText(_AL("0"));
	m_pTxt_FightRate->SetText(_AL("0"));
	GetDlgItem("Txt_Num")->SetText(_AL("0/0"));
	m_vecTeamsInfo.clear();
	m_pGroupInfoList->ResetContent();
	GetHostPlayer()->SetCrssvrTeamsId(-1);
	GetHostPlayer()->SetCrssvrTeamsMaster(-1);
	UpDateButtonShow();
}

TeamsGroupInfo* DlgCrssvrTeamsGroupInfo::GetTeamsInfo(int nIndex)
{
	if(nIndex < (int)m_vecTeamsInfo.size())
	{
		return &m_vecTeamsInfo[nIndex];
	}
	return NULL;
}

AUI_BEGIN_COMMAND_MAP(DlgCrssvrTeamsGroupRank, CDlgBase)

AUI_ON_COMMAND("Btn_Search", OnCommand_Search)
AUI_ON_COMMAND("Btn_Prev", OnCommand_Prev)
AUI_ON_COMMAND("Btn_Next", OnCommand_Next)

AUI_END_COMMAND_MAP()


DlgCrssvrTeamsGroupRank::DlgCrssvrTeamsGroupRank()
{
}
DlgCrssvrTeamsGroupRank::~DlgCrssvrTeamsGroupRank()
{

}
bool DlgCrssvrTeamsGroupRank::OnInitDialog()
{
	m_pTxt_GroupRank = (AUILabel*)GetDlgItem("Txt_GroupRank");
	m_pTxt_Page= (AUILabel*)GetDlgItem("Txt_Page");
	
	m_pTxt_Page->SetText(_AL("1 / 1"));
	m_pTxt_GroupRank->SetText(_AL(""));
	
	m_nCurPage = 1;
	m_nMaxPage = 1;
	return true;
}

void DlgCrssvrTeamsGroupRank::OnShowDialog()
{

}
void DlgCrssvrTeamsGroupRank::OnCommand_Search(const char * szCommand)
{
	ACString szTemp = GetDlgItem("DEFAULT_Txt_Input")->GetText();
	if(szTemp.IsEmpty())
	{
		GetGameSession()->toptable_get(96, 0, true);
		return;
	}
	GetGameSession()->c2s_CmdSendCrssvrSearch(szTemp);
	GetDlgItem("DEFAULT_Txt_Input")->SetText(_AL(""));
}

void DlgCrssvrTeamsGroupRank::OnCommand_Prev(const char * szCommand)
{
	m_nCurPage--;
	if(m_nCurPage < 0)
	{
		m_nCurPage = 0;
		return;
	}
	GetGameSession()->toptable_get(96, m_nCurPage,true);
}
void DlgCrssvrTeamsGroupRank::OnCommand_Next(const char * szCommand)
{
	m_nCurPage++;
	if(m_nCurPage >= m_nMaxPage)
	{
		m_nCurPage = m_nMaxPage;
		return;
	}
	GetGameSession()->toptable_get(96, m_nCurPage, true);
}

void DlgCrssvrTeamsGroupRank::UpDateGroupRank(void* pData)
{
	GNET::GetTopTable_Re* pInfo = static_cast<GNET::GetTopTable_Re*>(pData);
	int nMax =  pInfo->maxsize;
	m_nCurPage = pInfo->pageid;
	m_nMaxPage = nMax / 20;
	if(m_nMaxPage == 0)
	{
		m_nMaxPage = 1;
	}

	DlgCrssvrTeamsSub *pDlgMain = (DlgCrssvrTeamsSub*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupRankSub");
	pDlgMain->UpDateCrssveInfo(pData,m_nCurPage);
	
	ACString szTemp;
	if(pInfo->selfrank == -1)
	{
		szTemp = _AL("--");
	}
	else
	{
		szTemp.Format(_AL("%d"), pInfo->selfrank);
	}
	m_pTxt_GroupRank->SetText(szTemp);
	UpDatePageText();
}

void DlgCrssvrTeamsGroupRank::UpDateGroupSearchRank(void* pData)
{
	m_nCurPage = 1;
	m_nMaxPage = 1;
	DlgCrssvrTeamsSub *pDlgMain = (DlgCrssvrTeamsSub*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupRankSub");
	pDlgMain->UpDateCrssveInfo(pData,m_nCurPage);
}

void DlgCrssvrTeamsGroupRank::UpDatePageText()
{
	ACString szTemp;
	szTemp.Format(_AL("%d / %d"),m_nCurPage + 1,m_nMaxPage);
	m_pTxt_Page->SetText(szTemp);
}

AUI_BEGIN_COMMAND_MAP(DlgCrssvrTeamsApply, CDlgBase)
AUI_END_COMMAND_MAP()

DlgCrssvrTeamsApply::DlgCrssvrTeamsApply()
{
	for(int nIndex = 0; nIndex < CRSSVRTEAMSMEMBER; nIndex++)
	{
			m_pPlayerA[nIndex] = NULL;
			m_pPlayerB[nIndex] = NULL;
	}
}
DlgCrssvrTeamsApply::~DlgCrssvrTeamsApply()
{
	for(int nIndex = 0; nIndex < CRSSVRTEAMSMEMBER; nIndex++)
	{	
		A3DRELEASE(m_pPlayerA[nIndex]);
		m_pPlayerA[nIndex] = NULL;
		A3DRELEASE(	m_pPlayerB[nIndex]);
		m_pPlayerB[nIndex] = NULL;
	}
}

bool DlgCrssvrTeamsApply::OnInitDialog()
{

	AString strName;
	for(int nIndex = 0; nIndex < CRSSVRTEAMSMEMBER; nIndex++)
	{
		strName.Format("Lab_NameA0%d", nIndex+1);
		m_pTxt_PlayerNameA_A[nIndex] = (AUILabel*)GetDlgItem(strName);
		m_pTxt_PlayerNameA_A[nIndex]->SetText(_AL(""));

		strName.Format("Txt_ProfA0%d", nIndex+1);
		m_pTxt_ProfA[nIndex] = (PAUITEXTAREA)GetDlgItem(strName);
		m_pTxt_ProfA[nIndex]->SetText(_AL(""));

		strName.Format("Img_CharA0%d", nIndex+1);
		m_pImg_IconA_A[nIndex] = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		if( m_pImg_IconA_A[nIndex] )
			m_pImg_IconA_A[nIndex]->SetAcceptMouseMessage(true);

		strName.Format("Lab_NameB0%d", nIndex+1);
		m_pTxt_PlayerNameB_A[nIndex]= (AUILabel*)GetDlgItem(strName);
		m_pTxt_PlayerNameB_A[nIndex]->SetText(_AL(""));

		strName.Format("Txt_ProfB0%d", nIndex+1);
		m_pTxt_ProfB[nIndex] = (PAUITEXTAREA)GetDlgItem(strName);
		m_pTxt_ProfB[nIndex]->SetText(_AL(""));

		strName.Format("Img_CharB0%d", nIndex+1);
		m_pImg_IconB_A[nIndex]= (PAUIIMAGEPICTURE)GetDlgItem(strName);
		if( m_pImg_IconB_A[nIndex] )
			m_pImg_IconB_A[nIndex]->SetAcceptMouseMessage(true);
	}
	m_pTxt_TeamNameA = (AUILabel*)GetDlgItem("Lab_Team01");
	m_pTxt_PlayerRankA= (AUILabel*)GetDlgItem("Txt_RankA");
	m_pTxt_PlayerScoreA= (AUILabel*)GetDlgItem("Txt_ScoreA");
	m_pTxt_PlayerFightA= (AUILabel*)GetDlgItem("Txt_FightA");


	m_pTxt_TeamNameB = (AUILabel*)GetDlgItem("Lab_Team02");

	m_pTxt_PlayerRankB= (AUILabel*)GetDlgItem("Txt_RankB");
	m_pTxt_PlayerScoreB= (AUILabel*)GetDlgItem("Txt_ScoreB");
	m_pTxt_PlayerFightB= (AUILabel*)GetDlgItem("Txt_FightB");

	return true;
}

void DlgCrssvrTeamsApply::UpdateCrssvrTeamsApplyInfo(void* pData)
{
	for(int nIndex = 0; nIndex < CRSSVRTEAMSMEMBER; nIndex++)
	{
		m_pTxt_PlayerNameA_A[nIndex]->SetText(_AL(""));
		m_pTxt_ProfA[nIndex]->SetText(_AL(""));
		m_pTxt_PlayerNameB_A[nIndex]->SetText(_AL(""));
		m_pTxt_ProfB[nIndex]->SetText(_AL(""));
		m_pImg_IconB_A[nIndex]->SetData(-1,"roleId");
		m_pImg_IconA_A[nIndex]->SetData(-1,"roleId");
	}

	m_pTxt_TeamNameA->SetText(_AL(""));
	m_pTxt_PlayerRankA->SetText(_AL(""));
	m_pTxt_PlayerScoreA->SetText(_AL(""));
	m_pTxt_PlayerFightA->SetText(_AL(""));


	m_pTxt_TeamNameB->SetText(_AL(""));

	m_pTxt_PlayerRankB->SetText(_AL(""));
	m_pTxt_PlayerScoreB->SetText(_AL(""));
	m_pTxt_PlayerFightB->SetText(_AL(""));

	TeamRaidMappingSuccess* p = (TeamRaidMappingSuccess*)pData;
	if(p->teamdata_a.info.teamid == GetHostPlayer()->GetCrssvrTeamsId())
	{
		ACString szText;
		m_pTxt_TeamNameA->SetText(ACString((const ACHAR*)(p->teamdata_a.info.name.begin()),p->teamdata_a.info.name.size()/sizeof(ACHAR)));
		szText.Format(_AL("%d"),	p->a_ranking);
		m_pTxt_PlayerRankA->SetText(szText);
		szText.Format(_AL("%d"),	p->teamdata_a.info.score);
		m_pTxt_PlayerScoreA->SetText(szText);
		m_pTxt_PlayerFightA->SetText(szText);
		for(int nIndex = 0 ; nIndex < (int)p->teamdata_a.roles.size(); nIndex++)
		{
			GCrssvrTeamsRole* pRoleInfo = &p->teamdata_a.roles[nIndex];
			m_pTxt_PlayerNameA_A[nIndex]->SetText(ACString((const ACHAR*)(pRoleInfo->name.begin()),pRoleInfo->name.size()/sizeof(ACHAR)));
			szText.Format(_AL("%s %s %d"), GetStringFromTable(7001+pRoleInfo->camp),GetStringFromTable(PROFESSION_START +  pRoleInfo->occupation),pRoleInfo->level);
			m_pTxt_ProfA[nIndex]->SetText(szText);
		}

		for(int nIndex = 0; nIndex < (int)p->teamdata_a_detail.size(); nIndex++)
		{
			TeamRaidRoleBrief* pData = &p->teamdata_a_detail[nIndex];
			m_pPlayerA[nIndex] = new CECLoginPlayer(GetGame()->GetGameRun()->GetWorld()->GetPlayerMan());
			const GNET::RoleInfo& info = GetGame()->GetGameRun()->GetSelectedRoleInfo();
			if (!m_pPlayerA[nIndex]->Load(CECPlayer::GetPhysiqueByProf(pData->occup),pData->occup, pData->faceid,pData->hairid,pData->gender, NULL))
			{
				m_pPlayerA[nIndex]->Release();
				delete m_pPlayerA[nIndex];
				m_pPlayerA[nIndex] = 0;
				return;
			}
			m_pPlayerA[nIndex]->SetPos(A3DVECTOR3(0));
			m_pPlayerA[nIndex]->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
			m_pPlayerA[nIndex]->GetPlayerModel()->SetAutoUpdateFlag(false);

			if( m_pPlayerA[nIndex]->IsAllResReady() )
			{
				m_pImg_IconA_A[nIndex]->SetRenderCallback(ApplyRenderCallback,
					(DWORD)m_pPlayerA[nIndex], 0,0);
				m_pImg_IconA_A[nIndex]->SetData(pData->roleid,"roleId");
			}
			else
				m_pImg_IconA_A[nIndex]->SetRenderCallback(NULL, 0);

		}

		for(int nIndex = 0 ; nIndex < (int)p->teamdata_b.roles.size(); nIndex++)
		{
			GCrssvrTeamsRole* pRoleInfo = &p->teamdata_b.roles[nIndex];
			m_pTxt_PlayerNameB_A[nIndex]->SetText(ACString((const ACHAR*)(pRoleInfo->name.begin()),pRoleInfo->name.size()/sizeof(ACHAR)));

			szText.Format(_AL("%s %s %d"), GetStringFromTable(7001+pRoleInfo->camp),GetStringFromTable(PROFESSION_START +  pRoleInfo->occupation),pRoleInfo->level);
			m_pTxt_ProfB[nIndex]->SetText(szText);
		}

		for(int nIndex = 0; nIndex < (int)p->teamdata_b_detail.size(); nIndex++)
		{
			TeamRaidRoleBrief* pData = &p->teamdata_b_detail[nIndex];
			m_pPlayerB[nIndex] = new CECLoginPlayer(GetGame()->GetGameRun()->GetWorld()->GetPlayerMan());
			const GNET::RoleInfo& info = GetGame()->GetGameRun()->GetSelectedRoleInfo();
			if (!m_pPlayerB[nIndex]->Load(CECPlayer::GetPhysiqueByProf(pData->occup),pData->occup, pData->faceid,pData->hairid,pData->gender, NULL))
			{
				m_pPlayerB[nIndex]->Release();
				delete m_pPlayerB[nIndex];
				m_pPlayerB[nIndex] = 0;
				return;
			}
			m_pPlayerB[nIndex]->SetPos(A3DVECTOR3(0));
			m_pPlayerB[nIndex]->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
			m_pPlayerB[nIndex]->GetPlayerModel()->SetAutoUpdateFlag(false);


			if( m_pPlayerB[nIndex]->IsAllResReady() )
			{
				m_pImg_IconB_A[nIndex]->SetRenderCallback(ApplyRenderCallback,
					(DWORD)m_pPlayerB[nIndex], 0);
				m_pImg_IconB_A[nIndex]->SetData(pData->roleid,"roleId");
			}
			else
				m_pImg_IconB_A[nIndex]->SetRenderCallback(NULL, 0);
		}
		m_pTxt_TeamNameB->SetText(ACString((const ACHAR*)(p->teamdata_b.info.name.begin()),p->teamdata_b.info.name.size()/sizeof(ACHAR)));
		szText.Format(_AL("%d"),	p->b_ranking);
		m_pTxt_PlayerRankB->SetText(szText);
		szText.Format(_AL("%d"),	p->teamdata_b.info.score);
		m_pTxt_PlayerScoreB->SetText(szText);
		m_pTxt_PlayerFightB->SetText(szText);
	}
	else if(p->teamdata_b.info.teamid == GetHostPlayer()->GetCrssvrTeamsId())
	{
		ACString szText;
		m_pTxt_TeamNameB->SetText(ACString((const ACHAR*)(p->teamdata_a.info.name.begin()),p->teamdata_a.info.name.size()/sizeof(ACHAR)));
		szText.Format(_AL("%d"),	p->a_ranking);
		m_pTxt_PlayerRankB->SetText(szText);
		szText.Format(_AL("%d"),	p->teamdata_a.info.score);
		m_pTxt_PlayerScoreB->SetText(szText);
		m_pTxt_PlayerFightB->SetText(szText);
		for(int nIndex = 0 ; nIndex < (int)p->teamdata_a.roles.size(); nIndex++)
		{
			GCrssvrTeamsRole* pRoleInfo = &p->teamdata_a.roles[nIndex];
			m_pTxt_PlayerNameB_A[nIndex]->SetText(ACString((const ACHAR*)(pRoleInfo->name.begin()),pRoleInfo->name.size()/sizeof(ACHAR)));
			szText.Format(_AL("%s %s %d"), GetStringFromTable(7001+pRoleInfo->camp),GetStringFromTable(PROFESSION_START +  pRoleInfo->occupation),pRoleInfo->level);
			m_pTxt_ProfB[nIndex]->SetText(szText);
		}

		for(int nIndex = 0; nIndex < (int)p->teamdata_a_detail.size(); nIndex++)
		{
			TeamRaidRoleBrief* pData = &p->teamdata_a_detail[nIndex];
			m_pPlayerB[nIndex] = new CECLoginPlayer(GetGame()->GetGameRun()->GetWorld()->GetPlayerMan());
			const GNET::RoleInfo& info = GetGame()->GetGameRun()->GetSelectedRoleInfo();
			if (!m_pPlayerB[nIndex]->Load(CECPlayer::GetPhysiqueByProf(pData->occup),pData->occup, pData->faceid,pData->hairid,pData->gender, NULL))
			{
				m_pPlayerB[nIndex]->Release();
				delete m_pPlayerB[nIndex];
				m_pPlayerB[nIndex] = 0;
				return;
			}
			m_pPlayerB[nIndex]->SetPos(A3DVECTOR3(0));
			m_pPlayerB[nIndex]->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
			m_pPlayerB[nIndex]->GetPlayerModel()->SetAutoUpdateFlag(false);

			if( m_pPlayerB[nIndex]->IsAllResReady() )
			{
				m_pImg_IconB_A[nIndex]->SetRenderCallback(ApplyRenderCallback,
					(DWORD)m_pPlayerB[nIndex], 0,0);
				m_pImg_IconB_A[nIndex]->SetData(pData->roleid,"roleId");
			}
			else
				m_pImg_IconB_A[nIndex]->SetRenderCallback(NULL, 0);

		}

		for(int nIndex = 0 ; nIndex < (int)p->teamdata_b.roles.size(); nIndex++)
		{
			GCrssvrTeamsRole* pRoleInfo = &p->teamdata_b.roles[nIndex];
			m_pTxt_PlayerNameA_A[nIndex]->SetText(ACString((const ACHAR*)(pRoleInfo->name.begin()),pRoleInfo->name.size()/sizeof(ACHAR)));

			szText.Format(_AL("%s %s %d"), GetStringFromTable(7001+pRoleInfo->camp),GetStringFromTable(PROFESSION_START +  pRoleInfo->occupation),pRoleInfo->level);
			m_pTxt_ProfA[nIndex]->SetText(szText);
		}

		for(int nIndex = 0; nIndex < (int)p->teamdata_b_detail.size(); nIndex++)
		{
			TeamRaidRoleBrief* pData = &p->teamdata_b_detail[nIndex];
			m_pPlayerA[nIndex] = new CECLoginPlayer(GetGame()->GetGameRun()->GetWorld()->GetPlayerMan());
			const GNET::RoleInfo& info = GetGame()->GetGameRun()->GetSelectedRoleInfo();
			if (!m_pPlayerA[nIndex]->Load(CECPlayer::GetPhysiqueByProf(pData->occup),pData->occup, pData->faceid,pData->hairid,pData->gender, NULL))
			{
				m_pPlayerA[nIndex]->Release();
				delete m_pPlayerA[nIndex];
				m_pPlayerA[nIndex] = 0;
				return;
			}
			m_pPlayerA[nIndex]->SetPos(A3DVECTOR3(0));
			m_pPlayerA[nIndex]->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
			m_pPlayerA[nIndex]->GetPlayerModel()->SetAutoUpdateFlag(false);


			if( m_pPlayerA[nIndex]->IsAllResReady() )
			{
				m_pImg_IconA_A[nIndex]->SetRenderCallback(ApplyRenderCallback,
					(DWORD)m_pPlayerA[nIndex], 0);
				m_pImg_IconA_A[nIndex]->SetData(pData->roleid,"roleId");
			}
			else
				m_pImg_IconA_A[nIndex]->SetRenderCallback(NULL, 0);
		}
		m_pTxt_TeamNameA->SetText(ACString((const ACHAR*)(p->teamdata_b.info.name.begin()),p->teamdata_b.info.name.size()/sizeof(ACHAR)));
		szText.Format(_AL("%d"),	p->b_ranking);
		m_pTxt_PlayerRankA->SetText(szText);
		szText.Format(_AL("%d"),	p->teamdata_b.info.score);
		m_pTxt_PlayerScoreA->SetText(szText);
		m_pTxt_PlayerFightA->SetText(szText);
	}
}

void DlgCrssvrTeamsApply::UpdateQueueUpState(int nRoleId)
{
	for(int nIndex = 0; nIndex < CRSSVRTEAMSMEMBER; nIndex++)
	{
		int nId =m_pImg_IconA_A[nIndex]->GetData("roleId");
		if(nId == nRoleId)
		{
			m_pImg_IconA_A[nIndex]->SetRenderCallback(ApplyRenderCallback,
				(DWORD)m_pPlayerA[nIndex], true);
			break;
		}
		nId =m_pImg_IconB_A[nIndex]->GetData("roleId");
		if(nId == nRoleId)
		{
			m_pImg_IconB_A[nIndex]->SetRenderCallback(ApplyRenderCallback,
				(DWORD)m_pPlayerB[nIndex], true);
			break;
		}
	}
}

void DlgCrssvrTeamsApply::ApplyRenderCallback(const A3DRECT &rc, DWORD param1, DWORD param2,DWORD param3)
{
	static const float _dist[2] = { 0.79f, 0.7f };
	static const float _height[2] = { 0.08f, 0.08f };

	if( param1 == 0 )
		return;

	CECPlayer * pPlayer = (CECPlayer *) param1;
	int 		isOk = (int)param2 ;

	A3DViewport * pCurViewport = g_pGame->GetA3DEngine()->GetActiveViewport();

	A3DViewport * pPortraitViewport = g_pGame->GetPortraitRender()->GetPortraitViewport();
	A3DCameraBase * pCamera = pPortraitViewport->GetCamera();

	A3DVIEWPORTPARAM viewParam = *pPortraitViewport->GetParam();
	viewParam.X = rc.left;
	viewParam.Y = rc.top;
	viewParam.Width = rc.right - rc.left;
	viewParam.Height = rc.bottom - rc.top;
	pPortraitViewport->SetParam(&viewParam);
	((A3DCamera *) pCamera)->SetProjectionParam(-1.0f, -1.0f, -1.0f, ((float)viewParam.Width) / viewParam.Height);
	A3DSkinModel * pFaceModel = pPlayer->GetPortraitModel();
	pFaceModel->Update(g_pGame->GetTickTime());
	A3DBone * pBoneUp;
	if(pPlayer->GetPhysique() == PHYSIQUE_TETRAPOD)  //四组体型的模型有 有两套骨骼  其他位置也有引用Bip01 Head的地方  暂时这样处理 等美术修改名字
		pBoneUp = pFaceModel->GetSkeleton()->GetBone("Bip02 Head", NULL);
	else
		pBoneUp = pFaceModel->GetSkeleton()->GetBone("Bip01 Head", NULL);
	A3DMATRIX4 mat = pFaceModel->GetAbsoluteTM();
	mat.SetRow(3, pBoneUp->GetAbsoluteTM().GetRow(3));

	A3DMATRIX4 matHead = a3d_RotateY(0) * mat;
	A3DVECTOR3 vecPos = matHead.GetRow(3);
	A3DVECTOR3 vecDir = Normalize(matHead.GetRow(2));
	A3DVECTOR3 vecUp = Normalize(matHead.GetRow(1));

	int index = pPlayer->GetGender();
	pCamera->SetPos(vecPos + vecDir * _dist[index] + vecUp * _height[index]);
	pCamera->SetDirAndUp(-vecDir, vecUp);
	pPortraitViewport->Active();
	pPortraitViewport->ClearDevice();

	A3DSkinModel::LIGHTINFO LightInfo;
	LightInfo.colAmbient	= A3DCOLORVALUE(0.8f, 0.8f, 0.8f, 1.0f);
	LightInfo.vLightDir		= Normalize(A3DVECTOR3(-1.0f, -1.0f, -1.0f));
	LightInfo.colDirDiff	= A3DCOLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.bPtLight		= false;

	pFaceModel->SetLightInfo(LightInfo);
	A3DEnvironment::DIRLIGHT dirLightParams = *(g_pGame->GetA3DEngine()->GetA3DEnvironment()->GetDirLightParam());
	if(isOk)
	{

		dirLightParams.Ambient0 = A3DCOLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		dirLightParams.DirPower = 1.f;//m_Lighting.fSunPower * d + m_NightLighting.fSunPower * n;
		g_pGame->GetA3DEngine()->GetA3DEnvironment()->SetDirLightParam(&dirLightParams);
	}
	else
	{
		dirLightParams.Ambient0 = A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
		dirLightParams.DirPower = 1.f;//m_Lighting.fSunPower * d + m_NightLighting.fSunPower * n;
		g_pGame->GetA3DEngine()->GetA3DEnvironment()->SetDirLightParam(&dirLightParams);
	}

	

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	pFaceModel->RenderAtOnce(pPortraitViewport, 0);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);

	pCurViewport->Active();
}


AUI_BEGIN_COMMAND_MAP(DlgCrssvrTeamsMain, CDlgBase)

AUI_ON_COMMAND("Btn_GroupInfo", OnCommand_GroupInfo)
AUI_ON_COMMAND("Btn_GroupApply", OnCommand_GroupApply)
AUI_ON_COMMAND("Btn_Award", OnCommand_Award)

AUI_END_COMMAND_MAP()


DlgCrssvrTeamsMain::DlgCrssvrTeamsMain()
{
	m_nWindowsType = 0;
}
DlgCrssvrTeamsMain::~DlgCrssvrTeamsMain()
{

}
bool DlgCrssvrTeamsMain::OnInitDialog()
{
	m_pTxt_GroupName = (AUILabel*)GetDlgItem("Txt_GroupName");
	m_pTxt_GroupScore= (AUILabel*)GetDlgItem("Txt_GroupScore");
	m_pLab_GroupScore= (AUILabel*)GetDlgItem("Lab_GroupScore");
	m_pTxt_SelfScore= (AUILabel*)GetDlgItem("Txt_SelfScore");
	m_pLab_SelfScore= (AUILabel*)GetDlgItem("Lab_SelfScore");
	m_pImg_Icon= (AUIImagePicture*)GetDlgItem("Img_Icon");

	//m_pTxt_SelfFightTimeWeek = (AUILabel*)GetDlgItem("Txt_SelfFightTimeWeek");
	//m_pLab_SelfFightTimeWeek = (AUILabel*)GetDlgItem("Lab_SelfFightTimeWeek");
	//m_pTxt_SelfFightTimeSeason = (AUILabel*)GetDlgItem("Txt_SelfFightTimeSeason");
	//m_pLab_SelfFightTimeSeason = (AUILabel*)GetDlgItem("Lab_SelfFightTimeSeason");

	m_pTxt_SelfFightTimeAll = (AUILabel*)GetDlgItem("Txt_SelfFightTimeAll");
	m_pLab_SelfFightTimeAll = (AUILabel*)GetDlgItem("Lab_SelfFightTimeAll");
	m_pTxt_SelfWin = (AUILabel*)GetDlgItem("Txt_SelfWin");
	m_pLab_SelfWin = (AUILabel*)GetDlgItem("Lab_SelfWin");
	m_pTxt_SelfLose = (AUILabel*)GetDlgItem("Txt_SelfLose");
	m_pLab_SelfLose = (AUILabel*)GetDlgItem("Lab_SelfLose");
	m_pTxt_SelfRate = (AUILabel*)GetDlgItem("Txt_SelfRate");
	m_pLab_SelfRate = (AUILabel*)GetDlgItem("Lab_SelfRate");

	m_pTxt_Rule = (PAUITEXTAREA)GetDlgItem("Txt_Rule");
	AString strName;
	for (int i = 0; i < eWindowsType_Count; ++ i)
	{
		strName.Format("Sub_0%d", i+1);
		m_pSubItems[i] = (AUISubDialog*)GetDlgItem(strName);
		m_pSubItems[i]->Show(false);
	}
	ACString szTemp = _AL("0");
	m_pTxt_GroupName->SetText(szTemp);
	m_pTxt_GroupScore->SetText(szTemp);
	m_pTxt_SelfScore->SetText(szTemp);

	//m_pTxt_SelfFightTimeWeek->SetText(szTemp);
	//m_pTxt_SelfFightTimeSeason->SetText(szTemp);
	m_pTxt_SelfFightTimeAll->SetText(szTemp);
	m_pTxt_SelfWin->SetText(szTemp);
	m_pTxt_SelfLose->SetText(szTemp);
	m_pTxt_SelfRate->SetText(szTemp);


	
	return true;
}

void DlgCrssvrTeamsMain::OnShowDialog()
{
	SetWindowsType(2);
	GetGameSession()->c2s_CmdSendCrssvrTeamsBase();
	GetGameSession()->c2s_CmdSendCrssvrGetRoleNotify();
	if(GetHostPlayer()->GetSrcServerID() != 0)
	{
		GetDlgItem("Btn_Award")->Enable(false);
	}
	else
	{
		GetDlgItem("Btn_Award")->Enable(true);
	}
}

 void DlgCrssvrTeamsMain::OnHideDialog()
 {
	 DlgCrssvrTeamsImg*	pDlg = (DlgCrssvrTeamsImg*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupRank_Member");
	 pDlg->Show(false);
 }

void DlgCrssvrTeamsMain::OnCommand_GroupInfo(const char * szCommand)
{
	SetWindowsType(1);

}
void DlgCrssvrTeamsMain::OnCommand_GroupApply(const char * szCommand)
{
	//DlgCrssvrTeamsApply *pDlgApply = (DlgCrssvrTeamsApply*)GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupList");
	//if(pDlgApply)
	//{
	//	pDlgApply->Show(true);
	//}
	if(g_pGame->GetGuildEnter() == true)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20051), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if(	GetHostPlayer()->GetRaidBattleInfo() > 0)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20007), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if(GetHostPlayer()->GetCrssvrTeamsId() == -1)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20045), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	if(GetHostPlayer()->GetTeam())
	{
		if(GetHostPlayer()->GetTeam()->GetLeaderID() != GetHostPlayer()->GetCharacterID())
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(20006), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
		c2s_SendCmd6v6CollisionApply(543,795,true,true);
	}
	else if(GetHostPlayer()->GetCrssvrTeamsId() != -1)
	{
		c2s_SendCmd6v6CollisionApply(543,795,false,true);
	}

}

void DlgCrssvrTeamsMain::SetDialog(int nIndex)
{
	SetWindowsType(3);
	PAUIDIALOG pDlg;
	if(nIndex == 1)
	{
		pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupAward01");
	}
	else if(nIndex == 2)
	{
		pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupAward02");
	}
	else if(nIndex == 3)
	{
		pDlg = GetGameUIMan()->GetDialog("Win_Battle_Dungeon_6v6_GroupAward03");
	}
	m_pSubItems[eWindowsType_Sub3]->SetDialog(pDlg);
	m_pSubItems[eWindowsType_Sub3]->Show(true);
}

void DlgCrssvrTeamsMain::OnCommand_Award(const char * szCommand)
{
	SetDialog(1);
}

void DlgCrssvrTeamsMain::SetWindowsType(int val)
{
	m_nWindowsType = val;
	UpdateCrssvrTeams();
}

void DlgCrssvrTeamsMain::UpData(void* pData)
{
	GNET::CrssvrTeamsTeamBase_Re* p = (GNET::CrssvrTeamsTeamBase_Re*)pData;
	ACString strName = ACString((const ACHAR*)p->name.begin(), p->name.size() / sizeof (ACHAR));
	m_pTxt_GroupName->SetText(strName);
	ACString szTemp;
	szTemp.Format(_AL("%d"),p->score);
	m_pTxt_GroupScore->SetText(szTemp);

	szTemp.Format(_AL("%d"),p->battle_wins);
	m_pTxt_SelfWin->SetText(szTemp);

	szTemp.Format(_AL("%d"),p->battle_losses);
	m_pTxt_SelfLose->SetText(szTemp);

	int nTemp  = 0;
	if(p->battle_wins != 0)
	{
		nTemp = int((float)(p->battle_wins / (float)(p->battle_wins + p->battle_losses + p->battle_draws))* 100);
		szTemp.Format(_AL("%d"),nTemp);
		szTemp +=_AL("%");
	}
	else
	{
		szTemp = _AL("0%");
	}
	m_pTxt_SelfRate->SetText(szTemp);
}

void DlgCrssvrTeamsMain::UpDataSlefScore(int nScore)
{
	ACString szTemp;
	szTemp.Format(_AL("%d"),nScore);
	m_pTxt_SelfScore->SetText(szTemp);
}

void DlgCrssvrTeamsMain::UpdateCrssvrTeams()
{
	m_pTxt_GroupName->Show(false);
	m_pLab_GroupScore->Show(false);
	m_pTxt_GroupScore->Show(false);
	m_pTxt_SelfScore->Show(false);
	m_pLab_SelfScore->Show(false);
	m_pImg_Icon->Show(false);
	//m_pTxt_SelfFightTimeWeek->Show(false);
	//m_pLab_SelfFightTimeWeek->Show(false);
	//m_pTxt_SelfFightTimeSeason->Show(false);
	//m_pLab_SelfFightTimeSeason->Show(false);
	m_pTxt_SelfFightTimeAll->Show(false);
	m_pLab_SelfFightTimeAll->Show(false);
	m_pTxt_SelfWin->Show(false);
	m_pLab_SelfWin->Show(false);
	m_pTxt_SelfLose->Show(false);
	m_pLab_SelfLose->Show(false);
	m_pTxt_SelfRate->Show(false);
	m_pLab_SelfRate->Show(false);

	m_pTxt_Rule->Show(false);
	for (int i = 0; i < eWindowsType_Count; ++ i)
	{
		m_pSubItems[i]->Show(false);
	}
	if(m_nWindowsType == 2)
	{
		m_pTxt_GroupName->Show(true);
		m_pLab_GroupScore->Show(true);
		m_pTxt_GroupScore->Show(true);
		m_pTxt_SelfScore->Show(true);
		m_pLab_SelfScore->Show(true);
		m_pImg_Icon->Show(true);

		m_pTxt_Rule->Show(true);
	}
	else if(m_nWindowsType == 1
		||   m_nWindowsType == 0)
	{
		//m_pTxt_SelfFightTimeWeek->Show(true);
		//m_pLab_SelfFightTimeWeek->Show(true);
		//m_pTxt_SelfFightTimeSeason->Show(true);
		//m_pLab_SelfFightTimeSeason->Show(true);
		m_pTxt_SelfFightTimeAll->Show(true);
		m_pLab_SelfFightTimeAll->Show(true);
		m_pTxt_SelfWin->Show(true);
		m_pLab_SelfWin->Show(true);
		m_pTxt_SelfLose->Show(true);
		m_pLab_SelfLose->Show(true);
		m_pTxt_SelfRate->Show(true);
		m_pLab_SelfRate->Show(true);

		m_pSubItems[m_nWindowsType]->Show(true);
	}
	else if( m_nWindowsType == 3)
	{
		m_pTxt_GroupName->Show(true);
		m_pLab_GroupScore->Show(true);
		m_pTxt_GroupScore->Show(true);
		m_pTxt_SelfScore->Show(true);
		m_pLab_SelfScore->Show(true);
		m_pImg_Icon->Show(true);
		m_pSubItems[eWindowsType_Sub3]->Show(true);
	}
}

void DlgCrssvrTeamsMain::ClearData()
{
	ACString szTemp = _AL("0"); 
	m_pTxt_GroupName->SetText(_AL(""));
	m_pTxt_GroupScore->SetText(szTemp);
	m_pTxt_SelfRate->SetText(szTemp);
	m_pTxt_SelfFightTimeAll->SetText(szTemp);
	m_pTxt_SelfWin->SetText(szTemp);
	m_pTxt_SelfLose->SetText(szTemp);
	m_pTxt_SelfRate->SetText(szTemp);
}