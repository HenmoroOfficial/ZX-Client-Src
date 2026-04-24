// File		: DlgFriendSeek.cpp
// Creator	: Fu Chonggang
// Date		: 2009/02/11
//FriendSeeking.dtf
//FriendSeekingMsg.dtf
//FriendInfoPub.dtf
//AskingForGuild.dtf
//AskingForFriend.dtf
//AskingForTeacher.dtf
//AskingForMarriage.dtf

#include "AFI.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\CSplit.h"
#include "DlgFriendSeeking.h"
#include "DlgSnSInfo.h"
#include "DlgEPEquip.h"
#include "DlgTrade.h"
#include "DlgRankList.h"
#include "EC_GameSession.h"
#include "EC_Algorithm.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_ManPlayer.h"
#include "EC_Player.h"
#include "EC_World.h"
#include "EC_Faction.h"
#include "EC_IvtrItem.h"
#include "time.h"

#include "snspressmessage_re.hpp"
#include "snscancelmessage_re.hpp"
#include "snsapply_re.hpp"
#include "snslistmessage_re.hpp"
#include "snsgetmessage_re.hpp"
#include "snsgetplayerinfo_re.hpp"
#include "snsvote_re.hpp"
#include "snsacceptapply_re.hpp"

#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "elementdataman.h"

#define new A_DEBUG_NEW


#define SNSINFORMATION_NUM 50
#define SNSINFORMATION_NUMONEPAGE 15
#define PIELOVECONFIGDATAID	101
#define MSGFRESHTIME		5000

typedef abase::hash_map<int, CDlgFriendSeek::PlayerInfoCache> MsgID2PlayerInfoMap;
typedef abase::hash_map<int, CDlgFriendSeek::GetMessageCache> MsgID2MsgMap;

int CDlgFriendSeek::m_iIntervel[] = {24, 24, 24, 24};
int CDlgFriendSeek::m_iMsgID = 0;
int CDlgFriendSeek::m_iSeekingType = 0;
int CDlgFriendSeek::m_iLastReleaseTime = 0;
int CDlgFriendSeek::m_iMsgType = 0;
int CDlgFriendSeek::m_iApplyRoleID = 0;
int CDlgFriendSeek::m_iMsgRoleID = 0;
int CDlgFriendSeek::m_iPlayerInfoType = 0;
bool CDlgFriendSeek::m_bShowInfo = false;
int CDlgFriendSeek::m_iCurPage	=0;		//0-3
int CDlgFriendSeek::m_iCurIndex	=0;
CDlgFriendSeek::FriendSeekTableInfo CDlgFriendSeek::m_curInfo;
CDlgFriendSeek::ApplyCondiction CDlgFriendSeek::m_acSelf[];
ACString CDlgFriendSeek::m_strMsgLeave[];
MsgID2PlayerInfoMap CDlgFriendSeek::m_id2PlayerInfoCache;
MsgID2MsgMap CDlgFriendSeek::m_mapMsgCache[];

int s_nLevel[] = { 30,45,60,75,90,105,120,135,150};
#define STAR_TO_CHARM(STAR) (20000*((STAR)*(STAR)*(STAR)+4*(STAR)))

AUI_BEGIN_COMMAND_MAP(CDlgFriendSeek, CDlgBase)

AUI_ON_COMMAND("sortdayleft",	OnCommand_SortDayLeft)
AUI_ON_COMMAND("sortgender",	OnCommand_SortGender)
AUI_ON_COMMAND("sortfavor",		OnCommand_SortFavor)
AUI_ON_COMMAND("Sortname",		OnCommand_SortName)
AUI_ON_COMMAND("sortfocus",		OnCommand_SortFocus)
AUI_ON_COMMAND("Sortanswer",	OnCommand_SortAnswer)
AUI_ON_COMMAND("Sortlevel",		OnCommand_SortLevel)

AUI_ON_COMMAND("rdo*",			OnCommand_RdoType)

AUI_ON_COMMAND("tail",			OnCommand_Tail)
AUI_ON_COMMAND("head",			OnCommand_Head)
AUI_ON_COMMAND("next",			OnCommand_Next)
AUI_ON_COMMAND("last",			OnCommand_Last)
AUI_ON_COMMAND("nextrank",		OnCommand_NextRank)
AUI_ON_COMMAND("lastrank",		OnCommand_LastRank)

AUI_ON_COMMAND("release",		OnCommand_MsgRelease)
AUI_ON_COMMAND("withdraw",		OnCommand_MsgWithdraw)
AUI_ON_COMMAND("refresh",		OnCommand_Refresh)
AUI_ON_COMMAND("support",		OnCommand_Support)
AUI_ON_COMMAND("oppose",		OnCommand_Oppose)
AUI_ON_COMMAND("info",			OnCommand_Information)
AUI_ON_COMMAND("msg",			OnCommand_Msg)
AUI_ON_COMMAND("apply",			OnCommand_Apply)
AUI_ON_COMMAND("perc",			OnCommand_PersCenter)
AUI_ON_COMMAND("charmrank",		OnCommand_CharmRank)

AUI_ON_COMMAND("press",			OnCommand_PressInfo)

AUI_ON_COMMAND("confirmmarriage",		OnCommand_ConfirmMarriage)
AUI_ON_COMMAND("confirmteacher",		OnCommand_ConfirmTeacher)
AUI_ON_COMMAND("confirmguild",			OnCommand_ConfirmGuild)
AUI_ON_COMMAND("confirmfriend",			OnCommand_ConfirmFriend)

AUI_ON_COMMAND("sendback",			OnCommand_SendBack)

AUI_ON_COMMAND("accept",			OnCommand_Accept)
AUI_ON_COMMAND("refuse",			OnCommand_Refuse)
AUI_ON_COMMAND("delete",			OnCommand_Delete)

AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgFriendSeek, CDlgBase)

AUI_ON_EVENT("List_All",	WM_LBUTTONDOWN,		OnEventLButtonDown_Lst_ListInfo)
AUI_ON_EVENT("List_All",	WM_LBUTTONDBLCLK,	OnEventLButtonDown_Lst_ListInformation)
AUI_ON_EVENT("List_Top",	WM_LBUTTONDOWN,		OnEventLButtonDown_Lst_ListMsg)

AUI_END_EVENT_MAP()

CDlgFriendSeek::CDlgFriendSeek()
{
}

CDlgFriendSeek::~CDlgFriendSeek()
{
	for (int i=0; i<SEEKTYPE_MAX; i++)
	{
		if (m_vecFSTable[i].size()>0)
		{
			m_vecFSTable[i].clear();
		}
	}
}

//FriendSeeking.dtf
//FriendSeekingMsg.dtf
//FriendInfoPub.dtf
bool CDlgFriendSeek::OnInitDialog()
{
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pPlConfig	= (PIE_LOVE_CONFIG*)pDB->get_data_ptr(PIELOVECONFIGDATAID, ID_SPACE_CONFIG, DataType);
	if ( m_szName == "Win_FriendInfoPub" )
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Profession");
		if( pCombo )
		{
			for (int i=0; i<15; i++)
			{
				pCombo->AddString(GetStringFromTable(7162+i));
			}
			pCombo->SetCurSel(0);
		}

		pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Gender");
		if( pCombo )
		{
			pCombo->AddString(GetStringFromTable(7140));
			pCombo->AddString(GetStringFromTable(7141));
			pCombo->AddString(GetStringFromTable(7142));
			pCombo->SetCurSel(0);
		}

		pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_GuildLevel");
		if( pCombo )
		{
			for (int i=0; i<6; i++)
			{
				pCombo->AddString(GetStringFromTable(7153+i));
			}
			pCombo->SetCurSel(0);
		}

		m_pEdtMessage = (PAUIEDITBOX)GetDlgItem("Edt_Message");
		m_pEdtMessage->SetIsAutoReturn(true);
	}
	else if ( m_szName == "Win_FriendSeeking" )
	{
		m_iApplyRoleID		= 0;
		m_iMsgRoleID		= 0;
		m_iMsgID			= -1;
		m_iMsgIDDst			= -1;
		m_iPlayerInfoType	= 0;
		m_iSeekingType		= SEEKTYPE_MARIAGE;
		m_iSortType			= SORTTYPE_ORIG;
		m_iCurIndex			= -1;
		m_iCurPage			= 0;
		m_id2PlayerInfoCache.clear();
		int i;
		for (i=0; i<SEEKTYPE_MAX; i++)
		{
			m_iLastRefreshTime[i] = 0;
			m_vecFSTable[i].clear();
			m_mapMsgCache[i].clear();
		}
		m_pLabPage = (PAUILABEL)GetDlgItem("Txt_Page");
		m_pLst_All = (PAUILISTBOX)GetDlgItem("List_All");
		m_Btn_Refresh = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Renew");
		m_Btn_Head = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_First");
		m_Btn_Tail = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Last");
		m_Btn_Last = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Previous");
		m_Btn_Next = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Next");
		m_Btn_WithDraw = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Withdraw");
		m_Btn_Msg = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Msg");
		m_Btn_Information = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Info");
		m_pLabTitle = (PAUILABEL)GetDlgItem("Lab_PerCharm2");
		for (i=0; i<10; i++)
		{
			char szName[20];
			sprintf(szName,"Img_Star%d",i+1);
			GetDlgItem(szName)->Show(false);
		}
		CheckRadioButton(1, 1);
	}
	else if ( m_szName == "Win_FriendSeekingMsg" )
	{
		m_bCanSend = false;
		m_iCurMsgPage			= 0;
		m_iCurMsgIndex			= -1;
		m_iLastRefreshMsgTime	= 0;
		m_pLabPage = (PAUILABEL)GetDlgItem("Txt_Page");
		m_pLst_All = (PAUILISTBOX)GetDlgItem("List_Top");
		m_pEdtAnswer = (PAUIEDITBOX)GetDlgItem("Edit_Answer");
		m_pTxtMsgHead = (PAUITEXTAREA)GetDlgItem("Txt_Msg2");
		m_pTvMessage = (PAUITREEVIEW)GetDlgItem("Tv_Msg");
		m_Btn_Refresh = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Refresh");
		m_Btn_Head = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_First");
		m_Btn_Tail = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Last");
		m_Btn_Last = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Previous");
		m_Btn_Next = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Next");
		m_Btn_Accept = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Accept");
		m_Btn_Refuse = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Refuse");
		m_Btn_Send = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Send");
		m_Btn_Information = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_CheckInfo");
		m_Btn_VoteSupport = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Support");
		m_Btn_VoteOppose = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Oppose");
		m_Btn_Apply = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Apply");
		m_Btn_Delete = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Delete");

		m_pEdtAnswer->SetIsAutoReturn(true);
		CheckRadioButton(1, 1);
	}
	else if ( m_szName == "Win_FriendSeekingPerCharm" )
	{
		m_iCurCharmPage		= 0;
		m_pLst_Charm = (PAUILISTBOX)GetDlgItem("Lst_Charm");
		m_pLabTitle = (PAUILABEL)GetDlgItem("Lab_Title");
	}
	return true;
}

