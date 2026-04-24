/********************************************************************
	created:	2013/07/17
	created:	17:7:2013   19:53
	filename: 	e:\Works\ZElementClient\DlgCrssvrTeams.h
	file path:	e:\Works\ZElementClient
	file base:	DlgCrssvrTeams
	file ext:	h
	author:		zhuyan
	
	purpose:	战队信息
*********************************************************************/

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"

// 小图标
class DlgCrssvrTeamsMin  : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
public:
	DlgCrssvrTeamsMin();
	~DlgCrssvrTeamsMin();
	virtual bool OnInitDialog();

	void OnCommand_Click(const char * szCommand);
	void OnCommand_Group(const char * szCommand);
	void OnCommand_6V6(const char * szCommand);
};

// 小图标
class DlgCrssvrTeamsCreateInput  : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
public:
	DlgCrssvrTeamsCreateInput();
	~DlgCrssvrTeamsCreateInput();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	void OnCommand_OK(const char * szCommand);

private:
	PAUIEDITBOX* m_pEdit;
};

class CDlgCrssvrTeamsOptionName : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgCrssvrTeamsOptionName();
	virtual ~CDlgCrssvrTeamsOptionName();

	void OnCommand_Group(const char *szCommand);
	void OnCommand_Transfer(const char *szCommand);
	void OnCommand_Whisper(const char *szCommand);
	void OnCommand_Kickout(const char *szCommand);
};
struct TeamsGroupInfo
{
	int roleid;
	ACString name;
	unsigned short level;
	unsigned char occupation;
	int score;
	char camp;
	char isonline;
	int reserved1;
	int reserved2;
	int reserved3;
	int reserved4;
	int reserved5;
};

class SortCrssvrTeamsInfo
{
public:
	int nRold;
	SortCrssvrTeamsInfo(int nID) : nRold(nID){}
	bool operator ()(TeamsGroupInfo & p1, TeamsGroupInfo & p2)
	{
		if(p1.roleid == nRold)
		{
			return true;
		}
		else if(p2.roleid == nRold)
		{
			return false;
		}
		if(p1.camp < p2.camp)
		{
			return true;
		}
		else if(p1.camp == p2.camp)
		{
			if(p1.isonline > 0 && p2.isonline == 0)
			{
				return true;
			}
			else if(p1.isonline > 0 && p2.isonline > 0)
			{
				if(p1.level > p2.level)
				{
					return true;
				}
				else if(p1.level == p2.level)
				{
					if(p1.score > p2.score)
					{	
						return true;
					}
				}
			}
		}
		return false;
	}
};

// 战队信息
class DlgCrssvrTeamsGroupInfo  : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP()
public:
	typedef abase::vector<TeamsGroupInfo> vecTeamsGroupInfo;
	DlgCrssvrTeamsGroupInfo();
	~DlgCrssvrTeamsGroupInfo();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	void UpDateButtonShow();

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void	EnableObject(PAUIDIALOG pDlg, const char *pszCommand, bool bEnable);
	void OnCommand_Rank(const char * szCommand);
	void OnCommand_Exit(const char * szCommand);
	void OnCommand_ChangeGroupName(const char * szCommand);
	void OnCommand_Refresh(const char * szCommand);

	void OnCommand_Dismiss(const char * szCommand);
	void OnCommand_Recruit(const char * szCommand);
	void OnCommand_CreateGroup(const char * szCommand);
	
	void UpDateGroupInfoList(void* pData);
	void UpDateGroupInfoBase(void* pData);

	void AddTeamsGroupInfo( void* pInfo );

	void ClearData();

	TeamsGroupInfo* GetTeamsInfo(int nIndex);
	PAUILISTBOX  m_pGroupInfoList;
private:
	AUILabel			*m_pLab_GroupScore;	// 战队当前积分
	AUILabel			*m_pTxt_GroupScore;	// 战队当前积分
	AUILabel			*m_pTxt_FightTime;		// 竞技场  对战
	AUILabel			*m_pLab_FightTime;		// 竞技场  对战


	AUILabel			*m_pTxt_FightWin;		// 胜利
	AUILabel			*m_pLab_FightWin;	// 胜利
	AUILabel			*m_pTxt_FightLose;	// 失败
	AUILabel			*m_pLab_FightLose;		// 失败

	AUILabel			*m_pTxt_FightRate;	// 胜率
	AUILabel			*m_pLab_FightRate;		// 胜率

	AUILabel			*m_pTxt_GroupName;		// 战队名字

	PAUISTILLIMAGEBUTTON m_Btn_CreateGroup;	// 创建战队
	PAUISTILLIMAGEBUTTON m_Btn_ChangeName;	// 改变战队名称
	PAUISTILLIMAGEBUTTON m_Btn_Exit;	// 退出战队
	PAUISTILLIMAGEBUTTON m_Btn_Recruit;	// 接收队员
	PAUISTILLIMAGEBUTTON m_Btn_Dismiss;	// 解散战队
	PAUISTILLIMAGEBUTTON m_Btn_Refresh;	// 刷新

	vecTeamsGroupInfo m_vecTeamsInfo;
	A2DSprite* m_pImageStar;
	int m_nIconIndex;
	abase::vector<A2DSprite*> m_vecImageList;			// 动态生成显示的图片
};

