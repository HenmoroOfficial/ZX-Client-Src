// Filename	: DlgChat.cpp
// Creator	: Tom Zhou
// Date		: October 11, 2005

#include "AFI.h"
#include "A3DGfxKeyPoint.h"
#include "AWScriptFile.h"
#include "A2DSprite.h"
#include "privilege.hxx"
#include "AUI\\CSplit.h"
#include "AUI\\AUIDef.h"
#include "AUI\\A3DFTFont.h"
#include "AUI\\AUICommon.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUICTranslate.h"
#include "DlgChat.h"
#include "DlgSetting.h"
#include "DlgAskHelpToGM.h"
#include "DlgMinimizeBar.h"
#include "DlgMiniMap.h"
#include "DlgFriendChat.h"
#include "DlgPetDetail.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_World.h"
#include "EC_HostPlayer.h"
#include "EC_Configs.h"
#include "EC_Resource.h"
#include "EC_Inventory.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrConsume.h"
#include "EC_Model.h"
#include "EC_Utility.h"
#include "EC_FixedMsg.h"
#include "EC_GameRecord.h"
#include "EC_AutoMove.h"
#include "EC_Algorithm.h"
#include "EC_GameUIMisc.h"
#include "EC_Friend.h"
#include "EC_Achievement.h"
#include "LuaEvent.h"

extern game_record gr;

#define new A_DEBUG_NEW
#define CDLGCHAT_MAX_HISTORY	20
#define CDLGCHAT_TYPE_PER_PAGE 28
#define CDLGCHAT_TYPE_PER_LINE 4
#define CDLGCHAT_TOTAL_TYPES 40


AUI_BEGIN_COMMAND_MAP(CDlgChat, CDlgBase)

AUI_ON_COMMAND("close",			OnCommand_close)
AUI_ON_COMMAND("lock",			OnCommand_lock)
AUI_ON_COMMAND("Btn_Lock",		OnCommand_lock)
AUI_ON_COMMAND("speak",			OnCommand_speak)
AUI_ON_COMMAND("popface",		OnCommand_popface)
AUI_ON_COMMAND("speakmode*",	OnCommand_speakmode)
AUI_ON_COMMAND("channel",		OnCommand_channel)
AUI_ON_COMMAND("chgalpha",		OnCommand_chgalpha)
AUI_ON_COMMAND("clear",			OnCommand_clear)
AUI_ON_COMMAND("Btn_Clear",		OnCommand_clear)
AUI_ON_COMMAND("set*",			OnCommand_set)
AUI_ON_COMMAND("send",			OnCommand_send)
AUI_ON_COMMAND("joinchannel",	OnCommand_joinchannel)
AUI_ON_COMMAND("Btn_Cancel",	OnCommand_CANCEL)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)
AUI_ON_COMMAND("minimize",		OnCommandMinimize)
AUI_ON_COMMAND("action",		OnCommand_Action)
AUI_ON_COMMAND("tkto",			OnCommand_Talk)
AUI_ON_COMMAND("popman",		OnCommand_PopFaceManage)
AUI_ON_COMMAND("talk*",			OnCommand_TalkAction)
AUI_ON_COMMAND("save",			OnCommand_TalkSave)
AUI_ON_COMMAND("new",			OnCommand_TalkNew)
AUI_ON_COMMAND("del",			OnCommand_TalkDelete)
AUI_ON_COMMAND("reset",			OnCommand_TalkReset)
AUI_ON_COMMAND("ascend",		OnCommand_TalkSortAscend)
AUI_ON_COMMAND("descend",		OnCommand_TalkSortDescend)
AUI_ON_COMMAND("Btn_Edit*",		OnCommand_LocalChatInfo)
AUI_ON_COMMAND("Btn_Confirm",	OnCommand_LocalChatInfoSave)
AUI_ON_COMMAND("Ch_Up*",		OnCommand_LocalChatSet)
AUI_ON_COMMAND("Ch_Down*",		OnCommand_LocalChatSet)
AUI_ON_COMMAND("Btn_SettingOther",	OnCommand_SettingGame)
AUI_ON_COMMAND("custommode*",OnCommand_CustomMode)
AUI_ON_COMMAND("Btn_Setting",OnCommand_ChatSetting)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgChat, CDlgBase)

AUI_ON_EVENT("DEFAULT_Txt_Speech",	WM_CHAR,			OnEventChar_Txt_Speech)
AUI_ON_EVENT("DEFAULT_Txt_Speech",	WM_KEYDOWN,			OnEventKeyDown_Txt_Speech)
AUI_ON_EVENT("DEFAULT_Txt_Speech",	WM_SYSCHAR,			OnEventSysChar_Txt_Speech)
AUI_ON_EVENT("Txt_Content",			WM_LBUTTONDOWN,		OnEventLButtonDown_Txt_Content)
AUI_ON_EVENT("Txt_Content",			WM_LBUTTONUP,		OnEventLButtonUp_Txt_Content)
AUI_ON_EVENT("Txt_Content",			WM_RBUTTONUP,		OnEventRButtonUp_Txt_Content)
AUI_ON_EVENT("Txt_Content",			WM_MOUSEMOVE,		OnEventMouseMove_Txt_Content)
AUI_ON_EVENT("Img_ZoomIn",			WM_LBUTTONDOWN,		OnEventLButtonDown_Img_ZoomIn)
AUI_ON_EVENT("Img_ZoomIn",			WM_LBUTTONUP,		OnEventLButtonUp_Img_ZoomIn)
AUI_ON_EVENT("Img_ZoomIn",			WM_MOUSEMOVE,		OnEventMouseMove_Img_ZoomIn)
AUI_ON_EVENT("Face_*",				WM_LBUTTONUP,		OnEventLButtonUp_Face)
AUI_ON_EVENT("Lst_Word",			WM_LBUTTONDOWN,		OnEventLButtonDown_Lst_Word)
AUI_ON_EVENT("*",					WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,					WM_MOUSEWHEEL,		OnEventMouseWheel)

AUI_END_EVENT_MAP()

int CDlgChat::m_nCurChatAlpha = 0;
int CDlgChat::m_nActiveChannelSet = -1;
int CDlgChat::m_nChatMsgCount1 = 0;
int CDlgChat::m_nChatMsgCount2 = 0;
int CDlgChat::m_nCurHistory = -1;
int CDlgChat::m_nMouseLastX;
int CDlgChat::m_nMouseLastY;
SIZE CDlgChat::m_nOldSize;
DWORD CDlgChat::m_dwTickFarCry = 0;
bool CDlgChat::m_bPopType = 0;
ACString CDlgChat::m_strCurChatText;
abase::vector<CDlgChat::CHAT_MSG> CDlgChat::m_vecHistory;
abase::vector<CDlgChat::CHAT_MSG> CDlgChat::m_vecChatMsg;
int CDlgChat::m_nMsgIndex = 1;
abase::vector<CDlgChat::QUICK_TALK>	CDlgChat::m_vecQuickTalk;
bool CDlgChat::m_bLoaded = 0;
int CDlgChat::m_iCurrentPage = 0;
int CDlgChat::m_iCurrentShow = CDlgChat::DLGCHAT_SHOWINFO_BOTH;
int CDlgChat::m_iSpeakWay = 0;
ACString CDlgChat::m_strWhisperName = _AL("");

//							普通		组队	密语		帮派		家族	世界		圈子		同服
static ACHAR *a_szFlag[] = { _AL(""), _AL("!!"), _AL("/"), _AL("!~"), _AL("!@"), _AL("$"), _AL("!#"), _AL("!&") };
static int a_IndexSetCross[] = { 0, 1, 2, 4, 5, 6};

const ACHAR * CDlgChat::m_pszColor[CLIENT_CHAT_MAX] =
{
	_AL("^FFFFFF"),		// GP_CHAT_LOCAL
	_AL("^FFFF00"),		// GP_CHAT_FARCRY
	_AL("^00FF00"),		// GP_CHAT_TEAM
	_AL("^00FFFF"),		// GP_CHAT_FACTION
	_AL("^FF7FFF"),		// GP_CHAT_WHISPER
	_AL("^FF7F00"),		// GP_CHAT_DAMAGE
	_AL("^FF7F00"),		// GP_CHAT_FIGHT
	_AL("^FF7F00"),		// GP_CHAT_TRADE
	_AL("^FF7F00"),		// GP_CHAT_SYSTEM
	_AL("^FF0000"),		// GP_CHAT_BROADCAST
	_AL("^FF7F00"),		// GP_CHAT_MISC
	_AL("^14A9FF"),		// GP_CHAT_FAMILY
	_AL("^F7C8C6"),		// GP_CHAT_CIRCLE
	_AL("^00FFFF"),		// GP_CHAT_ZONE
	_AL("^FFFF00"),		// GP_CHAT_SPEAKER
	_AL("^FFFF00"),		// GP_CHAT_RUMOR
	_AL("^E9C916"),		// CLIENT_CHAT_MONEY
	_AL("^FF80C0"),		// CLIENT_CHAT_EXPERIENCE
	_AL("^FF8000"),		// CLIENT_CHAT_PICKUP
	_AL("^FF0000"),		// CLIENT_CHAT_KILL
	_AL("^FFFFFF"),		// CLIENT_CHAT_RESERVED3
};

CDlgChat::CDlgChat()
{
	m_pObjLock = NULL;
}

CDlgChat::~CDlgChat()
{
	for (size_t i = 0; i < m_vecChatMsg.size(); i++)
	{
		delete m_vecChatMsg[i].pItem;
		delete m_vecChatMsg[i].pAchievement;
	}

	m_vecChatMsg.clear();
	m_vecQuickTalk.clear();
	m_bLoaded = 0;
}

bool CDlgChat::OnInitDialog()
{
	size_t i;
	for (i = 0; i < m_vecChatMsg.size(); i++)
		delete m_vecChatMsg[i].pItem;

	m_vecHistory.clear();
	m_vecChatMsg.clear();

	m_bLocked = false;
	m_nCurChatAlpha = CDLGCHAT_MAX_CHAT_ALPHA - 1;
	m_nActiveChannelSet = -1;
	m_nChatMsgCount1 = 0;
	m_nChatMsgCount2 = 0;
	m_nCurHistory = -1;
	m_strCurChatText = _AL("");

	m_pObjLock = GetDlgItem("Btn_Lock");
	if( GetDlgItem("Img_ZoomIn") )
		GetDlgItem("Img_ZoomIn")->SetAcceptMouseMessage(true);
	if (stricmp(m_szName, "Win_PopTalk") == 0)
	{
		m_pScl_TalkView = (PAUISCROLL)GetDlgItem("Scl_Item");
		SetCanMove(false);
	}
	else if (stricmp(m_szName, "Win_Chat") == 0)
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Channel");
		pCombo->SetAlign(AUIFRAME_ALIGN_CENTER);
		for( i = 475; i <= 480; i++ )
			pCombo->AddString(GetStringFromTable(i));

		pCombo->AddString(GetStringFromTable(482));
		pCombo->AddString(GetStringFromTable(483));
		pCombo->SetCurSel(0);	// Normal channel.

		//GetDlgItem("Txt_Channel")->SetText(GetStringFromTable(475));
		GetGameUIMan()->AttachEmotion(GetDlgItem("DEFAULT_Txt_Speech"));
		CheckRadioButton(1, 1);
		CheckRadioButton(2, 0);
		SetCanMove(false);
	}
	else if (stricmp(m_szName, "Win_KfpkChat") == 0)
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Channel");
		pCombo->SetAlign(AUIFRAME_ALIGN_CENTER);
		for( i = 0; i <= 5; i++ )
			pCombo->AddString(GetStringFromTable(475+a_IndexSetCross[i]));
		pCombo->SetCurSel(0);	// Normal channel.
		
		GetGameUIMan()->AttachEmotion(GetDlgItem("DEFAULT_Txt_Speech"));
		CheckRadioButton(1, 1);
		SetCanMove(false);
	}
	else if (stricmp(m_szName, "Win_ChatInfo") == 0)
	{
		PAUITEXTAREA pText = (PAUITEXTAREA)GetDlgItem("Txt_Content");
		pText->SetMaxLines(CECGAMEUIMAN_MAX_MSGS);
		pText->SetLeftScrollBar(true);
		GetGameUIMan()->AttachEmotion(pText);
		if (GetGameUIMan()->GetControlImgLst()->size()>0)
			pText->SetImageList(GetGameUIMan()->GetControlImgLst());
		SetCanMove(false);
	}
	else if (stricmp(m_szName, "Win_ChatInfo2") == 0)
	{
		PAUITEXTAREA pText = (PAUITEXTAREA)GetDlgItem("Txt_Content");
		pText->SetMaxLines(CECGAMEUIMAN_MAX_MSGS);
		pText->SetLeftScrollBar(true);
		if (GetGameUIMan()->GetControlImgLst()->size()>0)
			pText->SetImageList(GetGameUIMan()->GetControlImgLst());
		GetGameUIMan()->AttachEmotion(pText);		
		SetCanMove(false);
	}
	else if (stricmp(m_szName, "Win_WhisperChat") == 0)
	{
		PAUITEXTAREA pText = (PAUITEXTAREA)GetDlgItem("Txt_Chat");
		GetGameUIMan()->AttachEmotion(pText);
		if (GetGameUIMan()->GetControlImgLst()->size()>0)
			pText->SetImageList(GetGameUIMan()->GetControlImgLst());		
		GetGameUIMan()->AttachEmotion(GetDlgItem("DEFAULT_Txt_Speech"));
	}
	else if (stricmp(m_szName, "Win_ChatSpecInfo") == 0)
	{
		PAUITEXTAREA pText = (PAUITEXTAREA)GetDlgItem("Txt_Content");
		pText->SetForceRenderScroll(false);
		if (GetGameUIMan()->GetControlImgLst()->size()>0)
			pText->SetImageList(GetGameUIMan()->GetControlImgLst());		
		GetGameUIMan()->AttachEmotion(pText);
		GetDlgItem("ShapeGfx")->Show(false);
	}

	return true;
}