//FriendSeeking.dtf
//FriendSeekingMsg.dtf
//FriendInfoPub.dtf
void CDlgFriendSeek::OnShowDialog()
{
	if ( m_szName == "Win_FriendInfoPub" )
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Level");
		pCombo->ResetContent();
		for (int i=0; i<5; i++)
		{
			pCombo->AddString(GetStringFromTable(7143+i));
		}
		if (m_iSeekingType!=2)
		{
			pCombo->AddString(GetStringFromTable(7148));
			pCombo->AddString(GetStringFromTable(7149));
			pCombo->AddString(GetStringFromTable(7150));
			pCombo->AddString(GetStringFromTable(7151));
			pCombo->AddString(GetStringFromTable(7152));
		}
		pCombo->SetCurSel(0);

		pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Gender");
		if (m_iSeekingType == SEEKTYPE_MARIAGE)
		{
			if (GetHostPlayer()->GetGender())
				pCombo->SetCurSel(1);
			else
				pCombo->SetCurSel(2);
			pCombo->Enable(false);
		}
		else
		{
			pCombo->SetCurSel(0);
			pCombo->Enable(true);
		}
		pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_GuildLevel");
		if (m_iSeekingType == SEEKTYPE_GUILD)
			pCombo->Enable(false);
		else
			pCombo->Enable(true);
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Title");
		pLab->SetText(GetStringFromTable(7194+m_iSeekingType));
		ACHAR szText[256];
		a_sprintf(szText,GetStringFromTable(7225+m_iSeekingType),GetHostPlayer()->GetName());
		m_pEdtMessage->SetText(szText);
	}
	else if (m_szName == "Win_FriendSeeking")
	{
		m_iLastRefreshTime[m_iSeekingType] = GetTickCount()-MSGFRESHTIME;
		int id = GetHostPlayer()->GetCharacterID();
		MsgID2PlayerInfoMap::iterator iter = m_id2PlayerInfoCache.find(id);
		if (iter==m_id2PlayerInfoCache.end())
		{
			GetGameSession()->sns_get_playerinfo(0,id);
		}
		int iii = m_iSeekingType;
		OnCommand_Refresh("");
		/*
		AUIOBJECT_SETPROPERTY p;
		if ( 0 == m_iSeekingType )
		{
			sprintf(p.fn, "Window\\FriendSeeking_Lover.tga");
		}
		else if ( 1 == m_iSeekingType )
		{
			sprintf(p.fn, "Window\\FriendSeeking_Friend.tga");
		}
		else if ( 2 == m_iSeekingType )
		{
			sprintf(p.fn, "Window\\FriendSeeking_Teacher.tga");
		}
		else if ( 3 == m_iSeekingType )
		{
			sprintf(p.fn, "Window\\FriendSeeking_Guild.tga");
		}
		SetProperty("Frame Image", &p);
		*/
		m_pLabTitle->SetText(GetStringFromTable(7340+m_iSeekingType));
	}
	else if ( m_szName == "Win_FriendSeekingMsg" )
	{
		m_iLastRefreshMsgTime = GetTickCount()-MSGFRESHTIME;
		m_bCanSend = false;
		int idPlayer = GetHostPlayer()->GetCharacterID();
		if ( idPlayer == m_iMsgRoleID)
		{
			m_bCanSend = true;
		}
		else
		{
			int iSize = m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[0].size();
			int i;
			for (i=0; i<iSize; i++)
			{
				if (idPlayer == m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[0][i].nRoleid)
				{
					m_bCanSend = true;
				}
			}
			iSize = m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[1].size();
			for (i=0; i<iSize; i++)
			{
				if (idPlayer == m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[1][i].nRoleid)
				{
					m_bCanSend = true;
				}
			}
			iSize = m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[2].size();
			for (i=0; i<iSize; i++)
			{
				if (idPlayer == m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[2][i].nRoleid)
				{
					m_bCanSend = true;
				}
			}
		}
		OnCommand_Refresh("");
	}
	else if ( m_szName == "Win_FriendSeekingPerCenter" )
	{
		int id = GetHostPlayer()->GetCharacterID();
		MsgID2PlayerInfoMap::iterator iter = m_id2PlayerInfoCache.find(id);
		if (iter==m_id2PlayerInfoCache.end())
		{
			GetGameSession()->sns_get_playerinfo(0,id);
		}
		else
		{
			ACHAR szText[20];
			PlayerInfoCache selfInfo = m_id2PlayerInfoCache[id];
			PAUILABEL pLab_MarryCharm = (PAUILABEL)GetDlgItem("Txt_Per");
			PAUILABEL pLab_MarryStar = (PAUILABEL)GetDlgItem("Txt_PerStar");
			PAUILABEL pLab_FriendCharm = (PAUILABEL)GetDlgItem("Txt_Friend");
			PAUILABEL pLab_FriendStar = (PAUILABEL)GetDlgItem("Txt_FriendStar");
			PAUILABEL pLab_TeachCharm = (PAUILABEL)GetDlgItem("Txt_Teacher");
			PAUILABEL pLab_TeachStar = (PAUILABEL)GetDlgItem("Txt_TeacherStar");
			PAUILABEL pLab_GuildCharm = (PAUILABEL)GetDlgItem("Txt_Guild");
			PAUILABEL pLab_GuildStar = (PAUILABEL)GetDlgItem("Txt_GuildStar");
			a_sprintf(szText,_AL("%d"),selfInfo.playerinfo.charmvec[0]);
			pLab_MarryCharm->SetText(szText);
			a_sprintf(szText,_AL("%d"),Charm2Star(selfInfo.playerinfo.charmvec[0]));
			pLab_MarryStar->SetText(szText);
			a_sprintf(szText,_AL("%d"),selfInfo.playerinfo.charmvec[1]);
			pLab_FriendCharm->SetText(szText);
			a_sprintf(szText,_AL("%d"),Charm2Star(selfInfo.playerinfo.charmvec[1]));
			pLab_FriendStar->SetText(szText);
			a_sprintf(szText,_AL("%d"),selfInfo.playerinfo.charmvec[2]);
			pLab_TeachCharm->SetText(szText);
			a_sprintf(szText,_AL("%d"),Charm2Star(selfInfo.playerinfo.charmvec[2]));
			pLab_TeachStar->SetText(szText);
			a_sprintf(szText,_AL("%d"),selfInfo.playerinfo.charmvec[3]);
			pLab_GuildCharm->SetText(szText);
			a_sprintf(szText,_AL("%d"),Charm2Star(selfInfo.playerinfo.charmvec[3]));
			pLab_GuildStar->SetText(szText);
		}
	}
	else if ( m_szName == "Win_FriendSeekingPerCharm" )
	{
		/*
		AUIOBJECT_SETPROPERTY p;
		if ( 0 == m_iSeekingType )
		{
			sprintf(p.fn, "Window\\FriendSeeking_Lover.tga");
		}
		else if ( 1 == m_iSeekingType )
		{
			sprintf(p.fn, "Window\\FriendSeeking_Friend.tga");
		}
		else if ( 2 == m_iSeekingType )
		{
			sprintf(p.fn, "Window\\FriendSeeking_Teacher.tga");
		}
		else if ( 3 == m_iSeekingType )
		{
			sprintf(p.fn, "Window\\FriendSeeking_Guild.tga");
		}
		SetProperty("Frame Image", &p);
		*/
		m_pLabTitle->SetText(GetStringFromTable(7340+m_iSeekingType));
		AddCharmRank();
	}
}

//FriendSeeking.dtf
//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnTick()
{
	if (m_szName == "Win_FriendSeeking")
	{
		if ( GetTickCount() - m_iLastRefreshTime[m_iSeekingType] < MSGFRESHTIME )
			m_Btn_Refresh->Enable(false);
		else
			m_Btn_Refresh->Enable(true);

		if (-1==m_iCurIndex )
		{
			m_Btn_Information->Enable(false);
			m_Btn_Msg->Enable(false);
		}
		else
		{
			m_Btn_Information->Enable(true);
			m_Btn_Msg->Enable(true);
		}

//		if (-1==m_iCurIndex || GetHostPlayer()->GetCharacterID() == m_iMsgRoleID)
//		{
//			m_Btn_Apply->Enable(false);
//			m_Btn_VoteOppose->Enable(false);
//			m_Btn_VoteSupport->Enable(false);
//		}
//		else
//		{
//			m_Btn_Apply->Enable(true);
//			m_Btn_VoteSupport->Enable(true);
//			m_Btn_VoteOppose->Enable(true);
//		}
		if (-1==m_iCurIndex || GetHostPlayer()->GetCharacterID() != m_iMsgRoleID)
			m_Btn_WithDraw->Enable(false);
		else
			m_Btn_WithDraw->Enable(true);

		if (GetHostPlayer()->IsGM())
		{
			m_Btn_WithDraw->Enable(true);
		}
	}
	else if (m_szName == "Win_FriendSeekingMsg")
	{
		if ( GetTickCount() - m_iLastRefreshMsgTime < MSGFRESHTIME )
		{
			m_Btn_Refresh->Enable(false);
		}
		else
			m_Btn_Refresh->Enable(true);

		m_Btn_Information->Enable(false);
		m_Btn_Accept->Enable(false);
		m_Btn_Refuse->Enable(false);
		if (-1!=m_iCurMsgIndex)
		{
			m_Btn_Information->Enable(true);
		}
		if (-1!=m_iCurMsgIndex && 0==m_iMsgType && GetHostPlayer()->GetCharacterID() == m_iMsgRoleID)
		{
			m_Btn_Accept->Enable(true);
			m_Btn_Refuse->Enable(true);
		}

		if (GetHostPlayer()->GetCharacterID() == m_iMsgRoleID)
		{
			m_Btn_Apply->Enable(false);
			m_Btn_VoteOppose->Enable(false);
			m_Btn_VoteSupport->Enable(false);
		}
		else
		{
			m_Btn_Apply->Enable(true);
			m_Btn_VoteSupport->Enable(true);
			m_Btn_VoteOppose->Enable(true);
		}

		if (NULL!=m_pTvMessage->GetSelectedItem() &&
			(m_iMsgRoleID==GetHostPlayer()->GetCharacterID() ||
			GetHostPlayer()->IsGM()))
		{
			m_Btn_Delete->Enable(true);
		}
		else
			m_Btn_Delete->Enable(false);
	}
	else if ( m_szName == "Win_FriendSeekingPerCharm" )
	{
		AlignTo(GetGameUIMan()->GetDialog("Win_FriendSeeking"),AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
						AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM, -1);
	}
}

class CompareFriendSeekInfoShowOrderByFavor
{
public:
	bool operator ()(CDlgFriendSeek::FriendSeekTableInfo & p1, CDlgFriendSeek::FriendSeekTableInfo & p2)
	{
		return p1.nFavor > p2.nFavor;
	}
};

class CompareFriendSeekMsgShowOrderByFavor
{
public:
	bool operator ()(CDlgFriendSeek::ApplyInfo & p1, CDlgFriendSeek::ApplyInfo & p2)
	{
		return p1.nCount > p2.nCount;
	}
};

class CompareFriendSeekInfoShowOrderByGender
{
public:
	bool operator ()(CDlgFriendSeek::FriendSeekTableInfo & p1, CDlgFriendSeek::FriendSeekTableInfo & p2)
	{
		return p1.bGender < p2.bGender;
	}
};

class CompareFriendSeekMsgShowOrderByGender
{
public:
	bool operator ()(CDlgFriendSeek::ApplyInfo & p1, CDlgFriendSeek::ApplyInfo & p2)
	{
		return p1.nGender < p2.nGender;
	}
};

class CompareFriendSeekInfoShowOrderByFocus
{
public:
	bool operator ()(CDlgFriendSeek::FriendSeekTableInfo & p1, CDlgFriendSeek::FriendSeekTableInfo & p2)
	{
		return p1.nFocus > p2.nFocus;
	}
};

class CompareFriendSeekInfoShowOrderByAnswer
{
public:
	bool operator ()(CDlgFriendSeek::FriendSeekTableInfo & p1, CDlgFriendSeek::FriendSeekTableInfo & p2)
	{
		return p1.nAnswer > p2.nAnswer;
	}
};

class CompareFriendSeekInfoShowOrderByTime
{
public:
	bool operator ()(CDlgFriendSeek::FriendSeekTableInfo & p1, CDlgFriendSeek::FriendSeekTableInfo & p2)
	{
		return p1.nPressTime > p2.nPressTime;
	}
};

class CompareFriendSeekMsgShowOrderByLevel
{
public:
	bool operator ()(CDlgFriendSeek::ApplyInfo & p1, CDlgFriendSeek::ApplyInfo & p2)
	{
		return p1.nLevel > p2.nLevel;
	}
};

