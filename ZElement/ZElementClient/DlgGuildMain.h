/********************************************************************
	created:	2013/07/25
	created:	25:7:2013   14:01
	filename: 	e:\Works\ZElementClient\CDlgGuildMain.h
	file path:	e:\Works\ZElementClient
	file base:	CDlgGuildMain
	file ext:	h
	author:		zhuyan
	
	purpose:	帮派界面
*********************************************************************/

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIProgress.h"
#include "DlgBaseExplorer.h"
class CDlgGuildInfoSub;

enum eGuildWindowsType
{
	eGuildWindowsType_Family,
	eGuildWindowsType_Guild,
	eGuildWindowsType_Guild_Info,
	eGuildWindowsType_Guild_Create,
	eGuildWindowsType_Guild_Shoop,
	eGuildWindowsType_Guild_BBS,
	eGuildWindowsType_Count
};

class CDlgGuildInfoSubItem : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
public:
	CDlgGuildInfoSubItem();
	~CDlgGuildInfoSubItem();
	virtual bool OnInitDialog();
	virtual bool Release();
	void UpDateItem(int nItemID);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void UpDateItemText(ACString szText);
	void SetSubGuildInfo(void* pData,int nType,ACString szTime);
	ACString				m_szGuilInfo;
	int m_nItemID;
private:
	CDlgGuildInfoSub*		m_pDlgGuildInfoSubMan;
	AUILabel			*m_pTxt_Description;					// 帮派等级
};

class CDlgGuildInfoSubItemExpand : public CDlgGuildInfoSubItem
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
public:
	CDlgGuildInfoSubItemExpand();
	~CDlgGuildInfoSubItemExpand();
	virtual bool OnInitDialog();
	void SetSubGuildInfoContent(ACString szContent,int nItemID);
	virtual void UpDateContent(ACString szText);
	void UpDateItemShow(int nItemID);
	void OnCommmandLink(const char* szCommand);

	void OnEventLButtonDownText(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventMouseMoveText(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

protected:

	// 寻径格式： &location(周一仙,300,250,240,1)& 类似任务描述
	void UpdateTextArea(AUITextArea*);

private:
	PAUITEXTAREA				m_pTxt;

	PAUISTILLIMAGEBUTTON m_Btn_Link;	// 创建战队

	PAUIIMAGEPICTURE		m_pImg_Item;

};

class CDlgGuildInfoSub : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
public:
	typedef abase::vector<AUISubDialog*> vecSubDialog;
	typedef abase::vector<CDlgGuildInfoSubItem*> vecDlgItems;

	CDlgGuildInfoSub();
	~CDlgGuildInfoSub();
	virtual bool OnInitDialog();
	virtual bool Release();
	bool Render();
	bool ExpandItem(CDlgGuildInfoSubItem* pDlg, bool bExpand = true);
	void InitItem(void* pData);
private:
	AUISubDialog*			m_pSubItem;			// 单个成就界面容器
	vecDlgItems				m_vecDlgItems;		// 对话框列表
	vecSubDialog			m_vecSubDialogs;	// 子对话框控件列表
	CDlgGuildInfoSubItemExpand*	m_pDlgItemExpand;	// 展开的成就界面
	A3DRECT					m_rcWindowOld;
};


class CDlgGuildInfo : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgGuildInfo();
	~CDlgGuildInfo();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	void OnCommand_Editor(const char * szCommand);
	void OnCommand_GetRankAward(const char * szCommand);
	void OnCommand_getmoney(const char* szCommand);
	void UpDateGuildInfo(void* pData);
	void BuildGuildWarRankList(void *pData);
	void UpdateGuildSiege(void* pData);
