// File     : DlgFrdCallBck.cpp
// Creator  : Ma Yuanzheng
// Date     : 2010.11.5
// Comment  : 快雪时晴系统界面实现
// Copyright (c) 2010 Zhuxian Studio, All Rights Reserved.


#include "DlgFrdCallBck.h"
#include "EC_Friend.h"

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "AUI//AUICTranslate.h"



#define new A_DEBUG_NEW


AUI_BEGIN_COMMAND_MAP(CDlgFrdCallBck,CDlgBase)
//To do: add command map here
AUI_ON_COMMAND("Btn_Send",OnCmdSendMail)
AUI_ON_COMMAND("Btn_Refresh",OnCmdRefresh)
AUI_ON_COMMAND("Btn_Close",OnCmdCancel)
AUI_ON_COMMAND("Combo_Model",OnCmdChooseMail)
AUI_ON_COMMAND("Lst_Friendlist",OnCmdChooseFriend)//这是双击
AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgFrdCallBck,CDlgBase)
//To do: add event map here
AUI_ON_EVENT("Lst_Friendlist",WM_LBUTTONDOWN,OnEventButtonDown)
AUI_ON_EVENT("Lst_Friendlist",WM_RBUTTONDOWN,OnEventButtonDown)
AUI_END_EVENT_MAP()


CDlgFrdCallBck::CDlgFrdCallBck(){
	iMailSentNum     = MAX_FRIENDCALLBCKMAIL; // 在好友召回系统启动前禁发邮件
	iDays            = 0;
	dwSuccTime       = 0;
	iMailSucc        = 0;
	dwSendTime       = 0;
	m_pLab_Receiver  = NULL;
	m_pLab_Title     = NULL;
	m_pTxt_Content   = NULL;
	m_pLst_Friends   = NULL;
	m_pBtn_Refresh   = NULL;
	m_pBtn_Send      = NULL;
	m_pCombo_Template= NULL;
	m_pLab_MailLeft  = NULL;
	bAMailCanSend    = false;
}

CDlgFrdCallBck::~CDlgFrdCallBck(){
}


void CDlgFrdCallBck::DoDataExchange(bool bSave){
	CDlgBase::DoDataExchange(bSave);
	if(strstr(GetName(),"Win_MailtoFriend_Success")) return;
	DDX_Control("Txt_Receiver"  ,m_pLab_Receiver);
	DDX_Control("Txt_MailTitle" ,m_pLab_Title);
	DDX_Control("Txt_Content"   ,m_pTxt_Content);
	DDX_Control("Lst_Friendlist",m_pLst_Friends);
	DDX_Control("Btn_Refresh"   ,m_pBtn_Refresh);
	DDX_Control("Btn_Send"      ,m_pBtn_Send);
	DDX_Control("Combo_Model"   ,m_pCombo_Template);
	DDX_Control("Txt_Left"      ,m_pLab_MailLeft);	
}


bool CDlgFrdCallBck::OnInitDialog(){

	if(strstr(GetName(),"Win_MailtoFriend_Success")) return true;
	AString strTabFile("Interfaces\\");
	strTabFile += m_szFilename;

	strTabFile.MakeLower();
	const char* szStrTabFile = strTabFile;
	char *pch = strstr((char*)szStrTabFile, ".");
	ASSERT(pch);
	strcpy(pch, ".stf");	// String table file.

	bool bval = ImportStringTable(strTabFile);
	if( !bval ) return AUI_ReportError(__LINE__, 1, "CDlgFrdCallBck::OnInitDialog, Failed to call ImportStringTable()");

	m_pCombo_Template->ResetContent();
	for (int i = MAILTEMPNUM - 1; i >= 0; i--) {
		ACHAR sMail[256];
		a_sprintf(sMail,GetStringFromTable(MAILTEMP_TABLE_INDEX + 19),i+1);
		m_pCombo_Template->InsertString(0,sMail);
		m_pCombo_Template->SetItemData(0,i);
	}
	int t = rand()%MAILTEMPNUM;
	m_pCombo_Template->SetCurSel(t);
	
	m_pLst_Friends->SetCurSel(0);
	ACString str;
	str.Format(GetStringFromTable(MAILTEMP_TABLE_INDEX + t),m_pLst_Friends->GetText(m_pLst_Friends->GetCurSel()),GetHostPlayer()->GetName());
	AUICTranslate trans;
	m_pTxt_Content->SetText(trans.Translate(str));
	//这时候取到的服务器时间不对，改动到SetMailNum的时候取
	//iDays = ((g_pGame->GetServerGMTTime() - g_pGame->GetTimeZoneBias() * 60)+TIME_SECOND_BIAS)/(3600 * 24);

	ACHAR  szLeft[10];
	a_sprintf(szLeft, _AL("%d/%d"), iMailSentNum, MAX_FRIENDCALLBCKMAIL);
	m_pLab_MailLeft->SetText(szLeft);
	Show(true);
	
	return true;
}