//FriendSeeking.dtf
void CDlgFriendSeek::OnCommand_SortDayLeft(const char * szCommand)
{
	m_iSortType = SORTTYPE_LEAVETIME;
	SortList();
	AddToList(m_iCurPage);
}
//FriendSeeking.dtf
//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnCommand_SortGender(const char * szCommand)
{
	if (m_szName == "Win_FriendSeeking")
	{
		m_iSortType = SORTTYPE_GENDER;
		SortList();
		AddToList(m_iCurPage);
	}
	else
	{
		BubbleSort(m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType].begin(), 
			m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType].end(), 
			CompareFriendSeekMsgShowOrderByGender());
		AddVoteList();
	}
}
//FriendSeeking.dtf
//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnCommand_SortFavor(const char * szCommand)
{
	if (m_szName == "Win_FriendSeeking")
	{
		m_iSortType = SORTTYPE_FAVOR;
		SortList();
		AddToList(m_iCurPage);
	}
	else
	{
		BubbleSort(m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType].begin(), 
			m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType].end(), 
			CompareFriendSeekMsgShowOrderByFavor());
		AddVoteList();
	}
}
void CDlgFriendSeek::OnCommand_SortName(const char * szCommand)
{
}
//FriendSeeking.dtf
void CDlgFriendSeek::OnCommand_SortFocus(const char * szCommand)
{
	m_iSortType = SORTTYPE_FOCUS;
	SortList();
	AddToList(m_iCurPage);
}
//FriendSeeking.dtf
void CDlgFriendSeek::OnCommand_SortAnswer(const char * szCommand)
{
	m_iSortType = SORTTYPE_ANSWER;
	SortList();
	AddToList(m_iCurPage);
}
//FriendSeeking.dtf
void CDlgFriendSeek::OnCommand_SortLevel(const char * szCommand)
{
	BubbleSort(m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType].begin(), 
			m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType].end(), 
			CompareFriendSeekMsgShowOrderByLevel());
	AddVoteList();
}
//FriendSeeking.dtf
//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnCommand_RdoType(const char * szCommand)
{
	if (m_szName == "Win_FriendSeeking")
	{
		m_iCurPage = 0;
		m_iCurIndex = -1;
		m_iSeekingType = atoi(szCommand + strlen("rdo"))-1;
		GetDlgItem("Btn_Favor")->SetText(GetStringFromTable(7280+m_iSeekingType));
		GetDlgItem("Lab_Star")->SetText(GetStringFromTable(7295+m_iSeekingType));
		m_iSortType = SORTTYPE_ORIG;
		if (m_vecFSTable[m_iSeekingType].size()==0)
		{
			OnCommand_Refresh("");
		}
		else
		{
			SortList();
			AddToList(m_iCurPage);
			OnEventLButtonDown_Lst_ListInfo(0,0,NULL);
		}
		/*
		AUIOBJECT_SETPROPERTY p;
		if ( 0 == m_iSeekingType )
		{
			sprintf(p.fn, "Window\\FriendSeeking_Lover.tga");
		}
		else if ( 1 == m_iSeekingType )
		{
			sprintf(p.fn, "Window\\FriendSeeking_Friend.tga");
		}
		else if ( 2 == m_iSeekingType )
		{
			sprintf(p.fn, "Window\\FriendSeeking_Teacher.tga");
		}
		else if ( 3 == m_iSeekingType )
		{
			sprintf(p.fn, "Window\\FriendSeeking_Guild.tga");
		}
		SetProperty("Frame Image", &p);
		
		((CDlgFriendSeek*)GetGameUIMan()->GetDialog("Win_FriendSeekingPerCharm"))->SetProperty("Frame Image", &p);
		*/
		m_pLabTitle->SetText(GetStringFromTable(7340+m_iSeekingType));
		((CDlgFriendSeek*)GetGameUIMan()->GetDialog("Win_FriendSeekingPerCharm"))->ChangeTitle();
		((CDlgFriendSeek*)GetGameUIMan()->GetDialog("Win_FriendSeekingPerCharm"))->AddCharmRank();
	}
	else if ( m_szName == "Win_FriendSeekingMsg" )
	{
		m_iMsgType = atoi(szCommand + strlen("rdo"))-11;
		m_iCurMsgPage = 0;
		m_iCurMsgIndex = -1;
		AddVoteList();
		OnEventLButtonDown_Lst_ListMsg(0,0,NULL);
	}
	
}
//FriendSeeking.dtf
//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnCommand_Tail(const char * szCommand)
{
	if (m_szName == "Win_FriendSeeking")
	{
		int iSize = m_vecFSTable[m_iSeekingType].size();
		int iSizePage = (iSize+14)/15;
		m_iCurPage = iSizePage-1;
		AddToList();
	}
	else if (m_szName == "Win_FriendSeekingMsg")
	{
		int iSize = m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType].size();
		int iSizePage = (iSize+14)/15;
		m_iCurMsgPage = iSizePage-1;
		AddVoteList();
	}
}
//FriendSeeking.dtf
//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnCommand_Head(const char * szCommand)
{
	if (m_szName == "Win_FriendSeeking")
	{
		int iSize = m_vecFSTable[m_iSeekingType].size();
		int iSizePage = (iSize+14)/15;
		if (iSize == 0)
		{
			return;
		}
		m_iCurPage = 0;
		AddToList();
	}
	else if (m_szName == "Win_FriendSeekingMsg")
	{
		int iSize = m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType].size();
		int iSizePage = (iSize+14)/15;
		if (iSize ==0)
		{
			return;
		}
		m_iCurMsgPage = 0;
		AddVoteList();
	}
}
//FriendSeeking.dtf
//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnCommand_Next(const char * szCommand)
{
	if (m_szName == "Win_FriendSeeking")
	{
		int iSize = m_vecFSTable[m_iSeekingType].size();
		int iSizePage = (iSize+14)/15;
		if ( iSize==0 || (m_iCurPage+1) == iSizePage )
		{
			return;
		}
		m_iCurPage++;
		if (m_iCurPage>3)
		{
			m_iCurPage = 3;
		}
		AddToList();
	}
	else if (m_szName == "Win_FriendSeekingMsg")
	{
		int iSize = m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType].size();
		int iSizePage = (iSize+14)/15;
		if ( iSize==0 || (m_iCurMsgPage+1) == iSizePage )
			return;
		m_iCurMsgPage++;
		AddVoteList();
	}
}
//FriendSeeking.dtf
//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnCommand_Last(const char * szCommand)
{
	if (m_szName == "Win_FriendSeeking")
	{
		m_iCurPage--;
		if (m_iCurPage<=0)
		{
			m_iCurPage = 0;
		}
		AddToList();
	}
	else if (m_szName == "Win_FriendSeekingMsg")
	{
		int iSize = m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType].size();
		if (m_iCurMsgPage <=0 )
			return;
		m_iCurMsgPage--;
		AddVoteList();
	}
}

void CDlgFriendSeek::OnCommand_NextRank(const char * szCommand)
{
	if (m_iCurCharmPage<2)
	{
		m_iCurCharmPage++;
		AddCharmRank();
	}
}

void CDlgFriendSeek::OnCommand_LastRank(const char * szCommand)
{
	if (m_iCurCharmPage>0)
	{
		m_iCurCharmPage--;
		AddCharmRank();
	}
}