private:
	AUIImagePicture		*m_pImg_GuildIcon;		// 帮派头像
	AUIImagePicture		*m_pImg_Icon01;		// 帮战图标
	AUIImagePicture		*m_pImg_Icon02;		// 帮战图标

	PAUIPROGRESS			m_pPrg_GuildHonor;	// 帮派灵气进度条
	PAUIPROGRESS			m_pPrg_LandOccupy;	//帮派社稷掌控
	AUILabel			*m_pTxt_GuildLevel;					// 帮派等级
	AUILabel			*m_pTxt_GuildProsperity;			// 帮派建设度
	AUILabel			*m_pTxt_PplCap;						// 帮派家族数
	AUILabel			*m_pTxt_PplOnline;					// 帮派在线人数
	AUILabel			*m_pTxt_GuildHonor;				// 帮派灵气
	AUILabel			*m_pTxt_LandOccupy;				// 帮派社稷掌控


	AUILabel			*m_pTxt_GetGuildwarMoney;	// 帮战奖金
	//AUILabel			*m_pTxt_Gold;							// 帮派资金
	AUILabel			*m_pTxt_Money;						// 帮派元宝
	//AUILabel			*m_pTxt_GuildWarInfo;			// 帮战信息
	//AUILabel			*m_pTxt_GuildWarTime;			// 帮派越战时间
	//AUILabel			*m_pTxt_GuildWarStart;			// 帮派是否开战

	AUILabel			*m_pTxt_DefenderGuild;			// 守城帮派名称
	AUILabel			*m_pTxt_Defender;					// 守城帮主名称
	AUILabel			*m_pTxt_DefenderName;			// 守城方

	PAUITEXTAREA m_pTxt_Message;
	
		int	m_nGuildWarPage;
};

class CDlgGuildLandCreate : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgGuildLandCreate();
	~CDlgGuildLandCreate();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	void OnCommand_Create(const char * szCommand);
	void OnCommand_Up(const char * szCommand);
	void OnCommand_Down(const char * szCommand);
	void OnCommand_Close(const char * szCommand);
	void UpDateList(void* pData);
private:
	AUILabel			*m_pTxt_Page;		// 页码
	int m_nCurPage;
	int m_nMaxPage;
};

class CDlgGuildInputMoney: public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgGuildInputMoney();
	~CDlgGuildInputMoney();
	virtual bool OnInitDialog();
	void OnCommand_OK(const char * szCommand);
private:
};

struct Fac_BuildField
{
	int  nIndex;
	int  nId;
	int  nLevel;
	int  nStatus;
};

class CDlgGuildBuild : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgGuildBuild();
	~CDlgGuildBuild();
	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual void OnShowDialog();
	void OnCommand_EnterLand(const char * szCommand);
	void OnCommand_Donate(const char * szCommand);
	void OnCommand_ExitLand(const char * szCommand);
	void OnCommand_Edit(const char * szCommand);
	void OnCommand_GRule(const char * szCommand);
	void UpDateList();
	void UpDataMsg(void* pData);
	void UpDataEdit(ACString szMsg);
	void SetEnterBuild(bool val);
	void UpDateFacBaseInfo();
	void UpDataBuildInfo();

	void SetIsSend(bool val) { m_bIsSend = val; }
private:
	AUILabel			*m_pTxt_Grass;		// 草
	AUILabel			*m_pTxt_Mine;		// 矿
	AUILabel			*m_pTxt_MonsterFood;		// 兽粮
	AUILabel			*m_pTxt_MonsterCore;		// 妖核
	AUILabel			*m_pTxt_Money;				// 钱
	AUILabel			*m_pTxt_GActivity;				// 活跃度
	AUILabel			*m_pTxt_Task;				// 任务数量
	PAUIIMAGEPICTURE		m_pImg_WorldMap;
	AUILabel			*m_pLab_BuildStatus;				// 建设中建筑物名称
	AUILabel			*m_pLab_Prgs;						//完成度
	AUIImagePicture*		m_pIcon_build;				// 正在建筑图标
	AUIImagePicture		*m_pImg_Icon[18];		// 建筑物图标
	PAUITEXTAREA		m_pTxt_Message;
	PAUILISTBOX		m_pTaskList;				// list

	bool					m_bIsEnter;
	bool					m_bIsSend;
};

class CDlgGuildBBS : public CDlgBaseExplorer
{
public:
	CDlgGuildBBS() {}
	virtual ~CDlgGuildBBS() {}

	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

private:
	int	m_iTime;
	AString m_strToken;
	static const int ms_TimeCircle;
};


class CDlgGuildMain : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgGuildMain();
	~CDlgGuildMain();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	void OnCommand_Rdo(const char * szCommand);
	void UpDateRdo(int nIndex);
	void UpDataFactionCoupon(int nNum);
	void OnEventLButtonDownText(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventMouseMoveText(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
private:
	AUISubDialog		*m_pSubItems[eGuildWindowsType_Count];
	AUILabel			*m_pTxt_SelfContribution;		// 帮派个人贡献度
	AUILabel			*m_pLab_SelfContribution;		// 帮派个人贡献度
};