void CDlgChat::OnCommand_close(const char * szCommand)
{
	if( m_szName == "Win_Chat" || m_szName == "Win_KfpkChat" )
	{
		switch (m_iCurrentShow)
		{
		case DLGCHAT_SHOWINFO_BOTH:
			GetGameUIMan()->m_pDlgChat2->Show(false);
			break;
		case DLGCHAT_SHOWINFO_11:
			Show(false);
			GetGameUIMan()->m_pDlgChat1->Show(false);
			GetGameUIMan()->m_pDlgChat2->Show(false);
			GetGameUIMan()->GetDialog("Win_ChatClose")->Show(true);
			break;
		case DLGCHAT_SHOWINFO_12:
			GetGameUIMan()->m_pDlgChat2->Show(true);
			break;
		}
	}
	else
	{
		Show(false);
		GetGameUIMan()->m_pDlgChat1->Show(true);
		GetGameUIMan()->m_pDlgChat->Show(true);
	}
	m_iCurrentShow = (m_iCurrentShow+1)%DLGCHAT_SHOWINFO_MAX;
	RearrangeWindow();
}

void CDlgChat::OnCommand_lock(const char * szCommand)
{
	if( stricmp(m_szName, "Win_ChatInfo2") == 0 )
	{
		m_bLocked = !m_bLocked;
		PAUICHECKBOX pCheck = (PAUICHECKBOX)GetDlgItem("Btn_Lock");
		pCheck->Check(m_bLocked);
		if( !m_bLocked )
		{
			PAUITEXTAREA pText = (PAUITEXTAREA)GetDlgItem("Txt_Content");
			pText->SetData(pText->GetFirstLine());
			pText->ScrollToBottom();
			pCheck->SetFlash(false);
		}
	}
	else
	{
		m_bLocked = !m_bLocked;
		PAUICHECKBOX pCheck = (PAUICHECKBOX)GetGameUIMan()->m_pDlgChat->GetDlgItem("Btn_Lock");
		pCheck->Check(m_bLocked);
		if( !m_bLocked )
		{
			PAUITEXTAREA pText = (PAUITEXTAREA)GetGameUIMan()->m_pDlgChat1->GetDlgItem("Txt_Content");
			pText->SetData(pText->GetFirstLine());
			pText->ScrollToBottom();
			pCheck->SetFlash(false);
		}
	}
}
// 去掉起始标识符的纯聊天内容长度
int CDlgChat::GetTextLength(const ACHAR* szText)
{
	if (a_strlen(szText)==0)
		return 0;

	int preLen = 0;
	if( szText[0] == '/' )
	{
		const ACHAR* pSpace = a_strstr(szText, _AL(" "));
		if (pSpace)
		{
			preLen = pSpace - szText + 1;
		}

	}
	else
	{
		for( int i = 0; i < sizeof(a_szFlag) / sizeof(ACHAR *); i++ )
		{
			if( a_strstr(szText, a_szFlag[i]) == szText )
			{
				preLen = a_strlen(a_szFlag[i]);
				break;
			}
		}
	}

	return GetLengthExpItem(szText)-preLen;
}

void CDlgChat::OnCommand_speak(const char * szCommand)
{
	PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Speech");
	int nPack, nSlot;
	if( pEdit->HasIvtrItem() )
		nPack = pEdit->GetIvtrItemPack();
	else
		nPack = -1;
	nSlot = pEdit->GetIvtrItemIndex();

	int nAchievement = -1;
	if (pEdit->GetItemCount(enumEITask) > 0)
	{
		EditBoxItemMap::iterator& it = ((EditBoxItemsSet*) (&pEdit->GetItemSet()))->GetItemIterator();
		while(!it.is_eof())
		{
			EditBoxItemBase* pItem = it->second;
			if (pItem->GetType() == enumEITask)
			{
				nAchievement = *(unsigned int*)pItem->GetExtraData();
				break;
			}

			++it;
		}
	}
	
	if (nPack>=0 && nSlot>=0 && GetHostPlayer()->GetPack(nPack)->GetItem(nSlot)==NULL)
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(9120), GP_CHAT_MISC);
		return;
	}
	else if(nAchievement >= 0 && GetHostPlayer()->GetAchievementMan()->GetAchievement(nAchievement) == NULL)
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(9120), GP_CHAT_MISC);
		return;
	}

	ACString strText = pEdit->GetText();
    
	//添加频道前缀
	{
	   strText = a_szFlag[m_iSpeakWay] + strText;
	}

	if( strText.GetLength() <= 0 )
	{
		ChangeFocus(NULL);
		return;
	}

	//pEdit->SetText(a_szFlag[m_iSpeakWay]);
	pEdit->SetText(_AL(""));
//	DetermineChannelByText(a_szFlag[m_iSpeakWay]);
	GetGameUIMan()->BringWindowToTop(this);
	ChangeFocus(pEdit);

	GetGameUIMan()->FilterBadWords(strText);

	// force message that start with "/gm"
	// go to dialog CDlgAskHelpToGM
	strText.TrimLeft();
	if (strText.GetLength() >= 3 && strText.Left(3).CompareNoCase(_AL("/gm")) == 0)
	{
		DWORD dwLastReportTime = GetGameUIMan()->m_pDlgAskHelpToGM->GetData();
		
#ifndef _DEBUG
		// GM can send msg without delta-time restriction
		if (GetTickCount() - dwLastReportTime < 300 * 1000 &&
			!g_pGame->GetGameRun()->GetHostPlayer()->IsGM())
		{
			GetGameUIMan()->AddChatMessage(GetStringFromTable(612), GP_CHAT_MISC);
			return;
		}
#endif

		m_strCurChatText = _AL("");
		ChangeFocus(NULL);
		strText.CutLeft(3);
		GetGameUIMan()->m_pDlgAskHelpToGM->ShowWithHelpMessage(strText);
		return;
	}


	int i;
	CHAT_MSG cm;
	DWORD dwTime = GetTickCount();

	if( strText.GetLength() > 1 && strText[0] == '$' && m_iSpeakWay == CHATCHANNEL_WORLD)
	{
		CDlgChat::CHAT_MSG cm;
		cm.cChannel = GP_CHAT_FARCRY;
		cm.dwTime = m_dwTickFarCry;
		cm.strMsg = strText.Right(strText.GetLength()-a_strlen(a_szFlag[m_iSpeakWay]));;
		cm.nPack = nPack;
		cm.nSlot = nSlot;
		cm.nAchievement = nAchievement;
		m_vecHistory.push_back(cm);
		m_nCurHistory = m_vecHistory.size();
		if( GetHostPlayer()->GetPack()->GetItemTotalNum(GetGameRun()->GetSpecialIDConfig().id_speaker) < 1 &&
			GetHostPlayer()->GetPack()->GetItemTotalNum(GetGameRun()->GetSpecialIDConfig().id_speaker2) < 1)
		{
			GetGameUIMan()->AddChatMessage(GetStringFromTable(731), GP_CHAT_MISC);
			return;
		}
	}
	else
	{
		bool bUnder30 = !GetHostPlayer()->IsGM() &&
			GetGame()->GetConfigs()->Limit30Words() &&
			GetHostPlayer()->GetBasicProps().iLevel<30 && 
			GetHostPlayer()->GetRebornInfo().size()==0;
		if ( bUnder30 && GetTextLength(strText)>12 )
		{
			GetGameUIMan()->AddChatMessage(GetStringFromTable(361), GP_CHAT_MISC);
			m_nCurHistory = m_vecHistory.size();
			return;
		}
		if( m_vecHistory.size() > 0 )
		{
			cm = m_vecHistory[m_vecHistory.size() - 1];
			if (bUnder30 && GetHostPlayer()->GetCoolTime(GP_CT_NEWBIE_CHAT))
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(362), GP_CHAT_MISC);
				m_nCurHistory = m_vecHistory.size();
				return;
			}
			if( dwTime - cm.dwTime <= 1000 )
			{
				GetGameUIMan()->AddChatMessage(GetStringFromTable(272), GP_CHAT_MISC);
				m_nCurHistory = m_vecHistory.size();
				return;
			}

			for( i = 0; i < (int)m_vecHistory.size(); i++ )
			{
				cm = m_vecHistory[i];
				if( cm.cChannel != GP_CHAT_WHISPER &&
					0 == strText.Compare(cm.strMsg) && cm.nPack == nPack && cm.nSlot == nSlot &&
					cm.nAchievement == nAchievement && dwTime - cm.dwTime <= 6000 )
				{
					GetGameUIMan()->AddChatMessage(GetStringFromTable(273), GP_CHAT_MISC);
					m_nCurHistory = m_vecHistory.size();
					return;
				}
			}
		}
	}
	//表情动作判断
	int iQk1 = strText.Find(_AL(":"));
	int iQk2 = strText.Find(GetGameUIMan()->GetStringFromTable(8493));
	int iQkSpace = strText.Find(_AL(" "));
	ACString strQkText;
	if (iQkSpace<0 || (iQk1>=0&&iQk1==(iQkSpace+1)) || (iQk2>=0&&(iQk2=iQkSpace+1)))
	{
		int iQk = 0;
		if (iQk1<0)
			if (iQk2<0)
				iQk = -1;
		else
			iQk = iQk2;
		else
			if (iQk2<0)
				iQk = iQk1;
			else
				iQk = iQk1>iQk2 ? iQk2 : iQk1;
		if (iQk>=0)
		{
			ACString strQuick = strText.Mid(iQk+1);
			for (UINT i=0; i<m_vecQuickTalk.size(); i++)
			{
				QUICK_TALK qkItem = m_vecQuickTalk[i];
				if (strQuick == qkItem.strName)
				{
					ACHAR ch = 0xdbff;
					int idSelObj = GetHostPlayer()->GetSelectedTarget();
					if (GetCheckedRadioButton(1)==3)		// 密语频道
					{
						strText = strText.Left(iQk+1) + ACString(ch,2) + qkItem.strTarget;
					}
					else if( idSelObj != 0 && ISPLAYERID(idSelObj))
					{
						int iLeft = qkItem.strTarget.Find(_AL("#n"));
						ACHAR szText[80];
						AUI_ConvertChatString(GetGameRun()->GetPlayerName(idSelObj, true), szText);
						ACString strTemp = qkItem.strTarget;
						while (iLeft>=0)
						{
							int iRight = strTemp.GetLength() - iLeft - 2;
							strTemp.Format(_AL("%s%s%s"),strTemp.Left(iLeft),szText,strTemp.Right(iRight));
							iLeft = strTemp.Find(_AL("#n"));
						}
						strText = strText.Left(iQk+1) + ACString(ch,2) + strTemp;
					}
					else
					{
						strText = strText.Left(iQk+1) + ACString(ch,2) + qkItem.strNoTarget;
					}
					strQkText = _AL(":")+qkItem.strName;
				}
			}
		}
	}

	char cChannel;
	const ACHAR *pszMsg;
	ACHAR szName[80], szText[512], szMsg[1024] = _AL("");
	CECGameSession *pSession = GetGameSession();
	CECHostPlayer *pHost = GetGameRun()->GetHostPlayer();

	if( strText.GetLength() >= 2 && strText[0] == '!' && strText[1] == '!' && m_iSpeakWay == CHATCHANNEl_TEAM)
	{
		cChannel = GP_CHAT_TEAM;
		pszMsg = (const ACHAR *)strText + 2;
		if( *pszMsg == _AL('\0') )
		{
			ChangeFocus(NULL);
			return;
		}
	}
	else if( strText.GetLength() >= 2 && strText[0] == '!' && strText[1] == '&' && m_iSpeakWay == CHATCHANNEL_SAMESERVER)
	{
		cChannel = GP_CHAT_ZONE;
		pszMsg = (const ACHAR *)strText + 2;
		if( *pszMsg == _AL('\0') )
		{
			ChangeFocus(NULL);
			return;
		}
	}
	else if( strText.GetLength() >= 2 && strText[0] == '!' && strText[1] == '~' && m_iSpeakWay == CHATCHANNEL_FACTION)
	{
		cChannel = GP_CHAT_FACTION;
		pszMsg = (const ACHAR *)strText + 2;
		if( *pszMsg == _AL('\0') )
		{
			ChangeFocus(NULL);
			return;
		}

		pSession->faction_chat(pszMsg, cChannel, nPack, nSlot, nAchievement);
	}
	else if( strText.GetLength() >= 2 && strText[0] == '!' && strText[1] == '@' && m_iSpeakWay == CHATCHANNEL_FAMILY)
	{
		cChannel = GP_CHAT_FAMILY;
		pszMsg = (const ACHAR *)strText + 2;
		if( *pszMsg == _AL('\0') )
		{
			ChangeFocus(NULL);
			return;
		}

		pSession->faction_chat(pszMsg, cChannel, nPack, nSlot, nAchievement);
	}
	else if( strText.GetLength() >= 2 && strText[0] == '!' && strText[1] == '#' && m_iSpeakWay == CHATCHANNEL_CIRCLE)
	{
		cChannel = GP_CHAT_CIRCLE;
		pszMsg = (const ACHAR *)strText + 2;
		if( *pszMsg == _AL('\0') )
		{
			ChangeFocus(NULL);
			return;
		}

		pSession->circle_chat(pszMsg, cChannel, nPack, nSlot, nAchievement);
	}
	else if( strText.GetLength() >= 1 && strText[0] == '$' && m_iSpeakWay == CHATCHANNEL_WORLD)
	{
		cChannel = GP_CHAT_FARCRY;
		pszMsg = (const ACHAR *)strText + 1;
		if( *pszMsg == _AL('\0') )
		{
			ChangeFocus(NULL);
			return;
		}
		ChangeFocus(NULL);
		PAUIDIALOG pMsgBox;
		GetGameUIMan()->MessageBox("Game_ChatWorld", GetStringFromTable(732), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetText(pszMsg);
		pMsgBox->SetData(nPack);
		pMsgBox->SetDataPtr((void*)nSlot);
		pMsgBox->SetData64(nAchievement);

		return;
	}
	else if( strText.GetLength() >= 1 && strText[0] == '/' && m_iSpeakWay == CHATCHANNEL_PRIVATE)
	{
		if( strText[1] == _AL('\0') )
		{
			ChangeFocus(NULL);
			return;
		}
		CSplit s((const ACHAR *)strText + 1);
		CSPLIT_STRING_VECTOR vec = s.Split(_AL(" "));

		if( vec.size() > 1 )
		{
			if( a_stricmp(_AL("GM"), vec[0]) == 0 )
			{
				PAUIDIALOG pDlg = GetGameUIMan()->m_pDlgAskHelpToGM;
				DWORD dwLastReportTime = pDlg->GetData();

				if( (GetTickCount() - dwLastReportTime) / 1000 > 300 )
					pDlg->SetData(GetTickCount());
				else
				{
					GetGameUIMan()->AddChatMessage(GetStringFromTable(612), GP_CHAT_MISC);
					ChangeFocus(NULL);
					return;
				}
			}
			
			pszMsg = (const ACHAR *)strText + vec[0].GetLength() + 2;

			cChannel = GP_CHAT_WHISPER;
			pSession->SendPrivateChatData(vec[0], pszMsg, 0, 0, nPack, nSlot, true, nAchievement);
			
			AUI_ConvertChatString(vec[0], szName);
			AUI_ConvertChatString(pszMsg, szText, false);
			if( a_stricmp(szName, _AL("GM")) == 0 )
				GetGameUIMan()->AddChatMessage(
					GetGameUIMan()->GetStringFromTable(569), GP_CHAT_MISC);

			ACString strMsg = szText;
			ACString strTemp;
			ACHAR ch = 0xdbff;
			ACString strMark = ACString(ch,2);
			if((0 == strMsg.Find(_AL(":")+strMark)||0 == strMsg.Find(GetGameUIMan()->GetStringFromTable(8493))) && 0 <= strMsg.Find(_AL("#N")))
			{
				strMsg = strMsg.Mid(3);
				int iLeft = strMsg.Find(_AL("#N"));
				while(iLeft>=0)
				{
					strMsg[iLeft] = _AL('%');
					strMsg[iLeft+1] = _AL('s');
					strTemp = strMsg;
					strMsg.Format(strTemp, GetHostPlayer()->GetName());
					iLeft = strMsg.Find(_AL("#N"));
				}
				iLeft = strMsg.Find(_AL("#n"));
				while(iLeft>=0)
				{
					strMsg[iLeft] = _AL('%');
					strMsg[iLeft+1] = _AL('s');
					strTemp = strMsg;
					strMsg.Format(strTemp,szName);
					iLeft = strMsg.Find(_AL("#n"));
				}
				a_sprintf(szMsg, GetGameUIMan()->GetStringFromTable(8494)+strMsg);
			}
			else
			{
				a_sprintf(szMsg, GetStringFromTable(233), szName, szText);
			}
			pEdit->SetText(pEdit->GetText() + vec[0] + _AL(" "));
		}
		else
		{
			a_strcpy(szMsg, GetStringFromTable(234));
			cChannel = GP_CHAT_MISC;
		}
	}
	else
	{
		cChannel = GP_CHAT_LOCAL;
		pszMsg = (const ACHAR *)strText;
	}
	
	if( cChannel != GP_CHAT_WHISPER &&
		cChannel != GP_CHAT_FACTION &&
		cChannel != GP_CHAT_FAMILY &&
		cChannel != GP_CHAT_CIRCLE &&
		cChannel != GP_CHAT_MISC )
	{
		pSession->SendChatData(cChannel, pszMsg, nPack, nSlot,-1, nAchievement);
		AUI_ConvertChatString(pszMsg, szText, false);
		AUI_ConvertChatString(pHost->GetName(), szName);
		a_sprintf(szMsg, GetStringFromTable(210), szName, szText);
	}

	if( a_strlen(szMsg) > 0 )
	{
		if( cChannel != GP_CHAT_LOCAL && cChannel != GP_CHAT_TEAM && cChannel != GP_CHAT_ZONE)
		{
			if( nPack >= 0 )
			{
				CECInventory *pPack = GetHostPlayer()->GetPack(nPack);
				if( pPack )
				{
					CECIvtrItem *pItem = pPack->GetItem(nSlot);
					GetGameUIMan()->AddChatMessage(szMsg, cChannel, -1, NULL, 0, pHost->GetCurEmotionSet(), pItem->Clone());
				}
			}
			else if( nAchievement >= 0 )
			{
				sAchievement *achiv = GetHostPlayer()->GetAchievementMan()->GetAchievement(nAchievement);
				if (achiv)
					GetGameUIMan()->AddChatMessage(szMsg, cChannel, -1, NULL, 0, pHost->GetCurEmotionSet(), NULL, new sAchievement(*achiv));
			}
			else
				GetGameUIMan()->AddChatMessage(szMsg, cChannel, -1, NULL, 0, pHost->GetCurEmotionSet());
		
			if (gr.get_state() == game_record::enum_gr_record)
			{
				frame_record* fr = gr.inc_frame();

				if (gr.get_replay_setting(REPLAY_PRIVATE_CHAT) && cChannel == GP_CHAT_WHISPER
				 || gr.get_replay_setting(REPLAY_COMMON_CHAT) && cChannel != GP_CHAT_WHISPER)
				{
					GNET::Marshal::OctetsStream os;
					os.reserve(512);
					os << cChannel;
					os << GNET::Octets(szMsg, a_strlen(szMsg) * sizeof(ACHAR));
					fr->push_event(new event_record_data(RECORD_DATA_CHAT_MSG, os));
				}
			}
		}
	}

	if( m_vecHistory.size() >= CDLGCHAT_MAX_HISTORY )
		m_vecHistory.erase(m_vecHistory.begin());
	m_strCurChatText = _AL("");

	cm.cChannel = cChannel;
	cm.dwTime = dwTime;
	ACHAR ch = 0xdbff;
	ACString strMark(ch,2);
	if (a_strstr(strText,strMark))
	{
		strText = strQkText;
	}
	cm.strMsg = strText.Right(strText.GetLength()-a_strlen(a_szFlag[m_iSpeakWay]));
	cm.nPack = nPack;
	cm.nSlot = nSlot;
	cm.nAchievement = nAchievement;
	m_vecHistory.push_back(cm);
	m_nCurHistory = m_vecHistory.size();

	ChangeFocus(NULL);
}