//FriendSeeking.dtf
void CDlgFriendSeek::OnCommand_MsgRelease(const char * szCommand)
{
	if (GetHostPlayer()->GetBasicProps().iLevel<15)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(7130), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if (GetHostPlayer()->GetMoneyAmount() < m_pPlConfig->sns_config[m_iSeekingType].fee_press_msg)
	{
		ACHAR szText[30];
		ACHAR szText1[256];
		CECIvtrItem::GetPriceText(szText, m_pPlConfig->sns_config[m_iSeekingType].fee_press_msg);
		a_sprintf(szText1,GetStringFromTable(7125),szText);
		GetGameUIMan()->MessageBox("",szText1, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if ( -1!=m_pPlConfig->sns_config[m_iSeekingType].id_press_msg_item &&
		-1!=m_pPlConfig->sns_config[m_iSeekingType].id2_press_msg_item &&
		GetHostPlayer()->GetPack()->GetItemTotalNum(m_pPlConfig->sns_config[m_iSeekingType].id_press_msg_item) < 1 &&
		GetHostPlayer()->GetPack()->GetItemTotalNum(m_pPlConfig->sns_config[m_iSeekingType].id2_press_msg_item) < 1 )
	{
		ACHAR szText[256];
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(m_pPlConfig->sns_config[m_iSeekingType].id_press_msg_item, 0, 1);
		a_sprintf(szText,GetStringFromTable(7126),pItem ? pItem->GetName() : _AL("0"));
		if (pItem)
		{
			delete pItem;
		}
		GetGameUIMan()->MessageBox("",szText, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
	{
		GetGameUIMan()->m_pDlgFriendInfoPub->SetInfoType(m_iSeekingType);
		GetGameUIMan()->m_pDlgFriendInfoPub->Show(true);
	}
}
//FriendSeeking.dtf
void CDlgFriendSeek::OnCommand_MsgWithdraw(const char * szCommand)
{
	int charmStar = m_vecFSTable[m_iSeekingType][m_iCurPage*15+m_iCurIndex].charm_player;
	if (charmStar>3)
		GetGameUIMan()->MessageBox("Game_WithdrawMsg",GetStringFromTable(7131), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
	else
		GetGameUIMan()->MessageBox("Game_WithdrawMsg",GetStringFromTable(7132), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
}
//FriendSeeking.dtf
//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnCommand_Refresh(const char * szCommand)
{
	if (m_szName == "Win_FriendSeeking")
	{
		m_iLastRefreshTime[m_iSeekingType] = GetTickCount();
		GetGameSession()->sns_list_message(m_iSeekingType);
	}
	else if (m_szName == "Win_FriendSeekingMsg")
	{
		m_iLastRefreshMsgTime = GetTickCount();
		GetGameSession()->sns_get_message(m_iSeekingType,m_iMsgID);
	}
}
//FriendSeeking.dtf
void CDlgFriendSeek::OnCommand_Support(const char * szCommand)
{
	if (-1==m_pPlConfig->sns_config[m_iSeekingType].id_support_msg_item ||
		GetHostPlayer()->GetPack()->GetItemTotalNum(m_pPlConfig->sns_config[m_iSeekingType].id_support_msg_item) > 0)
	{
		ACHAR szText[30];
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(m_pPlConfig->sns_config[m_iSeekingType].id_support_msg_item, 0, 1);
		a_sprintf(szText,GetStringFromTable(7133),pItem ? pItem->GetName() : _AL("0"));
		if (pItem)
		{
			delete pItem;
		}
		GetGameUIMan()->MessageBox("Game_VoteSupport",szText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
	{
		ACHAR szText[30];
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(m_pPlConfig->sns_config[m_iSeekingType].id_support_msg_item, 0, 1);
		a_sprintf(szText,GetStringFromTable(7193),pItem ? pItem->GetName() : _AL("0"));
		if (pItem)
		{
			delete pItem;
		}
		GetGameUIMan()->MessageBox("",szText, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
}
//FriendSeeking.dtf
void CDlgFriendSeek::OnCommand_Oppose(const char * szCommand)
{
	if (-1==m_pPlConfig->sns_config[m_iSeekingType].id_against_msg_item ||
		GetHostPlayer()->GetPack()->GetItemTotalNum(m_pPlConfig->sns_config[m_iSeekingType].id_against_msg_item) > 0)
	{
		ACHAR szText[30];
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(m_pPlConfig->sns_config[m_iSeekingType].id_against_msg_item, 0, 1);
		a_sprintf(szText,GetStringFromTable(7134),pItem ? pItem->GetName() : _AL("0"));
		if (pItem)
		{
			delete pItem;
		}
		GetGameUIMan()->MessageBox("Game_VoteOppose",szText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
	{
		ACHAR szText[30];
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(m_pPlConfig->sns_config[m_iSeekingType].id_against_msg_item, 0, 1);
		a_sprintf(szText,GetStringFromTable(7192),pItem ? pItem->GetName() : _AL("0"));
		if (pItem)
		{
			delete pItem;
		}
		GetGameUIMan()->MessageBox("",szText, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
}
//FriendSeeking.dtf
void CDlgFriendSeek::OnCommand_Msg(const char * szCommand)
{
	MsgID2MsgMap::iterator it = m_mapMsgCache[m_iSeekingType].find(m_iMsgID);
	m_curInfo = m_vecFSTable[m_iSeekingType][m_iCurPage*15 + m_iCurIndex];
	if ( it ==  m_mapMsgCache[m_iSeekingType].end() )
		GetGameSession()->sns_get_message(m_iSeekingType,m_iMsgID);
	else
	{
//		int idPlayer = GetHostPlayer()->GetCharacterID();
//		if ( idPlayer == m_iMsgRoleID)
//		{
			GetGameUIMan()->GetDialog("Win_FriendSeeking")->Show(false);
			GetGameUIMan()->GetDialog("Win_FriendSeekingPerCharm")->Show(false);
			GetGameUIMan()->GetDialog("Win_FriendSeekingMsg")->Show(true);
//			return;
//		}
//		else
//		{
//			for(int j=0; j<3; j++)
//			{
//				int iSize = m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[j].size();
//				for (int i=0; i<iSize; i++)
//				{
//					int id = m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[j][i].nRoleid;
//					if (idPlayer == m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[j][i].nRoleid)
//					{
//						GetGameUIMan()->GetDialog("Win_FriendSeeking")->Show(false);
//						GetGameUIMan()->GetDialog("Win_FriendSeekingMsg")->Show(true);
//						return;
//					}
//				}
//			}
//		}
//		GetGameUIMan()->MessageBox("",GetStringFromTable(7290), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
}
//FriendSeeking.dtf
//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnCommand_Information(const char * szCommand)
{
	CDlgSnsInfo *pDlgSnsInfo = (CDlgSnsInfo *)GetGameUIMan()->GetDialog("Win_PersInfoCheck");
	int id;
	if ( m_szName == "Win_FriendSeeking" )
		id = m_iMsgRoleID;
	else if ( m_szName == "Win_FriendSeekingMsg" )
		id = m_iApplyRoleID;
	MsgID2PlayerInfoMap::iterator iter = m_id2PlayerInfoCache.find(id);
	if (iter==m_id2PlayerInfoCache.end())
	{
		m_bShowInfo = true;
		GetGameSession()->sns_get_playerinfo(0,id);
	}
	else
	{
		pDlgSnsInfo->SetInfo(m_id2PlayerInfoCache[id], true);
	}
}

void CDlgFriendSeek::OnCommand_PersCenter(const char * szCommand)
{
	CDlgFriendSeek *pDlgPersCenter = (CDlgFriendSeek *)GetGameUIMan()->GetDialog("Win_FriendSeekingPerCenter");
	pDlgPersCenter->Show(!pDlgPersCenter->IsShow());
}

void CDlgFriendSeek::OnCommand_CharmRank(const char * szCommand)
{
	CDlgFriendSeek *pDlgCharmRank = (CDlgFriendSeek *)GetGameUIMan()->GetDialog("Win_FriendSeekingPerCharm");
	pDlgCharmRank->Show(!pDlgCharmRank->IsShow());
}

void CDlgFriendSeek::OnCommand_Delete(const char * szCommand)
{
	if (m_pTvMessage->GetSelectedItem())
	{
		m_idelLeaveID = m_pTvMessage->GetItemData(m_pTvMessage->GetSelectedItem(),"");
		GetGameUIMan()->MessageBox("Game_DeleteMsg",GetStringFromTable(7299), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
	};
}

void CDlgFriendSeek::DeleteLeaveMsg()
{
	GetGameSession()->sns_del_message(m_iMsgID,m_iSeekingType,m_idelLeaveID);
}

bool CDlgFriendSeek::CheckCondition()
{
	CECHostPlayer* pHostPlayer = GetHostPlayer();
	FriendSeekTableInfo curTable = m_vecFSTable[m_iSeekingType][m_iCurPage*15 + m_iCurIndex];
	int idFaction = pHostPlayer->GetFactionID();
	if ((255 != curTable.aCondiction.nFactionLevel && (idFaction==0 ||curTable.aCondiction.nFactionLevel>GetGame()->GetFactionMan()->GetFaction(idFaction)->GetLevel()))||
		(255 != curTable.aCondiction.bGender && curTable.aCondiction.bGender != (pHostPlayer->GetGender()!=0) )||
		(255 != curTable.aCondiction.nLevel && curTable.aCondiction.nLevel> pHostPlayer->GetBasicProps().iLevel) ||
		(255 != curTable.aCondiction.uOccupation && !CheckProfression(curTable.aCondiction.uOccupation) ))
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(7210), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return false;
	}
	else 
		return true;
}

//FriendSeeking.dtf
void CDlgFriendSeek::OnCommand_Apply(const char * szCommand)
{
	CDlgFriendSeek *pDlgFriendSeeking = (CDlgFriendSeek *)GetGameUIMan()->GetDialog("Win_FriendSeeking");
	if (!pDlgFriendSeeking->CheckCondition())
	{
		return;
	}
	CECHostPlayer* pHostPlayer = GetHostPlayer();
	ACHAR szText[256];
	ACHAR szText1[30];
	CDlgFriendSeek *pDlgApply;
	PAUILABEL pLab;
		
	switch(m_iSeekingType) {
	case SEEKTYPE_MARIAGE:
		if (GetHostPlayer()->GetSpouse()>0)
		{
			GetGameUIMan()->MessageBox("",GetStringFromTable(7289), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
		else
		{
			CECIvtrItem::GetPriceText(szText1, m_pPlConfig->sns_config[m_iSeekingType].fee_apply_msg);
			a_sprintf(szText,GetStringFromTable(7306),szText1);
			pDlgApply = (CDlgFriendSeek *)GetGameUIMan()->GetDialog("Win_MarriageAsking");
			pLab = (PAUILABEL)pDlgApply->GetDlgItem("Lab_1");
			pLab->SetText(szText);
			pDlgApply->Show(true);
		}
		return;
	case SEEKTYPE_FRIEND:
		CECIvtrItem::GetPriceText(szText1, m_pPlConfig->sns_config[m_iSeekingType].fee_apply_msg);
		a_sprintf(szText,GetStringFromTable(7307),szText1);
		pDlgApply = (CDlgFriendSeek *)GetGameUIMan()->GetDialog("Win_AskingForFriend");
		pLab = (PAUILABEL)pDlgApply->GetDlgItem("Lab_1");
		pLab->SetText(szText);
		pDlgApply->Show(true);
		return;
	case SEEKTYPE_TEACHER:
		if (pHostPlayer->GetBasicProps().iLevel>=90)
		{
			GetGameUIMan()->MessageBox("",GetStringFromTable(7269), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
		else
		{
			CECIvtrItem::GetPriceText(szText1, m_pPlConfig->sns_config[m_iSeekingType].fee_apply_msg);
			a_sprintf(szText,GetStringFromTable(7308),szText1);
			pDlgApply = (CDlgFriendSeek *)GetGameUIMan()->GetDialog("Win_AskingForTeacher");
			pLab = (PAUILABEL)pDlgApply->GetDlgItem("Lab_1");
			pLab->SetText(szText);
			pDlgApply->Show(true);
		}
		return;
	case SEEKTYPE_GUILD:
		if (pHostPlayer->GetFactionID()>0)
		{
			GetGameUIMan()->MessageBox("",GetStringFromTable(7270), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
		else
		{
			CECIvtrItem::GetPriceText(szText1, m_pPlConfig->sns_config[m_iSeekingType].fee_apply_msg);
			a_sprintf(szText,GetStringFromTable(7309),szText1);
			pDlgApply = (CDlgFriendSeek *)GetGameUIMan()->GetDialog("Win_AskingForGuild");
			pLab = (PAUILABEL)pDlgApply->GetDlgItem("Lab_1");
			pLab->SetText(szText);
			pDlgApply->Show(true);
		}
		return;
	default:
		assert(0);
	}
}

bool CDlgFriendSeek::CheckProfression(int cont)
{
	int prof = GetHostPlayer()->GetProfession();
	switch(cont) {
	case -1:
		return true;
	case 0:
		if ( prof==7 || prof==8 || prof==9 || prof==19 || prof==20 || prof==21 )
			return true;
		else
			return false;
	case 1:
		if ( prof==4 || prof==5 || prof==6 || prof==16 || prof==17 || prof==18 )
			return true;
		else
			return false;
	case 2:
		if ( prof==10 || prof==11 || prof==12 || prof==22 || prof==23 || prof==24 )
			return true;
		else
			return false;
	case 3:
		if ( prof==25 || prof==26 || prof==27 || prof==28 || prof==29 )
			return true;
		else
			return false;
	case 4:
		if ( prof==1 || prof==2 || prof==3 || prof==13 || prof==14 || prof==15 )
			return true;
		else
			return false;
	case 5:
		if ( prof>=33 && prof<=38 )
			return true;
		else
			return false;
	case 6:
		if ( prof>=39 && prof<=44 )
			return true;
		else
			return false;
	case 7:
		if ( prof>=45 && prof<=50 )
			return true;
		else
			return false;
	case 8:
		if ( prof>=51 && prof<=56 )
			return true;
		else
			return false;
	case 9:
		if ( prof>=64 && prof<=69 )
			return true;
		else
			return false;
	case 10:
		if ( prof>=96 && prof<=101 )
			return true;
		else
			return false;
	case 11:
		if ( prof>=56 && prof<=60 )
			return true;
		else
			return false;
	case 12:
		if ( prof>=103 && prof<=107 )
			return true;
		else
			return false;
	case 13:
		if ( prof>=109 && prof<=113 )
			return true;
		else
			return false;
	default:
		assert(0);
	}
	return false;
}
P_AUITREEVIEW_ITEM CDlgFriendSeek::GetSuitItem(P_AUITREEVIEW_ITEM item, int data)
{
	P_AUITREEVIEW_ITEM pItem = item;
	while(pItem)
	{
		if (m_pTvMessage->GetItemData(pItem)==(UINT)data)
		{
			return pItem;
		}
		if ( m_pTvMessage->GetFirstChildItem(pItem) )
		{
			P_AUITREEVIEW_ITEM pItemTemp = GetSuitItem(m_pTvMessage->GetFirstChildItem(pItem), data);
			if (pItemTemp)
			{
				return pItemTemp;
			}
			else
				pItem = m_pTvMessage->GetNextSiblingItem(pItem);
		}
		else
			pItem = m_pTvMessage->GetNextSiblingItem(pItem);
	}
	return NULL;
}

P_AUITREEVIEW_ITEM CDlgFriendSeek::InsertParentMsg(MsgVector& msgVer, int index)
{
	MsgInfo msgInfo = msgVer[index];
	time_t iTime= (time_t)msgInfo.time;
	struct tm *tim = localtime( &iTime );
	P_AUITREEVIEW_ITEM pItem;
	ACHAR szText[256];
	UINT iSize = m_curInfo.acceptList.size();
	UINT index1=0;

	if (msgInfo.bInserted)
	{
		pItem = m_pTvMessage->GetFirstChildItem(m_pTvMessage->GetRootItem());
		P_AUITREEVIEW_ITEM pItemRe = GetSuitItem(pItem, msgInfo.index);
		if (pItemRe)
		{
			return pItemRe;
		}
		else
		{
			return pItem;
		}
	}

	for (UINT i=0; i<msgVer.size(); i++)
	{
		if (msgInfo.dst_index == msgVer[i].index)
		{
			switch(msgInfo.type) {
			case 0:
				a_sprintf(szText,GetStringFromTable(7229),msgInfo.rolename,
					tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday,tim->tm_hour,tim->tm_min,msgInfo.message);
				msgVer[index].bInserted = true;
				m_pTxtMsgHead->AppendText(szText);
				return m_pTvMessage->GetFirstChildItem(m_pTvMessage->GetRootItem());
			case 1:
				for(index1=0; index1<iSize; index1++)
				{
					if (m_curInfo.acceptList[index1] == msgInfo.roleid)
					{
						break;
					}
				}
				if (index1!=iSize && iSize>0)
				{
					a_sprintf(szText,GetStringFromTable(7219),GetStringFromTable(7218), msgInfo.rolename, GetStringFromTable(7221+m_iSeekingType),
						msgInfo.message,tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday,tim->tm_hour,tim->tm_min);
				}
				else
				{
					a_sprintf(szText,GetStringFromTable(7219),_AL(""),msgInfo.rolename, GetStringFromTable(7221+m_iSeekingType),
						msgInfo.message,tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday,tim->tm_hour,tim->tm_min);
				}
				break;
			case 2:
				a_sprintf(szText,GetStringFromTable(7220),msgInfo.rolename,
					msgInfo.message,tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday,tim->tm_hour,tim->tm_min);
				break;
			default:
				assert(0);
			}
			ACString strText = szText;
			FilterReturn(strText);
			pItem = m_pTvMessage->InsertItem(strText,InsertParentMsg(msgVer,i));
			m_pTvMessage->Expand(pItem, AUITREEVIEW_EXPAND_EXPAND);
			msgVer[index].bInserted = true;
			m_pTvMessage->SetItemData(pItem,msgInfo.index);
			return pItem;
		}
	}
	
	pItem = m_pTvMessage->GetFirstChildItem(m_pTvMessage->GetRootItem());
	switch(msgInfo.type) {
	case 0:
		a_sprintf(szText,GetStringFromTable(7229),msgInfo.rolename,
			tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday,tim->tm_hour,tim->tm_min,msgInfo.message);
		msgVer[index].bInserted = true;
		m_pTxtMsgHead->AppendText(szText);
		return m_pTvMessage->GetFirstChildItem(m_pTvMessage->GetRootItem());
	case 1:
		for(index1=0; index1<iSize; index1++)
		{
			if (m_curInfo.acceptList[index1] == msgInfo.roleid)
			{
				break;
			}
		}
		if (index1!=iSize && iSize>0)
		{
			a_sprintf(szText,GetStringFromTable(7219),GetStringFromTable(7218), msgInfo.rolename, GetStringFromTable(7221+m_iSeekingType),
				msgInfo.message,tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday,tim->tm_hour,tim->tm_min);
		}
		else
		{
			a_sprintf(szText,GetStringFromTable(7219),_AL(""),msgInfo.rolename, GetStringFromTable(7221+m_iSeekingType),
				msgInfo.message,tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday,tim->tm_hour,tim->tm_min);
		}
		break;
	case 2:
		a_sprintf(szText,GetStringFromTable(7220),msgInfo.rolename,
			msgInfo.message,tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday,tim->tm_hour,tim->tm_min);
		break;
	default:
		assert(0);
	}
	ACString strText = szText;
	FilterReturn(strText);
	pItem = m_pTvMessage->InsertItem(strText);
	m_pTvMessage->Expand(pItem, AUITREEVIEW_EXPAND_EXPAND);
	msgVer[index].bInserted = true;
	m_pTvMessage->SetItemData(pItem,msgInfo.index);
	return pItem;
}
//FriendInfoPub.dtf
void CDlgFriendSeek::OnCommand_PressInfo(const char * szCommand)
{
	ACString strText = m_pEdtMessage->GetText();
	if (strText.GetLength()>30)
		GetGameUIMan()->MessageBox("",GetStringFromTable(7138), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	else if (SEEKTYPE_MARIAGE == m_iSeekingType && GetHostPlayer()->GetSpouse() > 0				 || 
			 SEEKTYPE_TEACHER == m_iSeekingType && GetHostPlayer()->GetBasicProps().iLevel < 105 ||
			 SEEKTYPE_GUILD == m_iSeekingType	&& 0 == GetHostPlayer()->GetFactionID())	//Ìõ¼þ²»Âú×ã
		GetGameUIMan()->MessageBox("",GetStringFromTable(7124), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	else
	{
		ACHAR szText[30];
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(m_pPlConfig->sns_config[m_iSeekingType].id_press_msg_item, 0, 1);
		a_sprintf(szText,GetStringFromTable(7292),pItem ? pItem->GetName() : _AL("0"));
		if (pItem)
		{
			delete pItem;
		}
		GetGameUIMan()->MessageBox("Game_PressMsg",szText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
	}
}

void CDlgFriendSeek::PressInfo()
{
	ACString strText = m_pEdtMessage->GetText();
	ACHAR szText[512];
	AUI_ConvertChatString(strText, szText, false);
	strText = szText;
	GetGameUIMan()->FilterBadWords(strText);
	PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Profession");
	int iOccupation = pCombo->GetCurSel();
	unsigned char ucGender;
	pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Gender");
	ucGender = pCombo->GetCurSel();

	pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Level");
	int iLevel = pCombo->GetCurSel();
	pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_GuildLevel");
	int iFactionLevel = pCombo->GetCurSel();
	m_iLastReleaseTime = time(NULL);
	m_acSelf[m_iSeekingType].uOccupation = iOccupation-1;
	m_acSelf[m_iSeekingType].bGender = ucGender-1;
	m_acSelf[m_iSeekingType].nLevel = iLevel>0?s_nLevel[iLevel-1]:-1;
	m_acSelf[m_iSeekingType].nFactionLevel = iFactionLevel-1;
	m_strMsgLeave[m_iSeekingType] = strText;
	if (!GetGameUIMan()->m_pDlgTrade->IsShow())
	{
		GetGameSession()->c2s_CmdPressSNSMessage(m_iSeekingType, iOccupation-1, ucGender-1, iLevel>0?s_nLevel[iLevel-1]:-1, iFactionLevel-1, strText);
		Show(false);
	}

}

//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnCommand_Accept(const char * szCommand)
{
	GetGameUIMan()->MessageBox("Game_MsgAccept",GetStringFromTable(7136), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
}

//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnCommand_Refuse(const char * szCommand)
{
	GetGameUIMan()->MessageBox("Game_MsgRefuse",GetStringFromTable(7139), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
}

//all
void CDlgFriendSeek::OnCommand_CANCEL(const char * szCommand)
{
	if (m_szName == "Win_FriendSeekingMsg")
	{
		Show(false);
		GetGameUIMan()->GetDialog("Win_FriendSeeking")->Show(true);
	}
	if (m_szName == "Win_FriendSeeking")
	{
		Show(false);
		GetGameUIMan()->GetDialog("Win_FriendSeekingPerCharm")->Show(false);
	}
	else
		Show(false);
}
//FriendSeeking.dtf
void CDlgFriendSeek::SortList()
{
	int i;
	FSTableVector::iterator iter = NULL;
	BubbleSort(m_vecFSTable[m_iSeekingType].begin(), m_vecFSTable[m_iSeekingType].end(), CompareFriendSeekInfoShowOrderByFavor());
	i=0;
	iter = m_vecFSTable[m_iSeekingType].begin();
	int iSize = m_vecFSTable[m_iSeekingType].size();
	if (iSize<=5)
	{
		return;
	}
	while (i<5)
	{
		iter++;
		i++;
	}
	switch(m_iSortType) {
	case SORTTYPE_ORIG:
		BubbleSort(iter, m_vecFSTable[m_iSeekingType].end(), CompareFriendSeekInfoShowOrderByTime());
		if (iSize<=15)
		{
			return;
		}
		i=0;
		while (i<10)
		{
			iter++;
			i++;
		}
		BubbleSort(iter, m_vecFSTable[m_iSeekingType].end(), CompareFriendSeekInfoShowOrderByFavor());
		break;
	case SORTTYPE_FAVOR:
		BubbleSort(iter, m_vecFSTable[m_iSeekingType].end(), CompareFriendSeekInfoShowOrderByFavor());
		break;
	case SORTTYPE_GENDER:
		BubbleSort(iter, m_vecFSTable[m_iSeekingType].end(), CompareFriendSeekInfoShowOrderByGender());
		break;
	case SORTTYPE_FOCUS:
		BubbleSort(iter, m_vecFSTable[m_iSeekingType].end(), CompareFriendSeekInfoShowOrderByFocus());
		break;
	case SORTTYPE_ANSWER:
		BubbleSort(iter, m_vecFSTable[m_iSeekingType].end(), CompareFriendSeekInfoShowOrderByAnswer());
		break;
	case SORTTYPE_LEAVETIME:
		BubbleSort(iter, m_vecFSTable[m_iSeekingType].end(), CompareFriendSeekInfoShowOrderByTime());
		break;
	default:
		assert(0);
	}
}
//FriendSeeking.dtf
void CDlgFriendSeek::AddToList(int iPage)
{
	if (-1==iPage)
	{
		iPage = m_iCurPage;
	}
	m_pLst_All->ResetContent();
	int iSizePage = (m_vecFSTable[m_iSeekingType].size()+14)/15;
	if ( iSizePage< 2)
	{
		ACHAR szText[20];
		a_sprintf(szText,_AL("1/1"));
		m_pLabPage->SetText(szText);
		m_Btn_Head->Enable(false);
		m_Btn_Tail->Enable(false);
		m_Btn_Last->Enable(false);
		m_Btn_Next->Enable(false);
	}
	else if (m_iCurPage == 0)
	{
		ACHAR szText[20];
		a_sprintf(szText,_AL("1/%d"),iSizePage);
		m_pLabPage->SetText(szText);
		m_Btn_Head->Enable(false);
		m_Btn_Tail->Enable(true);
		m_Btn_Last->Enable(false);
		m_Btn_Next->Enable(true);
	}
	else if (m_iCurPage == iSizePage-1)
	{
		ACHAR szText[20];
		a_sprintf(szText,_AL("%d/%d"),iSizePage,iSizePage);
		m_pLabPage->SetText(szText);
		m_Btn_Head->Enable(true);
		m_Btn_Tail->Enable(false);
		m_Btn_Last->Enable(true);
		m_Btn_Next->Enable(false);
	}
	else
	{
		ACHAR szText[20];
		a_sprintf(szText,_AL("%d/%d"),m_iCurPage+1,iSizePage);
		m_pLabPage->SetText(szText);
		m_Btn_Head->Enable(true);
		m_Btn_Tail->Enable(true);
		m_Btn_Last->Enable(true);
		m_Btn_Next->Enable(true);
	}
	FSTableVector::iterator iter = m_vecFSTable[m_iSeekingType].begin();
	int i=0;
	for ( i=0; i<iPage*15; i++ )
	{
		iter++;
		if (iter == m_vecFSTable[m_iSeekingType].end())
		{
			assert(0);
			return;
		}
	}
	for ( i=0; i<SNSINFORMATION_NUMONEPAGE; i++)
	{
		if (iter == m_vecFSTable[m_iSeekingType].end())
		{
			break;
		}
		FriendSeekTableInfo fstInfo = *iter++;
		if (i==0)
		{
			m_iMsgRoleID = fstInfo.nRoleID;
		}
		ACString strText;
		ACHAR szTime[20];
		int iRemainTime = fstInfo.nPressTime + 7*24*3600 - g_pGame->GetServerGMTTime();
		if (iRemainTime > 24*3600)
		{
			a_sprintf(szTime, GetStringFromTable(1610),iRemainTime/(24*3600), (iRemainTime%(24*3600))/3600);
		}
		else if (iRemainTime > 3600)
		{
			a_sprintf(szTime, GetStringFromTable(1611),iRemainTime/3600, (iRemainTime%3600)/60);
		}
		else if (iRemainTime > 60)
		{
			a_sprintf(szTime, GetStringFromTable(1609),iRemainTime/60, (iRemainTime%60));
		}
		else if (iRemainTime > 0)
		{
			a_sprintf(szTime, GetStringFromTable(1601),iRemainTime);
		}
		else
			a_sprintf(szTime, _AL("error"));
		if (iPage ==0 && i<5)
		{
			strText.Format(_AL("%s\t%d\t%s\t%d\t%d\t%s\t"),	GetStringFromTable(7137)+fstInfo.strName, fstInfo.nFavor, 
				GetStringFromTable(fstInfo.bGender ? 7142 : 7141), fstInfo.nFocus, fstInfo.nAnswer, szTime);
			m_pLst_All->AddString(strText);
			m_pLst_All->SetItemTextColor(i,A3DCOLORRGB(255, 0, 0));
		}
		else
		{
			strText.Format(_AL("%s\t%d\t%s\t%d\t%d\t%s\t"),	fstInfo.strName, fstInfo.nFavor, 
				GetStringFromTable(fstInfo.bGender ? 7142 : 7141), fstInfo.nFocus, fstInfo.nAnswer, szTime);
			m_pLst_All->AddString(strText);
			if (fstInfo.charm_player>0)
			{
				m_pLst_All->SetItemTextColor(i,A3DCOLORRGB(0,255,0));
			}
			else
				m_pLst_All->SetItemTextColor(i,A3DCOLORRGB(22,88,177));
		}
	}
	OnEventLButtonDown_Lst_ListInfo(0,0,NULL);
}

//FriendSeekingMsg.dtf
void CDlgFriendSeek::AddVoteList(int iPage)
{
	int iP;
	if (iPage<0)
	{
		iP = m_iCurMsgPage;
	}
	else
		iP = iPage;
	m_pLst_All->ResetContent();
	int iSizePage = (m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType].size()+14)/15;
	if ( iSizePage< 2)
	{
		ACHAR szText[20];
		a_sprintf(szText,_AL("1/1"));
		m_pLabPage->SetText(szText);
		m_Btn_Head->Enable(false);
		m_Btn_Tail->Enable(false);
		m_Btn_Last->Enable(false);
		m_Btn_Next->Enable(false);
	}
	else if (m_iCurMsgPage == 0)
	{
		ACHAR szText[20];
		a_sprintf(szText,_AL("1/%d"),iSizePage);
		m_pLabPage->SetText(szText);
		m_Btn_Head->Enable(false);
		m_Btn_Tail->Enable(true);
		m_Btn_Last->Enable(false);
		m_Btn_Next->Enable(true);
	}
	else if (m_iCurMsgPage == iSizePage-1)
	{
		ACHAR szText[20];
		a_sprintf(szText,_AL("%d/%d"),iSizePage,iSizePage);
		m_pLabPage->SetText(szText);
		m_Btn_Head->Enable(true);
		m_Btn_Tail->Enable(false);
		m_Btn_Last->Enable(true);
		m_Btn_Next->Enable(false);
	}
	else
	{
		ACHAR szText[20];
		a_sprintf(szText,_AL("%d/%d"),m_iCurMsgPage+1,iSizePage);
		m_pLabPage->SetText(szText);
		m_Btn_Head->Enable(true);
		m_Btn_Tail->Enable(true);
		m_Btn_Last->Enable(true);
		m_Btn_Next->Enable(true);
	}


	ApplyInfoVector::iterator iter = m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType].begin();
	int i;
	for ( i=0; i<iP*15; i++ )
	{
		iter++;
		if (iter == m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType].end())
		{
			assert(0);
			return;
		}
	}
	for (i=0; i<SNSINFORMATION_NUMONEPAGE; i++)
	{
		if (iter == m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType].end())
		{
			return;
		}
		ApplyInfo applyInfo = *iter++;
		if (i==0)
		{
			m_iApplyRoleID = applyInfo.nRoleid;
		}
		ACString strText;
		strText.Format(_AL("%s\t%d\t%s\t%d\t"),	applyInfo.strName, applyInfo.nCount,
			GetStringFromTable(7141+applyInfo.nGender), applyInfo.nLevel);
		m_pLst_All->AddString(strText);
	}
}

void CDlgFriendSeek::AddTreeList()
{
	m_pTvMessage->DeleteAllItems();
	m_pTxtMsgHead->SetText(_AL(""));
	P_AUITREEVIEW_ITEM pItem = m_pTvMessage->GetRootItem();
	for (UINT i=0; i<m_mapMsgCache[m_iSeekingType][m_iMsgID].msgVector.size(); i++)
	{
		MsgInfo msgInfo = m_mapMsgCache[m_iSeekingType][m_iMsgID].msgVector[i];
		if (msgInfo.bInserted)
		{
			continue;
		}
		else
			InsertParentMsg(m_mapMsgCache[m_iSeekingType][m_iMsgID].msgVector,i);
	};
}

//AskingForMarriage.dtf
void CDlgFriendSeek::OnCommand_ConfirmMarriage(const char * szCommand)
{
	if (GetHostPlayer()->GetMoneyAmount() < m_pPlConfig->sns_config[m_iSeekingType].fee_apply_msg)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(656), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if (-1==m_pPlConfig->sns_config[0].id_apply_msg_item ||
		GetHostPlayer()->GetPack()->GetItemTotalNum(m_pPlConfig->sns_config[0].id_apply_msg_item) >= 1)
	{
		ACString szMsg;
		PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Edt_Message");
		szMsg = pEdit->GetText();
		ACHAR szText[512];
		AUI_ConvertChatString(szMsg, szText, false);
		szMsg = szText;
		GetGameUIMan()->FilterBadWords(szMsg);
		GetGameSession()->c2s_CmdApplySNSMessage(0, m_iMsgID, szMsg);
		Show(false);
	}
	else
		GetGameUIMan()->MessageBox("",GetStringFromTable(7181), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
}

//AskingForFriend.dtf
void CDlgFriendSeek::OnCommand_ConfirmFriend(const char * szCommand)
{
	if (GetHostPlayer()->GetMoneyAmount() < m_pPlConfig->sns_config[m_iSeekingType].fee_apply_msg)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(656), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if (-1==m_pPlConfig->sns_config[1].id_apply_msg_item ||
		GetHostPlayer()->GetPack()->GetItemTotalNum(m_pPlConfig->sns_config[1].id_apply_msg_item) >= 1)
	{
		ACString szMsg;
		PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Edt_Message");
		szMsg = pEdit->GetText();
		ACHAR szText[512];
		AUI_ConvertChatString(szMsg, szText, false);
		szMsg = szText;
		GetGameUIMan()->FilterBadWords(szMsg);
		GetGameSession()->c2s_CmdApplySNSMessage(1, m_iMsgID, szMsg);
		Show(false);
	}
	else
		GetGameUIMan()->MessageBox("",GetStringFromTable(7183), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
}

//AskingForTeacher.dtf
void CDlgFriendSeek::OnCommand_ConfirmTeacher(const char * szCommand)
{
	if (GetHostPlayer()->GetMoneyAmount() < m_pPlConfig->sns_config[m_iSeekingType].fee_apply_msg)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(656), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if (-1==m_pPlConfig->sns_config[2].id_apply_msg_item ||
		GetHostPlayer()->GetPack()->GetItemTotalNum(m_pPlConfig->sns_config[2].id_apply_msg_item) >= 1)
	{
		ACString szMsg;
		PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Edt_Message");
		szMsg = pEdit->GetText();
		ACHAR szText[512];
		AUI_ConvertChatString(szMsg, szText, false);
		szMsg = szText;
		GetGameUIMan()->FilterBadWords(szMsg);
		GetGameSession()->c2s_CmdApplySNSMessage(2, m_iMsgID, szMsg);
		Show(false);
	}
	else
		GetGameUIMan()->MessageBox("",GetStringFromTable(7185), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
}

//AskingForGuild.dtf
void CDlgFriendSeek::OnCommand_ConfirmGuild(const char * szCommand)
{
	if (GetHostPlayer()->GetMoneyAmount() < m_pPlConfig->sns_config[m_iSeekingType].fee_apply_msg)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(656), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if (-1==m_pPlConfig->sns_config[3].id_apply_msg_item ||
		GetHostPlayer()->GetPack()->GetItemTotalNum(m_pPlConfig->sns_config[3].id_apply_msg_item) >= 1)
	{
		ACString szMsg;
		PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Edt_Message");
		szMsg = pEdit->GetText();
		ACHAR szText[512];
		AUI_ConvertChatString(szMsg, szText, false);
		szMsg = szText;
		GetGameUIMan()->FilterBadWords(szMsg);
		GetGameSession()->c2s_CmdApplySNSMessage(3, m_iMsgID, szMsg);
		Show(false);
	}
	else
		GetGameUIMan()->MessageBox("",GetStringFromTable(7187), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
}

//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnCommand_SendBack(const char * szCommand)
{
// 	if (!m_bCanSend)
//	{
//		GetGameUIMan()->MessageBox("",GetStringFromTable(7291), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
//		return;
//	}
	if (m_pTvMessage->GetSelectedItem())
	{
		m_iMsgIDDst = m_pTvMessage->GetItemData(m_pTvMessage->GetSelectedItem(),"");
	}
	else
		m_iMsgIDDst = -1;
	ACString strText = m_pEdtAnswer->GetText();
	ACHAR szText[512];
	AUI_ConvertChatString(strText, szText, false);
	m_strMsg = szText;
	FilterReturn(m_strMsg);
	if (m_strMsg.GetLength()>30)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(7138), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	GetGameUIMan()->FilterBadWords(m_strMsg);
	if (GetHostPlayer()->GetMoneyAmount() < m_pPlConfig->sns_config[m_iSeekingType].fee_response_msg)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(7191), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
	{
		ACHAR szText[30];
		ACHAR szText1[256];
		CECIvtrItem::GetPriceText(szText, m_pPlConfig->sns_config[m_iSeekingType].fee_response_msg);
		a_sprintf(szText1,GetStringFromTable(7190),szText);
		GetGameUIMan()->MessageBox("Game_SendBackMsg",szText1, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
	}
}

//FriendSeeking.dtf
void CDlgFriendSeek::OnEventLButtonDown_Lst_ListInfo(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	m_iCurIndex = m_pLst_All->GetCurSel();
	if (m_iCurIndex >= m_pLst_All->GetCount())
	{
		m_iCurIndex = -1;
		PAUITEXTAREA pTxtInfo = (PAUITEXTAREA)GetDlgItem("Txt_SelfIntro");
		pTxtInfo->SetText(_AL(""));
		pTxtInfo = (PAUITEXTAREA)GetDlgItem("Txt_1");
		pTxtInfo->SetText(_AL(""));
		pTxtInfo = (PAUITEXTAREA)GetDlgItem("Txt_2");
		pTxtInfo->SetText(_AL(""));
		for (int i=0; i<10; i++)
		{
			char szName[20];
			sprintf(szName,"Img_Star%d",i+1);
			PAUIOBJECT pObj = GetDlgItem(szName);
			if (pObj)
			{
				pObj->Show(false);
			}
		}
		return;
	}
	bool bSuit = false;
	CECHostPlayer* pHostPlayer = GetHostPlayer();
	FriendSeekTableInfo curTable = m_vecFSTable[m_iSeekingType][m_iCurPage*15 + m_iCurIndex];
	int idFaction = pHostPlayer->GetFactionID();
	if ((255 != curTable.aCondiction.nFactionLevel && (idFaction==0 ||curTable.aCondiction.nFactionLevel>GetGame()->GetFactionMan()->GetFaction(idFaction)->GetLevel()))||
		(255 != curTable.aCondiction.bGender && curTable.aCondiction.bGender != (pHostPlayer->GetGender()!=0) )||
		(255 != curTable.aCondiction.nLevel && curTable.aCondiction.nLevel> pHostPlayer->GetBasicProps().iLevel) ||
		(255 != curTable.aCondiction.uOccupation && !CheckProfression(curTable.aCondiction.uOccupation) ))
	{
		bSuit = false;
	}
	else
	{
		bSuit = true;
	}
	PAUITEXTAREA pTxtApply = (PAUITEXTAREA)GetDlgItem("Txt_2");
	pTxtApply->SetText(_AL(""));
	pTxtApply->ScrollToTop();
	ACHAR szText[256];
	AUICTranslate trans;
	FriendSeekTableInfo curInfo = m_vecFSTable[m_iSeekingType][m_iCurPage*15+m_iCurIndex];
	if (!bSuit)
	{
		pTxtApply->AppendText(_AL("^FF0000"));
	}
	a_sprintf(szText, GetStringFromTable(7208),	GetStringFromTable((char)curInfo.aCondiction.uOccupation+7163),
		GetStringFromTable((char)curInfo.aCondiction.bGender+7141));
	pTxtApply->AppendText(szText);
	a_sprintf(szText, GetStringFromTable(7209),	GetStringFromTable((char)curInfo.aCondiction.nLevel==-1 ? 7143 : curInfo.aCondiction.nLevel/15+7142),
		GetStringFromTable((char)curInfo.aCondiction.nFactionLevel+7154));
	pTxtApply->AppendText(szText);
	PAUITEXTAREA pTxtSelfIntro = (PAUITEXTAREA)GetDlgItem("Txt_SelfIntro"); 
	pTxtSelfIntro->SetText(curInfo.strMsg);

	int charm = curInfo.charm_player;
	pObj = GetDlgItem("Edt_BackStar");
	a_sprintf(szText,_AL("%d"),curInfo.charmstar_player);
	pObj->SetHint(szText);
	int i;
	for (i=0; i<charm; i++)
	{
		char szName[20];
		sprintf(szName,"Img_Star%d",i+1);
		PAUIOBJECT pObj = GetDlgItem(szName);
		if (pObj)
		{
			pObj->Show(true);
			pObj->SetHint(szText);
		}
	}
	for ( i=charm; i<10; i++)
	{
		char szName[20];
		sprintf(szName,"Img_Star%d",i+1);
		PAUIOBJECT pObj = GetDlgItem(szName);
		if (pObj)
		{
			pObj->Show(false);
		}
	}
	

	m_iMsgID = curInfo.nMsgID;
	m_iMsgRoleID = curInfo.nRoleID;
	MsgID2PlayerInfoMap::iterator iter = m_id2PlayerInfoCache.find(m_iMsgRoleID);
	if(iter == m_id2PlayerInfoCache.end())
	{
		m_iPlayerInfoType = 0;
		GetGameSession()->sns_get_playerinfo(0,m_vecFSTable[m_iSeekingType][m_iCurPage*15 + m_iCurIndex].nRoleID);
	}
	else
	{
		PlayerInfoCache playerInfoCache = m_id2PlayerInfoCache[m_iMsgRoleID];
		PAUITEXTAREA pTxtInfo = (PAUITEXTAREA)GetDlgItem("Txt_1");
		pTxtInfo->SetText(_AL(""));
		pTxtInfo->ScrollToTop();
		ACHAR szText[256];
		ACString strText;

		a_sprintf(szText, GetStringFromTable(7201),GetStringFromTable(playerInfoCache.roleinfo.gender>0 ? 7142 : 7141));
		strText = szText;
		a_sprintf(szText, GetStringFromTable(playerInfoCache.roleinfo.sub_level>0? 7011:7200),playerInfoCache.roleinfo.level);
		strText = strText + szText;
		pTxtInfo->AppendText(strText);
		a_sprintf(szText, GetStringFromTable(7203),GetStringFromTable(playerInfoCache.roleinfo.spouse>0 ? 7160 : 7161));
		strText = szText;
		a_sprintf(szText, GetStringFromTable(7202),GetStringFromTable(PROFESSION_START + playerInfoCache.roleinfo.occupation));
		strText += szText;
		pTxtInfo->AppendText(strText);

		if (playerInfoCache.roleinfo.sectname.IsEmpty() || playerInfoCache.roleinfo.sectname == playerInfoCache.playerinfo.rolename)
			a_sprintf(szText, GetStringFromTable(7204),GetStringFromTable(7199));
		else
			a_sprintf(szText, GetStringFromTable(7204),playerInfoCache.roleinfo.sectname);
		pTxtInfo->AppendText(szText);
		if (playerInfoCache.roleinfo.familyname.IsEmpty())
			a_sprintf(szText, GetStringFromTable(7205),GetStringFromTable(7199));
		else
			a_sprintf(szText, GetStringFromTable(7205),playerInfoCache.roleinfo.familyname);
		pTxtInfo->AppendText(szText);
		if (playerInfoCache.roleinfo.factionname.IsEmpty())
			a_sprintf(szText, GetStringFromTable(7206),GetStringFromTable(7199));
		else
			a_sprintf(szText, GetStringFromTable(7206),playerInfoCache.roleinfo.factionname);
		pTxtInfo->AppendText(szText);
	}
}

//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnEventLButtonDown_Lst_ListMsg(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	m_iCurMsgIndex = m_pLst_All->GetCurSel();
	if (m_iCurMsgIndex >= m_pLst_All->GetCount())
	{
		m_iCurMsgIndex = -1;
		return;
	}
	m_iApplyRoleID = m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[m_iMsgType][m_iCurMsgPage*15+m_iCurMsgIndex].nRoleid;
}

//FriendSeekingMsg.dtf
//FriendSeeking.dtf
void CDlgFriendSeek::OnEventLButtonDown_Lst_ListInformation(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (m_iCurIndex>=0)
		OnCommand_Msg("");
}

//FriendSeeking.dtf
void CDlgFriendSeek::OnPrtc_SNSPressMessageRe(GNET::Protocol* pProtocol)
{
	GNET::SNSPressMessage_Re* p = static_cast<GNET::SNSPressMessage_Re*>(pProtocol);
	if (p->retcode == ERROR_SUCCESS)
	{
		m_iSeekingType = p->message_type;
		m_iSortType = SORTTYPE_ORIG;
		FriendSeekTableInfo fstInfo;
		fstInfo.nMsgID = p->message_id;
		fstInfo.nRoleID = p->roleid;
		fstInfo.strName = GetHostPlayer()->GetName();
		fstInfo.nFavor = 0;
		fstInfo.bGender = GetHostPlayer()->GetGender()>0 ? true : false;
		fstInfo.nFocus = 0;
		fstInfo.nAnswer = 0;
		fstInfo.charm_player = 0;
		fstInfo.charmstar_player = 0;
		fstInfo.nItemType = ITEMTYPE_OTHER;
		fstInfo.strMsg = m_strMsgLeave[p->message_type];
		fstInfo.nPressTime = m_iLastReleaseTime;
		fstInfo.aCondiction.uOccupation = m_acSelf[p->message_type].uOccupation;
		fstInfo.aCondiction.bGender = m_acSelf[p->message_type].bGender;
		fstInfo.aCondiction.nLevel = m_acSelf[p->message_type].nLevel;
		fstInfo.aCondiction.nFactionLevel = m_acSelf[p->message_type].nFactionLevel;
		m_vecFSTable[m_iSeekingType].push_back(fstInfo);
		if (m_vecFSTable[m_iSeekingType].size()>0)
		{
			m_iMsgID = m_vecFSTable[m_iSeekingType][0].nMsgID;
			SortList();
			AddToList(m_iCurPage);
		}
		GetGameUIMan()->AddChatMessage(GetStringFromTable(7302), GP_CHAT_MISC);
	}
	else
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(7230+p->retcode), GP_CHAT_MISC);
	}
}
//FriendSeeking.dtf
void CDlgFriendSeek::OnPrtc_SNSCancelMessageRe(GNET::Protocol* pProtocol)
{
	GNET::SNSCancelMessage_Re* p = static_cast<GNET::SNSCancelMessage_Re*>(pProtocol);
	if (p->retcode == ERROR_SUCCESS)
	{
		for (unsigned int i=0; i<m_vecFSTable[p->message_type].size(); i++)
		{
			FriendSeekTableInfo fstInfo = m_vecFSTable[p->message_type][i];
			if (fstInfo.nMsgID == p->message_id)
			{
				m_vecFSTable[p->message_type].erase(&(m_vecFSTable[p->message_type][i]));
				SortList();
				AddToList();
				return;
			}
		}
	}
	else
		GetGameUIMan()->AddChatMessage(GetStringFromTable(7230+p->retcode), GP_CHAT_MISC);
}

//FriendSeeking.dtf
void CDlgFriendSeek::OnPrtc_SNSApplyRe(GNET::Protocol* pProtocol)
{
	GNET::SNSApply_Re* p = static_cast<GNET::SNSApply_Re*>(pProtocol);
	if (p->retcode == ERROR_SUCCESS)
	{
		if (p->oper)
		{
			CDlgFriendSeek *pFriendSeekMsg = (CDlgFriendSeek*)GetGameUIMan()->GetDialog("Win_FriendSeekingMsg");
			pFriendSeekMsg->InsertMessage(p->index, p->dst_index);
		}
		else
		{
			ApplyInfo applyinfo;
			applyinfo.nRoleid = GetHostPlayer()->GetCharacterID();
			applyinfo.strName = GetHostPlayer()->GetName();
			applyinfo.nCount = 0;
			applyinfo.nLastTime = 0;
			applyinfo.nLevel = 0;
			applyinfo.nGender = GetHostPlayer()->GetGender();
			UINT i;
			for (i=0; i<m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[0].size(); i++)
			{
				if (m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[0][i].nRoleid == applyinfo.nRoleid )
				{
					break;
				}
			}
			if (i==m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[0].size())
			{
				m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[0].push_back(applyinfo);
			}
			GetGameUIMan()->AddChatMessage(GetStringFromTable(7304), GP_CHAT_MISC);
		}
	}
	else
		GetGameUIMan()->AddChatMessage(GetStringFromTable(7230+p->retcode), GP_CHAT_MISC);
}
//FriendSeeking.dtf
void CDlgFriendSeek::OnPrtc_SNSListMessageRe(GNET::Protocol* pProtocol)
{
	GNET::SNSListMessage_Re* p = static_cast<GNET::SNSListMessage_Re*>(pProtocol);
	if (p->retcode != ERROR_SUCCESS)
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(7230+p->retcode), GP_CHAT_MISC);
		return;
	}
	m_iSeekingType = p->message_type;
	m_vecFSTable[p->message_type].clear();
	unsigned int i;
	for (i=0; i < p->lists.size(); i++)
	{
		FriendSeekTableInfo fstInfo;
		fstInfo.nMsgID = p->lists[i].message_id;
		fstInfo.nRoleID = p->lists[i].roleid;
		ACString strName = ACString((const ACHAR*)p->lists[i].rolename.begin(), p->lists[i].rolename.size() / sizeof(ACHAR));
		fstInfo.strName = strName;
		fstInfo.nFavor = p->lists[i].charm;
		fstInfo.bGender = p->lists[i].gender>0 ? true : false;
		fstInfo.nFocus = p->lists[i].attention;
		fstInfo.nAnswer = p->lists[i].apply_count;
		fstInfo.charmstar_player = p->lists[i].charmstar_player;
		int charm = 0;
		while (20000*(charm*charm*charm+4*charm)<fstInfo.charmstar_player)
		{
			charm++;
		}
		charm--;
		if (charm<0)
		{
			charm = 0;
		}
		fstInfo.charm_player = charm;
		fstInfo.nItemType = ITEMTYPE_OTHER;
		strName = ACString((const ACHAR*)p->lists[i].message.begin(), p->lists[i].message.size() / sizeof(ACHAR));
		fstInfo.strMsg = strName;
		fstInfo.nPressTime = p->lists[i].press_time;
		fstInfo.aCondiction.uOccupation = p->lists[i].condition.occupation;
		fstInfo.aCondiction.bGender = p->lists[i].condition.gender;
		fstInfo.aCondiction.nLevel = p->lists[i].condition.level;
		fstInfo.aCondiction.nFactionLevel = p->lists[i].condition.faction_level;
		fstInfo.acceptList.clear();
		for (UINT index1=0; index1<p->lists[i].accept_list.size(); index1++)
		{
			fstInfo.acceptList.push_back(p->lists[i].accept_list[index1]);
		}
		m_vecFSTable[p->message_type].push_back(fstInfo);
	}
	ACHAR szText[20];
	a_sprintf(szText,_AL("%d/%d"),1,(p->lists.size()+14)/15+1);
	m_pLabPage->SetText(szText);
	SortList();
	AddToList(m_iCurPage);
	if (p->lists.size()>0)
	{
		OnEventLButtonDown_Lst_ListInfo(0,0,NULL);
	}
}

//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnPrtc_SNSGetMessageRe(GNET::Protocol* pProtocol)
{
	GNET::SNSGetMessage_Re* p = static_cast<GNET::SNSGetMessage_Re*>(pProtocol);
	if (p->retcode != ERROR_SUCCESS)
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(7230+p->retcode), GP_CHAT_MISC);
		return;
	}
	unsigned int i=0;
	m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[0].clear();
	while (i<p->apply_list.size())
	{
		ApplyInfo applyinfo;
		applyinfo.nRoleid = p->apply_list[i].roleid;
		applyinfo.strName = ACString((const ACHAR*)p->apply_list[i].rolename.begin(), p->apply_list[i].rolename.size() / sizeof(ACHAR));
		applyinfo.nCount = p->apply_list[i].count;
		applyinfo.nLastTime = p->apply_list[i].last_time;
		applyinfo.nLevel = p->apply_list[i].level;
		applyinfo.nGender = p->apply_list[i].gender;
		m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[0].push_back(applyinfo);
		i++;
	}

	i=0;
	m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[1].clear();
	while (i<p->agree_list.size())
	{
		ApplyInfo applyinfo;
		applyinfo.nRoleid = p->agree_list[i].roleid;
		applyinfo.strName = ACString((const ACHAR*)p->agree_list[i].rolename.begin(), p->agree_list[i].rolename.size() / sizeof(ACHAR));
		applyinfo.nCount = p->agree_list[i].count;
		applyinfo.nLastTime = p->agree_list[i].last_time;
		applyinfo.nLevel = p->agree_list[i].level;
		applyinfo.nGender = p->agree_list[i].gender;
		m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[1].push_back(applyinfo);
		i++;
	}

	i=0;
	m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[2].clear();
	while (i<p->deny_list.size())
	{
		ApplyInfo applyinfo;
		applyinfo.nRoleid = p->deny_list[i].roleid;
		applyinfo.strName = ACString((const ACHAR*)p->deny_list[i].rolename.begin(), p->deny_list[i].rolename.size() / sizeof(ACHAR));
		applyinfo.nCount = p->deny_list[i].count;
		applyinfo.nLastTime = p->deny_list[i].last_time;
		applyinfo.nLevel = p->deny_list[i].level;
		applyinfo.nGender = p->deny_list[i].gender;
		m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[2].push_back(applyinfo);
		i++;
	}

	i=0;
	m_mapMsgCache[m_iSeekingType][m_iMsgID].msgVector.clear();
	while (i<p->leave_msgs.size())
	{
		MsgInfo msgInfo;
		msgInfo.index = p->leave_msgs[i].index;
		msgInfo.dst_index = p->leave_msgs[i].dst_index;
		msgInfo.type = p->leave_msgs[i].type;
		msgInfo.roleid = p->leave_msgs[i].roleid;
		msgInfo.rolename = ACString((const ACHAR*)p->leave_msgs[i].rolename.begin(), p->leave_msgs[i].rolename.size() / sizeof(ACHAR));
		msgInfo.time = p->leave_msgs[i].time;
		msgInfo.message = ACString((const ACHAR*)p->leave_msgs[i].message.begin(), p->leave_msgs[i].message.size() / sizeof(ACHAR));
		msgInfo.bInserted = false;
		m_mapMsgCache[m_iSeekingType][m_iMsgID].msgVector.push_back(msgInfo);
		i++;
	}
	AddVoteList();
	OnEventLButtonDown_Lst_ListMsg(0,0,NULL);
	AddTreeList();
//	OnCommand_RdoType("Rdo11");
	m_bCanSend = false;
	int idPlayer = GetHostPlayer()->GetCharacterID();
	if ( idPlayer == m_iMsgRoleID)
	{
		m_bCanSend = true;
	}
	else
	{
		int iSize = m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[0].size();
		int i;
		for (i=0; i<iSize; i++)
		{
			if (idPlayer == m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[0][i].nRoleid)
			{
				m_bCanSend = true;
			}
		}
		iSize = m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[1].size();
		for ( i=0; i<iSize; i++)
		{
			if (idPlayer == m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[1][i].nRoleid)
			{
				m_bCanSend = true;
			}
		}
		iSize = m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[2].size();
		for (i=0; i<iSize; i++)
		{
			if (idPlayer == m_mapMsgCache[m_iSeekingType][m_iMsgID].vecList[2][i].nRoleid)
			{
				m_bCanSend = true;
			}
		}
	}
	if (!IsShow())
	{
		Show(true);
	}
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_FriendSeeking");
	if (pDlg->IsShow())
	{
		pDlg->Show(false);
		GetGameUIMan()->GetDialog("Win_FriendSeekingPerCharm")->Show(false);
	}
}

//FriendSeeking.dtf
void CDlgFriendSeek::OnPrtc_SNSVoteRe(GNET::Protocol* pProtocol)
{
	GNET::SNSVote_Re* p = static_cast<GNET::SNSVote_Re*>(pProtocol);
	if (p->retcode != ERROR_SUCCESS)
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(7230+p->retcode), GP_CHAT_MISC);
		return;
	}
	GetGameUIMan()->AddChatMessage(GetStringFromTable(7303), GP_CHAT_MISC);

	ApplyInfo applyinfo;
	applyinfo.nRoleid = GetHostPlayer()->GetCharacterID();
	applyinfo.strName = GetHostPlayer()->GetName();
	applyinfo.nCount = 0;
	applyinfo.nLastTime = 0;
	applyinfo.nLevel = 0;
	applyinfo.nGender = GetHostPlayer()->GetGender();
	if (p->oper)
		m_mapMsgCache[p->message_type][p->message_id].vecList[1].push_back(applyinfo);
	else
		m_mapMsgCache[p->message_type][p->message_id].vecList[2].push_back(applyinfo);

	for (UINT i=0; i<m_vecFSTable[m_iSeekingType].size(); i++)
	{
		if (m_vecFSTable[m_iSeekingType][i].nMsgID == p->message_id)
		{
			if (p->oper)
				m_vecFSTable[m_iSeekingType][i].nFavor++;
			else
				m_vecFSTable[m_iSeekingType][i].nFavor--;
			if (m_vecFSTable[m_iSeekingType][i].nFavor<0)
				m_vecFSTable[m_iSeekingType][i].nFavor=0;
			else if (m_vecFSTable[m_iSeekingType][i].nFavor>9999)
				m_vecFSTable[m_iSeekingType][i].nFavor=9999;
			m_vecFSTable[m_iSeekingType][i].nFocus++;
			//AddToList();
		}
	}
}
//FriendSeeking.dtf
//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnPrtc_SNSGetPlayerInfoRe(GNET::Protocol* pData)
{
	GNET::SNSGetPlayerInfo_Re* p = static_cast<GNET::SNSGetPlayerInfo_Re*>(pData);
	if (p->retcode != ERROR_SUCCESS)
	{
		if (p->retcode == 1)
		{
			GetGameUIMan()->MessageBox("",GetStringFromTable(8177), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
		else
			GetGameUIMan()->AddChatMessage(GetStringFromTable(7230+p->retcode), GP_CHAT_MISC);
		return;
	}
	PlayerInfoCache playerInfoCache;

	playerInfoCache.roleinfo.level			= p->info.roleinfo.level;
	playerInfoCache.roleinfo.sub_level		= p->info.roleinfo.sub_level;
	playerInfoCache.roleinfo.gender			= p->info.roleinfo.gender;
	playerInfoCache.roleinfo.occupation		= p->info.roleinfo.occupation;
	playerInfoCache.roleinfo.spouse			= p->info.roleinfo.spouse;
	playerInfoCache.roleinfo.sectname		= ACString((const ACHAR*)p->info.roleinfo.sectname.begin(), p->info.roleinfo.sectname.size() / sizeof(ACHAR));
	playerInfoCache.roleinfo.familyname		= ACString((const ACHAR*)p->info.roleinfo.familyname.begin(), p->info.roleinfo.familyname.size() / sizeof(ACHAR));
	playerInfoCache.roleinfo.factionname	= ACString((const ACHAR*)p->info.roleinfo.factionname.begin(), p->info.roleinfo.factionname.size() / sizeof(ACHAR));
	playerInfoCache.roleinfo.faction_level	= p->info.roleinfo.faction_level;

	playerInfoCache.playerinfo.roleid		= p->info.playerinfo.roleid;
	playerInfoCache.playerinfo.is_private	= p->info.playerinfo.is_private;
	playerInfoCache.playerinfo.rolename		= ACString((const ACHAR*)p->info.playerinfo.rolename.begin(), p->info.playerinfo.rolename.size() / sizeof(ACHAR));
	playerInfoCache.playerinfo.nickname		= ACString((const ACHAR*)p->info.playerinfo.nickname.begin(), p->info.playerinfo.nickname.size() / sizeof(ACHAR));
	playerInfoCache.playerinfo.birthday		= p->info.playerinfo.birthday;
	playerInfoCache.playerinfo.age			= p->info.playerinfo.age;
	playerInfoCache.playerinfo.province		= p->info.playerinfo.province;
	playerInfoCache.playerinfo.city			= p->info.playerinfo.city;
	playerInfoCache.playerinfo.career		= ACString((const ACHAR*)p->info.playerinfo.career.begin(), p->info.playerinfo.career.size() / sizeof(ACHAR));
	playerInfoCache.playerinfo.address		= ACString((const ACHAR*)p->info.playerinfo.address.begin(), p->info.playerinfo.address.size() / sizeof(ACHAR));
	playerInfoCache.playerinfo.introduce	= ACString((const ACHAR*)p->info.playerinfo.introduce.begin(), p->info.playerinfo.introduce.size() / sizeof(ACHAR));;
	playerInfoCache.playerinfo.charmvec[0]	= p->info.playerinfo.charm_personal;
	playerInfoCache.playerinfo.charmvec[1]	= p->info.playerinfo.charm_friend;
	playerInfoCache.playerinfo.charmvec[2]	= p->info.playerinfo.charm_sect;
	playerInfoCache.playerinfo.charmvec[3]	= p->info.playerinfo.charm_faction;
	playerInfoCache.playerinfo.press_time	= p->info.playerinfo.press_time;

	m_id2PlayerInfoCache[p->info.playerinfo.roleid] = playerInfoCache;
	if (p->info.playerinfo.roleid == GetHostPlayer()->GetCharacterID())
	{
		m_id2PlayerInfoCache[GetHostPlayer()->GetCharacterID()] = playerInfoCache;
		CDlgSnsInfo *pDlgSnsInfo = (CDlgSnsInfo *)GetGameUIMan()->GetDialog("Win_PersInfo");
		pDlgSnsInfo->SetSelfInfo(playerInfoCache, pDlgSnsInfo->IsShow());
		CDlgFriendSeek *pDlgPersCenter = (CDlgFriendSeek*)GetGameUIMan()->GetDialog("Win_FriendSeekingPerCenter");
		if (pDlgPersCenter->IsShow())
		{
			pDlgPersCenter->OnShowDialog();

		}
	}
	else if (GetGameUIMan()->GetDialog("Win_EPEquip")->IsShow())
	{
		if (playerInfoCache.playerinfo.is_private)
		{
			GetGameUIMan()->MessageBox("",GetStringFromTable(8177), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
		else
		{
			GetGameUIMan()->GetDialog("Win_EPEquip")->Show(false);
			CDlgSnsInfo *pDlgSnsInfo = (CDlgSnsInfo *)GetGameUIMan()->GetDialog("Win_PersInfoCheck");
			pDlgSnsInfo->SetInfo(playerInfoCache, true);
		}
	}

	if (m_bShowInfo)
	{
		CDlgSnsInfo *pDlgSnsInfo = (CDlgSnsInfo *)GetGameUIMan()->GetDialog("Win_PersInfoCheck");
		pDlgSnsInfo->SetInfo(playerInfoCache, true);
		m_bShowInfo = false;
	}

	//FriendSeeking.dtf
	if (0==m_iPlayerInfoType )
	{
		CDlgFriendSeek *pDlgFriendSeeking = (CDlgFriendSeek *)(GetGameUIMan()->GetDialog("Win_FriendSeeking"));
		PAUITEXTAREA pTxtInfo = (PAUITEXTAREA)pDlgFriendSeeking->GetDlgItem("Txt_1");
		pTxtInfo->SetText(_AL(""));
		pTxtInfo->ScrollToTop();
		ACHAR szText[256];
		ACString strText;

		a_sprintf(szText, GetStringFromTable(7201),GetStringFromTable(playerInfoCache.roleinfo.gender>0 ? 7142 : 7141));
		strText = szText;
		a_sprintf(szText, GetStringFromTable(playerInfoCache.roleinfo.sub_level>0? 7011:7200),playerInfoCache.roleinfo.level);
		strText = strText + szText;
		pTxtInfo->AppendText(strText);
		a_sprintf(szText, GetStringFromTable(7203),GetStringFromTable(playerInfoCache.roleinfo.spouse>0 ? 7160 : 7161));
		strText = szText;
		a_sprintf(szText, GetStringFromTable(7202),GetStringFromTable(PROFESSION_START + playerInfoCache.roleinfo.occupation));
		strText += szText;
		pTxtInfo->AppendText(strText);
		if (playerInfoCache.roleinfo.sectname.IsEmpty() || playerInfoCache.roleinfo.sectname == playerInfoCache.playerinfo.rolename)
			a_sprintf(szText, GetStringFromTable(7204),GetStringFromTable(7199));
		else
			a_sprintf(szText, GetStringFromTable(7204),playerInfoCache.roleinfo.sectname);
		pTxtInfo->AppendText(szText);
		if (playerInfoCache.roleinfo.familyname.IsEmpty())
			a_sprintf(szText, GetStringFromTable(7205),GetStringFromTable(7199));
		else
			a_sprintf(szText, GetStringFromTable(7205),playerInfoCache.roleinfo.familyname);
		pTxtInfo->AppendText(szText);
		if (playerInfoCache.roleinfo.factionname.IsEmpty())
			a_sprintf(szText, GetStringFromTable(7206),GetStringFromTable(7199));
		else
			a_sprintf(szText, GetStringFromTable(7206),playerInfoCache.roleinfo.factionname);
		pTxtInfo->AppendText(szText);
	}
}

//FriendSeekingMsg.dtf
void CDlgFriendSeek::OnPrtc_SNSAcceptApplyRe(GNET::Protocol* pData)
{
	GNET::SNSAcceptApply_Re* p = static_cast<GNET::SNSAcceptApply_Re*>(pData);
	if (p->retcode != ERROR_SUCCESS)
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(7230+p->retcode), GP_CHAT_MISC);
	}
	int roleId = p->apply_roleid;
	m_curInfo.acceptList.push_back(roleId);
	OnCommand_Refresh("");
}

void CDlgFriendSeek::OnPrtc_SNSDeleteLeaveMsgRe(GNET::Protocol* pData)
{
	GNET::SNSAcceptApply_Re* p = static_cast<GNET::SNSAcceptApply_Re*>(pData);
	if (p->retcode != ERROR_SUCCESS)
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(7230+p->retcode), GP_CHAT_MISC);
	}
	else
		OnCommand_Refresh("");
	return;
}

void CDlgFriendSeek::InsertMessage(int id, int dstId)
{
	P_AUITREEVIEW_ITEM pItem = m_pTvMessage->GetFirstChildItem(m_pTvMessage->GetRootItem());
	P_AUITREEVIEW_ITEM pItemRe = GetSuitItem(pItem, dstId);
	if (!pItemRe)
	{
		pItemRe = m_pTvMessage->GetRootItem();
	}
	ACHAR szText[256];
	struct tm tim = g_pGame->GetServerLocalTime();
	a_sprintf(szText,GetStringFromTable(7220),GetHostPlayer()->GetName(),
				m_strMsg,tim.tm_year+1900,tim.tm_mon+1,tim.tm_mday,tim.tm_hour,tim.tm_min);
	pItem = m_pTvMessage->InsertItem(szText, pItemRe);
	m_pTvMessage->SetItemData(pItem,id);

}

void CDlgFriendSeek::SetPlayerInfo(int is_private, const ACHAR* szNickname, int birthday, int province, int city, const ACHAR* szCareer, const ACHAR* szAddress, const ACHAR* szIntroduce)
{
	PlayerInfoCache &playerinfo = m_id2PlayerInfoCache[GetHostPlayer()->GetCharacterID()];
	playerinfo.playerinfo.is_private = is_private;
	playerinfo.playerinfo.nickname = szNickname;
	playerinfo.playerinfo.birthday = birthday;
	playerinfo.playerinfo.province = province;
	playerinfo.playerinfo.city = city;
	playerinfo.playerinfo.career = szCareer;
	playerinfo.playerinfo.address = szAddress;
	playerinfo.playerinfo.introduce = szIntroduce;
}

void CDlgFriendSeek::FilterReturn(ACString &str)
{
	for( int i = 0; i < str.GetLength()-1; i++ )
	{
		if ( str[i]== '\r' || str[i]== '\n')
		{
			str[i] = ' ';
		}
	}
}

void CDlgFriendSeek::AddCharmRank()
{
	ACString strPage = GetGameUIMan()->m_pDlgRankList->GetCorresText(m_iSeekingType+80, m_iCurCharmPage);
	if (!strPage.IsEmpty())
	{
		m_pLst_Charm->ResetContent();
		while( strPage.GetLength() != 0 )
		{
			int i = strPage.Find(_AL('\r'));
			AddStringToList(strPage.Left(i));
			strPage.CutLeft(i + 1);
		}
	}
}

void CDlgFriendSeek::AddStringToList(const ACHAR* szText)
{
	CSplit s(szText);
	CSplit::VectorAWString vec = s.Split(_AL("\t"));
	ACString strText = _AL("");
	A3DCOLOR color[6];
	DWORD i;
	for(i = 0; i < vec.size(); i++ )
	{
		if( vec[i].GetLength() > 1 && vec[i][0] == _AL('^') )
		{
			if( vec[i][1] ==  _AL('^') )
			{
				vec[i].CutLeft(1);
				color[i] = 0xFFFFFFFF;
			}
			else if( vec[i].GetLength() > 7 )
			{
				color[i] = 0;
				for(int j = 1; j < 7; j++ )
					if( vec[i][j] >= '0' && vec[i][j] <= '9' )
						color[i] = color[i] * 16 + vec[i][j] - '0';
					else if( vec[i][j] >= 'A' && vec[i][j] <= 'F' )
						color[i] = color[i] * 16 + vec[i][j] - 'A' + 10;
					else if( vec[i][j] >= 'a' && vec[i][j] <= 'f' )
						color[i] = color[i] * 16 + vec[i][j] - 'f' + 10;
				color[i] = color[i] | 0xFF000000;
				vec[i].CutLeft(7);
				if( vec[i].GetLength() > 1 && vec[i][0] ==  _AL('^'))
					vec[i].CutLeft(1);
			}
		}
		else
			color[i] = 0xFFFFFFFF;
		strText += vec[i] + _AL('\t');
	}
	m_pLst_Charm->AddString(strText);
//	for(i = 0; i < 6; i++ )
//		m_pLst_All->SetItemTextColor(m_pLst_All->GetCount() - 1, color[i], i);
}

int CDlgFriendSeek::Charm2Star(int charm)
{
	int star;
	for (star=0; star<10; star++)
	{
		if (STAR_TO_CHARM(star+1)>=charm)
		{
			break;
		}
	}
	return star;
}

void CDlgFriendSeek::ChangeTitle()
{
	if ( m_szName == "Win_FriendSeekingPerCharm" )
		m_pLabTitle->SetText(GetStringFromTable(7340+m_iSeekingType));
}