void CDlgFrdCallBck::OnShowDialog(){
	//g_pGame->GetGameSession()->GetFriendCallBckInfo();
	if(strstr(GetName(),"Win_MailtoFriend_Success")) return;
	m_pCombo_Template->SetCurSel(rand()%MAILTEMPNUM);
	m_pLst_Friends->SetCurSel(0);
	//为了判断新的一天，iDays在OnShow的时候取一下当前天数
	int inewDays = ((g_pGame->GetServerGMTTime() - g_pGame->GetTimeZoneBias() * 60)+TIME_SECOND_BIAS)/(3600 * 24);
	if(iDays==0){//第一次获取天数
		iDays = inewDays;
	}else{//不是第一次获取天数，则如果是新的一天，清零
		if(inewDays - iDays >= 1){
			iDays = inewDays;
			SetMailNum(0);
		}
	}
	BuildFriendsLst();
}

void CDlgFrdCallBck::OnTick(){

	if(strstr(GetName(),"Win_MailtoFriend_Success")) return;

	int iServerTime = g_pGame->GetServerGMTTime() - g_pGame->GetTimeZoneBias() * 60 +TIME_SECOND_BIAS;
	if(iServerTime/(3600*24) - iDays >= 1){
		iDays = iServerTime/(3600*24);
		SetMailNum(0);
	}
	if(m_pLst_Friends->GetCount()>0){
		CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();
		int curSel = m_pLst_Friends->GetCurSel();
		int iTime = pMan->GetFriendLastCallBckTimeByID(m_pLst_Friends->GetItemData(curSel));
		int iHigh = CDTIME_FRIENDCALLBCK - (iServerTime - iTime);//还有几秒
		int iLow = 0;
		ACString pszHint;
		if (iHigh <= 0 &&curSel<m_pLst_Friends->GetCount())  m_pBtn_Send->Enable(true);
		else m_pBtn_Send->Enable(false); 

		for(int i = m_pLst_Friends->GetCount()-1;i>=0;i--){
			iTime = pMan->GetFriendLastCallBckTimeByID(m_pLst_Friends->GetItemData(i));
			iHigh = CDTIME_FRIENDCALLBCK - (iServerTime - iTime);//还有几秒
			iLow = 0;
			if (iHigh <= 0){                                      //刚过10天，冷冻期已过
				CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();
				m_pLst_Friends->SetItemTextColor(i,A3DCOLORRGB(246,196,121));
				m_pLst_Friends->SetText(i,((CECFriendMan::FRIEND*)pMan->GetFriendByID(m_pLst_Friends->GetItemData(i)))->GetName());
				m_pLst_Friends->SetItemHint(i,_AL(""));
			}else if(iHigh > (3600*24)){                                     //剩余一天多到10天
				iLow = (iHigh/3600)%24;
				iHigh = iHigh/(3600*24);
				pszHint.Format(GetStringFromTable(MAILTEMP_TABLE_INDEX+16),iHigh,iLow);
				m_pLst_Friends->SetItemHint(i,pszHint);
			}else if(iHigh > 3600){            //剩余一小时多到24小时
				iLow = (iHigh/60)%60;
				iHigh = iHigh/3600;
				pszHint.Format(GetStringFromTable(MAILTEMP_TABLE_INDEX+17),iHigh,iLow);
				m_pLst_Friends->SetItemHint(i,pszHint);
			}else{                 //剩余几秒多到60分
				iLow = iHigh%60;
				iHigh = iHigh/60;
				pszHint.Format(GetStringFromTable(MAILTEMP_TABLE_INDEX+18),iHigh,iLow);
				m_pLst_Friends->SetItemHint(i,pszHint);
			}
		}
	}else{
		m_pBtn_Send->Enable(false);
	}
	if(iMailSentNum >= MAX_FRIENDCALLBCKMAIL || GetTickCount() - dwSendTime < 3000){
		m_pBtn_Send->Enable(false);
	}
}

