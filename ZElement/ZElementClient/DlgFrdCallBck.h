// File    : DlgFrdCallBck.h
// Creator : Ma Yuanzheng
// Date    : 2010.11.5
// Comment : 好友召回系统界面
// Copyright (c) 2010 Zhuxian Studio, All Rights Reserved.

#pragma  once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUILabel.h"

#define MAX_FRIENDCALLBCKMAIL 5                     //每天发送召回邮件的上限
#define CDTIME_FRIENDCALLBCK  (3600 * 24 * 10)		  //冷冻期 秒数
#define CDDAYS_FRIENDCALLBCK  10                     //冷冻期 天数
#define MAILTEMPNUM           12                       //召回邮件的模板数
#define MAILTEMP_TABLE_INDEX  0                   //召回邮件模板内容在stf资源文件的起始位置
#define TIME_SECOND_BIAS	  (-(0 * 3600 + 0 * 60 + 0)) //测试用，0点后的几时几分几秒算新的一天

class CDlgFrdCallBck : public CDlgBase{


	
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()


public:

	enum{SENDER_AWARD = 1,
		BACKER_AWARD,
	};

	void BuildFriendsLst();				//创建好友列表
	void RefreshFriend();				//刷新好友列表（本地）


	void SetMailNum(int n);				//设置今天已经发送邮件的数量
	bool ReachMAXFrdCallBckMail();		//是否达到可发送邮件数上限
	void IncreaseMailNum();				//今天发送邮件记录加一，成功记录加一
	void RefreshHint();					//显示或消失“发送成功”提示

	void OnCmdChooseMail(const char* szCommand);
	void OnCmdSendMail(const char* szCommand);
	void OnCmdRefresh(const char* szCommand);
	void OnCmdCancel(const char* szCommand);
	void OnCmdChooseFriend(const char* szCommand);

	void OnEventButtonDBCLK(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	CDlgFrdCallBck();
	virtual ~CDlgFrdCallBck();


protected:

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual void DoDataExchange(bool bSave);

	//To do: add dialog control Object here
	PAUILABEL              m_pLab_Receiver;
	PAUILABEL              m_pLab_Title;
	PAUITEXTAREA           m_pTxt_Content;
	PAUILISTBOX            m_pLst_Friends;
	PAUISTILLIMAGEBUTTON   m_pBtn_Refresh;
	PAUISTILLIMAGEBUTTON   m_pBtn_Send;
	PAUICOMBOBOX           m_pCombo_Template;
	PAUILABEL			   m_pLab_MailLeft;

	int					   iMailSentNum;	 //  Callback Mail Sent Today 
	int                    iDays;			//  How many days passed since 1970.1.1 00:00:00
	int                    dwSendTime;      // for button enable
	int                    dwSuccTime;      // for success tip dialog show
	int					   iMailSucc;       // for success tip dialog show
	bool                   bAMailCanSend;
};
inline bool CDlgFrdCallBck::ReachMAXFrdCallBckMail(){return iMailSentNum >= MAX_FRIENDCALLBCKMAIL;}