// 战队搜索
class DlgCrssvrTeamsGroupRank  : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
public:
	DlgCrssvrTeamsGroupRank();
	~DlgCrssvrTeamsGroupRank();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	void OnCommand_Search(const char * szCommand);
	void OnCommand_Prev(const char * szCommand);
	void OnCommand_Next(const char * szCommand);

	void UpDateGroupRank(void* pData);
	void UpDateGroupSearchRank(void* pData);
	void UpDatePageText();
private:
	AUILabel			*m_pTxt_Page;				// 页码
	AUILabel			*m_pTxt_GroupRank;		// 竞技场  对战

	int m_nCurPage;
	int m_nMaxPage;
};
#define CRSSVRTEAMSMEMBER 6
// 战队主界面
class DlgCrssvrTeamsApply  : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
public:
	DlgCrssvrTeamsApply();
	~DlgCrssvrTeamsApply();
	virtual bool OnInitDialog();

	void UpdateCrssvrTeamsApplyInfo(void* pData); 
	void UpdateQueueUpState(int nRoleId);
	static void ApplyRenderCallback(const A3DRECT &rc, DWORD param1, DWORD param2,DWORD param3);
private:
	AUILabel			*m_pTxt_TeamNameA;		// A战队名字
	AUILabel			*m_pTxt_PlayerNameA_A[CRSSVRTEAMSMEMBER];
	PAUITEXTAREA	m_pTxt_ProfA[CRSSVRTEAMSMEMBER];

	PAUIIMAGEPICTURE		m_pImg_IconA_A[CRSSVRTEAMSMEMBER];	

	CECLoginPlayer* m_pPlayerA[CRSSVRTEAMSMEMBER];

	AUILabel			*m_pTxt_PlayerRankA;			// A组玩家排名
	AUILabel			*m_pTxt_PlayerScoreA;		// A组玩家积分
	AUILabel			*m_pTxt_PlayerFightA;			// A组玩家战斗力

	AUILabel			*m_pTxt_TeamNameB;		// B战队名字
	AUILabel			*m_pTxt_PlayerNameB_A[CRSSVRTEAMSMEMBER];	
	PAUIIMAGEPICTURE		m_pImg_IconB_A[CRSSVRTEAMSMEMBER];	
	PAUITEXTAREA	m_pTxt_ProfB[CRSSVRTEAMSMEMBER];

	AUILabel			*m_pTxt_PlayerRankB;			//B组玩家排名
	AUILabel			*m_pTxt_PlayerScoreB;		// B组玩家基本
	AUILabel			*m_pTxt_PlayerFightB;			// B组玩家战斗力
	CECLoginPlayer* m_pPlayerB[CRSSVRTEAMSMEMBER];
};

// 战队奖励
class DlgCrssvrTeamsAward  : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
public:
	DlgCrssvrTeamsAward();
	~DlgCrssvrTeamsAward();
	virtual bool OnInitDialog();
};

enum eWindowsType
{
	eWindowsType_Sub1,
	eWindowsType_Sub2,
	eWindowsType_Sub3,
	eWindowsType_Count,
};

// 战队主界面
class DlgCrssvrTeamsMain  : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
public:
	DlgCrssvrTeamsMain();
	~DlgCrssvrTeamsMain();
	virtual bool OnInitDialog();
	void OnShowDialog();
	virtual void OnHideDialog();
	void OnCommand_GroupInfo(const char * szCommand);
	void OnCommand_GroupApply(const char * szCommand);
	void OnCommand_Award(const char * szCommand);
	void SetDialog(int nIndex);
	void SetWindowsType(int val);
	void UpdateCrssvrTeams();
	void UpData(void* pData);
	void UpDataSlefScore(int nScore);
	void ClearData();
private:
	AUISubDialog		*m_pSubItems[eWindowsType_Count];

	AUILabel			*m_pTxt_GroupName;		// 战队名字
	AUILabel			*m_pLab_GroupScore;	// 战队积分
	AUILabel			*m_pTxt_GroupScore;	// 战队积分
	AUILabel			*m_pTxt_SelfScore;		// 战队个人积分
	AUILabel			*m_pLab_SelfScore;		// 战队个人积分


	//AUILabel			*m_pTxt_SelfFightTimeWeek;		// 本周对战场次
	//AUILabel			*m_pLab_SelfFightTimeWeek;	// 本周对战场次
	//AUILabel			*m_pTxt_SelfFightTimeSeason;	// 本赛季对战场次
	//AUILabel			*m_pLab_SelfFightTimeSeason;		// 本赛季对战场次

	AUILabel			*m_pTxt_SelfFightTimeAll;		// 竞技场对战
	AUILabel			*m_pLab_SelfFightTimeAll;	// 竞技场对战
	AUILabel			*m_pTxt_SelfWin;		// 胜利
	AUILabel			*m_pLab_SelfWin;		// 胜利


	AUILabel			*m_pTxt_SelfLose;		//失败
	AUILabel			*m_pLab_SelfLose;	// 失败
	AUILabel			*m_pTxt_SelfRate;		// 胜率
	AUILabel			*m_pLab_SelfRate;		// 胜率

	AUIImagePicture		*m_pImg_Icon;

	PAUITEXTAREA				m_pTxt_Rule;

	int m_nWindowsType;
};