void CDlgFrdCallBck::BuildFriendsLst(){
	int i, j;
	CECFriendMan::GROUP *pGroup;
	CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();
	if(!pMan) return;
	int iGroupNum = pMan->GetGroupNum();
	m_pLst_Friends->ResetContent();
	for( i = 0; i <iGroupNum;  i++ )
	{
		pGroup = pMan->GetGroupByIndex(i);
		for( j = 0; j < pGroup->aFriends.GetSize(); j++)
		{
			if(pGroup->aFriends[j]->iLine < 0){
				if((g_pGame->GetServerGMTTime() - g_pGame->GetTimeZoneBias()*60 + TIME_SECOND_BIAS) - pMan->GetFriendLastCallBckTimeByID(pGroup->aFriends[j]->id) >= CDTIME_FRIENDCALLBCK){
					m_pLst_Friends->InsertString(0,pGroup->aFriends[j]->GetName());
					m_pLst_Friends->SetItemTextColor(0,A3DCOLORRGB(246,196,121));
					m_pLst_Friends->SetItemData(0,pGroup->aFriends[j]->id);
				}else{
					m_pLst_Friends->InsertString(m_pLst_Friends->GetCount(),pGroup->aFriends[j]->GetName());
					m_pLst_Friends->SetItemTextColor(m_pLst_Friends->GetCount()-1,A3DCOLORRGB(128, 128, 128));
					m_pLst_Friends->SetItemData(m_pLst_Friends->GetCount()-1,pGroup->aFriends[j]->id);
				}
			}		
		}
	}
	if (m_pLst_Friends->GetCount() > 0) {
		m_pLst_Friends->SetCurSel(0);
		m_pLab_Receiver->SetText(m_pLst_Friends->GetText(m_pLst_Friends->GetCurSel()));
		ACString str;
		str.Format(GetStringFromTable(MAILTEMP_TABLE_INDEX + m_pCombo_Template->GetCurSel()),m_pLst_Friends->GetText(m_pLst_Friends->GetCurSel()),GetHostPlayer()->GetName());
		AUICTranslate trans;
		m_pTxt_Content->SetText(trans.Translate(str));
	}
	m_pBtn_Send->Enable(false);
}

void CDlgFrdCallBck::RefreshFriend(){
	BuildFriendsLst();
	//g_pGame->GetGameSession()->GetFriendCallBckInfo();
}

void CDlgFrdCallBck::OnCmdChooseMail(const char* szCommand){
	if(m_pCombo_Template && m_pCombo_Template->GetCount() > 0 &&m_pTxt_Content)
	{	
		ACString str;
		str.Format(GetStringFromTable(MAILTEMP_TABLE_INDEX + m_pCombo_Template->GetCurSel()),m_pLst_Friends->GetText(m_pLst_Friends->GetCurSel()),GetHostPlayer()->GetName());
		AUICTranslate trans;
		m_pTxt_Content->SetText(trans.Translate(str));
	}

}

void CDlgFrdCallBck::OnCmdSendMail(const char* szCommand){
	int selFriend = m_pLst_Friends->GetCurSel();
	if(m_pLst_Friends->GetCount() > 0 && selFriend >=0){
		g_pGame->GetGameSession()->friendCallBckMail_Send(m_pLst_Friends->GetItemData(selFriend),m_pCombo_Template->GetItemData(m_pCombo_Template->GetCurSel()));
		dwSendTime = GetTickCount();
		m_pBtn_Send->Enable(false);
	}
}

void CDlgFrdCallBck::OnEventButtonDBCLK(WPARAM wParam, LPARAM lParam, AUIObject * pObj){

}