void CDlgChat::OnCommand_popface(const char * szCommand)
{
	PAUIOBJECT pEdit = GetDlgItem("DEFAULT_Txt_Speech");
	PAUIDIALOG pShow;
	LuaEvent::FirstEmotionAction();
	if (m_bPopType)
	{
		pShow = GetGameUIMan()->GetDialog("Win_PopTalk");
	} 
	else
	{
		pShow = GetGameUIMan()->GetDialog("Win_Popface");
	}
	
	pShow->Show(!pShow->IsShow());
	pShow->AlignTo(this,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
	pShow->SetDataPtr(this,"ptr_AUIDialog");

	GetGameUIMan()->BringWindowToTop(GetGameUIMan()->m_pDlgChat);
	GetGameUIMan()->m_pDlgChat->ChangeFocus(pEdit);
}

void CDlgChat::OnCommand_speakmode(const char * szCommand)
{
	PAUIEDITBOX pEdit = dynamic_cast<PAUIEDITBOX>(GetDlgItem("DEFAULT_Txt_Speech"));

	int nMode = atoi(szCommand + strlen("speakmode"));
	ACString strText = pEdit->GetText();
	ACString strName;
	A3DCOLOR color = 0xffffffff;
	if( nMode != 0 )
	{
		if (pEdit->HasIvtrItem())
		{
			CECIvtrItem *pItem = GetHostPlayer()->GetPack(pEdit->GetIvtrItemPack())->GetItem(pEdit->GetIvtrItemIndex());
			TransformItemColorName(pItem, strName, color);
		}
		//strText.CutLeft(a_strlen(a_szFlag[m_iSpeakWay]));
		if (m_iSpeakWay==2)	// 密语频道，保存名字
		{
			if(strText.GetLength()==0 || strText[0]==_AL(' '))
			{
				strText.CutLeft(1);
				m_strWhisperName = _AL("");
			}
			else
			{
				CSplit s(strText);
				CSPLIT_STRING_VECTOR vec = s.Split(_AL(" "));
				
				if( vec.size() > 1 )
				{
					m_strWhisperName = vec[0];
					strText.CutLeft(vec[0].GetLength()+1);
				}
				else if( vec.size() == 1 )
				{
					if (pEdit->HasIvtrItem())	// 名字不能是物品
					{
						strText = _AL("");
						m_strWhisperName = _AL("");
					}
					else
					{
						strText = _AL("");
						m_strWhisperName = vec[0];
					}
				}
				else
					m_strWhisperName = _AL("");
			}
		}
		m_iSpeakWay = nMode - 1;
		if (m_iSpeakWay==2)	// 密语频道，添加名字
		{
			strText = m_strWhisperName+_AL(" ")+strText;
		}
	}

	// Get achieve id from editor.
	unsigned int achivID = -1;
	if (pEdit->GetItemCount(enumEITask) > 0)
	{
		EditBoxItemMap::iterator& it = ((EditBoxItemsSet*) (&pEdit->GetItemSet()))->GetItemIterator();
		while(!it.is_eof())
		{
			EditBoxItemBase* pItem = it->second;
			if (pItem->GetType() == enumEITask && pItem->GetExtraData() != NULL)
			{
				achivID = *(unsigned int*) pItem->GetExtraData();
				break;
			}

			++it;
		}
	}

	pEdit->SetTextAndItem(/*a_szFlag[m_iSpeakWay]+*/strText, strName, color);
	if (pEdit->GetItemCount(enumEIIvtrlItem)>0)
	{
		pEdit->SetIvtrItemLocInfo(pEdit->GetIvtrItemPack(), pEdit->GetIvtrItemIndex());
	}
	else if (pEdit->GetItemCount(enumEITask) > 0 && achivID != -1)
	{
		// Set achieve id to editor.
		EditBoxItemMap::iterator& it = ((EditBoxItemsSet*) (&pEdit->GetItemSet()))->GetItemIterator();
		while(!it.is_eof())
		{
			EditBoxItemBase* pItem = it->second;
			if (pItem->GetType() == enumEITask)
			{
				CECAchievementMan* pMan = GetHostPlayer()->GetAchievementMan();
				sAchievement* achiv = pMan->GetAchievement(achivID);
				if (achiv)
				{
					ACString strName;
					A3DCOLOR color;
					CDlgChat::TransformAchievementColorName(achiv, strName, color);

					pItem->SetColor(color);
					pItem->SetName(strName);

					pItem->SetExtraData(&achiv->id, sizeof(unsigned int));
					break;
				}
			}

			++it;
		}
	}

	//DetermineChannelByText(a_szFlag[m_iSpeakWay]);
	GetGameUIMan()->BringWindowToTop(this);
	ChangeFocus(pEdit);
}

void CDlgChat::OnCommand_channel(const char * szCommand)
{
	PAUIOBJECT pEdit = GetDlgItem("DEFAULT_Txt_Speech");
	GetGameUIMan()->BringWindowToTop(this);
	ChangeFocus(pEdit);
}

void CDlgChat::OnCommand_chgalpha(const char * szCommand)
{
	m_nCurChatAlpha = (m_nCurChatAlpha + 1) % (CDLGCHAT_MAX_CHAT_ALPHA - 1);
}

void CDlgChat::OnCommand_clear(const char * szCommand)
{
	EC_LOCAL_SETTING ls = g_pGame->GetConfigs()->GetLocalSettings();
	PAUITEXTAREA pText = (PAUITEXTAREA)GetDlgItem("Txt_Content");
	abase::vector<CDlgChat::CHAT_MSG>::iterator it = m_vecChatMsg.begin();
	if (stricmp(m_szName, "Win_ChatInfo2") == 0)
	{
		while (it!=m_vecChatMsg.end())
		{
			int cChannel = it->cChannel;
			if ((cChannel == GP_CHAT_DAMAGE	||
				cChannel == GP_CHAT_SYSTEM	||
				cChannel == GP_CHAT_BROADCAST	||
				cChannel == GP_CHAT_MISC		||
				(cChannel>=CLIENT_CHAT_MIN) && (cChannel<CLIENT_CHAT_MAX)) && ls.local_chat.bChannel[cChannel])
			{
				delete it->pItem;
				it = m_vecChatMsg.erase(it);
			}
			else
				it++;
		}
	}
	else
	{
		while (it!=m_vecChatMsg.end())
		{
			int cChannel = it->cChannel;
			if (!(cChannel == GP_CHAT_DAMAGE	||
					cChannel == GP_CHAT_SYSTEM	||
				cChannel == GP_CHAT_BROADCAST	||
				cChannel == GP_CHAT_MISC		||
				(cChannel>=CLIENT_CHAT_MIN) && (cChannel<CLIENT_CHAT_MAX)) || !ls.local_chat.bChannel[cChannel])
			{
				delete it->pItem;
				it = m_vecChatMsg.erase(it);
			}
			else
				it++;
		}
		pText = (PAUITEXTAREA)GetGameUIMan()->m_pDlgChat1->GetDlgItem("Txt_Content");
	}
	pText->SetText(_AL(""));

	PAUIOBJECT pEdit = GetGameUIMan()->m_pDlgChat->GetDlgItem("DEFAULT_Txt_Speech");
	GetGameUIMan()->BringWindowToTop(GetGameUIMan()->m_pDlgChat);
	ChangeFocus(pEdit);
}

void CDlgChat::OnCommand_set(const char * szCommand)
{
	int nNewSet;
	if( strcmp(szCommand, "setchannel") == 0 )
	{
		int nCurSel = ((PAUICOMBOBOX)GetDlgItem("Combo_Channel"))->GetCurSel();
		int max = 5;
		if(m_szName == "Win_Chat")
			max = 7;

		if (nCurSel<0||nCurSel>max)
			return;
		
		if (m_szName=="Win_Chat")
		{
			nNewSet = nCurSel;
			//目前同服频道没有添加，将其跳过
			if(nNewSet >= 6)
				++nNewSet;
		}
		else
			nNewSet = a_IndexSetCross[nCurSel];
	}
	else
		nNewSet = atoi(szCommand + strlen("set")) - 1;
	if( m_iSpeakWay != 0 && nNewSet == m_nActiveChannelSet ) return;

	PAUIOBJECT pEdit = GetDlgItem("DEFAULT_Txt_Speech");
	m_nActiveChannelSet = nNewSet;
	pEdit->SetText(_AL(""));
	if(m_szName == "Win_Chat")
	{
		ACHAR *a_pszPrefix[] = { _AL(""), _AL("!!"), _AL("/"), _AL("!~"), _AL(""), _AL("$"),_AL("!&"), _AL(""),_AL("") };
		DetermineChannelByText(a_pszPrefix[nNewSet]);

	}
	else
	{
		ACHAR *a_pszPrefix[] = { _AL(""), _AL("!!"), _AL("/"), _AL("!~"), _AL(""), _AL("$"), _AL("!&") };
		DetermineChannelByText(a_pszPrefix[nNewSet]);
	}

	RebuildChatContents(m_nActiveChannelSet);
	GetGameUIMan()->BringWindowToTop(this);
	ChangeFocus(pEdit);
	//UpdateChatSetInfo();

	if(m_szName == "Win_Chat")
	{
	  // GetDlgItem("Txt_Channel")->SetText(GetGameUIMan()->GetStringFromTable(475+nNewSet));
	   CheckRadioButton(2,nNewSet-6);
	  
	}
}

void CDlgChat::OnCommand_CANCEL(const char * szCommand)
{
	if( m_szName == "Win_Popface" ||
		m_szName == "Win_PopTalk" ||
		m_szName == "Win_WhisperChat" ||
//		m_szName == "Win_WhisperChoose"||
		m_szName == "Win_PopfaceManage"||
		m_szName == "Win_Chat_Edit" )
	{
		SetData(0);
		Show(false);
	}
	else
	{
		PAUIOBJECT pEdit = GetGameUIMan()->m_pDlgChat->GetDlgItem("DEFAULT_Txt_Speech");
		if( pEdit->IsFocus() )
		{
			pEdit->SetText(_AL(""));
			ChangeFocus(NULL);
		}
		else
			GetGameUIMan()->RespawnMessage();	
	}
}

void CDlgChat::OnCommandMinimize(const char *szCommand)
{
	GetGameUIMan()->m_pDlgMinimizeBar[0]->MinimizeDialog(this, 
		GetGameRun()->GetPlayerName(GetData(), true), CDlgMinimizeBar::MINIMIZEBAR_TYPE_WHISPER);
}

void CDlgChat::OnCommand_Action(const char *szCommand)
{
	PAUIDIALOG pShow = GetGameUIMan()->GetDialog("Win_Popface");
	PAUIDIALOG pHide = GetGameUIMan()->GetDialog("Win_PopTalk");
	if (pHide->IsShow())
	{
		pShow->SetDataPtr(pHide->GetDataPtr("ptr_AUIDialog"),"ptr_AUIDialog");
	}
	pShow->Show(true);
	pHide->Show(false);
	pShow->AlignTo(GetGameUIMan()->GetDialog("Win_Chat"),
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
	m_bPopType = 0;
}

void CDlgChat::OnCommand_Talk(const char *szCommand)
{
	PAUIDIALOG pShow = GetGameUIMan()->GetDialog("Win_PopTalk");
	PAUIDIALOG pHide = GetGameUIMan()->GetDialog("Win_Popface");
	if (pHide->IsShow())
	{
		pShow->SetDataPtr(pHide->GetDataPtr("ptr_AUIDialog"),"ptr_AUIDialog");
	}
	pShow->Show(true);
	pHide->Show(false);
	pShow->AlignTo(GetGameUIMan()->GetDialog("Win_Chat"),
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
	m_bPopType = 1;
}

void CDlgChat::OnCommand_PopFaceManage(const char *szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_PopfaceManage");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgChat::OnCommand_TalkAction(const char *szCommand)
{
	int iChannel=0;
	int index = atoi(szCommand+strlen("Btn_"))-1;
	QUICK_TALK qkItem = m_vecQuickTalk[index+m_iCurrentPage*CDLGCHAT_TYPE_PER_LINE];
	PAUIDIALOG pChat;
	PAUIEDITBOX pEdit;
	if( GetDataPtr("ptr_AUIDialog") )
		pChat = (PAUIDIALOG)GetDataPtr("ptr_AUIDialog");
	else
		pChat = GetGameUIMan()->m_pDlgChat;
	
	if( strstr(pChat->GetName(), "Win_FriendChat") )
		pEdit = (PAUIEDITBOX)pChat->GetDlgItem("Txt_Content");
	else
	{
		pEdit = (PAUIEDITBOX)pChat->GetDlgItem("DEFAULT_Txt_Speech");
		iChannel = GetGameUIMan()->m_pDlgChat->GetCheckedRadioButton(1);
	}
	ACString strText(_AL(":"));
	strText += qkItem.strName;
	if (strstr(pChat->GetName(), "Win_FriendChat"))
	{
		CDlgFriendChat *pDlgFriendChat = (CDlgFriendChat*)GetDataPtr("ptr_AUIDialog");
		pEdit->SetText(strText);
		pDlgFriendChat->OnCommandSend("");
	}
	else
	{
			const ACHAR *pszText = pEdit->GetText();
			int iSpeekWay = CDlgChat::GetSpeekWay();
			//密语频道
			if(iSpeekWay == 2)
			{
			  ACString strTextWisper = pszText;
			  int iFirstSpace = strTextWisper.Find(_AL(" "));
			  if (iFirstSpace<0)
			  {
				  pEdit->SetText(strTextWisper+strText);
			  }
			  else
				  pEdit->SetText(strTextWisper.Left(iFirstSpace+1)+strText);
			}
            else
              pEdit->SetText(strText);

			GetGameUIMan()->m_pDlgChat->OnCommand_speak("");
		
#if 0
			const ACHAR *pszText = pEdit->GetText();
			int i;
			for(i = 1; i < sizeof(a_szFlag) / sizeof(ACHAR *); i++ )
			{
				if( a_strstr(pszText, a_szFlag[i]) == pszText )
				{
					if (a_szFlag[i][0]==_AL('/')) // 密语
					{
						ACString strTextWisper = pszText;
						int iFirstSpace = strTextWisper.Find(_AL(" "));
						if (iFirstSpace<0)
						{
							pEdit->SetText(strTextWisper+strText);
						}
						else
							pEdit->SetText(strTextWisper.Left(iFirstSpace+1)+strText);
					}
					else
						pEdit->SetText(a_szFlag[i]+strText);
					break;
				}
			}

			if (i==sizeof(a_szFlag) / sizeof(ACHAR *))
				pEdit->SetText(strText);

		   GetGameUIMan()->m_pDlgChat->OnCommand_speak("");
#endif
	}
}

void CDlgChat::OnCommand_TalkSave(const char *szCommand)
{
	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_Word");
	int index = pLst->GetCurSel();
	PAUIEDITBOX pEdtName = (PAUIEDITBOX)GetDlgItem("Edt_Command");
	PAUIEDITBOX pEdtNoTarget = (PAUIEDITBOX)GetDlgItem("Edt_NoAim");
	PAUIEDITBOX pEdtTarget = (PAUIEDITBOX)GetDlgItem("Edt_Aim");
	ACString strText = pEdtNoTarget->GetText();
	if (strText.Find(_AL("#N"))<0)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(8492), MB_OK,
						A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	strText = pEdtTarget->GetText();
	if (strText.Find(_AL("#N"))<0 || strText.Find(_AL("#n"))<0)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(8492), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	QUICK_TALK &talkItem = m_vecQuickTalk[index];
	talkItem.strName = pEdtName->GetText();
	talkItem.strNoTarget = pEdtNoTarget->GetText();
	FilterReturn(talkItem.strNoTarget);
	talkItem.strTarget = pEdtTarget->GetText();
	FilterReturn(talkItem.strTarget);
	talkItem.bOk = true;
	pLst->DeleteString(index);
	pLst->InsertString(index,talkItem.strName);
	ReBuildQuickTalkButton();
	pLst->SetCurSel(index);
	OnEventLButtonDown_Lst_Word(0,0,NULL);
	SaveQuickTalk();
}

void CDlgChat::OnCommand_TalkNew(const char *szCommand)
{
	QUICK_TALK talkItem;
	talkItem.strName = GetGameUIMan()->GetStringFromTable(8491);
	talkItem.bOk = false;
	m_vecQuickTalk.push_back(talkItem);
	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_Word");
	pLst->AddString(_AL(":")+talkItem.strName);
	int iCount = pLst->GetCount();
	pLst->SetCurSel(iCount-1);
	OnEventLButtonDown_Lst_Word(0,0,NULL);
}

void CDlgChat::OnCommand_TalkDelete(const char *szCommand)
{
	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_Word");
	int index = pLst->GetCurSel();
	m_vecQuickTalk.erase(&m_vecQuickTalk[index]);
	SaveQuickTalk();
	ReBuildQuickTalkButton();
	OnEventLButtonDown_Lst_Word(0,0,NULL);
}

void CDlgChat::OnCommand_TalkReset(const char *szCommand)
{
	GetGameUIMan()->MessageBox("Game_QuickTalkReset", GetGameUIMan()->GetStringFromTable(8592), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
}

void CDlgChat::TalkReset()
{
	LoadQuickTalk(true);
	ReBuildQuickTalkButton();
	OnEventLButtonDown_Lst_Word(0,0,NULL);
}

class CompareDlgChatQuickTalkByFavorAscend
{
public:
	bool operator ()(CDlgChat::QUICK_TALK& p1, CDlgChat::QUICK_TALK& p2)
	{
		return p1.strName > p2.strName;
	}
};

class CompareDlgChatQuickTalkByFavorDescend
{
public:
	bool operator ()(CDlgChat::QUICK_TALK& p1, CDlgChat::QUICK_TALK& p2)
	{
		return p1.strName < p2.strName;
	}
};

void CDlgChat::OnCommand_TalkSortAscend(const char *szCommand)
{
	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_Word");
	int iCurSel = pLst->GetCurSel();
	if (iCurSel>0 && (iCurSel+1)<=pLst->GetCount())
	{
		std::swap(m_vecQuickTalk[iCurSel], m_vecQuickTalk[iCurSel-1]);
		iCurSel--;
	}
	//pLst->SortItems(AUILISTBOX_SORT_ASCENT, AUILISTBOX_SORTTYPE_STRING, 0);
 	//BubbleSort(m_vecQuickTalk.begin(), m_vecQuickTalk.end(),CompareDlgChatQuickTalkByFavorDescend());
	ReBuildQuickTalkButton();
	pLst->SetCurSel(iCurSel);
	//OnEventLButtonDown_Lst_Word(0,0,NULL);
}

void CDlgChat::OnCommand_TalkSortDescend(const char *szCommand)
{
	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_Word");
	int iCurSel = pLst->GetCurSel();
	if (iCurSel>=0 && (iCurSel+1)<pLst->GetCount())
	{
		std::swap(m_vecQuickTalk[iCurSel], m_vecQuickTalk[iCurSel+1]);
		iCurSel++;
	}
// 	pLst->SortItems(AUILISTBOX_SORT_DESCENT, AUILISTBOX_SORTTYPE_STRING, 0);
// 	BubbleSort(m_vecQuickTalk.begin(), m_vecQuickTalk.end(),CompareDlgChatQuickTalkByFavorAscend());
	ReBuildQuickTalkButton();
	pLst->SetCurSel(iCurSel);
//	OnEventLButtonDown_Lst_Word(0,0,NULL);
}

void CDlgChat::OnCommand_LocalChatInfo(const char *szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Chat_Edit");
	UpdateChatSetInfo();
	pDlg->Show(!pDlg->IsShow());
	pDlg->AlignTo(this, AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_BOTTOM);
}

void CDlgChat::OnCommand_LocalChatInfoSave(const char *szCommand)
{
	EC_LOCAL_SETTING ls = g_pGame->GetConfigs()->GetLocalSettings();
	ls.local_chat.bChannel[5] = ((PAUICHECKBOX)GetDlgItem("Ch_Up6"))->IsChecked();
	ls.local_chat.bChannel[8] = ((PAUICHECKBOX)GetDlgItem("Ch_Up9"))->IsChecked();
	ls.local_chat.bChannel[9] = ((PAUICHECKBOX)GetDlgItem("Ch_Up10"))->IsChecked();
	ls.local_chat.bChannel[10] = ((PAUICHECKBOX)GetDlgItem("Ch_Up11"))->IsChecked();
	ls.local_chat.bChannel[16] = ((PAUICHECKBOX)GetDlgItem("Ch_Up17"))->IsChecked();
	ls.local_chat.bChannel[17] = ((PAUICHECKBOX)GetDlgItem("Ch_Up18"))->IsChecked();
	ls.local_chat.bChannel[18] = ((PAUICHECKBOX)GetDlgItem("Ch_Up19"))->IsChecked();
	ls.local_chat.bChannel[19] = ((PAUICHECKBOX)GetDlgItem("Ch_Up20"))->IsChecked();
	g_pGame->GetConfigs()->SetLocalSettings(ls);
	RebuildChatContents();
	Show(false);
}

void CDlgChat::OnCommand_LocalChatSet(const char *szCommand)
{
	AString strName;

	if (strstr(szCommand, "Ch_Up"))
	{
		int index = atoi(szCommand + strlen("Ch_Up"));
		strName.Format("Ch_Down%d", index);
		PAUICHECKBOX pChkUp = (PAUICHECKBOX)GetDlgItem(szCommand);
		PAUICHECKBOX pChkDown = (PAUICHECKBOX)GetDlgItem(strName);
		pChkDown->Check(!pChkUp->IsChecked());
	}
	else if (strstr(szCommand, "Ch_Down"))
	{
		int index = atoi(szCommand + strlen("Ch_Down"));
		strName.Format("Ch_Up%d", index);
		PAUICHECKBOX pChkUp = (PAUICHECKBOX)GetDlgItem(strName);
		PAUICHECKBOX pChkDown = (PAUICHECKBOX)GetDlgItem(szCommand);
		pChkUp->Check(!pChkDown->IsChecked());
	}
}

void CDlgChat::OnCommand_SettingGame(const char *szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_SettingGame");
	GetGameUIMan()->m_pDlgSettingCurrent = (CDlgSetting*)pDlg;
	GetGameUIMan()->m_pDlgSettingCurrent->Show(true);
}
void CDlgChat::OnCommand_CustomMode(const char* szCommand)
{
	if (strstr(szCommand, "custommode"))
	{
       int index = atoi(szCommand + strlen("custommode"));
       PAUICOMBOBOX pCombo = (PAUICOMBOBOX)GetDlgItem("Combo_Channel");
	   pCombo->SetCurSel(index+5);
	   OnCommand_set("setchannel");
	}
}
void CDlgChat::OnCommand_ChatSetting(const char* szCommand)
{  
    bool bShow = GetGameUIMan()->GetDialog("Win_Chat_Setting")->IsShow();
	GetGameUIMan()->GetDialog("Win_Chat_Setting")->Show(!bShow);

}
void CDlgChat::RebuildChatContents(int nChannelSet)
{
	if (nChannelSet != -1)
	{
		m_nActiveChannelSet = nChannelSet;
	}
	int i;
	EC_GAME_SETTING gs = g_pGame->GetConfigs()->GetGameSettings();
	EC_LOCAL_SETTING ls = g_pGame->GetConfigs()->GetLocalSettings();

	
	CDlgChat *pChat = (CDlgChat *)GetGameUIMan()->GetDialog("Win_ChatInfo");
	PAUITEXTAREA pText1 = (PAUITEXTAREA)pChat->GetDlgItem("Txt_Content");
	pChat = (CDlgChat *)GetGameUIMan()->GetDialog("Win_ChatInfo2");
	PAUITEXTAREA pText2 = (PAUITEXTAREA)pChat->GetDlgItem("Txt_Content");
	PAUITEXTAREA pText = NULL;

	pText1->SetText(_AL(""));
	pText2->SetText(_AL(""));
	if( m_nActiveChannelSet < EC_USERCHANNEL_NUM - 1 )
		m_nChatMsgCount1 = 0;
	else
		m_nChatMsgCount2 = 0;

	for( i = 0; i < (int)m_vecChatMsg.size(); i++ )
	{
		if( !gs.bChannel[m_nActiveChannelSet][m_vecChatMsg[i].cChannel] )
			continue;
		pText = pText1;
		int cChannel = m_vecChatMsg[i].cChannel;
		if (cChannel == GP_CHAT_DAMAGE		||
			cChannel == GP_CHAT_SYSTEM		||
			cChannel == GP_CHAT_BROADCAST	||
			cChannel == GP_CHAT_MISC		||
			(cChannel>=CLIENT_CHAT_MIN) && (cChannel<CLIENT_CHAT_MAX))
		{
			if (ls.local_chat.bChannel[cChannel])
				pText = pText2;
		}

		if( a_strlen(pText->GetText()) > 0 )
			pText->AppendText(_AL("\r"));

		CECIvtrItem* pItem = m_vecChatMsg[i].pItem;
		sAchievement* pAchiv = m_vecChatMsg[i].pAchievement;

		ACString strName;
		A3DCOLOR color;

		if (pItem)
			CDlgChat::TransformItemColorName(pItem, strName, color);
		else
			CDlgChat::TransformAchievementColorName(pAchiv, strName, color);

		pText->AppendText(m_pszColor[m_vecChatMsg[i].cChannel]);
		
		if (m_vecChatMsg[i].cChannel==GP_CHAT_SYSTEM || m_vecChatMsg[i].cChannel==GP_CHAT_SPEAKER
			 || m_vecChatMsg[i].cChannel==GP_CHAT_RUMOR)
		{
			ACString strChannelIcon;
			EditBoxItemBase item(enumEIImage);
			item.SetImageIndex(CECGameUIMan::ICONLIST_CHANNEL);			
			int indexImg;
			if (m_vecChatMsg[i].cChannel==GP_CHAT_SYSTEM)
				indexImg = 0;
			else if (m_vecChatMsg[i].cChannel==GP_CHAT_SPEAKER)
				indexImg = 1;
			else if (m_vecChatMsg[i].cChannel==GP_CHAT_RUMOR)
				indexImg = 2;
			item.SetImageFrame(indexImg);
			strChannelIcon = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			pText->AppendText(strChannelIcon);
		}
		else
		{
			pText->AppendText(GetStringFromTable(13100+m_vecChatMsg[i].cChannel));
		}
		pText->AppendText(m_vecChatMsg[i].strMsg, m_vecChatMsg[i].nMsgIndex, strName, color);

		// Set achievement editbox item color.
		if (pItem == NULL && pAchiv != NULL)
		{
			EditBoxItemsSet* pItemSet = (EditBoxItemsSet*) &pText->GetItemSet();
			EditBoxItemMap::iterator iter = pItemSet->GetItemIterator();

			while(!iter.is_eof())
			{
				EditBoxItemBase* pItem = iter->second;
				if (pItem->GetType() == enumEITask && pItem->GetMsgIndex() == m_vecChatMsg[i].nMsgIndex)
				{
					pItem->SetName(strName);
					pItem->SetColor(color);

					//	Just for calling AUITextArea::BuildRenderCache() to refresh achievement color.
					pText->SetFontAndColor(NULL, -1, pText->GetColor());

					break;
				}

				++iter;
			}
		}

		if( m_nActiveChannelSet < EC_USERCHANNEL_NUM - 1 )
			m_nChatMsgCount1++;
		else
			m_nChatMsgCount2++;
	}
	
	pText1->ScrollToBottom();
	pText2->ScrollToBottom();
}

int CDlgChat::GetChatAlpha()
{
	return m_nCurChatAlpha;
}

int CDlgChat::GetActiveChannelSet()
{
	return m_nActiveChannelSet;
}

abase::vector<CDlgChat::CHAT_MSG> & CDlgChat::GetAllChatMsgs()
{
	return m_vecChatMsg;
}

int CDlgChat::GetMsgCount(int nWin)
{
	return 1 == nWin ? m_nChatMsgCount1 : m_nChatMsgCount2;
}

void CDlgChat::IncreaseMsgCount(int nWin)
{
	if( 1 == nWin )
		m_nChatMsgCount1++;
	else
		m_nChatMsgCount2++;
}

void CDlgChat::OnEventMouseMove_Txt_Content(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;
	AUITEXTAREA_NAME_LINK Link;
	
	GetNameLinkMouseOn(x, y, pObj, &Link);
	if( Link.strName.GetLength() == 0 )
	{
		AUITEXTAREA_EDITBOX_ITEM Item;
		GetItemLinkItemOn(x, y, pObj, &Item);
	}
}

void CDlgChat::OnEventLButtonDown_Img_ZoomIn(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	GetGame()->ChangeCursor(RES_CUR_DRAG);
	SetCaptureObject(pObj);
	m_nMouseLastX = GET_X_LPARAM(lParam);
	m_nMouseLastY = GET_Y_LPARAM(lParam);
	m_nOldSize = GetSize();
}

void CDlgChat::OnEventLButtonUp_Img_ZoomIn(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	GetGame()->ChangeCursor(RES_CUR_NORMAL);
	SetCaptureObject(NULL);
}

void CDlgChat::OnEventMouseMove_Img_ZoomIn(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	GetGame()->ChangeCursor(RES_CUR_DRAG);
	if( GetCaptureObject() == pObj )
	{
		SIZE size = m_nOldSize;
		size.cx += GET_X_LPARAM(lParam) - m_nMouseLastX;
		size.cy += m_nMouseLastY - GET_Y_LPARAM(lParam);
		SetSize(size.cx, size.cy);
		float fWindowScale = GetGameUIMan()->GetWindowScale();
		a_Clamp(m_nDefaultWidth, 314, 800);
		a_Clamp(m_nDefaultHeight, 100, 600);
		m_nWidth = int(m_nDefaultWidth * fWindowScale);
		m_nHeight = int(m_nDefaultHeight * fWindowScale);
		size = GetSize();
		GetDlgItem("Txt_Content")->SetSize(size.cx, size.cy);

		pObj->SetPos(size.cx - pObj->GetSize().cx, 0);

		RearrangeWindow();
		if (m_szName == "Win_ChatInfo")
		{
			GetGameUIMan()->m_pDlgChat2->AlignTo(this, 
				AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
				AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_TOP);
			PAUITEXTAREA pText = (PAUITEXTAREA)GetDlgItem("Txt_Content");
			RebuildChatContents(m_nActiveChannelSet);
			
			//PAUIOBJECT pObj = GetDlgItem("Btn_Edit1");
			//pObj->SetPos(size.cx - pObj->GetSize().cx, size.cy - pObj->GetSize().cy);
		}
		else if (m_szName == "Win_ChatInfo2")
		{
			PAUITEXTAREA pText = (PAUITEXTAREA)GetDlgItem("Txt_Content");
			RebuildChatContents(m_nActiveChannelSet);

			PAUIOBJECT pObj = GetDlgItem("Btn_Lock");
			pObj->SetPos(pObj->GetPos().x, size.cy - pObj->GetSize().cy);
			pObj = GetDlgItem("Btn_Clear");
			pObj->SetPos(pObj->GetPos().x, size.cy - pObj->GetSize().cy);
			pObj = GetDlgItem("Img_Line");
			pObj->SetPos(0, size.cy - pObj->GetSize().cy);
		}
	}
}

void CDlgChat::OnEventLButtonDown_Txt_Content(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;

	AUITEXTAREA_NAME_LINK Link;
	GetGameUIMan()->m_bClickedChatPart = GetNameLinkMouseOn(x, y, pObj, &Link);
	if( Link.strName.GetLength() != 0 )
	{
		GetGameUIMan()->SwitchToPrivateChat(Link.strName,false,true);
	}
	else
	{
		AUITEXTAREA_EDITBOX_ITEM Item;
		Item.m_pItem = NULL;
		GetItemLinkItemOn(x, y, pObj, &Item);
		if( Item.m_pItem != NULL )
		{
			GetGameUIMan()->m_bClickedUserName = true;

			if( Item.m_pItem->GetType() == enumEIIvtrlItem )
			{
				for (size_t i = 0; i < m_vecChatMsg.size(); i++)
				{
					if (m_vecChatMsg[i].nMsgIndex == Item.m_pItem->GetMsgIndex() )
					{
						if (m_vecChatMsg[i].pItem)
						{
							CECIvtrItem *pItem = m_vecChatMsg[i].pItem;
							GetGameUIMan()->CommonMessage("Win_ItemDesc", "set_item", (unsigned int)pItem, (long)this);

							break;
						}
					}
				}
			}
			else if( Item.m_pItem->GetType() == enumEICoord )
			{
				A3DVECTOR3 vec;
				int idInstance;
				a_sscanf(Item.m_pItem->GetInfo(), _AL("%f%f%f%d"), 
					&vec.x, &vec.y, &vec.z, &idInstance);
//				if( idInstance != GetWorld()->GetInstanceID() )
//				{
//					PAUIDIALOG pMsgBox;
//					m_pAUIManager->MessageBox("", GetStringFromTable(762), MB_OK,
//						A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
//					pMsgBox->SetLife(3);
//				}
//				else
				{
					GetGameUIMan()->m_pDlgMiniMap->m_dwTempTargetTime = GetTickCount();
					GetGameUIMan()->m_pDlgMiniMap->m_vecTempTarget = vec;
					GetGameUIMan()->m_pDlgMiniMap->m_idTempTargetInstance = idInstance;

					g_ECAutoMove.SearchPath(GetHostPlayer()->GetPos(), vec, idInstance);
					
					if( idInstance == GetWorld()->GetInstanceID() && GetHostPlayer()->GetModelTempArrow() )
						GetHostPlayer()->GetModelTempArrow()->PlayActionByName("动画", 1.0f, true);
				}
			}
			// set achievement as task.
			else if( (Item.m_pItem->GetType() == enumEITask) )
			{
				for (size_t i = 0; i < m_vecChatMsg.size(); i++)
				{
					if (m_vecChatMsg[i].nMsgIndex != Item.m_pItem->GetMsgIndex() )
						continue;

					if (m_vecChatMsg[i].pAchievement)
					{
						GetGameUIMan()->CommonMessage("Win_Achievement_Desc", "set_achievement", (unsigned int)m_vecChatMsg[i].pAchievement, (long)this);
						break;
					}
				}
			}
		}
	}
}

void CDlgChat::OnEventLButtonUp_Txt_Content(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;

	AUITEXTAREA_NAME_LINK Link;
	GetGameUIMan()->m_bClickedChatPart = GetNameLinkMouseOn(x, y, pObj, &Link);
	if( Link.strName.GetLength() != 0 )
	{
		GetGameUIMan()->SwitchToPrivateChat(Link.strName,true,true);
	}
	else
	{
		AUITEXTAREA_EDITBOX_ITEM Item;
		Item.m_pItem = NULL;
		GetItemLinkItemOn(x, y, pObj, &Item);
		if( Item.m_pItem != NULL )
			GetGameUIMan()->m_bClickedUserName = true;
		ChangeFocus(NULL);
	}
}

void CDlgChat::OnEventRButtonUp_Txt_Content(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - p->X;
	int y = GET_Y_LPARAM(lParam) - p->Y;

	AUITEXTAREA_NAME_LINK Link;
	GetNameLinkMouseOn(x, y, pObj, &Link);
	if( Link.strName.GetLength() == 0 ) return;

	GNET::Privilege *pPrivilege = GetGame()->GetPrivilege();
	int idPlayer = GetGameRun()->GetPlayerID(Link.strName);

	if( (wParam & MK_CONTROL) && pPrivilege->Has_Toggle_NameID() )
	{
		ACString strText;

		strText.Format(_AL("ID of %s: %d"), Link.strName, idPlayer);
		GetGameUIMan()->MessageBox("", strText, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if( idPlayer > 0 )
	{
		x -= p->X;
		y -= p->Y + GetGameUIMan()->GetDialog("Win_QuickAction")->GetSize().cy;
		GetGameUIMan()->PopupPlayerContextMenu(idPlayer, x, y);
	}
}

bool CDlgChat::GetNameLinkMouseOn(int x, int y,
	PAUIOBJECT pObj, P_AUITEXTAREA_NAME_LINK pLink)
{
	bool bClickedChatPart = false;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT ptPos = pObj->GetPos();
	PAUITEXTAREA pText = (PAUITEXTAREA)pObj;

	GetGame()->ChangeCursor(RES_CUR_NORMAL);
	if( pText->GetHitArea(x - ptPos.x, y - ptPos.y) == AUITEXTAREA_RECT_FRAME )
	{
		int i;
		abase::vector<AUITEXTAREA_NAME_LINK> &vecNameLink = pText->GetNameLink();

		x += p->X;
		y += p->Y;
		for( i = 0; i < (int)vecNameLink.size(); i++ )
		{
			if( vecNameLink[i].rc.PtInRect(x, y) )
			{
				GetGame()->ChangeCursor(RES_CUR_HAND);
				*pLink = vecNameLink[i];
				break;
			}
		}
		bClickedChatPart = true;
	}

	return bClickedChatPart;
}

bool CDlgChat::GetItemLinkItemOn(int x, int y,
	PAUIOBJECT pObj, AUITEXTAREA_EDITBOX_ITEM *pLink)
{
	bool bClickedChatPart = false;
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	POINT ptPos = pObj->GetPos();
	PAUITEXTAREA pText = (PAUITEXTAREA)pObj;

	GetGame()->ChangeCursor(RES_CUR_NORMAL);
	if( pText->GetHitArea(x - ptPos.x, y - ptPos.y) == AUITEXTAREA_RECT_FRAME )
	{
		int i;
		abase::vector<AUITEXTAREA_EDITBOX_ITEM> &vecItemLink = pText->GetEditBoxItems();

		x += p->X;
		y += p->Y;
		for( i = 0; i < (int)vecItemLink.size(); i++ )
		{
			if( (vecItemLink[i].m_pItem->GetType() == enumEIIvtrlItem ||
				vecItemLink[i].m_pItem->GetType() == enumEICoord || vecItemLink[i].m_pItem->GetType() == enumEITask)
				&& vecItemLink[i].rc.PtInRect(x, y) )
			{
				GetGame()->ChangeCursor(RES_CUR_HAND);
				*pLink = vecItemLink[i];
				break;
			}
		}
		bClickedChatPart = true;
	}

	return bClickedChatPart;
}

void CDlgChat::OnEventChar_Txt_Speech(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( m_szName == "Win_Chat" || m_szName == "Win_KfpkChat" )
		//DetermineChannelByText(pObj->GetText());
	{    
		//切频道
		if(wParam == VK_SPACE)
		{
			ACString strText = pObj->GetText();
			ACString strFlag ;
			for( int i = 1; i < sizeof(a_szFlag) / sizeof(ACHAR *); i++ )
			{
				strFlag = a_szFlag[i] ;
				strFlag +=  _AL(" ");
				if( a_stricmp(strText, strFlag) == 0 )
				{
					m_iSpeakWay = i;
					CheckRadioButton(1, i + 1);
					pObj->SetText(_AL(""));
					break;
				}
			}
		}
	   m_strCurChatText = pObj->GetText();
	}
}

void CDlgChat::OnEventKeyDown_Txt_Speech(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( m_szName == "Win_Chat" || m_szName == "Win_KfpkChat" )
	{
		if( wParam == VK_UP && m_nCurHistory > 0 )
		{
			m_nCurHistory--;
			pObj->SetText(m_vecHistory[m_nCurHistory].strMsg);
			if( m_vecHistory[m_nCurHistory].nPack >= 0 )
			{
				CECInventory *pPack = GetHostPlayer()->GetPack(m_vecHistory[m_nCurHistory].nPack);
				if( pPack )
				{
					CECIvtrItem *pItem = pPack->GetItem(m_vecHistory[m_nCurHistory].nSlot);
					if( pItem )
					{
						ACString strName;
						A3DCOLOR color;
						TransformItemColorName(pItem, strName, color);
						((PAUIEDITBOX)pObj)->SetTextAndItem(m_vecHistory[m_nCurHistory].strMsg, strName, color);
					}
				}

				((PAUIEDITBOX)pObj)->SetIvtrItemLocInfo(m_vecHistory[m_nCurHistory].nPack, m_vecHistory[m_nCurHistory].nSlot);
			}
			else if (m_vecHistory[m_nCurHistory].nAchievement >= 0)
			{
				PAUIEDITBOX pEdit = (PAUIEDITBOX) pObj;
				pEdit->SetTextAndItem(m_vecHistory[m_nCurHistory].strMsg);

				if (pEdit->GetItemCount(enumEITask) > 0)
				{
					EditBoxItemMap::iterator& it = ((EditBoxItemsSet*) (&pEdit->GetItemSet()))->GetItemIterator();
					while(!it.is_eof())
					{
						EditBoxItemBase* pItem = it->second;
						if (pItem->GetType() == enumEITask)
						{
							CECAchievementMan* pMan = GetHostPlayer()->GetAchievementMan();
							sAchievement* achiv = pMan->GetAchievement(m_vecHistory[m_nCurHistory].nAchievement);

							
							ACString strName;
							A3DCOLOR color;
							CDlgChat::TransformAchievementColorName(achiv, strName, color);

							pItem->SetColor(color);
							pItem->SetName(strName);

							pItem->SetExtraData(&m_vecHistory[m_nCurHistory].nAchievement, sizeof(unsigned int));
							break;
						}

						++it;
					}
				}
			}
							
		}
		else if( wParam == VK_DOWN && m_nCurHistory < (int)m_vecHistory.size() )
		{
			m_nCurHistory++;
			if( m_nCurHistory < (int)m_vecHistory.size() )
			{
				pObj->SetText(m_vecHistory[m_nCurHistory].strMsg);
				if( m_vecHistory[m_nCurHistory].nPack >= 0 )
				{
					CECInventory *pPack = GetHostPlayer()->GetPack(m_vecHistory[m_nCurHistory].nPack);
					if( pPack )
					{
						CECIvtrItem *pItem = pPack->GetItem(m_vecHistory[m_nCurHistory].nSlot);
						if( pItem )
						{
							ACString strName;
							A3DCOLOR color;
							TransformItemColorName(pItem, strName, color);
							((PAUIEDITBOX)pObj)->SetTextAndItem(m_vecHistory[m_nCurHistory].strMsg, strName, color);
						}
					}
				}
				else if (m_vecHistory[m_nCurHistory].nAchievement >= 0)
				{
					PAUIEDITBOX pEdit = (PAUIEDITBOX) pObj;
					pEdit->SetTextAndItem(m_vecHistory[m_nCurHistory].strMsg);

					if (pEdit->GetItemCount(enumEITask) > 0)
					{
						EditBoxItemMap::iterator& it = ((EditBoxItemsSet*) (&pEdit->GetItemSet()))->GetItemIterator();
						while(!it.is_eof())
						{
							EditBoxItemBase* pItem = it->second;
							if (pItem->GetType() == enumEITask)
							{
								CECAchievementMan* pMan = GetHostPlayer()->GetAchievementMan();
								sAchievement* achiv = pMan->GetAchievement(m_vecHistory[m_nCurHistory].nAchievement);

								ACString strName;
								A3DCOLOR color;
								CDlgChat::TransformAchievementColorName(achiv, strName, color);

								pItem->SetColor(color);
								pItem->SetName(strName);

								pItem->SetExtraData(&m_vecHistory[m_nCurHistory].nAchievement, sizeof(unsigned int));
								break;
							}

							++it;
						}
					}
				}
				if(m_vecHistory[m_nCurHistory].nPack>=0)
					((PAUIEDITBOX)pObj)->SetIvtrItemLocInfo(m_vecHistory[m_nCurHistory].nPack, m_vecHistory[m_nCurHistory].nSlot);
			}
			else
				pObj->SetText(m_strCurChatText);
		}
		//DetermineChannelByText(pObj->GetText());
		m_strCurChatText = pObj->GetText();
	}
	else if( m_szName == "Win_WhisperChat" )
	{
		if( int(wParam) == VK_RETURN && AUI_PRESS(VK_CONTROL) ) 
			OnCommand_send("send");
	}
}

void CDlgChat::OnEventSysChar_Txt_Speech(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( m_szName == "Win_WhisperChat" )
	{
		if( (TCHAR(wParam) == 's' || TCHAR(wParam) == 'S') )
			OnCommand_send("send");
	}
}

void CDlgChat::DetermineChannelByText(const ACHAR *pszText)
{
	int i;

	m_iSpeakWay = 0;
	CheckRadioButton(1, 1);
	for( i = 1; i < sizeof(a_szFlag) / sizeof(ACHAR *); i++ )
	{
		if( a_strstr(pszText, a_szFlag[i]) == pszText )
		{
			m_iSpeakWay = i;
			CheckRadioButton(1, i + 1);
			break;
		}
	}
	m_strCurChatText = pszText;
	if(i != sizeof(a_szFlag) / sizeof(ACHAR *))
	{
		m_strCurChatText.CutRight(a_strlen(a_szFlag[i])-1);
	}
	if (GetCheckedRadioButton(1)<=0)
	{
		CheckRadioButton(1, 1);
		m_strCurChatText = _AL("");
		PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Speech");
		pEdit->SetText(_AL(""));
	}
}

void CDlgChat::OnEventLButtonUp_Face(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUIDIALOG pChat;
	PAUIEDITBOX pEdit;
	int nIndex = atoi(pObj->GetName() + strlen("Face_"));

	if( GetDataPtr("ptr_AUIDialog") )
		pChat = (PAUIDIALOG)GetDataPtr("ptr_AUIDialog");
	else
		pChat = GetGameUIMan()->m_pDlgChat;

	if( strstr(pChat->GetName(), "Win_FriendChat") )
		pEdit = (PAUIEDITBOX)pChat->GetDlgItem("Txt_Content");
	else if( stricmp(pChat->GetName(), "Win_ChannelChat") == 0 )
		pEdit = (PAUIEDITBOX)pChat->GetDlgItem("Txt_Content");
	else if( stricmp(pChat->GetName(), "Win_ChatSpec") == 0 )
		pEdit = (PAUIEDITBOX)pChat->GetDlgItem("Txt_Content");
	else
		pEdit = (PAUIEDITBOX)pChat->GetDlgItem("DEFAULT_Txt_Speech");

	int nEmotion = GetHostPlayer()->GetCurEmotionSet();
	A2DSprite* pSprite = NULL;
	abase::vector<AUITEXTAREA_EMOTION>* pEmotion = NULL;
	GetGameUIMan()->GetEmotionList(&pSprite, &pEmotion, nEmotion);
	if( pSprite && pEmotion && nIndex < (int)pEmotion->size() )
	{
		if (pEdit->GetItemCount(enumEIEmotion) < 3)
			pEdit->AppendItem(enumEIEmotion, 0, L"W", MarshalEmotionInfo(nEmotion, nIndex));
	}

	GetGameUIMan()->BringWindowToTop(pChat);
	pChat->ChangeFocus(pEdit);
}

void CDlgChat::OnEventLButtonDown_Lst_Word(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_Word");
	int index = pLst->GetCurSel();
	if (index<0 || index>=pLst->GetCount())
	{
		return;
	}
	PAUIEDITBOX pEdtName = (PAUIEDITBOX)GetDlgItem("Edt_Command");
	PAUIEDITBOX pEdtNoTarget = (PAUIEDITBOX)GetDlgItem("Edt_NoAim");
	PAUIEDITBOX pEdtTarget = (PAUIEDITBOX)GetDlgItem("Edt_Aim");
	ACString strName = m_vecQuickTalk[index].strName;
// 	if (strName.GetLength()>0)
// 	{
// 		strName.CutLeft(1);
// 	}
	pEdtName->SetText(strName);
	pEdtNoTarget->SetText(m_vecQuickTalk[index].strNoTarget);
	pEdtTarget->SetText(m_vecQuickTalk[index].strTarget);
}

bool CDlgChat::IsLocked()
{
	return m_bLocked;
}

void CDlgChat::OnCommand_send(const char * szCommand)
{
	PAUIOBJECT pEdit = GetDlgItem("DEFAULT_Txt_Speech");
	ACString strText = pEdit->GetText();
	if( strText.GetLength() <= 0 ) return;
	GetGameUIMan()->FilterBadWords(strText);

	int idPlayer = GetData();
	const ACHAR *pszName = GetGameRun()->GetPlayerName(idPlayer, true);

	GetGameSession()->SendPrivateChatData(pszName, strText, GNET::CHANNEL_NORMAL, idPlayer, -1, 0, true);

	ACHAR szName[80], szText[512], szMsg[512];
	AUI_ConvertChatString(pszName, szName);
	AUI_ConvertChatString(strText, szText, false);
	a_sprintf(szMsg, GetStringFromTable(233), szName, szText);
	GetGameUIMan()->AddChatMessage(szMsg, GP_CHAT_WHISPER, idPlayer, pszName, 0, GetHostPlayer()->GetCurEmotionSet());

	pEdit->SetText(_AL(""));
	ChangeFocus(pEdit);
}

void CDlgChat::OnCommand_joinchannel(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_ChannelJoin");
	if( pDlg  )
		pDlg->Show(!pDlg->IsShow());
}

void CDlgChat::OnShowDialog()
{
	CDlgBase::OnShowDialog();
	
	if( m_szName == "Win_WhisperChat" )
	{
		GetGameUIMan()->m_pDlgMinimizeBar[0]->RestoreDialog(this);
		ChangeFocus(GetDlgItem("DEFAULT_Txt_Speech"));
	}
	if (stricmp(m_szName, "Win_PopTalk") == 0)
	{
		if( m_bLoaded || LoadQuickTalk() )
		{
			CDlgChat *pDlgPopMan = (CDlgChat*)GetGameUIMan()->GetDialog("Win_PopfaceManage");
			pDlgPopMan->ReBuildQuickTalkButton();
		}
		else
			AUI_ReportError(__LINE__, 1,"CDlgChat::OnShowDialog(), quick talk file error!");
	}
	else if (stricmp(m_szName, "Win_PopfaceManage") == 0)
	{
		if( m_bLoaded || LoadQuickTalk() )
		{
			ReBuildQuickTalkButton();	
			OnEventLButtonDown_Lst_Word(0,0,NULL);
			
		}
		else
			AUI_ReportError(__LINE__, 1,"CDlgChat::OnShowDialog(), quick talk file error!");
	}
	else if(stricmp(m_szName,"Win_Chat") == 0)
	{
	    RefreshChannelTab();
	}
}

void CDlgChat::OnTick()
{
	if( stricmp(m_szName, "Win_Chat") == 0 )
	{
		GetDlgItem("Btn_Class")->Enable(GetHostPlayer()->GetCircleID()!=0);
	}
	else if( stricmp(m_szName, "Win_Popface") == 0 )
	{
		PAUIDIALOG pDlg = (PAUIDIALOG)GetDataPtr("ptr_AUIDialog");
		GetDlgItem("Btn_Talk")->Show(pDlg && strstr(pDlg->GetName(), "Win_ChatSpec")==0);
		if( pDlg && !pDlg->IsShow() )
			Show(false);
		else
		{
			char szName[20];
			PAUIIMAGEPICTURE pImage;
			int i;
			A2DSprite* pSprite = NULL;
			abase::vector<AUITEXTAREA_EMOTION>* pEmotion = NULL;
			GetGameUIMan()->GetEmotionList(&pSprite, &pEmotion, GetHostPlayer()->GetCurEmotionSet());
			if( pSprite && pEmotion )
			{
				for( i = 0; ; i++ )
				{
					sprintf(szName, "Face_%02d", i);
					pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
					if( !pImage ) break;

					if( i < (int)pEmotion->size() )
					{
						pImage->SetCover(pSprite, (*pEmotion)[i].nStartPos);
					}
				}
				PAUIDIALOG pDlgMouseOn;
				PAUIOBJECT pObjMouseOn;
				GetGameUIMan()->GetMouseOn(&pDlgMouseOn, &pObjMouseOn);
				if( pDlgMouseOn && pDlgMouseOn == this &&
					pObjMouseOn && strncmp(pObjMouseOn->GetName(), "Face_", strlen("Face_") ) == 0 )
				{
					pImage = (PAUIIMAGEPICTURE)pObjMouseOn;
					i = atoi(pImage->GetName() + strlen("Face_"));
					if( i < (int)pEmotion->size() )
					{
						int nTick = int(GetTickCount() * 0.06);
						int nBase = (*pEmotion)[i].nStartPos;
						int nFrame = (*pEmotion)[i].nNumFrames;
						int nThisTick = nTick % (*pEmotion)[i].nFrameTick[nFrame - 1];
						int nOffset = 0;
						while( nOffset < nFrame - 1 && nThisTick > (*pEmotion)[i].nFrameTick[nOffset] )
							nOffset++;
						pImage->SetCover(pSprite, nBase + nOffset );
					}
				}
			}
		}
	}
	else if( stricmp(m_szName, "Win_PopTalk") == 0 )
	{
		int battleType = GetHostPlayer()->GetBattleType();
		if (battleType==BATTLE_TYPE_ARENA || battleType==BATTLE_TYPE_ARENAREBORN || battleType==BATTLE_TYPE_CRSSVR || GetHostPlayer()->IsInAnonymousMap())
		{
			GetGameUIMan()->GetDialog("Win_Popface")->Show(true);
			Show(false);
		}

		if (m_iCurrentPage != m_pScl_TalkView->GetBarLevel())
		{
			m_iCurrentPage = m_pScl_TalkView->GetBarLevel();
			for (UINT i=0; i<CDLGCHAT_TYPE_PER_PAGE; i++)
			{
				AString strName;
				strName.Format(("Btn_%d"),i+1);
				PAUISTILLIMAGEBUTTON pBtn = (PAUISTILLIMAGEBUTTON)GetDlgItem(strName);
				if (i+m_iCurrentPage*CDLGCHAT_TYPE_PER_LINE<m_vecQuickTalk.size())
				{
					QUICK_TALK qtItem = m_vecQuickTalk[i+m_iCurrentPage*CDLGCHAT_TYPE_PER_LINE];
					pBtn->SetText(_AL(":")+qtItem.strName);
				}
				else
					pBtn->Show(false);
			}
		}
		UpdateHint();
	}
	else if( stricmp(m_szName, "Win_PopfaceManage") == 0 )
	{
		PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_Word");
		PAUIEDITBOX pEdtName = (PAUIEDITBOX)GetDlgItem("Edt_Command");
		PAUIEDITBOX pEdtNoTarget = (PAUIEDITBOX)GetDlgItem("Edt_NoAim");
		PAUIEDITBOX pEdtTarget = (PAUIEDITBOX)GetDlgItem("Edt_Aim");
		if (pLst->GetCurSel()<0 || pLst->GetCurSel()>=pLst->GetCount())
		{
			pEdtName->SetText(_AL(""));
			pEdtName->Enable(false);
			pEdtNoTarget->SetText(_AL(""));
			pEdtNoTarget->Enable(false);
			pEdtTarget->SetText(_AL(""));
			pEdtTarget->Enable(false);
		}
		else
		{
			pEdtName->Enable(true);
			pEdtNoTarget->Enable(true);
			pEdtTarget->Enable(true);
		}
	}
	else if( stricmp(m_szName, "Win_ChatSpecInfo") == 0 )
	{
		PAUITEXTAREA pText = (PAUITEXTAREA)GetDlgItem("Txt_Content");
		unsigned int uData = pText->GetData("time");
		uData += GetGame()->GetTickTime();
		pText->SetData(uData, "time");
		if (uData>30*60000)
		{
			pText->SetText(_AL(""));
			pText->SetData(0, "time");
		}
	}
}

bool CDlgChat::Render()
{
	bool bval = CDlgBase::Render();
	if( stricmp(m_szName, "Win_ChatInfo") == 0 )
	{
		PAUITEXTAREA pText = (PAUITEXTAREA)GetDlgItem("Txt_Content");
		if( pText->GetFirstLine() != (int)pText->GetData() )
			if( IsLocked() ) 
			{
				if( pText->GetFirstLine() + pText->GetLinesPP() == pText->GetLines() )
					OnCommand_lock("");
			}
			else if( pText->GetFirstLine() + pText->GetLinesPP() != pText->GetLines() )
				OnCommand_lock("");
		pText->SetData(pText->GetFirstLine());
	}
	if( stricmp(m_szName, "Win_ChatInfo2") == 0 )
	{
		PAUITEXTAREA pText = (PAUITEXTAREA)GetDlgItem("Txt_Content");
		if( pText->GetFirstLine() != (int)pText->GetData() )
			if( IsLocked() ) 
			{
				if( pText->GetFirstLine() + pText->GetLinesPP() == pText->GetLines() )
					OnCommand_lock("");
			}
			else if( pText->GetFirstLine() + pText->GetLinesPP() != pText->GetLines() )
				OnCommand_lock("");
			pText->SetData(pText->GetFirstLine());
	}
	return bval;
}

void CDlgChat::TransformItemColorName(CECIvtrItem *pItem, ACString &strName, A3DCOLOR &color)
{
	ACString szName, strText;
	if( !pItem )
	{
		strName = _AL("");
		color = 0xffffffff;
		return;
	}
	szName = pItem->GetName();
	if( pItem->IsEquipment())
	{
		CECIvtrEquip *pEquip = (CECIvtrEquip*)pItem;

		if (!pEquip->IsTalismanMain() && !pEquip->IsSpeaker() && !pEquip->IsChangeShapeCard())
		{
			CECStringTab* pDescTab = g_pGame->GetItemDesc();
			ACHAR szText[100];

			if( pEquip->GetEssence().num_estone )
			{
				a_sprintf(szText, pDescTab->GetWideString(ITEMDESC_ENAME), pEquip->GetEssence().num_estone);
				szName += szText;
			}
			if( pEquip->GetEssence().num_pstone )
			{
				a_sprintf(szText, pDescTab->GetWideString(ITEMDESC_PNAME), pEquip->GetEssence().num_pstone);
				szName += szText;
			}
		}
	}
	strName.Format(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(939), szName);

	ACString strColor = pItem->GetColor();
	if( strColor.GetLength() >= 7 && strColor[0] == '^' )
	{
		color = 0;
		for( int j = 0; j < 6; j++ )
		{
			ACHAR nValue = strColor[j + 1];
			if( nValue >= '0' && nValue <= '9' )
				nValue -= '0';
			else if( nValue >= 'a' && nValue <= 'z' )
				nValue = nValue - 'a' + 0xA;
			else if( nValue >= 'A' && nValue <= 'Z' )
				nValue = nValue - 'A' + 0xA;
			else
				nValue = 0;
			color = color * 0x10 + nValue;
		}
		color |= 0xFF000000;
	}
	else
		color = A3DCOLORRGB(255, 255, 255);
}

void CDlgChat::TransformAchievementColorName(sAchievement *pAchiv,  ACString &strName, A3DCOLOR &color)
{
	if( !pAchiv )
	{
		strName = _AL("");
		color = 0xffffffff;
		return;
	}

	strName.Format(_AL("【%s】"), pAchiv->name);
	ACHAR tempName[256] = {0};
	if( a_sscanf(pAchiv->name, _AL("^%6x%[^^]^%*6x"), &color, tempName) != 0)
		strName.Format(_AL("【%s】"), tempName);

	if (!pAchiv->bFinish)
		color = 0x888888;

	color |= 0xFF000000;
}

void CDlgChat::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	int zDelta = (short)HIWORD(wParam);
	if( zDelta > 0 )
		zDelta = 1;
	else
		zDelta = -1;
// 	if( m_pScl_TalkView->IsShow())
// 		m_pScl_TalkView->SetBarLevel(m_pScl_TalkView->GetBarLevel() - zDelta);
}

bool CDlgChat::SaveQuickTalk()
{
	char szPath[MAX_PATH];
	sprintf(szPath, "%s\\UserData\\QuickTalk", af_GetBaseDir());
	CreateDirectoryA(szPath, NULL);
	sprintf(szPath, "%s\\UserData\\QuickTalk\\%d.txt", af_GetBaseDir(), g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID());
	AFile afile;
	if (!afile.Open(szPath, AFILE_CREATENEW|AFILE_TEXT|AFILE_NOHEAD|AFILE_TEMPMEMORY))
	{
		return false;
	}
	for (UINT i=0; i<m_vecQuickTalk.size(); i++)
	{
		QUICK_TALK talkItem = m_vecQuickTalk[i];
		if (!talkItem.bOk)
		{
			continue;
		}
		ACString strText;
		strText.Format(_AL("%s\t%s\t%s"), talkItem.strName, talkItem.strNoTarget, talkItem.strTarget);
		AString str(WC2AS(strText));
		afile.WriteLine(str);
	}
	afile.Close();
	return true;
}

void CDlgChat::RearrangeWindow()
{
	GetGameUIMan()->m_pDlgChat1->AlignTo(GetGameUIMan()->m_pDlgChat,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_TOP);
	GetGameUIMan()->m_pDlgChat2->AlignTo(GetGameUIMan()->m_pDlgChat1,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_TOP);
	PAUIDIALOG pDlgBase;
	if (m_iCurrentShow==DLGCHAT_SHOWINFO_NONE)
	{
		pDlgBase = GetGameUIMan()->GetDialog("Win_ChatClose");
	}
	else if (m_iCurrentShow==DLGCHAT_SHOWINFO_BOTH)
	{
		pDlgBase = GetGameUIMan()->m_pDlgChat2;
	}
	else
	{
		pDlgBase = GetGameUIMan()->m_pDlgChat1;
	}
	GetGameUIMan()->GetDialog("Win_ChatSpecInfo")->AlignTo(pDlgBase,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_TOP);

}

bool CDlgChat::LoadQuickTalk(bool bReset)
{
 	char szPath[MAX_PATH];
 	sprintf(szPath, "%s\\UserData\\QuickTalk\\%d.txt", af_GetBaseDir(), g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID());
	AFile afile;
	if (!bReset && afile.Open(szPath, AFILE_OPENEXIST|AFILE_TEXT|AFILE_NOHEAD|AFILE_TEMPMEMORY))
	{
		m_vecQuickTalk.clear();
		char line[AFILE_LINEMAXLEN];
		DWORD  read_len;
		while (afile.ReadLine(line, AFILE_LINEMAXLEN, &read_len))
		{
			CSplit s(AS2WC(line));
			CSplit::VectorAWString vec = s.Split(_AL("\t"));
			if( vec.size() == 3 )
			{
				QUICK_TALK talkItem;
				talkItem.strName = vec[0];				
				talkItem.strNoTarget = vec[1];
				talkItem.strTarget = vec[2];
				talkItem.bOk = true;
				m_vecQuickTalk.push_back(talkItem);
			}
		}
		afile.Close();
	}
	else
	{
		AWScriptFile s;
		if( s.Open("surfaces\\ingame\\quicktalk.txt") )
		{
			m_vecQuickTalk.clear();
			while( !s.IsEnd() )
			{
				if( !s.GetNextToken(true) )
					break;
				QUICK_TALK talkItem;
				BEGIN_FAKE_WHILE2
					
				talkItem.strName = s.m_szToken;				
				CHECK_BREAK2(s.GetNextToken(true));
				talkItem.strNoTarget = s.m_szToken;
				CHECK_BREAK2(s.GetNextToken(true));
				talkItem.strTarget = s.m_szToken;
				m_vecQuickTalk.push_back(talkItem);
				talkItem.bOk = true;
				
				END_FAKE_WHILE2;
			}
			s.Close();
		}
		else
			return false;
	}
	m_bLoaded = true;
	return true;
}

void CDlgChat::ReBuildQuickTalkButton()
{
	if (0!=stricmp(m_szName,"Win_PopfaceManage"))
	{
		return;
	}
	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("Lst_Word");
	pLst->ResetContent();
	UINT i;
	for ( i=0; i<m_vecQuickTalk.size(); i++)
	{
		pLst->AddString(_AL(":")+m_vecQuickTalk[i].strName);
	}
	CDlgChat *pDlgQuickTalk = (CDlgChat*)GetGameUIMan()->GetDialog("Win_PopTalk");
	PAUISCROLL pScrBar = (PAUISCROLL)pDlgQuickTalk->GetDlgItem("Scl_Item");
	pScrBar->Show(m_vecQuickTalk.size()>CDLGCHAT_TYPE_PER_PAGE);
	if (m_vecQuickTalk.size()>CDLGCHAT_TYPE_PER_PAGE)
	{
		pScrBar->SetScrollRange(0, (m_vecQuickTalk.size()-1)/CDLGCHAT_TYPE_PER_LINE + 1 - CDLGCHAT_TYPE_PER_PAGE/CDLGCHAT_TYPE_PER_LINE);
		pScrBar->SetScrollStep(1);
		pScrBar->SetBarLevel(0);
		m_iCurrentPage = 0;
	}
	for (i=0; i<CDLGCHAT_TYPE_PER_PAGE; i++)
	{
		AString strName;
		strName.Format(("Btn_%d"),i+1);
		PAUISTILLIMAGEBUTTON pBtn = (PAUISTILLIMAGEBUTTON)	pDlgQuickTalk->GetDlgItem(strName);
		if (i+m_iCurrentPage*CDLGCHAT_TYPE_PER_LINE<m_vecQuickTalk.size())
		{
			QUICK_TALK qtItem = m_vecQuickTalk[i+m_iCurrentPage*CDLGCHAT_TYPE_PER_LINE];
			pBtn->SetText(_AL(":")+qtItem.strName);
			pBtn->Show(true);
		}
		else
			pBtn->Show(false);
	}
}

void CDlgChat::FilterReturn(ACString &str)
{
	for( int i = 0; i < str.GetLength()-1; i++ )
	{
		if ( str[i]== '\r' || str[i]== '\n')
		{
			str[i] = ' ';
		}
	}
}

void CDlgChat::UpdateHint()
{
	PAUIDIALOG pDlgMouseOn;
	PAUIOBJECT pObjMouseOn;
	GetGameUIMan()->GetMouseOn(&pDlgMouseOn, &pObjMouseOn);

	if (pDlgMouseOn != GetGameUIMan()->GetDialog("Win_PopTalk")) return;
	if (!pObjMouseOn) return;

	if (strstr(pObjMouseOn->GetName(),"Btn_Emotion") || strstr(pObjMouseOn->GetName(),"Btn_Talk")) return;
	if (!strstr(pObjMouseOn->GetName(),"Btn_")) return;
	
	int index = atoi(pObjMouseOn->GetName()+strlen("Btn_"))-1;
	if (index<0 ||index>27)
		return;
	QUICK_TALK qkItem = m_vecQuickTalk[index+m_iCurrentPage*CDLGCHAT_TYPE_PER_LINE];
	pObjMouseOn->SetHint(_AL(":")+qkItem.strName + _AL("\r") + qkItem.strTarget + _AL("\r") + qkItem.strNoTarget );

}

void CDlgChat::UpdateChatSetInfo()
{
	EC_GAME_SETTING gs = g_pGame->GetConfigs()->GetGameSettings();
	EC_LOCAL_SETTING ls = g_pGame->GetConfigs()->GetLocalSettings();
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Chat_Edit");

	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up6"))->Check(ls.local_chat.bChannel[5]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up9"))->Check(ls.local_chat.bChannel[8]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up10"))->Check(ls.local_chat.bChannel[9]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up11"))->Check(ls.local_chat.bChannel[10]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up17"))->Check(ls.local_chat.bChannel[16]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up18"))->Check(ls.local_chat.bChannel[17]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up19"))->Check(ls.local_chat.bChannel[18]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up20"))->Check(ls.local_chat.bChannel[19]);

	
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down6"))->Check(!ls.local_chat.bChannel[5]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down9"))->Check(!ls.local_chat.bChannel[8]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down10"))->Check(!ls.local_chat.bChannel[9]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down11"))->Check(!ls.local_chat.bChannel[10]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down17"))->Check(!ls.local_chat.bChannel[16]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down18"))->Check(!ls.local_chat.bChannel[17]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down19"))->Check(!ls.local_chat.bChannel[18]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down20"))->Check(!ls.local_chat.bChannel[19]);
	
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up6"))->Enable(gs.bChannel[m_nActiveChannelSet][5]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up9"))->Enable(gs.bChannel[m_nActiveChannelSet][8]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up10"))->Enable(gs.bChannel[m_nActiveChannelSet][9]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up11"))->Enable(gs.bChannel[m_nActiveChannelSet][10]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up17"))->Enable(gs.bChannel[m_nActiveChannelSet][16]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up18"))->Enable(gs.bChannel[m_nActiveChannelSet][17]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up19"))->Enable(gs.bChannel[m_nActiveChannelSet][18]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up20"))->Enable(gs.bChannel[m_nActiveChannelSet][19]);

	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down6"))->Enable(gs.bChannel[m_nActiveChannelSet][5]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down9"))->Enable(gs.bChannel[m_nActiveChannelSet][8]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down10"))->Enable(gs.bChannel[m_nActiveChannelSet][9]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down11"))->Enable(gs.bChannel[m_nActiveChannelSet][10]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down17"))->Enable(gs.bChannel[m_nActiveChannelSet][16]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down18"))->Enable(gs.bChannel[m_nActiveChannelSet][17]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down19"))->Enable(gs.bChannel[m_nActiveChannelSet][18]);
	((PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down20"))->Enable(gs.bChannel[m_nActiveChannelSet][19]);

	PAUICHECKBOX pChkUp[8];
	pChkUp[0] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up6");
	pChkUp[1] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up9");
	pChkUp[2] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up10");
	pChkUp[3] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up11");
	pChkUp[4] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up17");
	pChkUp[5] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up18");
	pChkUp[6] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up19");
	pChkUp[7] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Up20");
	PAUICHECKBOX pChkDown[8];
	pChkDown[0] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down6");
	pChkDown[1] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down9");
	pChkDown[2] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down10");
	pChkDown[3] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down11");
	pChkDown[4] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down17");
	pChkDown[5] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down18");
	pChkDown[6] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down19");
	pChkDown[7] = (PAUICHECKBOX)pDlg->GetDlgItem("Ch_Down20");
	for (int i=0; i<8; i++)
	{
		if (!pChkUp[i]->IsEnabled())
			pChkUp[i]->Check(false);
		if (!pChkDown[i]->IsEnabled())
			pChkDown[i]->Check(false);
	}
}

void CDlgChat::ReplaceChatNames(ACString strSrcName, ACString strDstName)
{
	int i;
	for( i = 0; i < (int)m_vecChatMsg.size(); i++ )
	{
		ACString strSrc = m_vecChatMsg[i].strMsg;
		ACString strDst;
		const ACHAR* p = strSrc;
		int iStartQuote = -1;
		int j;
		for (j=0; j<strSrc.GetLength(); ++j)
		{
			if (strSrc[j]==_AL('&'))
			{
				if (iStartQuote>=0)
				{
					ACString strName = strSrc.Mid(iStartQuote+1, j-iStartQuote-1);
					if (strName==strSrcName)
						strDst += strDstName;
					else
						strDst += strName;
					iStartQuote = -1;
				}
				else
				{
					iStartQuote = j;
				}
				strDst += strSrc[j];
			}
			else if(iStartQuote<0)
				strDst += strSrc[j];
		}
		m_vecChatMsg[i].strMsg = strDst;
	}
}

void CDlgChat::RefreshChannelTab()
{
	if( stricmp(m_szName, "Win_Chat") == 0 )
	{
	 
		EC_LOCAL_SETTING ls = g_pGame->GetConfigs()->GetLocalSettings();
		GetDlgItem("Btn_Set1")->Show(ls.local_chat.bShowChannel[0]);
		GetDlgItem("Btn_Set2")->Show(ls.local_chat.bShowChannel[1]);
		GetDlgItem("Btn_Set3")->Show(ls.local_chat.bShowChannel[2]);
		GetDlgItem("Btn_Set4")->Show(ls.local_chat.bShowChannel[4]);
		GetDlgItem("Btn_Set5")->Show(ls.local_chat.bShowChannel[3]);
		GetDlgItem("Btn_Set6")->Show(ls.local_chat.bShowChannel[6]);
		GetDlgItem("Btn_Class")->Show(ls.local_chat.bShowChannel[5]);
		GetDlgItem("Rdo_Custom01")->Show(ls.local_chat.bShowChannel[7]);
		GetDlgItem("Rdo_Custom02")->Show(ls.local_chat.bShowChannel[8]);
	}
}