void CDlgFrdCallBck::OnEventButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj){
	if(strstr(GetName(),"Win_MailtoFriend_Success")) return;

	POINT ptPos = pObj->GetPos();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	PAUILISTBOX pList = (PAUILISTBOX)pObj;
	if(pList->GetHitArea(x,y) == AUILISTBOX_RECT_TEXT){
		int curSel = m_pLst_Friends->GetCurSel();
		ACString friendName = _AL("");
		if(curSel < m_pLst_Friends->GetCount()){
			friendName = m_pLst_Friends->GetText(curSel);
			CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();
			int iTime = pMan->GetFriendLastCallBckTimeByID(m_pLst_Friends->GetItemData(m_pLst_Friends->GetCurSel()));
			if (iTime >= 0
				&& (g_pGame->GetServerGMTTime() - g_pGame->GetTimeZoneBias() * 60 + TIME_SECOND_BIAS) - iTime >= CDTIME_FRIENDCALLBCK
				&& iMailSentNum < MAX_FRIENDCALLBCKMAIL){
					m_pBtn_Send->Enable(true);
			}else{
				m_pBtn_Send->Enable(false);
			}	
		}
		else
			m_pBtn_Send->Enable(false);
		
		m_pLab_Receiver->SetText(friendName);
		ACString str;
		str.Format(GetStringFromTable(m_pCombo_Template->GetCurSel()),friendName,GetHostPlayer()->GetName());
		AUICTranslate trans;
		m_pTxt_Content->SetText(trans.Translate(str));
	}
}

void CDlgFrdCallBck::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj){

}


void CDlgFrdCallBck::OnCmdRefresh(const char* szCommand){
	RefreshFriend();
}

void CDlgFrdCallBck::OnCmdCancel(const char* szCommand){
	Show(false);
}

void CDlgFrdCallBck::OnCmdChooseFriend(const char* szCommand){
		int curSel = m_pLst_Friends->GetCurSel();
		ACString friendName = _AL("");
		if(curSel < m_pLst_Friends->GetCount()){
			friendName = m_pLst_Friends->GetText(curSel);
			CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();
			int iTime = pMan->GetFriendLastCallBckTimeByID(m_pLst_Friends->GetItemData(m_pLst_Friends->GetCurSel()));
			if (iTime >= 0
				&& (g_pGame->GetServerGMTTime() - g_pGame->GetTimeZoneBias() * 60 + TIME_SECOND_BIAS)- iTime >= CDTIME_FRIENDCALLBCK
				&& iMailSentNum < MAX_FRIENDCALLBCKMAIL){
					m_pBtn_Send->Enable(true);
			}else{
				m_pBtn_Send->Enable(false);
			}	
		}
		else
			m_pBtn_Send->Enable(false);
		
		m_pLab_Receiver->SetText(friendName);
		ACString str;
		str.Format(GetStringFromTable(m_pCombo_Template->GetCurSel()),friendName,GetHostPlayer()->GetName());
		AUICTranslate trans;
		m_pTxt_Content->SetText(trans.Translate(str));
}

void CDlgFrdCallBck::SetMailNum(int n){
	iMailSentNum = n;
	ACHAR  szLeft[10];
	a_sprintf(szLeft, _AL("%d/%d"), iMailSentNum, MAX_FRIENDCALLBCKMAIL);
	m_pLab_MailLeft->SetText(szLeft);
	//每次设置成功今天的发送邮件后，都记录一下今天
	iDays = ((g_pGame->GetServerGMTTime() - g_pGame->GetTimeZoneBias() * 60)+TIME_SECOND_BIAS)/(3600 * 24);
	
}


void CDlgFrdCallBck::IncreaseMailNum(){
	if(strstr(GetName(),"Win_MailtoFriend_Success")){
		iMailSucc++;
		return;
	}
	if(++iMailSentNum > MAX_FRIENDCALLBCKMAIL)
		iMailSentNum = MAX_FRIENDCALLBCKMAIL;
	ACHAR  szLeft[10];
	a_sprintf(szLeft, _AL("%d/%d"), iMailSentNum, MAX_FRIENDCALLBCKMAIL);
	m_pLab_MailLeft->SetText(szLeft);
}

void CDlgFrdCallBck::RefreshHint(){
	if (IsShow()) {
		if( GetTickCount() - dwSuccTime >= 2000)
			Show(false);
	}else if(iMailSucc > 0){
		iMailSucc--;
		dwSuccTime = GetTickCount();
		Show(true);
	}
}